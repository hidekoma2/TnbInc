#pragma once
/**
 *@file
 * 静的簡易マップ管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * 静的簡易マップクラス.
 *
 *		静的なマップを作成し、キーで値を検索するためのフレームです。
 *		(クラス的な使い方にはしません）
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		const CStaticMapT<int,LPCTSTR> tMap[] = 
 *		{
 *			0,		_T(""),
 *			1,		_T("def文処理中"),
 *			2,		_T("dim文処理中"),
 *			3,		_T("func文処理中"),
 *		};
 *		CString s = CStaticMapT<int,LPCTSTR>::Find(2, tMap);			
 *		TRACE1("[%s]\n", s);
 *		</pre></td></tr></table>
 *
 *	@param TYP キーの型。型は比較機能が必須です。
 *	@param VAL 値の型。型はコピー機能が必須です。省略すると、 LPCTSTR になります。
 *
 *	@par必要ファイル
 *			TnbStaticMap.h
 *
 *	@date 07/06/29	新規
 *	@date 08/09/01	デフォルトの値をテンプレートではなく、メソッドで指定するように変更。
 */
template<typename TYP, typename VAL = LPCTSTR>
class CStaticMapT
{
public:

	TYP			key;	///< キー
	VAL			val;	///< 値

	/**
	 * [取得] 取得
	 *	@param[out] _key 取得したキー
	 *	@param[out] _val 取得した値
	 *	@param[in] index インデックス
	 *	@param ts 検索対象のマップテーブル。本クラスの配列である必要があります。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。
	 */
	template<typename TYPS>
	static bool Get(TYP& _key, VAL& _val, INDEX index, const TYPS& ts)
	{
		const CStaticMapT* P = ts;		//正しい ts を指定していないとここでエラー。
		IgnoreUnusedValue(P);
		if ( index < sizeof(ts) / sizeof(CStaticMapT) )
		{
			_key = ts[index].key;
			_val = ts[index].val;
			return true;
		}
		return false;
	}
	
	/**
	 * [検索] 値検索
	 *	@param key 検索するキー
	 *	@param ts 検索対象のマップテーブル。本クラスの配列である必要があります。
	 *	@param def なかったときの値になります。省略すると VAL() になります。
	 *	@retval defの値 見つからず。
	 *	@retval 上記以外 key に対する値。
	 */
	template<typename TYPS>
	static VAL Find(const TYP& key, const TYPS& ts, const VAL& def = VAL())
	{
		const CStaticMapT* P = ts;		//正しい ts を指定していないとここでエラー。
		IgnoreUnusedValue(P);
		loop ( i, sizeof(ts) / sizeof(CStaticMapT) )
		{
			if ( ts[i].key == key )
			{
				return ts[i].val;
			}
		}
		return def;
	}
	
	/**
	 * [検索] 値検索
	 *	@param key 検索するキー
	 *	@param ts 検索対象のマップテーブル。本クラスの配列である必要があります。
	 *	@param def なかったときの値になります。省略すると VAL() になります。
	 *	@retval defの値 見つからず。
	 *	@retval 上記以外 key に対する値。
	 */
	template<typename TYPS>
	static VAL FindString(const TYP& key, const TYPS& ts, const VAL& def = VAL())
	{
		const CStaticMapT* P = ts;		//正しい ts を指定していないとここでエラー。
		loop ( i, sizeof(ts) / sizeof(CStaticMapT) )
		{
			if ( STRLIB::Compare(ts[i].key, key) == 0 )
			{
				return ts[i].val;
			}
		}
		return def;
	}

	/**
	 * [検索] キー検索
	 *	@param val 検索する値
	 *	@param ts 検索対象のマップテーブル。本クラスの配列である必要があります。
	 *	@param def なかったときの値になります。省略すると TYP() になります。
	 *	@retval defの値 見つからず。
	 *	@retval 上記以外 val に対するキー。
	 */
	template<typename TYPS>
	static TYP FindKey(const VAL& val, const TYPS& ts, const TYP& def = TYP())
	{
		const CStaticMapT* P = ts;		//正しい ts を指定していないとここでエラー。
		IgnoreUnusedValue(P);
		loop ( i, sizeof(ts) / sizeof(CStaticMapT) )
		{
			if ( ts[i].val == val )
			{
				return ts[i].key;
			}
		}
		return def;
	}
};



}; // TNB
