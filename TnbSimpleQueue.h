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



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �ȈՃ����O���L���[�^���Ǘ��e���v���[�g
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
 *	@par�K�v�t�@�C��
 *			TnbSimpleQueue.h
 * 
 *	@date 13/08/29 �V�K�쐬
 */
template<typename TYP, bool ISI = false, class INV = TYP>
class CSimpleQueueT : CCopyImpossible
{
public:

	/**
	 * �R���X�g���N�^
	 *	@note	�R���X�g���N�^���ɁA�����O�L���[�̍ő�T�C�Y���w�肵�܂��B
	 *			���I�ɑ傫���Ȃ�܂���B
	 *	@param size �����O�L���[�̃T�C�Y�B�ȗ������10000�ł��B
	 */
	explicit CSimpleQueueT(size_t size = 10000) : m_bufferSize(size), m_pBuffer(new TYP[size]), m_dataSize(0), m_readIndex(0)
	{
		ASSERT( size != 0 );
	}
	
	/**
	 * �f�X�g���N�^
	 */
	virtual ~CSimpleQueueT(void)
	{
		delete[] m_pBuffer;
	}

	/**
	 * [�ݒ�] �����O�L���[�T�C�Y�w��.
	 *	@note �R���X�g���N�^�ł��w��ł��܂��B
	 *	@note ���̃��\�b�h���g���ƁA���̓N���A����܂��B
	 *	@param size �����O�L���[�̃T�C�Y�B
	 */
	void SetQueueSize(size_t size)
	{
		ASSERT( size != 0 );
		delete[] m_pBuffer;
		m_bufferSize = size;
		m_pBuffer = new TYP[size];
		m_dataSize = 0;
		m_readIndex = 0;
	}

	/**
	 * [�擾] �����O�L���[�T�C�Y�擾.
	 *	@return �����O�L���[�̃T�C�Y�B
	 */
	size_t GetQueueSize(void) const
	{
		return m_bufferSize;
	}

	/**
	 * [�擾] �v�f���擾
	 *	@return �T�C�Y
	 */
	size_t GetSize(void) const 
	{
		return m_dataSize;
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
	INDEX Add(const TYP& t)
	{
		size_t r = GetSize();
		return (AddElements(1, &t) == 1) ? r : INVALID_INDEX;
	}

	/**
	 * [�폜] �f�[�^�S�폜.
	 *		�L���[�̓��e����ɂ��܂��B
	 *	@return ���true
	 */
	bool RemoveAll(void)
	{
		SetQueueSize(m_bufferSize);
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
	size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( (m_dataSize) + size > (m_bufferSize) )
		{
			//����ȏ�ɂȂ�Ȃ�G���[
			return INVALID_SIZE;
		}
		size_t bs = m_bufferSize;
		INDEX tmp = (m_readIndex + m_dataSize) % bs;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				m_pBuffer[tmp++] = *P++;
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
				m_pBuffer[tmp++] = TYP();
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		m_dataSize += size;
		return size;
	}

	/**
	 * [�擾] �擪�v�f���o��.
	 *		�L���[����폜���܂�
	 *	@return �v�f���e
	 *	@throw CEmptyException ��ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP Take(void)
	{
		TYP t;
		if ( TakeElements(1, &t) == 1 )
		{
			return t;
		}
		ASSERT0( false, "CSimpleQueueT::Take()", "�L���[����̎��擾���삳��܂����B" );
		throw CEmptyException();
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
	size_t TakeElements(size_t size, TYP* P = NULL)
	{
		if ( m_dataSize < size )
		{
			//�L���[�ɂ��̒����̏�񂪂Ȃ��̂ŃG���[
			return INVALID_SIZE;
		}
		size_t bs = m_bufferSize;
		if ( P != NULL || ISI )
		{
			INDEX tmp = m_readIndex;
			loop ( i, size )
			{
				if ( P != NULL )
				{
					*P++ = m_pBuffer[tmp];
				}
				if ( ISI )
				{
					m_pBuffer[tmp] = INV();
				}
				if ( ++tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		m_readIndex += size;
		m_readIndex %= bs;
		m_dataSize -= size;
		return size;
	}

	/**
	 * [�擾] �v�f�Q��
	 *	@param index �C���f�b�N�X
	 *	@return �Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& Ref(INDEX index) const
	{
		if ( GetSize() > index )
		{
			INDEX tmp = m_readIndex + index;
			tmp %= m_bufferSize;
			return m_pBuffer[tmp];
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [�擾] �v�f�A�h���X�擾
	 *	@param index �C���f�b�N�X
	 *	@return �Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP& Ref(INDEX index)
	{
		if ( GetSize() > index )
		{
			INDEX tmp = m_readIndex + index;
			tmp %= m_bufferSize;
			return m_pBuffer[tmp];
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [�擾] �i�[�\���擾.
	 *	@note Ring�L���[�̓����O�L���[�̃T�C�Y���Œ�ł��B
	 *	@return �T�C�Y
	 */
	size_t GetFreeSize(void) const
	{
		return (m_bufferSize) - (m_dataSize);
	}

private:

	size_t			m_bufferSize;		///< �o�b�t�@�̒���
	size_t			m_dataSize;		///< ���ۂ̃f�[�^�̒���
	INDEX			m_readIndex;		///< �Ǎ��݃|�W�V����
	TYP*			m_pBuffer;		///< �o�b�t�@
};



}; // TNB
