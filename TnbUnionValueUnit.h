#pragma once
/**
 *@file
 * UNIONバリュー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbUnion.h"
#include "TnbValueUnit.h"
#include "TnbValueParUnit.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUEUNIT
 * UNION 単位付値管理クラス
 *
 *		Set() メソッドにより、どんな型の値でも格納しておくことが出来ます。
 *		Get() メソッドで取り出す時は、 Set() した時と同じ型である必要があります。
 *	
 *	@note コピーコンストラクタ、コピーオペレータが使用可能です。
 *
 *	@par必要ファイル
 *			TnbUnionValueUnit.h
 *
 *	@date 07/01/30 新規
 */
class CUnionValueUnit
{
public:

	///	ファンクションベースクラス
	struct IFuncBaseVu : CUnion::IFuncBase
	{
		/// 数値取得
		virtual CRealNumber GetValue(LPVOID V) const = 0;
		/// 単位記号取得
		virtual CStr GetUnitMarkString(LPVOID V) const = 0;
	};

private:

	/// 専用Unionクラス
	class CUnionVu : public CUnion
	{
		DEFSUPER(CUnion);
		///	ファンクションクラス
		template<typename T>
		struct CFuncT : CUnionValueUnit::IFuncBaseVu
		{
			virtual void Destractor(LPVOID V)
			{
				delete static_cast<T*>(V);
			}
			virtual CRealNumber GetValue(LPVOID V) const
			{
				return static_cast<T*>(V)->GetValue();
			}
			virtual CStr GetUnitMarkString(LPVOID V) const
			{
				return static_cast<T*>(V)->GetUnitMarkString();
			}
		};
	public:
		/// コンストラクタ.
		CUnionVu(void) : _super() {}
//		/**
//		 * コンストラクタ.
//		 *	T 型で t の値を持つインスタンス作成。
//		 */
//		template<typename T>
//		CUnionVu(const T& t, DWORD dwParam=0) : m_hpHead(new THead)
//		{
//			m_hpHead->Set(t, dwParam, new CFuncT<T>);
//		}
		/// コピーコンストラクタ.
		CUnionVu(const CUnionVu& other) : _super(other) {}
		/// 代入.
		template<typename T>
		void Set(const T& t, DWORD dwParam = 0)
		{
			m_hpHead = new THead;
			m_hpHead->Set(t, dwParam, new CFuncT<T>);
		}
		/// 値取得
		CRealNumber GetValue(void) const
		{
			return reinterpret_cast<IFuncBaseVu*>(m_hpHead->m_pFuncBase)->GetValue(m_hpHead->m_pVal);
		}
		/// 単位記号取得
		CStr GetUnitMarkString(void) const
		{
			return reinterpret_cast<IFuncBaseVu*>(m_hpHead->m_pFuncBase)->GetUnitMarkString(m_hpHead->m_pVal);
		}
	};

	CUnionVu	m_uni;
	CStr		m_strFormat;
	CStr		m_strInvalid;

public:

	/// コンストラクタ
	CUnionValueUnit(void) : m_uni()
	{
	}

//	/*
//	 * コンストラクタ
//	 *	@note T 型は ValueUnit 系クラス（CAbstractUnitのサブクラス）に限ります。
//	 *	@param t 代入値
//	 */
//	template<typename T>
//	CUnionValueUnit(const T& t) : m_uni(t, T::KINDCODE)
//	{
//		ASSERT( T::TNBLIB_VALUEUNIT == -1 );
//	}

	/// デストラクタ
	~CUnionValueUnit(void)
	{
	}

	/**
	 * [設定] 空化.
	 *	何も保持しないインスタンス化
	 */
	void Empty(void)
	{
		m_uni.Empty();
	}

	/**
	 * [設定] 代入.
	 *	T 型で t の値を持つインスタンスになります。
	 *	@note T 型は ValueUnit 系クラス（CAbstractUnitのサブクラス）に限ります。
	 *	@param t 代入値。
	 *	@param lpszFormat 数値部分のフォーマット。"%f" が含まれている必要があります。
	 *	@param lpszInvalid 数字が異常の場合の表示文字。
	 */
	template<typename T>
	void Set(const T& t, LPCTSTR lpszFormat = NULL, LPCTSTR lpszInvalid = NULL)
	{
		ASSERT( T::TNBLIB_VALUEUNIT == -1 );
		m_uni.Set(t, T::KINDCODE);
		m_strFormat = lpszFormat;
		m_strInvalid = (lpszInvalid == NULL ) ? _T("?") : lpszInvalid;
	}

	/**
	 * [取得] 単位種コード取得.
	 *	@return 単位種コード
	 */
	DWORD GetUnitKindCode(void) const
	{
		return m_uni.GetParam();
	}

	/**
	 * [取得] 取得.
	 *	T 型で t の値を持つインスタンスから、値を得ます。。
	 *	@note T 型は、代入コンストラクト / Put() した時と同じ型である必要があります。
	 *	@param[out] _t 取得オブジェクト
	 *	@retval true 成功
	 *	@retval false 失敗(型が異なる)
	 */
	template<typename T>
	bool Get(T& _t) const
	{
		if ( T::KINDCODE != m_uni.GetParam() )
		{
			return false;	//型が異なる
		}
		return m_uni.Get(_t);
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
		if ( T::KINDCODE != m_uni.GetParam() )
		{
			throw CNoSuchTypeException();	//型が異なる
		}
		return m_uni.Ref(P);
	}

	/**
	 * [取得] 文字列取得
	 *	@return 文字列。
	 */
	CStr ToString(void) const
	{
		CStr strRet;
		CRealNumber r = GetValue();
		if ( ! r.IsEmpty() )
		{
			CStr s = r.IsValid() ? r.ToString(m_strFormat) : m_strInvalid;
			strRet = CStr::Fmt(_T("%s %s"), s, m_uni.GetUnitMarkString());
		}
		return strRet;
	}

	/**
	 * [取得] 値取得
	 *	@return 値。
	 */
	CRealNumber GetValue(void) const
	{
		return m_uni.GetValue();
	}

	/**
	 * [取得] 単位記号取得
	 *	@return 文字列。
	 */
	CStr GetUnitMarkString(void) const
	{
		return m_uni.GetUnitMarkString();
	}
};



};//TNB



