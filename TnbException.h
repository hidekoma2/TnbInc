#pragma once
/**
 *@file
 * ��O��ԊǗ��֌W�̃w�b�_
 *
 *		TNB Library�Ŕ��������O�N���X��Base�N���X��錾���Ă��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrLib.h"
#include "TnbPointerHandle.h"



#include <tchar.h>



//TNB Library
namespace TNB
{



/**@ingroup EXCEPTION
 * ��O�x�[�X�N���X
 *
 *	@note ��O�����͂��̃N���X���x�[�X�ɂ��܂��B
 *
 *	@date 06/05/03	�V�K�쐬�B
 */
class CTnbException
{
	CArrayPtrHandleT<TCHAR>	m_hpText;
	DWORD					m_dwParam;
public:
	/**
	 * �R���X�g���N�^
	 *	@param lpszMsg ������
	 *	@param dwParam �p�����[�^
	 */
	explicit CTnbException(LPCTSTR lpszMsg, DWORD dwParam = 0)
	{
		if ( lpszMsg == NULL ){ lpszMsg = _T(""); }
		TRACE1("throw Tnb-Exception;[%s]\n", lpszMsg);
		size_t iSize = STRLIB::GetLen(lpszMsg);
		m_hpText = new TCHAR[iSize + 1];
		STRLIB::Copy(m_hpText, lpszMsg);
		m_dwParam = dwParam;
	}
	/**
	 * [�擾] �p�����[�^�擾
	 *	@return �p�����[�^
	 */
	DWORD GetParam(void) const { return m_dwParam; }
	/**
	 * [�擾] ��O������擾
	 *	@return ������
	 */
	virtual LPCTSTR GetString(void) const { return m_hpText; }
	/**
	 * [�\��] ���e�\��
	 *	@note catch �����Ƃ��A���̃��\�b�h���R�[������悤�ɂ��Ă�������
	 *	@note �f�o�O�r���h�̎��A���e��\�����܂��B�����[�X�ł̂Ƃ��́A�������܂���B
	 */
	void OnCatch(void) const
	{
		TRACE1("catch Tnb-Exception;[%s]\n", GetString());
	}
};



/**@ingroup EXCEPTION
 * INDEX�͈͊O��O
 */
class CIndexOutOfBoundsException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CIndexOutOfBoundsException(DWORD dwParam = 0) : _super(_T("INDEX���͈͊O�ł��B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * �ǉ����s��O
 */
class CAppendFailureException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CAppendFailureException(DWORD dwParam = 0) : _super(_T("�ǉ��Ɏ��s���܂����B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * �擾�v�f(��)������O
 */
class CEmptyException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CEmptyException(DWORD dwParam = 0) : _super(_T("��ł��B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * �f�[�^���炸������O
 */
class CShortOfDataException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CShortOfDataException(DWORD dwParam = 0) : _super(_T("�f�[�^������܂���B"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * �����ُ픭����O
 */
class CInvalidLengthException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CInvalidLengthException(DWORD dwParam = 0) : _super(_T("�������ُ�ł��B"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * �^�C�v�����O
 */
class CNoSuchTypeException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CNoSuchTypeException(DWORD dwParam = 0) : _super(_T("�^�C�v���قȂ�܂��B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * �p�����[�^�s����O
 */
class CInvalidParamException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CInvalidParamException(DWORD dwParam = 0) : _super(_T("�p�����[�^���s���ł��B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * NULL�|�C���^��O
 */
class CNullPointerException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CNullPointerException(DWORD dwParam = 0) : _super(_T("NULL�|�C���^�ł��B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * �T�|�[�g�O��O
 */
class CNotSupportException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CNotSupportException(DWORD dwParam = 0) : _super(_T("�T�|�[�g����Ă��܂���B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * ���t����Ȃ���O
 */
class CNotFoundException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CNotFoundException(DWORD dwParam = 0) : _super(_T("���t����܂���B"), dwParam) {}
};



/**@ingroup EXCEPTION
 * �ǂݏ������s������O
 *	@note Param�ɂ̓V�X�e���G���[�R�[�h�������܂��B
 */
class CIoFailureException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// �R���X�g���N�^
	CIoFailureException(LPCTSTR lpszMsg = NULL, DWORD dwParam = 0)
		: _super((lpszMsg == NULL) ? _T("���o�͎��s�ł��B") : lpszMsg, dwParam) {}
}; 



/**@ingroup EXCEPTION
 * �I�[�v�����s������O
 *	@note Param�ɂ̓V�X�e���G���[�R�[�h�������܂��B
 */
class COpenFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// �R���X�g���N�^
	COpenFailureException(DWORD dwParam) : _super(_T("�I�[�v�����s�ł��B"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * �ǂݍ��ݎ��s������O
 *	@note Param�ɂ̓V�X�e���G���[�R�[�h�������܂��B
 */
class CReadFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// �R���X�g���N�^
	CReadFailureException(DWORD dwParam) : _super(_T("�ǂݍ��ݎ��s�ł��B"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * �������ݎ��s������O
 *	@note Param�ɂ̓V�X�e���G���[�R�[�h�������܂��B
 */
class CWriteFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// �R���X�g���N�^
	CWriteFailureException(DWORD dwParam) : _super(_T("�������ݎ��s�ł��B"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * �폜���s������O
 *	@note Param�ɂ̓V�X�e���G���[�R�[�h�������܂��B
 */
class CDeleteFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// �R���X�g���N�^
	CDeleteFailureException(DWORD dwParam) : _super(_T("�폜���s�ł��B"), dwParam) {}
}; 



}; //TNB
