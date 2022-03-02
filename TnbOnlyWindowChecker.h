#pragma once
/**
 *@file
 * オンリーウィンドウチェック関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * オンリーウィンドウチェッカー.
 *
 *		アプリケーションの多重起動防止に使用します。
 *	
 *	@note ２つ目の起動かチェックし、２つ目なら、１つ目のアプリをアクティブにしてタイトルバーを点滅させます
 *			(この動きは OnFound() をオーバーライドすることで変更可能です)。
 *
 *	\par使用例
 *		\code
 *		BOOL CMainDlg::OnInitDialog()
 *		{
 *			CDialog::OnInitDialog();
 *			// 多重起動チェック
 *			static COnlyWindowChecker s_owc(_T("TnbSampleApp.2011.10.12"));	//static にする
 *			if ( s_owc.IsOnly(GetSafeHwnd()) )
 *			{
 *				EndDialog(true);
 *				return TRUE;
 *			}
 *			// "バージョン情報..." メニュー項目をシステム メニューへ追加します。
 *			ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
 *			ASSERT(IDM_ABOUTBOX < 0xF000);
 *			CMenu* pSysMenu = GetSystemMenu(FALSE);
 *			if (pSysMenu != NULL)
 *				
 *				;
 *				;
 *
 *		\endcode
 *		※ static 宣言せず、 COnlyWindowChecker のインスタンスを CMainDlg のメンバにしてもよい。
 *
 *	@par必要ファイル
 *			TnbOnlyWindowChecker.h
 *
 *	@date 11/10/12 新規作成。
 */
class COnlyWindowChecker : public CCopyImpossible
{
public:
	
	/// デストラクタ
	~COnlyWindowChecker(void)
	{
		m_DeleteMutex();
	}

	/**
	 * コンストラクタ.
	 *	@param lpszUniquelyText 識別用のユニークな文字列.
	 */
	COnlyWindowChecker(LPCTSTR lpszUniquelyText) : m_uniquelyText(lpszUniquelyText), m_pMutex(NULL)
	{
	}

	/**
	 * [確認] チェック.
	 *		同じウィンドウがあるか、チェックします。
	 *	@param hWnd 自分のウィンドウハンドル
	 *	@retval true あった。即時終了する必要あり。
	 *	@retval false なかった。
	 *	@throw CInvalidParamException 設定が異常。 hWnd が正しくないかコンストラクタでユニーク文字列が正しく指定されていないか。
	 */
	bool IsOnly(HWND hWnd)
	{
		if ( hWnd == NULL || ! ::IsWindow(hWnd) || m_uniquelyText.IsEmpty() )
		{
			ASSERT0( false, "COnlyWindowChecker::IsOnly()", "パラメータの設定が不正です。" );
			throw CInvalidParamException();
		}
		m_DeleteMutex();
		m_pMutex = new CSyncMutex(true, m_uniquelyText + _T("_mx"));
		if ( ! m_pMutex->Lock(0) )
		{
			// 誰か使っている
			m_hWnd = hWnd;
			::EnumWindows(ms_EnumFunc, reinterpret_cast<LPARAM>(this));
			return true;
		}
		// 初めて
		VERIFY( ::SetProp(hWnd, m_uniquelyText, reinterpret_cast<HANDLE>(TRUE)) );
		return false;
	}

protected:

	/**
	 * [通知] 同じウィンドウ発見.
	 *	@note 本メソッドでは、ウィンドウをアクティブにし、３回キャプションを点滅させます。
	 *	@param hWnd ウィンドウハンドル
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
	/// MUTEX 破棄
	void m_DeleteMutex(void)
	{
		if ( m_pMutex != NULL )
		{
			delete m_pMutex;
			m_pMutex = NULL;
		}
	}
	/// ENUMコールバック関数
	BOOL m_EnumFunc(HWND hWnd)
	{
		if ( m_hWnd != hWnd )
		{
			HANDLE h = ::GetProp(hWnd, m_uniquelyText);
			if ( h != NULL )
			{
				OnFound(hWnd);
				return FALSE; // enum終わり
			}
		}
		return TRUE;
	}
	/// ENUMコールバック関数
	static BOOL CALLBACK ms_EnumFunc(HWND hWnd, LPARAM lParam)
	{
		COnlyWindowChecker* P = reinterpret_cast<COnlyWindowChecker*>(lParam);
		return P->m_EnumFunc(hWnd);
	}

	CSyncMutex*		m_pMutex;			///< 排他
	CStr			m_uniquelyText;		///< 識別用ユニーク文字
	HWND			m_hWnd;				///< コールバック内用ワーク
};



};//TNB

