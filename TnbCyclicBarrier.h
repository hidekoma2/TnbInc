#pragma once
/**
 *@file
 * バリア関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup SYNC
 * バリアクラス
 *
 *		指定の数（パーティ数）が Wait に入るまでバリアする（進行を阻む）クラスです。
 *
 *	@note パーティ数が２ならば、 CExchanger を使用したほうが、軽量です。
 *
 *	@par必要ファイル
 *			TnbCyclicBarrier.h
 *
 *	@date 06/04/20 新規作成
 */
class CCyclicBarrier : CCopyImpossible
{
public:

	/**@ingroup SYNC
	 * バリアクラスのリスナーインターフェース
	 *
	 *		定数のWaitがそろった時の通知用インターフェースです。
	 *
	 *	@see CCyclicBarrier::CCyclicBarrier()
	 *	@see CCyclicBarrier::Init()
	 *
	 *	@par必要ファイル
	 *			TnbCyclicBarrier.h
	 *
	 *	@date 06/04/20 新規作成
	 */
	struct IListener
	{
		/**デストラクタ*/
		virtual ~IListener(void) {}

		/** 
		 * [通知] そろった通知
		 *
		 *		定数のWaitがそろったときに実行されます。
		 *		この関数からリターンするまで、それぞれのWait()からリターンされません。
		 *	@note この関数が実行されるスレッドは一番最初に Wait()されたスレッドになります。
		 *			つまり、不定と考えて処理を実装してください。
		 */
		virtual void OnGathered(void) = 0;
	};

private:

	int				m_iParties;				///< パーティ数
	IListener*		m_piListener;			///< リスナー
	CSyncEvent*		m_psyncEvent;			///< EVENT配列用ポインタ
	LONG			m_lWaitingCount;		///< Wait開始スレッド数
	CSyncSection	m_syncParam;			///< パラメータ排他用
	CSyncSemaphore*	m_psyncSema;			///< Wait排他用
	LONG			m_lWaitExecutingCount;	///< Wait関数実行中カウンタ
	bool			m_boIsReset;			///< リセットフラグ

public:

	/**
	 * コンストラクタ
	 *	@param iParties パーティの数。省略すると２です。
	 *	@param I リスナー。省略できます。
	 */
	explicit CCyclicBarrier(int iParties = 2, IListener* I = NULL)
	{
		m_psyncEvent = NULL;
		m_psyncSema	= NULL;
		m_iParties = 0;
		m_piListener = NULL;
		m_boIsReset	= false;
		m_lWaitingCount = 0;
		m_lWaitExecutingCount = 0;
		Init(iParties, I);
	}

	/**
	 * デストラクタ
	 *	@note 待機中のWait()がある場合、少し処理がかかる可能性があります。
	 */
	~CCyclicBarrier(void)
	{
		Reset();
		while ( m_lWaitExecutingCount != 0 )
		{
			::Sleep(10); 
		}
		if ( m_psyncSema != NULL )
		{
			delete m_psyncSema;
		}
		if ( m_psyncEvent != NULL )
		{
			delete[] m_psyncEvent;
		}
	}
	
	/**
	 * [設定] 初期化
	 *	@note 待機中のWait()がある場合、少し処理がかかる可能性があります。
	 *	@note 待機中のWait()は -1 でリターンします。
	 *	@param iParties パーティの数（2以上を指定)。省略すると２です。
	 *	@param I リスナー。省略できます。
	 *	@retval true 成功。
	 *	@retval false 失敗（iParties の値が２未満が指定されている）。
	 */
	bool Init(int iParties = 2, IListener* I = NULL)
	{
		if ( iParties < 2 )
		{
			ASSERT0(false, "CCyclicBarrier::Init()", "パーティ数が異常です。");
			return false;
		}
		Reset();
		while ( m_lWaitExecutingCount != 0 )
		{
			::Sleep(10); 
		}
		if ( m_psyncSema != NULL )
		{
			delete m_psyncSema;
		}
		m_psyncSema = new CSyncSemaphore(NULL, iParties);
		//
		if ( m_iParties != iParties )
		{
			if ( m_psyncEvent != NULL )
			{
				delete[] m_psyncEvent;
			}
			m_psyncEvent = new CSyncEvent[iParties];
			m_iParties = iParties;
		}
		m_piListener = I;
		loop ( i, m_iParties )
		{
			m_psyncEvent[i].ToLock();
		}
		m_lWaitingCount = 0;
		return true;
	}

	/**
	 * [操作] リセット
	 *	@note Wait処理中のスレッドがある場合、少し処理がかかる可能性があります。
	 *	@note 待機中の Wait()は -1 でリターンします。
	 */
	void Reset(void)
	{
		m_boIsReset = true;
		for ( int i = m_iParties - 1; i >= 0; i-- )
		{
			m_psyncEvent[i].Unlock();
		}
	}

	/**
	 * [排他] ウェイト
	 *	@note すべてのパーティがWaitを呼び出すまで待機します。
	 *	@retval ０以上 到着インデックス
	 *	@retval ０未満 エラー
	 */
	int Wait(void)
	{
		::InterlockedIncrement(&m_lWaitExecutingCount);
		LONG lNo = 0;
		int iRc = 0;
		SYNCBLOCK(m_psyncSema)
		{
			SYNCBLOCK(&m_syncParam)
			{
				lNo = m_lWaitingCount++;
				if ( lNo == 0 )
				{
					//== TopWaiter
					m_boIsReset = false;
					loop ( i, m_iParties )
					{
						m_psyncEvent[i].ToLock();
					}
				}
			}
			//
			if ( lNo == 0 )
			{
				//== TopWaiter
				for ( int i = 1; i < m_iParties; i++ )
				{
					m_psyncEvent[i].Lock();	//他のWaiter待ち
				}
				if ( ! m_boIsReset )
				{
					EXCLUSIVE(&m_syncParam);
					if ( m_piListener != NULL )
					{
						m_piListener->OnGathered();
					}
					m_psyncEvent[0].Unlock();
					m_lWaitingCount = 0;
					iRc = 0;
				}
				else
				{
					iRc = -1;
				}
				break;
			}
			else if ( m_iParties > lNo )
			{
				//== 後続のWaiter
				m_psyncEvent[lNo].Unlock();
				m_psyncEvent[0].Lock();	// TopWaiter待ち
				iRc = (m_boIsReset) ? -1 : lNo;
				break;
			}
			else
			{
				ASSERTLIB(false);	//こないはず
			}
		} 
		::InterlockedDecrement(&m_lWaitExecutingCount);
		return iRc;
	}

	/**
	 * [取得] 待機数
	 *	@return Wait()で待っている数
	 */
	int GetNumberWaiting(void) { return m_lWaitingCount; }

	/**
	 * [取得] パーティ数
	 *	@return パーティの数
	 */
	int GetParties(void) { return m_iParties; }
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
