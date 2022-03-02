#pragma once
/**
 *@file
 * �L���[�C���O���|�[�g�֌W�̃w�b�_
 *
 *		IReport ����̓ǂݏo�����L���[�C���O�������邱�ƂŁA��͏������s���₷�����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbReport.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * �L���[�C���O���|�[�g�N���X
 *
 *		IReport ����ǂ݂������f�[�^���L���[�C���O���Ă��܂��B���̃f�[�^�� IListener �Ŏ擾���邱�Ƃ��\�ł��B
 *		�L���[�C���O���Ă���f�[�^�̂��߁A��͂��e�ՂɂȂ�܂��B
 *
 *	@note �o�͂̓L���[�C���O�T�|�[�g���܂���B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		class CFoo : private CQueueingReporter::IListener
 *		{
 *		public:
 *			/// �I�[�v��.
 *			bool Open(IReport* pReport)
 *			{
 *				m_queue.SetAliveTime(100); //100ms �o���Ă��p�P�b�g�������Ȃ��͔̂j��
 *				m_queue.SetEnvironment(pReport, this);
 *					;
 *			}
 *
 *		private:
 *			virtual bool OnReportEvent(const CReportEvent& ev)
 *			{
 *				ASSERT( ! ev.HasData() );
 *				ev.GetEvent();
 *				return true;
 *			}
 *
 *			virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c)
 *			{
 *				// (c �����)
 *				if ( r == 0 )
 *				{
 *					return 0; // ��͂���ɂ̓f�[�^������Ȃ�
 *				}
 *				if ( r < 0 )
 *				{
 *					return 1; // �w�b�_��񂪂��������̂� 1byte �̂Ă�
 *				}
 *				// (�p�P�b�g�m�F)
 *				return r; // ��͂����p�P�b�g����Ԃ�
 *			}
 *		};
 *
 *	</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbQueueingReporter.h
 * 
 *	@date 08/09/09	�V�K�쐬
 *	@date 09/02/19	IsAlive(), IsConnect(), Send() ��ǉ��B
 *	@date 09/03/25	Purge() ��ǉ��B
 *	@date 09/03/31	CQueueingReporter �ɉ����B
 *	@date 09/04/09	�L���[�̗L�����Ԃ�ݒ�\�ɁB
 *	@date 11/07/14	SetAliveTime() �̎��Ԃ̈Ӗ���ύX
 *	@date 11/09/20	IListener::OnReportDustyData() �ǉ��B
 *	@date 15/02/20	SetTearminalTime() ��V�K�B���ԂŏI�[�����߂郂�[�h��ǉ��B
 */
class CQueueingReporter : CCopyImpossible
{
public:

	/**@ingroup REPORT
	 * �L���[�C���O���|�[�g�̃��X�i�[�C���^�[�t�F�[�X
	 * 
	 *	@see CQueueingReporter::SetEnvironment()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbQueueingReporter.h
	 *
	 *	@date 08/09/09	�V�K�쐬
	 *	@date 11/09/20	OnReportDustyData() �ǉ��B
	 */
	struct IListener
	{
		/**�f�X�g���N�^*/
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �C�x���g�����ʒm
		 *	@note �C�x���g�������������ɃR�[������܂��B
		 *	@param ev ��M�������e�i�C�x���g�̂݁j�B
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnReportEvent(const CReportEvent& ev) = 0;

		/**
		 * [�ʒm] �f�[�^��M�ʒm
		 *	@note �f�[�^����M�������ɃR�[������܂��B�f�[�^����͂��A�������Ă��������B
		 *	@param[in] boIsLast true �Ȃ�N���[�Y���ꂽ���߁A�Ō�̃f�[�^�ł��邱�Ƃ�\���B
		 *	@param[in] c �ϊ���RAW�f�[�^(��M�f�[�^)�B
		 *	@return ���������f�[�^��
		 */
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c) = 0;

		/**
		 * [�ʒm] �S�~�f�[�^���ʒm.
		 *		�L���[�� CQueueingReporter::SetAliveTime() �Ŏw�肵�����ԁA���܂��Ă���Ɣj�����܂��B���̔j�������f�[�^��ʒm���܂��B
		 *	@param b �j�����ꂽ�f�[�^
		 */
		virtual void OnReportDustyData(BYTE b) {}
	};

