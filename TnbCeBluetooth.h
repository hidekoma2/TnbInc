#pragma once
/**
 *@file
 * CE 用 Bluetooth関係のヘッダ
 *
 *	@note Winsock2 を使うので、マクロ "_TnbWINSOCK2_ENABLE" を宣言しておきます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCeBluetoothApi.h"
#include "TnbStrVector.h"
#include "TnbRegistryAccessor.h"
#include "TnbTickCount.h"



//TNB Library
namespace TNB{



#ifdef _TnbDOXYGEN	//Document作成用シンボル
/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) アドレス.
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
typedef ULONGLONG BT_ADDR;

/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) ハンドル.
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
typedef unsigned short BT_HANDLE;
#else 
 #define BT_HANDLE unsigned short
#endif



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) リンクキー管理.
 *
 *		16BYTE のリンクキーを管理しています。
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothLinkKey
{
public:

	/// コンストラクタ.
	CBluetoothLinkKey(void)
	{
		Zero(m_key);
	}

	/**
	 * [参照] アドレス参照.
	 *	@return リンクキーアドレス.
	 */
	BYTE* Refer(void)
	{
		return m_key;
	}

	/**
	 * [参照] アドレス参照.
	 *	@return リンクキーアドレス.
	 */
	const BYTE* Refer(void) const
	{
		return m_key;
	}

	/**
	 * [参照] 要素参照.
	 *	@param index インデックス。 0～15 を指定できます。
	 *	@return 参照.
	 */
	BYTE& operator[](INDEX index)
	{
		ASSERT( index < 16 );
		return m_key[index];
	}

	/**
	 * [参照] 要素参照.
	 *	@param index インデックス。 0～15 を指定できます。
	 *	@return 参照.
	 */
	const BYTE& operator[](INDEX index) const
	{
		ASSERT( index < 16 );
		return m_key[index];
	}

