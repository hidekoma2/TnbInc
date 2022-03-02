#pragma once
/**
 *@file
 * �y�[�W�t���^�u�R���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �y�[�W�t���^�u�R���g���[���e���v���[�g
 *
 *		�^�u�̒��ɁA�w��̃_�C�A���O�������Ƃ��o����^�u�R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń^�u�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCTabCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B
 *		CDialog �̃T�u�N���X���쐬���A AddItem �Œǉ����邾���ł�
 * 
 *	@note �E�B���h�E�� AddItem() ���ɃN���G�C�g���܂��B ���̍� WM_INITDIALOG ���b�Z�[�W��ʒm���܂��B
 *
 *	@note �^�u�ؑւ����A�V���ɕ\��������ʂ� WM_ACTIVATE ���b�Z�[�W�i�p�����^�� WA_ACTIVE �j��ʒm���܂��B
 *		���܂ŕ\�����Ă�����ʂɂ� WM_ACTIVATE ���b�Z�[�W�i�p�����^�� WA_INACTIVE �j��ʒm���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPagesTabCtrl.h
 * 
 *	@date 09/04/06	�V�K�쐬
 *	@date 09/04/17	TCM_SETCURSEL �ɂ��Ή��B
 *	@date 09/05/26	CE�p�ł����삷��悤�ɑΉ��B
 *	@date 09/06/16	WM_ACTIVATE ���b�Z�[�W�Ή��B
 *	@date 10/03/23	Z�I�[�_�[�A����
 *	@date 10/04/15	�e���v���[�g��
 *	@date 11/05/26	GetWndCurSel() �ǉ��B
 *	@date 11/06/07	GetWndItem() �ǉ��B
 *	@date 19/06/27  WM_MOVE ���b�Z�[�W�ɂ��Ή��B
 */
template<typename TAB = CTabCtrl>
class CPagesTabCtrlT : public TAB
{
	DEFSUPER(TAB);
public:

	/**
	 * [�ǉ�] �A�C�e���ǉ�.
	 *	@param lpszName �A�C�e���l�[���i�^�u���j�B
	 *	@param pWnd �y�[�W�ɂȂ� CDialog �̃T�u�N���X�̃C���X�^���X�i�N���G�C�g���Ȃ����Ɓj�B
	 *				�{�C���X�^���X�ŕێ����Ȃ��̂ŁA�j�����Ȃ��ł��������B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �ǉ������B�l�͒ǉ������A�C�e��No�B
	 */
	template<typename TYP>
	int AddItem(LPCTSTR lpszName, TYP* pWnd)
	{
		return AddItem(lpszName, pWnd, TYP::IDD);
	}

	/**
	 * [�ǉ�] �A�C�e���ǉ�.
	 *	@param lpszName �A�C�e���l�[���i�^�u���j�B
	 *	@param pWnd �y�[�W�ɂȂ� CDialog �̃T�u�N���X�̃C���X�^���X�i�N���G�C�g���Ȃ����Ɓj�B
	 *				�{�C���X�^���X�ŕێ����Ȃ��̂ŁA�j�����Ȃ��ł��������B
	 *	@param idd �y�[�W�ɂȂ� CDialog �̃��\�[�XID�B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �ǉ������B�l�͒ǉ������A�C�e��No�B
	 */
	int AddItem(LPCTSTR lpszName, CDialog* pWnd, UINT idd)
	{
		CWnd* pParent = GetParent();
		CDialog* P = pWnd;
		if ( ::IsWindow(P->GetSafeHwnd()) )
		{
			ASSERT0( false, "AddItem", "���ɃN���G�C�g����Ă���Object���w�肳��܂����B" );
			return -1;
		}
		if ( ! P->Create(idd, pParent) )
		{
			ASSERT0( false, "AddItem", "�N���G�C�g�Ɏ��s���܂����B" );
			return -1;
		}
		P->SetParent(pParent);
		this->SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		P->SetWindowText(lpszName);
		DWORD style = P->GetStyle();
		if ( (style & WS_CHILD) == 0 )
		{
			ASSERT0( false, "AddItem", "�_�C�A���O�̃��\�[�X�ɁA�`���C���h�̃X�^�C��������܂���B" );
			P->DestroyWindow();
			return -1;
		}
		P->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
		P->ModifyStyle(DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU, 0, SWP_FRAMECHANGED);
		int i = _super::GetItemCount();
		int r = _super::InsertItem(i, lpszName);
		if ( r >= 0 )
		{
			m_apPages.Add(P);
			int o = _super::GetCurSel();
			if ( o < 0 )
			{
				o = 0;
			}
			_super::SetCurSel(o);
			m_ResizeAll();
		}
		else
		{
			P->DestroyWindow();
		}
		return r;
	}
	
