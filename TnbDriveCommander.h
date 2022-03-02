#pragma once
/**
 *@file
 * �h���C�u�R�}���_�[�֌W�̃w�b�_
 *
 *		SCSI�R�}���h�ɂăh���C�u�𑀍삷�邱�Ƃ��ł��܂��B
 *
 *	@note Windows NT�n�� SPTI (SCSI PASS THROUGH INTERFACE) ���g�p���Ă��܂��B���̂��߁A Win9x�n OS�ł͓����܂���B
 *
 *	@note �{�@�\���g�p����ɂ́AWindows SDK ���K�v�ɂȂ�܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include <ntddscsi.h>	// SDK
#include <WinIoCtl.h>	// SDK
#include "TnbPointerHandle.h"
#include "TnbHandleHandle.h"
#include "TnbCollection.h"
#include "TnbWriter.h"



//TNB Library
namespace TNB
{



/**@ingroup ETC
 * �h���C�u�R�}���_�[�N���X
 *	
 *		IWriter �C���^�[�t�F�[�X�������Ă��܂��B�܂��A SCSI�R�}���h�ɂăf�o�C�X�𑀍삷�邱�Ƃ��ł��܂��B
 *
 *	@note	�w�肵���������𒼐ڎg�킸�A��x�����Ŋm�ۂ������������g�p���Ă��܂��B
 *			���̂��߁A�w�肷�郁�����ɐ����͂���܂��񂪁A�I�[�o�[�w�b�h������܂��B
 *
 *	@note Windows NT�n�� SPTI (SCSI PASS THROUGH INTERFACE) ���g�p���Ă��܂��B���̂��߁A Win9x�n OS�ł͓����܂���B
 *
 *	@note CDB �� LUN�A�^�O�͖�������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDriveCommander.h
 *
 *	@date 07/02/15	�V�K�쐬
 *	@date 15/01/21	�����h���C�u�ԍ��ɂ��I�[�v���ǉ��B
 *	@date 15/01/28	�N���X���ύX�B IWriter �����B
 *	@date 15/06/03	IsOpened() �̔���o�O�C���B
 */
class CDriveCommander : public IWriter
{
public:
	
	/**@ingroup ETC
	 * SCSI �Z���X�f�[�^�^
	 *	@note GetSense() �̖߂�l�Ŏg�p����Ă��܂��B
	 */
	struct TSenseData
	{
		BYTE KEY;		///< SCSI Sense Key
		BYTE ASC;		///< SCSI Additional Sense Code
		BYTE ASCQ;		///< SCSI Additional Sense Code Qualifier
		/// [����] �S���N���A
		void Clear(void) {
			KEY = ASC = ASCQ = 0;
		}
	};


	//-----


	/// �R���X�g���N�^
	CDriveCommander(void) : m_timeout(2)
	{
		m_sense.Clear();
	}

	/**
	 * [�m�F] �I�[�v���ς݁H
	 *	@retval true �I�[�v���ς�
	 *	@retval false ��
	 */
	bool IsOpened(void) const
	{
		return ! m_hhDrive.IsNull();
	}

	/// [����] �N���[�Y
	virtual void Close(void)
	{
		m_hhDrive.Null();
	}

	/**
	 * [����] �I�[�v��.
	 *	@note �{�C���X�^���X�Ŏg�p�������h���C�u���w�肵�܂��B 
	 *	@param driveNo �h���C�u�ԍ�
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool OpenDrive(DWORD driveNo)
	{
		Close();
		m_hhDrive = ::CreateFile(CStr::Fmt(_T("\\\\.\\PhysicalDrive%d"), driveNo),
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								0, NULL);
		return ! m_hhDrive.IsNull();
	}

	/**
	 * [����] �I�[�v��.
	 *	@note �{�C���X�^���X�Ŏg�p�������h���C�u���w�肵�܂��B 
	 *	@param driveLetter �h���C�u���^�[�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool OpenLetter(TCHAR driveLetter)
	{
		Close();
		m_hhDrive = ::CreateFile(CStr::Fmt(_T("\\\\.\\%c:"), driveLetter),
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								0, NULL);
		return ! m_hhDrive.IsNull();
	}

	/**
	 * [�擾] �Z���X�f�[�^�擾
	 *	@note ���̃��\�b�h�́A Send�`()�� �߂�l���Q(CheckCodition)�̎��A�g�p���܂��B
	 *	@return �Z���X�f�[�^�B
	 */
	const TSenseData& GetSense(void) const
	{
		return m_sense;
	}

