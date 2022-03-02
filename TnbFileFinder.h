#pragma once
/**
 *@file
 * ファイル検索関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ファイル検索抽象クラス
 *
 *		指定のディレクトリからファイルを検索するためのクラスです。
 *
 *	@par必要ファイル
 *			TnbFileFinder.h
 *
 *	@date 06/11/30 新規
 */
class CAbstractFileFinder : CCopyImpossible
{
public:
	/// 検索タイプ
	enum EFindType
	{
		ALL,			///< 全部対象
		FILE_ONLY,		///< ファイルのみ
		DIR_ONLY,		///< ディレクトリのみ
	};

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	/// イテレータ
	class iterator
	{
		CAbstractFileFinder*	m_pff;
	public:
		iterator(CAbstractFileFinder* pff) : m_pff(pff)
		{
			if ( m_pff != NULL && ! m_pff->IsFinding() )
			{
				m_pff = NULL;
			}
		}
		const WIN32_FIND_DATA* operator->(void)
		{
			if ( m_pff == NULL ) { return NULL; }
			return &(m_pff->Get());
		}
		const WIN32_FIND_DATA& operator*(void)
		{
			if ( m_pff == NULL ) { throw CEmptyException(); }
			return m_pff->Get();
		}
		iterator& operator++(void)
		{
			if ( m_pff != NULL && ! m_pff->Next() ) { m_pff = NULL; }
			return *this;
		}
		iterator& operator++(int)
		{
			return operator++();
		}
		bool operator==(const iterator& i) const
		{
			return m_pff == i.m_pff;
		}
		bool operator!=(const iterator& i) const
		{
			return ! operator==(i);
		}
	};
	#endif //_TnbDOXYGEN

	/**
	 * [反復] 先頭iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@note Find() で、成功してからイテレータを取得します。
	 *	@return iterator
	 */
	iterator begin(void)
	{
		return iterator(this);
	}

	/**
	 * [反復] 最後iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator end(void)
	{
		return iterator(NULL);
	}


	//-------------------------


	/// コンストラクタ
	CAbstractFileFinder(void) : m_isFinding(false)
	{
	}

	/// デストラクタ
	virtual ~CAbstractFileFinder(void) {}

	/**
	 * [設定] 検索終了.
	 *	@note 検索中の場合、検索を停止します（ Next() メソッドは false を返すようになります）。
	 */
	void Finish(void)
	{
		m_isFinding = false;
		m_strPath.Empty();
		m_strMask.Empty();
		OnFinish();
	}
	
	/**
	 * [検索] 検索開始
	 *	@note 検索は必ずこのメソッドから開始します。
	 *	@param lpszPath 検索パス指定。
	 *	@param lpszMask 検索マスク指定。ワイルドカードによる指定が必要です。省略すると *.* が指定されます。
	 *	@param type 検索タイプ。
	 *	@retval true 一つ以上発見。発見した内容は Get() メソッド等で取得可能。
	 *	@retval false 未発見。
	 */
	bool Start(LPCTSTR lpszPath, LPCTSTR lpszMask = _T("*.*"), EFindType type = ALL)
	{
		Finish();
		CStr s = lpszPath;
		if ( lpszMask != NULL )
		{
			if ( lpszMask[0] != '\\' && lpszMask[0] != '/' )
			{
				s += _T("\\");
			}
			s += lpszMask;
			s.Replace(_T("\\\\"), _T("\\"));
		}
		if ( OnStart(m_data, s) )
		{
			m_isFinding = true;
			m_strPath = lpszPath;
			m_strMask = lpszMask;
			m_type = type;
			if ( m_IsTarget(m_data, m_type) )
			{
				return true;
			}
			return Next();
		}
		Finish();
		return false;
	}

