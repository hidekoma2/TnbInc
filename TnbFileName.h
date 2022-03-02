#pragma once
/**
 *@file
 * �t�@�C���l�[���֌W�̃w�b�_
 *
 *		�t�@�C�����Ǘ��̃N���X������܂��B
 *		�t�@�C��������A�L���A�����A�T�C�Y�A���t�Ȃǂ��擾�ł��܂��B
 *		�܂��A�����A���t�̕ύX���o���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFile.h"
#include "TnbStrEx.h"
#include "TnbDntStr.h"
#ifndef _WIN32_WCE
 #include <SHLWAPI.h>
 #include <shellapi.h>
 #pragma comment(lib,"SHLWAPI.lib")
 #include <ShObjIdl.h>
 #include <ShlGuid.h>
#endif



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN
 #ifndef FOF_NOERRORUI
  #define FOF_NOERRORUI 			 0x0400  // don't put up error UI
 #endif
#endif



/**@ingroup FILE
 * �t�@�C�����Ǘ��N���X
 *
 *		�t�@�C��������A�L���A�����A�T�C�Y�A���t�Ȃǂ��擾�ł��܂��B
 *		�܂��A�����A���t�̕ύX���o���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileName.h
 *
 *	@date 06/05/01	�V�K�쐬�B
 *	@date 10/10/05	�\���ύX�B CopyEx() , InsertAtEndOfName() �V�K�B
 *	@date 11/09/16	IsShortcut(), MakeShortcut() , ResolveShortcut() �V�K�B
 *	@date 12/01/24	InsertAtEndOfName() �̎d�l�A���ǁB
 *	@date 12/05/11	MakeDirectory() �Ŗ����ċN�Ăяo���̉\�����������B�C���B
 *	@date 13/09/02	GetSizeOnDisk(), GetBytesOfCluster() �V�K�B
 */
class CFileName
{
public:

	/**
	 * �R���X�g���N�^
	 *	@note �ێ����閼�O�̓J�����g�p�X���ɂȂ�܂��B
	 */
	CFileName(void) : m_strName(TNB::GetProcessPath()), m_boIsExists(false)
	{
		IsExist();
	}

