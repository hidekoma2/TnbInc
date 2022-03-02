#pragma once
/**
 *@file
 * メールコンテンツ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"
#include "TnbStrOperator.h"
#include "TnbJisStr.h"
#include "TnbBase64Str.h"
#include "TnbTime.h"
#include "TnbFile.h"
#include "TnbMap.h"
#include "TnbStrEx.h"



//TNB Library
namespace TNB
{



/**@ingroup MAIL
 * メールアドレス管理クラス
 *
 *	@note 一組のメールアドレスと名前を管理しています。
 *
 *	@note 複製可能です。
 *
 *	@par必要ファイル
 *			TnbMailContent.h
 * 
 *	@date 06/10/06 新規。
 *	@date 08/07/30 Sender表記サポート。
 */
class CMailAddress
{
	CAscii		m_ascAddress;		///< アドレス(ASCIIのみ)
	CAscii		m_ascName;			///< なまえ
public:

	/// コンストラクタ
	CMailAddress(void) 
	{
	}

	/**
	 * コンストラクタ
	 *	@param ascAddress アドレス(ASCII)
	 *	@param ascName 名前。省略可能。
	 */
	CMailAddress(CAscii ascAddress, CAscii ascName = "") : m_ascAddress(ascAddress), m_ascName(ascName)
	{
	}

	/**
	 * [取得] アドレス取得
	 *	@return アドレス
	 */
	CAscii GetAddress(void) const
	{
		return m_ascAddress;
	}

	/**
	 * [取得] 名前取得
	 *	@return 名前
	 */
	CAscii GetName(void) const
	{
		return m_ascName;
	}

	/**
	 * [設定] アドレス設定
	 *	@param ascAddress アドレス(ASCII)
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetAddress(const CAscii& ascAddress)
	{
		m_ascAddress = ascAddress;
		return IsValid();
	}

	/**
	 * [設定] 名前設定
	 *	@param ascName 名前
	 */
	void SetName(const CAscii& ascName)
	{
		m_ascName = ascName;
	}

	/**
	 * [確認] 正常情報確認
	 *	@retval true 本インスタンスがもっている情報は有効。
	 *	@retval false 無効（誤り有り）
	 */
	bool IsValid(void) const
	{
		if ( ! m_ascAddress.IsEmpty() && STRLIB::IsAsciiOnly(m_ascAddress) )
		{
			return true;
		}
		return false;
	}
	
	/**
	 * [取得] 宛先表記.
	 *	@note 名前 ＜アドレス＞ という形の文字列を返します。名前がない場合、アドレスのみ返します（＜＞なし）。
	 *	@return 表記
	 */
	CAscii GetSenderPath(void) const
	{
		if ( m_ascName.IsEmpty() )
		{
			return m_ascAddress;
		}
		return CAscii::Fmt("%s <%s>", m_ascName, m_ascAddress);
	}

	/**
	 * [取得] 宛先表記
	 *	@param path 名前 ＜アドレス＞ という形の文字列をしていします。＜＞がない場合、エラーになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetSenderPath(LPCSTR path)
	{
		CAscii s = path;
		INT_PTR f0 = s.Find('<');
		INT_PTR f1 = s.Find('>', f0);
		if ( f0 >= 0 && f1 > f0 )
		{
			m_ascName = s.Left(f0);
			m_ascName.TrimRight(' ');
			m_ascAddress = s.Mid(f0 + 1, (f1 - f0) - 1);
			return true;
		}
		return false;
	}
};



/**@ingroup MAIL
 * メール内容管理クラス
 *
 *		差出者、宛先者、件名、内容を管理します。
 *		管理情報からメール送信用のテキストを作成することが可能です
 *		(受信は、{@link TNB::CPop3Client POP3クラス}が用意してあります)。
 *		受信したテキストを分析し、本クラスに記憶することが可能です
 *		(送信は、{@link TNB::CSmtpClient SMTPクラス}が用意してあります)。
 *
 *	@note インスタンスの複製が可能です。
 *
 *	@todo 解析データのエラー時の処理、及び通知が未実装です。
 *
 *	@par使用例
 *	<table>
 *	<tr><td><pre>
 *		CMailContent mail;
 *		mail.SetFromMailAddr("boss＠tnb.co.jp", "ぼす");		
 *		mail.AddToMailAddr("oyabun＠tnb.co.jp", "おやぶん");
 *		mail.AddCcMailAddr("master＠tnb.co.jp", "マスター");
 *		mail.SetSubject("例の件");
 *		mail.AddSendTextData("こんにちわ\r\n例の件、どうですか？");
 *		TRACE1("%s\n", mail.GetContentForSend());
 *	</pre></td></tr>
 *	<tr><td><pre>
 *		From: =?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss＠tnb.co.jp>
 *		To: =?ISO-2022-JP?B?GyRCJCokZCRWJHMbKEI=?= <oyabun＠tnb.co.jp>		
 *		Cc: =?ISO-2022-JP?B?GyRCJV4lOSU/ITwbKEI=?= <master＠tnb.co.jp>
 *		Subject: =?ISO-2022-JP?B?GyRCTmMkTjdvGyhC?=
 *		MIME-Version: 1.0
 *		X-Mailer: TNB Tiny Mailer
 *		Content-Type: text/plain; charset="ISO-2022-JP"
 *		Content-Transfer-Encoding: 7bit
 *
 *		.$B$3$s$K$A$o.(B
 *		.$BNc$N7o!"$I$&$G$9$+!).(B
 *		．
 *
 *	</pre></td></tr>
 *	</table>
 *
 *	@par必要ファイル
 *			TnbMailContent.h
 * 
 *	@date 06/10/05 作成だけ作成
 *	@date 06/10/06 SJIS文字列のままBASE64化していた。修正。
 *	@date 06/10/30 読み込み(解析)、実装。
 *	@date 06/10/31 読み込み(解析)結果、取得メソッド実装。
 */
class CMailContent
{
	CAscii						m_ascMailerName;	///< メーラー名
	CMailAddress				m_addrFrom;			///< 送信者メールアドレス
	CMailAddress				m_addrReply;		///< リプライメールアドレス
	enum{ TO = 0, CC = 1, BCC = 2, KINDMAX = 3 };
	CVectorT<CMailAddress>		m_aaddrTo[KINDMAX];	///< 宛先 [0];To [1];Cc [2];Bcc 。
	CAscii						m_ascSubject;		///< タイトル
	CAsciiVector				m_vascContents;		///< 内容テキスト
	CMapT<CAscii,CByteVector>	m_mapFileDatas;		///< ファイル内容＜ファイル名,内容＞
	CAsciisMap					m_mapUnknownCmd;	///< 解析時、不明なコマンド&パラメータ

