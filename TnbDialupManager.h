#pragma once
/**
 *@file
 * �_�C�A���A�b�v�֌W�̃w�b�_
 *
 *		�_�C�A���A�b�v��A�ؒf�܂ł̊Ď����s�����Ƃ��ł��܂��B
 *		�_�C�A���A�b�v�̃A�^�b�`���ł��܂��B
 *		�G���g���̈ꗗ���擾�ł��܂��B
 *
 *	@note Windows�� RAS�|API ���g�p���Ă��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"
#include "TnbStrEx.h"
#include "TnbMap.h"
#include "TnbThread.h"
#include "TnbStaticMap.h"
#include <Ras.h>
#include <RasError.h>
#ifndef _WIN32_WCE
 #include "TnbRegistryAccessor.h"
 #pragma comment(lib,"RasApi32.lib")
 #if ( WINVER >= 0x500 )
   #ifndef RASEAPINFO
	#error 
	// WINVER 0x0500 �ȏ�̏ꍇ�AVC6++ �t���� RAS.h �͎g�p�ł��܂���i�o�O�j
	// SDK ��RAS.h ���C���N���[�h����悤�ɂ��Ă��������B
	//�i��߼�݁��ިڸ�؂� SDK�̃w�b�_�̂ق�����ɂȂ�悤�ɂ���A�Ȃǁj�B
   #endif
 #endif
#endif



//TNB Library
namespace TNB
{



/// �{�w�b�_��p TRACE �}�N��
#define _DIALUP_TRACE(FUNC, VAL) \
	if ( (VAL) != 0 ) { TRACE3("%s failed: Error %u[%s]\n", _T(#FUNC), VAL, GetErrorString(VAL)); }



/**@ingroup RAS
 * �_�C�A���A�b�v�Ǘ��N���X
 *
 *		�C���X�^���X��ɂ��A��̃_�C�A���A�b�v���Ǘ��A�Ď����邱�Ƃ��ł��܂��B
 *		���łɐڑ����Ă���_�C�A���A�b�v���A�^�b�`���邱�Ƃ��\�ł��B
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbDialupManager.h
 *
 *	@date 06/04/01	�V�K�쐬
 *	@date 08/07/08	CE�ł̒���
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 */
class CDialupManager : TNB::CThread::IRunner, CCopyImpossible
{
public:

	/**@ingroup RAS
	 * �_�C�A���A�b�v�̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		CDialupManager �̒ʒm�p�C���^�[�t�F�[�X�ł��B
	 *
	 *	@see CDialupManager::Attach()
	 *	@see CDialupManager::Dial()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbDialupManager.h
	 *
	 *	@date 06/04/01	�V�K�쐬
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/** 
		 * [�ʒm] �G���[����.
		 *	@note �������M������A�ڑ��Ɏ��s/�ؒf�ɂȂ��Ă��܂��B�Ȍ�A���X�i�[�ɕ񍐂͗L��܂���B
		 *	@param dwError �G���[�R�[�h�B
		 */
		virtual void OnDialupError(DWORD dwError) = 0;

		/** 
		 * [�ʒm] ��ԕω�����
		 *	@param eState ��ԃR�[�h�B									<BR>
		 *			�uRASCS_Connected�v����M������ڑ������ł��B		<BR>
		 *			�uRASCS_Disconnected�v����M������ؒf�ł��B�Ȍナ�X�i�[�ɕ񍐂͗L��܂���B<BR>
		 *			����ȊO�͏�ԕω��̒ʒm�ɂȂ�܂��B				<BR>
		 */
		virtual void OnDialupChangeState(RASCONNSTATE eState) = 0;

	private:
		friend class CDialupManager;		///< �t�����h�N���X�錾
	};

