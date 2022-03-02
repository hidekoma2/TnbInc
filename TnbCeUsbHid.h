#pragma once
/**
 *@file
 * USB HID�̃A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeUsbHid.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbCommunication.h"
#include "TnbRegistryAccessor.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#ifndef CTL_CODE
 #define CTL_CODE( DeviceType, Function, Method, Access )                  \
    ( ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) )
#endif

/// USBHIDT �R���g���[���R�[�h�x�[�X�l
#define IOCTL_USBHIDT_BASE 0xFC

#endif



/**@ingroup COMMUNICATION WINCE
 * USB-HID�A�N�Z�X�N���X(CE��p)
 *
 *		��p�̃f�o�C�X�h���C�o�iUSBHID_T.DLL�j���g���� HID �f�o�C�X�̓��o�͂��T�|�[�g���܂��B
 *
 *	@note ��p�̃f�o�C�X�h���C�o�̃��W�X�g���o�^���s�����\�b�h���p�ӂ��Ă��܂��B�� RegistDriver()
 * 
 *	@par�K�v�t�@�C��
 *			TnbCeUsbHid.h
 * 
 *	@date 10/03/11	�V�K�쐬
 *	@date 11/06/15	���C�u�����֒ǉ�
 */
class CCeUsbHid : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);
public:

	/**
	 * �f�o�C�X�`�F�b�N.
	 *		�h���C�o�o�[�W�����A�ڑ����Ă���f�o�C�X�̏����`�F�b�N���܂��B
	 *	@note DeviceIoControl() �� dwIoControlCode �� CC ���w�肵�܂��B
	 *		���̎��AlpInBuf �ɖ{�\���̂̃C���X�^���X�A 
	 *		nInBufSize �ɖ{�\���̂� sizeof() ���w�肵�܂��B
	 */
	struct TIoCtrlBlock_CheckDevice
	{
		/// �萔
		enum 
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F00, 0, 1/*Read*/), ///< �R���g���[���R�[�h
			SIGNATURE = 'ThiD'
		};
		DWORD	signature;				///< [out] �V�O�l�`���BSIGNATURE ���i�[����Ă���B
		DWORD	version;				///< [out] �o�[�W������񂪊i�[�����B
		DWORD	outputReportLength;		///< [out] OUTPUT�T�C�Y���i�[�����B
		DWORD	inputReportLength;		///< [out] INPUT�T�C�Y���i�[�����B
	};

	/**
	 * ������擾.
	 *		�f�o�C�X�̕�������擾���܂��B
	 *	@note DeviceIoControl() �� dwIoControlCode �� CC ���w�肵�܂��B
	 *		���̎��AlpInBuf �ɖ{�\���̂̃C���X�^���X�A 
	 *		nInBufSize �ɖ{�\���̂� sizeof() ���w�肵�܂��B
	 */
	struct TIoCtrlBlock_GetString
	{
		/// �萔
		enum
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F01, 0, 1/*Read*/) ///< �R���g���[���R�[�h
		};
		DWORD	type;					///< [in] �����^�C�v CUsbHid::EStringType
		DWORD	index;					///< [in]  Only used with stringType == HID_STRING_INDEXED
		LPWSTR	lpStringData;			///< [in,out] �擾�����f�[�^���i�[�����o�b�t�@�A�h���X���w�肷��B
		DWORD	stringDataBufferSize;	///< [in] �����f�[�^�����w�肷��B
		DWORD	gotStringDataSize;		///< [out] �擾���������i�[�����B
	};

	/**
	 * �ǂݍ���.
	 *		�f�o�C�X����ǂݍ��݂܂��B
	 *	@note DeviceIoControl() �� dwIoControlCode �� CC ���w�肵�܂��B
	 *		���̎��AlpInBuf �ɖ{�\���̂̃C���X�^���X�A 
	 *		nInBufSize �ɖ{�\���̂� sizeof() ���w�肵�܂��B
	 */
	struct TIoCtrlBlock_Read
	{
		/// �萔
		enum
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F10, 0, 1/*Read*/) ///< �R���g���[���R�[�h
		};
		DWORD	mode;					///< [in] 0;interrupt
		DWORD	timeout;				///< [in] �^�C���A�E�g����(ms)���w�肷��
		LPVOID	lpInputData;			///< [in,out] ��M�f�[�^���i�[�����o�b�t�@�A�h���X���w�肷��B
		DWORD	inputDataBufferSize;	///< [in] ��M�\�f�[�^�T�C�Y���w�肷��B
		DWORD	inputedDataSize;		///< [out] ��M�f�[�^�T�C�Y���i�[�����B
	};

	/**
	 * ��������.
	 *		�f�o�C�X�֏������݂܂��B
	 *	@note DeviceIoControl() �� dwIoControlCode �� CC ���w�肵�܂��B
	 *		���̎��AlpInBuf �ɖ{�\���̂̃C���X�^���X�A 
	 *		nInBufSize �ɖ{�\���̂� sizeof() ���w�肵�܂��B
	 */
	struct TIoCtrlBlock_Write
	{
		/// �萔
		enum
		{
			CC = CTL_CODE(IOCTL_USBHIDT_BASE, 0x0F20, 0, 2/*Write*/) ///< �R���g���[���R�[�h
		};
		DWORD	mode;					///< [in] 0;interrupt
		DWORD	timeout;				///< [in] �^�C���A�E�g����(ms)���w�肷��
		LPCVOID	lpcOutputData;			///< [in] ���M�f�[�^�̃A�h���X���w�肷��B
		DWORD	outputDataSize;			///< [in] ���M�f�[�^�T�C�Y���w�肷��B
	};

	/** �����^�C�v
	 * @note GetString() �̑������Ŏg�p���܂��B
	 */
	enum EStringType
	{
		INDEXED = 0,	///< String Index.
		MANUFACTURER,	///< Manufacturer identifer.
		PRODUCT,		///< Product identifer.
		SERIALNUMBER,	///< Serial number.
	};


	//------------------------------------


	/// �R���X�g���N�^ 
	explicit CCeUsbHid(void) 
		: m_handle(INVALID_HANDLE_VALUE), m_property(_T("HID"), TNBVIW_KIND_USB), m_recvTimeout(100), m_deviceIndex(1)
	{
	}

	/// �f�X�g���N�^
	virtual ~CCeUsbHid(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �f�o�C�X�C���f�b�N�X�ݒ�.
	 *		�f�t�H���g�ł͂P�ł��B�ʏ�A�ύX����K�v�͂���܂���B
	 *	@note Open() �̑O�Ɏg�p���Ă��������B
	 *	@param i �f�o�C�X�C���f�b�N�X
	 */
	void SetDeviceIndex(int i)
	{
		m_deviceIndex = i;
	}

	/**
	 * [�擾] ���\�擾.
	 *	@note ���m�ȏ��� Open��Ɏ擾�ł��܂��B
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property;
	}

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void)
	{
		Close();
		// �f�o�C�X�̃I�[�v��
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
			TRACE1("CeUsbHid.CheckDevice, ���� = %d\n", r);
			TRACE1(" �o�[�W������� = 0x%X\n", m_info.version);
			TRACE1(" OUTPUT�T�C�Y = 0x%X\n", m_info.outputReportLength);
			TRACE1(" INPUT�T�C�Y = 0x%X\n", m_info.inputReportLength);
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
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
		//Port�̃N���[�Y
		if ( m_handle != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	virtual bool IsOpened(void) const
	{
		return m_handle != INVALID_HANDLE_VALUE; 
	}

	/**
	 * [�m�F] �ڑ��m�F.
	 *	@retval true �ڑ����B
	 *	@retval false �ؒf���B
	 */
	virtual bool IsConnect(void) const
	{
		return IsOpened();
	}

	/**
	 * [�m�F] ���M�\�m�F.
	 *	@retval true ���M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanSend(void) const
	{
		return IsOpened() && (m_info.outputReportLength > 0);
	}

	/**
	 * [�m�F] ��M�\�m�F.
	 *	@retval true ��M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanReceive(void) const
	{
		return IsOpened() && (m_info.inputReportLength > 0);
	}

	/**
	 * [�ݒ�] ��M�^�C���A�E�g���Ԑݒ�.
	 *	@note ���ݒ莞�� 100ms �ł��B
	 *	@param to �^�C���A�E�g����(ms)�B
	 *	@return true �Œ�
	 */
	bool SetReceiveTimeout(DWORD to)
	{
		m_recvTimeout = to;
		return true;
	}

	/**
	 * [����] ���M.
	 *	@note OutputReport���Ŋ���؂�Ȃ��f�[�^�T�C�Y��n���ƁA����Ȃ��Ƃ���͂O��Padding����܂��B
	 *	@param size �T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval ��L�ȊO ���M�f�[�^�T�C�Y�B
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
	 * [����] ��M.
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval 0 ��M�f�[�^�Ȃ��B
	 *	@retval ��L�ȊO ��M�f�[�^�T�C�Y�B
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
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@retval INVALID_SIZE �s���B
	 *	@retval ��L�ȊO ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		return INVALID_SIZE;
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B
	 *	@retval CONNECTING	�I�[�v�����ڑ����
	 *	@retval CLOSED		�N���[�Y���	
	 */
	virtual EState Check(void) const
	{
		return IsConnect() ? CONNECTING : CLOSED;
	}

	/**
	 * [����] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void) {}

	/**
	 * [�擾] ������擾.
	 *	@param type �����^�C�v.
	 *	@param index �����^�C�v�� INDEXED �̏ꍇ�A�w��K�{�B�P�ȏ�B
	 *	@retval Empty �G���[�B
	 *	@retval ��L�ȊO �����B������͎擾����������B
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
	 * [�擾] �f�o�C�X���擾
	 *	@note Open���Ă��Ȃ����̖߂�l�͕s��ł��B
	 *	@return �f�o�C�X���
	 */
	const TIoCtrlBlock_CheckDevice& GetDeviceInfo(void) const
	{
		return m_info;
	}

	/**
	 * [�ݒ�] �h���C�o�[���W�X�g���o�^.
	 *	@param vendorId �x���_�[ID
	 *	@param productId �v���_�N�gID
	 *	@param usagePage USAGE�y�[�W
	 *	@param usageId USAGE ID
	 *	@param lpszDriverName �h���C�o�[DLL��
	 *	@retval true ����
	 *	@retval false ���s.���̃h���C�o�[DLL��������Ȃ�
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
	int							m_deviceIndex;	///< �f�o�C�X�ԍ�
	TIoCtrlBlock_CheckDevice	m_info;			///< �f�o�C�X���
	HANDLE						m_handle;		///< �n���h��
	TProperty					m_property;		///< ���\
	DWORD						m_recvTimeout;	///< ��M���̃^�C���A�E�g
};



}; //TNB
