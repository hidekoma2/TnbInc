#pragma once
/**
 *@file
 * ダミーウィンドウ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"
#include "TnbSimpleMap.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 // HWNDとリスナーのMAP
 _SELECTANY CSimpleMapT<LPVOID, LPVOID>* s_pmapDmyWndHwndProc = NULL; 
 _SELECTANY CSyncSection* s_psyncDmyWnd = NULL; 

 #ifndef WS_OVERLAPPEDWINDOW
  #define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
 #endif
#endif



/**@ingroup WINAPI
 * ダミーウィンドウクラス
 *
 *		簡易的なウィンドウを作成します。 MFC を使用していません。
 *	
 *	@note CUI アプリでメッセージの受信などでウィンドウハンドルが必要な時、本クラスで見えないウィンドウを作成し、
 *		メッセージの受信処理を行うことが出来ます。
 *
 *	@par必要ファイル
 *			TnbDummyWindow.h
 *
 *	@date 06/06/14	新規作成
 *	@date 09/04/22	マップを CSimpleMap に変更
 *	@date 09/07/22	ブラシ指定可能に変更
 *	@date 10/04/19	作成時、メニューを指定できるようにした。
 *	@date 10/05/10	スタイル関係のメソッド追加。
 *	@date 11/08/31	CreateByTemplate() を追加。
 *	@date 11/09/09	ダイアログ WM_CTLCOLOR～ 系の特殊メッセージに対応。
 *	@date 15/11/20	SELECTANY オブジェクトの破棄の方法を WM_DESTROY 時に移管した。
 */
class CDummyWindow : CCopyImpossible
{
public:

	/**@ingroup WINAPI
	 * ダミーウィンドウクラスのリスナーインターフェース
	 *
	 *	@see CDummyWindow::Create()
	 *
	 *	@par必要ファイル
	 *			TnbDummyWindow.h
	 *
	 *	@date 06/06/14	新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}
		/**
		 * [通知] 受信メッセージ通知.
		 *	@note WM_CREATE は、通知されません。
		 *	@note WM_DESTROY を最後にウィンドウは破棄されます。
		 *	@param[out] _result 結果格納。 true でリターンしたときに使用されます。
		 *	@param[in] hWnd ウィンドウハンドル
		 *	@param[in] message メッセージ
		 *	@param[in] wParam WPARAM
		 *	@param[in] lParam LPARAM
		 *	@retval true 処理済(DefWindowProc()を呼び出さなくなります)
		 *	@retval false 未処理
		 */
		virtual bool OnWindowMessage(LRESULT& _result, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	};


	//----


	/// コンストラクタ
	CDummyWindow(void) : m_hWnd(NULL)
	{
		m_brush = static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
	}

	/// デストラクタ
	~CDummyWindow(void)
	{
		Destroy();
	}

	/**
	 * [取得] ウィンドウハンドル.
	 *		保持しているウィンドウのハンドルを返します。
	 *	@return ハンドル
	 */
	operator HWND(void) const
	{
		return m_hWnd;
	}

	/**
	 * [取得] ウィンドウハンドル.
	 *		保持しているウィンドウのハンドルを返します。
	 *	@return ハンドル
	 */
	HWND GetSafeHwnd(void) const
	{
		return m_hWnd;
	}

	/**
	 * [確認] ウィンドウの有無.
	 *	@retval true 有り
	 *	@retval false なし
	 */
	bool IsWindow(void) const
	{
		return !! ::IsWindow(m_hWnd);
	}

	/**
	 * [操作] ウィンドウの破棄.
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
	 * [設定] 背景ブラシ設定.
	 *		Create() よりも先に指定してください。
	 *	@note CreateByTemplate() を使う場合、本メソッドは意味ありません。
	 *	@param b ブラシ
	 */
	void SetBrush(HBRUSH b)
	{
		m_brush = b;
	}

