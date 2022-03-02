#pragma once
/**
 *@file
 * マップファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMap.h"
#include "TnbStrVector.h"
#include "TnbTextFile.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * マップファイルクラス.
 *
 *		CMapT の情報を csvファイルで保存、読込みするためのクラスです。
 *		継承し KeyToString(), ValueToString(), StringToKey(), StringToValue() をオーバーライドする必要があります。
 *
 *	@param	KEY	キーとする型。クラスの場合コピー機能、比較機能( IComparableT か、引き算)が必須。
 *	@param	VAL	値とする型。クラスの場合コピー機能が必須。
 *	@param	INK	キーとする型の入力型。省略可能。
 *
 *	@par必要ファイル
 *			TnbMapFile.h
 *
 *	@date 15/03/10	暫定新規
 */
template<typename KEY, typename VAL, typename INK=KEY>
class CMapFileT
{
public:
	/**
	 * [保存] ファイル保存.
	 *		指定の map をファイルに保存します。
	 *	@param lpszFileName ファイル名
	 *	@param map マップ
	 *	@param comma カンマ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Save(LPCTSTR lpszFileName, const CMapT<KEY, VAL, INK>& map, TCHAR comma = ',')
	{
		CStr s;
		CStrVector vs;
		loop ( i, map )
		{
			s = KeyToString(map.At(i).first);
			s += period;
			s += ValueToString(map.At(i).second);
			vs.Add(s);
		}
		return CTextFileWriter::NewFile(lpszFileName, vs);
	}

	/**
	 * [読込] ファイル読込み.
	 *		指定のファイルを map に読込みます。
	 *	@param[out] _map マップ
	 *	@param[in] lpszFileName ファイル名
	 *	@param[in] comma カンマ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Load(CMapT<KEY, VAL, INK>& _map, LPCTSTR lpszFileName, TCHAR comma = ',')
	{
		_map.RemoveAll();
		CTextFileReader r;
		if ( r.Read(lpszFileName) )
		{
			CConstStrVector vs;
			if ( r.GetContents(vs) )
			{
				CStrVector v;
				loop ( i, vs.GetLineCount() )
				{
					v = CStrOperator::SeparatePeriod(vs.GetString(i), period);
					if ( v.GetSize() >= 2 )
					{
						KEY key = StringToKey(v[0]);
						VAL val = StringToValue(v[1]);
						_map[key] = val;
					}
				}
				return true;
			}
		}
		return false;
	}

protected:
	/**
	 * [変換] キーを文字列化.
	 *	@param key キー
	 *	@return 文字列
	 */
	virtual CStr KeyToString(const KEY& key) = 0;

	/**
	 * [変換] 値を文字列化.
	 *	@param val 値
	 *	@return 文字列
	 */
	virtual CStr ValueToString(const VAL& val) = 0;

	/**
	 * [変換] 文字列をキー化.
	 *	@param lpsz 文字列
	 *	@return キー
	 */
	virtual KEY StringToKey(LPCTSTR lpsz) = 0;

	/**
	 * [変換] 文字列を値化.
	 *	@param lpsz 文字列
	 *	@return 値
	 */
	virtual VAL StringToValue(LPCTSTR lpsz) = 0;
};



};
