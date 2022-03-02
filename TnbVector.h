#pragma once
/**
 *@file
 * 配列型情報管理関係のヘッダ
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbException.h"
#include "TnbCollection.h"
#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * 配列型情報管理テンプレート
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *	
 *	@note 関数の戻り型として使用するときに便利な、「無効状態」を管理できます
 *			( Invalid() , IsValid() )。
 *
 *	@note	begin() / end() によるイテレータは、単純に要素のポインタになっています。
 *			そのため、高速にアクセスが可能です。ただし、スレッドセーブでは有りませんので、
 *			スレッドで使用している場合、Lockするようにしてください。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	ISequenceCollectionT を実装しています。
 *
 *	@note スレッドセーブ、コピー機能を省いた簡易的な CSimpleVectorT もあります。
 *
 *	@par必要ファイル
 *			TnbVector.h
 *
 *	@date 06/01/01 旧CDataBlockを抽象化。
 *	@date 06/04/19 ICollectionT を実装するように変更。
 *	@date 06/04/21 コンストラクト時に追加サイズ指定可能にした。
 *	@date 06/04/22 CVectorHandleT を廃止。Clone() メソッド、追加。
 *	@date 06/05/01 ISequenceCollectionT を実装するように変更。
 *	@date 06/05/15 Cloneを廃止。
 *	@date 06/05/23 AddElementsなど、IFに移行したメソッドの削除。
 *	@date 06/05/30 Insert() メソッド、追加。
 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
 *	@date 06/08/13 イテレータを STL にあわせて、改修。
 *	@date 06/08/25 SetSize の最適化。
 *	@date 06/09/13 CopyElements() メソッド、Vector専用に追加。
 *	@date 06/10/26 最適化。
 *	@date 06/12/06 Remove()に削除数指定のメソッド、追加。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 08/01/09 Remove()の部バグ修正。
 *	@date 09/09/07 CopyElements() -> SetElements() に改名。
 *	@date 11/04/19 GetBuffer() 中のコピーをサポート。
 */
