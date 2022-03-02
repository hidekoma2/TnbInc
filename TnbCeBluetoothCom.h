#pragma once
/**
 *@file
 * CE 用 Bluetooth シリアルアクセス関係のヘッダ
 *
 *	@note Winsock2 を使うので、マクロ "_TnbWINSOCK2_ENABLE" を宣言しておきます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothCom.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbRs232c.h"
#include "TnbCeBluetooth.h"



//TNB Library
namespace TNB{



/**@ingroup COMMUNICATION CEBLUETOOTH
 * Bluetooth(CE) 仮想COMポート管理クラス.
 *
 *		Bluetoothの仮想COMポートを管理します。実際の通信は行いません。
 *	
 *	@see CBluetoothCom は仮想COMポート管理、および、通信までサポートしています。
 *
 *	@par必要ファイル
 *			TnbCeBluetoothCom.h
 * 
 *	@date 09/12/01	新規作成
 *	@date 10/08/16	クライアント専用の GetAddress() 廃止。 GetPeerAddress() 新規。
 */
class CBluetoothVirtualComPort : CCopyImpossible
{
public:

	/// コンストラクタ
	CBluetoothVirtualComPort(void)
		: m_hVirCom(NULL), m_spdHandle(0), m_btAddr(0), m_hServerCom(INVALID_HANDLE_VALUE)
	{
	}
	
	/// デストラクタ
	~CBluetoothVirtualComPort(void)
	{
		Delete();
	}

	/**
	 * [作成] クライアントポート作成.
	 *		クライアント側の仮想ポートを作成します。既にポートがあれば一度破棄します。
	 *	@note 一つのインスタンスで、サーバ側、クライアント側いずれかのポート一つだけしかオープンできません。
	 *			複数必要な場合、複数インスタンスを用意してください。
	 *	@param portNo COMポートナンバー。 数値でも文字列でも指定可能です。
	 *	@param btAddr サーバの Bluetoothアドレス。ペアリング接続しているものを指定します。
	 *	@param channel チャネル。省略すると自動で選択されます（最初に見つけたデバイスになる）。
	 *	@param service サービス。省略すると SerialPortServiceClass_UUID になります。
	 *	@retval true 成功.
	 *	@retval false 失敗
	 */
	bool CreateClientPort(CRs232c::CPort portNo, const BT_ADDR& btAddr, int channel = -1, GUID service = SerialPortServiceClass_UUID)
	{
		Delete();
		m_btAddr = btAddr;
		m_port = portNo;
		m_hVirCom = ms_Create(m_port, m_btAddr, false, 0, channel, service);
		return m_hVirCom != NULL;
	}

	/**
	 * [作成] サーバポート作成.
	 *		サーバ側の仮想ポートを作成します。既にポートがあれば一度破棄します。
	 *	@note 一つのインスタンスで、サーバ側、クライアント側いずれかのポート一つだけしかオープンできません。
	 *			複数必要な場合、複数インスタンスを用意してください。
	 *	@param portNo COMポートナンバー。 数値でも文字列でも指定可能です。
	 *	@param portFlag ポートフラグ。省略すると 「RFCOMM_PORT_FLAGS_AUTHENTICATE | RFCOMM_PORT_FLAGS_ENCRYPT」になります。
	 *	@retval true 成功.
	 *	@retval false 失敗
	 */
	bool CreateServerPort(CRs232c::CPort portNo, int portFlag = RFCOMM_PORT_FLAGS_AUTHENTICATE | RFCOMM_PORT_FLAGS_ENCRYPT)
	{
		Delete();
		m_btAddr = 0;
		m_port = portNo;
		m_hVirCom = ms_Create(portNo, m_btAddr, true, portFlag, 0);
		if ( m_hVirCom != NULL )
		{
			DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;	// アクセス (読み取りおよび書き込み) モード
			DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
			DWORD flag = FILE_ATTRIBUTE_NORMAL;
			m_hServerCom = ::CreateFile(portNo.GetPortName(), desiredAccess, shareMode, NULL, OPEN_EXISTING, flag, NULL);
			m_spdHandle = ms_RegisterSDP(m_hServerCom);
		}
		return m_hVirCom != NULL;
	}

