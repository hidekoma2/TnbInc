#pragma once
/**
 *@file
 * �J�X�^���E�B���h�E�t���[���}�X�^�[�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCustomWindowFrame.h"
#include "TnbWindowsVersion.h"
#include "TnbSimpleMap.h"
#include "TnbBitFlag.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
_SELECTANY class CCustomWindowFrameMaster* g_pCwfmMaster = NULL;	///< �}�X�^�[�C���X�^���X
#endif



/**@ingroup DRAWABLE MFCCONTROL
 * �J�X�^���E�B���h�E�t���[���}�X�^�[�N���X.
 *	
 *		�_�C�A���O�� NonClient ������ύX�ł��܂��B�܂��A���ߐF���w�肵�A��`�ȊO�̃t���[���ɂ��Ή����܂��B
 *
 *	@note �v���Z�X�����[�J���t�b�N���Ă���̂ŁA�v���Z�X�ŕ\�������S�E�B���h�E�� NonClient ������ύX�ł��܂��B
 *
 *	@par�g�p��
 *		\code
 *
 *		BOOL CFooApp::InitInstance()
 *		{
 *			CCustomWindowFrameMaster frame;
 *				;
 *			// �t���[���̐ݒ�
 *			CWindowCaptionDrawer ttd(NULL);
 *			ttd.SetBarColor(RGB(255, 0, 0), RGB(0, 0, 0));
 *			frame.SetTitleBitmap(ttd);
 *			frame.SetFrameBitmap(IDB_FRAME);
 *			frame.SetCloseButtonBitmap(CPoint(-6, 5), IDB_CLOSE, 4);		
 *				;
 *			// �t�b�N����B
 *			frame.Hook();
 *				;
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *		TnbMfcCustomWindowFrameMaster.h
 *
 *	@date 08/10/06 �V�K�쐬
 *	@date 09/04/22 �}�b�v�� CSimpleMap �ɕύX
 *	@date 09/05/12 VISTA �̏ꍇ�AHook���Ȃ��悤�ɕύX�B
 *	@date 10/04/26 Hook �̂��߂̃C���X�^���X�͕ێ����Ă����K�v���Ȃ������B
 */
class CCustomWindowFrameMaster : public CCustomWindowFrame
{
	DEFSUPER(CCustomWindowFrame);
public:

	/// �R���X�g���N�^
	CCustomWindowFrameMaster(void) : m_pCwfmOldFrame(NULL)
	{
	}

	/// �f�X�g���N�^
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
	 * [�ݒ�] �t�b�N.
	 *		�v���Z�X�ň�A�t�b�N�o���܂��B�{�C���X�^���X�j������A�t�b�N�͗L���ł��B
	 *	@param isDisableVista true ���w�肷��ƁA VISTA�ȍ~�̏ꍇ�A�t�b�N���܂���B
	 *						false ���w�肷��ƁA VISTA�ȍ~�ł��A�t�b�N���܂��B
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
	 * [�ݒ�] �A���t�b�N.
	 *		Hook() �������̂��������܂��B
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
	//Unhook�����p�T�u�N���X
	class CReject
	{
	public:	
		/// �f�X�g���N�^
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
			// ���j���[�H
			canAttach = true; 
			isMenu = true;
			if ( nCode == HCBT_CREATEWND )
				TTRACE0("�����j���[!\n");
		}
		if ( wi.atomWindowType == 0x8000 && styleEx== 0x181 )
		{
			// ���j���[�H
			canAttach = true; 
			isMenu = true;
			if ( nCode == HCBT_CREATEWND )
				TTRACE0("�����j���[!\n");
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
		// ���̃t�b�N�ɓn��
		return ::CallNextHookEx(g_pCwfmMaster->m_hCwfmCbtHook, nCode, wParam, lParam);
	}

	HHOOK										m_hCwfmCbtHook;		///< CBT�t�b�N�n���h��
	CSimpleMapT<HWND, CCustomWindowFrame*>		m_cwfmMap;			///< �Ǘ��}�b�v
	CCustomWindowFrame*							m_pCwfmOldFrame;	///< 
};



}; //MFC
}; //TNB
