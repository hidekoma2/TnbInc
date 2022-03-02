#pragma once
/**
 *@file
 * 自由型ビットマップボタン関係のヘッダ
 *
 *		ビットマップの形状にあわせたグラフィカルなボタンを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcAbstractButton.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL BITMAP
 * 自由型ビットマップボタンコントロール
 *
 *		ビットマップの形に合わせたボタンを作成することが出来ます。
 *
 *		ボタンに使用するベースビットマップとなる「通常状態」を必ず指定する必要があります
 *		（この「通常状態」のビットマップの形状が、ボタンの形状になります）。
 *		このビットマップは、 SetBitmap() で指定するか、
 *		リソースエディタでピクチャーコントロールのプロパティで、タイプに「ビットマップ」を選択し、
 *		イメージでビットマップの選択をしておきます(後者の場合リソースにビットマップをおく必要あり)。
 * 
 *		他に、「無効状態」「押下状態」「ホバー状態」とフォーカスのビットマップを指定することが
 *		出来ます。 
 *		フォーカスの表現方法は２種類あり、一つは「フォーカス状態」として、他の状態と同じように表示する方法、
 *		もう一つは、「他のボタンの上にオーバーラップして表示」する方法です。前者は SetFocusBitmap() 、
 *		後者は SetFocusMarkBitmap() で指定します（後で設定したほうが適用されます）。		
 * 
 *	@note	ボタンの形状は、「通常状態」のビットマップの形状で作成されます。そのため、他の状態
 *			のビットマップが「通常状態」と大きさが異なる場合、全部表示されない、あるいはごみが
 *			残る可能性があります。
 *
 *	@note	SetValidRgn() を使用することで、四角いビットマップでも、ボタンの範囲を自由型にすることも
 *			出来ます。
 *
 *	@note 有効、無効、表示、非表示は、 MFC の CButton と同じように操作できます。
 *
 *	@par使い方
 *		リソースエディタでボタンかスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton or CStatic）を本クラスに置き換えて使用します。
 *
 *	@see CAbstractButton
 *
 *	@par必要ファイル
 *			TnbMfcRgnButton.h
 * 
 *	@date 07/09/26 新規作成
 *	@date 07/10/10 アタッチ後のビットマップの変更を可能にした。
 *	@date 07/10/11 押下時のフォーカスマーク表示OFFSETを指定できるようにした。
 *	@date 07/10/16 フォーカス表示方法を二種類用意した。
 *	@date 07/10/22 残像モードを用意。
 *	@date 07/10/24 SetValidRgn() 追加。
 *	@date 07/10/26 コピーオペレータ、対応。
 *	@date 07/11/08 SetDefaultButtonStyle() 追加。
 *	@date 08/01/17 WM_SETTEXT でもショートカットを設定できるように対応。
 *	@date 08/02/06 SetDefaultButtonStyle() を親に移動。
 */
class CRgnButton : public CAbstractButton
{
	DEFSUPER(CAbstractButton);
	CSize				m_size;				///< ノーマルビットマップの大きさ
	COLORREF			m_transColor;		///< 透過色
	HRGN				m_hRgnClip;			///< ビットマップのクリップリージョン情報。
	HRGN				m_hRgnCtrl;			///< ビットマップのコントロールリージョン情報。
	bool				m_boIsDoSetRgn;		///< true の時、SetWindowRgnする（初期化用）
	CBitmapImage		m_lastBitmap;		///< 最後に書いたビットマップ。
	UINT				m_bitmapChangeTime;	///< ビットマップの書き換え時間。
	CPoint				m_pushedOffset;		///< 押下時のフォーカスマークの移動値。
	enum { TIMERID_DRAW = 1 };
	/// HRGN 削除
	void m_DeleteRgn(HRGN& _hRgn)
	{
		if ( _hRgn != NULL )
		{
			_DeleteObject(_hRgn);
			_hRgn = NULL;
		}
	}
	void m_DeleteRgns(void)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			_super::SetWindowRgn(NULL, false);
		}
		m_DeleteRgn(m_hRgnClip);
		m_DeleteRgn(m_hRgnCtrl);
	}
	void m_SetRgn(HRGN& _hRgn, HRGN h)
	{
		m_DeleteRgn(_hRgn);
		_hRgn = ::CreateRectRgn(0, 0, 1, 1);
		::CombineRgn(_hRgn, h, NULL, RGN_COPY);
	}
	/// 設定
	bool m_SetBaseBitmap(CBitmapHandle h, COLORREF color)
	{
		if ( h.GetSize(m_size) )
		{
			m_DeleteRgns();
			CBitmapImage bi;
			bi.Attach(h);
			m_hRgnClip = bi.CreateRgn(color);
			m_SetRgn(m_hRgnCtrl, m_hRgnClip);
			m_boIsDoSetRgn = true;
			_super::SetWindowPos(NULL, 0, 0, m_size.cx, m_size.cy, SWP_NOZORDER | SWP_NOMOVE);
			_super::SetWindowRgn(NULL, true);
			return true;
		}
		return false;
	}
	/// 
	void m_DrawBmp(CDC* pDC, CBitmapHandle hb)
	{
		if ( hb == NULL ) { return; }
		CPoint pos1(0, 0);
		CPoint pos2((_super::GetButtonState() == ES_Pushed) ? m_pushedOffset : pos1);
		OnDrawingButton(pos2, pDC, true);
		MFCLIB::SelectClipRgnOrg(pDC, m_hRgnClip);
//		pDC->DrawState(pos1, m_size, hb, DST_BITMAP);
		hb.Draw(*pDC, pos1.x, pos1.y);
		OnDrawingButton(pos2, pDC, false);
		if ( m_focusMaskColor != CLR_INVALID )
		{
			if ( _super::HasFocus() && ! m_focusBitmap.IsEmpty() )
			{
				const SIZE& s = m_focusBitmap.GetSize();
				HDC hdc = m_focusBitmap.GetDC();
				::TransparentBlt(*pDC, pos2.x, pos2.y, s.cx, s.cy, hdc, 0, 0, s.cx, s.cy, m_focusMaskColor);
				m_focusBitmap.ReleaseDC();
			}
		}
	}

