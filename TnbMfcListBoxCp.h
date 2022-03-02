#pragma once
/**
 *@file
 * コピー機能付ListBox関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbClipboard.h"
#include "TnbStrAdder.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 文字列コピー機能付ListBoxコントロール
 *
 *		Ctrl+A , Ctrl+C が使えるListBoxコントロールになります。
 *
 *	@par使い方
 *		リソースエディタでリストボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListBox）を本クラスに置き換えて使用します。\n
 *		リストボックスのプロパティは、「選択(C)」を「拡張」にしておいてください。
 * 
 *	@note スクロールすると、親に WM_VSCROLL を SendMessage します。LPARAM には 本コントロールの HWND を渡します。
 *		そのため、 MFC の CWnd::OnVScroll() の第三引数の CScrollBar* には本クラスのインスタンスが渡ります。
 *		CScrollBar* として操作しないように注意してください。また、 WPARAM には、現在の TopIndex がはいっていますが、
 *		65535 行を超えると、おかしな値になりますので、 GetTopIndex() で現在の表示位置を確認するようにしてください。
 *		
 *
 *	@par必要ファイル
 *			TnbMfcListBoxCp.h
 * 
 *	@date 06/01/01	新規作成
 *	@date 09/10/05	PostString() 追加
 *	@date 10/02/15	最大ライン数設定可能に.
 *	@date 10/05/20	Ctrl+A で LBN_SELCHANGE を通知するようにした。
 *	@date 11/03/02	最大ライン数のデフォルトを変更。 
 *	@date 11/07/20	スクロールしたら親に WM_VSCROLL を SendMessage するようにした。 LPARAM には 本コントロールの HWND。
 *	@date 14/01/10	GetText() を追加。LBS_HASSTRING スタイルをつけられないサブクラス対策。
 */
class CListBoxCp : public CListBox
{
	DEFSUPER(CListBox);
public:

	/// コンストラクタ
	CListBoxCp(void) : m_maxLine(20000)
	{
	}

	/**
	 * [設定] 最大ライン数設定.
	 *	@note デフォルトは 20000 行です。
	 *	@param maxLine 最大ライン。 -1 なら、最大ラインのチェックをしません(推奨しません)。
	 */
	void SetMaxLine(int maxLine)
	{
		m_maxLine = maxLine;
	}

	/**
	 * [取得] 最大ライン数取得.
	 *	@retval 0未満 最大ライン、未指定.
	 *	@retval 0以上 
	 */
	int GetMaxLine(void) const
	{
		return m_maxLine;
	}

	/**
	 * [選択] 全項目選択.
	 */
	void AllSetSel(void)
	{
		int iItemCount = _super::GetCount();
		if ( iItemCount > 0 )
		{
			_super::SelItemRange(TRUE, 0, iItemCount - 1);
		}
	}

	/**
	 * [処理] 文字列取得.
	 *		LIST 内の文字列を改行で連結し返します。
	 *	@param boIsSelectOnly true なら選択されている項目のみを収集します。
	 *	@return 文字列。
	 */
	CString ToString(bool boIsSelectOnly = true)
	{
		CStrAdder strText;
		int iItemCount = _super::GetCount();
		if ( 0 < iItemCount )
		{
			CString s;
			for ( int i = 0; i < iItemCount; i++ )
			{
				if ( ! boIsSelectOnly || _super::GetSel(i) > 0 )
				{
					//選択されている
					GetText(i, s);
					strText += s;
					strText += _T("\r\n");
				}
			}
		}
		return CString(strText);
	}

	/**
	 * [処理] 選択項目コピー.
	 *		LIST 内の文字列を改行で連結し、クリップボードへコピーします。
	 *	@param boIsSelectOnly true なら選択されている項目のみを収集します。
	 */
	void Copy(bool boIsSelectOnly = true)
	{
		CString strText = ToString(boIsSelectOnly);
		if ( ! strText.IsEmpty() )
		{
			CClipboard clip(_super::m_hWnd);
			clip.SetString(strText);
		}
	}

	/**
	 * [追加] 文字列追加.
	 *	@note 別スレッドからでも追加が可能です。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 */
	void PostString(int nIndex, LPCTSTR lpszItem)
	{
		if ( ! ::IsWindow(m_hWnd) ) { return; }
		CString* P = new CString(lpszItem);
		if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
		{
			delete P;
		}
	}

