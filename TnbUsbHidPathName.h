#pragma once
/**
 *@file
 * USB HID�̃p�X�l�[�����Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbRegistryAccessor.h"
#include "TnbWindowsVersion.h"

#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#pragma pack(push,4)

// HIDUSAGE.H ���
#ifndef __HIDUSAGE_H__

	typedef USHORT USAGE, *PUSAGE;

#endif // __HIDUSAGE_H__

// HIDPI.H ���
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

//HIDSDI.h ���
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
 * hid.dll �A�N�Z�X�N���X
 *
 *		hid.dll �𓮓I�Ƀ����N���A���̃��\�b�h���A�N�Z�X�ł��܂��B
 *		����ɂ��A DDK(WDK)���Ȃ��Ă��A HID �֌W�� API ���g�p���邱�Ƃ��o���܂��B
 *
 *		hid.dll �������Ă��郁�\�b�h�Ɠ������̂������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbUsbHidPathName.h
 * 
 *	@date 10/02/04 �V�K�쐬
 *	@date 10/03/17 GetString�n���\�b�h���A�ǉ��B�@
 *	@date 13/01/08 HidD_SetOutputReport(), HidD_GetInputReport(), �ǉ��B
 */
class CUsbHidApi
{
public:

	/// �R���X�g���N�^
	CUsbHidApi(void) : m_module(NULL)
	{
	}
	
	/**
	 * �R�s�[�R���X�g���N�^.
	 *	@note �����R�s�[���܂���
	 *	@param other �R�s�[��
	 */
	CUsbHidApi(const CUsbHidApi& other) : m_module(NULL)
	{
	}

	/// �f�X�g���N�^
	~CUsbHidApi(void)
	{
		if ( m_module != NULL )
		{
			::FreeLibrary(m_module);
			m_module = NULL;
		}
	}

	/**
	 * �R�s�[�I�y���[�^
	 *	@note �����R�s�[���܂���
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��
	 */
	CUsbHidApi& operator=(const CUsbHidApi& other)
	{
		return *this;
	}

	/**
	 * [�擾] �����擾.
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] pAttributes �擾�����������i�[����܂�.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetAttributes(HANDLE hidDeviceObject, PHIDD_ATTRIBUTES pAttributes) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PHIDD_ATTRIBUTES pAttributes) = 0;
		return Func(P, "HidD_GetAttributes")(hidDeviceObject, pAttributes);
	}

	/**
	 * [�擾] PREPARSED�f�[�^�擾.
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] ppPreparsedData �擾���� PREPARSED�f�[�^���i�[����Ă���|�C���^���i�[����܂�.
	 *					�g�p��͕K���A HidD_FreePreparsedData() �ŊJ�����Ă��������B
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetPreparsedData(HANDLE hidDeviceObject, PHIDP_PREPARSED_DATA* ppPreparsedData) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PHIDP_PREPARSED_DATA* ppPreparsedData) = 0;
		return Func(P, "HidD_GetPreparsedData")(hidDeviceObject, ppPreparsedData);
	}

	/**
	 * [�J��] PREPARSED�f�[�^�J��.
	 *		HidD_GetPreparsedData() �Ŏ擾�����f�[�^�̊J�����s���܂��B
	 *	@param[in] pPreparsedData HidD_GetPreparsedData() �Ŋl������ PREPARSED�f�[�^�̃|�C���^.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_FreePreparsedData(PHIDP_PREPARSED_DATA pPreparsedData) const
	{
		BOOLEAN (__stdcall *P)(PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidD_FreePreparsedData")(pPreparsedData);
	}

	/**
	 * [���M] �@�\���|�[�g���M
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[in] pReportBuffer ���|�[�g���e.
	 *	@param[in] reportBufferLength ���|�[�g����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_SetFeature(HANDLE hidDeviceObject,PVOID pReportBuffer, ULONG reportBufferLength)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject,PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_SetFeature")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}

	/**
	 * [��M] �@�\���|�[�g��M
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] pReportBuffer ��M�������|�[�g���e���i�[�����o�b�t�@�ł�.
	 *	@param[in] reportBufferLength ��M�o�b�t�@����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetFeature(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_GetFeature")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}

	/**
	 * [���M] Output���|�[�g���M
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[in] pReportBuffer ���|�[�g���e.
	 *	@param[in] reportBufferLength ���|�[�g����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_SetOutputReport(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_SetOutputReport")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}

	/**
	 * [��M] Input���|�[�g��M
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] pReportBuffer ��M�������|�[�g���e���i�[�����o�b�t�@�ł�.
	 *	@param[in] reportBufferLength ��M�o�b�t�@����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetInputReport(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pReportBuffer, ULONG reportBufferLength) = 0;
		return Func(P, "HidD_GetInputReport")(hidDeviceObject, pReportBuffer, reportBufferLength);
	}
		
	/**
	 * [�擾] HID GUID �擾.
	 *	@param[out] pHidGuid �擾���� GUID ���i�[����܂��B
	 */
	void HidD_GetHidGuid(LPGUID pHidGuid) const
	{
		void (__stdcall *P)(LPGUID pHidGuid) = NULL;
		Func(P, "HidD_GetHidGuid")(pHidGuid);
	}

