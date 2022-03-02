#pragma once
/**
 *@file
 * カラー選択ボタン関係のヘッダ
 *
 *		カラーを選択するボタンを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcDrawingButton.h"
#include "TnbFontHandle.h"
#include "TnbAccessor.h"



//TNB Library
namespace TNB {
namespace MFC {


	
/**
 * カラー選択ボタン 選択変更.
 *	@see CColorSelectButton
 */
#define CSBN_SELCHANGE 10



/**@ingroup MFCCONTROL
 * カラー選択ボタンコントロール
 *
 *	@note カラーを一つ選択することが出来るボタンコントロールになります。
 *
 *	@par使い方
 *		リソースエディタでボタンコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton）を本クラスに置き換えて使用します。
 *
 *	@note カラーが変更されると、 CSBN_SELCHANGE が親に通知されます。
 *		\code
 *		BEGIN_MESSAGE_MAP(CFooDlg, CDialog)
 *		//{{AFX_MSG_MAP(CFooDlg)
 *					:
 *			ON_CONTROL(CSBN_SELCHANGE, IDC_BUTTON_COLOR, OnColorButtonSelChange)
 *					:
 *		//}}AFX_MSG_MAP
 *		END_MESSAGE_MAP()
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbMfcColorSelectButton.h
 * 
 *	@date 10/05/24 新規作成
 *	@date 16/06/28 デフォルトカラーが履歴になるように仕様変更。
 *	@date 16/06/28 デフォルトカラーが指定の IAccessor に保存されるように機能追加。
 */
class CColorSelectButton : public CDrawingButton
{
	DEFSUPER(CDrawingButton);
public:

	/// コンストラクタ
	CColorSelectButton(void) : m_pAccessor(NULL)
	{
		m_face.SetSelectColor(RGB(255, 255, 255));
		m_default.Resize(DEFAULTMAX);
		loop ( i, DEFAULTMAX )
		{
			BYTE r = down_cast<BYTE>((i & _BIT(0)) == 0 ? 255 : 0);
			BYTE g = down_cast<BYTE>((i & _BIT(1)) == 0 ? 255 : 0);
			BYTE b = down_cast<BYTE>((i & _BIT(2)) == 0 ? 255 : 0);
			m_default[i] = RGB(r, g, b);
		}
	}

	/**
	 * [設定] アクセサー指定.
	 *	@param pAccessor アクセサー。指定したアクセサーは破棄しないでください。
	 *	@param lpszAccessorSectionName アクセサーのセクション名
	 */
	void SetAccessor(IAccessor* pAccessor, LPCTSTR lpszAccessorSectionName)
	{
		m_pAccessor = pAccessor;
		m_accessorSectionName = lpszAccessorSectionName;
		IAccessor::CSection sec(m_pAccessor, m_accessorSectionName);
		loop ( i, DEFAULTMAX )
		{
			CStr key = CStr::Fmt(_T("c%d"), i + 1);
			m_default[i] = sec.QueryDword(key, m_default[i]);
		}
	}

	/**
	 * [設定] 選択色設定.
	 *	@param c 色
	 *	@return 設定前の色
	 */
	COLORREF SetSelectColor(COLORREF c)
	{
		COLORREF r = m_face.GetSelectColor();
		m_face.SetSelectColor(c);
		m_InsertDefaultColor(c);
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			_super::SetNormalDrawer(m_face);
			_super::RedrawWindow();
		}
		return r;
	}

	/**
	 * [取得] 選択色取得.
	 *	@return 色
	 */
	COLORREF GetSelectColor(void) const
	{
		return m_face.GetSelectColor();
	}

	/**
	 * [設定] デフォルト色設定.
	 *	@param index インデックス。0～7 が指定可能。
	 *	@param c 色
	 *	@return 設定前の色
	 */
	COLORREF SetDefaultColor(INDEX index, COLORREF c)
	{
		if ( index >= 0 && index < DEFAULTMAX )
		{
			COLORREF r = m_default[index];
			m_default[index] = c;
			return r;
		}
		return INVALID_INDEX;
	}

	/**
	 * [取得] デフォルト色取得.
	 *	@param index インデックス。0～7 が指定可能。
	 *	@return 色
	 */
	COLORREF GetDefaultColor(INDEX index) const
	{
		if ( index >= 0 && index < DEFAULTMAX )
		{
			return m_default[index];
		}
		return INVALID_INDEX;
	}

