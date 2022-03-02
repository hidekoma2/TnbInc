#pragma once
/**
 *@file
 *	タブコントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbTextDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * タブコントロール
 *
 *	@par使い方
 *		リソースエディタでタブコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CTabCtrl）を本クラスに置き換えて使用します。
 *
 *	@see CCustomWindowFrame を使う場合、タブコントロールは本クラスを使う必要があります。
 *
 *	@par必要ファイル
 *			TnbMfcTabCtrlEx.h
 * 
 *	@date 09/04/22	新規作成
 *	@date 12/02/29	SetItemText() 新規。
 */
class CTabCtrlEx : public CTabCtrl
{
	DEFSUPER(CTabCtrl);
public:

	/// コンストラクタ
	CTabCtrlEx(void) : m_selectTextColor(CLR_INVALID), m_selectBackColor(CLR_INVALID), m_isAttachedSpin(false)
	{
	}

	/**
	 * [設定] 選択タブカラー設定.
	 *	@param text テキスト色
	 *	@param back 背景色
	 */
	void SetSelectedColor(COLORREF text, COLORREF back = CLR_INVALID)
	{
		m_selectTextColor = text;
		m_selectBackColor = back;
	}

	/**
	 * [取得] Item文字列取得.
	 *	@param nItem タブの 0 から始まるインデックス。
	 *	@return 文字列。
	 */
	CString GetItemText(int nItem) const
	{
		TC_ITEM	tci;
		CString s;
		tci.mask = TCIF_TEXT;
		tci.pszText = s.GetBuffer(512);
		tci.cchTextMax = 511;
		if ( _super::GetItem(nItem, &tci) )
		{
			s.ReleaseBuffer();
		}
		else
		{
			s.ReleaseBuffer();
			s.Empty();
		}
		return s;
	}

	/**
	 * [設定] Item文字列設定.
	 *	@param nItem タブの 0 から始まるインデックス。
	 *	@param lpszText 文字列。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetItemText(int nItem, LPCTSTR lpszText)
	{
		TC_ITEM	tci;
		CString s = lpszText;
		tci.mask = TCIF_TEXT;
		tci.pszText = s.GetBuffer(512);
		tci.cchTextMax = 511;
		BOOL r = _super::SetItem(nItem, &tci);
		s.ReleaseBuffer();
		return !! r;
	}

	/**
	 * [取得] スピンボタン取得.
	 *	@note タブコントロールについているスピンボタンを返します。
	 *	@return スピンボタン
	 */
	CSpinButtonCtrl* GetSpinButtonCtrl(void)
	{
		if ( ! m_isAttachedSpin && ! ::IsWindow(m_spin) )
		{
			m_spin.SubclassWindow(GetDlgItem(1)->GetSafeHwnd());
		}
		return &m_spin;
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_isAttachedSpin = false;
		_super::ModifyStyle(0, TCS_OWNERDRAWFIXED);
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
		if ( message == WM_CREATE )
		{
			m_isAttachedSpin = false;
		}
		if ( message == WM_DESTROY )
		{
			if ( ::IsWindow(m_spin) )
			{
				m_spin.UnsubclassWindow();
			}
		}
		if ( ! m_isAttachedSpin && ! ::IsWindow(m_spin) )
		{
			HWND h = GetDlgItem(1)->GetSafeHwnd();
			if ( h != NULL )
			{
				m_spin.SubclassWindow(h);
				m_isAttachedSpin = true;
			}
		}
		if ( message == WM_ERASEBKGND )
		{
			HDC dc = reinterpret_cast<HDC>(wParam);
			LRESULT res = _super::GetParent()->SendMessage(WM_CTLCOLORBTN, reinterpret_cast<WPARAM>(dc), _super::GetDlgCtrlID());
			HBRUSH b = reinterpret_cast<HBRUSH>(res);
			if ( b != NULL )
			{
				CRect rc;
				GetClientRect(rc);
				POINT po;
				MFCLIB::GetControlPos(this, po);
				::SetBrushOrgEx(dc, po.x, po.y, NULL);
				::FillRect(dc, &rc, b);
				return FALSE;
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
	
	/**
	 * [通知] オーナードロー処理.
	 * 		オーナードロー時コールされます。
	 *	@param lpDrawItemStruct 描画情報.
	 */
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if ( lpDrawItemStruct->CtlType != ODT_TAB || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		const CString& s = GetItemText(lpDrawItemStruct->itemID);
		RECT rc = lpDrawItemStruct->rcItem;
		HDC dc = lpDrawItemStruct->hDC;
		LRESULT res = _super::GetParent()->SendMessage(WM_CTLCOLORBTN, reinterpret_cast<WPARAM>(dc), _super::GetDlgCtrlID());
		HBRUSH b = reinterpret_cast<HBRUSH>(res);
		COLORREF c = ::GetTextColor(dc);
		if ( (lpDrawItemStruct->itemState & ODS_SELECTED) != 0 )
		{
			if ( IS_RGBVALUE(m_selectBackColor) )
			{
				CBrush br;
				br.CreateSolidBrush(m_selectBackColor);
				::FillRect(dc, &rc, br);
			}
			else
			{
				::FillRect(dc, &rc, b);
			}
			if ( IS_RGBVALUE(m_selectTextColor) )
			{
				c = m_selectTextColor;
			}
		}
		else
		{
			::FillRect(dc, &rc, b);
		}
		::SetBkMode(dc, TRANSPARENT);
		rc.top += 5;
//		rc.bottom -= 3;//下付きの場合

		if ( false )
		{
			LOGFONT lf;
			CFont* pFont = GetFont();
			pFont->GetLogFont(&lf);
			lf.lfEscapement = 900;
			CStr s = "@";
			s += lf.lfFaceName;
			_tcscpy(lf.lfFaceName, s);
			CFont f;
			f.CreateFontIndirect(&lf);
			HGDIOBJ h = ::SelectObject(dc, f);
			CTextDrawer::DrawTextRect(dc, rc, DT_CENTER, c, s);
			::SelectObject(dc, h);
			return; 
		}


		CTextDrawer::DrawTextRect(dc, rc, DT_CENTER, c, s);
	}

private:
	COLORREF		m_selectTextColor;		///< 選択文字色
	COLORREF		m_selectBackColor;		///< 選択背景色
	CSpinButtonCtrl	m_spin;
	bool			m_isAttachedSpin;
};



}; // MFC
}; // TNB
