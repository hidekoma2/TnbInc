#pragma once
/**
 *@file
 * BIT�Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */




#include "TnbStr.h"
#include "TnbComparable.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



/**@ingroup VALUE
 * Bit�Ǘ��N���X
 *
 *		BIT���� BIT�����̏���ێ��o���܂��B
 *		�eBIT�̌v�Z�A�ݒ�A�A���A���o���Ȃǂ��e�Ղɉ\�ł��B
 *
 *	@todo ����1BIT���������Ă��邽�߁A����قǏ����͑����L��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitSet.h
 *
 *	@date 06/08/10 �V�K�쐬
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
class CBitSet : public IComparableT<TNB::CBitSet>
{
	DEFSUPER(IComparableT<TNB::CBitSet>); 
	size_t	m_bitSize;							///< BIT �̐�
	BYTE*	m_pBuf;								///< �o�b�t�@

	/// BYTE�o�b�t�@�̊m�ې�
	size_t m_BufSize(void) const
	{
		return (m_bitSize + 7) / 8;
	}
	/// BitNo�v�Z
	INDEX m_BitNo(INDEX index) const
	{
		return index & 7;
	}
	/// ByteNo�v�Z
	INDEX m_ByteNo(INDEX index) const
	{
		return index / 8;
	}
	/// BIT�e�X�g
	bool m_Test(INDEX index) const
	{
		return (m_pBuf[m_ByteNo(index)] & _BIT(m_BitNo(index))) != 0;
	}
	/// BIT �Z�b�g
	void m_Set(INDEX index)
	{
		m_pBuf[m_ByteNo(index)] |= _BIT(m_BitNo(index));
	}
	/// BIT ���Z�b�g
	void m_Reset(INDEX index)
	{
		m_pBuf[m_ByteNo(index)] &= ~_BIT(m_BitNo(index));
	}
	/// BIT �m��
	void m_Fix(INDEX index, bool b)
	{
		(b) ? m_Set(index) : m_Reset(index);
	}
	/// BIT ���]
	void m_Flip(INDEX index)
	{
		m_Test(index) ? m_Reset(index) : m_Set(index);
	}

public:

	/** 
	 * �R���X�g���N�^
	 *	@param bitSize �ێ��ł���BIT�����w�肵�܂��B�ȗ������32 �ł��B
	 *	@throw CNotSupportException �w��r�b�g�����O�ȉ��̏ꍇ�A�X���[����܂��B
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
	 * �R���X�g���N�^.
	 *		0��1�̗���̕����񂩂�BIT���A���e��ǂݍ��݂܂��B
	 *	@note �r�b�g���͕����񒷁B���e�� '0' ������ȊO�ŁABIT��On,Off��ݒ肵�Ă��܂��B
	 *	@param lpszBin 0��1�̕�����B
	 *	@throw CNotSupportException �w�蕶���񒷂��O�ȉ��̏ꍇ�A�X���[����܂��B
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
	 * �R�s�[�R���X�g���N�^.
	 *		�ێ�����BIT���A���e�𕡐����܂��B
	 *	@param other �R�s�[��
	 */
	CBitSet(const CBitSet& other) : _super()
	{
		m_bitSize = 0;
		m_pBuf = NULL;
		operator=(other);
	}

	/// �f�X�g���N�^
	virtual ~CBitSet(void)
	{
		if ( m_pBuf != NULL )
		{
			delete[] m_pBuf;
			m_pBuf = NULL;
		}
	}

	/** 
	 * [���] �R�s�[�I�y���[�^.
	 *		�ێ�����BIT���A���e�𕡐����܂��B
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
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
	 * [���] �f�[�^���.
	 *		�ێ����� BYTE���A���e���w�肵�܂��B�{�N���X�̃T�C�Y�́A�w��Byte���~�W�ɂȂ�܂��B
	 *	@param size �R�s�[���f�[�^��
	 *	@param P �R�s�[���f�[�^
	 *	@return �����̎Q��
	 *	@throw CNotSupportException �w�蕶���񒷂��O�ȉ��̏ꍇ�A�X���[����܂��B
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
	 * [���] ���l���.
	 *		�ێ�����BIT���͂��̂܂܂ɁA���e�Ɋi�[���܂��B
	 *	@note BIT���ɂ���Ă͒l�����肫��Ȃ���������܂���B
	 *	@param ullValue ����l
	 *	@return �����̎Q��
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
	 * [�擾] ���l���o��.
	 *		�ő�32BIT�܂Ŏ��o�����Ƃ��\�ł��B
	 *	@return ���l
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
	 * [�擾] ���l���o��.
	 *		�ő�32BIT�܂Ŏ��o�����Ƃ��\�ł��B
	 *	@return ���l
	 */
	int GetSignedInt(void) const
	{
		int r = GetSignedInt();
		if ( Test(GetSize() - 1) )
		{
			//�����g���̕K�v����
			for ( size_t i = GetSize(); i < sizeof(int) * 8; i++ )
			{
				r |= (1 << i);
			}
		}
		return r;
	}

	/** 
	 * [�擾] ���l���o��.
	 *		�ő�64BIT�܂Ŏ��o�����Ƃ��\�ł��B
	 *	@return ���l
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
	 * [�擾] ���l���o��.
	 *		�ő�64BIT�܂Ŏ��o�����Ƃ��\�ł��B
	 *	@return ���l
	 */
	LONGLONG GetSignedInt64(void) const
	{
		LONGLONG r = GetUnsignedInt64();
		if ( Test(GetSize() - 1) )
		{
			//�����g���̕K�v����
			for ( size_t i = GetSize(); i < sizeof(ULONGLONG) * 8; i++ )
			{
				r |= (1ui64 << i);
			}
		}
		return r;
	}

	/** 
	 * [�擾] ��������o��.
	 *		'0' '1' �̗���̕�����œ��e�����o�����Ƃ��\�ł��B
	 *		���̕�����́A�R���X�g���N�^�Ŏw��\�ł��B
	 *	@return ������B
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
	 * [�擾] BIT���T�C�Y�擾.
	 *		�{�C���X�^���X�Ŏg�p�\��BIT����Ԃ��܂��B
	 *	@return BIT���T�C�Y
	 */
	size_t GetSize(void) const
	{
		return m_bitSize;
	}

	/** 
	 * [�ݒ�] BIT���T�C�Y�ݒ�.
	 *		�{�C���X�^���X�Ŏg�p�\��BIT����ݒ肵�܂��B
	 *	@param size �T�C�Y�B
	 *	@return �����̎Q��
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
	 * [�m�F] BIT�̏��.
	 *	@param index BIT�w��B�O�`(BIT��-1)�Ŏw�肵�܂��B
	 *	@retval true �Z�b�g����Ă��܂�('1')�B
	 *	@retval false ���Z�b�g����Ă��܂�('0')�B
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
	 * [�ݒ�] BIT�Z�b�g.
	 *	@param index BIT�w��B�O�`(BIT��-1)�Ŏw�肵�܂��B
	 *	@return �����̎Q��
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
	 * [�ݒ�] BIT�Z�b�g.
	 *	@note �SBIT���Z�b�g����܂��B 
	 *	@return �����̎Q��
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
	 * [�ݒ�] BIT���Z�b�g.
	 *	@param index BIT�w��B�O�`(BIT��-1)�Ŏw�肵�܂��B
	 *	@return �����̎Q��
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
	 * [�ݒ�] BIT���Z�b�g.
	 *	@note �SBIT�����Z�b�g����܂��B 
	 *	@return �����̎Q��
	 */
	CBitSet& Reset(void)
	{
		memset(m_pBuf, 0, m_BufSize());
		return *this;
	}

	/** 
	 * [�ݒ�] BIT���].
	 *	@param index BIT�w��B�O�`(BIT��-1)�Ŏw�肵�܂��B
	 *	@return �����̎Q��
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
	 * [�ݒ�] BIT���].
	 *	@note �SBIT�����]���܂��B 
	 *	@return �����̎Q��
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
	 * [�m�F] INDEX�L���`�F�b�N.
	 *		�l���A�O�`(BIT��-1)�͈̔͂����肵�܂��B
	 *	@param index BIT�w��B
	 *	@retval true �L���B
	 *	@retval false �����B
	 */
	bool IsInRange(INDEX index) const
	{
		return TNB::IsInRange(index, m_bitSize);
	}

	/**
	 * [�擾] �ꕔ���o��.
	 *		�w��̈ʒu����A�w��BIT�����̏����A�V���� CBitSet �Ɋi�[���܂��B
	 *	@param index BIT�w��B
	 *	@param bitSize ���o��BIT���B
	 *	@return �w��BIT���� CBitSet
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
	 * [�v�Z] �����Z
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �����̎Q��
	 */
	CBitSet& operator+=(const CBitSet& t)
	{
		CBitSet bs = *this + t;
		return *this = bs;
	}

	/**
	 * [�v�Z] AND�v�Z
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �����̎Q��
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
	 * [�v�Z] OR�v�Z
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �����̎Q��
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
	 * [�v�Z] XOR�v�Z
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �����̎Q��
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
	 * [�v�Z] �E�V�t�g�v�Z
	 *	@param dwPos �V�t�g��
	 *	@return �����̎Q��
	 *	@todo �������ł��B
	 */
	CBitSet& operator>>=(DWORD dwPos)
	{
		ASSERT0(false, "CBitSet::operator>>=()", "������");
		return *this;
	}

	/**
	 * [�v�Z] ���V�t�g�v�Z
	 *	@param dwPos �V�t�g��
	 *	@return �����̎Q��
	 *	@todo �������ł��B
	 */
	CBitSet& operator<<=(DWORD dwPos)
	{
		ASSERT0(false, "CBitSet::operator<<=()", "������");
		return *this;
	}

	/**
	 * [�v�Z] �����Z���ʎ擾
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �v�Z����
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
	 * [�擾] AND�v�Z���ʎ擾
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �v�Z����
	 */
	CBitSet operator&(const CBitSet& t) const
	{
		CBitSet bs(*this);
		return bs &= t;
	}

	/**
	 * [�擾] OR�v�Z���ʎ擾
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �v�Z����
	 */
	CBitSet operator|(const CBitSet& t) const
	{
		CBitSet bs(*this);
		return bs |= t;
	}

	/**
	 * [�擾] XOR�v�Z���ʎ擾
	 *	@param t �v�Z����BBIT�T�C�Y���قȂ��Ă��Ă��\���܂���B
	 *	@return �v�Z����
	 */
	CBitSet operator^(const CBitSet& t) const
	{
		CBitSet bs(*this);
		return bs ^= t;
	}

	/**
	 * [�擾] �E�V�t�g�v�Z���ʎ擾
	 *	@param dwPos �V�t�g��
	 *	@return �v�Z����
	 */
	CBitSet operator<<(DWORD dwPos) const
	{
		CBitSet bs(*this);
		return bs <<= dwPos;
	}

	/**
	 * [�擾] ���V�t�g�v�Z���ʎ擾
	 *	@param dwPos �V�t�g��
	 *	@return �v�Z����
	 */
	CBitSet operator>>(DWORD dwPos) const
	{
		CBitSet bs(*this);
		return bs >>= dwPos;
	}

	/**
	 * [�擾] BIT���]���擾
	 *	@return BIT�����]���� CBitSet
	 */
	CBitSet operator~(void) const
	{
		CBitSet bs(*this);
		return bs.Flip();
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
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
 * [�ǉ�] �V���A���C�Y
 *	@param[out] _sr �V���A���C�U�[
 *	@param[in] t �f�[�^
 *	@return ISerializer �̎Q��
 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
 */
inline ISerializer& operator<<(ISerializer& _sr, const CBitSet& t)
{
	_sr << t.ToString();
	return _sr;
}

/**@ingroup VALUE
 * [�擾] �f�V���A���C�Y
 *	@param[in] ds �f�V���A���C�U�[
 *	@param[out] _t �f�[�^
 *	@return IDeserializer �̎Q��
 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, CBitSet& _t)
{
	CStr s;
	ds >> s;
	_t = CBitSet(s);
	return ds;
}



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
