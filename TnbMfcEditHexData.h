#pragma once
/**
 *@file
 * HEXデータ入力Edit関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbClipboard.h"
#include "TnbBitmapImage.h"
#include "TnbMfcBitmapDC.h"
#include "TnbStrOperator.h"
#include "TnbDummyCollection.h"
#include "TnbSelectedBeltDrawer.h"
#include "TnbFontHandle.h"
#include "TnbDcSelectAssistant.h"
#include <imm.h>
#ifndef _WIN32_WCE
 #pragma comment(lib,"imm32.lib")
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * HEXデータ入力Editコントロールテンプレート
 *
 *		1byte 毎にスペースが空くので、個々のデータが見やすく編集できます。
 *
 *	@note 編集時、親に通知される WM_COMMAND には、以下のものがあります。
 *		- EN_HSCROLL
 *		- EN_VSCROLL
 *		- EN_SELCHANGE
 *		- EN_CHANGE
 *		- EN_SETFOCUS
 *		- EN_KILLFOCUS
 *
 *	@note WM_PASTE , WM_COPY , WM_CUT にも対応しています。
 *
 *	@note スーパークラスは EDITコントロール( CEdit )ではないので、ご注意下さい。
 *	
 *	@par使い方
 *		リソースエディタでスタティックコントロール（エディットコントロールでも可）を張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic or CEdit）を本クラスに置き換えて使用します。
 *
 *	@param WIDTH 表示幅BYTE数。 1以上を指定するとマルチラインになります。-1 ならシングルライン。
 *
 *	@par必要ファイル
 *			TnbMfcEditHexData.h
 * 
 *	@date 10/04/01 新規作成
 *	@date 10/05/18 EN_CHANGE の発行タイミング修正。カレット表示修正。
 *	@date 10/06/22 テンキーの入力にバグ有り。修正。
 *	@date 11/10/19 Disable 状態のコントロールの表記を修正。
 *	@date 15/05/07 ペースト(ReplaceSel())処理で a～f が正しく入力されないバグを修正。
 */
template<int WIDTH = -1>
class CEditHexDataT : public CWnd
{
	DEFSUPER(CWnd);
public: 

	/// コンストラクタ.
	CEditHexDataT(void) : m_maxLength(-1), m_isInsertMode(false), m_isEnableNotify(false)
		, m_selectCaretIndex(-1), m_isCaletVisidle(false), m_isShowCaretAlways(false)
	{
		ASSERT( WIDTH != 0 );
		m_margin.x = 3;
		m_margin.y = 2;
		m_textColor = ::GetSysColor(COLOR_WINDOWTEXT);
		m_backColor = ::GetSysColor(COLOR_WINDOW);
		m_activeHighlightColor = ::GetSysColor(COLOR_HIGHLIGHT);
		m_inactiveHighlightColor = ::GetSysColor(COLOR_BTNSHADOW);
		m_hasCaret = false;
	}
	
	/**
	 * [設定] マージン設定.
	 *		左上のマージン（文字の表示されるまでの空間）を指定できます。
	 *	@note 未設定の場合、 横 3、縦 2 dot です。
	 *	@param p マージン
	 */
	void SetMargins(const POINT& p)
	{
		m_margin = p;
		m_Redraw();
	}

	/**
	 * [設定] インサートモード設定.
	 *		本コントロールは、インサートモードと上書きモードがあります。
	 *	@note デフォルトでは上書きモードです。 ユーザは[insert]キーでこれらを切り替えることが出来ます。
	 *	@param isEnable true ならインサートモード。false なら上書きモードです。
	 */
	void SetInsertMode(bool isEnable)
	{
		m_isInsertMode = isEnable;
		m_Redraw();
	}

	/**
	 * [取得] インサートモード取得.
	 *		本コントロールは、インサートモードと上書きモードがあります。
	 *	@retval true インサートモード
	 *	@retval false 上書きモード
	 */
	bool IsInsertMode(void) const
	{
		return m_isInsertMode;
	}

	/**
	 * [設定] 最大長設定.
	 *		入力できるBYTE数を指定できます。指定しない場合、無限に入力できます。
	 *	@note マルチラインの場合、最大長を指定すると、サイズ固定になります。
	 *	@note 入力情報はEmptyになります。
	 *	@param maxLength 最大長。省略すると、無限になります。
	 *	@param defaultData マルチラインの場合、デフォルトの値を指定できます。
	 */
	void SetLimitLength(int maxLength = -1, BYTE defaultData = 0)
	{
		m_maxLength = maxLength;
		m_defaultData = defaultData;
		m_SetData(CByteVector());
	}

	/**
	 * [取得] マルチライン確認.
	 *	@retval true マルチラインモード
	 *	@retval false シングルラインモード
	 */
	bool IsMultiLineMode(void) const
	{
		return WIDTH > 0;
	}

	/**
	 * [取得] 入力データ取得.
	 *	@return 入力データ.
	 */
	const CByteVector& GetData(void) const
	{
		return m_data;
	}

	/**
	 * [設定] データ設定.
	 *	@param vb データ.
	 */
	void SetData(const CByteVector& vb)
	{
		m_SetData(vb);
	}

	/**
	 * [取得] 可視行数取得.
	 *		クライアント領域に表示できる行数を返します。
	 *	@return 行数
	 */
	int GetVisibleLineCount(void) const
	{
		if ( IsMultiLineMode() )
		{
			CRect rc;
			_super::GetClientRect(rc);
			return (rc.Height() + m_charSize.cy - 1) / m_charSize.cy;
		}
		return 1;
	}

	/**
	 * [取得] 行数取得.
	 *		全データの行数を返します。
	 *	@return 行数
	 */
	int GetLineCount(void) const
	{
		if ( IsMultiLineMode() )
		{
			return (m_data.GetSize() + WIDTH - 1) / WIDTH;
		}
		return 1;
	}

	/**
	 * [取得] 可視最上行取得.
	 *		表示している最上の行数を返します。
	 *	@return 行数。一番上が０。
	 */
	int GetFirstVisibleLine(void) const
	{
		if ( IsMultiLineMode() )
		{
			return m_viewPos.y / m_charSize.cy;
		}
		return 0;
	}

