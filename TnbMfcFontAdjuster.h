#pragma once
/**
 *@file
 * フォント調節関係のヘッダ
 *
 *		コントロールの高さに追従してフォントの大きさを調節させるための機能を提供します。
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
 * フォント調節クラス
 *
 *		inline なコントロールです。
 *
 *		本クラスを使用すると、指定のコントロールの高さによりフォントの大きさを変化させることが出来ます。
 *
 *	@param TYP 要素の型。CWnd のサブクラスである必要があります。
 *
 *	@par使用例
 *		\code
 *		//= 型宣言 (typedefしておかないと Wizardでエラーに成ります)
 *		typedef CFontAdjusterAddinT<CStatic> CFaStatic;	
 *
 *		//= ダイアログクラス宣言
 *	 	CFaStatic	m_text;		// <- 元々 CStaticだったメンバを書き換えた		
 *
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbMfcFontAdjuster.h
 * 
 *	@date 09/09/25 新規作成
 *	@date 09/10/22 フォント指定、カラー指定メソッド追加。
 */
template<typename TYP = CStatic>
class CFontAdjusterAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// コンストラクタ
	CFontAdjusterAddinT(void) : m_isSafeFontSetting(false), m_textColor(CLR_INVALID)
	{
	}

	/**
	 * [設定] フォントサイズリセット.
	 *		設定されたフォントサイズに戻します。
	 *	@note 今後、今のウィンドウサイズとフォントサイズを基準に、リサイズ時フォントを調節します。
	 */
	void ResetFontSize(void)
	{
		m_height = m_GetClientHeight();
		m_Adjust();
	}

	/**
	 * [設定] 文字色指定.
	 *	@param c 色。 CLR_INVALID を指定すると、デフォルトになります。
	 */
	void SetTextColor(COLORREF c = CLR_INVALID)
	{
		m_textColor = c;
	}
	
	/**
	 * [設定] 背景色指定.
	 *	@param c 色。 CLR_INVALID を指定すると、デフォルトになります。
	 */
	void SetBackColor(COLORREF c = CLR_INVALID)
	{
		if ( m_backBrush.GetSafeHandle() != NULL )
		{
			m_backBrush.DeleteObject();
		}
		if ( IS_RGBVALUE(c) )
		{
			m_backBrush.CreateSolidBrush(c);
		}
	}

	/**
	 * [設定] フォント属性設定.
	 *	@param logFont フォント属性
	 */
	void SetLogFont(const LOGFONT& logFont)
	{
		m_logFont = logFont;
		m_Adjust();
	}

	/**
	 * [取得] フォント属性取得.
	 *	@param[out] _logFont フォント属性が格納される。
	 */
	void GetLogFont(LOGFONT& _logFont) const
	{
		_logFont = m_logFont
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
		case WM_SETFONT:
			if ( ! m_isSafeFontSetting )
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_Check();
				return r;
			}
			break;
		case WM_SIZE:
			m_Adjust();
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] for notifications from parent
	 *	@note
	 *		CWndのメソッドをオーバーライドしています。
	 *		メッセージ受信したらコールされます。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE	処理済。
	 *	@retval FALSE	未処理。
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		switch ( message )
		{
		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			if ( IS_RGBVALUE(m_textColor) )
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				::SetBkMode(dc, TRANSPARENT);
				::SetTextColor(dc, m_textColor);
				if ( m_backBrush.GetSafeHandle() == NULL )
				{
					int f = COLOR_3DFACE;
					if ( message == WM_CTLCOLOREDIT )
					{
						f = COLOR_WINDOW;
					}
					m_backBrush.CreateSysColorBrush(f);
				}
				*_pResult = reinterpret_cast<LRESULT>(m_backBrush.GetSafeHandle());
				return true;
			}
			break;
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		m_Check();
	}

private:
	// クライアント高さ取得
	int m_GetClientHeight(void)
	{
		CRect rc;
		_super::GetClientRect(rc);
		return rc.Height();
	}
	// 現在のサイズとフォントを確認
	void m_Check(void)
	{
		m_height = m_GetClientHeight();
		Zero(m_logFont);
		CFont* pFont = _super::GetFont();
		if ( pFont != NULL )
		{
			pFont->GetLogFont(&m_logFont);
		}
		m_Adjust();
	}
	// 調整
	void m_Adjust(void)
	{
		m_isSafeFontSetting = true;
		int height = m_GetClientHeight();
		{
			_super::SetFont(NULL);
			m_font.DeleteObject();
		}
		LOGFONT	lf = m_logFont;
		lf.lfHeight = m_logFont.lfHeight * height / m_height;
		m_font.CreateFontIndirect(&lf);
		_super::SetFont(&m_font);
		m_isSafeFontSetting = false;
	}
	LOGFONT		m_logFont;				///< オリジナルフォント
	int			m_height;				///< オリジナル高さ
	CFont		m_font;					///< 使用フォント
	bool		m_isSafeFontSetting;	///< 自己フォント設定中
	COLORREF	m_textColor;			///< 文字カラー
	CBrush		m_backBrush;			///< 背景ブラシ
};



}; //MFC
}; //TNB



