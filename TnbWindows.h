#pragma once
/**
 *@file
 * �E�B���h�E�Y�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * [Ctrl]+[Alt]+[Del]�Č�
 *
 *	@note �u CCtrlAltDel::Exec(); �v�Ŏ��s���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbWindows.h
 *
 *	@date 06/07/12�G�V�K�쐬�B
 */
class CCtrlAltDel : public CThreadlet::IRunner
{
	/// �R���X�g���N�^
	CCtrlAltDel(void) {}

	/// �X���b�h���s��
	virtual void Run(LPARAM lParam)
	{
		HDESK old_desktop = ::GetThreadDesktop(::GetCurrentThreadId());
		DWORD dwDesiredAccess =		 DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW
									| DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL
									| DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS
									| DESKTOP_SWITCHDESKTOP | GENERIC_WRITE;
		HDESK h = ::OpenDesktop(_T("Winlogon"), 0, FALSE, dwDesiredAccess);
		if ( ::SetThreadDesktop(h) ) 
		{
			::PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
		}
		::SetThreadDesktop(old_desktop);
	}
public:
	/**
	 * [���s] [Ctrl]+[Alt]+[Del]�Č�
	 */
	static void Exec(void)
	{
		CThreadlet::Start(0, new CCtrlAltDel);
	}
};



/**@ingroup WINAPI
 * �V���b�g�_�E��
 *
 *	@note �V���b�g�_�E���́u CPowerManager::Shutdown(); �v�Ŏ��s���܂��B
 *	@note �T�X�y���h�́u CPowerManager::Suspend(); �v�Ŏ��s���܂��B
 *
 *	@par���V���b�g�_�E���ɕt����
 *		<table><tr><td><pre>
 *		���s����ƁA�E�B���h�E�����A�v���ɁA WM_QUERYENDSESSION, 0, 0 ��ʒm(SendMessage)���܂��B
 *		�A�v���́A���̃��b�Z�[�W�ɂ��A�I�����Ă��ǂ���Ԃɂ��܂��B�����A�I���������Ȃ��ꍇ�A
 *		���̃��b�Z�[�W�̉����� FALSE ��Ԃ��܂��B
 *
 *		������ WM_ENDSESSION ���ʒm����܂��B wParam �� FALSE �Ȃ�A�N�����V���b�g�_�E�������ۂ������߁A
 *		�V���b�g�_�E���̓L�����Z�����ꂽ���Ƃ��Ӗ����܂��B
 *		lParam �� TURE �Ȃ烍�O�I�t���悤�Ƃ��Ă��鎞�AFALSE�Ȃ�V���b�g�_�E�����悤�Ƃ��Ă��鎞���Ӗ����܂��B
 *		</pre></td></tr></table>
 *
 *	@par���T�X�y���h�ɕt����
 *		<table><tr><td><pre>
 *		���s����ƁA�E�B���h�E�����A�v���ɁA WM_POWERBROADCAST, wParam, 0 ��ʒm(SendMessage)���܂��B
 *		wParam �́A�ȉ��̂悤�ɂȂ��Ă��܂��B
 *		�EPBT_APMQUERYSUSPEND ; 
 *							�T�X�y���h�⍇���B�����ŁA BROADCAST_QUERY_DENY ��Ԃ����ƂŁA
 *							�T�X�y���h�����ۂł��܂��B�S�A�v���� TRUE��Ԃ��ƃT�X�y���h�����ɓ���܂��B\n
 *							��VISTA �ł͔��s����܂���B
 *		�EPBT_APMQUERYSUSPENDFAILED �G
 *							�T�X�y���h�₢���킹�ŁA��ł� BROADCAST_QUERY_DENY ��Ԃ�����(���ۂ�����)�A
 *							�T�X�y���h�L�����Z�����A�S���A�v���ɒʒm����܂��B\n
 *							��VISTA �ł͔��s����܂���B
 *		�EPBT_APMSUSPEND ; 
 *							�T�X�y���h�J�n�ʒm�B���ۂł��܂���B
 *		�EPBT_APMRESUMESUSPEND ;
 *							���A�ʒm(���W���[��)�B
 *		�EPBT_APMRESUMECRITICAL�G
 *							�N���e�B�J���X���[�v����̕��A�ʒm�B��L�T�X�y���h�̃��b�Z�[�W�Ȃ��ɃT�X�y���h
 *							�����链�Ȃ�������(�d�r�؂�Ȃ�)�A���A���ɒʒm����܂��B
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbWindows.h
 *
 *	@date 06/10/24�G�V�K�쐬�B
 */
