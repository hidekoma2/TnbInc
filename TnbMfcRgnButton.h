#pragma once
/**
 *@file
 * ���R�^�r�b�g�}�b�v�{�^���֌W�̃w�b�_
 *
 *		�r�b�g�}�b�v�̌`��ɂ��킹���O���t�B�J���ȃ{�^����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcAbstractButton.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL BITMAP
 * ���R�^�r�b�g�}�b�v�{�^���R���g���[��
 *
 *		�r�b�g�}�b�v�̌`�ɍ��킹���{�^�����쐬���邱�Ƃ��o���܂��B
 *
 *		�{�^���Ɏg�p����x�[�X�r�b�g�}�b�v�ƂȂ�u�ʏ��ԁv��K���w�肷��K�v������܂�
 *		�i���́u�ʏ��ԁv�̃r�b�g�}�b�v�̌`�󂪁A�{�^���̌`��ɂȂ�܂��j�B
 *		���̃r�b�g�}�b�v�́A SetBitmap() �Ŏw�肷�邩�A
 *		���\�[�X�G�f�B�^�Ńs�N�`���[�R���g���[���̃v���p�e�B�ŁA�^�C�v�Ɂu�r�b�g�}�b�v�v��I�����A
 *		�C���[�W�Ńr�b�g�}�b�v�̑I�������Ă����܂�(��҂̏ꍇ���\�[�X�Ƀr�b�g�}�b�v�������K�v����)�B
 * 
 *		���ɁA�u������ԁv�u������ԁv�u�z�o�[��ԁv�ƃt�H�[�J�X�̃r�b�g�}�b�v���w�肷�邱�Ƃ�
 *		�o���܂��B 
 *		�t�H�[�J�X�̕\�����@�͂Q��ނ���A��́u�t�H�[�J�X��ԁv�Ƃ��āA���̏�ԂƓ����悤�ɕ\��������@�A
 *		������́A�u���̃{�^���̏�ɃI�[�o�[���b�v���ĕ\���v������@�ł��B�O�҂� SetFocusBitmap() �A
 *		��҂� SetFocusMarkBitmap() �Ŏw�肵�܂��i��Őݒ肵���ق����K�p����܂��j�B		
 * 
 *	@note	�{�^���̌`��́A�u�ʏ��ԁv�̃r�b�g�}�b�v�̌`��ō쐬����܂��B���̂��߁A���̏��
 *			�̃r�b�g�}�b�v���u�ʏ��ԁv�Ƒ傫�����قȂ�ꍇ�A�S���\������Ȃ��A���邢�͂��݂�
 *			�c��\��������܂��B
 *
 *	@note	SetValidRgn() ���g�p���邱�ƂŁA�l�p���r�b�g�}�b�v�ł��A�{�^���͈̔͂����R�^�ɂ��邱�Ƃ�
 *			�o���܂��B
 *
 *	@note �L���A�����A�\���A��\���́A MFC �� CButton �Ɠ����悤�ɑ���ł��܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^�����X�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton or CStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@see CAbstractButton
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcRgnButton.h
 * 
 *	@date 07/09/26 �V�K�쐬
 *	@date 07/10/10 �A�^�b�`��̃r�b�g�}�b�v�̕ύX���\�ɂ����B
 *	@date 07/10/11 �������̃t�H�[�J�X�}�[�N�\��OFFSET���w��ł���悤�ɂ����B
 *	@date 07/10/16 �t�H�[�J�X�\�����@����ޗp�ӂ����B
 *	@date 07/10/22 �c�����[�h��p�ӁB
 *	@date 07/10/24 SetValidRgn() �ǉ��B
 *	@date 07/10/26 �R�s�[�I�y���[�^�A�Ή��B
 *	@date 07/11/08 SetDefaultButtonStyle() �ǉ��B
 *	@date 08/01/17 WM_SETTEXT �ł��V���[�g�J�b�g��ݒ�ł���悤�ɑΉ��B
 *	@date 08/02/06 SetDefaultButtonStyle() ��e�Ɉړ��B
 */
