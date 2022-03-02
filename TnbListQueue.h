#pragma once
/**
 *@file
 * �L���[�^���Ǘ��֌W�̃w�b�_
 *
 *		����FIFO(First In First Out)�ŊǗ�����N���X�ł��B 
 *		Add�n���\�b�h�Œǉ����ATake�n���\�b�h�ŌÂ��̂ق�����������o���܂��B
 *		�܂��AGet,Set ���\�b�h�ɂ��A�C���f�b�N�X�ɂ��A�N�Z�X���\�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbQueue.h"
#include "TnbList.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * ���X�g���L���[�^���Ǘ��e���v���[�g
 *	
 *	@note �L���[�̑傫���͉ςł��B�������̋����������܂��B
 *
 *	@note	�e�v�f�Ƀp�����[�^�l�i32bit�̔C�ӂ̒l�j����ɂ��邱�Ƃ��o���܂��B
 *			���̒l�́AAppend�n �� Copy�n ���g���Ēǉ��A��������ƁA0�ɂȂ��Ă��܂��܂��B
 *			ListQueue ���m�̘A���A�����́A�I�y���[�^���g���Ă��������B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	IQueueT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbListQueue.h
 * 
 *	@date 06/04/25	�V�K�쐬
 *	@date 06/05/15 Clone��p�~�B
 *	@date 06/05/29 �e�v�f�Ƀp�����[�^���������悤�ɂ����B
 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/08 CListT ��e�N���X�ɕύX�B
 */
template<typename TYP>
class CListQueueT : public CListT<TYP> , public IQueueT<TYP>
{
	DEFSUPER(CListT<TYP>);
public:

	/** �R���X�g���N�^ */
	CListQueueT(void) : _super()
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^ 
	 *	@param other �R�s�[��
	 */
	CListQueueT(const CListQueueT& other) : _super(other)
	{
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CListQueueT(void) {}

	/**
	 * [���] �R�s�[�I�y���[�^ 
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CListQueueT& operator=(const CListQueueT& other)
	{
		_super::operator=(other);
		return *this;
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^
	 *	@note ���C���X�^���X�ŊǗ����Ă���f�[�^�Ɏw��̃f�[�^��
	 *		 �ǉ����܂��B
	 *	@param queue �ǉ�����ListQueue
	 *	@return �����̎Q��
	 */
	CListQueueT& operator+=(const CListQueueT<TYP>& queue)
	{
		_super::operator+=(queue);
		return *this;
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
	 * [�ǉ�] �v�f�ǉ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *		�v�f����L���[�̍Ō�ɒǉ�����B
	 *	@param t �ǉ�����v�f�B�ȗ��\�B
	 *	@param dwParam �p�����[�^�l�B
	 *	@return �ǉ�����INDEX�B
	 */
	INDEX Add(const TYP& t, DWORD dwParam)
	{
		return _super::Add(t, dwParam);
	}

	/**
	 * [�ǉ�] �v�f�ǉ�.
	 *		�v�f����L���[�̍Ō�ɒǉ�����B
	 *	@param t �ǉ�����v�f�B�ȗ��\�B
	 *	@param dwParam �p�����[�^�l�B
	 *	@return �ǉ�����INDEX�B
	 */
	INDEX AddEx(const TYP& t, DWORD dwParam)
	{
		return _super::AddEx(t, dwParam);
	}

	/**
	 * [�擾] �����v�f���o��
	 *	@note	�w��o�b�t�@�ɃR�s�[���A�L���[����폜���܂��B
	 *	@param[in]	size	���o�����B
	 *	@param[out]	P		���o���o�b�t�@�B NULL�Ȃ�擾���܂���B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		EXCLUSIVE(this);
		if ( size > GetSize() )
		{
			return INVALID_SIZE;
		}
		if ( P != NULL )
		{
			loop ( i, size )
			{
				*P++ = _super::At(i);
			}
		}
		loop ( i, size )
		{
			_super::Remove(0);
		}
		return size;
	}

private:
	friend class CListQueueTest;	///< �t�����h�N���X�錾
};



}; // TNB
