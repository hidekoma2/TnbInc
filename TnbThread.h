#pragma once
/**
 *@file
 * スレッド管理関係のヘッダ
 *
 *		容易にスレッドの処理を作成することができます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbExchanger.h"
#include "TnbStrLib.h"
#ifndef _WIN32_WCE
 #include <process.h>
#endif



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#if ! defined(_WIN32_WCE)
  #if defined(__AFX_H__)
	// MFC
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		CWinThread* P = ::AfxBeginThread(static_cast<AFX_THREADPROC>(pAddr), pParam, 0, 0, CREATE_SUSPENDED, NULL);
		if ( P != NULL )
		{
			*pThreadId = P->m_nThreadID;
			return P->m_hThread;
		}
		return NULL;
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::AfxEndThread(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
	}
  #elif defined(_MT)
	// Cランタイム使用バージョン
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		UINT uId = 0;
		typedef unsigned(__stdcall* P_FUNC)(LPVOID);
		INT_PTR dw = ::_beginthreadex(NULL, 0 , static_cast<P_FUNC>(pAddr), pParam, CREATE_SUSPENDED, &uId);
		*pThreadId = uId;
		return reinterpret_cast<HANDLE>(dw);
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::_endthreadex(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
		::CloseHandle(h);
	}
  #else
 	// API 使用バージョン
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		return ::CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(pAddr), pParam, CREATE_SUSPENDED, pThreadId);
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::ExitThread(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
		::CloseHandle(h);
	}
  #endif
#else
	// API 使用バージョン
	inline HANDLE il_CreateThread(LPVOID pAddr, LPVOID pParam, DWORD* pThreadId)
	{
		return ::CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(pAddr), pParam, CREATE_SUSPENDED, pThreadId);
	}
	inline void il_ExitThread(DWORD dwResult)
	{
		::ExitThread(dwResult);
	}
	inline void il_CloseThreadHandle(HANDLE h)
	{
		::CloseHandle(h);
	}
#endif

#ifdef _DEBUG
  // デバグ表示用
  #ifdef __MSVCPP6__
	#define TNB_THREADNAMEFRM1	"ｽﾚｯﾄﾞ 0x%X"
	#define TNB_THREADNAMEFRM2	"ｽﾚｯﾄﾞ 0x%X [%s]"
	#define TNB_THREADTRACETYPE	"ｽﾚｯﾄﾞ 0x%X [%s] %s。\n"
  #else
	#define TNB_THREADNAMEFRM1	"スレッド 'Win32 スレッド' (0x%x)"
	#define TNB_THREADNAMEFRM2	"スレッド 'Win32 スレッド' (0x%x) [%s]"
	#define TNB_THREADTRACETYPE	"スレッド 'Win32 スレッド' (0x%x) [%s] %s。\n"
  #endif
#endif

#endif //_TnbDOXYGEN



/**@ingroup THREAD
 * スレッド状態管理クラス
 *
 *		スレッドの状態を管理します。
 *
 *	@par必要ファイル
 *			TnbThread.h
 *
 *	@date 09/09/10	CThread から分離
 *	@date 10/01/14	プライオリティの設定方法を明記。
 *	@date 10/03/19	SetOwnerHwnd() 追加。
 */
class CThreadStatus
{
public:

	/// コンストラクタ.
	CThreadStatus(void) : m_hThread(NULL), m_threadId(NULL)
	{
	}

	/// デストラクタ
	virtual ~CThreadStatus(void)
	{
	}

	/**
	 * [取得] スレッドのハンドル取得
	 *	@retval ハンドル
	 */
	operator HANDLE(void) const
	{
		return m_hThread;
	}

	/**
	 * [取得] スレッドのハンドル取得
	 *	@retval ハンドル
	 */
	HANDLE GetHandle(void) const
	{
		return m_hThread;
	}

	/**
	 * [取得] スレッドのID取得
	 *	@return ID
	 */
	DWORD GetId(void) const
	{
		if ( ! IsAlive() )
		{
			return 0;
		}
		return m_threadId;
	}

