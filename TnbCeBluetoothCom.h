#pragma once
/**
 *@file
 * CE �p Bluetooth �V���A���A�N�Z�X�֌W�̃w�b�_
 *
 *	@note Winsock2 ���g���̂ŁA�}�N�� "_TnbWINSOCK2_ENABLE" ��錾���Ă����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeBluetoothCom.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbRs232c.h"
#include "TnbCeBluetooth.h"



//TNB Library
namespace TNB{



/**@ingroup COMMUNICATION CEBLUETOOTH
 * Bluetooth(CE) ���zCOM�|�[�g�Ǘ��N���X.
 *
 *		Bluetooth�̉��zCOM�|�[�g���Ǘ����܂��B���ۂ̒ʐM�͍s���܂���B
 *	
 *	@see CBluetoothCom �͉��zCOM�|�[�g�Ǘ��A����сA�ʐM�܂ŃT�|�[�g���Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetoothCom.h
 * 
 *	@date 09/12/01	�V�K�쐬
 *	@date 10/08/16	�N���C�A���g��p�� GetAddress() �p�~�B GetPeerAddress() �V�K�B
 */
class CBluetoothVirtualComPort : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CBluetoothVirtualComPort(void)
		: m_hVirCom(NULL), m_spdHandle(0), m_btAddr(0), m_hServerCom(INVALID_HANDLE_VALUE)
	{
	}
	
	/// �f�X�g���N�^
	~CBluetoothVirtualComPort(void)
	{
		Delete();
	}

	/**
	 * [�쐬] �N���C�A���g�|�[�g�쐬.
	 *		�N���C�A���g���̉��z�|�[�g���쐬���܂��B���Ƀ|�[�g������Έ�x�j�����܂��B
	 *	@note ��̃C���X�^���X�ŁA�T�[�o���A�N���C�A���g�������ꂩ�̃|�[�g����������I�[�v���ł��܂���B
	 *			�����K�v�ȏꍇ�A�����C���X�^���X��p�ӂ��Ă��������B
	 *	@param portNo COM�|�[�g�i���o�[�B ���l�ł�������ł��w��\�ł��B
	 *	@param btAddr �T�[�o�� Bluetooth�A�h���X�B�y�A�����O�ڑ����Ă�����̂��w�肵�܂��B
	 *	@param channel �`���l���B�ȗ�����Ǝ����őI������܂��i�ŏ��Ɍ������f�o�C�X�ɂȂ�j�B
	 *	@param service �T�[�r�X�B�ȗ������ SerialPortServiceClass_UUID �ɂȂ�܂��B
	 *	@retval true ����.
	 *	@retval false ���s
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
	 * [�쐬] �T�[�o�|�[�g�쐬.
	 *		�T�[�o���̉��z�|�[�g���쐬���܂��B���Ƀ|�[�g������Έ�x�j�����܂��B
	 *	@note ��̃C���X�^���X�ŁA�T�[�o���A�N���C�A���g�������ꂩ�̃|�[�g����������I�[�v���ł��܂���B
	 *			�����K�v�ȏꍇ�A�����C���X�^���X��p�ӂ��Ă��������B
	 *	@param portNo COM�|�[�g�i���o�[�B ���l�ł�������ł��w��\�ł��B
	 *	@param portFlag �|�[�g�t���O�B�ȗ������ �uRFCOMM_PORT_FLAGS_AUTHENTICATE | RFCOMM_PORT_FLAGS_ENCRYPT�v�ɂȂ�܂��B
	 *	@retval true ����.
	 *	@retval false ���s
	 */
	bool CreateServerPort(CRs232c::CPort portNo, int portFlag = RFCOMM_PORT_FLAGS_AUTHENTICATE | RFCOMM_PORT_FLAGS_ENCRYPT)
	{
		Delete();
		m_btAddr = 0;
		m_port = portNo;
		m_hVirCom = ms_Create(portNo, m_btAddr, true, portFlag, 0);
		if ( m_hVirCom != NULL )
		{
			DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;	// �A�N�Z�X (�ǂݎ�肨��я�������) ���[�h
			DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
			DWORD flag = FILE_ATTRIBUTE_NORMAL;
			m_hServerCom = ::CreateFile(portNo.GetPortName(), desiredAccess, shareMode, NULL, OPEN_EXISTING, flag, NULL);
			m_spdHandle = ms_RegisterSDP(m_hServerCom);
		}
		return m_hVirCom != NULL;
	}

	/**
	 * [�m�F] �L���m�F.
	 *	@retval true �|�[�g�쐬�ς݁B
	 *	@retval false �|�[�g���쐬�B
	 */
	bool IsValid(void) const
	{
		return m_hVirCom != NULL;
	}

	/**
	 * [�擾] �쐬�|�[�g���擾.
	 *		�쐬�����|�[�g����Ԃ��܂��B
	 *	@return �|�[�g��.
	 */
	const CRs232c::CPort& GetCreatedPort(void) const
	{
		return m_port;
	}

	/**
	 * [�擾] ����A�h���X�擾.
	 *		�N���C�A���g�|�[�g�̏ꍇ�A CreateClientPort() �œn�����A�T�[�o�̃A�h���X���Ԃ�܂��B
	 *		�T�[�o�̏ꍇ�A�ڑ����ꂽ�f�o�C�X�̃A�h���X��Ԃ��܂��B
	 *	@param[out] _btAddr �A�h���X���i�[����܂��B
	 *	@retval true �����B�N���C�A���g�|�[�g�̏ꍇ�A��ɐ������܂��B
	 *	@retval false ���s�B�T�[�o�|�[�g�ɑ��̃f�o�C�X���ڑ����Ă��Ȃ��ꍇ�A���s���܂��B
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
	 * [�j��] �|�[�g�j��.
	 *		CreateServerPort() , CreateClientPort() �ō쐬�����|�[�g��j�����܂��B
	 *	@note �|�[�g���쐬���Ă��Ȃ��ꍇ�A�������܂���B
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
	// �쐬
	static HANDLE ms_Create(CRs232c::CPort portNo, const BT_ADDR& btAddr, bool isServer, int portFlag, int channel
		, GUID service = SerialPortServiceClass_UUID)
	{
		int port = portNo.GetNumber();
		CStr prefix = portNo.GetPrefix();
		//== COM �|�[�g�̎g�p��ԃ`�F�b�N
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
		//== ����
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

	// [�o�^] SDP�o�^.
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
			0x19, 0x11, 0x01, 0x09, 0x01, 0x00, 0x09, 0x01,	// 0x1101 �V���A��
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

	// [�o�^] SDP�o�^����.
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

	HANDLE			m_hVirCom;		///< ���zCOM�n���h��
	HANDLE			m_hServerCom;	///< �T�[�o�n���h��
	ULONG			m_spdHandle;	///< SDP �n���h��
	CRs232c::CPort	m_port;			///< �|�[�g
	BT_ADDR			m_btAddr;		///< �A�h���X
};



/**@ingroup COMMUNICATION CEBLUETOOTH
 * Bluetooth(CE) �N���C�A���g�V���A���A�N�Z�X�N���X.
 *
 *	@note Open�ݒ�ȊO�́A CRs232c �Ɠ����ł��B
 *
 *	@note ���肪�ؒf����ƁA�n���h���������ɂȂ�\��������܂�
 *			( Check() �� CLOSED ���Ԃ�)�B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetoothCom.h
 * 
 *	@date 09/10/01	�V�K�쐬
 *	@date 09/12/08	FindRfcommChannel() �ǉ��B
 */
