#pragma once
/**
 *@file
 * 描画情報コントロール抽象クラス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbSelectedBeltDrawer.h"
#include "TnbBitmapDrawer.h"
#include "TnbSimpleVector.h"
#include "TnbHandleHandle.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報コントロール抽象クラス
 *
 *		選択マークを表示するコントロールの作成をサポートする機能を持っています。\n
 *		また、背景とテキスト描画機能もサポートします。
 *
 *	@par必要ファイル
 *			TnbMfcAbstractDrawingCtrl.h
 * 
 *	@date 07/12/07	新規作成。
 *	@date 08/08/25	クラス名変更。サブWnd管理機能追加。
 *	@date 10/02/26	使用する描画メソッドを IDrawable::Draw() から IDrawable::DrawEx() に変更。 GetTextDrawStyle() 追加。
 *	@date 11/08/04	SetDisabledStyleDrawer() , DrawDisabledStyle() 追加。
 */
class CAbstractDrawingCtrl
{
public:
	/// セレクトマークタイプ
	enum ESelectMarkType
	{
		BACKGROUND,			///< 背景に描画
		OVERLAP,			///< アイテム描画上に描画
	};


	//---------------------


	/**
	 * [設定] デフォルトのマーク描画指定.
	 *		フォーカスマーク、選択マークをWindow標準と同じ様にします。
	 *	@param base ベースの色。省略すると Windows標準と同じ様にします。
	 */
	void SetDefaultMarkDrawer(COLORREF base = ::GetSysColor(COLOR_WINDOW))
	{
		m_pBackgroundDrawer = new CBoxFillDrawer(base);
		m_pSelectMarkDrawer = new CSelectedBeltDrawer(::GetSysColor(COLOR_HIGHLIGHT), base, true);
		m_pNoActiveSelectMarkDrawer = new CSelectedBeltDrawer(::GetSysColor(COLOR_BTNFACE), base, true);
		m_pFocusMarkDrawer = new CFocusFrameDrawer();
		{
			int c1 = ::GetSysColor(COLOR_WINDOW);
			int c2 = ::GetSysColor(COLOR_BTNFACE);
			int r1 = GetRValue(c1);
			int r2 = GetRValue(c2);
			int r = r2 - (r1 - r2);
			r = (r < 0) ? 0 : r;	
			int g1 = GetGValue(c1);
			int g2 = GetGValue(c2);
			int g = g2 - (g1 - g2);
			g = (g < 0) ? 0 : g;	
			int b1 = GetBValue(c1);
			int b2 = GetBValue(c2);
			int b = b2 - (b1 - b2);
			b = (b < 0) ? 0 : b;	
			m_pDisabledStyleDrawer = new CSemitransparentBoxFillDrawer(RGB(r, g, b), 50);
		}
		m_selectMarkType = OVERLAP;
		m_pTextDrawer.Null();
		SetBackColor(base);
	}

	/**
	 * [設定] 背景描画指定.
	 *	@param draw Resize() が有効な各アイテムの背景描画情報。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetBackgroundDrawer(const IDrawable& draw)
	{
		return m_SetDrawer(m_pBackgroundDrawer, draw);
	}

	/**
	 * [設定] 選択マーク描画指定.
	 *	@note アクティブ時と非アクティブ時で同じ選択マークを使います。
	 *	@param draw Resize() が有効な選択マークの描画情報。
	 *	@param type 選択マークのタイプ。省略すると、 OVERLAP になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw, ESelectMarkType type = OVERLAP)
	{
		m_selectMarkType = type;
		bool r = m_SetDrawer(m_pSelectMarkDrawer, draw);
		if ( r )
		{
			m_pNoActiveSelectMarkDrawer = m_pSelectMarkDrawer;
		}
		return r;
	}
	
	/**
	 * [設定] 選択マーク描画指定.
	 *	@param draw1 Resize() が有効なアクティブ時の選択マークの描画情報。
	 *	@param draw2 Resize() が有効な非アクティブ時の選択マークの描画情報。
	 *	@param type 選択マークのタイプ。省略すると、 OVERLAP になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw1, const IDrawable& draw2, ESelectMarkType type = OVERLAP)
	{
		if ( ! SetSelectMarkDrawer(draw1, type) ) { return false; }
		return m_SetDrawer(m_pNoActiveSelectMarkDrawer, draw2);
	}
	
	/**
	 * [設定] フォーカスマーク描画指定.
	 *	@param draw Resize() が有効なフォーカスマークの描画情報。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetFocusMarkDrawer(const IDrawable& draw)
	{
		return m_SetDrawer(m_pFocusMarkDrawer, draw);
	}

	/**
	 * [設定] 無効状態描画指定.
	 *	@note 全て描画後にこの描画情報が使用されます。
	 *	@param draw Resize() が有効な無効状態描画情報。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetDisabledStyleDrawer(const IDrawable& draw)
	{
		return m_SetDrawer(m_pDisabledStyleDrawer, draw);
	}

	/**
	 * [設定] 背景色設定.
	 *	@param color 色
	 */
	void SetBackColor(COLORREF color)
	{
		m_backColor = color;
		if ( color == CLR_INVALID )
		{
			m_backColor = ::GetSysColor(COLOR_WINDOW);
		}
		m_backColorBrush.Null();
	}

