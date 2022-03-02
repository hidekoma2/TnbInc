#pragma once
/**
 *@file
 * ツリー型情報管理関係のヘッダ
 *
 *		ちょうどPCのフォルダとファイルのような情報の管理が可能な、
 *		Treeのテンプレートがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"
#include "TnbMap.h"
#include "TnbCollectionAdapter.h"
#include "TnbStr.h"
#include "TnbSerializeAdapter.h"



//TNB Library
namespace TNB
{



/**@ingroup TREE
 * ツリー型情報管理テンプレートクラス
 *
 *	@note		ICollenctionT は実装していません。
 *	@attention	本クラスのコピー機能は実体の参照の複製になります。
 *				別の実体にしたい場合、 Clone() を使う必要があります。
 *
 *	@param	KEY キーとする型。クラスの場合コピー機能が必須。
 *	@param	VAL 値とする型。クラスの場合コピー機能が必須。
 *	@param	INK キーとする型の入力型。省略可能。
 *
 *	@par必要ファイル
 *			TnbTree.h
 * 
 *	@date 06/04/28 新規作成
 *	@date 06/09/04 全面改訂
 */
template<typename KEY, typename VAL, typename INK = KEY>
class CTreeT : public ISerializable
{
	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	class COneHandle;										///< 前方宣言
	typedef CMapT<KEY,COneHandle,INK> CMapKey;				///< 内部用MAP型定義
	/// 個々情報型
	struct TOne
	{
		VAL							value;					///< 自分の値
		CPointerHandleT< CMapKey >	hpMap;					///< 枝情報
		/// コンストラクタ
		TOne(void) : value(VAL()) {}
	};

	/// 個々情報管理ハンドルクラス
	class COneHandle : public CPointerHandleT<TOne>, public ISerializable
	{
		DEFSUPER(CPointerHandleT<TOne>);
	public:
		/// コンストラクタ 
		COneHandle(void) {}
		/**
		 * 代入コンストラクタ.
		 *	@param P new で確保した TOne ポインタ。
		 */
		COneHandle(TOne* P) : _super(P) {}
		/**
		 * コピーコンストラクタ.
		 *	@param other 同じ型のインスタンス。
		 */
		COneHandle(const COneHandle& other) : _super(other) {}
		/**
		 * [通知] シリアライズ
		 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
		 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
		 */
		virtual void Serialize(ISerializer& _sr) const
		{
			_sr << ((*this)->value);
			size_t l = 0;
			if ( ! ((*this)->hpMap.IsNull()) )
			{
				l = (*this)->hpMap->GetSize();
			}
			_sr << l;
			if ( l > 0 )
			{
				_sr << *((*this)->hpMap);
			}
		}
		/**
		 * [通知] デシリアライズ
		 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
		 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
		 */
		virtual void Deserialize(const IDeserializer& ds)
		{
			COneHandle o = new TOne;
			ds >> (o->value);
			size_t l = 0;
			ds >> l;
			if ( l > 0 )
			{
				o->hpMap = new CMapKey;
				ds >> *(o->hpMap);
			}
			*this = o;
		}
	};

	typedef typename CMapKey::CConstKeysAdapter CConstKeysAdapter;	///< キー一覧用型

	#endif //_TnbDOXYGEN

	COneHandle	m_one;				///< 保持しているTreeの大元

public:

	/**@ingroup TREE
	 * ツリー型情報管理用 全キー走査コマンドインターフェース
	 *
	 *	@note	本インターフェースを実装し、 CTreeT::Scan() に渡すことで、
	 *			全情報をスキャンし、キー、値が順次本インターフェースに通知されます。
	 *
	 *	@see CTreeT::Scan
	 */
	struct ICommand
	{
		/// デストラクタ
		virtual ~ICommand(void) {}

		/**
		 * [通知] キーと値発見
		 *	@param iNestLevel 深さ
		 *	@param key 発見したキー。
		 *	@param value 発見したキー下の値。
		 *	@param tree 発見したキーを持つTREE。
		 *	@retval true 走査継続
		 *	@retval false 走査停止
		 */
		virtual bool OnFound(int iNestLevel, const KEY& key, const VAL& value, const CTreeT& tree) = 0;
	};

private:

	/**
	 * [走査] 全要素走査.
	 *		順次発見し、 ICommand インターフェースに通知します。
	 *	@param[in]	I コマンド
	 *	@param[in]	tree 対象の tree
	 *	@param[out]	_iNestLevel ネスト深さ
	 *	@retval true 最後まで走査
	 *	@retval false 中断
	 */
	bool m_Scan(ICommand* I, const CTreeT& tree, int& _iNestLevel)
	{
		_iNestLevel++;
		CTreeT::CConstKeysAdapter vkey = tree.GetKeysAdapter();
		loop ( i, vkey.GetSize() )
		{
			const KEY& k = vkey.At(i);
			ASSERTLIB(tree.HasKey(k));
			if ( ! I->OnFound(_iNestLevel, k, tree.At(k), tree) )
			{
				return false;
			}
			if ( tree.HasLeaf(k) && ! m_Scan(I, tree(k), _iNestLevel) )
			{
				return false;
			}
		}
		_iNestLevel--;
		return true;
	}

public:

	/**
	 * コンストラクタ 
	 *	@note 枝が一つもないTree
	 */
	CTreeT(void) : m_one(new TOne)
	{
		m_one->hpMap = new CMapKey;
	}

	/** 
	 * コピーコンストラクタ
	 *	@note 情報の共有をします。
	 *	@param other コピー元
	 */
	CTreeT(const CTreeT& other) : m_one(other.m_one) 
	{
	}

	/** デストラクタ */
	virtual ~CTreeT(void)
	{
		m_one.Null();
	}

	/**
	 * [代入] コピーオペレータ
	 *	@attention 情報の共有をします。
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CTreeT& operator=(const CTreeT& other)
	{
		m_one = other.m_one;
		return *this;
	}

	/**
	 * [取得] 直下の情報の参照
	 *	@return 情報(値と枝)
	 */
	const VAL& AtSelf(void) const
	{
		return m_one->value;
	}

	/**
	 * [取得] 直下の情報の参照
	 *	@return 情報(値と枝)
	 */
	VAL& AtSelf(void)
	{
		return m_one->value;
	}

	/**
	 * [確認] キー有無
	 *	@param key キー
	 *	@retval true 存在する
	 *	@retval false 存在しない
	 */
	bool HasKey(INK key) const
	{
		return m_one->hpMap->HasKey(key);
	}

	/**
	 * [取得] キー下の値の参照
	 *	@param key キー
	 *	@return 値の参照
	 *	@throw CEmptyException キーが存在しない時にスローされます。 
	 */
	const VAL& At(INK key) const
	{
		if ( HasKey(key) )
		{
			return 	m_one->hpMap->operator[](key)->value;
		}
		throw CEmptyException();
	}

	/**
	 * [取得] キー下の値の参照
	 *	@param key キー
	 *	@return 値の参照
	 *	@throw CEmptyException キーが存在しない時にスローされます。 
	 */
	const VAL& operator[](INK key) const
	{
		return At(key);
	}

	/**
	 * [取得] キー下の値の参照
	 *	@note キーがない場合、追加します。	
	 *	@param key キー
	 *	@return 値の参照
	 */
	VAL& At(INK key)
	{
		if ( ! HasKey(key) )
		{
			COneHandle P = new TOne;
			P->value = VAL();
			m_one->hpMap->SetKey(key, P);
		}
		return 	m_one->hpMap->operator[](key)->value;
	}

	/**
	 * [取得] キー下の値の参照
	 *	@note キーがない場合、追加します。	
	 *	@param key キー
	 *	@return 値の参照
	 */
	VAL& operator[](INK key)
	{
		return At(key);
	}

	/**
	 * [取得] キー下の値取得
	 *	@param key キー
	 *	@return 値の参照
	 *	@throw CEmptyException キーが存在しない時にスローされます。 
	 */
	VAL Get(INK key) const
	{
		return At(key);
	}

	/**
	 * [取得] キーの数
	 *	@return 数
	 */
	size_t GetSize(void) const
	{
		return m_one->hpMap->GetSize();
	}

	/**
	 * [取得] キー一覧
	 *	@return 一覧
	 */
	CVectorT<KEY> EnumKeys(void) const
	{
		return m_one->hpMap->EnumKeys();
	}

	/**
	 * [設定] キー下の値設定
	 *	@param key キー
	 *	@param v 値
	 *	@retval true 設定成功
	 *	@retval false キーは存在しない
	 */
	bool Set(INK key, VAL v)
	{
		if ( HasKey(key) )
		{
			m_one->hpMap->operator[](key)->value = v;
			return true;
		}
		return false;
	}

	/**
	 * [追加] キー下の値追加
	 *	@param key キー
	 *	@param v 値
	 *	@retval true 追加成功
	 *	@retval false キーは存在していた
	 */
	bool Add(INK key, VAL v)
	{
		if ( HasKey(key) )
		{
			return false;
		}
		COneHandle P = new TOne;
		P->value = v;
		m_one->hpMap->SetKey(key, P);
		return true;
	}