	/**
	 * [設定] 可視最上行設定.
	 *		一番上に表示する行を指定します。
	 *	@param line 行数。
	 */
	void SetFirstVisibleLine(int line)
	{
		if ( IsMultiLineMode() )
		{
			int y = line;
			int l = GetLineCount() - GetVisibleLineCount() + 1;
			if ( y < 0 )
			{
				y = 0;
			}
			else if ( y > l )
			{
				y = l;
			}
			y *= m_charSize.cy;
			if ( y != m_viewPos.y )
			{
				m_viewPos.y = y;
				m_Redraw();
				NotifyToParent(EN_VSCROLL);
			}
		}
	}

	/**
	 * [設定] 選択位置設定.
	 *		指定のインデックスにカレット位置を移動します。
	 *	@param index インデックス
	 */
	void SetSel(int index)
	{
		m_ReleaseSelect();
		m_SetCaretIndex(index);
		m_Redraw();
	}

	/**
	 * [設定] 選択位置設定.
	 *		指定のインデックス間を選択状態にします。
	 *	@param startIndex 選択開始インデックス
	 *	@param endIndex 選択終了インデックス
	 */
	void SetSel(int startIndex, int endIndex)
	{
		if ( startIndex == endIndex )
		{
			m_ReleaseSelect();
			m_SetCaretIndex(startIndex);
			m_Redraw();
			return;
		}
		if ( endIndex < 0 )
		{
			endIndex = m_GetIndexMax();
		}
		m_ReleaseSelect();
		m_SetCaretIndex(startIndex, false, true);
		ASSERT( m_caretDrawPos.x >= 0 );
		m_SetSelect(m_caretIndex, m_caretDrawPos);
		m_SetCaretIndex(endIndex);
		m_Redraw();
	}

	/**
	 * [取得] 選択位置取得.
	 *		カレットの位置や、選択範囲を取得します。
	 *	@note _startIndex と _endIndex が同じ場合、未選択と判断できます。
	 *	@param[out] _startIndex 選択状態の場合、選択開始位置が返ります。通常の場合、カレットの位置が入ります。
	 *	@param[out] _endIndex 選択状態の場合、選択終了位置が返ります。通常の場合、カレットの位置が入ります。
	 */
	void GetSel(int& _startIndex, int& _endIndex) const
	{
		if ( m_selectCaretIndex < 0 )
		{
			// 未選択状態
			_startIndex = m_caretIndex;
			_endIndex = m_caretIndex;
		}
		else
		{
			// 選択状態
			_startIndex = m_selectCaretIndex;
			_endIndex = m_caretIndex;
		}
	}

	/**
	 * [置換] データ置換.
	 *		選択されている部分を指定のデータに置換します。
	 *		選択されていない場合、インサートモードの場合カレットの位置から挿入します。上書きモードの場合、上書きします。
	 *	@note 文字列は、 0～9, A～F, a～f 以外は無視します。
	 *	@param lpszText 文字列
	 */
	void ReplaceSel(LPCTSTR lpszText)
	{
		CString s = lpszText;
		if ( ! s.IsEmpty() )
		{
			bool nowInsertMode = m_isInsertMode;
			if ( m_selectCaretIndex >= 0 )
			{
				m_isInsertMode = true;
			}
			m_isEnableNotify = false;
			_super::SetRedraw(false);
			s.MakeUpper(); // 大文字化
			loop ( i, s.GetLength() )
			{
				m_Input(s.GetAt(i), false);
			}
			_super::SetRedraw(true);
			m_isEnableNotify = true;
			m_isInsertMode = nowInsertMode;
			NotifyToParent(EN_SELCHANGE);
			NotifyToParent(EN_CHANGE);
		}
		else
		{
			m_DeleteSelectedData();
			m_ReleaseSelect();
		}
		m_Redraw();
	}

	/**
	 * [置換] データ置換.
	 *		選択されている部分を指定のデータに置換します。
	 *		選択されていない場合、インサートモードの場合カレットの位置から挿入します。上書きモードの場合、上書きします。
	 *	@param vb データ.
	 */
	void ReplaceSel(const CByteVector& vb)
	{
		ReplaceSel(CStrOperator::BinaryToHexString(vb));
	}

	/**
	 * [設定] ツールチップ追加.
	 *		入力済みのデータ量をツールチップで表示するようにします。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddToolTip(void)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			m_toolTip.Create(this, TTS_ALWAYSTIP);
			m_toolTip.AddTool(this, _T(""));
			m_toolTip.Activate(TRUE);
			return true;
		}
		return false;
	}

	/**
	 * [設定] 常にカレット表示.
	 *	@note フォーカスが無い時も、カレットの位置がわかるようにします。
	 */
	void ShowCaretAlways(void)
	{
		m_isShowCaretAlways = true;
	}

	/**
	 * [設定] 文字色設定.
	 *	@param color 色
	 *	@return 設定前の文字色
	 */
	COLORREF SetTextColor(COLORREF color)
	{
		COLORREF r = m_textColor;
		m_textColor = color;
		m_Redraw();
		return r;
	}

	/**
	 * [設定] 背景色設定.
	 *	@param color 色
	 *	@return 設定前の背景色
	 */
	COLORREF SetBkColor(COLORREF color)
	{
		COLORREF r = m_backColor;
		m_backColor = color;
		m_Redraw();
		return r;
	}

	/**
	 * [設定] ハイライト色設定.
	 *	@param activeColor アクティブ時の背景色
	 *	@param inactiveColor 非アクティブ時の背景色
	 */
	void SetHighlightColor(COLORREF activeColor, COLORREF inactiveColor)
	{
		m_activeHighlightColor = activeColor;
		m_inactiveHighlightColor = inactiveColor;
	}

