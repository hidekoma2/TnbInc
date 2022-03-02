#pragma once
/**
 *@file
 * �_�C�A���A�b�v�Ď��֌W�̃w�b�_
 *
 *		�w��̃_�C�A���A�b�v�̊Ď����s���܂��B
 *		�ڑ����ԁA����MByte�𓾂邱�Ƃ��ł��܂��B
 *
 *	@note Windows �� RAS�|API ���g�p���Ă��܂��B
 *	@note Windows �� IPHELPER�|API ���g�p���Ă��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDialupManager.h"
#include "TnbTime.h"
#include "TnbIcmp.h"
#include "TnbIpHelper.h"



//TNB Library
namespace TNB
{



/**@ingroup RAS
 * �_�C�A���A�b�v�ڑ��Ď��N���X
 *
 *		�N���X��ɂ��A��̃_�C�A���A�b�v�G���g�����Ď��ł��܂��B
 *
 *	@note �{�N���X��DialUp�̋@�\�͗L��܂���
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@todo	����M���A�ڑ����Ԃ�DWORD�ŊǗ����Ă��邽�߁A4G�ŃI�[�o�[�t���[���܂��B
 *			�΍�͉\���H
 *
 *	@par�K�v�t�@�C��
 *			TnbDialupWatcher.h
 *
 *	@date 06/04/01�G�V�K�쐬�B
 *	@date 06/06/29�G���X�i�[�ǉ��B 
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 */
class CDialupWatcher : TNB::CThread::IRunner , CCopyImpossible
{
	typedef CDialupManager __manage;		///< �Ǘ��N���X���Ē�`
	enum
	{
		E_CheckInterval = 500		///< �`�F�b�N�Ԋu
	};
public:

	/**@ingroup RAS
	 * �_�C�A���A�b�v�ڑ���ԏ��^
	 *
	 *		���Ԃ�T�C�Y�Ȃǂ��ꊇ�Ǘ����܂� 
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbDialupWatcher.h
	 *
	 *	@date 06/04/01�G�V�K�쐬�B
	 */
	struct TConnectInfo
	{
		/** �ڑ����Ԃ̏��
		 *	true�Ȃ�������i�Ď��J�n���ڑ�����Ă������߁j
		 *	false�Ȃ琳�����B
		 */
		bool		boIsDoubtfulConnectSpan;
		CSystemTime	timeConnectTime;			///< �ڑ��J�n���� 
		DWORD		dwConnectingSpan;			///< �ڑ��p������ ms						
		bool		boIsValidIfRow;				///< �ڑ����� true;�L��/false;����	
		MIB_IFROW	tMibIfRow;					///< �ڑ����							
		/** [�ݒ�] ������ */
		void Init(void)
		{
			boIsValidIfRow			= false;
			boIsDoubtfulConnectSpan	= true;
			timeConnectTime			= CSystemTime::GetCurrent();
			dwConnectingSpan		= 0;
		}
		/** [�擾] �J�n���Ԏ擾 @return �J�n���� */
		CSystemTime GetStartTime(void)
		{
			return timeConnectTime;
		}
		/** [�擾] �I�����Ԏ擾 @return �I������ */
		CSystemTime GetEndTime(void)
		{
			return timeConnectTime.AddMillSec(dwConnectingSpan);
		}
	};

	/**@ingroup RAS
	 * �_�C�A���A�b�v�ڑ��Ď��̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbDialupWatcher.h
	 *
	 *	@date 06/06/29�G�V�K�쐬�B
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �ڑ�
		 *	@param boIsDoubtfulConnectSpan �ڑ����Ԃ̏�ԁB
		 *				true�Ȃ�������i�Ď��J�n���ڑ�����Ă������߁j
		 *				false�Ȃ琳�����B
		 */
		virtual void OnDialConnect(bool boIsDoubtfulConnectSpan) = 0;

		/**
		 * [�ʒm] �ؒf
		 *	@param t �ڑ����̓��e�B
		 */
		virtual void OnDialDisconnect(const TConnectInfo& t) = 0;
	};

