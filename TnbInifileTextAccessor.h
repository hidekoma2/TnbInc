#pragma once
/**
 *@file
 * ini�t�@�C���`���e�L�X�g���A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTemporaryAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * ini�t�@�C���`���e�L�X�g���A�N�Z�X�N���X
 *
 *		�Z�N�V�����ŋ�؂�ꂽ���ɕ����̃L�[�ƒl�����`�̏����������ŊǗ����A
 *		GetText() �Ńe�L�X�g�����A SetText() �ŏ��𕜌����邱�Ƃ��o����N���X�ł��B
 *
 *		operator[]() �� �Z�N�V�������A�N�Z�X�N���X({@link IAccessor::CSection CSection})���擾���A
 *		Query�`() �A Write�`() �ȂǂŃL�[���ɃA�N�Z�X���܂��B
 *
 *	@note	�{�C���^�[�t�F�[�X�ɂ� QueryValue() , WriteValue() �ȂǁA
 *			�Z�N�V�������w�肵�L�[���𑀍삷�郁�\�b�h������܂����A
 *			��{�I�� {@link IAccessor::CSection CSection} ���g�p���Ă��������B
 *
 *	@see	IIAccessor ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbInifileTextAccessor.h
 *
 *	@date 12/04/26 �V�K�쐬
 */
class CInifileTextAccessor : public CTemporaryAccessor
{
	DEFSUPER(CTemporaryAccessor);
public:

	/**
	 * [�ݒ�] �e�L�X�g�ݒ�.
	 *	@param lpszText �e�L�X�g
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetText(LPCTSTR lpszText)
	{
		_super::DeleteAllSection();
		CStrVector vs = CStrOperator::SeparateLine(lpszText);
		//
		CStr sec;
		loop ( i, vs.GetSize() )
		{
			const CStr& a = vs[i];
			if ( a.IsEmpty() )
			{
				continue;
			}
			if ( a[0] == _T('[') )
			{
				int ofs = a.Find(_T(']'), 1);
				if ( ofs >= 0 )
				{
					sec = a.Mid(1, ofs - 1);
				}
				continue;
			}
			int ofs = a.Find('=');
			if ( ofs >= 0 )
			{
				CStr val;
				CStr key = a.FindCut(_T('='), &val);
				IAccessor::CValue vv(val);
				if ( ! _super::WriteValue(sec, key, vv) )
				{
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * [�擾] �e�L�X�g�擾.
	 *	@return �e�L�X�g
	 */
	CStr GetText(void) const
	{
		CStr data;
		CStrVector sections = _super::EnumSectionNames();
		loop ( i, sections.GetSize() )
		{
			const CStr& sec = sections[i];
			data += CStr::Fmt(_T("[%s]\r\n"), sec);
			CStrVector keys = _super::EnumKeyNames(sec);
			loop ( i, keys.GetSize() )
			{
				const CStr& key = keys[i];
				CValue v = _super::QueryValue(sec, key);
				if ( ! v.IsNull() )
				{
					data += CStr::Fmt(_T("%s=%s\r\n"), key, v.QueryString());
				}
			}
		}
		return data;
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
	 *	@note �{�N���X�ł͂Ȃɂ����܂���B
	 *	@return ��� true
	 */
	virtual bool Flush(void)
	{
		return true;
	}
};



};//TNB

