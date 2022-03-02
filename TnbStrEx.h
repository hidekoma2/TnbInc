#pragma once
/**
 *@file
 * �����񏈗��֌W�̃w�b�_
 *
 *		CStrT �N���X���g�����A�ėp�I�Ȋ֐����L��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user, "T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * [�擾] PC���擾
 *	@note WindowCE�ł́AEmpty�����������܂��B
 *	@return PC���擾
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CStr GetComputerName(void)
{
	CStr str;
	#ifndef _WIN32_WCE
		DWORD dwLength = MAX_COMPUTERNAME_LENGTH + 1;
		::GetComputerName(str.GetBuffer(dwLength), &dwLength);
		str.ReleaseBuffer();
	#else
		str = _T("CE");
	#endif
	return str;
}



/**@ingroup STRING
 * [�擾] �v���Z�X�̃p�X�擾.
 *		���݂̃v���Z�X�̃p�X���擾���܂��B
 *	@return �p�X(������̍Ō�Ƀp�X��؂蕶���͂��܂���)
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CStr GetProcessPath(void)
{
	CStr str;
	DWORD dwRc = ::GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);
	str.ReleaseBuffer();
	if ( dwRc != 0 )
	{
//		INT_PTR p1 = str.ReverseFind('\\');
//		INT_PTR p2 = str.ReverseFind('/');
//		INT_PTR p0 = (p1 < p2) ? p2 : p1;
		INT_PTR p = str.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return str.Left(p);
		}
	}
	return str;
}



/**@ingroup STRING
 * [�擾] �v���Z�X�l�[���擾.
 *		���݂̃v���Z�X�l�[�����擾���܂��B
 *	@return �l�[��
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CStr GetProcessName(void)
{
	CStr str;
	DWORD dwRc = ::GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);
	str.ReleaseBuffer();
	if ( dwRc != 0 )
	{
//		INT_PTR p1 = str.ReverseFind('\\');
//		INT_PTR p2 = str.ReverseFind('/');
//		INT_PTR p0 = (p1 < p2) ? p2 : p1;
		INT_PTR p = str.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return str.Mid(static_cast<INDEX>(p + 1));
		}
	}
	return str;
}



/**@ingroup STRING
 * [�擾] ���\�[�X������擾
 *	@param nID Windows�̕����񃊃\�[�XID
 *	@param hInst �C���X�^���X�n���h���B
 *			�ȗ�����ƁA GetInstanceHandle(EI_String) �œ�����n���h�����g�p���܂��B
 *			MFC�v���W�F�N�g�łȂ����A SetInstanceHandle(EI_String) �Őݒ肳�ꂽ�n���h�����g�p���܂��B
 *	@return ������
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CStr LoadStr(UINT nID, HINSTANCE hInst = NULL)
{
	if ( hInst == NULL )
	{
		hInst = GetInstanceHandleByTnb(EI_String);
	}
	CStr str;
	int iLen = 256;
	while ( true )
	{
		int r = ::LoadString(hInst, nID, str.GetBuffer(iLen), iLen);
		str.ReleaseBuffer();
		if ( r == 0 )
		{
			str.Empty();
			break;
		}
		if ( r < iLen - 2 )
		{
			break;
		}
		iLen *= 2;
	}
	return str;
}



/**@ingroup STRING
 * [�ϊ�] �����ϊ�(ASCII/SJIS�p)
 *	@note �S�p�A���p�A���ȁA�J�i�A�啶���A�������̕ϊ��B
 *	@see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/nls_5s2v.asp
 *	@param lpsz ���̕�����
 *	@param dwFlags �ړI�Ƃ���ϊ�<BR>
 *					LCMAP_LOWERCASE	- �p�啶�����������֕ϊ�	<BR>
 *					LCMAP_FULLWIDTH	- ���p������S�p�����֕ϊ�	<BR>
 *					LCMAP_HALFWIDTH	- �S�p�����𔼊p�����֕ϊ�	<BR>
 *					LCMAP_HIRAGANA	- �J�^�J�i���Ђ炪�Ȃ֕ϊ�	<BR>
 *					LCMAP_KATAKANA	- �Ђ炪�Ȃ��J�^�J�i�֕ϊ�	<BR>
 *					LCMAP_UPPERCASE	- �p��������啶���֕ϊ�	<BR>
 *					LCMAP_LOWERCASE	- �p�啶�����������֕ϊ�	
 *	@return �ϊ���̕�����
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CAscii LocalMapString(LPCSTR lpsz, DWORD dwFlags)
{
	// ���p���S�p�ϊ��ōő�2�{�̃T�C�Y�ɂȂ�
	size_t iLen = (STRLIB::GetLen(lpsz) + 1) * 2;
	CWorkMemT<char> work(iLen);
	int iRc = ::LCMapStringA(::GetUserDefaultLCID(),
				dwFlags,
				lpsz, -1,	// ���͕�����ƒ���(NULL�I�[�Ȃ璷����-1�w��\)
				work, ToInt(iLen));// �ϊ���̕�������i�[����o�b�t�@�ƃo�b�t�@��
	if ( iRc <= 0 )
	{
		work.Free();
	}
	return CAscii(work);
}



/**@ingroup STRING
 * [�ϊ�] �����ϊ�(UNICODE�p)
 *	@note �S�p�A���p�A���ȁA�J�i�A�啶���A�������̕ϊ��B
 *	@see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/nls_5s2v.asp
 *	@param lpsz ���̕�����
 *	@param dwFlags �ړI�Ƃ���ϊ�<BR>
 *					LCMAP_LOWERCASE	- �p�啶�����������֕ϊ�	<BR>
 *					LCMAP_FULLWIDTH	- ���p������S�p�����֕ϊ�	<BR>
 *					LCMAP_HALFWIDTH	- �S�p�����𔼊p�����֕ϊ�	<BR>
 *					LCMAP_HIRAGANA	- �J�^�J�i���Ђ炪�Ȃ֕ϊ�	<BR>
 *					LCMAP_KATAKANA	- �Ђ炪�Ȃ��J�^�J�i�֕ϊ�	<BR>
 *					LCMAP_UPPERCASE	- �p��������啶���֕ϊ�	<BR>
 *					LCMAP_LOWERCASE	- �p�啶�����������֕ϊ�	
 *	@return �ϊ���̕�����
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CUnicode LocalMapString(LPCWSTR lpsz, DWORD dwFlags)
{
	size_t iLen = STRLIB::GetLen(lpsz) + 1;
	CWorkMemT<WCHAR> work(iLen);
	int iRc = ::LCMapStringW(::GetUserDefaultLCID(),
				dwFlags,
				lpsz, -1,	// ���͕�����ƒ���(NULL�I�[�Ȃ璷����-1�w��\)
				work, ToInt(iLen));// �ϊ���̕�������i�[����o�b�t�@�ƃo�b�t�@��
	if ( iRc <= 0 )
	{
		work.Free();
	}
	return CUnicode(work);
}



/**@ingroup STRING
 * [�ϊ�] �������ϊ�(ASCII/SJIS�p)
 *	@note �������֕ϊ�������������쐬�B
 *	@param lpsz ���̕�����
 *	@return �ϊ���̕�����
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CAscii LowerString(LPCSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_LOWERCASE);
}



/**@ingroup STRING
 * [�ϊ�] �������ϊ�(UNICODE�p)
 *	@note �������֕ϊ�������������쐬�B
 *	@param lpsz ���̕�����
 *	@return �ϊ���̕�����
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CUnicode LowerString(LPCWSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_LOWERCASE);
}



/**@ingroup STRING
 * [�ϊ�] �啶���ϊ�(ASCII/SJIS�p)
 *	@note �啶���֕ϊ�������������쐬�B
 *	@param lpsz ���̕�����
 *	@return �ϊ���̕�����
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CAscii UpperString(LPCSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_UPPERCASE);
}



/**@ingroup STRING
 * �啶���ϊ�(UNICODE�p)
 *	@note �啶���֕ϊ�������������쐬�B
 *	@param lpsz ���̕�����
 *	@return �ϊ���̕�����
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CUnicode UpperString(LPCWSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_UPPERCASE);
}



/**@ingroup STRING
 * [�ϊ�] SystemError�R�[�h������
 *	@param dwError GetLastError() �̖߂�l�ȂǁASystemError�R�[�h�B
 *	@param withRowValue true �Ȃ琶���l�����ʕ��ŕ�����̍Ō�ɕt�����܂��B
 *	@return ������
 *	@par�K�v�t�@�C��
 *			TnbStrEx.h
 */
inline CStr SystemErrorToMessageText(DWORD dwError, bool withRowValue = false)
{
	LPTSTR lpBuffer = NULL;
	const DWORD dwFlag	= FORMAT_MESSAGE_ALLOCATE_BUFFER 
						| FORMAT_MESSAGE_FROM_SYSTEM
						| FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD r = ::FormatMessage( 
					dwFlag,
					NULL,
					dwError,
					LANG_USER_DEFAULT,
					reinterpret_cast<LPTSTR>(&lpBuffer),
					0,
					NULL);
	CStr strErrMessage;
	if ( r == 0 )
	{
		strErrMessage = _T("Unknown");
	}
	else
	{
		strErrMessage = lpBuffer;
		strErrMessage.TrimRight(_T("\n\r"));
		::LocalFree(lpBuffer);
	}
	if ( withRowValue )
	{
		strErrMessage += CStr::Fmt(_T("(%d)"), dwError);
	}
	return strErrMessage;
}



}; //TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")

