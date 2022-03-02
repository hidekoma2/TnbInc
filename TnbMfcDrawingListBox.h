#pragma once
/**
 *@file
 * �`����ListBox�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbDrawingContainer.h"
#include "TnbVector.h"
#include "TnbMfcBitmapDC.h"
#include "TnbMfcAbstractDrawingCtrl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����ListBox�R���g���[��
 *
 *		�s�P�ʂŕ\������`������w��o���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�{�b�N�X�̃v���p�e�B�́u�I�[�i�[�`��(O)�v���u�ρv�ɂ��Ă����Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingListBox.h
 * 
 *	@date 07/12/07	�V�K�쐬
 *	@date 10/01/07	Destroy���̃}�[�N�֌W�̏������R��C���B
 *	@date 10/04/26	�\���ύX
 *	@date 11/08/03	�`�掞�A IDrawable::DrawEx() ���g���悤�ɕύX�B CalcDrawParam() �ǉ��B
 */
class CDrawingListBox : public CListBox, public CAbstractDrawingCtrl
{
	DEFSUPER(CListBox);
	typedef CAbstractDrawingCtrl	_mark;			///< SelectMark�Ǘ����Đ錾
public:

	/// �R���X�g���N�^
	CDrawingListBox(void) : _super(), m_itemsWidth(0)
	{
	}

	/// �f�X�g���N�^
	~CDrawingListBox(void)
	{
		m_ResetContent();
	}

	/**
	 * [�擾] �`����擾.
	 *	@param index �擾����ʒu.
	 *	@retval NULL �G���[.
	 *	@retval NULL�ȊO �`����Bdelete ���Ă͂����܂���B
	 */
	const IDrawable* GetDrawing(int index) const
	{
		if ( ! m_drawersList.IsInRange(index) )
		{
			return NULL;
		}
		return m_drawersList[index];
	}

	/**
	 * [�ǉ�] �`�����s�ǉ�.
	 *	@note ��s�ǉ����܂��B
	 *	@param draw �`����
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �ǉ������C���f�b�N�X
	 */
	int AddDrawing(const IDrawable& draw)
	{
		int r = _super::AddString(_T(""));
		if ( r >= 0 )
		{
			if ( ! m_InsertDrawing(r, draw) )
			{
				r = LB_ERR;
			}
		}
		return r;
	}

	/**
	 * [�ǉ�] �������s�ǉ�.
	 *	@note ��s�ǉ����܂��B
	 *	@param lpszItem ������
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �ǉ������C���f�b�N�X
	 */
	int AddString(LPCTSTR lpszItem)
	{
		return _super::AddString(lpszItem);
	}
	
	/**
	 * [�ǉ�] �`�����s�}��.
	 *	@note ��s�}�����܂��B
	 *	@param index �}������ʒu
	 *	@param draw �`����
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �ǉ������C���f�b�N�X
	 */
	int InsertDrawing(int index, const IDrawable& draw)
	{
		int r = _super::InsertString(index, _T(""));
		if ( r >= 0 )
		{
			if ( ! m_InsertDrawing(r, draw) )
			{
				r = LB_ERR;
			}
		}
		return r;
	}

	/**
	 * [�ǉ�] �������s�}��.
	 *	@note ��s�}�����܂��B
	 *	@param index �}������ʒu
	 *	@param lpszItem ������
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �ǉ������C���f�b�N�X
	 */
	int InsertString(int index, LPCTSTR lpszItem)
	{
		return _super::InsertString(index, lpszItem);
	}

	/**
	 * [�ǉ�] �`����ύX.
	 *	@note �ݒ肵���`�����ύX���܂��B
	 *	@param index �ύX����ʒu
	 *	@param draw �`����
	 *	@retval false �G���[�B
	 *	@retval true ����
	 */
	int ResetDrawing(int index, const IDrawable& draw)
	{
		return m_ResetDrawing(index, draw);
	}

