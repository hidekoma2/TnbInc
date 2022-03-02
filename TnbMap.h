#pragma once
/**
 *@file
 * マップ型情報管理関係のヘッダ
 *
 *		一意なキーに値を割り当ててコレクションすることができます。
 *		キーで値を取り出したり削除することができます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPair.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * マップ型情報管理テンプレート
 *
 *		一意なキーに値を割り当ててコレクションすることができます。
 *		キーで値を取り出したり削除することができます。
 *
 *	@note At() や Set() など、 CPairT 型で、直接アクセスすると、
 *			同Keyが2つ以上存在するMAPも出来てしまいますのでご注意ください。
 *
 *	@note	KEY の一覧を作成する EnumKeys() があります。
 *			参照だけであれば、 GetKeysAdapter() などで CConstKeysAdapter CConstValuesAdapter を
 *			作成し、アクセスすることも出来ます。
 *
 *	@attention 現在 STL の map と違い、格納時にキーによるソートしていません。
 *
 *	@param	KEY	キーとする型。クラスの場合コピー機能、比較機能( IComparableT か、引き算)が必須。
 *	@param	VAL	値とする型。クラスの場合コピー機能が必須。
 *	@param	INK	キーとする型の入力型。省略可能。
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@note ICollectionT の TYP は CPairT<KEY,VAL> になります。
 *
 *	@par必要ファイル
 *			TnbMap.h
 *
 *	@date 06/01/01	新規
 *	@date 06/04/21	ICollectionT 実装
 *	@date 06/04/27	INKパラメータ追加。キーの有無チェックメソッド、追加。
 *	@date 06/05/15	Cloneを廃止。
 *	@date 06/06/28	GetKeysAdapter() , GetValuesAdapter() 追加。
 *	@date 06/08/01	GetConstPointer/GetPointerに依存しないつくりに変更。
 *	@date 06/08/11	Element() を At() に改名。ReferElementPtr() を廃止。
 *	@date 06/09/12	SetSize()必ず失敗するようにした。
 *	@date 07/03/15	index,size管理を unsignedに変更。
 *	@date 12/05/22	Append() の処理を追加。
 *	@date 12/10/19	IsEqual() を専用に実装。
 */
template<typename KEY, typename VAL, typename INK=KEY>
class CMapT : public ICollectionT<TNB::CPairT<KEY,VAL> >
{
	typedef ICollectionT<TNB::CPairT<KEY,VAL> > _super;	///< 親クラス名再宣言
	DEFSUPER(_super);
public:

	typedef TNB::CPairT<KEY,VAL> CPair;	///< ペア型 定義

