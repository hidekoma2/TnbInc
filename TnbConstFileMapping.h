#pragma once
/**
 *@file
 * ファイルマッピング処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFileName.h"
#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT FILE
 * コンストファイルマッピングテンプレートクラス
 *
 *		実体をファイル上に持つコレクションクラスです。そのため、 TYP に指定できる
 *		型が制限されますが、 4G を超える情報量をコレクションできます（ただし、インデックスは０～4G まで）。
 *
 *	@param TYP	保持する型。メモリイメージをそのままファイルにするため、基本データ型、および基本データ型のみの構造体に限ります。
 *
 *	@see	IConstCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbConstFileMapping.h
 *
 *	@date 09/03/18	新規作成
 *	@date 11/08/24	構造を一新。 ISequenceCollectionT インターフェース実装、廃止。
 *	@date 11/08/30	マッピングサイズも指定可能に。
 */
template<typename TYP>
class CConstFileMappingT : public IConstCollectionT<TYP>, CCopyImpossible
{
	DEFSUPER(IConstCollectionT<TYP>);
public:

	/**
	 * コンストラクタ.
	 *	@param multi マッピングするサイズの倍率を指定。通常、２．
	 */
	CConstFileMappingT(DWORD multi = 2) : _super(), m_pType(NULL), m_hMapping(NULL), m_nowPage(-1), m_offset(0), m_size(0)
	{
		SYSTEM_INFO si;
		::GetSystemInfo(&si);
		m_pageSize = si.dwAllocationGranularity * multi;
	}

	/**
	 * デストラクタ.
	 */
	~CConstFileMappingT(void)
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
	 *	@note オープンしていないと、Get,Refer系メソッドは無意味な値を返します。
	 *	@retval true オープン済み
	 *	@retval false 未オープン。
	 */
	bool IsOpened(void) const
	{
		return m_reader.CanRead();
	}

	/**
	 * [操作] オープン.
	 *	@param lpszName 読み取り対象のファイル名。
	 *	@param offset コレクション開始のファイルポインタ。ファイルの先頭にヘッダなどがあり、それ以降を配列とする場合に有効です。
	 *	@param size コレクション数。省略すると、ファイルサイズから判断します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Open(LPCTSTR lpszName, LONGLONG offset = 0, size_t size = INVALID_SIZE)
	{
		Close();
		m_offset = offset;
		if ( m_reader.Open(lpszName, true) )
		{
			m_size = ToIndex((m_reader.GetSize() - m_offset) / sizeof(TYP));
			if ( size != INVALID_SIZE )
			{
				if ( m_size > size )
				{
					m_size = size;
				}
			}
			return true;
		}
		return false;
	}

	/**
	 * [操作] クローズ.
	 *		オープンしたファイルを開放します。
	 */
	void Close(void)
	{
		m_Unmapping();
		m_reader.Close();
		m_size = 0;
	}

	/**
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
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
			m_hMapping = ::CreateFileMapping(m_reader, NULL, PAGE_READONLY, 0, 0, NULL);
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
			LONGLONG ns = min(m_pageSize * 2, m_reader.GetSize() - pl);
			m_pType = static_cast<BYTE*>(::MapViewOfFile(m_hMapping, FILE_MAP_READ, static_cast<DWORD>(pl >> 32), static_cast<DWORD>(pl), down_cast<DWORD>(ns)));
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

	CFileReader			m_reader;		///< ファイル読みこみクラス
	DWORD				m_pageSize;		///< ページサイズ
	CSyncSection		m_sync;			///< 同期オブジェクト
	LONGLONG			m_offset;		///< オフセット
	size_t				m_size;			///< データ数
	mutable HANDLE		m_hMapping;		///< マッピングハンドル
	mutable BYTE*		m_pType;		///< マッピングアドレス
	mutable LONGLONG	m_nowPage;		///< 現在のマッピングページ
	friend class CConstFileMappingTest;
};



/**@ingroup FILE
 * Constファイルマッピングクラス
 *
 *	@note	TYP は BYTE の CConstFileMappingT です。
 *
 *	@par必要ファイル
 *			TnbConstFileMapping.h
 *
 *	@date 09/03/18 新規作成
 */
typedef CConstFileMappingT<BYTE> CConstFileMapping;



}; // TNB
