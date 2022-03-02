#pragma once
/**
 *@file
 * 情報群管理アダプタ関係のヘッダ
 *
 *		情報群管理のアダプタクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * アダプタベーステンプレート
 *
 *		アダプタ―のベースとなるテンプレートクラスです。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbCollectionAdapter.h
 *
 *	@date 06/09/13 抽象化して新規作成
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 11/06/01 コピーオペレータを実装
 */
template<typename TYP>
class CAbstractAdapterT : public IConstCollectionT<TYP>
{
	DEFSUPER(IConstCollectionT<TYP>);
protected:

	const IConstCollectionT<TYP>*	m_pCol;		///< 対象のコレクション
	size_t							m_size;	///< サイズ

public:

	/**
	 * コンストラクタ
	 *	@param c IConstCollectionT を持つコレクション
	 */
	CAbstractAdapterT(const IConstCollectionT<TYP>* c)
		: _super(), m_pCol(c), m_size(0)
	{
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
		return m_pCol->Lock(); 
	}

	/// [排他] アンロック
	virtual void Unlock(void) const 
	{
		m_pCol->Unlock(); 
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [代入] コピーオペレータ.
	 *	@param other コピー元.
	 *	@return 自分の参照
	 */
	CAbstractAdapterT& operator=(const CAbstractAdapterT& other)
	{
		m_pCol = other.m_pCol;
		m_size = other.m_size;
		return *this;
	}
};



/**@ingroup COLLECT ITERATOR
 * オフセット参照アダプタテンプレート
 *
 *		IConstCollectionT を持つコレクションの一部を操作する時にこのアダプタを使います。
 *		コンストラクタでコレクションとオフセット、サイズを指定すると、その範囲のコレクションになります。
 *		実体は、コンストラクタで渡したものなので、本インスタンスが有効の間、実体を消さないようにします。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbCollectionAdapter.h
 *
 *	@date 06/05/23 新規作成
 *	@date 06/08/01 GetConstPointerに依存しないつくりに変更。
 *	@date 06/09/13 抽象ベースクラス使用。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 09/11.09 サイズ指定方法の変更。
 */
template<typename TYP>
class CConstOffsetAdapterT : public CAbstractAdapterT<TYP>
{
	DEFSUPER(CAbstractAdapterT<TYP>);
	INDEX	m_startIndex;				///< 開始オフセット値
public:

	/**
	 * コンストラクタ
	 *	@param c IConstCollectionT を持つコレクション
	 *	@param startIndex 開始インデックスを指定
	 *	@param size サイズを指定。省略した場合、c の最後までが長さになります。
	 *				また、c よりも大きいサイズを指定しても c の最後までになります。
	 */
	CConstOffsetAdapterT(const IConstCollectionT<TYP>* c, INDEX startIndex, size_t size = INVALID_SIZE)
		: _super(c), m_startIndex(startIndex) 
	{
		_super::m_size = size;
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		size_t l = m_pCol->GetSize() - m_startIndex;
		if ( _super::m_size != INVALID_SIZE && l > _super::m_size)
		{
			l = _super::m_size;
		}
		return l;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_pCol->At(index + m_startIndex);
	}
};



/**@ingroup COLLECT ITERATOR
 * リバース参照アダプタテンプレート
 *
 *		IConstCollectionT を持つコレクションをインデックスを逆順にするアダプタです。
 *		コンストラクタでコレクションを指定すると、並びが逆のコレクションになります。
 *		実体は、コンストラクタで渡したものなので、本インスタンスが有効の間、実体を消さないようにします。
 *		また、サイズはコンストラクタ時に記憶しますので、元コレクションを拡張しても本クラスのサイズは変化しません。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbCollectionAdapter.h
 *
 *	@date 06/06/01 新規作成
 *	@date 06/08/01 GetConstPointerに依存しないつくりに変更。
 *	@date 06/09/13 抽象ベースクラス使用。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP>
class CConstReverseAdapterT : public CAbstractAdapterT<TYP>
{
	DEFSUPER(CAbstractAdapterT<TYP>);
public:

	/**
	 * コンストラクタ
	 *	@param c IConstCollectionT を持つコレクション
	 */
	explicit CConstReverseAdapterT(const IConstCollectionT<TYP>* c) : _super(c) 
	{
		_super::m_size = c->GetSize();
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_pCol->At(_super::m_size - index - 1);
	}
};



