#pragma once
/**
 *@file
 * ファイル関係のヘッダ
 *
 *		読み込みインターフェースがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSerializer.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * 読み込みインターフェース
 *
 *	@note 読み込み動作をサポートするインターフェースです。
 *
 *	@par必要ファイル
 *			TnbFile.h
 *
 *	@date 06/10/02 抽象化して新規作成
 *	@date 06/10/06 一部のメソッド名を変更。
 */
struct IReader : IDeserializer
{
	/// シークのオフセット指定モード
	enum ESeekMode
	{
		TOP		= FILE_BEGIN,		///< 頭からのオフセット指定
		CURRENT	= FILE_CURRENT,		///< 現在位置からのオフセット指定
		END		= FILE_END			///< 末からのオフセット指定
	};

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const = 0;

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void) = 0;

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const = 0;

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const = 0;

	/**
	 * [操作] シーク.
	 *		アクセスポインタを移動します。
	 *	@param llOffset アクセスポインタの移動数。
	 *	@param eSeek 移動開始アクセスポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const = 0;

	/**
	 * [取得] 読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const = 0;

	/**
	 * [取得] 残りサイズ取得.
	 *		現在の位置から最後までのサイズを返します。
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	LONGLONG GetLeftoverSize(void) const
	{
		LONGLONG lc = Seek(0, CURRENT);
		LONGLONG le = GetSize();
		if ( lc >= 0 && le >= 0 )
		{
			return le - lc;
		}
		return -1;
	}

	/**
	 * [取得] 読み込み.
	 *	@note 指定サイズ読めないとエラーになります。
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 *	@throw CShortOfDataException 指定サイズ読込めないと、スローされます。
	 */
	void ReadExactly(size_t size, LPVOID _P) const
	{
		if ( ! CanRead() )
		{ 
			throw CReadFailureException(ERROR_INVALID_HANDLE); 
		}
		if ( Read(size, _P) != size )
		{
			throw CShortOfDataException();
		}
	}

	/**
	 * [取得] 読み込み
	 *	@note 指定サイズ読めないとエラーになります。
	 *	@param size 読み込みサイズ。0ならファイル最後まで読み込む。
	 *	@return ByteVector	ファイル内容（サイズは読み込めたサイズ）。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 *	@throw CShortOfDataException 指定サイズ読込めないと、スローされます。
	 */
	CByteVector ReadExactly(size_t size = 0) const
	{
		CByteVector vb = ReadInto(size);
		if ( size > 0 && size != vb.GetSize() )
		{
			throw CShortOfDataException();
		}
		return vb;
	}

	/**
	 * [取得] 読み込み
	 *	@param size 読み込みサイズ。0ならファイル最後まで読み込む。
	 *	@return ByteVector	ファイル内容（サイズは読み込めたサイズ）。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	CByteVector ReadInto(size_t size = 0) const
	{
		if ( ! CanRead() )
		{
			throw CReadFailureException(ERROR_INVALID_HANDLE); 
		}
		CByteVector vb;
		if ( size == 0 )
		{
			size = ToIndex(GetLeftoverSize());
		}
		if ( size > 0 )
		{
			size_t i = Read(size, vb.GetBuffer(size));
			vb.ReleaseBuffer();
			vb.SetSize(i);
		}
		return vb;
	}

	/**
	 * [取得] 読み込み.
	 *	@param[out] _out 読込先。クリアせず、後ろに追加します。
	 *	@param size 読み込みサイズ。省略か0ならファイル最後まで読み込む。
	 *	@param oneBlockSize 一度に読み込むサイズ。省略なら４K。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	size_t ReadInto(ICollectionT<BYTE>& _out, size_t size = 0, size_t oneBlockSize = 4096) const
	{
		if ( ! CanRead() )
		{
			throw CReadFailureException(ERROR_INVALID_HANDLE); 
		}
		CByteVector vb;
		if ( size == 0 )
		{
			size = ToIndex(GetLeftoverSize());
		}
		size_t r = 0;
		while ( size > 0 )
		{
			size_t l = size;
			if ( l > oneBlockSize ){ l = oneBlockSize; }
			vb = ReadInto(l);
			l = vb.GetSize();
			if ( l == 0 ){ break; }
			_out.Append(vb);
			size -= l;
			r += l;
		}
		return r;
	}

	/**
	 * [取得] 取得.
	 *		指定データを取得します。
	 *	@param[in] size 取得するデータサイズ(単位BYTE)
	 *	@param[out] _P 取得するデータ
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void GetRaw(size_t size, LPVOID _P) const
	{
		ReadExactly(size, _P);
	}
};



}; //TNB