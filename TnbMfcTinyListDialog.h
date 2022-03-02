#pragma once
/**
 *@file
 * Listウィンドウ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
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
 * 簡易リストウィンドウ
 *
 *		行単位で色を指定出来る ListBoxコントロールを画面一面にもつモードレスダイアログです。。
 *
 *	@note 親ウィンドウの表示非表示メニューIDを Create() で指定すると、Show Hide により自動的に表示チェックが on off します。
 *			ただし、メニューアイテム選択時の Show Hide は自動化できません。親ダイアログのハンドラで 
 *			xxx.ShowWindow(CTinyListDialog::TOGGLE); としてください。
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
 *			メニューIDを指定していない場合、 PostString() で表示されるようになります。
 *
 *	@see CColorListBox
 *	@see CTinyListDialogEx
 *
 *	@par必要ファイル
 *			TnbMfcTinyListDialog.h
 * 
 *	@date 09/04/02	新規作成
 *	@date 09/04/30	親のメニューの制御もサポート
 *	@date 09/08/06	メニューのIDシンボルを変更。
 *	@date 10/04/19	メニュー管理方法修正
 *	@date 11/02/09	白黒高速モード、追加。
 *	@date 11/10/04	OnCreate() 追加。
 *	@date 12/01/27	OnDestroy() 追加。
 *	@date 15/03/09	PostStringCr() 追加。
 */
class CTinyListDialog : private CDummyWindow::IListener
{
public:

	/// メニューID
	enum EMenuId
	{
		MenuId_Hide = 1000,		///< 隠す
		MenuId_Clear = 1001,	///< クリア
	};

	/// 表示モード型
	enum EShowMode
	{
		SHOW,		///< 表示
		HIDE,		///< 非表示
		TOGGLE,		///< トグル
	};

	/// 移動モード型
	enum EMoveMode
	{
		CENTER,		///< 中央
		UNDER,		///< 下
		RIGHT,		///< 右
	};

	/**
	 * コンストラクタ.
	 *	@param isColorMode true ならカラーモード。 false なら白黒モードです。
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
	 * [作成] ウィンドウ作成.
	 *	@note 非表示状態で作成されます。
	 *	@param size ウィンドウ大きさ
	 *	@param hFont フォント
	 *	@param hParentWnd 親ウィンドウ
	 *	@param lpszTitle ウィンドウタイトル
	 *	@param hIcon ウィンドウアイコン
	 *	@param parentMenuItem 親ウィンドウの表示非表示メニュー。省略可能。
	 *	@retval true 作成成功
	 *	@retval false 失敗
	 */
	bool Create(const CSize& size, HFONT hFont, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		// メニュー
		if ( m_menu.GetSafeHmenu() == NULL )
		{
			m_subMenu1.CreateMenu();
			m_subMenu2.CreateMenu();
			m_menu.CreateMenu();
			m_subMenu1.AppendMenu(0, MenuId_Hide, _T("隠す(&H)"));
			m_subMenu2.AppendMenu(0, MenuId_Clear, _T("クリア(&C)"));
			m_menu.AppendMenu(MF_POPUP, reinterpret_cast<UINT_PTR>(m_subMenu1.GetSafeHmenu()), _T("ファイル(&F)"));
			m_menu.AppendMenu(MF_POPUP, reinterpret_cast<UINT_PTR>(m_subMenu2.GetSafeHmenu()), _T("表示(&H)"));
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
	 * [作成] ウィンドウ作成.
	 *	@note 非表示状態で作成されます。
	 *	@param size ウィンドウ大きさ
	 *	@param hParentWnd 親ウィンドウ
	 *	@param lpszTitle ウィンドウタイトル
	 *	@param hIcon ウィンドウアイコン
	 *	@param parentMenuItem 親ウィンドウの表示非表示メニュー。省略可能。
	 *	@retval true 作成成功
	 *	@retval false 失敗
	 */
	bool Create(const CSize& size, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hParentWnd, WM_GETFONT, 0, 0));
		return Create(size, hFont, hParentWnd, lpszTitle, hIcon, parentMenuItem);
	}

