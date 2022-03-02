#pragma once
/**
 *@file
 * ���Q�Ǘ��A�_�v�^�֌W�̃w�b�_
 *
 *		���Q�Ǘ��̃A�_�v�^�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �A�_�v�^�x�[�X�e���v���[�g
 *
 *		�A�_�v�^�\�̃x�[�X�ƂȂ�e���v���[�g�N���X�ł��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollectionAdapter.h
 *
 *	@date 06/09/13 ���ۉ����ĐV�K�쐬
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 11/06/01 �R�s�[�I�y���[�^������
 */
template<typename TYP>
class CAbstractAdapterT : public IConstCollectionT<TYP>
{
	DEFSUPER(IConstCollectionT<TYP>);
protected:

	const IConstCollectionT<TYP>*	m_pCol;		///< �Ώۂ̃R���N�V����
	size_t							m_size;	///< �T�C�Y

public:

	/**
	 * �R���X�g���N�^
	 *	@param c IConstCollectionT �����R���N�V����
	 */
	CAbstractAdapterT(const IConstCollectionT<TYP>* c)
		: _super(), m_pCol(c), m_size(0)
	{
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
		return m_pCol->Lock(); 
	}

	/// [�r��] �A�����b�N
	virtual void Unlock(void) const 
	{
		m_pCol->Unlock(); 
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [���] �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��
	 */
	CAbstractAdapterT& operator=(const CAbstractAdapterT& other)
	{
		m_pCol = other.m_pCol;
		m_size = other.m_size;
		return *this;
	}
};



/**@ingroup COLLECT ITERATOR
 * �I�t�Z�b�g�Q�ƃA�_�v�^�e���v���[�g
 *
 *		IConstCollectionT �����R���N�V�����̈ꕔ�𑀍삷�鎞�ɂ��̃A�_�v�^���g���܂��B
 *		�R���X�g���N�^�ŃR���N�V�����ƃI�t�Z�b�g�A�T�C�Y���w�肷��ƁA���͈̔͂̃R���N�V�����ɂȂ�܂��B
 *		���̂́A�R���X�g���N�^�œn�������̂Ȃ̂ŁA�{�C���X�^���X���L���̊ԁA���̂������Ȃ��悤�ɂ��܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollectionAdapter.h
 *
 *	@date 06/05/23 �V�K�쐬
 *	@date 06/08/01 GetConstPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/09/13 ���ۃx�[�X�N���X�g�p�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 09/11.09 �T�C�Y�w����@�̕ύX�B
 */
template<typename TYP>
class CConstOffsetAdapterT : public CAbstractAdapterT<TYP>
{
	DEFSUPER(CAbstractAdapterT<TYP>);
	INDEX	m_startIndex;				///< �J�n�I�t�Z�b�g�l
public:

	/**
	 * �R���X�g���N�^
	 *	@param c IConstCollectionT �����R���N�V����
	 *	@param startIndex �J�n�C���f�b�N�X���w��
	 *	@param size �T�C�Y���w��B�ȗ������ꍇ�Ac �̍Ō�܂ł������ɂȂ�܂��B
	 *				�܂��Ac �����傫���T�C�Y���w�肵�Ă� c �̍Ō�܂łɂȂ�܂��B
	 */
	CConstOffsetAdapterT(const IConstCollectionT<TYP>* c, INDEX startIndex, size_t size = INVALID_SIZE)
		: _super(c), m_startIndex(startIndex) 
	{
		_super::m_size = size;
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		size_t l = m_pCol->GetSize() - m_startIndex;
		if ( _super::m_size != INVALID_SIZE && l > _super::m_size)
		{
			l = _super::m_size;
		}
		return l;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_pCol->At(index + m_startIndex);
	}
};



/**@ingroup COLLECT ITERATOR
 * ���o�[�X�Q�ƃA�_�v�^�e���v���[�g
 *
 *		IConstCollectionT �����R���N�V�������C���f�b�N�X���t���ɂ���A�_�v�^�ł��B
 *		�R���X�g���N�^�ŃR���N�V�������w�肷��ƁA���т��t�̃R���N�V�����ɂȂ�܂��B
 *		���̂́A�R���X�g���N�^�œn�������̂Ȃ̂ŁA�{�C���X�^���X���L���̊ԁA���̂������Ȃ��悤�ɂ��܂��B
 *		�܂��A�T�C�Y�̓R���X�g���N�^���ɋL�����܂��̂ŁA���R���N�V�������g�����Ă��{�N���X�̃T�C�Y�͕ω����܂���B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollectionAdapter.h
 *
 *	@date 06/06/01 �V�K�쐬
 *	@date 06/08/01 GetConstPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/09/13 ���ۃx�[�X�N���X�g�p�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP>
class CConstReverseAdapterT : public CAbstractAdapterT<TYP>
{
	DEFSUPER(CAbstractAdapterT<TYP>);
public:

	/**
	 * �R���X�g���N�^
	 *	@param c IConstCollectionT �����R���N�V����
	 */
	explicit CConstReverseAdapterT(const IConstCollectionT<TYP>* c) : _super(c) 
	{
		_super::m_size = c->GetSize();
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_pCol->At(_super::m_size - index - 1);
	}
};



