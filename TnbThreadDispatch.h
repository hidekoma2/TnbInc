#pragma once
/**
 *@file
 * スレッド処理関係のヘッダ
 *
 *		メッセージ通知で、別スレッド内で処理をさせることができます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup THREAD
 * スレッド処理制御クラス
 *
 *		メッセージ通知で、別スレッド内で処理をさせることができます。
 *
 *	@note	処理は、リスナーを実装したクラスを登録するか、本クラスを継承し
 *			OnThreadDispatchReceive() をオーバーライドします。（推奨は前者）
 *	
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbThreadDispatch.h
 *
 *	@date 06/04/01	新規作成
 *	@date 06/08/03	ThreadID取得メソッド追加。本クラスを継承して処理を実現させる方法も追加。
 *	@date 06/10/25	OnSetup() メソッド追加。
 *	@date 09/08/27	構成変更。
 *	@date 10/03/19	ReferThreadStatus() 追加。
 *	@date 11/01/06	DispatchMessage() を行うようにした。
 *	@date 11/02/03	IListener::OnThreadDispatchSetup() を呼ぶタイミングを変更。
 *	@date 12/05/31	IsDispatching() を追加。
 */
class CThreadDispatch : TNB::CThread::IRunner, CCopyImpossible
{
	typedef TNB::CThread::IRunner __runner;	///< IF名再宣言
public:

