#pragma once
/**
 *@file
 * アーカイバー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbArchiverDllHandler.h"
#include "TnbStrVector.h"
#include "TnbTemporaryFile.h"



//TNB Library
namespace TNB
{



/**@ingroup ARCHIVEFILE
 * アーカイバクラス
 *
 *		圧縮/展開をサポートするクラスです。
 *
 *	@par必要ファイル
 *			TnbArchiver.h
 *
 *	@date 06/12/14 新規作成
 *	@date 09/07/16 作業ディレクトリを自動設定するようにした。
 */
class CArchiver
{
public:

	/**
	 * コンストラクタ
	 *	@param pDll アーカイバDLL管理クラスのインスタンス。
	 *				本インスタンスで使用中、破棄してはいけません。
	 *	@throw CNullPointerException 使用できないアーカイバ管理クラスを指定すると、スローされます。
	 */
	CArchiver(CArchiverDllHandler* pDll) : m_pDll(pDll)
	{
		ASSERT0( pDll != NULL, "CArchiver()", "正しいアーカイバ管理クラスが指定されていません。" );
		if ( pDll == NULL ) 
		{
			throw CNullPointerException();
		}
		ASSERT0( pDll->IsReady(), "CArchiver()", "使用可能なアーカイバ管理クラスが指定されていません。" );
		if ( ! pDll->IsReady() ) 
		{
			throw CNullPointerException(); 
		}
		::GetTempPathA(MAX_PATH, m_strWorkDir.GetBuffer(MAX_PATH));
		m_strWorkDir.ReleaseBuffer();
	}
	
	/**
	 * [取得] 対象アーカイブ取得
	 *	@note SetTargetArchiveFile() で設定したファイル名が取得できます。
	 *	@return アーカイブ名
	 */
	CStr GetTargetArchiveFile(void) const
	{
		CStr s = m_strArcFile;
		return s;
	}

	/**
	 * [設定] 対象アーカイブ指定
	 *	@param lpszArcFile 対象アーカイブ	
	 *	@retval ture 指定ファイルがないか、正しいアーカイブの場合。
	 *	@retval false 指定ファイルが存在し、正しくないアーカイブの場合。
	 */
	bool SetTargetArchiveFile(LPCTSTR lpszArcFile)
	{
		bool r = false;
		if ( ToInt(::GetFileAttributes(lpszArcFile)) == -1 )
		{
			r = (::GetLastError() == ERROR_FILE_NOT_FOUND);
		}
		else if ( m_pDll->CheckArchive(CAscii(lpszArcFile), 0) )
		{
			r = true;
		}
		if ( r )
		{
			m_strArcFile = lpszArcFile;
		}
		return r;
	}

	/**
	 * [設定] 作業ディレクトリ指定
	 *	@note 設定しない場合、システムのテンポラリディレクトリが使用されます。
	 *	@param lpszWorkDir ディレクトリ
	 */
	void SetWorkDirectory(LPCTSTR lpszWorkDir)
	{
		m_strWorkDir = lpszWorkDir;
	}

	/**
	 * [操作] 削除.
	 *		アーカイブファイルから削除します。
	 *	@param lpszName 削除する名前。ワイルドカードも可能。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int Delete(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( m_strArcFile.IsEmpty() )
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_Delete, lpszName, lpszOption);
	}

	/**
	 * [操作] 削除.
	 *		アーカイブファイルから削除します。
	 *	@param vs 削除する名前。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int Delete(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( m_strArcFile.IsEmpty() ) 
		{
			return -1; 
		}
		return m_ExecList(m_pDll->EC_Delete, vs, lpszOption);
	}

	/**
	 * [操作] 取得.
	 *		アーカイブファイルからファイルを展開します。
	 *	@param lpszName 展開する名前。ワイルドカードも可能。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int Extract(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() )
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_Extract, lpszName, lpszOption);
	}

	/**
	 * [操作] 展開.
	 *		アーカイブファイルからファイルを展開します。
	 *	@param vs 削除する名前。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int Extract(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() ) 
		{
			return -1; 
		}
		return m_ExecList(m_pDll->EC_Extract, vs, lpszOption);
	}

	/**
	 * [操作] 展開
	 *	@param[in] lpszName 対象ファイル。
	 *	@param[in,out] _vb 読み出すサイズを設定して起きます。 実行後は抽出したサイズになっています。
	 *	@param[in] lpszOption 追加オプション（省略可能）。
	 *	@retval 0 正常終了
	 *	@retval 0以外 エラー発生
	 */
	int ExtractMem(LPCTSTR lpszName, CByteVector& _vb, LPCTSTR lpszOption = NULL)
	{
		int r = -1;
		if ( ! m_strArcFile.IsEmpty() ) 
		{
			size_t dwSize = _vb.GetSize();
			#ifdef _UNICODE
				int r = m_pDll->ExtractMem(m_strArcFile, CAscii(lpszName), _vb.GetBuffer(), dwSize, CAscii(lpszOption));
			#else
				int r = m_pDll->ExtractMem(m_strArcFile, lpszName, _vb.GetBuffer(), dwSize, lpszOption);
			#endif
			_vb.ReleaseBuffer();
			_vb.SetSize((r == 0) ? dwSize : 0);
		}
		return r;
	}

