#pragma once
/**
 *@file
 * �t�@�C���֌W�̃w�b�_
 *
 *		 �t�@�C�����������Ǘ��N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbWriter.h"



//TNB Library
namespace TNB
{


/**@ingroup FILE MEMORY
 * �t�@�C�����������Ǘ��N���X
 *
 *	 ���������t�@�C���Ɍ����Ă�
 *
 *	@par�K�v�t�@�C��
 *			TnbLikenMemToFile.h
 *
 *	@date 06/10/06 �V�K�쐬
 */
class CLikenMemToFile : public IWriter, CCopyImpossible
{
	const BYTE*					m_pbcConstPtr;		///< �O���������Q�Ɨp
	BYTE*						m_pbPtr;			///< �O���������A�N�Z�X�p
	LONGLONG					m_llSize;			///< �O���������T�C�Y
	mutable CAutoVectorT<BYTE>	m_vbContent;		///< �ϗL���p�o�b�t�@
	mutable LONGLONG			m_llPos;			///< �A�N�Z�X�|�C���^

public:

	/**
	 * �R���X�g���N�^.
	 *	@note	�w��T�C�Y�̉ω\�ȃI�u�W�F�N�g���쐬���܂��B
	 *	@param size �T�C�Y�B
	 */
	CLikenMemToFile(size_t size = 0)
		: m_pbcConstPtr(NULL)
		, m_pbPtr(NULL)
		, m_llSize(size)
		, m_llPos(0)
	{
		m_vbContent.SetSize(size);
	}

	/**
	 * �R���X�g���N�^.
	 *	@note	�w��f�[�^�́A�ǂݍ��ݐ�p�̃I�u�W�F�N�g���쐬���܂��B
	 *	@param size �f�[�^�T�C�Y�B
	 *	@param P �f�[�^���e�B
	 */
	CLikenMemToFile(size_t size, LPCVOID P)
		: m_pbcConstPtr(static_cast<const BYTE*>(P))
		, m_pbPtr(NULL)
		, m_llSize(size)
		, m_llPos(0)
	{
	}

	/**
	 * �R���X�g���N�^.
	 *	@note	�w��f�[�^�̕ύX�\�̃I�u�W�F�N�g���쐬���܂��B
	 *			�ύX�͏o���܂���B
	 *	@param size �f�[�^�T�C�Y�B
	 *	@param P �f�[�^���e�B
	 */
	CLikenMemToFile(size_t size, LPVOID P)
		: m_pbcConstPtr(NULL)
		, m_pbPtr(static_cast<BYTE*>(P))
		, m_llSize(size)
		, m_llPos(0)
	{
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const { return _T("-"); }

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void) {}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return true;
	}

	/**
	 * [�m�F] �����݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanWrite(void) const
	{
		return m_pbcConstPtr == NULL;
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		if ( m_pbcConstPtr != NULL || m_pbPtr != NULL )
		{
			return m_llSize;
		}
		return m_vbContent.GetSize();
	}

	/**
	 * [����] �V�[�N.
	 *		�A�N�Z�X�|�C���^���ړ����܂��B
	 *	@param llOffset �A�N�Z�X�|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�A�N�Z�X�|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		LONGLONG l = GetSize();
		LONGLONG p;
		switch ( eSeek )
		{
		case TOP:
			p = llOffset;
			break;
		case END:
			p = l + llOffset;
			break;
		default:
			p = m_llPos + llOffset;
			break;
		}
		if ( p < 0 )
		{
			return -1;
		}
		else if ( p > l )
		{
			if ( m_pbcConstPtr == NULL && m_pbPtr == NULL )
			{
				m_vbContent.SetSize(ToInt(p));
			}
			else
			{
				return -1;
			}
		}
		m_llPos = p;
		return p;
	}

	/**
	 * [�擾] �ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		const BYTE* B = NULL;
		if ( m_pbcConstPtr != NULL )
		{
			B = m_pbcConstPtr;
		}
		else if ( m_pbPtr != NULL )
		{
			B = m_pbPtr;
		}
		else
		{
			B = m_vbContent.ReferBuffer();
		}
		LONGLONG s = size;
		if ( s > GetLeftoverSize() )
		{
			s = GetLeftoverSize();
		}
		BYTE* Q = static_cast<BYTE*>(_P);
		MemCopy(Q, &B[m_llPos], ToInt(s));
		m_llPos += s;
		return ToInt(s);
	}

	/**
	 * [�ۑ�] ��������
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@throw CWriteFailureException �R���X�g���N�^ throw ��L���ɂ��Ă��āA�����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		if ( m_pbcConstPtr != NULL )
		{
			throw CWriteFailureException(ERROR_ACCESS_DENIED);
		}
		if ( m_pbPtr != NULL )
		{
			if ( static_cast<LONGLONG>(size) >= GetLeftoverSize() )
			{
				throw CWriteFailureException(ERROR_ACCESS_DENIED);
			}
			MemCopy(&m_pbPtr[m_llPos], static_cast<const BYTE*>(P), size);
		}
		else
		{
			if ( m_llPos + size > m_vbContent.GetSize() )
			{
				m_vbContent.SetSize(ToInt(m_llPos + size));
			}
			const BYTE* B = static_cast<const BYTE*>(P);
			loop ( i, size )
			{
				m_vbContent.Set(ToInt(m_llPos + i), *B++);
			}
		}
		m_llPos += size;
	}

	/**
	 * [�ݒ�] EOF���w��.
	 *		���݂̃t�@�C���|�C���^�̈ʒu��EOF�ɂ��܂��B
	 *	@throw CTnbException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void SetEnd(void)
	{
		if ( m_pbcConstPtr != NULL && m_pbPtr != NULL )
		{
			throw CWriteFailureException(ERROR_ACCESS_DENIED);
		}
		m_vbContent.SetSize(ToInt(m_llPos));
	}
};



}; // TNB


