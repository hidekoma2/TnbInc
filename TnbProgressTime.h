#pragma once
/**
 *@file
 * 進捗時間関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTickCount.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * 進捗時間管理クラス
 *
 *		全体の大きさを入れ、順次、進捗を通知することで、おおよその残り時間を計算します。
 *	
 *	@note	かかった時間から、全体の時間を割り出し、残り時間をだしていますので、
 *			進捗の進みにばらつきがある時、数値はかなりあやふやになります。
 *
 *
 *	@par使用例
 *		\code
 *		 	CProgressTime prog;
 *			CTickCount tt;
 *			loop ( i, 20 )
 *			{
 *				prog += 5;
 *				Sleep(500);
 *				TRACE2("%3d ; 残り %f秒\n", i, prog.EstimateRemainingSecond());	
 *			}
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbProgressTime.h
 *
 *	@date 07/01/19	新規作成
 *	@date 11/03/07	計算方法を改善。メソッド改善。
 */
class CProgressTime
{
public:

	/**
	 * コンストラクタ.
	 *	@param ri 進み具合再チェック間隔(秒)。省略すると5秒です。
	 */
	CProgressTime(DWORD ri = 5) : m_recheckInverval(ri * 1000)
	  , m_total(0), m_pos(0), m_lastPos(0), m_parSec(0.0), m_lastParSec(0.0), m_previousResult(-1)
	{
	}

	/**
	 * [開始] 開始.
	 *	@param total 全体値
	 */
	void Start(LONGLONG total = 100)
	{
		m_total = total;
		m_pos = 0;
		m_lastPos = 0;
		m_parSec = 0.0;
		m_lastParSec = 0.0;
		m_tick.Reset();
		m_lastTick.Reset();
		m_previousResult = -1;
		m_previousTick.Reset();
	}

	/**
	 * [設定] レジューム.
	 *	@note 長時間空いた時に再計算させるために使用します。
	 */
	void Resume(void)
	{
		m_parSec = 0.0;
		m_lastParSec = 0.0;
		m_lastTick.Reset();
		m_previousResult = -1;
		m_previousTick.Reset();
	}

	/**
	 * [設定] 進捗
	 *	@note 進捗値を指定します。
	 *	@param pos 進捗値。通常、全体値以下の値です。
	 */
	void SetPos(LONGLONG pos)
	{
		m_pos = pos;
	}

	/**
	 * [設定] 進捗、全体値設定
	 *	@note 全体値も変更する場合のみ、これを使用します。
	 *	@param pos 進捗値。通常、全体値以下の値です。
	 *	@param total 全体値。
	 */
	void Set(LONGLONG pos, LONGLONG total)
	{
		m_pos = pos;
		m_total = total;
	}

	/**
	 * [設定] 進捗
	 *	@note 指定加算数、進捗します。
	 *	@param d 加算数
	 */
	void operator+=(LONGLONG d)
	{
		m_pos += d;
	}

	/**
	 * [設定] 進捗
	 *	@note ＋１、進捗します。
	 */
	void operator++(void)
	{
		operator+=(1);
	}

	/**
	 * [設定] 進捗
	 *	@note ＋１、進捗します。
	 */
	void operator++(int)
	{
		operator+=(1);
	}

	/**
	 * [取得] 進捗値取得
	 *	@return 進捗値
	 */
	LONGLONG GetPos(void) const
	{
		return m_pos;
	}

	/**
	 * [取得] 全体値取得
	 *	@return 全体値
	 */
	LONGLONG GetTotal(void) const
	{
		return m_total;
	}

	/**
	 * [取得] 経過割合取得
	 *	@return 割合
	 */
	double GetRate(void) const
	{
		if ( m_total <= 0 )
		{
			return 0.0;
		}
		double p = double(m_pos) / double(m_total);
		if ( p > 1.0 )
		{
			p = 1.0;
		}
		return p;
	}

	/**
	 * [計算] 残り時間計算
	 *	@note Startしてからの時間と、全体長さと現在の進捗から、残り時間を概算します。
	 *	@retval ０以上 概算した残り秒。
	 *	@retval マイナス 計算中など、概算できなかった。
	 */
	int EstimateRemainingSecond(void)
	{
		int pr = m_previousResult;
		int r = m_Calc();
		m_previousResult = r;
		if ( r <= 0 )
		{
			return r;
		}
		if ( pr < 0 || m_previousTick.IsPassedAndReset(1000) )
		{
			// 以前のが無い or 1秒以上経過している。
			return r;
		}
		int rr = (r + pr) / 2;
		return rr;
	}

	/**
	 * [計算] 残り時間計算
	 *	@note Startしてからの時間と、全体長さと現在の進捗から、残り時間を概算します。
	 *	@retval ０以上 概算した残りミリ秒。
	 *	@retval マイナス 計算中など、概算できなかった。
	 *	@deprecated 精度は秒です。 EstimateRemainingSecond() を使用してください。
	 */
	int _deprecated EstimateRemainingTime(void)
	{
		return EstimateRemainingSecond() * 1000;
	}

private:
	// 残り時間計算
	int m_Calc(void)
	{
		if ( m_pos >= m_total )
		{
			return 0;
		}
		if ( m_pos == 0 )
		{
			return 0;
		}
		DWORD lp = m_lastTick.GetPassedCount();
		if ( lp >= m_recheckInverval )
		{
			//X秒超えた
			m_lastTick.Reset();
			double d = static_cast<double>(m_pos - m_lastPos);		//このX秒で変化した数
			d *= 1000.0;
			d /= lp;		//一秒間で増える数
			m_lastParSec = m_parSec;
			m_parSec = d;
			m_lastPos = m_pos;
		}
		if ( m_parSec > 0.0 )
		{
			double r = static_cast<double>(m_total - m_pos);	//残りの数
			if ( m_lastParSec > 0.0 )
			{
				r /= (m_parSec + m_lastParSec) / 2.0;
			}
			else
			{
				r /= m_parSec;
			}
			return int(r) + 1;
		}
		//
		DWORD passed = m_tick.GetPassedCount();
		UINT r = static_cast<UINT>(passed * m_total / m_pos - passed);
		if ( r < m_recheckInverval )
		{
			return r / 1000 + 1;
		}
		return -1;
	}

	CTickCount	m_tick;				///< カウンタ
	LONGLONG	m_total;			///< 全体値
	LONGLONG	m_pos;				///< 進捗値
	//
	CTickCount	m_lastTick;			///< 一つ前のカウンタ
	LONGLONG	m_lastPos;			///< 一つ前の進捗値
	double		m_parSec;			///< 一秒間に進んだ数
	double		m_lastParSec;		///< 一つ前の一秒間に進んだ数
	DWORD		m_recheckInverval;	///< 進捗最チェック間隔(ms)
	//
	CTickCount	m_previousTick;		///< 最後のカウンタ
	int			m_previousResult;	///< 最後のリザルト
};



};//TNB

//
// 10％終わった時、5秒かかった
// 残りは90％。さて、後何秒かかる？
//		答え、45秒。
//	 10% : 5秒   100% : 50秒
//
// 50％終わった時、30秒かかった
// 残りは50％。さて、後何秒かかる？
//		答え、30秒。
//	 50% : 30秒   100% : 60秒
//
// Step2；
// 最近の10％を終わるのに5秒かかった5*100

