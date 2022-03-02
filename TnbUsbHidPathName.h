#pragma once
/**
 *@file
 * USB HIDのパスネーム情報管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbRegistryAccessor.h"
#include "TnbWindowsVersion.h"

#ifndef _TnbDOXYGEN	//Document作成用シンボル

#pragma pack(push,4)

// HIDUSAGE.H より
#ifndef __HIDUSAGE_H__

	typedef USHORT USAGE, *PUSAGE;

#endif // __HIDUSAGE_H__

// HIDPI.H より
#ifndef __HIDPI_H__

	typedef enum _HIDP_REPORT_TYPE
	{
		HidP_Input,
		HidP_Output,
		HidP_Feature
	} HIDP_REPORT_TYPE;

	typedef struct _HIDP_CAPS
	{
		USAGE	Usage;
		USAGE	UsagePage;
		USHORT	InputReportByteLength;
		USHORT	OutputReportByteLength;
		USHORT	FeatureReportByteLength;
		USHORT	Reserved[17];

		USHORT	NumberLinkCollectionNodes;

		USHORT	NumberInputButtonCaps;
		USHORT	NumberInputValueCaps;
		USHORT	NumberInputDataIndices;

		USHORT	NumberOutputButtonCaps;
		USHORT	NumberOutputValueCaps;
		USHORT	NumberOutputDataIndices;

		USHORT	NumberFeatureButtonCaps;
		USHORT	NumberFeatureValueCaps;
		USHORT	NumberFeatureDataIndices;
	} HIDP_CAPS, *PHIDP_CAPS;

	typedef struct _HIDP_LINK_COLLECTION_NODE
	{
		USAGE    LinkUsage;
		USAGE    LinkUsagePage;
		USHORT   Parent;
		USHORT   NumberOfChildren;
		USHORT   NextSibling;
		USHORT   FirstChild;
		ULONG    CollectionType: 8;  // As defined in 6.2.2.6 of HID spec
		ULONG    IsAlias : 1; // This link node is an allias of the next link node.
		ULONG    Reserved: 23;
		PVOID    UserContext; // The user can hang his coat here.
	} HIDP_LINK_COLLECTION_NODE, *PHIDP_LINK_COLLECTION_NODE;

	typedef struct _HIDP_BUTTON_CAPS
	{
		USAGE    UsagePage;
		UCHAR    ReportID;
		BOOLEAN  IsAlias;

		USHORT   BitField;
		USHORT   LinkCollection;   // A unique internal index pointer

		USAGE    LinkUsage;
		USAGE    LinkUsagePage;

		BOOLEAN  IsRange;
		BOOLEAN  IsStringRange;
		BOOLEAN  IsDesignatorRange;
		BOOLEAN  IsAbsolute;

		ULONG    Reserved[10];
		union {
			struct {
				USAGE    UsageMin,         UsageMax;
				USHORT   StringMin,        StringMax;
				USHORT   DesignatorMin,    DesignatorMax;
				USHORT   DataIndexMin,     DataIndexMax;
			} Range;
			struct  {
				USAGE    Usage,            Reserved1;
				USHORT   StringIndex,      Reserved2;
				USHORT   DesignatorIndex,  Reserved3;
				USHORT   DataIndex,        Reserved4;
			} NotRange;
		};

	} HIDP_BUTTON_CAPS, *PHIDP_BUTTON_CAPS;

	typedef struct _HIDP_VALUE_CAPS
	{
		USAGE    UsagePage;
		UCHAR    ReportID;
		BOOLEAN  IsAlias;

		USHORT   BitField;
		USHORT   LinkCollection;   // A unique internal index pointer

		USAGE    LinkUsage;
		USAGE    LinkUsagePage;

		BOOLEAN  IsRange;
		BOOLEAN  IsStringRange;
		BOOLEAN  IsDesignatorRange;
		BOOLEAN  IsAbsolute;

		BOOLEAN  HasNull;        // Does this channel have a null report   union
		UCHAR    Reserved;
		USHORT   BitSize;        // How many bits are devoted to this value?

		USHORT   ReportCount;    // See Note below.  Usually set to 1.
		USHORT   Reserved2[5];

		ULONG    UnitsExp;
		ULONG    Units;

		LONG     LogicalMin,       LogicalMax;
		LONG     PhysicalMin,      PhysicalMax;

		union {
			struct {
				USAGE    UsageMin,         UsageMax;
				USHORT   StringMin,        StringMax;
				USHORT   DesignatorMin,    DesignatorMax;
				USHORT   DataIndexMin,     DataIndexMax;
			} Range;

			struct {
				USAGE    Usage,            Reserved1;
				USHORT   StringIndex,      Reserved2;
				USHORT   DesignatorIndex,  Reserved3;
				USHORT   DataIndex,        Reserved4;
			} NotRange;
		};
	} HIDP_VALUE_CAPS, *PHIDP_VALUE_CAPS;

	typedef PUCHAR  PHIDP_REPORT_DESCRIPTOR;
	typedef struct _HIDP_PREPARSED_DATA * PHIDP_PREPARSED_DATA;
	#ifndef FACILITY_HID_ERROR_CODE
	 #define FACILITY_HID_ERROR_CODE 0x11
	#endif
	#define HIDP_ERROR_CODES(SEV, CODE) \
			((NTSTATUS) (((SEV) << 28) | (FACILITY_HID_ERROR_CODE << 16) | (CODE)))
	#define HIDP_STATUS_SUCCESS                  (HIDP_ERROR_CODES(0x0,0))

#endif // __HIDPI_H__

//HIDSDI.h より
#ifndef _HIDSDI_H

	typedef struct _HIDD_ATTRIBUTES 
	{
		ULONG   Size; // = sizeof (struct _HIDD_ATTRIBUTES)
		// Vendor ids of this hid device
		USHORT  VendorID;
		USHORT  ProductID;
		USHORT  VersionNumber;
		// Additional fields will be added to the end of this structure.
	} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;
	typedef LONG NTSTATUS;

#endif // _HIDSDI_H

#pragma pack(pop)

#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup USB
 * hid.dll アクセスクラス
 *
 *		hid.dll を動的にリンクし、そのメソッドをアクセスできます。
 *		これにより、 DDK(WDK)がなくても、 HID 関係の API を使用することが出来ます。
 *
 *		hid.dll が持っているメソッドと同じものを持っています。
 *
 *	@par必要ファイル
 *			TnbUsbHidPathName.h
 * 
 *	@date 10/02/04 新規作成
 *	@date 10/03/17 GetString系メソッド等、追加。　
 *	@date 13/01/08 HidD_SetOutputReport(), HidD_GetInputReport(), 追加。
 */
