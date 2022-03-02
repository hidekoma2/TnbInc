#pragma once
/**
 *@file
 * 描画情報マーキー表示関係のヘッダ
 *
 *		描画情報をマーキー表示するためのコントロールを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapDrawer.h"
#include "TnbTextDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報マーキー表示コントロールクラス
 *
 *		描画情報を移動させながら表示するコントロールです。
 *
 *	@par使い方
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcMarqueeStatic.h
 * 
 *	@date 10/05/20 新規作成
 *	@date 10/11/12 方向に IMMOVALE を追加
 */
class CMarqueeStatic : public CWnd
{
	DEFSUPER(CWnd);
public:

	/// コンストラクタ
	CMarqueeStatic(void) : _super(), m_margin(0, 0), m_mode(Mode_Scroll)
		, m_direction(LEFT), m_amount(6), m_delay(85)
		, m_backColor(RGB(0, 0, 0)), m_withBackDraw(true)
	{
	}

	/**
	 * [設定] マージン.
	 *	@param cx 左右の余白
	 *	@param cy 上下の余白
	 */
	void SetMargin(size_t cx, size_t cy)
	{
		m_margin.cx = cx;
		m_margin.cy = cy;
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			CRect rc;
			_super::GetClientRect(&rc);
			m_clientSize = rc.Size() - m_margin - m_margin;
			m_Reset();
		}
	}

	/// モード
	enum EMode
	{
		Mode_Scroll,	///< スクロール
		Mode_Slide,		///< 端で停止
	};

	/**
	 * [設定] モード設定.
	 *	@param mode モード
	 */
	void SetMode(EMode mode)
	{
		m_mode = mode;
		m_Reset();
	}
	
	/// 方向
	enum EDirection
	{
		LEFT,		///< 右から左へ
		RIGHT,		///< 左から右へ
		UP,			///< 下から上へ
		DOWN,		///< 上から下へ
		IMMOVALE,	///< 不動
	};

	/**
	 * [設定] 方向設定.
	 *	@param d 方向
	 */
	void SetDirection(EDirection d)
	{
		m_direction = d;
		m_Reset();
	}

	/**
	 * [設定] 移動速度設定.
	 *	@param amount 一回の移動量.
	 *	@param delay 移動間隔(ms)
	 */
	void SetSpeed(DWORD amount, DWORD delay)
	{
		m_amount = amount;
		m_delay = delay;
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			SetTimer(TIMERID_MARQUEE, m_delay, NULL);
		}
	}

	/**
	 * [設定] 背景色設定.
	 *	@param c 背景色.
	 */
	void SetBackColor(COLORREF c)
	{
		m_backColor = c;
		m_Reset();
	}

	/**
	 * [設定] 描画情報設定.
	 *	@param draw 描画情報. 本オブジェクトにはクローンが記憶されるので、 draw は破棄してもかまいません。
	 *	@param withBackDraw 描画情報が背景を全部描画する場合、 true を指定してください。
	 */
	void SetDrawingObject(const IDrawable& draw, bool withBackDraw = true)
	{
		m_withBackDraw = withBackDraw;
		m_pDrawing = draw.Clone();
		if ( ! m_pDrawing.IsNull() )
		{
			m_pDrawing->GetSize(m_drawingSize);
		}
		m_Reset();
	}

	/**
	 * [設定] 描画情報設定.
	 *	@param bmp ビットマップ
	 */
	void SetDrawingBitmap(CBitmapHandle bmp)
	{
		SetDrawingObject(CBitmapDrawer(bmp), true);
	}

	/**
	 * [設定] 描画文字列設定.
	 *	@note 文字の背景は、本メソッドより先に、 SetBackColor() を使い設定する必要があります。
	 *	@param font フォント.
	 *	@param textColor 文字色.
	 *	@param lpszText テキスト.
	 *	@param style スタイル
	 *	@param margin マージン.
	 */
	void SetDrawingText(HFONT font, COLORREF textColor, LPCTSTR lpszText, DWORD style = DT_CENTER | DT_VCENTER, int margin = 0)
	{
		CTextBkDrawer d(font, textColor, m_backColor, lpszText, style, margin);
		d.AdjustSize();
		SetDrawingObject(d, true);
	}

