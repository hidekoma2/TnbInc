#pragma once
/**
 *@file
 * 描画情報TreeCtrl関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcAbstractCustomDraw.h"
#include "TnbMfcAbstractDrawingCtrl.h"
#include "TnbMap.h"



//TNB Library
namespace TNB {
namespace MFC {



/// チェックボックスの変更
#define TVC_CHECKCHANGED 0xFFFF



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報TreeCtrlコントロール
 *
 *		アイテム単位で表示する描画情報を指定出来ます。
 *
 *	@par使い方
 *		リソースエディタでツリーコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CTreeCtrl）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingTreeCtrl.h
 * 
 *	@date 08/09/11	新規作成
 *	@date 10/01/07	Destroy時のマーク関係の初期化漏れ修正。
 */
class CDrawingTreeCtrl : public CTreeCtrl, public CAbstractDrawingCtrl, protected CAbstractCustomDraw
{
	DEFSUPER(CTreeCtrl);
	typedef CAbstractCustomDraw _custom; 
	typedef CAbstractDrawingCtrl _mark; 

	/// 選択マーク描画
	void m_DrawSelectMaker(HDC dc, const RECT& rect, HTREEITEM h)
	{
		if ( (GetItemState(h, TVIS_SELECTED ) & TVIS_SELECTED) != 0  )
		{
			_mark::DrawSelectMark(dc, rect, ::GetFocus() == m_hWnd);
		}
	}

	/// 削除
	void m_DeleteItem(HTREEITEM h)
	{
		HTREEITEM hh = _super::GetChildItem(h);
		while ( hh != NULL )
		{
			m_DeleteItem(hh);
			hh = _super::GetNextSiblingItem(hh);
		}
		m_map.RemoveKey(h);
	}

	void m_NotifyParent(HTREEITEM h)
	{
		NMTREEVIEW nm = { 0 };
		nm.hdr.hwndFrom = *this;
		nm.hdr.idFrom = GetDlgCtrlID();
		nm.hdr.code = TVN_SELCHANGED;
		nm.action = TVC_CHECKCHANGED;
		nm.itemOld.hItem = h;
		nm.itemNew.hItem = h;
		GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), reinterpret_cast<LPARAM>(&nm));
	}

