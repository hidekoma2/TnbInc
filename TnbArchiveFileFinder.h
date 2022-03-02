#pragma once
/**
 *@file
 * アーカイブファイル検索関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbArchiverDllHandler.h"
#include "TnbFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE ARCHIVEFILE
 * アーカイブ内ファイル検索クラス
 *
 *		指定のアーカイブファイルからファイルを検索するためのクラスです。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		C7zDllHandler arc;
 *		CArchiveFileFinder ff(&arc);
 *		if ( ff.Find("c:\\$NtUninstallKB_Vol1.7z", "*.*", 0, ff.FILE_ONLY) )
 *		{
 *			TRACE1("%s\n", ff.GetFindingPath());
 *			do
 *			{
 *				TRACE1("  found = %s\n", ff->cFileName);
 *			} while ( ff.Next() );
 *		}
 *		</pre></td></tr></table>
 *		※ CFileFinder 同様に iterator を使用した検索も可能です。
 *	@note iterator 操作時、元の CArchiveFileFinder が使用されます。
 *
 *	@note UNICODE プロジェクトでも OEMコードで動きます。
 *
 *	@par必要ファイル
 *			TnbArchiveFileFinder.h
 *
 *	@todo 他の FileFinder に比べ、フォルダの概念がない。
 *
 *	@date 06/12/14 新規
 *	@date 08/03/27 改造中
 */
class CArchiveFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
	CArchiverDllHandler*	m_pDll;			///< アーカイバDLL管理
	CAscii					m_strArcFile;	///< 検索対象アーカイブ
	DWORD					m_dwMode;		///< モード
	HARC					m_hArc;			///< 検索用ハンドル
	TIndividualInfo			m_foundInfo;

	/**
	 * [変換] Arc用属性から Win32属性へ変更
	 *	@note 属性は以下のように変換されます。\n
	 *		FILE_ATTRIBUTE_READONLY		<- FA_RDONLY(読み込み専用ファイル)\n
	 *		FILE_ATTRIBUTE_HIDDEN		<- FA_HIDDEN(不可視属性ファイル)\n
	 *		FILE_ATTRIBUTE_SYSTEM		<- FA_SYSTEM(システムファイル)\n
	 *		FILE_ATTRIBUTE_DIRECTORY	<- FA_DIREC(ディレクトリ)\n
	 *		FILE_ATTRIBUTE_ARCHIVE		<- FA_ARCH(保存ビット)\n
	 *		FILE_ATTRIBUTE_ENCRYPTED	<- FA_ENCRYPTED(パスワード保護されたファイル)\n
	 *		FILE_ATTRIBUTE_TEMPORARY	<- FA_LABEL(ボリュームラベル)\n
	 *		FILE_ATTRIBUTE_NORMAL		<- 上記のいずれでもない時
	 *	@param aa Arc用属性値
	 *	@return Win32属性値
	 */
	static DWORD ms_ArcAttrToWin32Attr(int aa)
	{
		DWORD r = 0;
		if ( (aa & FA_RDONLY) != 0 )	{ r |= FILE_ATTRIBUTE_READONLY;		}
		if ( (aa & FA_HIDDEN) != 0 )	{ r |= FILE_ATTRIBUTE_HIDDEN;		}
		if ( (aa & FA_SYSTEM) != 0 )	{ r |= FILE_ATTRIBUTE_SYSTEM;		}
		if ( (aa & FA_LABEL) != 0 )		{ r |= FILE_ATTRIBUTE_TEMPORARY;	}
		if ( (aa & FA_DIREC) != 0 )		{ r |= FILE_ATTRIBUTE_DIRECTORY;	}
		if ( (aa & FA_ARCH) != 0 )		{ r |= FILE_ATTRIBUTE_ARCHIVE;		}
		if ( (aa & FA_ENCRYPTED) != 0 )	{ r |= FILE_ATTRIBUTE_ENCRYPTED;	}
		if ( r == 0 )					{ r |= FILE_ATTRIBUTE_NORMAL;		}
		return r;
	}

	/**
	 * [作成] 検索情報作成
	 *	@note インスタンス内の情報( m_foundInfo )と、 アーカイバから情報を取得します。
	 *	@param[out] _win32Info 情報書込み。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool m_MakeWin32Info(WIN32_FIND_DATA& _win32Info)
	{
		Zero(_win32Info);
		try
		{
			//-- 属性変換
			int arcAttr = 0;
			m_pDll->Func(arcAttr, "GetAttribute", m_hArc);
			_win32Info.dwFileAttributes = ms_ArcAttrToWin32Attr(arcAttr);
			//-- 書込時間
			BOOL r;
			if ( m_pDll->QueryFunctionList(ISARC_GET_WRITE_TIME_EX) )
			{
				m_pDll->Func(r, "GetWriteTimeEx", m_hArc, &(_win32Info.ftLastWriteTime));
			}
			else
			{
				//_win32Info.ftLastWriteTime
				//m_foundInfo.wDate , m_foundInfo.wTime;
			}
			_win32Info.ftCreationTime = _win32Info.ftLastWriteTime;
			_win32Info.ftLastAccessTime = _win32Info.ftLastWriteTime;
			if ( m_pDll->QueryFunctionList(ISARC_GET_CREATE_TIME_EX) )
			{
				m_pDll->Func(r, "GetCreateTimeEx", m_hArc, &(_win32Info.ftCreationTime));
			}
			if ( m_pDll->QueryFunctionList(ISARC_GET_ACCESS_TIME_EX) )
			{
				m_pDll->Func(r, "GetAccessTimeEx", m_hArc, &(_win32Info.ftLastAccessTime));
			}
			//-- サイズ
			_win32Info.nFileSizeHigh = 0;
			_win32Info.nFileSizeLow = m_foundInfo.dwOriginalSize;
			//-- 予備	
			_win32Info.dwReserved0 = 0;
			_win32Info.dwReserved1 = 0;
			//-- 名前
			CStr s = m_foundInfo.szFileName;
			STRLIB::Copy(_win32Info.cFileName, s.Left(MAX_PATH));
			_win32Info.cAlternateFileName[0] = 0;
			return true;
		}
		catch(CNullPointerException& e)
		{
			e.OnCatch();
		}
		catch(...)
		{
			ASSERTLIB(false);
			throw;
		}
		return false;
	}

public:

	/// コンストラクタ
	CArchiveFileFinder(void) : _super(), m_pDll(NULL), m_hArc(NULL), m_dwMode(0)
	{
	}

	/// デストラクタ
	virtual ~CArchiveFileFinder(void)
	{
		OnFinish();
	}

	/**
	 * [設定] 初期化
	 *	@note インスタンス作成後、検索の前に一度はこのメソッドを実行します。
	 *	@param pDll アーカイバDLL管理クラスのインスタンス。
	 *				本インスタンスで使用中、破棄してはいけません。
	 *	@param lpszArcFileName 対象のアーカイブファイル名。
	 *	@param dwMode		アーカイバ種による。省略すると０。
	 *	@retval true 成功。
	 *	@retval false 失敗。正しいアーカイバが指定されていないか、アーカイブファイルが見つからない。
	 */
	bool Initialize(CArchiverDllHandler* pDll, LPCTSTR lpszArcFileName, DWORD dwMode = 0)
	{
		Finish();
		if ( pDll == NULL ) 
		{
			ASSERT0( false, "CArchiveFileFinder()", "正しいアーカイバ管理クラスが指定されていません。" );
			return false;
		}
		if ( ! pDll->IsReady() ) 
		{
			ASSERT0( false, "CArchiveFileFinder()", "使用可能なアーカイバ管理クラスが指定されていません。" );
			return false;
		}
		int r = static_cast<int>(::GetFileAttributes(lpszArcFileName));
		if ( r < 0 )
		{
			_GetLastError("GetFileAttributes");
			return false;
		}
		m_pDll = pDll;
		m_strArcFile = lpszArcFileName;
		m_dwMode = dwMode;
		return true;
	}
	
	/**
	 * [取得] 検索情報参照
	 *	@note Get() に比べ、インスタンスで記憶しているデータを返すだけなので、高速です。
	 *	@retval NULL 失敗
	 *	@retval NULL以外 ファイル情報の参照。
	 */
	const TIndividualInfo* RefPtr(void) const
	{
		if ( m_hArc != NULL )
		{
			return &m_foundInfo;
		}
		return NULL;
	}

	/**
	 * [取得] 使用アーカイブファイル名、取得
	 *	@note Find() で渡したアーカイブファイル名を返すだけです。
	 *	@return ファイル名。
	 */
	CAscii GetTargetArchiveFileName(void) const
	{
		return m_strArcFile;
	}

	/**
	 * [取得] 使用アーカイバDLL管理クラス、参照
	 *	@note コンストラクタで指定したアーカイバDll管理クラスのアドレスを返すだけです。
	 *	@return 参照。
	 */
	CArchiverDllHandler* GetDllHandler(void) const
	{
		return m_pDll;
	}

