#pragma once
/**
 *@file
 * �E�B���h�E�Y�\���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDynamicFunc.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �E�B���h�E�Y�\���Ǘ�
 *
 *		API �� PrintWindow ���Č����܂��B XP�ȊO�ł� WM_PRINT ���g���A�݊��œ����܂��B
 *	
 *	@par�K�v�t�@�C��
 *			TnbWindowPrinter.h
 *
 *	@date 08/01/22 �V�K�쐬�B
 */
class CWindowPrinter
{
	bool m_hasPrintWindow;	///< �t���O
public:
	/// �R���X�g���N�^
	CWindowPrinter(void) : m_hasPrintWindow(true)
	{
	}

	/**
	 * [�`��] �E�B���h�E�`��.
	 *	@param hWnd �ΏۃE�B���h�E
	 *	@param hdc �f�o�C�X�R���e�L�X�g.
	 *	@param isClientOnly true �Ȃ�N���C�A���g�̂݁B false �Ȃ� window�B
	 *	@retval true ����
	 *	@retval false ���s
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