	/**
	 * [作成] コンテキストメニュー作成.
	 *	@param[out] _menu 作成したメニューが格納されます。
	 *	@param[in] canCopy コピー、カットが可能なら true を指定します。
	 *	@param[in] topMenuId メニューIDを指定します。指定の IDが切り取り、+1がコピー、+2がペーストに割り当てられます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool MakeContextMenu(CMenu& _menu, bool canCopy, int topMenuId)
	{
		if ( _menu.GetSafeHmenu() != NULL )
		{
			_menu.DestroyMenu();
		}
		BOOL r = TRUE;
		r &= _menu.CreatePopupMenu();
		CClipboard clipb;
		bool canPaste = clipb.IsAvailable();
		r &= _menu.AppendMenu(MF_STRING | (canCopy ? 0 : MF_GRAYED), topMenuId + 0, _T("切り取り(&T)"));
		r &= _menu.AppendMenu(MF_STRING | (canCopy ? 0 : MF_GRAYED), topMenuId + 1, _T("コピー(&C)"));
		r &= _menu.AppendMenu(MF_STRING | (canPaste ? 0 : MF_GRAYED), topMenuId + 2, _T("貼り付け(&P)"));
		return !! r;
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@note IMEを無効化しています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_hasCaret = false;
		::ImmAssociateContext(_super::GetSafeHwnd(), NULL);	// IME入力禁止
		::SetClassLong(_super::GetSafeHwnd(), GCL_HCURSOR, reinterpret_cast<LONG>(::LoadCursor(NULL, IDC_IBEAM)));
		m_font.Set(_super::GetSafeHwnd());
		m_SetData(CByteVector());
		_super::PreSubclassWindow();
		m_isEnableNotify = true;
		m_CheckCharSize();
	}
		
	/**
	 * [通知] for translating Windows messages in main message pump
	 *	@param pMsg メッセージ
	 *	@retval TRUE ディスパッチできない場合
	 *	@retval FALSE ディスパッチする必要がある場合
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if ( ::IsWindow(m_toolTip) )
		{
			m_toolTip.RelayEvent(pMsg);
		}
		return _super::PreTranslateMessage(pMsg);
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
		case WM_DESTROY:
			m_toolTip.DestroyWindow();
			m_isEnableNotify = false;
			break;
		case WM_SETFONT:
			{
				HFONT hFont = reinterpret_cast<HFONT>(wParam);
				if ( hFont != NULL )
				{
					m_font.SetClone(hFont);
					m_CheckCharSize();
					if ( LOWORD(lParam) != 0 )
					{
						m_Redraw();
					}
				}
			}
			break;
		case WM_PAINT:
			m_DrawClient(CPaintDC(this));
			return 0;
			break;
		case WM_SETTEXT:
			m_OnSetText(reinterpret_cast<LPCTSTR>(lParam));
			return 1;
			break;
		case WM_GETTEXTLENGTH:
			if ( m_dataString.GetLength() > 0 )
			{
				return (m_dataString.GetLength() - 1) * sizeof(TCHAR);
			}
			return 0;
			break;
		case WM_GETTEXT:
			if ( wParam > 0 && m_dataString.GetLength() > 0 )
			{
				LPTSTR pDst = reinterpret_cast<TCHAR*>(lParam);
				LPCTSTR pSrc = m_dataString;
				size_t n = m_dataString.GetLength() - 1; //最後はSPだから
				size_t m = min(wParam, n);
				loop ( i, m )
				{
					*pDst++ = *pSrc++;
				}
				*pDst = 0;
				return m * sizeof(TCHAR);
			}
			return 0;
			break;
		case WM_SETFOCUS:
			m_Redraw();
			NotifyToParent(EN_SETFOCUS);
			return 0;
			break;
		case WM_KILLFOCUS:
			m_HideCaret();
			m_Redraw();
			NotifyToParent(EN_KILLFOCUS);
			return 0;
			break;
		case WM_KEYDOWN:
			return m_OnChar(wParam, lParam);
		case WM_KEYUP:
		case WM_CHAR:
			return 0;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			_super::SetFocus();
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			return m_OnMouse(message, wParam, lParam);
			break;
		case WM_MOUSEWHEEL:
			{
				short d = HIWORD(wParam);
				SetFirstVisibleLine(GetFirstVisibleLine() - (d / 120));
			}
			break;
		case WM_ERASEBKGND:
			if ( IsMultiLineMode() )
			{
				return 0;
			}
			if ( IsWindowEnabled() )
			{
				HDC hdc = reinterpret_cast<HDC>(wParam);
				CRect rect;
				GetClientRect(rect);
				CBrush b(m_backColor);
				::FillRect(hdc, &rect, b);
				return 0;
			}
			break;
		case WM_ENABLE:
			{
				CWnd* pParent = GetParent();
				if ( pParent != NULL )
				{
					CRect rect;
					GetClientRect(rect);
					ClientToScreen(rect);
					pParent->ScreenToClient(rect);
					pParent->InvalidateRect(rect, TRUE);
				}
				Invalidate(TRUE);
			}
			return 0;
			break;
		case WM_GETDLGCODE:
			return DLGC_WANTCHARS | DLGC_WANTARROWS;
			break;
		case WM_COPY:
		case WM_CUT:
			if ( m_selectCaretIndex >= 0 && m_caretIndex != m_selectCaretIndex )
			{
				CClipboard clipb;
				int x1 = m_GetCaretIndex() / 2;
				int x2 = m_selectCaretIndex / 2;
				CStr s = CStrOperator::BinaryToHexString(m_data.Mid(min(x1, x2), ::abs(x1 - x2)));
				clipb.SetString(s);
				if ( message == WM_CUT )
				{
					m_DeleteSelectedData();
					m_ReleaseSelect();
					m_Redraw();
				}
				return 1;
			}
			return 0;
			break;
		case WM_PASTE:
			ReplaceSel(CClipboard().GetString());
			return 1;
			break;
		case WM_CONTEXTMENU:
			{
				CMenu menu;
				bool canCopy = (m_selectCaretIndex >= 0);
				if ( MakeContextMenu(menu, canCopy, 100) )
				{
					CPoint po(lParam);
					if ( po.x == -1 && po.y == -1 )
					{
						po = _super::GetCaretPos();
						if ( po.x < 0 )
						{
							::GetCursorPos(&po);
						}
						else
						{
							_super::ClientToScreen(&po);
							po.y += m_charSize.cy;
						}
					} 
					TrackPopupMenuEx(menu, 0, po.x, po.y, _super::GetSafeHwnd(), NULL);
				}
				return 0;
			}
			break;
		case WM_COMMAND:
			{
				switch ( LOWORD(wParam) )
				{
				case 100:
					SendMessage(WM_CUT);
					break;
				case 101:
					SendMessage(WM_COPY);
					break;
				case 102:
					SendMessage(WM_PASTE);
					break;
				}
			}
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] 親ウィンドウへ通知.
	 *		EN_UPDATE など、親ウィンドウに WM_COMMAND を通知します。
	 *	@param notifyCode コード
	 */
	virtual void NotifyToParent(WORD notifyCode)
	{
		if ( m_isEnableNotify )
		{
			MFCLIB::SendCommandMessage(this, notifyCode);
		}
	}