	/**
	 * [変換] 専用のBASE64型からデコード.
	 *	【=?ISO-2022-JP?B?～～～?=】の部分をデコードし、他は、そのまま。
	 *	@param ascSrc 元データ
	 *	@return デコード結果
	 */
	CAscii m_FromBase64(const CAscii& ascSrc) const
	{
		CAscii ascRc;
		const int TAGSIZE = sizeof("=?ISO-2022-JP?B?") - 1;
		CBase64Str base64;
		CAscii asc = ascSrc;
		while ( ! asc.IsEmpty() )
		{
			INT_PTR f = asc.Find("=?");
			if ( f < 0 )
			{	
				break;
			}
			INT_PTR e = asc.Find("?=", f);
			if ( e < 0 )
			{
				break;
			}
			if ( f > 0 )
			{
				ascRc += asc.Left(f);
			}
			if ( asc.Mid(f, TAGSIZE).IsEqualNoCase("=?ISO-2022-JP?B?") )
			{
				CJisStr jis;
				jis.SetOfJis(base64.DecodeForStr(asc.Mid(f + TAGSIZE, e - f - TAGSIZE)));
				ascRc += jis.ToAscii();
			}
			else
			{
				ascRc += asc.Left(e + 2);
			}
			asc = asc.Mid(e + 2);
		}
		ascRc += asc;
		asc.Empty();
		return ascRc;
	}

	/**
	 * [変換] 専用のBASE64型にエンコード.
	 *   =?コード形式?タイプ?タイトル?=
	 *   | |         ||     ||       |_ ?= 終了コード
	 *   | |         ||     ||_ エンコードタイトル文字列
	 *   | |         ||     |_ ? 区切りコード
	 *   | |         ||_ BASE64 は「B」
	 *   | |         |_ ? 区切りコード
	 *   | |_ BASE64 は「iso-2022-jp」
	 *   |_ =? 開始コード
	 *	@note 30文字以上の場合、改行挟んで分割して作成します。
	 *	@param ascSrc 変換前
	 *	@return 変換後
	 */
	CAscii m_ToBase64(const CAscii& ascSrc) const
	{
		CAscii ascRc;
		if ( ! ascSrc.IsEmpty() )
		{
			CAscii asc = ascSrc;
			while ( asc.GetLength() > 30 )
			{
				CAscii a = asc.Left(30);
				if ( STRLIB::IsAsciiOnly(a) && a.Find(',') < 0 )
				{
					ascRc += CAscii::Fmt("%s\r\n ", a);
				}
				else
				{
					ascRc += CAscii::Fmt("=?ISO-2022-JP?B?%s?=\r\n ", CBase64Str().Encode(CJisStr::ToJis(a)));
				}
				asc = asc.Mid(a.GetLength());
			}
			if ( STRLIB::IsAsciiOnly(asc) && asc.Find(',') < 0 )
			{
				ascRc += asc;
			}
			else
			{
				ascRc += CAscii::Fmt("=?ISO-2022-JP?B?%s?=", CBase64Str().Encode(CJisStr::ToJis(asc)));
			}
		}
		return ascRc;
	}

