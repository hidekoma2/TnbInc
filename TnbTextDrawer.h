#pragma once
/**
 *@file
 * �e�L�X�g�`��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDrawable.h"
#include "TnbDcSelectAssistant.h"
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #ifndef BS_TYPEMASK
  #define BS_TYPEMASK 0x0000000F
 #endif
 #ifndef SS_TYPEMASK
  #define SS_TYPEMASK 0x0000001FL
 #endif
#endif



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * �e�L�X�g�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�e�L�X�g��`�悵�܂��B
 *
 *	@note �e�L�X�g�\���p�̃T�[�r�X�֐�������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTextDrawer.h
 *
 *	@date 07/11/15 �V�K�쐬
 *	@date 07/12/13 VCENTER , BOTTOM �̎��̓�����}���`���C���ł����Ғʂ�ɏo��悤�ɂ����B
 *	@date 08/01/11 ���}�[�W����ǉ�
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 *	@date 08/02/22 Disable�����\�����@�ύX�B
 *	@date 10/02/26 �Q�b�^�[���\�b�h�ǉ��B
 *	@date 10/03/02 ModifyText() ���\�b�h�A�ǉ��B
 *	@date 15/04/23 �R���X�g���N�g���T�C�Y���A�W���X�g����悤�ɕύX�B
 */
class CTextDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * �R���X�g���N�^
	 *	@param size	�T�C�Y
	 *	@param font	�t�H���g
	 *	@param textColor �J���[
	 *	@param str ������
	 *	@param drawStyle DrawText�X�^�C���w��B	�ȗ������ �uDT_CENTER | DT_VCENTER�v
	 *	@param margin ���̃}�[�W�B�ȗ�����ƂO�ɂȂ�܂��B
	 */
	CTextDrawer(const SIZE& size, HFONT font, COLORREF textColor, LPCTSTR str, DWORD drawStyle = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(size), m_font(font), m_textColor(textColor), m_str(str), m_drawStyle(drawStyle), m_margin(margin)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 *	@param font	�t�H���g
	 *	@param textColor �J���[
	 *	@param str ������
	 *	@param drawStyle DrawText�X�^�C���w��B	�ȗ������ �uDT_CENTER | DT_VCENTER�v
	 *	@param margin ���̃}�[�W�B�ȗ�����ƂO�ɂȂ�܂��B
	 */
	CTextDrawer(HFONT font, COLORREF textColor, LPCTSTR str, DWORD drawStyle = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(), m_font(font), m_textColor(textColor), m_str(str), m_drawStyle(drawStyle), m_margin(margin)
	{
		AdjustSize(0);
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CTextDrawer(m_size, m_font, m_textColor, m_str, m_drawStyle, m_margin);
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
		RECT rc = { x + m_margin, y, x + m_size.cx, y + m_size.cy };
		CDcSelectAssistant dca(dc);
//		int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
		dca.SetBkMode(TRANSPARENT);
		if ( m_font != NULL )
		{
//			HGDIOBJ oldObj = ::SelectObject(dc, m_font);
			dca.SelectFont(m_font);
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
//			::SelectObject(dc, oldObj);
		}
		else
		{
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
		}
//		::SetBkMode(dc, oldBkMode);
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *	@param color �J���[
	 */
	void SetTextColor(COLORREF color)
	{
		m_textColor = color;
	}

	/**
	 * [�擾] �����F�擾.
	 *	@return �J���[
	 */
	COLORREF GetTextColor(void) const
	{
		return m_textColor;
	}

	/**
	 * [�ݒ�] ������ݒ�.
	 *	@note �T�C�Y�͕ω����܂���B
	 *	@param str ������
	 */
	void SetString(LPCTSTR str)
	{
		m_str = str;
	}

	/**
	 * [�擾] ������擾.
	 *	@return  ������
	 */
	LPCTSTR GetString(void) const
	{
		return m_str;
	}

	/**
	 * [�ݒ�] �X�^�C���ݒ�.
	 *	@note �T�C�Y�͕ω����܂���B
	 *	@param drawStyle DrawText�X�^�C���w��B	�ȗ������ �uDT_CENTER | DT_VCENTER�v
	*/
	void SetDrawStyle(DWORD drawStyle = DT_CENTER | DT_VCENTER)
	{
		m_drawStyle = drawStyle;
	}

	/**
	 * [�擾] �X�^�C���擾.
	 *	@return DrawText�X�^�C��
	*/
	DWORD GetDrawStyle(void) const
	{
		return m_drawStyle;
	}

	/**
	 * [�ݒ�] �T�C�Y����.
	 *		���݂̃T�C�Y�ƕ������DrawText�X�^�C�����g���āA�傫���𒲐����܂��B
	 *	@param margin �}�[�W���B�K�v�ȃT�C�Y�ɉ�����T�C�Y�ł��B
	 *	@param hWnd DC���擾����n���h�����w�肵�܂��B�ȗ��\�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AdjustSize(int margin = 1, HWND hWnd = NULL)
	{
		HDC dc = ::GetDC(hWnd);
		CDcSelectAssistant dca(dc);
//		HGDIOBJ oldObj = ::SelectObject(dc, m_font);
		dca.SelectFont(m_font);
		SIZE s = m_size;
		bool r = CalcTextSize(s, dc, m_drawStyle, m_str);
//		if ( oldObj != NULL )
//		{
//			::SelectObject(dc, oldObj);
//		}
		dca.Restore();
		::ReleaseDC(hWnd, dc);
		if ( r )
		{
			m_size.cx = s.cx + margin + m_margin;
			m_size.cy = s.cy + margin;
		}
		return r;
	}

	
	// -----------------

	
	/**
	 * [�ϊ�] Static�X�^�C����DrawText�X�^�C���ɕϊ�
	 *	@param staticStyle Static�X�^�C��
	 *	@return DrawText�X�^�C��
	 */
	static DWORD StaticToDrawStyle(DWORD staticStyle)
	{
		UINT r = 0;
		switch ( staticStyle & SS_TYPEMASK )
		{
		case SS_CENTER:		r = DT_CENTER;		break;
		case SS_LEFT:		r = DT_LEFT;		break;
		case SS_RIGHT:		r = DT_RIGHT;		break;
		case SS_SIMPLE:		r = DT_SINGLELINE;	break;
		};
		if ( (staticStyle & SS_TYPEMASK) != SS_LEFTNOWORDWRAP )
		{
			r |= DT_WORDBREAK;
		}
		if ( (staticStyle & SS_CENTERIMAGE) != 0 )
		{
			r |= DT_VCENTER | DT_SINGLELINE;
		}
		if ( (staticStyle & SS_NOPREFIX) != 0 )
		{
			r |= DT_NOPREFIX;
		}
		#ifndef _WIN32_WCE
			if ( (staticStyle & SS_WORDELLIPSIS) == SS_WORDELLIPSIS )
			{
				r |= DT_WORD_ELLIPSIS;
			}
			else if ( (staticStyle & SS_ENDELLIPSIS) != 0 )
			{
				r |= DT_END_ELLIPSIS;
			}
			else if ( (staticStyle & SS_PATHELLIPSIS) != 0 )
			{
				r |= DT_PATH_ELLIPSIS;
			}
		#endif
		return r;
	}

	/**
	 * [�ϊ�] Button�X�^�C����DrawText�X�^�C���ɕϊ�
	 *	@param buttonStyle Button�X�^�C��
	 *	@return DrawText�X�^�C��
	 */
	static DWORD ButtonToDrawStyle(DWORD buttonStyle)
	{
		DWORD r = 0;
		bool isPushButton = false;
		if ( (buttonStyle & BS_TYPEMASK) <= BS_DEFPUSHBUTTON || (buttonStyle & BS_PUSHLIKE) != 0 )
		{
			isPushButton = true;
		}
		switch ( buttonStyle & 0x0300 )
		{
		case BS_LEFT:	/*0x00000100L*/		r |= DT_LEFT;		break;
		case BS_RIGHT:	/*0x00000200L*/		r |= DT_RIGHT;		break;
		case BS_CENTER:	/*0x00000300L*/		r |= DT_CENTER;		break;
		default:
			r |= (isPushButton ? DT_CENTER : DT_LEFT);
			break;
		}

		switch ( buttonStyle & 0x0C00 )
		{
		case BS_TOP:	/*0x00000400L*/		r |= DT_TOP;		break;
		case BS_BOTTOM:	/*0x00000800L*/		r |= DT_BOTTOM;		break;
		default:
		case BS_VCENTER:/*0x00000C00L*/		r |= DT_VCENTER;	break;
		}
		if ( (buttonStyle & BS_MULTILINE) == 0 )
		{
			r |= DT_SINGLELINE;
		}
		else
		{
			r |= DT_WORDBREAK;
		}
		return r;
	}

	/**
	 * [�v�Z] �����\���傫���v�Z.
	 *	@param[in,out] _size �\���T�C�Y�B�X�^�C���ɕ����s���܂܂�Ă���ꍇ�A�֐��R�[�����A���͐ݒ肵�Ă����Ă��������B
	 *					�߂�� bottom ���ݒ肳��܂��B
	 *	@param[in] dc �f�o�C�X�R���e�L�X�g
	 *	@param[in] drawStyle DrawText�X�^�C��
	 *	@param[in] str �\������
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool CalcTextSize(SIZE& _size, HDC dc, UINT drawStyle, LPCTSTR str)
	{
		RECT rc = { 0, 0, _size.cx, _size.cy };
		int r = ::DrawText(dc, str, -1, &rc, (drawStyle & 0xFFFFFFF0) | DT_CALCRECT);
		_size.cx = rc.right;
		_size.cy = rc.bottom;
		return r != 0;
	}

	/**
	 * [�v�Z] �����\���͈͌v�Z.
	 *	@param[in,out] _rect �\���͈́B�߂�ŁA�\�������͈͂��ݒ肳��܂��B
	 *	@param[in] dc �f�o�C�X�R���e�L�X�g
	 *	@param[in] drawStyle DrawText�X�^�C��
	 *	@param[in] str �\������
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool CalcTextRect(RECT& _rect, HDC dc, DWORD drawStyle, LPCTSTR str)
	{
		SIZE sz = { _rect.right - _rect.left, _rect.bottom - _rect.top };
		if ( ! CalcTextSize(sz, dc, drawStyle, str) ) { return false; }
		RECT rc = { 0, 0, sz.cx, sz.cy };
		int cx = (_rect.right - _rect.left) - sz.cx;
		int cy = (_rect.bottom - _rect.top) - sz.cy;
		if ( (drawStyle & DT_CENTER) != 0 )
		{
			cx /= 2;
		}
		else if ( (drawStyle & DT_RIGHT) == 0 )
		{
			cx = 0;
		}
		if ( (drawStyle & DT_VCENTER) != 0 )
		{
			cy /= 2;
		}
		else if ( (drawStyle & DT_BOTTOM) == 0 )
		{
			cy = 0;
		}
		::OffsetRect(&rc, cx + _rect.left, cy + _rect.top);
		_rect = rc;
		return true;
	}

	/**
	 * [�\��] �͈͕����\��.
	 *	@param[in,out] _rect �\���͈́B�߂�ŁA�\�������͈͂��ݒ肳��܂��B
	 *	@param[in] dc �f�o�C�X�R���e�L�X�g
	 *	@param[in] drawStyle DrawText�X�^�C��
	 *	@param[in] offset ����1�ƕ����Q�̂���
	 *	@param[in] color1 �����F�P���w�肵�܂��B CLR_INVALID ���w�肷��ƁA �x�̂���O���[�̕����ɂȂ�܂��B
	 *					CLR_AUTOSELECT ���w�肷��ƁA���݂̃e�L�X�g�J���[���g���܂��B
	 *	@param[in] color2 �����F�Q���w�肵�܂��B
	 *	@param[in] str �\������
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool DrawTextRect(RECT& _rect, HDC dc, DWORD drawStyle, const POINT& offset, COLORREF color1, COLORREF color2, LPCTSTR str)
	{
		if ( ! CalcTextRect(_rect, dc, drawStyle, str) ) 
		{
			return false;
		}
		drawStyle &= ~DT_VCENTER;
		RECT rc = _rect;
		if ( color1 == CLR_INVALID )
		{
			#ifndef _WIN32_WCE
				::DrawState(dc, NULL, &ms_DrawStateProc,
					reinterpret_cast<LPARAM>(str),
					static_cast<WPARAM>(drawStyle), 
					rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, DST_COMPLEX | DSS_DISABLED);
				return true;
			#else
				POINT po = { 1, 1 };
				return DrawTextRect(dc, _rect, drawStyle, po, ::GetSysColor(COLOR_GRAYTEXT), ::GetSysColor(COLOR_3DHILIGHT), str);
			#endif
		}
		if ( color1 == CLR_AUTOSELECT )
		{
			color1 = ::GetTextColor(dc);
		}
		if ( color2 == CLR_INVALID )
		{
			COLORREF oldColor = ::SetTextColor(dc, color1);
			::DrawText(dc, str, -1, &rc, drawStyle);
			::SetTextColor(dc, oldColor);
		}
		else
		{
			COLORREF oldColor = ::SetTextColor(dc, color2);
			::OffsetRect(&rc, offset.x, offset.y);
			::DrawText(dc, str, -1, &rc, drawStyle);
			::SetTextColor(dc, color1);
			rc = _rect;
			::DrawText(dc, str, -1, &rc, drawStyle);
			::SetTextColor(dc, oldColor);
		}
		return true;
	}

	/**
	 * [�\��] �͈͕����\��.
	 *	@param[in] dc �f�o�C�X�R���e�L�X�g
	 *	@param[in] rect �\���͈́B
	 *	@param[in] drawStyle DrawText�X�^�C��
	 *	@param[in] offset ����1�ƕ����Q�̂���
	 *	@param[in] color1 �����F�P���w�肵�܂��B CLR_INVALID ���w�肷��ƁA �x�̂���O���[�̕����ɂȂ�܂��B
	 *					CLR_AUTOSELECT ���w�肷��ƁA���݂̃e�L�X�g�J���[���g���܂��B
	 *	@param[in] color2 �����F�Q���w�肵�܂��B
	 *	@param[in] str �\������
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool DrawTextRect(HDC dc, const RECT& rect, DWORD drawStyle, const POINT& offset, COLORREF color1, COLORREF color2, LPCTSTR str)
	{
		RECT rc = rect;
		return DrawTextRect(rc, dc, drawStyle, offset, color1, color2, str);
	}

	/**
	 * [�\��] �͈͕����\��.
	 *	@param[in,out] _rect �\���͈́B�߂�ŁA�\�������͈͂��ݒ肳��܂��B
	 *	@param[in] dc �f�o�C�X�R���e�L�X�g
	 *	@param[in] drawStyle DrawText�X�^�C��
	 *	@param[in] color �����F�P���w�肵�܂��B CLR_INVALID ���w�肷��ƁA �x�̂���O���[�̕����ɂȂ�܂��B
	 *					CLR_AUTOSELECT ���w�肷��ƁA���݂̃e�L�X�g�J���[���g���܂��B
	 *	@param[in] str �\������
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool DrawTextRect(RECT& _rect, HDC dc, DWORD drawStyle, COLORREF color, LPCTSTR str)
	{
		POINT po = { 0, 0 };
		return DrawTextRect(_rect, dc, drawStyle, po, color, CLR_INVALID, str);
	}

	/**
	 * [�\��] �͈͕����\��.
	 *	@param[in] dc �f�o�C�X�R���e�L�X�g
	 *	@param[in] rect �\���͈́B
	 *	@param[in] drawStyle DrawText�X�^�C��
	 *	@param[in] color �����F�P���w�肵�܂��B CLR_INVALID ���w�肷��ƁA �x�̂���O���[�̕����ɂȂ�܂��B
	 *					CLR_AUTOSELECT ���w�肷��ƁA���݂̃e�L�X�g�J���[���g���܂��B
	 *	@param[in] str �\������
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool DrawTextRect(HDC dc, const RECT& rect, DWORD drawStyle, COLORREF color, LPCTSTR str)
	{
		POINT po = { 0, 0 };
		RECT rc = rect;
		return DrawTextRect(rc, dc, drawStyle, po, color, CLR_INVALID, str);
	}

	#ifndef _WIN32_WCE

	/**
	 * [�v�Z] ������ҏW.
	 *		�\�����Ɏ��܂�悤�ɁA�������ҏW���܂��B
	 *	@param[out] _str �ҏW���ʂ��i�[�����B
	 *	@param[in] size �\���T�C�Y�B
	 *	@param[in] dc �f�o�C�X�R���e�L�X�g�B
	 *	@param[in] lpsz �\��������B
	 *	@param[in] drawStyle DrawText�X�^�C���B�ʏ�A DT_END_ELLIPSIS �� DT_PATH_ELLIPSIS ���w�肵�܂��B�ȗ�����ƁA DT_END_ELLIPSIS | DT_NOPREFIX �ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool ModifyText(CSimpleStr& _str, const SIZE& size, HDC dc, LPCTSTR lpsz, UINT drawStyle = DT_END_ELLIPSIS | DT_NOPREFIX)
	{
		size_t len = STRLIB::GetLen(lpsz);
		CWorkMemT<TCHAR> work(len + 10);
		STRLIB::Copy(work, lpsz);
		RECT rc = { 0, 0, size.cx, size.cy };
		int r = ::DrawTextEx(dc, work, -1, &rc, (drawStyle & 0xFFFFFFF0) | DT_MODIFYSTRING | DT_CALCRECT, NULL);
		if ( r != 0 )
		{
			_str = work;
		}
		return r != 0;
	}

	/**
	 * [�v�Z] ������ҏW.
	 *		�\�����Ɏ��܂�悤�ɁA�������ҏW���܂��B
	 *	@param[out] _str �ҏW���ʂ��i�[�����B
	 *	@param[in] hWnd �E�B���h�E�n���h���B��������A�T�C�Y�ƃt�H���g�𓾂܂��B
	 *	@param[in] lpsz �\��������B
	 *	@param[in] drawStyle DrawText�X�^�C���B�ʏ�A DT_END_ELLIPSIS �� DT_PATH_ELLIPSIS ���w�肵�܂��B�ȗ�����ƁA DT_END_ELLIPSIS | DT_NOPREFIX �ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool ModifyText(CSimpleStr& _str, HWND hWnd, LPCTSTR lpsz, UINT drawStyle = DT_END_ELLIPSIS | DT_NOPREFIX)
	{
		HDC dc = ::GetDC(hWnd);
		if ( dc != NULL )
		{
//			HGDIOBJ hFont = reinterpret_cast<HGDIOBJ>(::SendMessage(hWnd, WM_GETFONT, 0, 0));
			HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hWnd, WM_GETFONT, 0, 0));
			CDcSelectAssistant dca(dc);
//			HGDIOBJ hOld = ::SelectObject(dc, hFont);
			dca.SelectFont(hFont);
			RECT rc;
			::GetClientRect(hWnd, &rc);
			SIZE size = { rc.right - rc.left, rc.bottom - rc.top };
			bool r = ModifyText(_str, size, dc, lpsz, drawStyle);
//			::SelectObject(dc, hOld);
			dca.Restore();
			::ReleaseDC(hWnd, dc);
			return r;
		}
		return false;
	}

	#endif // _WIN32_WCE

protected:
	HFONT			m_font;			///< �t�H���g
	COLORREF		m_textColor;	///< �e�L�X�g�J���[
	CSimpleStr		m_str;			///< ������
	DWORD			m_drawStyle;	///< DrawText�X�^�C��
	int				m_margin;		///< ���}�[�W��

private:
	/// DrawTextRect �p�A�R�[���o�b�N
	static BOOL CALLBACK ms_DrawStateProc(HDC hDC, LPARAM lParam, WPARAM wParam, int cx, int cy)
	{
		LPCTSTR str = reinterpret_cast<LPCTSTR>(lParam);
		DWORD drawStyle = static_cast<DWORD>(wParam);
		RECT rc = { 0, 0, cx, cy };
		::DrawText(hDC, str, -1, &rc, drawStyle);
		return TRUE;
	}
};



/**@ingroup DRAWABLE
 * �e�L�X�g�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�e�L�X�g��w�i�F�t�ŕ`�悵�܂��B
 *
*	@par�K�v�t�@�C��
 *			TnbTextDrawer.h
 *
 *	@date 07/12/12 �V�K�쐬
 */
class CTextBkDrawer : public CTextDrawer
{
	DEFSUPER(CTextDrawer);
	COLORREF m_backColor;	///< �w�i�F
public:
	/**
	 * �R���X�g���N�^
	 *	@param size	�T�C�Y
	 *	@param font	�t�H���g
	 *	@param textColor �����J���[
	 *	@param backColor �w�i�J���[
	 *	@param str ������
	 *	@param style DrawText�X�^�C���w��B	�ȗ������ �uDT_CENTER | DT_VCENTER�v�B
	 *	@param margin ���̃}�[�W�B�ȗ�����ƂO�ɂȂ�܂��B
	 */
	CTextBkDrawer(const SIZE& size, HFONT font, COLORREF textColor, COLORREF backColor, LPCTSTR str, DWORD style = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(size, font, textColor, str, style, margin), m_backColor(backColor)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param font	�t�H���g
	 *	@param textColor �����J���[
	 *	@param backColor �w�i�J���[
	 *	@param str ������
	 *	@param style DrawText�X�^�C���w��B	�ȗ������ �uDT_CENTER | DT_VCENTER�v�B
	 *	@param margin ���̃}�[�W�B�ȗ�����ƂO�ɂȂ�܂��B
	 */
	CTextBkDrawer(HFONT font, COLORREF textColor, COLORREF backColor, LPCTSTR str, DWORD style = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(font, textColor, str, style, margin), m_backColor(backColor)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CTextBkDrawer(m_size, m_font, m_textColor, m_backColor, m_str, m_drawStyle, m_margin);
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
		RECT rc = { x + m_margin, y, x + m_size.cx, y + m_size.cy };
		CDcSelectAssistant dca(dc);
//		COLORREF oldBackColor = ::SetBkColor(dc, m_backColor);
		dca.SetBkColor(m_backColor);
		::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
		if ( m_font != NULL )
		{
//			HGDIOBJ oldObj = ::SelectObject(dc, m_font);
			dca.SelectFont(m_font);
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
//			::SelectObject(dc, oldObj);
		}
		else
		{
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
		}
//		::SetBkColor(dc, oldBackColor);
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *	@param color �J���[
	 */
	void SetBackColor(COLORREF color)
	{
		m_backColor = color;
	}
};



}; //TNB


