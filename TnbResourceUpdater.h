#pragma once
/**
 *@file
 * リソース更新関係のヘッダ
 *
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbFile.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// アイコンファイルクラス
class CIconFile
{
	#pragma pack( push )
	#pragma pack( 1 )

	/// アイコンファイル構造
	struct ICONDIR
	{
		WORD		idReserved;		///< 予約（０）
		WORD		idType;			///< タイプ（アイコン＝１)
		WORD		idCount;		///< イメージ数
		//    ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
	};
	/// アイコン各種情報
	struct ICONDIRENTRY
	{
		BYTE		bWidth;			///< Width, in pixels, of the image
		BYTE		bHeight;		///< Height, in pixels, of the image
		BYTE		bColorCount;	///< Number of colors in image (0 if >=8bpp)
		BYTE		bReserved;		///< Reserved ( must be 0)
		WORD		wPlanes;		///< Color Planes
		WORD		wBitCount;		///< Bits per pixel
		DWORD		dwBytesInRes;	///< How many bytes in this resource?
		DWORD		dwImageOffset;	///< Where in the file is this image?
	};

	/// 
	struct GRPICONDIRENTRY
	{
	   BYTE   bWidth;               // Width, in pixels, of the image
	   BYTE   bHeight;              // Height, in pixels, of the image
	   BYTE   bColorCount;          // Number of colors in image (0 if >=8bpp)
	   BYTE   bReserved;            // Reserved
	   WORD   wPlanes;              // Color Planes
	   WORD   wBitCount;            // Bits per pixel
	   DWORD   dwBytesInRes;         // how many bytes in this resource?
	   WORD   nID;                  // the ID
	};
	#pragma pack( pop )

	ICONDIR					m_iconDir;			// アイコンディレクトリ
	CWorkMemT<ICONDIRENTRY>	m_iconEntrys;		// アイコンディレクトリエントリ
	CWorkMemT<CWorkMem>		m_iconImages;		///< イメージグループデータ
	CWorkMem				m_iconGroupData;	///< イメージグループデータ

public:

	// コンストラクタ
	CIconFile()
	{
		Zero(m_iconDir);
	}

	// アイコンの持つイメージの数を取得する
	size_t GetImageCount() const
	{
		return m_iconDir.idCount;
	}

	// index番目のイメージデータを取得する
	BYTE* GetImageData(INDEX index)
	{
		ASSERT( index < GetImageCount());
		return m_iconImages[index].Ref();
	}

	// index番目のイメージサイズを取得する
	DWORD GetImageSize(INDEX index) const
	{
		ASSERT(index < GetImageCount());
		return m_iconEntrys[index].dwBytesInRes;
	}

	// アイコンファイルをロードする
	bool Load(LPCTSTR pszFileName)
	{
		// アイコンファイルのオープン
		HANDLE hFile = ::CreateFile(pszFileName, GENERIC_READ,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		bool r = (hFile != INVALID_HANDLE_VALUE);
		DWORD dwRead = 0;
		// ICONDIR部分の読み込み
		r &= !! ::ReadFile(hFile, &m_iconDir, sizeof(ICONDIR), &dwRead, NULL);
		ASSERT( m_iconDir.idReserved == 0 );
		ASSERT( m_iconDir.idType == 1 );
		ASSERT( m_iconDir.idCount > 0 );
		// ICONDIRENTRYの読み込み
		m_iconEntrys.Resize(m_iconDir.idCount);
		loop ( i, m_iconDir.idCount )
		{
			r &= !! ::ReadFile(hFile, &(m_iconEntrys[i]), sizeof(ICONDIRENTRY), &dwRead, NULL);
		}
		// イメージデータの読み込み
		m_iconImages.Resize(m_iconDir.idCount);
		loop ( i, m_iconDir.idCount )
		{
			::SetFilePointer(hFile, m_iconEntrys[i].dwImageOffset, NULL, FILE_BEGIN);
			m_iconImages[i].Resize(m_iconEntrys[i].dwBytesInRes);
			r &= !! ::ReadFile(hFile, m_iconImages[i], m_iconEntrys[i].dwBytesInRes, &dwRead, NULL);
		}
		::CloseHandle(hFile);
		return r;
	}

	// アイコングループデータのサイズ取得
	size_t SizeOfIconGroupData() const
	{
		return sizeof(ICONDIR) + sizeof(GRPICONDIRENTRY) * GetImageCount();
	}

	// アイコングループデータの作成（nBaseIDはRT_ICONの基底インデックス番号）
	BYTE* CreateIconGroupData(int nBaseID)
	{
		// アイコングループデータの領域確保
		m_iconGroupData.Resize(SizeOfIconGroupData());
		// アイコングループディレクトリを書き込む
		memcpy(m_iconGroupData, &m_iconDir, sizeof(ICONDIR));
		int offset = sizeof(ICONDIR);
		loop ( i, GetImageCount() )
		{
			GRPICONDIRENTRY grpEntry;
			// イメージデータからBITMAPINFOHEADER取得
			BITMAPINFOHEADER bitmapheader;
			memcpy(&bitmapheader, GetImageData(i), sizeof(BITMAPINFOHEADER));
			// アイコングループエントリ作成
			grpEntry.bWidth			= m_iconEntrys[i].bWidth;
			grpEntry.bHeight		= m_iconEntrys[i].bHeight;
			grpEntry.bColorCount	= m_iconEntrys[i].bColorCount;
			grpEntry.bReserved		= m_iconEntrys[i].bReserved;
			grpEntry.wPlanes		= bitmapheader.biPlanes;
			grpEntry.wBitCount		= bitmapheader.biBitCount;
			grpEntry.dwBytesInRes	= m_iconEntrys[i].dwBytesInRes;
			grpEntry.nID			= static_cast<WORD>(nBaseID + i);
			// アイコングループエントリを書き込む
			memcpy(m_iconGroupData.Ref() + offset, &grpEntry, sizeof(GRPICONDIRENTRY));
			offset += sizeof(GRPICONDIRENTRY);
		}
		return m_iconGroupData.Ref();
	}
};

#endif // _TnbDOXYGEN



/**@ingroup FILE
 * リソース更新関係クラス.
 *
 *	ファイルに付加しているリソースを更新できます。
 *
 *	@par必要ファイル
 *			TnbResourceUpdater.h
 *
 *	@date 08/08/04 新規作成
 */
