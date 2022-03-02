#pragma once
/**
 *@file
 * �t�@�C���o�[�W�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



#ifndef _WIN32_WCE
 #include <Winver.h>
 #pragma comment(lib,"version.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �t�@�C���o�[�W�����擾�N���X.
 *
 *	���\�[�X�ɂĕt������Ă�������擾���邱�Ƃ��o���܂��B
 *	�t�@�C���w�肷�邱�ƂŁA���v���Z�X�ȊO�̏����擾�ł��܂��B
 *
 *	@note Version���\�[�X�� "SpecialBuild" �ɕ����������ƁA�X�y�V�����r���h�t���O�������܂��B
 *	@note Version���\�[�X�� "PrivateBuild" �ɕ����������ƁA�v���C�x�[�g�r���h�t���O�������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileVersion.h
 *
 *	@date 06/09/19 �V�K�쐬
 *	@date 08/04/11 UNICODE���\�[�X�`�F�b�N�B WritePush() �ǉ��B
 */
class CFileVersion
{
public:

	/// �R���X�g���N�^
	CFileVersion(void) : m_pFileInfo(NULL), m_pLangCodePage(NULL), m_langCodePageSize(0)
	{
	}

	/**
	 * [�m�F] �L����
	 *	@retval true Open�ɐ������Ă���
	 *	@retval false Open���Ă��Ȃ��B
	 */
	bool IsValid(void) const
	{
		return m_pFileInfo != NULL;
	}

	/**
	 * [�ݒ�] �I�[�v��
	 *	@param lpszFile �Ώۂ̃t�@�C�����B�ȗ�����Ǝ��v���Z�X���ΏۂɂȂ�܂��B
	 *	@param hInst �Ώۂ̃C���X�^���X�B lpszFile �� NULL �ɂ��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(LPCTSTR lpszFile = NULL, HINSTANCE hInst = NULL)
	{
		CStr strFile;
		if ( lpszFile != NULL )
		{
			strFile = lpszFile;
		}
		else
		{
			::GetModuleFileName(hInst, strFile.GetBuffer(MAX_PATH), MAX_PATH);
			strFile.ReleaseBuffer();
		}
		DWORD dummy;
		DWORD size = ::GetFileVersionInfoSize(strFile.GetBuffer(), &dummy);
		if ( size != 0)
		{
			m_workMem.Resize(size);
			ZeroMemory(m_workMem, size);
			if ( ::GetFileVersionInfo(strFile.GetBuffer(), 0, size, m_workMem) )
			{
				UINT uLen;
				if ( ::VerQueryValue(m_workMem, _T("\\"), reinterpret_cast<LPVOID*>(&m_pFileInfo), &uLen) )
				{
					if ( m_pFileInfo->dwSignature == VS_FFI_SIGNATURE )
					{
						if ( ::VerQueryValue(m_workMem,
							_T("\\VarFileInfo\\Translation"),
							reinterpret_cast<LPVOID*>(&m_pLangCodePage),
							&m_langCodePageSize) )
						{
							m_langCodePageSize /= sizeof(DWORD);
							strFile.ReleaseBuffer();
							m_fileName = strFile;
							return true;
						}
					}
				}
			}
		}
		Close();
		return false;
	}

	/**
	 * [�ݒ�] �N���[�Y
	 */
	void Close(void)
	{
		m_workMem.Free();
		m_pFileInfo = NULL;
		m_pLangCodePage = NULL;
		m_langCodePageSize = 0;
		m_fileName.Empty();
	}

	/**
	 * [�擾] ���擾
	 *	@retval NULL�ȊO ���
	 *	@retval NULL ���s�i�I�[�v�����Ă��Ȃ��Ǝ��s���܂��j
	 */
	const VS_FIXEDFILEINFO* GetFileInfo(void) const
	{
		return m_pFileInfo;
	}

	/**
	 * [�擾] ���擾.
	 *	@note �擾�����������������āA WritePush() ���邱�ƂŁA�������������邱�Ƃ��o���܂��B
	 *	@retval NULL�ȊO ���
	 *	@retval NULL ���s�i�I�[�v�����Ă��Ȃ��Ǝ��s���܂��j
	 */
	VS_FIXEDFILEINFO* GetFileInfo(void)
	{
		return m_pFileInfo;
	}

