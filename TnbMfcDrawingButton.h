#pragma once
/**
 *@file
 * 描画情報ボタン関係のヘッダ
 *
 *		描画情報を使用したボタンを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPartsDrawable.h"
#include "TnbMfcAbstractButton.h"
#include "TnbMfcBitmapDC.h"
#include "TnbWndTextDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報ボタンコントロール
 *
 *	@note リソースエディタでの設定のコントロールになります。
 *
 *	@par使い方
 *		リソースエディタでボタンかラジオボタンかチェックボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton）を本クラスに置き換えて使用します。
 *
 *	@see CAbstractButton
 *
 *	@par必要ファイル
 *			TnbMfcDrawingButton.h
 * 
 *	@date 07/11/24 新規作成
 *	@date 07/12/20 ３ステートチェックボックスのプッシュボタン型にも対応（Check時はDrawをハーフトーンで表示）。
 *	@date 07/12/20 プッシュボタンの動作を修正。
 */
class CDrawingButton : public CAbstractButton
{
	DEFSUPER(CAbstractButton);
	typedef CDrawTextBase _text;
	enum { TIMERID_DRAW = 1 };
	/// コントロール種類値
	enum EKind
	{
		BUTTON		= DFCS_BUTTONPUSH,		///< ボタン
		CHECKBOX	= DFCS_BUTTONCHECK,		///< チェックボックス
		CB3STATE	= DFCS_BUTTON3STATE,	///< 3ステートチェックボックス
		RADIOBUTTON	= DFCS_BUTTONRADIO,		///< ラジオボタン
	};
	bool					m_boIsAuto;				///< オート（コントロールのスタイル(途中変更未サポート)）
	int						m_checkState;			///< チェック状態 BST_UNCHECKEDなど
	CBitmapHandle			m_buttonBitmap;			///< 画面の画像
	CBitmapHandle			m_lastBitmap;			///< 最後に表示した画像
	int						m_bitmapChangeTime;
	// チェック変更（グループ内の他のをオフにする）
	void m_SetCheck(int c)
	{
		if ( c < 0 || c > 2 ) { c = 1;/*0以外なら1にしておく*/ }
		if ( m_checkState != c )
		{
			m_buttonBitmap.Null();
			m_checkState = c;
			_super::Invalidate();
		}
		if ( m_kind == RADIOBUTTON )
		{
			if ( c == BST_CHECKED )
			{
				CWnd* pParent = _super::GetParent();
				HWND hWnd = m_hWnd;
				while ( true )
				{
					HWND h = ::GetNextDlgGroupItem(*pParent, hWnd, true);
					if ( h == NULL || h == m_hWnd ) { break; }
					::SendMessage(h, BM_SETCHECK, BST_UNCHECKED, 0); 
					hWnd = h;
				}
				_super::ModifyStyle(0, WS_TABSTOP);
			}
			else
			{
				_super::ModifyStyle(WS_TABSTOP, 0);
			}
		}
	}
	EKind				m_kind;					///< コントロール種類(途中変更未サポート)
	LONG				m_originalStyle;		///< オリジナルスタイル
	LONG				m_originalExStyle;		///< オリジナル拡張スタイル
	IPartsDrawable*		m_pPartsDraw;			///< コントロールの描画IF
	IDrawable::Ptr		m_pNormalDraw;			///< ノーマル時の表示情報
	IDrawable::Ptr		m_pDisableDraw;			///< Disable時の表示情報
protected:

	/**
	 * [取得] 種類取得.
	 *	@return 種類
	 */
	EKind GetKind(void) const { return m_kind; }

	/**
	 * [取得] オリジナルスタイル取得.
	 *	@return BUTTONスタイル.
	 */
	LONG GetOriginalStyle(void) const { return m_originalStyle; }

	/**
	 * [取得] パーツ描画インターフェース取得.
	 *	@return パーツ描画インターフェース
	 */
	IPartsDrawable* ReferPartsDraw(void) { return m_pPartsDraw; }

	/**
	 * [確認] ボタンタイプ取得
	 *	@retval true プッシュボタンタイプ.
	 *	@retval false 否。
	 */
	bool IsPushButtonStyle(void) const
	{
		return (m_kind == BUTTON || (m_originalStyle & BS_PUSHLIKE) != 0);
	}
	
