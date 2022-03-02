#pragma once
/**
 *@file
 * 遅延再描画コントロール処理関係のヘッダ
 *
 *		多くの情報の更新の際、描画のタイミングを遅らせて、まとめて描画する（処理を軽減出来る）機能を提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcSetRedrawEx.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 遅延再描画アドイン
 *
 *		inline なコントロールです。
 *
 *		多くの情報の更新の際、描画のタイミングを遅らせて、まとめて描画する（処理を軽減出来る）機能を提供します。
 *
 *	@param TYP 要素の型。CWnd のサブクラスである必須があります。
 *
 *	@par使用例
 *		\code
 *		//= 型宣言 (typedefしておかないと Wizardでエラーに成ります)
 *		typedef CDelayedRedrawAddinT<CStatic> CDrStatic;	
 *
 *		//= ダイアログクラス宣言
 *	 	CDrStatic	m_text;		// <- 元々 CStaticだったメンバを書き換えた		
 *
 *		\endcode
 *		
 *	@par必要ファイル
 *			TnbMfcDelayedRedraw.h
 * 
 *	@date 10/02/04 新規作成
 *	@date 10/11/01 CWnd::RedrawWindow() のフラグ、変更（CE対策）。
 */
template<typename TYP = CStatic>
class CDelayedRedrawAddinT : public CSetRedrawExAddinT<TYP>
{
	DEFSUPER(CSetRedrawExAddinT<TYP>);
public:

	/// コンストラクタ
	CDelayedRedrawAddinT(void) : m_postDrawTiming(200), m_lastDrawInterval(100)
	{
	}

	/**
	 * [設定] タイミング設定.
	 *	@param postDrawTiming 内容更新が続いている間、再描画するタイミング(ms)。(def.200)
	 *	@param lastDrawInterval 内容更新がひと段落したと判断する時間(ms)。(def.100)
	 */
	void SetTiming(DWORD postDrawTiming, DWORD lastDrawInterval)
	{
		m_postDrawTiming = postDrawTiming;
		m_lastDrawInterval = lastDrawInterval;
	}
	
	/**
	 * [設定] 遅延再描画指定.
	 *		内容更新時、その前にこのメソッドをコールして置きます。
	 */
	void DelayedRedraw(void)
	{
		if ( _super::GetRedraw() )
		{
			_super::SetRedraw(FALSE);
		}
		_super::SetTimer(TIMERID_LAST, m_lastDrawInterval, NULL);
		if ( ! m_isOnRedrawTimer )
		{
			m_isOnRedrawTimer = true;
			_super::SetTimer(TIMERID_POST, m_postDrawTiming, NULL);
		}
	}

	/**
	 * [設定] ウィンドウタイトル設定.
	 *	@param lpszString 文字列.
	 */
	void SetWindowText(LPCTSTR lpszString)
	{
		DelayedRedraw();
		_super::SetWindowText(lpszString);
	}

	/**
	 * [処理] 再描画.
	 *	@note 遅延描画せず、すぐに描画指示します。
	 */
	void UpdateWindow(void)
	{
		m_isOnRedrawTimer = false;
		_super::KillTimer(TIMERID_POST);
		_super::KillTimer(TIMERID_LAST);
		_super::SetRedraw(TRUE);
		_super::Invalidate(FALSE);
		_super::UpdateWindow();
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_isOnRedrawTimer = false;
		_super::PreSubclassWindow();
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
		if ( message == WM_TIMER )
		{
			if ( wParam == TIMERID_POST )
			{
				_super::KillTimer(TIMERID_POST);
				m_isOnRedrawTimer = false;
				_super::SetRedraw(TRUE);
				_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				_super::SetRedraw(FALSE);
				_super::SetTimer(TIMERID_LAST, m_lastDrawInterval, NULL);
			}
			else if ( wParam == TIMERID_LAST )
			{
				// 更新無くなって 100msたった
				_super::KillTimer(TIMERID_LAST);
				_super::SetRedraw(TRUE);
				_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	enum
	{
		TIMERID_POST = 20001,		///< 時々描画タイマー
		TIMERID_LAST = 20002,		///< 最終描画タイマー
	};
	bool	m_isOnRedrawTimer;		///< 再描画タイマーON？
	DWORD	m_postDrawTiming;		///< 時々描画するタイミング
	DWORD	m_lastDrawInterval;		///< 最終描画時間
};



}; //MFC
}; //TNB



