#pragma once
/**
 *@file
 * �`����HeaderCtrl�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbVector.h"
#include "TnbBitmapImage.h"
#include "TnbTextDrawer.h"
#include "TnbPartsDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����HeaderCtrl�R���g���[��
 *
 *		�J�����P�ʂŕ\������`������w��o���܂��B
 *
 *		�J�����̔w�i�� SetBackground() �Ŏw�肵�܂��B�f�t�H���g�ł́A�W���I�ȃ{�^���̂悤�ȕ\���ɂȂ�܂��B
 *		�e�J�����̃A�C�e���́A���̔w�i�̒��ɕ\�������悤�ɂȂ�܂��B
 *
 *	@note CDrawingListCtrl �ɂē����Ă��܂��B CDrawingListCtrl::GetHeaderCtrl() �ŎQ�Ɖ\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingHeaderCtrl.h
 * 
 *	@date 07/12/07	�V�K�쐬
 *	@date 10/01/06	�J�����T�C�Y�ύX�֎~�@�\��t���B
 *	@date 11/10/14	�e�A�C�e���̏�ԃ`�F�b�N�ɃI�[�i�[�h���[�ł͂Ȃ��J�X�^���h���[���g���悤�ɂ����B Reset() ���\�b�h�̃_�~�[���B
 */
class CDrawingHeaderCtrl : public CHeaderCtrl
{
	DEFSUPER(CHeaderCtrl);
public:

	/// �R���X�g���N�^
	CDrawingHeaderCtrl(void) : m_isStateChecking(false), m_isAutoAdjustWidth(true)
		, m_textColor(::GetSysColor(COLOR_WINDOWTEXT)), m_backColor(CLR_INVALID), m_isEnableChangeWidth(true)
	{
		m_pPartsDraw = &m_standardParts;
	}

	/**
	 * [�ݒ�] �e�L�X�g�J���[�ݒ�.
	 *	@note �e�J�����̕`�����ݒ肵�Ă��Ȃ��ꍇ�A�ݒ肳��Ă���t�H���g�Ɛݒ肳��Ă��镶����\�����܂��B
	 *			���̍ێg����F�ł��B
	 *	@param color �e�L�X�g�J���[�B
	 */
	void SetTextColor(COLORREF color)
	{
		m_textColor = color;
	}

	/**
	 * [�ݒ�] �w�i�ݒ�
	 *	@param pParts �J�����g�\���B IPartsDrawable::DrawButtonControl() �� DFCS_BUTTONPUSH ���g�p���܂��B
	 *	@param color �w�i�F�B�J�����̂Ȃ��Ƃ����h��F���w�肵�܂��B�ȗ�����ƁA pParts �ŕ`�悵�܂�
	 */
	void SetBackground(const IPartsDrawable* pParts, COLORREF color = CLR_INVALID)
	{
		m_pPartsDraw = pParts;
		m_backColor = color;
		if ( m_backBrush.GetSafeHandle() != NULL )
		{
			m_backBrush.DeleteObject();
		}
	}

	/**
	 * [�ݒ�] �J�����`����ݒ�
	 *	@param col �J�����i���o�[�B
	 *	@param draw �`����B�J�����g�̒��ɔ[�܂�悤�ɂ��Ă��������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetColumnDrawer(int col, const IDrawable& draw)
	{
		if ( col >=0 && _super::GetItemCount() > col )
		{
			return m_columnDrawers.Set(col, draw.Clone());
		}
		return false;
	}

	/**
	 * [�ݒ�] �������������[�h�ݒ�.
	 *	@note �f�t�H���g�ł� true �ł��B
	 *	@param b true ����ƁA�����N���C�A���g�Ɠ����ɂȂ�悤�ɒ������܂�(���X�N���[���o�[���o�Ȃ�)�B\n
	 */
	void SetAdjustWidthMode(bool b = true)
	{
		m_isAutoAdjustWidth = b;
	}

	/**
	 * [�擾] �������������[�h.
	 *	@retval true �����N���C�A���g�Ɠ����ɂȂ�悤�ɒ������܂�(���X�N���[���o�[���o�Ȃ�)�B
	 *	@retval false ���������Ȃ�
	 */
	bool IsAdjustWidthMode(void) const
	{
		return m_isAutoAdjustWidth;
	}

