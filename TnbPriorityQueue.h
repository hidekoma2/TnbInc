#pragma once
/**
 *@file
 * 優先順位付キュー型情報管理関係のヘッダ
 *
 *		追加した情報を、優先順位が高いほうから取り出ることができるクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbListQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * 優先順位付キュー型情報管理テンプレート
 *	
 *		追加した情報を、優先順位が高いほうから取り出ることができるクラスがあります。
 *
 *	@note キューの大きさは可変です。メモリの許す限り入ります。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	IQueueT を実装しています。
 *
 *	@par必要ファイル
 *			TnbPrioritytQueue.h
 * 
 *	@date 06/04/25 新規作成
 *	@date 06/05/15 Cloneを廃止。
 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
 */
template<typename TYP>
class CPriorityQueueT : public ICollectionMidT<TYP>, public IQueueT<TYP>, CCopyImpossible
{
	DEFSUPER(ICollectionMidT<TYP>);
	CListT<CListQueueT<TYP> >	m_listQueue;	///< 情報管理
	int							m_iLevelMax;	///< レベル範囲

	/**
	 * レベル→インデックス変換
	 *	@param iLevel レベル
	 *	@retval INVALID_INDEX エラー。
	 *	@retval それ以外 インデックス。
	 */
	INDEX m_LevelToIndex(int iLevel)
	{
		if ( m_iLevelMax < iLevel )
		{
			return INVALID_INDEX;
		}
		iLevel += m_iLevelMax;
		if ( iLevel < 0 )
		{
			return INVALID_INDEX;
		}
		return iLevel;
	}

protected:

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const
	{
		if ( index >= 0 )
		{
			try
			{
				for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
				{
					size_t size = m_listQueue[i].GetSize();
					if ( size > index )
					{
						return &(m_listQueue[i][index]);	//範囲内のはず
					}
					index -= size;
				}
			}
			catch(CTnbException& e)
			{
				e.OnCatch();
			}
		}
		return NULL;
	}

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		if ( index >= 0 )
		{
			try
			{
				for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
				{
					size_t size = m_listQueue[i].GetSize();
					if ( size > index )
					{
						return &(m_listQueue[i][index]);	//範囲内のはず
					}
					index -= size;
				}
			}
			catch(CTnbException& e)
			{
				e.OnCatch();
			}
		}
		return NULL;
	}

