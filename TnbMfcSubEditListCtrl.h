#pragma once
/**
 *@file
 *	�T�u�A�C�e���ҏW���X�g�R���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcDrawingListCtrl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �T�u�A�C�e���ҏW���X�g��p�G�f�B�b�g�C���^�[�t�F�[�X.
 *
 *		�T�u�A�C�e���ҏW���X�g�Ŏg�p����G�f�B�^���`����C���^�[�t�F�[�X�ł��B
 *
 *	@see CSubEditListCtrl
 *
 *	@note SELC = SubEditListCtrl
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSubEditListCtrl.h
 * 
 *	@date 12/02/16	�C���^�[�t�F�[�X��
 */
struct ISelcEditFp
{
	/// �f�X�g���N�^
	virtual ~ISelcEditFp(void) {}
	/**
	 * [�ݒ�] �J�n�\���ʒu�w��.
	 *	@param rect �\���ʒu
	 *	@param clientWidth �N���C�A���g�̕�
	 */
	virtual void SetStartRect(const RECT& rect, int clientWidth) = 0;
	/**
	 * [�擾] CEdit�擾.
	 *	@return CEdit�̃|�C���^
	 */
	virtual CEdit* GetEdit(void) = 0;
	/**
	 * [�ʒm] �ҏW�I���ʒm.
	 */
	virtual void OnEditEnd(void) = 0;
};

/**@ingroup MFCCONTROL
 * �T�u�A�C�e���ҏW���X�g��p�G�f�B�b�g�R���g���[���e���v���[�g.
 *
 *		�T�u�A�C�e���ҏW���X�g�Ŏg�p����G�f�B�^�R���g���[�����`����e���v���[�g�ł��B
 *	
 *	@see CSubEditListCtrl
 *
 *	@note SELC = SubEditListCtrl
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSubEditListCtrl.h
 * 
 *	@date 11/07/04	�V�K�쐬
 */
