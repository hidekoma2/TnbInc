#pragma once
/**
 *@file
 * カーソルコントロール処理関係のヘッダ
 *
 *		コントロールの状態でマウスカーソルを変化させるための機能を提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



#ifndef __AFX_H__
 #error
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * マウスカーソル制御クラス
 *
 *		inline なコントロールです。
 *
 *		本クラスを使用すると、指定のコントロールの状態によりマウスカーソルを変化させることが出来ます。
 *
 *	@param TYP 要素の型。CWnd のサブクラスである必須があります。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		//= 型宣言 (typedefしておかないと Wizardでエラーに成ります)
 *		typedef CCursorControlAddinT<CStatic> CCcStatic;	
 *
 *		//= ダイアログクラス宣言
 *	 	CCcStatic	m_text;		// <- 元々 CStaticだったメンバを書き換えた		
 *
 *		//= 設定(OnInitDialog()など)
 *		m_text.SetHoverCursor(::LoadCursor(NULL, IDC_HAND));
 *		</pre></td></tr></table>
 *		
 *	@par必要ファイル
 *			TnbMfcCursorControl.h
 * 
 *	@date 07/10/19 新規作成
 *	@date 07/10/30 チラツキをなくした。
 */
template<typename TYP = CStatic>
class CCursorControlAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// コンストラクタ
	CCursorControlAddinT(void) : m_normalCursor(NULL), m_hoverCursor(NULL), m_pushedCursor(NULL)
		, m_boHasHover(false), m_boIsPushed(false), m_boIsEnable(true)
	{
		#ifdef IDC_HAND
			m_hoverCursor = ::LoadCursor(NULL, IDC_HAND);
		#endif
	}

	/**
	 * [設定] ホバー状態のカーソル設定.
	 *	@param h カーソル。本インスタンスが破棄されるまで破棄しないでください。
	 */
	void SetHoverCursor(HCURSOR h)
	{
		m_hoverCursor = h;
	}

	/**
	 * [設定] 押下状態のカーソル設定.
	 *	@param h カーソル。本インスタンスが破棄されるまで破棄しないでください。
	 */
	void SetPushedCursor(HCURSOR h)
	{
		m_pushedCursor = h;
	}

	/**
	 * [設定] カーソル設定コピー.
	 *		他のコントロールの設定をコピーします。
	 *	@param other 他のコントロール。
	 */
	template<typename T>
	void CopyCursorState(const CCursorControlAddinT<T>& other)
	{
		m_hoverCursor = other.m_hoverCursor;
		m_pushedCursor = other.m_pushedCursor;
	}

	/**
	 * [設定] 特殊カーソルモード.
	 *		本クラスのカーソル変更機能を一時的に停止し、指定のカーソルにします。
	 *	@note 元に戻すには、 ResetCursor() を使用します。
	 *	@param h カーソル。
	 */
	void SpecialCursor(HCURSOR h)
	{
		m_boIsEnable = false;
		::SetCursor(h);
	}

	/**
	 * [設定] 特殊カーソルモード解除.
	 *		SpecialCursor() の設定を解除します。
	 */
	void ResetCursor(void)
	{
		m_boIsEnable = true;
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
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if ( ! m_boIsPushed )
			{
				m_boIsPushed = true;
				m_CheckNowState();
			}
			break;
		case WM_LBUTTONUP:
			if ( m_boIsPushed )
			{
				m_boIsPushed = false;
				m_CheckNowState();
			}
			break;
		case WM_MOUSEMOVE:
		case WM_MOUSEHOVER:
			if ( ! m_boHasHover )
			{
				m_boHasHover = true;
				m_HoverSub();
			}
			m_CheckNowState();
			break;
		case WM_MOUSELEAVE:
			if ( m_boHasHover )
			{
				m_boHasHover = false;
				m_CheckNowState();
			}
			break;
		case WM_ENABLE:
			m_CheckNowState();
			break;
		case WM_DESTROY:
			m_boHasHover = false;
			m_boIsPushed = false;
			m_CheckNowState();
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	HCURSOR				m_normalCursor;	///< 標準状態のカーソル
	HCURSOR				m_hoverCursor;	///< ホバー状態のカーソル
	HCURSOR				m_pushedCursor;	///< 押下状態のカーソル
	bool				m_boHasHover;	///< true ならホバー状態
	bool				m_boIsPushed;	///< true なら押下状態
	bool				m_boIsEnable;	///< true なら機能する
	/// 次のホバーイベントをもらうための処理
	void m_HoverSub(void)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		::_TrackMouseEvent(&tme);
	}
	/// 現在の状態チェックしカーソル変更	
	void m_CheckNowState(void)
	{
		if ( ! m_boIsEnable ) { return; }
		if ( m_normalCursor == NULL )
		{
			m_normalCursor = ::GetCursor();
			::SetClassLongPtr(m_hWnd, GCLP_HCURSOR, NULL); //Winに関連している CursorをNULL化
		}
		HCURSOR h = m_normalCursor;
		if ( _super::IsWindowEnabled() )
		{
			if ( m_boIsPushed && m_pushedCursor != NULL )
			{
				h = m_pushedCursor;
			}
			else if ( m_boHasHover && m_hoverCursor != NULL )
			{
				h = m_hoverCursor;
			}
		}
		::SetCursor(h);
	}
};



}; //MFC
}; //TNB



