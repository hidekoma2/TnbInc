#pragma once
/**
 *@file
 * ファイル選択ダイアログ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#if (_WIN32_WINNT >= 0x0500) && defined(CFileDialog) && defined(__MSVCPP6__)

#undef CFileDialog

// commdlg.h の OPENFILENAME と同じ
struct TOpenFileName
{
   DWORD        lStructSize;
   HWND         hwndOwner;
   HINSTANCE    hInstance;
   LPCTSTR       lpstrFilter;
   LPTSTR        lpstrCustomFilter;
   DWORD        nMaxCustFilter;
   DWORD        nFilterIndex;
   LPTSTR        lpstrFile;
   DWORD        nMaxFile;
   LPTSTR        lpstrFileTitle;
   DWORD        nMaxFileTitle;
   LPCTSTR       lpstrInitialDir;
   LPCTSTR       lpstrTitle;
   DWORD        Flags;
   WORD         nFileOffset;
   WORD         nFileExtension;
   LPCTSTR       lpstrDefExt;
   LPARAM       lCustData;
   LPOFNHOOKPROC lpfnHook;
   LPCTSTR       lpTemplateName;
};

// AfxDlgs.h のそれと同じ
class CFileDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CFileDialog)

public:
// Attributes
	TOpenFileName m_ofn; // open file parameter block

// Constructors
	CFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

// Operations
	virtual int DoModal();

	// Helpers for parsing file name after successful return
	// or during Overridable callbacks if OFN_EXPLORER is set
	CString GetPathName() const;  // return full path and filename
	CString GetFileName() const;  // return only filename
	CString GetFileExt() const;   // return only ext
	CString GetFileTitle() const; // return file title
	BOOL GetReadOnlyPref() const; // return TRUE if readonly checked

	// Enumerating multiple file selections
	POSITION GetStartPosition() const;
	CString GetNextPathName(POSITION& pos) const;

	// Helpers for custom templates
	void SetTemplate(UINT nWin3ID, UINT nWin4ID);
	void SetTemplate(LPCTSTR lpWin3ID, LPCTSTR lpWin4ID);

	// Other operations available while the dialog is visible
	CString GetFolderPath() const; // return full path
	void SetControlText(int nID, LPCSTR lpsz);
	void HideControl(int nID);
	void SetDefExt(LPCSTR lpsz);

// Overridable callbacks
protected:
	friend UINT CALLBACK _AfxCommDlgProc(HWND, UINT, WPARAM, LPARAM);
	virtual UINT OnShareViolation(LPCTSTR lpszPathName);
	virtual BOOL OnFileNameOK();
	virtual void OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);

	// only called back if OFN_EXPLORER is set
	virtual void OnInitDone();
	virtual void OnFileNameChange();
	virtual void OnFolderChange();
	virtual void OnTypeChange();

// Implementation
#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bOpenFileDialog;       // TRUE for file open, FALSE for file save
	CString m_strFilter;          // filter string
						// separate fields with '|', terminate with '||\0'
	TCHAR m_szFileTitle[64];       // contains file title after return
	TCHAR m_szFileName[_MAX_PATH]; // contains full path name after return

	OPENFILENAME*  m_pofnTemp;

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
#endif // _WIN32_WINNT >= 0x0500
#endif //_TnbDOXYGEN



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL FILE
 * ファイル選択コモンダイアログ
 *
 *		文字列管理バッファを拡張します。
 *		複数選択を行なう際に有効。
 *
 *	@par使用例 - 保存先ファイルを指定したい場合
 *		\code
 *		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
 *		LPCTSTR lpszFilter = _T("FISIDファイル (*.fisid)|*.fisid|すべてのファイル (*.*)|*.*||");
 *		CFileDialogEx d(FALSE, _T("*.fisid"), _T(""), dwFlags, lpszFilter, this);
 *		if ( d.DoModal() != IDOK )
 *		{
 *			return;
 *		}
 *		CString fileName = d.GetPathName();
 *			:
 *		\endcode
 *
 *	@par使用例 - 読込元ファイルを指定したい場合
 *		\code
 *		DWORD dwFlags = OFN_HIDEREADONLY;
 *		LPCTSTR lpszFilter = _T("FISIDファイル (*.fisid)|*.fisid|すべてのファイル (*.*)|*.*||");
 *		CFileDialogEx d(TRUE, _T("*.fisid"), _T(""), dwFlags, lpszFilter, this);
 *		if ( d.DoModal() != IDOK )
 *		{
 *			return;
 *		}
 *		CString fileName = d.GetPathName();
 *			:
 *		\endcode
 *
 *	@attention VC++6 の環境で、SDK を入れている場合、 CFileDialog がヘッダとランタイムで
 *		ずれが発生してしまうため、最初に TnbHotWinver.h をインクルードしてください。
 *
 *	@par必要ファイル
 *			TnbMfcFileDialogEx.h
 * 
 *	@date 06/12/01	新規作成
 *	@date 10/01/18	コメント追加
 *	@date 10/04/19	右下のリサイズボックスをHIDEにするようにした。
 *	@date 10/07/12	ある条件でデストラクタで例外が発生する件を修正。
 *	@date 14/05/25	SetInitialPath() を新規追加！
 */
