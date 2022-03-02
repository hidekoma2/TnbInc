#pragma once
/**
 *@file
 * �ʐM���|�[�^�֌W�̃w�b�_
 *
 *	@note	�}�N�� "_TnbREPORT_Viewer_ON" ����`����Ă��邩�A�f�o�O�r���h�Ȃ�A Viewer�ɂ��ʐM���O���Ƃ邱�Ƃ��o���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */


#include "TnbAbstractReport.h"
#include "TnbThread.h"
#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT COMMUNICATION
 * �ʐM���|�[�^�N���X
 *
 *		ICommunication �����A�񓯊��ʐM���T�|�[�g���܂�(IReport �C���^�[�t�F�[�X��񋟂��܂��j�B		
 *
 *	@note CCommunicationReporterT ���g�p�����ق����A�e���v���[�g�^�C�v�ŕ���邽�߁A�L�q���P�����ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCommunicationReporter.h
 * 
 *	@date 07/09/13 �V�K�쐬
 *	@date 08/10/15 �f�X�g���N�^���� Stop ��p�~�B
 *	@date 09/08/27 SetReceiveThreadPriority() �ǉ��B�\���ύX�B
 *	@date 09/09/10 SetReceiveThreadPriority() �p�~�A GetReceiveThreadStatusGet() �ǉ��B
 *	@date 10/05/28 SetTester() �̏����ɑΉ��B�|�[�����O�������P�B
 *	@date 10/08/05 ITester �p�~�B
 *	@date 11/03/01 �f�t�H���g�̃|�[�����O�Ԋu�� 1ms ��
 *	@date 11/03/10 ReportEvent_IrregularEnd ��ǉ�
 *	@date 11/06/14 GetReportID() ���\�b�h�̖߂�^�ύX�B
 *	@date 11/07/06 ���M�Ɏ��s���Ă��Ă����M���O���łĂ����̂��C���B
 */
class CCommunicationReporter : public CAbstractReport, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractReport);
public:

	/** 
	 * �R���X�g���N�^ 
	 *	@note �{�N���X�̃C���X�^���X�� ICommunication �̃C���X�^���X�𓯂��X�R�[�v�ɂ����ꍇ�A 
	 *		ICommunication �̂ق�����ɐ錾���Ă������Ɓi��ɍ\�z����A��ɔj������邽�߁j�B
	 *	@param pComm �ʐM�N���X�B�{�C���X�^���X���j�������܂ŁA�j�����Ȃ��ł��������B
	 *	@param dwInterfaceId �C���^�[�t�F�[�XID�B�ȗ�����ƂO�B
	 *				(����́A�����̒ʐM�N���X�ŋ��ʂ̃��X�i�[���g�p���鎞�A���ʂƂ��Ďg�p���܂�)
	 */
	explicit CCommunicationReporter(ICommunication* pComm, DWORD dwInterfaceId = 0) : _super(dwInterfaceId)
	{
		m_piCommunication = pComm;		// �ʐM�N���X
		m_lpbReceiveWork = NULL;		// ��M�p�o�b�t�@�A�h���X
		m_dwPollingTiming = 1;			// �Ԋu
		m_boIsConnect = false;
		m_receivePacketMaxSize = E_ReceiveDataBufferSize;
		m_hasOndemandStop = false;
	}

	/** �f�X�g���N�^ */
	virtual ~CCommunicationReporter(void)
	{
		if ( m_lpbReceiveWork != NULL )
		{
			delete[] m_lpbReceiveWork;
			m_lpbReceiveWork = NULL;
		}
	}

	/**
	 * [�擾] ���|�[�g��.
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const
	{
		return m_piCommunication->GetProperty().typeName;
	}

	/**
	 * [�擾] ���|�[�g ID.
	 *		��ނ̎��ʒl��Ԃ��܂��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return m_piCommunication->GetProperty().typeId;
	}

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		Open() ���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const
	{ 
		return m_threadRecv.IsAlive() && m_piCommunication->IsOpened();
	}

	/**
	 * [�m�F] �ڑ�����Ă��邩
	 *	@retval true	CTS ON��ԁB
	 *	@retval	false	CTS OFF��ԁB
	 */
	virtual bool IsConnect(void) const 
	{
		return m_piCommunication->IsConnect(); 
	}

	/** 
	 * [�ݒ�] �A�C�h���C�x���g�ݒ�
	 *	@note	��M���Ȃ����A�w��̊Ԋu�ŃR�[���o�b�N�𔭐������邱�Ƃ��ł��܂��B
	 *			�J�n��A���邢�͎�M������A�w��ms�b��ɃR�[���o�b�N���n�܂�܂��B
	 *			Open���ɗL���̐؂�ւ��͉\�ł��B
	 *	@note SetReceivePollingTiming() ��ICommunication �̃T�u�N���X��
	 *		{@link TNB::ICommunication::SetReceiveTimeout ��M�҂�����} ��
	 *		�ݒ�ɐ��x���e������܂��B
	 *	@param iInterval Idle�C�x���g��������Ԋu���w�肵�܂��B�O�Ȃ�AIdle�C�x���g�Ȃ��B
	 */
	virtual void SetIdleEventMode(int iInterval = 0)
	{
		_super::SetIdleEventMode(iInterval);
	}

	/**
	 * [�ݒ�] ��M�`�F�b�N�Ԋu�ݒ�.
	 *		��M��񂪂��邩�A�|�[�����O����^�C�~���O���w�肵�܂��B
	 *	@note	�ݒ肵�Ȃ��ꍇ�A1ms �ɂȂ��Ă��܂��B���A���^�C�����������ꍇ�A���̒l�����������Ȃ��ƁA
	 *			���M���Ă����M�܂łɎ��Ԃ��������Ă��܂��܂��B
	 *	@note ICommunication �̃T�u�N���X��{@link TNB::ICommunication::SetReceiveTimeout ��M�҂�����} ��
	 *			�L���Ȃ�A�����L���ɂ��A���̎�M�`�F�b�N�Ԋu�� 0 �ɂ���̂��x�X�g�ł��B
	 *	@param dwPollingTiming �^�C�~���O(�P��;�~���b)
	 */
	void SetReceivePollingTiming(DWORD dwPollingTiming)
	{
		m_dwPollingTiming = dwPollingTiming;
	}

	/**
	 * [����] �ʐM���|�[�g�J�n.
	 *	@note	�g�p���Ă���ʐM�N���X�ŁAOpen�ł���悤�ɐݒ��A�{���\�b�h���g�p���܂��B
	 *	@retval true �����B
	 *	@retval false �G���[�B ::GetLastError() �ŏڍ׎擾�\�i���̒l�� SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	virtual bool Start(void)
	{
		if ( m_threadRecv.IsAlive() )
		{
			//�X���b�h��~
			m_threadRecv.Stop();
		}
		if ( m_piCommunication->Open() )
		{
			const ICommunication::TProperty& param = m_piCommunication->GetProperty();
			if ( param.receivePacketSize != INVALID_SIZE )
			{
				//�T�C�Y�Œ肾
				m_receivePacketMaxSize = param.receivePacketSize;
			}
			if ( m_lpbReceiveWork != NULL )
			{
				delete[] m_lpbReceiveWork;
			}
			m_lpbReceiveWork = new BYTE[m_receivePacketMaxSize];
			//
			m_threadRecv.SetRunner(this);
			#ifdef _DEBUG
				m_threadRecv.Start(CStr::Fmt(_T("%s:�Ď�"), GetReportName()));
			#else
				m_threadRecv.Start();
			#endif
			return true;
		}
		return false;
	}

	/**
	 * [����] �ʐM���|�[�g��~.
	 *	@note ��M�p�X���b�h����~���܂��B
	 */
	virtual void Stop(void)
	{
		m_hasOndemandStop = true;
		m_piCommunication->Close();
		if ( m_threadRecv.IsAlive() )
		{
			//�X���b�h��~
			m_threadRecv.Stop();
		}
		m_hasOndemandStop = false;
	}

	/**
	 * [���M] �f�[�^���M
	 *	@param dataSize ���M����f�[�^���B
	 *	@param lpcvData ���M����f�[�^�B
	 *	@retval true	�����B
	 *	@retval	false	���s�B
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		if ( ! m_piCommunication->IsOpened() )
		{
			return false;
		}
		bool r = m_piCommunication->Send(dataSize, lpcvData) == dataSize;
		if ( r )
		{
			_super::m_OnSend(dataSize, lpcvData);
		}
		return r;
	}

	/**
	 * [�ݒ�] �p�[�W.
	 *	@note �ʐM�̃S�~����
	 */
	virtual void Purge(void)
	{
		EXCLUSIVE( &m_syncReceive );
		m_piCommunication->Purge();
	}

	/**
	 * [�Q��] ��M�X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadRecv;
	}

private:

	/// ���N���X�̌Œ�ݒ�
	enum
	{
		E_ReceiveDataBufferSize = 10000,		///< ��M�p�o�b�t�@�T�C�Y		
	};
	CSyncSection	m_syncReceive;			///< ���V�[�u�̔r��
	CThread			m_threadRecv;			///< ��M�X���b�h�Ǘ�
	ICommunication*	m_piCommunication;		///< �ʐM�N���X
	LPBYTE			m_lpbReceiveWork;		///< ��M�p�o�b�t�@�A�h���X
	DWORD			m_dwPollingTiming;		///< �Ԋu
	bool			m_boIsConnect;			///< �R�l�N�g�t���O
	bool			m_hasOndemandStop;		///< �X�g�b�v�v������
	size_t			m_receivePacketMaxSize;	///< ��M�p�P�b�g�ő�T�C�Y

	/**
	 * �X���b�h�����֐�
	 *	@note	��M����ƁA�R�[���o�b�N�֐����R�[�����܂��B
	 *			��M���s����ƁA�n���h�����N���[�Y���A
	 *			�R�[���o�b�N�֐��֒ʒm���A�X���b�h���I�����܂��B
	 *	@retval �O�i�Œ�)�B
	 */
	virtual DWORD Run(void)
	{
		CTickCount tick;
		bool isReceived = true;
		DWORD ec = 0;
		//
		while ( IsRunnable() )
		{
			if ( isReceived )
			{
				isReceived = false;
			}
			else if ( m_dwPollingTiming > 0 )
			{
				::Sleep(m_dwPollingTiming);
			}
			//== ��M���r��ON
			m_syncReceive.Lock();
			ICommunication::EState s = m_piCommunication->Check();
			ec = ::GetLastError();
			if ( s == ICommunication::CLOSED )
			{
				m_syncReceive.Unlock();
				break;	//Close���ꂽ��A�I��
			}
			if ( s == ICommunication::OPENNING && m_boIsConnect )
			{
				//��M���r��OFF
				m_syncReceive.Unlock();
				//�ؒf��ʒm�B
				m_OnEvent(CReportEvent(this, ReportEvent_Disconnect));
				m_boIsConnect = false;
				continue;
			}
			if ( s == ICommunication::CONNECTING && ! m_boIsConnect )
			{
				//�ڑ���ʒm�B
				if ( ! m_OnEvent(CReportEvent(this, ReportEvent_Connect)) )
				{
					break;
				}
				m_boIsConnect = true;
			}
			if ( m_piCommunication->CanReceive() )
			{
				size_t dwReceivedDataCount = INVALID_SIZE;
				{
					//---- 1Byte�ȏ゠�� or �킩��Ȃ�(INVALID_SIZE)�B
					if ( dwReceivedDataCount > m_receivePacketMaxSize )
					{
						dwReceivedDataCount = m_receivePacketMaxSize;
					}
					if ( m_lpbReceiveWork == NULL )
					{
						//��M���r��OFF
						m_syncReceive.Unlock();
						continue;
					}
					size_t dwReadBytes = m_piCommunication->Receive(dwReceivedDataCount, m_lpbReceiveWork);
					//
					if ( dwReadBytes == INVALID_SIZE )
					{
						//���s
						//��M���r��OFF
						m_syncReceive.Unlock();
						continue;
					}
					//�R�[���o�b�N�֐��֒ʒm�B
					if ( dwReadBytes > 0 )
					{
						//��M���r��OFF
						m_syncReceive.Unlock();
						CReportEvent ev(this);
						ev.SetData(dwReadBytes, m_lpbReceiveWork);
						if ( ! m_OnEvent(ev) )
						{
							break;
						}
						isReceived = true;
						tick.Reset();
						continue;
					}
				}
			}	
			//��M���r��OFF
			m_syncReceive.Unlock();
			if ( _super::GetIdleInterval() > 0 )
			{
				//---- ��M�f�[�^�Ȃ�&IDLE�C�x���g�v������B
				if ( tick.IsPassedAndReset(_super::GetIdleInterval()) )
				{
					//�R�[���o�b�N�֐��֒ʒm�B
					if ( ! m_OnEvent(CReportEvent(this, ReportEvent_Idle)) )
					{
						break;
					}
				}
			}
		}
		//�R�[���o�b�N�֐��֒ʒm�B
		if ( m_boIsConnect )
		{
			m_boIsConnect = false;
			//�ؒf��ʒm�B
			m_OnEvent(CReportEvent(this, ReportEvent_Disconnect));
		}
		m_piCommunication->Close();
		::SetLastError(ec);
		if ( ! m_hasOndemandStop )
		{ 
			m_OnEvent(CReportEvent(this, ReportEvent_IrregularEnd));
		}
		m_OnEvent(CReportEvent(this, ReportEvent_End));
		return 0;
	}

	friend class CCommunicationReporterTest;		///< �t�����h�N���X�錾
};



/**@ingroup REPORT COMMUNICATION
 * �ʐM���|�[�^�e���v���[�g
 *
 *		ICommunication �����A�񓯊��ʐM���T�|�[�g���܂�(IReport �C���^�[�t�F�[�X��񋟂��܂��j�B	
 *
 *	@note CCommunicationReporter �ɔ�ׁA�e���v���[�g�^�C�v�ŕ���邽�߁A�L�q���P�����ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCommunicationReporter.h
 *
 *	@param COMM ICommunication �̃T�u�N���X��
 * 
 *	@date 09/03/31 �V�K�쐬
 *	@date 11/02/04 �f�X�g���N�^���ɃN���[�Y����悤�ɂ����B
 */
template<typename COMM>
class CCommunicationReporterT : public CCommunicationReporter
{
	DEFSUPER(CCommunicationReporter);
public:

	/**
	 * �R���X�g���N�^
	 *	@param dwInterfaceId �C���^�[�t�F�[�XID�B�ȗ�����ƂO�B
	 *				(����́A�����̒ʐM�N���X�ŋ��ʂ̃��X�i�[���g�p���鎞�A���ʂƂ��Ďg�p���܂�)
	 */
	explicit CCommunicationReporterT(DWORD dwInterfaceId = 0) : _super(&m_comm, dwInterfaceId)
	{
	}

	/// �f�X�g���N�^
	virtual ~CCommunicationReporterT(void)
	{
		Stop();
	}

	/**
	 * [�Q��] �ʐM�N���X�Q��.
	 *	@note ����Ă���ʐM�N���X�̎Q�Ƃ�Ԃ��܂��B
	 *	@return �ʐM�N���X
	 */
	COMM& ReferCommunication(void)
	{
		return m_comm;
	}

	/**
	 * [�Q��] �ʐM�N���X�Q��.
	 *	@note ����Ă���ʐM�N���X�̎Q�Ƃ�Ԃ��܂��B
	 *	@return �ʐM�N���X
	 */
	COMM* operator->(void)
	{
		return &m_comm;
	}

	/**
	 * [�Q��] �ʐM�N���X�Q��.
	 *	@note ����Ă���ʐM�N���X�̎Q�Ƃ�Ԃ��܂��B
	 *	@return �ʐM�N���X
	 */
	const COMM& ReferCommunication(void) const
	{
		return m_comm;
	}

	/**
	 * [�Q��] �ʐM�N���X�Q��.
	 *	@note ����Ă���ʐM�N���X�̎Q�Ƃ�Ԃ��܂��B
	 *	@return �ʐM�N���X
	 */
	const COMM* operator->(void) const
	{
		return &m_comm;
	}

private:
	COMM	m_comm;
};



}; // TNB