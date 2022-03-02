#pragma once
/**
 *@file
 * �X�^�b�N�^���Ǘ��֌W�̃w�b�_
 *
 *		����LIFO(Last In First Out)�ŊǗ�����N���X�ł��B 
 *		Add�n���\�b�h�Œǉ����ATake�n���\�b�h�ŐV�����ق�����������o���܂��B
 *		�܂��AGet,Set ���\�b�h�ɂ��A�C���f�b�N�X�ɂ��A�N�Z�X���\�ł��B
 *		���̎��C���f�b�N�X�́A�Â����Ɋ���U���Ă��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �X�^�b�N�^���Ǘ��e���v���[�g
 *
 *		����LIFO(Last In First Out)�ŊǗ�����N���X�ł��B 
 *		Add�n���\�b�h�Œǉ����ATake�n���\�b�h�ŐV�����ق�����������o���܂��B
 *		�܂��AGet,Set ���\�b�h�ɂ��A�C���f�b�N�X�ɂ��A�N�Z�X���\�ł��B
 *		���̎��C���f�b�N�X�́A�Â����Ɋ���U���Ă��܂��B
 *
 *	@note 	�����^�̃C���X�^���X�͑�����\�ł��B���̎��A���ӂ̃C���X
 *			�^���X�́A����j�����A�E�ӂ̃C���X�^���X�̏������L����悤��
 *			�Ȃ�܂��i���̂��ߑ����Ƃ�<b>����</b>�ł��j�B
 *			���̌�A�����ꂩ�̃C���X�^���X�𑀍삷��ƁA
 *			���𕡐������̃C���X�^���X�ɉe�����Ȃ��悤�ɂ��Ă��܂��B
 *			�i�������쐬���鎞�A���Ԃ�������\�����L��܂��j
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	ISequenceCollectionT ���������Ă��܂��B
 *	@see	IQueueT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbStack.h
 * 
 *	@date 06/05/01 �V�K�쐬
 *	@date 06/05/15 Clone��p�~�B
 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/08 CVectorT ��e�N���X�ɕύX�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP>
class CStackT : public CVectorT<TYP> , public IQueueT<TYP>
{
	DEFSUPER(CVectorT<TYP>);
public:

	/**
	 * �R���X�g���N�^ 
	 *	@note �w��̃������m�ێ��̒ǉ��T�C�Y��ݒ肵�܂��B
	 *	@note �f�[�^�͕ێ����܂���
	 *	@param size �������m�ێ��̒ǉ��T�C�Y�BSetIncrementSize() �Ɠ����ł��B
	 */
	explicit CStackT(size_t size = 0) : _super(size) 
	{
	}

	/** �f�X�g���N�^ */
	virtual ~CStackT(void) 
	{
	}

	/**
	 * [�ǉ�] �����v�f�ǉ�.
	 *		�Ō�ɒǉ����܂��B
	 *	@param size �ǉ�����f�[�^�̐��B
	 *	@param P �ǉ�����f�[�^��̃|�C���^�B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		return _super::AddElements(size, P);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �v�f�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t)
	{
		return _super::Add(t);
	}

	/**
	 * [�擾] �擪�v�f���o��.
	 *		�L���[����폜���܂�
	 *	@return �v�f���e
	 *	@throw CEmptyException ��ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP Take(void)
	{
		EXCLUSIVE(this);
		size_t l = GetSize();
		if ( l == 0 )
		{
			throw CEmptyException();
		}
		TYP t = _super::Get(l - 1);
		Remove(l - 1);
		return t;
	}

	/**
	 * [�擾] �����v�f���o��
	 *	@note	�w��o�b�t�@�ɃR�s�[���A�L���[����폜���܂��B
	 *	@param[in]	size	���o�����B
	 *	@param[out]	P		���o���o�b�t�@�B NULL�Ȃ�擾���܂���B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		���o�������B
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		EXCLUSIVE(this);
		size_t l = GetSize();
		if ( l == 0 || size > l )
		{
			return INVALID_SIZE; 
		}
		l--;
		if ( P != NULL )
		{
			for ( INDEX i = 0; i < size; i++ )
			{
				*P++ = _super::At(l - i);
			}
		}
		loop ( i, size )
		{
			_super::Remove(l - i);
		}
		return size;
	}

private:
	friend class CStackTest;///< �t�����h�N���X�錾
};



}; // TNB
