#pragma once
/**
 *@file
 * 文字列操作関係のヘッダ
 *
 *		文字列を操作する関数群があります。	
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"
#include "TnbBlockLinkVector.h"
#include "TnbCollectionAdapter.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * 文字列操作クラス
 *
 *		文字列操作関数群を クラス関数で持っています。
 *
 *	@par必要ファイル
 *			TnbStrOperator.h
 * 
 *	@date 06/05/05	新規作成
 *	@date 06/10/27	テンプレート化
 *	@date 07/07/07	クラス名変更。
 */
template<typename TYP>
class CStrOperatorT
{
public:

	/**
	 * [検索] 改行位置検索.
	 *		指定文字列から改行の位置を検索します
	 *	@note ￥r(0x0D), ￥n(0x0A), ￥r￥n(0x0D,0x0A) いずれも対応します。
	 *	@param lpsz 文字列（開始ポインタ）
	 *	@retval NULL 改行なし
	 *	@retval NULL以外 改行の位置。
	 */
	static const TYP* FindCr(const TYP* lpsz)
	{
		if ( lpsz != NULL )
		{
			while ( *lpsz != 0 )
			{
				if ( *lpsz == 0x0D || *lpsz == 0x0A )
				{
					return lpsz;
				}
				lpsz += STRLIB::GetCharSize(*lpsz);
			}
		}
		return NULL;
	}

