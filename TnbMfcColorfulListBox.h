#pragma once
/**
 *@file
 * カラフル表示機能付ListBox関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerVector.h"
#include "TnbMfcListBoxCp.h"
#include "TnbMfcBitmapDC.h"
#include "TnbDcSelectAssistant.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * カラフルListBoxコントロール
 *
 *		一文字単位で色を指定出来る ListBoxコントロールです。
 *
 *		Ctrl+A , Ctrl+C が使える ListBoxコントロールです。
 *
 *	@note 選択行は、文字色と背景色が反転し表示されます。
 *
 *	@par使い方
 *		リソースエディタでリストボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListBox）を本クラスに置き換えて使用します。\n
 *		リストボックスのプロパティは、「選択(C)」を「拡張」に、「オーナー描画(O)」を「可変」に
 *		文字列ありのチェックをONにしておいてください。
 *	
 *		ヘッダファイル
 *		\code
 *		class CFooDlg : public CDialog
 *		{
 *				;
 *			CColorfulListBox m_listColorful;		//← CListBox を CColorfulListBox に書き換え
 *				;
 *		};
 *		\endcode
 *		
 *		\code
 *		void CFooDlg::Foo(void)
 *		{
 *			CColorfulListBox::CColor cc;
 *			cc.Add(4, 2, RGB(255, 0, 0), RGB(0, 255, 0)); // 4文字目から2文字分の色を指定
 *			cc.Add(8, 2, RGB(255, 255, 0), RGB(0, 255, 0)); // 8文字目から2文字分の色を指定
 *			m_listColorful.SetCurrentColor(cc);	// 色設定
 *			m_listColorful.AddString("1234567890abcdefghijklmn");
 *			m_listColorful.AddString("あいうえおかきくけこ");
 *			m_listColorful.AddString("1234n");
 *			m_listColorful.AddString("1あい!");
 *			cc.RemoveAll();
 *			cc.Add(5, 3, RGB(255, 255, 0), RGB(0, 0, 0)); // 5文字目から3文字分の色を指定
 *			cc.Add(9, 5, RGB(255, 255, 255), RGB(255, 0, 0)); // 9文字目から5文字分の色を指定
 *			m_listColorful.SetCurrentColor(cc);	// 色再設定
 *			m_listColorful.AddString("1234567890abcdefghijklmn");
 *			m_listColorful.AddString("あいうえおかきくけこ");
 *			m_listColorful.AddString("1234n");
 *			m_listColorful.AddString("1あい!");
 *		}
 *		\endcode
 *
 *	@note スクロールすると、親に WM_VSCROLL を SendMessage します。LPARAM には 本コントロールの HWND を渡します。
 *		そのため、 MFC の CWnd::OnVScroll() の第三引数の CScrollBar* には本クラスのインスタンスが渡ります。
 *		CScrollBar* として操作しないように注意してください。
 *
 *	@par必要ファイル
 *			TnbMfcColorfulListBox.h
 * 
 *	@date 09/06/02	新規作成
 */
class CColorfulListBox : public CListBoxCp
{
	DEFSUPER(CListBoxCp);
public:
	
