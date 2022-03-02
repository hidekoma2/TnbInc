#pragma once
/**
 *@file
 * �e���|�����t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �e���|�����t�@�C���Ǘ��e���v���[�g�N���X
 *
 *		�������̓��e���ꎞ�I�Ƀt�@�C���ɂ��đ��삷�邽�߂̃N���X�ł��B
 *		�w��̃f�[�^���e�̃t�@�C�����e���|�����ɍ쐬���A�s�K�v���i�f�X�g���N�^����A
 *		Free() , Release() ���j�Ƀt�@�C�����폜���܂��B
 *
 *	@note ��C���X�^���X�ɕt���A�Ǘ�����t�@�C���͈�����ł��B
 *
 *	@param PFIX �e���|�����t�@�C���̃v���t�B�b�N�X�B�ʏ�ꕶ����ASCII�R�[�h���w�肵�܂��B
 *				�ő�S�����܂ŋL�q�\�ł����B���Ԃ��t�ɂȂ邱�Ƃɒ��ӁB\n
 *				ex) CTemporaryFileT<'ABCD'> �Ȃ�A'�`��DCBA0000.tmp' �Ƃ����t�@�C�����ɂȂ�܂��B 
 *
 *	@par�K�v�t�@�C��
 *			TnbTemporaryFile.h
 *
 *	@date 06/12/13 �V�K�쐬
 *	@date 08/10/30 SetTempraryFolder() �V�K�ǉ��B
 */
template<DWORD PFIX>
class CTemporaryFileT : CCopyImpossible
{
	CStr		m_tempFolder;	///< �Ώۃt�H���_��
	CStr		m_targetFile;	///< �Ǘ��t�@�C����
	bool m_Create(void)
	{
		Free();
		CStr strPath;
		if ( m_tempFolder.IsEmpty() )
		{
			::GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));
			strPath.ReleaseBuffer();
		}
		else
		{
			strPath = m_tempFolder;
		}
		const static TCHAR tc[5] = { (PFIX&0xFF), ((PFIX>>8)&0xFF), ((PFIX>>16)&0xFF), ((PFIX>>24)&0xFF), 0 };
		::GetTempFileName(strPath, tc, 0, m_targetFile.GetBuffer(MAX_PATH));
		m_targetFile.ReleaseBuffer();
		return ! m_targetFile.IsEmpty();
	}

