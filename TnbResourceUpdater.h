#pragma once
/**
 *@file
 * ���\�[�X�X�V�֌W�̃w�b�_
 *
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbFile.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// �A�C�R���t�@�C���N���X
class CIconFile
{
	#pragma pack( push )
	#pragma pack( 1 )

	/// �A�C�R���t�@�C���\��
	struct ICONDIR
	{
		WORD		idReserved;		///< �\��i�O�j
		WORD		idType;			///< �^�C�v�i�A�C�R�����P)
		WORD		idCount;		///< �C���[�W��
		//    ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
	};
	/// �A�C�R���e����
	struct ICONDIRENTRY
	{
		BYTE		bWidth;			///< Width, in pixels, of the image
		BYTE		bHeight;		///< Height, in pixels, of the image
		BYTE		bColorCount;	///< Number of colors in image (0 if >=8bpp)
		BYTE		bReserved;		///< Reserved ( must be 0)
		WORD		wPlanes;		///< Color Planes
		WORD		wBitCount;		///< Bits per pixel
		DWORD		dwBytesInRes;	///< How many bytes in this resource?
		DWORD		dwImageOffset;	///< Where in the file is this image?
	};

	/// 
	struct GRPICONDIRENTRY
	{
	   BYTE   bWidth;               // Width, in pixels, of the image
	   BYTE   bHeight;              // Height, in pixels, of the image
	   BYTE   bColorCount;          // Number of colors in image (0 if >=8bpp)
	   BYTE   bReserved;            // Reserved
	   WORD   wPlanes;              // Color Planes
	   WORD   wBitCount;            // Bits per pixel
	   DWORD   dwBytesInRes;         // how many bytes in this resource?
	   WORD   nID;                  // the ID
	};
	#pragma pack( pop )

	ICONDIR					m_iconDir;			// �A�C�R���f�B���N�g��
	CWorkMemT<ICONDIRENTRY>	m_iconEntrys;		// �A�C�R���f�B���N�g���G���g��
	CWorkMemT<CWorkMem>		m_iconImages;		///< �C���[�W�O���[�v�f�[�^
	CWorkMem				m_iconGroupData;	///< �C���[�W�O���[�v�f�[�^

public:

	// �R���X�g���N�^
	CIconFile()
	{
		Zero(m_iconDir);
	}

	// �A�C�R���̎��C���[�W�̐����擾����
	size_t GetImageCount() const
	{
		return m_iconDir.idCount;
	}

	// index�Ԗڂ̃C���[�W�f�[�^���擾����
	BYTE* GetImageData(INDEX index)
	{
		ASSERT( index < GetImageCount());
		return m_iconImages[index].Ref();
	}

	// index�Ԗڂ̃C���[�W�T�C�Y���擾����
	DWORD GetImageSize(INDEX index) const
	{
		ASSERT(index < GetImageCount());
		return m_iconEntrys[index].dwBytesInRes;
	}

	// �A�C�R���t�@�C�������[�h����
	bool Load(LPCTSTR pszFileName)
	{
		// �A�C�R���t�@�C���̃I�[�v��
		HANDLE hFile = ::CreateFile(pszFileName, GENERIC_READ,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		bool r = (hFile != INVALID_HANDLE_VALUE);
		DWORD dwRead = 0;
		// ICONDIR�����̓ǂݍ���
		r &= !! ::ReadFile(hFile, &m_iconDir, sizeof(ICONDIR), &dwRead, NULL);
		ASSERT( m_iconDir.idReserved == 0 );
		ASSERT( m_iconDir.idType == 1 );
		ASSERT( m_iconDir.idCount > 0 );
		// ICONDIRENTRY�̓ǂݍ���
		m_iconEntrys.Resize(m_iconDir.idCount);
		loop ( i, m_iconDir.idCount )
		{
			r &= !! ::ReadFile(hFile, &(m_iconEntrys[i]), sizeof(ICONDIRENTRY), &dwRead, NULL);
		}
		// �C���[�W�f�[�^�̓ǂݍ���
		m_iconImages.Resize(m_iconDir.idCount);
		loop ( i, m_iconDir.idCount )
		{
			::SetFilePointer(hFile, m_iconEntrys[i].dwImageOffset, NULL, FILE_BEGIN);
			m_iconImages[i].Resize(m_iconEntrys[i].dwBytesInRes);
			r &= !! ::ReadFile(hFile, m_iconImages[i], m_iconEntrys[i].dwBytesInRes, &dwRead, NULL);
		}
		::CloseHandle(hFile);
		return r;
	}

	// �A�C�R���O���[�v�f�[�^�̃T�C�Y�擾
	size_t SizeOfIconGroupData() const
	{
		return sizeof(ICONDIR) + sizeof(GRPICONDIRENTRY) * GetImageCount();
	}

	// �A�C�R���O���[�v�f�[�^�̍쐬�inBaseID��RT_ICON�̊��C���f�b�N�X�ԍ��j
	BYTE* CreateIconGroupData(int nBaseID)
	{
		// �A�C�R���O���[�v�f�[�^�̗̈�m��
		m_iconGroupData.Resize(SizeOfIconGroupData());
		// �A�C�R���O���[�v�f�B���N�g������������
		memcpy(m_iconGroupData, &m_iconDir, sizeof(ICONDIR));
		int offset = sizeof(ICONDIR);
		loop ( i, GetImageCount() )
		{
			GRPICONDIRENTRY grpEntry;
			// �C���[�W�f�[�^����BITMAPINFOHEADER�擾
			BITMAPINFOHEADER bitmapheader;
			memcpy(&bitmapheader, GetImageData(i), sizeof(BITMAPINFOHEADER));
			// �A�C�R���O���[�v�G���g���쐬
			grpEntry.bWidth			= m_iconEntrys[i].bWidth;
			grpEntry.bHeight		= m_iconEntrys[i].bHeight;
			grpEntry.bColorCount	= m_iconEntrys[i].bColorCount;
			grpEntry.bReserved		= m_iconEntrys[i].bReserved;
			grpEntry.wPlanes		= bitmapheader.biPlanes;
			grpEntry.wBitCount		= bitmapheader.biBitCount;
			grpEntry.dwBytesInRes	= m_iconEntrys[i].dwBytesInRes;
			grpEntry.nID			= static_cast<WORD>(nBaseID + i);
			// �A�C�R���O���[�v�G���g������������
			memcpy(m_iconGroupData.Ref() + offset, &grpEntry, sizeof(GRPICONDIRENTRY));
			offset += sizeof(GRPICONDIRENTRY);
		}
		return m_iconGroupData.Ref();
	}
};

#endif // _TnbDOXYGEN



/**@ingroup FILE
 * ���\�[�X�X�V�֌W�N���X.
 *
 *	�t�@�C���ɕt�����Ă��郊�\�[�X���X�V�ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbResourceUpdater.h
 *
 *	@date 08/08/04 �V�K�쐬
 */
