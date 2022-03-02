#pragma once
/**
 *@file
 * 描画情報ComboBox関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbMfcDrawingListBox.h"
#include "TnbMfcDrawingButton.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報ListBoxコントロール
 *
 *		行単位で表示する描画情報を指定出来ます。
 *
 *	@par使い方
 *		リソースエディタでコンボボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CComboBox）を本クラスに置き換えて使用します。\n
 *		リストボックスのプロパティの「オーナー描画(O)」を「可変」にしておいてください。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingComboBox.h
 * 
 *	@date 08/03/18	新規作成
 *	@date 10/04/26	構成変更
 *	@date 11/08/11	サブクラス化した時、ListBoxのハンドルがわかればサブクラス化するようにした。
 */
class CDrawingComboBox : public CComboBox
{
	DEFSUPER(CComboBox);
public:

	/// コンストラクタ
	CDrawingComboBox(void) : _super(), m_isUseDrawingButton(false)
	{
	}

	/// デストラクタ
	~CDrawingComboBox(void)
	{
	}

	/**
	 * [取得] 描画情報取得.
	 *	@param index 取得する位置.
	 *	@retval NULL エラー.
	 *	@retval NULL以外 描画情報。delete してはいけません。
	 */
	const IDrawable* GetDrawing(int index) const
	{
		return m_listBox.GetDrawing(index);
	}

	/**
	 * [追加] 文字列一行追加.
	 *	@note 一行追加します。
	 *	@param lpszItem 文字列
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 追加したインデックス
	 */
	int AddString(LPCTSTR lpszItem)
	{
		return _super::AddString(lpszItem);
	}
	
	/**
	 * [追加] 描画情報一行追加.
	 *	@note 一行追加します。
	 *	@param draw 描画情報
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 追加したインデックス
	 */
	int AddDrawing(const IDrawable& draw)
	{
		m_InitListBox();
		int r = _super::AddString(_T(""));
		if ( r >= 0 )
		{
			if ( m_listBox.ResetDrawing(r, draw) )
			{
				m_CheckHeight(draw);
			}
			else
			{
				_super::DeleteString(r);
				r = LB_ERR;
			}
		}
		return r;
	}

	/**
	 * [追加] 文字列一行挿入.
	 *	@note 一行挿入します。
	 *	@param index 挿入する位置
	 *	@param lpszItem 文字列
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 追加したインデックス
	 */
	int InsertString(int index, LPCTSTR lpszItem)
	{
		return _super::InsertString(index, lpszItem);
	}

	/**
	 * [追加] 描画情報一行挿入.
	 *	@note 一行挿入します。
	 *	@param index 挿入する位置
	 *	@param draw 描画情報
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 追加したインデックス
	 */
	int InsertDrawing(int index, const IDrawable& draw)
	{
		m_InitListBox();
		int r = _super::InsertString(index, _T(""));
		if ( r >= 0 )
		{
			if ( m_listBox.ResetDrawing(r, draw) )
			{
				m_CheckHeight(draw);
			}
			else
			{
				_super::DeleteString(r);
				r = LB_ERR;
			}
		}
		return r;
	}

	/**
	 * [削除] 一行削除.
	 *	@param index 削除する位置
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 成功。数字は残りの数。
	 */
	int DeleteDrawing(UINT index)
	{
		m_InitListBox();
		_super::DeleteString(index);
		return m_listBox.DeleteDrawing(index);
	}

	/**
	 * [削除] 一行削除.
	 *	@param index 削除する位置
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 成功。数字は残りの数。
	 */
	int DeleteString(UINT index)
	{
		return DeleteDrawing(index);
	}
	
	/**
	 * [削除] 全描画情報削除.
	 */
	void ResetContent(void)
	{
		m_InitListBox();
		_super::ResetContent();
		m_listBox.ResetContent();
	}

	/**
	 * [設定] デフォルトのマーク描画指定.
	 *	@note フォーカスマーク、選択マークをWindow標準と同じにします。
	 */
	void SetDefaultMarkDrawer(void)
	{
		m_listBox.SetDefaultMarkDrawer();
	}

