#pragma once
/**
 *@file
 * シングルセット情報管理関係のヘッダ
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * シングルセット情報管理テンプレート
 *
 *		同じ値は２つ格納できません。小さい順にソートされて格納されます。
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *	@note 検索は Find() より GetEqualIndex() を使ったほうが高速です。
 *
 *	@param TYP 保持する型（operator<() 、及びコピー可能の型に限る）
 *		\code
 *		// 例）TFoo を格納したい場合
 *		struct TFoo
 *		{
 *			bool operator<(const TFoo& v) const
 *			{
 *				return a < v.a;
 *			}
 *			CStr a;
 *		};
 *		// 例）int を大きい順に格納したい場合
 *		struct TInteger
 *		{
 *			bool operator<(const TInteger& other) const
 *			{
 *				return value > other.value;
 *			}
 *			int value;
 *		};
 *		\endcode
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ISequenceCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbSet.h
 *
 *	@date 11/10/03 新規作成
 *	@date 12/07/29 InsertElements() 新規。
 *	@date 12/08/15 InsertElements() の仕様変更。 operator[] 追加。
 *	@date 14/11/10 Insert時、値が同じでも代入するように変更。
 */
template<typename TYP>
class CSingleSetT : public IConstCollectionT<TYP>, public ISequenceCollectionT<TYP>
{
public:

	/**
	 * コンストラクタ
	 *	@note 指定のメモリ確保時の追加サイズを設定します。
	 *	@note データは保持しません
	 *	@param size メモリ確保時の追加サイズ。 SetIncrementSize() と同じです。
	 */
	explicit CSingleSetT(size_t size = 0) : m_data(size)
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@note コピー元とバッファを共有します。
	 *	@param other コピー元
	 */
	CSingleSetT(const CSingleSetT<TYP>& other)
	{
		m_data = other.m_data;
	}
	
	/**
	 * [代入] 代入
	 *	@note 自インスタンスで管理しているバッファを開放し、	
	 *		代入元の情報を共有するようになります。
	 *	@param other 代入元 
	 *	@return 自分の参照
	 */
	CSingleSetT& operator=(const CSingleSetT& other)
	{
		m_data = other.m_data;
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
		return m_data.Lock(dwTime);
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		m_data.Unlock();
	}

	/**
	 * [取得] データアドレス取得.
	 *	@note	このアドレスの内容は書き換えてはいけません。
	 *	@note	取得したアドレスは delete で開放してはいけません。
	 *	@note	取得したアドレスは本クラスの計算等、操作すると無効になります。
	 *	@return アドレス。
	 */
	virtual const TYP* ReferBuffer(void) const
	{
		return m_data.ReferBuffer();
	}

	/**
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_data.GetSize();
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_data[index];
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
		return m_data.At(index);
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 RemoveEx() を使用してください。
	 *	@param index インデックス
	 *	@retval false	エラー。
	 *	@retval true	成功。
	 */
	bool Remove(INDEX index)
	{
		return m_data.Remove(index);
	}

	/**
	 * [削除] 空化
	 *	@note 自インスタンスで管理しているバッファを空にします。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool RemoveAll(void)
	{
		return m_data.RemoveAll();
	}

	/**
	 * [削除] 指定要素の削除.
	 *	@note 指定の要素を一つ、削除します。
	 *	@param	t 要素
	 *	@retval true 削除
	 *	@retval false 未発見
	 */
	bool Delete(const TYP& t)
	{
		INDEX i = GetEqualIndex(t);
		if ( i != INVALID_INDEX )
		{
			return m_data.Remove(i);
		}
		return false;
	}

	/**
	 * [確認] 値あるか？
	 *	@param t 要素
	 *	@retval false	未発見
	 *	@retval true	発見
	 */
	bool Has(const TYP& t) const
	{
		return GetEqualIndex(t) != INVALID_INDEX;
	}

