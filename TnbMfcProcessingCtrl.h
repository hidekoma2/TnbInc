#pragma once
/**
 *@file
 * 処理中表記コントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbThread.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 処理中コントロール
 *
 *		inline なコントロールです。
 *		リソースEDITORでスタティックコントロールを張り、CStatic に割り当て後、
 *		本クラスに置き換えることを想定しています。
 *
 *		指定のタイミングで、マーカーが右往左往します。
 *
 *	@par必要ファイル
 *			TnbMfcProcessingCtrl.h
 * 
 *	@date 06/07/20	新規作成
 *	@date 07/10/31	WM_PRINTCLIENT 対応。
 *	@date 07/11/16	IDrawable 対応。
 *	@date 11/08/31	構造整理。
 *	@date 12/03/27	ReferThreadStatus() 新規。
 */
class CProcessingCtrl : public CWnd , TNB::CThread::IRunner
{
	DEFSUPER(CWnd);
public:

	/// モード
	enum EMode
	{
		Mode_GotoRight,	///< 左から右へ
		Mode_RoundTrip,	///< 右往左往
	};

	/// コンストラクタ
	CProcessingCtrl(void) 
		: _super(), m_size(0, 0), m_colorMark(::GetSysColor(COLOR_HIGHLIGHT))
		, m_isActing(false), m_interval(10), m_step(1), m_colorBack(::GetSysColor(COLOR_BTNFACE))
		, m_markWidth(6), m_pos(0), m_mode(1)
	{
		m_thread.SetRunner(this);
	}

	/// デストラクタ
	~CProcessingCtrl(void)
	{
		m_thread.Stop();
		m_DeleteDrawer();
	}

	/**
	 * [設定] 動き指定
 	 *	@param mode true なら右往左往します。 false なら左から右への移動のみを繰り返します。
	 *	@param interval 移動速度(ms)。指定時間毎に移動処理をします。
	 *	@param step マークの移動数。省略すると６です。
	 *	@param colorBack 背景の色
	 */
	void SetParameter(EMode mode, DWORD interval, int step = 6, COLORREF colorBack = CLR_INVALID)
	{
		m_mode = mode;
		m_interval = interval;
		m_step = step;
		m_colorBack = (colorBack != CLR_INVALID) ? colorBack : ::GetSysColor(COLOR_BTNFACE);
	}

	/**
	 * [設定] マーカー指定.
	 *		単純色のマーカーを指定します。
	 *	@param colorMark マークの色
	 *	@param width マークの幅
	 */
	void SetMarker(COLORREF colorMark, int width)
	{
		Action(false);
		m_DeleteDrawer();
		m_colorMark = (colorMark != CLR_INVALID) ? colorMark : ::GetSysColor(COLOR_HIGHLIGHT);
		m_markWidth = width;
	}

	/**
	 * [設定] マーカー指定.
	 *		描画情報をマーカーとして指定します。
	 *	@param draw 描画情報. 本オブジェクトには表示情報が記憶されるので、 draw は破棄してもかまいません。
	 */
	void SetMarker(const IDrawable& draw)
	{
		Action(false);
		m_DeleteDrawer();
		SIZE s;
		if ( draw.GetSize(s) )
		{
			m_piMarkerDrawer = draw.Clone();
			m_markWidth = s.cx;
		}
	}

	/**
	 * [設定] 動作.
	 *		マーカーの動作を開始/停止します。
	 *		作成時は停止しています。
	 *	@param isAction true で動作開始 / falseで動作停止します。
	 */
	void Action(bool isAction)
	{
		m_isActing = isAction;
		m_thread.Stop();
		if ( isAction )
		{
			m_pos = 0;
			m_thread.Start(_T("ProcessingCtrl"));
		}
		RedrawWindow();
	}

	/**
	 * [確認] 動作確認.
	 *		マーカーの動作確認止ます。
	 *	@retval true 動作
	 *	@retval false 停止
	 */
	bool IsActing(void) const
	{
		return m_isActing;
	}

	/**
	 * [設定] スレッド状態種取得
	 *	@return スレッド状態
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		CRect rc;
		_super::GetClientRect(rc);
		m_size = rc.Size();
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
		if ( message == WM_PAINT )
		{
			CPaintDC dc(this); // 描画用のデバイス コンテキスト
			m_Draw(&dc);
			return 0;
		}
		else if ( message == WM_PRINTCLIENT )
		{
			CDC* pDc = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
			m_Draw(pDc);
			return 0;
		}
		else if ( message == WM_DESTROY )
		{
			m_isActing = false;
			m_thread.Stop();
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * スレッド処理関数
	 *	@retval ０（固定)。
	 */
	DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			::Sleep(m_interval);
			if ( m_isActing )
			{
				if ( m_mode > 0 )
				{
					m_pos += m_step;
					if ( m_pos + m_markWidth > m_size.cx )
					{
						m_pos = m_size.cx - m_markWidth;
						m_mode = -1;
					}
				}
				else if ( m_mode < 0 )
				{
					if ( m_pos >= m_step )
					{
						m_pos -= m_step;
					}
					else
					{
						m_mode = 1;
						m_pos = 0;
					}
				}
				else 
				{
					m_pos += m_step;
					if ( m_pos > m_size.cx )
					{
						m_pos = -m_markWidth;
					}
				}
				if ( IsRunnable() )
				{
					_super::Invalidate();
				}
			}
		}
		return 0;
	}

private:
	/// マーカー描画削除
	void m_DeleteDrawer(void)
	{
		m_piMarkerDrawer.Null();
	}
	/**
	 * 描画
	 *	@param pDC 描画先のCDC。
	 */
	void m_Draw(CDC* pDC)
	{
		CRect cliRect(CPoint(0, 0), m_size);
		HRGN rgn = ::CreateRectRgnIndirect(&cliRect);
		MFCLIB::SelectClipRgnOrg(pDC, rgn);
		_DeleteObject(rgn);
		if ( m_isActing )
		{
			CRect markRect(m_pos, 0, m_pos + m_markWidth, m_size.cy);
			if ( m_piMarkerDrawer.IsNull() )
			{
				pDC->FillSolidRect(markRect, m_colorMark);
			}
			else
			{
				m_piMarkerDrawer->Draw(*pDC, m_pos, 0);
			}
			HRGN rgn = ::CreateRectRgnIndirect(&markRect);
			MFCLIB::SelectClipRgnOrg(pDC, rgn, RGN_DIFF);
			_DeleteObject(rgn);
		}
		pDC->FillSolidRect(cliRect, m_colorBack);
		pDC->SelectClipRgn(NULL);
	}
	CThread			m_thread;			///< スレッド
	CSize			m_size;				///< クライアントの大きさ
	COLORREF		m_colorBack;		///< 背景色
	bool			m_isActing;			///< 動かす？
	DWORD			m_interval;			///< 速度
	int				m_step;				///< 移動量
	COLORREF		m_colorMark;		///< マークカラー
	IDrawable::Ptr	m_piMarkerDrawer;	///< マーカー
	int				m_markWidth;		///< マーカーの幅
	int				m_pos;				///< 現在の座標
	int				m_mode;				///< 移動モード 0;正順のみ -1;降順中 1;正順中
};



}; //MFC
}; //TNB