/**@ingroup COLLECT ITERATOR
 * �A�y���h�Q�ƃA�_�v�^�e���v���[�g
 *
 *		IConstCollectionT �����R���N�V�������A�����ĎQ�Ƃ��鎞�ɂ��̃A�_�v�^���g���܂��B
 *		���̂́A�R���X�g���N�^�œn�������̂Ȃ̂ŁA�{�C���X�^���X���L���̊ԁA���̂������Ȃ��悤�ɂ��܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollectionAdapter.h
 *
 *	@date 06/06/30 �V�K�쐬
 *	@date 06/08/01 GetConstPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP>
class CConstAppendAdapterT : public CAbstractAdapterT<TYP>
{
	DEFSUPER(CAbstractAdapterT<TYP>);
	const IConstCollectionT<TYP>*	m_p2ndCol;	///< �ΏۃR���N�V����(��)
	size_t	m_sizeCol;
public:

	/**
	 * �R���X�g���N�^
	 *	@param c1 IConstCollectionT �����R���N�V����
	 *	@param c2 IConstCollectionT �����R���N�V����
	 */
	CConstAppendAdapterT(const IConstCollectionT<TYP>* c1, const IConstCollectionT<TYP>* c2)
		: _super(c1), m_p2ndCol(c2)
	{
		m_sizeCol = m_pCol->GetSize();
		_super::m_size = m_sizeCol + m_p2ndCol->GetSize();
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
		return m_pCol->Lock() && m_p2ndCol->Lock();
	}

	/// [�r��] �A�����b�N
	virtual void Unlock(void) const
	{
		m_p2ndCol->Unlock();
		m_pCol->Unlock();
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		return (index < m_sizeCol) ? m_pCol->At(index) : m_p2ndCol->At(index - m_sizeCol);
	}
};



