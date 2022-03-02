#pragma once
/**
 *@file
 * �X���b�h�Ǘ��֌W�̃w�b�_
 *
 *		�e�ՂɃX���b�h�̏������쐬���邱�Ƃ��ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbExchanger.h"
#include "TnbStrLib.h"
#ifndef _WIN32_WCE
 #include <process.h>
#endif



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#if ! defined(_WIN32_WCE)
  #if defined(__AFX_H__)
	// MFC
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		CWinThread* P = ::AfxBeginThread(static_cast<AFX_THREADPROC>(pAddr), pParam, 0, 0, CREATE_SUSPENDED, NULL);
		if ( P != NULL )
		{
			*pThreadId = P->m_nThreadID;
			return P->m_hThread;
		}
		return NULL;
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::AfxEndThread(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
	}
  #elif defined(_MT)
	// C�����^�C���g�p�o�[�W����
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		UINT uId = 0;
		typedef unsigned(__stdcall* P_FUNC)(LPVOID);
		INT_PTR dw = ::_beginthreadex(NULL, 0 , static_cast<P_FUNC>(pAddr), pParam, CREATE_SUSPENDED, &uId);
		*pThreadId = uId;
		return reinterpret_cast<HANDLE>(dw);
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::_endthreadex(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
		::CloseHandle(h);
	}
  #else
 	// API �g�p�o�[�W����
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		return ::CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(pAddr), pParam, CREATE_SUSPENDED, pThreadId);
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::ExitThread(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
		::CloseHandle(h);
	}
  #endif
#else
	// API �g�p�o�[�W����
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		return ::CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(pAddr), pParam, CREATE_SUSPENDED, pThreadId);
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::ExitThread(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
		::CloseHandle(h);
	}
#endif

#ifdef _DEBUG
  // �f�o�O�\���p
  #ifdef __MSVCPP6__
	#define TNB_THREADNAMEFRM1	"�گ�� 0x%X"
	#define TNB_THREADNAMEFRM2	"�گ�� 0x%X [%s]"
	#define TNB_THREADTRACETYPE	"�گ�� 0x%X [%s] %s�B\n"
  #else
	#define TNB_THREADNAMEFRM1	"�X���b�h 'Win32 �X���b�h' (0x%x)"
	#define TNB_THREADNAMEFRM2	"�X���b�h 'Win32 �X���b�h' (0x%x) [%s]"
	#define TNB_THREADTRACETYPE	"�X���b�h 'Win32 �X���b�h' (0x%x) [%s] %s�B\n"
  #endif
#endif

#endif //_TnbDOXYGEN



/**@ingroup THREAD
 * �X���b�h��ԊǗ��N���X
 *
 *		�X���b�h�̏�Ԃ��Ǘ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbThread.h
 *
 *	@date 09/09/10	CThread ���番��
 *	@date 10/01/14	�v���C�I���e�B�̐ݒ���@�𖾋L�B
 *	@date 10/03/19	SetOwnerHwnd() �ǉ��B
 */
class CThreadStatus
{
public:

	/// �R���X�g���N�^.
	CThreadStatus(void) : m_hThread(NULL), m_threadId(NULL)
	{
	}

	/// �f�X�g���N�^
	virtual ~CThreadStatus(void)
	{
	}

	/**
	 * [�擾] �X���b�h�̃n���h���擾
	 *	@retval �n���h��
	 */
	operator HANDLE(void) const
	{
		return m_hThread;
	}

	/**
	 * [�擾] �X���b�h�̃n���h���擾
	 *	@retval �n���h��
	 */
	HANDLE GetHandle(void) const
	{
		return m_hThread;
	}

	/**
	 * [�擾] �X���b�h��ID�擾
	 *	@return ID
	 */
	DWORD GetId(void) const
	{
		if ( ! IsAlive() )
		{
			return 0;
		}
		return m_threadId;
	}

