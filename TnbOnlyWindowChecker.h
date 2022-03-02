#pragma once
/**
 *@file
 * �I�����[�E�B���h�E�`�F�b�N�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �I�����[�E�B���h�E�`�F�b�J�[.
 *
 *		�A�v���P�[�V�����̑��d�N���h�~�Ɏg�p���܂��B
 *	
 *	@note �Q�ڂ̋N�����`�F�b�N���A�Q�ڂȂ�A�P�ڂ̃A�v�����A�N�e�B�u�ɂ��ă^�C�g���o�[��_�ł����܂�
 *			(���̓����� OnFound() ���I�[�o�[���C�h���邱�ƂŕύX�\�ł�)�B
 *
 *	\par�g�p��
 *		\code
 *		BOOL CMainDlg::OnInitDialog()
 *		{
 *			CDialog::OnInitDialog();
 *			// ���d�N���`�F�b�N
 *			static COnlyWindowChecker s_owc(_T("TnbSampleApp.2011.10.12"));	//static �ɂ���
 *			if ( s_owc.IsOnly(GetSafeHwnd()) )
 *			{
 *				EndDialog(true);
 *				return TRUE;
 *			}
 *			// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B
 *			ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
 *			ASSERT(IDM_ABOUTBOX < 0xF000);
 *			CMenu* pSysMenu = GetSystemMenu(FALSE);
 *			if (pSysMenu != NULL)
 *				
 *				;
 *				;
 *
 *		\endcode
 *		�� static �錾�����A COnlyWindowChecker �̃C���X�^���X�� CMainDlg �̃����o�ɂ��Ă��悢�B
 *
 *	@par�K�v�t�@�C��
 *			TnbOnlyWindowChecker.h
 *
 *	@date 11/10/12 �V�K�쐬�B
 */
class COnlyWindowChecker : public CCopyImpossible
{
public:
	
	/// �f�X�g���N�^
	~COnlyWindowChecker(void)
	{
		m_DeleteMutex();
	}

	/**
	 * �R���X�g���N�^.
	 *	@param lpszUniquelyText ���ʗp�̃��j�[�N�ȕ�����.
	 */
	COnlyWindowChecker(LPCTSTR lpszUniquelyText) : m_uniquelyText(lpszUniquelyText), m_pMutex(NULL)
	{
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		�����E�B���h�E�����邩�A�`�F�b�N���܂��B
	 *	@param hWnd �����̃E�B���h�E�n���h��
	 *	@retval true �������B�����I������K�v����B
	 *	@retval false �Ȃ������B
	 *	@throw CInvalidParamException �ݒ肪�ُ�B hWnd ���������Ȃ����R���X�g���N�^�Ń��j�[�N�����񂪐������w�肳��Ă��Ȃ����B
	 */
	bool IsOnly(HWND hWnd)
	{
		if ( hWnd == NULL || ! ::IsWindow(hWnd) || m_uniquelyText.IsEmpty() )
		{
			ASSERT0( false, "COnlyWindowChecker::IsOnly()", "�p�����[�^�̐ݒ肪�s���ł��B" );
			throw CInvalidParamException();
		}
		m_DeleteMutex();
		m_pMutex = new CSyncMutex(true, m_uniquelyText + _T("_mx"));
		if ( ! m_pMutex->Lock(0) )
		{
			// �N���g���Ă���
			m_hWnd = hWnd;
			::EnumWindows(ms_EnumFunc, reinterpret_cast<LPARAM>(this));
			return true;
		}
		// ���߂�
		VERIFY( ::SetProp(hWnd, m_uniquelyText, reinterpret_cast<HANDLE>(TRUE)) );
		return false;
	}

protected:

	/**
	 * [�ʒm] �����E�B���h�E����.
	 *	@note �{���\�b�h�ł́A�E�B���h�E���A�N�e�B�u�ɂ��A�R��L���v�V������_�ł����܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 */
	virtual void OnFound(HWND hWnd)
	{
		if ( ::IsWindowVisible(hWnd) )
		{
			::ShowWindow(hWnd, SW_SHOWNORMAL);
			::SetForegroundWindow(hWnd);
			FLASHWINFO fi = { sizeof(FLASHWINFO) };
			fi.hwnd = hWnd;
			fi.dwFlags = FLASHW_ALL;
			fi.uCount = 3;
			fi.dwTimeout = 1000 / (3 * 2);
			::FlashWindowEx(&fi);
		}
	}

private:
	/// MUTEX �j��
	void m_DeleteMutex(void)
	{
		if ( m_pMutex != NULL )
		{
			delete m_pMutex;
			m_pMutex = NULL;
		}
	}
	/// ENUM�R�[���o�b�N�֐�
	BOOL m_EnumFunc(HWND hWnd)
	{
		if ( m_hWnd != hWnd )
		{
			HANDLE h = ::GetProp(hWnd, m_uniquelyText);
			if ( h != NULL )
			{
				OnFound(hWnd);
				return FALSE; // enum�I���
			}
		}
		return TRUE;
	}
	/// ENUM�R�[���o�b�N�֐�
	static BOOL CALLBACK ms_EnumFunc(HWND hWnd, LPARAM lParam)
	{
		COnlyWindowChecker* P = reinterpret_cast<COnlyWindowChecker*>(lParam);
		return P->m_EnumFunc(hWnd);
	}

	CSyncMutex*		m_pMutex;			///< �r��
	CStr			m_uniquelyText;		///< ���ʗp���j�[�N����
	HWND			m_hWnd;				///< �R�[���o�b�N���p���[�N
};



};//TNB

