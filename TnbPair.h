#pragma once
/**
 *@file
 * ペア型関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSerializer.h"
#include "TnbComparable.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * ペア型テンプレートクラス
 *
 *		キーと値を保持するだけの型です。
 *		主にCMapT の配列保持する型として使用しています。
 *	
 *	@param	FIRST	一つ目の型。クラスの場合コピー機能、比較機能( IComparableT か、引き算)が必須。
 *	@param	SECOND	二つ目の型。クラスの場合コピー機能が必須。
 *
 *	@par必要ファイル
 *			TnbPair.h
 *
 *	@date 06/04/21 新規
 *	@date 06/05/19 IComparableT 、実装。
 *	@date 06/08/30 ISerializable 、実装。
 *	@date 07/01/25 コンストラクタを一つ追加。変数名を KEY,VALから1st 2nd に変更。
 */
template<typename FIRST, typename SECOND>
class CPairT : public IComparableT< TNB::CPairT<FIRST, SECOND> >, public ISerializable
{
public:

	FIRST		first;		///< 一つ目の型の値
	SECOND		second;		///< 二つ目の型の値

	/// コンストラクタ
	CPairT(void) : first(FIRST()), second(SECOND())
	{
	}

	/**
	 * コンストラクタ
	 *	@param v1 FIRST の値
	 *	@param v2 SECOND の値
	 */
	CPairT(const FIRST& v1, const SECOND& v2) : first(v1), second(v2)
	{
	}

	/** 
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CPairT(const CPairT& other) : first(other.first), second(other.second) 
	{
	}

	/** 
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CPairT& operator=(const CPairT& other)
	{
		first	= other.first;
		second	= other.second; 
		return *this;
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CPairT& t) const
	{
		INT_PTR r = IComparatorT<FIRST>::GetDefault().CompareTo(first, t.first);
		if ( r != 0 )
		{
			return r;
		}
		return IComparatorT<SECOND>::GetDefault().CompareTo(second, t.second);
	}

	/**
	 * [通知] シリアライズ
	 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << first << second;
	}

	/**
	 * [通知] デシリアライズ
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> first >> second;
	}
};



}; // TNB
