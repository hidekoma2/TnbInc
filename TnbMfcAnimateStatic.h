#pragma once
/**
 *@file
 * アニメ表示関係のヘッダ
 *
 *		アニメを表示するためのコントロールを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapAnimater.h"
#include "TnbMfcRgnControl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL ANIMATE
 * アニメ表示コントロールクラス
 *
 *	@note Disable時、アニメを一時停止し、 Enable時に再開します。
 *	@note Hide時、アニメを停止し、Show時、頭から再生します。
 *
 *	@par使い方
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		// ダイアログのヘッダ、ダイアログクラスのメンバ。
 *		CAnimateStatic m_anime;
 *
 *		// OnInitDialog() 内
 *		m_anime.SetFirstFrame("1.bmp", CPenetrateWiper());
 *		m_anime.AddBaseFrame("1.bmp", 5);
 *		m_anime.AddBaseFrame("2.bmp", 5);
 *		m_anime.AddBaseFrame("3.bmp", 5);
 *		m_anime.AddBaseFrame("4.bmp", 5);
 *		ASSERT( anime.GetLength() == 21 ); 
 *		m_anime.SetAnimationParameter(false, 1000 / 21, false, true);
 *
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbMfcAnimateStatic.h
 * 
 *	@date 07/11/14 新規作成
 *	@date 07/11/30 CBitmapAnimater 使用するように変更。
 *	@date 07/12/04 HIDE時、Disable時に、アニメ停止するように変更。
 *	@date 07/12/12 リージョン指定を可能にした。
 *	@date 09/04/15 SetAnimationParameter() を StartAnimation()に改名。
 */
class CAnimateStatic : public CRgnControlAddinT<CWnd>, public CBitmapAnimater
{
	DEFSUPER(CRgnControlAddinT<CWnd>);
	typedef CBitmapAnimater _animater;
public:

	/// コンストラクタ
	CAnimateStatic(void) 
		: _super(), m_interval(1000), m_nowIndex(0), m_mode(0), m_useCache(false), m_isStopInDrag(false), m_doAnimate(false)
	{
	}

	/**
	 * [設定] アニメ設定.
	 *	@note 設定し、アニメを開始します。
	 *	@param mode true なら正順、降順を繰り返します。 false なら正順を繰り返します。
	 *	@param interval 指定時間(ms)ごとにフレームを書き換えます。
	 *	@param useCache 省略か false なら、キャッシュ使いません。true ならキャッシュを使用します。\n
	 *					※キャッシュはメモリは使いますが、処理は軽くなります。
	 *	@param isStopInDrag 省略か false なら、ドラック中もアニメーションします。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool StartAnimation(bool mode, DWORD interval, bool useCache = false, bool isStopInDrag = false)
	{
		m_Stop();
		if ( _animater::GetLength() > 0 )
		{
			m_mode = mode;
			m_interval = interval;
			m_useCache = useCache;
			m_isStopInDrag = isStopInDrag;
			_animater::FlashCache();
			//
			m_nowIndex = 0;
			m_nowBitmap.Null();
			m_doAnimate = true;
			m_Start();
			return true;
		}
		return false;
	}

	/**
	 * [設定] アニメ停止
	 */
	void StopAnimation(void)
	{
		m_doAnimate = false;
		m_Stop();
	}