class CRgnButton : public CAbstractButton
{
	DEFSUPER(CAbstractButton);
	CSize				m_size;				///< �m�[�}���r�b�g�}�b�v�̑傫��
	COLORREF			m_transColor;		///< ���ߐF
	HRGN				m_hRgnClip;			///< �r�b�g�}�b�v�̃N���b�v���[�W�������B
	HRGN				m_hRgnCtrl;			///< �r�b�g�}�b�v�̃R���g���[�����[�W�������B
	bool				m_boIsDoSetRgn;		///< true �̎��ASetWindowRgn����i�������p�j
	CBitmapImage		m_lastBitmap;		///< �Ō�ɏ������r�b�g�}�b�v�B
	UINT				m_bitmapChangeTime;	///< �r�b�g�}�b�v�̏����������ԁB
	CPoint				m_pushedOffset;		///< �������̃t�H�[�J�X�}�[�N�̈ړ��l�B
	enum { TIMERID_DRAW = 1 };
	/// HRGN �폜
	void m_DeleteRgn(HRGN& _hRgn)
	{
		if ( _hRgn != NULL )
		{
			_DeleteObject(_hRgn);
			_hRgn = NULL;
		}
	}
	void m_DeleteRgns(void)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			_super::SetWindowRgn(NULL, false);
		}
		m_DeleteRgn(m_hRgnClip);
		m_DeleteRgn(m_hRgnCtrl);
	}
	void m_SetRgn(HRGN& _hRgn, HRGN h)
	{
		m_DeleteRgn(_hRgn);
		_hRgn = ::CreateRectRgn(0, 0, 1, 1);
		::CombineRgn(_hRgn, h, NULL, RGN_COPY);
	}
	/// �ݒ�
	bool m_SetBaseBitmap(CBitmapHandle h, COLORREF color)
	{
		if ( h.GetSize(m_size) )
		{
			m_DeleteRgns();
			CBitmapImage bi;
			bi.Attach(h);
			m_hRgnClip = bi.CreateRgn(color);
			m_SetRgn(m_hRgnCtrl, m_hRgnClip);
			m_boIsDoSetRgn = true;
			_super::SetWindowPos(NULL, 0, 0, m_size.cx, m_size.cy, SWP_NOZORDER | SWP_NOMOVE);
			_super::SetWindowRgn(NULL, true);
			return true;
		}
		return false;
	}
	/// 
	void m_DrawBmp(CDC* pDC, CBitmapHandle hb)
	{
		if ( hb == NULL ) { return; }
		CPoint pos1(0, 0);
		CPoint pos2((_super::GetButtonState() == ES_Pushed) ? m_pushedOffset : pos1);
		OnDrawingButton(pos2, pDC, true);
		MFCLIB::SelectClipRgnOrg(pDC, m_hRgnClip);
//		pDC->DrawState(pos1, m_size, hb, DST_BITMAP);
		hb.Draw(*pDC, pos1.x, pos1.y);
		OnDrawingButton(pos2, pDC, false);
		if ( m_focusMaskColor != CLR_INVALID )
		{
			if ( _super::HasFocus() && ! m_focusBitmap.IsEmpty() )
			{
				const SIZE& s = m_focusBitmap.GetSize();
				HDC hdc = m_focusBitmap.GetDC();
				::TransparentBlt(*pDC, pos2.x, pos2.y, s.cx, s.cy, hdc, 0, 0, s.cx, s.cy, m_focusMaskColor);
				m_focusBitmap.ReleaseDC();
			}
		}
	}

