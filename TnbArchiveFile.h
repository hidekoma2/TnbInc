#pragma once
/**
 *@file
 * アーカイブファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbArchiver.h"
#include "TnbTemporaryDirectory.h"
#include "TnbFile.h"
#include "TnbArchiveFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE ARCHIVEFILE
 * アーカイブファイル読み込みクラス
 *
 *		アーカイブファイル内のファイル一つの読み込みをサポートするクラスです。
 *
 *	@par必要ファイル
 *			TnbArchiveFile.h
 *
 *	@date 06/12/19 新規作成
 *	@date 08/03/27 フォルダを作成するようにした
 *	@date 10/10/04 展開時、サブフォルダを含んで展開するように
 *					変更（対応していないアーカイバがあるため）
 */
class CArchiveFileReader : public IReader
{
	DEFSUPER(IReader);
public:

	/// コンストラクタ
	CArchiveFileReader(void)
	{
	}

	/// デストラクタ
	virtual ~CArchiveFileReader(void)
	{
		m_Close();
	}

	/**
	 * [設定] オープン
	 *	@note 既に一つ開いていたらクローズしてから指定のファイルを開きます。
	 *	@param arcdll 有効なアーカイバDLL
	 *	@param lpszArcFile アーカイブファイル
	 *	@param lpszName アーカイブファイル内のファイル。
	 *	@param lpszOpt アーカイブ取り出し時のオプション。
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool Open(CArchiverDllHandler& arcdll, LPCTSTR lpszArcFile, LPCTSTR lpszName, LPCTSTR lpszOpt = NULL)
	{
		Close();
		if ( ! arcdll.IsReady() || arcdll.IsRunning() )
		{
			return false;
		}
		CArchiver arc(&arcdll);
		arc.SetTargetArchiveFile(lpszArcFile);
		CStr strPath = m_tempDir.Create();
		arc.SetWorkDirectory(strPath);
		bool r = (arc.ExtractWithDirectory(lpszName, lpszOpt) == 0);
		if ( r )
		{
			CStr ss = strPath + _T("\\") + lpszName;
			ss.Replace(_T("\\\\"), _T("\\"));
			r = m_reader.Open(ss);
		}
		if ( ! r )
		{
			m_tempDir.Free();
		}
		return r;
	}

	/**
	 * [設定] オープン
	 *	@param ff 検索結果
	 *	@param lpszOpt アーカイブ取り出し時のオプション。
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool Open(const CArchiveFileFinder& ff, LPCTSTR lpszOpt = NULL)
	{
		const TIndividualInfo* P = ff.RefPtr();
		if ( ff.IsFinding() && P != NULL )
		{
			#ifdef _UNICODE
				return Open(*ff.GetDllHandler(), CStr(ff.GetTargetArchiveFileName()), CStr(P->szFileName), lpszOpt);
			#else
				return Open(*ff.GetDllHandler(), ff.GetTargetArchiveFileName(), P->szFileName, lpszOpt);
			#endif
		}
		return false;
	}

	/**
	 * [取得] オープン名取得
	 *	@note 得られる名前は、テンポラリファイル名になります。
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_reader.GetOpenName();
	}

	/**
	 * [操作] クローズ
	 *	@note 使用していたテンポラリファイルも削除。
	 */
	virtual void Close(void)
	{
		m_Close();
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return m_reader.CanRead();
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		return m_reader.GetSize();
	}

	/**
	 * [操作] シーク.
	 *		アクセスポインタを移動します。
	 *	@param llOffset アクセスポインタの移動数。
	 *	@param eSeek 移動開始アクセスポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return m_reader.Seek(llOffset, eSeek);
	}

	 /**
	 * [取得] 読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		return m_reader.Read(size, _P);
	}

private:
	///クローズ
	void m_Close(void)
	{
		m_reader.Close();
		m_tempDir.Free();
	}
	CFileReader					m_reader;	///< ファイル読み込み
	CTemporaryDirectoryT<'CRA'>	m_tempDir;	///< テンポラリ

	friend class CArchiveFileReaderTest;
};



}; // TNB