	/**
	 * [確認] 有効確認.
	 *	@retval true ポート作成済み。
	 *	@retval false ポート未作成。
	 */
	bool IsValid(void) const
	{
		return m_hVirCom != NULL;
	}

	/**
	 * [取得] 作成ポート名取得.
	 *		作成したポート名を返します。
	 *	@return ポート名.
	 */
	const CRs232c::CPort& GetCreatedPort(void) const
	{
		return m_port;
	}

	/**
	 * [取得] 相手アドレス取得.
	 *		クライアントポートの場合、 CreateClientPort() で渡した、サーバのアドレスが返ります。
	 *		サーバの場合、接続されたデバイスのアドレスを返します。
	 *	@param[out] _btAddr アドレスが格納されます。
	 *	@retval true 成功。クライアントポートの場合、常に成功します。
	 *	@retval false 失敗。サーバポートに他のデバイスが接続していない場合、失敗します。
	 */
	bool GetPeerAddress(BT_ADDR& _btAddr) const
	{
		if ( m_btAddr != 0 )
		{
			_btAddr = m_btAddr;
			return true;
		}
		DWORD dwSizeOut = 0;
		BOOL r = ::DeviceIoControl(m_hServerCom, IOCTL_BLUETOOTH_GET_PEER_DEVICE, NULL, 0, &_btAddr, sizeof(_btAddr), &dwSizeOut, NULL);
		return !! r;
	}

	/**
	 * [破棄] ポート破棄.
	 *		CreateServerPort() , CreateClientPort() で作成したポートを破棄します。
	 *	@note ポートを作成していない場合、何もしません。
	 */
	void Delete(void)
	{
		if ( m_hServerCom != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hServerCom);
			m_hServerCom = INVALID_HANDLE_VALUE;
		}
		if ( m_spdHandle != 0 )
		{
			ms_DeRegisterSDP(m_spdHandle);
			m_spdHandle = 0;
		}
		if ( m_hVirCom != NULL )
		{
			::DeregisterDevice(m_hVirCom);
			m_hVirCom = NULL;
		}
	}