class CUsbHidApi
{
public:

	/// コンストラクタ
	CUsbHidApi(void) : m_module(NULL)
	{
	}
	
	/**
	 * コピーコンストラクタ.
	 *	@note 何もコピーしません
	 *	@param other コピー元
	 */
	CUsbHidApi(const CUsbHidApi& other) : m_module(NULL)
	{
	}

	/// デストラクタ
	~CUsbHidApi(void)
	{
		if ( m_module != NULL )
		{
			::FreeLibrary(m_module);
			m_module = NULL;
		}
	}

	/**
	 * コピーオペレータ
	 *	@note 何もコピーしません
	 *	@param other コピー元.
	 *	@return 自分の参照
	 */
	CUsbHidApi& operator=(const CUsbHidApi& other)
	{
		return *this;
	}

	/**
	 * [取得] 属性取得.
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] pAttributes 取得した属性が格納されます.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetAttributes(HANDLE hidDeviceObject, PHIDD_ATTRIBUTES pAttributes) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PHIDD_ATTRIBUTES pAttributes) = 0;
		return Func(P, "HidD_GetAttributes")(hidDeviceObject, pAttributes);
	}

	/**
	 * [取得] PREPARSEDデータ取得.
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] ppPreparsedData 取得した PREPARSEDデータが格納されているポインタが格納されます.
	 *					使用後は必ず、 HidD_FreePreparsedData() で開放してください。
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetPreparsedData(HANDLE hidDeviceObject, PHIDP_PREPARSED_DATA* ppPreparsedData) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PHIDP_PREPARSED_DATA* ppPreparsedData) = 0;
		return Func(P, "HidD_GetPreparsedData")(hidDeviceObject, ppPreparsedData);
	}

	/**
	 * [開放] PREPARSEDデータ開放.
	 *		HidD_GetPreparsedData() で取得したデータの開放を行います。
	 *	@param[in] pPreparsedData HidD_GetPreparsedData() で獲得した PREPARSEDデータのポインタ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_FreePreparsedData(PHIDP_PREPARSED_DATA pPreparsedData) const
	{
		BOOLEAN (__stdcall *P)(PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidD_FreePreparsedData")(pPreparsedData);
	}

	/**
	 * [送信] 機能レポート送信
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[in] pReportBuffer レポート内容.
	 *	@param[in] reportBufferLength レポート長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_SetFeature(HANDLE hidDeviceObject,PVOID pReportBuffer, ULONG reportBufferLength)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject,PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_SetFeature")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}

	/**
	 * [受信] 機能レポート受信
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] pReportBuffer 受信したレポート内容が格納されるバッファです.
	 *	@param[in] reportBufferLength 受信バッファ長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetFeature(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_GetFeature")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}

	/**
	 * [送信] Outputレポート送信
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[in] pReportBuffer レポート内容.
	 *	@param[in] reportBufferLength レポート長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_SetOutputReport(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_SetOutputReport")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}

	/**
	 * [受信] Inputレポート受信
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] pReportBuffer 受信したレポート内容が格納されるバッファです.
	 *	@param[in] reportBufferLength 受信バッファ長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetInputReport(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_GetInputReport")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}
		
	/**
	 * [取得] HID GUID 取得.
	 *	@param[out] pHidGuid 取得した GUID が格納されます。
	 */
	void HidD_GetHidGuid(LPGUID pHidGuid) const
	{
		void (__stdcall *P)(LPGUID pHidGuid) = NULL;
		Func(P, "HidD_GetHidGuid")(pHidGuid);
	}