private:

	CThread				m_threadWatch;	///< �X���b�h�Ǘ�
	IListener*			m_piListener;	///< ���X�i�[
	HRASCONN			m_hRasConn;		///< RAS�n���h��
	CSyncSection		m_syncFunc;		///< �r��
	enum
	{
		E_ConnectCheckInterval = 100,	///< �ڑ����̃`�F�b�N�Ԋu
		E_DisconnectCheckInterval = 500,///< �ؒf�m�F�̃`�F�b�N�Ԋu
	};

	/** 
	 *	RASCONN�\����
	 *
	 *		 RASCONN��Ver4.01�ȍ~�̌^��p�ӂ��Ă��܂� 
	 */
	struct TRasConn
	{
		DWORD		dwSize;									///< sizeof(TRasConn)
		HRASCONN	hRasConn;								///< RAS�̃n���h��
		TCHAR		szEntryName[RAS_MaxEntryName + 1];		///< �G���g����
		#ifndef _WIN32_WCE
		TCHAR		szDeviceType[RAS_MaxDeviceType + 1];	///< �f�o�C�X��
		TCHAR		szDeviceName[RAS_MaxDeviceName + 1];	///< �f�o�C�X��
		TCHAR		szPhonebook[MAX_PATH];					///< �d�b����
		DWORD		dwSubEntry;								///< �T�u�G���g��
		#endif
		/**
		 * RASCONN�̃|�C���^�擾
		 *	@return RASCONN�̃|�C���^
		 */
		operator LPRASCONN(){ return reinterpret_cast<LPRASCONN>(this); }
		/**
		 * �d�b����r
		 *	@param s �d�b��
		 *	@retval true �����d�b���B
		 *	@retval false �قȂ�B
		 */
		bool IsPhoneBookEqual(CStr& s) const
		{
			#ifndef _WIN32_WCE
				return (s.IsEmpty() || STRLIB::Compare(szPhonebook, s) == 0);
			#else
				return s.IsEmpty();
			#endif
		}

	};

	/// TRasConn�̔z��錾
	typedef CVectorT<TRasConn> CRasConnVector;

	/**
	 * �ڑ�����TRasConn�S���擾
	 *	@return TRasConn�̔z��B�����Ȃ�API�G���[�����B
	 */
	static CRasConnVector ms_GetConnectingEnties(void)
	{
		CRasConnVector aRasConn;
		CByteVector ba;
		TRasConn* P = reinterpret_cast<TRasConn*>(ba.GetBuffer(sizeof(TRasConn)));
		P->dwSize = sizeof(TRasConn);
		DWORD dwBufferSize = sizeof(TRasConn);
		DWORD dwEntries = 0;
		DWORD dwRc = ::RasEnumConnections(*P, &dwBufferSize, &dwEntries);
		if ( dwRc == ERROR_BUFFER_TOO_SMALL )
		{
			P = reinterpret_cast<TRasConn*>(ba.GetBuffer(dwBufferSize));
			P->dwSize = sizeof(TRasConn);
			dwRc = ::RasEnumConnections(*P, &dwBufferSize, &dwEntries);
		}
		if ( dwRc != ERROR_SUCCESS )
		{
			_DIALUP_TRACE(RasEnumConnections, dwRc);
			aRasConn.Invalid();
			return aRasConn;
		}
		else
		{
			loop ( i, dwEntries )
			{
				aRasConn.Add(P[i]);
			}
		}
		return aRasConn;
	}

	/// �n���h���ƃN���X�̃}�b�v
	typedef CMapT<HRASCONN, CDialupManager*> CHndThisMap;

	/**
	 * �n���h���̃N���A.
	 *		�����o�� m_hRasConn���N���A���A�}�b�v����폜���܂��B
	 */
	void m_ClearHandleRasConn(void)
	{
		if ( m_hRasConn != NULL )
		{
			ms_ElementMap().RemoveKey(m_hRasConn);
			m_hRasConn = NULL;
		}
	}

	/**
	 * �n���h���ƃN���X�̃}�b�v�Q��
	 *	@return �}�b�v�̎Q��
	 */
	static CHndThisMap& ms_ElementMap(void)
	{
		static CHndThisMap s_mapHndThis;
		return s_mapHndThis;
	}

	/**
	 * RasDial()�̃R�[���o�b�N.
	 *		�G���[���������܂��B
	 *	@param h			��������RAS�̃n���h��
	 *	@param uMsg			���������C�x���g�̃^�C�v
	 *	@param state		���ꂩ����낤�Ƃ��Ă���ڑ����
	 *	@param dwError		���������G���[�R�[�h
	 *	@param dwExError	���������G���[�̊g���R�[�h)
	 */
	static VOID WINAPI 
	ms_RasDialFunc1(HRASCONN h, UINT uMsg, RASCONNSTATE state, DWORD dwError, DWORD dwExError)
	{
		if ( dwError != 0 )
		{
			TRACE3("RasDialFunc s=%d e=%u[%s]\n", state, dwError, GetErrorString(dwError));
			CDialupManager* P;
			if ( ms_ElementMap().Lookup(h, P) != 0 )
			{
				//����
				if ( (reinterpret_cast<ULONG_PTR>(P) & 0x80000000) == 0 )
				{
					if ( P->m_piListener != NULL )
					{
						if ( P->m_threadWatch.IsAlive() )
						{
							P->m_piListener->OnDialupError(dwError);
						}
					}
					P->HangUp();
				}
			}
			else
			{
				ms_ElementMap()[h] = reinterpret_cast<CDialupManager*>(static_cast<size_t>(dwError | 0x80000000));
			}
		}
	}

	/** 
	 * �X���b�h�����{��
	 *	@retval 0�i�Œ�j�B
	 */
	virtual DWORD Run(void)
	{
		DWORD dwRc;
		RASCONNSTATUS tStatus;
		RASCONNSTATE eNowState = static_cast<RASCONNSTATE>(0xFFFF);
		//
		m_piListener->OnDialupChangeState(RASCS_OpenPort);
		//
		bool boIsFirst = true;
		while ( IsRunnable() )
		{
			::Sleep(E_ConnectCheckInterval);
			dwRc = GetConnectStatus(tStatus);
			if ( dwRc != NULL )
			{
				//�G���[������ms_RasDialFunc1()�ōs���Ă��܂��B
				if ( dwRc == ERROR_INVALID_HANDLE )
				{
					::Sleep(500);
					continue;
				}
				break;
			}
			if ( tStatus.dwError != 0 )
			{
				//�G���[������ms_RasDialFunc1()�ōs���Ă��܂��B
				break;
			}
			//
			if ( boIsFirst )
			{
				boIsFirst = false;
				eNowState = tStatus.rasconnstate;
				TRACE1("ChangeState =[%s]\n", StateToString(eNowState));
				m_piListener->OnDialupChangeState(eNowState);
			}
			else if ( eNowState != tStatus.rasconnstate )
			{
				eNowState = tStatus.rasconnstate;
				TRACE1("ChangeState =[%s]\n", StateToString(eNowState));
				m_piListener->OnDialupChangeState(eNowState);
			}
			//
			if ( eNowState == RASCS_Connected )
			{
				while ( IsRunnable() )
				{
					dwRc = GetConnectStatus(tStatus);
					if ( dwRc != NULL || tStatus.rasconnstate == RASCS_Disconnected )
					{
						//�ؒf���ꂽ
						TRACE1("ChangeState =[%s]\n", StateToString(tStatus.rasconnstate));
						m_ClearHandleRasConn();
						m_piListener->OnDialupChangeState(RASCS_Disconnected);
						break;
					}
					::Sleep(E_DisconnectCheckInterval);
				}
				break;
			}
			if ( eNowState == RASCS_Disconnected )
			{
				m_ClearHandleRasConn();
				break;
			}
		}
		return 0;
	}

	/**
	 * ������T�C�Y�O�̃|�C���^��NULL�ɂ���
	 *	@param[in,out] _lpszPhoneBook �d�b�����̃|�C���^
	 */
	static void ms_CheckPhoneBook(LPCTSTR& _lpszPhoneBook)
	{
		if ( _lpszPhoneBook != NULL && _lpszPhoneBook[0] == '\0' )
		{
			_lpszPhoneBook = NULL;
		}
	}

	#ifndef _WIN32_WCE
		static LPCTSTR NcStr(LPCTSTR lpsz) { return lpsz; } 
	#else
		class NcStr
		{
			CStr m_str;
			bool m_isNull;
		public:
			NcStr(LPCTSTR lpsz) : m_str(lpsz), m_isNull(lpsz == NULL) {}
			operator LPTSTR(void) { return m_isNull ? NULL : m_str.GetBuffer(256); }
		};
	#endif //_WIN32_WCE