	/**
	 * [�m�F] �X���b�h�������Ă��邩
	 *	@retval true	�����Ă���
	 *	@retval false	���Ȃ�
	 */
	bool IsAlive(void) const
	{
		return m_hThread != NULL;
	}

	/**
	 * [�ݒ�] �֘A�E�B���h�E�n���h���ݒ�.
	 *		�X���b�h�I�����Ƀ|���v����E�B���h�E�n���h�����w�肵�܂��B
	 *	@see CThread::SetOwnerHwnd()
	 *	@param hWnd �E�B���h�E�n���h��
	 */
	void SetOwnerHwnd(HWND hWnd)
	{
		m_hOwnerWnd = hWnd;
	}

	/**
	 * [�擾] �D��x�擾
	 *	@note �ݒ肵�Ă��Ȃ��ꍇ�A�D��x�� THREAD_PRIORITY_NORMAL �ł��B
	 *	@note Start���Ă��Ȃ��ꍇ�A�l�� THREAD_PRIORITY_NORMAL �ł��B
	 *	@retval THREAD_PRIORITY_TIME_CRITICAL	���A���^�C��
	 *	@retval THREAD_PRIORITY_HIGHEST			��
	 *	@retval THREAD_PRIORITY_ABOVE_NORMAL	�ʏ�ȏ�
	 *	@retval THREAD_PRIORITY_NORMAL			�ʏ�
	 *	@retval THREAD_PRIORITY_BELOW_NORMAL	�ʏ�ȉ�
	 *	@retval THREAD_PRIORITY_LOWEST			��
	 *	@retval THREAD_PRIORITY_ABOVE_IDLE		�A�C�h���ȏ�
	 *	@retval THREAD_PRIORITY_IDLE			�A�C�h��
	 */
	int GetPriority(void) const
	{
		return IsAlive() ? ::GetThreadPriority(m_hThread) : THREAD_PRIORITY_NORMAL;
	}

	/**
	 * [�ݒ�] �D��x�ݒ�
	 *	@param priority �D��x�B GetPriority() �̖߂�l���Q�ƁB
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetPriority(int priority = THREAD_PRIORITY_NORMAL)
	{
		if ( IsAlive() )
		{
			if ( ::SetThreadPriority(m_hThread, priority) )
			{
				return true;
			}
			_GetLastError("SetThreadPriority");
		}
		return false;
	}

	/**
	 * [���s] PostThread���b�Z�[�W
	 *	@param message ���b�Z�[�W�B
	 *	@param wParam WPARAM�B
	 *	@param lParam LPARAM�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		bool r = !! ::PostThreadMessage(GetId(), message, wParam, lParam);
		if ( ! r )
		{
			_GetLastError("PostThreadMessage");
		}
		return r;
	}

#ifndef _WIN32_WCE
	/**
	 * [�ݒ�] �v���Z�b�T�A�t�B�j�e�B�}�X�N�ݒ�.
	 *	@param affinityMask �A�t�B�j�e�B�}�X�N�B�e�r�b�g�͂��̃X���b�h�����s�ł���v���Z�b�T��\���܂��B
	 *	@retval 0 �G���[�B
	 *	@retval 0�ȊO �ȑO�̃A�t�B�j�e�B�}�X�N�B
	 */
	DWORD_PTR SetAffinityMask(DWORD_PTR affinityMask)
	{
		DWORD_PTR r = ::SetThreadAffinityMask(m_hThread, affinityMask);
		if ( r == 0 )
		{
			_GetLastError("SetThreadAffinityMask");
		}
		return r;
	}
#endif

protected:
	HANDLE				m_hThread;			///< �X���b�h�̃n���h��
	DWORD				m_threadId;			///< �X���b�h��ID
	HWND				m_hOwnerWnd;		///< �I�[�i�[�E�B���h�E
};



