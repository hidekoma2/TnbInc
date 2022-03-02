#pragma once
/**
 *@file
 * カスタムウィンドウフレームのヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbWindowProcedureHooker.h"
#include "TnbBitmapBankDrawer.h"
#include "TnbBitmapBorderDrawer.h"
#include "TnbTextDrawer.h"
#include "TnbMfcDrawingMenu.h"
#include "TnbHandleHandle.h"
#include "TnbFontHandle.h"
#include "TnbDcSelectAssistant.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #ifndef WM_NCMOUSELEAVE
  #define WM_NCMOUSELEAVE 0x02A2
 #endif
 #ifndef TME_NONCLIENT
  #define TME_NONCLIENT 0x10
 #endif
#endif



/**@ingroup DRAWABLE
 * ウィンドウタイトル描画情報クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *	@see CCustomWindowFrame 内で使用しています。
 *
 *	@par必要ファイル
 *			TnbMfcCustomWindowFrame.h
 *
 *	@date 08/10/06 新規作成
 */
class CWindowCaptionDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * コンストラクタ
	 *	@param hWnd 対象ウィンドウ
	 */
	CWindowCaptionDrawer(HWND hWnd) : m_hWnd(hWnd), m_nWidthEllipse(5), m_nHeightEllipse(5)
		, m_barColor(::GetSysColor(COLOR_ACTIVECAPTION)), m_naBarColor(::GetSysColor(COLOR_INACTIVECAPTION))
		, m_textColor(::GetSysColor(COLOR_CAPTIONTEXT)), m_naTextColor(::GetSysColor(COLOR_INACTIVECAPTIONTEXT))
	{
	}

	/**
	 * [設定] バーの色指定.
	 *	@note 指定しない場合、システムの色が使用されます。
	 *	@param c1 アクティブ時の色
	 *	@param c2 非アクティブ時の色
	 */
	void SetBarColor(COLORREF c1, COLORREF c2)
	{
		m_barColor = c1;
		m_naBarColor = c2;
	}

	/**
	 * [設定] 文字の色指定.
	 *	@note 指定しない場合、システムの色が使用されます。
	 *	@param c1 アクティブ時の色
	 *	@param c2 非アクティブ時の色
	 */
	void SetTextColor(COLORREF c1, COLORREF c2)
	{
		m_textColor = c1;
		m_naTextColor = c2;
	}

	/**
	 * [設定] バーの丸み指定
	 *	@param widthEllipse 楕円の高さ。デフォルトは 5 です。
	 *	@param heightEllipse 楕円の高さ。デフォルトは 5 です。
	 */
	void SetBarEllipse(int widthEllipse = 5, int heightEllipse = 5)
	{
		m_nWidthEllipse = widthEllipse;
		m_nHeightEllipse = heightEllipse;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const 
	{
		return new CWindowCaptionDrawer(*this); 
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@note 本クラスは何もしません。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 *	@param lParam LPARAM。 0ならアクティブ、 1なら非アクティブを描画します。
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		HWND hWnd = ::WindowFromDC(dc);
		if ( hWnd != NULL )
		{
			m_hWnd = hWnd;
		}
		// バー
		{
			COLORREF c1 = (lParam == 0) ? m_barColor : m_naBarColor;
			CBitmapImage bi;
			bi.Set(m_size.cx, m_size.cy, c1);
			CBitmapImage mb;
			mb.Set(m_size.cx, m_size.cy);
			mb.GradationFill(RGB(255, 255, 255), 0, true);
			bi.CustomizeAlpha(mb);
			if ( m_nWidthEllipse > 0 || m_nHeightEllipse > 0 )
			{
				HRGN rgn = ::CreateRoundRectRgn(x, y, x + m_size.cx, y + m_size.cy, m_nWidthEllipse, m_nHeightEllipse);
				::SelectClipRgn(dc, rgn);
				bi.SemitransparectBlt(dc, x, y, -1, CLR_INVALID);
				::SelectClipRgn(dc, NULL);
				_DeleteObject(rgn);
			}
			else
			{
				bi.SemitransparectBlt(dc, x, y, -1, CLR_INVALID);
			}
		}
		// アイコン
		HICON icon = reinterpret_cast<HICON>(::SendMessage(m_hWnd, WM_GETICON, 0, 0));
		if ( icon != NULL )
		{
//			::DrawIconEx(dc, x + 2, (m_size.cy - 16) / 2 + y, icon, CalcScaleX(16, dc), CalcScaleY(16, dc), 0, 0, DI_NORMAL);
			::DrawIconEx(dc, x + 2, y + 1, icon, CalcScaleX(16, dc), CalcScaleY(16, dc), 0, 0, DI_NORMAL);
			x += CalcScaleX(16 + 5, dc);
		}
		else
		{
			x += 2;
		}
		// 文字
		CWorkMemT<TCHAR> w(256);
		::GetWindowText(m_hWnd, w.Ref(), 256);
		if ( m_hFont.IsNull() )
		{
			VERIFY( m_hFont.SetSystemCaptionFont() );
		}
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		COLORREF color = (lParam == 0) ? m_textColor : m_naTextColor;
		//
		CDcSelectAssistant dca(dc);
		dca.SetBkMode(TRANSPARENT);
		dca.SelectFont(m_hFont);
		CTextDrawer::DrawTextRect(dc, rc, DT_LEFT | DT_VCENTER, color, w);
	}

private:
	mutable HWND		m_hWnd;				///< 対象ウィンドウ
	mutable CFontHandle	m_hFont;			///< フォント
	COLORREF			m_barColor;			///< バー色
	COLORREF			m_naBarColor;		///< バー色
	COLORREF			m_textColor;		///< 文字色
	COLORREF			m_naTextColor;		///< 文字色
	int					m_nWidthEllipse;	///< 楕円の高さ
	int					m_nHeightEllipse;	///< 楕円の幅
};



