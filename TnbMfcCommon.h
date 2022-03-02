#pragma once
/**
 *@file
 * MFCコントロール共通のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDynamicFunc.h"
#include "TnbTextDrawer.h"
#include "TnbBitmapImage.h"


#ifndef __AFX_H__
 #error
#endif



//TNB Library
namespace TNB {
namespace MFC {


	
/// MFC共通ライブラリ
namespace MFCLIB 
{

	/** @addtogroup	MFC
	 * @{
	 */

	/**
	 * [変換] StaticスタイルをDrawTextスタイルに変換
	 *	@param staticStyle Staticスタイル
	 *	@return DrawTextスタイル
	 *	@deprecated CTextDrawer::StaticToDrawStyle() を使用してください。
	 */
	inline UINT _deprecated StaticToDrawStyle(DWORD staticStyle)
	{
		return CTextDrawer::StaticToDrawStyle(staticStyle);
	}

	/**
	 * [設定] コントロール位置設定.
	 *		コントロールを、(親ウィンドウの)指定のクライアント座標に移動します。
	 *	@param pWnd コントロール。
	 *	@param x クライアント X座標。-1なら移動なし。
	 *	@param y クライアント Y座標。-1なら移動なし。
	 *	@param boIsRepaint ウィンドウを再描画するかどうかを指定します。
	 *					true を指定すると、ウィンドウに WM_PAINT メッセージが送られます。
	 *					false を指定すると、再描画は行われません。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	inline bool SetControlPos(CWnd* pWnd, int x, int y, bool boIsRepaint = false)
	{
		CWnd* pParent = pWnd->GetParent();
		if ( pParent == NULL ) { return false; }
		CRect rc;
		pWnd->GetWindowRect(&rc);
		pParent->ScreenToClient(&rc);
		int xx = (x < 0) ? rc.left : x;
		int yy = (y < 0) ? rc.top : y;
		pWnd->MoveWindow(xx, yy, rc.Width(), rc.Height(), boIsRepaint);
		return true;
	}

	/**
	 * [取得] コントロール位置取得.
	 *		コントロールの(親ウィンドウの)クライアント座標を取得します。
	 *	@param pWnd コントロール。
	 *	@param[out] _po 座標
	 *	@retval true 成功。 _po に座標が入ります。
	 *	@retval false 失敗。
	 */
	inline bool GetControlPos(CWnd* pWnd, POINT& _po)
	{
		_po.x = _po.y = 0;
		if ( ! ::IsWindow(pWnd->GetSafeHwnd()) ) { return false; }
		CWnd* pParent = pWnd->GetParent();
		if ( pParent == NULL ) { return false; }
		CRect rc;
		pWnd->GetWindowRect(&rc);
		pParent->ScreenToClient(&rc);
		_po = rc.TopLeft();
		return true;
	}

	/**
	 * [設定] クライアントサイズ設定.
	 *		指定のウィンドウのウィンドウサイズを指定のクライアントサイズになるように変更します。
	 *	@param pWnd ウィンドウ。
	 *	@param cx 横の大きさ。-1なら変更なし。
	 *	@param cy 縦の大きさ。-1なら変更なし。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	inline bool ChangeClientSize(CWnd* pWnd, int cx = -1, int cy = -1)
	{
		CRect rcWin;
		CRect rcCli;
		pWnd->GetWindowRect(&rcWin);
		pWnd->GetClientRect(&rcCli);
		int wx = rcWin.Width() - rcCli.Width();
		int wy = rcWin.Height() - rcCli.Height();
		int xx = (cx < 0) ? rcCli.Width() : cx;
		int yy = (cy < 0) ? rcCli.Height() : cy;
		return !! pWnd->SetWindowPos(NULL, 0, 0, wx + xx, wy + yy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	/**
	 * [取得] クライアント位置取得.
	 *		指定のウィンドウの左上からクライアントの左上のすれを返します。
	 *	@param pWnd ウィンドウ。
	 *	@return 位置。
	 */
	inline CPoint GetClientPoint(CWnd* pWnd)
	{
		CRect winRc;
		pWnd->GetWindowRect(&winRc);
		CRect cliRc;
		pWnd->GetClientRect(&cliRc);
		pWnd->ClientToScreen(&cliRc);
		return cliRc.TopLeft() - winRc.TopLeft();
	}

	/**
	 * [表示] 箱表示.
	 *	@param pDC デバイスコンテキスト
	 *	@param rect 表示範囲
	 *	@param pen ペン
	 */
	inline void DrawBox(CDC* pDC, const RECT& rect, CPen& pen)
	{
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right - 1, rect.top);
		pDC->LineTo(rect.right - 1, rect.bottom - 1);
		pDC->LineTo(rect.left, rect.bottom - 1);
		pDC->LineTo(rect.left, rect.top);
		pDC->SelectObject(pOldPen);
	}

	/**
	 * [表示] 箱表示.
	 *	@param pDC デバイスコンテキスト
	 *	@param rect 表示範囲
	 *	@param nPenStyle ペンのスタイル
	 *	@param nWidth ペンの幅
	 *	@param color 文字色
	 */
	inline void DrawBox(CDC* pDC, const RECT& rect, int nPenStyle, int nWidth, COLORREF color)
	{
		CPen pen;
		pen.CreatePen(nPenStyle, nWidth, color);
		DrawBox(pDC, rect, pen);
	}

	/**
	 * [表示] 破線箱表示.
	 *	@param pDC デバイスコンテキスト
	 *	@param rect 表示範囲
	 *	@param color 色
	 */
	inline void DrawDottedBox(CDC* pDC, const RECT& rect, COLORREF color)
	{
		DrawBox(pDC, rect, PS_DOT, 1, color);
	}

	/**
	 * [表示] 親表示.
	 *		指定のコントロールの範囲の親を再描画します。
	 *	@param pWndControl コントロール。
	 *	@param boIsRedraw true なら ::RedrawWindow() , false なら InvalidateRect() を使用します。
	 */
	inline void DrawParent(CWnd* pWndControl, bool boIsRedraw = true)
	{
		if ( pWndControl != NULL )
		{
			CRect rect;
			pWndControl->GetWindowRect(&rect);
			CWnd* pWnd = pWndControl->GetParent();
			if ( pWnd != NULL )
			{
				pWnd->ScreenToClient(&rect);
				if ( boIsRedraw )
				{
					pWnd->RedrawWindow(&rect);
				}
				else
				{
					pWnd->InvalidateRect(&rect);
				}
			}
		}
	}

	/**
	 * [設定] クライアントサイズ調整.
	 *		指定のクライアントのサイズを文字列とDRAWスタイルから調整します。
	 *	@param pWnd デバイスコンテキストのウィンドウ
	 *	@param pDC デバイスコンテキスト
	 *	@param drawStyle Drawスタイル
	 *	@param lpsz 文字列
	 *	@param margin マージン。必要なクライアントサイズに加えるサイズです。
	 *			プラス指定すると、位置を変えずに、幅、高さのみ変更されます。\n
	 *			マイナス指定すると、位置、幅、高さを変更します。\n
	 *			AdjustSize(10) ；位置を変えずに、文字が入る最小の幅、高さに +10したサイズを設定します。\n
	 *			AdjustSize(-5) ；位置を縦横左上に 5 移動し、文字が入る最小の幅、高さに +10したサイズを設定します。
	 */
	inline void AdjustClientSize(CWnd* pWnd, CDC* pDC, UINT drawStyle, LPCTSTR lpsz, int margin = 2)
	{
		CString str = lpsz;
		if ( str.IsEmpty() ) { return; }
		CRect rect;
		pWnd->GetClientRect(&rect);
		CSize sz(rect.Width(), 0);
		if ( CTextDrawer::CalcTextSize(sz, *pDC, drawStyle, str) )
		{
			if ( margin >= 0 )
			{
				ChangeClientSize(pWnd, sz.cx + margin, sz.cy + margin);
			}
			else
			{
				CPoint po;
				ChangeClientSize(pWnd, sz.cx - margin * 2, sz.cy - margin * 2);
				GetControlPos(pWnd, po);
				SetControlPos(pWnd, po.x + margin, po.y + margin);
			}
		}
	}

