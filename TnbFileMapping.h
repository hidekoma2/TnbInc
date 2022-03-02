#pragma once
/**
 *@file
 * ファイルマッピング処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFile.h"
#include "TnbTemporaryFile.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT FILE 
 * ファイルマッピングテンプレートクラス
 *
 *		実体をファイル上に持つコレクションクラスです。そのため、 TYP に指定できる
 *		型が制限されますが、 4G を超える情報量をコレクションできます（ただし、インデックスは０～4G まで）。
 *
 *	@attention 挿入、削除処理は、ファイルに対してアクセスが発生するため、速度的にはよく有りません。
 *
 *	@attention Open() メソッドでファイル名を指定した場合、本インスタンスがなくなっても、ファイルは残ります。
 *
 *	@note コレクションインターフェースを持っていますが、インスタンスのコピーは認めていません。
 *
 *	@param TYP	保持する型。メモリイメージをそのままファイルにするため、基本データ型、および基本データ型のみの構造体に限ります。
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbFileMapping.h
 *
 *	@date 06/05/08 新規作成
 *	@date 11/08/24 構造を一新。 ISequenceCollectionT インターフェース実装、廃止。
 */
template<typename TYP>
class CFileMappingT : public ICollectionT<TYP>, CCopyImpossible
{
	DEFSUPER(ICollectionT<TYP>);
public:

	/**
	 * コンストラクタ.
	 *	@param multi マッピングするサイズの倍率を指定。通常、２．
	 */
	CFileMappingT(DWORD multi = 2) : _super(), m_pType(NULL), m_hMapping(NULL), m_nowPage(-1), m_isSeeked(false), m_offset(0)
	{
		SYSTEM_INFO si;
		::GetSystemInfo(&si);
		m_pageSize = si.dwAllocationGranularity * multi;
	}

