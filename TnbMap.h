#pragma once
/**
 *@file
 * �}�b�v�^���Ǘ��֌W�̃w�b�_
 *
 *		��ӂȃL�[�ɒl�����蓖�ĂăR���N�V�������邱�Ƃ��ł��܂��B
 *		�L�[�Œl�����o������폜���邱�Ƃ��ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPair.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �}�b�v�^���Ǘ��e���v���[�g
 *
 *		��ӂȃL�[�ɒl�����蓖�ĂăR���N�V�������邱�Ƃ��ł��܂��B
 *		�L�[�Œl�����o������폜���邱�Ƃ��ł��܂��B
 *
 *	@note At() �� Set() �ȂǁA CPairT �^�ŁA���ڃA�N�Z�X����ƁA
 *			��Key��2�ȏ㑶�݂���MAP���o���Ă��܂��܂��̂ł����ӂ��������B
 *
 *	@note	KEY �̈ꗗ���쐬���� EnumKeys() ������܂��B
 *			�Q�Ƃ����ł���΁A GetKeysAdapter() �Ȃǂ� CConstKeysAdapter CConstValuesAdapter ��
 *			�쐬���A�A�N�Z�X���邱�Ƃ��o���܂��B
 *
 *	@attention ���� STL �� map �ƈႢ�A�i�[���ɃL�[�ɂ��\�[�g���Ă��܂���B
 *
 *	@param	KEY	�L�[�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\�A��r�@�\( IComparableT ���A�����Z)���K�{�B
 *	@param	VAL	�l�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\���K�{�B
 *	@param	INK	�L�[�Ƃ���^�̓��͌^�B�ȗ��\�B
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@note ICollectionT �� TYP �� CPairT<KEY,VAL> �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMap.h
 *
 *	@date 06/01/01	�V�K
 *	@date 06/04/21	ICollectionT ����
 *	@date 06/04/27	INK�p�����[�^�ǉ��B�L�[�̗L���`�F�b�N���\�b�h�A�ǉ��B
 *	@date 06/05/15	Clone��p�~�B
 *	@date 06/06/28	GetKeysAdapter() , GetValuesAdapter() �ǉ��B
 *	@date 06/08/01	GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
 *	@date 06/08/11	Element() �� At() �ɉ����BReferElementPtr() ��p�~�B
 *	@date 06/09/12	SetSize()�K�����s����悤�ɂ����B
 *	@date 07/03/15	index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 12/05/22	Append() �̏�����ǉ��B
 *	@date 12/10/19	IsEqual() ���p�Ɏ����B
 */
template<typename KEY, typename VAL, typename INK=KEY>
class CMapT : public ICollectionT<TNB::CPairT<KEY,VAL> >
{
	typedef ICollectionT<TNB::CPairT<KEY,VAL> > _super;	///< �e�N���X���Đ錾
	DEFSUPER(_super);
public:

	typedef TNB::CPairT<KEY,VAL> CPair;	///< �y�A�^ ��`