private:
	// 作成
	static HANDLE ms_Create(CRs232c::CPort portNo, const BT_ADDR& btAddr, bool isServer, int portFlag, int channel
		, GUID service = SerialPortServiceClass_UUID)
	{
		int port = portNo.GetNumber();
		CStr prefix = portNo.GetPrefix();
		//== COM ポートの使用状態チェック
		DEVMGR_DEVICE_INFORMATION ddi = { 0 };
		ddi.dwSize = sizeof(DEVMGR_DEVICE_INFORMATION);
		HANDLE hh = ::FindFirstDevice(DeviceSearchByDeviceName
								, CStr::Fmt(_T("%s%d"), prefix, port), &ddi);
		if ( hh != INVALID_HANDLE_VALUE )
		{
			TTRACE1("found hDevice = 0x%08X\n", ddi.hDevice);
			TTRACE1("     hParent = 0x%08X\n", ddi.hParentDevice);
			TTRACE1("     szDeviceName = %s\n", ddi.szDeviceName);
			TTRACE1("     szLegacyName = %s\n", ddi.szLegacyName);
			TTRACE1("     szBusName = %s\n", ddi.szBusName);
			TTRACE1("     szDeviceKey = %s\n", ddi.szDeviceKey);
			if ( ddi.szDeviceKey[0] == 0 )
			{
				if ( ! ::DeregisterDevice(ddi.hDevice) )
				{
					_GetLastError("DeregisterDevice");
				}
			}
			::FindClose(hh);
		}
		//== 割当
		PORTEMUPortParams pp = { 0 };
		pp.device = btAddr;
		pp.flocal = isServer;
		pp.uiportflags = portFlag;// | RFCOMM_PORT_FLAGS_REMOTE_DCB;
		if ( isServer )
		{
			pp.channel = RFCOMM_CHANNEL_MULTIPLE;
		}
		else
		{
			if ( channel >= 0 )
			{
				pp.channel = channel;
			}
			else
			{
				pp.channel = 0;
				pp.uuidService = service;
			}
		}
		DWORD info = reinterpret_cast<DWORD>(&pp);
		HANDLE h = ::RegisterDevice(prefix, port, _T("btd.dll"), info);
		if ( h == NULL )
		{
			_GetLastError("RegisterDevice");
		}
		return h;
	}

	// [登録] SDP登録.
	static ULONG ms_RegisterSDP(HANDLE handle)
	{
		CBluetoothApi api;
		const int SDP_RECORD_SIZE = 0x0000004d;
		const int SDP_CHANNEL_OFFSET = 26;
		static const BYTE rgbSdpRecordSPP[SDP_RECORD_SIZE] = 
		{
			0x35, 0x4b, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19,
			0x11, 0x01, 0x09, 0x00, 0x04, 0x35, 0x0c, 0x35,
			0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00,
			0x03, 0x08,
						0x01,								// server channel goes here (+26)
							  0x09, 0x00, 0x06, 0x35, 0x09,
			0x09, 0x65, 0x6e, 0x09, 0x00, 0x6a, 0x09, 0x01,
			0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06,
			0x19, 0x11, 0x01, 0x09, 0x01, 0x00, 0x09, 0x01,	// 0x1101 シリアル
			0x00, 0x25, 0x12, 'S',  'h',  'a',  'r',  'e',
			'd',  ' ',  'S',  'e',  'r',  'i',  'a',  'l',
			' ',  'P',  'o',  'r',  't'
		};
		ASSERT( rgbSdpRecordSPP[SDP_CHANNEL_OFFSET - 1] == 0x08 );
		ASSERT( rgbSdpRecordSPP[SDP_CHANNEL_OFFSET    ] == 0x01 );
		ASSERT( rgbSdpRecordSPP[SDP_CHANNEL_OFFSET + 1] == 0x09 );
		ULONG recordHandle = 0;
		DWORD port = 0;
		DWORD dwSizeOut = 0;
		if ( ! ::DeviceIoControl(handle, IOCTL_BLUETOOTH_GET_RFCOMM_CHANNEL, NULL, 0, &port, sizeof(port), &dwSizeOut, NULL)) 
		{
			TRACE1("BLUETOOTH GATEWAY:: Failed to retrieve port server channel, error = %d\n", GetLastError());
			return 0;
		}
		struct
		{
			BTHNS_SETBLOB b;
			unsigned char uca[SDP_RECORD_SIZE];
		} bigBlob;
		ULONG ulSdpVersion = BTH_SDP_VERSION;
		bigBlob.b.pRecordHandle = &recordHandle;
		bigBlob.b.pSdpVersion = &ulSdpVersion;
		bigBlob.b.fSecurity = 0;
		bigBlob.b.fOptions = 0;
		bigBlob.b.ulRecordLength = SDP_RECORD_SIZE;
		memcpy(bigBlob.b.pRecord, rgbSdpRecordSPP, SDP_RECORD_SIZE);
		bigBlob.b.pRecord[SDP_CHANNEL_OFFSET] = static_cast<BYTE>(port);
		BLOB blob;
		blob.cbSize = sizeof(BTHNS_SETBLOB) + SDP_RECORD_SIZE - 1;
		blob.pBlobData = reinterpret_cast<BYTE*>(&bigBlob);
		WSAQUERYSET service = { 0 };
		service.dwSize = sizeof(service);
		service.lpBlob = &blob;
		service.dwNameSpace = NS_BTH;
		int r = api.BthNsSetService(&service, RNRSERVICE_REGISTER, 0);
		if ( r != ERROR_SUCCESS )
		{
			TRACE1("BLUETOOTH GATEWAY:: BthNsSetService fails with status %d\n", r);
			return 0;
		}
		TRACE2("BLUETOOTH GATEWAY:: Created SDP record 0x%08x, channel %d\n", recordHandle, port);
		return recordHandle;
	}

	// [登録] SDP登録解除.
	static void ms_DeRegisterSDP(ULONG recordHandle)
	{
		CBluetoothApi api;
		ULONG ulSdpVersion = BTH_SDP_VERSION;
		BTHNS_SETBLOB delBlob = { 0 };
		delBlob.pRecordHandle = &recordHandle;
		delBlob.pSdpVersion = &ulSdpVersion;
		BLOB blob = { 0 };
		blob.cbSize = sizeof(BTHNS_SETBLOB);
		blob.pBlobData = (PBYTE) &delBlob;
		WSAQUERYSET service = { 0 };
		service.dwSize = sizeof(service);
		service.lpBlob = &blob;
		service.dwNameSpace = NS_BTH;
		int r = api.BthNsSetService (&service, RNRSERVICE_DELETE, 0);
		TRACE2("BLUETOOTH GATEWAY: removed SDP record 0x%08x (%d)\n", recordHandle, r);
	}

	HANDLE			m_hVirCom;		///< 仮想COMハンドル
	HANDLE			m_hServerCom;	///< サーバハンドル
	ULONG			m_spdHandle;	///< SDP ハンドル
	CRs232c::CPort	m_port;			///< ポート
	BT_ADDR			m_btAddr;		///< アドレス
};