public:

	/**@ingroup RAS
	 * �_�C�A���p�����[�^�^
	 *
	 *		�_�C�A�����邽�߂̏����Ǘ����Ă܂�
	 */
	struct TDialParams
	{
		CStr strPhoneBook;		///< �d�b��
		CStr strEntryName;		///< �G���g����
		CStr strUserName;		///< ���[�U��
		CStr strPassword;		///< �p�X���[�h
		CStr strPhoneNumber;	///< �d�b�ԍ�
		CStr strDeviceName;		///< �f�o�C�X��
		#ifndef _WIN32_WCE
		/**
		 * �d�b�����擾
		 *	@return �d�b���B���ݒ�Ȃ�NULL�B
		 */
		LPCTSTR GetPhoneBook(void) const
		{
			if ( strPhoneBook.IsEmpty() )
			{
				return NULL;
			}
			return strPhoneBook;
		}
		#else
		LPWSTR GetPhoneBook(void) const { return NULL; }
		#endif
		/**
		 *	�{���͗L�����ۂ�
		 *	@retval true	�L���B
		 *	@retval false	�����B
		 */
		bool IsInvalid(void) const
		{
			return strEntryName.IsEmpty();
		}
	};

	/**
	 * �R���X�g���N�^ 
	 */
	CDialupManager(void) : m_piListener(NULL), m_hRasConn(NULL)
	{
		m_threadWatch.SetRunner(this);
	}

	/**
	 * �f�X�g���N�^ 
	 *	@note �A�^�b�`���Ă��Ă� HangUp() ���R�[�����Ȃ����莩���I�ɐؒf���邱�Ƃ͂���܂���B
	 */
	virtual ~CDialupManager(void)
	{
		m_ClearHandleRasConn();
	}

	/**
	 * [�m�F] �A�^�b�`���Ă��邩.
	 *	@note Dial�����ꍇ���A�^�b�`��ԂɂȂ�܂��B
	 *	@retval true �A�^�b�`���Ă���
	 *	@retval false �A�^�b�`���Ă��Ȃ�
	 */
	bool IsAttach(void) const
	{
		return m_hRasConn != NULL;
	}

	/**
	 * [�ݒ�] �A�^�b�`.
	 *		�A�^�b�`��A�ڑ����Ď����܂��B
	 *	@note �w��̃G���g�������ڑ�����Ă��邩�̃`�F�b�N�ɂ��Ȃ�܂��B
	 *	@param lpszPhoneBook �d�b�����BNULL�w��\�B
	 *	@param lpszEntryName �G���g�����B
	 *	@param piDialupListener ���X�i�[�BNULL���w�肷��Ɛؒf���Ď����܂���B
	 *	@retval true	�A�^�b�`����
	 *	@retval false	���s�i�R�l�N�g����Ă��Ȃ�/�{�C���X�^���X��������/���̃v���Z�X�Őڑ����j
	 */
	bool Attach(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryName, IListener* piDialupListener = NULL)
	{
		EXCLUSIVE(&m_syncFunc);
		ms_CheckPhoneBook(lpszPhoneBook);
		Detach();
		HRASCONN hRasConn = GetRasConn(lpszPhoneBook, lpszEntryName);
		if ( hRasConn != NULL )
		{
			//����
			m_hRasConn = hRasConn;
			ms_ElementMap()[m_hRasConn] = this;
			m_piListener = piDialupListener;
			if ( piDialupListener != NULL )
			{
				#ifdef _DEBUG
					m_threadWatch.Start(CStr(lpszEntryName) + _T("Dialup"));
				#else
					m_threadWatch.Start();
				#endif
			}
			return true;
		}
		return false;
	}

	/**
	 * [����] �_�C�A��.
	 *		�ڑ���A�ڑ����Ď����܂��B
	 *	@note ���ɂȂ����Ă���ꍇ�A�p�����[�^���قȂ��Ă��Ă��ؒf���܂���B
	 *	@note �p�����[�^�̃f�o�C�X���́AEnumDeviceNames()�œ���ꂽ���O���A
	 *			Empty���w�肵�Ă�������(Empty�̏ꍇDevice�̓f�t�H���g�̂܂�)�B
	 *	@param D �_�C�A���p�����[�^
	 *	@param piDialupListener ���X�i�[�BNULL���w�肷��Ɛڑ���������܂Ń��^�[�����܂���B�܂��ؒf���Ď����܂���B
	 *	@retval 0		����(�񓯊��̏ꍇ�A��Ŏ��s���ʒm����邱�Ƃ���)
	 *	@retval 0�ȊO	�G���[�B GetErrorString()�ŕ����񉻂ł��܂��B
	 */
	DWORD Dial(const TDialParams& D, IListener* piDialupListener = NULL)
	{
		EXCLUSIVE(&m_syncFunc);
		if ( D.IsInvalid() )
		{
			TRACE0("Dial �p�����[�^�ُ�\n");
			return ERROR_INVALID_PARAMETER;
		}
		//==�A�^�b�`���Ă�����f�^�b�`���Ă���
		Detach();
		//==�f�o�C�X�w��
		DWORD dwSize = 0;
		::RasGetEntryProperties(D.GetPhoneBook(), NcStr(D.strEntryName), NULL, &dwSize, NULL, NULL);
		CByteVector ba;
		LPRASENTRY ptEntry = reinterpret_cast<LPRASENTRY>(ba.GetBuffer(dwSize));
		ptEntry->dwSize = dwSize;
		DWORD dwRc = ::RasGetEntryProperties(D.GetPhoneBook(), NcStr(D.strEntryName), ptEntry, &dwSize, NULL, NULL);
		if ( dwRc != 0 )
		{
			_DIALUP_TRACE(RasGetEntryProperties, dwRc);
			return dwRc;
		}
		if ( ! D.strDeviceName.IsEmpty() )
		{
			if ( D.strDeviceName.Compare(ptEntry->szDeviceName) != 0 )
			{
				STRLIB::Copy(ptEntry->szDeviceName, D.strDeviceName);
				dwRc = ::RasSetEntryProperties(D.GetPhoneBook(), NcStr(D.strEntryName), ptEntry, dwSize, NULL, NULL);
				if ( dwRc != 0 )
				{
					_DIALUP_TRACE(RasSetEntryProperties, dwRc);
					return dwRc;
				}
			}
		}
		//
		RASDIALPARAMS t;
		t.dwSize = sizeof(RASDIALPARAMS);
		STRLIB::Copy(t.szEntryName, D.strEntryName);
		STRLIB::Copy(t.szUserName, D.strUserName);
		STRLIB::Copy(t.szPassword, D.strPassword);
		STRLIB::Copy(t.szPhoneNumber, D.strPhoneNumber);
		t.szCallbackNumber[0] = '\0';
		t.szDomain[0] = '\0';
		//
		HRASCONN hRasConn = NULL;
		if ( piDialupListener == NULL )
		{
			//===����
			dwRc = ::RasDial(NULL, D.GetPhoneBook(), &t, 0, NULL, &hRasConn);
		}
		else
		{
			//===�񓯊�
			dwRc = ::RasDial(NULL, D.GetPhoneBook(), &t, 1, ms_RasDialFunc1, &hRasConn);
		}
		if ( dwRc != 0 )
		{
			_DIALUP_TRACE(RasDial, dwRc);
			return dwRc;
		}
		//
		dwRc = ::RasSetEntryDialParams(D.GetPhoneBook(), &t, false);
		if ( dwRc != 0 )
		{
			_DIALUP_TRACE(RasSetEntryDialParams, dwRc);
			HangUp();
			return dwRc;
		}
		//
		m_hRasConn = hRasConn;
		//
		CDialupManager* pMan;
		if ( ms_ElementMap().Lookup(hRasConn, pMan) != 0 )
		{
			DWORD dwError = static_cast<DWORD>(reinterpret_cast<size_t>(pMan));
			ASSERTLIB((dwError & 0x80000000) != 0);
			dwError &= 0x7FFFFFFF;
			_DIALUP_TRACE(RasDial, dwError);
			HangUp();
			return dwError;
		}
		//
		ms_ElementMap()[hRasConn] = this;
		if ( piDialupListener != NULL )
		{
			m_piListener = piDialupListener;
			#ifdef _DEBUG
				m_threadWatch.Start(D.strEntryName + _T("Dialup"));
			#else
				m_threadWatch.Start();
			#endif
		}
		return 0;
	}

	/**
	 * [�ݒ�] �f�^�b�`.
	 *	@note Attach/Dial���Ă��Ȃ��Ƃ��R�[�����Ă������x������܂���i�������܂���j
	 */
	void Detach(void)
	{
		EXCLUSIVE(&m_syncFunc);
		if ( m_hRasConn == NULL )
		{
			return;
		}
		m_threadWatch.Stop();
		m_piListener = NULL;
		m_ClearHandleRasConn();
		return;
	}

	/**
	 * [����] �ؒf.
	 *		�{�C���X�^���X�ŁADial �� Attach�����ڑ����A�ؒf���܂��B
	 *	@retval 0		�����B(�ڑ����Ă��Ȃ��ꍇ�������ɂȂ�܂�)
	 *	@retval 0�ȊO	�G���[�B GetErrorString()�ŕ����񉻂ł��܂��B
	 */
	DWORD HangUp(void)
	{
		EXCLUSIVE(&m_syncFunc);
		if ( m_hRasConn == NULL )
		{
			m_piListener = NULL;
			return 0;
		}
		//
		m_threadWatch.Stop();
		DWORD dwRc = ::RasHangUp(m_hRasConn);
		_DIALUP_TRACE(RasHangUp, dwRc);
		//
		RASCONNSTATUS tStatus;
		while ( true )
		{
			if ( GetConnectStatus(tStatus) == ERROR_INVALID_HANDLE )
			{
				break;
			}
			::Sleep(10);
		}
		if ( dwRc == ERROR_NO_CONNECTION )
		{
			dwRc = 0;
		}
		m_piListener = NULL;
		m_ClearHandleRasConn();
		return dwRc;
	}

	/**
	 * [�擾] �ڑ���Ԏ擾
	 *	@param[out]	_tStatus ���
	 *	@retval 0		����
	 *	@retval 0�ȊO	�G���[�B GetErrorString()�ŕ����񉻂ł��܂��B
	 */
	DWORD GetConnectStatus(RASCONNSTATUS& _tStatus) const
	{
		_tStatus.dwSize = sizeof(RASCONNSTATUS);
		DWORD dwRc = ::RasGetConnectStatus(m_hRasConn, &_tStatus);
		_DIALUP_TRACE(RasGetConnectStatus, dwRc);
		return dwRc;
	}

	/**
	 * [�擾] �f�o�C�X���擾.
	 *		�{�C���X�^���X�ň����Ă���ڑ��̃f�o�C�X���������܂��B
	 *	@return �f�o�C�X���B�A�^�b�`(�ڑ�)���Ă��Ȃ��ꍇEmpty���Ԃ�܂��B
	 */
	CStr GetConnectedDeviceName(void) const
	{
		CStr str;
		RASCONNSTATUS tStatus;
		DWORD dwRc = GetConnectStatus(tStatus);
		if ( dwRc == 0 )
		{
			str = tStatus.szDeviceName;
		}
		return str;
	}

	/**
	 * [�擾] IP�A�h���X�擾.
	 *		�{�C���X�^���X�ň����Ă���ڑ���IP�A�h���X�������܂��B
	 *	@return IP�A�h���X�B�A�^�b�`(�ڑ�)���Ă��Ȃ��ꍇEmpty���Ԃ�܂��B
	 */
	CStr GetIpAddress(void) const
	{
		return GetIpAddress(m_hRasConn);
	}

	/**
	 * [�擾] RAS�n���h���擾.
	 *		�{�C���X�^���X�ň����Ă���ڑ���RAS�n���h���������܂��B
	 *	@return �n���h���B�A�^�b�`(�ڑ�)���Ă��Ȃ��ꍇNULL�ɂȂ�܂��B
	 */
	operator HRASCONN()
	{
		return m_hRasConn;
	}
	
	/**
	 * [�Q��] �Ď��X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadWatch;
	}


	//============================================================================


	/**
	 * [�擾] �ڑ���IP�A�h���X�擾
	 *	@param hRasConn RAS�̃n���h��
	 *	@return IP�A�h���X�B���ڑ��Ȃ�Empty���Ԃ�܂��B
	 */
	static CStr GetIpAddress(HRASCONN hRasConn)
	{
		RASPPPIP tRasPppIp = { sizeof(RASPPPIP) };
		DWORD dwLen = sizeof(RASPPPIP);
		DWORD dwRc = ::RasGetProjectionInfo(hRasConn, RASP_PppIp, &tRasPppIp, &dwLen);
		_DIALUP_TRACE(RasGetProjectionInfo, dwRc);
		if ( dwRc == 0 )
		{	
			return tRasPppIp.szIpAddress;
		}
		return _T("");
	}

	/**
	 * [�m�F] �w��Entry�ڑ�������ԃ`�F�b�N
	 *	@param lpszPhoneBook �d�b�����BNULL�w��\�B
	 *	@param lpszEntryname �G���g�����B
	 *	@return RAS�̃n���h���BNULL�Ȃ疢�ڑ�(�ڑ����������܂�)
	 */
	static HRASCONN GetRasConn(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryname)
	{
		CStr strPb = lpszPhoneBook;
		CRasConnVector aRass = ms_GetConnectingEnties();
		loop ( i, aRass.GetSize() )
		{
			const TRasConn &T = aRass.At(i);
			if ( STRLIB::Compare(T.szEntryName, lpszEntryname) == 0 )
			{
				if ( T.IsPhoneBookEqual(strPb) )
				{
					//����
					HRASCONN hRasComm = T.hRasConn;
					RASCONNSTATUS tStatus = { sizeof(RASCONNSTATUS) };
					DWORD dwRc = ::RasGetConnectStatus(hRasComm, &tStatus);
					_DIALUP_TRACE(RasGetConnectStatus, dwRc);
					if ( dwRc != 0 || tStatus.rasconnstate != RASCS_Connected )
					{
						return NULL;
					}
					return hRasComm;
				}
			}
		}
		return NULL;
	}

	/**
	 * [�擾] �G���g�����ꗗ
	 *	@param	lpszPhoneBook	�d�b�����BNULL�w��\�B
	 *	@return �G���g�����ꗗ�B�����Ȃ�API�G���[�����B
	 */
	static CStrVector EnumEntryNames(LPCTSTR lpszPhoneBook)
	{
		ms_CheckPhoneBook(lpszPhoneBook);
		CStrVector vstrNames;
		CByteVector ba;
		LPRASENTRYNAME P = reinterpret_cast<LPRASENTRYNAME>(ba.GetBuffer(sizeof(RASENTRYNAME)));
		P->dwSize = sizeof(RASENTRYNAME);
		DWORD dwBufferSize = sizeof(RASENTRYNAME);
		DWORD dwEntries = 0;
		DWORD dwRc = ::RasEnumEntries(NULL, NcStr(lpszPhoneBook), P, &dwBufferSize, &dwEntries);
		if ( dwRc == ERROR_BUFFER_TOO_SMALL )
		{
			P = reinterpret_cast<LPRASENTRYNAME>(ba.GetBuffer(dwBufferSize));
			P->dwSize = sizeof(RASENTRYNAME);
			dwRc = ::RasEnumEntries(NULL, NcStr(lpszPhoneBook), P, &dwBufferSize, &dwEntries);
		}
		if ( dwRc != ERROR_SUCCESS )
		{
			_DIALUP_TRACE(RasEnumEntries, dwRc);
			vstrNames.Invalid();
			return vstrNames;
		}
		else
		{
			vstrNames.SetIncrementSize(dwEntries);
			loop ( i, dwEntries )
			{
				vstrNames.Add(P->szEntryName);
				P++;
			}
		}
		return vstrNames;
	}

	/**
	 * [�擾] �ڑ����G���g�����ꗗ
	 *	@return �G���g�����ꗗ�B�����Ȃ�API�G���[�����B
	 */
	static CStrVector EnumConnectedEntryNames(void)
	{
		CStrVector vstrEntryNames;
		//
		CRasConnVector aRass = ms_GetConnectingEnties();
		if ( ! aRass.IsValid() )
		{
			vstrEntryNames.Invalid();
			return vstrEntryNames;
		}
		size_t iLen = aRass.GetSize();
		vstrEntryNames.SetIncrementSize(iLen);
		loop ( i, iLen )
		{
			vstrEntryNames.Add(aRass.At(i).szEntryName);
		}
		//
		return vstrEntryNames;
	}

	/**
	 * [�擾] �G���g���̃_�C�A���p�����[�^�擾
	 *	@param lpszPhoneBook �d�b�����BNULL�w��\�B
	 *	@param lpszEntryName �G���g�����B
	 *	@return �p�����[�^�B�G���[�̏ꍇ�A�����ȃp�����[�^���Ԃ�܂��B
	 */
	static TDialParams GetDialParams(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryName)
	{
		ms_CheckPhoneBook(lpszPhoneBook);
		TDialParams tRc;
		//
		RASDIALPARAMS tParams = { sizeof(RASDIALPARAMS) };
		STRLIB::Copy(tParams.szEntryName, lpszEntryName);
		//
		BOOL boHasPassword = TRUE;
		DWORD dwParamsRc = ::RasGetEntryDialParams(NcStr(lpszPhoneBook), &tParams, &boHasPassword);
		if ( dwParamsRc != 0 )
		{
			_DIALUP_TRACE(RasGetEntryDialParams, dwParamsRc);
			return tRc;
		}
		//
		DWORD dwSize = 0;
		::RasGetEntryProperties(NcStr(lpszPhoneBook), NcStr(lpszEntryName), NULL, &dwSize, NULL, NULL);
		CByteVector ba;
		LPRASENTRY ptEntry = reinterpret_cast<LPRASENTRY>(ba.GetBuffer(dwSize));
		ptEntry->dwSize = dwSize;
		DWORD dwPropertyRc = ::RasGetEntryProperties(NcStr(lpszPhoneBook), NcStr(lpszEntryName), ptEntry, &dwSize, NULL, NULL);
		if ( dwPropertyRc != 0 )
		{
			_DIALUP_TRACE(RasGetEntryProperties, dwPropertyRc);
			return tRc;
		}
		//
		tRc.strEntryName	= lpszEntryName;
		tRc.strUserName		= tParams.szUserName;
		if ( boHasPassword )
		{
			tRc.strPassword	= tParams.szPassword;
		}
		tRc.strPhoneNumber	= ptEntry->szLocalPhoneNumber;
		tRc.strDeviceName	= ptEntry->szDeviceName;
		tRc.strPhoneBook	= lpszPhoneBook;
		//
		return tRc;
	}

	/**
	 * [�擾] �ڑ�����Ă���f�o�C�X���ꗗ
	 *	@note	�ꗗ�쐬��RAS-API���g�p���Ă��܂��B
	 *	@param boIsModemOnly true�Ȃ�MODEM�̂݁B�ȗ������RAS�Ǘ��̃f�o�C�X���ׂāB
	 *	@return �f�o�C�X�̖��O�̈ꗗ�B�����Ȃ�API�G���[�����B
	 */
	static CStrVector EnumDeviceNames(bool boIsModemOnly = true)
	{
		CStrVector vstrNames;
		CByteVector ba;
		LPRASDEVINFO P = reinterpret_cast<LPRASDEVINFO>(ba.GetBuffer(sizeof(RASDEVINFO)));
		P->dwSize = sizeof(RASDEVINFO);
		DWORD dwBufferSize = sizeof(RASDEVINFO);
		DWORD dwEntries = 0;
		DWORD dwRc = ::RasEnumDevices(P, &dwBufferSize, &dwEntries);
		if ( dwRc == ERROR_BUFFER_TOO_SMALL )
		{
			P = reinterpret_cast<LPRASDEVINFO>(ba.GetBuffer(dwBufferSize));
			P->dwSize = sizeof(RASDEVINFO);
			dwRc = ::RasEnumDevices(P, &dwBufferSize, &dwEntries);
		}
		if ( dwRc != ERROR_SUCCESS)
		{
			_DIALUP_TRACE(RasEnumDevices, dwRc);
			vstrNames.Invalid();
			return vstrNames;
		}
		else
		{
			vstrNames.SetIncrementSize(dwEntries);
			loop ( i, dwEntries )
			{
				if ( ! boIsModemOnly || STRLIB::Compare(P->szDeviceType, RASDT_Modem) == 0)
				{
					vstrNames.Add(P->szDeviceName);
				}
				P++;
			}
		}
		return vstrNames;
	}

	#ifndef _WIN32_WCE
	/**
	 * [�擾] �f�o�C�X(���f��)���ꗗ.
	 *			COMx�ɑ΂���f�o�C�X���������܂��B
	 *	@note	CStrVector ��Index 1���uCOM1�v�̃f�o�C�X���AIndex 2���uCOM2�v�̃f�o�C�X���ƂȂ�܂��B
	 *			Empty�Ȃ�f�o�C�X�����ł��B
	 *	@note	�ꗗ�쐬�Ƀ��W�X�g�����Q�Ƃ��Ă��܂��B
	 *	@param[out] _vstrModemNames Size��257�ɂȂ��Ă���ACOM�|�[�g��Index�ŁA���ꂼ��ɑ��݂���΃��f�������i�[�����B
	 *	@return ������(�z�񐔂ł͂Ȃ�)
	 */
	static int EnumModemPorts(CStrVector& _vstrModemNames)
	{
		OSVERSIONINFO tOsVerInfo = { sizeof(OSVERSIONINFO) };
		::GetVersionEx(&tOsVerInfo);
		//�n�r�ɂ��擾���郌�W�X�g���p�X���Ⴄ
		CStr strModemBase;
		if ( tOsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		{
			//Windows95/98�n
			strModemBase = "SYSTEM\\CurrentControlSet\\Services\\Class\\Modem";
		}
		else
		{
			//WindowsNT�n
			strModemBase = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96D-E325-11CE-BFC1-08002BE10318}";
		}
		//
		int iFoundItemNumber = 0;
		_vstrModemNames.RemoveAll();
		_vstrModemNames.SetSize(257);
//		CRegistryAccessor regbase(CRegistryAccessor(HKEY_LOCAL_MACHINE)[strModemBase]);
		CRegistryAccessor regbase(CRegistryAccessor(HKEY_LOCAL_MACHINE).GetHandle(strModemBase));
		CStrVector strSecNames = regbase.EnumSectionNames();
		loop ( i, strSecNames.GetSize() )
		{
			CRegistryAccessor::CSection reg =regbase[strSecNames[i]];
			//���f�����擾		//"FriendlyName"�̃L�[���ƃf�o�C�X�}�l�[�W���Ɠ����\�L
			CStr strModemName = reg.QueryString(_T("Model"));
			if ( strModemName.IsEmpty() )
			{
				continue;
			}
			//COM�|�[�g���擾����
			CStr strPort = reg.QueryString(_T("AttachedTo"));
			if ( strPort.GetLength() < 4 )
			{
				continue;
			}
			int iPort = strPort.ToInt(3);
			if ( iPort < 0 || iPort > 256 )
			{
				continue;
			}
			_vstrModemNames.Set(iPort, strModemName);
			iFoundItemNumber++;
		}
		return iFoundItemNumber;
	}

	/**
	 * [�擾] �f�o�C�X�������R�}���h�ݒ�.
	 *			�w��f�o�C�X(���f��)�́u�ǉ��̏������R�}���h�v��ݒ肵�܂��B
	 *	@note	���W�X�g���𑀍삵�Ă��܂��B
	 *	@param lpszModemName �f�o�C�X(���f��)���B
	 *	@param lpszCommand �ǉ��̏������R�}���h�BNULL���w�肷��ƁA�N���A���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static int SetUserInitCommand(LPCTSTR lpszModemName, LPCTSTR lpszCommand)
	{
		OSVERSIONINFO tOsVerInfo = { sizeof(OSVERSIONINFO) };
		::GetVersionEx(&tOsVerInfo);
		//�n�r�ɂ��擾���郌�W�X�g���p�X���Ⴄ
		CStr strModemBase;
		if ( tOsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		{
			//Windows95/98�n
			strModemBase = "SYSTEM\\CurrentControlSet\\Services\\Class\\Modem";
		}
		else
		{
			//WindowsNT�n
			strModemBase = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96D-E325-11CE-BFC1-08002BE10318}";
		}
		//
		CRegistryAccessor regbase(CRegistryAccessor(HKEY_LOCAL_MACHINE).GetHandle(strModemBase));
		CStrVector strSecNames = regbase.EnumSectionNames();
		loop ( i, strSecNames.GetSize() )
		{
			CRegistryAccessor::CSection reg =regbase[strSecNames[i]];
			//���f�����擾		//"FriendlyName"�̃L�[���ƃf�o�C�X�}�l�[�W���Ɠ����\�L
			CStr strModemName = reg.QueryString(_T("Model"));
			if ( strModemName.IsEqual(lpszModemName) )
			{
				bool r;
				if ( lpszCommand == NULL || lpszCommand[0] == 0 )
				{
					r = reg.DeleteKey(_T("UserInit"));
				}
				else
				{
					r = reg.WriteString(_T("UserInit"), lpszCommand);
				}
				return r;
			}
		}
		return false;
	}
	#endif //_WIN32_WCE

	/**
	 * [�쐬] �d�b�G���g���쐬
	 *	@param lpszPhoneBook	�d�b�����BNULL�w��\�B
	 *	@param lpszEntryName	�G���g�����B
	 *	@param lpszDeviceName	�f�o�C�X���B
	 *	@param lpszAreaCode		�G���A�R�[�h�iNULL�w��\�j
	 *	@param lpszPhoneNumber	�d�b�ԍ��B
	 *	@param lpszUserName		���[�U���B
	 *	@param lpszPassword		�p�X���[�h�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool CreatePhoneBookEntry(
				LPCTSTR lpszPhoneBook,
				LPCTSTR lpszEntryName,
				LPCTSTR lpszDeviceName,
				LPCTSTR lpszAreaCode,
				LPCTSTR lpszPhoneNumber,
				LPCTSTR lpszUserName,
				LPCTSTR lpszPassword )
	{
		if ( lpszPhoneBook != NULL && lpszPhoneBook[0] == '\0' )
		{
			lpszPhoneBook = NULL;
		}
		// RASENTRY �\���̂̃T�C�Y
		RASENTRY rasEntry = { sizeof(RASENTRY) };
		rasEntry.dwfOptions =
				// DNS���g�p����
				RASEO_SpecificNameServers
				//�@�����[�g�f�t�H���g�Q�[�g�E�F�C
				| RASEO_RemoteDefaultGateway;
		//
		CStr strAreaCode = lpszAreaCode;
		if ( ! strAreaCode.IsEmpty() )
		{
			// �s�O�_�C�A�����g�p����
			rasEntry.dwfOptions |= RASEO_UseCountryAndAreaCodes;
		}
		rasEntry.dwfOptions = 0x38010118;	//�Ȃ�ł��������H
		//
		rasEntry.dwCountryID			= 81;
		rasEntry.dwCountryCode			= 81;
		rasEntry.dwfNetProtocols		= RASNP_Ip;
		rasEntry.dwFramingProtocol		= RASFP_Ppp;
		_tcsncpy(rasEntry.szAreaCode, strAreaCode, sizeof(rasEntry.szAreaCode) - 1);
		_tcsncpy(rasEntry.szLocalPhoneNumber, lpszPhoneNumber, sizeof(rasEntry.szLocalPhoneNumber) - 1);
		_tcsncpy(rasEntry.szDeviceType, RASDT_Modem, sizeof(rasEntry.szDeviceType) - 1);
		_tcsncpy(rasEntry.szDeviceName, lpszDeviceName, sizeof(rasEntry.szDeviceName) - 1);
		DWORD dwError;
		dwError = ::RasSetEntryProperties (NcStr(lpszPhoneBook), NcStr(lpszEntryName), &rasEntry, sizeof(RASENTRY), NULL, NULL);
		if ( dwError != 0 )
		{
			// �G���[����
			return false;
		}
		RASDIALPARAMS	dialParm = {sizeof(RASDIALPARAMS)};
		dialParm.dwSize = sizeof (RASDIALPARAMS);
		_tcsncpy(dialParm.szEntryName, lpszEntryName, sizeof(dialParm.szEntryName) - 1);
		_tcsncpy(dialParm.szUserName, lpszUserName, sizeof(dialParm.szUserName) - 1);
		_tcsncpy(dialParm.szPassword, lpszPassword, sizeof(dialParm.szPassword) - 1);
		//
		dwError = ::RasSetEntryDialParams(NcStr(lpszPhoneBook), &dialParm, FALSE);
		if ( dwError != 0 )
		{
			// �G���[����
			return false;
		}
		return true;
	}

	/**
	 * [�ϊ�] �G���[�R�[�h�𕶎���ɕϊ�
	 *	@param dwError �G���[�R�[�h
	 *	@return ������
	 */
	static CStr GetErrorString(DWORD dwError)
	{
		CStr str;
		#ifndef _WIN32_WCE
			::RasGetErrorString(dwError, str.GetBuffer(256), 256 * sizeof(TCHAR));
			str.ReleaseBuffer();
		#else
			if ( dwError >= 600 && dwError <= 752 )
			{
				str.Format(_T("RAS�G���[(%d)"), dwError);
			}
		#endif
		if ( str.IsEmpty() )
		{
			str = SystemErrorToMessageText(dwError);
		}
		str.Remove('\n');
		str.Remove('\r');
		return str;
	}

	/**
	 * [�ϊ�] �X�e�[�^�X�𕶎���ɕϊ�
	 *	@param eState �X�e�[�^�X�R�[�h
	 *	@return ������
	 */
	static CStr StateToString(RASCONNSTATE eState)
	{
		const CStaticMapT<RASCONNSTATE, LPCTSTR> map[] =
		{
			{	RASCS_OpenPort				,_T("�|�[�g���J���Ă��܂�")			},
			{	RASCS_PortOpened			,_T("�|�[�g���J���܂���")			},
			{	RASCS_ConnectDevice			,_T("�@���ڑ����Ă��܂�")			},
			{	RASCS_DeviceConnected		,_T("�@���ڑ����܂���")			},
			{	RASCS_AllDevicesConnected	,_T("���ׂĂ̋@���ڑ����܂���")	},
			{	RASCS_Authenticate			,_T("�F�ؒ��ł�")					},
			{	RASCS_AuthNotify			,_T("�F�؂��I�����܂���")			},
			{	RASCS_AuthRetry				,_T("�F�؂����g���C���ł�")			},
			{	RASCS_AuthCallback			,_T("�F�؂��R�[���o�b�N���ł�")		},
			{	RASCS_AuthChangePassword	,_T("�F�؃p�X���[�h��ύX���Ă��܂�")},
			{	RASCS_AuthProject			,_T("���e�̔F�ؒ��ł�")				},
			{	RASCS_AuthLinkSpeed			,_T("�ڑ����x��F�ؒ��ł�")			},
			{	RASCS_AuthAck				,_T("ACK�F�ؒ��ł�")				},
			{	RASCS_ReAuthenticate		,_T("�ĔF�ؒ��ł�")					},
			{	RASCS_Authenticated			,_T("�F�؂��I�����܂���")			},
			{	RASCS_PrepareForCallback	,_T("�R�[���o�b�N�������ł�")		},
			{	RASCS_WaitForModemReset		,_T("���f���̃��Z�b�g�҂��ł�")		},
			{	RASCS_WaitForCallback		,_T("�R�[���o�b�N�̑ҋ@���ł�")		},
			{	RASCS_Projected				,_T("���e�I�����܂���")				},
		#ifndef _WIN32_WCE
		#if ( WINVER >= 0x400 )
			{	RASCS_StartAuthentication	,_T("�F�؂��J�n���܂�")				},
			{	RASCS_CallbackComplete		,_T("�R�[���o�b�N���������܂���")	},
			{	RASCS_LogonNetwork			,_T("�l�b�g���[�N�Ƀ��O�I�����܂���")},
		#endif
			{	RASCS_SubEntryConnected		,_T("SubEntryConnected")			},
 			{	RASCS_SubEntryDisconnected	,_T("SubEntryDisconnected")			},
			{	RASCS_Interactive			,_T("Interactive")					},
			{	RASCS_RetryAuthentication	,_T("RetryAuthentication")			},
			{	RASCS_CallbackSetByCaller	,_T("CallbackSetByCaller")			},
			{	RASCS_PasswordExpired		,_T("PasswordExpired")				},
		#if ( WINVER >= 0x500 )
			{	RASCS_InvokeEapUI			,_T("InvokeEapUI")					},
		#endif
		#endif //_WIN32_WCE
			{	RASCS_Connected				,_T("�ڑ�����")						},
			{	RASCS_Disconnected			,_T("�ؒf����")						},
		};
		LPCTSTR P = CStaticMapT<RASCONNSTATE, LPCTSTR>::Find(eState, map);
		return (P != NULL) ? CStr(P) : CStr::Fmt(_T("Unknown State Code [%d]"), eState);
	}
};



