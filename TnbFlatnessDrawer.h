#pragma once
/**
 *@file
 * 平坦描画情報のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawable.h"



//TNB Library
namespace TNB 
{



/**@ingroup DRAWABLE
 * 平坦描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定された情報で、DC上の画像を加工します。
 *
 *	@par必要ファイル
 *			TnbFlatnessDrawer.h
 *
 *	@date 08/07/01 新規作成
 */
class CFlatnessDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color1;	///< 描画色１。
	COLORREF	m_color2;	///< 描画色２。影のように描画色１から右下にずれて描画される色。 CLR_INVALID を指定すると描画されない。
	COLORREF	m_back;		///< base の色の部分の描画色。省略すると base と同じ色をそれとする。
	COLORREF	m_base;		///< 元の絵の背景に当たる色。省略すると、左下の色をそれとする。
	COLORREF	m_work;		///< ワーク用の色。普通省略します。
public:

	/**
	 * コンストラクタ.
	 *	@param color1 描画色１
	 *	@param color2 描画色２。影のように描画色１から右下にずれて描画される色。 CLR_INVALID を指定すると描画されない。
	 *	@param back base の色の部分の描画色。省略すると base と同じ色をそれとする。
	 *	@param base 元の絵の背景に当たる色。省略すると、左下の色をそれとする。
	 *	@param work ワーク用の色。普通省略します。
	 */
	CFlatnessDrawer(COLORREF color1, COLORREF color2, COLORREF back = CLR_AUTOSELECT, COLORREF base = CLR_AUTOSELECT, COLORREF work = RGB(255, 255, 255))
		: m_color1(color1), m_color2(color2), m_back(back), m_base(base), m_work(work)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CFlatnessDrawer(m_color1, m_color2, m_back, m_base, m_work);
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		COLORREF base = m_base;
		COLORREF back = m_back;
		if ( base == CLR_AUTOSELECT )
		{
			base = ::GetPixel(dc, x, y + m_size.cy - 1);
		}
		if ( ! IS_RGBVALUE(back) )
		{
			back = base;
		}
		const DWORD	CP_ROP = 0xE20746;
		HBITMAP hBmp = ::CreateBitmap(m_size.cx, m_size.cy, 1, 1, NULL);
		HDC maskDC = ::CreateCompatibleDC(dc);
		HGDIOBJ oldBmp = ::SelectObject(maskDC, hBmp);
		{
			COLORREF oldColor = ::SetBkColor(dc, base);
			::BitBlt(maskDC, 0, 0, m_size.cx, m_size.cy, dc, x, y, SRCCOPY);
			::SetBkColor(dc, m_work);
			::BitBlt(maskDC, 0, 0, m_size.cx, m_size.cy, dc, x, y, NOTSRCERASE);
			//
			RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
			HBRUSH hr = ::CreateSolidBrush(back);
			::FillRect(dc, &rc, hr);
			_DeleteObject(hr);
			//
			HBRUSH brush1 = ::CreateSolidBrush(m_color1);
			HGDIOBJ oldBrush = ::SelectObject(dc, brush1);
			::BitBlt(dc, x, y, m_size.cx, m_size.cy, maskDC, 0, 0, CP_ROP);
			::SelectObject(dc, oldBrush);
			_DeleteObject(brush1);
			if ( m_color2 != CLR_INVALID )
			{
				HBRUSH brush2 = ::CreateSolidBrush(m_color2);
				HGDIOBJ oldBrush = ::SelectObject(dc, brush2);
				::BitBlt(dc, x - 1, y - 1, m_size.cx, m_size.cy, maskDC, 0, 0, CP_ROP);
				::SelectObject(dc, oldBrush);
				_DeleteObject(brush2);
			}
			::SetBkColor(dc, oldColor);
		}
		::SelectObject(maskDC, oldBmp);
		_DeleteObject(maskDC);
		_DeleteObject(hBmp);
	}

	/**
	 * [取得] アクティブ選択状態設定インスタンス作成.
	 *	@note 画像をアクティブ選択状態に変換する描画情報を作成します。
	 *	@param base 元の絵の背景に当たる色。省略すると、左下の色をそれとする。
	 *	@param work ワーク用の色。普通省略します。
	 *	@return CFlatnessDrawer のポインタ。使用後は delete する必要があります。
	 */
	static CFlatnessDrawer* CreateSelectedType(COLORREF base = CLR_AUTOSELECT, COLORREF work = RGB(255, 255, 255))
	{
		return new CFlatnessDrawer(::GetSysColor(COLOR_HIGHLIGHTTEXT), CLR_INVALID, ::GetSysColor(COLOR_HIGHLIGHT), base, work);
	}

	/**
	 * [取得] 非アクティブ選択状態設定インスタンス作成.
	 *	@note 画像を非アクティブ選択状態に変換する描画情報を作成します。
	 *	@param base 元の絵の背景に当たる色。省略すると、左下の色をそれとする。
	 *	@param work ワーク用の色。普通省略します。
	 *	@return CFlatnessDrawer のポインタ。使用後は delete する必要があります。
	 */
	static CFlatnessDrawer* CreateNaSelectedType(COLORREF base = CLR_AUTOSELECT, COLORREF work = RGB(255, 255, 255))
	{
		return new CFlatnessDrawer(::GetSysColor(COLOR_WINDOWTEXT), CLR_INVALID, ::GetSysColor(COLOR_BTNFACE), base, work);
	}
};



};	// TNB
