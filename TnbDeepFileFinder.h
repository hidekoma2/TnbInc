#pragma once
/**
 *@file
 * ファイル検索関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ディープファイル検索クラス
 *
 *		ディレクトリを深く掘り込んで検索するクラスです。
 *		本クラスを継承し、 OnStartFindFolder() , OnFoundFolder() , OnFoundFile() に処理を実装します。
 *
 *	@par必要ファイル
 *			TnbDeepFileFinder.h
 *
 *	@date 09/07/16 新規作成
 *	@date 09/09/29 OndemandStop() で指定した値が Execute() の戻り値にならないバグを修正。 
 */
template<typename TYP = CFileFinder>
class CDeepFileFinderT
{
public:

	/// コンストラクタ
	CDeepFileFinderT(void) : m_depth(0), m_continue(true), m_result(0)
	{
		#ifdef _DEBUG
			TYP ff;
			CAbstractFileFinder* P = &ff;		// ここでエラー出る場合、 TYP に CAbstractFileFinder のサブクラスを指定して無い可能性があります。
			IgnoreUnusedValue(P);
		#endif
	}

	/**
	 * [実行] 検索開始.
	 *	@param lpszPath 検索を開始するパスを指定します。
	 *	@return リザルト( OndemandStop() で指定した値)。
	 */
	DWORD Execute(LPCTSTR lpszPath)
	{
		m_depth = 0;
		m_continue = true;
		m_result = 0;
		m_Find(lpszPath);
		return m_result;
	}

protected:

	/**
	 * [依頼] 停止依頼.
	 *		検索の停止を指示します。
	 *	@note OnStartFindFolder(), OnFoundFolder() , OnFoundFile() 内から、使用します。
	 *	@param result リザルト。 Execute() の戻り値を指定できます。
 	 */
	void OndemandStop(DWORD result)
	{
		m_result = result;
		m_continue = false;
	}

	/**
	 * [通知] フォルダ検索開始通知.
	 *		フォルダの検索開始を通知します。
	 *	@note 検索を停止したい場合、 OndemandStop() を呼び出してください。
	 *	@param[in,out] finder 検索に使用するファインダー。必要に応じて設定をしてください。
	 *	@param[in] lpszTargetPath 対象のフォルダ名
	 *	@param[in] depth 深さ。最初のフォルダ層が１で、以下深くなるごとに＋１されます。
	 */
	virtual void OnStartFindFolder(TYP& finder, LPCTSTR lpszTargetPath, int depth)
	{
	}

	/**
	 * [通知] フォルダ発見通知.
	 *		フォルダを発見すると通知します。
	 *	@note 検索を停止したい場合、 OndemandStop() を呼び出してください。
	 *	@param lpszFoundFolder 発見したフォルダ名
	 *	@param data 発見したフォルダの情報
	 *	@param depth 深さ。最初のフォルダ層が１で、以下深くなるごとに＋１されます。
	 *	@retval true このフォルダ内の検索を行う。
	 *	@retval false このフォルダ内の検索は行わない。
	 */
	virtual bool OnFoundFolder(LPCTSTR lpszFoundFolder, const WIN32_FIND_DATA& data, int depth)
	{
		::OutputDebugString(_T("OnFoundFolder("));
		::OutputDebugString(lpszFoundFolder);
		::OutputDebugString(_T(")\n"));
		return true;
	}

	/**
	 * [通知] ファイル発見通知.
	 *		ファイルを発見すると通知します。
	 *	@note 検索を停止したい場合、 OndemandStop() を呼び出してください。
	 *	@param lpszFoundName 発見したファイル名
	 *	@param data 発見したファイルの情報
	 *	@param depth 深さ。最初のフォルダ層が１で、以下深くなるごとに＋１されます。
	 */
	virtual void OnFoundFile(LPCTSTR lpszFoundName, const WIN32_FIND_DATA& data, int depth)
	{
		::OutputDebugString(_T("OnFoundFile("));
		::OutputDebugString(lpszFoundName);
		::OutputDebugString(_T(")\n"));
	}

private:
	int		m_depth;	///< 深さ
	bool	m_continue;	///< 続行フラグ
	DWORD	m_result;	///< リザルト
	// 検索処理
	void m_Find(LPCTSTR lpszPath)
	{
		m_depth++;
		if ( m_continue )
		{
			TYP ff;
			OnStartFindFolder(ff, lpszPath, m_depth);
			if ( m_continue && ff.Start(lpszPath, _T("\\*")) )
			{
				do
				{
					CStr fileName = ff->cFileName; 
					CStr fullPath;
					fullPath.Format(_T("%s\\%s"), lpszPath, fileName);
					if ( (ff->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
					{
						// Dir
						if ( OnFoundFolder(fullPath, ff.Get(), m_depth) )
						{
							m_Find(fullPath);
						}
					} 
					else
					{
						OnFoundFile(fullPath, ff.Get(), m_depth);
					}
				} 
				while ( m_continue && ff.Next() );
			}
		}
		m_depth--;
	}
};



/**@ingroup FILE
 * ディープファイル検索クラス
 *
 *		ディレクトリを深く掘り込んで検索するクラスです。
 *		本クラスを継承し、OnNewFiner() , OnFoundFolder() , OnFoundFile() に処理を実装します。
 *
 *	@par必要ファイル
 *			TnbDeepFileFinder.h
 *
 *	@date 09/07/16 新規作成
 */
typedef CDeepFileFinderT<> CDeepFileFinder;



}; // TNB