	/**@ingroup COLLECT ITERATOR
	 * �}�b�v�^���̃L�[�̂ݎQ�ƃA�_�v�^�N���X
	 *
	 *		IConstCollectionT �����A�L�[�����̃R���N�V�����Ƃ��ĎQ�Ƃ��鎞�ɂ��̃A�_�v�^���g���܂��B
	 *		���̂́A�R���X�g���N�^�œn�������̂Ȃ̂ŁA�{�C���X�^���X���L���̊ԁA���̂������Ȃ��悤�ɂ��܂��B
	 *
	 *		�C���f�b�N�X�͎��̂� CMapT �Ɠ����Ȃ̂ŁA���̃N���X�ŃC���f�b�N�X���Q�Ƃ��A
	 *		���̂� CMapT �𑀍삷�邱�Ƃ��\�ł��B
	 *
	 *	@see	IConstCollectionT ���������Ă��܂��B
	 *
	 *	@note IConstCollectionT �� TYP �� KEY �ɂȂ�܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMap.h
	 *
	 *	@date 06/06/28	�V�K
	 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
	 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
	 */
	class CConstKeysAdapter : public IConstCollectionT<KEY>
	{
		DEFSUPER(IConstCollectionT<KEY>);
		const CMapT* m_pMap;					///< �Ώۂ̃}�b�v
	public:
		/**
		 * �R���X�g���N�^
		 *	@param map CMapT �̃C���X�^���X
		 */
		CConstKeysAdapter(const CMapT* map) : _super(), m_pMap(map) {}
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
			return m_pMap->Lock();
		}
		/// [�r��] �A�����b�N
		virtual void Unlock(void) const
		{
			m_pMap->Unlock();
		}
		/**
		 * [�擾] �v�f���擾
		 *	@return �v�f��
		 */
		virtual size_t GetSize(void) const
		{
			return m_pMap->GetSize();
		}
		/**
		 * [�擾] �v�f�̎Q�Ǝ擾.
		 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
		 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
		 *	@param index �C���f�b�N�X
		 *	@return �Q��
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 */
		virtual const KEY& At(INDEX index) const
		{
			return m_pMap->At(index).first;
		}
	};

	/**@ingroup COLLECT ITERATOR
	 * �}�b�v�^���̒l�̂ݎQ�ƃA�_�v�^�N���X
	 *
	 *		IConstCollectionT �����A�l�����̃R���N�V�����Ƃ��ĎQ�Ƃ��鎞�ɂ��̃A�_�v�^���g���܂��B
	 *		���̂́A�R���X�g���N�^�œn�������̂Ȃ̂ŁA�{�C���X�^���X���L���̊ԁA���̂������Ȃ��悤�ɂ��܂��B
	 *
	 *		�C���f�b�N�X�͎��̂� CMapT �Ɠ����Ȃ̂ŁA���̃N���X�ŃC���f�b�N�X���Q�Ƃ��A
	 *		���̂� CMapT �𑀍삷�邱�Ƃ��\�ł��B
	 *
	 *	@see	IConstCollectionT ���������Ă��܂��B
	 *
	 *	@note IConstCollectionT �� TYP �� VAL �ɂȂ�܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMap.h
	 *
	 *	@date 06/06/28	�V�K
	 *	@date 06/08/01 GetConstPointer/GetPointer�Ɉˑ����Ȃ�����ɕύX�B
	 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
	 */
	class CConstValuesAdapter : public IConstCollectionT<VAL>
	{
		DEFSUPER(IConstCollectionT<VAL>);
		const CMapT* m_pMap;				///< �Ώۂ�MAP
	public:
		/**
		 * �R���X�g���N�^
		 *	@param map CMapT �̃C���X�^���X
		 */
		CConstValuesAdapter(const CMapT* map) : _super(), m_pMap(map) {}
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
			return m_pMap->Lock();
		}
		/// [�r��] �A�����b�N
		virtual void Unlock(void) const
		{
			m_pMap->Unlock();
		}
		/**
		 * [�擾] �v�f���擾
		 *	@return �v�f��
		 */
		virtual size_t GetSize(void) const
		{
			return m_pMap->GetSize();
		}
		/**
		 * [�擾] �v�f�̎Q�Ǝ擾.
		 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
		 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
		 *	@param index �C���f�b�N�X
		 *	@return �Q��
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 */
		virtual const VAL& At(INDEX index) const
		{
			return m_pMap->At(index).second;
		}
	};

private:

	CVectorT< CPair >	m_vPair;	///< CPairT�̔z��

	/**
	 * �L�[�̒l�̃|�C���^�擾
	 *	@param key �T���L�[
	 *	@retval NULL		key�͑��݂��Ȃ�
	 *	@retval NULL�ȊO	VAL�̃|�C���^
	 */
	const VAL* m_SearchKey(INK key) const
	{
		const CPair* P = m_vPair.ReferBuffer();
		if ( P != NULL )
		{
			loop ( i, m_vPair.GetSize() )
			{
				if ( P[i].first == key )
				{
					return &(P[i].second);
				}
			}
		}
		return NULL;
	}

	/**
	 * �L�[�̒l�̃|�C���^�擾
	 *	@param key �T���L�[
	 *	@retval NULL		key�͑��݂��Ȃ�
	 *	@retval NULL�ȊO	VAL�̃|�C���^
	 */
	VAL* m_SearchKey(INK key)
	{
		loop ( i, m_vPair.GetSize() )
		{
			if ( m_vPair[i].first == key )
			{
				return &(m_vPair[i].second);
			}
		}
		return NULL;
	}