protected:

	/**
	 * [通知] メニュー描画.
	 *	@param nIDCtl コントロール
	 *	@param lpDrawItemStruct 描画情報
	 */
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if ( lpDrawItemStruct->CtlType == ODT_MENU )
		{
			HDC dc = lpDrawItemStruct->hDC;
			CRect rc = lpDrawItemStruct->rcItem;
			COLORREF color = lpDrawItemStruct->itemData;
			{
				CDcSelectAssistant dca(dc);
				bool isSel = ((lpDrawItemStruct->itemState & CDIS_SELECTED) != 0);
				::FillRect(dc, &rc, ::GetSysColorBrush(isSel ? COLOR_HIGHLIGHT : COLOR_MENU));
				CBrush brush(color);
				dca.SelectBrush(brush);
				CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
				dca.SelectPen(pen);
				rc.DeflateRect(3, 3);
				::Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
			}
		}
		_super::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
	
	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		SetBitmapChangeTime(0);
		_super::PreSubclassWindow();
		CRect rc;
		_super::GetClientRect(rc);
		CFontHandle f;
		f.Set(80, _T("MS Gothic"));
		COLORREF c = m_face.GetSelectColor();
		m_face.SetFont(f);
		m_face.Resize(rc.Size());
		m_face.SetTextColor(CLR_INVALID);
		m_face.SetSelectColor(CLR_INVALID);
		_super::SetDisableDrawer(m_face);
		m_face.SetTextColor(RGB(0, 0, 0));
		m_face.SetSelectColor(c);
		_super::SetNormalDrawer(m_face);
	}

	/**
	 * [通知] クリック.
	 *		BN_CLICKED を親に通知する前にコールされます。
	 */
	virtual void OnClickButton(void)
	{
		_super::OnClickButton();
		CMenu menu;
		CRect rc;
		_super::GetWindowRect(&rc);
		menu.CreatePopupMenu();
		UINT pos = static_cast<UINT>(-1);
		loop ( i, DEFAULTMAX )
		{
			menu.InsertMenu(pos, MF_OWNERDRAW, ITEM_DEFAULTTOP + i, reinterpret_cast<LPCTSTR>(m_default[i]));
		}
		menu.InsertMenu(pos, MF_SEPARATOR);
		menu.InsertMenu(pos, 0, ITEM_OTHER, _T("その他..."));
		menu.TrackPopupMenu(0, rc.left, rc.bottom, this, NULL);
		return;
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
		if ( message == WM_DRAWITEM )
		{
			OnDrawItem(wParam, reinterpret_cast<LPDRAWITEMSTRUCT>(lParam));
		}
		else if ( message == WM_COMMAND )
		{
			if ( wParam == ITEM_OTHER )
			{
				CColorDialog dlg(m_face.GetSelectColor());
				if ( dlg.DoModal() == IDOK )
				{
					COLORREF c = dlg.GetColor();
					m_face.SetSelectColor(c);
					m_InsertDefaultColor(c);
					_super::SetNormalDrawer(m_face);
					_super::Invalidate();
					MFCLIB::SendCommandMessage(this, CSBN_SELCHANGE);
				}
			}
			else if ( wParam >= ITEM_DEFAULTTOP && ITEM_DEFAULTTOP + DEFAULTMAX > wParam )
			{
				COLORREF c = m_default[wParam - 2000];
				m_face.SetSelectColor(c);
				m_InsertDefaultColor(c);
				_super::SetNormalDrawer(m_face);
				_super::Invalidate();
				MFCLIB::SendCommandMessage(this, CSBN_SELCHANGE);
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// デフォルトカラーに挿入
	void m_InsertDefaultColor(COLORREF c)
	{
		if ( m_default[0] == c )
		{
			return;
		}
		loop ( i, m_default.GetSize() )
		{
			if ( c == m_default[i] )
			{
				// 途中に同じ色があった。
				loop_dn ( j, i )
				{
					m_default[j + 1] = m_default[j];
				}
				m_default[0] = c;
				m_WriteDefault();
				return;
			}
		}
		loop_dn ( i, m_default.GetSize() - 1)
		{
			m_default[i + 1] = m_default[i];
		}
		m_default[0] = c;
		m_WriteDefault();
	}

	/// デフォルト(アクセサーに)書込み
	void m_WriteDefault(void)
	{
		if ( m_pAccessor == NULL )
		{
			return;
		}
		IAccessor::CSection sec(m_pAccessor, m_accessorSectionName);
		loop ( i, DEFAULTMAX )
		{
			CStr key = CStr::Fmt(_T("c%d"), i + 1);
			sec.WriteDword(key, m_default[i]);
		}
	}

	/// ボタンフェイス描画
	class CFaceDrawer : public CNullDrawer
	{
	public:
		/// コンストラクタ
		CFaceDrawer(CFontHandle font = NULL, COLORREF textColor = CLR_INVALID, COLORREF selectColor = CLR_INVALID)
			: m_font(font), m_selectColor(selectColor), m_textColor(textColor)
		{
		}
		// 選択色取得
		COLORREF GetSelectColor(void) const
		{
			return m_selectColor;
		}
		// 選択色設定
		void SetSelectColor(COLORREF c)
		{
			m_selectColor = c;
		}
		// 文字色設定
		void SetTextColor(COLORREF c)
		{
			m_textColor = c;
		}
		// フォント設定
		void SetFont(CFontHandle font)
		{
			m_font = font;
		}
		/**
		 * [作成] クローン作成.
		 *		自分と同じ処理を行うクラスを作成します。
		 *	@return ポインタ。使用後は delete する必要があります。
		 */
		virtual IDrawable* Clone(void) const 
		{
			CFaceDrawer* P = new CFaceDrawer;
			*P = *this;
			return P;
		}
		/**
		 * [描画] 描画.
		 *		指定のデバイスコンテキストの指定の座標に描画します。
		 *	@note 本クラスは何もしません。
		 *	@param dc デバイスコンテキスト
		 *	@param x Ｘ座標
		 *	@param y Ｙ座標
		 */
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			CDcSelectAssistant dca(dc);
			CRect rc(CPoint(x, y), m_size);
			dca.SelectFont(m_font);
			rc.DeflateRect(5, 5);
			if ( m_font != NULL )
			{
				CRect r = rc;
				CTextDrawer::DrawTextRect(r, dc, DT_RIGHT | DT_VCENTER, m_textColor, _T("▼"));
				r.left -= 3;
				r.right = r.left + 2;
				::DrawEdge(dc, &r, EDGE_ETCHED, BF_LEFT | BF_RIGHT);
				rc.right = r.left - 4;
			}
			if ( IS_RGBVALUE(m_selectColor) )
			{
				CBrush brush(m_selectColor);
				CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
				dca.SelectBrush(brush);
				dca.SelectPen(pen);
				::Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
			}
		}
	private:
		COLORREF	m_selectColor;	///< カラー　CLR_INVALIDなら表示しない
		COLORREF	m_textColor;	///< カラー　CLR_INVALIDならエンボス
		CFontHandle	m_font;			///< フォント
	};

	enum
	{
		DEFAULTMAX = 8,			///< デフォルト数
		ITEM_OTHER = 1000,		///< その他メニュー
		ITEM_DEFAULTTOP = 2000,	///< デフォルトトップ
	};
	CFaceDrawer				m_face;					///< ボタン描画情報
	CWorkMemT<COLORREF>		m_default;				///< デフォルト
	IAccessor*				m_pAccessor;			///< アクセサ
	CStr					m_accessorSectionName;	///< アクセサのセクション名
};



}; //MFC
}; //TNB



