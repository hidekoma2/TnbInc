#pragma once
/**
 *@file
 * ���{�������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbComparable.h"
#include "TnbSerializer.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * ���{�������Ǘ�
 *
 *		64bit �ȏ�̐������������Ƃ��o���܂��B
 *
 *	@note �������ɂ́A���g���G���f�B�A���Ŋi�[����Ă��܂��B
 *
 *	@param PLACE �����B�Q�ȏ���w��ł��܂��B PLACE x 32 bit �܂ł̒l�������܂��B
 *
 *	@note �ŏ��l�͓����Ǘ��̊֌W�ŁA�ő�l �� -1 ���������l�ɂȂ�܂��B
 *
 *	@note PLACE �̈قȂ�N���X�Ԃł́A�v�Z�ł��܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbLongInteger.h
 *
 *	@date 09/10/28 �V�K
 */
template<size_t PLACE = 8>
class CLongIntegerT : public IComparableT< TNB::CLongIntegerT<PLACE> >, public ISerializable
{
public:
	
	/**
	 * �R���X�g���N�^.
	 *	@note �ێ����Ă���l�� 0 �ɂȂ�܂��B
	 */
	CLongIntegerT(void)
	{
		Zero();
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��.
	 */
	CLongIntegerT(const CLongIntegerT& other)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] = other.m_num[i];
		}
	}

	/**
	 * ����R���X�g���N�^.
	 *	@param val �����l.
	 */
	CLongIntegerT(LONGLONG val)
	{
		operator=(val);
	}

	/**
	 * [���] �[�����.
	 *	@note �ێ����Ă���l�� 0 �ɂȂ�܂��B
	 */
	void Zero(void)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] = 0;
		}
	}

	/**
	 * [���] �l���.
	 *		������ŁA�l�������܂��B
	 *	@note ������̓��ɋ󔒂͂��Ȃ��ł��������B
	 *	\code
	 *		CLongIntegerT<4> val;
	 *		val.SetString("-123456789");
	 *	\endcode
	 *	@param lpsz ������.
	 *	@return �g�p������.
	 */
	size_t SetString(LPCTSTR lpsz)
	{
		Zero();
		CStr str = lpsz;
		size_t len = str.GetLength();
		bool isMinus = false;
		loop ( i, len )
		{
			if ( i == 0 && str[i] == '-' )
			{
				isMinus = true;
			}
			else if ( str[i] < '0' || str[i] > '9' )
			{
				len = i;
				break;
			}
		}
		size_t ret = len;
		if ( isMinus )
		{
			len--;
			str = str.Mid(1, len);
		}
		else
		{
			str = str.Left(len);
		}
		INDEX cnt = 0;
		while ( len > 0 )
		{
			CStr s = str.Right(9);
			CLongIntegerT x(s.ToInt());
			loop ( i, cnt )
			{
				x *= INT_VALUE_MAX;
			}
			operator+=(x);
			len -= s.GetLength();
			str = str.Left(len);
			cnt++;
		}
		if ( isMinus )
		{
			*this = operator-();
		}
		return ret;
	}

	/**
	 * [���] �l���.
	 *		HEX������ŁA�l�������܂��B
	 *	@note �}�C�i�X�w��͏o���܂���B
	 *	@note ������̓��� '0x' �����Ȃ��ł��������B
	 *	@note ������̓��ɓ��ɋ󔒂͂��Ȃ��ł��������B
	 *	\code
	 *		CLongIntegerT<4> val;
	 *		val.SetHexString("8000000080e412ca");
	 *	\endcode
	 *	@param lpsz ������.
	 *	@return �g�p������.
	 */
	size_t SetHexString(LPCTSTR lpsz)
	{
		Zero();
		size_t len = STRLIB::GetLen(lpsz);
		loop ( i, len )
		{
			if ( STRLIB::HexCharToInt(lpsz[i]) < 0 )
			{
				len = i;
				break;
			}
		}
		loop( i, len )
		{
			size_t j = i / 8;
			if ( j >= DWORD_LEN )
			{
				break;
			}
			int c = STRLIB::HexCharToInt(lpsz[len - 1 - i]);
			DWORD k = (c & 0xFF) << ((i & 7) * 4);
			m_num[j] |= k;
		}
		return len;
	}

	/**
	 * [�ݒ�] ���f�[�^�ݒ�.
	 *		BYTE�̔z��𐔒l�Ƃ��Đݒ肵�܂��B
	 *	@note �{�N���X�̓��g���G���f�B�A���Ŋi�[����Ă��܂��B
	 *	@note �T�C�Y�� byte �w��\�ł����A�ʏ�A GetBytesCount() �œ�����l�Ɠ��������ɂ��܂��B
	 *			�Z���ꍇ�A��ʂ͂O�ɂȂ�܂��B
	 *	@param size ���f�[�^�T�C�Y.
	 *	@param lpRawData ���f�[�^.
	 */
	void SetRawData(size_t size, LPCVOID lpRawData)
	{
		Zero();
		if ( size > BYTE_LEN )
		{
			size = BYTE_LEN;
		}
		const BYTE* P1 = static_cast<const BYTE*>(lpRawData);
		BYTE* P2 = reinterpret_cast<BYTE*>(m_num);
		loop ( i, size )
		{
			*P2++ = *P1++;
		}
	}

	/**
	 * [�擾] ���f�[�^�擾.
	 *		�Ǘ����Ă��鐔�l��BYTE�̔z��Ƃ��Ď擾���܂��B
	 *	@note �擾�T�C�Y�́A GetBytesCount() �œ�����l�Ɠ��������ɂȂ�܂��B
	 *	@param[out] _rawData ���f�[�^���i�[����܂��B
	 */
	void GetRawData(CWorkMem& _rawData) const
	{
		_rawData.Resize(BYTE_LEN);
		const BYTE* P1 = reinterpret_cast<const BYTE*>(m_num);
		BYTE* P2 = _rawData.Ref();
		loop ( i, BYTE_LEN )
		{
			*P2++ = *P1++;
		}
	}

	/**
	 * [�Q��] ���f�[�^�Q��.
	 *		�Ǘ����Ă��鐔�l��BYTE�̔z��Ƃ��ĎQ�Ƃ��܂��B
	 *	@note �T�C�Y�́A GetBytesCount() �œ�����l�ł��B
	 *	@return ���f�[�^�A�h���X�B
	 */
	const BYTE* RefRawData(void) const
	{
		return reinterpret_cast<const BYTE*>(m_num);
	}

	/**
	 * [�擾] �r�b�g�擾.
	 *	@param bitNo �r�b�gNo�B �O�` GetBitsCount() �������w��ł��܂��B
	 *	@retval true �w��r�b�g�͂P�B
	 *	@retval false �w��r�b�g�͂O�B
	 */
	bool GetBit(INDEX bitNo) const
	{
		size_t l = bitNo / 32;
		if ( l < DWORD_LEN )
		{
			return (m_num[l] & _BIT(bitNo % 32)) != 0;
		}
		return false;
	}

	/**
	 * [�ݒ�] �r�b�g�ݒ�.
	 *	@param bitNo �r�b�gNo�B �O�` GetBitsCount() �������w��ł��܂��B
	 *	@param v �r�b�g���
	 */
	void SetBit(INDEX bitNo, bool v = true)
	{
		size_t l = bitNo / 32;
		if ( l < DWORD_LEN )
		{
			if ( v )
			{
				m_num[l] |= _BIT(bitNo % 32);
			}
			else
			{
				m_num[l] &= ~_BIT(bitNo % 32);
			}
		}
	}

	/**
	 * �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator=(const CLongIntegerT& other)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] = other.m_num[i];
		}
		return *this;
	}

	/**
	 * ����I�y���[�^.
	 *	@param val �l.
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator=(LONGLONG val)
	{
		LONGLONG* P =reinterpret_cast<LONGLONG*>(m_num);
		*P = val;
		DWORD u = (val < 0) ? DWORD_MAX : 0;
		for ( size_t i = 2 ; i < DWORD_LEN; i++ )
		{
			m_num[i] = u;
		}
		return *this;
	}

	/**
	 * [�v�Z] �v���X����.
	 *		���̂܂܂̒l��Ԃ��܂��B
	 *	@return �l.
	 */
	CLongIntegerT operator+(void) const
	{
		return *this;
	}

	/**
	 * [�v�Z] �}�C�i�X����.
	 *		�ێ����Ă���l�� -1 ���������l��Ԃ��܂��B
	 *	@return �������]�l.
	 */
	CLongIntegerT operator-(void) const
	{
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = ~m_num[i];
		}
		if ( x.m_num[0] == DWORD_MAX )
		{
			//���オ��
			x.m_num[0] = 0;
			for ( size_t i = 1; i < DWORD_LEN; i++ )
			{
				if ( x.m_num[i] == DWORD_MAX )
				{
					x.m_num[i] = 0;
				}
				else
				{
					x.m_num[i]++;
					break;
				}
			}
		}
		else
		{
			x.m_num[0]++;
		}
		return x;
	}

	/**
	 * [�v�Z] �r�b�g���].
	 *	@return �r�b�g���]�����l.
	 */
	CLongIntegerT operator~(void) const
	{
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = ~m_num[i];
		}
		return x;
	}

	/**
	 * [���] ���Z���.
	 *		�ێ����Ă���l�Ɏw��̒l�������A�����ێ����܂��B
	 *	@param val ���Z�l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator+=(const CLongIntegerT& val)
	{
		return operator-=(-val);
	}

	/**
	 * [���] ���Z���.
	 *		�ێ����Ă���l�Ɏw��̒l�������A�����ێ����܂��B
	 *	@param val ���Z�l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator-=(const CLongIntegerT& val)
	{
		if ( this != &val )
		{
			loop ( i, DWORD_LEN )
			{
				bool isBorrow = true;
				if ( m_num[i] >= val.m_num[i] )
				{
					isBorrow = false;
				}
				m_num[i] -= val.m_num[i];
				size_t j = i + 1;
				while ( isBorrow && j < DWORD_LEN )
				{
					if ( m_num[j] >= 1 )
					{
						isBorrow = false;
					}
					m_num[j]--;
					j++;
				}
			}
		}
		else 
		{
			Zero();
		}
		return *this;
	}

	/**
	 * [���] ��Z���.
	 *		�ێ����Ă���l�Ɏw��̒l�������A�����ێ����܂��B
	 *	@param val ��Z�l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator*=(const CLongIntegerT& val)
	{
		*this = operator*(*this, val);
		return *this;
	}

	/**
	 * [���] ���Z���.
	 *		�ێ����Ă���l���w��̒l�Ŋ���A�����ێ����܂��B
	 *	@param val ���Z�l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator/=(const CLongIntegerT& val)
	{
		*this = operator/(*this, val);
		return *this;
	}

	/**
	 * [���] �]��Z���.
	 *		�ێ����Ă���l���w��̒l�Ŋ���A�]��̒l��ێ����܂��B
	 *	@param val ���Z�l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator%=(const CLongIntegerT& val)
	{
		*this = operator%(*this, val);
		return *this;
	}

	/**
	 * [���] AND �Z���.
	 *		�ێ����Ă���l�Ǝw��̒l�� AND ���A�����ێ����܂��B
	 *	@param val �l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator&=(const CLongIntegerT& val)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] &= val.m_num[i];
		}
		return *this;
	}

	/**
	 * [���] OR �Z���.
	 *		�ێ����Ă���l�Ǝw��̒l�� OR ���A�����ێ����܂��B
	 *	@param val �l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator|=(const CLongIntegerT& val)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] |= val.m_num[i];
		}
		return *this;
	}

	/**
	 * [���] XOR �Z���.
	 *		�ێ����Ă���l�Ǝw��̒l�� XOR ���A�����ێ����܂��B
	 *	@param val �l
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator^=(const CLongIntegerT& val)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] ^= val.m_num[i];
		}
		return *this;
	}

	/**
	 * [���] ���V�t�g���.
	 *		�ێ����Ă���l���w��̐��������V�t�g�����l��ێ����܂��B
	 *	@param val �V�t�g��
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator<<=(size_t val)
	{
		while ( val > 0 )
		{
			if ( val >= 32 )
			{
				for ( int i = DWORD_LEN - 1; i > 0; i-- )
				{
					m_num[i] = m_num[i - 1];
				}
				val -= 32;
				m_num[0] = 0;
			}
			else 
			{
				for ( int i = DWORD_LEN - 1; i > 0; i-- )
				{
					m_num[i] = (m_num[i] << 1) | ((m_num[i - 1] & 0x80000000) ? 1 : 0);
				}
				m_num[0] <<= 1;
				val--;
			}
		}
		return *this;
	}

	/**
	 * [���] �E�V�t�g���.
	 *		�ێ����Ă���l���w��̐������E�V�t�g�����l��ێ����܂��B
	 *	@param val �V�t�g��
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator>>=(size_t val)
	{
		while ( val > 0 )
		{
			if ( val >= 32 )
			{
				loop ( i, DWORD_LEN - 1 )
				{
					m_num[i] = m_num[i + 1];
				}
				m_num[DWORD_LEN - 1] = 0;
				val -= 32;
			}
			else 
			{
				loop ( i, DWORD_LEN - 1 )
				{
					m_num[i] = (m_num[i] >> 1) | ((m_num[i + 1] & 1) ? 0x80000000 : 0);
				}
				m_num[DWORD_LEN - 1] >>= 1;
				val--;
			}
		}
		return *this;
	}

	/**
	 * [�v�Z] ���V�t�g.
	 *		�ێ����Ă���l���w��̐��������V�t�g�����l��Ԃ��܂��B
	 *	@param val �V�t�g��
	 *	@return �l.
	 */
	CLongIntegerT operator<<(size_t val) const
	{
		CLongIntegerT x = *this;
		x.operator<<=(val);
		return x;
	}

	/**
	 * [�v�Z] �E�V�t�g.
	 *		�ێ����Ă���l���w��̐������E�V�t�g�����l��Ԃ��܂��B
	 *	@param val �V�t�g��
	 *	@return �l.
	 */
	CLongIntegerT operator>>(size_t val) const
	{
		CLongIntegerT x = *this;
		x.operator>>=(val);
		return x;
	}

	/**
	 * [���] �f�N�������g.
	 *		�ێ����Ă���l�� -1 ���܂��B
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator--(void)
	{
		m_num[0]--;
		if ( m_num[0] == DWORD_MAX )
		{
			for ( size_t i = 1; i < DWORD_LEN; i++ )
			{
				m_num[i]--;
				if ( m_num[i] != DWORD_MAX )
				{
					break;
				}
			}
		}
		return *this;
	}

	/**
	 * [���] �f�N�������g.
	 *		�ێ����Ă���l��Ԃ��A�ێ����Ă���l�� -1 ���܂��B
	 *	@return �v�Z�O�̒l�B
	 */
	CLongIntegerT operator--(int)
	{
		CLongIntegerT x = *this;
		operator--();
		return x;
	}

	/**
	 * [���] �C���N�������g.
	 *		�ێ����Ă���l�� +1 ���܂��B
	 *	@return �����̎Q��.
	 */
	CLongIntegerT& operator++(void)
	{
		m_num[0]++;
		if ( m_num[0] == 0 )
		{
			for ( size_t i = 1; i < DWORD_LEN; i++ )
			{
				m_num[i]++;
				if ( m_num[i] != 0 )
				{
					break;
				}
			}
		}
		return *this;
	}

	/**
	 * [���] �C���N�������g.
	 *		�ێ����Ă���l��Ԃ��A�ێ����Ă���l�� +1 ���܂��B
	 *	@return �v�Z�O�̒l�B
	 */
	CLongIntegerT operator++(int)
	{
		CLongIntegerT x = *this;
		operator++();
		return x;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a + b �̒l�B
	 */
	friend CLongIntegerT operator+(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator+=(b);
		return x;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a - b �̒l�B
	 */
	friend CLongIntegerT operator-(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator-=(b);
		return x;
	}

	/**
	 * [�v�Z] ��Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a �~ b �̒l�B
	 */
	friend CLongIntegerT operator*(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CWorkMemT<DWORD> w(DWORD_LEN * 2);
		w.Fill(0);
		const BYTE* pV1 = reinterpret_cast<const BYTE*>(a.m_num);
		const BYTE* pV2 = reinterpret_cast<const BYTE*>(b.m_num);
		BYTE* pWork = reinterpret_cast<BYTE*>(w.Ref());
		loop ( i, BYTE_LEN )
		{
			if ( pV2[i] != 0 )
			{
				loop ( j, BYTE_LEN )
				{
					if ( pV1[j] != 0 )
					{
						size_t ix = i + j;
						BYTE* P = &pWork[ix];
						DWORD kk = pV1[j] * pV2[i] + *P;
						*P++ = static_cast<BYTE>(kk & 0xFF);
						ix++;
						kk /= 0x100;
						while ( (kk != 0) && (ix < (BYTE_LEN * 2)) )
						{
							kk += *P;
							*P++ = static_cast<BYTE>(kk & 0xFF);
							ix++;
							kk /= 0x100;
						}
					}
				}
			}
		}
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = w[i];
		}
		return x;
	}

	/**
	 * [�v�Z] ���Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a �� b �̏��B
	 */
	friend CLongIntegerT operator/(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT quotient;
		CLongIntegerT remainder;
		m_Divide(a, b, quotient, remainder);
		return quotient;
	}

	/**
	 * [�v�Z] �]��Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a �� b �̗]��B
	 */
	friend CLongIntegerT operator%(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT quotient;
		CLongIntegerT remainder;
		m_Divide(a, b, quotient, remainder);
		return remainder;
	}

	/**
	 * [�v�Z] AND �Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a and b �̒l�B
	 */
	friend CLongIntegerT operator&(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator&=(b);
		return x;
	}

	/**
	 * [�v�Z] OR �Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a or b �̒l�B
	 */
	friend CLongIntegerT operator|(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator|=(b);
		return x;
	}

	/**
	 * [�v�Z] XOR �Z.
	 *	@param a �l�P
	 *	@param b �l�Q
	 *	@return a xor b �̒l�B
	 */
	friend CLongIntegerT operator^(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator^=(b);
		return x;
	}

	/**
	 * [�v�Z] ��Βl.
	 *	@param val �l.
	 *	@return val �̐�Βl�B
	 */
	friend CLongIntegerT abs(const CLongIntegerT& val)
	{
		if ( val.IsMinus() )
		{
			return -val;
		}
		return val;
	}

	/**
	 * [�v�Z] ���Z.
	 *		�ێ����Ă���l���w��̏��Z�l�Ŋ���A���ʂ�Ԃ��܂��B
	 *	@param[out] _quotient �v�Z���ʂ̏��B
	 *	@param[out] _remainder �v�Z���ʂ̗]��B
	 *	@param[in] val ���Z�l.
	 *	@retval true ����.
	 *	@retval false ���s�i0���G���[�j.
	 */
	bool Divide(CLongIntegerT& _quotient, CLongIntegerT& _remainder, const CLongIntegerT& val) const
	{
		return m_Divide(*this, val, _quotient, _remainder);
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CLongIntegerT<PLACE>& t) const
	{
		INT_PTR sign1 = IsMinus() ? -1 : 1;
		INT_PTR sign2 = t.IsMinus() ? -1 : 1;
		if ( sign1 == sign2 )
		{
			for ( int i = DWORD_LEN - 1; i >= 0; i-- )
			{
				if ( m_num[i] > t.m_num[i] )
				{
					return sign1;
				}
				else if ( m_num[i] < t.m_num[i] )
				{
					return -sign1;
				}
			}
		}
		else 
		{
			return sign1;
		}
		return 0;
	}

	/**
	 * [�m�F] �}�C�i�X�m�F.
	 *		�ێ����Ă���l�̊m�F�����܂��B
	 *	@retval true �}�C�i�X�i�O�����j�B
	 *	@retval false �v���X�i�O�ȏ�j�B
	 */
	bool IsMinus(void) const
	{
		return (m_num[DWORD_LEN - 1] & 0x80000000) != 0;
	}

	/**
	 * [�m�F] �[���m�F.
	 *		�ێ����Ă���l�̊m�F�����܂��B
	 *	@retval true �[���B
	 *	@retval false �[���ȊO
	 */
	bool IsZero(void) const
	{
		loop ( i, DWORD_LEN )
		{
			if ( m_num[i] != 0 )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [�m�F] �ێ��l�m�F.
	 *		�ێ����Ă���l�� int �ŕ\���\���A�m�F�����܂��B
	 *	@retval true �\���\�B
	 *	@retval false �s�\�B
	 */
	bool CanExpressInt(void) const
	{
		return m_CanExpress(1);
	}

	/**
	 * [�擾] int �擾.
	 *	@note CanExpressInt() �̌��ʂ� true �łȂ��ꍇ�A�擾�ł���l�͕s��ł��B
	 *	@return �l
	 */
	int ToInt(void) const
	{
		return m_num[0];
	}

	/**
	 * [�m�F] �ێ��l�m�F.
	 *		�ێ����Ă���l�� LONGLONG �ŕ\���\���A�m�F�����܂��B
	 *	@retval true �\���\�B
	 *	@retval false �s�\�B
	 */
	bool CanExpressLonglong(void) const
	{
		if ( DWORD_LEN > 2 )
		{
			return m_CanExpress(2);
		}
		return true;
	}

	/**
	 * [�擾] LONGLONG �擾.
	 *	@note CanExpressLonglong() �̌��ʂ� true �łȂ��ꍇ�A�擾�ł���l�͕s��ł��B
	 *	@return �l
	 */
	LONGLONG ToLonglong(void) const
	{
		LONGLONG r = m_num[1];
		r <<= 32;
		return r | m_num[0];
	}
	
	/**
	 * [�擾] �L�� bit ���擾.
	 *	@note �ێ����Ă���l���}�C�i�X�̏ꍇ�A GetBitsCount() �Ɠ����l�ɂȂ�܂��B
	 *	@return bit��
	 */
	size_t GetValidBitsCount(void) const
	{
		loop_dn ( i, DWORD_LEN )
		{
			if ( m_num[i] != 0 )
			{
				loop_dn ( j, 32 )
				{
					if ( (m_num[i] & _BIT(j)) != 0 )
					{
						return i * 32 + j + 1;
					}
				}
			}
		}
		return 0;
	}

	/**
	 * [�擾] HEX������.
	 *		�ێ����Ă���l�� HEX �����񉻂��܂��B
	 *	@param isUpperCase false ���w�肷��Ə������A �ȗ� �� true ���w�肷��Ƒ啶���ō쐬���܂��B
	 *	@return ������.
	 */
	CStr ToHexString(bool isUpperCase = true) const
	{
		CStr s;
		LPCTSTR lpszFmt = isUpperCase ? _T("%08X") :  _T("%08x");
		loop ( i, DWORD_LEN )
		{
			s += CStr::Fmt(lpszFmt, m_num[DWORD_LEN - 1 - i]);
		}
		return s;
	}

	/**
	 * [�擾] ������.
	 *		�ێ����Ă���l�𕶎��񉻂��܂��B
	 *	@return ������.
	 */
	CStr ToString(void) const
	{
		CLongIntegerT x = *this;
		bool isMinus = x.IsMinus();
		if ( isMinus )
		{
			x = -x;
		}
		CStr s;
		while ( x > 0 )
		{
			s = CStr::Fmt(_T("%09d"), (x % INT_VALUE_MAX).ToInt()) + s;
			x /= INT_VALUE_MAX;
		}
		s.TrimLeft('0');
		if ( s.IsEmpty() )
		{
			return _T("0");
		}
		if ( isMinus )
		{
			return _T("-") + s;
		}
		return s;
	}

	/**
	 * [����] �V���A���C�Y
	 *	@param[out] _sr �V���A���C�U�[�B 
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		size_t l = DWORD_LEN;
		_sr << l << l;
		loop ( i, l )
		{
			_sr << m_num[i];
		}
	}

	/**
	 * [����] �f�V���A���C�Y
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		Zero();
		size_t l1, l2;
		ds >> l1 >> l2;
		if ( l1 != l2 || l1 != DWORD_LEN )
		{
			throw CInvalidParamException();
		}
		loop ( i, l1 )
		{
			ds >> m_num[i];
		}
	}

	/**
	 * [�擾] bit ���擾.
	 *	@return bit��
	 */
	static size_t GetBitsCount(void)
	{
		return BIT_LEN;
	}

	/**
	 * [�擾] byte ���擾.
	 *	@return byte��
	 */
	static size_t GetBytesCount(void)
	{
		return BYTE_LEN;
	}

	/**
	 * [�擾] �ő包���擾.
	 *		10�i���ŕ\���錅����Ԃ��܂��B �Ⴆ�΁A 4 �Ȃ� 9999 �` -9999 �͕\�����Ƃ��o���܂��B
	 *	@return ����.
	 */
	static size_t GetMaxPlace(void)
	{
		return GetMax().ToString().GetLength() - 1;
	}

	/**
	 * [�擾] �ő�l�擾.
	 *		�Ǘ��ł���ő�̒l��Ԃ��܂��B
	 *	@return �l.
	 */
	static CLongIntegerT GetMax(void)
	{
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = DWORD_MAX;
		}
		x.m_num[DWORD_LEN - 1] = 0x7FFFFFFF;
		return x;
	}

	/**
	 * [�擾] �ŏ��l�擾.
	 *		�Ǘ��ł���ŏ��̒l��Ԃ��܂��B
	 *	@return �l.
	 */
	static CLongIntegerT GetMin(void)
	{
		CLongIntegerT x;
		x.m_num[0] = 1;
		x.m_num[DWORD_LEN - 1] = 0x80000000;
		return x;
	}

	/**
	 * [�擾] �ϊ��l�擾.
	 *		�����񂩂�l���쐬���܂��B
	 *	@note ������̓��ɋ󔒂͂��Ȃ��ł��������B
	 *	\code
	 *		CLongIntegerT<4> val = CLongIntegerT<4>::FromString("-123456789");
	 *	\endcode
	 *	@param lpsz ������.
	 *	@return �l.
	 */
	static CLongIntegerT FromString(LPCTSTR lpsz)
	{
		CLongIntegerT x;
		x.SetString(lpsz);
		return x;
	}

	/**
	 * [�擾] �ϊ��l�擾.
	 *		HEX�����񂩂�l���쐬���܂��B
	 *	@note �}�C�i�X�w��͏o���܂���B
	 *	@note ������̓��� '0x' �����Ȃ��ł��������B
	 *	@note ������̓��ɓ��ɋ󔒂͂��Ȃ��ł��������B
	 *	\code
	 *		CLongIntegerT<4> val = CLongIntegerT<4>::FromHexString("8000000080e412ca");
	 *	\endcode
	 *	@param lpsz ������.
	 *	@return �l.
	 */
	static CLongIntegerT FromHexString(LPCTSTR lpsz)
	{
		CLongIntegerT x;
		x.SetHexString(lpsz);
		return x;
	}

private:

	/// ����Z
	static bool m_Divide(const CLongIntegerT& a, const CLongIntegerT& b, CLongIntegerT& _quotient, CLongIntegerT& _remainder)
	{
		_quotient.Zero();
		_remainder.Zero();
		if ( a.IsZero() )
		{
			return true;
		}
		if ( b.IsZero() )
		{
			return false ;
		}
		CLongIntegerT aa = abs(a);
		CLongIntegerT bb = abs(b);
		if ( aa < bb )
		{
			_remainder = a;
			return true;
		}
		INT_PTR cnt = aa.GetValidBitsCount() - bb.GetValidBitsCount();
		ASSERT( cnt >= 0 );
		bb <<= cnt;
		for ( INT_PTR i = 0; i <= cnt; i++ )
		{
			_quotient <<= 1;
			if ( aa >= bb )
			{
				aa -= bb;
				_quotient.m_num[0] |= 1;
			}
			bb >>= 1;
		}
		if ( a.IsMinus() != b.IsMinus() )
		{
			_quotient = -_quotient;
		}
		_remainder = aa;
		if ( a.IsMinus() )
		{
			_remainder = -_remainder;
		}
		return true;
	}
	/// �\���\�H
	bool m_CanExpress(INDEX idx) const
	{
		DWORD n = m_num[idx];
		if ( n == 0 || n != DWORD_MAX )
		{
			for ( size_t i = idx + 1; i < DWORD_LEN; i++ )
			{
				if ( n != m_num[i] )
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
	#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��
	INT_PTR operator- (const TYP &t) const
	{
		return 0;
	}
	#endif // _TnbDOXYGEN
	/// �萔
	enum
	{
		DWORD_LEN = PLACE,			///< DWORD �ł̒���
		BYTE_LEN = PLACE * 4,		///< BYTE �ł̒���
		BIT_LEN = PLACE * 32,		///< BIT �ł̒���
		INT_VALUE_MAX = 1000000000,	///< int �̕\���\�l�i9���j
	};
	DWORD	m_num[DWORD_LEN];	///< �f�[�^
	friend class CLongIntegerTest;
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

template<> class CLongIntegerT<0> 
{
	CLongIntegerT(void); // PLACE �� 0 �͎w��ł��܂���B
};

template<> class CLongIntegerT<1> 
{
	CLongIntegerT(void); // PLACE �� 1 �͎w��ł��܂���B
};

#endif // _TnbDOXYGEN



/**@ingroup VALUE
 * 256bit �����Ǘ�
 *
 *		256bit �̐������������Ƃ��o���܂��B
 *
 *	@note �������ɂ́A���g���G���f�B�A���Ŋi�[����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbLongInteger.h
 *
 *	@date 09/10/28 �V�K
 */
typedef CLongIntegerT<8> INT256;



}; // TNB
