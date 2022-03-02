#pragma once
/**
 *@file
 * SMART (Self Monitoring, Analysis and Reporting Technology)�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbSimpleVector.h"
#include <WinIoCtl.h>



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * SMART (Self Monitoring, Analysis and Reporting Technology)�A�N�Z�X�N���X.
 *
 *	\code
 *		CSmartAccessor sa;
 *		loop ( d, 26 )
 *		{
 *			if ( sa.Collect(d) )
 *			{
 *				TRACE1("Found Disk %d\n", d);
 *				TRACE3("    ���x=%d��, �d��������=%d��, �g�p����=%d����\n"
 *						, sa.GetTemperature(), sa.GetPowerCycleCount(), sa.GetPowerOnHoursCount() );
 *				loop ( i, sa.GetAttributeCount() )
 *				{
 *					const CSmartAccessor::TAttribute* a = sa.GetAttribute(i);
 *					ASSERT( a != NULL );
 *					TRACE3("    %02X[%-38s] %s\n", a->bAttrID, a->GetAttributeIdString(), a->ToString());
 *				}
 *			}
 *		}
 *	\endcode
 *
 *
 *	@par�K�v�t�@�C�� 
 *			TnbSmartAccessor.h
 *
 *	@date 12/02/14 �V�K�쐬
 */
class CSmartAccessor : CCopyImpossible
{
public:
	
	/**@ingroup FILE
	 * SMART �o�[�W�������
	 */
	struct TVersion
	{
		BYTE	bVersion;		///< Binary driver version.
		BYTE	bRevision;		///< Binary driver revision.
		BYTE	bReserved;		///< Not used.
		BYTE	bIDEDeviceMap;	///< Bit map of IDE devices.
		DWORD	fCapabilities;	///< Bit mask of driver capabilities.
		DWORD	dwReserved[4];	///< For future use.
	};

