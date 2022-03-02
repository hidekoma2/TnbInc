#pragma once
/**
 *@file
 * �t�@�C���֌W�̃w�b�_
 *
 *		�ǂݍ��݃C���^�[�t�F�[�X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSerializer.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �ǂݍ��݃C���^�[�t�F�[�X
 *
 *	@note �ǂݍ��ݓ�����T�|�[�g����C���^�[�t�F�[�X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFile.h
 *
 *	@date 06/10/02 ���ۉ����ĐV�K�쐬
 *	@date 06/10/06 �ꕔ�̃��\�b�h����ύX�B
 */
struct IReader : IDeserializer
{
	/// �V�[�N�̃I�t�Z�b�g�w�胂�[�h
	enum ESeekMode
	{
		TOP		= FILE_BEGIN,		///< ������̃I�t�Z�b�g�w��
		CURRENT	= FILE_CURRENT,		///< ���݈ʒu����̃I�t�Z�b�g�w��
		END		= FILE_END			///< ������̃I�t�Z�b�g�w��
	};

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const = 0;

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void) = 0;

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const = 0;

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const = 0;

	/**
	 * [����] �V�[�N.
	 *		�A�N�Z�X�|�C���^���ړ����܂��B
	 *	@param llOffset �A�N�Z�X�|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�A�N�Z�X�|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const = 0;

	/**
	 * [�擾] �ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const = 0;

	/**
	 * [�擾] �c��T�C�Y�擾.
	 *		���݂̈ʒu����Ō�܂ł̃T�C�Y��Ԃ��܂��B
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	LONGLONG GetLeftoverSize(void) const
	{
		LONGLONG lc = Seek(0, CURRENT);
		LONGLONG le = GetSize();
		if ( lc >= 0 && le >= 0 )
		{
			return le - lc;
		}
		return -1;
	}

	/**
	 * [�擾] �ǂݍ���.
	 *	@note �w��T�C�Y�ǂ߂Ȃ��ƃG���[�ɂȂ�܂��B
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 *	@throw CShortOfDataException �w��T�C�Y�Ǎ��߂Ȃ��ƁA�X���[����܂��B
	 */
	void ReadExactly(size_t size, LPVOID _P) const
	{
		if ( ! CanRead() )
		{ 
			throw CReadFailureException(ERROR_INVALID_HANDLE); 
		}
		if ( Read(size, _P) != size )
		{
			throw CShortOfDataException();
		}
	}

	/**
	 * [�擾] �ǂݍ���
	 *	@note �w��T�C�Y�ǂ߂Ȃ��ƃG���[�ɂȂ�܂��B
	 *	@param size �ǂݍ��݃T�C�Y�B0�Ȃ�t�@�C���Ō�܂œǂݍ��ށB
	 *	@return ByteVector	�t�@�C�����e�i�T�C�Y�͓ǂݍ��߂��T�C�Y�j�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 *	@throw CShortOfDataException �w��T�C�Y�Ǎ��߂Ȃ��ƁA�X���[����܂��B
	 */
	CByteVector ReadExactly(size_t size = 0) const
	{
		CByteVector vb = ReadInto(size);
		if ( size > 0 && size != vb.GetSize() )
		{
			throw CShortOfDataException();
		}
		return vb;
	}

	/**
	 * [�擾] �ǂݍ���
	 *	@param size �ǂݍ��݃T�C�Y�B0�Ȃ�t�@�C���Ō�܂œǂݍ��ށB
	 *	@return ByteVector	�t�@�C�����e�i�T�C�Y�͓ǂݍ��߂��T�C�Y�j�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	CByteVector ReadInto(size_t size = 0) const
	{
		if ( ! CanRead() )
		{
			throw CReadFailureException(ERROR_INVALID_HANDLE); 
		}
		CByteVector vb;
		if ( size == 0 )
		{
			size = ToIndex(GetLeftoverSize());
		}
		if ( size > 0 )
		{
			size_t i = Read(size, vb.GetBuffer(size));
			vb.ReleaseBuffer();
			vb.SetSize(i);
		}
		return vb;
	}

	/**
	 * [�擾] �ǂݍ���.
	 *	@param[out] _out �Ǎ���B�N���A�����A���ɒǉ����܂��B
	 *	@param size �ǂݍ��݃T�C�Y�B�ȗ���0�Ȃ�t�@�C���Ō�܂œǂݍ��ށB
	 *	@param oneBlockSize ��x�ɓǂݍ��ރT�C�Y�B�ȗ��Ȃ�SK�B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	size_t ReadInto(ICollectionT<BYTE>& _out, size_t size = 0, size_t oneBlockSize = 4096) const
	{
		if ( ! CanRead() )
		{
			throw CReadFailureException(ERROR_INVALID_HANDLE); 
		}
		CByteVector vb;
		if ( size == 0 )
		{
			size = ToIndex(GetLeftoverSize());
		}
		size_t r = 0;
		while ( size > 0 )
		{
			size_t l = size;
			if ( l > oneBlockSize ){ l = oneBlockSize; }
			vb = ReadInto(l);
			l = vb.GetSize();
			if ( l == 0 ){ break; }
			_out.Append(vb);
			size -= l;
			r += l;
		}
		return r;
	}

	/**
	 * [�擾] �擾.
	 *		�w��f�[�^���擾���܂��B
	 *	@param[in] size �擾����f�[�^�T�C�Y(�P��BYTE)
	 *	@param[out] _P �擾����f�[�^
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void GetRaw(size_t size, LPVOID _P) const
	{
		ReadExactly(size, _P);
	}
};



}; //TNB