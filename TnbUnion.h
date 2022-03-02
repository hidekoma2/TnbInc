#pragma once
/**
 *@file
 * UNION関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * UNION管理クラス
 *
 *		Set() メソッドにより、どんな型の値でも格納しておくことが出来ます。
 *		Get() メソッドで取り出す時は、 Set() した時と同じ型である必要があります。
 *
 *	@note インスタンスを複製しても、型の実態は一つなので、変更すると、複製した
 *			インスタンス、全部に影響があります。
 *
 *	@par使用例
 *		\code
 *		int i = 99;
 *		CStr s = "A";
 *		CString ss ="X"; 
 *		CUnion u(i);
 *		CUnion u2(u);
 *		TRACE1("%s\n", u.GetTypeName());
 *		TRACE1("%s\n", u2.GetTypeName());
 *		u.Set(ss);
 *		TRACE1("%s\n", u.GetTypeName());
 *		u.Set(s);
 *		u.Set(s);
 *		VERIFY( u.Get(s) );
 *		VERIFY( ! u2.Get(s) );
 *		TRACE1("%s\n", u.GetTypeName());
 *		TRACE1("%s\n", u2.GetTypeName());
 *		TRACE1("%d\n", u.Ref((CStr*)0).GetLength());
 *		TRACE1("%s\n", u.Ref((CStr*)0));
 *		TRACE1("%d\n", u2.Ref((int*)0));
 *		u.Set(i);
 *		u.Get(i);
 *		TRACE1("%s\n", u.GetTypeName());
 *		TRACE1("%s\n", u2.GetTypeName());
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbUnion.h
 *
 *	@date 06/09/25 新規
 *	@date 07/01/25 Releaseビルドの場合、型判定をサイズのみにした。
 *	@date 07/02/28 ToString のIFを用意。
 */
class CUnion
{
public:

	///	UNION管理クラス用ファンクションインターフェース
	struct IFuncBase
	{
		/// デストラクタ
		virtual ~IFuncBase(void) {}
		/// デストラクト実行
		virtual void Destractor(LPVOID V) = 0;
	};


	//-----------------


	/** 
	 * コンストラクタ.
	 *	何も保持しないインスタンス作成。
	 */
	CUnion(void) : m_hpHead(new THead) 
	{
	}

