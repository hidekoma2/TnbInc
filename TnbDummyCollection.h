#pragma once
/**
 *@file
 * �_�~�[�R���N�V�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �_�~�[�R���N�V�����e���v���[�g
 *
 *		�T�C�Y�����Ǘ����Ȃ��R���N�V�����ł��B
 *		������Ă��擾����Ƃ��ׂăR���X�g���N�^�Ŏw�肵�� TYP �̒l�ɂȂ��Ă��܂��܂��B
 *
 *		ICollectionT �C���^�[�t�F�[�X��~���� API �ɑ΂��āA�T�C�Y�̂ݗ~�������Ȃǂ�
 *		�g�p�ł��܂��B
 *	
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDummyCollection.h
 *
 *	@date 06/09/01 �V�K
 *	@date 06/09/06 �T�C�Y��Index�͐������`�F�b�N����l�ɂ����B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 10/04/09 �R���X�g���N�^�ŁA�l���w��ł���悤�ɂ����B
 */
template<typename TYP>
class CDummyCollectionT : public ICollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param typ �����B��̒l.
	 */
	CDummyCollectionT(const TYP& typ = TYP()) : _super(), m_data(typ), m_size(0)
	{
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime �_�~�[�B�ȗ����Ă��������B
	 *	@return ���true�B
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const { return true; }

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const {}

	/**
	 * [�擾] �v�f���擾.
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return �ꎞ�I�ȎQ�ƁB���e�� TYP() �ł��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
		return m_data;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̎Q��
	 *	@throw CNotSupportException �T�|�[�g���Ă��Ȃ����Ƃ��������߂ɃX���[����܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP& Ref(INDEX index)
	{
		if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
		m_temp = m_data;
		return m_temp;
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@note �C���f�b�N�X�`�F�b�N�݂̂��܂��B t �͋L�����܂���B
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@return ��� true 
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		return IsInRange(index);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@note �T�C�Y���{�P���܂��B t �͋L�����܂���B
	 *	@param t �v�f�B
	 *	@return �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t)
	{
		return m_size++;
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@note �T�C�Y���|�P���܂��B
	 *	@param index �C���f�b�N�X
	 *	@retval true	�폜����
	 *	@retval false	���s
	 */
	virtual bool Remove(INDEX index)
	{
		if ( ! IsInRange(index) )
		{
			return false; 
		}
		m_size--;
		return true;
	}

	/**
	 * [�폜] �S�v�f�폜 .
	 *	@note �T�C�Y���O�ɂ��܂��B
	 *	@return ��� true 
	 */
	virtual bool RemoveAll(void)
	{
		m_size = 0;
		return true;
	}

private:
	size_t			m_size;			///< �Ǘ��T�C�Y
	TYP				m_data;			///< �f�[�^
	mutable TYP		m_temp;			///< �Q�Ɨp
};



}; // TNB
