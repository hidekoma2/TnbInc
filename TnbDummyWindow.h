#pragma once
/**
 *@file
 * �_�~�[�E�B���h�E�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"
#include "TnbSimpleMap.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 // HWND�ƃ��X�i�[��MAP
 _SELECTANY CSimpleMapT<LPVOID, LPVOID>* s_pmapDmyWndHwndProc = NULL; 
 _SELECTANY CSyncSection* s_psyncDmyWnd = NULL; 

 #ifndef WS_OVERLAPPEDWINDOW
  #define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
 #endif
#endif



/**@ingroup WINAPI
 * �_�~�[�E�B���h�E�N���X
 *
 *		�ȈՓI�ȃE�B���h�E���쐬���܂��B MFC ���g�p���Ă��܂���B
 *	
 *	@note CUI �A�v���Ń��b�Z�[�W�̎�M�ȂǂŃE�B���h�E�n���h�����K�v�Ȏ��A�{�N���X�Ō����Ȃ��E�B���h�E���쐬���A
 *		���b�Z�[�W�̎�M�������s�����Ƃ��o���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDummyWindow.h
 *
 *	@date 06/06/14	�V�K�쐬
 *	@date 09/04/22	�}�b�v�� CSimpleMap �ɕύX
 *	@date 09/07/22	�u���V�w��\�ɕύX
 *	@date 10/04/19	�쐬���A���j���[���w��ł���悤�ɂ����B
 *	@date 10/05/10	�X�^�C���֌W�̃��\�b�h�ǉ��B
 *	@date 11/08/31	CreateByTemplate() ��ǉ��B
 *	@date 11/09/09	�_�C�A���O WM_CTLCOLOR�` �n�̓��ꃁ�b�Z�[�W�ɑΉ��B
 *	@date 15/11/20	SELECTANY �I�u�W�F�N�g�̔j���̕��@�� WM_DESTROY ���Ɉڊǂ����B
 */
class CDummyWindow : CCopyImpossible
{
public:

	/**@ingroup WINAPI
	 * �_�~�[�E�B���h�E�N���X�̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *	@see CDummyWindow::Create()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbDummyWindow.h
	 *
	 *	@date 06/06/14	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}
		/**
		 * [�ʒm] ��M���b�Z�[�W�ʒm.
		 *	@note WM_CREATE �́A�ʒm����܂���B
		 *	@note WM_DESTROY ���Ō�ɃE�B���h�E�͔j������܂��B
		 *	@param[out] _result ���ʊi�[�B true �Ń��^�[�������Ƃ��Ɏg�p����܂��B
		 *	@param[in] hWnd �E�B���h�E�n���h��
		 *	@param[in] message ���b�Z�[�W
		 *	@param[in] wParam WPARAM
		 *	@param[in] lParam LPARAM
		 *	@retval true ������(DefWindowProc()���Ăяo���Ȃ��Ȃ�܂�)
		 *	@retval false ������
		 */
		virtual bool OnWindowMessage(LRESULT& _result, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	};


	//----


	/// �R���X�g���N�^
	CDummyWindow(void) : m_hWnd(NULL)
	{
		m_brush = static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
	}

	/// �f�X�g���N�^
	~CDummyWindow(void)
	{
		Destroy();
	}

	/**
	 * [�擾] �E�B���h�E�n���h��.
	 *		�ێ����Ă���E�B���h�E�̃n���h����Ԃ��܂��B
	 *	@return �n���h��
	 */
	operator HWND(void) const
	{
		return m_hWnd;
	}

	/**
	 * [�擾] �E�B���h�E�n���h��.
	 *		�ێ����Ă���E�B���h�E�̃n���h����Ԃ��܂��B
	 *	@return �n���h��
	 */
	HWND GetSafeHwnd(void) const
	{
		return m_hWnd;
	}

	/**
	 * [�m�F] �E�B���h�E�̗L��.
	 *	@retval true �L��
	 *	@retval false �Ȃ�
	 */
	bool IsWindow(void) const
	{
		return !! ::IsWindow(m_hWnd);
	}

