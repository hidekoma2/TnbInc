#pragma once
/**
 *@file
 * 通信報告監視アダプタ関係のヘッダ
 *
 *		IReport のサブクラスの送受信を監視（ロガーに通知）することが出来ます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbReport.h"
#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * 通信報告監視アダプタ
 *
 *		レポーター（ IReport を実装したクラス）を本クラスに登録( SetReporter() )して使用します。
 *		本クラスは登録したレポーターを使って通信を行います。その際、通信内容をロガー（ ILogger を実装したクラス）
 *		に通知することが出来ます。
 *
 *	\par使用例
 *	\code
 *
 *	// .h
 *	class CFooDlg : public CDialog, CReportWatchAdapter::ILogger
 *	{
 *	protected:
 *		virtual void OnReportWatchEventLog(EReportEvent ev);
 *		virtual void OnReportWatchDataLog(EKind kind, size_t dataSize, const BYTE* pData);
 *	private:
 *		CReportWatchAdapter					m_rwp;
 *		CCommunicationReporterT<CRs232c>	m_rs232c;
 *		CPacketCommander					m_commander;
 *	};
 *
 *	// .cpp
 *	CFooDlg::CFooDlg(void) : CDialog(CFooDlg::IDD, pParent)
 *			, m_rwp(&m_rs232c)				// アダプタが使う通信クラスを指定
 *	{
 *	}
 *	void CFooDlg::OnReportWatchEventLog(EReportEvent ev)
 *	{
 *		//ログ表示
 *	}
 *	void CFooDlg::OnReportWatchDataLog(EKind kind, size_t dataSize, const BYTE* pData)
 *	{
 *		//ログ表示
 *	}
 *	void CFooDlg::OnInitDialog()
 *	{
 *			;
 *		m_rwp.SetLogger(this);
 *		m_commander.Open(&m_rwp, true);	//アダプタのほうを指定する。通信は m_rs232c が担う。
 *			;
 *	}
 *
 *	\endcode
 *	@par必要ファイル
 *			TnbReportWatchAdapter.h
 * 
 *	@date 11/07/06 新規作成
 */
class CReportWatchAdapter : public IReport, CCopyImpossible
{
	DEFSUPER(IReport);
public:

	/**@ingroup REPORT
	 * 通信報告監視アダプタのロガーインターフェース
	 *
	 *		IReport の通知用インターフェースです。
	 *
	 *	@see CReportWatchAdapter::SetLogger()
	 *
	 *	@par必要ファイル
	 *			TnbReportWatchAdapter.h
	 * 
	 *	@date 11/07/06 新規作成
	 */
	struct ILogger
	{
		/// 種類
		enum EKind
		{
			Kind_Send,		///< 送信
			Kind_SendError,	///< 送信エラー(実際は送信されていない)
			Kind_Received,	///< 受信
		};

		/// デストラクタ
		virtual ~ILogger(void) {}

		/**
		 * [通知] イベント発生.
		 *		 データを受信を除く、イベント発生時にコールされます。
		 *	@param ev イベント
		 */
		virtual void OnReportWatchEventLog(EReportEvent ev) = 0;

		/**
		 * [通知] データ通信発生.
		 *		 データの送受信時にコールされます。
		 *	@param kind 種類
		 *	@param dataSize データ長
		 *	@param pData データ
		 */
		virtual void OnReportWatchDataLog(EKind kind, size_t dataSize, const BYTE* pData) = 0;
	};


	//-----------------------------


	/**
	 * コンストラクタ.
	 *	@param pRep レポータのインスタンス
	 */
	CReportWatchAdapter(IReport* pRep)
	{
		ASSERT( pRep != NULL );	//インスタンスを指定してください
		m_pReporter = pRep;
		m_in.pListener = NULL;
		m_in.pLogger = NULL;
	}

	/**
	 * [登録] レポーター登録.
	 *	@param pRep レポータ
	 *	@retval true 成功.
	 *	@retval false 失敗。 Start() 後は失敗します。 pRep が NULL の時も失敗します。
	 */
	bool SetReporter(IReport* pRep)
	{
		if ( ! IsAlive() && pRep != NULL )
		{
			m_pReporter = pRep;
			return true;
		}
		return false;
	}

