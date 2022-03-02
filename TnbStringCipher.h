#pragma once
/**
 *@file
 * 暗号文字列関係のヘッダ
 *
 *		暗号化、複合化、をサポート
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup CIPHER STRING
 * 文字列暗号復号クラス
 *
 *		簡易的な暗号化/復号化をすることが出来ます。
 *		暗号化しても文字列になっています。
 *
 *	@par必要ファイル
 *			TnbStringCipher.h
 *
 *	@date 06/07/21	新規作成
 *	@date 12/04/19	Encode() , Decode() を const メソッドに変更。
 */
class CStringCipher
{
public:

	/**
	 * [設定] キー設定.
	 *		暗号、復号に使用するキーを指定します。当然、暗号と同じキーでないと復号できません。
	 *	@note 内部では ASCII に変換され使用されます。
	 *	@param ascKey 暗号キー
	 */
	void SetKey(const CAscii& ascKey)
	{
		m_ascKey = ascKey;
	}

	/**
	 * [設定] 暗号化
	 *	@param lpsz 元文字列。
	 *	@return 暗号文字列。
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 */
	CAscii Encode(LPCSTR lpsz) const
	{
		return m_Shift(lpsz, true);
	}

	/**
	 * [設定] 復号化
	 *	@param lpsz 暗号文字列。
	 *	@return 元文字列。
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 */
	CAscii Decode(LPCSTR lpsz) const
	{
		return m_Shift(lpsz, false);
	}

private:
	/**
	 * 範囲シフト.
	 *		指定内容で変更値によりコードをシフトします。
	 *	@param c 元コード。
	 *	@param iShift シフト値
	 *	@param iMin シフト対象範囲(最小値)
	 *	@param iMax シフト対象範囲(最大値)
	 *	@retval -1 元コードが iMin～iMax でない場合。
	 *	@return 0以上 シフトしたコード。
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
	 * コードシフト.
	 *		変更値によりコードをシフトします。
	 *	@param c 元コード
	 *	@param iShift シフト値
	 *	@return シフトしたコード。
	 *			ただし、元コードが 0x20～0x80, 0xA0～0xDF, 0xE0～0xFB でない場合、元コードのままになる。
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
	 * 文字列シフト.
	 *	@param lpsz 元文字列
	 *	@param boIsEnc trueなら暗号化、falseなら複合化
	 *	@return 処理後の文字列。
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 */
	CAscii m_Shift(LPCSTR lpsz, bool boIsEnc) const
	{
		if ( m_ascKey.IsEmpty() )
		{
			ASSERT0(false, "CCipherData", "暗号、復号キーが指定されていません。");
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

	CAscii		m_ascKey;				///< 暗号、復号のキー
};



};//TNB
