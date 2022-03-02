#pragma once
/**
 *@file
 * バッファリングレポーター関係のヘッダ
 *
 *		IReport からの読み出しをバッファリングし、受信処理を軽くします。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAbstractReport.h"
#include "TnbThread.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * バッファリングレポータークラス
 *
 *
 *
 *	@par必要ファイル
 *			TnbBufferingReporter.h
 *
 *	@date 12/03/13	新規作成
 */
class CBufferingReporter : public CAbstractReport
{
public:

	/**
	 * コンストラクタ
	 *	@param queueSize バッファキューの数。省略すると10000です。
	 */
	explicit CBufferingReporter(int queueSize = 10000) : m_in(queueSize)
	{
	}

	/**
	 * [設定] 環境設定.
	 *		使用するレポートを指定します。必ず指定する必要があります。
	 *	@param pReport レポート。
	 */
	void SetEnvironment(IReport* pReport)
	{
		m_in.pReport = pReport;
	}
	
	/**
	 * [登録] 共通 Listner 登録.
	 *	@note 登録できる IReport::IListener は一つだけです。
	 *	@param piCommListener リスナー。省略すると解除になります。
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL)
	{
		m_in.pListener = piCommListener;
	}
	
	/**
	 * [取得] レポート名.
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const
	{
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->GetReportName();
	}

	/**
	 * [取得] レポート ID.
	 *		種類の識別値を返します。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->GetReportID();
	}

	/**
	 * [確認] Aliveチェック.
	 *		Open() メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const
	{ 
		ASSERT( m_in.pReport != NULL );
		return m_thread.IsAlive() && m_in.pReport->IsAlive();
	}

	/**
	 * [確認] 接続されているか
	 *	@retval true	接続。
	 *	@retval	false	未接続。
	 */
	virtual bool IsConnect(void) const 
	{
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->IsConnect(); 
	}

	/** 
	 * [設定] アイドルイベント設定
	 *	@note	受信がない時、指定の間隔でコールバックを発生させることができます。
	 *			開始後、あるいは受信処理後、指定ms秒後にコールバックが始まります。
	 *			Open中に有無の切り替えは可能です。
	 *	@note SetReceivePollingTiming() とICommunication のサブクラスの
	 *		{@link TNB::ICommunication::SetReceiveTimeout 受信待ち時間} の
	 *		設定に精度が影響されます。
	 *	@param iInterval Idleイベントをあげる間隔を指定します。０なら、Idleイベントなし。
	 */
	virtual void SetIdleEventMode(int iInterval = 0)
	{
		ASSERT( m_in.pReport != NULL );
		m_in.pReport->SetIdleEventMode(iInterval);
	}

	/**
	 * [操作] 通信レポート開始.
	 *	@note	使用している通信クラスで、Openできるように設定後、本メソッドを使用します。
	 *	@retval true 成功。
	 *	@retval false エラー。 ::GetLastError() で詳細取得可能（その値は SystemErrorToMessageText() で文字列化できます)。
	 */
	virtual bool Start(void)
	{
		if ( m_in.pReport != NULL )
		{
			if ( m_thread.IsAlive() )
			{
				//スレッド停止
				m_thread.Stop();
			}
			m_in.pReport->SetListener(&m_in);
			if ( m_in.pReport->Start() )
			{
				m_thread.SetRunner(&m_in);
				m_in.event.ToLock();
				m_in.canRunnable = true;
				#ifdef _DEBUG
					m_thread.Start(CStr::Fmt(_T("BufferingReport[%s]:監視"), GetReportName()));
				#else
					m_thread.Start();
				#endif
				return true;
			}
		}
		return false;
	}

	/**
	 * [操作] 通信レポート停止.
	 *	@note 受信用スレッドも停止します。
	 */
	virtual void Stop(void)
	{
		ASSERT( m_in.pReport != NULL );
		m_in.canRunnable = false;
		m_in.event.Unlock();
		m_in.pReport->Stop();
		if ( m_thread.IsAlive() )
		{
			//スレッド停止
			m_thread.Stop();
		}
	}

	/**
	 * [送信] データ送信
	 *	@param dataSize 送信するデータ長。
	 *	@param lpcvData 送信するデータ。
	 *	@retval true	成功。
	 *	@retval	false	失敗。
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		ASSERT( m_in.pReport != NULL );
		return m_in.pReport->Send(dataSize, lpcvData);
	}

	/**
	 * [設定] パージ.
	 *	@note 通信のゴミを浄化
	 */
	virtual void Purge(void)
	{
		ASSERT( m_in.pReport != NULL );
		m_in.pReport->Purge();
	}

	/**
	 * [参照] 受信スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

private:
	/// 内部用クラス
	struct TInner : IReport::IListener, TNB::CThread::IRunner
	{
		DEFPARENTLISTENER(CBufferingReporter, IParentListener);

		IReport*						pReport;		///< 登録された IReport
		IParentListener*				pListener;		///< 登録されたリスナー
		CBlockingQueueT<CByteVector>	queue;			///< バッファキュー
		CSyncEvent						event;			///< イベント
		bool							canRunnable;	///< スレッド継続フラグ
		/// コンストラクタ
		TInner(int queueSize = 10000) : queue(queueSize), event(true, true), pReport(NULL), pListener(NULL)
		{
		}
		/// [通知] イベント発生
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
		/// スレッド
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
	TInner		m_in;		///< 内部情報管理
	CThread		m_thread;	///< スレッド
};



}; // TNB