	/**
	 * [取得] 文字色取得.
	 *	@return コントロールの無効状態を考慮した文字色
	 */
	COLORREF GetTextColor(void) const
	{
		return IsWindowEnabled() ? m_textColor : ::GetSysColor(COLOR_GRAYTEXT);
	}

	/**
	 * [取得] 背景色取得.
	 *	@return コントロールの無効状態を考慮した背景色
	 */
	COLORREF GetBkColor(void) const
	{
		return IsWindowEnabled() ? m_backColor : ::GetSysColor(COLOR_MENU);
	}

private:
	// 再表示
	void m_Redraw(void)
	{
		_super::Invalidate();
	}

	// 文字の大きさチェック
	void m_CheckCharSize(void)
	{
		CClientDC dc(this);
		HGDIOBJ hOld = ::SelectObject(dc, m_font);
		m_charSize = dc.GetTextExtent(_T("8"));
		::SelectObject(dc, hOld);
	}

	/**
	 * [取得] INDEX最大値.
	 *		現在指定可能なINDEXの最大値を返します。
	 *	@return 最大値。
	 */
	int m_GetIndexMax(void) const
	{
		return m_data.GetSize() * 2;
	}

	/**
	 * [通知] WM_SETTEXT 通知　
	 *		文字列をHEX文字列として解析し、本コントロールのデータとします。
	 *	@param lpsz 通知された文字列
	 */
	void m_OnSetText(LPCTSTR lpsz)
	{
		m_SetData(CStrOperator::HexStringToBinary(lpsz));
	}

	/**
	 * [設定] データ設定.
	 *	@note カレット位置を初期化し、選択状態を解除します。
	 *	@param vb データ.
	 */
	void m_SetData(const CByteVector& vb)
	{
		m_data = vb;
		if ( m_maxLength > 0 && ToInt(m_data.GetSize()) > m_maxLength )
		{
			m_data.SetSize(m_maxLength);
		}
		m_MakeViewData();
		m_viewPos.x = 0;
		m_viewPos.y = 0;
		m_SetCaretIndex(0);
		m_ReleaseSelect();
		m_Redraw();
		NotifyToParent(EN_SELCHANGE);
		NotifyToParent(EN_CHANGE);
		NotifyToParent(EN_VSCROLL);
		NotifyToParent(EN_HSCROLL);
	}
	
	/**
	 * [指定] カレットの位置指定
	 *	@param p カレット位置.
	 *	@param isNotify true なら親に通知.
	 *	@param isCalcDrawPos true なら、すぐに描画位置の計算を行う
	 */
	void m_SetCaretIndex(int p, bool isNotify = true, bool isCalcDrawPos = false)
	{
		if ( p < 0 )
		{
			p = 0;
		}
		int im = m_GetIndexMax();
		if ( p > im )
		{
			p = im;
		}
		bool r = (m_caretIndex != p);
		if ( r )
		{
			m_caretIndex = p;
			m_caretDrawPos.x = -1;
		}
		if ( isCalcDrawPos && m_caretDrawPos.x < 0 )
		{
			CClientDC dc(this);
			HGDIOBJ hOld = ::SelectObject(dc, m_font);
			m_caretDrawPos = m_IndexToClientPoint(dc, m_caretIndex);
			::SelectObject(dc, hOld);
		}
		if ( r )
		{
			m_isCaletVisidle = true;
			if ( isNotify )
			{
				NotifyToParent(EN_SELCHANGE);
			}
		}
	}

	// カレットのインデックス取得
	int m_GetCaretIndex(void) const
	{
		return m_caretIndex;
	}

	/**
	 * [通知] マウスメッセージ通知
	 *	@param message WM_LBUTTONUP, WM_LBUTTONDOWN, WM_MOUSEMOVE のいずれか
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return 0
	 */
	LRESULT m_OnMouse(UINT message, WPARAM wParam, LPARAM lParam)
	{
		bool isCaptrued = (::GetCapture() == _super::GetSafeHwnd());
		if ( message == WM_LBUTTONUP )
		{
			// UP
			if ( isCaptrued )
			{
				::ReleaseCapture();
				if ( m_selectCaretIndex == m_GetCaretIndex() )
				{
					m_ReleaseSelect();
					m_Redraw();
				}
			}
			return 0;
		}
		if ( message == WM_MOUSEMOVE && ! isCaptrued )
		{
			return 0;
		}
		CPoint po(lParam);
		CClientDC dc(this);
		HGDIOBJ hOld = ::SelectObject(dc, m_font);
		int l = m_ClientPointToIndex(dc, po);
		::SelectObject(dc, hOld);
		if ( message == WM_LBUTTONDOWN )
		{
			bool isShift = (::GetAsyncKeyState(VK_SHIFT) < 0);
			if ( m_selectCaretIndex < 0 )
			{
				if ( isShift )
				{
					m_SetSelect(m_caretIndex, m_caretDrawPos);
					::SetCapture(_super::GetSafeHwnd());
				}
			}
			else
			{
				if ( ! isShift )
				{
					m_ReleaseSelect();
				}
			}
		}
		m_SetCaretIndex(l, true, true);
		if ( ! isCaptrued )
		{
			CPoint spo = po - m_margin;
			_super::ClientToScreen(&spo);
			if ( ::DragDetect(m_hWnd, spo) )
			{
				// プレス！
				m_SetSelect(m_caretIndex, m_caretDrawPos);
				::SetCapture(_super::GetSafeHwnd());
			}
		}
		_super::SetFocus();
		m_Redraw();
		return 0;
	}
	
