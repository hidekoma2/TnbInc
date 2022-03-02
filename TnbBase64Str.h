#pragma once
/**
 *@file
 * BASE64型管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbVector.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * BASE64風文字列変換抽象クラス
 *
 *		MIME BASE64 文字列とバイナリの相互変換をサポートする抽象クラスです。
 *
 *	@note そのまま使用することはありません。
 *
 *	@see CBase64Str
 *
 *	@par必要ファイル
 *			TnbBase64Str.h
 * 
 *	@date 06/10/11	抽象化して作成。
 */
template<typename TYP>
class CAbstractBase64T : CCopyImpossible
{
public:
	/// 定数
	enum{ 
		PADDING = 0x10000		///< パティングコード
	};

private:

	const TYP* m_lpszResult;	///< 解析終了ポインタ

	/*
	 * BASE64文字を数値に変換
	 *	@param c BASE64文字。
	 *	@retval -1		Base64の文字ではなかった
	 *	@retval PADDING	パディング文字だった。
	 *	@retval 0～63	BASE64文字だ。
	 */
	virtual int m_Base64ToInt(TYP c) = 0;

	/*
	 * 数値をBASE64文字に変換
	 *	@param i 数値。PADDING か 0～63。
	 *	@retval -1 範囲エラー(ASSERTされます)
	 *	@retval 0以上 BASE64文字。
	 */
	virtual TYP m_IntToBase64(int i) = 0;

	/*
	 * パディングを表すBASE64文字
	 *	@retval BASE64文字。
	 */
	TYP m_PaddingCode(void)
	{
		return m_IntToBase64(PADDING);
	}

	/*
	 * 空白文字をスキップ
	 */
	const TYP* m_SkipSpace(const TYP* P)
	{
		while ( *P == ' ' || *P == '\r' || *P == '\n' || *P == '\t' )
		{
			P++;
		}
		return P;
	}

public:

	/// コンストラクタ
	CAbstractBase64T(void) : m_lpszResult(NULL)
	{
	}

	/**
	 * [変換] デコード
	 *	@note NULL終端かBASE64以外の文字が現れるまでデコードします。
	 *		データが4の倍数でない場合、エラーになります。
	 *	@note スペース、タブ、改行コードは無視してデコードします。
	 *	@param lpszBase64 BASE64フォーマットのテキスト。
	 *	@return デコード結果
	 */
	CVectorT<BYTE> Decode(const TYP* lpszBase64)
	{
		m_lpszResult = NULL;
		CByteVector vb;
		const TYP* P = m_SkipSpace(lpszBase64);
		INT_PTR iLen = STRLIB::GetLen(P);
		while ( iLen > 0 )
		{
			const TYP* Q = P;
			if ( iLen < 4 )
			{
				if ( 0 <= m_Base64ToInt(*P) )
				{
					vb.Invalid();
				}
				break;
			}
			int r1 = m_Base64ToInt(*P);
			if ( r1 < 0 )
			{
				break;	//正常
			}
			P = m_SkipSpace(P + 1);
			int r2 = m_Base64ToInt(*P);
			P = m_SkipSpace(P + 1);
			int r3 = m_Base64ToInt(*P);
			P = m_SkipSpace(P + 1);
			int r4 = m_Base64ToInt(*P);
			P = m_SkipSpace(P + 1);
			if ( r1 >= 0x100 || r2 >= 0x100 || r2 < 0 || r3 < 0 || r4 < 0 )
			{
				vb.Invalid();
				break;
			}
			vb.Add(static_cast<BYTE>(((r1<<2)&0xFC) | ((r2>>4)&0x03)));
			if ( r3 < 0x0100 )
			{
				vb.Add(static_cast<BYTE>(((r2<<4)&0xF0) | ((r3>>2)&0x0F)));
				if ( r4 < 0x0100 )
				{
					vb.Add(static_cast<BYTE>(((r3<<6)&0xC0) | ((r4)&0x3F)));
				}
			}
			iLen -= (P-Q);
			if ( r3 >= 0x0100 || r4 >= 0x0100 )
			{
				break;
			}
		}
		m_lpszResult = P;
		return vb;
	}

