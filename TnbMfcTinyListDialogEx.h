#pragma once
/**
 *@file
 * Listウィンドウ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcTinyListDialog.h"
#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 拡張簡易リストウィンドウ
 *
 *		行単位で色を指定出来る ListBoxコントロールを画面一面にもつモードレスダイアログです。。
 *
 *		CTinyListDialog に比べ、本画面のメッセージキューを別途用意しているため、
 *		メインダイアログでメッセージポンプを動かさずに長時間ログを出力しても、
 *		本画面の更新が行われます。
 *
 *	@note 親ウィンドウの表示非表示メニューIDを Create() で指定すると、Show Hide により自動的に表示チェックが on off します。
 *			ただし、メニューアイテム選択時の Show Hide は自動化できません。親ダイアログのハンドラで 
 *			xxx.ShowWindow(CTinyListDialog::TOGGLE); としてください。
 *			\code
 *				// CTinyListDialogEx m_log;
 *				void CFooDlg::OnIniDialog(void)
 *				{
 *						;
 *					m_log.Create(CSize(100, 100), GetSafeHwnd(), "title", m_hIcon, ID_MENU_LIST_VIEW);
 *						;
 *				}
 *				void CFooDlg::OnMenuListView(void)
 *				{
 *					m_log.ShowWindow(CTinyListDialog::TOGGLE);
 *				}
 *			\endcode
 *			メニューIDを指定していない場合、 PostString() で表示されるようになります。
 *
 *	@note 以下のようにメイン画面で LOG 関数を用意しておくと便利です。
 *			\code
 *				enum ELogType
 *				{
 *					RECV	= RGB(204, 255, 204),
 *					SEND	= RGB(204, 255, 255),
 *					NORMAL	= RGB(255, 255, 255),
 *					ERR		= RGB(255,153,153),
 *				};
 *
 *				void CFooDlg::Log(ELogType tt, LPCTSTR lpszFmt, ...)
 *				{
 *					va_list args;
 *					va_start(args, lpszFmt);
 *					CString s;
 *					s.FormatV(lpszFmt, args); 
 *					va_end(args);
 *					m_log.PostString(-1, s, RGB(0, 0, 0), tt);
 *				}
 *
 *				void CFooDlg::Log(LPCTSTR lpszFmt, ...)
 *				{
 *					va_list args;
 *					va_start(args, lpszFmt);
 *					CString s;
 *					s.FormatV(lpszFmt, args); 
 *					va_end(args);
 *					m_log.PostString(-1, s, RGB(0, 0, 0), NORMAL);
 *				}
 *			\endcode
 *
 *	@note 継承して メニュー項目を変更することも出来ます。
 *
 *	@see CColorListBox
 *	@see CTinyListDialog
 *
 *	@par必要ファイル
 *			TnbMfcTinyListDialogEx.h
 * 
 *	@date 11/02/03	新規作成
 */
class CTinyListDialogEx : public CTinyListDialog, CThreadDispatch::IListener
{
	DEFSUPER(CTinyListDialog);
public:

	/**
	 * コンストラクタ.
	 *	@param isColorMode true ならカラーモード。 false なら白黒モードです。
	 */
	CTinyListDialogEx(bool isColorMode = true) : _super(isColorMode)
	{
	}

	/**
	 * [作成] ウィンドウ作成.
	 *	@note 非表示状態で作成されます。
	 *	@param size ウィンドウ大きさ
	 *	@param hFont フォント
	 *	@param hParentWnd 親ウィンドウ
	 *	@param lpszTitle ウィンドウタイトル
	 *	@param hIcon ウィンドウアイコン
	 *	@param parentMenuItem 親ウィンドウの表示非表示メニュー。省略可能。
	 *	@retval true 作成成功
	 *	@retval false 失敗
	 */
	bool Create(const CSize& size, HFONT hFont, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		m_size = size;
		m_hFont = hFont;
		m_hParentWnd = hParentWnd;
		m_title = lpszTitle;
		m_hIcon = hIcon;
		m_parentMenuItem = parentMenuItem;
		m_thread.SetListener(this);
		#ifdef _DEBUG
			m_thread.Start(_T("TinyListDialogEx::MessagePump"));
		#else
			m_thread.Start();
		#endif
		return IsWindow();
	}

	/**
	 * [作成] ウィンドウ作成.
	 *	@note 非表示状態で作成されます。
	 *	@param size ウィンドウ大きさ
	 *	@param hParentWnd 親ウィンドウ
	 *	@param lpszTitle ウィンドウタイトル
	 *	@param hIcon ウィンドウアイコン
	 *	@param parentMenuItem 親ウィンドウの表示非表示メニュー。省略可能。
	 *	@retval true 作成成功
	 *	@retval false 失敗
	 */
	bool Create(const CSize& size, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hParentWnd, WM_GETFONT, 0, 0));
		return Create(size, hFont, hParentWnd, lpszTitle, hIcon, parentMenuItem);
	}

	/**
	 * [操作] ウィンドウの破棄.
	 */
	void Destroy(void)
	{
		_super::Destroy();
		m_thread.Stop();
	}

protected:

	/**
	 * [通知] 開始通知.
	 *	受信スレッドが開始されると、通知されます。
	 */
	virtual void OnThreadDispatchSetup(void) 
	{
		_super::Create(m_size, m_hFont, m_hParentWnd, m_title, m_hIcon, m_parentMenuItem);
	}

	/**
	 * [通知] メッセージ受信.
	 *		最後は必ず nMsg に WM_QUIT が入って通知されます。
	 *	@param nMsg メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@retval true	正常
	 *	@retval false	切断要求
	 */
	virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		return true;
	}

private:
	CSize				m_size;				///< サイズ
	HFONT				m_hFont;			///< フォント
	HWND				m_hParentWnd;		///< 親ウィンドウ
	CStr				m_title;			///< タイトル
	HICON				m_hIcon;			///< アイコン
	UINT				m_parentMenuItem;	///< 親のメニューアイテム
	CThreadDispatch		m_thread;			///< スレッド
};



}; // MFC
}; // TNB