#ifndef _WIN32_WCE
	/**
	 * [設定] クライアントサイズ調整.
	 *		指定のクライアントのサイズを文字列とDRAWスタイルから調整します。
	 *	@param pDC デバイスコンテキスト
	 *	@param drawStyle Drawスタイル
	 *	@param lpsz 文字列
	 *	@param margin マージン。必要なクライアントサイズに加えるサイズです。
	 *			プラス指定すると、位置を変えずに、幅、高さのみ変更されます。\n
	 *			マイナス指定すると、位置、幅、高さを変更します。\n
	 *			AdjustSize(10) ；位置を変えずに、文字が入る最小の幅、高さに +10したサイズを設定します。\n
	 *			AdjustSize(-5) ；位置を縦横左上に 5 移動し、文字が入る最小の幅、高さに +10したサイズを設定します。
	 */
	inline void AdjustClientSize(CDC* pDC, UINT drawStyle, LPCTSTR lpsz, int margin = 2)
	{
		AdjustClientSize(pDC->GetWindow(), pDC, drawStyle, lpsz, margin);
	}
#endif

	/**
	 * [取得] 親画像取得.
	 *		指定のコントロールの範囲の親の画像を取得します。
	 *	@param pWndControl コントロール。
	 *	@param backColor 背景色。クリッピング範囲外の色を指定します。 
	 *					CLR_AUTOSELECT なら、 WM_ERASEBKGND を使い、先に背景を描画します。
	 *					省略すると、背景を描画処理しません。
	 *	@return 画像
	 */
	inline CBitmapHandle GetParentImage(CWnd* pWndControl, COLORREF backColor = CLR_INVALID)
	{
		CBitmapHandle b;
		if ( pWndControl != NULL )
		{
			CWnd* pWnd = pWndControl->GetParent();
			if ( pWnd != NULL )
			{
				CRect rc;
				pWndControl->GetClientRect(rc);
				pWndControl->ClientToScreen(rc);
				pWnd->ScreenToClient(rc);
				CBitmapImage bi;
				bi.SetFromClient(*pWnd, rc, backColor);
				b = bi.GetBitmapHandle();
			}
		}
		return b;
	}

	/**
	 * [設定] STATIC 文字列設定＆サイズ調整.
	 *		指定の文字列をコントロールへ設定し、STATICのスタイルを考慮しコントロールのサイズを調整します。
	 *	@param pWnd STATICコントロール.
	 *	@param lpsz 文字列
	 *	@param margin マージン。必要なクライアントサイズに加えるサイズです。
	 *			プラス指定すると、位置を変えずに、幅、高さのみ変更されます。\n
	 *			マイナス指定すると、位置、幅、高さを変更します。\n
	 *			AdjustSize(10) ；位置を変えずに、文字が入る最小の幅、高さに +10したサイズを設定します。\n
	 *			AdjustSize(-5) ；位置を縦横左上に 5 移動し、文字が入る最小の幅、高さに +10したサイズを設定します。
	 */
	inline void SetTextForStatic(CWnd* pWnd, LPCTSTR lpsz, int margin = 2)
	{
		CString str = lpsz;
		pWnd->SetWindowText(str);
		if ( str.IsEmpty() ) { return; }
		UINT drawStyle = CTextDrawer::StaticToDrawStyle(pWnd->GetStyle());
		CDC* pDC = pWnd->GetDC();
		CGdiObject* pOld = pDC->SelectObject(pWnd->GetFont());
		AdjustClientSize(pWnd, pDC, drawStyle, lpsz, margin);
		pDC->SelectObject(pOld);
		pWnd->ReleaseDC(pDC);
	}

	/**
	 * [設定] リージョン設定.
	 *		WindowOrg の設定に考慮した SelectClipRgnです。
	 *	@param pDC デバイスコンテキスト
	 *	@param rgn リージョン
	 *	@param mode 実行する操作を指定します。次の値のいずれかになります。
	 *		<table>
	 *		<tr><td>値</td>	<td>説明</td>
	 *		</tr>
	 *		<tr><td>RGN_AND</td>	
	 *						<td>新しいクリップ領域は、現在のクリップ領域と pRgn で識別される
	 *						領域の重なり合った領域を結合します。</td>
	 *		</tr>
	 *		<tr><td>RGN_COPY</td>
	 *						<td>新しいクリップ領域は、pRgn で識別される領域のコピーです。
	 *						pRgn で識別される領域が NULL のときは、新しいクリップ領域は
	 *						既定のクリップ領域 (NULL 領域) になります。</td>
	 *		</tr>
	 *		<tr><td>RGN_DIFF</td>
	 *						<td>新しいクリップ領域は、pRgn で識別される領域から現在の
	 *						クリップ領域を除外した領域を結合します。</td>
	 *		</tr>
	 *		<tr><td>RGN_OR</td>
	 *						<td>新しいクリップ領域は、現在のクリップ領域と pRgn で
	 *						識別される領域を結合します。</td>
	 *		</tr>
	 *		<tr><td>RGN_XOR</td>
	 *						<td>新しいクリップ領域は、現在のクリップ領域と pRgn で
	 *						識別される領域を結合し、重なり合った領域を除外します。</td>
	 *		</tr>
	 *		</table>
	 *	@return 戻り値は領域の型を返します。
	 *	@retval COMPLEXREGION	新しいクリップ領域には、重なり合った境界線があります。
	 *	@retval ERROR			デバイス コンテキストまたは領域が無効です。
	 *	@retval NULLREGION		新しいクリップ領域は空です。
	 *	@retval SIMPLEREGION	新しいクリップ領域には重なり合う境界線がありません。
	 */
	inline int SelectClipRgnOrg(CDC* pDC, HRGN rgn, int mode = RGN_COPY)
	{
		int r = ERROR;
		CPoint offset = pDC->GetWindowOrg();
		if ( offset.x == 0 && offset.y == 0 )
		{
			r = ::ExtSelectClipRgn(*pDC, rgn, mode);
		}
		else
		{
			HRGN tmp= ::CreateRectRgn(0, 0, 0, 0);
			::CombineRgn(tmp, rgn, NULL, RGN_COPY);
			::OffsetRgn(tmp, -offset.x, -offset.y);
			r = ::ExtSelectClipRgn(*pDC, tmp, mode);
			_DeleteObject(tmp);
		}
		return r;
	}

	/**
	 * アクションフラグ値.
	 *		SetLyeredWindow() で使用します。
	 */
	enum ELayeredAction
	{
		E_ColorKey			= _BIT(0),	///< 透明色として crKey を使います。
		E_Alpha				= _BIT(1),	///< bAlpha を使って、レイヤードウィンドウの不透明度を決定します。
		E_SetLayeredStyle	= _BIT(31),	///< WS_EX_LAYERED スタイルを付加します。
	};

	/**
	 * [設定] SetLayeredWindow.
	 *		API の SetLayeredWindowAttributes を使用します。
	 *	@param pWnd 対象のハンドルを指定します。 WS_EX_LAYERED 指定がない場合、付加します。
	 *	@param crKey レイヤードウィンドウの作成時に使う透明のカラーキーを指定します。
	 *				ウィンドウによって描画されるこの色のピクセルはすべて透明になります。
	 *	@param bAlpha レイヤードウィンドウの不透明度を示すアルファ値を指定します。
	 *				0 を指定すると、ウィンドウは完全に透明になります。255 を指定すると、ウィンドウは不透明になります。
	 *	@param dwFlags アクションフラグを指定します。 ELayeredAction の値を組み合わせて使います。 
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	inline bool SetLayeredWindow(CWnd* pWnd, COLORREF crKey = 0, BYTE bAlpha = 255, DWORD dwFlags = E_Alpha | E_SetLayeredStyle)
	{
		bool r = false;
		if ( (dwFlags & E_SetLayeredStyle) != 0 )
		{
			dwFlags &= ~E_SetLayeredStyle;
			pWnd->ModifyStyleEx(0, WS_EX_LAYERED);
		}
		try
		{
			typedef BOOL (__stdcall *PSETLAYEREDATTR)(HWND, COLORREF, BYTE, DWORD); 
			r = !! CDynamicFuncT<PSETLAYEREDATTR>("user32.dll", "SetLayeredWindowAttributes")()
										(pWnd->m_hWnd, crKey, bAlpha, dwFlags);
		}
		catch (CNullPointerException& e)
		{
			e.OnCatch();
			r = false;
		}
		return r;
	}

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
		enum 
		{
			_TnbProcessingFlag  = WS_SYSMENU,
			_TnbWndLong		= GWL_STYLE
		};
		
	#endif

	/**
	 * [設定] 処理済フラグ操作.
	 *		HWND の処理済フラグを設定します。
	 *	@note CPositionAdjuster や CDrawing* というコントロールで使用されます。
	 *	@param hWnd ウィンドウハンドル
	 *	@param isEnable 処理済フラグ
	 */
	inline void SetProcessingFlag(HWND hWnd, bool isEnable)
	{
		LONG l = ::GetWindowLong(hWnd, _TnbWndLong);
		if ( isEnable )
		{
			l |= _TnbProcessingFlag;
		}
		else
		{
			l &= ~_TnbProcessingFlag;
		}
		::SetWindowLong(hWnd, _TnbWndLong, l);
		ASSERT( l == ::GetWindowLong(hWnd, _TnbWndLong) );
	}

	/**
	 * [取得] 処理済フラグ取得.
	 *		HWND の処理済フラグを取得します。
	 *	@note CPositionAdjuster や CDrawing* というコントロールで使用されます。
	 *	@param hWnd ウィンドウハンドル
	 *	@retval true 処理済
	 *	@retval false 未処理
	 */
	inline bool IsProcessingFlag(HWND hWnd)
	{
		LONG l = ::GetWindowLong(hWnd, _TnbWndLong);
		return (l & _TnbProcessingFlag) != 0;
	}

	/**
	 * [処理] メッセージポンプ
	 */
	inline void PumpMessage(void)
	{
		MSG msg;
		while ( ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		{
			AfxGetThread()->PumpMessage();
		}
	}

	/**
	 * [処理] WM_COMMAND送信.
	 *		コントロールが親画面に WM_COMMAND を投げる処理を行います。
	 *	@param pCtrl コントロール
	 *	@param cmd 通知するコード
	 *	@return 結果
	 */
	inline LRESULT SendCommandMessage(CWnd* pCtrl, UINT cmd)
	{
		CWnd* pParent = pCtrl->GetParent();
		if ( pParent != NULL && ::IsWindow(pParent->GetSafeHwnd()) )
		{
			WPARAM wp = MAKEWPARAM(pCtrl->GetDlgCtrlID(), cmd);
			LPARAM lp = reinterpret_cast<LPARAM>(pCtrl->GetSafeHwnd());
			return pParent->SendMessage(WM_COMMAND, wp, lp);
		}
		return 0;
	}

	/**
	 * @}
	 */
}; //MFCLIB