/**@ingroup DRAWABLE MFCCONTROL
 * カスタムウィンドウフレーム.
 *	
 *		ダイアログの NonClient 部分を変更できます。また、透過色を指定し、矩形以外のフレームにも対応します。
 *
 *	@note ウィンドウのフレーム、ボタンの形状、位置を自由にカスタマイズできます。
 *			キャプションバーは上部固定になります。	
 *
 *	@par使用例
 *		\code
 *		class CFooDlg : public CDialog
 *		{
 *				;
 *		    CCustomWindowFrame m_frame;
 *				;
 *		};
 *
 *		BOOL CFooDlg::OnInitDialog()
 *		{
 *			CDialog::OnInitDialog();
 *				;
 *			// フレームの設定
 *			CWindowCaptionDrawer ttd(NULL);
 *			ttd.SetBarColor(RGB(255, 0, 0), RGB(0, 0, 0));
 *			m_frame.SetTitleBitmap(ttd);
 *			m_frame.SetFrameBitmap(IDB_FRAME);
 *			m_frame.SetCloseButtonBitmap(CPoint(-6, 5), IDB_CLOSE, 4);
 *				;
 *			// アタッチする。
 *			m_frame.Attach(*this);
 *				;
 *		\endcode
 *
 *	@todo ReadOnly の EDIT でスクロール時、文字が重なってしまう・・・（背景が再描画されない）。
 *
 *	@todo システムメニュー変化が拾えない。 WM_MENUSELECT が必要。
 *
 *	@par必要ファイル
 *			TnbMfcCustomWindowFrame.h
 *
 *	@date 08/10/06	新規作成
 *	@date 09/09/29	メニューあるなら、タイトルバーの高さは固定に。
 *	@date 10/04/19	SetTextColor 追加。
 *	@date 10/04/26	ポップアップメニューも対象に。
 *	@date 10/06/15	サイズ変更時にも RGN を設定するようにした。
 *	@date 10/07/20	ホバー状態の処理を修正
 *	@date 11/09/01	内部的に NC 描画時、WM_NCPAINT していたが、直接関数コールに変更。
 *	@date 11/10/12	WM_INITDIALOG の時にもシステムメニューのチェックを行うようにした。
 */
class CCustomWindowFrame : public COwnerdrawMenuSupportProcedureHooker
{
	DEFSUPER(COwnerdrawMenuSupportProcedureHooker);
public:

	/// コンストラクタ
	CCustomWindowFrame(void) 
		: m_pressedButton(BK_Nothing), m_transColor(CLR_INVALID), m_hNcRgn(NULL)
		, m_isActive(true), m_boHasHover(false), m_isInit(false), m_isInitWindowRgn(true), m_canMenuCheck(true)
		, m_frameWidths(0, 0, 0, 0), m_titleRect(0, 0, 0, 0), m_titleHeight(0), m_isEvenBackGround(false)
		, m_textColor(CLR_INVALID), m_isMenuInit(false), m_isNcResized(false)
	{
	}

	/// デストラクタ
	~CCustomWindowFrame(void)
	{
		if ( m_hNcRgn != NULL )
		{
			_DeleteObject(m_hNcRgn);
			m_hNcRgn = NULL;
		}
	}

	/**
	 * [設定] フレームビットマップ設定
	 *	@param bmp 画像。この画像は 9分割され、それぞれの隅、枠、中央に使われます。
	 */
	void SetFrameBitmap(CBitmapHandle bmp)
	{
		CBitmapBorderDrawer bbd(bmp);
		m_backGround[0] = ::CreatePatternBrush(bbd.GetParts(5)); //中央
		m_backGround[1] = ::CreatePatternBrush(bbd.GetParts(5)); //中央
		m_isEvenBackGround = true;
		m_pFrameDrawer = bbd.Clone();
	}

	/**
	 * [設定] フレームビットマップ設定
	 *	@param bmp1 アクティブ時の画像。この画像は 9分割され、それぞれの隅、枠、中央に使われます。
	 *	@param bmp2 非アクティブ時の画像。この画像は 9分割され、それぞれの隅、枠、中央に使われます。
	 */
	void SetFrameBitmap(CBitmapHandle bmp1, CBitmapHandle bmp2)
	{
		CBitmapBorderDrawer bbd1(bmp1);
		CBitmapBorderDrawer bbd2(bmp2);
		m_backGround[0] = ::CreatePatternBrush(bbd1.GetParts(5)); //中央
		m_backGround[1] = ::CreatePatternBrush(bbd2.GetParts(5)); //中央
		m_isEvenBackGround = false;
		CBankDrawer db;
		m_pFrameDrawer = db.Add(bbd1).Add(bbd2).Clone();
	}

	/**
	 * [設定] フレームビットマップ設定
	 *	@param draw フレーム描画情報。
	 *	@param bmp 中央のブラシ画像を指定します。
	 */
	void SetFrameBitmap(const IDrawable& draw, CBitmapHandle bmp)
	{
		m_backGround[0] = ::CreatePatternBrush(bmp);
		m_backGround[1] = ::CreatePatternBrush(bmp);
		m_isEvenBackGround = true;
		m_pFrameDrawer = draw.Clone();
	}

	/**
	 * [設定] 透過色指定
	 *	@note フレームビットマップの透過色を指定できます。これにより任意の形のウィンドウが作成できます。
	 *		本メソッドを使わない場合、デフォルトは矩形のウィンドウになります。
	 *	@param color 透過色。 CLR_AUTOSELECT を使うと右下の色を透過色とします。
	 */
	void SetFrameTransColor(COLORREF color = CLR_AUTOSELECT)
	{
		m_transColor = color;
	}

	/**
	 * [設定] タイトルバー描画情報指定.
	 *	@note フレームビットマップでタイトルを書くことも出来ますが、別途子のメソッドで指定することも出来ます。
	 *			指定しない場合、フレーム描画上に CWindowCaptionDrawer() でアイコンとウィンドウテキストが描画されます。
	 *	@param draw 描画情報
	 */
	void SetTitleBitmap(const IDrawable& draw)
	{
		m_pTitleDrawer = draw.Clone();
	}

	/**
	 * [設定] 大きさ設定.
	 *		NonClientの上下左右の幅、および、タイトルバーの高さを指定します。
	 *	@attention メニューがあるウィンドウの場合、本メソッドは正しく機能しません。メニュー無しの場合のみ使用してください。
	 *	@param frameWidths NoClientの上下左右の幅(RECT本来の使い方ではない)
	 *	@param titleHeight タイトルバーの高さ。省略するとシステムの高さが使われます。
	 */
	void SetFrameWidths(const RECT& frameWidths, int titleHeight = -1)
	{
		if ( titleHeight <= 0 )
		{
			titleHeight = ::GetSystemMetrics(SM_CYCAPTION);
		}
		::SetRect(&m_titleRect, frameWidths.left, frameWidths.top, 0, frameWidths.top + titleHeight);
		m_titleHeight = titleHeight;
		m_frameWidths = frameWidths;
		m_frameWidths.top += titleHeight;
	}

