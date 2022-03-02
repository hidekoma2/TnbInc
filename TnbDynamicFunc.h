#pragma once
/**
 *@file
 * 動的関数管理関係のヘッダ
 *
 *		DLL内の関数を動的に呼び出し実行することができるクラスが有ります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup ETC
 * 動的関数管理クラステンプレート
 *
 *		DLL内の関数を動的に呼び出し実行することが容易にできます。
 *
 *	@param FUNC 関数型。それ以外は保証外。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *
 *		// 関数の型を宣言
 *		typedef LONG (WINAPI* P_InterlockedCompareExchange)(LONG*, LONG, LONG);
 *		// 動的管理クラス宣言＆関数読み込み
 *		CDynamicFuncT<P_InterlockedCompareExchange> func1("kernel32.dll", "InterlockedCompareExchange");		
 *		// 実行
 *		LONG l1 = 0;
 *		LONG l2 = func1()(&l1, 1, 0); //空括弧を一つはさむのがポイント		
 *	
 *		// 宣言と呼び出しを一行で行なうことも出来ます。
 *		typedef HANDLE (WINAPI* P_IcmpCreateFile)(VOID);
 *		HANDLE h = CDynamicFuncT<P_IcmpCreateFile>("ICMP.dll", "IcmpCreateFile")()();
 *		</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbDynamicFunc.h
 *
 *	@date 06/09/21 新規作成
 *	@date 08/01/29 LoadFunc 追加
 */
template<typename FUNC>
class CDynamicFuncT : CCopyImpossible
{
	HMODULE	m_hModule;			///< モジュールハンドル
	FUNC	m_pFunc;			///< 関数アドレス
	void m_Unload(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
		m_pFunc	= NULL;
	}
public:

	/**
	 * コンストラクタ
	 *	@note LoadFunc() を使い、使用する関数を設定する必要があります。
	 */
	CDynamicFuncT(void) : m_hModule(NULL), m_pFunc(NULL)
	{
	}

	/**
	 * コンストラクタ
	 *	@param lpszDllName コールしたい関数の含まれるDll名。
	 *	@param lpszFuncName コールしたい関数名。
	 *	@throw CNullPointerException 指定の関数が見つからない時にスローされます。
	 */
	CDynamicFuncT(LPCSTR lpszDllName, LPCSTR lpszFuncName)
		: m_hModule(NULL), m_pFunc(NULL)
	{
		if ( ! LoadFunc(lpszDllName, lpszFuncName) )
		{
			throw CNullPointerException();
		}
	}

	/**
	 * [設定] 関数読込み.
	 *	@param lpszDllName コールしたい関数の含まれるDll名。
	 *	@param lpszFuncName コールしたい関数名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool LoadFunc(LPCSTR lpszDllName, LPCSTR lpszFuncName)
	{
		m_Unload();
		m_hModule = ::LoadLibraryA(lpszDllName);
		if ( m_hModule != NULL )
		{
			m_pFunc = reinterpret_cast<FUNC>(::GetProcAddressA(m_hModule, lpszFuncName));
		}
		return (m_pFunc != NULL);
	}

	/**
	 * デストラクタ
	 */
	~CDynamicFuncT(void)
	{
		m_Unload();
	}

	/**
	 * [確認] 関数アドレスを持っているか
	 *	@retval true 持っている
	 *	@retval flase もっていない。
	 */
	bool HasFunc(void) const
	{
		return (m_pFunc != NULL);
	}

	/**
	 * [取得] 関数アドレス取得.
	 *	@note HasFunc() の戻り値が true の時、成功します。
	 *	@return 関数アドレス
	 *	@throw CNullPointerException 指定の関数が見つからない時にスローされます。
	 */
	FUNC operator()(void) const
	{
		if ( m_pFunc == NULL )
		{
			throw CNullPointerException();
		}
		return m_pFunc;
	}
};



/**@ingroup ETC
 * 動的関数アドレス取得関数
 *
 *	@param[out] _pFnc FUNC関数型の実体（ポインタ）。
 *	@param[in] hInstance インスタンスハンドル。
 *	@param[in] lpszName 関数名。
 *	@retval true 取得成功
 *	@retval false 失敗
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *
 *		// 関数の型とポインタを宣言
 *		LONG (*pGetData)(LONG);
 *		// 動的管理クラス宣言＆関数読み込み
 *		GetFuncAddress(pGetData, hModule, "GetData");		
 *		// 実行
 *		LONG l = pGetData(0);		
 *		</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbDynamicFunc.h
 *
 *	@date 06/12/08 新規作成
 */
template<typename FUNC>
bool GetFuncAddress(FUNC& _pFnc, HINSTANCE hInstance, LPCSTR lpszName)
{
	_pFnc = reinterpret_cast<FUNC>(::GetProcAddressA(hInstance, lpszName));
	return _pFnc != NULL;
}



}; // TNB