	/**
	 * [�ݒ�] �A�b�v�f�[�g.
	 *	@retval true ����
	 *	@retval false ��
	 */
	bool Update()
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [�ݒ�] Dismount Volume.
	 *	@retval true ����
	 *	@retval false ��
	 */
	bool DismountVolume()
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [�ݒ�] Lock Volume.
	 *	@retval true ����
	 *	@retval false ��
	 */
	bool LockVolume(void)
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [�ݒ�] Unlock Volume.
	 *	@retval true ����
	 *	@retval false ��
	 */
	bool UnlockVolume(void)
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [���M] CDB���M.
	 *	@param cdb CDB�B LUN �G���A�͖�������܂��B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int SendCommand(const ISequenceCollectionT<BYTE>& cdb)
	{
		if ( m_hhDrive.IsNull() ) 
		{
			::SetLastError(ERROR_ACCESS_DENIED);
			return -1; 
		}
		CCdbInfo tmp(0, SCSI_IOCTL_DATA_UNSPECIFIED); //SCSI_IOCTL_DATA_IN
		tmp.SetCdb(cdb, m_timeout);
		return m_SendCdb(tmp);
	}

	/**
	 * [���M] CDB���M(�����݃t�F�[�Y�t).
	 *	@param cdb CDB�B LUN �G���A�͖�������܂��B
	 *	@param data �����݃f�[�^(CDB�ɐ������f�[�^�T�C�Y�����w�肵�Ă�������)�B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int SendCommandWithWritePhase(
			const ISequenceCollectionT<BYTE>& cdb, const ISequenceCollectionT<BYTE>& data)
	{
		if ( m_hhDrive.IsNull() ) 
		{
			::SetLastError(ERROR_ACCESS_DENIED);
			return -1; 
		}
		size_t l = data.GetSize();
		CCdbInfo tmp(l, static_cast<BYTE>((l > 0) ? SCSI_IOCTL_DATA_OUT : SCSI_IOCTL_DATA_UNSPECIFIED));
		tmp.SetCdb(cdb, m_timeout);
		if ( l > 0 )
		{
			MemCopy(tmp.RefData(), data.ReferBuffer(), l);
		}
		return m_SendCdb(tmp);
	}

	/**
	 * [���M] CDB���M(�Ǎ��݃t�F�[�Y�t).
	 *	@param[out] _d �ǂݍ��݃f�[�^�B
	 *	@param[in] cdb CDB�B LUN �G���A�͖�������܂��B
	 *	@param[in] readSize �ǂݍ��݃f�[�^��(CDB�ɐ������ǂݍ��݃f�[�^�T�C�Y���w�肵�Ă�������)�B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int SendCommandWithReadPhase(ICollectionT<BYTE>& _d,
					const ISequenceCollectionT<BYTE>& cdb, int readSize)
	{
		if ( m_hhDrive.IsNull() ) 
		{
			::SetLastError(ERROR_ACCESS_DENIED);
			return -1; 
		}
		CCdbInfo tmp(readSize, SCSI_IOCTL_DATA_IN);
		tmp.SetCdb(cdb, m_timeout);
		int r = m_SendCdb(tmp);
		if ( r == 0 )
		{
			_d.CopyElements(readSize, tmp.RefData());
		}
		return r;
	}

	/**
	 * [���M] MODESENSE CDB���M.
	 *	@note Mode Parameter Header �� 2byte�ڂ� Media ID�A3Byte�ڂ̍ŏ��Bit�� WriteProtect ��Ԃł��i1�Ȃ� WP�j�B
	 *	@param[out] _d �ǂݍ��݃f�[�^�B
	 *	@param[in] pc �y�[�W�R�[�h�A����сA�y�[�W�R���g���[���l�B
	 *	@param[in] isWithBd true �Ȃ� Block Descriptor ���ǂݍ��݂܂��B �ȗ��� false �Ȃ� �ǂݍ��݂܂���B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int Send_ModeSense(ICollectionT<BYTE>& _d, BYTE pc, bool isWithBd = false)
	{
		BYTE cdb[6] = {0x1A/*ModeSense*/, _BIT(3)/*Block Descriptor �]���Ȃ�*/, pc, 0, 4, 0};
		int r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		if ( r == 0 && _d.GetSize() >= 4 )
		{
			cdb[1] = static_cast<BYTE>((isWithBd && _d.At(3) == 8) ? 0 : _BIT(3));
			cdb[4] = static_cast<BYTE>(4 + _d.At(3) + _d.At(0));
			r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		}
		return r;
	}

	/**
	 * [���M] INQUIRY CDB���M.
	 *	@param[out] _d �ǂݍ��݃f�[�^�B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int Send_Inquiry(ICollectionT<BYTE>& _d)
	{
		BYTE cdb[6] = {0x12/*Inquiry*/, 0, 0, 0, 8, 0};
		int r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		if ( r == 0 )
		{
			cdb[4] = static_cast<BYTE>(5 + _d.At(4));
			r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		}
		return r;
	}

	/**
	 * [���M] TEST UNIT CDB���M.
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int Send_TestUnit(void)
	{
		BYTE cdb[6] = {0/*TestUnit*/, 0, 0, 0, 0, 0};
		return SendCommand(CArrayAdapterT<BYTE>(cdb));
	}

	/**
	 * [���M] STARTSTOP CDB���M.
	 *	@param b �t���O�B 2 �Ȃ� Eject�A0 �Ȃ� Load ���܂��B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int Send_StartStop(BYTE b = 2)
	{
		BYTE cdb[6] = {0x1B/*StartStop*/, 1, 0, 0, b, 0};
		return SendCommand(CArrayAdapterT<BYTE>(cdb));
	}

	/**
	 * [���M] Read Capacity CDB���M.
	 *	@param[out] _blockAddress �������A�Ō�̃u���b�N�A�h���X���i�[����܂��B
	 *	@param[out] _blockLength �������A�u���b�N�����i�[����܂��B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int Send_ReadCapacity(DWORD& _blockAddress, DWORD& _blockLength)
	{
		BYTE cdb[10] = {0x25/*ReadCapacity*/, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		CByteVector vb;
		int r = SendCommandWithReadPhase(vb, CArrayAdapterT<BYTE>(cdb), 8);
		if ( r == 0 )
		{
			BYTE* p = reinterpret_cast<BYTE*>(vb.GetBuffer());
			_blockAddress = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
			_blockLength = p[4] << 24 | p[5] << 16 | p[6] << 8 | p[7];
		}
		return r;
	}

	/**
	 * [���M] Write(10) CDB���M.
	 *	@param ba �u���b�N�A�h���X
	 *	@param d �f�[�^�B������ div �̔{���ł���K�v������܂��B
	 *	@param div �u���b�N���B�ʏ� 512 �ł��B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int Send_Write10(DWORD ba, const ISequenceCollectionT<BYTE>& d, DWORD div = 512)
	{
		BYTE cdb[10] = {0x2A/*Write10*/, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		cdb[2] = static_cast<BYTE>(ba >> 24);
		cdb[3] = static_cast<BYTE>(ba >> 16);
		cdb[4] = static_cast<BYTE>(ba >> 8);
		cdb[5] = static_cast<BYTE>(ba);
		DWORD l = d.GetSize() / div;
		cdb[7] = static_cast<BYTE>(l >> 8);
		cdb[8] = static_cast<BYTE>(l);
		int r = SendCommandWithWritePhase(CArrayAdapterT<BYTE>(cdb), d);
		return r;
	}

	/**
	 * [�擾] �n���h���擾.
	 *	@return �n���h��
	 */ 
	HANDLE GetSafeHandle(void) const
	{
		return m_hhDrive;
	}

	/**
	 * [�擾] �x���_ID �擾.
	 *	@param[out] _id �擾�����x���_ID���i�[����܂��B
	 *	@param[in] withRevision true �Ȃ�Ő����܂�ID��Ԃ��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetVendorId(CStr& _id, bool withRevision = false) const
	{
		CDriveCommander* p = const_cast<CDriveCommander*>(this);
		CByteVector vb;
		p->Send_TestUnit();	// ��Ǎ���
		if ( p->Send_Inquiry(vb) == 0 && vb.GetSize() >= 36 )
		{
			const char* p = reinterpret_cast<const char*>(vb.ReferBuffer());
			CAscii a1;
			a1.SetFromLeft(&p[8], 8);
			_id = a1;
			a1.SetFromLeft(&p[16], 16);
			_id += a1;
			if ( withRevision )
			{
				a1.SetFromLeft(&p[32], 4);
				_id += a1;
			}
			return true;
		}
		return false;
	}

	/**
	 * [�擾] �I�[�v�����擾.
	 *		�x���_ID���Ԃ���܂��B
	 *	@return �x���_ID
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		m_vendorId.Empty();
		GetVendorId(m_vendorId);
		return m_vendorId;
	}

	/**
	 * [�m�F] �ǂݍ��݉\���H
	 *	@note TESTUNIT �ŏ�Ԃ��m�F���Ă��܂��B
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		CDriveCommander* p = const_cast<CDriveCommander*>(this);
		return (p->Send_TestUnit()) == 0;
	}

	/**
	 * [�m�F] �����݉\���H
	 *	@note MODESENSE �� WP �̃r�b�g���m�F���Ă��܂��B
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanWrite(void) const
	{
		CDriveCommander* p = const_cast<CDriveCommander*>(this);
		CByteVector vb;
		if ( CanRead() && p->Send_ModeSense(vb, 0x3F) == 0 && vb.GetSize() >= 4 )
		{
			//TNB::DebugDump(vb.GetSize(), vb.ReferBuffer());
			// vb[1]; //Media ID
			// vb[2]; // 3Byte�ڂ̍ŏ��Bit�� WriteProtect ��Ԃł��i1�Ȃ� WP�j�B
			return (vb[2] & _BIT(7)) == 0;
		}
		return false;
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *	@note IOCTL_DISK_GET_DRIVE_GEOMETRY_EX ���g���A�T�C�Y�𓾂Ă��܂��B
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const 
	{
		CWorkMem w(sizeof(DISK_GEOMETRY_EX) + sizeof(DISK_PARTITION_INFO) + sizeof(DISK_DETECTION_INFO));
		w.Fill(0);
		DWORD returnedBytes = 0;
		BOOL r = ::DeviceIoControl(
				GetSafeHandle(), IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, w.Ref(), w.GetSize(), &returnedBytes, NULL);
		DISK_GEOMETRY_EX* pGeometry = reinterpret_cast<DISK_GEOMETRY_EX*>(w.Ref());
		//DISK_PARTITION_INFO* pPartition= DiskGeometryGetPartition(pGeometry);
		//DISK_DETECTION_INFO* pDetection = DiskGeometryGetDetect(pGeometry);
		LONGLONG ll = pGeometry->DiskSize.QuadPart;
		return r ? ll : -1;
	}

	/**
	 * [�擾] GEOMETRY �擾.
	 *	@note IOCTL_DISK_GET_DRIVE_GEOMETRY_EX ���g���A�T�C�Y�𓾂Ă��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetGeometry(DISK_GEOMETRY& _g) const 
	{
		CWorkMem w(sizeof(DISK_GEOMETRY_EX) + sizeof(DISK_PARTITION_INFO) + sizeof(DISK_DETECTION_INFO));
		w.Fill(0);
		DWORD returnedBytes = 0;
		BOOL r = ::DeviceIoControl(
				GetSafeHandle(), IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, w.Ref(), w.GetSize(), &returnedBytes, NULL);
		DISK_GEOMETRY_EX* pGeometry = reinterpret_cast<DISK_GEOMETRY_EX*>(w.Ref());
		_g = pGeometry->Geometry;
		return !! r;
	}

	/**
	 * [����] �V�[�N.
	 *		�A�N�Z�X�|�C���^���ړ����܂��B
	 *	@attention 512 bytes �P�ʂł̈ړ�������悤�ɂ��Ă��������B
	 *	@param llOffset �A�N�Z�X�|�C���^�̈ړ����B
	 *	@param seek �ړ��J�n�A�N�Z�X�|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode seek = TOP) const 
	{
		if ( seek == END )
		{
			llOffset += GetSize();
			seek = TOP;
		}
		LARGE_INTEGER r;
		r.QuadPart = llOffset;
		r.LowPart = ::SetFilePointer(GetSafeHandle(), r.LowPart, &r.HighPart, seek);
		if ( r.LowPart == INVALID_SET_FILE_POINTER )
		{
			if ( ::GetLastError() != NO_ERROR )
			{
				r.QuadPart = -1;
			}
		}
		return r.QuadPart;
	}

	/**
	 * [�擾] �ǂݍ���
	 *	@attention 512 bytes �P�ʂł̃A�N�Z�X������悤�ɂ��Ă��������B
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		DWORD dwRead;
		BOOL boRc = ::ReadFile(GetSafeHandle(), _P, ToDword(size), &dwRead, NULL);
		if ( ! boRc )
		{
			throw CReadFailureException(::GetLastError());
		}
		return dwRead;
	}

	/**
	 * [�ۑ�] ��������
	 *	@attention 512 bytes �P�ʂł̃A�N�Z�X������悤�ɂ��Ă��������B
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		if ( size >= ULONG_MAX )
		{
			throw CWriteFailureException(ERROR_BAD_LENGTH);
		}
		DWORD dwWrite;
		bool boRc = !! ::WriteFile(GetSafeHandle(), P, ToDword(size), &dwWrite, NULL);
		DWORD dwError = boRc ? 0 : ::GetLastError();
		if ( ! boRc || size != dwWrite )
		{
			throw CWriteFailureException(dwError);
		}
	}
	
	/**
	 * [�ݒ�] EOF���w��.
	 *		�K�����s���܂��B�B
	 *	@throw CNotSupportException ���s
	 */
	virtual void SetEnd(void)
	{
		throw CNotSupportException();
	}

