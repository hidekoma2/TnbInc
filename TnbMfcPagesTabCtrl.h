#pragma once
/**
 *@file
 * ページ付きタブコントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ページ付きタブコントロールテンプレート
 *
 *		タブの中に、指定のダイアログを持つことが出来るタブコントロールです。
 *
 *	@par使い方
 *		リソースエディタでタブコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CTabCtrl）を本クラスに置き換えて使用します。
 *		CDialog のサブクラスを作成し、 AddItem で追加するだけです
 * 
 *	@note ウィンドウは AddItem() 時にクリエイトします。 その際 WM_INITDIALOG メッセージを通知します。
 *
 *	@note タブ切替え時、新たに表示される画面に WM_ACTIVATE メッセージ（パラメタは WA_ACTIVE ）を通知します。
 *		今まで表示していた画面にも WM_ACTIVATE メッセージ（パラメタは WA_INACTIVE ）を通知します。
 *
 *	@par必要ファイル
 *			TnbMfcPagesTabCtrl.h
 * 
 *	@date 09/04/06	新規作成
 *	@date 09/04/17	TCM_SETCURSEL にも対応。
 *	@date 09/05/26	CE用でも動作するように対応。
 *	@date 09/06/16	WM_ACTIVATE メッセージ対応。
 *	@date 10/03/23	Zオーダー、整理
 *	@date 10/04/15	テンプレート化
 *	@date 11/05/26	GetWndCurSel() 追加。
 *	@date 11/06/07	GetWndItem() 追加。
 *	@date 19/06/27  WM_MOVE メッセージにも対応。
 */
template<typename TAB = CTabCtrl>
class CPagesTabCtrlT : public TAB
{
	DEFSUPER(TAB);
public:

	/**
	 * [追加] アイテム追加.
	 *	@param lpszName アイテムネーム（タブ名）。
	 *	@param pWnd ページになる CDialog のサブクラスのインスタンス（クリエイトしないこと）。
	 *				本インスタンスで保持しないので、破棄しないでください。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 追加成功。値は追加したアイテムNo。
	 */
	template<typename TYP>
	int AddItem(LPCTSTR lpszName, TYP* pWnd)
	{
		return AddItem(lpszName, pWnd, TYP::IDD);
	}