	/**
	 * [����] �E�B���h�E�̔j��.
	 */
	void Destroy(void)
	{
		if ( m_hWnd != NULL )
		{
			::DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
		m_UnregisterClass();
		m_className.Empty();
	}

	/**
	 * [�ݒ�] �w�i�u���V�ݒ�.
	 *		Create() ������Ɏw�肵�Ă��������B
	 *	@note CreateByTemplate() ���g���ꍇ�A�{���\�b�h�͈Ӗ�����܂���B
	 *	@param b �u���V
	 */
	void SetBrush(HBRUSH b)
	{
		m_brush = b;
	}

	/**
	 * [����] �E�B���h�E�̍쐬.
	 *		�E�B���h�E���쐬���܂��B
	 *	@note ��̃C���X�^���X�ŊǗ��ł���E�B���h�E�͈�����ł��B
	 *		�����쐬���悤�Ƃ���ƁA��̃E�B���h�E���j������܂��B
	 *	@param I ���X�i�[
	 *	@param lpszTitle �E�B���h�E�̃^�C�g���B�ȗ��\�ł��BFindWindow�ȂǂŁA
	 *					�^�C�g�����K�v�ȏꍇ�w�肵�܂��B
	 *	@param lpszClassName �E�B���h�E�̃N���X���B�ȗ��\�ł��BFindWindow�ȂǂŁA
	 *					�N���X�����K�v�ȏꍇ�w�肵�܂��B
	 *	@param dwStyle �E�B���h�E�X�^�C���B�ȗ�����ƁA WS_OVERLAPPEDWINDOW �������w�肳��܂��B
	 *	@param hMenu ���j���[�B�ȗ������ NULL ���w�肳��܂��B
	 *	@param hParent �e�E�B���h�E�B�ȗ������ NULL ���w�肳��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Create(CDummyWindow::IListener* I, LPCTSTR lpszTitle = NULL, LPCTSTR lpszClassName = NULL, DWORD dwStyle = WS_OVERLAPPEDWINDOW, HMENU hMenu = NULL, HWND hParent = NULL)
	{
		if ( I != NULL )
		{
			m_CreateSub();
			if ( lpszTitle == NULL )
			{
				lpszTitle = _T("");
			}
			m_className = lpszClassName;
			if ( m_className.IsEmpty() )
			{
				m_className.Format(_T("TnbLib:%d:%d"), ::GetCurrentProcessId(), this);
			}
			m_RegisterClass();
			HINSTANCE hInst = GetInstanceHandleByTnb(EI_Process);
			m_hWnd = ::CreateWindow(m_className, lpszTitle, dwStyle, 100, 0, 100, 0, hParent, hMenu, hInst, NULL);
			if ( m_hWnd != NULL )
			{
				(*s_pmapDmyWndHwndProc)[m_hWnd] = I;
				return true;
			}
		}
		return false;
	}

	/**
	 * [����] �E�B���h�E�̍쐬�i�e���v���[�g�g�p�j.
	 *		���\�[�X�̃e���v���[�g���g�p���ăE�B���h�E���쐬���܂��B
	 *	@note ��̃C���X�^���X�ŊǗ��ł���E�B���h�E�͈�����ł��B
	 *		�����쐬���悤�Ƃ���ƁA��̃E�B���h�E���j������܂��B
	 *	@param I ���X�i�[
	 *	@param templateId �_�C�A���O�̃��\�[�X�_�e���v���[�g�h�c���w�肵�܂��B
	 *	@param hParent �e�E�B���h�E�B�ȗ������ NULL ���w�肳��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool CreateByTemplate(CDummyWindow::IListener* I, UINT templateId, HWND hParent = NULL)
	{
		if ( I != NULL )
		{
			m_CreateSub();
			HINSTANCE hInst = GetInstanceHandleByTnb(EI_Process);
			HANDLE h = ::GetCurrentThread();
			(*s_pmapDmyWndHwndProc)[h] = I;	//���L�[
			m_hWnd = ::CreateDialog(hInst, MAKEINTRESOURCE(templateId), hParent, ms_DlgProc);
			if ( m_hWnd != NULL )
			{
				(*s_pmapDmyWndHwndProc)[m_hWnd] = I;
				s_pmapDmyWndHwndProc->RemoveKey(h);	//���L�[�폜
				return true;
			}
		}
		return false;
	}

	/**
	 * [�擾] �X�^�C���擾.
	 *	@return �X�^�C��
	 */
	DWORD GetStyle(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_STYLE);
	}

	/**
	 * [�擾] �g���X�^�C���擾.
	 *	@return �g���X�^�C��
	 */
	DWORD GetStyleEx(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}