template<typename TYP>
class CVectorT : public ICollectionT<TYP> , public ISequenceCollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
public:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// const_iterator型宣言
	typedef const TYP* const_iterator;
	// iterator型宣言
	typedef TYP* iterator;
	#endif //_TnbDOXYEM

	/**
	 * [反復] 先頭const_iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return m_hptHead->data(0); }

	/**
	 * [反復] 最後const_iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return m_hptHead->data(GetSize()); }

	/**
	 * [反復] 先頭iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator begin(void) { m_Separate(); return m_hptHead->data(0); }

	/**
	 * [反復] 最後iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator end(void) { m_Separate(); return m_hptHead->data(GetSize()); }

	/**
	 * [反復] 挿入
	 *	@param ite 挿入する位置の iterator
	 *	@param t 要素
	 *	@return 挿入した場所の iterator
	 */
	iterator insert(iterator ite, const TYP& t = TYP())
	{
		INT_PTR index = ite - begin();
		Insert(index, t);
		return begin() + index;
	}

	/**
	 * [反復] 削除
	 *	@param ite 削除する位置の iterator
	 *	@return 削除した次の iterator
	 */
	iterator erase(iterator ite)
	{
		INT_PTR index = ite - begin();
		if ( ! Remove(index) ) { return end(); }
		return begin() + index;
	}

	/**
	 * [反復] 先頭に挿入
	 *	@param t 要素
	 */
	void push_front(const TYP& t) { Insert(0, t); }

	/**
	 * [反復] 最後に追加
	 *	@param t 要素
	 */
	void push_back(const TYP& t) { Add(t); }


	//------------------------


	/**
	 * コンストラクタ
	 *	@note 指定のメモリ確保時の追加サイズを設定します。
	 *	@note データは保持しません
	 *	@param size メモリ確保時の追加サイズ。 SetIncrementSize() と同じです。
	 */
	explicit CVectorT(size_t size = 0) : _super()
	{
		m_Init();
		SetIncrementSize(size);
	}

	/**
	 * コピーコンストラクタ
	 *	@note コピー元とバッファを共有します。
	 *	@param other コピー元の CVectorT インスタンス
	 */
	CVectorT(const CVectorT<TYP>& other) : _super()
	{
		m_Init();
		if ( other.m_hptHead.GetReferCount() < 0)
		{
			SetElements(other.GetSize(), other.ReferBuffer());
		}
		else
		{
			m_hptHead = other.m_hptHead;
		}
	}
	
	/// デストラクタ
	virtual ~CVectorT(void)
	{
		m_hptHead.Null();
	}

	/**
	 * [代入] 代入
	 *	@note 自インスタンスで管理しているバッファを開放し、	
	 *		代入元の情報を共有するようになります。
	 *	@param other 代入する CVectorT 
	 *	@return 自分の参照
	 */
	CVectorT& operator=(const CVectorT& other)
	{
		EXCLUSIVE(&other);
		if ( other.m_hptHead.GetReferCount() < 0)
		{
			SetElements(other.GetSize(), other.ReferBuffer());
		}
		else
		{
			m_hptHead = other.m_hptHead;
		}
		return *this;
	}

	/**
	 * [取得] 要素の取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 GetEx() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の複製
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP Get(INDEX index) const
	{
		return * m_hptHead->datack(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		return * m_hptHead->datack(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP& Ref(INDEX index)
	{
		TYP* P = m_GetPtr(index);
		if ( P == NULL )
		{
			throw CIndexOutOfBoundsException();
		}
		return *P;
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
		TYP* P = m_GetPtr(index);
		if ( P == NULL )
		{
			return false; 
		}
		*P = t;
		return true;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& operator[](INDEX index) const
	{
		return At(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。操作するとオブジェクト内の要素に影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP& operator[](INDEX index)
	{
		TYP* P = m_GetPtr(index);
		if ( P != NULL ){ return *P; }
		throw CIndexOutOfBoundsException();
	}
	
	/**
	 * [追加] 追加オペレータ
	 *	@note 自インスタンスで管理しているデータに指定のデータを
	 *		 追加します。
	 *	@param vector 追加するCVectorT
	 *	@return 自分の参照
	 */
	CVectorT& operator+=(const CVectorT<TYP>& vector)
	{
		AddElements(vector.GetSize(), vector.ReferBuffer());
		return *this;
	}

	/**
	 * [作成] 連結
	 *	@note	自インスタンスで管理しているデータに指定のデータを
	 *			連結した、 CVectorT を作成します。
	 *			自インスタンスは変化しません。
	 * @param other 連結する CVectorT
	 * @return 新規インスタンス
	 */
	CVectorT<TYP> operator+(const CVectorT<TYP>& other) const
	{
		CVectorT v = *this;
		v += other;
		return v;
	}

	/**
	 * [設定] 余白サイズ
	 *	@note 新規にメモリを確保したとき、必要なサイズよりいくつ多く
	 *		確保しておくか、という値を設定できます。
	 *	@note 通常は自動です。
	 *	@note コピーではこの値は引き継がれません。
	 *	@param size メモリ確保時の追加サイズ。マイナスなら自動です。
	 */
	void SetIncrementSize(size_t size)
	{
		m_incrementSize = size;
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
	 * [取得] サイズ取得
	 *	@return 本インスタンスが管理しているデータの長さ。
	 */
	virtual size_t GetSize(void) const
	{
		ASSERTLIB(! m_hptHead.IsNull());
		return m_hptHead->dataSize;
	}

	/**
	 * [追加] 要素一つ追加.
	 *		自インスタンスで管理しているバッファに指定の一データを連結します。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 要素。
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
		return RemoveElements(index, 1) == 1;
	}

	/**
	 * [削除] 要素削除.
	 *	@param index インデックス
	 *	@param size 削除数。省略すると０。０を指定するとそれ以降全部削除します。
	 *	@retval INVALID_SIZE	エラー。
	 *	@return それ以外		削除した数。
	 */
	virtual size_t RemoveElements(INDEX index, size_t size = 0)
	{
		EXCLUSIVE(this);
		if ( IsInRange(index) )
		{
			m_Separate();
			size_t nowSize = GetSize();
			TYP* P = m_hptHead->pBuffer;
			//削除サイズ調整
			if ( size == 0 || nowSize - index <= size )
			{
				if ( index == 0 )
				{
					return RemoveAll() ? nowSize : INVALID_SIZE;
				}
				size = nowSize - index;
			}
			//削除範囲の後の移動が必要な数
			if ( nowSize > index + size )
			{
				loop ( i, nowSize - (index + size) )
				{
					ASSERTLIB( index + i + size < nowSize );
					P[index + i] = P[index + i + size];
				}
			}
			//いらない部分を消す
			size_t s = m_hptHead->dataSize - size;
			loop ( i, size )
			{
				P[i + s] = TYP();
			}
			//
			m_hptHead->dataSize -= size;
			return size;
		}
		return INVALID_SIZE;
	}

	/**
	 * [追加] 複数要素追加.
	 *		最後に追加します。
	 *	@param size 追加するデータ長。
	 *	@param P 追加するデータ。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@return それ以外		追加した数。
	 */
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( size == 0 )
		{
		}
		else if ( m_hptHead.IsNull() )
		{
			ASSERTLIB(false);
			//=== 自分は情報を持っていない
			THead* lpTmp = m_CreateHead(size);
			m_CopyBuffer(lpTmp->pBuffer, P, size);
			m_hptHead = lpTmp;
		}
		else
		{
			EXCLUSIVE(this);
			m_hptHead->syncObj.Lock();
			THead* ptHead = m_hptHead;
			if ( m_hptHead.GetReferCount() > 1 )
			{
				size_t l = ptHead->dataSize + size;
				THead* lpTmp = m_CreateHead(l);
				m_CopyBuffer(lpTmp->pBuffer, ptHead->pBuffer, ptHead->dataSize);
				m_CopyBuffer(lpTmp->data(ptHead->dataSize), P, size);
				m_hptHead->syncObj.Unlock();
				m_hptHead = lpTmp;
			}
			else
			{
				m_hptHead->syncObj.Unlock();
				size_t l = ptHead->dataSize + size;
				//二つを足したバッファサイズを確保してある？
				if ( ptHead->bufferSize >= l )
				{
					//== 足りる
					m_CopyBuffer(ptHead->data(ptHead->dataSize), P, size);
					ptHead->dataSize = l;
				}
				else
				{
					//== 足りない
					//新たにメモリ確保
					size_t ss = m_GetSecureSize(l);
					TYP* N = new TYP[ss];
					//データを代入
					m_CopyBuffer(&N[0], ptHead->pBuffer, ptHead->dataSize);
					m_CopyBuffer(&N[ptHead->dataSize], P, size);
					//現在のバッファを開放
					if ( ptHead->pBuffer != NULL )
					{
						delete[] ptHead->pBuffer;
					}
					//新しくバッファアドレスを指定
					ptHead->bufferSize = ss;
					ptHead->dataSize = l;
					ptHead->pBuffer = N;
				}
			}
		}
		return size;
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
		CVectorT v;
		size_t r = v.AddElements(size, P);
		if ( r >= 0 )
		{
			m_hptHead = v.m_hptHead;
		}
		return r;
	}
	
	/**
	 * [追加] 追加.
	 *		指定のデータを追加します。
	 *	@note 追加元データの範囲を限定したい場合、元データに CConstOffsetAdapterT を使用します。
	 *	@param c 追加するデータ
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	virtual size_t Append(const IConstCollectionT<TYP>& c)
	{
		EXCLUSIVE2(&c, this);
		size_t as = c.GetSize();
		size_t j = GetSize();
		SetSize(j + as);
		TYP* P = m_hptHead->data(j);
		loop ( i, as )
		{
			*P++ = c.At(i);
		}
		return as;
	}

	/**
	 * [削除] 空化
	 *	@note 自インスタンスで管理しているバッファを空にします。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool RemoveAll(void)
	{
		m_Init();
		return true;
	}

	/**
	 * [追加] 要素一つ挿入
	 *	@note 指定のインデックスに要素を追加します。
	 *	@param index インデックス
	 *	@param t 要素
	 *	@retval true	挿入成功
	 *	@retval false	失敗（インデックス無効）
	 */
	virtual bool Insert(INDEX index, const TYP& t)
	{
		EXCLUSIVE(this);
		if ( ! TNB::IsInRange(index, GetSize() + 1) )
		{
			return false;
		}
		if ( Add(TYP()) == INVALID_INDEX )
		{
			return false;
		}
		size_t size = GetSize() - 1;
		TYP* P = m_hptHead->pBuffer;
		for ( size_t i = size; i > index; i-- )
		{
			P[i] = P[i - 1];
		}
		P[index] = t;
		return true;
	}

	/**
	 * [操作] 無効状態にする
	 *	@note 状態としてはRemoveAllと同じ。
	 */
	void Invalid(void)
	{
		RemoveAll();
		m_hptHead->boValid = false;
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
		if ( size != INVALID_SIZE )
		{
			EXCLUSIVE(this);
			if ( IsEmpty() )
			{
				//バッファなし
				SetElements(size);
			}
			else if ( size == 0 )
			{
				m_Init();
			}
			else
			{
				size_t now = m_hptHead->dataSize;
				if ( now > size )
				{
					//今より小さく
					m_Separate();
					TYP* P = m_hptHead->data(size);
					for ( size_t i = size; i < now; i++ )
					{
						*P++ = TYP();
					}
					m_hptHead->dataSize = size;
				}
				else if ( now < size )
				{
					//今より大きく
					AddElements(size - now);
				}
			}
		}
		return true;
	}

	/**
	 * [取得] データアドレス取得
	 *	@note	このアドレスの内容は書き換えてはいけません。書き換えるための
	 *			アドレスは GetBuffer() を使用して取得してください。
	 *	@note	取得したアドレスは delete で開放してはいけません。
	 *	@note	取得したアドレスは本クラスの計算等、操作すると無効になります。
	 *	@return アドレス。確保していない場合、NULLが返ります。
	 */
	virtual const TYP* ReferBuffer(void) const
	{
		ASSERTLIB(! m_hptHead.IsNull());
		return m_hptHead->pBuffer;
	}

	/**
	 * [取得] 参照数取得
	 *	@note 管理しているデータが他から参照されているか調べることが出来ます。
	 *	@return 参照数
	 */
	int GetReferCount(void)
	{
		return m_hptHead.GetReferCount();
	}

	/**
	 * [確認] 有効チェック
	 *	@note Invalid()で無効にされたとき、false になります。
	 *		無効はデータなし状態と同じです。
	 *	@retval true 有効（データがEmptyの可能性もあり）
	 *	@retval false 無効（データはEmpty）
	 */
	bool IsValid(void) const
	{
		if ( IsEmpty() && ! m_hptHead->boValid )
		{
			return false;
		}
		return true;
	}

	/**
	 * [作成] 切り出し
	 *	@note	自インスタンスで管理しているデータの中から
	 *			任意の場所のデータを取り出し、CVectorTを作成します。
	 *			自インスタンスは変化しません。
	 * @param startIndex 切り出し開始位置
	 * @param size	切り出しサイズ。省略すると、管理している最後までを切り出します。
	 * @return 新規インスタンス。
	 */
	CVectorT<TYP> Mid(INDEX startIndex, size_t size = 0) const
	{
		CVectorT v;
		EXCLUSIVE(this);
		if ( ! IsEmpty() )
		{
			if ( m_hptHead->dataSize > startIndex )
			{
				if ( size == 0 || m_hptHead->dataSize - startIndex < size )
				{
					//取得データ長まで取り出せない 最後まで取得にする
					size = m_hptHead->dataSize - startIndex;
				}
				v.SetElements(size, m_hptHead->data(startIndex));
			}	
		}
		return v;
	}

	/**
	 * [操作] データアドレス取得
	 *
	 *		この関数で取得したデータの内容は自由に書き換えてかまいません。もちろん
	 *		確保したサイズ以上はアクセスしてはいけません。
	 *
	 *		取得したアドレスは delete で開放してはいけません。
	 *
	 *		データの書き換えが終わったら ReleaseBuffer() を実行してください。
	 *
	 *	@note	この関数を行った後でも以前と変わらぬ使い方が出来ます。ただし、複製の際
	 *			GetBuffer() 実行後 ReleaseBuffer() を実装するまで、Cloneを作成するように
	 *			なります。
	 *	@note	参照だけであるなら ReferBuffer() を使用してください。高速です。
	 *	@note	GetBuffer() 実行後、 ReleaseBuffer() するまでクラスに対する操作は
	 *			禁止です（取得したアドレスが無効になる可能性があるため）。
	 *	@note	この関数では他と共用していたメモリを独自に確保しなおしています。
	 *			他のバッファとは分離されることに注意してください。
	 *	@param size 確保するバッファサイズ。省略すると、変化せずアドレスを返します。
	 *	@return アドレス	データがない場合NULLがかえります。
	 */
	TYP* GetBuffer(size_t size = 0)
	{
		if ( IsEmpty() && size == 0 )
		{
			return NULL;
		}
		m_hptHead->syncObj.Lock();
		Lock();
		//---自クラスにバッファがある
		int iCnt = m_hptHead.GetReferCount();
		if ( iCnt < 0 )
		{
			//---もう固定バッファになってる
			m_hptHead->syncObj.Unlock();
			if ( size > 0 )
			{
				SetSize(size);
			}
		}
		else if ( iCnt == 1 )
		{
			//---このクラスでしか使ってない
			m_hptHead->syncObj.Unlock();
			if ( size > 0 )
			{
				SetSize(size);
			}
			//参照数を-１にする。
			m_hptHead.SetReferCount(-1);
		}
		else
		{
			//---他でも使われてる
			//新たに確保せねば
			if ( size == 0 )
			{
				size = m_hptHead->dataSize;
			}
			//新たにメモリ確保
			THead* lpTmp = m_CreateHead(size);
			//データを代入
			m_CopyBuffer(lpTmp->pBuffer, m_hptHead->pBuffer, size);
			//新しくバッファアドレスを指定
			m_hptHead->syncObj.Unlock();
			m_hptHead = lpTmp;
			//参照数を-１にする。
			m_hptHead.SetReferCount(-1);
		}
		Unlock();
		return m_hptHead->pBuffer;
	}

	/**
	 * [操作] データの管理を元に戻す.
	 *			GetBuffer() を実行するとバッファの内容を書き換え
	 *			ても問題ないようにバッファの管理を変更します。
	 *			本関数はその状態を元に戻す関数です。
	 *			戻さないと、コピー処理の際のオーバーヘッドが大きくなります。
	 *	@note	本関数実行後、上記の関数で取得したバッファ内容は書き換えないでください。
	 */
	void ReleaseBuffer(void)
	{
		if ( ! IsEmpty() )
		{
			m_hptHead->syncObj.Lock();
			//---自クラスにバッファがある
			if ( m_hptHead.GetReferCount() < 0 )
			{
				//---固定バッファになってる
				//参照1の普通のバッファになる
				m_hptHead.SetReferCount(1);
			}
			m_hptHead->syncObj.Unlock();
		}
	}

	/**
	 * [作成] Vector作成
	 *	@note	指定コレクションのデータを CVectorT に変換します。
	 *			指定コレクションは変化しません。
	 *	@param c	変換する IConstCollectionT を持つインスタンス
	 *	@param size 要素サイズ。省略すると最後までになります。
	 *	@return 新規 CVectorT<TYP>
	 */
	inline friend CVectorT<TYP> ToVector(const IConstCollectionT<TYP>& c, size_t size = 0)
	{
		EXCLUSIVE(&c);
		size_t l = c.GetSize();
		if ( size == 0 || size > l )
		{
			size = l; 
		}
		CVectorT vb;
		vb.SetSize(size);
		TYP* P = vb.m_hptHead->data();
		loop ( i, size )
		{
			*P++ = c.At(i);
		}
		return vb;
	}

private:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	/// 配列型情報管理の管理型
	struct THead
	{
		CSyncSection syncObj;	///< 参照排他
		size_t	bufferSize;		///< バッファの長さ
		size_t	dataSize;		///< 実際のデータの長さ
		bool	boValid;		///< 有効状態ならtrue、無効状態ならfalse
		TYP *	pBuffer;		///< バッファ

		/// コンストラクタ
		THead(void) : bufferSize(0) , dataSize(0) , boValid(true) , pBuffer(NULL)	{}

		/// デストラクタ
		virtual ~THead(void)
		{
			if ( pBuffer != NULL )
			{
				delete[] pBuffer;
				pBuffer = NULL;
			} 
		}

		/**
		 * バッファアドレス取得
		 *	@param f オフセット
		 *	@return バッファアドレス
		 */
		TYP* data(INDEX f = 0) const
		{
			return pBuffer + f; 
		}

		/**
		 * チェック付きバッファアドレス取得
		 *	@param f オフセット
		 *	@return バッファアドレス
		 */
		TYP* datack(INDEX f = 0) const
		{
			if ( TNB::IsInRange(f, dataSize) )
			{
				return pBuffer + f; 
			}
			throw CIndexOutOfBoundsException();
		}
	};
	#endif

	CPointerHandleT<THead>	m_hptHead;			///< 管理用ヘッダのポインタハンドル
	CSyncSection			m_syncFunc;			///< 参照排他
	size_t					m_incrementSize;	///< 要素スロットの最小数	

	/**
	 * 確保すべきサイズ取得
	 *	@param	size 必要サイズ
	 *	@return	確保サイズ
	 */
	size_t m_GetSecureSize(size_t size)
	{
		//追加サイズ計算
		size_t marginSize = m_incrementSize;
		if ( marginSize == 0 )
		{
			//自動になっている
			marginSize = size / 8;
			marginSize = (marginSize < 4) ? 4 : ((marginSize > 1024) ? 1024 : marginSize);
		}
		return size + marginSize;
	}

	/**
	 * 管理用領域確保
	 *	@note 管理用の構造体を動的に確保し初期化します。
	 *	@note 必要サイズより大きめにとっておきます
	 *	@param size 必要サイズ
	 *	@return HEADのポインタ
	 */
	THead* m_CreateHead(size_t size)
	{
		THead* lptTmpHead = new THead;
		//長さを入れる
		size_t bs = m_GetSecureSize(size);
		lptTmpHead->dataSize	= size;
		lptTmpHead->bufferSize	= bs;
		//バッファを確保する
		lptTmpHead->pBuffer = new TYP[bs];
		return lptTmpHead;
	}

	/**
	 * バッファコピー
	 *	@note lpSrcがNULLならクリア処理になります
	 *	@param[out] _lpDst	コピー先ポインタ。
	 *	@param[in]	lpSrc	コピー元ポインタ。NULLの場合、TYP()を代入。
	 *	@param[in]	iLen	コピー長さ。
	 */
	void m_CopyBuffer(TYP* _lpDst, const TYP* lpSrc, size_t iLen)
	{
		ASSERTLIB(_lpDst != NULL);
		if ( lpSrc == NULL )
		{
			TYP t = TYP();
			while ( iLen-- )
			{
				*_lpDst++ = t; 
			}
		}
		else
		{
			ASSERTLIB(! ::IsBadReadPtr(lpSrc, iLen));
			while ( iLen-- )
			{
				*_lpDst++ = *lpSrc++;
			}
		}
	}

	/**
	 * メンバ変数の初期化
	 *	@note コンストラクタ用、メンバ変数の初期化
	 */
	void m_Init(void)
	{
		m_hptHead = new THead;
		m_incrementSize = 0;
	}

	/** 他とバッファを共有していたら分離する */
	void m_Separate(void)
	{
		m_hptHead->syncObj.Lock();
		if ( m_hptHead.GetReferCount() > 1 )
		{
			size_t size = m_hptHead->dataSize;
			//新たにメモリ確保
			THead* lpTmp = m_CreateHead(size);
			//データを代入
			m_CopyBuffer(lpTmp->pBuffer, m_hptHead->pBuffer, size);
			//新しくバッファアドレスを指定
			m_hptHead->syncObj.Unlock();
			m_hptHead = lpTmp;
		}
		else
		{
			m_hptHead->syncObj.Unlock();
		}
	}

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	TYP* m_GetPtr(INDEX index)
	{
		if ( TNB::IsInRange(index, m_hptHead->dataSize) )
		{
			m_Separate();
			return m_hptHead->data(index);
		}
		return NULL;
	}

	friend class CVectorTest;	///<フレンドクラス宣言
};