#undef _DIALUP_TRACE



}; // TNB



#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��

/**@ingroup RAS
 * RAS��ԃR�[�h
 */
enum RASCONNSTATE
{
	RASCS_OpenPort				,///< �|�[�g���J���Ă��܂�
	RASCS_PortOpened			,///< �|�[�g���J���܂���
	RASCS_ConnectDevice			,///< �@���ڑ����Ă��܂�
	RASCS_DeviceConnected		,///< �@���ڑ����܂���
	RASCS_AllDevicesConnected	,///< ���ׂĂ̋@���ڑ����܂���
	RASCS_Authenticate			,///< �F�ؒ��ł�
	RASCS_AuthNotify			,///< �F�؂��I�����܂���
	RASCS_AuthRetry				,///< �F�؂����g���C���ł�
	RASCS_AuthCallback			,///< �F�؂��R�[���o�b�N���ł�
	RASCS_AuthChangePassword	,///< �F�؃p�X���[�h��ύX���Ă��܂�
	RASCS_AuthProject			,///< ���e�̔F�ؒ��ł�
	RASCS_AuthLinkSpeed			,///< �ڑ����x��F�ؒ��ł�
	RASCS_AuthAck				,///< ACK�F�ؒ��ł�
	RASCS_ReAuthenticate		,///< �ĔF�ؒ��ł�
	RASCS_Authenticated			,///< �F�؂��I�����܂���
	RASCS_PrepareForCallback	,///< �R�[���o�b�N�������ł�
	RASCS_WaitForModemReset		,///< ���f���̃��Z�b�g�҂��ł�
	RASCS_WaitForCallback		,///< �R�[���o�b�N�̑ҋ@���ł�
	RASCS_Projected				,///< ���e�I�����܂���
	RASCS_StartAuthentication	,///< �F�؂��J�n���܂�
	RASCS_CallbackComplete		,///< �R�[���o�b�N���������܂���
	RASCS_LogonNetwork			,///< �l�b�g���[�N�Ƀ��O�I�����܂���
	RASCS_SubEntryConnected		,///< SubEntryConnected
 	RASCS_SubEntryDisconnected	,///< SubEntryDisconnected
	RASCS_Interactive			,///< Interactive
	RASCS_RetryAuthentication	,///< RetryAuthentication
	RASCS_CallbackSetByCaller	,///< CallbackSetByCaller
	RASCS_PasswordExpired		,///< PasswordExpired
	RASCS_InvokeEapUI			,///< InvokeEapUI
	RASCS_Connected				,///< �ڑ�����
	RASCS_Disconnected			,///< �ؒf����
};

#endif //_TnbDOXYGEN
