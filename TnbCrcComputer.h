#pragma once
/**
 *@file
 * CRC(Cyclic Redundancy Check)関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup CIPHER
 * CRC-16-CCITT(Cyclic Redundancy Check 16bit)計算クラス.
 *
 *	指定情報の CRC を計算します。 XMODEM , Bluetooth で使用されています。
 *
 *	@par必要ファイル 
 *			TnbCrcComputer.h
 *
 *	@date 12/02/14 新規作成
 */
class CCrc16 : CCopyImpossible
{
public:
	/// 定数
	enum
	{
		CRCPOLY1 = 0x1021,		///< MSB ファースト定数
		CRCPOLY2 = 0x8408,		///< LSB ファースト 定数
		BYTE_BIT = 8,			///< BYTE の bit数
		TABLE_SIZE = 256,		///< テーブルサイズ
	};

	/**
	 * [計算] テーブル計算(MSBファースト)
	 *	@param size データサイズ
	 *	@param P データ
	 *	@param r 初期値
	 *	@return CRC値
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
	 * [計算] 計算(MSBファースト)
	 *	@param size データサイズ
	 *	@param P データ
	 *	@param r 初期値
	 *	@return CRC値
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
	 * [計算] テーブル計算(LSBファースト)
	 *	@param size データサイズ
	 *	@param P データ
	 *	@param r 初期値
	 *	@return CRC値
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
	 * [計算] 計算(LSBファースト)
	 *	@param size データサイズ
	 *	@param P データ
	 *	@param r 初期値
	 *	@return CRC値
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
	// テーブル作成(MSBファースト)
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
	// テーブル作成(LSBファースト)
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
	CWorkMemT<WORD> m_table1;	///< テーブル(MSB)
	CWorkMemT<WORD> m_table2;	///< テーブル(LSB)
};



/**@ingroup CIPHER
 * CRC-16-IBM(Cyclic Redundancy Check 16bit)計算クラス.
 *
 *	指定情報の CRC を計算します。 USB などで使用されてます。
 *
 *	@par必要ファイル 
 *			TnbCrcComputer.h
 *
 *	@date 12/11/13 新規作成
 */
class CCrc16Ibm : CCopyImpossible
{
public:

	/**
	 * [計算] 計算(0xA001)
	 *	@param size データサイズ
	 *	@param P データ
	 *	@return CRC値
	 */
	static WORD Calc1(size_t size, const void* P)
	{
		return ms_Calc(size, P, 0xA001);
	}

	/**
	 * [計算] 計算(0x8005)
	 *	@param size データサイズ
	 *	@param P データ
	 *	@return CRC値
	 */
	static WORD Calc2(size_t size, const void* P)
	{
		return ms_Calc(size, P, 0x8005);
	}

private:
	// [計算] 計算
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
 * CRC32(Cyclic Redundancy Check 32bit)計算クラス.
 *
 *	指定情報の CRC32 を計算します。
 *
 *	@par必要ファイル 
 *			TnbCrcComputer.h
 *
 *	@date 12/05/19 新規作成
 */
class CCrc32 : CCopyImpossible
{
public:

	/**
	 * [計算] テーブル計算
	 *	@param size データサイズ
	 *	@param p データ
	 *	@return CRC値
	 */
	DWORD CalcTb(size_t size, const void* p)
	{
		return CalcTb(0xffffffffL, size, p) ^ 0xffffffffL;
	}

	/**
	 * [計算] テーブル計算
	 *	@param crc 初期CRC
	 *	@param size データサイズ
	 *	@param p データ
	 *	@return CRC値
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
	// テーブル作成
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
	CWorkMemT<DWORD>	m_table;	///< テーブル
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
