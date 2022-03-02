#pragma once
/**
 *@file
 * ROWヘッダコントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbWindowProcedureHooker.h"
#include "TnbBitmapImage.h"
#include "TnbDrawable.h"
#include "TnbMfcBitmapDC.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ROWヘッダコントロール
 *
 *	@par使い方
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcRowHeaderCtrl.h
 * 
 *	@date 11/10/31	新規作成
 *	@date 14/04/07	WM_SIZE, WM_MOVE に対応。
 *	@date 14/04/25	InsertItem(IDrawable&) を追加。
 *	@date 14/05/23	表示範囲の修正。
 *	@date 14/05/26	HitTest() を追加。 SetPartialView() を新規。
 *	@date 14/10/15	GetHeaderWidth() を新規。
 *	@date 15/10/01	カラム幅変更時にバディに横スクロールバー制限モードなのに出てしまうのを(とりあえず)改善
 */
class CRowHeaderCtrl : public CWnd
{
	DEFSUPER(CWnd);
public:

	/// コンストラクタ
	CRowHeaderCtrl(void) : m_pBuddyCtrl(NULL), m_headerWidth(0), m_offset(2, 0)
		, m_textColor(::GetSysColor(COLOR_WINDOWTEXT)), m_backColor(::GetSysColor(COLOR_BTNFACE))
		, m_hCursor(NULL), m_isWidthVariable(true), m_textStyle(DT_LEFT | DT_VCENTER), m_hasCapture(false)
		, m_isPartialOk(false)
	{
	}

	/**
	 * [設定] テキスト表示スタイル設定.
	 *	@note デフォルトは DT_LEFT | DT_VCENTER です。
	 *	@param ts スタイル。
	 */
	void SetTextStyle(DWORD ts)
	{
		m_textStyle = ts;
	}

	/**
	 * [設定] 半端表示設定.
	 *		スクロールによって全体が見えないアイテムの表示方法を指定します。
	 *	@param isPartialOk true なら欠けていても表示します。 false なら表示しません（デフォルト）。
	 */
	void SetPartialView(bool isPartialOk = false)
	{
		m_isPartialOk = isPartialOk;
	}

	/**
	 * [設定] テキストカラー設定.
	 *	@note 各カラムの描画情報を設定していない場合、設定されているフォントと設定されている文字を表示します。
	 *			その際使われる色です。
	 *	@param color テキストカラー。
	 */
	void SetTextColor(COLORREF color)
	{
		m_textColor = color;
	}
	
	/**
	 * [設定] ヘッダ設定 
	 *	@param pDrawer ヘッダ描画情報。省略すると、独自の描画（下部と右部に色反転線）で描画します。文字は表示しません。
	 */
	void SetHeaderDrawer(const IDrawable* pDrawer = NULL)
	{
		m_phHeaderDrawer.Null();
		if ( pDrawer != NULL )
		{
			m_phHeaderDrawer = pDrawer->Clone();
		}
		Invalidate(FALSE);
	}