protected:

	CThread			m_threadWatch;		///< �X���b�h�Ǘ�			
	CStr			m_strPhoneBook;		///< �d�b����				
	CStr			m_strEntryName;		///< �G���g����				
	DWORD			m_dwConnectedTick;	///< �ڑ��������̎���		
	HRASCONN		m_hRasConn;			///< �ڑ����̃n���h��		
	INDEX			m_mibIndex;			///< �ڑ�����MIB�C���f�b�N�X
	CByteVector		m_vbIfTableTmp;		///< GetIfTable�p�e���|����	
	TConnectInfo	m_tInfo;			///< �ڑ����				
	int				m_iCheckInterval;	///< ��ԃ`�F�b�N�̊Ԋu ms	
	IListener*		m_piListener;		///< ���X�i�[		

	/** 
	 * �X���b�h�����{��
	 *	@retval 0�i�Œ�j�B
	 */
	virtual DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			//==�ڑ��Ď����[�v
			if ( m_hRasConn == NULL )
			{
				while ( IsRunnable() )
				{
					m_hRasConn = __manage::GetRasConn(m_strPhoneBook, m_strEntryName);
					if ( m_hRasConn != NULL )
					{
						//�ڑ����ꂽ
						TRACE1("CDialupWatcher::�ڑ����m[%s]�I\n", m_strEntryName);
						m_tInfo.boIsDoubtfulConnectSpan = false;
						if ( m_piListener != NULL )
						{
							m_piListener->OnDialConnect(false);
						}
						break;
					}
					::Sleep(m_iCheckInterval);
				}
				if ( ! IsRunnable() )
				{
					break;
				}
			}
			//
			m_dwConnectedTick			= ::GetTickCount();
			m_tInfo.timeConnectTime		= CSystemTime::GetCurrent();
			m_tInfo.dwConnectingSpan	= 0;
			CAscii strAddress				= __manage::GetIpAddress(m_hRasConn);
			ASSERTLIB(! strAddress.IsEmpty());
			m_mibIndex = CIpHelper::GetMibIndex(strAddress);
			CIpHelper::GetMibIfRow(m_mibIndex, m_tInfo.tMibIfRow);
			m_tInfo.boIsValidIfRow = true;
			//
			//==�ؒf�Ď����[�v
			RASCONNSTATUS tStatus;
			while ( IsRunnable() )
			{
				::Sleep(m_iCheckInterval);
				CIpHelper::GetMibIfRow(m_mibIndex, m_tInfo.tMibIfRow);
				m_tInfo.dwConnectingSpan = ::GetTickCount() - m_dwConnectedTick;
				tStatus.dwSize = sizeof(RASCONNSTATUS);
				DWORD dwRc = ::RasGetConnectStatus(m_hRasConn, &tStatus);
				if ( dwRc != 0 || tStatus.rasconnstate == RASCS_Disconnected )
				{
					//�ؒf���ꂽ
					m_hRasConn = NULL;
					TRACE1("CDialupWatcher::�ؒf���m[%s]�I\n", m_strEntryName);
					TRACE1("   �ڑ����� = %d�b\n", m_tInfo.dwConnectingSpan / 1000);
					TRACE1("             (%d��)\n", m_tInfo.dwConnectingSpan / 1000 / 60);
					TRACE1("   ��MByte = %dbyte\n", m_tInfo.tMibIfRow.dwInOctets);
					TRACE1("   ���MByte = %dbyte\n", m_tInfo.tMibIfRow.dwOutOctets);
					if ( m_piListener != NULL )
					{
						m_piListener->OnDialDisconnect(m_tInfo);
					}
					break;
				}
			}
		}
		m_tInfo.Init();
		return 0;
	}

