#pragma once
/**
 *@file
 * �E�B���h�E�Y�t�b�N�֌W�̃w�b�_
 *
 *		�V���{�� "TnbGLOBALHOOK" ���`���A�{�N���X��DLL���Ŏg�p����ƁA�O���[�o���t�b�N���\�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

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

// �E�B���h�E�Y�t�b�N�֌W
namespace WinHook
{
	#ifdef TnbGLOBALHOOK
		#pragma comment(linker, "/section:.tnbwinhook,RWS")
		#pragma data_seg(".tnbwinhook")
	#endif

	// �E�B���h�E�t�b�N�Ώۂ̃C���X�^���X�n���h��
	_SELECTANY HINSTANCE	g_hInstance		= NULL;
	// �E�B���h�E�t�b�N�Ώۂ̃X���b�hID
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
 * �E�B���h�E�Y�t�b�N�Ǘ�
 *
 *		�N���X�֐������L��܂���B�C���X�^���X���쐬���邱�Ƃ͏o���܂���B
 *
 *		Hook() �̖߂�l�͋L�����Ă����A�o�^���� HOOKPROC �^�̃v���V�[�W���֐��̍Ō�� ::CallNextHookEx() �ɓn���悤�ɂ��Ă��������B
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
 *	@note TnbWindowsHooker.h ���C���N���[�h����O�ɃV���{�� "TnbGLOBALHOOK" ���`���A�{�N���X��DLL���Ŏg�p����ƁA
 *		�O���[�o���t�b�N���\�ł��B�܂��A���̍ہA���L�Z�N�V�������́u.tnbwinhook�v�ɂȂ��Ă��܂��̂ŁA�K�v�ȕϐ��́A
 *		���L���Ă��������B
 *		\code 
 *		#pragma data_seg(".tnbwinhook")
 *		static HWND s_hWnd = NULL;		// ���L�Z�N�V����
 *		static HHOOK s_hook = NULL;		// ���L�Z�N�V����
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
 *	@par�K�v�t�@�C��
 *			TnbWindowsHooker.h
 *
 *	@date 06/06/05 �V�K�쐬�B
 *	@date 06/12/18 ���O�ύX�B
 *	@date 11/11/28 pragma �A�R�����g�ǉ��B OnDllMain() �ǉ��B
 */
class CWindowsHooker
{
	typedef HHOOK (*LPHOOKFUNC)(bool, HOOKPROC);

	//Unhook�����p�T�u�N���X
	class CReject
	{
	public:	~CReject(void);
	};

	// �w���Hook�^�C�v�̊֐��A�h���X�擾
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
	 * �R���X�g���N�^.
	 *	@note �C���X�^���X�s�K�v�Ȃ̂ŁA�R���X�g���N�^��private�ɂȂ��Ă��܂��B
	 */
	CWindowsHooker(void);

public:

	/**
	 * [�ݒ�] DllMain ���̏���
	 *		DLL �̃��C���֐����R�[�����ꂽ���ɃR�[������֐��ł��B reason �ɂ���āA�������s���܂��B
	 *	@note DLL_PROCESS_ATTACH ���ɂ́A���W���[���̃n���h�����L�����܂��B
	 *			DLL_PROCESS_DETACH ���ɂ́A���ׂẴt�b�N���������܂��B
	 *	@param hModule DllMain �̑�������n���܂��B
	 *	@param reason DllMain �̑�������n���܂��B
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
	 * [�ݒ�] ���o�^
	 *	@note �o�^�ł���̂́A�v���Z�X�ň�����ł��B
	 *	@param hInstance �C���X�^���X�n���h���B���[�J���t�b�N�̏ꍇ NULL ���w�肵�܂��B
	 *	@param dwThreadId �X���b�hID�B�ȗ�����ƂO���ݒ肳��܂��B\n
	 *			���X���b�hID �ɂO���w�肷��ƁA�����̂��ׂẴX���b�h�Ɋ֘A�t�����܂��B 
	 */
	static void SetEnvironment(HINSTANCE hInstance, DWORD dwThreadId = 0)
	{
		WinHook::g_hInstance = hInstance;
		WinHook::g_dwThreadId = dwThreadId;
	}

	/**
	 * [�o�^] �t�b�N�o�^
	 *	@note �w��^�C�v�̃t�b�N���J�n���܂��i���X�i�[���R�[�������悤�ɂȂ�܂��j�B
	 *	@param iHookType �t�b�N�^�C�v�B WH_MOUSE�ȂǁAWH_�` �Ŏn�܂�V���{�����w�肵�܂��B
	 *	@param PF HOOKPROC�^�̃v���V�[�W���֐��B( LRESULT CALLBACK (*HOOKPROC)(int, WPARAM, LPARAM) )
	 *	@retval NULL�ȊO �t�b�N�n���h���B�L�����Ă����APF���� CallNextHookEx() �̑������Ɏg�p���Ă��������B
	 *	@retval NULL ���s�B
	 */
	static HHOOK Hook(int iHookType, HOOKPROC PF)
	{
		static CReject s_cReje;
		LPHOOKFUNC P = m_GetFunc(iHookType);
		if ( P != NULL ){ return P(true, PF); }
		return NULL;
	}
	
	/**
	 * [�o�^] �t�b�N����
	 *	@note �w��^�C�v�̃t�b�N���������܂��B
	 *	@param iHookType �t�b�N�^�C�v�B WH_MOUSE�ȂǁAWH_�` �Ŏn�܂�V���{�����w�肵�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool Unhook(int iHookType)
	{
		LPHOOKFUNC P = m_GetFunc(iHookType);
		if ( P != NULL ){ P(false, NULL); }
		return P != NULL;
	}

	/**
	 * [�o�^] �S�t�b�N���X�i�[����
	 */
	static void UnhookAll(void)
	{
		for ( int i = WH_MIN; i < 30; i++ )
		{
			Unhook(i);
		}
	}
};



/// �j���p�̃f�X�g���N�^
inline CWindowsHooker::CReject::~CReject(void)
{
	CWindowsHooker::UnhookAll();
}



};//TNB