template<typename TYP>
class CSelcEditFpT : public TYP, public ISelcEditFp
{
	DEFSUPER(TYP);
public:
	/// �R���X�g���N�^
	CSelcEditFpT(void) : m_rect(0)
	{
	}
	/**
	 * [�ݒ�] �J�n�\���ʒu�w��.
	 *	@param rect �\���ʒu
	 *	@param clientWidth �N���C�A���g�̕�
	 */
	void SetStartRect(const RECT& rect, int clientWidth)
	{
		m_rect = rect;
		m_clientWidth = clientWidth;
	}
	/**
	 * [�擾] CEdit�擾.
	 *	@return CEdit�̃|�C���^
	 */
	CEdit* GetEdit(void)
	{
		return this;
	}
	/**
	 * [�ʒm] �ҏW�I���ʒm.
	 */
	virtual void OnEditEnd(void) {}
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
		if ( message == WM_WINDOWPOSCHANGING )
		{
			WINDOWPOS* P = reinterpret_cast<WINDOWPOS*>(lParam);
			P->y = m_rect.top;
			P->cy = m_rect.Height();
			if ( (P->flags & (SWP_NOSIZE | SWP_NOMOVE)) == 0 )
			{
				P->x = m_rect.left;
				if ( P->cx < m_rect.Width() )
				{
					P->cx = m_rect.Width();
				}
				if ( (P->x) + (P->cx) > m_clientWidth )
				{
					// �͂ݏo��
					int d = m_clientWidth - (P->cx);
					if ( d >= 0 )
					{
						P->x = d;
					}
					else
					{
						P->x = 0;
						P->cx = m_clientWidth;
					}
				}
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
private:
	CRect	m_rect;			///< �ʒu
	int		m_clientWidth;
};



/**@ingroup MFCCONTROL
 * �T�u�A�C�e���ҏW���X�g�R���g���[��.
 *
 *		���X�g�R���g���[���̃T�u�A�C�e����I�����邱�Ƃ��o����悤�ɂȂ�܂��B
 *		�܂��A�T�u�A�C�e���̕ҏW���\�ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�� List Ctrl �R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		�e�A�C�e����ҏW�\�ɂ���ꍇ�A���X�g�R���g���[���̃v���p�e�B�́u���x���̕ҏW�v�Ƀ`�F�b�N�����Ă����Ă��������B
 *
 *	@note �v���p�e�B�Łu�w�b�_���\�[�g���Ȃ��v�ɂ���ƃ}�E�X�ɂ��J�����̕��ύX���֎~���Ă��܂��B 
 *			���̏�Ԃŕ��ύX�����������ꍇ�A GetHeaderCtrl()->EnableChangeWidth(true); �Ƃ��܂��B
 *
 *	@note �P��I��(LVS_SINGLESEL)�̃X�^�C����t�����܂��B�����̑I���͏o���܂���B
 *
 *	@note �p���� GetSubItemEditControl() ���I�[�o�[���C�h���邱�ƂŔC�ӂ� Edit�R���g���[����
 *			�C�ӂ̃A�C�e���Ŏg�p�ł��܂��B
 *		\code
 *		// ex) �T�u�A�C�e���O�͕ҏW�s�B�Q�C�R�͓Ǝ��̕ҏW�B���͕W���B	
 *		class CMySubEditListCtrl : public CSubEditListCtrl
 *		{
 *			DEFSUPER(CSubEditListCtrl);
 *		protected:
 *			class CMyEdit : public CSelcEditFpT<CEditHexByte>
 *			{
 *				virtual void OnEditEnd(void) 
 *				{
 *					CEditHexByte::OnEditEnd();
 *				}
 *			};
 *			virtual ISelcEditFp* GetSubItemEditControl(int item, int subItem)
 *			{
 *				if ( subItem == 0 ){ return NULL; }
 *				if ( subItem == 2 || subItem == 3 ) return _super::GetSubItemEditControl(item, subItem);
 *				return &m_edit;
 *			}
 *			CMyEdit m_edit;
 *		};
 *		\endcode
 *
 *	@note ���x���̕ҏW���I��������ɁA LVN_ENDLABELEDIT ���e�E�B���h�E�ɒʒm����܂����A
 *			����A�L�����Z���̔��f�͈ȉ��̂悤�ɂ��܂��iCListCtrl �ł������j�B
 *	\code
 *		void CFooDlg::OnEndlabeleditListSubedit(NMHDR* pNMHDR, LRESULT* pResult) 
 *		{
 *			LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
 *			if ( (pDispInfo->item.mask & LVIF_TEXT) != 0 )
 *			{
 *				// �ύX
 *				TRACE1( "text = [%s]\n", m_myListCtrl.GetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem) );
 *			}
 *			*pResult = 0;
 *		}
 *	\endcode
 *
 *	@todo �{�N���X�� CDrawingListCtrl ���p�����Ă��܂����A �I���}�[�N�A�t�H�[�J�X�}�[�N�͕ύX�s�\�ł��B
 *		SetDefaultMarkDrawer() , SetSelectMarkDrawer() , SetFocusMarkDrawer() ���g�p���Ă��ύX�ł��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSubEditListCtrl.h
 * 
 *	@date 11/07/04	�V�K�쐬
 *	@date 11/09/15	���A�C�e���ԂňقȂ�T�u�A�C�e�����N���b�N�������ɕҏW�J�n���Ȃ��悤�ɂ����B
 *			�����e�L�X�g�ҏW���[�h�̏ꍇ�ł� LVN_ENDLABELEDIT ��ʒm����悤�ɕύX�B
 *	@date 11/09/19	������P�B�ҏW�J�n���A�Ώۂ̃T�u�A�C�e����\������悤�ɂ����B EDIT �̑傫���ςɑΉ��B
 *	@date 11/11/08	�J�[�\���̍��E�������������A�e�� WM_NOTIFY ���s���Ȃ����C���B
 *	@date 12/02/29	�V EditLabel() ��p�ӁB �����e�L�X�g�ҏW���[�h���A�ҏW���ɃN���b�N����ƈقȂ�T�u�A�C�e�����ύX����Ȃ��悤�ɏC���B
 */
class CSubEditListCtrl : public CDrawingListCtrl
{
	DEFSUPER(CDrawingListCtrl);
public:

	/// �R���X�g���N�^
	CSubEditListCtrl(void) : m_currentSubItem(-1), m_pSubclassEdit(NULL), m_isAutoEdit(false), m_editingSubItem(-1)
	{
	}

	/**
	 * [�ݒ�] �����e�L�X�g�ҏW���[�h.
	 *	@note �f�t�H���g�� false �ł��B
	 *	@param isEnable true �Ȃ�ҏW���ʂ������I�� List Ctrl �ɔ��f�����܂��B false �Ȃ炵�܂���B
	 */
	void SetAutoEditMode(bool isEnable)
	{
		m_isAutoEdit = isEnable;
	}

	/**
	 * [�ݒ�] �T�u�A�C�e�����x���ҏW�J�n.
	 *	@param iItem �A�C�e��.
	 *	@param iSubItem �T�u�A�C�e��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool EditSubItemLabel(int iItem, int iSubItem)
	{
		m_currentSubItem = iSubItem;
		return _super::EditLabel(iItem) != NULL;
	}

	/**
	 * [�ݒ�] �A�C�e�����x���ҏW�J�n.
	 *	@retval true ����
	 *	@retval false ���s�B�I������Ă��Ȃ����͎��s���܂��B
	 */
	bool EditLabel(void)
	{
		int iItem = _super::GetSelectedItem();
		int iSubItem = m_currentSubItem;
		if ( iItem >= 0 && iSubItem >=0 )
		{
			return _super::EditLabel(iItem) != NULL;
		}
		return false;
	}

	/**
	 * [�m�F] �ҏW���m�F.
	 *	@retval true ���x���ҏW��.
	 *	@retval false ��
	 */
	bool IsEditingLabel(void) const
	{
		return m_editingSubItem >= 0;
	}

	/**
	 * [�擾] �I���A�C�e���擾.
	 *	@note �P��I���X�^�C����p�ł��B
	 *	@retval �}�C�i�X �I���Ȃ�
	 *	@retval �O�ȏ� �C���f�b�N�X
	 */
	int GetSelectedItem(void) const
	{
		return _super::GetSelectedItem();
	}

	/**
	 * [�擾] �I���T�u�A�C�e��No�擾.
	 *	@return �T�u�A�C�e��No
	 */
	int GetSelectedSubItem(void) const
	{
		return m_currentSubItem;
	}

	/**
	 * [�ݒ�] �I���A�C�e���ݒ�.
	 *	@note �P��I���X�^�C����p�ł��B
	 *	@param item �A�C�e���m��
	 *	@param subItem �T�u�A�C�e���m��
	 */
	void SetSelectedItem(int item, int subItem)
	{
		m_currentSubItem = subItem;
		_super::SetSelectedItem(item);
	}

	/**
	 * [�ݒ�] �A�C�e���\��.
	 *		�w��̃A�C�e����������悤�ɃX�N���[�����܂��B
	 *	@param item �A�C�e���m��
	 *	@param subItem �T�u�A�C�e���m��
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
	 */
	BOOL EnsureVisibleEx(int item, int subItem)
	{
		BOOL r = _super::EnsureVisible(item, FALSE);
		m_ViewSubItem(subItem);
		return r;
	}

	/**
	 * [�ݒ�] �f�t�H���g�̃}�[�N�`��w��.
	 *		�t�H�[�J�X�}�[�N�A�I���}�[�N��Window�W���Ɠ����l�ɂ��܂��B
	 *	@param base �x�[�X�̐F�B�ȗ������ Windows�W���Ɠ����l�ɂ��܂��B
	 */
	void SetDefaultMarkDrawer(COLORREF base = ::GetSysColor(COLOR_WINDOW))
	{
		_super::SetDefaultMarkDrawer(base);
		_super::SetSelectMarkDrawer(
			CMySelectDrawer(this, ::GetSysColor(COLOR_HIGHLIGHT), base, true),
			CMySelectDrawer(this, ::GetSysColor(COLOR_BTNFACE), base, false)
		);
		_super::SetFocusMarkDrawer(CMyFoucsDrawer(this));
		CTextDrawer tx(*GetFont(), RGB(0, 0, 0), _T(""), DT_END_ELLIPSIS);
		_super::SetTextDrawer(tx);
	}

	/**
	 * [�ݒ�] �I���}�[�N�`��w��.
	 *	@note �{�N���X�ł́A�K�����s���܂��B
	 *	@param draw Resize() ���L���ȑI���}�[�N�̕`����B
	 *	@param type �I���}�[�N�̃^�C�v�B�ȗ�����ƁA OVERLAP �ɂȂ�܂��B
	 *	@return false ���s�B
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw, ESelectMarkType type = OVERLAP)
	{
		return false;
	}
	
	/**
	 * [�ݒ�] �I���}�[�N�`��w��.
	 *	@note �{�N���X�ł́A�K�����s���܂��B
	 *	@param draw1 Resize() ���L���ȃA�N�e�B�u���̑I���}�[�N�̕`����B
	 *	@param draw2 Resize() ���L���Ȕ�A�N�e�B�u���̑I���}�[�N�̕`����B
	 *	@param type �I���}�[�N�̃^�C�v�B�ȗ�����ƁA OVERLAP �ɂȂ�܂��B
	 *	@return false ���s�B
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw1, const IDrawable& draw2, ESelectMarkType type = OVERLAP)
	{
		return false;
	}
	
	/**
	 * [�ݒ�] �t�H�[�J�X�}�[�N�`��w��.
	 *	@note �{�N���X�ł́A�K�����s���܂��B
	 *	@param draw Resize() ���L���ȃt�H�[�J�X�}�[�N�̕`����B
	 *	@return false ���s�B
	 */
	bool SetFocusMarkDrawer(const IDrawable& draw)
	{
		return false;
	}

	/**
	 * [�擾] Edit�R���g���[���擾.
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO edit�̃|�C���^
	 */
	CEdit* GetEditControl(void) const
	{
		if ( m_pSubclassEdit != NULL )
		{
			return m_pSubclassEdit->GetEdit();
		}
		return NULL;
	}
	
protected:

	/**
	 * [�Q��] �T�u�A�C�e���ҏW�R���g���[���擾.
	 *	@param item �A�C�e���m��
	 *	@param subItem �T�u�A�C�e���m��
	 *	@retval NULL �ҏW�s��
	 *	@retval NULL�ȊO �ҏW�p�� EDIT �R���g���[��
	 */
	virtual ISelcEditFp* GetSubItemEditControl(int item, int subItem)
	{
		return &m_myEdit;
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
		m_drawingSubItem = P->subItemNo;
		return _super::OnItemPainting(P);
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
				NMLVDISPINFO* lpLv = reinterpret_cast<NMLVDISPINFO*>(lParam);
				if ( lpLv->hdr.code == LVN_BEGINLABELEDIT )
				{
					LVITEM& item = lpLv->item;
					if ( m_currentSubItem >= 0 ) 
					{
						item.iSubItem = m_currentSubItem;
						m_editingSubItem = m_currentSubItem;
						if ( _super::GetItemDrawer(item.iItem, item.iSubItem) != NULL )
						{
							// �`���񂪂���g
							*_pResult = TRUE;
							return TRUE;
						}
						ISelcEditFp* pIEdit = GetSubItemEditControl(item.iItem, item.iSubItem);
						if ( pIEdit == NULL )
						{
							*_pResult = TRUE;
							return TRUE;
						}
						CEdit* pEdit = pIEdit->GetEdit();
						m_ViewSubItem(m_currentSubItem);
						CRect rect;
						_super::GetSubItemRect(item.iItem, m_currentSubItem, LVIR_LABEL, rect);
						CSize sz(max(rect.Width() + 2, 50), rect.Height() + 3);
						CRect clRc;
						GetClientRect(clRc);
						CPoint pos = rect.TopLeft();
						pos.x -= 2;
						pos.y -= 2;
						if ( pos.x + sz.cx > clRc.right )
						{
							pos.x = clRc.right - sz.cx;
						}
						if ( pos.x < 0 )
						{
							pos.x = 0;
						}
						CEdit* pNowEdit = _super::GetEditControl();
						pNowEdit->SetWindowText(GetItemText(item.iItem, m_currentSubItem));
						pIEdit->SetStartRect(CRect(pos, sz), clRc.Width());
						pEdit->SubclassWindow(pNowEdit->m_hWnd);
						m_pSubclassEdit = pIEdit;
					}
					return FALSE;
				}
				else if ( lpLv->hdr.code == LVN_ENDLABELEDIT )
				{
					if ( m_pSubclassEdit != NULL )
					{
						CEdit* pEd = m_pSubclassEdit->GetEdit();
						LVITEM& item = lpLv->item;
						if ( m_editingSubItem >= 0 && pEd->GetSafeHwnd() != NULL ) 
						{
							item.iSubItem = m_editingSubItem;
							m_editingSubItem = -1;
							if ( m_isAutoEdit && (item.mask & LVIF_TEXT) != 0 )
							{
								m_pSubclassEdit->OnEditEnd();
								CString s;
								pEd->GetWindowText(s);
								_super::SetItemText(item.iItem, item.iSubItem, s);
							}
							pEd->UnsubclassWindow();
							m_pSubclassEdit = NULL;
						}
					}
					return FALSE;
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
		case CMySelectDrawer::WM_SELC_DRAWINGSUBITEM:
			return m_drawingSubItem;
			break;
		case WM_KEYDOWN:
			if ( wParam == VK_F2 )
			{
				if ( m_currentSubItem >= 0 )
				{
					_super::EditLabel(_super::GetSelectedItem());
				}
			}
			else if ( wParam == VK_RIGHT || wParam == VK_LEFT )
			{
				CWnd* P = _super::GetParent();
				if ( P != NULL )
				{
					// �e�ɒʒm
					NMLVKEYDOWN nm;
					nm.hdr.code = LVN_KEYDOWN;
					nm.hdr.hwndFrom = _super::GetSafeHwnd();
					nm.hdr.idFrom = _super::GetDlgCtrlID();
					nm.wVKey = static_cast<WORD>(wParam);
					nm.flags = 0;
					P->SendMessage(WM_NOTIFY, nm.hdr.idFrom, reinterpret_cast<LPARAM>(&nm));
				}
				CHeaderCtrl* H = _super::GetHeaderCtrl();
				int m = H->GetItemCount();
				int c = m_currentSubItem;
				if ( wParam == VK_RIGHT )
				{
					if ( m_currentSubItem < 0 )
					{
						m_currentSubItem = 0;
					}
					else if ( m_currentSubItem < m - 1 )
					{
						m_currentSubItem++;
					}
				}
				else //if ( wParam == VK_LEFT )
				{
					if ( m_currentSubItem < 0 )
					{
						m_currentSubItem = m - 1;
					}
					else if ( m_currentSubItem > 0 )
					{
						m_currentSubItem--;
					}
				}
				if ( c != m_currentSubItem )
				{
					m_ViewSubItem(m_currentSubItem);
					CRect rc;
					_super::GetItemRect(GetSelectedItem(), rc, LVIR_BOUNDS);
					_super::InvalidateRect(rc, FALSE);
				}
				return 0;
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				LVHITTESTINFO ht;
				ht.pt = CPoint(lParam);
				if ( _super::SubItemHitTest(&ht) >= 0 )
				{
					if ( m_currentSubItem != ht.iSubItem )
					{
						m_currentSubItem = ht.iSubItem;
						DWORD ff = (_super::GetStyle() & LVS_EDITLABELS);	//�ҏW�X�^�C����Ԏ擾
						_super::ModifyStyle(ff, 0);	//�ҏW�X�^�C��OFF�B
						LRESULT r = _super::WindowProc(message, wParam, lParam);
						_super::ModifyStyle(0, ff);	//�ҏW�X�^�C�����ցB
						CRect rc;
						_super::GetItemRect(ht.iItem, rc, LVIR_BOUNDS);
						_super::InvalidateRect(rc, FALSE);
						return r;
					}
				}
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
		_super::PreSubclassWindow();
		_super::ModifyStyle(0, LVS_SINGLESEL);
		_super::SetExtendedStyle(LVS_EX_FULLROWSELECT | _super::GetExtendedStyle());
		_super::SetAdjustWidthMode(false);
		SetDefaultMarkDrawer();
	}

private:
	/// [�ݒ�] �A�C�e�����x���ҏW�J�n.
	//	@note �g�p�֎~
	CEdit* EditLabel(int item);

	/// �w��̃T�u�A�C�e����������悤�ɂ���
	void m_ViewSubItem(int subItem)
	{
		CRect rc;
		//�����Ă���͈�
		GetClientRect(rc);
		int cx1 = rc.Width();
		int x1 = GetScrollPos(SB_HORZ);
		//���������A�C�e���͈̔�
		CHeaderCtrl* H = _super::GetHeaderCtrl();
		H->GetItemRect(subItem, rc);
		int cx2 = rc.Width();
		int x2 = rc.left;
		//����
		int xx = x1;
		if ( (x1 + cx1) < (x2 + cx2) )
		{
			xx = (x2 + cx2) - cx1;
		}
		if ( x1 > x2 )
		{
			xx = x2;
		}
		Scroll(CSize(xx - x1, 0));
	}
	/// �I��`��
	class CMySelectDrawer : public CSelectedBeltDrawer
	{
		DEFSUPER(CSelectedBeltDrawer);
	public:
		enum
		{
			WM_SELC_DRAWINGSUBITEM = WM_APP,
		};
		CMySelectDrawer(CSubEditListCtrl* H, COLORREF beltColor, COLORREF backColor, bool isTextInvert = true)
			: m_pCtrl(H), _super(beltColor, backColor, isTextInvert)
		{
		}
		// [�쐬] �N���[���쐬.
		virtual IDrawable* Clone(void) const
		{
			CMySelectDrawer* P = new CMySelectDrawer(m_pCtrl, 0, 0, false);
			*P = *this;
			return P;
		}
		// [�`��] �`��.
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			if ( m_pCtrl->SendMessage(WM_SELC_DRAWINGSUBITEM) == m_pCtrl->GetSelectedSubItem() )
			{
				_super::Draw(dc, x , y);
			}
		}
	private:
		CSubEditListCtrl*	m_pCtrl; ///< �e�R���g���[��
	};

	/// �t�H�[�J�X�`��
	class CMyFoucsDrawer : public CNullDrawer
	{
	public:
		CMyFoucsDrawer(CSubEditListCtrl* H) : m_pCtrl(H)
		{
		}
		// [�쐬] �N���[���쐬.
		virtual IDrawable* Clone(void) const
		{
			return new CMyFoucsDrawer(m_pCtrl);
		}
		// [�`��] �`��.
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			CHeaderCtrl* H = m_pCtrl->GetHeaderCtrl();
			if ( H != NULL )
			{
				CRect rc;
				H->GetItemRect(m_pCtrl->GetSelectedSubItem(), rc);
				rc.OffsetRect(x, y);
				rc.bottom = rc.top + m_size.cy;
				::DrawFocusRect(dc, rc);
			}
		}
	private:
		CSubEditListCtrl*	m_pCtrl; ///< �e�R���g���[��
	};

	class CMyEdit : public CSelcEditFpT<CEdit>
	{
	};

	CMyEdit			m_myEdit;			///< ���O��Edit�R���g���[��
	ISelcEditFp*	m_pSubclassEdit;	///< �T�u�N���X�����Ă���Edit�R���g���[��
	bool			m_isAutoEdit;		///< �����e�L�X�g�X�V���[�h
	int				m_currentSubItem;	///< �I�𒆃T�u�A�C�e��No
	int				m_drawingSubItem;	///< �`�撆�T�u�A�C�e��No
	int				m_editingSubItem;	///< �ҏW���̃T�u�A�C�e��No
};



}; // MFC
}; // TNB
