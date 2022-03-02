#pragma once
/**
 *@file
 * ボタン抽象関係のヘッダ
 *
 *		ボタンを作成するためのフレームを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#define BN_PUSHED_T			0x100
#define BN_UNPUSHED_T		0x101
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ボタンコントロール抽象クラス
 *
 *		本クラスを継承して自由なボタンコントロールを作成できます。
 *
 *	@note マウスが上に乗った時に BN_HILITE 、離れた時に BN_UNHILITE を、親に通知しています。
 *	@par使用例
 *		\code
 *		//= 宣言
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_BN_CLICK(IDC_BMP_KAGE2, OnButtonClick)		// クリックした時
 *		ON_CONTROL(BN_HILITE, IDC_BMP_KAGE2, OnButtonHover) // 上にマウスが来た時		
 *			:
 *		\endcode
 *
 *	@note プレスモードの時、プレスした時、 BN_PUSHED_T 、リリース時に BN_UNPUSHED_T を、親に通知しています。
 *	@par使用例
 *		\code
 *		//= 宣言
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_CONTROL(BN_PUSHED_T, IDC_BMP_KAGE2, OnButtonPushed) // プレスした時
 *		ON_CONTROL(BN_UNPUSHED_T, IDC_BMP_KAGE2, OnButtonUnpushed) // リリースした時
 *			:
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbMfcAbstractButton.h
 * 
 *	@date 07/10/04 新規作成
 *	@date 07/10/11 Enterキーに対応（親がダイアログベース以外は対象外）。
 *	@date 07/10/17 ショートカットに対応。
 *	@date 07/10/19 親に通知する前に処理を入れられるように対応。
 *	@date 07/10/22 マウスホバーで親に通知するようにした。WM_NCPAINT,WM_ENABLE の処理、改善
 *	@date 07/10/26 WM_SETFONT,WM_GETFONT 対応。メソッド名、整理。
 *	@date 07/10/31 WM_PRINTCLIENT に対応。
 *	@date 07/11/07 状態変化通知追加。
 *	@date 07/11/12 WM_COMMAND の LPARAM 内容修正。
 *	@date 07/11/20 Window破棄後のメッセージ受信処理の対応(修正)。
 *	@date 07/12/20 リソースエディタにボタンを適用した時ごみが出るのを修正。
 *	@date 07/12/27 BM_CLICK イベント処理を改善。 OnClickButton() の通知タイミングを改善。
 *	@date 08/02/06 SetDefaultButtonStyle() を追加。
 *	@date 08/02/07 ホバー状態を整理。 BN_DOUBLECLICKED サポート。
 *	@date 08/02/18 プレス時の KillFocus の処理を改善。
 *	@date 08/02/22 フォーカスフラグ,廃止
 *	@date 08/03/03 EP_Message 時、二回親に押下通知されるのを修正。
 *	@date 08/03/10 自分生存確認を強化。
 *	@date 09/09/25 構成変更。
 *	@date 10/11/09 SetPressedMode() を追加。押し、離しのイベントを新規
 *	@date 14/05/20 EPressMode を追加。 SetPressedMode(EPressMode) を追加。
 */
class CAbstractButton : public CWnd
{
	DEFSUPER(CWnd);
public:

	/// コンストラクタ
	CAbstractButton(void) 
		: m_shortcutKey(0), m_pushState(EP_Non), m_boHasHover(false)
		, m_nowState(ES_Normal), m_lastState(EState(-1)), m_hFont(NULL), m_boInBmClick(false)
		, m_boIsOnClickDisp(false), m_canSendDblClk(false), m_pressedMode(PM_Unused)
	{
	}

	/**
	 * コピーオペレータ.
	 *	@note フォントハンドル、ショートカットキー情報のみ複製します。
	 *	@param other コピー元。
	 *	@return 自分の参照
	 */
	CAbstractButton& operator=(const CAbstractButton& other)
	{
		m_shortcutKey = other.m_shortcutKey;
		m_hFont = other.m_hFont;
		return *this;
	}

	/// デストラクタ
	virtual ~CAbstractButton(void)
	{
	}

	/**
	 * [設定] ショートカットキー設定.
	 *		フォーカスが無いときでもキーボードの ALT + で押せるショートカットキーに対応するキーを指定します。
	 *		指定しない場合、ショートカットは出来ません。
	 *	@param key キー。 'A' ～ 'Z' を指定します。
	 */
	void SetShortcutKey(TCHAR key)
	{
		m_shortcutKey = key;
	}

