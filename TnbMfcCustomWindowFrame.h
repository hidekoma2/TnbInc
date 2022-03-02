#pragma once
/**
 *@file
 * �J�X�^���E�B���h�E�t���[���̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbWindowProcedureHooker.h"
#include "TnbBitmapBankDrawer.h"
#include "TnbBitmapBorderDrawer.h"
#include "TnbTextDrawer.h"
#include "TnbMfcDrawingMenu.h"
#include "TnbHandleHandle.h"
#include "TnbFontHandle.h"
#include "TnbDcSelectAssistant.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #ifndef WM_NCMOUSELEAVE
  #define WM_NCMOUSELEAVE 0x02A2
 #endif
 #ifndef TME_NONCLIENT
  #define TME_NONCLIENT 0x10
 #endif
#endif



/**@ingroup DRAWABLE
 * �E�B���h�E�^�C�g���`����N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *	@see CCustomWindowFrame ���Ŏg�p���Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcCustomWindowFrame.h
 *
 *	@date 08/10/06 �V�K�쐬
 */
class CWindowCaptionDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * �R���X�g���N�^
	 *	@param hWnd �ΏۃE�B���h�E
	 */
	CWindowCaptionDrawer(HWND hWnd) : m_hWnd(hWnd), m_nWidthEllipse(5), m_nHeightEllipse(5)
		, m_barColor(::GetSysColor(COLOR_ACTIVECAPTION)), m_naBarColor(::GetSysColor(COLOR_INACTIVECAPTION))
		, m_textColor(::GetSysColor(COLOR_CAPTIONTEXT)), m_naTextColor(::GetSysColor(COLOR_INACTIVECAPTIONTEXT))
	{
	}

	/**
	 * [�ݒ�] �o�[�̐F�w��.
	 *	@note �w�肵�Ȃ��ꍇ�A�V�X�e���̐F���g�p����܂��B
	 *	@param c1 �A�N�e�B�u���̐F
	 *	@param c2 ��A�N�e�B�u���̐F
	 */
	void SetBarColor(COLORREF c1, COLORREF c2)
	{
		m_barColor = c1;
		m_naBarColor = c2;
	}

	/**
	 * [�ݒ�] �����̐F�w��.
	 *	@note �w�肵�Ȃ��ꍇ�A�V�X�e���̐F���g�p����܂��B
	 *	@param c1 �A�N�e�B�u���̐F
	 *	@param c2 ��A�N�e�B�u���̐F
	 */
	void SetTextColor(COLORREF c1, COLORREF c2)
	{
		m_textColor = c1;
		m_naTextColor = c2;
	}

	/**
	 * [�ݒ�] �o�[�̊ۂݎw��
	 *	@param widthEllipse �ȉ~�̍����B�f�t�H���g�� 5 �ł��B
	 *	@param heightEllipse �ȉ~�̍����B�f�t�H���g�� 5 �ł��B
	 */
	void SetBarEllipse(int widthEllipse = 5, int heightEllipse = 5)
	{
		m_nWidthEllipse = widthEllipse;
		m_nHeightEllipse = heightEllipse;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const 
	{
		return new CWindowCaptionDrawer(*this); 
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@note �{�N���X�͉������܂���B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 *	@param lParam LPARAM�B 0�Ȃ�A�N�e�B�u�A 1�Ȃ��A�N�e�B�u��`�悵�܂��B
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		HWND hWnd = ::WindowFromDC(dc);
		if ( hWnd != NULL )
		{
			m_hWnd = hWnd;
		}
		// �o�[
		{
			COLORREF c1 = (lParam == 0) ? m_barColor : m_naBarColor;
			CBitmapImage bi;
			bi.Set(m_size.cx, m_size.cy, c1);
			CBitmapImage mb;
			mb.Set(m_size.cx, m_size.cy);
			mb.GradationFill(RGB(255, 255, 255), 0, true);
			bi.CustomizeAlpha(mb);
			if ( m_nWidthEllipse > 0 || m_nHeightEllipse > 0 )
			{
				HRGN rgn = ::CreateRoundRectRgn(x, y, x + m_size.cx, y + m_size.cy, m_nWidthEllipse, m_nHeightEllipse);
				::SelectClipRgn(dc, rgn);
				bi.SemitransparectBlt(dc, x, y, -1, CLR_INVALID);
				::SelectClipRgn(dc, NULL);
				_DeleteObject(rgn);
			}
			else
			{
				bi.SemitransparectBlt(dc, x, y, -1, CLR_INVALID);
			}
		}
		// �A�C�R��
		HICON icon = reinterpret_cast<HICON>(::SendMessage(m_hWnd, WM_GETICON, 0, 0));
		if ( icon != NULL )
		{
//			::DrawIconEx(dc, x + 2, (m_size.cy - 16) / 2 + y, icon, CalcScaleX(16, dc), CalcScaleY(16, dc), 0, 0, DI_NORMAL);
			::DrawIconEx(dc, x + 2, y + 1, icon, CalcScaleX(16, dc), CalcScaleY(16, dc), 0, 0, DI_NORMAL);
			x += CalcScaleX(16 + 5, dc);
		}
		else
		{
			x += 2;
		}
		// ����
		CWorkMemT<TCHAR> w(256);
		::GetWindowText(m_hWnd, w.Ref(), 256);
		if ( m_hFont.IsNull() )
		{
			VERIFY( m_hFont.SetSystemCaptionFont() );
		}
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		COLORREF color = (lParam == 0) ? m_textColor : m_naTextColor;
		//
		CDcSelectAssistant dca(dc);
		dca.SetBkMode(TRANSPARENT);
		dca.SelectFont(m_hFont);
		CTextDrawer::DrawTextRect(dc, rc, DT_LEFT | DT_VCENTER, color, w);
	}

private:
	mutable HWND		m_hWnd;				///< �ΏۃE�B���h�E
	mutable CFontHandle	m_hFont;			///< �t�H���g
	COLORREF			m_barColor;			///< �o�[�F
	COLORREF			m_naBarColor;		///< �o�[�F
	COLORREF			m_textColor;		///< �����F
	COLORREF			m_naTextColor;		///< �����F
	int					m_nWidthEllipse;	///< �ȉ~�̍���
	int					m_nHeightEllipse;	///< �ȉ~�̕�
};



/**@ingroup DRAWABLE MFCCONTROL
 * �J�X�^���E�B���h�E�t���[��.
 *	
 *		�_�C�A���O�� NonClient ������ύX�ł��܂��B�܂��A���ߐF���w�肵�A��`�ȊO�̃t���[���ɂ��Ή����܂��B
 *
 *	@note �E�B���h�E�̃t���[���A�{�^���̌`��A�ʒu�����R�ɃJ�X�^�}�C�Y�ł��܂��B
 *			�L���v�V�����o�[�͏㕔�Œ�ɂȂ�܂��B	
 *
 *	@par�g�p��
 *		\code
 *		class CFooDlg : public CDialog
 *		{
 *				;
 *		    CCustomWindowFrame m_frame;
 *				;
 *		};
 *
 *		BOOL CFooDlg::OnInitDialog()
 *		{
 *			CDialog::OnInitDialog();
 *				;
 *			// �t���[���̐ݒ�
 *			CWindowCaptionDrawer ttd(NULL);
 *			ttd.SetBarColor(RGB(255, 0, 0), RGB(0, 0, 0));
 *			m_frame.SetTitleBitmap(ttd);
 *			m_frame.SetFrameBitmap(IDB_FRAME);
 *			m_frame.SetCloseButtonBitmap(CPoint(-6, 5), IDB_CLOSE, 4);
 *				;
 *			// �A�^�b�`����B
 *			m_frame.Attach(*this);
 *				;
 *		\endcode
 *
 *	@todo ReadOnly �� EDIT �ŃX�N���[�����A�������d�Ȃ��Ă��܂��E�E�E�i�w�i���ĕ`�悳��Ȃ��j�B
 *
 *	@todo �V�X�e�����j���[�ω����E���Ȃ��B WM_MENUSELECT ���K�v�B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcCustomWindowFrame.h
 *
 *	@date 08/10/06	�V�K�쐬
 *	@date 09/09/29	���j���[����Ȃ�A�^�C�g���o�[�̍����͌Œ�ɁB
 *	@date 10/04/19	SetTextColor �ǉ��B
 *	@date 10/04/26	�|�b�v�A�b�v���j���[���ΏۂɁB
 *	@date 10/06/15	�T�C�Y�ύX���ɂ� RGN ��ݒ肷��悤�ɂ����B
 *	@date 10/07/20	�z�o�[��Ԃ̏������C��
 *	@date 11/09/01	�����I�� NC �`�掞�AWM_NCPAINT ���Ă������A���ڊ֐��R�[���ɕύX�B
 *	@date 11/10/12	WM_INITDIALOG �̎��ɂ��V�X�e�����j���[�̃`�F�b�N���s���悤�ɂ����B
 */
class CCustomWindowFrame : public COwnerdrawMenuSupportProcedureHooker
{
	DEFSUPER(COwnerdrawMenuSupportProcedureHooker);
public:

	/// �R���X�g���N�^
	CCustomWindowFrame(void) 
		: m_pressedButton(BK_Nothing), m_transColor(CLR_INVALID), m_hNcRgn(NULL)
		, m_isActive(true), m_boHasHover(false), m_isInit(false), m_isInitWindowRgn(true), m_canMenuCheck(true)
		, m_frameWidths(0, 0, 0, 0), m_titleRect(0, 0, 0, 0), m_titleHeight(0), m_isEvenBackGround(false)
		, m_textColor(CLR_INVALID), m_isMenuInit(false), m_isNcResized(false)
	{
	}

	/// �f�X�g���N�^
	~CCustomWindowFrame(void)
	{
		if ( m_hNcRgn != NULL )
		{
			_DeleteObject(m_hNcRgn);
			m_hNcRgn = NULL;
		}
	}

	/**
	 * [�ݒ�] �t���[���r�b�g�}�b�v�ݒ�
	 *	@param bmp �摜�B���̉摜�� 9��������A���ꂼ��̋��A�g�A�����Ɏg���܂��B
	 */
	void SetFrameBitmap(CBitmapHandle bmp)
	{
		CBitmapBorderDrawer bbd(bmp);
		m_backGround[0] = ::CreatePatternBrush(bbd.GetParts(5)); //����
		m_backGround[1] = ::CreatePatternBrush(bbd.GetParts(5)); //����
		m_isEvenBackGround = true;
		m_pFrameDrawer = bbd.Clone();
	}

	/**
	 * [�ݒ�] �t���[���r�b�g�}�b�v�ݒ�
	 *	@param bmp1 �A�N�e�B�u���̉摜�B���̉摜�� 9��������A���ꂼ��̋��A�g�A�����Ɏg���܂��B
	 *	@param bmp2 ��A�N�e�B�u���̉摜�B���̉摜�� 9��������A���ꂼ��̋��A�g�A�����Ɏg���܂��B
	 */
	void SetFrameBitmap(CBitmapHandle bmp1, CBitmapHandle bmp2)
	{
		CBitmapBorderDrawer bbd1(bmp1);
		CBitmapBorderDrawer bbd2(bmp2);
		m_backGround[0] = ::CreatePatternBrush(bbd1.GetParts(5)); //����
		m_backGround[1] = ::CreatePatternBrush(bbd2.GetParts(5)); //����
		m_isEvenBackGround = false;
		CBankDrawer db;
		m_pFrameDrawer = db.Add(bbd1).Add(bbd2).Clone();
	}

	/**
	 * [�ݒ�] �t���[���r�b�g�}�b�v�ݒ�
	 *	@param draw �t���[���`����B
	 *	@param bmp �����̃u���V�摜���w�肵�܂��B
	 */
	void SetFrameBitmap(const IDrawable& draw, CBitmapHandle bmp)
	{
		m_backGround[0] = ::CreatePatternBrush(bmp);
		m_backGround[1] = ::CreatePatternBrush(bmp);
		m_isEvenBackGround = true;
		m_pFrameDrawer = draw.Clone();
	}

	/**
	 * [�ݒ�] ���ߐF�w��
	 *	@note �t���[���r�b�g�}�b�v�̓��ߐF���w��ł��܂��B����ɂ��C�ӂ̌`�̃E�B���h�E���쐬�ł��܂��B
	 *		�{���\�b�h���g��Ȃ��ꍇ�A�f�t�H���g�͋�`�̃E�B���h�E�ɂȂ�܂��B
	 *	@param color ���ߐF�B CLR_AUTOSELECT ���g���ƉE���̐F�𓧉ߐF�Ƃ��܂��B
	 */
	void SetFrameTransColor(COLORREF color = CLR_AUTOSELECT)
	{
		m_transColor = color;
	}

	/**
	 * [�ݒ�] �^�C�g���o�[�`����w��.
	 *	@note �t���[���r�b�g�}�b�v�Ń^�C�g�����������Ƃ��o���܂����A�ʓr�q�̃��\�b�h�Ŏw�肷�邱�Ƃ��o���܂��B
	 *			�w�肵�Ȃ��ꍇ�A�t���[���`���� CWindowCaptionDrawer() �ŃA�C�R���ƃE�B���h�E�e�L�X�g���`�悳��܂��B
	 *	@param draw �`����
	 */
	void SetTitleBitmap(const IDrawable& draw)
	{
		m_pTitleDrawer = draw.Clone();
	}

	/**
	 * [�ݒ�] �傫���ݒ�.
	 *		NonClient�̏㉺���E�̕��A����сA�^�C�g���o�[�̍������w�肵�܂��B
	 *	@attention ���j���[������E�B���h�E�̏ꍇ�A�{���\�b�h�͐������@�\���܂���B���j���[�����̏ꍇ�̂ݎg�p���Ă��������B
	 *	@param frameWidths NoClient�̏㉺���E�̕�(RECT�{���̎g�����ł͂Ȃ�)
	 *	@param titleHeight �^�C�g���o�[�̍����B�ȗ�����ƃV�X�e���̍������g���܂��B
	 */
	void SetFrameWidths(const RECT& frameWidths, int titleHeight = -1)
	{
		if ( titleHeight <= 0 )
		{
			titleHeight = ::GetSystemMetrics(SM_CYCAPTION);
		}
		::SetRect(&m_titleRect, frameWidths.left, frameWidths.top, 0, frameWidths.top + titleHeight);
		m_titleHeight = titleHeight;
		m_frameWidths = frameWidths;
		m_frameWidths.top += titleHeight;
	}

	/**
	 * [�ݒ�] �傫���ݒ�.
	 *		NonClient�̏㉺���E�̕��A����сA�^�C�g���o�[�̍������w�肵�܂��B
	 *	@attention ���j���[������E�B���h�E�̏ꍇ�A�{���\�b�h�͐������@�\���܂���B���j���[�����̏ꍇ�̂ݎg�p���Ă��������B
	 *	@param frameWidth  NoClient�̏㉺���E�̂��ׂĂ̕��B�ȗ�����ƃV�X�e���̕����g���܂��B
	 *	@param titleHeight �^�C�g���o�[�̍����B�ȗ�����ƃV�X�e���̍������g���܂��B
	 */
	void SetFrameWidths(int frameWidth = -1, int titleHeight = -1)
	{
		RECT rc;
		if ( frameWidth >= 0 )
		{
			::SetRect(&rc, frameWidth, frameWidth, frameWidth, frameWidth);
		}
		else
		{
			::SetRectEmpty(&rc);
		}
		SetFrameWidths(rc, titleHeight);
	}

	/**
	 * [�ݒ�] Close�{�^���ݒ�
	 *	@param pos �{�^���ʒu�B���W���}�C�i�X�̏ꍇ�A�E�[(���[)����̃I�t�Z�b�g�ɂȂ�܂��B
	 *	@param bmp �{�^���摜�B�����傫���̉摜���ANormal,Press,Hover,Disable �̏��ɉ��ɕ��ׂ��摜���w�肵�܂��B
	 *	@param num �{�^���摜�̐��B�ʏ�ȗ����܂��i�S�ɂȂ�܂��j�B
	 *	@param color ���߂���F�B CLR_INVALID ���w�肷��ƁA���ߐF�͂Ȃ��B
	 */
	void SetCloseButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Close].SetDrawer(pos, bbd, HTCLOSE, SC_CLOSE);
	}

	/**
	 * [�ݒ�] Minimum�{�^���ݒ�
	 *	@param pos �{�^���ʒu�B���W���}�C�i�X�̏ꍇ�A�E�[(���[)����̃I�t�Z�b�g�ɂȂ�܂��B
	 *	@param bmp �{�^���摜�B�����傫���̉摜���ANormal,Press,Hover,Disable �̏��ɉ��ɕ��ׂ��摜���w�肵�܂��B
	 *	@param num �{�^���摜�̐��B�ʏ�ȗ����܂��i�S�ɂȂ�܂��j�B
	 *	@param color ���߂���F�B CLR_INVALID ���w�肷��ƁA���ߐF�͂Ȃ��B
	 */
	void SetMinimumButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Minimum].SetDrawer(pos, bbd, HTMINBUTTON, SC_MINIMIZE);
	}

	/**
	 * [�ݒ�] Maximum�{�^���ݒ�
	 *	@param pos �{�^���ʒu�B���W���}�C�i�X�̏ꍇ�A�E�[(���[)����̃I�t�Z�b�g�ɂȂ�܂��B
	 *	@param bmp �{�^���摜�B�����傫���̉摜���ANormal,Press,Hover,Disable �̏��ɉ��ɕ��ׂ��摜���w�肵�܂��B
	 *	@param num �{�^���摜�̐��B�ʏ�ȗ����܂��i�S�ɂȂ�܂��j�B
	 *	@param color ���߂���F�B CLR_INVALID ���w�肷��ƁA���ߐF�͂Ȃ��B
	 */
	void SetMaximumButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Maximum].SetDrawer(pos, bbd, HTMAXBUTTON, SC_MAXIMIZE);
	}

	/**
	 * [�ݒ�] Restore�{�^���ݒ�
	 *	@param pos �{�^���ʒu�B���W���}�C�i�X�̏ꍇ�A�E�[(���[)����̃I�t�Z�b�g�ɂȂ�܂��B�ʏ�AMaximum�{�^���Ɠ����ꏊ���w�肵�܂��B
	 *	@param bmp �{�^���摜�B�����傫���̉摜���ANormal,Press,Hover,Disable �̏��ɉ��ɕ��ׂ��摜���w�肵�܂��B
	 *	@param num �{�^���摜�̐��B�ʏ�ȗ����܂��i�S�ɂȂ�܂��j�B
	 *	@param color ���߂���F�B CLR_INVALID ���w�肷��ƁA���ߐF�͂Ȃ��B
	 */
	void SetRestoreButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Restore].SetDrawer(pos, bbd, HTMAXBUTTON, SC_RESTORE);
	}

	/**
	 * [�ݒ�] Help�{�^���ݒ�
	 *	@param pos �{�^���ʒu�B���W���}�C�i�X�̏ꍇ�A�E�[(���[)����̃I�t�Z�b�g�ɂȂ�܂��B
	 *	@param bmp �{�^���摜�B�����傫���̉摜���ANormal,Press,Hover,Disable �̏��ɉ��ɕ��ׂ��摜���w�肵�܂��B
	 *	@param num �{�^���摜�̐��B�ʏ�ȗ����܂��i�S�ɂȂ�܂��j�B
	 *	@param color ���߂���F�B CLR_INVALID ���w�肷��ƁA���ߐF�͂Ȃ��B
	 */
	void SetHelpButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Help].SetDrawer(pos, bbd, HTHELP, SC_CONTEXTHELP);
	}

	/**
	 * [�ݒ�] �e�L�X�g�F.
	 *	@param color �e�L�X�g�F
	 */
	void SetTextColor(COLORREF color)
	{
		if ( color == RGB(255, 255, 255) )
		{
			color = RGB(255, 255, 254);
		}
		m_textColor = color;
		m_menuBar.SetTextColor(color);
	}

	/**
	 * [�擾] �w�i�u���V�擾
	 *	@param isActive true �Ȃ�A�N�e�B�u�A false �Ȃ��A�N�e�B�u�̃u���V��Ԃ��܂��B
	 *	@return �w�i�u���V
	 */
	HBRUSH GetBackground(bool isActive)
	{
		return m_backGround[isActive ? 0 : 1];
	}

	/**
	 * [�ݒ�] �ݒ�̕���.
	 *	@note �{�^����摜�̐ݒ���R�s�[���܂��B
	 *	@param o �R�s�[���B��������̂Ō��͔j�����Ă����܂��܂���B
	 */
	void CopyParameter(const CCustomWindowFrame& o)
	{
		m_pFrameDrawer = o.m_pFrameDrawer;
		m_backGround[0] = o.m_backGround[0];
		m_backGround[1] = o.m_backGround[1];
		m_isEvenBackGround = o.m_isEvenBackGround;
		m_pTitleDrawer.Null();
		if ( ! o.m_pTitleDrawer.IsNull() )
		{
			m_pTitleDrawer = o.m_pTitleDrawer->Clone();
		}
		m_textColor = o.m_textColor;
		m_frameWidths = o.m_frameWidths;
		m_titleRect = o.m_titleRect;
		m_titleHeight = o.m_titleHeight;
		m_transColor = o.m_transColor;
		loop ( i, BK_Max )
		{
			m_button[i] = o.m_button[i];
		}
		m_menuBar.CopyParameter(o.m_menuBar);
	}

	/**
	 * [�Q��] ���j���[�o�[�Q��.
	 *		�ێ����Ă��郁�j���[�o�[�̎Q�Ƃ�Ԃ��܂��B
	 *	@return �Q��.
	 */
	CDrawingMenuBar& ReferMenuBar(void)
	{
		return m_menuBar;
	}

	/**
	 * [�Q��] ���j���[�o�[�Q��.
	 *		�ێ����Ă��郁�j���[�o�[�̎Q�Ƃ�Ԃ��܂��B
	 *	@return �Q��.
	 */
	CDrawingMenu& ReferPopupMenu(void)
	{
		return m_popupMenu;
	}

