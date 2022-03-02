#pragma once
/**
 *@file
 * 簡易配列型情報管理関係のヘッダ
 *
 *		一意なキーに値を割り当ててコレクションすることができます。
 *		キーで値を取り出したり削除することができます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * マップ型情報管理テンプレート
 *
 *		一意なキーに値を割り当ててコレクションすることができます。
 *		キーで値を取り出したり削除することができます。
 *	
 *	@see CMapT 
 *
 *	@param	KEY	キーとする型。クラスの場合コピー機能、比較機能( IComparableT か、引き算)が必須。
 *	@param	VAL	値とする型。クラスの場合コピー機能が必須。
 *	@param	INK	キーとする型の入力型。省略可能。
 *
 *	@par必要ファイル
 *			TnbSimpleMap.h
 *
 *	@date 09/04/22 新規作成.
 *	@date 19/07/07 サイズ指定のコンストラクタ追加
 */
template<typename KEY, typename VAL, typename INK=KEY>
class CSimpleMapT
{
public:

	/// パラメータ型
	struct TParam
	{
		KEY		key;	///< キー
		VAL		val;	///< 値
		/**
		 * コンストラクタ
		 *	@param k キー
		 *	@param v 値
		 */
		TParam(INK k = KEY(), VAL v = VAL()) : key(k), val(v)
		{}
	};

	/// コンストラクタ
	explicit CSimpleMapT(void) : m_isValidLast(false)
	{
	}

	/// コンストラクタ
	/// @param bufferSize バッファサイズ
	explicit CSimpleMapT(size_t bufferSize) : m_isValidLast(false), m_map(bufferSize)
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元。
	 */
	CSimpleMapT(const CSimpleMapT& other)
	{
		m_map = other.m_map;
	}

	/**
	 * [複製] コピーオペレータ
	 *	@param other コピー元。
	 *	@return 自分の参照
	 */
	CSimpleMapT& operator=(const CSimpleMapT& other)
	{
		m_map = other.m_map;
		return *this;
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	size_t GetSize(void) const
	{
		return m_map.GetSize();
	}

	/**
	 * [取得] キーに対する値の参照
	 *	@note キーがない場合、新規に追加します。
	 *	@param key キー
	 *	@return VALの参照（操作すると、MAP内の情報も変化します）。
	 */
	VAL& operator[](INK key)
	{
		INDEX i = Find(key);
		if ( i != INVALID_INDEX )
		{
			return m_map[i].val;
		}
		i = m_map.Add(TParam(key, VAL()));
		m_lastKey = key;
		m_lastIndex = i;
		m_isValidLast = true;
		return m_map[i].val;
	}

	/**
	 * [取得] キーに対する値の参照
	 *	@note キーがない場合、新規に追加します。
	 *	@param key キー
	 *	@return VALの参照（操作すると、MAP内の情報も変化します）。
	 *	@throw CEmptyException キーが存在しないことを示すためにスローされます。
	 */
	const VAL& operator[](INK key) const
	{
		INDEX i = Find(key);
		if ( i != INVALID_INDEX )
		{
			return m_map[i].val;
		}
		throw CEmptyException();
	}

	/**
	 * [削除] キーと値を削除
	 *	@param key キー
	 *	@retval true 成功
	 *	@retval false 失敗（キーがない）
	 */
	bool RemoveKey(INK key)
	{
		INDEX i = Find(key);
		if ( i != INVALID_INDEX )
		{
			m_map.Remove(i);
			m_isValidLast = false;
			return true;
		}
		return false;
	}

	/**
	 * [参照] パラメータ参照.
	 *	@param index インデックス
	 *	@return パラメータ参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TParam& Ref(INDEX index) const
	{
		return m_map[index];
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@param index インデックス
	 *	@retval false	エラー。
	 *	@retval true	成功。
	 */
	bool Remove(INDEX index)
	{
		m_isValidLast = false;
		return m_map.Remove(index);
	}

	/**
	 * [検索] キー検索.
	 *	@param key キー
	 *	@retval INVALID_INDEX 存在しない
	 *	@retval 上記以外 発見。数値はインデックス。
	 */
	INDEX Find(INK key) const
	{
		if ( m_isValidLast && m_lastKey == key )
		{
			ASSERT( m_map[m_lastIndex].key == m_lastKey );
			return m_lastIndex;
		}
		loop ( i, m_map.GetSize() )
		{
			if ( m_map[i].key == key )
			{
				m_lastKey = key;
				m_lastIndex = i;
				m_isValidLast = true;
				return i;
			}
		}
		return INVALID_INDEX;
	}
	/**
	 * [確認] キー有無
	 *	@param key キー
	 *	@retval true 存在する
	 *	@retval false 存在しない
	 */
	bool HasKey(INK key) const
	{
		return Find(key) != INVALID_INDEX;
	}

	/**
	 * [削除] 空化
	 *	@note 自インスタンスで管理しているバッファを空にします。
	 */
	void RemoveAll(void)
	{
		m_isValidLast = false;
		m_map.RemoveAll();
	}

private:
	CSimpleVectorT<TParam>	m_map;	///< マップ
	mutable	bool	m_isValidLast;	///< ラスト情報有効？
	mutable	INK		m_lastKey;		///< ラストのキー
	mutable INDEX	m_lastIndex;	///< ラストのインデックス
};



}; // TNB