/**@ingroup COLLECT ITERATOR
 * アペンド参照アダプタテンプレート
 *
 *		IConstCollectionT を持つコレクション二つを連結して参照する時にこのアダプタを使います。
 *		実体は、コンストラクタで渡したものなので、本インスタンスが有効の間、実体を消さないようにします。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbCollectionAdapter.h
 *
 *	@date 06/06/30 新規作成
 *	@date 06/08/01 GetConstPointerに依存しないつくりに変更。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP>
class CConstAppendAdapterT : public CAbstractAdapterT<TYP>
{
	DEFSUPER(CAbstractAdapterT<TYP>);
	const IConstCollectionT<TYP>*	m_p2ndCol;	///< 対象コレクション(後)
	size_t	m_sizeCol;
public:

	/**
	 * コンストラクタ
	 *	@param c1 IConstCollectionT を持つコレクション
	 *	@param c2 IConstCollectionT を持つコレクション
	 */
	CConstAppendAdapterT(const IConstCollectionT<TYP>* c1, const IConstCollectionT<TYP>* c2)
		: _super(c1), m_p2ndCol(c2)
	{
		m_sizeCol = m_pCol->GetSize();
		_super::m_size = m_sizeCol + m_p2ndCol->GetSize();
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
		return m_pCol->Lock() && m_p2ndCol->Lock();
	}

	/// [排他] アンロック
	virtual void Unlock(void) const
	{
		m_p2ndCol->Unlock();
		m_pCol->Unlock();
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		return (index < m_sizeCol) ? m_pCol->At(index) : m_p2ndCol->At(index - m_sizeCol);
	}
};



