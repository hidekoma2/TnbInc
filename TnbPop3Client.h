#pragma once
/**
 *@file
 * POP3(Post Office Protocol version 3)関係のヘッダ
 *
 *	@note Winsock を使用しています。
 *
 *	@note	マクロ "_TnbWINSOCK2_ENABLE" が定義されていると、 Winsock2.0 を使用します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSocket.h"
#include "TnbMailContent.h"
#include "TnbMd5Computer.h"
#include "TnbQueueingReporter.h"
#include "TnbListQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup MAIL
 * Mail 用クライアントベースクラス
 *
 *	@note そのまま使うことはありません。
 *
 *	@par必要ファイル
 *			TnbPop3Client.h
 * 
 *	@date 06/09/28 CPop3Client を抽象化して作成。
 *	@date 07/06/15 通信クラスを交換できるよう対応
 *	@date 08/07/31 送信したコマンドのコールバックを追加
 *	@date 09/09/15 TnbStream クラスから QueueingReporter クラスへ換装。メソッドの整理。
 */
class CAbstractMailClient
{
public:

	/**@ingroup MAIL
	 * Mail用リスナーインターフェース
	 *
	 *		サーバからのレスポンス文字列を拾うことが出来ます。
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}
		/**
		 * [通知] レスポンス受信.
		 *	@param lpszRes レスポンス文字列
		 */
		virtual void OnMailResponse(LPCSTR lpszRes) = 0;
		/**
		 * [通知] コマンド送信.
		 *	@param lpszCmd コマンド文字列
		 */
		virtual void OnMailCommand(LPCSTR lpszCmd) = 0;
	};


	//---------------------------------


	/// コンストラクタ
	CAbstractMailClient(void) : m_socket(false), m_piRep(&m_socket), m_pListener(NULL), m_responseTimeout(5000)
	{
		m_queue.SetEnvironment(m_piRep, &m_inner);
	}

	/// デストラクタ
	virtual ~CAbstractMailClient(void)
	{
		Disconnect();
	}

	/**
	 * [取得] エラー文字列
	 *	@return 文字列
	 */
	LPCSTR GetErrorText(void) const
	{
		return m_ascErrorText;
	}

	/**
	 * [設定] リスナー設定
	 *	@param pListener リスナー。省略するとリスナー解除になります。
	 */
	void SetListener(IListener* pListener = NULL)
	{
		m_pListener = pListener;
	}

	/**
	 * [設定] 通信設定.
	 *	@note Winsock を使用して通信を行なうために、サーバ名、ポート番号を指定します。
	 *	@note 本メソッドを実行すると SetReportInterface() の設定は無効になります。
	 *	@param lpszName サーバ名。
	 *	@param wPort ポートナンバー。
	 */
	void SetServerName(LPCTSTR lpszName, WORD wPort)
	{
		Disconnect();
		m_socket.SetServer(CSocketAddress(wPort, lpszName));
		SetReportInterface(&m_socket);
	}

	/**
	 * [設定] 通信設定
	 *	@note 指定の IReport を使用して通信を行ないます。
	 *	@note 本メソッドを実行すると SetServerName() の設定は無効になります。
	 *	@param pReport 通信インターフェース。{@link IReport::Start() Start()}
	 *				で通信開始できるように設定しておく必要があります。
	 */
	void SetReportInterface(IReport* pReport)
	{
		Disconnect();
		m_piRep = (pReport != NULL) ? pReport : m_piRep;
		m_inner.Empty();
		m_queue.SetEnvironment(m_piRep, &m_inner);
	}

	/**
	 * [設定] レスポンス待ち時間.
	 *	レスポンスを待つ時間を設定できます。デフォルトでは 5000ms です。
	 *	@param tm レスポンス待ち時間。
	 */
	void SetResponseTimeout(DWORD tm)
	{
		m_responseTimeout = tm;
	}

	/**
	 * [切断] メールサーバ切断.
	 *		"QUIT"を投げ、Socketを閉じます。
	 *	@note 接続してなければ何もしません。
	 *	@retval true 正常切断。
	 *	@retval false 異常切断（理由は GetErrorText() で取得可能.）。
	 */
	bool Disconnect(void)
	{
		bool r = true;
		if ( m_piRep->IsConnect() )
		{
			CAscii a;
			r = QuitCommand(a);
			loop ( i, 10 )
			{
				if ( ! m_piRep->IsConnect() )
				{
					break;
				}
				::Sleep(100);
			}
			m_piRep->Stop();
			m_inner.Empty();
		}
		return r;
	}

	/**
	 * [接続] メールサーバ接続
	 *	@note サーバに接続し、ユーザ認証まで行ないます。
	 *	@retval true 接続成功。
	 *	@retval false 接続失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool Connect(void)
	{
		Disconnect();
		m_ascErrorText.Empty();
		if ( ! m_piRep->Start() )
		{
			SetErrorText("Server Not Found.");
			return false;
		}
		if ( ! OnConnect() )
		{
			m_piRep->Stop();
			return false;
		}
		return true;
	}

protected:

	/**
	 * [確認] レスポンスチェック
	 *	@param a 受信した文字列
	 *	@retval true 正常．
	 *	@retval false 異常
	 */
	virtual bool IsValidResponse(const CAscii& a) const = 0;

	/**
	 * [設定] 終了コマンド
	 *	@note Command() を使うことを想定。
	 *	@param[out] _res レスポンス文字が格納。
	 *	@retval true 成功。
	 *	@retval false 失敗（理由は GetErrorText() で取得可能.）。
	 */
	virtual bool QuitCommand(CAscii& _res) = 0;

	/**
	 * [設定] 接続時処理
	 *	@note 通常、認証処理を行なう
	 *	@retval Empty状態 成功。
	 *	@retval 文字有り エラー。文字列は GetErrorText() で取得可能になる。
	 */
	virtual bool OnConnect(void) = 0;

	/**
	 * [受信] レスポンス受信
	 *	@param[out] _res 受信文字列が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。文字列は GetErrorText() で取得可能になる。
	 */
	bool Read(CAscii& _res)
	{
		bool r = m_inner.Read(_res, m_responseTimeout);
		if ( r )
		{
			TRACE1( "Mail-Command Response = [%s]\n", _res );
			if ( m_pListener != NULL )
			{
				m_pListener->OnMailResponse(_res);
			}
		}
		else
		{
			SetErrorText("Timeout.");
		}
		return r;
	}

	/**
	 * [送信] コマンド送信
	 *	@param lpszCmd コマンド。改行無し。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Write(LPCSTR lpszCmd)
	{
		if ( m_piRep != NULL )
		{
			CAscii a = lpszCmd;
			a += "\r\n";
			LPCSTR lpsz = a;
			return m_piRep->Send(a.GetLength(), lpsz);
		}
		return false;
	}

	/**
	 * コマンド送信＆レスポンス受信
	 *	@param[out] _res レスポンス文字列が格納されます。
	 *	@param[in] lpszCmd コマンド(改行はいりません)
	 *	@retval true 成功。
	 *	@retval false 失敗。文字列は GetErrorText() で取得可能になる。
	 */
	bool Command(CAscii& _res, LPCSTR lpszCmd)
	{
		Write(lpszCmd);
		if ( m_pListener != NULL )
		{
			m_pListener->OnMailCommand(lpszCmd);
		}
		if ( ! Read(_res) )
		{
			return false;
		}
		if ( ! IsValidResponse(_res) )
		{
			SetErrorText(_res); //受信文字列をエラー文字列とする
			_res.Empty();
			return false;
		}
		return true;
	}

	/**
	 * コマンド送信＆レスポンス受信
	 *	@note レスポンスの文字列が要らない場合、これを使います。
	 *	@param[in] lpszCmd コマンド(改行はいりません)
	 *	@retval true 成功。
	 *	@retval false 失敗。文字列は GetErrorText() で取得可能になる。
	 */
	bool Command(LPCSTR lpszCmd)
	{
		CAscii s;
		return Command(s, lpszCmd);
	}

	/**
	 * [読込] ピリオド行まで読み込み
	 *	@param[out] _lines レスポンス文字。ピリオド行は含みません。
	 *	@retval true 成功。
	 *	@retval false 失敗。文字列は GetErrorText() で取得可能になる。
	 */
	bool ReadLines(CAsciiVector& _lines)
	{
		_lines.RemoveAll();
		CAscii res;
		while ( true )
		{
			bool r = m_inner.Read(res, m_responseTimeout);
			if ( ! r )
			{
				SetErrorText("Timeout.");
				return false;
			}
			if ( res == "." )
			{
				break;
			}
			_lines.Add(res);
		}
		return true;
	}

	/**
	 * [確認] 接続確認
	 *	@retval true 成功。
	 *	@retval false 失敗。文字列は GetErrorText() で取得可能になる。
	 */
	bool IsConnect(void)
	{
		if ( ! m_piRep->IsConnect() )
		{
			m_ascErrorText = "Server Not Connected.";
			return false;
		}
		return true;
	}

	/**
	 * [設定] エラー文字列
	 *	@param lpsz 文字列
	 */
	void SetErrorText(LPCSTR lpsz)
	{
		m_ascErrorText = lpsz;
	}