	/**
	 * [削除] キー下の値と枝を削除
	 *	@param key キー
	 *	@retval true 成功
	 *	@retval false 失敗（キーが存在しない）
	 */
	bool Remove(INK key)
	{
		if ( ! HasKey(key) )
		{
			return false;
		}
		return m_one->hpMap->RemoveKey(key);
	}
	
	/**
	 * [確認] キーは枝を持つか
	 *	@param key キー
	 *	@retval true 持っている
	 *	@retval false 持っていない
	 */
	bool HasLeaf(INK key) const
	{
		if ( ! HasKey(key) )
		{
			return false;
		}
		const CPointerHandleT<TOne>& c = m_one->hpMap->operator[](key);
		if ( c->hpMap.IsNull() )
		{
			return false;
		}
		return ! c->hpMap->IsEmpty();
	}

	/**
	 * [追加] キー下に枝を追加
	 *	@note キーがあると失敗します。
	 *	@attention	追加するTREEが、自分の子供、親でないことを保障してから枝を指定してください。
	 *	@param key キー
	 *	@param other 枝
	 *	@retval true 追加成功
	 *	@retval false キーは存在していた
	 */
	bool AddLeaf(INK key, CTreeT<KEY,VAL,INK>& other)
	{
		if ( HasKey(key) )
		{
			return false;
		}
		//add 自分の子供、親でないことを確認。
		m_one->hpMap->SetKey(key, other.m_one);
		return true;
	}

	/**
	 * [削除] 値と枝を削除
	 */
	void RemoveAll(void)
	{
		m_one = new TOne;
		m_one->hpMap = new CMapKey;
	}

	/**
	 * [取得] キーのみの参照アダプタ取得. 
	 *		キーのみ参照アダプタクラスを作成します。
	 *	@return CConstKeysAdapter
	 */
	CConstKeysAdapter GetKeysAdapter(void) const
	{
		return m_one->hpMap->GetKeysAdapter();
	}

	/**
	 * [取得] キー下のTree取得
	 *	@note 取得したTreeは操作可能です。操作すると元のTreeに影響が有ります。
	 *	@note 存在しないキーを指定すると拡張されます。
	 *	@param key キー
	 *	@return Tree
	 */
	CTreeT<KEY,VAL,INK> Refer(INK key)
	{
		if ( ! HasKey(key) )
		{
			COneHandle P = new TOne;
			P->value = VAL();
			m_one->hpMap->SetKey(key, P);
		}
		COneHandle& c = m_one->hpMap->operator[](key);
		if ( c->hpMap.IsNull() )
		{
			c->hpMap = new CMapKey;
		}
		CTreeT t;
		t.m_one = c;
		return t;
	}

	/**
	 * [取得] キー下のTree取得
	 *	@note 取得したTreeは操作可能です。操作すると元のTreeに影響が有ります。
	 *	@note 存在しないキーを指定すると拡張されます。
	 *	@param key キー
	 *	@return Tree
	 */
	CTreeT<KEY,VAL,INK> operator()(INK key)
	{
		return Refer(key);
	}

	/**
	 * [取得] キー下のTree取得
	 *	@param key キー
	 *	@return Tree
	 *	@throw CEmptyException キーの下の枝が存在しない時にスローされます。 
	 */
	const CTreeT<KEY,VAL,INK> Refer(INK key) const
	{
		if ( ! HasKey(key) )
		{ 
			throw CEmptyException();
		}
		const COneHandle& c = m_one->hpMap->operator[](key);
		if ( c->hpMap.IsNull() )
		{
			throw CEmptyException();
		}
		CTreeT t;
		t.m_one = c;
		return t;
	}

	/**
	 * [取得] キー下のTree取得
	 *	@param key キー
	 *	@return Tree
	 *	@throw CEmptyException キーの下の枝が存在しない時にスローされます。 
	 */
	const CTreeT<KEY,VAL,INK> operator()(INK key) const
	{
		return Refer(key);
	}

	/**
	 * [通知] シリアライズ
	 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << m_one;
	}

	/**
	 * [通知] デシリアライズ
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> m_one;
	}

	/**
	 * [複製] 複製作成
	 *	@return 複製。変更しても本インスタンスに影響有りません。
	 *	@todo 現在シリアライズ機能を使って複製しています。そのため処理速度は遅いです。
	 */
	CTreeT<KEY,VAL,INK> Clone(void) const
	{
		CVectorT<BYTE> vb;
		CSerializeAdapter sr(&vb);
		sr << m_one;
		CTreeT t;
		CDeserializeAdapter ds(&vb);
		ds >> t.m_one;
		return t;
	}