	/**
	 * [�ݒ�] ���ύX���[�h�ݒ�.
	 *	@param b true ���w�肷��ƁA�}�E�X�ɂ��e�J�����̕��𒲐��ł��܂��B false �Ȃ�A������֎~���܂��B
	 */
	void EnableChangeWidth(bool b = true)
	{
		m_isEnableChangeWidth = b;
	}

	/**
	 * [�m�F] �R���g���[�������݂��邩�H
	 *	@retval true ����
	 *	@retval false �Ȃ�(�N���G�C�g or �A�^�b�`����Ă��Ȃ�)
	 */
	bool IsWindow(void) const
	{
		return !! ::IsWindow(_super::GetSafeHwnd());
	}

	/**
	 * [�擾] �J�������擾.
	 *	@note �R���g���[�������݂��Ȃ��ꍇ�A�P��Ԃ��܂��B
	 *	@return ���B
	 */
	int GetItemCount() const
	{
		return IsWindow() ? _super::GetItemCount() : 1;
	}

	/**
	 * [�ݒ�] �ݒ�.
	 *	@deprecated �p�~�\��̃��\�b�h�ł��B
	 */
	void _deprecated Reset(void)
	{
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		if ( (GetStyle() & HDS_BUTTONS) == 0 )
		{
			m_isEnableChangeWidth = false;
		}
		_super::PreSubclassWindow();
	}

