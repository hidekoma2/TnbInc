#pragma once
/**
 *@file
 * CRC(Cyclic Redundancy Check)�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup CIPHER
 * CRC-16-CCITT(Cyclic Redundancy Check 16bit)�v�Z�N���X.
 *
 *	�w����� CRC ���v�Z���܂��B XMODEM , Bluetooth �Ŏg�p����Ă��܂��B
 *
 *	@par�K�v�t�@�C�� 
 *			TnbCrcComputer.h
 *
 *	@date 12/02/14 �V�K�쐬
 */
class CCrc16 : CCopyImpossible
{
public:
	/// �萔
	enum
	{
		CRCPOLY1 = 0x1021,		///< MSB �t�@�[�X�g�萔
		CRCPOLY2 = 0x8408,		///< LSB �t�@�[�X�g �萔
		BYTE_BIT = 8,			///< BYTE �� bit��
		TABLE_SIZE = 256,		///< �e�[�u���T�C�Y
	};

	/**
	 * [�v�Z] �e�[�u���v�Z(MSB�t�@�[�X�g)
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@param r �����l
	 *	@return CRC�l
	 */
	WORD CalcTb1(size_t size, const void* P, WORD r = 0xFFFF)
	{
		m_InitTable1();
		const BYTE* B = static_cast<const BYTE*>(P);
		loop ( i, size )
		{
			r = static_cast<WORD>((r << BYTE_BIT) ^ m_table1[BYTE((r >> (16 - BYTE_BIT)) ^ *B++)]);
		}
		return static_cast<WORD>(~r);
	}

	/**
	 * [�v�Z] �v�Z(MSB�t�@�[�X�g)
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@param r �����l
	 *	@return CRC�l
	 */
	static WORD Calc1(size_t size, const void* P, WORD r = 0xFFFF)
	{
		const BYTE* B = static_cast<const BYTE*>(P);
		loop ( i, size )
		{
			r ^= B[i] << (16 - BYTE_BIT);
			loop ( j, BYTE_BIT )
			{
				r = static_cast<WORD>(((r & 0x8000) != 0) ? ((r << 1) ^ CRCPOLY1) : (r << 1));
			}
		}
		return static_cast<WORD>(~r);
	}

	/**
	 * [�v�Z] �e�[�u���v�Z(LSB�t�@�[�X�g)
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@param r �����l
	 *	@return CRC�l
	 */
	WORD CalcTb2(size_t size, const void* P, WORD r = 0xFFFF)
	{
		m_InitTable2();
		const BYTE* B = static_cast<const BYTE*>(P);
		loop ( i, size )
		{
			r = static_cast<WORD>((r >> BYTE_BIT) ^ m_table2[BYTE(r ^ *B++)]);
		}
		return static_cast<WORD>(r ^ 0xFFFF);
	}

	/**
	 * [�v�Z] �v�Z(LSB�t�@�[�X�g)
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@param r �����l
	 *	@return CRC�l
	 */
	static WORD Calc2(size_t size, const void* P, WORD r = 0xFFFF)
	{
		const BYTE* B = static_cast<const BYTE*>(P);
		loop ( i, size )
		{
			r ^= B[i];
			loop ( j, BYTE_BIT )
			{
				r = static_cast<WORD>(((r & 1) != 0) ? ((r >> 1) ^ CRCPOLY2) : (r >> 1));
			}
		}
		return static_cast<WORD>(r ^ 0xFFFF);
	}

private:
	// �e�[�u���쐬(MSB�t�@�[�X�g)
	void m_InitTable1(void)
	{
		if ( m_table1.GetSize() == 0 )
		{
			m_table1.Resize(TABLE_SIZE);
			loop ( i, TABLE_SIZE )
			{
				WORD r = static_cast<WORD>(i << (16 - BYTE_BIT));
				loop ( j, BYTE_BIT )
				{
					r = static_cast<WORD>(((r & 0x8000) != 0) ? ((r << 1) ^ CRCPOLY1) : (r << 1));
				}
				m_table1[i] = r;
			}
		}
	}
	// �e�[�u���쐬(LSB�t�@�[�X�g)
	void m_InitTable2(void)
	{
		if ( m_table2.GetSize() == 0 )
		{
			m_table2.Resize(TABLE_SIZE);
			loop ( i, TABLE_SIZE )
			{
				WORD r = static_cast<WORD>(i);
				loop ( j, BYTE_BIT )
				{
					r = static_cast<WORD>(((r & 1) != 0) ? ((r >> 1) ^ CRCPOLY2) : (r >> 1));
				}
				m_table2[i] = r;
			}
		}
	}
	CWorkMemT<WORD> m_table1;	///< �e�[�u��(MSB)
	CWorkMemT<WORD> m_table2;	///< �e�[�u��(LSB)
};



