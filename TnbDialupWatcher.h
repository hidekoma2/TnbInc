#pragma once
/**
 *@file
 * ダイアルアップ監視関係のヘッダ
 *
 *		指定のダイアルアップの監視を行います。
 *		接続時間、送受信Byteを得ることができます。
 *
 *	@note Windows の RAS－API を使用しています。
 *	@note Windows の IPHELPER－API を使用しています。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDialupManager.h"
#include "TnbTime.h"
#include "TnbIcmp.h"
#include "TnbIpHelper.h"



//TNB Library
namespace TNB
{



/**@ingroup RAS
 * ダイアルアップ接続監視クラス
 *
 *		クラス一つにつき、一つのダイアルアップエントリを監視できます。
 *
 *	@note 本クラスにDialUpの機能は有りません
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@todo	送受信数、接続時間をDWORDで管理しているため、4Gでオーバーフローします。
 *			対策は可能か？
 *
 *	@par必要ファイル
 *			TnbDialupWatcher.h
 *
 *	@date 06/04/01；新規作成。
 *	@date 06/06/29；リスナー追加。 
 *	@date 10/03/19	ReferThreadStatus() 追加。
 */
class CDialupWatcher : TNB::CThread::IRunner , CCopyImpossible
{
	typedef CDialupManager __manage;		///< 管理クラス名再定義
	enum
	{
		E_CheckInterval = 500		///< チェック間隔
	};
public:

	/**@ingroup RAS
	 * ダイアルアップ接続状態情報型
	 *
	 *		時間やサイズなどを一括管理します 
	 *
	 *	@par必要ファイル
	 *			TnbDialupWatcher.h
	 *
	 *	@date 06/04/01；新規作成。
	 */
	struct TConnectInfo
	{
		/** 接続時間の状態
		 *	trueなら怪しい（監視開始時接続されていたため）
		 *	falseなら正しい。
		 */
		bool		boIsDoubtfulConnectSpan;
		CSystemTime	timeConnectTime;			///< 接続開始時間 
		DWORD		dwConnectingSpan;			///< 接続継続時間 ms						
		bool		boIsValidIfRow;				///< 接続情報は true;有効/false;無効	
		MIB_IFROW	tMibIfRow;					///< 接続情報							
		/** [設定] 初期化 */
		void Init(void)
		{
			boIsValidIfRow			= false;
			boIsDoubtfulConnectSpan	= true;
			timeConnectTime			= CSystemTime::GetCurrent();
			dwConnectingSpan		= 0;
		}
		/** [取得] 開始時間取得 @return 開始時間 */
		CSystemTime GetStartTime(void)
		{
			return timeConnectTime;
		}
		/** [取得] 終了時間取得 @return 終了時間 */
		CSystemTime GetEndTime(void)
		{
			return timeConnectTime.AddMillSec(dwConnectingSpan);
		}
	};

	/**@ingroup RAS
	 * ダイアルアップ接続監視のリスナーインターフェース
	 *
	 *	@par必要ファイル
	 *			TnbDialupWatcher.h
	 *
	 *	@date 06/06/29；新規作成。
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] 接続
		 *	@param boIsDoubtfulConnectSpan 接続時間の状態。
		 *				trueなら怪しい（監視開始時接続されていたため）
		 *				falseなら正しい。
		 */
		virtual void OnDialConnect(bool boIsDoubtfulConnectSpan) = 0;

		/**
		 * [通知] 切断
		 *	@param t 接続時の内容。
		 */
		virtual void OnDialDisconnect(const TConnectInfo& t) = 0;
	};

protected:

	CThread			m_threadWatch;		///< スレッド管理			
	CStr			m_strPhoneBook;		///< 電話帳名				
	CStr			m_strEntryName;		///< エントリ名				
	DWORD			m_dwConnectedTick;	///< 接続した時の時間		
	HRASCONN		m_hRasConn;			///< 接続中のハンドル		
	INDEX			m_mibIndex;			///< 接続中のMIBインデックス
	CByteVector		m_vbIfTableTmp;		///< GetIfTable用テンポラリ	
	TConnectInfo	m_tInfo;			///< 接続状態				
	int				m_iCheckInterval;	///< 状態チェックの間隔 ms	
	IListener*		m_piListener;		///< リスナー		

	/** 
	 * スレッド処理本体
	 *	@retval 0（固定）。
	 */
	virtual DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			//==接続監視ループ
			if ( m_hRasConn == NULL )
			{
				while ( IsRunnable() )
				{
					m_hRasConn = __manage::GetRasConn(m_strPhoneBook, m_strEntryName);
					if ( m_hRasConn != NULL )
					{
						//接続された
						TRACE1("CDialupWatcher::接続検知[%s]！\n", m_strEntryName);
						m_tInfo.boIsDoubtfulConnectSpan = false;
						if ( m_piListener != NULL )
						{
							m_piListener->OnDialConnect(false);
						}
						break;
					}
					::Sleep(m_iCheckInterval);
				}
				if ( ! IsRunnable() )
				{
					break;
				}
			}
			//
			m_dwConnectedTick			= ::GetTickCount();
			m_tInfo.timeConnectTime		= CSystemTime::GetCurrent();
			m_tInfo.dwConnectingSpan	= 0;
			CAscii strAddress				= __manage::GetIpAddress(m_hRasConn);
			ASSERTLIB(! strAddress.IsEmpty());
			m_mibIndex = CIpHelper::GetMibIndex(strAddress);
			CIpHelper::GetMibIfRow(m_mibIndex, m_tInfo.tMibIfRow);
			m_tInfo.boIsValidIfRow = true;
			//
			//==切断監視ループ
			RASCONNSTATUS tStatus;
			while ( IsRunnable() )
			{
				::Sleep(m_iCheckInterval);
				CIpHelper::GetMibIfRow(m_mibIndex, m_tInfo.tMibIfRow);
				m_tInfo.dwConnectingSpan = ::GetTickCount() - m_dwConnectedTick;
				tStatus.dwSize = sizeof(RASCONNSTATUS);
				DWORD dwRc = ::RasGetConnectStatus(m_hRasConn, &tStatus);
				if ( dwRc != 0 || tStatus.rasconnstate == RASCS_Disconnected )
				{
					//切断された
					m_hRasConn = NULL;
					TRACE1("CDialupWatcher::切断検知[%s]！\n", m_strEntryName);
					TRACE1("   接続時間 = %d秒\n", m_tInfo.dwConnectingSpan / 1000);
					TRACE1("             (%d分)\n", m_tInfo.dwConnectingSpan / 1000 / 60);
					TRACE1("   受信Byte = %dbyte\n", m_tInfo.tMibIfRow.dwInOctets);
					TRACE1("   送信Byte = %dbyte\n", m_tInfo.tMibIfRow.dwOutOctets);
					if ( m_piListener != NULL )
					{
						m_piListener->OnDialDisconnect(m_tInfo);
					}
					break;
				}
			}
		}
		m_tInfo.Init();
		return 0;
	}