	/**
	 * [�ݒ�] �X�^�C���ύX.
	 *	@param dwRemove ��������X�^�C��
	 *	@param dwAdd �ǉ�����X�^�C��
	 *	@param nFlags SetWindowPos �ɓn���t���O�BSetWindowPos ���Ăяo���Ȃ��Ƃ��� 0 �ł��B����l�� 0 �ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		return m_ModifyStyle(GWL_STYLE, dwRemove, dwAdd, nFlags);
	}

	/**
	 * [�ݒ�] �g���X�^�C���ύX.
	 *	@param dwRemove ��������g���X�^�C��
	 *	@param dwAdd �ǉ�����g���X�^�C��
	 *	@param nFlags SetWindowPos �ɓn���t���O�BSetWindowPos ���Ăяo���Ȃ��Ƃ��� 0 �ł��B����l�� 0 �ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		return m_ModifyStyle(GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
	}

private:

#if 0
	//Unhook�����p�T�u�N���X
	class CReject
	{
	public:	
		~CReject(void)
		{
			if ( s_pmapDmyWndHwndProc != NULL )
			{
				s_pmapDmyWndHwndProc->RemoveAll();
				delete s_pmapDmyWndHwndProc;
				s_pmapDmyWndHwndProc = NULL;
			}
			if ( s_psyncDmyWnd != NULL )
			{
				delete s_psyncDmyWnd;
				s_psyncDmyWnd = NULL;
			}
		}
	};
#endif
	/// Create�T�u
	void m_CreateSub(void)
	{
		if ( s_pmapDmyWndHwndProc == NULL )
		{
//			static CReject s_cReje;
			s_pmapDmyWndHwndProc = new CSimpleMapT<LPVOID, LPVOID>;
			s_psyncDmyWnd = new CSyncSection;
		}
		Destroy();
	}
	/// �X�^�C���ύX
	bool m_ModifyStyle(int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
	{
		DWORD dwStyle = ::GetWindowLong(m_hWnd, nStyleOffset);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if ( dwStyle == dwNewStyle )
		{
			return false;
		}
		::SetWindowLong(m_hWnd, nStyleOffset, dwNewStyle);
		if ( nFlags != 0 )
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}
		return true;
	}
	/// [�ʒm] ���b�Z�[�W����.
	static bool CALLBACK ms_CmnProc(LRESULT& _result, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if ( s_pmapDmyWndHwndProc == NULL )
		{
			return false;
		}
		s_psyncDmyWnd->Lock();
		IListener* I = NULL;
		if ( s_pmapDmyWndHwndProc->HasKey(hWnd) )
		{
			LPVOID P = (*s_pmapDmyWndHwndProc)[hWnd];
			I = static_cast<IListener*>(P);
		}
		else
		{
			HANDLE h = ::GetCurrentThread();
			if ( s_pmapDmyWndHwndProc->HasKey(h) )
			{
				LPVOID P = (*s_pmapDmyWndHwndProc)[h];
				I = static_cast<IListener*>(P);
			}
		}
		bool r = false;
		if ( I != NULL )
		{
			r = I->OnWindowMessage(_result, hWnd, uMsg, wParam, lParam);
		}
		if ( uMsg == WM_DESTROY )
		{
			s_pmapDmyWndHwndProc->RemoveKey(hWnd);
			if ( s_pmapDmyWndHwndProc->GetSize() == 0 )
			{
				if ( s_pmapDmyWndHwndProc != NULL )
				{
					delete s_pmapDmyWndHwndProc;
					s_pmapDmyWndHwndProc = NULL;
				}
				if ( s_psyncDmyWnd != NULL )
				{
					s_psyncDmyWnd->Unlock();
					delete s_psyncDmyWnd;
					s_psyncDmyWnd = NULL;
				}
			}
		}
		if ( s_psyncDmyWnd != NULL )
		{
			s_psyncDmyWnd->Unlock();
		}
		return r;
	}
	static LRESULT CALLBACK ms_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT r;
		if ( ms_CmnProc(r, hWnd, uMsg, wParam, lParam) )
		{
			return r;
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	/// [�ʒm] ���b�Z�[�W����.
	static INT_PTR CALLBACK ms_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT r;
		if ( ms_CmnProc(r, hWnd, uMsg, wParam, lParam) )
		{
			switch ( uMsg )
			{
			case WM_CHARTOITEM:
			case WM_COMPAREITEM:
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORDLG:
			case WM_CTLCOLOREDIT:
			case WM_CTLCOLORLISTBOX:
			case WM_CTLCOLORSCROLLBAR:
			case WM_CTLCOLORSTATIC:
			case WM_CTLCOLORMSGBOX:
			case WM_INITDIALOG:
			case WM_QUERYDRAGICON:
			case WM_VKEYTOITEM:
				return r; // WM_CTLCOLOR�` �n�Ȃǂ� DWL_MSGRESULT ���g�킸�A�߂�l�ŕԂ��B
			}
			#ifdef DWL_MSGRESULT
				::SetWindowLong(hWnd, DWL_MSGRESULT, r);
			#else
				::SetWindowLongPtr(hWnd, DWLP_MSGRESULT, r);
			#endif
			return TRUE;
		}
		return FALSE;
	}
	/// [�o�^] �E�B���h�E�N���X�o�^
	ATOM m_RegisterClass(void)
	{
		#ifndef _WIN32_WCE
			WNDCLASSEX wc	= { sizeof(WNDCLASSEX) };
		#else
			WNDCLASS wc		= { sizeof(WNDCLASS) };
		#endif
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= ms_WndProc;
		wc.hInstance		= GetInstanceHandleByTnb(EI_Process);
		wc.hbrBackground	= m_brush;
		wc.lpszClassName	= m_className;
		#ifndef _WIN32_WCE
			return ::RegisterClassEx(&wc);
		#else
			return ::RegisterClass(&wc);
		#endif
	}
	/// [�o�^] �E�B���h�E�N���X����
	void m_UnregisterClass(void)
	{
		if ( ! m_className.IsEmpty() )
		{
			::UnregisterClass(m_className, GetInstanceHandleByTnb(EI_Process));
			m_className.Empty();
		}
	}
	HWND	m_hWnd;			///< �ێ�����E�B���h�E�n���h��
	CStr	m_className;	///< �쐬�����E�B���h�E�N���X��
	HBRUSH	m_brush;		///< �w�i�u���V
};




};///TNB
