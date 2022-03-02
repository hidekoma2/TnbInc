#pragma once
/**
 *@file
 * ウィンドウズ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * [Ctrl]+[Alt]+[Del]再現
 *
 *	@note 「 CCtrlAltDel::Exec(); 」で実行します。
 *
 *	@par必要ファイル
 *			TnbWindows.h
 *
 *	@date 06/07/12；新規作成。
 */
class CCtrlAltDel : public CThreadlet::IRunner
{
	/// コンストラクタ
	CCtrlAltDel(void) {}

	/// スレッド実行部
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
	 * [実行] [Ctrl]+[Alt]+[Del]再現
	 */
	static void Exec(void)
	{
		CThreadlet::Start(0, new CCtrlAltDel);
	}
};



/**@ingroup WINAPI
 * シャットダウン
 *
 *	@note シャットダウンは「 CPowerManager::Shutdown(); 」で実行します。
 *	@note サスペンドは「 CPowerManager::Suspend(); 」で実行します。
 *
 *	@par●シャットダウンに付いて
 *		<table><tr><td><pre>
 *		実行すると、ウィンドウを持つアプリに、 WM_QUERYENDSESSION, 0, 0 を通知(SendMessage)します。
 *		アプリは、このメッセージにより、終了しても良い状態にします。もし、終了したくない場合、
 *		このメッセージの応答で FALSE を返します。
 *
 *		続いて WM_ENDSESSION が通知されます。 wParam が FALSE なら、誰かがシャットダウンを拒否したため、
 *		シャットダウンはキャンセルされたことを意味します。
 *		lParam が TURE ならログオフしようとしている時、FALSEならシャットダウンしようとしている時を意味します。
 *		</pre></td></tr></table>
 *
 *	@par●サスペンドに付いて
 *		<table><tr><td><pre>
 *		実行すると、ウィンドウを持つアプリに、 WM_POWERBROADCAST, wParam, 0 を通知(SendMessage)します。
 *		wParam は、以下のようになっています。
 *		・PBT_APMQUERYSUSPEND ; 
 *							サスペンド問合せ。応答で、 BROADCAST_QUERY_DENY を返すことで、
 *							サスペンドを拒否できます。全アプリが TRUEを返すとサスペンド処理に入ります。\n
 *							※VISTA では発行されません。
 *		・PBT_APMQUERYSUSPENDFAILED ；
 *							サスペンド問い合わせで、一つでも BROADCAST_QUERY_DENY を返した時(拒否した時)、
 *							サスペンドキャンセルを、全部アプリに通知されます。\n
 *							※VISTA では発行されません。
 *		・PBT_APMSUSPEND ; 
 *							サスペンド開始通知。拒否できません。
 *		・PBT_APMRESUMESUSPEND ;
 *							復帰通知(レジューム)。
 *		・PBT_APMRESUMECRITICAL；
 *							クリティカルスリープからの復帰通知。上記サスペンドのメッセージなしにサスペンド
 *							せざる得なかった時(電池切れなど)、復帰時に通知されます。
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbWindows.h
 *
 *	@date 06/10/24；新規作成。
 */
class CPowerManager
{
	/// コンストラクタ
	CPowerManager(void)
	{
	}

