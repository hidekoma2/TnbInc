#pragma once
/**
 *@file
 * 文字列情報配列管理関係のヘッダ
 *
 *		文字列の配列を管理します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBlockLinkVector.h"



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * 不変的文字列情報配列管理クラス.
 *
 *		一つのテキストから、敷居文字において分割し配列として記憶します。
 *
 *	@par必要ファイル
 *			TnbConstStrVector.h
 * 
 *	@date 09/08/31	新規作成
 */
template<typename TYP, TYP C1 = 0x0D, TYP C2 = 0x0A>
class CConstStrVectorT : CCopyImpossible
{
public:

	/// コンストラクタ
	CConstStrVectorT(void) : m_lpszText(NULL)
	{
	}

	/**
	 * [設定] 空っぽ化.
	 *		保持しているデータを破棄します。
	 */
	void Empty(void)
	{
		m_work.Free();
		m_params.RemoveAll();
	}

	/**
	 * [設定] データ設定.
	 *		指定の文字列データを複製し、記憶します。
	 *	@param lpszText 文字列データ
	 *	@return 分割数。
	 */
	size_t SetClone(const TYP* lpszText)
	{
		size_t len = STRLIB::GetLen(lpszText);
		m_work.Resize(len + 1);
		STRLIB::Copy(m_work, lpszText);
		return SetRefered(m_work);
	}

	/**
	 * [設定] データ設定.
	 *		指定の文字列データのアドレスを記憶し使用します。
	 *		そのため、本クラス使用中は、指定した文字列データを破棄しないでください。
	 *	@param lpszText 文字列データ
	 *	@return 分割数。
	 */
	size_t SetRefered(TYP* lpszText)
	{
		m_lpszText = lpszText;
		m_params.RemoveAll();
		INDEX ind = 0;
		while ( true )
		{
			TYP* P = lpszText;
			size_t len = 0;
			while ( true )
			{
				if ( *P == 0 )
				{
					if ( len == 0 )
					{
						return m_params.GetSize();
					}
					else
					{
						break;
					}
				}
				else if ( *P == C1 || *P == C2 )
				{
					*P++ = 0;
					if ( C2 != 0 && *P == C2 )
					{
						*P++ = 0;
					}
					break;
				}
				int s = STRLIB::GetCharSize(*P);
				P += s;
				len += s;
			}
			m_params.Add(TParam(ind, len));
			ind += P - lpszText;
			lpszText += P - lpszText;
		}
		ASSERT( false );
		return 0;
	}

	/**
	 * [取得] ライン数取得.
	 *	@return ライン数
	 */
	size_t GetLineCount(void) const
	{
		return m_params.GetSize();
	}

	/**
	 * [取得] 長さ取得.
	 *	@param index インデックス。０以上 GetLength() 未満。
	 *	@return 長さ
	 */
	size_t GetLength(INDEX index) const
	{
		return m_params[index].size;
	}

	/**
	 * [取得] 文字列取得.
	 *	@param index インデックス。０以上 GetLength() 未満。
	 *	@return 文字列
	 */
	const TYP* GetString(INDEX index) const
	{
		return &m_lpszText[m_params[index].index];
	}

private:
	struct TParam
	{
		INDEX	index;
		size_t	size;
		// コンストラクタ.
		TParam(INDEX i = 0, size_t s = 0) : index(i), size(s)
		{
		}
	};
	CBlockLinkVectorT<TParam>	m_params;
	TYP*						m_lpszText;
	CWorkMemT<TYP>				m_work;
};



/**
 * 不変的文字列情報配列管理クラス.
 *
 *	@par必要ファイル
 *			TnbConstStrVector.h
 * 
 *	@date 09/08/31	新規作成
 */
typedef CConstStrVectorT<TCHAR> CConstStrVector;



}; // TNB