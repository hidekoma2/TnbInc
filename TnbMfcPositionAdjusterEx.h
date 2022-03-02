#pragma once
/**
 *@file
 * �R���g���[���ʒu�̒����֌W�̃w�b�_
 *
 *		IDrawable �𗘗p�����R���g���[�������p�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcPositionAdjuster.h"
#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * �R���g���[���ʒu�����p�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂����A�`�悵�܂���B
 *		�`��ʒu���𗘗p���āA�R���g���[���̈ʒu�𐧌䂵�܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�R���g���[�����A�w��̈ʒu�Ɉړ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPositionAdjusterEx.h
 *
 *	@date 08/09/17 �V�K�쐬
 */
class CPositionAdjusterEx : public CPositionAdjuster
{
	DEFSUPER(CPositionAdjuster);
protected:

	CPointerHandleT<CWnd> m_phWnd;			///< �R���g���[��

public:

	/**
	 * �R���X�g���N�^
	 *	@param pWnd �ΏۃE�B���h�E(�R���g���[��)�B  new �ō쐬�����I�u�W�F�N�g��n���܂��B
	 *	@param canResize �ȗ��� true �Ȃ烊�T�C�Y�Ή��B false �Ȃ烊�T�C�Y�͋֎~���܂��B
	 *	@param boAlways	true �Ȃ� DC ��NULL �ȊO�ł��z�u�ύX�B�ȗ��� false �Ȃ� dc �� NULL �̏ꍇ�̂ݒ������܂��B
	 */
	CPositionAdjusterEx(CWnd* pWnd, bool canResize = true, bool boAlways = false)
		: m_phWnd(pWnd), _super(pWnd, canResize, boAlways)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CPositionAdjusterEx* P = new CPositionAdjusterEx(NULL);
		P->m_phWnd = m_phWnd;
		P->m_pWnd = m_pWnd;
		P->m_boCanResize = m_boCanResize;
		P->m_boAlways = m_boAlways;
		return P;
	}
};



};///TNB