	/**
	 * ����R���X�g���N�^
	 *	@param lpszName �t�@�C����
	 */
	CFileName(LPCTSTR lpszName) : m_boIsExists(false)
	{
		operator=(lpszName);
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w�肵������������H�����L�����܂��B
	 *	@param lpszName �t�@�C����
	 *	@param dummy �_�~�[�B
	 */
	CFileName(LPCTSTR lpszName, bool dummy) : m_boIsExists(false)
	{
		m_strName = lpszName;
		m_strName.TrimRight('\\');
		IsExist();
	}

	/**
	 * [�ݒ�] ����I�y���[�V����
	 *	@param lpszName �t�@�C����
	 *	@return �����̎Q��
	 */
	CFileName& operator=(LPCTSTR lpszName)
	{
#ifndef _WIN32_WCE
		CStr s1, s2;
		bool r = !! ::PathSearchAndQualify(lpszName, s1.GetBuffer(MAX_PATH), MAX_PATH);
		s1.ReleaseBuffer();
		if ( r )
		{
			r = (::GetLongPathName(s1, s2.GetBuffer(MAX_PATH), MAX_PATH) != 0);
			s2.ReleaseBuffer();
		}
		if ( r )
		{
			m_strName = s2;
		}
		else
#endif
		{
			m_strName = lpszName;
		}
		m_strName.TrimRight('\\');
		IsExist();
		return *this;
	}

	/**
	 * [�ݒ�] ���
	 *	@note ���̊֐��́A�t�@�C���̑��݂Ə����m�F���݂̏����i�[���鎞�Ɏg�p���܂��B
	 *	@param lpszName �t�@�C����
	 *	@param tFileAttr �t�@�C�����
	 */
	void Set(LPCTSTR lpszName, const WIN32_FILE_ATTRIBUTE_DATA& tFileAttr)
	{
		m_strName = lpszName;
		m_strName.TrimRight('\\');
		m_tInfo = tFileAttr;
		m_boIsExists = true;
	}

	/**
	 * [�ݒ�] ���
	 *	@note ���̊֐��́A�t�@�C���̑��݂Ə����m�F���݂̏����i�[���鎞�Ɏg�p���܂��B
	 *	@param lpszPath �p�X��
	 *	@param t �t�@�C�����
	 */
	void Set(LPCTSTR lpszPath, const WIN32_FIND_DATA& t)
	{
		m_strName.Format(_T("%s\\%s"), lpszPath, t.cFileName);
		m_strName.Replace(_T("\\\\"), _T("\\"));
		m_strName.TrimRight('\\');
		m_tInfo.dwFileAttributes	= t.dwFileAttributes;
		m_tInfo.ftCreationTime		= t.ftCreationTime;
		m_tInfo.ftLastAccessTime	= t.ftLastAccessTime;
		m_tInfo.ftLastWriteTime		= t.ftLastWriteTime;
		m_tInfo.nFileSizeHigh		= t.nFileSizeHigh;
		m_tInfo.nFileSizeLow		= t.nFileSizeLow;
		m_boIsExists = true;
	}

	/**
	 * [�m�F] ��r.
	 *		�����t�@�C�����w���Ă��邩�A���ׂ邱�Ƃ��o���܂��B
	 *	@param lpszName �t�@�C����
	 *	@retval true ����
	 *	@retval false �قȂ�
	 */
	bool IsSameFile(LPCTSTR lpszName) const
	{
		CFileName N(lpszName);
		CStr str = N.GetFullShortName();
		return str.CompareNoCase(GetFullShortName()) == 0;
	}

	/**
	 * [�擾] �t��Name���擾
	 *	@return �t��Name��
	 */
	operator LPCTSTR(void) const
	{
		return m_strName;
	}

	/**
	 * [�擾] �t��Name���擾
	 *	@return �t��Name��
	 */
	CStr GetFullName(void) const
	{
		return m_strName;
	}

	/**
	 * [�擾] FullShortName���擾
	 *	@return Name��
	 */
	CStr GetFullShortName(void) const
	{
		#ifndef _WIN32_WCE
			CStr str;
			::GetShortPathName(m_strName, str.GetBuffer(MAX_PATH), MAX_PATH);
			str.ReleaseBuffer();
			return str;
		#else
			return m_strName;
		#endif
	}

	/**
	 * [�擾] �t�@�C�����擾
	 *	@return �t�@�C����
	 */
	CStr GetFileName(void) const
	{
		INT_PTR p = m_strName.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return m_strName.Mid(p + 1);
		}
		return m_strName;
	}

	/**
	 * [�擾] �t�@�C���^�C�g���擾
	 *	@note �t�@�C��������g���q�����������̂��擾�ł��܂��B
	 *	@return �t�@�C���^�C�g��
	 */
	CStr GetFileTitle(void) const
	{
		CStr s = GetFileName();
		INDEX idx = s.Find(_T('.'));
		if ( idx != INVALID_INDEX )
		{
			s = s.Left(idx);
		}
		return s;
	}

	/**
	 * [�擾] ShortName���擾
	 *	@return Name��
	 */
	CStr GetShortName(void) const
	{
		CStr s = GetFullShortName();
		INT_PTR p = s.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return s.Mid(p + 1);
		}
		return s;
	}

	/**
	 * [�擾] �g���q�擾
	 *	@return �g���q
	 */
	CStr GetExtName(void) const
	{
		INT_PTR p = m_strName.ReverseFind('.');
		INT_PTR p1 = m_strName.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 && p1 < p )
		{
			return m_strName.Mid(p + 1);
		}
		return _T("");
	}

	/**
	 * [�擾] �p�X�擾
	 *	@return �p�X
	 */
	CStr GetPathName(void) const
	{
		INT_PTR p = m_strName.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return m_strName.Left(p);
		}
		return m_strName;
	}

	/**
	 * [�擾] �t�@�C���T�C�Y�擾
	 *	@note �L���ȃt�@�C�������w�肵�Ă��鎞�̂ݐ������܂��B
	 *	@retval �O�ȏ� �t�@�C���T�C�Y
	 *	@retval �}�C�i�X�@���s
	 */
	LONGLONG GetSize(void) const
	{
		LONGLONG r = -1;
		if ( m_boIsExists )
		{
			LARGE_INTEGER i;
			i.HighPart = m_tInfo.nFileSizeHigh;
			i.LowPart = m_tInfo.nFileSizeLow;
			r = i.QuadPart;
		}
		return r;
	}

