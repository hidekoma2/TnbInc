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



#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * キュー型情報管理インターフェーステンプレート
 *
 *		情報をFIFO(First In First Out)で管理するクラスです。 
 *		Add系メソッドで追加し、Take系メソッドで古いのほうから情報を取り出します。
 *	
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	本IF を使うサブクラスは ICollectionT も一緒に実装します。
 *
 *	@date 06/04/17 CRingQueueを抽象化して作成
 *	@date 06/05/15 Cloneを廃止。
 *	@date 06/05/23 AddElementsなど、IFに移行したメソッドの削除。
 *	@date 06/08/08 ICollectionT の継承を廃止。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP>
struct IQueueT
{
	/// デストラクタ
	virtual ~IQueueT(void) 
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
	virtual size_t AddElements(size_t size, const TYP* P = NULL) = 0;

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
	 * [取得] 複数要素取り出し
	 *	@note	指定バッファにコピーし、キューから削除します。
	 *	@param[in]	size	取り出す数。
	 *	@param[out]	P		取り出すバッファ。 NULLなら取得しません。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		取り出した数。
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL) = 0;

	/**
	 * [取得] 先頭要素取り出し.
	 *		キューから削除します
	 *	@return 要素内容
	 *	@throw CEmptyException 空であることを示すためにスローされます。
	 */
	virtual TYP Take(void)
	{
		TYP t;
		if ( TakeElements(1, &t) == 1 )
		{
			return t;
		}
		ASSERT0( false, "IQueue::Take()", "キューが空の時取得操作されました。" );
		throw CEmptyException();
	}

	/**
	 * [削除] 要素削除.
	 *		キューから指定数のデータを削除します。
	 *	@param size 削除したい数。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		削除した数。
	 */
	virtual size_t EraseElements(size_t size)
	{
		return TakeElements(size);
	}

	/**
	 * [取得] 要素配列取出し.
	 *		指定バッファにコピーし、キューから削除します。
	 *	@param[in]	size	取り出す数。
	 *	@return 要素配列。配列がEmptyの場合、指定数のデータがなかったという意味。
	 */
	CVectorT<TYP> TakeoutToVector(size_t size)
	{
		CVectorT<TYP> v;
		bool boRc = (TakeElements(size, v.GetBuffer(size)) == size);
		v.ReleaseBuffer();
		if ( ! boRc )
		{
			v.Invalid();
		}
		return v;
	}
};



/**@ingroup COLLECT ITERATOR
 * リング式キュー型情報管理テンプレート
 *	
 *	@note キューの大きさは固定です。最初にすべてのインスタンスが作られます
 *			(破棄されるのは、デストラクタの時です)。
 *			そのためメモリの確保開放が発生しません。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *	@param ISI	trueならGet/Eraseした時に、不必要になったバッファの部分を
 *				INV を代入して初期化します。
 *				TYPがクラスでメモリを確保しているときなどにtrueにします。
 *	@param INV	 初期化時に使う値。省略すると、TYP()が使われます。
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	IQueueT を実装しています。
 *
 *	@par必要ファイル
 *			TnbQueue.h
 * 
 *	@date 06/01/01 新規作成
 *	@date 06/04/13 CCopyImpossibleクラス、継承
 *	@date 06/04/25 コピー機能、追加。
 *	@date 06/05/15 Cloneを廃止。
 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 11/03/07 SetQueueSize(), GetQueueSize() を追加。構造整理。
 *	@date 13/02/28 IsFull() 新規、追加。
 */
template<typename TYP, bool ISI = false, class INV = TYP>
class CRingQueueT : public ICollectionMidT<TYP>, public IQueueT<TYP>
{
	DEFSUPER(ICollectionMidT<TYP>);
public:

	/**
	 * コンストラクタ
	 *	@note	コンストラクタ時に、リングキューの最大サイズを指定します。
	 *			動的に大きくなりません。
	 *	@param size リングキューのサイズ。省略すると10000です。
	 */
	explicit CRingQueueT(size_t size = 10000) : _super(), m_hptHead(new THead(size))
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@note コピー元とバッファを共有します。
	 *	@param queue コピー元の CRingQueueT インスタンス
	 */
	CRingQueueT(const CRingQueueT& queue) : _super(), m_hptHead(queue.m_hptHead)
	{
	}
	