protected:

	/**
	 * [�ʒm] �A�^�b�`���O�ʒm
	 *	@param hWnd �A�^�b�`����HWND�B
	 */
	virtual void PreAttach(HWND hWnd)
	{
		int fx = ::GetSystemMetrics(SM_CXFIXEDFRAME);
		int fy = ::GetSystemMetrics(SM_CYFIXEDFRAME);
		int ch = ::GetSystemMetrics(SM_CYCAPTION);
		DWORD style = ::GetWindowLong(hWnd, GWL_STYLE);
		if ( (style & WS_THICKFRAME) != 0 )
		{
			fx = ::GetSystemMetrics(SM_CXSIZEFRAME);
			fy = ::GetSystemMetrics(SM_CYSIZEFRAME);
		}
		if ( (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != 0 )
		{
			ch = ::GetSystemMetrics(SM_CYSMCAPTION);
		}
		if ( (style & WS_CAPTION) != WS_CAPTION )
		{
			ch = 0;
		}
		if ( (style & WS_BORDER) == 0 )
		{
			fx = fy = 0;
		}
		if ( m_frameWidths.IsRectNull() )
		{
			m_frameWidths.SetRect(fx, fy + ch, fx, fy);
		}
		if ( m_titleRect.IsRectNull() )
		{
			m_titleRect.SetRect(fx, fy, 0, fy + ch);
			m_titleHeight = ch;
		}
	}
	
	/**
	 * [�ʒm] �A�^�b�`�ʒm.
	 *		�A�^�b�`���ꂽ�Ƃ��ɒʒm����܂��B
	 */
	virtual void OnAttached(void) 
	{
		m_CheckMenu();
		//
		if ( ! m_pTitleDrawer.IsNull() )
		{
			HDC dc = ::GetWindowDC(m_hWnd);
			m_pTitleDrawer->DrawEx(dc, 0, 0, 0);
			::ReleaseDC(m_hWnd, dc);
		}
		CRect rc;
		::GetClientRect(m_hWnd, rc);
		m_windowSize = rc.Size() + CSize(m_frameWidths.left + m_frameWidths.right, m_frameWidths.top + m_frameWidths.bottom);
		m_menuHeight = 0;
		MENUBARINFO mbi;
		mbi.cbSize = sizeof(MENUBARINFO);
		if ( ::GetMenuBarInfo(m_hWnd, OBJID_MENU, 0, &mbi) )
		{
			CRect r = mbi.rcBar;
			m_menuHeight = r.Height() + 1;
			m_windowSize.cy += m_menuHeight;
		}	
		::SetWindowPos(m_hWnd, NULL, 0, 0, m_windowSize.cx, m_windowSize.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		//
		m_isInit = true;
	}
	
	/**
	 * [�ʒm] �f�^�b�`���O�ʒm.
	 *		�f�^�b�`���钼�O�ɒʒm����܂��B
	 */
	virtual void PreDetach(void)
	{
		m_menuBar.Detach();
	}

	/**
	 * [�ʒm] �f�^�b�`�ʒm.
	 *		�f�^�b�`���ꂽ�Ƃ��ɒʒm����܂��B
	 */
	virtual void OnDetacted(void) 
	{
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

private:
	/// �{�^����
	enum EButtonKind
	{
		BK_Close = 0,	///< Close�{�^���BHitTest�� HTCLOSE �B Message�� SC_CLOSE �B
		BK_Minimum,		///< �ŏ����{�^���BHitTest�� HTMINBUTTON �B Message�� SC_MINIMIZE �B
		BK_Maximum,		///< �ő剻�{�^���BHitTest�� HTMAXBUTTON �B Message�� SC_MAXIMIZE �B
		BK_Restore,		///< Restore�{�^���BHitTest�� HTMAXBUTTON �B Message�� SC_RESTORE �B
		BK_Help,		///< Help�{�^���BHitTest�� HTHELP �B Message�� SC_CONTEXTHELP �B
		BK_Nothing,		///< �Ȃ��B
		BK_Max = BK_Nothing,
	};
	/// ���j���[���`�F�b�N���A�{�^���̕\���A�L���ݒ�������Ȃ�
	void m_CheckMenu(void)
	{
		if ( m_canMenuCheck )
		{
			LONG l = ::GetWindowLong(m_hWnd, GWL_STYLE);
			loop ( i, BK_Max )
			{
				m_button[i].SetVisibleMode(false);
			}
			if ( (l & WS_SYSMENU) != 0 )
			{
				m_button[BK_Close].SetVisibleMode(true);
				HMENU hMenu = ::GetSystemMenu(m_hWnd, FALSE);
				UINT state = ::GetMenuState(hMenu, SC_CLOSE, MF_BYCOMMAND);
				m_button[BK_Close].SetEnableMode((state & (MF_GRAYED | MF_DISABLED)) == 0);
				if ( (l & WS_MINIMIZEBOX) != 0 )
				{
					m_button[BK_Minimum].SetVisibleMode(true);
					m_button[BK_Minimum].SetEnableMode(true);
					if ( (l & WS_MAXIMIZEBOX) == 0 )
					{
						m_button[BK_Maximum].SetVisibleMode(true);
						m_button[BK_Maximum].SetEnableMode(false);
					}
				}
				if ( (l & WS_MAXIMIZEBOX) != 0 )
				{
					int b = ::IsZoomed(m_hWnd) ? BK_Restore : BK_Maximum;
					m_button[b].SetVisibleMode(true);
					if ( (l & WS_MINIMIZEBOX) == 0 )
					{
						m_button[BK_Minimum].SetVisibleMode(true);
						m_button[BK_Minimum].SetEnableMode(false);
					}
				}
			}
			if ( (l & DS_CONTEXTHELP) != 0 )
			{
				m_button[BK_Help].SetVisibleMode(true);
			}
		}
	}
	/**
	 * [�`��] �t���[���`��
	 *	@param dc DC�B
	 *	@param size �t���[���`��T�C�Y�B
	 *	@param rect �^�C�g���`��͈́B
	 *	@param isActive true �Ȃ�A�N�e�B�u�摜�`��
	 */
	void m_OnFramePaint(HDC dc, const CSize& size, const CRect& rect, bool isActive)
	{
		CBitmapImage bi;
		bi.Set(size.cx, size.cy);
		{
			HDC dc = bi.GetDC();
			if ( ! m_pFrameDrawer.IsNull() )
			{
				::SetBrushOrgEx(dc, m_frameWidths.left, m_frameWidths.top, NULL);
				m_pFrameDrawer->Resize(size);
				m_pFrameDrawer->DrawEx(dc, 0, 0, ! isActive);
			}
			if ( ! m_pTitleDrawer.IsNull() )
			{
				m_pTitleDrawer->Resize(rect.Size());
				m_pTitleDrawer->DrawEx(dc, rect.left, rect.top, ! isActive);
			}
			else
			{
				CWindowCaptionDrawer t(m_hWnd);
				t.Resize(rect.Size());
				t.DrawEx(dc, rect.left, rect.top, ! isActive);
			}
			loop ( i, BK_Max )
			{
				m_button[i].Draw(dc, m_windowSize);
			}
			m_menuBar.Draw(dc);
			bi.ReleaseDC();
		}
		bi.Draw(dc, 0, 0);
	}

	/**
	 * [�m�F] �q�b�g�e�X�g
	 *	@param r �I���W�i�����Ԃ����e�X�g�l
	 *	@param pos �}�E�X�̃E�B���h�E���΍��W
	 *	@return �}�E�X�ʒu�������e�X�g�l
	 */
	LRESULT m_OnNcHitTest(LRESULT r, const POINT& pos)
	{
		loop ( i, BK_Max )
		{
			LRESULT l = m_button[i].HitTest(pos, m_windowSize);
			if ( l != HTNOWHERE )
			{
				return l;
			}
		}
		if ( r == HTSYSMENU )
		{
		}
		else if ( m_titleRect.PtInRect(pos) )
		{
			r = HTCAPTION;
		}
		else if ( r == HTCAPTION )
		{
			r = HTNOWHERE;
		}
		else if ( r == HTCLOSE || r == HTMINBUTTON || r == HTMAXBUTTON )
		{
			r = HTCAPTION;
		}
//		TRACE3("OnNcHitTest x=%d, y=%d, r=%d\n", pos.x, pos.y, r);
		return r;
	}

	/**
	 * [�ʒm] �}�E�X���[�u�ʒm
	 * @param wParam �}�E�X�ʒu�������e�X�g���ʒl
	 */
	void m_OnNcMouseMove(WPARAM wParam)
	{
		bool r = false;
		if ( m_pressedButton == BK_Nothing )
		{
			loop ( i, BK_Max )
			{
				r |= m_button[i].SetHoverMode(wParam);
			}
		}
		else
		{
			r |= m_button[m_pressedButton].SetPressMode(wParam);
			r |= m_button[m_pressedButton].SetHoverMode(wParam);
		}
		if ( r )
		{
			m_NcPaint();
		}
	}

	/**
	 * [�ʒm] �{�^���C�x���g�ʒm
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam �}�E�X�ʒu�������e�X�g�l
	 *	@param[in,out] _lParam ���W�B�ύX�\�B
	 *	@retval true ��������
	 *	@retval false default�̏������s��
	 */
	bool m_OnNcButton(UINT message, WPARAM wParam, LPARAM& _lParam)
	{
		switch ( message )
		{
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
		default:
			return false;
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
			if ( wParam == HTCLOSE )
			{
				_lParam = MAKELONG(-1, -1);
			}
			return false;
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_LBUTTONUP:
			break;
		}
		if ( message == WM_NCLBUTTONUP || message == WM_LBUTTONUP)
		{
			if ( m_pressedButton == BK_Nothing )
			{
				return false;
			}
			EButtonKind b = m_pressedButton;
			m_SetButtonPressState(BK_Nothing);
			m_button[b].SetPressMode(false);
			m_NcPaint();
			if ( m_button[b].IsHitTest(wParam) )
			{
				m_button[b].PostSysCommand(GetSafeHwnd());
			}
			return true;
		}
		loop ( i, BK_Max )
		{
			if ( m_button[i].IsHitTest(wParam) && m_button[i].isEnable() )
			{
				m_SetButtonPressState(static_cast<EButtonKind>(i));
				m_button[i].SetPressMode(true);
				m_NcPaint();
				return true;
			}
		}
		return false;
	}

	/**
	 * [�v�Z] �T�C�Y�v�Z
	 *	@note �E�B���h�E�̈ʒu��T�C�Y���ύX���ꂽ�Ƃ��ɁA�A�v���P�[�V�����ŃE�B���h�E�̃N���C�A���g�̈�𐧌�ł��܂��B
	 *			boCalcValidRects �̒l�Ƃ͖��֌W�ɁANCCALCSIZE_PARAMS �\���̂̃����o rgrc �Ŏw�肵���z��̍ŏ��̎l�p�`��
	 *			�E�B���h�E�̍��W�������܂��B�q�E�B���h�E�ł̍��W�́A�e�E�B���h�E�̃N���C�A���g�̈�̑��΍��W�ł��B
	 *			�g�b�v���x�� �E�B���h�E�ł̍��W�́A��ʍ��W�ł��B
	 *			�A�v���P�[�V�����́A�N���C�A���g�̈�̃T�C�Y�ƈʒu�𔽉f���邽�߂ɁArgrc[0] �̎l�p�`���C������K�v������܂��B 
	 *			boCalcValidRects �� TRUE �̂Ƃ��̂݁Argrc[1] �� rgrc[2] �̎l�p�`���L���ɂȂ�܂��B
	 *			���̂Ƃ� rgrc[1] �ɂ́A�ړ��܂��̓T�C�Y�ύX�����O�̃E�B���h�E�̍��W��ێ����܂��B
	 *			rgrc[2] �ɂ́A�E�B���h�E���ړ������O�̃E�B���h�E�̃N���C�A���g�̈�̍��W���ێ�����܂��B
	 *			���ׂĂ̍��W�́A�e�E�B���h�E�܂��͉�ʂ̑��΍��W�ł��B
	 *
	 *			����̎����ł́A�N���C�A���g�̈�̃T�C�Y���E�B���h�E�̓��� (�X�N���[�� �o�[�⃁�j���[�Ȃǂ̗L��) 
	 *			����Ɍv�Z���A���ʂ� lpncsp �ɐݒ肵�܂��B
	 *	@param boCalcValidRects
	 *					�N���C�A���g�̈���̗L���ȏ����܂ޕ������A�v���P�[�V�������w�肷��K�v�����邩�ǂ������w�肵�܂��B
	 *					Windows �͗L���ȏ���V�����N���C�A���g�̈���̎w�肳�ꂽ�̈�փR�s�[���܂��B
	 *					���̃p�����[�^�� TRUE �̏ꍇ�́A�A�v���P�[�V�����̓N���C�A���g�̈�̂ǂ̕������L���ł��邩���w�肵�܂��B
	 *	@param pParams 
	 *					���̍\���̂́A�A�v���P�[�V������ CWnd �̎l�p�` (�N���C�A���g�̈�A���E���A�^�C�g���A�X�N���[�� �o�[�Ȃǂ��܂�)
	 *					�̐V�������W��T�C�Y���v�Z���邽�߂̏���ێ����܂��B
	 *					
	 */
	void m_OnNcCalcSize(bool boCalcValidRects, NCCALCSIZE_PARAMS* pParams)
	{
		if ( pParams->lppos == NULL || ! boCalcValidRects ) 
		{
			return;
		}
		CPoint po(pParams->lppos->x, pParams->lppos->y);
		CRect rc(CPoint(0, 0), CSize(pParams->lppos->cx, pParams->lppos->cy));
		rc.DeflateRect(m_frameWidths);
		MENUBARINFO mbi = { sizeof(MENUBARINFO) };
		m_menuHeight = 0;
		if ( ::GetMenuBarInfo(m_hWnd, OBJID_MENU, 0, &mbi) )
		{
			m_menuHeight = (mbi.rcBar.bottom - mbi.rcBar.top) + 1;
		}
		rc.top += m_menuHeight;
		pParams->rgrc[0] = rc + po;
	}

	/**
	 * [�ݒ�] �{�^��������Ԑݒ�
	 *	@param bn �{�^���ԍ�
	 */
	void m_SetButtonPressState(EButtonKind bn)
	{
		if ( bn == BK_Nothing )
		{
			if ( m_pressedButton != BK_Nothing )
			{
				m_pressedButton = BK_Nothing;
				::ReleaseCapture();
			}
		}
		else
		{
			if ( m_pressedButton == BK_Nothing )
			{
				m_pressedButton = bn;
				::SetCapture(m_hWnd);
			}
		}
	}

	/**
	 * �{�^���Ǘ��N���X
	 */
	class CMyButton
	{
	public:
		/// �R���X�g���N�^
		CMyButton(void) : m_onButton(false), m_isPress(false), m_isEnable(true), m_isVisible(false), m_size(0, 0), m_point(0, 0)
		{
		}
		/**
		 * [�v�Z] �ʒu�v�Z
		 *	@param[out] _rect �E�B���h�E���W�ł̃{�^���ʒu���傫��
		 *	@param[in] sz �E�B���h�E�̑傫��
		 */
		void GetRect(RECT& _rect, const SIZE& sz) const
		{
			if ( m_isVisible )
			{
				_rect.left = (m_point.x >= 0) ? (m_point.x) : (sz.cx + m_point.x - m_size.cx);
				_rect.right = _rect.left + m_size.cx;
				_rect.top = (m_point.y >= 0) ? (m_point.y) : (sz.cy + m_point.y - m_size.cy);
				_rect.bottom = _rect.top + m_size.cy;
			}
			else
			{
				::SetRectEmpty(&_rect);
			}
		}
		/**
		 * [�m�F] �L���m�F
		 *	@retval true �L���B
		 *	@retval false �����B
		 */
		bool isEnable(void) const
		{
			return m_isVisible && m_isEnable && ! m_pDrawer.IsNull();
		}
		/**
		 * [����] �q�b�g�e�X�g
		 *	@param pos �}�E�X�̃E�B���h�E���΍��W
		 *	@param sz �E�B���h�E�̑傫��
		 *	@retval HTNOWHERE�ȊO ��ɂ���B
		 *	@retval HTNOWHERE �Ȃ��B
		 */
		LRESULT HitTest(const POINT& pos, const SIZE& sz) const
		{
			CRect rc;
			GetRect(rc, sz);
			return rc.PtInRect(pos) ? m_hitTest : HTNOWHERE;
		}
		/**
		 * [����] �q�b�g�e�X�g
		 *	@param wParam �q�b�g�e�X�g����
		 *	@retval true ��ɂ���B
		 *	@retval falseE �Ȃ��B
		 */
		bool IsHitTest(WPARAM wParam) const
		{
			return wParam == m_hitTest;
		}
		/**
		 * [�ݒ�] �\����Ԑݒ�
		 *	@param isVisible true;�\���� false;��
		 */
		void SetVisibleMode(bool isVisible)
		{
			m_isVisible = isVisible;
		}
		/**
		 * [�ݒ�] �z�o�[��Ԑݒ�
		 *	@param isEnable true;�z�o�[�� false;��
		 */
		void SetEnableMode(bool isEnable)
		{
			m_isEnable = isEnable;
		}
		/**
		 * [�ݒ�] �z�o�[��Ԑݒ�
		 *	@param wParam �q�b�g�e�X�g����
		 *	@retval true �ω�����B
		 *	@retval false ���ɖ����B
		 */
		bool SetHoverMode(WPARAM wParam)
		{
			bool isHover = (wParam == m_hitTest);
			bool r = (m_onButton != isHover);
			m_onButton = isHover;
			return r;
		}
		/**
		 * [����] �}�E�X�{�^������
		 *	@param wParam �q�b�g�e�X�g����
		 *	@retval true �ω�����B
		 *	@retval false ���ɖ����B
		 */
		bool SetPressMode(WPARAM wParam)
		{
			bool isPress = (wParam == m_hitTest);
			bool r = (m_isPress != isPress);
			m_isPress = isPress;
			return r;
		}
		/**
		 * [����] �}�E�X�{�^������
		 *	@param isPress true;�{�^��DOWN false;�{�^��UP
		 */
		void SetPressMode(bool isPress)
		{
			m_isPress = isPress;
		}
		/**
		 * [�`��] �`��.
		 *	@note ���݂̏�Ԃŕ`��
		 *	@param dc DC
		 *	@param sz �E�B���h�E�̑傫��
		 */
		void Draw(HDC dc, const SIZE& sz) const
		{
			if ( ! m_isVisible || m_pDrawer.IsNull() ) 
			{
				return;
			}
			CRect rc;
			GetRect(rc, sz);
			LPARAM p = 0;
			if ( ! m_isEnable )
			{
				p = 3;
			}
			else if ( m_isPress )
			{
				p = 1;
			}
			else if ( m_onButton )
			{
				p = 2;
			}
			m_pDrawer->DrawEx(dc, rc.left, rc.top, p);
		}
		/**
		 * [�ݒ�] �{�^���ݒ�
		 *	@param pos �{�^���ʒu�B���W���}�C�i�X�̏ꍇ�A�E�[(���[)����̃I�t�Z�b�g�ɂȂ�܂��B
		 *	@param draw �`����Bbank 0;Normal 1;Press 2;Hover
		 *	@param hitTest �q�b�g�e�X�g�l
		 *	@param msg ���b�Z�[�W
		 */
		void SetDrawer(const POINT& pos, const IDrawable& draw, UINT hitTest, UINT msg)
		{
			m_pDrawer = draw.Clone();
			m_point = pos;
			m_pDrawer->GetSize(m_size);
			m_hitTest = hitTest;
			m_message = msg;
		}
		/**
		 * [���M] �V�X�e���R�}���h���M
		 *	@param hWnd ���M��E�B���h�E
		 */
		void PostSysCommand(HWND hWnd)
		{
			::PostMessage(hWnd, WM_SYSCOMMAND, m_message, 0);
		}
	private:
		CSize			m_size;				///< �{�^���T�C�Y
		CPoint			m_point;			///< ���΍��W�B�}�C�i�X�̏ꍇ�A�E(��)����̑��΍��W
		IDrawable::Ptr	m_pDrawer;			///< Bank����`����  0;�ʏ� 1;���� 2;�z�o�[ 3;����
		bool			m_onButton;			///< ��ɂ���H
		bool			m_isPress;			///< �������H
		bool			m_isEnable;			///< �L���H
		bool			m_isVisible;		///< �\���H
		UINT			m_hitTest;			///< �q�b�g�e�X�g�l
		UINT			m_message;			///< ���M���b�Z�[�W
	};

	// ��pBrush�`��
	class CMyBrushDrawer : public CNullDrawer
	{
		DEFSUPER(CNullDrawer);
		CBrushHandle	m_brush;
	public:
		/// �R���X�g���N�^
		CMyBrushDrawer(CBrushHandle brush) : m_brush(brush)
		{
		}
		/// �N���[���쐬.
		virtual IDrawable* Clone(void) const 
		{
			return new CMyBrushDrawer(*this); 
		}
		/// [�`��] �`��.
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			RECT r = { x, y, x + m_size.cx, y + m_size.cy };
			::FillRect(dc, &r, m_brush);
		}
	};
	COLORREF			m_textColor;			///< �����F
	IDrawable::Ptr		m_pFrameDrawer;			///< �t���[���`��(Bank 0;�A�N�e�B�u 1;��A�N�e�B�u)
	CBrushHandle		m_backGround[2];		///< �w�i�C���[�W(Bank 0;�A�N�e�B�u 1;��A�N�e�B�u)
	bool				m_isEvenBackGround;		///< true �Ȃ�w�i�C���[�W�͓�������
	IDrawable::Ptr		m_pTitleDrawer;			///< �^�C�g���`��(Bank 0;�A�N�e�B�u 1;��A�N�e�B�u)
	CRect				m_frameWidths;			///< �t���[���̌���
	CRect				m_titleRect;			///< �^�C�g���o�[�Ƃ���͈�(Right�͎g���܂���)
	int					m_titleHeight;			///< �^�C�g������
	bool				m_isActive;				///< �A�N�e�B�u?
	CPoint				m_windowPoint;			///< �E�B���h�E�ʒu
	CSize				m_windowSize;			///< �E�B���h�E�T�C�Y
	int					m_menuHeight;			///< ���j���[����
	bool				m_isInitWindowRgn;		///< true �Ȃ烊�[�W����������
	HRGN				m_hNcRgn;				///< 
	COLORREF			m_transColor;			///< ���ߐF

	CMyButton			m_button[BK_Max];		///< �{�^��
	EButtonKind			m_pressedButton;		///< �v���X���̃{�^�� BK_Nothing;�v���X����
	bool				m_boHasHover;			///< true �Ȃ�z�o�[���
	bool				m_isInit;				///< true �Ȃ珉������
	bool				m_canMenuCheck;			///< true �Ȃ烁�j���[�̏�ԃ`�F�b�N���s���Ă��悢
	//
	CDrawingMenuBar		m_menuBar;				///< �}�C���j���[�o�[
	CDrawingMenu		m_popupMenu;			///< �}�C�|�b�v�A�b�v���j���[
	bool				m_isMenuInit;			///< ���j���[�������݁H
	bool				m_isNcResized;			///< �T�C�Y�ύX���� true

	/// �E�B���h�E�̌`�Đݒ�
	void m_SetWindowRgn(void)
	{
		if ( m_hNcRgn != NULL )
		{
			HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
			::CombineRgn(rgn, m_hNcRgn, NULL, RGN_COPY);
			::SetWindowRgn(m_hWnd, rgn, true);
		}
	}
	/**
	 * SYSTEM���j���[�𖳌����Č��̃v���V�[�W�����R�[��.
	 */
	LRESULT m_OrgWndProcWithNoSysmeny(UINT message, WPARAM wParam, LPARAM lParam)
	{
		m_canMenuCheck = false;
		LONG nowStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		::SetWindowLong(m_hWnd, GWL_STYLE, nowStyle & (~WS_SYSMENU));
		LRESULT r = _super::WindowProc(message, wParam, lParam);
		::SetWindowLong(m_hWnd, GWL_STYLE, nowStyle);
		m_canMenuCheck = true;
		return r;
	}

	void m_NcPaint(void)
	{
		CRect rc;
		::GetWindowRect(m_hWnd, rc);
		CRect cliRc;
		::GetClientRect(m_hWnd, cliRc);
		if ( m_isInitWindowRgn )
		{
			m_isInitWindowRgn = false;
			if ( m_hNcRgn != NULL )
			{
				_DeleteObject(m_hNcRgn);
			}
			if ( ! m_frameWidths.IsRectNull() && m_transColor != CLR_INVALID && ! m_pFrameDrawer.IsNull() )
			{
				m_pFrameDrawer->Resize(rc.Size());
				CBitmapImage bi = CBitmapDrawer::ToBitmap(*m_pFrameDrawer, m_transColor);
				m_hNcRgn = bi.CreateRgn(m_transColor);
			}
			m_SetWindowRgn();
		}
		cliRc.OffsetRect(m_frameWidths.left, m_frameWidths.top + m_menuHeight);
		HDC dc = ::GetWindowDC(m_hWnd);
		::ExcludeClipRect(dc, cliRc.left, cliRc.top, cliRc.right, cliRc.bottom);
		m_titleRect.right = rc.Width() - m_frameWidths.right;
		if ( m_menuHeight != 0 )
		{
			// ���j���[����Ȃ�A�^�C�g���o�[�̍����͌Œ�
			int h = ::GetSystemMetrics(SM_CYCAPTION);
			m_titleRect.bottom = m_titleRect.top + h;
			m_frameWidths.top -= m_titleHeight;
			m_frameWidths.top += h;
			m_titleHeight = h;
		}
		m_OnFramePaint(dc, rc.Size(), m_titleRect, m_isActive);
		::ReleaseDC(m_hWnd, dc);
	}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_INITDIALOG:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_CheckMenu();
				return r;
			}
			break;
		case WM_CTLCOLOREDIT:
			{
				HWND hWnd = reinterpret_cast<HWND>(lParam);
				if ( (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TRANSPARENT) != 0 )
				{
					HDC dc = reinterpret_cast<HDC>(wParam);
					::SetBkMode(dc, TRANSPARENT);
					HBRUSH b = m_backGround[m_isActive ? 0 : 1];
					_super::WindowProc(message, wParam, lParam);
					if ( IS_RGBVALUE(m_textColor) )
					{
						::SetTextColor(dc, m_textColor);
					}
					return reinterpret_cast<LRESULT>(b);
				}
			}
			break;
		case WM_CTLCOLORSTATIC:
			{
				HWND hWnd = reinterpret_cast<HWND>(lParam);
				LRESULT r = ::SendMessage(hWnd, WM_GETDLGCODE, 0, 0);
				if ( (r & DLGC_HASSETSEL) != 0 )
				{
					// EDIT��ReadOnly
				}
				//�ȉ��ɑ���
			}	
		case WM_CTLCOLORBTN:
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				::SetBkMode(dc, TRANSPARENT);
				HBRUSH b = m_backGround[m_isActive ? 0 : 1];
				_super::WindowProc(message, wParam, lParam);
				if ( IS_RGBVALUE(m_textColor) )
				{
					::SetTextColor(dc, m_textColor);
				}
				return reinterpret_cast<LRESULT>(b);
			}
			break;
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORMSGBOX:
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				HBRUSH b = m_backGround[m_isActive ? 0 : 1];
				_super::WindowProc(message, wParam, lParam);
				if ( IS_RGBVALUE(m_textColor) )
				{
					::SetTextColor(dc, m_textColor);
				}
				return reinterpret_cast<LRESULT>(b);
			}
			break;
		case WM_WINDOWPOSCHANGED:
			{
				CRect oldRc(m_windowPoint, m_windowSize);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				const WINDOWPOS* P = reinterpret_cast<const WINDOWPOS*>(lParam);
				m_windowPoint.x = P->x;
				m_windowPoint.y = P->y;
				bool b = false;
				b |= (m_windowSize.cx != P->cx);
				m_windowSize.cx = P->cx;
				b |= (m_windowSize.cy != P->cy);
				m_windowSize.cy = P->cy;
				if ( b )
				{
					m_isNcResized = true;
					POINT po;
					if ( ::GetCursorPos(&po) )
					{
						WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
						m_OnNcMouseMove(w);
					}
					m_isInit = true;
					m_isInitWindowRgn = true;
					m_CheckMenu();
					m_NcPaint();
				}
				return r;
			}
			break;
		case WM_NCCALCSIZE:
			if ( ! m_isMenuInit )
			{
				m_isMenuInit = true;
				CFrameDrawer f1(false, BDR_SUNKENINNER, BF_RECT);
				CFrameDrawer f2(false, BDR_RAISEDINNER, BF_RECT);
				m_menuBar.SetSelectMarkDrawer(f1, f2);
				m_menuBar.SetBackgroundDrawer(CMyBrushDrawer(m_backGround[0]));
				m_menuBar.ReferSubParameter().CopyParameter(m_menuBar);
				m_menuBar.ReferSubParameter().SetDefaultMarkDrawer();
				m_menuBar.ReferSubParameter().SetSelectMarkDrawer(CBoxInverter());
				m_popupMenu.CopyParameter(m_menuBar);
				m_popupMenu.SetDefaultMarkDrawer();
				m_popupMenu.SetBackgroundDrawer(CMyBrushDrawer(m_backGround[0]));
				m_popupMenu.SetSelectMarkDrawer(CBoxInverter());
				m_menuBar.SetLeftMargin(-1);
			}
			if ( ::GetMenu(m_hWnd) != NULL && m_menuBar.m_hMenu == NULL )
			{
				m_menuBar.Attach(this, false);
				::DrawMenuBar(m_hWnd);
				m_menuBar.Draw();
			}
			{
				NCCALCSIZE_PARAMS* P = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_OnNcCalcSize(wParam != 0, P);
				return r;
			}
			break;
		case WM_SIZE:
		case WM_MOVE:
			m_SetWindowRgn();
			break;
		case WM_NCPAINT:
			m_NcPaint();
			return 0;
			break; 
		case WM_SETCURSOR:
			return m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
			break;
		case WM_NCHITTEST:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				CPoint p(lParam);
				return m_OnNcHitTest(r, CPoint(p.x - m_windowPoint.x, p.y - m_windowPoint.y));
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_pressedButton >= 0 )
			{
				CPoint po(lParam);
				::ClientToScreen(m_hWnd, &po);
				WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
				m_OnNcMouseMove(w);
			}
			break;
		case WM_CAPTURECHANGED:
			if ( m_pressedButton != BK_Nothing )
			{
				m_SetButtonPressState(BK_Nothing);
				loop ( i, BK_Max )
				{
					m_button[i].SetPressMode(false);
				}
			}
			break;
		case WM_ACTIVATE:
			m_isActive = !! wParam;
			if ( ! m_isEvenBackGround )
			{
				m_menuBar.SetBackgroundDrawer(CMyBrushDrawer(m_backGround[m_isActive ? 0 : 1]));
				::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE);
			}
			else
			{
				CRect rc(0, 0, 1, 1);
				::RedrawWindow(m_hWnd, rc, NULL, RDW_FRAME | RDW_NOERASE | RDW_NOINTERNALPAINT | RDW_INVALIDATE);
			}
			m_SetWindowRgn();
			break;
		case WM_NCACTIVATE:
			m_NcPaint();
			return TRUE;
			break;
		case WM_NCMOUSELEAVE:
			if ( m_boHasHover )
			{
				m_boHasHover = false;
				POINT po;
				if ( ::GetCursorPos(&po) )
				{
					WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
					m_OnNcMouseMove(w);
				}
			}
			break;
		case WM_NCMOUSEMOVE:
			if ( m_isNcResized )
			{
				m_isNcResized = false;
				m_SetWindowRgn();
			}
			if ( m_isInit && m_isActive )
			{
				m_isInit = false;
				if ( (::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == 0 )
				{
					SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
					SendMessage(WM_NCLBUTTONUP, HTCAPTION, 0);
				}
				m_NcPaint();
			}
			m_OnNcMouseMove(wParam);
			if ( ! m_boHasHover )
			{
				m_boHasHover = true;
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof (TRACKMOUSEEVENT);
				tme.hwndTrack = m_hWnd;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
				::_TrackMouseEvent(&tme);
			}
			if ( m_isInit )
			{
				m_NcPaint();
			}
			return m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
			break;
		case WM_INITMENU:
			{
				LRESULT r = m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
				m_NcPaint();
				return r;
			}
			break;
		case WM_INITMENUPOPUP:
			if ( m_popupMenu.m_hMenu != NULL )
			{
				m_popupMenu.Detach();
			}
			m_popupMenu.Attach(this, reinterpret_cast<HMENU>(wParam), true, true);
			if ( HIWORD(lParam) == 1 )
			{
				LRESULT r = m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
				m_NcPaint();
				return r;
			}
			break;
		case WM_MENUSELECT:
		case WM_STYLECHANGED:
			if ( m_canMenuCheck )
			{
				m_CheckMenu();
				m_NcPaint();
			}
			break;
		case WM_LBUTTONUP:
			if ( m_pressedButton >= 0 )
			{
				CPoint po(lParam);
				::ClientToScreen(m_hWnd, &po);
				WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
				if ( m_OnNcButton(message, w, lParam) )
				{
					return 0;
				}
			}
			break;
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONUP:
			if ( m_OnNcButton(message, wParam, lParam) )
			{
				return 0;
			}
			switch ( wParam )
			{
			case HTCAPTION:
			case HTLEFT:
			case HTRIGHT:
			case HTTOP:
			case HTTOPLEFT:
			case HTTOPRIGHT:
			case HTBOTTOM:
			case HTBOTTOMLEFT:
			case HTBOTTOMRIGHT:
				return m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
			}
			break;
		case WM_NCLBUTTONUP :
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONDBLCLK:
			if ( m_OnNcButton(message, wParam, lParam) )
			{
				return 0;
			}
			break;
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
			return 0;
		case WM_DESTROY:
		case WM_NCDESTROY:
			{
				if ( m_hNcRgn != NULL )
				{
					_DeleteObject(m_hNcRgn);
					m_hNcRgn = NULL;
				}
				::ReleaseCapture();
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				_super::Destroy();
				return r;
			}
			break;
		case WM_THEMECHANGED:
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}
};



}; //MFC
}; //TNB