/**@ingroup MFC
 * コントロール一括処理クラス
 *
 *		継承して使用します。 OnFound() に処理を処理を書きます。
 *		ダイアログのハンドルを渡すと、それ以下のコントロールをすべて検索し、
 *		コールバックします。
 * 
 *	@par必要ファイル
 *			TnbMfcCommon.h
 * 
 *	@date 07/11/08 新規作成
 */
class CFindDescendants
{
public:
	/**
	 * [通知] 発見.
	 *	@param hWnd 発見したウィンドウ.
	 *	@retval true 継続。
	 *	@retval false 検索中止.
	 */
	virtual bool OnFound(HWND hWnd) = 0;

	/**
	 * [実行] 検索開始.
	 *	@param hWnd 検索する親ウィンドウ.
	 *	@param bDeep 検索するレベルを指定します。true の場合はすべての子ウィンドウを検索します。
	 *					false の場合は直系の子ウィンドウのみを検索します。
	 *	@retval true 全検索終了.
	 *	@retval false 検索中止された.
	 */
	bool Execute(HWND hWnd, bool bDeep = false)
	{
		for (HWND hChild = ::GetTopWindow(hWnd); hChild != NULL; hChild = ::GetNextWindow(hChild, GW_HWNDNEXT))
		{
			if ( ! OnFound(hChild) )
			{
				return false;
			}
			if (bDeep && ::GetTopWindow(hChild) != NULL)
			{
				// send to child windows after parent
				if ( ! Execute(hChild, bDeep) )
				{
					return false;
				}
			}
		}
		return true;
	}
};



