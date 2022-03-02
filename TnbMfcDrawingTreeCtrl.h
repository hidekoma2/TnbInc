#pragma once
/**
 *@file
 * �`����TreeCtrl�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcAbstractCustomDraw.h"
#include "TnbMfcAbstractDrawingCtrl.h"
#include "TnbMap.h"



//TNB Library
namespace TNB {
namespace MFC {



/// �`�F�b�N�{�b�N�X�̕ύX
#define TVC_CHECKCHANGED 0xFFFF



/**@ingroup MFCCONTROL DRAWABLE
 * �`����TreeCtrl�R���g���[��
 *
 *		�A�C�e���P�ʂŕ\������`������w��o���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ńc���[�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCTreeCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingTreeCtrl.h
 * 
 *	@date 08/09/11	�V�K�쐬
 *	@date 10/01/07	Destroy���̃}�[�N�֌W�̏������R��C���B
 */
class CDrawingTreeCtrl : public CTreeCtrl, public CAbstractDrawingCtrl, protected CAbstractCustomDraw
{
	DEFSUPER(CTreeCtrl);
	typedef CAbstractCustomDraw _custom; 
	typedef CAbstractDrawingCtrl _mark; 

	/// �I���}�[�N�`��
	void m_DrawSelectMaker(HDC dc, const RECT& rect, HTREEITEM h)
	{
		if ( (GetItemState(h, TVIS_SELECTED ) & TVIS_SELECTED) != 0  )
		{
			_mark::DrawSelectMark(dc, rect, ::GetFocus() == m_hWnd);
		}
	}

	/// �폜
	void m_DeleteItem(HTREEITEM h)
	{
		HTREEITEM hh = _super::GetChildItem(h);
		while ( hh != NULL )
		{
			m_DeleteItem(hh);
			hh = _super::GetNextSiblingItem(hh);
		}
		m_map.RemoveKey(h);
	}

	void m_NotifyParent(HTREEITEM h)
	{
		NMTREEVIEW nm = { 0 };
		nm.hdr.hwndFrom = *this;
		nm.hdr.idFrom = GetDlgCtrlID();
		nm.hdr.code = TVN_SELCHANGED;
		nm.action = TVC_CHECKCHANGED;
		nm.itemOld.hItem = h;
		nm.itemNew.hItem = h;
		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), reinterpret_cast<LPARAM>(&nm));
	}

