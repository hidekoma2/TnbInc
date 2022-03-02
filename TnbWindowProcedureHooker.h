#pragma once
/**
 *@file
 * �E�B���h�E�v���V�[�W���t�b�N�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSimpleMap.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 // HWND�ƃ��X�i�[��MAP
 _SELECTANY CSimpleMapT<HWND, LPVOID>* s_pmapWphHwndProc = NULL; 
 _SELECTANY bool s_isWphInit = true; 
#endif



/**@ingroup WINAPI
 * �E�B���h�E�v���V�[�W���t�b�N�N���X
 *
 *		WindowProc() ���p�����āA Attach ���邱�ƂŁA�E�B���h�E�v���V�[�W�����������邱�Ƃ��\�ł��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		class CMyProc : public CWindowProcedureHooker
 *		{
 *			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
 *			{
 *				if ( message == XXX )
 *				{
 *					// �������s���B �K�v�ɂ���āA�X�[�p�[�N���X�� WindowProc���R�[�������ɏI����Ă��悵�B
 *				}
 *				return CWindowProcedureHooker::WindowProc(message, wParam, lParam); 
 *			}
 *		};
 *
 *		class CFooDlg : public CDialog
 *		{
 *				;
 *		    CMyProc m_procHook;
 *				;
 *		};
 *
 *		BOOL CFooDlg::OnInitDialog()
 *		{
 *			CDialog::OnInitDialog();
 *				;
 *			m_procHook.Attach(*this);	// �A�^�b�`����B����ȍ~�A CMyProc�� WindowProc���R�[���o�b�N�����B		
 *				;
 *
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbWindowProcedureHooker.h
 *
 *	@date 08/10/21 �V�K�쐬
 *	@date 09/04/22 �}�b�v�� CSimpleMap �ɕύX
 *	@date 09/04/23 ���v���V�[�W���R�[���� ::CallWindowProc() ���g���悤�ɏC��
 *	@date 11/11/29 IsAttached() �V�K�B
 */
class CWindowProcedureHooker
{
public:
	/// �R���X�g���N�^
	CWindowProcedureHooker(void) : m_hWnd(NULL), m_pOrgWndProc(NULL)
	{
	}

	/// �f�X�g���N�^
	~CWindowProcedureHooker(void)
	{
		Destroy();
	}

	/**
	 * [�m�F] �A�^�b�`���H
	 *	@retval true �A�^�b�`��
	 *	@retval false ��
	 */
	bool IsAttached(void) const
	{
		return m_pOrgWndProc != NULL;
	}

	/**
	 * [�ݒ�] �A�^�b�`
	 *	@param hWnd �Ώۂ̃E�B���h�E
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Attach(HWND hWnd)
	{
		Detach();
		if ( s_isWphInit && s_pmapWphHwndProc == NULL )
		{
			s_isWphInit = false;
			static CReject s_cRejeWPH;
			s_pmapWphHwndProc = new CSimpleMapT<HWND, LPVOID>;
		}
		if ( s_pmapWphHwndProc == NULL || s_pmapWphHwndProc->HasKey(hWnd) )
		{
			return false;
		}
		//
		PreAttach(hWnd);
		//
		WNDPROC oldWndProc = m_SetWndProc(hWnd, ms_WndProc);
		if ( oldWndProc == ms_WndProc || oldWndProc == NULL )
		{
			return false;
		}
		m_pOrgWndProc = oldWndProc;
		m_hWnd = hWnd;
		(*s_pmapWphHwndProc)[hWnd] = this;
		//
		OnAttached();
		return true;
	}

	/**
	 * [�ݒ�] �f�^�b�`
	 */
	void Detach(void)
	{
		if ( m_pOrgWndProc != NULL )
		{
			PreDetach();
			WNDPROC oldWndProc = m_SetWndProc(m_hWnd, m_pOrgWndProc);
			IgnoreUnusedValue(oldWndProc);
			if ( s_pmapWphHwndProc != NULL )
			{
				s_pmapWphHwndProc->RemoveKey(m_hWnd);
			}
			OnDetached();
			m_hWnd = NULL;
			m_pOrgWndProc = NULL;
		}
	}

	/**
	 * [�ݒ�] �j��
	 *	@note Detach() �Ƃ͈قȂ�A PreDetach() , OnDetached() ���Ăяo����܂���B
	 */
	void Destroy(void)
	{
		if ( m_pOrgWndProc != NULL )
		{
			WNDPROC oldWndProc = m_SetWndProc(m_hWnd, m_pOrgWndProc);
			IgnoreUnusedValue(oldWndProc);
			if ( s_pmapWphHwndProc != NULL )
			{
//				s_pmapWphHwndProc->RemoveKey(m_hWnd);
			}
			m_hWnd = NULL;
			m_pOrgWndProc = NULL;
		}
	}

