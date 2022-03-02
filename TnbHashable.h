#pragma once
/**
 *@file
 * HASH値保持関係のヘッダ
 *
 *		通常のオブジェクトにHASH値を付加するテンプレートクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSerializer.h"
#include "TnbComparator.h"



namespace TNB
{



/**@ingroup VALUE
 * HASH値保持テンプレートクラス.
 *
 *		通常のオブジェクトにHASH値を付加するテンプレートです。主に、大きな型をコレクションで
 *		使用する場合、ハッシュを付加することで、検索速度UPが期待できます。
 *
 *	@parハッシュ値：
 *		HASH値は以下の特徴があります。HashCalc() を作成する場合、以下の規則に従ってください。 
 *
 *		<table><tr><td><pre>
 *		プロセス実行中に、変化のないオブジェクトは、一定の整数を返す。
 *
 *		同じ内容のオブジェクトでも、異なるプロセスなら同じ整数とは限らない。
 *
 *		２つのオブジェクトの内容が異なる場合、それぞれのハッシュ値は必ず一致しない。
 *
 *		２つのオブジェクトの内容が同じ場合でも、それぞれのハッシュ値が一致するとは限らない。	
 *		ただし、極力同じ値になるようなハッシュ値であることが好ましい。
 *
 *		異なる型同士では、ハッシュ値は意味を持たない。 
 *		</pre></td></tr></table>
 *
 *	@par使用例
 *		CStr 系は CalcHash() メソッドが既に用意してあるため以下のよう使用できます。
 *		<table><tr><td><pre>
 *
 *		CVectorT< CHashableT<CStr> > vhstr;		//宣言
 *				;
 *		int f1 = vhstr.Find(CStr("ABCDE"));		
 *		</pre></td></tr></table>
 *
 *	@parハッシュ値計算作成例
 *		他のものを使用したい場合、以下のように CalcHash() メソッドを、 TNB::HASH ネームスペース下に
 *		作成します。
 *		<table><tr><td><pre>
 *
 *		namespace TNB
 *		{
 *		  namespace HASH
 *		  {
 *		    // ハッシュ値計算.
 *		    inline DWORD CalcHash(const TFoo& str)
 *		    { ～ }
 *		  };
 *		};
 *		</pre></td></tr></table>
 *
 *	@note ISerializable を持っています。シリアライズ可能です。
 *
 *	@param TYP 要素の型。
 *
 *	@par必要ファイル
 *			TnbHashable.h
 *
 *	@date 06/09/12 新規
 */
template<typename TYP>
class CHashableT : public IComparableT< TNB::CHashableT<TYP> >, public ISerializable
{
	DWORD	m_dwHash;	///< ハッシュ値
	TYP		m_typ;		///< 値

public:

	/**
	 * コンストラクタ.
	 */
	CHashableT(void) : m_typ(TYP()), m_dwHash(HASH::CalcHash(m_typ))
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@note コピー元のハッシュ値を引き継ぎます。
	 *	@param t コピー元
	 */
	CHashableT(const CHashableT<TYP>& t) : m_typ(t.m_typ), m_dwHash(t.m_dwHash)
	{
	}

	/**
	 * [代入] 代入コンストラクタ
	 *	@note 要素の値からハッシュ値の計算をしています。
	 *	@param t 要素
	 */
	CHashableT(const TYP& t) : m_typ(t)
	{
		m_dwHash = HASH::CalcHash(m_typ);
	}

	/**
	 * [代入] コピーオペレータ
	 *	@note コピー元のハッシュ値を引き継ぎます。
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CHashableT& operator=(const CHashableT<TYP>& other)
	{
		m_typ = other.m_typ;
		m_dwHash = other.m_dwHash;
		return *this;
	}

	/**
	 * [代入] 代入オペレータ
	 *	@note 要素の値からハッシュ値の計算をしています。
	 *	@param t 要素
	 *	@return 自分の参照
	 */
	CHashableT& operator=(const TYP& t)
	{
		m_typ = t;
		m_dwHash = HASH::CalcHash(m_typ);
		return *this;
	}

	/**
	 * [取得] HASH値取得
	 *	@return ハッシュ値
	 */
	DWORD GetHashCode(void) const
	{
		return m_dwHash;
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CHashableT<TYP>& t) const
	{
		return IComparatorT<TYP>::GetDefault().CompareTo(m_typ, t.m_typ);
	}

	/**
	 * [確認] 比較
	 *	@note まず、ハッシュ値の比較を行い、同じなら実際の値を比較します。
	 *	@param t 比較対象
	 *	@retval true 自分は t と同じ
	 *	@retval false 自分は t と異なる
	 */
	virtual bool IsEqual(const CHashableT<TYP>& t) const 
	{
		if ( m_dwHash != t.m_dwHash )
		{
			return false;	//ハッシュ値が異なるので、絶対違う
		}
		return IComparatorT<TYP>::GetDefault().IsEqualTo(m_typ, t.m_typ);
	}

	/**
	 * [取得] 値参照取得
	 *	@return 要素の参照
	 */
	const TYP& At(void) const
	{
		return m_typ; 
	}

	/**
	 * [取得] 値参照取得
	 *	@return 要素の参照
	 */
	operator const TYP&(void) const
	{
		return m_typ; 
	}

	/**
	 * [処理] シリアライズ.
	 *	@note 指定のシリアライザーに本オブジェクトの保持している情報を出力します。
	 *	@param[out] _sr シリアライザー
	 */
	virtual void Serialize(ISerializer& _sr) const	
	{
		_sr << m_typ;
	}

	/**
	 * [処理] デシリアライズ.
	 *	@note 指定のデシリアライザーからに本オブジェクトに情報を取り込みます。
	 *	@param[in] ds デシリアライザー
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> m_typ;
		m_dwHash = HASH::CalcHash(m_typ);
	}
};



namespace HASH
{
	/**
	 * [HASH] ハッシュ値計算.
	 *	@note 本 DWORD 用は、テスト。
	 *	@param dw
	 *	@return ハッシュ値
	 */
	inline DWORD CalcHash(DWORD dw)
	{
		return dw;
	}
};


}; // TNB
