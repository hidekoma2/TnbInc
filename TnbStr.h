#pragma once
/**
 *@file
 * 文字列管理関係のヘッダ
 *
 *		文字列を管理するクラスが有ります。
 *		ASCII/SJISとUNICODEをプロジェクトに関係なく使うこともできます。
 *		また、それらは相互に容易に変換可能です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSerializer.h"
#include "TnbStrLib.h"
#include "TnbTinyStringFormater.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document 作成用シンボル
 // CStrTのNULL状態
// _SELECTANY LPCVOID gpStrNull = &g_plNullBase[3];
#define gpStrNull (LPCVOID)(&(g_plNullBase[3]))
#endif



/**@ingroup STRING ITERATOR
 * 文字列管理テンプレート
 *
 *		複製、成形、保管が容易に出来る文字列管理クラスです。
 *
 *	@note IComparableT は実装していませんが、比較機能は持っています。
 *
 *	@note ISerializable は実装していませんが、シリアライズ機能は使用可能です。
 *
 *	@note begin() / end() による{@link ITERATOR 反復子}取得が可能です。
 *			SJIS を扱っている場合、２バイト文字が考慮されませんので注意してください。
 *
 *	@attention	本テンプレートを直接使うケースは有りません。
 *			CStr,CAscii,CUnicodeクラスが用意されています。
 *
 *	@see CStr, CAscii, CUnicode
 *
 *	@par必要ファイル
 *			TnbStr.h
 *
 *	@date 06/01/01	新規作成
 *	@date 06/05/12	代入系はいくつか
 *	@date 06/06/26	UNICODE系のバッファ確保数が文字数なのにByte数になっていたのを修正。
 *	@date 06/07/28	operator+ をメンバからグローバル関数へ変更。 LPCTSTR + CStr を可能にした。
 *	@date 06/08/13	イテレータを STL にあわせて、改修。
 *	@date 06/08/30	シリアライズ関係を追加。
 *	@date 07/06/29	Sandwich() の追加。
 *	@date 10/08/03	FindOneOf(), ReverseFindOneOf() の追加。構造整理。
 *	@date 12/03/07	IntToString(), DwordToString(), ToHexString() 追加。
 *	@date 15/03/16	ReferBuffer() を追加。
 *	@date 19/07/03	MakeUpper(), MakeLower() 新規。
 */
template<typename TYP>
class CStrT
{
public:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// const_iterator型宣言
	typedef const TYP* const_iterator;
	// iterator型宣言
	typedef TYP* iterator;
	#endif //_TnbDOXYEM

	/**
	 * [反復] 先頭const_iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return m_lpBuf; }

	/**
	 * [反復] 最後const_iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return begin() + GetLength(); }

	/**
	 * [反復] 先頭iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator begin(void) { GetBuffer(); ReleaseBuffer(); return m_lpBuf; }

	/**
	 * [反復] 最後iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator end(void) { return begin() + GetLength(); }

	/**
	 * [反復] 挿入
	 *	@param ite 挿入する位置の iterator
	 *	@param t 要素
	 *	@return 挿入した場所の iterator
	 */
	iterator insert(iterator ite, const TYP& t = TYP())
	{
		INDEX index = ite - begin();
		Insert(index, t);
		return begin() + index;
	}

	/**
	 * [反復] 先頭に挿入
	 *	@param t 要素
	 */
	void push_front(TYP t) { Insert(0, t); }

	/**
	 * [反復] 最後に追加
	 *	@param t 要素
	 */
	void push_back(TYP t) { operator+=(t); }


	//-------------------------


	/// コンストラクタ
	CStrT(void)
	{
		m_InitBuf(); 
	}

	/**
	 * コピーコンストラクタ
	 *	@param str 他のインスタンス
	 */
	CStrT(const CStrT& str)
	{
		if ( ! str.IsEmpty() )
		{
			::InterlockedIncrement(str.m_GetRefP());
			m_lpBuf = str.m_lpBuf;
		}
		else
		{
			m_InitBuf();
		}
	}

