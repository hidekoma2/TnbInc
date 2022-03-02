#pragma once
/**
 *@file
 * キューイングレポート関係のヘッダ
 *
 *		IReport からの読み出しをキューイング処理することで、解析処理を行いやすくします。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbReport.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * キューイングレポートクラス
 *
 *		IReport から読みだしたデータをキューイングしています。そのデータを IListener で取得することが可能です。
 *		キューイングしているデータのため、解析が容易になります。
 *
 *	@note 出力はキューイングサポートしません。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		class CFoo : private CQueueingReporter::IListener
 *		{
 *		public:
 *			/// オープン.
 *			bool Open(IReport* pReport)
 *			{
 *				m_queue.SetAliveTime(100); //100ms 経ってもパケットがそろわないのは破棄
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
 *				// (c を解析)
 *				if ( r == 0 )
 *				{
 *					return 0; // 解析するにはデータが足りない
 *				}
 *				if ( r < 0 )
 *				{
 *					return 1; // ヘッダ情報がおかしいので 1byte 捨てる
 *				}
 *				// (パケット確認)
 *				return r; // 解析したパケット数を返す
 *			}
 *		};
 *
 *	</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbQueueingReporter.h
 * 
 *	@date 08/09/09	新規作成
 *	@date 09/02/19	IsAlive(), IsConnect(), Send() を追加。
 *	@date 09/03/25	Purge() を追加。
 *	@date 09/03/31	CQueueingReporter に改名。
 *	@date 09/04/09	キューの有効時間を設定可能に。
 *	@date 11/07/14	SetAliveTime() の時間の意味を変更
 *	@date 11/09/20	IListener::OnReportDustyData() 追加。
 *	@date 15/02/20	SetTearminalTime() を新規。時間で終端を決めるモードを追加。
 */
class CQueueingReporter : CCopyImpossible
{
public:

	/**@ingroup REPORT
	 * キューイングレポートのリスナーインターフェース
	 * 
	 *	@see CQueueingReporter::SetEnvironment()
	 *
	 *	@par必要ファイル
	 *			TnbQueueingReporter.h
	 *
	 *	@date 08/09/09	新規作成
	 *	@date 11/09/20	OnReportDustyData() 追加。
	 */
	struct IListener
	{
		/**デストラクタ*/
		virtual ~IListener(void) {}

		/**
		 * [通知] イベント発生通知
		 *	@note イベントが発生した時にコールされます。
		 *	@param ev 受信した内容（イベントのみ）。
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnReportEvent(const CReportEvent& ev) = 0;

		/**
		 * [通知] データ受信通知
		 *	@note データを受信した時にコールされます。データを解析し、応答してください。
		 *	@param[in] boIsLast true ならクローズされたため、最後のデータであることを表す。
		 *	@param[in] c 変換元RAWデータ(受信データ)。
		 *	@return 処理したデータ数
		 */
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c) = 0;