	/**
	 * コンストラクタ.
	 *	T 型で t の値を持つインスタンス作成。
	 *	@note T 型はコピー機能が必要です。
	 *	@param t 代入値
	 *	@param dwParam パラメータ
	 */
	template<typename T>
	CUnion(const T& t, DWORD dwParam = 0) : m_hpHead(new THead)
	{
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/** 
	 * コピーコンストラクタ.
	 *	@param other コピー元
	 */
	CUnion(const CUnion& other) : m_hpHead(other.m_hpHead)
	{
	}

	/// デストラクタ
	~CUnion(void)
	{
		Empty();
	}

	/** 
	 * [代入] コピーオペレータ.
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CUnion& operator=(const CUnion& other)
	{
		m_hpHead = other.m_hpHead;
		return *this;
	}

	/**
	 * [設定] 空化.
	 *	何も保持しないインスタンス化
	 */
	void Empty(void)
	{
		m_hpHead = new THead;
	}

	/**
	 * [設定] 代入.
	 *	T 型で t の値を持つインスタンスになります。
	 *	@note T 型はコピー機能が必要です。
	 *	@attention	同じ型で代入してもメモリの再確保が発生します。
	 *				本クラスをワークとしては使用しないほうが良いです。
	 *	@param t 代入値
	 *	@param dwParam パラメータ
	 */
	template<typename T>
	void Set(const T& t, DWORD dwParam = 0)
	{
		m_hpHead = new THead;
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/**
	 * [取得] パラメータ取得.
	 *	@return パラメータ
	 */
	DWORD GetParam(void) const
	{
		return m_hpHead->m_dwParam;
	}

	/**
	 * [取得] 取得.
	 *	T 型で t の値を持つインスタンスから、値を得ます。。
	 *	@note T 型は、代入コンストラクト / Set() した時と同じ型である必要があります。
	 *	@param[out] _t 取得オブジェクト
	 *	@retval true 成功
	 *	@retval false 失敗(型が異なる)
	 */
	template<typename T>
	bool Get(T& _t) const
	{
		#ifdef _DEBUG
			if ( strcmp(m_hpHead->m_strType, typeid(T).name()) != 0 )
			{
				return false;	//型が異なる
			}
		#endif
		if ( m_hpHead->m_dwSizeof != sizeof(T) )
		{
			return false;	//型が異なる
		}
		_t = *(static_cast<const T*>(m_hpHead->m_pVal));
		return true;
	}

	/**
	 * [取得] 参照取得.
	 *	T 型で t の値を持つインスタンスの持つ値の参照を得ます。。
	 *	@note T 型は、代入コンストラクト / Put() した時と同じ型である必要があります。
	 *	@param P 取得したい型のポインタを指定してください（アクセスしないのでNULLで構いません）。
	 *	@throw CNoSuchTypeException インスタンスが持つ型と T 型が異なる場合、スローされます。
	 *	@return 参照
	 */
	template<typename T>
	const T& Ref(T* P) const 
	{
		#ifdef _DEBUG
			if ( strcmp(m_hpHead->m_strType, typeid(T).name()) != 0 )
			{
				throw CNoSuchTypeException();	//型が異なる
			}
		#endif
		if ( m_hpHead->m_dwSizeof != sizeof(T) )
		{
			throw CNoSuchTypeException();	//型が異なる
		}
		return *(static_cast<const T*>(m_hpHead->m_pVal));
	}

	/**
	 * [取得] 型名取得.
	 *	@note DEBUG ビルドの時のみ有効です。
	 *	@return インスタンスが保持している型名を返します。
	 */
	LPCSTR GetTypeName(void) const
	{
		#ifdef _DEBUG
			return m_hpHead->m_strType;
		#else
			return "";
		#endif
	}

protected:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	/// 値管理クラス
	struct THead
	{
		IFuncBase*		m_pFuncBase;	///< 保持している型のファンクション
		LPVOID			m_pVal;			///< 保持している型のポインタ
		DWORD			m_dwSizeof;		///< 保持している型のサイズ
		DWORD			m_dwParam;		///< 汎用パラメータ値
		#ifdef _DEBUG
			CSimpleAscii m_strType;		///< 保持している型の名前
		#endif
		/// コンストラクタ
		THead(void) : m_pFuncBase(NULL), m_pVal(NULL), m_dwSizeof(0), m_dwParam(0)
		{
		}
		/// デストラクタ
		~THead(void)
		{
			if ( m_pFuncBase != NULL )
			{
				m_pFuncBase->Destractor(m_pVal);
				delete m_pFuncBase;
				m_pFuncBase = NULL;
			}
		}
		/// セット
		template<typename T>
		void Set(const T& t, DWORD dwParam, IFuncBase* P = NULL)
		{
			m_pFuncBase		= P;
			m_pVal			= new T(t);
			m_dwParam		= dwParam;
			m_dwSizeof		= sizeof(T);
			#ifdef _DEBUG
				m_strType	= typeid(T).name();
			#endif
		}
	};

	///	デストラクト用クラス
	template<typename T>
	struct CFuncT : IFuncBase
	{
		virtual void Destractor(LPVOID V)
		{
			delete static_cast<T*>(V);
		}
	};

	#endif // _TnbDOXYGEN

	CPointerHandleT<THead> m_hpHead;	///< 実体参照ハンドル
};



/**@ingroup VALUE
 * UNION管理拡張クラス
 *
 *		Set() メソッドにより、どんな型(ToString()メソッドを持つ型に限る)の値でも格納しておくことが出来ます。
 *		Get() メソッドで取り出す時は、 Set() した時と同じ型である必要があります。
 *		ToString() メソッドで文字列として取り出せます。
 *	
 *	@note CUnion とは異なり、格納できる型は ToString() を持つ型のみになります。
 *
 *	@par必要ファイル
 *			TnbUnion.h
 *
 *	@date 07/02/28 新規
 */
class CUnionEx : public CUnion
{
	DEFSUPER(CUnion);
public:

	///	UNION管理拡張クラス用ファンクションインターフェース
	struct IFuncBaseEx : CUnion::IFuncBase
	{
		/// 文字列へ変換
		virtual void ToString(CSimpleStr& _s, LPVOID V) = 0;
	};


	//------------


	/** 
	 * コンストラクタ.
	 *	何も保持しないインスタンス作成。
	 */
	CUnionEx(void) : _super() 
	{
	}

	/**
	 * コンストラクタ.
	 *	T 型で t の値を持つインスタンス作成。
	 *	@note T 型はコピー機能が必要です。
	 *	@param t 代入値
	 *	@param dwParam パラメータ
	 */
	template<typename T>
	CUnionEx(const T& t, DWORD dwParam = 0) : m_hpHead(new THead)
	{
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/** 
	 * コピーコンストラクタ.
	 *	@param other コピー元
	 */
	CUnionEx(const CUnionEx& other) : _super(other)
	{
	}

	/**
	 * [設定] 代入.
	 *	T 型で t の値を持つインスタンスになります。
	 *	@note T 型はコピー機能が必要です。
	 *	@attention	同じ型で代入してもメモリの再確保が発生します。
	 *				本クラスをワークとしては使用しないほうが良いです。
	 *	@param t 代入値
	 *	@param dwParam パラメータ
	 */
	template<typename T>
	void Set(const T& t, DWORD dwParam = 0)
	{
		m_hpHead = new THead;
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/**
	 * [取得] 文字列取得
	 *	@param[out] _s 取得文字
	 *	@throw CNullPointerException 何も代入していないときに使用するとスローされます。
	 */
	void ToString(CSimpleStr& _s) const
	{
		if ( m_hpHead->m_pFuncBase == NULL )
		{
			throw CNullPointerException();
		}
		reinterpret_cast<IFuncBaseEx*>(m_hpHead->m_pFuncBase)->ToString(_s, m_hpHead->m_pVal);
	}

private:
	///	ファンクションクラス
	template<typename T>
	struct CFuncT : CUnionEx::IFuncBaseEx
	{
		virtual void Destractor(LPVOID V)
		{
			delete static_cast<T*>(V);
		}
		virtual void ToString(CSimpleStr& _s, LPVOID V)
		{
			_s = static_cast<T*>(V)->ToString();
		}
	};
};



};//TNB



#if 0
	int i = 99;
	CStr s = "A";
	CString ss ="X"; 
	CUnion u(i);
	CUnion u2(u);
	TRACE1("%s\n",u.GetTypeName());
	TRACE1("%s\n",u2.GetTypeName());
	u.Put(ss);
	TRACE1("%s\n",u.GetTypeName());
	u.Put(s);
	u.Put(s);
	VERIFY( u.Get(s) );
	VERIFY( ! u2.Get(s) );
	TRACE1("%s\n",u.GetTypeName());
	TRACE1("%s\n",u2.GetTypeName());
	TRACE1("%d\n",u.Ref((CStr*)0).GetLength());
	TRACE1("%s\n",u.Ref((CStr*)0));
	TRACE1("%d\n",u2.Ref((int*)0));
	u.Put(i);
	u.Get(i);
	TRACE1("%s\n",u.GetTypeName());
	TRACE1("%s\n",u2.GetTypeName());
#endif
