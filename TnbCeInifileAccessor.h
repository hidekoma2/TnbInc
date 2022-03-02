#pragma once
/**
 *@file
 * CE��p INI �t�@�C�����A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeInifileAccessor.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbInifileTextAccessor.h"
#include "TnbFileName.h"
#include "TnbFile.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR WINCE
 * ini�t�@�C�����A�N�Z�X�N���X(CE��p)
 *
 *		�Z�N�V�����ŋ�؂�ꂽ���ɕ����̃L�[�ƒl�����`�̏����������ŊǗ����A
 *		�C�ӂ̃^�C�~���O�ŁA�t�@�C���ɕۑ����Ă����A������𕜌����邱�Ƃ�
 *		�o����N���X�ł��B
 *
 *		�R���X�g���N�^�� SetBase() �ŏ��̃x�[�X���w�肵�A
 *		operator[]() �� �Z�N�V�������A�N�Z�X�N���X({@link IAccessor::CSection CSection})���擾���A
 *		Query�`() �A Write�`() �ȂǂŃL�[���ɃA�N�Z�X���܂��B
 *
 *	@attention Write�`() �ŏ���ύX���Ă��A Flush() ���R�[�����Ȃ�����t�@�C���ɏ����o���܂���B
 *
 *	@todo ���݃T�u�Z�N�V�����������s���ł��B
 *
 *	@note	�{�C���^�[�t�F�[�X�ɂ� QueryValue() , WriteValue() �ȂǁA
 *			�Z�N�V�������w�肵�L�[���𑀍삷�郁�\�b�h������܂����A
 *			��{�I�� {@link IAccessor::CSection CSection} ���g�p���Ă��������B
 *
 *	@see	IIAccessor ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeInifileAccessor.h
 *
 *	@date 08/07/02 �V�K�쐬
 *	@date 12/04/26 �e�N���X�ύX
 */
class CCeInifileAccessor : public CInifileTextAccessor
{
	DEFSUPER(CInifileTextAccessor);
public:
	
	/** 
	 * �R���X�g���N�^
	 *	@param lpszFile �t�@�C���� �B�ȗ�����ƁA MakeInifilePath() �̌��ʂ��g���܂��B	 
	 */
	explicit CCeInifileAccessor(LPCTSTR lpszFile = NULL) : _super()
	{
		SetBase(lpszFile);
	}

	/**
	 * [�ݒ�] �Ώۃt�@�C���w��
	 *	@param lpszFile �t�@�C����
	 */
	void SetBase(LPCTSTR lpszFile = NULL)
	{
		m_strFile = (lpszFile == NULL) ? MakeInifilePath() : lpszFile;
		m_Read(m_strFile);
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
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Inifile");
	}

	/**
	 * [����] �t���b�V��.
	 *	@note �R���X�g���N�^�� SetBase �Ŏw�肵���t�@�C���ɕύX�������o���܂��B
	 *		���̃��\�b�h���R�[�����Ȃ�����A�t�@�C���͕ω����܂���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool Flush(void)
	{
		CFileName fn = m_strFile;
		try
		{
			m_Write(fn);
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			return false;
		}
		catch(...)
		{
			return false;
		}
		return true;
	}

private:
	/**
	 * [�ݒ�] �e�L�X�g�ݒ�.
	 *	@param lpszText �e�L�X�g
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetText(LPCTSTR lpszText);

	/**
	 * [�擾] �e�L�X�g�擾.
	 *	@return �e�L�X�g
	 */
	CStr GetText(void) const;

	/**
	 * �t�@�C���ǂݍ���
	 *	@param lpszFile �t�@�C����
	 */
	bool m_Read(LPCTSTR lpszFile)
	{
		_super::DeleteAllSection();
		CFileName name = lpszFile;
		if ( ! name.IsExist() )
		{
			return false;
		}
		CFileReader f;
		if ( ! f.Open(name) )
		{
			return false;
		}
		CByteVector vb = f.ReadInto();
		if ( vb.IsEmpty() )
		{
			return false;
		}
		vb.Add(0);
		CStr str = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
		return _super::SetText(str);
	}

	/**
	 * �t�@�C�������o��
	 *	@param lpszFile �t�@�C����
	 */
	void m_Write(LPCTSTR lpszFile)
	{
		CFileWriter f;
		if ( ! f.New(lpszFile) )
		{
			return;
		}
		CAscii d = _super::GetText();
		LPCSTR lpsz = d;
		f.Write(d.GetLength(), lpsz);
	}

	CStr			m_strFile;		///< �t�@�C����
};



/**@ingroup ACCESSOR
 * ini�t�@�C�����A�N�Z�X�N���X
 *
 *	@note CE�ł� �����悤�� CInifileAccessor ���g����悤�ɐ錾�B
 */
typedef CCeInifileAccessor CInifileAccessor;



};//TNB

