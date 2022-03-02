#pragma once
/**
 *@file
 * 押しっぱなし可能のボタン関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



#ifdef _WIN32_WCE
 #error not support "PressButton"
#endif // _WIN32_WCE



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #ifndef BN_PUSHED
  #define BN_PUSHED BN_HILITE
 #endif
 #ifndef BN_UNPUSHED
  #define BN_UNPUSHED BN_UNHILITE 
 #endif
#endif //_TnbDOXYGEN



/**@ingroup MFCCONTROL
 * ボタンコントロール
 *
 *			ボタンを押下後、そのまま離さず、移動させることで、押しっぱなしも
 *			表現可能なボタンです。
 *
 *	@par使い方
 *		リソースエディタでボタンかラジオボタンかチェックボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton）を本クラスに置き換えて使用します。
 *		プレス時、 BN_PUSHED 、リリース時、 BN_UNPUSHED 、親ウィンドウに WM_COMMAND で通視されます。
 *
 *	@par必要ファイル
 *			TnbMfcPressButton.h
 * 
 *	@date 06/09/26 新規作成
 */
class CPressButton : public CButton
{
	DEFSUPER(CButton);
	COLORREF	m_colorBack;		///< 背景の色
	bool		m_boIsPushed;		///< 

	void PostCommand(UINT uCmd)
	{
		MFCLIB::SendCommandMessage(this, uCmd);
//		CWnd* pcWnd = GetParent();
//		if ( pcWnd != NULL )
//		{
//			pcWnd->PostMessage(WM_COMMAND, uCmd * 0x10000 | GetDlgCtrlID(), 
//												reinterpret_cast<LPARAM>(m_hWnd));
//		}
	}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return _super::OnCommand(wParam, lParam);
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
		if ( message == WM_LBUTTONDOWN )
		{
			if ( ! m_boIsPushed )
			{
				m_boIsPushed = true;
				RedrawWindow();
				PostCommand(BN_PUSHED);
			}
			return 0;
		}
		else if ( message == WM_LBUTTONUP )
		{
			if ( m_boIsPushed )
			{
				m_boIsPushed = false;
				RedrawWindow();
				PostCommand(BN_UNPUSHED);
				PostCommand(BN_CLICKED);				
			}
			return 0;
		}
		else if ( message == WM_LBUTTONDBLCLK )
		{
			if ( ! m_boIsPushed )
			{
				PostCommand(BN_PUSHED);
			}
			m_boIsPushed = false;
			RedrawWindow();
			PostCommand(BN_UNPUSHED);
			PostCommand(BN_DOUBLECLICKED);				
			return 0;
		}
		else if ( message == WM_SETFOCUS || message == WM_KILLFOCUS )
		{
			UpdateWindow();
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
		UINT uiDefBtnSte = _super::GetButtonStyle() & 0xFF;
		if ( uiDefBtnSte == BS_PUSHBUTTON || uiDefBtnSte == BS_DEFPUSHBUTTON )
		{
			_super::SetButtonStyle(BS_AUTOCHECKBOX);
			_super::ModifyStyle(0, BS_OWNERDRAW);
		}
		_super::PreSubclassWindow();
	}

	/**
	 * [通知] 描画.
	 *		描画するタイミングでコールされる。
	 *	@note CButton の virtual メソッド
	 *	@param lpDrawItemStruct 描画するための情報
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if ( lpDrawItemStruct->CtlType != ODT_BUTTON || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
		dc->SaveDC();
		UINT uItemState = lpDrawItemStruct->itemState;

		//== ボタンフレーム描画
		UINT uState = DFCS_BUTTONPUSH | 0x0800/*DFCS_TRANSPARENT*/;
		if ( m_boIsPushed )
		{
			uState |= DFCS_PUSHED;
		}
		dc->DrawFrameControl(&(lpDrawItemStruct->rcItem), DFC_BUTTON, uState);
		//===== 内枠FILL
		if ( ToInt(m_colorBack) >= 0 )
		{
			CRect cRect = lpDrawItemStruct->rcItem;
			cRect.DeflateRect(2, 2, 2, 2);
			dc->FillSolidRect(cRect, m_colorBack);
		}
		//== 文字
		CString strText;
		GetWindowText(strText);
		if ( ! strText.IsEmpty() )
		{
			CRect cRect = lpDrawItemStruct->rcItem;
			cRect.DeflateRect(4, 4, 4, 4);
			if ( m_boIsPushed )
			{
				cRect.OffsetRect(1, 1);
			}
			dc->SetBkMode(TRANSPARENT);		// バックカラーは変更なし
			if ( (uItemState & ODS_DISABLED) != 0 )
			{
				dc->SetTextColor(GetSysColor(COLOR_GRAYTEXT));	//グレイ
			}
			dc->DrawText(strText, cRect, DT_CENTER | DT_VCENTER);
		}	
//		//== フォーカス点線
//		if ( ::GetFocus() == m_hWnd )
//		{
//			CRect cRect = lpDrawItemStruct->rcItem;
//			cRect.DeflateRect(3,3,3,3);
//			if ( m_boIsPushed )
//			{
//				cRect.OffsetRect(1,1);
//			}
//			dc->DrawFocusRect(&cRect);
//		}
		dc->RestoreDC(-1);
	}

public:

	/// コンストラクタ
	CPressButton(void)
		: m_colorBack(COLORREF(-1))
		, m_boIsPushed(false)
	{
	}
	
	/**
	 * [設定] ボタン色
	 *	@note ボタンの表面の色を指定できます。
	 *	@param color カラー。省略すると、システムカラーになります。
	 */
	void SetBkColor(COLORREF color = -1)
	{
		m_colorBack = color;
	}

	/**
	 * [取得] ボタン状態
	 *	@retval true 押下状態
	 *	@retval false 非押下状態
	 */
	bool IsPushed(void) const
	{
		return m_boIsPushed;
	}

	/**
	 * [設定] ボタン押下状態
	 */
	void Push(void)
	{
		PostMessage(WM_LBUTTONDOWN);
	}

	/**
	 * [設定] ボタン非押下状態
	 */
	void Unpush(void)
	{
		PostMessage(WM_LBUTTONUP);
	}

};



}; //MFC
}; //TNB



