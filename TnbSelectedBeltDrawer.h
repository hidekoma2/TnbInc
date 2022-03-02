#pragma once
/**
 *@file
 * �I��ѕ`����̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDrawable.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB 
{



/**@ingroup DRAWABLE
 * �I��ѕ`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ���ŁADC��̉摜�����H���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSelectedBeltDrawer.h
 *
 *	@date 10/04/28 �V�K�쐬
 */
class CSelectedBeltDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param beltColor �ѐF�i�`��F�j
	 *	@param backColor �w�i�F�i���̐F�j
	 *	@param isTextInvert true �Ȃ�e�L�X�g(�w�i�ȊO)�̐F���]�����܂��B
	 */
	CSelectedBeltDrawer(COLORREF beltColor, COLORREF backColor, bool isTextInvert = true)
		: m_beltColor(beltColor), m_backColor(backColor), m_isTextInvert(isTextInvert)
	{
	}

	/**
	 * �R���X�g���N�^.
	 *	@param size �T�C�Y
	 *	@param beltColor �ѐF�i�`��F�j
	 *	@param backColor �w�i�F�i���̐F�j
	 *	@param isTextInvert true �Ȃ�e�L�X�g(�w�i�ȊO)�̐F���]�����܂��B
	 */
	CSelectedBeltDrawer(const SIZE& size, COLORREF beltColor, COLORREF backColor, bool isTextInvert = true)
		: _super(size), m_beltColor(beltColor), m_backColor(backColor), m_isTextInvert(isTextInvert)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CSelectedBeltDrawer(m_size, m_beltColor, m_backColor, m_isTextInvert);
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		CBitmapImage bi;
		COLORREF col = m_backColor ^ m_beltColor;
		RECT rect = { x, y, x + m_size.cx, y + m_size.cy };
		bi.SetFromDC(dc, rect);	// �`��͈͂���荞��
		CBitmapHandle bh = bi.CreateMaskBitmap(m_isTextInvert, m_backColor); //�����̂Ƃ���𔲂��o��
		bi.Set(m_size.cx, m_size.cy, col);
		bi.Insert(0, 0, bh, m_isTextInvert ? SRCPAINT : SRCAND); //�����̂Ƃ�����}�X�N����
		bi.BitBlt(dc, x, y, SRCINVERT);
	}

private:
	COLORREF	m_beltColor;	///< �ѐF
	COLORREF	m_backColor;	///< �w�i�F
	bool		m_isTextInvert;	///< �w�i�ȊO�̔��]�H
};



};	// TNB