	/**
	 * [設定] 最初のフレーム画像設定
	 *	@note これを実行すると、今までの情報はクリアされます。
	 *	@param bmp ビットマップ
	 *	@param defaultWiper AddBaseFrame() でワイパーが省略された時に使用されるワイパー。
	 *				複製を記憶しますので、破棄してかまいません。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetFirstFrame(CBitmapHandle bmp, const IBitmapWipable& defaultWiper)
	{
		bool r = _animater::SetFirstFrame(bmp, defaultWiper);
		m_Resize();
		return r;
	}

	/**
	 * [設定] 最初のフレーム画像設定
	 *	@note これを実行すると、今までの情報はクリアされます。
	 *	@param bmp ビットマップ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetFirstFrame(CBitmapHandle bmp)
	{
		bool r = _animater::SetFirstFrame(bmp);
		m_Resize();
		return r;
	}

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	/* 旧名
	 * アニメ開始。 StartAnimation() を使ってください。
	 */
	bool SetAnimationParameter(bool mode, DWORD interval, bool useCache = false, bool isStopInDrag = false)
	{
		return StartAnimation(mode, interval, useCache, isStopInDrag);
	}
	#endif

protected:

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
		case WM_ERASEBKGND:
			TTRACE0("WM_ERASEBKGND\n");
			if ( _animater::GetLength() != 0 )
			{
				return 0;
			}
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDc = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				m_Draw(pDc);
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // 描画用のデバイス コンテキスト
				RECT rc;
				GetClientRect(&rc);
				if ( ::RectVisible(dc, &rc) )
				{
					m_Draw(&dc);
				}
			}
			return 0;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			(wParam != 0) ? m_Start() : m_Stop();
			return 0;
		case WM_SHOWWINDOW:
			TTRACE0("WM_SHOWWINDOW\n");
			if ( wParam != 0 )
			{
				m_nowIndex = 0;
				m_nowBitmap.Null();
				m_Start();
			}
			else
			{
				m_Stop();
			}
			return 0;
		case WM_TIMER:
			TTRACE0("WM_TIMER\n");
			if ( wParam == TIMERID_ANIMA )
			{
				if ( ! ::IsWindowEnabled(m_hWnd) )
				{
					m_Stop();
				}
				else if ( m_IsAnimation() )
				{
					if ( m_mode > 0 )
					{
						m_nowIndex++;
						if ( m_nowIndex >= _animater::GetLength() )
						{
							m_nowIndex = _animater::GetLength() - 2;
							m_mode = -1;
						}
					}
					else if ( m_mode < 0 )
					{
						if ( m_nowIndex > 0 )
						{
							m_nowIndex--;
						}
						else
						{
							m_nowIndex = 1;
							m_mode = 1;
						}
					}
					else 
					{
						m_nowIndex++;
						if ( m_nowIndex >= _animater::GetLength() )
						{
							m_nowIndex = 0;
						}
					}
					m_nowBitmap.Null();
//					RedrawWindow(NULL, NULL, RDW_INVALIDATE);
					RedrawWindow();
				}
			}
			break;
		case WM_LBUTTONDOWN:
			return 0;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::ModifyStyle(0, SS_NOTIFY);
		_super::PreSubclassWindow();
		m_Resize();
	}

private:
	enum { TIMERID_ANIMA = 10000 };
	DWORD					m_interval;		///< 表示切替タイミング(ms)
	bool					m_doAnimate;	///< アニメする
	INDEX					m_nowIndex;		///< 表示中のインデックス
	CBitmapHandle			m_nowBitmap;	///< 表示中の絵
	int						m_mode;			///< 0;正順のみ -1;降順中 1;正順中
	bool					m_useCache;		///< true;キャッシュする
	bool					m_isStopInDrag;	///< true;ドラック中アニメ停止
	/// リサイズ
	void m_Resize(void)
	{
		if ( _animater::GetLength() == 0 ) { return; }
		SIZE size;
		if ( _animater::FrameAt(0).GetSize(size) )
		{
			MFCLIB::ChangeClientSize(this, size.cx, size.cy);
		}
	}
	/// 描画.
	void m_Draw(CDC* pDC)
	{
		if ( m_nowBitmap.IsNull() && _animater::GetLength() != 0 )
		{
			m_nowBitmap = m_useCache ? _animater::FrameAtWithCache(m_nowIndex): _animater::FrameAt(m_nowIndex);
		}
		if ( m_clientRgn != NULL )
		{
			MFCLIB::SelectClipRgnOrg(pDC, m_clientRgn);
		}
		if ( ! m_nowBitmap.IsNull() )
		{
			m_nowBitmap.Draw(*pDC);
		}
	}
	/// アニメする？
	bool m_IsAnimation(void)
	{
		if ( m_isStopInDrag && ::GetCapture() != NULL )
		{
			return false;
		}
		return _animater::GetLength() > 1;
	}
	/// アニメ開始(ただし、条件によっては開始しない)
	void m_Start(void)
	{
		if ( m_doAnimate && ::IsWindow(m_hWnd) )
		{
			SetTimer(TIMERID_ANIMA, m_interval, NULL);
		}
	}
	/// アニメ停止
	void m_Stop(void)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			KillTimer(TIMERID_ANIMA);
		}
	}

};



}; //MFC
}; //TNB



