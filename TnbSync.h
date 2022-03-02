#pragma once
/**
 *@file
 * 同期処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbException.h"
#include "TnbTickCount.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



#include <malloc.h>



//TNB Library
namespace TNB
{



/**@ingroup SYNC
 * 排他基本インターフェース
 *	
 *		Lock と Unlock を持ちます。
 *
 *	@date 06/01/01	新規作成
 *	@date 06/08/23	メンバをconst宣言
 */
struct ISynchronized
{
	/** デストラクタ */
	virtual ~ISynchronized(void) {}

	/** 
	 * [排他] ロック
	 *	@param dwTime ロックにトライし続けるﾐﾘ秒。省略すると無限に待ちます。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const = 0;

	/** [排他] アンロック */
	virtual void Unlock(void) const = 0;
};



/**@ingroup SYNC
 * HANDLE型排他抽象クラス
 *
 *		ハンドルを使った排他の抽象クラスです。
 *		GetHandle() を実装すると、 ISynchronized::Lock() も実装されます。
 *
 *	@date 06/10/31	新規作成
 */
class CAbstractSyncHandle : public ISynchronized
{
public:

	/**
	 * [取得] ハンドル取得
	 *	@return ハンドル
	 */
	virtual HANDLE GetHandle(void) const = 0;

	/**
	 * [取得] ハンドル取得
	 *	@return ハンドル
	 */
	operator HANDLE(void) const 
	{
		return GetHandle(); 
	}
	
	/** 
	 * [排他] ロック
	 *	@param dwTime ロックにトライし続けるﾐﾘ秒。省略すると無限に待ちます。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		HANDLE h = GetHandle();
		if ( h != NULL )
		{
			DWORD dwRc = ::WaitForSingleObject(h, dwTime);
			ASSERTLIB(dwRc != WAIT_FAILED);
			if ( dwRc == WAIT_OBJECT_0 )
			{
				return true;//成功した
			}
		}
		return false;
	}
};


/**@ingroup SYNC
 * Section排他管理クラス
 *
 *		CriticalSectionを使った同期を実現します。
 *
 *	@note 同インスタンスのみで同期が取れます。
 *	@attention 同スレッドではロックできません。同一スレッドの場合ネストロックになります。
 *			ロックの数だけアンロックをしてください。
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_initializecriticalsection.asp
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/01/01	新規作成
 */
class CSyncSection : public ISynchronized, CCopyImpossible
{
public:

	/// コンストラクタ
	CSyncSection(void)
	{
		::InitializeCriticalSection(&m_cs); 
	}

	/** デストラクタ */
	virtual ~CSyncSection(void)
	{
		::DeleteCriticalSection(&m_cs); 
	}

	/** 
	 * [排他] ロック
	 *	@attention 他のクラスのLock()メソッドとは異なり、dwTimeは無視され、
	 *			INFINITEとして動作します。つまりどんな値を入れても、ロック
	 *			出来るまでリターンされません。
	 *	@param dwTime 値は無視されます。
	 *	@return true 固定
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		::EnterCriticalSection(&m_cs);
		return true;
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		::LeaveCriticalSection(&m_cs); 
	}

private:
	mutable CRITICAL_SECTION	m_cs;	///< 排他情報
	friend class CSyncSectionEx;		///< 仲間宣言
};



/**@ingroup SYNC
 * 拡張Section排他管理クラス
 *
 *		CriticalSectionを使った同期を実現します。
 *
 *	@note CSyncSection との差異は、 Lock() メソッドにおいて、
 *		時間を指定できる点です。		
 *
 *	@note 同インスタンスのみで同期が取れます。
 *	@attention 同スレッドではロックできません。同一スレッドの場合ネストロックになります。
 *			ロックの数だけアンロックをしてください。
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_initializecriticalsection.asp
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/07/18	新規作成
 *	@date 06/07/27	dwTime を有効にした。
 */
class CSyncSectionEx : public CSyncSection
{
	DEFSUPER(CSyncSection);	
public:

	/** コンストラクタ */
	CSyncSectionEx(void) : _super(), m_hModule(NULL), m_pTryEntryCs(NULL)
	{
	}

