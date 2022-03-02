#pragma once
/**
 *@file
 * 一秒平均カウンタ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"
#include "TnbTickCount.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * 一秒間平均カウンタ管理クラス
 *
 *		一秒間に事象が何回発生したかをカウントすることが出来ます。
 *
 *	@par必要ファイル
 *			TnbParSecondCounter.h
 *
 *	@date 09/08/26	新規作成
 */
class CParSecondCounter : CCopyImpossible
{
public:

	/**
	 * コンストラクタ.
	 *	@param second 平均をとる期間(s)。省略すると5秒。
	 */
	CParSecondCounter(size_t second = 5)
	{
		Initialize(second);
	}

	/**
	 * [設定] 初期化.
	 *		カウンタを初期化します。
	 *	@param second 平均をとる期間(s)。省略すると5秒。
	 */
	void Initialize(size_t second = 5)
	{
		if ( second < 1 )
		{
			second = 1;
		}
		m_postCounts.Resize(second + 1);
		m_postCounts[0] = 0;
		m_validCount = 0;
		m_newestIndex = 0;
		m_nowTickSecond = 0;
		m_maxPostCount = 0;
		m_tick.Reset();
	}

	/**
	 * [設定] ポスト.
	 *		事象がおきた時にこれをコールします。
	 *	@param c 事象数。省略すると１が指定されます。
	 */
	void Post(DWORD c = 1)
	{
		EXCLUSIVE(&m_sync);
		DWORD t = m_tick.GetPassedCount() / 1000;
		if ( m_nowTickSecond == t )
		{
			m_postCounts[m_newestIndex] += c;
			return;
		}
		m_nowTickSecond = t;
		size_t count = m_postCounts.GetSize();
		if ( m_validCount < count )
		{
			m_validCount++;
		}
		if ( m_postCounts[m_newestIndex] > m_maxPostCount )
		{
			m_maxPostCount = m_postCounts[m_newestIndex];
		}
		m_newestIndex = (m_newestIndex + 1) % count;
		m_postCounts[m_newestIndex] = c;
	}

	/**
	 * [取得] 平均取得.
	 *	@return 平均数.
	 */
	double GetAverage(void) const
	{
		EXCLUSIVE(&m_sync);
		if ( m_validCount > 1 )
		{
			size_t count = m_postCounts.GetSize();
			DWORD r = 0;
			loop ( i, m_validCount - 1 )
			{
				r += m_postCounts[(count + count + m_newestIndex - i - 1) % count];
			}
			double a = r;
			return a / (m_validCount - 1);
		}
		return 0;
	}

	/**
	 * [取得] 最大数取得.
	 *	@return 最大数.
	 */
	DWORD GetMax(void) const
	{
		return m_maxPostCount;
	}

private:
	CWorkMemT<DWORD>	m_postCounts;		///< ポストした数。一つが一秒。
	DWORD				m_validCount;		///< 有効数
	DWORD				m_newestIndex;		///< 現在のインデックス
	DWORD				m_nowTickSecond;	///< 現在の秒
	DWORD				m_maxPostCount;		///< 最大数
	CTickCount			m_tick;				///< TICK
	CSyncSection		m_sync;
};



};