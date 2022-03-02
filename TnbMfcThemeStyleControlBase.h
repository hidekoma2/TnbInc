#pragma once
/**
 *@file
 * テーマスタイルパーツ描画コントロールベース関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbThemeStylePartsDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {




/**@ingroup MFCCONTROL
 * テーマスタイルパーツ描画コントロールクラス
 *
 *		テーマスタイルパーツ描画するコントロールの作成をサポートする機能を持っています。
 *
 *	@par必要ファイル
 *			TnbMfcThemeStyleControlBase.h
 * 
 *	@date 08/02/04	新規作成
 */
template<typename TYP>
class CThemeStyleControlBaseT : public TYP
{
protected:
	CThemeStylePartsDrawer	m_partsDrawer;		///< テーマパーツ描画

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
		if ( message == 0x031A/*WM_THEMECHANGED*/ )
		{
			m_partsDrawer.ThemeChanged();
		}
		return TYP::WindowProc(message, wParam, lParam);
	}
public:
	/// コンストラクタ
	CThemeStyleControlBaseT(void)
	{
	}
};



}; // MFC
}; // TNB
