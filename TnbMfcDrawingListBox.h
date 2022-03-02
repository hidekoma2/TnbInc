#pragma once
/**
 *@file
 * 描画情報ListBox関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbDrawingContainer.h"
#include "TnbVector.h"
#include "TnbMfcBitmapDC.h"
#include "TnbMfcAbstractDrawingCtrl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報ListBoxコントロール
 *
 *		行単位で表示する描画情報を指定出来ます。
 *
 *	@par使い方
 *		リソースエディタでリストボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListBox）を本クラスに置き換えて使用します。\n
 *		リストボックスのプロパティの「オーナー描画(O)」を「可変」にしておいてください。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingListBox.h
 * 
 *	@date 07/12/07	新規作成
 *	@date 10/01/07	Destroy時のマーク関係の初期化漏れ修正。
 *	@date 10/04/26	構成変更
 *	@date 11/08/03	描画時、 IDrawable::DrawEx() を使うように変更。 CalcDrawParam() 追加。
 */
class CDrawingListBox : public CListBox, public CAbstractDrawingCtrl
{
	DEFSUPER(CListBox);
	typedef CAbstractDrawingCtrl	_mark;			///< SelectMark管理名再宣言
public:

	/// コンストラクタ
	CDrawingListBox(void) : _super(), m_itemsWidth(0)
	{
	}

	/// デストラクタ
	~CDrawingListBox(void)
	{
		m_ResetContent();
	}

