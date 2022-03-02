#pragma once
/**
 *@file
 * ファイルパス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifdef _WIN32_WCE
	#error TnbFilePathManager.h is only supported on Windows NT Series platforms.
#endif // _WIN32_WCE

#include "TnbFile.h"
#include "TnbStrEx.h"

#include <SHLWAPI.h>
#pragma comment(lib,"SHLWAPI.lib")



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ファイルパス管理クラス
 *
 *		一つの管理パスと、インスタンス時のパスを持っています。
 *
 *	@note インスタンスがある間、プロセスのパスをカレントにし、終了すると戻ります。
 *		ファイル選択ダイアログ等使う時に有効です。
 *
 *	@par必要ファイル
 *			TnbFilePathManager.h
 *
 *	@date 04/01/01 新規作成
 *	@date 06/05/01 CStringの使用をやめた
 *	@date 07/09/18 相対パス、絶対パス変換メソッド、追加。
 */
class CFilePathManager
{
public:

	/**
	 * [取得] フォルダ名抽出.
	 *	@note ファイル名のフォルダ指定部分までを返します。
	 *		たとえば、"c:/Foo/ASB.txt" なら "c:/Foo" が戻ります。
	 *	@param lpszFullPath ファイル名
	 *	@return パス名
	 */
	static CStr GetPathName(LPCTSTR lpszFullPath)
	{
		CStr str = lpszFullPath;
		INT_PTR p = str.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return str.Left(p + 1);
		}
		return str;
	}
	
	/**
	 * [取得] プロセスのカレントパス取得.
	 *		現在のカレントパスを返します。
	 *	@return パス名
	 */
	static CStr GetCurrentPath(void)
	{
		CStr str;
		DWORD dwRc = ::GetCurrentDirectory(MAX_PATH, str.GetBuffer(MAX_PATH));
		str.ReleaseBuffer();
		ASSERTLIB( dwRc != 0 );
		IgnoreUnusedValue(dwRc);
		return str;
	}

	/**
	 * コンストラクタ.
	 *	@note 現在のディレクトリを記憶し、プロセスのパスに切り替えます。
	 */
	CFilePathManager(void) 
		: m_strCurrentPath(GetCurrentPath()), m_strManagePath(GetProcessPath())
	{
		::SetCurrentDirectory(m_strManagePath);
	}

	/**
	 * デストラクタ.
	 *	@note コンストラクタ時のパスに戻します。
	 */
	~CFilePathManager(void)
	{
		::SetCurrentDirectory(m_strCurrentPath);
	}

	/**
	 * [設定] パスリセット.
	 *	@note カレントパスを、管理パスか、コンストラクタ時のパスにします。
	 *	@param boIsManagePath true なら管理パスに、 false ならコンストラクタ時のパスにします。
	 */
	void ResetPath(bool boIsManagePath = true) const
	{
		::SetCurrentDirectory(m_strCurrentPath);
	}

	/**
	 * [取得] 管理パス取得
	 *		インスタンスで管理しているパスを返します。
	 *	@note インスタンス生成時のパスか、 SetPath() で指定したパスを管理しています。
	 *	@return 管理パス。
	 */
	CStr GetPath(void) const
	{
		return m_strManagePath;
	}
	
	/**
	 * [設定] 管理パス指定.
	 *	@note インスタンスで管理しているパスを変更し、カレントパスも変更します。
	 *	@param lpszPath パス名
	 *	@retval true 成功。
	 *	@retval false 失敗。カレントパス、管理パスは変更されません。
	 */
	bool SetPath(LPCTSTR lpszPath)
	{
		::Sleep(0);
		bool boRc = !! ::SetCurrentDirectory(lpszPath);
		if ( boRc )
		{
			m_strManagePath = GetCurrentPath();
		}
		return boRc;
	}

	/**
	 * [設定] 管理パス指定.
	 *		指定のファイルあるディレクトリを管理パス、および、カレントパスにします。
	 *	@param lpszFile ファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetPathOfFile(LPCTSTR lpszFile)
	{
		return SetPath(GetPathName(lpszFile));
	}

	/**
	 * [取得] 相対パス作成.
	 *		管理パスからの相対パスに変換します。
	 *	@param path パス。通常絶対パスを指定します。
	 *	@return 相対パス。
	 */
	CStr GetRelativePathTo(LPCTSTR path) const
	{
		if ( IsRelative(path) )
		{
			//== 相対パス
			return path;
		}
		//== 絶対パス
		CStr s;
		BOOL r = ::PathRelativePathTo(s.GetBuffer(MAX_PATH),
							GetPath(),
							FILE_ATTRIBUTE_DIRECTORY,
							path,
							FILE_ATTRIBUTE_ARCHIVE);
		s.ReleaseBuffer();
		if ( ! r )
		{
			s.Empty();
		}
		return s;
	}

	/**
	 * [取得] 絶対パス作成.
	 *		管理パスからの相対パスを、絶対パスに変換します。
	 *	@param path 管理パスからの相対パスを指定します。
	 *	@return 絶対パス。
	 */
	CStr GetCanonicalize(LPCTSTR path) const
	{
		if ( ! IsRelative(path) )
		{
			//== 絶対パス
			return path;
		}
		//== 相対パスだ
		CStr s;
		BOOL r = ::PathCanonicalize(s.GetBuffer(MAX_PATH), GetPath() + _T("\\") + path);
		s.ReleaseBuffer();
		if ( ! r )
		{
			s.Empty();
		}
		return s;
	}

	/**
	 * [操作] ファイルオープン.
	 *	@note 指定ディレクトリ→プロセスディレクトリ→カレントディレクトリ
	 *		という順で検索し、読み込み属性でオープンします。
	 *	@param lpszFile オープンしたいファイル
	 *	@param lpszPath 検索するディレクトリ。NULLならプロセスディレクトリから検索開始します。
	 *	@return ファイルハンドル。
	 *			アクセスできないハンドルならファイルが見つからなかったことになります。
	 */
	CFileReader Open(LPCTSTR lpszFile, LPCTSTR lpszPath = NULL) const
	{
		CStr path = lpszPath;
		CFileReader fileHandle;
		CStr strFullpath;
		//
		loop ( iType, 3 )
		{
			if ( iType == 0 || iType == 1 )
			{
				//0;指定パス 1;EXEパス から
				if ( iType == 0 && path.IsEmpty() )
				{
					continue;
				}
				if ( iType == 1 )
				{
					path = GetProcessPath();
				}
				//
				if ( ! ::PathIsRelative(lpszFile) )
				{
					//絶対パス
					continue;
				}
				//
				if ( ! ::PathSearchAndQualify(
							CStr::Fmt(_T("%s\\%s"), path, lpszFile),
							strFullpath.GetBuffer(MAX_PATH),
							MAX_PATH))
				{
					continue;
				}
				strFullpath.ReleaseBuffer();
			}
			else
			{
				//パス補完なし
				strFullpath = lpszFile;
			}
			//
			{
				TCHAR atcBuf[MAX_PATH];
				::GetShortPathName(strFullpath, atcBuf, MAX_PATH);
				::GetLongPathName(atcBuf, strFullpath.GetBuffer(MAX_PATH), MAX_PATH);
				strFullpath.ReleaseBuffer();
			}
			//
			if ( fileHandle.Open(strFullpath, true) )
			{
				break;
			}
		}
		return fileHandle;
	}

	/**
	 * [確認] 相対パスチェック
	 *	@param lpszPath パス
	 *	@retval true 相対パス
	 *	@retval false 絶対パス
	 */
	static bool IsRelative(LPCTSTR lpszPath)
	{
		return !! ::PathIsRelative(lpszPath);
	}

private:
	CStr m_strCurrentPath;		///< コンストラクタ時のパス
	CStr m_strManagePath;		///< 今の管理パス
};



}; // TNB