	/**
	 * [検索] 指定要素の検索.
	 *	@note 開始インデックスを指定したい場合や、比較方法を独自に指定したい場合、 Find() を使います。
	 *	@param	t 要素
	 *	@retval INVALID_INDEX	未発見
	 *	@retval それ以外		発見したインデックス
	 */
	INDEX GetEqualIndex(const TYP& t) const
	{
		if ( IsEmpty() )
		{
			return INVALID_INDEX;
		}
		size_t sz = m_data.GetSize();
		if ( t < m_data[0] || m_data[sz - 1] < t )
		{
			return INVALID_INDEX;
		}
		INDEX f = 0;
		INDEX z = sz;
		while ( z > 0 )
		{
			INDEX x = z / 2;
			if ( m_data[f + x] < t )
			{
				// 大きい
				f += (x + 1);
				z = (z - 1) / 2;
			}
			else if ( t < m_data[f + x] )
			{
				// 小さい
				z /= 2;
			}
			else
			{
				return f + x;
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [検索] 指定要素挿入.
	 *	@param	t 要素
	 *	@retval INVALID_INDEX	失敗
	 *	@retval それ以外		成功。値は挿入したインデックス
	 */
	INDEX Insert(const TYP& t)
	{
		INDEX r = m_Insert(t);
		#ifdef _DEBUG
			if ( r != INVALID_INDEX )
			{
				if ( r >= 1 )
				{
					ASSERT( ! (t < At(r - 1)) );
				}
				if ( r + 1 < GetSize() )
				{
					ASSERT( ! (At(r + 1) < t) );
				}
			}
		#endif
		return r;
	}

	/**
	 * [検索] 指定要素挿入.
	 *	@param	t 要素
	 *	@retval INVALID_INDEX	失敗
	 *	@retval それ以外		成功。値は挿入したインデックス
	 */
	INDEX InsertEx(const TYP& t)
	{
		EXCLUSIVE( this );
		return Insert(t);
	}

	/**
	 * [検索] 指定要素挿入.
	 *	@param	t 要素
	 *	@retval false	失敗
	 *	@retval true	成功
	 */
	bool Insert(const IConstCollectionT<TYP>& t)
	{
		EXCLUSIVE( this );
		loop ( i, t.GetSize() )
		{
			if ( Insert(t.At(i)) == INVALID_INDEX )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [検索] 指定要素挿入.
	 *	@param size データ長。
	 *	@param P 挿入する内容を指定。
	 *	@retval INVALID_SIZE	エラー。
	 *	@return それ以外		削除した数。
	 */
	size_t InsertElements(size_t size, const TYP* P)
	{
		EXCLUSIVE( this );
		size_t cnt = 0;
		loop ( i, size )
		{
			if ( ! Has(*P) )
			{
				if ( Insert(*P) == INVALID_INDEX )
				{
					return INVALID_SIZE;
				}
				cnt++;
			}
			P++;
		}
		return cnt;
	}

protected:
	/**
	 * [通知] 同じ値発見通知.
	 *	@param index 同じ値のインデックス
	 *	@return Insert() の戻り値
	 */
	virtual INDEX OnFoundEqual(INDEX index)
	{
		return index;
	}

	CVectorT<TYP> m_data;	///< データ

private:
	/**
	 * [検索] 指定要素挿入.
	 *	@param	t 要素
	 *	@retval INVALID_INDEX	失敗
	 *	@retval それ以外		成功。値は挿入したインデックス
	 */
	INDEX m_Insert(const TYP& t)
	{
		if ( IsEmpty() )
		{
			return m_data.Add(t);
		}
		size_t sz = m_data.GetSize();
		if ( t < m_data[0] )
		{
			m_data.Insert(0, t);
			return 0;
		}
		if ( m_data[sz - 1] < t )
		{
			return m_data.Add(t);
		}
		INDEX f = 0;
		INDEX z = sz;
		while ( z > 0 )
		{
			INDEX x = z / 2;
			if ( m_data[f + x] < t )
			{
				// 大きい
				f += (x + 1);
				z = (z - 1) / 2;
			}
			else if ( t < m_data[f + x] )
			{
				// 小さい
				z /= 2;
			}
			else
			{
				// 同じ値発見
				m_data[f + x] = t;
				return OnFoundEqual(f + x);
			}
		}
		if ( m_data[f] < t )
		{
			f++;
		}
		m_data.Insert(f, t);
		return f;
	}
	friend class CSetTest;
};



#if 0

/**@ingroup COLLECT
 * マルチセット情報管理テンプレート
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *	@param TYP 保持する型（operator<() 、及びコピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ISequenceCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbSet.h
 *
 *	@date 11/10/03 新規作成
 */
template<typename TYP>
class CMultiSetT : public CSingleSetT<TYP>
{
public:

	/**
	 * [検索] 指定要素挿入.
	 *	@param	t 要素
	 *	@retval INVALID_INDEX	失敗
	 *	@retval それ以外		成功。値は挿入したインデックス
	 */
	INDEX Insert(const TYP& v)
	{
		loop ( i, m_data.GetSize() )
		{
			if ( v < m_data[i] || ! (m_data[i] < v) )
			{
				//その要素より小さいか同じ
				m_data.Insert(i, v);
				return i;
			}
		}
		m_data.Add(v);
		return m_data.GetSize() - 1;
	}

	/**
	 * [削除] 指定要素の削除.
	 *	@note 指定の要素をすべて、削除します。
	 *	@param	t 要素
	 *	@retval true 削除
	 *	@retval false 未発見
	 */
	bool Delete(const TYP& t)
	{
		TRange r = GetEqualRange(t);
		if ( r.lower != INVALID_INDEX )
		{
			m_data.RemoveElement(r.lower, (r.upper - r.lower) + 1);
			return true;
		}
		return false;
	}

	struct TRange
	{
		INDEX lower;
		INDEX upper;
	};

	/**
	 * [検索] 指定要素の検索.
	 *	@note 同じ値のもの含まれているインデックスを返します。
	 *	@param	t 要素
	 *	@return 範囲。 値が INVALID_INDEX の場合、未発見
	 */
	TRange GetEqualRange(const TYP& v) const
	{
		TRange r = { INVALID_INDEX, INVALID_INDEX };
		size_t sz = m_data.GetSize();
		for ( INDEX i = 0; i < sz; i++ )
		{
			if ( m_IsEqual(v, m_data[i]) )
			{
				//同じ
				r.lower = i;
				i++;
				for ( ; i < sz; i++ )
				{
					if ( v < m_data[i] || m_data[i] < v )
					{
						r.upper = i - 1;
						break;
					}
				}
				break;
			}
		}
		return r;
	}
	/**
	 * [比較] 同じ？
	 *	@param t1 値１
	 *	@param t2 値２
	 *	@retval true 同じ
	 *	@retval false 違う
	 */
	bool m_IsEqual(const TYP& t1, const TYP& t2) const
	{
		return ! (t1 < t2) && ! (t2 < t1);
	}

};

#endif



}; //TNB
