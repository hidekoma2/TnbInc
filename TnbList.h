#pragma once
/**
 *@file
 * ���X�g�^���Ǘ��֌W�̃w�b�_
 *
 *		�^�̔z����Ǘ����܂��B
 *		�z��̌X�����ꂼ�ꃁ�������m�ۂ������N���z��Ƃ��Ă邽�߁A�e�Ղ�
 *		�z�񐔂𑝂₷���Ƃ��ł��܂��i�������̍Ĕz�u�͔������Ȃ�)�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"
#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * ���X�g�^���Ǘ��e���v���[�g
 *
 *		�^�̔z����Ǘ����܂��B
 *		�z��̌X�����ꂼ�ꃁ�������m�ۂ������N���z��Ƃ��Ă邽�߁A�e�Ղ�
 *		�z�񐔂𑝂₷���Ƃ��ł��܂��i�������̍Ĕz�u�͔������Ȃ�)�B
 *
 *	@note	�e�v�f�ɑ����l�i�C�ӂ̌^�j����ɂ��邱�Ƃ��o���܂��B
 *			���̒l�́A Append() �� Copy() ���g���Ēǉ��A��������ƁA ATT() �ɂȂ�܂��B
 *			List ���m�̘A���A�����́A�I�y���[�^���g���Ă��������B
 *
 *	@note	List�R���N�V�����̃V���A���C�Y�A�f�V���A���C�Y�́A�K���{���X�g�R���N�V����
 *			�ōs���Ă��������B������񂪂��邽�߁A���̃R���N�V�����ƌ݊����L��܂���B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *	@param ATT TYP�Ƒ΂ɕێ����鑮���^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbList.h
 *
 *	@date 06/04/24 �V�K�쐬
 *	@date 06/04/28 SetSize()�ǉ��BElement�ɂ��T�C�Y�g���ɑΉ��B
 *	@date 06/05/03 Element�ɂ��T�C�Y�g����p�~�B
 *	@date 06/05/15 Clone��p�~�B
 *	@date 06/05/29 �e�v�f�Ƀp�����[�^���������悤�ɂ����B
 *	@date 06/08/01 GetConstPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/30 �V���A���C�Y�@�\�A�ǉ��B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 07/06/19 �p�����[�^�������Ƃ��A�e���v���[�g���B RemoveOnCheck() �� Cull() �ɉ����B
 */
template<typename TYP, typename ATT = DWORD>
class CListT : public ICollectionMidT<TYP>
{
	DEFSUPER(ICollectionMidT<TYP>);	

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/// LIST�̗v�f���
	struct TOne
	{
		TOne*	pNextOne;	///< ���̃p�[�c
		ATT		attribute;	///< �����l
		TYP		value;		///< �v�f���
		/// �R���X�g���N�^
		TOne(TYP t) : value(t) , attribute(ATT()) , pNextOne(NULL) 
		{
		}
		/** 
		 * Next���폜
		 *	@note Next�̎Q�Ɛ�ȉ��A���ׂ�Delete����܂��B
		 */
		void Delete(void)
		{
			if ( pNextOne != NULL )
			{
				pNextOne->Delete();
				delete pNextOne;
				pNextOne = NULL;
			}
		}
	};

	/** 
	 * �v�f�ێ��^
	 *	@note CListT�Ŏ��^�ł��B���̉��ɗv�f�������Ԃ牺����܂��B
	 *	@note value �͖��g�p�ł��B
	 */
	struct TTop : public TOne, CCopyImpossible
	{
		CSyncSection	m_syncObj;		///< �r���I�u�W�F�N�g
		size_t			size;			///< �p�[�c�̂Ԃ牺���萔
		TOne*			pLastOne;		///< �Ō��One�̈ʒu�L���i�L���b�V���I�Ȏg�p�@�j
		/// �R���X�g���N�^
		TTop(void) : TOne(TYP()) , size(0) , pLastOne(NULL) {}
		/// �f�X�g���N�^
		~TTop(void)
		{
			TOne::Delete();
		}
		/**
		 * �v�f�Ǘ��^�̃|�C���^�擾
		 *	@param index �C���f�b�N�X
		 *	@retval NULL	�C���f�b�N�X�͖���
		 *	@retval NULL�ȊO	TOne�̃|�C���^
		 */
		TOne* GetPointer(INDEX index) const
		{
			const TOne* P = this;
			for ( INDEX i = index + 1; i > 0; i-- )
			{
				if ( P->pNextOne == NULL )
				{
					P = NULL;
					break;
				}
				P = P->pNextOne;
			}
			return const_cast<TOne*>(P);
		}

