#pragma once
/**
 *@file
 * 配列型情報管理関係のヘッダ
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *		各要素にパラメータ(属性)をつけることが可能です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * 配列型情報管理テンプレート
 *
 *		型の配列を管理します。
 *
 *	@note	各要素に属性値（任意の型）を一つにつけることが出来ます。
 *			この値は、Append系 や Copy系 を使って追加、複製すると、 ATT() になります。
 *			List 同士の連結、複製は、オペレータを使ってください。
 *
 *	@note	AttributeVector のシリアライズ、デシリアライズは、必ず AttributeVector
 *			で行ってください。属性情報があるため、他のコレクションと互換が有りません。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *	@param ATT TYP と対に保持する属性型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbAttributedVector.h
 *
 *	@date 07/06/19 新規作成
 *	@date 10/02/03 構造変更
 *	@date 11/04/19 SetElements() のオーバーライド。処理を修正。
 */
template<typename TYP, typename ATT = DWORD>
class CAttributedVectorT : public CVectorT<TYP>
{
	DEFSUPER(CVectorT<TYP>);
public:

	/// コンストラクタ
	CAttributedVectorT(void) : _super()
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CAttributedVectorT(const CAttributedVectorT& other) : _super(other), m_vAttr(other.m_vAttr)
	{
	}

	/// デストラクタ
	virtual ~CAttributedVectorT(void)
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CAttributedVectorT& operator=(const CAttributedVectorT& other)
	{
		_super::operator=(other);
		m_vAttr = other.m_vAttr;
		return *this;
	}

	/**
	 * [追加] 追加オペレータ.
	 *		自インスタンスで管理しているデータに指定のデータを
	 *		追加します。
	 *	@param other 追加データ
	 *	@return 自分の参照
	 */
	CAttributedVectorT& operator+=(const CAttributedVectorT& other)
	{
		_super::operator+=(other);
		m_vAttr += other.m_vAttr;
		return *this;
	}

	/**
	 * [作成] 連結
	 *	@note	自インスタンスで管理しているデータに指定のデータを
	 *			連結した、 CVectorT を作成します。
	 *			自インスタンスは変化しません。
	 * @param other 連結するデータ
	 * @return 新規インスタンス
	 */
	CAttributedVectorT operator+(const CAttributedVectorT& other) const
	{
		CAttributedVectorT av;
		CVectorT<TYP>& v1 = av;
		const CVectorT<TYP>& v2 = *this;
		v1 = v2 + other;
		av.m_vAttr = m_vAttr + other.m_vAttr;
		return av;
	}

	/**
	 * [取得] データ参照取得.
	 *	@return データ配列の参照
	 */
	virtual const CVectorT<TYP>& Refer(void) const
	{
		return *this;
	}

	/**
	 * [削除] 要素一つ削除
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 RemoveEx() を使用してください。
	 *	@param index インデックス
	 *	@retval true	削除成功
	 *	@retval false	失敗（インデックス無効）
	 */
	virtual bool Remove(INDEX index)
	{
		return _super::Remove(index) && m_vAttr.Remove(index);
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
		size_t r = _super::SetElements(size, P);
		m_vAttr.RemoveAll();
		m_vAttr.SetSize(_super::GetSize());
		return r;
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
		return Set(index, t, 0);
	}

	/**
	 * [設定] 要素の設定.
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	virtual bool SetEx(INDEX index, const TYP& t)
	{
		return SetEx(index, t, 0);
	}

	/**
	 * [削除] 全要素削除 
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool RemoveAll(void)
	{
		return _super::RemoveAll() && m_vAttr.RemoveAll();
	}

	/**
	 * [設定] 要素の入れ替え
	 *	@note ポインタだけを入れ替えています。要素のコピー処理は発生しません。
	 *	@param index1 インデックス１
	 *	@param index2 インデックス２
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual void Swap(INDEX index1, INDEX index2)
	{
		_super::Swap(index1, index2);
		m_vAttr.Swap(index1, index2);
	}

	/**
	 * [追加] 要素一つ追加
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 要素。
	 *	@param attribute 属性値。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX Add(const TYP& t, const ATT& attribute)
	{
		INDEX r1 = _super::Add(t);
		INDEX r2 = m_vAttr.Add(attribute);
		if ( IsInvalid(r1)  || IsInvalid(r2) )
		{
			return INVALID_INDEX;
		}
		ASSERTLIB( r1 == r2 );
		return r1;
	}

	/**
	 * [追加] 要素一つ追加
	 *	@param t 要素。
	 *	@param attribute 属性値。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX AddEx(const TYP& t, const ATT& attribute)
	{
		EXCLUSIVE(this);
		return Add(t, attribute);
	}

	/**
	 * [設定] 要素の設定
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 SetEx() を使用してください。
	 *	@param index インデックス
	 *	@param t 内容
	 *	@param attribute 属性値。
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	bool Set(INDEX index, const TYP& t, const ATT& attribute)
	{
		return _super::Set(index, t) && m_vAttr.Set(index, attribute);
	}

	/**
	 * [設定] 要素の設定
	 *	@param index インデックス
	 *	@param t 内容
	 *	@param attribute 属性値。
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	bool SetEx(INDEX index, const TYP& t, const ATT& attribute)
	{
		EXCLUSIVE(this);
		return Set(index, t, attribute);
	}

	/**
	 * [取得] 要素の属性値、取得
	 *	@param index インデックス
	 *	@return 属性値
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	ATT GetAttribute(INDEX index) const
	{
		EXCLUSIVE(this);
		return m_vAttr.Get(index);
	}

	/**
	 * [設定] 要素の属性値、設定
	 *	@param index インデックス
	 *	@param attribute 属性値。
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	bool SetAttribute(INDEX index, const ATT& attribute)
	{
		return m_vAttr.SetEx(index, attribute);
	}

	/**
	 * [追加] 要素一つ挿入.
	 *		指定のインデックスに要素を追加します。
	 *	@param index インデックス
	 *	@param t 要素
	 *	@param attribute 属性値。
	 *	@retval true	挿入成功
	 *	@retval false	失敗（インデックス無効）
	 */
	bool Insert(INDEX index, const TYP& t, const ATT& attribute = ATT())
	{
		EXCLUSIVE(this);
		return _super::Insert(inex, t) && m_vAttr.Insert(index, attribute);
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
		return _super::SetSize(size) && m_vAttr.SetSize(size);
	}

	/**
	 * [処理] シリアライズ
	 *	@note CAttributeVectorT のシリアライズデータは、必ず、同じCAttributeVectorTでデシリアライズしてください。
	 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_super::Serialize(_sr);
		_sr << m_vAttr;
	}

	/**
	 * [処理] デシリアライズ
	 *	@note CAttributeVectorT のシリアライズデータは、必ず、同じ CAttributeVectorT でデシリアライズしてください。
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		_super::Deserialize(ds);
		ds >> m_vAttr;
	}

private:
	CVectorT<ATT>		m_vAttr;	///< 属性の配列
};



}; // TNB