/**@ingroup THREAD
 * �X���b�h�Ǘ��N���X
 *
 *		IRunner ����Ǘ����A�X���b�h�œ������܂��B
 *
 *	@par�g�p��
 *		\code
 *
 *		class CMyThread : public CThread::IRunner
 *		{
 *		public:
 *			void Start(void)
 *			{
 *				m_thread.SetRunner(this);
 *				m_thread.Start();
 *			}
 *		protected:
 *			virtual DWORD Run(void)
 *			{
 *				while ( IsRunnable() )
 *				{
 *					;				//�X���b�h�ł̏���
 *				}
 *				return 0;
 *			};
 *			CThread m_thread;
 *		};
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbThread.h
 *
 *	@date 06/04/01 �V�K�쐬
 *	@date 06/04/21 Thread�N���҂��� CCyclicBarrier ���g���悤�ɕύX
 *	@date 06/08/03 �X���b�h�쐬�� _beginthread ���g�p����悤�ɂ����B
 *	@date 06/09/08 Stop���A���ɃX���b�h���Ȃ��ꍇ���l���B
 *	@date 06/09/19 ������ Exchanger ���g���悤�ɂ����B
 *	@date 09/09/10 CThreadStatus �����𕪗��B Resume(), Suspend() �ǉ��B
 *	@date 10/01/12 Terminate() �ǉ��B
 *	@date 19/06/10 Terminate() �C���B�n���h����NULL�ɂ��Y��Ă����B
 */
class CThread : public CThreadStatus, CCopyImpossible
{
	DEFSUPER(CThreadStatus);
public:

	/**@ingroup THREAD
	 * �X���b�h���s�Ǘ������i�[�C���^�[�t�F�[�X
	 *
	 *		�{�N���X���p���������̂� CThread �N���X�ɈϏ����邱�ƂŃX���b�h��e�ՂɁA
	 *		�����ł��܂��B�p�������N���X�̓X���b�h�œ������������� Run() �Ɏ������܂��B\n
	 *		Stop�v��������ƁA IsRunnable() �̖߂�l��false�ɂȂ�܂��B
	 *		Run() ���ł� IsRunnable() ���|�[�����O�� �߂�l�� false �ɂȂ������A Run() �������I�����A���^�[������K�v���L��܂��B
	 *
	 *	@see CThread::CThread()
	 *	@see CThread::SetRunner()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbThread.h
	 *
	 *	@date 06/04/01	�V�K�쐬
	 *	@date 06/07/26	PostMessage() �ǉ��B
	 *	@date 08/10/09	OnStartThread() �ǉ��B
	 *	@date 09/09/11	�T�X�y���h�v���ǉ��B OnSetupThread() �ǉ��B OnStartThread() �p�~�B
	 *	@date 10/03/19	OnExitThread() �ǉ��B
	 */
	struct IRunner
	{
		/** �f�X�g���N�^ */
		virtual ~IRunner(void) {}

		/** �R���X�g���N�^ */
		IRunner(void) : m_canRunnable(true), m_isOnDemandSuspend(false)
		{
		}

		/**
		 * [�m�F] ���s�\���ۂ�
		 *	@retval true	���s�\�B
		 *	@retval false	���s��~�v���BRun�֐��͒�����Return���邱�ƁB
		 */
		bool IsRunnable(void) const
		{
			if ( m_isOnDemandSuspend )
			{
				m_isOnDemandSuspend = false;
				::SuspendThread(::GetCurrentThread());
			}
			return m_canRunnable;
		}

	protected:

		/** 
		 * [����] �X���b�h�����{��
		 *	@note ���̊֐��ɃX���b�h�̏������L�q���܂��B
		 *		���[�v�� IsRunnable() ���R�[�����A�U�Ȃ�֐������^�[������悤�ɁA
		 *		����K�v���L��܂��B
		 *	@return �߂�l�B CThread::GetThreadResult() �œ�����l�ł��B
		 */
		virtual DWORD Run(void) = 0;

		/**
		 * [�ʒm] �X���b�h�J�n�O�ʒm.
		 *		CThread::Start() ���s���A���^�[������O�ɂ��̃��\�b�h���V�K�X���b�h��ŃR�[������܂��B
		 */
		virtual void OnSetupThread(void) {}