	/**
	 * �R���X�g���N�^
	 *	@param iQueueSize ��MByte�L���[�̃T�C�Y�B�ȗ������10000�ł��B
	 */
	explicit CQueueingReporter(int iQueueSize = 10000) : m_in(iQueueSize)
	{
	}

	/// �f�X�g���N�^
	~CQueueingReporter(void)
	{
		ResetEnvironment();
	}

	/**
	 * [�ݒ�] ���ݒ�
	 *	@note �{�N���X�̃C���X�^���X�ƃ��|�[�^�[�̃C���X�^���X�𓯂��X�R�[�v�ɂ����ꍇ�A 
	 *		���|�[�^�[�̂ق�����ɐ錾���Ă������Ɓi��ɍ\�z����A��ɔj������邽�߁j�B
	 *	@param pReport ���|�[�g�B
	 *	@param pListener ���X�i�[�B
	 */
	void SetEnvironment(IReport* pReport, IListener* pListener)
	{
		Purge();
		m_in.m_pListener = pListener;
		m_in.m_pReport = pReport;
		if ( m_in.m_pReport != NULL )
		{
			m_in.m_pReport->SetListener(&m_in);
		}
	}

	/// [�ݒ�] �����Z�b�g
	void ResetEnvironment(void)
	{
		if ( m_in.m_pReport != NULL )
		{
			m_in.m_pReport->SetListener(NULL);
			m_in.m_pListener = NULL;
			m_in.m_pReport = NULL;
		}
	}

	/**
	 * [�ݒ�] �L���[�L�����Ԑݒ�.
	 *		�L���[�i�[��A��莞�Ԏ�M���Ȃ��ꍇ�A�L���[���e��j�����邱�Ƃ��o���܂��B
	 *	@note SetTerminalTime() �Ɠ����Ɏw��ł��܂���B
	 *	@param time �L���[�̗L������(ms)�B�Ō�Ɏ�M���ĉ�͂��Ă���w�莞�Ԍo�߂�����A�L���[�̐擪�̈�o�C�g��j�����āA
	 *					�ēx��͂����܂��B�ȗ����͖���( DWORD_MAX )�ł��B
	 */
	void SetAliveTime(DWORD time = DWORD_MAX)
	{
		m_in.m_aliveTime = time;
		m_in.m_tearminalTime = DWORD_MAX;
	}

	/**
	 * [�ݒ�] �I�[���Ԑݒ�.
	 *		�L���[�i�[��A��莞�Ԏ�M���Ȃ��ꍇ�ɃL���[���e����͂���悤�ɂ��܂��B
	 *	@note SetAliveTime() �Ɠ����Ɏw��ł��܂���B
	 *	@param time �I�[����(ms)�B
	 */
	void SetTearminalTime(DWORD time = DWORD_MAX)
	{
		m_in.m_tearminalTime = time;
		m_in.m_aliveTime = DWORD_MAX;
	}

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		�e�T�u�N���X�̊J�n���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	bool IsAlive(void) const
	{
		if ( m_in.m_pReport != NULL )
		{
			return m_in.m_pReport->IsAlive();
		}
		return false;
	}	

	/**
	 * [�m�F] �ڑ��`�F�b�N
	 *	@retval true	�ڑ����B
	 *	@retval	false	�ؒf�B
	 */
	bool IsConnect(void) const
	{
		if ( m_in.m_pReport != NULL )
		{
			return m_in.m_pReport->IsConnect();
		}
		return false;
	}

	/**
	 * [���M] �f�[�^���M.
	 *	@note �P�Ƀ��|�[�^�ɑ��M�˗����邾���ł��B
	 *	@param dataSize ���M����f�[�^���B
	 *	@param lpcData ���M����f�[�^�B
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	bool Send(size_t dataSize, LPCVOID lpcData)
	{
		if ( m_in.m_pReport != NULL )
		{
			return m_in.m_pReport->Send(dataSize, lpcData);
		}
		return false;
	}

	/**
	 * [����] �L���[�o�b�t�@�N���A.
	 *	@note �L���[�ɂ��܂��Ă���f�[�^���N���A���܂��B
	 */
	void Purge(void)
	{
		m_in.m_queueData.RemoveAll();	//RAW���̂ݍ폜
	}

