#pragma once
/**
 *@file
 * �v���Z�X�n���h���Ǘ��w�b�_
 *
 *	@note	�}�N�� "_TnbPROCESS_PipeInOut_ENABLE" ����`����Ă���ƁA {@link TNB::CProcessHandle::Create() CProcessHandle::Create()}
 *			�ŁA�p�C�v���w�肵���v���Z�X�N�����g�p�ł��܂��i TnbPipe.h ���K�v�ɂȂ�܂��j�B
 *
 *	@note	�}�N�� "_TnbPROCESS_EnumProcesses_ENABLE" ����`����Ă���ƁA 
 *				{@link TNB::CProcessHandle::EnumProcesses() EnumProcesses()},
 *				{@link TNB::CProcessHandle::GetProcessName() GetProcessName()} ���g�p�ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#define __TNBPROCESSHANDLE_H__ ///< included symbol
#include "TnbHandleHandle.h"
#ifdef _TnbPROCESS_PipeInOut_ENABLE
 #include "TnbPipe.h"
#endif
#ifdef _TnbPROCESS_EnumProcesses_ENABLE
 #include "TnbVector.h"
 #include <Tlhelp32.h>
 #include <Psapi.h>
 #pragma comment( lib, "Psapi.lib" )
#endif
#ifdef _DEBUG
 #include <TnbTime.h>
#endif



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �v���Z�X�n���h���Ǘ��N���X
 *
 *		�v���Z�X�n���h������Ǘ����܂��B
 *		�v���Z�X���s���T�|�[�g���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbProcessHandle.h
 * 
 *	@date 07/01/24 �V�K�쐬
 *	@date 11/11/10 �v���Z�XID���L������悤�ɂ���
 *	@date 12/05/11 EnumProcessesEntry() �V�K�B
 */
class CProcessHandle
{
public:

	/// �R���X�g���N�^
	CProcessHandle(void)
	{
	}

	/**
	 * [�ʒm] �����I��.
	 *		�Ǘ����Ă���v���Z�X���^�[�~�l�[�g���܂��B
	 *	@param uExitCode �I���R�[�h�B�ȗ������0�ɂȂ�܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Terminate(UINT uExitCode = 0)
	{
		bool r = false;
		if ( ! m_hhProcess.IsNull() )
		{
			r = !! ::TerminateProcess(m_hhProcess, uExitCode);
		}
		return r;
	}

	/**
	 * [����] �I���҂�.
	 *		�Ǘ����Ă���v���Z�X�̏I����҂��܂��B
	 *	@param dwWait �^�C���A�E�g���Ԃ��A�~���b�Ŏw�肵�܂��BINFINITE ���w�肷��ƏI������܂ő҂������܂��B
	 *	@retval WAIT_ABANDONED �v���Z�X���Ǘ����Ă��Ȃ����C�Ώۂ̃v���Z�X�����ɂȂ��B
	 *	@retval WAIT_OBJECT_0 �I���������Ƃ��Ӗ����܂�(���ɏI�����Ă����ꍇ���܂�) �B 
	 *	@retval WAIT_TIMEOUT �^�C���A�E�g���Ԃ��o�߂������Ƃ��Ӗ����܂��B
	 */
	DWORD WaitForExit(DWORD dwWait = INFINITE)
	{
		DWORD r = WAIT_ABANDONED;
		if ( ! m_hhProcess.IsNull() )
		{
			r = ::WaitForSingleObject(m_hhProcess, dwWait);	//�v���Z�X�ҋ@
		}
		return r;
	}

	/**
	 * [�m�F] �N���m�F.
	 *		�Ǘ����Ă���v���Z�X�������Ă��邩�m�F���܂��B
	 *	@retval true �����Ă���B
	 *	@retval false ���Ȃ��B
	 */
	bool IsAlive(void) const
	{
		DWORD r = WAIT_ABANDONED;
		if ( ! m_hhProcess.IsNull() )
		{
			r = ::WaitForSingleObject(m_hhProcess, 0);
		}
		return r == WAIT_TIMEOUT;
	}

	/**
	 * [�擾] �v���Z�X�n���h���擾
	 *	@retval INVALID_HANDLE_VALUE �Ǘ��n���h���Ȃ�
	 *	@retval INVALID_HANDLE_VALUE�ȊO �v���Z�X�n���h��
	 */
	operator HANDLE(void) const
	{
		return m_hhProcess;
	}

	/**
	 * [�擾] �v���Z�X�n���h���擾
	 *	@retval INVALID_HANDLE_VALUE �Ǘ��n���h���Ȃ�
	 *	@retval INVALID_HANDLE_VALUE�ȊO �v���Z�X�n���h��
	 */
	HANDLE GetProcessHandle(void) const
	{
		return m_hhProcess;
	}

