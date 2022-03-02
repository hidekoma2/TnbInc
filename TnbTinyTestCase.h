#pragma once
/**
 *@file
 * 簡易テストケース関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//TNB Library
namespace TNB
{



#ifdef TC_TRUE
 #error T-TestCase と同時には使用できません。
#endif



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#define __TTC3(XX,P1,P2,P3)	do { char* _b=new char[1024]; TNB::STRLIB::PrintF(_b,1024,XX,P1,P2,P3); ::OutputDebugStringA(_b); delete[] _b; } while ( false )
#endif

/**@ingroup ETC
 * 簡易テストケースフレームワーク用 評価マクロ.
 *	@note {@link TNB::CTinyTestCase CTinyTestCase} のサブクラスの {@link TNB::CTinyTestCase::Test1() Test1()} ～ {@link TNB::CTinyTestCase::Test5 Test5()} 内で使用します。
 *	@param X bool になる評価式.
 */
#define TC_TRUE(X) { bool _R=X; if ( _R ) {::OutputDebugStringA("     ○ " #X ";\n"); }\
					else { ::OutputDebugStringA("     × " #X ";\n"); ::Sleep(10); __TTC3("%s(%d): %s\n", __FILE__, __LINE__, "[" #X "]でアサートしました。"); \
					ASSERT1(_R, NULL, "%s", "[" #X "]\n\n上記箇所でアサートしました。"); } m_TcTrue(_R); }

/**@ingroup ETC
 * 簡易テストケースフレームワーク用 評価マクロ.
 *	@note {@link TNB::CTinyTestCase CTinyTestCase} のサブクラスの {@link TNB::CTinyTestCase::Test1() Test1()} ～ {@link TNB::CTinyTestCase::Test5 Test5()} 内で使用します。
 *	@param X void になる評価式.
 */
#define TC_VOID(X) ::OutputDebugString(_T("        ") _T(#X) _T(";\n")); X;



/**@ingroup ETC
 * 簡易テストケースフレームワーク.
 *
 *		テストケースを簡易的に作成するフレームクラスです。
 *
 *	Test1() ～ Test5() に TC_TRUE() を使ったテストケースを記述します。
 *
 *	@par必要ファイル
 *			TnbTinyTestcase.h
 *
 *	@date 09/04/21 新規作成
 *	@date 11/07/07 Run() に戻り値を付加
 */
class CTinyTestCase
{
public:

	/**
	 * [実行] テスト開始.
	 *	@retval true 全て成功
	 *	@retval false 一つでも失敗
	 */
	bool Run(void)
	{
		bool r = true;
		r &= m_Test(&CTinyTestCase::Test1);
		r &= m_Test(&CTinyTestCase::Test2);
		r &= m_Test(&CTinyTestCase::Test3);
		r &= m_Test(&CTinyTestCase::Test4);
		r &= m_Test(&CTinyTestCase::Test5);
		return r;
	}

protected:
	/// テスト１
	virtual void Test1(void) = 0;
	/// テスト２
	virtual void Test2(void) {}
	/// テスト３
	virtual void Test3(void) {}
	/// テスト４
	virtual void Test4(void) {}
	/// テスト５
	virtual void Test5(void) {}

	/**
	 * [表示] テストケース説明
	 *	@param lpsz ケース説明
	 */
	void TC_CAPTION_CASE(LPCSTR lpsz)
	{
		::OutputDebugString(_T("//-----------------------\n"));
		::OutputDebugString(_T("//") + CStr(lpsz) + _T("\n"));
	}
	/**
	 * [表示] テスト説明
	 *	@param lpsz 説明
	 */
	void TC_CAPTION_TEST(LPCSTR lpsz)
	{
		::OutputDebugString(CStr(_T("   //")) + lpsz + _T("\n"));
	}
	/**
	 * [評価] 評価
	 *	@attention 本メソッドを使わず、 TC_TRUE() マクロを使用すること。
	 *	@param r 評価結果
	 */
	void m_TcTrue(bool r)
	{
		m_totalCount++;
		if ( r )
		{
			m_trueCount++;
		}
	}
private:
	typedef void (CTinyTestCase::*P_Test)(void);
	bool m_Test(P_Test t)
	{
		m_totalCount = 0;
		m_trueCount = 0;
		(this->*t)();
		if ( m_totalCount != 0 )
		{
			::OutputDebugString(CStr::Fmt(_T("★★ Total = %d  NG = %d ★★\n"), m_totalCount, m_totalCount - m_trueCount));
		}
		return m_totalCount == m_trueCount;
	}
	DWORD m_totalCount;		///< テスト総数
	DWORD m_trueCount;		///< テストTRUE数
};



}; // TNB