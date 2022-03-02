#pragma once
/**
 *@file
 * 描画情報表示関係のヘッダ
 *
 *		描画情報を表示するためのコントロールを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapDrawer.h"
#include "TnbMfcRgnControl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報表示コントロールクラス
 *
 *		描画情報を表示するコントロールです。
 *
 *	@par使い方
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingStatic.h
 * 
 *	@date 07/11/14 新規作成
 *	@date 07/12/19 Add 系メソッドに isAdjust 引数、追加。
 *	@date 09/07/24 SetDrawingObject() の動きを少し調整。
 *	@date 10/04/05 構成改善。再描画フラグを追加。
 *	@date 11/03/15 サブクラス化時に元クラスが画像を持っていたら破棄するように修正。
 *	@date 14/04/07 WM_SIZE に対応。
 */
class CDrawingStatic : public CRgnControlAddinT<CWnd>
{
	DEFSUPER(CRgnControlAddinT<CWnd>);
public:

	/// デストラクタ
	virtual ~CDrawingStatic(void)
	{
	}

	/// コンストラクタ
	CDrawingStatic(void) : _super()
	{
	}

	/// 代入コンストラクタ
	CDrawingStatic(const IDrawable& draw) : _super()
	{
		m_pDrawing = draw.Clone();
	}

	/**
	 * コピーオペレータ.
	 *	@param other コピー元。
	 *	@return 自分の参照
	 */
	CDrawingStatic& operator=(const CDrawingStatic& other)
	{
		if ( ! other.m_pDrawing.IsNull() )
		{
			m_pDrawing = other.m_pDrawing->Clone();
		}
		m_Resize();
		return *this;
	}

	/**
	 * [設定] 描画情報設定.
	 *	@param draw 描画情報. 本オブジェクトにはクローンが記憶されるので、 draw は破棄してもかまいません。
	 *	@param isClientSizeAdjust true か省略なら 指定の draw の大きさにクライアントを調整します。
	 *					false ならクライアントの大きさを draw に設定します（クライアントの大きさは変わりません）。
	 *	@param isRedraw true なら再描画します。 false か省略ならなにもしません。
	 */
	void SetDrawingObject(const IDrawable& draw, bool isClientSizeAdjust = true, bool isRedraw = false)
	{
		m_pDrawing = draw.Clone();
		if ( isClientSizeAdjust )
		{
			m_Resize();
		}
		else if ( m_pDrawing != NULL )
		{
			CRect rc;
			_super::GetClientRect(&rc);
			m_pDrawing->Resize(rc.Size());
		}
		if ( isRedraw )
		{
			CClientDC dc(this);
			m_Draw(&dc);
		}
	}

	/**
	 * [設定] 描画情報設定.
	 *	@param bmp ビットマップ
	 *	@param isAdjust true か省略なら 指定の bmp の大きさにクライアントを調整します。
	 *					false ならクライアントの大きさは変わりません。
	 *	@param isRedraw true なら再描画します。 false か省略ならなにもしません。
	 */
	void SetDrawingBitmap(CBitmapHandle bmp, bool isAdjust = true, bool isRedraw = false)
	{
		SetDrawingObject(CBitmapDrawer(bmp), isAdjust ,isRedraw);
	}

	/**
	 * [設定] クライアントリージョン設定.
	 *	@param color コントロール外にする色。省略すると、左下の色を対象色とします。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetClientRgnEx(COLORREF color = CLR_AUTOSELECT)
	{
		if ( m_pDrawing.IsNull() ) { return false; }
		if ( color == CLR_INVALID )
		{
			HRGN rgn = NULL;
			return !! SetClientRgn(rgn);
		}
		CBitmapImage bi(CBitmapDrawer::ToBitmap(*m_pDrawing, color));
		HRGN rgn = bi.CreateRgn(color);
		if ( ! SetClientRgn(rgn) )
		{
			_DeleteObject(rgn);
			return false;
		}
		return true;
	}

protected:

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
		case WM_ERASEBKGND:
			TTRACE0("WM_ERASEBKGND\n");
			return 0;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				m_Draw(pDC);
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // 描画用のデバイス コンテキスト
				m_Draw(&dc);
			}
			return 0;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			return 0;
		case WM_SIZE:
			if ( ! m_pDrawing.IsNull() )
			{
				CRect rc;
				_super::GetClientRect(&rc);
				m_pDrawing->Resize(rc.Size());
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
		HBITMAP hBmp = reinterpret_cast<HBITMAP>(_super::SendMessage(STM_GETIMAGE, IMAGE_BITMAP));
		if ( hBmp != NULL )
		{
			::DeleteObject(hBmp);
		}
		HICON hIcon = reinterpret_cast<HICON>(_super::SendMessage(STM_GETIMAGE, IMAGE_ICON));
		if ( hIcon != NULL )
		{
			::DeleteObject(hIcon);
		}
		_super::ModifyStyle(0, SS_OWNERDRAW);
		_super::PreSubclassWindow();
		m_Resize();
	}

private:
	IDrawable::Ptr	m_pDrawing;		///< 保持する描画情報
	/// リサイズ
	void m_Resize(void)
	{
		if ( m_pDrawing.IsNull() ) { return; }
		SIZE size;
		if ( m_pDrawing->GetSize(size) )
		{
			MFCLIB::ChangeClientSize(this, size.cx, size.cy);
		}
	}
	/// 描画.
	void m_Draw(CDC* pDC)
	{
		if ( ! m_pDrawing.IsNull() )
		{
			if ( m_clientRgn != NULL )
			{
				MFCLIB::SelectClipRgnOrg(pDC, m_clientRgn);
			}
			else
			{
#ifndef _WIN32_WCE
				//@ 100316 いらない？ CEではあるとNG
				CRect rc;
				_super::GetClientRect(&rc);
				HRGN rgn = ::CreateRectRgnIndirect(&rc);
				::ExtSelectClipRgn(*pDC, rgn, RGN_AND);
				_DeleteObject(rgn);
#endif
			}
			CWnd* pWnd = _super::GetParent();
			if ( pWnd != NULL )
			{
				pWnd->SendMessage(WM_CTLCOLORSTATIC,
					reinterpret_cast<WPARAM>(pDC->GetSafeHdc()), reinterpret_cast<LPARAM>(m_hWnd));
			}
			m_pDrawing->Draw(*pDC);
		}
	}
};



}; //MFC
}; //TNB



