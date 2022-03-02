#pragma once
/**
 *@file
 * 文字入力Edit関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include <imm.h>
#ifndef _WIN32_WCE
 #pragma comment(lib,"imm32.lib")
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ASCII文字専用Editコントロール
 *
 *		２バイトコードの入力を禁止できます。
 *		継承して、 m_strValidChars を書き換えると有効な文字を指定できます。
 *		IsValidChar() をオーバーライドすることで、より詳細な入力制限が出来ます。
 *		また、入力された文字を差し替えることも出来ます。
 *		OnUpdateChar() をオーバーライドすることで、変更イベントをハンドル出来ます。
 *
 *	@par使い方
 *		リソースエディタでエディトボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CEdit）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcEditAscii.h
 * 
 *	@date 06/01/01	新規作成
 *	@date 06/11/17	CStr の使用から CString に切り替え。
 *	@date 07/12/19	Cut&Pasteを制限可能に。Beepモードも用意。
 *	@date 08/05/15	TAB入力を整理。
 *	@date 09/09/16	CE 対策。
 */
class CEditAscii : public CEdit
{
	DEFSUPER(CEdit);
public:

	/// コンストラクタ
	CEditAscii(void) : _super(), m_boCanCutPaste(true), m_boCanCallUpdate(true), m_boIsPasting(false)
		, m_strValidChars(	_T("!\x22#$%&'()=~|")
							_T("1234567890-^\\")
							_T("@[;:],./")
							_T("`{+*}<>?_")
							_T("abcdefghijklmnopqrstuvwxyz")
							_T("ABCDEFGHIJKMNLOPQRSTUVWXYZ")
							_T(" ") )
	{
	}

	/**
	 * コンストラクタ
	 *	@param lpszValidChars 有効文字群指定。
	 */
	CEditAscii(LPCTSTR lpszValidChars) : _super(), m_boCanCutPaste(true), m_boCanCallUpdate(true), m_boIsPasting(false)
		, m_strValidChars(lpszValidChars)
	{
	}

	/// デストラクタ
	virtual ~CEditAscii() 
	{
	}

	/**
	 * [取得] 入力文字取得
	 *	@return 文字列
	 */
	CString GetText(void) const
	{
		CString s;
		_super::GetWindowText(s);
		return s;
	}

	/**
	 * [設定] 文字列設定
	 *	@param lpszText 設定する文字
	 *	@param boIsNomoveSel trueならSELの位置を変えないようにする。
	 *						falseならSELは初期化される(普通にSetWindowTextした時と同じ動作)。
	 */
	void SetText(LPCTSTR lpszText, bool boIsNomoveSel = false)
	{
		if ( ! boIsNomoveSel )
		{
			_super::SetWindowText(lpszText);
		}
		else
		{
			int iCurSel = LOWORD(_super::GetSel());
			_super::SetWindowText(lpszText);
			_super::SetSel(iCurSel, iCurSel);
		}
	}

	/**
	 * [設定] 入力可能文字設定.
	 *	@param chars 入力可能文字
	 */
	void SetValidChars(LPCTSTR chars)
	{
		m_strValidChars = chars;
	}

	/**
	 * [取得] 入力可能文字取得.
	 *	@return 入力可能文字
	 */
	CString GetValidChars(void) const
	{
		return m_strValidChars;
	}