	/**
	 * デストラクタ
	 */
	virtual ~CRingQueueT(void)
	{
		m_hptHead.Null();
	}

	/**
	 * [代入] コピーオペレータ
	 *	@note 自インスタンスで管理しているバッファを開放し、
	 *		代入元とバッファを共有します。
	 *	@param other 代入するCRingQueueT
	 *	@return 自分の参照
	 */
	CRingQueueT& operator=(const CRingQueueT& other)
	{
		EXCLUSIVE(&other);
		m_hptHead = other.m_hptHead;
		return *this;
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
		return m_syncFunc.Lock();
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		m_syncFunc.Unlock();
	}

	/**
	 * [設定] リングキューサイズ指定.
	 *	@note コンストラクタでも指定できます。
	 *	@note このメソッドを使うと、情報はクリアされます。
	 *	@param size リングキューのサイズ。
	 */
	void SetQueueSize(size_t size)
	{
		m_hptHead = new THead(size);
		ASSERT( GetSize() == 0 );
	}

	/**
	 * [取得] リングキューサイズ取得.
	 *	@return リングキューのサイズ。
	 */
	size_t GetQueueSize(void) const
	{
		return m_hptHead->bufferSize;
	}

	/**
	 * [取得] 要素数取得
	 *	@return サイズ
	 */
	virtual size_t GetSize(void) const 
	{
		return m_hptHead->dataSize;
	}

	/**
	 * [確認] 満タン？
	 *	@retval true キューは満タン。これ以上追加できない状態。
	 *	@retval false 否。
	 */
	bool IsFull(void) const
	{
		if ( GetQueueSize() <= GetSize() )
		{
			return true;
		}
		return false;
	}

	/**
	 * [追加] 要素追加.
	 *		要素一つをキューの最後に追加する。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 追加する要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t)
	{
		size_t r = GetSize();
		return (AddElements(1, &t) == 1) ? r : INVALID_INDEX;
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
		if ( IsInRange(index) )
		{
			m_Separate();
			THead* H = *m_hptHead.ReferP();
			size_t sz = H->dataSize;
			INDEX ri = H->readIndex;
			size_t bs	= H->bufferSize;
			loop ( i, sz -index - 1 )
			{
				INDEX dst = (ri + i + index) % bs;
				INDEX src = (ri + i + index + 1) % bs;
				H->pBuffer[dst] = H->pBuffer[src];
			}
			if ( ISI )
			{
				INDEX dst = (ri + sz - 1) % bs;
				H->pBuffer[dst] = INV();
			}
			(H->dataSize)--;
			return true;
		}
		return false; 
	}

	/**
	 * [削除] データ全削除.
	 *		キューの内容を空にします。
	 *	@return 常にtrue
	 */
	virtual bool RemoveAll(void)
	{
		EXCLUSIVE(this);
		size_t l = m_hptHead->dataSize;
		if ( l > 0 )
		{
			TakeElements(l);
			ASSERT( m_hptHead->dataSize == 0 );
		}
		return true;
	}

