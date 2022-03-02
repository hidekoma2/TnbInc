#pragma once
/**
 *@file
 * USB HIDのアクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeUsbHid.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCommunication.h"
#include "TnbRegistryAccessor.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#ifndef CTL_CODE
 #define CTL_CODE( DeviceType, Function, Method, Access )                  \
    ( ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) )
#endif

/// USBHIDT コントロールコードベース値
#define IOCTL_USBHIDT_BASE 0xFC

#endif



/**@ingroup COMMUNICATION WINCE
 * USB-HIDアクセスクラス(CE専用)
 *
 *		専用のデバイスドライバ（USBHID_T.DLL）を使った HID デバイスの入出力をサポートします。
 *
 *	@note 専用のデバイスドライバのレジストリ登録を行うメソッドも用意しています。→ RegistDriver()
 * 
 *	@par必要ファイル
 *			TnbCeUsbHid.h
 * 
 *	@date 10/03/11	新規作成
 *	@date 11/06/15	ライブラリへ追加
 */
class CCeUsbHid : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);
public:

	/**
	 * デバイスチェック.
	 *		ドライババージョン、接続しているデバイスの情報をチェックします。
	 *	@note DeviceIoControl() の dwIoControlCode に CC を指定します。
	 *		その時、lpInBuf に本構造体のインスタンス、 
	 *		nInBufSize に本構造体の sizeof() を指定します。
	 */
	struct TIoCtrlBlock_CheckDevice
	{
		/// 定数
		enum 
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F00, 0, 1/*Read*/), ///< コントロールコード
			SIGNATURE = 'ThiD'
		};
		DWORD	signature;				///< [out] シグネチャ。SIGNATURE が格納されている。
		DWORD	version;				///< [out] バージョン情報が格納される。
		DWORD	outputReportLength;		///< [out] OUTPUTサイズが格納される。
		DWORD	inputReportLength;		///< [out] INPUTサイズが格納される。
	};

	/**
	 * 文字列取得.
	 *		デバイスの文字列を取得します。
	 *	@note DeviceIoControl() の dwIoControlCode に CC を指定します。
	 *		その時、lpInBuf に本構造体のインスタンス、 
	 *		nInBufSize に本構造体の sizeof() を指定します。
	 */
	struct TIoCtrlBlock_GetString
	{
		/// 定数
		enum
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F01, 0, 1/*Read*/) ///< コントロールコード
		};
		DWORD	type;					///< [in] 文字タイプ CUsbHid::EStringType
		DWORD	index;					///< [in]  Only used with stringType == HID_STRING_INDEXED
		LPWSTR	lpStringData;			///< [in,out] 取得文字データが格納されるバッファアドレスを指定する。
		DWORD	stringDataBufferSize;	///< [in] 文字データ長を指定する。
		DWORD	gotStringDataSize;		///< [out] 取得文字長が格納される。
	};

	/**
	 * 読み込み.
	 *		デバイスから読み込みます。
	 *	@note DeviceIoControl() の dwIoControlCode に CC を指定します。
	 *		その時、lpInBuf に本構造体のインスタンス、 
	 *		nInBufSize に本構造体の sizeof() を指定します。
	 */
	struct TIoCtrlBlock_Read
	{
		/// 定数
		enum
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F10, 0, 1/*Read*/) ///< コントロールコード
		};
		DWORD	mode;					///< [in] 0;interrupt
		DWORD	timeout;				///< [in] タイムアウト時間(ms)を指定する
		LPVOID	lpInputData;			///< [in,out] 受信データが格納されるバッファアドレスを指定する。
		DWORD	inputDataBufferSize;	///< [in] 受信可能データサイズを指定する。
		DWORD	inputedDataSize;		///< [out] 受信データサイズが格納される。
	};

	/**
	 * 書き込み.
	 *		デバイスへ書き込みます。
	 *	@note DeviceIoControl() の dwIoControlCode に CC を指定します。
	 *		その時、lpInBuf に本構造体のインスタンス、 
	 *		nInBufSize に本構造体の sizeof() を指定します。
	 */
	struct TIoCtrlBlock_Write
	{
		/// 定数
		enum
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F20, 0, 2/*Write*/) ///< コントロールコード
		};
		DWORD	mode;					///< [in] 0;interrupt
		DWORD	timeout;				///< [in] タイムアウト時間(ms)を指定する
		LPCVOID	lpcOutputData;			///< [in] 送信データのアドレスを指定する。
		DWORD	outputDataSize;			///< [in] 送信データサイズを指定する。
	};

	/** 文字タイプ
	 * @note GetString() の第一引数で使用します。
	 */
	enum EStringType
	{
		INDEXED = 0,	///< String Index.
		MANUFACTURER,	///< Manufacturer identifer.
		PRODUCT,		///< Product identifer.
		SERIALNUMBER,	///< Serial number.
	};


	//------------------------------------


	/// コンストラクタ 
	explicit CCeUsbHid(void) 
		: m_handle(INVALID_HANDLE_VALUE), m_property(_T("HID"), TNBVIW_KIND_USB), m_recvTimeout(100), m_deviceIndex(1)
	{
	}

	/// デストラクタ
	virtual ~CCeUsbHid(void)
	{
		Close();
	}

	/**
	 * [設定] デバイスインデックス設定.
	 *		デフォルトでは１です。通常、変更する必要はありません。
	 *	@note Open() の前に使用してください。
	 *	@param i デバイスインデックス
	 */
	void SetDeviceIndex(int i)
	{
		m_deviceIndex = i;
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
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		Close();
		// デバイスのオープン
		CStr s;
		s.Format(_T("HID%d:"), m_deviceIndex);
		HANDLE h = ::CreateFile(s, GENERIC_READ | GENERIC_WRITE,0, NULL, OPEN_EXISTING, 0, NULL);
		if ( h == INVALID_HANDLE_VALUE )
		{
			if ( ::GetLastError() != ERROR_DEV_NOT_EXIST )
			{
				_GetLastError("CreateFile");
			}
			return false;
		}
		BOOL r = ::DeviceIoControl(h, m_info.CC, &m_info, sizeof(m_info), NULL, 0, NULL, NULL);
		if ( r == 1 && m_info.signature == m_info.SIGNATURE )
		{
			TRACE1("CeUsbHid.CheckDevice, 結果 = %d\n", r);
			TRACE1(" バージョン情報 = 0x%X\n", m_info.version);
			TRACE1(" OUTPUTサイズ = 0x%X\n", m_info.outputReportLength);
			TRACE1(" INPUTサイズ = 0x%X\n", m_info.inputReportLength);
			//
			m_handle = h;
			m_property.typeName = _T("HID");
			m_property.typeId = TNBVIW_KIND_USB + (reinterpret_cast<ULONG_PTR>(this) & 0xFFFF);
			m_property.receivePacketSize = m_info.inputReportLength;
			m_property.sendPacketSize = m_info.outputReportLength;
			return true;
		}
		return false;
	}

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
		//Portのクローズ
		if ( m_handle != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	virtual bool IsOpened(void) const
	{
		return m_handle != INVALID_HANDLE_VALUE; 
	}

	/**
	 * [確認] 接続確認.
	 *	@retval true 接続中。
	 *	@retval false 切断中。
	 */
	virtual bool IsConnect(void) const
	{
		return IsOpened();
	}

	/**
	 * [確認] 送信可能確認.
	 *	@retval true 送信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanSend(void) const
	{
		return IsOpened() && (m_info.outputReportLength > 0);
	}

	/**
	 * [確認] 受信可能確認.
	 *	@retval true 受信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanReceive(void) const
	{
		return IsOpened() && (m_info.inputReportLength > 0);
	}

	/**
	 * [設定] 受信タイムアウト時間設定.
	 *	@note 未設定時は 100ms です。
	 *	@param to タイムアウト時間(ms)。
	 *	@return true 固定
	 */
	bool SetReceiveTimeout(DWORD to)
	{
		m_recvTimeout = to;
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
		TIoCtrlBlock_Write cb;
		cb.mode = 0;
		cb.timeout = 300;
		cb.outputDataSize = size;
		cb.lpcOutputData = P;
		BOOL r = ::DeviceIoControl(m_handle, cb.CC, &cb, sizeof(cb), NULL, 0, NULL, NULL);
		if ( ! r )
		{
			m_ConnectCheck();
		}
		return r ? size : INVALID_SIZE;
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
		TIoCtrlBlock_Read cb;
		cb.mode = 0;
		cb.timeout = m_recvTimeout;
		cb.inputDataBufferSize = size;
		cb.lpInputData = _P;
		BOOL r = ::DeviceIoControl(m_handle, cb.CC, &cb, sizeof(cb), NULL, 0, NULL, NULL);
		if ( ! r )
		{
			m_ConnectCheck();
		}
		return r ? cb.inputedDataSize : INVALID_SIZE;
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
	virtual void Purge(void) {}

	/**
	 * [取得] 文字列取得.
	 *	@param type 文字タイプ.
	 *	@param index 文字タイプが INDEXED の場合、指定必須。１以上。
	 *	@retval Empty エラー。
	 *	@retval 上記以外 成功。文字列は取得した文字列。
	 */
	CStr GetString(EStringType type, INDEX index = 0) const
	{
		const int LEN = 1024;
		CStr s;
		TIoCtrlBlock_GetString cb;
		cb.type = type;
		cb.index = index;
		cb.lpStringData = s.GetBuffer(LEN);
		cb.stringDataBufferSize = LEN;
		BOOL r = ::DeviceIoControl(m_handle, cb.CC, &cb, sizeof(cb), NULL, 0, NULL, NULL);
		s.ReleaseBuffer();
		if ( ! r )
		{
			s.Empty();
		}
		return s;
	}

	/**
	 * [取得] デバイス情報取得
	 *	@note Openしていない時の戻り値は不定です。
	 *	@return デバイス情報
	 */
	const TIoCtrlBlock_CheckDevice& GetDeviceInfo(void) const
	{
		return m_info;
	}

	/**
	 * [設定] ドライバーレジストリ登録.
	 *	@param vendorId ベンダーID
	 *	@param productId プロダクトID
	 *	@param usagePage USAGEページ
	 *	@param usageId USAGE ID
	 *	@param lpszDriverName ドライバーDLL名
	 *	@retval true 成功
	 *	@retval false 失敗.そのドライバーDLLが見つからない
	 */
	static bool RegistDriver(WORD vendorId = 0x20FC, WORD productId = 1, WORD usagePage = 0xFF00, WORD usageId = 1, LPCTSTR lpszDriverName = _T("USBHID_T.DLL"))
	{
		CStr s;
		//---
		s.Format(_T("\\Windows\\%s"), lpszDriverName);
		::SetLastError(0);
		DWORD a = ::GetFileAttributes(s);
		if ( _GetLastError("GetFileAttributes") == ERROR_FILE_NOT_FOUND )
		{
			return false;
		}
		//---
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, _T("Drivers"));
		//
		s.Format(_T("USB\\LoadClients\\%d_%d\\Default\\3\\Hid_Class"), vendorId, productId);
		CRegistryAccessor::CSection sec = reg[s];
		sec.WriteString(_T("DLL"), lpszDriverName);
		//
		s.Format(_T("USB\\ClientDrivers\\Hid_t\\Hid_Class"));
		sec = reg[s];
		sec.WriteString(_T("DLL"), lpszDriverName);
		sec.WriteString(_T("Prefix"), _T("HID"));
		sec.WriteDword(_T("QueuedTransferCount"), 2);
		//
		s.Format(_T("USB\\ClientDrivers\\Hid_t\\Instance"));
		sec = reg[s];
		sec.WriteString(_T("DLL"), lpszDriverName);
		//
		s.Format(_T("HID\\LoadClients\\Default\\Default\\%d_%d\\PSL"), usagePage, usageId);
		sec = reg[s];
		sec.WriteString(_T("DLL"), lpszDriverName);
		return true;
	}

private:
	void m_ConnectCheck(void)
	{
		switch ( ::GetLastError() )
		{
		case 0x649:
		case ERROR_NOT_SUPPORTED:
			Close();
			break;
		default:
			break;
		}
	}
	int							m_deviceIndex;	///< デバイス番号
	TIoCtrlBlock_CheckDevice	m_info;			///< デバイス状態
	HANDLE						m_handle;		///< ハンドル
	TProperty					m_property;		///< 性能
	DWORD						m_recvTimeout;	///< 受信時のタイムアウト
};



}; //TNB