	/**
	 * [�擾] �L���[�󂫃T�C�Y�擾.
	 *	@return �󂫃T�C�Y
	 */
	size_t GetQueueFreeSize(void) const
	{
		return m_in.m_queueData.GetFreeSize();
	}

	/**
	 * [�擾] ���|�[�^�擾
	 *	@return ���|�[�^
	 */
	IReport* ReferReport(void)
	{
		return m_in.m_pReport;
	}	

	/**
	 * [�擾] ���|�[�^�擾
	 *	@return ���|�[�^
	 */
	const IReport* ReferReport(void) const
	{
		return m_in.m_pReport;
	}	

private:
	/// �����p�N���X
	class CInner : public IReport::IListener
	{
		DEFPARENTLISTENER(CQueueingReporter, IParentListener);
	public:
		/// �R���X�g���N�^
		CInner(int queueSize = 10000) 
			: m_queueData(queueSize), m_pListener(NULL), m_pReport(NULL), m_aliveTime(DWORD_MAX), m_tearminalTime(DWORD_MAX)
		{
		}
		/// [�ʒm] �C�x���g����
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			if ( m_pListener == NULL )
			{
				return true; 
			}
			if ( ev.HasData() )
			{
				EXCLUSIVE( &m_queueData );
				//== �f�[�^����
				size_t size = ev.GetDataSize();
				const BYTE* B = ev.GetData();
				while ( size > 0 )
				{
					size_t freeSize = m_queueData.GetFreeSize();
					if ( freeSize == 0 )
					{
						ASSERT0( false, "CQueueingReporter", "�L���[�����ӂ�܂����B" );
						break;
					}
					size_t l = size;
					if ( l > freeSize )
					{
						l = freeSize;
					}
					m_queueData.AddElements(l, B);
					if ( m_tearminalTime == DWORD_MAX )
					{
						m_OnDeseri(false);
					}
					else
					{
						m_pReport->SetIdleEventMode(m_tearminalTime);
					}
					B += l;
					size -= l;
				}
				return true;
			}
			//== �C�x���g����
			switch ( ev.GetEvent() )
			{
			case ReportEvent_Disconnect:
				m_OnDeseri(true);
				m_queueData.RemoveAll();	//RAW���̂ݍ폜
				break;
			case ReportEvent_Idle:
				if ( m_aliveTime != DWORD_MAX )
				{
					EXCLUSIVE( &m_queueData );
					BYTE b;
					if ( m_queueData.TakeElements(1, &b) == 1 )
					{
						m_pListener->OnReportDustyData(b);
					}
					m_pReport->SetIdleEventMode(0);
					m_OnDeseri(false);
				}
				if ( m_tearminalTime != DWORD_MAX )
				{
					m_OnDeseri(false);
					m_pReport->SetIdleEventMode(0);
				}
				break;
			default:
				break;
			}
			return m_pListener->OnReportEvent(ev);
		}

		CRingQueueT<BYTE>	m_queueData;	///< ��M���f�[�^
		IParentListener*	m_pListener;	///< ���X�i�[
		IReport*			m_pReport;		///< �o�^���� IReport
		DWORD				m_aliveTime;	///< �L���[�P�f�[�^�L������(ms). @note �w�莞�Ԃ��߂���ƃL���[���N���A���܂��B
		DWORD				m_tearminalTime;///< �I�[����(ms). @note �w�莞�ԃf�[�^�Ȃ��ƏI�[�Ƃ��āA�R�}���h����͂��܂��B

	protected:
		/// �����Ă���L���[����͂����� 
		void m_OnDeseri(bool boIsLast)
		{
			while ( ! m_queueData.IsEmpty() )
			{
				size_t r = m_pListener->OnReportData(boIsLast, m_queueData);
				if ( r == 0 )
				{
					if ( boIsLast )
					{
						m_queueData.RemoveAll();
					}
					else
					{
						if ( m_aliveTime != DWORD_MAX )
						{
							m_pReport->SetIdleEventMode(m_aliveTime);
						}
					}
					return;
				}
				m_queueData.EraseElements(r);
			}
		}
	};
	CInner		m_in;	///< �������Ǘ�
};



}; // TNB