	/**
	 * [�擾] ���擾(������)
	 *	@param lpszType �擾����B�ȉ��̕�����̓������ꂩ���w�肵�܂��B<BR>
	 *					"CompanyName"		- ��Ж�				<BR>
	 *					"FileDescription"	- ����					<BR>
	 *					"FileVersion"		- �t�@�C���E�o�[�W����	<BR>
	 *					"InternalName"		- ������				<BR>
	 *					"LegalCopyright"	- ���쌠				<BR>
	 *					"OriginalFileName"	- �����t�@�C����		<BR>
	 *					"ProductName"		- ���i��				<BR>
	 *					"ProductVersion"	- ���i�o�[�W����		<BR>
	 *					"Comments"			- �R�����g				<BR>
	 *					"LegalTrademarks"	- ���W					<BR>
	 *					"PrivateBuild"		- ��ײ�ްāE����ޏ��	<BR>
	 *					"SpecialBuild"		- ��߼�فE����ޏ��
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������Ԃ��܂��B
	 *	@retval ������ ����
	 *	@retval Empty��� ���s�i�I�[�v�����Ă��Ȃ��Ǝ��s���܂��j
	 */
	CStr Get(LPCTSTR lpszType, WORD wLanguage = 0) const
	{
		LPVOID P = m_GetTextPtr(lpszType, wLanguage);
		if ( P != NULL )
		{
			if ( HasUnicodeType() )
			{
				return CStr(reinterpret_cast<LPCWSTR>(P));
			}
			else
			{
				return CStr(reinterpret_cast<LPCSTR>(P));
			}
		}
		return CStr();
	}

	/**
	 * [�擾] About��ʗp������擾.
	 *	@param lpszVerText �o�[�W���������\�L�B
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������Ԃ��܂��B
	 *	@return ���s�܂񂾕�����
	 */
	CStr GetAboutText(LPCTSTR lpszVerText, WORD wLanguage = 0) const
	{
		if ( ! IsValid() ){ return CStr(); }
		return CStr::Fmt(_T("%s %s %s\n\n%s")
			, Get(_T("FileDescription"))
			, lpszVerText
			, GetFileVersionText()
			, GetCopyrightText()
		);
	}

	/**
	 * [�擾] ���쌠������擾.
	 *	@note "LegalCopyright" �� "CompanyName" ��A�������������Ԃ��܂��B
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������Ԃ��܂��B
	 *	@return ���쌠������
	 */
	CStr GetCopyrightText(WORD wLanguage = 0) const
	{
		if ( ! IsValid() ){ return CStr(); }
		return CStr::Fmt(_T("%s %s"),
				Get(_T("LegalCopyright"), wLanguage),
				Get(_T("CompanyName"), wLanguage));
	}

	/**
	 * [�擾] �t�@�C���o�[�W����������擾.
	 *	@note "FileVersion" �ƃt���O����쐬�����������Ԃ��܂��B
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������Ԃ��܂��B
	 *	@return ������
	 */
	CStr GetFileVersionText(WORD wLanguage = 0) const
	{
		return m_GetVersionText(_T("FileVersion"), wLanguage);
	}

	/**
	 * [�擾] �v���_�N�g�o�[�W����������擾.
	 *	@note "ProductVersion" �ƃt���O����쐬�����������Ԃ��܂��B
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������Ԃ��܂��B
	 *	@return ������
	 */
	CStr GetProductVersionText(WORD wLanguage = 0) const
	{
		return m_GetVersionText(_T("ProductVersion"), wLanguage);
	}

	/**
	 * [�擾] �X�y�V�����r���h�`�F�b�N.
	 *	@retval true �X�y�V�����r���h
	 *	@retval false ��
	 */
	bool IsSpecialBuild(void) const
	{
		if ( IsValid() )
		{
			DWORD f = m_pFileInfo->dwFileFlags & m_pFileInfo->dwFileFlagsMask;
			return (f & VS_FF_SPECIALBUILD) != 0;
		}
		return false;
	}

	/**
	 * [�擾] �v���C�x�[�g�r���h�`�F�b�N.
	 *	@retval true �v���C�x�[�g�r���h
	 *	@retval false ��
	 */
	bool IsPrivateBuild(void) const
	{
		if ( IsValid() )
		{
			DWORD f = m_pFileInfo->dwFileFlags & m_pFileInfo->dwFileFlagsMask;
			return (f & VS_FF_PRIVATEBUILD) != 0;
		}
		return false;
	}

#ifndef _WIN32_WCE

