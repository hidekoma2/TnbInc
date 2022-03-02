#pragma once
/**
 *@file
 * �Í�������֌W�̃w�b�_
 *
 *		�Í����A�������A���T�|�[�g
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup CIPHER STRING
 * ������Í������N���X
 *
 *		�ȈՓI�ȈÍ���/�����������邱�Ƃ��o���܂��B
 *		�Í������Ă�������ɂȂ��Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbStringCipher.h
 *
 *	@date 06/07/21	�V�K�쐬
 *	@date 12/04/19	Encode() , Decode() �� const ���\�b�h�ɕύX�B
 */
class CStringCipher
{
public:

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
	 * [�ݒ�] �Í���
	 *	@param lpsz ��������B
	 *	@return �Í�������B
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 */
	CAscii Encode(LPCSTR lpsz) const
	{
		return m_Shift(lpsz, true);
	}

	/**
	 * [�ݒ�] ������
	 *	@param lpsz �Í�������B
	 *	@return ��������B
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 */
	CAscii Decode(LPCSTR lpsz) const
	{
		return m_Shift(lpsz, false);
	}

private:
	/**
	 * �͈̓V�t�g.
	 *		�w����e�ŕύX�l�ɂ��R�[�h���V�t�g���܂��B
	 *	@param c ���R�[�h�B
	 *	@param iShift �V�t�g�l
	 *	@param iMin �V�t�g�Ώ۔͈�(�ŏ��l)
	 *	@param iMax �V�t�g�Ώ۔͈�(�ő�l)
	 *	@retval -1 ���R�[�h�� iMin�`iMax �łȂ��ꍇ�B
	 *	@return 0�ȏ� �V�t�g�����R�[�h�B
	 */
	static int ms_RangeShift(int c, int iShift, int iMin, int iMax)
	{
		ASSERTLIB(iMin < iMax);
		if ( c < iMin || c > iMax )
		{
			return -1;
		}
		int iRange = iMax - iMin;
		if ( iShift >= 0 )
		{
			iShift %= iRange;
		}
		else
		{
			iShift = -((-iShift) % iRange);
		}
		//
		int iCode = (c - iMin) + iShift;
		while ( iCode < 0x0000 )
		{
			iCode += iRange;
		}
		while ( iCode >= iRange )
		{
			iCode -= iRange;
		}
		ASSERTLIB( (iCode + iMin) >= iMin && (iCode + iMin) < iMax ); 
		return iCode + iMin;
	}

	/**
	 * �R�[�h�V�t�g.
	 *		�ύX�l�ɂ��R�[�h���V�t�g���܂��B
	 *	@param c ���R�[�h
	 *	@param iShift �V�t�g�l
	 *	@return �V�t�g�����R�[�h�B
	 *			�������A���R�[�h�� 0x20�`0x80, 0xA0�`0xDF, 0xE0�`0xFB �łȂ��ꍇ�A���R�[�h�̂܂܂ɂȂ�B
	 */
	static BYTE ms_ShiftChar(BYTE c, int iShift)
	{
		int r = ms_RangeShift(c, iShift, 0x20, 0x80); 
		if ( r < 0 ) { r = ms_RangeShift(c, iShift, 0x81, 0x9F); }
		if ( r < 0 ) { r = ms_RangeShift(c, iShift, 0xA0, 0xDF); }
		if ( r < 0 ) { r = ms_RangeShift(c, iShift, 0xE0, 0xFB); }
		return ( r < 0 ) ? c : static_cast<BYTE>(r);
	}

	/**
	 * ������V�t�g.
	 *	@param lpsz ��������
	 *	@param boIsEnc true�Ȃ�Í����Afalse�Ȃ畡����
	 *	@return ������̕�����B
	 *	@throw CEmptyException �L�[���ݒ肳��Ă��Ȃ��ƃX���[����܂��B
	 */
	CAscii m_Shift(LPCSTR lpsz, bool boIsEnc) const
	{
		if ( m_ascKey.IsEmpty() )
		{
			ASSERT0(false, "CCipherData", "�Í��A�����L�[���w�肳��Ă��܂���B");
			throw CEmptyException();
		}
		const BYTE* B = reinterpret_cast<const BYTE*>(LPCSTR(m_ascKey));
		size_t iKeySize = m_ascKey.GetLength();
		CAscii str;
		size_t iLen = STRLIB::GetLen(lpsz);
		if ( iLen > 0 )
		{
			char* P = str.GetBuffer(iLen);
			if ( boIsEnc )
			{
				loop ( i, iLen )
				{
					P[i] = ms_ShiftChar(lpsz[i], B[i % iKeySize]);
				}
			}
			else
			{
				loop ( i, iLen )
				{
					P[i] = ms_ShiftChar(lpsz[i], -B[i % iKeySize]);
				}
			}
			P[iLen] = 0;
			str.ReleaseBuffer();
		}
		return str;
	}

	CAscii		m_ascKey;				///< �Í��A�����̃L�[
};



};//TNB
