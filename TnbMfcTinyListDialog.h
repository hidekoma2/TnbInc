#pragma once
/**
 *@file
 * List�E�B���h�E�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcColorListBox.h"
#include "TnbMfcLightListBox.h"
#include "TnbMfcDelayedRedraw.h"
#include "TnbDummyWindow.h"
#include "TnbConstStrVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �ȈՃ��X�g�E�B���h�E
 *
 *		�s�P�ʂŐF���w��o���� ListBox�R���g���[������ʈ�ʂɂ����[�h���X�_�C�A���O�ł��B�B
 *
 *	@note �e�E�B���h�E�̕\����\�����j���[ID�� Create() �Ŏw�肷��ƁAShow Hide �ɂ�莩���I�ɕ\���`�F�b�N�� on off ���܂��B
 *			�������A���j���[�A�C�e���I������ Show Hide �͎������ł��܂���B�e�_�C�A���O�̃n���h���� 
 *			xxx.ShowWindow(CTinyListDialog::TOGGLE); �Ƃ��Ă��������B
 *			\code
 *				// CTinyListDialog m_log;
 *				void CFooDlg::OnIniDialog(void)
 *				{
 *						;
 *					m_log.Create(CSize(100, 100), GetSafeHwnd(), "title", m_hIcon, ID_MENU_LIST_VIEW);
 *						;
 *				}
 *				void CFooDlg::OnMenuListView(void)
 *				{
 *					m_log.ShowWindow(CTinyListDialog::TOGGLE);
 *				}
 *			\endcode
 *			���j���[ID���w�肵�Ă��Ȃ��ꍇ�A PostString() �ŕ\�������悤�ɂȂ�܂��B
 *
 *	@see CColorListBox
 *	@see CTinyListDialogEx
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcTinyListDialog.h
 * 
 *	@date 09/04/02	�V�K�쐬
 *	@date 09/04/30	�e�̃��j���[�̐�����T�|�[�g
 *	@date 09/08/06	���j���[��ID�V���{����ύX�B
 *	@date 10/04/19	���j���[�Ǘ����@�C��
 *	@date 11/02/09	�����������[�h�A�ǉ��B
 *	@date 11/10/04	OnCreate() �ǉ��B
 *	@date 12/01/27	OnDestroy() �ǉ��B
 *	@date 15/03/09	PostStringCr() �ǉ��B
 */
class CTinyListDialog : private CDummyWindow::IListener
{
public:

	/// ���j���[ID
	enum EMenuId
	{
		MenuId_Hide = 1000,		///< �B��
		MenuId_Clear = 1001,	///< �N���A
	};

	/// �\�����[�h�^
	enum EShowMode
	{
		SHOW,		///< �\��
		HIDE,		///< ��\��
		TOGGLE,		///< �g�O��
	};

	/// �ړ����[�h�^
	enum EMoveMode
	{
		CENTER,		///< ����
		UNDER,		///< ��
		RIGHT,		///< �E
	};

	/**
	 * �R���X�g���N�^.
	 *	@param isColorMode true �Ȃ�J���[���[�h�B false �Ȃ甒�����[�h�ł��B
	 */
	CTinyListDialog(bool isColorMode = true) : m_hParentWnd(NULL), m_parentMenuItem(0), m_isColorMode(isColorMode)
	{
		if ( isColorMode )
		{
			m_pListBox = &m_logColorList;
		}
		else
		{
			m_pListBox = &m_logBwList;
		}
	}