	/**@ingroup THREAD
	 * スレッド処理制御のリスナーインターフェース
	 *
	 *		CThreadDispatchのリスナーです。メッセージを受信すると通知されます。
	 *
	 *	@see CThreadDispatch::SetListener()
	 *
	 *	@par必要ファイル
	 *			TnbThreadDispatch.h
	 *
	 *	@date 06/04/01	新規作成
	 *	@date 06/10/25	OnSetup() メソッド追加。
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] メッセージ受信.
		 *		最後は必ず nMsg に WM_QUIT が入って通知されます。
		 *	@param nMsg メッセージ
		 *	@param wParam WPARAM
		 *	@param lParam LPARAM
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam) = 0;

		/**
		 * [通知] 開始通知.
		 *		受信スレッドが開始されると、通知されます。
		 *	@note CThreadDispatch::Start() から返る前に作成されたスレッド上から呼ばれます。
		 */
		virtual void OnThreadDispatchSetup(void) {}
	};


	//-------------------------


	/** コンストラクタ */
	CThreadDispatch(void) : m_isDispatching(false)
	{
		m_lisDef.pDisp = this;
		m_piListener = &m_lisDef;
		m_thread.SetRunner(this);
	}

	/** デストラクタ */
	virtual ~CThreadDispatch(void)
	{
		Stop();
	}

	/**
	 * [登録] リスナー登録
	 *	@note	リスナーを登録しないと、メッセージ受信時、本クラスの OnThreadDispatchReceive() がコールされます。
	 *			（リスナーを使用するほうを推奨します）
	 *	@param piListener リスナー
	 */
	void SetListener(IListener* piListener)
	{
		m_piListener = (piListener != NULL) ? piListener : &m_lisDef;
	}

	/**
	 * [設定] 優先度設定.
	 *		メッセージキュースレッドの優先度を指定できます。
	 *	@param priority 優先度（-2～2, 15。0がノーマル。大きいほうが優先度大）
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetPriority(int priority)
	{
		return m_thread.SetPriority(priority);
	}

	/**
	 * [取得] スレッドのID取得
	 *	@return ID
	 */
	DWORD GetThreadId(void) const
	{
		return m_thread.GetId();
	}

	/**
	 * [設定] スレッド開始
	 *	@param lpszName スレッドの名前（DEBUG以外では無視されます）
	 *	@retval true 成功。
	 *	@retval false 失敗(スレッド起動失敗/既にスレッド実行中)
	 */
	bool Start(LPCTSTR lpszName = NULL)
	{
		m_isDispatching = false;
		if ( m_thread.IsAlive() )
		{
			return false;
		}
		#ifdef _DEBUG
			if ( lpszName == NULL) { lpszName = _T("ThreadDispatch:MessagePump"); }
		#endif
		if ( m_thread.Start(lpszName) )
		{
			DWORD dwTick = ::GetTickCount();
			while ( (::GetTickCount() - dwTick) < 5000 )
			{
				if ( this->PostMessage(WM_NULL) )
				{
					return true;
				}
				::Sleep(100);
			}
			m_thread.Stop();
		}
		return false;
	}

	/**
	 * [設定] スレッド停止
	 *	@retval true 成功（スレッドを開始していない時やTerminateした時も成功としています）
	 *	@retval false 失敗（リスナースレッド内で Stop() を使用すると失敗します）
	 */
	bool Stop(void)
	{
		m_isDispatching = false;
		if ( m_thread.IsAlive() )
		{
			m_thread.PostMessage(WM_QUIT);
			return m_thread.Stop();
		}
		return true;
	}

	/**
	 * [確認] スレッドが生きているか
	 *	@retval true	生きている
	 *	@retval false	いない
	 */
	bool IsAlive(void) const
	{
		return m_thread.IsAlive();
	}

	/**
	 * [設定] スレッドへPostMessage
	 *	@param nMsg メッセージ
	 *	@param wParam WPARAM。省略可能。
	 *	@param lParam LPARAM。省略可能。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	bool PostMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		if ( m_thread.IsAlive() )
		{
			return m_thread.PostMessage(nMsg, wParam, lParam);
		}
		return false;
	}

	/**
	 * [確認] 処理中？
	 *	@retval true	処理中
	 *	@retval false	否
	 */
	bool IsDispatching(void) const
	{
		return m_isDispatching;
	}

	/**
	 * [通知] メッセージ受信.
	 *		最後は必ず nMsgにWM_QUITが入って通知されます。
	 *	@note	リスナーを登録しないと、メッセージ受信時このメソッドがコールされます。
	 *			本クラスを継承し、本メソッドを継承することでも、処理が実装できます。
	 *			（リスナーを使用するほうを推奨します）
	 *	@param nMsg メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@retval true	正常
	 *	@retval false	切断要求
	 */
	virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		ASSERT0(false, "CThreadDispatch", "継承もリスナ登録も行われずに使用されています。");
		return false;
	}

	/**
	 * [通知] 開始通知.
	 *		受信スレッドが開始されると、通知されます。
	 *	@note	リスナーを登録しないと、メッセージ受信時このメソッドがコールされます。
	 *			本クラスを継承し、本メソッドを継承することでも、処理が実装できます。
	 *			（リスナーを使用するほうを推奨します）
	 */
	virtual void OnThreadDispatchSetup(void)
	{
		ASSERT0(false, "CThreadDispatch", "継承もリスナ登録も行われずに使用されています。");
	}

	/**
	 * [参照] 処理スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

private:
	TNB::CThread	m_thread;			///< Dispatchスレッド管理	
	IListener*		m_piListener;		///< リスナー
	bool			m_isDispatching;	///< 処理中

	/**
	 * [通知] スレッド開始前通知.
	 *		CThread::Start() 実行時、リターンする前にこのメソッドが新規スレッド上でコールされます。
	 */
	virtual void OnSetupThread(void) 
	{
		if ( m_piListener != NULL )
		{
			m_piListener->OnThreadDispatchSetup();
		}
	}

	/**
	 * スレッド処理関数
	 *	@retval 0、固定
	 */
	virtual DWORD Run(void)
	{
		MSG msg;
		while ( ::GetMessage(&msg, NULL, 0, 0) > 0 )
		{
			if ( msg.message != WM_NULL && m_piListener != NULL )
			{
				m_isDispatching = true;
				bool r = m_piListener->OnThreadDispatchReceive(msg.message, msg.wParam, msg.lParam);
				m_isDispatching = false;
				if ( ! r )
				{
					this->PostMessage(WM_QUIT);
				}
				else
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
		}
		ASSERTLIB(msg.message == WM_QUIT);
		if ( m_piListener != NULL )
		{
			m_isDispatching = true;
			m_piListener->OnThreadDispatchReceive(msg.message, msg.wParam, msg.lParam);
			m_isDispatching = false;
		}
		return 0;
	}

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	/// デフォルトリスナ
	struct TDefaultListener : IListener
	{
		CThreadDispatch* pDisp;
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			return pDisp->OnThreadDispatchReceive(nMsg, wParam, lParam);
		}
		virtual void OnThreadDispatchSetup(void)
		{
			pDisp->OnThreadDispatchSetup();
		}
	} m_lisDef;

	#endif //_TnbDOXYGEN
};



/**@ingroup THREAD
 * マルチスレッド処理制御クラス
 *
 *		メッセージ通知で、複数のスレッドで同じ処理を同時に処理をさせることができます。
 *
 *	@note	処理は、リスナーを実装したクラスを登録します。
 *	
 *	@param COUNT スレッド数
 *
 *	@par必要ファイル
 *			TnbThreadDispatch.h
 *
 *	@date 12/03/23	新規作成
 */
