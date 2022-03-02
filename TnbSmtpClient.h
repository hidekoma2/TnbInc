#pragma once
/**
 *@file
 * SMTP(Simple Mail Transfer Protocol)関係のヘッダ
 *
 *	@note Winsock を使用しています。
 *
 *	@note	マクロ "_TnbWINSOCK2_ENABLE" が定義されていると、 Winsock2.0 を使用します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPop3Client.h"



//TNB Library
namespace TNB
{



/**@ingroup MAIL
 * SMTP(Simple Mail Transfer Protocol)プロトコル処理クラス
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		CPop3Client pop3;	//POP Before SMTP 用
 *		pop3.SetServerName(_T("Localhost"));
 *		pop3.SetUser(_T("master"), _T("master"));
 *		pop3.SetKind(pop3.APOP);
 *	
 *		CMailContent mail;	//メール内容
 *		mail.SetFromMailAddr("boss@tnb.co.jp", "ぼす");		
 *		mail.AddToMailAddr("oyabun@tnb.co.jp", "おやぶん");
 *		mail.AddCcMailAddr("master@tnb.co.jp", "マスター");
 *		mail.SetSubject("例の件");
 *		mail.AddSendTextData("こんにちわ\r\n例の件、どうですか？");		
 *
 *		CSmtpClient smtp;
 *		smtp.SetServerName(_T("localhost"));
 *		smtp.SetDomain(_T("boss@tnb.co.jp"));
 *		smtp.SetPopBeforeMode(&pop3);
 *		smtp.Send(mail);
 *	</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbSmtpClient.h
 * 
 *	@date 06/09/28 暫定作成(とりあえず、送れる)
 *	@date 06/09/29 POP Before SMTP 、追加。
 *	@date 06/10/03 BASE64 変換対応。テキストだけならメーラとして使用可能に。
 *	@date 06/10/05 マルチパーツ対応。
 *	@date 07/06/15 通信クラスを交換できるよう対応
 *	@date 08/07/31 ExtendedHelloMode() を追加。
 *	@date 09/09/15	親クラスの整理に追従。
 */
class CSmtpClient : public CAbstractMailClient
{
	DEFSUPER(CAbstractMailClient);
public:

	/// コンストラクタ
	CSmtpClient(void) : _super(), m_pPop3(NULL), m_isEhloMode(true)
	{
	}

	/// デストラクタ
	virtual ~CSmtpClient(void)
	{
		Disconnect();
	}

	/**
	 * [設定] ExtendedHelloMode設定
	 *	@note デフォルトでは true （EHLOが使われる）です。
	 *	@param enable true なら EHLO 、 false なら HELO が使われます。
	 */
	void SetExtendedHelloMode(bool enable)
	{
		m_isEhloMode = enable;
	}

	/**
	 * [取得] 接続成功時の文字列.
	 *	@note	Connectに成功すると、文字が送られてくる。それをこの関数で得ることが可能。
	 *			Disconnectしても次接続するまで同じ文字列が得られます
	 *	@return 文字列
	 */
	CAscii GetOpeningMessage(void) const
	{
		return m_ascOpeningMessage;
	}

	/**
	 * [設定] 通信設定.
	 *	@note Winsock を使用して通信を行なうために、サーバ名、ポート番号を指定します。
	 *	@note 本メソッドを実行すると SetReportInterface() の設定は無効になります。
	 *	@param lpszName サーバ名。
	 *	@param wPort ポートナンバー。省略すると25。
	 */
	void SetServerName(LPCTSTR lpszName, WORD wPort = 25)
	{
		_super::SetServerName(lpszName, wPort);
	}

	/**
	 * [設定] 送信名設定
	 *	@note 送信のために必須な設定です。
	 *	@param lpszDomain ドメイン名（HELOコマンドで使用）。
	 */
	void SetDomain(LPCTSTR lpszDomain)
	{
		m_ascDomain = lpszDomain;
	}

	/**
	 * [設定] POP Before SMTP 設定
	 *	@param pPop3 認証設定した、POP3クライアント。NULLなら設定解除。
	 *	@param dwWait 認証後のWait(ms)。省略すると300msになります。
	 */
	void SetPopBeforeMode(CPop3Client* pPop3, DWORD dwWait = 300)
	{
		m_pPop3 = pPop3;
		m_dwPop3Wait = dwWait;
	}

	/**
	 * [設定] 送信者アドレス設定
	 *	@attention Send() で適用する CMailContent クラスで設定されているものと異なる送信者にしたい場合のみに使用します。
	 *	@param lpszFromMailAddr アドレス
	 */
	void SetFromMailAddr(LPCTSTR lpszFromMailAddr)
	{
		m_ascFromMailAddr = lpszFromMailAddr;
	}
	
	/**
	 * [設定] 宛先名設定初期化
	 */
	void InitToMailAddr(void)
	{
		m_vascToMailAddrs.RemoveAll();
	}

	/**
	 * [設定] 宛先名設定
	 *	@attention Send() で適用する CMailContent クラスで設定されているものと異なる宛先に送りたい場合のみに使用します。
	 *	@param lpszToAddr 宛先アドレス(Ascii文字のみ)。
	 */
	void AddToAddr(LPCTSTR lpszToAddr)
	{
		m_vascToMailAddrs.Add(lpszToAddr);
	}

