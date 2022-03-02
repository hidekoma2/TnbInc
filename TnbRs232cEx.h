#pragma once
/**
 *@file
 * RS232C�̃A�N�Z�X�֌W�̃w�b�_
 *
 *	@note	�}�N�� "_TnbRS232C_EnumExistPorts_DISABLE" ����`����Ă���ƁA CRs232c::EnumExistPorts() 
 *			���g�p�֎~�ɂ��܂��B���̑���A TnbRegistryAccessor.h �͕s�K�v�ɂȂ�܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbRs232c.h"
#include "TnbThread.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

class COverlapped : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	COverlapped(void) : m_hEvent(NULL)
	{
		Zero(m_overLapped);
	}

	/// �f�X�g���N�^
	~COverlapped(void)
	{
		if ( m_hEvent != NULL )
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL; 
		}
	}

	/**
	 * [�Q��] OVERLAPPED �\���̎Q��.
	 *	@note ���e��ݒ肵���\���̂�Ԃ��܂��B
	 *	@return OVERLAPPED �\���̎Q��
	 */
	operator OVERLAPPED*(void)
	{
		Zero(m_overLapped);
		if ( m_hEvent == NULL )
		{
			m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
			::ResetEvent(m_hEvent);
		}
		::ResetEvent(m_hEvent);
		m_overLapped.hEvent = m_hEvent; 
		return &m_overLapped;
	}
	
	/**
	 * [�擾] ���ʎ擾.
	 *	@param h �n���h���B
	 *	@param wait �҂�����(ms)�B INFINITE ���w�肷��ƁA���ʂ��A��܂� Wait���܂��B
	 *	@retval 0 �^�C���A�E�g�i�܂����ʖ����j�B
	 *	@retval INVALID_SIZE �ǂݍ��݃G���[�B
	 *	@retval ��L�ȊO �Ǎ��ݐ����B
	 */
	size_t GetResult(HANDLE h, DWORD wait = 0)
	{
		if ( ::GetLastError() == ERROR_IO_PENDING )
		{
			// �񓯊��ǂݎ�葀��̌��ʂ��`�F�b�N����B
			::Sleep(0);
			DWORD work = 0;
			if( ::GetOverlappedResult(h, &m_overLapped, &work, FALSE) )
			{
				return work;
			}
			else if ( ::GetLastError() != ERROR_IO_INCOMPLETE )
			{
				return INVALID_SIZE;
			}
		}
		size_t result = INVALID_SIZE;
		DWORD r = 0;
		switch ( ::WaitForSingleObject(m_overLapped.hEvent, wait) )
		{
		case WAIT_OBJECT_0:
			if ( ::GetOverlappedResult(h, &m_overLapped, &r, FALSE) )
			{
				result = r; // ����
				::CancelIo(h);
			}
			break;
		case WAIT_TIMEOUT:
			result = 0;
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
		default:
			::CancelIo(h);
			break;
		}
		return result;
	}
	HANDLE			m_hEvent;		///< �C�x���g�n���h��
	OVERLAPPED		m_overLapped;	///< �I�[�o�[���b�v�Ǘ�
};

#endif //_TnbDOXYGEN



/**@ingroup COMMUNICATION
 * �g��RS232C�A�N�Z�X�N���X.
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbRs232cEx.h
 * 
 *	@date 09/11/13	�V�K�쐬
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 */
class CRs232cEx : public CRs232c, CThread::IRunner
{
	DEFSUPER(CRs232c);
public:

	/**@ingroup COMMUNICATION
	 * �g��RS232C�C�x���g���X�i�[
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbRs232cEx.h
	 * 
	 *	@date 09/11/13	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �C�x���g�ʒm.
		 *		RS232C�̃C�x���g������������A�ʒm����܂��B
		 *	@param event �C�x���g�B�ȉ��̃C�x���g�� or �ŕ����ʒm����܂��B�N���[�Y���ꂽ���� 0 ���ʒm����܂��B
		 *		- EV_BREAK		-- ���͏�Ńu���[�N�����o�����Ƃ��B 
		 *		- EV_CTS		-- CTS�i ���M�j�M���̏�Ԃ��ς�����Ƃ��B 
		 *		- EV_DSR		-- DSR�i �f�[�^�Z�b�g���f�B�j�M���̏�Ԃ��ς�����Ƃ��B 
		 *		- EV_ERR		-- �����ԃG���[�����������Ƃ��B�����ԃG���[�ɂ́ACE_FRAME�ACE_OVERRUN�ACE_RXPARITY ������܂��B 
		 *		- EV_RING		-- �Ăяo���M�������o�����Ƃ��B 
		 *		- EV_RLSD		-- RLSD�i ��M���M�����o�j�M���̏�Ԃ��ς�����Ƃ��B 
		 *		- EV_RXCHAR		-- 1 ������M���A���̓o�b�t�@�ɓ��ꂽ�Ƃ��B
		 *		- EV_RXFLAG		-- �C�x���g��������M���A���̓o�b�t�@�ɓ��ꂽ�Ƃ��B�C�x���g�����̓f�o�C�X�� DCB �\���̂Ŏw�肵�܂��B���̍\���̂��g���ăV���A���|�[�g��ݒ肷��ɂ́ASetCommState �֐����g���܂��B 
		 *		- EV_TXEMPTY	-- �o�̓o�b�t�@�̍Ō�̕����𑗐M�����Ƃ��B
		 */
		virtual void OnRs232cEvent(DWORD event) = 0;
	};


	//-----------------------------


	/// �R���X�g���N�^
	CRs232cEx(void) : m_propertyEx(_T("RS232C"), TNBVIW_KIND_RS232C), m_pListener(NULL)
	{
		m_propertyEx = _super::GetProperty();
		m_propertyEx.receiveTimeout = 300;
		m_eventMask = EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY | EV_CTS | EV_DSR | EV_RLSD | EV_BREAK;
		m_eventMask |= EV_ERR | EV_RING | EV_PERR | EV_RX80FULL | EV_EVENT1 | EV_EVENT2;
	}

	/// �f�X�g���N�^
	~CRs232cEx(void)
	{
		Close();
	}


	/**
	 * [�ݒ�] �Ď�����C�x���g�w��.
	 *	@param event �Ď�����C�x���g�B�ȉ��̃C�x���g�� or �ŘA�����Ă��Ă��܂��B�f�t�H���g�͑S�ĊĎ��ΏۂɂȂ��Ă��܂��B
	 *		- EV_BREAK		-- ���͏�Ńu���[�N�����o�����Ƃ��B 
	 *		- EV_CTS		-- CTS�i ���M�j�M���̏�Ԃ��ς�����Ƃ��B 
	 *		- EV_DSR		-- DSR�i �f�[�^�Z�b�g���f�B�j�M���̏�Ԃ��ς�����Ƃ��B 
	 *		- EV_ERR		-- �����ԃG���[�����������Ƃ��B�����ԃG���[�ɂ́ACE_FRAME�ACE_OVERRUN�ACE_RXPARITY ������܂��B 
	 *		- EV_RING		-- �Ăяo���M�������o�����Ƃ��B 
	 *		- EV_RLSD		-- RLSD�i ��M���M�����o�j�M���̏�Ԃ��ς�����Ƃ��B 
	 *		- EV_RXCHAR		-- 1 ������M���A���̓o�b�t�@�ɓ��ꂽ�Ƃ��B
	 *		- EV_RXFLAG		-- �C�x���g��������M���A���̓o�b�t�@�ɓ��ꂽ�Ƃ��B�C�x���g�����̓f�o�C�X�� DCB �\���̂Ŏw�肵�܂��B���̍\���̂��g���ăV���A���|�[�g��ݒ肷��ɂ́ASetCommState �֐����g���܂��B 
	 *		- EV_TXEMPTY	-- �o�̓o�b�t�@�̍Ō�̕����𑗐M�����Ƃ��B
	 */
	void SetListenEvent(DWORD event)
	{
		m_eventMask = event;
	}

	/**
	 * [�ݒ�] ���X�i�[�o�^.
	 *	@param P ���X�i�[
	 */
	void SetListener(IListener* P)
	{
		m_pListener = P;
	}

	/**
	 * [�擾] ���\�擾
	 *	@note ���m�ȏ��� Open��Ɏ擾�ł��܂��B
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_propertyEx;
	}

	/**
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@return true �����B
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		m_propertyEx.receiveTimeout = to;
		return true;
	}

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void)
	{
		bool r = _super::Open();
		m_propertyEx.typeName = _super::GetProperty().typeName;
		m_propertyEx.typeId = _super::GetProperty().typeId;
		if ( r )
		{
			m_thread.SetRunner(this);
			return m_thread.Start();
		}
		Close();
		return false;
	}

	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
		::SetCommMask(GetHandle(), 0);
		_super::Close();
		m_thread.Stop();
		m_propertyEx.typeName = _super::GetProperty().typeName;
		m_propertyEx.typeId = _super::GetProperty().typeId;
	}


	/**
	 * [����] ��M.
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval 0 ��M�f�[�^�Ȃ��B
	 *	@retval ��L�ȊO ��M�f�[�^�T�C�Y�B
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		if ( size == 0 || m_propertyEx.receiveTimeout == 0 )
		{
			DWORD dwErrors;
			COMSTAT tComStat;
			if ( ! ::ClearCommError(GetHandle(), &dwErrors, &tComStat) )
			{
				_GetLastError("ClearCommError");
				return INVALID_SIZE;
			}
			if ( size > tComStat.cbInQue )
			{
				size = tComStat.cbInQue;
			}
			if ( size == 0 )
			{
				return 0;
			}
		}
		size_t redSize  = 0;
		if ( ReadPort(redSize, _P, size, GetHandle()) )
		{
			return redSize;
		}
		return INVALID_SIZE;
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
		::SetCommMask(GetHandle(), m_eventMask);
		DWORD event;
		while ( IsRunnable() )
		{
			::WaitCommEvent(GetHandle(), &event, m_eventOverlapped);
			if ( ::GetLastError() != ERROR_ACCESS_DENIED )
			{
				DWORD r = m_eventOverlapped.GetResult(GetHandle(), INFINITE);
				if ( r == sizeof(DWORD) )
				{
//					TRACE1("�� RS232C Event = 0x%04X\n", event);
					if ( m_pListener != NULL )
					{
						m_pListener->OnRs232cEvent(event);
					}
					if ( event == 0 )
					{
						break;
					}
				}
			}
			else
			{
				::SetCommMask(GetHandle(), m_eventMask);
			}
		}
		return 0;
	};

	/**
	 * [�ʒm] �|�[�g�쐬.
	 *		�|�[�g�쐬���A�R�[������܂��B
	 *	@param lpszPortName �|�[�g��
	 *	@retval INVALID_HANDLE_VALUE ���s�B
	 *	@retval ��L�ȊO �����B�l�̓n���h���B�g�p��� CloseHandle() ���Ă��������B
	 */
	virtual HANDLE CreatePort(LPCTSTR lpszPortName) const
	{
		DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;	// �A�N�Z�X (�ǂݎ�肨��я�������) ���[�h
		DWORD flag = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
		HANDLE h = ::CreateFile(lpszPortName, desiredAccess, 0, NULL, OPEN_EXISTING, flag, NULL);
		_GetLastError("CreateFile");
		return h;
	}

	/**
	 * [�ʒm] �|�[�g��������.
	 *		�|�[�g�������ݎ��A�R�[������܂��B
	 *	@param[out] _writtenSize �������񂾃T�C�Y���i�[����܂��B
	 *	@param[in] len �������݃f�[�^�T�C�Y
	 *	@param[in] P �������݃f�[�^
	 *	@param[in] h �n���h��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool WritePort(size_t& _writtenSize, size_t len, LPCVOID P, HANDLE h) const
	{
		DWORD dwWritten = 0;
		::WriteFile(h, P, ToDword(len), &dwWritten, m_writeOverlapped);
		dwWritten = m_writeOverlapped.GetResult(h, INFINITE);
		if ( dwWritten == INVALID_SIZE )
		{
			return false;
		}
		_writtenSize = dwWritten;
		return true;
	}

	/**
	 * [�ʒm] �|�[�g�ǂݍ���.
	 *		�|�[�g�ǂݍ��ݎ��A�R�[������܂��B
	 *	@param[out] _redSize �������񂾃T�C�Y���i�[����܂��B
	 *	@param[out] _P �ǂݍ��݃f�[�^���i�[����܂��B
	 *	@param[in] len �ǂݍ��݃f�[�^�ő�
	 *	@param[in] h �n���h��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool ReadPort(size_t& _redSize, LPVOID _P, size_t len, HANDLE h) const
	{
		DWORD dwRed = 0;
		::ReadFile(h, _P, ToDword(len), &dwRed, m_readOverlapped);
		dwRed = m_readOverlapped.GetResult(h, m_propertyEx.receiveTimeout);
		if ( dwRed == INVALID_SIZE )
		{
			return false;
		}
		_redSize = dwRed;
		return true;
	}

private:
	DWORD					m_eventMask;		///< �Ď��Ώ�
	IListener*				m_pListener;		///< ���X�i�[
	TProperty				m_propertyEx;		///< ���\
	mutable CThread			m_thread;			///< �X���b�h
	mutable COverlapped		m_readOverlapped;	///< R �I�[�o�[���b�v
	mutable COverlapped		m_writeOverlapped;	///< W �I�[�o�[���b�v
	mutable COverlapped		m_eventOverlapped;	///< �C�x���g�I�[�o�[���b�v
};



}; // TNB
