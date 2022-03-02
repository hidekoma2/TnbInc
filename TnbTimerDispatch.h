#pragma once
/**
 *@file
 * タイマー処理関係のヘッダ
 *
 *		Windowが無くてもメッセージポンプがなくてもタイマーを使うことができます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME THREAD
 * タイマー処理クラス
 *
 *		指定の時間間隔ごとに処理をさせることが出来ます。
 *
 *	@note	処理は、リスナーを実装したクラスを登録するか、本クラスを継承し
 *			OnTimer() をオーバーライドします。（推奨は前者）
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbTimerDispatch.h
 *
 *	@date 06/08/03	新規作成
 *	@date 06/10/25	OnSetup() メソッド追加。
 *	@date 10/03/19	ReferThreadStatus() 追加。
 *	@date 19/06/10	OnSetup() メソッドが機能していなかったのを修正。
 */
class CTimerDispatch
{
public:

	/**@ingroup TIME THREAD
	 * タイマー処理のリスナーインターフェース
	 *
	 *		CTimerDispatch のリスナーです。タイマーが発生すると通知されます。
	 *
	 *	@see CTimerDispatch::SetListener()
	 *
	 *	@par必要ファイル
	 *			TnbTimerDispatch.h
	 *
	 *	@date 06/08/02	新規作成
	 *	@date 06/10/25	OnSetup() メソッド追加。
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] タイマー発生.
		 *		SetTimer() で指定した時間が経過するとコールされます。 KillTimer() するまで、
		 *		定期的にコールされます。
		 *	@param uTimerId タイマーID
		 *	@param lParam SetTimer() で指定した 任意の32BIT値
		 *	@param timer SetTimerした CTimerDispatch オブジェクト。 SetTimer() KillTimer() を使用できます。
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnTimer(UINT_PTR uTimerId, LPARAM lParam, const CTimerDispatch& timer) = 0;

		/**
		 * [通知] 開始通知.
		 *	受信スレッドが開始されると、通知されます。
		 */
		virtual void OnSetup(void) {}
	};

private:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	/// 内部イベント用値
	enum
	{
		WMTT_SETTIMER	= WM_APP,		///< タイマー開始
		WMTT_KILLTIMER	= WM_APP + 1,	///< タイマー停止
	};

	/// 内部処理用パラメータ型
	struct TParam
	{
		UINT_PTR	uTimerId;		///< 戻り値
		UINT		uElapse;		///< 時間(ms)
		LPARAM		lParam;			///< 任意のパラメータ
		CSyncEvent	eventSuccess;	///< 戻り値格納イベント
		/// コンストラクタ
		TParam(UINT uEla, LPARAM lp) : uTimerId(0), uElapse(uEla), lParam(lp)
		{
			eventSuccess.ToLock();
		}
	};

	/// Dispatchリスナー
	class CListener : public CThreadDispatch::IListener
	{
		DEFPARENTLISTENER(CTimerDispatch, IParentListener);
	public:

		/// 初期設定
		void Set(CTimerDispatch* P, IParentListener* I)
		{
			m_pcTimer	 = P;
			m_piListener = I;
		}

	private:
		/// メッセージ受信.
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			switch ( nMsg )
			{
			case WMTT_SETTIMER:
				{
					TParam* P = reinterpret_cast<TParam*>(lParam);
					UINT_PTR i = ::SetTimer(NULL, 0, P->uElapse, reinterpret_cast<TIMERPROC>(P->lParam));
					ASSERT(i != 0);
					P->uTimerId = i;
					P->eventSuccess.Unlock();
				}
				break;
			case WMTT_KILLTIMER:
				::KillTimer(NULL, lParam);
				break;
			case WM_TIMER:
				ASSERTLIB(m_piListener != NULL);
				ASSERTLIB(m_pcTimer != NULL);
				return m_piListener->OnTimer(wParam, lParam, *m_pcTimer);
				break;
			default:
				break;
			}
			return true;
		}
		/// [通知] 開始通知
		virtual void OnThreadDispatchSetup(void)
		{
			ASSERTLIB(m_piListener != NULL);
			m_piListener->OnSetup();
		}
		IParentListener*	m_piListener;	///< TimerDispatchのリスナー
		CTimerDispatch*		m_pcTimer;		///< TimerDispatchの参照
	};

	friend class CTimerDispatch::CListener;	///< フレンド宣言
	CThreadDispatch		m_dispatch;			///< Dispatchクラス
	CListener			m_listener;			///< Dispatchリスナークラス

	/// デフォルトリスナー
	struct TDefaultListener : IListener
	{
		virtual bool OnTimer(UINT_PTR uTimerId, LPARAM lParam, const CTimerDispatch& timer)
		{
			return const_cast<CTimerDispatch&>(timer).OnTimer(uTimerId, lParam);
		}
	} m_lisDef;

	#endif //_TnbDOXYGEN

