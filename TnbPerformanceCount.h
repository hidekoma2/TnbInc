#pragma once
/**
 *@file
 * �o�ߎ��ԊǗ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * �o�ߎ��ԊǗ��i������\�j
 *
 *		���Ԃ̌o�߂������N���X�ł��B
 *
 *		API �� QueryPerformanceCounter() ���g�p���Ă��܂��B
 * 
 *	@attention �{�N���X�Ŏg�p���Ă��� API �� CPU �N���b�N�����x�ɉe�����܂��B
 *			�܂��A���I�� CPU �N���b�N���ύX�����\�������邽�߁A���m�Ȏ��ԂɂȂ�Ȃ��\�����L��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPerformanceCount.h
 *
 *	@date 11/08/24	�V�K�쐬
 */
class CPerformanceCount
{
public:

	/**
	 * �R���X�g���N�^
	 *	@note ���݂̎��Ԃ��L�����܂��B
	 */
	CPerformanceCount(void)
	{
		LARGE_INTEGER li;
		m_isValid = !! ::QueryPerformanceFrequency(&m_freq);
		m_isValid &= !! ::QueryPerformanceCounter(&m_count);
		::QueryPerformanceCounter(&li);
		m_apiCount = li.QuadPart - m_count.QuadPart;
		ASSERT0( m_isValid, "CPerformanceCount", "PerformanceCount ���g�p�ł��܂���." );
		//TRACE2("PerformanceCount Resolution= %d(ns), %d\n", GetResolutionNanoCount(), static_cast<DWORD>(m_apiCount) );
		::QueryPerformanceCounter(&m_count);
		m_count.QuadPart += m_apiCount;
	}

	/**
	 * [�m�F] �L���H
	 *	@retval true �{�C���X�^���X�͗L���B
	 *	@retval false �����B
	 */
	bool IsValid(void) const
	{
		return m_isValid;
	}

	/**
	 * [�ݒ�] ���Z�b�g.
	 *		���݂̎��Ԃ��L�����܂��B
	 */
	void Reset(void)
	{
		::QueryPerformanceFrequency(&m_freq);
		::QueryPerformanceCounter(&m_count);
		m_count.QuadPart += m_apiCount;
	}

	/**
	 * [�擾] �o�ߎ��Ԏ擾.
	 *		�R���X�g���N�^�A���邢�� Reset() ����̎���(��s)��Ԃ��܂��B
	 *	@return �o�ߎ���(��s)
	 */
	LONGLONG GetPassedCount(void) const
	{
		LARGE_INTEGER	c;
		::QueryPerformanceCounter(&c);
		LONGLONG d = c.QuadPart - m_count.QuadPart;
		return (d * 1000 * 1000 / m_freq.QuadPart);
	}

	/**
	 * [�擾] �o�ߎ��Ԏ擾.
	 *		�R���X�g���N�^�A���邢�� Reset() ����̎���(ns)��Ԃ��܂��B
	 *	@return �o�ߎ���(ns)
	 */
	LONGLONG GetPassedNanoCount(void) const
	{
		LARGE_INTEGER	c;
		::QueryPerformanceCounter(&c);
		LONGLONG d = c.QuadPart - m_count.QuadPart;
		return (d * 1000 * 1000 * 1000 / m_freq.QuadPart);
	}

	/**
	 * [�擾] ����x���Ԏ擾.
	 *		����x����(ns)��Ԃ��܂��B
	 *	@note �C���X�^���X�\�z���A Reset() ���̕���x���Ԃ�Ԃ��܂��B
	 *	@return ����(ns)
	 */
	DWORD GetResolutionNanoCount(void) const
	{
		return static_cast<DWORD>(1000 * 1000 * 1000 / m_freq.QuadPart);
	}

	/**
	 * [�m�F] �o�ߊm�F.
	 *		�R���X�g���N�^�A���邢�� Reset() ����̎���(��s)�Ɣ�r���܂��B
	 *	@param dwTime ����(��s)
	 *	@retval true dwTime �ȏ�o�߂����B
	 *	@retval false �o�߂��Ă��Ȃ��B
	 */
	bool IsPassed(DWORD dwTime) const
	{
		return GetPassedCount() >= dwTime;
	}

	/**
	 * [�m�F] �o�ߊm�F�����Z�b�g.
	 *		�R���X�g���N�^�A���邢�� Reset() ����̎���(��s)�Ɣ�r���܂��B
	 *		�o�߂��Ă����烊�Z�b�g�����܂��B
	 *	@param dwTime ����(��s)
	 *	@retval true dwTime �ȏ�o�߂����B
	 *	@retval false �o�߂��Ă��Ȃ��B
	 */
	bool IsPassedAndReset(DWORD dwTime)
	{
		bool r = IsPassed(dwTime);
		if ( r ){ Reset(); }
		return r;
	}

	/**
	 * [�o��] �o�ߎ��ԕ\��.
	 *		�J���p�ł��B�R���X�g���N�^�A���邢�� Reset() ����̎���(ms)���o�͂��܂��B
	 *	@note �����[�X�r���h���A�������܂���B
	 */
	void OutputDebugPassedCount(void) const
	{
		TRACE1( " passed count = %.6f(ms)\n",  static_cast<double>(GetPassedNanoCount() / 1000000.0) );
	}

private:
	LARGE_INTEGER	m_count;		///< �J�E���^
	LARGE_INTEGER	m_freq;			///< ����x
	LONGLONG		m_apiCount;		///< QueryPerformanceCounter() �̎��s�J�E���^
	bool			m_isValid;		///< �L���H
};



};//TNB
