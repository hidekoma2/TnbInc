#pragma once
/**
 *@file
 * �h���C�u�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDriveCommander.h"
#include "TnbDump.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �h���C�u���N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbDriveInformation.h
 *
 *	@date 07/02/19	�V�K�쐬
 *	@date 15/01/21	�����h���C�u�ԍ��擾���\�b�h�ǉ�
 */
class CDriveInformation
{
public:

	/// �R���X�g���N�^
	CDriveInformation(void) : m_cLetter(0)
	{
	}

	/**
	 * [�ݒ�] �f�^�b�`.
	 *		�{�C���X�^���X�ŊǗ�����h���C�u���Ȃ����܂��B
	 */
	void Detatch(void)
	{
		m_cLetter = 0;
	}

	/**
	 * [�ݒ�] �A�^�b�`.
	 *		�{�C���X�^���X�ŊǗ�����h���C�u��ݒ肵�܂��B
	 *	@param cLetter �h���C�u���^�[ 'A'�`'Z'
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool Atatch(TCHAR cLetter)
	{
		Detatch();
		if ( ExistDrive(cLetter) )
		{
			m_cLetter = cLetter;
			return true;
		}
		return false;
	}

	/**
	 * [�m�F] �L�����H.
	 *		�{�C���X�^���X�ŊǗ������Ă��邩�ۂ��A�m�F���܂��B
	 *	@retval true �Ǘ����B
	 *	@retval false �ہB
	 */
	bool IsValid(void) const
	{
		return m_cLetter != 0;
	}

	/**
	 * [�m�F] ��ފm�F.
	 *		�{�C���X�^���X�ŊǗ�����h���C�u�̎�ނ��m�F���܂��B
	 *	@retval DRIVE_UNKNOWN �h���C�u�̎�ނ𔻕ʂł��܂���ł����B 
	 *	@retval DRIVE_NO_ROOT_DIR �w��̃��[�g�f�B���N�g�������݂��܂���B���Ƃ��΁A�p�X�Ƀ{�����[�����}�E���g����Ă��܂���i���t�H�[�}�b�g��A���f�B�A���}������Ă��Ȃ��Ȃǁj�B 
	 *	@retval DRIVE_REMOVABLE ���̃f�B�X�N�́A�h���C�u������o���܂��B 
	 *	@retval DRIVE_FIXED ���̃f�B�X�N�́A�h���C�u������o���܂���B 
	 *	@retval DRIVE_REMOTE ���̃h���C�u�́A�����[�g�i�l�b�g���[�N�j�h���C�u�ł��B 
	 *	@retval DRIVE_CDROM ���̃h���C�u�́ACD-ROM �h���C�u�ł��B 
	 *	@retval DRIVE_RAMDISK ���̃h���C�u�́ARAM �f�B�X�N�ł��B 
	 */
	DWORD GetType(void) const
	{
		TCHAR a[] = { m_cLetter, ':', '\\', 0 };
		return ::GetDriveType(a);
	}

	/**
	 * [�m�F] �����h���C�u�ԍ��擾.
	 *		�{�C���X�^���X�ŊǗ�����h���C�u�̕����h���C�u�ԍ����m�F���܂�(��Ƃ͌���Ȃ�)�B
	 *	@param[out] _pdn �����h���C�u�ԍ�
	 *	@retval true �擾����
	 *	@retval false ���s
	 */
	bool GetPhysicalDriveNumber(CVectorT<int>& _pdn) const
	{
		TCHAR a[] = { m_cLetter, ':', '\\', 0 };
		return GetPhysicalDriveNumber(_pdn, a);
	}

	/**
	 * [�m�F] ���O���\���H.
	 *		�{�C���X�^���X�ŊǗ�����h���C�u�������[�o�����m�F���܂��B
	 *	@retval true ���f�B�A���O���\�B
	 *	@retval false �s�\�B
	 */
	bool IsRemoval(void) const
	{
		switch ( GetType() )
		{
		case DRIVE_REMOVABLE:	// ���̃f�B�X�N�́A�h���C�u������o���܂��B 
		case DRIVE_CDROM:	// ���̃h���C�u�́ACD-ROM �h���C�u�ł��B 
			return true;
		}
		return false;
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
		CDriveCommander scsi;	
		if ( scsi.OpenLetter(m_cLetter) )
		{
			return scsi.GetVendorId(_id, withRevision);
		}
		return false;
	}

	/**
	 * [�쐬] SCSI�R�}���_�[�쐬.
	 *		�Ǘ��h���C�u�� SCSI�R�}���_�[���쐬���܂��B
	 *	@return �R�}���_�[�B������ԂȂ�G���[�ł��B
	 */
	CDriveCommander CreateCommander(void)
	{
		CDriveCommander scsi;	
		scsi.OpenLetter(m_cLetter);
		return scsi;
	}

	/**
	 * [�m�F] �A�N�Z�X�\���H.
	 *		�{�C���X�^���X�ŊǗ�����h���C�u���A�N�Z�X�\���m�F���܂��B
	 *	@retval true �L��
	 *	@retval false ����
	 */
	bool IsReady(void) const
	{
		if ( GetType() == DRIVE_FIXED )
		{
			return true;
		}
		CDriveCommander scsi;	
		if ( scsi.OpenLetter(m_cLetter) )
		{
			scsi.Send_TestUnit();
			return scsi.Send_TestUnit() == 0;
		}
		return false;
	}
	
	/**
	 * [�m�F] �����݉\���H.
	 *		�{�C���X�^���X�ŊǗ�����h���C�u�������݉\���m�F���܂��B
	 *	@retval true ���f�B�A���O���\�B
	 *	@retval false �s�\�B
	 */
	bool CanWrite(void) const
	{
		if ( GetType() == DRIVE_FIXED )
		{
			return true;
		}
		CDriveCommander scsi;	
		if ( scsi.OpenLetter(m_cLetter) )
		{
			return scsi.CanWrite(); //���C�g�v���e�N�g
		}
		return false;	// �������߂Ȃ�
	}