#ifndef _WIN32_WCE
	/**
	 * [�擾] �f�B�X�N��̃T�C�Y�擾
	 *	@note �f�B�X�N��Ő�L���Ă���T�C�Y��Ԃ��܂��B
	 *	@retval �O�ȏ� �t�@�C���T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	LONGLONG GetSizeOnDisk(void) const
	{
		LONGLONG r = -1;
		::SetLastError(NO_ERROR);
		CStr fn = GetFullName();
		WIN32_FILE_ATTRIBUTE_DATA t;
		if ( ::GetFileAttributesEx(fn, GetFileExInfoStandard, &t) )
		{
			LARGE_INTEGER l;
			if ( (t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
			{
				// DIR �̏ꍇ
				l.HighPart = t.nFileSizeHigh;
				l.LowPart = t.nFileSizeLow;
				r = l.QuadPart;
			}
			else
			{
				// FILE �̏ꍇ
				DWORD h;
				l.LowPart = ::GetCompressedFileSize(fn, &h);
				if ( ::GetLastError() == 0 )
				{
					l.HighPart = h;
					r = l.QuadPart;
					// �N���X�^�T�C�Y�A�Z�N�^�[�T�C�Y
					DWORD sectorsPerCluster;
					DWORD bytesPerSector;
					DWORD numberOfFreeClusters;
					DWORD totalNumberOfClusters;
					if ( ::GetDiskFreeSpace(GetPathName(), &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters) )
					{
						DWORD d = sectorsPerCluster * bytesPerSector; // �Z�N�^�[ byte��
						r = (r + d - 1) / d * d;
					}
					else
					{
						r = -1;
					}
				}
			}
		}
		return r;
	}

	/**
	 * [�擾] �N���X�^�T�C�Y�擾.
	 *	@note �f�B�X�N��Ő�L���Ă���T�C�Y��Ԃ��܂��B
	 *	@retval �O�ȊO �N���X�^�o�C�g��
	 *	@retval �O �擾���s
	 */
	DWORD GetBytesOfCluster(void) const
	{
		if ( m_boIsExists )
		{
			CStr fn = GetFullName();
			if ( ! IsDirectory() )
			{
				fn = GetPathName();
			}
			// �N���X�^�T�C�Y�A�Z�N�^�[�T�C�Y
			DWORD sectorsPerCluster;
			DWORD bytesPerSector;
			DWORD numberOfFreeClusters;
			DWORD totalNumberOfClusters;
			if ( ::GetDiskFreeSpace(GetPathName(), &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters) )
			{
				return sectorsPerCluster * bytesPerSector; // �Z�N�^�[ byte��
			}
		}
		return 0;
	}
