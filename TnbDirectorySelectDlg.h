#pragma once
/**
 *@file
 * ディレクトリ選択ダイアログ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include <shlobj.h>



//TNB Library
namespace TNB
{
	
	
	
/**@ingroup FILE MFCCONTROL
 * ディレクトリ選択ダイアログ
 *	
 *		ディレクトリ選択ダイアログを表示します。
 *
 *	@par必要ファイル
 *			TnbDirectorySelectDlg.h
 *
 *	@date 08/03/26 新規作成
 */
class CDirectorySelectDlg
{
public:

	/**@ingroup FILE
	 * ディレクトリ選択ダイアログリスナー
	 *
	 *	@see CDirectorySelectDlg
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] ダイアログ初期化通知.
		 *	@note ダイアログが表示される前に通知されます。
		 *	@param hWnd フォルダ選択ダイアログのウィンドウハンドル
		 */
		virtual void OnDirectorySelectDlgInitialized(HWND hWnd) = 0;

		/**
		 * [通知] 選択変更通知.
		 *	@note ダイアログが表示される前にも通知される可能性があります。
		 *	@param hWnd フォルダ選択ダイアログのウィンドウハンドル
		 *	@param lpszPath 現在選択されているPATH.
		 *	@retval true OKボタンは enable
		 *	@retval false OKボタンは disable
		 */
		virtual bool OnDirectorySelectDlgSelectChanged(HWND hWnd, LPCTSTR lpszPath) const = 0;
	};
	
	/// コンストラクタ
	CDirectorySelectDlg(void) : m_piListener(NULL)
	{
		m_path[0] = 0;
	}

	/**
	 * [設定] リスナー設定.
	 *	@note DoModal() の前に使用します。
	 *	@param I リスナー
	 */
	void SetListener(IListener* I)
	{
		m_piListener = I;
	}

	/**
	 * [取得] 選択パス取得.
	 *	@note DoModal() の後に使用します。
	 *	@return パス。
	 */
	LPCTSTR GetSelectedPath(void) const
	{
		return m_path;
	}

	/**
	 * [実行] 選択画面表示.
	 *	@param lpszDirectory 初期選択フォルダ指定
	 *	@param lpszTitle タイトル指定（タイトルバーの文字列ではありません）
	 *	@param uFlag ダイアログボックスのオプションフラグを指定します。0または以下の値の組み合わせを指定します。
	 *		<table>
	 *		<tr><td>シンボル</td><td>内容</td></tr><tr><td>
	 *		0x0001 (BIF_RETURNONLYFSDIRS)	</td><td>
	 *			ファイルシステムディレクトリのみを返します。それ以外のアイテムが選択されているときには、[OK]ボタンは灰色表示になります。</td></tr><tr><td>
	 *		0x0002 (BIF_DONTGOBELOWDOMAIN)	</td><td>
	 *			ダイアログボックスのツリービューコントロールにドメインレベルのネットワークフォルダを含めないようにします。</td></tr><tr><td>
	 *		0x0004 (BIF_STATUSTEXT)			</td><td>
	 *			ダイアログボックスにステータス領域を表示します。表示テキストを設定するには、コールバック関数からダイアログボックスにメッセージを送信します。</td></tr><tr><td>
	 *		0x0008 (BIF_RETURNFSANCESTORS)	</td><td>
	 *			シェルネームスペース階層	構造の中でルートフォルダの下にあるファイルシステムサブフォルダのみを返します。それ以外のアイテムが選択されているときには、[OK]ボタンは灰色表示になります。</td></tr><tr><td>
	 *		0x0010 (BIF_EDITBOX)			</td><td>
	 *			Version 4.71 以降： ユーザーがアイテム名を書き込むことができるエディットコントロールを表示します。</td></tr><tr><td>
	 *		0x0020 (BIF_VALIDATE)			</td><td>
	 *			Version 4.71 以降： ユーザーがエディットコントロールに無効な名前を入力した場合に、 BFFM_VALIDATEFAILED メッセージとともにコールバック関数が呼び出されます。BIF_EDITBOXフラグが指定されていない場合は、このフラグは無視されます。</td></tr><tr><td>
	 *		0x0040 (BIF_NEWDIALOGSTYLE)		</td><td>
	 *			Version 5.0 以降： 新しいユーザーインターフェースを使用します。従来のダイアログボックスよりも大きい、リサイズ可能なダイアログボックスが表示され、ダイアログボックスへのドラッグアンドドロップ、フォルダの再整理、ショートカットメニュー、新しいフォルダ作成、削除、その他のショートカットメニューコマンドが追加されます。</td></tr><tr><td>
	 *		0x0050 (BIF_USENEWUI)			</td><td>
	 *			Version 5.0 以降： エディットコントロールを持つ、新しいユーザーインターフェースを使用します。このフラグはBIF_EDITBOX|BIF_NEWDIALOGSTYLEと同等です。</td></tr><tr><td>
	 *		0x0080 (BIF_BROWSEINCLUDEURLS)	</td><td>
	 *			Version 5.0 以降： URLを表示することができるようにします。BIF_USENEWUIとBIF_BROWSEINCLUDEFILESが同時に指定されていなければなりません。これらのフラグが設定されているとき、選択されたアイテムを含むフォルダがサポートする場合にのみ、URLが表示されます。アイテムの属性を問い合わせるためにフォルダのIShellFolder::GetAttributesOf メソッドが呼び出されたときに、フォルダによってSFGAO_FOLDER属性フラグが設定された場合にのみ、URLが表示されます。</td></tr><tr><td>
	 *		0x0100 (BIF_UAHINT)				</td><td>
	 *			Version 6.0 以降： エディットコントロールの代わりに、ダイアログボックスに用法ヒントを追加します。BIF_NEWDIALOGSTYLEフラグとともに指定しなければなりません。</td></tr><tr><td>
	 *		0x0200 (BIF_NONEWFOLDERBUTTON)	</td><td>
	 *			Version 6.0 以降： ダイアログボックスに「新しいフォルダ」ボタンを表示しないようにします。BIF_NEWDIALOGSTYLEフラグとともに指定しなければなりません。</td></tr><tr><td>
	 *		0x0400 (BIF_NOTRANSLATETARGETS)	</td><td>
	 *			Version 6.0 以降： 選択されたアイテムがショートカットであるとき、そのリンク先ではなく、ショートカットファイル自体のPIDLを返します。</td></tr><tr><td>
	 *		0x1000 (BIF_BROWSEFORCOMPUTER)	</td><td>
	 *			コンピュータのみを返します。それ以外のアイテムが選択されているときには、[OK]ボタンは灰色表示になります。</td></tr><tr><td>
	 *		0x2000 (BIF_BROWSEFORPRINTER)	</td><td>
	 *			プリンタのみを返します。それ以外のアイテムが選択されているときには、OK ボタンは灰色表示になります。</td></tr><tr><td>
	 *		0x4000 (BIF_BROWSEINCLUDEFILES)	</td><td>
	 *			Version 4.71 以降： フォルダとファイルを表示します。</td></tr><tr><td>
	 *		0x8000 (BIF_SHAREABLE)			</td><td>
	 *			Version 5.0 以降： リモートシステム上にある共有リソースを表示できるようにします。BIF_USENEWUIフラグとともに指定しなければなりません。</td></tr>
 	 *		</table>
	 *	@param hWnd 親ウィンドウ。
	 *	@retval IDOK 選択
	 *	@retval IDCANCEL キャンセル
	 */
	int DoModal(LPCTSTR lpszDirectory, LPCTSTR lpszTitle, UINT uFlag = 0, HWND hWnd = NULL)
	{
		m_path[0] = 0;
		LPMALLOC lpMalloc = NULL;
		HRESULT hr = ::SHGetMalloc(&lpMalloc);
		if ( FAILED(hr) )
		{
			return IDCANCEL;
		}
		// ブラウズ情報受け取りバッファ領域の確保
		LPTSTR lpBuffer = static_cast<LPTSTR>(lpMalloc->Alloc(MAX_PATH));
		if ( lpBuffer == NULL )
		{
			return IDCANCEL;
		}
		int	r = IDCANCEL;
		HRESULT hCom = ::CoInitialize(NULL);
		// BROWSEINFO構造体の初期値設定
		BROWSEINFO bi = { 0 };
		bi.hwndOwner = hWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = lpBuffer;
		bi.lpszTitle = lpszTitle;
		bi.ulFlags = uFlag;
		bi.lpfn = ms_BrowseCallbackProc; // コールバック関数のアドレスを設定
		TParam param;
		param.lpszDefaultDir = lpszDirectory;
		param.piListener = m_piListener;
		bi.lParam = reinterpret_cast<LPARAM>(&param);
		// フォルダ選択ダイアログの表示 
		LPITEMIDLIST pidlBrowse = ::SHBrowseForFolder(&bi);
		if ( pidlBrowse != NULL )
		{
			// PIDL形式の戻り値のファイルシステムのパスに変換
			if ( ::SHGetPathFromIDList(pidlBrowse, lpBuffer) )
			{
				// 取得成功
				_tcscpy(m_path, lpBuffer);
				r = IDOK;
			}
			// SHBrowseForFolderの戻り値PIDLを解放
			lpMalloc->Free(pidlBrowse);
		}
		// クリーンアップ処理
		lpMalloc->Free(lpBuffer);
		if ( hCom == S_OK )
		{
			::CoUninitialize();
		}
		//
		return r;
	}

