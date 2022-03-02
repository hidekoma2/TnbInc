#pragma once
/**
 *@file
 * カスタムウィンドウフレームマスターのヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCustomWindowFrame.h"
#include "TnbWindowsVersion.h"
#include "TnbSimpleMap.h"
#include "TnbBitFlag.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document作成用シンボル
_SELECTANY class CCustomWindowFrameMaster* g_pCwfmMaster = NULL;	///< マスターインスタンス
#endif



/**@ingroup DRAWABLE MFCCONTROL
 * カスタムウィンドウフレームマスタークラス.
 *	
 *		ダイアログの NonClient 部分を変更できます。また、透過色を指定し、矩形以外のフレームにも対応します。
 *
 *	@note プロセスをローカルフックしているので、プロセスで表示される全ウィンドウの NonClient 部分を変更できます。
 *
 *	@par使用例
 *		\code
 *
 *		BOOL CFooApp::InitInstance()
 *		{
 *			CCustomWindowFrameMaster frame;
 *				;
 *			// フレームの設定
 *			CWindowCaptionDrawer ttd(NULL);
 *			ttd.SetBarColor(RGB(255, 0, 0), RGB(0, 0, 0));
 *			frame.SetTitleBitmap(ttd);
 *			frame.SetFrameBitmap(IDB_FRAME);
 *			frame.SetCloseButtonBitmap(CPoint(-6, 5), IDB_CLOSE, 4);		
 *				;
 *			// フックする。
 *			frame.Hook();
 *				;
 *		\endcode
 *
 *	@par必要ファイル
 *		TnbMfcCustomWindowFrameMaster.h
 *
 *	@date 08/10/06 新規作成
 *	@date 09/04/22 マップを CSimpleMap に変更
 *	@date 09/05/12 VISTA の場合、Hookしないように変更。
 *	@date 10/04/26 Hook のためのインスタンスは保持しておく必要をなくした。
 */
class CCustomWindowFrameMaster : public CCustomWindowFrame
{
	DEFSUPER(CCustomWindowFrame);
public:

	/// コンストラクタ
	CCustomWindowFrameMaster(void) : m_pCwfmOldFrame(NULL)
	{
	}

	/// デストラクタ
	~CCustomWindowFrameMaster(void)
	{
		loop ( i, m_cwfmMap.GetSize() )
		{
			delete m_cwfmMap.Ref(i).val;
		}
		if ( m_pCwfmOldFrame != NULL )
		{
			delete m_pCwfmOldFrame;
			m_pCwfmOldFrame = NULL;
		}
	}

	/**
	 * [設定] フック.
	 *		プロセスで一つ、フック出来ます。本インスタンス破棄後も、フックは有効です。
	 *	@param isDisableVista true を指定すると、 VISTA以降の場合、フックしません。
	 *						false を指定すると、 VISTA以降でも、フックします。
	 */
	void Hook(bool isDisableVista = false)
	{
		CWindowsVersion wv;
		if ( ! isDisableVista || wv.GetOsVersion() < CWindowsVersion::WINVISTA )
		{
			if ( g_pCwfmMaster == NULL )
			{
				static CReject s_cRejeCWFM;
				g_pCwfmMaster = new CCustomWindowFrameMaster();
				g_pCwfmMaster->CopyParameter(*this);
				g_pCwfmMaster->m_hCwfmCbtHook = ::SetWindowsHookEx(WH_CBT, ms_HookCbtProc, NULL, ::GetCurrentThreadId());
			}
		}
	}

	/**
	 * [設定] アンフック.
	 *		Hook() したものを解除します。
	 */
	static void Unhook(void)
	{
		if ( g_pCwfmMaster != NULL )
		{
			delete g_pCwfmMaster;
			g_pCwfmMaster = NULL;
		}
	}

private:
	//Unhook処理用サブクラス
	class CReject
	{
	public:	
		/// デストラクタ
		~CReject(void)
		{
			CCustomWindowFrameMaster::Unhook();
		}
	};