	/**
	 * [設定] 背景描画指定.
	 *	@param draw Resize() が有効な各アイテムの背景描画情報。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetBackgroundDrawer(const IDrawable& draw)
	{
		return m_listBox.SetBackgroundDrawer(draw);
	}

	/**
	 * [設定] 選択マーク描画指定.
	 *	@note アクティブ時と非アクティブ時で同じ選択マークを使います。
	 *	@param draw Resize() が有効な選択マークの描画情報。
	 *	@param type 選択マークのタイプ。省略すると、 OVERLAP になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw, CAbstractDrawingCtrl::ESelectMarkType type = CAbstractDrawingCtrl::OVERLAP)
	{
		return m_listBox.SetSelectMarkDrawer(draw, type);
	}
	
	/**
	 * [設定] 選択マーク描画指定.
	 *	@param draw1 Resize() が有効なアクティブ時の選択マークの描画情報。
	 *	@param draw2 Resize() が有効な非アクティブ時の選択マークの描画情報。
	 *	@param type 選択マークのタイプ。省略すると、 OVERLAP になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw1, const IDrawable& draw2, CAbstractDrawingCtrl::ESelectMarkType type = CAbstractDrawingCtrl::OVERLAP)
	{
		return m_listBox.SetSelectMarkDrawer(draw1, draw2, type);
	}
	
	/**
	 * [設定] フォーカスマーク描画指定.
	 *	@param draw Resize() が有効なフォーカスマークの描画情報。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetFocusMarkDrawer(const IDrawable& draw)
	{
		return m_listBox.SetFocusMarkDrawer(draw);
	}

	/**
	 * [設定] 背景色設定.
	 *	@param color 色
	 */
	void SetBackColor(COLORREF color)
	{
		m_listBox.SetBackColor(color);
	}

	/**
	 * [設定] テキスト描画情報指定
	 *	@note 指定しない場合でも、標準相当の描画が行われます。
	 *	@param text テキスト描画
	 */
	void SetTextDrawer(const CTextDrawer& text)
	{
		m_listBox.SetTextDrawer(text);
	}

	/**
	 * [参照] ボタンコントロールクラス参照.
	 *		本コンボボックスが持つボタンコントロールクラスの参照を返します。
	 *	@note UseDrawingButton() メソッドをコールしていない場合、無効なコントロールが返されます。
	 *	@return ボタンコントロールの参照
	 */
	CDrawingButton& ReferButtonControl(void)
	{
		return m_button;
	}

