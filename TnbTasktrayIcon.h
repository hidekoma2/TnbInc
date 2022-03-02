#pragma once
/**
 *@file
 * タスクトレイアイコン管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
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
 * タスクトレイアイコン管理クラス
 *
 *		アイコンのクリックは、登録したリスナー（ CTasktrayIcon::IListener ）
 *		に通知されます。
 *
 *	@par必須処理
 *		アイコンのクリックをリッスンするには、親のWindowProc をフックし、
 *		本クラスの RelayEvent() をコールする必要が有ります。
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
 *	@note	メニューは最初のポップアップメニュー以下が使用されます。
 *
 *	@note	Information() を使用するには、プロジェクトでシンボル _WIN32_IE に 
 *			0x0500 以上を指定しておく必要が有ります。
 *
 *	@par必要ファイル
 *			TnbTasktrayIcon.h
 *
 *	@date 06/04/17 新規
 *	@date 06/06/01 Explorerがリブートした時にICON再登録するようにした。
 *	@date 06/06/05 MENU IDを登録すると、メニューを自動的に出すようにした。
 *	@date 06/06/20 Information() メソッド、追加。
 *	@date 06/07/25 HMENU も登録できるようにした。
 *	@date 08/04/04 HideInformation を追加。
 */
class CTasktrayIcon : CCopyImpossible
{
public:

	/**@ingroup ICON
	 * タスクバーアイコン管理のリスナーインターフェーズ
	 *
	 *		CTasktrayIcon のコールバック通知用のインターフェースです。
	 *
	 *	@see CTasktrayIcon::SetListener()
	 *
	 *	@par必要ファイル
	 *			TnbTasktrayIcon.h
	 *
	 *	@date	06/04/17	新規
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] アイコン操作通知
		 *	@note アイコンに対してクリックされると通知されます。
		 *	@param uMsg 通知内容。
		 *			WM_LBUTTONDOWN / WM_LBUTTONUP / WM_LBUTTONDBLCLK /
		 *			WM_RBUTTONDOWN / WM_RBUTTONUP / WM_RBUTTONDBLCLK 
		 *			のいずれかです。
		 *	@param tPoint クリックされた座標。
		 */
		virtual void OnNotifyTasktrayIcon(UINT uMsg, const POINT& tPoint) = 0;

