#pragma once
/**
 *@file
 * リスト型情報管理関係のヘッダ
 *
 *		型の配列を管理します。
 *		配列の個々をそれぞれメモリを確保しリンクし配列としてるため、容易に
 *		配列数を増やすことができます（メモリの再配置は発生しない)。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"
#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * リスト型情報管理テンプレート
 *
 *		型の配列を管理します。
 *		配列の個々をそれぞれメモリを確保しリンクし配列としてるため、容易に
 *		配列数を増やすことができます（メモリの再配置は発生しない)。
 *
 *	@note	各要素に属性値（任意の型）を一つにつけることが出来ます。
 *			この値は、 Append() や Copy() を使って追加、複製すると、 ATT() になります。
 *			List 同士の連結、複製は、オペレータを使ってください。
 *
 *	@note	Listコレクションのシリアライズ、デシリアライズは、必ず本リストコレクション
 *			で行ってください。属性情報があるため、他のコレクションと互換が有りません。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *	@param ATT TYPと対に保持する属性型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbList.h
 *
 *	@date 06/04/24 新規作成
 *	@date 06/04/28 SetSize()追加。Elementによるサイズ拡張に対応。
 *	@date 06/05/03 Elementによるサイズ拡張を廃止。
 *	@date 06/05/15 Cloneを廃止。
 *	@date 06/05/29 各要素にパラメータをつけれられるようにした。
 *	@date 06/08/01 GetConstPointerに依存しないつくりに変更。
 *	@date 06/08/30 シリアライズ機能、追加。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 07/06/19 パラメータ→属性とし、テンプレート化。 RemoveOnCheck() を Cull() に改名。
 */
template<typename TYP, typename ATT = DWORD>
class CListT : public ICollectionMidT<TYP>
{
	DEFSUPER(ICollectionMidT<TYP>);	

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	/// LISTの要素一つ
	struct TOne
	{
		TOne*	pNextOne;	///< 次のパーツ
		ATT		attribute;	///< 属性値
		TYP		value;		///< 要素情報
		/// コンストラクタ
		TOne(TYP t) : value(t) , attribute(ATT()) , pNextOne(NULL) 
		{
		}
		/** 
		 * Nextを削除
		 *	@note Nextの参照先以下、すべてDeleteされます。
		 */
		void Delete(void)
		{
			if ( pNextOne != NULL )
			{
				pNextOne->Delete();
				delete pNextOne;
				pNextOne = NULL;
			}
		}
	};

	/** 
	 * 要素保持型
	 *	@note CListTで持つ型です。この下に要素が複数ぶら下がります。
	 *	@note value は未使用です。
	 */
	struct TTop : public TOne, CCopyImpossible
	{
		CSyncSection	m_syncObj;		///< 排他オブジェクト
		size_t			size;			///< パーツのぶら下がり数
		TOne*			pLastOne;		///< 最後のOneの位置記憶（キャッシュ的な使用法）
		/// コンストラクタ
		TTop(void) : TOne(TYP()) , size(0) , pLastOne(NULL) {}
		/// デストラクタ
		~TTop(void)
		{
			TOne::Delete();
		}
		/**
		 * 要素管理型のポインタ取得
		 *	@param index インデックス
		 *	@retval NULL	インデックスは無効
		 *	@retval NULL以外	TOneのポインタ
		 */
		TOne* GetPointer(INDEX index) const
		{
			const TOne* P = this;
			for ( INDEX i = index + 1; i > 0; i-- )
			{
				if ( P->pNextOne == NULL )
				{
					P = NULL;
					break;
				}
				P = P->pNextOne;
			}
			return const_cast<TOne*>(P);
		}

		/**
		 * 要素管理の最後のポインタ取得
		 *	@return NULL以外	TOneのポインタ
		 */
		TOne* GetLastPointer(void)
		{
			#ifndef _DEBUG
				if ( pLastOne != NULL ){ return pLastOne; } 
			#endif
			TOne* P = this;
			while ( true )
			{
				if ( P->pNextOne == NULL )
				{
					break;
				}
				P = P->pNextOne;
			}
			ASSERTLIB(pLastOne == NULL || pLastOne == P);
			pLastOne = P; 
			return P;
		}
	};
	#endif

	CPointerHandleT<TTop> m_hpTop;	///< 要素保持のポインタハンドル
	CSyncSection		m_syncFunc;	///< 排他オブジェクト

