#pragma once
/**
 *@file
 *@ingroup REPORT
 * Mailslotのアクセス関係のヘッダ
 *
 *		受信処理は、リスナーを登録することで行います。
 *
 */



#include "TnbDef.h"
#include "TnbAbstractReport.h"
#include "TnbCommon.h"
#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * Mailslot受信イベント管理クラス
 *
 *		データ（長さとアドレス）かイベントのいずれかを保持するクラスです。
 *		リスナーで使用します。
 *
 *	@see CMailslot::IListener::OnMailslotEvent()
 *
 *	@par必要ファイル
 *			TnbMailslot.h
 */
typedef TNB::CReportEvent CMailslotEvent;



/**@ingroup REPORT
 * Mailslotセンダークラス
 *
 *		送信専用のMailslotクラスです。二つのモードがあります（コンストラクタ時に指定します）。	<BR>
 *		一つは送信ごとにOpenし送信後Closeするモードです（こちらがDefaultモード）。					<BR>
 *		一つは指定があるまでCloseしないモードです（MaintainOpenモード）。						<BR>
 *
 *	@note 本クラスは受信機能がないので、 IReport::IListener を登録しても意味が有りません。
 *
 *	@par必要ファイル
 *			TnbMailslot.h
 * 
 *	@date 06/05/11 新規作成
 *	@date 06/08/31 CommViewer に対応。
 *	@date 06/09/09 
 *	@date 07/02/09 親クラス変更。
 */
class CMailslotSender: public CAbstractReport
{
	typedef CAbstractReport __super;

	CStr	m_strMailslotName;		///< メールスロット名
	CStr	m_strDomainName;		///< ドメイン名
	HANDLE	m_hMailslot;			///< ハンドル
	bool	m_boIsMaintainOpenMode;	///< trueオープンしっぱなし

	/**
	 * Mailslotをオープン
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD m_Open(void)
	{
		if ( m_strMailslotName.IsEmpty() )
		{
			return ERROR_INVALID_PARAMETER;
		}
		CStr str;
		if ( m_strDomainName.IsEmpty() )
		{
			str.Format(_T("\\\\.\\mailslot\\%s"), m_strMailslotName);
		}
		else
		{
			str.Format(_T("\\\\%s\\mailslot\\%s"), m_strDomainName, m_strMailslotName);
		}
		HANDLE h = ::CreateFile(
						str,			// メールスロット名
						GENERIC_WRITE,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL
		);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateFile");
		}
		m_hMailslot = h;
		return 0;
	}

	/**
	 * [確認] アクセス可能かチェック
	 *	@note Defaultモードの場合、trueを返します。
	 *	@retval true	アクセス可能。
	 *	@retval	false	アクセス不可能。
	 */
	bool m_CanAccess(void) const
	{
		if ( ! m_boIsMaintainOpenMode )
		{
			return true;
		}
		return m_hMailslot != NULL;
	}

	/**
	 * [操作] 停止
	 */
	void m_Stop(void)
	{
		if ( m_hMailslot != NULL )
		{
			::CloseHandle(m_hMailslot);
			m_hMailslot = NULL;
		}
	}

public:

	/**
	 * コンストラクタ
	 *	@param boIsMaintainOpenMode 省略かfalseなら、送信ごとにOpenし送信後Closeするモード（こちらがDefaultモード）。
	 *							trueなら、指定があるまでCloseしないモード（MaintainOpenモード）。						<BR>
	 */
	explicit CMailslotSender(bool boIsMaintainOpenMode = false)
	{
		m_hMailslot = NULL;
		m_boIsMaintainOpenMode = boIsMaintainOpenMode;
	}

