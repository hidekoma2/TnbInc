#pragma once
/**
 *@file
 * 分数関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbComparable.h"



namespace TNB
{



/**@ingroup VALUE
 * 分数管理.
 *		分母と分子の2つの値で分数を管理します。
 *		四則演算にも対応しています。
 *
 *	@param TYP 要素の型。
 *
 *	@par必要ファイル
 *			TnbFraction.h
 *
 *	@date 16/04/15 新規
 */
template<typename TYP = int>
class CFractionT : public IComparableT<CFractionT<TYP> >
{
public:

	/** 
	 * コンストラクタ
	 *	@param num 分子
	 *	@param den 分母
	 */
	CFractionT(TYP num, TYP den) : m_numerator(num), m_denominator(den)
	{
	}

	/**
	 * [設定] 値設定.
	 *	@param num 分子
	 *	@param den 分母
	 */
	void Set(TYP num, TYP den)
	{
		m_numerator = num;
		m_denominator = den;
	}

	/**
	 * [代入] 代入.
	 *	@param other 代入元
	 *	@return 自分の参照
	 */
	CFractionT& operator=(const CFractionT& other)
	{
		m_numerator = other.m_numerator;
		m_denominator = other.m_denominator;
		return *this;
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CFractionT& t) const
	{
		TYP t1 = m_numerator * t.m_denominator;
		TYP t2 = t.m_numerator * m_denominator;
		if ( t1 == t2 )
		{
			return 0;
		}
		return (t1 > t2) ? 1 : -1;
	}

	/**
	 * [代入] 加算代入.
	 *	@param other 加算数
	 *	@return 自分の参照
	 */
	CFractionT& operator+=(const CFractionT& other)
	{
		if ( m_denominator == other.m_denominator )
		{
			m_numerator += other.m_numerator;
		}
		else
		{
			m_denominator *= other.m_denominator;
			m_numerator *= other.m_denominator;
			m_numerator += other.m_numerator * other.m_denominator;
		}
		return LeastCommonDivisor();
	}

	/**
	 * [代入] 減算代入.
	 *	@param other 減算数
	 *	@return 自分の参照
	 */
	CFractionT& operator-=(const CFractionT& other)
	{
		if ( m_denominator == other.m_denominator )
		{
			m_numerator -= other.m_numerator;
		}
		else
		{
			m_denominator *= other.m_denominator;
			m_numerator *= other.m_denominator;
			m_numerator -= other.m_numerator * other.m_denominator;
		}
		return LeastCommonDivisor();
	}

	/**
	 * [代入] 乗算代入.
	 *	@param other 乗算数
	 *	@return 自分の参照
	 */
	CFractionT& operator*=(const CFractionT& other)
	{
		m_numerator *= other.m_numerator;
		m_denominator *= other.m_denominator;
		return LeastCommonDivisor();
	}

	/**
	 * [代入] 乗算代入.
	 *	@param val 乗算数
	 *	@return 自分の参照
	 */
	CFractionT& operator*=(TYP val)
	{
		m_numerator *= val;
		return LeastCommonDivisor();
	}

	/**
	 * [代入] 除算代入.
	 *	@param other 除算数
	 *	@return 自分の参照
	 */
	CFractionT& operator/=(const CFractionT& other)
	{
		m_numerator *= other.m_denominator;
		m_denominator *= other.m_numerator;
		return LeastCommonDivisor();
	}

	/**
	 * [代入] 除算代入.
	 *	@param val 除算数
	 *	@return 自分の参照
	 */
	CFractionT& operator/=(TYP val)
	{
		m_denominator *= val;
		return LeastCommonDivisor();
	}

	/**
	 * [設定] マイナス化.
	 *	@return 自分の参照
	 */
	CFractionT& operator-(void)
	{
		m_numerator = -m_numerator;
		return *this;
	}

	/**
	 * [計算] 加算.
	 *	@param other 加算数
	 *	@return 結果
	 */
	CFractionT operator+(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r += other);
	}

	/**
	 * [計算] 減算.
	 *	@param other 減算数
	 *	@return 結果
	 */
	CFractionT operator-(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r -= other);
	}

	/**
	 * [計算] 乗算.
	 *	@param other 乗算数
	 *	@return 結果
	 */
	CFractionT operator*(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r *= other);
	}

	/**
	 * [計算] 乗算.
	 *	@param val 乗算数
	 *	@return 結果
	 */
	CFractionT operator*(TYP val) const
	{
		CFractionT r = *this;
		return (r *= val);
	}

	/**
	 * [計算] 除算.
	 *	@param other 除算数
	 *	@return 結果
	 */
	CFractionT operator/(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r /= other);
	}

	/**
	 * [計算] 除算.
	 *	@param val 除算数
	 *	@return 結果
	 */
	CFractionT operator/(TYP val) const
	{
		CFractionT r = *this;
		return (r /= val);
	}

	/**
	 * [取得] 分子取得.
	 *		保持している分子を返します。
	 *	@return 分子
	 */
 	TYP GetNumerator(void) const
	{
		return m_numerator;
	}

	/**
	 * [取得] 分母取得.
	 *		保持している分母を返します。
	 *	@return 分母
	 */
	TYP GetDenominator(void) const
	{
		return m_denominator;
	}

	/**
	 * [取得] 実数取得.
	 *		保持している、分子を分母で割った値を返します。
	 *	@return 実数
	 */
	double Get(void) const
	{
		double r = m_numerator;
		return r / m_denominator;
	}

	/**
	 * [変換] 最小公倍.
	 *		分母と分子を最小公倍数で割る。
	 */

	CFractionT& LeastCommonDivisor(void)
	{
		TYP r = ms_gcd(m_numerator, m_denominator);	//分子と分母の最小公倍数
		if ( r == 0 )
		{
			; // 何もしない
		}
		else
		{
			m_numerator /= r;
			m_denominator /= r;
		}
		return *this;
	}

private:

	/// 最大公約数（Greatest Common Divisor）
	static TYP ms_gcd(TYP m, TYP n)
	{
		if ( 0 == m || 0 == n )
		{
			return 0; // 引数に０がある場合は０を返す
		}
		if ( m < n )
		{
			Swap(m, n);
		}
		// ユークリッドの方法
		while ( m != n )
		{
			(m > n) ? (m -= n) : (n -= m);
		}
		return m;
	}

	/// 最小公倍数（Least Common Multiple）
	static TYP ms_lcm(TYP m, TYP n)
	{
		if ( 0 == m || 0 == n )
		{
			return 0; // 引数に０がある場合は０を返す
		}
		if ( m < n )
		{
			Swap(m, n);
		}
		return ((m / ms_gcd(m, n)) * n); // lcm = m * n / gcd(m,n)
	}

	TYP	m_numerator;	///< 分子
	TYP	m_denominator;	///< 分母
	friend class CFractionTest;
};



/**@ingroup VALUE
 * 分数管理.
 *	@note 分母と分子を int で管理しています。
 *	@see CFractionT このテンプレートの TYP は「int」になります。
 *	@par必要ファイル
 *			TnbFraction.h
 */
typedef CFractionT<int> CFraction;



}; // TNB