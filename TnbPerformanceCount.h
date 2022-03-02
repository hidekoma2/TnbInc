#pragma once
/**
 *@file
 * 経過時間管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * 経過時間管理（高分解能）
 *
 *		時間の経過を扱うクラスです。
 *
 *		API の QueryPerformanceCounter() を使用しています。
 * 
 *	@attention 本クラスで使用している API は CPU クロックが精度に影響します。
 *			また、動的に CPU クロックが変更される可能性もあるため、正確な時間にならない可能性が有ります。
 *
 *	@par必要ファイル
 *			TnbPerformanceCount.h
 *
 *	@date 11/08/24	新規作成
 */
class CPerformanceCount
{
public:

	/**
	 * コンストラクタ
	 *	@note 現在の時間を記憶します。
	 */
	CPerformanceCount(void)
	{
		LARGE_INTEGER li;
		m_isValid = !! ::QueryPerformanceFrequency(&m_freq);
		m_isValid &= !! ::QueryPerformanceCounter(&m_count);
		::QueryPerformanceCounter(&li);
		m_apiCount = li.QuadPart - m_count.QuadPart;
		ASSERT0( m_isValid, "CPerformanceCount", "PerformanceCount が使用できません." );
		//TRACE2("PerformanceCount Resolution= %d(ns), %d\n", GetResolutionNanoCount(), static_cast<DWORD>(m_apiCount) );
		::QueryPerformanceCounter(&m_count);
		m_count.QuadPart += m_apiCount;
	}

	/**
	 * [確認] 有効？
	 *	@retval true 本インスタンスは有効。
	 *	@retval false 無効。
	 */
	bool IsValid(void) const
	{
		return m_isValid;
	}

	/**
	 * [設定] リセット.
	 *		現在の時間を記憶します。
	 */
	void Reset(void)
	{
		::QueryPerformanceFrequency(&m_freq);
		::QueryPerformanceCounter(&m_count);
		m_count.QuadPart += m_apiCount;
	}

	/**
	 * [取得] 経過時間取得.
	 *		コンストラクタ、あるいは Reset() からの時間(μs)を返します。
	 *	@return 経過時間(μs)
	 */
	LONGLONG GetPassedCount(void) const
	{
		LARGE_INTEGER	c;
		::QueryPerformanceCounter(&c);
		LONGLONG d = c.QuadPart - m_count.QuadPart;
		return (d * 1000 * 1000 / m_freq.QuadPart);
	}

	/**
	 * [取得] 経過時間取得.
	 *		コンストラクタ、あるいは Reset() からの時間(ns)を返します。
	 *	@return 経過時間(ns)
	 */
	LONGLONG GetPassedNanoCount(void) const
	{
		LARGE_INTEGER	c;
		::QueryPerformanceCounter(&c);
		LONGLONG d = c.QuadPart - m_count.QuadPart;
		return (d * 1000 * 1000 * 1000 / m_freq.QuadPart);
	}

	/**
	 * [取得] 分解度時間取得.
	 *		分解度時間(ns)を返します。
	 *	@note インスタンス構築時、 Reset() 時の分解度時間を返します。
	 *	@return 時間(ns)
	 */
	DWORD GetResolutionNanoCount(void) const
	{
		return static_cast<DWORD>(1000 * 1000 * 1000 / m_freq.QuadPart);
	}

	/**
	 * [確認] 経過確認.
	 *		コンストラクタ、あるいは Reset() からの時間(μs)と比較します。
	 *	@param dwTime 時間(μs)
	 *	@retval true dwTime 以上経過した。
	 *	@retval false 経過していない。
	 */
	bool IsPassed(DWORD dwTime) const
	{
		return GetPassedCount() >= dwTime;
	}

	/**
	 * [確認] 経過確認＆リセット.
	 *		コンストラクタ、あるいは Reset() からの時間(μs)と比較します。
	 *		経過していたらリセットもします。
	 *	@param dwTime 時間(μs)
	 *	@retval true dwTime 以上経過した。
	 *	@retval false 経過していない。
	 */
	bool IsPassedAndReset(DWORD dwTime)
	{
		bool r = IsPassed(dwTime);
		if ( r ){ Reset(); }
		return r;
	}

	/**
	 * [出力] 経過時間表示.
	 *		開発用です。コンストラクタ、あるいは Reset() からの時間(ms)を出力します。
	 *	@note リリースビルド時、何もしません。
	 */
	void OutputDebugPassedCount(void) const
	{
		TRACE1( " passed count = %.6f(ms)\n",  static_cast<double>(GetPassedNanoCount() / 1000000.0) );
	}

private:
	LARGE_INTEGER	m_count;		///< カウンタ
	LARGE_INTEGER	m_freq;			///< 分解度
	LONGLONG		m_apiCount;		///< QueryPerformanceCounter() の実行カウンタ
	bool			m_isValid;		///< 有効？
};



};//TNB