public:

	/// コンストラクタ
	CTimerDispatch(void)
	{
		m_listener.Set(this, &m_lisDef);
		m_dispatch.SetListener(&m_listener);
	}

	/// デストラクタ
	virtual ~CTimerDispatch(void)
	{
		Stop();
	}

	/**
	 * [登録] リスナー登録
	 *	@note	リスナーを登録しないと、タイマー発生時、本クラスの OnTimer() がコールされます。
	 *			（リスナーを使用するほうを推奨します）
	 *	@param P リスナー
	 */
	void SetListener(IListener* P)
	{
		m_listener.Set(this, (P != NULL) ? P : &m_lisDef);
	}

	/**
	 * [設定] スレッド開始
	 *	@param lpszName スレッドの名前（DEBUG以外では無視されます）
	 *	@retval true 成功。
	 *	@retval false 失敗(スレッド起動失敗/既にスレッド実行中)
	 */
	bool Start(LPCTSTR lpszName = NULL)
	{
		#ifdef _DEBUG
			if ( lpszName == NULL) { lpszName = _T("TimerDispatch:MessagePump"); }
		#endif
		return m_dispatch.Start(lpszName);
	}

	/**
	 * [設定] スレッド停止
	 *	@retval true 成功（スレッドを開始していない時やTerminateした時も成功としています）
	 *	@retval false 失敗（リスナースレッド内で Stop() を使用すると失敗します）
	 */
	bool Stop(void)
	{
		return m_dispatch.Stop();
	}

	/**
	 * [設定] タイマーセット
	 *	@param uElapse タイムアウト値 (ミリ秒指定)
	 *	@param lParam 任意の32BITの値。
	 *	@retval 0 失敗。
	 *	@retval 0以外 成功。値はタイマーID。
	 */
	UINT_PTR SetTimer(UINT uElapse, LPARAM lParam = 0) const
	{
		if ( m_dispatch.GetThreadId() == ::GetCurrentThreadId() )
		{
			return ::SetTimer(NULL, 0, uElapse, reinterpret_cast<TIMERPROC>(lParam));
		}
		TParam t(uElapse, lParam);
		if ( m_dispatch.PostMessage(WMTT_SETTIMER, 0, reinterpret_cast<LPARAM>(&t)) )
		{
			t.eventSuccess.Lock();	//Post先の処理が終わるのを待つ
			return t.uTimerId;		//Post先で値を入れてくれている
		}
		return 0;
	}

	/**
	 * [設定] タイマー破棄
	 *	@param uTimerId タイマーID。 SetTimer() の戻り値を使います。
	 *	@retval true 成功。(不正な タイマーIDを指定しても成功になります)
	 *	@retval false 失敗。( スレッドが動いていない)
	 */
	bool KillTimer(UINT_PTR uTimerId) const
	{
		if ( m_dispatch.GetThreadId() == ::GetCurrentThreadId() )
		{
			return !! ::KillTimer(NULL, uTimerId);
		}
		return m_dispatch.PostMessage(WMTT_KILLTIMER, 0, uTimerId);
	}

	/**
	 * [参照] 処理スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_dispatch.ReferThreadStatus();
	}

	/**
	 * [通知] タイマー発生.
	 *		SetTimer() で指定した時間が経過するとコールされます。 KillTimer() するまで、
	 *		定期的にコールされます。
	 *	@note	リスナーを登録しないと、タイマー発生時このメソッドがコールされます。
	 *			本クラスを継承し、本メソッドを継承することでも、処理が実装できます。
	 *			（リスナーを使用するほうを推奨します）
	 *	@param uTimerId タイマーID
	 *	@param lParam SetTimer() で指定した 任意の32BIT値
	 *	@retval true	正常
	 *	@retval false	切断要求
	 */
	virtual bool OnTimer(UINT_PTR uTimerId, LPARAM lParam)
	{
		ASSERT0(false, "CTimerDispatch", "継承もリスナ登録も行われずに使用されています。");
		return false;
	}

private:
	friend class CTimerDispatchTest;
};



};//TNB
