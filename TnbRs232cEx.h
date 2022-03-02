#pragma once
/**
 *@file
 * RS232Cのアクセス関係のヘッダ
 *
 *	@note	マクロ "_TnbRS232C_EnumExistPorts_DISABLE" が定義されていると、 CRs232c::EnumExistPorts() 
 *			を使用禁止にします。その代わり、 TnbRegistryAccessor.h は不必要になります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbRs232c.h"
#include "TnbThread.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

class COverlapped : CCopyImpossible
{
public:

	/// コンストラクタ
	COverlapped(void) : m_hEvent(NULL)
	{
		Zero(m_overLapped);
	}

	/// デストラクタ
	~COverlapped(void)
	{
		if ( m_hEvent != NULL )
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL; 
		}
	}

	/**
	 * [参照] OVERLAPPED 構造体参照.
	 *	@note 内容を設定し多構造体を返します。
	 *	@return OVERLAPPED 構造体参照
	 */
	operator OVERLAPPED*(void)
	{
		Zero(m_overLapped);
		if ( m_hEvent == NULL )
		{
			m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
			::ResetEvent(m_hEvent);
		}
		::ResetEvent(m_hEvent);
		m_overLapped.hEvent = m_hEvent; 
		return &m_overLapped;
	}
	
	/**
	 * [取得] 結果取得.
	 *	@param h ハンドル。
	 *	@param wait 待ち時間(ms)。 INFINITE を指定すると、結果が帰るまで Waitします。
	 *	@retval 0 タイムアウト（まだ結果無し）。
	 *	@retval INVALID_SIZE 読み込みエラー。
	 *	@retval 上記以外 読込み成功。
	 */
	size_t GetResult(HANDLE h, DWORD wait = 0)
	{
		if ( ::GetLastError() == ERROR_IO_PENDING )
		{
			// 非同期読み取り操作の結果をチェックする。
			::Sleep(0);
			DWORD work = 0;
			if( ::GetOverlappedResult(h, &m_overLapped, &work, FALSE) )
			{
				return work;
			}
			else if ( ::GetLastError() != ERROR_IO_INCOMPLETE )
			{
				return INVALID_SIZE;
			}
		}
		size_t result = INVALID_SIZE;
		DWORD r = 0;
		switch ( ::WaitForSingleObject(m_overLapped.hEvent, wait) )
		{
		case WAIT_OBJECT_0:
			if ( ::GetOverlappedResult(h, &m_overLapped, &r, FALSE) )
			{
				result = r; // 成功
				::CancelIo(h);
			}
			break;
		case WAIT_TIMEOUT:
			result = 0;
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
		default:
			::CancelIo(h);
			break;
		}
		return result;
	}
	HANDLE			m_hEvent;		///< イベントハンドル
	OVERLAPPED		m_overLapped;	///< オーバーラップ管理
};

#endif //_TnbDOXYGEN



/**@ingroup COMMUNICATION
 * 拡張RS232Cアクセスクラス.
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbRs232cEx.h
 * 
 *	@date 09/11/13	新規作成
 *	@date 10/03/19	ReferThreadStatus() 追加。
 */
class CRs232cEx : public CRs232c, CThread::IRunner
{
	DEFSUPER(CRs232c);
public:

	/**@ingroup COMMUNICATION
	 * 拡張RS232Cイベントリスナー
	 *
	 *	@par必要ファイル
	 *			TnbRs232cEx.h
	 * 
	 *	@date 09/11/13	新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] イベント通知.
		 *		RS232Cのイベントが発生したら、通知されます。
		 *	@param event イベント。以下のイベントが or で複数通知されます。クローズされた時は 0 が通知されます。
		 *		- EV_BREAK		-- 入力上でブレークを検出したとき。 
		 *		- EV_CTS		-- CTS（ 送信可）信号の状態が変わったとき。 
		 *		- EV_DSR		-- DSR（ データセットレディ）信号の状態が変わったとき。 
		 *		- EV_ERR		-- 回線状態エラーが発生したとき。回線状態エラーには、CE_FRAME、CE_OVERRUN、CE_RXPARITY があります。 
		 *		- EV_RING		-- 呼び出し信号を検出したとき。 
		 *		- EV_RLSD		-- RLSD（ 受信線信号検出）信号の状態が変わったとき。 
		 *		- EV_RXCHAR		-- 1 文字受信し、入力バッファに入れたとき。
		 *		- EV_RXFLAG		-- イベント文字を受信し、入力バッファに入れたとき。イベント文字はデバイスの DCB 構造体で指定します。この構造体を使ってシリアルポートを設定するには、SetCommState 関数を使います。 
		 *		- EV_TXEMPTY	-- 出力バッファの最後の文字を送信したとき。
		 */
		virtual void OnRs232cEvent(DWORD event) = 0;
	};


	//-----------------------------


	/// コンストラクタ
	CRs232cEx(void) : m_propertyEx(_T("RS232C"), TNBVIW_KIND_RS232C), m_pListener(NULL)
	{
		m_propertyEx = _super::GetProperty();
		m_propertyEx.receiveTimeout = 300;
		m_eventMask = EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY | EV_CTS | EV_DSR | EV_RLSD | EV_BREAK;
		m_eventMask |= EV_ERR | EV_RING | EV_PERR | EV_RX80FULL | EV_EVENT1 | EV_EVENT2;
	}

	/// デストラクタ
	~CRs232cEx(void)
	{
		Close();
	}


	/**
	 * [設定] 監視するイベント指定.
	 *	@param event 監視するイベント。以下のイベントを or で連結してしています。デフォルトは全て監視対象になっています。
	 *		- EV_BREAK		-- 入力上でブレークを検出したとき。 
	 *		- EV_CTS		-- CTS（ 送信可）信号の状態が変わったとき。 
	 *		- EV_DSR		-- DSR（ データセットレディ）信号の状態が変わったとき。 
	 *		- EV_ERR		-- 回線状態エラーが発生したとき。回線状態エラーには、CE_FRAME、CE_OVERRUN、CE_RXPARITY があります。 
	 *		- EV_RING		-- 呼び出し信号を検出したとき。 
	 *		- EV_RLSD		-- RLSD（ 受信線信号検出）信号の状態が変わったとき。 
	 *		- EV_RXCHAR		-- 1 文字受信し、入力バッファに入れたとき。
	 *		- EV_RXFLAG		-- イベント文字を受信し、入力バッファに入れたとき。イベント文字はデバイスの DCB 構造体で指定します。この構造体を使ってシリアルポートを設定するには、SetCommState 関数を使います。 
	 *		- EV_TXEMPTY	-- 出力バッファの最後の文字を送信したとき。
	 */
	void SetListenEvent(DWORD event)
	{
		m_eventMask = event;
	}

	/**
	 * [設定] リスナー登録.
	 *	@param P リスナー
	 */
	void SetListener(IListener* P)
	{
		m_pListener = P;
	}

	/**
	 * [取得] 性能取得
	 *	@note 正確な情報は Open後に取得できます。
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_propertyEx;
	}

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@return true 成功。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		m_propertyEx.receiveTimeout = to;
		return true;
	}

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		bool r = _super::Open();
		m_propertyEx.typeName = _super::GetProperty().typeName;
		m_propertyEx.typeId = _super::GetProperty().typeId;
		if ( r )
		{
			m_thread.SetRunner(this);
			return m_thread.Start();
		}
		Close();
		return false;
	}

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
		::SetCommMask(GetHandle(), 0);
		_super::Close();
		m_thread.Stop();
		m_propertyEx.typeName = _super::GetProperty().typeName;
		m_propertyEx.typeId = _super::GetProperty().typeId;
	}


	/**
	 * [処理] 受信.
	 *	@param[in] size サイズ。
	 *	@param[out] _P データ。size 分のメモリは確保しておく必要があります。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 0 受信データなし。
	 *	@retval 上記以外 受信データサイズ。
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		if ( size == 0 || m_propertyEx.receiveTimeout == 0 )
		{
			DWORD dwErrors;
			COMSTAT tComStat;
			if ( ! ::ClearCommError(GetHandle(), &dwErrors, &tComStat) )
			{
				_GetLastError("ClearCommError");
				return INVALID_SIZE;
			}
			if ( size > tComStat.cbInQue )
			{
				size = tComStat.cbInQue;
			}
			if ( size == 0 )
			{
				return 0;
			}
		}
		size_t redSize  = 0;
		if ( ReadPort(redSize, _P, size, GetHandle()) )
		{
			return redSize;
		}
		return INVALID_SIZE;
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
		::SetCommMask(GetHandle(), m_eventMask);
		DWORD event;
		while ( IsRunnable() )
		{
			::WaitCommEvent(GetHandle(), &event, m_eventOverlapped);
			if ( ::GetLastError() != ERROR_ACCESS_DENIED )
			{
				DWORD r = m_eventOverlapped.GetResult(GetHandle(), INFINITE);
				if ( r == sizeof(DWORD) )
				{
//					TRACE1("★ RS232C Event = 0x%04X\n", event);
					if ( m_pListener != NULL )
					{
						m_pListener->OnRs232cEvent(event);
					}
					if ( event == 0 )
					{
						break;
					}
				}
			}
			else
			{
				::SetCommMask(GetHandle(), m_eventMask);
			}
		}
		return 0;
	};

	/**
	 * [通知] ポート作成.
	 *		ポート作成時、コールされます。
	 *	@param lpszPortName ポート名
	 *	@retval INVALID_HANDLE_VALUE 失敗。
	 *	@retval 上記以外 成功。値はハンドル。使用後は CloseHandle() してください。
	 */
	virtual HANDLE CreatePort(LPCTSTR lpszPortName) const
	{
		DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;	// アクセス (読み取りおよび書き込み) モード
		DWORD flag = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
		HANDLE h = ::CreateFile(lpszPortName, desiredAccess, 0, NULL, OPEN_EXISTING, flag, NULL);
		_GetLastError("CreateFile");
		return h;
	}

	/**
	 * [通知] ポート書き込み.
	 *		ポート書き込み時、コールされます。
	 *	@param[out] _writtenSize 書き込んだサイズが格納されます。
	 *	@param[in] len 書き込みデータサイズ
	 *	@param[in] P 書き込みデータ
	 *	@param[in] h ハンドル
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool WritePort(size_t& _writtenSize, size_t len, LPCVOID P, HANDLE h) const
	{
		DWORD dwWritten = 0;
		::WriteFile(h, P, ToDword(len), &dwWritten, m_writeOverlapped);
		dwWritten = m_writeOverlapped.GetResult(h, INFINITE);
		if ( dwWritten == INVALID_SIZE )
		{
			return false;
		}
		_writtenSize = dwWritten;
		return true;
	}

	/**
	 * [通知] ポート読み込み.
	 *		ポート読み込み時、コールされます。
	 *	@param[out] _redSize 書き込んだサイズが格納されます。
	 *	@param[out] _P 読み込みデータが格納されます。
	 *	@param[in] len 読み込みデータ最大
	 *	@param[in] h ハンドル
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool ReadPort(size_t& _redSize, LPVOID _P, size_t len, HANDLE h) const
	{
		DWORD dwRed = 0;
		::ReadFile(h, _P, ToDword(len), &dwRed, m_readOverlapped);
		dwRed = m_readOverlapped.GetResult(h, m_propertyEx.receiveTimeout);
		if ( dwRed == INVALID_SIZE )
		{
			return false;
		}
		_redSize = dwRed;
		return true;
	}

private:
	DWORD					m_eventMask;		///< 監視対象
	IListener*				m_pListener;		///< リスナー
	TProperty				m_propertyEx;		///< 性能
	mutable CThread			m_thread;			///< スレッド
	mutable COverlapped		m_readOverlapped;	///< R オーバーラップ
	mutable COverlapped		m_writeOverlapped;	///< W オーバーラップ
	mutable COverlapped		m_eventOverlapped;	///< イベントオーバーラップ
};



}; // TNB
