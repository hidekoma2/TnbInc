#pragma once
/**
 *@file
 * ���Q�Ǘ��֌W�̃w�b�_
 *
 *		���Q�Ǘ����邽�߂̃e���v���[�g�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbComparator.h"
#include "TnbStr.h"
#include "TnbSync.h"
#include "TnbException.h"
#include "TnbSerializer.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

// Input Iterator �x�[�X�e���v���[�g
template<typename COL, typename TYP> 
class CConstIteratorBaseT
{
	const COL *	m_V;
	mutable INDEX	m_ind;
public:
	CConstIteratorBaseT(const COL* V = NULL, INDEX i = 0): m_V(V), m_ind(i) {}
	INDEX GetIndex(void) const { return m_ind; }
	TYP operator*(void) const { return m_V->Get(m_ind); }
	const TYP* operator->(void) const { return &(m_V->At(m_ind)); }
	bool operator==(const CConstIteratorBaseT& I) const
	{
		return m_V == I.m_V && m_ind == I.m_ind;
	}
	bool operator!=(const CConstIteratorBaseT& I) const
	{
		return m_V != I.m_V || m_ind != I.m_ind;
	}
	CConstIteratorBaseT& operator++(void) const
	{
		m_ind++;
		return *this;
	}
	CConstIteratorBaseT operator++(int) const
	{
		CConstIteratorBaseT I = *this;
		m_ind++;
		return I;
	}
	const TYP& operator[](INDEX i) const{ return m_V->At(m_ind+i);	}
	CConstIteratorBaseT& operator+=(INT_PTR i) {m_ind+=i; return *this;}
	CConstIteratorBaseT& operator-=(INT_PTR i) {m_ind-=i; return *this;}
};

// Randam Access Iterator �x�[�X�}�N��
#define _ITERATORCORE(NAME, COLL)										\
		COLL*	m_V;													\
		mutable INDEX m_ind;											\
	public:																\
		NAME(COLL* V = NULL, INDEX i = 0) : m_V(V), m_ind(i) {}			\
		INDEX GetIndex(void) const { return m_ind; }					\
		bool operator==(const NAME& I) const {return m_ind==I.m_ind;}	\
		bool operator!=(const NAME& I) const {return m_ind!=I.m_ind;}	\
		bool operator<=(const NAME& I) const {return m_ind<=I.m_ind;}	\
		bool operator<(const NAME& I) const	{return m_ind<I.m_ind;}		\
		bool operator>=(const NAME& I) const {return m_ind>=I.m_ind;}	\
		bool operator>(const NAME& I) const	{return m_ind>I.m_ind;}		\
		NAME& operator+=(INT_PTR i)		{m_ind+=i; return *this;}		\
		NAME& operator-=(INT_PTR i)		{m_ind-=i; return *this;}		\
		NAME operator+(INT_PTR i) const {NAME I=*this; return I+=i;}	\
		NAME operator-(INT_PTR i) const {NAME I=*this; return I-=i;}	\
		INT_PTR operator-(const NAME& I) const {return m_ind-I.m_ind; }	\
		NAME& operator++(void)		{m_ind++; return *this;}			\
		NAME& operator--(void)		{m_ind--; return *this;}			\
		NAME operator++(int)		{NAME I=*this; m_ind++;	return I;}	\
		NAME operator--(int)		{NAME I=*this; m_ind--;	return I;}


// Randam Access Iterator �x�[�X�e���v���[�g
template<typename COL, typename TYP> 
class CIteratorBaseT
{
	_ITERATORCORE(CIteratorBaseT, COL)
public:
	const TYP& operator*(void) const 	{ return m_V->At(m_ind);	}
	const TYP* operator->(void) const 	{ return &(m_V->At(m_ind));	}
	const TYP& operator[](INDEX i) const{ return m_V->At(m_ind+i);	}
	TYP& operator*(void)				{ return m_V->Ref(m_ind);	}
	TYP* operator->(void)				{ return &(m_V->Ref(m_ind));}
	TYP& operator[](INDEX i)			{ return m_V->Ref(m_ind+i);	}
};

#endif //_TnbDOXYGEN



/**@ingroup COLLECT ITERATOR
 * ���Q�Ǘ��C���^�[�t�F�[�X�e���v���[�g
 *
 *		�ǂݍ��ݐ�p�̕����̗v�f(TYP)���Ǘ�����C���^�[�t�F�[�X�ł��B
 *
 *		���̃C���^�[�t�F�[�X�����N���X�͈ȉ��̓����������܂��B
 *
 *		 �@<BR>
 *		 �� Lock() / Unlock() �̔r�����\�b�h�A�y�сA�X���b�h�Z�[�u<BR>
 *			@par
 *					Lock() / Unlock() �͏��̔r���̂��߂Ɏg�p���܂��B
 *					Get() , At() , STL�݊����\�b�h(�������Ŏ���)�ȊO�̃��\�b�h��
 *					Lock() ���\�b�h���g��Ȃ��Ă��A�X���b�h�Z�[�u�ɂȂ��Ă��܂����A
 *					���\�b�h�ƃ��\�b�h�̊ԂŃI�u�W�F�N�g�̏�Ԃ��ς��̂�����邽�߂ɁA
 *					��A�̏������s�Ȃ��ꍇ�A Lock() ���ׂ��ł��B\n
 *					�����̃X���b�h����A�N�Z�X�����\�����Ȃ��ꍇ�́A�s�v�ł��B
 *
 *		 �� Get() / GetEx() �ɂ��v�f�̒l���擾<BR>
 *		 �� At() �ɂ��v�f�̎Q�Ǝ擾<BR>
 *			@par
 *					�w��̌^�̗v�f�̎擾���s���܂��B
 *					�C���f�b�N�X�i���єԍ��j�ŃA�N�Z�X���܂��B
 *					<BR>�� At() �̓T�u�N���X�ɂ���Ă͕������ꂽ�v�f�̎Q�Ƃ�Ԃ����̂�����܂��B
 *
 *		 �� Find() / FindMax() / FindMin() �ɂ��w��v�f�A�ő�A�ŏ��v�f�̌���<BR>
 *			@par
 *					�ȉ��̕��@�̓��A�����ꂩ�ɂ���Ď����ł��܂��B
 *					<ul type=circle>
 *					<li> IComparatorT �����������R���p���[�^�[���쐬���A�����ɂĈϏ�����B
 *					<li> IChecker �����������`�F�b�J�[���쐬���A�����ɂĈϏ�����B
 *					<li> �����ȗ����A�ėp�������g�p����i IComparableT �Q�Ɓj�B
 *					</ul>
 *			@par
 *			\code
 *			CVectorT<CFoo> vDat;
 *			     ;         //�f�[�^���
 *			struct IMyComp : IComparatorT<CFoo>
 *			{
 *			  INT_PTR CompareTo( const CFoo &T1, const CFoo &T2 )
 *			  {
 *			    return T1.Comp(T2);
 *			  }
 *			} comp;
 *			CFoo TT;
 *			INDEX index = vDat.FindMax(&comp, TT);  //��Ԃł����v�f�̃C���f�b�N�X���Ԃ�B			
 *			\endcode
 *
 *		 �� Compare() �ɂ��S�v�f�̔�r<BR>
 *			@par
 *					�e�v�f�� IComparatorT::GetDefault() �œ�����R���p���[�^���g�p����
 *					��r���܂��B
 *
 *		 �� ISerializer �����N���X�ɂ��A�V���A���C�Y<BR>
 *			@par
 *					ISerializer ���g���A�����V���A���C�Y���邱�Ƃ��\�ł��B
 *					�V���A���C�Y�����f�[�^�́A�����v�f�^�̑��̃R���N�V������
 *					�f�V���A���C�Y���邱�Ƃ��\�ł��B
 *					<BR>���v�f�ɂ́A�V���A���C�Y�\�Ȍ^��K�p���Ă��������B
 *
 *		 �� Standard Template Library�Ƃ̐e�a��<BR>
 *		 �� begin() / end() �ɂ��{@link ITERATOR �����q}<BR>
 *			@par
 *					���\�b�h�����������Ŏn�܂���̂́A���ׂ� STL �Ƃ̐e�a���̂��߂ɗp�ӂ���Ă��܂��B
 *					�����q�̓C���X�^���X�̎Q�Ƃ������Ă��邾���Ȃ̂ŁA�����q���L���̊ԁA
 *					�C���X�^���X�͏����Ȃ��悤�ɂ��Ă��������B
 *					�܂��A�����̃��\�b�h�A�����q�́A�X���b�h�Z�[�u�ɂȂ�܂���̂ŁA
 *					�K�v�ɉ����� Lock() ���ׂ��ł��B
 *
 *	@note �{�C���^�[�t�F�[�X����������ꍇ�A�Œ���ȉ��̃��\�b�h��p�ӂ���K�v���L��܂��B<BR>
 *	@par
 *			<ol>
 *			<li> At()
 *			<li> GetSize()
 *			</ol>
 *
 *	@see	ISynchronized ���������Ă��܂��B
 *	@see	IComparableT ���������Ă��܂��B
 *	@see	ISerializable ���������Ă��܂��B������ �f�V���A���C�Y�̓T�|�[�g���Ă܂���B
 *	
 *	@throw CTnbException �ǂݍ��ݏ������ɃG���[���������ꍇ�A CTnbException �̃T�u�N���X���X���[����\�����L��܂��B
 *
 *	@param TYP �v�f�̌^�B�^�̓R�s�[�@�\���K�{�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollection.h
 *
 *	@date 06/05/15 ICollectionT������ɒ��ۉ����č쐬�B
 *	@date 06/05/17 Find�֌W���\�b�h�A�ǉ��B
 *	@date 06/05/20 �R���p���[�^�[�����B
 *	@date 06/05/23 Offset�A�_�v�^�V�K�ɔ���Find�֌W���\�b�h�̈��������B�`�F�b�J�[IF�ǉ��B
 *	@date 06/06/29 �����R���p���[�^���P�B
 *	@date 06/07/04 �R���p���[�^IF�𕪗��B
 *	@date 06/08/01 GetConstPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/11 Element() �� At() �ɉ����B
 *	@date 06/08/13 �C�e���[�^�� STL �ɂ��킹�āA���C�B
 *	@date 06/08/30 �V���A���C�Y�@�\�A�ǉ��B
 *	@date 06/09/13 IsEqual() �ǉ��B
 *	@date 06/10/26 GetEx() ��ǉ��B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 09/09/10 operator size_t �ǉ��B
 *	@date 11/06/01 �R�s�[�I�y���[�^������
 *	@date 11/07/11 �z�񌟍��� Find() ��ǉ��B
 *	@date 11/08/24 GetElements() �ɑ�O�����A�ǉ��B
 *	@date 19/07/04 FindMin() FindMax() �����ρB
 */
