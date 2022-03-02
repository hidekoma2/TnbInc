#pragma once
/**
 *@file
 * CE 用電源監視関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCePowerWatcher.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCeMsgQueueWatcher.h"
#include <pm.h>



//TNB Library
namespace TNB{



/**@ingroup WINCE THREAD
 * 電源監視 (CE専用).
 *
 *	@par必要ファイル
 *			TnbCePowerWatcher.h
 * 
 *	@date 09/12/01	新規作成
 *	@date 10/03/30	Open/Close -> Start/Stop に変更。
 */
class CPowerWatcher
{
public:

	/**@ingroup WINCE THREAD
	 * 電源監視のリスナーインターフェース (CE専用).
	 *
	 *	@par必要ファイル
	 *			TnbCePowerWatcher.h
	 * 
	 *	@date 09/12/01	新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] 電源状態変化通知.
		 *	@note 電源状態に変化が発生したら通知されます。
		 *	@param flag フラグ
		 *	@param lpszState 状態文字列
		 */
		virtual void OnPowerTransitionEvent(DWORD flag, LPCTSTR lpszState) {}

		/**
		 * [通知] レジューム通知.
		 *	@note レジュームしたら通知されます。
		 */
		virtual void OnPowerResumeEvent(void) {}

		/**
		 * [通知] DC/AC切り替え通知.
		 *	@note DC ←→ AC 時、通知されます。
		 */
		virtual void OnPowerStatusChange(void) {}

		/**
		 * [通知] 電源状態変化通知.
		 *	@note 電源状態が変化したら通知されます。
		 *	@param info 状態.
		 */
		virtual void OnPowerInfoChange(const POWER_BROADCAST_POWER_INFO& info) {}
	};


	//---------------------------


	/// コンストラクタ
	CPowerWatcher(void) : m_handle(NULL)
	{
		m_inner.pListener = NULL;
	}

	/// デストラクタ
	~CPowerWatcher(void)
	{
		Stop();
	}

	/**
	 * [取得] ハンドル取得.
	 *	@return ハンドル.
	 */
	HANDLE GetSafeHandle(void) const
	{
		return m_handle;
	}

	/**
	 * [設定] リスナー設定.
	 *	@param P リスナー.
	 */
	void SetListener(IListener* P)
	{
		m_inner.pListener = P;
	}

	/**
	 * [確認] 監視確認.
	 *	@retval true 監視中.
	 *	@retval false 未スタート.
	 */
	bool IsStarted(void) const
	{
		return m_handle != NULL;
	}

	/**
	 * [設定] 監視開始.
	 *	@param flag 監視フラグ。省略すると、全状態を監視します。
	 *	@retval true 成功。
	 *	@retval false 失敗。 詳細は ::GetLastError() で得られます。
	 */
	bool Start(DWORD flag = POWER_NOTIFY_ALL)
	{
		Stop();
		m_msgQueue.SetListener(&m_inner);
		if ( m_msgQueue.Start(3, true, _T("PowerWatch")) )
		{
			m_handle = ::RequestPowerNotifications(m_msgQueue, flag);
		}
		if ( m_handle != NULL )
		{
			return true;
		}
		DWORD r = ::GetLastError();
		Stop();
		::SetLastError(r);
		return false;
	}

	/**
	 * [設定] 監視停止.
	 *	@note 監視開始していない場合、何もしません。
	 */
	void Stop(void)
	{
		if ( m_handle != NULL )
		{
			::StopPowerNotifications(m_handle);
			m_handle = NULL;
		}
		m_msgQueue.Stop();
	}

private:
	/// POWER_BROADCAST 構造体 改
	struct TMyPowerBroadcast
	{
		DWORD	Message;	///< メッセージ
		DWORD	Flags;		///< フラグ
		DWORD	Length;		///< 電源状態長さ
		union
		{
			POWER_BROADCAST_POWER_INFO	powerInfo;	///< 電源情報
			TCHAR						state[260];	///< 電源状態
		};
	};
	/// インナークラス
	class CInner : public CMsgQueueWatcherT<TMyPowerBroadcast>::IListener
	{
	public:
		/**
		 * [通知] 受信通知.
		 *	@note メッセージを受信したら通知されます。
		 *	@param data メッセージデータ.
		 *	@param isAlert true ならアラーとメッセージ.
		 */
		virtual void OnMsgQueueWatcherEvent(const TMyPowerBroadcast& data, size_t length, bool isAlert)
		{
			if ( pListener != NULL )
			{
				switch ( data.Message )
				{
				case PBT_POWERINFOCHANGE:
					ASSERT( data.Length == sizeof(POWER_BROADCAST_POWER_INFO) );
					pListener->OnPowerInfoChange(data.powerInfo);
					break;
				case PBT_POWERSTATUSCHANGE:
					pListener->OnPowerStatusChange();
					break;
				case PBT_RESUME:
					pListener->OnPowerResumeEvent();
					break;
				case PBT_TRANSITION:
					pListener->OnPowerTransitionEvent(data.Flags, data.state);
					break;
				}
			}
		}
		CPowerWatcher::IListener* pListener;	///< リスナー
	};
	CInner									m_inner;		///< インナー
	HANDLE									m_handle;		///< ハンドル
	CMsgQueueWatcherT<TMyPowerBroadcast>	m_msgQueue;		///< メッセージキュー
};



}; // TNB