	/**
	 * [設定] デフォルトボタンスタイル設定.
	 *		本ボタンにデフォルトボタンスタイルをつけます。
	 *	@note 他のボタンにフォーカスがない時に、リターンを押下すると動作するのが、デフォルトボタンです。
	 */
	void SetDefaultButtonStyle(void)
	{
		CWnd* P = _super::GetParent();
		if ( P != NULL ) 
		{
			P->SendMessage(DM_SETDEFID, _super::GetDlgCtrlID(), 0);
		}
	}

	/**
	 * [取得] 状態取得.
	 *		オプション ボタンまたはチェック ボックスの状態を取得します。
	 *	@return 状態。次のマスク値が使用できます。
	 *		<table>
	 *		<tr><td>マスク</td>	<td>説明</td></tr>
	 *					<tr><td>
	 *		0x0003		</td><td>
	 *				オン、オフ状態を調べます (オプション ボタンとチェック ボックスの場合だけ)。
	 *				0 のときは、ボタンはオフです。1 のときは、ボタンはオンです。オプション ボタンはチェック マーク (?) が付いている状態がオンです。チェック ボックスは X マークが付いている状態がオンです。2 は、チェック状態が不確定であることを示します (3 ステート チェック ボックスのみ)。
	 *				3 ステート チェック ボックスは淡色表示状態のとき不確定状態を示します。</td>
	 *					</tr><tr><td>
	 *		0x0004		</td><td>
	 *				強調表示状態を調べます。0 以外の値のときは、ボタンが強調表示状態です。
	 *				マウスの左ボタンをクリックしたままにすると、ボタンは強調表示状態になります。
	 *				マウス ボタンを離すと、強調表示状態は解除されます。</td>
	 *					</tr><tr><td>
	 *		0x0008		</td><td>
	 *				フォーカス状態を調べます。0 以外の値のときは、ボタンにフォーカスがあることを示します。</td>
	 *					</tr>
	 *		</table>
	 */
	UINT GetState(void) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return static_cast<UINT>(::SendMessage(m_hWnd, BM_GETSTATE, 0, 0));
	} 

	/// プレスモード種
	enum EPressMode
	{
		PM_Unused,		///< 通常ボタン
		PM_Message,		///< プレスメッセージ対応
		PM_Lockable,	///< プレスロック機能も有効
	};

	/**
	 * [設定] プレスモード設定.
	 *		プレスロックモード付きのボタンにすることが出来ます。
	 *		プレスモードにすると、プレスすると親ウィンドウに BN_PUSHED_T が、
	 *		リリースすると BN_UNPUSHED_T が通知されます。
	 *	@param isEnable true ならプレスモード。 false なら通常ボタンにします。
	 */
	void SetPressedMode(bool isEnable)
	{
		m_pressedMode = isEnable ? PM_Lockable : PM_Unused;
	}	
	
	/**
	 * [設定] プレスモード設定.
	 *		プレスメッセージ、プレスロックモード付きのボタンにすることが出来ます。
	 *		プレスモードにすると、プレスすると親ウィンドウに BN_PUSHED_T が、
	 *		リリースすると BN_UNPUSHED_T が通知されます。
	 *	@param pm プレスモード
	 */
	void SetPressedMode(EPressMode pm)
	{
		m_pressedMode = pm;
	}