	/**
	 * 代入コンストラクタ(ASCII/SJIS用)
	 *	@param lpText 文字列
	 */
	CStrT(const char* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			m_lpBuf = ms_NewBuf(TYP(), lpText);
		}
		else
		{
			m_InitBuf();
		}
	}

	/**
	 * 代入コンストラクタ(UNICODE用)
	 *	@param lpText 文字列
	 */
	CStrT(const WCHAR* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			m_lpBuf = ms_NewBuf(TYP(), lpText);
		}
		else
		{
			m_InitBuf();
		}
	}

	/** [削除] 空化 */
	void Empty(void)
	{
		if ( m_lpBuf != static_cast<const TYP*>(gpStrNull) ) 
		{
			if ( ::InterlockedDecrement(m_GetRefP()) <= 0 )	
			{
				ms_Delete(m_lpBuf);
			}
			m_InitBuf();
		}
		ASEERT_NULLBASE;
	}

	/** デストラクタ */
	~CStrT(void)
	{
		Empty(); 
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other 他のインスタンス
	 *	@return 自分の参照
	 */
	CStrT& operator=(const CStrT& other)
	{
		if ( ! other.IsEmpty() )
		{
			::InterlockedIncrement(other.m_GetRefP());
			m_SetBuf(other.m_lpBuf);
		}
		else
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [代入] 代入(ASCII/SJIS用)
	 *	@param lpText 文字列
	 *	@return 自分の参照
	 */
	CStrT& operator=(const char* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			m_SetBuf(ms_NewBuf(TYP(), lpText));
		}
		else 
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [代入] 代入(UNICODE用)
	 *	@param lpText 文字列
	 *	@return 自分の参照
	 */
	CStrT& operator=(const WCHAR* lpText)
	{
		if ( lpText != NULL && *lpText != 0 ) 
		{
			m_SetBuf(ms_NewBuf(TYP(), lpText));
		}
		else 
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [代入] 文字数制限代入.
	 *		指定の文字列の左(先頭)から指定文字分だけ代入します。
	 *	@param lpText 文字列
	 *	@param iLen 文字数
	 *	@return 自分の参照
	 */
	CStrT& SetFromLeft(const TYP* lpText, size_t iLen)
	{
		if ( lpText == NULL || STRLIB::GetLen(lpText) <= iLen )
		{
			operator=(lpText);
		}
		else if ( iLen <= 0 )
		{
			Empty();
		}
		else
		{
			TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iLen);
			MemCopy(P, lpText, iLen);
			P[iLen] = 0;
			ms_CalcInfoPointer(P)->lLength = iLen;
			m_SetBuf(P);
		}
		return *this;
	}

	/**
	 * [追加] 追加オペレータ(ASCII/SJIS用)
	 *	@param lpText 文字列
	 *	@return 自分の参照
	 */
	CStrT& operator+=(const char* lpText)
	{
		return m_AddText(lpText);
	}

	/**
	 * [追加] 追加オペレータ(UNICODE用)
	 *	@param lpText 文字列
	 *	@return 自分の参照
	 */
	CStrT& operator+=(const WCHAR* lpText)
	{
		return m_AddText(lpText);
	}

	/**
	 * [追加] 追加オペレータ.
	 *	@note バッファを共有していない場合で、格納できるなら再メモリ確保は行わない
	 *	@param t 文字
	 *	@return 自分の参照
	 */
	CStrT& operator+=(TYP t)
	{
		TYP at[2] = {t, 0};
		return operator+=(at);
	}

	/**
	 * [代入] 書式付き文字列代入.
	 *	@par使用例
	 *		\code
	 *		void Log(LPCTSTR lpszFormat, ...)
	 *		{
	 *			CStr s;
	 *			va_list va;
	 *			va_start(va, lpszFormat);
	 *			s.FormatV(lpszFormat, va);
	 *			va_end(va);
	 *			::OutputDebugString(s);
	 *		}
	 *		\endcode
	 *
	 *	@param lpszFormat 書式指定文字列
	 *	@param V VA-LIST
	 */
	void FormatV(const TYP* lpszFormat, va_list V)
	{
		m_SetBuf(ms_Format(lpszFormat, V));
	}

	/**
	 * [代入] 書式付き文字列代入.
	 *	@param lpszFormat 書式指定文字列
	 *	@param ... 省略可能な引数
	 */
	void Format(const TYP* lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		FormatV(lpszFormat, args);
		va_end(args);
	}

	/**
	 * [取得] 参照オペレータ.
	 *	@return 持っている文字列の先頭ポインタ
	 */
	operator const TYP*(void) const
	{
		return m_lpBuf;
	}

	/**
	 * [取得] バッファ参照.
	 *	@return 持っている文字列の先頭ポインタ
	 */
	const TYP* ReferBuffer(void) const
	{
		return m_lpBuf;
	}

	/**
	 * [操作] 一文字置き換え
	 *	@param index 置き換えインデックス
	 *	@param t 置き換える文字
	 */
	void SetAt(INDEX index, TYP t)
	{
		if ( ! IsInRange(index, GetLength()) )
		{
			ASSERT1( false, "CStrT::SetAt()", "範囲外のIndex(%d)が指定されました。", index );
			return;
		}
		m_Separate();
		m_lpBuf[index] = t;
	}

	/**
	 * [挿入] 文字列挿入
	 *	@param index 挿入開始インデックス
	 *	@param lpText 挿入文字列
	 */
	void Insert(INDEX index, const TYP* lpText)
	{
		if ( GetLength() <= index )
		{
			m_AddText(lpText);
			return;
		}
		size_t iAddSize = STRLIB::GetLen(lpText);
		size_t iNowSize = GetLength();
		if ( *(m_GetRefP()) != 1 || m_GetBufSize() <= iAddSize + iNowSize )
		{
			TYP* P = ms_NewBuf(TYP(), m_lpBuf, iAddSize);	//足りないので新たに確保
			m_SetBuf(P);
		}
		// ずらす(NUL文字も含めるので、LOOP一回多くしている) 
		for ( INT_PTR i = iNowSize - index; i >= 0; i-- )
		{
			m_lpBuf[index + i + iAddSize] = m_lpBuf[index + i];
		}
		// 植え込み
		loop ( i, iAddSize )
		{
			m_lpBuf[index + i] = lpText[i];
		}
		m_GetInfoP()->lLength = iAddSize + iNowSize;
	}

	/**
	 * [挿入] 文字挿入
	 *	@param index 挿入開始インデックス
	 *	@param c 文字
	 */
	void InsertAt(INDEX index, TYP c)
	{
		TYP buf[2] = { c, 0 };
		Insert(index, buf);
	}

	/**
	 * [操作] 文字削除.
	 *	@note リードバイト(文字コードの一バイト目)を指定すると、続くトレイルバイトも削除されます。
	 *	@param index 削除するインデックス
	 *	@param iLen 削除する文字数(長さを超えている場合、指定インデックス以下がすべて削除されます)
	 *	@retval 0 削除していない
	 *	@retval 1以上 削除した文字数。ASCII/SJISの場合、iLen と同じ値になるとは限りません。
	 */
	int Delete(INDEX index, size_t iLen = 1)
	{
		if ( ! IsInRange(index, GetLength()) )
		{
			ASSERT1( false, "CStrT::Delete()", "範囲外のIndex(%d)が指定されました。", index );
			return 0;
		}
		if ( iLen == 0 )
		{
			ASSERT1( false, "CStrT::Delete()", "範囲外の長さ(%d)が指定されました。", 0 );
			return 0;
		}
		iLen--;
		if ( index + iLen >= GetLength() )
		{
			*this = Left(index);
			return 0;
		}
		m_Separate();
		size_t iDelSize = STRLIB::GetCharSize(m_lpBuf[index + iLen]) + iLen;
		size_t iSize = GetLength() - iDelSize;
		for ( size_t i = index; i < iSize; i++ ) 
		{
			m_lpBuf[i] = m_lpBuf[i + iDelSize];
		}
		m_lpBuf[iSize] = 0;
		m_GetInfoP()->lLength -= iDelSize;
		ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
		return iDelSize;
	}

	/**
	 * [操作] 最終文字削除.
	 *	@note 一文字削ります。
	 */
	void DeleteLast(void)
	{
		INT_PTR len = GetLength();
		if ( len > 0 )
		{
			m_Separate();
			m_lpBuf[len - 1] = 0;
			m_GetInfoP()->lLength -= 1;
			ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
		}
	}

	/**
	 * [取得] 一文字取得
	 *	@param index インデックス。1文字目がインデックス０になります。
	 *	@return インデックスの位置の文字。
	 */
	TYP GetAt(INDEX index) const
	{
		if ( IsInRange(index, GetLength()) )
		{
			return m_lpBuf[index];
		}
		ASSERT1( false, "CStrT::GetAt()", "範囲外のIndex(%d)が指定されました。", index );
		return 0;
	}

	/**
	 * [取得] 文字列長
	 *	@return 文字列長
	 */
	size_t GetLength(void) const
	{
		return m_GetInfoP()->lLength;
	}

	/**
	 * [確認] 空チェック
	 *	@retval true	空っぽ
	 *	@retval false	データ有り
	 */
	bool IsEmpty(void) const
	{
		return m_GetInfoP()->lLength == 0;
	}

	/**
	 * [確認] 検索.
	 *	@param t 検索する文字
	 *	@param iFromIndex 検索を開始するインデックス。省略すると頭からになります。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	INT_PTR Find(TYP t, INDEX iFromIndex = 0) const
	{
		return STRLIB::IndexOf(m_lpBuf, t, iFromIndex);
	}

	/**
	 * [確認] 検索.
	 *	@param lpsz 検索する文字列
	 *	@param iFromIndex 検索を開始するインデックス。省略すると頭からになります。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	INT_PTR Find(const TYP* lpsz, INDEX iFromIndex = 0) const
	{
		return STRLIB::IndexOf(m_lpBuf, lpsz, iFromIndex);
	}

	/**
	 * [確認] 検索.
	 *	@param lpsz 検索する文字セット
	 *	@param iFromIndex 検索を開始するインデックス。省略すると頭からになります。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	INT_PTR FindOneOf(const TYP* lpsz, INDEX iFromIndex = 0) const
	{
		return STRLIB::IndexOneOf(m_lpBuf, lpsz, iFromIndex);
	}

	/**
	 * [確認] 検索(後ろから)
	 *	@param t 検索する文字
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	INT_PTR ReverseFind(TYP t) const
	{
		INT_PTR lRc = -1;
		INT_PTR iPos = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(m_lpBuf, t, iPos);
			if ( r < 0 )
			{
				break;
			}
			lRc = r;
			iPos = r + STRLIB::GetCharSize(m_lpBuf[r]);
		}
		return lRc;
	}

	/**
	 * [確認] 検索(後ろから)
	 *	@param lpsz 検索する文字列
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	INT_PTR ReverseFind(const TYP* lpsz) const
	{
		INT_PTR lRc = -1;
		INT_PTR iPos = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(m_lpBuf, lpsz, iPos);
			if ( r < 0 )
			{
				break;
			}
			lRc = r;
			iPos = r + STRLIB::GetCharSize(m_lpBuf[r]);
		}
		return lRc;
	}

	/**
	 * [確認] 検索(後ろから)
	 *	@param lpsz 検索する文字セット
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	INT_PTR ReverseFindOneOf(const TYP* lpsz) const
	{
		INT_PTR lRc = -1;
		INT_PTR iPos = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOneOf(m_lpBuf, lpsz, iPos);
			if ( r < 0 )
			{
				break;
			}
			lRc = r;
			iPos = r + STRLIB::GetCharSize(m_lpBuf[r]);
		}
		return lRc;
	}

	/**
	 * [確認] 文字列比較.
	 *	@note 文字列同士の引き算をすると、比較結果が返ります。
	 *	@param lpszSubject 比較対照文字列
	 *	@retval ０		自分は lpszSubjectと同じ
	 *	@retval １以上	自分は lpszSubject より大きい
	 *	@retval ０未満	自分は lpszSubject より小さい
	 */
	int operator-(const TYP* lpszSubject) const
	{
		return Compare(lpszSubject);
	}

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval ０		自分は lpszSubjectと同じ
	 *	@retval １以上	自分は lpszSubject より大きい
	 *	@retval ０未満	自分は lpszSubject より小さい
	 */
	int Compare(const TYP* lpszSubject) const
	{
		return STRLIB::Compare(m_lpBuf, lpszSubject);
	}

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	同じ
	 *	@retval false	異なる
	 */
	bool IsEqual(const TYP* lpszSubject) const { return Compare(lpszSubject) == 0; }

	/**
	 * [確認] 文字列比較(大小区別無く比較)
	 *	@param lpszSubject 比較対照文字列
	 *	@retval ０		自分は lpszSubjectと同じ
	 *	@retval １以上	自分は lpszSubject より大きい
	 *	@retval ０未満	自分は lpszSubject より小さい
	 */
	int CompareNoCase(const TYP* lpszSubject) const
	{
		return STRLIB::Compare(m_lpBuf, lpszSubject, -1, NORM_IGNORECASE);
	}

	/**
	 * [確認] 文字列比較(比較オプション指定)
	 *	@param lpszSubject 比較対照文字列
	 *	@param dwCmpFlags 比較オプション(以下の値の論理和) <BR>
	 *			NORM_IGNORECASE	大文字/小文字を区別しない					<BR>
	 *			NORM_IGNOREKANATYPE	ひらがな/カタカナを区別しない			<BR>
	 *			NORM_IGNORENONSPACE	場所を取らない文字を区別しない			<BR>
	 *			NORM_IGNORESYMBOLS	記号を無視する							<BR>
	 *			NORM_IGNOREWIDTH	1バイト文字とそれと同じ2バイト文字を区別しない<BR>
	 *			SORT_STRINGSORT	句読点を記号として扱う						<BR>
	 *	@retval ０		自分は lpszSubjectと同じ
	 *	@retval １以上	自分は lpszSubject より大きい
	 *	@retval ０未満	自分は lpszSubject より小さい
	 */
	int CompareByOption(const TYP* lpszSubject, DWORD dwCmpFlags) const
	{
		return STRLIB::Compare(m_lpBuf, lpszSubject, -1, dwCmpFlags);
	}

	/**
	 * [確認] 文字列比較(大小区別無く比較)
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	同じ
	 *	@retval false	異なる
	 */
	bool IsEqualNoCase(const TYP* lpszSubject) const { return CompareNoCase(lpszSubject) == 0; }

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	同じ
	 *	@retval false	異なる
	 */
	bool operator==(const TYP* lpszSubject) const { return Compare(lpszSubject) == 0; }

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	異なる
	 *	@retval false	同じ
	 */
	bool operator!=(const TYP* lpszSubject) const { return Compare(lpszSubject) != 0; }

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	自分は lpszSubject より大きい
	 *	@retval false	偽
	 */
	bool operator>(const TYP* lpszSubject) const { return Compare(lpszSubject) > 0; }

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	自分は lpszSubject と同じか大きい
	 *	@retval false	偽
	 */
	bool operator>=(const TYP* lpszSubject) const { return Compare(lpszSubject) >= 0; }

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	自分は lpszSubject より小さい
	 *	@retval false	偽
	 */
	bool operator<(const TYP* lpszSubject) const { return Compare(lpszSubject) < 0; }

	/**
	 * [確認] 文字列比較
	 *	@param lpszSubject 比較対照文字列
	 *	@retval true	自分は lpszSubject と同じか小さい
	 *	@retval false	偽
	 */
	bool operator<=(const TYP* lpszSubject) const { return Compare(lpszSubject) <= 0; }

	/**
	 * [作成] 範囲取得.
	 *	@note	２Byteコードを意識し、途中で分断されないようにします。
	 *			その為、iSize で指定したサイズより長い文字列が得られる可能性もあります。
	 *	@param iOffset 取り出し開始Offset
	 *	@param iSize 取り出し文字数。省略すると最後まで取り出します。
	 *	@return 文字列
	 */
	CStrT Mid(INDEX iOffset, size_t iSize = INVALID_SIZE) const
	{
		CStrT str;
		size_t l = GetLength();
		if ( l > iOffset && iSize != 0 )
		{
			if ( iSize == INVALID_SIZE || iOffset + iSize > l )
			{
				iSize = l - iOffset;
			}
			// = サイズの補正
			if ( sizeof(TYP) == 1 )
			{
				size_t iPos = 0;
				while ( iPos < iSize )
				{
					iPos += STRLIB::GetCharSize(m_lpBuf[iOffset + iPos]);
				}
				iSize = iPos;
			}			
			TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iSize);
			MemCopy(P, &m_lpBuf[iOffset], iSize);
			P[iSize] = 0;
			ms_CalcInfoPointer(P)->lLength = iSize;
			str.m_SetBuf(P);
			ASSERTLIB( STRLIB::GetLen(str.m_lpBuf) == str.GetLength() );
		}
		return str;
	}

	/**
	 * [作成] 範囲取得.
	 *	@param iSize 先頭から取り出す文字数。
	 *	@return 文字列
	 */
	CStrT Left(size_t iSize) const
	{
		return Mid(0, iSize);
	}

	/**
	 * [作成] 範囲取得.
	 *	@param iSize 末尾から取り出す文字数。
	 *	@return 文字列
	 */
	CStrT Right(INT_PTR iSize) const
	{
		size_t l = GetLength();
		if ( static_cast<size_t>(iSize) > l )
		{
			return *this;
		}
		return Mid(l - iSize);
	}

	/**
	 * [作成] 前後連結.
	 *	@param lpszBefore 前につける文字列
	 *	@param lpszAfter 後につける文字列
	 *	@return 文字列
	 */
	CStrT Sandwich(const TYP* lpszBefore, const TYP* lpszAfter) const
	{
		CStrT str = lpszBefore;
		str += m_lpBuf;
		str += lpszAfter;
		return str;
	}

	/**
	 * [取得] 数値(int)へ変換
	 *	@param iOffset 取り出し開始Offset
	 *	@param iBase ベースになる値。普通 10 や 16 を指定。 0を指定すると、 
	 *					lpsz の表記に従う（先頭が0x,0Xなら16進数,0なら8進数,それ以外は10進数）。
	 *	@return 変換した値
	 */
	int ToInt(INDEX iOffset = 0, int iBase = 10) const
	{
		int i = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			i = STRLIB::ToInt(&m_lpBuf[iOffset], iBase);
		}
		return i;
	}

	/**
	 * [取得] 数値(DWORD)へ変換
	 *	@param iOffset 取り出し開始Offset
	 *	@param iBase ベースになる値。普通 10 や 16 を指定。 0を指定すると、 
	 *					lpsz の表記に従う（先頭が0x,0Xなら16進数,0なら8進数,それ以外は10進数）。
	 *	@return 変換した値
	 */
	DWORD ToDword(INDEX iOffset = 0, int iBase = 10) const
	{
		int i = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			i = STRLIB::ToDword(&m_lpBuf[iOffset], iBase);
		}
		return i;
	}

	/**
	 * [取得] 数値(double)へ変換
	 *	@param iOffset 取り出し開始Offset
	 *	@return 変換した値
	 */
	double ToDouble(INDEX iOffset = 0) const
	{
		double a = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			a = STRLIB::ToDouble(&m_lpBuf[iOffset]);
		}
		return a;
	}

	/**
	 * [取得] 数値(LONGLONG)へ変換
	 *	@param iOffset 取り出し開始Offset
	 *	@return 変換した値
	 */
	LONGLONG ToLonglong(INDEX iOffset = 0) const
	{
		LONGLONG ll = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			ll = STRLIB::ToLonglong(&m_lpBuf[iOffset]);
		}
		return ll;
	}

	/**
	 * [操作] 書き込みバッファ要求.
	 *		必要サイズの書き込んでもよいバッファを返します。
	 *	@note	使用後、必ず ReleaseBuffer() をコールしてください。
	 *	@par使用例
	 *	<table><tr><td><pre>
	 *
	 *	  CStr str;
	 *	  DWORD dwRc = GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);		
	 *	  str.ReleaseBuffer();
	 *	  int p1 = str.ReverseFind('\\');
	 *	</pre></td></tr></table>
	 *	@param iLength 必要な長さ。省略なら現在の長さ
	 *	@return 書き込み可能アドレス
	 */
	TYP* GetBuffer(size_t iLength = 0)
	{
		size_t lNowBufSize = m_GetBufSize();
		if ( lNowBufSize == 0 )
		{
			//今、空っぽ
			if ( iLength > 0 )
			{
				m_SetBuf(ms_New(TYP(), iLength + 1));
				*m_lpBuf = 0;
			}
			else
			{
				return NULL;
			}
		}
		else if ( lNowBufSize < iLength )
		{
			//サイズ拡張
			TYP* P = ms_New(TYP(), iLength); 
			MemCopy(P, m_lpBuf, lNowBufSize);
			m_SetBuf(P);
		}
		else if ( *(m_GetRefP()) > 1 )
		{
			//共有している,複製する必要有り
			TYP* P = ms_New(TYP(), lNowBufSize); 
			MemCopy(P, m_lpBuf, lNowBufSize);
			m_SetBuf(P);
		}
		m_GetInfoP()->lLength = 0;
		return m_lpBuf;
	}

	/**
	 * [操作] 割り当てたバッファを開放.
	 *		GetBuffer() で割り当てたバッファを開放し、文字列を本クラスに取り込みます。
	 *	@note GetBuffer()で取得した領域に書き込みが終わったら必ず ReleaseBuffer() してください。
	 *	@see GetBuffer()
	 */
	void ReleaseBuffer(void)
	{
		if ( *(m_GetRefP()) == 1 )
		{
			m_GetInfoP()->lLength = STRLIB::GetLen(m_lpBuf);
		}
	}

	/**
	 * [処理] 先頭から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param t 取り除く文字。省略する空白になります。
	 *	@return 自分の参照
	 */
	CStrT& TrimLeft(TYP t = ' ')
	{
		return m_TrimLeft(t);
	}

	/**
	 * [処理] 先頭から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param lpsz 取り除く文字セット。
	 *	@return 自分の参照
	 */
	CStrT& TrimLeft(const TYP* lpsz)
	{
		return m_TrimLeft(lpsz);
	}

	/**
	 * [処理] 末尾から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param t 取り除く文字。省略すると空白になります。
	 *	@return 自分の参照
	 */
	CStrT& TrimRight(TYP t = ' ')
	{
		return m_TrimRight(t);
	}

	/**
	 * [処理] 末尾から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param lpsz 取り除く文字セット。
	 *	@return 自分の参照
	 */
	CStrT& TrimRight(const TYP* lpsz)
	{
		return m_TrimRight(lpsz);
	}

	/**
	 * [処理] 両端から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param t 取り除く文字。省略すると空白になります。
	 *	@return 自分の参照
	 */
	CStrT& Trim(TYP t = ' ')
	{
		m_TrimLeft(t);
		return m_TrimRight(t);
	}

	/**
	 * [処理] 両端から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param lpsz 取り除く文字セット。
	 *	@return 自分の参照
	 */
	CStrT& Trim(const TYP* lpsz)
	{
		m_TrimLeft(lpsz);
		return m_TrimRight(lpsz);
	}

	/**
	 * [処理] 文字置換.
	 *	@note 保持する情報に対し処理します。
	 *	@note 文字を省くことはできません。置換するだけです。
	 *	@param tOld 置き換えられる文字
	 *	@param tNew 置き換える文字
	 *	@return 置換した数
	 */
	int Replace(TYP tOld, TYP tNew)
	{
		return m_Replace(tOld, tNew);
	}

	/**
	 * [処理] 文字置換.
	 *	@note 保持する情報に対し処理します。
	 *	@note 文字を省くことはできません。置換するだけです。
	 *	@param lpszOld 置き換えられる文字セット
	 *	@param tNew 置き換える文字
	 *	@return 置換した数
	 */
	int Replace(const TYP* lpszOld, TYP tNew)
	{
		return m_Replace(lpszOld, tNew);
	}

	/**
	 * [処理] 文字置換.
	 *	@note 保持する情報に対し処理します。
	 *	@param lpszOld 置き換えられる文字列
	 *	@param lpszNew 置き換える文字列
	 *	@return 置換した数
	 */
	int Replace(const TYP* lpszOld, const TYP* lpszNew)
	{
		int iRc = 0;
		if ( lpszOld != NULL) 
		{
			size_t iOldLen = STRLIB::GetLen(lpszOld);
			size_t iNewLen = (lpszNew == NULL) ? 0 : STRLIB::GetLen(lpszNew);
			size_t iLength = GetLength();
			//いくつあるか調べる
			int iCount = 0;
			INT_PTR index = 0;
			INT_PTR r;
			while ( (r = STRLIB::IndexOf(m_lpBuf, lpszOld, index)) >= 0 )
			{
				iCount++;
				index = r + iOldLen;
			}
			iLength += (iNewLen - iOldLen) * iCount;
			//確保して順次コピーする
			TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iLength);
			TYP* pNew = P;
			TYP* pOld = m_lpBuf;
			while ( (r = STRLIB::IndexOf(pOld, lpszOld, 0)) >= 0 )
			{
				MemCopy(pNew, pOld, r);			//キー文字までコピー
				pNew += r;
				MemCopy(pNew, lpszNew, iNewLen);	//新しい文字をコピー
				pNew += iNewLen;
				pOld += iOldLen+r;
				iRc++;
			}
			MemCopy(pNew, pOld, STRLIB::GetLen(pOld) + 1);
			m_SetBuf(P);
			m_GetInfoP()->lLength = iLength;
			ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
		}
		return iRc;
	}

	/**
	 * [処理] 文字削除.
	 *	@note 保持する情報に対し処理します。
	 *	@param t 削除する文字
	 *	@return 削除した数
	 */
	int Remove(TYP t)
	{
		TYP aBuf[2] = {t, 0};
		const TYP* P = NULL;
		return Replace(aBuf, P);
	}

	/**
	 * [作成] 切り分け
	 *	@note 指定セパレータ文字の前後の文字列を作成します。ない場合、文字列がそのままコピーされます。
	 *	@param[in] c セパレータ文字
	 *	@param[out]	_pstrRest セパレータ文字の後の文字列が格納されます。
	 *	@return セパレータ文字前までの文字列
	 */
	CStrT FindCut(TYP c, CStrT* _pstrRest = NULL) const
	{
		INT_PTR r = STRLIB::IndexOf(m_lpBuf, c, 0);
		if ( r >= 0 && _pstrRest != NULL )
		{
			*_pstrRest = Mid(r + 1);
		}
		return Mid(0, r);
	}

	/**
	 * [変換] 大文字化
	 */
	void MakeUpper(void)
	{
		ms_strupr(m_lpBuf);
	}

	/**
	 * [変換] 小文字化
	 */
	void MakeLower(void)
	{
		ms_strlwr(m_lpBuf);
	}


	//------------------------------------


	/**
	 * [設定] int型文字列化.
	 *		指定の int 型の数値を文字列に変換します。
	 *	@param value int型数値.
	 *	@return 書式化した文字数
	 */
	static CStrT IntToString(int value)
	{
		CStrT s;
		LPTSTR p = s.GetBuffer(32);
		INT_PTR i = CTinyStringFormaterT<TYP>::FormatInt(p, value);
		p[i] = 0;
		s.ReleaseBuffer();
		return s;
	}

	/**
	 * [設定] DWORD型文字列化.
	 *		指定の DWORD 型の数値を文字列に変換します。
	 *	@param value DWORD型数値.
	 *	@return 書式化した文字数
	 */
	static CStrT DwordToString(DWORD value)
	{
		CStrT s;
		LPTSTR p = s.GetBuffer(32);
		INT_PTR i = CTinyStringFormaterT<TYP>::FormatDword(p, value);
		p[i] = 0;
		s.ReleaseBuffer();
		return s;
	}

	/**
	 * [設定] DWORD型 HEX 文字列化(０パディング付き).
	 *		指定の int 型の数値を指定の文字数の文字列に変換します。
	 *	@param value DWORD 型数値.
	 *	@param width 変換後の文字数
	 *	@return 書式化した文字数
	 */
	static CStrT ToHexString(DWORD value, size_t width)
	{
		CStrT s;
		LPTSTR p = s.GetBuffer(width);
		 CTinyStringFormaterT<TYP>::FormatHex(p, value, width);
		s.ReleaseBuffer();
		return s;
	}

	/**
	 * [作成] 書式付き文字列作成
	 *	@param lpszFormat 書式指定文字列
	 *	@param ... 省略可能な引数
	 *	@return 文字列
	 */
	static CStrT Fmt(const TYP* lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		CStrT str;
		str.FormatV(lpszFormat, args);
		va_end(args);
		return str;
	}

	/**
	 * [作成] 指定文字を並べた文字列作成
	 *	@param t 文字
	 *	@param length 文字数
	 *	@return 文字列
	 */
	static CStrT Lineup(TYP t, size_t length)
	{
		TYP* P = ms_NewBuf(TYP(), ms_NullStr(), length);
		loop ( i, length )
		{
			P[i] = t;
		}
		P[length] = 0;
		ms_CalcInfoPointer(P)->lLength = length;
		CStrT str;
		str.m_SetBuf(P);
		ASSERTLIB( STRLIB::GetLen(str.m_lpBuf) == str.GetLength() );
		return str;
	}

	/**
	 * [作成] 指定文字を並べた文字列作成
	 *	@param lpsz 文字列
	 *	@param iSize 文字数
	 *	@return 文字列
	 */
	static CStrT Lineup(const TYP* lpsz, int iSize)
	{
		size_t l = STRLIB::GetLen(lpsz);
		TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iSize * l);
		TYP* Q = P;
		loop ( i, iSize )
		{
			loop ( j, l )
			{
				*Q++ = lpsz[j];
			}
		}
		*Q = 0;
		ms_CalcInfoPointer(P)->lLength = iSize * l;
		CStrT str;
		str.m_SetBuf(P);
		ASSERTLIB( STRLIB::GetLen(str.m_lpBuf) == str.GetLength() );
		return str;
	}

	/**
	 * [代入] イテレータ代入.
	 *		イテレータ指定で代入します。番兵イテレータか ￥０か max文字数を代入し文字列にします。
	 *	@param is 開始イテレータ。
	 *	@param ie 番兵イテレータ。
	 *	@param max 最大文字数
	 *	@return 文字列
	 */
	template<typename ITE>
	static CStrT FromIterator(ITE is, ITE ie = ITE(), size_t max = INVALID_SIZE)
	{
		CStrT s;
		while ( is != ie && *is != 0 && max-- != 0)
		{
			s += *is++;
		}
		return s;
	}

	/**
	 * [代入] WindowText代入.
	 *		指定のWindowのテキストが代入されます。
	 *	@param hWnd ウィンドウハンドル
	 *	@return 文字列
	 */
	static CStrT FromWindowText(HWND hWnd)
	{
		CStrT s;
		int iLen = ::GetWindowTextLength(hWnd);
		if ( iLen != 0 )
		{
			CWorkMemT<TCHAR> m(iLen + 1);
			::GetWindowText(hWnd, m, iLen + 1);
			s.m_SetBuf(ms_NewBuf(TYP(), m));
		}
		return s;
	}

	#ifdef __AFX_H__

	/**
	 * [代入] WindowText代入.
	 *		指定のWindowのテキストが代入されます。
	 *	@param pcWnd ウィンドウ
	 *	@return 文字列
	 */
	static CStrT FromWindowText(CWnd* pcWnd)
	{
		return FromWindowText(pcWnd->GetSafeHwnd());
	}

	#endif