	/** 他とバッファを共有していたら分離する */
	void m_Separate(void)
	{
		m_hpTop->m_syncObj.Lock();
		if ( m_hpTop.GetReferCount() > 1 )
		{
			//新たにメモリ確保
			TTop* P = new TTop;
			TOne* pNew = P;
			//複製
			TOne* pOld = m_hpTop->pNextOne;
			loop ( i, GetSize() )
			{
				ASSERTLIB(pOld != NULL);
				pNew->pNextOne = new TOne(pOld->value);
				pNew->pNextOne->attribute = pOld->attribute;
				P->size++;
				P->pLastOne = pNew->pNextOne;
				//
				pOld = pOld->pNextOne;
				pNew = pNew->pNextOne;
			}
			//新しくバッファアドレスを指定
			m_hpTop->m_syncObj.Unlock();
			m_hpTop = P;
		}
		else
		{
			m_hpTop->m_syncObj.Unlock();
		}
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
		const TOne* P = m_hpTop->GetPointer(index);
		return (P == NULL) ? NULL : &(P->value);
	}

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		m_Separate();
		TOne* P = m_hpTop->GetPointer(index);
		return (P == NULL) ? NULL : &(P->value);
	}

public:

	/**
	 * [反復] 挿入
	 *	@param ite 挿入する位置の iterator
	 *	@param t 要素
	 *	@return 挿入した場所の iterator
	 */
	iterator insert(iterator ite, const TYP& t = TYP())
	{
		INDEX index = ite.GetIndex();
		Insert(index, t);
		return begin() + index;
	}

	/**
	 * [反復] 先頭に挿入
	 *	@param t 要素
	 */
	void push_front(const TYP& t) 
	{
		Insert(0, t); 
	}


	//------------------------

	
	/// コンストラクタ
	CListT(void) : _super(), m_hpTop(new TTop) 
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CListT(const CListT& other) : _super(), m_hpTop(other.m_hpTop) 
	{
	}

	/// デストラクタ
	virtual ~CListT(void)
	{
		m_hpTop.Null();
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CListT& operator=(const CListT& other)
	{
		m_hpTop = other.m_hpTop;
		return *this;
	}

	/**
	 * [追加] 追加オペレータ.
	 *		自インスタンスで管理しているデータに指定のデータを
	 *		追加します。
	 *	@param list 追加データ
	 *	@return 自分の参照
	 */
	CListT& operator+=(const CListT& list)
	{
		EXCLUSIVE2(list, *this);
		TOne* P = list.GetPointer(0);
		loop ( i, list.m_hpTop->GetSize() )
		{
			ASSERTLIB( P != NULL );
			VERIFYLIB( Add(P->value, P->attribute) != INVALID_INDEX );
			P = P->pNextOne;
		}
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

	/// [排他] アンロック
	virtual void Unlock(void) const
	{
		m_syncFunc.Unlock();
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_hpTop->size;
	}

	/**
	 * [追加] 要素一つ追加
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 要素。省略可能。
	 *	@return 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t)
	{
		return Add(t, ATT());
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
		m_Separate();
		m_hpTop->m_syncObj.Lock();
		TOne* P = m_hpTop->GetPointer(index - 1);
		if ( P == NULL )
		{
			m_hpTop->m_syncObj.Unlock();
			return false;
		}
		TOne* pTmp = P->pNextOne;
		if ( pTmp == NULL )
		{
			m_hpTop->m_syncObj.Unlock();
			return false;
		}
		P->pNextOne = pTmp->pNextOne;
		delete pTmp;
		m_hpTop->size--;
		m_hpTop->pLastOne = NULL;
		m_hpTop->m_syncObj.Unlock();
		return true;
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
		return Set(index, t, ATT());
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
		return SetEx(index, t, ATT());
	}

	/**
	 * [削除] 全要素削除 
	 *	@return 常にtrue
	 */
	virtual bool RemoveAll(void)
	{
		m_hpTop = new TTop;
		return true;
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
		if ( index1 == index2 )
		{
			return;
		}
		EXCLUSIVE(this);
		m_Separate();
		TOne* P1 = m_hpTop->GetPointer(index1 - 1);
		TOne* P2 = m_hpTop->GetPointer(index2 - 1);
		if ( P1 == NULL || P2 == NULL ){ throw CIndexOutOfBoundsException(); }
		TOne* P1n = P1->pNextOne;
		TOne* P2n = P2->pNextOne;
		if ( P1n == NULL || P2n == NULL ){ throw CIndexOutOfBoundsException(); }
		TOne** P1nn = &(P1n->pNextOne);
		TOne** P2nn = &(P2n->pNextOne);
		//
		P1->pNextOne = P2n;
		P2->pNextOne = P1n;
		TOne* P = *P1nn;
		*P1nn = *P2nn;
		*P2nn = P;
		m_hpTop->pLastOne = NULL;
	}

	/**
	 * [追加] 要素一つ追加
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 要素。
	 *	@param attribute 属性値。
	 *	@return 追加したINDEX。
	 */
	INDEX Add(const TYP& t, const ATT& attribute)
	{
		m_Separate();
		m_hpTop->m_syncObj.Lock();
		INDEX r = m_hpTop->size;
		TOne* P = m_hpTop->GetLastPointer();
		ASSERTLIB(P != NULL);
		P->pNextOne = new TOne(t);
		P->pNextOne->attribute = attribute;
		m_hpTop->size++;
		m_hpTop->pLastOne = P->pNextOne;
		m_hpTop->m_syncObj.Unlock();
		return r;
	}

	/**
	 * [追加] 要素一つ追加
	 *	@param t 要素。
	 *	@param attribute 属性値。
	 *	@return 追加したINDEX。
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
		m_Separate();
		TOne* P = m_hpTop->GetPointer(index);
		if ( P == NULL )
		{
			return false;
		}
		P->value = t;
		P->attribute = attribute;
		return true;
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
	const ATT& GetAttribute(INDEX index) const
	{
		EXCLUSIVE(this);
		TOne* P = m_hpTop->GetPointer(index);
		if ( P != NULL )
		{
			return P->attribute;
		}
		ASSERT1(false, "CListT::GetAttribute", "範囲外のIndex(%d)が指定されました。", index);
		throw CIndexOutOfBoundsException();
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
		m_Separate();
		if ( index == GetSize() )
		{
			return Add(t) != INVALID_INDEX;
		}
		m_hpTop->m_syncObj.Lock();
		TOne* P = m_hpTop->GetPointer(index - 1);
		if ( P == NULL )
		{
			m_hpTop->m_syncObj.Unlock();
			return false;
		}
		TOne* pNew = new TOne(t);
		pNew->attribute = attribute;
		pNew->pNextOne = P->pNextOne;
		P->pNextOne = pNew;
		m_hpTop->size++;
		m_hpTop->m_syncObj.Unlock();
		return true;
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
		if ( size == INVALID_SIZE )
		{
			return false;
		}
		EXCLUSIVE(this);
		m_hpTop->m_syncObj.Lock();
		if ( size < m_hpTop->size )
		{
			//今より小さく
			m_Separate();
			TOne* P = m_hpTop->GetPointer(size - 1);
			P->Delete();
			m_hpTop->size = size;
			m_hpTop->m_syncObj.Unlock();
		}
		else if ( size > m_hpTop->size )
		{
			//今より大きく
			INT_PTR l = size - m_hpTop->size;
			m_Separate();
			m_hpTop->m_syncObj.Unlock();
			while ( l-- )
			{
				Add(TYP());
			}
		}
		return true;
	}

	/**
	 * [削除] 間引き.
	 *		指定の条件に合わないもの(チェッカーの結果により)を削除します。
	 *	@param	checker {@link TNB::IConstCollectionT::IChecker チェッカー}
	 *	@param	boIsReverse	省略すると条件に一致を残します。
	 *						trueを指定すると条件不一致を残します。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		削除した数。
	 */
	virtual size_t Cull(const IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		size_t i = 0;
		while ( i < GetSize() )
		{
			if ( ! checker.IsValid(At(i)) ^ boIsReverse )
			{
				if ( ! Remove(i) ){ return INVALID_SIZE; }
				r++;
			}
			else
			{
				i++;
			}
		}
		return r;
	}//memo;Listは頭から削除していったほうが高速のため、特別に実装。

	#ifndef _WIN32_WCE
	/**
	 * [削除] 間引き.
	 *		指定の条件に合わない属性を持つもの(チェッカーの結果により)を削除します。
	 *	@param	checker {@link TNB::IConstCollectionT::IChecker チェッカー}
	 *	@param	boIsReverse	省略すると条件に一致を残します。
	 *						trueを指定すると条件不一致を残します。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		削除した数。
	 */
	size_t CullOnAttribute(typename const IConstCollectionT<ATT>::IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		if ( ! IsEmpty() )
		{
			for ( INDEX i = GetSize() - 1; i >= 0; i-- )
			{
				if ( ! checker.IsValid(GetAttribute(i)) ^ boIsReverse )
				{
					if ( ! Remove(i) )
					{
						return INVALID_SIZE;
					}
					r++;
				}
			}
		}
		return r;
	}
	#endif // _WIN32_WCE

	/**
	 * [処理] シリアライズ
	 *	@attention Listコレクションのシリアライズデータは、必ず、Listコレクションでデシリアライズしてください。<BR>
	 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		size_t l = GetSize();
		_sr << l << l;
		loop ( i, l )
		{
			_sr << At(i);
			_sr << GetAttribute(i);
		}
	}

	/**
	 * [処理] デシリアライズ
	 *	@attention Listコレクションのシリアライズデータは、必ず、Listコレクションでデシリアライズしてください。
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		size_t l1, l2;
		ds >> l1 >> l2;
		if ( l1 != l2 )
		{
			throw CInvalidParamException();
		}
		RemoveAll();
		TYP t;
		ATT attribute;
		loop ( i, l1 )
		{
			ds >> t;
			ds >> attribute;
			Add(t, attribute);
		}
	}
};



/**@ingroup COLLECT
 * BYTE配列管理クラス
 *	@par必要ファイル
 *			TnbList.h
 */
typedef TNB::CListT< BYTE > CByteList;




}; // TNB