	/**
	 * [�擾] �X���b�h�n���h���擾
	 *	@retval INVALID_HANDLE_VALUE �Ǘ��n���h���Ȃ�
	 *	@retval INVALID_HANDLE_VALUE�ȊO �X���b�h�n���h��
	 */
	HANDLE GetThreadHandle(void) const
	{
		return m_hhThread;
	}

	/**
	 * [�擾] �v���Z�XID�擾
	 *	@note Attach() �Őݒ肵���ꍇ�A�v���Z�XID �͊Ǘ����܂���B
	 *	@retval 0 �Ǘ�ID�Ȃ�
	 *	@retval 0�ȊO �v���Z�XID
	 */
	DWORD GetProcessId(void) const
	{
		return m_processId;
	}

	/**
	 * [�擾] �v���Z�X�I���R�[�h�擾
	 *	@retval -1 �G���[�B
	 *	@retval 0�ȏ� �I���R�[�h�B
	 */
	LONGLONG GetExitCode(void) const
	{
		DWORD dw;
		if ( ! ::GetExitCodeProcess(m_hhProcess, &dw) )
		{
			return -1;
		}
		return dw;
	}

	/**
	 * [�ݒ�] �A�^�b�`.
	 *		�w��̃n���h�����Ǘ�����悤�ɂ��܂��B
	 *	@note �v���Z�XID �͊Ǘ����܂���B
	 *	@param hProcess �v���Z�X�n���h��
	 *	@param hThread �X���b�h�n���h��
	 */
	void Attach(HANDLE hProcess, HANDLE hThread)
	{
		m_hhProcess = hProcess;
		m_hhThread = hThread;
		m_processId = 0;
	}

	/**
	 * [�ݒ�] �f�^�b�`.
	 *		�Ǘ��n���h����������܂��i��������Ă���ꍇ�A�n���h���� Close����܂���j�B
	 */
	void Detach(void)
	{
		m_hhProcess.Null();
		m_hhThread.Null();
		m_processId = 0;
	}

#ifndef _WIN32_WCE
 	/**
	 * [���s] �v���Z�X���s
	 *	@param lpszExecName ���s�t�@�C����
	 *	@param lpszCommand �R�}���h���C��
	 *	@param si �X�^�[�g���
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	bool Create(LPCTSTR lpszExecName, LPCTSTR lpszCommand, STARTUPINFO& si)
	{
		Detach();
		//
		PROCESS_INFORMATION pi;
		CSimpleStr s = lpszCommand;
		if ( ::CreateProcess(lpszExecName, s, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) )
		{
			::WaitForInputIdle(pi.hProcess, INFINITE);
			m_hhProcess = pi.hProcess;
			m_hhThread = pi.hThread;
			m_processId = pi.dwProcessId;
			return true;
		}
		_GetLastError("CreateProcess");
		return false;
	}

 	/**
	 * [���s] �v���Z�X���s
	 *	@param lpszExecName ���s�t�@�C����
	 *	@param lpszCommand �R�}���h���C��
	 *	@param boIsShowWindow false �Ȃ�Window���\����ԂŋN�������܂��B
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	bool Create(LPCTSTR lpszExecName, LPCTSTR lpszCommand, bool boIsShowWindow = true)
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = static_cast<WORD>(boIsShowWindow ? SW_SHOWDEFAULT : SW_HIDE);
		return Create(lpszExecName, lpszCommand, si);
	}
#else
 	/**
	 * [���s] �v���Z�X���s
	 *	@param lpszExecName ���s�t�@�C����
	 *	@param lpszCommand �R�}���h���C��
	 *	@param boDummy �_�~�[�ł��B
	 *	@retval true	����
	 *	@retval	false	���s
	 */
	bool Create(LPCTSTR lpszExecName, LPCTSTR lpszCommand, bool boDummy = true)
	{
		Detach();
		//
		PROCESS_INFORMATION pi;
		if ( ::CreateProcess(lpszExecName, lpszCommand, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi) )
		{
			m_hhProcess = pi.hProcess;
			m_hhThread = pi.hThread;
			m_processId = pi.dwProcessId;
			return true;
		}
		return false;
	}
#endif // _WIN32_WCE