private:

	/**
	 * マーク文字.
	 *	
	 *	@par必要ファイル
	 *			TnbStrOperator.h
	 * 
	 *	@date 10/08/02	新規作成
	 */
	template<typename TYP>
	class CGroupT
	{
	public:
		/**
		 * コンストラクタ.
		 *	@param c 文字
		 */
		CGroupT(TYP c) : m_char(c), m_lpsz(NULL)
		{
		}
		/**
		 * コンストラクタ.
		 *	@param lpsz 文字群
		 */
		CGroupT(const TYP* lpsz) : m_char(0), m_lpsz(lpsz)
		{
		}
		bool IsEqual(TYP c) const
		{
			if ( m_char != 0 )
			{
				return c == m_char;
			}
			return STRLIB::IndexOf(m_lpsz, c) >= 0;
		}
	private:
		TYP					m_char;
		const TYP*			m_lpsz;
	};

	/**
	 * [処理] 先頭から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param t 取り除く文字。省略する空白になります。
	 *	@return 自分の参照
	 */
	CStrT& m_TrimLeft(CGroupT<TYP> t)
	{
		m_Separate();
		size_t l = GetLength();
		loop ( i, l ) 
		{
			if ( ! t.IsEqual(m_lpBuf[i]) ) 
			{
				m_GetInfoP()->lLength -= i;
				for ( size_t j = 0; i < l + 1; i++, j++ ) 
				{
					m_lpBuf[j] = m_lpBuf[i];
				}
				ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
				return *this;
			}
		}
		Empty();
		return *this;
	}

	/**
	 * [処理] 末尾から文字をトリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param t 取り除く文字。省略すると空白になります。
	 *	@return 自分の参照
	 */
	CStrT& m_TrimRight(CGroupT<TYP> t)
	{
		m_Separate();
		size_t l = GetLength();
		for ( INT_PTR i = l - 1; i >= 0; i-- ) 
		{
			if ( ! t.IsEqual(m_lpBuf[i]) ) 
			{
				m_GetInfoP()->lLength = static_cast<size_t>(i + 1);
				m_lpBuf[i + 1] = 0;
				ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
				return *this;
			}
		}
		Empty();
		return *this;
	}

	/**
	 * [処理] 文字置換.
	 *	@note 保持する情報に対し処理します。
	 *	@note 文字を省くことはできません。置換するだけです。
	 *	@param tOld 置き換えられる文字
	 *	@param tNew 置き換える文字
	 *	@return 置換した数
	 */
	int m_Replace(CGroupT<TYP> tOld, TYP tNew)
	{
		int iRc = 0;
		if ( tNew != 0 ) 
		{
			m_Separate();
			size_t l = GetLength();
			for ( size_t i = 0; i < l; i += STRLIB::GetCharSize(m_lpBuf[i]) ) 
			{
				if ( tOld.IsEqual(m_lpBuf[i]) )
				{
					m_lpBuf[i] = tNew;
					iRc++;
				}
			}
		}
		return iRc;
	}
	
	/**
	 * 管理構造体型.
	 *
	 *	@note 文字列のポインタの前に管理領域（TInfo）が存在するようになっています。 
	 */
	struct TInfo
	{
		LONG	lRef;			///< 参照数
		size_t	lLength;		///< 有効文字数(NULL文字を除く)
		size_t	lBufSize;		///< 格納可能文字数(NULL文字を含む)
	};

	/**
	 * 管理情報のポインタ場所計算
	 *	@param lpsz 文字列のポインタ
	 *	@return ポインタ
	 */
	static TInfo* ms_CalcInfoPointer(LPVOID lpsz)
	{
		BYTE* B = static_cast<BYTE*>(lpsz);
		return reinterpret_cast<TInfo*>(B - sizeof(TInfo));
	}

	/**
	 * メモリ確保(ASCII/SJIS用)
	 *	@param d ダミー
	 *	@param iLen	確保したい長さ
	 *	@return 確保したポインタ（開放はms_Deleteを使用すること）
	 */
	static char* ms_New(char d, size_t iLen)
	{
		BYTE* B = new BYTE[iLen + sizeof(TInfo)];
		TInfo* I = reinterpret_cast<TInfo*>(B);
		I->lRef = 1;
		I->lLength = 0;
		I->lBufSize = iLen;
		return reinterpret_cast<char*>(B + sizeof(TInfo));
	}

	/**
	 * メモリ確保(UNICODE用)
	 *	@param d ダミー
	 *	@param iLen	確保したい長さ
	 *	@return 確保したポインタ（開放はms_Deleteを使用すること）
	 */
	static WCHAR* ms_New(WCHAR d, size_t iLen)
	{
		BYTE* B = new BYTE[iLen * sizeof(WCHAR) + sizeof(TInfo)];
		TInfo* I = reinterpret_cast<TInfo*>(B);
		I->lRef = 1;
		I->lLength = 0;
		I->lBufSize = iLen;
		return reinterpret_cast<WCHAR*>(B + sizeof(TInfo));
	}
	
	/**
	 * メモリ開放
	 *	@param pMem	ms_Newで確保したポインタ
	 */
	static void ms_Delete(LPVOID pMem)
	{
		ASSERTLIB( pMem != gpStrNull );
		BYTE* B = static_cast<BYTE*>(pMem);
		delete[] (B - sizeof(TInfo));
	}

	/** 
	 * 書式付き文字列作成
	 *	@param lpsz 書式指定文字列
	 *	@param V 省略可能な引数
	 *	@return 確保して文字列の入ったメモリ（開放はms_Deleteを使用すること）
	 */
	template<typename T>
	static T* ms_Format(const T* lpsz, va_list V)
	{
		size_t iLen = 1024 + STRLIB::GetLen(lpsz);
		while ( true )
		{
			CWorkMemT<T> work(iLen); 
			if ( STRLIB::VPrintF(work, iLen, lpsz, V) )
			{
				size_t r = STRLIB::GetLen(work);
				T* X = ms_New(T(), r + 1);
				const T* W = work;
				MemCopy(X, W, r + 1);
				ms_CalcInfoPointer(X)->lLength = r;
				return X;
			}
			iLen *= 2;
		}
	}

	/**
	 * バッファサイズ
	 *	@param iSize 必須サイズ
	 *	@param iExSize 拡張サイズ。マイナスなら必須サイズ＋１が返る。0以上なら、
	 *					必須サイズ＋拡張サイズ＋余白＋１が返る。
	 *	@return 確保すべきバッファサイズ
	 */
	static size_t ms_GetSecureSize(size_t iSize, size_t iExSize = INVALID_SIZE)
	{
		if ( iExSize != INVALID_SIZE )
		{
			iSize += iExSize;
			size_t l = iSize / 8;
			l = (l < 4) ? 4 : ((l > 1024) ? 1024 : l);
			iSize += l;
		}
		return iSize + 1;
	}

	/** 
	 * 代入(ASCII/SJIS to ASCII/SJIS)
	 *	@param d 0ならメモリ確保、1なら処理せずNULLを返す。
	 *	@param lpsz 元文字列
	 *	@param iExSize 拡張サイズ
	 *	@retval NULL以外 確保して文字列の入ったメモリ（開放はms_Deleteを使用すること）
	 *	@retval NULL	dが1だった時。
	 */
	static LPSTR ms_NewBuf(char d, LPCSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		if ( d == 1 )
		{
			return NULL; 
		}
		size_t iSize = STRLIB::GetLen(lpsz);
		LPSTR P = ms_New(char(), ms_GetSecureSize(iSize, iExSize)); 
		MemCopy(P, lpsz, iSize + 1);
		ms_CalcInfoPointer(P)->lLength = iSize;
		return P;
	}

	/** 
	 * 代入(UNICODE to ASCII/SJIS)
	 *	@param d ダミー
	 *	@param lpsz 元文字列
	 *	@param iExSize 拡張サイズ
	 *	@return 確保して文字列の入ったメモリ（開放はms_Deleteを使用すること）
	 */
	static LPSTR ms_NewBuf(char d, LPCWSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		int size = ::WideCharToMultiByte(CP_ACP, 0, lpsz, -1, NULL, 0, NULL, NULL);
		char* P = ms_New(char(), ms_GetSecureSize(size, iExSize)); 
		::WideCharToMultiByte(CP_ACP, 0, lpsz, TNB::ToInt(STRLIB::GetLen(lpsz) + 1), P, size, NULL, NULL);
		ms_CalcInfoPointer(P)->lLength = size - 1;
		return P; 
	}
	
	/** 
	 * 代入(ASCII/SJIS to UNICODE)
	 *	@param d ダミー
	 *	@param lpsz 元文字列
	 *	@param iExSize 拡張サイズ
	 *	@return 確保して文字列の入ったメモリ（開放はms_Deleteを使用すること）
	 */
	static LPWSTR ms_NewBuf(WCHAR d, LPCSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		int size = ::MultiByteToWideChar(CP_ACP, 0, lpsz, -1, NULL, 0); 
		WCHAR* P = ms_New(WCHAR(), ms_GetSecureSize(size, iExSize));
		::MultiByteToWideChar(CP_ACP, 0, lpsz, TNB::ToInt(STRLIB::GetLen(lpsz) + 1), P, size); 
		ms_CalcInfoPointer(P)->lLength = size - 1;
		return P; 
	}
	
	/** 
	 * 代入(UNICODE to UNICODE)
	 *	@param d 0ならメモリ確保、1なら処理せずNULLを返す。
	 *	@param lpsz 元文字列
	 *	@param iExSize 拡張サイズ
	 *	@retval NULL以外 確保して文字列の入ったメモリ（開放はms_Deleteを使用すること）
	 *	@retval NULL	dが1だった時。
	 */
	static LPWSTR ms_NewBuf(WCHAR d, LPCWSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		if ( d == 1 )
		{
			return NULL;
		}
		size_t iSize = STRLIB::GetLen(lpsz);
		LPWSTR P = ms_New(WCHAR(), ms_GetSecureSize(iSize, iExSize)); 
		MemCopy(P, lpsz, iSize + 1);
		ms_CalcInfoPointer(P)->lLength = iSize;
		return P;
	}

	/**
	 * NULL文字列
	 *	@return サイズ0の文字列。
	 */
	static const TYP* ms_NullStr(void) 
	{
		return static_cast<const TYP*>(gpStrNull);
	}

	// 小文字化
	static void ms_strlwr(LPSTR _p)
	{
		strlwr(_p);
	}

	// 小文字化
	static void ms_strlwr(LPWSTR _p)
	{
		wcslwr(_p);
	}
	
	// 大文字化
	static void ms_strupr(LPSTR _p)
	{
		strupr(_p);
	}

	// 大文字化
	static void ms_strupr(LPWSTR _p)
	{
		wcsupr(_p);
	}


	//--------------------------
	

	/** 初期化 */
	void m_InitBuf(void)
	{
		const TYP* P = static_cast<const TYP*>(gpStrNull); 
		m_lpBuf = const_cast<TYP*>(P); 
	}

	/**
	 * 管理情報のポインタ取得
	 *	@return ポインタ
	 */
	TInfo* m_GetInfoP(void) const
	{
		return ms_CalcInfoPointer(m_lpBuf);
	}

	/**
	 * 参照数のポインタ取得
	 *	@return 参照数のポインタ
	 */
	LONG* m_GetRefP(void) const
	{
		return &(m_GetInfoP()->lRef);
	}

	/**
	 * バッファサイズ取得
	 *	@return サイズの参照
	 */
	size_t m_GetBufSize(void) const
	{
		return m_GetInfoP()->lBufSize;
	}
	
	/** 
	 * バッファを設定
	 *	@note 情報があればEmptyしてから代入する。
	 *	@param P バッファ
	 */
	void m_SetBuf(TYP* P)
	{
		Empty();
		m_lpBuf = P;
	}
	
	/** 他とバッファを共有していたら分離する */
	void m_Separate(void)
	{
		if ( *(m_GetRefP()) > 1 )
		{
			//共有している,複製する必要有り
			TYP* P = ms_NewBuf(TYP(), m_lpBuf);
			m_SetBuf(P);
		}
	}

	/**
	 * 加算
	 *	@param lpText 文字列
	 *	@return 自分の参照
	 */
	template<typename T>
	CStrT& m_AddText(const T* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			TYP* pNew = ms_NewBuf(TYP(), lpText);
			size_t iAddSize = ms_CalcInfoPointer(pNew)->lLength;
			size_t iNowSize = GetLength();
			if ( *(m_GetRefP()) == 1 && m_GetBufSize() > iAddSize + iNowSize )
			{
				MemCopy(&m_lpBuf[iNowSize], pNew, iAddSize + 1);
			}
			else
			{
				TYP* P = ms_NewBuf(TYP(), m_lpBuf, iAddSize);
				MemCopy(&P[iNowSize], pNew, iAddSize + 1);
				m_SetBuf(P);
			}
			m_GetInfoP()->lLength = iAddSize + iNowSize;
			ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
			ms_Delete(pNew);
		}
		return *this;
	}

	TYP*	m_lpBuf;			///< データ
};