private:
	BYTE	m_key[16]; ///< リンクキー
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) デバイスクラス管理.
 *
 *		一つの COD (class of device) を管理します。
 *		サービスクラス、メジャーデバイスクラス、マイナーデバイスクラスの取得が可能です。
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothClass
{
public:

	/** 
	 * サービスクラス種類.
	 *	@note bit単位で意味を成します。
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
	 * メジャーデバイスクラス種類.
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
	 * コンストラクタ.
	 *	@param cod デバイスクラス。省略すると 0 が指定されます。
	 */
	CBluetoothClass(DWORD cod = 0)
	{
		m_class.cod = cod;
	}

	/**
	 * [取得] デバイスクラス取得.
	 *	@return デバイスクラス
	 */
	operator DWORD(void) const
	{
		return m_class.cod;
	}

	/**
	 * [取得] デバイスクラス取得.
	 *	@return デバイスクラス
	 */
	DWORD GetDeviceClass(void) const
	{
		return m_class.cod;
	}

	/**
	 * [取得] サービスクラス取得.
	 *	@return サービスクラス。 EServiceClass を or したものです。
	 */
	DWORD GetServiceClass(void) const
	{
		return m_class.serviceClass;
	}

	/**
	 * [設定] サービスクラス設定.
	 *	@param c サービスクラス。 EServiceClass を or したものです。
	 */
	void SetServiceClass(DWORD c)
	{
		m_class.serviceClass = c;
	}

	/**
	 * [取得] メジャーデバイスクラス取得.
	 *	@return メジャーデバイスクラス。通常、 EMajorDeviceClass の値です。
	 */
	DWORD GetMajorDeviceClass(void) const
	{
		return m_class.majorDeviceClass;
	}

	/**
	 * [設定] メジャーデバイスクラス設定.
	 *	@param c メジャーデバイスクラス。通常、 EMajorDeviceClass の値です。
	 */
	void SetMajorDeviceClass(DWORD c)
	{
		m_class.majorDeviceClass = c;
	}

	/**
	 * [取得] メジャーデバイスクラス文字列取得.
	 *	@return メジャーデバイスクラス文字列.
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
	 * [取得] マイナーデバイスクラス取得.
	 *	@return マイナーデバイスクラス。
	 */
	DWORD GetMinorDeviceClass(void) const
	{
		return m_class.minorDeviceClass;
	}

	/**
	 * [設定] マイナーデバイスクラス設定.
	 *	@param c マイナーデバイスクラス。
	 */
	void SetMinorDeviceClass(DWORD c)
	{
		m_class.minorDeviceClass = c;
	}

	/**
	 * [取得] マイナーデバイスクラス文字列取得.
	 *	@return マイナーデバイスクラス文字列.
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
	 * [取得] Computerデバイスクラス文字列取得.
	 *	@return 文字列.
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
	 * [取得] Phoneデバイスクラス文字列取得.
	 *	@return 文字列.
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
	 * [取得] AVデバイスクラス文字列取得.
	 *	@return 文字列.
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
	 * [取得] Peripheralデバイスクラス文字列取得.
	 *	@return 文字列.
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
	 * [取得] Imagingデバイスクラス文字列取得.
	 *	@return 文字列.
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
		DWORD		cod;	///< デバイスクラス
		struct
		{
			DWORD	formatType		:2;		///< フォーマットタイプ
			DWORD	minorDeviceClass:6;		///< マイナーデバイスクラス
			DWORD	majorDeviceClass:5;		///< メジャーデバイスクラス
			DWORD	serviceClass	:11;	///< サービスクラス
			DWORD	padding			:8;		///< (空)
		};
	} m_class;	///< クラス
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) サービス属性管理.
 *
 *		SDPの生データを解析、管理します。
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothServiceAttribute : CCopyImpossible
{
public:

	/**@ingroup CEBLUETOOTH
	 * Bluetooth(CE) サービス属性値管理.
	 *
	 *	@par必要ファイル
	 *			TnbCeBluetooth.h
	 * 
	 *	@date 09/10/01	新規作成
	 */
	struct TValue
	{
		SDP_TYPE			type;	///< タイプ
		SDP_SPECIFICTYPE	spec;	///< 詳細タイプ
		union
		{
			SDP_LARGE_INTEGER_16	int128;		///< spec = SDP_ST_INT128 の場合の値
			SDP_ULARGE_INTEGER_16	uint128;	///< spec = SDP_ST_UINT128 の場合の値
			GUID					uuid128;	///< spec = SDP_ST_UUID128 の場合の値
			DWORD					uuid32;		///< spec = SDP_ST_UUID32 の場合の値
			WORD					uuid16;		///< spec = SDP_ST_UUID16 の場合の値
			LONGLONG				int64;		///< spec = SDP_ST_INT64 の場合の値
			ULONGLONG				uint64;		///< spec = SDP_ST_UINT64 の場合の値
			LONG					int32;		///< spec = SDP_ST_INT32 の場合の値
			DWORD					uint32;		///< spec = SDP_ST_UINT32 の場合の値
			SHORT					int16;		///< spec = SDP_ST_INT16 の場合の値
			WORD					uint16;		///< spec = SDP_ST_UINT16 の場合の値
			CHAR					int8;		///< spec = SDP_ST_INT8 の場合の値
			BYTE					uint8;		///< spec = SDP_ST_UINT8 の場合の値
			bool					booleanVal;	///< type = SDP_TYPE_BOOLEAN / SDP_TYPE_STRING の場合の値
		};
		CAscii						str;		///< type = SDP_TYPE_URL の場合の値

		/**
		 * コンストラクタ.
		 *	@param t タイプ
		 *	@param s 詳細
		 */
		TValue(SDP_TYPE t = SDP_TYPE_NIL, SDP_SPECIFICTYPE s = SDP_ST_NONE) : type(t), spec(s)
		{
			Zero(int128);
		}

		/**
		 * [取得] 文字列化.
		 *	@param[out] pType タイプ文字列が格納されます。いらない場合 NULL を指定します。
		 *	@return 文字列.
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

	/// ID配列.
	typedef CVectorT<WORD> TIdsList;
	/// 値配列.
	typedef CVectorT<TValue> TValuesList;


	//----------------------------


	/// コンストラクタ
	CBluetoothServiceAttribute(void) : m_ppSdpRecords(NULL), m_numRecords(0), m_lastResult(S_OK)
	{
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	}
	
	/// デストラクタ
	~CBluetoothServiceAttribute(void)
	{
		Free();
	}

	/**
	 * [設定] 破棄.
	 *		保持している情報をすべて破棄します。
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
	 * [確認] 有効確認.
	 *		Analyze() で成功したら本クラスが保持しているデータが有効になります。
	 *	@retval true 有効.
	 *	@retval false 無効.
	 */
	bool IsValid(void) const
	{
		return m_ppSdpRecords != NULL;
	}

	/**
	 * [取得] 保持レコード数.
	 *	@return レコード数.
	 */
	size_t GetRecordCount(void) const
	{
		return m_numRecords;
	}

	/**
	 * [取得] 最終リザルト取得.
	 *	@return リザルト.
	 */
	HRESULT GetLastResult(void) const
	{
		return m_lastResult;
	}

	/**
	 * [解析] 解析.
	 *		生データ解析.
	 *	@param size 生データ長.
	 *	@param lpRawData 生データ.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は GetLastResult() で得られます。
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
	 * [検索] レコード検索.
	 *		指定レコードを探し、内容をメンバに記憶します。
	 *	@note Analyze() 内で SearchRecord(0) が実行されています。 レコード０以外を扱う時に、
	 *			本メソッドを使います。
	 *	@param record レコード番号。 0 から GetRecordCount() 未満の値を指定できます。
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は GetLastResult() で得られます。
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
	 * [確認] 属性データ確認.
	 *	@param attributeId 属性ID。
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool HasAttribute(WORD attributeId) const
	{
		return m_attrIds.Find(attributeId) != INVALID_INDEX;
	}

	/**
	 * [取得] 属性ID一覧取得.
	 *	@return 属性ID一覧.
	 */
	const TIdsList& GetAttributeIdList(void) const
	{
		return m_attrIds;
	}

	/**
	 * [取得] 属性IDデータ取得.
	 *	@param attributeId 属性ID。
	 *		<table>
	 *		<tr>
	 *			<td>属性ID</td>	<td>シンボル</td>
	 *			<td>情報内容</td>
	 *		</tr><tr>
	 *			<td> 0x0000 </td> <td> SDP_ATTRIB_RECORD_HANDLE </td>
	 *			<td> Record Handle </td>
	 *		</tr><tr>
	 *			<td> 0x0001 </td> <td> SDP_ATTRIB_CLASS_ID_LIST </td>
	 *			<td> Class ID List </td>
	 *		</tr><tr>
	 *			<td> 0x0004 </td> <td> SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST </td>
	 *			<td> Protocol Descriptor List.\n UUID16 = 3 の直後の UINT8 は RFCOMM Channel ID。 </td>
	 *		</tr><tr>
	 *			<td> 0x0005 </td> <td> SDP_ATTRIB_BROWSE_GROUP_LIST </td>
	 *			<td> Browse Group List </td>
	 *		</tr><tr>
	 *			<td> 0x0006 </td> <td> SDP_ATTRIB_LANG_BASE_ATTRIB_ID_LIST </td>
	 *			<td> Language Base\n 順に Language, Character Encoding, Base Attribute ID </td>
	 *		</tr><tr>
	 *			<td> 0x0009 </td> <td> SDP_ATTRIB_PROFILE_DESCRIPTOR_LIST </td>
	 *			<td> Profile Descriptor List\n UINT16は Version。 </td>
	 *		</tr>
 	 *		</table>
	 *	@return データ.
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
	 * [取得] 属性文字列取得.
	 *	@note 属性内の文字列を抽出します。
	 *	@return 文字列群.
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
	TIdsList				m_attrIds;			///< 検索したID一覧
	CVectorT<TValuesList>	m_attrDetails;		///< 検索した属性データ
	TValuesList				m_dummy;			///< 失敗時の応答用.
	ISdpRecord**			m_ppSdpRecords;		///< array of pSdpRecords
	ULONG					m_numRecords;		///< number of elements in pSdpRecords
	HRESULT					m_lastResult;		///< ラストリザルト
	/// レコードチェック
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
	/// レコードチェック(UINT)
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
	/// レコードチェック(INT)
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
	/// レコードチェック(UUID)
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
	/// レコードチェックサブ
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
	/// ロード
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
	/// リザルト管理
	bool m_Res(HRESULT hr)
	{
		m_lastResult = hr;
		return SUCCEEDED(hr);
	}
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) アドレス管理.
 *
 *	各 Bluetooth 対応デバイスを識別するときに使用される 48 ビット アドレスを、一つ管理しています。
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothAddress
{
public:

	/**
	 * コンストラクタ.
	 *	@param btAddr Bluetoothアドレス.
	 */
	CBluetoothAddress(BT_ADDR btAddr = 0) : m_btAddr(btAddr)
	{
	}

	/**
	 * コピーコンストラクタ.
	 *	@param other コピー元.
	 */
	CBluetoothAddress(const CBluetoothAddress& other)
	{
		m_btAddr = other.m_btAddr;
	}

	/**
	 * コピーオペレータ.
	 *	@param other コピー元.
	 *	@return 自分の参照.
	 */
	CBluetoothAddress& operator=(const CBluetoothAddress& other)
	{
		m_btAddr = other.m_btAddr;
		return *this;
	}

	/**
	 * [確認] 有効確認.
	 *	@note 保持しているアドレスが 0 なら、無効とします。
	 *	@retval true 有効.
	 *	@retval false 無効.
	 */
	bool IsValid(void) const
	{
		return m_btAddr != 0;
	}

	/**
	 * [取得] アドレス取得.
	 *	@return アドレス.
	 */
	operator const BT_ADDR&(void) const
	{
		return m_btAddr;
	}

	/**
	 * [取得] アドレス取得.
	 *	@return アドレス.
	 */
	const BT_ADDR& GetAddress(void) const
	{
		return m_btAddr;
	}

	/**
	 * [取得] アドレス文字列取得.
	 *		保持しているアドレスを xx:xx:xx:xx:xx:xx と言う文字列にします。
	 *	@return アドレス文字列.
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
	 * [設定] アドレス文字列設定.
	 *		アドレスを xx:xx:xx:xx:xx:xx と言う文字列で指定します。
	 *	@param lpszAddress アドレス
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
	BT_ADDR		m_btAddr;		///< アドレス

private:
	// int化
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
 * Bluetooth(CE) ローカル管理.
 *
 *		自分自身のデバイスを管理.
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothLocal : private CBluetoothApi
{
	DEFSUPER(CBluetoothApi);
public:

	/// コンストラクタ
	CBluetoothLocal(void)
	{
	}

	/**
	 * [取得] アドレス取得.
	 *	@param[out] _addr 取得したアドレスが格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [取得] 名前取得.
	 *	@retval Empty状態 エラー。詳細は、 ::GetLastError() で得られます。
	 *	@retval それ以外 成功。内容は名前。
	 */
	CStr GetName(void) const
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, KEY_QUERY_VALUE);
		return reg[_T("Ident")].QueryString(_T("Name"));
	}

	/**
	 * [設定] 名前設定.
	 *	@param lpszName 設定する名前
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetName(LPCTSTR lpszName)
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE);
		return reg[_T("Ident")].WriteString(_T("Name"), lpszName);
	}

	/**
	 * [取得] デバイスクラス取得.
	 *	@param[out] _class 取得したデバイスクラス
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool GetClass(CBluetoothClass& _class) const
	{
		UINT cl = 0;
		bool r = m_Ret(BthReadCOD(&cl));
		_class = cl;
		return r;
	}

	/**
	 * [設定] デバイスクラス設定.
	 *	@param clas デバイスクラス
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetClass(const CBluetoothClass& clas)
	{
		UINT cl = clas.GetDeviceClass();
		return m_Ret(BthWriteCOD(cl));
	}

	/**
	 * [設定] 認証有効無効.
	 *	@param isEnable ture なら認証を有効、 false なら無効にします。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool EnableAuthentication(bool isEnable)
	{
		return m_Ret(BthWriteAuthenticationEnable(isEnable ? 1 : 0));
	}

	/**
	 * [取得] 認証状態取得.
	 *	@param[out] _isEnabled 認証状態が格納されます。 ture なら認証を有効、 false なら無効を表します。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	
	/// モード
	enum EMode
	{
		PowerOff			= -1,	///< Power OFF。
		DisabledAll			= 0,	///< Power ON。Inquiry は Disable、 Page Scan は Disable。
		EnabledInquiry		= 1,	///< Power ON。Inquiry は Enable、  Page Scan は Disable。
		EnabledPageScan		= 2,	///< Power ON。Inquiry は Disable、 Page Scan は Enable。
		EnabledAll			= 3,	///< Power ON。Inquiry は Enable、  Page Scan は Enable。
	};

	/**
	 * [取得] モード取得.
	 *	@param[out] _mode モードが格納されます。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [設定] モード設定.
	 *	@note OFFからONにする場合、時間がかかることがあります。その最大待ち時間を指定できます。
	 *	@param mode モード。
	 *	@param timeout OFF->ONのタイムアウト時間(ms)。時間内に OFF->ONにならなかった時、 GetLastError() は ERROR_TIMEOUT を返します。
	 *	@retval true 設定成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [取得] ページタイムアウト時間取得.
	 *	@param[out] _timeout 時間が格納される。 数値 x 0.625ms が実時間
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [設定] ページタイムアウト時間設定.
	 *	@param timeout 時間。 数値 x 0.625ms が実時間。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetPageTimeout(DWORD timeout)
	{
		WORD ot = static_cast<WORD>(timeout);
		return m_Ret(BthWritePageTimeout(ot));
	}

	/**
	 * [取得] ページタイムアウト時間取得.
	 *	@note 時間をミリ秒で扱いますが、誤差がでます。
	 *	@param[out] _timeout 時間が格納される。単位はミリ秒。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [設定] ページタイムアウト時間設定.
	 *	@note 時間をミリ秒で扱いますが、誤差がでます。
	 *	@param timeout 時間。単位はミリ秒。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetPageTimeoutMs(DWORD timeout)
	{
		double d = static_cast<double>(timeout);
		return SetPageTimeout(static_cast<DWORD>(d / .625));
	}

	/**
	 * [取得] 状態取得.
	 *		装置の状態を取得します。
	 *	@retval HCI_HARDWARE_RUNNING The adapter is installed and the stack is running. 
	 *	@retval HCI_HARDWARE_INITIALIZING  The adapter might be installed. 
	 *										The stack is currently on the way up. Call again later.
	 *	@retval HCI_HARDWARE_ERROR The adapter might be installed. 
	 *								The stack is on the way down. Call again later.
	 *	@retval HCI_HARDWARE_SHUTDOWN The adapter is installed, but the stack is not running. 
	 *	@retval HCI_HARDWARE_NOT_PRESENT The adapter is not present. 
	 *	@retval HCI_HARDWARE_UNKNOWN  The stack is not present. 詳細は、 ::GetLastError() で得られます。
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
	 * [設定] アイドル接続切断.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool TerminateIdleConnections(void)
	{
		return m_Ret(BthTerminateIdleConnections());
	}

	/**
	 * [設定] Inquiryフィルタ設定.
	 *		指定のアドレスを検索対象にします。
	 *	@note 複数個対象にしたい場合、本メソッドを数回使います。
	 *	@param addr アドレス。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetInquiryFilter(const CBluetoothAddress& addr)
	{
		BT_ADDR ba = addr;
		return m_Ret(BthSetInquiryFilter(&ba));
	}

	/**
	 * [設定] Inquiryフィルタ設定解除.
	 *		SetInquiryFilter() での指定を解除します。
	 *	@note 一つだけの解除は出来ません。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool ClearInquiryFilter(void)
	{
		return m_Ret(BthClearInquiryFilter());
	}

	/**
	 * [検索] Inquiry実行.
	 *		周りのデバイスを検索します。
	 *	@param timeout 検索時間(単位；ms)。精度は 1280ms です。最大は 60秒です。
	 *	@return アドレス一覧. Invalid 状態ならエラー。
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
 * Bluetooth(CE) デバイス管理.
 *
 *		一つのアドレスを管理.
 *		そのアドレスのデバイスのサービスなど取得可能。
 *
 *	@note 一部のメソッドは CreateScoConnection() などで、
 *		ベースバンド接続していないと、失敗します（ エラーで ERROR_NOT_FOUND が返る）。
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 *	@date 10/01/07	PINコードの解釈を改善。
 */
