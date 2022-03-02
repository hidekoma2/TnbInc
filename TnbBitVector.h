#pragma once
/**
 *@file
 * �z��^bit���Ǘ��֌W�̃w�b�_
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �z��^bit���Ǘ��e���v���[�g
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *	
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@note ICollectionT �� TYP �� bool �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitVector.h
 *
 *	@date 06/09/01 �V�K
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
class CBitVector : public ICollectionT<bool>
{
	DEFSUPER(ICollectionT<bool>);
	CVectorT<BYTE>			m_vbData;		///< �Ǘ��p�f�[�^
	size_t					m_dataSize;		///< ���ۂ̃f�[�^�T�C�Y
	CSyncSection			m_syncFunc;		///< �Q�Ɣr��
	mutable bool			m_boTemp;		///< �Q�Ɨp

	/** 
	 * �w��ʒu����w�萔�܂ł��E�V�t�g���܂��B�������Ƃ���́A�O�ɂȂ�܂��B
	 *	ex) �w��ʒu 4
	 *		0 1001 0110 1110
	 *			��
	 *		0 0100 1011 1110
	 *	@param startIndex �J�n��INDEX
	 *	@param B �f�[�^
	 *	@param len �f�[�^BYTE��
	 */
	static void ms_ShiftRight(INDEX startIndex, BYTE* B, size_t len)
	{
		bool boLastBit = false;
		BYTE bTop = B[startIndex / 8];
		for ( INDEX i = len - 1; i >= startIndex / 8; i-- )
		{
			bool b = (B[i] & _BIT(0)) != 0;
			B[i] >>= 1;
			if ( boLastBit )
			{
				B[i] |= _BIT(7);
			}
			boLastBit = b;
		}
		INDEX pos = startIndex & 7;
		if ( pos != 0 )
		{
			BYTE bMask = static_cast<BYTE>(_BIT(pos) - 1);
			bTop &= bMask;
			B[startIndex / 8] &= ~bMask;
			B[startIndex / 8] |= bTop;
		}
	}

	/** 
	 * �w��ʒu����w�萔�܂ł����V�t�g���܂��B�������Ƃ���́A�O�ɂȂ�܂��B
	 *	ex) �w��ʒu 8
	 *		0 1001 0110 1110
	 *			��
	 *		1 0010 1100 1110
	 *	@param startIndex �J�n��INDEX
	 *	@param B �f�[�^
	 *	@param len �f�[�^BYTE��
	 */
	static void ms_ShiftLeft(INDEX startIndex, BYTE* B, size_t len)
	{
		bool boLastBit = false;
		BYTE bTop = B[startIndex / 8];
		for ( INDEX i = startIndex / 8; i < len; i++ )
		{
			bool b = (B[i] & _BIT(7)) != 0;
			B[i] <<= 1;
			if ( boLastBit )
			{
				B[i] |= _BIT(0);
			}
			boLastBit = b;
		}
		INDEX pos = startIndex & 7;
		if ( pos != 0 )
		{
			BYTE bMask = static_cast<BYTE>(_BIT(pos) - 1);
			bTop &= bMask;
			B[startIndex / 8] &= ~bMask;
			B[startIndex / 8] |= bTop;
		}
	}