/**@ingroup STRING
 * ASCII文字列クラス
 *	@note UNICODE プロジェクトでもASCII/SJISが使えます。
 *	@note 一部の代入系オペレータは、違う文字コード文字列の代入も可能です。
 *	@note printf系や Format() Fmt() の ％s に対して、そのまま渡すことが出来ます。
 *	@attention インデックス指定をする際、トレイルバイト(文字コードの二バイト目)を指すと動作は保障できません。
 *	@see CStrT このテンプレートの TYP は「char」になります。
 *	@par必要ファイル
 *			TnbStr.h
 */
typedef TNB::CStrT<char>	CAscii;



/**\ingroup STRING
 * UNICODE文字列クラス
 *	@note ASCII プロジェクトでもUNICODEが使えます。
 *	@note 一部の代入系オペレータは、違う文字コード文字列の代入も可能です。
 *	@note printf系や Format() Fmt() の ％s に対して、そのまま渡すことが出来ます。
 *	@see CStrT このテンプレートの TYP は「WCHAR」になります。
 *	@par必要ファイル
 *			TnbStr.h
 */
typedef TNB::CStrT<WCHAR>	CUnicode;



/**@ingroup STRING
 * 文字列クラス
 *	@note プロジェクトで指定した文字コードが使えます。
 *	@note 一部の代入系オペレータは、違う文字コード文字列の代入も可能です。
 *	@note printf系や Format() Fmt() の ％s に対して、そのまま渡すことが出来ます。
 *	@attention インデックス指定をする際、トレイルバイト(文字コードの二バイト目)を指すと動作は保障できません。
 *	@see CStrT このテンプレートの TYP は「TCHAR」になります。
 *	@par必要ファイル
 *			TnbStr.h
 */