/**@ingroup COMMUNICATION CEBLUETOOTH
 * Bluetooth(CE) クライアントシリアルアクセスクラス.
 *
 *	@note Open設定以外は、 CRs232c と同じです。
 *
 *	@note 相手が切断すると、ハンドルが無効になる可能性があります
 *			( Check() で CLOSED が返る)。
 *
 *	@par必要ファイル
 *			TnbCeBluetoothCom.h
 * 
 *	@date 09/10/01	新規作成
 *	@date 09/12/08	FindRfcommChannel() 追加。
 */
class CBluetoothCom : public CRs232c
{
	DEFSUPER(CRs232c);
public:

	/// コンストラクタ
	CBluetoothCom(void) : m_btAddr(0), m_isServer(false)
	{
	}

	/// デストラクタ
	~CBluetoothCom(void)
	{
		Close();
	}

	/**
	 * [設定] クライアント設定.
	 *		クライアントとしてオープンする設定をします。
	 *	@param btAddr サーバの Bluetoothアドレス。ペアリング接続しているものを指定します。
	 *	@param channel チャネル。省略すると自動で選択されます（最初に見つけたデバイスになる）。
	 */
	void SetClientParameter(const BT_ADDR& btAddr, int channel = -1)
	{
		Close();
		m_btAddr = btAddr;
		m_channel = channel;
		m_isServer = false;
	}

	/**
	 * [設定] サーバ設定.
	 *		サーバとしてオープンする設定をします。
	 *	@param portFlag フラグ。
	 */
	void SetServerParameter(int portFlag = RFCOMM_PORT_FLAGS_AUTHENTICATE | RFCOMM_PORT_FLAGS_ENCRYPT)
	{
		Close();
		m_btAddr = 0;
		m_channel = -1;
		m_portFlag = portFlag;
		m_isServer = true;
	}