	private:
		friend class CTasktrayIcon;	///< フレンドクラス宣言
	};

	/**
	 * コンストラクタ
	 *	@param uID 同じアプリで複数のタスクトレイアイコンを使わないなら省略できます。
	 *			使う場合、インスタンスごとに同じ数字にならないように指定します。
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

	/// デストラクタ
	virtual ~CTasktrayIcon(void)
	{
		Hide();
	}

	/**
	 * [登録] リスナー登録
	 *	@note アイコンに対してのクリックイベントを拾うにはリスナーを登録します。
	 *	@param piListener リスナー
	 */
	void SetListener(IListener* piListener)
	{
		m_piListener = piListener;
	}
	
	/**
	 * [処理] イベント中継
	 *	@note 親ウィンドウの WindowProc() からメッセージを流してください。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@retval true 処理実施。親ウィンドウのメッセージ処理は行う必要がありません。
	 *	@retval false 処理なし。親ウィンドウのメッセージ処理を行ってください。
	 */
	bool RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == m_uTaskbarCreatedMessage )
		{
			//Explorerの再起動など。ICONの再設定が必要
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
	 * [作成] アイコン作成
	 *	@param hWnd 通知するウィンドウハンドル
	 *	@param hIcon 表示するICON
	 *	@param lpTip 表示するチップ内容（省略可能）
	 *	@retval true	成功
	 *	@retval false	失敗
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
	 * [削除] アイコン消去
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
	 * [設定] メニュー変更（ID指定）
	 *	@note これでIDを指定するとハンドル指定は無効になります。
	 *	@param id メニューのID
	 */
	void SetMenu(int id = 0)
	{
		m_idMenu = id;
		m_hMenu	= NULL;	
	}

	/**
	 * [設定] メニュー変更（ハンドル指定）
	 *	@note これでハンドルを指定するとID指定は無効になります。
	 *	@param h メニューのハンドル
	 */
	void SetMenu(HMENU h = NULL)
	{
		m_idMenu = 0;
		m_hMenu	= h;
	}

	/**
	 * [設定] アイコン変更
	 *	@param hIcon アイコン
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
	 * [設定] チップ内容変更
	 *	@param lpTip 文字列
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
	/// インフォメーションアイコン種
	enum EInfoIcon
	{
		EII_None		= 0x00000000,	///< アイコンなし
		EII_Info		= 0x00000001,	///< ！マーク
		EII_Warning		= 0x00000002,	///< 警告マーク
		EII_Error		= 0x00000003,	///< エラーマーク
		EII_NsNone		= 0x00000010,	///< アイコンなし（音なし）
		EII_NsInfo		= 0x00000011,	///< ！マーク（音なし）
		EII_NsWarning	= 0x00000012,	///< 警告マーク（音なし）
		EII_NsError		= 0x00000013,	///< エラーマーク（音なし）
	};

	/**
	 * [設定] インフォメーション表示
	 *	@note	本メソッドを使用するには、プロジェクトでシンボル _WIN32_IE に 
	 *			0x0500 以上を指定しておく必要が有ります。
	 *	@param eIcon	アイコンの種類を EInfoIcon から指定します。	
	 *	@param lpszTitle	タイトル文字。
	 *	@param lpszMessage	内容。
	 *	@param dwTimeout	インフォメーションの閉じる時間(ms)
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
	 * [設定] インフォメーション非表示
	 *	@note	本メソッドを使用するには、プロジェクトでシンボル _WIN32_IE に 
	 *			0x0500 以上を指定しておく必要が有ります。
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
	 * [表示] メニュー表示.
	 *	@note 登録してあるメニューを表示します。
	 *	@param po 座標
	 *	@param flag フラグ。 TPM_LEFTALIGN など。
	 *	@retval 0以外 メニュー表示成功。 フラグに TPM_RETURNCMD を指定していた場合、選択されたメニューID。
	 *	@retval 0 メニュー表示失敗。 フラグに TPM_RETURNCMD を指定していた場合、未選択の場合も０になる。
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
	 * ICONデータ型管理クラス
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
		// コンストラクタ
		CIconData(void)
		{
			Zero(*this);
			cbSize = sizeof(CIconData);
		}
		/// NOTIFYICONDATA型取得
		operator NOTIFYICONDATA*(void)
		{
			return reinterpret_cast<NOTIFYICONDATA*>(this);
		}
		/**
		 * Tip設定
		 *	@param lpTip チップに表示する文字列
		 */
		void SetTip(LPCTSTR lpTip)
		{
			CStr s = lpTip;
			STRLIB::Copy(szTip, s.Left(sizeof(szTip) - 1));
		}
		/**
		 * Flag設定
		 *	@note 設定されている情報により、フラグを設定します。
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
	 * アイコン登録/変更
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
				break;/*成功*/ 
			}
			if ( _GetLastError("Shell_NotifyIcon") != ERROR_TIMEOUT )
			{
				break;/*失敗*/ 
			}
			::Sleep(500);
			if ( ::Shell_NotifyIcon(NIM_MODIFY, P) )
			{
				break;/*成功*/ 
			}
		}
	}
	//
	CIconData		m_icondata;					///< タスクアイコン情報
	int				m_idMenu;					///< 表示するメニューID
	HMENU			m_hMenu;					///< 表示するメニューハンドル
	bool			m_boShow;					///< 表示しているか？
	IListener*		m_piListener;				///< リスナー
	CSyncCounter	m_syncClick;				///< マウスクリック処理排他
	UINT			m_uTaskbarCreatedMessage;	///< Explorerのリブート通知メッセージ
};



}; // TNB
