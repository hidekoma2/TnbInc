#pragma once
/**
 *@file
 * 通信報告関係のヘッダ
 *
 *		受信処理は、リスナーを登録することで行います。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * 通信受信イベント種.
 *		IReport のサブクラスが通知するイベント種です。
 */
enum EReportEvent
{
	ReportEvent_Non,			///< イベントなし.	@note 通知されるものではありません。
	ReportEvent_Idle,			///< アイドル.		@note 使用できないサブクラスもあります。
	ReportEvent_End,			///< 終了.			@note Alive状態を終えました。
	ReportEvent_Error,			///< エラー.		@note エラーコード共に通知されます。
	ReportEvent_Connect,		///< 接続.			@note 切断とペアに使用されます。
	ReportEvent_Disconnect,		///< 切断.			@note 接続とペアで使用されます。
	ReportEvent_IrregularEnd,	///< 不正終了.		@note Stopではない、強制的な終了時に通知されます。

	ECE_Non = ReportEvent_Non,				///< イベントなし(旧シンボル)
	ECE_Idle = ReportEvent_Idle,			///< アイドル(旧シンボル)
	ECE_End = ReportEvent_End,				///< 終了(旧シンボル)
	ECE_Error = ReportEvent_Error,			///< エラー(旧シンボル)
	ECE_Connect = ReportEvent_Connect,		///< 接続(旧シンボル)
	ECE_Disconnect = ReportEvent_Disconnect,///< 切断(旧シンボル)
};

/**@ingroup REPORT
 * 通信受信イベント種.
 *		@note 旧名です。 EReportEvent を使用してください。
 */
typedef EReportEvent EReportEventId;



/**@ingroup REPORT
 * 通信受信イベント管理クラス
 *
 *		リスナーのメソッド OnReportEvent() で通知される情報です。
 *		データ（長さとアドレス）かイベントのいずれかを保持します。
 *
 *	@see IReport::IListener::OnReportEvent()
 *
 *	@par必要ファイル
 *			TnbReport.h
 * 
 *	@date 06/05/11 新規作成
 *	@date 06/07/07 InterfaceID概念、追加
 *	@date 11/03/10 ReportEvent_IrregularEnd を追加
 *	@date 11/06/14 GetInterfaceId() メソッドの戻り値変更。
 */
class CReportEvent
{
	const BYTE*		m_lpcbData;			///< 受信データアドレス
	size_t			m_dataSize;			///< 受信データサイズ
	EReportEvent	m_eEvent;			///< 受信イベント週類
	DWORD			m_dwErrorCode;		///< エラーコード
	friend struct IReport;				///< パートナークラス宣言

protected:

	mutable IReport*	m_piReport;	///< Report Objectの参照

public:

	/**
	 * コンストラクタ
	 *	@param piComm IReport のサブクラスのポインタ
	 *	@param eEvent イベントコード（省略可能）
	 *	@param dwError エラーコード（省略可能）
	 */
	explicit 
	CReportEvent(IReport* piComm, EReportEvent eEvent = ReportEvent_Non, DWORD dwError = 0)
	{
		m_lpcbData			= NULL;
		m_dataSize			= 0;
		m_eEvent			= eEvent;
		m_dwErrorCode		= dwError;
		m_piReport	= piComm;
	}

	/**
	 * [取得] インターフェースID取得
	 *		@note 基本的に IReport のサブクラスのコンストラクタで指定した値を返します。
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const;	//本体はファイル下位に記述

	/**
	 * [送信] データ送信
	 *	@attention 本クラスでは受信時に送信処理は出来ません。
	 *	@param dataSize 送信するデータ長。
	 *	@param lpcvData 送信するデータ。
	 *	@return 常にfalse。
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData) const;	//本体はファイル下位に記述

	/**
	 * [確認] Dataを持っているか
	 *	@retval true	Dataを持っている
	 *	@retval false	持っていない
	 */
	bool HasData(void) const
	{
		return m_lpcbData != NULL;
	}

	/**
	 * [確認] Event(Error)を持っている
	 *	@retval true	Event(Error)を持っている
	 *	@retval false	持っていない
	 */
	bool HasEvent(void) const
	{
		return ! HasData();
	}

