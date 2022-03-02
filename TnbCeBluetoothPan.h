#pragma once
/**
 *@file
 * CE 用 Bluetooth PAN 関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothCom.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCeBluetooth.h"
#include "TnbRegistryAccessor.h"
#include <pm.h>



#ifndef _TnbDOXYGEN	//Document作成用シンボル

//-- bt_ddi.h -----------------------------------------------------------
#define OID_PAN_CONNECT			0xffff0101
#define OID_PAN_DISCONNECT 		0xffff0102
#define OID_PAN_AUTHENTICATE	0xffff0103
#define OID_PAN_ENCRYPT			0xffff0104

//-- ntddndis.h ----------------------------------------------------
#ifdef UNDER_CE
#define DD_NDIS_DEVICE_NAME TEXT("NDS0:")
#else
#define DD_NDIS_DEVICE_NAME "\\Device\\UNKNOWN"
#endif
typedef ULONG NDIS_OID, *PNDIS_OID;
#define _NDIS_CONTROL_CODE(request,method) \
            CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, request, method, FILE_ANY_ACCESS)
#define WINCE_IOCTL_START 8
#define	IOCTL_NDIS_BIND_ADAPTER			_NDIS_CONTROL_CODE( WINCE_IOCTL_START+4, METHOD_OUT_DIRECT )

//-- nuiouser.h ----------------------------------------------------
#define	NDISUIO_DEVICE_NAME		TEXT("UIO1:")
#define FSCTL_NDISUIO_BASE		FILE_DEVICE_NETWORK
#define _NDISUIO_CTL_CODE(_Function, _Method, _Access)	\
			CTL_CODE(FSCTL_NDISUIO_BASE, _Function, _Method, _Access)
#define IOCTL_NDISUIO_SET_OID_VALUE		\
			_NDISUIO_CTL_CODE(0x205, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define	IOCTL_NDISUIO_NIC_STATISTICS		\
			_NDISUIO_CTL_CODE(0x209, METHOD_BUFFERED, FILE_ANY_ACCESS)
typedef struct {
  ULONG Size;
  LPCTSTR ptcDeviceName;	//PTCHAR
  ULONG DeviceState;
  ULONG MediaType;
  ULONG MediaState;
  ULONG PhysicalMediaType;
  ULONG LinkSpeed;
  ULONGLONG PacketsSent;
  ULONGLONG PacketsReceived;
  ULONG InitTime;
  ULONG ConnectTime;
  ULONGLONG BytesSent;
  ULONGLONG BytesReceived;
  ULONGLONG DirectedBytesReceived;
  ULONGLONG DirectedPacketsReceived;
  ULONG PacketsReceiveErrors;
  ULONG PacketsSendErrors;
  ULONG ResetCount;
  ULONG MediaSenseConnectCount;
  ULONG MediaSenseDisconnectCount;
} NIC_STATISTICS, *PNIC_STATISTICS;

//-- ndispwr.h ----------------------------------------------------
#define NDISPWR_DEVICE_NAME     TEXT("NPW1:")
#define FSCTL_NDISPWR_BASE      FILE_DEVICE_NETWORK
#define _NDISPWR_CTL_CODE(_Function, _Method, _Access)  \
            CTL_CODE(FSCTL_NDISPWR_BASE, _Function, _Method, _Access)
#define IOCTL_NPW_SAVE_POWER_STATE  \
            _NDISPWR_CTL_CODE(0x200, METHOD_BUFFERED, FILE_ANY_ACCESS)
typedef struct _NDISPWR_SAVEPOWERSTATE
{
    LPCWSTR                pwcAdapterName;
    CEDEVICE_POWER_STATE   CePowerState;
}   NDISPWR_SAVEPOWERSTATE, *PNDISPWR_SAVEPOWERSTATE;
#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) PAN クラス.
 *
 *	@par必要ファイル
 *			TnbCeBluetoothPan.h
 * 
 *	@date 10/06/22	新規作成
 *	@date 10/07/27	Deactivate の処理を改善。アドレスなしの Disconnect() を廃止。
 *	@date 11/04/22	Activate() でアクティブならない時、アダプタの電源ONにするようにした。
 */
class CBluetoothPan : CCopyImpossible
{
public:

	/**
	 * [設定] 有効.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は ::GetLastError() で取得可能。
	 */
	static bool Activate(void)
	{
		CBluetoothApi api;
		DWORD r = api.BthActivatePAN(true);
		if ( ! IsActivated() )
		{
			SetAdapterPower(GetDeviceName(), PwrDeviceUnspecified);
			r = api.BthActivatePAN(true);
		}
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [設定] 無効.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は ::GetLastError() で取得可能。
	 */
	static bool Deactivate(void)
	{
		CBluetoothApi api;
		DWORD r = api.BthActivatePAN(false);
		if ( r == ERROR_SUCCESS )
		{
			while ( true )
			{
				r = api.BthActivatePAN(false);
				if ( r != ERROR_SUCCESS )
				{
					if ( r == ERROR_INVALID_STATE )
					{
						r = ERROR_SUCCESS;
					}
					break;
				}
			}
		}
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [取得] 有効確認.
	 *		現在 PAN が有効か、チェックします。
	 *	@retval true 有効になっている。
	 *	@retval false 無効になっている。
	 */
	static bool IsActivated(void)
	{
		NIC_STATISTICS ns;
		return ms_GetState(ns);
	}

	/**
	 * [取得] 接続確認.
	 *		現在 PAN 接続されているか、チェックします。
	 *	@retval true 接続中。
	 *	@retval false 未接続 or 失敗。 ::GetLastError() が 0 なら未接続。 0 以外なら失敗している。
	 */
	static bool IsConnected(void)
	{
		NIC_STATISTICS ns;
		bool r = ms_GetState(ns);
		if ( r )
		{
			::SetLastError(0);
			return ns.MediaState == 0/*MEDIA_STATE_CONNECTED*/;
		}
		return false;
	}

	/**
	 * [設定] 接続.
	 *	@param btAddr 接続先 Bluetooth アドレス。
	 *	@param serviceId 接続するサービス。 NAPServiceClass_UUID などを指定。
	 *	@param isAuthenticate 認証モード.
	 *	@param isEncrypt 暗号モード.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は ::GetLastError() で取得可能。
	 */
	static bool Connect(const BT_ADDR& btAddr, const GUID& serviceId, bool isAuthenticate, bool isEncrypt)
	{
		DWORD r = ERROR_SUCCESS;
		HANDLE h = ms_OpenNdisUio();
		if ( h == INVALID_HANDLE_VALUE )
		{
			r = _GetLastError("CreateFile");
		}
		if ( r == ERROR_SUCCESS )
		{
			TOidDwordValue v(OID_PAN_AUTHENTICATE, isAuthenticate);
			r = ms_SetOidValue(h, v);
		}
		if ( r == ERROR_SUCCESS )
		{
			TOidDwordValue v(OID_PAN_ENCRYPT, isEncrypt);
			r = ms_SetOidValue(h, v);
		}
		if ( r == ERROR_SUCCESS )
		{
			TOidBtAddrValue v(OID_PAN_CONNECT, btAddr, serviceId);
			r = ms_SetOidValue(h, v);
		}
		if ( INVALID_HANDLE_VALUE != h )
		{
			::CloseHandle(h);
		}
		if ( r == ERROR_SUCCESS )
		{
			::Sleep(50);
			if ( ! IsConnected() )
			{
				r = ERROR_NOT_FOUND;
			}
		}
		::SetLastError(r);
		return (r == ERROR_SUCCESS);
	}

	/**
	 * [設定] 接続.
	 *		接続前に SDP でサービスを確認し、 NAP, GN, PANU があれば接続します。
 	 *	@note サービス検索を３つしているため、{@link CBluetoothLocal::GetPageTimeoutMs() ページタイムアウト時間}の影響を受けます。
	 *		必要に応じて{@link CBluetoothLocal::SetPageTimeoutMs() ページタイムアウト時間の設定}を行ってください。
	 *	@param btAddr 接続先 Bluetooth アドレス。
	 *	@param isAuthenticate 認証モード.
	 *	@param isEncrypt 暗号モード.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は ::GetLastError() で取得可能
	 *				(デバイスがない、 PAN 関係のサービスがない場合、 WSASERVICE_NOT_FOUND が返ります)。
	 */
	static bool Connect(const BT_ADDR& btAddr, bool isAuthenticate, bool isEncrypt)
	{
		CBluetoothDevice dev = btAddr;
		if ( dev.HasService(NAPServiceClassID_UUID16) )
		{
			return Connect(btAddr, NAPServiceClass_UUID, isAuthenticate, isEncrypt);
		}
		if ( dev.HasService(GNServiceClassID_UUID16) )
		{
			return Connect(btAddr, GNServiceClass_UUID, isAuthenticate, isEncrypt);
		}
		if ( dev.HasService(PANUServiceClassID_UUID16) )
		{
			return Connect(btAddr, PANUServiceClass_UUID, isAuthenticate, isEncrypt);
		}
		return false;
	}

	/**
	 * [設定] 切断.
	 *	@note 接続していなくても成功が返る可能性もあります。先に IsConnected() を使用してください。
	 *	@param btAddr 接続先 Bluetooth アドレス。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は ::GetLastError() で取得可能。
	 */
	static bool Disconnect(const BT_ADDR& btAddr)
	{
		DWORD r = ERROR_SUCCESS;
		HANDLE h = ms_OpenNdisUio();
		if ( h == INVALID_HANDLE_VALUE )
		{
			r = _GetLastError("CreateFile");
		}
		if ( r == ERROR_SUCCESS )
		{
			GUID g = { 0 };
			TOidBtAddrValue v(OID_PAN_DISCONNECT, btAddr, g);
			r = ms_SetOidValue(h, v);
		}
		if ( INVALID_HANDLE_VALUE != h )
		{
			::CloseHandle(h);
		}
		if ( r == ERROR_SUCCESS )
		{
			::Sleep(50);
			if ( IsConnected() )
			{
				r = ERROR_NOT_FOUND;
			}
		}
		::SetLastError(r);
		return (r == ERROR_SUCCESS);
	}

	/**
	 * [取得] レジストリ PAN タイプ取得.
	 *		現在のレジストリのPAN のタイプを取得します。
	 *	@retval NAPServiceClassID_UUID16 NAP。
	 *	@retval PANUServiceClassID_UUID16 PANU。
	 *	@retval GNServiceClassID_UUID16 GN。
	 *	@retval 上記以外 エラー。レジストリが壊れている可能性有り。
	 */
	static WORD GetRegistPanType(void)
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, _T("Comm\\BTPAN1\\Parms"), KEY_READ);
		CRegistryAccessor::CSection sec = reg[_T("")];
		CStr s = sec.QueryString(_T("AdapterType"));
		if ( s == _T("NAP") )
		{
			return NAPServiceClassID_UUID16;
		}
		else if ( s == _T("PANU") )
		{
			return PANUServiceClassID_UUID16;
		}
		else if ( s == _T("GN") )
		{
			return GNServiceClassID_UUID16;
		}
		return 0;
	}