	/** デストラクタ */
	virtual ~CSyncSectionEx(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ロックにトライし続けるﾐﾘ秒。省略すると無限に待ちます。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		if ( dwTime == INFINITE )
		{
			return _super::Lock(dwTime);
		}
		if ( m_hModule == NULL )
		{
			m_hModule = ::LoadLibraryA("kernel32.dll");
			m_pTryEntryCs = reinterpret_cast<P_TryEntryCs>(::GetProcAddressA(m_hModule, "TryEnterCriticalSection"));
		}
		if ( m_pTryEntryCs == NULL )
		{
			ASSERT0(false, "CSyncSectionEx::Lock()", 
					"API 'TryEnterCriticalSection()' が見つかりません。\nOS がNT4.0以降でない可能性があります。");
			return false;
		}
		if ( dwTime == 0 )
		{
			return !! m_pTryEntryCs(&m_cs);	//TryEntryCriticalSection実行
		}
		CTickCount tick;
		while ( ! tick.IsPassed(dwTime) )
		{
			if ( m_pTryEntryCs(&m_cs) )	//TryEntryCriticalSection実行
			{
				return true;
			}
			::Sleep(1);
		}
		return false;
	}

private:
	typedef DWORD (WINAPI* P_TryEntryCs)(LPCRITICAL_SECTION);
	mutable P_TryEntryCs m_pTryEntryCs;			///< 「TryEntryCriticalSection」の関数アドレス
	mutable HMODULE			m_hModule;			///< モジュールハンドル
};



/**@ingroup SYNC
 * Mutex排他管理クラス
 *
 *		MUTEXを使った同期を実現します。
 *
 *	@note 別プロセスでも別インスタンスでも同じ名前なら同期が取れます（同じ排他オブジェクトとして動きます）。
 *	@attention 同スレッドではロックできません。同一スレッドの場合ネストロックになります。
 *			ロックの数だけアンロックをしてください。
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createmutex.asp
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/01/01 新規作成
 *	@date 06/10/31 抽象クラスを使用するように変更。
 *	@date 06/11/15 作成失敗したら スローするように変更。
 */
class CSyncMutex : public CAbstractSyncHandle , CCopyImpossible
{
	DEFSUPER(CAbstractSyncHandle);
public:

