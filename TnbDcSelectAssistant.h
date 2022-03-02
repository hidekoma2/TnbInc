#pragma once
/**
 *@file
 * �f�o�C�X�e�L�X�g�Z���N�g�A�V�X�^���g�֌W�̃w�b�_
 *
 *		HDC �ɑ΂��� SelectObject �ȂǊǗ����A�V�X�g���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �f�o�C�X�e�L�X�g�Z���N�g�A�V�X�^���g
 *
 *
 *	@par�K�v�t�@�C��
 *			TnbDcSelectAssistant.h
 *
 *	@date	10/05/13 �V�K�쐬�B
 */
class CDcSelectAssistant : CCopyImpossible
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@param dc �A�V�X�g�Ώۂ̃f�o�C�X�R���e�L�X�g.
	 */
	CDcSelectAssistant(HDC dc) : m_dc(dc) 
		, m_textColor(CLR_INVALID), m_backColor(CLR_INVALID), m_backMode(-1)
		, m_font(NULL), m_brush(NULL), m_pen(NULL), m_bitmap(NULL)
	{
	}

	/**
	 * �f�X�g���N�^.
	 *	@note Restore() ���R�[������܂��B
	 */
	~CDcSelectAssistant(void)
	{
		Restore();
	}

	/**
	 * [�ݒ�] ���X�g�A.
	 *		�{�C���X�^���X�L�����ɑI��/�ݒ肳�ꂽ���e�����ɖ߂��܂��B
	 */
	void Restore(void)
	{
		if ( m_textColor != CLR_INVALID )
		{
			::SetTextColor(m_dc, m_textColor);
			m_textColor = CLR_INVALID;
		}
		if ( m_backColor != CLR_INVALID )
		{
			::SetBkColor(m_dc,m_backColor);
			m_backColor = CLR_INVALID;
		}
		if ( m_backMode >= 0 )
		{
			::SetBkMode(m_dc, m_backMode);
			m_backMode = -1;
		}
		if ( m_font != NULL )
		{
			::SelectObject(m_dc, m_font);
			m_font = NULL;
		}
		if ( m_brush != NULL )
		{
			::SelectObject(m_dc, m_brush);
			m_brush = NULL;
		}
		if ( m_pen != NULL )
		{
			::SelectObject(m_dc, m_pen);
			m_pen = NULL;
		}
		if ( m_bitmap != NULL )
		{
			::SelectObject(m_dc, m_bitmap);
			m_bitmap = NULL;
		}
	}

	/**
	 * [�ݒ�] �����F�ݒ�. 
	 *	@param c �J���[
	 */
	void SetTextColor(COLORREF c)
	{
		COLORREF r = ::SetTextColor(m_dc, c);
		if ( m_textColor == CLR_INVALID )
		{
			m_textColor = r;
		}
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�. 
	 *	@param c �J���[
	 */
	void SetBkColor(COLORREF c)
	{
		COLORREF r = ::SetBkColor(m_dc, c);
		if ( m_backColor == CLR_INVALID )
		{
			m_backColor = r;
		}
	}

	/**
	 * [�ݒ�] �w�i���[�h�ݒ�. 
	 *	@param mode ���[�h�B TRANSPARENT �� OPAQUE ���w��ł��܂��B
	 */
	void SetBkMode(int mode)
	{
		int r = ::SetBkMode(m_dc, mode);
		if ( m_backMode < 0 )
		{
			m_backMode = r;
		}
	}

	/**
	 * [�I��] �u���V�I��. 
	 *	@param brush �u���V
	 */
	void SelectBrush(HBRUSH brush)
	{
		HGDIOBJ r = ::SelectObject(m_dc, brush);
		if ( m_brush == NULL )
		{
			m_brush = static_cast<HBRUSH>(r);
		}
	}

	/**
	 * [�I��] �y���I��. 
	 *	@param pen �y��
	 */
	void SelectPen(HPEN pen)
	{
		HGDIOBJ r = ::SelectObject(m_dc, pen);
		if ( m_pen == NULL )
		{
			m_pen = static_cast<HPEN>(r);
		}
	}

	/**
	 * [�I��] �摜�I��. 
	 *	@param bitmap �摜
	 */
	void SelectBitmap(HBITMAP bitmap)
	{
		HGDIOBJ r = ::SelectObject(m_dc, bitmap);
		if ( m_bitmap == NULL )
		{
			m_bitmap = static_cast<HBITMAP>(r);
		}
	}

	/**
	 * [�I��] �t�H���g�I��. 
	 *	@param font �t�H���g
	 */
	void SelectFont(HFONT font)
	{
		HGDIOBJ r = ::SelectObject(m_dc, font);
		if ( m_font == NULL )
		{
			m_font = static_cast<HFONT>(r);
		}
	}

	#ifdef __AFX_H__

	/**
	 * [�I��] �t�H���g�I��. 
	 *	@param pFont �t�H���g
	 */
	void SelectFont(CFont* pFont)
	{
		if ( pFont != NULL )
		{
			SelectFont(static_cast<HFONT>(pFont->GetSafeHandle()));
		}
	}

	#endif

private:
	HDC			m_dc;			///< �Ǘ����� DC
	COLORREF	m_textColor;	///< �����J���[
	COLORREF	m_backColor;	///< �w�i�J���[
	int			m_backMode;		///< �w�i���[�h
	HFONT		m_font;			///< �t�H���g
	HBRUSH		m_brush;		///< �u���V
	HPEN		m_pen;			///< �y��
	HBITMAP		m_bitmap;		///< �摜
};



};//TNB
