#pragma once
/**
 *@file
 * スムース進捗コントロール関係のヘッダ
 *
 *		スムースなバーの動きを提供する進捗コントロールを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * スムース進捗コントロールクラス
 *
 *		スムースなバーの動きを提供する進捗コントロールです。
 *
 *	@par使い方
 *		リソースエディタでプログレスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CProgressCtrl）を本クラスに置き換えて使用します。
 *
 *	@param PGC プログレスコントロールのクラス。省略すると CProgressCtrl
 *
 *	@par必要ファイル
 *			TnbMfcSmoothProgressCtrl.h
 * 
 *	@date 10/04/05 新規作成
 */
template<typename PGC = CProgressCtrl>
class CSmoothProgressCtrlT : public PGC
{
	DEFSUPER(PGC);
public:

	/// コンストラクタ
	CSmoothProgressCtrlT(void) : m_step(1), m_currentPos(0), m_maxDelta(-1.0)
	{
	}

	/**
	 * [設定] バーの色指定.
	 *	@param color バーの色。省略するとデフォルトの色になります。
	 *	@return 設定前の色。
	 */
	COLORREF SetBarColor(COLORREF color = CLR_DEFAULT) 
	{
		return _super::SendMessage(PBM_SETBARCOLOR, 0, color);
	}

	/**
	 * [設定] 背景色指定.
	 *	@param color 背景色。省略するとデフォルトの色になります。
	 *	@return 設定前の色。
	 */
	COLORREF SetBkColor(COLORREF color = CLR_DEFAULT)
	{
		return _super::SendMessage(PBM_SETBKCOLOR, 0, color);
	}

	/**
	 * [設定] 範囲設定.
	 *	@param nLower 下限。デフォルトは 0．
	 *	@param nUpper 上限。デフォルトは 100.
	 */
	void SetRange(short nLower, short nUpper)
	{
		SetRange32(nLower, nUpper);
	}

	/**
	 * [設定] 範囲設定.
	 *	@param nLower 下限。デフォルトは 0．
	 *	@param nUpper 上限。デフォルトは 100.
	 */
	void SetRange32(int nLower, int nUpper)
	{
		_super::SetRange32(nLower, nUpper);
		m_lower = nLower;
		m_upper = nUpper;
	}

	/**
	 * [取得] 範囲設定取得.
	 *	@param[out] _nLower 下限が格納されます。
	 *	@param[out] _nUpper 上限が格納されます。
	 */
	void GetRange(int& _nLower, int& _nUpper)
	{
		_super::GetRange(_nLower, _nUpper);
	}

	/**
	 * [取得] ポジション取得.
	 *	@return ポジション.
	 */
	int GetPos(void)
	{
		return m_currentPos;
	}

	/**
	 * [設定] ポジション設定.
	 *	@note スムースに移動せず、ポジションを移動させます。
	 *	@param nPos ポジション.
	 *	@return 設定前のポジション.
	 */
	int SetPos(int nPos)
	{
		m_currentPos = nPos;
		m_viewPos = nPos;
		m_viewDelta = 0.0;
		return _super::SetPos(nPos);
	}

	/**
	 * [設定] ステップ値設定.
	 *		StepIt() で、移動するポジション増分を指定します。
	 *	@param nStep 増分.
	 *	@return 設定前の増分.
	 */
	int SetStep(int nStep)
	{
		Swap(m_step, nStep);
		return nStep;
	}

	/**
	 * [設定] ポジション移動.
	 *		SetStep() で指定した増分だけ、ポジションを移動させます。
	 *	@return 移動前のポジション.
	 */
	int StepIt(void)
	{
		return OffsetPos(m_step);
	}

	/**
	 * [設定] ポジション移動.
	 *	@param nPos 増分。この分だけ、ポジションを移動させます。
	 *	@return 移動前のポジション.
	 */
	int OffsetPos(int nPos)
	{
		int r = m_currentPos;
		m_currentPos += nPos;
		_super::SetTimer(TIMERID_MOVE, 10, NULL);
		return  r;
	}

	/**
	 * [設定] 最大移動値設定.
	 *		指定のポジションまで移動する時の、一回の移動値の最大を指定することが出来ます。
	 *	@param d 最大移動値。省略すると、最大値を解除します。
	 */
	void SetMaxDelta(double d = -1.0)
	{
		m_maxDelta = d;			///< 移動値の最大
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::GetRange(m_lower, m_upper);
		SetPos(0);
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
		if ( message == WM_TIMER && wParam == TIMERID_MOVE )
		{
			OnMoveTimer(wParam);
			return 0;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] WM_TIMER イベント.
	 *	@param nIDEvent タイマーイベント
	 */
	void OnMoveTimer(UINT nIDEvent)
	{
		ASSERT( nIDEvent == TIMERID_MOVE );
		if ( m_currentPos != ToInt(m_viewPos + 0.005) )
		{
			// 表示位置と、内部位置にずれがある。
			m_viewDelta = m_currentPos - m_viewPos;
			if ( m_upper != m_currentPos )
			{
				m_viewDelta /= 20.0; //20回で
			}
			else
			{
				m_viewDelta /= 5.0; //5回で
			}
		}
		else
		{
			m_viewDelta = 0.0;
//			_super::SetPos(m_currentPos);
			KillTimer(TIMERID_MOVE);
		}
		if ( m_viewDelta != 0.0 )
		{
			//== デルタがある。
			//TRACE1("delta = %.4f\n", m_viewDelta);
			if ( m_maxDelta > 0.0 )
			{
				if ( ::fabs(m_viewDelta) > ::fabs(m_maxDelta) )
				{
					bool m = (m_viewDelta > 0.0);
					m_viewDelta = m ? m_maxDelta : -m_maxDelta;
				}
			}
			m_viewPos += m_viewDelta;
			if ( m_viewPos < m_lower )
			{
				m_viewPos = m_lower;
			}
			if ( m_viewPos > m_upper )
			{
				m_viewPos = m_upper;
			}
			_super::SetPos(ToInt(m_viewPos + 0.005));
		}
	}

private:
	enum { TIMERID_MOVE = 1 };
	double	m_viewPos;			///< 表示中のポジション
	double	m_viewDelta;		///< 移動値
	double	m_maxDelta;			///< 移動値の最大
	int		m_currentPos;		///< 現在のポジション(設定されている値)
	int		m_step;				///< StepIt のステップ値
	int		m_lower;			///< 下限
	int		m_upper;			///< 上限
};



/**@ingroup MFCCONTROL
 * スムース進捗コントロールクラス
 *
 *		スムースなバーの動きを提供する進捗コントロールです。
 *
 *	@par使い方
 *		リソースエディタでプログレスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CProgressCtrl）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcSmoothProgressCtrl.h
 * 
 *	@date 10/04/05 新規作成
 */
typedef CSmoothProgressCtrlT<> CSmoothProgressCtrl;



};
};
