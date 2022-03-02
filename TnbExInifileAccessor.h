#pragma once
/**
 *@file
 * �g��ini�t�@�C���A�N�Z�X�֌W�̃w�b�_
 *		
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifdef _WIN32_WCE
 #error not support "ExInifileAccessor"
#endif // _WIN32_WCE



#include "TnbInifileAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * �g��ini�t�@�C���A�N�Z�X�N���X
 *
 *		ini�t�@�C���ɃA�N�Z�X����֐��Q�ł��BCInifileAccessor�ƈقȂ�A�^�����Ǘ����Ă��邽�߁A
 *		�݊����L��܂���(�L�[�̒l�Ɍ^��Prefix������ĕ��̊Ǘ������Ă��܂�)�B
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
 *	@date 06/05/16 �V�K�쐬
 *	@date 06/07/13 �L�[�A�l�� =(�C�R�[��)�A���s���܂܂�Ă��Ă�OK�ɂȂ�悤�ɂ����B
 *	@date 06/10/31 IAccessor �̉��C�ɑΉ��B
 */
class CExInifileAccessor : public CInifileAccessor
{
	DEFSUPER(CInifileAccessor);
	enum { E_BufferSize = 65536 };		///< �擾�ő�T�C�Y
	/**
	 * [�m�F] ��ރ`�F�b�N
	 *	@param lpsz ������
	 *	@return ���
	 */
	static EKind ms_CheckKind(LPCTSTR lpsz)
	{
		if ( lpsz[0] != '_' || lpsz[2] != '_' )
		{
			return EK_Nothing;
		}
		switch ( lpsz[1] )
		{
		case EK_String:			// ������
		case EK_PluralStrings:	// �����̕�����Q
		case EK_Binary:			// �o�C�i��
		case EK_Dword:			// 32Bit Unsigned
		case EK_Longlong:		// 64Bit Signed
			break;
		default:
			return EK_Nothing;
		}
		return static_cast<EKind>(lpsz[1]);
	}

	/**
	 * �����ݗp������쐬
	 *	@param lpsz �I���W�i��������
	 *	@return �����ݗp������
	 */
	static CStr ms_ToWritingStr(LPCTSTR lpsz)
	{
		CStr s = lpsz;
		if ( s.IsEmpty() )
		{
			s = _T("@");
		}
		else
		{
			s.Replace(_T("\\"), _T("\\\\"));
			s.Replace(_T("\n"), _T("\\n"));
			s.Replace(_T("\r"), _T("\\r"));
			s.Replace(_T("="), _T("\\-"));
			s.Replace(_T(" "), _T("\\_"));
		}
		return s;
	}

	/**
	 * �I���W�i��������쐬
	 *	@param lpsz �����ݗp������
	 *	@return �I���W�i��������
	 */
	static CStr ms_ToOriginalStr(LPCTSTR lpsz)
	{
		CStr s = lpsz;
		s.Replace(_T("\\_"), _T(" "));
		s.Replace(_T("\\n"), _T("\n"));
		s.Replace(_T("\\r"), _T("\r"));
		s.Replace(_T("\\-"), _T("="));
		s.Replace(_T("\\\\"), _T("\\"));
		return s;
	}

	/**
	 * [�ǉ�] ��������L�^
	 *	@param eKind �������ގ��
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param lpszData �L�[�̒l�i�����񉻂������́j
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_Write(EKind eKind, LPCTSTR lpszSectionName, LPCTSTR lpszKey, LPCTSTR lpszData)
	{
		CStr s = CStr::Fmt(_T("_%c_%s"), eKind, lpszData);
		return !! ::WritePrivateProfileString(
							lpszSectionName,
							ms_ToWritingStr(lpszKey),
							ms_ToWritingStr(s),
							GetBaseFileName()
		);
	}

	/**
	 * [�擾] ��������擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
	 */
	CStr m_Query(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CStr strValue;
		::GetPrivateProfileString(
								lpszSectionName,
								ms_ToWritingStr(lpszKey),
								_T(""),
								strValue.GetBuffer(E_BufferSize),
								E_BufferSize,
								GetBaseFileName());
		strValue.ReleaseBuffer();
		strValue = ms_ToOriginalStr(strValue); 
		return strValue.Mid(3);
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param lpszFile �t�@�C���� �B�ȗ�����ƁA MakeInifilePath() �̌��ʂ��g���܂��B
	 */
	explicit CExInifileAccessor(LPCTSTR lpszFile = NULL) : _super(lpszFile)
	{
	}

	/**
	 * [�擾] �L�[���ꗗ�擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return ���O�ꗗ�B������ԂȂ疢�Ή��B
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		CStrVector vstr = _super::EnumKeyNames(lpszSectionName);
		loop ( i, vstr.GetSize() )
		{
			vstr[i] = ms_ToOriginalStr(vstr[i]);
		}
		return vstr;
	}

	/**
	 * [�擾] �����擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return ����B�Ȃ��ꍇ�AEK_Nothing �ɂȂ�܂��B
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		TCHAR atcBuf[16];
		DWORD dwRc = ::GetPrivateProfileString(
								lpszSectionName,
								ms_ToWritingStr(lpszKey),
								_T(""),
								atcBuf,
								16,
								GetBaseFileName());
		if ( dwRc < 3 )
		{
			return EK_Nothing;
		}
		return ms_CheckKind(atcBuf);
	}

	/**
	 * [�擾] ���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
	 *	@throw CEmptyException	��񂪂Ȃ����A�X���[����܂��B
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CValue v;
		CStr str = m_Query(lpszSectionName, lpszKey);
		switch ( GetKeyKind(lpszSectionName, lpszKey) )
		{
		case EK_Nothing:
			break;
		case EK_String:
			v = CValue(str);
			break;
		case EK_PluralStrings:
			v = CValue(StrToStrVectorEx(str));
			break;
		case EK_Binary:
			v = CValue(CStrOperator::HexStringToBinary(str));
			break;
		case EK_Dword:
			v = CValue(static_cast<DWORD>(str.ToLonglong()));
			break;
		case EK_Longlong:
			v = CValue(str.ToLonglong());
			break;
		default:
			ASSERT(false);
			break;
		}
		return v;
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
		EKind kind = value.GetKind();
		if ( kind == EK_Nothing )
		{
			return _super::WriteValue(lpszSectionName, lpszKey, value);
		}
		return m_Write(kind, lpszSectionName, lpszKey, value.QueryString());
	}
};



}; // TNB
