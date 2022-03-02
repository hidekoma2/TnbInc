#pragma once
/**
 *@file
 * ���������֌W�̃w�b�_
 *
 *		WaitableTimer���g������������������N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"



#if ( _WIN32_WINNT >= 0x0400 ) || ( _WIN32_WINDOWS > 0x0400 )
#else
 #error Reason "This class cannot be used by the target OS."
#endif



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup SYNC
 * WaitableTimer�r���Ǘ��N���X
 *
 *		WaitableTimer���g�����������������܂��B
 *
 *	@note �������� ISynchronized ���������Ă���r���N���X�Ƃ͎g�p���@���قȂ�܂��B
 *
 *	@note	����I�ɃA�����b�N�𔭐�������ꍇ�A�^�C�~���O(�������e)�ɂ���ẮA
 *			��ȏ�̃X���b�h�������ɓ������Ƃ�����܂��i�w�莞�Ԃ��ƂɁA�A�����b�N�ɂȂ邽�߁j�B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		static CSyncWaitableTimer s_timer;
 *		DWORD WINAPI DoThread(DWORD ThreadCount)
 *		{
 *			for ( int i = 0; i < 5; i++) 
 *			{
 *			  EXCLUSIVE(&s_timer);
 *			  TRACE1("�X���b�h%d: �����J�n\n", ThreadCount);
 *			  Sleep(400*ThreadCount);
 *			  TRACE1("�X���b�h%d: �����I��\n", ThreadCount);
 *			}
 *			return 0;
 *		}
 *		void Main()
 *		{
 *			HANDLE hThread[2];
 *			DWORD ThreadID[2];
 *			for ( int i = 0; i < 2; i++ ) 
 *			{
 *			  hThread[i] = CreateThread(
 *			        NULL, 0, (LPTHREAD_START_ROUTINE)DoThread,
 *			        (LPVOID)i, 0, &ThreadID[i]);
 *			}
 *			TRACE0("�J�n\n");
 *			s_timer.Start(1000,1000);
 *			WaitForMultipleObjects(2, hThread, TRUE, INFINITE);// �X���b�h�̏I���܂őҋ@		
 *			CloseHandle(hThread[0]);
 *			CloseHandle(hThread[1]);
 *		}
 *	</pre></td></tr></table>
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createwaitabletimer.asp
 *
 *	@par�K�v�t�@�C�� 
 *			TnbSyncWaitableTimer.h
 *
 *	@date 06/09/21	�V�K�쐬
 *	@date 06/11/15 �쐬���s������ �X���[����悤�ɕύX�B
 */
class CSyncWaitableTimer : public CAbstractSyncHandle, CCopyImpossible
{
	HANDLE	m_hTimer;		///< �r���n���h��

public:

	/**
	 * �R���X�g���N�^
	 *	@note �쐬���́A���b�N��Ԃł��B
	 *	@param boIsManualReset false���ȗ��Ȃ玩�����Z�b�g�I�u�W�F�N�g�A
							true�Ȃ�蓮���Z�b�g�I�u�W�F�N�g�ɂȂ�܂��B
	 *	@param lpszName Event�̖��O�B�ȗ��\�B
	 *	@throw CInvalidParamException ���O�s���ȂǁA�����I�u�W�F�N�g���쐬�ł��Ȃ��������A�X���[����܂��B
	 */
	explicit CSyncWaitableTimer(bool boIsManualReset = false, LPCTSTR lpszName = NULL)
	{
		m_hTimer = ::CreateWaitableTimer(NULL, boIsManualReset, lpszName);
		ASSERT0(
			m_hTimer != NULL,
			"CSyncWaitableTimer::CSyncWaitableTimer()",
			"WaitableTimer���������ł��܂��񂵂��B���O�ɖ�肪����\�����L��܂��B"
		);
		if ( m_hTimer == NULL )
		{
			throw CInvalidParamException();
		}
		::CancelWaitableTimer(m_hTimer);
	}

	/**
	 * �R���X�g���N�^
	 *	@param hTimer CreateWaitableTimer()�ō쐬�����n���h��
	 */
	explicit CSyncWaitableTimer(HANDLE hTimer)
	{
		VERIFY( m_hTimer = hTimer );
	}

	/** �f�X�g���N�^ */
	virtual ~CSyncWaitableTimer(void)
	{
		if ( m_hTimer != NULL )
		{
			::CloseHandle(m_hTimer);
		}
	}

	/**
	 * [�擾] �n���h���擾
	 *	@return �n���h��
	 */
	virtual HANDLE GetHandle(void) const
	{
		return m_hTimer;
	}

	/**
	 * [�r��] �A�����b�N.
	 *	@note	�{�֐��̓_�~�[�ł��B
	 *	@note	Start�Ŏw�肵���^�C�~���O�ŃA�����b�N��Ԃ���u�������邽�߁A�蓮�ł̓A�����b�N�ł��܂���B
	 */
	virtual void Unlock(void) const
	{
		// �Ȃɂ����Ȃ�
	}

	/**
	 * [�r��] �J�n
	 *	@param dwLagTime �J�n�܂ł̎���(ms)
	 *	@param dwInterval �J�n��A����I�ɃA�����b�N�ɂȂ�Ԋu(ms)�B<BR>
	 *					�O���w�肷��ƁA�ŏ����A�����b�N��A�����ƃ��b�N��ԂɂȂ�܂��B
	 *					������x Start ������K�v������܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Start(DWORD dwLagTime, DWORD dwInterval)
	{
		if ( m_hTimer != NULL )
		{
			LARGE_INTEGER li;
			li.QuadPart = dwLagTime;
			li.QuadPart *= -10000;
			return !! ::SetWaitableTimer(m_hTimer, &li, dwInterval, NULL, 0, FALSE);
		}
		return false;
	}

	/**
	 * [�r��] ���b�N��Ԃɂ���.
	 *	@note ���b�N��ԂɌŒ肳��܂��B
	 */
	void ToLock(void)
	{
		if ( m_hTimer != NULL )
		{
			::CancelWaitableTimer(m_hTimer);
		}
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")


#if 0
static CSyncWaitableTimer s_timer;

DWORD WINAPI DoThread(DWORD ThreadCount)
{
	for (int i=0;i<5;i++) 
	{
		EXCLUSIVE(&s_timer);
		TRACE1("�X���b�h%d: �����J�n\n", ThreadCount);
		Sleep(400*ThreadCount);
		TRACE1("�X���b�h%d: �����I��\n", ThreadCount);
	}
	return 0;
}

void ____wt() 
{
	HANDLE hThread[2];
	DWORD ThreadID[2];
	/* �X���b�h��2�쐬 */
	for (int i=0; i<2; i++) 
	{
		hThread[i] = CreateThread(
					NULL, 0, (LPTHREAD_START_ROUTINE)DoThread,
					(LPVOID)i,0,&ThreadID[i]);
	}
	TRACE0("�J�n\n");
	s_timer.Start(1000,1000);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);// �X���b�h�̏I���܂őҋ@
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	typedef LONG (WINAPI *P_InterlockedCompareExchange)(LONG*,LONG,LONG);
	typedef HANDLE (WINAPI *P_IcmpCreateFile)(VOID);
	HANDLE h = CDynamicFuncT<P_IcmpCreateFile>("ICMP.dll","IcmpCreateFile")()();

	CDynamicFuncT<P_InterlockedCompareExchange> func1("kernel32.dll","InterlockedCompareExchange");
	LONG l1 = 0;
	LONG l2 = func1()(&l1,1,0);
}
#endif