	/**
	 * カラフルListBoxコントロールのカラー情報.
	 *	
	 *		情報として、ベース文字色、ベース背景色を持っています。
	 *		それ以外に、指定範囲のみに文字色、背景色を指定できます。
	 */
	class CColor
	{
	public:
		/// パラメータ
		struct TParam
		{
			size_t		len;		///< 長さ
			COLORREF	textColor;	///< 文字色
			COLORREF	backColor;	///< 背景色
		};
		/**
		 * コンストラクタ
		 *	@note ベース文字色、ベース背景色はシステムカラーと同じになります。
		 */
		CColor(void) : m_textColor(::GetSysColor(COLOR_WINDOWTEXT)), m_backColor(::GetSysColor(COLOR_WINDOW))
		{
		}
		/**
		 * コンストラクタ
		 *	@param textColor ベース文字色
		 *	@param backColor ベース背景色
		 */
		CColor(COLORREF textColor, COLORREF backColor) : m_textColor(textColor), m_backColor(backColor)
		{
		}
		/**
		 * [確認] 空っぽ？
		 *	@retval true 空っぽ.
		 *	@retval false 情報有り
		 */
		bool IsEmpty(void) const
		{
			return m_infos.IsEmpty();
		}
		/**
		 * [消去] 全消去.
		 */
		void RemoveAll(void)
		{
			m_infos.RemoveAll();
			m_params.RemoveAll();
		}
		/**
		 * [追加] 色情報追加.
		 *	@param pos ポジション。一番左(先頭)が０です。
		 *	@param len 長さ.
		 *	@param textColor 文字色。省略するとベースの文字色になります。
		 *	@param backColor 背景色。省略するとベースカ背景色になります。
		 */
		void Add(INDEX pos, size_t len, COLORREF textColor = CLR_INVALID, COLORREF backColor = CLR_INVALID)
		{
			if ( ! IS_RGBVALUE(textColor) )
			{
				textColor = m_textColor;
			}
			if ( ! IS_RGBVALUE(backColor) )
			{
				backColor = m_backColor;
			}
			if ( pos + len >= m_infos.GetSize() )
			{
				TInfo pa(m_textColor, m_backColor);
				loop ( i, (pos + len) - m_infos.GetSize() )
				{
					m_infos.Add(pa);
				}
			}
			TInfo pa(textColor, backColor);
			loop ( i, len )
			{
				m_infos.Set(pos + i, pa);
			}
			m_params.RemoveAll();
		}
		/**
		 * [取得] 情報一覧取得.
		 *	@note 左端から順に同じが何個続いているかと言う情報が得られます。例えば、 {5, 赤}, {3, 青} と言う情報なら、
		 *		赤が５文字、続いて青が３文字と言う情報です。
		 *	@note CColorfulListBox 内で使用します。使うことはほとんどありません。
		 *	@return 情報一覧。
		 */
		const CVectorT<TParam>& GetParam(void) const
		{
			if ( m_params.IsEmpty() )
			{
				if ( m_infos.IsEmpty() )
				{
					TParam pa = { 100000, m_textColor, m_backColor };
					m_params.Add(pa);
				}
				else
				{
					size_t ll = 0;
					loop ( i, m_infos.GetSize() - 1 )
					{
						ll++;
						if ( m_infos[i].textColor != m_infos[i + 1].textColor || m_infos[i].backColor != m_infos[i + 1].backColor )
						{
							//違う
							TParam pa = { ll, m_infos[i].textColor, m_infos[i].backColor };
							m_params.Add(pa);
							ll = 0;
						}
					}
					ll++;
					INDEX i = m_infos.GetSize() - 1;
					TParam pa = { ll, m_infos[i].textColor, m_infos[i].backColor };
					m_params.Add(pa);
					TParam pa2 = { 100000, m_textColor, m_backColor };
					m_params.Add(pa2);
				}
			}
			return m_params;
		}

	private:
		/// パラメータ
		struct TInfo
		{
			COLORREF	textColor;	///< 文字色
			COLORREF	backColor;	///< 背景色
			/// コンストラクタ
			TInfo(COLORREF text = 0, COLORREF back = 0)
				: textColor(text), backColor(back)
			{
			}
		};
		CVectorT<TInfo>				m_infos;
		mutable CVectorT<TParam>	m_params;
		COLORREF					m_textColor;	///< ベースの文字色
		COLORREF					m_backColor;	///< ベースの背景色
	};

	/// コンストラクタ
	CColorfulListBox(void)
		: _super(), m_iItemHeight(-1), m_iItemWidth(-1), m_margin(0), m_isView(true), m_postCount(-1), m_oneCharWidth(-1)
	{
	}

	/// デストラクタ
	~CColorfulListBox(void)
	{
		m_ColorsRemoveAll();
	}

	/**
	 * [設定] 文字色設定.
	 *		追加する文字列の色を指定します。
	 *	@param color 色指定
	 */
	void SetCurrentColor(const CColorfulListBox::CColor& color)
	{
		m_currentColor = color;
	}