protected:
	
	/// 状態
	enum EState 
	{
		ES_Normal,				///< 標準状態
		ES_Hover,				///< ホバー状態
		ES_Pushed,				///< 押下状態
		ES_Disable,				///< 無効状態
	};

	/**
	 * [取得] ボタン状態取得.
	 *	@note 現在の状態を取得します
	 *	@return 状態。
	 */
	EState GetButtonState(void) const
	{
		return m_nowState;
	}

	/**
	 * [設定] ボタン状態設定.
	 *	@note 内部情報を書き換えるだけです。再表示はしません。
	 *	@param s 状態。
	 */
	void SetButtonState(EState s)
	{
		m_nowState = s;
	}

	/**
	 * [設定] ダブルクリックイベントサポート
	 *	@note デフォルトでは false です。
	 *	@param canSendDblClk true なら、 BN_DOUBLECLICKED を親に通知するようになります。 
	 */
	void SetSendDblClkMode(bool canSendDblClk)
	{
		m_canSendDblClk = canSendDblClk;
	}

	/**
	 * [処理] 描画.
	 *	@param pDC デバイスコンテキスト。
	 */
	virtual void OnDrawButton(CDC* pDC) = 0;

	/**
	 * [確認] ポイントがボタン上にあるか.
	 *	@param po スクリーン座標。
	 *	@retval true 上にある。
	 *	@retval false ない。
	 */
	virtual bool IsOnButton(const POINT& po) const
	{
		return ::WindowFromPoint(po) == m_hWnd;
	}

	/**
	 * [通知] 現在の状態確認通知
	 *	@param st 現在の状態
	 *	@return 新しい状態
	 */
	virtual EState OnCheckState(EState st)
	{
		if ( m_IsAlive() )
		{
			if ( ! _super::IsWindowEnabled() )
			{
				st = ES_Disable;
			}
			else if ( m_pushState == EP_Mouse )
			{
				st = ES_Pushed;
				CPoint po = m_curpos;
				ClientToScreen(&po);
				if ( ! IsOnButton(po) )
				{
					st = ES_Normal;
				}
			}
			else if ( m_pushState != EP_Non )
			{
				st = ES_Pushed;
			}
			else if ( m_boHasHover )
			{
				st = ES_Hover;
			}
			else
			{
				st = ES_Normal;
			}
		}
		return st;
	}

	/**
	 * [確認] フォーカスの有無
	 *	@retval true フォーカスあり。
	 *	@retval false フォーカスなし。
	 */
	virtual bool HasFocus(void) const
	{
		return ::GetFocus() == m_hWnd;
	}

	/**
	 * [通知] クリック.
	 *		BN_CLICKED を親に通知する前にコールされます。
	 */
	virtual void OnClickButton(void)
	{
	}

	/**
	 * [通知] 状態変化通知.
	 *	@note 現在の状態を通知します。フォーカスが変化した時も通知します（stateは同じ値です）。
	 *	@param state 状態。
	 */
	virtual void OnChangeState(EState state)
	{
	}

	/**
	 * [通知] for notifications from parent
	 *	@note
	 *		CWndのメソッドをオーバーライドしています。
	 *		メッセージ受信したらコールされます。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE	処理済。
	 *	@retval FALSE	未処理。
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_COMMAND && lParam == reinterpret_cast<LPARAM>(m_hWnd) )
		{
			ASSERTLIB( (wParam & 0xFFFF) == static_cast<WPARAM>(GetDlgCtrlID()) );
			ASSERTLIB( (BN_CLICKED * 0x10000) == 0 );
			if ( (wParam & 0xFFFF0000) == 0 )
			{
				if ( ! m_boIsOnClickDisp )
				{
					OnClickButton();
				}
				else
				{
					m_boIsOnClickDisp = false;
				}
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
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
		case 0x0128/*WM_UPDATEUISTATE*/:
			TTRACE0("WM_UPDATEUISTATE\n");
			return 0;
		case WM_ERASEBKGND:
			TTRACE0("WM_ERASEBKGND\n");
			return 0;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				if ( pDC != NULL )
				{
					OnDrawButton(pDC);
				}
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // 描画用のデバイス コンテキスト
				OnDrawButton(&dc);
			}
			return 0;
		case 0x031A/*WM_THEMECHANGED*/:
			OnChangeState(m_nowState);
			break;
		case WM_SETFOCUS:
			TTRACE0("WM_SETFOCUS\n");
			OnChangeState(m_nowState);
			m_RedrawWindow();
			return 0;
		case WM_KILLFOCUS:
			TTRACE0("WM_KILLFOCUS\n");
			if ( m_pushState != EP_Non && ! m_IsPressedLockMode() )
			{
				m_pushState = EP_Non;
				::ReleaseCapture();
				m_HoverOff(false);
				m_CheckNowState(false);
				m_RedrawWindow();
			}
			else
			{
				OnChangeState(m_nowState);
			}
			m_RedrawWindow();
			return 0;
		case WM_GETDLGCODE:
			TTRACE0("WM_GETDLGCODE\n");
			if ( HasFocus() )
			{
				return DLGC_DEFPUSHBUTTON | DLGC_BUTTON;
			}
			return DLGC_UNDEFPUSHBUTTON | DLGC_BUTTON;
		case WM_GETTEXTLENGTH:
			TTRACE0("WM_GETTEXTLENGTH\n");
			if ( m_shortcutKey != 0 )
			{
				return 2 * sizeof(TCHAR);
			}
			break;
		case WM_GETTEXT:
			TTRACE0("WM_GETTEXT\n");
			if ( m_shortcutKey != 0 && wParam >= 2 )
			{
				TCHAR* P = reinterpret_cast<TCHAR*>(lParam);
				*P++ = '&';
				*P++ = m_shortcutKey;
				return 2;
			}
			break;
		case WM_KEYDOWN:
			TTRACE2("WM_KEYDOWN 0x%04X,0x%04X\n",wParam,lParam);
			if ( wParam == ' ' )
			{
				m_Push(EP_Key);
			}
			return 0;
		case WM_KEYUP:
			TTRACE2("WM_KEYUP 0x%04X,0x%04X\n",wParam,lParam);
			if ( wParam == ' ' )
			{
				m_Unpush(EP_Key);
				if ( m_IsPressedLockMode() )
				{
					m_Unpush(EP_Mouse);
				}
			}
			return 0;
		case WM_LBUTTONDOWN:
			TTRACE0("WM_LBUTTONDOWN\n");