	/**
	 * [確認] スレッドが生きているか
	 *	@retval true	生きている
	 *	@retval false	いない
	 */
	bool IsAlive(void) const
	{
		return m_hThread != NULL;
	}

	/**
	 * [設定] 関連ウィンドウハンドル設定.
	 *		スレッド終了時にポンプするウィンドウハンドルを指定します。
	 *	@see CThread::SetOwnerHwnd()
	 *	@param hWnd ウィンドウハンドル
	 */
	void SetOwnerHwnd(HWND hWnd)
	{
		m_hOwnerWnd = hWnd;
	}

	/**
	 * [取得] 優先度取得
	 *	@note 設定していない場合、優先度は THREAD_PRIORITY_NORMAL です。
	 *	@note Startしていない場合、値は THREAD_PRIORITY_NORMAL です。
	 *	@retval THREAD_PRIORITY_TIME_CRITICAL	リアルタイム
	 *	@retval THREAD_PRIORITY_HIGHEST			高
	 *	@retval THREAD_PRIORITY_ABOVE_NORMAL	通常以上
	 *	@retval THREAD_PRIORITY_NORMAL			通常
	 *	@retval THREAD_PRIORITY_BELOW_NORMAL	通常以下
	 *	@retval THREAD_PRIORITY_LOWEST			低
	 *	@retval THREAD_PRIORITY_ABOVE_IDLE		アイドル以上
	 *	@retval THREAD_PRIORITY_IDLE			アイドル
	 */
	int GetPriority(void) const
	{
		return IsAlive() ? ::GetThreadPriority(m_hThread) : THREAD_PRIORITY_NORMAL;
	}

	/**
	 * [設定] 優先度設定
	 *	@param priority 優先度。 GetPriority() の戻り値を参照。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetPriority(int priority = THREAD_PRIORITY_NORMAL)
	{
		if ( IsAlive() )
		{
			if ( ::SetThreadPriority(m_hThread, priority) )
			{
				return true;
			}
			_GetLastError("SetThreadPriority");
		}
		return false;
	}

	/**
	 * [実行] PostThreadメッセージ
	 *	@param message メッセージ。
	 *	@param wParam WPARAM。
	 *	@param lParam LPARAM。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		bool r = !! ::PostThreadMessage(GetId(), message, wParam, lParam);
		if ( ! r )
		{
			_GetLastError("PostThreadMessage");
		}
		return r;
	}

#ifndef _WIN32_WCE
	/**
	 * [設定] プロセッサアフィニティマスク設定.
	 *	@param affinityMask アフィニティマスク。各ビットはこのスレッドを実行できるプロセッサを表します。
	 *	@retval 0 エラー。
	 *	@retval 0以外 以前のアフィニティマスク。
	 */
	DWORD_PTR SetAffinityMask(DWORD_PTR affinityMask)
	{
		DWORD_PTR r = ::SetThreadAffinityMask(m_hThread, affinityMask);
		if ( r == 0 )
		{
			_GetLastError("SetThreadAffinityMask");
		}
		return r;
	}
#endif

protected:
	HANDLE				m_hThread;			///< スレッドのハンドル
	DWORD				m_threadId;			///< スレッドのID
	HWND				m_hOwnerWnd;		///< オーナーウィンドウ
};



/**@ingroup THREAD
 * スレッド管理クラス
 *
 *		IRunner を一つ管理し、スレッドで動かします。
 *
 *	@par使用例
 *		\code
 *
 *		class CMyThread : public CThread::IRunner
 *		{
 *		public:
 *			void Start(void)
 *			{
 *				m_thread.SetRunner(this);
 *				m_thread.Start();
 *			}
 *		protected:
 *			virtual DWORD Run(void)
 *			{
 *				while ( IsRunnable() )
 *				{
 *					;				//スレッドでの処理
 *				}
 *				return 0;
 *			};
 *			CThread m_thread;
 *		};
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbThread.h
 *
 *	@date 06/04/01 新規作成
 *	@date 06/04/21 Thread起動待ちに CCyclicBarrier を使うように変更
 *	@date 06/08/03 スレッド作成に _beginthread を使用するようにした。
 *	@date 06/09/08 Stop時、既にスレッドがない場合も考慮。
 *	@date 06/09/19 同期に Exchanger を使うようにした。
 *	@date 09/09/10 CThreadStatus 部分を分離。 Resume(), Suspend() 追加。
 *	@date 10/01/12 Terminate() 追加。
 *	@date 19/06/10 Terminate() 修正。ハンドルをNULLにし忘れていた。
 */
