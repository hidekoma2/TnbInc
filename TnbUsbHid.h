#pragma once
/**
 *@file
 * USB HIDのアクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCommunication.h"
#include "TnbVector.h"
#include "TnbUsbHidPathName.h"
#include "TnbOverlapReader.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION USB
 * USB-HIDアクセスクラス
 *
 *		HIDデバイスの入出力をサポートします。
 *	@note オープン後に、デバイスが抜かれた場合、 
 *			IsConnect() で false が返ります（ IsOpened() は true ）。
 *
 *	@see CUsbHidFinder によりHIDネームを取得して使用します。
 *
 *	@par必要ファイル
 *			TnbUsbHid.h
 * 
 *	@date 06/09/04	新規作成
 *	@date 06/09/09	IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *					ECE_Connect , ECE_Disconnect イベント、追加。
 *	@date 06/12/08	指定デバイスの存在チェックメソッド、追加。
 *	@date 07/02/09	親クラス変更。
 *	@date 07/04/20	ReadでロックしてClose出来ないのを改善。
 *	@date 07/04/25	検索機能は別クラスへ。DevicePathNameでOpenするように変更。
 *	@date 07/06/11	親クラスの Start() メソッドに対応。
 *	@date 07/09/13	親クラス変更。
 *	@date 08/02/12	Report IDの設定可能に。
 *	@date 09/03/19	読み込みに COverlapReader を使うように修正。
 *	@date 09/04/09	SetReceiveTimeout() を新規。
 *	@date 09/05/15	Send の戻り値が成功しても不正値だった。修正。
 *	@date 10/03/17	GetString系メソッド追加。　
 *	@date 10/05/28	SetReceiveTimeout() を改良。
 *	@date 10/11/26	IsConnect() の改良。
 *	@date 11/10/18	SetWithTimeoutSendMode() を新規。
 *	@date 11/11/29	SetSendTimeout() を新規。 SetWithTimeoutSendMode() を廃止。
 *	@date 12/03/16	ハンドルクローズ処理を改善。
 *	@date 13/01/09	読み込みが占有されているデバイスでも制限付きでオープンできるようにした。
 */
class CUsbHid : public ICommunication, CUsbHidApi, CCopyImpossible
{
	DEFSUPER(ICommunication);
public:

	/// コンストラクタ 
	explicit CUsbHid(void) 
		: m_hUpPipe(INVALID_HANDLE_VALUE), m_hDownPipe(INVALID_HANDLE_VALUE), m_sendTimeout(0)
		, m_reportId(0), m_property(_T("HID"), TNBVIW_KIND_USB, 100)
	{
		ASSERT( m_property.receiveTimeout == 100 );
	}

	/// デストラクタ
	virtual ~CUsbHid(void)
	{
		Close();
	}

	/**
	 * [取得] 性能取得.
	 *	@note 正確な情報は Open後に取得できます。
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property;
	}

	/**
	 * [処理] オープン.
	 *	@note true が返っても、受信できない（ CanReceive() が false を返す）場合もあります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		Close();
		// デバイスのオープン
		DWORD x = (m_sendTimeout != 0) ? FILE_FLAG_OVERLAPPED : 0;
		HANDLE hDown = ::CreateFile(
						m_hidName.GetPathName(), GENERIC_WRITE,
						(FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, x, 0);
		if ( hDown == INVALID_HANDLE_VALUE )
		{
			_GetLastError("CreateFile");
			::SetLastError(ERROR_NOT_FOUND);
			return false;
		}
		HANDLE hUp = ::CreateFile (
						m_hidName.GetPathName(), (GENERIC_READ | GENERIC_WRITE),
						(FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		if ( hUp == INVALID_HANDLE_VALUE )
		{
			_GetLastError("CreateFile");
			// Read が出来なくても以下に続く。
		}
		m_hDownPipe = hDown;
		m_hUpPipe = hUp;
		//
		TRACE0( "CHid::Openしました\n" );
		m_property.typeName = CStr::Fmt(_T("HID-V%04X-P%04X"), m_hidName.GetVendorId(), m_hidName.GetProductId());	// タイプ名。
		m_property.typeId = TNBVIW_KIND_USB + (reinterpret_cast<ULONG_PTR>(this) & 0xFFFF);
		m_property.receivePacketSize = m_hidName.GetInputReportByteLength() - 1;
		m_property.sendPacketSize = m_hidName.GetOutputReportByteLength() - 1;
		m_reader.Attach(m_hUpPipe, m_property.receivePacketSize + 1);
		return true;
	}

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
		m_reader.Detach();
		//Portのクローズ
		if ( m_hUpPipe != INVALID_HANDLE_VALUE )
		{
			m_CloseHandle(m_hUpPipe);
			m_hUpPipe = INVALID_HANDLE_VALUE;
		}
		if ( m_hDownPipe != INVALID_HANDLE_VALUE )
		{
			m_CloseHandle(m_hDownPipe);
			m_hDownPipe = INVALID_HANDLE_VALUE;
		}
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	virtual bool IsOpened(void) const
	{
		return m_hDownPipe != INVALID_HANDLE_VALUE; 
	}

	/**
	 * [確認] 接続確認.
	 *	@retval true 接続中。
	 *	@retval false 切断中。
	 */
	virtual bool IsConnect(void) const
	{
		GetReceiveBufferCount();
		if ( ::GetLastError() == ERROR_DEVICE_NOT_CONNECTED )
		{
			return false;
		}
		return IsOpened();
	}

