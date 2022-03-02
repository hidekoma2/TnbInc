#pragma once
/**
 *@file
 * 情報変換関係のヘッダ
 *
 *		ある型からある型に変換するためのクラスがあります。
 *		コレクションの要素を、一括で変換することも出来ます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"
#include "TnbPair.h"



namespace TNB
{



/**@ingroup COLLECT
 * 情報変換ベースクラス
 *
 *		指定の SRC型 を 指定の DST型に変換するクラスのベースです。
 *		Converter() を オーバーライドして、期待する変換クラスを宣言し使用します。
 *
 *	@par使用例
 *		\code
 *		class CMyConv : public CConverterT<int, CStr> 
 *		{
 *			//CStr→intのコンバータ
 *			virtual void  Converter(int& r, const CStr& src)			
 *			{
 *				r = src.ToInt();
 *			}
 *	 	};
 *		\endcode
 *
 *		SRC 型を持つ IConstCollectionT や、DST型を持つ、 ICollectionT を指定することで、
 *		コレクション内の情報を全て変換することも出来ます(これが本クラスの目的)。
 *
 *		また、コレクションのネストも変換可能です。
 *
 *		なお、 CMapT の場合、キーは型は同じ、値の型を SRC と DST のものを指定します。
 *		
 *	@par使用例
 *		\code
 *		CVectorT<CMapT<char, int> > vmapi;
 *		CVectorT<CMapT<char, CStr> > vmaps;
 *				;
 *		CMyConv().Convert(vmapi, vmaps);	///CStr→intのコンバータで変換可能
 *		\endcode
 *	
 *	@note SRC,DST に、 コレクション型 や ペア型 を指定できません。
 *
 *	@param DST 変換後の型
 *	@param SRC 変換前の型
 *
 *	@par必要ファイル
 *			TnbConverter.h
 *
 *	@date 06/09/12 新規作成。
 *	@date 16/06/16 バグ修正。
 */
template<typename DST, typename SRC>
class CConverterT
{
protected:

	/**
	 * [変換] 変換器
	 *	@note ここにSRC型からDST型を作成するメソッドを実装する
	 *	@param[out] _dst 変換後
	 *	@param[in] src 変換元
	 *	@throw CTnbException 変換が不可能の場合、スローすることが出来ます。
	 */
	virtual void Converter(DST& _dst, const SRC& src) = 0;

public:

	/**
	 * [変換] 単要素変換
	 *	@param[out] _dst 変換後
	 *	@param[in] src 変換元
	 *	@throw CTnbException 変換が不可能を示すために、スローされることがあります。
	 */
	void Convert(DST& _dst, const SRC& src)
	{
		Converter(_dst, src);
	}

	/**
	 * [変換] コレクション変換
	 *	@param[out] _dst 変換後
	 *	@param[in] src 変換元
	 *	@throw CEmptyException コレクションに追加不可能であることを示すために、スローされることがあります。
	 *	@throw CTnbException 変換が不可能を示すために、スローされることがあります。
	 */
	template<typename D, typename S>
	void Convert(ICollectionT<D>& _dst, const IConstCollectionT<S>& src)
	{
		EXCLUSIVE2(&_dst, &src);
		size_t l = src.GetSize();
		if ( l == 0 )
		{
			_dst.RemoveAll();
			return;
		}
		bool canRefMode = true;
		try
		{
			if ( _dst.SetSize(l) )
			{
				_dst.Ref(0);
			}
			else
			{
				canRefMode = false;
			}
		}
		catch(CNotSupportException& e)
		{
			e.OnCatch();
			canRefMode = false;
		}
		//
		if ( canRefMode )
		{
			loop ( i, l )
			{
				Convert(_dst.Ref(i), src.At(i)); 
			}
		}
		else
		{
			_dst.RemoveAll();
			D d;
			loop ( i, l )
			{
				Convert(d, src.At(i));
				if ( _dst.Add(d) == INVALID_INDEX )
				{
					throw CEmptyException();
				}
			}
		}
	}

	/**
	 * [変換] ペア型変換.
	 *	@note 値の型のみ変換します。キーは同じ内容になります。
	 *	@param[out] _dst 変換後
	 *	@param[in] src 変換元
	 *	@throw CTnbException 変換が不可能を示すために、スローされることがあります。
	 */
	template<typename T, typename D, typename S>
	void Convert(CPairT<T, D>& _dst, const CPairT<T, S>& src)
	{
		_dst.first = src.first;
		Convert(_dst.second, src.second);
	}
};



}; // TNB
