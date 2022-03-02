#pragma once
/**
 *@file
 * 文字列操作ライブラリ関係のヘッダ
 *
 *		CRT の関数をパックし、オーバーロードし、char でも WCHAR でも同じ
 *		名前で使えるようにしたものがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



#include <stdlib.h>
#include <WinNls.h>



//TNB Library
namespace TNB {
namespace STRLIB 
{

	/** @addtogroup	STRING
	 * @{
	 */

	/**
	 * [計算] 文字列長計算(ASCII/SJIS用)
	 *	@param lpsz 文字列
	 *	@return 文字列長
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline size_t GetLen(LPCSTR lpsz)
	{
		size_t r = 0;
		LPCSTR P = lpsz;
		if ( P != NULL )
		{
			while ( *P++ != 0 )
			{
				r++;
			}
			ASSERTLIB(r == strlen(lpsz));
		}
		return r;
	}

	/**
	 * [計算] 文字列長計算(UNICDE用)
	 *	@param lpsz 文字列
	 *	@return 文字列長
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline size_t GetLen(LPCWSTR lpsz)
	{
		size_t r = 0;
		LPCWSTR P = lpsz;
		if ( P != NULL )
		{
			while ( *P++ != 0 )
			{
				r++;
			}
			ASSERTLIB(r == wcslen(lpsz));
		}
		return r;
	}

	/**
	 * [複製] 文字列コピー(ASCII/SJIS用)
	 *	@note 領域が重なっている場合、動作は未保証です。
	 *	@param[out] _dst	コピー先
	 *	@param[in] src	コピー元
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline void Copy(LPSTR _dst, LPCSTR src)
	{
		ASSERT(! ::IsBadWritePtr(_dst, GetLen(src)));
		do
		{
			*_dst = *src++;
		}
		while ( *_dst++ != 0 );
	}

	/**
	 * [複製] 文字列コピー(UNICODE用)
	 *	@note 領域が重なっている場合、動作は未保証です。
	 *	@param[out] _dst	コピー先
	 *	@param[in] src	コピー元
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline void Copy(LPWSTR _dst, LPCWSTR src)
	{
		ASSERT(! ::IsBadWritePtr(_dst, GetLen(src) * sizeof(WCHAR)));
		do
		{
			*_dst = *src++;
		}
		while ( *_dst++ != 0 );
	}

	/** 
	 * [比較] 文字列比較(ASCII/SJIS用)
	 *	@param P1 比較１
	 *	@param P2 比較２
	 *	@param len 比較するサイズ。省略かマイナスならNULLを自動的に計算されます。
	 *	@param dwCmpFlags 比較オプション(以下の値の論理和) <BR>
	 *			NORM_IGNORECASE	大文字/小文字を区別しない					<BR>
	 *			NORM_IGNOREKANATYPE	ひらがな/カタカナを区別しない			<BR>
	 *			NORM_IGNORENONSPACE	場所を取らない文字を区別しない			<BR>
	 *			NORM_IGNORESYMBOLS	記号を無視する							<BR>
	 *			NORM_IGNOREWIDTH	1バイト文字とそれと同じ2バイト文字を区別しない<BR>
	 *			SORT_STRINGSORT	句読点を記号として扱う						<BR>
	 *	@retval ＜０ P1よりP2のほうが大きい
	 *	@retval ０	 P1はP2と同じ
	 *	@retval ＞０ P1よりP2のほうが小さい
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline int Compare(LPCSTR P1, LPCSTR P2, INT_PTR len = -1, DWORD dwCmpFlags = 0)
	{
		if ( P2 == NULL ){ P2 = ""; }
		int l = ToInt(len);
		#ifndef _WIN32_WCE
			return ::CompareStringA(LOCALE_USER_DEFAULT, dwCmpFlags, P1, l, P2, l) - CSTR_EQUAL;
		#else
			DWORD f = NORM_IGNOREKANATYPE | NORM_IGNORENONSPACE | NORM_IGNORESYMBOLS;
			if ( (dwCmpFlags & f) != 0 )
			{
				ASSERT0(false, "STRLIB::Compare()", "一部のフラグは無視されます");
			}
			if ( (dwCmpFlags & NORM_IGNORECASE) != 0 )
			{
				return (l < 0) ? ::_stricmp(P1, P2) : ::_strnicmp(P1, P2, l);
			}
			return (l < 0) ? ::strcmp(P1, P2) : ::strncmp(P1, P2, l);
		#endif
	}

	/** 
	 * [比較] 文字列比較(UNICODE用)
	 *	@param P1 比較１
	 *	@param P2 比較２
	 *	@param len 比較するサイズ。省略かマイナスならNULLを自動的に計算されます。
	 *	@param dwCmpFlags 比較オプション 
	 *	@retval ＜０ P1よりP2のほうが大きい
	 *	@retval ０	 P1はP2と同じ
	 *	@retval ＞０ P1よりP2のほうが小さい
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline int Compare(LPCWSTR P1, LPCWSTR P2, INT_PTR len = -1, DWORD dwCmpFlags = 0)
	{
		if ( P2 == NULL ){ P2 = L""; }
		int l = ToInt(len);
		return ::CompareStringW(LOCALE_USER_DEFAULT, dwCmpFlags, P1, l, P2, l) - CSTR_EQUAL;
	}

	/**
	 * [検索] 文字検索(ASCII/SJIS用)
	 *	@param lpszText 検索対象文字列
	 *	@param c 検索文字
	 *	@param iFromIndex 開始インデックス。省略すると0です。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCSTR lpszText, char c, INDEX iFromIndex = 0)
	{
		LPCSTR P = ::strchr(&lpszText[iFromIndex], c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [検索] 文字検索(UNICODE用)
	 *	@param lpszText 検索対象文字列
	 *	@param c 検索文字
	 *	@param iFromIndex 開始インデックス。省略すると0です。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCWSTR lpszText, WCHAR c, INDEX iFromIndex = 0)
	{
		LPCWSTR P = ::wcschr(&lpszText[iFromIndex], c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [検索] 文字セット検索(ASCII/SJIS用)
	 *	@param lpszText 検索対象文字列
	 *	@param lpszMark 検索文字セット
	 *	@param iFromIndex 開始インデックス。省略すると0です。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOneOf(LPCSTR lpszText, LPCSTR lpszMark, INDEX iFromIndex = 0)
	{
		size_t l = GetLen(&lpszText[iFromIndex]);
		size_t r = ::strcspn(&lpszText[iFromIndex], lpszMark);
		return (r >= l) ? -1 : r + iFromIndex;
	}

	/**
	 * [検索] 文字セット検索(UNICODE用)
	 *	@param lpszText 検索対象文字列
	 *	@param lpszMark 検索文字セット
	 *	@param iFromIndex 開始インデックス。省略すると0です。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOneOf(LPCWSTR lpszText, LPCWSTR lpszMark, INDEX iFromIndex = 0)
	{
		size_t l = GetLen(&lpszText[iFromIndex]);
		size_t r = ::wcscspn(&lpszText[iFromIndex], lpszMark);
		return (r >= l) ? -1 : r + iFromIndex;
	}

	/**
	 * [検索] 降順文字検索(ASCII/SJIS用)
	 *	@param lpszText 検索対象文字列
	 *	@param c 検索文字
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR ReverseIndexOf(LPCSTR lpszText, char c)
	{
		LPCSTR P = ::strrchr(lpszText, c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [検索] 降順文字検索(UNICODE用)
	 *	@param lpszText 検索対象文字列
	 *	@param c 検索文字
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR ReverseIndexOf(LPCWSTR lpszText, char c)
	{
		LPCWSTR P = ::wcsrchr(lpszText, c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [検索] 文字列検索(ASCII/SJIS用)
	 *	@param lpszText 検索対象文字列
	 *	@param lpszWord 検索文字列
	 *	@param iFromIndex 開始インデックス。省略すると0です。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCSTR lpszText, LPCSTR lpszWord, INDEX iFromIndex = 0)
	{
		LPCSTR P = strstr(&lpszText[iFromIndex], lpszWord);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [検索] 文字列検索(UNICDE用)
	 *	@param lpszText 検索対象文字列
	 *	@param lpszWord 検索文字列
	 *	@param iFromIndex 開始インデックス。省略すると0です。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCWSTR lpszText, LPCWSTR lpszWord, INDEX iFromIndex = 0)
	{
		LPCWSTR P = wcsstr(&lpszText[iFromIndex], lpszWord);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [検索] パス区切り検索(ASCII/SJIS用)
	 *	@param lpszText 検索対象ファイル名文字列
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR PathIndexOf(LPCSTR lpszText)
	{
		INT_PTR p1 = ReverseIndexOf(lpszText, '\\');
		INT_PTR p2 = ReverseIndexOf(lpszText, '/');
		return (p1 < p2) ? p2 : p1;
	}

	/**
	 * [検索] パス区切り検索(UNICODE用)
	 *	@param lpszText 検索対象ファイル名文字列
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline INT_PTR PathIndexOf(LPCWSTR lpszText)
	{
		INT_PTR p1 = ReverseIndexOf(lpszText, '\\');
		INT_PTR p2 = ReverseIndexOf(lpszText, '/');
		return (p1 < p2) ? p2 : p1;
	}

	/**
	 * [取得] 文字のサイズ(ASCII/SJIS用)
	 *	@note SJISの場合、全角チェックの判断が出来ます。
	 *	@param c 文字
	 *	@retval 2 二バイト文字の一Byte目
	 *	@retval 1 その他
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline int GetCharSize(char c)
	{
		return ::IsDBCSLeadByte(c) ? 2 : 1;
	}

	/**
	 * [取得] 文字のサイズ(UNICODE用)
	 *	@param c 文字
	 *	@return １、固定。
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline int GetCharSize(WCHAR c)
	{
		return 1; 
	}

	/**
	 * [変換] INT変換(ASCII/SJIS用).
	 *	@param lpsz 文字列。
	 *	@param iBase ベースになる値。普通 10 や 16 を指定。 0を指定すると、 
	 *					lpsz の表記に従う（先頭が0x,0Xなら16進数,0なら8進数,それ以外は10進数）。
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline int ToInt(LPCSTR lpsz, int iBase = 10) 
	{
		return strtol(lpsz, NULL, iBase);
	}

	/**
	 * [変換] INT変換(UNICODE用).
	 *	@param lpsz 文字列。
	 *	@param iBase ベース。10や16。 0を指定すると、 lpsz の表記に従う。 
	 *						先頭が0x,0Xなら16進数,0なら8進数,それ以外は10進数。
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline int ToInt(LPCWSTR lpsz, int iBase = 10)
	{
		return wcstol(lpsz, NULL, iBase); 
	}

	/**
	 * [変換] INT変換(ASCII/SJIS用).
	 *	@param lpsz 文字列。
	 *	@param iBase ベースになる値。普通 10 や 16 を指定。 0を指定すると、 
	 *					lpsz の表記に従う（先頭が0x,0Xなら16進数,0なら8進数,それ以外は10進数）。
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline DWORD ToDword(LPCSTR lpsz, int iBase = 10)
	{
		return strtoul(lpsz, NULL, iBase);
	}

	/**
	 * [変換] INT変換(UNICODE用).
	 *	@param lpsz 文字列。
	 *	@param iBase ベースになる値。普通 10 や 16 を指定。 0を指定すると、 
	 *					lpsz の表記に従う（先頭が0x,0Xなら16進数,0なら8進数,それ以外は10進数）。
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline DWORD ToDword(LPCWSTR lpsz, int iBase = 10)
	{
		return wcstoul(lpsz, NULL, iBase);
	}

	/**
	 * [変換] double変換(ASCII/SJIS用)
	 *	@param lpsz 文字列
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline double ToDouble(LPCSTR lpsz) 
	{
		return strtod(lpsz, NULL); 
	}

	/**
	 * [変換] double変換(UNICODE用)
	 *	@param lpsz 文字列
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline double ToDouble(LPCWSTR lpsz) 
	{ 
		return wcstod(lpsz, NULL); 
	}

	/**
	 * [変換] LONGLONG変換(ASCII/SJIS用)
	 *	@param lpsz 文字列
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline LONGLONG ToLonglong(LPCSTR lpsz)
	{
		return _atoi64(lpsz); 
	}

	/**
	 * [変換] LONGLONG変換(UNICODE用)
	 *	@param lpsz 文字列
	 *	@return 数値
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline LONGLONG ToLonglong(LPCWSTR lpsz)
	{
		return _wtoi64(lpsz);
	}

	/**
	 * [判断] ASCIIオンリー判断
	 *	@note	ASCIIコードだけの文字列か、否か判断します。
	 *	@param lpsz 文字列
	 *	@retval true ASCIIのみだ。
	 *	@retval false SJISなども含まれる。
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline bool IsAsciiOnly(LPCSTR lpsz)
	{
		while ( *lpsz != 0 )
		{
			if ( GetCharSize(*lpsz++) != 1 )
			{
				return false;// ASCII以外発見
			}
		}
		return true;
	}

	/**
	 * [変換] HEX文字数値変換
	 *	@note '0' ～ 'F' → 0x00 ～ 0x0F
	 *	@param c HEX文字
	 *	@retval 0以上	変換完了
	 *	@retval 0未満	変換失敗(知らない文字)
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline int HexCharToInt(int c)
	{
		int r = -1;
		if ( c >= 'a' && c <= 'f' )
		{
			r = (c - 'a') + 10;
		}
		else if ( c >= 'A' && c <= 'F' )
		{
			r = (c - 'A') + 10;
		}
		else if ( c >= '0' && c <= '9' )
		{
			r = c - '0';
		}
		return r;
	}

	/**
	 * [変換] 数値HEX文字変換
	 *	@note 0x00 ～ 0x0F → '0' ～ 'F'
	 *	@param c 数値。下位４Bitのみ使用します。
	 *	@return HEX文字
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	inline char IntToHexChar(int c)
	{
		return "0123456789ABCDEF"[c & 0x000F];
	}

	/**
	 * [変換] エスケープ文字表記変換
	 *	@note '￥x～' という文字列を文字コードに変換します。
	 *	@param[in,out] _lpsz '￥' を指しているポインタ。変換成功すると、その文字の次のポインタになります。
	 *	@retval 0以上	変換完了
	 *	@retval 0未満	変換失敗(知らない文字) _lpsz は変化しません。
	 *	@par必要ファイル
	 *			TnbStrLib.h
	 */
	template<typename TYP>
	inline int EscCharToInt(const TYP*& _lpsz)
	{
		if ( *_lpsz != '\\' ){ return -1; }
		int r = -1;
		const TYP* P = _lpsz;
		P++;
		TYP c = *P++;
		switch ( c )
		{
		case 't':	// タブ文字
			r = '\t';
			break;
		case 'n':	// 改行文字
			r = '\n';
			break;
		case 'r':	// キャリッジリターン
			r = '\r';
			break;
		case 'e':
			r = '\x1B';
			break;
		case '\\':
			r = '\\';
			break;
		case 'x':	// \xhh		16進数値を持つ文字
		case 'u':	// \uhhhh	16進数値を持つ文字
			loop( i, (c == 'x') ? 2 : 4 )
			{
				int c = STRLIB::HexCharToInt(*P);
				if ( c < 0 ) { break; }
				if ( r < 0 ) { r = 0; }
				r *= 16;
				r += c;
				P++;
			}
			break;
		case '0':
			if ( *P >= '0' && *P <= '9' )
			{
				loop ( i, 3 )
				{
					if ( *P < '0' || *P > '7' ) { break; }
					if ( r < 0 ) { r = 0; }
					r *= 8;
					r += (*P - '0');
					P++;
				}
			}
			break;
		default:
			break;
		}
		if ( r >= 0 )
		{
			_lpsz = P;
		}
		return r;
	}

}; // STRLIB



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")