	/**
	 * [変換] デコード
	 *	@note NULL終端かBASE64以外の文字が現れるまでデコードします。
	 *		データが4の倍数でない場合、エラーになります。
	 *	@note スペース、タブ、改行コードは無視してデコードします。
	 *	@attention エンコード前がテキストでないデータは、本関数でデコードしないで下さい。
	 *	@param lpszBase64 BASE64フォーマットのテキスト。
	 *	@return デコード結果。
	 */
	CAscii DecodeForStr(const TYP* lpszBase64)
	{
		CAscii asc;
		CByteVector vb = Decode(lpszBase64);
		if ( ! vb.IsEmpty() )
		{
			vb.Add(0);	//NULL終端追加
			asc = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
		}
		return asc;
	}

	/**
	 * [取得] デコード終了アドレス
	 *	@note Decode() / DecodeForStr() で成功後、デコードの停止したアドレスを得ることが出来ます。
	 *	@return アドレス
	 */
	const TYP* GetDecodeEndPtr(void) const
	{
		return m_lpszResult;
	}
	
	/**
	 * [変換] エンコード
	 *	@param size データサイズ
	 *	@param P データ
	 *	@return エンコード結果。
	 */
	virtual CStrT<TYP> Encode(size_t size, LPCVOID P)
	{
		CStrT<TYP> strRc;
		const BYTE* B = static_cast<const BYTE*>(P);
		int r;
		while ( size > 0 )
		{			
			if ( size == 1 )
			{
				r = B[0]>>2;
				strRc += m_IntToBase64(r);
				r = ((B[0]<<4)&0x30);
				strRc += m_IntToBase64(r);
				strRc += m_PaddingCode();
				strRc += m_PaddingCode();
				break;
			}
			else if ( size == 2 )
			{
				r = B[0]>>2;
				strRc += m_IntToBase64(r);
				r = ((B[0]<<4)&0x30) | ((B[1]>>4)&0x0F);
				strRc += m_IntToBase64(r);
				r = ((B[1]<<2)&0x3C);
				strRc += m_IntToBase64(r);
				strRc += m_PaddingCode();
				break;
			}
			r = B[0]>>2;
			strRc += m_IntToBase64(r);
			r = ((B[0]<<4)&0x30) | ((B[1]>>4)&0x0F);
			strRc += m_IntToBase64(r);
			r = ((B[1]<<2)&0x3C) | ((B[2]>>6)&0x03);
			strRc += m_IntToBase64(r);
			r = B[2]&0x3F;
			strRc += m_IntToBase64(r);
			size -= 3;
			B += 3;
		}
		return strRc;
	}

	/**
	 * [変換] エンコード
	 *	@param lpsz データ
	 *	@return エンコード結果。
	 */
	CStrT<TYP> Encode(LPCSTR lpsz)
	{
		return Encode(STRLIB::GetLen(lpsz), lpsz);
	}

	/**
	 * [変換] エンコード
	 *	@param c データ
	 *	@return エンコード結果。
	 */
	CStrT<TYP> Encode(const ISequenceCollectionT<BYTE>& c)
	{
		return Encode(c.GetSize(), c.ReferBuffer());
	}
};



/**@ingroup STRING
 * MIME BASE64型文字列変換
 *
 *		MIME BASE64 文字列とバイナリの相互変換をサポートするクラスです。
 *
 *	@par必要ファイル
 *			TnbBase64Str.h
 * 
 *	@date 06/09/27	新規作成
 *	@date 06/10/11	抽象化したクラスを継承
 */
class CBase64Str : public CAbstractBase64T<char>
{
	DEFSUPER(CAbstractBase64T<char>);

