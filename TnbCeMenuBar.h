#pragma once
/**
 *@file
 * CE��p ���j���[�o�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeMenuBar.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbDef.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#ifndef TBIF_BYINDEX
 #define TBIF_BYINDEX 0x80000000
#endif
#endif



//TNB Library
namespace TNB{



/**@ingroup WINCE 
 * ���j���[�o�[�N���X(CE��p)
 *
 *		���̃N���X�́A CE���L�̃��j���[�o�[�̎������T�|�[�g������̂ł��B
 *		���\�[�X�G�f�B�^�Ńg�b�v�Q�̃��j���[��p�ӂ��Ă����A�{�N���X�� Create ����ƁA
 *		�g�b�v���j���[���A���j���[�o�[�̃g�b�v�ɂȂ�A�ȉ��A�T�u���j���[�ɂȂ�܂��B
 *		��́A���ʂ̃��j���[�Ɠ����ł��B
 *
 *	@note ���\�[�X�̃��j���[�̃g�b�v�́A�`�F�b�N����AGray�ADisable �͋֎~�Ƃ���B
 *
 *	@note �S��ʃA�v���ɂ��ā� http://support.microsoft.com/kb/266244/ja
 *
 *	@par�g�p��
 *		�E�v���W�F�N�g�� rc2 �t�@�C���Ɉȉ���ǉ�����B
 *		\code
 *		// ��999 �łȂ��Ă������B �����̒l�� Create() �̃o�[��PURE���\�[�XID�ɓK�p���邱�ƁB		
 *		999 RCDATA MOVEABLE PURE 
 *		BEGIN
 *			0, 0
 *		\endcode
 *		�E�_�C�A���O�̃N���X�ɃC���X�^���X���������AOnInitDialog() �� Create����B
 *		\code
 *		BOOL CFooDlg::OnInitDialog()
 *		{
 *			CDialog::OnInitDialog(); 
 *				;
 *			m_menuBar.Create(GetSafeHwnd(), 999, IDR_MENU_MAIN);
 *							//�� 999�� rc2�̒l�Ɠ����BIDR_MENU_MAIN �̓��j���̃��\�[�XID�B
 *		\endcode
 *		�EWM_ACTIVATE �̃n���h�� OnActivate() �ɁA�ȉ��̂悤�ɒǉ�����B
 *		\code
 *		void CFooDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
 *		{
 *			CDialog::OnActivate(nState, pWndOther, bMinimized);
 *			m_menuBar.OnActivate(nState);�@
 *		}
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbCeMenuBar.h
 *
 *	@date 08/07/25	�V�K�쐬
 *	@date 08/11/17	�^�X�N�o�[�����킹�� Show/Hide �o����悤�ɕύX�B
 *	@date 10/08/17	OnActivate() ��ǉ��B
 */