class CThread : public CThreadStatus, CCopyImpossible
{
	DEFSUPER(CThreadStatus);
public:

	/**@ingroup THREAD
	 * スレッド実行管理ランナーインターフェース
	 *
	 *		本クラスを継承したものを CThread クラスに委譲することでスレッドを容易に、
	 *		実現できます。継承したクラスはスレッドで動かした処理を Run() に実装します。\n
	 *		Stop要求があると、 IsRunnable() の戻り値がfalseになります。
	 *		Run() 内では IsRunnable() をポーリングし 戻り値が false になった時、 Run() 処理を終了し、リターンする必要が有ります。
	 *
	 *	@see CThread::CThread()
	 *	@see CThread::SetRunner()
	 *
	 *	@par必要ファイル
	 *			TnbThread.h
	 *
	 *	@date 06/04/01	新規作成
	 *	@date 06/07/26	PostMessage() 追加。
	 *	@date 08/10/09	OnStartThread() 追加。
	 *	@date 09/09/11	サスペンド要求追加。 OnSetupThread() 追加。 OnStartThread() 廃止。
	 *	@date 10/03/19	OnExitThread() 追加。
	 */
	struct IRunner
	{
		/** デストラクタ */
		virtual ~IRunner(void) {}

		/** コンストラクタ */
		IRunner(void) : m_canRunnable(true), m_isOnDemandSuspend(false)
		{
		}

		/**
		 * [確認] 実行可能か否か
		 *	@retval true	実行可能。
		 *	@retval false	実行停止要求。Run関数は直ちにReturnすること。
		 */
		bool IsRunnable(void) const
		{
			if ( m_isOnDemandSuspend )
			{
				m_isOnDemandSuspend = false;
				::SuspendThread(::GetCurrentThread());
			}
			return m_canRunnable;
		}

	protected:

		/** 
		 * [動作] スレッド処理本体
		 *	@note この関数にスレッドの処理を記述します。
		 *		ループで IsRunnable() をコールし、偽なら関数をリターンするように、
		 *		する必要が有ります。
		 *	@return 戻り値。 CThread::GetThreadResult() で得られる値です。
		 */
		virtual DWORD Run(void) = 0;

		/**
		 * [通知] スレッド開始前通知.
		 *		CThread::Start() 実行時、リターンする前にこのメソッドが新規スレッド上でコールされます。
		 */
		virtual void OnSetupThread(void) {}

		/**
		 * [通知] スレッド終了通知.
		 *		スレッドが終了時、コールされます。
		 */
		virtual void OnExitThread(void) {}

	private:
		volatile bool		m_canRunnable;		///< 実行可能か否か
		mutable bool		m_isOnDemandSuspend;///< サスペンド要求
		CExchangerT<BYTE>	m_excThreadReady;	///< スレッド開始の同期用
		friend class CThread;					///< フレンドクラス宣言
	};


	//---------------------------


	/// コンストラクタ
	CThread(void) : m_pRunner(NULL), m_lastResult(0)
	{
	}

	/** 
	 * デストラクタ
	 *	@note スレッドが生きている場合、停止させます。
	 */
	virtual ~CThread(void)
	{
		Stop();
	}

	/**
	 * [設定] ランナー、設定
	 *	@note スレッドが生きている場合、エラーになります。
	 *	@param pRunner ランナー
	 *	@retval true	成功
	 *	@retval false	失敗(スレッドが生きている)
	 */
	bool SetRunner(IRunner* pRunner)
	{
		if ( IsAlive() )
		{
			return false;
		}
		m_pRunner = pRunner;
		return true;
	}

