#pragma once
/**
 *@file
 * �ʐM�񍐊Ď��A�_�v�^�֌W�̃w�b�_
 *
 *		IReport �̃T�u�N���X�̑���M���Ď��i���K�[�ɒʒm�j���邱�Ƃ��o���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbReport.h"
#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * �ʐM�񍐊Ď��A�_�v�^
 *
 *		���|�[�^�[�i IReport �����������N���X�j��{�N���X�ɓo�^( SetReporter() )���Ďg�p���܂��B
 *		�{�N���X�͓o�^�������|�[�^�[���g���ĒʐM���s���܂��B���̍ہA�ʐM���e�����K�[�i ILogger �����������N���X�j
 *		�ɒʒm���邱�Ƃ��o���܂��B
 *
 *	\par�g�p��
 *	\code
 *
 *	// .h
 *	class CFooDlg : public CDialog, CReportWatchAdapter::ILogger
 *	{
 *	protected:
 *		virtual void OnReportWatchEventLog(EReportEvent ev);
 *		virtual void OnReportWatchDataLog(EKind kind, size_t dataSize, const BYTE* pData);
 *	private:
 *		CReportWatchAdapter					m_rwp;
 *		CCommunicationReporterT<CRs232c>	m_rs232c;
 *		CPacketCommander					m_commander;
 *	};
 *
 *	// .cpp
 *	CFooDlg::CFooDlg(void) : CDialog(CFooDlg::IDD, pParent)
 *			, m_rwp(&m_rs232c)				// �A�_�v�^���g���ʐM�N���X���w��
 *	{
 *	}
 *	void CFooDlg::OnReportWatchEventLog(EReportEvent ev)
 *	{
 *		//���O�\��
 *	}
 *	void CFooDlg::OnReportWatchDataLog(EKind kind, size_t dataSize, const BYTE* pData)
 *	{
 *		//���O�\��
 *	}
 *	void CFooDlg::OnInitDialog()
 *	{
 *			;
 *		m_rwp.SetLogger(this);
 *		m_commander.Open(&m_rwp, true);	//�A�_�v�^�̂ق����w�肷��B�ʐM�� m_rs232c ���S���B
 *			;
 *	}
 *
 *	\endcode
 *	@par�K�v�t�@�C��
 *			TnbReportWatchAdapter.h
 * 
 *	@date 11/07/06 �V�K�쐬
 */
class CReportWatchAdapter : public IReport, CCopyImpossible
{
	DEFSUPER(IReport);
public:

	/**@ingroup REPORT
	 * �ʐM�񍐊Ď��A�_�v�^�̃��K�[�C���^�[�t�F�[�X
	 *
	 *		IReport �̒ʒm�p�C���^�[�t�F�[�X�ł��B
	 *
	 *	@see CReportWatchAdapter::SetLogger()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbReportWatchAdapter.h
	 * 
	 *	@date 11/07/06 �V�K�쐬
	 */
	struct ILogger
	{
		/// ���
		enum EKind
		{
			Kind_Send,		///< ���M
			Kind_SendError,	///< ���M�G���[(���ۂ͑��M����Ă��Ȃ�)
			Kind_Received,	///< ��M
		};

		/// �f�X�g���N�^
		virtual ~ILogger(void) {}

		/**
		 * [�ʒm] �C�x���g����.
		 *		 �f�[�^����M�������A�C�x���g�������ɃR�[������܂��B
		 *	@param ev �C�x���g
		 */
		virtual void OnReportWatchEventLog(EReportEvent ev) = 0;

		/**
		 * [�ʒm] �f�[�^�ʐM����.
		 *		 �f�[�^�̑���M���ɃR�[������܂��B
		 *	@param kind ���
		 *	@param dataSize �f�[�^��
		 *	@param pData �f�[�^
		 */
		virtual void OnReportWatchDataLog(EKind kind, size_t dataSize, const BYTE* pData) = 0;
	};


	//-----------------------------


	/**
	 * �R���X�g���N�^.
	 *	@param pRep ���|�[�^�̃C���X�^���X
	 */
	CReportWatchAdapter(IReport* pRep)
	{
		ASSERT( pRep != NULL );	//�C���X�^���X���w�肵�Ă�������
		m_pReporter = pRep;
		m_in.pListener = NULL;
		m_in.pLogger = NULL;
	}

