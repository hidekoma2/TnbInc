#pragma once
/**
 *@file
 * 同期処理関係のヘッダ
 *
 *		WaitableTimerを使った同期を実現するクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"



#if ( _WIN32_WINNT >= 0x0400 ) || ( _WIN32_WINDOWS > 0x0400 )
#else
 #error Reason "This class cannot be used by the target OS."
#endif



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup SYNC
 * WaitableTimer排他管理クラス
 *
 *		WaitableTimerを使った同期を実現します。
 *
 *	@note 多少他の ISynchronized を実装している排他クラスとは使用方法が異なります。
 *
 *	@note	定期的にアンロックを発生させる場合、タイミング(処理内容)によっては、
 *			二つ以上のスレッドが同時に動くことがあります（指定時間ごとに、アンロックになるため）。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		static CSyncWaitableTimer s_timer;
 *		DWORD WINAPI DoThread(DWORD ThreadCount)
 *		{
 *			for ( int i = 0; i < 5; i++) 
 *			{
 *			  EXCLUSIVE(&s_timer);
 *			  TRACE1("スレッド%d: 処理開始\n", ThreadCount);
 *			  Sleep(400*ThreadCount);
 *			  TRACE1("スレッド%d: 処理終了\n", ThreadCount);
 *			}
 *			return 0;
 *		}
 *		void Main()
 *		{
 *			HANDLE hThread[2];
 *			DWORD ThreadID[2];
 *			for ( int i = 0; i < 2; i++ ) 
 *			{
 *			  hThread[i] = CreateThread(
 *			        NULL, 0, (LPTHREAD_START_ROUTINE)DoThread,
 *			        (LPVOID)i, 0, &ThreadID[i]);
 *			}
 *			TRACE0("開始\n");
 *			s_timer.Start(1000,1000);
 *			WaitForMultipleObjects(2, hThread, TRUE, INFINITE);// スレッドの終了まで待機		
 *			CloseHandle(hThread[0]);
 *			CloseHandle(hThread[1]);
 *		}
 *	</pre></td></tr></table>
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createwaitabletimer.asp
 *
 *	@par必要ファイル 
 *			TnbSyncWaitableTimer.h
 *
 *	@date 06/09/21	新規作成
 *	@date 06/11/15 作成失敗したら スローするように変更。
 */
class CSyncWaitableTimer : public CAbstractSyncHandle, CCopyImpossible
{
	HANDLE	m_hTimer;		///< 排他ハンドル

public:

	/**
	 * コンストラクタ
	 *	@note 作成時は、ロック状態です。
	 *	@param boIsManualReset falseか省略なら自動リセットオブジェクト、
							trueなら手動リセットオブジェクトになります。
	 *	@param lpszName Eventの名前。省略可能。
	 *	@throw CInvalidParamException 名前不正など、同期オブジェクトが作成できなかった時、スローされます。
	 */
	explicit CSyncWaitableTimer(bool boIsManualReset = false, LPCTSTR lpszName = NULL)
	{
		m_hTimer = ::CreateWaitableTimer(NULL, boIsManualReset, lpszName);
		ASSERT0(
			m_hTimer != NULL,
			"CSyncWaitableTimer::CSyncWaitableTimer()",
			"WaitableTimerが初期化できませんした。名前に問題がある可能性が有ります。"
		);
		if ( m_hTimer == NULL )
		{
			throw CInvalidParamException();
		}
		::CancelWaitableTimer(m_hTimer);
	}

	/**
	 * コンストラクタ
	 *	@param hTimer CreateWaitableTimer()で作成したハンドル
	 */
	explicit CSyncWaitableTimer(HANDLE hTimer)
	{
		VERIFY( m_hTimer = hTimer );
	}

	/** デストラクタ */
	virtual ~CSyncWaitableTimer(void)
	{
		if ( m_hTimer != NULL )
		{
			::CloseHandle(m_hTimer);
		}
	}

	/**
	 * [取得] ハンドル取得
	 *	@return ハンドル
	 */
	virtual HANDLE GetHandle(void) const
	{
		return m_hTimer;
	}

	/**
	 * [排他] アンロック.
	 *	@note	本関数はダミーです。
	 *	@note	Startで指定したタイミングでアンロック状態が一瞬発生するため、手動ではアンロックできません。
	 */
	virtual void Unlock(void) const
	{
		// なにもしない
	}

	/**
	 * [排他] 開始
	 *	@param dwLagTime 開始までの時間(ms)
	 *	@param dwInterval 開始後、定期的にアンロックになる間隔(ms)。<BR>
	 *					０を指定すると、最初一回アンロック後、ずっとロック状態になります。
	 *					もう一度 Start をする必要があります。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Start(DWORD dwLagTime, DWORD dwInterval)
	{
		if ( m_hTimer != NULL )
		{
			LARGE_INTEGER li;
			li.QuadPart = dwLagTime;
			li.QuadPart *= -10000;
			return !! ::SetWaitableTimer(m_hTimer, &li, dwInterval, NULL, 0, FALSE);
		}
		return false;
	}

	/**
	 * [排他] ロック状態にする.
	 *	@note ロック状態に固定されます。
	 */
	void ToLock(void)
	{
		if ( m_hTimer != NULL )
		{
			::CancelWaitableTimer(m_hTimer);
		}
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")


#if 0
static CSyncWaitableTimer s_timer;

DWORD WINAPI DoThread(DWORD ThreadCount)
{
	for (int i=0;i<5;i++) 
	{
		EXCLUSIVE(&s_timer);
		TRACE1("スレッド%d: 処理開始\n", ThreadCount);
		Sleep(400*ThreadCount);
		TRACE1("スレッド%d: 処理終了\n", ThreadCount);
	}
	return 0;
}

void ____wt() 
{
	HANDLE hThread[2];
	DWORD ThreadID[2];
	/* スレッドを2つ作成 */
	for (int i=0; i<2; i++) 
	{
		hThread[i] = CreateThread(
					NULL, 0, (LPTHREAD_START_ROUTINE)DoThread,
					(LPVOID)i,0,&ThreadID[i]);
	}
	TRACE0("開始\n");
	s_timer.Start(1000,1000);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);// スレッドの終了まで待機
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	typedef LONG (WINAPI *P_InterlockedCompareExchange)(LONG*,LONG,LONG);
	typedef HANDLE (WINAPI *P_IcmpCreateFile)(VOID);
	HANDLE h = CDynamicFuncT<P_IcmpCreateFile>("ICMP.dll","IcmpCreateFile")()();

	CDynamicFuncT<P_InterlockedCompareExchange> func1("kernel32.dll","InterlockedCompareExchange");
	LONG l1 = 0;
	LONG l2 = func1()(&l1,1,0);
}
#endif