protected:

	/**
	 * [通知] 描画開始.
	 *		一連の描画の開始を通知します。
	 *	@note dc は以下、 OnItemPainting() OnItemPainted() OnEndItemPaint() と続く一連の描画で共通で
	 *			使うデバイスコンテキストです。 OnItemPainting() などでデバイスコンテキストの
	 *			状態変更して戻すタイミングがない場合、本メソッドで記憶し、 OnEndItemPaint()
	 *			で元に戻すようにします。
	 *	@param dc デバイスコンテキスト。
	 */
	virtual void OnBeginItemPaint(HDC dc) 
	{
		m_lowLine = 0;
		_mark::BeginItemPaint();
	}

	/**
	 * [通知] 描画終了.
	 *		一連の描画の終了を通知します。
	 *	@see OnBeginItemPaint()
	 *	@param dc デバイスコンテキスト。
	 */
	virtual void OnEndItemPaint(HDC dc) 
	{
		bool r = _mark::EndItemPaint();
		// 描画していないところを描画
		CRect rc;
		GetClientRect(rc);
		rc.top = m_lowLine;
		_mark::DrawBackColor(dc, rc);
		// 再描画
		if ( r )
		{
			Invalidate();
		}
	}

	/**
	 * [通知] アイテム描画.
	 *	@note サブアイテム毎に通知されます。
	 *	@param[in,out] P 描画用情報。範囲やアイテムNOが入っています。
	 *	@retval CDRF_NOTIFYITEMDRAW 通常の描画を行う(本メソッドでは何もしていない)。
	 *	@retval CDRF_NEWFONT フォントや文字色を変更した時返します。
	 *	@retval CDRF_SKIPDEFAULT 本メソッドで描画ずみ。
	 */
	virtual LRESULT OnItemPainting(TDrawParam* P)
	{
		HRGN rgn = ::CreateRectRgnIndirect(&P->rect);
		::ExtSelectClipRgn(P->dc, rgn, RGN_DIFF);
		_DeleteObject(rgn);
		m_lowLine = max(m_lowLine, P->rect.bottom);
		// チェックボックスの周りが透過するので書いておく
		_mark::DrawBackColor(P->dc, CRect(0, P->rect.top, P->rect.left, P->rect.bottom));
		return CDRF_NOTIFYITEMDRAW;		// チェックボックス、線を描いてもらう
	}

	/**
	 * [通知] アイテム描画終わり.
	 *	@note サブアイテム描画が終わったあと、アイテム毎に通知されます。
	 *	@param[in] P 描画用情報。 rect は、サブアイテムも含む範囲。
	 */
	virtual void OnItemPainted(const TDrawParam* P) 
	{
		HTREEITEM h = reinterpret_cast<HTREEITEM>(P->itemNo);
		::SelectClipRgn(P->dc, NULL);
		//
		CString str = GetItemText(h);
		IDrawable* pDraw = m_map[h];
		CRect rc = P->rect;
		//
		if ( pDraw != NULL && ! str.IsEmpty() )
		{
			// BG持ち描画情報用処理
			pDraw->Resize(rc.Size());
			pDraw->Draw(P->dc, rc.left, rc.top);
			pDraw->Draw(NULL, rc.left, rc.top);
			if ( _mark::GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(P->dc, rc, h);
			}
			if ( (P->state & CDIS_FOCUS) != 0 )
			{
				_mark::DrawFocusMark(P->dc, rc);
			}
			return;
		}
		rc.OffsetRect(-rc.TopLeft());
		CBitmapImage bi;
		bi.Set(rc.Width(), rc.Height(), _mark::GetBackColor());
		if ( ! bi.IsEmpty() )
		{
			CBitmapDC dc(&bi);
			_mark::DrawBackground(dc, rc);
			if ( _mark::GetSelectMarkType() == BACKGROUND )
			{
				m_DrawSelectMaker(dc, rc, h);
			}
			if ( pDraw != NULL )
			{
				pDraw->Resize(rc.Size());
				pDraw->Draw(dc, 0, 0);
			}
			else
			{
				_mark::SetDefaultTextDrawer(this);
				_mark::DrawText(dc, rc, str);
			}
			if ( _mark::GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(dc, rc, h);
			}
			if ( (P->state & CDIS_FOCUS) != 0 )
			{
				_mark::DrawFocusMark(dc, rc);
			}
			dc.Draw(P->dc, P->rect.left, P->rect.top);
			if ( pDraw != NULL )
			{
				pDraw->Draw(NULL, P->rect.left, P->rect.top);
			}
		}
	}

	/**
	 * [確認] サブアイテム概念の有無
	 *	@retval ture サブアイテムあり。
	 *	@retval false ない。
	 */
	virtual bool HasSubItem(void) const
	{
		return false;
	}

	/**
	 * [確認] アイテムRECT計算
	 *	@note _pa.itemNo, _pa.subItemNo などから、 _pa.rect の設定を行ってください。
	 *	@note _pa.state の調整も可能です。
	 *	@param[in,out] _pa 描画情報
	 *	@param[in] isSub true なら subアイテム用の計算をする。
	 */
	virtual void OnCalcItemRect(TDrawParam& _pa, bool isSub)
	{
		CRect rc;
		_super::GetItemRect(reinterpret_cast<HTREEITEM>(_pa.itemNo), rc, TRUE);
		_pa.rect.left = rc.left;	// テキストの始まる線にする
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
			if ( lpNmHdr->hwndFrom == m_hWnd)
			{
				if ( _custom::OnChildNotify(message, wParam, lParam, _pResult) )
				{
					return TRUE;
				}
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
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
		case TVM_DELETEITEM:
			{
				HTREEITEM h = reinterpret_cast<HTREEITEM>(lParam);
				if ( h == TVI_ROOT )
				{
					m_map.RemoveAll();
				}
				else
				{
					m_DeleteItem(h);
				}
			}
			break;
		case WM_KEYDOWN:
			if ( wParam == ' ' )
			{
				HTREEITEM h = _super::GetSelectedItem();
				if ( h != NULL )
				{
					BOOL old = GetCheck(h);
					LRESULT r =_super::WindowProc(message, wParam, lParam);
					if ( old != GetCheck(h) )
					{
						m_NotifyParent(h);
					}
					return r;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
//				CPoint pos(LOWORD(lParam), HIWORD(lParam));
				CPoint pos(lParam);
				UINT fg;
				HTREEITEM h = _super::HitTest(pos, &fg);
				if ( h != NULL )
				{
					if ( (fg & TVHT_ONITEMRIGHT) != 0 )
					{
						_super::SelectItem(h);
					}
					else if ( (fg & TVHT_ONITEMSTATEICON) != 0 )
					{
						BOOL old = GetCheck(h);
						LRESULT r =_super::WindowProc(message, wParam, lParam);
						ASSERT( old != GetCheck(h) );
						m_NotifyParent(h);
						return r;
					}
				}
			}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case WM_SIZE:
			Invalidate();
			break;
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
			{
				_super::SetRedraw(FALSE);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				_super::SetRedraw(TRUE);
				return r;
			}
			break;
		case WM_ERASEBKGND:
			return 1;
		case TVM_SETBKCOLOR:
			SetBackColor(lParam);
			break;
		case WM_DESTROY:
			_mark::AllReset();
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
		_super::PreSubclassWindow();
		if ( (_super::GetStyle() & TVS_CHECKBOXES) != 0 )
		{
			// バグ？「チェックボックスがある場合、一度スタイルをなくす」
			_super::ModifyStyle(TVS_CHECKBOXES, 0, 0);
			_super::ModifyStyle(0, TVS_CHECKBOXES, 0);
		}
		_super::ModifyStyle(0, TVS_NONEVENHEIGHT, 0);
		SetBackColor(GetBkColor());
	}

	CMapT<HTREEITEM, IDrawable::Ptr>	m_map;		///< HTREEITEM と 描画情報の関連
	int									m_lowLine;	///< 背景描画用
	
public:

	/// コンストラクタ
	CDrawingTreeCtrl(void) : m_lowLine(0)
	{
	}

	/**
	 * [追加] アイテム追加
	 *	@param draw 描画情報
	 *	@param hParent 親のハンドル。
	 *	@param hInsertAfter 追加する一つ前のアイテムのハンドル。
	 *	@param hasBg 描画情報が背景をすべて描画する場合、 true にします（チラツキを軽減できます）。
	 *	@retval NULL 失敗
	 *	@retval NULL以外 成功。値はハンドルです。
	 */
	HTREEITEM InsertItem(const IDrawable &draw, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST, bool hasBg = false)
	{
		HTREEITEM h = _super::InsertItem(hasBg ? _T("*") : _T(""), hParent, hInsertAfter);
		if ( h != NULL )
		{
			m_map[h] = draw.Clone();
		}
		return h;
	}

	/**
	 * [追加] アイテム追加.
	 *	@param lpszItem 表示文字列
	 *	@param hParent 親のハンドル。
	 *	@param hInsertAfter 追加する一つ前のアイテムのハンドル。
	 *	@retval NULL 失敗
	 *	@retval NULL以外 成功。値はハンドルです。
	 */
	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST)
	{
		return _super::InsertItem(lpszItem, hParent, hInsertAfter);
	}

	/**
	 * [取得] アイテムの描画情報取得.
	 *	@param item アイテムハンドル.
	 *	@retval NULL エラー.
	 *	@retval NULL以外 描画情報。delete してはいけません。
	 */
	IDrawable* GetItemDrawer(HTREEITEM item)
	{
		return m_map[item];
	}

	/**
	 * [設定] 線色指定
	 *	@param clrNew 線の色
	 *	@return 直前の線の色
	 */
	COLORREF SetLineColor(COLORREF clrNew = CLR_DEFAULT)
	{
		return TreeView_SetLineColor(*this, clrNew);
	}

	/**
	 * [取得] アイテム状態取得
	 *	@param hItem アイテムハンドル
	 *	@return 状態
	 */
	UINT GetState(HTREEITEM hItem) const
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item;
		item.mask = TVIF_HANDLE | TVIF_STATE;
		item.hItem = hItem;
		item.stateMask = TVIS_STATEIMAGEMASK;
		VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));
		return item.state >> 12;
	}

	/**
	 * [設定] アイテム状態設定
	 *	@param hItem アイテムハンドル
	 *	@param state 状態
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetState(HTREEITEM hItem, UINT state)
	{
		ASSERT(::IsWindow(m_hWnd));
		TVITEM item;
		item.mask = TVIF_HANDLE | TVIF_STATE;
		item.hItem = hItem;
		item.stateMask = TVIS_STATEIMAGEMASK;
		item.state = INDEXTOSTATEIMAGEMASK(state);
		return !! ::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
	}
};


	
}; // MFC
}; // TNB
