#pragma once
/**
 *@file
 * テキスト付き進捗コントロール関係のヘッダ
 *
 *		テキスト付きの進捗コントロールを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbStr.h"
#include "TnbBitmapImage.h"
#include "TnbMfcBitmapDC.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * テキスト付き進捗コントロールクラス
 *
 *	@par使い方
 *		リソースエディタでプログレスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CProgressCtrl）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcTextProgressCtrl.h
 * 
 *	@date 16/06/28 新規作成
 */
class CTextProgressCtrl : public CProgressCtrl
{
	DEFSUPER(CProgressCtrl);
public:

	/// コンストラクタ
	CTextProgressCtrl(void) : m_barColor(::GetSysColor(COLOR_HIGHLIGHT)), m_backColor(::GetSysColor(COLOR_WINDOW))
	{
	}

	/**
	 * [設定] 文字列設定.
	 *	@param lpszText 文字列
	 */
	void SetWindowText(LPCTSTR lpszText)
	{
		_super::SetWindowText(lpszText);	
		m_CreateImage();
		RedrawWindow();
	}

	/**
	 * [設定] カラー設定.
	 *	@param barColor 文字、バーの色
	 *	@param backColor 背景の色
	 */
	void SetColor(COLORREF barColor, COLORREF backColor)
	{
		m_barColor = barColor;
		m_backColor = backColor;
		m_CreateImage();
		RedrawWindow();
	}	

	/**
	 * [設定] フォント設定
	 *	@param pFont フォント。指定したフォントは破棄しないでください。
	 *	@param bRedraw ウィンドウを再描画する場合は TRUE。それ以外の場合は FALSE。
	 */
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE)
	{
		_super::SetFont(pFont, FALSE);
		if ( bRedraw )
		{
			m_CreateImage();
			RedrawWindow();
		}
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_CreateImage();
		_super::PreSubclassWindow();
		RedrawWindow();
	}
	
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
		case WM_PRINTCLIENT:
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				if ( pDC != NULL )
				{
					OnDrawProgressImage(pDC);
				}
			}
			return 0;
		case WM_PAINT:
			{
				CPaintDC dc(this); // 描画用のデバイス コンテキスト
				OnDrawProgressImage(&dc);
			}
			return 0;
		case WM_SIZE:
			{
				CSize sz(lParam);
				m_CreateImage(sz.cx, sz.cy);
			}
			break;
		default:
			break;
		}		
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// プログレスバーのイメージ表示
	void OnDrawProgressImage(CDC* pDC)
	{
		CSize sz = m_image1.GetSize();
		//
		int f = _super::GetPos();
		int m1, m2;
		GetRange(m1, m2);
		int xx = f * sz.cx / (m2 - m1);
		CRect r1(0, 0, xx, sz.cy);
		m_image2.Cut(r1).Draw(pDC->GetSafeHdc());
		CRect r2(xx, 0, sz.cx, sz.cy);
		m_image1.Cut(r2).Draw(pDC->GetSafeHdc(), xx, 0);
	}

	/// イメージ作成サブ
	void m_Sub(CBitmapImage& _bi, int cx, int cy, COLORREF c1, COLORREF c2)
	{
		_bi.Set(cx, cy, c2);
		CBitmapDC dc(&_bi);
		CFont* pFont = dc.SelectObject(GetFont());
		CString s;
		GetWindowText(s);
		dc.SetTextColor(c1);
		dc.SetBkColor(c2);
		CRect rc(0, 0, cx, cy);
		CTextDrawer::DrawTextRect(dc, rc, DT_CENTER | DT_VCENTER, c1, s);
		dc.SelectObject(pFont);
	}

	/// イメージ作成
	void m_CreateImage(int cx = 0, int cy = 0)
	{
		if ( cx == 0 || cy == 0 )
		{
			CRect rc;
			GetWindowRect(rc);
			cx = rc.Width();
			cy = rc.Height();
		}
		m_Sub(m_image1, cx, cy, m_barColor, m_backColor);
		m_Sub(m_image2, cx, cy, m_backColor, m_barColor);
	}

	COLORREF		m_backColor;	///< 背景色
	COLORREF		m_barColor;		///< 描画色
	CBitmapImage	m_image1;		///< 内部イメージ
	CBitmapImage	m_image2;		///< 内部イメージ
};



};
};
