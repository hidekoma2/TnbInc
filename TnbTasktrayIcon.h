#pragma once
/**
 *@file
 * �^�X�N�g���C�A�C�R���Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"
#include "TnbStr.h"



#include <shellapi.h>
#ifndef _WIN32_WCE
 #pragma comment(lib, "shell32.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup ICON
 * �^�X�N�g���C�A�C�R���Ǘ��N���X
 *
 *		�A�C�R���̃N���b�N�́A�o�^�������X�i�[�i CTasktrayIcon::IListener �j
 *		�ɒʒm����܂��B
 *
 *	@par�K�{����
 *		�A�C�R���̃N���b�N�����b�X������ɂ́A�e��WindowProc ���t�b�N���A
 *		�{�N���X�� RelayEvent() ���R�[������K�v���L��܂��B
 *		\code
 *			LRESULT CXxxxxDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
 *			{
 *				if ( m_tasktrayicon.RelayEvent(message, wParam, lParam) ) 
 *				{
 *					return 0;
 *				}
 *				return CDialog::WindowProc(message, wParam, lParam);
 *			}
 *		\endcode
 *
 *	@note	���j���[�͍ŏ��̃|�b�v�A�b�v���j���[�ȉ����g�p����܂��B
 *
 *	@note	Information() ���g�p����ɂ́A�v���W�F�N�g�ŃV���{�� _WIN32_IE �� 
 *			0x0500 �ȏ���w�肵�Ă����K�v���L��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTasktrayIcon.h
 *
 *	@date 06/04/17 �V�K
 *	@date 06/06/01 Explorer�����u�[�g��������ICON�ēo�^����悤�ɂ����B
 *	@date 06/06/05 MENU ID��o�^����ƁA���j���[�������I�ɏo���悤�ɂ����B
 *	@date 06/06/20 Information() ���\�b�h�A�ǉ��B
 *	@date 06/07/25 HMENU ���o�^�ł���悤�ɂ����B
 *	@date 08/04/04 HideInformation ��ǉ��B
 */
class CTasktrayIcon : CCopyImpossible
{
public:

	/**@ingroup ICON
	 * �^�X�N�o�[�A�C�R���Ǘ��̃��X�i�[�C���^�[�t�F�[�Y
	 *
	 *		CTasktrayIcon �̃R�[���o�b�N�ʒm�p�̃C���^�[�t�F�[�X�ł��B
	 *
	 *	@see CTasktrayIcon::SetListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTasktrayIcon.h
	 *
	 *	@date	06/04/17	�V�K
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �A�C�R������ʒm
		 *	@note �A�C�R���ɑ΂��ăN���b�N�����ƒʒm����܂��B
		 *	@param uMsg �ʒm���e�B
		 *			WM_LBUTTONDOWN / WM_LBUTTONUP / WM_LBUTTONDBLCLK /
		 *			WM_RBUTTONDOWN / WM_RBUTTONUP / WM_RBUTTONDBLCLK 
		 *			�̂����ꂩ�ł��B
		 *	@param tPoint �N���b�N���ꂽ���W�B
		 */
		virtual void OnNotifyTasktrayIcon(UINT uMsg, const POINT& tPoint) = 0;

