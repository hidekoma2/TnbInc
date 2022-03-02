#pragma once
/**
 *@file
 * リサイズダイアログ関係のヘッダ
 *
 *		リサイズ機能を持つダイアログのベースを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



/// MFC static リンク対策
#define classCResizeDialog classCDialog



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * リサイズダイアログベースクラス
 *
 *		inline なコントロールです。
 *
 *		本クラスを CDialog の代わりに継承すると、サイズ変更可能なダイアログになります。
 *
 *		CDialog を使ったダイアログを作る要領でリソースを作成しクラス生成後、 CDialog
 *		を本クラスに置き換える形の実装方法を想定しています。
 *
 *		ダイアログのリソースは、「境界線(Border)」を「サイズ変更枠」にし、「水平スクロールバー(Horizontal Scrollbar)」
 *		「垂直スクロールバー(Vertical Scrollbar)」にチェックを付けない(falseにする)ようにします。
 *
 *		また、 OnPaintで描画しているものは、 CPaintOrgDC を使い DC を取得してください。
 *
 *		後は、普通のダイアログと同じです。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		//= 宣言(ヘッダファイル)
 *			class CXxxDlg : public CResizeDialog	// ← CDialog を CResizeDialog に変更。		
 *					// 他の 「CDialog」 も同様
 *			{
 *					:
 *				virtual void OnPrintClient(CDC* pDC);	// WM_PRINT に対応する場合、追加
 *					:
 *
 *		//= 設定(ソースファイル) WM_PRINT に対応する場合
 *			
 *			void CXxxDlg::OnPaint() 
 *			{
 *				CPaintOrgDC dc(this);	// ← デバイスコンテキストの取得
 *				OnPrintClient(&dc);
 *			}
 *			void CXxxDlg::OnPrintClient(CDC* pDC)
 *			{
 *				m_bmps.Draw(*pDC);	// 後は普通に
 *			}
 *				:
 *
 *		//= 設定(ソースファイル) WM_PRINT に対応しない場合
 *			
 *			void CXxxDlg::OnPaint() 
 *			{
 *				CPaintOrgDC dc(this);	// ← デバイスコンテキストの取得
 *				m_bmps.Draw(dc);	// 後は普通に
 *			}
 *				:
 *
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbMfcResizeDialog.h
 * 
 *	@date 07/10/18 新規作成
 *	@date 07/10/22 スクロール関係メッセージ、誰宛かを見るように修正。
 *	@date 07/10/24 スクロール関係メッセージ、修正。ホイールにも対応。 GetOriginalSize() 追加。
 *	@date 07/10/26 WM_PAINT 処理を修正。 テンプレートをやめ、CDialog継承へ。
 *	@date 07/10/27 OnOffsetPaintを廃止。 縦スクロールバーが出ないバグを修正。
 *	@date 07/10/29 SetScrollOrg() を追加。
 *	@date 07/10/31 WM_PRINT , WM_PRINTCLIENT に対応。 OnPrintClient() 追加。
 *	@date 07/11/02 SCROLLバーが出ないことがまれにあった。明示的に表示 true するようにした。
 *	@date 07/11/06 SetOriginalSize() の引数は Window サイズ用だった。 Client サイズ指定メソッドも新規。
 *	@date 08/01/29 フレーム、テーマ変更に対応。
 *	@date 08/02/01 スクロールバーがないのみ、 WM_MOUSEWHELL スルーするようにした。
 *				   SetScrollOrg() に第二引数追加。 CPaintOrgDC クラスを追加。
 *	@date 08/04/11 クライアントがスクロールバーより小さくなったらスクロールバーをHIDEにするようにした。
 */
class CResizeDialog : public CDialog
{
	DEFSUPER(CDialog);
	enum { DEFAULT_LINESTEP = 20 };
public:

	/**@ingroup MFCCONTROL
	 * リサイズダイアログ用 PaintDC クラス
	 *
	 *	@see CResizeDialog
	 *	@par必要ファイル
	 *			TnbMfcResizeDialog.h
	 * 
	 *	@date 08/02/01 新規作成
	 */
	class CPaintOrgDC : public CPaintDC
	{
		DEFSUPER(CPaintDC);
		CResizeDialog*	m_pResizeDlg;
	public:
		/**
		 * コンストラクタ
		 *	@param pWnd CResizeDialogのインスタンス
		 */
		explicit CPaintOrgDC(CResizeDialog* pWnd) : _super(pWnd), m_pResizeDlg(pWnd)
		{
			m_pResizeDlg->SetScrollOrg(this, true);
		}
		/// デストラクタ
		~CPaintOrgDC(void)
		{
			m_pResizeDlg->SetScrollOrg(this, false);
		}
	};

	/// コンストラクタ
	CResizeDialog(void) 
		: _super(), m_originalSize(0, 0), m_lineStep(DEFAULT_LINESTEP)
		, m_onThemeChanged(false), m_canSizeDisp(true)
	{
	}

	/**
	 * コンストラクタ
	 *	@param lpszTemplateName ダイアログ ボックス テンプレート リソース名を表す null で終わる文字列。
	 *	@param pParentWnd ダイアログ オブジェクトを所有する CWnd 型の親ウィンドウ
	 *					またはオーナー ウィンドウオブジェクトへのポインタ。
	 *					NULL の場合は、ダイアログ オブジェクトの親ウィンドウがアプリケーションのメイン ウィンドウになります。
	 */
	CResizeDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
		: _super(lpszTemplateName, pParentWnd), m_originalSize(0, 0), m_lineStep(DEFAULT_LINESTEP)
		, m_onThemeChanged(false), m_canSizeDisp(true)
	{
	}

