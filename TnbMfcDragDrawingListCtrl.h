#pragma once
/**
 *@file
 * ドラッグ付き描画情報ListCtrl関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcDrawingListCtrl.h"
#include "TnbMfcDrag.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * ドラッグ付き描画情報ListCtrlコントロール
 *
 *		行単位で表示する描画情報を指定出来ます。
 *
 *	@par使い方
 *		リソースエディタでリストコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListCtrl）を本クラスに置き換えて使用します。\n
 *		リストコントロールのプロパティは「オーナー描画(O)」を「可変」にしておいてください。
 *
 *	@par必要ファイル
 *			TnbMfcDragDrawingListCtrl.h
 * 
 *	@date 07/12/07 新規作成
 *	@date 08/01/16 ドラッグによるスクロール速度を調整。
 */
class CDragDrawingListCtrl : public CDragAddinT<CDrawingListCtrl>
{
	DEFSUPER(CDragAddinT<CDrawingListCtrl>);
	int		m_itemHeight;		///< アイテムの高さ
	int		m_headerHeight;		///< ヘッダーの高さ
	int		m_ctrlId;			///< 仮想コントロールID
	int		m_scrollCount;		///< ドラッグスクロールカウンタ
	/// アイテムの高さ取得
	int GetItemHeight(void)
	{
		if ( m_itemHeight < 0 )
		{
			CRect rc;
			GetItemRect(0, &rc, LVIR_BOUNDS);
			m_itemHeight = rc.Height();
		}
		return m_itemHeight;
	}
	/// ヘッダーの高さ取得
	int GetHeaderHeight(void)
	{
		if ( m_headerHeight < 0 )
		{
			CRect rc;
			GetHeaderCtrl()->GetClientRect(rc);
			m_headerHeight = rc.Height();
		}
		return m_headerHeight;
	}

protected:

	/**
	 * [取得] 仮想コントロールIDベース取得.
	 *	@return ベース。
	 */
	int GetVirtualCtrlID(void) const
	{
		return m_ctrlId;
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
		if ( message == WM_LBUTTONDOWN )
		{
			CPoint po;
			::GetCursorPos(&po);
			ScreenToClient(&po);
			if ( _super::HitTest(po, NULL) >= 0 )
			{
				CListCtrl::WindowProc(message, wParam, lParam);
			}
		}
		else if ( IsNofityMessage(message) && wParam == static_cast<WPARAM>(MAKELONG(CANDROP, GetDragGroupID())) )
		{
			if ( ! _super::CanDropping() )
			{
				return false;
			}
			TDropParam* P = reinterpret_cast<TDropParam*>(lParam);	//クライアント座標が格納されている
			if ( P->point.y < GetHeaderHeight() + 2 )
			{
				return false;
			}
			int h = GetItemHeight();
			CRect rc;
			_super::GetClientRect(&rc);
			LONG scrollHeight = 0;
			if ( P->point.y < GetHeaderHeight() + 2 + h )
			{
				if ( _super::GetTopIndex() > 0 && ++m_scrollCount > 4 )
				{
					scrollHeight = -h;
				}
			}
			else if ( rc.bottom - h < P->point.y )
			{
				int i1 = _super::GetItemCount() - _super::GetTopIndex();
				int i2 = _super::GetCountPerPage();
				if ( i1 > i2 && ++m_scrollCount > 4 )
				{
					scrollHeight = h;
				}
			}
			if ( scrollHeight != 0 )
			{
				m_scrollCount = 0;
				_super::Scroll(CSize(0, scrollHeight));
				_super::Invalidate(false);
			}
			int r = _super::HitTest(P->point, NULL);
			if ( r >= 0 )
			{
				RECT rc;
				_super::GetItemRect(r, &rc, LVIR_BOUNDS);
				_DeleteObject(P->hRgn);
				P->hRgn = ::CreateRectRgnIndirect(&rc);
				ASSERT1( m_ctrlId + r < 0x10000, "CDrawingListCtrlDg", "仮想コントロールIDが WORD で表現できない値になってしまいました(%u)。", m_ctrlId + r );
				P->ctrlID = static_cast<WORD>(m_ctrlId + r);
				return true;
			}
			return false;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] ドラッグ開始.
	 *	@param[in,out] _rc アイテム範囲。inで ::GetWindowRect() した範囲が入っています（Window座標）。
	 *	@param[in] po マウスのポジション（Window座標）。
	 *	@retval 0 ドラック出来ない.
	 *	@retval 1以上 コントロールID
	 */
	virtual int OnBeginDrag(RECT& _rc, const POINT& po)
	{
		CPoint p = po;
		ScreenToClient(&p);
		int r = HitTest(p, NULL);
		if ( r >= 0 )
		{
			if ( ! ::DragDetect(m_hWnd, p) )
			{
				return 0;
			}
//			UINT s = LVIS_SELECTED | LVIS_FOCUSED;
//			SetItemState(r, s, s);
			GetItemRect(r, &_rc, LVIR_BOUNDS);
			ClientToScreen(&_rc);
			//void SetDraggingBitmap(CBitmapHandle bmp)
			return m_ctrlId + r;
		}
		return 0;
	}	


public:

	/// コンストラクタ
	CDragDrawingListCtrl(void) 
		: _super(), m_ctrlId(0x8000), m_itemHeight(-1), m_headerHeight(-1), m_scrollCount(0)
	{
	}

	/// デストラクタ
	~CDragDrawingListCtrl(void)
	{
	}

	/**
	 * [設定] 仮想コントロールIDベース設定.
	 *	@note 実際のコントロールIDではなく、仮想的にコントロールIDを使い、
	 *			各Itemの識別をするようにします。
	 *			ベース＋リストに含まれる数が WORD で表現できる範囲を
	 *			超えないようにしてください。
	 *	@note デフォルトでは、 0x8000 になっています。
	 *	@param base ベース。
	 */
	void SetVirtualCtrlID(int base)
	{
		m_ctrlId = base;
	}
};



}; // MFC
}; // TNB