/**@ingroup CIPHER
 * CRC-16-IBM(Cyclic Redundancy Check 16bit)�v�Z�N���X.
 *
 *	�w����� CRC ���v�Z���܂��B USB �ȂǂŎg�p����Ă܂��B
 *
 *	@par�K�v�t�@�C�� 
 *			TnbCrcComputer.h
 *
 *	@date 12/11/13 �V�K�쐬
 */
class CCrc16Ibm : CCopyImpossible
{
public:

	/**
	 * [�v�Z] �v�Z(0xA001)
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@return CRC�l
	 */
	static WORD Calc1(size_t size, const void* P)
	{
		return ms_Calc(size, P, 0xA001);
	}

	/**
	 * [�v�Z] �v�Z(0x8005)
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@return CRC�l
	 */
	static WORD Calc2(size_t size, const void* P)
	{
		return ms_Calc(size, P, 0x8005);
	}

private:
	// [�v�Z] �v�Z
	static WORD ms_Calc(size_t size, const void* P, WORD r)
	{
		WORD crc = 0;
		const BYTE* B = static_cast<const BYTE*>(P);
		loop ( i, size )
		{
			BYTE b = B[i];
			loop ( j, 8 )
			{
				crc = static_cast<WORD>(((b ^ static_cast<BYTE>(crc)) & 1) ? ((crc >> 1) ^ r) : (crc >> 1));
				b >>= 1;
			}
		}
		return crc;
	}
};



/**@ingroup CIPHER
 * CRC32(Cyclic Redundancy Check 32bit)�v�Z�N���X.
 *
 *	�w����� CRC32 ���v�Z���܂��B
 *
 *	@par�K�v�t�@�C�� 
 *			TnbCrcComputer.h
 *
 *	@date 12/05/19 �V�K�쐬
 */
class CCrc32 : CCopyImpossible
{
public:

	/**
	 * [�v�Z] �e�[�u���v�Z
	 *	@param size �f�[�^�T�C�Y
	 *	@param p �f�[�^
	 *	@return CRC�l
	 */
	DWORD CalcTb(size_t size, const void* p)
	{
		return CalcTb(0xffffffffL, size, p) ^ 0xffffffffL;
	}

	/**
	 * [�v�Z] �e�[�u���v�Z
	 *	@param crc ����CRC
	 *	@param size �f�[�^�T�C�Y
	 *	@param p �f�[�^
	 *	@return CRC�l
	 */
	DWORD CalcTb(DWORD crc, size_t size, const void* p)
	{
		const BYTE *b = static_cast<const BYTE*>(p);
		DWORD c = crc;
		m_InitTable();
		loop ( n, size )
		{
			c = m_table[(c ^ b[n]) & 0xff] ^ (c >> 8);
		}
		return c;
	}

private:
	// �e�[�u���쐬
	void m_InitTable(void)
	{
		if ( m_table.GetSize() == 0 )
		{
			m_table.Resize(256);
			loop ( n, 256 )
			{
				DWORD c = static_cast<DWORD>(n);
				loop ( k, 8 )
				{
					if ( (c & 1) != 0 )
					{
						c = 0xedb88320L ^ (c >> 1);
					}
					else
					{
						c >>= 1;
					}
				}
				m_table[n] = c;
			}
		}
	}
	CWorkMemT<DWORD>	m_table;	///< �e�[�u��
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
