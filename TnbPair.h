#pragma once
/**
 *@file
 * �y�A�^�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSerializer.h"
#include "TnbComparable.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * �y�A�^�e���v���[�g�N���X
 *
 *		�L�[�ƒl��ێ����邾���̌^�ł��B
 *		���CMapT �̔z��ێ�����^�Ƃ��Ďg�p���Ă��܂��B
 *	
 *	@param	FIRST	��ڂ̌^�B�N���X�̏ꍇ�R�s�[�@�\�A��r�@�\( IComparableT ���A�����Z)���K�{�B
 *	@param	SECOND	��ڂ̌^�B�N���X�̏ꍇ�R�s�[�@�\���K�{�B
 *
 *	@par�K�v�t�@�C��
 *			TnbPair.h
 *
 *	@date 06/04/21 �V�K
 *	@date 06/05/19 IComparableT �A�����B
 *	@date 06/08/30 ISerializable �A�����B
 *	@date 07/01/25 �R���X�g���N�^����ǉ��B�ϐ����� KEY,VAL����1st 2nd �ɕύX�B
 */
template<typename FIRST, typename SECOND>
class CPairT : public IComparableT< TNB::CPairT<FIRST, SECOND> >, public ISerializable
{
public:

	FIRST		first;		///< ��ڂ̌^�̒l
	SECOND		second;		///< ��ڂ̌^�̒l

	/// �R���X�g���N�^
	CPairT(void) : first(FIRST()), second(SECOND())
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param v1 FIRST �̒l
	 *	@param v2 SECOND �̒l
	 */
	CPairT(const FIRST& v1, const SECOND& v2) : first(v1), second(v2)
	{
	}

	/** 
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CPairT(const CPairT& other) : first(other.first), second(other.second) 
	{
	}

	/** 
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CPairT& operator=(const CPairT& other)
	{
		first	= other.first;
		second	= other.second; 
		return *this;
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CPairT& t) const
	{
		INT_PTR r = IComparatorT<FIRST>::GetDefault().CompareTo(first, t.first);
		if ( r != 0 )
		{
			return r;
		}
		return IComparatorT<SECOND>::GetDefault().CompareTo(second, t.second);
	}

	/**
	 * [�ʒm] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << first << second;
	}

	/**
	 * [�ʒm] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> first >> second;
	}
};



}; // TNB