public:

	/**
	 * コンストラクタ
	 *	@param iLevel プライオリティレベル範囲。たとえば４にすると、-４～４ までになります。
	 *					デフォルトでは、３（-３～３）までになっています。
	 */
	explicit CPriorityQueueT(int iLevel = 3) : _super()
	{
		if ( iLevel < 0 )
		{
			iLevel = -iLevel;
		}
		m_iLevelMax = iLevel;
		CListQueueT<TYP> q;
		loop ( i, iLevel * 2 + 1 )
		{
			m_listQueue.Add(q);
		}
	}

	/** 
	 * [排他] ロック
	 *	@note	Lock/Unlockは情報の排他のために使用します。スレッドセーブに
	 *			するために、一連の情報操作前にLockし操作終了後にUnlockする必要が
	 *			あります（単発の操作時は大丈夫です）。
	 *	@param dwTime ダミーです。省略してください。
	 *	@return 常にtrue。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		return m_listQueue.Lock();
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		m_listQueue.Unlock();
	}

	/**
	 * [取得] 要素数取得
	 *	@return 値
	 */
	virtual size_t GetSize(void) const 
	{
		EXCLUSIVE(this);
		size_t size = 0;
		for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
		{
			size += m_listQueue[i].GetSize();
		}
		return size;
	}


	/**
	 * [追加] 要素追加.
	 *		連続したデータをプライオリティレベル０のキューの最後に追加します。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 追加する要素。省略可能。
	 *	@return 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t)
	{
		return Add(0, t);
	}

	/**
	 * [追加] 複数要素追加.
	 *		連続したデータをプライオリティレベル０のキューの最後に追加します。
	 *	@param size 追加するデータの数
	 *	@param P 追加するデータ列のポインタ。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		return AddElements(0, size, P);
	}

	/**
	 * [削除] データ全削除.
	 *		キューの内容を空にします。
	 *	@return 常にtrue
	 */
	virtual bool RemoveAll(void)
	{
		return m_listQueue.RemoveAll();
	}
	
	/**
	 * [取得] レベル別要素数取得
	 *	@param iLevel プライオリティレベル
	 *	@retval INVALID_SIZE	エラー。レベル指定誤り
	 *	@retval それ以外		そのレベルの数。
	 */
	size_t GetSize(int iLevel) const 
	{
		EXCLUSIVE(this);
		INDEX index = m_LevelToIndex(iLevel);
		if ( index != INVALID_INDEX )
		{
			return m_listQueue[index].GetSize();
		}
		return INVALID_SIZE;
	}

	/**
	 * [追加] レベル別複数要素追加.
	 *		連続したデータを指定のプライオリティレベルのキューの最後に追加します。
	 *	@param iLevel プライオリティレベル
	 *	@param size 追加するデータの数。
	 *	@param P 追加するデータ列のポインタ。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	size_t AddElements(int iLevel, size_t size, const TYP* P = NULL)
	{
		EXCLUSIVE(this);
		if ( P != NULL )
		{
			loop ( i, size )
			{
				Add(iLevel, *P++);
			}
		}
		else
		{
			loop ( i, size )
			{
				Add(iLevel, TYP());
			}
		}
		return size;
	}

	/**
	 * [追加] レベル別要素追加.
	 *		連続したデータを指定のプライオリティレベルのキューの最後に追加します。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param iLevel プライオリティレベル
	 *	@param t 追加する要素
	 *	@retval INVALID_INDEX 失敗。(レベル指定不正)
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX Add(int iLevel, const TYP& t)
	{
		INDEX index = m_LevelToIndex(iLevel);
		if ( index == INVALID_INDEX )
		{
			return INVALID_INDEX;
		}
		return m_listQueue[index].Add(t);
	}

	/**
	 * [追加] レベル別要素追加.
	 *		連続したデータを指定のプライオリティレベルのキューの最後に追加します。
	 *	@param iLevel プライオリティレベル
	 *	@param t 追加する要素
	 *	@retval INVALID_INDEX 失敗。(レベル指定不正)
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX AddEx(int iLevel, const TYP& t)
	{
		EXCLUSIVE(this);
		return Add(iLevel, t);
	}

	/**
	 * [追加] レベル別追加.
	 *		連続したデータを指定のプライオリティレベルのキューの最後に追加します。
	 *	@note 追加元データの範囲を限定したい場合、CConstOffsetAdapterT を使用します。
	 *	@param iLevel プライオリティレベル
	 *	@param c 追加するデータ
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	size_t Append(int iLevel, const IConstCollectionT<TYP> &c)
	{
		EXCLUSIVE(this);
		INDEX index = m_LevelToIndex(iLevel);
		if ( index == INVALID_SIZE )
		{
			return INVALID_SIZE;
		}
		return m_listQueue[index].Append(c);
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
		for ( INDEX i = m_listQueue.GetSize() - 1; i >= 0; i-- )
		{
			if ( ! m_listQueue[i].IsEmpty() )
			{
				TYP t = m_listQueue[i].Get(0);
				m_listQueue[i].Remove(0);
				return t;
			}
		}
		ASSERT0(false, "CPriorityQueueT::Take()", "キューが空の時取得操作されました。");
		throw CEmptyException();
	}

	/**
	 * [取得] レベル別先頭要素取り出し.
	 *		キューから削除します
	 *	@param iLevel プライオリティレベル
	 *	@return 要素内容
	 *	@throw CEmptyException 空であることを示すためにスローされます。
	 */
	TYP Take(int iLevel)
	{
		EXCLUSIVE(this);
		INDEX index = m_LevelToIndex(iLevel);
		if ( index != INVALID_INDEX )
		{
			if ( ! m_listQueue[index].IsEmpty() )
			{
				TYP t = m_listQueue[index].Get(0);
				m_listQueue[index].Remove(0);
				return t;
			}
		}
		ASSERT0(false, "CPriorityQueueT::Take()", "キューが空の時取得操作されました。");
		throw CEmptyException();
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
		if ( size > GetSize() )
		{
			return INVALID_SIZE;
		}
		if ( P != NULL )
		{
			loop ( i, size )
			{
				*P++ = Take();
			}
		}
		else
		{
			loop ( i, size )
			{
				Take();
			}
		}
		return size;
	}
};



}; // TNB