private:
	// インナークラス
	class CInner : public CQueueingReporter::IListener
	{
	public:
		/// [削除] バッファクリア
		void Empty(void)
		{
			m_recvLines.RemoveAll();
		}

		/**
		 * [取得] 受信情報読み込み.
		 *	@param[out] _res 受信情報が格納されます。
		 *	@param timeout 情報がない場合、最大待つ時間(ms)
		 */
		bool Read(CAscii& _res, DWORD time)
		{
			CTickCount tc;
			do
			{
				if ( ! m_recvLines.IsEmpty() )
				{
					_res = m_recvLines.Take();
					return true;
				}
				::Sleep(100);
			}
			while ( ! tc.IsPassed(time) );
			return false;
		}
	protected:
		// イベント
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			ASSERT( ! ev.HasData() );
			return true;
		}
		// jyy真
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c)
		{
			if ( c.IsEmpty() )
			{
				return 0;
			}
			BYTE b = c.At(0);
			if ( b == 0 || b == 0x0A )
			{
				return 1;
			}
			loop ( i, c.GetSize() )
			{
				b = c.At(i);
				if ( b == 0 || b == 0x0D )
				{
					CAscii a;
					LPSTR lpsz = a.GetBuffer(i + 1);
					loop ( j, i )
					{
						lpsz[j] = c.At(j);
					}
					lpsz[i] = 0;
					a.ReleaseBuffer();
					m_recvLines.Add(a);
					return i + 1;
				}
			}
			return 0;
		}
	private:
		CListQueueT<CAscii> m_recvLines;	///< 受信情報
	};

	CSocketClient		m_socket;			///< ソケット
	IReport*			m_piRep;			///< 通信ＩＦ
	CQueueingReporter	m_queue;			///< 受信キュー
	CInner				m_inner;			///< 内部受信クラス
	CAscii				m_ascErrorText;		///< エラー文字
	IListener*			m_pListener;		///< リスナー
	DWORD				m_responseTimeout;	///< レスポンスタイムアウト(ms)
};



