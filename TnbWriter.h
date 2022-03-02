#pragma once
/**
 *@file
 * ファイル関係のヘッダ
 *
 *		書き込みインターフェースがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbReader.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * 書き込みインターフェース
 *
 *	@note 書き込み動作をサポートするインターフェースです。
 *
 *	@par必要ファイル
 *			TnbWriter.h
 *
 *	@date 06/10/02 抽象化して新規作成
 *	@date 06/10/06 一部のメソッド名を変更。
 *	@date 06/12/19 IReader インターフェイス梱包。
 */
struct IWriter : IReader, ISerializer
{
	/**
	 * [確認] 書込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanWrite(void) const = 0;

	/**
	 * [保存] 書き込み
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	virtual void Write(size_t size, LPCVOID P) = 0;

	/**
	 * [設定] EOFを指定.
	 *		現在のファイルポインタの位置をEOFにします。
	 *	@throw CTnbException 書込みに失敗すると、スローされます。
	 */
	virtual void SetEnd(void) = 0;

	/**
	 * [保存] 書き込み.
	 *	@note 内部で一度 CVectorT<BYTE> に変換しています。
	 *	@param c 書き込みデータ。
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	void WriteFrom(const IConstCollectionT<BYTE>& c)
	{
		if ( ! CanWrite() )
		{
			throw CWriteFailureException(ERROR_INVALID_HANDLE); 
		}
		CVectorT<BYTE> v;
		v.Copy(c);
		Write(v.GetSize(), v.ReferBuffer());
	}

	/**
	 * [保存] 書き込み.
	 *	@note reader の現在の位置から読み出し、書き込みます。
	 *	@param reader 書き込みデータ 。
	 *	@param size 読み込みサイズ。省略か０ならファイル最後まで読み込む。
	 *	@param oneBlockSize 一度に読み込むサイズ。省略なら４K。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	void WriteFrom(const IReader& reader, size_t size = 0, size_t oneBlockSize = 4096)
	{
		if ( ! CanWrite() )
		{
			throw CWriteFailureException(ERROR_INVALID_HANDLE); 
		}
		CByteVector vb;
		if ( size == 0 )
		{
			size = ToIndex(reader.GetLeftoverSize());
		}
		while ( size > 0 )
		{
			size_t l = size;
			if ( l > oneBlockSize ){ l = oneBlockSize; }
			vb = reader.ReadInto(l);
			l = vb.GetSize();
			if ( l == 0 ){ break; }
			Write(vb.GetSize(), vb.ReferBuffer());
			size -= l;
		}
	}

	/**
	 * [追加] 追加.
	 *		指定データを追加します。
	 *	@param size 追加するデータサイズ(単位BYTE)
	 *	@param P 追加するデータ
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void AddRaw(size_t size, LPCVOID P)
	{
		Write(size, P);
	}
};



}; // TNB