	/// 指定権限追加
	static bool ms_AddAuthority(LPCTSTR lpName = SE_SHUTDOWN_NAME)
	{
		// アクセストークンを開く
		HANDLE hHandle;
		if ( ! ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hHandle) )
		{
			return false;
		}
		TOKEN_PRIVILEGES token;
		token.PrivilegeCount = 1;	// 数は1つ
		if ( ! ::LookupPrivilegeValue(NULL, lpName, &(token.Privileges[0].Luid)) )
		{
			::CloseHandle(hHandle);
			return false;
		}
		token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // 有効
		if ( ! ::AdjustTokenPrivileges(hHandle, FALSE, &token, 0, NULL, NULL) )
		{
			::CloseHandle(hHandle);
			return false;
		}
		// アクセストークンを閉じる
		return !! ::CloseHandle(hHandle);
	}

	/// シャットダウン権限追加
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
	 * [実行] シャットダウン
	 *	@note Windows95系では、このメソッドのみ使用可能です。
	 *	@param uFlags シャットダウンフラグ。省略すると EWX_POWEROFF。\n
	 *						EWX_LOGOFF -	呼び出し側のプロセスのセキュリティコンテキストで実行されているすべてのプロセスを終了し、現在のユーザーをログオフさせます。 \n
	 *						EWX_POWEROFF -	システムをシャットダウンした後、電源を切ります。システムは、パワーオフ機能をサポートしていなければなりません。 \n
	 *						EWX_REBOOT -	システムをシャットダウンした後、システムを再起動します。 \n
	 *						EWX_SHUTDOWN -	システムをシャットダウンして、電源を切っても安全な状態にします。すべてのバッファをディスクへフラッシュし（バッファの内容をディスクに書き込み）、動作していたすべてのプロセスを停止します。 \n
	 *					上記シンボルに加えて or で以下のシンボルも指定できます。\n
	 *						EWX_FORCE -		プロセスを強制的に終了させます。このフラグを指定すると、システムは、現在実行されているアプリケーションへ WM_QUERYENDSESSION メッセージや WM_ENDSESSION メッセージを送信しません。この結果、アプリケーションがデータを失う可能性もあります。したがって、このフラグは、緊急時にのみ指定してください。 \n
	 *						EWX_FORCEIFHUNG -	プロセスが WM_QUERYENDSESSION または WM_ENDSESSION メッセージに応答しない場合、それらのプロセスを終了させます。EWX_FORCE フラグを指定すると、EWX_FORCEIFHUNG フラグは無視されます。 (Windows 2000以上のみ有効)
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
	 * [実行] シャットダウン
	 *	@note WindowsNT系専用です。
	 *	@param lpMachineName		コンピュータ名。NULLならLOCALが対象。
	 *	@param lpMessage			表示するべきメッセージ。NULLなら表示しません。
	 *	@param dwTimeout			表示時間(秒)。
	 *	@param bForceAppsClosed		強制終了オプション。
	 *						TRUE を指定すると、強制的に終了させます。
	 *						FALSE を指定すると、アプリケーションを終了するようユーザーに指示する
	 *						ダイアログボックスが表示されます。 
	 *	@param bRebootAfterShutdown	再起動オプション。
	 *						TRUE を指定すると、コンピュータは再起動します。
	 *						FALSE を指定すると、コンピュータは、電源を切っても安全な状態になったことを示す
	 *						メッセージを表示します。
	 *	@retval true 成功。シャットダウン開始。
	 *	@retval false 失敗。
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
	 * [実行] シャットダウン
	 *	@param bForceAppsClosed		強制終了オプション。
	 *						TRUE を指定すると、強制的に終了させます。
	 *						FALSE を指定すると、アプリケーションを終了するようユーザーに指示する
	 *						ダイアログボックスが表示されます。 
	 *	@param bRebootAfterShutdown	再起動オプション。
	 *						TRUE を指定すると、コンピュータは再起動します。
	 *						FALSE を指定すると、コンピュータは、電源を切っても安全な状態になったことを示す
	 *						メッセージを表示します。
	 *	@retval true 成功。シャットダウン開始。
	 *	@retval false 失敗。
	 */
	static bool Shutdown(bool bForceAppsClosed = false, bool bRebootAfterShutdown = true)
	{
		bool r;
		OSVERSIONINFO tOsVerInfo = { sizeof(OSVERSIONINFO) };
		::GetVersionEx(&tOsVerInfo);
		if ( tOsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		{
			//Windows95/98系
			UINT uFlag = bRebootAfterShutdown ? EWX_REBOOT : EWX_POWEROFF;
			if ( bForceAppsClosed )
			{
				uFlag |= EWX_FORCE;
			}
			r = Shutdown95(uFlag);
		}
		else
		{
			//WindowsNT系
			r = ShutdownNt(NULL, NULL, 0, bForceAppsClosed, bRebootAfterShutdown);
		}
		return r;
	}

	/**
	 * [実行] シャットダウン中止.
	 *		Exec() で開始したシャットダウンを停止できます。ただし、表示時間(秒)以内に本メソッドを
	 *		実行する必要があります。
	 *	@note WindowsNT系専用です。
	 *	@param lpMachineName	シャットダウン中止するコンピュータ名。NULLならLOCALが対象。
	 *	@retval true 成功。シャットダウンはキャンセル。
	 *	@retval false 失敗。
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
	 * [実行] サスペンド
	 *	@param fSuspend true を指定すると サスペンド、false を指定すると､システムが休止状態に入ります。 
	 *	@param fForce \n
	 *			true を指定すると、関数は各アプリケーションやドライバに PBT_APMSUSPEND イベントをブロードキャストした後、直ちに操作を中断します。\n
	 *			false を指定すると、関数は各アプリケーションに PBT_APMQUERYSUSPEND イベントをブロードキャストし、操作を中断するためのアクセス権を要求します。 
	 *	@retval true 成功。(サスペンドしてレジュームしたことを表す) 。
	 *	@retval false 失敗。
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
	 * [取得] システム電源状態取得
	 *	@param[out] _lpSystemPowerStatus 情報
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool GetStatus(LPSYSTEM_POWER_STATUS _lpSystemPowerStatus)
	{
		return !! ::GetSystemPowerStatus(_lpSystemPowerStatus);
	}

	/**
	 * [設定] シャットダウンパラメータ設定
	 *	@note システム内の他のプロセスから見た相対的なシャットダウンの順序を設定します。
	 *		設定しないと中間になっています。
	 *	@note WindowsNT系、専用です。
	 *	@param level レベル。 0x000～0x4FF を指定可能。 \n
	 *		0x000～0x0FF	最後の方でシャットダウンするシステムの構成要素用	\n
	 *		0x100～0x1FF	最後の方でシャットダウンするアプリケーション用		\n
	 *		0x200～0x2FF	“中間”でシャットダウンするアプリケーション用		\n
	 *		0x300～0x3FF	最初の方でシャットダウンするアプリケーション用		\n
	 *		0x400～0x4FF	最初の方でシャットダウンするシステムの構成要素用
	 *	@retval true 成功。
	 *	@retval false 失敗。拡張エラー情報を取得するには、 GetLastError() を使います。
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
