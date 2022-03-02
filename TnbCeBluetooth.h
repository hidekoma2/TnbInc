#pragma once
/**
 *@file
 * CE �p Bluetooth�֌W�̃w�b�_
 *
 *	@note Winsock2 ���g���̂ŁA�}�N�� "_TnbWINSOCK2_ENABLE" ��錾���Ă����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCeBluetoothApi.h"
#include "TnbStrVector.h"
#include "TnbRegistryAccessor.h"
#include "TnbTickCount.h"



//TNB Library
namespace TNB{



#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��
/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �A�h���X.
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
typedef ULONGLONG BT_ADDR;

/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �n���h��.
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
typedef unsigned short BT_HANDLE;
#else 
 #define BT_HANDLE unsigned short
#endif



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �����N�L�[�Ǘ�.
 *
 *		16BYTE �̃����N�L�[���Ǘ����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothLinkKey
{
public:

	/// �R���X�g���N�^.
	CBluetoothLinkKey(void)
	{
		Zero(m_key);
	}

	/**
	 * [�Q��] �A�h���X�Q��.
	 *	@return �����N�L�[�A�h���X.
	 */
	BYTE* Refer(void)
	{
		return m_key;
	}

	/**
	 * [�Q��] �A�h���X�Q��.
	 *	@return �����N�L�[�A�h���X.
	 */
	const BYTE* Refer(void) const
	{
		return m_key;
	}

	/**
	 * [�Q��] �v�f�Q��.
	 *	@param index �C���f�b�N�X�B 0�`15 ���w��ł��܂��B
	 *	@return �Q��.
	 */
	BYTE& operator[](INDEX index)
	{
		ASSERT( index < 16 );
		return m_key[index];
	}

	/**
	 * [�Q��] �v�f�Q��.
	 *	@param index �C���f�b�N�X�B 0�`15 ���w��ł��܂��B
	 *	@return �Q��.
	 */
	const BYTE& operator[](INDEX index) const
	{
		ASSERT( index < 16 );
		return m_key[index];
	}