protected:

	CBitmapHandle		m_bmpButtons[4];	///< �u�ʏ�v�u�z�o�[�v�u�����v[�����v��Ԃ�BITMAP.
	CBitmapImage		m_focusBitmap;		///< �t�H�[�J�X(�}�[�N)BITMAP
	COLORREF			m_focusMaskColor;	///< �t�H�[�J�X�}�[�N�̓��ߐF(-1�Ȃ�t�H�[�J�X�̓}�[�N�ł͂Ȃ�������)

	/**
	 * [�ʒm] �`�撆.
	 *		Bitmap�\���̑O��ɃR�[������܂��B
	 *	@param[in,out] _pos �\���ʒu�B
	 *	@param[in] pDC �`����CDC�B
	 *	@param[in] boIsFirst Bitmap�\���O�� true�A�\����� false �Œʒm����܂��B
	 */
	virtual void OnDrawingButton(CPoint& _pos, CDC* pDC, bool boIsFirst)
	{
	}

	/**
	 * [�ʒm] �`��
	 *	@param pDC �`����CDC�B
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		CBitmapHandle hb = m_bmpButtons[_super::GetButtonState()];
		if ( hb.IsNull() )
		{
			hb = m_bmpButtons[ES_Normal];
		}
		if ( m_focusMaskColor == CLR_INVALID )
		{
			//�t�H�[�J�X�́A�C���[�W
			if ( _super::GetButtonState() == ES_Normal && _super::HasFocus() && ! m_focusBitmap.IsEmpty() )
			{
				hb = m_focusBitmap.GetBitmapHandle();
			}
		}
		_super::KillTimer(TIMERID_DRAW);
		if ( m_lastBitmap.IsEmpty() )
		{
			m_lastBitmap.Attach(hb);
			m_DrawBmp(pDC, hb);
		}
		else if ( m_bitmapChangeTime == 0 || hb.IsNull() )
		{
			m_DrawBmp(pDC, hb);
		}
		else
		{
			CBitmapImage bi;
			VERIFYLIB( bi.Set(hb) );
//			VERIFYLIB( bi.InsertOnSemitransparect(0, 0, m_lastBitmap, 50) );//50%����
			VERIFYLIB( bi.InsertOnAlphaBlend(0, 0, m_lastBitmap, 50) );//50%����
			m_DrawBmp(pDC, bi.GetBitmapHandle());
			m_lastBitmap.Attach(hb);
			_super::SetTimer(TIMERID_DRAW, m_bitmapChangeTime, NULL);
		}
	}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == WM_PAINT )
		{
			if ( m_boIsDoSetRgn )
			{
				m_boIsDoSetRgn = false;
				_super::SetWindowRgn(m_hRgnCtrl, true);
				m_hRgnCtrl = NULL;	// �J����Win���s��
				_super::RedrawWindow();
				return 0;
			}
		}
		else if ( message == WM_SHOWWINDOW )
		{
			m_lastBitmap.Empty();
		}
		else if ( message == WM_DESTROY )
		{
			m_DeleteRgns();
		}
		else if ( message == WM_SETTEXT )
		{
			CString s = reinterpret_cast<LPCTSTR>(lParam);
			int f = s.Find('&');
			if ( f >= 0 && (f + 1) < s.GetLength() )
			{
				SetShortcutKey(s[f + 1]);
			}
		}
		else if ( message == WM_TIMER && wParam == TIMERID_DRAW )
		{
			KillTimer(TIMERID_DRAW);
			CClientDC dc(this);
			m_DrawBmp(&dc, m_lastBitmap.GetBitmapHandle());
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		HBITMAP hBase = reinterpret_cast<HBITMAP>(::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0L));// GetBitmap()
		if ( ! m_bmpButtons[ES_Normal].IsNull() )
		{
			if ( ! m_SetBaseBitmap(m_bmpButtons[ES_Normal], m_transColor) )
			{
				m_bmpButtons[ES_Normal].Null();
			}
		}
		if ( m_bmpButtons[ES_Normal].IsNull() )
		{
			CRect rect;
	 		_super::GetClientRect(rect);
			if ( hBase == NULL )
			{
				HDC dc = ::GetDC(NULL);
				hBase = ::CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
				::ReleaseDC(NULL, dc);
				m_bmpButtons[ES_Normal] = hBase;
			}
			else
			{
				m_bmpButtons[ES_Normal].SetClone(hBase);
			}
			VERIFYLIB( m_SetBaseBitmap(m_bmpButtons[ES_Normal], m_transColor) );
		}
		_super::PreSubclassWindow();
	}

public:

	/// �f�X�g���N�^
	virtual ~CRgnButton(void)
	{
		m_DeleteRgns();
	}

	/// �R���X�g���N�^
	CRgnButton(void) 
		: m_boIsDoSetRgn(false), m_hRgnClip(NULL), m_hRgnCtrl(NULL), m_pushedOffset(0, 0)
		, m_transColor(CLR_AUTOSELECT), m_focusMaskColor(CLR_INVALID), m_bitmapChangeTime(40)
	{
	}

	/**
	 * [����] ���R�s�[.
	 *		�ێ����Ă�����𕡐����܂��B
	 *	@note ��������̓R�s�[����܂���B
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CRgnButton& operator=(const CRgnButton& other)
	{
		_super::operator=(other);
		m_size = other.m_size;
		m_transColor = other.m_transColor;
		m_boIsDoSetRgn = other.m_boIsDoSetRgn;
		m_lastBitmap = other.m_lastBitmap;
		m_bitmapChangeTime = other.m_bitmapChangeTime;
		m_pushedOffset = other.m_pushedOffset;
		m_focusBitmap = other.m_focusBitmap;
		m_focusMaskColor = other.m_focusMaskColor;
		loop ( i, 4 )
		{
			m_bmpButtons[i] = other.m_bmpButtons[i];
		}
		m_SetRgn(m_hRgnClip, other.m_hRgnClip);
		if ( ! ::IsWindow(other) )
		{
			m_SetRgn(m_hRgnCtrl, other.m_hRgnCtrl);
		}
		else
		{
			other.GetWindowRgn(m_hRgnCtrl);
		}
		//
		if ( ::IsWindow(m_hWnd) ) 
		{
			m_SetBaseBitmap(m_bmpButtons[ES_Normal], m_transColor);
		}
		return *this;
	}

	/**
	 * [�ݒ�] �r�b�g�}�b�v�؂�ւ����Ԑݒ�.
	 *		��Ԃ��ς���ăr�b�g�}�b�v��ύX���鎞�A�f�t�H���g�ł́A�؂�ւ��O�̃r�b�g�}�b�v�ƁA
	 *		�؂�ւ���̃r�b�g�}�b�v��50�������̉摜�� 30ms �b�\������悤�ɂȂ��Ă��܂��i���̂���
	 *		�ӂ���Ɛ؂�ւ��悤�Ɍ����܂��j�B���̐؂�ւ����Ԃ�{���\�b�h�Ŏw�肷�邱�Ƃ��o���܂��B
	 *	@note �����摜�͈ꖇ�����p�ӂ��Ȃ����߁A100ms���炢������ł��B
	 *	@param ms �؂�ւ����ԁi�P�� ms�j�B
	 */
	void SetBitmapChangeTime(UINT ms)
	{
		m_bitmapChangeTime = ms;
	}

	/**
	 * [�ݒ�] �x�[�X�r�b�g�}�b�v�w��.
	 *		�{�^���̃x�[�X�ƂȂ�r�b�g�}�b�v���w�肵�܂��B���̃r�b�g�}�b�v�Ń{�^���̌`���m�肳��܂��B
	 *	@note �{���\�b�h���g��Ȃ��ꍇ�A���\�[�X�G�f�B�^�Œ������r�b�g�}�b�v���x�[�X�ɂ��܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param color ���ߐF�w��B�ȗ�����ƁA�r�b�g�}�b�v�̍����̐F�����ߐF�Ƃ��܂��B
	 *						CLR_INVALID ���w�肷��Ɠ��ߐF�Ȃ��ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�n���h���������ł��B
	 */
	bool SetBitmap(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		if ( bmp.IsNull() ) { return false; }
		m_bmpButtons[ES_Normal] = bmp;
		m_transColor = color;
		if ( ::IsWindow(m_hWnd) ) 
		{
			if ( ! m_SetBaseBitmap(bmp, color) )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [�ݒ�] ������ԃr�b�g�}�b�v�w��.
	 *		������Ԃ�\���r�b�g�}�b�v���w�肵�܂��B
	 *		�x�[�X�ƂȂ�r�b�g�}�b�v�Ɠ����傫���ɂ���K�v������܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	void SetPushedBitmap(CBitmapHandle bmp)
	{
		m_bmpButtons[ES_Pushed] = bmp;
	}

	/**
	 * [�ݒ�] ������ԃr�b�g�}�b�v�w��.
	 *		������Ԃ�\���r�b�g�}�b�v���w�肵�܂��B
	 *		�x�[�X�ƂȂ�r�b�g�}�b�v�Ɠ����傫���ɂ���K�v������܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	void SetDisableBitmap(CBitmapHandle bmp)
	{
		m_bmpButtons[ES_Disable] = bmp;
	}

	/**
	 * [�ݒ�] �z�o�[��ԃr�b�g�}�b�v�w��.
	 *		�z�o�[��Ԃ�\���r�b�g�}�b�v���w�肵�܂��B
	 *		�x�[�X�ƂȂ�r�b�g�}�b�v�Ɠ����傫���ɂ���K�v������܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	void SetHoverBitmap(CBitmapHandle bmp)
	{
		m_bmpButtons[ES_Hover] = bmp;
	}

	/**
	 * [�ݒ�] �t�H�[�J�X��ԃr�b�g�}�b�v�w��.
	 *		�t�H�[�J�X��Ԃ�\���r�b�g�}�b�v���w�肵�܂��B
	 *		�x�[�X�ƂȂ�r�b�g�}�b�v�Ɠ����傫���ɂ���K�v������܂��B
	 *	@note SetFocusMaskBitmap() ���g���Ɩ{���\�b�h�̐ݒ�͔j������܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	void SetFocusBitmap(CBitmapHandle bmp)
	{
		m_focusBitmap.Set(bmp);
		m_focusMaskColor = CLR_INVALID;
	}

	/**
	 * [�ݒ�] �t�H�[�J�X��ԃ}�[�N�r�b�g�}�b�v�w��.
	 *		�t�H�[�J�X��Ԃ�\���}�[�N�̃r�b�g�}�b�v���w�肵�܂��B
	 *		���̃r�b�g�}�b�v�̏�ɕ\���������̂ɂȂ�܂��B
	 *		SetPushedMarkOffset() �̐ݒ�̉e�����󂯂܂��B
	 *	@note SetFocusBitmap() ���g���Ɩ{���\�b�h�̐ݒ�͔j������܂��B
	 *	@param bmp �r�b�g�}�b�v�n���h���B�n���h���͕s�K�v�ɂȂ����玩���I�ɔj�����܂��B
	 *	@param color ���ߐF�w��B�ȗ�����ƁA�r�b�g�}�b�v�̍����̐F�𓧉ߐF�Ƃ��܂��B
	 */
	void SetFocusMarkBitmap(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		m_focusBitmap.Set(bmp);
		if ( ! IS_RGBVALUE(color) )
		{
			const SIZE& s = m_focusBitmap.GetSize();
			HDC dc = m_focusBitmap.GetDC();
			color = ::GetPixel(dc, 0, s.cy - 1);
			m_focusBitmap.ReleaseDC();
		}
		m_focusMaskColor = color;
	}

	/**
	 * [�ݒ�] �������\������ݒ�.
	 *		�������Ƀt�H�[�J�X�}�[�N�̂�����w�肷�邱�Ƃ��o���܂��B�f�t�H���g�ł́A�ړ��Ȃ��ł��i0,0�j�B
	 *	@param pos ����
	 */
	void SetPushedMarkOffset(const POINT& pos)
	{
		m_pushedOffset = pos;
	}

	/**
	 * [�ݒ�] �r�b�g�}�b�v�w��.
	 *		�u�ʏ��ԁv�u�z�o�[��ԁv�u������ԁv[������ԁv�u�t�H�[�J�X��ԁv��\���r�b�g�}�b�v�𓯎��Ɏw�肵�܂��B
	 *	@note �u�ʏ��ԁv�u�z�o�[��ԁv�u������ԁv�u������ԁv�u�t�H�[�J�X��ԁv�̊G�𓯂��傫���ŉ��ɕ��ׂ��r�b�g�}�b�v��
	 *			�w�肷�邱�ƂŁA�S�������؂�ɐݒ肷�邱�Ƃ��\�ł��B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param color ���ߐF�w��B�ȗ�����ƁA�r�b�g�}�b�v�̍����̐F�𓧉ߐF�Ƃ��܂��B
	 *						CLR_INVALID ���w�肷��Ɠ��ߐF�Ȃ��ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�n���h���������ł��B
	 */
	bool SetAllBitmaps(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		CBitmapImage baseImage;
		if ( baseImage.Set(bmp) )
		{
			CSize baseSize = baseImage.GetSize();
			bool r = true;
			int x = 0;
			int l = baseSize.cx / 5;
			CBitmapImage bi;
			for ( int i = 0; i < 5; i++ )
			{
				bi = baseImage.Cut(CRect(x, 0, x + l, baseSize.cy));
				if ( i == 0 )
				{
					r &= SetBitmap(bi, color);
				}
				else if ( i == 4 )
				{
					SetFocusBitmap(bi);
				}
				else
				{
					m_bmpButtons[i] = bi;
				}
				x += l;
			}
			return r;
		}
		return false;
	}

	/**
	 * [�ݒ�] �{�^���L���͈͎w��.
	 *		�{�^���̉��������̍s����͈͂��w�肷�邱�Ƃ��o���܂��B�`��͎l�p���Ȃ�܂��B
	 *	@note �g�p����ꍇ�́A�r�b�g�}�b�v�w��̍Ō�Ɏg�p���Ă��������B
	 *	@param h ���[�W�����n���h���B �n���h���͓����ŕ������܂��̂ŁA�j�����Ă��܂��܂���B
	 */
	void SetValidRgn(HRGN h)
	{
		m_DeleteRgn(m_hRgnClip);
		m_SetRgn(m_hRgnCtrl, h);
		m_boIsDoSetRgn = true;
		_super::RedrawWindow();
	}
};



}; //MFC
}; //TNB



