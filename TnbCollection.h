#pragma once
/**
 *@file
 * 情報群管理関係のヘッダ
 *
 *		情報群管理するためのテンプレートクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbComparator.h"
#include "TnbStr.h"
#include "TnbSync.h"
#include "TnbException.h"
#include "TnbSerializer.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

// Input Iterator ベーステンプレート
template<typename COL, typename TYP> 
class CConstIteratorBaseT
{
	const COL *	m_V;
	mutable INDEX	m_ind;
public:
	CConstIteratorBaseT(const COL* V = NULL, INDEX i = 0): m_V(V), m_ind(i) {}
	INDEX GetIndex(void) const { return m_ind; }
	TYP operator*(void) const { return m_V->Get(m_ind); }
	const TYP* operator->(void) const { return &(m_V->At(m_ind)); }
	bool operator==(const CConstIteratorBaseT& I) const
	{
		return m_V == I.m_V && m_ind == I.m_ind;
	}
	bool operator!=(const CConstIteratorBaseT& I) const
	{
		return m_V != I.m_V || m_ind != I.m_ind;
	}
	CConstIteratorBaseT& operator++(void) const
	{
		m_ind++;
		return *this;
	}
	CConstIteratorBaseT operator++(int) const
	{
		CConstIteratorBaseT I = *this;
		m_ind++;
		return I;
	}
	const TYP& operator[](INDEX i) const{ return m_V->At(m_ind+i);	}
	CConstIteratorBaseT& operator+=(INT_PTR i) {m_ind+=i; return *this;}
	CConstIteratorBaseT& operator-=(INT_PTR i) {m_ind-=i; return *this;}
};

// Randam Access Iterator ベースマクロ
#define _ITERATORCORE(NAME, COLL)										\
		COLL*	m_V;													\
		mutable INDEX m_ind;											\
	public:																\
		NAME(COLL* V = NULL, INDEX i = 0) : m_V(V), m_ind(i) {}			\
		INDEX GetIndex(void) const { return m_ind; }					\
		bool operator==(const NAME& I) const {return m_ind==I.m_ind;}	\
		bool operator!=(const NAME& I) const {return m_ind!=I.m_ind;}	\
		bool operator<=(const NAME& I) const {return m_ind<=I.m_ind;}	\
		bool operator<(const NAME& I) const	{return m_ind<I.m_ind;}		\
		bool operator>=(const NAME& I) const {return m_ind>=I.m_ind;}	\
		bool operator>(const NAME& I) const	{return m_ind>I.m_ind;}		\
		NAME& operator+=(INT_PTR i)		{m_ind+=i; return *this;}		\
		NAME& operator-=(INT_PTR i)		{m_ind-=i; return *this;}		\
		NAME operator+(INT_PTR i) const {NAME I=*this; return I+=i;}	\
		NAME operator-(INT_PTR i) const {NAME I=*this; return I-=i;}	\
		INT_PTR operator-(const NAME& I) const {return m_ind-I.m_ind; }	\
		NAME& operator++(void)		{m_ind++; return *this;}			\
		NAME& operator--(void)		{m_ind--; return *this;}			\
		NAME operator++(int)		{NAME I=*this; m_ind++;	return I;}	\
		NAME operator--(int)		{NAME I=*this; m_ind--;	return I;}


// Randam Access Iterator ベーステンプレート
template<typename COL, typename TYP> 
class CIteratorBaseT
{
	_ITERATORCORE(CIteratorBaseT, COL)
public:
	const TYP& operator*(void) const 	{ return m_V->At(m_ind);	}
	const TYP* operator->(void) const 	{ return &(m_V->At(m_ind));	}
	const TYP& operator[](INDEX i) const{ return m_V->At(m_ind+i);	}
	TYP& operator*(void)				{ return m_V->Ref(m_ind);	}
	TYP* operator->(void)				{ return &(m_V->Ref(m_ind));}
	TYP& operator[](INDEX i)			{ return m_V->Ref(m_ind+i);	}
};

#endif //_TnbDOXYGEN



/**@ingroup COLLECT ITERATOR
 * 情報群管理インターフェーステンプレート
 *
 *		読み込み専用の複数の要素(TYP)を管理するインターフェースです。
 *
 *		このインターフェースを持つクラスは以下の特徴を持ちます。
 *
 *		 　<BR>
 *		 ● Lock() / Unlock() の排他メソッド、及び、スレッドセーブ<BR>
 *			@par
 *					Lock() / Unlock() は情報の排他のために使用します。
 *					Get() , At() , STL互換メソッド(小文字で実装)以外のメソッドは
 *					Lock() メソッドを使わなくても、スレッドセーブになっていますが、
 *					メソッドとメソッドの間でオブジェクトの状態が変わるのを避けるために、
 *					一連の処理を行なう場合、 Lock() すべきです。\n
 *					複数のスレッドからアクセスされる可能性がない場合は、不要です。
 *
 *		 ● Get() / GetEx() による要素の値を取得<BR>
 *		 ● At() による要素の参照取得<BR>
 *			@par
 *					指定の型の要素の取得が行えます。
 *					インデックス（並び番号）でアクセスします。
 *					<BR>※ At() はサブクラスによっては複製された要素の参照を返すものもあります。
 *
 *		 ● Find() / FindMax() / FindMin() による指定要素、最大、最小要素の検索<BR>
 *			@par
 *					以下の方法の内、いずれかによって実現できます。
 *					<ul type=circle>
 *					<li> IComparatorT を実装したコンパレーターを作成し、引数にて委譲する。
 *					<li> IChecker を実装したチェッカーを作成し、引数にて委譲する。
 *					<li> 両方省略し、汎用処理を使用する（ IComparableT 参照）。
 *					</ul>
 *			@par
 *			\code
 *			CVectorT<CFoo> vDat;
 *			     ;         //データ代入
 *			struct IMyComp : IComparatorT<CFoo>
 *			{
 *			  INT_PTR CompareTo( const CFoo &T1, const CFoo &T2 )
 *			  {
 *			    return T1.Comp(T2);
 *			  }
 *			} comp;
 *			CFoo TT;
 *			INDEX index = vDat.FindMax(&comp, TT);  //一番でかい要素のインデックスが返る。			
 *			\endcode
 *
 *		 ● Compare() による全要素の比較<BR>
 *			@par
 *					各要素を IComparatorT::GetDefault() で得られるコンパレータを使用して
 *					比較します。
 *
 *		 ● ISerializer 実装クラスによる、シリアライズ<BR>
 *			@par
 *					ISerializer を使い、情報をシリアライズすることが可能です。
 *					シリアライズしたデータは、同じ要素型の他のコレクションで
 *					デシリアライズすることが可能です。
 *					<BR>※要素には、シリアライズ可能な型を適用してください。
 *
 *		 ● Standard Template Libraryとの親和性<BR>
 *		 ● begin() / end() による{@link ITERATOR 反復子}<BR>
 *			@par
 *					メソッド名が小文字で始まるものは、すべて STL との親和性のために用意されています。
 *					反復子はインスタンスの参照を持っているだけなので、反復子が有効の間、
 *					インスタンスは消さないようにしてください。
 *					また、これらのメソッド、反復子は、スレッドセーブになりませんので、
 *					必要に応じて Lock() すべきです。
 *
 *	@note 本インターフェースを実装する場合、最低限以下のメソッドを用意する必要が有ります。<BR>
 *	@par
 *			<ol>
 *			<li> At()
 *			<li> GetSize()
 *			</ol>
 *
 *	@see	ISynchronized を実装しています。
 *	@see	IComparableT を実装しています。
 *	@see	ISerializable を実装しています。ただし デシリアライズはサポートしてません。
 *	
 *	@throw CTnbException 読み込み処理時にエラーがあった場合、 CTnbException のサブクラスをスローする可能性が有ります。
 *
 *	@param TYP 要素の型。型はコピー機能が必須です。
 *
 *	@par必要ファイル
 *			TnbCollection.h
 *
 *	@date 06/05/15 ICollectionTをさらに抽象化して作成。
 *	@date 06/05/17 Find関係メソッド、追加。
 *	@date 06/05/20 コンパレーター整理。
 *	@date 06/05/23 Offsetアダプタ新規に伴いFind関係メソッドの引数整理。チェッカーIF追加。
 *	@date 06/06/29 自動コンパレータ改善。
 *	@date 06/07/04 コンパレータIFを分離。
 *	@date 06/08/01 GetConstPointerに依存しないつくりに変更。
 *	@date 06/08/11 Element() を At() に改名。
 *	@date 06/08/13 イテレータを STL にあわせて、改修。
 *	@date 06/08/30 シリアライズ機能、追加。
 *	@date 06/09/13 IsEqual() 追加。
 *	@date 06/10/26 GetEx() を追加。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 09/09/10 operator size_t 追加。
 *	@date 11/06/01 コピーオペレータを実装
 *	@date 11/07/11 配列検索の Find() を追加。
 *	@date 11/08/24 GetElements() に第三引数、追加。
 *	@date 19/07/04 FindMin() FindMax() を改変。
 */