class CBluetoothDevice : public CBluetoothAddress, private CBluetoothApi
{
	DEFSUPER(CBluetoothAddress);
public:

	/**
	 * コンストラクタ.
	 *	@param btAddr Bluetoothアドレス.
	 */
	CBluetoothDevice(const BT_ADDR& btAddr = 0) : _super(btAddr)
	{
	}

	/**
	 * コンストラクタ.
	 *	@param btHandle Bluetoothハンドル.
	 */
	CBluetoothDevice(BT_HANDLE btHandle)
	{
		if ( BthGetAddress(btHandle, &m_btAddr) != ERROR_SUCCESS )
		{
			m_btAddr = 0;
		}
	}

	/**
	 * コピーコンストラクタ.
	 *	@param other コピー元.
	 */
	CBluetoothDevice(const CBluetoothDevice& other)
	{
		m_btAddr = other.m_btAddr;
	}

	/**
	 * コピーオペレータ.
	 *	@param other コピー元.
	 *	@return 自分の参照.
	 */
	CBluetoothDevice& operator=(const CBluetoothDevice& other)
	{
		m_btAddr = other.m_btAddr;
		return *this;
	}

	/// Bluetooth(CE) デバイスのバージョン管理
	struct TVersion
	{
		BYTE version;		///< バージョン
		WORD subVersion;	///< サブバージョン
		WORD manufacturer;	///< メーカー
		BYTE features;		///< features
	};