	/**
	 * [取得] 一行高さ.
	 *	@note チェックボックス、ラジオボタンの上付き下付きの位置を計算する時に使用されます。
	 *	@param pDC デバイスコンテキスト
	 *	@return 高さ
	 */
	virtual int GetLineHeight(CDC* pDC)
	{
		return 16;
	}

	/**
	 * [処理] 描画.
	 *	@param pDC デバイスコンテキスト。
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		if ( ! m_buttonBitmap.IsNull() )
		{
			m_buttonBitmap.Draw(*pDC);
			return;
		}
		MakeButtonImage(pDC);
		_super::KillTimer(TIMERID_DRAW);
		if ( m_lastBitmap.IsNull() || m_bitmapChangeTime == 0 )
		{
			m_buttonBitmap.Draw(*pDC);
			m_lastBitmap = m_buttonBitmap;
		}
		else
		{
			CBitmapImage bi;
			VERIFYLIB( bi.Set(m_buttonBitmap) );
			VERIFYLIB( bi.InsertOnAlphaBlend(0, 0, m_lastBitmap, 50) );//50%合成
			bi.Draw(*pDC);
			m_lastBitmap = m_buttonBitmap;
			_super::SetTimer(TIMERID_DRAW, m_bitmapChangeTime, NULL);
		}
	}

	/**
	 * [処理] 描画.
	 *	@param pDC デバイスコンテキスト。
	 */
	void MakeButtonImage(CDC* pDC)
	{
		CBitmapImage bi;
		bi.Set(MFCLIB::GetParentImage(this, CLR_AUTOSELECT));
		CBitmapDC dc(&bi);
		dc.SetTextColor(pDC->GetTextColor());
		CWnd* pWnd = _super::GetParent();
		if ( pWnd != NULL )
		{
			pWnd->SendMessage(WM_CTLCOLORBTN,
				reinterpret_cast<WPARAM>(dc.GetSafeHdc()), reinterpret_cast<LPARAM>(m_hWnd));
		}
		const IDrawable* pDraw = m_pNormalDraw;
		UINT uState = 0;
		#ifdef DFCS_TRANSPARENT
			uState |= DFCS_TRANSPARENT;
		#endif
		switch ( _super::GetButtonState() )
		{
		case ES_Pushed:
			uState |= DFCS_PUSHED;
			break;
		case ES_Hover:
			#ifdef DFCS_HOT
				uState |= DFCS_HOT;
			#endif
			break;
		case ES_Disable:
			uState |= DFCS_INACTIVE;
			if ( ! m_pDisableDraw.IsNull() )
			{
				pDraw = m_pDisableDraw;
			}
			break;
		}
		if ( HasFocus() )
		{
			uState |= DFCS_FOCUS;
		}
		//
		bool isPushType = IsPushButtonStyle();
		if ( m_kind == BUTTON )
		{
			uState |= DFCS_BUTTONPUSH;
		}
		else
		{
			EKind kind = m_kind;
			if ( kind == CB3STATE && m_checkState == BST_CHECKED )
			{
				kind = CHECKBOX;
			}
			if ( (m_originalStyle & BS_PUSHLIKE) != 0)
			{
				uState |= DFCS_BUTTONPUSH;
			}
			else
			{
				uState |= kind;
			}
			if ( kind == CB3STATE )
			{
				if ( m_checkState == BST_INDETERMINATE )
				{
					uState |= DFCS_CHECKED;
				}
			}
			else if ( m_checkState == BST_CHECKED )
			{
				uState |= DFCS_CHECKED;
			}
		}
		#ifdef DFCS_FLAT
			if ( (m_originalStyle & BS_FLAT) != 0 )
			{
				uState |= DFCS_FLAT;
			}
		#endif
		CRect rc;
		_super::GetClientRect(rc);
		if ( isPushType )
		{
			m_pPartsDraw->DrawButtonControl(dc, rc, uState);
		}
		else
		{
			CRect r = rc;
			m_pPartsDraw->DrawButtonControl(NULL, r, uState);
			int l = (GetLineHeight(pDC) - r.Height() + 1) / 2;
			switch( m_originalStyle & 0x0C00 )
			{
			case BS_TOP:/*0x00000400L*/
				r.OffsetRect(0, l);
				break;
			case BS_BOTTOM:/*0x00000800L*/
				r.OffsetRect(0, rc.Height() - r.bottom - l);
				break;
			default:
			case BS_VCENTER:/*0x00000C00L*/	
				r.OffsetRect(0, (rc.Height() - r.bottom) / 2);
				break;
			}
			if ( (m_originalStyle & BS_LEFTTEXT) != 0 || (m_originalExStyle & WS_EX_RIGHT) != 0 )
			{
				//左に
				r.left = rc.right - r.right;
				r.right = rc.right;
				m_pPartsDraw->DrawButtonControl(dc, r, uState);
				rc.right -= r.Width() + 3;
			}
			else
			{
				//右に
				m_pPartsDraw->DrawButtonControl(dc, r, uState);
				rc.left += r.Width() + 3;
			}
		}
		if ( pDraw != NULL )
		{
			SIZE size;
			pDraw->GetSize(size);
			int x = 0;
			int y = (rc.Height() - size.cy) / 2;
			if ( isPushType )
			{
				x = (rc.Width() - size.cx) / 2;
				if ( _super::GetButtonState() == ES_Pushed )
				{
					POINT po;
					m_pPartsDraw->GetPushedOffset(po);
					rc.OffsetRect(po);
				}
			}
			else if ( (m_originalExStyle & WS_EX_RIGHT) != 0 )
			{
				x = rc.Width() - size.cx;
			}
			pDraw->Draw(dc, rc.left + x, rc.top + y);
			//
			if ( isPushType && m_kind == CB3STATE && m_checkState == BST_INDETERMINATE )
			{
				CRect rc;
				_super::GetClientRect(rc);
				CBitmapImage bi;
				bi.Set(rc.Width(), rc.Height());
				{
					CBitmapDC d(&bi);
					m_pPartsDraw->DrawButtonControl(d, rc, uState);
				}
				bi.AlphaBlend(dc, 0, 0, 50);
			}
			//
			if ( HasFocus() )
			{
				if ( isPushType )
				{
					POINT po;
					m_pPartsDraw->GetEdgeOffset(po);
					rc.DeflateRect(po.x + 1, po.y + 1);
					if ( _super::GetButtonState() == ES_Pushed )
					{
						CPoint po;
						m_pPartsDraw->GetPushedOffset(po);
						rc.OffsetRect(-po);
					}
				}
				else
				{
					rc.left += x;
					rc.top += y;
					rc.right = rc.left + size.cx;
					rc.bottom = rc.top + size.cy;
				}
				m_pPartsDraw->DrawFocusRect(dc, &rc);
			}
		}
		CBitmapImage* P = dc.Detach();
		m_buttonBitmap = P->GetBitmapHandle();
	}