	/**
	 * [操作] ウィンドウの作成.
	 *		ウィンドウを作成します。
	 *	@note 一つのインスタンスで管理できるウィンドウは一つだけです。
	 *		複数作成しようとすると、先のウィンドウが破棄されます。
	 *	@param I リスナー
	 *	@param lpszTitle ウィンドウのタイトル。省略可能です。FindWindowなどで、
	 *					タイトルが必要な場合指定します。
	 *	@param lpszClassName ウィンドウのクラス名。省略可能です。FindWindowなどで、
	 *					クラス名が必要な場合指定します。
	 *	@param dwStyle ウィンドウスタイル。省略すると、 WS_OVERLAPPEDWINDOW だけが指定されます。
	 *	@param hMenu メニュー。省略すると NULL が指定されます。
	 *	@param hParent 親ウィンドウ。省略すると NULL が指定されます。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [操作] ウィンドウの作成（テンプレート使用）.
	 *		リソースのテンプレートを使用してウィンドウを作成します。
	 *	@note 一つのインスタンスで管理できるウィンドウは一つだけです。
	 *		複数作成しようとすると、先のウィンドウが破棄されます。
	 *	@param I リスナー
	 *	@param templateId ダイアログのリソース点テンプレートＩＤを指定します。
	 *	@param hParent 親ウィンドウ。省略すると NULL が指定されます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool CreateByTemplate(CDummyWindow::IListener* I, UINT templateId, HWND hParent = NULL)
	{
		if ( I != NULL )
		{
			m_CreateSub();
			HINSTANCE hInst = GetInstanceHandleByTnb(EI_Process);
			HANDLE h = ::GetCurrentThread();
			(*s_pmapDmyWndHwndProc)[h] = I;	//仮キー
			m_hWnd = ::CreateDialog(hInst, MAKEINTRESOURCE(templateId), hParent, ms_DlgProc);
			if ( m_hWnd != NULL )
			{
				(*s_pmapDmyWndHwndProc)[m_hWnd] = I;
				s_pmapDmyWndHwndProc->RemoveKey(h);	//仮キー削除
				return true;
			}
		}
		return false;
	}

	/**
	 * [取得] スタイル取得.
	 *	@return スタイル
	 */
	DWORD GetStyle(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_STYLE);
	}

	/**
	 * [取得] 拡張スタイル取得.
	 *	@return 拡張スタイル
	 */
	DWORD GetStyleEx(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}

	/**
	 * [設定] スタイル変更.
	 *	@param dwRemove 除去するスタイル
	 *	@param dwAdd 追加するスタイル
	 *	@param nFlags SetWindowPos に渡すフラグ。SetWindowPos を呼び出さないときは 0 です。既定値は 0 です。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		return m_ModifyStyle(GWL_STYLE, dwRemove, dwAdd, nFlags);
	}

	/**
	 * [設定] 拡張スタイル変更.
	 *	@param dwRemove 除去する拡張スタイル
	 *	@param dwAdd 追加する拡張スタイル
	 *	@param nFlags SetWindowPos に渡すフラグ。SetWindowPos を呼び出さないときは 0 です。既定値は 0 です。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		return m_ModifyStyle(GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
	}

private:

#if 0
	//Unhook処理用サブクラス
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
	/// Createサブ
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
	/// スタイル変更
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
	/// [通知] メッセージ処理.
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
	/// [通知] メッセージ処理.
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
				return r; // WM_CTLCOLOR～ 系などは DWL_MSGRESULT を使わず、戻り値で返す。
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
	/// [登録] ウィンドウクラス登録
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
	/// [登録] ウィンドウクラス解除
	void m_UnregisterClass(void)
	{
		if ( ! m_className.IsEmpty() )
		{
			::UnregisterClass(m_className, GetInstanceHandleByTnb(EI_Process));
			m_className.Empty();
		}
	}
	HWND	m_hWnd;			///< 保持するウィンドウハンドル
	CStr	m_className;	///< 作成したウィンドウクラス名
	HBRUSH	m_brush;		///< 背景ブラシ
};




};///TNB