#endif

	/**
	 * [�擾] �t�@�C�����擾
	 *	@note �L���ȃt�@�C�������w�肵�Ă��鎞�̂ݐ������܂��B
	 *	@param[out] P �t�@�C�����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetAttributeData(LPWIN32_FILE_ATTRIBUTE_DATA P) const
	{
		bool r = IsExist();
		if ( r && P != NULL )
		{
			*P = m_tInfo;
		}
		return r;
	}

	/**
	 * [�擾] �t�@�C�����擾
	 *	@note �L���ȃt�@�C�������w�肵�Ă��鎞�̂ݐ������܂��B
	 *	@param[out] P �t�@�C�����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetFindData(LPWIN32_FIND_DATA P) const
	{
		bool r = IsExist();
		if ( r && P != NULL )
		{
			STRLIB::Copy(P->cFileName, GetFileName());
			P->dwFileAttributes	= m_tInfo.dwFileAttributes;
			P->ftCreationTime	= m_tInfo.ftCreationTime;
			P->ftLastAccessTime	= m_tInfo.ftLastAccessTime;
			P->ftLastWriteTime	= m_tInfo.ftLastWriteTime;
			P->nFileSizeHigh	= m_tInfo.nFileSizeHigh;
			P->nFileSizeLow	= m_tInfo.nFileSizeLow;
			#if !defined(_WIN32_WCE)
				STRLIB::Copy(P->cAlternateFileName, GetShortName());
			#endif
		}
		return r;
	}

	/**
	 * [�擾] �t�@�C������������ǉ�.
	 *		�t�@�C�����̖��ɕ������ǉ������t�@�C������Ԃ��܂��B
	 *	@note �{�C���X�^���X�̊Ǘ�����t�@�C�����͕ύX���܂���B
	 *	@param lpsz �ǉ����镶����B
	 *	@param lpszExt �ύX����g���q�B�ȗ��A NULL �Ȃ�A�ύX���܂���B
	 *	@return ���H�����t�@�C���p�X�B
	 */
	CStr InsertAtEndOfName(LPCTSTR lpsz, LPCTSTR lpszExt = NULL) const
	{
		CStr s = GetFullName();
		INT_PTR p = s.ReverseFind('.');
		INT_PTR p1 = s.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 && p1 < p )
		{
			if ( lpszExt != NULL )
			{
				return s.Left(p) + lpsz + _T(".") + lpszExt;
			}
			return s.Left(p) + lpsz + s.Mid(p);
		}
		if ( lpszExt != NULL )
		{
			return s + lpsz + _T(".") + lpszExt;
		}
		return s + lpsz;
	}
	
	/**
	 * [�m�F] �L���`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsExist(void) const
	{
		m_boIsExists = ms_GetInfo(m_strName, &m_tInfo);
		return m_boIsExists;
	}

	/**
	 * [�m�F] ReadOnly�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsReadOnly(void) const { return m_IsAttr(FILE_ATTRIBUTE_READONLY); }

	/**
	 * [�m�F] Directory�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsDirectory(void) const { return m_IsAttr(FILE_ATTRIBUTE_DIRECTORY); }

	/**
	 * [�m�F] Compressed�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsCompressed(void) const { return m_IsAttr(FILE_ATTRIBUTE_COMPRESSED); }

	/**
	 * [�m�F] System�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsSystem(void) const { return m_IsAttr(FILE_ATTRIBUTE_SYSTEM); }

	/**
	 * [�m�F] Hidden�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsHidden(void) const { return m_IsAttr(FILE_ATTRIBUTE_HIDDEN); }

	/**
	 * [�m�F] Temporary�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsTemporary(void) const { return m_IsAttr(FILE_ATTRIBUTE_TEMPORARY); }

	/**
	 * [�m�F] Normal�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsNormal(void) const { return m_IsAttr(FILE_ATTRIBUTE_NORMAL); }

	/**
	 * [�m�F] Archived�����`�F�b�N
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool IsArchived(void) const { return m_IsAttr(FILE_ATTRIBUTE_ARCHIVE); }

	/**
	 * [�擾] �ŏI�����ݓ����擾
	 *	@return �����B���s�����Empty��Ԃ� CFileTimeEx �ɂȂ�B
	 */
	CFileTimeEx GetLastWriteTime(void) const
	{
		CFileTimeEx ft;
		if ( m_boIsExists ){ ft = m_tInfo.ftLastWriteTime; }
		return ft;
	}

	/**
	 * [�擾] �ŏI�A�N�Z�X�����擾
	 *	@return �����B���s�����Empty��Ԃ� CFileTimeEx �ɂȂ�B
	 */
	CFileTimeEx GetLastAccessTime(void) const
	{
		CFileTimeEx ft;
		if ( m_boIsExists ){ ft = m_tInfo.ftLastAccessTime; }
		return ft;
	}

	/**
	 * [�擾] �쐬�����擾
	 *	@return �����B���s�����Empty��Ԃ� CFileTimeEx �ɂȂ�B
	 */
	CFileTimeEx GetCreationTime(void) const
	{
		CFileTimeEx ft;
		if ( m_boIsExists ){ ft = m_tInfo.ftCreationTime; }
		return ft;
	}

	/**
	 * [�ݒ�] ReadOnly�����ݒ�
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetReadOnly(void){ return m_SetAttr(FILE_ATTRIBUTE_READONLY); }

	/**
	 * [�ݒ�] Hidden�����ݒ�
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetHidden(void){ return m_SetAttr(FILE_ATTRIBUTE_HIDDEN); }

	/**
	 * [�ݒ�] Normal�����ݒ�
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetNormal(void){ return m_SetAttr(FILE_ATTRIBUTE_NORMAL); }

	/**
	 * [�ݒ�] �t�@�C�����t�ݒ�
	 *	@param timeCreation �쐬���t
	 *	@param timeLastAccess �ŏI�A�N�Z�X���t
	 *	@param timeLastWrite �����ݓ��t
	 *	@retval 0 ����
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD SetTime(const CFileTimeEx& timeCreation, const CFileTimeEx& timeLastAccess, const CFileTimeEx& timeLastWrite)
	{
		CFileWriter file;
		if ( file.Open(m_strName) )
		{
			::SetLastError(0);
			file.SetTime(timeCreation, timeLastAccess, timeLastWrite);
		}
		return ::GetLastError();
	}

	/**
	 * [�쐬] �t�@�C���ꗗ�쐬
	 *	@note �ێ�����Directory�̏ꍇ�A�ȉ��̃t�@�C��/�t�H���_���������܂��B
	 *	@param lpszOpt �����p�̃I�v�V�����B�ȗ������ ��*.* ���w�肳��܂��B
	 *	@return �t���p�X�t�@�C�����ꗗ
	 */
	CStrVector GetFiles(LPCTSTR lpszOpt = _T("\\*.*"))
	{
		CStrVector vstrResult;
		if ( IsDirectory() )
		{
			WIN32_FIND_DATA t;
			HANDLE h = ::FindFirstFile(m_strName + lpszOpt, &t);
			if ( h != INVALID_HANDLE_VALUE )
			{
				while ( true )
				{
					if ( t.cFileName[0] != '.' )
					{
						vstrResult.Add(CStr::Fmt(_T("%s\\%s"), m_strName, t.cFileName));
					}
					if ( ! ::FindNextFile(h, &t) )
					{
						::FindClose(h);
						break;
					}
				}
			}
		}
		return vstrResult;
	}

	/**
	 * [�쐬] �t�H���_�쐬.
	 *	@note �K�w���[���ꍇ�A���ׂč쐬���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool MakeDirectory(void)
	{
		if ( ! ::CreateDirectory(m_strName, NULL) )
		{
			DWORD e = ::GetLastError();
			if ( e == ERROR_NOT_READY )
			{
				return false;
			}
			if ( e != ERROR_ALREADY_EXISTS )
			{
				CStr n = GetPathName();
				if ( m_strName == n )
				{
					return false;
				}
				CFileName(n).MakeDirectory();
				if ( ! ::CreateDirectory(m_strName, NULL) )
				{
					return false;
				}
			}
		}
		IsExist();
		return true;
	}

	/**
	 * [����] ���l�[��.
	 *	@note �t�@�C���ł��f�B���N�g���ł����l�[���\�ł��B
	 *	@note	��������ƁA�{�I�u�W�F�N�g�ŋL�����Ă���t�@�C���������l�[����̂��̂ɂȂ�܂��B
	 *	@param	lpszNew			���l�[���㖼�B
	 *	@param	boIsAllowUndo	true�Ȃ�UNDO�Ɏc��悤�ɂ��܂��B
	 *							false or�ȗ������UNDO�Ɏc���܂���B
	 *	@param	boIsSilent		false�Ȃ�UI���o���A���[�U�Ɋm�F�����܂��B
	 *							true or�ȗ��Ȃ� UI����؏o���܂���B
	 *	@retval true ����
	 *	@retval false ���s�i�L�����Z�����܂ށj
	 */
	bool Rename(LPCTSTR lpszNew, bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() )
		{
			CFileName fnNew = m_FullPath(lpszNew);
			if ( m_ShFileOp(FO_RENAME, fnNew, boIsAllowUndo, boIsSilent) )
			{
				//����
				*this = fnNew;
				return true;
			}
		}
		return false;
	}

	/**
	 * [����] �폜.
	 *	@note �t�@�C���ł��f�B���N�g���ł��폜�\�ł��B
	 *	@param	boIsAllowUndo	true�Ȃ�UNDO�Ɏc��悤�ɂ��܂��B
	 *							false or�ȗ������UNDO�Ɏc���܂���B
	 *	@param	boIsSilent		false�Ȃ�UI���o���A���[�U�Ɋm�F�����܂��B
	 *							true or�ȗ��Ȃ� UI����؏o���܂���B
	 *	@retval true ����
	 *	@retval false ���s�i�L�����Z�����܂ށj
	 */
	bool Remove(bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() && m_ShFileOp(FO_DELETE, NULL, boIsAllowUndo, boIsSilent) )
		{
			//����
			IsExist();
			return true;
		}
		return false;
	}

	/**
	 * [����] �R�s�[.
	 *	@note �t�@�C���ł��f�B���N�g���ł��R�s�[�\�ł��B
	 *	@param	lpszNew			�R�s�[�於�B
	 *	@param	boIsAllowUndo	true�Ȃ�UNDO�Ɏc��悤�ɂ��܂��B
	 *							false or�ȗ������UNDO�Ɏc���܂���B
	 *	@param	boIsSilent		false�Ȃ�UI���o���A���[�U�Ɋm�F�����܂��B
	 *							true or�ȗ��Ȃ� UI����؏o���܂���B
	 *	@retval true ����
	 *	@retval false ���s�i�L�����Z�����܂ށj
	 */
	bool CopyTo(LPCTSTR lpszNew, bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() && m_ShFileOp(FO_COPY, m_FullPath(lpszNew), boIsAllowUndo, boIsSilent) )
		{
			return true;
		}
		return false;
	}

	/**
	 * [����] ���[�u.
	 *	@note �t�@�C���ł��f�B���N�g���ł����[�u�\�ł��B
	 *	@note	��������ƁA�{�I�u�W�F�N�g�ŋL�����Ă���t�@�C���������[�u��̂��̂ɂȂ�܂��B
	 *	@param	lpszNew			���[�u�於�B
	 *	@param	boIsAllowUndo	true�Ȃ�UNDO�Ɏc��悤�ɂ��܂��B
	 *							false or�ȗ������UNDO�Ɏc���܂���B
	 *	@param	boIsSilent		false�Ȃ�UI���o���A���[�U�Ɋm�F�����܂��B
	 *							true or�ȗ��Ȃ� UI����؏o���܂���B
	 *	@retval true ����
	 *	@retval false ���s�i�L�����Z�����܂ށj
	 */
	bool MoveTo(LPCTSTR lpszNew, bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() )
		{
			CFileName fnNew = m_FullPath(lpszNew);
			if ( m_ShFileOp(FO_MOVE, fnNew, boIsAllowUndo, boIsSilent) )
			{
				//����
				*this = fnNew;
				return true;
			}
		}
		return false;
	}

	/**
	 * [����] �R�s�[.
	 *	@note �f�B���N�g���̓R�s�[�ł��܂���B
	 *	@param	lpszNew			�R�s�[�於�B
	 *	@param	boIsAllowUndo	true�Ȃ�UNDO�Ɏc��悤�ɂ��܂��B
	 *							false or�ȗ������UNDO�Ɏc���܂���B
	 *	@retval empty ���s
	 *	@retval ��L�ȊO �����B������́A���ۂɃR�s�[�����t�@�C�����B
	 */
	CStr CopyEx(LPCTSTR lpszNew, bool boIsAllowUndo = false)
	{
		if ( IsExist() && ! IsDirectory() )
		{
			CStr s = lpszNew;
			if ( IsExist(s) )
			{
				CStr p1 = s;
				CStr p2;
				INT_PTR f1 = s.ReverseFind('.');
				INT_PTR f2 = s.ReverseFindOneOf(_T("\\/"));
				if ( f1 >= 0 && f2 < f1 )
				{
					p1 = s.Left(f1);
					p2 = s.Mid(f1);
				}
				loop ( i, 100000 )
				{
					s.Format(_T("%s(%d)%s"), p1, i + 1, p2);
					if ( ! IsExist(s) )
					{
						if ( CopyTo(s, boIsAllowUndo, true) )
						{
							return s;
						}
						break;
					}
				}
			}
			else
			{
				if ( CopyTo(s, boIsAllowUndo, true) )
				{
					return s;
				}
			}
		}
		return _T("");
	}

	/**
	 * [�쐬] �V���[�g�J�b�g�쐬.
	 *		�{�C���X�^���X���ێ����Ă���t�@�C�����̃V���[�g�J�b�g���쐬���܂��B
	 *	@attention �{���\�b�h�� COM ���g�p���Ă��܂��B��� CoInitialize() ���A���������Ă����Ă��������B
	 *	@param lpszLinkName �V���[�g�J�b�g�t�@�C�����B�g���q�� lnk �ɂ���悤�ɂ��Ă��������B
	 *	@param lpszDescription �������B CE �ł͖�������܂��B
	 *	@param lpszParameter �p�����[�^�B�ȗ��\�B CE �ł͖�������܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool MakeShortcut(LPCTSTR lpszLinkName, LPCTSTR lpszDescription, LPCTSTR lpszParameter = NULL) const
	{
#ifndef _WIN32_WCE
		// ShellLink �쐬
		IShellLink* pSL = NULL;
		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
						IID_IShellLink, reinterpret_cast<LPVOID*>(&pSL));
		if ( SUCCEEDED(hr) )
		{ 
			pSL->SetPath(m_strName);
			pSL->SetWorkingDirectory(GetPathName());
			pSL->SetDescription(lpszDescription);
			if ( lpszParameter != NULL )
			{
				pSL->SetArguments(lpszParameter);
			}
			// ShellLink ���� PersistFile �𓾂�
			IPersistFile* pPF = NULL; 
			hr = pSL->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPF));
			if ( SUCCEEDED(hr) ) 
			{ 
				hr = pPF->Save(CUnicode(lpszLinkName), TRUE);
				pPF->Release();
				pPF = NULL;
			} 
			pSL->Release();
			pSL = NULL;
		}
		return SUCCEEDED(hr);
#else
		CStr ln = lpszLinkName;
		CStr fn = m_strName;
		return !! ::SHCreateShortcut(ln.GetBuffer(MAX_PATH), fn.GetBuffer(MAX_PATH));
#endif
	}
	
	/**
	 * [�m�F] Shortcut�`�F�b�N
	 *	@attention �{���\�b�h�� COM ���g�p���Ă��܂��B��� CoInitialize() ���A���������Ă����Ă��������B
	 *	@retval true �V���[�g�J�b�g�t�@�C��
	 *	@retval false ��
	 */
	bool IsShortcut(void) const
	{
#ifndef _WIN32_WCE
		IShellLink* pSL = NULL;
		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								IID_IShellLink, reinterpret_cast<LPVOID*>(&pSL)); 
		if ( SUCCEEDED(hr) ) 
		{ 
			IPersistFile* pPF = NULL; 
			hr = pSL->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPF));
			if ( SUCCEEDED(hr) ) 
			{ 
				hr = pPF->Load(CUnicode(m_strName), STGM_READ);
				pPF->Release();
				pPF = NULL;
			} 
			pSL->Release(); 
			pSL = NULL;
		} 
		return SUCCEEDED(hr);
