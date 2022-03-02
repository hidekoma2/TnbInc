#pragma once
/**
 *@file
 * リンクテキスト関係のヘッダ
 *
 *		ハイパーリンクテキストを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcAbstractButton.h"
#include "TnbMfcCursorControl.h"
#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * リンクテキストコントロール
 *
 *		inline なコントロールです。
 *
 *		マウスホバーや訪問済み状態で色を変更できるリンクテキストを作成できます。
 *
 *		リソースエディタでスタティックコントロールを張り、CStatic に割り当て後、
 *		本クラスに置き換えて使用します。
 *
 *	@see CAbstractButton
 *
 *	@par必要ファイル
 *			TnbMfcLinkText.h
 * 
 *	@date 07/10/19 新規作成
 *	@date 07/10/26 テキスト表示部分を抽象化して CTextDrawer 作成。
 *	@date 07/11/06 AdjustSize() 追加
 *	@date 07/11/07 アンダーバーの有無、指定可能に。
 */
class CLinkText : public CCursorControlAddinT<CAbstractButton>, public CDrawTextBase
{
	DEFSUPER(CCursorControlAddinT<CAbstractButton>);
	typedef CDrawTextBase _text;
	TColor			m_hoverColors;			///< ホバー状態色
	TColor			m_visitColors;			///< 一度開いた状態色
	COLORREF		m_focusMarkColor;		///< フォーカスマーク
	bool			m_boIsVisit;			///< true なら訪問済み
	bool			m_boIsUnderlineHoverOnly;///< true ならホバー時のみアンダーライン
	/// 
	CFont* m_SetFont(CDC* pDC, CFont& f, bool withUnderline)
	{
		LOGFONT logFont;
		_super::GetFont()->GetLogFont(&logFont);
		logFont.lfUnderline = withUnderline;	//アンダーライン
		f.CreateFontIndirect(&logFont);
		return pDC->SelectObject(&f);
	}

protected:

	/**
	 * [通知] 描画
	 *	@param pDC 描画先のCDC。
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		CString str;
		GetWindowText(str);
		if ( str.IsEmpty() )
		{
			return;
		}
		bool withUnderline = false;
		TColor gc = m_normalColors;
		TColor* pc = &m_normalColors;
		switch ( _super::GetButtonState() )
		{
		case ES_Pushed:	// 押下状態
			withUnderline = true;
		case ES_Normal:	// 標準状態
		default:
			if ( m_boIsVisit )
			{
				pc = &m_visitColors;
			}
			break;
		case ES_Hover:	// ホバー状態
			pc = &m_hoverColors;
			withUnderline = true;
			break;
		case ES_Disable:// 無効状態
			gc.forward = CLR_INVALID;
			pc = &gc;
			break;
		}
		if ( ! m_boIsUnderlineHoverOnly )
		{ 
			withUnderline = true;
		}
		CFont font;
		CFont* pOldFont = m_SetFont(pDC, font, withUnderline);
		RECT rect;
		_super::GetClientRect(&rect);
		UINT fmt = CTextDrawer::StaticToDrawStyle(_super::GetStyle()) | DT_NOPREFIX;
		pDC->SetBkMode(TRANSPARENT);		// バックカラーは変更なし
		_text::DrawText(pDC, rect, fmt, *pc, str);
		if ( HasFocus() )
		{
			MFCLIB::DrawDottedBox(pDC, rect, m_focusMarkColor);
		}
		pDC->SelectObject(pOldFont);
	}

	/**
	 * [通知] 状態変化通知.
	 *	@note 現在の状態を取得します
	 *	@param state 状態。
	 */
	virtual void OnChangeState(EState state)
	{
		MFCLIB::DrawParent(this);
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
		if ( message == WM_ERASEBKGND ) { return 0; }
		LRESULT r = _super::WindowProc(message, wParam, lParam);
		if ( message == WM_SETFOCUS || message == WM_KILLFOCUS || message == WM_ACTIVATE )
		{
			MFCLIB::DrawParent(this);
		}
		return r;
	}
	
public:

	/// コンストラクタ
	CLinkText(void) : m_boIsVisit(false), m_focusMarkColor(RGB(0, 0, 0)), m_boIsUnderlineHoverOnly(false)
	{
	}

	/// デストラクタ
	virtual ~CLinkText(void)
	{
	}

	/**
	 * [複製] 情報複製.
	 *	@note 保持している情報を複製します。
	 *	@param other コピー元.
	 *	@return 自分の参照
	 */
	CLinkText& operator=(const CLinkText& other)
	{
		_super::operator=(other);
		_text::operator=(other);
		m_hoverColors = other.m_hoverColors;
		m_visitColors = other.m_visitColors;
		m_focusMarkColor = other.m_focusMarkColor;
		m_boIsUnderlineHoverOnly = other.m_boIsUnderlineHoverOnly;
		return *this;
	}

	/**
	 * [設定] 訪問済みフラグ設定.
	 *		訪問済み状態にすることが出来ます。普通状態とは、色が異なるだけです。
	 *	@param r 設定フラグ
	 */
	void SetVisitFlag(bool r = true)
	{
		m_boIsVisit = r;
		RedrawWindow();
	}
	