	/**
	 * アドレス表記変換
	 *	@note "名前 <アドレス>" という表記にします。
	 *	@param addr アドレス情報
	 *	@return メール用表記
	 */
	CAscii m_FmtAddr(const CMailAddress& addr) const
	{
		return CAscii::Fmt("%s <%s>", m_ToBase64(addr.GetName().TrimRight()), addr.GetAddress());
	}

	/**
	 * 区切文字列作成
	 *	@note コール毎に異なる文字列を作成するので、取得して記憶して使うようにしてください。
	 *	@return 文字列
	 */
	CAscii m_MakeMultiPartString(void) const
	{
		CSystemTime time = CSystemTime::GetCurrent();
		return CAscii::Fmt("-----MultiPart_%s_", CBase64Str().Encode(CAscii(time.Format())));
	}

	//-- 以下データ解析用

	/*
	 * ヘッダ分離.
	 *	１）空行があるまでをヘッダと認識.
	 *	２）空文字で始める行を上の行の続きとする.
	 *	３）ascContent からヘッダ部を削除。
	 *	@param[in,out] _ascContent コンテンツ
	 *	@return ヘッダ
	 */
	CAsciiVector m_SeparateHead(CAsciiVector& _ascContent)
	{
		CAsciiVector vaHead;
		size_t i = 0;
		while ( true )
		{
			if ( i >= _ascContent.GetSize() )
			{
				// 最後まで行った。
				ASSERT(false);//全部ヘッダ？ちとおかしい
				_ascContent.RemoveAll();
				break;
			}
			const CAscii& asc = _ascContent[i];
			if ( asc.IsEmpty() )
			{
				_ascContent = _ascContent.Mid(i + 1);
				break;
			}
			if ( asc[0] == ' ' || asc[0] == '\t' )
			{
				//= 頭空行、前のコマンドに連結
				if ( ! vaHead.IsEmpty() )
				{
					CAscii a = asc;
//					a.TrimLeft(' ').TrimLeft('\t');
					a.TrimLeft(" \t");
					vaHead[vaHead.GetSize() - 1] += a;
				}
				else
				{
					ASSERT(false); // 無視するしか・・・
				}
			}
			else if ( asc.Find(':') >= 1 )
			{
				//= パラメータあり
				vaHead.Add(asc);
			}
			else
			{
				//= それ以外.. 無視するしか...
				ASSERT(false); 
			}
			i++;
		}
		return vaHead;
	}

	/**
	 * アドレス(From,To など)の行を解析
	 *	@note "=?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss@tnb.co.jp>,aaaa <sub@tnb.co.jp>" のようなデータ
	 */
	CVectorT<CMailAddress> m_AnalyzAddrLine(const CAscii& aParam)
	{
		CVectorT<CMailAddress> vAddr;
		CAsciiVector va = TNB::CAsciiOperator::SeparatePeriod(aParam, ',');
		loop ( i, va.GetSize() )
		{
			CAscii asc = m_FromBase64(va[i]);
			INT_PTR f = asc.ReverseFind('<');
			if ( f >= 0 )
			{
				INT_PTR e = asc.Find('>', f);
				if ( e >= 0 )
				{
					TRACE2A("Found: MailAddr[%s] Name[%s]\n", asc.Mid(f + 1, e - f - 1), asc.Left(f).TrimRight());
					vAddr.Add(CMailAddress(asc.Mid(f + 1, e - f - 1), asc.Left(f).TrimRight()));
					continue;
				}
			}
			vAddr.Add(CMailAddress("", asc));
		}
		return vAddr;
	}

