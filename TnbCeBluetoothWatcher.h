#pragma once
/**
 *@file
 * CE �p Bluetooth �Ď��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothWatcher.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCeMsgQueueWatcher.h"
#include <bt_api.h>



//TNB Library
namespace TNB{



/**@ingroup CEBLUETOOTH THREAD
 * Bluetooth(CE) �Ď�.
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetoothWatcher.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothWatcher
{
public:

	/**@ingroup CEBLUETOOTH THREAD
	 * Bluetooth(CE) �Ď��̃��X�i�[.
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbCeBluetoothWatcher.h
	 * 
	 *	@date 09/10/01	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �C�x���g�ʒm.
		 *		�C�x���g������������ʒm����܂��B
		 *	@note �C�x���g ID�ƃC�x���g���^�̊֌W�́A�ȉ��̒ʂ�ł��B
		 *		<table>
		 *		<tr><td>�C�x���g ID(�l)</td>		<td>�C�x���g���^</td>			<td>���e</td>
		 *		</tr>
		 *		<tr><td>BTE_CONNECTION(100)</td>	<td>BT_CONNECT_EVENT</td>		<td>�ڑ�</td>
		 *		</tr>
		 *		<tr><td>BTE_DISCONNECTION(101)</td>	<td>BT_DISCONNECT_EVENT</td>	<td>�ؒf</td>
		 *		</tr>
		 *		<tr><td>BTE_ROLE_SWITCH(102)</td>	<td>BT_ROLE_SWITCH_EVENT</td>	<td>ROLE �X�C�b�`</td>
		 *		</tr>
		 *		<tr><td>BTE_MODE_CHANGE(103)</td>	<td>BT_MODE_CHANGE_EVENT</td>	<td>���[�h�`�F���W</td>
		 *		</tr>
		 *		<tr><td>BTE_PAGE_TIMEOUT(104)</td>	<td>(�Ȃ�)</td>					<td>�y�[�W�^�C���A�E�g</td>
		 *		</tr>
		 *		<tr><td>BTE_KEY_NOTIFY(200)</td>	<td>BT_LINK_KEY_EVENT</td>		<td>�L�[�ʒm</td>
		 *		</tr>
		 *		<tr><td>BTE_KEY_REVOKED(201)</td>	<td>BT_LINK_KEY_EVENT</td>		<td>�L�[�ʒm</td>
		 *		</tr>
		 *		<tr><td>BTE_LOCAL_NAME(300)</td>	<td>(�Ȃ�)</td>					<td>���O�ύX</td>
		 *		</tr>
		 *		<tr><td>BTE_COD(301)</td>			<td>(�Ȃ�)</td>					<td>COD�ύX</td>
		 *		</tr>
		 *		<tr><td>BTE_STACK_UP(400)</td>		<td>(�Ȃ�)</td>					<td>�X�^�b�N�A�b�v</td>
		 *		</tr>
		 *		<tr><td>BTE_STACK_DOWN(4001)</td>	<td>(�Ȃ�)</td>					<td>�X�^�b�N�_�E��</td>
		 *		</tr>
		 *		<tr><td>BTE_AVDTP_STATE(500)</td>	<td>BT_AVDTP_STATE_CHANGE</td>	<td>AVDTP��ԕω�</td>
		 *		</tr>
		 *		</table>
		 *	@par �T���v��
		 *	\code
		 *		void CFoo::OnBluetoothEvent(DWORD eventId, LPCVOID lpEventData)
		 *		{
		 *			if ( eventId == BTE_CONNECTION )
		 *			{
		 *				const BT_CONNECT_EVENT* P = static_cast<const BT_CONNECT_EVENT*>(lpEventData);
		 *				TRACE0("Connection Event\n");
		 *				TRACE1("-- handle=%d\n", P->hConnection);
		 *				TRACE2("-- bta=%04x%08x\n", GET_NAP(P->bta), GET_SAP(P->bta));
		 *				TRACE1("-- Link Type=%s\n", P->ucLinkType ? _T("ACL") : _T("SCO"));
		 *				TRACE1("-- Encryption Mode=%d\n", P->ucEncryptMode);
		 *			}
		 *		}
		 *	\endcode
		 *	@param eventId �C�x���g ID.
		 *	@param lpEventData �C�x���g���.
		 */
		virtual void OnBluetoothEvent(DWORD eventId, LPCVOID lpEventData) = 0;
	};


	//------------------------------------


	/// �R���X�g���N�^
	CBluetoothWatcher(void) : m_handle(NULL)
	{
	}

	/// �f�X�g���N�^
	~CBluetoothWatcher(void)
	{
		Stop();
	}

	/**
	 * [�o�^] ���X�i�[�o�^.
	 *	@param P ���X�i�[�B
	 */
	void SetListener(IListener* P)
	{
		m_inner.m_pListener = P ;
	}

	/**
	 * [�m�F] �X�^�[�g�m�F.
	 *	@retval true �X�^�[�g�ς�.
	 *	@retval false ���X�^�[�g�B
	 */
	bool IsStarted(void) const
	{
		return m_handle != NULL;
	}

	/**
	 * [�ݒ�] �Ď��X�^�[�g.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Start(void)
	{
		Stop();
		m_queue.SetListener(&m_inner);
		if ( m_queue.Start(10, true, _T("CBluetoothWatcher")) )
		{
			DWORD f = BTE_CLASS_CONNECTIONS | BTE_CLASS_PAIRING | BTE_CLASS_DEVICE | BTE_CLASS_STACK | BTE_CLASS_AVDTP;
			m_handle = ::RequestBluetoothNotifications(f, m_queue.GetSafeHandle());
			if ( m_handle != NULL )
			{
				return true;
			}
		}
		Stop();
		return false;
	}

	/**
	 * [�ݒ�] �Ď��X�g�b�v.
	 *	@note �X�^�[�g���Ă��Ȃ��ꍇ�A�������܂���B
	 */
	void Stop(void)
	{
		if ( m_handle != NULL )
		{
			::StopBluetoothNotifications(m_handle);
			m_handle = NULL;
		}
		m_queue.Stop();
	}

private:
	/// �����N���X
	class CInner : public CMsgQueueWatcherT<BTEVENT>::IListener
	{
	public:
		CInner(void) : m_pListener(NULL)
		{
		}
		virtual void OnMsgQueueWatcherEvent(const BTEVENT& ev, size_t length, bool isAlert)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnBluetoothEvent(ev.dwEventId, ev.baEventData);
			}
		}
		CBluetoothWatcher::IListener* m_pListener;
	};
	CInner						m_inner;	///< �����N���X
	CMsgQueueWatcherT<BTEVENT>	m_queue;	///< �L���[
	HANDLE						m_handle;	///< �n���h��
};



}; //TNB