	/**
	 * デストラクタ.
	 *		アタッチしたファイルを開放します。
	 *	@note Open() の際、指定したファイルはそのままです。必要に応じて削除してください。
	 *		Open() の際、ファイル名を省略した場合、テンポラリファイルは削除されます。
	 */
	~CFileMappingT(void)
	{
		Close();
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ダミー。省略してください。
	 *	@return 常にtrue。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return m_sync.Lock(); 
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const 
	{
		m_sync.Unlock();
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン済み
	 *	@retval false 未オープン
	 */
	bool IsOpened(void) const
	{
		return m_writer.CanWrite();
	}

	/**
	 * [操作] オープン.
	 *	@param lpszName データ記憶用に使うファイル名。省略するとテンポラリフォルダに任意の名前で作成します。
	 *	@param offset コレクション開始位置。ファイルの先頭にヘッダなどがあり、それ以降を配列とする場合に有効です。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Open(LPCTSTR lpszName = NULL, LONGLONG offset = 0)
	{
		m_offset = offset;
		CStr n = lpszName;
		if ( n.IsEmpty() )
		{
			n = m_tempFileName.Create();
		}
		return m_writer.Open(n, false, true);
	}

	/**
	 * [操作] クローズ.
	 *		オープンしたファイルを閉じます。
	 *	@note Open() の際、指定したファイルはそのままです。必要に応じて削除してください。
	 *		Open() の際、ファイル名を省略した場合、テンポラリファイルは削除されます。
	 */
	void Close(void)
	{
		m_Unmapping();
		m_writer.Close();
		m_tempFileName.Free();
	}

	/**
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return ToIndex((m_writer.GetSize() - m_offset) / sizeof(TYP));
	}

	/**
	 * [設定] 要素数設定.
	 *	@note 大きくすることも小さくすることも出来ます。
	 *	@param size サイズ
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool SetSize(size_t size)
	{
		m_Unmapping();
		if ( m_writer.Seek(m_IndexToFilePos(size)) < 0 )
		{
			return false;
		}
		m_writer.SetEnd();
		m_isSeeked = true;
		return true;
	}

	/**
	 * [取得] 要素の取得.
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
	 */
	virtual TYP Get(INDEX index) const
	{
		return At(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@attention 参照先はインスタンス内のワークのアドレスを指しています。
	 *			次の At() により先の内容が変化する可能性が有りますので、
	 *			必要に応じて値をコピーしておくようにしてください。
	 *	@param index インデックス
	 *	@return 一時的な参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		const TYP* P = m_Mapping(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 
	 */
	virtual TYP& Ref(INDEX index)
	{
		TYP* P = m_Mapping(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [取得] 要素の取得.
	 *		Get() と同じ動作です。
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& operator[](INDEX index) const
	{
		return At(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照オブジェクト。これを操作するとファイルに影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP& operator[](INDEX index)
	{
		return Ref(index);
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@param t 要素
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t)
	{
		m_Unmapping();
		m_writer.Write(sizeof(TYP), &t);
		return true; 
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 RemoveEx() を使用してください。
	 *	@param index インデックス
	 *	@retval false	エラー。
	 *	@retval true	成功。
	 */
	virtual bool Remove(INDEX index)
	{
		if ( IsInRange(index) )
		{
			size_t nl = GetSize();
			loop ( i, (nl - index) - 1 )
			{
				INDEX d = i + index;
				Set(d, Ref(d + 1));
			}
			return SetSize(nl - 1);
		}
		return false;
	}

	/**
	 * [削除] 全要素削除 .
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool RemoveAll(void)
	{
		m_Unmapping();
		if ( ! m_writer.CanWrite() )
		{
			return false;
		}
		m_writer.Seek(m_offset);
		m_writer.SetEnd();
		return true;
	}

	/**
	 * [設定] 要素の設定.
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		if ( IsInRange(index) )
		{
			Ref(index) = t;
			return true;
		}
		return false;
	}

private:
	/// インデックス→ファイルポインタ
	LONGLONG m_IndexToFilePos(INDEX index) const
	{
		LONGLONG l = index;
		return l * sizeof(TYP) + m_offset;
	}
	/// アンマッピング
	void m_Unmapping(void)
	{
		if ( m_pType != NULL )
		{
			::UnmapViewOfFile(m_pType);
			m_pType = NULL;
			m_nowPage = -1;
		}
		if ( m_hMapping != NULL )
		{
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
		}
		if ( m_isSeeked )
		{
			m_isSeeked = false;
			m_writer.Seek(0, CFileWriter::END);
		}
	}
	/// マッピング
	TYP* m_Mapping(INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			return NULL;
		}
		if ( m_hMapping == NULL )
		{
			m_hMapping = ::CreateFileMapping(m_writer, NULL, PAGE_READWRITE, 0, 0, NULL);
			if ( m_hMapping == NULL )
			{
				return NULL;
			}
		}
		LONGLONG fl = m_IndexToFilePos(index);
		LONGLONG pl = fl / m_pageSize * m_pageSize;
		if ( pl != m_nowPage )
		{
			if ( m_pType != NULL )
			{
				::UnmapViewOfFile(m_pType);
				m_pType = NULL;
			}
			m_nowPage = pl;
			LONGLONG ns = min(m_pageSize * 2, m_writer.GetSize() - pl);
			m_pType = static_cast<BYTE*>(::MapViewOfFile(m_hMapping, FILE_MAP_ALL_ACCESS, static_cast<DWORD>(pl >> 32), static_cast<DWORD>(pl), down_cast<DWORD>(ns)));
			if ( m_pType == NULL )
			{
				_GetLastError("MapViewOfFile");
				::CloseHandle(m_hMapping);
				m_hMapping = NULL;
				return NULL;
			}
		}
		return reinterpret_cast<TYP*>(&m_pType[fl - pl]);
	}

	CFileWriter			m_writer;		///< ファイル書き込みクラス
	CTemporaryFile		m_tempFileName;	///< テンポラリファイル名
	DWORD				m_pageSize;		///< ページサイズ
	CSyncSection		m_sync;			///< 同期オブジェクト
	bool				m_isSeeked;		///< シークが必要？
	LONGLONG			m_offset;		///< コレクション開始位置
	mutable HANDLE		m_hMapping;		///< マッピングハンドル
	mutable BYTE*		m_pType;		///< マッピングアドレス
	mutable LONGLONG	m_nowPage;		///< 現在のマッピングページ
};



/**@ingroup FILE
 * ファイルマッピングクラス
 *
 *	@note	TYP は BYTE の CFileMappingT です。
 *
 *	@par必要ファイル
 *			TnbFileMapping.h
 *
 *	@date 06/10/25 新規作成
 */
typedef CFileMappingT<BYTE> CFileMapping;



}; // TNB
