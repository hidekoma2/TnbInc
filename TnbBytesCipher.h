#pragma once
/**
 *@file
 * �Í����֌W�̃w�b�_
 *
 *		�Í����A�������A���T�|�[�g
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbSlideCompressor.h"



//TNB Library
namespace TNB
{



/**@ingroup CIPHER
 * ���Í������N���X
 *
 *		�ȈՓI�ȈÍ���/�����������邱�Ƃ��o���܂��B
 *		���k���\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBytesCipher.h
 *
 *	@date 06/07/12 �V�K�쐬
 *	@date 06/08/10 �o�͐�����ۉ��w��ł���悤�ɂ����B
 *	@date 06/11/15 �G���[���������B
 *	@date 07/05/10 �N���X���ύX�B
 */
class CBytesCipher
{
	CAscii		m_ascKey;					///< �Í��A�����̃L�[
	bool		m_boIsCompressionEnable;	///< ���k�L��

	/**
	 * �e�f�[�^��ADD/SUB�ϊ�
	 *	@param[in,out] _out �ϊ������f�[�^��ǉ����܂��B
	 *	@param[in] in �ϊ����f�[�^�B
	 *	@param[in] boIsAdd true �L�[���𑫂��B false �L�[���������B
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 */
	bool m_Add(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in, bool boIsAdd = true)
	{
		if ( m_ascKey.IsEmpty() )
		{
			ASSERT0(false, "CBytesCipher", "�Í��A�����L�[���w�肳��Ă��܂���B");
			throw CEmptyException();
		}
		bool r = true;
		const BYTE* B = reinterpret_cast<const BYTE*>(LPCSTR(m_ascKey));
		size_t iKeySize = m_ascKey.GetLength();
		size_t base = _out.GetSize();
		_out.SetSize(base + in.GetSize());
		if ( boIsAdd )
		{
			loop ( i, in.GetSize() )
			{
				r &= _out.Set(base + i, static_cast<BYTE>(in.At(i) + B[i % iKeySize]));
			}
		}
		else
		{
			loop ( i, in.GetSize() )
			{
				r &= _out.Set(base + i, static_cast<BYTE>(in.At(i) - B[i % iKeySize]));
			}
		}
		return r;
	}

public:

	/// �R���X�g���N�^
	CBytesCipher(void) : m_boIsCompressionEnable(false)
	{
	}

	/**
	 * [�ݒ�] �L�[�ݒ�.
	 *		�Í��A�����Ɏg�p����L�[���w�肵�܂��B���R�A�Í��Ɠ����L�[�łȂ��ƕ����ł��܂���B
	 *	@note �����ł� ASCII �ɕϊ�����g�p����܂��B
	 *	@param ascKey �Í��L�[
	 */
	void SetKey(const CAscii& ascKey)
	{
		m_ascKey = ascKey;
	}

	/**
	 * [�ݒ�] ���k���[�h.
	 *		�Í����Ɉ��k���邩�w�肵�܂��B�f�t�H���g�� disable�ł��B
	 *		�Í��Ɠ������[�h�łȂ��ƕ����ł��܂���B
	 *	@param boIsEnable true ���w�肷��ƈ��k���[�h����ł�(�Í��f�[�^�����f�[�^�ƈقȂ�T�C�Y�ɂȂ�)�B<BR>
	 *					false �Ȃ�Í������邾���ł��i�Í��f�[�^�����f�[�^�Ɠ����T�C�Y�ɂȂ�j�B
	 */
	void Compression(bool boIsEnable)
	{
		m_boIsCompressionEnable = boIsEnable;
	}

	/**
	 * [�ݒ�] �Í���
	 *	@param[out] _out �Í��f�[�^�i�[��B�ǉ�����܂��̂ŁA�ʏ�Empty��Ԃ̃R���N�V�������w�肷��悤�ɂ��܂��B
	 *	@param[in] in ���f�[�^
	 *	@retval true �G���R�[�h�����B
	 *	@retval false ���s�B
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 *	@throw CTnbException �w��R���N�V�����̍\���ɂ��A�Ǎ��ݏ����ŃX���[�����\��������܂��B
	 */
	bool Encode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in)
	{
		EXCLUSIVE2(&_out, &in);
		bool r = false;
		if ( m_boIsCompressionEnable )
		{
			CSlideCompressor comp;
			CByteVector vb;
			if ( comp.Encode(vb, in) )
			{
				r = m_Add(_out, vb);
			}
		}
		else
		{
			r = m_Add(_out, in);
		}
		return r;
	}

	/**
	 * [�ݒ�] �Í���
	 *	@param c ���f�[�^
	 *	@return �Í��f�[�^
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 *	@throw CTnbException �w��R���N�V�����̍\���ɂ��A�Ǎ��ݏ����ŃX���[�����\��������܂��B
	 */
	CByteVector Encode(const IConstCollectionT<BYTE>& c)
	{
		CByteVector vb;
		if ( ! Encode(vb, c) )
		{
			vb.Invalid();
		}
		return vb;
	}

	/**
	 * [�ݒ�] ������
	 *	@param[out] _out �Í��f�[�^�i�[��B�ǉ�����܂��̂ŁA�ʏ�Empty��Ԃ̃R���N�V�������w�肷��悤�ɂ��܂��B
	 *	@param[in] in �Í��f�[�^
	 *	@retval true �f�R�[�h�����B
	 *	@retval false ���s�B
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 *	@throw CTnbException �w��R���N�V�����̍\���ɂ��A�Ǎ��ݏ����ŃX���[�����\��������܂��B
	 */
	bool Decode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in)
	{
		EXCLUSIVE2(&_out, &in);
		bool r = false;
		if ( m_boIsCompressionEnable )
		{
			CSlideCompressor comp;
			CByteVector vb;
			if ( m_Add(vb, in, false) )
			{
				r = comp.Decode(_out, vb);
			}
		}
		else
		{
			r = m_Add(_out, in, false);
		}
		return r;
	}

	/**
	 * [�ݒ�] ������
	 *	@param c �Í��f�[�^
	 *	@return ���f�[�^�BInvalid��ԂȂ�G���[(���k�W�J�G���[�������̉\���L��)�B
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 *	@throw CTnbException �w��R���N�V�����̍\���ɂ��A�Ǎ��ݏ����ŃX���[�����\��������܂��B
	 */
	CByteVector Decode(const IConstCollectionT<BYTE>& c)
	{
		CByteVector vb;
		if ( ! Decode(vb, c) )
		{
			vb.Invalid();
		}
		return vb;
	}
};



};//TNB