	/**
	 * [����] �A�C�e������.
	 *	@note �w��̃E�B���h�E�n���h��(�A�C�e��)�������Ă���A�C�e��No��Ԃ��܂��B
	 *	@param page �E�B���h�E�n���h��
	 *	@retval �}�C�i�X �Ȃ������B
	 *	@retval 0�ȏ� ���������B�l�͔��������A�C�e��No�B
	 */
	int FindItem(HWND page) const
	{
		loop ( i, m_apPages.GetSize() )
		{
			if ( m_apPages[i]->GetSafeHwnd() == page )
			{
				return i;
			}
		}
		return -1;
	}

	/**
	 * [�擾] �E�B���h�E�擾.
	 *		�w��̃C���f�b�N�X�ɓo�^���ꂽ CWnd �̃|�C���^��Ԃ��܂��B
	 *		���̎��̂́A AddItem() �œo�^���ꂽ�C���X�^���X�ɂȂ��Ă��܂��B
	 *	@param nItem �擾����A�C�e���̃C���f�b�N�X
	 *	@retval NULL�ȊO �����B�l�� CWnd �̃|�C���^
	 *	@retval NULL ���s�B�I������Ă��Ȃ��B
	 */
	CWnd* GetWndItem(int nItem) const
	{
		if ( m_apPages.IsInRange(nItem) )
		{
			return m_apPages[nItem];
		}
		return NULL;
	}

	/**
	 * [�I��] �^�u�I��.
	 *	@param nItem �I������A�C�e���̃C���f�b�N�X
	 *	@retval �}�C�i�X ���s�B
	 *	@retval 0�ȏ� �����B�l�͒��O�ɑI������Ă����C���f�b�N�X�B
	 */
	int SetCurSel(int nItem) 
	{
		return _super::SetCurSel(nItem); 
	}

	/**
	 * [�I��] �^�u�I��.
	 *	@param page �E�B���h�E�n���h���B
	 *	@retval �}�C�i�X ���s�B
	 *	@retval 0�ȏ� �����B�l�͒��O�ɑI������Ă����C���f�b�N�X�B
	 */
	int SetCurSel(HWND page)
	{
		return _super::SetCurSel(FindItem(page));
	}

	/**
	 * [�擾] �I���A�C�e���擾.
	 *		SetCurSel() �őI�����ꂽ�A�C�e���̃C���f�b�N�X��Ԃ��܂��B
	 *	@retval 0�ȏ� �I������Ă���A�C�e���̃C���f�b�N�X�B
	 *	@retval �}�C�i�X �I������Ă��Ȃ��B
	 */
	int GetCurSel(void) const 
	{
		return _super::GetCurSel();
	}

	/**
	 * [�擾] �I���E�B���h�E�擾. 
	 *		SetCurSel() �őI�����ꂽ�A�C�e���ɓo�^����Ă��� CWnd �̃|�C���^��Ԃ��܂��B 
	 *		���̎��̂́A AddItem() �œo�^���ꂽ�C���X�^���X�ɂȂ��Ă��܂��B
	 *	@retval NULL�ȊO �����B�l�� CWnd �̃|�C���^
	 *	@retval NULL ���s�B�I������Ă��Ȃ��B
	 */
	CWnd* GetWndCurSel(void) const
	{
		return GetWndItem(_super::GetCurSel());
	}

	/**
	 * [�폜] �A�C�e���폜.
	 *	@param nItem �A�C�e��No�B
	 *	@retval TRUE �폜�����B
	 *	@retval FALSE ���s�B
	 */
	BOOL DeleteItem(int nItem)
	{
		if ( m_apPages.IsInRange(nItem) )
		{
			m_apPages[nItem]->DestroyWindow();
			m_apPages.Remove(nItem);
			return _super::DeleteItem(nItem);
		}
		return FALSE;
	}

	/**
	 * [�폜] �S�A�C�e���폜.
	 *	@retval TRUE �폜�����B
	 *	@retval FALSE ���s�B
	 */
	BOOL DeleteAllItems(void)
	{
		loop ( i, m_apPages.GetSize() )
		{
			if ( ::IsWindow(m_apPages[i]->GetSafeHwnd()) )
			{
				m_apPages[i]->DestroyWindow();
			}
		}
		m_apPages.RemoveAll();
		return _super::DeleteAllItems();
	}

protected:

	CSimpleVectorT<CWnd*> m_apPages;	///< �y�[�W�E�B���h�E�Q

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
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
			if ( lpNmHdr->hwndFrom == m_hWnd && lpNmHdr->code == TCN_SELCHANGE )
			{
				m_SetCurSel(_super::GetCurSel());
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
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
		switch ( message )
		{
		case TCM_SETCURSEL:
			m_SetCurSel(wParam);
			break;
		case WM_MOVE:
			m_ResizeAll();
			break;
		case WM_SIZE:
			m_ResizeAll();
			break;
		case WM_DESTROY:
			m_apPages.RemoveAll();
			//DeleteAllItems();
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// �I��
	void m_SetCurSel(int idx)
	{
		ASSERT( ToInt(m_apPages.GetSize()) == _super::GetItemCount() );
		if ( m_apPages.IsInRange(idx) )
		{
			m_apPages[idx]->ShowWindow(SW_SHOW);
			loop ( i, m_apPages.GetSize() )
			{
				if ( idx != ToInt(i) )
				{
					BOOL r = m_apPages[i]->IsWindowVisible();
					m_apPages[i]->ShowWindow(SW_HIDE);
					if ( r )
					{
						m_apPages[i]->SendMessage(WM_ACTIVATE, WA_INACTIVE, 0);
					}
				}
			}
			m_apPages[idx]->SendMessage(WM_ACTIVATE, WA_ACTIVE, 0);
			m_apPages[idx]->SetFocus();
		}
	}
	/// �q���S�����T�C�Y
	void m_ResizeAll(void)
	{
		CRect r;
		_super::GetWindowRect(&r);			// �^�u�R���g���[���̃E�B���h�E�̈�i�X�N���[�����W�n�j
		_super::GetParent()->ScreenToClient(&r);	// �N���C�A���g���W�n�ɏC��
		_super::AdjustRect(FALSE, &r);		// �E�B���h�E�̈�Ɋ�Â��A�\�����̗̈�ɒ���
		loop ( i, m_apPages.GetSize() )
		{
			m_apPages[i]->MoveWindow(r.left, r.top, r.Width(), r.Height());
		}
	}
	BOOL GetItem(int nItem, TCITEM* pTabCtrlItem) const;
	BOOL SetItem(int nItem, TCITEM* pTabCtrlItem);
	BOOL SetItemExtra(int nBytes);
	BOOL InsertItem(int nItem);
	void AdjustRect(BOOL bLarger, LPRECT lpRect);
	void RemoveImage(int nImage);
};



/**@ingroup MFCCONTROL
 * �y�[�W�t���^�u�R���g���[��
 *
 *		�^�u�̒��ɁA�w��̃_�C�A���O�������Ƃ��o����^�u�R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń^�u�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCTabCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B
 *		CDialog �̃T�u�N���X���쐬���A AddItem �Œǉ����邾���ł��B
 *
 *	@note �^�u�ؑւ����AAddItem �Œǉ�������ʃN���X�ɁA WM_ACTIVATE ���b�Z�[�W�� Send���܂��B
 *		���܂őI������Ă�����ʃN���X�� WA_INACTIVE �A�V�����I�����ꂽ��ʃN���X�� WA_ACTIVE ��
 *		�ʒm����܂��B
 *
 *	@par�g�p��
 *		\code
 *		[.h �t�@�C��]
 *
 *		class CFooDlg : public CDialog {
 *				;
 *			CPagesTabCtrl	m_pagesTab;			//�^�u�R���g���[��		
 *			CPageFooAbcDlg	m_pageFooAbcDlg;	//�^�u���T�u�_�C�A���O
 *			CPageFooXyzDlg	m_pageFooXyzDlg;	//�^�u���T�u�_�C�A���O
 *			//�T�u�_�C�A���O�́ACreate���Ȃ����ƁB �X�^�C���̓`���C���h�ɂ��Ă������ƁB
 *
 *		[.cpp �t�@�C��]
 *		
 *		BOOL CFooDlg::OnInitDialog() {
 *				;
 *			m_pagesTab.AddItem("ABC", &m_pageFooAbcDlg);
 *			m_pagesTab.AddItem("XYZ", &m_pageFooXyzDlg);
 *
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPagesTabCtrl.h
 * 
 *	@date 10/04/25 �V�K�錾
 */
class CPagesTabCtrl : public CPagesTabCtrlT<>
{
};



}; // MFC
}; // TNB