	/**
	 * [操作] オープン.
	 *		指定の名前のメールスロットをオープンします。
	 *		すでにOpenしていたら一度クローズします。
	 *		一度設定すると次回からは引数なしのOpenを使用できます。
	 *	@note Defaultモードの場合、名前を記憶するだけで、常に成功します。
	 *	@param lpszMailslotName メールスロット名
	 *	@param lpszDomainName ドメイン名/コンピュータ名/* を指定。省略ならローカル。
	 *	@see http://msdn.microsoft.com/library/ja/jpipc/html/_win32_createmailslot.asp
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD Open(LPCTSTR lpszMailslotName, LPCTSTR lpszDomainName = NULL)
	{
		m_strMailslotName = lpszMailslotName;
		m_strDomainName = lpszDomainName;
		if ( m_CanAccess() )
		{
			m_Stop();	//接続中ならCloseします。
		}
		return Open();
	}

	/**
	 * [操作] オープン
	 *	@note	二回目以降、使用できます。
	 *	@note Defaultモードの場合、何もせず常に成功します。
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD Open(void)
	{
		if ( ! m_boIsMaintainOpenMode || m_CanAccess() )
		{
			return 0;
		}
		return m_Open();
	}

	/**
	 * [取得] コミュニケーション名.
	 *	@note 開発用のメソッドです。
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const
	{
		return CStr::Fmt(_T("MailslotSender[%s]:"), m_strMailslotName);
	}

	/**
	 * [取得] コミュニケーションID.
	 *		TNBVIW_KIND_??? + PORT を返します。
	 *	@note 開発用のメソッドです。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return TNBVIW_KIND_MAILS + (reinterpret_cast<DWORD>(m_hMailslot) & 0xFFFF);
	}

	/**
	 * [確認] 開始チェック
	 *	@note Defaultモードの場合、常にtrueを返します。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const
	{
		return m_CanAccess(); 
	}
	
	/**
	 * [確認] 接続チェック
	 *	@note Defaultモードの場合、常にtrueを返します。
	 *	@retval true	接続中。
	 *	@retval	false	切断。
	 */
	virtual bool IsConnect(void) const
	{
		return IsAlive();
	}

	/**
	 * [操作] 停止
	 */
	virtual void Stop(void)
	{
		m_Stop();
	}

	/**
	 * [送信] データ送信
	 *	@note Defaultモードの場合、オープン、送信、クローズ、という処理を行います。
	 *	@param sizeData 送信するデータ長。
	 *	@param lpcvData 送信するデータ。
	 *	@retval true	成功
	 *	@retval	false	失敗
	 */
	virtual bool Send(size_t sizeData, LPCVOID lpcvData)
	{
		__super::m_OnSend(sizeData, lpcvData);
		if ( ! m_boIsMaintainOpenMode )
		{
			DWORD dw = m_Open();
			if ( dw != 0 )
			{
				return false; 
			}
		}
		else
		{
			Open();
		}
		if ( m_CanAccess() )
		{
			DWORD dwWritten;
			if ( ::WriteFile(m_hMailslot, lpcvData, sizeData, &dwWritten, NULL) )
			{
				if ( dwWritten == sizeData )
				{
					if ( ! m_boIsMaintainOpenMode )
					{
						m_Stop(); 
					}
					return true;
				}
			}
		}
		if ( ! m_boIsMaintainOpenMode )
		{
			m_Stop();
		}
		return false;
	}
};



/**@ingroup REPORT
 * Mailslotクラス
 *
 *		受信設定は CMailslot::IListener か IReport::IListener のサブクラスを登録し行います。
 *
 *		送信処理は、 CMailslotSender のdefaultモードと同じ動作になります。
 *
 *	@par必要ファイル
 *			TnbMailslot.h
 * 
 *	@date 06/05/11	新規作成
 *	@date 06/07/07	InterfaceID概念、追加
 *	@date 06/08/31	CommViewer に対応。
 */
class CMailslot : public CAbstractReport, TNB::CThread::IRunner
{
	typedef CAbstractReport __super;
public:

	/**@ingroup REPORT
	 * Mailslotのリスナーインターフェース
	 *
	 *		CMailslot の通知用インターフェースです。
	 *
	 *	@see CMailslot::SetListener()
	 *
	 *	@par必要ファイル
	 *			TnbMailslot.h
	 * 
	 *	@date 06/05/11	新規作成
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] データを受信をした時など、コール
		 *	@note	本クラスのイベントは「{@link TNB::ECE_Error ECE_Error}」のみです。
		 *	@param ev 受信した内容（データかイベント）。
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnMailslotEvent(const CMailslotEvent& ev) = 0;
	};

private:

	CStr				m_strMailslotName;		///< メールスロット名
	IListener*			m_piMailslotListner;	///< リスナー
	CSyncSection		m_syncReceive;			///< レシーブの排他
	CThread				m_threadRecv;			///< 受信スレッド管理
	HANDLE				m_hMailslot;			///< Slot用ハンドル
	CMailslotSender		m_cMailslotSender;		///

	/**
	 * スレッド処理関数
	 *	@note	受信すると、コールバック関数をコールします。
	 *			受信失敗すると、ハンドルをクローズし、
	 *			コールバック関数へ通知し、スレッドを終了します。
	 *	@retval ０（固定)。
	 */
	virtual DWORD Run(void)
	{
		while( IsRunnable() )
		{
			::Sleep(1);
			//受信中排他ON
			m_syncReceive.Lock();
			// 現在の状態を取得。
			DWORD dwSize;
			if ( ! ::GetMailslotInfo(m_hMailslot, NULL, &dwSize, NULL, NULL) )
			{
				DWORD dwErr = _GetLastError("GetMailslotInfo");
				//受信中排他OFF
				m_syncReceive.Unlock();
				//APIエラーを、通知。
				if ( ! m_OnMailslotEvent(CMailslotEvent(this, ECE_Error, dwErr)) )
				{
					break;
				}
				continue;
			}
			if ( ! CanAccess() )
			{
				m_syncReceive.Unlock();
				break;	//Closeされたら、終了
			}
			// 現在受信しているDataを取得
			if ( dwSize != MAILSLOT_NO_MESSAGE )
			{
				// 1Byte以上ある。
//				CPointerHandleT<BYTE,PDT_Array> hpBuff = new BYTE[dwSize];
				CWorkMem buf(dwSize);
				DWORD dwReadBytes = 0;
				// Mailslotからの読み込み
//				BOOL boRc = ::ReadFile(m_hMailslot,hpBuff,dwSize,&dwReadBytes,NULL);
				BOOL boRc = ::ReadFile(m_hMailslot, buff, dwSize, &dwReadBytes, NULL);
				m_syncReceive.Unlock();		//受信中排他OFF
				if ( ! boRc || dwReadBytes == 0 )
				{
					//失敗
					DWORD dwErr = _GetLastError("ReadFile");
					if ( ! m_OnMailslotEvent(CMailslotEvent(this, ECE_Error, dwErr)) )
					{
						break;
					}
					continue;
				}
				//コールバック関数へ通知。
				CMailslotEvent ev(this);
//				ev.SetData(dwReadBytes, hpBuff);
				ev.SetData(dwReadBytes, buff);
				if ( ! m_OnMailslotEvent(ev) )
				{
					break;
				}
			}
			else 
			{
				//---受信データなし
				//受信中排他OFF
				m_syncReceive.Unlock();
				::Sleep(200);
			}
		}
		//コールバック関数へ通知。
		m_OnMailslotEvent(CMailslotEvent(this, ECE_End));
		m_syncReceive.Lock();
		if ( CanAccess() )
		{
			HANDLE h = m_hMailslot;
			m_hMailslot = NULL;
			::CloseHandle(h);
		}
		m_syncReceive.Unlock();
		return 0;
	}

	/**
	 * Mailslotをオープン
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD m_Open(void)
	{
		if ( m_strMailslotName.IsEmpty() )
		{
			return ERROR_INVALID_PARAMETER;
		}
		CStr str = CStr::Fmt(_T("\\\\.\\mailslot\\%s"), m_strMailslotName);
		HANDLE h = ::CreateMailslot(
						str,					// メールスロット名
						0,						// 最大メッセージサイズ
						MAILSLOT_WAIT_FOREVER,	// 読み取りタイムアウト
						NULL					// 継承オプション
		);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateMailslot");
		}
		m_hMailslot = h;
		return 0;
	}

	/**
	 * コールバック通知実行
	 *	@param ev 通知する内容
	 *	@retval true	正常
	 *	@retval false	切断要求
	 */
	bool m_OnMailslotEvent(CMailslotEvent& ev)
	{
		bool boRc = __super::m_OnEvent(ev);
		if ( m_piMailslotListner != NULL )
		{
			boRc = m_piMailslotListner->OnMailslotEvent(ev);
		}
		return boRc;
	}

public:

	/** 
	 * コンストラクタ 
	 *	@param dwInterfaceId インターフェースID。省略すると０。
	 *				(これは、複数の通信クラスで共通のリスナーを使用する時、識別として使用します)
	 */
	explicit CMailslot(DWORD dwInterfaceId=0)
		: __super(dwInterfaceId), m_cMailslotSender(false), m_hMailslot(NULL)
	{
	}

	/** デストラクタ */
	virtual ~CMailslot(void)
	{
		Stop();
	}

	/**
	 * [取得] コミュニケーション名.
	 *	@note 開発用のメソッドです。
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const
	{
		return CStr::Fmt(_T("Mailslot[%s]:"), m_strMailslotName);
	}

	/**
	 * [取得] コミュニケーションID.
	 *		TNBVIW_KIND_??? + PORT を返します。
	 *	@note 開発用のメソッドです。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const
	{
		return TNBVIW_KIND_MAILS + (reinterpret_cast<DWORD>(m_hMailslot) & 0xFFFF);
	}

	/**
	 * [確認] 開始チェック
	 *	@note Defaultモードの場合、常にtrueを返します。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const 
	{
		return m_hMailslot != NULL;	
	}
	
	/**
	 * [確認] 接続チェック
	 *	@note Defaultモードの場合、常にtrueを返します。
	 *	@retval true	接続中。
	 *	@retval	false	切断。
	 */
	virtual bool IsConnect(void) const 
	{
		return IsAlive();
	}

	/**
	 * [登録] Listnerの登録
	 *	@note 登録できる CMailslot::IListener は一つだけです。
	 *	@param piMailslotListner リスナー。省略すると解除になります。
	 */
	void SetListener(IListener* piMailslotListner = NULL)
	{
		m_piMailslotListner = piMailslotListner;
	}

	/**
	 * [操作] オープン.
	 *		指定の名前のメールスロットをオープンします。
	 *		すでにOpenしていたら一度クローズします。
	 *		一度設定すると次回からは引数なしのOpenを使用できます。
	 *	@see http://msdn.microsoft.com/library/ja/jpipc/html/_win32_createmailslot.asp
	 *	@param lpszMailslotName		受信用メールスロット名
	 *	@param lpszSendMailslotName 送信先メールスロット名。送信しないなら指定しなくても可。
	 *	@param lpszSendDomainName	送信先ドメイン名/コンピュータ名/* を指定。省略ならローカル。
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD Open(LPCTSTR lpszMailslotName, LPCTSTR lpszSendMailslotName = NULL, LPCTSTR lpszSendDomainName = NULL)
	{
		m_cMailslotSender.Open(lpszSendMailslotName, lpszSendDomainName);
		m_strMailslotName = lpszMailslotName;
		if ( CanAccess() )
		{
			Stop();	//接続中ならCloseします。
		}
		return Open();
	}

	/**
	 * [操作] オープン
	 *	@note	二回目以降、使用できます。
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD Open(void)
	{
		if ( CanAccess() )
		{
			return 0;
		}
		DWORD dwOpenRc = m_Open();
		if ( dwOpenRc == 0 )
		{
			m_threadRecv.SetRunner( this ) ;
			#ifdef _DEBUG
				m_threadRecv.Start(CStr::Fmt(_T("Mailslot[%s]:監視"), m_strMailslotName));
			#else
				m_threadRecv.Start();
			#endif
		}
		return dwOpenRc;
	}

	/**
	 * [操作] クローズ
	 *	@note 受信用スレッドも停止します。
	 */
	virtual void Stop(void)
	{
		m_threadRecv.Stop();
		if ( CanAccess() )
		{
			//Portのクローズ
			m_syncReceive.Lock();
			HANDLE h = m_hMailslot;
			m_hMailslot = NULL;
			::CloseHandle(h);
			m_syncReceive.Unlock();
		}
	}

	/**
	 * [送信] データ送信
	 *	@param sizeData 送信するデータ長。
	 *	@param lpcvData 送信するデータ。
	 *	@retval true	成功
	 *	@retval	false	失敗
	 */
	virtual bool Send(size_t sizeData, LPCVOID lpcvData)
	{
		__super::m_OnSend(sizeData, lpcvData);
		return m_cMailslotSender.Send(sizeData, lpcvData);
	}

	/**
	 * [取得] ハンドル取得
	 *	@return ハンドル
	 */
	operator HANDLE&()
	{
		return m_hMailslot;
	}
};



}; // TNB
