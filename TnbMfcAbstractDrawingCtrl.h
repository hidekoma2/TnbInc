#pragma once
/**
 *@file
 * �`����R���g���[�����ۃN���X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbSelectedBeltDrawer.h"
#include "TnbBitmapDrawer.h"
#include "TnbSimpleVector.h"
#include "TnbHandleHandle.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����R���g���[�����ۃN���X
 *
 *		�I���}�[�N��\������R���g���[���̍쐬���T�|�[�g����@�\�������Ă��܂��B\n
 *		�܂��A�w�i�ƃe�L�X�g�`��@�\���T�|�[�g���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcAbstractDrawingCtrl.h
 * 
 *	@date 07/12/07	�V�K�쐬�B
 *	@date 08/08/25	�N���X���ύX�B�T�uWnd�Ǘ��@�\�ǉ��B
 *	@date 10/02/26	�g�p����`�惁�\�b�h�� IDrawable::Draw() ���� IDrawable::DrawEx() �ɕύX�B GetTextDrawStyle() �ǉ��B
 *	@date 11/08/04	SetDisabledStyleDrawer() , DrawDisabledStyle() �ǉ��B
 */
class CAbstractDrawingCtrl
{
public:
	/// �Z���N�g�}�[�N�^�C�v
	enum ESelectMarkType
	{
		BACKGROUND,			///< �w�i�ɕ`��
		OVERLAP,			///< �A�C�e���`���ɕ`��
	};


	//---------------------


	/**
	 * [�ݒ�] �f�t�H���g�̃}�[�N�`��w��.
	 *		�t�H�[�J�X�}�[�N�A�I���}�[�N��Window�W���Ɠ����l�ɂ��܂��B
	 *	@param base �x�[�X�̐F�B�ȗ������ Windows�W���Ɠ����l�ɂ��܂��B
	 */
	void SetDefaultMarkDrawer(COLORREF base = ::GetSysColor(COLOR_WINDOW))
	{
		m_pBackgroundDrawer = new CBoxFillDrawer(base);
		m_pSelectMarkDrawer = new CSelectedBeltDrawer(::GetSysColor(COLOR_HIGHLIGHT), base, true);
		m_pNoActiveSelectMarkDrawer = new CSelectedBeltDrawer(::GetSysColor(COLOR_BTNFACE), base, true);
		m_pFocusMarkDrawer = new CFocusFrameDrawer();
		{
			int c1 = ::GetSysColor(COLOR_WINDOW);
			int c2 = ::GetSysColor(COLOR_BTNFACE);
			int r1 = GetRValue(c1);
			int r2 = GetRValue(c2);
			int r = r2 - (r1 - r2);
			r = (r < 0) ? 0 : r;	
			int g1 = GetGValue(c1);
			int g2 = GetGValue(c2);
			int g = g2 - (g1 - g2);
			g = (g < 0) ? 0 : g;	
			int b1 = GetBValue(c1);
			int b2 = GetBValue(c2);
			int b = b2 - (b1 - b2);
			b = (b < 0) ? 0 : b;	
			m_pDisabledStyleDrawer = new CSemitransparentBoxFillDrawer(RGB(r, g, b), 50);
		}
		m_selectMarkType = OVERLAP;
		m_pTextDrawer.Null();
		SetBackColor(base);
	}

