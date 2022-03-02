#pragma once
/**
 *@file
 * 文字列情報配列管理関係のヘッダ
 *
 *		文字列の配列（連続したメモリ配置）を管理します。
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * 文字列配列管理クラス
 *	@par必要ファイル
 *			TnbStrVector.h
 */
typedef TNB::CVectorT< CStr > CStrVector;



/**@ingroup COLLECT
 * 文字列配列管理クラス
 *	@par必要ファイル
 *			TnbStrVector.h
 */
typedef TNB::CVectorT< CAscii > CAsciiVector;



/**@ingroup COLLECT
 * 文字列配列管理クラス
 *	@par必要ファイル
 *			TnbStrVector.h
 */
typedef TNB::CVectorT< CUnicode > CUnicodeVector;



/**@ingroup STRING
 * [変換] ASCII文字列→文字単位配列
 *	@note 指定のASCII文字列を、WORD ベクタに変換します。
 *		二バイト文字、一バイト文字をそれぞれ一要素で格納します。
 *		終端文字（￥０）は入りません。
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param lpszAscii 変換文字列
 *	@return WORD 配列
 */
inline CWordVector StringToWordVector(LPCSTR lpszAscii)
{
	CWordVector vw;
	LPCSTR P = lpszAscii;
	while ( P != NULL && *P != 0 )
	{
		char c = *P++;
		BYTE b = c;
		if ( STRLIB::GetCharSize(c) == 2 )
		{
			BYTE d = *P++;
			vw.Add(static_cast<WORD>((b << 8) | d));
		}
		else
		{
			vw.Add(b);
		}
	}
	return vw;
}



/**@ingroup STRING
 * [変換] UNICODE文字列→文字単位配列
 *	@note 指定のUNICODE文字列を、WORD ベクタに変換します。
 *		終端文字（￥０）は入りません。
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param lpszUnicode 変換文字列
 *	@return WORD 配列
 */
inline CWordVector StringToWordVector(LPCWSTR lpszUnicode)
{
	CWordVector vw;
	vw.SetElements(STRLIB::GetLen(lpszUnicode), reinterpret_cast<const WORD*>(lpszUnicode));
	return vw;
}



/**@ingroup STRING
 * [変換] CStrVector → CStr 
 *	@note 文字列配列を連結し、一つの文字列にします。終端文字（￥０）は入りません。
 *	@note 敷居文字が文字列内に含まれていると、正常に処理できません。
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param vs 文字列配列。
 *	@param sepaChar 敷居キャラクタ。省略すると ^。
 *	@return 文字列。
 */
inline CStr StrVectorToStr(const CStrVector& vs, TCHAR sepaChar = '^')
{
	CStr str;
	loop ( i, vs.GetSize() )
	{
		if ( i != 0 )
		{
			str += sepaChar;
		}
		str += vs[i];
	}
	return str;
}

/**@ingroup STRING
 * [変換] CStr → CStrVector 
 *	@note StrVectorToStr() で連結した文字列を、文字列配列に分解します。
 *	@note 敷居文字が文字列内に含まれていると、正常に処理できません。
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param lpsz StrVectorToStr() で連結した文字列。
 *	@param sepaChar 敷居キャラクタ。省略すると ^。
 *	@return 文字列。
 */
inline CStrVector StrToStrVector(LPCTSTR lpsz, TCHAR sepaChar = '^')
{
	CStrVector vs;
	if ( STRLIB::GetLen(lpsz) > 0 )
	{
		INDEX index = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(lpsz, sepaChar, index);
			if ( r < 0 )
			{
				break;
			}
			CStr s;
			s.SetFromLeft(&lpsz[index], static_cast<size_t>(r - index));
			vs.Add(s);
			index = static_cast<INDEX>(r + 1);
		}
		vs.Add(CStr(&lpsz[index]));
	}
	return vs;
}

/**@ingroup STRING
 * [変換] CStrVector → CStr 
 *	@note 文字列配列を連結し、一つの文字列にします。
 *		終端文字（￥０）は入りません。
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param vs 文字列配列。
 *	@param sepaChar 敷居キャラクタ。省略すると ^。
 *	@param escChar 識別キャラクタ。省略すると ￥。
 *	@return 文字列。
 */
inline CStr StrVectorToStrEx(const CStrVector& vs, TCHAR sepaChar = '^', TCHAR escChar = '\\')
{
	CStr str;
	TCHAR aEsc[2] = { escChar, 0 };
	TCHAR aEsc2[3] = { escChar, escChar, 0 };
	TCHAR aSepa[3] = { escChar, sepaChar, 0 };
	loop ( i, vs.GetSize() )
	{
		if ( i != 0 )
		{
			str += aSepa;
		}
		CStr s = vs[i];
		s.Replace(aEsc, aEsc2);
		str += s;
	}
	return str;
}