	/**
	 * [通知] クリック.
	 *		BN_CLICKED を親に通知する前にコールされます。
	 */
	virtual void OnClickButton(void)
	{
		if ( m_boIsAuto )
		{
			int c = BST_CHECKED;
			if ( m_kind != RADIOBUTTON )
			{
				switch( m_checkState )
				{
				default:
					ASSERT( false );
					//以下へ続く
				case BST_UNCHECKED:
					c = BST_CHECKED;
					break;
				case BST_CHECKED:
					c = (m_kind == CB3STATE) ? BST_INDETERMINATE : BST_UNCHECKED;
					break;
				case BST_INDETERMINATE:
					c = BST_UNCHECKED;
					break;
				};
			}
			m_SetCheck(c);
		}
	}

	/**
	 * [通知] 状態変化通知.
	 *	@note 現在の状態を取得します
	 *	@param state 状態。
	 */
	virtual void OnChangeState(EState state)
	{
		m_buttonBitmap.Null();
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
		switch( message )
		{
		case WM_TIMER:
			if ( wParam == TIMERID_DRAW )
			{
				KillTimer(TIMERID_DRAW);
				CClientDC dc(this);
				m_lastBitmap.Draw(dc);
			}
			break;
		case WM_GETDLGCODE:
			if ( m_kind == RADIOBUTTON )
			{
				return DLGC_RADIOBUTTON | DLGC_BUTTON;
			}
			else if ( m_kind != BUTTON )
			{
				return DLGC_BUTTON;
			}
			break;
		case BM_GETSTATE:
			return _super::WindowProc(message, wParam, lParam) | m_checkState;
		case BM_GETCHECK:
			return m_checkState;
		case BM_SETCHECK:
			m_SetCheck(ToInt(wParam));
			return 0;
		case BM_SETSTYLE:
			if ( lParam != 0 ) { RedrawWindow(); }
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
		m_checkState = BST_UNCHECKED;
		m_originalStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		m_originalExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		m_boIsAuto = false;
		bool canSendDblClk = ((m_originalStyle & BS_NOTIFY) != 0);
		switch ( m_originalStyle & BS_TYPEMASK )
		{
		case BS_AUTOCHECKBOX:
			m_boIsAuto = true;
			m_kind = CHECKBOX;	//チェックボックス
			break;
		case BS_CHECKBOX:
			m_kind = CHECKBOX;	//チェックボックス
			break;
		case BS_AUTO3STATE:
			m_boIsAuto = true;
			m_kind = CB3STATE;	//3ステートチェックボックス
			break;
		case BS_3STATE:
			m_kind = CB3STATE;	//3ステートチェックボックス
			break;
		case BS_AUTORADIOBUTTON:
			m_boIsAuto = true;
			m_kind = RADIOBUTTON;	//ラジオボタン
			break;
		case BS_RADIOBUTTON:
			canSendDblClk = true;
			m_kind = RADIOBUTTON;	//ラジオボタン
			break;
		case BS_DEFPUSHBUTTON:
			{
				CWnd* P = _super::GetParent();
				if ( P != NULL ) { P->SendMessage(DM_SETDEFID, _super::GetDlgCtrlID(), 0); }
			}
			m_kind = BUTTON;		//普通のボタン
			break;
		default:
			m_kind = BUTTON;		//普通のボタン
			break;
		}
		_super::SetSendDblClkMode(canSendDblClk);
		_super::PreSubclassWindow();
		m_buttonBitmap.Null();
	}

public:

	/// コンストラクタ
	CDrawingButton(void) : _super(), m_pPartsDraw(NULL), m_bitmapChangeTime(50)
	{
		SetPartsDrawer(NULL);
	}

	/**
	 * [設定] パーツ描画指定.
	 *	@param pPartsDraw パーツ描画。参照のみ記憶します。破棄しないでください。
	 *					未指定時や、 NULL 指定時は、標準のコントロールを表示します。
	 *	@note 本クラスでは、 IPartsDrawable の GetPushedOffset() , DrawFrameControl() , DrawFocusRect() を使用します。
	 */
	void SetPartsDrawer(IPartsDrawable* pPartsDraw = NULL)
	{
		if ( pPartsDraw != NULL )
		{
			m_pPartsDraw = pPartsDraw;
		}
		else
		{
			static CStandardPartsDrawer s_partsDrawer;
			m_pPartsDraw = &s_partsDrawer;
		}
	}

	/**
	 * [設定] ビットマップ切り替え時間設定.
	 *		状態が変わってビットマップを変更する時、デフォルトでは、切り替え前のビットマップと、
	 *		切り替え後のビットマップの50％合成の画像を 30ms 秒表示するようになっています（そのため
	 *		ふわっと切り替わるように見えます）。この切り替え時間を本メソッドで指定することが出来ます。
	 *	@note 合成画像は一枚しか用意しないため、100msぐらいが上限です。
	 *	@param ms 切り替え時間（単位 ms）。
	 */
	void SetBitmapChangeTime(UINT ms)
	{
		m_bitmapChangeTime = ms;
	}

	/**
	 * [設定] 通常時の描画情報指定.
	 *	@param draw 描画情報。クローンを記憶しまうので、破棄してもかまいません。
	 */
	void SetNormalDrawer(const IDrawable& draw)
	{
		m_pNormalDraw = draw.Clone();
		if ( ::IsWindow(*this) )
		{
			m_buttonBitmap.Null();
		}
	}

	/**
	 * [設定] Disable時の描画情報指定.
	 *	@param draw 描画情報。クローンを記憶しまうので、破棄してもかまいません。
	 */
	void SetDisableDrawer(const IDrawable& draw)
	{
		m_pDisableDraw = draw.Clone();
		if ( ::IsWindow(*this) )
		{
			m_buttonBitmap.Null();
		}
	}

	/**
	 * [設定] サイズ調整.
	 *		コントロールの大きさを現在の設定に適した(最小の)大きさに変更します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AdjustSize(void)
	{
		if ( m_kind == BUTTON || (m_originalStyle & BS_PUSHLIKE) != 0 )
		{
			if ( ! m_pNormalDraw.IsNull() )
			{
				SIZE sz;
				m_pNormalDraw->GetSize(sz);
				POINT po;
				m_pPartsDraw->GetEdgeOffset(po);
				return MFCLIB::ChangeClientSize(this, sz.cx + po.x * 2, sz.cy + po.y * 2);
			}
			return true; //調整なし
		}
		//
		UINT uState = 0;
		switch ( m_kind )
		{
		case RADIOBUTTON:	uState = DFCS_BUTTONRADIO;	break;
		case CHECKBOX:		uState = DFCS_BUTTONCHECK;	break;
		case CB3STATE:		uState = DFCS_BUTTON3STATE;	break;
		default:	
			ASSERTLIB( false );	
			break;
		}
		CRect rc;
		_super::GetClientRect(rc);
		m_pPartsDraw->DrawButtonControl(NULL, rc, uState);
		SIZE size = { 0, 0 };
		if ( ! m_pNormalDraw.IsNull() )
		{
			m_pNormalDraw->GetSize(size);
		}
		size.cx += rc.Width() + 3;
		size.cy = max(size.cy, rc.Height());
		return MFCLIB::ChangeClientSize(this, size.cx, size.cy);
	}

	/**
	 * [取得] チェック状態取得.
	 *	@retval BST_UNCHECKED		ボタンがオフの状態
	 *	@retval BST_CHECKED			ボタンがオンの状態
	 *	@retval BST_INDETERMINATE	ボタンの状態が不確定 (ボタンに BS_3STATE スタイルまたは BS_AUTO3STATE スタイルが設定されているときだけ)
 	 */
	int GetCheck(void) const
	{
		return static_cast<int>(::SendMessage(m_hWnd, BM_GETCHECK, 0, 0)); 
	}
	
	/**
	 * [設定] チェック状態設定.
	 *	@param nCheck 以下のパラメータを指定できます。\n
	 *		<table><tr><td>
	 *		パラメータ							</td><td>
	 *					説明					</td></tr><tr><td>
	 *		BST_UNCHECKED						</td><td>
	 *					ボタンがオフの状態		</td></tr><tr><td>
	 *		BST_CHECKED							</td><td>
	 *					ボタンがオンの状態		</td></tr><tr><td>
	 *		BST_INDETERMINATE					</td><td>
	 *					ボタンの状態が不確定	</td></tr>
	 *		</table>
 	 */
	void SetCheck(int nCheck)
	{
		::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0); 
	}
};



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報ボタンコントロール
 *
 *		inline なコントロールです。
 *
 *		リソースエディタでボタンかラジオボタンかチェックボックスコントロールを張り、クラスに割り当て後、
 *		本クラスに置き換えて使用します。
 *
 *		リソースで書かれた文字列を表示します。
 *
 *	@note リソースエディタでの設定のままのボタンコントロールになります。
 *
 *	@note SetPartsDrawer() , SetNormalDrawer() , SetDisableDrawer() を指定しないと、
 *		透過するだけのただの普通のボタンコントロールになります。
 *
 *	@see CAbstractButton
 *
 *	@par必要ファイル
 *			TnbMfcDrawingButton.h
 * 
 *	@date 07/11/24 新規作成
 */