	/**
	 * [�ύX] ���ύX.
	 *	@note �t�@�C���̕ύX�͂��܂���B�C���X�^���X���̏���ύX���܂��B�t�@�C���ɔ��f������ɂ� WritePush() ���g���܂��B
	 *	@attention �V�K�ɐݒ肷�邱�Ƃ�A���̏���������������͐ݒ�ł��܂���B
	 *	@param lpszType �ύX����B
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������ύX���܂��B
	 *	@param lpszValue �ύX���镶����B
	 *	@retval �}�C�i�X �G���[
	 *	@retval 0�ȏ� �ύX����������.
	 */
	INT_PTR Modify(LPCTSTR lpszType, LPCTSTR lpszValue, WORD wLanguage = 0)
	{
		INT_PTR res = -1;
		if ( IsValid() )
		{
			LPVOID P = m_GetTextPtr(lpszType, wLanguage);
			if ( P != NULL )
			{
				if ( HasUnicodeType() )
				{
					LPWSTR lpsz = reinterpret_cast<LPWSTR>(P);
					res = STRLIB::GetLen(lpsz);
					CUnicode val = lpszValue;
					STRLIB::Copy(lpsz, val.Left(res));
				}
				else
				{
					LPSTR lpsz = reinterpret_cast<LPSTR>(P);
					res = STRLIB::GetLen(lpsz);
					CAscii val = lpszValue;
					STRLIB::Copy(lpsz, val.Left(res));
				}
			}
		}
		return res;
	}

	/**
	 * [�ύX] �t�@�C���o�[�W�����ύX.
	 *	@note �t�@�C���̕ύX�͂��܂���B�C���X�^���X���̏���ύX���܂��B�t�@�C���ɔ��f������ɂ� WritePush() ���g���܂��B
	 *	@param v1 �o�[�W�����P
	 *	@param v2 �o�[�W�����Q
	 *	@param v3 �o�[�W�����R
	 *	@param v4 �o�[�W�����S
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������ύX���܂��B
	 *	@retval �}�C�i�X �G���[
	 *	@retval 0�ȏ� �ύX����������.
	 */
	INT_PTR ModifyFileVersion(WORD v1, WORD v2, WORD v3, WORD v4, WORD wLanguage = 0)
	{
		if ( IsValid() )
		{
			CStr s;
			s.Format(_T("%d,%d,%d,%d"), v1, v2, v3, v4);
			m_pFileInfo->dwFileVersionMS = MAKELONG(v2, v1);
			m_pFileInfo->dwFileVersionLS = MAKELONG(v4, v3);
			return Modify(_T("FileVersion"), s, wLanguage);
		}
		return -1;
	}

	/**
	 * [�ύX] �v���_�N�g�o�[�W�����ύX.
	 *	@note �t�@�C���̕ύX�͂��܂���B�C���X�^���X���̏���ύX���܂��B�t�@�C���ɔ��f������ɂ� WritePush() ���g���܂��B
	 *	@param v1 �o�[�W�����P
	 *	@param v2 �o�[�W�����Q
	 *	@param v3 �o�[�W�����R
	 *	@param v4 �o�[�W�����S
	 *	@param wLanguage ����R�[�h�B�ȗ�����ƍŏ��Ɍ���������ύX���܂��B
	 *	@retval �}�C�i�X �G���[
	 *	@retval 0�ȏ� �ύX����������.
	 */
	INT_PTR ModifyProductVersion(WORD v1, WORD v2, WORD v3, WORD v4, WORD wLanguage = 0)
	{
		if ( IsValid() )
		{
			CStr s;
			s.Format(_T("%d,%d,%d,%d"), v1, v2, v3, v4);
			m_pFileInfo->dwProductVersionMS = MAKELONG(v2, v1);
			m_pFileInfo->dwProductVersionLS = MAKELONG(v4, v3);
			return Modify(_T("ProductVersion"), s, wLanguage);
		}
		return -1;
	}

	/**
	 * [�ݒ�] ��������.
	 *	@note Modify() �� ModifyFileVersion() , GetFileInfo() �Ȃǂɂ��ύX���������t�@�C���ɏ����o���܂��B
	 *	@retval true ����
	 *	@retval false �G���[
	 */
	bool WritePush(void)
	{
		if ( IsValid() )
		{
			HANDLE h = ::BeginUpdateResource(m_fileName, FALSE);
			if ( h != NULL )
			{
				bool r = !! ::UpdateResource(h, 
									RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
									m_workMem.Ref(), ToDword(m_workMem.GetSize()));
				r &= !! ::EndUpdateResource(h, FALSE);
				return r;
			}
		}
		return false;
	}

#endif // _WIN32_WCE

protected:

	/**
	 * [�ʒm] GetVersionText �R�[���ʒm.
	 *	@param[in,out] _str �o�[�W����������B �ҏW���邱�ƂŁA GetVersionText �̖߂�l�����H�ł��܂��B
	 *	@param[in] flag �t���O�B VS_FF_DEBUG �Ȃǂ̃t���O���͂����Ă܂��B
	 *	@param[in] wLanguage �����Q�[�W�B
	 */
	virtual void OnGetVerisonText(CStr& _str, DWORD flag, WORD wLanguage) const
	{
		if ( (flag & VS_FF_SPECIALBUILD) != 0 )
		{
			_str += _T(" ");
			_str += Get(_T("SpecialBuild"), wLanguage);
		}
		if ( (flag & VS_FF_PRIVATEBUILD) != 0 )
		{
			_str += _T(" ");
			_str += Get(_T("PrivateBuild"), wLanguage);
		}
		if ( (flag & VS_FF_PRERELEASE) != 0 )
		{
			_str += _T(" [PreRelease��]");
		}
		if ( (flag & VS_FF_DEBUG) != 0 )
		{
			_str += _T(" ��Debug�Ł�");
		}
	}

private:
	/// ����R�[�h�y�[�W�^
	struct TLangAndCodePage
	{
		WORD wLanguage;	///< ����
		WORD wCodePage;	///< �R�[�h�y�[�W
	};
	mutable CWorkMem	m_workMem;			///< ���[�N
	VS_FIXEDFILEINFO*	m_pFileInfo;		///< ���[�N����FIXEDFILEINFO�̃|�C���^
	TLangAndCodePage*	m_pLangCodePage;	///< ���[�N���̌���R�[�h�y�[�W�̃|�C���^
	UINT				m_langCodePageSize;	///< ����R�[�h�y�[�W�̐�
	CStr				m_fileName;			///< �����Ă���t�@�C����
	/// �|�C���^�擾
	LPVOID m_GetTextPtr(LPCTSTR lpszType, WORD wLanguage = 0) const
	{
		if ( IsValid() )
		{
			// �e����ƃR�[�h�y�[�W�̃t�@�C���̐�����ǂݎ��B
			for ( UINT i = 0; i < m_langCodePageSize; i++ )
			{
				if ( wLanguage == 0 || m_pLangCodePage[i].wLanguage == wLanguage )
				{
					CStr strKey;
					strKey.Format(
							_T("\\StringFileInfo\\%04x%04x\\%s"),
							m_pLangCodePage[i].wLanguage,		//Lang
							m_pLangCodePage[i].wCodePage,		//Code
							lpszType);
					// i �Ԗڂ̌���ƃR�[�h�y�[�W�̃t�@�C���̐������擾����B
					UINT dmy;
					if ( HasUnicodeType() )
					{
						CUnicode s = strKey;
						LPWSTR P;
						if ( ::VerQueryValueW(m_workMem,
								s.GetBuffer(),
								reinterpret_cast<LPVOID*>(&P),
								&dmy) )
						{
							return P;
						}
					}
					else
					{
						#ifndef _WIN32_WCE
						CAscii s = strKey;
						LPSTR P;
						if ( ::VerQueryValueA(m_workMem,
								s.GetBuffer(),
								reinterpret_cast<LPVOID*>(&P),
								&dmy) )
						{
							return P;
						}
						#endif
					}
				}
			}
		}
		return NULL;
	}

	/**
	 * [�m�F] UNICODE���\�[�X�m�F
	 *	@retval true UNICODE ���\�[�X��
	 *	@retval false ASCI ���\�[�X���A OPEN���Ă��Ȃ��B
	 */
	bool HasUnicodeType(void) const
	{
		bool r = false;
		if ( m_pFileInfo != NULL )
		{
			switch ( m_pFileInfo->dwFileOS )
			{
			case VOS_DOS_WINDOWS16:
			case VOS_OS216_PM16:
				break;
			default:
				r = true;
				break;
			}
		}
		return r;
	}

	/// �o�[�W���������쐬
	CStr m_GetVersionText(LPCTSTR lpszVer, WORD wLanguage = 0) const
	{
		CStr s;
		if ( IsValid() )
		{
			s = Get(lpszVer, wLanguage);
			DWORD f = m_pFileInfo->dwFileFlags & m_pFileInfo->dwFileFlagsMask;
			OnGetVerisonText(s, f, wLanguage);
		}
		return s;
	}
};



}; // TNB



#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��

/**
 * �t�@�C���o�[�W�������\����.
 *	@see https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms646997%28v=vs.85%29.aspx
 */
struct VS_FIXEDFILEINFO 
{ 
};

#endif // _TnbDOXYGEN


