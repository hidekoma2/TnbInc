#pragma once
/**
 *@file
 * �o�ߎ��ԊǗ��֌W�̃w�b�_
 *
 *	@note	�}�N�� "_TnbTIME_Winmm_DISABLE" ����`����Ă���ƁA GetTickCount() ���g���܂�
 *			�i���x�͗����܂����A�������y�������\��������܂��j�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



#ifdef _WIN32_WCE
 #define _TnbTIME_Winmm_DISABLE
#endif

#ifndef _TnbTIME_Winmm_DISABLE
 #include <mmsystem.h>
 #pragma comment(lib, "winmm.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * �o�ߎ��ԊǗ��N���X
 *
 *		���Ԃ̌o�߂������N���X�ł��B
 *
 *	@note "_TnbTIME_Winmm_DISABLE"����`����Ă��Ȃ��ꍇ�AAPI�� timeGetTime() ���g�p���܂��B
 *	@note "_TnbTIME_Winmm_DISABLE"����`����Ă���ꍇ�AAPI�� GetTickCount() ���g�p���܂��B
 *
 *	@note	��(�Q�l) "_TnbTIME_Winmm_DISABLE"����`����Ă���ꍇ�̐��x<BR>
 *				Windows NT 3.5�ȍ~�F��P�O�~���b���ƂɃJ�E���g	<BR>
 *				Windows NT 3.1�F��P�U�~���b���ƂɃJ�E���g		<BR>
 *				Windows 95�n�F��T�T�~���b���ƂɃJ�E���g		<BR>
 *
 *	@par�K�v�t�@�C��
 *			TnbTickCount.h
 *
 *	@date 06/06/29	�V�K�쐬
 *	@date 06/09/20	timeGetTime() ���g�p����悤�ɕύX�B
 *	@date 09/02/24	�l�t���R���X�g���N�^��p�ӁB
 *	@date 09/10/28	OutputDebugPassedCount() �ǉ��B
 */
class CTickCount
{
public:

	/**
	 * �R���X�g���N�^
	 *	@note ���݂�tick���L�����܂��B
	 */
	CTickCount(void) : m_dwTick(m_Get())
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note ���݂�tick�����21���O(��ԉ�����tick)���L�����܂��B
	 *	@param b �_�~�[
	 */
	explicit CTickCount(bool b) : m_dwTick(m_Get())
	{
		m_dwTick -= 0x80000000;
	}

	/**
	 * [�ݒ�] ���Z�b�g.
	 *		���݂̎��Ԃ��L�����܂��B
	 */
	void Reset(void)
	{
		m_dwTick = m_Get();
	}

	/**
	 * [�擾] Reset���̎��Ԏ擾.
	 *		�R���X�g���N�^�A���邢�� Reset() �������́A����(OS�N�����Ă���̎���)��Ԃ��܂��B
	 *	@return ����(ms)
	 */
	DWORD GetResetedCount(void) const
	{
		return m_dwTick;
	}

	/**
	 * [�擾] �o�ߎ��Ԏ擾.
	 *		�R���X�g���N�^�A���邢�� Reset() ����̎���(ms)��Ԃ��܂��B
	 *	@return �o�ߎ���(ms)
	 */
	DWORD GetPassedCount(void) const
	{
		return m_Get() - m_dwTick;
	}

	/**
	 * [�m�F] �o�ߊm�F.
	 *		�R���X�g���N�^�A���邢�� Reset() ����̎���(ms)�Ɣ�r���܂��B
	 *	@param dwTime ����(ms)
	 *	@retval true dwTime �ȏ�o�߂����B
	 *	@retval false �o�߂��Ă��Ȃ��B
	 */
	bool IsPassed(DWORD dwTime) const
	{
		return GetPassedCount() >= dwTime;
	}

	/**
	 * [�m�F] �o�ߊm�F�����Z�b�g.
	 *		�R���X�g���N�^�A���邢�� Reset() ����̎���(ms)�Ɣ�r���܂��B
	 *		�o�߂��Ă����烊�Z�b�g�����܂��B
	 *	@param dwTime ����(ms)
	 *	@param isNow ���Z�b�g�̕��@�B true �Ȃ獡�̎��ԂɃ��Z�b�g���܂��B false �Ȃ�w�莞�Ԍo�߂��������������ɂ��܂��B
	 *	@retval true dwTime �ȏ�o�߂����B
	 *	@retval false �o�߂��Ă��Ȃ��B
	 */
	bool IsPassedAndReset(DWORD dwTime, bool isNow = true)
	{
		bool r = IsPassed(dwTime);
		if ( r )
		{
			if ( isNow )
			{
				Reset();
			}
			else
			{
				m_dwTick += dwTime;
				if ( IsPassed(dwTime) )
				{
					Reset();
				}
			}
		}
		return r;
	}

	/**
	 * [�o��] �o�ߎ��ԕ\��.
	 *		�J���p�ł��B�R���X�g���N�^�A���邢�� Reset() ����̎���(ms)���o�͂��܂��B
	 *	@note �����[�X�r���h���A�������܂���B
	 */
	void OutputDebugPassedCount(void) const
	{
		TRACE1( " passed count = %d(ms)\n", GetPassedCount() );
	}

private:
	DWORD m_dwTick;		///< TickCount�L���p

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	  #ifndef _TnbTIME_Winmm_DISABLE
		/// timePeriod�̎����ݒ�p�N���X
		class CAutoPeriod
		{
		public:
			CAutoPeriod(void)
			{
				VERIFY(timeBeginPeriod(1) == TIMERR_NOERROR);
			}
			~CAutoPeriod(void)
			{
				VERIFY(timeEndPeriod(1) == TIMERR_NOERROR);		
			}
		};
		/// ���݂̃V�X�e���N�����Ԏ擾
		DWORD m_Get(void) const
		{
			static CAutoPeriod s_autoPeriod;
			return ::timeGetTime();
		}
	  #else
		/// ���݂̃V�X�e���N�����Ԏ擾
		DWORD m_Get(void) const
		{
			return ::GetTickCount();
		}
	  #endif
	#endif // _TnbDOXYGEN
};



};//TNB