class CDrawingButtonTx : public CDrawingButton
{
	DEFSUPER(CDrawingButton);
protected:
	COLORREF		m_color;	///< 文字の色
	CWndTextDrawer*	m_pText;	///< 文字表示
	/// 文字表示設定
	void SetTextDrawer(CWndTextDrawer* D)
	{
		if ( m_pText != NULL )
		{
			delete m_pText;
			m_pText = NULL;
		}
		m_pText = D;
		if ( m_pText != NULL )
		{
			m_pText->SetTextStyle(CTextDrawer::ButtonToDrawStyle(_super::GetOriginalStyle()));
			SetTextColor(m_color);
		}
	}
	/**
	 * [取得] 一行高さ.
	 *	@note チェックボックス、ラジオボタンの上付き下付きの位置を計算する時に使用されます。
	 *	@param pDC デバイスコンテキスト
	 *	@return 高さ
	 */
	virtual int GetLineHeight(CDC* pDC)
	{
		CFont* pFont = _super::GetFont();
		if ( pFont != NULL )
		{
			CFont* old = pDC->SelectObject(pFont);
			TEXTMETRIC t;
			pDC->GetTextMetrics(&t);
			pDC->SelectObject(old);
			return t.tmHeight + t.tmExternalLeading * 2;
		}
		return 16; //適当
	}