/**@ingroup MFC
 * テキスト描画ベースクラス
 *
 *		テキストを描画するコントロールに本クラスを継承してあります。
 *		ユーザが直接使うことはありません。
 *
 *	@par必要ファイル
 *			TnbMfcCommon.h
 * 
 *	@date 07/10/24 新規作成
 */
class CDrawTextBase
{
protected:
	/// カラーセット型
	struct TColor
	{
		COLORREF	forward;		///< 前の文字の色
		COLORREF	back;			///< 後ろの文字の色
		/// コンストラクタ
		TColor(void) : forward(::GetSysColor(COLOR_WINDOWTEXT)), back(CLR_INVALID) { }
		/// コンストラクタ @param c 前の文字の色
		TColor(COLORREF c) : forward(c), back(CLR_INVALID) { }
	};
	TColor			m_normalColors;		///< 標準状態色
	TColor			m_disableColors;	///< 無効状態色
	CPoint			m_offset;			///< 文字と文字のずれ

	/**
	 * [表示] 文字表示.
	 *		指定のカラーで表示します。文字と文字のずれは、 SetTextDrawOffset() で設定したものを使います。
	 *	@param pDC デバイスコンテキスト
	 *	@param rect 表示範囲
	 *	@param drawStyle Drawスタイル
	 *	@param c カラー情報
	 *	@param str 表示文字
	 */
	virtual void DrawText(CDC* pDC, const RECT& rect, UINT drawStyle, const TColor& c, LPCTSTR str)
	{
		if ( (drawStyle & DT_VCENTER) != 0 )
		{
			drawStyle &= ~DT_VCENTER;
			int h = rect.bottom - rect.top;
			CSize sz(rect.right - rect.left, h);
			CTextDrawer::CalcTextSize(sz, *pDC, drawStyle, str);
			CRect rc = rect;
			rc.top += (h - sz.cy) / 2;
			CTextDrawer::DrawTextRect(*pDC, rc, drawStyle, m_offset, c.forward, c.back, str);
			return;
		}
		CTextDrawer::DrawTextRect(*pDC, rect, drawStyle, m_offset, c.forward, c.back, str);
	}

