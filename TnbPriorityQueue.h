#pragma once
/**
 *@file
 * �D�揇�ʕt�L���[�^���Ǘ��֌W�̃w�b�_
 *
 *		�ǉ����������A�D�揇�ʂ������ق�������o�邱�Ƃ��ł���N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbListQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �D�揇�ʕt�L���[�^���Ǘ��e���v���[�g
 *	
 *		�ǉ����������A�D�揇�ʂ������ق�������o�邱�Ƃ��ł���N���X������܂��B
 *
 *	@note �L���[�̑傫���͉ςł��B�������̋����������܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	IQueueT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPrioritytQueue.h
 * 
 *	@date 06/04/25 �V�K�쐬
 *	@date 06/05/15 Clone��p�~�B
 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 */
template<typename TYP>
class CPriorityQueueT : public ICollectionMidT<TYP>, public IQueueT<TYP>, CCopyImpossible
{
	DEFSUPER(ICollectionMidT<TYP>);
	CListT<CListQueueT<TYP> >	m_listQueue;	///< ���Ǘ�
	int							m_iLevelMax;	///< ���x���͈�

	/**
	 * ���x�����C���f�b�N�X�ϊ�
	 *	@param iLevel ���x��
	 *	@retval INVALID_INDEX �G���[�B
	 *	@retval ����ȊO �C���f�b�N�X�B
	 */
	INDEX m_LevelToIndex(int iLevel)
	{
		if ( m_iLevelMax < iLevel )
		{
			return INVALID_INDEX;
		}
		iLevel += m_iLevelMax;
		if ( iLevel < 0 )
		{
			return INVALID_INDEX;
		}
		return iLevel;
	}

protected:

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const
	{
		if ( index >= 0 )
		{
			try
			{
				for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
				{
					size_t size = m_listQueue[i].GetSize();
					if ( size > index )
					{
						return &(m_listQueue[i][index]);	//�͈͓��̂͂�
					}
					index -= size;
				}
			}
			catch(CTnbException& e)
			{
				e.OnCatch();
			}
		}
		return NULL;
	}

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		if ( index >= 0 )
		{
			try
			{
				for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
				{
					size_t size = m_listQueue[i].GetSize();
					if ( size > index )
					{
						return &(m_listQueue[i][index]);	//�͈͓��̂͂�
					}
					index -= size;
				}
			}
			catch(CTnbException& e)
			{
				e.OnCatch();
			}
		}
		return NULL;
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param iLevel �v���C�I���e�B���x���͈́B���Ƃ��΂S�ɂ���ƁA-�S�`�S �܂łɂȂ�܂��B
	 *					�f�t�H���g�ł́A�R�i-�R�`�R�j�܂łɂȂ��Ă��܂��B
	 */
	explicit CPriorityQueueT(int iLevel = 3) : _super()
	{
		if ( iLevel < 0 )
		{
			iLevel = -iLevel;
		}
		m_iLevelMax = iLevel;
		CListQueueT<TYP> q;
		loop ( i, iLevel * 2 + 1 )
		{
			m_listQueue.Add(q);
		}
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
		return m_listQueue.Lock();
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		m_listQueue.Unlock();
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �l
	 */
	virtual size_t GetSize(void) const 
	{
		EXCLUSIVE(this);
		size_t size = 0;
		for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
		{
			size += m_listQueue[i].GetSize();
		}
		return size;
	}


	/**
	 * [�ǉ�] �v�f�ǉ�.
	 *		�A�������f�[�^���v���C�I���e�B���x���O�̃L���[�̍Ō�ɒǉ����܂��B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �ǉ�����v�f�B�ȗ��\�B
	 *	@return �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t)
	{
		return Add(0, t);
	}

	/**
	 * [�ǉ�] �����v�f�ǉ�.
	 *		�A�������f�[�^���v���C�I���e�B���x���O�̃L���[�̍Ō�ɒǉ����܂��B
	 *	@param size �ǉ�����f�[�^�̐�
	 *	@param P �ǉ�����f�[�^��̃|�C���^�B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		return AddElements(0, size, P);
	}

	/**
	 * [�폜] �f�[�^�S�폜.
	 *		�L���[�̓��e����ɂ��܂��B
	 *	@return ���true
	 */
	virtual bool RemoveAll(void)
	{
		return m_listQueue.RemoveAll();
	}
	
	/**
	 * [�擾] ���x���ʗv�f���擾
	 *	@param iLevel �v���C�I���e�B���x��
	 *	@retval INVALID_SIZE	�G���[�B���x���w����
	 *	@retval ����ȊO		���̃��x���̐��B
	 */
	size_t GetSize(int iLevel) const 
	{
		EXCLUSIVE(this);
		INDEX index = m_LevelToIndex(iLevel);
		if ( index != INVALID_INDEX )
		{
			return m_listQueue[index].GetSize();
		}
		return INVALID_SIZE;
	}

	/**
	 * [�ǉ�] ���x���ʕ����v�f�ǉ�.
	 *		�A�������f�[�^���w��̃v���C�I���e�B���x���̃L���[�̍Ō�ɒǉ����܂��B
	 *	@param iLevel �v���C�I���e�B���x��
	 *	@param size �ǉ�����f�[�^�̐��B
	 *	@param P �ǉ�����f�[�^��̃|�C���^�B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	size_t AddElements(int iLevel, size_t size, const TYP* P = NULL)
	{
		EXCLUSIVE(this);
		if ( P != NULL )
		{
			loop ( i, size )
			{
				Add(iLevel, *P++);
			}
		}
		else
		{
			loop ( i, size )
			{
				Add(iLevel, TYP());
			}
		}
		return size;
	}

	/**
	 * [�ǉ�] ���x���ʗv�f�ǉ�.
	 *		�A�������f�[�^���w��̃v���C�I���e�B���x���̃L���[�̍Ō�ɒǉ����܂��B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param iLevel �v���C�I���e�B���x��
	 *	@param t �ǉ�����v�f
	 *	@retval INVALID_INDEX ���s�B(���x���w��s��)
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX Add(int iLevel, const TYP& t)
	{
		INDEX index = m_LevelToIndex(iLevel);
		if ( index == INVALID_INDEX )
		{
			return INVALID_INDEX;
		}
		return m_listQueue[index].Add(t);
	}

	/**
	 * [�ǉ�] ���x���ʗv�f�ǉ�.
	 *		�A�������f�[�^���w��̃v���C�I���e�B���x���̃L���[�̍Ō�ɒǉ����܂��B
	 *	@param iLevel �v���C�I���e�B���x��
	 *	@param t �ǉ�����v�f
	 *	@retval INVALID_INDEX ���s�B(���x���w��s��)
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX AddEx(int iLevel, const TYP& t)
	{
		EXCLUSIVE(this);
		return Add(iLevel, t);
	}

	/**
	 * [�ǉ�] ���x���ʒǉ�.
	 *		�A�������f�[�^���w��̃v���C�I���e�B���x���̃L���[�̍Ō�ɒǉ����܂��B
	 *	@note �ǉ����f�[�^�͈̔͂����肵�����ꍇ�ACConstOffsetAdapterT ���g�p���܂��B
	 *	@param iLevel �v���C�I���e�B���x��
	 *	@param c �ǉ�����f�[�^
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	size_t Append(int iLevel, const IConstCollectionT<TYP> &c)
	{
		EXCLUSIVE(this);
		INDEX index = m_LevelToIndex(iLevel);
		if ( index == INVALID_SIZE )
		{
			return INVALID_SIZE;
		}
		return m_listQueue[index].Append(c);
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
		for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
		{
			if ( ! m_listQueue[i].IsEmpty() )
			{
				TYP t = m_listQueue[i].Get(0);
				m_listQueue[i].Remove(0);
				return t;
			}
		}
		ASSERT0(false, "CPriorityQueueT::Take()", "�L���[����̎��擾���삳��܂����B");
		throw CEmptyException();
	}

	/**
	 * [�擾] ���x���ʐ擪�v�f���o��.
	 *		�L���[����폜���܂�
	 *	@param iLevel �v���C�I���e�B���x��
	 *	@return �v�f���e
	 *	@throw CEmptyException ��ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP Take(int iLevel)
	{
		EXCLUSIVE(this);
		INDEX index = m_LevelToIndex(iLevel);
		if ( index != INVALID_INDEX )
		{
			if ( ! m_listQueue[index].IsEmpty() )
			{
				TYP t = m_listQueue[index].Get(0);
				m_listQueue[index].Remove(0);
				return t;
			}
		}
		ASSERT0(false, "CPriorityQueueT::Take()", "�L���[����̎��擾���삳��܂����B");
		throw CEmptyException();
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
		if ( size > GetSize() )
		{
			return INVALID_SIZE;
		}
		if ( P != NULL )
		{
			loop ( i, size )
			{
				*P++ = Take();
			}
		}
		else
		{
			loop ( i, size )
			{
				Take();
			}
		}
		return size;
	}
};



}; // TNB