#else
		CStr s;
		if ( ::SHGetShortcutTarget(m_strName, s.GetBuffer(MAX_PATH), MAX_PATH) )
		{
			s.ReleaseBuffer();
			return true;
		}
		return false;
#endif
	}

	/**
	 * [�ݒ�] �V���[�g�J�b�g����.
	 *		�{�C���X�^���X���ێ����Ă���t�@�C�������V���[�g�J�b�g�̏ꍇ�A�����N��̃t�@�C�����ɕϊ����܂��B
	 *	@attention �{���\�b�h�� COM ���g�p���Ă��܂��B��� CoInitialize() ���A���������Ă����Ă��������B
	 *	@param[out] _description �V���[�g�J�b�g�̐��������i�[����܂��B CE �ł͎擾����܂���B
	 *	@param[in] hWnd �L���ȃE�B���h�E�n���h�����w�肷��ƁA�V���[�g�J�b�g���Ȃ��Ƃ��ɒT�� UI ���o�܂��B CE �ł͋@�\���܂���B
	 *	@retval true �����ɐ������A�{�C���X�^���X�ŋL�����܂����B
	 *	@retval false ���s�B�V���[�g�J�b�g�t�@�C���łȂ����A�Q�Ɛ�̃t�@�C��������
	 */
	bool ResolveShortcut(CStr& _description, HWND hWnd = NULL)
	{
	#ifndef _WIN32_WCE
		IShellLink* pSL = NULL;
		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								IID_IShellLink, reinterpret_cast<LPVOID*>(&pSL)); 
		if ( SUCCEEDED(hr) ) 
		{ 
			IPersistFile* pPF = NULL; 
			hr = pSL->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPF));
			if ( SUCCEEDED(hr) ) 
			{ 
				hr = pPF->Load(CUnicode(m_strName), STGM_READ);
				if ( hWnd != NULL && SUCCEEDED(hr) ) 
				{ 
					hr = pSL->Resolve(hWnd, 0); 
				}
				WIN32_FIND_DATA wfd; 
				CStr path;
				if ( SUCCEEDED(hr) ) 
				{ 
					hr = pSL->GetPath(path.GetBuffer(MAX_PATH), MAX_PATH, &wfd, SLGP_SHORTPATH); 
					path.ReleaseBuffer();
				}
				if ( SUCCEEDED(hr) ) 
				{ 
					// Get the description of the target. 
					hr = pSL->GetDescription(_description.GetBuffer(MAX_PATH), MAX_PATH); 
					_description.ReleaseBuffer();
				}
				if ( SUCCEEDED(hr) ) 
				{
					operator=(path);
				}
				pPF->Release();
				pPF = NULL;
			} 
			pSL->Release(); 
			pSL = NULL;
		} 
		return SUCCEEDED(hr);