	/**
	 * [確認] フォーカスの有無
	 *	@retval true フォーカスあり。
	 *	@retval false フォーカスなし。
	 */
	virtual bool HasFocus(void) const
	{
		return _super::IsPushButtonStyle() ? _super::HasFocus() : false;
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		CRect rect;
		_super::GetClientRect(&rect);
		CWndTextDrawer* pText = NULL;
		if ( ! _super::IsPushButtonStyle() )
		{
			IPartsDrawable* pParts = _super::ReferPartsDraw();
			CRect r = rect;
			pParts->DrawButtonControl(NULL, r, _super::GetKind());
			rect.right -= r.Width() + 3;
			pText = new CWndTextDrawer(rect.Size(), *this, m_color, 0, 2, 2, pParts);
		}
		else
		{
			POINT po;
			_super::ReferPartsDraw()->GetEdgeOffset(po);
			pText = new CWndTextDrawer(rect.Size(), *this, m_color, 0, po.x + 2, po.y + 2, NULL);
		}
		SetTextDrawer(pText);
	}
public:

	/// コンストラクタ
	CDrawingButtonTx(void) : m_color(CLR_AUTOSELECT), m_pText(NULL)
	{
	}

	/// デストラクタ
	~CDrawingButtonTx(void)
	{
		if ( m_pText != NULL )
		{
			delete m_pText;
			m_pText = NULL;
		}
	}