template<typename TYP>
struct IConstCollectionT : ISynchronized, ISerializable, IComparableT<TNB::IConstCollectionT<TYP> >
{
	/**@ingroup COLLECT
	 * 情報群管理インターフェースのチェッカーインターフェース.
	 *		有効条件を IsValid() に実装します。 
	 */
	struct IChecker
	{
		virtual ~IChecker(void) {}	///< デストラクタ
		/**
		 * [確認] チェック
		 *	@param T チェック対象
		 *	@retval true T は有効
		 *	@retval false T は無効
		 */
		virtual bool IsValid(const TYP& T) const = 0;
	};


	//---------------------------------


	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// const_iterator型
	typedef CConstIteratorBaseT<IConstCollectionT<TYP>, TYP> const_iterator;
	// value_type宣言
	typedef TYP value_type;
	#endif //_TnbDOXYEM

	/**
	 * [反復] 先頭const_iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return const_iterator(this, 0); }

	/**
	 * [反復] 最後const_iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return const_iterator(this, GetSize()); }


	//---------------------------------


	/// コンストラクタ
	IConstCollectionT(void) 
	{
	}

	/**
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	operator size_t(void) const 
	{
		return GetSize();
	}

	/**
	 * [取得] 要素数取得.
	 *	@note Pure関数です。
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const = 0;

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const = 0;

	/**
	 * [取得] 要素の取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 GetEx() を使用してください。
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP Get(INDEX index) const
	{
		return At(index);
	}

	/**
	 * [取得] 要素の取得.
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP GetEx(INDEX index) const
	{
		EXCLUSIVE(this);
		return At(index);
	}

	/**
	 * [取得] 複数要素取り出し.
	 *	@param[in] size 取り出すデータの数。
	 *	@param[out] _P コピーするポインタ。size 分の領域がある必要があります。
	 *	@param[in] offset 取り出し開始のインデックス。省略すると０。
	 *	@return コピーした数
	 */
	virtual size_t GetElements(size_t size, TYP* _P, INDEX offset = 0) const
	{
		EXCLUSIVE(this);
		if ( offset >= GetSize() )
		{
			return 0;
		}
		if ( size + offset > GetSize() )
		{
			size = GetSize() - offset;
		}
		for ( INDEX i = 0; i < size; i++ )
		{
			*_P++ = At(i + offset);
		}
		return size;
	}

	/**
	 * [確認] 要素の有無確認.
	 *	@note GetSize()の戻り値が０か否かでチェックしています。
	 *	@retval true	要素なし
	 *	@retval false	要素有り
	 */
	bool IsEmpty(void) const
	{
		return GetSize() == 0;
	}

	/**
	 * [確認] INDEXの有効確認.
	 *	@note GetSize()の戻り値を使用してチェックしています。
	 *	@param index インデックス
	 *	@retval true	指定Indexは有効。
	 *	@retval false	指定Indexは無効(範囲外)。
	 */
	bool IsInRange(INDEX index) const
	{
		return TNB::IsInRange(index, GetSize());
	}