	/**
	 * [設定] 大きさ設定.
	 *		NonClientの上下左右の幅、および、タイトルバーの高さを指定します。
	 *	@attention メニューがあるウィンドウの場合、本メソッドは正しく機能しません。メニュー無しの場合のみ使用してください。
	 *	@param frameWidth  NoClientの上下左右のすべての幅。省略するとシステムの幅が使われます。
	 *	@param titleHeight タイトルバーの高さ。省略するとシステムの高さが使われます。
	 */
	void SetFrameWidths(int frameWidth = -1, int titleHeight = -1)
	{
		RECT rc;
		if ( frameWidth >= 0 )
		{
			::SetRect(&rc, frameWidth, frameWidth, frameWidth, frameWidth);
		}
		else
		{
			::SetRectEmpty(&rc);
		}
		SetFrameWidths(rc, titleHeight);
	}

	/**
	 * [設定] Closeボタン設定
	 *	@param pos ボタン位置。座標がマイナスの場合、右端(下端)からのオフセットになります。
	 *	@param bmp ボタン画像。同じ大きさの画像を、Normal,Press,Hover,Disable の順に横に並べた画像を指定します。
	 *	@param num ボタン画像の数。通常省略します（４になります）。
	 *	@param color 透過する色。 CLR_INVALID を指定すると、透過色はなし。
	 */
	void SetCloseButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Close].SetDrawer(pos, bbd, HTCLOSE, SC_CLOSE);
	}

	/**
	 * [設定] Minimumボタン設定
	 *	@param pos ボタン位置。座標がマイナスの場合、右端(下端)からのオフセットになります。
	 *	@param bmp ボタン画像。同じ大きさの画像を、Normal,Press,Hover,Disable の順に横に並べた画像を指定します。
	 *	@param num ボタン画像の数。通常省略します（４になります）。
	 *	@param color 透過する色。 CLR_INVALID を指定すると、透過色はなし。
	 */
	void SetMinimumButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Minimum].SetDrawer(pos, bbd, HTMINBUTTON, SC_MINIMIZE);
	}

	/**
	 * [設定] Maximumボタン設定
	 *	@param pos ボタン位置。座標がマイナスの場合、右端(下端)からのオフセットになります。
	 *	@param bmp ボタン画像。同じ大きさの画像を、Normal,Press,Hover,Disable の順に横に並べた画像を指定します。
	 *	@param num ボタン画像の数。通常省略します（４になります）。
	 *	@param color 透過する色。 CLR_INVALID を指定すると、透過色はなし。
	 */
	void SetMaximumButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Maximum].SetDrawer(pos, bbd, HTMAXBUTTON, SC_MAXIMIZE);
	}

	/**
	 * [設定] Restoreボタン設定
	 *	@param pos ボタン位置。座標がマイナスの場合、右端(下端)からのオフセットになります。通常、Maximumボタンと同じ場所を指定します。
	 *	@param bmp ボタン画像。同じ大きさの画像を、Normal,Press,Hover,Disable の順に横に並べた画像を指定します。
	 *	@param num ボタン画像の数。通常省略します（４になります）。
	 *	@param color 透過する色。 CLR_INVALID を指定すると、透過色はなし。
	 */
	void SetRestoreButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Restore].SetDrawer(pos, bbd, HTMAXBUTTON, SC_RESTORE);
	}

	/**
	 * [設定] Helpボタン設定
	 *	@param pos ボタン位置。座標がマイナスの場合、右端(下端)からのオフセットになります。
	 *	@param bmp ボタン画像。同じ大きさの画像を、Normal,Press,Hover,Disable の順に横に並べた画像を指定します。
	 *	@param num ボタン画像の数。通常省略します（４になります）。
	 *	@param color 透過する色。 CLR_INVALID を指定すると、透過色はなし。
	 */
	void SetHelpButtonBitmap(const POINT& pos, CBitmapHandle bmp, int num = 4, COLORREF color = CLR_INVALID)
	{
		CBitmapBankDrawer bbd(bmp, num, color);
		m_button[BK_Help].SetDrawer(pos, bbd, HTHELP, SC_CONTEXTHELP);
	}

	/**
	 * [設定] テキスト色.
	 *	@param color テキスト色
	 */
	void SetTextColor(COLORREF color)
	{
		if ( color == RGB(255, 255, 255) )
		{
			color = RGB(255, 255, 254);
		}
		m_textColor = color;
		m_menuBar.SetTextColor(color);
	}

	/**
	 * [取得] 背景ブラシ取得
	 *	@param isActive true ならアクティブ、 false なら非アクティブのブラシを返します。
	 *	@return 背景ブラシ
	 */
	HBRUSH GetBackground(bool isActive)
	{
		return m_backGround[isActive ? 0 : 1];
	}

	/**
	 * [設定] 設定の複製.
	 *	@note ボタンや画像の設定をコピーします。
	 *	@param o コピー元。複製するので元は破棄してもかまいません。
	 */
	void CopyParameter(const CCustomWindowFrame& o)
	{
		m_pFrameDrawer = o.m_pFrameDrawer;
		m_backGround[0] = o.m_backGround[0];
		m_backGround[1] = o.m_backGround[1];
		m_isEvenBackGround = o.m_isEvenBackGround;
		m_pTitleDrawer.Null();
		if ( ! o.m_pTitleDrawer.IsNull() )
		{
			m_pTitleDrawer = o.m_pTitleDrawer->Clone();
		}
		m_textColor = o.m_textColor;
		m_frameWidths = o.m_frameWidths;
		m_titleRect = o.m_titleRect;
		m_titleHeight = o.m_titleHeight;
		m_transColor = o.m_transColor;
		loop ( i, BK_Max )
		{
			m_button[i] = o.m_button[i];
		}
		m_menuBar.CopyParameter(o.m_menuBar);
	}

	/**
	 * [参照] メニューバー参照.
	 *		保持しているメニューバーの参照を返します。
	 *	@return 参照.
	 */
	CDrawingMenuBar& ReferMenuBar(void)
	{
		return m_menuBar;
	}

	/**
	 * [参照] メニューバー参照.
	 *		保持しているメニューバーの参照を返します。
	 *	@return 参照.
	 */
	CDrawingMenu& ReferPopupMenu(void)
	{
		return m_popupMenu;
	}