	/**
	 * [�擾] Manufacturer ������擾
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] pBuffer ��M���������񂪊i�[�����o�b�t�@�ł�.
	 *	@param[in] bufferLength �o�b�t�@����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetManufacturerString(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetManufacturerString")(hidDeviceObject, pBuffer, bufferLength);
	}

	/**
	 * [�擾] Product ������擾
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] pBuffer ��M���������񂪊i�[�����o�b�t�@�ł�.
	 *	@param[in] bufferLength �o�b�t�@����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetProductString(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetProductString")(hidDeviceObject, pBuffer, bufferLength);
	}

	/**
	 * [�擾] SerialNumber ������擾
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] pBuffer ��M���������񂪊i�[�����o�b�t�@�ł�.
	 *	@param[in] bufferLength �o�b�t�@����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetSerialNumberString(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetSerialNumberString")(hidDeviceObject, pBuffer, bufferLength);
	}

	/**
	 * [�擾] INDEXED ������擾
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[in] stringIndex �C���f�b�N�X
	 *	@param[out] pBuffer ��M���������񂪊i�[�����o�b�t�@�ł�.
	 *	@param[in] bufferLength �o�b�t�@����.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetIndexedString(HANDLE hidDeviceObject, ULONG stringIndex, PVOID pBuffer, ULONG bufferLength) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, ULONG stringIndex, PVOID pBuffer, ULONG bufferLength) = 0;
		return Func(P, "HidD_GetIndexedString")(hidDeviceObject, stringIndex, pBuffer, bufferLength);
	}

	/**
	 * [�擾] ���̓o�b�t�@���擾.
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param[out] pNumberBuffers ���̓o�b�t�@�����i�[�����o�b�t�@�ł�.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_GetNumInputBuffers(HANDLE hidDeviceObject, PULONG pNumberBuffers) const
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, PULONG pNumberBuffers) = 0;
		return Func(P, "HidD_GetNumInputBuffers")(hidDeviceObject, pNumberBuffers);
	}

	/**
	 * [�ݒ�] ���̓o�b�t�@���ݒ�.
	 *	@param hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@param numberBuffers ���̓o�b�t�@��.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_SetNumInputBuffers(HANDLE hidDeviceObject, ULONG numberBuffers)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject, ULONG numberBuffers) = 0;
		return Func(P, "HidD_SetNumInputBuffers")(hidDeviceObject, numberBuffers);
	}

	/**
	 * [����] �L���[�t���b�V��.
	 *	@param[in] hidDeviceObject HID�f�o�C�X���I�[�v�������n���h��.
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOLEAN HidD_FlushQueue(HANDLE hidDeviceObject)
	{
		BOOLEAN (__stdcall *P)(HANDLE hidDeviceObject) = 0;
		return Func(P, "HidD_FlushQueue")(hidDeviceObject);
	}
	
	/**
	 * [�擾] �\�͎擾.
	 *	@param[in] pPreparsedData PREPARSED�f�[�^.
	 *	@param[out] pCapabilities �擾�����\�̓f�[�^���i�[����܂��B
	 *	@retval HIDP_STATUS_SUCCESS ����.
	 *	@retval ��L�ȊO ���s.
	 */
	NTSTATUS HidP_GetCaps(PHIDP_PREPARSED_DATA pPreparsedData, PHIDP_CAPS pCapabilities) const
	{
		NTSTATUS (__stdcall *P)(PHIDP_PREPARSED_DATA pPreparsedData, PHIDP_CAPS pCapabilities) = 0;
		return Func(P, "HidP_GetCaps")(pPreparsedData, pCapabilities);
	}
	
