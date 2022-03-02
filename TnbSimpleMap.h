#pragma once
/**
 *@file
 * �ȈՔz��^���Ǘ��֌W�̃w�b�_
 *
 *		��ӂȃL�[�ɒl�����蓖�ĂăR���N�V�������邱�Ƃ��ł��܂��B
 *		�L�[�Œl�����o������폜���邱�Ƃ��ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * �}�b�v�^���Ǘ��e���v���[�g
 *
 *		��ӂȃL�[�ɒl�����蓖�ĂăR���N�V�������邱�Ƃ��ł��܂��B
 *		�L�[�Œl�����o������폜���邱�Ƃ��ł��܂��B
 *	
 *	@see CMapT 
 *
 *	@param	KEY	�L�[�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\�A��r�@�\( IComparableT ���A�����Z)���K�{�B
 *	@param	VAL	�l�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\���K�{�B
 *	@param	INK	�L�[�Ƃ���^�̓��͌^�B�ȗ��\�B
 *
 *	@par�K�v�t�@�C��
 *			TnbSimpleMap.h
 *
 *	@date 09/04/22 �V�K�쐬.
 *	@date 19/07/07 �T�C�Y�w��̃R���X�g���N�^�ǉ�
 */
template<typename KEY, typename VAL, typename INK=KEY>
class CSimpleMapT
{
public:

	/// �p�����[�^�^
	struct TParam
	{
		KEY		key;	///< �L�[
		VAL		val;	///< �l
		/**
		 * �R���X�g���N�^
		 *	@param k �L�[
		 *	@param v �l
		 */
		TParam(INK k = KEY(), VAL v = VAL()) : key(k), val(v)
		{}
	};

	/// �R���X�g���N�^
	explicit CSimpleMapT(void) : m_isValidLast(false)
	{
	}

	/// �R���X�g���N�^
	/// @param bufferSize �o�b�t�@�T�C�Y
	explicit CSimpleMapT(size_t bufferSize) : m_isValidLast(false), m_map(bufferSize)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[���B
	 */
	CSimpleMapT(const CSimpleMapT& other)
	{
		m_map = other.m_map;
	}

	/**
	 * [����] �R�s�[�I�y���[�^
	 *	@param other �R�s�[���B
	 *	@return �����̎Q��
	 */
	CSimpleMapT& operator=(const CSimpleMapT& other)
	{
		m_map = other.m_map;
		return *this;
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	size_t GetSize(void) const
	{
		return m_map.GetSize();
	}

	/**
	 * [�擾] �L�[�ɑ΂���l�̎Q��
	 *	@note �L�[���Ȃ��ꍇ�A�V�K�ɒǉ����܂��B
	 *	@param key �L�[
	 *	@return VAL�̎Q�Ɓi���삷��ƁAMAP���̏����ω����܂��j�B
	 */
	VAL& operator[](INK key)
	{
		INDEX i = Find(key);
		if ( i != INVALID_INDEX )
		{
			return m_map[i].val;
		}
		i = m_map.Add(TParam(key, VAL()));
		m_lastKey = key;
		m_lastIndex = i;
		m_isValidLast = true;
		return m_map[i].val;
	}

	/**
	 * [�擾] �L�[�ɑ΂���l�̎Q��
	 *	@note �L�[���Ȃ��ꍇ�A�V�K�ɒǉ����܂��B
	 *	@param key �L�[
	 *	@return VAL�̎Q�Ɓi���삷��ƁAMAP���̏����ω����܂��j�B
	 *	@throw CEmptyException �L�[�����݂��Ȃ����Ƃ��������߂ɃX���[����܂��B
	 */
	const VAL& operator[](INK key) const
	{
		INDEX i = Find(key);
		if ( i != INVALID_INDEX )
		{
			return m_map[i].val;
		}
		throw CEmptyException();
	}

	/**
	 * [�폜] �L�[�ƒl���폜
	 *	@param key �L�[
	 *	@retval true ����
	 *	@retval false ���s�i�L�[���Ȃ��j
	 */
	bool RemoveKey(INK key)
	{
		INDEX i = Find(key);
		if ( i != INVALID_INDEX )
		{
			m_map.Remove(i);
			m_isValidLast = false;
			return true;
		}
		return false;
	}

	/**
	 * [�Q��] �p�����[�^�Q��.
	 *	@param index �C���f�b�N�X
	 *	@return �p�����[�^�Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TParam& Ref(INDEX index) const
	{
		return m_map[index];
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@param index �C���f�b�N�X
	 *	@retval false	�G���[�B
	 *	@retval true	�����B
	 */
	bool Remove(INDEX index)
	{
		m_isValidLast = false;
		return m_map.Remove(index);
	}

	/**
	 * [����] �L�[����.
	 *	@param key �L�[
	 *	@retval INVALID_INDEX ���݂��Ȃ�
	 *	@retval ��L�ȊO �����B���l�̓C���f�b�N�X�B
	 */
	INDEX Find(INK key) const
	{
		if ( m_isValidLast && m_lastKey == key )
		{
			ASSERT( m_map[m_lastIndex].key == m_lastKey );
			return m_lastIndex;
		}
		loop ( i, m_map.GetSize() )
		{
			if ( m_map[i].key == key )
			{
				m_lastKey = key;
				m_lastIndex = i;
				m_isValidLast = true;
				return i;
			}
		}
		return INVALID_INDEX;
	}
	/**
	 * [�m�F] �L�[�L��
	 *	@param key �L�[
	 *	@retval true ���݂���
	 *	@retval false ���݂��Ȃ�
	 */
	bool HasKey(INK key) const
	{
		return Find(key) != INVALID_INDEX;
	}

	/**
	 * [�폜] ��
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@����ɂ��܂��B
	 */
	void RemoveAll(void)
	{
		m_isValidLast = false;
		m_map.RemoveAll();
	}

private:
	CSimpleVectorT<TParam>	m_map;	///< �}�b�v
	mutable	bool	m_isValidLast;	///< ���X�g���L���H
	mutable	INK		m_lastKey;		///< ���X�g�̃L�[
	mutable INDEX	m_lastIndex;	///< ���X�g�̃C���f�b�N�X
};



}; // TNB

