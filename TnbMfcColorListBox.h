#pragma once
/**
 *@file
 * カラー表示機能付ListBox関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcListBoxCp.h"
#include "TnbPointerHandle.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * カラーListBoxコントロール
 *
 *		行単位で色を指定出来る ListBoxコントロールです。。
 *
 *		Ctrl+A , Ctrl+C が使える ListBoxコントロールです。
 *
 *	@note 選択行は、文字色と背景色が反転し表示されます。
 *
 *	@par使い方
 *		リソースエディタでリストボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListBox）を本クラスに置き換えて使用します。\n
 *		リストボックスのプロパティは、「選択(C)」を「拡張」に、「オーナー描画(O)」を「可変」に
 *		文字列ありのチェックを OFF にしておいてください。
 *
 *	@note スクロールすると、親に WM_VSCROLL を SendMessage します。LPARAM には 本コントロールの HWND を渡します。
 *		そのため、 MFC の CWnd::OnVScroll() の第三引数の CScrollBar* には本クラスのインスタンスが渡ります。
 *		CScrollBar* として操作しないように注意してください。
 *
 *	@par必要ファイル
 *			TnbMfcColorListBox.h
 * 
 *	@date 06/11/08	新規作成
 *	@date 07/10/02	背景の指定も可能にした。
 *	@date 08/11/19	SetPostViewMode() , PostString() を新規追加。
 *	@date 09/04/30	PostString時、即描画&スクロールではなく、多少遅延出来るようにした。
 *	@date 09/11/12	構造整理。
 *	@date 11/03/02	挿入中カウンタを廃止。
 *	@date 11/09/13	SetBackColor() 追加。
 */
class CColorListBox : public CListBoxCp
{
	DEFSUPER(CListBoxCp);
public:
	
	/// コンストラクタ
	CColorListBox(void) : _super(), m_itemHeight(-1), m_itemWidth(-1), m_margin(0), m_isView(true)
		, m_currentTextColor(::GetSysColor(COLOR_WINDOWTEXT)), m_currentBackColor(::GetSysColor(COLOR_WINDOW))
	{
	}

	/**
	 * [設定] 最大ライン数設定.
	 *	@note デフォルトは 20000 行です。
	 *	@param maxLine 最大ライン。
	 */
	void SetMaxLine(int maxLine)
	{
		m_lineDatas.SetQueueSize(maxLine + 1);
		_super::SetMaxLine(maxLine);
	}

	/**
	 * [設定] 背景色設定.
	 *		文字列のないところの色を指定します。
	 *	@param color 背景色指定(COLORREF)。省略すると、デフォルト（システム）の色で描画されます。
	 */
	void SetBackColor(COLORREF color = CLR_AUTOSELECT)
	{
		if ( m_backBrush.GetSafeHandle() != NULL )
		{
			m_backBrush.DeleteObject();
		}
		if ( IS_RGBVALUE(color) )
		{
			m_backBrush.CreateSolidBrush(color);
			ASSERT( m_backBrush.GetSafeHandle() != NULL );
		}
	}

	/**
	 * [設定] 文字色設定.
	 *		追加する文字列の色を指定します。
	 *	@param textColor 文字色指定(COLORREF)。
	 *	@param backColor 背景色指定(COLORREF)。
	 */
	void SetCurrentColor(COLORREF textColor, COLORREF backColor)
	{
		m_currentTextColor = textColor;
		m_currentBackColor = backColor;
	}

	/**
	 * [設定] 文字色設定.
	 *		指定行の色を指定します。
	 *	@param nIndex インデックス
	 *	@param textColor 文字色指定(COLORREF)
	 *	@param backColor 背景色指定(COLORREF)
	 */
	void SetColor(int nIndex, COLORREF textColor, COLORREF backColor)
	{
		if ( m_lineDatas.IsInRange(nIndex) )
		{
			TParam& p = m_lineDatas[nIndex];
			p.textColor = textColor;
			p.backColor = backColor;
			RedrawWindow();
		}
	}

	/**
	 * [設定] マージン設定
	 *	@param m 上下のマージン
	 */
	void SetMargin(DWORD m)
	{
		m_margin = m;
		m_itemHeight = -1;
	}

	/**
	 * [設定] PostString表示モード設定.
	 *	@note PostString() 実行時、追加した文字列が表示するようにスクロールさせるか否か、設定できます。
	 *	@param isView true なら追加文字列が表示されるようにスクロールします。 false ならスクロールしません。
	 */
	void SetPostViewMode(bool isView)
	{
		m_isView = isView;
	}
	