	/**
	 * [追加] 複数要素追加.
	 *		連続したデータをキューの最後に追加します。
	 *	@attention	指定したデータ数、全部入る空きがないとエラーになります。
	 *	@param size 追加するデータの数
	 *	@param P 追加するデータ列のポインタ。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		EXCLUSIVE(this);
		m_Separate();
		THead* H = *m_hptHead.ReferP();
		if ( (H->dataSize) + size > (H->bufferSize) )
		{
			//それ以上になるならエラー
			return INVALID_SIZE;
		}
		H->syncObj.Lock();
		size_t bs	= H->bufferSize;
		INDEX tmp = (H->readIndex + H->dataSize) % bs;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				H->pBuffer[tmp++] = *P++;
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		else
		{
			loop ( i, size )
			{
				H->pBuffer[tmp++] = TYP();
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(H->dataSize) += size;
		H->syncObj.Unlock();
		return size;
	}

	/**
	 * [取得] 複数要素取り出し.
	 *		指定バッファにコピーし、キューから削除します。
	 *	@attention	指定数分、データ無いとエラーになります。
	 *	@param[in]	size	取り出す数。
	 *	@param[out]	P		取り出すバッファ。 NULLなら取得しません。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		取り出した数。
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		EXCLUSIVE(this);
		m_Separate();
		THead* H = *m_hptHead.ReferP();
		H->syncObj.Lock();
		if ( (H->dataSize) < size )
		{
			//キューにその長さの情報がないのでエラー
			H->syncObj.Unlock();
			return INVALID_SIZE;
		}
		size_t bs = H->bufferSize;
		if ( P != NULL || ISI )
		{
			INDEX tmp = H->readIndex;
			loop ( i, size )
			{
				if ( P != NULL )
				{
					*P++ = H->pBuffer[tmp];
				}
				if ( ISI )
				{
					H->pBuffer[tmp] = INV();
				}
				if ( ++tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(H->readIndex) += size;
		(H->readIndex) %= bs;
		(H->dataSize) -= size;
		H->syncObj.Unlock();
		return size;
	}

	/**
	 * [取得] 格納可能数取得.
	 *	@note Ringキューはリングキューのサイズが固定です。
	 *	@return サイズ
	 */
	size_t GetFreeSize(void) const
	{
		return (m_hptHead->bufferSize) - (m_hptHead->dataSize);
	}

protected:

	/// 配列型情報管理の管理型
	struct THead
	{
		CSyncSection	syncObj;		///< 参照排他
		CSyncEvent		syncTaking;		///< Takeした時一時的にUnlockされる(CBlockingQueueT用)
		CSyncEvent		syncAdding;		///< Addした時一時的にUnlockされる(CBlockingQueueT用)
		size_t			bufferSize;		///< バッファの長さ
		size_t			dataSize;		///< 実際のデータの長さ
		INDEX			readIndex;		///< 読込みポジション
		TYP*			pBuffer;		///< バッファ
		/// コンストラクタ
		THead(size_t size) : readIndex(0), bufferSize(size), dataSize(0)
		{
			pBuffer = new TYP[size];
		}
		/// デストラクタ
		~THead(void)
		{
			if ( pBuffer != NULL )
			{
				delete[] pBuffer; 
			}
		}
	};

	CPointerHandleT<THead>	m_hptHead;			///< 管理用ヘッダのポインタハンドル

private:

	CSyncSection			m_syncFunc;			///< 参照排他

	/** 他とバッファを共有していたら分離する */
	void m_Separate(void)
	{
		if ( m_hptHead.GetReferCount() > 1 )
		{
			size_t size = m_hptHead->bufferSize;
			THead* pNew = new THead(size);
			if ( (m_hptHead->dataSize) > 0 )
			{
				pNew->dataSize	= m_hptHead->dataSize;
				pNew->readIndex	= m_hptHead->readIndex;
				TYP* P1 = pNew->pBuffer;
				TYP* P2 = m_hptHead->pBuffer;
				while ( (size--) > 0 )
				{
					*P1++ = *P2++;
				}
			}
			m_hptHead = pNew;
		}
	}

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const
	{
		if ( IsInRange(index) )
		{
			INDEX tmp;
			THead* H = *m_hptHead.ReferP();
			tmp = (H->readIndex) + index;
			tmp %= (H->bufferSize);
			return &(H->pBuffer[tmp]);
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
		if ( IsInRange(index) )
		{
			m_Separate();
			INDEX tmp;
			THead* H = *m_hptHead.ReferP();
			tmp = (H->readIndex) + index;
			tmp %= (H->bufferSize);
			return &(H->pBuffer[tmp]);
		}
		return NULL;
	}

	friend class CQueueTest;///< フレンドクラス宣言
};



/**@ingroup COLLECT ITERATOR
 * ブロック付キュー型情報管理テンプレート
 *
 *		指定サイズを超えての追加や、空の時の取得時にブロックされるキューです。
 *		他は、CRingQueueT と同じです。template のパラメータもそちらを参照してください。
 *	
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	IQueueT を実装しています。
 *
 *	@par必要ファイル
 *			TnbQueue.h
 * 
 *	@date 06/04/25 新規作成
 *	@date 06/05/15 Cloneを廃止。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP, bool ISI = false, class INV = TYP>
class CBlockingQueueT : public CRingQueueT<TYP, ISI, INV>
{
	typedef CRingQueueT<TYP, ISI, INV> _super;	///< 親クラス名再宣言
	DEFSUPER(_super);
public:

	/**
	 * コンストラクタ
	 *	@note	コンストラクタ時に、リングキューの最大サイズを指定します。
	 *			動的に大きくなりません。
	 *	@param size リングキューのサイズ。省略すると10000です。
	 */
	explicit CBlockingQueueT(size_t size = 10000) : _super(size)
	{
	}