#else
		CStr s;
		if ( ::SHGetShortcutTarget(m_strName, s.GetBuffer(MAX_PATH), MAX_PATH) )
		{
			s.ReleaseBuffer();
			operator=(s);
			_description.Empty();
			return true;
		}
		return false;
#endif
	}

	/**
	 * [�ݒ�] �V���[�g�J�b�g����.
	 *		�{�C���X�^���X���ێ����Ă���t�@�C�������V���[�g�J�b�g�̏ꍇ�A�����N��̃t�@�C�����ɕϊ����܂��B
	 *	@attention �{���\�b�h�� COM ���g�p���Ă��܂��B��� CoInitialize() ���A���������Ă����Ă��������B
	 *	@retval true �����ɐ������A�{�C���X�^���X�ŋL�����܂����B
	 *	@retval false ���s�B�V���[�g�J�b�g�t�@�C���łȂ����A�Q�Ɛ�̃t�@�C��������
	 */
	bool ResolveShortcut(void)
	{
		CStr s;
		return ResolveShortcut(s, NULL);
	}
	
	/**
	 * [�m�F] �L���`�F�b�N.
	 *	@param lpszPath �m�F�Ώ��̃p�X.
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	static bool IsExist(LPCTSTR lpszPath)
	{
		return CFileName(lpszPath).IsExist();
	}

private:
	/// �w��̑�����true���`�F�b�N
	bool m_IsAttr(DWORD dwCheck) const
	{
		if ( m_boIsExists )
		{
			return (m_tInfo.dwFileAttributes & dwCheck) != 0;
		}
		return false;
	}
	/// �w��̑�����ݒ�
	bool m_SetAttr(DWORD dwAttr)
	{
		bool r = !! ::SetFileAttributes(m_strName, dwAttr);
		m_boIsExists = ms_GetInfo(m_strName, &m_tInfo);
		return r;
	}
	/// �t�@�C�����擾
	static bool ms_GetInfo(LPCTSTR lpszName, LPWIN32_FILE_ATTRIBUTE_DATA P)
	{
		WIN32_FIND_DATA t;
		HANDLE h = ::FindFirstFile(lpszName, &t);
		if ( h != INVALID_HANDLE_VALUE )
		{
			::FindClose(h);
			P->dwFileAttributes	= t.dwFileAttributes;
			P->ftCreationTime	= t.ftCreationTime;
			P->ftLastAccessTime	= t.ftLastAccessTime;
			P->ftLastWriteTime	= t.ftLastWriteTime;
			P->nFileSizeHigh	= t.nFileSizeHigh;
			P->nFileSizeLow		= t.nFileSizeLow;
			return true;
		}
		return false;
	}
	/**
	 * �t���p�X��
	 *	@param lpszName �t�@�C����
	 *	@return �t���p�X
	 */
	CStr m_FullPath(LPCTSTR lpszName)
	{
		#ifndef _WIN32_WCE
			CStr str;
			if ( ::PathIsRelative(lpszName) )
			{
				str = CStr::Fmt(_T("%s\\%s"), GetPathName(), lpszName);
			}
			else
			{
				str = lpszName;
			}
			return str;
		#else
			return lpszName;
		#endif
	}
	/**
	 * SHFileOperation�̎��s
	 *	@param[in]	wFunc			FO_DELETE,FO_COPY �ȂǓ�����w�肵�܂��B
	 *	@param[in]	lpszTo			FO_DELETE�ȊO�͏����Ώې���w�肷��K�v���L��܂��B
	 *	@param[in]	boIsAllowUndo	true�Ȃ�UNDO�Ɏc��悤�ɂ��܂��B
	 *	@param[in]	boIsSilent		false�Ȃ�UI���o���A���[�U�Ɋm�F�����܂�
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_ShFileOp(WORD wFunc, LPCTSTR lpszTo, bool boIsAllowUndo, bool boIsSilent)
	{
		CDoubleNullTerminateStr strdntFrom(m_strName);
		return m_ShFileOp(wFunc, strdntFrom, lpszTo, boIsAllowUndo, boIsSilent);
	}
	/**
	 * SHFileOperation�̎��s
	 *	@param[in]	wFunc			FO_DELETE,FO_COPY �ȂǓ�����w�肵�܂��B
	 *	@param[in]	strdntFrom		�����Ώی����w�肵�܂��B
	 *	@param[in]	lpszTo			FO_DELETE�ȊO�͏����Ώې���w�肷��K�v���L��܂��B
	 *	@param[in]	boIsAllowUndo	true�Ȃ�UNDO�Ɏc��悤�ɂ��܂��B
	 *	@param[in]	boIsSilent		false�Ȃ�UI���o���A���[�U�Ɋm�F�����܂�
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_ShFileOp(WORD wFunc, const CDoubleNullTerminateStr& strdntFrom, LPCTSTR lpszTo, 
				bool boIsAllowUndo, bool boIsSilent)
	{
		SHFILEOPSTRUCT tFileOp;
		CDoubleNullTerminateStr strdntTo(lpszTo);
		tFileOp.hwnd	= NULL;							// �E�B���h�E�n���h��
		tFileOp.wFunc	= wFunc;						// ���s���鑀��
		tFileOp.pFrom	= strdntFrom;					// �Ώۃt�@�C����
		tFileOp.pTo		= NULL;							// �ړI�t�@�C����
		if ( lpszTo != NULL )
		{
			tFileOp.pTo = strdntTo;
		}
		tFileOp.fFlags	= 0;							// Flag
		if ( boIsSilent )
		{
			tFileOp.fFlags |= FOF_SILENT		// �o�߃_�C�A���O����
							| FOF_NOCONFIRMATION// �m�F�Łu���ׂāv�w��
							| FOF_NOERRORUI;	// �G���[���A�_�C�A���O���o���Ȃ�
		}
		if ( boIsAllowUndo )
		{
			tFileOp.fFlags |= FOF_ALLOWUNDO;	//UNDO
		}
		tFileOp.fAnyOperationsAborted = FALSE;			// [out]����
		tFileOp.hNameMappings = NULL;					// [out]�t�@�C�����}�b�s���O�I�u�W�F�N�g
		tFileOp.lpszProgressTitle = _T("");
		return ::SHFileOperation(&tFileOp) == 0;
	}
	CStr								m_strName;		///< �ێ�����t�@�C����
	mutable bool						m_boIsExists;	///< �t�@�C�������݂��邩�H
	mutable	WIN32_FILE_ATTRIBUTE_DATA	m_tInfo;		///< �t�@�C�����
};



}; // TNB

