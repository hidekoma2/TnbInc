#pragma once
/**
 *@file
 * TinyMacroLang 計算関係のヘッダ
 *	
 *		TMLの計算のクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTmlResult.h"
#include "TnbTmlValue.h"
#include "TnbStrLib.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang 文字列解析
 *
 *		文字列表記を TML 用に変換するために使用します。
 *
 *	@par必要ファイル
 *			TnbTmlTextEscape.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
class CTmlTextEscape : public CTml
{
public:

	/// コンストラクタ
	CTmlTextEscape(void)
	{
	}

	/**
	 * コピーコンストラクタ;
	 *	@param other コピー元
	 */
	CTmlTextEscape(const CTmlTextEscape& other) : m_contents(other.m_contents), m_aWildPos(other.m_aWildPos)
	{
	}

	/// デストラクタ
	virtual ~CTmlTextEscape(void)
	{
	}

	/// Empty
	void Empty(void)
	{
		m_contents.RemoveAll();
		m_aWildPos.RemoveAll();
	}

	/** 
	 * [取得] CTmlValue 型で取得.
	 *		保持しているデータを CTmlValue に変換します。
	 *	@return 変換後の CTmlValue
	 */
	CTmlValue ToValue(void) const
	{
		CTmlValue v;
		v = m_contents;
		loop ( i, m_aWildPos.GetSize() )
		{
			v.AddWildPoint(m_aWildPos[i]);
		}
		return v;
	}
	
	/**
	 * [代入] 文字列を代入.
	 *		本インスタンスに指定文字列を格納します。
	 *		その際、エスケープシーケンスをチェックします。
	 *	@param lpszData 代入文字列
	 *	@return 結果。 Result_Nop は返りません。
	 */
	CTmlResult SetData(LPCSTR lpszData)
	{
		return m_SetNormal(lpszData);
	}

	/**
	 * [代入] HEX表記文字列を代入.
	 *		本インスタンスに指定された文字列を HEX表記として解析し、格納します。
	 *	@param lpszData 代入文字列
	 *	@return 結果。 Result_Nop は返りません。
	 */
	CTmlResult SetHexToData(LPCSTR lpszData)
	{
		return m_SetHex(lpszData);
	}

private:
	CByteVector		m_contents;	///< 内容
	CVectorT<INDEX>	m_aWildPos;	///< ワイルドの位置

	/**
	 * 文字列を代入
	 *	@param lpszData 文字列
	 *	@return 結果。
	 */
	CTmlResult m_SetNormal(LPCSTR lpszData)
	{
		Empty();
		CByteVector ba;
		int iPos = 0;
		char c;
		while ( true )
		{
			c = lpszData[iPos++];
			if ( c == 0 )
			{
				//終端か " なら終了
				break;
			}
			else if ( STRLIB::GetCharSize(c) == 2 )
			{
				//全角
				ba.Add(c);
				ba.Add(lpszData[iPos++]);
			}
			else if ( c=='\\' )
			{
				//次の文字。
				c = lpszData[iPos++];
				switch ( c )
				{
				case '0':
					ba.Add(0);
					break;
				case 'x':
					//HEX表記
					{
						int r = 0;
						int i;
						c = lpszData[iPos];
						if ( c == 0 )
						{
							//ESCコードが不明
							return CTmlResult(Operation_Text_EscHex, Result_InvalidEscSeq);
						}
						i = STRLIB::HexCharToInt(c);
						if ( i < 0 )
						{
							// HEX表記が異常
							return CTmlResult(Operation_Text_EscHex, Result_InvalidHexNotation);
						}
						r = i * 0x10;
						c = lpszData[iPos + 1];
						if ( c == 0 )
						{
							//ESCコードが不明
							return CTmlResult(Operation_Text_EscHex, Result_InvalidEscSeq);
						}
						i =  STRLIB::HexCharToInt(c);
						if ( i < 0 )
						{
							// HEX表記が異常
							return CTmlResult(Operation_Text_EscHex, Result_InvalidHexNotation);
						}
						r += i;
						iPos += 2;
						ba.Add(static_cast<BYTE>(r));
					}
					break;
				case '*':
					//ワイルドカード
					m_aWildPos.Add(ba.GetSize());
					ba.Add('_' );
					break;
				default:
					//ESCコードが不明
					return CTmlResult(Operation_Text_Esc, Result_InvalidEscSeq);
				}
			}
			else
			{
				ba.Add(c);
			}
		}
		m_contents = ba;
		return Result_Success;
	}

	/**
	 * HEX指定で代入
	 * "ABCD" を代入すると、0xAB 0xCD として登録
	 *	ワイルドカードは使えません。
	 *	@param lpszData 文字列
	 *	@return 結果。
	 */
	CTmlResult m_SetHex(LPCSTR lpszData)
	{
		Empty();
		//
		CAscii s = lpszData;
		s.Remove(' ');
		s.Remove('-');
		s.Remove('_');
		lpszData = s;
		//
		CByteVector ba;
		size_t max = STRLIB::GetLen(lpszData);
		for ( UINT i = 0; i < max; i += 2 )
		{
			if ( lpszData[i] == 0 )
			{
				break;
			}
			int r1 = STRLIB::HexCharToInt(lpszData[i]);
			int r2 = STRLIB::HexCharToInt(lpszData[i + 1]);
			if ( r1 >= 0 && r2 >= 0 )
			{
				//HEX表記だ!
				ba.Add (static_cast<BYTE>(r1 * 0x10 + r2));
			}
			else
			{
				// HEX表記が異常
				return CTmlResult(Operation_Text, Result_InvalidHexNotation);
			}
		}
		m_contents = ba;
		return Result_Success;
	}

	friend class CTmlTextEscapeTest;
};



}; // TNB