	/**
	 * [通知] WM_CHAR 通知　
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return 0
	 */
	LRESULT m_OnChar(WPARAM wParam, LPARAM lParam)
	{
		if ( (lParam & _BIT(31)) != 0 )
		{
			//離された時はなにもすることがない。
			return 0;
		}
		bool isShift = (::GetAsyncKeyState(VK_SHIFT) < 0);
		bool isCtrl = (::GetAsyncKeyState(VK_CONTROL) < 0);
		//
		if ( isShift && wParam == VK_INSERT )
		{
			isShift = false;
			isCtrl = true;
			wParam = 'V';
		}
		if ( isCtrl )
		{
			switch ( wParam )
			{
			case 'A':
				SetSel(0, -1);
				break;
			case 'C':
				SendMessage(WM_COPY);
				break;
			case 'X':
				SendMessage(WM_CUT);
				break;
			case 'V':
				SendMessage(WM_PASTE);
				break;
			default:
				break;
			}
			return 0;
		}
		m_Input(wParam, isShift);
		return 0;
	}

	/**
	 * [追加] 入力　
	 *	@param ch キャラコード
	 *	@param isShift シフト状態
	 */
	void m_Input(WPARAM ch, bool isShift)
	{
		bool isWork = true;
		bool isSendChange = false;
		int nowPosition = m_GetCaretIndex();
		POINT nowDrawPos = m_caretDrawPos;
		//
		if ( ! IsMultiLineMode() )
		{
			// SingleLine
			switch( ch )
			{
			case VK_UP:
			case VK_PRIOR:
				ch = VK_HOME;
				break;
			case VK_DOWN:
			case VK_NEXT:
				ch = VK_END;
				break;
			}
		}
		//
		switch ( ch )
		{
		case VK_INSERT:
			m_isInsertMode = ! m_isInsertMode;
			NotifyToParent(EN_SELCHANGE);
			isWork = false;
			if ( m_hasCaret )
			{
				POINT p = _super::GetCaretPos();
				if ( m_isInsertMode )
				{
					_super::CreateSolidCaret(2, m_charSize.cy);
				}
				else
				{
					_super::CreateSolidCaret(m_charSize.cx, m_charSize.cy);
				}
				_super::ShowCaret();
				_super::SetCaretPos(p);
			}
			break;
		case VK_DELETE:
			if ( ! m_DeleteSelectedData() )
			{
				int x = m_GetCaretIndex() / 2;
				m_SetCaretIndex(x * 2);
				m_data.Remove(x);
				m_MakeViewData();
				isSendChange = true;
			}
			else
			{
				m_ReleaseSelect();
			}
			isShift = false;
			break;
		case VK_BACK:
			if ( ! m_DeleteSelectedData() )
			{
				if ( m_GetCaretIndex() == 0 )
				{
					break;
				}
				int x = (m_GetCaretIndex() - 1) / 2;
				m_SetCaretIndex(x * 2);
				m_data.Remove(x);
				m_MakeViewData();
				isSendChange = true;
			}
			else
			{
				m_ReleaseSelect();
			}
			isShift = false;
			break;
		case VK_HOME:
			m_SetCaretIndex(0);
			break;
		case VK_END:
			m_SetCaretIndex(m_GetIndexMax());
			break;
		case VK_PRIOR:
		case VK_NEXT:
			{
				int s = ((nowPosition & 1) == 0) ? 0 : 1;
				int w = WIDTH * 2 * (GetVisibleLineCount() - 1);
				if  ( ch == VK_PRIOR )
				{
					w = -w;
				}
				int n = nowPosition + w + s;
				m_SetCaretIndex(n);
			}
			break;
		case VK_UP:
			{
				int s = ((nowPosition & 1) == 0) ? 0 : 1;
				int n = nowPosition - WIDTH * 2 - s;
				if ( n >= 0 )
				{
					m_SetCaretIndex(n);
				}
			}
			break;
		case VK_DOWN:
			{
				int s = ((nowPosition & 1) == 0) ? 0 : 1;
				int n = nowPosition + WIDTH * 2 + s;
				if ( n < m_GetIndexMax() )
				{
					m_SetCaretIndex(n);
				}
			}
			break;
		case VK_RIGHT:
			if ( ! isShift && m_selectCaretIndex >= 0 )
			{
				// 選択状態
				if ( m_selectCaretIndex < nowPosition )
				{
					break;
				}
			}
			if ( m_GetIndexMax() > nowPosition )
			{
				int s = ((nowPosition & 1) == 0) ? 2 : 1;
				m_SetCaretIndex(nowPosition + s);
			}
			break;
		case VK_LEFT:
			if ( ! isShift && m_selectCaretIndex >= 0 )
			{
				// 選択状態
				if ( m_selectCaretIndex > nowPosition )
				{
					break;
				}
			}
			if ( nowPosition > 0 )
			{
				int s = ((nowPosition & 1) == 0) ? 2 : 1;
				m_SetCaretIndex(nowPosition - s);
			}
			break;
		default:
			if ( isShift )
			{
				isWork = false;
			}
			else
			{
				int c = -1;
				if ( ch >= VK_NUMPAD0 && ch <= VK_NUMPAD9 )
				{
					c = ch - VK_NUMPAD0;
				}
				else
				{
					c = STRLIB::HexCharToInt(ch);
				}
				if ( c < 0 )
				{
					isWork = false;
				}
				else
				{
					bool isDeleted = m_DeleteSelectedData();
					bool isLead = ((m_GetCaretIndex() & 1) == 0);
					INDEX x = m_GetCaretIndex() / 2;
					if ( m_data.GetSize() <= x )
					{
						//末にある
						if ( m_maxLength > 0 && m_maxLength <= ToInt(m_data.GetSize()) )
						{
							break;
						}
						else
						{
							x = m_data.GetSize();
							m_data.SetSize(x + 1);
							m_data[x] = static_cast<BYTE>(c << 4);
							m_MakeViewData();
							isSendChange = true;
						}
					}
					else if ( isLead )
					{
						if ( m_isInsertMode || isDeleted )
						{
							if ( IsMultiLineMode() && m_maxLength > 0 )
							{
								// マルチライン＆リミット
								m_data.Insert(x, static_cast<BYTE>(c << 4));
								m_MakeViewData();
								isSendChange = true;
							}
							else 
							{
								if ( m_maxLength > 0 && m_maxLength <= ToInt(m_data.GetSize()) )
								{
									break;
								}
								else
								{
									m_data.Insert(x, static_cast<BYTE>(c << 4));
									m_MakeViewData();
									isSendChange = true;
								}
							}
						}
						else
						{
							m_data[x] &= 0x0F;
							m_data[x] |= static_cast<BYTE>(c << 4);
							m_MakeViewData();
							isSendChange = true;
						}
					}
					else
					{
						m_data[x] &= 0xF0;
						m_data[x] |= static_cast<BYTE>(c);
						m_MakeViewData();
						isSendChange = true;
					}
					m_SetCaretIndex(m_GetCaretIndex() + 1);
				}
			}
			break;
		}
		if ( isWork )
		{
			if ( isShift && m_selectCaretIndex < 0 )
			{
				m_SetSelect(nowPosition, nowDrawPos);
			}
			if ( ! isShift && m_selectCaretIndex >= 0 )
			{
				m_ReleaseSelect();
			}
			m_Redraw();
		}
		if ( isSendChange )
		{
			NotifyToParent(EN_CHANGE);
		}
	}

