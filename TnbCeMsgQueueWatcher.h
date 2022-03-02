#pragma once
/**
 *@file
 * CE �p���b�Z�[�W�L���[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeMsgQueueWatcher.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbThread.h"
#include <Msgqueue.h>



//TNB Library
namespace TNB
{



/**@ingroup WINCE THREAD
 * ���b�Z�[�W�L���[�Ď� (CE��p).
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 * 
 *	@par�K�v�t�@�C��
 *			TnbCeMsgQueueWatcher.h
 *
 *	@date 09/10/01	�V�K�쐬
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 *	@date 10/03/30	Open/Close -> Start/Stop �ɕύX�B
 */
template<typename TYP>
class CMsgQueueWatcherT : CThread::IRunner
{
public:

	/**@ingroup WINCE THREAD
	 * ���b�Z�[�W�L���[�Ď��̃��X�i�[�C���^�[�t�F�[�X (CE��p).
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbCeMsgQueueWatcher.h
	 * 
	 *	@date 09/10/01	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] ��M�ʒm.
		 *	@note ���b�Z�[�W����M������ʒm����܂��B
		 *	@param data ���b�Z�[�W�f�[�^.
		 *	@param length ��M�f�[�^����.
		 *	@param isAlert true �Ȃ�A���[�ƃ��b�Z�[�W.
		 */
		virtual void OnMsgQueueWatcherEvent(const TYP& data, size_t length, bool isAlert) = 0;
	};


	//---------------------------


	/// �R���X�g���N�^
	CMsgQueueWatcherT(void) : m_handle(NULL), m_pListener(NULL)
	{
	}

	/// �f�X�g���N�^
	~CMsgQueueWatcherT(void)
	{
		Stop();
	}

	/**
	 * [�擾] �n���h���擾.
	 *	@return �n���h��.
	 */
	HANDLE GetSafeHandle(void) const
	{
		return m_handle;
	}

	/**
	 * [�擾] �n���h���擾.
	 *	@return �n���h��.
	 */
	operator HANDLE(void) const
	{
		return m_handle;
	}

	/**
	 * [�ݒ�] ���X�i�[�ݒ�.
	 *	@param P ���X�i�[.
	 */
	void SetListener(IListener* P)
	{
		m_pListener = P;
	}

	/**
	 * [�m�F] �Ď��m�F.
	 *	@retval true �Ď���.
	 *	@retval false ���X�^�[�g.
	 */
	bool IsStarted(void) const
	{
		return m_handle != NULL;
	}

	/**
	 * [�ݒ�] �Ď��J�n.
	 *	@param maxMessagesCount �L���[�̑傫��.
	 *	@param isRead true �Ȃ�ǂݍ��݁A false �Ȃ珑�����݂ŃI�[�v�����܂��B
	 *	@param lpszThreadName �X���b�h���B�f�o�b�O�p�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B �ڍׂ� ::GetLastError() �œ����܂��B
	 */
	bool Start(size_t maxMessagesCount = 0, bool isRead = true, LPCTSTR lpszThreadName = NULL)
	{
		MSGQUEUEOPTIONS opt = { 0 };
		opt.dwSize = sizeof(MSGQUEUEOPTIONS);
		opt.dwFlags = 0;
		opt.dwMaxMessages = maxMessagesCount;
		if ( maxMessagesCount == 0 )
		{
			opt.dwFlags = MSGQUEUE_NOPRECOMMIT;
		}
		opt.cbMaxMessage = sizeof(TYP);
		opt.bReadAccess = isRead;
		return m_Start(NULL, opt, lpszThreadName);
	}

	/**
	 * [�ݒ�] �Ď��J�n.
	 *	@param lpszQueueName �L���[��.
	 *	@param option �I�v�V����.
	 *	@param lpszThreadName �X���b�h���B�f�o�b�O�p�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B �ڍׂ� ::GetLastError() �œ����܂��B
	 */
	bool Start(LPCTSTR lpszQueueName, const MSGQUEUEOPTIONS& option, LPCTSTR lpszThreadName = NULL)
	{
		MSGQUEUEOPTIONS opt = option;
		return m_Start(lpszQueueName, opt,lpszThreadName);
	}

	/**
	 * [�ݒ�] �Ď���~.
	 *	@note �J�n���Ă��Ȃ��ꍇ�A�������܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B �ڍׂ� ::GetLastError() �œ����܂��B
	 */
	bool Stop(void)
	{
		bool r = true;
		if ( m_handle != NULL )
		{
			r = !! ::CloseMsgQueue(m_handle);
			m_handle = NULL;
		}
		m_thread.Stop();
		return r;
	}

	/**
	 * [�ݒ�] ���M.
	 *	@param data ���M�f�[�^.
	 *	@param timeout �^�C���A�E�g(ms)�B
	 *	@param isAlert true �Ȃ�A���[�g�A false �Ȃ�ʏ푗�M�B
	 *	@retval true �����B
	 *	@retval false ���s�B �ڍׂ� ::GetLastError() �œ����܂��B
	 */
	bool Send(const TYP& data, DWORD timeout = INFINITE, bool isAlert = false)
	{
		DWORD fg = isAlert ? MSGQUEUE_MSGALERT : 0;
		TYP t = data;
		return !! ::WriteMsgQueue(m_handle, &t, sizeof(data), timeout, fg);
	}

	/**
	 * [�擾] �L���[���.
	 *	@param[out] _info ��񂪊i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B �ڍׂ� ::GetLastError() �œ����܂��B
	 */
	bool GetInfo(MSGQUEUEINFO& _info) const
	{
		_info.dwSize = sizeof(MSGQUEUEINFO);
		return !! ::GetMsgQueueInfo(m_handle, &_info);
	}

	/**
	 * [�Q��] �Ď��X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

protected:
	
	/** 
	 * [����] �X���b�h�����{��
	 *	@note ���̊֐��ɃX���b�h�̏������L�q���܂��B
	 *		���[�v�� IsRunnable() ���R�[�����A�U�Ȃ�֐������^�[������悤�ɁA
	 *		����K�v���L��܂��B
	 *	@return �߂�l�B CThread::GetThreadResult() �œ�����l�ł��B
	 */
	virtual DWORD Run(void)
	{
		while ( IsRunnable() && IsStarted() )
		{
//			DWORD dw = ::MsgWaitForMultipleObjects(1, &m_handle, FALSE, INFINITE, QS_ALLEVENTS);
			DWORD dw = ::WaitForSingleObject(m_handle, INFINITE);
			if ( dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED_0 )
			{
				TYP buf;
				DWORD dwRead = 0;
				DWORD dwFlags = 0;
				while ( ::ReadMsgQueue(m_handle, &buf, sizeof(TYP), &dwRead, 0, &dwFlags) )
				{
					if ( m_pListener != NULL )
					{
						m_pListener->OnMsgQueueWatcherEvent(buf, dwRead, dwFlags == MSGQUEUE_MSGALERT);
					}
				}
				DWORD err = ::GetLastError();
				if ( err != ERROR_TIMEOUT && err != ERROR_INVALID_HANDLE )
				{
					if ( ERROR_INSUFFICIENT_BUFFER == _GetLastError("ReadMsgQueue") )
					{
						ASSERT( false );
					}
				}
			}
		}
		return 0;
	}

private:
	bool m_Start(LPCTSTR lpszQueueName, MSGQUEUEOPTIONS& _opt, LPCTSTR lpszThreadName)
	{
		Stop();
		_opt.dwSize = sizeof(MSGQUEUEOPTIONS);
		m_handle = ::CreateMsgQueue(lpszQueueName, &_opt);
		if ( m_handle != NULL )
		{
			m_thread.SetRunner(this);
			if ( lpszThreadName == NULL )
			{
				lpszThreadName = _T("CMsgQueueWatcher");
			}
			if ( m_thread.Start(lpszThreadName) )
			{
				return true;
			}
		}
		Stop();
		return false;
	}

	HANDLE		m_handle;			///< �n���h��
	IListener*	m_pListener;		///< ���X�i�[
	CThread		m_thread;			///< �X���b�h
};



}; // TNB