protected:

	/**
	 * [通知] アタッチ直前通知
	 *	@param hWnd アタッチするHWND。
	 */
	virtual void PreAttach(HWND hWnd)
	{
		int fx = ::GetSystemMetrics(SM_CXFIXEDFRAME);
		int fy = ::GetSystemMetrics(SM_CYFIXEDFRAME);
		int ch = ::GetSystemMetrics(SM_CYCAPTION);
		DWORD style = ::GetWindowLong(hWnd, GWL_STYLE);
		if ( (style & WS_THICKFRAME) != 0 )
		{
			fx = ::GetSystemMetrics(SM_CXSIZEFRAME);
			fy = ::GetSystemMetrics(SM_CYSIZEFRAME);
		}
		if ( (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != 0 )
		{
			ch = ::GetSystemMetrics(SM_CYSMCAPTION);
		}
		if ( (style & WS_CAPTION) != WS_CAPTION )
		{
			ch = 0;
		}
		if ( (style & WS_BORDER) == 0 )
		{
			fx = fy = 0;
		}
		if ( m_frameWidths.IsRectNull() )
		{
			m_frameWidths.SetRect(fx, fy + ch, fx, fy);
		}
		if ( m_titleRect.IsRectNull() )
		{
			m_titleRect.SetRect(fx, fy, 0, fy + ch);
			m_titleHeight = ch;
		}
	}
	
	/**
	 * [通知] アタッチ通知.
	 *		アタッチされたときに通知されます。
	 */
	virtual void OnAttached(void) 
	{
		m_CheckMenu();
		//
		if ( ! m_pTitleDrawer.IsNull() )
		{
			HDC dc = ::GetWindowDC(m_hWnd);
			m_pTitleDrawer->DrawEx(dc, 0, 0, 0);
			::ReleaseDC(m_hWnd, dc);
		}
		CRect rc;
		::GetClientRect(m_hWnd, rc);
		m_windowSize = rc.Size() + CSize(m_frameWidths.left + m_frameWidths.right, m_frameWidths.top + m_frameWidths.bottom);
		m_menuHeight = 0;
		MENUBARINFO mbi;
		mbi.cbSize = sizeof(MENUBARINFO);
		if ( ::GetMenuBarInfo(m_hWnd, OBJID_MENU, 0, &mbi) )
		{
			CRect r = mbi.rcBar;
			m_menuHeight = r.Height() + 1;
			m_windowSize.cy += m_menuHeight;
		}	
		::SetWindowPos(m_hWnd, NULL, 0, 0, m_windowSize.cx, m_windowSize.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		//
		m_isInit = true;
	}
	
	/**
	 * [通知] デタッチ直前通知.
	 *		デタッチする直前に通知されます。
	 */
	virtual void PreDetach(void)
	{
		m_menuBar.Detach();
	}

	/**
	 * [通知] デタッチ通知.
	 *		デタッチされたときに通知されます。
	 */
	virtual void OnDetacted(void) 
	{
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

private:
	/// ボタン種
	enum EButtonKind
	{
		BK_Close = 0,	///< Closeボタン。HitTest→ HTCLOSE 。 Message→ SC_CLOSE 。
		BK_Minimum,		///< 最小化ボタン。HitTest→ HTMINBUTTON 。 Message→ SC_MINIMIZE 。
		BK_Maximum,		///< 最大化ボタン。HitTest→ HTMAXBUTTON 。 Message→ SC_MAXIMIZE 。
		BK_Restore,		///< Restoreボタン。HitTest→ HTMAXBUTTON 。 Message→ SC_RESTORE 。
		BK_Help,		///< Helpボタン。HitTest→ HTHELP 。 Message→ SC_CONTEXTHELP 。
		BK_Nothing,		///< なし。
		BK_Max = BK_Nothing,
	};
	/// メニューをチェックし、ボタンの表示、有効設定をおこなう
	void m_CheckMenu(void)
	{
		if ( m_canMenuCheck )
		{
			LONG l = ::GetWindowLong(m_hWnd, GWL_STYLE);
			loop ( i, BK_Max )
			{
				m_button[i].SetVisibleMode(false);
			}
			if ( (l & WS_SYSMENU) != 0 )
			{
				m_button[BK_Close].SetVisibleMode(true);
				HMENU hMenu = ::GetSystemMenu(m_hWnd, FALSE);
				UINT state = ::GetMenuState(hMenu, SC_CLOSE, MF_BYCOMMAND);
				m_button[BK_Close].SetEnableMode((state & (MF_GRAYED | MF_DISABLED)) == 0);
				if ( (l & WS_MINIMIZEBOX) != 0 )
				{
					m_button[BK_Minimum].SetVisibleMode(true);
					m_button[BK_Minimum].SetEnableMode(true);
					if ( (l & WS_MAXIMIZEBOX) == 0 )
					{
						m_button[BK_Maximum].SetVisibleMode(true);
						m_button[BK_Maximum].SetEnableMode(false);
					}
				}
				if ( (l & WS_MAXIMIZEBOX) != 0 )
				{
					int b = ::IsZoomed(m_hWnd) ? BK_Restore : BK_Maximum;
					m_button[b].SetVisibleMode(true);
					if ( (l & WS_MINIMIZEBOX) == 0 )
					{
						m_button[BK_Minimum].SetVisibleMode(true);
						m_button[BK_Minimum].SetEnableMode(false);
					}
				}
			}
			if ( (l & DS_CONTEXTHELP) != 0 )
			{
				m_button[BK_Help].SetVisibleMode(true);
			}
		}
	}
	/**
	 * [描画] フレーム描画
	 *	@param dc DC。
	 *	@param size フレーム描画サイズ。
	 *	@param rect タイトル描画範囲。
	 *	@param isActive true ならアクティブ画像描画
	 */
	void m_OnFramePaint(HDC dc, const CSize& size, const CRect& rect, bool isActive)
	{
		CBitmapImage bi;
		bi.Set(size.cx, size.cy);
		{
			HDC dc = bi.GetDC();
			if ( ! m_pFrameDrawer.IsNull() )
			{
				::SetBrushOrgEx(dc, m_frameWidths.left, m_frameWidths.top, NULL);
				m_pFrameDrawer->Resize(size);
				m_pFrameDrawer->DrawEx(dc, 0, 0, ! isActive);
			}
			if ( ! m_pTitleDrawer.IsNull() )
			{
				m_pTitleDrawer->Resize(rect.Size());
				m_pTitleDrawer->DrawEx(dc, rect.left, rect.top, ! isActive);
			}
			else
			{
				CWindowCaptionDrawer t(m_hWnd);
				t.Resize(rect.Size());
				t.DrawEx(dc, rect.left, rect.top, ! isActive);
			}
			loop ( i, BK_Max )
			{
				m_button[i].Draw(dc, m_windowSize);
			}
			m_menuBar.Draw(dc);
			bi.ReleaseDC();
		}
		bi.Draw(dc, 0, 0);
	}

	/**
	 * [確認] ヒットテスト
	 *	@param r オリジナルが返したテスト値
	 *	@param pos マウスのウィンドウ相対座標
	 *	@return マウス位置を示すテスト値
	 */
	LRESULT m_OnNcHitTest(LRESULT r, const POINT& pos)
	{
		loop ( i, BK_Max )
		{
			LRESULT l = m_button[i].HitTest(pos, m_windowSize);
			if ( l != HTNOWHERE )
			{
				return l;
			}
		}
		if ( r == HTSYSMENU )
		{
		}
		else if ( m_titleRect.PtInRect(pos) )
		{
			r = HTCAPTION;
		}
		else if ( r == HTCAPTION )
		{
			r = HTNOWHERE;
		}
		else if ( r == HTCLOSE || r == HTMINBUTTON || r == HTMAXBUTTON )
		{
			r = HTCAPTION;
		}
//		TRACE3("OnNcHitTest x=%d, y=%d, r=%d\n", pos.x, pos.y, r);
		return r;
	}

	/**
	 * [通知] マウスムーブ通知
	 * @param wParam マウス位置を示すテスト結果値
	 */
	void m_OnNcMouseMove(WPARAM wParam)
	{
		bool r = false;
		if ( m_pressedButton == BK_Nothing )
		{
			loop ( i, BK_Max )
			{
				r |= m_button[i].SetHoverMode(wParam);
			}
		}
		else
		{
			r |= m_button[m_pressedButton].SetPressMode(wParam);
			r |= m_button[m_pressedButton].SetHoverMode(wParam);
		}
		if ( r )
		{
			m_NcPaint();
		}
	}

	/**
	 * [通知] ボタンイベント通知
	 *	@param[in] message メッセージ
	 *	@param[in] wParam マウス位置を示すテスト値
	 *	@param[in,out] _lParam 座標。変更可能。
	 *	@retval true 処理した
	 *	@retval false defaultの処理を行う
	 */
	bool m_OnNcButton(UINT message, WPARAM wParam, LPARAM& _lParam)
	{
		switch ( message )
		{
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
		default:
			return false;
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
			if ( wParam == HTCLOSE )
			{
				_lParam = MAKELONG(-1, -1);
			}
			return false;
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_LBUTTONUP:
			break;
		}
		if ( message == WM_NCLBUTTONUP || message == WM_LBUTTONUP)
		{
			if ( m_pressedButton == BK_Nothing )
			{
				return false;
			}
			EButtonKind b = m_pressedButton;
			m_SetButtonPressState(BK_Nothing);
			m_button[b].SetPressMode(false);
			m_NcPaint();
			if ( m_button[b].IsHitTest(wParam) )
			{
				m_button[b].PostSysCommand(GetSafeHwnd());
			}
			return true;
		}
		loop ( i, BK_Max )
		{
			if ( m_button[i].IsHitTest(wParam) && m_button[i].isEnable() )
			{
				m_SetButtonPressState(static_cast<EButtonKind>(i));
				m_button[i].SetPressMode(true);
				m_NcPaint();
				return true;
			}
		}
		return false;
	}

	/**
	 * [計算] サイズ計算
	 *	@note ウィンドウの位置やサイズが変更されたときに、アプリケーションでウィンドウのクライアント領域を制御できます。
	 *			boCalcValidRects の値とは無関係に、NCCALCSIZE_PARAMS 構造体のメンバ rgrc で指定した配列の最初の四角形が
	 *			ウィンドウの座標を持ちます。子ウィンドウでの座標は、親ウィンドウのクライアント領域の相対座標です。
	 *			トップレベル ウィンドウでの座標は、画面座標です。
	 *			アプリケーションは、クライアント領域のサイズと位置を反映するために、rgrc[0] の四角形を修正する必要があります。 
	 *			boCalcValidRects が TRUE のときのみ、rgrc[1] と rgrc[2] の四角形が有効になります。
	 *			このとき rgrc[1] には、移動またはサイズ変更される前のウィンドウの座標を保持します。
	 *			rgrc[2] には、ウィンドウが移動される前のウィンドウのクライアント領域の座標が保持されます。
	 *			すべての座標は、親ウィンドウまたは画面の相対座標です。
	 *
	 *			既定の実装では、クライアント領域のサイズをウィンドウの特性 (スクロール バーやメニューなどの有無) 
	 *			を基に計算し、結果を lpncsp に設定します。
	 *	@param boCalcValidRects
	 *					クライアント領域内の有効な情報を含む部分をアプリケーションが指定する必要があるかどうかを指定します。
	 *					Windows は有効な情報を新しいクライアント領域内の指定された領域へコピーします。
	 *					このパラメータが TRUE の場合は、アプリケーションはクライアント領域のどの部分が有効であるかを指定します。
	 *	@param pParams 
	 *					この構造体は、アプリケーションが CWnd の四角形 (クライアント領域、境界線、タイトル、スクロール バーなどを含む)
	 *					の新しい座標やサイズを計算するための情報を保持します。
	 *					
	 */
	void m_OnNcCalcSize(bool boCalcValidRects, NCCALCSIZE_PARAMS* pParams)
	{
		if ( pParams->lppos == NULL || ! boCalcValidRects ) 
		{
			return;
		}
		CPoint po(pParams->lppos->x, pParams->lppos->y);
		CRect rc(CPoint(0, 0), CSize(pParams->lppos->cx, pParams->lppos->cy));
		rc.DeflateRect(m_frameWidths);
		MENUBARINFO mbi = { sizeof(MENUBARINFO) };
		m_menuHeight = 0;
		if ( ::GetMenuBarInfo(m_hWnd, OBJID_MENU, 0, &mbi) )
		{
			m_menuHeight = (mbi.rcBar.bottom - mbi.rcBar.top) + 1;
		}
		rc.top += m_menuHeight;
		pParams->rgrc[0] = rc + po;
	}

	/**
	 * [設定] ボタン押下状態設定
	 *	@param bn ボタン番号
	 */
	void m_SetButtonPressState(EButtonKind bn)
	{
		if ( bn == BK_Nothing )
		{
			if ( m_pressedButton != BK_Nothing )
			{
				m_pressedButton = BK_Nothing;
				::ReleaseCapture();
			}
		}
		else
		{
			if ( m_pressedButton == BK_Nothing )
			{
				m_pressedButton = bn;
				::SetCapture(m_hWnd);
			}
		}
	}

	/**
	 * ボタン管理クラス
	 */
	class CMyButton
	{
	public:
		/// コンストラクタ
		CMyButton(void) : m_onButton(false), m_isPress(false), m_isEnable(true), m_isVisible(false), m_size(0, 0), m_point(0, 0)
		{
		}
		/**
		 * [計算] 位置計算
		 *	@param[out] _rect ウィンドウ座標でのボタン位置＆大きさ
		 *	@param[in] sz ウィンドウの大きさ
		 */
		void GetRect(RECT& _rect, const SIZE& sz) const
		{
			if ( m_isVisible )
			{
				_rect.left = (m_point.x >= 0) ? (m_point.x) : (sz.cx + m_point.x - m_size.cx);
				_rect.right = _rect.left + m_size.cx;
				_rect.top = (m_point.y >= 0) ? (m_point.y) : (sz.cy + m_point.y - m_size.cy);
				_rect.bottom = _rect.top + m_size.cy;
			}
			else
			{
				::SetRectEmpty(&_rect);
			}
		}
		/**
		 * [確認] 有効確認
		 *	@retval true 有効。
		 *	@retval false 無効。
		 */
		bool isEnable(void) const
		{
			return m_isVisible && m_isEnable && ! m_pDrawer.IsNull();
		}
		/**
		 * [処理] ヒットテスト
		 *	@param pos マウスのウィンドウ相対座標
		 *	@param sz ウィンドウの大きさ
		 *	@retval HTNOWHERE以外 上にある。
		 *	@retval HTNOWHERE ない。
		 */
		LRESULT HitTest(const POINT& pos, const SIZE& sz) const
		{
			CRect rc;
			GetRect(rc, sz);
			return rc.PtInRect(pos) ? m_hitTest : HTNOWHERE;
		}
		/**
		 * [処理] ヒットテスト
		 *	@param wParam ヒットテスト結果
		 *	@retval true 上にある。
		 *	@retval falseE ない。
		 */
		bool IsHitTest(WPARAM wParam) const
		{
			return wParam == m_hitTest;
		}
		/**
		 * [設定] 表示状態設定
		 *	@param isVisible true;表示中 false;否
		 */
		void SetVisibleMode(bool isVisible)
		{
			m_isVisible = isVisible;
		}
		/**
		 * [設定] ホバー状態設定
		 *	@param isEnable true;ホバー中 false;否
		 */
		void SetEnableMode(bool isEnable)
		{
			m_isEnable = isEnable;
		}
		/**
		 * [設定] ホバー状態設定
		 *	@param wParam ヒットテスト結果
		 *	@retval true 変化あり。
		 *	@retval false 特に無し。
		 */
		bool SetHoverMode(WPARAM wParam)
		{
			bool isHover = (wParam == m_hitTest);
			bool r = (m_onButton != isHover);
			m_onButton = isHover;
			return r;
		}
		/**
		 * [処理] マウスボタン押下
		 *	@param wParam ヒットテスト結果
		 *	@retval true 変化あり。
		 *	@retval false 特に無し。
		 */
		bool SetPressMode(WPARAM wParam)
		{
			bool isPress = (wParam == m_hitTest);
			bool r = (m_isPress != isPress);
			m_isPress = isPress;
			return r;
		}
		/**
		 * [処理] マウスボタン押下
		 *	@param isPress true;ボタンDOWN false;ボタンUP
		 */
		void SetPressMode(bool isPress)
		{
			m_isPress = isPress;
		}
		/**
		 * [描画] 描画.
		 *	@note 現在の状態で描画
		 *	@param dc DC
		 *	@param sz ウィンドウの大きさ
		 */
		void Draw(HDC dc, const SIZE& sz) const
		{
			if ( ! m_isVisible || m_pDrawer.IsNull() ) 
			{
				return;
			}
			CRect rc;
			GetRect(rc, sz);
			LPARAM p = 0;
			if ( ! m_isEnable )
			{
				p = 3;
			}
			else if ( m_isPress )
			{
				p = 1;
			}
			else if ( m_onButton )
			{
				p = 2;
			}
			m_pDrawer->DrawEx(dc, rc.left, rc.top, p);
		}
		/**
		 * [設定] ボタン設定
		 *	@param pos ボタン位置。座標がマイナスの場合、右端(下端)からのオフセットになります。
		 *	@param draw 描画情報。bank 0;Normal 1;Press 2;Hover
		 *	@param hitTest ヒットテスト値
		 *	@param msg メッセージ
		 */
		void SetDrawer(const POINT& pos, const IDrawable& draw, UINT hitTest, UINT msg)
		{
			m_pDrawer = draw.Clone();
			m_point = pos;
			m_pDrawer->GetSize(m_size);
			m_hitTest = hitTest;
			m_message = msg;
		}
		/**
		 * [送信] システムコマンド送信
		 *	@param hWnd 送信先ウィンドウ
		 */
		void PostSysCommand(HWND hWnd)
		{
			::PostMessage(hWnd, WM_SYSCOMMAND, m_message, 0);
		}
	private:
		CSize			m_size;				///< ボタンサイズ
		CPoint			m_point;			///< 相対座標。マイナスの場合、右(下)からの相対座標
		IDrawable::Ptr	m_pDrawer;			///< Bankあり描画情報  0;通常 1;押下 2;ホバー 3;無効
		bool			m_onButton;			///< 上にある？
		bool			m_isPress;			///< 押下中？
		bool			m_isEnable;			///< 有効？
		bool			m_isVisible;		///< 表示？
		UINT			m_hitTest;			///< ヒットテスト値
		UINT			m_message;			///< 送信メッセージ
	};

	// 専用Brush描画
	class CMyBrushDrawer : public CNullDrawer
	{
		DEFSUPER(CNullDrawer);
		CBrushHandle	m_brush;
	public:
		/// コンストラクタ
		CMyBrushDrawer(CBrushHandle brush) : m_brush(brush)
		{
		}
		/// クローン作成.
		virtual IDrawable* Clone(void) const 
		{
			return new CMyBrushDrawer(*this); 
		}
		/// [描画] 描画.
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			RECT r = { x, y, x + m_size.cx, y + m_size.cy };
			::FillRect(dc, &r, m_brush);
		}
	};
	COLORREF			m_textColor;			///< 文字色
	IDrawable::Ptr		m_pFrameDrawer;			///< フレーム描画(Bank 0;アクティブ 1;非アクティブ)
	CBrushHandle		m_backGround[2];		///< 背景イメージ(Bank 0;アクティブ 1;非アクティブ)
	bool				m_isEvenBackGround;		///< true なら背景イメージは同じもの
	IDrawable::Ptr		m_pTitleDrawer;			///< タイトル描画(Bank 0;アクティブ 1;非アクティブ)
	CRect				m_frameWidths;			///< フレームの厚さ
	CRect				m_titleRect;			///< タイトルバーとする範囲(Rightは使いません)
	int					m_titleHeight;			///< タイトル高さ
	bool				m_isActive;				///< アクティブ?
	CPoint				m_windowPoint;			///< ウィンドウ位置
	CSize				m_windowSize;			///< ウィンドウサイズ
	int					m_menuHeight;			///< メニュー高さ
	bool				m_isInitWindowRgn;		///< true ならリージョン初期化
	HRGN				m_hNcRgn;				///< 
	COLORREF			m_transColor;			///< 透過色

	CMyButton			m_button[BK_Max];		///< ボタン
	EButtonKind			m_pressedButton;		///< プレス中のボタン BK_Nothing;プレス無し
	bool				m_boHasHover;			///< true ならホバー状態
	bool				m_isInit;				///< true なら初期化中
	bool				m_canMenuCheck;			///< true ならメニューの状態チェックを行ってもよい
	//
	CDrawingMenuBar		m_menuBar;				///< マイメニューバー
	CDrawingMenu		m_popupMenu;			///< マイポップアップメニュー
	bool				m_isMenuInit;			///< メニュー初期ずみ？
	bool				m_isNcResized;			///< サイズ変更時に true

	/// ウィンドウの形再設定
	void m_SetWindowRgn(void)
	{
		if ( m_hNcRgn != NULL )
		{
			HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
			::CombineRgn(rgn, m_hNcRgn, NULL, RGN_COPY);
			::SetWindowRgn(m_hWnd, rgn, true);
		}
	}
	/**
	 * SYSTEMメニューを無効して元のプロシージャをコール.
	 */
	LRESULT m_OrgWndProcWithNoSysmeny(UINT message, WPARAM wParam, LPARAM lParam)
	{
		m_canMenuCheck = false;
		LONG nowStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		::SetWindowLong(m_hWnd, GWL_STYLE, nowStyle & (~WS_SYSMENU));
		LRESULT r = _super::WindowProc(message, wParam, lParam);
		::SetWindowLong(m_hWnd, GWL_STYLE, nowStyle);
		m_canMenuCheck = true;
		return r;
	}

	void m_NcPaint(void)
	{
		CRect rc;
		::GetWindowRect(m_hWnd, rc);
		CRect cliRc;
		::GetClientRect(m_hWnd, cliRc);
		if ( m_isInitWindowRgn )
		{
			m_isInitWindowRgn = false;
			if ( m_hNcRgn != NULL )
			{
				_DeleteObject(m_hNcRgn);
			}
			if ( ! m_frameWidths.IsRectNull() && m_transColor != CLR_INVALID && ! m_pFrameDrawer.IsNull() )
			{
				m_pFrameDrawer->Resize(rc.Size());
				CBitmapImage bi = CBitmapDrawer::ToBitmap(*m_pFrameDrawer, m_transColor);
				m_hNcRgn = bi.CreateRgn(m_transColor);
			}
			m_SetWindowRgn();
		}
		cliRc.OffsetRect(m_frameWidths.left, m_frameWidths.top + m_menuHeight);
		HDC dc = ::GetWindowDC(m_hWnd);
		::ExcludeClipRect(dc, cliRc.left, cliRc.top, cliRc.right, cliRc.bottom);
		m_titleRect.right = rc.Width() - m_frameWidths.right;
		if ( m_menuHeight != 0 )
		{
			// メニューあるなら、タイトルバーの高さは固定
			int h = ::GetSystemMetrics(SM_CYCAPTION);
			m_titleRect.bottom = m_titleRect.top + h;
			m_frameWidths.top -= m_titleHeight;
			m_frameWidths.top += h;
			m_titleHeight = h;
		}
		m_OnFramePaint(dc, rc.Size(), m_titleRect, m_isActive);
		::ReleaseDC(m_hWnd, dc);
	}

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_INITDIALOG:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_CheckMenu();
				return r;
			}
			break;
		case WM_CTLCOLOREDIT:
			{
				HWND hWnd = reinterpret_cast<HWND>(lParam);
				if ( (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TRANSPARENT) != 0 )
				{
					HDC dc = reinterpret_cast<HDC>(wParam);
					::SetBkMode(dc, TRANSPARENT);
					HBRUSH b = m_backGround[m_isActive ? 0 : 1];
					_super::WindowProc(message, wParam, lParam);
					if ( IS_RGBVALUE(m_textColor) )
					{
						::SetTextColor(dc, m_textColor);
					}
					return reinterpret_cast<LRESULT>(b);
				}
			}
			break;
		case WM_CTLCOLORSTATIC:
			{
				HWND hWnd = reinterpret_cast<HWND>(lParam);
				LRESULT r = ::SendMessage(hWnd, WM_GETDLGCODE, 0, 0);
				if ( (r & DLGC_HASSETSEL) != 0 )
				{
					// EDITのReadOnly
				}
				//以下に続く
			}	
		case WM_CTLCOLORBTN:
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				::SetBkMode(dc, TRANSPARENT);
				HBRUSH b = m_backGround[m_isActive ? 0 : 1];
				_super::WindowProc(message, wParam, lParam);
				if ( IS_RGBVALUE(m_textColor) )
				{
					::SetTextColor(dc, m_textColor);
				}
				return reinterpret_cast<LRESULT>(b);
			}
			break;
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORMSGBOX:
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				HBRUSH b = m_backGround[m_isActive ? 0 : 1];
				_super::WindowProc(message, wParam, lParam);
				if ( IS_RGBVALUE(m_textColor) )
				{
					::SetTextColor(dc, m_textColor);
				}
				return reinterpret_cast<LRESULT>(b);
			}
			break;
		case WM_WINDOWPOSCHANGED:
			{
				CRect oldRc(m_windowPoint, m_windowSize);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				const WINDOWPOS* P = reinterpret_cast<const WINDOWPOS*>(lParam);
				m_windowPoint.x = P->x;
				m_windowPoint.y = P->y;
				bool b = false;
				b |= (m_windowSize.cx != P->cx);
				m_windowSize.cx = P->cx;
				b |= (m_windowSize.cy != P->cy);
				m_windowSize.cy = P->cy;
				if ( b )
				{
					m_isNcResized = true;
					POINT po;
					if ( ::GetCursorPos(&po) )
					{
						WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
						m_OnNcMouseMove(w);
					}
					m_isInit = true;
					m_isInitWindowRgn = true;
					m_CheckMenu();
					m_NcPaint();
				}
				return r;
			}
			break;
		case WM_NCCALCSIZE:
			if ( ! m_isMenuInit )
			{
				m_isMenuInit = true;
				CFrameDrawer f1(false, BDR_SUNKENINNER, BF_RECT);
				CFrameDrawer f2(false, BDR_RAISEDINNER, BF_RECT);
				m_menuBar.SetSelectMarkDrawer(f1, f2);
				m_menuBar.SetBackgroundDrawer(CMyBrushDrawer(m_backGround[0]));
				m_menuBar.ReferSubParameter().CopyParameter(m_menuBar);
				m_menuBar.ReferSubParameter().SetDefaultMarkDrawer();
				m_menuBar.ReferSubParameter().SetSelectMarkDrawer(CBoxInverter());
				m_popupMenu.CopyParameter(m_menuBar);
				m_popupMenu.SetDefaultMarkDrawer();
				m_popupMenu.SetBackgroundDrawer(CMyBrushDrawer(m_backGround[0]));
				m_popupMenu.SetSelectMarkDrawer(CBoxInverter());
				m_menuBar.SetLeftMargin(-1);
			}
			if ( ::GetMenu(m_hWnd) != NULL && m_menuBar.m_hMenu == NULL )
			{
				m_menuBar.Attach(this, false);
				::DrawMenuBar(m_hWnd);
				m_menuBar.Draw();
			}
			{
				NCCALCSIZE_PARAMS* P = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_OnNcCalcSize(wParam != 0, P);
				return r;
			}
			break;
		case WM_SIZE:
		case WM_MOVE:
			m_SetWindowRgn();
			break;
		case WM_NCPAINT:
			m_NcPaint();
			return 0;
			break; 
		case WM_SETCURSOR:
			return m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
			break;
		case WM_NCHITTEST:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				CPoint p(lParam);
				return m_OnNcHitTest(r, CPoint(p.x - m_windowPoint.x, p.y - m_windowPoint.y));
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_pressedButton >= 0 )
			{
				CPoint po(lParam);
				::ClientToScreen(m_hWnd, &po);
				WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
				m_OnNcMouseMove(w);
			}
			break;
		case WM_CAPTURECHANGED:
			if ( m_pressedButton != BK_Nothing )
			{
				m_SetButtonPressState(BK_Nothing);
				loop ( i, BK_Max )
				{
					m_button[i].SetPressMode(false);
				}
			}
			break;
		case WM_ACTIVATE:
			m_isActive = !! wParam;
			if ( ! m_isEvenBackGround )
			{
				m_menuBar.SetBackgroundDrawer(CMyBrushDrawer(m_backGround[m_isActive ? 0 : 1]));
				::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE);
			}
			else
			{
				CRect rc(0, 0, 1, 1);
				::RedrawWindow(m_hWnd, rc, NULL, RDW_FRAME | RDW_NOERASE | RDW_NOINTERNALPAINT | RDW_INVALIDATE);
			}
			m_SetWindowRgn();
			break;
		case WM_NCACTIVATE:
			m_NcPaint();
			return TRUE;
			break;
		case WM_NCMOUSELEAVE:
			if ( m_boHasHover )
			{
				m_boHasHover = false;
				POINT po;
				if ( ::GetCursorPos(&po) )
				{
					WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
					m_OnNcMouseMove(w);
				}
			}
			break;
		case WM_NCMOUSEMOVE:
			if ( m_isNcResized )
			{
				m_isNcResized = false;
				m_SetWindowRgn();
			}
			if ( m_isInit && m_isActive )
			{
				m_isInit = false;
				if ( (::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == 0 )
				{
					SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
					SendMessage(WM_NCLBUTTONUP, HTCAPTION, 0);
				}
				m_NcPaint();
			}
			m_OnNcMouseMove(wParam);
			if ( ! m_boHasHover )
			{
				m_boHasHover = true;
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof (TRACKMOUSEEVENT);
				tme.hwndTrack = m_hWnd;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
				::_TrackMouseEvent(&tme);
			}
			if ( m_isInit )
			{
				m_NcPaint();
			}
			return m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
			break;
		case WM_INITMENU:
			{
				LRESULT r = m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
				m_NcPaint();
				return r;
			}
			break;
		case WM_INITMENUPOPUP:
			if ( m_popupMenu.m_hMenu != NULL )
			{
				m_popupMenu.Detach();
			}
			m_popupMenu.Attach(this, reinterpret_cast<HMENU>(wParam), true, true);
			if ( HIWORD(lParam) == 1 )
			{
				LRESULT r = m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
				m_NcPaint();
				return r;
			}
			break;
		case WM_MENUSELECT:
		case WM_STYLECHANGED:
			if ( m_canMenuCheck )
			{
				m_CheckMenu();
				m_NcPaint();
			}
			break;
		case WM_LBUTTONUP:
			if ( m_pressedButton >= 0 )
			{
				CPoint po(lParam);
				::ClientToScreen(m_hWnd, &po);
				WPARAM w = SendMessage(WM_NCHITTEST, 0, MAKELONG(po.x, po.y));
				if ( m_OnNcButton(message, w, lParam) )
				{
					return 0;
				}
			}
			break;
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONUP:
			if ( m_OnNcButton(message, wParam, lParam) )
			{
				return 0;
			}
			switch ( wParam )
			{
			case HTCAPTION:
			case HTLEFT:
			case HTRIGHT:
			case HTTOP:
			case HTTOPLEFT:
			case HTTOPRIGHT:
			case HTBOTTOM:
			case HTBOTTOMLEFT:
			case HTBOTTOMRIGHT:
				return m_OrgWndProcWithNoSysmeny(message, wParam, lParam);
			}
			break;
		case WM_NCLBUTTONUP :
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONDBLCLK:
			if ( m_OnNcButton(message, wParam, lParam) )
			{
				return 0;
			}
			break;
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
			return 0;
		case WM_DESTROY:
		case WM_NCDESTROY:
			{
				if ( m_hNcRgn != NULL )
				{
					_DeleteObject(m_hNcRgn);
					m_hNcRgn = NULL;
				}
				::ReleaseCapture();
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				_super::Destroy();
				return r;
			}
			break;
		case WM_THEMECHANGED:
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}
};



}; //MFC
}; //TNB