	/**
	 * 指定のクライアント座標の INDEX を返す。
	 */
	int m_ClientPointToIndex(CDC& dc, const POINT& point)
	{
		CPoint po = point;
		po = po - m_margin + m_viewPos;
		if ( po.x < 0 )
		{
			po.x = 0;
		}
		if ( po.y < 0 )
		{
			po.y = 0;
		}
		int w = (po.x) / m_charSize.cx;
		int h = (po.y) / m_charSize.cy;
		if ( IsMultiLineMode() )
		{
			//== Multi Line
			w = w / 3 * 2;
			if ( w > WIDTH * 2 )
			{
				w = WIDTH * 2;
			}
			return w + h * (WIDTH * 2);
		}
		//== Single Line
		while ( m_dataString.GetLength() > w )
		{
			if ( po.x < dc.GetTextExtent(m_dataString.Left(w + 1)).cx )
			{
				break;
			}
			w++;
		}
		w = w / 3 * 2;
		if ( w > m_GetIndexMax() )
		{
			w = m_GetIndexMax();
		}
		return w;
	}

	/// インデックスをクライアント座標に変換
	CPoint m_IndexToClientPoint(CDC& dc, int index)
	{
		CPoint pos;
		if ( IsMultiLineMode() )
		{
			//== MultiLine
			int x = index % (WIDTH * 2);
			pos.x = (x + (x / 2)) * m_charSize.cx;
			pos.y = (index / (WIDTH * 2)) * m_charSize.cy;
			if ( m_maxLength > 0 )
			{
				//リミット有り
				if ( index == m_GetIndexMax() )
				{
					pos.x = (WIDTH * 3) * m_charSize.cx;
					pos.y -= m_charSize.cy;
				}
			}
		}
		else
		{
			//== SingleLine
			int l = index + (index / 2);
			CPoint p = dc.GetTextExtent(m_dataString.Left(l));
			pos.x = p.x;
			pos.y = 0;
		}
		return pos;
	}

	/**
	 * [削除] 範囲内削除.
	 *		選択されている範囲を削除し、カレットの位置を調整します。
	 *	@retval true 削除処理を行った。
	 *	@retval false なにもしていない。
	 */
	bool m_DeleteSelectedData(void)
	{
		if ( m_selectCaretIndex >= 0 && m_caretIndex != m_selectCaretIndex )
		{
			int x1 = m_GetCaretIndex() / 2;
			int x2 = m_selectCaretIndex / 2;
			m_data.RemoveElements(min(x1, x2), ::abs(x1 - x2));
			m_MakeViewData();
			if ( m_GetCaretIndex() > m_selectCaretIndex )
			{
				m_SetCaretIndex(m_selectCaretIndex);
			}
			return true;
		}
		return false;
	}

	/**
	 * [設定] 表示用データ更新.
	 *		表示用文字列とツールチップを更新します。
	 */
	void m_MakeViewData(void)
	{
		if ( IsMultiLineMode() && m_maxLength > 0 )
		{
			// マルチライン＆リミット
			if ( ToInt(m_data.GetSize()) > m_maxLength )
			{
				m_data.SetSize(m_maxLength);
			}
			CDummyCollectionT<BYTE> dc(m_defaultData);
			dc.SetSize(m_maxLength - m_data.GetSize());
			m_data.Append(dc);
			ASSERT( ToInt(m_data.GetSize()) == m_maxLength );
		}
		//
		TCHAR* P = m_dataString.GetBuffer(m_data.GetSize() * 3 + 1);
		loop ( i, m_data )
		{
			BYTE b = m_data[i];
			*P++ = STRLIB::IntToHexChar(b >> 4);
			*P++ = STRLIB::IntToHexChar(b & 0x0F);
			*P++ = ' ';
		}
		*P = '\0';
		m_dataString.ReleaseBuffer();
		if ( ::IsWindow(m_toolTip) )
		{
			m_toolTip.UpdateTipText(CStr::Fmt(_T("has %d byte(s)."), m_data.GetSize()), this);
		}
//		NotifyToParent(EN_CHANGE);
	}