private:

	/// IOCTL_SCSI_PASS_THROUGH �̃p�����[�^�^
	struct TScsiPassThroughEx
	{
		SCSI_PASS_THROUGH	spt;		///< ��{�p�����[�^�^
		DWORD				boundary;	///< �o�E���_��
		BYTE				abSense[32];///< �Z���X���p
	};

	/// CDB ���Ǘ��N���X
	class CCdbInfo
	{
		CWorkMem	m_work;			///< ���[�N������
		size_t		m_size;			///< �T�C�Y
	public:
		/// �R���X�g���N�^
		CCdbInfo(size_t size, BYTE bMode) : m_work(0)
		{
			m_size = sizeof(TScsiPassThroughEx) + size;
			m_work.Resize(m_size);
			TScsiPassThroughEx* P = reinterpret_cast<TScsiPassThroughEx*>(m_work.Ref());
			::ZeroMemory(P, m_size);
			P->spt.Length				= sizeof(SCSI_PASS_THROUGH);
//			P->spt.SenseInfoLength		= 18;
			P->spt.SenseInfoLength		= 32;
			P->spt.DataIn				= bMode;
			P->spt.DataTransferLength	= static_cast<ULONG>(size);
			P->spt.TimeOutValue			= 2;
			if ( size != 0 )
			{
				P->spt.DataBufferOffset	= sizeof(TScsiPassThroughEx);
			}
			P->spt.SenseInfoOffset		= offsetof(TScsiPassThroughEx, abSense);
		}
		/// �Z�b�g
		void SetCdb(const ISequenceCollectionT<BYTE>& cdb, DWORD timeout)
		{
			TScsiPassThroughEx* P = reinterpret_cast<TScsiPassThroughEx*>(m_work.Ref());
			P->spt.CdbLength = static_cast<BYTE>(cdb.GetSize());
			MemCopy(P->spt.Cdb, cdb.ReferBuffer(), cdb.GetSize());
			P->spt.TimeOutValue = timeout;
		}
		/// �Q��
		TScsiPassThroughEx* Ref(void)
		{
			return reinterpret_cast<TScsiPassThroughEx*>(m_work.Ref());
		}
		/// �f�[�^�Q��
		BYTE* RefData(void)
		{
			return m_work.Ref() + sizeof(TScsiPassThroughEx);
		}
		/// CDB�T�C�Y
		size_t GetLength(void)
		{
			return m_size;
		}
	};

	/// �A�_�v�^�N���X
	template<typename TYP>
	class CArrayAdapterT : public ISequenceCollectionT<TYP>
	{
		DEFSUPER(ISequenceCollectionT<TYP>);
		const TYP*			m_pcType;			///< �ێ�����f�[�^�̃|�C���^
		size_t				m_size;				///< �ێ�����f�[�^�̒���
	public:
		/// �R���X�g���N�^
		template<typename T> CArrayAdapterT(const T& t)
			: _super(), m_pcType(static_cast<const TYP*>(t)), m_size(sizeof(T) / sizeof(TYP)) {}
		/// �v�f���擾
		virtual size_t GetSize(void) const { return m_size; }
		/// �f�[�^�A�h���X�擾.
		virtual const TYP* ReferBuffer(void) const { return m_pcType; }
	};

	/**
	 * [���M] CDB���M.
	 *	@param cdb CDB Info�B
	 *	@retval 0 ����
	 *	@retval 1�ȏ� SCSI�X�e�[�^�X�B �Q�Ȃ�Check Condition( GetSense() �ŃZ���X��񂪓����܂�)�B�W�Ȃ�Busy�B
	 *	@retval -1 ���s�B ::GetLastError() �ŃG���[���e���m�F�ł��܂��B
	 */
	int m_SendCdb(CCdbInfo& cdb)
	{
		DWORD dwResult = 0;
		TScsiPassThroughEx* P = cdb.Ref();
		BOOL r = ::DeviceIoControl(m_hhDrive,
								IOCTL_SCSI_PASS_THROUGH,
								P,
								cdb.GetLength(),
								P,
								cdb.GetLength(),
								&dwResult,
								NULL);
		m_sense.Clear();
		if ( r && P->spt.ScsiStatus != 0 )
		{
			m_sense.KEY = static_cast<BYTE>(P->abSense[2] & 0x0F);
			if ( P->abSense[7] >= 0x0A )
			{
				m_sense.ASC = P->abSense[12];
				m_sense.ASCQ = P->abSense[13];
			}
			return P->spt.ScsiStatus;
		}
		return r ? 0 : -1;
	}

	CHandleHandle	m_hhDrive;		///< �h���C�u�n���h��
	TSenseData		m_sense;		///< Rast�Z���X���
	DWORD			m_timeout;		///< �^�C���A�E�g����
	mutable CStr	m_vendorId;		///< �x���_ID
};



}; // TNB



#if 0

// ����
 http://support.microsoft.com/kb/871134/ja
 http://support.microsoft.com/kb/137247/ja

#endif