class CPowerManager
{
	/// �R���X�g���N�^
	CPowerManager(void)
	{
	}

	/// �w�茠���ǉ�
	static bool ms_AddAuthority(LPCTSTR lpName = SE_SHUTDOWN_NAME)
	{
		// �A�N�Z�X�g�[�N�����J��
		HANDLE hHandle;
		if ( ! ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hHandle) )
		{
			return false;
		}
		TOKEN_PRIVILEGES token;
		token.PrivilegeCount = 1;	// ����1��
		if ( ! ::LookupPrivilegeValue(NULL, lpName, &(token.Privileges[0].Luid)) )
		{
			::CloseHandle(hHandle);
			return false;
		}
		token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // �L��
		if ( ! ::AdjustTokenPrivileges(hHandle, FALSE, &token, 0, NULL, NULL) )
		{
			::CloseHandle(hHandle);
			return false;
		}
		// �A�N�Z�X�g�[�N�������
		return !! ::CloseHandle(hHandle);
	}

	/// �V���b�g�_�E�������ǉ�
	static bool ms_AddShutdownAuthority(LPCTSTR lpMachineName)
	{
		LPCTSTR P = SE_REMOTE_SHUTDOWN_NAME;
		if ( lpMachineName == NULL || *lpMachineName == 0 )
		{
			P = SE_SHUTDOWN_NAME;
		}
		return ms_AddAuthority(P);
	}

