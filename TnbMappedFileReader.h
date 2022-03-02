#pragma once
/**
 *@file
 * マップドファイル読込み関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbReader.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * マップドファイルリーダークラス
 *
 *		ファイルをメモリにマップしアクセスできます（ポインタで参照可能）。
 *
 *	@par必要ファイル
 *			TnbMappedFileReader.h
 *
 *	@date 08/08/21 新規作成
 */
class CMappedFileReader : public IReader
{
public:

	/// コンストラクタ
	CMappedFileReader(void) : m_readPos(0), m_fileSize(0), m_pView(NULL), m_hFile(INVALID_HANDLE_VALUE), m_hMapping(NULL)
	{
	}

	/// デストラクタ
	~CMappedFileReader(void)
	{
		Close();
	}

	/**
	 * [操作] オープン
	 *	@note すでにオープンしていたらCloseしてからオープンします
	 *	@param fileName ファイル名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Open(LPCTSTR fileName)
	{
		Close();
		m_hFile = ::CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if ( m_hFile != INVALID_HANDLE_VALUE )
		{
			m_fileSize = ::GetFileSize(m_hFile, NULL);
			m_hMapping = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			if ( m_hMapping != NULL )
			{
				m_pView = static_cast<const BYTE*>(::MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, 0));
				if ( m_pView != NULL )
				{
					m_fileName = fileName;
					return true;
				}
			}
		}
		Close();
		return false;
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_fileName;
	}

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void)
	{
		if ( m_pView != NULL )
		{
			::UnmapViewOfFile(m_pView);
			m_pView = NULL;
		}
		if ( m_hMapping != NULL )
		{
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
		}
		if ( m_hFile != INVALID_HANDLE_VALUE) 
		{
			::CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
		m_readPos = 0;
		m_fileSize = 0;
		m_fileName.Empty();
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return m_pView != NULL;
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		return m_fileSize;
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
		LONGLONG l = m_fileSize;
		LONGLONG p = llOffset;
		switch ( eSeek )
		{
		case END:
			p += l;
			break;
		case CURRENT:
			p += m_readPos;
			break;
		}
		if ( p < 0 )
		{
			return -1;
		}
		else if ( p > l )
		{
			p = l;
		}
		m_readPos = static_cast<INDEX>(p);
		return p;
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
		size_t s = size;
		size_t l = m_fileSize - m_readPos;
		if ( s > l )
		{
			s = l;
		}
		MemCopy(static_cast<BYTE*>(_P), &m_pView[m_readPos], s);
		m_readPos += s;
		return s;
	}

	/**
	 * [参照] データアドレス参照.
	 *	@retval NULL オープンしていない
	 *	@retval NULL以外 データの先頭アドレス
	 */
	const BYTE* Refer(void) const
	{
		return m_pView;
	}

	/**
	 * [取得] データ取得.
	 *	@param index インデックス。0以上、 GetSize()の値未満を指定できます。
	 *	@retval NULL オープンしていない
	 *	@retval NULL以外 データの先頭アドレス
	 */
	BYTE operator[](INDEX index) const
	{
		if ( index < m_fileSize )
		{
			return m_pView[index];
		}
		return NULL;
	}

private:
	const BYTE*			m_pView;		///< マッピング用
	HANDLE				m_hFile;		///< マッピング用
	HANDLE				m_hMapping;		///< マッピング用
	size_t				m_fileSize;		///< ファイルサイズ
	CSimpleStr			m_fileName;		///< ファイル名
	mutable INDEX		m_readPos;		///< 読込み位置
};



}; //TNB