	#ifdef _TnbPROCESS_PipeInOut_ENABLE
 	/**
	 * [���s] �p�C�v�w��v���Z�X���s.
	 *	@note stdin stdout strerr ���w�肵�܂��B
	 *	@param lpszCommand �R�}���h�B���s�t�@�C�����ƃI�v�V�������L�q�B
	 *	@param pInputPipe �v���Z�X�ւ̓��͗p�p�C�v(stdin)���w�肵�܂��B
	 *					{@link CPipe::Open Open} ���Ă���K�v������܂��B
	 *					�܂��A�����Ŏw�肵���p�C�v�̓v���Z�X�I���܂� �j���A����� Stop ���Ȃ��悤�ɂ��Ă��������B
	 *	@param pOutputPipe �v���Z�X����̏o�͗p�p�C�v(stdout)���w�肵�܂��B
	 *					{@link CPipe::Open Open} ���Ă���K�v������܂��B
	 *					�܂��A�����Ŏw�肵���p�C�v�̓v���Z�X�I���܂� �j���A����� Stop ���Ȃ��悤�ɂ��Ă��������B
	 *	@param pErrorPipe �v���Z�X����̃G���[�o�͗p�p�C�v(stderr)���w�肵�܂��B
	 *					{@link CPipe::Open Open} ���Ă���K�v������܂��B
	 *					�܂��A�����Ŏw�肵���p�C�v�̓v���Z�X�I���܂� �j���A����� Stop ���Ȃ��悤�ɂ��Ă��������B
	 *					\n �ȗ����邩�ANULL���w�肷��ƁA pOutoutPipe ���g�p���܂��B
	 *	@param boIsShowWindow false �Ȃ�Window���\����ԂŋN�������܂��B
	 *	@retval true	����
	 *	@retval	false	���s�i�p�C�v�͈�ł�{@link CPipe::Open Open}���Ă��Ȃ��ꍇ�����s���܂��j�B
	 */
	bool Create(LPCTSTR lpszCommand, CPipe* pInputPipe, CPipe* pOutputPipe, 
				CPipe* pErrorPipe = NULL, bool boIsShowWindow = true)
	{
		Detach();
		//
		if ( pErrorPipe == NULL ) { pErrorPipe = pOutputPipe; }
		if ( pInputPipe == NULL || ! pInputPipe->IsOpened() ) { return false; }
		if ( pOutputPipe == NULL || ! pOutputPipe->IsOpened() ) { return false; }
		if ( ! pErrorPipe->IsOpened() ) { return false; }
		//
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.hStdInput = pInputPipe->GetReadHandle();
		si.hStdOutput = pOutputPipe->GetWriteHandle();
		si.hStdError = pErrorPipe->GetWriteHandle();
		si.wShowWindow = static_cast<WORD>(boIsShowWindow ? SW_SHOWDEFAULT : SW_HIDE);
		//
		PROCESS_INFORMATION pi;
		CSimpleStr s = lpszCommand;
		if ( ::CreateProcess(NULL, s, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) )
		{
			::WaitForInputIdle(pi.hProcess, INFINITE);
			m_hhProcess = pi.hProcess;
			m_hhThread = pi.hThread;
			m_processId = pi.dwProcessId;
			return true;
		}
		_GetLastError("CreateProcess");
		return false;
	}
	#endif

