#pragma once
/**
 *@file
 * �Œ�o�b�t�@������Ǘ��֌W�̃w�b�_
 *
 *		�Œ�o�b�t�@��ŕ�������Ǘ�����N���X���L��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup STRING ITERATOR
 * �Œ�o�b�t�@������Ǘ��e���v���[�g
 *
 *		�Œ�o�b�t�@�ɕ�������i�[���Ǘ�����N���X�ł��B
 *		�ʐM�p�P�b�g��� NULL �I�[�̂Ȃ�������ȂǂɓK�p�ł��܂��B
 *
 *	@param LEN �o�b�t�@�̌Œ蒷
 *	@param PADDING �����񒷂�����Ȃ����ɕۊǂ��镶���B�ȗ������ ���O�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFixedBufferStr.h
 *
 *	@date 13/01/17	�V�K�쐬
 *	@date 16/02/18	CopyImpossible ���폜
 */
template<int LEN, char PADDING = 0>
class CFixedBufferStrT
{
public:

	/// �萔
	enum 
	{
		MAXIMUM_LENGTH = LEN,	///< �o�b�t�@��
		PADDING_CHAR = PADDING	///< �p�f�B���O����
	};

	/// �R���X�g���N�^.
	CFixedBufferStrT(void)
	{
		Empty();
	}

	/**
	 * ����R���X�g���N�^.
	 *	@param lpsz ������镶����
	 */
	CFixedBufferStrT(LPCSTR lpsz)
	{
		Set(lpsz);
	}

	/**
	 * ����I�y���[�^
	 *	@param other ������镶����
	 *	@return �����̎Q��
	 */
	CFixedBufferStrT& operator=(const CFixedBufferStrT& other)
	{
		Set(other.Get());
		return *this;
	}

	/**
	 * ����I�y���[�^
	 *	@param lpsz ������镶����
	 *	@return �����̎Q��
	 */
	CFixedBufferStrT& operator=(LPCSTR lpsz)
	{
		Set(lpsz);
		return *this;
	}

	/**
	 * [���] ��������.
	 *		�ێ��\�ȕ�����𒴂����ꍇ�A���������͔j������܂��B
	 *	@param lpsz ������镶����
	 */
	void Set(LPCSTR lpsz)
	{
		CAscii s;
		s.SetFromLeft(lpsz, LEN);
		ASSERT( s.GetLength() <= LEN );
		Empty();
		memcpy(m_dat, s, s.GetLength());
	}

	/**
	 * [�ݒ�] ��.
	 *		�ێ����Ă������S���A PADDING_CHAR �����ɂ��܂��B
	 */
	void Empty(void)
	{
		loop ( i, LEN )
		{
			m_dat[i] = PADDING_CHAR;
		}
	}

	/**
	 * [�擾] ������擾.
	 */
	CAscii Get(void) const
	{
		CAscii s;
		s.SetFromLeft(m_dat, LEN);
		return s;
	}

	/**
	 * [�Q��] ������擪�Q��.
	 *	@attention NULL�I�[����Ă��Ȃ�������̉\��������܂��B
	 *	@return ������̐擪
	 */
	LPCSTR Refer(void) const
	{
		return m_dat;
	}

	/**
	 * [�擾] ������.
	 *	@return ������
	 */
	size_t GetLength(void) const
	{
		CAscii s;
		s.SetFromLeft(m_dat, LEN);
		return s.GetLength();
	}

	/**
	 * [�擾] �ő啶����.
	 *		�i�[�ł���ő咷��Ԃ��܂��B
	 *	@return �ő啶����
	 */
	size_t GetMaximumLength(void) const
	{
		return LEN;
	}

private:
	char	m_dat[LEN];	///< �f�[�^
};



}; // TNB