protected:

	/**
	 * [�ʒm] �`��J�n.
	 *		��A�̕`��̊J�n��ʒm���܂��B
	 *	@note dc �͈ȉ��A OnItemPainting() OnItemPainted() OnEndItemPaint() �Ƒ�����A�̕`��ŋ��ʂ�
	 *			�g���f�o�C�X�R���e�L�X�g�ł��B OnItemPainting() �ȂǂŃf�o�C�X�R���e�L�X�g��
	 *			��ԕύX���Ė߂��^�C�~���O���Ȃ��ꍇ�A�{���\�b�h�ŋL�����A OnEndItemPaint()
	 *			�Ō��ɖ߂��悤�ɂ��܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnBeginItemPaint(HDC dc) 
	{
		m_lowLine = 0;
		_mark::BeginItemPaint();
	}

	/**
	 * [�ʒm] �`��I��.
	 *		��A�̕`��̏I����ʒm���܂��B
	 *	@see OnBeginItemPaint()
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnEndItemPaint(HDC dc) 
	{
		bool r = _mark::EndItemPaint();
		// �`�悵�Ă��Ȃ��Ƃ����`��
		CRect rc;
		GetClientRect(rc);
		rc.top = m_lowLine;
		_mark::DrawBackColor(dc, rc);
		// �ĕ`��
		if ( r )
		{
			Invalidate();
		}
	}

	/**
	 * [�ʒm] �A�C�e���`��.
	 *	@note �T�u�A�C�e�����ɒʒm����܂��B
	 *	@param[in,out] P �`��p���B�͈͂�A�C�e��NO�������Ă��܂��B
	 *	@retval CDRF_NOTIFYITEMDRAW �ʏ�̕`����s��(�{���\�b�h�ł͉������Ă��Ȃ�)�B
	 *	@retval CDRF_NEWFONT �t�H���g�╶���F��ύX�������Ԃ��܂��B
	 *	@retval CDRF_SKIPDEFAULT �{���\�b�h�ŕ`�悸�݁B
	 */
	virtual LRESULT OnItemPainting(TDrawParam* P)
	{
		HRGN rgn = ::CreateRectRgnIndirect(&P->rect);
		::ExtSelectClipRgn(P->dc, rgn, RGN_DIFF);
		_DeleteObject(rgn);
		m_lowLine = max(m_lowLine, P->rect.bottom);
		// �`�F�b�N�{�b�N�X�̎��肪���߂���̂ŏ����Ă���
		_mark::DrawBackColor(P->dc, CRect(0, P->rect.top, P->rect.left, P->rect.bottom));
		return CDRF_NOTIFYITEMDRAW;		// �`�F�b�N�{�b�N�X�A����`���Ă��炤
	}

	/**
	 * [�ʒm] �A�C�e���`��I���.
	 *	@note �T�u�A�C�e���`�悪�I��������ƁA�A�C�e�����ɒʒm����܂��B
	 *	@param[in] P �`��p���B rect �́A�T�u�A�C�e�����܂ޔ͈́B
	 */
	virtual void OnItemPainted(const TDrawParam* P) 
	{
		HTREEITEM h = reinterpret_cast<HTREEITEM>(P->itemNo);
		::SelectClipRgn(P->dc, NULL);
		//
		CString str = GetItemText(h);
		IDrawable* pDraw = m_map[h];
		CRect rc = P->rect;
		//
		if ( pDraw != NULL && ! str.IsEmpty() )
		{
			// BG�����`����p����
			pDraw->Resize(rc.Size());
			pDraw->Draw(P->dc, rc.left, rc.top);
			pDraw->Draw(NULL, rc.left, rc.top);
			if ( _mark::GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(P->dc, rc, h);
			}
			if ( (P->state & CDIS_FOCUS) != 0 )
			{
				_mark::DrawFocusMark(P->dc, rc);
			}
			return;
		}
		rc.OffsetRect(-rc.TopLeft());
		CBitmapImage bi;
		bi.Set(rc.Width(), rc.Height(), _mark::GetBackColor());
		if ( ! bi.IsEmpty() )
		{
			CBitmapDC dc(&bi);
			_mark::DrawBackground(dc, rc);
			if ( _mark::GetSelectMarkType() == BACKGROUND )
			{
				m_DrawSelectMaker(dc, rc, h);
			}
			if ( pDraw != NULL )
			{
				pDraw->Resize(rc.Size());
				pDraw->Draw(dc, 0, 0);
			}
			else
			{
				_mark::SetDefaultTextDrawer(this);
				_mark::DrawText(dc, rc, str);
			}
			if ( _mark::GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(dc, rc, h);
			}
			if ( (P->state & CDIS_FOCUS) != 0 )
			{
				_mark::DrawFocusMark(dc, rc);
			}
			dc.Draw(P->dc, P->rect.left, P->rect.top);
			if ( pDraw != NULL )
			{
				pDraw->Draw(NULL, P->rect.left, P->rect.top);
			}
		}
	}

	/**
	 * [�m�F] �T�u�A�C�e���T�O�̗L��
	 *	@retval ture �T�u�A�C�e������B
	 *	@retval false �Ȃ��B
	 */
	virtual bool HasSubItem(void) const
	{
		return false;
	}

	/**
	 * [�m�F] �A�C�e��RECT�v�Z
	 *	@note _pa.itemNo, _pa.subItemNo �Ȃǂ���A _pa.rect �̐ݒ���s���Ă��������B
	 *	@note _pa.state �̒������\�ł��B
	 *	@param[in,out] _pa �`����
	 *	@param[in] isSub true �Ȃ� sub�A�C�e���p�̌v�Z������B
	 */
	virtual void OnCalcItemRect(TDrawParam& _pa, bool isSub)
	{
		CRect rc;
		_super::GetItemRect(reinterpret_cast<HTREEITEM>(_pa.itemNo), rc, TRUE);
		_pa.rect.left = rc.left;	// �e�L�X�g�̎n�܂���ɂ���
	}

	/**
	 * [�ʒm] for notifications from parent
	 *	@note
	 *		CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *		���b�Z�[�W��M������R�[������܂��B
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
			if ( lpNmHdr->hwndFrom == m_hWnd)
			{
				if ( _custom::OnChildNotify(message, wParam, lParam, _pResult) )
				{
					return TRUE;
				}
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
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
		case TVM_DELETEITEM:
			{
				HTREEITEM h = reinterpret_cast<HTREEITEM>(lParam);
				if ( h == TVI_ROOT )
				{
					m_map.RemoveAll();
				}
				else
				{
					m_DeleteItem(h);
				}
			}
			break;
		case WM_KEYDOWN:
			if ( wParam == ' ' )
			{
				HTREEITEM h = _super::GetSelectedItem();
				if ( h != NULL )
				{
					BOOL old = GetCheck(h);
					LRESULT r =_super::WindowProc(message, wParam, lParam);
					if ( old != GetCheck(h) )
					{
						m_NotifyParent(h);
					}
					return r;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
//				CPoint pos(LOWORD(lParam), HIWORD(lParam));
				CPoint pos(lParam);
				UINT fg;
				HTREEITEM h = _super::HitTest(pos, &fg);
				if ( h != NULL )
				{
					if ( (fg & TVHT_ONITEMRIGHT) != 0 )
					{
						_super::SelectItem(h);
					}
					else if ( (fg & TVHT_ONITEMSTATEICON) != 0 )
					{
						BOOL old = GetCheck(h);
						LRESULT r =_super::WindowProc(message, wParam, lParam);
						ASSERT( old != GetCheck(h) );
						m_NotifyParent(h);
						return r;
					}
				}
			}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case WM_SIZE:
			Invalidate();
			break;
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
			{
				_super::SetRedraw(FALSE);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				_super::SetRedraw(TRUE);
				return r;
			}
			break;
		case WM_ERASEBKGND:
			return 1;
		case TVM_SETBKCOLOR:
			SetBackColor(lParam);
			break;
		case WM_DESTROY:
			_mark::AllReset();
			break;
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
		_super::PreSubclassWindow();
		if ( (_super::GetStyle() & TVS_CHECKBOXES) != 0 )
		{
			// �o�O�H�u�`�F�b�N�{�b�N�X������ꍇ�A��x�X�^�C�����Ȃ����v
			_super::ModifyStyle(TVS_CHECKBOXES, 0, 0);
			_super::ModifyStyle(0, TVS_CHECKBOXES, 0);
		}
		_super::ModifyStyle(0, TVS_NONEVENHEIGHT, 0);
		SetBackColor(GetBkColor());
	}

	CMapT<HTREEITEM, IDrawable::Ptr>	m_map;		///< HTREEITEM �� �`����̊֘A
	int									m_lowLine;	///< �w�i�`��p
	
public:

	/// �R���X�g���N�^
	CDrawingTreeCtrl(void) : m_lowLine(0)
	{
	}

	/**
	 * [�ǉ�] �A�C�e���ǉ�
	 *	@param draw �`����
	 *	@param hParent �e�̃n���h���B
	 *	@param hInsertAfter �ǉ������O�̃A�C�e���̃n���h���B
	 *	@param hasBg �`���񂪔w�i�����ׂĕ`�悷��ꍇ�A true �ɂ��܂��i�`���c�L���y���ł��܂��j�B
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO �����B�l�̓n���h���ł��B
	 */
	HTREEITEM InsertItem(const IDrawable &draw, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST, bool hasBg = false)
	{
		HTREEITEM h = _super::InsertItem(hasBg ? _T("*") : _T(""), hParent, hInsertAfter);
		if ( h != NULL )
		{
			m_map[h] = draw.Clone();
		}
		return h;
	}

	/**
	 * [�ǉ�] �A�C�e���ǉ�.
	 *	@param lpszItem �\��������
	 *	@param hParent �e�̃n���h���B
	 *	@param hInsertAfter �ǉ������O�̃A�C�e���̃n���h���B
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO �����B�l�̓n���h���ł��B
	 */
	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST)
	{
		return _super::InsertItem(lpszItem, hParent, hInsertAfter);
	}

	/**
	 * [�擾] �A�C�e���̕`����擾.
	 *	@param item �A�C�e���n���h��.
	 *	@retval NULL �G���[.
	 *	@retval NULL�ȊO �`����Bdelete ���Ă͂����܂���B
	 */
	IDrawable* GetItemDrawer(HTREEITEM item)
	{
		return m_map[item];
	}

	/**
	 * [�ݒ�] ���F�w��
	 *	@param clrNew ���̐F
	 *	@return ���O�̐��̐F
	 */
	COLORREF SetLineColor(COLORREF clrNew = CLR_DEFAULT)
	{
		return TreeView_SetLineColor(*this, clrNew);
	}

	/**
	 * [�擾] �A�C�e����Ԏ擾
	 *	@param hItem �A�C�e���n���h��
	 *	@return ���
	 */
	UINT GetState(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item;
		item.mask = TVIF_HANDLE | TVIF_STATE;
		item.hItem = hItem;
		item.stateMask = TVIS_STATEIMAGEMASK;
		VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));
		return item.state >> 12;
	}

	/**
	 * [�ݒ�] �A�C�e����Ԑݒ�
	 *	@param hItem �A�C�e���n���h��
	 *	@param state ���
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetState(HTREEITEM hItem, UINT state)
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item;
		item.mask = TVIF_HANDLE | TVIF_STATE;
		item.hItem = hItem;
		item.stateMask = TVIS_STATEIMAGEMASK;
		item.state = INDEXTOSTATEIMAGEMASK(state);
		return !! ::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
	}
};


	
}; // MFC
}; // TNB
