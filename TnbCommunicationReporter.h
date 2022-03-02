#pragma once
/**
 *@file
 * 通信レポータ関係のヘッダ
 *
 *	@note	マクロ "_TnbREPORT_Viewer_ON" が定義されているか、デバグビルドなら、 Viewerによる通信ログをとることが出来ます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */


#include "TnbAbstractReport.h"
#include "TnbThread.h"
#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT COMMUNICATION
 * 通信レポータクラス
 *
 *		ICommunication を包括し、非同期通信をサポートします(IReport インターフェースを提供します）。		
 *
 *	@note CCommunicationReporterT を使用したほうが、テンプレートタイプで包括するため、記述が単純化できます。
 *
 *	@par必要ファイル
 *			TnbCommunicationReporter.h
 * 
 *	@date 07/09/13 新規作成
 *	@date 08/10/15 デストラクタ時の Stop を廃止。
 *	@date 09/08/27 SetReceiveThreadPriority() 追加。構成変更。
 *	@date 09/09/10 SetReceiveThreadPriority() 廃止、 GetReceiveThreadStatusGet() 追加。
 *	@date 10/05/28 SetTester() の処理に対応。ポーリング処理改善。
 *	@date 10/08/05 ITester 廃止。
 *	@date 11/03/01 デフォルトのポーリング間隔を 1ms に
 *	@date 11/03/10 ReportEvent_IrregularEnd を追加
 *	@date 11/06/14 GetReportID() メソッドの戻り型変更。
 *	@date 11/07/06 送信に失敗していても送信ログがでていたのを修正。
 */
class CCommunicationReporter : public CAbstractReport, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractReport);
public:

	/** 
	 * コンストラクタ 
	 *	@note 本クラスのインスタンスと ICommunication のインスタンスを同じスコープにおく場合、 
	 *		ICommunication のほうを上に宣言しておくこと（先に構築され、後に破棄されるため）。
	 *	@param pComm 通信クラス。本インスタンスが破棄されるまで、破棄しないでください。
	 *	@param dwInterfaceId インターフェースID。省略すると０。
	 *				(これは、複数の通信クラスで共通のリスナーを使用する時、識別として使用します)
	 */
	explicit CCommunicationReporter(ICommunication* pComm, DWORD dwInterfaceId = 0) : _super(dwInterfaceId)
	{
		m_piCommunication = pComm;		// 通信クラス
		m_lpbReceiveWork = NULL;		// 受信用バッファアドレス
		m_dwPollingTiming = 1;			// 間隔
		m_boIsConnect = false;
		m_receivePacketMaxSize = E_ReceiveDataBufferSize;
		m_hasOndemandStop = false;
	}

	/** デストラクタ */
	virtual ~CCommunicationReporter(void)
	{
		if ( m_lpbReceiveWork != NULL )
		{
			delete[] m_lpbReceiveWork;
			m_lpbReceiveWork = NULL;
		}
	}

	/**
	 * [取得] レポート名.
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const
	{
		return m_piCommunication->GetProperty().typeName;
	}

	/**
	 * [取得] レポート ID.
	 *		種類の識別値を返します。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return m_piCommunication->GetProperty().typeId;
	}

	/**
	 * [確認] Aliveチェック.
	 *		Open() メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const
	{ 
		return m_threadRecv.IsAlive() && m_piCommunication->IsOpened();
	}

	/**
	 * [確認] 接続されているか
	 *	@retval true	CTS ON状態。
	 *	@retval	false	CTS OFF状態。
	 */
	virtual bool IsConnect(void) const 
	{
		return m_piCommunication->IsConnect(); 
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
		_super::SetIdleEventMode(iInterval);
	}

	/**
	 * [設定] 受信チェック間隔設定.
	 *		受信情報があるか、ポーリングするタイミングを指定します。
	 *	@note	設定しない場合、1ms になっています。リアルタイム性が高い場合、この値を小さくしないと、
	 *			送信してから受信までに時間がかかってしまいます。
	 *	@note ICommunication のサブクラスの{@link TNB::ICommunication::SetReceiveTimeout 受信待ち時間} が
	 *			有効なら、それを有効にし、この受信チェック間隔は 0 にするのがベストです。
	 *	@param dwPollingTiming タイミング(単位;ミリ秒)
	 */
	void SetReceivePollingTiming(DWORD dwPollingTiming)
	{
		m_dwPollingTiming = dwPollingTiming;
	}

	/**
	 * [操作] 通信レポート開始.
	 *	@note	使用している通信クラスで、Openできるように設定後、本メソッドを使用します。
	 *	@retval true 成功。
	 *	@retval false エラー。 ::GetLastError() で詳細取得可能（その値は SystemErrorToMessageText() で文字列化できます)。
	 */
	virtual bool Start(void)
	{
		if ( m_threadRecv.IsAlive() )
		{
			//スレッド停止
			m_threadRecv.Stop();
		}
		if ( m_piCommunication->Open() )
		{
			const ICommunication::TProperty& param = m_piCommunication->GetProperty();
			if ( param.receivePacketSize != INVALID_SIZE )
			{
				//サイズ固定だ
				m_receivePacketMaxSize = param.receivePacketSize;
			}
			if ( m_lpbReceiveWork != NULL )
			{
				delete[] m_lpbReceiveWork;
			}
			m_lpbReceiveWork = new BYTE[m_receivePacketMaxSize];
			//
			m_threadRecv.SetRunner(this);
			#ifdef _DEBUG
				m_threadRecv.Start(CStr::Fmt(_T("%s:監視"), GetReportName()));
			#else
				m_threadRecv.Start();
			#endif
			return true;
		}
		return false;
	}

	/**
	 * [操作] 通信レポート停止.
	 *	@note 受信用スレッドも停止します。
	 */
	virtual void Stop(void)
	{
		m_hasOndemandStop = true;
		m_piCommunication->Close();
		if ( m_threadRecv.IsAlive() )
		{
			//スレッド停止
			m_threadRecv.Stop();
		}
		m_hasOndemandStop = false;
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
		if ( ! m_piCommunication->IsOpened() )
		{
			return false;
		}
		bool r = m_piCommunication->Send(dataSize, lpcvData) == dataSize;
		if ( r )
		{
			_super::m_OnSend(dataSize, lpcvData);
		}
		return r;
	}

	/**
	 * [設定] パージ.
	 *	@note 通信のゴミを浄化
	 */
	virtual void Purge(void)
	{
		EXCLUSIVE( &m_syncReceive );
		m_piCommunication->Purge();
	}

	/**
	 * [参照] 受信スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadRecv;
	}

private:

	/// 自クラスの固定設定
	enum
	{
		E_ReceiveDataBufferSize = 10000,		///< 受信用バッファサイズ		
	};
	CSyncSection	m_syncReceive;			///< レシーブの排他
	CThread			m_threadRecv;			///< 受信スレッド管理
	ICommunication*	m_piCommunication;		///< 通信クラス
	LPBYTE			m_lpbReceiveWork;		///< 受信用バッファアドレス
	DWORD			m_dwPollingTiming;		///< 間隔
	bool			m_boIsConnect;			///< コネクトフラグ
	bool			m_hasOndemandStop;		///< ストップ要求あり
	size_t			m_receivePacketMaxSize;	///< 受信パケット最大サイズ

	/**
	 * スレッド処理関数
	 *	@note	受信すると、コールバック関数をコールします。
	 *			受信失敗すると、ハンドルをクローズし、
	 *			コールバック関数へ通知し、スレッドを終了します。
	 *	@retval ０（固定)。
	 */
	virtual DWORD Run(void)
	{
		CTickCount tick;
		bool isReceived = true;
		DWORD ec = 0;
		//
		while ( IsRunnable() )
		{
			if ( isReceived )
			{
				isReceived = false;
			}
			else if ( m_dwPollingTiming > 0 )
			{
				::Sleep(m_dwPollingTiming);
			}
			//== 受信中排他ON
			m_syncReceive.Lock();
			ICommunication::EState s = m_piCommunication->Check();
			ec = ::GetLastError();
			if ( s == ICommunication::CLOSED )
			{
				m_syncReceive.Unlock();
				break;	//Closeされたら、終了
			}
			if ( s == ICommunication::OPENNING && m_boIsConnect )
			{
				//受信中排他OFF
				m_syncReceive.Unlock();
				//切断を通知。
				m_OnEvent(CReportEvent(this, ReportEvent_Disconnect));
				m_boIsConnect = false;
				continue;
			}
			if ( s == ICommunication::CONNECTING && ! m_boIsConnect )
			{
				//接続を通知。
				if ( ! m_OnEvent(CReportEvent(this, ReportEvent_Connect)) )
				{
					break;
				}
				m_boIsConnect = true;
			}
			if ( m_piCommunication->CanReceive() )
			{
				size_t dwReceivedDataCount = INVALID_SIZE;
				{
					//---- 1Byte以上ある or わからない(INVALID_SIZE)。
					if ( dwReceivedDataCount > m_receivePacketMaxSize )
					{
						dwReceivedDataCount = m_receivePacketMaxSize;
					}
					if ( m_lpbReceiveWork == NULL )
					{
						//受信中排他OFF
						m_syncReceive.Unlock();
						continue;
					}
					size_t dwReadBytes = m_piCommunication->Receive(dwReceivedDataCount, m_lpbReceiveWork);
					//
					if ( dwReadBytes == INVALID_SIZE )
					{
						//失敗
						//受信中排他OFF
						m_syncReceive.Unlock();
						continue;
					}
					//コールバック関数へ通知。
					if ( dwReadBytes > 0 )
					{
						//受信中排他OFF
						m_syncReceive.Unlock();
						CReportEvent ev(this);
						ev.SetData(dwReadBytes, m_lpbReceiveWork);
						if ( ! m_OnEvent(ev) )
						{
							break;
						}
						isReceived = true;
						tick.Reset();
						continue;
					}
				}
			}	
			//受信中排他OFF
			m_syncReceive.Unlock();
			if ( _super::GetIdleInterval() > 0 )
			{
				//---- 受信データなし&IDLEイベント要求あり。
				if ( tick.IsPassedAndReset(_super::GetIdleInterval()) )
				{
					//コールバック関数へ通知。
					if ( ! m_OnEvent(CReportEvent(this, ReportEvent_Idle)) )
					{
						break;
					}
				}
			}
		}
		//コールバック関数へ通知。
		if ( m_boIsConnect )
		{
			m_boIsConnect = false;
			//切断を通知。
			m_OnEvent(CReportEvent(this, ReportEvent_Disconnect));
		}
		m_piCommunication->Close();
		::SetLastError(ec);
		if ( ! m_hasOndemandStop )
		{ 
			m_OnEvent(CReportEvent(this, ReportEvent_IrregularEnd));
		}
		m_OnEvent(CReportEvent(this, ReportEvent_End));
		return 0;
	}

	friend class CCommunicationReporterTest;		///< フレンドクラス宣言
};