	/**
	 * コンストラクタ
	 *	@param boIsLock 初期状態。trueならLock状態、省略かfalseならUnlock状態で作成します。
	 *	@param lpszName MUTEXの名前。省略可能。
	 *	@throw CInvalidParamException 名前不正など、同期オブジェクトが作成できなかった時、スローされます。
	 */
	explicit CSyncMutex(bool boIsLock = false, LPCTSTR lpszName = NULL) : _super()
	{
		m_hMutex = ::CreateMutex(NULL, boIsLock, lpszName);
		DWORD r = ::GetLastError();
		if ( r != 0 && r != 0xB7 )
		{
			_GetLastError("CreateMutex");
		}
		ASSERT0(
			m_hMutex != NULL,
			"CSyncMutex::CSyncMutex()",
			"Mutexが初期化できませんした。"
		);
		if ( m_hMutex == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/**
	 * コンストラクタ
	 *	@param hMutex CreateMutex()で作成したハンドル
	 *	@throw CInvalidParamException NULLハンドルを指定した時、スローされます。
	 */
	explicit CSyncMutex(HANDLE hMutex) : _super()
	{
		m_hMutex = hMutex;
		if ( m_hMutex == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** デストラクタ */
	virtual ~CSyncMutex(void)
	{
		if ( m_hMutex != NULL )
		{
			::ReleaseMutex(m_hMutex);
			::CloseHandle(m_hMutex);
		}
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		if ( m_hMutex != NULL )
		{
			::ReleaseMutex(m_hMutex);
		}
	}

	/**
	 * [取得] Mutexハンドル取得
	 *	@return ハンドル
	 */
	virtual HANDLE GetHandle(void) const 
	{
		return m_hMutex; 
	}

private:
	HANDLE	m_hMutex;				///< 排他ハンドル
};



/**@ingroup SYNC
 * Semaphore排他管理クラス
 *
 *		Semaphoreを使った同期を実現します。
 *
 *	@note 別プロセスでも別インスタンスでも同じ名前なら同期が取れます。
 *
 *	@note 同スレッドでもロックできます（デッドロックに注意）。
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createsemaphore.asp
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/05/01	新規作成
 *	@date 06/09/19	引数つきのUnlockメソッド、追加。
 *	@date 06/10/31	抽象クラスを使用するように変更。
 *	@date 06/11/15	作成失敗したら スローするように変更。
 */
class CSyncSemaphore : public CAbstractSyncHandle , CCopyImpossible
{
	DEFSUPER(CAbstractSyncHandle);
public:

	/**
	 * コンストラクタ
	 *	@param lpszName 同期用の名前を指定します。
	 *					省略すると別インスタンス同士の同期は取れません。
	 *	@param iMaximumCount 同時に実行できる数を指定します。省略すると１です。
	 *					同じ名前がすでに作成されている場合、この値は無視されます。
	 *	@throw CInvalidParamException 名前不正など、同期オブジェクトが作成できなかった時、スローされます。
	 */
	explicit CSyncSemaphore(LPCTSTR lpszName = NULL, int iMaximumCount = 1) : _super()
	{
		m_hSema = ::CreateSemaphore(NULL, iMaximumCount, iMaximumCount, lpszName);
		ASSERT0(
			m_hSema != NULL,
			"CSyncSemaphore::CSyncSemaphore()",
			"Semaphoreが初期化できませんした。"
		);
		if ( m_hSema == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/**
	 * コンストラクタ
	 *	@param hSema CreateSemaphore()で作成したハンドル
	 *	@throw CInvalidParamException NULLハンドルを指定した時、スローされます。
	 */
	explicit CSyncSemaphore(HANDLE hSema) : _super()
	{
		m_hSema = hSema;
		if ( m_hSema == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** デストラクタ */
	virtual ~CSyncSemaphore(void)
	{
		if ( m_hSema != NULL )
		{
			::ReleaseSemaphore(m_hSema, 1, NULL);
			::CloseHandle(m_hSema);
		}
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		if ( m_hSema != NULL )
		{
			::ReleaseSemaphore(m_hSema, 1, NULL);
		}
	}

	/**
	 * [排他] アンロック
	 *	@param lCount アンロックする数
	 *	@return 現在の回数
	 */
	LONG Unlock(LONG lCount) const
	{
		LONG l = 0;
		if ( m_hSema != NULL )
		{
			::ReleaseSemaphore(m_hSema, lCount, &l);
		}
		return l;
	}

	/**
	 * [取得] Mutexハンドル取得
	 *	@return ハンドル
	 */
	virtual HANDLE GetHandle(void) const 
	{
		return m_hSema; 
	}

private:
	HANDLE	m_hSema;				///< 排他ハンドル
};



/**@ingroup SYNC
 * Event排他管理クラス
 *
 *		EVENTを使った同期を実現します。
 *
 *	@note 別プロセスでも別インスタンスでも同じ名前なら同期が取れます。
 *
 *	@note 同スレッドでもロックできます（デッドロックに注意）。
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createevent.asp
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/04/19	新規作成
 *	@date 06/09/13	コンストラクタの第一デフォルト引数を変更。
 *					Lock状態とシグナル状態の関係を誤っていた。
 *	@date 06/10/31 抽象クラスを使用するように変更。
 *	@date 06/11/15 作成失敗したら スローするように変更。
 */
class CSyncEvent : public CAbstractSyncHandle , CCopyImpossible
{
	DEFSUPER(CAbstractSyncHandle);
public:

	/**
	 * コンストラクタ
	 *	@param boIsManualReset falseか省略なら自動リセットオブジェクト、
							trueなら手動リセットオブジェクトになります。
	 *	@param boIsLock 初期状態。trueならLock状態、省略かfalseならUnlock状態で作成します。
	 *	@param lpszName Eventの名前。省略可能。
	 *	@throw CInvalidParamException 名前不正など、同期オブジェクトが作成できなかった時、スローされます。
	 */
	explicit 
	CSyncEvent(bool boIsManualReset = false, bool boIsLock = false, LPCTSTR lpszName = NULL) : _super()
	{
		m_hEvent = ::CreateEvent(NULL, boIsManualReset, ! boIsLock, lpszName);
		ASSERT0(
			m_hEvent != NULL,
			"CSyncEvent::CSyncEvent()",
			"Eventが初期化できませんした。名前に問題がある可能性が有ります。"
		);
		if ( m_hEvent == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/**
	 * コンストラクタ
	 *	@param hEvent CreateEvent()で作成したハンドル
	 *	@throw CInvalidParamException NULLハンドルを指定した時、スローされます。
	 */
	explicit CSyncEvent(HANDLE hEvent) : _super()
	{
		m_hEvent = hEvent;
		if ( m_hEvent == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** デストラクタ */
	virtual ~CSyncEvent(void)
	{
		if ( m_hEvent != NULL )
		{
			::SetEvent(m_hEvent);
			::CloseHandle(m_hEvent);
		}
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		if ( m_hEvent != NULL )
		{
			::SetEvent(m_hEvent);
		}
	}

	/**
	 * [排他] ロック状態にする
	 *	@note 待機はしません。ロックに成功しない可能性も有ります。
	 */
	void ToLock(void)
	{
		if ( m_hEvent != NULL )
		{
			::ResetEvent(m_hEvent);
		}
	}

	/**
	 * [排他] ロック状態にする
	 *	@note 一瞬アンロック状態にします。
	 */
	void Pulse(void)
	{
		if ( m_hEvent != NULL )
		{
			::PulseEvent(m_hEvent);
		}
	}

	/**
	 * [取得] Mutexハンドル取得
	 *	@return ハンドル
	 */
	virtual HANDLE GetHandle(void) const 
	{
		return m_hEvent; 
	}

private:
	HANDLE	m_hEvent;				///< 排他ハンドル
};



/**@ingroup SYNC
 * カウンタ式排他クラス
 *
 *		カウンタを使った排他クラスです。ブロックすることより、進入可能かチェックし、
 *		使用中であればリターンする、という排他を多用するのに向いています。
 *
 *	@note 同インスタンスのみで同期が取れます。
 *	@note 同スレッドでもロックできます（デッドロックに注意）。
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/09/20	新規作成
 */
class CSyncCounter : public ISynchronized, CCopyImpossible
{
public:

	/**
	 * コンストラクタ 
	 *	@param lMax 進入許可最大数。省略すると１です。0以下は指定しないで下さい。
	 */
	CSyncCounter(LONG lMax=1) : m_lCounter(0), m_lMax(lMax) 
	{
	}

	/** デストラクタ */
	virtual ~CSyncCounter(void)
	{
		ASSERT0(
			m_lCounter == 0,
			"CSyncCounter::~CSyncCounter()",
			"内部ロックカウントの数が合いません。\n\tLock()とUnlock()回数があっていない可能性が有ります。"
		);
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ロックにトライし続けるﾐﾘ秒。省略すると無限に待ちます。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		if ( dwTime == 0 )
		{
			return m_check();
		}
		else if ( dwTime == INFINITE )
		{
			while ( true )
			{
				if ( m_check() )
				{
					return true;
				}
				::Sleep(1);
			}
		}
		else
		{
			CTickCount tick;
			while ( ! tick.IsPassed(dwTime) )
			{
				if ( m_check() )
				{
					return true;
				}
				::Sleep(1);
			}
		}
		return false;
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		if ( ::InterlockedDecrement(&m_lCounter) < 0 )
		{
			ASSERT0(false, "CSyncCounter::Unlock()", "Unlock()回数がLock()よりも多いです。");
			::InterlockedExchange(&m_lCounter, 0);
		}
	}

private:
	mutable LONG	m_lCounter;	///< 現在のカウンタ
	LONG			m_lMax;		///< 進入可能数

	/// 進入チェック
	bool m_check(void) const
	{
		if ( ::InterlockedIncrement(&m_lCounter) <= m_lMax )
		{
			return true;
		}
		::InterlockedDecrement(&m_lCounter);
		return false;
	}
};



/**@ingroup SYNC
 * 排他管理ダミークラス
 *
 *		メソッドは用意されますが、処理は何もしません。
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/05/01	新規作成
 */
class CSyncDummy : public ISynchronized
{
public:

	/** コンストラクタ */
	CSyncDummy(void) {}

	/** デストラクタ */
	virtual ~CSyncDummy(void) {}

	/** 
	 * [排他] ロック
	 *	@param dwTime ダミー。
	 *	@return 常に true。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true; 
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const {}
};



/**@ingroup SYNC
 * 簡易排他制御クラス
 *
 *		コンストラクト時にロックし、デストラクト時にアンロックします。
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/01/01	新規作成
 *	@date 06/04/17	クラス名変更（CSync -> CExclusive）
 *	@date 06/07/27	Unlockメソッド追加
 *	@date 06/09/13	無駄を省いた
 *	@date 06/11/15 引数異常時スローするように変更。
 */
class CExclusive
{
public:

	/**
	 * コンストラクタ
	 *	@param P ISynchronizedを実装したクラスのポインタ
	 *	@throw CNullPointerException P に NULL を指定するとスローされます。
	 */
	explicit CExclusive(const ISynchronized* P) : m_psyncObj(P)
	{
		if ( m_psyncObj == NULL )
		{
			throw CNullPointerException();
		}
		m_psyncObj->Lock();
	}

	/** デストラクタ */
	~CExclusive(void)
	{
		m_psyncObj->Unlock(); 
	}

private:
	const ISynchronized *	m_psyncObj;	///< 使用する同期オブジェクト 
};



/**@ingroup SYNC
 * 簡易排他制御マクロ.
 *
 *		CLSに ISynchronized 派生のインスタンスを指定すると、そのスコープ間は、
 *		排他されます。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		class CFoo{
 *		  CSyncSection m_syc;
 *		          ;
 *		  void ab(void){
 *		    if ( XX )
 *		    {
 *		      EXCLUSIVE(m_syc); //この間、排他される。		
 *		            ;           //／
 *		    }
 *		  }
 *		};
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/04/17	新規作成
 */
#define EXCLUSIVE(CLS)		CExclusive _cExclus_(CLS);


/**@ingroup SYNC
 * TRACE付き簡易排他制御マクロ.
 *
 *		CLSに ISynchronized 派生のインスタンスを指定すると、そのスコープ間は、
 *		排他されます。
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/07/27	新規作成
 */
#define EXCLUSIVE_(CLS)	\
					TRACE3("%s(%d):[%d] Lock要求\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());	\
					CExclusive _cExclus_(CLS);														\
					TRACE3("%s(%d):[%d] Lock成功\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());



/**@ingroup SYNC
 * 簡易排他ツイン制御マクロ.
 *
 *		CLS1,CLS2 に ISynchronized 派生のインスタンスを指定すると、そのスコープ間は、
 *		排他されます。
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/07/03	新規作成
 */
#define EXCLUSIVE2(CLS1,CLS2)	CExclusive _cExclus1_(CLS1); CExclusive _cExclus2_(CLS2);



/**@ingroup SYNC
 * TRACE付き簡易排他ツイン制御マクロ.
 *
 *		CLS1,CLS2 に ISynchronized 派生のインスタンスを指定すると、そのスコープ間は、
 *		排他されます。
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/07/27	新規作成
 */
#define EXCLUSIVE2_(CLS1,CLS2)	\
					TRACE3("%s(%d):[%d] Lock要求\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());	\
					CExclusive _cExclus1_(CLS1); CExclusive _cExclus2_(CLS2);						\
					TRACE3("%s(%d):[%d] Lock成功\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());



/**@ingroup SYNC
 * 簡易排他制御クラス
 *
 *		インスタンス作成時、ロックし、デストラクタ時アンロックします。
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/01/01	新規作成
 *	@date 06/04/17	クラス名変更（CSync -> CExclusive）
 *	@date 06/07/27	Unlockメソッド追加
 */
class CExclusiveEx : public CExclusive
{
	DEFSUPER(CExclusive); 
public:

	/**
	 * コンストラクタ
	 *	@param P ISynchronizedを実装したクラスのポインタ
	 *	@throw CNullPointerException P に NULL を指定するとスローされます。
	 */
	explicit CExclusiveEx(const ISynchronized* P) : _super(P), m_iChkCnt(0) 
	{
	}

	/**
	 * カウンタインクリメント
	 *	@note SYNCBLOCK用の管理メソッドです。通常直接使いません。
	 *	@return カウンタ
	 */
	operator int(void)
	{
		return m_iChkCnt++;
	}

private:
	int			m_iChkCnt;	///< カウンタ
};



/**@ingroup SYNC
 * 簡易排他ブロック制御マクロ.
 *
 *		CLSに ISynchronized 派生のインスタンスを指定すると、この後に続くブロックは、
 *		排他されます。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		class CFoo{
 *		  CSyncSection m_syc;
 *		          ;
 *		  void ab(void){
 *		    int i=0;
 *		    SYNCBLOCK(syc)
 *		    {                //この間、排他される。		
 *		            ;        //／
 *		    }
 *		    i=3;
 *		  }
 *		};
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbSync.h
 *
 *	@date 06/05/01	新規作成
 */
#define SYNCBLOCK(CLS) if (false); else for ( CExclusiveEx _cExclusEx_(CLS); _cExclusEx_ == 0; )



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