	/**
	 * [取得] Manufacturer 文字列取得
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] pBuffer 受信した文字列が格納されるバッファです.
	 *	@param[in] bufferLength バッファ長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetManufacturerString(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetManufacturerString")(hidDeviceObject, pBuffer, bufferLength);
	}

	/**
	 * [取得] Product 文字列取得
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] pBuffer 受信した文字列が格納されるバッファです.
	 *	@param[in] bufferLength バッファ長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetProductString(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetProductString")(hidDeviceObject, pBuffer, bufferLength);
	}

	/**
	 * [取得] SerialNumber 文字列取得
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] pBuffer 受信した文字列が格納されるバッファです.
	 *	@param[in] bufferLength バッファ長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetSerialNumberString(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetSerialNumberString")(hidDeviceObject, pBuffer, bufferLength);
	}

	/**
	 * [取得] INDEXED 文字列取得
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[in] stringIndex インデックス
	 *	@param[out] pBuffer 受信した文字列が格納されるバッファです.
	 *	@param[in] bufferLength バッファ長さ.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetIndexedString(HANDLE hidDeviceObject, ULONG stringIndex, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, ULONG stringIndex, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetIndexedString")(hidDeviceObject, stringIndex, pBuffer, bufferLength);
	}

	/**
	 * [取得] 入力バッファ数取得.
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param[out] pNumberBuffers 入力バッファ数が格納されるバッファです.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_GetNumInputBuffers(HANDLE hidDeviceObject, PULONG pNumberBuffers) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PULONG pNumberBuffers) = 0;
		return Func(P, "HidD_GetNumInputBuffers")(hidDeviceObject, pNumberBuffers);
	}

	/**
	 * [設定] 入力バッファ数設定.
	 *	@param hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@param numberBuffers 入力バッファ数.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_SetNumInputBuffers(HANDLE hidDeviceObject, ULONG numberBuffers)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, ULONG numberBuffers) = 0;
		return Func(P, "HidD_SetNumInputBuffers")(hidDeviceObject, numberBuffers);
	}

	/**
	 * [処理] キューフラッシュ.
	 *	@param[in] hidDeviceObject HIDデバイスをオープンしたハンドル.
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOLEAN HidD_FlushQueue(HANDLE hidDeviceObject)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject) = 0;
		return Func(P, "HidD_FlushQueue")(hidDeviceObject);
	}
	
	/**
	 * [取得] 能力取得.
	 *	@param[in] pPreparsedData PREPARSEDデータ.
	 *	@param[out] pCapabilities 取得した能力データが格納されます。
	 *	@retval HIDP_STATUS_SUCCESS 成功.
	 *	@retval 上記以外 失敗.
	 */
	NTSTATUS HidP_GetCaps(PHIDP_PREPARSED_DATA pPreparsedData, PHIDP_CAPS pCapabilities) const
	{
		NTSTATUS (__stdcall *P)(PHIDP_PREPARSED_DATA pPreparsedData, PHIDP_CAPS pCapabilities) = 0;
		return Func(P, "HidP_GetCaps")(pPreparsedData, pCapabilities);
	}
	
