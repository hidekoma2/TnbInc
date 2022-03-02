#pragma once
/**
 *@file
 * �z��^���Ǘ��֌W�̃w�b�_
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbException.h"
#include "TnbCollection.h"
#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �z��^���Ǘ��e���v���[�g
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *	
 *	@note �֐��̖߂�^�Ƃ��Ďg�p����Ƃ��ɕ֗��ȁA�u������ԁv���Ǘ��ł��܂�
 *			( Invalid() , IsValid() )�B
 *
 *	@note	begin() / end() �ɂ��C�e���[�^�́A�P���ɗv�f�̃|�C���^�ɂȂ��Ă��܂��B
 *			���̂��߁A�����ɃA�N�Z�X���\�ł��B�������A�X���b�h�Z�[�u�ł͗L��܂���̂ŁA
 *			�X���b�h�Ŏg�p���Ă���ꍇ�ALock����悤�ɂ��Ă��������B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	ISequenceCollectionT ���������Ă��܂��B
 *
 *	@note �X���b�h�Z�[�u�A�R�s�[�@�\���Ȃ����ȈՓI�� CSimpleVectorT ������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbVector.h
 *
 *	@date 06/01/01 ��CDataBlock�𒊏ۉ��B
 *	@date 06/04/19 ICollectionT ����������悤�ɕύX�B
 *	@date 06/04/21 �R���X�g���N�g���ɒǉ��T�C�Y�w��\�ɂ����B
 *	@date 06/04/22 CVectorHandleT ��p�~�BClone() ���\�b�h�A�ǉ��B
 *	@date 06/05/01 ISequenceCollectionT ����������悤�ɕύX�B
 *	@date 06/05/15 Clone��p�~�B
 *	@date 06/05/23 AddElements�ȂǁAIF�Ɉڍs�������\�b�h�̍폜�B
 *	@date 06/05/30 Insert() ���\�b�h�A�ǉ��B
 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/13 �C�e���[�^�� STL �ɂ��킹�āA���C�B
 *	@date 06/08/25 SetSize �̍œK���B
 *	@date 06/09/13 CopyElements() ���\�b�h�AVector��p�ɒǉ��B
 *	@date 06/10/26 �œK���B
 *	@date 06/12/06 Remove()�ɍ폜���w��̃��\�b�h�A�ǉ��B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 08/01/09 Remove()�̕��o�O�C���B
 *	@date 09/09/07 CopyElements() -> SetElements() �ɉ����B
 *	@date 11/04/19 GetBuffer() ���̃R�s�[���T�|�[�g�B
 */