		/**
		 * [�ʒm] �X���b�h�I���ʒm.
		 *		�X���b�h���I�����A�R�[������܂��B
		 */
		virtual void OnExitThread(void) {}

	private:
		volatile bool		m_canRunnable;		///< ���s�\���ۂ�
		mutable bool		m_isOnDemandSuspend;///< �T�X�y���h�v��
		CExchangerT<BYTE>	m_excThreadReady;	///< �X���b�h�J�n�̓����p
		friend class CThread;					///< �t�����h�N���X�錾
	};


	//---------------------------


	/// �R���X�g���N�^
	CThread(void) : m_pRunner(NULL), m_lastResult(0)
	{
	}

	/** 
	 * �f�X�g���N�^
	 *	@note �X���b�h�������Ă���ꍇ�A��~�����܂��B
	 */
	virtual ~CThread(void)
	{
		Stop();
	}

	/**
	 * [�ݒ�] �����i�[�A�ݒ�
	 *	@note �X���b�h�������Ă���ꍇ�A�G���[�ɂȂ�܂��B
	 *	@param pRunner �����i�[
	 *	@retval true	����
	 *	@retval false	���s(�X���b�h�������Ă���)
	 */
	bool SetRunner(IRunner* pRunner)
	{
		if ( IsAlive() )
		{
			return false;
		}
		m_pRunner = pRunner;
		return true;
	}

	/**
	 * [�ݒ�] �֘A�E�B���h�E�n���h���ݒ�.
	 *		�X���b�h�I�����Ƀ|���v����E�B���h�E�n���h�����w�肵�܂��B
	 *	@note �X���b�h���ŁA SendMessage() ���g������������ꍇ�A��~���f�b�g���b�N����\��������܂��B
	 *		����́A�I���҂����A�|���v����~���邽�߂ɔ������܂��B
	 *		����āA�X���b�h���ŁA SendMessage() ���g������������ꍇ�A�{���\�b�h�Ń��C���E�B���h�E��
	 *		�n���h�����w�肵�Ă����K�v������܂��B
	 *	@param hOwner �E�B���h�E�n���h��
	 */
	void SetOwnerHwnd(HWND hOwner)
	{
		_super::SetOwnerHwnd(hOwner);
	}

	/**
	 * [�擾] �X���b�h�I���l
	 *	@note �X���b�h�����x�����s���Ă��Ȃ��ꍇ�A�l�͂O�ł��B
	 *	@return �I���l
	 */
	DWORD GetThreadResult(void) const
	{
		return m_lastResult;
	}

	/**
	 * [�ݒ�] �X���b�h���f
	 *	@attention �ʏ�A�X���b�h���ɏI��������@�\��p�ӂ��ׂ��ł��B
	 *	@param result �X���b�h�I���l
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Terminate(DWORD result = 0)
	{
		BOOL r = ::TerminateThread(m_hThread, result);
		_GetLastError("TerminateThread");
		if ( r )
		{
			m_hThread = NULL;
		}
		return !! r;
	}

	/**
	 * [�ݒ�] �X���b�h��~�v��.
	 *		�X���b�h�ɑ΂��Ē�~�v�����܂��B
	 *	@note �X���b�h���I������̂�҂��܂���B
	 *	@attention ���p���ׂ��ł͂���܂���B�g�p����ꍇ�A{@link IRunner::OnExitThread() �I���ʒm} �ŏI�����m�F����悤�ɂ��Ă��������B
	 */
	void StopRequest(void)
	{
		if ( IsAlive() && m_pRunner->m_canRunnable )
		{
			#ifdef _DEBUG
				if ( ! m_threadName.IsEmpty() )
				{
					TRACE3( TNB_THREADTRACETYPE, m_threadId, m_threadName.Ref(), _T("��~�v��") );
				}
			#endif
			//��~�v��
			::ResumeThread(m_hThread);
			SetPriority(THREAD_PRIORITY_NORMAL);
			m_pRunner->m_canRunnable = false;
		}
	}

