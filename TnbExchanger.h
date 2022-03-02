#pragma once
/**
 *@file
 * スレッド間値入れ替え関係のヘッダ
 *
 *		スレッド間で任意の型同士の入れ替えを行なうクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"



namespace TNB
{



/**@ingroup SYNC
 * スレッド間値入れ替えテンプレートクラス.
 *
 *		スレッド間で任意の型同士の入れ替えが容易にできます。
 *
 *	@note TYP を BYTE などにして、単純に二つのスレッドの同期を取ることにも使用可能です。
 *
 *	@param TYP 要素の型。コピー機能を持っている必要があります。
 *
 *	@par必要ファイル
 *			TnbExchanger.h
 *
 *	@date 06/09/19 新規
 */
template<typename TYP>
class CExchangerT
{
	LONG			m_lCount;		///< 進入カウンタ
	CSyncSemaphore	m_syncSema;		///< 進入排他
	CSyncEvent		m_syncEvent1;	///< ブロック排他１
	CSyncEvent		m_syncEvent2;	///< ブロック排他２
	volatile TYP	m_tParam1st;	///< 交換パラメータ値１
	volatile TYP	m_tParam2nd;	///< 交換パラメータ値２

public:

	/// コンストラクタ
	CExchangerT(void)
		: m_lCount(0), m_syncSema(NULL, 2), m_syncEvent1(false, true), m_syncEvent2(false, true)
	{
	}

	/**
	 * [取得] 値交換.
	 *	@note	別スレッドで、同オブジェクトの 本メソッドがコールされるまで、ブロックされます。
	 *			別スレッドでコールされると、そこで指定した値が本メソッドの戻り値になります。
	 *	@param t 値。
	 *	@return 交換した値。
	 */
	TYP Exchange(const TYP& t)
	{
		EXCLUSIVE(&m_syncSema);
		TYP r;
		LONG l = ::InterlockedIncrement(&m_lCount);
		if ( l == 1 )
		{
			m_tParam1st = t;
			m_syncEvent2.Lock();
			m_syncEvent1.Unlock();
			r = m_tParam2nd;
		}
		else
		{
			m_lCount = 0;
			m_tParam2nd = t;
			m_syncEvent2.Unlock();
			m_syncEvent1.Lock();
			r = m_tParam1st;
		}
		return r;
	}
};



}; // TNB
