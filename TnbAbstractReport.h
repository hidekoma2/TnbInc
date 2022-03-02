#pragma once
/**
 *@file
 * 通信関係のヘッダ
 *
 *		受信処理は、リスナーを登録することで行います。
 *
 *	@note	マクロ "_TnbREPORT_Viewer_ON" が定義されているか、デバグビルドなら、 Viewer による通信ログをとることが出来ます。
 *	@note	デバグビルドの時、マクロ "_TnbREPORT_Dump_OFF" が定義されている場合、アウトプットに通信ログを表示しません。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbReport.h"
#include "TnbTickCount.h"
#include "TnbDump.h"
#include "TnbReportViewer.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * 通信アクセス抽象クラス
 *
 *		受信は、Listenerを登録し行います。
 *
 *	@par必要ファイル
 *			TnbAbstractReport.h
 * 
 *	@date 07/02/09 抽象化して作成
 *	@date 07/03/15 index, size管理を unsignedに変更。
 *	@date 09/04/08 IReport からメンバ移行。
 *	@date 10/08/05 ITester 廃止。
 *	@date 11/06/14 GetInterfaceId() メソッドの戻り型変更。
 *	@date 12/03/13 DisableReportViewer() を追加。
 */
class CAbstractReport : public IReport, public CSyncSection
{
	DEFSUPER(IReport);
public:

	/** 
	 * コンストラクタ 
	 *	@param dwInterfaceId インターフェースID。省略すると０。
	 *				(これは、複数の通信クラスで共通のリスナーを使用する時、識別として使用します)
	 */
	CAbstractReport(DWORD dwInterfaceId = 0) 
		: m_dwInterfaceId(dwInterfaceId), m_iIdleEventInterval(0), m_piListner(NULL)
	{
	}

	/** デストラクタ */
	virtual ~CAbstractReport(void) 
	{
	}

	/**
	 * [登録] 共通 Listner 登録.
	 *	@note 登録できる IReport::IListener は一つだけです。
	 *	@param piCommListener リスナー。省略すると解除になります。
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL)
	{
		m_piListner = piCommListener;
	}

	/**
	 * [取得] インターフェースID取得
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const
	{
		return m_dwInterfaceId;
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
		if ( iInterval > 0 )
		{
			m_iIdleEventInterval = iInterval;
		}
		else
		{
			m_iIdleEventInterval = 0;
		}
	}

	/** 
	 * [取得] アイドルイベント間隔取得
	 *	@retval 0		アイドルイベント発生無し
	 *	@retval 1以上	間隔(ms)
	 */
	int GetIdleInterval(void) const
	{
		return m_iIdleEventInterval;
	}

	/**
	 * [設定] パージ.
	 *		通信のゴミを浄化
	 *	@note 本クラスでは何もしません。
	 */
	virtual void Purge(void) 
	{
	}

	/**
	 * [確認] 接続待ち
	 *	@param dwTime 待ち時間(ms)
	 *	@retval true 接続
	 *	@retval false タイムアウト
	 */
	bool WaitConnect(DWORD dwTime) const
	{
		if ( ! IsAlive() )
		{
			return false;
		}
		CTickCount tick;
		while ( ! tick.IsPassed(dwTime) )
		{
			if ( IsConnect() )
			{
				return true;
			}
			::Sleep(10);
		}
		return IsConnect();
	}

	/**
	 * [設定] レポートビュワー停止.
	 *		本メソッドを使用すると、通信ログの出力を行いません。
	 *		デフォルトは、出力を行います（ただしビルドの設定にもよります）。
	 */
	void DisableReportViewer(void)
	{
		m_viewer.Invalid();
	}

protected:

	/**
	 * [内部] 送信情報通知
	 *	@note 処理は開発用TRACEのみ
	 *	@param sizeData 送信するデータ長。
	 *	@param lpcvData 送信するデータ。
	 */
	void m_OnSend(size_t sizeData, LPCVOID lpcvData)
	{
		TRACE1("%s:Send\n", GetReportName());
		#ifndef _TnbREPORT_Dump_OFF
			DebugDump(sizeData, lpcvData);
		#endif
		m_ToViewer(sizeData, lpcvData, TNBVIW_DIRE_SEND | GetReportID());
	}
	
	/**
	 * [内部] コールバック通知実行
	 *	@param ev 通知する内容
	 *	@retval true	正常
	 *	@retval false	切断要求
	 */
	bool m_OnEvent(const CReportEvent& ev)
	{
		if ( ev.HasData() )
		{
			//開発用TRACE
			TRACE1("%s:Receive\n", GetReportName());
			#ifndef _TnbREPORT_Dump_OFF
				DebugDump(ev.GetDataSize(), ev.GetData());
			#endif
		}
		if ( ev.HasData() )
		{
			m_ToViewer(ev.GetDataSize(), ev.GetData(), TNBVIW_DIRE_RECV | GetReportID());
		}
		else if ( ev.GetEvent() != ReportEvent_Idle )
		{
			m_ToViewer(ev.GetEventString(), GetReportID());
		}
		bool boRc = true;
		if ( m_piListner != NULL )
		{
			EXCLUSIVE(this);
			boRc = m_piListner->OnReportEvent(ev);
		}
		return boRc;
	}

	/**
	 * [出力] ダンプ出力.
	 *	@param sizeData データサイズ.
	 *	@param lpcvData データ
	 */
	void ToViewer(size_t sizeData, LPCVOID lpcvData)
	{
		m_ToViewer(sizeData, lpcvData, TNBVIW_TYPE_RAWTEXT | GetReportID());
	}

	/**
	 * [出力] 文字列出力.
	 *	@param lpszText 文字列.
	 */
	void ToViewer(LPCTSTR lpszText)
	{
		m_ToViewer(lpszText, TNBVIW_TYPE_RAWTEXT | GetReportID());
	}

private:
	// [出力] ダンプ出力.
	void m_ToViewer(size_t sizeData, LPCVOID lpcvData, DWORD cookie)
	{
		m_viewer.ToViewer(sizeData, lpcvData, cookie);
	}
	// [出力] 文字列出力.
	void m_ToViewer(LPCTSTR lpszText, DWORD cookie)
	{
		m_viewer.ToViewer(lpszText, cookie);
	}
	CReportViewer	m_viewer;				///< T-Report Viewer
	DWORD			m_dwInterfaceId;		///< インターフェースID
	IListener*		m_piListner;			///< リスナー
	int				m_iIdleEventInterval;	///< Idle通知の間隔(ms)
};



}; // TNB