	/**
	 * [取得] 相手アドレス取得.
	 *		クライアントとしてオープンした場合、サーバのアドレスが返ります。
	 *		サーバの場合、接続されたデバイスのアドレスを返します。
	 *	@param[out] _btAddr アドレスが格納されます。
	 *	@retval true 成功。クライアントの場合、常に成功します。
	 *	@retval false 失敗。サーバポートに他のデバイスが接続していない場合、失敗します。
	 */
	bool GetPeerAddress(BT_ADDR& _btAddr) const
	{
		if ( ! m_isServer )
		{
			_btAddr = m_btAddr;
			return true;
		}
		return m_virCom.GetPeerAddress(_btAddr);
	}

	/**
	 * [処理] オープン.
	 *		仮想COMポートを作成し、それをオープンします。
	 *	@note 先に SetClientParameter() か SetServerParameter() のいずれかと、
	 *			SetParameter() を使って、オープン設定をしておきます。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		CPort comPort;
		DWORD dwBaudRate;
		BYTE bByteSize;
		EParity eParity;
		EStopBits eStopBits;
		_super::GetParameter(comPort, dwBaudRate, bByteSize, eParity, eStopBits);
		_super::Close();
		bool r = false;
		if ( m_isServer )
		{
			r = m_virCom.CreateServerPort(comPort, m_portFlag);
		}
		else
		{
			r = m_virCom.CreateClientPort(comPort, m_btAddr, m_channel);
		}
		if ( r )
		{
			return _super::Open();
		}
		return false;
	}

	/**
	 * [処理] クローズ.
	 *		クローズし、仮想COMポートを破棄します。
	 *	@note オープンしていない時は何もしません。
	 */
	virtual void Close(void)
	{
		_super::Close();
		m_virCom.Delete();
	}

	/// Bluetooth(CE) クライアントシリアルアクセスのチャネル情報.
	struct TChannel
	{
		int		channel;	///< チャネル
		CStr	portName;	///< ポート名
	};

	/// チャネル情報リスト型
	typedef CVectorT<TChannel> CChannels;

	/**
	 * [取得] RFCOMMチャネル検索.
	 *		指定の bluetoothアドレスのシリアルポートのチャネルを取得します。
	 *	@note 本メソッドは SetClientParameter() の channel を得るために使用できます。
	 *	@param btAddr アドレス。
	 *	@param classId クラスID。省略すると SerialPortServiceClassID_UUID16 です。
	 *	@return チャネル情報リスト。
	 */
 	static CChannels FindChannels(const BT_ADDR& btAddr, WORD classId = SerialPortServiceClassID_UUID16)
	{
		CChannels vb;
		CBluetoothDevice ba(btAddr);
		CBluetoothServiceAttribute attr;
		if ( ba.HasService(attr, classId) && attr.IsValid() )
		{
			loop ( i, attr.GetRecordCount() )
			{
				if ( attr.SearchRecord(i) )
				{
					TChannel pp;
					pp.channel = -1;
					const CBluetoothServiceAttribute::TValuesList& list = attr.GetAttributeData(4/*protocol descriptor list*/);
					loop ( j, list.GetSize() )
					{
						if ( list[j].spec == SDP_ST_UUID16 && list[j].uuid16 == 3/**RFCOMM*/ )
						{
							if ( j + 1 < list.GetSize() && list[j + 1].spec == SDP_ST_UINT8 )
							{
								pp.channel = list[j + 1].uint8;
							}
							break;
						}
					}
					const CBluetoothServiceAttribute::TValuesList& l2 = attr.GetAttributeData(0x100);
					loop ( j, l2.GetSize() )
					{
						if ( l2[j].type == SDP_TYPE_STRING )
						{
							pp.portName = l2[j].str;
							break;
						}
					}
					if ( pp.channel >= 0 )
					{
						vb.Add(pp);
					}
				}
			}
		}
		return vb;
	}

private:
	CBluetoothVirtualComPort	m_virCom;	///< 仮想COM
	BT_ADDR						m_btAddr;	///< Bluetoothアドレス
	int							m_channel;	///< チャネル
	bool						m_isServer;	///< true ならサーバモード
	int							m_portFlag;	///< サーバ用ポートフラグ
};



};//TNB