	/**
	 * [設定] アンダーバー設定.
	 *	@param r true ならマウスが上にある時(ホバー状態、押下状態)の時のみ、アンダーバーがでるようになります。
	 *			false なら、常にアンダーバーが出ます。設定しない場合、常にアンダーバーがでます。
	 */
	void SetUnderlineMode(bool r)
	{
		m_boIsUnderlineHoverOnly = r;
	}

	/**
	 * [設定] ホバー状態テキスト色指定.
	 *	@param color1 前の文字の色
	 *	@param color2 後ろの文字の色。省略可能。
	 */
	void SetTextHoverColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_hoverColors.forward = color1;
		m_hoverColors.back = color2;
	}

	/**
	 * [設定] 訪問済み状態テキスト色指定.
	 *	@param color1 前の文字の色
	 *	@param color2 後ろの文字の色。省略可能。
	 */
	void SetTextVisitColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_visitColors.forward = color1;
		m_visitColors.back = color2;
	}

	/**
	 * [設定] フォーカスマーク色指定.
	 *	@param color 色
	 */
	void SetFocusMarkColor(COLORREF color)
	{
		m_focusMarkColor = color;
	}

	/**
	 * [設定] サイズ調整.
	 *		現在指定されているフォント、文字列、クライアントサイズから、
	 *		適したクライアントサイズに変更します。
	 *	@param margin マージン。必要なクライアントサイズに加えるサイズです。
	 *			プラス指定すると、位置を変えずに、幅、高さのみ変更されます。\n
	 *			マイナス指定すると、位置、幅、高さを変更します。\n
	 *			AdjustSize(10) ；位置を変えずに、文字が入る最小の幅、高さに +10したサイズを設定します。\n
	 *			AdjustSize(-5) ；位置を縦横左上に 5 移動し、文字が入る最小の幅、高さに +10したサイズを設定します。
	 */
	void AdjustSize(int margin = 2)
	{
		CString str;
		GetWindowText(str);
		if ( str.IsEmpty() ) { return; }
		UINT fmt = CTextDrawer::StaticToDrawStyle(_super::GetStyle()) | DT_NOPREFIX;
		CDC* pDC = GetDC();
		CFont font;
		CFont* pOldFont = m_SetFont(pDC, font, true);
		MFCLIB::AdjustClientSize(pDC, fmt, str, margin);
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}

	/**
	 * [設定] ショートカットキー設定.
	 *	@note 本クラスでは、本メソッドは無効です。
	 *	@param key ダミー
	 */
	void SetShortcutKey(TCHAR key)
	{
	}
};



/**@ingroup MFCCONTROL
 * ハイパーリンクテキストコントロール
 *
 *		inline なコントロールです。
 *
 *		マウスホバーや訪問済み状態で色を変更できるリンクテキストを作成できます。
 *
 *		リソースエディタでスタティックコントロールを張り、CStatic に割り当て後、
 *		本クラスに置き換えて使用します。
 *
 *		クリックは親に通知せず、 SetTarget() で設定したURLやファイルをオープンします。
 *
 *	@par必要ファイル
 *			TnbMfcLinkText.h
 * 
 *	@date 07/10/19 新規作成
 *	@date 07/11/08 フォーカスがあるときにリターンが効かないのを改善。
 *	@date 07/11/15 フォーカス移動をメッセージに修正。
 */
class CHyperLinkText : public CLinkText
{
	DEFSUPER(CLinkText);
	CString			m_action;		///< 対象
	CString			m_target;		///< 対象
protected:

	/**
	 * [通知] クリック.
	 *		BN_CLICKED を親に通知する前にコールされます。
	 */
	virtual void OnClickButton(void)
	{
		_super::SpecialCursor(::LoadCursor(NULL, IDC_WAIT));
		SHELLEXECUTEINFO sei;
		::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
		sei.cbSize = sizeof(SHELLEXECUTEINFO);	// Set Size
		sei.lpVerb = m_action;					// Set Verb
		sei.lpFile = m_target;					// Set Target
		sei.nShow = SW_SHOWNORMAL;				// Show Normal
		::ShellExecuteEx(&sei);
		_super::SetVisitFlag();
		_super::ResetCursor();
	}
	
public:

	/// コンストラクタ
	CHyperLinkText(void) : m_action(_T("open"))
	{
		_super::SetTextColor(RGB(0, 0, 255));
		_super::SetTextHoverColor(RGB(255, 128, 0));
		_super::SetTextVisitColor(RGB(200, 0, 0));
	}

	/**
	 * [設定] ターゲット設定.
	 *	@param lpszTarget ターゲット。URL やファイル名を指定します。
	 */
	void SetTarget(LPCTSTR lpszTarget)
	{
		m_target = lpszTarget;
	}

	/**
	 * [設定] アクション設定.
	 *	@note デフォルトでは open が指定されています。
	 *		他に、 edit , explore , find , print , properties などあり。
	 *	@param lpszAction アクション。
	 */
	void SetAction(LPCTSTR lpszAction)
	{
		m_action = lpszAction;
	}
};



}; //MFC
}; //TNB


