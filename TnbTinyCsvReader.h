#pragma once
/**
 *@file
 * CSV管理関係のヘッダ
 *
 *		CSVファイルを読み込み管理するクラスが有ります。
 *		ASCIIのみです。扱えるのは数値のみです。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBlocklinkVector.h"
#include "TnbFile.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * 簡易CSVファイル読み込みクラス.
 *	
 *		ASCII のみで書かれた数値のみのCSVファイルを高速で読み込むことができます。
 *
 *	@date 19/06/15 新規作成
 */
class CTinyCsvReader
{
public:

	/**
	 * [読込] CSV読み込み.
	 *	@param lpszFileName ファイル名.
	 *	@retval true 成功. インスタンスにデータを持ちます。
	 *	@retval false 失敗
	 */
	bool ReadCsv(LPCTSTR lpszFileName)
	{
		m_content.RemoveAll();
		CByteVector vb;
		if ( CFileReader::ReadAll(vb, lpszFileName) )
		{
			vb.Add(0);	//終端
			LPCSTR lpsz = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
			m_content = m_SeparateLine(lpsz);
			return true;
		}
		return false;
	}

	/**
	 * [取得] 行数取得.
	 *	@return 保持している内容の行数を取得します。
	 */
	size_t GetLineCount(void) const
	{
		return m_content.GetSize();
	}
		
	/**
	 * [取得] データ取得
	 *	@note 指定ラインのデータを取り出します。
	 *	@param line 行数
	 *	@param emptyValue 値なしの場合の値を指定します。
	 *	@return 配列データ.
	 */
	CVectorT<int> Get(INDEX line, int emptyValue) const
	{
		CBlockLinkVectorT<int> vs;
		if ( m_content.GetSize() > line )
		{
			LPCSTR lpsz = m_content[line].ReferBuffer();
			int s;
			if ( lpsz[0] != 0 )
			{
				while ( true )
				{
					INT_PTR r = m_FindComma(lpsz);
					if ( r < 0 )
					{
						s = (*lpsz == 0) ? emptyValue : ::atoi(lpsz);
					}
					else if ( r == 0 )
					{
						s = emptyValue;
						lpsz++;
					}
					else
					{
						s = ::atoi(lpsz);
						lpsz += r + 1;
					}
					vs.Add(s);
					if ( r < 0 )
					{
						break;
					}
				}
			}
		}
		CVectorT<int> v;
		v.Copy(vs);
		return v;
	}

	/**
	 * [取得] データ取得
	 *	@note 指定ラインのデータを取り出します。
	 *	@param line 行数
	 *	@param emptyValue 値なしの場合の値を指定します。
	 *	@return 配列データ.
	 */
	CVectorT<double> Get(INDEX line, double emptyValue) const
	{
		CBlockLinkVectorT<double> vs;
		if ( m_content.GetSize() > line )
		{
			LPCSTR lpsz = m_content[line].ReferBuffer();
			double s;
			if ( lpsz[0] != 0 )
			{
				while ( true )
				{
					INT_PTR r = m_FindComma(lpsz);
					if ( r < 0 )
					{
						s = (*lpsz == 0) ? emptyValue : ::atof(lpsz);
					}
					else if ( r == 0 )
					{
						s = emptyValue;
						lpsz++;
					}
					else
					{
						s = ::atof(lpsz);
						lpsz += r + 1;
					}
					vs.Add(s);
					if ( r < 0 )
					{
						break;
					}
				}
			}
		}
		CVectorT<double> v;
		v.Copy(vs);
		return v;
	}

private:

	/**
	 * [検索] 改行位置検索.
	 *		指定文字列から改行の位置を検索します
	 *	@note ￥r(0x0D), ￥n(0x0A), ￥r￥n(0x0D,0x0A) いずれも対応します。
	 *	@param lpsz 文字列（開始ポインタ）
	 *	@retval NULL 改行なし
	 *	@retval NULL以外 改行の位置。
	 */
	inline const char* m_FindCr(const char* lpsz) const
	{
		while ( *lpsz != 0 )
		{
			if ( *lpsz == 0x0D )
			{
				return lpsz;
			}
			lpsz ++;
		}
		return NULL;
	}

	/**
	 * [検索] カンマ位置検索.
	 *		指定文字列から改行の位置を検索します
	 *	@note ￥r(0x0D), ￥n(0x0A), ￥r￥n(0x0D,0x0A) いずれも対応します。
	 *	@param lpsz 文字列（開始ポインタ）
	 *	@retval NULL 改行なし
	 *	@retval NULL以外 改行の位置。
	 */
	inline INT_PTR m_FindComma(const char* lpsz) const
	{
		INT_PTR r = 0;
		while ( *lpsz != 0 )
		{
			if ( *lpsz == ',' )
			{
				return r;
			}
			lpsz++;
			r++;
		}
		return -1;
	}

	/**
	 * [取得] 一行取り出し
	 *	@note 指定データから一行を取り出します
	 *	@param[out]	_strLine 取得した一行
	 *	@param[in]	lpsz データの頭
	 *	@return 改行の次のポインタ。末ならNULLが返ります。
	 */
	const char* m_GetLine(CStrT<char>& _strLine, const char* lpsz)
	{
		_strLine.Empty();
		const char* P = NULL;
		P = m_FindCr(lpsz);
		if ( P == NULL )
		{
			//見つからなかった。全部が一行
			_strLine = lpsz;
		}
		else
		{
			INT_PTR iLen = P - lpsz;
			if ( iLen > 0 )
			{
				char* Q = _strLine.GetBuffer(iLen);
				memcpy(Q, lpsz, iLen);
				Q[iLen] = 0;
				_strLine.ReleaseBuffer();
			}
			if ( *P++ == 0x0D )
			{
				if ( *P == 0x0A )
				{
					P++;
				}
			}
		}
		return P;
	}

	/**
	 * [取得] 文字列毎に分解
	 *	@param lpsz データの頭
	 *	@return 改行ごとに分解した文字列の配列
	 */
	CVectorT<CStrT<char> > m_SeparateLine(const char* lpsz)
	{
		CBlockLinkVectorT<CStrT<char> > vs;
		CStrT<char> s;
		const char* P = lpsz;
		while ( P != NULL )
		{
			P = m_GetLine(s, P);
			vs.Add(s);
		}
		CVectorT<CStrT<char> > v;
		v.Copy(vs);
		return v;
	}

	CStrVector m_content;
};



}; // TNB