	/**
	 * [検索] 次検索
	 *	@note Find() で true の場合、次にこのメソッドを使用します。その後繰り返して
	 *			本メソッド使うことで複数の情報を得ることが出来ます。
	 *	@retval true 一つ以上発見。発見した内容はGetメソッドで取得可能。
	 *	@retval false 未発見。
	 */
	bool Next(void)
	{
		bool r = false;
		if ( IsFinding() )
		{
			while ( true )
			{
				r = OnNext(m_data);
				if ( ! r )
				{
					Finish();
					break;
				}
				else if ( m_IsTarget(m_data, m_type) )
				{
					break;
				}
			}
		}
		return r;
	}

	/**
	 * [確認] 検索中確認
	 *	@retval true 検索中。
	 *	@retval false していない。
	 */
	bool IsFinding(void) const
	{
		return m_isFinding;
	}

	/**
	 * [取得] 検索情報取得
	 *	@return ファイル情報の参照。
	 *	@throw CEmptyException 未検索、あるいは未発見時に本メソッドをコールすると、スローされます。
	 */
	const WIN32_FIND_DATA& Get(void) const
	{
		if ( ! IsFinding() ) { throw CEmptyException(); }
		return m_data;
	}

	/**
	 * [取得] 検索情報取得
	 *	@return ファイル情報の参照。
	 *	@throw CEmptyException 未検索、あるいは未発見時に本メソッドをコールすると、スローされます。
	 */
	const WIN32_FIND_DATA* operator->(void) const
	{
		return &Get();
	}

	/**
	 * [取得] 検索ファイル名取得
	 *	@return ファイル名
	 */
	CStr GetFoundName(void) const
	{
		if ( ! IsFinding() ) { return CStr(); }
		CStr s = m_strPath;
		s += _T("\\");
		s += Get().cFileName;
		s.Replace(_T("\\\\"), _T("\\"));
		s.TrimLeft('\\');
		return s;
	}

	/**
	 * [確認] ディレクトリか？
	 *	@note 検索中の時、発見した情報がディレクトリか否か調べることが出来ます。
	 *	@retval true ディレクトリ
	 *	@retval false ファイル or 未検索
	 */
	bool IsDirectory(void) const
	{
		if ( IsFinding() && (Get().dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			return true;
		}
		return false;
	}
#if 0
	/**
	 * [検索] 発見ディレクトリ下検索
	 *	@note 発見した情報がディレクトリの場合、その中の検索に移行します。
	 *	@retval NULL 失敗。
	 *	@retval NULL以外 成功。戻り値は、 Pop() に使用します。
	 */
	virtual HFINDPUSH Push(void) { return NULL; };

	/**
	 * [検索] 発見ディレクトリ下検索終了
	 *	@note Push() したときの状態に戻ります。
	 *	@param h Push() の戻り値。
	 */
	virtual void Pop(HFINDPUSH h) {};
#endif

protected:

	/**
	 * [通知] 検索開始通知
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@param[in] lpszName 検索パス指定。ワイルドカードによる指定が必要です。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnStart(WIN32_FIND_DATA& _data, LPCTSTR lpszName) = 0;

	/**
	 * [通知] 次検索通知
	 *	@note Find() で true の場合、次にこのメソッドを使用します。その後繰り返して
	 *			本メソッド使うことで複数の情報を得ることが出来ます。
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnNext(WIN32_FIND_DATA& _data) = 0;

	/**
	 * [通知] 検索終了通知
	 *	@note 検索中の場合、検索を停止します（ OnNext() メソッドは false を返すようになります）。
	 */
	virtual void OnFinish(void) = 0;

private:

	bool				m_isFinding;///< 検索中
	CStr				m_strPath;	///< 検索パス
	CStr				m_strMask;	///< 検索マスク
	EFindType			m_type;		///< 検索方法
	WIN32_FIND_DATA		m_data;		///< 検索結果 

	/**
	 * [確認] 対象チェック
	 *	@note 内部で使用するチェックメソッドです。
	 *	@param data ファイル情報
	 *	@param type チェック方法
	 *	@retval true 対象
	 *	@retval false 対象外
	 */
	bool m_IsTarget(const WIN32_FIND_DATA& data, EFindType type) const
	{
		if ( STRLIB::Compare(data.cFileName, _T("..")) == 0 )
		{
			return false;
		}
		if ( STRLIB::Compare(data.cFileName, _T(".")) == 0 )
		{
			return false;
		}
		switch ( type )
		{
		case FILE_ONLY:
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
			{
				return false;
			}
			break;
		case DIR_ONLY:
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
			{
				return false;
			}
			break;
		default:
			break;
		}
		return true;
	}
};



/**@ingroup FILE
 * ファイル検索クラス
 *
 *		指定のディレクトリからファイルを検索するためのクラスです。
 *		
 *	@note 検索時、ディレクトリ情報、[.][..]は、除外されます。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		CFileFinder ff;
 *		if ( ff.Start(_T(".\\*.*")) )
 *		{
 *			do
 *			{
 *				TRACE1("  found = %s\n", ff->cFileName);
 *			} while ( ff.Next() );
 *		}
 *		</pre></td></tr></table>
 *
 *	@par使用例(iterator使用)
 *		<table><tr><td><pre>
 *		CFileFinder ff;
 *		if ( ff.Start(_T(".\\*.*")) )
 *		{
 *			for ( CFileFinder::iterator i = ff.begin(); i != ff.end(); i++ )
 *			{
 *				TRACE1("  found = %s\n", i->cFileName);
 *			}
 *		}
 *		</pre></td></tr></table>
 *	@note iterator 操作時、元の CFileFinder が使用されます。
 *
 *	@par必要ファイル
 *			TnbFileFinder.h
 *
 *	@date 06/11/30 新規
 *	@date 08/03/23 構造変更。
 */
class CFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// コンストラクタ
	CFileFinder(void) : _super(), m_hFind(INVALID_HANDLE_VALUE)
	{
	}

