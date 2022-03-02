#pragma once
/**
 *@file
 * ini�t�@�C���A�N�Z�X�֌W�̃w�b�_
 *		
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE

#include "TnbAccessor.h"
#include "TnbDntStr.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * ini�t�@�C���A�N�Z�X�N���X
 *
 *		ini�t�@�C���ɃA�N�Z�X����֐��Q�ł��B
 *		API�� GetPrivateProfile�` �֐� SetPrivateProfile�` �ƃt�@�C���݊����L��܂��B
 *		
 *		�������A���ׂĂɂ����āA������Ƃ��ď������Ă��܂��B
 *
 *		�R���X�g���N�^�� SetBase() �ŏ��̃x�[�X���w�肵�A
 *		operator[]() �� �Z�N�V�������A�N�Z�X�N���X({@link IAccessor::CSection CSection})���擾���A
 *		Query�`() �A Write�`() �ȂǂŃL�[���ɃA�N�Z�X���܂��B
 *
 *	@note	�{�C���^�[�t�F�[�X�ɂ� QueryValue() , WriteValue() �ȂǁA
 *			�Z�N�V�������w�肵�L�[���𑀍삷�郁�\�b�h������܂����A
 *			��{�I�� {@link IAccessor::CSection CSection} ���g�p���Ă��������B
 *
 *	@see	IAccessor ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbInifileAccessor.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 06/04/20 CString���g�p��
 *	@date 06/05/08 CDoubleNullTerminateStr���g�p����悤�ɂ����B
 *	@date 06/05/16 IAccessor ����������悤�ɂ����B
 *	@date 06/06/08 �����o���ύX�B
 *	@date 06/10/31 IInformation �̉��C�ɑΉ��B
 *	@date 07/05/10 �N���X���ύX�B
 *	@date 08/04/03 �f�t�H���g�l�̈������C���B
 *	@date 09/07/28 NewBaseFile() �ǉ��B
 *	@date 11/09/15 �������ލہA��ɓǂݏo���A�����Ȃ牽�����Ȃ��悤�ɂ����B
 *	@date 11/11/07 ��L�Ή��Ƀo�O����B�ꕶ���̎��ɁA�������������߂Ȃ��\�����������B
 */
class CInifileAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
public:

	/**
	 * �R���X�g���N�^
	 *	@param lpszFile �t�@�C���� �B�ȗ�����ƁA MakeInifilePath() �̌��ʂ��g���܂��B
	 */
	explicit CInifileAccessor(LPCTSTR lpszFile = NULL) : _super()
	{
		CInifileAccessor::SetBase(lpszFile); 
	}

	/**
	 * [�ݒ�] �Ώۃt�@�C���w��
	 *	@param lpszFile �t�@�C����
	 */
	void SetBase(LPCTSTR lpszFile = NULL)
	{
		m_strFile = (lpszFile == NULL) ? MakeInifilePath() : lpszFile;
	}

	/**
	 * [�쐬] ini�t�@�C���p�X�쐬.
	 *		���s�t�@�C������ .exe�� .ini�ɕς������̂��쐬���܂��B
	 *	@return ini�t�@�C����FullPath
	 */
	static CStr MakeInifilePath(void)
	{
		CStr str = _super::MakeDefineFilePath();
		str += _T(".ini");
		return str;
	}

	/**
	 * [�擾] �Ώۃt�@�C���擾
	 *	@return �t�@�C����
	 */
	CStr GetBaseFileName(void) const
	{
		return m_strFile;
	}

	/**
	 * [�m�F] �Ώۃt�@�C�����݊m�F.
	 *		�Ώۂ� ini �t�@�C�����A���ݑ��݂��Ă��邩�A�m�F�ł��܂��B
	 *	@note �t�@�C���̗L���̂݁A�`�F�b�N���Ă��܂��B
	 *	@retval true ����B
	 *	@retval false �����B
	 */
	bool ExistBaseFile(void) const
	{
		DWORD r = ::GetFileAttributes(m_strFile);
		return ToInt(r) >= 0;
	}

	/**
	 * [�쐬] �Ώۃt�@�C���쐬.
	 *		�Ώۃt�@�C����V�K���܂��B���܂ł̏��͂��ׂĂȂ��Ȃ�܂��B
	 *	@note �ʏ�A�g���K�v�͂���܂��񂪁A�S�����������ꍇ�A UNICODE �`���ɂ������ꍇ�Ɏg���܂��B
	 *	\code
	 *		CInifileAccessor ini;
	 *		if ( ! ini.ExistBaseFile() )
	 *		{
	 *			ini.NewBaseFile(true); //unicode ��ini�t�@�C���ɂ���
	 *		}
	 *	\endcode
	 *	@param isUnicode false �Ȃ� ASCII(S-JIS)�A true �Ȃ� UNICODE �őΏۃt�@�C����V�K�쐬���܂��B
	 *	@retval true ����.
	 *	@retval false ���s
	 */
	bool NewBaseFile(bool isUnicode = false)
	{
		bool r = false;
		HANDLE h = ::CreateFile(m_strFile, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, 0, NULL);
		// ������
		if ( h != INVALID_HANDLE_VALUE )
		{
			if ( isUnicode )
			{
				BYTE buf[2] = { 0xFF, 0xFE };
				DWORD writed;
				if ( ::WriteFile(h, buf, 2, &writed, NULL) && 2 == writed )
				{
					r = true;
				}
			}
			else
			{
				r = true;
			}
			::CloseHandle(h);
		}
		return r;
	}

	/**
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Inifile");
	}

	/**
	 * [����] �t���b�V��.
	 *	@note ��������ɃL���b�V��������Ώ����o���܂��B
	 *	@return true �i�����j
	 */
	virtual bool Flush(void) 
	{
		::WritePrivateProfileString(NULL, NULL, NULL, m_strFile);	//�߂�l�͕K��false(�d�l)�Ȃ̂Ō��Ȃ�
		return true;
	}

	/**
	 * [�擾] �Z�N�V�������ꗗ�擾
	 *	@param lpszSectionName �Z�N�V�������B NULL���w�肷��ƁAROOT�̃Z�N�V�����ꗗ�������܂��B
	 *	@return �Z�N�V�������ꗗ
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const
	{
		CStrVector vs;
		CDoubleNullTerminateStr dntstr;
		DWORD dwRc = ::GetPrivateProfileSectionNames(
							dntstr.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		dntstr.ReleaseBuffer();
		if ( dwRc <= 0 )
		{
			return vs;
		}
		vs = dntstr.ToStrVector();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		if ( ! ss.IsEmpty() )
		{
			loop ( i, vs.GetSize() )
			{
				CStr s;
				if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
				{
					s = vs[i].Mid(len);
					if ( ! s.IsEmpty() && s[0] == '\\' )
					{
						s = s.Mid(1);
					}
					else
					{
						s.Empty();
					}
				}
				vs[i] = s;
			}
		}
		CStrVector vstrSecNames;
		loop ( i, vs.GetSize() )
		{
			if ( ! vs[i].IsEmpty() )
			{
				CStr s = vs[i];
				INDEX d = s.Find(_T('\\'));
				if ( d != INVALID_INDEX )
				{
					s = s.Left(d);
				}
				if ( vstrSecNames.Find(s) == INVALID_INDEX )
				{
					vstrSecNames.Add(s);
				}
			}
		}
		return vstrSecNames;
	}

	/**
	 * [�폜] �w��Z�N�V�����폜
	 *	@param lpszSectionName �Z�N�V�������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		CStrVector vs;
		CDoubleNullTerminateStr dntstr;
		DWORD dwRc = ::GetPrivateProfileSectionNames(
							dntstr.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		dntstr.ReleaseBuffer();
		if ( dwRc <= 0 )
		{
			return false;
		}
		vs = dntstr.ToStrVector();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		loop ( i, vs.GetSize() )
		{
			if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
			{
				if ( ! ::WritePrivateProfileString(vs[i], NULL, NULL, m_strFile) )
				{
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * [�擾] ���O�ꗗ�擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return ���O�ꗗ�B������ԂȂ疢�Ή��B
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		CStrVector vstrKeyNames;
		CDoubleNullTerminateStr dntstr;
		DWORD dwRc = ::GetPrivateProfileSection(
							lpszSectionName,
							dntstr.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		dntstr.ReleaseBuffer();
		if ( dwRc > 0 )
		{
			CStrVector vstr = dntstr.ToStrVector();
			loop ( i, vstr.GetSize() )
			{
				vstrKeyNames.Add(vstr[i].FindCut('='));
			}
		}
		return vstrKeyNames;
	}

	/**
	 * [�擾] �����擾
	 *	@note �{�N���X�ł� EK_String �� EK_Nothing �ɂȂ�܂��B
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return ����B�Ȃ��ꍇ�AEK_Nothing �ɂȂ�܂��B
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CStr dmy;
		if ( ! m_QueryString(dmy, lpszSectionName, lpszKey) )
		{
			return EK_Nothing;
		}
		return EK_String;
	}

	/**
	 * [�擾] ���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CStr s;
		if ( m_QueryString(s, lpszSectionName, lpszKey) )
		{
			return CValue(s);
		}
		return CValue();
	}

	/**
	 * [�ݒ�] ���ݒ�
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param value �ݒ肷��l�BEK_Nothing �̏ꍇ�A�L�[���폜���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 *	@throw CTnbException �ݒ莸�s���A�X���[�����\��������܂��B
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		if ( value.GetKind() == EK_Nothing )
		{
			return m_Delete(lpszSectionName, lpszKey);
		}
		return m_WriteString(lpszSectionName, lpszKey, value.QueryString());
	}

private:
	enum { E_BufferSize = 65536 };		///< �擾�ő�T�C�Y

	/**
	 * [�擾] ��������擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param lpszDefault �L�[���Ȃ������̒l��Ԃ��܂��B NULL�͎w��ł��܂���B
	 *	@return �L�[�̒l
	 */
	CStr m_GetString(LPCTSTR lpszSectionName, LPCTSTR lpszKey, LPCTSTR lpszDefault) const
	{
		ASSERT( lpszDefault != NULL );
		CStr strValue;
		::GetPrivateProfileString(
							lpszSectionName,
							lpszKey,
							lpszDefault,
							strValue.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		strValue.ReleaseBuffer();
		return strValue;
	}

	/**
	 * [�擾] ��������擾
	 *	@param[out] _r �擾������
	 *	@param[in] lpszSectionName �Z�N�V������
	 *	@param[in] lpszKey �L�[��
	 *	@retval true �擾����
	 *	@retval false �w��̃Z�N�V�����A�L�[���Ȃ������B
	 */
	bool m_QueryString(CStr& _r, LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		_r = m_GetString(lpszSectionName, lpszKey, _T("*1"));
		if ( _r.Compare(_T("*1")) == 0 )
		{
			_r = m_GetString(lpszSectionName, lpszKey, _T("*2"));
			if ( _r.Compare(_T("*2")) == 0 )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [�ǉ�] ��������L�^
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param lpszData �L�[�̒l
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_WriteString(LPCTSTR lpszSectionName, LPCTSTR lpszKey, LPCTSTR lpszData)
	{
		DWORD len = down_cast<DWORD>(STRLIB::GetLen(lpszData));
		CWorkMemT<TCHAR> work(len + 2);
		DWORD r = ::GetPrivateProfileString(lpszSectionName, lpszKey, _T(""), work, len + 2, m_strFile);
		if ( r == len && STRLIB::Compare(lpszData, work) == 0 )
		{
			return true;
		}
		return !! ::WritePrivateProfileString(lpszSectionName, lpszKey, lpszData, m_strFile);
	}

	/**
	 * [�폜] �w��L�[�폜
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_Delete(LPCTSTR lpszSectionName, LPCTSTR lpszKey)
	{
		if ( GetKeyKind(lpszSectionName, lpszKey) != EK_Nothing )
		{
			return !! ::WritePrivateProfileString(lpszSectionName, lpszKey, NULL, m_strFile);
		}
		return false;
	}

	CStr		m_strFile;				///< �����t�@�C����
	CStr		m_strSection;			///< �����Z�N�V������
};



}; // TNB

#else // _WIN32_WCE

#include "TnbCeInifileAccessor.h"

#endif // _WIN32_WCE