/**@ingroup COLLECT ITERATOR
 * 自動拡張列型情報管理テンプレート
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		CVectorT とほぼ同じですが、 Set() 、 At() などで、自動的に配列を拡張します。<BR>
 *		イテレータ操作中は拡張されません。
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	ISequenceCollectionT を実装しています。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@par必要ファイル
 *			TnbVector.h
 *
 *	@date 06/05/09　新規
 *	@date 07/03/15 index,size管理を unsignedに変更。
*/
template<typename TYP>
class CAutoVectorT : public CVectorT<TYP>
{
	DEFSUPER(CVectorT<TYP>);
public:

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
		if ( GetSize() <= index )
		{
			SetSize(index + 1);
		}
		return _super::Set(index, t);
	}

	/**
	 * [追加] 要素一つ挿入
	 *	@note 指定のインデックスに要素を追加します。
	 *	@param index インデックス
	 *	@param t 要素
	 *	@retval true	挿入成功
	 *	@retval false	失敗（インデックス無効）
	 */
	virtual bool Insert(INDEX index, const TYP& t)
	{
		if ( GetSize() < index )
		{
			SetSize(index);
		}
		return _super::Insert(index, t);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。操作するとオブジェクト内の要素に影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& operator[](INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		return _super::operator[](index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。操作するとオブジェクト内の要素に影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP& operator[](INDEX index)
	{
		if ( GetSize() <= index )
		{
			SetSize(index + 1);
		}
		return _super::operator[](index);
	}
};



/**@ingroup COLLECT
 * BYTE配列管理クラス
 *	@note 推奨prefixは「vb」です。
 *	@par必要ファイル
 *			TnbVector.h
 */
typedef TNB::CVectorT< BYTE > CByteVector;



/**@ingroup COLLECT
 * WORD配列管理クラス
 *	@note 推奨prefixは「vw」です。
 *	@par必要ファイル
 *			TnbVector.h
 */
typedef TNB::CVectorT< WORD > CWordVector;



/**@ingroup COLLECT
 * DWORD配列管理クラス
 *	@note 推奨prefixは「vdw」です。
 *	@par必要ファイル
 *			TnbVector.h
 */
typedef TNB::CVectorT< DWORD > CDwordVector;



#ifdef __AFX_H__

	/**@ingroup COLLECT
	 * [複製] CByteArrayから CByteVector へコピー
	 *	@note MFCプロジェクトの時のみ使用可能です。
	 *	@par必要ファイル
	 *			TnbVector.h
	 *	@param ba コピー元の CByteArray
	 *	@return CByteVector
	 */
	inline CByteVector ToByteVector(const CByteArray &ba)
	{
		CByteVector v;
		v.SetElements(static_cast<size_t>(ba.GetSize()), ba.GetData());
		return v;
	}

	/**@ingroup COLLECT
	 * [複製] CByteVector からCByteArrayへコピー
	 *	@note MFCプロジェクトの時のみ使用可能です。
	 *	@par必要ファイル
	 *			TnbVector.h
	 *	@param ba コピー先の CByteArray
	 *	@param dat コピー元の CByteVector
	 */
	inline void Copy(CByteArray &ba, const CByteVector &dat)
	{
		ba.SetSize(dat.GetSize());
		MemCopy(ba.GetData(), dat.ReferBuffer(), dat.GetSize());
	}

#endif



}; // TNB

