#pragma once
/**
 *@file
 * �o�b�t�@�����O���|�[�^�[�֌W�̃w�b�_
 *
 *		IReport ����̓ǂݏo�����o�b�t�@�����O���A��M�������y�����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAbstractReport.h"
#include "TnbThread.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * �o�b�t�@�����O���|�[�^�[�N���X
 *
 *
 *
 *	@par�K�v�t�@�C��
 *			TnbBufferingReporter.h
 *
 *	@date 12/03/13	�V�K�쐬
 */
class CBufferingReporter : public CAbstractReport
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param queueSize �o�b�t�@�L���[�̐��B�ȗ������10000�ł��B
	 */
	explicit CBufferingReporter(int queueSize = 10000) : m_in(queueSize)
	{
	}

	/**
	 * [�ݒ�] ���ݒ�.
	 *		�g�p���郌�|�[�g���w�肵�܂��B�K���w�肷��K�v������܂��B
	 *	@param pReport ���|�[�g�B
	 */
	void SetEnvironment(IReport* pReport)
	{
		m_in.pReport = pReport;
	}
	
	/**
	 * [�o�^] ���� Listner �o�^.
	 *	@note �o�^�ł��� IReport::IListener �͈�����ł��B
	 *	@param piCommListener ���X�i�[�B�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL)
	{
		m_in.pListener = piCommListener;
	}
	
	/**
	 * [�擾] ���|�[�g��.
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const
	{
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->GetReportName();
	}

	/**
	 * [�擾] ���|�[�g ID.
	 *		��ނ̎��ʒl��Ԃ��܂��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->GetReportID();
	}

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		Open() ���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const
	{ 
		ASSERT( m_in.pReport != NULL );
		return m_thread.IsAlive() && m_in.pReport->IsAlive();
	}

	/**
	 * [�m�F] �ڑ�����Ă��邩
	 *	@retval true	�ڑ��B
	 *	@retval	false	���ڑ��B
	 */
	virtual bool IsConnect(void) const 
	{
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->IsConnect(); 
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
		ASSERT( m_in.pReport != NULL );
		m_in.pReport->SetIdleEventMode(iInterval);
	}

	/**
	 * [����] �ʐM���|�[�g�J�n.
	 *	@note	�g�p���Ă���ʐM�N���X�ŁAOpen�ł���悤�ɐݒ��A�{���\�b�h���g�p���܂��B
	 *	@retval true �����B
	 *	@retval false �G���[�B ::GetLastError() �ŏڍ׎擾�\�i���̒l�� SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	virtual bool Start(void)
	{
		if ( m_in.pReport != NULL )
		{
			if ( m_thread.IsAlive() )
			{
				//�X���b�h��~
				m_thread.Stop();
			}
			m_in.pReport->SetListener(&m_in);
			if ( m_in.pReport->Start() )
			{
				m_thread.SetRunner(&m_in);
				m_in.event.ToLock();
				m_in.canRunnable = true;
				#ifdef _DEBUG
					m_thread.Start(CStr::Fmt(_T("BufferingReport[%s]:�Ď�"), GetReportName()));
				#else
					m_thread.Start();
				#endif
				return true;
			}
		}
		return false;
	}

	/**
	 * [����] �ʐM���|�[�g��~.
	 *	@note ��M�p�X���b�h����~���܂��B
	 */
	virtual void Stop(void)
	{
		ASSERT( m_in.pReport != NULL );
		m_in.canRunnable = false;
		m_in.event.Unlock();
		m_in.pReport->Stop();
		if ( m_thread.IsAlive() )
		{
			//�X���b�h��~
			m_thread.Stop();
		}
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
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->Send(dataSize, lpcvData);
	}

	/**
	 * [�ݒ�] �p�[�W.
	 *	@note �ʐM�̃S�~����
	 */
	virtual void Purge(void)
	{
		ASSERT( m_in.pReport != NULL );
		m_in.pReport->Purge();
	}

	/**
	 * [�Q��] ��M�X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

private:
	/// �����p�N���X
	struct TInner : IReport::IListener, TNB::CThread::IRunner
	{
		DEFPARENTLISTENER(CBufferingReporter, IParentListener);

		IReport*						pReport;		///< �o�^���ꂽ IReport
		IParentListener*				pListener;		///< �o�^���ꂽ���X�i�[
		CBlockingQueueT<CByteVector>	queue;			///< �o�b�t�@�L���[
		CSyncEvent						event;			///< �C�x���g
		bool							canRunnable;	///< �X���b�h�p���t���O
		/// �R���X�g���N�^
		TInner(int queueSize = 10000) : queue(queueSize), event(true, true), pReport(NULL), pListener(NULL)
		{
		}
		/// [�ʒm] �C�x���g����
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			if ( pListener != NULL )
			{
				if ( ev.HasEvent() )
				{
					return pListener->OnReportEvent(ev);
				}
				CByteVector vb;
				vb.SetElements(ev.GetDataSize(), ev.GetData());
				queue.Add(vb);
				event.Pulse();
				TRACE0("a1\n");
			}
			return true;
		}
		/// �X���b�h
		virtual DWORD Run(void)
		{
			while ( IsRunnable() && canRunnable )
			{
				::WaitForSingleObject(event, 100000);
				TRACE0("p1\n");
				while ( ! queue.IsEmpty() )
				{
					queue.Lock();
					CByteVector vb = queue.Take();
					queue.Unlock();
					if ( pListener != NULL )
					{
						CReportEvent e(pReport);
						e.SetData(vb.GetSize(), vb.ReferBuffer());
						pListener->OnReportEvent(e);
					}
				}
			}
			TRACE0("p2\n");
			return 0;
		};
	};
	TInner		m_in;		///< �������Ǘ�
	CThread		m_thread;	///< �X���b�h
};



}; // TNB