	/**
	 * [設定] 文字色設定.
	 *		指定行の色を指定します。
	 *	@param nIndex インデックス
	 *	@param color 色指定
	 */
	void SetColor(INDEX nIndex, const CColorfulListBox::CColor& color)
	{
		if ( m_colors.GetSize() > nIndex )
		{
			m_colors[nIndex]->color = color;
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
		m_iItemHeight = -1;
	}

	/**
	 * [設定] PostString表示モード設定.
	 *	@note PostString() 実行時、追加した文字列が表示するようにスクロールさせるか否か、設定できます。
	 *	@param isView true なら追加文字列が表示されるようにスクロールします。 false ならスクロールしません。
	 *	@param isDelayMode true ならスクロールを遅延させます(高速に量を表示させる場合に有効)。 false ならリアルタイムでスクロールしません。
	 */
	void SetPostViewMode(bool isView, bool isDelayMode = false)
	{
		m_isView = isView;
		m_postCount = isDelayMode ? 0 : -1;
	}

	/**
	 * [追加] 文字列追加.
	 *		指定した色で文字列を追加します。
	 *	@note 別スレッドからでも追加が可能です。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 */
	void PostString(int nIndex, LPCTSTR lpszItem)
	{
		CColor c;
		PostString(nIndex, lpszItem, c);
	}

	/**
	 * [追加] 文字列追加.
	 *		指定した色で文字列を追加します。
	 *	@note 別スレッドからでも追加が可能です。
	 *	@note 色を指定すると、 SetCurrentColor() で指定したのと同じ効果が残ります。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 *	@param color 色指定。
	 */
	void PostString(int nIndex, LPCTSTR lpszItem, const CColorfulListBox::CColor& color)
	{
		if ( ! ::IsWindow(m_hWnd) )
		{ 
			return;
		}
		TParam* P = new TParam;
		P->item = lpszItem;
		P->color = color;
		if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
		{
			delete P;
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
		UINT nIndex = lpDrawItemStruct->itemID;
//		if ( static_cast<UINT>(m_linesString.GetSize()) <= nIndex )
		if ( static_cast<UINT>(m_colors.GetSize()) <= nIndex )
		{
			return;
		}
		CString strText = m_colors[nIndex]->item;
		CRect rcItem = lpDrawItemStruct->rcItem;
		const CColor& colors = m_colors[nIndex]->color;
		const CVectorT<CColor::TParam> vp = colors.GetParam();
		//
		CBitmapImage bi;
		bi.Set(rcItem.Width(), rcItem.Height());
		{
			CRect rect = rcItem;
			rect.OffsetRect(-rect.left, -rect.top);
			CRect rc2 = rect;
			//
			CBitmapDC dc(&bi);
			CDcSelectAssistant dca(dc);
			dca.SelectFont(_super::GetFont());
			dca.SetBkMode(TRANSPARENT);
			rect.top += m_margin;
			COLORREF colorText = 0;
			COLORREF colorBack = 0;
			loop ( i, vp.GetSize() )
			{
				const CColor::TParam& v = vp[i];
				CString s = strText.Left(ToInt(v.len));
				colorText = v.textColor;
				colorBack = v.backColor;
				if ( (lpDrawItemStruct->itemState & ODS_SELECTED) != 0 )
				{
					colorText = v.backColor;
					colorBack = v.textColor;
				}
				if ( s.IsEmpty() )
				{
					rect.right = rect.left;
				}
				else
				{
					dc.DrawText(s, &rect, DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT);
					dc.FillSolidRect(&rect, colorBack);
					dca.SetTextColor(colorText);
					dc.DrawText(s, &rect, DT_SINGLELINE | DT_NOPREFIX);
					rect.left = rect.right;
				}
				rect.right = lpDrawItemStruct->rcItem.right;
				if ( strText.GetLength() > ToInt(v.len) )
				{
					strText = strText.Mid(ToInt(v.len));
				}
				else
				{
					strText.Empty();
				}
			}
			dc.FillSolidRect(&rect, colorBack);
			if ( ! strText.IsEmpty() )
			{
				dca.SetTextColor(colorText);
				dc.DrawText(strText, &rect, DT_SINGLELINE | DT_NOPREFIX);
			}
			//
			if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
			{
				dca.SetTextColor(RGB(255, 255, 255));
				dc.DrawFocusRect(&rc2);	//破線
			}
		}
		bi.Draw(lpDrawItemStruct->hDC, rcItem.left, rcItem.top);
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
		if ( m_iItemHeight < 0 )
		{
			LOGFONT lf;
			GetFont()->GetLogFont(&lf);
			m_iItemHeight = lf.lfHeight;
			if ( m_iItemHeight < 0 )
			{
				m_iItemHeight = -m_iItemHeight;
			}
			m_iItemHeight += m_margin * 2;
		}
		lpMeasureItemStruct->itemHeight = m_iItemHeight;
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
		int iIndex = ToInt(wParam);
		switch ( message )
		{
		case WM_ERASEBKGND: //背景
			{
				CDC dc;
				dc.Attach(reinterpret_cast<HDC>(wParam));
				CRect rect;
				GetClientRect(rect);
				int y = (GetCount() - GetTopIndex()) * m_iItemHeight;
				int h = rect.Height();
				if( h > y )
				{
					if ( y < 0 )
					{
						y = 0;
					}
					CBrush* B = dc.GetCurrentBrush();
					dc.FillRect(CRect(0, y, rect.Width(), h), B);
				}
				dc.Detach();
			}
			return 0;
		case WM_SETFONT:
			m_oneCharWidth = -1;
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			RedrawWindow();
			break;
		case LB_GETTEXTLEN: //文字列長取得
			// @param iIndex INDEX 
			// @return 長さ
			if ( IsInRange(iIndex, m_colors.GetSize()) )
			{
				return m_colors[iIndex]->item.GetLength();
			}
			return 0;
		case LB_GETTEXT: //文字列取得
			// @param[in] iIndex INDEX 
			// @param[out] lParam 格納先
			if ( IsInRange(iIndex, m_colors.GetSize()) )
			{
				STRLIB::Copy(reinterpret_cast<TCHAR*>(lParam), m_colors[iIndex]->item);
			}
			return 0;
		case LB_DELETESTRING: //削除
			// @param iIndex INDEX 
			if ( IsInRange(iIndex, m_colors.GetSize()) )
			{
//				m_linesString.RemoveAt(iIndex);
				delete m_colors[iIndex];
				m_colors.Remove(iIndex);
			}
			break;
		case LB_ADDSTRING: //追加
			// @param lParam 文字列アドレス 
			// @note 実際コントロールに設定するのは "" 
			{
				ASSERTLIB( ToInt(m_colors.GetSize()) == _super::GetCount() );
//				INT_PTR i = m_linesString.Add(reinterpret_cast<LPCTSTR>(lParam));
				TParam* P = new TParam;
				P->item = reinterpret_cast<LPCTSTR>(lParam);
				P->color = m_currentColor;
				m_colors.Add(P);
				m_CheckWidth(P->item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_INSERTSTRING: //挿入
			// @param iIndex INDEX 
			// @param lParam 文字列アドレス 
			// @note 実際コントロールに設定するのは "" 
			if ( iIndex < 0 )
			{
				ASSERTLIB( ToInt(m_colors.GetSize()) == _super::GetCount() );
				TParam* P = new TParam;
				P->item = reinterpret_cast<LPCTSTR>(lParam);
				P->color = m_currentColor;
				m_colors.Add(P);
				m_CheckWidth(P->item);
			}
			else if ( IsInRange(iIndex, m_colors.GetSize() + 1) )
			{
	//			m_linesString.InsertAt(iIndex, reinterpret_cast<LPCTSTR>(lParam));
				TParam* P = new TParam;
				P->item = reinterpret_cast<LPCTSTR>(lParam);
				P->color = m_currentColor;
				m_colors.Insert(iIndex, P);
				m_CheckWidth(P->item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_RESETCONTENT: //全消し
//			m_linesString.RemoveAll();
//			m_colors.RemoveAll();
			m_ColorsRemoveAll();
			m_iItemWidth = -1;
			_super::SetHorizontalExtent(m_iItemWidth);
			break;
		case WM_TIMER:
			if ( wParam == TIMERID_SETTOPINDEX )
			{
				KillTimer(TIMERID_SETTOPINDEX);
				m_postCount = 0;
				_super::SetTopIndex(m_lastPostIndex);
			}
			break;
		case WM_CLB_POSTSTRING:
			{
				TParam* P = reinterpret_cast<TParam*>(lParam);
				if ( ! P->color.IsEmpty() )
				{
					m_currentColor = P->color;
				}
				if ( ::IsWindow(m_hWnd) )
				{
//					CStr s;
//					s.Format("%s      -(%d)", P->item, GetCount());
//					LRESULT r = _super::InsertString(iIndex, s);
					LRESULT r = _super::InsertString(iIndex, P->item);
					KillTimer(TIMERID_SETTOPINDEX);
					if ( m_isView )
					{
						if ( m_postCount < 0 )
						{
							_super::SetTopIndex(ToInt(r));
						}
						else if ( ++m_postCount > 4 )
						{
							m_postCount = 0;
							_super::SetTopIndex(ToInt(r));
						}
						else
						{
							m_lastPostIndex = ToInt(r);
							SetTimer(TIMERID_SETTOPINDEX, 100, NULL);
						}
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
			ASSERT( dwStyle & LBS_OWNERDRAWVARIABLE );
			ASSERT( dwStyle & LBS_EXTENDEDSEL );
			ASSERT( dwStyle & LBS_HASSTRINGS );
			ASSERT( m_colors.GetSize() == 0 );
		#endif
		_super::PreSubclassWindow();
	}
	/**
	 * 破棄
	 */
	virtual void PostNcDestroy(void) 
	{
//		m_linesString.RemoveAll();
//		m_colors.RemoveAll();
		m_ColorsRemoveAll();
		_super::PostNcDestroy();
	}

private:
	enum { TIMERID_SETTOPINDEX = 1 };
	/// PostString 用パラメータ型
	struct TParam
	{
		CString		item;		///< 文字列
		CColor		color;		///< 色
	};
	int						m_iItemHeight;			///< Itemの高さ
	int						m_iItemWidth;			///< Itemの幅
	CColor					m_currentColor;			///< 次追加するカラー
	CVectorT<TParam*>		m_colors;				///< 表示文字のカラー
//	CPointerVectorT<TParam>	m_colors;				///< 表示文字のカラー
//	CPointerVectorT<CColor>	m_colors;				///< 表示文字のカラー
//	CStringArray			m_linesString;			///< 表示文字の内容
	DWORD					m_margin;				///< 表示開始位置
	bool					m_isView;				///< 追加時表示するか？
	int						m_oneCharWidth;
	//
	DWORD					m_lastPostIndex;
	int						m_postCount;

	void m_ColorsRemoveAll(void)
	{
		loop ( i, m_colors )
		{
			delete m_colors[i];
		}
		m_colors.RemoveAll();
	}

	/**
	 * コントロールの幅の調整
	 */
	void m_CheckWidth(const CString& strItem)
	{
		if ( m_oneCharWidth < 0 )
		{
			CDC* pDC = GetDC();
			CFont* pFont = _super::GetFont();
			CFont* pFontOld = pDC->SelectObject(pFont);
			m_oneCharWidth = pDC->GetTextExtent("W").cx;
			pDC->SelectObject(pFontOld);
			ReleaseDC(pDC);
		}
		int l = strItem.GetLength() * m_oneCharWidth;
		if ( m_iItemWidth < l )
		{
			m_iItemWidth = l;
			_super::SetHorizontalExtent(m_iItemWidth);
		}
	}
};



}; // MFC
}; // TNB