typedef TNB::CStrT<TCHAR>	CStr;



/**@ingroup STRING
 * 文字列加算(ASCII/SJIS用).
 *	@note 処理した文字列をリターンします。
 *	@param lpsz 元の文字列
 *	@param str 加算する文字列
 *	@return 文字列
 */
inline CAscii operator+(LPCSTR lpsz, const CAscii& str)
{
	CAscii s = lpsz;
	return s += str;
}

/**@ingroup STRING
 * 文字列加算(ASCII/SJIS用).
 *	@note 処理した文字列をリターンします。
 *	@param str1 元の文字列
 *	@param str2 加算する文字列
 *	@return 文字列
 */
inline CAscii operator+(const CAscii& str1, const CAscii& str2)
{
	CAscii s = str1;
	return s += str2;
}

/**@ingroup STRING
 * 文字列加算(UNICODE用)
 *	@note 処理した文字列をリターンします。
 *	@param lpsz 元の文字列
 *	@param str 加算する文字列
 *	@return 文字列
 */
inline CUnicode operator+(LPCWSTR lpsz, const CUnicode& str)
{
	CUnicode s = lpsz;
	return s += str;
}

/**@ingroup STRING
 * 文字列加算(UNICODE用)
 *	@note 処理した文字列をリターンします。
 *	@param str1 元の文字列
 *	@param str2 加算する文字列
 *	@return 文字列
 */