template<typename TYP>
struct IConstCollectionT : ISynchronized, ISerializable, IComparableT<TNB::IConstCollectionT<TYP> >
{
	/**@ingroup COLLECT
	 * ���Q�Ǘ��C���^�[�t�F�[�X�̃`�F�b�J�[�C���^�[�t�F�[�X.
	 *		�L�������� IsValid() �Ɏ������܂��B 
	 */
	struct IChecker
	{
		virtual ~IChecker(void) {}	///< �f�X�g���N�^
		/**
		 * [�m�F] �`�F�b�N
		 *	@param T �`�F�b�N�Ώ�
		 *	@retval true T �͗L��
		 *	@retval false T �͖���
		 */
		virtual bool IsValid(const TYP& T) const = 0;
	};


	//---------------------------------


	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// const_iterator�^
	typedef CConstIteratorBaseT<IConstCollectionT<TYP>, TYP> const_iterator;
	// value_type�錾
	typedef TYP value_type;
	#endif //_TnbDOXYEM

	/**
	 * [����] �擪const_iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return const_iterator(this, 0); }

	/**
	 * [����] �Ō�const_iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return const_iterator(this, GetSize()); }


	//---------------------------------


	/// �R���X�g���N�^
	IConstCollectionT(void) 
	{
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@return �v�f��
	 */
	operator size_t(void) const 
	{
		return GetSize();
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@note Pure�֐��ł��B
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const = 0;

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̎Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const = 0;

	/**
	 * [�擾] �v�f�̎擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A GetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP Get(INDEX index) const
	{
		return At(index);
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP GetEx(INDEX index) const
	{
		EXCLUSIVE(this);
		return At(index);
	}

	/**
	 * [�擾] �����v�f���o��.
	 *	@param[in] size ���o���f�[�^�̐��B
	 *	@param[out] _P �R�s�[����|�C���^�Bsize ���̗̈悪����K�v������܂��B
	 *	@param[in] offset ���o���J�n�̃C���f�b�N�X�B�ȗ�����ƂO�B
	 *	@return �R�s�[������
	 */
	virtual size_t GetElements(size_t size, TYP* _P, INDEX offset = 0) const
	{
		EXCLUSIVE(this);
		if ( offset >= GetSize() )
		{
			return 0;
		}
		if ( size + offset > GetSize() )
		{
			size = GetSize() - offset;
		}
		for ( INDEX i = 0; i < size; i++ )
		{
			*_P++ = At(i + offset);
		}
		return size;
	}

	/**
	 * [�m�F] �v�f�̗L���m�F.
	 *	@note GetSize()�̖߂�l���O���ۂ��Ń`�F�b�N���Ă��܂��B
	 *	@retval true	�v�f�Ȃ�
	 *	@retval false	�v�f�L��
	 */
	bool IsEmpty(void) const
	{
		return GetSize() == 0;
	}

	/**
	 * [�m�F] INDEX�̗L���m�F.
	 *	@note GetSize()�̖߂�l���g�p���ă`�F�b�N���Ă��܂��B
	 *	@param index �C���f�b�N�X
	 *	@retval true	�w��Index�͗L���B
	 *	@retval false	�w��Index�͖���(�͈͊O)�B
	 */
	bool IsInRange(INDEX index) const
	{
		return TNB::IsInRange(index, GetSize());
	}

	/**
	 * [����] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B 
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		EXCLUSIVE(this);
		size_t l = GetSize();
		_sr << l << l;
		loop ( i, l )
		{
			_sr << At(i);
		}
	}

	/**
	 * [����] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B
	 *	@throw CNotSupportException �T�|�[�g���Ă��Ȃ����Ƃ��������߂ɃX���[����܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		throw CNotSupportException();
	}

	/**
	 * [����] ������v�v�f�̌���.
	 *		�`�F�b�J�[( IChecker �̃T�u�N���X)���g�p���āA�������܂��B
	 *	@note �͈͂����肵�����ꍇ�A CConstOffsetAdapterT ���g�p���܂��B
	 *	@param	checker �`�F�b�J�[
	 *	@param	startIndex �����J�n�C���f�b�N�X�B
	 *	@param	boIsReverse	�ȗ�����Ə����Ɉ�v��T���܂��B
	 *						true���w�肷��Ə����s��v��T���܂��B
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		���������C���f�b�N�X
	 */
	INDEX Find(const IChecker& checker, INDEX startIndex = 0, bool boIsReverse = false) const
	{
		EXCLUSIVE(this);
		if ( IsInRange(startIndex) )
		{
			size_t size = GetSize();
			for ( size_t i = startIndex; i < size; i++ )
			{
				if ( checker.IsValid(At(i)) ^ boIsReverse )
				{
					return i;
				}
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [����] �w��z��̌���.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA�������܂��B
	 *	@note �͈͂����肵�����ꍇ�A CConstOffsetAdapterT ���g�p���܂��B
	 *	@param	t �z��
	 *	@param	comparator	�R���p���[�^�[
	 *	@param	startIndex �����J�n�C���f�b�N�X�B
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		���������C���f�b�N�X
	 */
	INDEX Find(const IConstCollectionT<TYP>& t, const IComparatorT<TYP>& comparator, INDEX startIndex = 0) const
	{
		EXCLUSIVE(this);
		if ( IsInRange(startIndex) )
		{
			size_t size = GetSize();
			for ( size_t i = startIndex; i < size; i++ )
			{
				size_t tsz = t.GetSize();
				if ( i + tsz > size || tsz == 0 )
				{
					break;
				}
				bool isFind = true;
				loop ( j, tsz )
				{
					if ( ! comparator.IsEqualTo(At(i + j), t.At(j)) )
					{
						isFind = false;
						break;
					}
				}
				if ( isFind )
				{
					return i;
				}
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [����] �w��z��̌���.
	 *		IComparatorT::GetDefault() �œ�����R���p���[�^���g�p���Č������܂��B
	 *	@note �͈͂����肵�����ꍇ�A CConstOffsetAdapterT ���g�p���܂��B
	 *	@param	t �z��
	 *	@param	startIndex �����J�n�C���f�b�N�X�B
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		���������C���f�b�N�X
	 */
	INDEX Find(const IConstCollectionT<TYP>& t, INDEX startIndex = 0) const
	{
		return Find(t, IComparatorT<TYP>::GetDefault(), startIndex);
	}

	/**
	 * [����] �w��v�f�̌���.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA�������܂��B
	 *	@note �͈͂����肵�����ꍇ�A CConstOffsetAdapterT ���g�p���܂��B
	 *	@param	t �v�f
	 *	@param	comparator	�R���p���[�^�[
	 *	@param	startIndex �����J�n�C���f�b�N�X�B
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		���������C���f�b�N�X
	 */
	INDEX Find(const TYP& t, const IComparatorT<TYP>& comparator, INDEX startIndex = 0) const
	{
		EXCLUSIVE(this);
		if ( IsInRange(startIndex) )
		{
			size_t size = GetSize();
			for ( size_t i = startIndex; i < size; i++ )
			{
				if ( comparator.IsEqualTo(At(i), t) )
				{
					return i;
				}
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [����] �w��v�f�̌���.
	 *		IComparatorT::GetDefault() �œ�����R���p���[�^���g�p���Č������܂��B
	 *	@note �͈͂����肵�����ꍇ�A CConstOffsetAdapterT ���g�p���܂��B
	 *	@param	t �v�f
	 *	@param	startIndex �����J�n�C���f�b�N�X�B
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		���������C���f�b�N�X
	 */
	INDEX Find(const TYP& t, INDEX startIndex = 0) const
	{
		return Find(t, IComparatorT<TYP>::GetDefault(), startIndex);
	}

	/**
	 * [����] �v�f�̍ő匟��.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA��r�������܂��B
	 *	@param comparator	�R���p���[�^�[
	 *	@param def �ŏ��l(�����l) 
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		�ő�����C���f�b�N�X
	 */
	INDEX FindMax(const IComparatorT<TYP>& comparator, const TYP& def) const
	{
		EXCLUSIVE(this);
		TYP t = def;
		INDEX r = INVALID_INDEX;
		loop_dn ( i, GetSize() )
		{
			if ( comparator.CompareTo(At(i), t) >= 0 )
			{
				t = Get(i);
				r = i;
			}
		}
		return r;
	}

	/**
	 * [����] �v�f�̍ő匟��.
	 *		IComparatorT::GetDefault() �œ�����R���p���[�^���g�p���Ĕ�r�������܂��B
	 *	@note �͈͂����肵�����ꍇ�A CConstOffsetAdapterT ���g�p���܂��B
	 *	@param def �ŏ��l(�����l) 
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		�ő�����C���f�b�N�X
	 */
	INDEX FindMax(const TYP& def) const
	{
		return FindMax(IComparatorT<TYP>::GetDefault(), def);
	}

	/**
	 * [����] �v�f�̍ŏ�����.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA��r�������܂��B
	 *	@param	comparator	�R���p���[�^�[
	 *	@param def �ő�l(�����l) 
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		�ŏ������C���f�b�N�X
	 */
	INDEX FindMin(const IComparatorT<TYP>& comparator, const TYP& def) const
	{
		EXCLUSIVE(this);
		TYP t = def;
		INDEX r = INVALID_INDEX;
		loop_dn ( i, GetSize() )
		{
			if ( comparator.CompareTo(At(i), t) <= 0 )
			{
				t = Get(i);
				r = i;
			}
		}
		return r;
	}

	/**
	 * [����] �v�f�̍ŏ�����.
	 *		IComparatorT::GetDefault() �œ�����R���p���[�^���g�p���Ĕ�r�������܂��B
	 *	@note �͈͂����肵�����ꍇ�A CConstOffsetAdapterT ���g�p���܂��B
	 *	@param def �ő�l(�����l) 
	 *	@retval INVALID_INDEX	������
	 *	@retval ����ȊO		�ŏ������C���f�b�N�X
	 */
	INDEX FindMin(const TYP& def) const
	{
		return FindMin(IComparatorT<TYP>::GetDefault(), def);
	}

	/**
	 * [�m�F] ��r.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA�S�v�f���r���܂��B
	 *	@param c ��r�Ώ�
	 *	@retval �O		������ c �Ɠ���
	 *	@retval �P�ȏ�	������ c ���傫��
	 *	@retval �O����	������ c ��菬����
	 */
	virtual INT_PTR Compare(const IConstCollectionT<TYP>& c) const
	{
		if ( this == &c )
		{
			return 0; 
		}
		EXCLUSIVE2(&c, this);
		const IComparatorT<TYP>& comparator = IComparatorT<TYP>::GetDefault();
		size_t mySize = GetSize();
		size_t youSize = c.GetSize();
		size_t size = (mySize < youSize) ? mySize : youSize;
		loop ( i, size )
		{
			INT_PTR r = comparator.CompareTo(At(i), c.At(i));
			if ( r != 0 )
			{
				return r;
			}
		}
		return mySize - youSize;
	}

	/**
	 * [�m�F] ��r.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA�S�v�f���r���܂��B
	 *	@param c ��r�Ώ�
	 *	@retval true	������ c �Ɠ���
	 *	@retval false	������ c �ƈقȂ�
	 */
	virtual bool IsEqual(const IConstCollectionT<TYP>& c) const
	{
		ASSERT( IComparatorT<TYP>::GetDefault().IsEqualTo(TYP(), TYP()) );
		if ( this == &c )
		{ 
			return true; 
		}
		EXCLUSIVE2(&c, this);
		size_t size = GetSize();
		if ( size != c.GetSize() )
		{
			return false; 
		}
		const IComparatorT<TYP>& comparator = IComparatorT<TYP>::GetDefault();
		loop ( i, size )
		{
			if ( ! comparator.IsEqualTo(At(i), c.At(i)) )
			{
				return false;
			}
		}
		return true;
	}

private:
	/// �R�s�[�I�y���[�^ (�g�p�֎~)
	void operator=(const IConstCollectionT& other);
};



/**@ingroup COLLECT ITERATOR
 * ���Q�Ǘ�����C���^�[�t�F�[�X�e���v���[�g
 *
 *		�����̗v�f�^(TYP)���Ǘ��A���삷��C���^�[�t�F�[�X�ł��B
 *
 *		���̃C���^�[�t�F�[�X�����N���X�͈ȉ��̓����������܂��B
 *
 *		 �@<BR>
 *		 �� Lock() / Unlock() �̔r�����\�b�h�A�y�сA�X���b�h�Z�[�u<BR>
 *			@par
 *					Lock() / Unlock() �͏��̔r���̂��߂Ɏg�p���܂��B
 *					Get(), At(), Set(), Add(), Remove(), STL�݊����\�b�h(�������Ŏ���)�ȊO�̃��\�b�h��
 *					Lock() ���\�b�h���g��Ȃ��Ă��A�X���b�h�Z�[�u�ɂȂ��Ă��܂����A
 *					���\�b�h�ƃ��\�b�h�̊ԂŃI�u�W�F�N�g�̏�Ԃ��ς��̂�����邽�߂ɁA
 *					��A�̏������s�Ȃ��ꍇ�A Lock() ���ׂ��ł��B\n
 *					�����̃X���b�h����A�N�Z�X�����\�����Ȃ��ꍇ�́A�s�v�ł��B
 *			@par
 *			\code
 *		
 *			CVectorT<long> vlDat;
 *			     ;
 *			vlDat.Lock();	//Lock�`Unlock�ԁA���̃X���b�h����vlDat�̑��삪�o���Ȃ��Ȃ�B		
 *			CVectorT<long>::iterator it = vlDat.begin();		
 *			*it = 1234;
 *			     ;
 *			vlDat.Unlock();	
 *			\endcode
 *
 *		 �� Get() / GetEx() �ɂ��v�f�̒l���擾<BR>
 *		 �� At() / Ref() �ɂ��v�f�̎Q�Ǝ擾<BR>
 *		 �� Add() / AddEx() �ɂ��v�f�̒ǉ�<BR>
 *		 �� Insert() / InsertEx() �ɂ��v�f�̑}��<BR>
 *		 �� Remove() / RemoveEx() �ɂ��v�f�̍폜<BR>
 *		 �� Set() / SetEx() �ɂ��v�f�̒l��ݒ�<BR>
 *		 �� Swap() �ɂ��v�f�̒l�̓���ւ�<BR>
 *		 �� RemoveAll() �ɂ��v�f�̑S�폜<BR>
 *			@par
 *					�w��̌^�̗v�f��ǉ��A�폜�A�ݒ�A�擾���s���܂��B
 *					��{�I�ɁA�C���f�b�N�X�i���єԍ��j�ŃA�N�Z�X���܂��B
 *					<BR>�� At() �̓T�u�N���X�ɂ���Ă͕������ꂽ�v�f�̎Q�Ƃ�Ԃ����̂�����܂��B
 *			@par
 *			\code
 *		
 *			CVectorT<long> vlDat;
 *			vlDat.Add( 12 );            // �v�f�ǉ�
 *			vlDat.Add( 34 );            // �v�f�ǉ�
 *			//���̎��_�� vlDat��{ 12,34 }�Ƃ�����������
 *			long l1 = vlDat.Get(0);     // �C���f�b�N�X�O�i�ŏ��j�̗v�f�擾
 *			ASSERT( l1==12 );
 *			vlDat.Set(1,56);            // �C���f�b�N�X�P�̗v�f�̒l��ݒ�
 *			vlDat.Set(0,78);            // �C���f�b�N�X�O�̗v�f�̒l��ݒ�
 *			//���̎��_�� vlDat��{ 78,56 }�Ƃ�����������
 *			vlDat.Remove(0);            // �C���f�b�N�X�O�̗v�f�폜
 *			//���̎��_�� vlDat��{ 56 }�Ƃ�����������
 *			vlDat.RemoveAll();          // �v�f�S�폜
 *			\endcode
 *
 *		 �� AddElements() �ɂ��v�f�̒ǉ�<BR>
 *		 �� SetElements() �ɂ��v�f�̃R�s�[<BR>
 *			@par
 *					�w������A�܂Ƃ߂Ēǉ� / �R�s�[���܂��B
 *					�K�v�Ȃ��̂�����ǉ�/�R�s�[�������ꍇ�A CConstAdapterT �� CIndexAdapterT ���g�p���܂��B
 * 
 *		 �� Append() �ɂ��v�f�̒ǉ�<BR>
 *		 �� Copy() �ɂ��v�f�̃R�s�[<BR>
 *			@par
 *					�N���X���قȂ��Ă��Ă��A���� IConstCollectionT �� TYP �������ł���΁A
 *					�ǉ�/�R�s�[���邱�Ƃ��o���܂��B
 *					�K�v�Ȃ��̂�����ǉ�/�R�s�[�������ꍇ�A CIndexAdapterT ���g�p���܂��B
 *
 *		 �� Sort() �ɂ��v�f�̃\�[�g<BR>
 *			@par
 *					�ȉ��̕��@�̓��A�����ꂩ�ɂ���Ď����ł��܂��B
 *					<ul type=circle>
 *					<li> IComparatorT �����������R���p���[�^�[���쐬���A�����ɂĈϏ�����B
 *					<li> �����ȗ����A�ėp�������g�p����i IComparableT �Q�Ɓj�B
 *					</ul>
 *			@par
 *			\code
 *			CVectorT<CFoo> vDat;
 *			     ;         //�f�[�^���
 *			struct IMyComp : IComparatorT<CFoo>
 *			{
 *			  INT_PTR CompareTo(const CFoo &T1, const CFoo &T2)
 *			  {
 *			    return T1.Comp(T2);
 *			  }
 *			} comp;
 *			vDat.Sort( &comp );  //���������Ƀ\�[�g�����			
 *			\endcode
 *
 *			@par
 *					CIndexAdapterT ���g���ƁAIndex�e�[�u���ɂ��\�[�g���\�ł�
 *					�i�Ώۂ̃R���N�V�����̕ω����Ȃ����߁A�����ł�����j�B
 *
 *		 �� ISerializer �����N���X�ɂ��A�V���A���C�Y<BR>
 *		 �� IDeserializer �����N���X�ɂ��A�f�V���A���C�Y<BR>
 *			@par
 *					ISerializer ���g���A�����V���A���C�Y���邱�Ƃ��\�ł��B<BR>
 *					IDeserializer ���g���A�V���A���C�Y���ꂽ�����f�V���A���C�Y���邱�Ƃ��\�ł��B
 *					���̎��A�����v�f�^�ł���ΈقȂ�R���N�V�����ł��f�V���A���C�Y�\�ł�(�ꕔ�̃R���N�V����������)�B
 *					<BR>���v�f�ɂ́A�V���A���C�Y�\�Ȍ^��K�p���Ă��������B
 *
 *		 ������R���X�g���N�^�A����I�y���[�^�̎���(�ꕔ������)<BR>
 *			@par
 *					�����^�̃C���X�^���X���m�͑�����\�ł��B
 *					���̎��A���ӂ̃C���X�^���X�́A����j�����A
 *					�E�ӂ̃C���X�^���X�̏������L����悤�ɂȂ�܂�
 *					�i���̂��ߑ����Ƃ�<b>����</b>�ł��j�B
 *					���̌�A�����ꂩ�̃C���X�^���X�𑀍삷��ƁA
 *					���𕡐�����̂ŁA���̃C���X�^���X�ɉe�����o�܂���
 *					�i�������쐬���鎞�A���Ԃ�������\�����L��܂��j�B
 *
 *		 �� ���A IConstCollectionT �̓����������p���ł��܂��B<BR>
 *
 *	@note �{�C���^�[�t�F�[�X����������ꍇ�A�Œ���ȉ��̃��\�b�h��p�ӂ���K�v���L��܂��B<BR>
 *	@par
 *			<ol>
 *			<li> GetSize()
 *			<li> At()
 *			<li> Ref()
 *			<li> Set()
 *			<li> Add()
 *			<li> Remove()
 *			</ol>
 *			      �� ICollectionMidT ���g���ƁA At() , Ref() �� Set() �͎����Ŏ�������܂��B
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *
 *	@throw CTnbException �ǂݍ��ݏ������ɃG���[���������ꍇ�A CTnbException �̃T�u�N���X���X���[����\�����L��܂��B
 *
 *	@param TYP �v�f�̌^�B�^�̓R�s�[�@�\���K�{�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollection.h
 *
 *	@date 06/04/19 ���ۉ�
 *	@date 06/04/28 Clone�����𒊏ۉ����Ă���IF�Ɏ����B
 *	@date 06/05/22 Swap���\�b�h�ǉ��B
 *	@date 06/05/23 AddElements�ACopy�n���\�b�h�ǉ��B
 *	@date 06/06/01 ���[�e�[�g�A�V���b�t�����\�b�h�ǉ��B
 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/11 Element() �� At() �ɉ����BReferElementPtr() ��p�~�B
 *	@date 06/08/13 �C�e���[�^�� STL �ɂ��킹�āA���C�B
 *	@date 06/08/24 �����t�폜 RemoveOnCheck() ��ǉ��B
 *	@date 06/08/30 �V���A���C�Y�@�\�A�ǉ��B
 *	@date 06/09/13 Copy�n���\�b�h�����ǁB
 *	@date 06/10/26 At() ��p�~�BSetEx(), AddtEx(), RemoveEx() ��ǉ��B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 07/06/19 CIndexAdapterT::Cull() ���쐬�������߁A �{�C���^�[�t�F�[�X�� �`OnCheck() ���\�b�h��p�~�B 
 *					RemoveOnCheck() �� Cull() �ɉ����B
 *	@date 07/09/18 TrimBottom() ��ǉ��B
 *	@date 09/09/07 CopyElements() -> SetElements() �ɉ����B
 *	@date 11/03/08 Insert() ��ǉ��B
 *	@date 11/06/01 �R�s�[�I�y���[�^������
 */
template<typename TYP>
struct ICollectionT : IConstCollectionT<TYP>
{
	/**@ingroup COLLECT
	 * ���Q�Ǘ�����C���^�[�t�F�[�X�̃\�[�g�����N���X
	 *
	 *		ICollectionT �����N���X�̗v�f���\�[�g���邱�Ƃ��ł��܂��B
	 *		�\�[�g������ IComparator �C���^�[�t�F�[�X���������Ċ֐��ɓn�����ƂŎ������܂��B
	 *
	 *	@note �ʏ�A���̃N���X���g�킸�AICollectionT �̃\�[�g���\�b�h���g�p���܂��B
	 *
	 *	@param TYP �R�s�[�������\�Ȍ^
	 *
	 *	@date 06/04/20 �V�K
	 */
	class CSort : CCopyImpossible
	{
		ICollectionT<TYP>*	m_piCollect;		///< �R���N�V�����̎Q��(qsort�Ŏg�p)
		const IComparatorT<TYP>*	m_piComparator;		///< �R���p���[�^�[�̎Q��(qsort�Ŏg�p)
		bool				m_boIsReverse;		///< ���o�[�X�t���O(qsort�Ŏg�p)
		/**
		 * QSORT�̃T�u���[�`��.
		 *	@note �ċN�Ăяo��������܂��B
		 *	@param left �J�n�C���f�b�N�X
		 *	@param right �I���C���f�b�N�X
		 */
		void m_qsort(size_t left, size_t right) 
		{
			if ( left >= right ){ return; }
			size_t j = left;
			for ( size_t i = left + 1; i <= right; i++ ) 
			{
				const TYP& c1 = m_piCollect->At(i);
				const TYP& c2 = m_piCollect->At(left);
				if ( (m_piComparator->CompareTo(c1, c2) < 0) ^ m_boIsReverse )
				{
					m_piCollect->Swap(i, ++j);
				}
			}
			m_piCollect->Swap(left, j);
			//
			if ( j > 0 )
			{
				m_qsort(left, j - 1);
			}
			m_qsort(j + 1, right);
		}

	public:
	
		/// �R���X�g���N�^
		CSort(void) {}

		/**
		 * [����] �N�C�b�N�\�[�g.
		 *		�R���p���[�^�[���������Ɣ��f�������Ƀ\�[�g���܂��B
		 *	@param[in,out]	_collect	�\�[�g�Ώ�
		 *	@param[in]		comp		�R���p���[�^�[
		 *	@param[in]		boIsReverse	true���w�肷��ƕ��т��t�ɂ��܂�
		 */
		void QuickSort(ICollectionT<TYP>& _collect, const IComparatorT<TYP>& comp, bool boIsReverse = false)
		{
			m_piCollect		= &_collect;
			m_piComparator	= &comp;
			m_boIsReverse	= boIsReverse;
			size_t l = _collect.GetSize();
			if ( l > 0 )
			{
				m_qsort(0, l - 1);
			}
		}

		/**
		 * [����] �o�u���\�[�g.
		 *		�R���p���[�^�[���������Ɣ��f�������Ƀ\�[�g���܂��B
		 *	@param[in,out]	_collect	�\�[�g�Ώ�
		 *	@param[in]		comp	�R���p���[�^�[
		 *	@param[in]		boIsReverse	true���w�肷��ƕ��т��t�ɂ��܂�
		 */
		void BubbleSort(ICollectionT<TYP>& _collect, const IComparatorT<TYP>& comp, bool boIsReverse = false)
		{
			size_t size = _collect.GetSize();
			loop ( i, size )
			{
				for ( size_t j = size - 1; j > i; j-- )
				{
					const TYP& c1 = _collect.At(j - 1);
					const TYP& c2 = _collect.At(j);
					if ( (comp.CompareTo(c1, c2) > 0) ^ boIsReverse )
					{
						_collect.Swap(j - 1, j);
					}
				}
			}
		}
	};


	//------------------------------------


	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// const_iterator�^�錾
	typedef CConstIteratorBaseT<ICollectionT<TYP>, TYP> const_iterator;
	// iterator�^�錾
	typedef CIteratorBaseT<ICollectionT<TYP>, TYP> iterator;
	#endif //_TnbDOXYEM

	/**
	 * [����] �擪const_iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return const_iterator(this, 0); }

	/**
	 * [����] �Ō�const_iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return const_iterator(this, GetSize()); }

	/**
	 * [����] �擪iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator begin(void) { return iterator(this, 0); }

	/**
	 * [����] �Ō�iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator end(void) { return iterator(this, GetSize()); }

	/**
	 * [����] �Ō�ɒǉ�
	 *	@param t �v�f
	 */
	void push_back(const TYP& t) { Add(t); }


	//------------------------------------

	
	/** 
	 * �R���X�g���N�^
	 */
	ICollectionT(void) 
	{
	}

	/** 
	 * �f�X�g���N�^
	 */
	virtual ~ICollectionT(void) 
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��
	 */
	ICollectionT& operator=(const ICollectionT& other)
	{
		Copy(other);
		return *this;
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
	virtual TYP& Ref(INDEX index) = 0;

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A SetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool Set(INDEX index, const TYP& t) = 0;

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	bool SetEx(INDEX index, const TYP& t)
	{
		EXCLUSIVE(this);
		return Set(index, t);
	}

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
	 * [�ǉ�] �v�f��ǉ�.
	 *	@param t �v�f�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX AddEx(const TYP& t)
	{
		EXCLUSIVE(this);
		return Add(t);
	}

	/**
	 * [�ǉ�] �v�f��}��.
	 *		�w��̃C���f�b�N�X�ɗv�f��ǉ����܂��B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A InsertEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@param t �v�f
	 *	@retval true	�}������
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	virtual bool Insert(INDEX index, const TYP& t)
	{
		size_t nowSize = GetSize();
		if ( index >= nowSize + 1 )
		{
			return false;
		}
		INDEX r = Add(TYP());
		if ( r == INVALID_INDEX )
		{
			return false;
		}
		loop_dn( i, nowSize - index )
		{
			Set(index + i + 1, At(index + i));
		}
		Set(index, t);
		return true;
	}

	/**
	 * [�ǉ�] �v�f��}��.
	 *		�w��̃C���f�b�N�X�ɗv�f��ǉ����܂��B
	 *	@param index �C���f�b�N�X
	 *	@param t �v�f
	 *	@retval true	�}������
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	virtual bool InsertEx(INDEX index, const TYP& t)
	{
		EXCLUSIVE(this);
		return Insert(index, t);
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval false	�G���[�B
	 *	@retval true	�����B
	 */
	virtual bool Remove(INDEX index) = 0;

	/**
	 * [�폜] �v�f��폜.
	 *	@param index �C���f�b�N�X
	 *	@retval false	�G���[�B
	 *	@retval true	�����B
	 */
	bool RemoveEx(INDEX index)
	{
		EXCLUSIVE(this);
		return Remove(index);
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
		size_t r = 0;
		if ( size == 0 )
		{
			size = INVALID_SIZE;
		}
		loop ( i, size )
		{
			if ( ! Remove(index) )
			{
				break;
			}
			r++;
		}
		return r;
	}

	/**
	 * [�폜] �S�v�f�폜 .
	 *	@note Remove(0)�𖳂��Ȃ�܂ŌĂяo���܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool RemoveAll(void)
	{
		EXCLUSIVE(this);
		while ( ! IsEmpty() )
		{
			if ( ! Remove(0) )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [�ݒ�] �v�f�̓���ւ�.
	 *	@param index1 �C���f�b�N�X�P
	 *	@param index2 �C���f�b�N�X�Q
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual void Swap(INDEX index1, INDEX index2)
	{
		if ( index1 != index2 )
		{
			EXCLUSIVE(this);
			TYP t = Get(index1);
			Set(index1, At(index2));
			Set(index2, t);
		}
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
		EXCLUSIVE(this);
		if ( P != NULL )
		{
			loop ( i, size )
			{
				if ( Add(*P++) == INVALID_INDEX )
				{
					return INVALID_SIZE; 
				}
			}
		}
		else
		{
			loop ( i, size )
			{
				if ( Add(TYP()) == INVALID_INDEX )
				{
					return INVALID_SIZE; 
				}
			}
		}
		return size;
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
		size_t r = 0;
		loop ( i, c.GetSize() )
		{
			if ( Add(c.At(i)) == INVALID_INDEX )
			{
				return INVALID_SIZE;
			}
			r++;
		}
		return r;
	}

	/**
	 * [�ݒ�] �R�s�[.
	 *		�Ǘ����Ă���o�b�t�@���J�����A�w��̃f�[�^���R�s�[���܂��B
	 *	@note �p�~�\��ł��B SetElements() ���g�p���Ă��������B
	 *	@param size �f�[�^���B
	 *	@param P �R�s�[������e���w��B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�����������B
	 */
	virtual size_t CopyElements(size_t size, const TYP* P = NULL)
	{
		return SetElements(size, P);
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
		if ( ! RemoveAll() )
		{
			return INVALID_SIZE; 
		}
		return AddElements(size, P);
	}

	/**
	 * [�ݒ�] �R�s�[.
	 *		�Ǘ����Ă���o�b�t�@���J�����A�w��̃f�[�^���R�s�[���܂��B
	 *	@note �R�s�[���͈̔͂����肵�����ꍇ�A���f�[�^�� CConstOffsetAdapterT ���g�p���܂��B
	 *	@note ��x�ʃ������ɃR�s�[���Ă��܂��B���̂��߁A�����I�u�W�F�N�g�ł��Ή��\�ł��B
	 *	@param c �R�s�[����f�[�^
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�����������B
	 */
	virtual size_t Copy(const IConstCollectionT<TYP>& c)
	{
		if ( this != &c )
		{
			EXCLUSIVE2(&c, this);
			size_t l = c.GetSize();
			if ( SetSize(l) )
			{
				loop ( i, l )
				{
					Set(i, c.At(i));
				}
			}
			return l;
		}
		return c.GetSize();
	}

	/**
	 * [�폜] �Ԉ���.
	 *		�w��̏����ɍ���Ȃ�����(�`�F�b�J�[�̌��ʂɂ��)���폜���܂��B
	 *	@param checker {@link TNB::IConstCollectionT::IChecker �`�F�b�J�[}�B
	 *	@param	boIsReverse	�ȗ�����Ə����Ɉ�v���c���܂��B
	 *						true���w�肷��Ə����s��v���c���܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�폜�������B
	 */
	virtual size_t Cull(const IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		if ( ! IsEmpty() )
		{
			loop_dn ( i, GetSize() )
			{
				if ( ! checker.IsValid(At(i)) ^ boIsReverse )
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
		EXCLUSIVE(this);
		if ( size == 0 )
		{
			return RemoveAll();
		}
		else
		{
			size_t nowSize = GetSize();
			if ( nowSize > size )
			{
				//����菬����
				for ( size_t i = nowSize - 1; i >= size; i-- )
				{
					if ( ! Remove(i) )
					{
						return false; 
					}
				}
			}
			else if ( nowSize < size )
			{
				//�����傫��
				size_t l = (size - nowSize);
				return AddElements(l) == l;
			}
		}
		return true;
	}

	/**
	 * [����] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		EXCLUSIVE(this);
		size_t l1, l2;
		ds >> l1 >> l2;
		if ( l1 != l2 )
		{
			throw CInvalidParamException();
		}
		RemoveAll();
		TYP t;
		loop ( i, l1 )
		{
			ds >> t;
			Add(t);
		}
	}

	/**
	 * [����] �\�[�g.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA��r���܂��B
	 *	@note �͈͂����肵�����ꍇ�A COffsetAdapterT ���g�p���܂��B
	 *	@param	comparator �R���p���[�^�[
	 *	@param	boIsReverse	�ȗ�����Ə��������Ƀ\�[�g���܂��B
	 *						true���w�肷��Ƒ傫�����ʃ\�[�g���܂��B
	 *	@param	boIsBubble	true���o�u���\�[�g�A���S���Y�����g���܂��B
	 *						�ʏ�̓N�C�b�N�\�[�g�A���S���Y���ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Sort(const IComparatorT<TYP>& comparator, bool boIsReverse = false, bool boIsBubble = false)
	{
		EXCLUSIVE(this);
		CSort sort;
		bool boRc = true;
		try
		{
			if ( boIsBubble )
			{
				sort.BubbleSort(*this, comparator, boIsReverse);
			}
			else
			{
				sort.QuickSort(*this, comparator, boIsReverse);
			}
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			boRc = false;
		}
		return boRc;
	}

	/**
	 * [����] �\�[�g.
	 *		IComparatorT::GetDefault() �œ�����R���p���[�^���g�p���Ĕ�r���܂��B
	 *	@note �͈͂����肵�����ꍇ�A COffsetAdapterT ���g�p���܂��B
	 *	@param	boIsReverse	�ȗ�����Ə��������Ƀ\�[�g���܂��B
	 *						true���w�肷��Ƒ傫�����ʃ\�[�g���܂��B
	 *	@param	boIsBubble	true���o�u���\�[�g�A���S���Y�����g���܂��B
	 *						�ʏ�̓N�C�b�N�\�[�g�A���S���Y���ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Sort(bool boIsReverse = false, bool boIsBubble = false)
	{	
		return Sort(IComparatorT<TYP>::GetDefault(), boIsReverse, boIsBubble);
	}

	/**
	 * [����] �V���b�t��.
	 *		�v�f�̕��т��V���b�t�����܂��B
	 *	@note �͈͂����肵�����ꍇ�A COffsetAdapterT ���g�p���܂��B
	 *	@param iDepth �V���b�t��������s�������w��ł��܂��B�����΃o������Ƃ������̂��L��܂���B
	 *				�ȗ������1��ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Shuffle(int iDepth = 1)
	{
		EXCLUSIVE(this);
		bool boRc = true;
		try
		{
			size_t size = GetSize();
			if ( size > 1 )
			{
				loop ( j, iDepth )
				{
					loop ( i, size )
					{
						size_t r = rand() % size;
						if ( r == i )
						{
							r = rand() % size; 
						} 
						Swap(i, r);
					}
				}
			}
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			boRc = false;
		}
		return boRc;
	}

	/**
	 * [����] �����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t �g�����Ώۂ̏��B
	 *	@param comparator �R���p���[�^�B
	 *	@return �c�������B
	 */
	size_t TrimBottom(const TYP& t, const IComparatorT<TYP>& comparator)
	{
		EXCLUSIVE(this);
		size_t len = GetSize();
		if ( len > 0 )
		{
			loop ( i, len ) 
			{
				if ( ! comparator.IsEqualTo(At(len - 1 - i), t) )
				{
					SetSize(len - i);
					return len - i;
				}
			}
			RemoveAll();
			return 0;
		}
		return len;
	}

	/**
	 * [����] �����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t �g�����Ώۂ̏��B
	 *	@return �c�������B
	 */
	size_t TrimBottom(const TYP& t)
	{
		return TrimBottom(t, IComparatorT<TYP>::GetDefault());
	}
};



/**@ingroup COLLECT ITERATOR
 * ���Q�Ǘ�����C���^�[�t�F�[�X�g���e���v���[�g
 *
 *		ICollectionT ���g�����Ă��܂��B�{�C���^�[�t�F�[�X�����Ɏ������A
 *		m_GetConstPointer() , m_GetPointer() ���������邱�ƂŁA�ȉ��̃��\�b�h��
 *		�p�ӂ��Ă���܂��B
 *
 *	@par
 *			1) Get const			<BR>
 *			2) At const				<BR>
 *			3) Ref					<BR>
 *			4) Set					<BR>
 *			5) operator[] const		<BR>
 *			6) operator[]			<BR>
 *
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@throw CTnbException �ǂݍ��ݏ������ɃG���[���������ꍇ�A CTnbException �̃T�u�N���X���X���[����\�����L��܂��B
 *
 *	@param TYP �v�f�̌^�B�^�̓R�s�[�@�\���K�{�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollection.h
 *
 *	@date 06/08/11 ���ۉ����āA�V�K�ɗp�ӁB
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP>
class ICollectionMidT : public ICollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
protected:

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const = 0;

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@retval NULL		���s�i�C���f�b�N�X�͈͊O�j
	 *	@retval NULL�ȊO	�A�h���X
	 */
	virtual TYP* m_GetPointer(INDEX index) = 0;

public:

	/// �R���X�g���N�^
	ICollectionMidT(void) : _super()
	{
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
		const TYP* P = m_GetConstPointer(index);
		if ( P != NULL )
		{
			return *P; 
		}
		throw CIndexOutOfBoundsException();
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
		const TYP* P = m_GetConstPointer(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
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
		TYP* P = m_GetPointer(index);
		if ( P == NULL )
		{
			throw CNotSupportException();
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
		TYP* P = m_GetPointer(index);
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
		TYP* P = m_GetPointer(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
	}
};



/**@ingroup COLLECT
 * �A���������z�u�^���Q�Ǘ��C���^�[�t�F�[�X�e���v���[�g
 *
 *		�����̗v�f�^(TYP)���Ǘ�����C���^�[�t�F�[�X�ł��B
 *		������������Ă���̂́A����A�������������Ŕz�u���ĊǗ����Ă���R���N�V�����ł��B
 *		GetSize() �ŃT�C�Y���m�F���A ReferBuffer() ���g�������Q�Ƃ��邱�Ƃ��o���܂��i�ύX�͏o���܂���j�B
 *
 *	@param TYP �v�f�̌^�B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollection.h
 *
 *	@date 06/05/01 �V�K�쐬
 *	@date 06/05/07 ISequenceCollectionT�ɖ��O�ύX�B
 *	@date 07/03/15 size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP>
struct ISequenceCollectionT
{
	/// �f�X�g���N�^
	virtual ~ISequenceCollectionT(void) {}

	/**
	 * [�擾] �f�[�^�A�h���X�擾.
	 *	@note	���̃A�h���X�̓��e�͏��������Ă͂����܂���B
	 *	@note	�擾�����A�h���X�� delete �ŊJ�����Ă͂����܂���B
	 *	@note	�擾�����A�h���X�͖{�N���X�̌v�Z���A���삷��Ɩ����ɂȂ�܂��B
	 *	@return �A�h���X�B�m�ۂ��Ă��Ȃ��ꍇ�ANULL���Ԃ�܂��B
	 */
	virtual const TYP* ReferBuffer(void) const = 0;

	/**
	 * [�擾] �v�f���擾.
	 *	@note Pure�֐��ł��B
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const = 0;
};



namespace HASH
{
	/** COLLECT
	 * [HASH] �n�b�V���l�v�Z.
	 *	@note TYP ���V�t�g���Z�\�̂��̂̂݁B
	 *	@param c IConstCollectionT
	 *	@return �n�b�V���l
	 */
	template<typename TYP>
	inline DWORD CalcHash(const IConstCollectionT<TYP>& c)
	{
		EXCLUSIVE(&c);
		DWORD dwRc = 0;
		loop ( i, c.GetSize() )
		{
			dwRc += c.At(i) << (i & 0x0F);
		}
		return dwRc;
	}
};



}; // TNB