	/// デストラクタ
	virtual ~CFileFinder(void)
	{
		Finish();
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
		Finish();
		m_hFind = ::FindFirstFile(lpszName, &_data);
		return (m_hFind != INVALID_HANDLE_VALUE);
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
		if ( m_hFind != INVALID_HANDLE_VALUE )
		{
			return !! ::FindNextFile(m_hFind, &_data);
		}
		return false;
	}

	/**
	 * [通知] 検索終了通知
	 *	@note 検索中の場合、検索を停止します（ OnNext() メソッドは false を返すようになります）。
	 */
	virtual void OnFinish(void)
	{
		if ( m_hFind != INVALID_HANDLE_VALUE )
		{
			::FindClose(m_hFind);
			m_hFind = INVALID_HANDLE_VALUE;
		}
	}
#if 0
	/**
	 * [検索] 発見ディレクトリ下検索
	 *	@note 発見した情報がディレクトリの場合、その中の検索に移行します。
	 *	@retval NULL 失敗。
	 *	@retval NULL以外 成功。戻り値は、 Pop() に使用します。
	 */
	virtual HFINDPUSH Push(void)
	{
		if ( IsDirectory() )
		{
			TPushParam* P = new TPushParam;
			P->hFind = m_hFind;
			P->strPath = m_strPath;
			P->data = m_data;
			//
			m_strPath += _T("\\");
			m_strPath += m_data.cFileName;
			m_hFind = ::FindFirstFile(m_strPath, &m_data);
			if ( m_hFind != INVALID_HANDLE_VALUE )
			{
				if ( ! IsTarget(m_data, m_type) )
				{
					if ( Next() )
					{
						return P;
					}
				}
			}
			m_hFind = P->hFind;
			m_strPath = P->strPath;
			m_data = P->data;
			delete P;
		}
		return NULL;
	}

	/**
	 * [検索] 発見ディレクトリ下検索終了
	 *	@note Push() したときの状態に戻ります。
	 *	@param h Push() の戻り値。
	 */
	virtual void Pop(HFINDPUSH h) 
	{
		if ( h != NULL )
		{
			TPushParam* P = reinterpret_cast<TPushParam*>(h);
			m_hFind = P->hFind;
			m_strPath = P->strPath;
			m_data = P->data;
			delete P;
		}
	};
#endif

private:
	HANDLE				m_hFind;	///< 検索ハンドル
	friend class CFileFinderTest;
};



}; //TNB