	/**
	 * [�擾] �����N�R���N�V�����m�[�h�擾.
	 *	@param[out] pLinkCollectionNodes �m�[�h���̔z��
	 *	@param[in,out] pLinkCollectionNodesLength in �Ńm�[�h���̔z�񐔁B out �Ŏ擾���������Ԃ�܂��B
	 *	@param[in] pPreparsedData PREPARSED�f�[�^.
	 *	@retval HIDP_STATUS_SUCCESS ����.
	 *	@retval ��L�ȊO ���s.
	 */
	NTSTATUS HidP_GetLinkCollectionNodes(PHIDP_LINK_COLLECTION_NODE pLinkCollectionNodes, PULONG pLinkCollectionNodesLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		NTSTATUS (__stdcall *P)(PHIDP_LINK_COLLECTION_NODE pLinkCollectionNodes, PULONG pLinkCollectionNodesLength, PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidP_GetLinkCollectionNodes")(pLinkCollectionNodes, pLinkCollectionNodesLength, pPreparsedData);
	}

	/**
	 * [�擾] �{�^���ڍא��\�擾.
	 *	@param[in] reportType ���|�[�g�^�C�v
	 *	@param[in] usagePage USAGE PAGE
	 *	@param[in] linkCollection LINK COLLECTION 
	 *	@param[in] usage USAGE
	 *	@param[out] pButtonCaps �{�^�����\���̔z��
	 *	@param[in,out] pButtonCapsLength in �Ń{�^�����\���̔z�񐔁B out �Ŏ擾���������Ԃ�܂��B
	 *	@param[in] pPreparsedData PREPARSED�f�[�^.
	 *	@retval HIDP_STATUS_SUCCESS ����.
	 *	@retval ��L�ȊO ���s.
	 */
	NTSTATUS HidP_GetSpecificButtonCaps(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_BUTTON_CAPS pButtonCaps, PUSHORT pButtonCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		NTSTATUS (__stdcall *P)(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_BUTTON_CAPS pButtonCaps, PUSHORT pButtonCapsLength, PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidP_GetSpecificButtonCaps")(reportType, usagePage, linkCollection, usage, pButtonCaps, pButtonCapsLength, pPreparsedData);
	}

	/**
	 * [�擾] �{�^�����\�擾.
	 *	@param[in] reportType ���|�[�g�^�C�v
	 *	@param[out] pButtonCaps �{�^�����\���̔z��
	 *	@param[in,out] pButtonCapsLength in �Ń{�^�����\���̔z�񐔁B out �Ŏ擾���������Ԃ�܂��B
	 *	@param[in] pPreparsedData PREPARSED�f�[�^.
	 *	@retval HIDP_STATUS_SUCCESS ����.
	 *	@retval ��L�ȊO ���s.
	 */
	NTSTATUS HidP_GetButtonCaps(HIDP_REPORT_TYPE reportType, PHIDP_BUTTON_CAPS pButtonCaps, PUSHORT pButtonCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		return HidP_GetSpecificButtonCaps(reportType, 0, 0, 0, pButtonCaps, pButtonCapsLength, pPreparsedData);
	}

	/**
	 * [�擾] �l�ڍא��\�擾.
	 *	@param[in] reportType ���|�[�g�^�C�v
	 *	@param[in] usagePage USAGE PAGE
	 *	@param[in] linkCollection LINK COLLECTION 
	 *	@param[in] usage USAGE
	 *	@param[out] pValueCaps �l���\���̔z��
	 *	@param[in,out] pValueCapsLength in �Œl���\���̔z�񐔁B out �Ŏ擾���������Ԃ�܂��B
	 *	@param[in] pPreparsedData PREPARSED�f�[�^.
	 *	@retval HIDP_STATUS_SUCCESS ����.
	 *	@retval ��L�ȊO ���s.
	 */
	NTSTATUS HidP_GetSpecificValueCaps(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_VALUE_CAPS pValueCaps, PUSHORT pValueCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		NTSTATUS (__stdcall *P)(HIDP_REPORT_TYPE reportType, USAGE usagePage, USHORT linkCollection, USAGE usage, PHIDP_VALUE_CAPS pValueCaps, PUSHORT pValueCapsLength, PHIDP_PREPARSED_DATA pPreparsedData) = 0;
		return Func(P, "HidP_GetSpecificValueCaps")(reportType, usagePage, linkCollection, usage, pValueCaps, pValueCapsLength, pPreparsedData);
	}

	/**
	 * [�擾] �l���\�擾.
	 *	@param[in] reportType ���|�[�g�^�C�v
	 *	@param[out] pValueCaps �l���\���̔z��
	 *	@param[in,out] pValueCapsLength in �Œl���\���̔z�񐔁B out �Ŏ擾���������Ԃ�܂��B
	 *	@param[in] pPreparsedData PREPARSED�f�[�^.
	 *	@retval HIDP_STATUS_SUCCESS ����.
	 *	@retval ��L�ȊO ���s.
	 */
	NTSTATUS HidP_GetValueCaps(HIDP_REPORT_TYPE reportType, PHIDP_VALUE_CAPS pValueCaps, PUSHORT pValueCapsLength, PHIDP_PREPARSED_DATA pPreparsedData)
	{
		return HidP_GetSpecificValueCaps(reportType, 0, 0, 0, pValueCaps, pValueCapsLength, pPreparsedData);
	}

private:

	// EXPORT�֐��R�[��
	template<typename TYP>
	TYP Func(TYP typ, LPCSTR lpsz) const
	{
		if ( m_module == NULL )
		{
			m_module = ::LoadLibrary(_T("hid.dll"));
			ASSERT( m_module != NULL );
		}
		ForceSet(typ, ::GetProcAddress(m_module, lpsz));
		ASSERT1( typ != NULL, "CUsbHidApi::Func", "[%s]��������܂���ł���", lpsz );
		return typ;
	}

	mutable HMODULE	m_module;	///< hid.dll �̃n���h��
};



/**@ingroup USB
 * USB-HID�p�X�l�[�����Ǘ��N���X
 *
 *		USB-HID �̃f�o�C�X����ӂɎ����p�X�l�[��(������)�ƁA����ɑΉ�����
 *		Vendor-ID,Product-ID�ƃp�X�l�[����Capabiliry���Ǘ��B
 *
 *		���݂��Ȃ� USB-HID�̃f�o�C�X�̃p�X�l�[���͊Ǘ��ł��܂���B
 *
 *	@see CUsbHid 
 *
 *	@note �����\
 *
 *	@note �u�p�X�l�[���v�Ƃ́A���ۂɂ��� HID�f�o�C�X���I�[�v�����邽�߂Ɏg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbUsbHidPathName.h
 * 
 *	@date 07/04/26 �V�K�쐬
 *	@date 10/02/04 DDK ���Ȃ��Ă��g����悤�ɐ錾�������B
 *	@date 10/02/23 GetManufacturer() , GetDeviceDescription() �ȂǁA�擾���\�b�h�ǉ��B
 *	@date 11/01/19 ToString() ��ǉ��B
 */
class CUsbHidPathName : CUsbHidApi
{
public:

	/**
	 * �R���X�g���N�^
	 *	@note Empty ��Ԃ̃N���X���쐬���܂��B
	 */
	CUsbHidPathName(void) : m_vendorId(0), m_productId(0), m_pathName()
	{
		Zero(m_hidpCaps);
	}

	/**
	 * �R���X�g���N�^
	 *	@note �w��̏����������N���X���쐬���܂��B
	 *	@param wVid Vendor ID�B
	 *	@param wPid Product ID�B
	 *	@param lpszPathName �p�X�l�[���B
	 *	@param cap Capability���
	 */
	CUsbHidPathName(WORD wVid, WORD wPid, LPCTSTR lpszPathName, const HIDP_CAPS& cap)
		: m_vendorId(wVid), m_productId(wPid), m_pathName(lpszPathName), m_hidpCaps(cap)
	{
	}

	/**
	 * [�m�F] ���L���m�F
	 *	@retval true �����
	 *	@retval false ��񂠂�
	 */
	bool IsEmpty(void) const
	{
		return m_pathName.IsEmpty();
//		return m_vendorId != 0;
	}

	/**
	 * [�ݒ�] ���Ȃ�
	 *	@note �{�C���X�^���X�̏����N���A���A��񖳂���Ԃɂ��܂��B
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
	 * [�ݒ�] �p�X�l�[���ݒ�.
	 *		�w��̃p�X�l�[�����g���AUSB-HID�f�o�C�X���A�N�Z�X���A 
	 *		Vendor ID,Product ID,Capability ���擾���A�{�C���X�^���X�ɐݒ肵�܂��B
	 *	@note �p�X�l�[�������̐ݒ�ł͂���܂���B
	 *	@param lpszPathName �p�X�l�[���B
	 *	@retval ERROR_SUCCESS �����B�{�C���X�^���X�ɏ�񂪊i�[����Ă��܂��B
	 *	@retval ��L�ȊO ���s�B�߂�l�̓G���[�R�[�h�B�{�C���X�^���X�́A��񖳂���ԂɂȂ�܂��B
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
		// Vendor ID��Product ID���擾
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
	 * [�ݒ�] �����N�R���N�V�����m�[�h���擾.
	 *	@param[out] _vn ��������񂪊i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s
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
	 * [�ݒ�] �{�^�����\���擾.
	 *	@param[out] _vn ��������񂪊i�[����܂��B
	 *	@param[in] reportType ���|�[�g�^�C�v
	 *	@retval true �����B
	 *	@retval false ���s
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
	 * [�ݒ�] �l���\���擾.
	 *	@param[out] _vn ��������񂪊i�[����܂��B
	 *	@param[in] reportType ���|�[�g�^�C�v
	 *	@retval true �����B
	 *	@retval false ���s
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
	 * [�擾] Capability���擾
	 *	@return ���̎Q�ƁB
	 */
	const HIDP_CAPS& GetCapability(void) const
	{
		return m_hidpCaps;
	}

	/**
	 * [�擾] �p�X�l�[���擾
	 *	@return �p�X�l�[���B
	 */
	const CStr& GetPathName(void) const
	{
		return m_pathName;
	}

	/**
	 * [�擾] �x���_ID�擾
	 *	@return �x���_ID�B
	 */
	WORD GetVendorId(void) const
	{
		return m_vendorId;
	}
	
	/**
	 * [�擾] �v���_�N�gID�擾
	 *	@return �v���_�N�gID�B
	 */
	WORD GetProductId(void) const
	{
		return m_productId;
	}

	/**
	 * [�擾] Input���|�[�g���擾
	 *	@return Input���|�[�g���B
	 */
	size_t GetInputReportByteLength(void) const
	{
		return m_hidpCaps.InputReportByteLength;
	}

	/**
	 * [�擾] Output���|�[�g���擾
	 *	@return Output���|�[�g���B
	 */
	size_t GetOutputReportByteLength(void) const
	{
		return m_hidpCaps.OutputReportByteLength;
	}

	/**
	 * [�擾] Feature���|�[�g���擾
	 *	@return Feature���|�[�g���B
	 */
	size_t GetFeatureReportByteLength(void) const
	{
		return m_hidpCaps.FeatureReportByteLength;
	}

	/**
	 * [�擾] �����ƎҎ擾.
	 *	@note ���W�X�g������擾���Ă��܂��B
	 *	@return �����Ǝ�
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
	 * [�擾] �����擾.
	 *	@note ���W�X�g������擾���Ă��܂��B
	 *	@return ����
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
	 * [�擾] �ꏊ���擾.
	 *	@note ���W�X�g������擾���Ă��܂��B
	 *	@return �ꏊ���
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

	/// Description ������Q
	struct TDescriptionStrings
	{
		CStr manufacturer;		///< Manufacturer ������
		CStr product;			///< Product ������
		CStr serialNumber;		///< SerialNumber ������
	};

	/**
	 * [�擾] Description ������擾.
	 *	@note ��x�f�o�C�X���I�[�v�����Ď擾���Ă��܂��B
	 *	@param[out] _ds ���������Description �����񂪊i�[����܂��B
	 *	@retval false ���s.
	 *	@retval true ����.
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
	 * [�擾] ������擾.
	 *	@note VID PID �Ȃǂ��܂ޕ������Ԃ��܂��B
	 *	@return ������B
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
	/// ���������N���X
	class CMyFinder : public CDeepAccessFinder
	{
	public:
		CStr	m_targetId;
		CStr	m_foundPath;
	protected:
		// �Z�N�V��������.
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
	/// ���W�X�g���p�X����
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
	/// ���W�X�g���̕����擾
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
	/// ���W�X�g���̃x�[�X�p�X�擾
	LPCTSTR m_GetBaseRegPath(void) const
	{
		return _T("System\\currentControlSet\\Enum");
	}
	WORD			m_vendorId;			///< �x���_ID
	WORD			m_productId;		///< �v���_�N�gID
	CStr			m_pathName;			///< �p�X�l�[��
	HIDP_CAPS		m_hidpCaps;			///< �e�ʓ�
	mutable CStr	m_regPathHid;		///< ���W�X�g���p�X HID
	mutable CStr	m_regPathUsb;		///< ���W�X�g���p�X USB
};



}; // TNB
