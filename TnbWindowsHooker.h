#pragma once
/**
 *@file
 * ウィンドウズフック関係のヘッダ
 *
 *		シンボル "TnbGLOBALHOOK" を定義し、本クラスをDLL内で使用すると、グローバルフックも可能です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#if ( _WIN32_WINNT < 0x0400 )

	#define WH_KEYBOARD_LL		13
	#define WH_MOUSE_LL			14

	/*
	 * Low level hook flags
	 */
	#define LLKHF_EXTENDED			(KF_EXTENDED >> 8)
	#define LLKHF_INJECTED			0x00000010
	#define LLKHF_ALTDOWN			(KF_ALTDOWN >> 8)
	#define LLKHF_UP				(KF_UP >> 8)
	#define LLMHF_INJECTED			0x00000001

	/*
	 * Structure used by WH_KEYBOARD_LL
	 */
	typedef struct tagKBDLLHOOKSTRUCT {
		DWORD	vkCode;
		DWORD	scanCode;
		DWORD	flags;
		DWORD	time;
		ULONG	dwExtraInfo;
	} KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

	/*
	 * Structure used by WH_MOUSE_LL
	 */
	typedef struct tagMSLLHOOKSTRUCT {
		POINT	pt;
		DWORD	mouseData;
		DWORD	flags;
		DWORD	time;
		ULONG	dwExtraInfo;
	} MSLLHOOKSTRUCT, FAR *LPMSLLHOOKSTRUCT, *PMSLLHOOKSTRUCT;

#endif // (_WIN32_WINNT < 0x0400)

#ifndef WH_HARDWARE
	#define WH_HARDWARE			8
#endif

// ウィンドウズフック関係
namespace WinHook
{
	#ifdef TnbGLOBALHOOK
		#pragma comment(linker, "/section:.tnbwinhook,RWS")
		#pragma data_seg(".tnbwinhook")
	#endif

	// ウィンドウフック対象のインスタンスハンドル
	_SELECTANY HINSTANCE	g_hInstance		= NULL;
	// ウィンドウフック対象のスレッドID
	_SELECTANY DWORD		g_dwThreadId	= 0;

	#define _WINHOOKMAC(NAME) \
		_SELECTANY HHOOK	s_hhk##NAME = NULL;												\
		static HHOOK Hook##NAME(bool b, HOOKPROC PF) {										\
			if ( ! b && s_hhk##NAME != NULL ){												\
			  ::UnhookWindowsHookEx(s_hhk##NAME); s_hhk##NAME = NULL;						\
			}																				\
			if ( b && s_hhk##NAME == NULL ){												\
			  s_hhk##NAME = SetWindowsHookEx(WH_##NAME, PF, g_hInstance, g_dwThreadId);		\
			}																				\
			return s_hhk##NAME;																\
		}

	_WINHOOKMAC( MSGFILTER		)
	_WINHOOKMAC( JOURNALRECORD	)
	_WINHOOKMAC( JOURNALPLAYBACK)
	_WINHOOKMAC( KEYBOARD		)
	_WINHOOKMAC( GETMESSAGE		)
	_WINHOOKMAC( CALLWNDPROC	)
	_WINHOOKMAC( CBT			)
	_WINHOOKMAC( SYSMSGFILTER	)
	_WINHOOKMAC( MOUSE			)
	_WINHOOKMAC( HARDWARE		)
	_WINHOOKMAC( DEBUG			)
	_WINHOOKMAC( SHELL			)
	_WINHOOKMAC( FOREGROUNDIDLE	)
	_WINHOOKMAC( CALLWNDPROCRET	)
	_WINHOOKMAC( KEYBOARD_LL	)
	_WINHOOKMAC( MOUSE_LL		)
	#ifdef TnbGLOBALHOOK
		#pragma data_seg()
	#endif
	#undef _WINHOOKMAC
};

#endif //_TnbDOXYGEN