/**@ingroup REPORT COMMUNICATION
 * 通信レポータテンプレート
 *
 *		ICommunication を包括し、非同期通信をサポートします(IReport インターフェースを提供します）。	
 *
 *	@note CCommunicationReporter に比べ、テンプレートタイプで包括するため、記述が単純化できます。
 *
 *	@par必要ファイル
 *			TnbCommunicationReporter.h
 *
 *	@param COMM ICommunication のサブクラス名
 * 
 *	@date 09/03/31 新規作成
 *	@date 11/02/04 デストラクタ時にクローズするようにした。
 */
template<typename COMM>
class CCommunicationReporterT : public CCommunicationReporter
{
	DEFSUPER(CCommunicationReporter);
public:

	/**
	 * コンストラクタ
	 *	@param dwInterfaceId インターフェースID。省略すると０。
	 *				(これは、複数の通信クラスで共通のリスナーを使用する時、識別として使用します)
	 */
	explicit CCommunicationReporterT(DWORD dwInterfaceId = 0) : _super(&m_comm, dwInterfaceId)
	{
	}

	/// デストラクタ
	virtual ~CCommunicationReporterT(void)
	{
		Stop();
	}

	/**
	 * [参照] 通信クラス参照.
	 *	@note 包括している通信クラスの参照を返します。
	 *	@return 通信クラス
	 */
	COMM& ReferCommunication(void)
	{
		return m_comm;
	}

	/**
	 * [参照] 通信クラス参照.
	 *	@note 包括している通信クラスの参照を返します。
	 *	@return 通信クラス
	 */
	COMM* operator->(void)
	{
		return &m_comm;
	}

	/**
	 * [参照] 通信クラス参照.
	 *	@note 包括している通信クラスの参照を返します。
	 *	@return 通信クラス
	 */
	const COMM& ReferCommunication(void) const
	{
		return m_comm;
	}

	/**
	 * [参照] 通信クラス参照.
	 *	@note 包括している通信クラスの参照を返します。
	 *	@return 通信クラス
	 */
	const COMM* operator->(void) const
	{
		return &m_comm;
	}

private:
	COMM	m_comm;
};



}; // TNB