	/**
	 * [����] �v���Z�X�I��.
	 *		�v���Z�XID �Ńv���Z�X���I���ł��܂��B �v���Z�X�n���h�����킩��ꍇ�A API �� ::TerminateProcess() ���g�p���܂��B
	 *	@param processId �v���Z�XID
	 *	@param uExitCode �I���R�[�h�B�ȗ������ 0 �ɂȂ�܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool TerminateProcessId(DWORD processId, UINT uExitCode = 0)
	{
		CHandleHandle hhProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, processId);
		if ( hhProcess != INVALID_HANDLE_VALUE )
		{
			return !! ::TerminateProcess(hhProcess, uExitCode);
		}
		return false;
	}

	#ifdef _TnbPROCESS_EnumProcesses_ENABLE
	/**
	 * [�擾] �v���Z�X���擾
	 *	@note �}�N�� "_TnbPROCESS_EnumProcesses_ENABLE" ����`����Ă��Ȃ��Ǝg�p�ł��܂���B
	 *	@param[out] _name �v���Z�X��
	 *	@param[in] processId �v���Z�XID
	 *	@param[out] pCreateTime ���̃v���Z�X�̍쐬���Ԃ��i�[����܂��B�v��Ȃ��ꍇ NULL ���w�肵�܂��B
	 *	@retval true �����B _name �Ɍ��ʂ��i�[����܂��B
	 *	@retval false ���s�B
	 */
	static bool GetProcessName(CSimpleStr& _name, DWORD processId, ::FILETIME* pCreateTime = NULL)
	{
		CHandleHandle hhProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
		if ( hhProcess != INVALID_HANDLE_VALUE )
		{
			HMODULE hMod;
			DWORD cbNeeded;
			if ( ::EnumProcessModules(hhProcess, &hMod, sizeof(hMod), &cbNeeded) )
			{
				CWorkMemT<TCHAR> s(MAX_PATH);
				if ( ::GetModuleBaseName(hhProcess, hMod, s, MAX_PATH) )
				{
					_name = s;
					if ( pCreateTime != NULL )
					{
						::FILETIME ft;
						::GetProcessTimes(hhProcess, pCreateTime, &ft, &ft, &ft);
					}
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * [�擾] �v���Z�XID�ꗗ�擾
	 *	@note �}�N�� "_TnbPROCESS_EnumProcesses_ENABLE" ����`����Ă��Ȃ��Ǝg�p�ł��܂���B
	 *	@param[out] _ids �v���Z�XID�ꗗ
	 *	@retval true �����B _ids �Ɍ��ʂ��i�[����܂��B
	 *	@retval false ���s�B
	 */
	static bool EnumProcesses(CWorkMemT<DWORD>& _ids)
	{
		const int MAX_CNT = 1024;
		CWorkMemT<DWORD> procs(MAX_CNT);
		DWORD cbNeeded = 0;
		if ( ! ::EnumProcesses(procs.Ref(), MAX_CNT * sizeof(DWORD), &cbNeeded) )
		{
			return false;
		}
		int len = cbNeeded / sizeof(DWORD);
		_ids.Reset(len, procs.Ref());
		return true;
	}

	/**
	 * [�擾] �v���Z�XID�ꗗ�擾
	 *		�w��̃v���Z�X���̃v���Z�XID���N�����V�������Ɉꗗ���܂��B
	 *	@note �}�N�� "_TnbPROCESS_EnumProcesses_ENABLE" ����`����Ă��Ȃ��Ǝg�p�ł��܂���B
	 *	@param[out] _ids �v���Z�XID�ꗗ
	 *	@param[in] lpszProcessName �v���Z�X���B 'abc.exe' �̂悤�ɁA�t�������w�肷�邱��
	 *	@retval true �����B _ids �Ɍ��ʂ��i�[����܂��B
	 *	@retval false ���s�B
	 */
	static bool EnumProcesses(CWorkMemT<DWORD>& _ids, LPCTSTR lpszProcessName)
	{
		const int MAX_CNT = 1024;
		INDEX cnt = 0;
		::FILETIME ft;
		CWorkMemT<TParam> w1;
		w1.Resize(MAX_CNT);
		CWorkMemT<DWORD> w2;
		if ( EnumProcesses(w2) )
		{
			CSimpleStr s;
			loop ( i, w2.GetSize() )
			{
				if ( GetProcessName(s, w2[i], &ft) )
				{
					if ( STRLIB::Compare(s, lpszProcessName, -1, NORM_IGNORECASE) == 0 )
					{
						w1[cnt].processId = w2[i];
						w1[cnt].createTime = ft;
						cnt++;
						if ( cnt >= MAX_CNT )
						{
							break;
						}
					}
				}
			}
			::qsort(w1, cnt, sizeof(TParam), ms_CompareParam);
			_ids.Resize(cnt);
			loop ( i, cnt )
			{
				TRACE2("found %d, %s\n", w1[i].processId, CSystemTime(w1[i].createTime).Format());
				_ids[i] = w1[i].processId;
			}
			return true;
		}
		return false;
	}

	/**
	 * [�擾] �v���Z�X���ꗗ�擾
	 *	@note �}�N�� "_TnbPROCESS_EnumProcesses_ENABLE" ����`����Ă��Ȃ��Ǝg�p�ł��܂���B
	 *	@return �ꗗ
	 */
	static CVectorT<PROCESSENTRY32> EnumProcessesEntry(void)
	{
		CVectorT<PROCESSENTRY32> v;
		HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);
		if ( ::Process32First(hSnapshot, &entry) )
		{
			do {
				v.Add(entry);
			} while( ::Process32Next(hSnapshot, &entry) );
		}
		::CloseHandle(hSnapshot);
		return v;
	}

private:
	/// �\�[�g�p�����[�^
	struct TParam
	{
		DWORD		processId;		///< �v���Z�XID
		::FILETIME	createTime;		///< �쐬����
	};
	/// ��r�֐�
	static int ms_CompareParam(const void* P1, const void* P2)
	{
		const TParam* Q1 = static_cast<const TParam*>(P1);
		const TParam* Q2 = static_cast<const TParam*>(P2);
		return CompareFileTime(&Q2->createTime, &Q1->createTime);
	}
	#endif // _TnbPROCESS_EnumProcesses_ENABLE

private:
	CHandleHandle	m_hhProcess;	///< �v���Z�X�n���h��
	CHandleHandle	m_hhThread;		///< �X���b�h�n���h��
	DWORD			m_processId;	///< �v���Z�XID
};



}; //TNB