	/**
	 * [設定] 背景設定 
	 *	@param color 背景色。アイテムのないところを塗る色を指定します。
	 *	@param pBkDrawer 背景描画情報。省略すると、独自の描画（下部と右部に色反転線）で描画します。
	 *	@param offsetX 背景とアイテムの X のオフセットを指定します。省略すると２です。
	 *	@param offsetY 背景とアイテムの Y のオフセットを指定します。省略すると０です。
	 */
	void SetBackground(COLORREF color, const IDrawable* pBkDrawer = NULL, int offsetX = 2, int offsetY = 0)
	{
		m_backColor = color;
		m_phBkDrawer.Null();
		m_offset.x = offsetX;
		m_offset.y = offsetY;
		if ( pBkDrawer != NULL )
		{
			m_phBkDrawer = pBkDrawer->Clone();
		}
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			_super::Invalidate(FALSE);
		}
	}

	/**
	 * [設定] 相棒リストコントロール設定.
	 *	@param pCtrl リストコントロール。コントロールの位置、大きさが変更されます。
	 *	@param headerWidth Rowヘッダの幅（ピクセル）。
	 *	@param isWidthVariable true なら可変可能に。 false なら固定に。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetBuddy(CListCtrl* pCtrl, size_t headerWidth, bool isWidthVariable = false)
	{
		if ( m_pBuddyCtrl == NULL )
		{
			if ( pCtrl != NULL && ::IsWindow(pCtrl->GetSafeHwnd()) )
			{
				m_myHooker.m_hParent = GetSafeHwnd();
				m_myHooker.Attach(pCtrl->GetSafeHwnd());
				pCtrl->ModifyStyle(WS_BORDER, 0);
				pCtrl->ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);
				SetWindowPos(pCtrl, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				bool isVisible = (GetStyle() & WS_VISIBLE) != 0;
				ShowWindow(isVisible ? SW_SHOW : SW_HIDE);
				SetFont(pCtrl->GetFont());
				//
				m_headerWidth = down_cast<int>(headerWidth);
				m_isWidthVariable = isWidthVariable;
				m_pBuddyCtrl = pCtrl;
				m_params.SetSize(pCtrl->GetItemCount());
				m_RewidthHeader();
				return true;
			}
		}
		return false;
	}

	/**
	 * [設定] ヘッダ幅設定.
	 *	@param headerWidth Rowヘッダの幅（ピクセル）。
	 *	@param isWidthVariable true なら可変可能に。 false なら固定に。
	 */
	void SetHeaderWidth(size_t headerWidth, bool isWidthVariable = false)
	{
		m_headerWidth = down_cast<int>(headerWidth);
		m_isWidthVariable = isWidthVariable;
		m_RewidthHeader();
	}

	/**
	 * [取得] ヘッダ幅取得.
	 *	@return Rowヘッダの幅（ピクセル）。
	 */
	size_t GetHeaderWidth(void) const
	{
		return m_headerWidth;
	}

	/**
	 * [取得] アイテム数取得
	 *	@note 相棒のリストコントロールの GetItemCount() と同じ値になります。
	 *	@return アイテム数
	 */
	size_t GetItemCount(void) const
	{
		if ( m_pBuddyCtrl != NULL )
		{
			ASSERT( ToInt(m_params.GetSize()) == m_pBuddyCtrl->GetItemCount() );
		}
		return m_params.GetSize();
	}

	/**
	 * [設定] アイテム描画情報設定.
	 *	@param item アイテムインデックス.
	 *	@param draw 描画情報。クローンを記憶しますので、破棄してかまいません。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetItemDrawer(int item, const IDrawable& draw)
	{
		if ( m_params.IsInRange(item) )
		{
			m_params[item].phDraw = draw.Clone();
			Invalidate(FALSE);
			return true;
		}
		return false;
	}

	/**
	 * [設定] アイテム文字情報設定.
	 *	@param item アイテムインデックス.
	 *	@param lpszText 文字情報
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetItemText(int item, LPCTSTR lpszText)
	{
		if ( m_params.IsInRange(item) )
		{
			m_params[item].phDraw.Null();
			m_params[item].text = lpszText;
			Invalidate(FALSE);
			return true;
		}
		return false;
	}

	/**
	 * [設定] アイテム挿入.
	 *	@note 相棒のリストコントロールの InsertItem() をアイテム文字列は空でコールし、 lpszItem は rowヘッダの文字列として使用します。
	 *	@param item アイテムインデックス.
	 *	@param lpszItem 文字列
	 *	@retval マイナス 失敗
	 *	@retval 0以上 成功。値は、挿入したアイテムインデックス。
	 */
	int InsertItem(int item, LPCTSTR lpszItem)
	{
		if ( m_pBuddyCtrl != NULL )
		{
			int r = m_pBuddyCtrl->InsertItem(item, _T(""));
			if ( r >= 0 )
			{
				SetItemText(r, lpszItem);
			}
			return r;
		}
		return -1;
	}

	/**
	 * [設定] アイテム挿入.
	 *	@note 相棒のリストコントロールの InsertItem() をアイテム文字列は空でコールし、 lpszItem は rowヘッダの文字列として使用します。
	 *	@param item アイテムインデックス.
	 *	@param draw 描画情報。クローンを記憶しますので、破棄してかまいません。
	 *	@retval マイナス 失敗
	 *	@retval 0以上 成功。値は、挿入したアイテムインデックス。
	 */
	int InsertItem(int item, const IDrawable& draw)
	{
		int r = InsertItem(item, _T(""));
		if ( r >= 0 )
		{
			SetItemDrawer(r, draw);
		}
		return r;
	}

	/**
	 * [取得] ヒットテスト.
	 *		指定の座標のアイテム番号を返します。
	 *	@param pt クライアント座標
	 *	@retval -99 ヘッダ部
	 *	@retval マイナス アイテムなし
	 *	@retval 上記以外 アイテム番号
	 */
	int HitTest(const CPoint& pt) const
	{
		int r = -1;
		if ( m_pBuddyCtrl != NULL )
		{
			CRect rc;
			GetClientRect(rc);
			if ( rc.PtInRect(pt) )
			{
				CHeaderCtrl* pHead = m_pBuddyCtrl->GetHeaderCtrl();
				if ( pHead != NULL )
				{
					pHead->GetWindowRect(rc);
					if ( pt.y >= rc.Height() )
					{
						r = m_pBuddyCtrl->HitTest(CPoint(0, pt.y));
					}
					else
					{
						r = -99; //　ヘッダ部
					}
				}
			}
		}
		return r;
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
		case CMyHooker::WM_RHC_HOOKMESSAGE:
			{
				CMyHooker::TMsg* P = reinterpret_cast<CMyHooker::TMsg*>(lParam);
				if ( P != NULL )
				{
					m_BuddyWindowProc(P->message, P->wParam, P->lParam, P->lResult);
				}
			}
			break;
		case WM_SIZE:
			m_RewidthHeader();
			break;
		case WM_MOVE:
			m_RewidthHeader();
			break;
		case WM_ERASEBKGND:
			return 0;
		case WM_PAINT:
			m_Draw(CPaintDC(this));
			return 0;
		case WM_LBUTTONDOWN:
			if ( m_isWidthVariable )
			{
				CPoint pos(lParam);
				if ( m_IsDragSpace(pos) )
				{
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
					::SetCapture(GetSafeHwnd());
					m_hasCapture = true;
					m_captureStartPos = CPoint(lParam);
					m_captureStartHeaderWidth = m_headerWidth;
				}
			}
			break;
		case WM_LBUTTONUP:
			if ( m_hasCapture )
			{
				::ReleaseCapture();
				m_hasCapture = false;
			}
			break;
		case WM_RBUTTONDOWN:
			if ( m_hasCapture )
			{
				::ReleaseCapture();
				m_hasCapture = false;
				m_headerWidth = m_captureStartHeaderWidth;
				m_RewidthHeader();
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_isWidthVariable )
			{
				if ( m_hCursor == NULL )
				{
					m_hCursor = ::GetCursor();
				}
				CPoint pos(lParam);
				if ( m_hasCapture )
				{
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
					int x = m_captureStartHeaderWidth + pos.x - m_captureStartPos.x;
					CRect rc;
					GetClientRect(rc);
					int l = rc.Width() - 5;
					if ( x > l )
					{
						x = l;
					}
					if ( x < 5 )
					{
						x = 5;
					}
					m_headerWidth = x;
					m_RewidthHeader();
					if ( m_pBuddyCtrl != NULL )
					{
						m_pBuddyCtrl->GetHeaderCtrl()->RedrawWindow();	//カラムの再描画により再カラム幅調整をする
					}
				}
				else if ( m_IsDragSpace(pos) )
				{
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
				}
				else
				{
					::SetCursor(m_hCursor);
				}
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
		m_pBuddyCtrl = NULL;
		m_hCursor = NULL;
		_super::ModifyStyle(0, SS_NOTIFY);
		_super::PreSubclassWindow();
	}

private:
	// ヘッダの幅変更
	void m_RewidthHeader(void)
	{
		if ( m_pBuddyCtrl != NULL )
		{
			CRect rc;
			GetClientRect(rc);
			ClientToScreen(rc);
			GetParent()->ScreenToClient(rc);
			rc.left += m_headerWidth;
			m_pBuddyCtrl->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
		}
	}
	// ドラック空間？
	bool m_IsDragSpace(const POINT& pos) const
	{
		if ( m_pBuddyCtrl != NULL )
		{
			CRect headerRect;
			CHeaderCtrl* pHead = m_pBuddyCtrl->GetHeaderCtrl();
			pHead->GetClientRect(headerRect);
			if ( pos.x >= m_headerWidth - 5 && pos.x <= m_headerWidth )
			{
				if ( pos.y < headerRect.Height() )
				{
					return true;
				}
			}
		}
		return false;
	}
			
	// 背景描画
	void m_BkDraw(HDC hdc, const CRect& rect)
	{
		if ( m_phBkDrawer != NULL )
		{
			m_phBkDrawer->Resize(rect.Size());
			m_phBkDrawer->Draw(hdc, rect.left, rect.top);
		}
		else
		{
			CRect r = rect;
			r.top = r.bottom - 1;
			::InvertRect(hdc, r);
			r = rect;
			r.left = r.right - 1;
			::InvertRect(hdc, r);
		}
	}
	// 全体描画
	void m_Draw(HDC hdc)
	{
		CRect clientRect;
		GetClientRect(clientRect);
		CBitmapImage bi;
		bi.Set(m_headerWidth, clientRect.Height(), m_backColor);
		if ( m_pBuddyCtrl != NULL )
		{
			CBitmapDC dc(&bi);
			CDcSelectAssistant dsa(dc);
			dsa.SetBkMode(TRANSPARENT);
			dsa.SelectFont(GetFont());
			dsa.SetTextColor(m_textColor);
			{
				// ヘッダ部
				CRect headerRect;
				CHeaderCtrl* pHead = m_pBuddyCtrl->GetHeaderCtrl();
				pHead->GetClientRect(headerRect);
				CRect r = headerRect;
				r.left = 0;
				r.right = m_headerWidth;
				m_BkDraw(dc, r);
				if ( ! m_phHeaderDrawer.IsNull() )
				{				
					m_phHeaderDrawer->Resize(r.Size());
					m_phHeaderDrawer->Draw(dc, r.left, r.top);
				}
			}
			int count = m_pBuddyCtrl->GetItemCount();
			int cpp = m_pBuddyCtrl->GetCountPerPage();
			int si = m_pBuddyCtrl->GetTopIndex();
			loop ( i, cpp + (m_isPartialOk ? 1 : 0) )
			{
				if ( si >= count )
				{
					break;
				}
				CRect rc;
				m_pBuddyCtrl->GetItemRect(si, rc, LVIR_BOUNDS);
				rc.left = 0;
				rc.right = m_headerWidth;
				m_BkDraw(dc, rc);
				rc.left += m_offset.x;
				rc.top += m_offset.y;
				HRGN hRgn = ::CreateRectRgnIndirect(&rc);
				::SelectClipRgn(dc, hRgn);
				if ( m_params.IsInRange(si) )
				{
					TParam& prm = m_params[si];
					if ( prm.phDraw.IsNull() )
					{
						::DrawText(dc, prm.text, -1, rc, m_textStyle);
					}
					else
					{
						prm.phDraw->Resize(rc.Size());
						prm.phDraw->Draw(dc, rc.left, rc.top);
					}
				}
				::SelectClipRgn(dc, NULL);
				_DeleteObject(hRgn);	
				si++;
			}
		}
		bi.Draw(hdc);
	}
	// 相棒のプロシージャ
	void m_BuddyWindowProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT lResult)
	{
		switch ( message )
		{
		case WM_KEYDOWN:
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
		case WM_SIZE:
			{
				CRect rc;
				GetClientRect(rc);
				rc.right = m_headerWidth;
				InvalidateRect(rc, FALSE);
			}
			break;
		case WM_SHOWWINDOW:
			_super::ShowWindow((wParam != FALSE) ? SW_SHOW : SW_HIDE);
			break;
		case LVM_INSERTITEM:
			if ( lResult >= 0 )
			{
				m_params.Insert(lResult, TParam());
			}
			break;
		case LVM_DELETEITEM:
			if ( lResult != 0 )
			{
				m_params.Remove(wParam);
			}
			break;
		case LVM_DELETEALLITEMS:
			if ( lResult != 0 )
			{
				m_params.RemoveAll();
			}
			break;
		}
	}

	/**
	 * 相棒のプロシージャフッカー.
	 */
	class CMyHooker : public CWindowProcedureHooker
	{
		DEFSUPER(CWindowProcedureHooker);
	public:
		enum
		{
			WM_RHC_HOOKMESSAGE = WM_APP,	///< フックメッセージ通知
		};
		/// タグ
		struct TMsg
		{
			LRESULT	lResult;		///< 結果
			UINT	message;		///< メッセージ
			WPARAM	wParam;			///< WPARAM
			LPARAM	lParam;			///< LPARAM
		};
		HWND	m_hParent;			///< フック相手
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
			case WM_SHOWWINDOW:
			case WM_KEYDOWN:
			case WM_VSCROLL:
			case WM_MOUSEWHEEL:
			case WM_SIZE:
			case LVM_INSERTITEM:
			case LVM_DELETEITEM:
			case LVM_DELETEALLITEMS:
				{
					LRESULT r = _super::WindowProc(message, wParam, lParam);
					TMsg msg = { 0 };
					msg.message = message;
					msg.wParam = wParam;
					msg.lParam = lParam;
					msg.lResult = r;
					::SendMessage(m_hParent, WM_RHC_HOOKMESSAGE, NULL, reinterpret_cast<LPARAM>(&msg));
					return r;
				}
				break;
			default:
				break;
			}
			return _super::WindowProc(message, wParam, lParam);
		}
	};

	/// パラメタ
	struct TParam
	{
		CStr			text;		///< 文字列
		IDrawable::Ptr	phDraw;		///< 描画情報
	};

	TParam				m_headParam;				///< ヘッダのパラメタ
	CVectorT<TParam>	m_params;					///< アイテムパラメータ群
	COLORREF			m_textColor;				///< 文字色
	IDrawable::Ptr		m_phHeaderDrawer;			///< ヘッダ描画
	IDrawable::Ptr		m_phBkDrawer;				///< アイテム背景描画
	COLORREF			m_backColor;				///< 背景色
	CMyHooker			m_myHooker;					///< バディプロシージャフッカー
	CListCtrl*			m_pBuddyCtrl;				///< バディコントロール
	int					m_headerWidth;				///< ROW ヘッダの幅
	CPoint				m_offset;					///< アイテム描画オフセット
	DWORD				m_textStyle;				///< 文字表示スタイル
	bool				m_isWidthVariable;			///< 幅は可変？
	HCURSOR				m_hCursor;					///< カーソル
	bool				m_hasCapture;				///< キャプチャー中？
	CPoint				m_captureStartPos;			///< 開始時の座標
	int					m_captureStartHeaderWidth;	///< 開始時の幅
	bool				m_isPartialOk;				///< 半端な表示を許可？
};



}; // MFC
}; // TNB
