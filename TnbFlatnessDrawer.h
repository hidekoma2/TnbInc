#pragma once
/**
 *@file
 * ���R�`����̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDrawable.h"



//TNB Library
namespace TNB 
{



/**@ingroup DRAWABLE
 * ���R�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ���ŁADC��̉摜�����H���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFlatnessDrawer.h
 *
 *	@date 08/07/01 �V�K�쐬
 */
class CFlatnessDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color1;	///< �`��F�P�B
	COLORREF	m_color2;	///< �`��F�Q�B�e�̂悤�ɕ`��F�P����E���ɂ���ĕ`�悳���F�B CLR_INVALID ���w�肷��ƕ`�悳��Ȃ��B
	COLORREF	m_back;		///< base �̐F�̕����̕`��F�B�ȗ������ base �Ɠ����F������Ƃ���B
	COLORREF	m_base;		///< ���̊G�̔w�i�ɓ�����F�B�ȗ�����ƁA�����̐F������Ƃ���B
	COLORREF	m_work;		///< ���[�N�p�̐F�B���ʏȗ����܂��B
public:

	/**
	 * �R���X�g���N�^.
	 *	@param color1 �`��F�P
	 *	@param color2 �`��F�Q�B�e�̂悤�ɕ`��F�P����E���ɂ���ĕ`�悳���F�B CLR_INVALID ���w�肷��ƕ`�悳��Ȃ��B
	 *	@param back base �̐F�̕����̕`��F�B�ȗ������ base �Ɠ����F������Ƃ���B
	 *	@param base ���̊G�̔w�i�ɓ�����F�B�ȗ�����ƁA�����̐F������Ƃ���B
	 *	@param work ���[�N�p�̐F�B���ʏȗ����܂��B
	 */
	CFlatnessDrawer(COLORREF color1, COLORREF color2, COLORREF back = CLR_AUTOSELECT, COLORREF base = CLR_AUTOSELECT, COLORREF work = RGB(255, 255, 255))
		: m_color1(color1), m_color2(color2), m_back(back), m_base(base), m_work(work)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CFlatnessDrawer(m_color1, m_color2, m_back, m_base, m_work);
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
		COLORREF base = m_base;
		COLORREF back = m_back;
		if ( base == CLR_AUTOSELECT )
		{
			base = ::GetPixel(dc, x, y + m_size.cy - 1);
		}
		if ( ! IS_RGBVALUE(back) )
		{
			back = base;
		}
		const DWORD	CP_ROP = 0xE20746;
		HBITMAP hBmp = ::CreateBitmap(m_size.cx, m_size.cy, 1, 1, NULL);
		HDC maskDC = ::CreateCompatibleDC(dc);
		HGDIOBJ oldBmp = ::SelectObject(maskDC, hBmp);
		{
			COLORREF oldColor = ::SetBkColor(dc, base);
			::BitBlt(maskDC, 0, 0, m_size.cx, m_size.cy, dc, x, y, SRCCOPY);
			::SetBkColor(dc, m_work);
			::BitBlt(maskDC, 0, 0, m_size.cx, m_size.cy, dc, x, y, NOTSRCERASE);
			//
			RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
			HBRUSH hr = ::CreateSolidBrush(back);
			::FillRect(dc, &rc, hr);
			_DeleteObject(hr);
			//
			HBRUSH brush1 = ::CreateSolidBrush(m_color1);
			HGDIOBJ oldBrush = ::SelectObject(dc, brush1);
			::BitBlt(dc, x, y, m_size.cx, m_size.cy, maskDC, 0, 0, CP_ROP);
			::SelectObject(dc, oldBrush);
			_DeleteObject(brush1);
			if ( m_color2 != CLR_INVALID )
			{
				HBRUSH brush2 = ::CreateSolidBrush(m_color2);
				HGDIOBJ oldBrush = ::SelectObject(dc, brush2);
				::BitBlt(dc, x - 1, y - 1, m_size.cx, m_size.cy, maskDC, 0, 0, CP_ROP);
				::SelectObject(dc, oldBrush);
				_DeleteObject(brush2);
			}
			::SetBkColor(dc, oldColor);
		}
		::SelectObject(maskDC, oldBmp);
		_DeleteObject(maskDC);
		_DeleteObject(hBmp);
	}

	/**
	 * [�擾] �A�N�e�B�u�I����Ԑݒ�C���X�^���X�쐬.
	 *	@note �摜���A�N�e�B�u�I����Ԃɕϊ�����`������쐬���܂��B
	 *	@param base ���̊G�̔w�i�ɓ�����F�B�ȗ�����ƁA�����̐F������Ƃ���B
	 *	@param work ���[�N�p�̐F�B���ʏȗ����܂��B
	 *	@return CFlatnessDrawer �̃|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	static CFlatnessDrawer* CreateSelectedType(COLORREF base = CLR_AUTOSELECT, COLORREF work = RGB(255, 255, 255))
	{
		return new CFlatnessDrawer(::GetSysColor(COLOR_HIGHLIGHTTEXT), CLR_INVALID, ::GetSysColor(COLOR_HIGHLIGHT), base, work);
	}

	/**
	 * [�擾] ��A�N�e�B�u�I����Ԑݒ�C���X�^���X�쐬.
	 *	@note �摜���A�N�e�B�u�I����Ԃɕϊ�����`������쐬���܂��B
	 *	@param base ���̊G�̔w�i�ɓ�����F�B�ȗ�����ƁA�����̐F������Ƃ���B
	 *	@param work ���[�N�p�̐F�B���ʏȗ����܂��B
	 *	@return CFlatnessDrawer �̃|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	static CFlatnessDrawer* CreateNaSelectedType(COLORREF base = CLR_AUTOSELECT, COLORREF work = RGB(255, 255, 255))
	{
		return new CFlatnessDrawer(::GetSysColor(COLOR_WINDOWTEXT), CLR_INVALID, ::GetSysColor(COLOR_BTNFACE), base, work);
	}
};



};	// TNB