	private:
		friend class CTasktrayIcon;	///< �t�����h�N���X�錾
	};

	/**
	 * �R���X�g���N�^
	 *	@param uID �����A�v���ŕ����̃^�X�N�g���C�A�C�R�����g��Ȃ��Ȃ�ȗ��ł��܂��B
	 *			�g���ꍇ�A�C���X�^���X���Ƃɓ��������ɂȂ�Ȃ��悤�Ɏw�肵�܂��B
	 */
	explicit CTasktrayIcon(UINT uID = 0)
	{
		m_icondata.uID				= uID;
		m_icondata.uCallbackMessage	= ::RegisterWindowMessage(CStr::Fmt(_T("__SSA_%u"), uID));
		m_icondata.ResetFlag();
		m_boShow					= false;
		m_piListener				= NULL;
		m_uTaskbarCreatedMessage	= ::RegisterWindowMessage(_T("TaskbarCreated"));
		m_idMenu					= 0;
		m_hMenu						= NULL;
	}

	/// �f�X�g���N�^
	virtual ~CTasktrayIcon(void)
	{
		Hide();
	}

	/**
	 * [�o�^] ���X�i�[�o�^
	 *	@note �A�C�R���ɑ΂��ẴN���b�N�C�x���g���E���ɂ̓��X�i�[��o�^���܂��B
	 *	@param piListener ���X�i�[
	 */
	void SetListener(IListener* piListener)
	{
		m_piListener = piListener;
	}
	
	/**
	 * [����] �C�x���g���p
	 *	@note �e�E�B���h�E�� WindowProc() ���烁�b�Z�[�W�𗬂��Ă��������B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@retval true �������{�B�e�E�B���h�E�̃��b�Z�[�W�����͍s���K�v������܂���B
	 *	@retval false �����Ȃ��B�e�E�B���h�E�̃��b�Z�[�W�������s���Ă��������B
	 */
	bool RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == m_uTaskbarCreatedMessage )
		{
			//Explorer�̍ċN���ȂǁBICON�̍Đݒ肪�K�v
			if ( m_boShow )
			{
				m_Modify(m_icondata);
			}
			return false;
		}
		if ( message != m_icondata.uCallbackMessage )
		{
			return false;
		}
		//TRACE1("RelayEvent = 0x%08X\n", lParam);
		switch ( lParam )
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			break;
		default:
			return false;
		}
		if ( ! m_syncClick.Lock(0) )
		{
			return false;
		}
		//
		bool rc = false;
		POINT tPoint = { 0x7FFF, 0x7FFF };
		::GetCursorPos(&tPoint);
		::SetForegroundWindow(m_icondata.hWnd);
		::SetFocus(m_icondata.hWnd);
		if ( m_piListener != NULL )
		{
			m_piListener->OnNotifyTasktrayIcon(static_cast<UINT>(lParam), tPoint);
		}
		#ifndef _WIN32_WCE
			if ( lParam == WM_RBUTTONDOWN )
			{
				rc = true;
			}
			if ( lParam == WM_RBUTTONUP )
			{
				rc = !! ShowMenu(tPoint, TPM_LEFTALIGN | TPM_RIGHTBUTTON);
			}
		#else
			if ( lParam == WM_LBUTTONUP )
			{
				rc = !! ShowMenu(tPoint, TPM_LEFTALIGN);
			}
		#endif
		m_syncClick.Unlock();
		return rc;
	}

	/**
	 * [�쐬] �A�C�R���쐬
	 *	@param hWnd �ʒm����E�B���h�E�n���h��
	 *	@param hIcon �\������ICON
	 *	@param lpTip �\������`�b�v���e�i�ȗ��\�j
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool Create(HWND hWnd, HICON hIcon, LPCTSTR lpTip = NULL)
	{
		m_icondata.hIcon = hIcon;
		m_icondata.hWnd	= hWnd;
		m_icondata.SetTip(lpTip);
		m_icondata.ResetFlag();
		m_Modify(m_icondata);
		m_boShow = true;
		return true;
	}

	/**
	 * [�폜] �A�C�R������
	 */
	void Hide(void)
	{
		if ( m_boShow )
		{
			::Shell_NotifyIcon(NIM_DELETE, m_icondata);
			m_boShow = false;
		}
	}

	/**
	 * [�ݒ�] ���j���[�ύX�iID�w��j
	 *	@note �����ID���w�肷��ƃn���h���w��͖����ɂȂ�܂��B
	 *	@param id ���j���[��ID
	 */
	void SetMenu(int id = 0)
	{
		m_idMenu = id;
		m_hMenu	= NULL;	
	}

	/**
	 * [�ݒ�] ���j���[�ύX�i�n���h���w��j
	 *	@note ����Ńn���h�����w�肷���ID�w��͖����ɂȂ�܂��B
	 *	@param h ���j���[�̃n���h��
	 */
	void SetMenu(HMENU h = NULL)
	{
		m_idMenu = 0;
		m_hMenu	= h;
	}

	/**
	 * [�ݒ�] �A�C�R���ύX
	 *	@param hIcon �A�C�R��
	 */
	void ModifyIcon(HICON hIcon)
	{
		if ( m_boShow )
		{
			m_icondata.hIcon = hIcon;
			m_icondata.ResetFlag();
			m_Modify(m_icondata);
		}
	}

	/**
	 * [�ݒ�] �`�b�v���e�ύX
	 *	@param lpTip ������
	 */
	void ModifyTip(LPCTSTR lpTip)
	{
		if ( m_boShow )
		{
			m_icondata.SetTip(lpTip);
			m_icondata.ResetFlag();
			m_Modify(m_icondata);
		}
	}