class CResourceUpdater : CCopyImpossible
{
	HANDLE		m_hUpdate;	///< 更新用ハンドル

	/// クローズ
	bool m_Close(bool isDiscard)
	{
		bool r = false;
		if ( m_hUpdate != NULL )
		{
			r = !! ::EndUpdateResource(m_hUpdate, isDiscard);
			m_hUpdate = NULL;
		}
		return r;
	}
public:

	/// コンストラクタ
	CResourceUpdater(void) : m_hUpdate(NULL)
	{
	}

	/// デストラクタ
	~CResourceUpdater(void)
	{
		m_Close(true);
	}

	/**
	 * [確認] 有効か
	 *	@retval true Openに成功している
	 *	@retval false Openしていない。
	 */
	bool IsValid(void) const
	{
		return m_hUpdate != NULL;
	}

	/**
	 * [設定] オープン
	 *	@param lpszFile 対象のファイル名。
	 *	@param isDeleteExistingResources 対象のファイルのパラメータの既存のリソースを削除するかどうか指定します。
	 *					true を渡すと、既存のリソースが削除され、更新後の実行可能ファイルには追加したリソースだけが含まれます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Open(LPCTSTR lpszFile, bool isDeleteExistingResources = false)
	{
		m_Close(true);
		m_hUpdate = ::BeginUpdateResource(lpszFile, isDeleteExistingResources);
		return IsValid();
	}

	/**
	 * [設定] 書き込み
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool WritePush(void)
	{
		return m_Close(false);
	}

	/**
	 * [変更] アイコン変更
	 *	@param resourceId リソースID
	 *	@param lpszIconFile アイコンファイル
	 *	@param wLanguage 言語ID
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool ModifyIcon(UINT resourceId, LPCTSTR lpszIconFile, WORD wLanguage = 0)
	{
		bool r = false;
		if ( IsValid() )
		{ 
			CIconFile iconFile;
			r = iconFile.Load(lpszIconFile);
			// RT_GROUP_ICON 書き込み
			r &= !! ::UpdateResource
				(
					m_hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(resourceId), wLanguage,
					iconFile.CreateIconGroupData(resourceId), ToDword(iconFile.SizeOfIconGroupData())
				);
			// RT_ICON書き込み
			loop ( i, iconFile.GetImageCount() )
			{
				r &= !! ::UpdateResource
					(
						m_hUpdate, RT_ICON, MAKEINTRESOURCE(resourceId+i), wLanguage,
						iconFile.GetImageData(i), iconFile.GetImageSize(i)
					);
			}
		}
		return r;
	}
};



}; // TNB



