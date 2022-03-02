#pragma once
/**
 *@file
 * CE 用 Bluetooth関係のヘッダ
 *
 *	@note Winsock2 を使うので、マクロ "_TnbWINSOCK2_ENABLE" を宣言しておきます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothApi.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbDef.h"
#include <winsock2.h>
#include <bthapi.h>
#include <bt_api.h>
#include <bt_sdp.h>
#pragma comment(lib,"bthguid.lib")
#pragma comment(lib,"Ws2.lib")



//TNB Library
namespace TNB{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

//#include "osInc/Btagpub.h"
#define IOCTL_AG_OPEN_AUDIO         0x01
#define IOCTL_AG_CLOSE_AUDIO        0x02
#define IOCTL_AG_CLOSE_CONTROL      0x03
#define IOCTL_AG_SET_SPEAKER_VOL    0x04
#define IOCTL_AG_SET_MIC_VOL        0x05
#define IOCTL_AG_GET_SPEAKER_VOL    0x06
#define IOCTL_AG_GET_MIC_VOL        0x07
#define IOCTL_AG_GET_POWER_MODE     0x08
#define IOCTL_AG_SET_POWER_MODE     0x09
#define IOCTL_AG_OPEN_CONTROL       0x0A
#define IOCTL_AG_SET_USE_HF_AUDIO   0x0B
#define IOCTL_AG_SET_INBAND_RING    0x0C

//#include "osInc/Bthid.h"
#define BTHHID_IOCTL_HIDConnect					1
#define BTHHID_IOCTL_HIDVerify					2
#define BTHHID_IOCTL_HIDDisconnect				3
#define BTHHID_IOCTL_HidSendControl				4
#define BTHHID_IOCTL_HidSendInterrupt			5



/// CE Bluetooth モジュール[btdrt.dll] 管理
/// BTD0 の管理
class CBluetoothApi : CCopyImpossible
{
public:
	/// コンストラクタ
	CBluetoothApi(void)
	{
		static CModule s_module;
		m_module = s_module;
	}

	//This function activates or deactivates Bluetooth Personal Area Networking (PAN).
	// Windows Embedded CE 6.0 and later
	int BthActivatePAN(BOOL fActivate)
	{
		int (*P)(BOOL fActivate) = NULL;
		P = Func(P, _T("BthActivatePAN"));
		return (P == NULL) ? ERROR_FUNCTION_FAILED : P(fActivate);
	}

	//This function requests that the existing connection with the peer device be authenticated.
	int BthAuthenticate(BT_ADDR* pba) const
	{
		int (*P)(BT_ADDR* pba) = NULL;
		return Func(P, _T("BthAuthenticate"))(pba);
	}

	//This function responds to a request for the personal identification number (PIN) for a Bluetooth device. 
	int BthAnswerPairRequest(BT_ADDR* pba, int cPinLength, unsigned char *ppin)
	{
		int (*P)(BT_ADDR* pba, int cPinLength, unsigned char *ppin) = NULL;
		return Func(P, _T("BthAnswerPairRequest"))(pba, cPinLength, ppin);
	}

	// This function cancels the current inquiry operation.
	int BthCancelInquiry(void)
	{
		int (*P)(void) = NULL;
		return Func(P, _T("BthCancelInquiry"))();
	}

	// This function clears the Inquiry Filter.
	int BthClearInquiryFilter(void)
	{
		int (*P)(void) = NULL;
		return Func(P, _T("BthClearInquiryFilter"))();
	}

	// This function closes the specified baseband connection.
	int BthCloseConnection(unsigned short handle)
	{
		int (*P)(unsigned short handle) = NULL;
		return Func(P, _T("BthCloseConnection"))(handle);
	}

	// This function creates an ACL connection to a device with the specified address.
	int BthCreateACLConnection(BT_ADDR* pbt, unsigned short* phandle)
	{
		int (*P)(BT_ADDR* pbt, unsigned short* phandle) = NULL;
		return Func(P, _T("BthCreateACLConnection"))(pbt, phandle);
	}

	// This function creates an SCO connection to a device with the specified address.
	int BthCreateSCOConnection(BT_ADDR* pbt, unsigned short* phandle)
	{
		int (*P)(BT_ADDR* pbt, unsigned short* phandle) = NULL;
		return Func(P, _T("BthCreateSCOConnection"))(pbt, phandle);
	}

	// This function places the ACL connection to the peer in HOLD mode.
	int BthEnterHoldMode(BT_ADDR* pba, unsigned short hold_mode_max, unsigned short hold_mode_min, unsigned short* pinterval)
	{
		int (*P)(BT_ADDR* pba, unsigned short hold_mode_max, unsigned short hold_mode_min, unsigned short* pinterval) = NULL;
		return Func(P, _T("BthEnterHoldMode"))(pba, hold_mode_max, hold_mode_min, pinterval);
	}

	//This function places the ACL connection to the peer specified by the Bluetooth address in PARK mode.
	int BthEnterParkMode(BT_ADDR* pba, unsigned short beacon_max, unsigned short beacon_min, unsigned short* pinterval)
	{
		int (*P)(BT_ADDR* pba, unsigned short beacon_max, unsigned short beacon_min, unsigned short* pinterval) = NULL;
		return Func(P, _T("BthEnterParkMode"))(pba, beacon_max, beacon_min, pinterval);
	}

	// This function places the ACL connection to the peer in SNIFF mode.
	int BthEnterSniffMode(BT_ADDR* pba, unsigned short sniff_mode_max, unsigned short sniff_mode_min, unsigned short sniff_attempt, unsigned short sniff_timeout, unsigned short* pinterval)
	{
		int (*P)(BT_ADDR* pba, unsigned short sniff_mode_max, unsigned short sniff_mode_min, unsigned short sniff_attempt, unsigned short sniff_timeout, unsigned short* pinterval) = NULL;
		return Func(P, _T("BthEnterSniffMode"))(pba, sniff_mode_max, sniff_mode_min, sniff_attempt, sniff_timeout, pinterval);
	}

	//This function forces the ACL connection to the peer to leave PARK mode.
	int BthExitParkMode(BT_ADDR* pba)
	{
		int (*P)(BT_ADDR* pba) = NULL;
		return Func(P, _T("BthExitParkMode"))(pba);
	}

	//This function forces the ACL connection to the peer to leave SNIFF mode.
	int BthExitSniffMode(BT_ADDR* pba)
	{
		int (*P)(BT_ADDR* pba) = NULL;
		return Func(P, _T("BthExitSniffMode"))(pba);
	}

	//This function retrieves the Bluetooth address based on the specified connection handle.
	int BthGetAddress(unsigned short handle, BT_ADDR* pba) const
	{
		int (*P)(unsigned short handle, BT_ADDR* pba) = NULL;
		return Func(P, _T("BthGetAddress"))(handle, pba);
	}

	//This function retrieves baseband connection data for all current connections.
	int BthGetBasebandConnections(int cConnections, BASEBAND_CONNECTION* pConnections, int* pcConnectionsReturned)
	{
		int (*P)(int cConnections, BASEBAND_CONNECTION* pConnections, int* pcConnectionsReturned) = NULL;
		return Func(P, _T("BthGetBasebandConnections"))(cConnections, pConnections, pcConnectionsReturned);
	};

	//This function retrieves the current mode of the ACL connection to the peer that is specified by the Bluetooth address. This function is equivalent to BTH_HCI_IOCTL_GET_HANDLE_MODE HCI IOCTL.
	int BthGetCurrentMode(BT_ADDR* pba, unsigned char* pmode) const
	{
		int (*P)(BT_ADDR* pba, unsigned char* pmode) = NULL;
		return Func(P, _T("BthGetCurrentMode"))(pba, pmode);
	}

	// This function obtains the hardware status.
	int BthGetHardwareStatus(int* pistatus) const
	{
		int (*P)(int* pistatus) = NULL;
		return Func(P, _T("BthGetHardwareStatus"))(pistatus);
	}

	//This function retrieves the link key for the Bluetooth device.
	int BthGetLinkKey(BT_ADDR* pba, unsigned char key[16]) const
	{
		int (*P)(BT_ADDR* pba, unsigned char key[16]) = NULL;
		return Func(P, _T("BthGetLinkKey"))(pba, key);
	}

	// This function retrieves the address of the Bluetooth peer device authentication that requires the PIN code. 
	int BthGetPINRequest(BT_ADDR* pbt) const
	{
		int (*P)(BT_ADDR* pbt) = NULL;
		return Func(P, _T("BthGetPINRequest"))(pbt);
	}

	// This functions retrieves the class of device (COD ) for a specified device that has a baseband connection with the local device.
	int BthGetRemoteCOD(BT_ADDR* pbt, unsigned int* pcod) const
	{
		int (*P)(BT_ADDR* pbt, unsigned int* pcod) = NULL;
		return Func(P, _T("BthGetRemoteCOD"))(pbt, pcod);
	}

	//This function retrieves the current role for a peer Bluetooth device.
	// Windows Embedded CE 6.0 and later
	int BthGetRole(BT_ADDR* pbt, USHORT* pusRole)
	{
		int (*P)(BT_ADDR* pbt, USHORT* pusRole) = NULL;
		P = Func(P, _T("BthGetRole"));
		return (P == NULL) ? ERROR_FUNCTION_FAILED : P(pbt, pusRole);
	}

	//This function initiates a client query that is constrained by the information contained within a WSAQUERYSET structure. This function returns a handle that is used by the BthNsLookupServiceNext function. 
	int BthNsLookupServiceBegin(LPWSAQUERYSET pQuerySet, DWORD dwFlags, LPHANDLE lphLookup) const
	{
		int (*P)(LPWSAQUERYSET pQuerySet, DWORD dwFlags, LPHANDLE lphLookup) = NULL;
		return Func(P, _T("BthNsLookupServiceBegin"))(pQuerySet, dwFlags, lphLookup);
	}

	//This function frees the handle after previous calls to the BthNsLookupServiceBegin and BthNsLookupServiceNext functions.
	int BthNsLookupServiceEnd(HANDLE hLookup) const
	{
		int (*P)(HANDLE hLookup) = NULL;
		return Func(P, _T("BthNsLookupServiceEnd"))(hLookup);
	}

	//This function retrieves the results of an SDP search. 
	int BthNsLookupServiceNext(HANDLE hLookup, DWORD dwFlags, LPDWORD lpdwBufferLength, LPWSAQUERYSET pResults) const
	{
		int (*P)(HANDLE hLookup, DWORD dwFlags, LPDWORD lpdwBufferLength, LPWSAQUERYSET pResults) = NULL;
		return Func(P, _T("BthNsLookupServiceNext"))(hLookup, dwFlags, lpdwBufferLength, pResults);
	}

	//This function adds, updates, or removes service records from the local SDP database.
	int BthNsSetService(LPWSAQUERYSET pSet, WSAESETSERVICEOP op, DWORD dwFlags)
	{
		int (*P)(LPWSAQUERYSET pSet, WSAESETSERVICEOP op, DWORD dwFlags) = NULL;
		return Func(P, _T("BthNsSetService"))(pSet, op, dwFlags);
	}

	//This function requests the personal identification number (PIN) for a Bluetooth device. 
	int BthPairRequest(BT_ADDR* pba, int cPinLength, unsigned char *ppin)
	{
		int (*P)(BT_ADDR* pba, int cPinLength, unsigned char *ppin) = NULL;
		return Func(P, _T("BthPairRequest"))(pba, cPinLength, ppin);
	}

	//This function performs an Inquiry operation.
	int BthPerformInquiry(unsigned int LAP, unsigned char length, unsigned char num_responses, unsigned int cBuffer, unsigned int* pcDiscoveredDevices, BthInquiryResult* InquiryList) const
	{
		int (*P)(unsigned int LAP, unsigned char length, unsigned char num_responses, unsigned int cBuffer, unsigned int* pcDiscoveredDevices, BthInquiryResult* InquiryList) = NULL;
		return Func(P, _T("BthPerformInquiry"))(LAP, length, num_responses, cBuffer, pcDiscoveredDevices, InquiryList);
	}

	//This function reads the value for the authentication setting.
	int BthReadAuthenticationEnable(unsigned char* pae) const
	{
		int (*P)(unsigned char* pae) = NULL;
		return Func(P, _T("BthReadAuthenticationEnable"))(pae);
	}

	// This function reads adapter's class of device.
	int BthReadCOD(unsigned int* pcod) const
	{
		int (*P)(unsigned int* pcod) = NULL;
		return Func(P, _T("BthReadCOD"))(pcod);
	}

	//This function retrieves the Bluetooth address of the current device.
	int BthReadLocalAddr(BT_ADDR* pba) const
	{
		int (*P)(BT_ADDR* pba) = NULL;
		return Func(P, _T("BthReadLocalAddr"))(pba);
	}

	//This function retrieves the current page timeout value.
	int BthReadPageTimeout(unsigned short* ptimeout) const
	{
		int (*P)(unsigned short* ptimeout) = NULL;
		return Func(P, _T("BthReadPageTimeout"))(ptimeout);
	}

	//This function reads the version and capabilities of Link Manager Protocol (LMP) stack on the Bluetooth device. It is a combination of the following two HCI commands: Read_Remote_Version_Information and Read_Remote_Supported_Features.
	int BthReadRemoteVersion(BT_ADDR* pba, unsigned char* plmp_version, unsigned short* plmp_subversion, unsigned short* pmanufacturer, unsigned char* plmp_features) const
	{
		int (*P)(BT_ADDR* pba, unsigned char* plmp_version, unsigned short* plmp_subversion, unsigned short* pmanufacturer, unsigned char* plmp_features) = NULL;
		return Func(P, _T("BthReadRemoteVersion"))(pba, plmp_version, plmp_subversion, pmanufacturer, plmp_features);
	}

	//This function reads the adapter's scan mode.
	int BthReadScanEnableMask(unsigned char* pmask) const
	{
		int (*P)(unsigned char* pmask) = NULL;
		return Func(P, _T("BthReadScanEnableMask"))(pmask);
	}

	//This function refuses an outstanding PIN request that is retrieved by BthGetPINRequest function.
	int BthRefusePINRequest(BT_ADDR* pbt)
	{
		int (*P)(BT_ADDR* pbt) = NULL;
		return Func(P, _T("BthRefusePINRequest"))(pbt);
	}

	// This function queries the name of a remote Bluetooth device.
	int BthRemoteNameQuery(BT_ADDR* pba, unsigned int cBuffer, unsigned int* pcRequired, WCHAR* szString) const
	{
		int (*P)(BT_ADDR* pba, unsigned int cBuffer, unsigned int* pcRequired, WCHAR* szString) = NULL;
		return Func(P, _T("BthRemoteNameQuery"))(pba, cBuffer, pcRequired, szString);
	}

	//This function removes the link key for a Bluetooth device.
	int BthRevokeLinkKey(BT_ADDR* pba)
	{
		int (*P)(BT_ADDR* pba) = NULL;
		return Func(P, _T("BthRevokeLinkKey"))(pba);
	}

	//This function revokes the personal identification number (PIN) for the Bluetooth device.
	int BthRevokePIN(BT_ADDR* pba)
	{
		int (*P)(BT_ADDR* pba) = NULL;
		return Func(P, _T("BthRevokePIN"))(pba);
	}

	//This function sets the Inquiry Filter to a specified class of device (COD). 
	int BthSetCODInquiryFilter (unsigned int cod, unsigned int codMask)
	{
		int (*P)(unsigned int cod, unsigned int codMask) = NULL;
		return Func(P, _T("BthSetCODInquiryFilter"))(cod, codMask);
	}

	//This function requests that the encryption mode on an existing connection is switched with the peer device.
	int BthSetEncryption(BT_ADDR* pba, int fOn)
	{
		int (*P)(BT_ADDR* pba, int fOn) = NULL;
		return Func(P, _T("BthSetEncryption"))(pba, fOn);
	}

	//This function sets the Inquiry Filter to a specified address.
	int BthSetInquiryFilter(BT_ADDR* pba)
	{
		int (*P)(BT_ADDR* pba) = NULL;
		return Func(P, _T("BthSetInquiryFilter"))(pba);
	}

	//This function stores the link key for the Bluetooth device.
	int BthSetLinkKey(BT_ADDR* pba, unsigned char key[16])
	{
		int (*P) (BT_ADDR* pba, unsigned char key[16]) = NULL;
		return Func(P, _T("BthSetLinkKey"))(pba, key);
	}

	//This function stores the personal identification number (PIN) for the Bluetooth device.
	int BthSetPIN(BT_ADDR* pba, int cPinLength, unsigned char* ppin)
	{
		int (*P)(BT_ADDR* pba, int cPinLength, unsigned char* ppin) = NULL;
		return Func(P, _T("BthSetPIN"))(pba, cPinLength, ppin);
	}

	//This function allows an application to provide an event that will be signaled by the stack when a device without a PIN sends a PIN request.
	int BthSetSecurityUI(HANDLE hEvent, DWORD dwStoreTimeout, DWORD dwProcTimeout)
	{
		int (*P)(HANDLE hEvent, DWORD dwStoreTimeout, DWORD dwProcTimeout) = NULL;
		return Func(P, _T("BthSetSecurityUI"))(hEvent, dwStoreTimeout, dwProcTimeout);
	}

	//This function changes the current role to the specified role.
	// Windows Embedded CE 6.0 and later
	int BthSwitchRole(BT_ADDR* pbt, USHORT usRole)
	{
		int (*P)(BT_ADDR* pbt, USHORT usRole) = NULL;
		P = Func(P, _T("BthSwitchRole"));
		return (P == NULL) ? ERROR_FUNCTION_FAILED : P(pbt, usRole);
	}

	//This function disconnects all idle connections held by the L2CAP layer.
	int BthTerminateIdleConnections(void)
	{
		int (*P)(void) = NULL;
		return Func(P, _T("BthTerminateIdleConnections"))();
	}

	//This function sets the value for the authentication setting.
	int BthWriteAuthenticationEnable(unsigned char ae)
	{
		int (*P)(unsigned char ae) = NULL;
		return Func(P, _T("BthWriteAuthenticationEnable"))(ae);
	}

	// This function sets adapter's class of device.
	int BthWriteCOD(unsigned int cod)
	{
		int (*P)(unsigned int cod) = NULL;
		return Func(P, _T("BthWriteCOD"))(cod);
	}

	// This function reads the current link policy for the existing connection with the peer specified by its Bluetooth address.
	int BthReadLinkPolicySettings(BT_ADDR* pba, unsigned short* plps) const
	{
		int (*P)(BT_ADDR* pba, unsigned short* plps) = 0;
		return Func(P, _T("BthReadLinkPolicySettings"))(pba, plps);
	}

	// This function writes the current link policy for the existing connection with the peer specified by its Bluetooth address.
	int BthWriteLinkPolicySettings(BT_ADDR* pba,  unsigned short lps)
	{
		int (*P)(BT_ADDR* pba,  unsigned short lps) = 0;
		return Func(P, _T("BthWriteLinkPolicySettings"))(pba, lps);
	}

	// This function configures the page timeout.
	int BthWritePageTimeout(unsigned short timeout)
	{
		int (*P)(unsigned short timeout) = NULL;
		return Func(P, _T("BthWritePageTimeout"))(timeout);
	}

	//This function sets the adapter's scan mode.
	int BthWriteScanEnableMask(unsigned char mask)
	{
		int (*P)(unsigned char mask) = NULL;
		return Func(P, _T("BthWriteScanEnableMask"))(mask);
	}

	//Enumerate all the possible modes of operation of the bluetooth radio
	//#include <bthutil.h>
	enum BTH_RADIO_MODE
	{
		BTH_POWER_OFF,		///< Turn off the Bluetooth radio. 
		BTH_CONNECTABLE,	///< Turn on the Bluetooth radio, and make it connectable. 
		BTH_DISCOVERABLE	///< Turn on the Bluetooth radio, and make it both connectable and discoverable. 
	};

	//Use the BthSetMode function to set the Bluetooth mode of operation and reflect it in the control panel.
	// This function is also used to persist that state across hardware insertion and reboot.
	int BthSetMode(DWORD mode)
	{
		int ret=ERROR_SUCCESS;
		switch ( mode )
		{
			case BTH_POWER_OFF:
				ms_BtdStartStop(false);
				break;
			case BTH_CONNECTABLE:
				ms_BtdStartStop(true);
				BthWriteScanEnableMask(2);
				break;
			case BTH_DISCOVERABLE:
				ms_BtdStartStop(true);
				BthWriteScanEnableMask(3);
				break;
			default:
				break;
		}
		return ERROR_SUCCESS;
	}

	// Use the BthGetMode function to retrieve the current mode of operation of the Bluetooth radio.
	int BthGetMode(DWORD* pMode) const
	{
		unsigned char mask;
		if ( BthReadScanEnableMask(&mask) != ERROR_SUCCESS )
		{
			*pMode = BTH_POWER_OFF;
		}
		else
		{
			if ( (mask & 0x0001) != 0 )
			{
				*pMode = BTH_DISCOVERABLE;
			}
			else
			{
				*pMode = BTH_CONNECTABLE;
			}
		}
		return ERROR_SUCCESS;
	}

protected:

	// EXPORT関数コール
	template<typename TYP>
	TYP* Func(TYP* typ, LPCTSTR lpsz) const
	{
		ForceSet(typ, ::GetProcAddress(m_module, lpsz));
		ASSERT1( typ != NULL, "CBluetoothApi::Func", "[%s]が見つかwareりませんでした", lpsz );
		return typ;
	}

	// 戻り値管理
	bool m_Ret(int r) const
	{
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

private:
	// BTD0 コントロール
	static bool ms_BtdStartStop(bool isEnable)
	{
		HANDLE hDev = ::CreateFile (L"BTD0:", GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL, OPEN_EXISTING, 0, NULL);
		if ( hDev == INVALID_HANDLE_VALUE )
		{
			return false;
		}
		const DWORD IOCTL_SERVICE_START = CTL_CODE(FILE_DEVICE_SERVICE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS);
		const DWORD IOCTL_SERVICE_STOP  = CTL_CODE(FILE_DEVICE_SERVICE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS);
		DWORD ioctl = (isEnable) ? IOCTL_SERVICE_START : IOCTL_SERVICE_STOP;
		DWORD outSize = 0;
		DWORD outBuffer = 0;
		WCHAR* pArgPtr = L"card";
		::DeviceIoControl (hDev, ioctl, pArgPtr, sizeof(WCHAR) * (4 + 1), NULL, outSize, &outBuffer, NULL);
		::CloseHandle(hDev);
		return true;
	}

	/// モジュール管理
	class CModule
	{
	public:
		/// コンストラクタ
		CModule(void) : m_module(::LoadLibrary(_T("btdrt.dll")))
		{
			ASSERT( m_module != NULL );
		}
		/// デストラクタ
		~CModule(void)
		{
			if ( m_module != NULL )
			{
				::FreeLibrary(m_module);
				m_module = NULL;
			}
		}
		/// 取得
		operator HMODULE(void) const
		{
			return m_module;
		}
	private:
		HMODULE		m_module;	///< btdrt.dll のハンドル
	};
	HMODULE		m_module;	///< btdrt.dll のハンドル
};

#endif // _TnbDOXYGEN	//Document作成用シンボル



};//TNB
