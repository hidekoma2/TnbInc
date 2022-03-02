#pragma once
/**
 *@file
 * JIS型文字列管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * JIS型文字列管理
 *
 *		JIS型の文字列を一つ保持するクラスです。
 *		ASCII,UNICODEとの相互変換用のメソッドを備えています。
 *
 *	@note	printf系や Format() Fmt() の ％s に対して、そのまま渡せません。
 *			CStr に変換してから渡してください。
 *
 *	@par必要ファイル
 *			TnbJisStr.h
 * 
 *	@date 06/09/27	新規作成。
 *	@date 08/08/06	ソース整理。 Vector を使わないように変更。
 *	@date 16/07/26	warring を整理。
 */
class CJisStr : public IComparableT<TNB::CJisStr>
{
	CAscii	m_vtcText;			///< 情報
	enum { ESC = 0x1B };
	/// 漢字リードByteチェック
	static bool IsKanji(int c)
	{
		return ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC));
	}
	/// 半角コードチェック
	static bool IsHankaku(int c)
	{
		return (c >= 0xa0 && c <= 0xdf);
	}
	/*
	 * [変換] 半角カナを全角カナに変換.
	 *	@param[out] _zenkaku 変換後の全角を格納
	 *	@param[in] P 半角カナのポインタ。次の文字も確認します。
	 *	@retval 1 濁点、半濁点無し変換
	 *	@retval 2 濁点、半濁点あり変換
	 */
	static int ms_HanToZen(WORD& _zenkaku, const BYTE* P)
	{
		const static WORD z[64] =
		{
			0x2121, 0x2123, 0x2156, 0x2157, 0x2122, 0x2126, 0x2572, 0x2521,
			0x2523, 0x2525, 0x2527, 0x2529, 0x2563, 0x2565, 0x2567, 0x2543,
			0x213c, 0x2522, 0x2524, 0x2526, 0x2528, 0x252a, 0x252b, 0x252d,
			0x252f, 0x2531, 0x2533, 0x2535, 0x2537, 0x2539, 0x253b, 0x253d,
			0x253f, 0x2541, 0x2544, 0x2546, 0x2548, 0x254a, 0x254b, 0x254c,
			0x254d, 0x254e, 0x254f, 0x2552, 0x2555, 0x2558, 0x255b, 0x255e,
			0x255f, 0x2560, 0x2561, 0x2562, 0x2564, 0x2566, 0x2568, 0x2569,
			0x256a, 0x256b, 0x256c, 0x256d, 0x256f, 0x2573, 0x212b, 0x212c
		};
		/// 濁点、半濁点型
		struct TMark
		{
			BYTE han;
			WCHAR zen;
		};
		/// 濁点テーブル
		const static TMark marks1[] =
		{
			{0xb3, 0x2574}, {0xb6, 0x252c}, {0xb7, 0x252e}, {0xb8, 0x2530},
			{0xb9, 0x2532}, {0xba, 0x2534}, {0xbb, 0x2536}, {0xbc, 0x2538},
			{0xbd, 0x253a}, {0xbe, 0x253c}, {0xbf, 0x253e}, {0xc0, 0x2540},
			{0xc1, 0x2542}, {0xc2, 0x2545}, {0xc3, 0x2547}, {0xc4, 0x2549},
			{0xca, 0x2550}, {0xcb, 0x2553}, {0xcc, 0x2556}, {0xcd, 0x2559},
			{0xce, 0x255c}, {0, 0}
		};
		/// 半濁点テーブル
		const static TMark marks2[] = 
		{
			{0xca, 0x2551}, {0xcb, 0x2554}, {0xcc, 0x2557}, {0xcd, 0x255a},
			{0xce, 0x255d}, {0, 0}
		};
		if ( P[1] == 0xde/*濁音符*/ )
		{
			loop ( i, countof(marks1) )
			{
				if ( P[0] == marks1[i].han )
				{
					_zenkaku = marks1[i].zen;
					return 2;
				}
			}
		}
		else if ( P[1] == 0xdf/*半濁音符*/ )
		{
			loop ( i, countof(marks2) )
			{
				if ( P[0] == marks2[i].han )
				{
					_zenkaku = marks2[i].zen;
					return 2;
				}
			}
		}
		int r = P[0] - 0xa0;
		ASSERT( r >= 0 && r < countof(z) );
		_zenkaku = z[r];
		return 1;
	}
	/*
	 * [変換] SJISの2バイトをJISに変換
	 *	@param[in,out] ph 上位BYTE
	 *	@param[in,out] pl 下位BYTE
	 */
	static void ms_SjisToJis(UINT *ph, UINT *pl)
	{
		*ph = *ph & 0xff;
		*pl = *pl & 0xff;
		if ( *ph <= 0x9F ) 
		{
			if ( *pl < 0x9F )
			{
				*ph = (*ph << 1) - 0xE1;
			}
			else 
			{
				*ph = (*ph << 1) - 0xE0;
			}
		}
		else
		{
			if ( *pl < 0x9F )
			{
				*ph = (*ph << 1) - 0x161;
			}
			else
			{
				*ph = (*ph << 1) - 0x160;
			}
		}
		if ( *pl < 0x7F )
		{
			*pl -= 0x1F;
		} 
		else if ( *pl < 0x9F )
		{
			*pl -= 0x20;
		}
		else 
		{
			*pl -= 0x7E;
		}
	}
	/*
	 * [変換] SJIS→ISO2022JP.
	 *		sjis_iso2022jp - SJISをISO-2022-JP(JIS)に変換 (RFC 1468)
	 *	@param[in] pSjis 元のSJIS文字列。
	 *	@param[out] pIso2022 変換したISO2022JP文字列。バッファは元の文字列の4倍必要。
	 *	@return 変換後の文字数。
	 */
	static INT_PTR ms_SjisToIso2022jp(LPCSTR pSjis, LPSTR pIso2022)
	{
		const BYTE* p = reinterpret_cast<const BYTE*>(pSjis);
		BYTE* r = reinterpret_cast<BYTE*>(pIso2022);
		bool isKanji = false;
		UINT c = 0;
		while ( (c = *(p++)) != '\0') 
		{
			if ( ! isKanji )
			{
				if ( IsKanji(c) || IsHankaku(c) )
				{
					*(r++) = ESC;
					*(r++) = '$';
					*(r++) = 'B';
					if ( IsKanji(c) )
					{
						UINT d = *(p++);
						ms_SjisToJis(&c, &d);
						*(r++) = static_cast<BYTE>(c);
						*(r++) = static_cast<BYTE>(d);
					}
					else
					{
						WORD e;
						int rc = ms_HanToZen(e, p - 1);
						if ( rc != 1 )
						{
							p++;
						}
						*(r++) = static_cast<BYTE>((e >> 8) & 0xff);
						*(r++) = static_cast<BYTE>(e & 0xff);
					}
					isKanji = true;
				} 
				else 
				{
					*(r++) = static_cast<BYTE>(c);
				}
			}
			else
			{
				if ( ! IsKanji(c) && ! IsHankaku(c) )
				{
					*(r++) = ESC;
					*(r++) = '(';
					*(r++) = 'B';
					*(r++) = static_cast<BYTE>(c);
					isKanji = false;
				}
				else 
				{
					if ( IsKanji(c) )
					{
						UINT d = *(p++);
						ms_SjisToJis(&c, &d);
						*(r++) = static_cast<BYTE>(c);
						*(r++) = static_cast<BYTE>(d);
					}
					else
					{
						WORD e;
						int rc = ms_HanToZen(e, p - 1);
						if (rc != 1) 
						{
							p++;
						}
						*(r++) = static_cast<BYTE>((e >> 8) & 0xff);
						*(r++) = static_cast<BYTE>(e & 0xff);
					}
				}
			}
		}
		if ( isKanji ) 
		{
			*(r++) = ESC;
			*(r++) = '(';
			*(r++) = 'B';
		}
		*r = '\0';
		return r - reinterpret_cast<BYTE*>(pIso2022);
	}
	/*
	 * [変換] JISの2バイトをSJISに変換
	 *	@param[in,out] ph 上位BYTE
	 *	@param[in,out] pl 下位BYTE
	 */
	static void ms_JisToSjis(UINT* ph, UINT* pl)
	{
		if ( (*ph & 1) != 0 )
		{
			if ( *pl < 0x60 )
			{
				*pl += 0x1F;
			}
			else
			{
				*pl += 0x20;
			}
		}
		else
		{
			*pl += 0x7E;
		}
		if ( *ph < 0x5F )
		{
			*ph = (*ph + 0xE1) >> 1;
		} 
		else 
		{
			*ph = (*ph + 0x161) >> 1;
		}
	}
	/**
	 * [変換] ISO-2022-JP(JIS)をSJISに変換 (RFC 1468)
	 *	@param[in] pIso2022 元のISO2022JP文字列。
	 *	@param[out] pSjis 変換したSJIS文字列。
	 *	@return 変換後の文字数。
	 */
	static INT_PTR ms_Iso2022jpToSjis(LPCSTR pIso2022, LPSTR pSjis)
	{
		LPCSTR p = pIso2022;
		LPSTR r = pSjis;
		int j = 0;
		bool jiskanji = false;
		bool hankaku = false;
		while ( *p != '\0')
		{
			j++;
			UINT c = *(p++);
			UINT d;
			if ( c == ESC )
			{
				if ( *p == '\0' )
				{
					break;
				}
				if ( (c = *(p++)) == '$' )
				{
					if ( (c = *(p++)) == '@' || c == 'B' )
					{
						jiskanji = true;
					}
					else 
					{
						*(r++) = ESC;
						*(r++) = '$';
						if ( c != '\0' )
						{
							*(r++) = static_cast<BYTE>(c);
						}
					}
				}
				else if ( c == '(' )
				{
					if ( (c = *(p++)) == 'H' || c == 'J' || c == 'B' )
					{
						jiskanji = false;
					}
					else
					{
						jiskanji = false;
						*(r++) = ESC;
						*(r++) = '(';
						if ( c != '\0' )
						{
							*(r++) = static_cast<BYTE>(c);
						}
					}
				}
				else if ( c == '*' )
				{
					if ( (c = *(p++)) == 'B' )
					{
						hankaku = false;
					}
					else if ( c == 'I' )
					{
						hankaku = false;
					}
				}
				else if ( hankaku && c == 'N' )
				{
					c = *(p++);
					*(r++) = static_cast<BYTE>(c + 0x80);
				}
				else if ( c == 'K' )
				{
					jiskanji = true;
				}
				else if ( c == 'H' )
				{
					jiskanji = false;
				}
				else
				{
					*(r++) = ESC;
					if ( c != '\0' )
					{
						*(r++) = static_cast<BYTE>(c);
					}
				}
			}
			else if ( jiskanji && (c == '\r' || c == '\n') )
			{
				jiskanji = false;
				*(r++) = static_cast<BYTE>(c);
			}
			else if ( jiskanji && c >= 0x21 && c <= 0x7E )
			{
				if ( *p == '\0' )
				{
					break;
				}
				if ( (d = *(p++)) >= 0x21 && d <= 0x7E )
				{
					ms_JisToSjis(&c, &d);
				}
				*(r++) = static_cast<BYTE>(c);
				if ( d != '\0' )
				{
					*(r++) = static_cast<BYTE>(d);
				}
			}
			else if ( c >= 0xA1 && c <= 0xFE )
			{
				if ( *p == '\0' )
				{
					break;
				}
				if ( (d = *(p++)) >= 0xA1 && d <= 0xFE )
				{
					d &= 0x7E;
					c &= 0x7E;
					ms_JisToSjis(&c, &d);
				}
				*(r++) = static_cast<BYTE>(c);
				if ( d != '\0' )
				{
					*(r++) = static_cast<BYTE>(d);
				}
			}
			else if ( c == 0x0E )
			{
				while ( *p != '\0' && *p != 0x0F && *p != '\r' && *p != '\n' && *p != ESC )
				{
					*(r++) = static_cast<BYTE>(*(p++) + 0x80);
				}
				if ( *p == 0x0F )
				{
					p++;
				}
			}
			else
			{
				*(r++) = static_cast<BYTE>(c);
			}
		}
		*r = '\0';
		return r - pSjis;
	}