public:

	/**
	 * [���s] �V���b�g�_�E��
	 *	@note Windows95�n�ł́A���̃��\�b�h�̂ݎg�p�\�ł��B
	 *	@param uFlags �V���b�g�_�E���t���O�B�ȗ������ EWX_POWEROFF�B\n
	 *						EWX_LOGOFF -	�Ăяo�����̃v���Z�X�̃Z�L�����e�B�R���e�L�X�g�Ŏ��s����Ă��邷�ׂẴv���Z�X���I�����A���݂̃��[�U�[�����O�I�t�����܂��B \n
	 *						EWX_POWEROFF -	�V�X�e�����V���b�g�_�E��������A�d����؂�܂��B�V�X�e���́A�p���[�I�t�@�\���T�|�[�g���Ă��Ȃ���΂Ȃ�܂���B \n
	 *						EWX_REBOOT -	�V�X�e�����V���b�g�_�E��������A�V�X�e�����ċN�����܂��B \n
	 *						EWX_SHUTDOWN -	�V�X�e�����V���b�g�_�E�����āA�d����؂��Ă����S�ȏ�Ԃɂ��܂��B���ׂẴo�b�t�@���f�B�X�N�փt���b�V�����i�o�b�t�@�̓��e���f�B�X�N�ɏ������݁j�A���삵�Ă������ׂẴv���Z�X���~���܂��B \n
	 *					��L�V���{���ɉ����� or �ňȉ��̃V���{�����w��ł��܂��B\n
	 *						EWX_FORCE -		�v���Z�X�������I�ɏI�������܂��B���̃t���O���w�肷��ƁA�V�X�e���́A���ݎ��s����Ă���A�v���P�[�V������ WM_QUERYENDSESSION ���b�Z�[�W�� WM_ENDSESSION ���b�Z�[�W�𑗐M���܂���B���̌��ʁA�A�v���P�[�V�������f�[�^�������\��������܂��B���������āA���̃t���O�́A�ً}���ɂ̂ݎw�肵�Ă��������B \n
	 *						EWX_FORCEIFHUNG -	�v���Z�X�� WM_QUERYENDSESSION �܂��� WM_ENDSESSION ���b�Z�[�W�ɉ������Ȃ��ꍇ�A�����̃v���Z�X���I�������܂��BEWX_FORCE �t���O���w�肷��ƁAEWX_FORCEIFHUNG �t���O�͖�������܂��B (Windows 2000�ȏ�̂ݗL��)
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool Shutdown95(UINT uFlags = EWX_POWEROFF)
	{
		if ( ! ms_AddAuthority() )
		{
			return false;
		}
		return !! ::ExitWindowsEx(uFlags, 0);
	}

	/**
	 * [���s] �V���b�g�_�E��
	 *	@note WindowsNT�n��p�ł��B
	 *	@param lpMachineName		�R���s���[�^���BNULL�Ȃ�LOCAL���ΏہB
	 *	@param lpMessage			�\������ׂ����b�Z�[�W�BNULL�Ȃ�\�����܂���B
	 *	@param dwTimeout			�\������(�b)�B
	 *	@param bForceAppsClosed		�����I���I�v�V�����B
	 *						TRUE ���w�肷��ƁA�����I�ɏI�������܂��B
	 *						FALSE ���w�肷��ƁA�A�v���P�[�V�������I������悤���[�U�[�Ɏw������
	 *						�_�C�A���O�{�b�N�X���\������܂��B 
	 *	@param bRebootAfterShutdown	�ċN���I�v�V�����B
	 *						TRUE ���w�肷��ƁA�R���s���[�^�͍ċN�����܂��B
	 *						FALSE ���w�肷��ƁA�R���s���[�^�́A�d����؂��Ă����S�ȏ�ԂɂȂ������Ƃ�����
	 *						���b�Z�[�W��\�����܂��B
	 *	@retval true �����B�V���b�g�_�E���J�n�B
	 *	@retval false ���s�B
	 */
	static bool ShutdownNt(LPCTSTR lpMachineName, LPCTSTR lpMessage, DWORD dwTimeout,
						bool bForceAppsClosed = false, bool bRebootAfterShutdown = true)
	{
		if ( ! ms_AddShutdownAuthority(lpMachineName) )
		{
			return false;
		}
		BOOL r = ::InitiateSystemShutdown(
					CSimpleStr(lpMachineName), CSimpleStr(lpMessage), dwTimeout, bForceAppsClosed, bRebootAfterShutdown);
		return !! r;
	}

	/**
	 * [���s] �V���b�g�_�E��
	 *	@param bForceAppsClosed		�����I���I�v�V�����B
	 *						TRUE ���w�肷��ƁA�����I�ɏI�������܂��B
	 *						FALSE ���w�肷��ƁA�A�v���P�[�V�������I������悤���[�U�[�Ɏw������
	 *						�_�C�A���O�{�b�N�X���\������܂��B 
	 *	@param bRebootAfterShutdown	�ċN���I�v�V�����B
	 *						TRUE ���w�肷��ƁA�R���s���[�^�͍ċN�����܂��B
	 *						FALSE ���w�肷��ƁA�R���s���[�^�́A�d����؂��Ă����S�ȏ�ԂɂȂ������Ƃ�����
	 *						���b�Z�[�W��\�����܂��B
	 *	@retval true �����B�V���b�g�_�E���J�n�B
	 *	@retval false ���s�B
	 */
	static bool Shutdown(bool bForceAppsClosed = false, bool bRebootAfterShutdown = true)
	{
		bool r;
		OSVERSIONINFO tOsVerInfo = { sizeof(OSVERSIONINFO) };
		::GetVersionEx(&tOsVerInfo);
		if ( tOsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		{
			//Windows95/98�n
			UINT uFlag = bRebootAfterShutdown ? EWX_REBOOT : EWX_POWEROFF;
			if ( bForceAppsClosed )
			{
				uFlag |= EWX_FORCE;
			}
			r = Shutdown95(uFlag);
		}
		else
		{
			//WindowsNT�n
			r = ShutdownNt(NULL, NULL, 0, bForceAppsClosed, bRebootAfterShutdown);
		}
		return r;
	}

	/**
	 * [���s] �V���b�g�_�E�����~.
	 *		Exec() �ŊJ�n�����V���b�g�_�E�����~�ł��܂��B�������A�\������(�b)�ȓ��ɖ{���\�b�h��
	 *		���s����K�v������܂��B
	 *	@note WindowsNT�n��p�ł��B
	 *	@param lpMachineName	�V���b�g�_�E�����~����R���s���[�^���BNULL�Ȃ�LOCAL���ΏہB
	 *	@retval true �����B�V���b�g�_�E���̓L�����Z���B
	 *	@retval false ���s�B
	 */
	static bool AbortShutdown(LPCTSTR lpMachineName)
	{
		if ( ! ms_AddShutdownAuthority(lpMachineName) )
		{
			return false;
		}
		BOOL r = ::AbortSystemShutdown(CSimpleStr(lpMachineName));
		return !! r;
	}

	/**
	 * [���s] �T�X�y���h
	 *	@param fSuspend true ���w�肷��� �T�X�y���h�Afalse ���w�肷��Ƥ�V�X�e�����x�~��Ԃɓ���܂��B 
	 *	@param fForce \n
	 *			true ���w�肷��ƁA�֐��͊e�A�v���P�[�V������h���C�o�� PBT_APMSUSPEND �C�x���g���u���[�h�L���X�g������A�����ɑ���𒆒f���܂��B\n
	 *			false ���w�肷��ƁA�֐��͊e�A�v���P�[�V������ PBT_APMQUERYSUSPEND �C�x���g���u���[�h�L���X�g���A����𒆒f���邽�߂̃A�N�Z�X����v�����܂��B 
	 *	@retval true �����B(�T�X�y���h���ă��W���[���������Ƃ�\��) �B
	 *	@retval false ���s�B
	 */
	static bool Suspend(bool fSuspend = true, bool fForce = false)
	{
		if ( ! ms_AddAuthority() )
		{
			return false;
		}
		return !! ::SetSystemPowerState(fSuspend, fForce);
	}

	/**
	 * [�擾] �V�X�e���d����Ԏ擾
	 *	@param[out] _lpSystemPowerStatus ���
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool GetStatus(LPSYSTEM_POWER_STATUS _lpSystemPowerStatus)
	{
		return !! ::GetSystemPowerStatus(_lpSystemPowerStatus);
	}

	/**
	 * [�ݒ�] �V���b�g�_�E���p�����[�^�ݒ�
	 *	@note �V�X�e�����̑��̃v���Z�X���猩�����ΓI�ȃV���b�g�_�E���̏�����ݒ肵�܂��B
	 *		�ݒ肵�Ȃ��ƒ��ԂɂȂ��Ă��܂��B
	 *	@note WindowsNT�n�A��p�ł��B
	 *	@param level ���x���B 0x000�`0x4FF ���w��\�B \n
	 *		0x000�`0x0FF	�Ō�̕��ŃV���b�g�_�E������V�X�e���̍\���v�f�p	\n
	 *		0x100�`0x1FF	�Ō�̕��ŃV���b�g�_�E������A�v���P�[�V�����p		\n
	 *		0x200�`0x2FF	�g���ԁh�ŃV���b�g�_�E������A�v���P�[�V�����p		\n
	 *		0x300�`0x3FF	�ŏ��̕��ŃV���b�g�_�E������A�v���P�[�V�����p		\n
	 *		0x400�`0x4FF	�ŏ��̕��ŃV���b�g�_�E������V�X�e���̍\���v�f�p
	 *	@retval true �����B
	 *	@retval false ���s�B�g���G���[�����擾����ɂ́A GetLastError() ���g���܂��B
	 */
	static bool SetShutdownNotifyLevel(WORD level)
	{
		DWORD l;
		DWORD f;
		if ( ::GetProcessShutdownParameters(&l, &f) )
		{
			return !! ::SetProcessShutdownParameters(level, f);
		}
		return false;
	}
};



};//TNB