	static LRESULT CALLBACK ms_HookCbtProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		WINDOWINFO wi = { sizeof(WINDOWINFO) };
		HWND hWnd = reinterpret_cast<HWND>(wParam);
		::GetWindowInfo(hWnd, &wi);
		LONG style = wi.dwStyle;
//			LONG styleEx = wi.dwExStyle;
		bool canAttach = true;
//			bool isMenu = false;
		canAttach &= ((style & (WS_DLGFRAME)) == WS_DLGFRAME);
		if ( wi.atomWindowType == 0x8002 ) { canAttach = true; }
		if ( wi.atomWindowType == 0xC23B ) { canAttach = true; }
		if ( wi.atomWindowType == 0xC16A ) { canAttach = true; }
#if 0
		if ( wi.atomWindowType == 0xC037 && styleEx== 0x000000A8 )
		{
			// メニュー？
			canAttach = true; 
			isMenu = true;
			if ( nCode == HCBT_CREATEWND )
				TTRACE0("↓メニュー!\n");
		}
		if ( wi.atomWindowType == 0x8000 && styleEx== 0x181 )
		{
			// メニュー？
			canAttach = true; 
			isMenu = true;
			if ( nCode == HCBT_CREATEWND )
				TTRACE0("↓メニュー!\n");
		}
#endif

		#ifdef _DEBUG
			if ( nCode == HCBT_CREATEWND )
			{
				CWindowStyleBitFlag ff = wi.dwStyle;
				TTRACE3("HCBT_CREATEWND : hWnd = 0x%08X, atom=%04X, ex=0x%08X\n", hWnd, wi.atomWindowType, wi.dwExStyle);
				TTRACE2("  style = 0x%08X \n", wi.dwStyle, ff.ToString());
				if ( ! canAttach )
				{
					TTRACE1("  -> Not Attach [0x%08X]\n", hWnd);
				}
			}
		#endif
		if ( canAttach )
		{
			switch ( nCode )
			{
			case HCBT_CREATEWND:
				if ( ! g_pCwfmMaster->m_cwfmMap.HasKey(hWnd) )
				{
					//TTRACE1("Create[0x%08X] atom=%04X\n", hWnd, wi.atomWindowType);
					CCustomWindowFrame* P = new CCustomWindowFrame;
					g_pCwfmMaster->m_cwfmMap[hWnd] = P;
					P->CopyParameter(*g_pCwfmMaster);
					CBT_CREATEWND* pCBT = reinterpret_cast<CBT_CREATEWND*>(lParam);
					if ( pCBT->lpcs->hMenu == NULL || wi.atomWindowType == 0xC256 )
					{
						P->Attach(hWnd);
						//::SendMessage(hWnd, WM_ACTIVATE, 1, 0);
					}
				}
				break;
			case HCBT_MOVESIZE:
			case HCBT_ACTIVATE:
				if ( g_pCwfmMaster->m_cwfmMap.HasKey(hWnd) )
				{
					CCustomWindowFrame* P = g_pCwfmMaster->m_cwfmMap[hWnd];
					if ( P->GetSafeHwnd() == NULL )
					{
						P->Attach(hWnd);
						::PostMessage(hWnd, WM_ACTIVATE, 1, 0);
					}
				}
				break;
			case HCBT_DESTROYWND:
				if ( g_pCwfmMaster->m_pCwfmOldFrame != NULL )
				{
					delete g_pCwfmMaster->m_pCwfmOldFrame;
					g_pCwfmMaster->m_pCwfmOldFrame = NULL;
				}
				if ( g_pCwfmMaster->m_cwfmMap.HasKey(hWnd) )
				{
					CCustomWindowFrame* P = g_pCwfmMaster->m_cwfmMap[hWnd];
					g_pCwfmMaster->m_pCwfmOldFrame = P;
					g_pCwfmMaster->m_cwfmMap.RemoveKey(hWnd);
				}
				break;
			}
		}
		// 次のフックに渡す
		return ::CallNextHookEx(g_pCwfmMaster->m_hCwfmCbtHook, nCode, wParam, lParam);
	}

	HHOOK										m_hCwfmCbtHook;		///< CBTフックハンドル
	CSimpleMapT<HWND, CCustomWindowFrame*>		m_cwfmMap;			///< 管理マップ
	CCustomWindowFrame*							m_pCwfmOldFrame;	///< 
};



}; //MFC
}; //TNB