	/**
	 * [描画] クライアント描画.
	 *	@param dc デバイスコンテキスト
	 */
	void m_DrawClient(CDC& dc)
	{
		CPoint nowPos = m_viewPos;
		CRect rc;
		GetClientRect(rc);
		HGDIOBJ hOld = ::SelectObject(dc, m_font);
		dc.IntersectClipRect(rc);
		if ( m_caretDrawPos.x < 0 )
		{
			m_caretDrawPos = m_IndexToClientPoint(dc, m_caretIndex);
		}
		dc.SetBkMode(TRANSPARENT);
		if ( IsMultiLineMode() )
		{
			m_DrawMultiLineMode(dc, rc);
		}
		else
		{
			m_DrawSingleLineMode(dc, rc);
		}
		::SelectObject(dc, hOld);
		//
		if ( m_HasFocus() )
		{
			CPoint p = m_caretDrawPos - m_viewPos;
			if ( rc.PtInRect(p) && rc.PtInRect(p + m_charSize) )
			{
				p.x--;
			}
			else
			{
				p.x = 60000;
			}
			_super::SetCaretPos(p + m_margin);
			if ( ! m_hasCaret )
			{
				m_hasCaret = true;
				if ( m_isInsertMode )
				{
					_super::CreateSolidCaret(2, m_charSize.cy);
				}
				else
				{
					_super::CreateSolidCaret(m_charSize.cx, m_charSize.cy);
				}
				_super::ShowCaret();
				_super::SetCaretPos(p + m_margin);
			}
		}
		if ( nowPos.x != m_viewPos.x )
		{
			NotifyToParent(EN_HSCROLL);
		}
		if ( nowPos.y != m_viewPos.y )
		{
			NotifyToParent(EN_VSCROLL);
		}
	}

	/**
	 * [描画] マルチラインクライアント描画.
	 *	@param targetDC デバイスコンテキスト
	 *	@param clientRect クライアント範囲
	 */
	void m_DrawMultiLineMode(CDC& targetDC, const CRect& clientRect)
	{
		CRect rc = clientRect;
		rc.left += m_margin.x;
		rc.top += m_margin.y;
		//== 表示位置補正
		if ( m_isCaletVisidle )
		{
			m_isCaletVisidle = false;
			CPoint p = m_IndexToClientPoint(targetDC, m_caretIndex);
			if ( rc.Width() - m_charSize.cx < p.x - m_viewPos.x )
			{
				m_viewPos.x = p.x - (rc.Width() - m_charSize.cx);
			}
			if ( p.x < m_viewPos.x )
			{
				m_viewPos.x = p.x;
			}
			int hh = rc.Height() / m_charSize.cy * m_charSize.cy;
			if ( hh - m_charSize.cy < p.y - m_viewPos.y )
			{
				m_viewPos.y = p.y - (hh - m_charSize.cy);
			}
			if ( p.y < m_viewPos.y )
			{
				m_viewPos.y = p.y;
			}
		}
		CBitmapImage bi;
		bi.Set(clientRect.Width(), clientRect.Height(), GetBkColor());
		//== 裏画面で画像作成
		{
			CBitmapDC dc(&bi);
			CDcSelectAssistant dca(dc);
			dc.SetWindowOrg(m_viewPos);
			dca.SelectFont(m_font);
			dca.SetTextColor(GetTextColor());
			dca.SetBkColor(GetBkColor());
			//== 文字表示
			rc.right += m_viewPos.x;
			rc.bottom = rc.top + m_charSize.cy;
			int d = WIDTH * 3;
			loop ( i, m_dataString.GetLength() / d + 1 )
			{
				if ( dc.RectVisible(rc) )
				{
					CString ss = m_dataString.Mid(i * d, d);
					dc.DrawText(ss, rc, DT_LEFT);
				}
				rc.top += m_charSize.cy;
				rc.bottom += m_charSize.cy;
			}
			//== 選択表示
			bool hasFocus = m_HasFocus();
			if ( m_selectCaretIndex >= 0 && m_selectCaretIndex != m_caretIndex )
			{
				POINT p1 = m_selectCaretDrawPos;
				POINT p2 = m_caretDrawPos;
				if ( p1.y == p2.y )
				{
					if ( p1.x > p2.x )
					{
						Swap(p1.x, p2.x);
					}
					CRect rc(p1.x, p1.y, p2.x - m_charSize.cx, p1.y + m_charSize.cy);
					m_DrawSelectMark(dc, rc + m_margin, hasFocus);
				}
				else
				{
					if ( p1.y > p2.y )
					{
						Swap(p1, p2);
					}
					loop ( i, (p2.y - p1.y) / m_charSize.cy - 1 )
					{
						CPoint ps(0, p1.y + (i + 1) * m_charSize.cy);
						CSize sz(m_charSize.cx * (WIDTH * 3 - 1), m_charSize.cy);
						m_DrawSelectMark(dc, CRect(ps, sz) + m_margin, hasFocus);
					}
					CRect rc1(p1.x, p1.y, m_charSize.cx * (WIDTH * 3 - 1), p1.y + m_charSize.cy);
					m_DrawSelectMark(dc, rc1 + m_margin, hasFocus);
					CRect rc2(0, p2.y, p2.x - m_charSize.cx, p2.y + m_charSize.cy);
					m_DrawSelectMark(dc, rc2 + m_margin, hasFocus);
				}
			}
			else if ( m_isShowCaretAlways && ! hasFocus )
			{
				CRect rc(m_caretDrawPos, m_charSize);
				if ( m_isInsertMode )
				{
					rc.right = rc.left + 1;
				}
				rc.left--;
				rc += m_margin;
				m_DrawSelectMark(dc, rc, false);
			}
		}
		bi.Draw(targetDC, 0, 0);
	}

