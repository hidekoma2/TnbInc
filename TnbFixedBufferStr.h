#pragma once
/**
 *@file
 * 固定バッファ文字列管理関係のヘッダ
 *
 *		固定バッファ上で文字列を管理するクラスが有ります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup STRING ITERATOR
 * 固定バッファ文字列管理テンプレート
 *
 *		固定バッファに文字列を格納し管理するクラスです。
 *		通信パケット上の NULL 終端のない文字列などに適用できます。
 *
 *	@param LEN バッファの固定長
 *	@param PADDING 文字列長が足りない時に保管する文字。省略すると ￥０です。
 *
 *	@par必要ファイル
 *			TnbFixedBufferStr.h
 *
 *	@date 13/01/17	新規作成
 *	@date 16/02/18	CopyImpossible を削除
 */
template<int LEN, char PADDING = 0>
class CFixedBufferStrT
{
public:

	/// 定数
	enum 
	{
		MAXIMUM_LENGTH = LEN,	///< バッファ長
		PADDING_CHAR = PADDING	///< パディング文字
	};

	/// コンストラクタ.
	CFixedBufferStrT(void)
	{
		Empty();
	}

	/**
	 * 代入コンストラクタ.
	 *	@param lpsz 代入する文字列
	 */
	CFixedBufferStrT(LPCSTR lpsz)
	{
		Set(lpsz);
	}

	/**
	 * 代入オペレータ
	 *	@param other 代入する文字列
	 *	@return 自分の参照
	 */
	CFixedBufferStrT& operator=(const CFixedBufferStrT& other)
	{
		Set(other.Get());
		return *this;
	}

	/**
	 * 代入オペレータ
	 *	@param lpsz 代入する文字列
	 *	@return 自分の参照
	 */
	CFixedBufferStrT& operator=(LPCSTR lpsz)
	{
		Set(lpsz);
		return *this;
	}

	/**
	 * [代入] 文字列代入.
	 *		保持可能な文字列を超えた場合、超えた分は破棄されます。
	 *	@param lpsz 代入する文字列
	 */
	void Set(LPCSTR lpsz)
	{
		CAscii s;
		s.SetFromLeft(lpsz, LEN);
		ASSERT( s.GetLength() <= LEN );
		Empty();
		memcpy(m_dat, s, s.GetLength());
	}

	/**
	 * [設定] 空化.
	 *		保持している情報を全部、 PADDING_CHAR 文字にします。
	 */
	void Empty(void)
	{
		loop ( i, LEN )
		{
			m_dat[i] = PADDING_CHAR;
		}
	}

	/**
	 * [取得] 文字列取得.
	 */
	CAscii Get(void) const
	{
		CAscii s;
		s.SetFromLeft(m_dat, LEN);
		return s;
	}

	/**
	 * [参照] 文字列先頭参照.
	 *	@attention NULL終端されていない文字列の可能性があります。
	 *	@return 文字列の先頭
	 */
	LPCSTR Refer(void) const
	{
		return m_dat;
	}

	/**
	 * [取得] 文字列長.
	 *	@return 文字列長
	 */
	size_t GetLength(void) const
	{
		CAscii s;
		s.SetFromLeft(m_dat, LEN);
		return s.GetLength();
	}

	/**
	 * [取得] 最大文字列長.
	 *		格納できる最大長を返します。
	 *	@return 最大文字列長
	 */
	size_t GetMaximumLength(void) const
	{
		return LEN;
	}

private:
	char	m_dat[LEN];	///< データ
};



}; // TNB