/**@ingroup COLLECT ITERATOR
 * �I�t�Z�b�g�A�_�v�^�e���v���[�g
 *
 *		ICollectionT �����R���N�V�����̈ꕔ�𑀍삷�鎞�ɂ��̃A�_�v�^���g���܂��B
 *		�R���X�g���N�^�ŃR���N�V�����ƃI�t�Z�b�g�A�T�C�Y���w�肷��ƁA���͈̔͂̃R���N�V�����ɂȂ�܂��B
 *		���̂́A�R���X�g���N�^�œn�������̂Ȃ̂ŁA�{�C���X�^���X���L���̊ԁA���̂������Ȃ��悤�ɂ��܂��B
 *
 *	@note Add , Append , Remove , RemoveAll , Copy �ȂǁA�R���N�V�����T�C�Y�̕ς�郁�\�b�h�͎��s���܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollectionAdapter.h
 *
 *	@date 06/05/23 �V�K�쐬
 *	@date 06/08/01 GetConstPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/11 Element() �� At() �ɉ����BReferElementPtr() ��p�~�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename TYP>
class COffsetAdapterT : public ICollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
	ICollectionT<TYP>*	m_pCol;			///< �Ώۂ̃R���N�V����
	INDEX				m_startIndex;	///< �J�n�I�t�Z�b�g
	size_t				m_size;			///< �T�C�Y
public:

	/**
	 * �R���X�g���N�^
	 *	@param c ICollectionT �����R���N�V����
	 *	@param startIndex �J�n�C���f�b�N�X���w��
	 *	@param size �T�C�Y���w��B�w�肪�Ȃ��ꍇ�Ac �̍Ō�܂ł������ɂȂ�܂��B
	 *				�܂��Ac �����傫���T�C�Y���w�肵�Ă� c �̍Ō�܂łɂȂ�܂��B
	 */
	COffsetAdapterT(ICollectionT<TYP>* c, INDEX startIndex, size_t size = 0)
		: _super(), m_pCol(c), m_startIndex(startIndex), m_size(size)
	{
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
		return m_pCol->Lock(); 
	}

	/// [�r��] �A�����b�N
	virtual void Unlock(void) const 
	{
		m_pCol->Unlock(); 
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		size_t l = m_pCol->GetSize() - m_startIndex;
		return TNB::IsInRange(m_size, l) ? m_size : l;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̎Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_pCol->At(index + m_startIndex);
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
		return m_pCol->Ref(index + m_startIndex);
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
		return m_pCol->Set(index + m_startIndex, t);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@attention �{�A�_�v�^�ł͎g�p�ł��܂���B
	 *	@param t �v�f�B
	 *	@return ��� INVALID_INDEX�B
	 */
	virtual INDEX Add(const TYP& t) 
	{
		return INVALID_INDEX; 
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@attention �{�A�_�v�^�ł͎g�p�ł��܂���B
	 *	@param index �C���f�b�N�X
	 *	@return ��� false
	 */
	virtual bool Remove(INDEX index) 
	{
		return false; 
	}

	/**
	 * [�ݒ�] �v�f�̓���ւ�
	 *	@param index1 �C���f�b�N�X�P
	 *	@param index2 �C���f�b�N�X�Q
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual void Swap(INDEX index1, INDEX index2)
	{
		m_pCol->Swap(index1 + m_startIndex, index2 + m_startIndex);
	}
};



/**@ingroup COLLECT ITERATOR
 * ���Q�ƃA�_�v�^�e���v���[�g
 *
 *		IConstCollectionT / ISequenceCollectionT �C���^�[�t�F�[�X�������Ȃ��z���
 *		IConstCollectionT / ISequenceCollectionT �ŎQ�Ƃł���悤�ɂ���A�_�v�^�N���X�ł��B
 *
 *	@note	begin() / end() �ɂ��C�e���[�^�́A�P���ɗv�f�̃|�C���^�ɂȂ��Ă��܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ISequenceCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollectionAdapter.h
 *
 *	@date 06/04/26 �V�K
 *	@date 06/05/01	ISequenceCollectionT ����������悤�ɕύX�B
 *					CConstCollectionT�ɖ��O�ύX�B
 *	@date 06/05/15 	CConstCollectorT�ɖ��O�ύX�B
 *	@date 06/06/29 	CConstAdapterT�ɖ��O�ύX�B
 *	@date 06/08/13 �C�e���[�^�� STL �ɂ��킹�āA���C�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 11/06/01 �R�s�[�I�y���[�^������
 */
template<typename TYP>
class CConstAdapterT : public IConstCollectionT<TYP>, public ISequenceCollectionT<TYP>
{
	DEFSUPER(IConstCollectionT<TYP>);
	const TYP*			m_pcType;		///< �ێ�����f�[�^�̃|�C���^
	size_t				m_size;			///< �ێ�����f�[�^�̒���
public:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// const_iterator�^�錾
	typedef const TYP* const_iterator;
	#endif //_TnbDOXYEM

	/**
	 * [����] �擪const_iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return m_pcType; }

	/**
	 * [����] �Ō�const_iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return m_pcType + m_size; }

	/**
	 * �R���X�g���N�^
	 *	@param size	�Q�ƌ��̒���
	 *	@param P	�Q�ƌ��̃|�C���^
	 */
	CConstAdapterT(size_t size, LPCVOID P) 
		: _super(), m_pcType(static_cast<const TYP*>(P)), m_size(size)
	{
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime �_�~�[�B�ȗ����Ă��������B
	 *	@return ���true�B
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true;
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const 
	{
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [�擾] �f�[�^�A�h���X�擾.
	 *	@note	���̃A�h���X�̓��e�͏��������Ă͂����܂���B
	 *	@note	�擾�����A�h���X�� delete �ŊJ�����Ă͂����܂���B
	 *	@note	�擾�����A�h���X�͖{�N���X�̌v�Z���A���삷��Ɩ����ɂȂ�܂��B
	 *	@return �A�h���X�B�m�ۂ��Ă��Ȃ��ꍇ�ANULL���Ԃ�܂��B
	 */
	virtual const TYP* ReferBuffer(void) const
	{
		return m_pcType;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return �Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
		return m_pcType[index];
	}

	/**
	 * [�擾] �f�[�^�v�f�擾
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̓��e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& operator[](INDEX index) const
	{
		return At(index);
	}

	/**
	 * [���] �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��
	 */
	CConstAdapterT& operator=(const CConstAdapterT& other)
	{
		m_pcType = other.m_pcType;
		m_size = other.m_size;
		return *this;
	}
};



/**@ingroup COLLECT ITERATOR
 * INDEX�A�_�v�^�e���v���[�g
 *
 *		IConstCollectionT �����R���N�V�������ꎞ�I�ɕ��ёւ��������ɂ��̃A�_�v�^���g���܂��B
 *		������Index�ϊ��e�[�u���������A�v�f�� Swap ��A�v�f�̍폜��Y����s���Ă��A���͕̂ω������A
 *		����Index�ϊ��e�[�u���̂ݑ��삵�܂��B
 *
 *		���̂́A�R���X�g���N�^�œn�������̂Ȃ̂ŁA�{�C���X�^���X���L���̊ԁA���̂������Ȃ��悤�ɂ��܂��B
 *		�܂��A���̂̃T�C�Y���ύX���Ȃ��悤�ɂ��Ă��������B
 *
 *	@note ICollectionT �͎����Ă��܂��񂪁A Sort() , Remove() , Swap() ���\�b�h���L��܂��B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCollectionAdapter.h
 *
 *	@date 06/08/22 �V�K�쐬
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 07/06/19 Cull() , ResetIndex() ��ǉ��B
 */
template<typename TYP>
class CIndexAdapterT : public IConstCollectionT<TYP>, CCopyImpossible
{
	DEFSUPER(IConstCollectionT<TYP>);

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	// memo; ICollectionT �� sort ���g�p���邽�߂ɃC���i�[�N���X���쐬���Ă���B
	struct TParam : ICollectionT<TYP>
	{
		DEFSUPER(ICollectionT<TYP>);
		const IConstCollectionT<TYP>*	m_pCol;		///< ���R���N�V�����Q��
		INDEX*							m_pIndexs;	///< Index�e�[�u��
		size_t							m_size;		///< Index�e�[�u���T�C�Y
		// ���b�N
		virtual bool Lock(DWORD dwTime = INFINITE) const { return m_pCol->Lock(); }
		// �A�����b�N
		virtual void Unlock(void) const { m_pCol->Unlock(); }
		// �v�f���擾
		virtual size_t GetSize(void) const { return m_size; }
		// �v�f�̎Q�Ǝ擾.
		virtual const TYP& At(INDEX index) const
		{
			if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
			return m_pCol->At(m_pIndexs[index]);
		}
		// �v�f�̎Q�Ǝ擾.
		virtual TYP& Ref(INDEX index) { throw CNotSupportException(); }
		// �v�f�̐ݒ�.
		virtual bool Set(INDEX index, const TYP& t) { return false; }
		// �v�f��ǉ�.
		virtual INDEX Add(const TYP& t) { return INVALID_INDEX; }
		// �v�f��폜.
		virtual bool Remove(INDEX index)
		{
			if ( ! IsInRange(index) ){ return false; }
			for ( INDEX i = index; i < m_size - 1; i++ )
			{
				m_pIndexs[i] = m_pIndexs[i + 1];
			}
			m_size--;
			return true;
		}
		// �v�f�̓���ւ�
		virtual void Swap(INDEX index1, INDEX index2)
		{
			if ( ! IsInRange(index1) || ! IsInRange(index2) ){ throw CIndexOutOfBoundsException(); }
			TNB::Swap(m_pIndexs[index1], m_pIndexs[index2]);
		}
	};

	#endif //Document�쐬�p�V���{��

	TParam m_param;		///< ����

public:

	/**
	 * �R���X�g���N�^
	 *	@param c IConstCollectionT �����R���N�V����
	 */
	CIndexAdapterT(const IConstCollectionT<TYP>* c) : _super()
	{
		m_param.m_pCol			= c;
		m_param.m_size			= c->GetSize();
		m_param.m_pIndexs		= new INDEX[m_param.m_size];
		loop ( i, m_param.m_size )
		{
			m_param.m_pIndexs[i] = i;
		}
	}

	/// �f�X�g���N�^
	virtual ~CIndexAdapterT(void)
	{
		if ( m_param.m_pIndexs != NULL )
		{
			delete[] m_param.m_pIndexs;
			m_param.m_pIndexs = NULL;
		}
	}

	/**
	 * [�擾] �I���W�i��Index�擾.
	 *	@note	At() , Get() �Ȃǂ͖{�N���X�̃��\�b�h�ŉ\�ł����A
	 *			Set() , Remove() �ȂǑΏۃR���N�V�����̏�������������s�������ꍇ�A
	 *			���̃��\�b�h�őΏۃR���N�V�����̃C���f�b�N�X�𓾂āA�ΏۃR���N�V�����𑀍삵�܂��B<BR>
	 *	@attention �ΏۃR���N�V�����𑀍삵����A��x ResetIndex() �����s���邩�A
	 *				���̃A�_�v�^�ł̑���͍s��Ȃ��悤�ɂ��Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval INVALID_INDEX �C���f�b�N�X���͈͊O
	 *	@retval INVALID_INDEX�ȊO �R���X�g���N�^�Ŏw�肵���R���N�V�����ł̃C���f�b�N�X
	 */
	INDEX GetRawIndex(INDEX index) const
	{
		return IsInRange(index) ? m_param.m_pIndexs[index] : INVALID_INDEX;
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
		return m_param.Lock(); 
	}

	/// [�r��] �A�����b�N
	virtual void Unlock(void) const 
	{
		m_param.Unlock(); 
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const 
	{
		return m_param.m_size; 
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �ꎞ�I�ȎQ��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_param.At(index);
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@param index �C���f�b�N�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Remove(INDEX index)
	{
		return m_param.Remove(index);
	}

	/**
	 * [�ݒ�] �v�f�̓���ւ�
	 *	@param index1 �C���f�b�N�X�P
	 *	@param index2 �C���f�b�N�X�Q
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	void Swap(INDEX index1, INDEX index2)
	{
		m_param.Swap(index1, index2);
	}

	/**
	 * [����] �\�[�g.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA��r���܂��B
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
		return m_param.Sort(comparator, boIsReverse, boIsBubble);
	}

	/**
	 * [����] �\�[�g.
	 *		IComparatorT::GetDefault() �œ�����R���p���[�^���g�p���Ĕ�r���܂��B
	 *	@param	boIsReverse	�ȗ�����Ə��������Ƀ\�[�g���܂��B
	 *						true���w�肷��Ƒ傫�����ʃ\�[�g���܂��B
	 *	@param	boIsBubble	true���o�u���\�[�g�A���S���Y�����g���܂��B
	 *						�ʏ�̓N�C�b�N�\�[�g�A���S���Y���ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Sort(bool boIsReverse = false, bool boIsBubble = false)
	{	
		return m_param.Sort(boIsReverse, boIsBubble);
	}

	/**
	 * [�폜] �Ԉ���.
	 *		�w��̃f�[�^�̏����ɍ���Ȃ�����(�`�F�b�J�[�̌��ʂɂ��)���폜���܂��B
	 *	@param	checker {@link IConstCollectionT::IChecker �`�F�b�J�[}
	 *	@param	boIsReverse	�ȗ�����Ə����Ɉ�v���c���܂��B
	 *						true���w�肷��Ə����s��v���c���܂��B
	 *	@retval �폜�������B
	 */
	size_t Cull(const IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		if ( m_param.m_size > 0 )C
		{
			size_t size = m_param.m_size;
			INDEX* P = new INDEX[size];
			MemCopy(P, m_param.m_pIndexs, size);
			for ( size_t i = 0; i < size; i++ )
			{
				if ( checker.IsValid(At(i)) ^ boIsReverse )
				{
					m_param.m_pIndexs[r++] = P[i];
				}
			}
			m_param.m_size = r;
			delete[] P;
		}
		return r;
	}

	/**
	 * [����] ���Z�b�g�C���f�b�N�X.
	 *	@note �C���f�b�N�X�e�[�u�������������܂��B�ΏۃR���N�V�����Ɠ������A�������тɂȂ�܂��B
	 */
	void ResetIndex(void)
	{
		if ( m_param.m_size	!= m_pCol->GetSize() )
		{
			m_param.m_size = m_pCol->GetSize();
			if ( m_param.m_pIndexs != NULL )
			{
				delete[] m_param.m_pIndexs;
			}
			m_param.m_pIndexs = new INDEX[m_param.m_size];
		}
		loop ( i, m_param.m_size )
		{
			m_param.m_pIndexs[i] = i;
		}
	}




};



}; // TNB

