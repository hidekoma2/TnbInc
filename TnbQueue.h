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



#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * �L���[�^���Ǘ��C���^�[�t�F�[�X�e���v���[�g
 *
 *		����FIFO(First In First Out)�ŊǗ�����N���X�ł��B 
 *		Add�n���\�b�h�Œǉ����ATake�n���\�b�h�ŌÂ��̂ق�����������o���܂��B
 *	
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	�{IF ���g���T�u�N���X�� ICollectionT ���ꏏ�Ɏ������܂��B
 *
 *	@date 06/04/17 CRingQueue�𒊏ۉ����č쐬
 *	@date 06/05/15 Clone��p�~�B
 *	@date 06/05/23 AddElements�ȂǁAIF�Ɉڍs�������\�b�h�̍폜�B
 *	@date 06/08/08 ICollectionT �̌p����p�~�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP>
struct IQueueT
{
	/// �f�X�g���N�^
	virtual ~IQueueT(void) 
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
	virtual size_t AddElements(size_t size, const TYP* P = NULL) = 0;

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �v�f�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t) = 0;

	/**
	 * [�擾] �����v�f���o��
	 *	@note	�w��o�b�t�@�ɃR�s�[���A�L���[����폜���܂��B
	 *	@param[in]	size	���o�����B
	 *	@param[out]	P		���o���o�b�t�@�B NULL�Ȃ�擾���܂���B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		���o�������B
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL) = 0;

	/**
	 * [�擾] �擪�v�f���o��.
	 *		�L���[����폜���܂�
	 *	@return �v�f���e
	 *	@throw CEmptyException ��ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP Take(void)
	{
		TYP t;
		if ( TakeElements(1, &t) == 1 )
		{
			return t;
		}
		ASSERT0( false, "IQueue::Take()", "�L���[����̎��擾���삳��܂����B" );
		throw CEmptyException();
	}

	/**
	 * [�폜] �v�f�폜.
	 *		�L���[����w�萔�̃f�[�^���폜���܂��B
	 *	@param size �폜���������B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�폜�������B
	 */
	virtual size_t EraseElements(size_t size)
	{
		return TakeElements(size);
	}

	/**
	 * [�擾] �v�f�z���o��.
	 *		�w��o�b�t�@�ɃR�s�[���A�L���[����폜���܂��B
	 *	@param[in]	size	���o�����B
	 *	@return �v�f�z��B�z��Empty�̏ꍇ�A�w�萔�̃f�[�^���Ȃ������Ƃ����Ӗ��B
	 */
	CVectorT<TYP> TakeoutToVector(size_t size)
	{
		CVectorT<TYP> v;
		bool boRc = (TakeElements(size, v.GetBuffer(size)) == size);
		v.ReleaseBuffer();
		if ( ! boRc )
		{
			v.Invalid();
		}
		return v;
	}
};



/**@ingroup COLLECT ITERATOR
 * �����O���L���[�^���Ǘ��e���v���[�g
 *	
 *	@note �L���[�̑傫���͌Œ�ł��B�ŏ��ɂ��ׂẴC���X�^���X������܂�
 *			(�j�������̂́A�f�X�g���N�^�̎��ł�)�B
 *			���̂��߃������̊m�ۊJ�����������܂���B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *	@param ISI	true�Ȃ�Get/Erase�������ɁA�s�K�v�ɂȂ����o�b�t�@�̕�����
 *				INV �������ď��������܂��B
 *				TYP���N���X�Ń��������m�ۂ��Ă���Ƃ��Ȃǂ�true�ɂ��܂��B
 *	@param INV	 ���������Ɏg���l�B�ȗ�����ƁATYP()���g���܂��B
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	IQueueT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbQueue.h
 * 
 *	@date 06/01/01 �V�K�쐬
 *	@date 06/04/13 CCopyImpossible�N���X�A�p��
 *	@date 06/04/25 �R�s�[�@�\�A�ǉ��B
 *	@date 06/05/15 Clone��p�~�B
 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 11/03/07 SetQueueSize(), GetQueueSize() ��ǉ��B�\�������B
 *	@date 13/02/28 IsFull() �V�K�A�ǉ��B
 */
template<typename TYP, bool ISI = false, class INV = TYP>
class CRingQueueT : public ICollectionMidT<TYP>, public IQueueT<TYP>
{
	DEFSUPER(ICollectionMidT<TYP>);
public:

	/**
	 * �R���X�g���N�^
	 *	@note	�R���X�g���N�^���ɁA�����O�L���[�̍ő�T�C�Y���w�肵�܂��B
	 *			���I�ɑ傫���Ȃ�܂���B
	 *	@param size �����O�L���[�̃T�C�Y�B�ȗ������10000�ł��B
	 */
	explicit CRingQueueT(size_t size = 10000) : _super(), m_hptHead(new THead(size))
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@note �R�s�[���ƃo�b�t�@�����L���܂��B
	 *	@param queue �R�s�[���� CRingQueueT �C���X�^���X
	 */
	CRingQueueT(const CRingQueueT& queue) : _super(), m_hptHead(queue.m_hptHead)
	{
	}
	
	/**
	 * �f�X�g���N�^
	 */
	virtual ~CRingQueueT(void)
	{
		m_hptHead.Null();
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@���J�����A
	 *		������ƃo�b�t�@�����L���܂��B
	 *	@param other �������CRingQueueT
	 *	@return �����̎Q��
	 */
	CRingQueueT& operator=(const CRingQueueT& other)
	{
		EXCLUSIVE(&other);
		m_hptHead = other.m_hptHead;
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
		return m_syncFunc.Lock();
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		m_syncFunc.Unlock();
	}

	/**
	 * [�ݒ�] �����O�L���[�T�C�Y�w��.
	 *	@note �R���X�g���N�^�ł��w��ł��܂��B
	 *	@note ���̃��\�b�h���g���ƁA���̓N���A����܂��B
	 *	@param size �����O�L���[�̃T�C�Y�B
	 */
	void SetQueueSize(size_t size)
	{
		m_hptHead = new THead(size);
		ASSERT( GetSize() == 0 );
	}

	/**
	 * [�擾] �����O�L���[�T�C�Y�擾.
	 *	@return �����O�L���[�̃T�C�Y�B
	 */
	size_t GetQueueSize(void) const
	{
		return m_hptHead->bufferSize;
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �T�C�Y
	 */
	virtual size_t GetSize(void) const 
	{
		return m_hptHead->dataSize;
	}

	/**
	 * [�m�F] ���^���H
	 *	@retval true �L���[�͖��^���B����ȏ�ǉ��ł��Ȃ���ԁB
	 *	@retval false �ہB
	 */
	bool IsFull(void) const
	{
		if ( GetQueueSize() <= GetSize() )
		{
			return true;
		}
		return false;
	}

	/**
	 * [�ǉ�] �v�f�ǉ�.
	 *		�v�f����L���[�̍Ō�ɒǉ�����B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �ǉ�����v�f�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t)
	{
		size_t r = GetSize();
		return (AddElements(1, &t) == 1) ? r : INVALID_INDEX;
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
		if ( IsInRange(index) )
		{
			m_Separate();
			THead* H = *m_hptHead.ReferP();
			size_t sz = H->dataSize;
			INDEX ri = H->readIndex;
			size_t bs	= H->bufferSize;
			loop ( i, sz -index - 1 )
			{
				INDEX dst = (ri + i + index) % bs;
				INDEX src = (ri + i + index + 1) % bs;
				H->pBuffer[dst] = H->pBuffer[src];
			}
			if ( ISI )
			{
				INDEX dst = (ri + sz - 1) % bs;
				H->pBuffer[dst] = INV();
			}
			(H->dataSize)--;
			return true;
		}
		return false; 
	}

	/**
	 * [�폜] �f�[�^�S�폜.
	 *		�L���[�̓��e����ɂ��܂��B
	 *	@return ���true
	 */
	virtual bool RemoveAll(void)
	{
		EXCLUSIVE(this);
		size_t l = m_hptHead->dataSize;
		if ( l > 0 )
		{
			TakeElements(l);
			ASSERT( m_hptHead->dataSize == 0 );
		}
		return true;
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
		EXCLUSIVE(this);
		m_Separate();
		THead* H = *m_hptHead.ReferP();
		if ( (H->dataSize) + size > (H->bufferSize) )
		{
			//����ȏ�ɂȂ�Ȃ�G���[
			return INVALID_SIZE;
		}
		H->syncObj.Lock();
		size_t bs	= H->bufferSize;
		INDEX tmp = (H->readIndex + H->dataSize) % bs;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				H->pBuffer[tmp++] = *P++;
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
				H->pBuffer[tmp++] = TYP();
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(H->dataSize) += size;
		H->syncObj.Unlock();
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
		EXCLUSIVE(this);
		m_Separate();
		THead* H = *m_hptHead.ReferP();
		H->syncObj.Lock();
		if ( (H->dataSize) < size )
		{
			//�L���[�ɂ��̒����̏�񂪂Ȃ��̂ŃG���[
			H->syncObj.Unlock();
			return INVALID_SIZE;
		}
		size_t bs = H->bufferSize;
		if ( P != NULL || ISI )
		{
			INDEX tmp = H->readIndex;
			loop ( i, size )
			{
				if ( P != NULL )
				{
					*P++ = H->pBuffer[tmp];
				}
				if ( ISI )
				{
					H->pBuffer[tmp] = INV();
				}
				if ( ++tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(H->readIndex) += size;
		(H->readIndex) %= bs;
		(H->dataSize) -= size;
		H->syncObj.Unlock();
		return size;
	}

	/**
	 * [�擾] �i�[�\���擾.
	 *	@note Ring�L���[�̓����O�L���[�̃T�C�Y���Œ�ł��B
	 *	@return �T�C�Y
	 */
	size_t GetFreeSize(void) const
	{
		return (m_hptHead->bufferSize) - (m_hptHead->dataSize);
	}

protected:

	/// �z��^���Ǘ��̊Ǘ��^
	struct THead
	{
		CSyncSection	syncObj;		///< �Q�Ɣr��
		CSyncEvent		syncTaking;		///< Take�������ꎞ�I��Unlock�����(CBlockingQueueT�p)
		CSyncEvent		syncAdding;		///< Add�������ꎞ�I��Unlock�����(CBlockingQueueT�p)
		size_t			bufferSize;		///< �o�b�t�@�̒���
		size_t			dataSize;		///< ���ۂ̃f�[�^�̒���
		INDEX			readIndex;		///< �Ǎ��݃|�W�V����
		TYP*			pBuffer;		///< �o�b�t�@
		/// �R���X�g���N�^
		THead(size_t size) : readIndex(0), bufferSize(size), dataSize(0)
		{
			pBuffer = new TYP[size];
		}
		/// �f�X�g���N�^
		~THead(void)
		{
			if ( pBuffer != NULL )
			{
				delete[] pBuffer; 
			}
		}
	};

	CPointerHandleT<THead>	m_hptHead;			///< �Ǘ��p�w�b�_�̃|�C���^�n���h��

private:

	CSyncSection			m_syncFunc;			///< �Q�Ɣr��

	/** ���ƃo�b�t�@�����L���Ă����番������ */
	void m_Separate(void)
	{
		if ( m_hptHead.GetReferCount() > 1 )
		{
			size_t size = m_hptHead->bufferSize;
			THead* pNew = new THead(size);
			if ( (m_hptHead->dataSize) > 0 )
			{
				pNew->dataSize	= m_hptHead->dataSize;
				pNew->readIndex	= m_hptHead->readIndex;
				TYP* P1 = pNew->pBuffer;
				TYP* P2 = m_hptHead->pBuffer;
				while ( (size--) > 0 )
				{
					*P1++ = *P2++;
				}
			}
			m_hptHead = pNew;
		}
	}

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const
	{
		if ( IsInRange(index) )
		{
			INDEX tmp;
			THead* H = *m_hptHead.ReferP();
			tmp = (H->readIndex) + index;
			tmp %= (H->bufferSize);
			return &(H->pBuffer[tmp]);
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
		if ( IsInRange(index) )
		{
			m_Separate();
			INDEX tmp;
			THead* H = *m_hptHead.ReferP();
			tmp = (H->readIndex) + index;
			tmp %= (H->bufferSize);
			return &(H->pBuffer[tmp]);
		}
		return NULL;
	}

	friend class CQueueTest;///< �t�����h�N���X�錾
};



/**@ingroup COLLECT ITERATOR
 * �u���b�N�t�L���[�^���Ǘ��e���v���[�g
 *
 *		�w��T�C�Y�𒴂��Ă̒ǉ���A��̎��̎擾���Ƀu���b�N�����L���[�ł��B
 *		���́ACRingQueueT �Ɠ����ł��Btemplate �̃p�����[�^����������Q�Ƃ��Ă��������B
 *	
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	IQueueT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbQueue.h
 * 
 *	@date 06/04/25 �V�K�쐬
 *	@date 06/05/15 Clone��p�~�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP, bool ISI = false, class INV = TYP>
class CBlockingQueueT : public CRingQueueT<TYP, ISI, INV>
{
	typedef CRingQueueT<TYP, ISI, INV> _super;	///< �e�N���X���Đ錾
	DEFSUPER(_super);
public:

	/**
	 * �R���X�g���N�^
	 *	@note	�R���X�g���N�^���ɁA�����O�L���[�̍ő�T�C�Y���w�肵�܂��B
	 *			���I�ɑ傫���Ȃ�܂���B
	 *	@param size �����O�L���[�̃T�C�Y�B�ȗ������10000�ł��B
	 */
	explicit CBlockingQueueT(size_t size = 10000) : _super(size)
	{
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CBlockingQueueT(void)
	{
	}

	/**
	 * [�擾] �����v�f���o��.
	 *		�w��o�b�t�@�ɃR�s�[���A�L���[����폜���܂��B
	 *	@note �w��T�C�Y���o���Ȃ��ꍇ�A����܂ő҂��܂��B
	 *	@param[in]	size	���o�����B
	 *	@param[out]	P		���o���o�b�t�@�B NULL�Ȃ�擾���܂���B
	 *	@return ���o�����B
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		if ( _super::TakeElements(size, P) != size )
		{
			//�Ȃ�����
			while ( true )
			{
				m_hptHead->syncAdding.Lock();
				if ( _super::TakeElements(size, P) == size )
				{
					break;
				}
			}
		}
		m_hptHead->syncTaking.Pulse();
		return size;
	}

	/**
	 * [�ǉ�] �����v�f�ǉ�.
	 *		�A�������f�[�^���L���[�̍Ō�ɒǉ�����B
	 *	@note	��t�̏ꍇ�A�󂭂܂ő҂��܂��B
	 *	@param size �ǉ�����f�[�^�̐��B�ȗ������ TYP()���������܂��B
	 *	@param P �ǉ�����f�[�^��̃|�C���^
	 *	@return �ǉ������� (��ɐ������܂�) 
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( _super::AddElements(size, P) != size )
		{
			//�ǉ��ł��Ȃ�����
			loop ( i, size )
			{
				while ( true )
				{
					if ( _super::AddElements(1, P) == 1 )
					{
						break;
					}
					::Sleep(10);
				}
				m_hptHead->syncAdding.Pulse();
				P++;
			}
		}
		m_hptHead->syncAdding.Pulse();
		return size;
	}
};



/**@ingroup COLLECT
 * BYTE�^(unsigned 8bit)�L���[�N���X
 *	@par�K�v�t�@�C��
 *			TnbQueue.h
 */
typedef TNB::CRingQueueT<BYTE> CByteQueue;



/**@ingroup COLLECT
 * BYTE�^(unsigned 8bit)�u���b�L���O�L���[�N���X
 *	@par�K�v�t�@�C��
 *			TnbQueue.h
 */
typedef TNB::CBlockingQueueT<BYTE> CByteBlockingQueue;



/**@ingroup COLLECT
 * DWORD�^(unsigned 32bit)�L���[�Ǘ��N���X
 *	@par�K�v�t�@�C��
 *			TnbQueue.h
 */
typedef TNB::CRingQueueT<DWORD> CDwordQueue;



/**@ingroup COLLECT
 * CStr�^�L���[�Ǘ��N���X
 *	@par�K�v�t�@�C��
 *			TnbQueue.h
 */
typedef TNB::CRingQueueT<CStr,true> CStrQueue;



/**@ingroup COLLECT
 * CStr�^�u���b�L���O�L���[�Ǘ��N���X
 *	@par�K�v�t�@�C��
 *			TnbQueue.h
 */
typedef TNB::CBlockingQueueT<CStr,true> CStrBlockingQueue;



#ifdef __AFX_H__

	/**@ingroup COLLECT
	 * CString�^�L���[�Ǘ��N���X
	 *	@note MFC�v���W�F�N�g�̎��̂ݎg�p�\�ł��B
	 *	@par�K�v�t�@�C��
	 *			TnbQueue.h
	 */
	typedef TNB::CRingQueueT<CString,true> CStringQueue;

#endif



}; // TNB