	/**
	 * [取得] バージョン取得.
	 *	@param[out] _version バージョン情報が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [確認] 存在確認.
	 *	@note GetName() を使って確認しています。
	 *	@retval true 存在。
	 *	@retval false いない。詳細は、 ::GetLastError() で得られます。
	 */
	bool IsExist(void) const
	{
		return ! GetName().IsEmpty();
	}

	/**
	 * [取得] デバイスクラス取得.
	 *	@note baseband 接続されていない場合、失敗します。
	 *	@param[out] _class
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [取得] デバイスクラス取得.
	 *	@note baseband 接続されていない場合、一度接続します。
	 *	@param[out] _class
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [取得] デバイス名取得.
	 *	@retval Empty文字列 失敗。詳細は、 ::GetLastError() で得られます。
	 *	@retval 上記以外 成功。文字列はデバイス名。
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
	 * [処理] 認証.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool Authenticate(void) const
	{
		BT_ADDR ba = m_btAddr;
		return m_Ret(BthAuthenticate(&ba));
	}

	/**
	 * [処理] 音声用パケットオープン.
	 *		SCO(Synchronous Connection Oriented) 接続します。
	 *	@note SCO の特徴
	 *		- マスタと一つのスレーブの 1vs1 の通信
	 *		- 通信速度を保障。
	 *		- パケットの再送はサポートしない
	 *		- リアルタイム性あり
	 *	@retval NULL以外 成功。 使用後、 CloseConnection() で切断してください。
	 *	@retval NULL 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [処理] データ通信用パケットオープン.
	 *		ACL(Asynchronous Connectionless) 接続します。
	 *	@note ACL の特徴
	 *		- マスタと複数のスレーブの 1vs多 の通信
	 *		- 速度を保障しない
	 *		- パケットの再送をサポート
	 *		- データ通信に利用
	 *	@retval NULL以外 成功。 使用後、 CloseConnection() で切断じてください。
	 *	@retval NULL 失敗。詳細は、 ::GetLastError() で得られます。
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

	/// ACL モード
	enum EMode
	{
		ACTIVE,	///< アクティブモード.
		HOLD,	///< ホールドモード.	@note リンクを中断させる。ピコネット内の同期は維持される。
		SNIFF,	///< スニフモード.		@note スレーブ特有。スニフ同期（定義した一定間隔）でパケットの送受信が可能。
		PARK	///< パークモード.		@note スレーブ特有の省電力モード。マスタからのパケットは受信可能。送信不可能。
	};

	/**
	 * [取得] .ACLモード取得.
	 *	@note ACL 接続していないとエラーになります。
	 *	@param[out] _mode 取得したモードが格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [設定] ホールドモード開始.
	 *	@note ACL 接続していないとエラーになります。
	 *	@param[out] _interval Baseband slots. 
	 *	@param[in] hold_mode_max The maximum acceptable number of Baseband slots to be in Hold Mode.
	 *	@param[in] hold_mode_min Minimum acceptable number of Baseband slots to be in Hold Mode.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool EnterHoldMode(WORD& _interval, WORD hold_mode_max, WORD hold_mode_min)
	{
		return m_Ret(BthEnterHoldMode(&m_btAddr, hold_mode_max, hold_mode_min, &_interval));
	}

	/**
	 * [設定] パークモード開始.
	 *	@note ACL 接続していないとエラーになります。
	 *	@param[out] _interval Baseband slots. 
	 *	@param[in] beacon_max The maximum acceptable number of Baseband slots between consecutive beacons. 
	 *	@param[in] beacon_min The minimum acceptable number of Baseband slots between consecutive beacons. 
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool EnterParkMode(WORD& _interval, WORD beacon_max, WORD beacon_min)
	{
		return m_Ret(BthEnterParkMode(&m_btAddr, beacon_max, beacon_min, &_interval));
	}

	/**
	 * [設定] スニフモード開始.
	 *	@note ACL 接続していないとエラーになります。
	 *	@param[out] _interval Baseband slots. 
	 *	@param[in] sniff_mode_max The maximum acceptable number of Baseband slots between each sniff period. The time length is equal to N * 0.625 msec, or 1 Baseband slot. The valid range for N is 0x0001 to 0xFFFF. The valid time range is 0.625 msec to 40.9 seconds.
	 *	@param[in] sniff_mode_min The minimum acceptable number of Baseband slots between each sniff period. The time length is equal to N * 0.625 msec, or 1 Baseband slot. The valid range for N is 0x0001 to 0xFFFF. The valid time range is 0.625 msec to 40.9 seconds.
	 *	@param[in] sniff_attempt The number of Baseband receive slots for sniff attempt. The length is equal to (2 * N ?1) * 0.625 msec. The valid range for N is 0x0001 to 0x7FFF. The valid time range is 0.625 msec to 40.9 seconds.
	 *	@param[in] sniff_timeout number of Baseband receive slots for sniff timeout. If N is greater than 0, the length is equal to (2 * N ?1) * 0.625 msec. If N is equal to 0, then the length is equal to 0 msec. The valid range for N is 0x0000 to 0x7FFF. The valid time range is 0 msec to 40.9 seconds.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool EnterSniffMode(WORD& _interval, WORD sniff_mode_max, WORD sniff_mode_min, WORD sniff_attempt, WORD sniff_timeout)
	{
		return m_Ret(BthEnterSniffMode(&m_btAddr, sniff_mode_max, sniff_mode_min, sniff_attempt, sniff_timeout, &_interval));
	}

	/**
	 * [設定] パークモード終了
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool ExitParkMode(void)
	{
		return m_Ret(BthExitParkMode(&m_btAddr));
	}

	/**
	 * [設定] スニフモード終了
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool ExitSniffMode(void)
	{
		return m_Ret(BthExitSniffMode(&m_btAddr));
	}

	/**
	 * [取得] LINKキー取得.
	 *	@note This function retrieves the link key for the Bluetooth device.
	 *	@param[out] _key 取得した LINKキー が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool GetLinkKey(CBluetoothLinkKey& _key) const
	{
		BT_ADDR ba = m_btAddr;
		return m_Ret(BthGetLinkKey(&ba, _key.Refer()));
	}

	/**
	 * [処理] PIN リクエスト辞退.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool RefusePinRequest(void)
	{
		BT_ADDR ba = m_btAddr;
		return m_Ret(BthRefusePINRequest(&ba));
	}

	/**
	 * [処理] ペアリクエスト.
	 *	@note This function requests the personal identification number (PIN) for a Bluetooth device. 
	 *	@param lpszPin PINコード. NULL の場合、 ￥0 1byte をコードとします。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [破棄] LINKキー破棄.
	 *	@note This function removes the link key for a Bluetooth device.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool RevokeLinkKey(void)
	{
		return m_Ret(BthRevokeLinkKey(&m_btAddr));
	}

	/**
	 * [破棄] PINコード破棄.
	 *	@note This function revokes the personal identification number (PIN) for the Bluetooth device.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool RevokePin(void)
	{
		return m_Ret(BthRevokePIN(&m_btAddr));
	}

	/**
	 * [設定] 暗号設定.
	 *	@note This function requests that the encryption mode on an existing connection is switched with the peer device.
	 *	@param isEnable true 暗号モードで通信します。 false なら暗号化しません。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetEncryption(bool isEnable)
	{
		return m_Ret(BthSetEncryption(&m_btAddr, isEnable ? 1 : 0));
	}

	/**
	 * [設定] LINKキー設定.
	 *	@note This function stores the link key for the Bluetooth device.
	 *	@param key LINKキー.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetLinkKey(const CBluetoothLinkKey& key)
	{
		CBluetoothLinkKey k = key;
		return m_Ret(BthSetLinkKey(&m_btAddr, k.Refer()));
	}

	/**
	 * [設定] PIN コード設定.
	 *	@note This function stores the personal identification number (PIN) for the Bluetooth device.
	 *	@param lpszPin PINコード. NULL の場合、 ￥0 1byte をコードとします。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [確認] サービス確認.
	 *		デバイスが、指定のサービスを持っているか確認できます。
	 *	@note 最悪{@link CBluetoothLocal::GetPageTimeoutMs() ページタイムアウト時間}かかります。
	 *		必要に応じて{@link CBluetoothLocal::SetPageTimeoutMs() ページタイムアウト時間の設定}を行ってください。
	 *	@param[out] _raw プロファイルサービスの生データが格納されます。
	 *	@param[in] uuid16 確認するサービスの UUID16 。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます
	 *					(サービスがない場合、 WSASERVICE_NOT_FOUND が返ります)。
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
	 * [確認] サービス確認.
	 *		デバイスが、指定のサービスを持っているか確認できます。
	 *	@note 最悪{@link CBluetoothLocal::GetPageTimeoutMs() ページタイムアウト時間}かかります。
	 *		必要に応じて{@link CBluetoothLocal::SetPageTimeoutMs() ページタイムアウト時間の設定}を行ってください。
	 *	@param[out] _attr プロファイルサービスの属性情報が格納されます。
	 *	@param[in] uuid16 確認するサービスの UUID16 。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます
	 *					(サービスがない場合、 WSASERVICE_NOT_FOUND が返ります)。
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
	 * [確認] サービス確認.
	 *		デバイスが、指定のサービスを持っているか確認できます。
	 *	@param[in] uuid16 確認するサービスの UUID16 。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます
	 *					(サービスがない場合、 WSASERVICE_NOT_FOUND が返ります)。
	 */
	bool HasService(WORD uuid16) const
	{
		CWorkMem sdp;
		return HasService(sdp, uuid16);
	}

#if 0
	/**
	 * [取得] サービス一覧取得.
	 *	@param[out] _list サービス UUID16 一覧が格納されます。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [取得] サービス一覧取得.
	 *		ベースとなるサービスリスト（ベースリスト）に含まれるサービス中で、
	 *		デバイスが持っているサービスを一覧します。
	 *	@note 一つのサービス検索に最悪{@link CBluetoothLocal::GetPageTimeoutMs() ページタイムアウト時間}かかります。
	 *		（ページタイムアウト時間が５秒になっていると、５つ検索するのに、最悪 25秒かかる可能性があります。）
	 *		必要に応じて{@link CBluetoothLocal::SetPageTimeoutMs() ページタイムアウト時間の設定}を行ってください。
	 *		\code
	 *		CBluetoothLocal local;
	 *		CBluetoothDevice dev;
	 *			;
	 *		local.SetPageTimeoutMs(1000);	// 1秒にする
	 *		const static WORD awUuidList[] = {
	 *			PublicBrowseGroupServiceClassID_UUID16,          // (0x1002)
	 *					;
	 *			};
	 *		}
	 *		CWorkMemT<WORD> ww;
	 *		bool r = dev.GetServicesList(ww, countof(awUuidList), awUuidList);
	 *		local.SetPageTimeoutMs(5000);	// 5秒にする
	 *
	 *		\endcode
	 *	@param[out] _list サービス UUID16 一覧が格納されます。
	 *	@param[in] baseListLength ベースリストの要素数。
	 *	@param[in] pBaseList ベースリスト。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [処理] コネクションクローズ.
	 *	@param handle ハンドル。
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	static bool CloseConnection(BT_HANDLE handle)
	{
		CBluetoothApi api;
		int r = api.BthCloseConnection(handle);
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [取得] ベースバンド接続一覧取得.
	 *	@param[out] _connections ベースバンド接続一覧が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます
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
	 * [取得] サービス名取得.
	 *	@param serviceUuid16 サービス UUID16 。 SerialPortServiceClassID_UUID16 など、
	 *			～ServiceClassID_UUID16 のシンボルを指定。
	 *	@return サービス名.
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

	// サービス問い合わせ
	bool m_QueryService(CWorkMem& _sdp, const SdpAttributeRange& range, WORD uuid16) const
	{
		// BLOB Data 準備
		BTHNS_RESTRICTIONBLOB resb = { 0 };
		resb.type = SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST;
		resb.numRange = 1;
		resb.pRange[0] = range;
		resb.uuids[0].uuidType = SDP_ST_UUID16;
		resb.uuids[0].u.uuid16 = uuid16;
		// BLOB 設定
		BLOB blob = { 0 };
		blob.cbSize = sizeof(BTHNS_RESTRICTIONBLOB);
		blob.pBlobData = reinterpret_cast<BYTE*>(&resb);
		// アドレス設定
		SOCKADDR_BTH sa = { 0 };
		sa.btAddr = m_btAddr;
		sa.addressFamily = AF_BT;
		CSADDR_INFO csai = { 0 };
		csai.RemoteAddr.lpSockaddr = reinterpret_cast<sockaddr*>(&sa);
		csai.RemoteAddr.iSockaddrLength = sizeof(sa);
		// QUERYSET設定
		WSAQUERYSET wsaq = { 0 };
		wsaq.dwSize = sizeof(wsaq);
		wsaq.dwNameSpace = NS_BTH;
		wsaq.lpBlob = &blob;
		wsaq.lpcsaBuffer = &csai;
		// 検索開始
		HANDLE hLookup = NULL;
		int ret = BthNsLookupServiceBegin(&wsaq, 0, &hLookup);
		if ( ret == ERROR_SUCCESS )
		{
			// バッファ確保
			CWorkMem work(1000);
			work.Fill(0);
			DWORD dwSize = work.GetSize();
			// デバイス検索
			WSAQUERYSET* pResults = reinterpret_cast<WSAQUERYSET*>(work.Ref());
			pResults->dwSize = sizeof(WSAQUERYSET);
			pResults->dwNameSpace = NS_BTH;
			int ret = BthNsLookupServiceNext(hLookup, 0, &dwSize, pResults);
			int ec = ::GetLastError();
			if ( ret != ERROR_SUCCESS && ec == ERROR_NOT_ENOUGH_MEMORY )
			{
				// 足りなかった.再確保＆検索
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
 * Bluetooth(CE) デバイス検索.
 *
 *		周辺のデバイスを検索します。
 *
 *	@note サービスは、発見後、 GetFoundDevice() でデバイスクラスを取得し、
 *		CBluetoothDevice::GetServicesList() 等を使用し取得できます。
 *
 *	@note CBluetoothLocal::Inquiry() のほうが高速です。
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothFinder : CBluetoothApi
{
	DEFSUPER(CBluetoothApi);
public:

	/// コンストラクタ
	CBluetoothFinder(void) : m_hLookup(NULL)
	{
	}

	/// デストラクタ
	~CBluetoothFinder(void)
	{
		Finish();
	}

	/**
	 * [設定] フィルタ設定.
	 *	@note 指定のアドレスを検索対象にします。
	 *	@param addr アドレス。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetFilter(const CBluetoothAddress& addr)
	{
		BT_ADDR ba = addr;
		int r = BthSetInquiryFilter(&ba);
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [設定] フィルタ設定解除.
	 *		SetFilter() で指定したフィルタを解除します。
	 *	@note 一つだけの解除は出来ません。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool ResetFilter(void)
	{
		int r = BthClearInquiryFilter();
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [検索] 検索開始.
	 *	@retval true 成功。 GetFound～() メソッドで発見したデバイス情報が得られます。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます
	 *				（デバイスが見つからない場合、 WSA_E_NO_MORE が返ります）。
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
	 * [検索] 次検索開
	 *	@retval true 成功。 GetFound～() メソッドで発見したデバイス情報が得られます。
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます
	 *				（デバイスが見つからない場合、 WSA_E_NO_MORE が返ります）。
	 */
	bool Next(void)
	{
		// バッファ確保
		CWorkMem work(1000);
		work.Fill(0);
		DWORD dwSize = work.GetSize();
		// デバイス検索
		DWORD flag = LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_BLOB;
		WSAQUERYSET* pResults = reinterpret_cast<WSAQUERYSET*>(work.Ref());
		pResults->dwSize = sizeof(WSAQUERYSET);
		pResults->dwNameSpace = NS_BTH;
		int ret = BthNsLookupServiceNext(m_hLookup, flag, &dwSize, pResults);
		if ( ret != ERROR_SUCCESS && ::GetLastError() == ERROR_NOT_ENOUGH_MEMORY )
		{
			// 足りなかった.再確保＆検索
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
		// 情報記憶
		m_foundDevice = ba;
		m_foundName = pResults->lpszServiceInstanceName;
		m_foundClass = pInqRes->cod;
		return m_foundDevice.IsValid();
	}

	/// [検索] 検索終了
	void Finish(void)
	{
		if ( m_hLookup != NULL )
		{
			BthNsLookupServiceEnd(m_hLookup);
			m_hLookup = NULL;
		}
	}

	/**
	 * [取得] 発見デバイス取得.
	 *	@note Start() , Next() で成功していない場合、不定な情報が得られます。
	 *	@return デバイス.
	 */
	const CBluetoothDevice& GetFoundDevice(void) const
	{
		return m_foundDevice;
	}

	/**
	 * [取得] 発見デバイスクラス取得.
	 *	@note Start() , Next() で成功していない場合、不定な情報が得られます。
	 *	@return デバイスクラス.
	 */
	const CBluetoothClass& GetFoundDeviceClass(void) const
	{
		return m_foundClass;
	}

	/**
	 * [取得] 発見デバイス名取得.
	 *	@note Start() , Next() で成功していない場合、不定な情報が得られます。
	 *	@return デバイス名.
	 */
	LPCTSTR GetFoundDeviceName(void) const
	{
		return m_foundName;
	}

private:
	HANDLE				m_hLookup;		///< 検索ハンドル
	CBluetoothDevice	m_foundDevice;	///< 発見デバイス
	CStr				m_foundName;	///< 発見デバイス名
	CBluetoothClass		m_foundClass;	///< 発見デバイスのクラス
};



/**@ingroup CEBLUETOOTH
 * Bluetooth(CE) Audio Gateway Service 管理.
 *
 *	@par必要ファイル
 *			TnbCeBluetooth.h
 * 
 *	@date 09/10/01	新規作成
 */
class CBluetoothAudioGatewayService
{
public:

	/**
	 * [処理] Closing Bluetooth control connection.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool Disconnect(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_CLOSE_CONTROL);
	}

	/**
	 * [処理] Opening Bluetooth control connection.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool Connect(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_OPEN_CONTROL);
	}

	/**
	 * [処理] Setting auto-answer option.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool AutoAnswer(void)
	{
		CBag ag;
		DWORD dwData = 1;
		return ag.Control(IOCTL_AG_SET_USE_HF_AUDIO, &dwData, sizeof(dwData));
	}

	/**
	 * [処理] Opening Bluetooth audio.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool OpenAudio(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_OPEN_AUDIO);
	}

	/**
	 * [処理] Closing Bluetooth audio.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool CloseAudio(void)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_CLOSE_AUDIO);
	}

	/**
	 * [設定] Turning on/off inband ring tones.
	 *	@param isOn true なら on 、 false なら off.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetInbandRing(bool isOn = true)
	{
		CBag ag;
		DWORD dwData = isOn ? 1 : 0;
		return ag.Control(IOCTL_AG_SET_INBAND_RING, &dwData, sizeof(dwData));
	}

	/**
	 * [設定] Turning on/off Power-save mode.
	 *	@param isOn true なら on 、 false なら off.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetPowerSaveMode(bool isOn = true)
	{
		CBag ag;
		DWORD dwData = isOn ? 1 : 0;
		return ag.Control(IOCTL_AG_SET_POWER_MODE, &dwData, sizeof(dwData));
	}

	/**
	 * [取得] Getting Power-save mode.
	 *	@param[out] _isOn 現在の Power-save モードが格納されます。 true なら on 、 false なら off.
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
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
	 * [設定] Setting speaker volume.
	 *	@param vol ボリューム値。
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetSpeakerVolume(int vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_SET_SPEAKER_VOL, &vol, sizeof(vol));
	}

	/**
	 * [取得] Getting speaker volume.
	 *	@param[out] _vol 現在のボリューム値が格納されます。
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool GetSpeakerVolume(int& _vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_GET_SPEAKER_VOL, NULL, 0, &_vol, sizeof(_vol));
	}

	/**
	 * [設定] Setting microphone volume.
	 *	@param vol ボリューム値。
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool SetMicVolume(int vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_SET_MIC_VOL, &vol, sizeof(vol));
	}

	/**
	 * [取得] Getting microphone volume.
	 *	@param[out] _vol 現在のボリューム値が格納されます。
	 *	@retval true 成功.
	 *	@retval false 失敗。詳細は、 ::GetLastError() で得られます。
	 */
	bool GetMicVolume(int& _vol)
	{
		CBag ag;
		return ag.Control(IOCTL_AG_GET_MIC_VOL, NULL, 0, &_vol, sizeof(_vol));
	}

private:
	/// BAG0: 管理
	class CBag
	{
	public:
		/// コンストラクタ
		CBag(void) : m_handle(::CreateFile(_T("BAG0:"), 0, 0, NULL, OPEN_EXISTING, 0, NULL))
		{
		}
		/// デストラクタ
		~CBag(void)
		{
			if ( m_handle != INVALID_HANDLE_VALUE )
			{
				::CloseHandle(m_handle);
				m_handle = INVALID_HANDLE_VALUE;
			}
		}
		/// コントロール
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
		HANDLE m_handle; ///< ハンドル
	};
};



#if 0

	CE の SDP は、一個ずつしか検索できない。

#endif

};//TNB
