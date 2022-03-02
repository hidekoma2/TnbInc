#pragma once
/**
 *@file
 * CE ダブルクリック関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeMfcDoubleClick.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbMfcCommon.h"
#include "TnbTickCount.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#ifndef STN_DBLCLK
 #define STN_DBLCLK 1
#endif
#ifndef BN_DOUBLECLICKED
 #define BN_DOUBLECLICKED 1
#endif
#endif // _TnbDOXYGEN



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document作成用シンボル
_SELECTANY DWORD s_dcaDoubleClickTime = 0;
_SELECTANY DWORD s_toleranceClickRange = TNB::CalcScaleX(10);
#endif // _TnbDOXYGEN



/**@ingroup MFCCONTROL WINCE 
 * ダブルクリックアドイン(CE専用).
 *	
 *		CE ではダブルクリックイベントが省略されています。
 *		これを制限付きで対応するアドインです。イベントハンドラは普通に IDE で作成できます。
 *	
 *	@note VC++2005の場合、 Wizard で ダブルクリックのハンドラを作成すると「シンボル( STN_DBLCLK や BN_DOUBLECLICKED )が
 *		宣言されていません」エラーが発生しますが、このヘッダで、宣言してあります。
 *
 *	@param WND CWnd のサブクラスである必要があります。通常、 スタティックコントロール、ボタンコントロールを指定します。
 *
 *	@note スタティックの場合、 SS_NOTIFY をつけてないと、ダブルクリックのイベントは発生しません。
 *
 *	\code
 *		//= 型宣言 (typedefしておかないと Wizardでエラーに成ります)
 *		typedef CDoubleClickAddinT<CStatic> CDcStatic;
 *
 *		//= ダイアログクラス宣言
 *		CDcStatic   m_text;     // <- 元々 CStaticだったメンバを書き換えた
 *	\endcode
 *
 *	@par必要ファイル
 *			TnbCeMfcDoubleClick.h
 * 
 *	@date 11/06/06	新規作成
 */
template<typename WND>
class CDoubleClickAddinT : public WND
{
	DEFSUPER(WND);
public:

	/// コンストラクタ
	CDoubleClickAddinT(void) : m_phase(STANDBY)
	{
		GetDoubleClickTime();
	}


	/**
	 * [設定] 許容クリック範囲設定.
	 *		タッチパネルでダブルクリックをする場合、同じ場所を二回タップすることはほぼ出来ません。
	 *		そこで少しずれてもダブルクリックとするための範囲を指定できるようにしてあります。
	 *		デフォルトは 96dpi 時 10 pixel(192 dpi 時 20 pixel)です。
	 *	@attention このメソッドは、他の CDoubleClickAddinT のインスタンスにも影響を与えます。
	 *	@param d 許容範囲(pixel) 
	 */
	static void SetToleranceClickRange(DWORD d)
	{
		s_toleranceClickRange = d;
	}

	/**
	 * [設定] ダブルクリック時間設定.
	 *		ダブルクリックとは、マウスボタンを 2 回続けてクリックすることです。
	 *		2 回目のクリックは、1 回目のクリックの後、指定された時間内に行わなければなりません。
	 *		この時間間隔のことをダブルクリック時間といいます。
	 *	@attention このメソッドは、他の CDoubleClickAddinT のインスタンスにも影響を与えます。
	 *	@note デフォルトは WINAPI の ：：GetDoubleClickTime() で得られる時間です。
	 *	@param t ダブルクリック時間(ms) 
	 */
	static void SetDoubleClickTime(DWORD t)
	{
		s_dcaDoubleClickTime = t;
	}

	/**
	 * [取得] ダブルクリック時間取得.
	 *	@note デフォルトは WINAPI の ：：GetDoubleClickTime() で得られる時間です。
	 *	@see SetDoubleClickTime()
	 *	@return ダブルクリック時間(ms) 
	 */
	static DWORD GetDoubleClickTime(void)
	{
		if ( s_dcaDoubleClickTime == 0 )
		{
			s_dcaDoubleClickTime = ::GetDoubleClickTime();
		}
		return s_dcaDoubleClickTime;
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_phase = STANDBY;
		LRESULT r = SendMessage(WM_GETDLGCODE);
		if ( (r & DLGC_STATIC) != 0 && (GetStyle() & SS_NOTIFY) == 0 )
		{
			m_phase = DISABLE;
		}
		_super::PreSubclassWindow();
	}

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_LBUTTONDOWN:
			if ( m_phase > STANDBY && m_IsPassed() )
			{
				m_phase = STANDBY;
			}
			if ( m_phase == STANDBY )
			{
				m_phase = UPWAIT;
//				m_point = CPoint(LOWORD(lParam), HIWORD(lParam));
				m_point = CPoint(lParam);
				m_tick.Reset();
			}
			else if ( m_phase == DOWNWAIT )
			{
				m_phase = m_Check(lParam) ? UPWAIT2 : STANDBY;
			}
			break;
		case WM_LBUTTONUP:
			if ( m_phase == UPWAIT )
			{
				m_phase = m_Check(lParam) ? DOWNWAIT : STANDBY;
			}
			else if ( m_phase == UPWAIT2 )
			{
				if ( m_Check(lParam) )
				{
					MFCLIB::SendCommandMessage(this, 1); //親へ通知
				}
				m_phase = STANDBY;
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_phase > STANDBY && ! m_Check(lParam) )
			{
				m_phase = STANDBY;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// フェーズ
	enum EPhase
	{
		DISABLE,	///< 無効
		STANDBY,	///< ボタンDOWN待ち
		UPWAIT,		///< ボタンUP待ち
		DOWNWAIT,	///< 二回目のボタンDOWN待ち
		UPWAIT2,	///< 二回目のボタンUP待ち
	};
	// エリア無いチェック
	bool m_IsArea(const POINT& p) const
	{
		DWORD x = abs(p.x - m_point.x);
		DWORD y = abs(p.y - m_point.y);
		return (x <= s_toleranceClickRange && y <= s_toleranceClickRange);
	}
	// 時間内チェック
	bool m_IsPassed(void) const
	{
		return m_tick.IsPassed(s_dcaDoubleClickTime);
	}
	// チェック
	bool m_Check(LPARAM lParam) const
	{
//		return (! m_IsPassed() && m_IsArea(CPoint(LOWORD(lParam), HIWORD(lParam))));
		return (! m_IsPassed() && m_IsArea(CPoint(lParam)));
	}
	CTickCount	m_tick;		///< 時間管理
	CPoint		m_point;	///< 座標
	int			m_phase;	///< フェーズ
};



}; // MFC
}; // TNB