public:

	/// �R���X�g���N�^
	CDialupWatcher(void) :m_iCheckInterval(E_CheckInterval), m_piListener(NULL), m_hRasConn(NULL)
	{
		m_tInfo.Init();
		m_threadWatch.SetRunner(this);
	}

	/// �f�X�g���N�^
	virtual ~CDialupWatcher(void)
	{
		Stop();
	}

	/**
	 * [�o�^] Listner�o�^
	 *	@note �o�^�ł��� CDialupWatcher::IListener �͈�����ł��B
	 *	@param I ���X�i�[�B�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	void SetListener(IListener* I = NULL)
	{
		m_piListener = I;
	}

	/**
	 * [�m�F] �ڑ��`�F�b�N
	 *	@retval true	�ڑ����B
	 *	@retval false	�Ď��X�^�[�g���Ă��Ȃ� or�ڑ����Ă��Ȃ��B
	 */
	bool IsConnect(void)
	{
		return m_hRasConn != NULL;
	}

	/**
	 * [�ݒ�] �ڑ����̏�����.
	 *		�ʏ�ؒf���Ɏg�p���܂��B
	 */
	void ClearConnectInfo(void)
	{
		m_tInfo.Init();
	}

	/**
	 * [�Q��] �ڑ����Q��
	 *	@return �ڑ����͌��݂̏��B�Ⴄ�ꍇ�A�Ō�̐ؒf���̏�Ԃ̏��B
	 */
	const TConnectInfo& GetConnectInfo(void)
	{
		if ( IsConnect() )
		{	
			CIpHelper::GetMibIfRow(m_mibIndex, m_tInfo.tMibIfRow);
			m_tInfo.dwConnectingSpan = ::GetTickCount() - m_dwConnectedTick;
		}
		return m_tInfo;
	}

	/**
	 * [�Q��] �ڑ����ԎQ��
	 *	@return �ڑ����͌��݂̏��B�Ⴄ�ꍇ�A�Ō�̐ؒf���̏�Ԃ̏��B
	 */
	DWORD GetConnectSpan(void)
	{
		return GetConnectInfo().dwConnectingSpan;
	}

	/**
	 * [����] �Ď���~.
	 *		�Ď����~���܂��B
	 */
	void Stop(void)
	{
		m_threadWatch.Stop();
		m_strEntryName.Empty();
		m_strPhoneBook.Empty();
		m_hRasConn = NULL;
		m_tInfo.Init();
	}

	/**
	 * [����] �Ď��J�n.
	 *		�w��̓��e�ŊĎ����܂��B
	 *	@param lpszPhoneBook �d�b�����BNULL�w��\�B
	 *	@param lpszEntryName �G���g����
	 *	@param iCheckInterval �Ď��Ԋu�B�ȗ������500ms�B
	 *	@retval true �Ď��J�n
	 *	@retval false ���s�i�G���g���[�����s���̏ꍇ�j
	 */
	bool Start(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryName, int iCheckInterval = E_CheckInterval)
	{
		if ( m_threadWatch.IsAlive() )
		{
			return false;
		}
		__manage::TDialParams tParam = __manage::GetDialParams(lpszPhoneBook, lpszEntryName);
		if ( tParam.IsInvalid() )
		{
			return false;
		}
		m_strPhoneBook = lpszPhoneBook;
		m_strEntryName = lpszEntryName;
		m_tInfo.boIsValidIfRow = false;
		m_hRasConn = __manage::GetRasConn(lpszPhoneBook, lpszEntryName);
		m_tInfo.boIsDoubtfulConnectSpan = (m_hRasConn != NULL);
		if ( m_hRasConn != NULL )
		{
			//�ڑ����ꂽ
			TRACE1("CDialupWatcher::���łɐڑ������m[%s]�I\n", m_strEntryName);
			if ( m_piListener != NULL )
			{
				m_piListener->OnDialConnect(true);
			}
		}
		m_iCheckInterval = iCheckInterval;
		#ifdef _DEBUG
			m_threadWatch.Start(CStr::Fmt(_T("<%s>�Ď�"), lpszEntryName));
		#else
			m_threadWatch.Start();
		#endif
		return true;
	}

	/**
	 * [�Q��] �Ď��X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadWatch;
	}
};



}; // TNB