	/**
	 * [�擾] �E�B���h�E�n���h���擾
	 *	@retval NULL �A�^�b�`���Ă��Ȃ�
	 *	@retval NULL�ȊO �E�B���h�E�n���h��
	 */
	HWND GetSafeHwnd(void) const
	{
		return m_hWnd;
	}

	/**
	 * [����] SendMessage.
	 *		�A�^�b�`���Ă���E�B���h�E��SendMessage���܂��B
	 *	@param message ���b�Z�[�W�B
	 *	@param wParam WPARAM�B
	 *	@param lParam LPARAM�B
	 *	@return ���ʁB
	 */
	LRESULT SendMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::SendMessage(m_hWnd, message, wParam, lParam);
	}

	/**
	 * [����] PostMessage.
	 *		�A�^�b�`���Ă���E�B���h�E��PostMessage���܂��B
	 *	@param message ���b�Z�[�W�B
	 *	@param wParam WPARAM�B
	 *	@param lParam LPARAM�B
	 *	@return ���ʁB
	 */
	BOOL PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::PostMessage(m_hWnd, message, wParam, lParam);
	}

	/**
	 * [�擾] �X�^�C���擾
	 *	@return �X�^�C��
	 */
	DWORD GetStyle(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_STYLE);
	}

	/**
	 * [�擾] �g���X�^�C���擾
	 *	@return �g���X�^�C��
	 */
	DWORD GetExStyle(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}

	/**
	 * [�ύX] �X�^�C���ύX
	 *	@param dwRemove ���������E�B���h�E �X�^�C��
	 *	@param dwAdd �ǉ������E�B���h�E �X�^�C��
	 */
	void ModifyStyle(DWORD dwRemove, DWORD dwAdd)
	{
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if ( dwStyle != dwNewStyle )
		{
			::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
		}
	}

	/**
	 * [�ύX] �g���X�^�C���ύX
	 *	@param dwRemove ���������E�B���h�E �g���X�^�C��
	 *	@param dwAdd �ǉ������E�B���h�E �g���X�^�C��
	 */
	void ModifyStyleEx(DWORD dwRemove, DWORD dwAdd)
	{
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if ( dwStyle != dwNewStyle )
		{
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewStyle);
		}
	}

protected:

	/**
	 * [�ʒm] �A�^�b�`���O�ʒm
	 *	@param hWnd �A�^�b�`����HWND�B
	 */
	virtual void PreAttach(HWND hWnd) {}
	
	/**
	 * [�ʒm] �A�^�b�`�ʒm
	 */
	virtual void OnAttached(void) {}
	
	/**
	 * [�ʒm] �f�^�b�`���O�ʒm
	 */
	virtual void PreDetach(void) {}

	/**
	 * [�ʒm] �f�^�b�`�ʒm
	 */
	virtual void OnDetached(void) {}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B������I�[�o�[���C�h���ď�����ǉ����܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT r = ::CallWindowProc(m_pOrgWndProc, m_hWnd, message, wParam, lParam);
		if ( message == WM_DESTROY )
		{
			Detach();
		}
		return r;
	}

	HWND				m_hWnd;					///< �Ǘ����Ă���E�B���h�E	

private:
	// WNDPROC�ݒ�
	WNDPROC m_SetWndProc(HWND hWnd, WNDPROC P)
	{
		LONG_PTR i = reinterpret_cast<LONG_PTR>(P);
		return reinterpret_cast<WNDPROC>(::SetWindowLongPtr(hWnd, GWLP_WNDPROC, i));
	}

	/**
	 * [�ʒm] WndProc 
	 *	@note ���b�Z�[�W�v���V�[�W��
	 */
	static LRESULT CALLBACK ms_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( s_pmapWphHwndProc != NULL )
		{
			CWindowProcedureHooker* P = static_cast<CWindowProcedureHooker*>((*s_pmapWphHwndProc)[hWnd]);
			if ( P != NULL )
			{
				return P->WindowProc(message, wParam, lParam);
			}
		}
		return 0;
	}
	//Unhook�����p�T�u�N���X
	class CReject
	{
	public:	
		/// �f�X�g���N�^
		~CReject(void)
		{
			if ( s_pmapWphHwndProc != NULL )
			{
//				ASSERT( s_pmapWphHwndProc->GetSize() == 0 );
				s_pmapWphHwndProc->RemoveAll();
				delete s_pmapWphHwndProc;
				s_pmapWphHwndProc = NULL;
			}
		}
	};
	WNDPROC				m_pOrgWndProc;			///< �t�b�N������ WndProc
};



}; //TNB