protected:

	/**
	 * [通知] 検索開始通知
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@param[in] lpszName 検索パス指定。ワイルドカードによる指定が必要です。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnStart(WIN32_FIND_DATA& _data, LPCTSTR lpszName)
	{
		if ( m_pDll != NULL )
		{
			Finish();
			m_hArc = m_pDll->OpenArchive(m_strArcFile, m_dwMode);
			if ( m_hArc != NULL )
			{
				int r;
				m_pDll->Func(r, "FindFirst", m_hArc, lpszName, &m_foundInfo);
				if ( r == 0 )
				{
					return m_MakeWin32Info(_data);
				}
			}
		}
		return false;
	}

	/**
	 * [通知] 次検索通知
	 *	@note Find() で true の場合、次にこのメソッドを使用します。その後繰り返して
	 *			本メソッド使うことで複数の情報を得ることが出来ます。
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnNext(WIN32_FIND_DATA& _data)
	{
		if ( m_pDll != NULL && m_hArc != NULL )
		{
			int r;
			m_pDll->Func(r, "FindNext", m_hArc, &m_foundInfo);
			if ( r == 0 )
			{
				return m_MakeWin32Info(_data);
			}
		}
		return false;
	}

	/**
	 * [通知] 検索終了通知
	 *	@note 検索中の場合、検索を停止します（ OnNext() メソッドは false を返すようになります）。
	 */
	virtual void OnFinish(void)
	{
		if ( m_pDll != NULL && m_hArc != NULL )
		{
			m_pDll->CloseArchive(m_hArc);
			m_hArc = NULL;
		}
	}
};



}; // TNB