	/**
	 * [走査] 全要素走査.
	 *		順次発見し、 ICommand インターフェースに通知します。
	 *	@param I コマンド
	 *	@retval true 最後まで走査
	 *	@retval false 中断
	 */
	bool Scan(ICommand* I)
	{
		int iNestLevel = 0;
		if ( ! I->OnFound(iNestLevel, KEY(), AtSelf(), *this) )
		{
			return false;
		}
		return m_Scan(I, *this, iNestLevel);
	}

private:
	friend class CTreeTest;
};



/**@ingroup TREE
 * ツリー型文字列情報管理クラス.
 *		キー=文字、値=文字のツリーを管理できます。
 *	@par必要ファイル
 *			TnbTree.h
 */
class CStrsTree : public CTreeT<CStr,CStr,LPCTSTR>
{
	typedef CTreeT<CStr,CStr,LPCTSTR> _super;	///< 親クラス名再宣言
	DEFSUPER(_super);

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	class CToStringCommand : public _super::ICommand
	{
		CStr m_str;		///< 結果
		/**
		 * [通知] キーと値発見
		 *	@param iNestLevel 深さ
		 *	@param key 発見したキー。
		 *	@param value 発見したキー下の値。
		 *	@param tree 発見したキーを持つTREE。
		 *	@retval true 走査継続
		 *	@retval false 走査停止
		 */
		virtual bool OnFound(int iNestLevel, const CStr& key, const CStr& value, const _super& tree)
		{
			m_str += CStr::Fmt(_T("%s[+]%s = %s\n")
							, CStr::Lineup(_T(" |"), iNestLevel)
							, key
							, value);
			return true;
		}
	public:
		CStr GetString(void) { return m_str; }
	};

	#endif //_TnbDOXYGEN

public:
	/** コンストラクタ */
	CStrsTree(void) : _super()
	{
	}

	/** 
	 * コピーコンストラクタ
	 *	@note 情報の共有をします。
	 *	@param other コピー元
	 */
	CStrsTree(const CTreeT<CStr,CStr,LPCTSTR>& other) : _super(other) 
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@attention 情報の共有をします。
	 *	@param other コピー元
 	 *	@return 自分の参照
	 */
	CStrsTree& operator=(const CTreeT<CStr,CStr,LPCTSTR>& other)
	{
		_super::operator=(other);
		return *this;
	}

	/**
	 * [変換] 単一文字列化
	 *	@return 変換後の文字列
	 */
	CStr ToString(void)
	{
		CToStringCommand c;
		_super::Scan(&c);
		return c.GetString();
	}
	/**
	 * [取得] キー下のTree取得
	 *	@note 取得したTreeは操作可能です。操作すると元のTreeに影響が有ります。
	 *	@note 存在しないキーを指定すると拡張されます。
	 *	@param key キー
	 *	@return Tree
	 */
	CStrsTree Refer(LPCTSTR key)
	{
		return _super::Refer(key);
	}

	/**
	 * [取得] キー下のTree取得
	 *	@note 取得したTreeは操作可能です。操作すると元のTreeに影響が有ります。
	 *	@note 存在しないキーを指定すると拡張されます。
	 *	@param key キー
	 *	@return Tree
	 */
	CStrsTree operator()(LPCTSTR key)
	{
		return Refer(key);
	}

	/**
	 * [取得] キー下のTree取得
	 *	@param key キー
	 *	@return Tree
	 *	@throw CEmptyException キーの下の枝が存在しない時にスローされます。 
	 */
	const CStrsTree Refer(LPCTSTR key) const
	{
		return _super::Refer(key);
	}

	/**
	 * [取得] キー下のTree取得
	 *	@param key キー
	 *	@return Tree
	 *	@throw CEmptyException キーの下の枝が存在しない時にスローされます。 
	 */
	const CStrsTree operator()(LPCTSTR key) const
	{
		return Refer(key);
	}

	/**
	 * [複製] 複製作成.
	 *	@return 複製。変更しても本インスタンスに影響有りません。
	 *	@todo 現在シリアライズ機能を使って複製しています。そのため処理速度は遅いです。
	 */
	CStrsTree Clone(void) const
	{
		return _super::Clone();
	}
};



}; // TNB



