#pragma once
/**
 *@file
 * キュー型情報管理関係のヘッダ
 *
 *		情報をFIFO(First In First Out)で管理するクラスです。 
 *		Add系メソッドで追加し、Take系メソッドで古いのほうから情報を取り出します。
 *		また、Get,Set メソッドによる、インデックスによりアクセスも可能です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbQueue.h"
#include "TnbList.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * リスト式キュー型情報管理テンプレート
 *	
 *	@note キューの大きさは可変です。メモリの許す限り入ります。
 *
 *	@note	各要素にパラメータ値（32bitの任意の値）を一つにつけることが出来ます。
 *			この値は、Append系 や Copy系 を使って追加、複製すると、0になってしまいます。
 *			ListQueue 同士の連結、複製は、オペレータを使ってください。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	IQueueT を実装しています。
 *
 *	@par必要ファイル
 *			TnbListQueue.h
 * 
 *	@date 06/04/25	新規作成
 *	@date 06/05/15 Cloneを廃止。
 *	@date 06/05/29 各要素にパラメータをつけれられるようにした。
 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
 *	@date 06/08/08 CListT を親クラスに変更。
 */
template<typename TYP>
class CListQueueT : public CListT<TYP> , public IQueueT<TYP>
{
	DEFSUPER(CListT<TYP>);
public:

	/** コンストラクタ */
	CListQueueT(void) : _super()
	{
	}

	/**
	 * コピーコンストラクタ 
	 *	@param other コピー元
	 */
	CListQueueT(const CListQueueT& other) : _super(other)
	{
	}

	/**
	 * デストラクタ
	 */
	virtual ~CListQueueT(void) {}

	/**
	 * [代入] コピーオペレータ 
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CListQueueT& operator=(const CListQueueT& other)
	{
		_super::operator=(other);
		return *this;
	}

	/**
	 * [追加] 追加オペレータ
	 *	@note 自インスタンスで管理しているデータに指定のデータを
	 *		 追加します。
	 *	@param queue 追加するListQueue
	 *	@return 自分の参照
	 */
	CListQueueT& operator+=(const CListQueueT<TYP>& queue)
	{
		_super::operator+=(queue);
		return *this;
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t)
	{
		return _super::Add(t);
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
		return _super::AddElements(size, P);
	}

	/**
	 * [追加] 要素追加.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *		要素一つをキューの最後に追加する。
	 *	@param t 追加する要素。省略可能。
	 *	@param dwParam パラメータ値。
	 *	@return 追加したINDEX。
	 */
	INDEX Add(const TYP& t, DWORD dwParam)
	{
		return _super::Add(t, dwParam);
	}

	/**
	 * [追加] 要素追加.
	 *		要素一つをキューの最後に追加する。
	 *	@param t 追加する要素。省略可能。
	 *	@param dwParam パラメータ値。
	 *	@return 追加したINDEX。
	 */
	INDEX AddEx(const TYP& t, DWORD dwParam)
	{
		return _super::AddEx(t, dwParam);
	}

	/**
	 * [取得] 複数要素取り出し
	 *	@note	指定バッファにコピーし、キューから削除します。
	 *	@param[in]	size	取り出す数。
	 *	@param[out]	P		取り出すバッファ。 NULLなら取得しません。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		EXCLUSIVE(this);
		if ( size > GetSize() )
		{
			return INVALID_SIZE;
		}
		if ( P != NULL )
		{
			loop ( i, size )
			{
				*P++ = _super::At(i);
			}
		}
		loop ( i, size )
		{
			_super::Remove(0);
		}
		return size;
	}

private:
	friend class CListQueueTest;	///< フレンドクラス宣言
};



}; // TNB