	/**
	 * [����] �t�H�[�}�b�g�_�C�A���O�\��
	 *	@note http://support.microsoft.com/kb/173688/ja
	 *	@param hWnd �e�ƂȂ�E�B���h�E�n���h��
	 *	@param hasCheckDlg true �Ȃ璆�~�A�Ď��s�A������₢���킹��A�d��G���[�̃_�C�A���O �{�b�N�X��\�����܂��B\n
	 *						false �Ȃ�\�����܂���B
	 *	@retval IDOK ����
	 *	@retval 0xFFFFFFFFL �t�H�[�}�b�g�Ɏ��s
	 *	@retval 0xFFFFFFFEL �t�H�[�}�b�g�͒��~���ꂽ
	 *	@retval 0xFFFFFFFDL �h���C�u�̓t�H�[�}�b�g����Ȃ�	
	 */
	DWORD FormatDlg(HWND hWnd, bool hasCheckDlg = false) const
	{
		DWORD r = 0xFFFFFFFEL;
		if ( IsValid() )
		{
			HMODULE hInst = ::LoadLibraryA("shell32.dll");
			if ( hInst != NULL )
			{
				DWORD (WINAPI *pShFormatDrive)(HWND hWnd, UINT drive, UINT fmdID, UINT options);
				TNB::ForceSet(pShFormatDrive, ::GetProcAddress(hInst, "SHFormatDrive"));
				if ( pShFormatDrive != NULL )
				{
					UINT nowMode = ::SetErrorMode(0);
					if ( hasCheckDlg )
					{
						::SetErrorMode(nowMode | SEM_FAILCRITICALERRORS);
					}
					else
					{
						::SetErrorMode(nowMode & ~SEM_FAILCRITICALERRORS);
					}
					int d = (m_cLetter | 0x20) - 'a';
					r = (pShFormatDrive)(hWnd, d, 0xFFFF, 0x0);
					::SetErrorMode(nowMode);
				}
				::FreeLibrary(hInst);
			}
		}
		return r;
	}

	/**
	 * [�m�F] �h���C�u���݊m�F.
	 *		�w��̃h���C�u�����݂��邩�m�F
	 *	@param cLetter �h���C�u���^�[ 'A'�`'Z'
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	static bool ExistDrive(TCHAR cLetter)
	{
		int r = (cLetter | 0x20) - 'a';
		if ( r >= 0 && r <= 25 )
		{
			DWORD d = ::GetLogicalDrives();
			return (d & _BIT(r)) != 0;
		}
		return false;
	}

	/**
	 * [�擾] �h���C�u���^�[�ꗗ.
	 *	@return ���݂���h���C�u���^�[�ꗗ
	 */
	static CVectorT<TCHAR> EnumDrive(void)
	{
		CVectorT<TCHAR> r;
		DWORD d = ::GetLogicalDrives();
		loop ( i, 26 )
		{
			if ( (d & _BIT(i)) != 0 )
			{
				r.Add(static_cast<TCHAR>(_T('A') + i));
			}
		}
		return r;
	}

	/**
	 * [�m�F] �����h���C�u�ԍ��擾.
	 *		�w��p�X�̕����h���C�u�ԍ����m�F���܂�(��Ƃ͌���Ȃ�)�B
	 *	@param[out] _pdn �����h���C�u�ԍ�
	 *	@param[in] lpszPath �p�X
	 *	@retval true �擾����
	 *	@retval false ���s
	 */
	static bool GetPhysicalDriveNumber(CVectorT<int>& _pdn, LPCTSTR lpszPath)
	{
		_pdn.RemoveAll();
		CStr s1;
		if ( ! ::GetVolumePathName(lpszPath, s1.GetBuffer(1024), 1024) )
		{
			return false;
		}
		s1.ReleaseBuffer();
		CStr vol;
		if ( ! ::GetVolumeNameForVolumeMountPoint(s1, vol.GetBuffer(1024), 1024) )
		{
			return false;
		}
		vol.ReleaseBuffer();
		vol.TrimRight('\\');
		HANDLE h = ::CreateFile(vol, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		VOLUME_DISK_EXTENTS buf;
		DWORD returnedSize = 0;
		BOOL r = ::DeviceIoControl(
				h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &buf, sizeof(buf), &returnedSize, NULL);
		DWORD le = _GetLastError("DeviceIoControl");
		if ( r && buf.NumberOfDiskExtents == 1 )
		{
			_pdn.Add(buf.Extents[0].DiskNumber);
			::CloseHandle(h);
			return true;
		}
		if ( le != ERROR_MORE_DATA ) 
		{
			::CloseHandle(h);
			return false;
		}
		size_t bs = sizeof(buf.NumberOfDiskExtents) + sizeof(buf.Extents) * buf.NumberOfDiskExtents;
		CWorkMem w(bs);
		VOLUME_DISK_EXTENTS* p = reinterpret_cast<VOLUME_DISK_EXTENTS *>(w.Ref());
		r = ::DeviceIoControl(
				h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, p, bs, &returnedSize, NULL);
		le = _GetLastError("DeviceIoControl");
		if ( r )
		{
			loop ( i, p->NumberOfDiskExtents )
			{
				_pdn.Add(p->Extents[i].DiskNumber);
			}
		}
		::CloseHandle(h);
		return !! r;
	}

private:
	TCHAR		m_cLetter;	///< �Ǘ��h���C�u���^�[�B�O�Ȃ疢�Ǘ��B
};



}; // TNB
