#pragma once
/**
 *@file
 * ページコントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ページコントロール
 *
 *		ダイアログ上の本コントロール内に、複数の画面を持ち、ページのように切替えられるページコントロールです。
 *
 *		本コントロールの大きさに応じて、内のダイアログもリサイズされます。
 *	
 *	@par使い方
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。\n
 *		ページとなる画面は普通にダイアログとして作成します（スタイルはチャイルドにしておく）。
 *		ダイアログの CDialog のサブクラスを作成し、 AddItem() で追加していきます。\n
 *		表示される画面は SetItemSel() で指定します。
 *
 *	@note ウィンドウは AddItem() 時にクリエイトします。 その際 WM_INITDIALOG メッセージを通知します。
 *
 *	@note 表示画面切替え時、新たに表示される画面に WM_ACTIVATE メッセージ（パラメタは WA_ACTIVE ）を通知します。
 *		今まで表示していた画面にも WM_ACTIVATE メッセージ（パラメタは WA_INACTIVE ）を通知します。
 *
 *	@par使用例
 *		\code
 *		[.h ファイル]
 *
 *		class CFooDlg : public CDialog {
 *				;
 *			CPagesCtrl		m_pageTab;			// ページコントロール
 *			CFooAbcSubDlg	m_subFooAbcDlg;		// タブ内サブダイアログ
 *			CFooXyzSubDlg	m_subFooXyzDlg;		// タブ内サブダイアログ
 *
 *		[.cpp ファイル]
 *		
 *		BOOL CFooDlg::OnInitDialog() {
 *				;
 *			m_pageTab.AddItem(&m_subFooAbcDlg); //サブダイアログは、Createしないでインスタンスを渡す。
 *			m_pageTab.AddItem(&m_subFooXyzDlg);
 *
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbMfcPagesCtrl.h
 * 
 *	@date 09/11/09	新規作成
 *	@date 10/03/23	Zオーダー、整理
 *	@date 11/05/20	GetItemSel() , GetWndItemSel() 追加。
 *	@date 11/06/07	GetWndItem() 追加。
 *	@date 11/06/28	コントロールのオーダーが末になってしまう問題を修正
 */
class CPagesCtrl : public CStatic
{
	DEFSUPER(CStatic);
public:

	/// コンストラクタ
	CPagesCtrl(void) : m_currentIndex(-1)
	{
	}

	/**
	 * [追加] アイテム追加.
	 *	@param pWnd ページになる CDialog のサブクラスのインスタンス。
	 *				本クラスで Create するので、先に Create しないでください。
	 *				また、pWnd 本インスタンスで保持しないので、破棄しないでください。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 追加成功。値は追加したアイテムNo。
	 */
	template<typename TYP>
	int AddItem(TYP* pWnd)
	{
		return AddItem(pWnd, TYP::IDD);
	}

	/**
	 * [追加] アイテム追加.
	 *	@param pWnd ページになる CDialog のサブクラスのインスタンス。
	 *				本クラスで Create するので、先に Create しないでください。
	 *				また、pWnd 本インスタンスで保持しないので、破棄しないでください。
	 *	@param idd ページになる CDialog のリソースID。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 追加成功。値は追加したアイテムNo。
	 */
	int AddItem(CDialog* pWnd, UINT idd)
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
		P->SetWindowPos(this, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		this->SetWindowPos(P, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		DWORD style = P->GetStyle();
		if ( (style & WS_CHILD) == 0 )
		{
			ASSERT0( false, "AddItem", "ダイアログのリソースに、チャイルドのスタイルがありません。" );
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
	 * [取得] アイテム数取得.
	 *	@note AddItem() した数を取得できます。
	 *	@return アイテム数.
	 */
	size_t GetItemCount(void) const
	{
		return m_apPages.GetSize();
	}

	/**
	 * [取得] 選択アイテム取得.
	 *		SetItemSel() されたアイテムのインデックスを返します。
	 *	@retval 0以上 選択されているアイテムのインデックス。
	 *	@retval マイナス 選択されていない。
	 */
	int GetItemSel(void) const
	{
		return m_currentIndex;
	}

	/**
	 * [取得] 選択アイテム取得.
	 *		SetItemSel() されたアイテムの CWnd のポインタを返します。
	 *		この実体は、 AddItem() で登録されたインスタンスになっています。
	 *	@retval NULL以外 成功。値は 選択されているアイテムの CWnd のポインタ
	 *	@retval NULL 失敗。選択されていない。
	 */
	CWnd* GetWndItemSel(void) const
	{
		return GetWndItem(m_currentIndex);
	}

	/**
	 * [選択] アイテム選択.
	 *	@param nItem 選択するアイテムのインデックス
	 *	@retval マイナス 失敗。
	 *	@retval 0以上 成功。値は直前に選択されていたインデックス。
	 */
	int SetItemSel(int nItem) 
	{
		int r = m_currentIndex;
		m_SetCurSel(nItem);
		m_currentIndex = nItem;
		return r;
	}

	/**
	 * [選択] タブ選択.
	 *	@param page ウィンドウハンドル。
	 *	@retval マイナス 失敗。
	 *	@retval 0以上 成功。値は直前に選択されていたインデックス。
	 */
	int SetItemSel(HWND page)
	{
		return SetItemSel(FindItem(page));
	}

	/**
	 * [削除] アイテム削除.
	 *	@param nItem アイテムNo。
	 *	@retval true 削除成功。
	 *	@retval false 失敗。
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
	 * [削除] 全アイテム削除.
	 *	@note 登録したウィンドウは破棄されます。
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

	CSimpleVectorT<CWnd*> m_apPages;	///< ページウィンドウ群

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_currentIndex = -1;
		_super::ShowWindow(SW_HIDE);
		_super::PreSubclassWindow();
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

	/// 選択
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
	/// 子供全部リサイズ
	void m_ResizeAll(void)
	{
		CRect r;
		_super::GetWindowRect(&r);
		_super::GetParent()->ScreenToClient(&r);	// クライアント座標系に修正
		loop ( i, m_apPages.GetSize() )
		{
			m_apPages[i]->MoveWindow(r.left, r.top, r.Width(), r.Height());
		}
	}
};



}; // MFC
}; // TNB