class CFileDialogEx : public CFileDialog
{
	DEFSUPER(CFileDialog);
	enum { E_BufferLength = 100000 };
public:

	/**
	 * コンストラクタ
	 *	@param bOpenFileDialog	[ファイルを開く] ダイアログ ボックスを構築するときは TRUE、
	 *							[ファイル名を付けて保存] ダイアログ ボックスを構築するときは FALSE を設定します。
	 *	@param lpszDefExt	既定のファイル拡張子。ファイル名用のエディット ボックスに、
	 *						ユーザーが拡張子を付けずにファイル名を入力したときは、
	 *						lpszDefExt に指定した拡張子が自動的にファイル名に追加されます。
	 *						このパラメータに NULL を指定すると、拡張子は追加されません。
	 *	@param lpszFileName	ファイル名用のエディット ボックスに初期表示されるファイル名。
	 *						NULL を指定すると、何も表示されません。
	 *	@param dwFlags	ダイアログ ボックスをカスタマイズするためのフラグの 1 つ以上組み合わせ。\n
	 *		<table>
	 *		<tr><td>シンボル</td><td>内容</td></tr><tr><td>
	 *		OFN_READONLY	</td><td>
	 *			読みとり専用のチェックの設定・取得</td></tr><tr><td>
	 *		OFN_OVERWRITEPROMPT</td><td>
	 *			重ね書きの確認ダイアログの表示</td></tr><tr><td>
	 *		OFN_HIDEREADONLY</td><td>
	 *			読みとり専用チェックボックスを表示しない</td></tr><tr><td>
	 *		OFN_NOCHANGEDIR</td><td>
	 *			カレントディレクトリを変更しない</td></tr><tr><td>
	 *		OFN_SHOWHELP</td><td>
	 *			ヘルプボタンを表示する</td></tr><tr><td>
	 *		OFN_ENABLEHOOK</td><td>
	 *			フック関数を使う</td></tr><tr><td>
	 *		OFN_ENABLETEMPLATE</td><td>
	 *			ダイアログテンプレートを使う</td></tr><tr><td>
	 *		OFN_ENABLETEMPLATEHANDLE</td><td>
	 *			ダイアログテンプレートハンドルを使う</td></tr><tr><td>
	 *		OFN_NOVALIDATE</td><td>
	 *			不正ファイル名を許可（フック関数使用時）</td></tr><tr><td>
	 *		OFN_ALLOWMULTISELECT</td><td>
	 *			複数ファイルの選択可。パスと各ファイル名は空白で区切られる</td></tr><tr><td>
	 *		OFN_EXTENSIONDIFFERENT</td><td>
	 *			ユーザがlpstrDefExtと異なる拡張子を入力した</td></tr><tr><td>
	 *		OFN_PATHMUSTEXIST</td><td>
	 *			不正確なパス入力を警告するダイアログを表示する</td></tr><tr><td>
	 *		OFN_FILEMUSTEXIST</td><td>
	 *			既存のファイルのみの入力を許す</td></tr><tr><td>
	 *		OFN_CREATEPROMPT</td><td>
	 *			新しいファイルを作成するのかを確認する</td></tr><tr><td>
	 *		OFN_SHAREAWARE</td><td>
	 *			ネットワークシェア違反のファイル名も許す</td></tr><tr><td>
	 *		OFN_NOREADONLYRETURN</td><td>
	 *			返されたファイルは読みとり専用ではない</td></tr><tr><td>
	 *		OFN_NOTESTFILECREATE</td><td>
	 *			ネットワーク関連（戻り値）</td></tr><tr><td>
	 *		OFN_NONETWORKBUTTON</td><td>
	 *			ネットワークボタンを隠す</td></tr><tr><td>
	 *		OFN_NOLONGNAMES</td><td>
	 *			長い名前を使わない</td></tr><tr><td>
	 *		OFN_EXPLORER</td><td>
	 *			エクスプローラ風の外観</td></tr><tr><td>
	 *		OFN_NODEREFERENCELINKS</td><td>
	 *			ショートカットファイルをそのまま返す</td></tr>
 	 *		</table>
	 *
	 *	@param lpszFilter	ファイルを特定するためのフィルタを指定する文字列の組み合わせ。
	 *						フィルタを指定すると、[ファイル名] ボックスには選択されたファイルだけが表示されるようになります。
	 *	@param pParentWnd	親ウィンドウまたはオーナー ウィンドウへのポインタ。
	 *	@param bVistaStyle TRUE なら　VISTAスタイルで表示。
	 */
	CFileDialogEx(BOOL bOpenFileDialog,
			LPCTSTR lpszDefExt = NULL,
			LPCTSTR lpszFileName = NULL,
			DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			LPCTSTR lpszFilter = NULL,
			CWnd* pParentWnd = NULL,
			BOOL bVistaStyle = FALSE
#if _MSC_VER < 1400
	) : _super(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd) 
#else
	) : _super(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, 0, bVistaStyle) 