	/**
	 * [�ݒ�] �X���b�h��~
	 *		�X���b�h�ɑ΂��Ē�~�v�����܂��B
	 *	@note �X���b�h���I�����邩�^�C���A�E�g����܂ŁA�҂��܂��B
	 *	@param dwWait �X���b�h�I���v�����Ă���҂��ԁB
	 *			�O�Ȃ�i�v�ɑ҂��A�ȗ������15�b�҂��܂��B
	 *			����ȏソ���Ă��I���Ȃ���Terminate���܂��B
	 *	@retval true �����i�X���b�h���J�n���Ă��Ȃ�����Terminate�������������Ƃ��Ă��܂��j
	 *	@retval false ���s�i���X�i�[�X���b�h���� Stop() ���g�p����Ǝ��s���܂��j
	 */
	bool Stop(DWORD dwWait = 15000)
	{
		if ( m_threadId == ::GetCurrentThreadId() )
		{
			ASSERT0( false, "CThread::Stop()", "���X�i�[�X���b�h�������� Stop() �͎g�p�ł��܂���B" );
			return false;
		}
		bool boRc = IsAlive();
		if ( boRc && m_hThread != NULL )
		{
			DWORD dwExitCode;
			if ( ! ::GetExitCodeThread(m_hThread, &dwExitCode) )
			{
				boRc = false;
			}
			else if ( dwExitCode != STILL_ACTIVE )
			{
				#ifdef _DEBUG
					if ( ! m_threadName.IsEmpty() )
					{
						TRACE3( TNB_THREADTRACETYPE, m_threadId, m_threadName.Ref(), _T("���ɒ�~...") );
					}
				#endif
				boRc = false;
			}
		}
		if ( boRc )
		{
			#ifdef _DEBUG
				if ( ! m_threadName.IsEmpty() )
				{
					TRACE3( TNB_THREADTRACETYPE, m_threadId, m_threadName.Ref(), _T("��~�v��") );
				}
				#if ( _WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400 )
				if ( ::IsDebuggerPresent() )
				{
					dwWait = 0;
				}
				#endif
			#endif
			//��~�v��
			::ResumeThread(m_hThread);
			SetPriority(THREAD_PRIORITY_NORMAL);
			m_pRunner->m_canRunnable = false;
			//
			if ( dwWait == 0 )
			{
				dwWait = DWORD_MAX;
			}
			else
			{
				dwWait /= 50;
			}
			//�X���b�h�I���҂�
			for ( DWORD i = 0; i < dwWait; i++ )
			{
				m_pRunner->m_canRunnable = false;
				if ( m_hThread == NULL )
				{
					break;
				}
				DWORD dwExitCode;
				if ( ::GetExitCodeThread(m_hThread, &dwExitCode) )
				{
					if ( dwExitCode != STILL_ACTIVE )
					{
						break;
					}
				}
				if ( m_hOwnerWnd != NULL )
				{
					PumpMessage(m_hOwnerWnd);
				}
				::Sleep(50);
			}
			if ( m_hThread != NULL )
			{
				//�I���Ȃ�������Term....
				if ( ! ::TerminateThread(m_hThread, 0) )
				{
					_GetLastError("TerminateThread");
				}
				TRACE0("Thread��Terminate���܂���\n");
				#ifdef _DEBUG
					CWorkMemT<TCHAR> str(1024);
					if ( ! m_threadName.IsEmpty() )
					{
						STRLIB::PrintF(str, 1024, _T("�w�莞�ԓ��ɁA") _T(TNB_THREADNAMEFRM2) _T("���I�����܂���ł����B\n          �����I�������܂����B")
							,m_threadId, m_threadName.Ref());
					}
					else
					{
						STRLIB::PrintF(str, 1024, _T("�w�莞�ԓ��ɁA") _T(TNB_THREADNAMEFRM1) _T("���I�����܂���ł����B\n          �����I�������܂����B")
							,m_threadId);
					}
					ASSERT1( false, "CThread::Stop()", "%s", str.Ref() );
				#endif
			}
		}
		if ( m_hThread != NULL )
		{
			il_CloseThreadHandle(m_hThread);
			m_hThread = NULL;
		}
		return true;
	}

