#pragma once
/**
 *@file
 * CE 用メッセージキュー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeMsgQueueWatcher.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbThread.h"
#include <Msgqueue.h>



//TNB Library
namespace TNB
{



/**@ingroup WINCE THREAD
 * メッセージキュー監視 (CE専用).
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 * 
 *	@par必要ファイル
 *			TnbCeMsgQueueWatcher.h
 *
 *	@date 09/10/01	新規作成
 *	@date 10/03/19	ReferThreadStatus() 追加。
 *	@date 10/03/30	Open/Close -> Start/Stop に変更。
 */
template<typename TYP>
class CMsgQueueWatcherT : CThread::IRunner
{
public:

	/**@ingroup WINCE THREAD
	 * メッセージキュー監視のリスナーインターフェース (CE専用).
	 *
	 *	@par必要ファイル
	 *			TnbCeMsgQueueWatcher.h
	 * 
	 *	@date 09/10/01	新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] 受信通知.
		 *	@note メッセージを受信したら通知されます。
		 *	@param data メッセージデータ.
		 *	@param length 受信データ長さ.
		 *	@param isAlert true ならアラーとメッセージ.
		 */
		virtual void OnMsgQueueWatcherEvent(const TYP& data, size_t length, bool isAlert) = 0;
	};


	//---------------------------


	/// コンストラクタ
	CMsgQueueWatcherT(void) : m_handle(NULL), m_pListener(NULL)
	{
	}

	/// デストラクタ
	~CMsgQueueWatcherT(void)
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
	 * [取得] ハンドル取得.
	 *	@return ハンドル.
	 */
	operator HANDLE(void) const
	{
		return m_handle;
	}

	/**
	 * [設定] リスナー設定.
	 *	@param P リスナー.
	 */
	void SetListener(IListener* P)
	{
		m_pListener = P;
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
	 *	@param maxMessagesCount キューの大きさ.
	 *	@param isRead true なら読み込み、 false なら書き込みでオープンします。
	 *	@param lpszThreadName スレッド名。デバッグ用です。
	 *	@retval true 成功。
	 *	@retval false 失敗。 詳細は ::GetLastError() で得られます。
	 */
	bool Start(size_t maxMessagesCount = 0, bool isRead = true, LPCTSTR lpszThreadName = NULL)
	{
		MSGQUEUEOPTIONS opt = { 0 };
		opt.dwSize = sizeof(MSGQUEUEOPTIONS);
		opt.dwFlags = 0;
		opt.dwMaxMessages = maxMessagesCount;
		if ( maxMessagesCount == 0 )
		{
			opt.dwFlags = MSGQUEUE_NOPRECOMMIT;
		}
		opt.cbMaxMessage = sizeof(TYP);
		opt.bReadAccess = isRead;
		return m_Start(NULL, opt, lpszThreadName);
	}

	/**
	 * [設定] 監視開始.
	 *	@param lpszQueueName キュー名.
	 *	@param option オプション.
	 *	@param lpszThreadName スレッド名。デバッグ用です。
	 *	@retval true 成功。
	 *	@retval false 失敗。 詳細は ::GetLastError() で得られます。
	 */
	bool Start(LPCTSTR lpszQueueName, const MSGQUEUEOPTIONS& option, LPCTSTR lpszThreadName = NULL)
	{
		MSGQUEUEOPTIONS opt = option;
		return m_Start(lpszQueueName, opt,lpszThreadName);
	}

	/**
	 * [設定] 監視停止.
	 *	@note 開始していない場合、何もしません。
	 *	@retval true 成功。
	 *	@retval false 失敗。 詳細は ::GetLastError() で得られます。
	 */
	bool Stop(void)
	{
		bool r = true;
		if ( m_handle != NULL )
		{
			r = !! ::CloseMsgQueue(m_handle);
			m_handle = NULL;
		}
		m_thread.Stop();
		return r;
	}

	/**
	 * [設定] 送信.
	 *	@param data 送信データ.
	 *	@param timeout タイムアウト(ms)。
	 *	@param isAlert true ならアラート、 false なら通常送信。
	 *	@retval true 成功。
	 *	@retval false 失敗。 詳細は ::GetLastError() で得られます。
	 */
	bool Send(const TYP& data, DWORD timeout = INFINITE, bool isAlert = false)
	{
		DWORD fg = isAlert ? MSGQUEUE_MSGALERT : 0;
		TYP t = data;
		return !! ::WriteMsgQueue(m_handle, &t, sizeof(data), timeout, fg);
	}

	/**
	 * [取得] キュー情報.
	 *	@param[out] _info 情報が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。 詳細は ::GetLastError() で得られます。
	 */
	bool GetInfo(MSGQUEUEINFO& _info) const
	{
		_info.dwSize = sizeof(MSGQUEUEINFO);
		return !! ::GetMsgQueueInfo(m_handle, &_info);
	}

	/**
	 * [参照] 監視スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

protected:
	
	/** 
	 * [動作] スレッド処理本体
	 *	@note この関数にスレッドの処理を記述します。
	 *		ループで IsRunnable() をコールし、偽なら関数をリターンするように、
	 *		する必要が有ります。
	 *	@return 戻り値。 CThread::GetThreadResult() で得られる値です。
	 */
	virtual DWORD Run(void)
	{
		while ( IsRunnable() && IsStarted() )
		{
//			DWORD dw = ::MsgWaitForMultipleObjects(1, &m_handle, FALSE, INFINITE, QS_ALLEVENTS);
			DWORD dw = ::WaitForSingleObject(m_handle, INFINITE);
			if ( dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED_0 )
			{
				TYP buf;
				DWORD dwRead = 0;
				DWORD dwFlags = 0;
				while ( ::ReadMsgQueue(m_handle, &buf, sizeof(TYP), &dwRead, 0, &dwFlags) )
				{
					if ( m_pListener != NULL )
					{
						m_pListener->OnMsgQueueWatcherEvent(buf, dwRead, dwFlags == MSGQUEUE_MSGALERT);
					}
				}
				DWORD err = ::GetLastError();
				if ( err != ERROR_TIMEOUT && err != ERROR_INVALID_HANDLE )
				{
					if ( ERROR_INSUFFICIENT_BUFFER == _GetLastError("ReadMsgQueue") )
					{
						ASSERT( false );
					}
				}
			}
		}
		return 0;
	}

private:
	bool m_Start(LPCTSTR lpszQueueName, MSGQUEUEOPTIONS& _opt, LPCTSTR lpszThreadName)
	{
		Stop();
		_opt.dwSize = sizeof(MSGQUEUEOPTIONS);
		m_handle = ::CreateMsgQueue(lpszQueueName, &_opt);
		if ( m_handle != NULL )
		{
			m_thread.SetRunner(this);
			if ( lpszThreadName == NULL )
			{
				lpszThreadName = _T("CMsgQueueWatcher");
			}
			if ( m_thread.Start(lpszThreadName) )
			{
				return true;
			}
		}
		Stop();
		return false;
	}

	HANDLE		m_handle;			///< ハンドル
	IListener*	m_pListener;		///< リスナー
	CThread		m_thread;			///< スレッド
};



}; // TNB