	/**
	 * [追加] 文字列追加.
	 *		指定した色で文字列を追加します。
	 *	@note 別スレッドからでも追加が可能です。
	 *	@note 色を指定すると、 SetCurrentColor() で指定したのと同じ効果が残ります。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 *	@param textColor 文字色指定(COLORREF)。省略した場合、最後に設定された文字色になります。
	 *	@param backColor 背景色指定(COLORREF)。省略した場合、最後に設定された背景色になります。
	 */
	void PostString(int nIndex, LPCTSTR lpszItem, COLORREF textColor = 0, COLORREF backColor = 0)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			TParam* P = new TParam(lpszItem, textColor, backColor);
			if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
			{
				delete P;
			}
		}
	}

	/**
	 * [追加] 文字列追加.
	 *		文字列を追加します。色は SetCurrentColor() や PostString() で設定したものになります。
	 *	@note 別スレッドからの実行は、予想外の動作をすることがあります。 PostString() を使用してください。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。挿入されたインデックスを返します。
	 */
	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		return _super::InsertString(nIndex, lpszItem);
	}

	/**
	 * [追加] 文字列追加.
	 *		文字列を最後に追加します。色は SetCurrentColor() や PostString() で設定したものになります。
	 *	@note 別スレッドからの実行は、予想外の動作をすることがあります。 PostString() を使用してください。
	 *	@param lpszItem 挿入する文字列。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。挿入されたインデックスを返します。
	 */
	int AddString(LPCTSTR lpszItem)
	{
		return _super::AddString(lpszItem);
	}

	/**
	 * [通知] オーナードロー通知.
	 *	@param lpDrawItemStruct ドロー要求状態
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		if ( lpDrawItemStruct->CtlType != ODT_LISTBOX || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		UINT nIndex = lpDrawItemStruct->itemID;
		if ( ! m_lineDatas.IsInRange(nIndex) )
		{
			return;
		}
		const TParam& p = m_lineDatas[nIndex];
		CString strText = p.viewItem;
		COLORREF textColor = p.textColor;
		COLORREF backColor = p.backColor;
		if ( (lpDrawItemStruct->itemState & ODS_SELECTED) != 0 )
		{
			Swap(textColor, backColor);
		}
		//
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		//
		COLORREF currenttextColor = dc.SetTextColor(textColor);
		int currentBkMode = dc.SetBkMode(TRANSPARENT);
		//
		dc.FillSolidRect(&(lpDrawItemStruct->rcItem), backColor);
		RECT rect = lpDrawItemStruct->rcItem;
		rect.top += m_margin;
		dc.DrawText(strText, &rect, DT_SINGLELINE | DT_NOPREFIX);
		if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
		{
			dc.DrawFocusRect(&(lpDrawItemStruct->rcItem));	//破線
		}
		dc.SetBkMode(currentBkMode);
		dc.SetTextColor(currenttextColor);
		dc.Detach();
	}

	/**
	 * [通知] オーナードロー計算通知.
	 *	@param lpMeasureItemStruct ドロー要求状態
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		if ( lpMeasureItemStruct->CtlType != ODT_LISTBOX || ToInt(lpMeasureItemStruct->CtlID) != _super::GetDlgCtrlID() )
		{
			return;
		}
		if ( m_itemHeight < 0 )
		{
			LOGFONT lf;
			GetFont()->GetLogFont(&lf);
			m_itemHeight = lf.lfHeight;
			if ( m_itemHeight < 0 )
			{
				m_itemHeight = -m_itemHeight;
			}
			m_itemHeight += m_margin * 2;
		}
		lpMeasureItemStruct->itemHeight = m_itemHeight;
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
		int index = ToInt(wParam);
		LPTSTR lpsz = reinterpret_cast<LPTSTR>(lParam);
		//
		switch ( message )
		{
		case WM_PAINT:
			if ( GetCount() == 0 )
			{
				CPaintDC dc(this);
				return 0;
			}
			break;
		case WM_ERASEBKGND: //背景
			{
				CDC dc;
				dc.Attach(reinterpret_cast<HDC>(wParam));
				CRect rect;
				GetClientRect(rect);
				int y = (GetCount() - GetTopIndex()) * m_itemHeight;
				int h = rect.Height();
				if( h > y )
				{
					if ( y < 0 )
					{
						y = 0;
					}
					CBrush* B = &m_backBrush;
					if ( B->GetSafeHandle() == NULL )
					{
						B = dc.GetCurrentBrush();
					}
					dc.FillRect(CRect(0, y, rect.Width(), h), B);
				}
				dc.Detach();
			}
			return 0;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			RedrawWindow();
			break;
		case LB_GETTEXTLEN: //文字列長取得
			// @param index INDEX 
			// @return 長さ
			if ( m_lineDatas.IsInRange(index) )
			{
				return m_lineDatas[index].item.GetLength();
			}
			return LB_ERR;
		case LB_GETTEXT: //文字列取得
			// @param[in] index INDEX 
			// @param[out] lParam 格納先
			if ( m_lineDatas.IsInRange(index) )
			{
				CStr s = m_lineDatas[index].item;
				STRLIB::Copy(lpsz, s);
				return s.GetLength();
			}
			return LB_ERR;
		case LB_DELETESTRING: //削除
			// @param index INDEX 
			if ( index == 0 )
			{
				m_lineDatas.TakeElements(1);
			}
			else if ( m_lineDatas.IsInRange(index) )
			{
				m_lineDatas.Remove(index);
			}
			break;
		case LB_ADDSTRING: //追加
			// @param lParam 文字列アドレス 
			// @note 実際コントロールに設定するのは "" 
			{
				ASSERTLIB( ToInt(m_lineDatas.GetSize()) == _super::GetCount() );
				TParam p(lpsz, m_currentTextColor, m_currentBackColor);
				m_lineDatas.Add(p);
				m_CheckWidth(p.item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_INSERTSTRING: //挿入
			// @param index INDEX 
			// @param lParam 文字列アドレス 
			// @note 実際コントロールに設定するのは "" 
			if ( index < 0 )
			{
				TParam p(lpsz, m_currentTextColor, m_currentBackColor);
				m_lineDatas.Add(p);
				m_CheckWidth(p.item);
			}
			else if ( IsInRange(index, m_lineDatas.GetSize() + 1) )
			{
				TParam p(lpsz, m_currentTextColor, m_currentBackColor);
				m_lineDatas.Insert(index, p);
				m_CheckWidth(p.item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_RESETCONTENT: //全消し
			m_lineDatas.RemoveAll();
			m_itemWidth = -1;
			_super::SetHorizontalExtent(m_itemWidth);
			break;
		case WM_CLB_POSTSTRING:
			{
				TParam* P = reinterpret_cast<TParam*>(lParam);
				if ( P->textColor != P->backColor )
				{
					m_currentTextColor = P->textColor;
					m_currentBackColor = P->backColor;
				}
				if ( ::IsWindow(m_hWnd) )
				{
					LRESULT r = _super::InsertString(index, P->item);
					if ( m_isView )
					{
						_super::SetTopIndex(ToInt(r));
					}
				}
				delete P;
				return 0;
			}
			break;
		case WM_DESTROY:
			{
				MSG msg;
				while ( ::PeekMessage(&msg, m_hWnd, WM_CLB_POSTSTRING, WM_CLB_POSTSTRING, PM_REMOVE) )
				{
					if ( msg.message == WM_CLB_POSTSTRING )
					{
						TParam* P = reinterpret_cast<TParam*>(msg.lParam);
						delete P;
					}
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
		#ifdef _DEBUG
			DWORD dwStyle = _super::GetStyle();
			ASSERT( dwStyle & LBS_EXTENDEDSEL );
			ASSERT( m_lineDatas.GetSize() == 0 );
			#ifndef _WIN32_WCE
				ASSERT( (dwStyle & LBS_OWNERDRAWVARIABLE) != 0 );
//				ASSERT( (dwStyle & LBS_HASSTRINGS) == 0 );
			#endif
		#endif
		_super::PreSubclassWindow();
		m_lineDatas.SetQueueSize(_super::GetMaxLine() + 1);
	}

	/**
	 * [通知] 破棄終了後通知
	 */
	virtual void PostNcDestroy(void) 
	{
		m_lineDatas.RemoveAll();
		_super::PostNcDestroy();
	}

