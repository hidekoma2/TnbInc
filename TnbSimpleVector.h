#pragma once
/**
 *@file
 * �ȈՔz��^���Ǘ��֌W�̃w�b�_
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * �ȈՔz��^���Ǘ��e���v���[�g
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *
 *	@note ��x�����m�ۂ����v�f�́ASetSize() �� Remove() �Ō��炵�Ă��A�m�ۂ��ꂽ�܂܂ɂȂ�܂��B
 *		���̍ہA���̗v�f�͔j������Ȃ��i�f�X�g���N�^�������Ȃ��j���Ƃɒ��ӁB
 *		SetSize() �� RemoveAll() �A�f�X�g���N�^�ŁA�S�J������܂��B
 *
 *	@see CVectorT 
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@par�K�v�t�@�C��
 *			TnbSimpleVector.h
 *
 *	@date 07/12/01 �V�K�쐬.
 *	@date 08/08/22 �R�s�[�n��ǉ�.
 *	@date 09/04/22 �C�e���[�^��p�~.
 *	@date 09/08/28 SetSize() �̑��x���P�B
 *	@date 09/10/27 Find() �ǉ��B
 *	@date 10/03/03 IsEmpty() , operator size_t() �ǉ��B
 *	@date 11/09/07 SetSize() �̓�������P
 *	@date 19/07/07 �T�C�Y�w��̃R���X�g���N�^�ǉ�
 */
template<typename TYP>
class CSimpleVectorT
{
public:
	/// �R���X�g���N�^
	explicit CSimpleVectorT(void) : m_bufferSize(0), m_dataSize(0), m_pBuffer(NULL)
	{
	}

	/// �R���X�g���N�^
	/// @param bufferSize �o�b�t�@�T�C�Y
	explicit CSimpleVectorT(size_t bufferSize) : m_bufferSize(bufferSize), m_dataSize(0)
	{
		m_pBuffer = new TYP[m_bufferSize];
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[���B
	 */
	CSimpleVectorT(const CSimpleVectorT& other) : m_bufferSize(0), m_dataSize(0), m_pBuffer(NULL)
	{
		operator=(other);
	}

	/// �f�X�g���N�^
	virtual ~CSimpleVectorT(void)
	{
		RemoveAll();
	}

	/**
	 * [����] �R�s�[�I�y���[�^
	 *	@param other �R�s�[���B
	 *	@return �����̎Q��
	 */
	CSimpleVectorT& operator=(const CSimpleVectorT& other)
	{
		size_t l = other.GetSize();
		SetSize(l);
		loop ( i, l )
		{
			Set(i, other.At(i));
		}
		return *this;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& At(INDEX index) const
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP& At(INDEX index) 
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& operator[](INDEX index) const
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƁB���삷��ƃI�u�W�F�N�g���̗v�f�ɉe�����L��܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP& operator[](INDEX index)
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}
	
	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	bool Set(INDEX index, const TYP& t)
	{
		if ( m_dataSize <= index ) { return false; }
		m_pBuffer[index] = t;
		return true;
	}

	/**
	 * [�m�F] INDEX�̗L���m�F.
	 *	@param index �C���f�b�N�X
	 *	@retval true	�w��Index�͗L���B
	 *	@retval false	�w��Index�͖���(�͈͊O)�B
	 */
	bool IsInRange(INDEX index) const
	{
		return TNB::IsInRange(index, GetSize());
	}

	/**
	 * [�m�F] �v�f�̗L���m�F.
	 *	@note GetSize()�̖߂�l���O���ۂ��Ń`�F�b�N���Ă��܂��B
	 *	@retval true	�v�f�Ȃ�
	 *	@retval false	�v�f�L��
	 */
	bool IsEmpty(void) const
	{
		return m_dataSize == 0;
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@return �{�C���X�^���X���Ǘ����Ă���f�[�^�̒����B
	 */
	size_t GetSize(void) const
	{
		return m_dataSize;
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@return �{�C���X�^���X���Ǘ����Ă���f�[�^�̒����B
	 */
	operator size_t(void) const
	{
		return m_dataSize;
	}

	/**
	 * [�擾] �o�b�t�@�擾
	 *	@return �{�C���X�^���X���Ǘ����Ă���o�b�t�@�A�h���X�B
	 */
	TYP* ReferBuffer(void)
	{
		return m_pBuffer;
	}

	/**
	 * [�擾] �o�b�t�@�擾
	 *	@return �{�C���X�^���X���Ǘ����Ă���o�b�t�@�A�h���X�B
	 */
	const TYP* ReferBuffer(void) const
	{
		return m_pBuffer;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�
	 *	@param s �T�C�Y�B
	 */
	void SetSize(size_t s)
	{
		if ( s == 0 )
		{
			RemoveAll();
		}
		else if ( s > GetSize() )
		{
			if ( m_bufferSize < s )
			{
				m_bufferSize = m_GetSecureSize(s + 1);
				TYP* P = new TYP[m_bufferSize];
				loop ( i, m_dataSize )
				{
					P[i] = m_pBuffer[i];
				}
				if ( m_pBuffer != NULL )
				{
					delete[] m_pBuffer;
				}
				m_pBuffer = P;
			}
		}
		m_dataSize = s;
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *		���C���X�^���X�ŊǗ����Ă���o�b�t�@�Ɏw��̈�f�[�^��A�����܂��B
	 *	@param t �v�f�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX Add(const TYP& t)
	{
		if ( m_bufferSize <= m_dataSize )
		{
			m_bufferSize = m_GetSecureSize(m_bufferSize + 1);
			TYP* P = new TYP[m_bufferSize];
			loop ( i, m_dataSize )
			{
				P[i] = m_pBuffer[i];
			}
			if ( m_pBuffer != NULL )
			{
				delete[] m_pBuffer;
			}
			m_pBuffer = P;
		}
		m_pBuffer[m_dataSize] = t;
		m_dataSize += 1;
		return m_dataSize - 1;
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@param index �C���f�b�N�X
	 *	@retval false	�G���[�B
	 *	@retval true	�����B
	 */
	bool Remove(INDEX index)
	{
		if ( IsInRange(index) ) 
		{
			for ( INDEX i = index ; i < m_dataSize - 1; i++ )
			{
				m_pBuffer[i] = m_pBuffer[i + 1];
			}
			m_dataSize--;
			return true;
		}
		return false;
	}

	/**
	 * [�폜] ��
	 *	@note ���C���X�^���X�ŊǗ����Ă���o�b�t�@����ɂ��܂��B
	 */
	void RemoveAll(void)
	{
		m_bufferSize = 0;
		m_dataSize = 0;
		if ( m_pBuffer != NULL )
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
	}

	/**
	 * [����] ����.
	 *	@param t �����v�f.
	 *	@retval INVALID_INDEX ������.
	 *	@retval ��L�ȊO �����B�l�́A�C���f�b�N�X�B
	 */
	INDEX Find(const TYP& t) const
	{
		loop ( i, m_dataSize )
		{
			if ( m_pBuffer[i] == t )
			{
				return i;
			}
		}
		return INVALID_INDEX;
	}

private:
	size_t	m_bufferSize;	///< �o�b�t�@�̒���
	size_t	m_dataSize;		///< ���ۂ̃f�[�^�̒���
	TYP *	m_pBuffer;		///< �o�b�t�@
	/// �m�ۂ��ׂ��T�C�Y�擾
	size_t m_GetSecureSize(size_t size)
	{
		size_t marginSize = size / 8;
		marginSize = (marginSize < 4) ? 4 : ((marginSize > 1024) ? 1024 : marginSize);
		return size + marginSize;
	}
};



}; // TNB