	/**
	 * [確認] 送信可能確認.
	 *	@retval true 送信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanSend(void) const
	{
		return (m_hDownPipe != INVALID_HANDLE_VALUE) && (m_hidName.GetOutputReportByteLength() > 0);
	}

	/**
	 * [確認] 受信可能確認.
	 *	@retval true 受信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanReceive(void) const
	{
		return (m_hUpPipe != INVALID_HANDLE_VALUE) && (m_hidName.GetInputReportByteLength() > 0);
	}

	/**
	 * [設定] レポートID設定.
	 *	@param id 送信レポートID、指定。
	 */
	void SetReportId(BYTE id)
	{
		m_reportId = id;
	}

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@return 常に true。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		m_property.receiveTimeout = to;
		return true;
	}

	/**
	 * [処理] 送信.
	 *	@note OutputReport長で割り切れないデータサイズを渡すと、足りないところは０でPaddingされます。
	 *	@param size サイズ。
	 *	@param P データ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 上記以外 送信データサイズ。
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( ! CanSend() )
		{
			return INVALID_SIZE;
		}
		size_t sizeOutputReport = m_hidName.GetOutputReportByteLength();
		size_t sentTotalSize = 0;		// 送信済みデータサイズ合計
		size_t sendSize = 0;			// 送信データサイズ
		CWorkMem work(sizeOutputReport);
		work[0] = m_reportId;		// REPORT_ID
		const BYTE* lpbData = static_cast<const BYTE*>(P);
		//
		bool isSuccess = true;
		//
		while ( true )
		{
			if ( (size - sentTotalSize) < sizeOutputReport )
			{
				sendSize = size - sentTotalSize;
				memset(work, 0, sizeOutputReport);
			}
			else
			{
				sendSize = sizeOutputReport -1;
			}
			// 送信データ 先頭は必ずREPORT_ID値とする（0x00 固定）
			MemCopy(&work[1], &lpbData[sentTotalSize], sendSize);
			// データ送信(NG時はリトライする(10回まで))
			bool r = false;
			loop ( i, 10 )
			{
				DWORD dwWriteLen = 0;
				if ( m_Write(m_hDownPipe, work, ToDword(sizeOutputReport), &dwWriteLen) )
				{
					if ( sizeOutputReport == dwWriteLen )
					{
						r = true; //成功
						break;
					}
				}
				if ( _GetLastError("WriteFile") == ERROR_DEVICE_NOT_CONNECTED )
				{
					Close();
					break;
				}
			}
			if ( ! r )
			{
				//送信失敗
				isSuccess = false;
				break;
			}
			sentTotalSize += sendSize;	// 送信済みサイズ
			if ( size <= sentTotalSize )
			{
				break;
			}
		}
		return isSuccess ? size : INVALID_SIZE;
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
		if ( ! CanReceive() )
		{
			return INVALID_SIZE;
		}
		size_t sizeInputReport = m_property.receivePacketSize; 
		CWorkMem work(sizeInputReport + 1);
		size_t r = m_reader.Read(work.Ref(), m_property.receiveTimeout);
		if ( r != INVALID_SIZE && r != 0 )
		{
			r--;
			if ( r > size )
			{
				r = size;
			}
			MemCopy(static_cast<BYTE*>(_P), work.Ref() + 1, r);
		}
		else
		{
			if ( ::GetLastError() == ERROR_DEVICE_NOT_CONNECTED )
			{
				Close();
			}
		}
		return r;
	}

	/**
	 * [確認] 受信データ確認.
	 *	@retval INVALID_SIZE 不明。
	 *	@retval 上記以外 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		return INVALID_SIZE;
	}

	/**
	 * [確認] チェック.
	 *		状態を確認します。
	 *	@retval CONNECTING	オープン＆接続状態
	 *	@retval CLOSED		クローズ状態	
	 */
	virtual EState Check(void) const
	{
		return IsConnect() ? CONNECTING : CLOSED;
	}

	/**
	 * [処理] パージ.
	 *		通信のゴミを浄化
	 */
	virtual void Purge(void) 
	{
		HidD_FlushQueue(m_hDownPipe);
	}

	/**
	 * [設定] USB-HIDデバイス指定.
	 *		使用するのデバイスを指定します。
	 *	@param hidName HID情報（使用するのはパスネームのみ）。 CUsbHidFinder を使い、得ることが出来ます。
	 *	@retval ERROR_SUCCESS 成功
	 *	@retval ERROR_SUCCESS以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD SetHidPathName(const CUsbHidPathName& hidName)
	{
		if ( hidName.IsEmpty() )
		{
			return ERROR_NOT_FOUND;
		}
		return SetHidPathName(hidName.GetPathName());
	}

	/**
	 * [設定] USB-HIDデバイス指定.
	 *		使用するのデバイスを指定します。
	 *	@param lpszPathName パスネーム。 CUsbHidFinder を使い、得ることが出来ます。
	 *	@retval ERROR_SUCCESS 成功
	 *	@retval ERROR_SUCCESS以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD SetHidPathName(LPCTSTR lpszPathName)
	{
		Close();
		return m_hidName.SetPathName(lpszPathName);
	}		

	/**
	 * [設定] タイムアウト付き送信モード.
	 *	@note オープンする前に使用してください。オープン後に使用するとクローズされます。
	 *	@note 0 以外を指定すると、送信の際、オーバーラップモードでオープンしたデバイスを使用します。
	 *		デフォルトは 0 です(通常にオープンしたデバイスを使用する)。
	 *	@param to 送信タイムアウト時間(ms)。
	 */
	void SetSendTimeout(DWORD to)
	{
		Close();
		m_sendTimeout = to;
	}

	/**
	 * [送信] SetOutputReport
	 *	@param dataSize 送信するデータ長。 GetOutputReportByteLength() の戻り値と同じ長さである必要があります。
	 *	@param lpcvData 送信するデータ。
	 *	@retval true	成功。
	 *	@retval	false	失敗。
	 */
	bool SetOutputReport(size_t dataSize, LPCVOID lpcvData)
	{
		if ( m_hidName.GetOutputReportByteLength() == dataSize )
		{
			return !! HidD_SetOutputReport(m_hDownPipe, const_cast<LPVOID>(lpcvData), ToDword(dataSize));
		}
		return false;
	}

	/**
	 * [受信] GetInputReport
	 *	@note 得られる情報長は、 GetInputReportByteLength() の戻り値と同じになります。
	 *	@retval Invalid状態 失敗。
	 *	@retval	それ以外 成功。内容は受信データ。
	 */
	CByteVector GetInputReport(void) const
	{
		CByteVector vb;
		size_t l = m_hidName.GetInputReportByteLength();
		if ( l > 0 )
		{
			BOOL r = !! HidD_GetInputReport(m_hUpPipe, vb.GetBuffer(l), ToDword(l));
			vb.ReleaseBuffer();
			if ( ! r )
			{
				vb.Invalid();
			}
		}
		return vb;
	}

	/**
	 * [送信] SetFeature
	 *	@param dataSize 送信するデータ長。 GetFeatureReportByteLength() の戻り値と同じ長さである必要があります。
	 *	@param lpcvData 送信するデータ。
	 *	@retval true	成功。
	 *	@retval	false	失敗。
	 */
	bool SetFeature(size_t dataSize, LPCVOID lpcvData)
	{
		if ( m_hidName.GetFeatureReportByteLength() == dataSize )
		{
			return !! HidD_SetFeature(m_hDownPipe, const_cast<LPVOID>(lpcvData), ToDword(dataSize));
		}
		return false;
	}

	/**
	 * [受信] GetFeature
	 *	@todo REPORT ID を指定できないとおかしい。
	 *	@note 得られる情報長は、 GetFeatureReportByteLength() の戻り値と同じになります。
	 *	@retval Invalid状態 失敗。
	 *	@retval	それ以外 成功。内容は受信データ。
	 */
	CByteVector GetFeature(void) const
	{
		CByteVector vb;
		size_t l = m_hidName.GetFeatureReportByteLength();
		if ( l > 0 )
		{
			BOOL r = !! HidD_GetFeature(m_hDownPipe, vb.GetBuffer(l), ToDword(l));
			vb.ReleaseBuffer();
			if ( ! r )
			{
				vb.Invalid();
			}
		}
		return vb;
	}

	/**
	 * [取得] パスネーム情報取得
	 *	@note Openしていない時の戻り値は不定です。
	 *	@return パスネーム情報。
	 */
	const CUsbHidPathName& GetName(void) const
	{
		return m_hidName;
	}

	/**
	 * [取得] Featureレポート長取得
	 *	@note Openしていない時の戻り値は不定です。
	 *	@return Featureレポート長。
	 */
	size_t GetFeatureReportByteLength(void) const
	{
		return m_hidName.GetFeatureReportByteLength();
	}

	/**
	 * [取得] Manufacturer 文字列取得
	 *	@retval Empty状態 失敗.
	 *	@retval 上記以外 成功。文字列は Manufacturer.
	 */
	CStr GetManufacturerString(void) const
	{
		CUnicode s;
		bool r = !! HidD_GetManufacturerString(m_hDownPipe, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [取得] Product 文字列取得
	 *	@retval Empty状態 失敗.
	 *	@retval 上記以外 成功。文字列は Product.
	 */
	CStr GetProductString(void) const
	{
		CUnicode s;
		bool r = !! HidD_GetProductString(m_hDownPipe, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [取得] SerialNumber 文字列取得
	 *	@retval Empty状態 失敗.
	 *	@retval 上記以外 成功。文字列は SerialNumber.
	 */
	CStr GetSerialNumberString(void) const
	{
		CUnicode s;
		bool r = !! HidD_GetSerialNumberString(m_hDownPipe, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [取得] INDEXED 文字列取得
	 *	@param[in] index インデックス
	 *	@retval Empty状態 失敗.
	 *	@retval 上記以外 成功。文字列は SerialNumber.
	 */
	CStr GetIndexedString(INDEX index) const
	{
		CUnicode s;
		bool r = !! HidD_GetIndexedString(m_hDownPipe, index, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [取得] 入力バッファ数取得.
	 *	@retval 0 失敗.
	 *	@retval 0以外 成功。値はバッファ数.
	 */
	DWORD GetReceiveBufferCount(void) const
	{
		ULONG numberBuffers;
		BOOLEAN r = HidD_GetNumInputBuffers(m_hDownPipe, &numberBuffers);
		return r ? numberBuffers : 0;
	}

	/**
	 * [設定] 入力バッファ数設定.
	 *	@note XP では最大 512 、 2000 では 最大 200 。最低は 2 。デフォルトは 32 です。
	 *	@param c 入力バッファ数.
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetReceiveBufferCount(DWORD c)
	{
		return !! HidD_SetNumInputBuffers(m_hDownPipe, c);
	}

private:
	bool m_CloseHandle(HANDLE h)
	{
		DWORD f;
		if ( ! ::GetHandleInformation(h, &f) )
		{
			_GetLastError("GetHandleInformation");
			return false;
		}
		return !! ::CloseHandle(h);
	}
	bool m_Write(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
	{
		if ( m_sendTimeout == 0 )
		{
			return !! ::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL);
		}
		OVERLAPPED overLapped;
		Zero(overLapped);
		HANDLE hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
		::ResetEvent(hEvent);
		overLapped.hEvent = hEvent;
		BOOL r = ::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, &overLapped);
		if ( r )
		{
			// 成功
		}
		else if ( ::GetLastError() == ERROR_IO_PENDING )
		{
			// 非同期読み取り操作の結果をチェックする。
			::Sleep(0);
			switch ( ::WaitForSingleObject(overLapped.hEvent, m_sendTimeout) )
			{
			case WAIT_OBJECT_0:
				r = ::GetOverlappedResult(hFile, &overLapped, lpNumberOfBytesWritten, FALSE);
				break;
			case WAIT_TIMEOUT:
			case WAIT_ABANDONED:
			case WAIT_FAILED:
			default:
				::CancelIo(hFile);
				r = FALSE;
				break;
			}
		}
		::CloseHandle(hEvent);
		return !! r;
	}

	HANDLE			m_hUpPipe;			///< 受信用ハンドル
	HANDLE			m_hDownPipe;		///< 送信用ハンドル
	CUsbHidPathName	m_hidName;			///< HIDネーム情報
	TProperty		m_property;			///< 性能
	BYTE			m_reportId;			///< 送信するレポートID。
	COverlapReader	m_reader;			///< 非同期読み込み。
	DWORD			m_sendTimeout;		///< 送信時のタイムアウト
	friend class CUsbHidTest;
};



}; // TNB