	/**
	 * [送信] メール送信.
	 *		メーラの設定、送信名、宛先など、先に設定しておく必要があります。
	 *	@param mail 送信内容。送信名、宛先など、設定完了しているオブジェクトを指定します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Send(const CMailContent& mail)
	{
		//情報はそろっている？
		if ( ! mail.IsCompletesForSend() )
		{
			SetErrorText("未設定項目有り");
			return false;
		}
		//POP Before SMTP
		if ( m_pPop3 != NULL )
		{
			if ( ! m_pPop3->Connect() )
			{
				SetErrorText(m_pPop3->GetErrorText());
				return false;
			}
			m_pPop3->Disconnect();
			::Sleep(m_dwPop3Wait);
		}
		//本体取り出しておく
		CAscii ascMain = mail.GetContentForSend();
		//接続
		if ( ! IsConnect() )
		{
			if ( ! Connect() )
			{
				return false;
			}
		}
		//MAIL FROMコマンド発行
		CAscii ascFromAddr = m_ascFromMailAddr;
		if ( ascFromAddr.IsEmpty() )
		{
			ascFromAddr = mail.ReferFromMailAddr().GetAddress();
		}
		if ( ! Command(CAscii::Fmt("MAIL FROM:<%s>", ascFromAddr)) )
		{
			return false;
		}
		//RCPT TOコマンド発行
		CAsciiVector vascToAddrs = m_vascToMailAddrs;
		if ( vascToAddrs.IsEmpty() )
		{
			vascToAddrs = mail.GetAddrsForSend();
		}
		loop ( i, vascToAddrs.GetSize() )
		{
			if ( ! Command(CAscii::Fmt("RCPT TO:<%s>", vascToAddrs[i])) )
			{
				return false;
			}
		}
		//DATAコマンド発行
		if ( ! Command("DATA") )
		{
			return false;
		}
		//本体送信
		return Command(ascMain);
	}

protected:

	/**
	 * [確認] レスポンスチェック
	 *	@param a 受信した文字列
	 *	@retval true 正常．
	 *	@retval false 異常
	 */
	virtual bool IsValidResponse(const CAscii& a) const
	{
		if ( a.IsEmpty() ) 
		{
			return false;
		}
		return (a[0] == '2' || a[0] == '3');
	}

	/**
	 * [設定] 終了コマンド
	 *	@note Command() を使うことを想定。
	 *	@param[out] _res レスポンス文字が格納。
	 *	@retval true 成功。
	 *	@retval false 失敗（理由は GetErrorText() で取得可能.）。
	 */
	virtual bool QuitCommand(CAscii& _res)
	{
		return Command(_res, "QUIT");
	}

	/**
	 * [設定] 接続時処理
	 *	@note 通常、認証処理を行なう
	 *	@retval Empty状態 成功。
	 *	@retval 文字有り エラー。文字列は GetErrorText() で取得可能になる。
	 */
	virtual bool OnConnect(void)
	{
		CAscii s;
		if ( Read(s) && s.Find("220") == 0 )
		{
			m_ascOpeningMessage = s;
			CAscii hel = m_isEhloMode ? "EHLO" : "HELO";
			if ( Command(CAscii::Fmt("%s %s", hel, m_ascDomain)) )
			{
				//数行来るが空読みしておく
				while ( true )
				{
					if ( ! Read(s) )
					{
						break;
					}
				}
				return true;
			}
		}
		return false;
	}

private:

	CAscii					m_ascOpeningMessage;///< 接続成功時の文字
	CAscii					m_ascDomain;		///< ドメイン名
	CAscii					m_ascFromMailAddr;	///< 送信者メールアドレス(強制設定用)
	CAsciiVector			m_vascToMailAddrs;	///< 宛先メールアドレス(強制設定用)
	CPop3Client*			m_pPop3;			///< POP Before SMTP
	DWORD					m_dwPop3Wait;		///< 認証後のWait
	bool					m_isEhloMode;		///< true;EHLO false HELO 使用
};



}; // TNB



#if 0

クライアント	HELO <satoh@some.where.com><CRLF>
サーバ	250 OK
クライアント	MAIL FROM:<satoh@some.where.com><CRLF>
サーバ	250 OK
	
クライアント	RCPT TO:<smith@another.com> <CRLF>
サーバ	250 OK
	
クライアント	DATA <CRLF>
サーバ	354 Start mail input; end with <CRLF>.<CRLF>
クライアント	こんにちは
クライアント	ところ、・・・ということで、
クライアント	返事をおまちしております
クライアント	<CRLF>.<CRLF>
サーバ	250 OK
クライアント	QUIT<CRLF>
サーバ	250 OK

	send.print("From: "+sender+CRLF);
	send.print("To: " + receiver+CRLF);
	send.print("Subject: " + title+CRLF);
	send.print("Comment: Unauthenticated sender"+CRLF);
	send.print("X-Mailer: Tiny SMTP Program"+CRLF);
	send.print(CRLF);
	send.print(message+CRLF);
	send.print(".");
	send.print(CRLF);
	send.flush();
#endif

#if 0
・一般的なメールエンコードは、Subject が BASE64、本文が JIS になって
    いるように思う。
    Outlook Express では、エンコードなしの S-JIS のままでも
    Subject、本文が BASE64 でも問題なく読める。
    インターネットへ送信するメールは、少なくとも BASE64 のエンコードは
    した方が安全だろう。

  ・メールヘッダのBASE64 Subject 形式は、次のとおりである。

    =?コード形式?タイプ?タイトル?=
    | |         ||     ||       |_ ?= 終了コード
    | |         ||     ||_ エンコードタイトル文字列
    | |         ||     |_ ? 区切りコード
    | |         ||_ BASE64 は「B」
    | |         |_ ? 区切りコード
    | |_ BASE64 は「iso-2022-jp」
    |_ =? 開始コード

  ・メール本文のBASE64 エンコード指定は、次のとおりである。

    MIME-Version: 1.0
    Content-Type: text/plain;
    	charset="iso-2022-jp"
    Content-Transfer-Encoding: base64
#endif