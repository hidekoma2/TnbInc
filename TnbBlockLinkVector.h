#pragma once
/**
 *@file
 * �z��^���Ǘ��֌W�̃w�b�_
 *
 *		�^�̔z����Ǘ����܂��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �z��^���Ǘ��e���v���[�g
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂��B
 *
 *		�ǉ����Ƀ������m�ۂ͍s���܂����A�Ĕz�u���s��Ȃ����߁A
 *		���� Vector�ɔ�ׂāA�y�ʂł��B
 *
 *	@note �����u���b�N�P�ʂŊǗ����Ă���A����炪�����N���Ĉ�̔z��Ƃ��ď������Ă��܂��B
 *
 *	
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@par�K�v�t�@�C��
 *			TnbBlockLinkVector.h
 *
 *	@date 11/02/10 �V�K
 *	@date 11/06/01 �J�����@�Ɍ��L��B�C���B
 *	@date 11/08/25 �u���b�N�T�C�Y�� 1 �� n �� �ɂ��邱�ƂŁA�p�t�H�[�}���X�A�b�v�B
 *	@date 11/09/06 GetBlockInformation(), GetBlockCount() ��ǉ��B
 *	@date 14/05/18 RemoveElements() �����ǁB
 */
template<typename TYP>
class CBlockLinkVectorT : public ICollectionMidT<TYP>, CCopyImpossible
{
	DEFSUPER(ICollectionMidT<TYP>);
public:

	/// �u���b�N���
	struct TBlockInformation
	{
		const TYP*	pData;	///< �v�f�z��
		size_t		size;	///< �v�f��
	};


	//-------


	/**
	 * �R���X�g���N�^.
	 *	�u���b�N�T�C�Y�� 1 �� 10 ��ɂȂ�܂��B
	 */
	CBlockLinkVectorT(void) : m_totalSize(0)
	{
		m_blockShift = 10;
		m_blockSize = down_cast<size_t>(_BIT(m_blockShift));
		m_blockMask = m_blockSize - 1;
	}

	/**
	 * �R���X�g���N�^.
	 *	@param bs �u���b�N�T�C�Y�B���ۂɂ͊ۂ߂��A�w��̃T�C�Y�𒴂��� 1 �� n ��̒l�ɂȂ�܂��B
	 */
	CBlockLinkVectorT(size_t bs) : m_totalSize(0)
	{
		m_blockShift = 1;	///< m_blockSize �Ɗ֘A
		m_blockSize = 1 << m_blockShift;
		while ( m_blockSize < bs )
		{
			m_blockSize <<= 1;
			m_blockShift++;
			if ( m_blockSize == 0 )
			{
				m_blockShift = 7;
				m_blockSize = 1 << m_blockShift;
			}
		}
		m_blockMask = m_blockSize - 1;
	}

