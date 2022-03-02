#pragma once
/**
 *@file
 * Boolean�^���Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



/**@ingroup VALUE
 * Boolean�^�N���X
 *
 *		�錾���Afalse�ŏ���������Ă���Abool BOOL ���Ƀ{�[�_�[���X�ł��B
 *		���ɁuBOOL�v������Win32API/MFC�ɑ΂���ꍇ�ɗL���ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBoolean.h
 * 
 *	@date	06/01/01 �V�K
 */
class CBoolean
{
	bool m_boValue;			///< ��ԕێ��ϐ� 

public:
	
	/**	�R���X�g���N�^ */
	CBoolean(void) : m_boValue(false) {}

	/**	
	 * �R���X�g���N�^ 
	 *	@param boIn	�@������Ԏw��
	 */
	CBoolean(bool boIn) : m_boValue(boIn) {}

	/**
	 * [�m�F] ��r.
	 *		�����Z������ƁA��r���ʂ��Ԃ�܂��B
	 *	@param b ��r�Ώ�
	 *	@retval �O		������ b �Ɠ���
	 *	@retval �P�ȏ�	������ b ���傫��
	 *	@retval �O����	������ b ��菬����
	 */
	int operator-(const CBoolean& b) const
	{
		return m_boValue - b.m_boValue; 
	}

	/**
	 * [�m�F] ��r.
	 *	@param b ��r�Ώ�
	 *	@retval true ����
	 *	@retval false �قȂ�
	 */
	bool operator==(const CBoolean& b) const 
	{
		return m_boValue == b.m_boValue; 
	}

	/**
	 * [�m�F] ��r.
	 *	@param b ��r�Ώ�
	 *	@retval true �قȂ�
	 *	@retval false ����
	 */
	bool operator!=(const CBoolean& b) const
	{
		return m_boValue != b.m_boValue; 
	}

	/**
	 * [�ݒ�] ���
	 *	@param boIn	�@�ݒ��Ԏw��
	 *	@return �ݒ����
	 */
	bool operator=(bool boIn)
	{
		return m_boValue = boIn;
	}

	/**
	 * [�擾] ��Ԕ��]�擾
	 *	@return ���]������ԁB
	 */
	bool operator !(void) const
	{
		return ! m_boValue; 
	}

	/**
	 * [�擾] ��Ԏ擾
	 *	@return ���
	 */
	operator bool&(void)
	{
		return m_boValue;
	}

	/**
	 * [�擾] ��Ԏ擾
	 *	@return ���
	 */
	operator const bool&(void) const 
	{
		return m_boValue;
	}

	/**
	 * [�擾] ��Ԏ擾
	 *	@retval true ��Ԃ��^�̎�
	 *	@retval false ��Ԃ��U�̎�
	 */
	bool IsTrue(void) const
	{
		return m_boValue;
	}

	/**
	 * [�擾] ��Ԏ擾
	 *	@retval false ��Ԃ��^�̎�
	 *	@retval true ��Ԃ��U�̎�
	 */
	bool IsFalse(void) const
	{
		return ! m_boValue;
	}
	
	/**
	 * [�擾] ������擾.
	 *		��Ԃ𕶎���ɕϊ����܂��B
	 *	@retval �htrue�h	��Ԃ��^�̎�
	 *	@retval �hfalse�h	��Ԃ��U�̎�
	 */
	LPCTSTR ToString(void) const
	{
		return m_boValue ? _T("true") : _T("false");
	}

	#ifdef _WINDEF_

	/**	
	 * �R���X�g���N�^�B	
	 *	@param boIn	�@������Ԏw��B
	 */
	CBoolean(BOOL boIn) : m_boValue(!!boIn) {}

	/**
	 * [�ݒ�] ���
	 *	@param boIn	�@�ݒ��Ԏw��
	 *	@return �ݒ����
	 */
	bool operator=(BOOL boIn)
	{
		return m_boValue = !! boIn;
	}

	/**
	 * [�擾] BOOL�^�擾
	 *	@return ���
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
 * Boolean�^�N���X
 *
 *		�錾���Afalse�ŏ���������Ă���Abool BOOL ���Ƀ{�[�_�[���X�ł��B
 *		���ɁuBOOL�v������Win32API/MFC�ɑ΂���ꍇ�ɗL���ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBoolean.h
 * 
 *	@date	06/01/01 �V�K
 */
typedef TNB::CBoolean Bool;



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")



}; // TNB