	/**
	 * [�ݒ�] �w�i�`��w��.
	 *	@param draw Resize() ���L���Ȋe�A�C�e���̔w�i�`����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetBackgroundDrawer(const IDrawable& draw)
	{
		return m_SetDrawer(m_pBackgroundDrawer, draw);
	}

	/**
	 * [�ݒ�] �I���}�[�N�`��w��.
	 *	@note �A�N�e�B�u���Ɣ�A�N�e�B�u���œ����I���}�[�N���g���܂��B
	 *	@param draw Resize() ���L���ȑI���}�[�N�̕`����B
	 *	@param type �I���}�[�N�̃^�C�v�B�ȗ�����ƁA OVERLAP �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw, ESelectMarkType type = OVERLAP)
	{
		m_selectMarkType = type;
		bool r = m_SetDrawer(m_pSelectMarkDrawer, draw);
		if ( r )
		{
			m_pNoActiveSelectMarkDrawer = m_pSelectMarkDrawer;
		}
		return r;
	}
	
	/**
	 * [�ݒ�] �I���}�[�N�`��w��.
	 *	@param draw1 Resize() ���L���ȃA�N�e�B�u���̑I���}�[�N�̕`����B
	 *	@param draw2 Resize() ���L���Ȕ�A�N�e�B�u���̑I���}�[�N�̕`����B
	 *	@param type �I���}�[�N�̃^�C�v�B�ȗ�����ƁA OVERLAP �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw1, const IDrawable& draw2, ESelectMarkType type = OVERLAP)
	{
		if ( ! SetSelectMarkDrawer(draw1, type) ) { return false; }
		return m_SetDrawer(m_pNoActiveSelectMarkDrawer, draw2);
	}
	
	/**
	 * [�ݒ�] �t�H�[�J�X�}�[�N�`��w��.
	 *	@param draw Resize() ���L���ȃt�H�[�J�X�}�[�N�̕`����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetFocusMarkDrawer(const IDrawable& draw)
	{
		return m_SetDrawer(m_pFocusMarkDrawer, draw);
	}

	/**
	 * [�ݒ�] ������ԕ`��w��.
	 *	@note �S�ĕ`���ɂ��̕`���񂪎g�p����܂��B
	 *	@param draw Resize() ���L���Ȗ�����ԕ`����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetDisabledStyleDrawer(const IDrawable& draw)
	{
		return m_SetDrawer(m_pDisabledStyleDrawer, draw);
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *	@param color �F
	 */
	void SetBackColor(COLORREF color)
	{
		m_backColor = color;
		if ( color == CLR_INVALID )
		{
			m_backColor = ::GetSysColor(COLOR_WINDOW);
		}
		m_backColorBrush.Null();
	}

	/**
	 * [�ݒ�] �e�L�X�g�`����w��
	 *	@note �w�肵�Ȃ��ꍇ�ł��A�W�������̕`�悪�s���܂��B
	 *	@param text �e�L�X�g�`��
	 */
	void SetTextDrawer(const CTextDrawer& text)
	{
		m_pTextDrawer = static_cast<CTextDrawer*>(text.Clone());
	}

	/**
	 * [�ǉ�] �֘AWindow�n���h���ǉ�
	 *	@note �R���g���[���Ɗ֘A����Window�n���h����o�^���Ă������ƂŁA�\����\�����T�|�[�g�ł��܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 */
	void AdditionRelatedHwnd(HWND hWnd)
	{
		m_wndList.Add(hWnd);
	}

	/**
	 * [����] �֘AWindow�n���h���փ|�X�g
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 */
	void PostMessageToRelatedHwnd(UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		loop ( i, m_wndList.GetSize() )
		{
			::PostMessage(m_wndList[i], message, wParam, lParam);
		}
	}

protected:

	/// �R���X�g���N�^
	CAbstractDrawingCtrl(void) : m_selectMarkType(OVERLAP), m_backColor(::GetSysColor(COLOR_WINDOW))
	{
	}

	/// �f�X�g���N�^
	virtual ~CAbstractDrawingCtrl(void)
	{
	}

	/**
	 * [�擾] �Z���N�g�}�[�N�^�C�v
	 *	@return �^�C�v
	 */
	ESelectMarkType	GetSelectMarkType(void) const
	{
		return m_selectMarkType;
	}

	/**
	 * [�擾] �w�i�F�擾.
	 *	@return �F
	 */
	COLORREF GetBackColor(void) const
	{
		return m_backColor;
	}

	/**
	 * [�`��] �I���}�[�N�\��
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �\���ʒu�B
	 *	@param isActive true �Ȃ�A�N�e�B�u�A false �Ȃ�m���A�N�e�B�u�̑I���}�[�N�������܂��B
	 *	@param lParam IDrawable::DrawEx() �� LPARAM �̒l
	 */
	void DrawSelectMark(HDC dc, const RECT& rect, bool isActive, LPARAM lParam = 0)
	{
		if ( isActive || m_pNoActiveSelectMarkDrawer.IsNull() )
		{
			m_DrawDrawer(m_pSelectMarkDrawer, dc, rect, lParam);
		}
		else
		{
			m_DrawDrawer(m_pNoActiveSelectMarkDrawer, dc, rect, lParam);
		}
	}

