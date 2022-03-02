#pragma once
/**
 *@file
 * 簡単なカスタムウィンドウ設定関係
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCustomWindowFrameMaster.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB 
{



/**@ingroup WINAPI
 * 簡単なカスタムウィンドウフレームクラス.
 *
 *		よく使われる設定をまとめただけのクラスです。
 *
 *	@par必要ファイル
 *			TnbMfcEasyCustomWindowFrame.h
 *
 *	@date 10/05/20 新規作成
 */
class CEasyCustomWindowFrame
{
public:

	/**
	 * コンストラクタ
	 *	@param isDisableVista true を指定すると、 VISTA以降の場合、フックしません。
	 *						false を指定すると、 VISTA以降でも、フックします。
	 */
	CEasyCustomWindowFrame(bool isDisableVista = false)
		: m_window(::GetSysColor(COLOR_BTNTEXT), ::GetSysColor(COLOR_BTNFACE))
		, m_activeCaption(::GetSysColor(COLOR_CAPTIONTEXT), ::GetSysColor(COLOR_ACTIVECAPTION))
		, m_inactiveCaption(::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTION))
		, m_baseColor(RGB(255, 255, 255)), m_isDisableVista(isDisableVista)
	{
	}

	/**
	 * [設定] ウィンドウ色設定.
	 *	@note  Hook() で指定するフレーム画像のベース色( SetBaseColor() で指定した色) を背景色に置き換えます。
	 *	@param text 文字色
	 *	@param back 背景色
	 */
	void SetWindowColor(COLORREF text, COLORREF back)
	{
		m_window.text = text;
		m_window.back = back;
	}

	/**
	 * [設定] ベース色設定.
	 *	@param c ベース色
	 */
	void SetBaseColor(COLORREF c)
	{
		m_baseColor = c;
	}

	/**
	 * [設定] キャプション色設定.
	 *	@param text 文字色
	 *	@param back 背景色
	 *	@param isActive true ならアクティブのキャプション、 false なら非アクティブのキャプションの設定を行う。
	 */
	void SetCaptionColor(COLORREF text, COLORREF back, bool isActive)
	{
		if ( isActive )
		{
			m_activeCaption.text = text;
			m_activeCaption.back = back;
		}
		else
		{
			m_inactiveCaption.text = text;
			m_inactiveCaption.back = back;
		}
	}

	/**
	 * [設定] カスタムウィンドウフレーム.
	 *	
	 *
	 *	@param frame フレーム。色を変えたいところを白(255, 255, 255)で描画。
	 *	@param button システムボタン。赤をベースで描画。RGBの操作で青と黄色を作成します。
	 *	@param num システムボタンの画像数。
	 */
	void Hook(CBitmapHandle frame, CBitmapHandle button, int num = 4)
	{
		CCustomWindowFrameMaster cwfm;
		CBitmapImage bi;
		CWindowCaptionDrawer ttd(NULL);
		ttd.SetBarColor(m_activeCaption.back, m_inactiveCaption.back);
		ttd.SetTextColor(m_activeCaption.text, m_inactiveCaption.text);
		cwfm.SetTitleBitmap(ttd);
		bi.Set(frame);
		bi.ChangePixelColor(m_baseColor, m_window.back);
		cwfm.SetFrameBitmap(bi);
		cwfm.SetFrameTransColor(CLR_AUTOSELECT);
		bi.Set(button);
		bi.ChangePixelColor(RGB(0, 0, 0), m_window.back);
		cwfm.SetCloseButtonBitmap(CPoint(CalcScaleX(-6), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		bi.Set(button);
		bi.ChangePixelColor(RGB(255, 0, 0), RGB(230, 0, 0), RGB(0, 255, 0));
		bi.ChangePixelColor(RGB(0, 0, 0), m_window.back);
		cwfm.SetMaximumButtonBitmap(CPoint(CalcScaleX(-23), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		cwfm.SetRestoreButtonBitmap(CPoint(CalcScaleX(-23), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		bi.Set(button);
		bi.ChangePixelColor(RGB(0, 0, 255), RGB(0, 255, 0), RGB(255, 0, 0));
		bi.ChangePixelColor(RGB(0, 0, 0), m_window.back);
		cwfm.SetMinimumButtonBitmap(CPoint(CalcScaleX(-40), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		cwfm.SetTextColor(m_window.text);
		cwfm.Hook(m_isDisableVista);
	}

private:

	/// ペア情報
	struct TPair
	{
		COLORREF	text;	///< 文字色
		COLORREF	back;	///< 背景色
		/// コンストラクタ
		TPair(COLORREF t, COLORREF b) : text(t), back(b)
		{
		}
	};
	COLORREF	m_baseColor;		///< ベース色
	TPair		m_window;			///< ウィンドウ色
	TPair		m_activeCaption;	///< アクティブキャプション色
	TPair		m_inactiveCaption;	///< 非アクティブキャプション色
	bool		m_isDisableVista;	///< VISTA の場合無効
};



}; // TNB
