#pragma once
/**
 *@file
 * 拡張 SetRedraw 関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 拡張 SetRedraw サポートクラス
 *
 *		WM_SETREDRAW メッセージをカウンタ制にするアドインです。
 *
 *		GetRedraw() , SetRedrawAbsolutely() が追加されます。
 *
 *	@note WM_SETREDRAW が拡張されます。
 *				- lParam == SET_REDRAW_COOKIE && wParam == 100 なら 描画しないカウンタを 0 に設定。
 *				- lParam == SET_REDRAW_COOKIE && wParam == 101 なら 描画しないカウンタ取得。
 *				- wParam == 0 なら 描画しないカウンタを+1。
 *				- wParam == 1 なら 描画しないカウンタを -1。
 *
 *	@par必要ファイル
 *			TnbMfcSetRedrawEx.h
 * 
 *	@date 10/06/01	新規作成
 */
template<typename TYP>
class CSetRedrawExAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// シンボル定義
	enum
	{
		SET_REDRAW_COOKIE = 'tsRx',		///< クッキー
	};

	/**
	 * [設定] 再描画許可・禁止設定.
	 *	@note これをアドオンすると、カウンタ式になります。つまり、禁止を３回行ったら、
	 *		許可を３階しないと、再描画されるようになりません。
	 *	@param bRedraw TURE なら再描画を許可します。 FALSE なら禁止します。
	 */
	void SetRedraw(BOOL bRedraw = TRUE)
	{
		_super::SetRedraw(bRedraw);
	}

	/**
	 * [設定] 再描画許可.
	 *	@note カウンタを初期化し、かならず再描画されるようにします。
	 */
	void SetRedrawAbsolutely(void)
	{
		ASSERT( ::IsWindow(_super::GetSafeHwnd()) );
		::SendMessage(_super::GetSafeHwnd(), WM_SETREDRAW, 100, SET_REDRAW_COOKIE);
	}

	/**
	 * [取得] 再描画状態取得.
	 *	@retval TRUE 再描画許可状態。
	 *	@retval FALSE 再描画禁止状態。
	 */
	BOOL GetRedraw(void) const
	{
		LRESULT r = ::SendMessage(_super::GetSafeHwnd(), WM_SETREDRAW, 101, SET_REDRAW_COOKIE);
		return r == 0;
	}

protected:

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *		ListBox 関係のメッセージをフックしています。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == WM_SETREDRAW )
		{
			if ( lParam == SET_REDRAW_COOKIE )
			{
				if ( wParam == 100 )
				{
					// 描画しないカウンタ = 0。
					m_count = 0;
					return _super::WindowProc(message, TRUE, 0);
				}
				else if ( wParam == 101 )
				{
					// 描画しないカウンタ取得。
					return m_count;
				}
				else
				{
					return 0;
				}
			}
			if ( wParam == 0 )
			{
				// 描画しないカウンタ +1
				m_count++;
				if ( m_count == 1 )
				{
					return _super::WindowProc(message, FALSE, 0);
				}
				return 0;
			}
			else
			{
				// 描画しないカウンタ -1
				if ( m_count > 0 )
				{
					m_count--;
					if ( m_count == 0 )
					{
						return _super::WindowProc(message, TRUE, 0);
					}
				}
				return 0;
			}
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
		m_count = 0;
		_super::PreSubclassWindow();
	}
	/**
	 * [通知] special pre-creation and window rect adjustment hooks.
	 *		Create時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param cs 構築データ.
	 *	@return 結果.
	 */
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
	{
		m_count = 0;
		return _super::PreCreateWindow(cs);
	}
	/**
	 * [通知] 破棄終了通知
	 */
	virtual void PostNcDestroy(void) 
	{
		m_count = 0;
		_super::PostNcDestroy();
	}

private:
	int			m_count;	///< 描画しないカウンタ。 0 なら描画許可する 1 以上なら描画停止
};



}; // MFC
}; // TNB