public:

	/// �R���X�g���N�^
	CTemporaryFileT(void)
	{
	}

	/**
	 * �f�X�g���N�^.
	 *		�쐬�����e���|�����t�@�C��������΍폜���܂��B
	 *	@note �폜�Ɏ��s����\��������̂ŁA��� Free() �Ȃǂ����Ă������Ƃ������߂��܂��B
	 */
	~CTemporaryFileT(void)
	{
		if ( ! m_targetFile.IsEmpty() )
		{
			::DeleteFile(m_targetFile);
			m_targetFile.Empty();
		}
	}

	/**
	 * [�擾] �e���|�����t�@�C�����擾
	 *	@return �t���p�X�t�@�C�����B empty��ԂȂ�쐬���Ă��Ȃ���ԁB
	 */
	CStr GetName(void) const
	{
		return m_targetFile;
	}

	/**
	 * [�m�F] �e���|�����t�@�C���L���m�F
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool HasThat(void) const
	{
		return ! m_targetFile.IsEmpty();
	}

	/**
	 * [����] ���.
	 *		�쐬�����e���|�����t�@�C�����폜���܂��B
	 *		�쐬���Ă��Ȃ���Ή������܂���B
	 *	@throw CDeleteFailureException �e���|�����t�@�C�����g���Ă���ƃX���[����܂��B
	 */
	void Free(void)
	{
		if ( ! m_targetFile.IsEmpty() )
		{
			if ( ! ::DeleteFile(m_targetFile) )
			{
				throw CDeleteFailureException(_GetLastError("DeleteFile"));
			}
			m_targetFile.Empty();
		}
	}

	/**
	 * [����] ���.
	 *		�e���|�����t�@�C���̊Ǘ����I�����܂��B
	 *		�쐬�����e���|�����t�@�C���͍폜���܂���B
	 */
	void Detach(void)
	{
		m_targetFile.Empty();
	}

	/**
	 * [����] ���.
	 *		�쐬�����e���|�����t�@�C�����������ɓǂݍ��݁A�폜���܂��B
	 *		�쐬���Ă��Ȃ���Ή������܂���B
	 *	@return �t�@�C�����e
	 *	@throw CIoFailureException �t�@�C�����쎞�ɃG���[����������ƁA�X���[����܂��B
	 *	@throw CDeleteFailureException �e���|�����t�@�C�����g���Ă���ƃX���[����܂��B	
	 */
	CByteVector Release(void)
	{
		CByteVector vb;
		if ( ! m_targetFile.IsEmpty() )
		{
			HANDLE h = ::CreateFile(m_targetFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				throw COpenFailureException(_GetLastError("CreateFile"));
			}
			DWORD dw = 0;
			DWORD size = ::GetFileSize(h, &dw);
			if ( size > 0 )
			{
				vb.SetSize(size);
				BOOL boRc = ::ReadFile(h, vb.GetBuffer(size), size, &dw, NULL);
				if ( ! boRc || dw != size )
				{
					throw CReadFailureException(_GetLastError("ReadFile"));
				}
				vb.ReleaseBuffer();
			}
			::CloseHandle(h);
			Free();
		}
		return vb;
	}

	/**
	 * [����] �쐬.
	 *		�e���|�����t�@�C�����ƃT�C�Y0�̃e���|�����t�@�C�����쐬���܂��B
	 *	@note �쐬�����e���|�����t�@�C��������ΐ�ɉ�����܂��B
	 *	@return �e���|�����t�@�C����
	 *	@throw CIoFailureException �t�@�C�����쎞�ɃG���[����������ƁA�X���[����܂��B
	 */
	CStr Create(void)
	{
		m_Create();
		return m_targetFile;
	}

	/**
	 * [����] �쐬.
	 *		�e���|�����t�@�C�����Ǝw��T�C�Y�̃e���|�����t�@�C�����쐬���܂��B
	 *	@note �쐬�����e���|�����t�@�C��������ΐ�ɉ�����܂��B
	 *	@param size �T�C�Y�B
	 *	@return �e���|�����t�@�C����
	 *	@throw CIoFailureException �t�@�C�����쎞�ɃG���[����������ƁA�X���[����܂��B
	 */
	CStr Create(size_t size)
	{
		if ( m_Create() && size > 0 )
		{
			HANDLE h = ::CreateFile(m_targetFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				throw COpenFailureException(_GetLastError("CreateFile"));
			}
			LONG l = 0;
			::SetFilePointer(h, ToDword(size), &l, FILE_BEGIN);
			VERIFY( ::SetEndOfFile(h) );
			::CloseHandle(h);
		}
		return m_targetFile;
	}

	/**
	 * [����] �쐬.
	 *		�e���|�����t�@�C�����Ǝw��f�[�^�̃e���|�����t�@�C�����쐬���܂��B
	 *	@note �쐬�����e���|�����t�@�C��������ΐ�ɉ�����܂��B
	 *	@param size �f�[�^�T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@return �e���|�����t�@�C����
	 *	@throw CIoFailureException �t�@�C�����쎞�ɃG���[����������ƁA�X���[����܂��B
	 */
	CStr Create(size_t size, LPCVOID P)
	{
		if ( m_Create() && size >= 0 )
		{
			HANDLE h = ::CreateFile(m_targetFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				throw COpenFailureException(_GetLastError("CreateFile"));
			}
			DWORD dw = 0;
			VERIFY( ::WriteFile(h, P, ToDword(size), &dw, NULL) );
			::CloseHandle(h);
		}
		return m_targetFile;
	}

	/**
	 * [����] �쐬.
	 *		�e���|�����t�@�C�����Ǝw��f�[�^�̃e���|�����t�@�C�����쐬���܂��B
	 *	@note �쐬�����e���|�����t�@�C��������ΐ�ɉ�����܂��B
	 *	@param vb �f�[�^�B
	 *	@return �e���|�����t�@�C����
	 *	@throw CIoFailureException �t�@�C�����쎞�ɃG���[����������ƁA�X���[����܂��B
	 */
	CStr Create(const ISequenceCollectionT<BYTE>& vb)
	{
		return Create(vb.GetSize(), vb.ReferBuffer());
	}

	/**
	 * [�ݒ�] �e���|�����t�H���_�w��.
	 *	@note �ݒ肵�Ȃ��ꍇ�A�V�X�e���̃e���|�����t�H���_�ɍ쐬���܂��B
	 *	@param pFolder �p�X�B
	 */
	void SetTempraryFolder(LPCTSTR pFolder)
	{
		m_tempFolder = pFolder;
	}

	/**
	 * [����] �A�^�b�`.
	 *		�w��̃t�@�C�����e���|�����t�@�C���Ƃ��܂��B
	 *	@note �쐬�����e���|�����t�@�C��������ΐ�ɉ�����܂��B
	 *	@param lpszFile �t�@�C����(�t���p�X)�B
	 *	@return �e���|�����t�@�C����
	 *	@throw COpenFailureException �t�@�C�������݂��Ȃ��ƁA�X���[����܂��B
	 */
	CStr Attach(LPCSTR lpszFile)
	{
		if ( ToInt(::GetFileAttributes(lpszFile)) == -1 )
		{
			throw COpenFailureException(_GetLastError("GetFileAttributes"));
		}
		m_targetFile = lpszFile;
		return m_targetFile;
	}
};



/**@ingroup FILE
 * �e���|�����t�@�C���Ǘ��e���v���[�g
 *
 *	@note �v���t�B�b�N�X�� tnb �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTemporaryFile.h
 *
 *	@date 06/12/13 �V�K�쐬
 */
typedef CTemporaryFileT<'bnt'> CTemporaryFile;



}; // TNB