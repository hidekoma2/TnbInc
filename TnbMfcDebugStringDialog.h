#pragma once
/**
 *@file
 * DebugString �E�B���h�E�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcTinyListDialog.h"
#include "TnbApiHooker.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
namespace DSD {
	typedef VOID (WINAPI* P_OutputDebugStringA)(LPCSTR lpOutputString);
	typedef VOID (WINAPI* P_OutputDebugStringW)(LPCWSTR lpOutputString);
	_SELECTANY CApiHookerT<P_OutputDebugStringA>* s_pHookerA = NULL;
	_SELECTANY CApiHookerT<P_OutputDebugStringW>* s_pHookerW = NULL;
	_SELECTANY CTinyListDialog* s_pListDialog = NULL;
};
#endif



/**@ingroup MFCCONTROL
 * DebugString �E�B���h�E
 *
 *		�J���x���p�ł��B
 *		API �� OutputDebugString() ���t�b�N���A��ʏ�ɕ\�����邱�Ƃ��o���郂�[�h���X�_�C�A���O�ł��B
 *
 *	@par�g�p��
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
 *	@note �����N���Ă��� dll ���� OutputDebugString() �̓t�b�N���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDebugStringDialog.h
 * 
 *	@date 12/01/27	�V�K�쐬
 */
class CDebugStringDialog : public CTinyListDialog
{
	DEFSUPER(CTinyListDialog);
public:

	/// �R���X�g���N�^
	CDebugStringDialog(void) : _super(false)
	{
	}

protected:

	/**
	 * [�ʒm] �쐬�����ʒm.
	 *	@note �E�B���h�E�쐬������ɒʒm����܂��B
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
	 * [�ʒm] �j���ʒm.
	 *	@note �E�B���h�E�j�����O�ɒʒm����܂��B
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
	 * [����] ���b�Z�[�W�|���v
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
	 * [�ʒm] OutputDebugStringA ���s��
	 *		OutputDebugStringA() ���s���ɃR�[������܂��B
	 *	@param lpszOutputString ������
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
	 * [�ʒm] OutputDebugStringW ���s��
	 *		OutputDebugStringW() ���s���ɃR�[������܂��B
	 *	@param lpszOutputString ������
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

