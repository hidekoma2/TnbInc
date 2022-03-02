#pragma once
/**
 *@file
 * ウィンドウプロシージャフック関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSimpleMap.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 // HWNDとリスナーのMAP
 _SELECTANY CSimpleMapT<HWND, LPVOID>* s_pmapWphHwndProc = NULL; 
 _SELECTANY bool s_isWphInit = true; 
#endif



/**@ingroup WINAPI
 * ウィンドウプロシージャフッククラス
 *
 *		WindowProc() を継承して、 Attach することで、ウィンドウプロシージャを処理することが可能です。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		class CMyProc : public CWindowProcedureHooker
 *		{
 *			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
 *			{
 *				if ( message == XXX )
 *				{
 *					// 処理を行う。 必要によって、スーパークラスの WindowProcをコールせずに終わってもよし。
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
 *			m_procHook.Attach(*this);	// アタッチする。これ以降、 CMyProcの WindowProcがコールバックされる。		
 *				;
 *
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbWindowProcedureHooker.h
 *
 *	@date 08/10/21 新規作成
 *	@date 09/04/22 マップを CSimpleMap に変更
 *	@date 09/04/23 旧プロシージャコールに ::CallWindowProc() を使うように修正
 *	@date 11/11/29 IsAttached() 新規。
 */
class CWindowProcedureHooker
{
public:
	/// コンストラクタ
	CWindowProcedureHooker(void) : m_hWnd(NULL), m_pOrgWndProc(NULL)
	{
	}

	/// デストラクタ
	~CWindowProcedureHooker(void)
	{
		Destroy();
	}

	/**
	 * [確認] アタッチ中？
	 *	@retval true アタッチ中
	 *	@retval false 否
	 */
	bool IsAttached(void) const
	{
		return m_pOrgWndProc != NULL;
	}

	/**
	 * [設定] アタッチ
	 *	@param hWnd 対象のウィンドウ
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
	 * [設定] デタッチ
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
	 * [設定] 破棄
	 *	@note Detach() とは異なり、 PreDetach() , OnDetached() が呼び出されません。
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
	 * [取得] ウィンドウハンドル取得
	 *	@retval NULL アタッチしていない
	 *	@retval NULL以外 ウィンドウハンドル
	 */
	HWND GetSafeHwnd(void) const
	{
		return m_hWnd;
	}

	/**
	 * [処理] SendMessage.
	 *		アタッチしているウィンドウにSendMessageします。
	 *	@param message メッセージ。
	 *	@param wParam WPARAM。
	 *	@param lParam LPARAM。
	 *	@return 結果。
	 */
	LRESULT SendMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::SendMessage(m_hWnd, message, wParam, lParam);
	}

	/**
	 * [処理] PostMessage.
	 *		アタッチしているウィンドウにPostMessageします。
	 *	@param message メッセージ。
	 *	@param wParam WPARAM。
	 *	@param lParam LPARAM。
	 *	@return 結果。
	 */
	BOOL PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ::PostMessage(m_hWnd, message, wParam, lParam);
	}

	/**
	 * [取得] スタイル取得
	 *	@return スタイル
	 */
	DWORD GetStyle(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_STYLE);
	}

	/**
	 * [取得] 拡張スタイル取得
	 *	@return 拡張スタイル
	 */
	DWORD GetExStyle(void) const
	{
		return ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}

	/**
	 * [変更] スタイル変更
	 *	@param dwRemove 除去されるウィンドウ スタイル
	 *	@param dwAdd 追加されるウィンドウ スタイル
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
	 * [変更] 拡張スタイル変更
	 *	@param dwRemove 除去されるウィンドウ 拡張スタイル
	 *	@param dwAdd 追加されるウィンドウ 拡張スタイル
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
	 * [通知] アタッチ直前通知
	 *	@param hWnd アタッチするHWND。
	 */
	virtual void PreAttach(HWND hWnd) {}
	
	/**
	 * [通知] アタッチ通知
	 */
	virtual void OnAttached(void) {}
	
	/**
	 * [通知] デタッチ直前通知
	 */
	virtual void PreDetach(void) {}

	/**
	 * [通知] デタッチ通知
	 */
	virtual void OnDetached(void) {}

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。これをオーバーライドして処理を追加します。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
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

	HWND				m_hWnd;					///< 管理しているウィンドウ	

private:
	// WNDPROC設定
	WNDPROC m_SetWndProc(HWND hWnd, WNDPROC P)
	{
		LONG_PTR i = reinterpret_cast<LONG_PTR>(P);
		return reinterpret_cast<WNDPROC>(::SetWindowLongPtr(hWnd, GWLP_WNDPROC, i));
	}

	/**
	 * [通知] WndProc 
	 *	@note メッセージプロシージャ
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
	//Unhook処理用サブクラス
	class CReject
	{
	public:	
		/// デストラクタ
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
	WNDPROC				m_pOrgWndProc;			///< フック下元の WndProc
};



}; //TNB
