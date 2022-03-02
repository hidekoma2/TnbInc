#pragma once
/**
 *@file
 * �t�@�C���p�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifdef _WIN32_WCE
	#error TnbFilePathManager.h is only supported on Windows NT Series platforms.
#endif // _WIN32_WCE

#include "TnbFile.h"
#include "TnbStrEx.h"

#include <SHLWAPI.h>
#pragma comment(lib,"SHLWAPI.lib")



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �t�@�C���p�X�Ǘ��N���X
 *
 *		��̊Ǘ��p�X�ƁA�C���X�^���X���̃p�X�������Ă��܂��B
 *
 *	@note �C���X�^���X������ԁA�v���Z�X�̃p�X���J�����g�ɂ��A�I������Ɩ߂�܂��B
 *		�t�@�C���I���_�C�A���O���g�����ɗL���ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFilePathManager.h
 *
 *	@date 04/01/01 �V�K�쐬
 *	@date 06/05/01 CString�̎g�p����߂�
 *	@date 07/09/18 ���΃p�X�A��΃p�X�ϊ����\�b�h�A�ǉ��B
 */
class CFilePathManager
{
public:

	/**
	 * [�擾] �t�H���_�����o.
	 *	@note �t�@�C�����̃t�H���_�w�蕔���܂ł�Ԃ��܂��B
	 *		���Ƃ��΁A"c:/Foo/ASB.txt" �Ȃ� "c:/Foo" ���߂�܂��B
	 *	@param lpszFullPath �t�@�C����
	 *	@return �p�X��
	 */
	static CStr GetPathName(LPCTSTR lpszFullPath)
	{
		CStr str = lpszFullPath;
		INT_PTR p = str.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return str.Left(p + 1);
		}
		return str;
	}
	
	/**
	 * [�擾] �v���Z�X�̃J�����g�p�X�擾.
	 *		���݂̃J�����g�p�X��Ԃ��܂��B
	 *	@return �p�X��
	 */
	static CStr GetCurrentPath(void)
	{
		CStr str;
		DWORD dwRc = ::GetCurrentDirectory(MAX_PATH, str.GetBuffer(MAX_PATH));
		str.ReleaseBuffer();
		ASSERTLIB( dwRc != 0 );
		IgnoreUnusedValue(dwRc);
		return str;
	}

	/**
	 * �R���X�g���N�^.
	 *	@note ���݂̃f�B���N�g�����L�����A�v���Z�X�̃p�X�ɐ؂�ւ��܂��B
	 */
	CFilePathManager(void) 
		: m_strCurrentPath(GetCurrentPath()), m_strManagePath(GetProcessPath())
	{
		::SetCurrentDirectory(m_strManagePath);
	}

	/**
	 * �f�X�g���N�^.
	 *	@note �R���X�g���N�^���̃p�X�ɖ߂��܂��B
	 */
	~CFilePathManager(void)
	{
		::SetCurrentDirectory(m_strCurrentPath);
	}

	/**
	 * [�ݒ�] �p�X���Z�b�g.
	 *	@note �J�����g�p�X���A�Ǘ��p�X���A�R���X�g���N�^���̃p�X�ɂ��܂��B
	 *	@param boIsManagePath true �Ȃ�Ǘ��p�X�ɁA false �Ȃ�R���X�g���N�^���̃p�X�ɂ��܂��B
	 */
	void ResetPath(bool boIsManagePath = true) const
	{
		::SetCurrentDirectory(m_strCurrentPath);
	}

	/**
	 * [�擾] �Ǘ��p�X�擾
	 *		�C���X�^���X�ŊǗ����Ă���p�X��Ԃ��܂��B
	 *	@note �C���X�^���X�������̃p�X���A SetPath() �Ŏw�肵���p�X���Ǘ����Ă��܂��B
	 *	@return �Ǘ��p�X�B
	 */
	CStr GetPath(void) const
	{
		return m_strManagePath;
	}
	
	/**
	 * [�ݒ�] �Ǘ��p�X�w��.
	 *	@note �C���X�^���X�ŊǗ����Ă���p�X��ύX���A�J�����g�p�X���ύX���܂��B
	 *	@param lpszPath �p�X��
	 *	@retval true �����B
	 *	@retval false ���s�B�J�����g�p�X�A�Ǘ��p�X�͕ύX����܂���B
	 */
	bool SetPath(LPCTSTR lpszPath)
	{
		::Sleep(0);
		bool boRc = !! ::SetCurrentDirectory(lpszPath);
		if ( boRc )
		{
			m_strManagePath = GetCurrentPath();
		}
		return boRc;
	}

	/**
	 * [�ݒ�] �Ǘ��p�X�w��.
	 *		�w��̃t�@�C������f�B���N�g�����Ǘ��p�X�A����сA�J�����g�p�X�ɂ��܂��B
	 *	@param lpszFile �t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetPathOfFile(LPCTSTR lpszFile)
	{
		return SetPath(GetPathName(lpszFile));
	}

	/**
	 * [�擾] ���΃p�X�쐬.
	 *		�Ǘ��p�X����̑��΃p�X�ɕϊ����܂��B
	 *	@param path �p�X�B�ʏ��΃p�X���w�肵�܂��B
	 *	@return ���΃p�X�B
	 */
	CStr GetRelativePathTo(LPCTSTR path) const
	{
		if ( IsRelative(path) )
		{
			//== ���΃p�X
			return path;
		}
		//== ��΃p�X
		CStr s;
		BOOL r = ::PathRelativePathTo(s.GetBuffer(MAX_PATH),
							GetPath(),
							FILE_ATTRIBUTE_DIRECTORY,
							path,
							FILE_ATTRIBUTE_ARCHIVE);
		s.ReleaseBuffer();
		if ( ! r )
		{
			s.Empty();
		}
		return s;
	}

	/**
	 * [�擾] ��΃p�X�쐬.
	 *		�Ǘ��p�X����̑��΃p�X���A��΃p�X�ɕϊ����܂��B
	 *	@param path �Ǘ��p�X����̑��΃p�X���w�肵�܂��B
	 *	@return ��΃p�X�B
	 */
	CStr GetCanonicalize(LPCTSTR path) const
	{
		if ( ! IsRelative(path) )
		{
			//== ��΃p�X
			return path;
		}
		//== ���΃p�X��
		CStr s;
		BOOL r = ::PathCanonicalize(s.GetBuffer(MAX_PATH), GetPath() + _T("\\") + path);
		s.ReleaseBuffer();
		if ( ! r )
		{
			s.Empty();
		}
		return s;
	}

	/**
	 * [����] �t�@�C���I�[�v��.
	 *	@note �w��f�B���N�g�����v���Z�X�f�B���N�g�����J�����g�f�B���N�g��
	 *		�Ƃ������Ō������A�ǂݍ��ݑ����ŃI�[�v�����܂��B
	 *	@param lpszFile �I�[�v���������t�@�C��
	 *	@param lpszPath ��������f�B���N�g���BNULL�Ȃ�v���Z�X�f�B���N�g�����猟���J�n���܂��B
	 *	@return �t�@�C���n���h���B
	 *			�A�N�Z�X�ł��Ȃ��n���h���Ȃ�t�@�C����������Ȃ��������ƂɂȂ�܂��B
	 */
	CFileReader Open(LPCTSTR lpszFile, LPCTSTR lpszPath = NULL) const
	{
		CStr path = lpszPath;
		CFileReader fileHandle;
		CStr strFullpath;
		//
		loop ( iType, 3 )
		{
			if ( iType == 0 || iType == 1 )
			{
				//0;�w��p�X 1;EXE�p�X ����
				if ( iType == 0 && path.IsEmpty() )
				{
					continue;
				}
				if ( iType == 1 )
				{
					path = GetProcessPath();
				}
				//
				if ( ! ::PathIsRelative(lpszFile) )
				{
					//��΃p�X
					continue;
				}
				//
				if ( ! ::PathSearchAndQualify(
							CStr::Fmt(_T("%s\\%s"), path, lpszFile),
							strFullpath.GetBuffer(MAX_PATH),
							MAX_PATH))
				{
					continue;
				}
				strFullpath.ReleaseBuffer();
			}
			else
			{
				//�p�X�⊮�Ȃ�
				strFullpath = lpszFile;
			}
			//
			{
				TCHAR atcBuf[MAX_PATH];
				::GetShortPathName(strFullpath, atcBuf, MAX_PATH);
				::GetLongPathName(atcBuf, strFullpath.GetBuffer(MAX_PATH), MAX_PATH);
				strFullpath.ReleaseBuffer();
			}
			//
			if ( fileHandle.Open(strFullpath, true) )
			{
				break;
			}
		}
		return fileHandle;
	}

	/**
	 * [�m�F] ���΃p�X�`�F�b�N
	 *	@param lpszPath �p�X
	 *	@retval true ���΃p�X
	 *	@retval false ��΃p�X
	 */
	static bool IsRelative(LPCTSTR lpszPath)
	{
		return !! ::PathIsRelative(lpszPath);
	}

private:
	CStr m_strCurrentPath;		///< �R���X�g���N�^���̃p�X
	CStr m_strManagePath;		///< ���̊Ǘ��p�X
};



}; // TNB
