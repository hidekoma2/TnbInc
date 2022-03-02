#pragma once
/**
 *@file
 * ポインタ配列管理関係のヘッダ
 *
 *		newで確保した一つ型のポインタ配列を管理します。
 *		動的に配列を増やすことができます。
 *		コピーや削除で不要になったポインタは自動的に delete します。
 *		複製しても、ポインタ先の実体は複製しません。ですので、実体を操作すると、
 *		コピー元にも影響が出ます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * ポインタ配列管理テンプレート
 *
 *		newで確保した一つ型のポインタ配列を管理します。
 * 
 *		動的に配列を増やすことができます。
 *		コピーや削除で不要になったポインタは自動的に delete します。
 *		複製しても、ポインタ先の実体は複製しません。ですので、実体を操作すると、
 *		コピー元にも影響が出ます。
 *
 *	@attention	new[] で確保したポインタは管理できません。
 *
 *	@param 	TYP 保持する型（コピーできなくてもよし）
 *
 *	@see IConstCollectionT を実装しています。ポインタ保護のためICollectionT は実装していません。
 *
 *	@note IConstCollectionT の TYP は TYP* になります。
 *
 *	@par必要ファイル
 *			TnbPointerVector.h
 *
 *	@date 06/04/01 新規作成
 *	@date 06/04/21 コンストラクト時に追加サイズ指定可能にした。
 *	@date 06/04/22 ポインタ保護のため、継承から実装に切り替えた。
 *	@date 06/05/17 CVectorTの仕様変更のため、本クラスも修正。
 *	@date 06/05/19 「VectorのPH」から「PHのVector」に変更。
 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
 *	@date 06/08/30 シリアライズはサポート出来ないようにした。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 09/02/12 Insert() 追加。
 *	@date 11/03/02 RemoveElements() 追加。
 */
template<typename TYP>
class CPointerVectorT : public IConstCollectionT<TYP*>
{
	DEFSUPER(IConstCollectionT<TYP*>);
	CVectorT< CPointerHandleT<TYP> > m_vhPointer;	///< ポインタの配列
public:

	/**
	 * コンストラクタ
	 *	@note 指定のメモリ確保時の追加サイズを設定します。
	 *	@note データは保持しません
	 *	@param size メモリ確保時の追加サイズ
	 */
	explicit CPointerVectorT(size_t size = 0) : _super()
	{
		m_vhPointer.SetIncrementSize(size);
	}
	
	/**
	 * コピーコンストラクタ
	 *	@param v コピー元
	 */
	CPointerVectorT(const CPointerVectorT<TYP>& v) : _super(), m_vhPointer(v.m_vhPointer)
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CPointerVectorT& operator=(const CPointerVectorT<TYP>& other) 
	{
		m_vhPointer = other.m_vhPointer;
		return *this;
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ダミー。省略してください。
	 *	@return 常にtrue。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const { return true; }

	/** [排他] アンロック */
	virtual void Unlock(void) const {}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP* const& At(INDEX index) const
	{
		return *(m_vhPointer.At(index).ReferP());
	}

	/**
	 * デストラクタ
	 */
	virtual ~CPointerVectorT(void) 
	{
	}

	/**
	 * [削除] 要素削除
	 *	@param index インデックス
	 *	@retval true	削除成功
	 *	@retval false	失敗
	 */
	bool Remove(INDEX index)
	{
		return m_vhPointer.Remove(index);
	}

	/**
	 * [削除] 空化
	 */
	void RemoveAll(void)
	{
		m_vhPointer.RemoveAll();
	}

	/**
	 * [削除] 要素削除.
	 *	@param index インデックス
	 *	@param size 削除数。省略すると０。０を指定するとそれ以降全部削除します。
	 *	@retval INVALID_SIZE	エラー。
	 *	@return それ以外		削除した数。
	 */
	size_t RemoveElements(INDEX index, size_t size = 0)
	{
		return m_vhPointer.RemoveElements(index, size);
	}

	/**
	 * [操作] サイズ指定
	 *	@note 大きくすることも小さくすることも出来ます。
	 *	@note 大きくした時、そのデータは NULL で埋められます。
	 *	@note 小さくした時、不必要にあったデータは delete されます。
	 *	@param size サイズ
	 */
	void SetSize(size_t size)
	{
		m_vhPointer.SetSize(size);
	}

	/**
	 * [追加] 要素一つ追加
	 *	@param P データのポインタ。newで確保したものに限ります。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX Add(TYP* P)
	{
		return m_vhPointer.Add(P); 
	}

	/**
	 * [挿入] 要素一つ挿入
	 *	@param index 追加するINDEX。
	 *	@param P データのポインタ。newで確保したものに限ります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Insert(INDEX index, TYP* P)
	{
		return m_vhPointer.Insert(index, P); 
	}

	/**
	 * [設定] 要素の設定
	 *	@param index インデックス
	 *	@param P データのポインタ。newで確保したものに限ります。
	 *	@retval true	設定成功
	 *	@retval false	失敗(インデックスが範囲外、など)
	 */
	bool Set(INDEX index, TYP* P)
	{
		return m_vhPointer.Set(index, P); 
	}
	
	/**
	 * [取得] 要素参照取得
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP* operator[](INDEX index) const 
	{
		return Get(index); 
	}
	
	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize( void ) const
	{ 
		return m_vhPointer.GetSize(); 
	}

	/**
	 * [処理] シリアライズ
	 *	@param[out] _sr シリアライザー。 
	 *	@throw CNotSupportException サポートしていないことを示すためにスローされます。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		throw CNotSupportException();
	}
};



}; // TNB