	/**
	 * [�o�^] ���|�[�^�[�o�^.
	 *	@param pRep ���|�[�^
	 *	@retval true ����.
	 *	@retval false ���s�B Start() ��͎��s���܂��B pRep �� NULL �̎������s���܂��B
	 */
	bool SetReporter(IReport* pRep)
	{
		if ( ! IsAlive() && pRep != NULL )
		{
			m_pReporter = pRep;
			return true;
		}
		return false;
	}

	/**
	 * [�o�^] ���K�[�o�^.
	 *	@param pLogger ���K�[
	 */
	void SetLogger(ILogger* pLogger)
	{
		m_in.pLogger = pLogger;
	}

	/**
	 * [�擾] �R�~���j�P�[�V������.
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const
	{
		return m_pReporter->GetReportName();
	}

	/**
	 * [�擾] �R�~���j�P�[�V����ID.
	 *		TNBVIW_KIND_??? + PORT ��Ԃ��܂��B
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return m_pReporter->GetReportID();
	}

	/**
	 * [�o�^] ����Listner�o�^.
	 *	@note �o�^�ł��� IReport::IListener �͈�����ł��B
	 *	@param piCommListener ���X�i�[�B�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL)
	{
		m_in.pListener = piCommListener;
	}

	/**
	 * [�擾] �C���^�[�t�F�[�XID�擾
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const
	{
		return m_pReporter->GetInterfaceId();
	}

	/** 
	 * [�ݒ�] �A�C�h���C�x���g�ݒ�
	 *	@note	��M���Ȃ����A�w��̊Ԋu�ŃR�[���o�b�N�𔭐������邱�Ƃ��ł��܂��B
	 *			�J�n��A���邢�͎�M������A�w��ms�b��ɃR�[���o�b�N���n�܂�܂��B
	 *			Open���ɗL���̐؂�ւ��͉\�ł��B
	 *	@param iInterval Idle�C�x���g��������Ԋu���w�肵�܂��B�O�Ȃ�AIdle�C�x���g�Ȃ��B
	 */
	virtual void SetIdleEventMode(int iInterval = 0)
	{
		m_pReporter->SetIdleEventMode(iInterval);
	}

	/**
	 * [����] �J�n.
	 *	@note �ݒ胁�\�b�h�Őݒ���s���Ă���{���\�b�h���g�p���A�J�n���܂��B
	 *	@retval true �����BAlive��ԂɂȂ�܂����B
	 *	@retval false ���s�BAlive��Ԃɂ͂Ȃ�܂���ł����B
	 */
	virtual bool Start(void)
	{
		m_pReporter->SetListener(&m_in);
		return m_pReporter->Start();
	}

	/**
	 * [����] ��~.
	 *		Alive��Ԃ��I�����܂��B
	 */
	virtual void Stop(void)
	{
		m_pReporter->Stop();
	}

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		�e�T�u�N���X�̊J�n���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const
	{
		return m_pReporter->IsAlive();
	}
	
	/**
	 * [�m�F] �ڑ��`�F�b�N
	 *	@retval true	�ڑ����B
	 *	@retval	false	�ؒf�B
	 */
	virtual bool IsConnect(void) const
	{
		return m_pReporter->IsConnect();
	}

	/**
	 * [���M] �f�[�^���M
	 *	@param dataSize ���M����f�[�^���B
	 *	@param lpcData ���M����f�[�^�B
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcData)
	{
		bool r = m_pReporter->Send(dataSize, lpcData);
		if ( m_in.pLogger != NULL )
		{
			EXCLUSIVE( &m_in.sync );
			m_in.pLogger->OnReportWatchDataLog((r ? (ILogger::Kind_Send) : (ILogger::Kind_SendError)), dataSize, static_cast<const BYTE*>(lpcData));
		}
		return r;
	}

	/**
	 * [�ݒ�] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void)
	{
		m_pReporter->Purge();
	}

private:
	/// �����N���X
	class CInner : public IReport::IListener
	{
	public:
		IReport::IListener*	pListener;		// ���X�i�[
		ILogger*			pLogger;		// ���K�[
		CSyncSection		sync;			// ����
		//[�ʒm] �C�x���g����
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			if ( pListener != NULL )
			{
				if ( pLogger != NULL )
				{
					EXCLUSIVE( &sync );
					if ( ev.HasData() )
					{
						pLogger->OnReportWatchDataLog(ILogger::Kind_Received, ev.GetDataSize(), ev.GetData());
					}
					else
					{
						pLogger->OnReportWatchEventLog(ev.GetEvent());
					}
				}
				return pListener->OnReportEvent(ev);
			}
			return true;
		}
	};
	CInner		m_in;			///< �����N���X
	IReport*	m_pReporter;	///< ���|�[�^�[
};



};
