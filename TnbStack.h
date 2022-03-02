#pragma once
/**
 *@file
 * スタック型情報管理関係のヘッダ
 *
 *		情報をLIFO(Last In First Out)で管理するクラスです。 
 *		Add系メソッドで追加し、Take系メソッドで新しいほうから情報を取り出します。
 *		また、Get,Set メソッドによる、インデックスによりアクセスも可能です。
 *		この時インデックスは、古い順に割り振られています。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * スタック型情報管理テンプレート
 *
 *		情報をLIFO(Last In First Out)で管理するクラスです。 
 *		Add系メソッドで追加し、Take系メソッドで新しいほうから情報を取り出します。
 *		また、Get,Set メソッドによる、インデックスによりアクセスも可能です。
 *		この時インデックスは、古い順に割り振られています。
 *
 *	@note 	同じ型のインスタンスは代入が可能です。この時、左辺のインス
 *			タンスは、情報を破棄し、右辺のインスタンスの情報を共有するように
 *			なります（そのため代入作業は<b>高速</b>です）。
 *			その後、いずれかのインスタンスを操作すると、
 *			情報を複製し他のインスタンスに影響がないようにしています。
 *			（複製を作成する時、時間がかかる可能性が有ります）
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	ISequenceCollectionT を実装しています。
 *	@see	IQueueT を実装しています。
 *
 *	@par必要ファイル
 *			TnbStack.h
 * 
 *	@date 06/05/01 新規作成
 *	@date 06/05/15 Cloneを廃止。
 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
 *	@date 06/08/08 CVectorT を親クラスに変更。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP>
class CStackT : public CVectorT<TYP> , public IQueueT<TYP>
{
	DEFSUPER(CVectorT<TYP>);
public:

	/**
	 * コンストラクタ 
	 *	@note 指定のメモリ確保時の追加サイズを設定します。
	 *	@note データは保持しません
	 *	@param size メモリ確保時の追加サイズ。SetIncrementSize() と同じです。
	 */
	explicit CStackT(size_t size = 0) : _super(size) 
	{
	}

	/** デストラクタ */
	virtual ~CStackT(void) 
	{
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
	 * [取得] 先頭要素取り出し.
	 *		キューから削除します
	 *	@return 要素内容
	 *	@throw CEmptyException 空であることを示すためにスローされます。
	 */
	virtual TYP Take(void)
	{
		EXCLUSIVE(this);
		size_t l = GetSize();
		if ( l == 0 )
		{
			throw CEmptyException();
		}
		TYP t = _super::Get(l - 1);
		Remove(l - 1);
		return t;
	}

	/**
	 * [取得] 複数要素取り出し
	 *	@note	指定バッファにコピーし、キューから削除します。
	 *	@param[in]	size	取り出す数。
	 *	@param[out]	P		取り出すバッファ。 NULLなら取得しません。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		取り出した数。
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		EXCLUSIVE(this);
		size_t l = GetSize();
		if ( l == 0 || size > l )
		{
			return INVALID_SIZE; 
		}
		l--;
		if ( P != NULL )
		{
			for ( INDEX i = 0; i < size; i++ )
			{
				*P++ = _super::At(l - i);
			}
		}
		loop ( i, size )
		{
			_super::Remove(l - i);
		}
		return size;
	}

private:
	friend class CStackTest;///< フレンドクラス宣言
};



}; // TNB