public:

	/**@ingroup COLLECT ITERATOR
	 * �z��^bit���Ǘ��̎Q�ƃN���X
	 *
	 *		BIT�𑀍삷�邽�߂̃N���X�ł��B
	 *
	 *	@note	CBitVector�ō쐬�������̂�Z���Ŏg�p���邱�Ƃ�z�肵�Ă��邽�߁A
	 *			�R���X�g���N�^�� private �錾���Ă��܂��B
	 *	
	 *	@see CBitVector::operator[]()
	 *
	 *	@date 06/09/01	�V�K�쐬
	 */
	class CRef
	{
		friend class CBitVector;
		CBitVector* m_vb;
		INDEX		m_index;
		/// �R���X�g���N�^
		CRef(const CBitVector* P, INDEX i)
		{
			m_vb = const_cast<CBitVector*>(P);
			m_index = i;
		}
	public:
		/**
		 * [�擾] �f�[�^�擾
		 *	@return �f�[�^�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 *	@throw CReadFailureException �t�@�C���ǂݍ��݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
		 */
		operator bool(void) const
		{
			return m_vb->At(m_index);
		}
		/**
		 * [�ݒ�] �f�[�^�ݒ�
		 *	@param b �������ރf�[�^�B
		 *	@return �������񂾃f�[�^�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 *	@throw CWriteFailureException �t�@�C���������݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
		 */
		bool operator=(bool b)
		{
			m_vb->Set(m_index, b);
			return b;
		}
	};

	
	//--------------------------------------


	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// const_iterator�^�錾
	typedef CConstIteratorBaseT<CBitVector, bool> const_iterator;
	// iterator�錾
	class iterator
	{
		_ITERATORCORE(iterator, CBitVector)
	public:
		CRef operator*(void)
		{
			return (*m_V)[m_ind];
		}
		CRef operator[](int i)
		{
			return (*m_V)[m_ind+i];
		}
	};
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
	 * [����] �}��
	 *	@param ite �}������ʒu�� iterator
	 *	@param t �v�f
	 *	@return �}�������ꏊ�� iterator
	 */
	iterator insert(iterator ite, bool t = false)
	{
		INDEX i = ite.GetIndex();
		Insert(i, t);
		return iterator(this, i);
	}

	/**
	 * [����] �擪�ɑ}��
	 *	@param t �v�f
	 */
	void push_front(bool t) { Insert(0, t); }


	//------------------------


	/**
	 * �R���X�g���N�^
	 *	@note �w��̃������m�ێ��̒ǉ��T�C�Y��ݒ肵�܂��B
	 *	@note �f�[�^�͕ێ����܂���
	 *	@param size �������m�ێ��̒ǉ��T�C�Y�B SetIncrementSize() �Ɠ����ł��B
	 */
	explicit CBitVector(size_t size = 0) : _super(), m_vbData(size) 
	{
		m_dataSize = 0;
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@note �R�s�[���ƃo�b�t�@�����L���܂��B
	 *	@param other �R�s�[���� CVectorT �C���X�^���X
	 */
	CBitVector(const CBitVector& other) : _super(), m_vbData(other.m_vbData)
	{
		m_dataSize = other.m_dataSize;
	}
	
	/** �f�X�g���N�^ */
	virtual ~CBitVector(void)
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@���J�����A	
	 *		������̏������L����悤�ɂȂ�܂��B
	 *	@param other ����� 
	 *	@return �����̎Q��
	 */
	CBitVector& operator=(const CBitVector& other)
	{
		m_vbData = other.m_vbData;
		m_dataSize = other.m_dataSize;
		return *this;
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
		m_vbData.SetIncrementSize(size);
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
		return m_dataSize;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �ꎞ�I�ȎQ��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const bool& At(INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		m_boTemp = (m_vbData[index / 8] & _BIT(index & 7)) != 0;
		return m_boTemp;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@attention �T�|�[�g�ł��܂���B operator[] ���g�p���܂��B
	 *	@param index �C���f�b�N�X
	 *	@return -
	 *	@throw CNotSupportException �T�|�[�g���Ă��Ȃ����Ƃ��������߂ɃX���[����܂��B
	 */
	virtual bool& Ref(INDEX index)
	{
		throw CNotSupportException();
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
	virtual bool Set(INDEX index, const bool& t)
	{
		if ( ! IsInRange(index) )
		{
			return false;
		}
		if ( t )
		{
			m_vbData[index / 8] |= _BIT(index & 7);
		}
		else
		{
			m_vbData[index / 8] &= ~_BIT(index & 7);
		}
		return true;
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@�Ɏw��̈�f�[�^��A�����܂��B
	 *	@param t �v�f�B
	 *	@return �ǉ�����INDEX
	 */
	virtual INDEX Add(const bool& t)
	{
		if ( m_dataSize == 0 || (m_dataSize & 7) == 7 )
		{
			m_vbData.Add(0);
		}
		if ( t )
		{
			m_vbData[m_dataSize / 8] |= _BIT(m_dataSize & 7);
		}
		else
		{
			m_vbData[m_dataSize / 8] &= ~_BIT(m_dataSize & 7);
		}
		return m_dataSize++;
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@param index �C���f�b�N�X
	 *	@retval true	�폜����
	 *	@retval false	���s
	 */
	virtual bool Remove(INDEX index)
	{
		if ( ! IsInRange(index) )
		{
			return false;
		}
		m_dataSize--;
		ms_ShiftRight(index, m_vbData.GetBuffer(), m_vbData.GetSize());
		m_vbData.ReleaseBuffer();
		return true;
	}

	/**
	 * [�폜] ��
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@����ɂ��܂��B
	 *	@return ���true�B
	 */
	virtual bool RemoveAll(void)
	{
		m_vbData.RemoveAll();
		m_dataSize = 0;
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
	bool Insert(INDEX index, const bool& t)
	{
		if ( index == m_dataSize )
		{
			return IsValid(Add(t));
		}
		if ( ! IsInRange(index) )
		{
			return false;
		}
		if ( m_dataSize == 0 || (m_dataSize & 7) == 7 )
		{
			m_vbData.Add(0);
		}
		ms_ShiftLeft(index, m_vbData.GetBuffer(), m_vbData.GetSize());
		m_vbData.ReleaseBuffer();
		if ( t )
		{
			m_vbData[index / 8] |= _BIT(index & 7);
		}
		else
		{
			m_vbData[index / 8] &= ~_BIT(index & 7);
		}
		m_dataSize++;
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
	const CRef operator[](INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		return CRef(this, index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB���삷��ƃI�u�W�F�N�g���̗v�f�ɉe�����L��܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	CRef operator[](INDEX index)
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		return CRef(this, index);
	}

private:
	friend class CBitVectorTest;	///<�t�����h�N���X�錾
};



}; // TNB