template<typename TYP>
class CVectorT : public ICollectionT<TYP> , public ISequenceCollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
public:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// const_iterator�^�錾
	typedef const TYP* const_iterator;
	// iterator�^�錾
	typedef TYP* iterator;
	#endif //_TnbDOXYEM

	/**
	 * [����] �擪const_iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return m_hptHead->data(0); }

	/**
	 * [����] �Ō�const_iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return m_hptHead->data(GetSize()); }

	/**
	 * [����] �擪iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator begin(void) { m_Separate(); return m_hptHead->data(0); }

	/**
	 * [����] �Ō�iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator end(void) { m_Separate(); return m_hptHead->data(GetSize()); }

	/**
	 * [����] �}��
	 *	@param ite �}������ʒu�� iterator
	 *	@param t �v�f
	 *	@return �}�������ꏊ�� iterator
	 */
	iterator insert(iterator ite, const TYP& t = TYP())
	{
		INT_PTR index = ite - begin();
		Insert(index, t);
		return begin() + index;
	}

	/**
	 * [����] �폜
	 *	@param ite �폜����ʒu�� iterator
	 *	@return �폜�������� iterator
	 */
	iterator erase(iterator ite)
	{
		INT_PTR index = ite - begin();
		if ( ! Remove(index) ) { return end(); }
		return begin() + index;
	}

	/**
	 * [����] �擪�ɑ}��
	 *	@param t �v�f
	 */
	void push_front(const TYP& t) { Insert(0, t); }

	/**
	 * [����] �Ō�ɒǉ�
	 *	@param t �v�f
	 */
	void push_back(const TYP& t) { Add(t); }


	//------------------------


	/**
	 * �R���X�g���N�^
	 *	@note �w��̃������m�ێ��̒ǉ��T�C�Y��ݒ肵�܂��B
	 *	@note �f�[�^�͕ێ����܂���
	 *	@param size �������m�ێ��̒ǉ��T�C�Y�B SetIncrementSize() �Ɠ����ł��B
	 */
	explicit CVectorT(size_t size = 0) : _super()
	{
		m_Init();
		SetIncrementSize(size);
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@note �R�s�[���ƃo�b�t�@�����L���܂��B
	 *	@param other �R�s�[���� CVectorT �C���X�^���X
	 */
	CVectorT(const CVectorT<TYP>& other) : _super()
	{
		m_Init();
		if ( other.m_hptHead.GetReferCount() < 0)
		{
			SetElements(other.GetSize(), other.ReferBuffer());
		}
		else
		{
			m_hptHead = other.m_hptHead;
		}
	}
	
	/// �f�X�g���N�^
	virtual ~CVectorT(void)
	{
		m_hptHead.Null();
	}

	/**
	 * [���] ���
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@���J�����A	
	 *		������̏������L����悤�ɂȂ�܂��B
	 *	@param other ������� CVectorT 
	 *	@return �����̎Q��
	 */
	CVectorT& operator=(const CVectorT& other)
	{
		EXCLUSIVE(&other);
		if ( other.m_hptHead.GetReferCount() < 0)
		{
			SetElements(other.GetSize(), other.ReferBuffer());
		}
		else
		{
			m_hptHead = other.m_hptHead;
		}
		return *this;
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A GetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̕���
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP Get(INDEX index) const
	{
		return * m_hptHead->datack(index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		return * m_hptHead->datack(index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̎Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP& Ref(INDEX index)
	{
		TYP* P = m_GetPtr(index);
		if ( P == NULL )
		{
			throw CIndexOutOfBoundsException();
		}
		return *P;
	}
	
	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A SetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		TYP* P = m_GetPtr(index);
		if ( P == NULL )
		{
			return false; 
		}
		*P = t;
		return true;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& operator[](INDEX index) const
	{
		return At(index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB���삷��ƃI�u�W�F�N�g���̗v�f�ɉe�����L��܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP& operator[](INDEX index)
	{
		TYP* P = m_GetPtr(index);
		if ( P != NULL ){ return *P; }
		throw CIndexOutOfBoundsException();
	}
	
	/**
	 * [�ǉ�] �ǉ��I�y���[�^
	 *	@note ���C���X�^���X�ŊǗ����Ă���f�[�^�Ɏw��̃f�[�^��
	 *		 �ǉ����܂��B
	 *	@param vector �ǉ�����CVectorT
	 *	@return �����̎Q��
	 */
	CVectorT& operator+=(const CVectorT<TYP>& vector)
	{
		AddElements(vector.GetSize(), vector.ReferBuffer());
		return *this;
	}

	/**
	 * [�쐬] �A��
	 *	@note	���C���X�^���X�ŊǗ����Ă���f�[�^�Ɏw��̃f�[�^��
	 *			�A�������A CVectorT ���쐬���܂��B
	 *			���C���X�^���X�͕ω����܂���B
	 * @param other �A������ CVectorT
	 * @return �V�K�C���X�^���X
	 */
	CVectorT<TYP> operator+(const CVectorT<TYP>& other) const
	{
		CVectorT v = *this;
		v += other;
		return v;
	}

	/**
	 * [�ݒ�] �]���T�C�Y
	 *	@note �V�K�Ƀ��������m�ۂ����Ƃ��A�K�v�ȃT�C�Y��肢������
	 *		�m�ۂ��Ă������A�Ƃ����l��ݒ�ł��܂��B
	 *	@note �ʏ�͎����ł��B
	 *	@note �R�s�[�ł͂��̒l�͈����p����܂���B
	 *	@param size �������m�ێ��̒ǉ��T�C�Y�B�}�C�i�X�Ȃ玩���ł��B
	 */
	void SetIncrementSize(size_t size)
	{
		m_incrementSize = size;
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
	 * [�擾] �T�C�Y�擾
	 *	@return �{�C���X�^���X���Ǘ����Ă���f�[�^�̒����B
	 */
	virtual size_t GetSize(void) const
	{
		ASSERTLIB(! m_hptHead.IsNull());
		return m_hptHead->dataSize;
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *		���C���X�^���X�ŊǗ����Ă���o�b�t�@�Ɏw��̈�f�[�^��A�����܂��B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �v�f�B
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
		return RemoveElements(index, 1) == 1;
	}

	/**
	 * [�폜] �v�f�폜.
	 *	@param index �C���f�b�N�X
	 *	@param size �폜���B�ȗ�����ƂO�B�O���w�肷��Ƃ���ȍ~�S���폜���܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@return ����ȊO		�폜�������B
	 */
	virtual size_t RemoveElements(INDEX index, size_t size = 0)
	{
		EXCLUSIVE(this);
		if ( IsInRange(index) )
		{
			m_Separate();
			size_t nowSize = GetSize();
			TYP* P = m_hptHead->pBuffer;
			//�폜�T�C�Y����
			if ( size == 0 || nowSize - index <= size )
			{
				if ( index == 0 )
				{
					return RemoveAll() ? nowSize : INVALID_SIZE;
				}
				size = nowSize - index;
			}
			//�폜�͈͂̌�̈ړ����K�v�Ȑ�
			if ( nowSize > index + size )
			{
				loop ( i, nowSize - (index + size) )
				{
					ASSERTLIB( index + i + size < nowSize );
					P[index + i] = P[index + i + size];
				}
			}
			//����Ȃ�����������
			size_t s = m_hptHead->dataSize - size;
			loop ( i, size )
			{
				P[i + s] = TYP();
			}
			//
			m_hptHead->dataSize -= size;
			return size;
		}
		return INVALID_SIZE;
	}

	/**
	 * [�ǉ�] �����v�f�ǉ�.
	 *		�Ō�ɒǉ����܂��B
	 *	@param size �ǉ�����f�[�^���B
	 *	@param P �ǉ�����f�[�^�B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@return ����ȊO		�ǉ��������B
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( size == 0 )
		{
		}
		else if ( m_hptHead.IsNull() )
		{
			ASSERTLIB(false);
			//=== �����͏��������Ă��Ȃ�
			THead* lpTmp = m_CreateHead(size);
			m_CopyBuffer(lpTmp->pBuffer, P, size);
			m_hptHead = lpTmp;
		}
		else
		{
			EXCLUSIVE(this);
			m_hptHead->syncObj.Lock();
			THead* ptHead = m_hptHead;
			if ( m_hptHead.GetReferCount() > 1 )
			{
				size_t l = ptHead->dataSize + size;
				THead* lpTmp = m_CreateHead(l);
				m_CopyBuffer(lpTmp->pBuffer, ptHead->pBuffer, ptHead->dataSize);
				m_CopyBuffer(lpTmp->data(ptHead->dataSize), P, size);
				m_hptHead->syncObj.Unlock();
				m_hptHead = lpTmp;
			}
			else
			{
				m_hptHead->syncObj.Unlock();
				size_t l = ptHead->dataSize + size;
				//��𑫂����o�b�t�@�T�C�Y���m�ۂ��Ă���H
				if ( ptHead->bufferSize >= l )
				{
					//== �����
					m_CopyBuffer(ptHead->data(ptHead->dataSize), P, size);
					ptHead->dataSize = l;
				}
				else
				{
					//== ����Ȃ�
					//�V���Ƀ������m��
					size_t ss = m_GetSecureSize(l);
					TYP* N = new TYP[ss];
					//�f�[�^����
					m_CopyBuffer(&N[0], ptHead->pBuffer, ptHead->dataSize);
					m_CopyBuffer(&N[ptHead->dataSize], P, size);
					//���݂̃o�b�t�@���J��
					if ( ptHead->pBuffer != NULL )
					{
						delete[] ptHead->pBuffer;
					}
					//�V�����o�b�t�@�A�h���X���w��
					ptHead->bufferSize = ss;
					ptHead->dataSize = l;
					ptHead->pBuffer = N;
				}
			}
		}
		return size;
	}

	/**
	 * [�ݒ�] �����v�f�ݒ�.
	 *		�Ǘ����Ă���o�b�t�@���J�����A�w��̃f�[�^���R�s�[���܂��B
	 *	@param size �f�[�^���B
	 *	@param P �R�s�[������e���w��B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�����������B
	 */
	virtual size_t SetElements(size_t size, const TYP* P = NULL)
	{
		CVectorT v;
		size_t r = v.AddElements(size, P);
		if ( r >= 0 )
		{
			m_hptHead = v.m_hptHead;
		}
		return r;
	}
	
	/**
	 * [�ǉ�] �ǉ�.
	 *		�w��̃f�[�^��ǉ����܂��B
	 *	@note �ǉ����f�[�^�͈̔͂����肵�����ꍇ�A���f�[�^�� CConstOffsetAdapterT ���g�p���܂��B
	 *	@param c �ǉ�����f�[�^
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	virtual size_t Append(const IConstCollectionT<TYP>& c)
	{
		EXCLUSIVE2(&c, this);
		size_t as = c.GetSize();
		size_t j = GetSize();
		SetSize(j + as);
		TYP* P = m_hptHead->data(j);
		loop ( i, as )
		{
			*P++ = c.At(i);
		}
		return as;
	}

	/**
	 * [�폜] ��
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@����ɂ��܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool RemoveAll(void)
	{
		m_Init();
		return true;
	}

	/**
	 * [�ǉ�] �v�f��}��
	 *	@note �w��̃C���f�b�N�X�ɗv�f��ǉ����܂��B
	 *	@param index �C���f�b�N�X
	 *	@param t �v�f
	 *	@retval true	�}������
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	virtual bool Insert(INDEX index, const TYP& t)
	{
		EXCLUSIVE(this);
		if ( ! TNB::IsInRange(index, GetSize() + 1) )
		{
			return false;
		}
		if ( Add(TYP()) == INVALID_INDEX )
		{
			return false;
		}
		size_t size = GetSize() - 1;
		TYP* P = m_hptHead->pBuffer;
		for ( size_t i = size; i > index; i-- )
		{
			P[i] = P[i - 1];
		}
		P[index] = t;
		return true;
	}

	/**
	 * [����] ������Ԃɂ���
	 *	@note ��ԂƂ��Ă�RemoveAll�Ɠ����B
	 */
	void Invalid(void)
	{
		RemoveAll();
		m_hptHead->boValid = false;
	}

	/**
	 * [����] �T�C�Y�w��
	 *	@note �傫�����邱�Ƃ����������邱�Ƃ��o���܂��B
	 *	@note �傫���������A���̃f�[�^��TYP()�Ŗ��߂��܂��B
	 *	@param size �T�C�Y
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool SetSize(size_t size)
	{
		if ( size != INVALID_SIZE )
		{
			EXCLUSIVE(this);
			if ( IsEmpty() )
			{
				//�o�b�t�@�Ȃ�
				SetElements(size);
			}
			else if ( size == 0 )
			{
				m_Init();
			}
			else
			{
				size_t now = m_hptHead->dataSize;
				if ( now > size )
				{
					//����菬����
					m_Separate();
					TYP* P = m_hptHead->data(size);
					for ( size_t i = size; i < now; i++ )
					{
						*P++ = TYP();
					}
					m_hptHead->dataSize = size;
				}
				else if ( now < size )
				{
					//�����傫��
					AddElements(size - now);
				}
			}
		}
		return true;
	}

	/**
	 * [�擾] �f�[�^�A�h���X�擾
	 *	@note	���̃A�h���X�̓��e�͏��������Ă͂����܂���B���������邽�߂�
	 *			�A�h���X�� GetBuffer() ���g�p���Ď擾���Ă��������B
	 *	@note	�擾�����A�h���X�� delete �ŊJ�����Ă͂����܂���B
	 *	@note	�擾�����A�h���X�͖{�N���X�̌v�Z���A���삷��Ɩ����ɂȂ�܂��B
	 *	@return �A�h���X�B�m�ۂ��Ă��Ȃ��ꍇ�ANULL���Ԃ�܂��B
	 */
	virtual const TYP* ReferBuffer(void) const
	{
		ASSERTLIB(! m_hptHead.IsNull());
		return m_hptHead->pBuffer;
	}

	/**
	 * [�擾] �Q�Ɛ��擾
	 *	@note �Ǘ����Ă���f�[�^��������Q�Ƃ���Ă��邩���ׂ邱�Ƃ��o���܂��B
	 *	@return �Q�Ɛ�
	 */
	int GetReferCount(void)
	{
		return m_hptHead.GetReferCount();
	}

	/**
	 * [�m�F] �L���`�F�b�N
	 *	@note Invalid()�Ŗ����ɂ��ꂽ�Ƃ��Afalse �ɂȂ�܂��B
	 *		�����̓f�[�^�Ȃ���ԂƓ����ł��B
	 *	@retval true �L���i�f�[�^��Empty�̉\��������j
	 *	@retval false �����i�f�[�^��Empty�j
	 */
	bool IsValid(void) const
	{
		if ( IsEmpty() && ! m_hptHead->boValid )
		{
			return false;
		}
		return true;
	}

	/**
	 * [�쐬] �؂�o��
	 *	@note	���C���X�^���X�ŊǗ����Ă���f�[�^�̒�����
	 *			�C�ӂ̏ꏊ�̃f�[�^�����o���ACVectorT���쐬���܂��B
	 *			���C���X�^���X�͕ω����܂���B
	 * @param startIndex �؂�o���J�n�ʒu
	 * @param size	�؂�o���T�C�Y�B�ȗ�����ƁA�Ǘ����Ă���Ō�܂ł�؂�o���܂��B
	 * @return �V�K�C���X�^���X�B
	 */
	CVectorT<TYP> Mid(INDEX startIndex, size_t size = 0) const
	{
		CVectorT v;
		EXCLUSIVE(this);
		if ( ! IsEmpty() )
		{
			if ( m_hptHead->dataSize > startIndex )
			{
				if ( size == 0 || m_hptHead->dataSize - startIndex < size )
				{
					//�擾�f�[�^���܂Ŏ��o���Ȃ� �Ō�܂Ŏ擾�ɂ���
					size = m_hptHead->dataSize - startIndex;
				}
				v.SetElements(size, m_hptHead->data(startIndex));
			}	
		}
		return v;
	}

	/**
	 * [����] �f�[�^�A�h���X�擾
	 *
	 *		���̊֐��Ŏ擾�����f�[�^�̓��e�͎��R�ɏ��������Ă��܂��܂���B�������
	 *		�m�ۂ����T�C�Y�ȏ�̓A�N�Z�X���Ă͂����܂���B
	 *
	 *		�擾�����A�h���X�� delete �ŊJ�����Ă͂����܂���B
	 *
	 *		�f�[�^�̏����������I������� ReleaseBuffer() �����s���Ă��������B
	 *
	 *	@note	���̊֐����s������ł��ȑO�ƕς��ʎg�������o���܂��B�������A�����̍�
	 *			GetBuffer() ���s�� ReleaseBuffer() ����������܂ŁAClone���쐬����悤��
	 *			�Ȃ�܂��B
	 *	@note	�Q�Ƃ����ł���Ȃ� ReferBuffer() ���g�p���Ă��������B�����ł��B
	 *	@note	GetBuffer() ���s��A ReleaseBuffer() ����܂ŃN���X�ɑ΂��鑀���
	 *			�֎~�ł��i�擾�����A�h���X�������ɂȂ�\�������邽�߁j�B
	 *	@note	���̊֐��ł͑��Ƌ��p���Ă�����������Ǝ��Ɋm�ۂ��Ȃ����Ă��܂��B
	 *			���̃o�b�t�@�Ƃ͕�������邱�Ƃɒ��ӂ��Ă��������B
	 *	@param size �m�ۂ���o�b�t�@�T�C�Y�B�ȗ�����ƁA�ω������A�h���X��Ԃ��܂��B
	 *	@return �A�h���X	�f�[�^���Ȃ��ꍇNULL��������܂��B
	 */
	TYP* GetBuffer(size_t size = 0)
	{
		if ( IsEmpty() && size == 0 )
		{
			return NULL;
		}
		m_hptHead->syncObj.Lock();
		Lock();
		//---���N���X�Ƀo�b�t�@������
		int iCnt = m_hptHead.GetReferCount();
		if ( iCnt < 0 )
		{
			//---�����Œ�o�b�t�@�ɂȂ��Ă�
			m_hptHead->syncObj.Unlock();
			if ( size > 0 )
			{
				SetSize(size);
			}
		}
		else if ( iCnt == 1 )
		{
			//---���̃N���X�ł����g���ĂȂ�
			m_hptHead->syncObj.Unlock();
			if ( size > 0 )
			{
				SetSize(size);
			}
			//�Q�Ɛ���-�P�ɂ���B
			m_hptHead.SetReferCount(-1);
		}
		else
		{
			//---���ł��g���Ă�
			//�V���Ɋm�ۂ��˂�
			if ( size == 0 )
			{
				size = m_hptHead->dataSize;
			}
			//�V���Ƀ������m��
			THead* lpTmp = m_CreateHead(size);
			//�f�[�^����
			m_CopyBuffer(lpTmp->pBuffer, m_hptHead->pBuffer, size);
			//�V�����o�b�t�@�A�h���X���w��
			m_hptHead->syncObj.Unlock();
			m_hptHead = lpTmp;
			//�Q�Ɛ���-�P�ɂ���B
			m_hptHead.SetReferCount(-1);
		}
		Unlock();
		return m_hptHead->pBuffer;
	}

	/**
	 * [����] �f�[�^�̊Ǘ������ɖ߂�.
	 *			GetBuffer() �����s����ƃo�b�t�@�̓��e����������
	 *			�Ă����Ȃ��悤�Ƀo�b�t�@�̊Ǘ���ύX���܂��B
	 *			�{�֐��͂��̏�Ԃ����ɖ߂��֐��ł��B
	 *			�߂��Ȃ��ƁA�R�s�[�����̍ۂ̃I�[�o�[�w�b�h���傫���Ȃ�܂��B
	 *	@note	�{�֐����s��A��L�̊֐��Ŏ擾�����o�b�t�@���e�͏��������Ȃ��ł��������B
	 */
	void ReleaseBuffer(void)
	{
		if ( ! IsEmpty() )
		{
			m_hptHead->syncObj.Lock();
			//---���N���X�Ƀo�b�t�@������
			if ( m_hptHead.GetReferCount() < 0 )
			{
				//---�Œ�o�b�t�@�ɂȂ��Ă�
				//�Q��1�̕��ʂ̃o�b�t�@�ɂȂ�
				m_hptHead.SetReferCount(1);
			}
			m_hptHead->syncObj.Unlock();
		}
	}

	/**
	 * [�쐬] Vector�쐬
	 *	@note	�w��R���N�V�����̃f�[�^�� CVectorT �ɕϊ����܂��B
	 *			�w��R���N�V�����͕ω����܂���B
	 *	@param c	�ϊ����� IConstCollectionT �����C���X�^���X
	 *	@param size �v�f�T�C�Y�B�ȗ�����ƍŌ�܂łɂȂ�܂��B
	 *	@return �V�K CVectorT<TYP>
	 */
	inline friend CVectorT<TYP> ToVector(const IConstCollectionT<TYP>& c, size_t size = 0)
	{
		EXCLUSIVE(&c);
		size_t l = c.GetSize();
		if ( size == 0 || size > l )
		{
			size = l; 
		}
		CVectorT vb;
		vb.SetSize(size);
		TYP* P = vb.m_hptHead->data();
		loop ( i, size )
		{
			*P++ = c.At(i);
		}
		return vb;
	}

private:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/// �z��^���Ǘ��̊Ǘ��^
	struct THead
	{
		CSyncSection syncObj;	///< �Q�Ɣr��
		size_t	bufferSize;		///< �o�b�t�@�̒���
		size_t	dataSize;		///< ���ۂ̃f�[�^�̒���
		bool	boValid;		///< �L����ԂȂ�true�A������ԂȂ�false
		TYP *	pBuffer;		///< �o�b�t�@

		/// �R���X�g���N�^
		THead(void) : bufferSize(0) , dataSize(0) , boValid(true) , pBuffer(NULL)	{}

		/// �f�X�g���N�^
		virtual ~THead(void)
		{
			if ( pBuffer != NULL )
			{
				delete[] pBuffer;
				pBuffer = NULL;
			} 
		}

		/**
		 * �o�b�t�@�A�h���X�擾
		 *	@param f �I�t�Z�b�g
		 *	@return �o�b�t�@�A�h���X
		 */
		TYP* data(INDEX f = 0) const
		{
			return pBuffer + f; 
		}

		/**
		 * �`�F�b�N�t���o�b�t�@�A�h���X�擾
		 *	@param f �I�t�Z�b�g
		 *	@return �o�b�t�@�A�h���X
		 */
		TYP* datack(INDEX f = 0) const
		{
			if ( TNB::IsInRange(f, dataSize) )
			{
				return pBuffer + f; 
			}
			throw CIndexOutOfBoundsException();
		}
	};
	#endif

	CPointerHandleT<THead>	m_hptHead;			///< �Ǘ��p�w�b�_�̃|�C���^�n���h��
	CSyncSection			m_syncFunc;			///< �Q�Ɣr��
	size_t					m_incrementSize;	///< �v�f�X���b�g�̍ŏ���	

	/**
	 * �m�ۂ��ׂ��T�C�Y�擾
	 *	@param	size �K�v�T�C�Y
	 *	@return	�m�ۃT�C�Y
	 */
	size_t m_GetSecureSize(size_t size)
	{
		//�ǉ��T�C�Y�v�Z
		size_t marginSize = m_incrementSize;
		if ( marginSize == 0 )
		{
			//�����ɂȂ��Ă���
			marginSize = size / 8;
			marginSize = (marginSize < 4) ? 4 : ((marginSize > 1024) ? 1024 : marginSize);
		}
		return size + marginSize;
	}

	/**
	 * �Ǘ��p�̈�m��
	 *	@note �Ǘ��p�̍\���̂𓮓I�Ɋm�ۂ����������܂��B
	 *	@note �K�v�T�C�Y���傫�߂ɂƂ��Ă����܂�
	 *	@param size �K�v�T�C�Y
	 *	@return HEAD�̃|�C���^
	 */
	THead* m_CreateHead(size_t size)
	{
		THead* lptTmpHead = new THead;
		//����������
		size_t bs = m_GetSecureSize(size);
		lptTmpHead->dataSize	= size;
		lptTmpHead->bufferSize	= bs;
		//�o�b�t�@���m�ۂ���
		lptTmpHead->pBuffer = new TYP[bs];
		return lptTmpHead;
	}

	/**
	 * �o�b�t�@�R�s�[
	 *	@note lpSrc��NULL�Ȃ�N���A�����ɂȂ�܂�
	 *	@param[out] _lpDst	�R�s�[��|�C���^�B
	 *	@param[in]	lpSrc	�R�s�[���|�C���^�BNULL�̏ꍇ�ATYP()�����B
	 *	@param[in]	iLen	�R�s�[�����B
	 */
	void m_CopyBuffer(TYP* _lpDst, const TYP* lpSrc, size_t iLen)
	{
		ASSERTLIB(_lpDst != NULL);
		if ( lpSrc == NULL )
		{
			TYP t = TYP();
			while ( iLen-- )
			{
				*_lpDst++ = t; 
			}
		}
		else
		{
			ASSERTLIB(! ::IsBadReadPtr(lpSrc, iLen));
			while ( iLen-- )
			{
				*_lpDst++ = *lpSrc++;
			}
		}
	}

	/**
	 * �����o�ϐ��̏�����
	 *	@note �R���X�g���N�^�p�A�����o�ϐ��̏�����
	 */
	void m_Init(void)
	{
		m_hptHead = new THead;
		m_incrementSize = 0;
	}

	/** ���ƃo�b�t�@�����L���Ă����番������ */
	void m_Separate(void)
	{
		m_hptHead->syncObj.Lock();
		if ( m_hptHead.GetReferCount() > 1 )
		{
			size_t size = m_hptHead->dataSize;
			//�V���Ƀ������m��
			THead* lpTmp = m_CreateHead(size);
			//�f�[�^����
			m_CopyBuffer(lpTmp->pBuffer, m_hptHead->pBuffer, size);
			//�V�����o�b�t�@�A�h���X���w��
			m_hptHead->syncObj.Unlock();
			m_hptHead = lpTmp;
		}
		else
		{
			m_hptHead->syncObj.Unlock();
		}
	}

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	TYP* m_GetPtr(INDEX index)
	{
		if ( TNB::IsInRange(index, m_hptHead->dataSize) )
		{
			m_Separate();
			return m_hptHead->data(index);
		}
		return NULL;
	}

	friend class CVectorTest;	///<�t�����h�N���X�錾
};



/**@ingroup COLLECT ITERATOR
 * �����g����^���Ǘ��e���v���[�g
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		CVectorT �Ƃقړ����ł����A Set() �A At() �ȂǂŁA�����I�ɔz����g�����܂��B<BR>
 *		�C�e���[�^���쒆�͊g������܂���B
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *	@see	ISequenceCollectionT ���������Ă��܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@par�K�v�t�@�C��
 *			TnbVector.h
 *
 *	@date 06/05/09�@�V�K
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
*/
template<typename TYP>
class CAutoVectorT : public CVectorT<TYP>
{
	DEFSUPER(CVectorT<TYP>);
public:

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A SetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		if ( GetSize() <= index )
		{
			SetSize(index + 1);
		}
		return _super::Set(index, t);
	}

	/**
	 * [�ǉ�] �v�f��}��
	 *	@note �w��̃C���f�b�N�X�ɗv�f��ǉ����܂��B
	 *	@param index �C���f�b�N�X
	 *	@param t �v�f
	 *	@retval true	�}������
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	virtual bool Insert(INDEX index, const TYP& t)
	{
		if ( GetSize() < index )
		{
			SetSize(index);
		}
		return _super::Insert(index, t);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB���삷��ƃI�u�W�F�N�g���̗v�f�ɉe�����L��܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& operator[](INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		return _super::operator[](index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB���삷��ƃI�u�W�F�N�g���̗v�f�ɉe�����L��܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP& operator[](INDEX index)
	{
		if ( GetSize() <= index )
		{
			SetSize(index + 1);
		}
		return _super::operator[](index);
	}
};



/**@ingroup COLLECT
 * BYTE�z��Ǘ��N���X
 *	@note ����prefix�́uvb�v�ł��B
 *	@par�K�v�t�@�C��
 *			TnbVector.h
 */
typedef TNB::CVectorT< BYTE > CByteVector;



/**@ingroup COLLECT
 * WORD�z��Ǘ��N���X
 *	@note ����prefix�́uvw�v�ł��B
 *	@par�K�v�t�@�C��
 *			TnbVector.h
 */
typedef TNB::CVectorT< WORD > CWordVector;



/**@ingroup COLLECT
 * DWORD�z��Ǘ��N���X
 *	@note ����prefix�́uvdw�v�ł��B
 *	@par�K�v�t�@�C��
 *			TnbVector.h
 */
typedef TNB::CVectorT< DWORD > CDwordVector;



#ifdef __AFX_H__

	/**@ingroup COLLECT
	 * [����] CByteArray���� CByteVector �փR�s�[
	 *	@note MFC�v���W�F�N�g�̎��̂ݎg�p�\�ł��B
	 *	@par�K�v�t�@�C��
	 *			TnbVector.h
	 *	@param ba �R�s�[���� CByteArray
	 *	@return CByteVector
	 */
	inline CByteVector ToByteVector(const CByteArray &ba)
	{
		CByteVector v;
		v.SetElements(static_cast<size_t>(ba.GetSize()), ba.GetData());
		return v;
	}

	/**@ingroup COLLECT
	 * [����] CByteVector ����CByteArray�փR�s�[
	 *	@note MFC�v���W�F�N�g�̎��̂ݎg�p�\�ł��B
	 *	@par�K�v�t�@�C��
	 *			TnbVector.h
	 *	@param ba �R�s�[��� CByteArray
	 *	@param dat �R�s�[���� CByteVector
	 */
	inline void Copy(CByteArray &ba, const CByteVector &dat)
	{
		ba.SetSize(dat.GetSize());
		MemCopy(ba.GetData(), dat.ReferBuffer(), dat.GetSize());
	}

#endif



}; // TNB