public:

	/// コンストラクタ
	CJisStr(void)
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CJisStr(const CJisStr& other) : m_vtcText(other.m_vtcText)
	{
	}

	/**
	 * 代入コンストラクタ(JIS用)
	 *	@param lpszJis 代入するJIS文字列
	 */
	CJisStr(LPCSTR lpszJis)
	{
		SetOfJis(lpszJis);
	}

	/**
	 * 代入コンストラクタ(ASCII用)
	 *	@param ascii 代入するASCII文字列
	 */
	CJisStr(const CAscii& ascii)
	{
		SetOfAscii(ascii);
	}

	/**
	 * 代入コンストラクタ(UNICODE用)
	 *	@param unicode 代入するUNICODE文字列
	 */
	CJisStr(const CUnicode& unicode)
	{
		SetOfUnicode(unicode);
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CJisStr& operator=(const CJisStr& other)
	{
		m_vtcText = other.m_vtcText;
		return *this;
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CJisStr& t) const
	{
		return STRLIB::Compare(ReferBuffer(), t.ReferBuffer());
	}

	/**
	 * [設定] 空化
	 */
	void Empty(void)
	{
		m_vtcText.Empty();
	}

	/**
	 * [確認] 空か
	 *	@retval true 空
	 *	@retval false 文字有り
	 */
	bool IsEmpty(void) const
	{
		return GetLength() == 0;
	}

	/**
	 * [設定] 代入(JIS用)
	 *	@param lpszJis 代入するJis型文字列
	 */
	void SetOfJis(LPCSTR lpszJis)
	{
		if ( lpszJis == NULL )
		{
			Empty();
		}
		else
		{
			size_t iSize = STRLIB::GetLen(lpszJis);
			MemCopy(m_vtcText.GetBuffer(iSize + 1), lpszJis, iSize + 1);
			m_vtcText.ReleaseBuffer();
		}
	}

	/**
	 * [設定] 代入(UNICODE用)
	 *	@param lpszUnicode 代入するUNICODE文字列
	 */
	void SetOfUnicode(LPCWSTR lpszUnicode)
	{
		SetOfAscii(CAscii(lpszUnicode));
	}

	/**
	 * [設定] 代入(ASCII用)
	 *	@param lpszAscii 代入するASCII文字列
	 */
	void SetOfAscii(LPCSTR lpszAscii)
	{
		TTRACE0("SJIS 文字列を JIS 文字列に変換\n");
		if ( lpszAscii != NULL )
		{
			size_t len = STRLIB::GetLen(lpszAscii);
			if ( len > 0 )
			{
				INT_PTR r = ms_SjisToIso2022jp(lpszAscii, m_vtcText.GetBuffer(len * 4 + 1));
				ASSERT( r < static_cast<INT_PTR>(len * 4 + 1));
				m_vtcText.ReleaseBuffer();
				return;
			}
		}
		Empty();
	}

	/**
	 * [追加] 追加オペレータ
	 *	@note 自分の持っている文字列に指定文字列を連結します。
	 *	@param j プラス情報
	 *	@return 自分の参照
	 */
	CJisStr& operator+=(const CJisStr& j)
	{
		m_vtcText += j.m_vtcText;
		return *this;
	}

	/**
	 * [作成] 加算オペレータ
	 *	@note 自分の持っている文字列と指定文字列を連結した文字列を作成します。
	 *	@param j 加算文字列
	 *	@return 新しいJIS文字列
	 */
	CJisStr operator+(const CJisStr& j) const
	{
		CJisStr newJis = *this;
		newJis += j;
		return newJis;
	}

	/**
	 * [取得] JIS文字列長さ 
	 *	@return JIS文字列長さ
	 */
	size_t GetLength(void) const
	{
		return STRLIB::GetLen(ReferBuffer());
	}

	/**
	 * [取得] UNICODE文字列取得 
	 *	@return UNICODE文字列
	 */
	CUnicode ToUnicode(void) const { return CUnicode(ToAscii()); }

	/**
	 * [取得] ASCII文字列取得 
	 *	@return ASCII(SJIS)文字列
	 */
	CAscii ToAscii(void) const
	{
		TTRACE0("Jis 文字列を S-JIS文字列に変換\n");
		CAscii asc;
		size_t len = m_vtcText.GetLength();
		size_t r = ms_Iso2022jpToSjis(m_vtcText, asc.GetBuffer(len));
		ASSERT( r < len );
		asc.ReleaseBuffer();
		return asc;
	}

	/**
	 * [取得] 文字列取得 
	 *	@return 文字列(ASCII/SJISかUNICODE)
	 */
	CStr ToStr(void) const { return CStr(ToAscii()); }

	/**
	 * [取得] JIS文字列参照取得 
	 *	@return JIS文字列の先頭ポインタ
	 */
	LPCSTR ReferBuffer(void) const
	{
		return m_vtcText;
	}

	/**
	 * [取得] JIS文字列参照取得 
	 *	@return JIS文字列の先頭ポインタ
	 */
	operator LPCSTR (void) const
	{
		return ReferBuffer();
	}

	/**
	 * [操作] 書き込みバッファ要求
	 *	@note JIS型文字列を書き込んでください。
	 *	@note 必要サイズの書き込んでもよいバッファを返します。
	 *	@note 使用後、必ず ReleaseBuffer() をコールしてください。
	 *	@param iLength 必要な長さ
	 *	@return 書き込み可能アドレス
	 */
	LPSTR GetBuffer(int iLength)
	{
		return m_vtcText.GetBuffer(iLength);
	}

	/**
	 * [操作] 割り当てたバッファを開放.
	 *		GetBuffer() で割り当てたバッファを開放し、文字列を本クラスに取り込みます。
	 *	@note GetBuffer()で取得した領域に書き込みが終わったら必ず ReleaseBuffer() してください。
	 */
	void ReleaseBuffer(void)
	{
		m_vtcText.ReleaseBuffer();
		CJisStr s;
		s.SetOfJis(m_vtcText);
		m_vtcText = s.m_vtcText;
	}

	/**
	 * [変換] JISに変換
	 *	@param lpszAscii ASCII/SJIS文字列
	 *	@return JIS文字列。
	 */
	static CJisStr ToJis(LPCSTR lpszAscii)
	{
		CJisStr j;
		j.SetOfAscii(lpszAscii);
		return j;
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
