#pragma once
/**
 *@file
 * 自由型透明ボタン関係のヘッダ
 *
 *		透明なボタンを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcAbstractButton.h"
#include "TnbMfcCursorControl.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL 
 * 自由型透明ボタンコントロール
 *
 *		透明なボタンを提供します。メインダイアログの背景が透過するので、背景をクリックするような処理が出来ます。
 *
 *	@note マウスをホバリングするとマウスカーソルが変化します。
 *
 *	@par使い方
 *		リソースエディタでボタンかスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton or CStatic）を本クラスに置き換えて使用します。
 *
 *	@see CAbstractButton
 *
 *	@par必要ファイル
 *			TnbMfcTransparentButton.h
 * 
 *	@date 09/04/14 新規作成
 */
class CTransparentButton : public CCursorControlAddinT<CAbstractButton>
{
	DEFSUPER(CCursorControlAddinT<CAbstractButton>);
public:

	/// コンストラクタ
	CTransparentButton(void) : m_hRgn(::CreateRectRgn(0, 0, 0, 0)), m_hoverParsent(-1)
	{
	}

	/// デストラクタ
	~CTransparentButton(void)
	{
		if ( m_hRgn != NULL )
		{
			_DeleteObject(m_hRgn);
			m_hRgn = NULL;
		}
	}

	/**
	 * [設定] ボタンサイズ設定.
	 *	@note ボタンは矩形になります。
	 *	@param sz サイズ
	 */
	void SetButtonSize(const SIZE& sz)
	{
		HRGN rgn = ::CreateRectRgn(0, 0, sz.cx, sz.cy);
		SetButtonRgn(rgn);
		_DeleteObject(rgn);
	}

	/**
	 * [設定] ボタン領域設定.
	 *	@note ボタンは自由形になります。
	 *	@param rgn 領域。設定後破棄してかまいません。
	 */
	void SetButtonRgn(HRGN rgn)
	{
		HRGN h = ::CreateRectRgn(0, 0, 0, 0);
		::CombineRgn(h, rgn, NULL, RGN_COPY);
		::CombineRgn(m_hRgn, rgn, NULL, RGN_COPY);
		CRect rc;
		::GetRgnBox(m_hRgn, rc);
		_super::SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOZORDER);
		_super::SetWindowRgn(h, true);
		m_hoverFill.Empty();
	}

	/**
	 * [設定] フォーカス色設定.
	 *	@param c カラー。　
	 */
	void SetFocusColor(COLORREF c = CLR_INVALID)
	{
		if ( m_focusBrush.GetSafeHandle() != NULL )
		{
			m_focusBrush.DeleteObject();
		}
		if ( IS_RGBVALUE(c) )
		{
			m_focusBrush.CreateHatchBrush(HS_DIAGCROSS, c);
		}
	}

	/**
	 * [設定] ホバーカラー設定.
	 *	@param parsent 濃さ。 100 で color そのもの。 -1 で、ホバー指定解除
	 *	@param color カラー。
	 */
	void SetHoverFillColor(int parsent, COLORREF color = RGB(0, 0, 0))
	{
		m_hoverParsent = parsent;
		m_hoverColor = color;
		m_hoverFill.Empty();
	}

protected:
	HRGN			m_hRgn;			///< コントロールリージョン
	CBrush			m_focusBrush;	///< フォーカスブラシ
	COLORREF		m_hoverColor;	///< ホバーカラー
	int				m_hoverParsent;	///< ホバーカラー透明度
	CBitmapImage	m_hoverFill;	///< ホバー用

	/**
	 * [通知] 描画
	 *	@param pDC 描画先のCDC。
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		switch ( _super::GetButtonState() )
		{
		case ES_Pushed:
		case ES_Hover:
			if ( m_hoverParsent >= 0 )
			{
				if ( m_hoverFill.IsEmpty() )
				{
					CRect rc;
					GetClientRect(rc);
					m_hoverFill.Set(rc.Width(), rc.Height(), m_hoverColor);
				}
				::SelectClipRgn(pDC->GetSafeHdc(), m_hRgn);
				m_hoverFill.AlphaBlend(*pDC, 0, 0, m_hoverParsent);
				::SelectClipRgn(pDC->GetSafeHdc(), NULL);
			}
			break;
		}
		if ( HasFocus() )
		{
			if ( m_focusBrush.GetSafeHandle() != NULL )
			{
				pDC->SetBkMode(TRANSPARENT);
				::FrameRgn(pDC->GetSafeHdc(), m_hRgn, m_focusBrush, 1, 1);
			}
		}
	}

	/**
	 * [通知] 状態変化通知.
	 *	@note 現在の状態を取得します
	 *	@param state 状態。
	 */
	virtual void OnChangeState(EState state)
	{
		MFCLIB::DrawParent(this, false);
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
		if ( message == WM_DESTROY )
		{
			if ( m_hRgn != NULL )
			{
				_DeleteObject(m_hRgn);
				m_hRgn = NULL;
			}
			if ( m_focusBrush.GetSafeHandle() != NULL )
			{
				m_focusBrush.DeleteObject();
			}
		}
		if ( message == WM_ERASEBKGND )
		{
			return 0;
		}
		LRESULT r = _super::WindowProc(message, wParam, lParam);
		if ( message == WM_SETFOCUS || message == WM_KILLFOCUS || message == WM_ACTIVATE || message == WM_MOUSELEAVE )
		{
			MFCLIB::DrawParent(this, false);
		}
		if ( message >= WM_LBUTTONDOWN && message <= WM_MOUSELAST )
		{
			MFCLIB::DrawParent(this, true);
		}
		return r;
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		CRect rc;
		_super::GetWindowRect(&rc);
		SetButtonSize(rc.Size());
		_super::PreSubclassWindow();
	}
};



}; //MFC
}; //TNB