	/// �f�X�g���N�^
	~CBlockLinkVectorT(void)
	{
		RemoveAll();
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime ���b�N�Ƀg���C���������ؕb�B�ȗ�����Ɩ����ɑ҂��܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return m_sync.Lock(dwTime);
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		m_sync.Unlock();
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@note Pure�֐��ł��B
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_totalSize;
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
		size_t i = m_totalSize;
		INDEX bn = i >> m_blockShift;
		INDEX idx = i & m_blockMask;
		if ( bn >= m_blocks.GetSize() )
		{
			TYP* P = new TYP[m_blockSize];
			m_blocks.Add(P);
		}
		TYP* B = m_blocks[bn];
		B[idx] = t;
		m_totalSize++;
		return i;
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
		INDEX is = m_totalSize;
		size_t i = m_totalSize - 1 + size;
		INDEX bn = i >> m_blockShift;
		while ( bn >= m_blocks.GetSize() )
		{
			TYP* P = new TYP[m_blockSize];
			m_blocks.Add(P);
		}
		m_totalSize += size;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				Set(is + i, *P++);
			}
		}
		else
		{
			loop ( i, min(m_blockSize, size) )	//memo �S�����K�v�͂Ȃ��̂ŁA��u���b�N���̂�
			{
				Set(is + i, TYP());
			}
		}
		return size;
	}

	/**
	 * [�폜] �S�v�f�폜 .
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool RemoveAll(void)
	{
		m_totalSize = 0;
		m_blocks.RemoveAll();
		return true;
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval false	�G���[�B
	 *	@retval true	�����B
	 */
	virtual bool Remove(INDEX index)
	{
		if ( index >= m_totalSize )
		{
			return false;
		}
		size_t d = m_totalSize - index;
		if ( d > 1 )
		{
			loop ( i, d - 1 )
			{
				Set(index + i, At(index + i + 1));
			}
		}
		m_totalSize--;
		if ( m_totalSize == 0 )
		{
			m_blocks.RemoveAll();
		}
		else
		{
			INDEX bn = (m_totalSize - 1) >> m_blockShift;
			if ( bn < m_blocks.GetSize() )
			{
				m_blocks.SetSize(bn + 1);
			}
		}
		return true;
	}

	/**
	 * [�폜] �v�f�폜.
	 *	@param index �C���f�b�N�X
	 *	@param size �폜���B�ȗ�����ƂO�B�O���w�肷��Ƃ���ȍ~�S���폜���܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�폜�������B
	 */
	virtual size_t RemoveElements(INDEX index, size_t size = 0)
	{
		EXCLUSIVE( &m_sync );
		size_t sz = GetSize();
		if ( sz < index )
		{
			return INVALID_SIZE;
		}
		if ( size == 0 || sz < index + size)
		{
			SetSize(index);
			return sz - index;
		}
		size_t z = sz - size - index;
		loop ( i, z )
		{
			Set(index + i, Get(index + size + i));
		}
		SetSize(index + z);
		return size;
	}

	/**
	 * [�擾] �u���b�N���擾.
	 *		�Ǘ����Ă���u���b�N�����擾�ł��܂��B
	 *	@attention ���̃��\�b�h�œ�������́A Set() , Add() �Ȃǂ̏���ύX���郁�\�b�h��
	 *			�R�[������Ɩ����Ȓl�ɂȂ�ꍇ���L��܂��B
 	 *	@return �u���b�N��
 	 */
	size_t GetBlockCount(void) const
	{
		return m_blocks.GetSize();
	}

	/**
	 * [�擾] �u���b�N���擾.
	 *		�Ǘ����Ă���u���b�N�����擾�ł��܂��B
	 *	@attention ���̃��\�b�h�œ�������́A Set() , Add() �Ȃǂ̏���ύX���郁�\�b�h��
	 *			�R�[������Ɩ����Ȓl�ɂȂ�ꍇ���L��܂��B
	 *	@param[out] _bi �u���b�N��񂪊i�[����܂��B
	 *	@param[in] index �u���b�N�C���f�b�N�X�B 0 �ȏ�A GetBlockCount() �����̒l���w��ł��܂��B
 	 *	@retval true ����
 	 *	@retval false ���s
 	 */
	bool GetBlockInformation(TBlockInformation& _bi, INDEX index) const
	{
		if ( m_blocks.IsInRange(index ) )
		{
			_bi.size = m_blockSize;
			if ( index == m_blocks.GetSize() - 1 )
			{
				_bi.size = (m_totalSize & m_blockMask);
			}
			_bi.pData = m_blocks[index];
			return true;
		}
		return false;
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
		if ( index >= m_totalSize )
		{
			return NULL;
		}
		INDEX bn = index >> m_blockShift;
		INDEX idx = index & m_blockMask;
		TYP* B = m_blocks[bn];
		return &B[idx];
	}

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		if ( index >= m_totalSize )
		{
			return NULL;
		}
		INDEX bn = index >> m_blockShift;
		INDEX idx = index & m_blockMask;
		TYP* B = m_blocks[bn];
		return &B[idx];
	}

private:
	CSyncSection						m_sync;			///< �r���p
	size_t								m_totalSize;	///< �S�̂̃T�C�Y
	CVectorT<CArrayPtrHandleT<TYP> >	m_blocks;		///< �u���b�N���ɊǗ�
	size_t								m_blockSize;	///< ��̃u���b�N�̃T�C�Y
	int									m_blockShift;	///< m_blockSize �Ɗ֘A
	INDEX								m_blockMask;	///< m_blockSize �Ɗ֘A
	friend class CBlockListVectorTest;	///<�t�����h�N���X�錾
};



}; // TNB