private:
	IListener*	m_piListener;			///< リスナー
	TCHAR		m_path[MAX_PATH];		///< 選択パス保存用
	/// コールバック通知用パラメータ
	struct TParam
	{
		LPCTSTR		lpszDefaultDir;	///< 初期パス
		IListener*	piListener;		///< リスナー
	};
	/**
	 * [通知] コールバック
	 *	@param hWnd 選択ダイアログのハンドル
	 *	@param uMsg メッセージ
	 *	@param lParam メッセージ用のパラメータ
	 *	@param lpData TParam のポインタ
	 */
	static int CALLBACK ms_BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		const TParam* P = reinterpret_cast<const TParam*>(lpData);
		if ( uMsg == BFFM_INITIALIZED )
		{
			::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<LPARAM>(P->lpszDefaultDir));
			if ( P->piListener != NULL )
			{
				P->piListener->OnDirectorySelectDlgInitialized(hWnd);
				bool r = P->piListener->OnDirectorySelectDlgSelectChanged(hWnd, P->lpszDefaultDir);
				::SendMessage(hWnd, BFFM_ENABLEOK, 0, r);
			}
		}
		else if ( uMsg == BFFM_SELCHANGED ) 
		{
			if ( P->piListener != NULL )
			{
				TCHAR path[MAX_PATH];
				bool r = false;
				if ( ::SHGetPathFromIDList(reinterpret_cast<LPCITEMIDLIST>(lParam), path) )
				{
					::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, reinterpret_cast<LPARAM>(path));
					r = P->piListener->OnDirectorySelectDlgSelectChanged(hWnd, path);
				}
				::SendMessage(hWnd, BFFM_ENABLEOK, 0, r);
			}
		}
		else if( uMsg == BFFM_VALIDATEFAILED )
		{
		}
		return 0;
	}
};



};//TNB



