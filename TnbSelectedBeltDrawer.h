#pragma once
/**
 *@file
 * 選択帯描画情報のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawable.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB 
{



/**@ingroup DRAWABLE
 * 選択帯描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定された情報で、DC上の画像を加工します。
 *
 *	@par必要ファイル
 *			TnbSelectedBeltDrawer.h
 *
 *	@date 10/04/28 新規作成
 */
class CSelectedBeltDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * コンストラクタ.
	 *	@param beltColor 帯色（描画色）
	 *	@param backColor 背景色（元の色）
	 *	@param isTextInvert true ならテキスト(背景以外)の色反転をします。
	 */
	CSelectedBeltDrawer(COLORREF beltColor, COLORREF backColor, bool isTextInvert = true)
		: m_beltColor(beltColor), m_backColor(backColor), m_isTextInvert(isTextInvert)
	{
	}

	/**
	 * コンストラクタ.
	 *	@param size サイズ
	 *	@param beltColor 帯色（描画色）
	 *	@param backColor 背景色（元の色）
	 *	@param isTextInvert true ならテキスト(背景以外)の色反転をします。
	 */
	CSelectedBeltDrawer(const SIZE& size, COLORREF beltColor, COLORREF backColor, bool isTextInvert = true)
		: _super(size), m_beltColor(beltColor), m_backColor(backColor), m_isTextInvert(isTextInvert)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CSelectedBeltDrawer(m_size, m_beltColor, m_backColor, m_isTextInvert);
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
		CBitmapImage bi;
		COLORREF col = m_backColor ^ m_beltColor;
		RECT rect = { x, y, x + m_size.cx, y + m_size.cy };
		bi.SetFromDC(dc, rect);	// 描画範囲を取り込む
		CBitmapHandle bh = bi.CreateMaskBitmap(m_isTextInvert, m_backColor); //文字のところを抜き出す
		bi.Set(m_size.cx, m_size.cy, col);
		bi.Insert(0, 0, bh, m_isTextInvert ? SRCPAINT : SRCAND); //文字のところをマスクする
		bi.BitBlt(dc, x, y, SRCINVERT);
	}

private:
	COLORREF	m_beltColor;	///< 帯色
	COLORREF	m_backColor;	///< 背景色
	bool		m_isTextInvert;	///< 背景以外の反転？
};



};	// TNB
