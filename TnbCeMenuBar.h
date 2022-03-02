#pragma once
/**
 *@file
 * CE専用 メニューバー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeMenuBar.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbDef.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#ifndef TBIF_BYINDEX
 #define TBIF_BYINDEX 0x80000000
#endif
#endif



//TNB Library
namespace TNB{



/**@ingroup WINCE 
 * メニューバークラス(CE専用)
 *
 *		このクラスは、 CE特有のメニューバーの実装をサポートするものです。
 *		リソースエディタでトップ２つのメニューを用意しておき、本クラスの Create すると、
 *		トップメニューが、メニューバーのトップになり、以下、サブメニューになります。
 *		後は、普通のメニューと同じです。
 *
 *	@note リソースのメニューのトップは、チェックあり、Gray、Disable は禁止とする。
 *
 *	@note 全画面アプリについて→ http://support.microsoft.com/kb/266244/ja
 *
 *	@par使用例
 *		・プロジェクトの rc2 ファイルに以下を追加する。
 *		\code
 *		// ↓999 でなくてもいい。 ここの値を Create() のバーのPUREリソースIDに適用すること。		
 *		999 RCDATA MOVEABLE PURE 
 *		BEGIN
 *			0, 0
 *		\endcode
 *		・ダイアログのクラスにインスタンスを持たせ、OnInitDialog() で Createする。
 *		\code
 *		BOOL CFooDlg::OnInitDialog()
 *		{
 *			CDialog::OnInitDialog(); 
 *				;
 *			m_menuBar.Create(GetSafeHwnd(), 999, IDR_MENU_MAIN);
 *							//↑ 999は rc2の値と同じ。IDR_MENU_MAIN はメニュのリソースID。
 *		\endcode
 *		・WM_ACTIVATE のハンドラ OnActivate() に、以下のように追加する。
 *		\code
 *		void CFooDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
 *		{
 *			CDialog::OnActivate(nState, pWndOther, bMinimized);
 *			m_menuBar.OnActivate(nState);　
 *		}
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbCeMenuBar.h
 *
 *	@date 08/07/25	新規作成
 *	@date 08/11/17	タスクバーもあわせて Show/Hide 出来るように変更。
 *	@date 10/08/17	OnActivate() を追加。
 */
class CCeMenuBar : CCopyImpossible
{
public:

	/// コンストラクタ
	CCeMenuBar(void) : m_menu(NULL), m_isShow(true)
	{
		Zero(m_mbi);
	}

	/**
	 * [取得] メニューバーハンドル
	 *	@retval NULL 作成されていません。
	 *	@retval NULL以外 メニューバーハンドル
	 */
	HWND GetMenuWndHandle(void) const
	{
		return m_mbi.hwndMB;
	}

	/**
	 * [破棄] メニュー破棄.
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
	 * [作成] メニューバー作成.
	 *	@param hWnd 親ウィンドウハンドル.
	 *	@param barId バーのPUREリソースID
	 *	@param menuId MENUのリソースID
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Create(HWND hWnd, UINT barId, UINT menuId)
	{
		m_isShow = true;
		return m_Create(hWnd, barId, MAKEINTRESOURCE(menuId));
	}

	/**
	 * [確認] 表示確認
	 *	@note タスクバー、メニューバーの表示確認です。
	 *	@retval true 表示状態
	 *	@retval false 非表示
	 */
	bool IsBarVisible(void) const
	{
		return !! ::IsWindowVisible(m_mbi.hwndMB);
	}

	/**
	 * [操作] 表示非表示.
	 *	@note 作成時、タスクバー、メニューバーは SHOW状態です。
	 *	@param isShow true ならタスクバー、メニューバーを SHOW状態に(見えるように)します。 
	 *					false なら HIDE状態にします。
	 *	@param withParent 省略するか ture なら、タスクバー、メニューバーの表示状態に応じて、
	 *						親のウィンドウサイズも最大になるように変化させます。\n
	 *						false なら親ウィンドウは操作しません。
	 *	@retval true 成功。
	 *	@retval false 失敗。 Createされていない場合、失敗します。
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
	 * [取得] トップメニューアイテム数
	 *	@return アイテム数
	 */
	size_t GetTopItemCount(void) const
	{
		return min(MAXMENU, ::GetMenuItemCount(m_menu));
	}

	/**
	 * [取得] メニューハンドル取得
	 *	@retval NULL エラー
	 *	@retval NULL以外 メニューハンドル
	 */
	HMENU GetTopMenu(void) const
	{
		return m_menu;
	}

	/**
	 * [設定] トップメニュー有効無効.
	 *	@param index トップメニュのインデックス。
	 *	@param isEnable true なら有効、 false なら無効にします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 Createされていない場合、失敗します。
	 */
	bool EnableTopItem(INDEX index, bool isEnable)
	{
		return SetTopItemState(index, isEnable ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE);
	}

	/**
	 * [設定] トップメニュー文字列設定.
	 *	@param index トップメニュのインデックス。
	 *	@param lpszText 文字列
	 *	@retval true 成功。
	 *	@retval false 失敗。 Createされていない場合、失敗します。
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
	 * [設定] トップメニュー状態設定.
	 *	@param index トップメニュのインデックス。
	 *	@param state ステータス。 TBSTATE_ENABLED など
	 *	@retval true 成功。
	 *	@retval false 失敗。 Createされていない場合、失敗します。
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
	 * [取得] トップメニュー状態取得.
	 *	@param index トップメニュのインデックス。
	 *	@retval DWORD_MAX 失敗.
	 *	@retval 上記以外 成功。数値は TBSTATE_ENABLED など。
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
	 * [処理] アクティブ化処理.
	 *	@note 本インスタンスを持っているウィンドウが WM_ACTIVATE を受けた時に、
	 *		本メソッドをコールするようにしてください。
	 *	@param nState アクティブ/非アクティブ状態値.
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
	 * [作成] メニューバー作成.
	 *	@param hWnd 親ウィンドウハンドル.
	 *	@param barId バーのPUREリソースID
	 *	@param menuName MENUのリソースネーム
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
	SHMENUBARINFO	m_mbi;			///< メニューバー情報
	HMENU			m_menu;			///< メニュー
	bool			m_isShow;		///< 表示フラグ
};



};//TNB


