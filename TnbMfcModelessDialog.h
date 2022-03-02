#pragma once
/**
 *@file
 * ���[�h���X�_�C�A���O�֌W�w�b�_ �t�@�C��
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ���[�h���X�E�B���h�E�Ǘ��e���v���[�g.
 *	
 *		�w��̃_�C�A���O�x�[�X�E�B���h�E�� Modeless �ŕ\�����܂��B
 *		Show() , Hide() , Destroy() ���\�b�h�ɂ��e�Ղɕ\��(�쐬)�A��\���A�j���ł���悤�ɂȂ�܂��B
 *
 *	@note �쐬�����E�B���h�E�̐e�E�B���h�E�� NULL �ł��B
 *
 *	@note �e�E�B���h�E�̕\����\�����j���[ID�� SetParentMenu() �Ŏw�肷��ƁAShow Hide �ɂ�莩���I�ɕ\���`�F�b�N�� on off ���܂��B
 *			�������A���j���[�A�C�e���I������ Show Hide �͎������ł��܂���B�e�_�C�A���O�̃n���h���� 
 *			xxx.Toggle(); �Ƃ��Ă��������B
 *			\code
 *				// CModelessDialogT<CSybDlg> m_subDlg;
 *				void CFooDlg::OnIniDialog(void)
 *				{
 *						;
 *					m_subDlg.SetParentMenu(GetSafeHwnd(), ID_MENU_LIST_VIEW);
 *					m_subDlg.Hide(); //��\���ō쐬���Ă����ꍇ�B
 *						;
 *				}
 *				void CFooDlg::OnMenuListView(void)
 *				{
 *					m_listDlg.Toggle();
 *				}
 *			\endcode
 *			�� CE �ł� SetParentMenu() �͎g���܂���B
 *
 *	@note operator->() ������̂ŁA DLG �̃����o�ɒ��ڃA�N�Z�X���\�ł��B
 *			\code
 *				// CModelessDialogT<CSybDlg> m_subDlg;
 *				void CFooDlg::Foo(void)
 *				{
 *						;
 *					HICON hIcon = m_subDlg->GetIcon();
 *						;
 *				}
 *			\endcode
 *
 *	@param DLG MFC �� CDialog ���p�������N���X�^
 *	@param HASTASK true �Ȃ�^�X�N�o�[�Ɍ���܂�(def.)�B false �Ȃ猻��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcModelessDialog.h
 *
 *	@date 06/09/20 �V�K�B
 *	@date 07/10/03 �ȗ����B
 *	@date 08/07/15 Hide�ł͔j��������\���ɂȂ�悤�ɂ��ADestory��ʓr�p�ӁB
 *	@date 09/08/03 SetParentMenu() , Toggle() ��ǉ��B
 *	@date 09/08/19 operator HWND() ��ǉ��B
 *	@date 10/01/06 ���鏈�����P�B���鎞�Ƀ��b�Z�[�W�𓊂�����悤�ɂ����B
 *	@date 10/08/05 �e�E�B���h�E�� NULL �Ƃ����B
 *	@date 10/09/01 HASTASK �ǉ�
 *	@date 11/06/09 CE �ɑΉ��B
 *	@date 11/08/31 CModelessDialogT �ɉ����B������ typedef �Ŏc���B
 *	@date 12/03/16 DisableWmClose() ��V�K�B
 */
template<typename DLG, bool HASTASK = true>
class CModelessDialogT
{
public:
	
	/// �R���X�g���N�^
	CModelessDialogT(void)
	{
	}

	/// �f�X�g���N�^
	~CModelessDialogT(void)
	{
		Destroy();
	}

	/**
	 * [�ݒ�] �e���j���[�ݒ�.
	 *	@attention CE �ł́A���̃��\�b�h�̓_�~�[�ł��i�@�\���܂���j�B
	 *	@param hWnd �e�E�B���h�E.
	 *	@param menuItem �{�_�C�A���O�� Show / Hide ���Ǘ�����e�E�B���h�E�̃��j���[ID�B
	 *	@param msg �������ɁA�ʒm���郁�b�Z�[�W�B
	 */
	void SetParentMenu(HWND hWnd, UINT menuItem, UINT msg = 0)
	{
		m_dlg.SetParentMenu(hWnd, menuItem, msg);
	}

	/**
	 * [�擾] �\�����
	 *	@retval true �\����
	 *	@retval false ��\��
	 */
	bool IsViewing(void) const
	{
		return IsVisible();
	}
	
	/**
	 * [�擾] �\�����
	 *	@retval true �\����
	 *	@retval false ��\��
	 */
	bool IsVisible(void) const
	{
		return ::IsWindow(m_dlg.m_hWnd) && ::IsWindowVisible(m_dlg.m_hWnd);
	}
	
	/**
	 * [�ݒ�] �\��.
	 *		�E�B���h�E��\�����܂��B
	 *	@note �E�B���h�E���쐬����Ă��Ȃ��Ƃ��A�E�B���h�E���쐬���܂��B
	 */
	void Show(void)
	{
		m_CreateDlg();
		m_dlg.ShowWindow(SW_SHOW);
	}

	/**
	 * [�ݒ�] ��\��.
	 *		�E�B���h�E���\���ɂ��܂��B
	 *	@note �E�B���h�E���쐬����Ă��Ȃ��Ƃ��A�E�B���h�E���쐬���܂��B
	 *	@note �E�B���h�E�͔j������܂���B�j������ꍇ�A Destroy() ���g�p���܂��B
	 */
	void Hide(void)
	{
		m_CreateDlg();
		if ( IsViewing() )
		{
			m_dlg.ShowWindow(SW_HIDE);
		}
	}