	/**
	 * [�ݒ�] �X���b�h�J�n
	 *	@param lpszName �X���b�h�̖��O�iDEBUG�ȊO�ł͖�������܂��j
	 *	@retval true ����
	 *	@retval false ���s(�����i�[�����o�^/�X���b�h�N�����s/���ɃX���b�h���s��)
	 */
	bool Start(LPCTSTR lpszName = NULL)
	{
		if ( m_pRunner == NULL || IsAlive() )
		{
			return false;
		}
		#ifdef _DEBUG
			m_threadName = lpszName;
		#endif
		//
		m_hThread = il_CreateThread(ms_ThreadMain, this, &m_threadId);
		if ( m_hThread == NULL )
		{
			// �X���b�h�N�����s
			TRACE0( "�X���b�h�N�����s");
			return false;
		}
		DWORD r = ::ResumeThread(m_hThread);
		ASSERT( ToInt(r) != -1 );
		IgnoreUnusedValue(r);
		m_pRunner->m_excThreadReady.Exchange(0/*�_�~�[*/);
		return true;
	}

	/**
	 * [���s] �T�X�y���h.
	 *	@note ���̃T�X�y���h�́A IRunneble::IsRunnable() �̒��ŁASuspend ���܂��B
	 *		���̂��߃T�X�y���h�̃^�C�~���O���R���g���[�����邱�Ƃ��o���܂��B
	 */
	void Suspend(void)
	{
		if ( m_pRunner != NULL )
		{
			m_pRunner->m_isOnDemandSuspend = true;
		}
	}

