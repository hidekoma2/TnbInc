#pragma once
/**
 *@file
 * CE �p Bluetooth PAN �֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothCom.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCeBluetooth.h"
#include "TnbRegistryAccessor.h"
#include <pm.h>



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

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
 * Bluetooth(CE) PAN �N���X.
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetoothPan.h
 * 
 *	@date 10/06/22	�V�K�쐬
 *	@date 10/07/27	Deactivate �̏��������P�B�A�h���X�Ȃ��� Disconnect() ��p�~�B
 *	@date 11/04/22	Activate() �ŃA�N�e�B�u�Ȃ�Ȃ����A�A�_�v�^�̓d��ON�ɂ���悤�ɂ����B
 */
class CBluetoothPan : CCopyImpossible
{
public:

	/**
	 * [�ݒ�] �L��.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� ::GetLastError() �Ŏ擾�\�B
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
	 * [�ݒ�] ����.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� ::GetLastError() �Ŏ擾�\�B
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
	 * [�擾] �L���m�F.
	 *		���� PAN ���L�����A�`�F�b�N���܂��B
	 *	@retval true �L���ɂȂ��Ă���B
	 *	@retval false �����ɂȂ��Ă���B
	 */
	static bool IsActivated(void)
	{
		NIC_STATISTICS ns;
		return ms_GetState(ns);
	}