	/**
	 * コンストラクタ
	 *	@param nIDTemplate ダイアログ ボックス テンプレート リソースの ID 番号。
	 *	@param pParentWnd ダイアログ オブジェクトを所有する CWnd 型の親ウィンドウ
	 *					またはオーナー ウィンドウオブジェクトへのポインタ。
	 *					NULL の場合は、ダイアログ オブジェクトの親ウィンドウがアプリケーションのメイン ウィンドウになります。
	 */
	CResizeDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) 
		: _super(nIDTemplate, pParentWnd), m_originalSize(0, 0), m_lineStep(DEFAULT_LINESTEP)
		, m_onThemeChanged(false), m_canSizeDisp(true)
	{
	}

	/**
	 * [取得] スクロール位置.
	 *	@return 左上端に表示されているクライアントの座標。
	 */
	CPoint GetScrollPoint(void) const
	{
		return CPoint(m_horizontalParam.GetPos(), m_verticalParam.GetPos());
	}

	/**
	 * [設定] デバイスコンテキストの原点設定、解除.
	 *		デバイス コンテキストのウィンドウの原点をスクロール位置にあわせます。
	 *	@note 設定と解除をセットで使用します。
	 *	@param pDc デバイスコンテキスト。
	 *	@param isBegin 省略すると原点設定します。 false を指定すると解除します。
	 */
	void SetScrollOrg(CDC* pDc, bool isBegin = true) const
	{
		int cx = m_horizontalParam.GetPos();
		int cy = m_verticalParam.GetPos();
		if ( isBegin )
		{
			pDc->OffsetWindowOrg(cx, cy);
		}
		else
		{
			pDc->OffsetWindowOrg(-cx, -cy);
		}
	}

	/**
	 * [取得] オリジナルウィンドウサイズ取得.
	 *		オリジナルのサイズ（ウィンドウの最大サイズ）を得ることが出来ます。
	 *	@note	サイズ変更している状態では、GetWindowRect() で、ウィンドウの本当の大きさを知ることが出来ません。
	 *			本メソッドは、サイズ変更していても、オリジナルの大きさを得ることが出来ます。
	 *	@return サイズ.
	 */
	CSize GetOriginalWindowSize(void) const
	{
		return GetOriginalSize() + m_offsetWinToClient;
	}

	/**
	 * [取得] オリジナルクライアントサイズ取得.
	 *		オリジナルのサイズ（クライアントの最大サイズ）を得ることが出来ます。
	 *	@note	サイズ変更している状態では、GetClientRect() で、クライアントの本当の大きさを知ることが出来ません。
	 *			本メソッドは、サイズ変更していても、オリジナルの大きさを得ることが出来ます。
	 *	@return サイズ.
	 */
	const CSize& GetOriginalSize(void) const
	{
		return m_originalSize;
	}

	/**
	 * [設定] オリジナルウィンドウサイズ設定.
	 *		オリジナルのサイズ（ウィンドウの最大サイズ）を設定することが出来ます。
	 *	@note	通常、リソースから自動的にサイズを得ます。
	 *	@param size サイズ.
	 */
	void SetOriginalWindowSize(const SIZE& size)
	{
		m_CheckOffset();
		UINT r = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
		m_canSizeDisp = false;
		_super::SetWindowPos(NULL, 0, 0, size.cx, size.cy, r);
		m_canSizeDisp = true;
		CRect rc;
		GetClientRect(&rc);
		m_originalSize = rc.Size();
		m_InitScrollInfo();
	}

	/**
	 * [設定] オリジナルクライアントサイズ設定.
	 *		オリジナルのサイズ（クライアントの最大サイズ）を設定することが出来ます。
	 *	@note	通常、リソースから自動的にサイズを得ます。
	 *	@param size サイズ.
	 */
	void SetOriginalSize(const SIZE& size)
	{
		m_CheckOffset();
		m_originalSize = size;
		CSize s = size;
		s += m_offsetWinToClient;
		UINT r = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
		m_canSizeDisp = false;
		_super::SetWindowPos(NULL, 0, 0, s.cx, s.cy, r);
		m_canSizeDisp = true;
		m_InitScrollInfo();
	}

	/**
	 * [設定] オリジナルクライアントサイズ変更.
	 *		オリジナルのサイズ（クライアントの最大サイズ）を変更することが出来ます。
	 *	@param size サイズ.
	 */
	void ChangeOriginalSize(const SIZE& size)
	{
		int dx = 0;
		int dy = 0;
		if ( m_horizontalParam.GetMax() < size.cx )
		{
			dx -= size.cx - m_horizontalParam.GetMax();
		}
		if ( m_verticalParam.GetMax() < size.cy )
		{
			dy -= size.cy - m_verticalParam.GetMax();
		}
		m_OnHScroll(SB_LEFT, 0, NULL);
		m_OnVScroll(SB_TOP, 0, NULL);
		m_horizontalParam.ShowBar(*this, SB_HORZ, false);
		m_verticalParam.ShowBar(*this, SB_VERT, false);
		//
		m_CheckOffset();
		m_originalSize = size;
		CSize s = size;
		s += m_offsetWinToClient;
		UINT r = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
		_super::SetWindowPos(NULL, 0, 0, s.cx, s.cy, r);
		m_InitScrollInfo();
		//
		_super::ScrollWindow(dx, 0);
		_super::ScrollWindow(0, dy);
	}

	/**
	 * [設定] ラインスクロール量設定.
	 *		スクロールバーの矢印ボタンを押した時にスクロールする量を指定します。
	 *	@note デフォルトでは、20です。
	 *	@param s スクロール量.
	 */
	void SetLineStep(int s)
	{
		m_lineStep = s;
	}

	/**
	 * [設定] オリジナルウィンドウサイズ化.
	 *		ウィンドウサイズをオリジナル戻します
	 */
	void ResetWindowsSize(void)
	{
		m_OnHScroll(SB_LEFT, 0, NULL);
		m_OnVScroll(SB_TOP, 0, NULL);
		SIZE sz;
		SetOriginalWindowSize(sz);
		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	/**
	 * [通知] WM_PRINTCLIENT メッセージ受信.
	 *		WM_PRINTCLIENT メッセージ受信したらコールされます。
	 *	@note 通常、OnPaint() で描画している内容と同じものを描きます。
	 *	@note SetScrollOrg() は使用しません。
	 *	@param pDC デバイスコンテキスト。
	 */	
	virtual void OnPrintClient(CDC* pDC)
	{
	}

protected:
	/// WinとClientの差を計算
	void m_CheckOffset(void)
	{
		if ( m_originalSize.cx == 0 )
		{
			CRect cliRc;
			_super::GetClientRect(&cliRc);
			CRect winRc;
			_super::GetWindowRect(&winRc);
			m_originalSize = cliRc.Size();
			m_offsetWinToClient = winRc.Size() - cliRc.Size();
		}
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
		LRESULT r = 0;
		switch ( message )
		{
		case WM_NCLBUTTONDBLCLK:
			TTRACE0("WM_NCLBUTTONDBLCLK\n");
			if ( wParam == HTCAPTION )
			{
				CSize s = GetOriginalWindowSize();
				_super::SetWindowPos(NULL, 0, 0, s.cx, s.cy, SWP_NOMOVE | SWP_NOZORDER);
				return 0;
			}
			break;
		case WM_SIZE:
			TTRACE0("WM_SIZE\n");
			r = _super::WindowProc(message, wParam, lParam);
			if ( m_canSizeDisp )
			{
				m_OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
			}
			return r;
		case WM_GETMINMAXINFO:
			TTRACE0("WM_GETMINMAXINFO\n");
			m_CheckOffset();
			if ( m_originalSize.cx != 0 )
			{
				LPMINMAXINFO P = reinterpret_cast<LPMINMAXINFO>(lParam);
				P->ptMaxTrackSize.x = m_originalSize.cx + m_offsetWinToClient.cx;
				P->ptMaxTrackSize.y = m_originalSize.cy + m_offsetWinToClient.cy;
			}
			break;
		case WM_HSCROLL:
			TTRACE2("WM_HSCROLL %d 0x%08X\n", LOWORD(wParam), lParam);
			m_OnHScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
			break;
		case WM_VSCROLL:
			TTRACE2("WM_VSCROLL %d 0x%08X\n", LOWORD(wParam), lParam);
			m_OnVScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
			break;
		case WM_MOUSEWHEEL:
			TTRACE0("WM_MOUSEWHEEL\n");
			if ( m_OnMouseWheel(LOWORD(wParam), static_cast<short>(HIWORD(wParam)), lParam) )
			{
				return TRUE;
			}
			break;
		case WM_PRINT:
			TTRACE1("WM_PRINT 0x%08X\n", lParam);
			r = _super::WindowProc(message, wParam, lParam & ~(PRF_OWNED | PRF_CHILDREN));
			if ( (lParam & PRF_CHILDREN) != 0 )
			{
				CRect rectWin;
				_super::GetWindowRect(rectWin);
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				CPoint orgOrg = pDC->GetWindowOrg();
				CWnd* pWnd = CWnd::GetWindow(GW_CHILD);
				while ( pWnd != NULL )
				{
					CRect rect;
					pWnd->GetWindowRect(rect);
					CSize po = rect.TopLeft() - rectWin.TopLeft();
					pDC->SetWindowOrg(orgOrg - po);
					pWnd->SendMessage(WM_PRINT, wParam, lParam | PRF_CHECKVISIBLE);
					pWnd = pWnd->GetNextWindow();
				}
				pDC->SetWindowOrg(orgOrg);
				if ( (lParam & PRF_NONCLIENT) != 0 )
				{
					SendMessage(message, wParam, PRF_NONCLIENT);
				}
			}
			return r;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				SetScrollOrg(pDC);
				OnPrintClient(pDC);
				SetScrollOrg(pDC, false);
			}
			break;
		case WM_INITDIALOG:
			TTRACE0("WM_INITDIALOG\n");
			r = _super::WindowProc(message, wParam, lParam);
			m_InitScrollInfo();
			return r;
		case WM_THEMECHANGED:
			TTRACE0("WM_THEMECHANGED\n");
			m_onThemeChanged = true;
			break;
		case WM_EXITSIZEMOVE:
			TTRACE0("WM_EXITSIZEMOVE\n");
			m_onThemeChanged = false;
			break;
		case WM_WINDOWPOSCHANGED:
			TTRACE0("WM_WINDOWPOSCHANGED\n");
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				WINDOWPOS* W = reinterpret_cast<WINDOWPOS*>(lParam);
				if ( m_onThemeChanged && (W->flags & SWP_FRAMECHANGED) != 0 )
				{
					CRect cliRc;
					_super::GetClientRect(&cliRc);
					CRect winRc;
					_super::GetWindowRect(&winRc);
					CSize sz = m_offsetWinToClient;
					m_offsetWinToClient = winRc.Size() - cliRc.Size();
					if ( m_horizontalParam.IsEnable() )
					{
						m_offsetWinToClient.cx -= ::GetSystemMetrics(SM_CYHSCROLL);
					}
					if ( m_verticalParam.IsEnable() )
					{
						m_offsetWinToClient.cy -= ::GetSystemMetrics(SM_CXHSCROLL);
					}
					m_OnSize(0, cliRc.right, cliRc.bottom);
				}
				return r;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// パラメータ
	class CBarParam
	{
		int		m_pos;				///< 現在位置
		int		m_max;				///< ＭＡＸ位置
		int		m_page;				///< ページサイズ
		bool	m_boIsEnable;			///< コントロール状態(trueならEnable)
	public:
		/// コンストラクタ
		CBarParam(void) : m_boIsEnable(false), m_pos(0), m_max(0), m_page(0) { }
		/// ポジション取得
		int GetPos(void) const { return m_boIsEnable ? m_pos : 0; }
		/// MAX取得
		int GetMax(void) const { return m_max; }
		/// コントロール状態取得 (trueならEnable)
		bool IsEnable(void) const { return m_boIsEnable; }
		/// MAX設定
		void SetMax(int max)
		{
			m_pos = 0;
			m_max = max;
			m_page = max;
		}
		/// ページセット。戻りとして現在の位置からのデルタ。
		int SetPage(int pa)
		{
			m_page = pa;
			int delta = 0;
			if ( m_pos + m_page > m_max )
			{
				delta = m_pos - (m_max - m_page);
				m_pos = m_max - m_page;
			}
			return delta;
		}
		/// バー表示・非表示
		void ShowBar(HWND h, int nBar, bool isEnable)
		{
			m_boIsEnable = isEnable;
			if ( m_boIsEnable )
			{
				SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL, 0, m_max - 1, m_page, m_pos, 0 };
				ASSERTLIB( si.cbSize == sizeof(SCROLLINFO) && si.nPos == m_pos );
				::SetScrollInfo(h, nBar, &si, TRUE);
				::ShowScrollBar(h, nBar, TRUE);
			}
			else
			{
				::ShowScrollBar(h, nBar, FALSE);
//				::InvalidateRect(h, NULL, false);
			}
		}
		/// スクロール。戻り値はデルタ
		int Scroll(UINT nSBCode, UINT nPos, int lineStep) const
		{
			int delta = 0;
			switch ( nSBCode )
			{
			case SB_LEFT: // SB_TOP
				delta = -m_pos;
				break;
			case SB_LINERIGHT: // SB_LINEDOWN
			case SB_PAGERIGHT: // SB_PAGEDOWN
				if ( m_pos + m_page < m_max )
				{
					int r = (nSBCode == SB_LINERIGHT) ? lineStep : m_page;
					delta = min(r, m_max - (m_pos + m_page));
				}
				break;
			case SB_LINELEFT: // SB_LINEUP
			case SB_PAGELEFT: // SB_PAGEUP
				if ( m_pos > 0 )
				{
					int r = (nSBCode == SB_LINELEFT) ? lineStep : m_page;
					delta = -min(r, m_pos);
				}
				break;
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				delta = static_cast<int>(nPos) - m_pos;
				break;
			default:
				break;
			}
			if ( m_pos + delta < 0 )
			{
				delta = -m_pos;
			}
			if ( m_pos + m_page + delta > m_max )
			{
				delta = m_max - (m_pos + m_page);
			}
			return delta;
		}
		/// ポジション移動
		int AddPos(int delta)
		{
			m_pos += delta;
			return m_pos;
		}
	};
	CSize		m_originalSize;		///< オリジナルのクライアントサイズ
	CSize		m_offsetWinToClient;///< WindowサイズとClientサイズの差
	CBarParam	m_horizontalParam;	///< 水平パラメータ
	CBarParam	m_verticalParam;	///< 垂直パラメータ
	int			m_lineStep;			///< ラインスクロール時の移動量
	bool		m_onThemeChanged;	///< テーマチェンジ中フラグ
	bool		m_canSizeDisp;		///< サイズ変更イベント処理

	/// スクロールバー情報初期化
	void m_InitScrollInfo(void)
	{
		_super::ShowScrollBar(SB_HORZ, FALSE);
		_super::ShowScrollBar(SB_VERT, FALSE);
		CRect rect;
		_super::GetClientRect(rect);
		m_horizontalParam.SetMax(rect.Width());
		m_verticalParam.SetMax(rect.Height());
	}
	/**
	 * サイズ変更通知処理
	 *	@param nType	サイズ変更のタイプ
	 *	@param cx		クライアント領域の新しい幅
	 *	@param cy		クライアント領域の新しい高さ
	 */
	void m_OnSize(WPARAM nType, int cx, int cy)
	{
		if ( m_originalSize.cx != 0 )
		{
			CRect rect;
			_super::GetWindowRect(rect);
			bool isEnable = !! (m_originalSize + m_offsetWinToClient != rect.Size());
			if ( isEnable )
			{
				if ( (rect.Height() - m_offsetWinToClient.cy) <= ::GetSystemMetrics(SM_CYHSCROLL) )
				{
					isEnable = false;
				}
			}
			_super::ScrollWindow(m_horizontalParam.SetPage(cx), m_verticalParam.SetPage(cy));
			m_horizontalParam.ShowBar(*this, SB_HORZ, isEnable);
			m_verticalParam.ShowBar(*this, SB_VERT, isEnable);
		}
	}
	/** 
	 * 水平スクロール処理
	 *	@param nSBCode		スクロール要求を示すスクロール バーのコード
	 *	@param nPos			スクロール ボックスの位置
	 *	@param hScrollBar	コントロールへのハンドル
	 */
	void m_OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
	{
		if ( hScrollBar == NULL )
		{
			int delta = m_horizontalParam.Scroll(nSBCode, nPos, m_lineStep);
			if ( delta != 0 )
			{
				_super::SetScrollPos(SB_HORZ, m_horizontalParam.AddPos(delta), TRUE);
				_super::ScrollWindow(-delta, 0);
			}
		}
	}
	/** 
	 * 垂直スクロール処理
	 *	@param nSBCode		スクロール要求を示すスクロール バーのコード
	 *	@param nPos			スクロール ボックスの位置
	 *	@param hScrollBar	コントロールへのハンドル
 	 */
	void m_OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
	{
		if ( hScrollBar == NULL )
		{
			int delta = m_verticalParam.Scroll(nSBCode, nPos, m_lineStep);
			if ( delta != 0 )
			{
				_super::SetScrollPos(SB_VERT, m_verticalParam.AddPos(delta), TRUE);
				_super::ScrollWindow(0, -delta);
			}
		}
	}
	/** 
	 * ホイール処理
	 *	@param fFlags フラグ
	 *	@param delta 移動量
	 *	@param lParam ポジション。
	 *	@retval true 処理した
	 *	@retval false なにもしていない
 	 */
	bool m_OnMouseWheel(UINT fFlags, short delta, LPARAM lParam)
	{
		if ( ! m_verticalParam.IsEnable() ) { return false; }
		int d = 0;
		if ( delta < 0)
		{
			d = m_verticalParam.Scroll(SB_LINEDOWN, 0, (-delta) / 10 + 1);
		}
		else if ( delta > 0 )
		{
			d = m_verticalParam.Scroll(SB_LINEUP, 0, (delta) / 10 + 1);
		}
		if ( d != 0 )
		{
			_super::SetScrollPos(SB_VERT, m_verticalParam.AddPos(d), TRUE);
			_super::ScrollWindow(0, -d);
		}
		return true;
	}
};



}; //MFC
}; //TNB



