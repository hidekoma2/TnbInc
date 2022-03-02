#pragma once
/**
 *@file
 * BIT管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */




#include "TnbStr.h"
#include "TnbComparable.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



/**@ingroup VALUE
 * Bit管理クラス
 *
 *		BIT数と BIT数分の情報を保持出来ます。
 *		各BITの計算、設定、連結、取り出しなどが容易に可能です。
 *
 *	@todo 現在1BITずつ処理しているため、それほど処理は早く有りません。
 *
 *	@par必要ファイル
 *			TnbBitSet.h
 *
 *	@date 06/08/10 新規作成
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
class CBitSet : public IComparableT<TNB::CBitSet>
{
	DEFSUPER(IComparableT<TNB::CBitSet>); 
	size_t	m_bitSize;							///< BIT の数
	BYTE*	m_pBuf;								///< バッファ

	/// BYTEバッファの確保数
	size_t m_BufSize(void) const
	{
		return (m_bitSize + 7) / 8;
	}
	/// BitNo計算
	INDEX m_BitNo(INDEX index) const
	{
		return index & 7;
	}
	/// ByteNo計算
	INDEX m_ByteNo(INDEX index) const
	{
		return index / 8;
	}
	/// BITテスト
	bool m_Test(INDEX index) const
	{
		return (m_pBuf[m_ByteNo(index)] & _BIT(m_BitNo(index))) != 0;
	}
	/// BIT セット
	void m_Set(INDEX index)
	{
		m_pBuf[m_ByteNo(index)] |= _BIT(m_BitNo(index));
	}
	/// BIT リセット
	void m_Reset(INDEX index)
	{
		m_pBuf[m_ByteNo(index)] &= ~_BIT(m_BitNo(index));
	}
	/// BIT 確定
	void m_Fix(INDEX index, bool b)
	{
		(b) ? m_Set(index) : m_Reset(index);
	}
	/// BIT 反転
	void m_Flip(INDEX index)
	{
		m_Test(index) ? m_Reset(index) : m_Set(index);
	}

public:

	/** 
	 * コンストラクタ
	 *	@param bitSize 保持できるBIT数を指定します。省略すると32 です。
	 *	@throw CNotSupportException 指定ビット数が０以下の場合、スローされます。
	 */
	explicit CBitSet(size_t bitSize = 32) : _super()
	{
		if ( bitSize == 0 )
		{
			throw CNotSupportException();
		}
		m_bitSize = bitSize;
		m_pBuf = new BYTE[m_BufSize()];
		memset(m_pBuf, 0, m_BufSize());
	}

	/** 
	 * コンストラクタ.
	 *		0と1の羅列の文字列からBIT数、内容を読み込みます。
	 *	@note ビット数は文字列長。内容は '0' かそれ以外で、BITのOn,Offを設定しています。
	 *	@param lpszBin 0と1の文字列。
	 *	@throw CNotSupportException 指定文字列長が０以下の場合、スローされます。
	 */
	explicit CBitSet(LPCTSTR lpszBin) : _super()
	{
		m_bitSize = STRLIB::GetLen(lpszBin);
		if ( m_bitSize == 0 )
		{
			throw CNotSupportException();
		}
		m_pBuf = new BYTE[m_BufSize()];
		memset(m_pBuf, 0, m_BufSize());
		loop ( i, m_bitSize )
		{
			if ( lpszBin[i] != '0' )
			{
				m_Set(m_bitSize - 1 - i);
			}
		}
	}

	/** 
	 * コピーコンストラクタ.
	 *		保持するBIT数、内容を複製します。
	 *	@param other コピー元
	 */
	CBitSet(const CBitSet& other) : _super()
	{
		m_bitSize = 0;
		m_pBuf = NULL;
		operator=(other);
	}

	/// デストラクタ
	virtual ~CBitSet(void)
	{
		if ( m_pBuf != NULL )
		{
			delete[] m_pBuf;
			m_pBuf = NULL;
		}
	}

	/** 
	 * [代入] コピーオペレータ.
	 *		保持するBIT数、内容を複製します。
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CBitSet& operator=(const CBitSet& other)
	{
		ASSERTLIB(other.m_bitSize > 0);
		if ( m_pBuf != NULL )
		{
			delete[] m_pBuf;
		}
		m_bitSize = other.m_bitSize;
		m_pBuf = new BYTE[m_BufSize()];
		MemCopy(m_pBuf, other.m_pBuf, m_BufSize());
		return *this;
	}

	/** 
	 * [代入] データ代入.
	 *		保持する BYTE数、内容を指定します。本クラスのサイズは、指定Byte数×８になります。
	 *	@param size コピー元データ長
	 *	@param P コピー元データ
	 *	@return 自分の参照
	 *	@throw CNotSupportException 指定文字列長が０以下の場合、スローされます。
	 */
	CBitSet& SetData(size_t size, LPCVOID P)
	{
		if ( size == 0 || P == NULL )
		{
			throw CNotSupportException();
		}
		if ( m_pBuf != NULL )
		{
			delete[] m_pBuf;
		}
		m_bitSize = size * 8;
		ASSERTLIB(m_BufSize() == size);
		m_pBuf = new BYTE[size];
		MemCopy(m_pBuf, static_cast<const BYTE*>(P), size);
		return *this;
	}

	/** 
	 * [代入] 数値代入.
	 *		保持するBIT数はそのままに、内容に格納します。
	 *	@note BIT数によっては値が入りきらないかもしれません。
	 *	@param ullValue 代入値
	 *	@return 自分の参照
	 */
	CBitSet& SetInteger(ULONGLONG ullValue)
	{
		Reset();
		if ( m_bitSize < sizeof(ULONGLONG) * 8 )
		{
			ullValue &= _BIT(m_bitSize) - 1;
		}
		size_t s = m_BufSize();
		if ( s >= 7 ){	m_pBuf[7] = static_cast<BYTE>(ullValue>>56); }
		if ( s >= 6 ){	m_pBuf[6] = static_cast<BYTE>(ullValue>>48); }
		if ( s >= 5 ){	m_pBuf[5] = static_cast<BYTE>(ullValue>>40); }
		if ( s >= 4 ){	m_pBuf[4] = static_cast<BYTE>(ullValue>>32); }
		if ( s >= 3 ){	m_pBuf[3] = static_cast<BYTE>(ullValue>>24); }
		if ( s >= 2 ){	m_pBuf[2] = static_cast<BYTE>(ullValue>>16); }
		if ( s >= 1 ){	m_pBuf[1] = static_cast<BYTE>(ullValue>> 8); }
						m_pBuf[0] = static_cast<BYTE>(ullValue    );
		return *this;
	}

	/** 
	 * [取得] 数値取り出し.
	 *		最大32BITまで取り出すことが可能です。
	 *	@return 数値
	 */
	UINT GetUnsignedInt(void) const
	{
		UINT r = 0;
		for ( INDEX i = 0; i < sizeof(UINT) * 8; i++ )
		{
			if ( Test(i) )
			{
				r |= (1 << i);
			}
		}
		return r;
	}

	/** 
	 * [取得] 数値取り出し.
	 *		最大32BITまで取り出すことが可能です。
	 *	@return 数値
	 */
	int GetSignedInt(void) const
	{
		int r = GetSignedInt();
		if ( Test(GetSize() - 1) )
		{
			//符号拡張の必要あり
			for ( size_t i = GetSize(); i < sizeof(int) * 8; i++ )
			{
				r |= (1 << i);
			}
		}
		return r;
	}

	/** 
	 * [取得] 数値取り出し.
	 *		最大64BITまで取り出すことが可能です。
	 *	@return 数値
	 */
	ULONGLONG GetUnsignedInt64(void) const
	{
		ULONGLONG r = 0;
		for ( INDEX i = 0; i < sizeof(ULONGLONG) * 8; i++ )
		{
			if ( Test(i) )
			{
				r |= (1ui64 << i);
			}
		}
		return r;
	}

	/** 
	 * [取得] 数値取り出し.
	 *		最大64BITまで取り出すことが可能です。
	 *	@return 数値
	 */
	LONGLONG GetSignedInt64(void) const
	{
		LONGLONG r = GetUnsignedInt64();
		if ( Test(GetSize() - 1) )
		{
			//符号拡張の必要あり
			for ( size_t i = GetSize(); i < sizeof(ULONGLONG) * 8; i++ )
			{
				r |= (1ui64 << i);
			}
		}
		return r;
	}

	/** 
	 * [取得] 文字列取り出し.
	 *		'0' '1' の羅列の文字列で内容を取り出すことが可能です。
	 *		この文字列は、コンストラクタで指定可能です。
	 *	@return 文字列。
	 */
	CStr ToString(void) const
	{
		CStr str;
		LPTSTR P = str.GetBuffer(m_bitSize + 1);
		for ( INDEX i = m_bitSize - 1; i >= 0; i-- )
		{
			*P++ = m_Test(i) ? _T('1') : _T('0');
		}
		*P = 0;
		str.ReleaseBuffer();
		return str;
	}

	/** 
	 * [取得] BIT数サイズ取得.
	 *		本インスタンスで使用可能なBIT数を返します。
	 *	@return BIT数サイズ
	 */
	size_t GetSize(void) const
	{
		return m_bitSize;
	}

	/** 
	 * [設定] BIT数サイズ設定.
	 *		本インスタンスで使用可能なBIT数を設定します。
	 *	@param size サイズ。
	 *	@return 自分の参照
	 */
	CBitSet& SetSize(size_t size)
	{
		if ( GetSize() != size )
		{
			CBitSet bs = GetMid(0, size);
			*this = bs;
		}
		return *this;
	}

	/** 
	 * [確認] BITの状態.
	 *	@param index BIT指定。０～(BIT数-1)で指定します。
	 *	@retval true セットされています('1')。
	 *	@retval false リセットされています('0')。
	 */
	bool Test(INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			return false;
		}
		return m_Test(index);
	}

	/** 
	 * [設定] BITセット.
	 *	@param index BIT指定。０～(BIT数-1)で指定します。
	 *	@return 自分の参照
	 */
	CBitSet& Set(INDEX index)
	{
		if ( IsInRange(index) )
		{
			m_Set(index); 
		}
		return *this;
	}

	/** 
	 * [設定] BITセット.
	 *	@note 全BITがセットされます。 
	 *	@return 自分の参照
	 */
	CBitSet& Set(void)
	{
		for ( INDEX i = 0; i < m_bitSize; i++ )
		{
			m_Set(i);
		}
		return *this;
	}

	/** 
	 * [設定] BITリセット.
	 *	@param index BIT指定。０～(BIT数-1)で指定します。
	 *	@return 自分の参照
	 */
	CBitSet& Reset(INDEX index)
	{
		if ( IsInRange(index) )
		{
			m_Reset(index);
		}
		return *this;
	}

	/** 
	 * [設定] BITリセット.
	 *	@note 全BITがリセットされます。 
	 *	@return 自分の参照
	 */
	CBitSet& Reset(void)
	{
		memset(m_pBuf, 0, m_BufSize());
		return *this;
	}

	/** 
	 * [設定] BIT反転.
	 *	@param index BIT指定。０～(BIT数-1)で指定します。
	 *	@return 自分の参照
	 */ 
	CBitSet& Flip(INDEX index)
	{
		if ( IsInRange(index) )
		{
			m_Flip(index);
		}
		return *this;
	}

	/** 
	 * [設定] BIT反転.
	 *	@note 全BITが反転します。 
	 *	@return 自分の参照
	 */ 
	CBitSet& Flip(void)
	{
		for ( INDEX i = 0; i<m_bitSize; i++ )
		{
			m_Flip(i);
		}
		return *this;
	}

	/** 
	 * [確認] INDEX有効チェック.
	 *		値が、０～(BIT数-1)の範囲か判定します。
	 *	@param index BIT指定。
	 *	@retval true 有効。
	 *	@retval false 無効。
	 */
	bool IsInRange(INDEX index) const
	{
		return TNB::IsInRange(index, m_bitSize);
	}

	/**
	 * [取得] 一部取り出し.
	 *		指定の位置から、指定BIT数分の情報を、新しい CBitSet に格納します。
	 *	@param index BIT指定。
	 *	@param bitSize 取り出すBIT数。
	 *	@return 指定BIT数の CBitSet
	 */
	CBitSet GetMid(INDEX index, size_t bitSize) const
	{
		CBitSet bs(bitSize);
		for ( INDEX i = 0; i < bitSize; i++ )
		{
			if ( Test(i + index) )
			{
				bs.m_Set(i);
			}
		}
		return bs;
	}

	/**
	 * [計算] 足し算
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 自分の参照
	 */
	CBitSet& operator+=(const CBitSet& t)
	{
		CBitSet bs = *this + t;
		return *this = bs;
	}

	/**
	 * [計算] AND計算
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 自分の参照
	 */
	CBitSet& operator&=(const CBitSet& t)
	{
		for ( INDEX i = 0; i < m_bitSize; i++ )
		{
			if ( ! t.Test(i) )
			{
				m_Reset(i);
			}
		}
		return *this;
	}

	/**
	 * [計算] OR計算
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 自分の参照
	 */
	CBitSet& operator|=(const CBitSet& t)
	{
		for ( INDEX i = 0; i < m_bitSize; i++ )
		{
			if ( t.Test(i) )
			{
				m_Set(i);
			}
		}
		return *this;
	}

	/**
	 * [計算] XOR計算
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 自分の参照
	 */
	CBitSet& operator^=(const CBitSet& t)
	{
		for ( INDEX i = 0; i < m_bitSize; i++ )
		{
			m_Fix(i, t.Test(i)^m_Test(i));
		}
		return *this;
	}

	/**
	 * [計算] 右シフト計算
	 *	@param dwPos シフト数
	 *	@return 自分の参照
	 *	@todo 未実装です。
	 */
	CBitSet& operator>>=(DWORD dwPos)
	{
		ASSERT0(false, "CBitSet::operator>>=()", "未実装");
		return *this;
	}

	/**
	 * [計算] 左シフト計算
	 *	@param dwPos シフト数
	 *	@return 自分の参照
	 *	@todo 未実装です。
	 */
	CBitSet& operator<<=(DWORD dwPos)
	{
		ASSERT0(false, "CBitSet::operator<<=()", "未実装");
		return *this;
	}

	/**
	 * [計算] 足し算結果取得
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 計算結果
	 */
	CBitSet operator+(const CBitSet& t)
	{
		size_t l1 = GetSize();
		size_t l2 = t.GetSize();
		CBitSet bs(l1 + l2);
		MemCopy(bs.m_pBuf, m_pBuf, m_BufSize());
		for ( INDEX i = 0; i < l2; i++ )
		{
			if ( t.Test(i) )
			{
				bs.m_Set(l1 + i);
			}
		}
		return bs;
	}

	/**
	 * [取得] AND計算結果取得
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 計算結果
	 */
	CBitSet operator&(const CBitSet& t) const
	{
		CBitSet bs(*this);
		return bs &= t;
	}

	/**
	 * [取得] OR計算結果取得
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 計算結果
	 */
	CBitSet operator|(const CBitSet& t) const
	{
		CBitSet bs(*this);
		return bs |= t;
	}

	/**
	 * [取得] XOR計算結果取得
	 *	@param t 計算相手。BITサイズが異なっていても構いません。
	 *	@return 計算結果
	 */
	CBitSet operator^(const CBitSet& t) const
	{
		CBitSet bs(*this);
		return bs ^= t;
	}

	/**
	 * [取得] 右シフト計算結果取得
	 *	@param dwPos シフト数
	 *	@return 計算結果
	 */
	CBitSet operator<<(DWORD dwPos) const
	{
		CBitSet bs(*this);
		return bs <<= dwPos;
	}

	/**
	 * [取得] 左シフト計算結果取得
	 *	@param dwPos シフト数
	 *	@return 計算結果
	 */
	CBitSet operator>>(DWORD dwPos) const
	{
		CBitSet bs(*this);
		return bs >>= dwPos;
	}

	/**
	 * [取得] BIT反転情報取得
	 *	@return BITが反転した CBitSet
	 */
	CBitSet operator~(void) const
	{
		CBitSet bs(*this);
		return bs.Flip();
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CBitSet& t) const
	{
		size_t size = (m_bitSize < t.m_bitSize) ? t.m_bitSize : m_bitSize;
		size_t l = size - 1;
		for ( INDEX i = 0; i < size; i++ )
		{
			int r = t.Test(l) - Test(l);
			if ( r != 0 )
			{
				return r;
			}
			l--;
		}
		return 0;
	}

private:
	friend class CBitSetTest;
};



/**@ingroup VALUE
 * [追加] シリアライズ
 *	@param[out] _sr シリアライザー
 *	@param[in] t データ
 *	@return ISerializer の参照
 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
 */
inline ISerializer& operator<<(ISerializer& _sr, const CBitSet& t)
{
	_sr << t.ToString();
	return _sr;
}

/**@ingroup VALUE
 * [取得] デシリアライズ
 *	@param[in] ds デシリアライザー
 *	@param[out] _t データ
 *	@return IDeserializer の参照
 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, CBitSet& _t)
{
	CStr s;
	ds >> s;
	_t = CBitSet(s);
	return ds;
}



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