	/**
	 * [移動] ウィンドウ移動.
	 *	@param mode 移動モード
	 */
	void MoveWindow(EMoveMode mode = UNDER)
	{
		if ( m_dmyWnd != NULL )
		{
			CRect rc;
			::GetWindowRect(m_hParentWnd, &rc);
			switch ( mode )
			{
			case CENTER:// 中央
				{
					CWnd* pDmyWnd = CWnd::FromHandle(m_dmyWnd);
					pDmyWnd->CenterWindow();
					pDmyWnd->Detach();
				}
				break;
			case UNDER:// 下
				::SetWindowPos(m_dmyWnd, NULL, rc.left, rc.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				break;
			case RIGHT:// 右
				::SetWindowPos(m_dmyWnd, NULL, rc.right, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				break;
			}
		}
	}

	/**
	 * [操作] ウィンドウの破棄.
	 */
	void Destroy(void)
	{
		m_dmyWnd.Destroy();
	}

	/**
	 * [取得] ウィンドウハンドル.
	 *		保持しているウィンドウのハンドルを返します。
	 *	@return ハンドル
	 */
	operator HWND(void)
	{
		return m_dmyWnd;
	}

	/**
	 * [確認] ウィンドウの有無.
	 *	@retval true 有り
	 *	@retval false なし
	 */
	bool IsWindow(void) const
	{
		return m_dmyWnd.IsWindow();
	}

	/**
	 * [確認] ウィンドウ表示状態.
	 *	@retval true 表示中
	 *	@retval false 非表示中（ウィンドウが作成されていない場合も false が返ります）
	 */
	bool IsWindowVisible(void)
	{
		return !! ::IsWindowVisible(m_dmyWnd);
	}

	/**
	 * [表示] 表示切替.
	 *	@param mode 表示モード
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
	 * [表示] トップ表示.
	 *	@param isTop true なら常にTOPに表示されるようになります。 false なら通常です。
	 */
	void SetTopMost(bool isTop = true)
	{
		::SetWindowPos(m_dmyWnd, isTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	/**
	 * [参照] カラーリストコントロール参照.
	 *	@note コンストラクタでカラーモードを指定した場合のみ、有効です。
	 *	@return リストコントロールオブジェクト
	 */
	CColorListBox& ReferColorList(void)
	{
		ASSERT( m_isColorMode );
		return m_logColorList;
	}

	/**
	 * [参照] 白黒リストコントロール参照.
	 *	@note コンストラクタで白黒モードを指定した場合のみ、有効です。
	 *	@return リストコントロールオブジェクト
	 */
	CLightListBox& ReferBwList(void)
	{
		ASSERT( ! m_isColorMode );
		return m_logBwList;
	}

	/**
	 * [参照] メニュー参照.
	 *	@note Create() の前にメニューを作成（アタッチ・ロード）することで、処理メニューを変更できます。
	 *	@note OnCreate() を継承して、メニューの状態を変更することも可能です。
	 *	@return メニューオブジェクト　
	 */
	CMenu& ReferMenu(void)
	{
		return m_menu;
	}

	/**
	 * [設定] 文字色設定.
	 *		追加する文字列の色を指定します。
	 *	@note コンストラクタでカラーモードを指定した場合のみ、有効です。
	 *	@param colorText 文字色指定(COLORREF)
	 *	@param colorBack 背景色指定(COLORREF)
	 */
	void SetCurrentColor(COLORREF colorText, COLORREF colorBack)
	{
		ASSERT( m_isColorMode );
		m_logColorList.SetCurrentColor(colorText, colorBack);
	}

	/**
	 * [設定] 文字色設定.
	 *		指定行の色を指定します。
	 *	@note コンストラクタでカラーモードを指定した場合のみ、有効です。
	 *	@param nIndex インデックス
	 *	@param colorText 文字色指定(COLORREF)
	 *	@param colorBack 背景色指定(COLORREF)
	 */
	void SetColor(int nIndex, COLORREF colorText, COLORREF colorBack)
	{
		ASSERT( m_isColorMode );
		m_logColorList.SetColor(nIndex, colorText, colorBack);
	}

	/**
	 * [設定] マージン設定
	 *	@param m 上下のマージン
	 */
	void SetMargin(DWORD m)
	{
		m_logColorList.SetMargin(m);
		m_logBwList.SetMargin(m);
	}

	/**
	 * [設定] 最大ライン数設定.
	 *	@param maxLine 最大ライン.
	 */
	void SetMaxLine(int maxLine)
	{
		m_logColorList.SetMaxLine(maxLine);
		m_logBwList.SetMaxLine(maxLine);
	}

	/**
	 * [設定] フォント設定
	 *	@param pFont フォント
	 */
	void SetFont(CFont* pFont)
	{
		m_pListBox->SetFont(pFont);
	}

	/**
	 * [設定] PostString表示モード設定.
	 *	@note PostString() 実行時、追加した文字列が表示するようにスクロールさせるか否か、設定できます。
	 *	@note コンストラクタでカラーモードを指定した場合のみ、有効です。
	 *	@param isView true なら追加文字列が表示されるようにスクロールします。 false ならスクロールしません。
	 */
	void SetPostViewMode(bool isView)
	{
		ASSERT( m_isColorMode );
		m_logColorList.SetPostViewMode(isView);
	}

	/**
	 * [追加] 文字列追加.
	 *		指定した色で文字列を追加します。
	 *	@note 色の指定は、コンストラクタでカラーモードを指定した場合のみ、有効です。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 *	@param colorText 文字色指定(COLORREF)。省略した場合、最後に設定された文字色になります。
	 *	@param colorBack 背景色指定(COLORREF)。省略した場合、最後に設定された背景色になります。
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
	 * [追加] 文字列追加.
	 *		指定した色で文字列を最後に追加します。
	 *	@note 別スレッドからでも追加が可能です。
	 *	@attention 改行コードは処理されません(その分高速です)。
	 *	@param lpszItem 挿入する文字列。
	 *	@param colorText 文字色指定(COLORREF)。省略した場合、最後に設定された文字色になります。
	 *	@param colorBack 背景色指定(COLORREF)。省略した場合、最後に設定された背景色になります。
	 */
	void PostString(LPCTSTR lpszItem, COLORREF colorText = 0, COLORREF colorBack = 0)
	{
		PostString(-1, lpszItem, colorText, colorBack);
	}

	/**
	 * [追加] 文字列追加.
	 *		指定した色で文字列を最後に追加します。改行コードがあれば複数行にして追加します。
	 *	@note 別スレッドからでも追加が可能です。
	 *	@param lpszItem 挿入する文字列。
	 *	@param colorText 文字色指定(COLORREF)。省略した場合、最後に設定された文字色になります。
	 *	@param colorBack 背景色指定(COLORREF)。省略した場合、最後に設定された背景色になります。
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
	 * [通知] 作成完了通知.
	 *	@note ウィンドウ作成完了後に通知されます。
	 */
	virtual void OnCreate(void)
	{
	}

	/**
	 * [通知] 破棄通知.
	 *	@note ウィンドウ破棄直前に通知されます。
	 */
	virtual void OnDestroy(void)
	{
	}

	/**
	 * [通知] コマンド通知
	 *	@param wParam メニューID
	 */
	virtual void OnCommand(WPARAM wParam)
	{
		switch ( wParam )
		{
		case MenuId_Hide://隠す
			::ShowWindow(*this, SW_HIDE);
			break;
		case MenuId_Clear://クリア
			m_pListBox->ResetContent();
			break;
		}
	}

private:
	/**
	 * [通知] 受信メッセージ通知.
	 *	@note WM_CREATE は、通知されません。
	 *	@note WM_DESTROY を最後にウィンドウは破棄されます。
	 *	@param[out] _result 結果格納。 true でリターンしたときに使用されます。
	 *	@param[in] hWnd ウィンドウハンドル
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@retval true 処理済(DefWindowProc()を呼び出さなくなります)
	 *	@retval false 未処理
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
	/// 内部LISTコントロール
	class CInListBox : public CDelayedRedrawAddinT<CColorListBox>
	{
		DEFSUPER(CDelayedRedrawAddinT<CColorListBox>);
	protected:
		/**
		 * [通知] for processing Windows messages.
		 *		メッセージ受信したらコールされます。
		 *		ListBox 関係のメッセージをフックしています。
		 *	@note CWndのメソッドをオーバーライドしています。
		 *	@param message メッセージ
		 *	@param wParam WPARAM
		 *	@param lParam LPARAM
		 *	@return リザルト。
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

	HWND			m_hParentWnd;		///< 親ウィンドウ
	CListBox*		m_pListBox;			///< 使用しているリストボックス
	CInListBox		m_logColorList;		///< カラーモード用リストボックス
	CLightListBox	m_logBwList;		///< 白黒用リストボックス
	CDummyWindow	m_dmyWnd;			///< ウィンドウ管理
	CMenu			m_menu;				///< メニュー
	CMenu			m_subMenu1;			///< サブメニュー１
	CMenu			m_subMenu2;			///< サブメニュー２
	UINT			m_parentMenuItem;	///< 親メニューのアイテム
	bool			m_isColorMode;		///< カラーモード
};



}; // MFC
}; // TNB
