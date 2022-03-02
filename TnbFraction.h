#pragma once
/**
 *@file
 * �����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbComparable.h"



namespace TNB
{



/**@ingroup VALUE
 * �����Ǘ�.
 *		����ƕ��q��2�̒l�ŕ������Ǘ����܂��B
 *		�l�����Z�ɂ��Ή����Ă��܂��B
 *
 *	@param TYP �v�f�̌^�B
 *
 *	@par�K�v�t�@�C��
 *			TnbFraction.h
 *
 *	@date 16/04/15 �V�K
 */
template<typename TYP = int>
class CFractionT : public IComparableT<CFractionT<TYP> >
{
public:

	/** 
	 * �R���X�g���N�^
	 *	@param num ���q
	 *	@param den ����
	 */
	CFractionT(TYP num, TYP den) : m_numerator(num), m_denominator(den)
	{
	}

	/**
	 * [�ݒ�] �l�ݒ�.
	 *	@param num ���q
	 *	@param den ����
	 */
	void Set(TYP num, TYP den)
	{
		m_numerator = num;
		m_denominator = den;
	}

	/**
	 * [���] ���.
	 *	@param other �����
	 *	@return �����̎Q��
	 */
	CFractionT& operator=(const CFractionT& other)
	{
		m_numerator = other.m_numerator;
		m_denominator = other.m_denominator;
		return *this;
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
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
	 * [���] ���Z���.
	 *	@param other ���Z��
	 *	@return �����̎Q��
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
	 * [���] ���Z���.
	 *	@param other ���Z��
	 *	@return �����̎Q��
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
	 * [���] ��Z���.
	 *	@param other ��Z��
	 *	@return �����̎Q��
	 */
	CFractionT& operator*=(const CFractionT& other)
	{
		m_numerator *= other.m_numerator;
		m_denominator *= other.m_denominator;
		return LeastCommonDivisor();
	}

	/**
	 * [���] ��Z���.
	 *	@param val ��Z��
	 *	@return �����̎Q��
	 */
	CFractionT& operator*=(TYP val)
	{
		m_numerator *= val;
		return LeastCommonDivisor();
	}

	/**
	 * [���] ���Z���.
	 *	@param other ���Z��
	 *	@return �����̎Q��
	 */
	CFractionT& operator/=(const CFractionT& other)
	{
		m_numerator *= other.m_denominator;
		m_denominator *= other.m_numerator;
		return LeastCommonDivisor();
	}

	/**
	 * [���] ���Z���.
	 *	@param val ���Z��
	 *	@return �����̎Q��
	 */
	CFractionT& operator/=(TYP val)
	{
		m_denominator *= val;
		return LeastCommonDivisor();
	}

	/**
	 * [�ݒ�] �}�C�i�X��.
	 *	@return �����̎Q��
	 */
	CFractionT& operator-(void)
	{
		m_numerator = -m_numerator;
		return *this;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param other ���Z��
	 *	@return ����
	 */
	CFractionT operator+(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r += other);
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param other ���Z��
	 *	@return ����
	 */
	CFractionT operator-(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r -= other);
	}

	/**
	 * [�v�Z] ��Z.
	 *	@param other ��Z��
	 *	@return ����
	 */
	CFractionT operator*(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r *= other);
	}

	/**
	 * [�v�Z] ��Z.
	 *	@param val ��Z��
	 *	@return ����
	 */
	CFractionT operator*(TYP val) const
	{
		CFractionT r = *this;
		return (r *= val);
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param other ���Z��
	 *	@return ����
	 */
	CFractionT operator/(const CFractionT& other) const
	{
		CFractionT r = *this;
		return (r /= other);
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param val ���Z��
	 *	@return ����
	 */
	CFractionT operator/(TYP val) const
	{
		CFractionT r = *this;
		return (r /= val);
	}

	/**
	 * [�擾] ���q�擾.
	 *		�ێ����Ă��镪�q��Ԃ��܂��B
	 *	@return ���q
	 */
 	TYP GetNumerator(void) const
	{
		return m_numerator;
	}

	/**
	 * [�擾] ����擾.
	 *		�ێ����Ă��镪���Ԃ��܂��B
	 *	@return ����
	 */
	TYP GetDenominator(void) const
	{
		return m_denominator;
	}

	/**
	 * [�擾] �����擾.
	 *		�ێ����Ă���A���q�𕪕�Ŋ������l��Ԃ��܂��B
	 *	@return ����
	 */
	double Get(void) const
	{
		double r = m_numerator;
		return r / m_denominator;
	}

	/**
	 * [�ϊ�] �ŏ����{.
	 *		����ƕ��q���ŏ����{���Ŋ���B
	 */

	CFractionT& LeastCommonDivisor(void)
	{
		TYP r = ms_gcd(m_numerator, m_denominator);	//���q�ƕ���̍ŏ����{��
		if ( r == 0 )
		{
			; // �������Ȃ�
		}
		else
		{
			m_numerator /= r;
			m_denominator /= r;
		}
		return *this;
	}

private:

	/// �ő���񐔁iGreatest Common Divisor�j
	static TYP ms_gcd(TYP m, TYP n)
	{
		if ( 0 == m || 0 == n )
		{
			return 0; // �����ɂO������ꍇ�͂O��Ԃ�
		}
		if ( m < n )
		{
			Swap(m, n);
		}
		// ���[�N���b�h�̕��@
		while ( m != n )
		{
			(m > n) ? (m -= n) : (n -= m);
		}
		return m;
	}

	/// �ŏ����{���iLeast Common Multiple�j
	static TYP ms_lcm(TYP m, TYP n)
	{
		if ( 0 == m || 0 == n )
		{
			return 0; // �����ɂO������ꍇ�͂O��Ԃ�
		}
		if ( m < n )
		{
			Swap(m, n);
		}
		return ((m / ms_gcd(m, n)) * n); // lcm = m * n / gcd(m,n)
	}

	TYP	m_numerator;	///< ���q
	TYP	m_denominator;	///< ����
	friend class CFractionTest;
};



/**@ingroup VALUE
 * �����Ǘ�.
 *	@note ����ƕ��q�� int �ŊǗ����Ă��܂��B
 *	@see CFractionT ���̃e���v���[�g�� TYP �́uint�v�ɂȂ�܂��B
 *	@par�K�v�t�@�C��
 *			TnbFraction.h
 */
typedef CFractionT<int> CFraction;



}; // TNB