	/**
	 * [設定] 関連ウィンドウハンドル設定.
	 *		スレッド終了時にポンプするウィンドウハンドルを指定します。
	 *	@note スレッド内で、 SendMessage() を使う処理がある場合、停止時デットロックする可能性があります。
	 *		これは、終了待ち中、ポンプが停止するために発生します。
	 *		よって、スレッド内で、 SendMessage() を使う処理がある場合、本メソッドでメインウィンドウの
	 *		ハンドルを指定しておく必要があります。
	 *	@param hOwner ウィンドウハンドル
	 */
	void SetOwnerHwnd(HWND hOwner)
	{
		_super::SetOwnerHwnd(hOwner);
	}

	/**
	 * [取得] スレッド終了値
	 *	@note スレッド中や一度も実行していない場合、値は０です。
	 *	@return 終了値
	 */
	DWORD GetThreadResult(void) const
	{
		return m_lastResult;
	}

	/**
	 * [設定] スレッド中断
	 *	@attention 通常、スレッド内に終了させる機能を用意すべきです。
	 *	@param result スレッド終了値
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Terminate(DWORD result = 0)
	{
		BOOL r = ::TerminateThread(m_hThread, result);
		_GetLastError("TerminateThread");
		if ( r )
		{
			m_hThread = NULL;
		}
		return !! r;
	}

	/**
	 * [設定] スレッド停止要求.
	 *		スレッドに対して停止要求します。
	 *	@note スレッドが終了するのを待ちません。
	 *	@attention 多用すべきではありません。使用する場合、{@link IRunner::OnExitThread() 終了通知} で終了を確認するようにしてください。
	 */
	void StopRequest(void)
	{
		if ( IsAlive() && m_pRunner->m_canRunnable )
		{
			#ifdef _DEBUG
				if ( ! m_threadName.IsEmpty() )
				{
					TRACE3( TNB_THREADTRACETYPE, m_threadId, m_threadName.Ref(), _T("停止要求") );
				}
			#endif
			//停止要求
			::ResumeThread(m_hThread);
			SetPriority(THREAD_PRIORITY_NORMAL);
			m_pRunner->m_canRunnable = false;
		}
	}

	/**
	 * [設定] スレッド停止
	 *		スレッドに対して停止要求します。
	 *	@note スレッドが終了するかタイムアウトするまで、待ちます。
	 *	@param dwWait スレッド終了要求してから待つ時間。
	 *			０なら永久に待ち、省略すると15秒待ちます。
	 *			それ以上たっても終わらないとTerminateします。
	 *	@retval true 成功（スレッドを開始していない時やTerminateした時も成功としています）
	 *	@retval false 失敗（リスナースレッド内で Stop() を使用すると失敗します）
	 */
	bool Stop(DWORD dwWait = 15000)
	{
		if ( m_threadId == ::GetCurrentThreadId() )
		{
			ASSERT0( false, "CThread::Stop()", "リスナースレッド処理内で Stop() は使用できません。" );
			return false;
		}
		bool boRc = IsAlive();
		if ( boRc && m_hThread != NULL )
		{
			DWORD dwExitCode;
			if ( ! ::GetExitCodeThread(m_hThread, &dwExitCode) )
			{
				boRc = false;
			}
			else if ( dwExitCode != STILL_ACTIVE )
			{
				#ifdef _DEBUG
					if ( ! m_threadName.IsEmpty() )
					{
						TRACE3( TNB_THREADTRACETYPE, m_threadId, m_threadName.Ref(), _T("既に停止...") );
					}
				#endif
				boRc = false;
			}
		}
		if ( boRc )
		{
			#ifdef _DEBUG
				if ( ! m_threadName.IsEmpty() )
				{
					TRACE3( TNB_THREADTRACETYPE, m_threadId, m_threadName.Ref(), _T("停止要求") );
				}
				#if ( _WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400 )
				if ( ::IsDebuggerPresent() )
				{
					dwWait = 0;
				}
				#endif
			#endif
			//停止要求
			::ResumeThread(m_hThread);
			SetPriority(THREAD_PRIORITY_NORMAL);
			m_pRunner->m_canRunnable = false;
			//
			if ( dwWait == 0 )
			{
				dwWait = DWORD_MAX;
			}
			else
			{
				dwWait /= 50;
			}
			//スレッド終了待ち
			for ( DWORD i = 0; i < dwWait; i++ )
			{
				m_pRunner->m_canRunnable = false;
				if ( m_hThread == NULL )
				{
					break;
				}
				DWORD dwExitCode;
				if ( ::GetExitCodeThread(m_hThread, &dwExitCode) )
				{
					if ( dwExitCode != STILL_ACTIVE )
					{
						break;
					}
				}
				if ( m_hOwnerWnd != NULL )
				{
					PumpMessage(m_hOwnerWnd);
				}
				::Sleep(50);
			}
			if ( m_hThread != NULL )
			{
				//終わらなかったらTerm....
				if ( ! ::TerminateThread(m_hThread, 0) )
				{
					_GetLastError("TerminateThread");
				}
				TRACE0("ThreadをTerminateしました\n");
				#ifdef _DEBUG
					CWorkMemT<TCHAR> str(1024);
					if ( ! m_threadName.IsEmpty() )
					{
						STRLIB::PrintF(str, 1024, _T("指定時間内に、") _T(TNB_THREADNAMEFRM2) _T("が終了しませんでした。\n          強制終了させました。")
							,m_threadId, m_threadName.Ref());
					}
					else
					{
						STRLIB::PrintF(str, 1024, _T("指定時間内に、") _T(TNB_THREADNAMEFRM1) _T("が終了しませんでした。\n          強制終了させました。")
							,m_threadId);
					}
					ASSERT1( false, "CThread::Stop()", "%s", str.Ref() );
				#endif
			}
		}
		if ( m_hThread != NULL )
		{
			il_CloseThreadHandle(m_hThread);
			m_hThread = NULL;
		}
		return true;
	}

