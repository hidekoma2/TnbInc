#pragma once
/**
 *@file
 * �ȈՃe�X�g�P�[�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//TNB Library
namespace TNB
{



#ifdef TC_TRUE
 #error T-TestCase �Ɠ����ɂ͎g�p�ł��܂���B
#endif



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#define __TTC3(XX,P1,P2,P3)	do { char* _b=new char[1024]; TNB::STRLIB::PrintF(_b,1024,XX,P1,P2,P3); ::OutputDebugStringA(_b); delete[] _b; } while ( false )
#endif

/**@ingroup ETC
 * �ȈՃe�X�g�P�[�X�t���[�����[�N�p �]���}�N��.
 *	@note {@link TNB::CTinyTestCase CTinyTestCase} �̃T�u�N���X�� {@link TNB::CTinyTestCase::Test1() Test1()} �` {@link TNB::CTinyTestCase::Test5 Test5()} ���Ŏg�p���܂��B
 *	@param X bool �ɂȂ�]����.
 */
#define TC_TRUE(X) { bool _R=X; if ( _R ) {::OutputDebugStringA("     �� " #X ";\n"); }\
					else { ::OutputDebugStringA("     �~ " #X ";\n"); ::Sleep(10); __TTC3("%s(%d): %s\n", __FILE__, __LINE__, "[" #X "]�ŃA�T�[�g���܂����B"); \
					ASSERT1(_R, NULL, "%s", "[" #X "]\n\n��L�ӏ��ŃA�T�[�g���܂����B"); } m_TcTrue(_R); }

/**@ingroup ETC
 * �ȈՃe�X�g�P�[�X�t���[�����[�N�p �]���}�N��.
 *	@note {@link TNB::CTinyTestCase CTinyTestCase} �̃T�u�N���X�� {@link TNB::CTinyTestCase::Test1() Test1()} �` {@link TNB::CTinyTestCase::Test5 Test5()} ���Ŏg�p���܂��B
 *	@param X void �ɂȂ�]����.
 */
#define TC_VOID(X) ::OutputDebugString(_T("        ") _T(#X) _T(";\n")); X;



/**@ingroup ETC
 * �ȈՃe�X�g�P�[�X�t���[�����[�N.
 *
 *		�e�X�g�P�[�X���ȈՓI�ɍ쐬����t���[���N���X�ł��B
 *
 *	Test1() �` Test5() �� TC_TRUE() ���g�����e�X�g�P�[�X���L�q���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTinyTestcase.h
 *
 *	@date 09/04/21 �V�K�쐬
 *	@date 11/07/07 Run() �ɖ߂�l��t��
 */
class CTinyTestCase
{
public:

	/**
	 * [���s] �e�X�g�J�n.
	 *	@retval true �S�Đ���
	 *	@retval false ��ł����s
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
	/// �e�X�g�P
	virtual void Test1(void) = 0;
	/// �e�X�g�Q
	virtual void Test2(void) {}
	/// �e�X�g�R
	virtual void Test3(void) {}
	/// �e�X�g�S
	virtual void Test4(void) {}
	/// �e�X�g�T
	virtual void Test5(void) {}

	/**
	 * [�\��] �e�X�g�P�[�X����
	 *	@param lpsz �P�[�X����
	 */
	void TC_CAPTION_CASE(LPCSTR lpsz)
	{
		::OutputDebugString(_T("//-----------------------\n"));
		::OutputDebugString(_T("//") + CStr(lpsz) + _T("\n"));
	}
	/**
	 * [�\��] �e�X�g����
	 *	@param lpsz ����
	 */
	void TC_CAPTION_TEST(LPCSTR lpsz)
	{
		::OutputDebugString(CStr(_T("   //")) + lpsz + _T("\n"));
	}
	/**
	 * [�]��] �]��
	 *	@attention �{���\�b�h���g�킸�A TC_TRUE() �}�N�����g�p���邱�ƁB
	 *	@param r �]������
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
			::OutputDebugString(CStr::Fmt(_T("���� Total = %d  NG = %d ����\n"), m_totalCount, m_totalCount - m_trueCount));
		}
		return m_totalCount == m_trueCount;
	}
	DWORD m_totalCount;		///< �e�X�g����
	DWORD m_trueCount;		///< �e�X�gTRUE��
};



}; // TNB