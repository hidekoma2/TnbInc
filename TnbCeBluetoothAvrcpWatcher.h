#pragma once
/**
 *@file
 * CE �p Bluetooth�֌W�̃w�b�_
 *
 *	@note Winsock2 ���g���̂ŁA�}�N�� "_TnbWINSOCK2_ENABLE" ��錾���Ă����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothAvrcpWatcher.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCeMsgQueueWatcher.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

// bt_avrcp.h ���
#ifndef __BT_AVRCP_H__

typedef struct _AvrcpMsg {
    uint    OpCode;
    uint    OpId;
    uint    Reserved[4];
} AvrcpMsg;

enum AVCOpCodes {
    UnitInfo_t = 0x30,
    SubunitInfo_t,
    PassThru_t = 0x7C
};

enum PassThruOpId {
    Select_t = 0x00,
    Up_t,
    Down_t,
    Left_t,
    Right_t,
    RightUp_t,
    RightDown_t,
    LeftUp_t,
    LeftDown_t,
    RootMenu_t,
    SetupMenu_t,
    ContentsMenu_t,
    FavMenu_t,
    Exit_t,

    ChannelUp_t = 0x30,
    ChannelDown_t,
    PrevChannel_t,
    SoundSel_t,
    InputSel_t,
    DisplayInfo_t,
    Help_t,
    PageUp_t,
    PageDown_t,
    
    Power_t = 0x40,
    VolUp_t,
    VolDown_t,
    Mute_t,
    Play_t,
    Stop_t,
    Pause_t,
    Record_t,
    Rewind_t,
    FastFwd_t,
    Eject_t,
    Forward_t,
    Backward_t
};
#endif // __BT_AVRCP_H__

#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) AVRCP �Ď�
 *
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetoothAvrcpWatcher.h
 * 
 *	@date 10/03/30	�V�K�쐬
 */
class CBluetoothAvrcpWatcher
{
public:

	/**@ingroup CEBLUETOOTH THREAD
	 * Bluetooth(CE) AVRCP �Ď��̃��X�i�[.
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbCeBluetoothAvrcpWatcher.h
	 * 
	 *	@date 10/03/30	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �C�x���g�ʒm.
		 *		�C�x���g������������ʒm����܂��B
		 *	@param code �R�[�h.
		 *	@param id ID.
		 */
		virtual void OnBluetoothAvrcpEvent(DWORD code, DWORD id) = 0;
	};


	//--------------------------------------------


	/// �R���X�g���N�^
	CBluetoothAvrcpWatcher(void) 
	{
		m_inner.pListener = NULL;
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
		return m_inner.msgQueue.IsStarted();
	}

	/**
	 * [�ݒ�] �Ď��X�^�[�g.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Start(void)
	{
		LPCTSTR lpszAvrcpQueueName = _T("Avrcp10MsgQ");
		Stop();
		m_inner.msgQueue.SetListener(&m_inner);
		MSGQUEUEOPTIONS opt = { 0 };
		opt.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		opt.dwMaxMessages = 20;
		opt.cbMaxMessage = 100;
		opt.bReadAccess = TRUE;
		if ( m_inner.msgQueue.Start(lpszAvrcpQueueName, opt, _T("AVRCP Watch")) )
		{
			return true;
		}
		DWORD r = ::GetLastError();
		Stop();
		::SetLastError(r);
		return false;
	}

	/**
	 * [�ݒ�] �Ď��X�g�b�v.
	 *	@note �X�^�[�g���Ă��Ȃ��ꍇ�A�������܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B �ڍׂ� ::GetLastError() �œ����܂��B
	 */
	bool Stop(void)
	{
		return m_inner.msgQueue.Stop();
	}

	/**
	 * [�Q��] �Ď��X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_inner.msgQueue.ReferThreadStatus();
	}

	/**
	 * [�ϊ�] ������.
	 *	@param code �R�[�h.
	 *	@param id ID
	 *	@return ������.
	 */
	static CStr ToString(DWORD code, DWORD id)
	{
		CStr s = _T("unknown");
		if ( code == PassThru_t )
		{
			#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
			#define _x_(SS) case SS##_t: s = _T(#SS); break;
			switch ( id & 0x7F )
			{
			_x_(Select);
			_x_(Up);		_x_(Down);		_x_(Left);		_x_(Right);
			_x_(RightUp);	_x_(RightDown);	_x_(LeftUp);	_x_(LeftDown);
			_x_(RootMenu);	_x_(SetupMenu);
			_x_(ContentsMenu);
			_x_(FavMenu);
			_x_(Exit);
			_x_(ChannelUp);	_x_(ChannelDown);
			_x_(PrevChannel);
			_x_(SoundSel);	_x_(InputSel);
			_x_(DisplayInfo);
			_x_(Help);
			_x_(PageUp);	_x_(PageDown);
			_x_(Power);
			_x_(VolUp);		_x_(VolDown);	_x_(Mute);
			_x_(Play);		_x_(Stop);		_x_(Pause);		_x_(Record);
			_x_(Rewind);	_x_(FastFwd);
			_x_(Eject);
			_x_(Forward);	_x_(Backward);
			default:
				break;
			}
			#undef _x_
			#endif _TnbDOXYGEN
			s += ((id & 0x80) == 0) ? _T(" Down") : _T(" Up");
		}
		return s;
	}