	/**
	 * [�ʒm] for notifications from parent
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE	�����ρB
	 *	@retval FALSE	�������B
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_NOTIFY )
		{
			LPNMHDR lpNmHdr = reinterpret_cast<LPNMHDR>(lParam);
			if ( lpNmHdr->code == NM_CUSTOMDRAW ) 
			{
				LPNMLVCUSTOMDRAW P = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
				*_pResult = CDRF_DODEFAULT;
				switch ( P->nmcd.dwDrawStage )
				{
				case CDDS_PREPAINT:
					*_pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
					break;
				case CDDS_ITEMPREPAINT:
					m_columnStates.Set(P->nmcd.dwItemSpec, P->nmcd.uItemState);
					break;
				}
				return TRUE;
			}
		} 
		return FALSE;
	}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *		ListBox �֌W�̃��b�Z�[�W���t�b�N���Ă��܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_PAINT:
			m_Draw(CPaintDC(this));
			return 0;
			break;
		case WM_PRINTCLIENT:
			if ( ! m_isStateChecking )
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				m_Draw(dc);
				return 0;
			}
			break;
		case WM_ERASEBKGND:
			if ( ! m_isStateChecking )
			{
				return 0;
			}
			break;
		case WM_MOUSEMOVE:
		case WM_SETCURSOR:
		case WM_LBUTTONDBLCLK:
			if ( ! m_isEnableChangeWidth )
			{
				return 0;
			}
			break;
		case HDM_DELETEITEM:
			{
				INDEX col = wParam;
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				if ( r != 0 )
				{
					m_columnDrawers.Remove(col);
				}
				return r;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// �`��
	void m_Draw(HDC dc)
	{
		CRect clientRect;
		_super::GetClientRect(&clientRect);
		// ��ԃ`�F�b�N����
		m_isStateChecking = true;
		CBitmapImage orgImg;
		orgImg.SetFromClient(*this, clientRect, CLR_AUTOSELECT); //WM_PRINTCLIENT �ŃJ�X�^���h���[�𓮂����A��Ԃ��E��
		m_isStateChecking = false;
		// �A�C�e���擾�I�u�W�F�N�g�p��
		HDITEM hdi;
		enum { sizeOfBuffer = 256 };
		TCHAR lpBuffer[sizeOfBuffer];
		hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = sizeOfBuffer;
		// �e�l�p��
		CPoint pushOffset;
		m_pPartsDraw->GetPushedOffset(pushOffset);
		CPoint edgeOffset;
		m_pPartsDraw->GetEdgeOffset(edgeOffset);
		int right = clientRect.right;
		CRect parentRect;
		bool isEnabled = !! _super::IsWindowEnabled();
		CWnd* pParent = _super::GetParent();
		if ( pParent != NULL )
		{
			pParent->GetClientRect(&parentRect);
			right = parentRect.right;
			isEnabled = !! pParent->IsWindowEnabled();
		}
		// ����������
		if ( m_isAutoAdjustWidth )
		{
			CRect rc = clientRect;
			int cnt = _super::GetItemCount();
			for ( int i = 0; i < cnt; i++ )
			{
				if ( ! _super::GetItem(i, &hdi) )
				{
					continue;
				}
				rc.right = rc.left + hdi.cxy;
				bool r = (rc.right > right);
				if ( i + 1 == cnt )
				{
					r = (rc.right != right);
				}
				if ( r )
				{
					HDITEM h;
					h.mask = HDI_WIDTH;
					h.cxy = right - rc.left;
					if ( h.cxy < 0 )
					{
						h.cxy = 0;
					}
					_super::SetItem(i, &h);
					rc.right = right;
				}
				rc.left = rc.right;
			}
		}
		// �e�A�C�e���`��
		CRect rc = clientRect;
		loop ( i, _super::GetItemCount() )
		{
			if ( ! _super::GetItem(ToInt(i), &hdi) )
			{
				continue;
			}
			rc.right = rc.left + hdi.cxy;
			//
			CPoint offset = edgeOffset;
			UINT state = DFCS_BUTTONPUSH | DFCS_ADJUSTRECT;
			if ( (m_columnStates[i] & CDIS_SELECTED) != 0 )
			{
				state |= DFCS_PUSHED;
				offset += pushOffset;
			}
			if ( ! isEnabled )
			{
				state |= DFCS_INACTIVE;
			}
			if ( (GetStyle() & HDS_BUTTONS) == 0 )
			{
				state |= DFCS_FLAT;
			}
			CRect r = rc;
			::FillRect(dc, &r, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
			m_pPartsDraw->DrawButtonControl(dc, &r, state);
			//
			HRGN hRgn = ::CreateRectRgnIndirect(&r);
			::SelectClipRgn(dc, hRgn);
			CSize clientSize = rc.Size() - edgeOffset - edgeOffset;
			IDrawable* pDr = m_columnDrawers[i];
			if ( pDr != NULL )
			{
				pDr->Resize(clientSize);
				pDr->Draw(dc, rc.left + offset.x, rc.top + offset.y);
			}
			else
			{
				CFont* pFont = _super::GetFont();
				UINT style = DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS;
				if ( (hdi.fmt & HDF_CENTER) != 0 )
				{
					style |= DT_CENTER;
				}
				else if ( (hdi.fmt & HDF_RIGHT) != 0 )
				{
					style |= DT_RIGHT;
				}
				COLORREF c = isEnabled ? m_textColor : CLR_INVALID;
				CTextDrawer d(clientSize, *pFont, c, lpBuffer, style);
				d.Draw(dc, rc.left + offset.x, rc.top + offset.y);
			}
			rc.left = rc.right;
			::SelectClipRgn(dc, NULL);
			_DeleteObject(hRgn);
		}
		// �]���`��
		if ( rc.right < right )
		{
			rc.right = right;
			if ( IS_RGBVALUE(m_backColor) )
			{
				if ( m_backBrush.GetSafeHandle() == NULL )
				{
					m_backBrush.CreateSolidBrush(m_backColor);
				}
				::FillRect(dc, rc, m_backBrush);
			}
			else
			{
				UINT state = DFCS_BUTTONPUSH;
				if ( ! _super::IsWindowEnabled() )
				{
					state |= DFCS_INACTIVE;
				}
				rc.right += 10; // �E�̕Ђ�\�������Ȃ�����
				m_pPartsDraw->DrawButtonControl(dc, &rc, state);
			}
		}
	}

	const IPartsDrawable*			m_pPartsDraw;			///< �J�����w�b�_�`��
	COLORREF						m_backColor;			///< �w�i�F
	CBrush							m_backBrush;			///< �w�i�u���V
	COLORREF						m_textColor;			///< �����F
	CAutoVectorT<IDrawable::Ptr>	m_columnDrawers;		///< �`����̔z��
	CAutoVectorT<UINT>				m_columnStates;			///< ��ԏ��̔z��
	CStandardPartsDrawer			m_standardParts;		///< �W���p�[�c�`��
	bool							m_isStateChecking;		///< ��ԃ`�F�b�N��
	bool							m_isAutoAdjustWidth;	///< ���������߃��[�h
	bool							m_isEnableChangeWidth;	///< ���ύX�\���[�h 
};



}; // MFC
}; // TNB