	/**
	 * [設定] スレッド開始
	 *	@param lpszName スレッドの名前（DEBUG以外では無視されます）
	 *	@retval true 成功
	 *	@retval false 失敗(ランナーが未登録/スレッド起動失敗/既にスレッド実行中)
	 */
	bool Start(LPCTSTR lpszName = NULL)
	{
		if ( m_pRunner == NULL || IsAlive() )
		{
			return false;
		}
		#ifdef _DEBUG
			m_threadName = lpszName;
		#endif
		//
		m_hThread = il_CreateThread(ms_ThreadMain, this, &m_threadId);
		if ( m_hThread == NULL )
		{
			// スレッド起動失敗
			TRACE0( "スレッド起動失敗");
			return false;
		}
		DWORD r = ::ResumeThread(m_hThread);
		ASSERT( ToInt(r) != -1 );
		IgnoreUnusedValue(r);
		m_pRunner->m_excThreadReady.Exchange(0/*ダミー*/);
		return true;
	}

	/**
	 * [実行] サスペンド.
	 *	@note このサスペンドは、 IRunneble::IsRunnable() の中で、Suspend します。
	 *		そのためサスペンドのタイミングをコントロールすることが出来ます。
	 */
	void Suspend(void)
	{
		if ( m_pRunner != NULL )
		{
			m_pRunner->m_isOnDemandSuspend = true;
		}
	}

