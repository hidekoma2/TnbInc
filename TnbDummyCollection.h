#pragma once
/**
 *@file
 * ダミーコレクション関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * ダミーコレクションテンプレート
 *
 *		サイズしか管理しないコレクションです。
 *		代入しても取得するとすべてコンストラクタで指定した TYP の値になってしまいます。
 *
 *		ICollectionT インターフェースを欲する API に対して、サイズのみ欲しい時などに
 *		使用できます。
 *	
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbDummyCollection.h
 *
 *	@date 06/09/01 新規
 *	@date 06/09/06 サイズとIndexは正しくチェックする様にした。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 10/04/09 コンストラクタで、値を指定できるようにした。
 */
template<typename TYP>
class CDummyCollectionT : public ICollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
public:

	/**
	 * コンストラクタ.
	 *	@param typ 扱う唯一の値.
	 */
	CDummyCollectionT(const TYP& typ = TYP()) : _super(), m_data(typ), m_size(0)
	{
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
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 一時的な参照。内容は TYP() です。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
		return m_data;
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
		if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
		m_temp = m_data;
		return m_temp;
	}

	/**
	 * [設定] 要素の設定.
	 *	@note インデックスチェックのみします。 t は記憶しません。
	 *	@param index インデックス
	 *	@param t 内容
	 *	@return 常に true 
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		return IsInRange(index);
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@note サイズを＋１します。 t は記憶しません。
	 *	@param t 要素。
	 *	@return 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t)
	{
		return m_size++;
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@note サイズを－１します。
	 *	@param index インデックス
	 *	@retval true	削除成功
	 *	@retval false	失敗
	 */
	virtual bool Remove(INDEX index)
	{
		if ( ! IsInRange(index) )
		{
			return false; 
		}
		m_size--;
		return true;
	}

	/**
	 * [削除] 全要素削除 .
	 *	@note サイズを０にします。
	 *	@return 常に true 
	 */
	virtual bool RemoveAll(void)
	{
		m_size = 0;
		return true;
	}

private:
	size_t			m_size;			///< 管理サイズ
	TYP				m_data;			///< データ
	mutable TYP		m_temp;			///< 参照用
};



}; // TNB