	/**
	 * [設定] カットペーストモード.
	 *	@param r true ならペースト、カットを許可します。
	 *			false なら許可しません。
	 *			コンストラクタ時、「許可」になっています。
	 */
	void SetCutPasteMode(bool r)
	{
		m_boCanCutPaste = r;
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
		::ImmAssociateContext(_super::m_hWnd, NULL);	// IME入力禁止
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
		if ( message == WM_SETTEXT )
		{
			LRESULT l = _super::WindowProc(message, wParam, lParam);
//			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(_super::GetDlgCtrlID(), EN_UPDATE), (LPARAM)m_hWnd);
			MFCLIB::SendCommandMessage(this, EN_UPDATE);
			return l;
		}
		else if ( message == WM_KEYDOWN )
		{
			if ( wParam == VK_TAB && ::GetAsyncKeyState(VK_CONTROL) < 0 )
			{
				return 0;
			}
		}
		else if ( message == WM_CHAR )
		{
			if ( wParam == VK_TAB )
			{
				return 0;
			}
			UINT wp = down_cast<UINT>(wParam);
			LONG lp = down_cast<LONG>(lParam);
			if ( ! IsValidChar(wp, lp, GetText()) )
			{
				OnInputInvalidChar();
				return 0;
			}
		}
		else if ( message == WM_PASTE )
		{
			if ( ! m_boCanCutPaste )
			{
				return 0;
			}
			m_boIsPasting = true;
			_super::SetRedraw(FALSE);
			LRESULT l = _super::WindowProc(message, wParam, lParam);
			m_boIsPasting = false;
			_super::SetRedraw(TRUE);
			_super::Invalidate();
			return l;
		}
		else if ( message == WM_CONTEXTMENU )
		{
			if ( ! m_boCanCutPaste )
			{
				return 0;
			}
		}
		else if ( message == WM_LBUTTONDOWN )
		{
			#ifdef _WIN32_WCE
				return _super::DefWindowProc(message, wParam, lParam);
			#endif
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] for notifications from parent
	 *	@note
	 *		CWndのメソッドをオーバーライドしています。
	 *		メッセージ受信したらコールされます。
	 *		WM_COMMANDの処理をフックしています。
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
			if ( m_boCanCallUpdate )
			{
				m_boCanCallUpdate = false;
				int ctrlID = _super::GetDlgCtrlID();
				if ( wParam == MAKEWPARAM(ctrlID, EN_UPDATE) )
				{
					if ( m_boIsPasting )
					{
						OnPasteChar();
					}
					OnUpdateChar();
				}
				else if ( wParam == MAKEWPARAM(ctrlID, EN_SETFOCUS) )
				{
					OnEditStart();
				}
				else if ( wParam == MAKEWPARAM(ctrlID, EN_KILLFOCUS) )
				{
					OnEditEnd();
				}
				m_boCanCallUpdate = true;
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [通知] ペーストされた時
	 */
	virtual void OnPasteChar(void)
	{
		CString s = GetText();
		LONG lTemp = 0;
		UINT nChar;
		//
		int i = 0;
		bool boHasInvalidChar = false;
		while ( i < s.GetLength() )
		{
			nChar = s[i];
			_super::SetSel(i, i);
			if ( ! IsValidChar(nChar, lTemp, s.Left(i)) )
			{
				s.Delete(i);
				boHasInvalidChar = true;
			}
			else
			{
				s.SetAt(i, static_cast<TCHAR>(nChar));
				i++;
			}
		}
		if ( boHasInvalidChar )
		{
			OnInputInvalidChar();
		}
		SetText(s);
		_super::SetSel(i, i);
	}

	/**
	 * [確認] 入力許可チェック
	 *	@param[in,out] _nChar WM_CHAR の WPARAM
	 *	@param[in,out] _lFlags WM_CHAR の LPARAM
	 *	@param[in] strNowText 入力済みの文字列
	 *	@retval true 入力を認める。
	 *	@retval false 入力を認めない。
	 */
	virtual bool IsValidChar(UINT& _nChar, LONG& _lFlags, const CString& strNowText)
	{
		bool r = (_lFlags & 1) != 0;
		if ( r && _nChar == 0x03 )	//Ctrl + C
		{
			return true;
		}
		if ( m_boCanCutPaste )
		{
			if ( r && _nChar == 0x18 )	//Ctrl + X
			{
				return true;
			}
			if ( r && _nChar == 0x16 )	//Ctrl + V
			{
				return true;
			}
		}
		if ( _nChar == '\b' || _nChar == VK_RETURN )
		{
			return true;
		}
		if ( m_strValidChars.Find(static_cast<TCHAR>(_nChar)) >= 0 )
		{
			return true;
		}
		return false;
	}

	/**
	 * [通知] 禁止文字入力発生時に通知.
	 */
	virtual void OnInputInvalidChar(void)
	{
		::MessageBeep(MB_OK);
	}

	/**
	 * [通知] 入力時に通知
	 *	@note	継承して本メソッドに処理を記述します。
	 *			本関数内で GetText() で現在の文字を取得し SetText() を使い再設定することが可能。
	 */
	virtual void OnUpdateChar(void) 
	{
	}

	/**
	 * [通知] フォーカスを得た時に通知
	 *	@note	継承して本メソッドに処理を記述します。
	 *			本関数内で GetText() で現在の文字を取得し SetText() を使い再設定することが可能。
	 */
	virtual void OnEditStart(void) 
	{
	}

	/**
	 * [通知] フォーカスを失った時に通知
	 *	@note	継承して本メソッドに処理を記述します。
	 *			本関数内で GetText() で現在の文字を取得し SetText() を使い再設定することが可能。
	 */
	virtual void OnEditEnd(void) 
	{
	}

private:
	bool m_boCanCutPaste;		///< Paste、Cutを許可するか？
	CString m_strValidChars;	///< 有効文字群
	bool m_boCanCallUpdate;		///< OnUpdateChar()をコールするか？
	bool m_boIsPasting;			///< OnPasteChar()をコールするか？
};



/**@ingroup MFCCONTROL
 * HEX文字オンリーEditコントロール
 *
 *		0～9、A～Fまでのみ入力が可能です。
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 09/04/06	新規作成
 */
class CEditHex : public CEditAscii
{
	DEFSUPER(CEditAscii);
public:

	/// コンストラクタ
	CEditHex(void) : _super()
	{
		_super::SetValidChars(_T("1234567890") _T("abcdef") _T("ABCDEF"));
	}

protected:
	/**
	 * [確認] 入力許可チェック
	 *	@param[in,out] _nChar WM_CHAR の WPARAM
	 *	@param[in,out] _lFlags WM_CHAR の LPARAM
	 *	@param[in] strNowText 入力済みの文字列
	 *	@retval true 入力を認める。
	 *	@retval false 入力を認めない。
	 */
	virtual bool IsValidChar(UINT& _nChar, LONG& _lFlags, const CString& strNowText)
	{
		if ( _nChar >= 'a' && _nChar <= 'f' )
		{
			_nChar -= ('a' - 'A');
		}
		return _super::IsValidChar(_nChar, _lFlags, strNowText);
	}
};



/**@ingroup MFCCONTROL
 * ASCIIファイル名Editコントロール
 *
 *		0～9、a～z、記号のみ入力が可能です。ファイル名に使えない \ / : * ? " < > | は禁止されます。
 *		全角も禁止されます。ご注意。
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 14/12/24	新規作成
 */
class CEditFileName : public CEditAscii
{
	DEFSUPER(CEditAscii);
public:

	/// コンストラクタ
	CEditFileName(void) : _super()
	{
		_super::SetValidChars(
							_T("!#$%&'()=~")
							_T("1234567890-^")
							_T("@[;],.")
							_T("`{+}_")
							_T("abcdefghijklmnopqrstuvwxyz")
							_T("ABCDEFGHIJKMNLOPQRSTUVWXYZ")
							_T(" ") );
	}
};



}; //MFC
}; //TNB