	/**
	 * [設定] テキスト描画情報指定
	 *	@note 指定しない場合でも、標準相当の描画が行われます。
	 *	@param text テキスト描画
	 */
	void SetTextDrawer(const CTextDrawer& text)
	{
		m_pTextDrawer = static_cast<CTextDrawer*>(text.Clone());
	}

	/**
	 * [追加] 関連Windowハンドル追加
	 *	@note コントロールと関連するWindowハンドルを登録しておくことで、表示非表示をサポートできます。
	 *	@param hWnd ウィンドウハンドル
	 */
	void AdditionRelatedHwnd(HWND hWnd)
	{
		m_wndList.Add(hWnd);
	}

	/**
	 * [処理] 関連Windowハンドルへポスト
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 */
	void PostMessageToRelatedHwnd(UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		loop ( i, m_wndList.GetSize() )
		{
			::PostMessage(m_wndList[i], message, wParam, lParam);
		}
	}

protected:

	/// コンストラクタ
	CAbstractDrawingCtrl(void) : m_selectMarkType(OVERLAP), m_backColor(::GetSysColor(COLOR_WINDOW))
	{
	}

	/// デストラクタ
	virtual ~CAbstractDrawingCtrl(void)
	{
	}

	/**
	 * [取得] セレクトマークタイプ
	 *	@return タイプ
	 */
	ESelectMarkType	GetSelectMarkType(void) const
	{
		return m_selectMarkType;
	}

	/**
	 * [取得] 背景色取得.
	 *	@return 色
	 */
	COLORREF GetBackColor(void) const
	{
		return m_backColor;
	}

	/**
	 * [描画] 選択マーク表示
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 表示位置。
	 *	@param isActive true ならアクティブ、 false ならノンアクティブの選択マークを書きます。
	 *	@param lParam IDrawable::DrawEx() の LPARAM の値
	 */
	void DrawSelectMark(HDC dc, const RECT& rect, bool isActive, LPARAM lParam = 0)
	{
		if ( isActive || m_pNoActiveSelectMarkDrawer.IsNull() )
		{
			m_DrawDrawer(m_pSelectMarkDrawer, dc, rect, lParam);
		}
		else
		{
			m_DrawDrawer(m_pNoActiveSelectMarkDrawer, dc, rect, lParam);
		}
	}

	/**
	 * [描画] 背景表示
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 表示位置。
	 *	@param lParam IDrawable::DrawEx() の LPARAM の値
	 */
	void DrawBackground(HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		m_DrawDrawer(m_pBackgroundDrawer, dc, rect, lParam);
	}

	/**
	 * [描画] フォーカスマーク表示
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 表示位置。
	 *	@param lParam IDrawable::DrawEx() の LPARAM の値
	 */
	void DrawFocusMark(HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		m_DrawDrawer(m_pFocusMarkDrawer, dc, rect, lParam);
	}

	/**
	 * [描画] 無効状態描画.
	 *	最後に使用されます。
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 表示位置。
	 *	@param lParam IDrawable::DrawEx() の LPARAM の値
	 */
	void DrawDisabledStyle(HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		m_DrawDrawer(m_pDisabledStyleDrawer, dc, rect, lParam);
	}

	/**
	 * [描画] 背景色描画
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 表示位置。
	 */
	void DrawBackColor(HDC dc, const RECT& rect)
	{
		if ( (rect.bottom - rect.top) > 0 )
		{
			if ( m_backColorBrush.IsNull() )
			{
				m_backColorBrush = ::CreateSolidBrush(m_backColor);
			}
			::FillRect(dc, &rect, m_backColorBrush);
		}
	}

	/**
	 * [描画] 文字列描画
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 表示位置。
	 *	@param lpsz 文字列
	 *	@param lParam IDrawable::DrawEx() の LPARAM の値
	 */
	void DrawText(HDC dc, const RECT& rect, LPCTSTR lpsz, LPARAM lParam = 0)
	{
		if ( ! m_pTextDrawer.IsNull() )
		{
			CSize sz = CRect(rect).Size();
			m_pTextDrawer->Resize(sz - CSize(2, 0));
			m_pTextDrawer->SetString(lpsz);
			m_pTextDrawer->DrawEx(dc, rect.left + 1, rect.top, lParam);
		}
	}

	/**
	 * [描画] 文字列描画
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 表示位置。
	 *	@param lpsz 文字列
	 *	@param drawStyle DrawTextスタイル指定。
	 *	@param lParam IDrawable::DrawEx() の LPARAM の値
	 */
	void DrawText(HDC dc, const RECT& rect, LPCTSTR lpsz, DWORD drawStyle, LPARAM lParam = 0)
	{
		if ( ! m_pTextDrawer.IsNull() )
		{
			CSize sz = CRect(rect).Size();
			m_pTextDrawer->SetDrawStyle(drawStyle);
			m_pTextDrawer->Resize(sz - CSize(2, 0));
			m_pTextDrawer->SetString(lpsz);
			m_pTextDrawer->DrawEx(dc, rect.left + 1, rect.top, lParam);
		}
	}