	/**
	 * デストラクタ
	 */
	virtual ~CBlockingQueueT(void)
	{
	}

	/**
	 * [取得] 複数要素取り出し.
	 *		指定バッファにコピーし、キューから削除します。
	 *	@note 指定サイズ取り出せない場合、入るまで待ちます。
	 *	@param[in]	size	取り出す数。
	 *	@param[out]	P		取り出すバッファ。 NULLなら取得しません。
	 *	@return 取り出し数。
	 */
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		if ( _super::TakeElements(size, P) != size )
		{
			//なかった
			while ( true )
			{
				m_hptHead->syncAdding.Lock();
				if ( _super::TakeElements(size, P) == size )
				{
					break;
				}
			}
		}
		m_hptHead->syncTaking.Pulse();
		return size;
	}

	/**
	 * [追加] 複数要素追加.
	 *		連続したデータをキューの最後に追加する。
	 *	@note	一杯の場合、空くまで待ちます。
	 *	@param size 追加するデータの数。省略すると TYP()が代入されます。
	 *	@param P 追加するデータ列のポインタ
	 *	@return 追加した数 (常に成功します) 
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( _super::AddElements(size, P) != size )
		{
			//追加できなかった
			loop ( i, size )
			{
				while ( true )
				{
					if ( _super::AddElements(1, P) == 1 )
					{
						break;
					}
					::Sleep(10);
				}
				m_hptHead->syncAdding.Pulse();
				P++;
			}
		}
		m_hptHead->syncAdding.Pulse();
		return size;
	}
};



/**@ingroup COLLECT
 * BYTE型(unsigned 8bit)キュークラス
 *	@par必要ファイル
 *			TnbQueue.h
 */
typedef TNB::CRingQueueT<BYTE> CByteQueue;



/**@ingroup COLLECT
 * BYTE型(unsigned 8bit)ブロッキングキュークラス
 *	@par必要ファイル
 *			TnbQueue.h
 */
typedef TNB::CBlockingQueueT<BYTE> CByteBlockingQueue;



/**@ingroup COLLECT
 * DWORD型(unsigned 32bit)キュー管理クラス
 *	@par必要ファイル
 *			TnbQueue.h
 */
typedef TNB::CRingQueueT<DWORD> CDwordQueue;



/**@ingroup COLLECT
 * CStr型キュー管理クラス
 *	@par必要ファイル
 *			TnbQueue.h
 */
typedef TNB::CRingQueueT<CStr,true> CStrQueue;



/**@ingroup COLLECT
 * CStr型ブロッキングキュー管理クラス
 *	@par必要ファイル
 *			TnbQueue.h
 */
typedef TNB::CBlockingQueueT<CStr,true> CStrBlockingQueue;



#ifdef __AFX_H__

	/**@ingroup COLLECT
	 * CString型キュー管理クラス
	 *	@note MFCプロジェクトの時のみ使用可能です。
	 *	@par必要ファイル
	 *			TnbQueue.h
	 */
	typedef TNB::CRingQueueT<CString,true> CStringQueue;

#endif



}; // TNB