	/**
	 * [�擾] �ڑ��m�F.
	 *		���� PAN �ڑ�����Ă��邩�A�`�F�b�N���܂��B
	 *	@retval true �ڑ����B
	 *	@retval false ���ڑ� or ���s�B ::GetLastError() �� 0 �Ȃ疢�ڑ��B 0 �ȊO�Ȃ玸�s���Ă���B
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
	 * [�ݒ�] �ڑ�.
	 *	@param btAddr �ڑ��� Bluetooth �A�h���X�B
	 *	@param serviceId �ڑ�����T�[�r�X�B NAPServiceClass_UUID �Ȃǂ��w��B
	 *	@param isAuthenticate �F�؃��[�h.
	 *	@param isEncrypt �Í����[�h.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� ::GetLastError() �Ŏ擾�\�B
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
	 * [�ݒ�] �ڑ�.
	 *		�ڑ��O�� SDP �ŃT�[�r�X���m�F���A NAP, GN, PANU ������ΐڑ����܂��B
 	 *	@note �T�[�r�X�������R���Ă��邽�߁A{@link CBluetoothLocal::GetPageTimeoutMs() �y�[�W�^�C���A�E�g����}�̉e�����󂯂܂��B
	 *		�K�v�ɉ�����{@link CBluetoothLocal::SetPageTimeoutMs() �y�[�W�^�C���A�E�g���Ԃ̐ݒ�}���s���Ă��������B
	 *	@param btAddr �ڑ��� Bluetooth �A�h���X�B
	 *	@param isAuthenticate �F�؃��[�h.
	 *	@param isEncrypt �Í����[�h.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� ::GetLastError() �Ŏ擾�\
	 *				(�f�o�C�X���Ȃ��A PAN �֌W�̃T�[�r�X���Ȃ��ꍇ�A WSASERVICE_NOT_FOUND ���Ԃ�܂�)�B
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
	 * [�ݒ�] �ؒf.
	 *	@note �ڑ����Ă��Ȃ��Ă��������Ԃ�\��������܂��B��� IsConnected() ���g�p���Ă��������B
	 *	@param btAddr �ڑ��� Bluetooth �A�h���X�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� ::GetLastError() �Ŏ擾�\�B
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
	 * [�擾] ���W�X�g�� PAN �^�C�v�擾.
	 *		���݂̃��W�X�g����PAN �̃^�C�v���擾���܂��B
	 *	@retval NAPServiceClassID_UUID16 NAP�B
	 *	@retval PANUServiceClassID_UUID16 PANU�B
	 *	@retval GNServiceClassID_UUID16 GN�B
	 *	@retval ��L�ȊO �G���[�B���W�X�g�������Ă���\���L��B
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
	 * [�ݒ�] ���W�X�g�� PAN �^�C�v�ݒ�.
	 *		���W�X�g����PAN �̃^�C�v��ݒ肵�܂��B�ݒ��A���Z�b�g����K�v������܂��B
	 *	@param uuid �^�C�v�w��B NAPServiceClassID_UUID16 �� PANUServiceClassID_UUID16 ���w��o���܂��B
	 *	@param isPublish true �Ȃ� SDP ��PAN �^�C�v�����J���܂��B false �Ȃ����J�ɂ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�ݒ�] ���W�X�g�� TCPIP �ݒ�l�擾.
	 *		\\HKLM\\Comm\\BTPAN1\\Parms\\TCPIP �ȉ��̒l���擾�ł��܂��B
	 *	@param lpszName �ݒ�l���B "DhcpServer" , "DhcpIPAddress" �ȂǁA�w�肵�܂��B
	 *	@return �l�B
	 */
	static CStr QueryRegistTcpipValue(LPCTSTR lpszName)
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, L"Comm\\BTPAN1\\Parms", KEY_READ);
		CRegistryAccessor::CSection sec= reg[L"TCPIP"];
		return sec.QueryString(lpszName);
	}

	/**
	 * [�擾] �f�o�C�X���擾.
	 *	@return �f�o�C�X�� �hBTPAN1�h���Ԃ�܂��B 
	 */
	static LPCTSTR GetDeviceName(void)
	{
		return _T("BTPAN1");
	}

	/**
	 * [�ݒ�] �A�_�v�^�d���ݒ�.
	 *	@param lpszAdapterName �A�_�v�^���B
	 *	@param dx ��ԁB
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool SetAdapterPower(LPCTSTR lpszAdapterName, CEDEVICE_POWER_STATE dx)
	{
		// NDISPWR_DEVICE �ݒ�
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
		// DD_NDIS_DEVICE ����
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
	/// OID VALUE �w�b�_�^
	struct TOidValue
	{
		NDIS_OID		oid;			///< OID
		LPCTSTR			lpszDeviceName;	///< �f�o�C�X��
		//�R���X�g���N�^
		TOidValue(NDIS_OID o) : oid(o), lpszDeviceName(CBluetoothPan::GetDeviceName())
		{}
	};
	/// DWORD �� OID VALUE �^
	struct TOidDwordValue : TOidValue
	{
		DWORD			data;			///< �f�[�^
		//�R���X�g���N�^
		TOidDwordValue(NDIS_OID o, DWORD d) : TOidValue(o), data(d)
		{}
	};
	/// �A�h���X �� OID VALUE �^
	/// @note ��� btAddr ��ݒ��A guid ��ݒ肷�邱��
	#pragma pack(push, 1)
	struct TOidBtAddrValue : TOidValue
	{
		union {
			struct {
				BYTE		addr[6];	///< �A�h���X(BD_ADDR)
				GUID		guid;		///< �T�[�r�X
			};
			BT_ADDR			btAddr;		///< Bluetooth �A�h���X
		};
		//�R���X�g���N�^
		TOidBtAddrValue(NDIS_OID o, BT_ADDR a, const GUID& g) : TOidValue(o), btAddr(a)
		{
			guid = g;
		}
	};
	#pragma pack(pop)
	// �I�[�v��
	static HANDLE ms_OpenNdisUio(void)
	{
		DWORD fa = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
		HANDLE h = ::CreateFile(NDISUIO_DEVICE_NAME, 0, 0, NULL, OPEN_EXISTING, fa, INVALID_HANDLE_VALUE);
		return h;
	}
	// �ݒ�
	template<typename VAL>
	static DWORD ms_SetOidValue(HANDLE h, VAL& v)
	{
		DWORD w = 0;
		DWORD c = IOCTL_NDISUIO_SET_OID_VALUE;
		BOOL r = ::DeviceIoControl(h, c, &v, sizeof(VAL), NULL, 0, &w, NULL);
		return r ? ERROR_SUCCESS : _GetLastError("DeviceIoControl");
	}
	// ��Ԏ擾
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
	// ��Ԏ擾
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