	/**
	 * [取得] リンクコレクションノード取得.
	 *	@param[out] pLinkCollectionNodes ノード情報の配列
	 *	@param[in,out] pLinkCollectionNodesLength in でノード情報の配列数。 out で取得した数が返ります。
	 *	@param[in] pPreparsedData PREPARSEDデータ.
	 *	@retval HIDP_STATUS_SUCCESS 成功.
	 *	@retval 上記以外 失敗.
	 */
	NTSTATUS HidP_GetLinkCollectionNodes(PHIDP_LINK_COLLECTION_NODE pLinkCollectionNodes, PULONG pLinkCollectionNodesLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		NTSTATUS (__stdcall *P)(PHIDP_LINK_COLLECTION_NODE pLinkCollectionNodes, PULONG pLinkCollectionNodesLength, PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidP_GetLinkCollectionNodes")(pLinkCollectionNodes, pLinkCollectionNodesLength, pPreparsedData);
	}

	/**
	 * [取得] ボタン詳細性能取得.
	 *	@param[in] reportType レポートタイプ
	 *	@param[in] usagePage USAGE PAGE
	 *	@param[in] linkCollection LINK COLLECTION 
	 *	@param[in] usage USAGE
	 *	@param[out] pButtonCaps ボタン性能情報の配列
	 *	@param[in,out] pButtonCapsLength in でボタン性能情報の配列数。 out で取得した数が返ります。
	 *	@param[in] pPreparsedData PREPARSEDデータ.
	 *	@retval HIDP_STATUS_SUCCESS 成功.
	 *	@retval 上記以外 失敗.
	 */
	NTSTATUS HidP_GetSpecificButtonCaps(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_BUTTON_CAPS pButtonCaps, PUSHORT pButtonCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		NTSTATUS (__stdcall *P)(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_BUTTON_CAPS pButtonCaps, PUSHORT pButtonCapsLength, PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidP_GetSpecificButtonCaps")(reportType, usagePage, linkCollection, usage, pButtonCaps, pButtonCapsLength, pPreparsedData);
	}

	/**
	 * [取得] ボタン性能取得.
	 *	@param[in] reportType レポートタイプ
	 *	@param[out] pButtonCaps ボタン性能情報の配列
	 *	@param[in,out] pButtonCapsLength in でボタン性能情報の配列数。 out で取得した数が返ります。
	 *	@param[in] pPreparsedData PREPARSEDデータ.
	 *	@retval HIDP_STATUS_SUCCESS 成功.
	 *	@retval 上記以外 失敗.
	 */
	NTSTATUS HidP_GetButtonCaps(HIDP_REPORT_TYPE reportType, PHIDP_BUTTON_CAPS pButtonCaps, PUSHORT pButtonCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		return HidP_GetSpecificButtonCaps(reportType, 0, 0, 0, pButtonCaps, pButtonCapsLength, pPreparsedData);
	}

	/**
	 * [取得] 値詳細性能取得.
	 *	@param[in] reportType レポートタイプ
	 *	@param[in] usagePage USAGE PAGE
	 *	@param[in] linkCollection LINK COLLECTION 
	 *	@param[in] usage USAGE
	 *	@param[out] pValueCaps 値性能情報の配列
	 *	@param[in,out] pValueCapsLength in で値性能情報の配列数。 out で取得した数が返ります。
	 *	@param[in] pPreparsedData PREPARSEDデータ.
	 *	@retval HIDP_STATUS_SUCCESS 成功.
	 *	@retval 上記以外 失敗.
	 */
	NTSTATUS HidP_GetSpecificValueCaps(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_VALUE_CAPS pValueCaps, PUSHORT pValueCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		NTSTATUS (__stdcall *P)(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_VALUE_CAPS pValueCaps, PUSHORT pValueCapsLength, PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidP_GetSpecificValueCaps")(reportType, usagePage, linkCollection, usage, pValueCaps, pValueCapsLength, pPreparsedData);
	}

	/**
	 * [取得] 値性能取得.
	 *	@param[in] reportType レポートタイプ
	 *	@param[out] pValueCaps 値性能情報の配列
	 *	@param[in,out] pValueCapsLength in で値性能情報の配列数。 out で取得した数が返ります。
	 *	@param[in] pPreparsedData PREPARSEDデータ.
	 *	@retval HIDP_STATUS_SUCCESS 成功.
	 *	@retval 上記以外 失敗.
	 */
	NTSTATUS HidP_GetValueCaps(HIDP_REPORT_TYPE reportType, PHIDP_VALUE_CAPS pValueCaps, PUSHORT pValueCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		return HidP_GetSpecificValueCaps(reportType, 0, 0, 0, pValueCaps, pValueCapsLength, pPreparsedData);
	}

private:

	// EXPORT関数コール
	template<typename TYP>
	TYP Func(TYP typ, LPCSTR lpsz) const
	{
		if ( m_module == NULL )
		{
			m_module = ::LoadLibrary(_T("hid.dll"));
			ASSERT( m_module != NULL );
		}
		ForceSet(typ, ::GetProcAddress(m_module, lpsz));
		ASSERT1( typ != NULL, "CUsbHidApi::Func", "[%s]が見つかりませんでした", lpsz );
		return typ;
	}

	mutable HMODULE	m_module;	///< hid.dll のハンドル
};



/**@ingroup USB
 * USB-HIDパスネーム情報管理クラス
 *
 *		USB-HID のデバイスを一意に示すパスネーム(文字列)と、それに対応する
 *		Vendor-ID,Product-IDとパスネームとCapabiliryを管理。
 *
 *		存在しない USB-HIDのデバイスのパスネームは管理できません。
 *
 *	@see CUsbHid 
 *
 *	@note 複製可能
 *
 *	@note 「パスネーム」とは、実際にその HIDデバイスをオープンするために使用します。
 *
 *	@par必要ファイル
 *			TnbUsbHidPathName.h
 * 
 *	@date 07/04/26 新規作成
 *	@date 10/02/04 DDK がなくても使えるように宣言を内包した。
 *	@date 10/02/23 GetManufacturer() , GetDeviceDescription() など、取得メソッド追加。
 *	@date 11/01/19 ToString() を追加。
 */
class CUsbHidPathName : CUsbHidApi
{
public:

	/**
	 * コンストラクタ
	 *	@note Empty 状態のクラスを作成します。
	 */
	CUsbHidPathName(void) : m_vendorId(0), m_productId(0), m_pathName()
	{
		Zero(m_hidpCaps);
	}

	/**
	 * コンストラクタ
	 *	@note 指定の情報を持ったクラスを作成します。
	 *	@param wVid Vendor ID。
	 *	@param wPid Product ID。
	 *	@param lpszPathName パスネーム。
	 *	@param cap Capability情報
	 */
	CUsbHidPathName(WORD wVid, WORD wPid, LPCTSTR lpszPathName, const HIDP_CAPS& cap)
		: m_vendorId(wVid), m_productId(wPid), m_pathName(lpszPathName), m_hidpCaps(cap)
	{
	}

	/**
	 * [確認] 情報有無確認
	 *	@retval true 空っぽ
	 *	@retval false 情報あり
	 */
	bool IsEmpty(void) const
	{
		return m_pathName.IsEmpty();
//		return m_vendorId != 0;
	}

	/**
	 * [設定] 情報なし
	 *	@note 本インスタンスの情報をクリアし、情報無し状態にします。
	 */
	void Empty(void)
	{
		m_vendorId = 0;
		m_productId = 0;
		m_pathName.Empty();
		m_regPathHid.Empty();
		m_regPathUsb.Empty();
		Zero(m_hidpCaps);
	}

	/**
	 * [設定] パスネーム設定.
	 *		指定のパスネームを使い、USB-HIDデバイスをアクセスし、 
	 *		Vendor ID,Product ID,Capability を取得し、本インスタンスに設定します。
	 *	@note パスネームだけの設定ではありません。
	 *	@param lpszPathName パスネーム。
	 *	@retval ERROR_SUCCESS 成功。本インスタンスに情報が格納されています。
	 *	@retval 上記以外 失敗。戻り値はエラーコード。本インスタンスは、情報無し状態になります。
	 */
	DWORD SetPathName(LPCTSTR lpszPathName)
	{
		Empty();
		DWORD shareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE);
		HANDLE h = ::CreateFile(lpszPathName, GENERIC_WRITE, shareMode, NULL, OPEN_EXISTING, 0, 0);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateFile");
		}
		DWORD dwResult = ERROR_SUCCESS;
		HIDD_ATTRIBUTES	attr;
		attr.Size = sizeof(HIDD_ATTRIBUTES);
		// Vendor IDとProduct IDを取得
		if ( ! HidD_GetAttributes(h, &attr) )
		{
			dwResult = _GetLastError("HidD_GetAttributes");
		}
		else
		{
			PHIDP_PREPARSED_DATA pPreparsedData;
			if ( ! HidD_GetPreparsedData(h, &pPreparsedData) )
			{
				dwResult = _GetLastError("HidD_GetPreparsedData");
			}
			else
			{
				HidP_GetCaps(pPreparsedData, &m_hidpCaps);
				HidD_FreePreparsedData(pPreparsedData);
				m_vendorId = attr.VendorID;
				m_productId = attr.ProductID;
				m_pathName = lpszPathName;
				m_regPathHid.Empty();
				m_regPathUsb.Empty();
//				TRACE3("desc=[%s], loca=[%s], mfg=[%s]\n", GetDeviceDescription(), GetLocationInformation(), GetManufacturer());
			}
		}
		::CloseHandle(h);
		return dwResult;
	}

	/**
	 * [設定] リンクコレクションノード情報取得.
	 *	@param[out] _vn 成功時情報が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗
	 */
	bool GetLinkCollectionNodes(CWorkMemT<HIDP_LINK_COLLECTION_NODE>& _vn)
	{
		_vn.Resize(m_hidpCaps.NumberLinkCollectionNodes);
		DWORD shareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE);
		HANDLE h = ::CreateFile(m_pathName, GENERIC_WRITE, shareMode, NULL, OPEN_EXISTING, 0, 0);
		if ( h == INVALID_HANDLE_VALUE )
		{
			_GetLastError("CreateFile");
			return false;
		}
		bool r = false;
		PHIDP_PREPARSED_DATA pPreparsedData;
		if ( HidD_GetPreparsedData(h, &pPreparsedData) )
		{
			ULONG ul = static_cast<ULONG>(_vn.GetSize());
			r = (HidP_GetLinkCollectionNodes(_vn.Ref(), &ul, pPreparsedData) == HIDP_STATUS_SUCCESS);
			HidD_FreePreparsedData(pPreparsedData);
		}
		::CloseHandle(h);
		return r;
	}