	/**
	 * [登録] ロガー登録.
	 *	@param pLogger ロガー
	 */
	void SetLogger(ILogger* pLogger)
	{
		m_in.pLogger = pLogger;
	}

	/**
	 * [取得] コミュニケーション名.
	 *	@note 開発用のメソッドです。
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const
	{
		return m_pReporter->GetReportName();
	}

	/**
	 * [取得] コミュニケーションID.
	 *		TNBVIW_KIND_??? + PORT を返します。
	 *	@note 開発用のメソッドです。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return m_pReporter->GetReportID();
	}

	/**
	 * [登録] 共通Listner登録.
	 *	@note 登録できる IReport::IListener は一つだけです。
	 *	@param piCommListener リスナー。省略すると解除になります。
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL)
	{
		m_in.pListener = piCommListener;
	}

	/**
	 * [取得] インターフェースID取得
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const
	{
		return m_pReporter->GetInterfaceId();
	}

	/** 
	 * [設定] アイドルイベント設定
	 *	@note	受信がない時、指定の間隔でコールバックを発生させることができます。
	 *			開始後、あるいは受信処理後、指定ms秒後にコールバックが始まります。
	 *			Open中に有無の切り替えは可能です。
	 *	@param iInterval Idleイベントをあげる間隔を指定します。０なら、Idleイベントなし。
	 */
	virtual void SetIdleEventMode(int iInterval = 0)
	{
		m_pReporter->SetIdleEventMode(iInterval);
	}

	/**
	 * [操作] 開始.
	 *	@note 設定メソッドで設定を行ってから本メソッドを使用し、開始します。
	 *	@retval true 成功。Alive状態になりました。
	 *	@retval false 失敗。Alive状態にはなれませんでした。
	 */
	virtual bool Start(void)
	{
		m_pReporter->SetListener(&m_in);
		return m_pReporter->Start();
	}

	/**
	 * [操作] 停止.
	 *		Alive状態を終了します。
	 */
	virtual void Stop(void)
	{
		m_pReporter->Stop();
	}

	/**
	 * [確認] Aliveチェック.
	 *		各サブクラスの開始メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const
	{
		return m_pReporter->IsAlive();
	}
	
	/**
	 * [確認] 接続チェック
	 *	@retval true	接続中。
	 *	@retval	false	切断。
	 */
	virtual bool IsConnect(void) const
	{
		return m_pReporter->IsConnect();
	}

	/**
	 * [送信] データ送信
	 *	@param dataSize 送信するデータ長。
	 *	@param lpcData 送信するデータ。
	 *	@retval true	成功
	 *	@retval	false	失敗
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcData)
	{
		bool r = m_pReporter->Send(dataSize, lpcData);
		if ( m_in.pLogger != NULL )
		{
			EXCLUSIVE( &m_in.sync );
			m_in.pLogger->OnReportWatchDataLog((r ? (ILogger::Kind_Send) : (ILogger::Kind_SendError)), dataSize, static_cast<const BYTE*>(lpcData));
		}
		return r;
	}

	/**
	 * [設定] パージ.
	 *		通信のゴミを浄化
	 */
	virtual void Purge(void)
	{
		m_pReporter->Purge();
	}

private:
	/// 内部クラス
	class CInner : public IReport::IListener
	{
	public:
		IReport::IListener*	pListener;		// リスナー
		ILogger*			pLogger;		// ロガー
		CSyncSection		sync;			// 同期
		//[通知] イベント発生
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			if ( pListener != NULL )
			{
				if ( pLogger != NULL )
				{
					EXCLUSIVE( &sync );
					if ( ev.HasData() )
					{
						pLogger->OnReportWatchDataLog(ILogger::Kind_Received, ev.GetDataSize(), ev.GetData());
					}
					else
					{
						pLogger->OnReportWatchEventLog(ev.GetEvent());
					}
				}
				return pListener->OnReportEvent(ev);
			}
			return true;
		}
	};
	CInner		m_in;			///< 内部クラス
	IReport*	m_pReporter;	///< レポーター
};



};
