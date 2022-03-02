#pragma once
/**
 *@file
 * DebugString ウィンドウ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcTinyListDialog.h"
#include "TnbApiHooker.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document作成用シンボル
namespace DSD {
	typedef VOID (WINAPI* P_OutputDebugStringA)(LPCSTR lpOutputString);
	typedef VOID (WINAPI* P_OutputDebugStringW)(LPCWSTR lpOutputString);
	_SELECTANY CApiHookerT<P_OutputDebugStringA>* s_pHookerA = NULL;
	_SELECTANY CApiHookerT<P_OutputDebugStringW>* s_pHookerW = NULL;
	_SELECTANY CTinyListDialog* s_pListDialog = NULL;
};
#endif



/**@ingroup MFCCONTROL
 * DebugString ウィンドウ
 *
 *		開発支援用です。
 *		API の OutputDebugString() をフックし、画面上に表示することが出来るモードレスダイアログです。
 *
 *	@par使用例
 *			\code
 *				// CDebugStringDialog m_dsd;
 *				void CFooDlg::OnIniDialog(void)
 *				{
 *						;
 *					if ( ! ::IsDebuggerPresent() )
 *					{
 *						m_dsd.Create(CSize(400, 400), GetSafeHwnd(), _T("OutputDebugString"), m_hIcon);
 *					}
 *						;
 *				}
 *			\endcode
 *
 *	@note リンクしている dll 内の OutputDebugString() はフックしません。
 *
 *	@par必要ファイル
 *			TnbMfcDebugStringDialog.h
 * 
 *	@date 12/01/27	新規作成
 */
class CDebugStringDialog : public CTinyListDialog
{
	DEFSUPER(CTinyListDialog);
public:

	/// コンストラクタ
	CDebugStringDialog(void) : _super(false)
	{
	}

protected:

	/**
	 * [通知] 作成完了通知.
	 *	@note ウィンドウ作成完了後に通知されます。
	 */
	virtual void OnCreate(void)
	{
		if ( DSD::s_pListDialog == this || DSD::s_pListDialog != NULL )
		{
			return;
		}
		DSD::s_pHookerA = new CApiHookerT<DSD::P_OutputDebugStringA>;
		if ( DSD::s_pHookerA->Hook(NULL, "OutputDebugStringA", ms_MyOutputDebugStringA) )
		{
			DSD::s_pHookerW = new CApiHookerT<DSD::P_OutputDebugStringW>;
			if ( DSD::s_pHookerW->Hook(NULL, "OutputDebugStringW", ms_MyOutputDebugStringW) )
			{
				DSD::s_pListDialog = this;
				return;
			}
		}
		OnDestroy();
	}

	/**
	 * [通知] 破棄通知.
	 *	@note ウィンドウ破棄直前に通知されます。
	 */
	virtual void OnDestroy(void)
	{
		DSD::s_pListDialog = NULL;
		if ( DSD::s_pHookerA != NULL )
		{
			DSD::s_pHookerA->Unhook();
			delete DSD::s_pHookerA;
			DSD::s_pHookerA = NULL;
		}
		if ( DSD::s_pHookerW != NULL )
		{
			DSD::s_pHookerW->Unhook();
			delete DSD::s_pHookerW;
			DSD::s_pHookerW = NULL;
		}
	}

private:

	/**
	 * [処理] メッセージポンプ
	 */
	static void ms_PumpMessage(void)
	{
		HWND h = *DSD::s_pListDialog;
		MSG msg;
		while ( ::PeekMessage(&msg, h, NULL, NULL, PM_NOREMOVE) )
		{
			if ( ::GetMessage(&msg, h, NULL, NULL) )
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

	/**
	 * [通知] OutputDebugStringA 実行時
	 *		OutputDebugStringA() 実行時にコールされます。
	 *	@param lpszOutputString 文字列
	 */
	static VOID WINAPI ms_MyOutputDebugStringA(LPCSTR lpszOutputString)
	{
		if ( DSD::s_pListDialog != NULL )
		{
			DSD::s_pListDialog->PostString(CStr(lpszOutputString));
			ms_PumpMessage();
		}
		(*DSD::s_pHookerA)()(lpszOutputString);
	}

	/**
	 * [通知] OutputDebugStringW 実行時
	 *		OutputDebugStringW() 実行時にコールされます。
	 *	@param lpszOutputString 文字列
	 */
	static VOID WINAPI ms_MyOutputDebugStringW(LPCWSTR lpszOutputString)
	{
		if ( DSD::s_pListDialog != NULL )
		{
			DSD::s_pListDialog->PostString(CStr(lpszOutputString));
			ms_PumpMessage();
		}
		(*DSD::s_pHookerW)()(lpszOutputString);
	}
};



}; // MFC
}; // TNB