#endif
	{
		m_strNameBuffer = lpszFileName;
		m_ofn.nMaxFile = E_BufferLength;
		m_ofn.lpstrFile = m_strNameBuffer.GetBuffer(E_BufferLength + 1);
	}

	/**
	 * [設定] 最初のディレクトリ指定.
	 *	@param lpszPath ディレクトリ
	 *	@return 選択ファイル数。
	 */
	void SetInitialPath(LPCTSTR lpszPath)
	{
		m_initialPath = lpszPath;
		m_ofn.lpstrInitialDir = m_initialPath;
	}

	/**
	 * [取得] 選択フルパス名取得.
	 *	@note 複数選択( OFN_ALLOWMULTISELECT フラグ)をつけた場合、これで選択したファイル、全部のフルパスが取得できます。
	 *	@param[out] _astrNames 選択文字列
	 *	@return 選択ファイル数。
	 */
	INT_PTR GetPathNames(CStringArray& _astrNames)
	{
		_astrNames.RemoveAll();
		POSITION pos = GetStartPosition();
		while ( pos != NULL )
		{
			_astrNames.Add(GetNextPathName(pos));
		}
		return _astrNames.GetSize();
	}

	/**
	 * [取得] 選択フルパス名取得.
	 *	@note 複数選択( OFN_ALLOWMULTISELECT フラグ)をつけた場合、 GetPathNames() を使用してください。
	 *	@return フルパス。
	 */
	CString GetPathName(void) const
	{
		return _super::GetPathName();
	}

protected:

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
		if ( message == WM_INITDIALOG )
		{
			GetNextWindow(GW_HWNDPREV)->ShowWindow(SW_HIDE);
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	CString	m_strNameBuffer;			///< 名前記憶用のバッファ
	CString m_initialPath;				///< パス記憶用
};



}; // MFC
}; // TNB
