#pragma once
/**
 *@file
 * プロセスハンドル管理ヘッダ
 *
 *	@note	マクロ "_TnbPROCESS_PipeInOut_ENABLE" が定義されていると、 {@link TNB::CProcessHandle::Create() CProcessHandle::Create()}
 *			で、パイプを指定したプロセス起動を使用できます（ TnbPipe.h が必要になります）。
 *
 *	@note	マクロ "_TnbPROCESS_EnumProcesses_ENABLE" が定義されていると、 
 *				{@link TNB::CProcessHandle::EnumProcesses() EnumProcesses()},
 *				{@link TNB::CProcessHandle::GetProcessName() GetProcessName()} が使用できます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
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
 * プロセスハンドル管理クラス
 *
 *		プロセスハンドルを一つ管理します。
 *		プロセス実行もサポートします。
 *
 *	@par必要ファイル
 *			TnbProcessHandle.h
 * 
 *	@date 07/01/24 新規作成
 *	@date 11/11/10 プロセスIDも記憶するようにした
 *	@date 12/05/11 EnumProcessesEntry() 新規。
 */
class CProcessHandle
{
public:

	/// コンストラクタ
	CProcessHandle(void)
	{
	}

	/**
	 * [通知] 強制終了.
	 *		管理しているプロセスをターミネートします。
	 *	@param uExitCode 終了コード。省略すると0になります。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [処理] 終了待ち.
	 *		管理しているプロセスの終了を待ちます。
	 *	@param dwWait タイムアウト時間を、ミリ秒で指定します。INFINITE を指定すると終了するまで待ち続けます。
	 *	@retval WAIT_ABANDONED プロセスを管理していないか，対象のプロセスが既にない。
	 *	@retval WAIT_OBJECT_0 終了したことを意味します(既に終了していた場合も含む) 。 
	 *	@retval WAIT_TIMEOUT タイムアウト時間が経過したことを意味します。
	 */
	DWORD WaitForExit(DWORD dwWait = INFINITE)
	{
		DWORD r = WAIT_ABANDONED;
		if ( ! m_hhProcess.IsNull() )
		{
			r = ::WaitForSingleObject(m_hhProcess, dwWait);	//プロセス待機
		}
		return r;
	}

	/**
	 * [確認] 起動確認.
	 *		管理しているプロセスが生きているか確認します。
	 *	@retval true 生きている。
	 *	@retval false いない。
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
	 * [取得] プロセスハンドル取得
	 *	@retval INVALID_HANDLE_VALUE 管理ハンドルなし
	 *	@retval INVALID_HANDLE_VALUE以外 プロセスハンドル
	 */
	operator HANDLE(void) const
	{
		return m_hhProcess;
	}

	/**
	 * [取得] プロセスハンドル取得
	 *	@retval INVALID_HANDLE_VALUE 管理ハンドルなし
	 *	@retval INVALID_HANDLE_VALUE以外 プロセスハンドル
	 */
	HANDLE GetProcessHandle(void) const
	{
		return m_hhProcess;
	}

	/**
	 * [取得] スレッドハンドル取得
	 *	@retval INVALID_HANDLE_VALUE 管理ハンドルなし
	 *	@retval INVALID_HANDLE_VALUE以外 スレッドハンドル
	 */
	HANDLE GetThreadHandle(void) const
	{
		return m_hhThread;
	}

	/**
	 * [取得] プロセスID取得
	 *	@note Attach() で設定した場合、プロセスID は管理しません。
	 *	@retval 0 管理IDなし
	 *	@retval 0以外 プロセスID
	 */
	DWORD GetProcessId(void) const
	{
		return m_processId;
	}

	/**
	 * [取得] プロセス終了コード取得
	 *	@retval -1 エラー。
	 *	@retval 0以上 終了コード。
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
	 * [設定] アタッチ.
	 *		指定のハンドルを管理するようにします。
	 *	@note プロセスID は管理しません。
	 *	@param hProcess プロセスハンドル
	 *	@param hThread スレッドハンドル
	 */
	void Attach(HANDLE hProcess, HANDLE hThread)
	{
		m_hhProcess = hProcess;
		m_hhThread = hThread;
		m_processId = 0;
	}

