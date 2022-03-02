#pragma once
/**
 *@file
 * 図形管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbVector.h"
#include "TnbPair.h"



//TNB Library
namespace TNB	
{



/**@ingroup VALUE COLLECT
 * 図形管理計算クラス
 *
 *	@todo 未完成。
 *
 *		連続した座標を持ち、図形を表します。
 *
 *	@param T 長さを管理する型。
 *	@param TR 面積を管理する型。省略すると T が指定されます。
 *
 *	@note 通常、 T は double か CValueDistance を想定しています。 
 *			T が CValueDistance の場合、 TR は CValueArea を想定しています。
 *
 *	@note CVectorT の TYP は CPairT<T, T> になります。
 *
 *	@par必要ファイル
 *			TnbFigurePointer.h
 * 
 *	@date 07/01/23 新規作成
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename T, typename TR = T>
class CFigurePointerT : public CVectorT<TNB::CPairT<T,T> >
{
	typedef CVectorT<TNB::CPairT<T,T> > _super; ///< 親クラス名再定義
	DEFSUPER(_super);

	/**
	 * [計算] 斜め距離計算 (√(水平距離^2 + 垂直距離^2))
	 *	@param x 水平距離
	 *	@param y 垂直距離
	 *	@return 斜め距離
	 */
	friend double CalcSlantingDistance(const double& x, const double& y)
	{
		return ::sqrt(x * x + y * y);
	}

public:

	typedef TNB::CPairT<T,T> CPointer;	///< 座標管理型 <BR> first = x座標 <BR> second = y座標

	/// コンストラクタ
	CFigurePointerT(void) : _super()
	{
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	virtual INDEX Add(const CPointer& t)
	{
		return _super::Add(t);
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param x x座標。
	 *	@param y y座標。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX Add(const T& x, const T& y)
	{
		return _super::Add(CPointer(x, y));
	}
	
	/**
	 * [追加] 要素一つ追加.
	 *	@param t 要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX AddEx(const CPointer& t)
	{
		return _super::AddEx(t);
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@param x x座標。
	 *	@param y y座標。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX AddEx(const T& x, const T& y)
	{
		return _super::AddEx(CPointer(x, y));
	}
	
	/**
	 * [取得] X座標の取得.
	 *	@param index インデックス
	 *	@return X座標の複製
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	T X(INDEX index) const
	{
		return GetEx(index).first;
	}

	/**
	 * [取得] Y座標の取得.
	 *	@param index インデックス
	 *	@return Y座標の複製
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	T Y(INDEX index) const
	{
		return GetEx(index).second;
	}

	/**
	 * [検索] 外接する四角形作成
	 *		オブジェクトに外接する四角形の座標を取得します。
	 *	@return Y最小のIndex
	 */
	int FindTop(void) const
	{
		int r = -1;
		T tMin(0);
		loop ( i, GetSize() )
		{
			const T& t = At(i).second;
			if ( tMin > t )
			{
				tMin = t;
				r = i;
			}
		}
		return r;
	}

	/**
	 * [計算] 範囲内面積計算.
	 *		登録された座標のTopとBottomをつないで囲んだ面積を計算します。
	 *	@return 面積(TR)
	 */
	TR CalcArea(void) const
	{
		EXCLUSIVE(this);
		TR rr;
		int size = GetSize();
		if ( size > 2 )
		{
			TR r(0);
			const CPointer* p = &At(0);
			const T& bx = p->first;
			const T& by = p->second;
			p = &At(1);
			for ( int i = 2; i < size; i++ )
			{
				const CPointer& a = At(i);
				r += (((p->first - bx) * (a.second - by)) - ((a.first - bx) * (p->second - by))) / 2;
				p = &a;
			}
			if ( r < TR(0) )
			{
				r *= -1;
			}
			rr = r;
		}
		return rr;
	}

	/**
	 * [計算] 総距離計算.
	 *		登録された座標をつないだ距離を計算します。
	 *	@param boIsLoop true を指定すると最初と最後をつないだ距離を計算します。
	 *	@return 距離
	 */
	T CalcDistance(bool boIsLoop = false) const
	{
		EXCLUSIVE(this);
		T r(0);
		int size = GetSize();
		if ( size > 1 )
		{
			const CPointer* p = &At(0);
			for ( int i = 1; i < size; i++ )
			{
				const CPointer& a = At(i);
				r += CalcSlantingDistance(p->first - a.first, p->second - a.second);
				p = &a;
			}
			if ( boIsLoop )
			{
				const CPointer& a = At(0);
				r += CalcSlantingDistance(p->first - a.first, p->second - a.second);
			}
		}
		return r;
	}
};



}; //TNB