class CCeMenuBar : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CCeMenuBar(void) : m_menu(NULL), m_isShow(true)
	{
		Zero(m_mbi);
	}

	/**
	 * [�擾] ���j���[�o�[�n���h��
	 *	@retval NULL �쐬����Ă��܂���B
	 *	@retval NULL�ȊO ���j���[�o�[�n���h��
	 */
	HWND GetMenuWndHandle(void) const
	{
		return m_mbi.hwndMB;
	}

	/**
	 * [�j��] ���j���[�j��.
	 */
	void Destory(void)
	{
		::SendMessage(m_mbi.hwndMB, WM_CLOSE, 0, 0);
		if ( m_menu != NULL )
		{
			::DestroyMenu(m_menu);
			m_menu = NULL;
		}
		Zero(m_mbi);
	}

	/**
	 * [�쐬] ���j���[�o�[�쐬.
	 *	@param hWnd �e�E�B���h�E�n���h��.
	 *	@param barId �o�[��PURE���\�[�XID
	 *	@param menuId MENU�̃��\�[�XID
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Create(HWND hWnd, UINT barId, UINT menuId)
	{
		m_isShow = true;
		return m_Create(hWnd, barId, MAKEINTRESOURCE(menuId));
	}

	/**
	 * [�m�F] �\���m�F
	 *	@note �^�X�N�o�[�A���j���[�o�[�̕\���m�F�ł��B
	 *	@retval true �\�����
	 *	@retval false ��\��
	 */
	bool IsBarVisible(void) const
	{
		return !! ::IsWindowVisible(m_mbi.hwndMB);
	}

	/**
	 * [����] �\����\��.
	 *	@note �쐬���A�^�X�N�o�[�A���j���[�o�[�� SHOW��Ԃł��B
	 *	@param isShow true �Ȃ�^�X�N�o�[�A���j���[�o�[�� SHOW��Ԃ�(������悤��)���܂��B 
	 *					false �Ȃ� HIDE��Ԃɂ��܂��B
	 *	@param withParent �ȗ����邩 ture �Ȃ�A�^�X�N�o�[�A���j���[�o�[�̕\����Ԃɉ����āA
	 *						�e�̃E�B���h�E�T�C�Y���ő�ɂȂ�悤�ɕω������܂��B\n
	 *						false �Ȃ�e�E�B���h�E�͑��삵�܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B Create����Ă��Ȃ��ꍇ�A���s���܂��B
	 */
	bool ShowBar(bool isShow, bool withParent = true)
	{
		if ( m_mbi.hwndMB == NULL )
		{
			return false;
		}
		if ( ! withParent )
		{
			m_isShow = isShow;
			return !! ::ShowWindow(m_mbi.hwndMB, isShow ? SW_SHOW : SW_HIDE);
		}
		bool r = true;
		HDC dc = ::GetWindowDC(NULL);
		int cx = ::GetDeviceCaps(dc, HORZRES);
		int cy = ::GetDeviceCaps(dc, VERTRES);
		::ReleaseDC(NULL, dc);
		RECT rc = { 0, 0, cx, cy };
		HWND hWnd = m_mbi.hwndParent;
		if ( isShow )
		{
			int height = ::CalcScaleY(26);
			r &= !! ::SHFullScreen(hWnd, SHFS_SHOWTASKBAR);
			r &= !! ::ShowWindow(m_mbi.hwndMB, SW_SHOW);
			rc.top = height;
			rc.bottom -= (2 * height);
			::SetForegroundWindow(hWnd);
		}
		else
		{
			if ( m_isShow )
			{
				SHINITDLGINFO shidi;
				shidi.hDlg = hWnd;
				shidi.dwMask = SHIDIM_FLAGS;
				shidi.dwFlags = SHIDIF_FULLSCREENNOMENUBAR | SHIDIF_SIPDOWN;
				::SHInitDialog(&shidi);
			}
			r &= !! ::SHFullScreen(hWnd, SHFS_HIDETASKBAR);
			r &= !! ::SHFullScreen(hWnd, SHFS_HIDESIPBUTTON);
			r &= !! ::ShowWindow(m_mbi.hwndMB, SW_HIDE);
		}
		r &= !! ::MoveWindow(m_mbi.hwndParent, rc.left, rc.top, rc.right, rc.bottom, TRUE); 
		m_isShow = isShow;
		return r;
	}

	/**
	 * [�擾] �g�b�v���j���[�A�C�e����
	 *	@return �A�C�e����
	 */
	size_t GetTopItemCount(void) const
	{
		return min(MAXMENU, ::GetMenuItemCount(m_menu));
	}

	/**
	 * [�擾] ���j���[�n���h���擾
	 *	@retval NULL �G���[
	 *	@retval NULL�ȊO ���j���[�n���h��
	 */
	HMENU GetTopMenu(void) const
	{
		return m_menu;
	}

	/**
	 * [�ݒ�] �g�b�v���j���[�L������.
	 *	@param index �g�b�v���j���̃C���f�b�N�X�B
	 *	@param isEnable true �Ȃ�L���A false �Ȃ疳���ɂ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B Create����Ă��Ȃ��ꍇ�A���s���܂��B
	 */
	bool EnableTopItem(INDEX index, bool isEnable)
	{
		return SetTopItemState(index, isEnable ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE);
	}

	/**
	 * [�ݒ�] �g�b�v���j���[������ݒ�.
	 *	@param index �g�b�v���j���̃C���f�b�N�X�B
	 *	@param lpszText ������
	 *	@retval true �����B
	 *	@retval false ���s�B Create����Ă��Ȃ��ꍇ�A���s���܂��B
	 */
	bool SetTopItemString(INDEX index, LPCTSTR lpszText)
	{
		CSimpleStr s = lpszText;
		TBBUTTONINFO info = { 0 };
		info.cbSize = sizeof(TBBUTTONINFO);
		info.dwMask = TBIF_TEXT | TBIF_BYINDEX;
		info.pszText = s;
		info.cchText = _tcslen(lpszText);
		LPARAM lParam = reinterpret_cast<LPARAM>(&info);
		return ::SendMessage(m_mbi.hwndMB, TB_SETBUTTONINFO, index, lParam) != 0;
	}

	/**
	 * [�ݒ�] �g�b�v���j���[��Ԑݒ�.
	 *	@param index �g�b�v���j���̃C���f�b�N�X�B
	 *	@param state �X�e�[�^�X�B TBSTATE_ENABLED �Ȃ�
	 *	@retval true �����B
	 *	@retval false ���s�B Create����Ă��Ȃ��ꍇ�A���s���܂��B
	 */
	bool SetTopItemState(INDEX index, DWORD state)
	{
		TBBUTTONINFO info = { 0 };
		info.cbSize = sizeof(TBBUTTONINFO);
		info.dwMask = TBIF_STATE | TBIF_BYINDEX;
		info.fsState = down_cast<BYTE>(state);
		LPARAM lParam = reinterpret_cast<LPARAM>(&info);
		return ::SendMessage(m_mbi.hwndMB, TB_SETBUTTONINFO, index, lParam) != 0;
	}

	/**
	 * [�擾] �g�b�v���j���[��Ԏ擾.
	 *	@param index �g�b�v���j���̃C���f�b�N�X�B
	 *	@retval DWORD_MAX ���s.
	 *	@retval ��L�ȊO �����B���l�� TBSTATE_ENABLED �ȂǁB
	 */
	DWORD GetTopItemState(INDEX index) const
	{
		TBBUTTONINFO info = { 0 };
		info.cbSize = sizeof(TBBUTTONINFO);
		info.dwMask = TBIF_STATE | TBIF_BYINDEX;
		LPARAM lParam = reinterpret_cast<LPARAM>(&info);
		if ( ::SendMessage(m_mbi.hwndMB, TB_GETBUTTONINFO, index, lParam) )
		{
			return info.fsState;
		}
		return DWORD_MAX;
	}

	/**
	 * [����] �A�N�e�B�u������.
	 *	@note �{�C���X�^���X�������Ă���E�B���h�E�� WM_ACTIVATE ���󂯂����ɁA
	 *		�{���\�b�h���R�[������悤�ɂ��Ă��������B
	 *	@param nState �A�N�e�B�u/��A�N�e�B�u��Ԓl.
	 */
	void OnActivate(UINT nState)
	{
		if ( nState != WA_INACTIVE && ! IsBarVisible() )
		{
			ShowBar(false);
		}
	}

