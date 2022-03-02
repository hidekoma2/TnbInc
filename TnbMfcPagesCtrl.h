#pragma once
/**
 *@file
 * �y�[�W�R���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �y�[�W�R���g���[��
 *
 *		�_�C�A���O��̖{�R���g���[�����ɁA�����̉�ʂ������A�y�[�W�̂悤�ɐؑւ�����y�[�W�R���g���[���ł��B
 *
 *		�{�R���g���[���̑傫���ɉ����āA���̃_�C�A���O�����T�C�Y����܂��B
 *	
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		�y�[�W�ƂȂ��ʂ͕��ʂɃ_�C�A���O�Ƃ��č쐬���܂��i�X�^�C���̓`���C���h�ɂ��Ă����j�B
 *		�_�C�A���O�� CDialog �̃T�u�N���X���쐬���A AddItem() �Œǉ����Ă����܂��B\n
 *		�\��������ʂ� SetItemSel() �Ŏw�肵�܂��B
 *
 *	@note �E�B���h�E�� AddItem() ���ɃN���G�C�g���܂��B ���̍� WM_INITDIALOG ���b�Z�[�W��ʒm���܂��B
 *
 *	@note �\����ʐؑւ����A�V���ɕ\��������ʂ� WM_ACTIVATE ���b�Z�[�W�i�p�����^�� WA_ACTIVE �j��ʒm���܂��B
 *		���܂ŕ\�����Ă�����ʂɂ� WM_ACTIVATE ���b�Z�[�W�i�p�����^�� WA_INACTIVE �j��ʒm���܂��B
 *
 *	@par�g�p��
 *		\code
 *		[.h �t�@�C��]
 *
 *		class CFooDlg : public CDialog {
 *				;
 *			CPagesCtrl		m_pageTab;			// �y�[�W�R���g���[��
 *			CFooAbcSubDlg	m_subFooAbcDlg;		// �^�u���T�u�_�C�A���O
 *			CFooXyzSubDlg	m_subFooXyzDlg;		// �^�u���T�u�_�C�A���O
 *
 *		[.cpp �t�@�C��]
 *		
 *		BOOL CFooDlg::OnInitDialog() {
 *				;
 *			m_pageTab.AddItem(&m_subFooAbcDlg); //�T�u�_�C�A���O�́ACreate���Ȃ��ŃC���X�^���X��n���B
 *			m_pageTab.AddItem(&m_subFooXyzDlg);
 *
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPagesCtrl.h
 * 
 *	@date 09/11/09	�V�K�쐬
 *	@date 10/03/23	Z�I�[�_�[�A����
 *	@date 11/05/20	GetItemSel() , GetWndItemSel() �ǉ��B
 *	@date 11/06/07	GetWndItem() �ǉ��B
 *	@date 11/06/28	�R���g���[���̃I�[�_�[�����ɂȂ��Ă��܂������C��
 */
class CPagesCtrl : public CStatic
{
	DEFSUPER(CStatic);
public:

	/// �R���X�g���N�^
	CPagesCtrl(void) : m_currentIndex(-1)
	{
	}

	/**
	 * [�ǉ�] �A�C�e���ǉ�.
	 *	@param pWnd �y�[�W�ɂȂ� CDialog �̃T�u�N���X�̃C���X�^���X�B
	 *				�{�N���X�� Create ����̂ŁA��� Create ���Ȃ��ł��������B
	 *				�܂��ApWnd �{�C���X�^���X�ŕێ����Ȃ��̂ŁA�j�����Ȃ��ł��������B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �ǉ������B�l�͒ǉ������A�C�e��No�B
	 */
	template<typename TYP>
	int AddItem(TYP* pWnd)
	{
		return AddItem(pWnd, TYP::IDD);
	}

