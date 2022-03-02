#pragma once
/**
 *@file
 * �t�@�C���֌W�̃w�b�_
 *
 *		�������݃C���^�[�t�F�[�X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbReader.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �������݃C���^�[�t�F�[�X
 *
 *	@note �������ݓ�����T�|�[�g����C���^�[�t�F�[�X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbWriter.h
 *
 *	@date 06/10/02 ���ۉ����ĐV�K�쐬
 *	@date 06/10/06 �ꕔ�̃��\�b�h����ύX�B
 *	@date 06/12/19 IReader �C���^�[�t�F�C�X����B
 */
struct IWriter : IReader, ISerializer
{
	/**
	 * [�m�F] �����݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanWrite(void) const = 0;

	/**
	 * [�ۑ�] ��������
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void Write(size_t size, LPCVOID P) = 0;

	/**
	 * [�ݒ�] EOF���w��.
	 *		���݂̃t�@�C���|�C���^�̈ʒu��EOF�ɂ��܂��B
	 *	@throw CTnbException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void SetEnd(void) = 0;

	/**
	 * [�ۑ�] ��������.
	 *	@note �����ň�x CVectorT<BYTE> �ɕϊ����Ă��܂��B
	 *	@param c �������݃f�[�^�B
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	void WriteFrom(const IConstCollectionT<BYTE>& c)
	{
		if ( ! CanWrite() )
		{
			throw CWriteFailureException(ERROR_INVALID_HANDLE); 
		}
		CVectorT<BYTE> v;
		v.Copy(c);
		Write(v.GetSize(), v.ReferBuffer());
	}

	/**
	 * [�ۑ�] ��������.
	 *	@note reader �̌��݂̈ʒu����ǂݏo���A�������݂܂��B
	 *	@param reader �������݃f�[�^ �B
	 *	@param size �ǂݍ��݃T�C�Y�B�ȗ����O�Ȃ�t�@�C���Ō�܂œǂݍ��ށB
	 *	@param oneBlockSize ��x�ɓǂݍ��ރT�C�Y�B�ȗ��Ȃ�SK�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	void WriteFrom(const IReader& reader, size_t size = 0, size_t oneBlockSize = 4096)
	{
		if ( ! CanWrite() )
		{
			throw CWriteFailureException(ERROR_INVALID_HANDLE); 
		}
		CByteVector vb;
		if ( size == 0 )
		{
			size = ToIndex(reader.GetLeftoverSize());
		}
		while ( size > 0 )
		{
			size_t l = size;
			if ( l > oneBlockSize ){ l = oneBlockSize; }
			vb = reader.ReadInto(l);
			l = vb.GetSize();
			if ( l == 0 ){ break; }
			Write(vb.GetSize(), vb.ReferBuffer());
			size -= l;
		}
	}

	/**
	 * [�ǉ�] �ǉ�.
	 *		�w��f�[�^��ǉ����܂��B
	 *	@param size �ǉ�����f�[�^�T�C�Y(�P��BYTE)
	 *	@param P �ǉ�����f�[�^
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void AddRaw(size_t size, LPCVOID P)
	{
		Write(size, P);
	}
};



}; // TNB
