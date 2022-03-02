#pragma once
/**
 *@file
 * モードレスダイアログ関係ヘッダ ファイル
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * モードレスウィンドウ管理テンプレート.
 *	
 *		指定のダイアログベースウィンドウを Modeless で表示します。
 *		Show() , Hide() , Destroy() メソッドにより容易に表示(作成)、非表示、破棄できるようになります。
 *
 *	@note 作成したウィンドウの親ウィンドウは NULL です。
 *
 *	@note 親ウィンドウの表示非表示メニューIDを SetParentMenu() で指定すると、Show Hide により自動的に表示チェックが on off します。
 *			ただし、メニューアイテム選択時の Show Hide は自動化できません。親ダイアログのハンドラで 
 *			xxx.Toggle(); としてください。
 *			\code
 *				// CModelessDialogT<CSybDlg> m_subDlg;
 *				void CFooDlg::OnIniDialog(void)
 *				{
 *						;
 *					m_subDlg.SetParentMenu(GetSafeHwnd(), ID_MENU_LIST_VIEW);
 *					m_subDlg.Hide(); //非表示で作成しておく場合。
 *						;
 *				}
 *				void CFooDlg::OnMenuListView(void)
 *				{
 *					m_listDlg.Toggle();
 *				}
 *			\endcode
 *			※ CE では SetParentMenu() は使えません。
 *
 *	@note operator->() があるので、 DLG のメンバに直接アクセスも可能です。
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
 *	@param DLG MFC の CDialog を継承したクラス型
 *	@param HASTASK true ならタスクバーに現れます(def.)。 false なら現れません。
 *
 *	@par必要ファイル
 *			TnbMfcModelessDialog.h
 *
 *	@date 06/09/20 新規。
 *	@date 07/10/03 簡略化。
 *	@date 08/07/15 Hideでは破棄せず非表示になるようにし、Destoryを別途用意。
 *	@date 09/08/03 SetParentMenu() , Toggle() を追加。
 *	@date 09/08/19 operator HWND() を追加。
 *	@date 10/01/06 閉じる処理改善。閉じる時にメッセージを投げられるようにした。
 *	@date 10/08/05 親ウィンドウを NULL とした。
 *	@date 10/09/01 HASTASK 追加
 *	@date 11/06/09 CE に対応。
 *	@date 11/08/31 CModelessDialogT に改名。旧名は typedef で残す。
 *	@date 12/03/16 DisableWmClose() を新規。
 */
template<typename DLG, bool HASTASK = true>
class CModelessDialogT
{
public:
	
	/// コンストラクタ
	CModelessDialogT(void)
	{
	}

	/// デストラクタ
	~CModelessDialogT(void)
	{
		Destroy();
	}

	/**
	 * [設定] 親メニュー設定.
	 *	@attention CE では、このメソッドはダミーです（機能しません）。
	 *	@param hWnd 親ウィンドウ.
	 *	@param menuItem 本ダイアログの Show / Hide を管理する親ウィンドウのメニューID。
	 *	@param msg 閉じた時に、通知するメッセージ。
	 */
	void SetParentMenu(HWND hWnd, UINT menuItem, UINT msg = 0)
	{
		m_dlg.SetParentMenu(hWnd, menuItem, msg);
	}

	/**
	 * [取得] 表示状態
	 *	@retval true 表示中
	 *	@retval false 非表示
	 */
	bool IsViewing(void) const
	{
		return IsVisible();
	}
	
	/**
	 * [取得] 表示状態
	 *	@retval true 表示中
	 *	@retval false 非表示
	 */
	bool IsVisible(void) const
	{
		return ::IsWindow(m_dlg.m_hWnd) && ::IsWindowVisible(m_dlg.m_hWnd);
	}
	
	/**
	 * [設定] 表示.
	 *		ウィンドウを表示します。
	 *	@note ウィンドウが作成されていないとき、ウィンドウを作成します。
	 */
	void Show(void)
	{
		m_CreateDlg();
		m_dlg.ShowWindow(SW_SHOW);
	}

	/**
	 * [設定] 非表示.
	 *		ウィンドウを非表示にします。
	 *	@note ウィンドウが作成されていないとき、ウィンドウを作成します。
	 *	@note ウィンドウは破棄されません。破棄する場合、 Destroy() を使用します。
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
	 * [設定] 表示/非表示.
	 *		表示していなければ、表示します。表示していれば、非表示にします。
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
	 * [設定] 破棄.
	 *		ウィンドウを破棄します。
	 */
	void Destroy(void)
	{
		if ( ::IsWindow(m_dlg.m_hWnd) )
		{
			m_dlg.DestroyWindow();
		}
	}

	/**
	 * [取得] ダイアログクラス参照
	 *	@return ダイアログクラスの参照
	 */
	const DLG* operator->(void) const
	{
		return &m_dlg;
	}

	/**
	 * [取得] ダイアログクラス参照
	 *	@return ダイアログクラスの参照
	 */
	DLG* operator->(void)
	{
		return &m_dlg;
	}

	/**
	 * [取得] ウィンドウハンドル取得
	 *	@return ウィンドウハンドル
	 */
	operator HWND(void)
	{
		return m_dlg;
	}

	/**
	 * [設定] WM_CLOSE 処理禁止.
	 *		デフォルトでは、 WM_CLOSE を受信すると HIDE になるようになっています。
	 *		本メソッドはこれを禁止します（何もしなくなります）。
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
	/// 内部クラス
	class CInner : public DLG
	{
		DEFSUPER(DLG);
	public:
		/// コンストラクタ
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
		 * [設定] 親メニュー設定.
		 *	@param hWnd 親ウィンドウ.
		 *	@param menuItem 本ダイアログの Show / Hide を管理する親ウィンドウのメニューID。
		 *	@param msg 閉じた時に、通知するメッセージ。
		 */
		void SetParentMenu(HWND hWnd, UINT menuItem, UINT msg)
		{
			m_hParentWnd = hWnd;
			m_parentMenuItem = menuItem;
			m_parentMsg = msg;
		}
		/// WM_CLOSE 処理禁止
		void DisableWmClose(void)
		{
			m_isEnableWmClose = false;
		}
	protected:
		/**
		 * [通知] for processing Windows messages.
		 *		メッセージ受信したらコールされます。
		 *	@note CWndのメソッドをオーバーライドしています。
		 *	@param message メッセージ
		 *	@param wParam WPARAM
		 *	@param lParam LPARAM
		 *	@return リザルト。
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
	CInner	m_dlg;				///< 対象Window
};



}; // MFC
}; // TNB