	/**
	 * [���s] ���W���[��.
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool Resume(void)
	{
		int r = ToInt(::ResumeThread(m_hThread));
		while ( r > 0 )
		{
			r = ToInt(::ResumeThread(m_hThread));
		}
		return r == 0;
	}

	/**
	 * [����] ���b�Z�[�W�|���v.
	 *	@note ���͌n�̃��b�Z�[�W�͔j�����܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 */
	static void PumpMessage(HWND hWnd)
	{
		MSG msg;
		while ( ::PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE) )
		{
			if ( ! ::GetMessage(&msg, hWnd, 0, 0) )
			{
				break;
			}
			bool r = (WM_KEYFIRST <= msg.message && msg.message <= WM_IME_KEYLAST);
			r |= (WM_MOUSEFIRST <= msg.message && msg.message <= WM_MOUSELAST);
			if ( r )
			{
				TRACE1("Drop Message = %d\n", msg.message);
			}
			else
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

private:
	IRunner*			m_pRunner;			///< �����i�[
	DWORD				m_lastResult;		///< �X���b�h�I���l
	CSimpleStr			m_threadName;		///< �X���b�h��(Debug�p)

	/** 
	 * �X���b�h�J�n�p
	 *		�����i�[��Run���Ăяo���܂��B
	 *	@param lpPoint this�|�C���^
	 *	@retval 0�A�Œ�
	 */
	static DWORD WINAPI ms_ThreadMain(LPVOID lpPoint)
	{
		CThread* P = static_cast<CThread*>(lpPoint);
		ASSERTLIB(P->m_threadId == ::GetCurrentThreadId());
		P->m_pRunner->m_canRunnable = true;
		P->m_pRunner->OnSetupThread();
		P->m_pRunner->m_excThreadReady.Exchange(0/*�_�~�[*/);
		#ifdef _DEBUG
			if ( ! P->m_threadName.IsEmpty() )
			{
				TRACE3( TNB_THREADTRACETYPE, P->m_threadId, P->m_threadName.Ref(), _T("�J�n"));
			}
		#endif
		P->m_lastResult = P->m_pRunner->Run();
		#ifdef _DEBUG
			if ( ! P->m_threadName.IsEmpty() )
			{
				TRACE3( TNB_THREADTRACETYPE, P->m_threadId, P->m_threadName.Ref(), _T("�I��"));
			}
		#endif
		P->m_pRunner->OnExitThread();
		il_CloseThreadHandle(P->m_hThread);
		P->m_hThread = NULL;
		P->m_threadId = 0;
		il_ExitThread(P->m_lastResult);
		return P->m_lastResult;
	}
};



/**@ingroup THREAD
 * �ȈՃX���b�h���s�N���X
 *
 *		IRunner ��n�����ƂŁA�X���b�h���N�����܂��B
 *		IRunner ���I�������^�C�~���O�ŁA���������J�����邱�Ƃ��o���܂��B
 *
 *	@note �{�N���X�̓C���X�^���X�͍�炸�A Start() �N���X���\�b�h���g���܂��B
 *
 *	@attention	�X���b�h�̏�Ԃ�Ď������܂���B
 *				�v���Z�X���I������O�ɃX���b�h���I����Ă���悤�ɂ��Ă��������B
 *
 *	@par�g�p��
 *		\code
 *			//�錾
 *			struct IFooRunner : public CThreadlet::IRunner
 *			{
 *				virtual void Run(LPARAM lParam)
 *				{
 *					::MessageBox(NULL, _T("ABC"), _T("XYZ"), MB_OK);		
 *				};
 *			};
 *			
 *			//���s
 *			CThreadlet::Start(12, new IFooRunner);
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbThread.h
 *
 *	@date 06/05/01 �V�K�쐬
 *	@date 06/08/03 �X���b�h�쐬�� _beginthread ���g�p����悤�ɂ����B
 *	@date 06/09/19 ������ Exchanger ���g���悤�ɂ����B
 *	@date 09/12/08 Start �̈����A�ǉ��B
 *	@date 10/01/14 �v���C�I���e�B�̐ݒ���@�𖾋L�B
 */
class CThreadlet
{
public:

	/**@ingroup THREAD
	 * �ȈՃX���b�h���s�N���X�̃����i�[
	 *
	 *	@see CThreadlet
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbThread.h
	 *
	 *	@date 06/05/01 �V�K�쐬
	 */
	struct IRunner
	{
		/// �f�X�g���N�^
		virtual ~IRunner(void) {}
		/**
		 * [�ʒm] �X���b�h������
		 *	@note �����ɃX���b�h�̏������������܂��B
		 *	@param lParam Start �̒l���n����܂��B
		 */
		virtual void Run(LPARAM lParam) = 0;
	};

	/**
	 * [�ݒ�] �X���b�h�J�n
	 *	@param lParam LPARAM �B
	 *	@param piRunner �����i�[�BwithDelete �� true �Ȃ�K�� new �������̂�n���܂��B
	 *	@param withDelete true �Ȃ�X���b�h�I������ piRunner �� delete ���܂��B
	 *	@param priority �D��x�B CThreadStatus::GetPriority() �̖߂�l���Q�ƁB
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool Start(LPARAM lParam, IRunner* piRunner, bool withDelete = true, int priority = THREAD_PRIORITY_NORMAL)
	{
		CThreadlet* P = new CThreadlet;
		P->m_pRunner = piRunner;
		P->m_lParam = lParam;
		P->m_withDelete = withDelete;
		//
		DWORD dwThreadId;
		HANDLE hThread = il_CreateThread(ms_ThreadMain, P, &dwThreadId);
		if ( hThread == NULL )
		{
			// �X���b�h�N�����s
			TRACE0( "�X���b�h�N�����s");
			if ( P->m_withDelete )
			{
				delete P->m_pRunner;
			}
			delete P;
			return false;
		}
		::SetThreadPriority(hThread, priority);
		DWORD r = ::ResumeThread(hThread);
		ASSERT(ToInt(r) != -1);
		IgnoreUnusedValue(r);
		il_CloseThreadHandle(hThread);
		P->m_excThreadReady.Exchange(0/*�_�~�[*/);
		return true;
	}

