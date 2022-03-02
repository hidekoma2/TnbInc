#pragma once
/**
 *@file
 * �_�~�[���A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * �_�~�[���A�N�Z�X�N���X
 *
 *		������؂����Ȃ��A�N�Z�X�Ǘ��ł��B
 *		Query���͎w��̃f�t�H���g�̒l��Ԃ��AWrite�͉������܂��񂪐�����Ԃ��܂��B
 *
 *		IAccessor ���K�v�ŁA��������K�v���Ȃ��ꍇ�A�f�t�H���g���g�������ꍇ�ȂǂɎg�p�ł��܂��B
 *
 *	@see	IAccessor ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDummyAccessor.h
 *
 *	@date 08/08/25 �V�K�쐬
 */
class CDummyAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
public:
	/**
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Dummy");
	}

	/**
	 * [����] �t���b�V��.
	 *	@note Write���������m�肳���܂��iSub�N���X�ɂ�蓮�삪���ƂȂ�܂��B�j�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool Flush(void)
	{
		return true;
	}

	/**
	 * [�擾] �Z�N�V�������ꗗ�擾
	 *	@param lpszSectionName �Z�N�V�������B NULL���w�肷��ƁAROOT�̃Z�N�V�����ꗗ�������܂��B
	 *	@return �Z�N�V�������ꗗ
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const
	{
		return CStrVector();
	}

	/**
	 * [�폜] �w��Z�N�V�����폜
	 *	@param lpszSectionName �Z�N�V�������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		return true;
	}

	/**
	 * [�擾] �L�[���ꗗ�擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return ���O�ꗗ�B������ԂȂ疢�Ή��B
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		return CStrVector();
	}

	/**
	 * [�擾] �����擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return ����B�L�[�����݂��Ȃ��ꍇ�AEK_Nothing �ɂȂ�܂��B
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		return EK_Nothing;
	}

	/**
	 * [�擾] ���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		return lpszKey;
	}

	/**
	 * [�ݒ�] ���ݒ�
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param value �ݒ肷��l�BEK_Nothing �̏ꍇ�A�L�[���폜���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		return true;
	}
};



}; //TNB