class CBluetoothCom : public CRs232c
{
	DEFSUPER(CRs232c);
public:

	/// �R���X�g���N�^
	CBluetoothCom(void) : m_btAddr(0), m_isServer(false)
	{
	}

	/// �f�X�g���N�^
	~CBluetoothCom(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �N���C�A���g�ݒ�.
	 *		�N���C�A���g�Ƃ��ăI�[�v������ݒ�����܂��B
	 *	@param btAddr �T�[�o�� Bluetooth�A�h���X�B�y�A�����O�ڑ����Ă�����̂��w�肵�܂��B
	 *	@param channel �`���l���B�ȗ�����Ǝ����őI������܂��i�ŏ��Ɍ������f�o�C�X�ɂȂ�j�B
	 */
	void SetClientParameter(const BT_ADDR& btAddr, int channel = -1)
	{
		Close();
		m_btAddr = btAddr;
		m_channel = channel;
		m_isServer = false;
	}

	/**
	 * [�ݒ�] �T�[�o�ݒ�.
	 *		�T�[�o�Ƃ��ăI�[�v������ݒ�����܂��B
	 *	@param portFlag �t���O�B
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
	 * [�擾] ����A�h���X�擾.
	 *		�N���C�A���g�Ƃ��ăI�[�v�������ꍇ�A�T�[�o�̃A�h���X���Ԃ�܂��B
	 *		�T�[�o�̏ꍇ�A�ڑ����ꂽ�f�o�C�X�̃A�h���X��Ԃ��܂��B
	 *	@param[out] _btAddr �A�h���X���i�[����܂��B
	 *	@retval true �����B�N���C�A���g�̏ꍇ�A��ɐ������܂��B
	 *	@retval false ���s�B�T�[�o�|�[�g�ɑ��̃f�o�C�X���ڑ����Ă��Ȃ��ꍇ�A���s���܂��B
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
	 * [����] �I�[�v��.
	 *		���zCOM�|�[�g���쐬���A������I�[�v�����܂��B
	 *	@note ��� SetClientParameter() �� SetServerParameter() �̂����ꂩ�ƁA
	 *			SetParameter() ���g���āA�I�[�v���ݒ�����Ă����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �N���[�Y.
	 *		�N���[�Y���A���zCOM�|�[�g��j�����܂��B
	 *	@note �I�[�v�����Ă��Ȃ����͉������܂���B
	 */
	virtual void Close(void)
	{
		_super::Close();
		m_virCom.Delete();
	}

	/// Bluetooth(CE) �N���C�A���g�V���A���A�N�Z�X�̃`���l�����.
	struct TChannel
	{
		int		channel;	///< �`���l��
		CStr	portName;	///< �|�[�g��
	};

	/// �`���l����񃊃X�g�^
	typedef CVectorT<TChannel> CChannels;

	/**
	 * [�擾] RFCOMM�`���l������.
	 *		�w��� bluetooth�A�h���X�̃V���A���|�[�g�̃`���l�����擾���܂��B
	 *	@note �{���\�b�h�� SetClientParameter() �� channel �𓾂邽�߂Ɏg�p�ł��܂��B
	 *	@param btAddr �A�h���X�B
	 *	@param classId �N���XID�B�ȗ������ SerialPortServiceClassID_UUID16 �ł��B
	 *	@return �`���l����񃊃X�g�B
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
	CBluetoothVirtualComPort	m_virCom;	///< ���zCOM
	BT_ADDR						m_btAddr;	///< Bluetooth�A�h���X
	int							m_channel;	///< �`���l��
	bool						m_isServer;	///< true �Ȃ�T�[�o���[�h
	int							m_portFlag;	///< �T�[�o�p�|�[�g�t���O
};



};//TNB