/**@ingroup MAIL
 * POP3(Post Office Protocol version 3)クライアント処理クラス
 *
 *	@par使用例
 *	\code
 *		CPop3Client pop3;
 *		// サーバ設定
 *		pop3.SetServerName(_T("Localhost"));
 *		pop3.SetUser(_T("master"), _T("master"));
 *		pop3.SetKind(pop3.APOP);
 *			;
 *		// 接続
 *		pop3.Connect();
 *		TRACE1("[%s]\n", pop3.GetOpeningMessage());
 *			;
 *		CPop3Client::TParam t = pop3.GetMailCount();	//メール数取得		
 *		CPop3Client::CParamVector vp = pop3.GetMailList();	// 各メールのNo＆サイズ
 *		CAscii ascUIDL = pop3.GetUidl(1); // No1のメールのUIDL取得
 *		CAsciiVector v = pop3.GetMailContent(1); // No1のメールの内容取得
 *		loop ( i, v.GetSize() )
 *		{
 *			CJisStr s;
 *			s.SetOfJis(v[i]);
 *			TRACE1("%s\n", s.ToAscii());
 *		}
 *		CMailContent mail;
 *		mail.SetContentFromReceive(v); // 解析
 *	\endcode
 *
 *	@par必要ファイル
 *			TnbPop3Client.h
 * 
 *	@date 06/09/27	新規作成
 *	@date 06/09/28	抽象化したクラスを使うように変更。
 *	@date 07/06/15	通信クラスを交換できるよう対応
 *	@date 09/09/15	親クラスの整理に追従。
 */
class CPop3Client : public CAbstractMailClient
{
	DEFSUPER(CAbstractMailClient);
public:
	
	/// 認証種類型
	enum EKind
	{
		POP3,	///< POP3
		APOP	///< APOP
	};

	/// POP3プロトコル処理クラス用リザルト型
	struct TParam
	{
		int number;	///< メールナンバー
		int size;	///< サイズ
	};

	typedef CVectorT<TParam> CParamVector;	///< リザルトパラメータの配列型宣言