private:
	BYTE	m_key[16]; ///< �����N�L�[
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �f�o�C�X�N���X�Ǘ�.
 *
 *		��� COD (class of device) ���Ǘ����܂��B
 *		�T�[�r�X�N���X�A���W���[�f�o�C�X�N���X�A�}�C�i�[�f�o�C�X�N���X�̎擾���\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothClass
{
public:

	/** 
	 * �T�[�r�X�N���X���.
	 *	@note bit�P�ʂňӖ��𐬂��܂��B
	 */
	enum EServiceClass
	{
		//Limited Discoverable Mode 0x0001,
		Positioning		= 0x0008,		///< Positioning (Location identification)
		Networking		= 0x0010,		///< Networking (LAN, Ad hoc, ...)
		Rendering		= 0x0020,		///< Rendering (Printing, Speaker, ...)
		Capturing		= 0x0040,		///< Capturing (Scanner, Microphone, ...)
		ObjectTransfer	= 0x0080,		///< Object Transfer (v-Inbox, v-Folder, ...)
		Audio			= 0x0100,		///< Audio (Speaker, Microphone, Headset service, ...)
		Telephony		= 0x0200,		///< Telephony (Cordless telephony, Modem, Headset service, ...)
		Information		= 0x0400,		///< Information (WEB-server, WAP-server, ...)
	};

	/**
	 * ���W���[�f�o�C�X�N���X���.
	 */
	enum EMajorDeviceClass
	{
		Miscellaneous	= 0x00,		///< Miscellaneous
		Computer		= 0x01,		///< Computer (desktop,notebook, PDA, organizers, .... ) 
		Phone			= 0x02,		///< Phone (cellular, cordless, payphone, modem, ...) 
		Network			= 0x03,		///< LAN /Network Access point 
		AV				= 0x04,		///< Audio/Video (headset,speaker,stereo, video display, vcr..... 
		Peripheral		= 0x05,		///< Peripheral (mouse, joystick, keyboards, ..... ) 
		Imaging			= 0x06,		///< Imaging (printing, scanner, camera, display, ...)
	};


	//----------------


	/**
	 * �R���X�g���N�^.
	 *	@param cod �f�o�C�X�N���X�B�ȗ������ 0 ���w�肳��܂��B
	 */
	CBluetoothClass(DWORD cod = 0)
	{
		m_class.cod = cod;
	}

	/**
	 * [�擾] �f�o�C�X�N���X�擾.
	 *	@return �f�o�C�X�N���X
	 */
	operator DWORD(void) const
	{
		return m_class.cod;
	}

	/**
	 * [�擾] �f�o�C�X�N���X�擾.
	 *	@return �f�o�C�X�N���X
	 */
	DWORD GetDeviceClass(void) const
	{
		return m_class.cod;
	}

	/**
	 * [�擾] �T�[�r�X�N���X�擾.
	 *	@return �T�[�r�X�N���X�B EServiceClass �� or �������̂ł��B
	 */
	DWORD GetServiceClass(void) const
	{
		return m_class.serviceClass;
	}

	/**
	 * [�ݒ�] �T�[�r�X�N���X�ݒ�.
	 *	@param c �T�[�r�X�N���X�B EServiceClass �� or �������̂ł��B
	 */
	void SetServiceClass(DWORD c)
	{
		m_class.serviceClass = c;
	}

	/**
	 * [�擾] ���W���[�f�o�C�X�N���X�擾.
	 *	@return ���W���[�f�o�C�X�N���X�B�ʏ�A EMajorDeviceClass �̒l�ł��B
	 */
	DWORD GetMajorDeviceClass(void) const
	{
		return m_class.majorDeviceClass;
	}

	/**
	 * [�ݒ�] ���W���[�f�o�C�X�N���X�ݒ�.
	 *	@param c ���W���[�f�o�C�X�N���X�B�ʏ�A EMajorDeviceClass �̒l�ł��B
	 */
	void SetMajorDeviceClass(DWORD c)
	{
		m_class.majorDeviceClass = c;
	}

	/**
	 * [�擾] ���W���[�f�o�C�X�N���X������擾.
	 *	@return ���W���[�f�o�C�X�N���X������.
	 */
	LPCTSTR GetMajorDeviceClassName(void) const
	{
		switch ( GetMajorDeviceClass() )
		{
		case Miscellaneous:	return _T("Miscellaneous");
		case Computer:		return _T("Computer");
		case Phone:			return _T("Phone");
		case Network:		return _T("Network");
		case AV:			return _T("Audio/Video");
		case Peripheral:	return _T("Peripheral");
		case Imaging:		return _T("Imaging");
		}
		return _T("");
	}

	/**
	 * [�擾] �}�C�i�[�f�o�C�X�N���X�擾.
	 *	@return �}�C�i�[�f�o�C�X�N���X�B
	 */
	DWORD GetMinorDeviceClass(void) const
	{
		return m_class.minorDeviceClass;
	}

	/**
	 * [�ݒ�] �}�C�i�[�f�o�C�X�N���X�ݒ�.
	 *	@param c �}�C�i�[�f�o�C�X�N���X�B
	 */
	void SetMinorDeviceClass(DWORD c)
	{
		m_class.minorDeviceClass = c;
	}

	/**
	 * [�擾] �}�C�i�[�f�o�C�X�N���X������擾.
	 *	@return �}�C�i�[�f�o�C�X�N���X������.
	 */
	LPCTSTR GetMinorDeviceClassName(void) const
	{
		switch ( GetMajorDeviceClass() )
		{
		case Computer:		return m_ComputerDeviceClassName();
		case Phone:			return m_PhoneDeviceClassName();
		case AV:			return m_AvDeviceClassName();
		case Peripheral:	return m_PeripheralDeviceClassName();
		case Imaging:		return m_ImagingDeviceClassName();
		}
		return _T("");
	}

private:
	/**
	 * [�擾] Computer�f�o�C�X�N���X������擾.
	 *	@return ������.
	 */
	LPCTSTR m_ComputerDeviceClassName(void) const
	{
		ASSERT ( GetMajorDeviceClass() == Computer );
		switch ( GetMinorDeviceClass() )
		{
		case 0:	return _T("Unclassified");
		case 1:	return _T("Desktop Computer");
		case 2:	return _T("Server Computer");
		case 3:	return _T("Laptop Computer");
		case 4:	return _T("Handled Computer");
		case 5:	return _T("Palm Computer");
		case 6:	return _T("Wearable Computer");
		}
		return _T("");
	}
	/**
	 * [�擾] Phone�f�o�C�X�N���X������擾.
	 *	@return ������.
	 */
	LPCTSTR m_PhoneDeviceClassName(void) const
	{
		ASSERT ( GetMajorDeviceClass() == Phone );
		switch ( GetMinorDeviceClass() )
		{
		case 0:		return _T("Unclassified");
		case 1:		return _T("Cellular Phone");
		case 2:		return _T("Cordless Phone");
		case 3:		return _T("Smartphone");
		case 4:		return _T("Modem");
		case 5:		return _T("ISDN Phone");
		}
		return _T("");
	}
	/**
	 * [�擾] AV�f�o�C�X�N���X������擾.
	 *	@return ������.
	 */
	LPCTSTR m_AvDeviceClassName(void) const
	{
		ASSERT ( GetMajorDeviceClass() == AV );
		switch ( GetMinorDeviceClass() )
		{
		case 0:		return _T("Unclassified");
		case 1:		return _T("Headset");
		case 2:		return _T("Handsfree");
		case 4:		return _T("Microphone");
		case 5:		return _T("Loudspeaker");
		case 6:		return _T("Headphone");
		case 7:		return _T("Portable Audio");
		case 8:		return _T("Car Audio");
		case 9:		return _T("Set Top Box");
		case 10:	return _T("Hifi Audio");
		case 11:	return _T("VCR");
		case 12:	return _T("Video Camera");
		case 13:	return _T("Camcoder");
		case 14:	return _T("Video Monitor");
		case 15:	return _T("Video Display & Loundspeaker");
		case 16:	return _T("Video Conferencing");
		case 18:	return _T("Gaiming Toy");
		}
		return _T("");
	}
	/**
	 * [�擾] Peripheral�f�o�C�X�N���X������擾.
	 *	@return ������.
	 */
	LPCTSTR m_PeripheralDeviceClassName(void) const
	{
		ASSERT ( GetMajorDeviceClass() == Peripheral );
		DWORD c = GetMinorDeviceClass();
		switch ( c & 0x3F )
		{
		case 0:		break;
		case 1:		return _T("Joystick");
		case 2:		return _T("Gamepad");
		case 3:		break;//return _T("Remote Control");
		case 4:		return _T("Sensing Device");
		case 5:		return _T("Digitizer Tablet");
		case 6:		return _T("Card Reader");
		}
		switch ( c & 0xC0 )
		{
		case 0x00:/*Unclassified*/	return _T("Unclassified");
		case 0x40:/*Keyboard*/		return _T("Keyboard");
		case 0x80:/*Pointer*/		return _T("Pointer");
		case 0xC0:/*Combo*/			return _T("Combo Device");
		}
		return _T("");
	}
	/**
	 * [�擾] Imaging�f�o�C�X�N���X������擾.
	 *	@return ������.
	 */
	LPCTSTR m_ImagingDeviceClassName(void) const
	{
		ASSERT ( GetMajorDeviceClass() == Imaging );
		#if 0
			switch ( GetMinorDeviceClass() )
			{
			#define RX_COD_MINOR_IMAJING_DISPLAY_MASK  0x10
			#define RX_COD_MINOR_IMAJING_CAMERA_MASK   0x20
			#define RX_COD_MINOR_IMAJING_SCANNER_MASK  0x40
			#define RX_COD_MINOR_IMAJING_PRINTER_MASK  0x80
			}
		#endif
		return _T("");
	}
	//
	union
	{
		DWORD		cod;	///< �f�o�C�X�N���X
		struct
		{
			DWORD	formatType		:2;		///< �t�H�[�}�b�g�^�C�v
			DWORD	minorDeviceClass:6;		///< �}�C�i�[�f�o�C�X�N���X
			DWORD	majorDeviceClass:5;		///< ���W���[�f�o�C�X�N���X
			DWORD	serviceClass	:11;	///< �T�[�r�X�N���X
			DWORD	padding			:8;		///< (��)
		};
	} m_class;	///< �N���X
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �T�[�r�X�����Ǘ�.
 *
 *		SDP�̐��f�[�^����́A�Ǘ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothServiceAttribute : CCopyImpossible
{
public:

	/**@ingroup CEBLUETOOTH
	 * Bluetooth(CE) �T�[�r�X�����l�Ǘ�.
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbCeBluetooth.h
	 * 
	 *	@date 09/10/01	�V�K�쐬
	 */
	struct TValue
	{
		SDP_TYPE			type;	///< �^�C�v
		SDP_SPECIFICTYPE	spec;	///< �ڍ׃^�C�v
		union
		{
			SDP_LARGE_INTEGER_16	int128;		///< spec = SDP_ST_INT128 �̏ꍇ�̒l
			SDP_ULARGE_INTEGER_16	uint128;	///< spec = SDP_ST_UINT128 �̏ꍇ�̒l
			GUID					uuid128;	///< spec = SDP_ST_UUID128 �̏ꍇ�̒l
			DWORD					uuid32;		///< spec = SDP_ST_UUID32 �̏ꍇ�̒l
			WORD					uuid16;		///< spec = SDP_ST_UUID16 �̏ꍇ�̒l
			LONGLONG				int64;		///< spec = SDP_ST_INT64 �̏ꍇ�̒l
			ULONGLONG				uint64;		///< spec = SDP_ST_UINT64 �̏ꍇ�̒l
			LONG					int32;		///< spec = SDP_ST_INT32 �̏ꍇ�̒l
			DWORD					uint32;		///< spec = SDP_ST_UINT32 �̏ꍇ�̒l
			SHORT					int16;		///< spec = SDP_ST_INT16 �̏ꍇ�̒l
			WORD					uint16;		///< spec = SDP_ST_UINT16 �̏ꍇ�̒l
			CHAR					int8;		///< spec = SDP_ST_INT8 �̏ꍇ�̒l
			BYTE					uint8;		///< spec = SDP_ST_UINT8 �̏ꍇ�̒l
			bool					booleanVal;	///< type = SDP_TYPE_BOOLEAN / SDP_TYPE_STRING �̏ꍇ�̒l
		};
		CAscii						str;		///< type = SDP_TYPE_URL �̏ꍇ�̒l

		/**
		 * �R���X�g���N�^.
		 *	@param t �^�C�v
		 *	@param s �ڍ�
		 */
		TValue(SDP_TYPE t = SDP_TYPE_NIL, SDP_SPECIFICTYPE s = SDP_ST_NONE) : type(t), spec(s)
		{
			Zero(int128);
		}

		/**
		 * [�擾] ������.
		 *	@param[out] pType �^�C�v�����񂪊i�[����܂��B����Ȃ��ꍇ NULL ���w�肵�܂��B
		 *	@return ������.
		 */
		CStr ToString(CStr* pType = NULL) const
		{
			CStr value;
			CStr typeName;
			switch ( type )
			{
			case SDP_TYPE_NIL:
				break;
			case SDP_TYPE_UINT:
			case SDP_TYPE_INT:
			case SDP_TYPE_UUID:
				switch ( spec )
				{
				case SDP_ST_UINT8:
					value.Format(_T("0x%02X"), uint8);
					typeName = _T("UINT8");
					break;
				case SDP_ST_UINT16:
					value.Format(_T("0x%04X"), uint16);
					typeName = _T("UINT16");
					break;
				case SDP_ST_UINT32:
					value.Format(_T("0x%08X"), uint32);
					typeName = _T("UINT32");
					break;
				case SDP_ST_UINT64:
					value.Format(_T("0x%16I64X"), uint64);
					typeName = _T("UINT64");
					break;
				case SDP_ST_UINT128:
					value.Format(_T("0x%16I64X%16I64X"), uint128.HighPart, uint128.LowPart);
					typeName = _T("UINT128");
					break;
				case SDP_ST_INT8:
					value.Format(_T("%d"), int8);
					typeName = _T("INT8");
					break;
				case SDP_ST_INT16:
					value.Format(_T("%d"), int16);
					typeName = _T("INT16");
					break;
				case SDP_ST_INT32:
					value.Format(_T("%d"), int32);
					typeName = _T("INT32");
					break;
				case SDP_ST_INT64:
					value.Format(_T("%16I64d\n"), int64);
					typeName = _T("INT64");
					break;
				case SDP_ST_INT128:
					value.Format(_T("0x%16I64X%16I64X"), int128.HighPart, int128.LowPart);
					typeName = _T("INT128");
					break;
				case SDP_ST_UUID16:
					value.Format(_T("0x%04X"), uint16);
					typeName = _T("UUID16");
					break;
				case SDP_ST_UUID32:
					value.Format(_T("0x%08X"), uint32);
					typeName = _T("UUID32");
					break;
				case SDP_ST_UUID128:
					value.Format(_T("%08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X")
						, uuid128.Data1, uuid128.Data2, uuid128.Data3
						, uuid128.Data4[0], uuid128.Data4[1], uuid128.Data4[2], uuid128.Data4[3]
						, uuid128.Data4[4], uuid128.Data4[5], uuid128.Data4[6], uuid128.Data4[7]);
					typeName = _T("UUID128");
					break;
				default:
					ASSERT( false );
					break;
				}
				break;
			case SDP_TYPE_BOOLEAN:
				value = booleanVal ? _T("true") : _T("false");
				typeName = _T("BOOL");
				break;
			case SDP_TYPE_STRING:
				value = str;
				typeName = _T("STRING");
				break;
			case SDP_TYPE_URL:
				value = str;
				typeName = _T("URL");
				break;
			default:
				ASSERT( false );
				break;
			}
			if ( pType != NULL )
			{
				*pType = typeName;
			}
			return value;
		}
	};

	/// ID�z��.
	typedef CVectorT<WORD> TIdsList;
	/// �l�z��.
	typedef CVectorT<TValue> TValuesList;


	//----------------------------


	/// �R���X�g���N�^
	CBluetoothServiceAttribute(void) : m_ppSdpRecords(NULL), m_numRecords(0), m_lastResult(S_OK)
	{
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	}
	
	/// �f�X�g���N�^
	~CBluetoothServiceAttribute(void)
	{
		Free();
	}

	/**
	 * [�ݒ�] �j��.
	 *		�ێ����Ă���������ׂĔj�����܂��B
	 */
	void Free(void)
	{
		if ( m_ppSdpRecords != NULL )
		{
			loop ( i, m_numRecords )
			{
				m_ppSdpRecords[i]->Release();
			}
			BYTE* B = reinterpret_cast<BYTE*>(m_ppSdpRecords);
			delete[] B;
			m_ppSdpRecords = NULL;
			m_numRecords = 0;
		}
	}

	/**
	 * [�m�F] �L���m�F.
	 *		Analyze() �Ő���������{�N���X���ێ����Ă���f�[�^���L���ɂȂ�܂��B
	 *	@retval true �L��.
	 *	@retval false ����.
	 */
	bool IsValid(void) const
	{
		return m_ppSdpRecords != NULL;
	}

	/**
	 * [�擾] �ێ����R�[�h��.
	 *	@return ���R�[�h��.
	 */
	size_t GetRecordCount(void) const
	{
		return m_numRecords;
	}

	/**
	 * [�擾] �ŏI���U���g�擾.
	 *	@return ���U���g.
	 */
	HRESULT GetLastResult(void) const
	{
		return m_lastResult;
	}

	/**
	 * [���] ���.
	 *		���f�[�^���.
	 *	@param size ���f�[�^��.
	 *	@param lpRawData ���f�[�^.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ� GetLastResult() �œ����܂��B
	 */
	bool Analyze(size_t size, LPCVOID lpRawData)
	{
		CWorkMem raw;
		raw.Reset(size, static_cast<const BYTE*>(lpRawData));
		Free();
		ISdpStream* pIStream = NULL;
		HRESULT hr = ::CoCreateInstance(__uuidof(SdpStream), NULL, CLSCTX_INPROC_SERVER
								, __uuidof(ISdpStream), reinterpret_cast<LPVOID*>(&pIStream));
		if ( SUCCEEDED(hr) )
		{
			ULONG ulError;
			hr = pIStream->Validate(raw.Ref(), raw.GetSize(), &ulError);
			if ( SUCCEEDED(hr) )
			{
				hr = pIStream->VerifySequenceOf(raw.Ref(), raw.GetSize(), SDP_TYPE_SEQUENCE, NULL, &m_numRecords);
				if ( SUCCEEDED(hr) )
				{
					ASSERT( m_numRecords > 0 );
					BYTE* B = new BYTE[sizeof(ISdpRecord*) * m_numRecords];
					m_ppSdpRecords = reinterpret_cast<ISdpRecord**>(B);
					ASSERT( m_ppSdpRecords != NULL );
					hr = pIStream->RetrieveRecords(raw.Ref(), raw.GetSize(), m_ppSdpRecords, &m_numRecords);
				}
			}
			pIStream->Release();
			pIStream = NULL;
		}
		if ( ! m_Res(hr) )
		{
			Free();
			return false;
		}
		if ( m_numRecords == 0 )
		{
			return m_Res(E_FAIL);
		}
		return SearchRecord(0);
	}

	/**
	 * [����] ���R�[�h����.
	 *		�w�背�R�[�h��T���A���e�������o�ɋL�����܂��B
	 *	@note Analyze() ���� SearchRecord(0) �����s����Ă��܂��B ���R�[�h�O�ȊO���������ɁA
	 *			�{���\�b�h���g���܂��B
	 *	@param record ���R�[�h�ԍ��B 0 ���� GetRecordCount() �����̒l���w��ł��܂��B
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ� GetLastResult() �œ����܂��B
	 */
	bool SearchRecord(INDEX record)
	{
		if ( record < m_numRecords )
		{
			return m_CheckRecord(m_ppSdpRecords[record]);
		}
		m_attrIds.RemoveAll();
		m_attrDetails.RemoveAll();
		return false;
	}

	/**
	 * [�m�F] �����f�[�^�m�F.
	 *	@param attributeId ����ID�B
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool HasAttribute(WORD attributeId) const
	{
		return m_attrIds.Find(attributeId) != INVALID_INDEX;
	}

	/**
	 * [�擾] ����ID�ꗗ�擾.
	 *	@return ����ID�ꗗ.
	 */
	const TIdsList& GetAttributeIdList(void) const
	{
		return m_attrIds;
	}

	/**
	 * [�擾] ����ID�f�[�^�擾.
	 *	@param attributeId ����ID�B
	 *		<table>
	 *		<tr>
	 *			<td>����ID</td>	<td>�V���{��</td>
	 *			<td>�����e</td>
	 *		</tr><tr>
	 *			<td> 0x0000 </td> <td> SDP_ATTRIB_RECORD_HANDLE </td>
	 *			<td> Record Handle </td>
	 *		</tr><tr>
	 *			<td> 0x0001 </td> <td> SDP_ATTRIB_CLASS_ID_LIST </td>
	 *			<td> Class ID List </td>
	 *		</tr><tr>
	 *			<td> 0x0004 </td> <td> SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST </td>
	 *			<td> Protocol Descriptor List.\n UUID16 = 3 �̒���� UINT8 �� RFCOMM Channel ID�B </td>
	 *		</tr><tr>
	 *			<td> 0x0005 </td> <td> SDP_ATTRIB_BROWSE_GROUP_LIST </td>
	 *			<td> Browse Group List </td>
	 *		</tr><tr>
	 *			<td> 0x0006 </td> <td> SDP_ATTRIB_LANG_BASE_ATTRIB_ID_LIST </td>
	 *			<td> Language Base\n ���� Language, Character Encoding, Base Attribute ID </td>
	 *		</tr><tr>
	 *			<td> 0x0009 </td> <td> SDP_ATTRIB_PROFILE_DESCRIPTOR_LIST </td>
	 *			<td> Profile Descriptor List\n UINT16�� Version�B </td>
	 *		</tr>
 	 *		</table>
	 *	@return �f�[�^.
	 */
	const TValuesList& GetAttributeData(WORD attributeId) const
	{
		INDEX f = m_attrIds.Find(attributeId);
		if ( f != INVALID_INDEX )
		{
			return m_attrDetails[f];
		}
		return m_dummy;
	}

	/**
	 * [�擾] ����������擾.
	 *	@note �������̕�����𒊏o���܂��B
	 *	@return ������Q.
	 */
	CStrVector GetAttributeString(void) const
	{
		CStrVector vs;
		loop ( i, m_attrDetails )
		{
			const TValuesList& list = m_attrDetails[i];
			loop ( i, list )
			{
				SDP_TYPE s = list[i].type;
				if ( s == SDP_TYPE_STRING || s == SDP_TYPE_URL )
				{
					vs.Add(CStr(list[i].str));
				}
			}
		}
		return vs;
	}
	
private:
	TIdsList				m_attrIds;			///< ��������ID�ꗗ
	CVectorT<TValuesList>	m_attrDetails;		///< �������������f�[�^
	TValuesList				m_dummy;			///< ���s���̉����p.
	ISdpRecord**			m_ppSdpRecords;		///< array of pSdpRecords
	ULONG					m_numRecords;		///< number of elements in pSdpRecords
	HRESULT					m_lastResult;		///< ���X�g���U���g
	/// ���R�[�h�`�F�b�N
	bool m_CheckRecord(ISdpRecord* pRecord)
	{
		WORD* pList = NULL;
		ULONG listSize = 0;
		m_attrIds.RemoveAll();
		m_attrDetails.RemoveAll();
		HRESULT hr = pRecord->GetAttributeList(&pList, &listSize);
		if ( SUCCEEDED(hr) )
		{
			m_attrIds.SetSize(listSize);
			m_attrDetails.SetSize(listSize);
			loop ( i, listSize )
			{
				BYTE* pStream = NULL;
				ULONG size = 0;
				WORD aid = pList[i];
				m_attrIds[i] = aid;
				hr = pRecord->GetAttributeAsStream(aid, &pStream, &size);
				if ( SUCCEEDED(hr) )
				{
					TTRACE1("Attrib ID %04X\n", aid);
					hr = m_CheckSub(m_attrDetails[i], pStream, size);
				}
				if ( pStream != NULL )
				{
					::CoTaskMemFree(pStream);
				}
				if ( ! SUCCEEDED(hr) )
				{
					break;
				}
			}
		}
		if ( pList != NULL )
		{
			::CoTaskMemFree(pList);
		}
		return m_Res(hr);
	}
	/// ���R�[�h�`�F�b�N(UINT)
	void m_CheckSubUint(TValue& _val, SDP_SPECIFICTYPE specType, BYTE* pSubElement, ISdpStream* pSdpStream)
	{
		switch ( specType )
		{
		case SDP_ST_UINT8:
			_val.uint8 = *pSubElement;
			break;
		case SDP_ST_UINT16:
			pSdpStream->RetrieveUint16(pSubElement, &_val.uint16);
			_val.uint16 = SwapEndian(_val.uint16);
			break;
		case SDP_ST_UINT32:
			pSdpStream->RetrieveUint32(pSubElement, &_val.uint32);
			_val.uint32 = SwapEndian(_val.uint32);
			break;
		case SDP_ST_UINT64:
			pSdpStream->RetrieveUint64(pSubElement, &_val.uint64);
			_val.uint64 = SwapEndian(_val.uint64);
			break;
		case SDP_ST_UINT128:
			pSdpStream->RetrieveUint128(pSubElement, &_val.uint128);
			_val.uint128.LowPart  = SwapEndian(_val.uint128.LowPart);
			_val.uint128.HighPart = SwapEndian(_val.uint128.HighPart);
			break;
		default:
			ASSERT( false );
			break;
		}
	}
	/// ���R�[�h�`�F�b�N(INT)
	void m_CheckSubInt(TValue& _val, SDP_SPECIFICTYPE specType, BYTE* pSubElement, ISdpStream* pSdpStream)
	{
		switch ( specType )
		{
		case SDP_ST_INT8:
			_val.int8 = *pSubElement;
			break;
		case SDP_ST_INT16:
			pSdpStream->RetrieveInt16(pSubElement, &_val.int16);
			_val.int16 = SwapEndian(static_cast<WORD>(_val.int16));
			break;
		case SDP_ST_INT32:
			pSdpStream->RetrieveInt32(pSubElement, &_val.int32);
			_val.int32 = SwapEndian(static_cast<DWORD>(_val.int32));
			break;
		case SDP_ST_INT64:
			pSdpStream->RetrieveInt64(pSubElement, &_val.int64);
			_val.int64 = SwapEndian(static_cast<ULONGLONG>(_val.int64));
			break;
		case SDP_ST_INT128:
			pSdpStream->RetrieveInt128(pSubElement, &_val.int128);
			_val.int128.LowPart  = SwapEndian(static_cast<ULONGLONG>(_val.int128.LowPart));
			_val.int128.HighPart = SwapEndian(static_cast<ULONGLONG>(_val.int128.HighPart));
			break;
		default:
			ASSERT( false );
			break;
		}
	}
	/// ���R�[�h�`�F�b�N(UUID)
	void m_CheckSubUuid(TValue& _val, SDP_SPECIFICTYPE specType, BYTE* pSubElement, ISdpStream* pSdpStream)
	{
		switch ( specType )
		{
		case SDP_ST_UUID16:
			pSdpStream->RetrieveUint16(pSubElement, &_val.uuid16);
			_val.uuid16 = SwapEndian(_val.uuid16);
			break;
		case SDP_ST_UUID32:
			pSdpStream->RetrieveUint32(pSubElement, &_val.uuid32);
			_val.uuid32 = SwapEndian(_val.uuid32);
			break;
		case SDP_ST_UUID128:
			pSdpStream->RetrieveUuid128(pSubElement, &_val.uuid128);
			_val.uuid128.Data1 = SwapEndian(_val.uuid128.Data1);
			_val.uuid128.Data2 = SwapEndian(_val.uuid128.Data2);
			_val.uuid128.Data3 = SwapEndian(_val.uuid128.Data3);
			break;
		default:
			ASSERT( false );
			break;
		}
	}
	/// ���R�[�h�`�F�b�N�T�u
	bool m_CheckSub(TValuesList& _list, BYTE* pStream, ULONG size)
	{
		ISdpStream* pSdpStream = NULL;
		if ( ! m_LoadAndValidate(&pSdpStream, &pStream, &size) )
		{
			return false;
		}
		HRESULT hr = E_FAIL;
		while ( size > 0 )
		{
			SDP_TYPE type;
			SDP_SPECIFICTYPE specType;
			ULONG recordSize;
			ULONG storageSize;
			BYTE* pSubElement = NULL;
			hr = pSdpStream->RetrieveElementInfo(pStream, &type, &specType, &recordSize, &storageSize, &pSubElement);
			ASSERT( SUCCEEDED(hr) );
			if ( type == SDP_TYPE_SEQUENCE || type == SDP_TYPE_ALTERNATIVE )
			{
				if ( ! m_CheckSub(_list, pSubElement, recordSize) )
				{
					return false;
				}
			}
			else 
			{
				TValue val(type, specType);
				switch ( type )
				{
				case SDP_TYPE_NIL:
					break;
				case SDP_TYPE_UINT:
					m_CheckSubUint(val, specType, pSubElement, pSdpStream);
					break;
				case SDP_TYPE_INT:
					m_CheckSubInt(val, specType, pSubElement, pSdpStream);
					break;
				case SDP_TYPE_UUID:
					m_CheckSubUuid(val, specType, pSubElement, pSdpStream);
					break;
				case SDP_TYPE_BOOLEAN:
					val.booleanVal = ((*pSubElement) != 0);
					break;
				case SDP_TYPE_STRING:
					val.str.SetFromLeft(reinterpret_cast<LPCSTR>(pSubElement), recordSize);
					break;
				case SDP_TYPE_URL:
					val.str.SetFromLeft(reinterpret_cast<LPCSTR>(pSubElement), recordSize);
					break;
				default:
					val.type = SDP_TYPE_NIL;
					break;
				}
				if ( val.type != SDP_TYPE_NIL )
				{
					#ifdef _DEBUG
						CStr type;
						CStr va = val.ToString(&type);
						TTRACE2("  %s: %s\n", type, va);
					#endif
					_list.Add(val);
				}
			}
			recordSize += 1 + storageSize;
			pStream += recordSize;
			size -= recordSize;
		}
		pSdpStream->Release();
		return m_Res(hr);
	}
	/// ���[�h
	bool m_LoadAndValidate(ISdpStream **ppSdpStream, BYTE **ppStream, ULONG *pSize)
	{
		HRESULT hr = ::CoCreateInstance(__uuidof(SdpStream), NULL, CLSCTX_INPROC_SERVER
								, __uuidof(ISdpStream), reinterpret_cast<LPVOID*>(ppSdpStream));
		if ( SUCCEEDED(hr) )
		{
			ULONG_PTR sdpError;
			ASSERT( *ppSdpStream != NULL );
			hr = (*ppSdpStream)->Validate(*ppStream, *pSize, &sdpError);
		}
		return m_Res(hr);
	}
	/// ���U���g�Ǘ�
	bool m_Res(HRESULT hr)
	{
		m_lastResult = hr;
		return SUCCEEDED(hr);
	}
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �A�h���X�Ǘ�.
 *
 *	�e Bluetooth �Ή��f�o�C�X�����ʂ���Ƃ��Ɏg�p����� 48 �r�b�g �A�h���X���A��Ǘ����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothAddress
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@param btAddr Bluetooth�A�h���X.
	 */
	CBluetoothAddress(BT_ADDR btAddr = 0) : m_btAddr(btAddr)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��.
	 */
	CBluetoothAddress(const CBluetoothAddress& other)
	{
		m_btAddr = other.m_btAddr;
	}

	/**
	 * �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��.
	 */
	CBluetoothAddress& operator=(const CBluetoothAddress& other)
	{
		m_btAddr = other.m_btAddr;
		return *this;
	}

	/**
	 * [�m�F] �L���m�F.
	 *	@note �ێ����Ă���A�h���X�� 0 �Ȃ�A�����Ƃ��܂��B
	 *	@retval true �L��.
	 *	@retval false ����.
	 */
	bool IsValid(void) const
	{
		return m_btAddr != 0;
	}

	/**
	 * [�擾] �A�h���X�擾.
	 *	@return �A�h���X.
	 */
	operator const BT_ADDR&(void) const
	{
		return m_btAddr;
	}

	/**
	 * [�擾] �A�h���X�擾.
	 *	@return �A�h���X.
	 */
	const BT_ADDR& GetAddress(void) const
	{
		return m_btAddr;
	}

	/**
	 * [�擾] �A�h���X������擾.
	 *		�ێ����Ă���A�h���X�� xx:xx:xx:xx:xx:xx �ƌ���������ɂ��܂��B
	 *	@return �A�h���X������.
	 */
	CStr GetAddressString(void) const
	{
		CStr str;
		loop ( i, 6 )
		{
			if ( i != 0 )
			{
				str += _T(":");
			}
			str += CStr::Fmt(_T("%02X"), (m_btAddr >> ((5 - i) * 8)) & 0xFF);
		}
		return str;
	}

	/**
	 * [�ݒ�] �A�h���X������ݒ�.
	 *		�A�h���X�� xx:xx:xx:xx:xx:xx �ƌ���������Ŏw�肵�܂��B
	 *	@param lpszAddress �A�h���X
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetAddressString(LPCTSTR lpszAddress)
	{
		BT_ADDR a = 0;
		if ( STRLIB::GetLen(lpszAddress) == 6 * 2 + 5 )
		{
			loop ( i, 5 )
			{
				if ( lpszAddress[i * 3 + 2] != ':' )
				{
					return false;
				}
			}
			loop ( i, 6 )
			{
				LONGLONG r = m_ToInt(&lpszAddress[i * 3]);
				if ( r < 0 || r > 255 )
				{
					return false;
				}
				a += r << ((5 - i) * 8);
			}
			m_btAddr = a;
			return true;
		}
		return false;
	}

protected:
	BT_ADDR		m_btAddr;		///< �A�h���X

private:
	// int��
	int m_ToInt(LPCTSTR lpsz) const
	{
		int i1 = STRLIB::HexCharToInt(lpsz[0]);
		int i2 = STRLIB::HexCharToInt(lpsz[1]);
		if ( i1 >= 0 && i2 >= 0 )
		{
			return i1 << 4 | i2;
		}
		return -1;
	}
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) ���[�J���Ǘ�.
 *
 *		�������g�̃f�o�C�X���Ǘ�.
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothLocal : private CBluetoothApi
{
	DEFSUPER(CBluetoothApi);
public:

	/// �R���X�g���N�^
	CBluetoothLocal(void)
	{
	}

	/**
	 * [�擾] �A�h���X�擾.
	 *	@param[out] _addr �擾�����A�h���X���i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetAddress(CBluetoothAddress& _addr) const
	{
		BT_ADDR ba;
		bool r = m_Ret(BthReadLocalAddr(&ba));
		if ( r )
		{
			_addr = ba;
		}
		return r;
	}

	/**
	 * [�擾] ���O�擾.
	 *	@retval Empty��� �G���[�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 *	@retval ����ȊO �����B���e�͖��O�B
	 */
	CStr GetName(void) const
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, KEY_QUERY_VALUE);
		return reg[_T("Ident")].QueryString(_T("Name"));
	}

	/**
	 * [�ݒ�] ���O�ݒ�.
	 *	@param lpszName �ݒ肷�閼�O
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetName(LPCTSTR lpszName)
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE);
		return reg[_T("Ident")].WriteString(_T("Name"), lpszName);
	}

	/**
	 * [�擾] �f�o�C�X�N���X�擾.
	 *	@param[out] _class �擾�����f�o�C�X�N���X
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetClass(CBluetoothClass& _class) const
	{
		UINT cl = 0;
		bool r = m_Ret(BthReadCOD(&cl));
		_class = cl;
		return r;
	}

	/**
	 * [�ݒ�] �f�o�C�X�N���X�ݒ�.
	 *	@param clas �f�o�C�X�N���X
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetClass(const CBluetoothClass& clas)
	{
		UINT cl = clas.GetDeviceClass();
		return m_Ret(BthWriteCOD(cl));
	}

	/**
	 * [�ݒ�] �F�ؗL������.
	 *	@param isEnable ture �Ȃ�F�؂�L���A false �Ȃ疳���ɂ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool EnableAuthentication(bool isEnable)
	{
		return m_Ret(BthWriteAuthenticationEnable(isEnable ? 1 : 0));
	}

	/**
	 * [�擾] �F�؏�Ԏ擾.
	 *	@param[out] _isEnabled �F�؏�Ԃ��i�[����܂��B ture �Ȃ�F�؂�L���A false �Ȃ疳����\���܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool IsEnabledAuthentication(bool& _isEnabled) const
	{
		BYTE ae;
		bool r = m_Ret(BthReadAuthenticationEnable(&ae));
		if ( r )
		{
			_isEnabled = (ae != 0);
		}
		return r;
	}
	
	/// ���[�h
	enum EMode
	{
		PowerOff			= -1,	///< Power OFF�B
		DisabledAll			= 0,	///< Power ON�BInquiry �� Disable�A Page Scan �� Disable�B
		EnabledInquiry		= 1,	///< Power ON�BInquiry �� Enable�A  Page Scan �� Disable�B
		EnabledPageScan		= 2,	///< Power ON�BInquiry �� Disable�A Page Scan �� Enable�B
		EnabledAll			= 3,	///< Power ON�BInquiry �� Enable�A  Page Scan �� Enable�B
	};

	/**
	 * [�擾] ���[�h�擾.
	 *	@param[out] _mode ���[�h���i�[����܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetMode(EMode& _mode) const
	{
		DWORD mode = 0;
		BYTE mask = 0;
		int r = BthGetMode(&mode);
		if ( r == ERROR_SUCCESS )
		{
			if ( mode == BTH_POWER_OFF )
			{
				_mode = PowerOff;
			}
			else
			{
				r = BthReadScanEnableMask(&mask);
				if ( r == ERROR_SUCCESS )
				{
					ASSERT( mask >= 0 && mask <= 3 );
					_mode = static_cast<EMode>(mask);
				}
			}
		}
		return m_Ret(r);
	}

	/**
	 * [�ݒ�] ���[�h�ݒ�.
	 *	@note OFF����ON�ɂ���ꍇ�A���Ԃ������邱�Ƃ�����܂��B���̍ő�҂����Ԃ��w��ł��܂��B
	 *	@param mode ���[�h�B
	 *	@param timeout OFF->ON�̃^�C���A�E�g����(ms)�B���ԓ��� OFF->ON�ɂȂ�Ȃ��������A GetLastError() �� ERROR_TIMEOUT ��Ԃ��܂��B
	 *	@retval true �ݒ萬���B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetMode(EMode mode, DWORD timeout = 3000)
	{
		int e = ERROR_SUCCESS;
		if ( mode == PowerOff )
		{
			e = BthSetMode(BTH_POWER_OFF);
		}
		else
		{
			int status;
			e = BthGetHardwareStatus(&status);
			if ( e == ERROR_SUCCESS && status == HCI_HARDWARE_NOT_PRESENT )
			{
				e = ERROR_DEVICE_NOT_CONNECTED;
			}
			if ( e == ERROR_SUCCESS )
			{
				e = BthSetMode(BTH_DISCOVERABLE);
			}
			if ( e == ERROR_SUCCESS )
			{
				e = ERROR_TIMEOUT;
				CTickCount tick;
				do
				{
					int e1 = BthGetHardwareStatus(&status);
					if ( e1 != ERROR_SUCCESS )
					{
						e = e1;
						break;
					}
					if ( status == HCI_HARDWARE_RUNNING )
					{
						e = BthWriteScanEnableMask(mode);
						break;
					}
					::Sleep(10);
				}
				while( ! tick.IsPassed(timeout) );
			}
		}
		return m_Ret(e);
	}

	/**
	 * [�擾] �y�[�W�^�C���A�E�g���Ԏ擾.
	 *	@param[out] _timeout ���Ԃ��i�[�����B ���l x 0.625ms ��������
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetPageTimeout(DWORD& _timeout) const
	{
		WORD ot = 0;
		bool r = m_Ret(BthReadPageTimeout(&ot));
		if ( r )
		{
			_timeout = ot;
		}
		return r;
	}

	/**
	 * [�ݒ�] �y�[�W�^�C���A�E�g���Ԑݒ�.
	 *	@param timeout ���ԁB ���l x 0.625ms �������ԁB
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetPageTimeout(DWORD timeout)
	{
		WORD ot = static_cast<WORD>(timeout);
		return m_Ret(BthWritePageTimeout(ot));
	}

	/**
	 * [�擾] �y�[�W�^�C���A�E�g���Ԏ擾.
	 *	@note ���Ԃ��~���b�ň����܂����A�덷���ł܂��B
	 *	@param[out] _timeout ���Ԃ��i�[�����B�P�ʂ̓~���b�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetPageTimeoutMs(DWORD& _timeout) const
	{
		DWORD v = 0;
		bool r = GetPageTimeout(v);
		if ( r )
		{
			double d = v;
			_timeout = static_cast<DWORD>(d * .625);
		}
		return r;
	}

	/**
	 * [�ݒ�] �y�[�W�^�C���A�E�g���Ԑݒ�.
	 *	@note ���Ԃ��~���b�ň����܂����A�덷���ł܂��B
	 *	@param timeout ���ԁB�P�ʂ̓~���b�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetPageTimeoutMs(DWORD timeout)
	{
		double d = static_cast<double>(timeout);
		return SetPageTimeout(static_cast<DWORD>(d / .625));
	}

	/**
	 * [�擾] ��Ԏ擾.
	 *		���u�̏�Ԃ��擾���܂��B
	 *	@retval HCI_HARDWARE_RUNNING The adapter is installed and the stack is running. 
	 *	@retval HCI_HARDWARE_INITIALIZING  The adapter might be installed. 
	 *										The stack is currently on the way up. Call again later.
	 *	@retval HCI_HARDWARE_ERROR The adapter might be installed. 
	 *								The stack is on the way down. Call again later.
	 *	@retval HCI_HARDWARE_SHUTDOWN The adapter is installed, but the stack is not running. 
	 *	@retval HCI_HARDWARE_NOT_PRESENT The adapter is not present. 
	 *	@retval HCI_HARDWARE_UNKNOWN  The stack is not present. �ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	int GetStatus(void) const
	{
		int s = HCI_HARDWARE_UNKNOWN;
		int e = BthGetHardwareStatus(&s);
		if ( e == ERROR_SUCCESS && s == HCI_HARDWARE_UNKNOWN )
		{
			e = ERROR_SERVICE_NOT_ACTIVE;
		}
		::SetLastError(e);
		return s;
	}

	/**
	 * [�ݒ�] �A�C�h���ڑ��ؒf.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool TerminateIdleConnections(void)
	{
		return m_Ret(BthTerminateIdleConnections());
	}

	/**
	 * [�ݒ�] Inquiry�t�B���^�ݒ�.
	 *		�w��̃A�h���X�������Ώۂɂ��܂��B
	 *	@note �����Ώۂɂ������ꍇ�A�{���\�b�h�𐔉�g���܂��B
	 *	@param addr �A�h���X�B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetInquiryFilter(const CBluetoothAddress& addr)
	{
		BT_ADDR ba = addr;
		return m_Ret(BthSetInquiryFilter(&ba));
	}

	/**
	 * [�ݒ�] Inquiry�t�B���^�ݒ����.
	 *		SetInquiryFilter() �ł̎w����������܂��B
	 *	@note ������̉����͏o���܂���B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool ClearInquiryFilter(void)
	{
		return m_Ret(BthClearInquiryFilter());
	}

	/**
	 * [����] Inquiry���s.
	 *		����̃f�o�C�X���������܂��B
	 *	@param timeout ��������(�P�ʁGms)�B���x�� 1280ms �ł��B�ő�� 60�b�ł��B
	 *	@return �A�h���X�ꗗ. Invalid ��ԂȂ�G���[�B
	 */
	CVectorT<BthInquiryResult> Inquiry(DWORD timeout = 1000) const
	{
		CVectorT<BthInquiryResult> vb;
		const UINT LAP = 0x9e8b33; // GIAC_LAP
		DWORD length = timeout / 1280;
		if ( length == 0 )
		{
			length = 1;
		}
		if ( length > 0x30 )
		{
			length = 0x30;
		}
		UINT got = 0;
		CWorkMemT<BthInquiryResult> ir(256);
		bool r = m_Ret(BthPerformInquiry(LAP, static_cast<BYTE>(length), 0, 256, &got, ir));
		if ( r )
		{
			vb.CopyElements(got, ir);
		}
		else
		{
			vb.Invalid();
		}
		return vb;
	}
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �f�o�C�X�Ǘ�.
 *
 *		��̃A�h���X���Ǘ�.
 *		���̃A�h���X�̃f�o�C�X�̃T�[�r�X�Ȃǎ擾�\�B
 *
 *	@note �ꕔ�̃��\�b�h�� CreateScoConnection() �ȂǂŁA
 *		�x�[�X�o���h�ڑ����Ă��Ȃ��ƁA���s���܂��i �G���[�� ERROR_NOT_FOUND ���Ԃ�j�B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 *	@date 10/01/07	PIN�R�[�h�̉��߂����P�B
 */
class CBluetoothDevice : public CBluetoothAddress, private CBluetoothApi
{
	DEFSUPER(CBluetoothAddress);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param btAddr Bluetooth�A�h���X.
	 */
	CBluetoothDevice(const BT_ADDR& btAddr = 0) : _super(btAddr)
	{
	}

	/**
	 * �R���X�g���N�^.
	 *	@param btHandle Bluetooth�n���h��.
	 */
	CBluetoothDevice(BT_HANDLE btHandle)
	{
		if ( BthGetAddress(btHandle, &m_btAddr) != ERROR_SUCCESS )
		{
			m_btAddr = 0;
		}
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��.
	 */
	CBluetoothDevice(const CBluetoothDevice& other)
	{
		m_btAddr = other.m_btAddr;
	}

	/**
	 * �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��.
	 */
	CBluetoothDevice& operator=(const CBluetoothDevice& other)
	{
		m_btAddr = other.m_btAddr;
		return *this;
	}

	/// Bluetooth(CE) �f�o�C�X�̃o�[�W�����Ǘ�
	struct TVersion
	{
		BYTE version;		///< �o�[�W����
		WORD subVersion;	///< �T�u�o�[�W����
		WORD manufacturer;	///< ���[�J�[
		BYTE features;		///< features
	};

	/**
	 * [�擾] �o�[�W�����擾.
	 *	@param[out] _version �o�[�W������񂪊i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetVersion(TVersion& _version)
	{
		BT_HANDLE h = CreateAclConnection();
		int e = BthReadRemoteVersion(&m_btAddr, 
				&_version.version, &_version.subVersion, &_version.manufacturer, &_version.features);
		if ( h != NULL )
		{
			CloseConnection(h);
		}
		return m_Ret(e);
	}

	/**
	 * [�m�F] ���݊m�F.
	 *	@note GetName() ���g���Ċm�F���Ă��܂��B
	 *	@retval true ���݁B
	 *	@retval false ���Ȃ��B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool IsExist(void) const
	{
		return ! GetName().IsEmpty();
	}

	/**
	 * [�擾] �f�o�C�X�N���X�擾.
	 *	@note baseband �ڑ�����Ă��Ȃ��ꍇ�A���s���܂��B
	 *	@param[out] _class
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetClass(CBluetoothClass& _class) const
	{
		UINT cl = 0;
		BT_ADDR ba = m_btAddr;
		bool r = m_Ret(BthGetRemoteCOD(&ba, &cl));
		_class = cl;
		return r;
	}

	/**
	 * [�擾] �f�o�C�X�N���X�擾.
	 *	@note baseband �ڑ�����Ă��Ȃ��ꍇ�A��x�ڑ����܂��B
	 *	@param[out] _class
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetClassEx(CBluetoothClass& _class)
	{
		bool r = GetClass(_class);
		if ( ! r && ::GetLastError() == ERROR_NOT_FOUND )
		{
			BT_HANDLE h = CreateAclConnection();
			r = GetClass(_class);
			CloseConnection(h);
		}
		return r;
	}

	/**
	 * [�擾] �f�o�C�X���擾.
	 *	@retval Empty������ ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 *	@retval ��L�ȊO �����B������̓f�o�C�X���B
	 */
	CStr GetName(void) const
	{
		BT_ADDR ba = m_btAddr;
		CUnicode str;
		const int len = 1024;
		UINT requ = 0;
		bool r = m_Ret(BthRemoteNameQuery(&ba, len, &requ, str.GetBuffer(len)));
		str.ReleaseBuffer();
		return r ? str : _T("");
	}

	/**
	 * [����] �F��.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool Authenticate(void) const
	{
		BT_ADDR ba = m_btAddr;
		return m_Ret(BthAuthenticate(&ba));
	}

	/**
	 * [����] �����p�p�P�b�g�I�[�v��.
	 *		SCO(Synchronous Connection Oriented) �ڑ����܂��B
	 *	@note SCO �̓���
	 *		- �}�X�^�ƈ�̃X���[�u�� 1vs1 �̒ʐM
	 *		- �ʐM���x��ۏ�B
	 *		- �p�P�b�g�̍đ��̓T�|�[�g���Ȃ�
	 *		- ���A���^�C��������
	 *	@retval NULL�ȊO �����B �g�p��A CloseConnection() �Őؒf���Ă��������B
	 *	@retval NULL ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	BT_HANDLE CreateScoConnection(void)
	{
		BT_HANDLE h = NULL;
		if ( ! m_Ret(BthCreateSCOConnection(&m_btAddr, &h)) )
		{
			return NULL;
		}
		return h;
	}

	/**
	 * [����] �f�[�^�ʐM�p�p�P�b�g�I�[�v��.
	 *		ACL(Asynchronous Connectionless) �ڑ����܂��B
	 *	@note ACL �̓���
	 *		- �}�X�^�ƕ����̃X���[�u�� 1vs�� �̒ʐM
	 *		- ���x��ۏႵ�Ȃ�
	 *		- �p�P�b�g�̍đ����T�|�[�g
	 *		- �f�[�^�ʐM�ɗ��p
	 *	@retval NULL�ȊO �����B �g�p��A CloseConnection() �Őؒf���Ă��������B
	 *	@retval NULL ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	BT_HANDLE CreateAclConnection(void)
	{
		BT_HANDLE h = NULL;
		if ( ! m_Ret(BthCreateACLConnection(&m_btAddr, &h)) )
		{
			return NULL;
		}
		return h;
	}

	/// ACL ���[�h
	enum EMode
	{
		ACTIVE,	///< �A�N�e�B�u���[�h.
		HOLD,	///< �z�[���h���[�h.	@note �����N�𒆒f������B�s�R�l�b�g���̓����͈ێ������B
		SNIFF,	///< �X�j�t���[�h.		@note �X���[�u���L�B�X�j�t�����i��`�������Ԋu�j�Ńp�P�b�g�̑���M���\�B
		PARK	///< �p�[�N���[�h.		@note �X���[�u���L�̏ȓd�̓��[�h�B�}�X�^����̃p�P�b�g�͎�M�\�B���M�s�\�B
	};

	/**
	 * [�擾] .ACL���[�h�擾.
	 *	@note ACL �ڑ����Ă��Ȃ��ƃG���[�ɂȂ�܂��B
	 *	@param[out] _mode �擾�������[�h���i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetCurrentMode(EMode& _mode) const
	{
		BT_ADDR ba = m_btAddr;
		BYTE m = 0;
		bool r = m_Ret(BthGetCurrentMode(&ba, &m));
		_mode = static_cast<EMode>(m);
		return r;
	}

	/**
	 * [�ݒ�] �z�[���h���[�h�J�n.
	 *	@note ACL �ڑ����Ă��Ȃ��ƃG���[�ɂȂ�܂��B
	 *	@param[out] _interval Baseband slots. 
	 *	@param[in] hold_mode_max The maximum acceptable number of Baseband slots to be in Hold Mode.
	 *	@param[in] hold_mode_min Minimum acceptable number of Baseband slots to be in Hold Mode.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool EnterHoldMode(WORD& _interval, WORD hold_mode_max, WORD hold_mode_min)
	{
		return m_Ret(BthEnterHoldMode(&m_btAddr, hold_mode_max, hold_mode_min, &_interval));
	}

	/**
	 * [�ݒ�] �p�[�N���[�h�J�n.
	 *	@note ACL �ڑ����Ă��Ȃ��ƃG���[�ɂȂ�܂��B
	 *	@param[out] _interval Baseband slots. 
	 *	@param[in] beacon_max The maximum acceptable number of Baseband slots between consecutive beacons. 
	 *	@param[in] beacon_min The minimum acceptable number of Baseband slots between consecutive beacons. 
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool EnterParkMode(WORD& _interval, WORD beacon_max, WORD beacon_min)
	{
		return m_Ret(BthEnterParkMode(&m_btAddr, beacon_max, beacon_min, &_interval));
	}

	/**
	 * [�ݒ�] �X�j�t���[�h�J�n.
	 *	@note ACL �ڑ����Ă��Ȃ��ƃG���[�ɂȂ�܂��B
	 *	@param[out] _interval Baseband slots. 
	 *	@param[in] sniff_mode_max The maximum acceptable number of Baseband slots between each sniff period. The time length is equal to N * 0.625 msec, or 1 Baseband slot. The valid range for N is 0x0001 to 0xFFFF. The valid time range is 0.625 msec to 40.9 seconds.
	 *	@param[in] sniff_mode_min The minimum acceptable number of Baseband slots between each sniff period. The time length is equal to N * 0.625 msec, or 1 Baseband slot. The valid range for N is 0x0001 to 0xFFFF. The valid time range is 0.625 msec to 40.9 seconds.
	 *	@param[in] sniff_attempt The number of Baseband receive slots for sniff attempt. The length is equal to (2 * N ?1) * 0.625 msec. The valid range for N is 0x0001 to 0x7FFF. The valid time range is 0.625 msec to 40.9 seconds.
	 *	@param[in] sniff_timeout number of Baseband receive slots for sniff timeout. If N is greater than 0, the length is equal to (2 * N ?1) * 0.625 msec. If N is equal to 0, then the length is equal to 0 msec. The valid range for N is 0x0000 to 0x7FFF. The valid time range is 0 msec to 40.9 seconds.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool EnterSniffMode(WORD& _interval, WORD sniff_mode_max, WORD sniff_mode_min, WORD sniff_attempt, WORD sniff_timeout)
	{
		return m_Ret(BthEnterSniffMode(&m_btAddr, sniff_mode_max, sniff_mode_min, sniff_attempt, sniff_timeout, &_interval));
	}

	/**
	 * [�ݒ�] �p�[�N���[�h�I��
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool ExitParkMode(void)
	{
		return m_Ret(BthExitParkMode(&m_btAddr));
	}

	/**
	 * [�ݒ�] �X�j�t���[�h�I��
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool ExitSniffMode(void)
	{
		return m_Ret(BthExitSniffMode(&m_btAddr));
	}

	/**
	 * [�擾] LINK�L�[�擾.
	 *	@note This function retrieves the link key for the Bluetooth device.
	 *	@param[out] _key �擾���� LINK�L�[ ���i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetLinkKey(CBluetoothLinkKey& _key) const
	{
		BT_ADDR ba = m_btAddr;
		return m_Ret(BthGetLinkKey(&ba, _key.Refer()));
	}

	/**
	 * [����] PIN ���N�G�X�g����.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool RefusePinRequest(void)
	{
		BT_ADDR ba = m_btAddr;
		return m_Ret(BthRefusePINRequest(&ba));
	}

	/**
	 * [����] �y�A���N�G�X�g.
	 *	@note This function requests the personal identification number (PIN) for a Bluetooth device. 
	 *	@param lpszPin PIN�R�[�h. NULL �̏ꍇ�A ��0 1byte ���R�[�h�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool PairRequest(LPCTSTR lpszPin)
	{
		CAscii pin = lpszPin;
		int len = pin.GetLength();
		if ( len == 0 )
		{
			BYTE b = 0;
			return m_Ret(BthPairRequest(&m_btAddr, 1, &b));
		}
		return m_Ret(BthPairRequest(&m_btAddr, len, reinterpret_cast<BYTE*>(pin.GetBuffer())));
	}

	/**
	 * [�j��] LINK�L�[�j��.
	 *	@note This function removes the link key for a Bluetooth device.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool RevokeLinkKey(void)
	{
		return m_Ret(BthRevokeLinkKey(&m_btAddr));
	}

	/**
	 * [�j��] PIN�R�[�h�j��.
	 *	@note This function revokes the personal identification number (PIN) for the Bluetooth device.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool RevokePin(void)
	{
		return m_Ret(BthRevokePIN(&m_btAddr));
	}

	/**
	 * [�ݒ�] �Í��ݒ�.
	 *	@note This function requests that the encryption mode on an existing connection is switched with the peer device.
	 *	@param isEnable true �Í����[�h�ŒʐM���܂��B false �Ȃ�Í������܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetEncryption(bool isEnable)
	{
		return m_Ret(BthSetEncryption(&m_btAddr, isEnable ? 1 : 0));
	}

	/**
	 * [�ݒ�] LINK�L�[�ݒ�.
	 *	@note This function stores the link key for the Bluetooth device.
	 *	@param key LINK�L�[.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetLinkKey(const CBluetoothLinkKey& key)
	{
		CBluetoothLinkKey k = key;
		return m_Ret(BthSetLinkKey(&m_btAddr, k.Refer()));
	}

	/**
	 * [�ݒ�] PIN �R�[�h�ݒ�.
	 *	@note This function stores the personal identification number (PIN) for the Bluetooth device.
	 *	@param lpszPin PIN�R�[�h. NULL �̏ꍇ�A ��0 1byte ���R�[�h�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetPin(LPCTSTR lpszPin)
	{
		CAscii pin = lpszPin;
		int len = pin.GetLength();
		if ( len == 0 )
		{
			BYTE b = 0;
			return m_Ret(BthSetPIN(&m_btAddr, 1, &b));
		}
		return m_Ret(BthSetPIN(&m_btAddr, len, reinterpret_cast<BYTE*>(pin.GetBuffer())));
	}

	/**
	 * [�m�F] �T�[�r�X�m�F.
	 *		�f�o�C�X���A�w��̃T�[�r�X�������Ă��邩�m�F�ł��܂��B
	 *	@note �ň�{@link CBluetoothLocal::GetPageTimeoutMs() �y�[�W�^�C���A�E�g����}������܂��B
	 *		�K�v�ɉ�����{@link CBluetoothLocal::SetPageTimeoutMs() �y�[�W�^�C���A�E�g���Ԃ̐ݒ�}���s���Ă��������B
	 *	@param[out] _raw �v���t�@�C���T�[�r�X�̐��f�[�^���i�[����܂��B
	 *	@param[in] uuid16 �m�F����T�[�r�X�� UUID16 �B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂�
	 *					(�T�[�r�X���Ȃ��ꍇ�A WSASERVICE_NOT_FOUND ���Ԃ�܂�)�B
	 */
	bool HasService(CWorkMem& _raw, WORD uuid16) const
	{
		SdpAttributeRange range = { 0, 0x200 };
		if ( m_QueryService(_raw, range, uuid16) )
		{
			return true;
		}
		return false;
	}

	/**
	 * [�m�F] �T�[�r�X�m�F.
	 *		�f�o�C�X���A�w��̃T�[�r�X�������Ă��邩�m�F�ł��܂��B
	 *	@note �ň�{@link CBluetoothLocal::GetPageTimeoutMs() �y�[�W�^�C���A�E�g����}������܂��B
	 *		�K�v�ɉ�����{@link CBluetoothLocal::SetPageTimeoutMs() �y�[�W�^�C���A�E�g���Ԃ̐ݒ�}���s���Ă��������B
	 *	@param[out] _attr �v���t�@�C���T�[�r�X�̑�����񂪊i�[����܂��B
	 *	@param[in] uuid16 �m�F����T�[�r�X�� UUID16 �B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂�
	 *					(�T�[�r�X���Ȃ��ꍇ�A WSASERVICE_NOT_FOUND ���Ԃ�܂�)�B
	 */
	bool HasService(CBluetoothServiceAttribute& _attr, WORD uuid16) const
	{
		CWorkMem sdp;
		if ( HasService(sdp, uuid16) )
		{
			_attr.Analyze(sdp.GetSize(), sdp.Ref());
			return true;
		}
		return false;
	}

	/**
	 * [�m�F] �T�[�r�X�m�F.
	 *		�f�o�C�X���A�w��̃T�[�r�X�������Ă��邩�m�F�ł��܂��B
	 *	@param[in] uuid16 �m�F����T�[�r�X�� UUID16 �B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂�
	 *					(�T�[�r�X���Ȃ��ꍇ�A WSASERVICE_NOT_FOUND ���Ԃ�܂�)�B
	 */
	bool HasService(WORD uuid16) const
	{
		CWorkMem sdp;
		return HasService(sdp, uuid16);
	}

#if 0
	/**
	 * [�擾] �T�[�r�X�ꗗ�擾.
	 *	@param[out] _list �T�[�r�X UUID16 �ꗗ���i�[����܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetServicesList(CWorkMemT<WORD>& _list) const
	{
		CTickCount tick;
		CWorkMemT<WORD> v(0xFF);
		INDEX d = 0;
		loop ( i, 0x30 )
		{
			if ( HasService(i + 0x1100) )
			{
				v[d++] = i + 0x1100;
			}
		}
		loop ( i, 0x20 )
		{
			if ( HasService(i + 0x1200) )
			{
				v[d++] = i + 0x1200;
			}
		}
		_list.Reset(d, v.Ref());
		tick.OutputDebugPassedCount();
		return true;
	}
#endif

	/**
	 * [�擾] �T�[�r�X�ꗗ�擾.
	 *		�x�[�X�ƂȂ�T�[�r�X���X�g�i�x�[�X���X�g�j�Ɋ܂܂��T�[�r�X���ŁA
	 *		�f�o�C�X�������Ă���T�[�r�X���ꗗ���܂��B
	 *	@note ��̃T�[�r�X�����ɍň�{@link CBluetoothLocal::GetPageTimeoutMs() �y�[�W�^�C���A�E�g����}������܂��B
	 *		�i�y�[�W�^�C���A�E�g���Ԃ��T�b�ɂȂ��Ă���ƁA�T��������̂ɁA�ň� 25�b������\��������܂��B�j
	 *		�K�v�ɉ�����{@link CBluetoothLocal::SetPageTimeoutMs() �y�[�W�^�C���A�E�g���Ԃ̐ݒ�}���s���Ă��������B
	 *		\code
	 *		CBluetoothLocal local;
	 *		CBluetoothDevice dev;
	 *			;
	 *		local.SetPageTimeoutMs(1000);	// 1�b�ɂ���
	 *		const static WORD awUuidList[] = {
	 *			PublicBrowseGroupServiceClassID_UUID16,          // (0x1002)
	 *					;
	 *			};
	 *		}
	 *		CWorkMemT<WORD> ww;
	 *		bool r = dev.GetServicesList(ww, countof(awUuidList), awUuidList);
	 *		local.SetPageTimeoutMs(5000);	// 5�b�ɂ���
	 *
	 *		\endcode
	 *	@param[out] _list �T�[�r�X UUID16 �ꗗ���i�[����܂��B
	 *	@param[in] baseListLength �x�[�X���X�g�̗v�f���B
	 *	@param[in] pBaseList �x�[�X���X�g�B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetServicesList(CWorkMemT<WORD>& _list, size_t baseListLength, const WORD* pBaseList) const
	{
		if ( baseListLength == 0 || pBaseList == NULL )
		{
//			return GetServicesList(_list);
			return false;
		}
		ASSERT ( ! ::IsBadReadPtr(pBaseList, sizeof(WORD) * baseListLength) );
		CWorkMemT<WORD> v(baseListLength);
		INDEX d = 0;
		loop ( i, baseListLength )
		{
			if ( HasService(pBaseList[i]) )
			{
				v[d++] = pBaseList[i];
			}
		}
		_list.Reset(d, v.Ref());
		return true;
	}

	/**
	 * [����] �R�l�N�V�����N���[�Y.
	 *	@param handle �n���h���B
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	static bool CloseConnection(BT_HANDLE handle)
	{
		CBluetoothApi api;
		int r = api.BthCloseConnection(handle);
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [�擾] �x�[�X�o���h�ڑ��ꗗ�擾.
	 *	@param[out] _connections �x�[�X�o���h�ڑ��ꗗ���i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂�
	 */
	static bool GetBasebandConnections(CWorkMemT<BASEBAND_CONNECTION>& _connections)
	{
		CBluetoothApi api;
		const int WORKLEN = 1000;
		CWorkMemT<BASEBAND_CONNECTION> w(WORKLEN);
		int len = 0;
		int r = api.BthGetBasebandConnections(WORKLEN, w.Ref(), &len);
		if ( r == ERROR_SUCCESS )
		{
			_connections.Reset(len, w.Ref());
			return true;
		}
		return false;
	}

	/**
	 * [�擾] �T�[�r�X���擾.
	 *	@param serviceUuid16 �T�[�r�X UUID16 �B SerialPortServiceClassID_UUID16 �ȂǁA
	 *			�`ServiceClassID_UUID16 �̃V���{�����w��B
	 *	@return �T�[�r�X��.
	 */
	static CStr ServiceToString(WORD serviceUuid16)
	{
		CStr s;
		switch ( serviceUuid16 )
		{
		case 0x0001:// SDP
			s = _T("SDP");
			break;
		case 0x0003:// RFCOMM
			s = _T("RFCOMM");
			break;
		case 0x0011:// HIDP
			s = _T("HIDP");
			break;
		case 0x0100:// L2CAP
			s = _T("L2CAP");
			break;
		case PublicBrowseGroupServiceClassID_UUID16:
			s = _T("Public Browse Group");
			break;
		case SerialPortServiceClassID_UUID16:
			s = _T("Serial Port");
			break;
		case LANAccessUsingPPPServiceClassID_UUID16:
			s = _T("LAN Access Using PPP");
			break;
		case DialupNetworkingServiceClassID_UUID16:
			s = _T("Dialup Net working");
			break;
		case IrMCSyncServiceClassID_UUID16:
			s = _T("IrMC Sync");
			break;
		case OBEXObjectPushServiceClassID_UUID16:
			s = _T("OBEX Object Push");
			break;
		case OBEXFileTransferServiceClassID_UUID16:
			s = _T("OBEX File Transfer");
			break;
		case IrMcSyncCommandServiceClassID_UUID16:
			s = _T("IrMc Sync Command");
			break;
		case HeadsetServiceClassID_UUID16:
			s = _T("Headset");
			break;
		case CordlessServiceClassID_UUID16:
			s = _T("Cordless");
			break;
		case AudioSourceServiceClassID_UUID16:
			s = _T("Audio Source");
			break;
		case AudioSinkServiceClassID_UUID16:
			s = _T("Audio Sink");
			break;
		case AV_RemoteControlTargetServiceClassID_UUID16:
			s = _T("A/V Remote Control Target");
			break;
		case AdvancedAudioDistributionServiceClassID_UUID16:
			s = _T("Advanced Audio Distribution");
			break;
		case AV_RemoteControlServiceClassID_UUID16:
			s = _T("A/V Remote Control");
			break;
		case VideoConferencingServiceClassID_UUID16:
			s = _T("Video Conferencing");
			break;
		case IntercomServiceClassID_UUID16:
			s = _T("Intercom");
			break;
		case FaxServiceClassID_UUID16:
			s = _T("Fax");
			break;
		case HeadsetAudioGatewayServiceClassID_UUID16:
			s = _T("Headset Audio Gateway");
			break;
		case PANUServiceClassID_UUID16:
			s = _T("PANU");
			break;
		case NAPServiceClassID_UUID16:
			s = _T("NAP");
			break;
		case GNServiceClassID_UUID16:
			s = _T("GN");
			break;
		case HandsfreeServiceClassID_UUID16:
			s = _T("Handsfree");
			break;
		case HandsfreeAudioGatewayServiceClassID_UUID16:
			s = _T("Handsfree Audio Gateway");
			break;
		case PnPInformationServiceClassID_UUID16:
			s = _T("PnP Information");
			break;
		case GenericNetworkingServiceClassID_UUID16:
			s = _T("Generic Networking");
			break;
		case GenericFileTransferServiceClassID_UUID16:
			s = _T("Generic File Transfer");
			break;
		case GenericAudioServiceClassID_UUID16:
			s = _T("Generic Audio");
			break;
		case GenericTelephonyServiceClassID_UUID16:
			s = _T("Generic Telephony");
			break;
		case 0x1113://WAP
			s = _T("WAP");
			break;
		case 0x1114://WAP_CLIENT
			s = _T("WAP Client");
			break;
		case 0x1118://DIRECT_PRINTING
			s = _T("Direct Printing");
			break;
		case 0x1119://REFERENCE_PRINTING
			s = _T("Reference Printing");
			break;
		case 0x111a://IMAGING
			s = _T("Imaging");
			break;
		case 0x111b://IMAGING_RESPONDER
			s = _T("Imaging Responder");
			break;
		case 0x111c://IMAGING_AUTOMATIC_ARCHIVE
			s = _T("Imaging Automatic Archive");
			break;
		case 0x111d://IMAGING_REFERENCE_OBJECTS
			s = _T("Imaging Reference Objects");
			break;
		case 0x1124://HID (HumanInterfaceDeviceService)
			s = _T("HID");
			break;
		case 0x112F://PBAP PSE (Phonebook Access Profile,Phonebook Server Equipment)
			s = _T("PBAP PSE");
			break;
		}
		return s;
	}

private:

	// �T�[�r�X�₢���킹
	bool m_QueryService(CWorkMem& _sdp, const SdpAttributeRange& range, WORD uuid16) const
	{
		// BLOB Data ����
		BTHNS_RESTRICTIONBLOB resb = { 0 };
		resb.type = SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST;
		resb.numRange = 1;
		resb.pRange[0] = range;
		resb.uuids[0].uuidType = SDP_ST_UUID16;
		resb.uuids[0].u.uuid16 = uuid16;
		// BLOB �ݒ�
		BLOB blob = { 0 };
		blob.cbSize = sizeof(BTHNS_RESTRICTIONBLOB);
		blob.pBlobData = reinterpret_cast<BYTE*>(&resb);
		// �A�h���X�ݒ�
		SOCKADDR_BTH sa = { 0 };
		sa.btAddr = m_btAddr;
		sa.addressFamily = AF_BT;
		CSADDR_INFO csai = { 0 };
		csai.RemoteAddr.lpSockaddr = reinterpret_cast<sockaddr*>(&sa);
		csai.RemoteAddr.iSockaddrLength = sizeof(sa);
		// QUERYSET�ݒ�
		WSAQUERYSET wsaq = { 0 };
		wsaq.dwSize = sizeof(wsaq);
		wsaq.dwNameSpace = NS_BTH;
		wsaq.lpBlob = &blob;
		wsaq.lpcsaBuffer = &csai;
		// �����J�n
		HANDLE hLookup = NULL;
		int ret = BthNsLookupServiceBegin(&wsaq, 0, &hLookup);
		if ( ret == ERROR_SUCCESS )
		{
			// �o�b�t�@�m��
			CWorkMem work(1000);
			work.Fill(0);
			DWORD dwSize = work.GetSize();
			// �f�o�C�X����
			WSAQUERYSET* pResults = reinterpret_cast<WSAQUERYSET*>(work.Ref());
			pResults->dwSize = sizeof(WSAQUERYSET);
			pResults->dwNameSpace = NS_BTH;
			int ret = BthNsLookupServiceNext(hLookup, 0, &dwSize, pResults);
			int ec = ::GetLastError();
			if ( ret != ERROR_SUCCESS && ec == ERROR_NOT_ENOUGH_MEMORY )
			{
				// ����Ȃ�����.�Ċm�ہ�����
				work.Resize(dwSize);
				work.Fill(0);
				pResults = reinterpret_cast<WSAQUERYSET*>(work.Ref());
				pResults->dwSize = sizeof(WSAQUERYSET);
				pResults->dwNameSpace = NS_BTH;
				ret = BthNsLookupServiceNext(hLookup, 0, &dwSize, pResults);
				ec = ::GetLastError();
			}
			BthNsLookupServiceEnd(hLookup);
			if ( ret == ERROR_SUCCESS )
			{
				if ( (pResults->lpBlob->cbSize) >= 4 )
				{
					_sdp.Reset(pResults->lpBlob->cbSize, pResults->lpBlob->pBlobData);
					return true;
				}
				::SetLastError(WSASERVICE_NOT_FOUND);
				return false;
			}
			::SetLastError(ec);
			_GetLastError("BthNsLookupServiceNext");
			return false;
		}
		_GetLastError("BthNsLookupServiceBegin");
		return false;
	}
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) �f�o�C�X����.
 *
 *		���ӂ̃f�o�C�X���������܂��B
 *
 *	@note �T�[�r�X�́A������A GetFoundDevice() �Ńf�o�C�X�N���X���擾���A
 *		CBluetoothDevice::GetServicesList() �����g�p���擾�ł��܂��B
 *
 *	@note CBluetoothLocal::Inquiry() �̂ق��������ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothFinder : CBluetoothApi
{
	DEFSUPER(CBluetoothApi);
public:

	/// �R���X�g���N�^
	CBluetoothFinder(void) : m_hLookup(NULL)
	{
	}

	/// �f�X�g���N�^
	~CBluetoothFinder(void)
	{
		Finish();
	}

	/**
	 * [�ݒ�] �t�B���^�ݒ�.
	 *	@note �w��̃A�h���X�������Ώۂɂ��܂��B
	 *	@param addr �A�h���X�B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetFilter(const CBluetoothAddress& addr)
	{
		BT_ADDR ba = addr;
		int r = BthSetInquiryFilter(&ba);
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [�ݒ�] �t�B���^�ݒ����.
	 *		SetFilter() �Ŏw�肵���t�B���^���������܂��B
	 *	@note ������̉����͏o���܂���B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool ResetFilter(void)
	{
		int r = BthClearInquiryFilter();
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [����] �����J�n.
	 *	@retval true �����B GetFound�`() ���\�b�h�Ŕ��������f�o�C�X��񂪓����܂��B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂�
	 *				�i�f�o�C�X��������Ȃ��ꍇ�A WSA_E_NO_MORE ���Ԃ�܂��j�B
	 */
	bool Start(void)
	{
		Finish();
		WSAQUERYSET wsaq = { 0 };
		wsaq.dwSize = sizeof(WSAQUERYSET);
		wsaq.dwNameSpace = NS_BTH;
		int ret = BthNsLookupServiceBegin(&wsaq, LUP_CONTAINERS, &m_hLookup);
		if ( ret != ERROR_SUCCESS )
		{
			m_hLookup = NULL;
			return false;
		}
		return Next();
	}

	/**
	 * [����] �������J
	 *	@retval true �����B GetFound�`() ���\�b�h�Ŕ��������f�o�C�X��񂪓����܂��B
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂�
	 *				�i�f�o�C�X��������Ȃ��ꍇ�A WSA_E_NO_MORE ���Ԃ�܂��j�B
	 */
	bool Next(void)
	{
		// �o�b�t�@�m��
		CWorkMem work(1000);
		work.Fill(0);
		DWORD dwSize = work.GetSize();
		// �f�o�C�X����
		DWORD flag = LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_BLOB;
		WSAQUERYSET* pResults = reinterpret_cast<WSAQUERYSET*>(work.Ref());
		pResults->dwSize = sizeof(WSAQUERYSET);
		pResults->dwNameSpace = NS_BTH;
		int ret = BthNsLookupServiceNext(m_hLookup, flag, &dwSize, pResults);
		if ( ret != ERROR_SUCCESS && ::GetLastError() == ERROR_NOT_ENOUGH_MEMORY )
		{
			// ����Ȃ�����.�Ċm�ہ�����
			work.Resize(dwSize);
			work.Fill(0);
			pResults = reinterpret_cast<WSAQUERYSET*>(work.Ref());
			pResults->dwSize = sizeof(WSAQUERYSET);
			pResults->dwNameSpace = NS_BTH;
			ret = BthNsLookupServiceNext(m_hLookup, flag, &dwSize, pResults);
		}
		if ( ret != ERROR_SUCCESS )
		{
			if ( ::GetLastError() != WSA_E_NO_MORE )
			{
				_GetLastError("BthNsLookupServiceNext");
			}
			return false;
		}
		BT_ADDR ba = 0;
		if ( pResults->dwNumberOfCsAddrs == 1 )
		{
			LPSOCKADDR lpszAddr = pResults->lpcsaBuffer->RemoteAddr.lpSockaddr;
			SOCKADDR_BTH* P = reinterpret_cast<SOCKADDR_BTH*>(lpszAddr);
			ba = P->btAddr;
		}
		ASSERT( pResults->lpBlob->cbSize == sizeof(BthInquiryResult) );
		BthInquiryResult* pInqRes = reinterpret_cast<BthInquiryResult*>(pResults->lpBlob->pBlobData);
		// ���L��
		m_foundDevice = ba;
		m_foundName = pResults->lpszServiceInstanceName;
		m_foundClass = pInqRes->cod;
		return m_foundDevice.IsValid();
	}

	/// [����] �����I��
	void Finish(void)
	{
		if ( m_hLookup != NULL )
		{
			BthNsLookupServiceEnd(m_hLookup);
			m_hLookup = NULL;
		}
	}

	/**
	 * [�擾] �����f�o�C�X�擾.
	 *	@note Start() , Next() �Ő������Ă��Ȃ��ꍇ�A�s��ȏ�񂪓����܂��B
	 *	@return �f�o�C�X.
	 */
	const CBluetoothDevice& GetFoundDevice(void) const
	{
		return m_foundDevice;
	}

	/**
	 * [�擾] �����f�o�C�X�N���X�擾.
	 *	@note Start() , Next() �Ő������Ă��Ȃ��ꍇ�A�s��ȏ�񂪓����܂��B
	 *	@return �f�o�C�X�N���X.
	 */
	const CBluetoothClass& GetFoundDeviceClass(void) const
	{
		return m_foundClass;
	}

	/**
	 * [�擾] �����f�o�C�X���擾.
	 *	@note Start() , Next() �Ő������Ă��Ȃ��ꍇ�A�s��ȏ�񂪓����܂��B
	 *	@return �f�o�C�X��.
	 */
	LPCTSTR GetFoundDeviceName(void) const
	{
		return m_foundName;
	}

private:
	HANDLE				m_hLookup;		///< �����n���h��
	CBluetoothDevice	m_foundDevice;	///< �����f�o�C�X
	CStr				m_foundName;	///< �����f�o�C�X��
	CBluetoothClass		m_foundClass;	///< �����f�o�C�X�̃N���X
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) Audio Gateway Service �Ǘ�.
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	�V�K�쐬
 */
class CBluetoothAudioGatewayService
{
public:

	/**
	 * [����] Closing Bluetooth control connection.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool Disconnect(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_CLOSE_CONTROL);
	}

	/**
	 * [����] Opening Bluetooth control connection.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool Connect(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_OPEN_CONTROL);
	}

	/**
	 * [����] Setting auto-answer option.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool AutoAnswer(void)
	{
		CBag ag;
		DWORD dwData = 1;
		return ag.Control(IOCTL_AG_SET_USE_HF_AUDIO, &dwData, sizeof(dwData));
	}

	/**
	 * [����] Opening Bluetooth audio.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool OpenAudio(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_OPEN_AUDIO);
	}

	/**
	 * [����] Closing Bluetooth audio.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool CloseAudio(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_CLOSE_AUDIO);
	}

	/**
	 * [�ݒ�] Turning on/off inband ring tones.
	 *	@param isOn true �Ȃ� on �A false �Ȃ� off.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetInbandRing(bool isOn = true)
	{
		CBag ag;
		DWORD dwData = isOn ? 1 : 0;
		return ag.Control(IOCTL_AG_SET_INBAND_RING, &dwData, sizeof(dwData));
	}

	/**
	 * [�ݒ�] Turning on/off Power-save mode.
	 *	@param isOn true �Ȃ� on �A false �Ȃ� off.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetPowerSaveMode(bool isOn = true)
	{
		CBag ag;
		DWORD dwData = isOn ? 1 : 0;
		return ag.Control(IOCTL_AG_SET_POWER_MODE, &dwData, sizeof(dwData));
	}

	/**
	 * [�擾] Getting Power-save mode.
	 *	@param[out] _isOn ���݂� Power-save ���[�h���i�[����܂��B true �Ȃ� on �A false �Ȃ� off.
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetPowerSaveMode(bool& _isOn)
	{
		CBag ag;
		DWORD dwData = 0;
		bool r = ag.Control(IOCTL_AG_GET_POWER_MODE, NULL, 0, &dwData, sizeof(dwData));
		_isOn = (dwData != 0);
		return r;
	}

	/**
	 * [�ݒ�] Setting speaker volume.
	 *	@param vol �{�����[���l�B
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetSpeakerVolume(int vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_SET_SPEAKER_VOL, &vol, sizeof(vol));
	}

	/**
	 * [�擾] Getting speaker volume.
	 *	@param[out] _vol ���݂̃{�����[���l���i�[����܂��B
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetSpeakerVolume(int& _vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_GET_SPEAKER_VOL, NULL, 0, &_vol, sizeof(_vol));
	}

	/**
	 * [�ݒ�] Setting microphone volume.
	 *	@param vol �{�����[���l�B
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool SetMicVolume(int vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_SET_MIC_VOL, &vol, sizeof(vol));
	}

	/**
	 * [�擾] Getting microphone volume.
	 *	@param[out] _vol ���݂̃{�����[���l���i�[����܂��B
	 *	@retval true ����.
	 *	@retval false ���s�B�ڍׂ́A ::GetLastError() �œ����܂��B
	 */
	bool GetMicVolume(int& _vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_GET_MIC_VOL, NULL, 0, &_vol, sizeof(_vol));
	}

private:
	/// BAG0: �Ǘ�
	class CBag
	{
	public:
		/// �R���X�g���N�^
		CBag(void) : m_handle(::CreateFile(_T("BAG0:"), 0, 0, NULL, OPEN_EXISTING, 0, NULL))
		{
		}
		/// �f�X�g���N�^
		~CBag(void)
		{
			if ( m_handle != INVALID_HANDLE_VALUE )
			{
				::CloseHandle(m_handle);
				m_handle = INVALID_HANDLE_VALUE;
			}
		}
		/// �R���g���[��
		bool Control(DWORD dwIoControlCode, LPVOID lpInBuffer = NULL, DWORD nInBufferSize = 0, LPVOID lpOutBuffer = NULL, DWORD nOutBufferSize = 0, LPDWORD lpBytesReturned = NULL, LPOVERLAPPED lpOverlapped = NULL)
		{
			bool r = !! ::DeviceIoControl(m_handle, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
			if ( ! r )
			{
				_GetLastError("DeviceIoControl");
			}
			return r;
		}
	private:
		HANDLE m_handle; ///< �n���h��
	};
};



#if 0

	CE �� SDP �́A������������ł��Ȃ��B

#endif

};//TNB