inline CUnicode operator+(const CUnicode& str1, const CUnicode& str2)
{
	CUnicode s = str1;
	return s += str2;
}

/**@ingroup STRING
 * シリアライズ(ASCII/SJIS).
 *	@note 指定のシリアライザーに本オブジェクトの保持している情報を出力します。
 *	@param[out] _sr シリアライザー
 *	@param[in] P 文字列
 *	@return シリアライザー
 */
inline ISerializer& operator<<(ISerializer& _sr, const char* P)
{
	TRACE0("Serialize[string(ascii)]\n");
	size_t l = STRLIB::GetLen(P);
	ASSERTLIB( l >= 0 );
	_sr << l << l;
	size_t iSize = (l + 1) * sizeof(char);
	_sr.AddRaw(iSize, P);
	return _sr;
}

/**@ingroup STRING
 * シリアライズ(ASCII/SJIS).
 *	@note 指定のシリアライザーに本オブジェクトの保持している情報を出力します。
 *	@param[out] _sr シリアライザー
 *	@param[in] t 文字列
 *	@return シリアライザー
 */
inline ISerializer& operator<<(ISerializer& _sr, const CAscii& t)		
{
	const char* P = t;
	return operator<<(_sr, P);
}

/**@ingroup STRING
 * デシリアライズ(ASCII/SJIS).
 *	@note 指定のデシリアライザーからに本オブジェクトに情報を取り込みます。
 *	@param[in] ds デシリアライザー
 *	@param[out] _t 文字列
 *	@return デシリアライザー
 *	@throw CInvalidParamException 長さ情報が異常の時スローされます。
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, CAscii& _t)		
{
	TRACE0("Deserialize[string(ascii)]\n");
	int l1, l2;
	ds >> l1 >> l2;
	if ( l1 != l2 )
	{
		throw CInvalidParamException();
	}
	l1++;
	ds.GetRaw(l1 * sizeof(char), _t.GetBuffer(l1 * sizeof(char)));
	_t.ReleaseBuffer();
	return ds;
}

/**@ingroup STRING
 * シリアライズ(UNICODE).
 *	@note 指定のシリアライザーに本オブジェクトの保持している情報を出力します。
 *	@param[out] _sr シリアライザー
 *	@param[in] P 文字列
 *	@return シリアライザー
 */
