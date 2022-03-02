#pragma once
/**
 *@file
 * CABファイルアーカイバー関係のヘッダ
 *
 *		CABファイルの作成、解凍するための機能があります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDeepFileFinder.h"
#include "TnbSet.h"
#include "TnbMap.h"
#include "TnbFileName.h"
#include "TnbProcessHandle.h"
#include "TnbStrAdder.h"
#include "TnbTemporaryDirectory.h"
#include "TnbFilePathManager.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * CABファイルアーカイバー
 *
 *	@par使用例
 *	@code
 *		CCabfileArchiver cab;
 *		cab.AddSourceFile(_T("C:\\Home\\temp\\TT\\a.dat"), _T(""));
 *		cab.AddSourceDirectory(_T("C:\\Home\\temp\\TT"), _T("\\GG"));
 *		cab.Attract(_T("C:\\Home\\temp\\ABC.cab"));
 *
 *		CCabfileArchiver::Extract(_T("C:\\Home\\temp\\BB"), _T("C:\\Home\\temp\\ABC.cab"));
 *	@endcode
 *	@par必要ファイル
 *			TnbCabfileArchiver.h
 *
 *	@date 12/08/15 新規作成
 */
class CCabfileArchiver
{
public:

	/**
	 * [設定] ソースディレクトリ追加.
	 *		指定のディレクトリ以下を圧縮対象にします。
	 *	@param lpszPath 追加するパス名
	 *	@param lpszCabPath CAB内のパスを指定します。指定しないとルートになります。　
	 */
	void AddSourceDirectory(LPCTSTR lpszPath, LPCTSTR lpszCabPath = NULL)
	{
		CMyFinder f(&m_map, lpszCabPath);
		f.Execute(lpszPath);
	}

	/**
	 * [設定] ソースファイル追加.
	 *		指定のファイルを圧縮対象にします。
	 *	@param lpszFileName 追加するファイル名
	 *	@param lpszCabPath CAB内のパスを指定します。指定しないとルートになります。　
	 *	@retval true 成功
	 *	@retval false 失敗。ファイルがないと失敗します。
	 */
	bool AddSourceFile(LPCTSTR lpszFileName, LPCTSTR lpszCabPath = NULL)
	{
		CFileName fn = lpszFileName;
		if ( fn.IsExist() )
		{
			CStr s = lpszCabPath;
			m_map[s].Insert(fn.GetFullName());
			return true;
		}
		return false;
	}

	/**
	 * [削除] 削除.
	 *		AddSourceDirectory() , AddSourceFile() したものを破棄します。ファイルには影響ありません。
	 */
	void RemoveAll(void)
	{
		m_map.RemoveAll();
	}

	/**
	 * [作成] 格納.
	 *		新しく cabファイルを作成します。
	 *	@param lpszCabFileName CABファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Attract(LPCTSTR lpszCabFileName)
	{
		CStrAdder sa;
		loop ( i, m_map )
		{
			CStr cabPath = m_map.At(i).first;
			const CSingleSetT<CStr>& v = m_map.At(i).second;
			if ( v.GetSize() > 0 )
			{
				sa.AddFormat(_T(".Set DestinationDir=\"%s\"\r\n"), cabPath);
				loop ( j, v )
				{
					sa.AddFormat(_T("\"%s\"\r\n"), v[j]);
				}
			}
		}
		CAscii a = CStr(sa);
		LPCSTR lpsz = a;
		CTemporaryFile temp;
		CTemporaryDirectory tempDir;
		CStr pt = tempDir.Create();
		CStr fn = temp.Create();
		if ( CFileWriter::NewFile(fn, a.GetLength(), lpsz) )
		{
			CFilePathManager fpm;
			::SetCurrentDirectory(pt);
			CStr cl;
			cl.Format(_T("makecab /F \"%s\""), fn);
			CProcessHandle ph;
			if ( ph.Create(NULL, cl, false) )
			{
				ph.WaitForExit();
				CFileName fn = pt + _T("\\disk1\\1.cab");
				return fn.CopyTo(lpszCabFileName);
			}
		}
		return false;
	}

	/**
	 * [作成] 解凍.
	 *		cabファイルを解凍します。
	 *	@param lpszPath 解凍先。
	 *	@param lpszCabFileName 解凍するCABファイル名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool Extract(LPCTSTR lpszPath, LPCTSTR lpszCabFileName)
	{
		CFileName fn = lpszPath;
		fn.MakeDirectory();
		CStr cl;
		cl.Format(_T("expand \"%s\" -F:* \"%s\""), lpszCabFileName, lpszPath);
		CProcessHandle ph;
		if ( ph.Create(NULL, cl, false) )
		{
			ph.WaitForExit();
			return ph.GetExitCode() == 0;
		}
		return false;
	}

private:
	/// 内部クラス
	class CMyFinder : public CDeepFileFinder
	{
		CMapT< CStr, CSingleSetT<CStr> >*	m_pMap;
		CStr								m_cabPath;
	public:
		CMyFinder(CMapT< CStr, CSingleSetT<CStr> >*	p, LPCTSTR lpszCabPath) : m_pMap(p), m_cabPath(lpszCabPath)
		{
		}
	protected:
		// [通知] ファイル発見通知.
		virtual void OnFoundFile(LPCTSTR lpszFoundName, const WIN32_FIND_DATA& data, int depth)
		{
			CFileName fn = lpszFoundName;
			CFileName path = fn.GetPathName();
			CStr cp = m_cabPath;
			if ( depth > 1 )
			{
				CStr ex;
				loop ( i, depth - 1 )
				{
					if ( ex.IsEmpty() )
					{
						ex = path.GetFileName();
					}
					else
					{
						ex = path.GetFileName() + _T("\\") + ex;
					}
					path = path.GetPathName();
				}
				if ( cp.IsEmpty() )
				{
					cp = ex;
				}
				else
				{
					cp = cp + _T("\\") + ex;
				}
			}
			(*m_pMap)[cp].Insert(fn.GetFullName());
		}
	};
	CMapT< CStr, CSingleSetT<CStr> >	m_map;	///< 情報
};



}; // TNB