//			m_curpos.x = LOWORD(lParam);
//			m_curpos.y = HIWORD(lParam);
			m_curpos = CPoint(lParam);
			m_Push(m_boInBmClick ? EP_Key : EP_Mouse);
			return 0;
		case WM_LBUTTONUP:
			TTRACE0("WM_LBUTTONUP\n");
			m_Unpush(m_boInBmClick ? EP_Key : EP_Mouse);
			return 0;
		case WM_LBUTTONDBLCLK:
			TTRACE0("WM_LBUTTONDBLCLK\n");
			if ( ! m_canSendDblClk )
			{
				m_Push(EP_Mouse);
			}
			else
			{
				m_SendCommand(BN_DBLCLK/*BN_DOUBLECLICKED*/);
			}
			return 0;
		case WM_MOUSEMOVE:
			TTRACE1("WM_MOUSEMOVE %X\n", lParam);
			m_curpos = CPoint(lParam);
			if ( ! m_boHasHover && m_pushState == EP_Non )
			{
				m_HoverOn(true);
			}
			else
			{
				m_CheckNowState(true);
			}
			break;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			m_CheckNowState(true);
			return 0;
		case WM_GETFONT:
			TTRACE0("WM_GETFONT\n");
			{
				HFONT f = m_hFont;
				if ( f == NULL )
				{
					f = reinterpret_cast<HFONT>(::SendMessage(::GetParent(m_hWnd), WM_GETFONT, 0, 0));
				}
				if ( f == NULL )
				{
					f = reinterpret_cast<HFONT>(::GetStockObject(SYSTEM_FONT));
				}
				return reinterpret_cast<LRESULT>(f);
			}
			break;
		case WM_SETFONT:
			TTRACE0("WM_SETFONT\n");
			m_hFont = reinterpret_cast<HFONT>(wParam);
			if ( lParam != 0 )
			{
				m_RedrawWindow();
			}
			return 0;
		case WM_DESTROY:
			TTRACE0("WM_DESTROY\n");
			m_pushState = EP_Non;
			m_boHasHover = false;
			SetButtonState(ES_Normal);
			m_lastState = static_cast<EState>(-1);
			m_hFont = NULL;
			m_boInBmClick = false;
			m_boIsOnClickDisp = false;
			break;
		case BM_CLICK:
			TTRACE0("BM_CLICK\n");
			{
				m_boInBmClick = true;
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_boInBmClick = false;
				return r;
			}
			break;
		case BM_GETSTATE:
			TTRACE0("BM_GETSTATE\n");
			{
				UINT r = 0;
				r |= (m_nowState == ES_Pushed) ? BST_PUSHED : 0;//強調
				r |= HasFocus() ? BST_FOCUS : 0;//フォーカス
				return r;
			}
			break;
		case BM_SETSTATE:
			TTRACE0("BM_SETSTATE\n");
			if ( wParam == 0 )
			{
				m_Unpush(EP_Message);
			}
			else
			{
				m_Push(EP_Message);
			}
			return 0;
		case 0x02A1/*WM_MOUSEHOVER*/:
			TTRACE0("WM_MOUSEHOVER\n");
			m_HoverOn(true);
			break;
		case 0x02A3/*WM_MOUSELEAVE*/:
			TTRACE0("WM_MOUSELEAVE\n");
			if ( m_boHasHover )
			{
				m_HoverOff(true);
			}
			else
			{
				m_CheckNowState(true);
			}
			break;
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
		m_CheckNowState(false);
	}

