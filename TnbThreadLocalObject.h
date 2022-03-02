#pragma once 
/**
 *@file
 * スレッドローカルオブジェクト管理関係のヘッダ
 *
 *		一つのインスタンスだが、別々のスレッドからアクセスしても
 *		干渉されないスレッドローカルなオブジェクトを管理するクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMap.h"



namespace TNB
{



/**@ingroup THREAD
 * スレッドローカルオブジェクトテンプレートクラス
 *	
 *		一つのインスタンスだが、別々のスレッドからアクセスしても
 *		干渉されないスレッドローカルなオブジェクトを管理します。
 *
 *	@note	作成できる数が一プロセス約1000個までとなっています。
 *			数を必要とする場合、 CThreadLocalObjectExT を使用します。
 *			ただし本クラスのほうが処理は高速です。
 *
 *	@note	int や DWORD などの基本型は、 CThreadLocalStorageT が使用できます。
 *			本クラスより高速です。
 *
 *	@param TYP スレッドローカルで管理したい型。コピー機能必須。
 *
 *	@par必要ファイル
 *			TnbThreadLocalObject.h
 *
 *	@date 06/10/23 新規
 */
template<typename TYP>
class CThreadLocalObjectT
{
	DWORD						m_dwTlsIndex;	///< TLS インデックス
	TYP							m_typ;			///< オブジェクトの初期値
	mutable CVectorT<TYP*>		m_vpTyp;		///< 確保オブジェクト群
	/// 値格納のポインタ取得
	TYP& m_Get(void) const
	{
		TYP* P = static_cast<TYP*>(::TlsGetValue(m_dwTlsIndex));
		if ( P == NULL )
		{
			P = new TYP(m_typ);
			m_vpTyp.Add(P);
			::TlsSetValue(m_dwTlsIndex, P);
		}
		return *P;
	}

public:

	/**
	 * コンストラクタ
	 *	@param t 値の初期状態。省略すると TYP() になります。
	 */
	CThreadLocalObjectT(const TYP& t = TYP()) : m_typ(t)
	{
		m_dwTlsIndex = ::TlsAlloc();
		if ( m_dwTlsIndex == TLS_OUT_OF_INDEXES )
		{
			throw CEmptyException();
		}
	}

	/// デストラクタ
	~CThreadLocalObjectT(void)
	{
		loop ( i, m_vpTyp.GetSize() )
		{
			delete m_vpTyp.At(i);
		}
		m_vpTyp.RemoveAll();
		::TlsFree(m_dwTlsIndex);
	}

	/**
	 * [取得] 値取得
	 *	@note Thread毎に管理が異なります。コンストラクタで指定した値か、同じスレッドで Set() した値が取れます。
	 *	@return 値。
	 */
	const TYP& Get(void) const
	{
		return m_Get();
	}

	/**
	 * [設定] 値設定
	 *	@note Thread毎に管理が異なります。設定しないと、コンストラクタで指定した値になっています。
	 *	@param t 値。
	 */
	void Set(const TYP& t)
	{
		m_Get() = t;
	}
};



/**@ingroup THREAD
 * 拡張スレッドローカルオブジェクトテンプレートクラス
 *	
 *		一つのインスタンスだが、別々のスレッドからアクセスしても
 *		干渉されないスレッドローカルなオブジェクトを管理します。
 *
 *	@note	MAPを使って管理するため、インスタンスの限界がありません（メモリの限り作成可能）。
 *			ただし CThreadLocalObjectT のほうが高速です。
 *
 *	@note	int や DWORD などの基本型は、 CThreadLocalStorageT が使用できます。
 *			本クラスより高速です。
 *
 *	@param TYP スレッドローカルで管理したい型。コピー機能必須。
 *
 *	@par必要ファイル
 *			TnbThreadLocalObject.h
 *
 *	@date 06/10/23 新規
 */
template<typename TYP>
class CThreadLocalObjectExT
{
	TYP							m_typ;			///< オブジェクトの初期値
	mutable CMapT<DWORD, TYP>	m_mapTyp;		///< 確保オブジェクト群
public:

	/**
	 * コンストラクタ
	 *	@param t 値の初期状態。省略すると TYP() になります。
	 */
	CThreadLocalObjectExT(const TYP& t = TYP()) : m_typ(t)
	{
	}

	/// デストラクタ
	~CThreadLocalObjectExT(void)
	{
	}

	/**
	 * [取得] 値取得
	 *	@note Thread毎に管理が異なります。コンストラクタで指定した値か、同じスレッドで Set() した値が取れます。
	 *	@return 値。
	 */
	const TYP& Get(void) const
	{
		EXCLUSIVE(&m_mapTyp);
		DWORD dw = ::GetCurrentThreadId();
		if ( ! m_mapTyp.HasKey(dw) )
		{
			m_mapTyp[dw] = m_typ;
		}
		return m_mapTyp[dw];
	}

	/**
	 * [設定] 値設定
	 *	@note Thread毎に管理が異なります。設定しないと、コンストラクタで指定した値になっています。
	 *	@param t 値。
	 */
	void Set(const TYP& t)
	{
		EXCLUSIVE(&m_mapTyp);
		DWORD dw = ::GetCurrentThreadId();
		if ( ! m_mapTyp.HasKey(dw) )
		{
			m_mapTyp[dw] = m_typ;
		}
		m_mapTyp[dw] = t;
	}
};



};



#if 0

DWORD gdwTlsIndex = 0;

CThreadLocalStorageT<DWORD> s_tsl_dwData;
CThreadLocalObjectT<CStr> s_tsl_strData = "#";



class Receiver : public CSocketUdp::IListener
{
public:
	virtual bool OnSocketEvent(const CSocketEvent& event)
	{
		DWORD r = s_tsl_dwData.Get();
		s_tsl_dwData.Set(::GetCurrentThreadId());
		TRACE2("[%08X] : %08X\n",::GetCurrentThreadId(), r);

		CStr s = s_tsl_strData.Get();
		s_tsl_strData.Set("Thread!!");
		TRACE2("[%08X] : %s\n",::GetCurrentThreadId(),s);




		if ( event.HasEvent() )
		{
			TRACE1("%s\n",event.GetEventString());
		}
		else
		{
			TNB::DumpData(event.GetDataSize(),event.GetData());
			CStr str;
			WORD wPort;
			event.GetPeerName(str, wPort);
			TRACE2("%s %d\n", str, wPort);
			event.Send(2,"AB");
		}
		return true;
	}
};



void CTnbTestDlg::OnBtnVir() 
{
	CDwordVector vdwIndex;
	while ( true )
	{
		DWORD d = ::TlsAlloc();
		if ( d == TLS_OUT_OF_INDEXES )
		{
			break;
		}
		vdwIndex.Add(d);
	}
	TRACE1("max= %d\n",vdwIndex.GetSize());




	DWORD r = s_tsl_dwData.Get();
	s_tsl_dwData.Set(10222);

	CStr s = s_tsl_strData.Get();
	s_tsl_strData.Set("Main");


#endif