#if _WIN32_IE >= 0x0500
	/// �C���t�H���[�V�����A�C�R����
	enum EInfoIcon
	{
		EII_None		= 0x00000000,	///< �A�C�R���Ȃ�
		EII_Info		= 0x00000001,	///< �I�}�[�N
		EII_Warning		= 0x00000002,	///< �x���}�[�N
		EII_Error		= 0x00000003,	///< �G���[�}�[�N
		EII_NsNone		= 0x00000010,	///< �A�C�R���Ȃ��i���Ȃ��j
		EII_NsInfo		= 0x00000011,	///< �I�}�[�N�i���Ȃ��j
		EII_NsWarning	= 0x00000012,	///< �x���}�[�N�i���Ȃ��j
		EII_NsError		= 0x00000013,	///< �G���[�}�[�N�i���Ȃ��j
	};

	/**
	 * [�ݒ�] �C���t�H���[�V�����\��
	 *	@note	�{���\�b�h���g�p����ɂ́A�v���W�F�N�g�ŃV���{�� _WIN32_IE �� 
	 *			0x0500 �ȏ���w�肵�Ă����K�v���L��܂��B
	 *	@param eIcon	�A�C�R���̎�ނ� EInfoIcon ����w�肵�܂��B	
	 *	@param lpszTitle	�^�C�g�������B
	 *	@param lpszMessage	���e�B
	 *	@param dwTimeout	�C���t�H���[�V�����̕��鎞��(ms)
	 */
	void ShowInformation(EInfoIcon eIcon, LPCTSTR lpszTitle, LPCTSTR lpszMessage, DWORD dwTimeout = 0)
	{
		if ( m_boShow )
		{
			CStr s;
			s = lpszTitle;
			STRLIB::Copy(m_icondata.szInfoTitle, s.Left(sizeof(m_icondata.szInfoTitle) - 1));
			s = lpszMessage;
			STRLIB::Copy(m_icondata.szInfo, s.Left(sizeof(m_icondata.szInfo) - 1));
			m_icondata.dwInfoFlags = eIcon;
			m_icondata.uTimeout = dwTimeout;
			//
			m_icondata.uFlags |= 0x00000010/*NIF_INFO*/;
			m_Modify(m_icondata);
			m_icondata.uFlags &= ~0x00000010/*NIF_INFO*/;
		}
	}

	/**
	 * [�ݒ�] �C���t�H���[�V������\��
	 *	@note	�{���\�b�h���g�p����ɂ́A�v���W�F�N�g�ŃV���{�� _WIN32_IE �� 
	 *			0x0500 �ȏ���w�肵�Ă����K�v���L��܂��B
	 */
	void HideInformation(void)
	{
		if ( m_boShow )
		{
			m_icondata.szInfoTitle[0] = 0;
			m_icondata.szInfo[0] = 0;
			m_icondata.dwInfoFlags = EII_None;
			m_icondata.uTimeout = 0;
			//
			m_icondata.uFlags |= 0x00000010/*NIF_INFO*/;
			m_Modify(m_icondata);
			m_icondata.uFlags &= ~0x00000010/*NIF_INFO*/;
		}
	}