#if 0
struct WIN32_FIND_DATA
{
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	TCHAR cFileName[ MAX_PATH ];
	TCHAR cAlternateFileName[ 14 ];
} WIN32_FIND_DATA;

enum
{
	FILE_ATTRIBUTE_ARCHIVE				= 0x00000020, ///< アーカイブファイルまたはアーカイブディレクトリです。アプリケーションはこの属性を、ファイルのバックアップや削除のためのマークとして使います。
	FILE_ATTRIBUTE_COMPRESSED			= 0x00000800, ///< 指定されたファイルまたはディレクトリは圧縮されています。ファイルの場合、ファイル内の全データが圧縮されていることを意味します。ディレクトリの場合、そのディレクトリ内に新しく作成されるファイルまたはサブディレクトリが、既定で圧縮状態になることを意味します。
	FILE_ATTRIBUTE_DEVICE				= 0x00000040, ///< 予約済み。使わないでください。
	FILE_ATTRIBUTE_DIRECTORY			= 0x00000010, ///< 指定されたハンドルは、ディレクトリに関連しています。
	FILE_ATTRIBUTE_ENCRYPTED			= 0x00004000, ///< 指定されたファイルまたはディレクトリは暗号化されています。ファイルの場合、ファイル内の全データストリームが暗号化されていることを意味します。ディレクトリの場合、そのディレクトリ内に新しく作成されるファイルまたはサブディレクトリが、既定で暗号化状態になることを意味します。
	FILE_ATTRIBUTE_HIDDEN				= 0x00000002, ///< 隠しファイルまたは隠しディレクトリです。通常のディレクトリリスティングでは表示されません。
	FILE_ATTRIBUTE_NORMAL				= 0x00000080, ///< 指定されたファイルまたはディレクトリには、特に属性はありません。単独で返った場合にのみ、この属性は有効です。
	FILE_ATTRIBUTE_NOT_CONTENT_INDEXED	= 0x00002000, ///< Windows 2000：このファイルは、「インデックスサービス」の対象になっていません。
	FILE_ATTRIBUTE_OFFLINE				= 0x00001000, ///< Windows 2000：このファイルのデータは、すぐには利用できません。この属性は、ファイルのデータがオフラインの記憶装置へ物理的に移動されたことを示します。この属性は、Windows 2000 の階層記憶管理ソフトウェアである「リモート記憶域」が利用するものです。アプリケーションは、任意にこの属性を変更するべきではありません。
	FILE_ATTRIBUTE_READONLY				= 0x00000001, ///< このファイルまたはディレクトリは読み取り専用です。アプリケーションはこのファイルの読み取りを行えますが、書き込みや削除はできません。ディレクトリの場合、アプリケーションは削除を行えません。
	FILE_ATTRIBUTE_REPARSE_POINT		= 0x00000400, ///< このファイルには、再解析ポイントが関連付けられています。
	FILE_ATTRIBUTE_SPARSE_FILE			= 0x00000200, ///< このファイルは、スパースファイル（疎なファイル、未使用の領域が多い、または同じ値が長く続くファイル）です。
	FILE_ATTRIBUTE_SYSTEM				= 0x00000004, ///< このファイルまたはディレクトリは、オペレーティングシステムの一部、またはオペレーティングシステム専用です。
	FILE_ATTRIBUTE_TEMPORARY			= 0x00000100, ///< このファイルは、一時ファイルとして使われています。ファイルシステムは、データをハードディスクのような大容量記憶装置へ書き込む代わりに、高速なアクセスが行えるよう、すべてのデータをメモリ内に維持することを試みます。アプリケーションは、必要がなくなった段階で一時ファイルをすぐに削除するべきです。	
};
#endif