	/**
	 * [�쐬] �E�B���h�E�쐬.
	 *	@note ��\����Ԃō쐬����܂��B
	 *	@param size �E�B���h�E�傫��
	 *	@param hFont �t�H���g
	 *	@param hParentWnd �e�E�B���h�E
	 *	@param lpszTitle �E�B���h�E�^�C�g��
	 *	@param hIcon �E�B���h�E�A�C�R��
	 *	@param parentMenuItem �e�E�B���h�E�̕\����\�����j���[�B�ȗ��\�B
	 *	@retval true �쐬����
	 *	@retval false ���s
	 */
	bool Create(const CSize& size, HFONT hFont, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		// ���j���[
		if ( m_menu.GetSafeHmenu() == NULL )
		{
			m_subMenu1.CreateMenu();
			m_subMenu2.CreateMenu();
			m_menu.CreateMenu();
			m_subMenu1.AppendMenu(0, MenuId_Hide, _T("�B��(&H)"));
			m_subMenu2.AppendMenu(0, MenuId_Clear, _T("�N���A(&C)"));
			m_menu.AppendMenu(MF_POPUP, reinterpret_cast<UINT_PTR>(m_subMenu1.GetSafeHmenu()), _T("�t�@�C��(&F)"));
			m_menu.AppendMenu(MF_POPUP, reinterpret_cast<UINT_PTR>(m_subMenu2.GetSafeHmenu()), _T("�\��(&H)"));
		}
		if ( ! m_dmyWnd.Create(this, lpszTitle, NULL, (WS_OVERLAPPEDWINDOW & ~WS_MINIMIZEBOX) | WS_CLIPCHILDREN, m_menu) )
		{
			return false;
		}
		m_parentMenuItem = parentMenuItem;
		m_hParentWnd = hParentWnd;
		CWnd* pDmyWnd = CWnd::FromHandle(m_dmyWnd);
		::SetWindowPos(m_dmyWnd, hParentWnd, 0, 0, size.cx, size.cy, 0);
		pDmyWnd->CenterWindow();
		CRect rc(0, 0, size.cx, size.cy);
		DWORD style = LBS_NOINTEGRALHEIGHT | LBS_EXTENDEDSEL | WS_VSCROLL;
		if ( m_isColorMode )
		{
			style |= LBS_OWNERDRAWVARIABLE;;
			style |= LBS_HASSTRINGS;
		}
		else
		{
			style |= LBS_OWNERDRAWFIXED | LBS_NODATA;
		}
		m_pListBox->Create(style, rc, pDmyWnd, 1000);
		m_pListBox->SetFont(CFont::FromHandle(hFont));
		m_pListBox->ShowWindow(SW_SHOW);
		m_pListBox->ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
		pDmyWnd->SendMessage(WM_SETICON, 0, reinterpret_cast<LPARAM>(hIcon));
		pDmyWnd->Detach();
		OnCreate();
		return true;
	}