	//----------------------------------


	/**
	 * コンストラクタ
	 *	@param kind 認証方法。省略すると POP3です( SetKind() で再設定することも可能)。
	 */
	CPop3Client(EKind kind = POP3) : _super(), m_kind(kind) 
	{
	}

	/// デストラクタ
	virtual ~CPop3Client(void)
	{
		Disconnect();
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
	 *	@param port ポートナンバー。省略すると110。
	 */
	void SetServerName(LPCTSTR lpszName, WORD port = 110)
	{
		_super::SetServerName(lpszName, port);
	}

	/**
	 * [設定] ユーザ設定
	 *	@param lpszUser ユーザ名。
	 *	@param lpszPassword パスワード。
	 */
	void SetUser(LPCTSTR lpszUser, LPCTSTR lpszPassword)
	{
		m_ascUser = lpszUser;
		m_ascPassword = lpszPassword;
	}

	/**
	 * [設定] 認証方法設定
	 *	@param kind 認証方法。
	 */
	void SetKind(EKind kind)
	{
		m_kind = kind;
	}

	/**
	 * [要求] NOOP送信
	 *	@note	接続確認として使えます。
	 *	@retval true 成功。
	 *	@retval false 失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool RequestNoop(void)
	{
		if ( IsConnect() )
		{
			return Command("NOOP");
		}
		return false;
	}

	/**
	 * [削除] メール削除要求
	 *	@note	指定の numberのメール削除要求を出します。実際削除されるのは、 Disconnect したときです。
	 *			その前に、 CancelDelete() を使うことで、削除を取り消すことが出来ます。
	 *	@param number ナンバー。 GetMailList() の戻り配列の TParam::number のいずれかが指定可能です。
	 *	@retval true 要求成功。
	 *	@retval false 要求失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool RequestDelete(int number)
	{
		if ( IsConnect() )
		{
			return Command(CAscii::Fmt("DELE %d", number));
		}
		return false;
	}

	/**
	 * [削除] メール削除要求取り消し
	 *	@note	接続してから今までの RequestDelete() を全てキャンセルします。
	 *	@retval true 要求取り消し成功。
	 *	@retval false 要求取り消し失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool CancelDelete(void)
	{
		if ( Connect() )
		{
			return Command("RSET");
		}
		return false;
	}

	/**
	 * [取得] メール数取得
	 *	@note サーバのメール数、サイズを取得します。
	 *	@param[out] _param 取得情報が格納されます。 TParam::number にメール数、 TParam::size にサイズが格納。
	 *	@retval true 成功。
	 *	@retval false 失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool GetMailCount(TParam& _param)
	{
		_param.number = 0;
		_param.size = 0;
		if ( IsConnect() )
		{
			CAscii res;
			if ( Command(res, "STAT") )
			{
				m_GetTwoInt(_param, res, 4);
				return true;
			}
		}
		return false;
	}

	/**
	 * [取得] メールリスト取得
	 *	@note サーバにあるメールナンバー、及びサイズのリストを取得します。
	 *	@param[out] _list メールリスト。各要素の number にメールナンバー、size にサイズを格納。
	 *	@retval true 成功。
	 *	@retval false 失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool GetMailList(CParamVector& _list)
	{
		_list.RemoveAll();
		if ( IsConnect() )
		{
			CAscii res;
			if ( Command(res, "LIST") )
			{
				size_t c = res.ToInt(4);
				CAsciiVector va;
				if ( ReadLines(va) )
				{
					if ( c == va.GetSize() )
					{
						loop ( i, va.GetSize() )
						{
							TParam t;
							m_GetTwoInt(t, va[i], 0);
							_list.Add(t);
						}
						return true;
					}
					SetErrorText("invalid list"); // 要求の 数取得できなかった。
				}
			}
		}
		return false;
	}

	/**
	 * [取得] メール取得
	 *	@param[out] _mail 取得したメール内容が格納されます。
	 *	@param[in] number ナンバー。 GetMailList() の戻り配列の TParam::number のいずれかが指定可能です。
	 *	@param[in] line 取り出し行。省略すると、全て得ます。
	 *	@retval true 成功。
	 *	@retval false 失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool GetMailContent(CAsciiVector& _mail, int number, int line = 0)
	{
		_mail.RemoveAll();
		if ( IsConnect() )
		{
			CAscii cmd;
			if ( line <= 0 )
			{
				cmd.Format("RETR %d", number);
			}
			else
			{
				cmd.Format("TOP %d %d", number, line);
			}
			if ( Command(cmd) )
			{
				return ReadLines(_mail);
			}
		}
		return false;
	}

	/**
	 * [取得] UIDL取得
	 *	@param[out] _uidl 取得した UIDL 文字列が格納されます。
	 *	@param[in] number ナンバー。 GetMailList() の戻り配列の TParam::number のいずれかが指定可能です。
	 *	@retval Empty状態 失敗（理由は GetErrorText() で取得可能.）。
	 *	@retval true 成功。
	 *	@retval false 失敗（理由は GetErrorText() で取得可能.）。
	 */
	bool GetUidl(CAscii& _uidl, int number)
	{
		_uidl.Empty();
		if ( IsConnect() )
		{
			CAscii cmd;
			cmd.Format("UIDL %d", number);
			CAscii res;
			if ( Command(res, cmd) )
			{
				if ( res.ToInt(4) == number )
				{
					INT_PTR p = res.ReverseFind(' ');
					_uidl = res.Mid(p + 1);
					return true;
				}
				SetErrorText("invalid uidl"); // 要求の uidl と異なる。
			}
		}
		return false;
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
		return a.Find("+OK") == 0;
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
		bool r = Read(s);
		if ( r && s.GetAt(0) == '+' )
		{
			m_ascOpeningMessage = s.Mid(4);
			if ( m_kind == POP3 )
			{
				//== POP3
				if ( Command("USER " + m_ascUser) )
				{
					if ( Command("PASS " + m_ascPassword) )
					{
						return true;
					}
				}
			}
			else
			{
				//== APOP
				INT_PTR is = s.Find('<');
				INT_PTR ie = s.Find('>', is);
				if ( is >= 0 && ie > is )
				{
					CAscii a = s.Mid(is, ie - is + 1);
					CAscii h = CMd5Computer::ComputeHash(a + m_ascPassword).GetString();
					if ( Command(CAscii::Fmt("APOP %s %s", m_ascUser, h)) )
					{
						return true;
					}
				}
			}
		}
		return false;
	}

private:

	CAscii		m_ascOpeningMessage;	///< 接続成功時の文字
	CAscii		m_ascUser;				///< ユーザ名
	CAscii		m_ascPassword;			///< パスワード
	EKind		m_kind;					///< 認証種類

	/**
	 * 文字列解析.
	 *	@note 指定位置から 「数値、スペース、数値」となっている文字列から数値２つを取り出します。
	 *	@param[out] _param 解析結果が格納されます。
	 *	@param[in] asc 元文字列
	 *	@param[in] iOffset 解析開始位置
	 */
	void m_GetTwoInt(TParam& _param, const CAscii& asc, int iOffset)
	{
		INT_PTR p = asc.ReverseFind(' ');
		_param.number = asc.ToInt(iOffset);
		_param.size = asc.ToInt(p + 1);
	}

};



}; // TNB