private:
	/**
	 * [�쐬] ���j���[�o�[�쐬.
	 *	@param hWnd �e�E�B���h�E�n���h��.
	 *	@param barId �o�[��PURE���\�[�XID
	 *	@param menuName MENU�̃��\�[�X�l�[��
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool m_Create(HWND hWnd, UINT barId, LPCTSTR menuName)
	{
		HINSTANCE hInst = GetInstanceHandleByTnb();
		Destory();
		m_mbi.cbSize = sizeof(SHMENUBARINFO);
		m_mbi.hwndParent = hWnd;
		m_mbi.nToolBarId = barId;
		m_mbi.hInstRes = hInst;
		if ( ! ::SHCreateMenuBar(&m_mbi) )
		{
			_GetLastError("SHCreateMenuBar");
			return false;
		}
		if ( menuName == NULL )
		{
			return true;
		}
		m_menu = ::LoadMenu(hInst, menuName);
		bool r = true;
		loop( i, min(MAXMENU, ::GetMenuItemCount(m_menu)) )
		{
			int len = ::GetMenuString(m_menu, i, NULL, 0, MF_BYPOSITION);
			if ( len == 0 )
			{
				r = false;
				break;
			}
			CWorkMemT<WCHAR> tempName(len + 1);
			if ( ::GetMenuString(m_menu, i, tempName.Ref(), len + 1, MF_BYPOSITION) == 0 )
			{
				r = false;
				break;
			}
			TBBUTTON tbb;
			Zero(tbb);
			tbb.iBitmap = I_IMAGENONE;
			tbb.fsState = TBSTATE_ENABLED;
			UINT state = ::GetMenuState(m_menu, i, MF_BYPOSITION);
			if ( state != 0xFFFFFFFF && (state & MF_GRAYED) != 0 )
			{
				tbb.fsState = TBSTATE_INDETERMINATE;
			}
			LPCTSTR lpsz = tempName;
			tbb.iString = reinterpret_cast<int>(lpsz);
			HMENU sub = ::GetSubMenu(m_menu, i);
			if ( sub != NULL )
			{
				tbb.fsStyle = TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE;
				tbb.dwData = reinterpret_cast<DWORD>(sub);
			}
			else
			{
				tbb.fsStyle = TBSTYLE_AUTOSIZE;
				tbb.idCommand = ::GetMenuItemID(m_menu, i);
			}
			::SendMessage(m_mbi.hwndMB, TB_INSERTBUTTON, i, reinterpret_cast<LPARAM>(&tbb));
		}
		if ( ! r )
		{
			Destory();
		}
		return r;
	}

	enum { MAXMENU = 2 };
	SHMENUBARINFO	m_mbi;			///< ���j���[�o�[���
	HMENU			m_menu;			///< ���j���[
	bool			m_isShow;		///< �\���t���O
};



};//TNB