inline ISerializer& operator<<(ISerializer& _sr, const WCHAR* P)
{
	TRACE0("Serialize[string(unicode)]\n");
	size_t l = STRLIB::GetLen(P);
	ASSERTLIB( l >= 0 );
	_sr << l << l;
	size_t iSize = (l + 1) * sizeof(WCHAR);
	_sr.AddRaw(iSize, P);
	return _sr;
}

/**@ingroup STRING
 * シリアライズ(UNICODE).
 *	@note 指定のシリアライザーに本オブジェクトの保持している情報を出力します。
 *	@param[out] _sr シリアライザー
 *	@param[in] t 文字列
 *	@return シリアライザー
 */
inline ISerializer& operator<<(ISerializer& _sr, const CUnicode& t)		
{
	const WCHAR* P = t;
	return operator<<(_sr, P);
}

/**@ingroup STRING
 * デシリアライズ(UNICODE).
 *	@note 指定のデシリアライザーからに本オブジェクトに情報を取り込みます。
 *	@param[in] ds デシリアライザー
 *	@param[out] _t 文字列
 *	@return デシリアライザー
 *	@throw CInvalidParamException 長さ情報が異常の時スローされます。
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, CUnicode& _t)	
{
	TRACE0("Deserialize[string(unicode)]\n");
	int l1, l2;
	ds >> l1 >> l2;
	if ( l1 != l2 )
	{
		throw CInvalidParamException();
	}
	l1++;
	ds.GetRaw(l1 * sizeof(WCHAR), _t.GetBuffer(l1 * sizeof(WCHAR)));
	_t.ReleaseBuffer();
	return ds;
}



namespace HASH
{
	/** STRING
	 * [HASH] ハッシュ値計算.
	 *	@note ASCII/SJIS文字列用です。
	 *	@param str CAscii
	 *	@return ハッシュ値
	 */
	inline DWORD CalcHash(const CAscii& str)
	{
		return CalcHash(str.GetLength(), str.operator LPCSTR());
	}

	/** STRING
	 * [HASH] ハッシュ値計算.
	 *	@note UNICODE文字列用です。
	 *	@param str CUnicode
	 *	@return ハッシュ値
	 */
	inline DWORD CalcHash(const CUnicode& str)
	{
		return CalcHash(str.GetLength() * 2, str.operator LPCWSTR());
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")