	/**
	 * [取得] データ長取得
	 *	@return 保持しているデータ長
	 */
	size_t GetDataSize(void) const
	{
		return HasData() ? m_dataSize : 0;
	}

	/**
	 * [取得] データアドレス取得
	 *	@return 保持しているデータアドレス
	 */
	const BYTE* GetData(void) const
	{
		return m_lpcbData;
	}

	/**
	 * [取得] イベントコード取得
	 *	@return 保持しているイベントコード
	 */
	EReportEvent GetEvent(void) const
	{
		return HasEvent() ? m_eEvent : ReportEvent_Non;
	}

	/**
	 * [取得] エラーコード取得
	 *	@return 保持しているエラーコード
	 */
	DWORD GetErrorCode(void) const
	{
		if ( HasEvent() && GetEvent() == ReportEvent_Error )
		{
			return m_dwErrorCode;
		}
		return 0;
	}

	/**
	 * [取得] イベントコード文字列取得
	 *	@return 保持しているイベントコードを文字列化したもの
	 */
	CStr GetEventString(void) const
	{
		switch ( GetEvent() )
		{
		case ReportEvent_Non:			return _T("なし");
		case ReportEvent_Idle:			return _T("アイドル");
		case ReportEvent_End:			return _T("終了");
		case ReportEvent_IrregularEnd:	return _T("強制終了");
		case ReportEvent_Connect:		return _T("接続");
		case ReportEvent_Disconnect:	return _T("切断");
		case ReportEvent_Error:			return CStr::Fmt(_T("エラー[%u]"), GetErrorCode());
		}
		return _T("");
	}

	/**
	 * [設定] データ設定.
	 *	@note ポインタは参照のみで、実体は本インスタンスで保持しません。
	 *	@param dataSize データ長
	 *	@param lpcData データアドレス
	 */
	void SetData(size_t dataSize, LPCVOID lpcData)
	{
		m_dataSize = dataSize;
		m_lpcbData = static_cast<const BYTE*>(lpcData);
	}
};



/**@ingroup REPORT
 * 通信アクセスインターフェース
 *
 *		受信は、Listenerを登録し行います。
 *
 *	@par必要ファイル
 *			TnbReport.h
 * 
 *	@date 06/05/11 新規作成
 *	@date 06/07/07 InterfaceID概念、追加
 *	@date 06/08/31 CommViewer に対応。
 *	@date 06/09/08 Idleイベントサポート。 Purge() / IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *	@date 07/02/09 抽象化。
 *	@date 07/06/11 Start() メソッドを追加。
 *	@date 09/04/08 メンバ変数をすべて廃止。メソッドを追加。
 *	@date 11/06/14 GetInterfaceId() GetReportID() メソッドの戻り値変更。
 */
struct IReport
{
	friend class CReportEvent;		///< パートナークラス宣言