/**@ingroup STRING
 * [変換] CStr → CStrVector 
 *	@note StrVectorToStrEx() で連結した文字列を、文字列配列に分解します。
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param lpsz StrVectorToStrEx() で連結した文字列。
 *	@param sepaChar 敷居キャラクタ。省略すると ^。
 *	@param escChar 識別キャラクタ。省略すると ￥。
 *	@return 文字列。
 */
inline CStrVector StrToStrVectorEx(LPCTSTR lpsz, TCHAR sepaChar = '^', TCHAR escChar = '\\')
{
	CStrVector vs;
	if ( lpsz != NULL && STRLIB::GetLen(lpsz) > 0 )
	{
		TCHAR aEsc[2] = { escChar, 0 };
		TCHAR aEsc2[3] = { escChar, escChar, 0 };
		TCHAR aSepa[3] = { escChar, sepaChar, 0 };
		INDEX index = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(lpsz, aSepa, index);
			if ( r < 0 )
			{
				break;
			}
			CStr s;
			s.SetFromLeft(&lpsz[index], static_cast<size_t>(r - index));
			s.Replace(aEsc2, aEsc);
			vs.Add(s);
			index = static_cast<INDEX>(r + 2);
		}
		CStr s = CStr(&lpsz[index]);
		s.Replace(aEsc2, aEsc);
		vs.Add(s);
	}
	return vs;
}

/**@ingroup STRING
 * [追加] 文字列追加特殊オペレータ. 
 *		％ で文字列を Vector 化するオペレータです。
 *	@note 多用はソースの可読性を落とすので、注意
 *	@par使用例
 *	\code
 *		// vs[0] == "ABC", vs[1] == "DEF", vs[2] == "GHI"
 *		CStrVector vs = CStr(_T("ABC")) % _T("DEF") % _T("GHI");
 *	\endcode
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param s1 配列１つの目の文字列。
 *	@param s2 配列２つの目の文字列。
 *	@return 文字列配列。
 */
inline CStrVector operator%(const CStr& s1, const CStr& s2)
{
	CStrVector vs;
	vs.Add(s1);
	vs.Add(s2);
	return vs;
}

/**@ingroup STRING
 * [追加] 文字列追加特殊オペレータ. 
 *		％ で文字列を Vector 化するオペレータです。
 *	@note 多用はソースの可読性を落とすので、注意
 *	@par使用例
 *	\code
 *		// vs[0] == "ABC", vs[1] == "DEF", vs[2] == "GHI"
 *		CStrVector vs = CStr(_T("ABC")) % _T("DEF") % _T("GHI");
 *	\endcode
 *	@par必要ファイル
 *			TnbStrVector.h
 *	@param vs 文字列配列。
 *	@param s 連結するの文字列。
 *	@return 文字列配列。
 */
inline CStrVector operator%(const CStrVector& vs, const CStr& s)
{
	CStrVector v = vs;
	v.Add(s);
	return v;
}

#ifdef __AFX_H__

	/**@ingroup COLLECT
	 * CString配列管理クラス
	 *	@note MFCプロジェクトの時のみ使用可能です。
	 *	@par必要ファイル
	 *			TnbStrVector.h
	 */
	typedef TNB::CVectorT<CString> CStringVector;

	/**@ingroup COLLECT
	 * [複製] CStringArrayから CStrVector へコピー
	 *	@note MFCプロジェクトの場合のみ使用の可能です。
	 *	@par必要ファイル
	 *			TnbStrVector.h
	 *	@param	astr 元の CStringArray 
	 *	@return CStrVector
	 */
	inline CStrVector ToStrVector(const CStringArray& astr)
	{
		CStrVector vs;
		size_t size = static_cast<size_t>(astr.GetSize());
		vs.SetSize(size);
		loop ( i, size )
		{
			vs.Set(i, CStr(astr[i]));
		}
		return vs;
	}

#endif



/**
 * [出力] デバッグトレース.
 *	@note リリースビルド時は何もしません。
 *	@param vs 文字列群
 */
#ifdef _DEBUG
inline void TRACE_SV(const CStrVector& vs) 
{
	loop ( i, vs )
	{
		::OutputDebugString(vs[i] + _T("\n"));
	}
}
#else
#define TRACE_SV(XX)
#endif



}; // TNB