	/**
	 * ヘッダの一行を解析。
	 *	@param aCmd コマンド。
	 *	@param aParam パラメータ。
	 *	@retval true 解析済み
	 *	@retval false 解析対象外、解析失敗
	 */
	bool m_AnalyzHeadLine(const CAscii& aCmd, const CAscii& aParam)
	{
		bool boRc = false;
		if ( aCmd.IsEqual("from") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				ASSERT(vAddr.GetSize() == 1);
				boRc = SetFromMailAddr(vAddr[0]);
			}
		}
		else if ( aCmd.IsEqual("reply-to") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				ASSERT(vAddr.GetSize() == 1);
				boRc = SetReplyMailAddr(vAddr[0].GetAddress());
			}
		}
		else if ( aCmd.IsEqual("to") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				boRc = true;
				loop ( i, vAddr.GetSize() )
				{
					boRc &= AddToMailAddr(vAddr[i]);
				}
			}
		}
		else if ( aCmd.IsEqual("cc") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				boRc = true;
				loop ( i, vAddr.GetSize() )
				{
					boRc &= AddCcMailAddr(vAddr[i]);
				}
			}
		}
		else if ( aCmd.IsEqual("bcc") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				boRc = true;
				loop ( i, vAddr.GetSize() )
				{
					boRc &= AddBccMailAddr(vAddr[i]);
				}
			}
		}
		else if ( aCmd.IsEqual("subject") )
		{
			CAscii a = m_FromBase64(aParam);
			TRACE1A("Found: Subject[%s]\n", a);
			SetSubject(a);
			boRc = true;
		}
		else if ( aCmd.IsEqual("x-mailer") )
		{
			TRACE1A("Found: X-Mailer[%s]\n", aParam);
			SetMailerName(aParam);
			boRc = true;
		}
		return boRc;
	}

	/**
	 * パラメータ行解析
	 *	@note [text/plain; charset="ISO-2022-JP"]を	
	 *			[text/plain],[charset],[ISO-2022-JP] というように分解します。
	 *			ついでに BASE64であれば、デコードします。
	 *			ついでに [0] [1] は小文字化します。
	 */
	CAsciiVector m_AnalyzParamLine(const CAscii& aParam)
	{
		CAsciiVector va;
		INT_PTR f = aParam.Find(';');
		if ( f < 0 )
		{
			va.Invalid();
			return va;
		}
		INT_PTR e = aParam.Find('=', f);
		if ( e < 0 )
		{
			va.Invalid();
			return va;
		}
		va.Add(LowerString(aParam.Left(f)));
		va.Add(LowerString(aParam.Mid(f + 1, e - f - 1).TrimLeft()));
		CAscii a = aParam.Mid(e + 1);
//		a.TrimLeft().TrimLeft('\042');
//		a.TrimRight().TrimRight('\042');
		a.Trim(" \042");
		va.Add(m_FromBase64(a));
		TRACE3A("Analyz Param: %s; %s=\042%s\042\n", va[0], va[1], va[2]);
		return va;
	}

	/**
	 * [解析] 受信データ解析
	 *	@param[in,out] _vaContent 受信内容。
	 *	@param[out] _mapUnknownCmd 知らないパラメータ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_SetContent(CAsciiVector& _vaContent, CAsciisMap& _mapUnknownCmd)
	{
		//= ヘッダを分離
		CAsciiVector vaHead = m_SeparateHead(_vaContent);
		//= ヘッダを解析
		loop ( i, vaHead.GetSize() )
		{
			const CAscii& a = vaHead[i]; 
			CAscii aParam;
			CAscii aCmd = LowerString(a.FindCut(':', &aParam));
			aParam.TrimLeft();
			TRACE2A("Found: Cmd[%s] = [%s]\n", aCmd, aParam);
			//
			if ( ! m_AnalyzHeadLine(aCmd, aParam) )
			{
				_mapUnknownCmd[aCmd] = aParam;
			}
		}
		//
		if ( ! _mapUnknownCmd.HasKey("content-type") )
		{
			return false;
		}
		CAsciiVector vaType = m_AnalyzParamLine(_mapUnknownCmd["content-type"]);
		if ( vaType.GetSize() == 3 && vaType[0].Find("multipart") == 0 && vaType[1].IsEqual("boundary") )
		{
			//== マルチパートだ！
			CAscii aBound = CAscii::Fmt("--%s", vaType[2]);
			CAscii aBoundLast = aBound + "--";
			CAsciisMap mapTempCmd;
			INDEX line = _vaContent.Find(aBound) + 1;
			bool boCanNext = true;
			while ( boCanNext )
			{
				INDEX f = _vaContent.Find(aBound, line);
				if ( f == INVALID_INDEX )
				{
					f = _vaContent.Find(aBoundLast, line);
					if ( f == INVALID_INDEX )
					{
						return false;
					}
					boCanNext = false;
				}
				mapTempCmd.RemoveAll();
				if ( ! m_SetContent(_vaContent.Mid(line, f - line), mapTempCmd) )	//再帰呼び出し
				{
					return false;
				}
				line = f + 1;
			}
			return true;
		}
		if ( vaType.GetSize() == 3 && vaType[0].Find("text") == 0 && vaType[1].IsEqual("charset") )
		{
			//== テキストパートだ！
			ASSERT(! vaType[2].IsEmpty());
			ASSERT(! _mapUnknownCmd["content-transfer-encoding"].IsEmpty());
			CAscii aRc;
			CJisStr jis;
			loop ( i, _vaContent.GetSize() )
			{
				const CAscii& a = _vaContent[i];
				if ( a.IsEqual(".") )
				{
					break;
				}
				if ( i != 0 )
				{
					aRc += "\r\n";
				}
				if ( a.IsEqual("..") )
				{
					aRc += ".";
				}
				else
				{
					jis.SetOfJis(a);
					aRc += jis.ToAscii();
				}
			}
			AddSendTextData(aRc);
			return true;
		}
		if ( ! _mapUnknownCmd.HasKey("content-disposition") )
		{
			return false;
		}
		CAsciiVector vaDisp = m_AnalyzParamLine(_mapUnknownCmd["content-disposition"]);
		if ( vaDisp.GetSize() != 3 )
		{
			return false;
		}
		if ( vaDisp[0].IsEqual("attachment") && vaDisp[1].IsEqual("filename") )
		{
			ASSERT(_mapUnknownCmd["content-transfer-encoding"].IsEqualNoCase("base64"));
			CAscii a;
			loop ( i, _vaContent.GetSize() )
			{
				a += _vaContent[i];
			}
			CBase64Str base64;
			AddSendBinaryData(vaDisp[2], base64.Decode(a));
			return true;
		}
		return false;
	}

public:

	/// コンストラクタ
	CMailContent(void) : m_ascMailerName("TNB Tiny Mailer") 
	{
	}

	/// デストラクタ
	~CMailContent(void)	{}

	/**
	 * [設定] 設定初期化.
	 *	@note コンストラクトした状態に戻します。
	 */
	void Init(void)
	{
		CMailContent s;
		*this = s;
	}

	/**
	 * [設定] 送信名設定
	 *	@note 送信のために必須な設定です。
	 *	@param ascFromAddr アドレス(Ascii文字のみ)。
	 *	@param ascFromName ユーザ名。省略可能。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetFromMailAddr(const CAscii& ascFromAddr, const CAscii& ascFromName = "")
	{
		m_addrFrom.SetAddress(ascFromAddr);
		m_addrFrom.SetName(ascFromName);
		return m_addrFrom.IsValid();
	}

	/**
	 * [設定] 送信名設定
	 *	@note 送信のために必須な設定です。
	 *	@param addr メールアドレス
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetFromMailAddr(const CMailAddress& addr)
	{
		m_addrFrom = addr;
		return m_addrFrom.IsValid();
	}

	/**
	 * [設定] リプライ名設定.
	 *	@note NULLを指定すると解除になります。
	 *	@param ascReply リプライ先メールアドレス(Ascii文字のみ)。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetReplyMailAddr(const CAscii& ascReply = "")
	{
		return m_addrReply.SetAddress(ascReply);
	}

	/**
	 * [設定] メーラー名設定
	 *	@note 設定しないと”TNB Tiny Mailer”になっています。
	 *	@param ascMailerName メーラ名。
	 */
	void SetMailerName(const CAscii& ascMailerName)
	{
		m_ascMailerName = ascMailerName;
	}

	/**
	 * [設定] 宛先名設定初期化
	 *	@note AddToMailAddr() , AddCcMailAddr() , AddBccMailAddr() で設定した宛先をクリアします。
	 */
	void InitToMailAddr(void)
	{
		m_aaddrTo[TO].RemoveAll();
		m_aaddrTo[CC].RemoveAll();
		m_aaddrTo[BCC].RemoveAll();
	}

	/**
	 * [設定] 宛先名設定
	 *	@note 送信のために必須な設定です。必ず一人は設定してください。
	 *	@note 対象が複数人いる場合、本メソッドを人数回使用する必要があります。
	 *	@param addr 宛先。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddToMailAddr(const CMailAddress& addr)
	{
		if ( addr.IsValid() )
		{
			return m_aaddrTo[TO].Add(addr) != INVALID_INDEX;
		}
		return false;
	}

	/**
	 * [設定] 宛先名設定
	 *	@note 送信のために必須な設定です。必ず一人は設定してください。
	 *	@note 対象が複数人いる場合、本メソッドを人数回使用する必要があります。
	 *	@param ascToAddr 宛先アドレス(Ascii文字のみ)。
	 *	@param ascToName 宛先ユーザ名。省略可能。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddToMailAddr(const CAscii& ascToAddr, const CAscii& ascToName = "")
	{
		return AddToMailAddr(CMailAddress(ascToAddr, ascToName));
	}

	/**
	 * [設定] 宛先名設定(CC)
	 *	@note 対象が複数人いる場合、本メソッドを人数回使用する必要があります。
	 *	@param addr 宛先。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddCcMailAddr(const CMailAddress& addr)
	{
		if ( addr.IsValid() )
		{
			return m_aaddrTo[CC].Add(addr) != INVALID_INDEX;
		}
		return false;
	}

	/**
	 * [設定] 宛先名設定(CC)
	 *	@note 対象が複数人いる場合、本メソッドを人数回使用する必要があります。
	 *	@param ascCcAddr 宛先アドレス(Ascii文字のみ)。
	 *	@param ascCcName 宛先ユーザ名。省略可能。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddCcMailAddr(const CAscii& ascCcAddr, const CAscii& ascCcName = "")
	{
		return AddCcMailAddr(CMailAddress(ascCcAddr, ascCcName));
	}

	/**
	 * [設定] 宛先名設定(BCC)
	 *	@note 対象が複数人いる場合、本メソッドを人数回使用する必要があります。
	 *	@param addr 宛先。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddBccMailAddr(const CMailAddress& addr)
	{
		if ( addr.IsValid() )
		{
			return m_aaddrTo[BCC].Add(addr) != INVALID_INDEX;
		}
		return false;
	}

	/**
	 * [設定] 宛先名設定(BCC)
	 *	@note 対象が複数人いる場合、本メソッドを人数回使用する必要があります。
	 *	@param ascBccAddr 宛先アドレス(Ascii文字のみ)。
	 *	@param ascBccName 宛先ユーザ名。省略可能。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddBccMailAddr(const CAscii& ascBccAddr, const CAscii& ascBccName = "")
	{
		return AddBccMailAddr(CMailAddress(ascBccAddr, ascBccName));
	}

	/**
	 * [設定] 件名設定
	 *	@param ascSubject 件名。
	 */
	void SetSubject(const CAscii& ascSubject)
	{
		m_ascSubject = ascSubject;
	}
	
	/**
	 * [設定] 送信内容初期化
	 *	@note AddSendTextData() , AddSendBinaryData() , AddSendFileData() の設定をクリアします。
	 */
	void InitSendData(void)
	{
		m_vascContents.RemoveAll();
		m_mapFileDatas.RemoveAll();
	}

	/**
	 * [設定] 送信テキスト追加
	 *	@note 送信内容にテキスト追加します。複数回本メソッドを使用すると、
	 *			連結ではなく、別のパートにテキストが追加されますのでご注意ください。
	 *	@attention 各行は、70文字程度になるようにしてください。
	 *	@note		改行コードは、 ￥r￥n を使用します。
	 *	@param ascText テキスト。
	 */
	void AddSendTextData(const CAscii& ascText)
	{
		m_vascContents.Add(ascText);		///< 内容テキスト
	}

	/**
	 * [設定] 送信データ追加
	 *	@note 送信内容にバイナリデータ追加
	 *	@param ascFileName ファイル名
	 *	@param vb 内容
	 */
	void AddSendBinaryData(const CAscii& ascFileName, const CByteVector& vb)
	{
		if ( ! ascFileName.IsEmpty() )
		{
			m_mapFileDatas[ascFileName] = vb;
		}
	}
	
	/**
	 * [設定] 送信データ追加
	 *	@note 指定ファイルを読込んでそれを、送信内容にバイナリデータとして追加。
	 *	@param lpszFile ファイル名
	 *	@retval true 成功
	 *	@retval false 失敗(指定ファイルがない、など)
	 *	@throw CIoFailureException ファイル読み込み時にエラーが発生すると、スローされます。
	 */
	bool AddSendFileData(LPCTSTR lpszFile)
	{
		CStr fname = lpszFile;
		CFileReader file;
		if ( file.Open(fname) )
		{
			CByteVector vb = file.ReadInto();
			if ( ! vb.IsEmpty() )
			{
//				INT_PTR l = fname.ReverseFind('\\');
//				if ( l < 0 )
//				{
//					l = fname.ReverseFind('/');
//				}
				INT_PTR l = fname.ReverseFindOneOf(_T("\\/"));
				if ( l >= 0 )
				{
					fname = fname.Mid(l + 1);
				}
				m_mapFileDatas[CAscii(fname)] = vb;
				return true;
			}
		}
		return false;
	}

	//--------------------------------------------
	
	/**
	 * [参照] メーラー名参照
	 *	@return 格納されているメーラ名。
	 */
	const CAscii& ReferMailerName(void) const
	{
		return m_ascMailerName;
	}

	/**
	 * [参照] 件名参照
	 *	@return 格納されている 件名。
	 */
	const CAscii& ReferSubject(void) const
	{
		return m_ascSubject;
	}
	
	/**
	 * [参照] 送信メールアドレス参照
	 *	@note 得られるのは SetUser() の第二引数で設定した文字列か、SetContentFromReceive() の解析結果です。
	 *	@return 格納送信メールアドレス情報
	 */
	const CMailAddress& ReferFromMailAddr(void) const
	{
		return m_addrFrom;
	}

	/**
	 * [参照] 宛先名参照
	 *	@return 格納されている TOの宛先メールアドレス情報群
	 */
	const CVectorT<CMailAddress>& ReferToMailAddr(void) const
	{
		return m_aaddrTo[TO];
	}

	/**
	 * [参照] 宛先名参照(CC)
	 *	@return 格納されている CCの宛先メールアドレス情報群
	 */
	const CVectorT<CMailAddress>& ReferCcMailAddr(void) const
	{
		return m_aaddrTo[CC];
	}

	/**
	 * [参照] 宛先名参照(BCC)
	 *	@return 格納されている BCCの宛先メールアドレス情報群
	 */
	const CVectorT<CMailAddress>& ReferBccMailAddr(void) const
	{
		return m_aaddrTo[BCC];
	}

	/**
	 * [参照] リプライ名参照.
	 *	@return リプライ先メールアドレス(Ascii文字のみ)。
	 */
	CAscii ReferReplyMailAddr(void) const
	{
		return m_addrReply.GetAddress();
	}

	/**
	 * [参照] テキストコンテンツ参照
	 *	@return 格納されている テキスト本文の Vector。
	 */
	const CAsciiVector& ReferTextDatas(void) const
	{
		return m_vascContents;
	}

	/**
	 * [参照] バイナリコンテンツ参照
	 *	@return 格納されている バイナリ名とバイナリ内容の Map。
	 */
	const CMapT<CAscii, CByteVector>& ReferBinaryDatas(void) const
	{
		return m_mapFileDatas;
	}

	//--------------------------------------------
	
	/**
	 * [確認] 送信可能確認.
	 *		送信するための情報は設定されているか、確認します。
	 *	@retval true 可能。
	 *	@retval false 足りない。
	 */
	bool IsCompletesForSend(void) const
	{
		if ( m_aaddrTo[TO].IsEmpty() )
		{
			return false;
		}
		if ( ! m_addrFrom.IsValid() )
		{
			return false; 
		}
		return true;
	}

	/**
	 * [取得] 送信用アドレス一覧
	 *	@note AddToAddr() , AddCcAddr() , AddBccAddr() で設定したアドレス一覧
	 *	@return 一覧
	 */
	CAsciiVector GetAddrsForSend(void) const
	{
		CAsciiVector vaRc;
		loop ( i, KINDMAX )
		{
			loop ( j, m_aaddrTo[i].GetSize() )
			{
				vaRc.Add(m_aaddrTo[i][j].GetAddress());
			}
		}
		return vaRc;
	}

	/**
	 * [取得] 送信用メール内容取得
	 *	@note ヘッダパート、本体パートなど、連結して作成します。
	 *	@return 内容。
	 */
	CAscii GetContentForSend(void) const
	{
		//== ヘッダ作成 (BCCは含まない
		CAscii ascHead;
		ascHead += CAscii::Fmt("From: %s\r\n", m_FmtAddr(m_addrFrom));
		for ( int i = TO; i <= CC; i++ )
		{
			if ( ! m_aaddrTo[i].IsEmpty() )
			{
				ascHead += (i == 0) ? ("To: ") : ("Cc: ");
				loop ( j, m_aaddrTo[i].GetSize() )
				{
					if ( j > 0 )
					{
						ascHead += ",\r\n ";
					}
					ascHead += CAscii::Fmt("%s", m_FmtAddr(m_aaddrTo[i][j]));
				}
				ascHead += "\r\n";
			}
		}
		if ( ! m_addrReply.GetAddress().IsEmpty() )
		{
			ascHead += CAscii::Fmt("Reply-To: %s\r\n", m_addrReply.GetAddress());
		}
		ascHead += CAscii::Fmt("Subject: %s\r\n", m_ToBase64(m_ascSubject));
		ascHead += "MIME-Version: 1.0\r\n";
		ascHead += CAscii::Fmt("X-Mailer: %s\r\n", m_ascMailerName);
		if ( m_vascContents.GetSize() <= 1 && m_mapFileDatas.GetSize() == 0 )
		{
			//== テキスト一個だけ！
			ascHead += "Content-Type: text/plain; charset=\042ISO-2022-JP\042\r\n";
			ascHead += "Content-Transfer-Encoding: 7bit\r\n";
			ascHead += "\r\n";
			//本文作成
			CAscii ascMain; 
			if ( ! m_vascContents.IsEmpty() )
			{
				ascMain = m_vascContents[0];
				ascMain.Replace("\n.", "\n..");
			}
			ascMain += "\r\n.";
			//ヘッダ + 本文(JISに変換して連結)
			CAscii ascRc = ascHead + CJisStr::ToJis(ascMain).ReferBuffer();
			return ascRc;		
		}
		//== マルチパートだ
		CAscii ascPart = m_MakeMultiPartString();
		ascHead += "Content-Type: multipart/mixed;\r\n";
		ascHead += CAscii::Fmt(" boundary=\042%s\042\r\n", ascPart);
		ascHead += "Content-Transfer-Encoding: 7bit\r\n";
		ascHead += "\r\n";
		//- Text部
		loop ( i, m_vascContents.GetSize() )
		{
			ascHead += CAscii::Fmt("--%s\r\n", ascPart);
			ascHead += "Content-Type: text/plain; charset=\042ISO-2022-JP\042\r\n";
			ascHead += "Content-Transfer-Encoding: 7bit\r\n";
			ascHead += "\r\n";
			CAscii a = m_vascContents[i];
			a.Replace("\n.", "\n..");
			a += "\r\n";
			ascHead += CJisStr::ToJis(a);
		}
		//- Binary部
		loop ( i, m_mapFileDatas.GetSize() )
		{
			const CMapT<CAscii,CByteVector>::CConstKeysAdapter& vk = m_mapFileDatas.GetKeysAdapter();
			ascHead += CAscii::Fmt("--%s\r\n", ascPart);
			ascHead += "Content-Type: application/octet-stream\r\n";
			ascHead += "Content-Disposition: attachment;\r\n";
			ascHead += CAscii::Fmt(" filename=\042%s\042\r\n", m_ToBase64(vk.At(i)));
			ascHead += "Content-Transfer-Encoding: base64\r\n";
			ascHead += "\r\n";
			const CByteVector& vb = m_mapFileDatas[vk.At(i)];
			size_t size = vb.GetSize();
			const BYTE* B = vb.ReferBuffer();
			const size_t MAX = 76 * 3 / 4;	//変換後が76文字になるように
			while ( size > MAX )
			{
				ascHead += CBase64Str().Encode(MAX, B);
				ascHead += "\r\n";
				B += MAX;
				size -= MAX;
			}
			ascHead += CBase64Str().Encode(size, B);
			ascHead += "\r\n";
		}
		//- 終端
		ascHead += CAscii::Fmt("--%s--\r\n.", ascPart);
		return ascHead;
	}

	//--------------------------------------------

	/**
	 * [解析] 受信データ解析.
	 *		解析し、本インスタンスに設定します。
	 *	@note Refer～() という名のメソッドで、解析した結果を得ることが出来ます。
	 *	@note 本クラスの知らないコマンドは、 本メソッド成功後、
	 *		ReferUnknownCommandFromReceive() にて参照することが出来ます。Dateなどは、これに含まれます。
	 *	@param lpszContent 受信内容。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetContentFromReceive(LPCSTR lpszContent)
	{
		Init();
		CAsciiVector vaContent = TNB::CAsciiOperator::SeparateLine(lpszContent);
		if ( ! m_SetContent(vaContent, m_mapUnknownCmd) )
		{
			Init();
			return false;
		}
		return true;
	}

	/**
	 * [解析] 受信データ解析.
	 *		解析し、本インスタンスに設定します。
	 *	@note Refer～() という名のメソッドで、解析した結果を得ることが出来ます。
	 *	@note 本クラスの知らないコマンドは、 本メソッド成功後、
	 *		ReferUnknownCommandFromReceive() にて参照することが出来ます。Dateなどは、これに含まれます。
	 *	@param vascContent 受信内容。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetContentFromReceive(const CAsciiVector& vascContent)
	{
		Init();
		CAsciiVector vaContent = vascContent;
		if ( ! m_SetContent(vaContent, m_mapUnknownCmd) )
		{
			Init();
			return false;
		}
		return true;
	}

	/**
	 * [取得] 受信データ解析結果
	 *		SetContentFromReceive() で解析中、本クラスの知らないコマンド&パラメータを抽出してあります。
	 *		それを参照することが出来ます。
	 *	@note SetContentFromReceive() が失敗すると、情報は残りません。
	 *	@return コマンドとパラメータのマップ
	 */
	const CAsciisMap& ReferUnknownCommandFromReceive(void) const
	{
		return m_mapUnknownCmd;
	}
};



}; // TNB