	/**
	 * [取得] 一行取り出し
	 *	@note 指定データから一行を取り出します
	 *	@param[out]	_strLine 取得した一行
	 *	@param[in]	lpsz データの頭
	 *	@return 改行の次のポインタ。末ならNULLが返ります。
	 */
	static const TYP* GetLine(CStrT<TYP>& _strLine, const TYP* lpsz)
	{
		_strLine.Empty();
		const TYP* P = NULL;
		if ( lpsz != NULL )
		{
			P = FindCr(lpsz);
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
					_strLine.SetFromLeft(lpsz, iLen);
				}
				if ( *P++ == 0x0D )
				{
					if ( *P == 0x0A )
					{
						P++;
					}
				}
			}
		}
		return P;
	}

	/**
	 * [取得] 各行連結
	 *	@note 各行を改行で連結します。
	 *	@param vs 各行情報
	 *	@param lpszCR 改行コード。省略すると 0x0D, 0x0A になります。
	 *	@return 連結結果
	 */
	static CStrT<TYP> ConnectLines(const CVectorT<CStrT<TYP> >& vs, const TYP* lpszCR = NULL)
	{
		CStrT<TYP> CR = lpszCR;
		if ( lpszCR == NULL )
		{
			CR = "\r\n";
		}
		CStrT<TYP> s;
		loop ( i, vs.GetSize() )
		{
			if ( i != 0 )
			{
				s += CR;
			}
			s += vs[i];
		}
		return s;	
	}

	/**
	 * [取得] 改行毎分離
	 *	@note 指定データを改行ごとに分離します。
	 *	@param lpsz	データの頭
	 *	@return 分離した結果
	 */
	static CVectorT<CStrT<TYP> > SeparateLine(const TYP* lpsz)
	{
		CBlockLinkVectorT<CStrT<TYP> > vs;
		CStrT<TYP> s;
		const TYP* P = lpsz;
		while ( P != NULL )
		{
			P = GetLine(s, P);
			vs.Add(s);
		}
		CVectorT<CStrT<TYP> > v;
		v.Copy(vs);
		return v;
	}

	/**
	 * [取得] トークン区切取得
	 *	@note 指定文字列を指定区切り文字で分割し取り出します。
	 *	@param lpsz		分割元文字列
	 *	@param lpszMark		区切り文字セット
	 *	@param isCheckDc	trueなら ダブルコーテーションを考慮。
	 *						その間の区切り文字は区切り文字として扱いません。
	 *						また、くくっているダブルコーテーションも削除します。
	 *						(ただし、区切り文字とダブルコーテーションの間にスペースがあると、認識されません)
	 *	@return 分割した文字列
	 */
	static CVectorT<CStrT<TYP> > SeparatePeriod(const TYP* lpsz, const TYP* lpszMark, bool isCheckDc = true)
	{
		CBlockLinkVectorT<CStrT<TYP> > vs;
		CStrT<TYP> s;
		if ( lpsz[0] != 0 )
		{
			while ( true )
			{
				if ( isCheckDc && lpsz[0] == '\"' )
				{
					//先頭が " だった。
					INT_PTR r = STRLIB::IndexOf(lpsz, '\"', 1);
					if ( r >= 0 )
					{
						//次の " を発見
						bool isEnd = (lpsz[r + 1] == 0);
						if ( STRLIB::IndexOf(lpszMark, lpsz[r + 1]) >= 0 || isEnd )
						{
							// ,"～", を発見
							s.SetFromLeft(&lpsz[1], r - 1);
							lpsz += r + 2;
							vs.Add(s);
							if ( isEnd )
							{
								break;
							}
							continue;
						}
					}
				}
				INT_PTR r = STRLIB::IndexOneOf(lpsz, lpszMark);
				if ( r < 0 )
				{
					s = lpsz;
				}
				else
				{
					s.SetFromLeft(lpsz, r);
					lpsz += r + 1;
				}
				vs.Add(s);
				if ( r < 0 )
				{
					break;
				}
			}
		}
		CVectorT<CStrT<TYP> > v;
		v.Copy(vs);
		return v;
	}

	/**
	 * [取得] トークン区切取得
	 *	@note 指定文字列を指定区切り文字で分割し取り出します。
	 *	@param lpsz		分割元文字列
	 *	@param mark		区切り文字
	 *	@param isCheckDc	trueなら ダブルコーテーションを考慮。
	 *						その間の区切り文字は区切り文字として扱いません。
	 *						また、くくっているダブルコーテーションも削除します。
	 *						(ただし、区切り文字とダブルコーテーションの間にスペースがあると、認識されません)
	 *	@return 分割した文字列
	 */
	static CVectorT<CStrT<TYP> > SeparatePeriod(const TYP* lpsz, TYP mark, bool isCheckDc = true)
	{
		TYP bb[2] = { mark, 0 };
		return SeparatePeriod(lpsz, bb, isCheckDc);
	}

	/**
	 * [作成] バイナリ→HEX文字列
	 *	@note 指定データをHEX文字列表記文字列に変換します。
	 *	@param c データ
	 *	@return HEX文字列
	 */
	static CStrT<TYP> BinaryToHexString(const IConstCollectionT<BYTE>& c)
	{
		size_t size = c.GetSize();
		CStrT<TYP> str;
		TYP* P = str.GetBuffer(size * 2 + 2);
		loop ( i, size )
		{
			const BYTE& b = c.At(i);
			*P++ = STRLIB::IntToHexChar(b >> 4);
			*P++ = STRLIB::IntToHexChar(b);
		}
		*P++ = 0;
		str.ReleaseBuffer();
		return str;
	}

	/**
	 * [作成] バイナリ→HEX文字列
	 *	@note 指定データをHEX文字列表記文字列に変換します。
	 *	@param dataSize データサイズ
	 *	@param P データアドレス
	 *	@return HEX文字列
	 */
	static CStrT<TYP> BinaryToHexString(size_t dataSize, LPCVOID P)
	{
		CConstAdapterT<BYTE> cc(dataSize, static_cast<const BYTE*>(P));
		return BinaryToHexString(cc);
	}

	/**
	 * [作成] HEX文字列→バイナリ
	 *	@note 指定データをHEX文字列表記文字列に変換します。
	 *	@note アンダーバー、スペース、タブ、改行、ハイフンは、無視します。
	*	@note 途中でHEX以外の文字があると、そこまでの処理をします。
	 *	@param lpszHex HEX文字列
	 *	@return バイナリデータ。
	 */
	static CByteVector HexStringToBinary(const TYP* lpszHex)
	{
		CByteVector vb;
		while ( true )
		{
			TYP c = *lpszHex++;
			if ( c == '_' || c ==' ' || c == '\t' || c =='\n' || c == '\r' || c =='-' )
			{
				continue;
			}
			int r = STRLIB::HexCharToInt(c);
			if ( r < 0 )
			{
				break;
			}
			vb.Add(static_cast<BYTE>(r));
		}
		CByteVector vbr;
		if ( ! vb.IsEmpty() )
		{
			INDEX startIndex = 0;
			size_t size = vb.GetSize();
			if ( (size & 1) != 0 )
			{
				vbr.Add(vb[0]);
				startIndex = 1;
			}
			for ( INDEX i = startIndex; i < size; i += 2 )
			{
				vbr.Add(static_cast<BYTE>((vb[i] << 4) | vb[i + 1]));
			}
		}
		return vbr;
	}

	/**
	 * タブコードをスペースに変換します。<BR>
	 * 改行は含まないようにしてください。
	 * @param lpsz 元のテキスト。
	 * @param iTabStep タブステップ数。省略すると４になります。
	 * @return タグコードの含まない文字列。
	 */
	static CStrT<TYP> TabToSpace(const TYP* lpsz, int iTabStep = 4)
	{
		CStrT<TYP> strWork;
		strWork = lpsz;
		while ( true )
		{
			INT_PTR iTabPosition = strWork.Find('\t');
			if ( iTabPosition < 0)
			{
				break;
			}
			strWork = 
					strWork.Left(iTabPosition) 
					+ CStrT<TYP>::Lineup(' ', iTabStep - (iTabPosition % iTabStep))
					+ &(strWork.operator const TYP*()[iTabPosition + 1]);
		}
		return strWork;
	}

private:
	friend class CStrOperatorTest;
};



/**
 * 簡易文字列操作関数群
 *
 *	@par必要ファイル
 *			TnbStrOperator.h
 * 
 *	@date 06/10/27	テンプレート化
 */
typedef CStrOperatorT<TCHAR> CStrOperator;

/**
 * 簡易文字列操作関数群(ASCII/SJIS)
 *
 *	@par必要ファイル
 *			TnbStrOperator.h
 * 
 *	@date 06/10/27	テンプレート化
 */
typedef CStrOperatorT<char> CAsciiOperator;

/**
 * 簡易文字列操作関数群(UNICODE)
 *
 *	@par必要ファイル
 *			TnbStrOperator.h
 * 
 *	@date 06/10/27	テンプレート化
 */
typedef CStrOperatorT<WCHAR> CUnicodeOperator;



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")