	/**
	 * [表示] 文字表示.
	 *		指定のカラーで表示します。文字と文字のずれは、 SetTextDrawOffset() で設定したものを使います。
	 *	@param pDC デバイスコンテキスト
	 *	@param rect 表示範囲
	 *	@param drawStyle Drawスタイル
	 *	@param boIsEnable true なら SetTextColor() で指定した色、 false なら SetTextDisableColor() で指定した色で表示。
	 *	@param str 表示文字
	 */
	void DrawTextEx(CDC* pDC, const RECT& rect, UINT drawStyle, bool boIsEnable, LPCTSTR str)
	{
		DrawText(pDC, rect, drawStyle, (boIsEnable ? m_normalColors : m_disableColors), str);
	}

public:

	/// コンストラクタ
	CDrawTextBase(void) : m_normalColors(), m_disableColors(CLR_INVALID), m_offset(0, 0)
	{
	}

	/**
	 * [設定] テキストオフセット設定.
	 *		本クラスは二つのテキストをずらして表示することで、奥行き、立体感を出すことが出来ます。
	 *		前と後ろテキストのずれを指定します。
	 *	@note 標準では 0,0 です。
	 *	@param x 横方向のずれ
	 *	@param y 縦方向のずれ
	 */
	void SetTextDeepOffset(int x, int y)
	{
		m_offset.x = x;
		m_offset.y = y;
	}

	/**
	 * [設定] テキスト色指定.
	 *	@param color1 前の文字の色を指定します。 CLR_INVALID を指定すると、 堀のあるグレーの文字になります。
	 *	@param color2 後ろの文字の色を指定します。省略可能。
	 */
	void SetTextColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_normalColors.forward = color1;
		m_normalColors.back = color2;
	}

	/**
	 * [設定] 無効状態テキスト色指定.
	 *	@param color1 前の文字の色を指定します。 CLR_INVALID を指定すると、 堀のあるグレーの文字になります。
	 *	@param color2 後ろの文字の色を指定します。。省略可能。
	 */
	void SetTextDisableColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_disableColors.forward = color1;
		m_disableColors.back = color2;
	}
};



}; //MFC
}; //TNB