template<size_t COUNT>
class CMultiThreadDispatchT : CCopyImpossible
{
public:

	/**@ingroup THREAD
	 * マルチスレッド処理制御のリスナーインターフェース
	 *
	 *		CMultiThreadDispatchのリスナーです。メッセージを受信すると通知されます。
	 *
	 *	@see CMultiThreadDispatch::SetListener()
	 *
	 *	@par必要ファイル
	 *			TnbThreadDispatch.h
	 *
	 *	@date 12/03/23	新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] メッセージ受信.
		 *		最後は必ず nMsg に WM_QUIT が入って通知されます。
		 *	@param nMsg メッセージ
		 *	@param wParam WPARAM
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnMultiDispatchReceive(UINT nMsg, WPARAM wParam) = 0;
	};


	//-----


	/// コンストラクタ
	CMultiThreadDispatchT(void)
	{
		m_in.pListener = NULL;
		loop ( i, COUNT )
		{
			m_in.tasks[i].SetListener(&m_in);
			m_in.isTaskWorking[i] = false;
		}
		m_in.taskIndex = 0;
	}

	/** 
	 * [設定] リスナー登録.
	 *	@param l リスナー
	 */
	void SetListener(IListener* l)
	{
		m_in.pListener = l;
	}

	/**
	 * [送信] メッセージ送信.
	 *		空いているスレッドにメッセージを通知します。空いていない場合、最初のスレッドに通知します。
	 *	@note リスナーの OnMultiDispatchReceive() に通知されます。
	 *	@param msg メッセージ.
	 *	@param wParam WPARAM
	 */
	void PostMessage(UINT msg, WPARAM wParam = 0)
	{
		INDEX tn = 0;
		loop ( i, COUNT )
		{
			tn = (i + m_in.taskIndex) % COUNT;
			if ( ! m_in.isTaskWorking[tn] )
			{
				break;
			}
		}
		m_in.isTaskWorking[tn] = true;
		m_in.tasks[tn].PostMessage(msg, wParam, tn);
	}

	/**
	 * [送信] メッセージ送信.
	 *		指定のスレッドにメッセージを通知します。
	 *	@note リスナーの OnMultiDispatchReceive() に通知されます。
	 *	@param index スレッドインデックス。 0以上、 COUNT 未満が指定できます。
	 *	@param msg メッセージ.
	 *	@param wParam WPARAM
	 */
	void PostMessageToOne(INDEX index, UINT msg, WPARAM wParam = 0)
	{
		if ( index < COUNT )
		{
			m_in.isTaskWorking[index] = true;
			m_in.tasks[index].PostMessage(msg, wParam, index);
		}
	}

	/**
	 * [設定] スレッド開始
	 *	@retval true 成功。
	 *	@retval false 失敗(スレッド起動失敗/既にスレッド実行中)
	 */
	bool Start(void)
	{
		bool r = true;
		m_in.taskIndex = 0;
		loop ( i, COUNT )
		{
			m_in.isTaskWorking[i] = false;
			#ifdef _DEBUG
				r &= m_in.tasks[i].Start(CStr::Fmt(_T("CMultiDispatch-%d"), i + 1));
			#else
				r &= m_in.tasks[i].Start();
			#endif
		}
		if ( ! r )
		{
			Stop();
		}
		return r;
	}

	/// [設定] スレッド停止
	void Stop(void)
	{
		loop ( i, COUNT )
		{
			m_in.tasks[i].Stop();
			m_in.isTaskWorking[i] = false;
		}
	}

private:
	/// 内部クラス
	struct TInner : CThreadDispatch::IListener
	{
		DEFPARENTLISTENER(typename CMultiThreadDispatchT<COUNT>, IParentListener);
		IParentListener*	pListener;					///< リスナー
		CThreadDispatch		tasks[COUNT];				///< タスク
		bool				isTaskWorking[COUNT];		///< タスク使用中？
		INDEX				taskIndex;					///< 次に使うタスクインデックス	
		/// [通知] メッセージ受信.
		///		最後は必ず nMsg に WM_QUIT が入って通知されます。
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			TRACE1("OnMultiDispatch-%d\n", lParam + 1);
			if ( pListener != NULL )
			{
				pListener->OnMultiDispatchReceive(nMsg, wParam);
			}
			isTaskWorking[lParam] = false;
			return true;
		}
	};
	TInner		m_in;	///< 内部情報
};



}; // TNB