protected:

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
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
		case WM_ERASEBKGND:
			TTRACE0("WM_ERASEBKGND\n");
			return 0;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				m_Draw(pDC);
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // 描画用のデバイス コンテキスト
				m_Draw(&dc);
			}
			return 0;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			return 0;
		case WM_TIMER:
			if ( wParam == TIMERID_MARQUEE )
			{
				if ( _super::IsWindowVisible() )
				{
					m_Next();
					_super::RedrawWindow();
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
		_super::ModifyStyle(0, SS_OWNERDRAW);
		_super::PreSubclassWindow();
		SetTimer(TIMERID_MARQUEE, m_delay, NULL);
		CRect rc;
		_super::GetClientRect(&rc);
		m_clientSize = rc.Size() - m_margin - m_margin;
		m_isStarted = false;
	}

private:

	void m_Reset(void)
	{
		m_isStarted = false;
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			SetTimer(TIMERID_MARQUEE, m_delay, NULL);
		}
	}

	/// 描画.
	void m_Draw(CDC* pDC)
	{
		if ( ! m_pDrawing.IsNull() )
		{
			CRect rc;
			_super::GetClientRect(&rc);
			pDC->IntersectClipRect(&rc);
			CPoint po = m_drawPoint + m_margin;
			if ( m_withBackDraw )
			{
				//背景も描画するDrawerだ
				m_pDrawing->Draw(*pDC, po.x, po.y);
				pDC->ExcludeClipRect(CRect(po, m_drawingSize));
				pDC->FillSolidRect(&rc, m_backColor);
			}
			else
			{
				//背景は書いてくれないDrawerだ
				pDC->FillSolidRect(&rc, m_backColor);
				m_pDrawing->Draw(*pDC, po.x, po.y);
			}
		}
	}

	// メンバの状態から、 m_drawPoint を更新する。
	void m_Next(void)
	{
		if ( ! m_isStarted )
		{
			// 開始！
			switch ( m_direction )
			{
			case LEFT: //右から開始
				m_drawPoint = CPoint(m_clientSize.cx + m_margin.cx, 0);
				break;
			case RIGHT: //左から開始
				m_drawPoint = CPoint(-m_drawingSize.cx - m_margin.cx, 0);
				break;
			case UP: //下から開始
				m_drawPoint = CPoint(0, m_clientSize.cy + m_margin.cy);
				break;
			case DOWN: //上から開始
				m_drawPoint = CPoint(0, -m_drawingSize.cy - + m_margin.cy);
				break;
			case IMMOVALE: //不動
				m_drawPoint = CPoint(0, 0);
				KillTimer(TIMERID_MARQUEE);
				break;
			}
			m_isStarted = true;
		}
		int w = (m_clientSize.cx - m_drawingSize.cx);
		int h = (m_clientSize.cy - m_drawingSize.cy);
		switch ( m_direction )
		{
		case LEFT: //左へ
			m_drawPoint.x -= m_amount;
			break;
		case RIGHT: //右へ
			m_drawPoint.x += m_amount;
			break;
		case UP: //上へ
			m_drawPoint.y -= m_amount;
			break;
		case DOWN: //下へ
			m_drawPoint.y += m_amount;
			break;
		}
		if ( m_mode == Mode_Scroll )
		{
			switch ( m_direction )
			{
			case LEFT: //左を超えたら右から
				if ( m_drawPoint.x < -m_drawingSize.cx ) 
				{
					m_drawPoint.x = m_clientSize.cx + m_margin.cx; 
				}
				break;
			case RIGHT: //右を超えたら左から
				if ( m_drawPoint.x > m_clientSize.cx )
				{
					m_drawPoint.x = -m_drawingSize.cx - m_margin.cx;
				}
				break;
			case UP: //上を超えたら下から
				if ( m_drawPoint.y < -m_drawingSize.cy ) 
				{
					m_drawPoint.y = m_clientSize.cy + m_margin.cy;
				}
				break;
			case DOWN: //下を超えたら上から
				if ( m_drawPoint.y > m_clientSize.cy )
				{
					m_drawPoint.y = -m_drawingSize.cy - m_margin.cy;
				}
				break;
			}
		}
		if ( m_mode == Mode_Slide )
		{
			bool isContinue = true;
			switch ( m_direction )
			{
			case LEFT: //左で終わり
				if ( m_drawPoint.x < 0 ) { m_drawPoint.x = 0; isContinue = false; }
				break;
			case RIGHT: //右で終わり
				if ( m_drawPoint.x > w ) { m_drawPoint.x = w; isContinue = false; }
				break;
			case UP: //上で終わり
				if ( m_drawPoint.y < 0 ) { m_drawPoint.y = 0; isContinue = false; }
				break;
			case DOWN: //下で終わり
				if ( m_drawPoint.y > h ) { m_drawPoint.y = h; isContinue = false; }
				break;
			}
			if ( ! isContinue )
			{
				KillTimer(TIMERID_MARQUEE);
			}
		}
	}

	enum
	{
		TIMERID_MARQUEE = 1,	
	};

	IDrawable::Ptr	m_pDrawing;		///< 保持する描画情報
	CSize			m_margin;		///< マージン
	EMode			m_mode;			///< モード
	EDirection		m_direction;	///< 方向
	DWORD			m_amount;		///< 一回の移動量
	DWORD			m_delay;		///< 移動間隔(ms)
	COLORREF		m_backColor;	///< 背景色
	bool			m_withBackDraw;	///< 背景も描画する描画情報
	// -- work
	CSize			m_drawingSize;	///< 描画サイズ(m_pDrawing の描画の大きさ)
	CSize			m_clientSize;	///< クライアントサイズ
	CPoint			m_drawPoint;	///< 現在の表示位置
	bool			m_isStarted;	///< 開始フラグ
};



}; //MFC
}; //TNB



