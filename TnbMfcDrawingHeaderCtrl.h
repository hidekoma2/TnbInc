#pragma once
/**
 *@file
 * 描画情報HeaderCtrl関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbVector.h"
#include "TnbBitmapImage.h"
#include "TnbTextDrawer.h"
#include "TnbPartsDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報HeaderCtrlコントロール
 *
 *		カラム単位で表示する描画情報を指定出来ます。
 *
 *		カラムの背景は SetBackground() で指定します。デフォルトでは、標準的なボタンのような表示になります。
 *		各カラムのアイテムは、その背景の中に表示されるようになります。
 *
 *	@note CDrawingListCtrl にて内包されています。 CDrawingListCtrl::GetHeaderCtrl() で参照可能です。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingHeaderCtrl.h
 * 
 *	@date 07/12/07	新規作成
 *	@date 10/01/06	カラムサイズ変更禁止機能を付加。
 *	@date 11/10/14	各アイテムの状態チェックにオーナードローではなくカスタムドローを使うようにした。 Reset() メソッドのダミー化。
 */
class CDrawingHeaderCtrl : public CHeaderCtrl
{
	DEFSUPER(CHeaderCtrl);
public:

	/// コンストラクタ
	CDrawingHeaderCtrl(void) : m_isStateChecking(false), m_isAutoAdjustWidth(true)
		, m_textColor(::GetSysColor(COLOR_WINDOWTEXT)), m_backColor(CLR_INVALID), m_isEnableChangeWidth(true)
	{
		m_pPartsDraw = &m_standardParts;
	}

	/**
	 * [設定] テキストカラー設定.
	 *	@note 各カラムの描画情報を設定していない場合、設定されているフォントと設定されている文字を表示します。
	 *			その際使われる色です。
	 *	@param color テキストカラー。
	 */
	void SetTextColor(COLORREF color)
	{
		m_textColor = color;
	}

	/**
	 * [設定] 背景設定
	 *	@param pParts カラム枠表示。 IPartsDrawable::DrawButtonControl() の DFCS_BUTTONPUSH を使用します。
	 *	@param color 背景色。カラムのないところを塗る色を指定します。省略すると、 pParts で描画します
	 */
	void SetBackground(const IPartsDrawable* pParts, COLORREF color = CLR_INVALID)
	{
		m_pPartsDraw = pParts;
		m_backColor = color;
		if ( m_backBrush.GetSafeHandle() != NULL )
		{
			m_backBrush.DeleteObject();
		}
	}

	/**
	 * [設定] カラム描画情報設定
	 *	@param col カラムナンバー。
	 *	@param draw 描画情報。カラム枠の中に納まるようにしてください。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetColumnDrawer(int col, const IDrawable& draw)
	{
		if ( col >=0 && _super::GetItemCount() > col )
		{
			return m_columnDrawers.Set(col, draw.Clone());
		}
		return false;
	}

	/**
	 * [設定] 幅自動調整モード設定.
	 *	@note デフォルトでは true です。
	 *	@param b true すると、幅をクライアントと同じになるように調整します(横スクロールバーが出ない)。\n
	 */
	void SetAdjustWidthMode(bool b = true)
	{
		m_isAutoAdjustWidth = b;
	}

	/**
	 * [取得] 幅自動調整モード.
	 *	@retval true 幅をクライアントと同じになるように調整します(横スクロールバーが出ない)。
	 *	@retval false 自動調整なし
	 */
	bool IsAdjustWidthMode(void) const
	{
		return m_isAutoAdjustWidth;
	}

	/**
	 * [設定] 幅変更モード設定.
	 *	@param b true を指定すると、マウスにより各カラムの幅を調整できます。 false なら、それを禁止します。
	 */
	void EnableChangeWidth(bool b = true)
	{
		m_isEnableChangeWidth = b;
	}

