#pragma once
/**
 *@file
 * BIT毎のフラグ管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbStaticMap.h"



//TNB Library
namespace TNB 
{



/**@ingroup VALUE
 * BITフラグ管理抽象クラス
 *
 *		BITを使ったフラグ情報を管理するための抽象クラスです。
 *		BITのチェックを簡易化します。
 *		Table() を実装することで、文字列化をサポートします。
 *
 *	@par必要ファイル
 *			TnbBitFlag.h
 * 
 *	@date 10/04/15	新規作成
 */
class CAbstructBitFlag
{
public:

	/**
	 * コンストラクタ.
	 *	@param flag 初期値。省略すると０が指定されます。
	 */
	CAbstructBitFlag(DWORD flag = 0) : m_flag(flag)
	{
	}

	/**
	 * [取得] フラグ取得.
	 *	@return フラグ
	 */
	operator DWORD(void) const
	{
		return m_flag;
	}

	/**
	 * [確認] 確認.
	 *	@note BITが複数経っている場合、すべて立っていないと有効になりません。
	 *	@param fg フラグ
	 *	@retval true 有効
	 *	@retval false 無効
	 */
	bool IsValid(DWORD fg) const
	{
		return ((m_flag & fg) == fg);
	}

	/**
	 * [確認] 確認.
	 *	@param bn ビットNo
	 *	@retval true 有効
	 *	@retval false 無効
	 */
	bool IsValidByBitNo(DWORD bn) const
	{
		return (m_flag & _BIT(bn)) != 0;
	}

	/**
	 * [設定] 有効.
	 *	@param fg フラグ
	 */
	void Valid(DWORD fg)
	{
		m_flag |= fg;
	}

	/**
	 * [設定] 無効.
	 *	@param fg フラグ
	 */
	void Invalid(DWORD fg)
	{
		m_flag &= ~fg;
	}

	/**
	 * [取得] 文字列化.
	 *		本インスタンスが保持しているフラグを Table() を使って文字列化します。
	 *	@return 文字列
	 */
	CStr ToString(void) const
	{
		CStr s;
		DWORD flag = m_flag;
		DWORD bf;
		CStr name;
		INDEX index = 0;
		while ( Table(bf, name, index) )
		{
			if ( (flag & bf) == bf )
			{
				s += _T("(") + name + _T(")");
				flag &= ~bf;
			}
			index++;
		}
		return s;
	}

protected:

	/**
	 * [取得] テーブル取得.
	 *	@param[out] _bf フラグ値が取得されます。
	 *	@param[out] _name フラグ名が取得されます。
	 *	@param[in] index インデックス
	 *	@retval true 取得成功
	 *	@retval false インデックス無効
	 */
	virtual bool Table(DWORD& _bf, CStr& _name, INDEX index) const = 0;

private:
	DWORD	m_flag;	///< フラグ値

};



/**@ingroup VALUE
 * Windowスタイル管理クラス
 *
 *	@par必要ファイル
 *			TnbBitFlag.h
 * 
 *	@date 10/04/15	新規作成
 */
class CWindowStyleBitFlag : public CAbstructBitFlag
{
	DEFSUPER(CAbstructBitFlag);
public:

	/**
	 * コンストラクタ.
	 *	@param flag 初期値。省略すると０が指定されます。
	 */
	CWindowStyleBitFlag(DWORD flag = 0) : _super(flag)
	{
	}

protected:

	/**
	 * [取得] テーブル取得.
	 *	@param[out] _bf フラグ値が取得されます。
	 *	@param[out] _name フラグ名が取得されます。
	 *	@param[in] index インデックス
	 *	@retval true 取得成功
	 *	@retval false インデックス無効
	 */
	virtual bool Table(DWORD& _bf, CStr& _name, INDEX index) const
	{
		const static CStaticMapT<DWORD ,LPCTSTR> maps[] = 
		{
			#ifndef _TnbDOXYGEN
			#define _X(XX) {XX, _T(#XX)}
			_X(WS_POPUP),
			_X(WS_CHILD),
			_X(WS_MINIMIZE),
			_X(WS_VISIBLE),
			_X(WS_DISABLED),
			_X(WS_CLIPSIBLINGS),
			_X(WS_CLIPCHILDREN),
			_X(WS_MAXIMIZE),
			_X(WS_CAPTION),
			_X(WS_BORDER),
			_X(WS_DLGFRAME),
			_X(WS_VSCROLL),
			_X(WS_HSCROLL),
			_X(WS_SYSMENU),
			_X(WS_THICKFRAME),
			_X(WS_MINIMIZEBOX),
			_X(WS_MAXIMIZEBOX),
			#undef _X
			#endif //_TnbDOXYGEN
		};
		LPCTSTR lpsz = NULL;
		if ( CStaticMapT<DWORD, LPCTSTR>::Get(_bf, lpsz, index, maps) )
		{
			_name = lpsz;
			return true;
		}
		return false;
	}
};



}; //TNB