/**@ingroup WINAPI
 * ウィンドウズフック管理
 *
 *		クラス関数しか有りません。インスタンスを作成することは出来ません。
 *
 *		Hook() の戻り値は記憶しておき、登録した HOOKPROC 型のプロシージャ関数の最後で ::CallNextHookEx() に渡すようにしてください。
 *		\code 
 *		static HWND s_hWnd = NULL;
 *		static HHOOK s_hook = NULL;
 *		LRESULT CALLBACK s_hookProc(int code, WPARAM wParam, LPARAM lParam)
 *		{
 *			if ( code == HC_ACTION ) {
 *				const CWPSTRUCT * P = reinterpret_cast<const CWPSTRUCT *>(lParam);
 *				if ( s_hWnd == P->hwnd ) {
 *					::MessageBeep(MB_OK);
 *				}
 *			}
 *			return ::CallNextHookEx(s_hook, code, wParam, lParam);
 *		}
 *		void TpHook(HWND hWnd)
 *		{
 *			CWindowsHooker::SetEnvironment(NULL, 0);
 *			s_hook = CWindowsHooker::Hook(WH_CALLWNDPROC, s_hookProc);
 *			s_hWnd = hWnd;
 *		}
 *		\endcode 
 *
 *	@note TnbWindowsHooker.h をインクルードする前にシンボル "TnbGLOBALHOOK" を定義し、本クラスをDLL内で使用すると、
 *		グローバルフックも可能です。また、その際、共有セクション名は「.tnbwinhook」になっていますので、必要な変数は、
 *		共有してください。
 *		\code 
 *		#pragma data_seg(".tnbwinhook")
 *		static HWND s_hWnd = NULL;		// 共有セクション
 *		static HHOOK s_hook = NULL;		// 共有セクション
 *		#pragma data_seg()
 *		BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID lpReserved) 
 *		{
 *			CWindowsHooker::OnDllMain(hModule, reason);
 *			return TRUE;
 *		}
 *		LRESULT CALLBACK s_hookProc(int code, WPARAM wParam, LPARAM lParam)
 *		{
 *			if ( code == HC_ACTION ) {
 *				const CWPSTRUCT * P = reinterpret_cast<const CWPSTRUCT *>(lParam);
 *				if ( s_hWnd == P->hwnd ) {
 *					::MessageBeep(MB_OK);
 *				}
 *			}
 *			return ::CallNextHookEx(s_hook, code, wParam, lParam);
 *		}
 *		__declspec(dllexport) void TpHook(HWND hWnd)
 *		{
 *			s_hook = CWindowsHooker::Hook(WH_CALLWNDPROC, s_hookProc);
 *			s_hWnd = hWnd;
 *		}
 *		\endcode 
 *	
 *	@see http://msdn.microsoft.com/library/en-us/winui/winui/windowsuserinterface/windowing/hooks/hookreference/hookfunctions/setwindowshookex.asp
 *	@par必要ファイル
 *			TnbWindowsHooker.h
 *
 *	@date 06/06/05 新規作成。
 *	@date 06/12/18 名前変更。
 *	@date 11/11/28 pragma 、コメント追加。 OnDllMain() 追加。
 */
class CWindowsHooker
{
	typedef HHOOK (*LPHOOKFUNC)(bool, HOOKPROC);

	//Unhook処理用サブクラス
	class CReject
	{
	public:	~CReject(void);
	};

	// 指定のHookタイプの関数アドレス取得
	static LPHOOKFUNC m_GetFunc(int iHookType)
	{
		switch ( iHookType )
		{
		case WH_MSGFILTER:		return WinHook::HookMSGFILTER;
		case WH_JOURNALRECORD:	return WinHook::HookJOURNALRECORD;
		case WH_JOURNALPLAYBACK:return WinHook::HookJOURNALPLAYBACK;
		case WH_KEYBOARD:		return WinHook::HookKEYBOARD;
		case WH_GETMESSAGE:		return WinHook::HookGETMESSAGE;
		case WH_CALLWNDPROC	:	return WinHook::HookCALLWNDPROC;
		case WH_CBT:			return WinHook::HookCBT;
		case WH_SYSMSGFILTER:	return WinHook::HookSYSMSGFILTER;
		case WH_MOUSE:			return WinHook::HookMOUSE;
		case WH_HARDWARE:		return WinHook::HookHARDWARE;
		case WH_DEBUG:			return WinHook::HookDEBUG;
		case WH_SHELL:			return WinHook::HookSHELL;
		case WH_FOREGROUNDIDLE:	return WinHook::HookFOREGROUNDIDLE;
		case WH_CALLWNDPROCRET:	return WinHook::HookCALLWNDPROCRET;
		case WH_KEYBOARD_LL:	return WinHook::HookKEYBOARD_LL;
		case WH_MOUSE_LL:		return WinHook::HookMOUSE_LL;
		default:	break;
		}
		return NULL;
	}

