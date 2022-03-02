#pragma once
/**
 *@file
 * 軽いListBox関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcListBoxCp.h"
#include "TnbMfcDelayedRedraw.h"
#include "TnbPointerHandle.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * LightListBoxコントロール
 *
 *		多くの行を高速にもてる、軽い ListBoxコントロールです。
 *
 *		Ctrl+A , Ctrl+C が使える ListBoxコントロールです。
 *
 *	@par使い方
 *		リソースエディタでリストボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListBox）を本クラスに置き換えて使用します。\n
 *		リストボックスのプロパティは、「オーナー描画(O)」を「固定」に
 *		文字列ありのチェックを OFF にしておいてください。
 *		更に LBS_NODATA のスタイルをつけて置いてください（CE の場合は、これをつけないでください）。
 *
 *	@attention LBS_NODATA は VC6 の場合、IDE からはこのスタイルをつけることは出来ません。
 *				直接 rc ファイルを開き編集してください）。
 *
 *	@note スクロールすると、親に WM_VSCROLL を SendMessage します。LPARAM には 本コントロールの HWND を渡します。
 *		そのため、 MFC の CWnd::OnVScroll() の第三引数の CScrollBar* には本クラスのインスタンスが渡ります。
 *		CScrollBar* として操作しないように注意してください。
 *
 *	@par必要ファイル
 *			TnbMfcLightListBox.h
 * 
 *	@date 10/11/01	新規作成。
 *	@date 10/11/30	CE 用カスタマイズ。背景描画処理修正。
 *	@date 11/03/08	情報をリングキューに変更。
 *	@date 11/03/16	キューの管理を調整。
 *	@date 11/06/17	CEのDelete処理を修正。
 *	@date 12/11/05	デフォルトの行数を 10万行に低減。
 *	@date 14/01/10	GetText() を追加。「LBS_NODATA をつけると LBS_HASSTRING スタイルをつけられないので LB_GETTEXT が4byte しか得られない」 対策。
 */
class CLightListBox : public CDelayedRedrawAddinT<CListBoxCp>
{
	DEFSUPER(CDelayedRedrawAddinT<CListBoxCp>);
public:

	/// データ管理型
	typedef CRingQueueT<CArrayPtrHandleT<TCHAR> > CDatas;
	
	/// コンストラクタ
	CLightListBox(void) : _super(), m_itemHeight(-1), m_margin(0)
	{
		m_clientSize.cx = -1;
		SetMaxLine(10 * 10000);
	}

	/**
	 * [設定] 最大ライン数設定.
	 *	@note デフォルトは 10万行です。
	 *	@param maxLine 最大ライン。 -1 なら、最大ラインのチェックをしません(推奨しません)。
	 */
	void SetMaxLine(int maxLine)
	{
		_super::SetMaxLine(maxLine);
		m_lineDatas.SetQueueSize(maxLine);
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
	 * [追加] 文字列追加.
	 *		指定した色で文字列を追加します。
	 *	@note 別スレッドからでも追加が可能です。
	 *	@param nIndex ダミーです。常に文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 */
	void PostString(int nIndex, LPCTSTR lpszItem)
	{
		LPTSTR P = CloneString(lpszItem);
		if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
		{
			delete P;
		}
	}

	/**
	 * [追加] 文字列追加.
	 *		文字列を追加します。
	 *	@note 別スレッドからの実行は、予想外の動作をすることがあります。 PostString() を使用してください。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。挿入されたインデックスを返します。
	 */
	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		LPTSTR P = CloneString(lpszItem);
		return m_InsertString(nIndex, P);
	}

	/**
	 * [追加] 文字列追加.
	 *		文字列を最後に追加します。
	 *	@note 別スレッドからの実行は、予想外の動作をすることがあります。 PostString() を使用してください。
	 *	@param lpszItem 追加する文字列。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。挿入されたインデックスを返します。
	 */
	int AddString(LPCTSTR lpszItem)
	{
		return InsertString(-1, lpszItem);
	}

	/**
	 * [取得] 文字列取得.
	 *	@param[in] nIndex 取得する文字列を 0 から始まるインデックスで指定します。
	 *	@param[out] lpszBuffer 文字列を格納するアドレスを指定します。 
	 *	@retval LB_ERR エラー。
	 *	@retval 上記以外 成功。数値は、取得した文字数。
	 */
	virtual int GetText(int nIndex, LPTSTR lpszBuffer) const
	{
		if ( m_lineDatas.IsInRange(nIndex) )
		{
			STRLIB::Copy(lpszBuffer, m_lineDatas[nIndex]);
			return STRLIB::GetLen(m_lineDatas[nIndex]);
		}
		return LB_ERR;
	}

