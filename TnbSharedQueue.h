#pragma once
/**
 *@file
 * ���L�������L���[�^���Ǘ��֌W�̃w�b�_
 *
 *		����FIFO(First In First Out)�ŊǗ�����N���X�ł��B 
 *		���́A���L�������ɕۊǂ��邽�߁A�v���Z�X�Ԃł��A���̋��L���o���܂��B
 *		�������A��������͊�{�^�A�y�сA��{�^�݈̂����\���݂̂̂ƂȂ�܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbQueue.h"
#include "TnbSharedMemory.h"
#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * ���L�L���[�^���Ǘ��e���v���[�g
 *	
 *		����FIFO(First In First Out)�ŊǗ�����N���X�ł��B 
 *		���́A���L�������ɕۊǂ��邽�߁A�v���Z�X�Ԃł��A���̋��L���o���܂��B
 *
 *	@note �L���[�̑傫���͌Œ�ł��B�ŏ��ɂ��ׂẴC���X�^���X������܂�
 *
 *	@param TYP �ێ�����^�i��{�^�A���邢�́A��{�^�݂̂̍\���̂Ɍ���j
 *	@param SIZ �L���[�̐��B�ȗ������ 10000 �ł��B
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	IQueueT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSharedQueue.h
 * 
 *	@date 06/09/12 �V�K�쐬
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP, size_t SIZ=10000>
class CSharedQueueT : public ICollectionMidT<TYP> , public IQueueT<TYP> , CCopyImpossible
{
	DEFSUPER(ICollectionMidT<TYP>);

	/// �z��^���Ǘ��̊Ǘ��^
	struct TInfo
	{
		size_t	bufferSize;		///< �o�b�t�@�̒���
		size_t	dataSize;		///< ���ۂ̃f�[�^�̒���
		INDEX	readIndex;		///< �Ǎ��݃|�W�V����
		INDEX	addIndex;		///< �ǉ��|�W�V����
		TYP		atData[SIZ];	///< �f�[�^
	};

	CSharedStructT<TInfo>	m_ssInfo;		///< �f�[�^���L������
	CSyncMutex *			m_psyncData;	///< �f�[�^�r���p

	/// ���L�������I�[�v�����̏����N���X
	class CSmOpenListener : public CSharedStructT<TInfo>::IListener
	{
	public:
		virtual void OnAction(TInfo* P)
		{
			P->bufferSize	= SIZ;
			P->dataSize	= 0;
			P->readIndex		= 0;
			P->addIndex		= 0;
		}
	};

protected:

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const
	{
		if ( m_psyncData == NULL ){ return NULL; }
		if ( ! IsInRange(index) ){ return NULL; }
		INDEX tmp = (m_ssInfo->readIndex) + index;
		tmp %= (m_ssInfo->bufferSize);
		return &(m_ssInfo->atData[tmp]);
	}

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		if ( m_psyncData == NULL ){ return NULL; }
		if ( ! IsInRange(index) ){ return NULL; }
		INDEX tmp = (m_ssInfo->readIndex) + index;
		tmp %= (m_ssInfo->bufferSize);
		return &(m_ssInfo->atData[tmp]);
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param boCanClose false�Ȃ�N���[�Y�����L����������Ȃ��Btrue���ȗ��Ȃ����B
	 *	@see CSharedStructT::CSharedStructT
	 */
	CSharedQueueT(bool boCanClose = true) : _super(), m_ssInfo(boCanClose)
	{
		m_psyncData = NULL;
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CSharedQueueT(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �I�[�v��
	 *	@note ���̃��\�b�h���s�Ȃ��ď��߂ăR���N�V�����Ƃ��Ďg�p�ł��܂��B
	 *	@param lpszName ���L���BTYP��SIZ���قȂ�N���X�̃C���X�^���X�œ������L���͎g�p���Ȃ��悤�ɂ��Ă��������B
	 *							�����^�̃C���X�^���X�ł��قȂ鋤�L���ɂ���ƕʂ̃L���[�ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Open(LPCTSTR lpszName)
	{
		Close();
		CSmOpenListener l;
		if ( m_ssInfo.Open(lpszName,&l) )
		{
			m_psyncData = new CSyncMutex(false, CStr::Fmt(_T("%s_Sync"), lpszName));
			if ( m_psyncData == NULL )
			{
				m_ssInfo.Close();
			}
		}
		return m_psyncData != NULL;
	}

	/**
	 * [�ݒ�] �N���[�Y
	 *		���̃��\�b�h���s�Ȃ��ƃR���N�V�����Ƃ��Ďg�p�ł��Ȃ��Ȃ�܂��B
	 *	@note �ʏ�g�p����K�v�͂���܂���B
	 */
	void Close(void)
	{
		m_ssInfo.Close();
		if ( m_psyncData != NULL )
		{
			delete m_psyncData;
			m_psyncData = NULL;
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
		if ( m_psyncData == NULL ){ return false; }
		return m_psyncData->Lock();
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		if ( m_psyncData == NULL ){ return; }
		m_psyncData->Unlock();
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �T�C�Y
	 */
	virtual size_t GetSize(void) const 
	{
		if ( m_psyncData == NULL ){ return 0; }
		return m_ssInfo->dataSize;
	}

	/**
	 * [�ǉ�] �v�f�ǉ�.
	 *		�v�f����L���[�̍Ō�ɒǉ�����B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �ǉ�����v�f�B
	 *	@retval INVALID_INDEX ���s�B�i�o�b�t�@��t�j�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t)
	{
		size_t r = GetSize();
		return (AddElements(1, &t) == 1) ? r : INVALID_INDEX;
	}

	/**
	 * [�폜] �f�[�^�S�폜.
	 *		�L���[�̓��e����ɂ��܂��B
	 *	@return ���true
	 */
	virtual bool RemoveAll(void)
	{
		if ( m_psyncData == NULL ){ return true; }
		return TakeElements(m_ssInfo->dataSize) != INVALID_SIZE;
	}

	/**
	 * [�ǉ�] �����v�f�ǉ�.
	 *		�A�������f�[�^���L���[�̍Ō�ɒǉ����܂��B
	 *	@attention	�w�肵���f�[�^���A�S������󂫂��Ȃ��ƃG���[�ɂȂ�܂��B
	 *	@param size �ǉ�����f�[�^�̐�
	 *	@param P �ǉ�����f�[�^��̃|�C���^�B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( m_psyncData == NULL )
		{
			return INVALID_SIZE; 
		}
		EXCLUSIVE(m_psyncData);
		if ( (m_ssInfo->dataSize) + size > (m_ssInfo->bufferSize) )
		{
			return INVALID_SIZE;	//����ȏ�ɂȂ�Ȃ�G���[
		}
		size_t bs	= m_ssInfo->bufferSize;
		INDEX tmp	= m_ssInfo->addIndex;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				m_ssInfo->atData[tmp++] = *P++;
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		else
		{
			loop ( i, size )
			{
				m_ssInfo->atData[tmp++] = TYP();
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(m_ssInfo->addIndex) += size;
		(m_ssInfo->addIndex) %= bs;
		(m_ssInfo->dataSize) += size;
		return size;
	}

	/**
	 * [�擾] �����v�f���o��.
	 *		�w��o�b�t�@�ɃR�s�[���A�L���[����폜���܂��B
	 *	@attention	�w�萔���A�f�[�^�����ƃG���[�ɂȂ�܂��B
	 *	@param[in]	size	���o�����B
	 *	@param[out]	P		���o���o�b�t�@�B NULL�Ȃ�擾���܂���B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		���o�������B
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		if ( m_psyncData == NULL )
		{
			return INVALID_SIZE;
		}
		EXCLUSIVE(m_psyncData);
		if ( (m_ssInfo->dataSize) < size )
		{
			return INVALID_SIZE;	//�L���[�ɂ��̒����̏�񂪂Ȃ��̂ŃG���[
		}
		size_t bs = m_ssInfo->bufferSize;
		if ( P != NULL )
		{
			INDEX tmp = m_ssInfo->readIndex;
			loop ( i, size )
			{
				*P++ = m_ssInfo->atData[tmp];
				if ( ++tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(m_ssInfo->readIndex) += size;
		(m_ssInfo->readIndex) %= bs;
		(m_ssInfo->dataSize) -= size;
		return size;
	}

	/**
	 * [�擾] �i�[�\���擾.
	 *	@note Ring�L���[�̓����O�L���[�̃T�C�Y���Œ�ł��B
	 *	@return �T�C�Y
	 */
	size_t GetFreeSize(void) const
	{
		if ( m_psyncData == NULL )
		{
			return 0;
		}
		return (m_ssInfo->bufferSize) - (m_ssInfo->dataSize);
	}

private:
	friend class CSharedQueueTest;///< �t�����h�N���X�錾
};



}; // TNB