	/**
	 * [実行] レジューム.
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool Resume(void)
	{
		int r = ToInt(::ResumeThread(m_hThread));
		while ( r > 0 )
		{
			r = ToInt(::ResumeThread(m_hThread));
		}
		return r == 0;
	}

	/**
	 * [処理] メッセージポンプ.
	 *	@note 入力系のメッセージは破棄します。
	 *	@param hWnd ウィンドウハンドル
	 */
	static void PumpMessage(HWND hWnd)
	{
		MSG msg;
		while ( ::PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE) )
		{
			if ( ! ::GetMessage(&msg, hWnd, 0, 0) )
			{
				break;
			}
			bool r = (WM_KEYFIRST <= msg.message && msg.message <= WM_IME_KEYLAST);
			r |= (WM_MOUSEFIRST <= msg.message && msg.message <= WM_MOUSELAST);
			if ( r )
			{
				TRACE1("Drop Message = %d\n", msg.message);
			}
			else
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

private:
	IRunner*			m_pRunner;			///< ランナー
	DWORD				m_lastResult;		///< スレッド終了値
	CSimpleStr			m_threadName;		///< スレッド名(Debug用)

	/** 
	 * スレッド開始用
	 *		ランナーのRunを呼び出します。
	 *	@param lpPoint thisポインタ
	 *	@retval 0、固定
	 */
	static DWORD WINAPI ms_ThreadMain(LPVOID lpPoint)
	{
		CThread* P = static_cast<CThread*>(lpPoint);
		ASSERTLIB(P->m_threadId == ::GetCurrentThreadId());
		P->m_pRunner->m_canRunnable = true;
		P->m_pRunner->OnSetupThread();
		P->m_pRunner->m_excThreadReady.Exchange(0/*ダミー*/);
		#ifdef _DEBUG
			if ( ! P->m_threadName.IsEmpty() )
			{
				TRACE3( TNB_THREADTRACETYPE, P->m_threadId, P->m_threadName.Ref(), _T("開始"));
			}
		#endif
		P->m_lastResult = P->m_pRunner->Run();
		#ifdef _DEBUG
			if ( ! P->m_threadName.IsEmpty() )
			{
				TRACE3( TNB_THREADTRACETYPE, P->m_threadId, P->m_threadName.Ref(), _T("終了"));
			}
		#endif
		P->m_pRunner->OnExitThread();
		il_CloseThreadHandle(P->m_hThread);
		P->m_hThread = NULL;
		P->m_threadId = 0;
		il_ExitThread(P->m_lastResult);
		return P->m_lastResult;
	}
};



/**@ingroup THREAD
 * 簡易スレッド実行クラス
 *
 *		IRunner を渡すことで、スレッドを起動します。
 *		IRunner が終了したタイミングで、メモリも開放することも出来ます。
 *
 *	@note 本クラスはインスタンスは作らず、 Start() クラスメソッドを使います。
 *
 *	@attention	スレッドの状態や監視もしません。
 *				プロセスが終了する前にスレッドが終わっているようにしてください。
 *
 *	@par使用例
 *		\code
 *			//宣言
 *			struct IFooRunner : public CThreadlet::IRunner
 *			{
 *				virtual void Run(LPARAM lParam)
 *				{
 *					::MessageBox(NULL, _T("ABC"), _T("XYZ"), MB_OK);		
 *				};
 *			};
 *			
 *			//実行
 *			CThreadlet::Start(12, new IFooRunner);
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbThread.h
 *
 *	@date 06/05/01 新規作成
 *	@date 06/08/03 スレッド作成に _beginthread を使用するようにした。
 *	@date 06/09/19 同期に Exchanger を使うようにした。
 *	@date 09/12/08 Start の引数、追加。
 *	@date 10/01/14 プライオリティの設定方法を明記。
 */
class CThreadlet
{
public:

	/**@ingroup THREAD
	 * 簡易スレッド実行クラスのランナー
	 *
	 *	@see CThreadlet
	 *
	 *	@par必要ファイル
	 *			TnbThread.h
	 *
	 *	@date 06/05/01 新規作成
	 */
	struct IRunner
	{
		/// デストラクタ
		virtual ~IRunner(void) {}
		/**
		 * [通知] スレッド処理部
		 *	@note ここにスレッドの処理を実装します。
		 *	@param lParam Start の値が渡されます。
		 */
		virtual void Run(LPARAM lParam) = 0;
	};

