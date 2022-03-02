#pragma once
/**
 *@file
 * 属性付き文字列管理関係のヘッダ
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
 * 属性付き文字列管理
 *
 *		ASCII/SJIS か UNICODE（プロジェクトの設定による）の一文字ごとに対になる、
 *		属性（TYP)を管理します。
 *
 *	@note	printf系や Format() Fmt() の ％s に対して、そのまま渡せません。
 *			CStr に変換してから渡してください。
 *
 *	@param TYP 属性の型
 *
 *	@par必要ファイル
 *			TnbAttributedStr.h
 * 
 *	@date 06/10/10	新規作成
 */
template<typename TYP>
class CAttributedStrT
{
	CStr			m_str;		///< 文字列
	CVectorT<TYP>	m_vattr;	///< 属性

public:

	/**
	 * コンストラクタ
	 *	@note 属性は全て TYP() になります。
	 *	@param lpsz 代入文字列
	 */
	explicit CAttributedStrT(LPCTSTR lpsz = NULL) : m_str(lpsz)
	{
		m_vattr.SetSize(m_str.GetLength());
	}

	/**
	 * コピーコンストラクタ
	 *	@param as コピー元。
	 */
	CAttributedStrT(const CAttributedStrT& as) : m_str(as.m_str), m_vattr(as.m_vattr)
	{
		ASSERTLIB(m_vattr.GetSize() == m_str.GetLength());
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元。
	 *	@return 自分の参照。
	 */
	CAttributedStrT& operator=(const CAttributedStrT& other)
	{
		m_str = other.m_str;
		m_vattr = other.m_vattr;
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@note 属性は全て TYP() になります。
	 *	@param lpsz 代入文字列
	 *	@return 自分の参照。
	 */
	CAttributedStrT& operator=(LPCTSTR lpsz)
	{
		m_str = lpsz;
		m_vattr.RemoveAll();
		m_vattr.SetSize(m_str.GetLength());
		return *this;
	}

	/** [削除] 空化 */
	void Empty(void)
	{
		m_str.Empty();
		m_vattr.RemoveAll();
	}

	/**
	 * [取得] 参照
	 *	@return 持っている文字列の先頭ポインタ
	 */
	LPCTSTR Refer(void) const
	{
		return m_str;
	}

	/**
	 * [追加] 加算代入オペレータ
	 *	@param as 追加内容
	 *	@return 自分の参照
	 */
	CAttributedStrT& operator+=(const CAttributedStrT& as)
	{
		m_str += as.m_str;
		m_vattr += as.m_vattr;
		ASSERTLIB(m_vattr.GetSize() == static_cast<size_t>(m_str.GetLength()));
		return *this;
	}

	/**
	 * [追加] 加算代入オペレータ
	 *	@note 追加された文字の属性は全て TYP() になります。
	 *	@param lpsz 文字列
	 *	@return 自分の参照
	 */
	CAttributedStrT& operator+=(LPCTSTR lpsz)
	{
		m_str += lpsz;
		m_vattr.SetSize(m_str.GetLength());
		ASSERTLIB(m_vattr.GetSize() == m_str.GetLength());
		return *this;
	}

	/**
	 * [操作] 一文字追加
	 *	@param c 追加する文字。
	 *	@param t その属性。
	 *	@return 自分の参照
	 */
	CAttributedStrT& AddAt(TCHAR c, TYP t)
	{
		m_str += c;
		m_vattr.Add(t);
		ASSERTLIB(m_vattr.GetSize() == m_str.GetLength());
		return *this;
	}

	/**
	 * [操作] 一文字置き換え
	 *	@param index 置き換えインデックス
	 *	@param c 置き換える文字。
	 *	@param t その属性。
	 */
	void SetAt(INDEX index, TCHAR c, TYP t)
	{
		m_str.SetAt(index, c);
		m_vattr.Set(index, t);
	}

	/**
	 * [挿入] 文字挿入
	 *	@param index 挿入開始インデックス
	 *	@param c 文字。
	 *	@param t その属性。
	 */
	void InsertAt(INDEX index, TCHAR c, TYP t)
	{
		m_str.InsertAt(index, c);
		m_vattr.Insert(index, t);
	}

	/**
	 * [挿入] 文字列挿入
	 *	@param index 挿入開始インデックス
	 *	@param lpsz 文字列。
	 *	@param t その属性(全てこの属性になります)。
	 */
	void Insert(INDEX index, LPCTSTR lpsz, TYP t=TYP())
	{
		m_str.Insert(index, lpsz);
		loop ( i, STRLIB::GetLen(lpsz) )
		{
			m_vattr.Insert(index, t);
		}
	}

	/**
	 * [挿入] 文字列挿入
	 *	@param index 挿入開始インデックス
	 *	@param as 文字列。
	 */
	void Insert(INDEX index, const CAttributedStrT& as)
	{
		m_str.Insert(index, as.m_str);
		loop ( i, as.m_vattr.GetSize() )
		{
			m_vattr.Insert(index + i, as.m_vattr[i]);
		}
	}

	/**
	 * [操作] 文字削除.
	 *	@note リードバイト(文字コードの一バイト目)を指定すると、続くトレイルバイトも削除されます。
	 *	@param index 削除するインデックス
	 *	@param iLen 削除する文字数(長さを超えている場合、指定インデックス以下がすべて削除されます)
	 *	@retval 0 削除していない
	 *	@retval 1以上 削除した文字数。ASCII/SJISの場合、iLen と同じ値になるとは限りません。
	 */
	int Delete(INDEX index, int iLen = 1)
	{
		int l = m_str.Delete(index, iLen);
		loop ( i, l )
		{
			m_vattr.Remove(index);
		}
		return l;
	}

	/**
	 * [取得] 一文字取得
	 *	@param index インデックス。1文字目がインデックス０になります。
	 *	@return インデックスの位置の文字。
	 */
	TCHAR GetAt(INDEX index) const
	{
		return m_str.GetAt(index);
	}

	/**
	 * [取得] 属性取得
	 *	@param index インデックス。1文字目がインデックス０になります。
	 *	@return インデックスの位置の属性。
	 */
	TYP GetAttribute(INDEX index) const
	{
		return m_vattr.Get(index);
	}

	/**
	 * [設定] 属性設定
	 *	@param index インデックス。1文字目がインデックス０になります。
	 *	@param t 設定する属性
	 */
	void SetAttribute(INDEX index, TYP t)
	{
		m_vattr.Set(index, t);
	}

	/**
	 * [取得] 文字列長
	 *	@return 文字列長
	 */
	size_t GetLength(void) const
	{
		return m_str.GetLength();
	}

	/**
	 * [確認] 空チェック
	 *	@retval true	空っぽ
	 *	@retval false	データ有り
	 */
	bool IsEmpty(void) const
	{
		return m_str.IsEmpty();
	}

	/**
	 * [確認] 検索
	 *	@param lpsz 検索する文字列
	 *	@param iFromIndex 検索を開始するインデックス。省略すると頭からになります。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	int Find(LPCTSTR lpsz, INDEX iFromIndex = 0) const
	{
		return m_str.Find(lpsz, iFromIndex);
	}

	/**
	 * [確認] 検索
	 *	@param c 検索する文字
	 *	@param iFromIndex 検索を開始するインデックス。省略すると頭からになります。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	int Find(TCHAR c, INDEX iFromIndex = 0) const
	{
		return m_str.Find(c, iFromIndex);
	}

	/**
	 * [確認] 属性検索
	 *	@param t 検索する属性
	 *	@param iFromIndex 検索を開始するインデックス。省略すると頭からになります。
	 *	@retval -1 未発見
	 *	@retval 0以上 発見インデックス
	 */
	int FindAttribute(TYP t, INDEX iFromIndex = 0) const
	{
		return m_vattr.Find(c, iFromIndex);
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")

