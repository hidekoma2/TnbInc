#pragma once
/**
 *@file
 * �`����ComboBox�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbMfcDrawingListBox.h"
#include "TnbMfcDrawingButton.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����ListBox�R���g���[��
 *
 *		�s�P�ʂŕ\������`������w��o���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃR���{�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCComboBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�{�b�N�X�̃v���p�e�B�́u�I�[�i�[�`��(O)�v���u�ρv�ɂ��Ă����Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingComboBox.h
 * 
 *	@date 08/03/18	�V�K�쐬
 *	@date 10/04/26	�\���ύX
 *	@date 11/08/11	�T�u�N���X���������AListBox�̃n���h�����킩��΃T�u�N���X������悤�ɂ����B
 */
class CDrawingComboBox : public CComboBox
{
	DEFSUPER(CComboBox);
public:

	/// �R���X�g���N�^
	CDrawingComboBox(void) : _super(), m_isUseDrawingButton(false)
	{
	}

	/// �f�X�g���N�^
	~CDrawingComboBox(void)
	{
	}

	/**
	 * [�擾] �`����擾.
	 *	@param index �擾����ʒu.
	 *	@retval NULL �G���[.
	 *	@retval NULL�ȊO �`����Bdelete ���Ă͂����܂���B
	 */
	const IDrawable* GetDrawing(int index) const
	{
		return m_listBox.GetDrawing(index);
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
	 * [�ǉ�] �`�����s�ǉ�.
	 *	@note ��s�ǉ����܂��B
	 *	@param draw �`����
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �ǉ������C���f�b�N�X
	 */
	int AddDrawing(const IDrawable& draw)
	{
		m_InitListBox();
		int r = _super::AddString(_T(""));
		if ( r >= 0 )
		{
			if ( m_listBox.ResetDrawing(r, draw) )
			{
				m_CheckHeight(draw);
			}
			else
			{
				_super::DeleteString(r);
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
	 * [�ǉ�] �`�����s�}��.
	 *	@note ��s�}�����܂��B
	 *	@param index �}������ʒu
	 *	@param draw �`����
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �ǉ������C���f�b�N�X
	 */
	int InsertDrawing(int index, const IDrawable& draw)
	{
		m_InitListBox();
		int r = _super::InsertString(index, _T(""));
		if ( r >= 0 )
		{
			if ( m_listBox.ResetDrawing(r, draw) )
			{
				m_CheckHeight(draw);
			}
			else
			{
				_super::DeleteString(r);
				r = LB_ERR;
			}
		}
		return r;
	}

	/**
	 * [�폜] ��s�폜.
	 *	@param index �폜����ʒu
	 *	@retval LB_ERR �G���[�B
	 *	@retval 0�ȏ� �����B�����͎c��̐��B
	 */
	int DeleteDrawing(UINT index)
	{
		m_InitListBox();
		_super::DeleteString(index);
		return m_listBox.DeleteDrawing(index);
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
		m_InitListBox();
		_super::ResetContent();
		m_listBox.ResetContent();
	}

	/**
	 * [�ݒ�] �f�t�H���g�̃}�[�N�`��w��.
	 *	@note �t�H�[�J�X�}�[�N�A�I���}�[�N��Window�W���Ɠ����ɂ��܂��B
	 */
	void SetDefaultMarkDrawer(void)
	{
		m_listBox.SetDefaultMarkDrawer();
	}

	/**
	 * [�ݒ�] �w�i�`��w��.
	 *	@param draw Resize() ���L���Ȋe�A�C�e���̔w�i�`����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetBackgroundDrawer(const IDrawable& draw)
	{
		return m_listBox.SetBackgroundDrawer(draw);
	}

	/**
	 * [�ݒ�] �I���}�[�N�`��w��.
	 *	@note �A�N�e�B�u���Ɣ�A�N�e�B�u���œ����I���}�[�N���g���܂��B
	 *	@param draw Resize() ���L���ȑI���}�[�N�̕`����B
	 *	@param type �I���}�[�N�̃^�C�v�B�ȗ�����ƁA OVERLAP �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw, CAbstractDrawingCtrl::ESelectMarkType type = CAbstractDrawingCtrl::OVERLAP)
	{
		return m_listBox.SetSelectMarkDrawer(draw, type);
	}
	
	/**
	 * [�ݒ�] �I���}�[�N�`��w��.
	 *	@param draw1 Resize() ���L���ȃA�N�e�B�u���̑I���}�[�N�̕`����B
	 *	@param draw2 Resize() ���L���Ȕ�A�N�e�B�u���̑I���}�[�N�̕`����B
	 *	@param type �I���}�[�N�̃^�C�v�B�ȗ�����ƁA OVERLAP �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw1, const IDrawable& draw2, CAbstractDrawingCtrl::ESelectMarkType type = CAbstractDrawingCtrl::OVERLAP)
	{
		return m_listBox.SetSelectMarkDrawer(draw1, draw2, type);
	}
	
	/**
	 * [�ݒ�] �t�H�[�J�X�}�[�N�`��w��.
	 *	@param draw Resize() ���L���ȃt�H�[�J�X�}�[�N�̕`����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetFocusMarkDrawer(const IDrawable& draw)
	{
		return m_listBox.SetFocusMarkDrawer(draw);
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *	@param color �F
	 */
	void SetBackColor(COLORREF color)
	{
		m_listBox.SetBackColor(color);
	}

	/**
	 * [�ݒ�] �e�L�X�g�`����w��
	 *	@note �w�肵�Ȃ��ꍇ�ł��A�W�������̕`�悪�s���܂��B
	 *	@param text �e�L�X�g�`��
	 */
	void SetTextDrawer(const CTextDrawer& text)
	{
		m_listBox.SetTextDrawer(text);
	}

	/**
	 * [�Q��] �{�^���R���g���[���N���X�Q��.
	 *		�{�R���{�{�b�N�X�����{�^���R���g���[���N���X�̎Q�Ƃ�Ԃ��܂��B
	 *	@note UseDrawingButton() ���\�b�h���R�[�����Ă��Ȃ��ꍇ�A�����ȃR���g���[�����Ԃ���܂��B
	 *	@return �{�^���R���g���[���̎Q��
	 */
	CDrawingButton& ReferButtonControl(void)
	{
		return m_button;
	}

	/**
	 * [�ݒ�] �{�^���ݒ�.
	 *		�{�R���{�{�b�N�X���J�X�^�}�C�Y�\�̃{�^�����g�����ۂ��A�w�肵�܂��B
	 *	@param r true �Ȃ� CDrawingButton ���g�p���܂��B
	 *			false �Ȃ�W���̃{�^�����g�p���܂��B
	 */
	void UseDrawingButton(bool r = true)
	{
		m_isUseDrawingButton = r;
		if ( ::IsWindow(m_button) )
		{
			if ( ! r )
			{
				m_button.DestroyWindow();
			}
			RedrawWindow();
		}
	}

protected:

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
			return 0;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			Invalidate();
			break;
		case WM_DESTROY:
			m_listBox.ResetContent();
			if ( m_listBox.GetSafeHwnd() != NULL )
			{
				m_listBox.UnsubclassWindow();
			}
			m_button.DestroyWindow();
			break;
		case WM_CTLCOLORLISTBOX:
			if ( m_listBox.GetSafeHwnd() == NULL )
			{
				m_listBox.SubclassWindow(reinterpret_cast<HWND>(lParam));
				m_listBox.SetOwner(this);
			}
			break;
		case WM_SETREDRAW:
			if ( m_listBox.GetSafeHwnd() != NULL )
			{
				m_listBox.SendMessage(WM_SETREDRAW, wParam, lParam);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if ( ::IsWindow(m_button) )
			{
				if ( ! m_validRect.PtInRect(CPoint(lParam)) )
				{
					return 0;
				}
			}
			break;
		case WM_COMMAND:
			if ( wParam == BUTTON_CTRLID )
			{
				SetFocus();
				ShowDropDown();
				m_listBox.RedrawWindow();
			}
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
			ASSERT0(dwStyle & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE), "CDrawingComboBox", "[�I�[�i�[�`��]��[�Ȃ�]�ȊO�ɂ��Ă�������");
			ASSERT0(dwStyle & CBS_DROPDOWNLIST, "CDrawingComboBox", "[]��[�h���b�v�_�E�����X�g]�ɂ��Ă�������");
			ASSERT0(dwStyle & CBS_HASSTRINGS, "CDrawingComboBox", "[������]��[����]�ɂ��Ă�������");
			ASSERT0((dwStyle & CBS_SORT) == 0, "CDrawingComboBox", "[�\�[�g]��[false]�ɂ��Ă�������");
		#endif
		_super::PreSubclassWindow();
		const int _CB_GETCOMBOBOXINFO = 0x0164;	
		_COMBOBOXINFO cbi;
		cbi.cbSize = sizeof(cbi);
		if ( SendMessage(_CB_GETCOMBOBOXINFO, 0, reinterpret_cast<LPARAM>(&cbi)) )
		{
			if ( m_listBox.GetSafeHwnd() == NULL )
			{
				HWND h = cbi.hwndList;
				m_listBox.SubclassWindow(h);
				m_listBox.SetOwner(this);
			}
		}
	}

	/**
	 * [�ʒm] �I�[�i�[�h���[����.
	 * 		�I�[�i�[�h���[���R�[������܂��B
	 *	@param lpDrawItemStruct �`����.
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		if ( lpDrawItemStruct->CtlType != ODT_COMBOBOX || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		const CRect& rc = lpDrawItemStruct->rcItem;
		if ( m_isUseDrawingButton && ! ::IsWindow(m_button) )
		{
			m_validRect = rc;
			DWORD style = BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE;
			int x = ::GetSystemMetrics(SM_CXVSCROLL);
			CRect r(rc.right + 1, rc.top, rc.right + x + 1, rc.bottom + 1);
			if ( (GetExStyle() & WS_EX_RIGHT) != 0 )
			{
				r.left = rc.left - x - 1;
				r.right = rc.left - 1;
			}
			m_button.Create(_T("Button"), _T(""), style, r, this, BUTTON_CTRLID);
			
		}
		if ( ::IsWindow(m_listBox) )
		{
			lpDrawItemStruct->CtlType = ODT_LISTBOX;
			lpDrawItemStruct->hwndItem = m_listBox.GetSafeHwnd();
			lpDrawItemStruct->CtlID = m_listBox.GetDlgCtrlID();
			m_listBox.MyDrawItem(lpDrawItemStruct);
		}
	}

	/**
	 * �I�[�i�[�h���[��.
	 *	�h���[��Item�̍����m�F�̂��߂ɃR�[�������
	 *	@param lpMeasureItemStruct ���
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		if ( lpMeasureItemStruct->CtlType != ODT_COMBOBOX || ToInt(lpMeasureItemStruct->CtlID) != _super::GetDlgCtrlID() )
		{
			return;
		}
		if ( ::IsWindow(m_listBox) )
		{
			lpMeasureItemStruct->CtlType = ODT_LISTBOX;
			lpMeasureItemStruct->CtlID = m_listBox.GetDlgCtrlID();
			m_listBox.MyMeasureItem(lpMeasureItemStruct);
		}
	}

private:
	/// �����pListBox
	class CMyListBox : public CDrawingListBox
	{
		DEFSUPER(CDrawingListBox);
	public:
		virtual void MyMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
		{
			_super::MeasureItem(lpMeasureItemStruct);
		}
		virtual void MyDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
		{
			_super::DrawItem(lpDrawItemStruct);
		}
	};
	/// CB_GETCOMBOBOXINFO ���b�Z�[�W�̃p�����^
	struct _COMBOBOXINFO
	{
		DWORD cbSize;
		RECT rcItem;
		RECT rcButton;
		DWORD stateButton;
		HWND hwndCombo;
		HWND hwndItem;
		HWND hwndList;
	};
	/**
	 * ListBox��������
	 *�i��u�h���b�v����WM_CTLCOLORLISTBOX �𔭐�������j
	 */
	void m_InitListBox(void)
	{
		if ( m_listBox.GetSafeHwnd() == NULL )
		{
			CRect rc;
			GetClientRect(&rc);
			MFCLIB::ChangeClientSize(this, -1, rc.bottom + 100);
			ShowDropDown();
			ShowDropDown(FALSE);
			MFCLIB::ChangeClientSize(this, -1, rc.bottom);
		}
	}
	/// �����`�F�b�N
	void m_CheckHeight(const IDrawable& draw)
	{
		int h = _super::GetItemHeight(-1);
		SIZE sz;
		if ( draw.GetSize(sz) )
		{
			if ( h < sz.cy )
			{
				_super::SetItemHeight(-1, sz.cy);
				m_button.DestroyWindow();
			}
		}
	}
	enum { BUTTON_CTRLID = 100 };
	CMyListBox						m_listBox;				///< ���X�g
	CDrawingButton					m_button;				///< �{�^��
	bool							m_isUseDrawingButton;	///< ���[�U�{�^���g�p����H
	CRect							m_validRect;			///< �L���͈�
};



}; // MFC
}; // TNB