	/**
	 * [�ݒ�] �\��/��\��.
	 *		�\�����Ă��Ȃ���΁A�\�����܂��B�\�����Ă���΁A��\���ɂ��܂��B
	 */
	void Toggle(void)
	{
		if ( IsViewing() )
		{
			Hide();
		}
		else
		{
			Show();
		}
	}

	/**
	 * [�ݒ�] �j��.
	 *		�E�B���h�E��j�����܂��B
	 */
	void Destroy(void)
	{
		if ( ::IsWindow(m_dlg.m_hWnd) )
		{
			m_dlg.DestroyWindow();
		}
	}

	/**
	 * [�擾] �_�C�A���O�N���X�Q��
	 *	@return �_�C�A���O�N���X�̎Q��
	 */
	const DLG* operator->(void) const
	{
		return &m_dlg;
	}

	/**
	 * [�擾] �_�C�A���O�N���X�Q��
	 *	@return �_�C�A���O�N���X�̎Q��
	 */
	DLG* operator->(void)
	{
		return &m_dlg;
	}

	/**
	 * [�擾] �E�B���h�E�n���h���擾
	 *	@return �E�B���h�E�n���h��
	 */
	operator HWND(void)
	{
		return m_dlg;
	}

	/**
	 * [�ݒ�] WM_CLOSE �����֎~.
	 *		�f�t�H���g�ł́A WM_CLOSE ����M����� HIDE �ɂȂ�悤�ɂȂ��Ă��܂��B
	 *		�{���\�b�h�͂�����֎~���܂��i�������Ȃ��Ȃ�܂��j�B
	 */
	void DisableWmClose(void)
	{
		m_dlg.DisableWmClose();
	}

private:
	void m_CreateDlg(void)
	{
		if ( ! ::IsWindow(m_dlg.m_hWnd) )
		{
			m_dlg.CreateDlg(DLG::IDD, HASTASK ? CWnd::GetDesktopWindow() : NULL);
		}
	}
	/// �����N���X
	class CInner : public DLG
	{
		DEFSUPER(DLG);
	public:
		/// �R���X�g���N�^
		CInner(void) : m_hParentWnd(NULL)
			, m_parentMenuItem(0), m_parentMsg(0), m_isPosChanging(false), m_isCreating(false), m_isEnableWmClose(true)
		{
		}
		void CreateDlg(DWORD dw, CWnd* pParent)
		{
			if ( ! ::IsWindow(_super::m_hWnd) )
			{
				m_isCreating = true;
				_super::Create(dw, pParent);
//				DWORD mov = WS_CHILD | WS_BORDER | WS_DLGFRAME | WS_VISIBLE;
//				DWORD add = WS_POPUP | WS_CAPTION;
				DWORD mov = WS_CHILD | WS_VISIBLE;
				DWORD add = WS_POPUP;
				_super::ModifyStyle(mov, add, SWP_FRAMECHANGED);
				m_isCreating = false;
			}
		}
		/**
		 * [�ݒ�] �e���j���[�ݒ�.
		 *	@param hWnd �e�E�B���h�E.
		 *	@param menuItem �{�_�C�A���O�� Show / Hide ���Ǘ�����e�E�B���h�E�̃��j���[ID�B
		 *	@param msg �������ɁA�ʒm���郁�b�Z�[�W�B
		 */
		void SetParentMenu(HWND hWnd, UINT menuItem, UINT msg)
		{
			m_hParentWnd = hWnd;
			m_parentMenuItem = menuItem;
			m_parentMsg = msg;
		}
		/// WM_CLOSE �����֎~
		void DisableWmClose(void)
		{
			m_isEnableWmClose = false;
		}
	protected:
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
#ifndef _WIN32_WCE
			case WM_WINDOWPOSCHANGING:
				if ( ! m_isCreating )
				{
					m_isPosChanging = true;
				}
				break;
			case WM_WINDOWPOSCHANGED:
				if ( m_isPosChanging )
				{
					m_isPosChanging = false;
					WINDOWPOS* P = reinterpret_cast<WINDOWPOS*>(lParam);
					if ( (P->flags & SWP_HIDEWINDOW) != 0 )
					{
						m_NotifyParent(0);
					}
				}
				break;
			case WM_SHOWWINDOW:
				if ( ! m_isCreating )
				{
					m_isPosChanging = false;
					m_NotifyParent(wParam);
				}
				break;
#endif // _WIN32_WCE
			case WM_CLOSE:
				if ( m_isEnableWmClose )
				{
					ShowWindow(SW_HIDE);
				}
//				return true;
				break;
			}
			return _super::WindowProc(message, wParam, lParam);
		}
	private:
		HWND	m_hParentWnd;
		UINT	m_parentMenuItem;
		UINT	m_parentMsg;
		bool	m_isPosChanging;
		bool	m_isCreating;
		bool	m_isEnableWmClose;
#ifndef _WIN32_WCE
		void m_NotifyParent(WPARAM wParam)
		{
			if ( m_parentMenuItem != 0 )
			{
				HMENU h = ::GetMenu(m_hParentWnd);
				if ( h != NULL )
				{
					UINT r = (wParam != 0) ? MF_CHECKED : MF_UNCHECKED;
					::CheckMenuItem(h, m_parentMenuItem, r);
				}
			}
			if ( m_parentMsg != 0 && wParam == 0 )
			{
				::PostMessage(m_hParentWnd, m_parentMsg, 0, 0);
			}
		}
#endif // _WIN32_WCE
	};
	CInner	m_dlg;				///< �Ώ�Window
};



}; // MFC
}; // TNB