class CResourceUpdater : CCopyImpossible
{
	HANDLE		m_hUpdate;	///< �X�V�p�n���h��

	/// �N���[�Y
	bool m_Close(bool isDiscard)
	{
		bool r = false;
		if ( m_hUpdate != NULL )
		{
			r = !! ::EndUpdateResource(m_hUpdate, isDiscard);
			m_hUpdate = NULL;
		}
		return r;
	}
public:

	/// �R���X�g���N�^
	CResourceUpdater(void) : m_hUpdate(NULL)
	{
	}

	/// �f�X�g���N�^
	~CResourceUpdater(void)
	{
		m_Close(true);
	}

	/**
	 * [�m�F] �L����
	 *	@retval true Open�ɐ������Ă���
	 *	@retval false Open���Ă��Ȃ��B
	 */
	bool IsValid(void) const
	{
		return m_hUpdate != NULL;
	}

	/**
	 * [�ݒ�] �I�[�v��
	 *	@param lpszFile �Ώۂ̃t�@�C�����B
	 *	@param isDeleteExistingResources �Ώۂ̃t�@�C���̃p�����[�^�̊����̃��\�[�X���폜���邩�ǂ����w�肵�܂��B
	 *					true ��n���ƁA�����̃��\�[�X���폜����A�X�V��̎��s�\�t�@�C���ɂ͒ǉ��������\�[�X�������܂܂�܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(LPCTSTR lpszFile, bool isDeleteExistingResources = false)
	{
		m_Close(true);
		m_hUpdate = ::BeginUpdateResource(lpszFile, isDeleteExistingResources);
		return IsValid();
	}

	/**
	 * [�ݒ�] ��������
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool WritePush(void)
	{
		return m_Close(false);
	}

	/**
	 * [�ύX] �A�C�R���ύX
	 *	@param resourceId ���\�[�XID
	 *	@param lpszIconFile �A�C�R���t�@�C��
	 *	@param wLanguage ����ID
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool ModifyIcon(UINT resourceId, LPCTSTR lpszIconFile, WORD wLanguage = 0)
	{
		bool r = false;
		if ( IsValid() )
		{ 
			CIconFile iconFile;
			r = iconFile.Load(lpszIconFile);
			// RT_GROUP_ICON ��������
			r &= !! ::UpdateResource
				(
					m_hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(resourceId), wLanguage,
					iconFile.CreateIconGroupData(resourceId), ToDword(iconFile.SizeOfIconGroupData())
				);
			// RT_ICON��������
			loop ( i, iconFile.GetImageCount() )
			{
				r &= !! ::UpdateResource
					(
						m_hUpdate, RT_ICON, MAKEINTRESOURCE(resourceId+i), wLanguage,
						iconFile.GetImageData(i), iconFile.GetImageSize(i)
					);
			}
		}
		return r;
	}
};



}; // TNB



