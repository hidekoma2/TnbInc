#pragma once
/**
 *@file
 * ウィンドウズ表示関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDynamicFunc.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * ウィンドウズ表示管理
 *
 *		API の PrintWindow を再現します。 XP以外でも WM_PRINT を使い、互換で動きます。
 *	
 *	@par必要ファイル
 *			TnbWindowPrinter.h
 *
 *	@date 08/01/22 新規作成。
 */
class CWindowPrinter
{
	bool m_hasPrintWindow;	///< フラグ
public:
	/// コンストラクタ
	CWindowPrinter(void) : m_hasPrintWindow(true)
	{
	}

	/**
	 * [描画] ウィンドウ描画.
	 *	@param hWnd 対象ウィンドウ
	 *	@param hdc デバイスコンテキスト.
	 *	@param isClientOnly true ならクライアントのみ。 false なら window。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool PrintWindow(HWND hWnd, HDC hdc, bool isClientOnly = true)
	{
		bool r = false;
		if ( m_hasPrintWindow )
		{
			try
			{
				typedef BOOL (WINAPI* P_PrintWindow)(HWND, HDC, UINT);
				CDynamicFuncT<P_PrintWindow> pw("USER32.DLL", "PrintWindow");
				r = !! pw()(hWnd, hdc, isClientOnly ? 1/*PW_CLIENTONLY*/ : 0);
			}
			catch ( CNullPointerException& e )
			{
				e.OnCatch();
				m_hasPrintWindow = false;
			}
		}
		if ( ! m_hasPrintWindow )
		{
			UINT message = isClientOnly ? WM_PRINTCLIENT : WM_PRINT;
			WPARAM wParam = reinterpret_cast<WPARAM>(hdc);
			LPARAM lParam = PRF_CLIENT | PRF_NONCLIENT | PRF_OWNED | PRF_CHILDREN;
			r = !! ::SendMessage(hWnd, message, wParam, lParam);
		}
		return r;
	}
};



};//TNB
