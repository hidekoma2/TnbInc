#pragma once
/**
 *@file
 * CE �p�d���Ď��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCePowerWatcher.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCeMsgQueueWatcher.h"
#include <pm.h>



//TNB Library
namespace TNB{



/**@ingroup WINCE THREAD
 * �d���Ď� (CE��p).
 *
 *	@par�K�v�t�@�C��
 *			TnbCePowerWatcher.h
 * 
 *	@date 09/12/01	�V�K�쐬
 *	@date 10/03/30	Open/Close -> Start/Stop �ɕύX�B
 */
class CPowerWatcher
{
public:

	/**@ingroup WINCE THREAD
	 * �d���Ď��̃��X�i�[�C���^�[�t�F�[�X (CE��p).
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbCePowerWatcher.h
	 * 
	 *	@date 09/12/01	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �d����ԕω��ʒm.
		 *	@note �d����Ԃɕω�������������ʒm����܂��B
		 *	@param flag �t���O
		 *	@param lpszState ��ԕ�����
		 */
		virtual void OnPowerTransitionEvent(DWORD flag, LPCTSTR lpszState) {}

		/**
		 * [�ʒm] ���W���[���ʒm.
		 *	@note ���W���[��������ʒm����܂��B
		 */
		virtual void OnPowerResumeEvent(void) {}

		/**
		 * [�ʒm] DC/AC�؂�ւ��ʒm.
		 *	@note DC ���� AC ���A�ʒm����܂��B
		 */
		virtual void OnPowerStatusChange(void) {}

		/**
		 * [�ʒm] �d����ԕω��ʒm.
		 *	@note �d����Ԃ��ω�������ʒm����܂��B
		 *	@param info ���.
		 */
		virtual void OnPowerInfoChange(const POWER_BROADCAST_POWER_INFO& info) {}
	};


	//---------------------------


	/// �R���X�g���N�^
	CPowerWatcher(void) : m_handle(NULL)
	{
		m_inner.pListener = NULL;
	}

	/// �f�X�g���N�^
	~CPowerWatcher(void)
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
	 * [�ݒ�] ���X�i�[�ݒ�.
	 *	@param P ���X�i�[.
	 */
	void SetListener(IListener* P)
	{
		m_inner.pListener = P;
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
	 *	@param flag �Ď��t���O�B�ȗ�����ƁA�S��Ԃ��Ď����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B �ڍׂ� ::GetLastError() �œ����܂��B
	 */
	bool Start(DWORD flag = POWER_NOTIFY_ALL)
	{
		Stop();
		m_msgQueue.SetListener(&m_inner);
		if ( m_msgQueue.Start(3, true, _T("PowerWatch")) )
		{
			m_handle = ::RequestPowerNotifications(m_msgQueue, flag);
		}
		if ( m_handle != NULL )
		{
			return true;
		}
		DWORD r = ::GetLastError();
		Stop();
		::SetLastError(r);
		return false;
	}

	/**
	 * [�ݒ�] �Ď���~.
	 *	@note �Ď��J�n���Ă��Ȃ��ꍇ�A�������܂���B
	 */
	void Stop(void)
	{
		if ( m_handle != NULL )
		{
			::StopPowerNotifications(m_handle);
			m_handle = NULL;
		}
		m_msgQueue.Stop();
	}

private:
	/// POWER_BROADCAST �\���� ��
	struct TMyPowerBroadcast
	{
		DWORD	Message;	///< ���b�Z�[�W
		DWORD	Flags;		///< �t���O
		DWORD	Length;		///< �d����Ԓ���
		union
		{
			POWER_BROADCAST_POWER_INFO	powerInfo;	///< �d�����
			TCHAR						state[260];	///< �d�����
		};
	};
	/// �C���i�[�N���X
	class CInner : public CMsgQueueWatcherT<TMyPowerBroadcast>::IListener
	{
	public:
		/**
		 * [�ʒm] ��M�ʒm.
		 *	@note ���b�Z�[�W����M������ʒm����܂��B
		 *	@param data ���b�Z�[�W�f�[�^.
		 *	@param isAlert true �Ȃ�A���[�ƃ��b�Z�[�W.
		 */
		virtual void OnMsgQueueWatcherEvent(const TMyPowerBroadcast& data, size_t length, bool isAlert)
		{
			if ( pListener != NULL )
			{
				switch ( data.Message )
				{
				case PBT_POWERINFOCHANGE:
					ASSERT( data.Length == sizeof(POWER_BROADCAST_POWER_INFO) );
					pListener->OnPowerInfoChange(data.powerInfo);
					break;
				case PBT_POWERSTATUSCHANGE:
					pListener->OnPowerStatusChange();
					break;
				case PBT_RESUME:
					pListener->OnPowerResumeEvent();
					break;
				case PBT_TRANSITION:
					pListener->OnPowerTransitionEvent(data.Flags, data.state);
					break;
				}
			}
		}
		CPowerWatcher::IListener* pListener;	///< ���X�i�[
	};
	CInner									m_inner;		///< �C���i�[
	HANDLE									m_handle;		///< �n���h��
	CMsgQueueWatcherT<TMyPowerBroadcast>	m_msgQueue;		///< ���b�Z�[�W�L���[
};



}; // TNB