	/**
	 * [取得] 文字列取得.
	 *	@param[in] nIndex 取得する文字列を 0 から始まるインデックスで指定します。
	 *	@param[out] rString 文字列を格納されます。
	 */
	virtual void GetText(int nIndex, CString& rString) const
	{
		if ( m_lineDatas.IsInRange(nIndex) )
		{
			rString = m_lineDatas[nIndex];
		}
	}

	/**
	 * [設定] 全文字列設定.
	 *	@param datas 全データ
	 *	@param index 表示ライン。省略すると最終行を表示します。
	 */
	void SetAllString(const CDatas& datas, int index = -1)
	{
		int l = ToInt(datas.GetQueueSize());
		if ( l != GetMaxLine() )
		{
			_super::SetMaxLine(l);
		}
		m_lineDatas = datas;
		m_Settle(ToInt((index < 0) ? (datas.GetSize() - 1) : index));
	}

	/**
	 * [取得] 全文字列取得.
	 *	@return 全データ
	 */
	const CDatas& GetAllString(void) const
	{
		return m_lineDatas;
	}

	/**
	 * [作成] 文字列クローン作成.
	 *		指定の文字列を複製します。
	 *	@param lpszText 文字列
	 *	@return 複製した文字列のポインタ。 いらなくなったら delete[] すること。
	 */
	static LPTSTR CloneString(LPCTSTR lpszText)
	{
		size_t len = STRLIB::GetLen(lpszText) + 1;
		LPTSTR P = new TCHAR[len];
		MemCopy(P, lpszText, len);
		return P;
	}

protected:

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
		UINT index = lpDrawItemStruct->itemID;
		if ( ! m_lineDatas.IsInRange(index) )
		{
			return;
		}
		LPTSTR P = m_lineDatas[index];
		HDC dc = lpDrawItemStruct->hDC;
		COLORREF textColor = 0;
		HBRUSH backBrush = NULL;
		if ( (lpDrawItemStruct->itemState & ODS_SELECTED) == 0 )
		{
			textColor = ::GetSysColor(COLOR_WINDOWTEXT);
			backBrush = ::GetSysColorBrush(COLOR_WINDOW);
		}
		else
		{
			textColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			backBrush = ::GetSysColorBrush(COLOR_HIGHLIGHT);
		}
		COLORREF currentTextColor = ::SetTextColor(dc, textColor);
		int currentBkMode = ::SetBkMode(dc, TRANSPARENT);
		::FillRect(dc, &(lpDrawItemStruct->rcItem), backBrush);
		RECT rect = lpDrawItemStruct->rcItem;
		rect.top += m_margin;
		::DrawText(dc, P, -1, &rect, DT_SINGLELINE | DT_NOPREFIX);
		if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
		{
			::SetTextColor(dc, 0);
			::DrawFocusRect(dc, &(lpDrawItemStruct->rcItem));	//破線
		}
		::SetBkMode(dc, currentBkMode);
		::SetTextColor(dc, currentTextColor);
	}

	/**
	 * [通知] オーナードロー計算通知.
	 *	@param lpMeasureItemStruct ドロー要求状態
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
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
		int index = ToInt(wParam);
		switch ( message )
		{
		case WM_SETFONT:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_itemHeight = -1;
				m_Settle();
				return r;
			}
			break;
		case WM_ERASEBKGND: //背景
			{
				if ( m_clientSize.cx < 0 )
				{
					CRect rc;
					_super::GetClientRect(rc);
					m_clientSize.cy = rc.Height();
					m_clientSize.cx = rc.Width();
				}
				HDC dc = reinterpret_cast<HDC>(wParam);
				int y = (_super::GetCount() - _super::GetTopIndex()) * m_itemHeight;
				int h = m_clientSize.cy;
				if( h > y )
				{
					if ( y < 0 )
					{
						y = 0;
					}
					HBRUSH b = static_cast<HBRUSH>(::GetCurrentObject(dc, OBJ_BRUSH));
					::FillRect(dc, CRect(0, y, m_clientSize.cx, h), b);
				}
			}
			return 0;
		case WM_SIZE:
			m_clientSize.cy = HIWORD(lParam);
			m_clientSize.cx = LOWORD(lParam);
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			_super::RedrawWindow();
			break;
		case WM_CLB_POSTSTRING:
			{
				LPTSTR P = reinterpret_cast<LPTSTR>(lParam);
				m_InsertString(index, P);
				return 0;
			}
			break;
		case LB_GETTEXTLEN: //文字列長取得
			// @param index INDEX 
			// @return 長さ
			if ( m_lineDatas.IsInRange(index) )
			{
				return STRLIB::GetLen(m_lineDatas[index]);
			}
			return LB_ERR;
		case LB_GETTEXT: //文字列取得
			// @param[in] index INDEX 
			// @param[out] lParam 格納先
			if ( m_lineDatas.IsInRange(index) )
			{
				STRLIB::Copy(reinterpret_cast<TCHAR*>(lParam), m_lineDatas[index]);
				return STRLIB::GetLen(m_lineDatas[index]);
			}
			return LB_ERR;
		case LB_DELETESTRING:
			if ( lParam != MAGIC_LPARAM )
			{
				if ( m_lineDatas.IsInRange(index) )
				{
					m_lineDatas.Remove(index);
					m_Settle();
				}
				return 0;
			}
			break;
		case LB_RESETCONTENT:
			if ( lParam != MAGIC_LPARAM )
			{
				if ( m_lineDatas.GetSize() > 0 )
				{
					m_lineDatas.RemoveAll();
					m_Settle();
				}
			}
			break;
		case WM_DESTROY:
			{
				UINT wm = WM_CLB_POSTSTRING;
				MSG msg;
				while ( ::PeekMessage(&msg, _super::GetSafeHwnd(), wm, wm, PM_REMOVE) )
				{
					if ( msg.message == wm )
					{
						LPTSTR P = reinterpret_cast<LPTSTR>(msg.lParam);
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
			ASSERT( (dwStyle & LBS_OWNERDRAWFIXED) != 0 );
			ASSERT( (dwStyle & LBS_HASSTRINGS) == 0 );
			ASSERT( (dwStyle & LBS_MULTICOLUMN) == 0 );
			ASSERT( m_lineDatas.GetSize() == 0 );
			#ifndef _WIN32_WCE
				ASSERT( (dwStyle & LBS_NODATA) != 0 );
			#endif
		#endif
		_super::PreSubclassWindow();
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
	enum { 
		MAGIC_LPARAM = 'Tllb'		// CE用
	};
	// 確定
	void m_Settle(int index = -1)
	{
		int l = ToInt(m_lineDatas.GetSize());
		#ifndef _WIN32_WCE
			_super::SendMessage(LB_SETCOUNT, l ,0);
		#else
			if ( l == 0 )
			{
				_super::SendMessage(LB_RESETCONTENT, 0, MAGIC_LPARAM);
			}
			else
			{
				size_t ll = _super::GetCount();
				int d = ll - l;
				if ( d < 0 )
				{
					loop ( i, -d )
					{
						_super::SendMessage(LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("")));
					}
				}
				else if ( d > 0 )
				{
					loop ( i, d )
					{
						_super::SendMessage(LB_DELETESTRING, 0, MAGIC_LPARAM);
					}
				}
			}
			ASSERT( l == _super::GetCount() );
		#endif
		if ( index >= 0 )
		{
			_super::SetTopIndex(index);
		}
		if ( m_itemHeight < 0 )
		{
			LOGFONT lf;
			_super::GetFont()->GetLogFont(&lf);
			m_itemHeight = lf.lfHeight;
			if ( m_itemHeight < 0 )
			{
				m_itemHeight = -m_itemHeight;
			}
			m_itemHeight += m_margin * 2;
			_super::SetItemHeight(0, m_itemHeight);
		}
		_super::DelayedRedraw();
	}
	/// 挿入
	/// lpszItem ; new で確保した文字列か ms_Close() で複製した文字列。
	int m_InsertString(int index, LPTSTR lpszItem)
	{
		CArrayPtrHandleT<TCHAR> p = lpszItem;
		INDEX r = m_lineDatas.Add(p);
		if ( r == INVALID_INDEX )
		{
			int ml = _super::GetMaxLine();
			int ll = (ml > 1000) ? 20 : 5;
			m_lineDatas.TakeElements(ll);
			r = m_lineDatas.Add(p);
		}
		if ( r != INVALID_INDEX )
		{
			m_Settle(ToInt(r));
			return ToInt(r);
		}
		return -1;
	}
	int		m_itemHeight;	///< Itemの高さ
	CDatas	m_lineDatas;	///< 表示文字の内容
	DWORD	m_margin;		///< 表示開始位置
	SIZE	m_clientSize;	///< クライアントサイズ
};



}; //MFC
}; //TNB
