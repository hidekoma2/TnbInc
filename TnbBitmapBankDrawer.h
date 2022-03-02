#pragma once
/**
 *@file
 * �r�b�g�}�b�v�o���N�`����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBankDrawer.h"
#include "TnbBitmapImage.h"
#include "TnbBitmapDrawer.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE BITMAP
 * �r�b�g�}�b�v�o���N�`����N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		LPARAM �ɂ��A�`������Ǘ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapBankDrawable.h
 *
 *	@date 08/10/06 �V�K�쐬
 */
class CBitmapBankDrawer : public CBankDrawer
{
	DEFSUPER(CBankDrawer);
public:

	/// �R���X�g���N�^
	CBitmapBankDrawer(void)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param bmp �r�b�g�}�b�v
	 *	@param num �������Bbmp �𕪉����ĕێ����܂��B
	 *	@param color ���߂���F�B CLR_INVALID ���w�肷��ƁA���ߐF�͂Ȃ��B
	 */
	CBitmapBankDrawer(CBitmapHandle bmp, size_t num = 1, COLORREF color = CLR_INVALID)
	{
		CBitmapImage bi = bmp;
		SIZE sz = bi.GetSize();
		size_t w = sz.cx / num;
		loop ( i, num )
		{
			RECT rc = { ToInt(i * w), 0, ToInt(i * w + w), sz.cy };
			m_phDraws.Add(new CSemitransparentBitmapDrawer(bi.Cut(rc), 100, color));
		}
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CBitmapBankDrawer* P = new CBitmapBankDrawer(*this);
		return P;
	}
};



};///TNB

