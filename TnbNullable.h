#pragma once
/**
 *@file
 * NULL状態有り型関係のヘッダ
 *
 *		NULL状態を持たない型にNULL状態を付加するテンプレートクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSerializer.h"
#include "TnbComparator.h"



namespace TNB
{



/**@ingroup VALUE
 * NULL状態有り化テンプレートクラス.
 *
 *		NULL状態を持たない型にNULL状態を保持できる機能を付加するテンプレートです。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *
 *			CNullableT<int> x;
 *			x = 5;
 *			int i = x;
 *			x++;
 *			bool bbbb = (x.At() < 5);		//要素の型との比較時は、At()を使用
 *			x.IsNull();						//NULL化
 *		</pre></td></tr>
 *
 *	@note ISerializable を持っています。コレクションの要素に使用してもシリアライズが可能です。
 *
 *	@param TYP 要素の型。
 *
 *	@par必要ファイル
 *			TnbNullable.h
 *
 *	@date 06/09/12 新規
 */
template<typename TYP>
class CNullableT : public IComparableT< TNB::CNullableT<TYP> >, public ISerializable
{
	bool	m_boIsNull;
	TYP		m_typ;

public:

	/**
	 * コンストラクタ.
	 *	@note NULL状態のオブジェクトになります。
	 */
	CNullableT(void) : m_typ(TYP()) , m_boIsNull(true) 
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param t コピー元
	 */
	CNullableT(const CNullableT<TYP>& t) : m_typ(t.m_typ) , m_boIsNull(t.m_boIsNull) 
	{
	}

	/**
	 * [代入] 代入コンストラクタ
	 *	@param t 要素
	 */
	CNullableT(const TYP& t) : m_typ(t) , m_boIsNull(false) 
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CNullableT& operator=(const CNullableT& other)
	{
		m_typ = other.m_typ;
		m_boIsNull = other.m_boIsNull;
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@param t 要素
	 *	@return 自分の参照
	 */
	CNullableT& operator=(const TYP& t)
	{
		m_typ = t;
		m_boIsNull = false;
		return *this;
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CNullableT<TYP>& t) const
	{
		if ( m_boIsNull )
		{
			return t.m_boIsNull ? 0 : -1;
		}
		if ( t.m_boIsNull ){ return 1; } 
		return IComparatorT<TYP>::GetDefault().CompareTo(m_typ, t.m_typ);
	}

	/**
	 * [取得] 参照取得
	 *	@return 要素の参照。変更すると、本インスタンスに影響があります。
	 *	@throw CNullPointerException NULL状態であることを示すためにスローされます。
	 */
	TYP& At(void)
	{
		if ( m_boIsNull ){ throw CNullPointerException(); }
		return m_typ;
	}

	/**
	 * [取得] 値参照取得
	 *	@return 要素の参照
	 *	@throw CNullPointerException NULL状態であることを示すためにスローされます。
	 */
	const TYP& At(void) const
	{
		if ( m_boIsNull ){ throw CNullPointerException(); }
		return m_typ; 
	}

	/**
	 * [取得] 参照取得
	 *	@return 要素の参照。変更すると、本インスタンスに影響があります。
	 *	@throw CNullPointerException NULL状態であることを示すためにスローされます。
	 */
	operator TYP&(void) { return At(); }

	/**
	 * [取得] 値参照取得
	 *	@return 要素の参照
	 *	@throw CNullPointerException NULL状態であることを示すためにスローされます。
	 */
	operator const TYP&(void) const { return At(); }

	/**
	 * [取得] 値取得
	 *	@return 要素
	 *	@throw CNullPointerException NULL状態であることを示すためにスローされます。
	 */
	TYP operator+(void) const
	{
		return At(); 
	}
	
	/**
	 * [取得] マイナス単項
	 *	@return マイナス単項の結果
	 *	@throw CNullPointerException NULL状態であることを示すためにスローされます。
	 */
	TYP operator-(void) const
	{
		return -At(); 
	}

	/**
	 * [確認] NULL状態確認
	 *	@retval true NULL状態。この状態で要素にアクセスすると、スローされます。
	 *	@retval false 非NULL状態。
	 */
	bool IsNull(void) const { return m_boIsNull; }

	/**
	 * [設定] NULL状態化
	 */
	void Null(void)
	{
		m_typ = TYP();
		m_boIsNull = true; 
	}

	/**
	 * [処理] シリアライズ.
	 *	@note 指定のシリアライザーに本オブジェクトの保持している情報を出力します。
	 *	@param[out] _sr シリアライザー
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << m_boIsNull << m_typ;
	}

	/**
	 * [処理] デシリアライズ.
	 *	@note 指定のデシリアライザーからに本オブジェクトに情報を取り込みます。
	 *	@param[in] ds デシリアライザー
	 */
	virtual void Deserializer(const IDeserializer& ds)
	{
		ds >> m_boIsNull >> m_typ;
	}
};



}; // TNB