	/**@ingroup COLLECT ITERATOR
	 * マップ型情報のキーのみ参照アダプタクラス
	 *
	 *		IConstCollectionT を持つ、キーだけのコレクションとして参照する時にこのアダプタを使います。
	 *		実体は、コンストラクタで渡したものなので、本インスタンスが有効の間、実体を消さないようにします。
	 *
	 *		インデックスは実体の CMapT と同じなので、このクラスでインデックスを参照し、
	 *		実体の CMapT を操作することも可能です。
	 *
	 *	@see	IConstCollectionT を実装しています。
	 *
	 *	@note IConstCollectionT の TYP は KEY になります。
	 *
	 *	@par必要ファイル
	 *			TnbMap.h
	 *
	 *	@date 06/06/28	新規
	 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
	 *	@date 07/03/15 index,size管理を unsignedに変更。
	 */
	class CConstKeysAdapter : public IConstCollectionT<KEY>
	{
		DEFSUPER(IConstCollectionT<KEY>);
		const CMapT* m_pMap;					///< 対象のマップ
	public:
		/**
		 * コンストラクタ
		 *	@param map CMapT のインスタンス
		 */
		CConstKeysAdapter(const CMapT* map) : _super(), m_pMap(map) {}
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
			return m_pMap->Lock();
		}
		/// [排他] アンロック
		virtual void Unlock(void) const
		{
			m_pMap->Unlock();
		}
		/**
		 * [取得] 要素数取得
		 *	@return 要素数
		 */
		virtual size_t GetSize(void) const
		{
			return m_pMap->GetSize();
		}
		/**
		 * [取得] 要素の参照取得.
		 *	@note	非スレッドセーブメソッドです。
		 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
		 *	@param index インデックス
		 *	@return 参照
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 */
		virtual const KEY& At(INDEX index) const
		{
			return m_pMap->At(index).first;
		}
	};

	/**@ingroup COLLECT ITERATOR
	 * マップ型情報の値のみ参照アダプタクラス
	 *
	 *		IConstCollectionT を持つ、値だけのコレクションとして参照する時にこのアダプタを使います。
	 *		実体は、コンストラクタで渡したものなので、本インスタンスが有効の間、実体を消さないようにします。
	 *
	 *		インデックスは実体の CMapT と同じなので、このクラスでインデックスを参照し、
	 *		実体の CMapT を操作することも可能です。
	 *
	 *	@see	IConstCollectionT を実装しています。
	 *
	 *	@note IConstCollectionT の TYP は VAL になります。
	 *
	 *	@par必要ファイル
	 *			TnbMap.h
	 *
	 *	@date 06/06/28	新規
	 *	@date 06/08/01 GetConstPointer/GetPointerに依存しないつくりに変更。
	 *	@date 07/03/15 index,size管理を unsignedに変更。
	 */
	class CConstValuesAdapter : public IConstCollectionT<VAL>
	{
		DEFSUPER(IConstCollectionT<VAL>);
		const CMapT* m_pMap;				///< 対象のMAP
	public:
		/**
		 * コンストラクタ
		 *	@param map CMapT のインスタンス
		 */
		CConstValuesAdapter(const CMapT* map) : _super(), m_pMap(map) {}
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
			return m_pMap->Lock();
		}
		/// [排他] アンロック
		virtual void Unlock(void) const
		{
			m_pMap->Unlock();
		}
		/**
		 * [取得] 要素数取得
		 *	@return 要素数
		 */
		virtual size_t GetSize(void) const
		{
			return m_pMap->GetSize();
		}
		/**
		 * [取得] 要素の参照取得.
		 *	@note	非スレッドセーブメソッドです。
		 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
		 *	@param index インデックス
		 *	@return 参照
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 */
		virtual const VAL& At(INDEX index) const
		{
			return m_pMap->At(index).second;
		}
	};

private:

	CVectorT< CPair >	m_vPair;	///< CPairTの配列

	/**
	 * キーの値のポインタ取得
	 *	@param key 探すキー
	 *	@retval NULL		keyは存在しない
	 *	@retval NULL以外	VALのポインタ
	 */
	const VAL* m_SearchKey(INK key) const
	{
		const CPair* P = m_vPair.ReferBuffer();
		if ( P != NULL )
		{
			loop ( i, m_vPair.GetSize() )
			{
				if ( P[i].first == key )
				{
					return &(P[i].second);
				}
			}
		}
		return NULL;
	}

	/**
	 * キーの値のポインタ取得
	 *	@param key 探すキー
	 *	@retval NULL		keyは存在しない
	 *	@retval NULL以外	VALのポインタ
	 */
	VAL* m_SearchKey(INK key)
	{
		loop ( i, m_vPair.GetSize() )
		{
			if ( m_vPair[i].first == key )
			{
				return &(m_vPair[i].second);
			}
		}
		return NULL;
	}