/**@ingroup COLLECT ITERATOR
 * オフセットアダプタテンプレート
 *
 *		ICollectionT を持つコレクションの一部を操作する時にこのアダプタを使います。
 *		コンストラクタでコレクションとオフセット、サイズを指定すると、その範囲のコレクションになります。
 *		実体は、コンストラクタで渡したものなので、本インスタンスが有効の間、実体を消さないようにします。
 *
 *	@note Add , Append , Remove , RemoveAll , Copy など、コレクションサイズの変わるメソッドは失敗します。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbCollectionAdapter.h
 *
 *	@date 06/05/23 新規作成
 *	@date 06/08/01 GetConstPointerに依存しないつくりに変更。
 *	@date 06/08/11 Element() を At() に改名。ReferElementPtr() を廃止。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP>
class COffsetAdapterT : public ICollectionT<TYP>
{
	DEFSUPER(ICollectionT<TYP>);
	ICollectionT<TYP>*	m_pCol;			///< 対象のコレクション
	INDEX				m_startIndex;	///< 開始オフセット
	size_t				m_size;			///< サイズ
public:

	/**
	 * コンストラクタ
	 *	@param c ICollectionT を持つコレクション
	 *	@param startIndex 開始インデックスを指定
	 *	@param size サイズを指定。指定がない場合、c の最後までが長さになります。
	 *				また、c よりも大きいサイズを指定しても c の最後までになります。
	 */
	COffsetAdapterT(ICollectionT<TYP>* c, INDEX startIndex, size_t size = 0)
		: _super(), m_pCol(c), m_startIndex(startIndex), m_size(size)
	{
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
		return m_pCol->Lock(); 
	}

	/// [排他] アンロック
	virtual void Unlock(void) const 
	{
		m_pCol->Unlock(); 
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		size_t l = m_pCol->GetSize() - m_startIndex;
		return TNB::IsInRange(m_size, l) ? m_size : l;
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
		return m_pCol->At(index + m_startIndex);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の参照
	 *	@throw CNotSupportException サポートしていないことを示すためにスローされます。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual TYP& Ref(INDEX index)
	{
		return m_pCol->Ref(index + m_startIndex);
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
		return m_pCol->Set(index + m_startIndex, t);
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@attention 本アダプタでは使用できません。
	 *	@param t 要素。
	 *	@return 常に INVALID_INDEX。
	 */
	virtual INDEX Add(const TYP& t) 
	{
		return INVALID_INDEX; 
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@attention 本アダプタでは使用できません。
	 *	@param index インデックス
	 *	@return 常に false
	 */
	virtual bool Remove(INDEX index) 
	{
		return false; 
	}

	/**
	 * [設定] 要素の入れ替え
	 *	@param index1 インデックス１
	 *	@param index2 インデックス２
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual void Swap(INDEX index1, INDEX index2)
	{
		m_pCol->Swap(index1 + m_startIndex, index2 + m_startIndex);
	}
};



/**@ingroup COLLECT ITERATOR
 * 情報参照アダプタテンプレート
 *
 *		IConstCollectionT / ISequenceCollectionT インターフェースを持たない配列を
 *		IConstCollectionT / ISequenceCollectionT で参照できるようにするアダプタクラスです。
 *
 *	@note	begin() / end() によるイテレータは、単純に要素のポインタになっています。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ISequenceCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbCollectionAdapter.h
 *
 *	@date 06/04/26 新規
 *	@date 06/05/01	ISequenceCollectionT を実装するように変更。
 *					CConstCollectionTに名前変更。
 *	@date 06/05/15 	CConstCollectorTに名前変更。
 *	@date 06/06/29 	CConstAdapterTに名前変更。
 *	@date 06/08/13 イテレータを STL にあわせて、改修。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 11/06/01 コピーオペレータを実装
 */
template<typename TYP>
class CConstAdapterT : public IConstCollectionT<TYP>, public ISequenceCollectionT<TYP>
{
	DEFSUPER(IConstCollectionT<TYP>);
	const TYP*			m_pcType;		///< 保持するデータのポインタ
	size_t				m_size;			///< 保持するデータの長さ
public:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// const_iterator型宣言
	typedef const TYP* const_iterator;
	#endif //_TnbDOXYEM

	/**
	 * [反復] 先頭const_iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return m_pcType; }

	/**
	 * [反復] 最後const_iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return m_pcType + m_size; }

	/**
	 * コンストラクタ
	 *	@param size	参照元の長さ
	 *	@param P	参照元のポインタ
	 */
	CConstAdapterT(size_t size, LPCVOID P) 
		: _super(), m_pcType(static_cast<const TYP*>(P)), m_size(size)
	{
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ダミー。省略してください。
	 *	@return 常にtrue。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true;
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const 
	{
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [取得] データアドレス取得.
	 *	@note	このアドレスの内容は書き換えてはいけません。
	 *	@note	取得したアドレスは delete で開放してはいけません。
	 *	@note	取得したアドレスは本クラスの計算等、操作すると無効になります。
	 *	@return アドレス。確保していない場合、NULLが返ります。
	 */
	virtual const TYP* ReferBuffer(void) const
	{
		return m_pcType;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
		return m_pcType[index];
	}

	/**
	 * [取得] データ要素取得
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& operator[](INDEX index) const
	{
		return At(index);
	}

	/**
	 * [代入] コピーオペレータ.
	 *	@param other コピー元.
	 *	@return 自分の参照
	 */
	CConstAdapterT& operator=(const CConstAdapterT& other)
	{
		m_pcType = other.m_pcType;
		m_size = other.m_size;
		return *this;
	}
};



/**@ingroup COLLECT ITERATOR
 * INDEXアダプタテンプレート
 *
 *		IConstCollectionT を持つコレクションを一時的に並び替えたい時にこのアダプタを使います。
 *		内部でIndex変換テーブルを持ち、要素の Swap や、要素の削除や添削を行っても、実体は変化せず、
 *		このIndex変換テーブルのみ操作します。
 *
 *		実体は、コンストラクタで渡したものなので、本インスタンスが有効の間、実体を消さないようにします。
 *		また、実体のサイズも変更しないようにしてください。
 *
 *	@note ICollectionT は持っていませんが、 Sort() , Remove() , Swap() メソッドが有ります。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@see	IConstCollectionT を実装しています。
 *
 *	@par必要ファイル
 *			TnbCollectionAdapter.h
 *
 *	@date 06/08/22 新規作成
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 07/06/19 Cull() , ResetIndex() を追加。
 */
template<typename TYP>
class CIndexAdapterT : public IConstCollectionT<TYP>, CCopyImpossible
{
	DEFSUPER(IConstCollectionT<TYP>);

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	// memo; ICollectionT の sort を使用するためにインナークラスを作成している。
	struct TParam : ICollectionT<TYP>
	{
		DEFSUPER(ICollectionT<TYP>);
		const IConstCollectionT<TYP>*	m_pCol;		///< 元コレクション参照
		INDEX*							m_pIndexs;	///< Indexテーブル
		size_t							m_size;		///< Indexテーブルサイズ
		// ロック
		virtual bool Lock(DWORD dwTime = INFINITE) const { return m_pCol->Lock(); }
		// アンロック
		virtual void Unlock(void) const { m_pCol->Unlock(); }
		// 要素数取得
		virtual size_t GetSize(void) const { return m_size; }
		// 要素の参照取得.
		virtual const TYP& At(INDEX index) const
		{
			if ( ! IsInRange(index) ){ throw CIndexOutOfBoundsException(); }
			return m_pCol->At(m_pIndexs[index]);
		}
		// 要素の参照取得.
		virtual TYP& Ref(INDEX index) { throw CNotSupportException(); }
		// 要素の設定.
		virtual bool Set(INDEX index, const TYP& t) { return false; }
		// 要素一つ追加.
		virtual INDEX Add(const TYP& t) { return INVALID_INDEX; }
		// 要素一つ削除.
		virtual bool Remove(INDEX index)
		{
			if ( ! IsInRange(index) ){ return false; }
			for ( INDEX i = index; i < m_size - 1; i++ )
			{
				m_pIndexs[i] = m_pIndexs[i + 1];
			}
			m_size--;
			return true;
		}
		// 要素の入れ替え
		virtual void Swap(INDEX index1, INDEX index2)
		{
			if ( ! IsInRange(index1) || ! IsInRange(index2) ){ throw CIndexOutOfBoundsException(); }
			TNB::Swap(m_pIndexs[index1], m_pIndexs[index2]);
		}
	};

	#endif //Document作成用シンボル

	TParam m_param;		///< 属性

public:

	/**
	 * コンストラクタ
	 *	@param c IConstCollectionT を持つコレクション
	 */
	CIndexAdapterT(const IConstCollectionT<TYP>* c) : _super()
	{
		m_param.m_pCol			= c;
		m_param.m_size			= c->GetSize();
		m_param.m_pIndexs		= new INDEX[m_param.m_size];
		loop ( i, m_param.m_size )
		{
			m_param.m_pIndexs[i] = i;
		}
	}

	/// デストラクタ
	virtual ~CIndexAdapterT(void)
	{
		if ( m_param.m_pIndexs != NULL )
		{
			delete[] m_param.m_pIndexs;
			m_param.m_pIndexs = NULL;
		}
	}

	/**
	 * [取得] オリジナルIndex取得.
	 *	@note	At() , Get() などは本クラスのメソッドで可能ですが、
	 *			Set() , Remove() など対象コレクションの書き換え操作を行いたい場合、
	 *			このメソッドで対象コレクションのインデックスを得て、対象コレクションを操作します。<BR>
	 *	@attention 対象コレクションを操作したら、一度 ResetIndex() を実行するか、
	 *				このアダプタでの操作は行わないようにしてください。
	 *	@param index インデックス
	 *	@retval INVALID_INDEX インデックスが範囲外
	 *	@retval INVALID_INDEX以外 コンストラクタで指定したコレクションでのインデックス
	 */
	INDEX GetRawIndex(INDEX index) const
	{
		return IsInRange(index) ? m_param.m_pIndexs[index] : INVALID_INDEX;
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
		return m_param.Lock(); 
	}

	/// [排他] アンロック
	virtual void Unlock(void) const 
	{
		m_param.Unlock(); 
	}

	/**
	 * [取得] 要素数取得
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const 
	{
		return m_param.m_size; 
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 一時的な参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const TYP& At(INDEX index) const
	{
		return m_param.At(index);
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@param index インデックス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Remove(INDEX index)
	{
		return m_param.Remove(index);
	}

	/**
	 * [設定] 要素の入れ替え
	 *	@param index1 インデックス１
	 *	@param index2 インデックス２
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	void Swap(INDEX index1, INDEX index2)
	{
		m_param.Swap(index1, index2);
	}

	/**
	 * [操作] ソート.
	 *		コンパレータ( IComparatorT のサブクラス)を使用して、比較します。
	 *	@param	comparator コンパレーター
	 *	@param	boIsReverse	省略すると小さい順にソートします。
	 *						trueを指定すると大きい順位ソートします。
	 *	@param	boIsBubble	trueをバブルソートアルゴリズムを使います。
	 *						通常はクイックソートアルゴリズムです。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Sort(const IComparatorT<TYP>& comparator, bool boIsReverse = false, bool boIsBubble = false)
	{
		return m_param.Sort(comparator, boIsReverse, boIsBubble);
	}

	/**
	 * [操作] ソート.
	 *		IComparatorT::GetDefault() で得られるコンパレータを使用して比較します。
	 *	@param	boIsReverse	省略すると小さい順にソートします。
	 *						trueを指定すると大きい順位ソートします。
	 *	@param	boIsBubble	trueをバブルソートアルゴリズムを使います。
	 *						通常はクイックソートアルゴリズムです。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Sort(bool boIsReverse = false, bool boIsBubble = false)
	{	
		return m_param.Sort(boIsReverse, boIsBubble);
	}

	/**
	 * [削除] 間引き.
	 *		指定のデータの条件に合わないもの(チェッカーの結果により)を削除します。
	 *	@param	checker {@link IConstCollectionT::IChecker チェッカー}
	 *	@param	boIsReverse	省略すると条件に一致を残します。
	 *						trueを指定すると条件不一致を残します。
	 *	@retval 削除した数。
	 */
	size_t Cull(const IChecker& checker, bool boIsReverse = false)
	{
		EXCLUSIVE(this);
		size_t r = 0;
		if ( m_param.m_size > 0 )C
		{
			size_t size = m_param.m_size;
			INDEX* P = new INDEX[size];
			MemCopy(P, m_param.m_pIndexs, size);
			for ( size_t i = 0; i < size; i++ )
			{
				if ( checker.IsValid(At(i)) ^ boIsReverse )
				{
					m_param.m_pIndexs[r++] = P[i];
				}
			}
			m_param.m_size = r;
			delete[] P;
		}
		return r;
	}

	/**
	 * [操作] リセットインデックス.
	 *	@note インデックステーブルを初期化します。対象コレクションと同じ数、同じ並びになります。
	 */
	void ResetIndex(void)
	{
		if ( m_param.m_size	!= m_pCol->GetSize() )
		{
			m_param.m_size = m_pCol->GetSize();
			if ( m_param.m_pIndexs != NULL )
			{
				delete[] m_param.m_pIndexs;
			}
			m_param.m_pIndexs = new INDEX[m_param.m_size];
		}
		loop ( i, m_param.m_size )
		{
			m_param.m_pIndexs[i] = i;
		}
	}




};



}; // TNB