	/**
	 * [設定] テキストカラー設定
	 *	@note デフォルトでは、デバイスコンテキストに設定されている色を使います.
	 *	@param color テキストカラー.
	 */
	void SetTextColor(COLORREF color)
	{
		m_color = color;
		if ( m_pText != NULL )
		{
			m_pText->SetTextColor(m_color);
			_super::SetNormalDrawer(*m_pText);
			m_pText->SetTextColor(CLR_INVALID);
			_super::SetDisableDrawer(*m_pText);
			_super::Invalidate();
		}
	}

	/**
	 * [設定] サイズ調整.
	 *		コントロールの大きさを現在の設定に適した(最小の)大きさに変更します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AdjustSize(void)
	{
		if ( m_pText != NULL )
		{
			CRect rc;
			GetClientRect(&rc);
			// 文字列の描画大きさ計算
			CString s;
			_super::GetWindowText(s);
			DWORD style = CTextDrawer::ButtonToDrawStyle(_super::GetOriginalStyle());
			CDC* pDC = GetDC();
			CGdiObject* old = pDC->SelectObject(_super::GetFont());
			CSize textSize = rc.Size();
			CTextDrawer::CalcTextSize(textSize, *pDC, style, s);
			pDC->SelectObject(old);
			ReleaseDC(pDC);
			textSize.cx += 3;
			textSize.cy += 4;
			// 描画情報更新
			CWndTextDrawer* pText = NULL;
			if ( ! _super::IsPushButtonStyle() )
			{
				IPartsDrawable* pParts = _super::ReferPartsDraw();
				pText = new CWndTextDrawer(textSize, *this, m_color, 0, 2, 2, pParts);
			}
			else
			{
				POINT po;
				_super::ReferPartsDraw()->GetEdgeOffset(po);
				pText = new CWndTextDrawer(textSize, *this, m_color, 0, po.x + 2, po.y + 2, NULL);
			}
			SetTextDrawer(pText);
			return _super::AdjustSize();
		}
		return false;
	}
};



}; //MFC
}; //TNB