	/**
	 * [�ǉ�] �A�C�e���ǉ�.
	 *	@param pWnd �y�[�W�ɂȂ� CDialog �̃T�u�N���X�̃C���X�^���X�B
	 *				�{�N���X�� Create ����̂ŁA��� Create ���Ȃ��ł��������B
	 *				�܂��ApWnd �{�C���X�^���X�ŕێ����Ȃ��̂ŁA�j�����Ȃ��ł��������B
	 *	@param idd �y�[�W�ɂȂ� CDialog �̃��\�[�XID�B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �ǉ������B�l�͒ǉ������A�C�e��No�B
	 */
	int AddItem(CDialog* pWnd, UINT idd)
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
		P->SetWindowPos(this, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		this->SetWindowPos(P, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		DWORD style = P->GetStyle();
		if ( (style & WS_CHILD) == 0 )
		{
			ASSERT0( false, "AddItem", "�_�C�A���O�̃��\�[�X�ɁA�`���C���h�̃X�^�C��������܂���B" );
			P->DestroyWindow();
			return -1;
		}
		P->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
		P->ModifyStyle(DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU, 0, SWP_FRAMECHANGED);
		int r = m_apPages.GetSize();
		m_apPages.Add(P);
		m_ResizeAll();
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
	 * [�擾] �A�C�e�����擾.
	 *	@note AddItem() ���������擾�ł��܂��B
	 *	@return �A�C�e����.
	 */
	size_t GetItemCount(void) const
	{
		return m_apPages.GetSize();
	}

	/**
	 * [�擾] �I���A�C�e���擾.
	 *		SetItemSel() ���ꂽ�A�C�e���̃C���f�b�N�X��Ԃ��܂��B
	 *	@retval 0�ȏ� �I������Ă���A�C�e���̃C���f�b�N�X�B
	 *	@retval �}�C�i�X �I������Ă��Ȃ��B
	 */
	int GetItemSel(void) const
	{
		return m_currentIndex;
	}

	/**
	 * [�擾] �I���A�C�e���擾.
	 *		SetItemSel() ���ꂽ�A�C�e���� CWnd �̃|�C���^��Ԃ��܂��B
	 *		���̎��̂́A AddItem() �œo�^���ꂽ�C���X�^���X�ɂȂ��Ă��܂��B
	 *	@retval NULL�ȊO �����B�l�� �I������Ă���A�C�e���� CWnd �̃|�C���^
	 *	@retval NULL ���s�B�I������Ă��Ȃ��B
	 */
	CWnd* GetWndItemSel(void) const
	{
		return GetWndItem(m_currentIndex);
	}

	/**
	 * [�I��] �A�C�e���I��.
	 *	@param nItem �I������A�C�e���̃C���f�b�N�X
	 *	@retval �}�C�i�X ���s�B
	 *	@retval 0�ȏ� �����B�l�͒��O�ɑI������Ă����C���f�b�N�X�B
	 */
	int SetItemSel(int nItem) 
	{
		int r = m_currentIndex;
		m_SetCurSel(nItem);
		m_currentIndex = nItem;
		return r;
	}

	/**
	 * [�I��] �^�u�I��.
	 *	@param page �E�B���h�E�n���h���B
	 *	@retval �}�C�i�X ���s�B
	 *	@retval 0�ȏ� �����B�l�͒��O�ɑI������Ă����C���f�b�N�X�B
	 */
	int SetItemSel(HWND page)
	{
		return SetItemSel(FindItem(page));
	}

	/**
	 * [�폜] �A�C�e���폜.
	 *	@param nItem �A�C�e��No�B
	 *	@retval true �폜�����B
	 *	@retval false ���s�B
	 */
	bool DeleteItem(int nItem)
	{
		if ( m_apPages.IsInRange(nItem) )
		{
			m_apPages[nItem]->DestroyWindow();
			m_apPages.Remove(nItem);
			return true;
		}
		return false;
	}

	/**
	 * [�폜] �S�A�C�e���폜.
	 *	@note �o�^�����E�B���h�E�͔j������܂��B
	 */
	void DeleteAllItems(void)
	{
		loop ( i, m_apPages.GetSize() )
		{
			m_apPages[i]->DestroyWindow();
		}
		m_apPages.RemoveAll();
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
		m_currentIndex = -1;
		_super::ShowWindow(SW_HIDE);
		_super::PreSubclassWindow();
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
		case WM_SIZE:
			m_ResizeAll();
			break;
		case WM_DESTROY:
			DeleteAllItems();
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	int m_currentIndex;

	/// �I��
	void m_SetCurSel(int idx)
	{
		if ( m_apPages.IsInRange(idx) )
		{
			m_apPages[idx]->ShowWindow(SW_SHOW);
		}
		loop ( i, m_apPages.GetSize() )
		{
			if ( idx != ToInt(i) )
			{
				if ( m_apPages[i]->IsWindowVisible() )
				{
					m_apPages[i]->ShowWindow(SW_HIDE);
					m_apPages[i]->SendMessage(WM_ACTIVATE, WA_INACTIVE, 0);
				}
			}
		}
		CWnd* pFocus = GetFocus();
		if ( pFocus != NULL )
		{
			CWnd* P = pFocus->GetParent();
			if ( P != NULL && ! P->IsWindowVisible() )
			{
				_super::SetFocus();
			}
		}
		if ( m_apPages.IsInRange(idx) )
		{
			m_apPages[idx]->SendMessage(WM_ACTIVATE, WA_ACTIVE, 0);
		}
	}
	/// �q���S�����T�C�Y
	void m_ResizeAll(void)
	{
		CRect r;
		_super::GetWindowRect(&r);
		_super::GetParent()->ScreenToClient(&r);	// �N���C�A���g���W�n�ɏC��
		loop ( i, m_apPages.GetSize() )
		{
			m_apPages[i]->MoveWindow(r.left, r.top, r.Width(), r.Height());
		}
	}
};



}; // MFC
}; // TNB