	/**@ingroup FILE
	 * SMART �����^
	 *	@note DRIVEATTRIBUTE �\���̂��x�[�X�ł�
	 */
	struct TAttribute
	{
		#pragma pack(push,1)
		BYTE	bAttrID;		///< Identifies which attribute
		WORD	wStatusFlags;	///< see bit definitions below
		BYTE	bAttrValue;		///< Current normalized value
		BYTE	bWorstValue;	///< How bad has it ever been?
		WORD	bRawValue[3];	///< Un-normalized value
		BYTE	bReserved;		///< ...
		#pragma	pack(pop)
		/**
		 * [�擾] Un-normalized value�擾
		 *		bRawValue[3] �� 48bit �l�Ƃ��ĕԂ��܂��B
		 *	@return �l
		 */
		ULONGLONG GetValue(void) const
		{
			ULONGLONG ll = bRawValue[2];
			ll <<= 16;	ll += bRawValue[1];
			ll <<= 16;	ll += bRawValue[0];
			return ll;
		}
		/**
		 * [�擾] ������
		 *		���݂̒l ���[�X�g�̒l ���f�[�^x6 �̕�����𐶐����܂��B
		 *	@return ������B
		 */
		CStr ToString(void) const
		{
			CStr s;
			s.Format(_T("%03d %03d  %012I64X"), bAttrValue, bWorstValue, GetValue());
			return s;
		}
		/**
		 * [�擾] �����h�c������擾
		 *	@param isEnglish true �Ȃ�p��A false �Ȃ���{��
		 *	@return ������
		 */
		CStr GetAttributeIdString(bool isEnglish = true) const
		{
			return GetAttributeIdString(bAttrID, isEnglish);
		}
		/**
		 * [�擾] �����h�c������擾
		 *	@param attributeId �����h�c
		 *	@param isEnglish true �Ȃ�p��A false �Ȃ���{��
		 *	@return ������
		 */
		static CStr GetAttributeIdString(BYTE attributeId, bool isEnglish = true)
		{
			CStr r;
			switch ( attributeId )
			{
			case 0x01:	r = isEnglish ? _T("Raw read error rate") : _T("�ǂݍ��݃G���[��");
				break;
			case 0x02:	r = isEnglish ? _T("Throughput performance") : _T("�X���[�v�b�g");
				break;
			case 0x03:	r = isEnglish ? _T("Spinup time") : _T("�X�s���A�b�v����");
				break;
			case 0x04:	r = isEnglish ? _T("Start/Stop count") : _T("��]�^��~��");
				break;
			case 0x05:	r = isEnglish ? _T("Reallocated sector count") : _T("�Ċ��蓖�ăZ�N�^�[��");
				break;
			case 0x06:	r = isEnglish ? _T("Read channel margin") : _T("���[�h�E�`�����l���E�}�[�W��");
				break;
			case 0x07:	r = isEnglish ? _T("Seek error rate") : _T("�V�[�N�G���[��");
				break;
			case 0x08:	r = isEnglish ? _T("Seek timer performance") : _T("�V�[�N�^�C�����\");
				break;
			case 0x09:	r = isEnglish ? _T("Power-on hours count") : _T("���v���p����");
				break;
			case 0x0A:	r = isEnglish ? _T("Spinup retry count") : _T("�X�s���A�b�v�Ď��s��");
				break;
			case 0x0B:	r = isEnglish ? _T("Calibration retry count") : _T("�L�����u���[�V�����Ď��s��");
				break;
			case 0x0C:	r = isEnglish ? _T("Power cycle count") : _T("�d��������");
				break;
			case 0x0D:
			case 0xC9:	r = isEnglish ? _T("Soft read error rate") : _T("�_���ǂݍ��݃G���[��");
				break;
			case 0xBB:
			case 0xBD:
			case 0xBE:	r = isEnglish ? _T("vendor-specific") : _T("�x���_�[�Ǝ����");
				break;
			case 0xBF:	r = isEnglish ? _T("G-sense error rate") : _T("�����x�Z���T�[���o�G���[��");
				break;
			case 0xC0:	r = isEnglish ? _T("Power-off retract count") : _T("�d���ؒf���");
				break;
			case 0xC1:	r = isEnglish ? _T("Load/Unload cycle count") : _T("���[�h�^�A�����[�h�E�T�C�N����");
				break;
			case 0xC2:	r = isEnglish ? _T("HDA temperature") : _T("���x");
				break;
			case 0xC3:	r = isEnglish ? _T("Hardware ECC recovered") : _T("�n�[�h�E�G�AECC����");
				break;
			case 0xC4:	r = isEnglish ? _T("Reallocation count") : _T("�Ċ��蓖�Đ�");
				break;
			case 0xC5:	r = isEnglish ? _T("Current pending sector count") : _T("�s����Z�N�^�[��");
				break;
			case 0xC6:	r = isEnglish ? _T("Offline scan uncorrectable count") : _T("�������G���[��");
				break;
			case 0xC7:	r = isEnglish ? _T("UDMA CRC error rate") : _T("UltraDMA CRC�G���[");
				break;
			case 0xC8:	r = isEnglish ? _T("Write error rate") : _T("�������݃G���[��");
				break;
			//case	0xC9:
			case 0xCA:	r = isEnglish ? _T("Data Address Mark errors") : _T("DAM(Data Address Mark)�G���[");
				break;
			case 0xCB:	r = isEnglish ? _T("Run out cancel") : _T("ECC�G���[");
				break;
			case 0xCC:	r = isEnglish ? _T("Soft ECC correction") : _T("�_��ECC����");
				break;
			case 0xCD:	r = isEnglish ? _T("Thermal asperity rate(TAR)") : _T("�M�G���[���iTAR�j");
				break;
			case 0xCE:	r = isEnglish ? _T("Flying height") : _T("�w�b�h��");
				break;
			case 0xCF:	r = isEnglish ? _T("Spin high current") : _T("�ő��]�d��");
				break;
			case 0xD0:	r = isEnglish ? _T("Spin buzz") : _T("�w�b�h����");
				break;
			case 0xD1:	r = isEnglish ? _T("Offline seek performance") : _T("�I�t���C���E�V�[�N���\");
				break;
			case 0xDC:	r = isEnglish ? _T("Disk shift") : _T("�f�B�X�N����");
				break;
			case 0xDD:	r = isEnglish ? _T("G-sense error rate") : _T("�����x�Z���T�[���o�G���[��");
				break;
			case 0xDE:	r = isEnglish ? _T("Loaded hours") : _T("���p����");
				break;
			case 0xDF:	r = isEnglish ? _T("Load/unload retry count") : _T("���[�h�^�A�����[�h�Ď��s��");
				break;
			case 0xE0:	r = isEnglish ? _T("Load friction") : _T("���[�h��R");
				break;
			case 0xE1:	r = isEnglish ? _T("Load/Unload cycle count") : _T("���[�h�^�A�����[�h�E�T�C�N����");
				break;
			case 0xE2:	r = isEnglish ? _T("Load-in time") : _T("���[�h�C������");
				break;
			case 0xE3:	r = isEnglish ? _T("Torque amplification count") : _T("�g���N������");
				break;
			case 0xE4:	r = isEnglish ? _T("Power-off retract count") : _T("�d���ؒf�ޔ�");
				break;
			case 0xE6:	r = isEnglish ? _T("GMR head amplitude") : _T("GMR�w�b�h�U��");
				break;
			case 0xE7:	r = isEnglish ? _T("Temperature") : _T("���x");
				break;
			case 0xF0:	r = isEnglish ? _T("Head flying hours") : _T("�V�[�N�ړ�����");
				break;
			case 0xFA:	r = isEnglish ? _T("Read error retry rate") : _T("�ǂݍ��݃G���[�Ď��s��");
				break;
			default:
				r = isEnglish ? _T("Unknown") : _T("�s��");
				break;
			}
			return	r;
		}
	};


	//---------------------------------------------------------


	/// �R���X�g���N�^
	CSmartAccessor(void) : m_isValidVersion(false)
	{
	}

	/**
	 * [�擾] ���W.
	 *		�w��̃h���C�uNo ���� SMART �������W���܂��B
	 *	@param deviceNo �h���C�u�m��
	 *	@retval true �����B�{�C���X�^���X���ɏ����L�����܂��B
	 *	@retval false ���s�B
	 */
	bool Collect(int deviceNo)
	{
		m_isValidVersion = false;
		return m_Collect(deviceNo);
	}

	/**
	 * [�擾] �o�[�W�������擾.
	 *	@retval NULL�ȊO �����B�|�C���^��̓o�[�W�������B
	 *	@retval NULL ���s�B Collect() ���������Ă��Ȃ��Ǝ��s���܂��B
	 */
	const TVersion* GetVersion(void) const
	{
		return m_isValidVersion ? &m_version : NULL;
	}

	/**
	 * [�擾] �������擾.
	 *	@retval �O�ȏ� �����B���l�͑������B
	 *	@retval �}�C�i�X ���s�B Collect() ���������Ă��Ȃ��Ǝ��s���܂��B
	 */
	size_t GetAttributeCount(void) const
	{
		return m_infos.GetSize();
	}

	/**
	 * [�擾] �������擾.
	 *	@param index �C���f�b�N�X�B 0�ȏ�A GetAttributeCount() �������w��ł��܂��B
	 *	@retval NULL�ȊO �����B�|�C���^��͑������B
	 *	@retval NULL ���s�B Collect() ���������Ă��Ȃ��Ǝ��s���܂��B
	 */
	const TAttribute* GetAttribute(INDEX index) const
	{
		if ( m_infos.IsInRange(index) )
		{
			return m_infos[index];
		}
		return NULL;
	}

	/**
	 * [�擾] �������擾.
	 *	@param id �����h�c�B
	 *	@retval NULL�ȊO �����B�|�C���^��͑����h�c�̑������B
	 *	@retval NULL �������B
	 */
	const TAttribute* FindAttributeId(BYTE id) const
	{
		loop ( i, m_infos.GetSize() )
		{
			if ( m_infos[i]->bAttrID == id )
			{
				return m_infos[i];
			}
		}
		return NULL;
	}

	/**
	 * [�擾] ���x���擾.
	 *	@retval �O�ȏ� �����B���l�͉��x�B
	 *	@retval �}�C�i�X ���s�B��񂪂Ȃ��\��������܂��B
	 */
	int GetTemperature(void) const
	{
		const TAttribute* p = FindAttributeId(0xC2/*���x*/);
		if ( p != NULL )
		{
			return p->bRawValue[0];
		}
		return -1;
	}

	/**
	 * [�擾] ���v���p���Ԏ擾.
	 *	@retval �O�ȏ� �����B���l�͍��v���p���ԁB
	 *	@retval �}�C�i�X ���s�B��񂪂Ȃ��\��������܂��B
	 */
	int GetPowerOnHoursCount(void) const
	{
		const TAttribute* p = FindAttributeId(0x09/*���v���p����*/);
		if ( p != NULL )
		{
			return ToInt(p->GetValue());
		}
		return -1;
	}

	/**
	 * [�擾] �d�������񐔎擾.
	 *	@retval �O�ȏ� �����B���l�͓d�������񐔁B
	 *	@retval �}�C�i�X ���s�B��񂪂Ȃ��\��������܂��B
	 */
	int GetPowerCycleCount(void) const
	{
		const TAttribute* p = FindAttributeId(0x0C/*�d��������*/);
		if ( p != NULL )
		{
			return ToInt(p->GetValue());
		}
		return -1;
	}

private:
	enum
	{
		DFP_GET_VERSION				= 0x00074080,
		DFP_RECEIVE_DRIVE_DATA		= 0x0007c088,
	};
	#pragma pack(push,1)
	struct TReadAttrData : SENDCMDOUTPARAMS
	{
		BYTE	bb[READ_ATTRIBUTE_BUFFER_SIZE - 1];
	};
	#pragma	pack(pop)
	// ���W
	bool m_Collect(int deviceNo)
	{
		CStr dev;
		dev.Format(_T("\\\\.\\PhysicalDrive%d"), deviceNo);
		HANDLE h = ::CreateFile(dev, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return false;
		}
		DWORD dwReturned;
		//== Version
		bool r1 = !! ::DeviceIoControl(h, DFP_GET_VERSION, NULL, 0, &m_version, sizeof(m_version), &dwReturned, NULL);
		if ( ! r1 )
		{
			::CloseHandle(h);
			return false;
		}
		m_isValidVersion = true;
		//== �����擾
		SENDCMDINPARAMS	sc;	//sSendCmd;
		TReadAttrData	ra;	//sSendCmdOutParam;
		Zero(sc);
		Zero(ra);
		sc.irDriveRegs.bFeaturesReg		= READ_ATTRIBUTES;
		sc.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;
		sc.irDriveRegs.bCylHighReg		= SMART_CYL_HI;
		sc.irDriveRegs.bDriveHeadReg	= static_cast<BYTE>(0xA0 | ((deviceNo & 1) << 4));	//�h���C�u�ԍ�
		sc.irDriveRegs.bCommandReg		= SMART_CMD;			//SMART READ DATA�R�}���h�ԍ�
		sc.cBufferSize					= READ_ATTRIBUTE_BUFFER_SIZE;
		sc.bDriveNumber					= static_cast<BYTE>(deviceNo);			//�h���C�u�ԍ�
		bool r2 = !! ::DeviceIoControl(h, DFP_RECEIVE_DRIVE_DATA, &sc, sizeof(sc), &ra, sizeof(ra), &dwReturned, NULL);
		::CloseHandle(h);
		if ( r2 && dwReturned == sizeof(TReadAttrData) )
		{
			return m_SetRowData(READ_ATTRIBUTE_BUFFER_SIZE, ra.bBuffer);
		}
		return	false;
	}
	/// ���f�[�^�Z�b�g
	bool m_SetRowData(size_t size, const void* pData)
	{
		m_infos.RemoveAll();
		if ( size != READ_ATTRIBUTE_BUFFER_SIZE)
		{
			return	false;
		}
		m_rowData.Reset(size, static_cast<const BYTE*>(pData));
		TAttribute* pAttribute = reinterpret_cast<TAttribute*>(&m_rowData[2]);
		loop ( i, 30 )
		{
			if ( pAttribute->bAttrID != 0 )
			{
				m_infos.Add(pAttribute);
			}
			pAttribute++;
		}
		return	true;
	}

	CWorkMem					m_rowData;			///< �����
	CSimpleVectorT<TAttribute*>	m_infos;			///< m_rowData �̃|�C���^�e�[�u��
	TVersion					m_version;			///< �o�[�W�������
	bool						m_isValidVersion;	///< �o�[�W�������͗L���H
};



}; // TNB