#if 0
From:						SetFromMailAddr
Reply-To:					SetReplyMailAddr
To:							AddToMailAddr
Cc:							AddCcMailAddr
Bcc:						AddBccMailAddr
Subject:					SetSubject
X-Mailer:					SetMailerName
Content-Type:
Content-Disposition:
Content-Transfer-Encoding:
#endif

#if 0
From: =?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss@tnb.co.jp>
To: =?ISO-2022-JP?B?GyRCJCokZCRWJHMbKEI=?= <oyabun@tnb.co.jp>
Cc: =?ISO-2022-JP?B?GyRCJV4lOSU/ITwbKEI=?= <master@tnb.co.jp>
Subject: =?ISO-2022-JP?B?GyRCTmMkTjdvGyhC?=
MIME-Version: 1.0
Content-Type: text/plain; charset="ISO-2022-JP"
Content-Transfer-Encoding: 7bit
#endif

#if 0
From: =?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss@tnb.co.jp>
To: =?ISO-2022-JP?B?GyRCJCokZCRWJHMbKEI=?= <oyabun@tnb.co.jp>
Cc: =?ISO-2022-JP?B?GyRCJV4lOSU/ITwbKEI=?= <master@tnb.co.jp>
Subject: =?ISO-2022-JP?B?GyRCTmMkTjdvGyhC?=
MIME-Version: 1.0
X-Mailer: TNB Tiny Mailer
Content-Type: multipart/mixed;
 boundary="-----MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_"

-------MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_
Content-Type: text/plain; charset="ISO-2022-JP"
Content-Transfer-Encoding: 7bit

 $B$3$s$K$A$o (B
 $BNc$N7o!"$I$&$G$9$+!) (B

-------MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_
Content-Type: application/octet-stream
Content-Disposition: attachment;
 filename="=?ISO-2022-JP?B?Q0NEGyRCJSslYSVpMytILxsoQl9XaW5kbw==?=.dat"
Content-Transfer-Encoding: base64

MTIzNDU=

-------MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_--
.
#endif
