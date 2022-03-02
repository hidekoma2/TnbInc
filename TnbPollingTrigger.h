#pragma once
/**
 *@file
 * ポーリング関係のヘッダ.
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup THREAD
 * ポーリングトリガークラス.
 *
 *		指定のポートインターフェースを監視し、変化したらリスナーに通知します。
 *
 *	@param TYP データの型。通常、BYTE WORD DWORD のいずれかを使います。
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbPollingTrigger.h
 * 
 *	@date 09/02/17 新規作成
 *	@date 10/03/19	ReferThreadStatus() 追加。
 */
template<typename TYP = BYTE>
class CPollingTriggerT : public CThread::IRunner
{
public:

	/**@ingroup THREAD
	 * ポーリングトリガーポートインターフェース.
	 *
	 *		CPollingTriggerT でデータをポーリングするポートを実装するために使います。
	 *
	 *	@par必要ファイル
	 *			TnbPollingTrigger.h
	 * 
	 *	@date 09/02/17 新規作成
	 */
	struct IPort
	{
		/// デストラクタ
		virtual ~IPort(void) {}
		/**
		 * [取得] ポートデータ取得
		 *	@param[out] _dat 取得データ
		 *	@retval 0 成功
		 *	@retval 0以外 失敗。リスナーの OnPollingError() で通知されます。
		 */
		virtual DWORD GetPortData(TYP& _dat) = 0;
	};

	/**@ingroup THREAD
	 * ポーリングトリガーリスナーインターフェース.
	 *
	 *		CPollingTriggerT でデータを受信(確定)を、通知するために使います。
	 *
	 *	@par必要ファイル
	 *			TnbPollingTrigger.h
	 * 
	 *	@date 09/02/17 新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}
		/**
		 * [通知] トリガー変化通知
		 *	@param changeBit 変化したBITが1に。
		 *	@param dataBit 変化後のBIT。
		 */
		virtual void OnPollingBitChanged(TYP changeBit, TYP dataBit) = 0;
		/**
		 * [通知] エラー通知
		 *	@param error エラーコード。
		 */
		virtual void OnPollingError(DWORD error) = 0;
	};


	//---------------------------------------


	/// コンストラクタ
	CPollingTriggerT(void) : m_pListener(NULL), m_interval(100), m_chatter(3), m_chatCount(0)
	{
	}

	/// デストラクタ
	virtual ~CPollingTriggerT(void)
	{
		m_thread.Stop();
	}

	/**
	 * [設定] ポーリング開始
	 *	@retval true 成功
	 *	@retval false 失敗.
	 */
	bool Start(void)
	{
		m_thread.Stop();
		m_thread.SetRunner(this);
		return m_thread.Start(_T("PollingTigger"));
	}

	/// [設定] クローズ.
	void Stop(void)
	{
		m_thread.Stop();
	}

	/**
	 * [設定] ポーリング設定
	 *	@note ポートとリスナーと設定間隔を指定します。
	 *	@param P ポート
	 *	@param L リスナー
	 *	@param interval チェック間隔(ms)
	 *	@param chatter 確定する数。
	 */
	void SetParameter(IPort* P, IListener* L, DWORD interval = 100, DWORD chatter = 3)
	{
		Stop();
		m_pPort = P;
		m_pListener = L;
		m_chatData.Resize(chatter);
		loop ( i, m_chatData.GetSize() )
		{
			m_chatData[i] = 0;
		}
		m_interval = interval;
		m_chatter = chatter;
		m_chatCount = chatter;
		m_fixData = 0;
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
	/// スレッド
	virtual DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			::Sleep(m_interval);
			TYP b;
			DWORD r = m_pPort->GetPortData(b);
			if ( r != 0 )
			{
				if ( m_pListener != NULL )
				{
					m_pListener->OnPollingError(r);
				}
			}
			else
			{
				m_Input(b);
			}
		}
		return 0;
	}
	/**
	 * 受信処理.
	 *	@note チャタリング処理
	 *	@param b 入力データ
	 */
	void m_Input(TYP b)
	{
		if ( m_chatter > 1 )
		{
			TYP cf = 0;	// 変化フラグ 最終的に 0 なら確定
			loop ( i, m_chatter - 1 )
			{
				TYP t = m_chatData[(m_chatCount - i) % m_chatter];
				cf |= TYP(t ^ b); //差異のあるBITは１になる。
			}
			TYP da = TYP((m_fixData & cf) | (b & ~cf));	//確定したものを格納。それ以外は前のまま
			TYP ch = TYP(m_fixData ^ da);				// 変化したら BITが１になる。
			m_fixData = da;
			if ( ch != 0 && m_pListener != NULL )
			{
				//TRACE2("  変化;[%02X] 確定;[%02X]\n", ch, da);
				m_pListener->OnPollingBitChanged(ch, da);
			}
			m_chatCount++;
			m_chatData[m_chatCount % m_chatter] = b;
		}
		else
		{
			TYP ch = TYP(m_fixData ^ b);				// 変化したら BITが１になる。
			m_fixData = b;
			if ( ch != 0 && m_pListener != NULL )
			{
				m_pListener->OnPollingBitChanged(ch, b);
			}
		}
	}

private:
	CThread				m_thread;		///< スレッド
	IPort*				m_pPort;		///< ポート
	IListener*			m_pListener;	///< リスナー
	DWORD				m_interval;		///< インターバル
	DWORD				m_chatter;		///< チャタ確定数
	CWorkMemT<TYP>		m_chatData;		///< チャタ用データ
	DWORD				m_chatCount;	///< チャタ用カウント
	TYP					m_fixData;		///< チャタ後決定値
	friend class CPollingTriggerTest;
};



/**@ingroup THREAD
 * ポーリングトリガークラス.
 *
 *		指定のポートインターフェースを監視し、変化したらリスナーに通知します。
 *
 *	@par必要ファイル
 *			TnbPollingTrigger.h
 * 
 *	@date 09/02/17 新規作成
 */
typedef CPollingTriggerT<BYTE> CPollingTrigger;



}; //TNB