private:
	/// 一行情報型
	struct TParam
	{
		CStr		item;		///< 文字列
		CStr		viewItem;	///<
		COLORREF	textColor;	///< 文字色
		COLORREF	backColor;	///< 背景色
		TParam(void){}
		TParam(LPCTSTR lpsz, COLORREF ct, COLORREF cb) : item(lpsz), textColor(ct), backColor(cb) 
		{
			viewItem = item;
			const int TABSTOP = 4;
			while ( true )
			{
				INT_PTR tabPos = viewItem.Find('\t');
				if ( tabPos < 0)
				{
					break;
				}
				CStr s = viewItem.Mid(tabPos + 1);
				viewItem = viewItem.Left(tabPos) 
						+ CStr::Lineup(' ', TABSTOP - (tabPos % TABSTOP)) + s;
			}
		}
	};
	int						m_itemHeight;			///< Itemの高さ
	int						m_itemWidth;			///< Itemの幅
	CBrush					m_backBrush;			///< アイテムのないところのカラー
	COLORREF				m_currentTextColor;		///< 次追加するカラー
	COLORREF				m_currentBackColor;		///< 次追加するカラー
	CRingQueueT<TParam>		m_lineDatas;			///< 表示文字の内容
	DWORD					m_margin;				///< 表示開始位置
	bool					m_isView;				///< 追加時表示するか？
	//
	DWORD					m_lastPostIndex;		///< 最終追加位置

	/**
	 * コントロールの幅の調整
	 */
	void m_CheckWidth(LPCTSTR lpszItem)
	{
		CDC* pDC = GetDC();
		CFont* pFont = _super::GetFont();
		CFont* pFontOld = pDC->SelectObject(pFont);
		const CSize& size = pDC->GetTextExtent(lpszItem);
		if ( m_itemWidth < size.cx )
		{
			m_itemWidth = size.cx;
			_super::SetHorizontalExtent(m_itemWidth);
		}
		pDC->SelectObject(pFontOld);
		ReleaseDC(pDC);
	}
};



}; // MFC
}; // TNB