	/**
	 * [�`��] �w�i�\��
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �\���ʒu�B
	 *	@param lParam IDrawable::DrawEx() �� LPARAM �̒l
	 */
	void DrawBackground(HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		m_DrawDrawer(m_pBackgroundDrawer, dc, rect, lParam);
	}

	/**
	 * [�`��] �t�H�[�J�X�}�[�N�\��
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �\���ʒu�B
	 *	@param lParam IDrawable::DrawEx() �� LPARAM �̒l
	 */
	void DrawFocusMark(HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		m_DrawDrawer(m_pFocusMarkDrawer, dc, rect, lParam);
	}

	/**
	 * [�`��] ������ԕ`��.
	 *	�Ō�Ɏg�p����܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �\���ʒu�B
	 *	@param lParam IDrawable::DrawEx() �� LPARAM �̒l
	 */
	void DrawDisabledStyle(HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		m_DrawDrawer(m_pDisabledStyleDrawer, dc, rect, lParam);
	}

	/**
	 * [�`��] �w�i�F�`��
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �\���ʒu�B
	 */
	void DrawBackColor(HDC dc, const RECT& rect)
	{
		if ( (rect.bottom - rect.top) > 0 )
		{
			if ( m_backColorBrush.IsNull() )
			{
				m_backColorBrush = ::CreateSolidBrush(m_backColor);
			}
			::FillRect(dc, &rect, m_backColorBrush);
		}
	}

	/**
	 * [�`��] ������`��
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �\���ʒu�B
	 *	@param lpsz ������
	 *	@param lParam IDrawable::DrawEx() �� LPARAM �̒l
	 */
	void DrawText(HDC dc, const RECT& rect, LPCTSTR lpsz, LPARAM lParam = 0)
	{
		if ( ! m_pTextDrawer.IsNull() )
		{
			CSize sz = CRect(rect).Size();
			m_pTextDrawer->Resize(sz - CSize(2, 0));
			m_pTextDrawer->SetString(lpsz);
			m_pTextDrawer->DrawEx(dc, rect.left + 1, rect.top, lParam);
		}
	}

	/**
	 * [�`��] ������`��
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �\���ʒu�B
	 *	@param lpsz ������
	 *	@param drawStyle DrawText�X�^�C���w��B
	 *	@param lParam IDrawable::DrawEx() �� LPARAM �̒l
	 */
	void DrawText(HDC dc, const RECT& rect, LPCTSTR lpsz, DWORD drawStyle, LPARAM lParam = 0)
	{
		if ( ! m_pTextDrawer.IsNull() )
		{
			CSize sz = CRect(rect).Size();
			m_pTextDrawer->SetDrawStyle(drawStyle);
			m_pTextDrawer->Resize(sz - CSize(2, 0));
			m_pTextDrawer->SetString(lpsz);
			m_pTextDrawer->DrawEx(dc, rect.left + 1, rect.top, lParam);
		}
	}

	/**
	 * [�擾] ������`��X�^�C���擾.
	 *	@return ������`��X�^�C��
	 */
	DWORD GetTextDrawStyle(void) const
	{
		if ( ! m_pTextDrawer.IsNull() )
		{
			return m_pTextDrawer->GetDrawStyle();
		}
		return 0;
	}