	/**
	 * [追加] 文字列追加.
	 *	@note 別スレッドからの実行は、予想外の動作をすることがあります。 PostString() を使用してください。
	 *	@param nIndex 文字列を挿入する位置を 0 から始まるインデックスで指定します。
	 *					このパラメータが -1 の場合、文字列はリストの最後に追加されます。
	 *	@param lpszItem 挿入する文字列。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。挿入されたインデックスを返します。
	 */
	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		m_CheckMaxLine();
		return _super::InsertString(nIndex, lpszItem);
	}

	/**
	 * [追加] 文字列追加.
	 *	@note 別スレッドからの実行は、予想外の動作をすることがあります。 PostString() を使用してください。
	 *	@param lpszItem 挿入する文字列。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。挿入されたインデックスを返します。
	 */
	int AddString(LPCTSTR lpszItem)
	{
		m_CheckMaxLine();
		return _super::AddString(lpszItem);
	}

	/**
	 * [削除] 文字列削除.
	 *	@note 削除後、表示位置を top に戻します。
	 *	@param nIndex 削除する文字列を 0 から始まるインデックスで指定します。
	 *	@retval LB_ERR エラー。
	 *	@retval 上記以外 成功。数値は、残りのアイテム数
	 */
	int DeleteString(UINT nIndex)
	{
		return _super::DeleteString(nIndex);
	}

	/**
	 * [削除] 文字列削除.
	 *	@note 表示位置を変えずに、文字列を削除します。
	 *	@param nIndex 削除する文字列を 0 から始まるインデックスで指定します。
	 *	@retval LB_ERR エラー。
	 *	@retval 上記以外 成功。数値は、残りのアイテム数
	 */
	int DeleteStringEx(UINT nIndex)
	{
		_super::SetRedraw(FALSE);
		int sel = _super::GetTopIndex();
		int r = _super::DeleteString(nIndex);
		_super::SetTopIndex(sel);
		_super::SetRedraw(TRUE);
		_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE);
		return r;
	}

	/**
	 * [取得] 文字列取得.
	 *	@param[in] nIndex 取得する文字列を 0 から始まるインデックスで指定します。
	 *	@param[out] lpszBuffer 文字列を格納するアドレスを指定します。 
	 *	@retval LB_ERR エラー。
	 *	@retval 上記以外 成功。数値は、取得した文字数。
	 */
	virtual int GetText(int nIndex, LPTSTR lpszBuffer) const
	{
		return _super::GetText(nIndex, lpszBuffer);
	}

	/**
	 * [取得] 文字列取得.
	 *	@param[in] nIndex 取得する文字列を 0 から始まるインデックスで指定します。
	 *	@param[out] rString 文字列を格納されます。
	 */
	virtual void GetText(int nIndex, CString& rString) const
	{
		_super::GetText(nIndex, rString);
	}
	
protected:
	enum { WM_CLB_POSTSTRING = WM_APP }; ///< PostString 用メッセージ

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		#ifdef _DEBUG
			DWORD dwStyle = _super::GetStyle();
			ASSERT(dwStyle & LBS_EXTENDEDSEL);
		#endif
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
		case WM_CHAR:
			if ( m_CheckCtrlKey(wParam, lParam) )
			{
				return TRUE;
			}
			break;
		case WM_CLB_POSTSTRING:
			{
				CString* P = reinterpret_cast<CString*>(lParam);
				if ( ::IsWindow(m_hWnd) )
				{
					int iIndex = ToInt(wParam);
					if ( m_maxLine >= 0 && _super::GetCount() >= m_maxLine )
					{
						_super::SetRedraw(FALSE);
						_super::DeleteString(0);
						LRESULT r = _super::InsertString(iIndex, *P);
						_super::SetTopIndex(ToInt(r));
						_super::SetRedraw(TRUE);
						_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE);
					}
					else
					{
						LRESULT r = _super::InsertString(iIndex, *P);
						_super::SetTopIndex(ToInt(r));
					}
				}
				delete P;
				return 0;
			}
			break;
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
			{
				_super::SetRedraw(FALSE);
				int top1 = _super::GetTopIndex();
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				int top2 = _super::GetTopIndex();
				_super::SetTopIndex(top1);
				_super::SetRedraw(TRUE);
				if ( top1 == top2 )
				{
					_super::RedrawWindow(NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE | RDW_ERASE);
				}
				else
				{
					_super::SetTopIndex(top2);
				}
				CWnd* pWnd = GetParent();
				if ( pWnd != NULL )
				{
					if ( message == WM_VSCROLL )
					{
						pWnd->SendMessage(WM_VSCROLL, wParam, reinterpret_cast<LPARAM>(GetSafeHwnd()));
					}
					else
					{
						WPARAM w = MAKELONG(SB_THUMBPOSITION, GetTopIndex());
						pWnd->SendMessage(WM_VSCROLL, w, reinterpret_cast<LPARAM>(GetSafeHwnd()));
					}
				}
				return r;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/**
	 * [確認] 入力キーのチェック.
	 *		Ctrl+A,Ctrl+C のチェックをします。
	 *	@param wParam WM_CHAR のWPARAM。
	 *	@param lParam WM_CHAR のLPARAM。
	 *	@retval true 処理済。
	 *	@retval false 未処理。
	 */
	bool m_CheckCtrlKey(WPARAM wParam, LPARAM lParam)
	{
		LPARAM lp = lParam & 0x00FFFFFF;
		if ( lp == 0x002e0001 && wParam == 0x03 )
		{
			//Ctrl + C
			CWaitCursor a;
			Copy();
			return true;
		}
		if ( lp == 0x001e0001 && wParam == 0x01 )
		{
			//Ctrl + A
			CWaitCursor a;
			AllSetSel();
			MFCLIB::SendCommandMessage(this, LBN_SELCHANGE);
			return true;
		}
		return false;
	}
	/// 追加できるか？限界超えるならDelete
	void m_CheckMaxLine(void)
	{
		if ( m_maxLine >= 0 && _super::GetCount() >= m_maxLine )
		{
			_super::DeleteString(0);
		}
	}
	int					m_maxLine;				///< 最大ライン
};



}; // MFC
}; // TNB
