#pragma once
/**
 *@file
 * �͈͊Ǘ��֌W�̃w�b�_
 *
 *		����l�Ƃ���l���y�A�ŊǗ�����N���X���L��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSerializer.h"
#include "TnbComparable.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * �͈͊Ǘ��e���v���[�g�N���X
 *
 *		����l�Ƃ���l���y�A�ŊǗ�����e���v���[�g�N���X�ł��B
 *
 *	@param TYP ��r�@�\�̂���^���w��ł��܂��B
 *	@param DIF �uTYP-TYP=DIF�v�uTYP+DIF=TYP�v�ɂȂ�悤�Ȍ^���w�肵�܂��B 
 *
 *	@par�K�v�t�@�C��
 *			TnbRange.h
 *
 *	@date 06/10/13	�V�K�쐬
 */
template<typename TYP, typename DIF=TYP>
class CRangeT : public IComparableT< TNB::CRangeT<TYP,DIF> >, public ISerializable 
{
	TYP		m_first;	///< �l�̊J�n(m_last�Ɠ�����������)
	TYP		m_last;		///< �l�̏I��(m_first�Ɠ������傫��)

	/// �ێ����Ă���l���e���r���A�召
	void m_CheckRange(void)
	{
		if ( m_first > m_last )
		{
			Swap(m_first, m_last);
			ASSERTLIB(m_first <= m_last);
		}
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param t �l
	 */
	CRangeT(const TYP& t = TYP()) : m_first(t), m_last(t) 
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note First,Last�͋t�ł��\���܂���( �����I�ɊǗ������ւ��܂�)�B
	 *	@param t1 �l�P
	 *	@param t2 �l�Q
	 */
	CRangeT(const TYP& t1, const TYP& t2) : m_first(t1), m_last(t2)
	{
		m_CheckRange();
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[���B
	 */
	CRangeT(const CRangeT& other) : m_first(other.m_first), m_last(other.m_last)
	{
		ASSERTLIB(m_first <= m_last);
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[���B
	 *	@return �����̎Q��
	 */
	CRangeT& operator=(const CRangeT& other)
	{
		m_first = other.m_first;
		m_last	= other.m_last;
		ASSERTLIB(m_first <= m_last);
		return *this;
	}

	/**
	 * [���] ���
	 *	@note First,Last�͋t�ł��\���܂���( �����I�ɊǗ������ւ��܂�)�B
	 *	@param t1 �l�P
	 *	@param t2 �l�Q
	 */
	void Set(const TYP& t1, const TYP& t2)
	{
		m_first = t1;
		m_last	= t2;
		m_CheckRange();
	}

	/**
	 * [���] First�l���
	 *	@attention Last�l���傫���l��ݒ肷��ƁA�����I�ɊǗ������ւ��܂��B
	 *	@param t �l
	 */
	void SetFirst(const TYP& t)
	{
		m_first = t;
		m_CheckRange();
	}

	/**
	 * [���] Last�l���
	 *	@attention First�l��菬�����l��ݒ肷��ƁA�����I�ɊǗ������ւ��܂��B
	 *	@param t �l
	 */
	void SetLast(const TYP& t)
	{
		m_first = t;
		m_CheckRange();
	}

	/**
	 * [���] Diff�l���
	 *	@note Diff�l���w�肷�邱�ƂŁAFirst�l����_�ɁALast�l��ݒ肵�܂��B
	 *			(Last�l = First�l + Diff�l)
	 *	@param d �l
	 */
	void SetRange(const DIF& d)
	{
		m_last = m_first + d;
		m_CheckRange();
	}

	/**
	 * [�擾] First�l�擾
	 *	@return �l
	 */
	TYP GetFirst(void)
	{
		return m_first;
	}

	/**
	 * [�擾] First�l�擾
	 *	@return �l
	 */
	const TYP& GetFirst(void) const
	{
		return m_first;
	}

	/**
	 * [�擾] Last�l�擾
	 *	@return �l
	 */
	TYP GetLast(void)
	{
		return m_last;
	}

	/**
	 * [�擾] Last�l�擾
	 *	@return �l
	 */
	const TYP& GetLast(void) const
	{
		return m_last;
	}

	/**
	 * [�v�Z] Diff�l�擾
	 *	@note Last�l - First�l���Ԃ�܂��B
	 *	@return Diff�l
	 */
	DIF Difference(void) const
	{
		return m_last - m_first;
	}

	/**
	 * [�擾] �����v�Z
	 *	@attention DIF �̌^������Z���T�|�[�g���Ă��邱�Ƃ��K�v�ł��B
	 *	@return �������w���l
	 */
	TYP Center(void) const
	{
		TYP r = m_first;
		return r += (Difference() / 2);
	}

	/**
	 * [�v�Z] ���Z
	 *	@note First�l Last�l�Ɏw��Diff�l�������܂��B
	 *	@param dif Diff�l
	 *	@return �����̎Q��
	 */
	CRangeT& operator+=(const DIF& dif)
	{
		m_first += dif;
		m_last	+= dif;
		m_CheckRange();
		return *this;
	}

	/**
	 * [�擾] ���Z
	 *	@note First�l Last�l�Ɏw��Diff�l�������� CRangeT ���쐬���܂��B
	 *	@param rng �Ώ�
	 *	@param dif Diff�l
	 *	@return ����
	 */
	inline friend CRangeT operator+(const CRangeT& rng, const DIF& dif)
	{
		CRangeT r = rng;
		return r += dif;
	}

	/**
	 * [�v�Z] ���Z
	 *	@note First�l Last�l�Ɏw��Diff�l�������܂��B
	 *	@param dif Diff�l
	 *	@return �����̎Q��
	 */
	CRangeT& operator-=(const DIF& dif)
	{
		m_first -= dif;
		m_last	-= dif;
		m_CheckRange();
		return *this;
	}

	/**
	 * [�擾] ���Z
	 *	@note First�l Last�l�Ɏw��Diff�l�������� CRangeT ���쐬���܂��B
	 *	@param rng �Ώ�
	 *	@param dif Diff�l
	 *	@return ����
	 */
	inline friend CRangeT operator-(const CRangeT& rng, const DIF& dif)
	{
		CRangeT r = rng;
		return r -= dif;
	}

	/**
	 * [�m�F] �͈͓��m�F
	 *	@param t �l
	 *	@retval true �͈͓�
	 *	@retval false �͈͊O
	 */
	bool IsInRange(const TYP& t) const
	{
		return m_first <= t && m_last >= t;
	}

	/**
	 * [�m�F] �͈͓��m�F
	 *	@note �Ώۂ��{�C���X�^���X�͈̔͂ɑS�Ċ܂܂�邩�m�F���܂��B
	 *	@param rng �Ώ�
	 *	@retval true �͈͓�
	 *	@retval false �͈͊O
	 */
	bool IsInRange(const CRangeT& rng) const
	{
		return m_first <= rng.m_first && m_last >= rng.m_last;
	}

	/**
	 * [�m�F] �͈͓��m�F
	 *	@param t �l
	 *	@retval true �͈͓�
	 *	@retval false �͈͊O
	 */
	bool InRange(const TYP& t) const
	{
		return IsInRange(t);
	}

	/**
	 * [�m�F] �͈͓��m�F
	 *	@note �Ώۂ��{�C���X�^���X�͈̔͂ɑS�Ċ܂܂�邩�m�F���܂��B
	 *	@param rng �Ώ�
	 *	@retval true �͈͓�
	 *	@retval false �͈͊O
	 */
	bool InRange(const CRangeT& rng) const
	{
		return IsInRange(rng);
	}

	/// �֌W�^�C�v
	enum ERelationType
	{
		UNRELATED,	///< ���֌W
		TOUCHED,	///< �ꕔ�ڐG
		WARPPED		///< ����
	};

	/**
	 * [�m�F] �֌W�m�F
	 *	@note �����ƑΏۂ̊֌W�𒲂ׂ܂��B
	 *	@param rng �Ώ�
	 *	@retval UNRELATED	���֌W�B�d�Ȃ��Ă��镔���͂���܂���B
	 *	@retval WARPPED		����B�Е����S�Ċ܂�ł��܂��B
	 *	@retval TOUCHED		�ꕔ�ڐG�B�ꕔ�d�Ȃ��Ă��镔��������܂�( Intersect() �Ŏ��o�����Ƃ��o���܂�)�B
	 */
	ERelationType GetRelation(const CRangeT& rng) const
	{
		CRangeT r1 = *this;
		CRangeT r2 = rng;
		if ( *this > rng )
		{
			r1 = rng;
			r2 = *this;
		}
		if ( r1.m_last >= r2.m_last )
		{
			return WARPPED;
		}
		if ( r1.m_last >= r2.m_first )
		{
			if ( r1.m_first == r2.m_first )
			{
				return WARPPED;
			}
			return TOUCHED;
		}
		return UNRELATED;
	}

	/**
	 * [�擾] �d���͈͎擾
	 *	@note �����ƑΏۂŏd�����Ă���͈͂����� CRangeT �������܂��B
	 *	@param rng �Ώ�
	 *	@return ����
	 *	@throw CEmptyException �d���͈͂��Ȃ��ꍇ�A�X���[����܂��B
	 */
	CRangeT Intersect(const CRangeT& rng) const
	{
		CRangeT r1 = *this;
		CRangeT r2 = rng;
		if ( *this > rng )
		{
			r1 = rng;
			r2 = *this;
		}
		ERelationType rt = r1.GetRelation(r2);
		if ( rt == UNRELATED )
		{
			throw CEmptyException(); //���֌W�B�d�Ȃ��Ă��镔���͂���܂���B
		}
		else if ( rt == WARPPED )
		{
			//����B�Е����S�Ċ܂�ł��܂��B
			if ( r1.m_first == r2.m_first )
			{
				return r1;
			}
			return r2;
		}
		return CRangeT(r2.m_first, r1.m_last);
	}

	/**
	 * [�擾] ����͈͎擾
	 *	@note �����ƑΏۂ����������� CRangeT �������܂��B
	 *	@param rng �Ώ�
	 *	@return ����
	 */
	CRangeT Union(const CRangeT& rng) const
	{
		TYP t1 = min(m_first, rng.m_first);
		TYP t2 = max(m_last, rng.m_last);
		return CRangeT(t1, t2);
	}

	/**
	 * [�m�F] ��r
	 *	@note First�l�������ꍇ�ALast�l���r����B
	 *	@param rng ��r�Ώ�
	 *	@retval �O		������ rng �Ɠ���
	 *	@retval �P�ȏ�	������ rng ���傫��
	 *	@retval �O����	������ rng ��菬����
	 */
	virtual INT_PTR Compare(const CRangeT& rng) const
	{
		INT_PTR r = IComparatorT<TYP>::GetDefault().CompareTo(m_first, rng.m_first);
		return (r != 0) ? r : IComparatorT<TYP>::GetDefault().CompareTo(m_last, rng.m_last);
	}

	/**
	 * [�ʒm] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << m_first << m_last;
	}

	/**
	 * [�ʒm] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> m_first >> m_last;
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