public:

	/// コンストラクタ
	CDialupWatcher(void) :m_iCheckInterval(E_CheckInterval), m_piListener(NULL), m_hRasConn(NULL)
	{
		m_tInfo.Init();
		m_threadWatch.SetRunner(this);
	}

	/// デストラクタ
	virtual ~CDialupWatcher(void)
	{
		Stop();
	}

	/**
	 * [登録] Listner登録
	 *	@note 登録できる CDialupWatcher::IListener は一つだけです。
	 *	@param I リスナー。省略すると解除になります。
	 */
	void SetListener(IListener* I = NULL)
	{
		m_piListener = I;
	}

	/**
	 * [確認] 接続チェック
	 *	@retval true	接続中。
	 *	@retval false	監視スタートしていない or接続していない。
	 */
	bool IsConnect(void)
	{
		return m_hRasConn != NULL;
	}

	/**
	 * [設定] 接続情報の初期化.
	 *		通常切断時に使用します。
	 */
	void ClearConnectInfo(void)
	{
		m_tInfo.Init();
	}

	/**
	 * [参照] 接続情報参照
	 *	@return 接続中は現在の情報。違う場合、最後の切断時の状態の情報。
	 */
	const TConnectInfo& GetConnectInfo(void)
	{
		if ( IsConnect() )
		{	
			CIpHelper::GetMibIfRow(m_mibIndex, m_tInfo.tMibIfRow);
			m_tInfo.dwConnectingSpan = ::GetTickCount() - m_dwConnectedTick;
		}
		return m_tInfo;
	}

	/**
	 * [参照] 接続時間参照
	 *	@return 接続中は現在の情報。違う場合、最後の切断時の状態の情報。
	 */
	DWORD GetConnectSpan(void)
	{
		return GetConnectInfo().dwConnectingSpan;
	}

	/**
	 * [操作] 監視停止.
	 *		監視を停止します。
	 */
	void Stop(void)
	{
		m_threadWatch.Stop();
		m_strEntryName.Empty();
		m_strPhoneBook.Empty();
		m_hRasConn = NULL;
		m_tInfo.Init();
	}

	/**
	 * [操作] 監視開始.
	 *		指定の内容で監視します。
	 *	@param lpszPhoneBook 電話帳名。NULL指定可能。
	 *	@param lpszEntryName エントリ名
	 *	@param iCheckInterval 監視間隔。省略すると500ms。
	 *	@retval true 監視開始
	 *	@retval false 失敗（エントリー名が不正の場合）
	 */
	bool Start(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryName, int iCheckInterval = E_CheckInterval)
	{
		if ( m_threadWatch.IsAlive() )
		{
			return false;
		}
		__manage::TDialParams tParam = __manage::GetDialParams(lpszPhoneBook, lpszEntryName);
		if ( tParam.IsInvalid() )
		{
			return false;
		}
		m_strPhoneBook = lpszPhoneBook;
		m_strEntryName = lpszEntryName;
		m_tInfo.boIsValidIfRow = false;
		m_hRasConn = __manage::GetRasConn(lpszPhoneBook, lpszEntryName);
		m_tInfo.boIsDoubtfulConnectSpan = (m_hRasConn != NULL);
		if ( m_hRasConn != NULL )
		{
			//接続された
			TRACE1("CDialupWatcher::すでに接続中検知[%s]！\n", m_strEntryName);
			if ( m_piListener != NULL )
			{
				m_piListener->OnDialConnect(true);
			}
		}
		m_iCheckInterval = iCheckInterval;
		#ifdef _DEBUG
			m_threadWatch.Start(CStr::Fmt(_T("<%s>監視"), lpszEntryName));
		#else
			m_threadWatch.Start();
		#endif
		return true;
	}

	/**
	 * [参照] 監視スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadWatch;
	}
};



}; // TNB
