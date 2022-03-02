#pragma once
/**
 *@file
 * 経過時間管理関係のヘッダ
 *
 *	@note	マクロ "_TnbTIME_Winmm_DISABLE" が定義されていると、 GetTickCount() を使います
 *			（精度は落ちますが、処理が軽減される可能性があります）。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



#ifdef _WIN32_WCE
 #define _TnbTIME_Winmm_DISABLE
#endif

#ifndef _TnbTIME_Winmm_DISABLE
 #include <mmsystem.h>
 #pragma comment(lib, "winmm.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * 経過時間管理クラス
 *
 *		時間の経過を扱うクラスです。
 *
 *	@note "_TnbTIME_Winmm_DISABLE"が定義されていない場合、APIの timeGetTime() を使用します。
 *	@note "_TnbTIME_Winmm_DISABLE"が定義されている場合、APIの GetTickCount() を使用します。
 *
 *	@note	●(参考) "_TnbTIME_Winmm_DISABLE"が定義されている場合の精度<BR>
 *				Windows NT 3.5以降：約１０ミリ秒ごとにカウント	<BR>
 *				Windows NT 3.1：約１６ミリ秒ごとにカウント		<BR>
 *				Windows 95系：約５５ミリ秒ごとにカウント		<BR>
 *
 *	@par必要ファイル
 *			TnbTickCount.h
 *
 *	@date 06/06/29	新規作成
 *	@date 06/09/20	timeGetTime() を使用するように変更。
 *	@date 09/02/24	値付きコンストラクタを用意。
 *	@date 09/10/28	OutputDebugPassedCount() 追加。
 */
class CTickCount
{
public:

	/**
	 * コンストラクタ
	 *	@note 現在のtickを記憶します。
	 */
	CTickCount(void) : m_dwTick(m_Get())
	{
	}

	/**
	 * コンストラクタ
	 *	@note 現在のtickから約21日前(一番遠くのtick)を記憶します。
	 *	@param b ダミー
	 */
	explicit CTickCount(bool b) : m_dwTick(m_Get())
	{
		m_dwTick -= 0x80000000;
	}

	/**
	 * [設定] リセット.
	 *		現在の時間を記憶します。
	 */
	void Reset(void)
	{
		m_dwTick = m_Get();
	}

	/**
	 * [取得] Reset時の時間取得.
	 *		コンストラクタ、あるいは Reset() した時の、時間(OS起動してからの時間)を返します。
	 *	@return 時間(ms)
	 */
	DWORD GetResetedCount(void) const
	{
		return m_dwTick;
	}

	/**
	 * [取得] 経過時間取得.
	 *		コンストラクタ、あるいは Reset() からの時間(ms)を返します。
	 *	@return 経過時間(ms)
	 */
	DWORD GetPassedCount(void) const
	{
		return m_Get() - m_dwTick;
	}

	/**
	 * [確認] 経過確認.
	 *		コンストラクタ、あるいは Reset() からの時間(ms)と比較します。
	 *	@param dwTime 時間(ms)
	 *	@retval true dwTime 以上経過した。
	 *	@retval false 経過していない。
	 */
	bool IsPassed(DWORD dwTime) const
	{
		return GetPassedCount() >= dwTime;
	}

	/**
	 * [確認] 経過確認＆リセット.
	 *		コンストラクタ、あるいは Reset() からの時間(ms)と比較します。
	 *		経過していたらリセットもします。
	 *	@param dwTime 時間(ms)
	 *	@param isNow リセットの方法。 true なら今の時間にリセットします。 false なら指定時間経過を差し引くだけにします。
	 *	@retval true dwTime 以上経過した。
	 *	@retval false 経過していない。
	 */
	bool IsPassedAndReset(DWORD dwTime, bool isNow = true)
	{
		bool r = IsPassed(dwTime);
		if ( r )
		{
			if ( isNow )
			{
				Reset();
			}
			else
			{
				m_dwTick += dwTime;
				if ( IsPassed(dwTime) )
				{
					Reset();
				}
			}
		}
		return r;
	}

	/**
	 * [出力] 経過時間表示.
	 *		開発用です。コンストラクタ、あるいは Reset() からの時間(ms)を出力します。
	 *	@note リリースビルド時、何もしません。
	 */
	void OutputDebugPassedCount(void) const
	{
		TRACE1( " passed count = %d(ms)\n", GetPassedCount() );
	}

private:
	DWORD m_dwTick;		///< TickCount記憶用

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	  #ifndef _TnbTIME_Winmm_DISABLE
		/// timePeriodの自動設定用クラス
		class CAutoPeriod
		{
		public:
			CAutoPeriod(void)
			{
				VERIFY(timeBeginPeriod(1) == TIMERR_NOERROR);
			}
			~CAutoPeriod(void)
			{
				VERIFY(timeEndPeriod(1) == TIMERR_NOERROR);		
			}
		};
		/// 現在のシステム起動時間取得
		DWORD m_Get(void) const
		{
			static CAutoPeriod s_autoPeriod;
			return ::timeGetTime();
		}
	  #else
		/// 現在のシステム起動時間取得
		DWORD m_Get(void) const
		{
			return ::GetTickCount();
		}
	  #endif
	#endif // _TnbDOXYGEN
};



};//TNB
