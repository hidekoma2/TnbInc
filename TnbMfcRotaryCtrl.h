#pragma once
/**
 *@file
 * ロータリー風値コントロール関係のヘッダ
 *
 *		0～15までのロータリーコントロールを作成するためのフレームを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcDrawingStatic.h"
#include "TnbBitmapImage.h"
#include "TnbGdiPlus.h"



#define ROTARY_CHANGED_T			0x100



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ロータリー風値コントロールクラス.
 *
 *		ロータリーを持ち、0x00～0x0F までの値を表すことが出来ます。
 *
 *	@par使い方
 *		値の書かれたビットマップと、0の向きの矢印の書かれたビットマップを２枚用意します。
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *		最初に SetImage() してください。
 *
 *	@note クリックによりロータリーの状態が変化した時、 ROTARY_CHANGED_T を、親に通知しています。
 *
 *	@par使用例
 *		\code
 *		//= 宣言
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_CONTROL(ROTARY_CHANGED_T, IDC_STATIC_DIPSW, OnChangedDipsw)
 *			:
 *		\endcode
 *
 *	@todo フォーカスを受けません。よってキーボードでの操作は出来ません。
 *		
 *	@par必要ファイル
 *			TnbMfcRotaryCtrl.h
 * 
 *	@date 16/07/26 新規作成
 */
class CRotaryCtrl : public CDrawingStatic
{
	DEFSUPER(CDrawingStatic);
public:

	/// コンストラクタ
	CRotaryCtrl(void) : m_value(0x00)
	{
	}

	/**
	 * [設定] イメージ指定.
	 *	@param b ベース画像.
	 *	@param a 矢印画像.
	 *	@param transColor 矢印画像の透過色.
	 */
	void SetImage(CBitmapHandle b, CBitmapHandle a, COLORREF transColor = RGB(255, 0, 255))
	{
		m_baseImage = b;
		m_arrowImage = a;
		m_arrowTransColor = transColor;
		m_ResetBitmap();
	}

	/**
	 * [取得] 値取得.
	 *	@return 値。
	 */
	BYTE GetValue(void) const
	{
		return m_value;
	}

	/**
	 * [設定] 値インクリメント.
	 */
	void Increment(void)
	{
		SetValue(static_cast<BYTE>((m_value + 1) & 0xF));
	}

	/**
	 * [設定] 値デクリメント.
	 */
	void Decrement(void)
	{
		SetValue(static_cast<BYTE>((m_value - 1) & 0xF));
	}

	/**
	 * [設定] 値設定.
	 *	@param value 値。
	 */
	void SetValue(BYTE value)
	{
		m_value = value;
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			m_ResetBitmap();
		}
	}

protected:
	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		m_ResetBitmap();
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
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			m_value++;
			m_value &= 0xF;
			m_ResetBitmap();
			SetFocus();
			MFCLIB::SendCommandMessage(this, ROTARY_CHANGED_T);
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			m_value--;
			m_value &= 0xF;
			m_ResetBitmap();
			SetFocus();
			MFCLIB::SendCommandMessage(this, ROTARY_CHANGED_T);
			break;
		case WM_MOUSEWHEEL:
			if ( static_cast<short>(HIWORD(wParam)) > 0 )
			{
				m_value++;
			}
			else
			{
				m_value--;
			}
			m_value &= 0xF;
			m_ResetBitmap();
			SetFocus();
			MFCLIB::SendCommandMessage(this, ROTARY_CHANGED_T);
			break;
		case WM_GETDLGCODE:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				return (r & ~DLGC_STATIC);
			}
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// 画像再構築
	void m_ResetBitmap(void)
	{
		CBitmapImage base;
		base.Set(m_baseImage);
		if ( base.IsEmpty() )
		{
			return;
		}
		m_size = base.GetSize();
		CBitmapImage arrow;
		arrow.Set(m_size.cx, m_size.cy, m_arrowTransColor);
		{
			CBitmapHandle h = m_arrowImage;
			CGdiPlus g(arrow.GetDC());
			g.SetImageInterpolationMode(CGdiPlus::NEAREST_NEIGHBOR);
			g.DrawRotateImage(CGdiPlus::CPos(m_size.cx / 2, m_size.cy / 2), h, (360.0 / 16.0) * m_value);
		}
		arrow.ReleaseDC();
		base.InsertOnTransparent(0, 0, arrow);
		_super::SetDrawingBitmap(base, true, true);
	}

	CBitmapHandle	m_baseImage;		///< ベース画像
	CBitmapHandle	m_arrowImage;		///< 矢印画像
	COLORREF		m_arrowTransColor;	///< 透過色
	BYTE			m_value;			///< 値
	SIZE			m_size;				///< コントロールサイズ
};



}; //MFC
}; //TNB
