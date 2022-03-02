#pragma once
/**
 *@file
 * ビットマップ描画関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapImage.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup BITMAP DRAWABLE
 * ビットマップ描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたビットマップを描画します。
 *
 *	@par必要ファイル
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/01 新規作成
 */
class CBitmapDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
protected:

	CBitmapHandle m_bmp;	///< ビットマップ
public:

	/**
	 * コンストラクタ
	 *	@param bmp ビットマップ。
	 */
	CBitmapDrawer(CBitmapHandle bmp) : m_bmp(bmp)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBitmapDrawer(m_bmp);
	}

	/**
	 * [取得] サイズ取得.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		return m_bmp.GetSize(_size);
	}

	/**
	 * [設定] サイズ設定.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを設定することが出来ます。
	 *	@note 本クラスはサイズ変更をサポートしません。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。サイズ変更はサポートしません。
	 */
	virtual bool Resize(const SIZE& size)
	{
		return false;
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
#ifndef _WIN32_WCE
		HBITMAP h = m_bmp;
		::DrawState(dc, NULL, NULL, reinterpret_cast<LPARAM>(h), 0, x, y, 0, 0, DST_BITMAP);
#else
		HBITMAP h = m_bmp;
		CBitmapHandle::Draw(dc, h, x, y);
#endif
	}

	/**
	 * [作成] ビットマップ作成.
	 *		描画情報から、ビットマップを作成します。
	 *	@param draw 描画情報
	 *	@param color 背景色を指定します。省略すると不定（塗りつぶししない) になります。
	 *				全エリア描画する描画情報の場合、背景色を指定する必要はありません。
	 *	@return ビットマップ。エラーの場合、 NULL状態のハンドルが返ります。
	 */
	static CBitmapHandle ToBitmap(const IDrawable& draw, COLORREF color = CLR_INVALID)
	{
		CBitmapImage bi;
		SIZE size;
		if ( draw.GetSize(size) )
		{
			if ( bi.Set(size.cx, size.cy, color) )
			{
				HDC dc = bi.GetDC();
				draw.Draw(dc);
				bi.ReleaseDC();
			}
		}
		return bi;
	}
};



#ifndef _WIN32_WCE
/**@ingroup BITMAP DRAWABLE
 * ビットマップ拡張描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたビットマップを描画します。
 *
 *	@par必要ファイル
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/01 新規作成
 */
class CBitmapExDrawer : public CBitmapDrawer
{
	DEFSUPER(CBitmapDrawer);
	UINT		m_uExFlag;		///< フラグ
public:

	/**
	 * コンストラクタ
	 *	@param bmp ビットマップハンドル。
	 *	@param uExFlag 挿入フラグ。以下の3種、使用できます。\n
	 *				省略か DSS_NORMAL - - - イメージをそのまま描画します。 \n
	 *				DSS_DISABLED - - - イメージを浮き上がらせます（エンボス）。\n
	 *				DSS_UNION - - - イメージをディザリングします。 
	 */
	CBitmapExDrawer(CBitmapHandle bmp, UINT uExFlag = DSS_NORMAL)
		: _super(bmp), m_uExFlag(uExFlag)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBitmapExDrawer(m_bmp, m_uExFlag);
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
		HBITMAP h = m_bmp;
		::DrawState(dc, NULL, NULL, reinterpret_cast<LPARAM>(h), 0, x, y, 0, 0, DST_BITMAP | m_uExFlag);
	}
};
#endif //_WIN32_WCE



/**@ingroup BITMAP DRAWABLE
 * ビットマップ透過描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたビットマップを透過,半透過処理を行いながら描画します。
 *
 *	@par必要ファイル
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/01 新規作成
 */
class CSemitransparentBitmapDrawer : public CBitmapDrawer
{
	DEFSUPER(CBitmapDrawer);
	int			m_parsent;		///< 濃さ
	COLORREF	m_color;		///< 透過色
public:

	/**
	 * コンストラクタ
	 *	@param bmp ビットマップハンドル。
	 *	@param par 描画画像の濃さ(単位;%) 。 -1 を指定すると、 保持しているイメージのピクセル毎のアルファチャネルを使用します。
	 *	@param color 透過する色。 CLR_INVALID を指定すると、透過色はなし。
	 */
	CSemitransparentBitmapDrawer(CBitmapHandle bmp, int par = 100, COLORREF color = CLR_INVALID)
		: _super(bmp), m_parsent(par), m_color(color)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CSemitransparentBitmapDrawer(m_bmp, m_parsent, m_color);
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
		CBitmapImage bi(m_bmp);
		bi.SemitransparectBlt(dc, x, y, m_parsent, m_color);
	}
};



/**@ingroup BITMAP DRAWABLE
 * タイル描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたビットマップ,描画情報を指定の大きさに敷き詰め描画します。
 *
 *	@par必要ファイル
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/14 新規作成
 */
class CTileDrawer : public CBitmapDrawer
{
	DEFSUPER(CBitmapDrawer);
	SIZE		m_size;		///< サイズ
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 *	@param bmp ビットマップ。
	 */
	CTileDrawer(const SIZE& size, CBitmapHandle bmp) : _super(bmp), m_size(size)
	{
	}

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 *	@param draw 描画情報。情報は本インスタンスで保持しますので、破棄してもかまいません。
	 *	@param color 背景色を指定します。省略すると不定（塗りつぶししない) になります。
	 *				全エリア描画する描画情報の場合、背景色を指定する必要はありません。
	 */
	CTileDrawer(const SIZE& size, const IDrawable& draw, COLORREF color = CLR_INVALID) 
		: _super(CBitmapDrawer::ToBitmap(draw, color)), m_size(size)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CTileDrawer(m_size, m_bmp);
	}

	/**
	 * [取得] サイズ取得.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		_size = m_size;
		return ! m_bmp.IsNull();
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
		CBitmapImage bi(m_bmp);
		RECT rect = { x, y, x + m_size.cx, y + m_size.cy };
		bi.DrawTile(dc, rect);	
	}
};



/**@ingroup DRAWABLE
 * 透過塗りつぶし描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定された色を半透過処理を行いながら描画します。
 *
 *	@par必要ファイル
 *			TnbBitmapDrawer.h
 *
 *	@date 07/12/13 新規作成
 *	@date 08/03/23 コンストラクタを追加。
 */
class CSemitransparentBoxFillDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color;		///< 色
	int			m_parsent;		///< 濃さ
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 *	@param color 色
	 *	@param par 描画の濃さ(単位;%) 
	 */
	CSemitransparentBoxFillDrawer(const SIZE& size, COLORREF color, int par = 100)
		: _super(size), m_color(color), m_parsent(par)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 *	@param color 色
	 *	@param par 描画の濃さ(単位;%) 
	 */
	CSemitransparentBoxFillDrawer(COLORREF color, int par = 100)
		: _super(), m_color(color), m_parsent(par)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CSemitransparentBoxFillDrawer(m_size, m_color, m_parsent);
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
		bi.Set(m_size.cx, m_size.cy, m_color);
		bi.AlphaBlend(dc, x, y, m_parsent);
	}
};



};///TNB