private:

	CExchangerT<BYTE>	m_excThreadReady;	///< �X���b�h�J�n�̓����p
	IRunner*			m_pRunner;			///< �����i�[
	bool				m_withDelete;		///< �I�����I�u�W�F�N�g��Delete���邩�H
	LPARAM				m_lParam;			///< LPARAM
	/** 
	 * [����] �X���b�h�J�n�p
	 *		�����i�[��Run���Ăяo���܂��B
	 *	@param lpPoint this�|�C���^
	 *	@retval 0�A�Œ�
	 */
	static DWORD WINAPI ms_ThreadMain(LPVOID lpPoint)
	{
		CThreadlet* P = static_cast<CThreadlet*>(lpPoint);
		P->m_excThreadReady.Exchange(0/*�_�~�[*/);
		P->m_pRunner->Run(P->m_lParam);
		::Sleep(200);
		if ( P->m_withDelete )
		{
			delete P->m_pRunner;
		}
		delete P;
		il_ExitThread(0);
		return 0;
	}

	/// �R���X�g���N�^
	CThreadlet(void) {}
};



/**@ingroup THREAD
 * �ꎞ�I�X���b�h�D��x�N���X
 *
 *		���悾���X���b�h�̗D��x���グ��(�ς�����)���Ɏg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbThread.h
 *
 *	@date 10/09/06 �V�K�쐬
 */
class CTemporarilyThreadPriority
{
public:
	
	/**
	 * �R���X�g���N�^.
	 *		���݂̃X���b�h�̗D��x�������܂��B
	 *	@param priority �D��x�B CThreadStatus::GetPriority() �̖߂�l���Q�ƁB
	 *			�ȗ�����ƁA THREAD_PRIORITY_TIME_CRITICAL �ɂȂ�܂��B
	 */
	CTemporarilyThreadPriority(int priority = THREAD_PRIORITY_TIME_CRITICAL)
		: m_hThread(::GetCurrentThread())
	{
		m_Init(priority);
	}

	/**
	 * �R���X�g���N�^.
	 *		�w��̃X���b�h�̗D��x�������܂��B
	 *	@param hThread �Ώۂ̃X���b�h.
	 *	@param priority �D��x�B CThreadStatus::GetPriority() �̖߂�l���Q�ƁB
	 *			�ȗ�����ƁA THREAD_PRIORITY_TIME_CRITICAL �ɂȂ�܂��B
	 */
	CTemporarilyThreadPriority(HANDLE hThread, int priority = THREAD_PRIORITY_TIME_CRITICAL) 
		: m_hThread(hThread)
	{
		m_Init(priority);
	}
	
	/**
	 * �f�X�g���N�^.
	 *	@note Restore() ���Ă��Ȃ��ꍇ�A Restore() ���R�[�����܂��B
	 */
	~CTemporarilyThreadPriority(void)
	{
		Restore();
	}

	/**
	 * [����] ���X�g�A.
	 *		�X���b�h�̗D��x�����ɖ߂��܂��B
	 */
	void Restore(void)
	{
		if ( m_hThread != NULL )
		{
			::SetThreadPriority(m_hThread, m_priority);
			m_hThread = NULL;
		}
	}

private:
	void m_Init(int p)
	{
		m_priority = ::GetThreadPriority(m_hThread);
		::SetThreadPriority(m_hThread, p);
		ASSERT( ::GetThreadPriority(m_hThread) == p );
	}
	HANDLE	m_hThread;		///< �Ώۂ̃X���b�h
	int		m_priority;		///< ���̗D��x
};



}; // TNB



