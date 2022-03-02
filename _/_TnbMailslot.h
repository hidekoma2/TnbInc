#pragma once
/**
 *@file
 *@ingroup REPORT
 * Mailslot�̃A�N�Z�X�֌W�̃w�b�_
 *
 *		��M�����́A���X�i�[��o�^���邱�Ƃōs���܂��B
 *
 */



#include "TnbDef.h"
#include "TnbAbstractReport.h"
#include "TnbCommon.h"
#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * Mailslot��M�C�x���g�Ǘ��N���X
 *
 *		�f�[�^�i�����ƃA�h���X�j���C�x���g�̂����ꂩ��ێ�����N���X�ł��B
 *		���X�i�[�Ŏg�p���܂��B
 *
 *	@see CMailslot::IListener::OnMailslotEvent()
 *
 *	@par�K�v�t�@�C��
 *			TnbMailslot.h
 */
typedef TNB::CReportEvent CMailslotEvent;



/**@ingroup REPORT
 * Mailslot�Z���_�[�N���X
 *
 *		���M��p��Mailslot�N���X�ł��B��̃��[�h������܂��i�R���X�g���N�^���Ɏw�肵�܂��j�B	<BR>
 *		��͑��M���Ƃ�Open�����M��Close���郂�[�h�ł��i�����炪Default���[�h�j�B					<BR>
 *		��͎w�肪����܂�Close���Ȃ����[�h�ł��iMaintainOpen���[�h�j�B						<BR>
 *
 *	@note �{�N���X�͎�M�@�\���Ȃ��̂ŁA IReport::IListener ��o�^���Ă��Ӗ����L��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbMailslot.h
 * 
 *	@date 06/05/11 �V�K�쐬
 *	@date 06/08/31 CommViewer �ɑΉ��B
 *	@date 06/09/09 
 *	@date 07/02/09 �e�N���X�ύX�B
 */
class CMailslotSender: public CAbstractReport
{
	typedef CAbstractReport __super;

	CStr	m_strMailslotName;		///< ���[���X���b�g��
	CStr	m_strDomainName;		///< �h���C����
	HANDLE	m_hMailslot;			///< �n���h��
	bool	m_boIsMaintainOpenMode;	///< true�I�[�v�������ςȂ�

	/**
	 * Mailslot���I�[�v��
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD m_Open(void)
	{
		if ( m_strMailslotName.IsEmpty() )
		{
			return ERROR_INVALID_PARAMETER;
		}
		CStr str;
		if ( m_strDomainName.IsEmpty() )
		{
			str.Format(_T("\\\\.\\mailslot\\%s"), m_strMailslotName);
		}
		else
		{
			str.Format(_T("\\\\%s\\mailslot\\%s"), m_strDomainName, m_strMailslotName);
		}
		HANDLE h = ::CreateFile(
						str,			// ���[���X���b�g��
						GENERIC_WRITE,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL
		);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateFile");
		}
		m_hMailslot = h;
		return 0;
	}

	/**
	 * [�m�F] �A�N�Z�X�\���`�F�b�N
	 *	@note Default���[�h�̏ꍇ�Atrue��Ԃ��܂��B
	 *	@retval true	�A�N�Z�X�\�B
	 *	@retval	false	�A�N�Z�X�s�\�B
	 */
	bool m_CanAccess(void) const
	{
		if ( ! m_boIsMaintainOpenMode )
		{
			return true;
		}
		return m_hMailslot != NULL;
	}

	/**
	 * [����] ��~
	 */
	void m_Stop(void)
	{
		if ( m_hMailslot != NULL )
		{
			::CloseHandle(m_hMailslot);
			m_hMailslot = NULL;
		}
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param boIsMaintainOpenMode �ȗ���false�Ȃ�A���M���Ƃ�Open�����M��Close���郂�[�h�i�����炪Default���[�h�j�B
	 *							true�Ȃ�A�w�肪����܂�Close���Ȃ����[�h�iMaintainOpen���[�h�j�B						<BR>
	 */
	explicit CMailslotSender(bool boIsMaintainOpenMode = false)
	{
		m_hMailslot = NULL;
		m_boIsMaintainOpenMode = boIsMaintainOpenMode;
	}

	/**
	 * [����] �I�[�v��.
	 *		�w��̖��O�̃��[���X���b�g���I�[�v�����܂��B
	 *		���ł�Open���Ă������x�N���[�Y���܂��B
	 *		��x�ݒ肷��Ǝ��񂩂�͈����Ȃ���Open���g�p�ł��܂��B
	 *	@note Default���[�h�̏ꍇ�A���O���L�����邾���ŁA��ɐ������܂��B
	 *	@param lpszMailslotName ���[���X���b�g��
	 *	@param lpszDomainName �h���C����/�R���s���[�^��/* ���w��B�ȗ��Ȃ烍�[�J���B
	 *	@see http://msdn.microsoft.com/library/ja/jpipc/html/_win32_createmailslot.asp
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD Open(LPCTSTR lpszMailslotName, LPCTSTR lpszDomainName = NULL)
	{
		m_strMailslotName = lpszMailslotName;
		m_strDomainName = lpszDomainName;
		if ( m_CanAccess() )
		{
			m_Stop();	//�ڑ����Ȃ�Close���܂��B
		}
		return Open();
	}

	/**
	 * [����] �I�[�v��
	 *	@note	���ڈȍ~�A�g�p�ł��܂��B
	 *	@note Default���[�h�̏ꍇ�A����������ɐ������܂��B
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD Open(void)
	{
		if ( ! m_boIsMaintainOpenMode || m_CanAccess() )
		{
			return 0;
		}
		return m_Open();
	}

	/**
	 * [�擾] �R�~���j�P�[�V������.
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const
	{
		return CStr::Fmt(_T("MailslotSender[%s]:"), m_strMailslotName);
	}

	/**
	 * [�擾] �R�~���j�P�[�V����ID.
	 *		TNBVIW_KIND_??? + PORT ��Ԃ��܂��B
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return TNBVIW_KIND_MAILS + (reinterpret_cast<DWORD>(m_hMailslot) & 0xFFFF);
	}

	/**
	 * [�m�F] �J�n�`�F�b�N
	 *	@note Default���[�h�̏ꍇ�A���true��Ԃ��܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const
	{
		return m_CanAccess(); 
	}
	
	/**
	 * [�m�F] �ڑ��`�F�b�N
	 *	@note Default���[�h�̏ꍇ�A���true��Ԃ��܂��B
	 *	@retval true	�ڑ����B
	 *	@retval	false	�ؒf�B
	 */
	virtual bool IsConnect(void) const
	{
		return IsAlive();
	}

	/**
	 * [����] ��~
	 */
	virtual void Stop(void)
	{
		m_Stop();
	}

	/**
	 * [���M] �f�[�^���M
	 *	@note Default���[�h�̏ꍇ�A�I�[�v���A���M�A�N���[�Y�A�Ƃ����������s���܂��B
	 *	@param sizeData ���M����f�[�^���B
	 *	@param lpcvData ���M����f�[�^�B
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	virtual bool Send(size_t sizeData, LPCVOID lpcvData)
	{
		__super::m_OnSend(sizeData, lpcvData);
		if ( ! m_boIsMaintainOpenMode )
		{
			DWORD dw = m_Open();
			if ( dw != 0 )
			{
				return false; 
			}
		}
		else
		{
			Open();
		}
		if ( m_CanAccess() )
		{
			DWORD dwWritten;
			if ( ::WriteFile(m_hMailslot, lpcvData, sizeData, &dwWritten, NULL) )
			{
				if ( dwWritten == sizeData )
				{
					if ( ! m_boIsMaintainOpenMode )
					{
						m_Stop(); 
					}
					return true;
				}
			}
		}
		if ( ! m_boIsMaintainOpenMode )
		{
			m_Stop();
		}
		return false;
	}
};



/**@ingroup REPORT
 * Mailslot�N���X
 *
 *		��M�ݒ�� CMailslot::IListener �� IReport::IListener �̃T�u�N���X��o�^���s���܂��B
 *
 *		���M�����́A CMailslotSender ��default���[�h�Ɠ�������ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMailslot.h
 * 
 *	@date 06/05/11	�V�K�쐬
 *	@date 06/07/07	InterfaceID�T�O�A�ǉ�
 *	@date 06/08/31	CommViewer �ɑΉ��B
 */
class CMailslot : public CAbstractReport, TNB::CThread::IRunner
{
	typedef CAbstractReport __super;
public:

	/**@ingroup REPORT
	 * Mailslot�̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		CMailslot �̒ʒm�p�C���^�[�t�F�[�X�ł��B
	 *
	 *	@see CMailslot::SetListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMailslot.h
	 * 
	 *	@date 06/05/11	�V�K�쐬
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �f�[�^����M���������ȂǁA�R�[��
		 *	@note	�{�N���X�̃C�x���g�́u{@link TNB::ECE_Error ECE_Error}�v�݂̂ł��B
		 *	@param ev ��M�������e�i�f�[�^���C�x���g�j�B
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnMailslotEvent(const CMailslotEvent& ev) = 0;
	};

private:

	CStr				m_strMailslotName;		///< ���[���X���b�g��
	IListener*			m_piMailslotListner;	///< ���X�i�[
	CSyncSection		m_syncReceive;			///< ���V�[�u�̔r��
	CThread				m_threadRecv;			///< ��M�X���b�h�Ǘ�
	HANDLE				m_hMailslot;			///< Slot�p�n���h��
	CMailslotSender		m_cMailslotSender;		///

	/**
	 * �X���b�h�����֐�
	 *	@note	��M����ƁA�R�[���o�b�N�֐����R�[�����܂��B
	 *			��M���s����ƁA�n���h�����N���[�Y���A
	 *			�R�[���o�b�N�֐��֒ʒm���A�X���b�h���I�����܂��B
	 *	@retval �O�i�Œ�)�B
	 */
	virtual DWORD Run(void)
	{
		while( IsRunnable() )
		{
			::Sleep(1);
			//��M���r��ON
			m_syncReceive.Lock();
			// ���݂̏�Ԃ��擾�B
			DWORD dwSize;
			if ( ! ::GetMailslotInfo(m_hMailslot, NULL, &dwSize, NULL, NULL) )
			{
				DWORD dwErr = _GetLastError("GetMailslotInfo");
				//��M���r��OFF
				m_syncReceive.Unlock();
				//API�G���[���A�ʒm�B
				if ( ! m_OnMailslotEvent(CMailslotEvent(this, ECE_Error, dwErr)) )
				{
					break;
				}
				continue;
			}
			if ( ! CanAccess() )
			{
				m_syncReceive.Unlock();
				break;	//Close���ꂽ��A�I��
			}
			// ���ݎ�M���Ă���Data���擾
			if ( dwSize != MAILSLOT_NO_MESSAGE )
			{
				// 1Byte�ȏ゠��B
//				CPointerHandleT<BYTE,PDT_Array> hpBuff = new BYTE[dwSize];
				CWorkMem buf(dwSize);
				DWORD dwReadBytes = 0;
				// Mailslot����̓ǂݍ���
//				BOOL boRc = ::ReadFile(m_hMailslot,hpBuff,dwSize,&dwReadBytes,NULL);
				BOOL boRc = ::ReadFile(m_hMailslot, buff, dwSize, &dwReadBytes, NULL);
				m_syncReceive.Unlock();		//��M���r��OFF
				if ( ! boRc || dwReadBytes == 0 )
				{
					//���s
					DWORD dwErr = _GetLastError("ReadFile");
					if ( ! m_OnMailslotEvent(CMailslotEvent(this, ECE_Error, dwErr)) )
					{
						break;
					}
					continue;
				}
				//�R�[���o�b�N�֐��֒ʒm�B
				CMailslotEvent ev(this);
//				ev.SetData(dwReadBytes, hpBuff);
				ev.SetData(dwReadBytes, buff);
				if ( ! m_OnMailslotEvent(ev) )
				{
					break;
				}
			}
			else 
			{
				//---��M�f�[�^�Ȃ�
				//��M���r��OFF
				m_syncReceive.Unlock();
				::Sleep(200);
			}
		}
		//�R�[���o�b�N�֐��֒ʒm�B
		m_OnMailslotEvent(CMailslotEvent(this, ECE_End));
		m_syncReceive.Lock();
		if ( CanAccess() )
		{
			HANDLE h = m_hMailslot;
			m_hMailslot = NULL;
			::CloseHandle(h);
		}
		m_syncReceive.Unlock();
		return 0;
	}

	/**
	 * Mailslot���I�[�v��
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD m_Open(void)
	{
		if ( m_strMailslotName.IsEmpty() )
		{
			return ERROR_INVALID_PARAMETER;
		}
		CStr str = CStr::Fmt(_T("\\\\.\\mailslot\\%s"), m_strMailslotName);
		HANDLE h = ::CreateMailslot(
						str,					// ���[���X���b�g��
						0,						// �ő僁�b�Z�[�W�T�C�Y
						MAILSLOT_WAIT_FOREVER,	// �ǂݎ��^�C���A�E�g
						NULL					// �p���I�v�V����
		);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateMailslot");
		}
		m_hMailslot = h;
		return 0;
	}

	/**
	 * �R�[���o�b�N�ʒm���s
	 *	@param ev �ʒm������e
	 *	@retval true	����
	 *	@retval false	�ؒf�v��
	 */
	bool m_OnMailslotEvent(CMailslotEvent& ev)
	{
		bool boRc = __super::m_OnEvent(ev);
		if ( m_piMailslotListner != NULL )
		{
			boRc = m_piMailslotListner->OnMailslotEvent(ev);
		}
		return boRc;
	}

public:

	/** 
	 * �R���X�g���N�^ 
	 *	@param dwInterfaceId �C���^�[�t�F�[�XID�B�ȗ�����ƂO�B
	 *				(����́A�����̒ʐM�N���X�ŋ��ʂ̃��X�i�[���g�p���鎞�A���ʂƂ��Ďg�p���܂�)
	 */
	explicit CMailslot(DWORD dwInterfaceId=0)
		: __super(dwInterfaceId), m_cMailslotSender(false), m_hMailslot(NULL)
	{
	}

	/** �f�X�g���N�^ */
	virtual ~CMailslot(void)
	{
		Stop();
	}

	/**
	 * [�擾] �R�~���j�P�[�V������.
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const
	{
		return CStr::Fmt(_T("Mailslot[%s]:"), m_strMailslotName);
	}

	/**
	 * [�擾] �R�~���j�P�[�V����ID.
	 *		TNBVIW_KIND_??? + PORT ��Ԃ��܂��B
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return TNBVIW_KIND_MAILS + (reinterpret_cast<DWORD>(m_hMailslot) & 0xFFFF);
	}

	/**
	 * [�m�F] �J�n�`�F�b�N
	 *	@note Default���[�h�̏ꍇ�A���true��Ԃ��܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const 
	{
		return m_hMailslot != NULL;	
	}
	
	/**
	 * [�m�F] �ڑ��`�F�b�N
	 *	@note Default���[�h�̏ꍇ�A���true��Ԃ��܂��B
	 *	@retval true	�ڑ����B
	 *	@retval	false	�ؒf�B
	 */
	virtual bool IsConnect(void) const 
	{
		return IsAlive();
	}

	/**
	 * [�o�^] Listner�̓o�^
	 *	@note �o�^�ł��� CMailslot::IListener �͈�����ł��B
	 *	@param piMailslotListner ���X�i�[�B�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	void SetListener(IListener* piMailslotListner = NULL)
	{
		m_piMailslotListner = piMailslotListner;
	}

	/**
	 * [����] �I�[�v��.
	 *		�w��̖��O�̃��[���X���b�g���I�[�v�����܂��B
	 *		���ł�Open���Ă������x�N���[�Y���܂��B
	 *		��x�ݒ肷��Ǝ��񂩂�͈����Ȃ���Open���g�p�ł��܂��B
	 *	@see http://msdn.microsoft.com/library/ja/jpipc/html/_win32_createmailslot.asp
	 *	@param lpszMailslotName		��M�p���[���X���b�g��
	 *	@param lpszSendMailslotName ���M�惁�[���X���b�g���B���M���Ȃ��Ȃ�w�肵�Ȃ��Ă��B
	 *	@param lpszSendDomainName	���M��h���C����/�R���s���[�^��/* ���w��B�ȗ��Ȃ烍�[�J���B
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD Open(LPCTSTR lpszMailslotName, LPCTSTR lpszSendMailslotName = NULL, LPCTSTR lpszSendDomainName = NULL)
	{
		m_cMailslotSender.Open(lpszSendMailslotName, lpszSendDomainName);
		m_strMailslotName = lpszMailslotName;
		if ( CanAccess() )
		{
			Stop();	//�ڑ����Ȃ�Close���܂��B
		}
		return Open();
	}

	/**
	 * [����] �I�[�v��
	 *	@note	���ڈȍ~�A�g�p�ł��܂��B
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD Open(void)
	{
		if ( CanAccess() )
		{
			return 0;
		}
		DWORD dwOpenRc = m_Open();
		if ( dwOpenRc == 0 )
		{
			m_threadRecv.SetRunner( this ) ;
			#ifdef _DEBUG
				m_threadRecv.Start(CStr::Fmt(_T("Mailslot[%s]:�Ď�"), m_strMailslotName));
			#else
				m_threadRecv.Start();
			#endif
		}
		return dwOpenRc;
	}

	/**
	 * [����] �N���[�Y
	 *	@note ��M�p�X���b�h����~���܂��B
	 */
	virtual void Stop(void)
	{
		m_threadRecv.Stop();
		if ( CanAccess() )
		{
			//Port�̃N���[�Y
			m_syncReceive.Lock();
			HANDLE h = m_hMailslot;
			m_hMailslot = NULL;
			::CloseHandle(h);
			m_syncReceive.Unlock();
		}
	}

	/**
	 * [���M] �f�[�^���M
	 *	@param sizeData ���M����f�[�^���B
	 *	@param lpcvData ���M����f�[�^�B
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	virtual bool Send(size_t sizeData, LPCVOID lpcvData)
	{
		__super::m_OnSend(sizeData, lpcvData);
		return m_cMailslotSender.Send(sizeData, lpcvData);
	}

	/**
	 * [�擾] �n���h���擾
	 *	@return �n���h��
	 */
	operator HANDLE&()
	{
		return m_hMailslot;
	}
};



}; // TNB
