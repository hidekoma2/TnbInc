#pragma once
/**
 *@file
 * CE タップ＆ホールドジェスチャー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeMfcGesture.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL WINCE 
 * タップ＆ホールドジェスチャー禁止アドイン(CE専用).
 *	
 *		CE ではタップしてホールドすると、くるくるでてきます。
 *		これを制限する(でないようにする)アドインです。
 *	
 *	@par必要ファイル
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	新規作成
 */
template<typename WND>
class CCeDisableGestureAddinT : public WND
{
	DEFSUPER(WND);
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
		if ( message == WM_LBUTTONDOWN )
		{
			return _super::DefWindowProc(message, wParam, lParam);
		}
		return _super::WindowProc(message, wParam, lParam);
	}
};



/**@ingroup WINCE 
 * タップ＆ホールドジェスチャー禁止 Dialog (CE専用).
 *	
 *		CE ではタップしてホールドすると、くるくるでてきます。
 *		これを制限した(でないようにした) CDialog です。
 *	
 *	@par必要ファイル
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	新規作成
 */
class CDialogDg : public CDialog
{
	DEFSUPER(CDialog);
public:

	/**
	 * コンストラクタ
	 *	@param lpszTemplateName ダイアログ ボックス テンプレート リソース名を表す null で終わる文字列。
	 *	@param pParentWnd ダイアログ オブジェクトを所有する CWnd 型の親ウィンドウ
	 *					またはオーナー ウィンドウオブジェクトへのポインタ。
	 *					NULL の場合は、ダイアログ オブジェクトの親ウィンドウがアプリケーションのメイン ウィンドウになります。
	 */
	explicit CDialogDg(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL)
		: CDialog(lpszTemplateName, pParentWnd)
	{
	}

	/**
	 * コンストラクタ
	 *	@param nIDTemplate ダイアログ ボックス テンプレート リソースの ID 番号。
	 *	@param pParentWnd ダイアログ オブジェクトを所有する CWnd 型の親ウィンドウ
	 *					またはオーナー ウィンドウオブジェクトへのポインタ。
	 *					NULL の場合は、ダイアログ オブジェクトの親ウィンドウがアプリケーションのメイン ウィンドウになります。
	 */
	explicit CDialogDg(UINT nIDTemplate, CWnd* pParentWnd = NULL)
		: CDialog(nIDTemplate, pParentWnd)
	{
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
		if ( message == WM_LBUTTONDOWN )
		{
			return _super::DefWindowProc(message, wParam, lParam);
		}
		return _super::WindowProc(message, wParam, lParam);
	}
};



/**@ingroup WINCE 
 * タップ＆ホールドジェスチャー禁止 Edit (CE専用).
 *	
 *		CE ではタップしてホールドすると、くるくるでてきます。
 *		これを制限した(でないようにした) CEdit です。
 *	
 *	@par必要ファイル
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	新規作成
 */
typedef CCeDisableGestureAddinT<CEdit> CEditDg;



/**@ingroup WINCE 
 * タップ＆ホールドジェスチャー禁止 ComboBox (CE専用).
 *	
 *		CE ではタップしてホールドすると、くるくるでてきます。
 *		これを制限した(でないようにした) CComboBox です。
 *	
 *	@par必要ファイル
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	新規作成
 */
typedef CCeDisableGestureAddinT<CComboBox> CComboBoxDg;



/**@ingroup WINCE 
 * タップ＆ホールドジェスチャー禁止 SpinButtonCtrl (CE専用).
 *	
 *		CE ではタップしてホールドすると、くるくるでてきます。
 *		これを制限した(でないようにした) CSpinButtonCtrl です。
 *	
 *	@par必要ファイル
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	新規作成
 */
typedef CCeDisableGestureAddinT<CSpinButtonCtrl> CSpinButtonCtrlDg;



}; //MFC
}; //TNB