	/**
	 * [操作] 展開.
	 *		アーカイブファイルからファイルを展開します。
	 *	@param lpszName 展開する名前。ワイルドカードも可能。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int ExtractWithDirectory(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() ) 
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_ExtractDir, lpszName, lpszOption);
	}

	/**
	 * [操作] 展開.
	 *		アーカイブファイルからファイルを展開します。
	 *	@param vs 削除する名前。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int ExtractWithDirectory(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() ) 
		{
			return -1; 
		}
		return m_ExecList(m_pDll->EC_ExtractDir, vs, lpszOption);
	}

	/**
	 * [操作] 格納.
	 *		アーカイブファイルへファイルを格納します。
	 *	@param lpszName 格納する名前。ワイルドカードも可能。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int Attract(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() )
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_Attract, lpszName, lpszOption);
	}

	/**
	 * [操作] 格納.
	 *		アーカイブファイルへファイルを格納します。
	 *	@param vs 削除する名前。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。エラーコード。
	 */
	int Attract(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() )
		{
			return -1;
		}
		return m_ExecList(m_pDll->EC_Attract, vs, lpszOption);
	}

	/**
	 * [操作] メモリから格納
	 *	@param[in] lpszName 格納ファイル名。
	 *	@param[in] time 格納時間。
	 *	@param[in] wAttr 格納属性。
	 *	@param[in] vb 格納内容。
	 *	@param[in] lpszOption 追加オプション（省略可能）。
	 *	@retval 0 正常終了
	 *	@retval 0以外 エラー発生
	 */
	int AttractMem(LPCTSTR lpszName, time_t time, WORD wAttr, const CByteVector& vb, LPCTSTR lpszOption = NULL)
	{
		int r = -1;
		if ( ! m_strArcFile.IsEmpty() )
		{
			DWORD dwSize = ToDword(vb.GetSize());
			#ifdef _UNICODE
				r = m_pDll->CompressMem(m_strArcFile, CAscii(lpszName), time, wAttr, vb.ReferBuffer(), dwSize, CAscii(lpszOption));
			#else
				r = m_pDll->CompressMem(m_strArcFile, lpszName, time, wAttr, vb.ReferBuffer(), dwSize, lpszOption);
			#endif
		}
		return r;
	}
private:
	CArchiverDllHandler*	m_pDll;			///< アーカイバDLL管理
	CAscii					m_strArcFile;	///< アーカイブファイル名
	CAscii					m_strWorkDir;	///< 作業ディレクトリ

	/// 設定確認
	bool m_IsReady(void) const
	{
		return (! m_strArcFile.IsEmpty()) && (! m_strWorkDir.IsEmpty());
	}

	/// 実行
	int m_Exec(CArchiverDllHandler::ECommand cmd, LPCTSTR lpszName, LPCTSTR lpszOption)
	{
		#ifdef _UNICODE
			return m_pDll->Execute(cmd, m_strArcFile, m_strWorkDir, CAscii(lpszName), CAscii(lpszOption));
		#else
			return m_pDll->Execute(cmd, m_strArcFile, m_strWorkDir, lpszName, lpszOption);
		#endif
	}

	/// レスポンスファイル実行
	int m_ExecList(CArchiverDllHandler::ECommand cmd, const CStrVector& vs, LPCTSTR lpszOption)
	{
		CAscii strContent;
		if ( lpszOption != NULL )
		{
			strContent += lpszOption;
			strContent += "\r\n";
		}
		loop ( i, vs.GetSize() )
		{
			strContent += CAscii(vs[i]);
			strContent += "\r\n";
		}
		CTemporaryFile tmp;
		CStr strName = tmp.Create(strContent.GetLength(), strContent.begin());
		int r = m_Exec(cmd, "@" + strName, NULL);
		tmp.Free();
		return r;
	}
};



}; // TNB