	/**
	 * [処理] シリアライズ
	 *	@param[out] _sr シリアライザー。 
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		EXCLUSIVE(this);
		size_t l = GetSize();
		_sr << l << l;
		loop ( i, l )
		{
			_sr << At(i);
		}
	}

	/**
	 * [処理] デシリアライズ
	 *	@param ds デシリアライザー。
	 *	@throw CNotSupportException サポートしていないことを示すためにスローされます。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		throw CNotSupportException();
	}

	/**
	 * [検索] 条件一致要素の検索.
	 *		チェッカー( IChecker のサブクラス)を使用して、検索します。
	 *	@note 範囲を限定したい場合、 CConstOffsetAdapterT を使用します。
	 *	@param	checker チェッカー
	 *	@param	startIndex 検索開始インデックス。
	 *	@param	boIsReverse	省略すると条件に一致を探します。
	 *						trueを指定すると条件不一致を探します。
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		発見したインデックス
	 */
	INDEX Find(const IChecker& checker, INDEX startIndex = 0, bool boIsReverse = false) const
	{
		EXCLUSIVE(this);
		if ( IsInRange(startIndex) )
		{
			size_t size = GetSize();
			for ( size_t i = startIndex; i < size; i++ )
			{
				if ( checker.IsValid(At(i)) ^ boIsReverse )
				{
					return i;
				}
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [検索] 指定配列の検索.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、検索します。
	 *	@note 範囲を限定したい場合、 CConstOffsetAdapterT を使用します。
	 *	@param	t 配列
	 *	@param	comparator	コンパレーター
	 *	@param	startIndex 検索開始インデックス。
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		発見したインデックス
	 */
	INDEX Find(const IConstCollectionT<TYP>& t, const IComparatorT<TYP>& comparator, INDEX startIndex = 0) const
	{
		EXCLUSIVE(this);
		if ( IsInRange(startIndex) )
		{
			size_t size = GetSize();
			for ( size_t i = startIndex; i < size; i++ )
			{
				size_t tsz = t.GetSize();
				if ( i + tsz > size || tsz == 0 )
				{
					break;
				}
				bool isFind = true;
				loop ( j, tsz )
				{
					if ( ! comparator.IsEqualTo(At(i + j), t.At(j)) )
					{
						isFind = false;
						break;
					}
				}
				if ( isFind )
				{
					return i;
				}
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [検索] 指定配列の検索.
	 *		IComparatorT::GetDefault() で得られるコンパレータを使用して検索します。
	 *	@note 範囲を限定したい場合、 CConstOffsetAdapterT を使用します。
	 *	@param	t 配列
	 *	@param	startIndex 検索開始インデックス。
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		発見したインデックス
	 */
	INDEX Find(const IConstCollectionT<TYP>& t, INDEX startIndex = 0) const
	{
		return Find(t, IComparatorT<TYP>::GetDefault(), startIndex);
	}

	/**
	 * [検索] 指定要素の検索.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、検索します。
	 *	@note 範囲を限定したい場合、 CConstOffsetAdapterT を使用します。
	 *	@param	t 要素
	 *	@param	comparator	コンパレーター
	 *	@param	startIndex 検索開始インデックス。
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		発見したインデックス
	 */
	INDEX Find(const TYP& t, const IComparatorT<TYP>& comparator, INDEX startIndex = 0) const
	{
		EXCLUSIVE(this);
		if ( IsInRange(startIndex) )
		{
			size_t size = GetSize();
			for ( size_t i = startIndex; i < size; i++ )
			{
				if ( comparator.IsEqualTo(At(i), t) )
				{
					return i;
				}
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [検索] 指定要素の検索.
	 *		IComparatorT::GetDefault() で得られるコンパレータを使用して検索します。
	 *	@note 範囲を限定したい場合、 CConstOffsetAdapterT を使用します。
	 *	@param	t 要素
	 *	@param	startIndex 検索開始インデックス。
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		発見したインデックス
	 */
	INDEX Find(const TYP& t, INDEX startIndex = 0) const
	{
		return Find(t, IComparatorT<TYP>::GetDefault(), startIndex);
	}

	/**
	 * [検索] 要素の最大検索.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、比較検索します。
	 *	@param comparator	コンパレーター
	 *	@param def 最小値(初期値) 
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		最大を持つインデックス
	 */
	INDEX FindMax(const IComparatorT<TYP>& comparator, const TYP& def) const
	{
		EXCLUSIVE(this);
		TYP t = def;
		INDEX r = INVALID_INDEX;
		loop_dn ( i, GetSize() )
		{
			if ( comparator.CompareTo(At(i), t) >= 0 )
			{
				t = Get(i);
				r = i;
			}
		}
		return r;
	}

	/**
	 * [検索] 要素の最大検索.
	 *		IComparatorT::GetDefault() で得られるコンパレータを使用して比較検索します。
	 *	@note 範囲を限定したい場合、 CConstOffsetAdapterT を使用します。
	 *	@param def 最小値(初期値) 
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		最大を持つインデックス
	 */
	INDEX FindMax(const TYP& def) const
	{
		return FindMax(IComparatorT<TYP>::GetDefault(), def);
	}

	/**
	 * [検索] 要素の最小検索.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、比較検索します。
	 *	@param	comparator	コンパレーター
	 *	@param def 最大値(初期値) 
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		最小を持つインデックス
	 */
	INDEX FindMin(const IComparatorT<TYP>& comparator, const TYP& def) const
	{
		EXCLUSIVE(this);
		TYP t = def;
		INDEX r = INVALID_INDEX;
		loop_dn ( i, GetSize() )
		{
			if ( comparator.CompareTo(At(i), t) <= 0 )
			{
				t = Get(i);
				r = i;
			}
		}
		return r;
	}

	/**
	 * [検索] 要素の最小検索.
	 *		IComparatorT::GetDefault() で得られるコンパレータを使用して比較検索します。
	 *	@note 範囲を限定したい場合、 CConstOffsetAdapterT を使用します。
	 *	@param def 最大値(初期値) 
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		最小を持つインデックス
	 */
	INDEX FindMin(const TYP& def) const
	{
		return FindMin(IComparatorT<TYP>::GetDefault(), def);
	}

	/**
	 * [確認] 比較.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、全要素を比較します。
	 *	@param c 比較対象
	 *	@retval ０		自分は c と同じ
	 *	@retval １以上	自分は c より大きい
	 *	@retval ０未満	自分は c より小さい
	 */
	virtual INT_PTR Compare(const IConstCollectionT<TYP>& c) const
	{
		if ( this == &c )
		{
			return 0; 
		}
		EXCLUSIVE2(&c, this);
		const IComparatorT<TYP>& comparator = IComparatorT<TYP>::GetDefault();
		size_t mySize = GetSize();
		size_t youSize = c.GetSize();
		size_t size = (mySize < youSize) ? mySize : youSize;
		loop ( i, size )
		{
			INT_PTR r = comparator.CompareTo(At(i), c.At(i));
			if ( r != 0 )
			{
				return r;
			}
		}
		return mySize - youSize;
	}

	/**
	 * [確認] 比較.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、全要素を比較します。
	 *	@param c 比較対象
	 *	@retval true	自分は c と同じ
	 *	@retval false	自分は c と異なる
	 */
	virtual bool IsEqual(const IConstCollectionT<TYP>& c) const
	{
		ASSERT( IComparatorT<TYP>::GetDefault().IsEqualTo(TYP(), TYP()) );
		if ( this == &c )
		{ 
			return true; 
		}
		EXCLUSIVE2(&c, this);
		size_t size = GetSize();
		if ( size != c.GetSize() )
		{
			return false; 
		}
		const IComparatorT<TYP>& comparator = IComparatorT<TYP>::GetDefault();
		loop ( i, size )
		{
			if ( ! comparator.IsEqualTo(At(i), c.At(i)) )
			{
				return false;
			}
		}
		return true;
	}

private:
	/// コピーオペレータ (使用禁止)
	void operator=(const IConstCollectionT& other);
};



/**@ingroup COLLECT ITERATOR
 * 情報群管理操作インターフェーステンプレート
 *
 *		複数の要素型(TYP)を管理、操作するインターフェースです。
 *
 *		このインターフェースを持つクラスは以下の特徴を持ちます。
 *
 *		 　<BR>
 *		 ● Lock() / Unlock() の排他メソッド、及び、スレッドセーブ<BR>
 *			@par
 *					Lock() / Unlock() は情報の排他のために使用します。
 *					Get(), At(), Set(), Add(), Remove(), STL互換メソッド(小文字で実装)以外のメソッドは
 *					Lock() メソッドを使わなくても、スレッドセーブになっていますが、
 *					メソッドとメソッドの間でオブジェクトの状態が変わるのを避けるために、
 *					一連の処理を行なう場合、 Lock() すべきです。\n
 *					複数のスレッドからアクセスされる可能性がない場合は、不要です。
 *			@par
 *			\code
 *		
 *			CVectorT<long> vlDat;
 *			     ;
 *			vlDat.Lock();	//Lock～Unlock間、他のスレッドからvlDatの操作が出来なくなる。		
 *			CVectorT<long>::iterator it = vlDat.begin();		
 *			*it = 1234;
 *			     ;
 *			vlDat.Unlock();	
 *			\endcode
 *
 *		 ● Get() / GetEx() による要素の値を取得<BR>
 *		 ● At() / Ref() による要素の参照取得<BR>
 *		 ● Add() / AddEx() による要素の追加<BR>
 *		 ● Insert() / InsertEx() による要素の挿入<BR>
 *		 ● Remove() / RemoveEx() による要素の削除<BR>
 *		 ● Set() / SetEx() による要素の値を設定<BR>
 *		 ● Swap() による要素の値の入れ替え<BR>
 *		 ● RemoveAll() による要素の全削除<BR>
 *			@par
 *					指定の型の要素を追加、削除、設定、取得が行えます。
 *					基本的に、インデックス（並び番号）でアクセスします。
 *					<BR>※ At() はサブクラスによっては複製された要素の参照を返すものもあります。
 *			@par
 *			\code
 *		
 *			CVectorT<long> vlDat;
 *			vlDat.Add( 12 );            // 要素追加
 *			vlDat.Add( 34 );            // 要素追加
 *			//この時点で vlDatは{ 12,34 }という情報を持つ
 *			long l1 = vlDat.Get(0);     // インデックス０（最初）の要素取得
 *			ASSERT( l1==12 );
 *			vlDat.Set(1,56);            // インデックス１の要素の値を設定
 *			vlDat.Set(0,78);            // インデックス０の要素の値を設定
 *			//この時点で vlDatは{ 78,56 }という情報を持つ
 *			vlDat.Remove(0);            // インデックス０の要素削除
 *			//この時点で vlDatは{ 56 }という情報を持つ
 *			vlDat.RemoveAll();          // 要素全削除
 *			\endcode
 *
 *		 ● AddElements() による要素の追加<BR>
 *		 ● SetElements() による要素のコピー<BR>
 *			@par
 *					指定情報を、まとめて追加 / コピーします。
 *					必要なものだけを追加/コピーしたい場合、 CConstAdapterT を CIndexAdapterT を使用します。
 * 
 *		 ● Append() による要素の追加<BR>
 *		 ● Copy() による要素のコピー<BR>
 *			@par
 *					クラスが異なっていても、同じ IConstCollectionT の TYP が同じであれば、
 *					追加/コピーすることが出来ます。
 *					必要なものだけを追加/コピーしたい場合、 CIndexAdapterT を使用します。
 *
 *		 ● Sort() による要素のソート<BR>
 *			@par
 *					以下の方法の内、いずれかによって実現できます。
 *					<ul type=circle>
 *					<li> IComparatorT を実装したコンパレーターを作成し、引数にて委譲する。
 *					<li> 両方省略し、汎用処理を使用する（ IComparableT 参照）。
 *					</ul>
 *			@par
 *			\code
 *			CVectorT<CFoo> vDat;
 *			     ;         //データ代入
 *			struct IMyComp : IComparatorT<CFoo>
 *			{
 *			  INT_PTR CompareTo(const CFoo &T1, const CFoo &T2)
 *			  {
 *			    return T1.Comp(T2);
 *			  }
 *			} comp;
 *			vDat.Sort( &comp );  //小さい順にソートされる			
 *			\endcode
 *
 *			@par
 *					CIndexAdapterT を使うと、Indexテーブルによるソートが可能です
 *					（対象のコレクションの変化がないため、高速でもある）。
 *
 *		 ● ISerializer 実装クラスによる、シリアライズ<BR>
 *		 ● IDeserializer 実装クラスによる、デシリアライズ<BR>
 *			@par
 *					ISerializer を使い、情報をシリアライズすることが可能です。<BR>
 *					IDeserializer を使い、シリアライズされた情報をデシリアライズすることが可能です。
 *					この時、同じ要素型であれば異なるコレクションでもデシリアライズ可能です(一部のコレクションを除く)。
 *					<BR>※要素には、シリアライズ可能な型を適用してください。
 *
 *		 ●代入コンストラクタ、代入オペレータの実装(一部を除く)<BR>
 *			@par
 *					同じ型のインスタンス同士は代入が可能です。
 *					この時、左辺のインスタンスは、情報を破棄し、
 *					右辺のインスタンスの情報を共有するようになります
 *					（そのため代入作業は<b>高速</b>です）。
 *					その後、いずれかのインスタンスを操作すると、
 *					情報を複製するので、他のインスタンスに影響が出ません
 *					（複製を作成する時、時間がかかる可能性が有ります）。
 *
 *		 ● 他、 IConstCollectionT の特徴を引き継いでいます。<BR>
 *
 *	@note 本インターフェースを実装する場合、最低限以下のメソッドを用意する必要が有ります。<BR>
 *	@par
 *			<ol>
 *			<li> GetSize()
 *			<li> At()
 *			<li> Ref()
 *			<li> Set()
 *			<li> Add()
 *			<li> Remove()
 *			</ol>
 *			      ※ ICollectionMidT を使うと、 At() , Ref() と Set() は自動で実装されます。
 *
 *	@see	IConstCollectionT を実装しています。
 *
 *	@throw CTnbException 読み込み処理時にエラーがあった場合、 CTnbException のサブクラスをスローする可能性が有ります。
 *
 *	@param TYP 要素の型。型はコピー機能が必須です。
 *
 *	@par必要ファイル
 *			TnbCollection.h
 *
 *	@date 06/04/19 抽象化
 *	@date 06/04/28 Clone処理を抽象化してこのIFに実装。
 *	@date 06/05/22 Swapメソッド追加。
 *	@date 06/05/23 AddElements、Copy系メソッド追加。
 *	@date 06/06/01 ローテート、シャッフルメソッド追加。
 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
 *	@date 06/08/11 Element() を At() に改名。ReferElementPtr() を廃止。
 *	@date 06/08/13 イテレータを STL にあわせて、改修。
 *	@date 06/08/24 条件付削除 RemoveOnCheck() を追加。
 *	@date 06/08/30 シリアライズ機能、追加。
 *	@date 06/09/13 Copy系メソッドを改良。
 *	@date 06/10/26 At() を廃止。SetEx(), AddtEx(), RemoveEx() を追加。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 07/06/19 CIndexAdapterT::Cull() を作成したため、 本インターフェースの ～OnCheck() メソッドを廃止。 
 *					RemoveOnCheck() を Cull() に改名。
 *	@date 07/09/18 TrimBottom() を追加。
 *	@date 09/09/07 CopyElements() -> SetElements() に改名。
 *	@date 11/03/08 Insert() を追加。
 *	@date 11/06/01 コピーオペレータを実装
 */
template<typename TYP>
struct ICollectionT : IConstCollectionT<TYP>
{
	/**@ingroup COLLECT
	 * 情報群管理操作インターフェースのソート処理クラス
	 *
	 *		ICollectionT を持つクラスの要素をソートすることができます。
	 *		ソート条件は IComparator インターフェースを実装して関数に渡すことで実現します。
	 *
	 *	@note 通常、このクラスを使わず、ICollectionT のソートメソッドを使用します。
	 *
	 *	@param TYP コピー処理が可能な型
	 *
	 *	@date 06/04/20 新規
	 */
	class CSort : CCopyImpossible
	{
		ICollectionT<TYP>*	m_piCollect;		///< コレクションの参照(qsortで使用)
		const IComparatorT<TYP>*	m_piComparator;		///< コンパレーターの参照(qsortで使用)
		bool				m_boIsReverse;		///< リバースフラグ(qsortで使用)
		/**
		 * QSORTのサブルーチン.
		 *	@note 再起呼び出しがあります。
		 *	@param left 開始インデックス
		 *	@param right 終了インデックス
		 */
		void m_qsort(size_t left, size_t right) 
		{
			if ( left >= right ){ return; }
			size_t j = left;
			for ( size_t i = left + 1; i <= right; i++ ) 
			{
				const TYP& c1 = m_piCollect->At(i);
				const TYP& c2 = m_piCollect->At(left);
				if ( (m_piComparator->CompareTo(c1, c2) < 0) ^ m_boIsReverse )
				{
					m_piCollect->Swap(i, ++j);
				}
			}
			m_piCollect->Swap(left, j);
			//
			if ( j > 0 )
			{
				m_qsort(left, j - 1);
			}
			m_qsort(j + 1, right);
		}

	public:
	
		/// コンストラクタ
		CSort(void) {}

		/**
		 * [操作] クイックソート.
		 *		コンパレーターが小さいと判断した順にソートします。
		 *	@param[in,out]	_collect	ソート対象
		 *	@param[in]		comp		コンパレーター
		 *	@param[in]		boIsReverse	trueを指定すると並びを逆にします
		 */
		void QuickSort(ICollectionT<TYP>& _collect, const IComparatorT<TYP>& comp, bool boIsReverse = false)
		{
			m_piCollect		= &_collect;
			m_piComparator	= &comp;
			m_boIsReverse	= boIsReverse;
			size_t l = _collect.GetSize();
			if ( l > 0 )
			{
				m_qsort(0, l - 1);
			}
		}

		/**
		 * [操作] バブルソート.
		 *		コンパレーターが小さいと判断した順にソートします。
		 *	@param[in,out]	_collect	ソート対象
		 *	@param[in]		comp	コンパレーター
		 *	@param[in]		boIsReverse	trueを指定すると並びを逆にします
		 */
		void BubbleSort(ICollectionT<TYP>& _collect, const IComparatorT<TYP>& comp, bool boIsReverse = false)
		{
			size_t size = _collect.GetSize();
			loop ( i, size )
			{
				for ( size_t j = size - 1; j > i; j-- )
				{
					const TYP& c1 = _collect.At(j - 1);
					const TYP& c2 = _collect.At(j);
					if ( (comp.CompareTo(c1, c2) > 0) ^ boIsReverse )
					{
						_collect.Swap(j - 1, j);
					}
				}
			}
		}
	};


	//------------------------------------


	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// const_iterator型宣言
	typedef CConstIteratorBaseT<ICollectionT<TYP>, TYP> const_iterator;
	// iterator型宣言
	typedef CIteratorBaseT<ICollectionT<TYP>, TYP> iterator;
	#endif //_TnbDOXYEM

	/**
	 * [反復] 先頭const_iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return const_iterator(this, 0); }

	/**
	 * [反復] 最後const_iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return const_iterator(this, GetSize()); }

	/**
	 * [反復] 先頭iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator begin(void) { return iterator(this, 0); }

	/**
	 * [反復] 最後iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator end(void) { return iterator(this, GetSize()); }

	/**
	 * [反復] 最後に追加
	 *	@param t 要素
	 */
	void push_back(const TYP& t) { Add(t); }


	//------------------------------------

	
	/** 
	 * コンストラクタ
	 */
	ICollectionT(void) 
	{
	}

	/** 
	 * デストラクタ
	 */
	virtual ~ICollectionT(void) 
	{
	}

	/**
	 * [代入] コピーオペレータ.
	 *	@param other コピー元.
	 *	@return 自分の参照
	 */
	ICollectionT& operator=(const ICollectionT& other)
	{
		Copy(other);
		return *this;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の参照
	 *	@throw CNotSupportException サポートしていないことを示すためにスローされます。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP& Ref(INDEX index) = 0;

	/**
	 * [設定] 要素の設定.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 SetEx() を使用してください。
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	virtual bool Set(INDEX index, const TYP& t) = 0;

	/**
	 * [設定] 要素の設定.
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	bool SetEx(INDEX index, const TYP& t)
	{
		EXCLUSIVE(this);
		return Set(index, t);
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t) = 0;

	/**
	 * [追加] 要素一つ追加.
	 *	@param t 要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX AddEx(const TYP& t)
	{
		EXCLUSIVE(this);
		return Add(t);
	}

	/**
	 * [追加] 要素一つ挿入.
	 *		指定のインデックスに要素を追加します。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 InsertEx() を使用してください。
	 *	@param index インデックス
	 *	@param t 要素
	 *	@retval true	挿入成功
	 *	@retval false	失敗（インデックス無効）
	 */
	virtual bool Insert(INDEX index, const TYP& t)
	{
		size_t nowSize = GetSize();
		if ( index >= nowSize + 1 )
		{
			return false;
		}
		INDEX r = Add(TYP());
		if ( r == INVALID_INDEX )
		{
			return false;
		}
		loop_dn( i, nowSize - index )
		{
			Set(index + i + 1, At(index + i));
		}
		Set(index, t);
		return true;
	}

	/**
	 * [追加] 要素一つ挿入.
	 *		指定のインデックスに要素を追加します。
	 *	@param index インデックス
	 *	@param t 要素
	 *	@retval true	挿入成功
	 *	@retval false	失敗（インデックス無効）
	 */
	virtual bool InsertEx(INDEX index, const TYP& t)
	{
		EXCLUSIVE(this);
		return Insert(index, t);
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 RemoveEx() を使用してください。
	 *	@param index インデックス
	 *	@retval false	エラー。
	 *	@retval true	成功。
	 */
	virtual bool Remove(INDEX index) = 0;

	/**
	 * [削除] 要素一つ削除.
	 *	@param index インデックス
	 *	@retval false	エラー。
	 *	@retval true	成功。
	 */
	bool RemoveEx(INDEX index)
	{
		EXCLUSIVE(this);
		return Remove(index);
	}

	/**
	 * [削除] 要素削除.
	 *	@param index インデックス
	 *	@param size 削除数。省略すると０。０を指定するとそれ以降全部削除します。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		削除した数。
	 */
	virtual size_t RemoveElements(INDEX index, size_t size = 0)
	{
		size_t r = 0;
		if ( size == 0 )
		{
			size = INVALID_SIZE;
		}
		loop ( i, size )
		{
			if ( ! Remove(index) )
			{
				break;
			}
			r++;
		}
		return r;
	}

	/**
	 * [削除] 全要素削除 .
	 *	@note Remove(0)を無くなるまで呼び出します。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool RemoveAll(void)
	{
		EXCLUSIVE(this);
		while ( ! IsEmpty() )
		{
			if ( ! Remove(0) )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [設定] 要素の入れ替え.
	 *	@param index1 インデックス１
	 *	@param index2 インデックス２
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual void Swap(INDEX index1, INDEX index2)
	{
		if ( index1 != index2 )
		{
			EXCLUSIVE(this);
			TYP t = Get(index1);
			Set(index1, At(index2));
			Set(index2, t);
		}
	}

	/**
	 * [追加] 複数要素追加.
	 *		最後に追加します。
	 *	@param size 追加するデータの数。
	 *	@param P 追加するデータ列のポインタ。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */ 
 	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		EXCLUSIVE(this);
		if ( P != NULL )
		{
			loop ( i, size )
			{
				if ( Add(*P++) == INVALID_INDEX )
				{
					return INVALID_SIZE; 
				}
			}
		}
		else
		{
			loop ( i, size )
			{
				if ( Add(TYP()) == INVALID_INDEX )
				{
					return INVALID_SIZE; 
				}
			}
		}
		return size;
	}

	/**
	 * [追加] 追加.
	 *		指定のデータを追加します。
	 *	@note 追加元データの範囲を限定したい場合、元データに CConstOffsetAdapterT を使用します。
	 *	@param c 追加するデータ
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	virtual size_t Append(const IConstCollectionT<TYP>& c)
	{
		EXCLUSIVE2(&c, this);
		size_t r = 0;
		loop ( i, c.GetSize() )
		{
			if ( Add(c.At(i)) == INVALID_INDEX )
			{
				return INVALID_SIZE;
			}
			r++;
		}
		return r;
	}

	/**
	 * [設定] コピー.
	 *		管理しているバッファを開放し、指定のデータをコピーします。
	 *	@note 廃止予定です。 SetElements() を使用してください。
	 *	@param size データ長。
	 *	@param P コピーする内容を指定。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		複製した数。
	 */
	virtual size_t CopyElements(size_t size, const TYP* P = NULL)
	{
		return SetElements(size, P);
	}

	/**
	 * [設定] 複数要素設定.
	 *		管理しているバッファを開放し、指定のデータをコピーします。
	 *	@param size データ長。
	 *	@param P コピーする内容を指定。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		複製した数。
	 */
	virtual size_t SetElements(size_t size, const TYP* P = NULL)
	{
		if ( ! RemoveAll() )
		{
			return INVALID_SIZE; 
		}
		return AddElements(size, P);
	}

	/**
	 * [設定] コピー.
	 *		管理しているバッファを開放し、指定のデータをコピーします。
	 *	@note コピー元の範囲を限定したい場合、元データに CConstOffsetAdapterT を使用します。
	 *	@note 一度別メモリにコピーしています。そのため、同じオブジェクトでも対応可能です。
	 *	@param c コピーするデータ
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		複製した数。
	 */
	virtual size_t Copy(const IConstCollectionT<TYP>& c)
	{
		if ( this != &c )
		{
			EXCLUSIVE2(&c, this);
			size_t l = c.GetSize();
			if ( SetSize(l) )
			{
				loop ( i, l )
				{
					Set(i, c.At(i));
				}
			}
			return l;
		}
		return c.GetSize();
	}

	/**
	 * [削除] 間引き.
	 *		指定の条件に合わないもの(チェッカーの結果により)を削除します。
	 *	@param checker {@link TNB::IConstCollectionT::IChecker チェッカー}。
	 *	@param	boIsReverse	省略すると条件に一致を残します。
	 *						trueを指定すると条件不一致を残します。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		削除した数。
	 */
	virtual size_t Cull(const IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		if ( ! IsEmpty() )
		{
			loop_dn ( i, GetSize() )
			{
				if ( ! checker.IsValid(At(i)) ^ boIsReverse )
				{
					if ( ! Remove(i) )
					{
						return INVALID_SIZE; 
					}
					r++;
				}
			}
		}
		return r;
	}

	/**
	 * [操作] サイズ指定
	 *	@note 大きくすることも小さくすることも出来ます。
	 *	@note 大きくした時、そのデータはTYP()で埋められます。
	 *	@param size サイズ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool SetSize(size_t size)
	{
		EXCLUSIVE(this);
		if ( size == 0 )
		{
			return RemoveAll();
		}
		else
		{
			size_t nowSize = GetSize();
			if ( nowSize > size )
			{
				//今より小さく
				for ( size_t i = nowSize - 1; i >= size; i-- )
				{
					if ( ! Remove(i) )
					{
						return false; 
					}
				}
			}
			else if ( nowSize < size )
			{
				//今より大きく
				size_t l = (size - nowSize);
				return AddElements(l) == l;
			}
		}
		return true;
	}

	/**
	 * [処理] デシリアライズ
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		EXCLUSIVE(this);
		size_t l1, l2;
		ds >> l1 >> l2;
		if ( l1 != l2 )
		{
			throw CInvalidParamException();
		}
		RemoveAll();
		TYP t;
		loop ( i, l1 )
		{
			ds >> t;
			Add(t);
		}
	}

	/**
	 * [操作] ソート.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、比較します。
	 *	@note 範囲を限定したい場合、 COffsetAdapterT を使用します。
	 *	@param	comparator コンパレーター
	 *	@param	boIsReverse	省略すると小さい順にソートします。
	 *						trueを指定すると大きい順位ソートします。
	 *	@param	boIsBubble	trueをバブルソートアルゴリズムを使います。
	 *						通常はクイックソートアルゴリズムです。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Sort(const IComparatorT<TYP>& comparator, bool boIsReverse = false, bool boIsBubble = false)
	{
		EXCLUSIVE(this);
		CSort sort;
		bool boRc = true;
		try
		{
			if ( boIsBubble )
			{
				sort.BubbleSort(*this, comparator, boIsReverse);
			}
			else
			{
				sort.QuickSort(*this, comparator, boIsReverse);
			}
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			boRc = false;
		}
		return boRc;
	}

	/**
	 * [操作] ソート.
	 *		IComparatorT::GetDefault() で得られるコンパレータを使用して比較します。
	 *	@note 範囲を限定したい場合、 COffsetAdapterT を使用します。
	 *	@param	boIsReverse	省略すると小さい順にソートします。
	 *						trueを指定すると大きい順位ソートします。
	 *	@param	boIsBubble	trueをバブルソートアルゴリズムを使います。
	 *						通常はクイックソートアルゴリズムです。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Sort(bool boIsReverse = false, bool boIsBubble = false)
	{	
		return Sort(IComparatorT<TYP>::GetDefault(), boIsReverse, boIsBubble);
	}

	/**
	 * [操作] シャッフル.
	 *		要素の並びをシャッフルします。
	 *	@note 範囲を限定したい場合、 COffsetAdapterT を使用します。
	 *	@param iDepth シャッフルを何回行うかを指定できます。数やればバラけるというものも有りません。
	 *				省略すると1回です。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Shuffle(int iDepth = 1)
	{
		EXCLUSIVE(this);
		bool boRc = true;
		try
		{
			size_t size = GetSize();
			if ( size > 1 )
			{
				loop ( j, iDepth )
				{
					loop ( i, size )
					{
						size_t r = rand() % size;
						if ( r == i )
						{
							r = rand() % size; 
						} 
						Swap(i, r);
					}
				}
			}
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			boRc = false;
		}
		return boRc;
	}

	/**
	 * [処理] 末尾トリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param t トリム対象の情報。
	 *	@param comparator コンパレータ。
	 *	@return 残った数。
	 */
	size_t TrimBottom(const TYP& t, const IComparatorT<TYP>& comparator)
	{
		EXCLUSIVE(this);
		size_t len = GetSize();
		if ( len > 0 )
		{
			loop ( i, len ) 
			{
				if ( ! comparator.IsEqualTo(At(len - 1 - i), t) )
				{
					SetSize(len - i);
					return len - i;
				}
			}
			RemoveAll();
			return 0;
		}
		return len;
	}

	/**
	 * [処理] 末尾トリム.
	 *	@note 保持する情報に対し処理します。
	 *	@param t トリム対象の情報。
	 *	@return 残った数。
	 */
	size_t TrimBottom(const TYP& t)
	{
		return TrimBottom(t, IComparatorT<TYP>::GetDefault());
	}
};



/**@ingroup COLLECT ITERATOR
 * 情報群管理操作インターフェース拡張テンプレート
 *
 *		ICollectionT を拡張しています。本インターフェースを代わりに実装し、
 *		m_GetConstPointer() , m_GetPointer() を実装することで、以下のメソッドを
 *		用意してくれます。
 *
 *	@par
 *			1) Get const			<BR>
 *			2) At const				<BR>
 *			3) Ref					<BR>
 *			4) Set					<BR>
 *			5) operator[] const		<BR>
 *			6) operator[]			<BR>
 *
 *	@see	ICollectionT を実装しています。
 *
 *	@throw CTnbException 読み込み処理時にエラーがあった場合、 CTnbException のサブクラスをスローする可能性が有ります。
 *
 *	@param TYP 要素の型。型はコピー機能が必須です。
 *
 *	@par必要ファイル
 *			TnbCollection.h
 *
 *	@date 06/08/11 抽象化して、新規に用意。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP>
class ICollectionMidT : public ICollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
protected:

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const = 0;

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual TYP* m_GetPointer(INDEX index) = 0;

public:

	/// コンストラクタ
	ICollectionMidT(void) : _super()
	{
	}

	/**
	 * [取得] 要素の取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 GetEx() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の複製
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP Get(INDEX index) const
	{
		const TYP* P = m_GetConstPointer(index);
		if ( P != NULL )
		{
			return *P; 
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		const TYP* P = m_GetConstPointer(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の参照
	 *	@throw CNotSupportException サポートしていないことを示すためにスローされます。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP& Ref(INDEX index)
	{
		TYP* P = m_GetPointer(index);
		if ( P == NULL )
		{
			throw CNotSupportException();
		}
		return *P;
	}

	/**
	 * [設定] 要素の設定.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 SetEx() を使用してください。
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		TYP* P = m_GetPointer(index);
		if ( P == NULL )
		{
			return false; 
		}
		*P = t;
		return true;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& operator[](INDEX index) const
	{
		return At(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。操作するとオブジェクト内の要素に影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP& operator[](INDEX index)
	{
		TYP* P = m_GetPointer(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
	}
};



/**@ingroup COLLECT
 * 連続メモリ配置型情報群管理インターフェーステンプレート
 *
 *		複数の要素型(TYP)を管理するインターフェースです。
 *		これを実装しているのは、情報を連続したメモリで配置して管理しているコレクションです。
 *		GetSize() でサイズを確認し、 ReferBuffer() を使い情報を参照することが出来ます（変更は出来ません）。
 *
 *	@param TYP 要素の型。
 *
 *	@par必要ファイル
 *			TnbCollection.h
 *
 *	@date 06/05/01 新規作成
 *	@date 06/05/07 ISequenceCollectionTに名前変更。
 *	@date 07/03/15 size管理を unsignedに変更。
 */
template<typename TYP>
struct ISequenceCollectionT
{
	/// デストラクタ
	virtual ~ISequenceCollectionT(void) {}

	/**
	 * [取得] データアドレス取得.
	 *	@note	このアドレスの内容は書き換えてはいけません。
	 *	@note	取得したアドレスは delete で開放してはいけません。
	 *	@note	取得したアドレスは本クラスの計算等、操作すると無効になります。
	 *	@return アドレス。確保していない場合、NULLが返ります。
	 */
	virtual const TYP* ReferBuffer(void) const = 0;

	/**
	 * [取得] 要素数取得.
	 *	@note Pure関数です。
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const = 0;
};



namespace HASH
{
	/** COLLECT
	 * [HASH] ハッシュ値計算.
	 *	@note TYP がシフト演算可能のもののみ。
	 *	@param c IConstCollectionT
	 *	@return ハッシュ値
	 */
	template<typename TYP>
	inline DWORD CalcHash(const IConstCollectionT<TYP>& c)
	{
		EXCLUSIVE(&c);
		DWORD dwRc = 0;
		loop ( i, c.GetSize() )
		{
			dwRc += c.At(i) << (i & 0x0F);
		}
		return dwRc;
	}
};



}; // TNB

