#pragma once 
/**
 *@file
 * スレッドローカル領域管理関係のヘッダ
 *
 *		一つのインスタンスだが、別々のスレッドからアクセスしても
 *		干渉されないスレッドローカルな領域を管理するクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#ifndef TLS_OUT_OF_INDEXES
 #define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)
#endif
#endif



/**@ingroup THREAD
 * スレッドローカル領域テンプレートクラス
 *	
 *		一つのインスタンスだが、別々のスレッドからアクセスしても
 *		干渉されないスレッドローカルな変数を管理します。
 *
 *	@note 各スレッドで使用する際、始めは必ず０になっています。指定できません。
 *
 *	@note 基本型以外は、 CThreadLocalStorageT などを使用します。
 *
 *	@param TYP サイズが sizeof(LPVOID) 以下の基本型変数。
 *
 *	@par必要ファイル
 *			TnbThreadLocalStorage.h
 *
 *	@date 06/10/23 新規
 */
template<typename TYP>
class CThreadLocalStorageT
{
	DWORD			m_dwTlsIndex;	///< TLS インデックス

public:

	/// コンストラクタ
	CThreadLocalStorageT(void)
	{
		ASSERT1( sizeof(LPVOID) >= sizeof(TYP),
				"CThreadLocalStorageT", "使用出来ない型(%s)が指定されています", typeid(TYP).name() );
		m_dwTlsIndex = ::TlsAlloc();
		if ( m_dwTlsIndex == TLS_OUT_OF_INDEXES )
		{
			throw CEmptyException();
		}
	}

	/// デストラクタ
	~CThreadLocalStorageT(void)
	{
		::TlsFree(m_dwTlsIndex);
	}

	/**
	 * [取得] 値取得
	 *	@note Thread毎に管理が異なります。０か、同じスレッドで Set() した値が取れます。
	 *	@return 値。
	 */
	TYP Get(void) const
	{
		return reinterpret_cast<TYP>(::TlsGetValue(m_dwTlsIndex));
	}

	/**
	 * [設定] 値設定
	 *	@note Thread毎に管理が異なります。設定しないと、値は０になっています。
	 *	@param t 値。
	 */
	void Set(const TYP& t)
	{
		::TlsSetValue(m_dwTlsIndex, reinterpret_cast<LPVOID>(t));
	}

	/**
	 * [取得] TLSインデックス取得
	 *	@note 通常、 TLSインデックスを意識する必要はありません。
	 *	@return TLSインデックス
	 */
	DWORD GetTlsIndex(void) const
	{
		return m_dwTlsIndex;
	}
};


#if 0
class CLastErrorManage
{
	CThreadLocalStorageT<BYTE> m_tlsError;	///< エラーコード

public:

	/// コンストラクタ
	CLastErrorManage(void)
	{
	}

	DWORD GetLastError(void) const
	{
		return m_tlsError.Get();
	}

	void SetLastError(DWORD dwError=::GetLastError())
	{
		m_tlsError.Set(dwError);
	}
};
#endif



}; //TNB