	/**
	 * [追加] アイテム追加.
	 *	@param lpszName アイテムネーム（タブ名）。
	 *	@param pWnd ページになる CDialog のサブクラスのインスタンス（クリエイトしないこと）。
	 *				本インスタンスで保持しないので、破棄しないでください。
	 *	@param idd ページになる CDialog のリソースID。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 追加成功。値は追加したアイテムNo。
	 */
	int AddItem(LPCTSTR lpszName, CDialog* pWnd, UINT idd)
	{
		CWnd* pParent = GetParent();
		CDialog* P = pWnd;
		if ( ::IsWindow(P->GetSafeHwnd()) )
		{
			ASSERT0( false, "AddItem", "既にクリエイトされているObjectが指定されました。" );
			return -1;
		}
		if ( ! P->Create(idd, pParent) )
		{
			ASSERT0( false, "AddItem", "クリエイトに失敗しました。" );
			return -1;
		}
		P->SetParent(pParent);
		this->SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		P->SetWindowText(lpszName);
		DWORD style = P->GetStyle();
		if ( (style & WS_CHILD) == 0 )
		{
			ASSERT0( false, "AddItem", "ダイアログのリソースに、チャイルドのスタイルがありません。" );
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
	 * [検索] アイテム検索.
	 *	@note 指定のウィンドウハンドル(アイテム)を持っているアイテムNoを返します。
	 *	@param page ウィンドウハンドル
	 *	@retval マイナス なかった。
	 *	@retval 0以上 検索成功。値は発見したアイテムNo。
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
	 * [取得] ウィンドウ取得.
	 *		指定のインデックスに登録された CWnd のポインタを返します。
	 *		この実体は、 AddItem() で登録されたインスタンスになっています。
	 *	@param nItem 取得するアイテムのインデックス
	 *	@retval NULL以外 成功。値は CWnd のポインタ
	 *	@retval NULL 失敗。選択されていない。
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
	 * [選択] タブ選択.
	 *	@param nItem 選択するアイテムのインデックス
	 *	@retval マイナス 失敗。
	 *	@retval 0以上 成功。値は直前に選択されていたインデックス。
	 */
	int SetCurSel(int nItem) 
	{
		return _super::SetCurSel(nItem); 
	}

	/**
	 * [選択] タブ選択.
	 *	@param page ウィンドウハンドル。
	 *	@retval マイナス 失敗。
	 *	@retval 0以上 成功。値は直前に選択されていたインデックス。
	 */
	int SetCurSel(HWND page)
	{
		return _super::SetCurSel(FindItem(page));
	}

	/**
	 * [取得] 選択アイテム取得.
	 *		SetCurSel() で選択されたアイテムのインデックスを返します。
	 *	@retval 0以上 選択されているアイテムのインデックス。
	 *	@retval マイナス 選択されていない。
	 */
	int GetCurSel(void) const 
	{
		return _super::GetCurSel();
	}

	/**
	 * [取得] 選択ウィンドウ取得. 
	 *		SetCurSel() で選択されたアイテムに登録されている CWnd のポインタを返します。 
	 *		この実体は、 AddItem() で登録されたインスタンスになっています。
	 *	@retval NULL以外 成功。値は CWnd のポインタ
	 *	@retval NULL 失敗。選択されていない。
	 */
	CWnd* GetWndCurSel(void) const
	{
		return GetWndItem(_super::GetCurSel());
	}

	/**
	 * [削除] アイテム削除.
	 *	@param nItem アイテムNo。
	 *	@retval TRUE 削除成功。
	 *	@retval FALSE 失敗。
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
	 * [削除] 全アイテム削除.
	 *	@retval TRUE 削除成功。
	 *	@retval FALSE 失敗。
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

	CSimpleVectorT<CWnd*> m_apPages;	///< ページウィンドウ群

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
	}
	
	/**
	 * [通知] for notifications from parent
	 *	@note
	 *		CWndのメソッドをオーバーライドしています。
	 *		メッセージ受信したらコールされます。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE	処理済。
	 *	@retval FALSE	未処理。
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
	/// 選択
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
	/// 子供全部リサイズ
	void m_ResizeAll(void)
	{
		CRect r;
		_super::GetWindowRect(&r);			// タブコントロールのウィンドウ領域（スクリーン座標系）
		_super::GetParent()->ScreenToClient(&r);	// クライアント座標系に修正
		_super::AdjustRect(FALSE, &r);		// ウィンドウ領域に基づき、表示部の領域に調整
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
 * ページ付きタブコントロール
 *
 *		タブの中に、指定のダイアログを持つことが出来るタブコントロールです。
 *
 *	@par使い方
 *		リソースエディタでタブコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CTabCtrl）を本クラスに置き換えて使用します。
 *		CDialog のサブクラスを作成し、 AddItem で追加するだけです。
 *
 *	@note タブ切替え時、AddItem で追加した画面クラスに、 WM_ACTIVATE メッセージを Sendします。
 *		今まで選択されていた画面クラスに WA_INACTIVE 、新しく選択された画面クラスに WA_ACTIVE が
 *		通知されます。
 *
 *	@par使用例
 *		\code
 *		[.h ファイル]
 *
 *		class CFooDlg : public CDialog {
 *				;
 *			CPagesTabCtrl	m_pagesTab;			//タブコントロール		
 *			CPageFooAbcDlg	m_pageFooAbcDlg;	//タブ内サブダイアログ
 *			CPageFooXyzDlg	m_pageFooXyzDlg;	//タブ内サブダイアログ
 *			//サブダイアログは、Createしないこと。 スタイルはチャイルドにしておくこと。
 *
 *		[.cpp ファイル]
 *		
 *		BOOL CFooDlg::OnInitDialog() {
 *				;
 *			m_pagesTab.AddItem("ABC", &m_pageFooAbcDlg);
 *			m_pagesTab.AddItem("XYZ", &m_pageFooXyzDlg);
 *
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbMfcPagesTabCtrl.h
 * 
 *	@date 10/04/25 新規宣言
 */
class CPagesTabCtrl : public CPagesTabCtrlT<>
{
};



}; // MFC
}; // TNB