private:
	/// 押下種類
	enum EPush
	{
		EP_Non,		///< 押下して無い
		EP_Mouse,	///< マウスによる押下
		EP_Key,		///< キーによる押下
		EP_Message	///< メッセージによる押下
	};
	EPush		m_pushState;			///< 押下種類
	bool		m_boHasHover;			///< true ホバー中
	bool		m_boInBmClick;			///< BM_CLICK 処理中フラグ
	bool		m_boIsOnClickDisp;		///< OnClickButton コール済み;
	TCHAR		m_shortcutKey;			///< ショートカットキーコード（ALT+ で押されたことになる）
	HFONT		m_hFont;				///< フォント
	bool		m_canSendDblClk;		///< BN_DOUBLECLICKED 送信サポート
	EState		m_nowState;				///< 現在の状態
	EState		m_lastState;			///< 最後に描画した State
	CPoint		m_curpos;				///< 現在のポジション
	EPressMode	m_pressedMode;			///< 押下しっ放し状態をもつ？
	bool m_WithPressedMessage(void) const
	{
		return m_pressedMode != PM_Unused;
	}
	bool m_IsPressedLockMode(void) const
	{
		return m_pressedMode == PM_Lockable;
	}
	/// 生存確認
	bool m_IsAlive(void) const
	{
		return !! ::IsWindow(GetSafeHwnd());
	}
	/// 親にコマンドをPostする。
	void m_SendCommand(UINT uCmd)
	{
		if ( m_IsAlive() )
		{
			MFCLIB::SendCommandMessage(this, uCmd);
		}
	}
	/// リドロー
	void m_RedrawWindow(void)
	{
		if ( m_IsAlive() )
		{
			_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
		}
	}
	/// 現在のインスタンスの状態、ウィンドウの状態をチェックし、 nowState を設定
	void m_CheckNowState(bool boRedraw)
	{
		if ( m_IsAlive() )
		{
			SetButtonState(OnCheckState(m_nowState));
			if ( m_nowState != m_lastState )
			{
				m_lastState = m_nowState;
				OnChangeState(m_nowState);
				if ( boRedraw )
				{
					m_RedrawWindow();
				}
			}
		}
	}
	/// Hover On
	void m_HoverOn(bool boRedraw)
	{
		#ifndef _WIN32_WCE
		if ( ! m_boHasHover )
		{
			m_boHasHover = true;
			m_CheckNowState(boRedraw);
			m_SendCommand(BN_HILITE);
			m_HoverSub();
		}
		#endif
	}
	/// Hover OFF
	void m_HoverOff(bool boRedraw)
	{
		#ifndef _WIN32_WCE
		if ( m_boHasHover )
		{
			m_boHasHover = false;
			m_CheckNowState(boRedraw);
			m_SendCommand(BN_UNHILITE);
		}
		#endif
	}
	/// 押下処理 
	void m_Push(EPush ep)
	{
		if ( ! m_IsAlive() ) { return; }
		if ( m_pushState == EP_Non )
		{
			m_pushState = ep;
			if ( ! m_IsPressedLockMode() )
			{
				_super::SetCapture();
				if ( m_WithPressedMessage() )
				{
					m_SendCommand(BN_PUSHED_T);
				}
			}
			else
			{
				m_SendCommand(BN_PUSHED_T);
			}
			m_CheckNowState(true);
		}
		_super::SetFocus();
	}
	/// リリース処理 
	void m_Unpush(EPush ep)
	{
		if ( ! m_IsAlive() ) { return; }
		if ( m_pushState != ep ) { return; }
		m_pushState = EP_Non;
		::ReleaseCapture();
		if ( ep != EP_Message ) 
		{
			CPoint po = m_curpos;
			ClientToScreen(&po);
			bool onButton = IsOnButton(po);
			if ( ep == EP_Key || onButton )
			{
				m_boIsOnClickDisp = true;
				OnClickButton();
				if ( ! m_IsAlive() ) { return; }
				#ifdef _WIN32_WCE
					m_CheckNowState(false);
					m_RedrawWindow();
					if ( m_IsPressedLockMode() || m_WithPressedMessageMode() )
					{
						m_SendCommand(BN_UNPUSHED_T);
					}
					m_SendCommand(BN_CLICKED);
					return;
				#endif
				if ( m_IsPressedLockMode() || m_WithPressedMessage() )
				{
					m_SendCommand(BN_UNPUSHED_T);
				}
				m_SendCommand(BN_CLICKED);
			}
			if ( ! onButton )
			{
				m_HoverOff(false);
			}
		}
		m_CheckNowState(false);
		m_RedrawWindow();
	}
	/// 次のホバーイベントをもらうための処理
	void m_HoverSub(void)
	{
		#ifndef _WIN32_WCE
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		::_TrackMouseEvent(&tme);
		#endif
	}
};



}; //MFC
}; //TNB