	/**
	 * [設定] ボタン性能情報取得.
	 *	@param[out] _vn 成功時情報が格納されます。
	 *	@param[in] reportType レポートタイプ
	 *	@retval true 成功。
	 *	@retval false 失敗
	 */
	bool GetButtonCaps(CWorkMemT<HIDP_BUTTON_CAPS>& _vn, HIDP_REPORT_TYPE reportType)
	{
		switch ( reportType )
		{
		case HidP_Input:
			_vn.Resize(m_hidpCaps.NumberInputButtonCaps);
			break;
		case HidP_Output:
			_vn.Resize(m_hidpCaps.NumberOutputButtonCaps);
			break;
		case HidP_Feature:
		default:
			_vn.Resize(m_hidpCaps.NumberFeatureButtonCaps);
			break;
		}
		DWORD shareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE);
		HANDLE h = ::CreateFile(m_pathName, GENERIC_WRITE, shareMode, NULL, OPEN_EXISTING, 0, 0);
		if ( h == INVALID_HANDLE_VALUE )
		{
			_GetLastError("CreateFile");
			return false;
		}
		bool r = false;
		PHIDP_PREPARSED_DATA pPreparsedData;
		if ( HidD_GetPreparsedData(h, &pPreparsedData) )
		{
			USHORT ul = static_cast<USHORT>(_vn.GetSize());
			r = (HidP_GetButtonCaps(reportType, _vn.Ref(), &ul, pPreparsedData) == HIDP_STATUS_SUCCESS);
			HidD_FreePreparsedData(pPreparsedData);
		}
		::CloseHandle(h);
		return r;
	}

	/**
	 * [設定] 値性能情報取得.
	 *	@param[out] _vn 成功時情報が格納されます。
	 *	@param[in] reportType レポートタイプ
	 *	@retval true 成功。
	 *	@retval false 失敗
	 */
	bool GetValueCaps(CWorkMemT<HIDP_VALUE_CAPS>& _vn, HIDP_REPORT_TYPE reportType)
	{
		switch ( reportType )
		{
		case HidP_Input:
			_vn.Resize(m_hidpCaps.NumberInputValueCaps);
			break;
		case HidP_Output:
			_vn.Resize(m_hidpCaps.NumberOutputValueCaps);
			break;
		case HidP_Feature:
		default:
			_vn.Resize(m_hidpCaps.NumberFeatureValueCaps);
			break;
		}
		DWORD shareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE);
		HANDLE h = ::CreateFile(m_pathName, GENERIC_WRITE, shareMode, NULL, OPEN_EXISTING, 0, 0);
		if ( h == INVALID_HANDLE_VALUE )
		{
			_GetLastError("CreateFile");
			return false;
		}
		bool r = false;
		PHIDP_PREPARSED_DATA pPreparsedData;
		if ( HidD_GetPreparsedData(h, &pPreparsedData) )
		{
			USHORT ul = static_cast<USHORT>(_vn.GetSize());
			r = (HidP_GetValueCaps(reportType, _vn.Ref(), &ul, pPreparsedData) == HIDP_STATUS_SUCCESS);
			HidD_FreePreparsedData(pPreparsedData);
		}
		::CloseHandle(h);
		return r;
	}

	/**
	 * [取得] Capability情報取得
	 *	@return 情報の参照。
	 */
	const HIDP_CAPS& GetCapability(void) const
	{
		return m_hidpCaps;
	}

	/**
	 * [取得] パスネーム取得
	 *	@return パスネーム。
	 */
	const CStr& GetPathName(void) const
	{
		return m_pathName;
	}

	/**
	 * [取得] ベンダID取得
	 *	@return ベンダID。
	 */
	WORD GetVendorId(void) const
	{
		return m_vendorId;
	}
	
	/**
	 * [取得] プロダクトID取得
	 *	@return プロダクトID。
	 */
	WORD GetProductId(void) const
	{
		return m_productId;
	}

	/**
	 * [取得] Inputレポート長取得
	 *	@return Inputレポート長。
	 */
	size_t GetInputReportByteLength(void) const
	{
		return m_hidpCaps.InputReportByteLength;
	}

	/**
	 * [取得] Outputレポート長取得
	 *	@return Outputレポート長。
	 */
	size_t GetOutputReportByteLength(void) const
	{
		return m_hidpCaps.OutputReportByteLength;
	}

	/**
	 * [取得] Featureレポート長取得
	 *	@return Featureレポート長。
	 */
	size_t GetFeatureReportByteLength(void) const
	{
		return m_hidpCaps.FeatureReportByteLength;
	}

	/**
	 * [取得] 製造業者取得.
	 *	@note レジストリから取得しています。
	 *	@return 製造業者
	 */
	CStr GetManufacturer(void) const
	{
		m_SearchRegPath();
		if ( ! m_regPathHid.IsEmpty() )
		{
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_GetBaseRegPath(), KEY_READ);
			return m_GetRegString(reg, m_regPathHid, _T("Mfg"));
		}
		return _T("");
	}

	/**
	 * [取得] 説明取得.
	 *	@note レジストリから取得しています。
	 *	@return 説明
	 */
	CStr GetDeviceDescription(void) const
	{
		m_SearchRegPath();
		if ( ! m_regPathHid.IsEmpty() )
		{
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_GetBaseRegPath(), KEY_READ);
			return m_GetRegString(reg, m_regPathHid, _T("DeviceDesc"));
		}
		return _T("");
	}

	/**
	 * [取得] 場所情報取得.
	 *	@note レジストリから取得しています。
	 *	@return 場所情報
	 */
	CStr GetLocationInformation(void) const
	{
		m_SearchRegPath();
		if ( ! m_regPathUsb.IsEmpty() )
		{
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_GetBaseRegPath(), KEY_READ);
			return m_GetRegString(reg, m_regPathUsb, _T("LocationInformation"));
		}
		return _T("");
	}

	/// Description 文字列群
	struct TDescriptionStrings
	{
		CStr manufacturer;		///< Manufacturer 文字列
		CStr product;			///< Product 文字列
		CStr serialNumber;		///< SerialNumber 文字列
	};

	/**
	 * [取得] Description 文字列取得.
	 *	@note 一度デバイスをオープンして取得しています。
	 *	@param[out] _ds 成功するとDescription 文字列が格納されます。
	 *	@retval false 失敗.
	 *	@retval true 成功.
	 */
	bool GetManufacturerString(TDescriptionStrings& _ds) const
	{
		_ds.manufacturer.Empty();
		_ds.product.Empty();
		_ds.serialNumber.Empty();
		HANDLE hDown = ::CreateFile(m_pathName, GENERIC_WRITE, (FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, 0, 0);
		if ( hDown == INVALID_HANDLE_VALUE )
		{
			return false;
		}
		CUnicode s;
		bool r = !! HidD_GetManufacturerString(hDown, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		if ( r )
		{
			_ds.manufacturer = s;
		}
		r = !! HidD_GetProductString(hDown, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		if ( r )
		{
			_ds.product = s;
		}
		r = !! HidD_GetSerialNumberString(hDown, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		if ( r )
		{
			_ds.serialNumber = s;
		}
		::CloseHandle(hDown);
		return true;
	}

	/**
	 * [取得] 文字列取得.
	 *	@note VID PID などを含む文字列を返します。
	 *	@return 文字列。
	 */
	CStr ToString(void) const
	{
		CStr str;
		USHORT vid = GetVendorId();
		USHORT pid = GetProductId();
		TDescriptionStrings ds;
		if ( GetManufacturerString(ds) )
		{
			if ( ds.manufacturer != _T("?") && ! ds.manufacturer.IsEmpty() )
			{
				str.Format(_T("VID=0x%04X, PID=0x%04X, [%s] - %s"), vid, pid, ds.product, ds.manufacturer);
				return str;
			}
		}
		CWindowsVersion wv;
		if ( wv.GetMajorVersion() <= 5/*XP,2000*/ )
		{
			str.Format(_T("VID=0x%04X, PID=0x%04X [%s]"), vid, pid, GetLocationInformation());
		}
		else
		{
			str.Format(_T("VID=0x%04X, PID=0x%04X, [%s] - %s"), vid, pid, GetDeviceDescription(), GetManufacturer());
		}
		return str;
	}

private:
	/// 内部検索クラス
	class CMyFinder : public CDeepAccessFinder
	{
	public:
		CStr	m_targetId;
		CStr	m_foundPath;
	protected:
		// セクション発見.
		virtual EResult OnFindSection(LPCTSTR lpszSection, IAccessor::CSection& sec, int depth)
		{
			if ( depth >= 2 )
			{
				return Previous;
			}
			if ( m_targetId.IsEqual(sec.QueryString(_T("ParentIdPrefix"))) )
			{
				m_foundPath = sec.GetSectionName();
				return Stop;
			}
			return Next;
		}
	};
	/// レジストリパス検索
	bool m_SearchRegPath(void) const
	{
		if ( m_regPathHid.IsEmpty() || m_regPathUsb.IsEmpty() )
		{
			INDEX f1 = m_pathName.Find('#');
			INDEX f2 = m_pathName.Find('#', f1 + 1);
			INDEX f3 = m_pathName.Find('#', f2 + 1);
			if ( f1 != INVALID_INDEX && f2 != INVALID_INDEX && f3 != INVALID_INDEX )
			{
				CMyFinder ff;
				CStr vpid = m_pathName.Mid(f1 + 1, f2 - f1 - 1);
				CStr vpid2 = vpid;
				INDEX f = vpid.Find('&', 18);
				if ( f != INVALID_INDEX )
				{
					vpid2 = vpid.Left(f);
				}
				CStr key = m_pathName.Mid(f2 + 1, f3 - f2 - 1);
				ff.m_targetId = key.Left(key.GetLength() - 5);
				CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_GetBaseRegPath(), KEY_READ);
				ff.Execute(reg[_T("USB\\") + vpid2]);
				m_regPathUsb = ff.m_foundPath;
				CStrVector vs = reg.EnumSectionNames(_T("HID\\") + vpid);
				if ( ! vs.IsEmpty() )
				{
					m_regPathHid.Format(_T("HID\\%s\\%s"), vpid, vs[0]);
				}
			}
		}
		return (! m_regPathHid.IsEmpty() && ! m_regPathUsb.IsEmpty());
	}
	/// レジストリの文字取得
	CStr m_GetRegString(CRegistryAccessor& reg, LPCTSTR lpszSection, LPCTSTR lpszKey) const
	{
		CStr s = reg[lpszSection].QueryString(lpszKey).Trim();
		INDEX f = s.Find(';');
		if ( f != INVALID_INDEX )
		{
			s = s.Mid(f + 1);
		}
		return s;
	}
	/// レジストリのベースパス取得
	LPCTSTR m_GetBaseRegPath(void) const
	{
		return _T("System\\currentControlSet\\Enum");
	}
	WORD			m_vendorId;			///< ベンダID
	WORD			m_productId;		///< プロダクトID
	CStr			m_pathName;			///< パスネーム
	HIDP_CAPS		m_hidpCaps;			///< 容量等
	mutable CStr	m_regPathHid;		///< レジストリパス HID
	mutable CStr	m_regPathUsb;		///< レジストリパス USB
};



}; // TNB
