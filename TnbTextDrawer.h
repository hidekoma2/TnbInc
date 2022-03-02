#pragma once
/**
 *@file
 * テキスト描画関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawable.h"
#include "TnbDcSelectAssistant.h"
#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #ifndef BS_TYPEMASK
  #define BS_TYPEMASK 0x0000000F
 #endif
 #ifndef SS_TYPEMASK
  #define SS_TYPEMASK 0x0000001FL
 #endif
#endif



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * テキスト描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたテキストを描画します。
 *
 *	@note テキスト表示用のサービス関数もあります。
 *
 *	@par必要ファイル
 *			TnbTextDrawer.h
 *
 *	@date 07/11/15 新規作成
 *	@date 07/12/13 VCENTER , BOTTOM の時の動作をマルチラインでも期待通りに出るようにした。
 *	@date 08/01/11 左マージンを追加
 *	@date 08/03/23 コンストラクタを追加。
 *	@date 08/02/22 Disable文字表示方法変更。
 *	@date 10/02/26 ゲッターメソッド追加。
 *	@date 10/03/02 ModifyText() メソッド、追加。
 *	@date 15/04/23 コンストラクト時サイズをアジャストするように変更。
 */
class CTextDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * コンストラクタ
	 *	@param size	サイズ
	 *	@param font	フォント
	 *	@param textColor カラー
	 *	@param str 文字列
	 *	@param drawStyle DrawTextスタイル指定。	省略すると 「DT_CENTER | DT_VCENTER」
	 *	@param margin 左のマージ。省略すると０になります。
	 */
	CTextDrawer(const SIZE& size, HFONT font, COLORREF textColor, LPCTSTR str, DWORD drawStyle = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(size), m_font(font), m_textColor(textColor), m_str(str), m_drawStyle(drawStyle), m_margin(margin)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 *	@param font	フォント
	 *	@param textColor カラー
	 *	@param str 文字列
	 *	@param drawStyle DrawTextスタイル指定。	省略すると 「DT_CENTER | DT_VCENTER」
	 *	@param margin 左のマージ。省略すると０になります。
	 */
	CTextDrawer(HFONT font, COLORREF textColor, LPCTSTR str, DWORD drawStyle = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(), m_font(font), m_textColor(textColor), m_str(str), m_drawStyle(drawStyle), m_margin(margin)
	{
		AdjustSize(0);
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CTextDrawer(m_size, m_font, m_textColor, m_str, m_drawStyle, m_margin);
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		RECT rc = { x + m_margin, y, x + m_size.cx, y + m_size.cy };
		CDcSelectAssistant dca(dc);
//		int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
		dca.SetBkMode(TRANSPARENT);
		if ( m_font != NULL )
		{
//			HGDIOBJ oldObj = ::SelectObject(dc, m_font);
			dca.SelectFont(m_font);
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
//			::SelectObject(dc, oldObj);
		}
		else
		{
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
		}
//		::SetBkMode(dc, oldBkMode);
	}

	/**
	 * [設定] 文字色設定.
	 *	@param color カラー
	 */
	void SetTextColor(COLORREF color)
	{
		m_textColor = color;
	}

	/**
	 * [取得] 文字色取得.
	 *	@return カラー
	 */
	COLORREF GetTextColor(void) const
	{
		return m_textColor;
	}

	/**
	 * [設定] 文字列設定.
	 *	@note サイズは変化しません。
	 *	@param str 文字列
	 */
	void SetString(LPCTSTR str)
	{
		m_str = str;
	}

	/**
	 * [取得] 文字列取得.
	 *	@return  文字列
	 */
	LPCTSTR GetString(void) const
	{
		return m_str;
	}

	/**
	 * [設定] スタイル設定.
	 *	@note サイズは変化しません。
	 *	@param drawStyle DrawTextスタイル指定。	省略すると 「DT_CENTER | DT_VCENTER」
	*/
	void SetDrawStyle(DWORD drawStyle = DT_CENTER | DT_VCENTER)
	{
		m_drawStyle = drawStyle;
	}

	/**
	 * [取得] スタイル取得.
	 *	@return DrawTextスタイル
	*/
	DWORD GetDrawStyle(void) const
	{
		return m_drawStyle;
	}

	/**
	 * [設定] サイズ調整.
	 *		現在のサイズと文字列とDrawTextスタイルを使って、大きさを調整します。
	 *	@param margin マージン。必要なサイズに加えるサイズです。
	 *	@param hWnd DCを取得するハンドルを指定します。省略可能。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AdjustSize(int margin = 1, HWND hWnd = NULL)
	{
		HDC dc = ::GetDC(hWnd);
		CDcSelectAssistant dca(dc);
//		HGDIOBJ oldObj = ::SelectObject(dc, m_font);
		dca.SelectFont(m_font);
		SIZE s = m_size;
		bool r = CalcTextSize(s, dc, m_drawStyle, m_str);
//		if ( oldObj != NULL )
//		{
//			::SelectObject(dc, oldObj);
//		}
		dca.Restore();
		::ReleaseDC(hWnd, dc);
		if ( r )
		{
			m_size.cx = s.cx + margin + m_margin;
			m_size.cy = s.cy + margin;
		}
		return r;
	}

	
	// -----------------

	
	/**
	 * [変換] StaticスタイルをDrawTextスタイルに変換
	 *	@param staticStyle Staticスタイル
	 *	@return DrawTextスタイル
	 */
	static DWORD StaticToDrawStyle(DWORD staticStyle)
	{
		UINT r = 0;
		switch ( staticStyle & SS_TYPEMASK )
		{
		case SS_CENTER:		r = DT_CENTER;		break;
		case SS_LEFT:		r = DT_LEFT;		break;
		case SS_RIGHT:		r = DT_RIGHT;		break;
		case SS_SIMPLE:		r = DT_SINGLELINE;	break;
		};
		if ( (staticStyle & SS_TYPEMASK) != SS_LEFTNOWORDWRAP )
		{
			r |= DT_WORDBREAK;
		}
		if ( (staticStyle & SS_CENTERIMAGE) != 0 )
		{
			r |= DT_VCENTER | DT_SINGLELINE;
		}
		if ( (staticStyle & SS_NOPREFIX) != 0 )
		{
			r |= DT_NOPREFIX;
		}
		#ifndef _WIN32_WCE
			if ( (staticStyle & SS_WORDELLIPSIS) == SS_WORDELLIPSIS )
			{
				r |= DT_WORD_ELLIPSIS;
			}
			else if ( (staticStyle & SS_ENDELLIPSIS) != 0 )
			{
				r |= DT_END_ELLIPSIS;
			}
			else if ( (staticStyle & SS_PATHELLIPSIS) != 0 )
			{
				r |= DT_PATH_ELLIPSIS;
			}
		#endif
		return r;
	}

	/**
	 * [変換] ButtonスタイルをDrawTextスタイルに変換
	 *	@param buttonStyle Buttonスタイル
	 *	@return DrawTextスタイル
	 */
	static DWORD ButtonToDrawStyle(DWORD buttonStyle)
	{
		DWORD r = 0;
		bool isPushButton = false;
		if ( (buttonStyle & BS_TYPEMASK) <= BS_DEFPUSHBUTTON || (buttonStyle & BS_PUSHLIKE) != 0 )
		{
			isPushButton = true;
		}
		switch ( buttonStyle & 0x0300 )
		{
		case BS_LEFT:	/*0x00000100L*/		r |= DT_LEFT;		break;
		case BS_RIGHT:	/*0x00000200L*/		r |= DT_RIGHT;		break;
		case BS_CENTER:	/*0x00000300L*/		r |= DT_CENTER;		break;
		default:
			r |= (isPushButton ? DT_CENTER : DT_LEFT);
			break;
		}

		switch ( buttonStyle & 0x0C00 )
		{
		case BS_TOP:	/*0x00000400L*/		r |= DT_TOP;		break;
		case BS_BOTTOM:	/*0x00000800L*/		r |= DT_BOTTOM;		break;
		default:
		case BS_VCENTER:/*0x00000C00L*/		r |= DT_VCENTER;	break;
		}
		if ( (buttonStyle & BS_MULTILINE) == 0 )
		{
			r |= DT_SINGLELINE;
		}
		else
		{
			r |= DT_WORDBREAK;
		}
		return r;
	}

	/**
	 * [計算] 文字表示大きさ計算.
	 *	@param[in,out] _size 表示サイズ。スタイルに複数行が含まれている場合、関数コール時、幅は設定しておいてください。
	 *					戻りで bottom が設定されます。
	 *	@param[in] dc デバイスコンテキスト
	 *	@param[in] drawStyle DrawTextスタイル
	 *	@param[in] str 表示文字
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool CalcTextSize(SIZE& _size, HDC dc, UINT drawStyle, LPCTSTR str)
	{
		RECT rc = { 0, 0, _size.cx, _size.cy };
		int r = ::DrawText(dc, str, -1, &rc, (drawStyle & 0xFFFFFFF0) | DT_CALCRECT);
		_size.cx = rc.right;
		_size.cy = rc.bottom;
		return r != 0;
	}

	/**
	 * [計算] 文字表示範囲計算.
	 *	@param[in,out] _rect 表示範囲。戻りで、表示した範囲が設定されます。
	 *	@param[in] dc デバイスコンテキスト
	 *	@param[in] drawStyle DrawTextスタイル
	 *	@param[in] str 表示文字
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool CalcTextRect(RECT& _rect, HDC dc, DWORD drawStyle, LPCTSTR str)
	{
		SIZE sz = { _rect.right - _rect.left, _rect.bottom - _rect.top };
		if ( ! CalcTextSize(sz, dc, drawStyle, str) ) { return false; }
		RECT rc = { 0, 0, sz.cx, sz.cy };
		int cx = (_rect.right - _rect.left) - sz.cx;
		int cy = (_rect.bottom - _rect.top) - sz.cy;
		if ( (drawStyle & DT_CENTER) != 0 )
		{
			cx /= 2;
		}
		else if ( (drawStyle & DT_RIGHT) == 0 )
		{
			cx = 0;
		}
		if ( (drawStyle & DT_VCENTER) != 0 )
		{
			cy /= 2;
		}
		else if ( (drawStyle & DT_BOTTOM) == 0 )
		{
			cy = 0;
		}
		::OffsetRect(&rc, cx + _rect.left, cy + _rect.top);
		_rect = rc;
		return true;
	}

	/**
	 * [表示] 範囲文字表示.
	 *	@param[in,out] _rect 表示範囲。戻りで、表示した範囲が設定されます。
	 *	@param[in] dc デバイスコンテキスト
	 *	@param[in] drawStyle DrawTextスタイル
	 *	@param[in] offset 文字1と文字２のずれ
	 *	@param[in] color1 文字色１を指定します。 CLR_INVALID を指定すると、 堀のあるグレーの文字になります。
	 *					CLR_AUTOSELECT を指定すると、現在のテキストカラーを使います。
	 *	@param[in] color2 文字色２を指定します。
	 *	@param[in] str 表示文字
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool DrawTextRect(RECT& _rect, HDC dc, DWORD drawStyle, const POINT& offset, COLORREF color1, COLORREF color2, LPCTSTR str)
	{
		if ( ! CalcTextRect(_rect, dc, drawStyle, str) ) 
		{
			return false;
		}
		drawStyle &= ~DT_VCENTER;
		RECT rc = _rect;
		if ( color1 == CLR_INVALID )
		{
			#ifndef _WIN32_WCE
				::DrawState(dc, NULL, &ms_DrawStateProc,
					reinterpret_cast<LPARAM>(str),
					static_cast<WPARAM>(drawStyle), 
					rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, DST_COMPLEX | DSS_DISABLED);
				return true;
			#else
				POINT po = { 1, 1 };
				return DrawTextRect(dc, _rect, drawStyle, po, ::GetSysColor(COLOR_GRAYTEXT), ::GetSysColor(COLOR_3DHILIGHT), str);
			#endif
		}
		if ( color1 == CLR_AUTOSELECT )
		{
			color1 = ::GetTextColor(dc);
		}
		if ( color2 == CLR_INVALID )
		{
			COLORREF oldColor = ::SetTextColor(dc, color1);
			::DrawText(dc, str, -1, &rc, drawStyle);
			::SetTextColor(dc, oldColor);
		}
		else
		{
			COLORREF oldColor = ::SetTextColor(dc, color2);
			::OffsetRect(&rc, offset.x, offset.y);
			::DrawText(dc, str, -1, &rc, drawStyle);
			::SetTextColor(dc, color1);
			rc = _rect;
			::DrawText(dc, str, -1, &rc, drawStyle);
			::SetTextColor(dc, oldColor);
		}
		return true;
	}

	/**
	 * [表示] 範囲文字表示.
	 *	@param[in] dc デバイスコンテキスト
	 *	@param[in] rect 表示範囲。
	 *	@param[in] drawStyle DrawTextスタイル
	 *	@param[in] offset 文字1と文字２のずれ
	 *	@param[in] color1 文字色１を指定します。 CLR_INVALID を指定すると、 堀のあるグレーの文字になります。
	 *					CLR_AUTOSELECT を指定すると、現在のテキストカラーを使います。
	 *	@param[in] color2 文字色２を指定します。
	 *	@param[in] str 表示文字
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool DrawTextRect(HDC dc, const RECT& rect, DWORD drawStyle, const POINT& offset, COLORREF color1, COLORREF color2, LPCTSTR str)
	{
		RECT rc = rect;
		return DrawTextRect(rc, dc, drawStyle, offset, color1, color2, str);
	}

	/**
	 * [表示] 範囲文字表示.
	 *	@param[in,out] _rect 表示範囲。戻りで、表示した範囲が設定されます。
	 *	@param[in] dc デバイスコンテキスト
	 *	@param[in] drawStyle DrawTextスタイル
	 *	@param[in] color 文字色１を指定します。 CLR_INVALID を指定すると、 堀のあるグレーの文字になります。
	 *					CLR_AUTOSELECT を指定すると、現在のテキストカラーを使います。
	 *	@param[in] str 表示文字
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool DrawTextRect(RECT& _rect, HDC dc, DWORD drawStyle, COLORREF color, LPCTSTR str)
	{
		POINT po = { 0, 0 };
		return DrawTextRect(_rect, dc, drawStyle, po, color, CLR_INVALID, str);
	}

	/**
	 * [表示] 範囲文字表示.
	 *	@param[in] dc デバイスコンテキスト
	 *	@param[in] rect 表示範囲。
	 *	@param[in] drawStyle DrawTextスタイル
	 *	@param[in] color 文字色１を指定します。 CLR_INVALID を指定すると、 堀のあるグレーの文字になります。
	 *					CLR_AUTOSELECT を指定すると、現在のテキストカラーを使います。
	 *	@param[in] str 表示文字
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool DrawTextRect(HDC dc, const RECT& rect, DWORD drawStyle, COLORREF color, LPCTSTR str)
	{
		POINT po = { 0, 0 };
		RECT rc = rect;
		return DrawTextRect(rc, dc, drawStyle, po, color, CLR_INVALID, str);
	}

	#ifndef _WIN32_WCE

	/**
	 * [計算] 文字列編集.
	 *		表示幅に収まるように、文字列を編集します。
	 *	@param[out] _str 編集結果が格納される。
	 *	@param[in] size 表示サイズ。
	 *	@param[in] dc デバイスコンテキスト。
	 *	@param[in] lpsz 表示文字列。
	 *	@param[in] drawStyle DrawTextスタイル。通常、 DT_END_ELLIPSIS か DT_PATH_ELLIPSIS を指定します。省略すると、 DT_END_ELLIPSIS | DT_NOPREFIX です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool ModifyText(CSimpleStr& _str, const SIZE& size, HDC dc, LPCTSTR lpsz, UINT drawStyle = DT_END_ELLIPSIS | DT_NOPREFIX)
	{
		size_t len = STRLIB::GetLen(lpsz);
		CWorkMemT<TCHAR> work(len + 10);
		STRLIB::Copy(work, lpsz);
		RECT rc = { 0, 0, size.cx, size.cy };
		int r = ::DrawTextEx(dc, work, -1, &rc, (drawStyle & 0xFFFFFFF0) | DT_MODIFYSTRING | DT_CALCRECT, NULL);
		if ( r != 0 )
		{
			_str = work;
		}
		return r != 0;
	}

	/**
	 * [計算] 文字列編集.
	 *		表示幅に収まるように、文字列を編集します。
	 *	@param[out] _str 編集結果が格納される。
	 *	@param[in] hWnd ウィンドウハンドル。ここから、サイズとフォントを得ます。
	 *	@param[in] lpsz 表示文字列。
	 *	@param[in] drawStyle DrawTextスタイル。通常、 DT_END_ELLIPSIS か DT_PATH_ELLIPSIS を指定します。省略すると、 DT_END_ELLIPSIS | DT_NOPREFIX です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool ModifyText(CSimpleStr& _str, HWND hWnd, LPCTSTR lpsz, UINT drawStyle = DT_END_ELLIPSIS | DT_NOPREFIX)
	{
		HDC dc = ::GetDC(hWnd);
		if ( dc != NULL )
		{
//			HGDIOBJ hFont = reinterpret_cast<HGDIOBJ>(::SendMessage(hWnd, WM_GETFONT, 0, 0));
			HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hWnd, WM_GETFONT, 0, 0));
			CDcSelectAssistant dca(dc);
//			HGDIOBJ hOld = ::SelectObject(dc, hFont);
			dca.SelectFont(hFont);
			RECT rc;
			::GetClientRect(hWnd, &rc);
			SIZE size = { rc.right - rc.left, rc.bottom - rc.top };
			bool r = ModifyText(_str, size, dc, lpsz, drawStyle);
//			::SelectObject(dc, hOld);
			dca.Restore();
			::ReleaseDC(hWnd, dc);
			return r;
		}
		return false;
	}

	#endif // _WIN32_WCE

protected:
	HFONT			m_font;			///< フォント
	COLORREF		m_textColor;	///< テキストカラー
	CSimpleStr		m_str;			///< 文字列
	DWORD			m_drawStyle;	///< DrawTextスタイル
	int				m_margin;		///< 左マージン

private:
	/// DrawTextRect 用、コールバック
	static BOOL CALLBACK ms_DrawStateProc(HDC hDC, LPARAM lParam, WPARAM wParam, int cx, int cy)
	{
		LPCTSTR str = reinterpret_cast<LPCTSTR>(lParam);
		DWORD drawStyle = static_cast<DWORD>(wParam);
		RECT rc = { 0, 0, cx, cy };
		::DrawText(hDC, str, -1, &rc, drawStyle);
		return TRUE;
	}
};



/**@ingroup DRAWABLE
 * テキスト描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたテキストを背景色付で描画します。
 *
*	@par必要ファイル
 *			TnbTextDrawer.h
 *
 *	@date 07/12/12 新規作成
 */
class CTextBkDrawer : public CTextDrawer
{
	DEFSUPER(CTextDrawer);
	COLORREF m_backColor;	///< 背景色
public:
	/**
	 * コンストラクタ
	 *	@param size	サイズ
	 *	@param font	フォント
	 *	@param textColor 文字カラー
	 *	@param backColor 背景カラー
	 *	@param str 文字列
	 *	@param style DrawTextスタイル指定。	省略すると 「DT_CENTER | DT_VCENTER」。
	 *	@param margin 左のマージ。省略すると０になります。
	 */
	CTextBkDrawer(const SIZE& size, HFONT font, COLORREF textColor, COLORREF backColor, LPCTSTR str, DWORD style = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(size, font, textColor, str, style, margin), m_backColor(backColor)
	{
	}

	/**
	 * コンストラクタ
	 *	@param font	フォント
	 *	@param textColor 文字カラー
	 *	@param backColor 背景カラー
	 *	@param str 文字列
	 *	@param style DrawTextスタイル指定。	省略すると 「DT_CENTER | DT_VCENTER」。
	 *	@param margin 左のマージ。省略すると０になります。
	 */
	CTextBkDrawer(HFONT font, COLORREF textColor, COLORREF backColor, LPCTSTR str, DWORD style = DT_CENTER | DT_VCENTER, int margin = 0)
		: _super(font, textColor, str, style, margin), m_backColor(backColor)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CTextBkDrawer(m_size, m_font, m_textColor, m_backColor, m_str, m_drawStyle, m_margin);
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		RECT rc = { x + m_margin, y, x + m_size.cx, y + m_size.cy };
		CDcSelectAssistant dca(dc);
//		COLORREF oldBackColor = ::SetBkColor(dc, m_backColor);
		dca.SetBkColor(m_backColor);
		::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
		if ( m_font != NULL )
		{
//			HGDIOBJ oldObj = ::SelectObject(dc, m_font);
			dca.SelectFont(m_font);
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
//			::SelectObject(dc, oldObj);
		}
		else
		{
			DrawTextRect(dc, rc, m_drawStyle, m_textColor, m_str);
		}
//		::SetBkColor(dc, oldBackColor);
	}

	/**
	 * [設定] 背景色設定.
	 *	@param color カラー
	 */
	void SetBackColor(COLORREF color)
	{
		m_backColor = color;
	}
};



}; //TNB