	/**
	 * [設定] レジストリ PAN タイプ設定.
	 *		レジストリのPAN のタイプを設定します。設定後、リセットする必要があります。
	 *	@param uuid タイプ指定。 NAPServiceClassID_UUID16 か PANUServiceClassID_UUID16 が指定出来ます。
	 *	@param isPublish true なら SDP にPAN タイプを公開します。 false なら非公開にします。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool SetRegistPanType(WORD uuid, bool isPublish)
	{
		if ( uuid == NAPServiceClassID_UUID16 || uuid == PANUServiceClassID_UUID16 )
		{
			CStr s;
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, _T("Comm\\BTPAN1\\Parms"));
			CRegistryAccessor::CSection sec = reg[_T("")];
			bool isPanu = (uuid == PANUServiceClassID_UUID16);
			//
			GUID id = NAPServiceClass_UUID;
			CStr adapterType = _T("NAP");
			CStr friendlyName= _T("Network Access Point");
			if ( isPanu )
			{
				id = PANUServiceClass_UUID;
				adapterType = _T("PANU");
				friendlyName= _T("Personal Ad-hoc Network");
			}
			bool r = true;
			r &= sec.WriteString(_T("AdapterType"), adapterType);
			r &= sec.WriteString(_T("FriendlyName"), friendlyName);
			r &= sec.WriteString(_T("Description"), CStr::Fmt(_T("Bluetooth %s Service"), adapterType));
			s.Format(_T("{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}"),
									id.Data1, id.Data2, id.Data3,
									id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3], 
									id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7]); 
			r &= sec.WriteString(_T("ServiceId"), s);
			if ( sec.HasKey(_T("ProtocolsToBindTo")) )
			{
				CStrVector vs;
				vs.Add(_T("NOT"));
				if ( ! isPanu )
				{
					vs.Add(_T("NDISUIO"));
				}
				r &= sec.WritePluralStrings(_T("ProtocolsToBindTo"), vs);
			}
			r &= sec.WriteDword(_T("PublishSdpOnBoot"), isPublish);
			return r;
		}
		return false;
	}

	/**
	 * [設定] レジストリ TCPIP 設定値取得.
	 *		\\HKLM\\Comm\\BTPAN1\\Parms\\TCPIP 以下の値を取得できます。
	 *	@param lpszName 設定値名。 "DhcpServer" , "DhcpIPAddress" など、指定します。
	 *	@return 値。
	 */
	static CStr QueryRegistTcpipValue(LPCTSTR lpszName)
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, L"Comm\\BTPAN1\\Parms", KEY_READ);
		CRegistryAccessor::CSection sec= reg[L"TCPIP"];
		return sec.QueryString(lpszName);
	}

	/**
	 * [取得] デバイス名取得.
	 *	@return デバイス名 ”BTPAN1”が返ります。 
	 */
	static LPCTSTR GetDeviceName(void)
	{
		return _T("BTPAN1");
	}

	/**
	 * [設定] アダプタ電源設定.
	 *	@param lpszAdapterName アダプタ名。
	 *	@param dx 状態。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool SetAdapterPower(LPCTSTR lpszAdapterName, CEDEVICE_POWER_STATE dx)
	{
		// NDISPWR_DEVICE 設定
		{
			HANDLE h = ::CreateFile(NDISPWR_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										NULL, OPEN_EXISTING, 0, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				return false;
			}
			NDISPWR_SAVEPOWERSTATE savePowerState;
			savePowerState.pwcAdapterName = lpszAdapterName;
			savePowerState.CePowerState = dx;
			BOOL r = ::DeviceIoControl(h, IOCTL_NPW_SAVE_POWER_STATE, &savePowerState, sizeof(NDISPWR_SAVEPOWERSTATE), NULL, 0, NULL, NULL);
			DWORD e = _GetLastError("DeviceIoControl");
			::CloseHandle(h);
			if ( ! r )
			{
				::SetLastError(e);
				return false;
			}
		}
		// DD_NDIS_DEVICE 処理
		{
			CStr s = lpszAdapterName;
			size_t sl = (s.GetLength() + 1) * sizeof(TCHAR);
			HANDLE h = ::CreateFile(DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										NULL, OPEN_EXISTING, 0, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				return false;
			}
			BOOL r = ::DeviceIoControl(h, IOCTL_NDIS_BIND_ADAPTER, s.GetBuffer(), sl, NULL, 0, NULL, NULL);
			DWORD e = _GetLastError("DeviceIoControl");
			::CloseHandle(h);
			if ( ! r )
			{
				::SetLastError(e);
				return false;
			}
		}
		return true;
	}

private:
	/// OID VALUE ヘッダ型
	struct TOidValue
	{
		NDIS_OID		oid;			///< OID
		LPCTSTR			lpszDeviceName;	///< デバイス名
		//コンストラクタ
		TOidValue(NDIS_OID o) : oid(o), lpszDeviceName(CBluetoothPan::GetDeviceName())
		{}
	};
	/// DWORD の OID VALUE 型
	struct TOidDwordValue : TOidValue
	{
		DWORD			data;			///< データ
		//コンストラクタ
		TOidDwordValue(NDIS_OID o, DWORD d) : TOidValue(o), data(d)
		{}
	};
	/// アドレス の OID VALUE 型
	/// @note 先に btAddr を設定後、 guid を設定すること
	#pragma pack(push, 1)
	struct TOidBtAddrValue : TOidValue
	{
		union {
			struct {
				BYTE		addr[6];	///< アドレス(BD_ADDR)
				GUID		guid;		///< サービス
			};
			BT_ADDR			btAddr;		///< Bluetooth アドレス
		};
		//コンストラクタ
		TOidBtAddrValue(NDIS_OID o, BT_ADDR a, const GUID& g) : TOidValue(o), btAddr(a)
		{
			guid = g;
		}
	};
	#pragma pack(pop)
	// オープン
	static HANDLE ms_OpenNdisUio(void)
	{
		DWORD fa = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
		HANDLE h = ::CreateFile(NDISUIO_DEVICE_NAME, 0, 0, NULL, OPEN_EXISTING, fa, INVALID_HANDLE_VALUE);
		return h;
	}
	// 設定
	template<typename VAL>
	static DWORD ms_SetOidValue(HANDLE h, VAL& v)
	{
		DWORD w = 0;
		DWORD c = IOCTL_NDISUIO_SET_OID_VALUE;
		BOOL r = ::DeviceIoControl(h, c, &v, sizeof(VAL), NULL, 0, &w, NULL);
		return r ? ERROR_SUCCESS : _GetLastError("DeviceIoControl");
	}
	// 状態取得
	static DWORD ms_GetStatis(NIC_STATISTICS& _ns, HANDLE h)
	{
		DWORD w = 0;
		DWORD c = IOCTL_NDISUIO_NIC_STATISTICS;
		Zero(_ns);
		_ns.Size = sizeof(NIC_STATISTICS);
		_ns.ptcDeviceName = CBluetoothPan::GetDeviceName();
		BOOL r = ::DeviceIoControl(h, c, NULL, 0, &_ns, sizeof(NIC_STATISTICS), &w, NULL);
		return r ? ERROR_SUCCESS : _GetLastError("DeviceIoControl");
	}
	// 状態取得
	static bool ms_GetState(NIC_STATISTICS& _ns)
	{
		DWORD r = ERROR_SUCCESS;
		HANDLE h = ms_OpenNdisUio();
		if ( h == INVALID_HANDLE_VALUE )
		{
			r = _GetLastError("CreateFile");
		}
		if ( r == ERROR_SUCCESS )
		{
			r = ms_GetStatis(_ns, h);
		}
		if ( INVALID_HANDLE_VALUE != h )
		{
			::CloseHandle(h);
		}
		::SetLastError(r);
		return (r == ERROR_SUCCESS);
	}
};



};//TNB

