#pragma once
/**
 *@file
 * 比較機能関係のヘッダ
 *
 *		比較するためのインターフェースがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * 比較機能インターフェース.
 *
 *		このインターフェースを継承し、 Compare() をオーバーロードすることで、
 *		その型に比較機能を実装することが出来ます( == や <= などで比較可能)。
 *
 *	@par使用例
 *		比較結果を Compare() に実装します。
 *	@par
 *		<table><tr><td><pre>
 *
 *		struct TMyType : IComparableT<TMyType>
 *		{
 *		    int i;
 *		    INT_PTR Compare(const TMyType& t) const		
 *		    {
 *		        return i - t.i;
 *		    }
 *		};		
 *		</pre></td></tr></table>
 *
 *	@param TYP 実装するクラスを指定します。
 *
 *	@par必要ファイル
 *			TnbComparable.h
 *
 *	@date 06/05/01 新規
 *	@date 06/09/13 IsEqual() も virtual 宣言に変更。
 */
template<typename TYP>
struct IComparableT
{
	/// デストラクタ
	virtual ~IComparableT(void) {}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const TYP& t) const = 0;

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval true 自分は t と同じ
	 *	@retval false 自分は t と異なる
	 */
	virtual bool IsEqual(const TYP& t) const { return Compare(t) == 0; }
	
	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	INT_PTR operator-(const TYP& t) const { return Compare(t); }
	
	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じ
	 *	@retval false	自分は t と異なる
	 */
	bool operator==(const TYP& t) const { return IsEqual(t); }
	
	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval true	自分は t と異なる
	 *	@retval false	自分は t と同じ
	 */
	bool operator!=(const TYP& t) const { return ! IsEqual(t); }
	
	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval true	自分は t より小さい
	 *	@retval false	自分は t と同じか大きい
	 */
	bool operator<(const TYP& t) const { return Compare(t) < 0; }
	
	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じか小さい
	 *	@retval false	自分は t より大きい
	 */
	bool operator<=(const TYP& t) const { return Compare(t) <= 0; }
	
	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval true	自分は t より大きい
	 *	@retval false	自分は t と同じか小さい
	 */
	bool operator>(const TYP& t) const { return Compare(t) > 0; }
	
	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じか大きい
	 *	@retval false	自分は t より小さい
	 */
	bool operator>=(const TYP& t) const { return Compare(t) >= 0; }
};



};