	/**
	 * [取得] 描画情報取得.
	 *	@param index 取得する位置.
	 *	@retval NULL エラー.
	 *	@retval NULL以外 描画情報。delete してはいけません。
	 */
	const IDrawable* GetDrawing(int index) const
	{
		if ( ! m_drawersList.IsInRange(index) )
		{
			return NULL;
		}
		return m_drawersList[index];
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
		int r = _super::AddString(_T(""));
		if ( r >= 0 )
		{
			if ( ! m_InsertDrawing(r, draw) )
			{
				r = LB_ERR;
			}
		}
		return r;
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
	 * [追加] 描画情報一行挿入.
	 *	@note 一行挿入します。
	 *	@param index 挿入する位置
	 *	@param draw 描画情報
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 追加したインデックス
	 */
	int InsertDrawing(int index, const IDrawable& draw)
	{
		int r = _super::InsertString(index, _T(""));
		if ( r >= 0 )
		{
			if ( ! m_InsertDrawing(r, draw) )
			{
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
	 * [追加] 描画情報変更.
	 *	@note 設定した描画情報を変更します。
	 *	@param index 変更する位置
	 *	@param draw 描画情報
	 *	@retval false エラー。
	 *	@retval true 成功
	 */
	int ResetDrawing(int index, const IDrawable& draw)
	{
		return m_ResetDrawing(index, draw);
	}

	/**
	 * [削除] 一行削除.
	 *	@param index 削除する位置
	 *	@retval LB_ERR エラー。
	 *	@retval 0以上 成功。数字は残りの数。
	 */
	int DeleteDrawing(UINT index)
	{
		int r = _super::DeleteString(index);
		if ( r >= 0 )
		{
			m_drawersList.Remove(index);
		}
		return r;
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
		m_ResetContent();
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			_super::SetHorizontalExtent(m_itemsWidth);
			_super::ResetContent();
		}
	}

protected:

	/**
	 * [取得] 描画用パラメータ計算.
	 *		描画情報の IDrawable::DrawEx() の lParam に渡す値を計算します。
	 *	@note 本メソッドでは、 itemNo を lParam としてます。
	 *	@param itemNo アイテム No.
	 *	@return パラメータ.
	 */
	virtual LPARAM CalcDrawParam(DWORD_PTR itemNo) const
	{
		return itemNo;
	}
	
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
			{
				HDC hdc = reinterpret_cast<HDC>(wParam);
				CRect rect;
				GetClientRect(rect);
				int y = 0;
				for ( int i = GetTopIndex(); i < GetCount(); i++ )
				{
					y += GetItemHeight(i);
				}
				int h = rect.Height();
				if( h > y )
				{
					_mark::DrawBackColor(hdc, CRect(0, y, rect.Width(), h));
				}
				return 0;
			}
			break;
		case WM_PAINT:
			if ( GetCount() == 0 )
			{
				CPaintDC dc(this);
				return 0;
			}
			else
			{
				_mark::BeginItemPaint();
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				_mark::EndItemPaint();
				return r;
			}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			Invalidate();
			break;
		case WM_DESTROY:
			m_ResetContent();
			_mark::AllReset();
			break;
		case LB_RESETCONTENT: //全消し
			m_ResetContent();
			_super::SetHorizontalExtent(m_itemsWidth);
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
			ASSERT0(dwStyle & (LBS_OWNERDRAWVARIABLE | LBS_OWNERDRAWFIXED), "CDrawingListBox", "[オーナー描画]を[なし]以外にしてください");
			ASSERT0((dwStyle & LBS_SORT) == 0, "CDrawingListBox", "[ソート]はOFFにしてください");
			//			ASSERT0(dwStyle & LBS_EXTENDEDSEL, "PreSubclassWindow", "");
		#endif
		_super::PreSubclassWindow();
	}

	/**
	 * オーナードロー時.
	 *	ドロー時Itemの高さ確認のためにコールされる
	 *	@param lpMeasureItemStruct 情報
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		if ( lpMeasureItemStruct->CtlType != ODT_LISTBOX || ToInt(lpMeasureItemStruct->CtlID) != _super::GetDlgCtrlID() )
		{
			return;
		}
	}
		
	/**
	 * [通知] オーナードロー処理.
	 * 		オーナードロー時コールされます。
	 *	@note 描画情報を用意することで、任意に描画させることも出来ます。
	 *	@param lpDrawItemStruct 描画情報.
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		if ( lpDrawItemStruct->CtlType != ODT_LISTBOX || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		UINT index = lpDrawItemStruct->itemID;
		HDC dc = lpDrawItemStruct->hDC;
		const CRect& rc = lpDrawItemStruct->rcItem;
		CSize sz = rc.Size();
		CBitmapImage bi;
		if ( bi.Set(sz.cx, sz.cy) )
		{
			LPARAM lParam = CalcDrawParam(index);
			CBitmapDC bmpDC(&bi);
			_mark::DrawBackground(bmpDC, CRect(CPoint(0, 0), sz), lParam);
			if ( GetSelectMarkType() == BACKGROUND )
			{
				m_DrawSelectMaker(bmpDC, sz, lpDrawItemStruct->itemState, lParam);
			}
			const IDrawable* pDraw = GetDrawing(index);
			if ( pDraw != NULL )
			{
				SIZE s;
				pDraw->GetSize(s);
				pDraw->DrawEx(bmpDC, 0, (sz.cy - s.cy) / 2, lParam);
			}
			else if ( ::IsWindow(m_hWnd) && static_cast<INDEX>(GetCount()) > index ) 
			{
				CString str;
				GetText(index, str);
				_mark::SetDefaultTextDrawer(this);
				_mark::DrawText(bmpDC, CRect(0, 0, sz.cx, sz.cy), str, lParam);
			}
			if ( GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(bmpDC, sz, lpDrawItemStruct->itemState, lParam);
			}
			if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
			{
				DrawFocusMark(bmpDC, CRect(CPoint(0, 0), sz), lParam);
			}
			if ( ! ::IsWindowEnabled(GetSafeHwnd()) )
			{
				DrawDisabledStyle(bmpDC, CRect(CPoint(0, 0), sz), lParam);
			}
			bmpDC.Draw(dc, rc.left, rc.top);
		}
	}

private:

	// アイテム全けし
	void m_ResetContent(void)
	{
		m_itemsWidth = 0;
		m_drawersList.RemoveAll();
	}
	//アイテムの幅計算（結果は m_iItemWidth に代入）
	void m_CalcItemWidth(bool withReheight = false)
	{
		m_itemsWidth = 0;
		loop ( i, m_drawersList.GetSize() )
		{
			SIZE size;
			IDrawable::Ptr P = m_drawersList[i];
			if ( ! P.IsNull() && m_drawersList[i]->GetSize(size) )
			{
				if ( m_itemsWidth < static_cast<DWORD>(size.cx) )
				{
					m_itemsWidth = size.cx;
				}
				if ( withReheight )
				{
					_super::SetItemHeight(ToInt(i), size.cy);
				}
			}
		}
	}
	/// アイテム追加
	bool m_InsertDrawing(int index, const IDrawable& draw)
	{
		if ( m_drawersList.Insert(index, draw.Clone()) )
		{
			SIZE size;
			draw.GetSize(size);
			m_CalcItemWidth(true);
			_super::SetHorizontalExtent(m_itemsWidth);
			_super::SetItemHeight(index, size.cy);
			return true;
		}
		_super::DeleteString(index);
		return false;
	}
	/// アイテム変更
	bool m_ResetDrawing(int index, const IDrawable& draw)
	{
		if ( m_drawersList.Set(index, draw.Clone()) )
		{
			SIZE size;
			draw.GetSize(size);
			m_CalcItemWidth();
			_super::SetHorizontalExtent(m_itemsWidth);
			_super::SetItemHeight(index, size.cy);
			return true;
		}
		return false;
	}
	/// 選択マーク描画
	void m_DrawSelectMaker(HDC dc, const SIZE& sz, UINT itemState, LPARAM lParam)
	{
		HWND hFocus = ::GetFocus();
		bool r = (hFocus == m_hWnd);
		if ( ! r && ::IsWindow(m_hWnd) )
		{
			CWnd* P = GetOwner();
			if ( P != NULL )
			{
				r = (hFocus == P->GetSafeHwnd());
			}
		}
		if ( (itemState & CDIS_SELECTED) != 0 )
		{
			_mark::DrawSelectMark(dc, CRect(CPoint(0, 0), sz), r, lParam);
		}
	}

	DWORD							m_itemsWidth;	///< 全Itemの幅
	CAutoVectorT<IDrawable::Ptr>	m_drawersList;	///< それぞれの行の描画情報
};



}; // MFC
}; // TNB