		/**
		 * �v�f�Ǘ��̍Ō�̃|�C���^�擾
		 *	@return NULL�ȊO	TOne�̃|�C���^
		 */
		TOne* GetLastPointer(void)
		{
			#ifndef _DEBUG
				if ( pLastOne != NULL ){ return pLastOne; } 
			#endif
			TOne* P = this;
			while ( true )
			{
				if ( P->pNextOne == NULL )
				{
					break;
				}
				P = P->pNextOne;
			}
			ASSERTLIB(pLastOne == NULL || pLastOne == P);
			pLastOne = P; 
			return P;
		}
	};
	#endif

	CPointerHandleT<TTop> m_hpTop;	///< �v�f�ێ��̃|�C���^�n���h��
	CSyncSection		m_syncFunc;	///< �r���I�u�W�F�N�g

	/** ���ƃo�b�t�@�����L���Ă����番������ */
	void m_Separate(void)
	{
		m_hpTop->m_syncObj.Lock();
		if ( m_hpTop.GetReferCount() > 1 )
		{
			//�V���Ƀ������m��
			TTop* P = new TTop;
			TOne* pNew = P;
			//����
			TOne* pOld = m_hpTop->pNextOne;
			loop ( i, GetSize() )
			{
				ASSERTLIB(pOld != NULL);
				pNew->pNextOne = new TOne(pOld->value);
				pNew->pNextOne->attribute = pOld->attribute;
				P->size++;
				P->pLastOne = pNew->pNextOne;
				//
				pOld = pOld->pNextOne;
				pNew = pNew->pNextOne;
			}
			//�V�����o�b�t�@�A�h���X���w��
			m_hpTop->m_syncObj.Unlock();
			m_hpTop = P;
		}
		else
		{
			m_hpTop->m_syncObj.Unlock();
		}
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
		const TOne* P = m_hpTop->GetPointer(index);
		return (P == NULL) ? NULL : &(P->value);
	}

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		m_Separate();
		TOne* P = m_hpTop->GetPointer(index);
		return (P == NULL) ? NULL : &(P->value);
	}

