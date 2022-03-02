#pragma once
/**
 *@file
 * Boolean型情報管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



/**@ingroup VALUE
 * Boolean型クラス
 *
 *		宣言時、falseで初期化されており、bool BOOL 共にボーダーレスです。
 *		特に「BOOL」を扱うWin32API/MFCに対する場合に有効です。
 *
 *	@par必要ファイル
 *			TnbBoolean.h
 * 
 *	@date	06/01/01 新規
 */
class CBoolean
{
	bool m_boValue;			///< 状態保持変数 

public:
	
	/**	コンストラクタ */
	CBoolean(void) : m_boValue(false) {}

	/**	
	 * コンストラクタ 
	 *	@param boIn	　初期状態指定
	 */
	CBoolean(bool boIn) : m_boValue(boIn) {}

	/**
	 * [確認] 比較.
	 *		引き算をすると、比較結果が返ります。
	 *	@param b 比較対照
	 *	@retval ０		自分は b と同じ
	 *	@retval １以上	自分は b より大きい
	 *	@retval ０未満	自分は b より小さい
	 */
	int operator-(const CBoolean& b) const
	{
		return m_boValue - b.m_boValue; 
	}

	/**
	 * [確認] 比較.
	 *	@param b 比較対照
	 *	@retval true 同じ
	 *	@retval false 異なる
	 */
	bool operator==(const CBoolean& b) const 
	{
		return m_boValue == b.m_boValue; 
	}

	/**
	 * [確認] 比較.
	 *	@param b 比較対照
	 *	@retval true 異なる
	 *	@retval false 同じ
	 */
	bool operator!=(const CBoolean& b) const
	{
		return m_boValue != b.m_boValue; 
	}

	/**
	 * [設定] 代入
	 *	@param boIn	　設定状態指定
	 *	@return 設定後状態
	 */
	bool operator=(bool boIn)
	{
		return m_boValue = boIn;
	}

	/**
	 * [取得] 状態反転取得
	 *	@return 反転した状態。
	 */
	bool operator !(void) const
	{
		return ! m_boValue; 
	}

	/**
	 * [取得] 状態取得
	 *	@return 状態
	 */
	operator bool&(void)
	{
		return m_boValue;
	}

	/**
	 * [取得] 状態取得
	 *	@return 状態
	 */
	operator const bool&(void) const 
	{
		return m_boValue;
	}

	/**
	 * [取得] 状態取得
	 *	@retval true 状態が真の時
	 *	@retval false 状態が偽の時
	 */
	bool IsTrue(void) const
	{
		return m_boValue;
	}

	/**
	 * [取得] 状態取得
	 *	@retval false 状態が真の時
	 *	@retval true 状態が偽の時
	 */
	bool IsFalse(void) const
	{
		return ! m_boValue;
	}
	
	/**
	 * [取得] 文字列取得.
	 *		状態を文字列に変換します。
	 *	@retval ”true”	状態が真の時
	 *	@retval ”false”	状態が偽の時
	 */
	LPCTSTR ToString(void) const
	{
		return m_boValue ? _T("true") : _T("false");
	}

	#ifdef _WINDEF_

	/**	
	 * コンストラクタ。	
	 *	@param boIn	　初期状態指定。
	 */
	CBoolean(BOOL boIn) : m_boValue(!!boIn) {}

	/**
	 * [設定] 代入
	 *	@param boIn	　設定状態指定
	 *	@return 設定後状態
	 */
	bool operator=(BOOL boIn)
	{
		return m_boValue = !! boIn;
	}

	/**
	 * [取得] BOOL型取得
	 *	@return 状態
	 */
	operator BOOL(void) const
	{
		return m_boValue; 
	}

	#endif//_WINDEF_

private:
	friend class CBooleanTest;
};



/**@ingroup VALUE
 * Boolean型クラス
 *
 *		宣言時、falseで初期化されており、bool BOOL 共にボーダーレスです。
 *		特に「BOOL」を扱うWin32API/MFCに対する場合に有効です。
 *
 *	@par必要ファイル
 *			TnbBoolean.h
 * 
 *	@date	06/01/01 新規
 */
typedef TNB::CBoolean Bool;



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")



}; // TNB
