#pragma once
/**
 *@file
 * 立体ビットマップメーカー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapImage.h"
#include "TnbHandleHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup BITMAP
 * 立体ビットマップメーカークラス
 *
 *		一つの画像の周りを立体にし、ボタン等に使用できるようにします。
 *
 *	@note 元の画像より、上下に 3 dot 大きくなります( SetFaceColor() 参照)。
 *
 *	@par必要ファイル
 *			TnbSolidBitmapMaker.h
 *
 *	@date 09/11/10 新規作成
 */
class CSolidBitmapMaker
{
public:

	/// コンストラクタ
	CSolidBitmapMaker(void)
	{
		m_DefaultBrush();
		m_transColor = CLR_AUTOSELECT;
		m_baseColor = RGB(255, 0, 255);
	}

	/**
	 * [設定] 背景色設定.
	 *	@note 作成した画像の背景色を指定します。指定がない場合、 マゼンタ(RGB(255, 0, 255))になります。
	 *	@param c 背景色.
	 */
	void SetBaseColor(COLORREF c)
	{
		m_baseColor = c;
	}

	/**
	 * [設定] 透過色設定.
	 *	@note 元の画像の透過する色を指定します。指定がない場合、左下の色を透過色とします。
	 *	@param c 透過色.
	 */
	void SetTransColor(COLORREF c)
	{
		m_transColor = c;
	}

	/**
	 * [設定] フェイスカラー設定.
	 *	@note デフォルトでは、システムのボタンと同じ色になります。
	 *	\parフェイスカラー配置:
	 *	\code
	 *		オリジナル画像
	 *			
	 *			　＠＠＠
	 *			　＠＠＠
	 *			　＠＠＠
	 *
	 *		でっばりの場合
	 *			
	 *			＋＋＋＋＋＃	＋ ; c1 明るい色
	 *			＋＠＠＠＊＃	＊ ; c2 暗い色
	 *			＋＠＠＠＊＃	＃ ; c3 もっと暗い色
	 *			＋＠＠＠＊＃
	 *			＋＊＊＊＊＃
	 *			＃＃＃＃＃＃
	 *
	 *		へっこみの場合
	 *			
	 *			＃＃＃＃＃＃	＋ ; c1 明るい色
	 *			＃＊＊＊＊＋	＊ ; c2 暗い色
	 *			＃＊＠＠＠＋	＃ ; c3 もっと暗い色
	 *			＃＊＠＠＠＋
	 *			＃＊＠＠＠＋
	 *			＃＋＋＋＋＋
	 *	\endcode
	 *	@see SetDefaultFaceColor()
	 *	@param c1 明るい色
	 *	@param c2 暗い色
	 *	@param c3 もっと暗い色
	 */
	void SetFaceColor(COLORREF c1, COLORREF c2, COLORREF c3)
	{
		m_brush1 = ::CreateSolidBrush(c1);
		m_currentBrush1 = m_brush1;
		m_brush2 = ::CreateSolidBrush(c2);
		m_currentBrush2 = m_brush2;
		m_brush3 = ::CreateSolidBrush(c3);
		m_currentBrush3 = m_brush3;
	}

	/**
	 * [設定] フェイスカラー設定.
	 *	@note システムのボタンと同じ色にします（デフォルト）。
	 *	@see SetFaceColor()
	 */
	void SetDefaultFaceColor(void)
	{
		m_DefaultBrush();
		m_brush1.Null();
		m_brush2.Null();
		m_brush3.Null();
	}

	/**
	 * [作成] 立体ビットマップ作成.
	 *	@param bh 元画像.
	 *	@param isPressed 省略, false の場合でっばり、 true の場合、へっこみ。
	 *	@return 作成画像.
	 */
	CBitmapImage Make(CBitmapHandle bh, bool isPressed = false) const
	{
		CBitmapImage bi = bh;
		if ( ! isPressed )
		{
			return m_MakeButton(bi);
		}
		return m_MakeButtonRv(bi);
	}

private:

	/// 標準ブラシ
	void m_DefaultBrush(void)
	{
		m_currentBrush1 = ::GetSysColorBrush(COLOR_3DHILIGHT);
		m_currentBrush2 = ::GetSysColorBrush(COLOR_3DSHADOW);
		m_currentBrush3 = ::GetSysColorBrush(COLOR_3DDKSHADOW);
	}
	
	/// 画像作成
	CBitmapImage m_MakeButton(const CBitmapImage& bmp) const
	{
		CBitmapImage bi;
		bi.Set(bmp.GetSize().cx + 3, bmp.GetSize().cy + 3, m_baseColor);
		HRGN hRgn = bmp.CreateRgn(m_transColor);
		HBRUSH b1 = m_currentBrush1;
		HBRUSH b2 = m_currentBrush2;
		HBRUSH b3 = m_currentBrush3;
		HDC hDC = bi.GetDC();
		// b3
		::OffsetRgn(hRgn, 3, 0);
		{
			::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, +3, -3);
		}
		::OffsetRgn(hRgn, -3, 0);
		// b1
		{
			::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, -2);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, -2, 0);
		}
		// b2
		::OffsetRgn(hRgn, 2, 1);
		{
			::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 1, -1);
		}
		::OffsetRgn(hRgn, -2, -1);
		//
		bi.ReleaseDC();
		bi.InsertOnTransparent(1, 1, bmp, m_transColor);
		_DeleteObject(hRgn);
		return bi;
	}

	/// 画像作成
	CBitmapImage m_MakeButtonRv(const CBitmapImage& bmp) const
	{
		CBitmapImage bi;
		bi.Set(bmp.GetSize().cx + 3, bmp.GetSize().cy + 3, m_baseColor);
		HRGN hRgn = bmp.CreateRgn(m_transColor);
		HBRUSH b1 = m_currentBrush1;
		HBRUSH b2 = m_currentBrush2;
		HBRUSH b3 = m_currentBrush3;
		HDC hDC = bi.GetDC();
		// b3
		{
			::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, -3);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -3, 0);
		}
		// b1
		::OffsetRgn(hRgn, 3, 1);
		{
			::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 2, -2);
		}
		::OffsetRgn(hRgn, -3, -1);
		// b2
		::OffsetRgn(hRgn, 1, 1);
		{
			::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, -1, 0);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 0, -1);
		}
		::OffsetRgn(hRgn, -1, -1);
		//
		bi.ReleaseDC();
		bi.InsertOnTransparent(2, 2, bmp, m_transColor);
		_DeleteObject(hRgn);
		return bi;
	}

	CBrushHandle	m_brush1;			///< 明るい線
	CBrushHandle	m_brush2;			///< 暗い線
	CBrushHandle	m_brush3;			///< もっと暗い線
	HBRUSH			m_currentBrush1;	///< 明るい線
	HBRUSH			m_currentBrush2;	///< 暗い線
	HBRUSH			m_currentBrush3;	///< もっと暗い線
	COLORREF		m_transColor;		///< 透過色
	COLORREF		m_baseColor;		///< 背景色
};



};///TNB