public:

	/**
	 * �R���X�g���N�^ 
	 *	@note �w��̃������m�ێ��̒ǉ��T�C�Y��ݒ肵�܂��B
	 *	@note �f�[�^�͕ێ����܂���
	 *	@param size �������m�ێ��̒ǉ��T�C�Y�BSetIncrementSize() �Ɠ����ł��B
	 */
	explicit CMapT(size_t size = 0) : _super(), m_vPair(size) 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^ 
	 *	@param map �R�s�[��
	 */
	CMapT(const CMapT& map) : _super(), m_vPair(map.m_vPair)
	{
	}

	/** �f�X�g���N�^ */
	virtual ~CMapT(void) 
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CMapT& operator=(const CMapT& other)
	{
		m_vPair = other.m_vPair;
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
		return m_vPair.Lock();
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		m_vPair.Unlock();
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
		m_vPair.SetIncrementSize(size); 
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_vPair.GetSize();
	}

	/**
	 * [�擾] �L�[�ƒl���擾.
	 *		�O�`(GetSize()�̖߂�l����) ���C���f�b�N�X�Ƃ��ACPairT<KEY,VAL> �^�Ŏ擾���܂��B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A GetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual CPair Get(INDEX index) const
	{
		return m_vPair.Get(index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �ꎞ�I�ȎQ��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const CPair& At(INDEX index) const
	{
		return m_vPair.At(index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �ꎞ�I�ȎQ��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual CPair& Ref(INDEX index)
	{
		return m_vPair.Ref(index);
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
	virtual bool Set(INDEX index, const CPair& t)
	{
		return m_vPair.Set(index, t);
	}

	/**
	 * [�ǉ�] �L�[�ƒl�ǉ�.
	 *		���łɃL�[������ꍇ�A�l�̂ݕύX���܂��B
	 *	@param t �v�f
	 *	@return ��ɂO�B �ǉ�����INDEX�͕Ԃ�܂���B
	 */
	virtual INDEX Add(const CPair& t)
	{
		SetKey(t.first, t.second);
		return 0;
	}

	/**
	 * [�ǉ�] �ǉ�.
	 *		�w��̃f�[�^��ǉ����܂��B
	 *	@note �����L�[������ꍇ�A�ǉ����� c �Ɋ܂܂��l�̂ق����L���ɂȂ�܂��B
	 *	@param c �ǉ�����f�[�^
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǉ��������B
	 */
	virtual size_t Append(const IConstCollectionT<CPair>& c)
	{
		EXCLUSIVE2(&c, this);
		loop ( i, c )
		{
			RemoveKey(c.At(i).first);
		}
		return _super::Append(c);
	}

	/**
	 * [�폜] �L�[�ƒl��폜.
	 *		�O�`(GetSize()�̖߂�l����) ���C���f�b�N�X�Ƃ��������܂��B
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval true	�폜����
	 *	@retval false	���s
	 */
	virtual bool Remove(INDEX index)
	{
		return m_vPair.Remove(index);
	}

	/**
	 * [�폜] ���ׂẴL�[�ƒl���폜
	 *	@return ���true
	 */
	virtual bool RemoveAll(void)
	{
		return m_vPair.RemoveAll();
	}

	/**
	 * [����] �T�C�Y�w��.
	 *	@param size �T�C�Y
	 *	@return �K�� false (���s)�B
	 */
	virtual bool SetSize(size_t size)
	{
		return false;
	}

	/**
	 * [�ݒ�] �L�[�ɑ΂���l�ݒ�
	 *	@note �L�[���Ȃ��ꍇ�A�V�K�ɒǉ����܂��B
	 *	@param key �L�[
	 *	@param newValue �l
	 */
	void SetKey(INK key, const VAL& newValue)
	{
		EXCLUSIVE(this);
		operator[](key) = newValue;
	}

	/**
	 * [�擾] �L�[�ɑ΂���l�̎Q��
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@note �L�[���Ȃ��ꍇ�A�V�K�ɒǉ����܂��B
	 *	@param key �L�[
	 *	@return VAL�̎Q�Ɓi���삷��ƁAMAP���̏����ω����܂��j�B
	 */
	VAL& operator[](INK key)
	{
		VAL* P = m_SearchKey(key);
		if ( P == NULL )
		{
			CPair t;
			t.first = key;
			m_vPair.Add(t);
			P = m_SearchKey(key);
		}
		return *P;
	}

	/**
	 * [�擾] �L�[�ɑ΂���l�̎Q��
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@note �L�[���Ȃ��ꍇ�A�V�K�ɒǉ����܂��B
	 *	@param key �L�[
	 *	@return VAL�̎Q�Ɓi���삷��ƁAMAP���̏����ω����܂��j�B
	 *	@throw CEmptyException �L�[�����݂��Ȃ����Ƃ��������߂ɃX���[����܂��B
	 */
	const VAL& operator[](INK key) const
	{
		const VAL* P = m_SearchKey(key);
		if ( P == NULL ){ throw CEmptyException(); }
		return *P;
	}

	/**
	 * [�폜] �L�[�ƒl���폜
	 *	@param key �L�[
	 *	@retval true ����
	 *	@retval false ���s�i�L�[���Ȃ��j
	 */
	bool RemoveKey(INK key)
	{
		EXCLUSIVE(this);
		const CPair* P = m_vPair.ReferBuffer();
		loop ( i, m_vPair.GetSize() )
		{
			if ( P[i].first == key )
			{
				m_vPair.Remove(i);
				return true;
			}
		}
		return false;
	}

	/**
	 * [�擾] �l�A�h���X�擾.
	 *		�L�[�ɑ΂���A�l�̃|�C���^�����o���܂�
	 *	@param key	�L�[
	 *	@retval NULL�ȊO �l�A�h���X
	 *	@retval NULL ���s(�L�[�Ȃ�)
	 */
	const VAL* GetValuePtr(INK key) const
	{
		EXCLUSIVE(this);
		return m_SearchKey(key);
	}

	/**
	 * [�擾] �l�A�h���X�擾.
	 *		�L�[�ɑ΂���A�l�̃|�C���^�����o���܂�
	 *	@param key	�L�[
	 *	@retval NULL�ȊO �l�A�h���X
	 *	@retval NULL ���s(�L�[�Ȃ�)
	 */
	VAL* GetValuePtr(INK key)
	{
		EXCLUSIVE(this);
		return m_SearchKey(key);
	}

	/**
	 * [�擾] �L�[�ƒl���擾.
	 *		�O�`(GetSize()�̖߂�l����) ���C���f�b�N�X�Ƃ��A�L�[�ƒl�����o���܂�
	 *	@param[in]	index	�v�f�ԍ�
	 *	@param[out] _rKey	�L�[
	 *	@param[out] _rValue	�l
	 *	@retval true �擾����
	 *	@retval false ���s�i�C���f�b�N�X���͈͊O�j
	 */
	bool Get(INDEX index, KEY& _rKey, VAL& _rValue) const
	{
		EXCLUSIVE(this);
		if ( ! TNB::IsInRange(index, m_vPair.GetSize()) )
		{
			return false;
		}
		const CPair* P = m_vPair.ReferBuffer();
		_rKey = P[index].first;
		_rValue = P[index].second;
		return true;
	}

	/**
	 * [�擾] �L�[�ɑ΂���l�擾
	 *	@param[in] key �L�[
	 *	@param[out] _rValue �l
	 *	@retval true �l�擾����
	 *	@retval false ���s�i�L�[���Ȃ��j
	 */
	bool Lookup(INK key, VAL& _rValue) const
	{
		EXCLUSIVE(this);
		const VAL* P = m_SearchKey(key);
		if ( P == NULL ){ return false; }
		_rValue = *P;
		return true;
	}

	/**
	 * [�m�F] �L�[�L��
	 *	@param key �L�[
	 *	@retval true ���݂���
	 *	@retval false ���݂��Ȃ�
	 */
	bool HasKey(INK key) const
	{
		EXCLUSIVE(this);
		return m_SearchKey(key) != NULL;
	}

	/**
	 * [�擾] �L�[�̈ꗗ
	 *	@return �L�[��Vector
	 */
	CVectorT<KEY> EnumKeys(void) const
	{
		EXCLUSIVE(this);
		CVectorT<KEY> v;
		v.Copy(GetKeysAdapter());
		return v;
	}
	
	/**
	 * [�m�F] ��r.
	 *		�R���p���[�^( IComparatorT �̃T�u�N���X)���g�p���āA�S�v�f���r���܂��B
	 *	@param c ��r�Ώ�
	 *	@retval true	������ c �Ɠ���
	 *	@retval false	������ c �ƈقȂ�
	 */
	virtual bool IsEqual(const IConstCollectionT<TNB::CPairT<KEY,VAL> >& c) const
	{
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
		const IComparatorT<VAL>& comparator = IComparatorT<VAL>::GetDefault();
		loop ( i, size )
		{
			const KEY& k = c.At(i).first;
			const VAL* p = GetValuePtr(k);
			if ( p == NULL )
			{
				return false;
			}
			const VAL& v = c.At(i).second;
			if ( ! comparator.IsEqualTo(v, *p) )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [�擾] �L�[�݂̂̎Q�ƃA�_�v�^�擾.
	 *		�L�[�̂ݎQ�ƃA�_�v�^�N���X���쐬���܂��B
	 *	@return CConstKeysAdapter
	 */
	CConstKeysAdapter GetKeysAdapter(void) const
	{
		return CConstKeysAdapter(this);
	}

	/**
	 * [�擾] �l�݂̂̎Q�ƃA�_�v�^�擾.
	 *		�l�̂ݎQ�ƃA�_�v�^�N���X���쐬���܂��B
	 *	@return CConstValuesAdapter
	 */
	CConstValuesAdapter GetValuesAdapter(void) const
	{
		return CConstValuesAdapter(this);
	}

private:
	friend class CMapTest;	///< �t�����h�N���X�錾
};



/**@ingroup COLLECT
 * ���������}�b�v�N���X.
 *	@par�K�v�t�@�C��
 *			TnbMap.h
 *	@date 06/01/01	�V�K
 */
typedef TNB::CMapT<CStr, CStr, LPCTSTR> CStrsMap;

/**@ingroup COLLECT
 * ���������}�b�v�N���X.
 *	@par�K�v�t�@�C��
 *			TnbMap.h
 *	@date 06/01/01	�V�K
 */
typedef TNB::CMapT<CAscii, CAscii, LPCSTR> CAsciisMap;



}; // TNB