	/**
	 * [�쐬] �E�B���h�E�쐬.
	 *	@note ��\����Ԃō쐬����܂��B
	 *	@param size �E�B���h�E�傫��
	 *	@param hParentWnd �e�E�B���h�E
	 *	@param lpszTitle �E�B���h�E�^�C�g��
	 *	@param hIcon �E�B���h�E�A�C�R��
	 *	@param parentMenuItem �e�E�B���h�E�̕\����\�����j���[�B�ȗ��\�B
	 *	@retval true �쐬����
	 *	@retval false ���s
	 */
	bool Create(const CSize& size, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hParentWnd, WM_GETFONT, 0, 0));
		return Create(size, hFont, hParentWnd, lpszTitle, hIcon, parentMenuItem);
	}

	/**
	 * [�ړ�] �E�B���h�E�ړ�.
	 *	@param mode �ړ����[�h
	 */
	void MoveWindow(EMoveMode mode = UNDER)
	{
		if ( m_dmyWnd != NULL )
		{
			CRect rc;
			::GetWindowRect(m_hParentWnd, &rc);
			switch ( mode )
			{
			case CENTER:// ����
				{
					CWnd* pDmyWnd = CWnd::FromHandle(m_dmyWnd);
					pDmyWnd->CenterWindow();
					pDmyWnd->Detach();
				}
				break;
			case UNDER:// ��
				::SetWindowPos(m_dmyWnd, NULL, rc.left, rc.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				break;
			case RIGHT:// �E
				::SetWindowPos(m_dmyWnd, NULL, rc.right, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				break;
			}
		}
	}

	/**
	 * [����] �E�B���h�E�̔j��.
	 */
	void Destroy(void)
	{
		m_dmyWnd.Destroy();
	}

	/**
	 * [�擾] �E�B���h�E�n���h��.
	 *		�ێ����Ă���E�B���h�E�̃n���h����Ԃ��܂��B
	 *	@return �n���h��
	 */
	operator HWND(void)
	{
		return m_dmyWnd;
	}

	/**
	 * [�m�F] �E�B���h�E�̗L��.
	 *	@retval true �L��
	 *	@retval false �Ȃ�
	 */
	bool IsWindow(void) const
	{
		return m_dmyWnd.IsWindow();
	}

	/**
	 * [�m�F] �E�B���h�E�\�����.
	 *	@retval true �\����
	 *	@retval false ��\�����i�E�B���h�E���쐬����Ă��Ȃ��ꍇ�� false ���Ԃ�܂��j
	 */
	bool IsWindowVisible(void)
	{
		return !! ::IsWindowVisible(m_dmyWnd);
	}

	/**
	 * [�\��] �\���ؑ�.
	 *	@param mode �\�����[�h
	 */
	void ShowWindow(EShowMode mode = SHOW)
	{
		int m = (mode == SHOW) ? SW_SHOW : SW_HIDE;
		if ( mode == TOGGLE )
		{
			m = IsWindowVisible() ? SW_HIDE : SW_SHOW;
		}
		::ShowWindow(m_dmyWnd, m);
	}

	/**
	 * [�\��] �g�b�v�\��.
	 *	@param isTop true �Ȃ���TOP�ɕ\�������悤�ɂȂ�܂��B false �Ȃ�ʏ�ł��B
	 */
	void SetTopMost(bool isTop = true)
	{
		::SetWindowPos(m_dmyWnd, isTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	/**
	 * [�Q��] �J���[���X�g�R���g���[���Q��.
	 *	@note �R���X�g���N�^�ŃJ���[���[�h���w�肵���ꍇ�̂݁A�L���ł��B
	 *	@return ���X�g�R���g���[���I�u�W�F�N�g
	 */
	CColorListBox& ReferColorList(void)
	{
		ASSERT( m_isColorMode );
		return m_logColorList;
	}

	/**
	 * [�Q��] �������X�g�R���g���[���Q��.
	 *	@note �R���X�g���N�^�Ŕ������[�h���w�肵���ꍇ�̂݁A�L���ł��B
	 *	@return ���X�g�R���g���[���I�u�W�F�N�g
	 */
	CLightListBox& ReferBwList(void)
	{
		ASSERT( ! m_isColorMode );
		return m_logBwList;
	}

	/**
	 * [�Q��] ���j���[�Q��.
	 *	@note Create() �̑O�Ƀ��j���[���쐬�i�A�^�b�`�E���[�h�j���邱�ƂŁA�������j���[��ύX�ł��܂��B
	 *	@note OnCreate() ���p�����āA���j���[�̏�Ԃ�ύX���邱�Ƃ��\�ł��B
	 *	@return ���j���[�I�u�W�F�N�g�@
	 */
	CMenu& ReferMenu(void)
	{
		return m_menu;
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *		�ǉ����镶����̐F���w�肵�܂��B
	 *	@note �R���X�g���N�^�ŃJ���[���[�h���w�肵���ꍇ�̂݁A�L���ł��B
	 *	@param colorText �����F�w��(COLORREF)
	 *	@param colorBack �w�i�F�w��(COLORREF)
	 */
	void SetCurrentColor(COLORREF colorText, COLORREF colorBack)
	{
		ASSERT( m_isColorMode );
		m_logColorList.SetCurrentColor(colorText, colorBack);
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *		�w��s�̐F���w�肵�܂��B
	 *	@note �R���X�g���N�^�ŃJ���[���[�h���w�肵���ꍇ�̂݁A�L���ł��B
	 *	@param nIndex �C���f�b�N�X
	 *	@param colorText �����F�w��(COLORREF)
	 *	@param colorBack �w�i�F�w��(COLORREF)
	 */
	void SetColor(int nIndex, COLORREF colorText, COLORREF colorBack)
	{
		ASSERT( m_isColorMode );
		m_logColorList.SetColor(nIndex, colorText, colorBack);
	}

	/**
	 * [�ݒ�] �}�[�W���ݒ�
	 *	@param m �㉺�̃}�[�W��
	 */
	void SetMargin(DWORD m)
	{
		m_logColorList.SetMargin(m);
		m_logBwList.SetMargin(m);
	}

	/**
	 * [�ݒ�] �ő僉�C�����ݒ�.
	 *	@param maxLine �ő僉�C��.
	 */
	void SetMaxLine(int maxLine)
	{
		m_logColorList.SetMaxLine(maxLine);
		m_logBwList.SetMaxLine(maxLine);
	}

	/**
	 * [�ݒ�] �t�H���g�ݒ�
	 *	@param pFont �t�H���g
	 */
	void SetFont(CFont* pFont)
	{
		m_pListBox->SetFont(pFont);
	}

	/**
	 * [�ݒ�] PostString�\�����[�h�ݒ�.
	 *	@note PostString() ���s���A�ǉ����������񂪕\������悤�ɃX�N���[�������邩�ۂ��A�ݒ�ł��܂��B
	 *	@note �R���X�g���N�^�ŃJ���[���[�h���w�肵���ꍇ�̂݁A�L���ł��B
	 *	@param isView true �Ȃ�ǉ������񂪕\�������悤�ɃX�N���[�����܂��B false �Ȃ�X�N���[�����܂���B
	 */
	void SetPostViewMode(bool isView)
	{
		ASSERT( m_isColorMode );
		m_logColorList.SetPostViewMode(isView);
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		�w�肵���F�ŕ������ǉ����܂��B
	 *	@note �F�̎w��́A�R���X�g���N�^�ŃJ���[���[�h���w�肵���ꍇ�̂݁A�L���ł��B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 *	@param colorText �����F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�����F�ɂȂ�܂��B
	 *	@param colorBack �w�i�F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�w�i�F�ɂȂ�܂��B
	 */
	void PostString(int nIndex, LPCTSTR lpszItem, COLORREF colorText = 0, COLORREF colorBack = 0)
	{
		if ( m_isColorMode )
		{
			m_logColorList.PostString(nIndex, lpszItem, colorText, colorBack);
		}
		else
		{
			m_logBwList.PostString(nIndex, lpszItem);
		}
		if ( m_parentMenuItem == 0 )
		{
			ShowWindow();
		}
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		�w�肵���F�ŕ�������Ō�ɒǉ����܂��B
	 *	@note �ʃX���b�h����ł��ǉ����\�ł��B
	 *	@attention ���s�R�[�h�͏�������܂���(���̕������ł�)�B
	 *	@param lpszItem �}�����镶����B
	 *	@param colorText �����F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�����F�ɂȂ�܂��B
	 *	@param colorBack �w�i�F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�w�i�F�ɂȂ�܂��B
	 */
	void PostString(LPCTSTR lpszItem, COLORREF colorText = 0, COLORREF colorBack = 0)
	{
		PostString(-1, lpszItem, colorText, colorBack);
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		�w�肵���F�ŕ�������Ō�ɒǉ����܂��B���s�R�[�h������Ε����s�ɂ��Ēǉ����܂��B
	 *	@note �ʃX���b�h����ł��ǉ����\�ł��B
	 *	@param lpszItem �}�����镶����B
	 *	@param colorText �����F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�����F�ɂȂ�܂��B
	 *	@param colorBack �w�i�F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�w�i�F�ɂȂ�܂��B
	 */
	void PostStringCr(LPCTSTR lpszItem, COLORREF colorText = 0, COLORREF colorBack = 0)
	{
		CConstStrVector vs;
		vs.SetClone(lpszItem);
		loop ( i, vs.GetLineCount() )
		{
			PostString(-1, vs.GetString(i), colorText, colorBack);
		}
	}

protected:

	/**
	 * [�ʒm] �쐬�����ʒm.
	 *	@note �E�B���h�E�쐬������ɒʒm����܂��B
	 */
	virtual void OnCreate(void)
	{
	}

	/**
	 * [�ʒm] �j���ʒm.
	 *	@note �E�B���h�E�j�����O�ɒʒm����܂��B
	 */
	virtual void OnDestroy(void)
	{
	}

	/**
	 * [�ʒm] �R�}���h�ʒm
	 *	@param wParam ���j���[ID
	 */
	virtual void OnCommand(WPARAM wParam)
	{
		switch ( wParam )
		{
		case MenuId_Hide://�B��
			::ShowWindow(*this, SW_HIDE);
			break;
		case MenuId_Clear://�N���A
			m_pListBox->ResetContent();
			break;
		}
	}

private:
	/**
	 * [�ʒm] ��M���b�Z�[�W�ʒm.
	 *	@note WM_CREATE �́A�ʒm����܂���B
	 *	@note WM_DESTROY ���Ō�ɃE�B���h�E�͔j������܂��B
	 *	@param[out] _result ���ʊi�[�B true �Ń��^�[�������Ƃ��Ɏg�p����܂��B
	 *	@param[in] hWnd �E�B���h�E�n���h��
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@retval true ������(DefWindowProc()���Ăяo���Ȃ��Ȃ�܂�)
	 *	@retval false ������
	 */	
	virtual bool OnWindowMessage(LRESULT& _result, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_CLOSE:
			::ShowWindow(m_dmyWnd, SW_HIDE);
			return true;
		case WM_MEASUREITEM:
			m_pListBox->MeasureItem(reinterpret_cast<LPMEASUREITEMSTRUCT>(lParam));
			break;
		case WM_DRAWITEM:
			m_pListBox->DrawItem(reinterpret_cast<LPDRAWITEMSTRUCT>(lParam));
			break;
		case WM_SIZE:
			if ( ::IsWindow(m_pListBox->GetSafeHwnd()) )
			{
				CSize sz(LOWORD(lParam), HIWORD(lParam));
				m_pListBox->SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER);
			}
			break;
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_CHAR:
			m_pListBox->SendMessage(message, wParam, lParam);
			break;
		case WM_DESTROY:
			m_menu.DestroyMenu();
			OnDestroy();
			break;
		case WM_SHOWWINDOW:
			if ( m_parentMenuItem != 0 )
			{
				HMENU h = ::GetMenu(m_hParentWnd);
				if ( h != NULL )
				{
					UINT r = (wParam != 0) ? MF_CHECKED : MF_UNCHECKED;
					::CheckMenuItem(h, m_parentMenuItem, r);
				}
			}
			break;
		case WM_COMMAND:
			OnCommand(wParam);
			break;
		default:
			break;
		}
		return false;
	}
	/// ����LIST�R���g���[��
	class CInListBox : public CDelayedRedrawAddinT<CColorListBox>
	{
		DEFSUPER(CDelayedRedrawAddinT<CColorListBox>);
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
			if ( message == WM_CLB_POSTSTRING )
			{
				_super::DelayedRedraw();
			}
			return _super::WindowProc(message, wParam, lParam);
		}
	};

	HWND			m_hParentWnd;		///< �e�E�B���h�E
	CListBox*		m_pListBox;			///< �g�p���Ă��郊�X�g�{�b�N�X
	CInListBox		m_logColorList;		///< �J���[���[�h�p���X�g�{�b�N�X
	CLightListBox	m_logBwList;		///< �����p���X�g�{�b�N�X
	CDummyWindow	m_dmyWnd;			///< �E�B���h�E�Ǘ�
	CMenu			m_menu;				///< ���j���[
	CMenu			m_subMenu1;			///< �T�u���j���[�P
	CMenu			m_subMenu2;			///< �T�u���j���[�Q
	UINT			m_parentMenuItem;	///< �e���j���[�̃A�C�e��
	bool			m_isColorMode;		///< �J���[���[�h
};



}; // MFC
}; // TNB