	/**@ingroup REPORT
	 * 通信のリスナーインターフェース
	 *
	 *		IReport の通知用インターフェースです。
	 *
	 *	@see IReport::SetCommListener()
	 *
	 *	@par必要ファイル
	 *			TnbReport.h
	 * 
	 *	@date 06/05/11	新規作成
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] イベント発生.
		 *		 データを受信時や、イベント発生時にコールされます。
		 *		ev の{@link CReportEvent::HasData() HasData()}、
		 *		{@link CReportEvent::HasEvent() HasEvent()}により、
		 *		どちらのタイミングでコールされたかわかります。
		 *	@par○イベント発生時
		 *		ev の{@link CReportEvent::GetEvent() GetEvent()} により、
		 *		どのイベントが発生したか知ることが出来ます。<BR>
		 *		ReportEvent_Idle :		Alive状態でデータ受信がないとき指定のタイミングで通知されます。
		 *								ただし使用できないサブクラスもあります。<BR>
		 *		ReportEvent_Error :		エラー発生時に通知されます。ev の
		 *								{@link CReportEvent::GetErrorCode() GetErrorCode()}
		 *								でエラーコードを知ることが出来ます。<BR>
		 *		ReportEvent_Connect :	通信可能になった時に通知されます。
		 *								戻り値で false を返した場合、 ReportEvent_Disconnect は通知されませんが、
		 *								通信不可能状態になります（ ReportEvent_End が通知されるサブクラスもあります）。<BR>
		 *		ReportEvent_Disconnect :通信不能になった時に通知されます。戻り値は意味を持ちません。
		 *								また、ev の{@link CReportEvent::Send() Send()}は使用できません(既に通信不能のため)。<BR>
		 *		ReportEvent_End :		Alive状態を終える時に通知されます。戻り値は意味を持ちません。
		 *								また、ev の{@link CReportEvent::Send() Send()}は使用できません(既に通信不能のため)。<BR>
		 *		ReportEvent_IrregularEnd : 強制的に切断された時(デバイスが抜かれた時など)に、ReportEvent_End を通知する前に、
		 *								通知されます（これの後に 必ず ReportEvent_End が通知されます）。
		 *								戻り値は意味を持ちません。また、ev の{@link CReportEvent::Send() Send()}は使用できません(既に通信不能のため)。<BR>
		 *	@par○データ受信時
		 *		ev の{@link CReportEvent::GetDataSize() GetDataSize()} により、受信した
		 *		データのサイズを得ることが出来ます。
		 *		ev の{@link CReportEvent::GetData() GetData()} により、受信した
		 *		データのアドレスを得ることが出来ます。
		 *
		 *	@param ev 受信した内容（データかイベント）。
		 *	@retval true	正常
		 *	@retval false	切断要求。 ReportEvent_End , ReportEvent_IrregularEnd イベントの場合、切断要求は無視されます。
		 */
		virtual bool OnReportEvent(const CReportEvent& ev) = 0;
	};


	//------------------------------


	/// デストラクタ
	virtual ~IReport(void) 
	{
	}

	/**
	 * [取得] コミュニケーション名.
	 *	@note 開発用のメソッドです。
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const = 0; 

	/**
	 * [取得] コミュニケーションID.
	 *		TNBVIW_KIND_??? + PORT を返します。
	 *	@note 開発用のメソッドです。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const = 0; 

	/**
	 * [登録] 共通Listner登録.
	 *	@note 登録できる IReport::IListener は一つだけです。
	 *	@param piCommListener リスナー。省略すると解除になります。
	 */
	virtual void SetListener(IReport::IListener* piCommListener = NULL) = 0;

	/**
	 * [取得] インターフェースID取得
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const = 0;

	/** 
	 * [設定] アイドルイベント設定
	 *	@note	受信がない時、指定の間隔でコールバックを発生させることができます。
	 *			開始後、あるいは受信処理後、指定ms秒後にコールバックが始まります。
	 *			Open中に有無の切り替えは可能です。
	 *	@param iInterval Idleイベントをあげる間隔を指定します。０なら、Idleイベントなし。
	 */
	virtual void SetIdleEventMode(int iInterval = 0) = 0;

	/**
	 * [操作] 開始.
	 *	@note 設定メソッドで設定を行ってから本メソッドを使用し、開始します。
	 *	@retval true 成功。Alive状態になりました。
	 *	@retval false 失敗。Alive状態にはなれませんでした。
	 */
	virtual bool Start(void) = 0;

	/**
	 * [操作] 停止.
	 *		Alive状態を終了します。
	 */
	virtual void Stop(void) = 0;

	/**
	 * [確認] Aliveチェック.
	 *		各サブクラスの開始メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const = 0;
	
	/**
	 * [確認] 接続チェック
	 *	@retval true	接続中。
	 *	@retval	false	切断。
	 */
	virtual bool IsConnect(void) const = 0;

	/**
	 * [送信] データ送信
	 *	@param dataSize 送信するデータ長。
	 *	@param lpcData 送信するデータ。
	 *	@retval true	成功
	 *	@retval	false	失敗
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcData) = 0;

	/**
	 * [設定] パージ.
	 *		通信のゴミを浄化
	 */
	virtual void Purge(void) = 0;
};



/// 送信
inline bool CReportEvent::Send(size_t dataSize, LPCVOID lpcvData) const
{
	return m_piReport->Send(dataSize, lpcvData);
}

/// インターフェースID取得
inline DWORD CReportEvent::GetInterfaceId(void) const
{
	return m_piReport->GetInterfaceId();
}



}; // TNB