	/**
	 * [確認] コントロールが存在するか？
	 *	@retval true ある
	 *	@retval false ない(クリエイト or アタッチされていない)
	 */
	bool IsWindow(void) const
	{
		return !! ::IsWindow(_super::GetSafeHwnd());
	}

	/**
	 * [取得] カラム数取得.
	 *	@note コントロールが存在しない場合、１を返します。
	 *	@return 数。
	 */
	int GetItemCount() const
	{
		return IsWindow() ? _super::GetItemCount() : 1;
	}

	/**
	 * [設定] 設定.
	 *	@deprecated 廃止予定のメソッドです。
	 */
	void _deprecated Reset(void)
	{
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		if ( (GetStyle() & HDS_BUTTONS) == 0 )
		{
			m_isEnableChangeWidth = false;
		}
		_super::PreSubclassWindow();
	}

	/**
	 * [通知] for notifications from parent
	 *		メッセージ受信したらコールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE	処理済。
	 *	@retval FALSE	未処理。
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_NOTIFY )
		{
			LPNMHDR lpNmHdr = reinterpret_cast<LPNMHDR>(lParam);
			if ( lpNmHdr->code == NM_CUSTOMDRAW ) 
			{
				LPNMLVCUSTOMDRAW P = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
				*_pResult = CDRF_DODEFAULT;
				switch ( P->nmcd.dwDrawStage )
				{
				case CDDS_PREPAINT:
					*_pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
					break;
				case CDDS_ITEMPREPAINT:
					m_columnStates.Set(P->nmcd.dwItemSpec, P->nmcd.uItemState);
					break;
				}
				return TRUE;
			}
		} 
		return FALSE;
	}

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *		ListBox 関係のメッセージをフックしています。
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
		case WM_PAINT:
			m_Draw(CPaintDC(this));
			return 0;
			break;
		case WM_PRINTCLIENT:
			if ( ! m_isStateChecking )
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				m_Draw(dc);
				return 0;
			}
			break;
		case WM_ERASEBKGND:
			if ( ! m_isStateChecking )
			{
				return 0;
			}
			break;
		case WM_MOUSEMOVE:
		case WM_SETCURSOR:
		case WM_LBUTTONDBLCLK:
			if ( ! m_isEnableChangeWidth )
			{
				return 0;
			}
			break;
		case HDM_DELETEITEM:
			{
				INDEX col = wParam;
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				if ( r != 0 )
				{
					m_columnDrawers.Remove(col);
				}
				return r;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// 描画
	void m_Draw(HDC dc)
	{
		CRect clientRect;
		_super::GetClientRect(&clientRect);
		// 状態チェックする
		m_isStateChecking = true;
		CBitmapImage orgImg;
		orgImg.SetFromClient(*this, clientRect, CLR_AUTOSELECT); //WM_PRINTCLIENT でカスタムドローを動かし、状態を拾う
		m_isStateChecking = false;
		// アイテム取得オブジェクト用意
		HDITEM hdi;
		enum { sizeOfBuffer = 256 };
		TCHAR lpBuffer[sizeOfBuffer];
		hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = sizeOfBuffer;
		// 各値用意
		CPoint pushOffset;
		m_pPartsDraw->GetPushedOffset(pushOffset);
		CPoint edgeOffset;
		m_pPartsDraw->GetEdgeOffset(edgeOffset);
		int right = clientRect.right;
		CRect parentRect;
		bool isEnabled = !! _super::IsWindowEnabled();
		CWnd* pParent = _super::GetParent();
		if ( pParent != NULL )
		{
			pParent->GetClientRect(&parentRect);
			right = parentRect.right;
			isEnabled = !! pParent->IsWindowEnabled();
		}
		// 幅自動調整
		if ( m_isAutoAdjustWidth )
		{
			CRect rc = clientRect;
			int cnt = _super::GetItemCount();
			for ( int i = 0; i < cnt; i++ )
			{
				if ( ! _super::GetItem(i, &hdi) )
				{
					continue;
				}
				rc.right = rc.left + hdi.cxy;
				bool r = (rc.right > right);
				if ( i + 1 == cnt )
				{
					r = (rc.right != right);
				}
				if ( r )
				{
					HDITEM h;
					h.mask = HDI_WIDTH;
					h.cxy = right - rc.left;
					if ( h.cxy < 0 )
					{
						h.cxy = 0;
					}
					_super::SetItem(i, &h);
					rc.right = right;
				}
				rc.left = rc.right;
			}
		}
		// 各アイテム描画
		CRect rc = clientRect;
		loop ( i, _super::GetItemCount() )
		{
			if ( ! _super::GetItem(ToInt(i), &hdi) )
			{
				continue;
			}
			rc.right = rc.left + hdi.cxy;
			//
			CPoint offset = edgeOffset;
			UINT state = DFCS_BUTTONPUSH | DFCS_ADJUSTRECT;
			if ( (m_columnStates[i] & CDIS_SELECTED) != 0 )
			{
				state |= DFCS_PUSHED;
				offset += pushOffset;
			}
			if ( ! isEnabled )
			{
				state |= DFCS_INACTIVE;
			}
			if ( (GetStyle() & HDS_BUTTONS) == 0 )
			{
				state |= DFCS_FLAT;
			}
			CRect r = rc;
			::FillRect(dc, &r, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
			m_pPartsDraw->DrawButtonControl(dc, &r, state);
			//
			HRGN hRgn = ::CreateRectRgnIndirect(&r);
			::SelectClipRgn(dc, hRgn);
			CSize clientSize = rc.Size() - edgeOffset - edgeOffset;
			IDrawable* pDr = m_columnDrawers[i];
			if ( pDr != NULL )
			{
				pDr->Resize(clientSize);
				pDr->Draw(dc, rc.left + offset.x, rc.top + offset.y);
			}
			else
			{
				CFont* pFont = _super::GetFont();
				UINT style = DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS;
				if ( (hdi.fmt & HDF_CENTER) != 0 )
				{
					style |= DT_CENTER;
				}
				else if ( (hdi.fmt & HDF_RIGHT) != 0 )
				{
					style |= DT_RIGHT;
				}
				COLORREF c = isEnabled ? m_textColor : CLR_INVALID;
				CTextDrawer d(clientSize, *pFont, c, lpBuffer, style);
				d.Draw(dc, rc.left + offset.x, rc.top + offset.y);
			}
			rc.left = rc.right;
			::SelectClipRgn(dc, NULL);
			_DeleteObject(hRgn);
		}
		// 余白描画
		if ( rc.right < right )
		{
			rc.right = right;
			if ( IS_RGBVALUE(m_backColor) )
			{
				if ( m_backBrush.GetSafeHandle() == NULL )
				{
					m_backBrush.CreateSolidBrush(m_backColor);
				}
				::FillRect(dc, rc, m_backBrush);
			}
			else
			{
				UINT state = DFCS_BUTTONPUSH;
				if ( ! _super::IsWindowEnabled() )
				{
					state |= DFCS_INACTIVE;
				}
				rc.right += 10; // 右の片を表示させないため
				m_pPartsDraw->DrawButtonControl(dc, &rc, state);
			}
		}
	}

	const IPartsDrawable*			m_pPartsDraw;			///< カラムヘッダ描画
	COLORREF						m_backColor;			///< 背景色
	CBrush							m_backBrush;			///< 背景ブラシ
	COLORREF						m_textColor;			///< 文字色
	CAutoVectorT<IDrawable::Ptr>	m_columnDrawers;		///< 描画情報の配列
	CAutoVectorT<UINT>				m_columnStates;			///< 状態情報の配列
	CStandardPartsDrawer			m_standardParts;		///< 標準パーツ描画
	bool							m_isStateChecking;		///< 状態チェック中
	bool							m_isAutoAdjustWidth;	///< 幅自動調節モード
	bool							m_isEnableChangeWidth;	///< 幅変更可能モード 
};



}; // MFC
}; // TNB