	/**
	 * [�폜] ��s�폜.
	 *	@param index �폜����ʒu
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �����B�����͎c��̐��B
	 */
	int DeleteDrawing(UINT index)
	{
		int r = _super::DeleteString(index);
		if ( r >= 0 )
		{
			m_drawersList.Remove(index);
		}
		return r;
	}

	/**
	 * [�폜] ��s�폜.
	 *	@param index �폜����ʒu
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �����B�����͎c��̐��B
	 */
	int DeleteString(UINT index)
	{
		return DeleteDrawing(index);
	}

	/**
	 * [�폜] �S�`����폜.
	 */
	void ResetContent(void)
	{
		m_ResetContent();
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			_super::SetHorizontalExtent(m_itemsWidth);
			_super::ResetContent();
		}
	}

protected:

	/**
	 * [�擾] �`��p�p�����[�^�v�Z.
	 *		�`����� IDrawable::DrawEx() �� lParam �ɓn���l���v�Z���܂��B
	 *	@note �{���\�b�h�ł́A itemNo �� lParam �Ƃ��Ă܂��B
	 *	@param itemNo �A�C�e�� No.
	 *	@return �p�����[�^.
	 */
	virtual LPARAM CalcDrawParam(DWORD_PTR itemNo) const
	{
		return itemNo;
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
		case WM_ERASEBKGND: //�w�i
			{
				HDC hdc = reinterpret_cast<HDC>(wParam);
				CRect rect;
				GetClientRect(rect);
				int y = 0;
				for ( int i = GetTopIndex(); i < GetCount(); i++ )
				{
					y += GetItemHeight(i);
				}
				int h = rect.Height();
				if( h > y )
				{
					_mark::DrawBackColor(hdc, CRect(0, y, rect.Width(), h));
				}
				return 0;
			}
			break;
		case WM_PAINT:
			if ( GetCount() == 0 )
			{
				CPaintDC dc(this);
				return 0;
			}
			else
			{
				_mark::BeginItemPaint();
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				_mark::EndItemPaint();
				return r;
			}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			Invalidate();
			break;
		case WM_DESTROY:
			m_ResetContent();
			_mark::AllReset();
			break;
		case LB_RESETCONTENT: //�S����
			m_ResetContent();
			_super::SetHorizontalExtent(m_itemsWidth);
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
		#ifdef _DEBUG
			DWORD dwStyle = _super::GetStyle();
			ASSERT0(dwStyle & (LBS_OWNERDRAWVARIABLE | LBS_OWNERDRAWFIXED), "CDrawingListBox", "[�I�[�i�[�`��]��[�Ȃ�]�ȊO�ɂ��Ă�������");
			ASSERT0((dwStyle & LBS_SORT) == 0, "CDrawingListBox", "[�\�[�g]��OFF�ɂ��Ă�������");
			//			ASSERT0(dwStyle & LBS_EXTENDEDSEL, "PreSubclassWindow", "");
		#endif
		_super::PreSubclassWindow();
	}

	/**
	 * �I�[�i�[�h���[��.
	 *	�h���[��Item�̍����m�F�̂��߂ɃR�[�������
	 *	@param lpMeasureItemStruct ���
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		if ( lpMeasureItemStruct->CtlType != ODT_LISTBOX || ToInt(lpMeasureItemStruct->CtlID) != _super::GetDlgCtrlID() )
		{
			return;
		}
	}
		
	/**
	 * [�ʒm] �I�[�i�[�h���[����.
	 * 		�I�[�i�[�h���[���R�[������܂��B
	 *	@note �`�����p�ӂ��邱�ƂŁA�C�ӂɕ`�悳���邱�Ƃ��o���܂��B
	 *	@param lpDrawItemStruct �`����.
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		if ( lpDrawItemStruct->CtlType != ODT_LISTBOX || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		UINT index = lpDrawItemStruct->itemID;
		HDC dc = lpDrawItemStruct->hDC;
		const CRect& rc = lpDrawItemStruct->rcItem;
		CSize sz = rc.Size();
		CBitmapImage bi;
		if ( bi.Set(sz.cx, sz.cy) )
		{
			LPARAM lParam = CalcDrawParam(index);
			CBitmapDC bmpDC(&bi);
			_mark::DrawBackground(bmpDC, CRect(CPoint(0, 0), sz), lParam);
			if ( GetSelectMarkType() == BACKGROUND )
			{
				m_DrawSelectMaker(bmpDC, sz, lpDrawItemStruct->itemState, lParam);
			}
			const IDrawable* pDraw = GetDrawing(index);
			if ( pDraw != NULL )
			{
				SIZE s;
				pDraw->GetSize(s);
				pDraw->DrawEx(bmpDC, 0, (sz.cy - s.cy) / 2, lParam);
			}
			else if ( ::IsWindow(m_hWnd) && static_cast<INDEX>(GetCount()) > index ) 
			{
				CString str;
				GetText(index, str);
				_mark::SetDefaultTextDrawer(this);
				_mark::DrawText(bmpDC, CRect(0, 0, sz.cx, sz.cy), str, lParam);
			}
			if ( GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(bmpDC, sz, lpDrawItemStruct->itemState, lParam);
			}
			if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
			{
				DrawFocusMark(bmpDC, CRect(CPoint(0, 0), sz), lParam);
			}
			if ( ! ::IsWindowEnabled(GetSafeHwnd()) )
			{
				DrawDisabledStyle(bmpDC, CRect(CPoint(0, 0), sz), lParam);
			}
			bmpDC.Draw(dc, rc.left, rc.top);
		}
	}

private:

	// �A�C�e���S����
	void m_ResetContent(void)
	{
		m_itemsWidth = 0;
		m_drawersList.RemoveAll();
	}
	//�A�C�e���̕��v�Z�i���ʂ� m_iItemWidth �ɑ���j
	void m_CalcItemWidth(bool withReheight = false)
	{
		m_itemsWidth = 0;
		loop ( i, m_drawersList.GetSize() )
		{
			SIZE size;
			IDrawable::Ptr P = m_drawersList[i];
			if ( ! P.IsNull() && m_drawersList[i]->GetSize(size) )
			{
				if ( m_itemsWidth < static_cast<DWORD>(size.cx) )
				{
					m_itemsWidth = size.cx;
				}
				if ( withReheight )
				{
					_super::SetItemHeight(ToInt(i), size.cy);
				}
			}
		}
	}
	/// �A�C�e���ǉ�
	bool m_InsertDrawing(int index, const IDrawable& draw)
	{
		if ( m_drawersList.Insert(index, draw.Clone()) )
		{
			SIZE size;
			draw.GetSize(size);
			m_CalcItemWidth(true);
			_super::SetHorizontalExtent(m_itemsWidth);
			_super::SetItemHeight(index, size.cy);
			return true;
		}
		_super::DeleteString(index);
		return false;
	}
	/// �A�C�e���ύX
	bool m_ResetDrawing(int index, const IDrawable& draw)
	{
		if ( m_drawersList.Set(index, draw.Clone()) )
		{
			SIZE size;
			draw.GetSize(size);
			m_CalcItemWidth();
			_super::SetHorizontalExtent(m_itemsWidth);
			_super::SetItemHeight(index, size.cy);
			return true;
		}
		return false;
	}
	/// �I���}�[�N�`��
	void m_DrawSelectMaker(HDC dc, const SIZE& sz, UINT itemState, LPARAM lParam)
	{
		HWND hFocus = ::GetFocus();
		bool r = (hFocus == m_hWnd);
		if ( ! r && ::IsWindow(m_hWnd) )
		{
			CWnd* P = GetOwner();
			if ( P != NULL )
			{
				r = (hFocus == P->GetSafeHwnd());
			}
		}
		if ( (itemState & CDIS_SELECTED) != 0 )
		{
			_mark::DrawSelectMark(dc, CRect(CPoint(0, 0), sz), r, lParam);
		}
	}

	DWORD							m_itemsWidth;	///< �SItem�̕�
	CAutoVectorT<IDrawable::Ptr>	m_drawersList;	///< ���ꂼ��̍s�̕`����
};



}; // MFC
}; // TNB