public:

	/**
	 * コンストラクタ 
	 *	@note 指定のメモリ確保時の追加サイズを設定します。
	 *	@note データは保持しません
	 *	@param size メモリ確保時の追加サイズ。SetIncrementSize() と同じです。
	 */
	explicit CMapT(size_t size = 0) : _super(), m_vPair(size) 
	{
	}

	/**
	 * コピーコンストラクタ 
	 *	@param map コピー元
	 */
	CMapT(const CMapT& map) : _super(), m_vPair(map.m_vPair)
	{
	}

	/** デストラクタ */
	virtual ~CMapT(void) 
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CMapT& operator=(const CMapT& other)
	{
		m_vPair = other.m_vPair;
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
		return m_vPair.Lock();
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		m_vPair.Unlock();
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
		m_vPair.SetIncrementSize(size); 
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_vPair.GetSize();
	}

	/**
	 * [取得] キーと値を取得.
	 *		０～(GetSize()の戻り値未満) をインデックスとし、CPairT<KEY,VAL> 型で取得します。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 GetEx() を使用してください。
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual CPair Get(INDEX index) const
	{
		return m_vPair.Get(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 一時的な参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const CPair& At(INDEX index) const
	{
		return m_vPair.At(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 一時的な参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual CPair& Ref(INDEX index)
	{
		return m_vPair.Ref(index);
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
	virtual bool Set(INDEX index, const CPair& t)
	{
		return m_vPair.Set(index, t);
	}

	/**
	 * [追加] キーと値追加.
	 *		すでにキーがある場合、値のみ変更します。
	 *	@param t 要素
	 *	@return 常に０。 追加したINDEXは返りません。
	 */
	virtual INDEX Add(const CPair& t)
	{
		SetKey(t.first, t.second);
		return 0;
	}

	/**
	 * [追加] 追加.
	 *		指定のデータを追加します。
	 *	@note 同じキーがある場合、追加する c に含まれる値のほうが有効になります。
	 *	@param c 追加するデータ
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	virtual size_t Append(const IConstCollectionT<CPair>& c)
	{
		EXCLUSIVE2(&c, this);
		loop ( i, c )
		{
			RemoveKey(c.At(i).first);
		}
		return _super::Append(c);
	}

	/**
	 * [削除] キーと値一つ削除.
	 *		０～(GetSize()の戻り値未満) をインデックスとし処理します。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 RemoveEx() を使用してください。
	 *	@param index インデックス
	 *	@retval true	削除成功
	 *	@retval false	失敗
	 */
	virtual bool Remove(INDEX index)
	{
		return m_vPair.Remove(index);
	}

	/**
	 * [削除] すべてのキーと値を削除
	 *	@return 常にtrue
	 */
	virtual bool RemoveAll(void)
	{
		return m_vPair.RemoveAll();
	}

	/**
	 * [操作] サイズ指定.
	 *	@param size サイズ
	 *	@return 必ず false (失敗)。
	 */
	virtual bool SetSize(size_t size)
	{
		return false;
	}

	/**
	 * [設定] キーに対する値設定
	 *	@note キーがない場合、新規に追加します。
	 *	@param key キー
	 *	@param newValue 値
	 */
	void SetKey(INK key, const VAL& newValue)
	{
		EXCLUSIVE(this);
		operator[](key) = newValue;
	}

	/**
	 * [取得] キーに対する値の参照
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@note キーがない場合、新規に追加します。
	 *	@param key キー
	 *	@return VALの参照（操作すると、MAP内の情報も変化します）。
	 */
	VAL& operator[](INK key)
	{
		VAL* P = m_SearchKey(key);
		if ( P == NULL )
		{
			CPair t;
			t.first = key;
			m_vPair.Add(t);
			P = m_SearchKey(key);
		}
		return *P;
	}

	/**
	 * [取得] キーに対する値の参照
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@note キーがない場合、新規に追加します。
	 *	@param key キー
	 *	@return VALの参照（操作すると、MAP内の情報も変化します）。
	 *	@throw CEmptyException キーが存在しないことを示すためにスローされます。
	 */
	const VAL& operator[](INK key) const
	{
		const VAL* P = m_SearchKey(key);
		if ( P == NULL ){ throw CEmptyException(); }
		return *P;
	}

	/**
	 * [削除] キーと値を削除
	 *	@param key キー
	 *	@retval true 成功
	 *	@retval false 失敗（キーがない）
	 */
	bool RemoveKey(INK key)
	{
		EXCLUSIVE(this);
		const CPair* P = m_vPair.ReferBuffer();
		loop ( i, m_vPair.GetSize() )
		{
			if ( P[i].first == key )
			{
				m_vPair.Remove(i);
				return true;
			}
		}
		return false;
	}

	/**
	 * [取得] 値アドレス取得.
	 *		キーに対する、値のポインタを取り出します
	 *	@param key	キー
	 *	@retval NULL以外 値アドレス
	 *	@retval NULL 失敗(キーなし)
	 */
	const VAL* GetValuePtr(INK key) const
	{
		EXCLUSIVE(this);
		return m_SearchKey(key);
	}

	/**
	 * [取得] 値アドレス取得.
	 *		キーに対する、値のポインタを取り出します
	 *	@param key	キー
	 *	@retval NULL以外 値アドレス
	 *	@retval NULL 失敗(キーなし)
	 */
	VAL* GetValuePtr(INK key)
	{
		EXCLUSIVE(this);
		return m_SearchKey(key);
	}

	/**
	 * [取得] キーと値を取得.
	 *		０～(GetSize()の戻り値未満) をインデックスとし、キーと値を取り出します
	 *	@param[in]	index	要素番号
	 *	@param[out] _rKey	キー
	 *	@param[out] _rValue	値
	 *	@retval true 取得成功
	 *	@retval false 失敗（インデックスが範囲外）
	 */
	bool Get(INDEX index, KEY& _rKey, VAL& _rValue) const
	{
		EXCLUSIVE(this);
		if ( ! TNB::IsInRange(index, m_vPair.GetSize()) )
		{
			return false;
		}
		const CPair* P = m_vPair.ReferBuffer();
		_rKey = P[index].first;
		_rValue = P[index].second;
		return true;
	}

	/**
	 * [取得] キーに対する値取得
	 *	@param[in] key キー
	 *	@param[out] _rValue 値
	 *	@retval true 値取得成功
	 *	@retval false 失敗（キーがない）
	 */
	bool Lookup(INK key, VAL& _rValue) const
	{
		EXCLUSIVE(this);
		const VAL* P = m_SearchKey(key);
		if ( P == NULL ){ return false; }
		_rValue = *P;
		return true;
	}

	/**
	 * [確認] キー有無
	 *	@param key キー
	 *	@retval true 存在する
	 *	@retval false 存在しない
	 */
	bool HasKey(INK key) const
	{
		EXCLUSIVE(this);
		return m_SearchKey(key) != NULL;
	}

	/**
	 * [取得] キーの一覧
	 *	@return キーのVector
	 */
	CVectorT<KEY> EnumKeys(void) const
	{
		EXCLUSIVE(this);
		CVectorT<KEY> v;
		v.Copy(GetKeysAdapter());
		return v;
	}
	
	/**
	 * [確認] 比較.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、全要素を比較します。
	 *	@param c 比較対象
	 *	@retval true	自分は c と同じ
	 *	@retval false	自分は c と異なる
	 */
	virtual bool IsEqual(const IConstCollectionT<TNB::CPairT<KEY,VAL> >& c) const
	{
		if ( this == &c )
		{ 
			return true; 
		}
		EXCLUSIVE2(&c, this);
		size_t size = GetSize();
		if ( size != c.GetSize() )
		{
			return false; 
		}
		const IComparatorT<VAL>& comparator = IComparatorT<VAL>::GetDefault();
		loop ( i, size )
		{
			const KEY& k = c.At(i).first;
			const VAL* p = GetValuePtr(k);
			if ( p == NULL )
			{
				return false;
			}
			const VAL& v = c.At(i).second;
			if ( ! comparator.IsEqualTo(v, *p) )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [取得] キーのみの参照アダプタ取得.
	 *		キーのみ参照アダプタクラスを作成します。
	 *	@return CConstKeysAdapter
	 */
	CConstKeysAdapter GetKeysAdapter(void) const
	{
		return CConstKeysAdapter(this);
	}

	/**
	 * [取得] 値のみの参照アダプタ取得.
	 *		値のみ参照アダプタクラスを作成します。
	 *	@return CConstValuesAdapter
	 */
	CConstValuesAdapter GetValuesAdapter(void) const
	{
		return CConstValuesAdapter(this);
	}

private:
	friend class CMapTest;	///< フレンドクラス宣言
};



/**@ingroup COLLECT
 * 文字文字マップクラス.
 *	@par必要ファイル
 *			TnbMap.h
 *	@date 06/01/01	新規
 */
typedef TNB::CMapT<CStr, CStr, LPCTSTR> CStrsMap;

/**@ingroup COLLECT
 * 文字文字マップクラス.
 *	@par必要ファイル
 *			TnbMap.h
 *	@date 06/01/01	新規
 */
typedef TNB::CMapT<CAscii, CAscii, LPCSTR> CAsciisMap;



}; // TNB