protected:

	CBitmapHandle		m_bmpButtons[4];	///< 「通常」「ホバー」「押下」[無効」状態のBITMAP.
	CBitmapImage		m_focusBitmap;		///< フォーカス(マーク)BITMAP
	COLORREF			m_focusMaskColor;	///< フォーカスマークの透過色(-1ならフォーカスはマークではなく直書き)

	/**
	 * [通知] 描画中.
	 *		Bitmap表示の前後にコールされます。
	 *	@param[in,out] _pos 表示位置。
	 *	@param[in] pDC 描画先のCDC。
	 *	@param[in] boIsFirst Bitmap表示前は true、表示後は false で通知されます。
	 */
	virtual void OnDrawingButton(CPoint& _pos, CDC* pDC, bool boIsFirst)
	{
	}

	/**
	 * [通知] 描画
	 *	@param pDC 描画先のCDC。
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		CBitmapHandle hb = m_bmpButtons[_super::GetButtonState()];
		if ( hb.IsNull() )
		{
			hb = m_bmpButtons[ES_Normal];
		}
		if ( m_focusMaskColor == CLR_INVALID )
		{
			//フォーカスは、イメージ
			if ( _super::GetButtonState() == ES_Normal && _super::HasFocus() && ! m_focusBitmap.IsEmpty() )
			{
				hb = m_focusBitmap.GetBitmapHandle();
			}
		}
		_super::KillTimer(TIMERID_DRAW);
		if ( m_lastBitmap.IsEmpty() )
		{
			m_lastBitmap.Attach(hb);
			m_DrawBmp(pDC, hb);
		}
		else if ( m_bitmapChangeTime == 0 || hb.IsNull() )
		{
			m_DrawBmp(pDC, hb);
		}
		else
		{
			CBitmapImage bi;
			VERIFYLIB( bi.Set(hb) );
//			VERIFYLIB( bi.InsertOnSemitransparect(0, 0, m_lastBitmap, 50) );//50%合成
			VERIFYLIB( bi.InsertOnAlphaBlend(0, 0, m_lastBitmap, 50) );//50%合成
			m_DrawBmp(pDC, bi.GetBitmapHandle());
			m_lastBitmap.Attach(hb);
			_super::SetTimer(TIMERID_DRAW, m_bitmapChangeTime, NULL);
		}
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
		if ( message == WM_PAINT )
		{
			if ( m_boIsDoSetRgn )
			{
				m_boIsDoSetRgn = false;
				_super::SetWindowRgn(m_hRgnCtrl, true);
				m_hRgnCtrl = NULL;	// 開放はWinが行う
				_super::RedrawWindow();
				return 0;
			}
		}
		else if ( message == WM_SHOWWINDOW )
		{
			m_lastBitmap.Empty();
		}
		else if ( message == WM_DESTROY )
		{
			m_DeleteRgns();
		}
		else if ( message == WM_SETTEXT )
		{
			CString s = reinterpret_cast<LPCTSTR>(lParam);
			int f = s.Find('&');
			if ( f >= 0 && (f + 1) < s.GetLength() )
			{
				SetShortcutKey(s[f + 1]);
			}
		}
		else if ( message == WM_TIMER && wParam == TIMERID_DRAW )
		{
			KillTimer(TIMERID_DRAW);
			CClientDC dc(this);
			m_DrawBmp(&dc, m_lastBitmap.GetBitmapHandle());
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
		HBITMAP hBase = reinterpret_cast<HBITMAP>(::SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0L));// GetBitmap()
		if ( ! m_bmpButtons[ES_Normal].IsNull() )
		{
			if ( ! m_SetBaseBitmap(m_bmpButtons[ES_Normal], m_transColor) )
			{
				m_bmpButtons[ES_Normal].Null();
			}
		}
		if ( m_bmpButtons[ES_Normal].IsNull() )
		{
			CRect rect;
	 		_super::GetClientRect(rect);
			if ( hBase == NULL )
			{
				HDC dc = ::GetDC(NULL);
				hBase = ::CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
				::ReleaseDC(NULL, dc);
				m_bmpButtons[ES_Normal] = hBase;
			}
			else
			{
				m_bmpButtons[ES_Normal].SetClone(hBase);
			}
			VERIFYLIB( m_SetBaseBitmap(m_bmpButtons[ES_Normal], m_transColor) );
		}
		_super::PreSubclassWindow();
	}

public:

	/// デストラクタ
	virtual ~CRgnButton(void)
	{
		m_DeleteRgns();
	}

	/// コンストラクタ
	CRgnButton(void) 
		: m_boIsDoSetRgn(false), m_hRgnClip(NULL), m_hRgnCtrl(NULL), m_pushedOffset(0, 0)
		, m_transColor(CLR_AUTOSELECT), m_focusMaskColor(CLR_INVALID), m_bitmapChangeTime(40)
	{
	}

	/**
	 * [複製] 情報コピー.
	 *		保持している情報を複製します。
	 *	@note 文字列情報はコピーされません。
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CRgnButton& operator=(const CRgnButton& other)
	{
		_super::operator=(other);
		m_size = other.m_size;
		m_transColor = other.m_transColor;
		m_boIsDoSetRgn = other.m_boIsDoSetRgn;
		m_lastBitmap = other.m_lastBitmap;
		m_bitmapChangeTime = other.m_bitmapChangeTime;
		m_pushedOffset = other.m_pushedOffset;
		m_focusBitmap = other.m_focusBitmap;
		m_focusMaskColor = other.m_focusMaskColor;
		loop ( i, 4 )
		{
			m_bmpButtons[i] = other.m_bmpButtons[i];
		}
		m_SetRgn(m_hRgnClip, other.m_hRgnClip);
		if ( ! ::IsWindow(other) )
		{
			m_SetRgn(m_hRgnCtrl, other.m_hRgnCtrl);
		}
		else
		{
			other.GetWindowRgn(m_hRgnCtrl);
		}
		//
		if ( ::IsWindow(m_hWnd) ) 
		{
			m_SetBaseBitmap(m_bmpButtons[ES_Normal], m_transColor);
		}
		return *this;
	}

	/**
	 * [設定] ビットマップ切り替え時間設定.
	 *		状態が変わってビットマップを変更する時、デフォルトでは、切り替え前のビットマップと、
	 *		切り替え後のビットマップの50％合成の画像を 30ms 秒表示するようになっています（そのため
	 *		ふわっと切り替わるように見えます）。この切り替え時間を本メソッドで指定することが出来ます。
	 *	@note 合成画像は一枚しか用意しないため、100msぐらいが上限です。
	 *	@param ms 切り替え時間（単位 ms）。
	 */
	void SetBitmapChangeTime(UINT ms)
	{
		m_bitmapChangeTime = ms;
	}

	/**
	 * [設定] ベースビットマップ指定.
	 *		ボタンのベースとなるビットマップを指定します。このビットマップでボタンの形が確定されます。
	 *	@note 本メソッドを使わない場合、リソースエディタで張ったビットマップをベースにします。
	 *	@param bmp ビットマップ。
	 *	@param color 透過色指定。省略すると、ビットマップの左下の色が透過色とします。
	 *						CLR_INVALID を指定すると透過色なしになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。ハンドルが無効です。
	 */
	bool SetBitmap(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		if ( bmp.IsNull() ) { return false; }
		m_bmpButtons[ES_Normal] = bmp;
		m_transColor = color;
		if ( ::IsWindow(m_hWnd) ) 
		{
			if ( ! m_SetBaseBitmap(bmp, color) )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [設定] 押下状態ビットマップ指定.
	 *		押下状態を表すビットマップを指定します。
	 *		ベースとなるビットマップと同じ大きさにする必要があります。
	 *	@param bmp ビットマップ。
	 */
	void SetPushedBitmap(CBitmapHandle bmp)
	{
		m_bmpButtons[ES_Pushed] = bmp;
	}

	/**
	 * [設定] 無効状態ビットマップ指定.
	 *		無効状態を表すビットマップを指定します。
	 *		ベースとなるビットマップと同じ大きさにする必要があります。
	 *	@param bmp ビットマップ。
	 */
	void SetDisableBitmap(CBitmapHandle bmp)
	{
		m_bmpButtons[ES_Disable] = bmp;
	}

	/**
	 * [設定] ホバー状態ビットマップ指定.
	 *		ホバー状態を表すビットマップを指定します。
	 *		ベースとなるビットマップと同じ大きさにする必要があります。
	 *	@param bmp ビットマップ。
	 */
	void SetHoverBitmap(CBitmapHandle bmp)
	{
		m_bmpButtons[ES_Hover] = bmp;
	}

	/**
	 * [設定] フォーカス状態ビットマップ指定.
	 *		フォーカス状態を表すビットマップを指定します。
	 *		ベースとなるビットマップと同じ大きさにする必要があります。
	 *	@note SetFocusMaskBitmap() を使うと本メソッドの設定は破棄されます。
	 *	@param bmp ビットマップ。
	 */
	void SetFocusBitmap(CBitmapHandle bmp)
	{
		m_focusBitmap.Set(bmp);
		m_focusMaskColor = CLR_INVALID;
	}

	/**
	 * [設定] フォーカス状態マークビットマップ指定.
	 *		フォーカス状態を表すマークのビットマップを指定します。
	 *		他のビットマップの上に表示されるものになります。
	 *		SetPushedMarkOffset() の設定の影響を受けます。
	 *	@note SetFocusBitmap() を使うと本メソッドの設定は破棄されます。
	 *	@param bmp ビットマップハンドル。ハンドルは不必要になったら自動的に破棄します。
	 *	@param color 透過色指定。省略すると、ビットマップの左下の色を透過色とします。
	 */
	void SetFocusMarkBitmap(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		m_focusBitmap.Set(bmp);
		if ( ! IS_RGBVALUE(color) )
		{
			const SIZE& s = m_focusBitmap.GetSize();
			HDC dc = m_focusBitmap.GetDC();
			color = ::GetPixel(dc, 0, s.cy - 1);
			m_focusBitmap.ReleaseDC();
		}
		m_focusMaskColor = color;
	}

	/**
	 * [設定] 押下時表示ずれ設定.
	 *		押下時にフォーカスマークのずれを指定することが出来ます。デフォルトでは、移動なしです（0,0）。
	 *	@param pos ずれ
	 */
	void SetPushedMarkOffset(const POINT& pos)
	{
		m_pushedOffset = pos;
	}

	/**
	 * [設定] ビットマップ指定.
	 *		「通常状態」「ホバー状態」「押下状態」[無効状態」「フォーカス状態」を表すビットマップを同時に指定します。
	 *	@note 「通常状態」「ホバー状態」「押下状態」「無効状態」「フォーカス状態」の絵を同じ大きさで横に並べたビットマップを
	 *			指定することで、全部いっぺんに設定することが可能です。
	 *	@param bmp ビットマップ。
	 *	@param color 透過色指定。省略すると、ビットマップの左下の色を透過色とします。
	 *						CLR_INVALID を指定すると透過色なしになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。ハンドルが無効です。
	 */
	bool SetAllBitmaps(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		CBitmapImage baseImage;
		if ( baseImage.Set(bmp) )
		{
			CSize baseSize = baseImage.GetSize();
			bool r = true;
			int x = 0;
			int l = baseSize.cx / 5;
			CBitmapImage bi;
			for ( int i = 0; i < 5; i++ )
			{
				bi = baseImage.Cut(CRect(x, 0, x + l, baseSize.cy));
				if ( i == 0 )
				{
					r &= SetBitmap(bi, color);
				}
				else if ( i == 4 )
				{
					SetFocusBitmap(bi);
				}
				else
				{
					m_bmpButtons[i] = bi;
				}
				x += l;
			}
			return r;
		}
		return false;
	}

	/**
	 * [設定] ボタン有効範囲指定.
	 *		ボタンの押下処理の行われる範囲を指定することが出来ます。描画は四角くなります。
	 *	@note 使用する場合は、ビットマップ指定の最後に使用してください。
	 *	@param h リージョンハンドル。 ハンドルは内部で複製しますので、破棄してかまいません。
	 */
	void SetValidRgn(HRGN h)
	{
		m_DeleteRgn(m_hRgnClip);
		m_SetRgn(m_hRgnCtrl, h);
		m_boIsDoSetRgn = true;
		_super::RedrawWindow();
	}
};



}; //MFC
}; //TNB