	/**
	 * [取得] 文字列描画スタイル取得.
	 *	@return 文字列描画スタイル
	 */
	DWORD GetTextDrawStyle(void) const
	{
		if ( ! m_pTextDrawer.IsNull() )
		{
			return m_pTextDrawer->GetDrawStyle();
		}
		return 0;
	}

	/**
	 * [設定] テキスト描画情報設定.
	 *	@note 設定されていない場合デフォルトの描画情報を設定します。
	 *	@param pWnd ウィンドウ
	 */
	void SetDefaultTextDrawer(CWnd* pWnd)
	{
		if ( m_pTextDrawer.IsNull() )
		{
			CFont* pFont = pWnd->GetFont();
			m_pTextDrawer = new CTextDrawer(*pFont, CLR_AUTOSELECT, _T(""), DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		}
	}

	/**
	 * [処理] 描画開始.
	 *	@note 一連の描画の最初にコールします。
	 */
	void BeginItemPaint(void) 
	{
		loop ( i, m_wndList.GetSize() )
		{
			HWND h = m_wndList[i];
			if ( ::IsWindow(h) )
			{
				MFCLIB::SetProcessingFlag(h, false);
			}
		}
	}

	/**
	 * [処理] 描画終了.
	 *	@note 一連の描画の最後にコールします。
	 *	@retval true 変化あり。
	 *	@retval false 変化無し。
	 */
	bool EndItemPaint(void) 
	{
		bool res = false;
		for ( INT_PTR i = m_wndList.GetSize() - 1; i >= 0; i-- )
		{
			HWND h = m_wndList[i];
			if ( ::IsWindow(h) )
			{
				BOOL r = MFCLIB::IsProcessingFlag(h);
				if ( r )
				{
					MFCLIB::SetProcessingFlag(h, false);
				}
				if ( ::IsWindowVisible(h) != r )
				{
					::ShowWindow(h, r ? SW_SHOW : SW_HIDE );
					res = true;
				}
			}
			else
			{
				m_wndList.Remove(i);
			}
		}
		return res;
	}

	/**
	 * [設定] 全設定リセット.
	 *		Set系メソッドで設定した内容を全部破棄します。
	 */
	void AllReset(void)
	{
		m_selectMarkType = OVERLAP;
		m_pSelectMarkDrawer.Null();
		m_pNoActiveSelectMarkDrawer.Null();
		m_pFocusMarkDrawer.Null();
		m_pBackgroundDrawer.Null();
		m_pDisabledStyleDrawer.Null();
		//
		m_backColorBrush.Null();
		m_pTextDrawer.Null();
		m_backColor = ::GetSysColor(COLOR_WINDOW);
		//
		m_wndList.RemoveAll();
	}

private:
	/// Drawable登録
	bool m_SetDrawer(IDrawable::Ptr& _P, const IDrawable& draw)
	{
		_P = draw.Clone();
		if ( ! _P.IsNull() )
		{
			if ( ! _P->Resize(CSize(1, 1)) )
			{
				_P.Null();
			}
		}
		return ! _P.IsNull();
	}
	// 描画
	void m_DrawDrawer(IDrawable::Ptr& _pDrawer, HDC dc, const RECT& rect, LPARAM lParam = 0)
	{
		if ( ! _pDrawer.IsNull() )
		{
			const CRect& rc = rect;
			if ( ! rc.IsRectEmpty() )
			{
				_pDrawer->Resize(rc.Size());
				_pDrawer->DrawEx(dc, rc.left, rc.top, lParam);
			}
		}
	}
	ESelectMarkType		m_selectMarkType;				///< セレクトマークタイプ
	IDrawable::Ptr		m_pSelectMarkDrawer;			///< セレクトマーク描画情報(サイズ変更サポート必須)
	IDrawable::Ptr		m_pNoActiveSelectMarkDrawer;	///< ノンアクティブセレクトマーク描画情報(サイズ変更サポート必須)
	IDrawable::Ptr		m_pFocusMarkDrawer;				///< フォーカスマーク描画情報(サイズ変更サポート必須)
	IDrawable::Ptr		m_pBackgroundDrawer;			///< 各Itemの背景(サイズ変更サポート必須)
	IDrawable::Ptr		m_pDisabledStyleDrawer;			///< 無効状態
	//
	CBrushHandle					m_backColorBrush;	///< 背景ブラシ
	CPointerHandleT<CTextDrawer>	m_pTextDrawer;		///< テキスト描画
	COLORREF						m_backColor;		///< 背景色。
	//
	CSimpleVectorT<HWND>			m_wndList;			///< WndList
};



}; // MFC
}; // TNB
