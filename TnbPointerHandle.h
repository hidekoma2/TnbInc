#pragma once
/**
 *@file
 * ポインタハンドル関係のヘッダ
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		一つのポインタをハンドルします。 auto_ptr クラスのようなものです。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"
#pragma warning( disable : 4284 )



//TNB Library
namespace TNB
{



/**@ingroup MEMORY
 * ポインタハンドルテンプレートベースクラス
 *
 *	@param TYP 保持する型。
 *	@param DELE	破棄するクラスを指定します。
 *	@param NUL	TYPの空っぽの値を指定します。デフォルトは０です。
 *
 *	@par必要ファイル
 *			TnbPointerHandle.h
 *
 *	@date	06/09/29 抽象化。
 *	@date	07/01/24 NULの扱い、修正。０も値に出来るようにした。
 */
template<typename TYP, typename DELE, INT_PTR NUL = 0>
class CPointerHandleBaseT
{
	/// 管理用の構造体
	struct THead
	{
		LONG	refs;			///< 参照数
		TYP		type;			///< 管理するTYPポインタ
		/**
		 * コンストラクタ
		 *	@param t typeに設定する値
		 *	@param l refsに設定する値
		 */
		THead(TYP t = reinterpret_cast<TYP>(NUL), LONG l = 1) : refs(l) , type(t) 
		{
		}
	};

	THead*	m_ptHead;			///< THeadのアドレス @note NULLになることはありません

	/// NULL状態にする
	void m_SetHeadNull(void)
	{
		m_ptHead = reinterpret_cast<THead*>(g_plNullBase);
	}

	/// NULL状態か
	bool m_IsHeadNull(void)
	{
		return m_ptHead == reinterpret_cast<THead*>(g_plNullBase);
	}

	void m_Null(void)
	{
		if ( ! m_IsHeadNull() )
		{
			if ( ::InterlockedDecrement(&(m_ptHead->refs)) <= 0 )
			{
				if ( m_ptHead->type != reinterpret_cast<TYP>(NUL) )
				{
					DELE()(m_ptHead->type);
				}
				delete m_ptHead;
			}
			m_SetHeadNull();
		}
	}

public:

	/**
	 * コンストラクタ.
	 *		TYPポインタを管理していないインスタンスが作成されます。
	 */
	CPointerHandleBaseT(void)
	{
		m_SetHeadNull(); 
		if ( NUL != 0 ) { m_ptHead = new THead; }
	}

	/**
	 * 代入コンストラクタ.
	 *		指定されたTYPポインタを管理するインスタンスが作成されます。
	 *		この時点では、TYPポインタは新インスタンスだけが参照しています。
	 *	@param t 値
	 */
	CPointerHandleBaseT(TYP t)
	{
		m_SetHeadNull();
		operator=(t);
	}

	/**
	 * コピーコンストラクタ.
	 *		コピー元にTYPポインタがある場合、新規作成の本インスタンスでも、
	 *		そこを参照するようになります。
	 *	@param other 同じ型のインスタンス。
	 */
	CPointerHandleBaseT(const CPointerHandleBaseT& other)
	{
		m_SetHeadNull();
		operator=(other);
	}

	/**
	 * デストラクタ.
	 *	「TYPポインタを管理していたら、開放します。
	 *	ただし、TYPポインタが他からも参照されていたら、参照を一つ減らすだけです。
	 */
	~CPointerHandleBaseT(void)
	{
		m_Null(); 
	}

	/**
	 * [代入] 代入.
	 *		本インスタンスで管理していたTYPポインタを開放し、
	 *		引数のTYPポインタを、本インスタンスで管理するようになります。
	 *	@param t 値
	 *	@return 自分の参照。
	 */
	CPointerHandleBaseT& operator=(TYP t)
	{
		if ( m_ptHead->type != t )
		{
			if ( t != reinterpret_cast<TYP>(NUL) )
			{
				m_Null();
				m_ptHead = new THead(t);
			}
			else
			{
				Null();
			}
		}
		return *this;
	}

	/**
	 * [代入] コピーオペレータ.
	 *		本インスタンスで管理していたTYPポインタを開放し、
	 *		コピー元にTYPポインタがある場合、本インスタンスでも、
	 *		そこを参照するようになります。
	 *	@param other 同じ型のインスタンス。
	 *	@return 自分の参照。
	 */
	CPointerHandleBaseT& operator=(const CPointerHandleBaseT& other)
	{
		if ( m_ptHead != other.m_ptHead )
		{
			if ( other.IsNull() )
			{
				//t は空。
				Null();
			}
			else if ( InterlockedCompareExchange(&(other.m_ptHead->refs), 2, 1) == 1 )
			{
				//前が１だったら２にして、新しくバッファアドレスを共有
				m_Null();
				m_ptHead = other.m_ptHead;
			}
			else if ( (other.m_ptHead->refs) >= 1 )
			{
				//前が１以上だったら＋１して、新しくバッファアドレスを共有
				m_Null();
				other.m_ptHead->refs++;	// 参照数をプライス１
				m_ptHead = other.m_ptHead;
			}
		}
		ASEERT_NULLBASE;
		return *this;
	}