	/**
	 * [設定] スレッド開始
	 *	@param lParam LPARAM 。
	 *	@param piRunner ランナー。withDelete が true なら必ず new したものを渡します。
	 *	@param withDelete true ならスレッド終了時に piRunner を delete します。
	 *	@param priority 優先度。 CThreadStatus::GetPriority() の戻り値を参照。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool Start(LPARAM lParam, IRunner* piRunner, bool withDelete = true, int priority = THREAD_PRIORITY_NORMAL)
	{
		CThreadlet* P = new CThreadlet;
		P->m_pRunner = piRunner;
		P->m_lParam = lParam;
		P->m_withDelete = withDelete;
		//
		DWORD dwThreadId;
		HANDLE hThread = il_CreateThread(ms_ThreadMain, P, &dwThreadId);
		if ( hThread == NULL )
		{
			// スレッド起動失敗
			TRACE0( "スレッド起動失敗");
			if ( P->m_withDelete )
			{
				delete P->m_pRunner;
			}
			delete P;
			return false;
		}
		::SetThreadPriority(hThread, priority);
		DWORD r = ::ResumeThread(hThread);
		ASSERT(ToInt(r) != -1);
		IgnoreUnusedValue(r);
		il_CloseThreadHandle(hThread);
		P->m_excThreadReady.Exchange(0/*ダミー*/);
		return true;
	}

private:

	CExchangerT<BYTE>	m_excThreadReady;	///< スレッド開始の同期用
	IRunner*			m_pRunner;			///< ランナー
	bool				m_withDelete;		///< 終了時オブジェクトをDeleteするか？
	LPARAM				m_lParam;			///< LPARAM
	/** 
	 * [動作] スレッド開始用
	 *		ランナーのRunを呼び出します。
	 *	@param lpPoint thisポインタ
	 *	@retval 0、固定
	 */
	static DWORD WINAPI ms_ThreadMain(LPVOID lpPoint)
	{
		CThreadlet* P = static_cast<CThreadlet*>(lpPoint);
		P->m_excThreadReady.Exchange(0/*ダミー*/);
		P->m_pRunner->Run(P->m_lParam);
		::Sleep(200);
		if ( P->m_withDelete )
		{
			delete P->m_pRunner;
		}
		delete P;
		il_ExitThread(0);
		return 0;
	}

	/// コンストラクタ
	CThreadlet(void) {}
};



/**@ingroup THREAD
 * 一時的スレッド優先度クラス
 *
 *		一区画だけスレッドの優先度を上げる(変えたい)時に使用します。
 *
 *	@par必要ファイル
 *			TnbThread.h
 *
 *	@date 10/09/06 新規作成
 */
class CTemporarilyThreadPriority
{
public:
	
	/**
	 * コンストラクタ.
	 *		現在のスレッドの優先度を扱います。
	 *	@param priority 優先度。 CThreadStatus::GetPriority() の戻り値を参照。
	 *			省略すると、 THREAD_PRIORITY_TIME_CRITICAL になります。
	 */
	CTemporarilyThreadPriority(int priority = THREAD_PRIORITY_TIME_CRITICAL)
		: m_hThread(::GetCurrentThread())
	{
		m_Init(priority);
	}

	/**
	 * コンストラクタ.
	 *		指定のスレッドの優先度を扱います。
	 *	@param hThread 対象のスレッド.
	 *	@param priority 優先度。 CThreadStatus::GetPriority() の戻り値を参照。
	 *			省略すると、 THREAD_PRIORITY_TIME_CRITICAL になります。
	 */
	CTemporarilyThreadPriority(HANDLE hThread, int priority = THREAD_PRIORITY_TIME_CRITICAL) 
		: m_hThread(hThread)
	{
		m_Init(priority);
	}
	
	/**
	 * デストラクタ.
	 *	@note Restore() していない場合、 Restore() をコールします。
	 */
	~CTemporarilyThreadPriority(void)
	{
		Restore();
	}

	/**
	 * [処理] リストア.
	 *		スレッドの優先度を元に戻します。
	 */
	void Restore(void)
	{
		if ( m_hThread != NULL )
		{
			::SetThreadPriority(m_hThread, m_priority);
			m_hThread = NULL;
		}
	}

private:
	void m_Init(int p)
	{
		m_priority = ::GetThreadPriority(m_hThread);
		::SetThreadPriority(m_hThread, p);
		ASSERT( ::GetThreadPriority(m_hThread) == p );
	}
	HANDLE	m_hThread;		///< 対象のスレッド
	int		m_priority;		///< 元の優先度
};



}; // TNB



