#pragma once
/**
 *@file
 * DIPSW 風ボタンコントロール関係のヘッダ
 *
 *		4個のswが並ぶコントロールを作成するためのフレームを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcDrawingStatic.h"
#include "TnbBitmapImage.h"



#define DIPSW_CHANGED_T			0x100



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * DIPSW 風ボタンコントロールクラス.
 *
 *		スライドスイッチを4個持ち、0x00～0x0F までの値を表すことが出来ます。
 *
 *	@par使い方
 *		マジックカラーコードでボタンを現したビットマップを一枚用意します。
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *		最初に SetImage() してください。
 *
 *	@note クリックにより DIPSW の状態が変化した時、 DIPSW_CHANGED_T を、親に通知しています。
 *
 *	@par使用例
 *		\code
 *		//= 宣言
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_CONTROL(DIPSW_CHANGED_T, IDC_STATIC_DIPSW, OnChangedDipsw)
 *			:
 *		\endcode
 *
 *	@todo フォーカスを受けません。よってキーボードでの操作は出来ません。
 *		
 *	@par必要ファイル
 *			TnbMfcDipswButton.h
 * 
 *	@date 16/07/26 新規作成
 */
class CDipswButton : public CDrawingStatic
{
	DEFSUPER(CDrawingStatic);
public:

	/**
	 * マジックカラーコード.
	 */
	enum EMagicColorCode
	{
		MCC0 = RGB(128, 0, 0),		///< 暗い赤
		MCC1 = RGB(255, 0, 0),		///< 赤
		MCC2 = RGB(0, 0, 128),		///< 暗い青
		MCC3 = RGB(0, 0, 255),		///< 青
		MCC4 = RGB(0, 128, 0),		///< 暗い緑
		MCC5 = RGB(0, 255, 0),		///< 緑
		MCC6 = RGB(0, 128, 128),	///< 暗い水色
		MCC7 = RGB(0, 255, 255),	///< 水色
	};

	/// コンストラクタ
	CDipswButton(void) : m_value(0x00)
	{
	}

	/**
	 * [設定] イメージ指定.
	 *	@param b ベース画像。swの部分がマジックカラーコードで書かれているもの。
	 *	@param onColor ONの際の色。マジックカラーコード以外の色にすること。
	 *	@param offColor OFFの際の色。マジックカラーコード以外の色にすること。
	 */
	void SetImage(CBitmapHandle b, COLORREF onColor = RGB(255, 255, 255), COLORREF offColor = RGB(1, 1, 1))
	{
		m_onColor = onColor;
		m_offColor = offColor;
		m_base = b;
		m_base.ToHaveRgbData();
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
			{
				CPoint po(lParam);
				int z = m_CheckPoint(po);
				if ( z >= 0 )
				{
					BYTE b = m_value;
					m_value ^= _BIT(z);
					if ( b != m_value )
					{
						m_ResetBitmap();
						MFCLIB::SendCommandMessage(this, DIPSW_CHANGED_T);
					}
				}
			}
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// チェックポイント @param po チェックするクライアント座標。
	int m_CheckPoint(CPoint po)
	{
		CSize sz = m_base.GetSize();
		if ( sz.cy <= po.y || sz.cx <= po.x )
		{
			return -1;
		}
		const RGBQUAD* pRgb = m_base.GetRgbDataPtr(po.y);
		RGBQUAD q = pRgb[po.x];
		switch ( RGB(q.rgbRed, q.rgbGreen, q.rgbBlue) )
		{
		case MCC0:
		case MCC1:
			return 0;
		case MCC2:
		case MCC3:
			return 1;
		case MCC4:
		case MCC5:
			return 2;
		case MCC6:
		case MCC7:
			return 3;
		default:
			break;
		}
		return -1;
	}

	/// 画像再構築
	void m_ResetBitmap(void)
	{
		CBitmapImage bi;
		bi.Set(m_base);
		COLORREF on = m_onColor;
		COLORREF of = m_offColor;
		bi.ChangePixelColor(MCC0, (m_value & _BIT(0)) ? on : of);
		bi.ChangePixelColor(MCC1, (m_value & _BIT(0)) ? of : on);
		bi.ChangePixelColor(MCC2, (m_value & _BIT(1)) ? on : of);
		bi.ChangePixelColor(MCC3, (m_value & _BIT(1)) ? of : on);
		bi.ChangePixelColor(MCC4, (m_value & _BIT(2)) ? on : of);
		bi.ChangePixelColor(MCC5, (m_value & _BIT(2)) ? of : on);
		bi.ChangePixelColor(MCC6, (m_value & _BIT(3)) ? on : of);
		bi.ChangePixelColor(MCC7, (m_value & _BIT(3)) ? of : on);
		_super::SetDrawingBitmap(bi, true, true);
	}

	CBitmapImage	m_base;			///< 元絵. @note マジックカラーコードで書かれていること。
	COLORREF		m_onColor;		///< ONの際の色. @note マジックカラーコード以外の色にすること
	COLORREF		m_offColor;		///< OFFの際の色. @note マジックカラーコード以外の色にすること
	BYTE			m_value;		///< 値	
};



}; //MFC
}; //TNB