#if 0
USER・PASS コマンドによってユーザ認証を行う。 
LIST コマンドでメール一覧を受け取る。 
それから各メールについて、 
RETR コマンドでメールの内容を受け取り、 
DELE コマンドでメールをサーバから削除する。 
QUIT コマンドで通信終了


サーバ			<wait for connection on TCP port 110>
クライアント	<open connection>
サーバ			+OK POP3 server ready <1896.697170952@dbc.mtview.ca.us>
--
クライアント	USER メールアカウント名<CRLF>
サーバ			+OK <CRLF>
クライアント	PASS パスワード<CRLF>
サーバ			+OK <CRLF>
--
クライアント	APOP mrose c4c9334bac560ecc979e58001b3e22fb
サーバ			+OK mrose's maildrop has 2 messages (320 octets)
--

クライアント	STAT
サーバ			+OK 2 320
クライアント	LIST
サーバ			+OK 2 messages (320 octets)
サーバ			1 120
サーバ			2 200
サーバ			.

クライアント	RETR 1
サーバ			+OK 120 octets
サーバ			<the POP3 server sends message 1>
サーバ			.

クライアント	DELE 1
サーバ			+OK message 1 deleted
クライアント	RETR 2
サーバ			+OK 200 octets
サーバ			<the POP3 server sends message 2>
サーバ			.
クライアント	DELE 2
サーバ			+OK message 2 deleted

クライアント	QUIT
サーバ			+OK dewey POP3 server signing off (maildrop empty)
クライアント	<close connection>
サーバ			<wait for next connection>
#endif