	/**
	 * コンストラクタ.
	 *	@note インスタンス不必要なので、コンストラクタはprivateになっています。
	 */
	CWindowsHooker(void);

public:

	/**
	 * [設定] DllMain 時の処理
	 *		DLL のメイン関数がコールされた時にコールする関数です。 reason によって、処理を行います。
	 *	@note DLL_PROCESS_ATTACH 時には、モジュールのハンドルを記憶します。
	 *			DLL_PROCESS_DETACH 時には、すべてのフックを解除します。
	 *	@param hModule DllMain の第一引数を渡します。
	 *	@param reason DllMain の第二引数を渡します。
	 */
	static void OnDllMain(HANDLE hModule, DWORD reason)
	{
		switch ( reason )
		{
		case DLL_PROCESS_ATTACH:
			WinHook::g_hInstance = static_cast<HINSTANCE>(hModule);
			WinHook::g_dwThreadId = 0;
			break;
		case DLL_PROCESS_DETACH:
			UnhookAll();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		default:
			break;
		}
	}

	/**
	 * [設定] 環境登録
	 *	@note 登録できるのは、プロセスで一つだけです。
	 *	@param hInstance インスタンスハンドル。ローカルフックの場合 NULL を指定します。
	 *	@param dwThreadId スレッドID。省略すると０が設定されます。\n
	 *			※スレッドID に０を指定すると、既存のすべてのスレッドに関連付けられます。 
	 */
	static void SetEnvironment(HINSTANCE hInstance, DWORD dwThreadId = 0)
	{
		WinHook::g_hInstance = hInstance;
		WinHook::g_dwThreadId = dwThreadId;
	}

	/**
	 * [登録] フック登録
	 *	@note 指定タイプのフックを開始します（リスナーがコールされるようになります）。
	 *	@param iHookType フックタイプ。 WH_MOUSEなど、WH_～ で始まるシンボルを指定します。
	 *	@param PF HOOKPROC型のプロシージャ関数。( LRESULT CALLBACK (*HOOKPROC)(int, WPARAM, LPARAM) )
	 *	@retval NULL以外 フックハンドル。記憶しておき、PF内で CallNextHookEx() の第一引数に使用してください。
	 *	@retval NULL 失敗。
	 */
	static HHOOK Hook(int iHookType, HOOKPROC PF)
	{
		static CReject s_cReje;
		LPHOOKFUNC P = m_GetFunc(iHookType);
		if ( P != NULL ){ return P(true, PF); }
		return NULL;
	}
	
	/**
	 * [登録] フック解除
	 *	@note 指定タイプのフックを解除します。
	 *	@param iHookType フックタイプ。 WH_MOUSEなど、WH_～ で始まるシンボルを指定します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool Unhook(int iHookType)
	{
		LPHOOKFUNC P = m_GetFunc(iHookType);
		if ( P != NULL ){ P(false, NULL); }
		return P != NULL;
	}

	/**
	 * [登録] 全フックリスナー解除
	 */
	static void UnhookAll(void)
	{
		for ( int i = WH_MIN; i < 30; i++ )
		{
			Unhook(i);
		}
	}
};



/// 破棄用のデストラクタ
inline CWindowsHooker::CReject::~CReject(void)
{
	CWindowsHooker::UnhookAll();
}



};//TNB