	/**
	 * [描画] シングルラインクライアント描画.
	 *	@param dca デバイスコンテキスト
	 *	@param clientRect クライアント範囲
	 */
	void m_DrawSingleLineMode(CDC& dc, const CRect& clientRect)
	{
		CRect rc = clientRect;
		rc.left += m_margin.x;
		rc.top += m_margin.y;
		//== 表示位置補正
		if ( m_isCaletVisidle )
		{
			m_isCaletVisidle = false;
			CPoint p = m_IndexToClientPoint(dc, m_caretIndex);
			if ( rc.Width() - m_charSize.cx < p.x - m_viewPos.x )
			{
				m_viewPos.x = (p.x - (rc.Width() - m_charSize.cx));
			}
			if ( p.x < m_viewPos.x )
			{
				m_viewPos.x = p.x;
			}
			m_viewPos.y = 0;
		}
		dc.SetWindowOrg(m_viewPos);
		//== 文字表示
		rc.right += m_viewPos.x;
		dc.SetTextColor(GetTextColor());
		dc.SetBkColor(GetBkColor());
		dc.DrawText(m_dataString, rc, DT_LEFT);
		//== 選択表示
		if ( m_selectCaretIndex >= 0 && m_selectCaretIndex != m_caretIndex )
		{
			int x1 = m_selectCaretDrawPos.x;
			int x2 = m_caretDrawPos.x;
			if ( x1 > x2 )
			{
				Swap(x1, x2);
			}
			CRect rc(x1, 0, x2 - m_charSize.cx + 2, m_charSize.cy);
			m_DrawSelectMark(dc, rc + m_margin);
		}
		else if ( m_isShowCaretAlways && ! m_HasFocus() )
		{
			CRect rc(m_caretDrawPos, m_charSize);
			if ( m_isInsertMode )
			{
				rc.right = rc.left + 1;
			}
			rc.left--;
			rc += m_margin;
			::DrawFocusRect(dc, &rc);
		}
	}
	
	/**
	 * [描画] 選択マーク描画
	 *	@param dc 描画対象.
	 *	@param rect 範囲
	 */
	void m_DrawSelectMark(CDC& dc, const CRect& rect, bool isActive = true)
	{
		COLORREF color = isActive ? m_activeHighlightColor : m_inactiveHighlightColor;
		CSelectedBeltDrawer d(rect.Size(), color, GetBkColor(), isActive);
		d.Draw(dc, rect.left, rect.top);
	}

	void m_SetSelect(int index, const POINT& drawPos)
	{
		m_selectCaretIndex = index;
		m_selectCaretDrawPos = drawPos;
		if ( (index & 1) != 0 )
		{
			// 奇数なら
			m_selectCaretIndex--;
			m_selectCaretDrawPos.x -= m_charSize.cx;
		}
	}

	void m_ReleaseSelect(void)
	{
		if ( m_selectCaretIndex >= 0 )
		{
			m_selectCaretIndex = -1;
		}
	}

	void m_HideCaret(void)
	{
		m_hasCaret = false;
		_super::HideCaret();
//		::DestroyCaret();
	}

	bool m_HasFocus(void) const
	{
		return (::GetFocus() == _super::GetSafeHwnd());
	}

	// 表示関係
	CFontHandle		m_font;					///< フォント
	CSize			m_charSize;				///< 文字サイズ。(幅は Insert用カレットの幅にも使用)
	CPoint			m_margin;				///< 左上の表示位置マージン
	CToolTipCtrl	m_toolTip;				///< ツールチップ
	COLORREF		m_textColor;			///< 文字の色
	COLORREF		m_backColor;			///< 背景の色
	COLORREF		m_activeHighlightColor;	///< アクティブハイライト色
	COLORREF		m_inactiveHighlightColor;///<非アクティブハイライト色
	// 状態関係
	bool			m_isInsertMode;			///< インサートモード
	int				m_maxLength;			///< 入力BYTE最大長。-1 なら無限。
	bool			m_isEnableNotify;		///< 通知許可
	// データ関係
	CByteVector		m_data;					///< 持っているデータ
	CString			m_dataString;			///< データを文字列化したもの
	BYTE			m_defaultData;
	// カレット関係
	int				m_caretIndex;			///< カレット位置。(BYTEに付き INDEX 2つ)
	CPoint			m_caretDrawPos;			///< カレット表示座標。(クライアント座標。x が -1 なら未計算を表す)
	int				m_selectCaretIndex;		///< 選択開始カレット位置。(-1 なら未選択状態を表す)
	CPoint			m_selectCaretDrawPos;	///< 選択開始カレット表示座標。(クライアント座標)
	bool			m_isShowCaretAlways;	///< カレット位置を常に表示
	bool			m_hasCaret;				///< カレット持ち中？
	// スクロール関係
	bool			m_isCaletVisidle;		///< カレットを見えるようにする。
	CPoint			m_viewPos;				///< クライアントの表示位置
};



/**@ingroup MFCCONTROL
 * シングルラインHEXデータ入力Editコントロール
 *
 *		1byte 毎にスペースが空くので、個々のデータが見やすく編集できます。
 *
 *	@note ベースは EDITコントロール( CEdit )ではないので、ご注意下さい。
 *	
 *	@par使い方
 *		リソースエディタでスタティックコントロール（エディットコントロール）を張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic or CEdit）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcEditHexData.h
 * 
 *	@date 10/04/01	新規作成
 */
class CEditHexData : public CEditHexDataT<>
{
};



/**@ingroup MFCCONTROL
 * マルチラインHEXデータ入力Editコントロール
 *
 *		1byte 毎にスペースが空くので、個々のデータが見やすく編集できます。
 *		16byte毎に改行されます。
 *
 *	@note ベースは EDITコントロール( CEdit )ではないので、ご注意下さい。
 *
 *	@attention このコントロールは等幅フォントを使用してください。
 *	
 *	@par使い方
 *		リソースエディタでスタティックコントロール（エディットコントロール）を張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic or CEdit）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcEditHexData.h
 * 
 *	@date 10/04/01	新規作成
 */
class CEditHexData16 : public CEditHexDataT<16>
{
};



}; //MFC
}; //TNB