		/**
		 * [通知] ゴミデータ化通知.
		 *		キューに CQueueingReporter::SetAliveTime() で指定した時間、溜まっていると破棄します。その破棄したデータを通知します。
		 *	@param b 破棄されたデータ
		 */
		virtual void OnReportDustyData(BYTE b) {}
	};

	/**
	 * コンストラクタ
	 *	@param iQueueSize 受信Byteキューのサイズ。省略すると10000です。
	 */
	explicit CQueueingReporter(int iQueueSize = 10000) : m_in(iQueueSize)
	{
	}

	/// デストラクタ
	~CQueueingReporter(void)
	{
		ResetEnvironment();
	}

	/**
	 * [設定] 環境設定
	 *	@note 本クラスのインスタンスとレポーターのインスタンスを同じスコープにおく場合、 
	 *		レポーターのほうを上に宣言しておくこと（先に構築され、後に破棄されるため）。
	 *	@param pReport レポート。
	 *	@param pListener リスナー。
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

	/// [設定] 環境リセット
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
	 * [設定] キュー有効時間設定.
	 *		キュー格納後、一定時間受信がない場合、キュー内容を破棄することが出来ます。
	 *	@note SetTerminalTime() と同時に指定できません。
	 *	@param time キューの有効時間(ms)。最後に受信して解析してから指定時間経過したら、キューの先頭の一バイトを破棄して、
	 *					再度解析をします。省略時は無効( DWORD_MAX )です。
	 */
	void SetAliveTime(DWORD time = DWORD_MAX)
	{
		m_in.m_aliveTime = time;
		m_in.m_tearminalTime = DWORD_MAX;
	}

	/**
	 * [設定] 終端時間設定.
	 *		キュー格納後、一定時間受信がない場合にキュー内容を解析するようにします。
	 *	@note SetAliveTime() と同時に指定できません。
	 *	@param time 終端時間(ms)。
	 */
	void SetTearminalTime(DWORD time = DWORD_MAX)
	{
		m_in.m_tearminalTime = time;
		m_in.m_aliveTime = DWORD_MAX;
	}

	/**
	 * [確認] Aliveチェック.
	 *		各サブクラスの開始メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
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
	 * [確認] 接続チェック
	 *	@retval true	接続中。
	 *	@retval	false	切断。
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
	 * [送信] データ送信.
	 *	@note 単にレポータに送信依頼するだけです。
	 *	@param dataSize 送信するデータ長。
	 *	@param lpcData 送信するデータ。
	 *	@retval true	成功
	 *	@retval	false	失敗
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
	 * [消去] キューバッファクリア.
	 *	@note キューにたまっているデータをクリアします。
	 */
	void Purge(void)
	{
		m_in.m_queueData.RemoveAll();	//RAW情報のみ削除
	}

	/**
	 * [取得] キュー空きサイズ取得.
	 *	@return 空きサイズ
	 */
	size_t GetQueueFreeSize(void) const
	{
		return m_in.m_queueData.GetFreeSize();
	}

	/**
	 * [取得] レポータ取得
	 *	@return レポータ
	 */
	IReport* ReferReport(void)
	{
		return m_in.m_pReport;
	}	

	/**
	 * [取得] レポータ取得
	 *	@return レポータ
	 */
	const IReport* ReferReport(void) const
	{
		return m_in.m_pReport;
	}	

private:
	/// 内部用クラス
	class CInner : public IReport::IListener
	{
		DEFPARENTLISTENER(CQueueingReporter, IParentListener);
	public:
		/// コンストラクタ
		CInner(int queueSize = 10000) 
			: m_queueData(queueSize), m_pListener(NULL), m_pReport(NULL), m_aliveTime(DWORD_MAX), m_tearminalTime(DWORD_MAX)
		{
		}
		/// [通知] イベント発生
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			if ( m_pListener == NULL )
			{
				return true; 
			}
			if ( ev.HasData() )
			{
				EXCLUSIVE( &m_queueData );
				//== データあり
				size_t size = ev.GetDataSize();
				const BYTE* B = ev.GetData();
				while ( size > 0 )
				{
					size_t freeSize = m_queueData.GetFreeSize();
					if ( freeSize == 0 )
					{
						ASSERT0( false, "CQueueingReporter", "キューがあふれました。" );
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
			//== イベントあり
			switch ( ev.GetEvent() )
			{
			case ReportEvent_Disconnect:
				m_OnDeseri(true);
				m_queueData.RemoveAll();	//RAW情報のみ削除
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

		CRingQueueT<BYTE>	m_queueData;	///< 受信生データ
		IParentListener*	m_pListener;	///< リスナー
		IReport*			m_pReport;		///< 登録した IReport
		DWORD				m_aliveTime;	///< キュー１データ有効時間(ms). @note 指定時間が過ぎるとキューをクリアします。
		DWORD				m_tearminalTime;///< 終端時間(ms). @note 指定時間データないと終端として、コマンドを解析します。

	protected:
		/// 持っているキューを解析させる 
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
	CInner		m_in;	///< 内部情報管理
};



}; // TNB