	/**
	 * [設定] デタッチ.
	 *		管理ハンドルを解放します（複製されている場合、ハンドルは Closeされません）。
	 */
	void Detach(void)
	{
		m_hhProcess.Null();
		m_hhThread.Null();
		m_processId = 0;
	}

#ifndef _WIN32_WCE
 	/**
	 * [実行] プロセス実行
	 *	@param lpszExecName 実行ファイル名
	 *	@param lpszCommand コマンドライン
	 *	@param si スタート情報
	 *	@retval true	成功
	 *	@retval	false	失敗
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
	 * [実行] プロセス実行
	 *	@param lpszExecName 実行ファイル名
	 *	@param lpszCommand コマンドライン
	 *	@param boIsShowWindow false ならWindowを非表示状態で起動させます。
	 *	@retval true	成功
	 *	@retval	false	失敗
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
	 * [実行] プロセス実行
	 *	@param lpszExecName 実行ファイル名
	 *	@param lpszCommand コマンドライン
	 *	@param boDummy ダミーです。
	 *	@retval true	成功
	 *	@retval	false	失敗
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
	 * [実行] パイプ指定プロセス実行.
	 *	@note stdin stdout strerr を指定します。
	 *	@param lpszCommand コマンド。実行ファイル名とオプションを記述。
	 *	@param pInputPipe プロセスへの入力用パイプ(stdin)を指定します。
	 *					{@link CPipe::Open Open} してある必要があります。
	 *					また、ここで指定したパイプはプロセス終了まで 破棄、および Stop しないようにしてください。
	 *	@param pOutputPipe プロセスからの出力用パイプ(stdout)を指定します。
	 *					{@link CPipe::Open Open} してある必要があります。
	 *					また、ここで指定したパイプはプロセス終了まで 破棄、および Stop しないようにしてください。
	 *	@param pErrorPipe プロセスからのエラー出力用パイプ(stderr)を指定します。
	 *					{@link CPipe::Open Open} してある必要があります。
	 *					また、ここで指定したパイプはプロセス終了まで 破棄、および Stop しないようにしてください。
	 *					\n 省略するか、NULLを指定すると、 pOutoutPipe を使用します。
	 *	@param boIsShowWindow false ならWindowを非表示状態で起動させます。
	 *	@retval true	成功
	 *	@retval	false	失敗（パイプは一つでも{@link CPipe::Open Open}していない場合も失敗します）。
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
	 * [処理] プロセス終了.
	 *		プロセスID でプロセスを終了できます。 プロセスハンドルがわかる場合、 API の ::TerminateProcess() を使用します。
	 *	@param processId プロセスID
	 *	@param uExitCode 終了コード。省略すると 0 になります。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [取得] プロセス名取得
	 *	@note マクロ "_TnbPROCESS_EnumProcesses_ENABLE" が定義されていないと使用できません。
	 *	@param[out] _name プロセス名
	 *	@param[in] processId プロセスID
	 *	@param[out] pCreateTime そのプロセスの作成時間が格納されます。要らない場合 NULL を指定します。
	 *	@retval true 成功。 _name に結果が格納されます。
	 *	@retval false 失敗。
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
	 * [取得] プロセスID一覧取得
	 *	@note マクロ "_TnbPROCESS_EnumProcesses_ENABLE" が定義されていないと使用できません。
	 *	@param[out] _ids プロセスID一覧
	 *	@retval true 成功。 _ids に結果が格納されます。
	 *	@retval false 失敗。
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
	 * [取得] プロセスID一覧取得
	 *		指定のプロセス名のプロセスIDを起動が新しい順に一覧します。
	 *	@note マクロ "_TnbPROCESS_EnumProcesses_ENABLE" が定義されていないと使用できません。
	 *	@param[out] _ids プロセスID一覧
	 *	@param[in] lpszProcessName プロセス名。 'abc.exe' のように、フル名を指定すること
	 *	@retval true 成功。 _ids に結果が格納されます。
	 *	@retval false 失敗。
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
	 * [取得] プロセス情報一覧取得
	 *	@note マクロ "_TnbPROCESS_EnumProcesses_ENABLE" が定義されていないと使用できません。
	 *	@return 一覧
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
	/// ソートパラメータ
	struct TParam
	{
		DWORD		processId;		///< プロセスID
		::FILETIME	createTime;		///< 作成時間
	};
	/// 比較関数
	static int ms_CompareParam(const void* P1, const void* P2)
	{
		const TParam* Q1 = static_cast<const TParam*>(P1);
		const TParam* Q2 = static_cast<const TParam*>(P2);
		return CompareFileTime(&Q2->createTime, &Q1->createTime);
	}
	#endif // _TnbPROCESS_EnumProcesses_ENABLE

private:
	CHandleHandle	m_hhProcess;	///< プロセスハンドル
	CHandleHandle	m_hhThread;		///< スレッドハンドル
	DWORD			m_processId;	///< プロセスID
};



}; //TNB