private:
	/// �C���i�[�N���X
	class CInner : public CMsgQueueWatcherT<AvrcpMsg>::IListener
	{
	public:
		/**
		 * [�ʒm] ��M�ʒm.
		 *	@note ���b�Z�[�W����M������ʒm����܂��B
		 *	@param data ���b�Z�[�W�f�[�^.
		 *	@param isAlert true �Ȃ�A���[�ƃ��b�Z�[�W.
		 */
		virtual void OnMsgQueueWatcherEvent(const AvrcpMsg& data, size_t length, bool isAlert)
		{
			if ( pListener != NULL )
			{
				pListener->OnBluetoothAvrcpEvent(data.OpCode, data.OpId);
			}
//			AvrcpMsg a = { 0 };
//			a.OpCode = 
//			a.OpId = Stop_t;
//			msgQueue.Send(a);
		}
		CBluetoothAvrcpWatcher::IListener* pListener;	///< ���X�i�[
		CMsgQueueWatcherT<AvrcpMsg>		msgQueue;		///< �L���[
	};
	CInner							m_inner;		///< �C���i�[
};



}; //TNB



#if 0


#define AVRCP_MSG_SZ   TEXT("Avrcp10MsgQ")


int AvrcpMsgProcess (void* p)
{
	static int avrcp_tick;
	bool_t PlayState = 0;
	bool_t bAvrcp = 0;
	fraction Percent;
	int Param = PLAYER_PLAY;
	node* Player = Context()->Player;
	MSGQUEUEOPTIONS mqOptions;
	memset (&mqOptions, 0, sizeof(mqOptions));
	mqOptions.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	mqOptions.dwSize = sizeof(mqOptions);
	mqOptions.dwMaxMessages = 2;
	mqOptions.cbMaxMessage = 100;
	mqOptions.bReadAccess = TRUE;
	hMsgQ = CreateMsgQueue(AVRCP_MSG_SZ, &mqOptions);
	if (! hMsgQ) 
	{
		goto exit_bt;
	}
	//_debug(L"MsgQueue created.");
	while (hMsgQ != 0) 
	{
		DWORD dwWait = WaitForSingleObject (hMsgQ, INFINITE);
		NodeRegLoadValue(AVRCP_ID,AVRCP_ENABLE,&bAvrcp,sizeof(int),TYPE_INT);
		if(! bAvrcp)
		{
			goto exit_bt;
		}
		if (WAIT_OBJECT_0 == dwWait) 
		{
			AvrcpMsg btAvrcp;
			DWORD dwFlags = 0;
			DWORD dwBytesRead = 0;
			BOOL fRet = ReadMsgQueue (hMsgQ, &btAvrcp, sizeof(AvrcpMsg), &dwBytesRead, 10, &dwFlags);
			if (fRet) 
			{
				if(hAvrcpConfMode==1)
				{
					avrcp_tick++;
					if(avrcp_tick>1)
					{
						//_debug(L"Avrcp Config Accessed:%x,tick:%d",btAvrcp.OpId,avrcp_tick);
						intConfAvrcpMsg=btAvrcp.OpId;
						avrcp_tick=0;
					}
				}
				else
				{
					//_debug(L"Avrcp Normal Accessed:%x",btAvrcp.OpId);
					//0x44 & 0xC4 = Pause
					//0x46 & 0xC6 = Play
					//0x4B & 0xCB = Next
					//0x4C & 0xCC = Prev
					//Pass_thru = 0x7c
					if(btAvrcp.OpId==GetAvrcpCmd(AVRCP_PLAY))
					{
						Param=PLAYER_PLAY;
						if(Player->Get(Player,Param,&PlayState,sizeof(PlayState))==ERR_NONE)
						{
							PlayState = !PlayState;
						}
						else
						{
							PlayState = 1 - PlayState;
						}
						Player->Set(Player,Param,&PlayState,sizeof(PlayState));
					}
					else if(btAvrcp.OpId==GetAvrcpCmd(AVRCP_PAUSE))
					{
						Param=PLAYER_PLAY;
						if(Player->Get(Player,Param,&PlayState,sizeof(PlayState))==ERR_NONE)
						{
							PlayState = !PlayState;
						}
						else
						{
							PlayState = 1 - PlayState;
						}
						Player->Set(Player,Param,&PlayState,sizeof(PlayState));
					}
					else if(btAvrcp.OpId==GetAvrcpCmd(AVRCP_STOP))
					{
						Param=PLAYER_STOP;
						Player->Set(Player,Param,NULL,1);
						Percent.Num = 0;
						Percent.Den = TRACKMAX;
						Player->Set(Player,PLAYER_PERCENT,&Percent,sizeof(Percent));
					}
					else if(btAvrcp.OpId==GetAvrcpCmd(AVRCP_PREV))
					{
						Param=PLAYER_PREV;
						Player->Set(Player,Param,NULL,0);
					}
					else if(btAvrcp.OpId==GetAvrcpCmd(AVRCP_NEXT))
					{
						Param=PLAYER_NEXT;
						Player->Set(Player,Param,NULL,0);
					}
					else if(btAvrcp.OpId==GetAvrcpCmd(AVRCP_BACKWARD))
					{
						Param=PLAYER_MOVEBACK;
						Player->Set(Player,Param,NULL,0);
					}
					else if(btAvrcp.OpId==GetAvrcpCmd(AVRCP_FORWARD))
					{
						Param=PLAYER_MOVEFFWD;
						Player->Set(Player,Param,NULL,0);
					}
					if(GetAvrcpCmd(AVRCP_OFFSCREEN))
					{
						if(GetAvrcpCmd(AVRCP_LOCK_KEYPAD)) keybd_event(0x85,0,0,0);
						SetSystemPowerState( NULL, POWER_STATE_IDLE, POWER_FORCE);
					}
				}
				//SendMessage(HWND_BROADCAST, WM_USER+1985, btAvrcp.OpId, btAvrcp.OpCode);
			}
		}
	}
exit_bt:
	if (hMsgQ) 
	{
		CloseMsgQueue(hMsgQ);
		hMsgQ=NULL;
	}
	return 0;
}

#endif
