#pragma once
/**
 *@file
 * 配列型情報管理関係のヘッダ
 *
 *		型の配列を管理します。
 *
 *		動的に配列を増やすことができます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * 配列型情報管理テンプレート
 *
 *		動的に配列を増やすことができます。
 *
 *		追加時にメモリ確保は行いますが、再配置を行わないため、
 *		他の Vectorに比べて、軽量です。
 *
 *	@note 情報をブロック単位で管理しており、それらがリンクして一つの配列として処理しています。
 *
 *	
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@par必要ファイル
 *			TnbBlockLinkVector.h
 *
 *	@date 11/02/10 新規
 *	@date 11/06/01 開放方法に誤り有り。修正。
 *	@date 11/08/25 ブロックサイズを 1 の n 乗 にすることで、パフォーマンスアップ。
 *	@date 11/09/06 GetBlockInformation(), GetBlockCount() を追加。
 *	@date 14/05/18 RemoveElements() を改良。
 */
template<typename TYP>
class CBlockLinkVectorT : public ICollectionMidT<TYP>, CCopyImpossible
{
	DEFSUPER(ICollectionMidT<TYP>);
public:

	/// ブロック情報
	struct TBlockInformation
	{
		const TYP*	pData;	///< 要素配列
		size_t		size;	///< 要素数
	};


	//-------


	/**
	 * コンストラクタ.
	 *	ブロックサイズは 1 の 10 乗になります。
	 */
	CBlockLinkVectorT(void) : m_totalSize(0)
	{
		m_blockShift = 10;
		m_blockSize = down_cast<size_t>(_BIT(m_blockShift));
		m_blockMask = m_blockSize - 1;
	}

	/**
	 * コンストラクタ.
	 *	@param bs ブロックサイズ。実際には丸められ、指定のサイズを超える 1 の n 乗の値になります。
	 */
	CBlockLinkVectorT(size_t bs) : m_totalSize(0)
	{
		m_blockShift = 1;	///< m_blockSize と関連
		m_blockSize = 1 << m_blockShift;
		while ( m_blockSize < bs )
		{
			m_blockSize <<= 1;
			m_blockShift++;
			if ( m_blockSize == 0 )
			{
				m_blockShift = 7;
				m_blockSize = 1 << m_blockShift;
			}
		}
		m_blockMask = m_blockSize - 1;
	}

	/// デストラクタ
	~CBlockLinkVectorT(void)
	{
		RemoveAll();
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ロックにトライし続けるﾐﾘ秒。省略すると無限に待ちます。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return m_sync.Lock(dwTime);
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		m_sync.Unlock();
	}

	/**
	 * [取得] 要素数取得.
	 *	@note Pure関数です。
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_totalSize;
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
		size_t i = m_totalSize;
		INDEX bn = i >> m_blockShift;
		INDEX idx = i & m_blockMask;
		if ( bn >= m_blocks.GetSize() )
		{
			TYP* P = new TYP[m_blockSize];
			m_blocks.Add(P);
		}
		TYP* B = m_blocks[bn];
		B[idx] = t;
		m_totalSize++;
		return i;
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
		INDEX is = m_totalSize;
		size_t i = m_totalSize - 1 + size;
		INDEX bn = i >> m_blockShift;
		while ( bn >= m_blocks.GetSize() )
		{
			TYP* P = new TYP[m_blockSize];
			m_blocks.Add(P);
		}
		m_totalSize += size;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				Set(is + i, *P++);
			}
		}
		else
		{
			loop ( i, min(m_blockSize, size) )	//memo 全部やる必要はないので、一ブロック分のみ
			{
				Set(is + i, TYP());
			}
		}
		return size;
	}

	/**
	 * [削除] 全要素削除 .
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool RemoveAll(void)
	{
		m_totalSize = 0;
		m_blocks.RemoveAll();
		return true;
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 RemoveEx() を使用してください。
	 *	@param index インデックス
	 *	@retval false	エラー。
	 *	@retval true	成功。
	 */
	virtual bool Remove(INDEX index)
	{
		if ( index >= m_totalSize )
		{
			return false;
		}
		size_t d = m_totalSize - index;
		if ( d > 1 )
		{
			loop ( i, d - 1 )
			{
				Set(index + i, At(index + i + 1));
			}
		}
		m_totalSize--;
		if ( m_totalSize == 0 )
		{
			m_blocks.RemoveAll();
		}
		else
		{
			INDEX bn = (m_totalSize - 1) >> m_blockShift;
			if ( bn < m_blocks.GetSize() )
			{
				m_blocks.SetSize(bn + 1);
			}
		}
		return true;
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
		EXCLUSIVE( &m_sync );
		size_t sz = GetSize();
		if ( sz < index )
		{
			return INVALID_SIZE;
		}
		if ( size == 0 || sz < index + size)
		{
			SetSize(index);
			return sz - index;
		}
		size_t z = sz - size - index;
		loop ( i, z )
		{
			Set(index + i, Get(index + size + i));
		}
		SetSize(index + z);
		return size;
	}

	/**
	 * [取得] ブロック数取得.
	 *		管理しているブロック数を取得できます。
	 *	@attention このメソッドで得られる情報は、 Set() , Add() などの情報を変更するメソッドを
	 *			コールすると無効な値になる場合が有ります。
 	 *	@return ブロック数
 	 */
	size_t GetBlockCount(void) const
	{
		return m_blocks.GetSize();
	}

	/**
	 * [取得] ブロック情報取得.
	 *		管理しているブロック情報を取得できます。
	 *	@attention このメソッドで得られる情報は、 Set() , Add() などの情報を変更するメソッドを
	 *			コールすると無効な値になる場合が有ります。
	 *	@param[out] _bi ブロック情報が格納されます。
	 *	@param[in] index ブロックインデックス。 0 以上、 GetBlockCount() 未満の値が指定できます。
 	 *	@retval true 成功
 	 *	@retval false 失敗
 	 */
	bool GetBlockInformation(TBlockInformation& _bi, INDEX index) const
	{
		if ( m_blocks.IsInRange(index ) )
		{
			_bi.size = m_blockSize;
			if ( index == m_blocks.GetSize() - 1 )
			{
				_bi.size = (m_totalSize & m_blockMask);
			}
			_bi.pData = m_blocks[index];
			return true;
		}
		return false;
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
		if ( index >= m_totalSize )
		{
			return NULL;
		}
		INDEX bn = index >> m_blockShift;
		INDEX idx = index & m_blockMask;
		TYP* B = m_blocks[bn];
		return &B[idx];
	}

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		if ( index >= m_totalSize )
		{
			return NULL;
		}
		INDEX bn = index >> m_blockShift;
		INDEX idx = index & m_blockMask;
		TYP* B = m_blocks[bn];
		return &B[idx];
	}

private:
	CSyncSection						m_sync;			///< 排他用
	size_t								m_totalSize;	///< 全体のサイズ
	CVectorT<CArrayPtrHandleT<TYP> >	m_blocks;		///< ブロック毎に管理
	size_t								m_blockSize;	///< 一つのブロックのサイズ
	int									m_blockShift;	///< m_blockSize と関連
	INDEX								m_blockMask;	///< m_blockSize と関連
	friend class CBlockListVectorTest;	///<フレンドクラス宣言
};



}; // TNB