	/**
	 * [確認] NULLチェック
	 *	@retval true TYPポインタ無し
	 *	@retval false TYPポインタ有り
	 */
	bool IsNull(void) const 
	{
		return m_ptHead->type == reinterpret_cast<TYP>(NUL);
	}

	/**
	 * [設定] 参照数設定
	 *	@note	通常使う必要は有りません。例外として、0以下の参照数は、参照を増やすことが
	 *			出来ない様になっているため、意図的に使用することが出来ます。
	 *	@param iCount 設定する参照数
	 *	@return 設定前の参照数
	 */
	int SetReferCount(int iCount)
	{
		if ( m_IsHeadNull() ){ return 0; }
		return ::InterlockedExchange(&(m_ptHead->refs), iCount);
	}

	/**
	 * [取得] 参照数取得.
	 *		TYPポインタが参照されている数を知ることが出来ます。
	 *	@note	通常使う必要は有りません。
	 *	@return 参照数
	 */
	int GetReferCount(void) const { return m_ptHead->refs; }

	/**
	 * [設定] 開放.
	 *		本インスタンスが管理しているTYPを開放します。
	 *	@note	operator=(NUL) と同じ動作です。
	 *	@note TYPが他からも使われていたら、参照を一つ減らすだけです。
	 */
	void Null(void)
	{
		m_Null();
		if ( NUL != 0 )
		{
			m_ptHead = new THead;
		}
	}

	/**
	 * [取得] TYPのポインタ取得.
	 *	@return TYPのポインタ
	 */
	TYP* ReferP(void) { return &(m_ptHead->type); }

	/**
	 * [取得] TYPのポインタ取得.
	 *	@return TYPのポインタ
	 */
	TYP const* ReferP(void) const { return &(m_ptHead->type); }

	/**
	 * [取得] TYP取得.
	 *		管理しているTYPを返します
	 *	@return TYP	の値
	 */
	operator TYP(void) { return m_ptHead->type; }

	/**
	 * [取得] TYP取得.
	 *		管理しているTYPを返します
	 *	@return TYP	の値
	 */
	operator const TYP(void) const { return m_ptHead->type; }

	/**
	 * [取得] TYP取得.
	 *		管理しているTYPを返します
	 *	@return TYP	の値
	 */
	TYP operator->(void) { return m_ptHead->type; }

	/**
	 * [取得] TYP取得.
	 *		管理しているTYPを返します
	 *	@return TYP	の値
	 */
	const TYP operator->(void) const { return m_ptHead->type; }

	/**
	 * [比較] TYP比較.
	 *		保持している TYPと比較します。
	 *	@param t 比較対照
	 *	@retval true 同じ
	 *	@retval false 異なる
	 */
	bool operator==(TYP t) const { return m_ptHead->type == t; }

	/**
	 * [比較] TYP比較.
	 *		保持している TYPと比較します。
	 *	@param t 比較対照
	 *	@retval false 同じ
	 *	@retval true 異なる
	 */
	bool operator!=(TYP t) const { return ! operator==(t); }

private:
	friend class CPointerHandleTest;	///< フレンドクラス宣言
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// PointerHandleBase用 シングルポインタDestoryクラス
struct TPhDeleteSinglePtr
{
	template<typename TYP> void operator()(TYP* P) { delete P; }
};
/// PointerHandleBase用 シングルポインタDestoryクラス
struct TPhDeleteArrayPtr
{
	template<typename TYP> void operator()(TYP* P) { delete[] P; }
};

#endif // _TnbDOXYGEN



/**@ingroup MEMORY
 * ポインタ型ハンドルテンプレート
 *
 *		この型に new で確保したポインタを渡しておくと、
 *		「領域」を参照する本クラスががなくなり次第、
 *		自動的に開放します。
 *
 *		「Javaの参照型」や「C++/CLRのハンドル型」と
 *		同じ使い方が可能になります。
 *
 *		コピーオペレータを持っているので、関数の戻り値などに
 *		も使用できます。
 *
 *	@note 推奨prefixは「hp」です。
 *
 *	@param TYP 保持する型
 *
 *	@par必要ファイル
 *			TnbPointerHandle.h
 *
 *	@date	04/11/02 新規作成
 *	@date	06/04/24 テンプレート化
 *	@date	06/05/18 スレッドセーブ化
 *	@date	06/05/20 ReferP()、追加。
 *	@date	06/09/05 Empty -> Null()に変更。New()、追加。
 *	@date	06/09/29 ベースクラスから作成するように変更。
 */
template<typename TYP>
class CPointerHandleT : public CPointerHandleBaseT<TYP*, TPhDeleteSinglePtr>
{
	typedef CPointerHandleBaseT<TYP*, TPhDeleteSinglePtr> _super;	///< 親クラス名再宣言
	DEFSUPER(_super);
public:

	/**
	 * コンストラクタ.
	 *		TYPポインタを管理していないインスタンスが作成されます。
	 */
	CPointerHandleT(void) : _super() 
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		指定されたTYPポインタを管理するインスタンスが作成されます。
	 *		この時点では、TYPポインタは新インスタンスだけが参照しています。
	 *	@param P new で確保したTYPポインタ。
	 */
	CPointerHandleT(TYP* P) : _super(P) 
	{
	}

	/**
	 * コピーコンストラクタ.
	 *		コピー元にTYPポインタがある場合、新規作成の本インスタンスでも、
	 *		そこを参照するようになります。
	 *	@param t 同じ型のインスタンス。
	 */
	CPointerHandleT(const CPointerHandleT& t) : _super(t) 
	{
	}

	/**
	 * [取得] TYPポインタ取得.
	 *		管理しているTYPポインタを返します
	 *	@retval NUL	 管理していない
	 *	@retval NUL以外 TYPポインタ
	 */
	TYP& operator*(void) { return **_super::ReferP(); }

	/**
	 * [取得] TYPポインタ取得.
	 *		管理しているTYPポインタを返します
	 *	@retval NUL	 管理していない
	 *	@retval NUL以外 TYPポインタ
	 */
	const TYP& operator*(void) const { return **_super::ReferP(); }
};



/**@ingroup MEMORY
 * ポインタ型ハンドルテンプレート
 *
 *		この型に new で確保したポインタを渡しておくと、
 *		「領域」を参照する本クラスががなくなり次第、
 *		自動的に開放します。
 *
 *		「Javaの参照型」や「C++/CLRのハンドル型」と
 *		同じ使い方が可能になります。
 *
 *		コピーオペレータを持っているので、関数の戻り値などに
 *		も使用できます。
 *
 *		@par
 *			<table><tr><td><pre>
 *			
 *			CArrayPtrHandleT<BYTE> foo(DWORD dwSize)
 *			{
 *			    CArrayPtrHandleT<BYTE> hpBuff = new BYTE[dwSize];
 *			    DWORD dwReadBytes = 0;
 *			    BOOL boRc = ::ReadFile(m_hFile,hpBuff,dwSize,&dwReadBytes,NULL);		
 *			    if ( ! boRc ){ hpBuff.Empty(); }
 *			    return hpBuff;
 *			}
 *			</pre></td></tr></table>
 *
 *	@note 推奨prefixは「hp」です。
 *
 *	@param TYP 保持する型
 *
 *	@par必要ファイル
 *			TnbPointerHandle.h
 *
 *	@date	06/09/29 別クラスとして定義。
 */
template<typename TYP>
class CArrayPtrHandleT : public CPointerHandleBaseT<TYP*, TPhDeleteArrayPtr>
{
	typedef CPointerHandleBaseT<TYP*, TPhDeleteArrayPtr> _super;	///< 親クラス名再宣言
	DEFSUPER(_super);
public:

	/**
	 * コンストラクタ.
	 *		TYPポインタを管理していないインスタンスが作成されます。
	 */
	CArrayPtrHandleT(void) : _super() 
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		指定されたTYPポインタを管理するインスタンスが作成されます。
	 *		この時点では、TYPポインタは新インスタンスだけが参照しています。
	 *	@param P new で確保したTYPポインタ。
	 */
	CArrayPtrHandleT(TYP* P) : _super(P) 
	{
	}

	/**
	 * コピーコンストラクタ.
	 *		コピー元にTYPポインタがある場合、新規作成の本インスタンスでも、
	 *		そこを参照するようになります。
	 *	@param t 同じ型のインスタンス。
	 */
	CArrayPtrHandleT(const CArrayPtrHandleT& t) : _super(t) 
	{
	}

	/**
	 * [取得] TYPポインタ取得.
	 *		管理しているTYPポインタを返します
	 *	@param ind インデックス。
	 *	@retval NUL	 管理していない
	 *	@retval NUL以外 TYPポインタ
	 */
	TYP& operator[](int ind) { return (*_super::ReferP())[ind]; }

	/**
	 * [取得] TYPポインタ取得.
	 *		管理しているTYPポインタを返します
	 *	@param ind インデックス。
	 *	@retval NUL	 管理していない
	 *	@retval NUL以外 TYPポインタ
	 */
	const TYP& operator[](int ind) const { return (*_super::ReferP())[ind]; }
};



};//TNB
