#pragma once
/**
 *@file
 * WND�e�L�X�g�`��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTextDrawer.h"
#include "TnbPartsDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * WND�e�L�X�g�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�E�B���h�E�n���h���̏��̕`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbWndTextDrawer.h
 *
 *	@date 07/12/01 �V�K�쐬
 *	@date 07/12/27 �t�H�[�J�X�g�́A�N���C�A���g�̈�ɏ������悤�ɑΏ��B
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 *	@date 09/11/05 SetBkMode() �ǉ��B
 */
class CWndTextDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * �R���X�g���N�^
	 *	@param size		�T�C�Y
	 *	@param hWnd		�E�B���h�E�n���h���i������ƃt�H���g�ƃt�H�[�J�X�𓾂܂��j
	 *	@param color	�J���[
	 *	@param style	DrawText�X�^�C��
	 *	@param cx		�I�t�Z�b�g
	 *	@param cy		�I�t�Z�b�g
	 *	@param pParts	�t�H�[�J�X�}�[�N�p( DrawFocusRect() �݂̂��g�p���Ă܂�)�B�t�H�[�J�X�}�[�N������Ȃ��ꍇNULL�B
	 */
	CWndTextDrawer(const SIZE& size, HWND hWnd, COLORREF color, DWORD style, int cx = 2, int cy = 2, const IPartsDrawable* pParts = NULL)
		: _super(size), m_style(style), m_color(color), m_hWnd(hWnd), m_cx(cx), m_cy(cy), m_pParts(pParts), m_bkMode(TRANSPARENT)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 *	@param hWnd		�E�B���h�E�n���h���i������ƃt�H���g�ƃt�H�[�J�X�𓾂܂��j
	 *	@param color	�J���[
	 *	@param style	DrawText�X�^�C��
	 *	@param cx		�I�t�Z�b�g
	 *	@param cy		�I�t�Z�b�g
	 *	@param pParts	�t�H�[�J�X�}�[�N�p( DrawFocusRect() �݂̂��g�p���Ă܂�)�B�t�H�[�J�X�}�[�N������Ȃ��ꍇNULL�B
	 */
	CWndTextDrawer(HWND hWnd, COLORREF color, DWORD style, int cx = 2, int cy = 2, const IPartsDrawable* pParts = NULL)
		: _super(), m_style(style), m_color(color), m_hWnd(hWnd), m_cx(cx), m_cy(cy), m_pParts(pParts), m_bkMode(TRANSPARENT)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CWndTextDrawer* P = new CWndTextDrawer(NULL, RGB(0, 0, 0), 0);
		*P = *this;
		return P;
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
		if ( ::IsWindow(m_hWnd) )
		{
			int iLen = ::GetWindowTextLength(m_hWnd);
			if ( iLen > 1 )
			{
				CWorkMemT<TCHAR> str(iLen + 2);
				::GetWindowText(m_hWnd, str, iLen + 1);
				RECT rc = { x + m_cx, y + m_cy, x + m_size.cx - m_cx, y + m_size.cy - m_cy };
				CDcSelectAssistant dca(dc);
//				int oldBkMode = ::SetBkMode(dc, m_bkMode);
				dca.SetBkMode(m_bkMode);
				HFONT font = reinterpret_cast<HFONT>(::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
//				HGDIOBJ oldObj = ::SelectObject(dc, font);
				dca.SelectFont(font);
				CTextDrawer::DrawTextRect(rc, dc, m_style, m_color, str);
				if ( m_pParts != NULL && ::GetFocus() == m_hWnd )
				{
					RECT wr;
					::GetClientRect(m_hWnd, &wr);
					rc.left--;
					rc.right++;
					rc.top--;
					rc.bottom += 2;
					::IntersectRect(&rc, &rc, &wr);
					m_pParts->DrawFocusRect(dc, &rc);
				}
//				::SelectObject(dc, oldObj);
//				::SetBkMode(dc, oldBkMode);
			}
		}
	}

	/**
	 * [�ݒ�] �F�ݒ�.
	 *	@param color �J���[
	 */
	void SetTextColor(COLORREF color)
	{
		m_color = color;
	}

	/**
	 * [�ݒ�] �X�^�C���ݒ�.
	 *	@param style	DrawText�X�^�C��
	 */
	void SetTextStyle(DWORD style)
	{
		m_style = style;
	}

	/**
	 * [�ݒ�] �w�i���[�h�ݒ�
	 *	@param mode TRANSPARENT �� OPAQUE ���w�肵�܂��B
	 */
	void SetBkMode(int mode)
	{
		m_bkMode = mode;
	}

private:
	HWND					m_hWnd;		///< �E�B���h�E�n���h���i������ƃt�H���g�ƃt�H�[�J�X�𓾂܂��j
	COLORREF				m_color;	///< �J���[
	DWORD					m_style;	///< Draw�X�^�C��
	int						m_cx;		///< �I�t�Z�b�g
	int						m_cy;		///< �I�t�Z�b�g
	const IPartsDrawable*	m_pParts;	///< �t�H�[�J�X�}�[�N�p�B�t�H�[�J�X�}�[�N������Ȃ��ꍇNULL�B
	int						m_bkMode;	///< BK���[�h
};



}; //TNB


