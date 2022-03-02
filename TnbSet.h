#pragma once
/**
 *@file
 * �V���O���Z�b�g���Ǘ��֌W�̃w�b�_
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * �V���O���Z�b�g���Ǘ��e���v���[�g
 *
 *		�����l�͂Q�i�[�ł��܂���B���������Ƀ\�[�g����Ċi�[����܂��B
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *	@note ������ Find() ��� GetEqualIndex() ���g�����ق��������ł��B
 *
 *	@param TYP �ێ�����^�ioperator<() �A�y�уR�s�[�\�̌^�Ɍ���j
 *		\code
 *		// ��jTFoo ���i�[�������ꍇ
 *		struct TFoo
 *		{
 *			bool operator<(const TFoo& v) const
 *			{
 *				return a < v.a;
 *			}
 *			CStr a;
 *		};
 *		// ��jint ��傫�����Ɋi�[�������ꍇ
 *		struct TInteger
 *		{
 *			bool operator<(const TInteger& other) const
 *			{
 *				return value > other.value;
 *			}
 *			int value;
 *		};
 *		\endcode
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ISequenceCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSet.h
 *
 *	@date 11/10/03 �V�K�쐬
 *	@date 12/07/29 InsertElements() �V�K�B
 *	@date 12/08/15 InsertElements() �̎d�l�ύX�B operator[] �ǉ��B
 *	@date 14/11/10 Insert���A�l�������ł��������悤�ɕύX�B
 */
template<typename TYP>
class CSingleSetT : public IConstCollectionT<TYP>, public ISequenceCollectionT<TYP>
{
public:

	/**
	 * �R���X�g���N�^
	 *	@note �w��̃������m�ێ��̒ǉ��T�C�Y��ݒ肵�܂��B
	 *	@note �f�[�^�͕ێ����܂���
	 *	@param size �������m�ێ��̒ǉ��T�C�Y�B SetIncrementSize() �Ɠ����ł��B
	 */
	explicit CSingleSetT(size_t size = 0) : m_data(size)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@note �R�s�[���ƃo�b�t�@�����L���܂��B
	 *	@param other �R�s�[��
	 */
	CSingleSetT(const CSingleSetT<TYP>& other)
	{
		m_data = other.m_data;
	}
	
	/**
	 * [���] ���
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@���J�����A	
	 *		������̏������L����悤�ɂȂ�܂��B
	 *	@param other ����� 
	 *	@return �����̎Q��
	 */
	CSingleSetT& operator=(const CSingleSetT& other)
	{
		m_data = other.m_data;
		return *this;
	}

	/** 
	 * [�r��] ���b�N
	 *	@note	Lock/Unlock�͏��̔r���̂��߂Ɏg�p���܂��B�X���b�h�Z�[�u��
	 *			���邽�߂ɁA��A�̏�񑀍�O��Lock������I�����Unlock����K�v��
	 *			����܂��i�P���̑��쎞�͑��v�ł��j�B
	 *	@param dwTime �_�~�[�ł��B�ȗ����Ă��������B
	 *	@return ���true�B
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		return m_data.Lock(dwTime);
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		m_data.Unlock();
	}

	/**
	 * [�擾] �f�[�^�A�h���X�擾.
	 *	@note	���̃A�h���X�̓��e�͏��������Ă͂����܂���B
	 *	@note	�擾�����A�h���X�� delete �ŊJ�����Ă͂����܂���B
	 *	@note	�擾�����A�h���X�͖{�N���X�̌v�Z���A���삷��Ɩ����ɂȂ�܂��B
	 *	@return �A�h���X�B
	 */
	virtual const TYP* ReferBuffer(void) const
	{
		return m_data.ReferBuffer();
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_data.GetSize();
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̎Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_data[index];
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& operator[](INDEX index) const
	{
		return m_data.At(index);
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval false	�G���[�B
	 *	@retval true	�����B
	 */
	bool Remove(INDEX index)
	{
		return m_data.Remove(index);
	}

	/**
	 * [�폜] ��
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@����ɂ��܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool RemoveAll(void)
	{
		return m_data.RemoveAll();
	}

	/**
	 * [�폜] �w��v�f�̍폜.
	 *	@note �w��̗v�f����A�폜���܂��B
	 *	@param	t �v�f
	 *	@retval true �폜
	 *	@retval false ������
	 */
	bool Delete(const TYP& t)
	{
		INDEX i = GetEqualIndex(t);
		if ( i != INVALID_INDEX )
		{
			return m_data.Remove(i);
		}
		return false;
	}

	/**
	 * [�m�F] �l���邩�H
	 *	@param t �v�f
	 *	@retval false	������
	 *	@retval true	����
	 */
	bool Has(const TYP& t) const
	{
		return GetEqualIndex(t) != INVALID_INDEX;
	}

	/**
	 * [����] �w��v�f�̌���.
	 *	@note �J�n�C���f�b�N�X���w�肵�����ꍇ��A��r���@��Ǝ��Ɏw�肵�����ꍇ�A Find() ���g���܂��B
	 *	@param	t �v�f
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		���������C���f�b�N�X
	 */
	INDEX GetEqualIndex(const TYP& t) const
	{
		if ( IsEmpty() )
		{
			return INVALID_INDEX;
		}
		size_t sz = m_data.GetSize();
		if ( t < m_data[0] || m_data[sz - 1] < t )
		{
			return INVALID_INDEX;
		}
		INDEX f = 0;
		INDEX z = sz;
		while ( z > 0 )
		{
			INDEX x = z / 2;
			if ( m_data[f + x] < t )
			{
				// �傫��
				f += (x + 1);
				z = (z - 1) / 2;
			}
			else if ( t < m_data[f + x] )
			{
				// ������
				z /= 2;
			}
			else
			{
				return f + x;
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [����] �w��v�f�}��.
	 *	@param	t �v�f
	 *	@retval INVALID_INDEX	���s
	 *	@retval ����ȊO		�����B�l�͑}�������C���f�b�N�X
	 */
	INDEX Insert(const TYP& t)
	{
		INDEX r = m_Insert(t);
		#ifdef _DEBUG
			if ( r != INVALID_INDEX )
			{
				if ( r >= 1 )
				{
					ASSERT( ! (t < At(r - 1)) );
				}
				if ( r + 1 < GetSize() )
				{
					ASSERT( ! (At(r + 1) < t) );
				}
			}
		#endif
		return r;
	}

	/**
	 * [����] �w��v�f�}��.
	 *	@param	t �v�f
	 *	@retval INVALID_INDEX	���s
	 *	@retval ����ȊO		�����B�l�͑}�������C���f�b�N�X
	 */
	INDEX InsertEx(const TYP& t)
	{
		EXCLUSIVE( this );
		return Insert(t);
	}

	/**
	 * [����] �w��v�f�}��.
	 *	@param	t �v�f
	 *	@retval false	���s
	 *	@retval true	����
	 */
	bool Insert(const IConstCollectionT<TYP>& t)
	{
		EXCLUSIVE( this );
		loop ( i, t.GetSize() )
		{
			if ( Insert(t.At(i)) == INVALID_INDEX )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [����] �w��v�f�}��.
	 *	@param size �f�[�^���B
	 *	@param P �}��������e���w��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@return ����ȊO		�폜�������B
	 */
	size_t InsertElements(size_t size, const TYP* P)
	{
		EXCLUSIVE( this );
		size_t cnt = 0;
		loop ( i, size )
		{
			if ( ! Has(*P) )
			{
				if ( Insert(*P) == INVALID_INDEX )
				{
					return INVALID_SIZE;
				}
				cnt++;
			}
			P++;
		}
		return cnt;
	}

protected:
	/**
	 * [�ʒm] �����l�����ʒm.
	 *	@param index �����l�̃C���f�b�N�X
	 *	@return Insert() �̖߂�l
	 */
	virtual INDEX OnFoundEqual(INDEX index)
	{
		return index;
	}

	CVectorT<TYP> m_data;	///< �f�[�^

private:
	/**
	 * [����] �w��v�f�}��.
	 *	@param	t �v�f
	 *	@retval INVALID_INDEX	���s
	 *	@retval ����ȊO		�����B�l�͑}�������C���f�b�N�X
	 */
	INDEX m_Insert(const TYP& t)
	{
		if ( IsEmpty() )
		{
			return m_data.Add(t);
		}
		size_t sz = m_data.GetSize();
		if ( t < m_data[0] )
		{
			m_data.Insert(0, t);
			return 0;
		}
		if ( m_data[sz - 1] < t )
		{
			return m_data.Add(t);
		}
		INDEX f = 0;
		INDEX z = sz;
		while ( z > 0 )
		{
			INDEX x = z / 2;
			if ( m_data[f + x] < t )
			{
				// �傫��
				f += (x + 1);
				z = (z - 1) / 2;
			}
			else if ( t < m_data[f + x] )
			{
				// ������
				z /= 2;
			}
			else
			{
				// �����l����
				m_data[f + x] = t;
				return OnFoundEqual(f + x);
			}
		}
		if ( m_data[f] < t )
		{
			f++;
		}
		m_data.Insert(f, t);
		return f;
	}
	friend class CSetTest;
};



#if 0

/**@ingroup COLLECT
 * �}���`�Z�b�g���Ǘ��e���v���[�g
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *	@param TYP �ێ�����^�ioperator<() �A�y�уR�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ISequenceCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSet.h
 *
 *	@date 11/10/03 �V�K�쐬
 */
template<typename TYP>
class CMultiSetT : public CSingleSetT<TYP>
{
public:

	/**
	 * [����] �w��v�f�}��.
	 *	@param	t �v�f
	 *	@retval INVALID_INDEX	���s
	 *	@retval ����ȊO		�����B�l�͑}�������C���f�b�N�X
	 */
	INDEX Insert(const TYP& v)
	{
		loop ( i, m_data.GetSize() )
		{
			if ( v < m_data[i] || ! (m_data[i] < v) )
			{
				//���̗v�f��菬����������
				m_data.Insert(i, v);
				return i;
			}
		}
		m_data.Add(v);
		return m_data.GetSize() - 1;
	}

	/**
	 * [�폜] �w��v�f�̍폜.
	 *	@note �w��̗v�f�����ׂāA�폜���܂��B
	 *	@param	t �v�f
	 *	@retval true �폜
	 *	@retval false ������
	 */
	bool Delete(const TYP& t)
	{
		TRange r = GetEqualRange(t);
		if ( r.lower != INVALID_INDEX )
		{
			m_data.RemoveElement(r.lower, (r.upper - r.lower) + 1);
			return true;
		}
		return false;
	}

	struct TRange
	{
		INDEX lower;
		INDEX upper;
	};

	/**
	 * [����] �w��v�f�̌���.
	 *	@note �����l�̂��̊܂܂�Ă���C���f�b�N�X��Ԃ��܂��B
	 *	@param	t �v�f
	 *	@return �͈́B �l�� INVALID_INDEX �̏ꍇ�A������
	 */
	TRange GetEqualRange(const TYP& v) const
	{
		TRange r = { INVALID_INDEX, INVALID_INDEX };
		size_t sz = m_data.GetSize();
		for ( INDEX i = 0; i < sz; i++ )
		{
			if ( m_IsEqual(v, m_data[i]) )
			{
				//����
				r.lower = i;
				i++;
				for ( ; i < sz; i++ )
				{
					if ( v < m_data[i] || m_data[i] < v )
					{
						r.upper = i - 1;
						break;
					}
				}
				break;
			}
		}
		return r;
	}
	/**
	 * [��r] �����H
	 *	@param t1 �l�P
	 *	@param t2 �l�Q
	 *	@retval true ����
	 *	@retval false �Ⴄ
	 */
	bool m_IsEqual(const TYP& t1, const TYP& t2) const
	{
		return ! (t1 < t2) && ! (t2 < t1);
	}

};

#endif



}; //TNB