	/**
	 * BASE64文字を数値に変換
	 *	@param c BASE64文字。
	 *	@retval -1		Base64の文字ではなかった
	 *	@retval PADDING	パディング文字だった。
	 *	@retval 0～63	BASE64文字だ。
	 */
	virtual int m_Base64ToInt(char c)
	{
		int r = -1;
		if ( 'A' <= c && c <= 'Z' )
		{
			r = c - 'A';
		}
		else if ( 'a' <= c && c <= 'z' )
		{
			r = (c - 'a') +26;
		}
		else if ( '0' <= c && c <= '9' ) 
		{
			r = (c - '0') +52;
		}
		else if ( '+' == c )
		{
			r = 62;
		}
		else if ( '/' == c ) 
		{
			r = 63;
		}
		else if ( '=' == c )
		{
			r = PADDING;
		}
		ASSERTLIB(r < 0 || m_IntToBase64(r) == c);
		return r;
	}

	/**
	 * 数値をBASE64文字に変換
	 *	@param i 数値。PADDING か 0～63。
	 *	@retval -1 範囲エラー(ASSERTされます)
	 *	@retval -1以外 BASE64文字。
	 */
	virtual char m_IntToBase64(int i)
	{
		const char* acAlpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		ASSERTLIB(STRLIB::GetLen(acAlpha) == 64);
		if ( i == PADDING )
		{
			return '=';
		}
		else if ( i >= 0 && i < 64 )
		{
			return acAlpha[i];
		}
		ASSERTLIB(false);
		return -1;
	}
};



/**@ingroup STRING
 * DQ復活の呪文風文字列変換
 *
 *		DQの復活の呪文風文字列とバイナリの相互変換をサポートするクラスです。
 *
 *	@note BASE64 の使用する65個の文字をひらがなに変更しただけのものです。
 *
 *	@par必要ファイル
 *			TnbBase64Str.h
 * 
 *	@date 06/10/11	新規作成
 */
class CDqSaveStr : public CAbstractBase64T<WCHAR>
{
	DEFSUPER(CAbstractBase64T<WCHAR>);

	const WCHAR* m_list(void)
	{
		return 	L"あいうえおかきくけこ"
				L"さしすせそたちつてと"
				L"なにぬねのはひふへほ"
				L"まみむめもやゆよらり"
				L"るれろわがぎぐげござ"
				L"じずぜぞだぢづでどば"
				L"びぶべぼ";
	}

	/**
	 * BASE64文字を数値に変換
	 *	@param c BASE64文字。
	 *	@retval -1		Base64の文字ではなかった
	 *	@retval PADDING	パディング文字だった。
	 *	@retval 0～63	BASE64文字だ。
	 */
	virtual int m_Base64ToInt(WCHAR c)
	{
		const WCHAR* aa = m_list();
		if ( L'ん' == c )
		{
			return PADDING;
		}
		loop ( i, 64 )
		{
			if ( *aa++ == c )
			{
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	/**
	 * 数値をBASE64文字に変換
	 *	@param i 数値。PADDING か 0～63。
	 *	@retval 0xFFFF 範囲エラー(ASSERTされます)
	 *	@retval 0xFFFF以外 BASE64文字。
	 */
	virtual WCHAR m_IntToBase64(int i)
	{
		const WCHAR* aa = m_list();
		ASSERTLIB(STRLIB::GetLen(aa) == 64);
		if ( i == PADDING )
		{
			return L'ん';
		}
		else if ( i >= 0 && i < 64 )
		{
			return aa[i];
		}
		ASSERTLIB(false);
		return 0xFFFF;
	}
public:
#if 0
	/**
	 * [変換] エンコード
	 *	@param size データサイズ
	 *	@param P データ
	 *	@return エンコード結果。
	 */
	CUnicode Encode(size_t size, LPCVOID P)
	{
		CUnicode r;
		CUnicode s = _super::Encode(size, P);
		while ( ! s.IsEmpty() )
		{
			r += s.Left(3);
			s = s.Mid(3);
			if ( s.IsEmpty() )
			{
				break;
			}
			r += ' ';
			r += s.Left(3);
			s = s.Mid(3);
			if ( s.IsEmpty() )
			{
				break;
			}
			r += ' ';
			r += s.Left(4);
			s = s.Mid(4);
			if ( s.IsEmpty() )
			{
				break;
			}
			r += L"\r\n";
		}
		return r;
	}
#endif
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")