public:

	/**
	 * [����] �}��
	 *	@param ite �}������ʒu�� iterator
	 *	@param t �v�f
	 *	@return �}�������ꏊ�� iterator
	 */
	iterator insert(iterator ite, const TYP& t = TYP())
	{
		INDEX index = ite.GetIndex();
		Insert(index, t);
		return begin() + index;
	}

	/**
	 * [����] �擪�ɑ}��
	 *	@param t �v�f
	 */
	void push_front(const TYP& t) 
	{
		Insert(0, t); 
	}


	//------------------------

	
	/// �R���X�g���N�^
	CListT(void) : _super(), m_hpTop(new TTop) 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CListT(const CListT& other) : _super(), m_hpTop(other.m_hpTop) 
	{
	}

	/// �f�X�g���N�^
	virtual ~CListT(void)
	{
		m_hpTop.Null();
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CListT& operator=(const CListT& other)
	{
		m_hpTop = other.m_hpTop;
		return *this;
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^.
	 *		���C���X�^���X�ŊǗ����Ă���f�[�^�Ɏw��̃f�[�^��
	 *		�ǉ����܂��B
	 *	@param list �ǉ��f�[�^
	 *	@return �����̎Q��
	 */
	CListT& operator+=(const CListT& list)
	{
		EXCLUSIVE2(list, *this);
		TOne* P = list.GetPointer(0);
		loop ( i, list.m_hpTop->GetSize() )
		{
			ASSERTLIB( P != NULL );
			VERIFYLIB( Add(P->value, P->attribute) != INVALID_INDEX );
			P = P->pNextOne;
		}
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

	/// [�r��] �A�����b�N
	virtual void Unlock(void) const
	{
		m_syncFunc.Unlock();
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_hpTop->size;
	}

	/**
	 * [�ǉ�] �v�f��ǉ�
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �v�f�B�ȗ��\�B
	 *	@return �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t)
	{
		return Add(t, ATT());
	}

	/**
	 * [�폜] �v�f��폜
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval true	�폜����
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	virtual bool Remove(INDEX index)
	{
		m_Separate();
		m_hpTop->m_syncObj.Lock();
		TOne* P = m_hpTop->GetPointer(index - 1);
		if ( P == NULL )
		{
			m_hpTop->m_syncObj.Unlock();
			return false;
		}
		TOne* pTmp = P->pNextOne;
		if ( pTmp == NULL )
		{
			m_hpTop->m_syncObj.Unlock();
			return false;
		}
		P->pNextOne = pTmp->pNextOne;
		delete pTmp;
		m_hpTop->size--;
		m_hpTop->pLastOne = NULL;
		m_hpTop->m_syncObj.Unlock();
		return true;
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
		return Set(index, t, ATT());
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool SetEx(INDEX index, const TYP& t)
	{
		return SetEx(index, t, ATT());
	}

	/**
	 * [�폜] �S�v�f�폜 
	 *	@return ���true
	 */
	virtual bool RemoveAll(void)
	{
		m_hpTop = new TTop;
		return true;
	}

	/**
	 * [�ݒ�] �v�f�̓���ւ�
	 *	@note �|�C���^���������ւ��Ă��܂��B�v�f�̃R�s�[�����͔������܂���B
	 *	@param index1 �C���f�b�N�X�P
	 *	@param index2 �C���f�b�N�X�Q
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual void Swap(INDEX index1, INDEX index2)
	{
		if ( index1 == index2 )
		{
			return;
		}
		EXCLUSIVE(this);
		m_Separate();
		TOne* P1 = m_hpTop->GetPointer(index1 - 1);
		TOne* P2 = m_hpTop->GetPointer(index2 - 1);
		if ( P1 == NULL || P2 == NULL ){ throw CIndexOutOfBoundsException(); }
		TOne* P1n = P1->pNextOne;
		TOne* P2n = P2->pNextOne;
		if ( P1n == NULL || P2n == NULL ){ throw CIndexOutOfBoundsException(); }
		TOne** P1nn = &(P1n->pNextOne);
		TOne** P2nn = &(P2n->pNextOne);
		//
		P1->pNextOne = P2n;
		P2->pNextOne = P1n;
		TOne* P = *P1nn;
		*P1nn = *P2nn;
		*P2nn = P;
		m_hpTop->pLastOne = NULL;
	}

	/**
	 * [�ǉ�] �v�f��ǉ�
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �v�f�B
	 *	@param attribute �����l�B
	 *	@return �ǉ�����INDEX�B
	 */
	INDEX Add(const TYP& t, const ATT& attribute)
	{
		m_Separate();
		m_hpTop->m_syncObj.Lock();
		INDEX r = m_hpTop->size;
		TOne* P = m_hpTop->GetLastPointer();
		ASSERTLIB(P != NULL);
		P->pNextOne = new TOne(t);
		P->pNextOne->attribute = attribute;
		m_hpTop->size++;
		m_hpTop->pLastOne = P->pNextOne;
		m_hpTop->m_syncObj.Unlock();
		return r;
	}

	/**
	 * [�ǉ�] �v�f��ǉ�
	 *	@param t �v�f�B
	 *	@param attribute �����l�B
	 *	@return �ǉ�����INDEX�B
	 */
	INDEX AddEx(const TYP& t, const ATT& attribute)
	{
		EXCLUSIVE(this);
		return Add(t, attribute);
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A SetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@param attribute �����l�B
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	bool Set(INDEX index, const TYP& t, const ATT& attribute)
	{
		m_Separate();
		TOne* P = m_hpTop->GetPointer(index);
		if ( P == NULL )
		{
			return false;
		}
		P->value = t;
		P->attribute = attribute;
		return true;
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@param attribute �����l�B
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	bool SetEx(INDEX index, const TYP& t, const ATT& attribute)
	{
		EXCLUSIVE(this);
		return Set(index, t, attribute);
	}

	/**
	 * [�擾] �v�f�̑����l�A�擾
	 *	@param index �C���f�b�N�X
	 *	@return �����l
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const ATT& GetAttribute(INDEX index) const
	{
		EXCLUSIVE(this);
		TOne* P = m_hpTop->GetPointer(index);
		if ( P != NULL )
		{
			return P->attribute;
		}
		ASSERT1(false, "CListT::GetAttribute", "�͈͊O��Index(%d)���w�肳��܂����B", index);
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [�ǉ�] �v�f��}��.
	 *		�w��̃C���f�b�N�X�ɗv�f��ǉ����܂��B
	 *	@param index �C���f�b�N�X
	 *	@param t �v�f
	 *	@param attribute �����l�B
	 *	@retval true	�}������
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	bool Insert(INDEX index, const TYP& t, const ATT& attribute = ATT())
	{
		EXCLUSIVE(this);
		m_Separate();
		if ( index == GetSize() )
		{
			return Add(t) != INVALID_INDEX;
		}
		m_hpTop->m_syncObj.Lock();
		TOne* P = m_hpTop->GetPointer(index - 1);
		if ( P == NULL )
		{
			m_hpTop->m_syncObj.Unlock();
			return false;
		}
		TOne* pNew = new TOne(t);
		pNew->attribute = attribute;
		pNew->pNextOne = P->pNextOne;
		P->pNextOne = pNew;
		m_hpTop->size++;
		m_hpTop->m_syncObj.Unlock();
		return true;
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
		if ( size == INVALID_SIZE )
		{
			return false;
		}
		EXCLUSIVE(this);
		m_hpTop->m_syncObj.Lock();
		if ( size < m_hpTop->size )
		{
			//����菬����
			m_Separate();
			TOne* P = m_hpTop->GetPointer(size - 1);
			P->Delete();
			m_hpTop->size = size;
			m_hpTop->m_syncObj.Unlock();
		}
		else if ( size > m_hpTop->size )
		{
			//�����傫��
			INT_PTR l = size - m_hpTop->size;
			m_Separate();
			m_hpTop->m_syncObj.Unlock();
			while ( l-- )
			{
				Add(TYP());
			}
		}
		return true;
	}

	/**
	 * [�폜] �Ԉ���.
	 *		�w��̏����ɍ���Ȃ�����(�`�F�b�J�[�̌��ʂɂ��)���폜���܂��B
	 *	@param	checker {@link TNB::IConstCollectionT::IChecker �`�F�b�J�[}
	 *	@param	boIsReverse	�ȗ�����Ə����Ɉ�v���c���܂��B
	 *						true���w�肷��Ə����s��v���c���܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�폜�������B
	 */
	virtual size_t Cull(const IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		size_t i = 0;
		while ( i < GetSize() )
		{
			if ( ! checker.IsValid(At(i)) ^ boIsReverse )
			{
				if ( ! Remove(i) ){ return INVALID_SIZE; }
				r++;
			}
			else
			{
				i++;
			}
		}
		return r;
	}//memo;List�͓�����폜���Ă������ق��������̂��߁A���ʂɎ����B

	#ifndef _WIN32_WCE
	/**
	 * [�폜] �Ԉ���.
	 *		�w��̏����ɍ���Ȃ�������������(�`�F�b�J�[�̌��ʂɂ��)���폜���܂��B
	 *	@param	checker {@link TNB::IConstCollectionT::IChecker �`�F�b�J�[}
	 *	@param	boIsReverse	�ȗ�����Ə����Ɉ�v���c���܂��B
	 *						true���w�肷��Ə����s��v���c���܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�폜�������B
	 */
	size_t CullOnAttribute(typename const IConstCollectionT<ATT>::IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		if ( ! IsEmpty() )
		{
			for ( INDEX i = GetSize() - 1; i >= 0; i-- )
			{
				if ( ! checker.IsValid(GetAttribute(i)) ^ boIsReverse )
				{
					if ( ! Remove(i) )
					{
						return INVALID_SIZE;
					}
					r++;
				}
			}
		}
		return r;
	}
	#endif // _WIN32_WCE

	/**
	 * [����] �V���A���C�Y
	 *	@attention List�R���N�V�����̃V���A���C�Y�f�[�^�́A�K���AList�R���N�V�����Ńf�V���A���C�Y���Ă��������B<BR>
	 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		size_t l = GetSize();
		_sr << l << l;
		loop ( i, l )
		{
			_sr << At(i);
			_sr << GetAttribute(i);
		}
	}

	/**
	 * [����] �f�V���A���C�Y
	 *	@attention List�R���N�V�����̃V���A���C�Y�f�[�^�́A�K���AList�R���N�V�����Ńf�V���A���C�Y���Ă��������B
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		size_t l1, l2;
		ds >> l1 >> l2;
		if ( l1 != l2 )
		{
			throw CInvalidParamException();
		}
		RemoveAll();
		TYP t;
		ATT attribute;
		loop ( i, l1 )
		{
			ds >> t;
			ds >> attribute;
			Add(t, attribute);
		}
	}
};



/**@ingroup COLLECT
 * BYTE�z��Ǘ��N���X
 *	@par�K�v�t�@�C��
 *			TnbList.h
 */
typedef TNB::CListT< BYTE > CByteList;




}; // TNB
