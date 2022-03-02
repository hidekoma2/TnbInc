#pragma once
/**
 *@file
 * CE 用 Bluetooth 監視関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothWatcher.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCeMsgQueueWatcher.h"
#include <bt_api.h>



//TNB Library
namespace TNB{



/**@ingroup CEBLUETOOTH THREAD
 * Bluetooth(CE) 監視.
 *
 *	@par必要ファイル
 *			TnbCeBluetoothWatcher.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothWatcher
{
public:

	/**@ingroup CEBLUETOOTH THREAD
	 * Bluetooth(CE) 監視のリスナー.
	 *
	 *	@par必要ファイル
	 *			TnbCeBluetoothWatcher.h
	 * 
	 *	@date 09/10/01	新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] イベント通知.
		 *		イベントが発生したら通知されます。
		 *	@note イベント IDとイベント情報型の関係は、以下の通りです。
		 *		<table>
		 *		<tr><td>イベント ID(値)</td>		<td>イベント情報型</td>			<td>内容</td>
		 *		</tr>
		 *		<tr><td>BTE_CONNECTION(100)</td>	<td>BT_CONNECT_EVENT</td>		<td>接続</td>
		 *		</tr>
		 *		<tr><td>BTE_DISCONNECTION(101)</td>	<td>BT_DISCONNECT_EVENT</td>	<td>切断</td>
		 *		</tr>
		 *		<tr><td>BTE_ROLE_SWITCH(102)</td>	<td>BT_ROLE_SWITCH_EVENT</td>	<td>ROLE スイッチ</td>
		 *		</tr>
		 *		<tr><td>BTE_MODE_CHANGE(103)</td>	<td>BT_MODE_CHANGE_EVENT</td>	<td>モードチェンジ</td>
		 *		</tr>
		 *		<tr><td>BTE_PAGE_TIMEOUT(104)</td>	<td>(なし)</td>					<td>ページタイムアウト</td>
		 *		</tr>
		 *		<tr><td>BTE_KEY_NOTIFY(200)</td>	<td>BT_LINK_KEY_EVENT</td>		<td>キー通知</td>
		 *		</tr>
		 *		<tr><td>BTE_KEY_REVOKED(201)</td>	<td>BT_LINK_KEY_EVENT</td>		<td>キー通知</td>
		 *		</tr>
		 *		<tr><td>BTE_LOCAL_NAME(300)</td>	<td>(なし)</td>					<td>名前変更</td>
		 *		</tr>
		 *		<tr><td>BTE_COD(301)</td>			<td>(なし)</td>					<td>COD変更</td>
		 *		</tr>
		 *		<tr><td>BTE_STACK_UP(400)</td>		<td>(なし)</td>					<td>スタックアップ</td>
		 *		</tr>
		 *		<tr><td>BTE_STACK_DOWN(4001)</td>	<td>(なし)</td>					<td>スタックダウン</td>
		 *		</tr>
		 *		<tr><td>BTE_AVDTP_STATE(500)</td>	<td>BT_AVDTP_STATE_CHANGE</td>	<td>AVDTP状態変化</td>
		 *		</tr>
		 *		</table>
		 *	@par サンプル
		 *	\code
		 *		void CFoo::OnBluetoothEvent(DWORD eventId, LPCVOID lpEventData)
		 *		{
		 *			if ( eventId == BTE_CONNECTION )
		 *			{
		 *				const BT_CONNECT_EVENT* P = static_cast<const BT_CONNECT_EVENT*>(lpEventData);
		 *				TRACE0("Connection Event\n");
		 *				TRACE1("-- handle=%d\n", P->hConnection);
		 *				TRACE2("-- bta=%04x%08x\n", GET_NAP(P->bta), GET_SAP(P->bta));
		 *				TRACE1("-- Link Type=%s\n", P->ucLinkType ? _T("ACL") : _T("SCO"));
		 *				TRACE1("-- Encryption Mode=%d\n", P->ucEncryptMode);
		 *			}
		 *		}
		 *	\endcode
		 *	@param eventId イベント ID.
		 *	@param lpEventData イベント情報.
		 */
		virtual void OnBluetoothEvent(DWORD eventId, LPCVOID lpEventData) = 0;
	};


	//------------------------------------


	/// コンストラクタ
	CBluetoothWatcher(void) : m_handle(NULL)
	{
	}

	/// デストラクタ
	~CBluetoothWatcher(void)
	{
		Stop();
	}

	/**
	 * [登録] リスナー登録.
	 *	@param P リスナー。
	 */
	void SetListener(IListener* P)
	{
		m_inner.m_pListener = P ;
	}

	/**
	 * [確認] スタート確認.
	 *	@retval true スタート済み.
	 *	@retval false 未スタート。
	 */
	bool IsStarted(void) const
	{
		return m_handle != NULL;
	}

	/**
	 * [設定] 監視スタート.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Start(void)
	{
		Stop();
		m_queue.SetListener(&m_inner);
		if ( m_queue.Start(10, true, _T("CBluetoothWatcher")) )
		{
			DWORD f = BTE_CLASS_CONNECTIONS | BTE_CLASS_PAIRING | BTE_CLASS_DEVICE | BTE_CLASS_STACK | BTE_CLASS_AVDTP;
			m_handle = ::RequestBluetoothNotifications(f, m_queue.GetSafeHandle());
			if ( m_handle != NULL )
			{
				return true;
			}
		}
		Stop();
		return false;
	}

	/**
	 * [設定] 監視ストップ.
	 *	@note スタートしていない場合、何もしません。
	 */
	void Stop(void)
	{
		if ( m_handle != NULL )
		{
			::StopBluetoothNotifications(m_handle);
			m_handle = NULL;
		}
		m_queue.Stop();
	}

private:
	/// 内部クラス
	class CInner : public CMsgQueueWatcherT<BTEVENT>::IListener
	{
	public:
		CInner(void) : m_pListener(NULL)
		{
		}
		virtual void OnMsgQueueWatcherEvent(const BTEVENT& ev, size_t length, bool isAlert)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnBluetoothEvent(ev.dwEventId, ev.baEventData);
			}
		}
		CBluetoothWatcher::IListener* m_pListener;
	};
	CInner						m_inner;	///< 内部クラス
	CMsgQueueWatcherT<BTEVENT>	m_queue;	///< キュー
	HANDLE						m_handle;	///< ハンドル
};



}; //TNB