	/**
	 * [�ݒ�] �e�L�X�g�`����ݒ�.
	 *	@note �ݒ肳��Ă��Ȃ��ꍇ�f�t�H���g�̕`�����ݒ肵�܂��B
	 *	@param pWnd �E�B���h�E
	 */
	void SetDefaultTextDrawer(CWnd* pWnd)
	{
		if ( m_pTextDrawer.IsNull() )
		{
			CFont* pFont = pWnd->GetFont();
			m_pTextDrawer = new CTextDrawer(*pFont, CLR_AUTOSELECT, _T(""), DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		}
	}

	/**
	 * [����] �`��J�n.
	 *	@note ��A�̕`��̍ŏ��ɃR�[�����܂��B
	 */
	void BeginItemPaint(void) 
	{
		loop ( i, m_wndList.GetSize() )
		{
			HWND h = m_wndList[i];
			if ( ::IsWindow(h) )
			{
				MFCLIB::SetProcessingFlag(h, false);
			}
		}
	}

	/**
	 * [����] �`��I��.
	 *	@note ��A�̕`��̍Ō�ɃR�[�����܂��B
	 *	@retval true �ω�����B
	 *	@retval false �ω������B
	 */
	bool EndItemPaint(void) 
	{
		bool res = false;
		for ( INT_PTR i = m_wndList.GetSize() - 1; i >= 0; i-- )
		{
			HWND h = m_wndList[i];
			if ( ::IsWindow(h) )
			{
				BOOL r = MFCLIB::IsProcessingFlag(h);
				if ( r )
				{
					MFCLIB::SetProcessingFlag(h, false);
				}
				if ( ::IsWindowVisible(h) != r )
				{
					::ShowWindow(h, r ? SW_SHOW : SW_HIDE );
					res = true;
				}
			}
			else
			{
				m_wndList.Remove(i);
			}
		}
		return res;
	}

	/**
	 * [�ݒ�] �S�ݒ胊�Z�b�g.
	 *		Set�n���\�b�h�Őݒ肵�����e��S���j�����܂��B
	 */
	void AllReset(void)
	{
		m_selectMarkType = OVERLAP;
		m_pSelectMarkDrawer.Null();
		m_pNoActiveSelectMarkDrawer.Null();
		m_pFocusMarkDrawer.Null();
		m_pBackgroundDrawer.Null();
		m_pDisabledStyleDrawer.Null();
		//
		m_backColorBrush.Null();
		m_pTextDrawer.Null();
		m_backColor = ::GetSysColor(COLOR_WINDOW);
		//
		m_wndList.RemoveAll();
	}

private:
	/// Drawable�o�^
	bool m_SetDrawer(IDrawable::Ptr& _P, const IDrawable& draw)
	{
		_P = draw.Clone();
		if ( ! _P.IsNull() )
		{
			if ( ! _P->Resize(CSize(1, 1)) )
			{
				_P.Null();
			}
		}
		return ! _P.IsNull();
	}
	// �`��
	void m_DrawDrawer(IDrawable::Ptr& _pDrawer, HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		if ( ! _pDrawer.IsNull() )
		{
			const CRect& rc = rect;
			if ( ! rc.IsRectEmpty() )
			{
				_pDrawer->Resize(rc.Size());
				_pDrawer->DrawEx(dc, rc.left, rc.top, lParam);
			}
		}
	}
	ESelectMarkType		m_selectMarkType;				///< �Z���N�g�}�[�N�^�C�v
	IDrawable::Ptr		m_pSelectMarkDrawer;			///< �Z���N�g�}�[�N�`����(�T�C�Y�ύX�T�|�[�g�K�{)
	IDrawable::Ptr		m_pNoActiveSelectMarkDrawer;	///< �m���A�N�e�B�u�Z���N�g�}�[�N�`����(�T�C�Y�ύX�T�|�[�g�K�{)
	IDrawable::Ptr		m_pFocusMarkDrawer;				///< �t�H�[�J�X�}�[�N�`����(�T�C�Y�ύX�T�|�[�g�K�{)
	IDrawable::Ptr		m_pBackgroundDrawer;			///< �eItem�̔w�i(�T�C�Y�ύX�T�|�[�g�K�{)
	IDrawable::Ptr		m_pDisabledStyleDrawer;			///< �������
	//
	CBrushHandle					m_backColorBrush;	///< �w�i�u���V
	CPointerHandleT<CTextDrawer>	m_pTextDrawer;		///< �e�L�X�g�`��
	COLORREF						m_backColor;		///< �w�i�F�B
	//
	CSimpleVectorT<HWND>			m_wndList;			///< WndList
};



}; // MFC
}; // TNB
