#pragma once
/**
 *@file
 * �|�C���^�z��Ǘ��֌W�̃w�b�_
 *
 *		new�Ŋm�ۂ�����^�̃|�C���^�z����Ǘ����܂��B
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂��B
 *		�R�s�[��폜�ŕs�v�ɂȂ����|�C���^�͎����I�� delete ���܂��B
 *		�������Ă��A�|�C���^��̎��͕̂������܂���B�ł��̂ŁA���̂𑀍삷��ƁA
 *		�R�s�[���ɂ��e�����o�܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �|�C���^�z��Ǘ��e���v���[�g
 *
 *		new�Ŋm�ۂ�����^�̃|�C���^�z����Ǘ����܂��B
 * 
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂��B
 *		�R�s�[��폜�ŕs�v�ɂȂ����|�C���^�͎����I�� delete ���܂��B
 *		�������Ă��A�|�C���^��̎��͕̂������܂���B�ł��̂ŁA���̂𑀍삷��ƁA
 *		�R�s�[���ɂ��e�����o�܂��B
 *
 *	@attention	new[] �Ŋm�ۂ����|�C���^�͊Ǘ��ł��܂���B
 *
 *	@param 	TYP �ێ�����^�i�R�s�[�ł��Ȃ��Ă��悵�j
 *
 *	@see IConstCollectionT ���������Ă��܂��B�|�C���^�ی�̂���ICollectionT �͎������Ă��܂���B
 *
 *	@note IConstCollectionT �� TYP �� TYP* �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPointerVector.h
 *
 *	@date 06/04/01 �V�K�쐬
 *	@date 06/04/21 �R���X�g���N�g���ɒǉ��T�C�Y�w��\�ɂ����B
 *	@date 06/04/22 �|�C���^�ی�̂��߁A�p����������ɐ؂�ւ����B
 *	@date 06/05/17 CVectorT�̎d�l�ύX�̂��߁A�{�N���X���C���B
 *	@date 06/05/19 �uVector��PH�v����uPH��Vector�v�ɕύX�B
 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/30 �V���A���C�Y�̓T�|�[�g�o���Ȃ��悤�ɂ����B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 09/02/12 Insert() �ǉ��B
 *	@date 11/03/02 RemoveElements() �ǉ��B
 */
template<typename TYP>
class CPointerVectorT : public IConstCollectionT<TYP*>
{
	DEFSUPER(IConstCollectionT<TYP*>);
	CVectorT< CPointerHandleT<TYP> > m_vhPointer;	///< �|�C���^�̔z��
public:

	/**
	 * �R���X�g���N�^
	 *	@note �w��̃������m�ێ��̒ǉ��T�C�Y��ݒ肵�܂��B
	 *	@note �f�[�^�͕ێ����܂���
	 *	@param size �������m�ێ��̒ǉ��T�C�Y
	 */
	explicit CPointerVectorT(size_t size = 0) : _super()
	{
		m_vhPointer.SetIncrementSize(size);
	}
	
	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param v �R�s�[��
	 */
	CPointerVectorT(const CPointerVectorT<TYP>& v) : _super(), m_vhPointer(v.m_vhPointer)
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CPointerVectorT& operator=(const CPointerVectorT<TYP>& other) 
	{
		m_vhPointer = other.m_vhPointer;
		return *this;
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
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return �Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP* const& At(INDEX index) const
	{
		return *(m_vhPointer.At(index).ReferP());
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CPointerVectorT(void) 
	{
	}

	/**
	 * [�폜] �v�f�폜
	 *	@param index �C���f�b�N�X
	 *	@retval true	�폜����
	 *	@retval false	���s
	 */
	bool Remove(INDEX index)
	{
		return m_vhPointer.Remove(index);
	}

	/**
	 * [�폜] ��
	 */
	void RemoveAll(void)
	{
		m_vhPointer.RemoveAll();
	}

	/**
	 * [�폜] �v�f�폜.
	 *	@param index �C���f�b�N�X
	 *	@param size �폜���B�ȗ�����ƂO�B�O���w�肷��Ƃ���ȍ~�S���폜���܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@return ����ȊO		�폜�������B
	 */
	size_t RemoveElements(INDEX index, size_t size = 0)
	{
		return m_vhPointer.RemoveElements(index, size);
	}

	/**
	 * [����] �T�C�Y�w��
	 *	@note �傫�����邱�Ƃ����������邱�Ƃ��o���܂��B
	 *	@note �傫���������A���̃f�[�^�� NULL �Ŗ��߂��܂��B
	 *	@note �������������A�s�K�v�ɂ������f�[�^�� delete ����܂��B
	 *	@param size �T�C�Y
	 */
	void SetSize(size_t size)
	{
		m_vhPointer.SetSize(size);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�
	 *	@param P �f�[�^�̃|�C���^�Bnew�Ŋm�ۂ������̂Ɍ���܂��B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX Add(TYP* P)
	{
		return m_vhPointer.Add(P); 
	}

	/**
	 * [�}��] �v�f��}��
	 *	@param index �ǉ�����INDEX�B
	 *	@param P �f�[�^�̃|�C���^�Bnew�Ŋm�ۂ������̂Ɍ���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Insert(INDEX index, TYP* P)
	{
		return m_vhPointer.Insert(index, P); 
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�
	 *	@param index �C���f�b�N�X
	 *	@param P �f�[�^�̃|�C���^�Bnew�Ŋm�ۂ������̂Ɍ���܂��B
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s(�C���f�b�N�X���͈͊O�A�Ȃ�)
	 */
	bool Set(INDEX index, TYP* P)
	{
		return m_vhPointer.Set(index, P); 
	}
	
	/**
	 * [�擾] �v�f�Q�Ǝ擾
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP* operator[](INDEX index) const 
	{
		return Get(index); 
	}
	
	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize( void ) const
	{ 
		return m_vhPointer.GetSize(); 
	}

	/**
	 * [����] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B 
	 *	@throw CNotSupportException �T�|�[�g���Ă��Ȃ����Ƃ��������߂ɃX���[����܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		throw CNotSupportException();
	}
};



}; // TNB