#endif

	/**
	 * [�\��] ���j���[�\��.
	 *	@note �o�^���Ă��郁�j���[��\�����܂��B
	 *	@param po ���W
	 *	@param flag �t���O�B TPM_LEFTALIGN �ȂǁB
	 *	@retval 0�ȊO ���j���[�\�������B �t���O�� TPM_RETURNCMD ���w�肵�Ă����ꍇ�A�I�����ꂽ���j���[ID�B
	 *	@retval 0 ���j���[�\�����s�B �t���O�� TPM_RETURNCMD ���w�肵�Ă����ꍇ�A���I���̏ꍇ���O�ɂȂ�B
	 */
	#ifndef _WIN32_WCE
	DWORD ShowMenu(const POINT& po, UINT flag = TPM_LEFTALIGN | TPM_RIGHTBUTTON)
	#else
	DWORD ShowMenu(const POINT& po, UINT flag = TPM_LEFTALIGN)
	#endif
	{
		if ( m_idMenu != 0 )
		{
			HINSTANCE hIns = TNB::GetInstanceHandleByTnb();
			HMENU hMenu = ::LoadMenu(hIns, MAKEINTRESOURCE(m_idMenu));
			if ( hMenu != NULL )
			{
				HMENU h = ::GetSubMenu(hMenu, 0);
				DWORD r = ::TrackPopupMenuEx(h, flag, po.x, po.y, m_icondata.hWnd, NULL);
				::DestroyMenu(h);
				::DestroyMenu(hMenu);
				return r;
			}
		}
		else if ( m_hMenu != NULL )
		{
			return ::TrackPopupMenuEx(m_hMenu, flag, po.x, po.y, m_icondata.hWnd, NULL);
		}
		return 0;
	}

private:

	/**@ingroup ICON
	 * ICON�f�[�^�^�Ǘ��N���X
	 */
	class CIconData
	{
	public:
		DWORD	cbSize;
		HWND	hWnd;
		UINT	uID;
		UINT	uFlags;
		UINT	uCallbackMessage;
		HICON	hIcon;
		#if _WIN32_IE < 0x0500
			TCHAR	szTip[64];
		#else
			TCHAR	szTip[128];
			DWORD	dwState;
			DWORD	dwStateMask;
			TCHAR	szInfo[256];
			union {
				UINT	uTimeout;
				UINT	uVersion;
			} DUMMYUNIONNAME;
			TCHAR	szInfoTitle[64];
			DWORD	dwInfoFlags;
			#if _WIN32_IE >= 0x0600
				GUID guidItem;
			#endif
		#endif
		// �R���X�g���N�^
		CIconData(void)
		{
			Zero(*this);
			cbSize = sizeof(CIconData);
		}
		/// NOTIFYICONDATA�^�擾
		operator NOTIFYICONDATA*(void)
		{
			return reinterpret_cast<NOTIFYICONDATA*>(this);
		}
		/**
		 * Tip�ݒ�
		 *	@param lpTip �`�b�v�ɕ\�����镶����
		 */
		void SetTip(LPCTSTR lpTip)
		{
			CStr s = lpTip;
			STRLIB::Copy(szTip, s.Left(sizeof(szTip) - 1));
		}
		/**
		 * Flag�ݒ�
		 *	@note �ݒ肳��Ă�����ɂ��A�t���O��ݒ肵�܂��B
		 */
		void ResetFlag(void)
		{
			uFlags = NIF_MESSAGE;
			if ( szTip[0] != 0 )
			{
				uFlags |= NIF_TIP;
			}
			if ( hIcon != NULL )
			{
				uFlags |= NIF_ICON;
			}
		}
	};

	/**
	 * �A�C�R���o�^/�ύX
	 */
	void m_Modify(NOTIFYICONDATA* P)
	{
		if ( m_boShow && ::Shell_NotifyIcon(NIM_MODIFY, P) )
		{
			return;
		}
		while ( true )
		{
			if ( ::Shell_NotifyIcon(NIM_ADD, P) )
			{
				break;/*����*/ 
			}
			if ( _GetLastError("Shell_NotifyIcon") != ERROR_TIMEOUT )
			{
				break;/*���s*/ 
			}
			::Sleep(500);
			if ( ::Shell_NotifyIcon(NIM_MODIFY, P) )
			{
				break;/*����*/ 
			}
		}
	}
	//
	CIconData		m_icondata;					///< �^�X�N�A�C�R�����
	int				m_idMenu;					///< �\�����郁�j���[ID
	HMENU			m_hMenu;					///< �\�����郁�j���[�n���h��
	bool			m_boShow;					///< �\�����Ă��邩�H
	IListener*		m_piListener;				///< ���X�i�[
	CSyncCounter	m_syncClick;				///< �}�E�X�N���b�N�����r��
	UINT			m_uTaskbarCreatedMessage;	///< Explorer�̃��u�[�g�ʒm���b�Z�[�W
};



}; // TNB