	/**
	 * [設定] ボタン設定.
	 *		本コンボボックスがカスタマイズ可能のボタンを使うか否か、指定します。
	 *	@param r true なら CDrawingButton を使用します。
	 *			false なら標準のボタンを使用します。
	 */
	void UseDrawingButton(bool r = true)
	{
		m_isUseDrawingButton = r;
		if ( ::IsWindow(m_button) )
		{
			if ( ! r )
			{
				m_button.DestroyWindow();
			}
			RedrawWindow();
		}
	}

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
		switch ( message )
		{
		case WM_ERASEBKGND: //背景
			return 0;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			Invalidate();
			break;
		case WM_DESTROY:
			m_listBox.ResetContent();
			if ( m_listBox.GetSafeHwnd() != NULL )
			{
				m_listBox.UnsubclassWindow();
			}
			m_button.DestroyWindow();
			break;
		case WM_CTLCOLORLISTBOX:
			if ( m_listBox.GetSafeHwnd() == NULL )
			{
				m_listBox.SubclassWindow(reinterpret_cast<HWND>(lParam));
				m_listBox.SetOwner(this);
			}
			break;
		case WM_SETREDRAW:
			if ( m_listBox.GetSafeHwnd() != NULL )
			{
				m_listBox.SendMessage(WM_SETREDRAW, wParam, lParam);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if ( ::IsWindow(m_button) )
			{
				if ( ! m_validRect.PtInRect(CPoint(lParam)) )
				{
					return 0;
				}
			}
			break;
		case WM_COMMAND:
			if ( wParam == BUTTON_CTRLID )
			{
				SetFocus();
				ShowDropDown();
				m_listBox.RedrawWindow();
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		#ifdef _DEBUG
			DWORD dwStyle = _super::GetStyle();
			ASSERT0(dwStyle & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE), "CDrawingComboBox", "[オーナー描画]を[なし]以外にしてください");
			ASSERT0(dwStyle & CBS_DROPDOWNLIST, "CDrawingComboBox", "[]を[ドロップダウンリスト]にしてください");
			ASSERT0(dwStyle & CBS_HASSTRINGS, "CDrawingComboBox", "[文字列]を[あり]にしてください");
			ASSERT0((dwStyle & CBS_SORT) == 0, "CDrawingComboBox", "[ソート]を[false]にしてください");
		#endif
		_super::PreSubclassWindow();
		const int _CB_GETCOMBOBOXINFO = 0x0164;	
		_COMBOBOXINFO cbi;
		cbi.cbSize = sizeof(cbi);
		if ( SendMessage(_CB_GETCOMBOBOXINFO, 0, reinterpret_cast<LPARAM>(&cbi)) )
		{
			if ( m_listBox.GetSafeHwnd() == NULL )
			{
				HWND h = cbi.hwndList;
				m_listBox.SubclassWindow(h);
				m_listBox.SetOwner(this);
			}
		}
	}

	/**
	 * [通知] オーナードロー処理.
	 * 		オーナードロー時コールされます。
	 *	@param lpDrawItemStruct 描画情報.
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		if ( lpDrawItemStruct->CtlType != ODT_COMBOBOX || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		const CRect& rc = lpDrawItemStruct->rcItem;
		if ( m_isUseDrawingButton && ! ::IsWindow(m_button) )
		{
			m_validRect = rc;
			DWORD style = BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE;
			int x = ::GetSystemMetrics(SM_CXVSCROLL);
			CRect r(rc.right + 1, rc.top, rc.right + x + 1, rc.bottom + 1);
			if ( (GetExStyle() & WS_EX_RIGHT) != 0 )
			{
				r.left = rc.left - x - 1;
				r.right = rc.left - 1;
			}
			m_button.Create(_T("Button"), _T(""), style, r, this, BUTTON_CTRLID);
			
		}
		if ( ::IsWindow(m_listBox) )
		{
			lpDrawItemStruct->CtlType = ODT_LISTBOX;
			lpDrawItemStruct->hwndItem = m_listBox.GetSafeHwnd();
			lpDrawItemStruct->CtlID = m_listBox.GetDlgCtrlID();
			m_listBox.MyDrawItem(lpDrawItemStruct);
		}
	}

	/**
	 * オーナードロー時.
	 *	ドロー時Itemの高さ確認のためにコールされる
	 *	@param lpMeasureItemStruct 情報
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		if ( lpMeasureItemStruct->CtlType != ODT_COMBOBOX || ToInt(lpMeasureItemStruct->CtlID) != _super::GetDlgCtrlID() )
		{
			return;
		}
		if ( ::IsWindow(m_listBox) )
		{
			lpMeasureItemStruct->CtlType = ODT_LISTBOX;
			lpMeasureItemStruct->CtlID = m_listBox.GetDlgCtrlID();
			m_listBox.MyMeasureItem(lpMeasureItemStruct);
		}
	}

private:
	/// 内部用ListBox
	class CMyListBox : public CDrawingListBox
	{
		DEFSUPER(CDrawingListBox);
	public:
		virtual void MyMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
		{
			_super::MeasureItem(lpMeasureItemStruct);
		}
		virtual void MyDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
		{
			_super::DrawItem(lpDrawItemStruct);
		}
	};
	/// CB_GETCOMBOBOXINFO メッセージのパラメタ
	struct _COMBOBOXINFO
	{
		DWORD cbSize;
		RECT rcItem;
		RECT rcButton;
		DWORD stateButton;
		HWND hwndCombo;
		HWND hwndItem;
		HWND hwndList;
	};
	/**
	 * ListBoxを初期化
	 *（一瞬ドロップしてWM_CTLCOLORLISTBOX を発生させる）
	 */
	void m_InitListBox(void)
	{
		if ( m_listBox.GetSafeHwnd() == NULL )
		{
			CRect rc;
			GetClientRect(&rc);
			MFCLIB::ChangeClientSize(this, -1, rc.bottom + 100);
			ShowDropDown();
			ShowDropDown(FALSE);
			MFCLIB::ChangeClientSize(this, -1, rc.bottom);
		}
	}
	/// 高さチェック
	void m_CheckHeight(const IDrawable& draw)
	{
		int h = _super::GetItemHeight(-1);
		SIZE sz;
		if ( draw.GetSize(sz) )
		{
			if ( h < sz.cy )
			{
				_super::SetItemHeight(-1, sz.cy);
				m_button.DestroyWindow();
			}
		}
	}
	enum { BUTTON_CTRLID = 100 };
	CMyListBox						m_listBox;				///< リスト
	CDrawingButton					m_button;				///< ボタン
	bool							m_isUseDrawingButton;	///< ユーザボタン使用する？
	CRect							m_validRect;			///< 有効範囲
};



}; // MFC
}; // TNB
