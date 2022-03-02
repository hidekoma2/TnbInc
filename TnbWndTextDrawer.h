#pragma once
/**
 *@file
 * WNDテキスト描画関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTextDrawer.h"
#include "TnbPartsDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * WNDテキスト描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたウィンドウハンドルの情報の描画します。
 *
 *	@par必要ファイル
 *			TnbWndTextDrawer.h
 *
 *	@date 07/12/01 新規作成
 *	@date 07/12/27 フォーカス枠は、クライアント領域に書かれるように対処。
 *	@date 08/03/23 コンストラクタを追加。
 *	@date 09/11/05 SetBkMode() 追加。
 */
class CWndTextDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * コンストラクタ
	 *	@param size		サイズ
	 *	@param hWnd		ウィンドウハンドル（文字列とフォントとフォーカスを得ます）
	 *	@param color	カラー
	 *	@param style	DrawTextスタイル
	 *	@param cx		オフセット
	 *	@param cy		オフセット
	 *	@param pParts	フォーカスマーク用( DrawFocusRect() のみを使用してます)。フォーカスマークがいらない場合NULL。
	 */
	CWndTextDrawer(const SIZE& size, HWND hWnd, COLORREF color, DWORD style, int cx = 2, int cy = 2, const IPartsDrawable* pParts = NULL)
		: _super(size), m_style(style), m_color(color), m_hWnd(hWnd), m_cx(cx), m_cy(cy), m_pParts(pParts), m_bkMode(TRANSPARENT)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 *	@param hWnd		ウィンドウハンドル（文字列とフォントとフォーカスを得ます）
	 *	@param color	カラー
	 *	@param style	DrawTextスタイル
	 *	@param cx		オフセット
	 *	@param cy		オフセット
	 *	@param pParts	フォーカスマーク用( DrawFocusRect() のみを使用してます)。フォーカスマークがいらない場合NULL。
	 */
	CWndTextDrawer(HWND hWnd, COLORREF color, DWORD style, int cx = 2, int cy = 2, const IPartsDrawable* pParts = NULL)
		: _super(), m_style(style), m_color(color), m_hWnd(hWnd), m_cx(cx), m_cy(cy), m_pParts(pParts), m_bkMode(TRANSPARENT)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		CWndTextDrawer* P = new CWndTextDrawer(NULL, RGB(0, 0, 0), 0);
		*P = *this;
		return P;
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
		if ( ::IsWindow(m_hWnd) )
		{
			int iLen = ::GetWindowTextLength(m_hWnd);
			if ( iLen > 1 )
			{
				CWorkMemT<TCHAR> str(iLen + 2);
				::GetWindowText(m_hWnd, str, iLen + 1);
				RECT rc = { x + m_cx, y + m_cy, x + m_size.cx - m_cx, y + m_size.cy - m_cy };
				CDcSelectAssistant dca(dc);
//				int oldBkMode = ::SetBkMode(dc, m_bkMode);
				dca.SetBkMode(m_bkMode);
				HFONT font = reinterpret_cast<HFONT>(::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
//				HGDIOBJ oldObj = ::SelectObject(dc, font);
				dca.SelectFont(font);
				CTextDrawer::DrawTextRect(rc, dc, m_style, m_color, str);
				if ( m_pParts != NULL && ::GetFocus() == m_hWnd )
				{
					RECT wr;
					::GetClientRect(m_hWnd, &wr);
					rc.left--;
					rc.right++;
					rc.top--;
					rc.bottom += 2;
					::IntersectRect(&rc, &rc, &wr);
					m_pParts->DrawFocusRect(dc, &rc);
				}
//				::SelectObject(dc, oldObj);
//				::SetBkMode(dc, oldBkMode);
			}
		}
	}

	/**
	 * [設定] 色設定.
	 *	@param color カラー
	 */
	void SetTextColor(COLORREF color)
	{
		m_color = color;
	}

	/**
	 * [設定] スタイル設定.
	 *	@param style	DrawTextスタイル
	 */
	void SetTextStyle(DWORD style)
	{
		m_style = style;
	}

	/**
	 * [設定] 背景モード設定
	 *	@param mode TRANSPARENT か OPAQUE を指定します。
	 */
	void SetBkMode(int mode)
	{
		m_bkMode = mode;
	}

private:
	HWND					m_hWnd;		///< ウィンドウハンドル（文字列とフォントとフォーカスを得ます）
	COLORREF				m_color;	///< カラー
	DWORD					m_style;	///< Drawスタイル
	int						m_cx;		///< オフセット
	int						m_cy;		///< オフセット
	const IPartsDrawable*	m_pParts;	///< フォーカスマーク用。フォーカスマークがいらない場合NULL。
	int						m_bkMode;	///< BKモード
};



}; //TNB


