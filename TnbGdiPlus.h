#pragma once
/**
 *@file
 * GDI+ 関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifdef new
 #pragma push_macro("new")
 #undef new
 #define __TnbNEEDPOPNEW__
#endif



#include "TnbPointerHandle.h"
#include "TnbStrLib.h"
#include <math.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 // HWNDとリスナーのMAP
 _SELECTANY ULONG_PTR s_uGdiToken = 0; 
#endif



/**@ingroup GDIPLUS BITMAP
 * GDI+ 管理クラス
 *
 *	@todo 作りかけ
 *
 *	@attention 非 UNICODEプロジェクトでも、本クラスの文字列は UNICODE で指定する必要があります。
 *				CUnicode 等、使用してください。
 *
 *	@par必要ファイル
 *			TnbGdiPlus.h
 * 
 *	@date 07/10/29 新規作成。
 *	@date 08/07/14 回転描画処理を改善。
 *	@date 09/01/14 Jpegのクオリティ指定、追加。
 *	@date 09/02/02 newマクロ(VC6)対策。
 *	@date 11/08/11 DT_* スタイルの処理が縦横逆になっていたのを修正。
 *	@date 19/03/27 DrawLine() 追加。
 */
class CGdiPlus
{
public:

	/**@ingroup GDIPLUS BITMAP
	 * GDI+用座標型
	 *
	 *	@par必要ファイル
	 *			TnbGdiPlus.h
	 * 
	 *	@date 08/07/14 新規作成。
	 */
	class CPos : public Gdiplus::PointF
	{
		DEFSUPER(Gdiplus::PointF);
		// 変換
		template<typename T> Gdiplus::REAL _(T t) const
		{
			return static_cast<Gdiplus::REAL>(t);
		}
	public:
		/**
		 * コンストラクタ.
		 *	@param x X座標
		 *	@param y Y座標
		 */
		CPos(double x, double y) : _super(_(x), _(y)) { }
		/**
		 * コンストラクタ.
		 *	@param pos 座標
		 */
		CPos(const POINT& pos) : _super(_(pos.x), _(pos.y)) { }
		/**
		 * コンストラクタ.
		 *	@param sz 座標
		 */
		CPos(const SIZE& sz) : _super(_(sz.cx), _(sz.cy)) { }
	};


	//-----------------------------


	/// デストラクタ
	virtual ~CGdiPlus(void)
	{
	}

	/**
	 * コンストラクタ.
	 */
	CGdiPlus(void) : m_hDC(NULL)
	{
		Startup();
	}
	
	/**
	 * コンストラクタ.
	 *	@param hdc デバイスコンテキストハンドル
	 */
	CGdiPlus(HDC hdc) : m_hDC(hdc)
	{
		Startup();
		m_pGraphics = Gdiplus::Graphics::FromHDC(hdc);
		m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	}
	
	/**
	 * コピーコンストラクタ.
	 *	@param other コピー元
	 */
	CGdiPlus(const CGdiPlus& other) : m_hDC(NULL)
	{
		operator=(other);
	}

	/**
	 * コピーオペレータ.
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CGdiPlus& operator=(const CGdiPlus& other)
	{
		if ( this != &other )
		{
			m_pGraphics.Null();
			if ( m_hDC != NULL )
			{
				m_pGraphics = Gdiplus::Graphics::FromHDC(m_hDC);
				m_pGraphics->SetTextRenderingHint(other.m_pGraphics->GetTextRenderingHint());
			}
			m_pFont = other.m_pFont;
			m_pBrush = other.m_pBrush;
			m_pPen = other.m_pPen;
			m_pStringFormat = other.m_pStringFormat;
		}
		return *this;
	}

	/**
	 * [設定] デバイスコンテキスト設定.
	 *		本クラスで扱うデバイスコンテキストを指定します。
	 *	@param hdc デバイスコンテキスト
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetDC(HDC hdc)
	{
		m_pGraphics.Null();
		m_hDC = hdc;
		m_pGraphics = Gdiplus::Graphics::FromHDC(hdc);
		m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		return m_Ret(m_pGraphics);
	}

	/**
	 * [設定] フォント設定.
	 *		本クラスで扱うフォントを指定します。
	 *	@param hFont フォントハンドル
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetFont(HFONT hFont)
	{
		m_pFont = new Gdiplus::Font(m_hDC, hFont);
		return m_Ret(m_pFont);
	}

	/**
	 * フォントスタイル値.
	 *	@see SetFont()
	 */
	enum EFontStyle
	{
		REGULAR		= Gdiplus::FontStyleRegular,		///< 通常
		BOLD		= Gdiplus::FontStyleBold,			///< 太字
		ITALIC		= Gdiplus::FontStyleItalic,			///< 斜め
		UNDERLINE	= Gdiplus::FontStyleUnderline,		///< 下線
		STRIKEOUT	= Gdiplus::FontStyleStrikeout,		///< Strikeout
	};

	/**
	 * [設定] フォント設定.
	 *		本クラスで扱うフォントを指定します。
	 *	@param lpszFontName フォント名
	 *	@param size サイズ
	 *	@param style フォントスタイル。 {@link TNB::CGdiPlus::EFontStyle EFontStyle} から複数「or」で指定できます。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetFont(LPCWSTR lpszFontName, float size, int style = REGULAR)
	{
		Gdiplus::FontFamily fontfamily(lpszFontName);
		m_pFont = new Gdiplus::Font(&fontfamily, size, style, Gdiplus::UnitPixel);
		return m_Ret(m_pFont);
	}

	/**
	 * [設定] ブラシ設定.
	 *		本クラスで扱うブラシを指定します。
	 *	@note 指定色単一のブラシが用意されます。
	 *	@param color ブラシの色
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetBrush(COLORREF color)
	{
		Gdiplus::Color c;
		c.SetFromCOLORREF(color);
		m_pBrush = new Gdiplus::SolidBrush(c);
		return m_Ret(m_pBrush);
	}

	/**
	 * [設定] ブラシ設定.
	 *		本クラスで扱うブラシを指定します。
	 *	@note 指定のビットマップ柄のブラシが用意されます。
	 *	@param bmp ブラシの模様にするビットマップ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetBrush(HBITMAP bmp)
	{
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		if ( bm.GetLastStatus() == Gdiplus::Ok )
		{
			m_pBrush = new Gdiplus::TextureBrush(&bm);
			return m_Ret(m_pBrush);
		}
		m_pBrush.Null();
		return false;
	}

//LinearGradientBrush
//HatchBrush

	
	/**
	 * [設定] ペン設定.
	 *		本クラスで扱うペンを指定します。
	 *	@note 指定色単一のペンが用意されます。
	 *	@param color ペンの色
	 *	@param width 幅
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetPen(COLORREF color, float width)
	{
		Gdiplus::Color co;
		co.SetFromCOLORREF(color);
		m_pPen = new Gdiplus::Pen(co, width);
		return m_Ret(m_pPen);
	}

	/**
	 * テキストレンダリングヒント値.
	 *	@see SetTextRenderingHint()
	 */
	enum ETextRenderingHint
	{
		SYSTEM_DEFAULT	= Gdiplus::TextRenderingHintSystemDefault,				///< システムデフォルト
		BITPERPIXEL		= Gdiplus::TextRenderingHintSingleBitPerPixelGridFit,	///< ビットマップ
		ANTIALIAS		= Gdiplus::TextRenderingHintAntiAliasGridFit,			///< アンチエイリアス
		CLEARTYPE		= Gdiplus::TextRenderingHintClearTypeGridFit			///< クリアタイプ
	};

	/**
	 * [設定] テキストレンダリングヒント設定.
	 *		本クラスで扱うテキストレンダリングヒントを指定します。
	 *	@param hint ヒント値
	 *	@param boIsGridFit ヒント値が、 BITPERPIXEL 、 ANTIALIAS の場合、 false を指定すると、Gridfitしません。
	 *	@retval false 失敗。 デバイスコンテキストが指定されていません。
	 *	@retval true 成功。
	 */
	bool SetTextRenderingHint(ETextRenderingHint hint, bool boIsGridFit = false)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		int i = hint;
		if ( ! boIsGridFit && (hint == BITPERPIXEL || hint == ANTIALIAS) )
		{
			i++;
		}
		Gdiplus::TextRenderingHint h = static_cast<Gdiplus::TextRenderingHint>(i);
		return m_Ret(m_pGraphics->SetTextRenderingHint(h));
	}	

	/**
	 * [設定] テキストスタイル設定.
	 *		DrawText する際使用するスタイルを指定します。
	 *	@param drawTextStyle DT_ で始まるシンボルの組み合わせを指定できます。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetTextStyle(DWORD drawTextStyle)
	{
		m_pStringFormat = ms_DtToStringFormat(drawTextStyle);
		return m_Ret(m_pStringFormat);
	}

	/**
	 * [描画] テキスト描画.
	 *	@note SetFont() , SetBrush() がされている必要があります。
	 *			また、 SetTextRenderingHint() , SetTextStyle() で表示設定が可能です。
	 *	@param rc 表示位置
	 *	@param lpsz 表示文字列
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawText(const RECT& rc, LPCWSTR lpsz) const
	{
		if ( m_pGraphics.IsNull() || m_pFont.IsNull() || m_pBrush.IsNull() ) { return false; }
		Gdiplus::RectF rcf(_(rc.left), _(rc.top), _(rc.right - rc.left + 1), _(rc.bottom - rc.top + 1));
		Gdiplus::Status r = m_pGraphics->DrawString(lpsz, -1, m_pFont, rcf, m_pStringFormat, m_pBrush);
		return m_Ret(r);
	}

	/**
	 * [描画] テキスト描画.
	 *	@note SetFont() , SetBrush() がされている必要があります。
	 *			また、 SetTextRenderingHint() , SetTextStyle() で表示設定が可能です。
	 *	@param po 表示位置
	 *	@param lpsz 表示文字列
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawText(const CGdiPlus::CPos& po, LPCWSTR lpsz) const
	{
		if ( m_pGraphics.IsNull() || m_pFont.IsNull() || m_pBrush.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->DrawString(lpsz, -1, m_pFont, po, m_pStringFormat, m_pBrush);
		return m_Ret(r);
	}

	/**
	 * イメージ操作アルゴリズムモード値.
	 *	@see SetImageInterpolationMode()
	 */
	enum EInterpolationMode
	{
		BICUBIC			= Gdiplus::InterpolationModeBicubic,			///< 双三次補間を指定します。事前フィルタ処理は実行されません。このモードは、イメージを元のサイズの 25% 以下にするような縮小処理には適していません。
		BILINEAR		= Gdiplus::InterpolationModeBilinear,			///< 双一次補間を指定します。事前フィルタ処理は実行されません。このモードは、イメージを元のサイズの 50% 以下にするような縮小処理には適していません。
//		I_DEFAULT		= Gdiplus::InterpolationModeDefault,			///< 既定のモードを指定します。
		HIGH			= Gdiplus::InterpolationModeHighQuality,		///< 高品質補間を指定します。
		HQ_BICUBIC		= Gdiplus::InterpolationModeHighQualityBicubic,	///< 高品質双三次補間を指定します。事前フィルタ処理が適用され、高品質の縮小処理が実行されます。このモードを使用すると、変換後のイメージが高品質になります。
		HQ_BILINEAR		= Gdiplus::InterpolationModeHighQualityBilinear,///< 高品質双一次補間を指定します。事前フィルタ処理が適用され、高品質の縮小処理が実行されます。 
//		I_INVALID		= Gdiplus::InterpolationModeInvalid,			///< 無効なモードを指定します。 
		LOW				= Gdiplus::InterpolationModeLowQuality,			///< 低品質補間を指定します。
		NEAREST_NEIGHBOR = Gdiplus::InterpolationModeNearestNeighbor,	///< 最近傍補間を指定します。
	};
	
	/**
	 * [設定] イメージ描画アルゴリズム設定.
	 *		イメージを拡大または回転するときのアルゴリズムを指定します。 
	 *	@param mode モード値。
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool SetImageInterpolationMode(EInterpolationMode mode)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->SetInterpolationMode(static_cast<Gdiplus::InterpolationMode>(mode));
		return m_Ret(r);
	}

	/**
	 * [設定] イメージ描画属性クリア.
	 *	@note SetImageTransparentColor() の設定を無効にします。
	 */
	void ClearImageAttribute(void)
	{
		m_pImageAttr = NULL;
	}

	/**
	 * [設定] イメージ描画透過色指定.
	 *		イメージ描画時に透過処理を行うことが出来ます。
	 *	@note 色の 3 つの要素 (赤、緑、青) が、それぞれ対応する要素の上位と下位のカラー キーの間にある場合、その色は透明になります。
	 *	@see ClearImageAttribute
	 *	@param colorLow 下位のカラー キー値。 
	 *	@param colorHigh 上位のカラー キー値。 
	 */
	void SetImageTransparentColor(COLORREF colorLow, COLORREF colorHigh)
	{
		if ( m_pImageAttr.IsNull() )
		{
			m_pImageAttr = new Gdiplus::ImageAttributes();
		}
		Gdiplus::Color c1;
		Gdiplus::Color c2;
		c1.SetFromCOLORREF(colorLow);
		c2.SetFromCOLORREF(colorHigh);
		m_pImageAttr->SetColorKey(c1, c2);
	}

	/**
	 * [設定] イメージ描画属性参照.
	 *	@note 低レベルメソッドです。本インスタンスで管理するイメージ属性の参照を返すことで、
	 *			自由に属性の設定をすることが可能です（GDI+の知識が必要です）。
	 *	@return Gdiplus::ImageAttribute のポインタ.
	 */
	CPointerHandleT<Gdiplus::ImageAttributes> ReferImageAttribute(void)
	{
		if ( m_pImageAttr.IsNull() )
		{
			m_pImageAttr = new Gdiplus::ImageAttributes();
		}
		return m_pImageAttr;
	}

	/**
	 * [描画] ビットマップ描画.
	 *	@param po 表示位置
	 *	@param bmp ビットマップ
	 *	@param mag 倍率。省略すると 1 になります。
	 *	@param isFlipX true を指定すると 水平方向に反転します。
	 *	@param isFlipY true を指定すると 垂直方向に反転します。
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawImage(const CGdiPlus::CPos& po, HBITMAP bmp, double mag = 1.0, bool isFlipX = false, bool isFlipY = false) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		Gdiplus::PointF pos[3];
		pos[0].X = _(po.X);
		pos[0].Y = _(po.Y);
		pos[1].X = _(po.X + bm.GetWidth() * mag);
		pos[1].Y = _(po.Y);
		pos[2].X = _(po.X);
		pos[2].Y = _(po.Y + bm.GetHeight() * mag);
		if ( isFlipX )
		{
			 pos[0].X = _(po.X + bm.GetWidth() * mag);		// 水平方向の反転を処理します。
			 pos[1].X = _(po.X);
			 pos[2].X = pos[0].X;
		}
		if ( isFlipY )
		{
			pos[0].Y = _(po.Y + bm.GetHeight() * mag);		// 垂直方向の反転を処理します。
			pos[1].Y = pos[0].Y;
			pos[2].Y = _(po.Y);
		}
		return m_DrawImage(bm, pos, 3);
	}

	/**
	 * [描画] ビットマップフィット描画.
	 *		指定の表示位置に画像の比率を変えずに表示します。
	 *	@param rc 表示位置
	 *	@param bmp ビットマップ
	 *	@param isFlipX true を指定すると 水平方向に反転します。
	 *	@param isFlipY true を指定すると 垂直方向に反転します。
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawFitImage(const RECT& rc, HBITMAP bmp, bool isFlipX = false, bool isFlipY = false) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		double width = rc.right - rc.left;
		double height = rc.bottom - rc.top;
		double dx = width / bm.GetWidth();
		double dy = height / bm.GetHeight();
		CGdiPlus::CPos po(0, 0);
		double d = 1.0;
		if ( dx > dy )
		{
			//dy をベースにする
			d = dy;
			double x = (width - bm.GetWidth() * d);
			po.X = static_cast<float>(x / 2.0);
		}
		else
		{
			//dx をベースにする
			d = dx;
			double y = (height - bm.GetHeight() * d);
			po.Y = static_cast<float>(y / 2.0);
		}
		return DrawImage(po, bmp, d, isFlipX, isFlipY);
	}
	
	/**
	 * [描画] ビットマップ回転描画.
	 *	@param po 重心表示位置。
	 *	@param bmp ビットマップ。
	 *	@param hotSpot ビットマップの重心。ビットマップの相対座標を指定します。
	 *	@param angle 角度 ( 0～359°)
	 *	@param mag 倍率。省略すると 1 になります。
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawRotateImage(const CGdiPlus::CPos& po, HBITMAP bmp, const CGdiPlus::CPos& hotSpot, double angle, double mag = 1.0) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE hPalette = NULL;
		Gdiplus::Bitmap bm(bmp, hPalette);
		Gdiplus::PointF pos[3];
		double restW = bm.GetWidth() - hotSpot.X;
		double restH = bm.GetHeight() - hotSpot.Y;
		double radian = ToRadian(angle);
		double sinVal = ::sin(radian);
		double cosVal = ::cos(radian);
		// 回転・拡大 & 平行移動
		pos[0].X = _(((hotSpot.X * -cosVal) - (hotSpot.Y * -sinVal)) * mag + po.X);
		pos[0].Y = _(((hotSpot.X * -sinVal) + (hotSpot.Y * -cosVal)) * mag + po.Y);
		pos[1].X = _(((restW * cosVal) - (hotSpot.Y * -sinVal)) * mag + po.X);
		pos[1].Y = _(((restW * sinVal) + (hotSpot.Y * -cosVal)) * mag + po.Y);
		pos[2].X = _(((hotSpot.X * -cosVal) - (restH * sinVal)) * mag + po.X);
		pos[2].Y = _(((hotSpot.X * -sinVal) + (restH * cosVal)) * mag + po.Y);
		//
		return m_DrawImage(bm, pos, 3);
	}

	/**
	 * [描画] ビットマップ回転描画.
	 *	@note 重心は画像の中心になります。
	 *	@param po 重心表示位置。
	 *	@param bmp ビットマップ。
	 *	@param angle 角度 ( 0～359°)
	 *	@param mag 倍率。省略すると 1 になります。
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawRotateImage(const CGdiPlus::CPos& po, HBITMAP bmp, double angle, double mag = 1.0) const
	{
		BITMAP bm;
		if ( ::GetObject(bmp, sizeof(BITMAP), &bm) )
		{
			return DrawRotateImage(po, bmp, CPos(bm.bmWidth / 2.0, bm.bmHeight / 2.0), angle, mag);
		}
		return false;
	}

	/**
	 * [描画] ビットマップ傾斜描画.
	 *	@param po 表示位置
	 *	@param bmp ビットマップ
	 *	@param diff X方向、Y方向のずれ
	 *	@param mag 倍率。省略すると 1 になります。
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawSkewImage(const CGdiPlus::CPos& po, HBITMAP bmp, const CGdiPlus::CPos& diff, double mag = 1.0) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		Gdiplus::PointF pos[3];
		pos[0].X = _(po.X);
		pos[0].Y = _(po.Y);
		pos[1].X = _(po.X + bm.GetWidth() * mag);
		pos[1].Y = _(po.Y + diff.Y * mag);
		pos[2].X = _(po.X + diff.X * mag);
		pos[2].Y = _(po.Y + bm.GetHeight() * mag);
		//
		return m_DrawImage(bm, pos, 3);
	}

	/**
	 * スムージングモード値.
	 *	@see SetSmoothingMode()
	 */
	enum ESmoothingMode
	{
//		S_INVALID		= Gdiplus::SmoothingModeInvalid,		///< 無効なモードを指定します。
//		S_DEFAULT		= Gdiplus::SmoothingModeDefault,		///< アンチエイリアス処理しないことを指定します。
		HIGH_SPEED		= Gdiplus::SmoothingModeHighSpeed,		///< 高速で、低品質のレンダリングを指定します。
		HIGH_QUALITY	= Gdiplus::SmoothingModeHighQuality,	///< 高品質で低速なレンダリングを指定します。
		NONE			= Gdiplus::SmoothingModeNone,			///< アンチエイリアス処理しないことを指定します。
		ANTI_ALIAS		= Gdiplus::SmoothingModeAntiAlias		///< アンチエイリアス処理されたレタリングを指定します。
	};

	/**
	 * [設定] スムージングモード設定.
	 *	@note 直線、曲線、および塗りつぶし領域の角に対してスムージング (アンチエイリアスとも呼ばれます) を使用するかどうかを指定します。
	 *	@param mode モード
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool SetSmoothingMode(ESmoothingMode mode)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->SetSmoothingMode(static_cast<Gdiplus::SmoothingMode>(mode));
		return m_Ret(r);
	}

	/**
	 * [描画] ライン描画.
	 *		ペンは SetPen() で指定します。
	 *	@param s 始点
	 *	@param e 終点
	 *	@retval false 失敗。 デバイスコンテキストなどが指定されていません。
	 *	@retval true 成功。
	 */
	bool DrawLine(const CPos& s, const CPos& e)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->DrawLine(m_pPen, s, e);
		return m_Ret(r);
	}

  /*
	bool DrawArc()
	bool DrawCurve()
	bool DrawEllipse()
	bool DrawPie()
	bool DrawPolygon()
	bool DrawRectangle()

	bool FillCurve()
	bool FillEllipse()
	bool FillPie()
	bool FillPolygon()
	bool FillRectangle()
	bool FillRegion()
	*/

	//------------------------------------

	/**
	 * [設定] GDI+ 初期化.
	 *	@note プログラムで一度だけコールする必要があります。
	 *			自動的にコールされますが、時間がかかるため、
	 *			プログラムの最初で一度コールしておいたほうがよいかもしれません。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool Startup(void)
	{
		if ( s_uGdiToken == 0 )
		{
			static CReject r;
			Gdiplus::GdiplusStartupInput startupInput;
			Gdiplus::GdiplusStartup(&s_uGdiToken, &startupInput, NULL);
		}
		return s_uGdiToken != 0;
	}

	/**
	 * [表示] 文字列表示.
	 *	@param dc デバイスコンテキスト。
	 *	@param hFont フォント。
	 *	@param drawTextStyle DT_ で始まるシンボルの組み合わせを指定できます。
	 *	@param color 色。
	 *	@param po 座標。
	 *	@param lpsz 文字列。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool DrawAntialiasText(HDC dc, HFONT hFont, DWORD drawTextStyle, COLORREF color, const CGdiPlus::CPos& po, LPCWSTR lpsz)
	{
		CGdiPlus g(dc);
		g.SetFont(hFont);
		g.SetBrush(color);
		g.SetTextRenderingHint(ANTIALIAS);
		g.SetTextStyle(drawTextStyle);
		return g.DrawText(po, lpsz);
	}

	/**
	 * [作成] 画像ファイル読込み.
	 *		BMP, JPEG などのファイルを読み込み、 HBITMAP を作成します。
	 *	@param lpsz ファイル名。
	 *	@retval NULL以外 成功。
	 *	@retval NULL 失敗。
	 */
	static HBITMAP LoadBitmap(LPCWSTR lpsz)
	{
		Startup();
		Gdiplus::Bitmap bm(lpsz);
		if ( bm.GetLastStatus() == Gdiplus::Ok )
		{
			Gdiplus::Color c;
			c.SetFromCOLORREF(RGB(255, 255, 255));
			HBITMAP hBmp;
			if ( bm.GetHBITMAP(c, &hBmp) == Gdiplus::Ok )
			{
				return hBmp;
			}
		}
		return NULL;
	}

	/**
	 * [作成] サムネイル画像ファイル読込み.
	 *		画像ファイル内のサムネイルを読み込み、 HBITMAP を作成します。
	 *	@param lpsz ファイル名。
	 *	@retval NULL以外 成功。
	 *	@retval NULL 失敗。サムネイルがない場合も失敗になります。
	 */
	static HBITMAP LoadThumbnail(LPCWSTR lpsz)
	{
		Startup();
		Gdiplus::Bitmap bmp(lpsz);
		if ( bmp.GetLastStatus() == Gdiplus::Ok )
		{
			UINT size = bmp.GetPropertyItemSize(PropertyTagThumbnailData);
			if ( size > 0 )
			{
				HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, size);
				BYTE* B = static_cast<BYTE *>(::GlobalLock(hGlobal));
				Gdiplus::PropertyItem* P = reinterpret_cast<Gdiplus::PropertyItem*>(B);
				bmp.GetPropertyItem(PropertyTagThumbnailData, size, P);
				TNB::MemCopy(&B[0], &B[16], size - 16);
				::GlobalUnlock(hGlobal);
				IStream* pis = NULL;
				CreateStreamOnHGlobal(hGlobal, TRUE, &pis);
				Gdiplus::Bitmap bm(pis);
				if ( bm.GetLastStatus() == Gdiplus::Ok )
				{
					Gdiplus::Color c;
					c.SetFromCOLORREF(RGB(0, 0, 0));
					HBITMAP hBmp;
					if ( bm.GetHBITMAP(c, &hBmp) == Gdiplus::Ok )
					{
						pis->Release();
						::GlobalFree(hGlobal);
						return hBmp;
					}
				}
				if ( pis != NULL )
				{
					pis->Release();
				}
				::GlobalFree(hGlobal);
			}
		}
		return NULL;
	}

	/**
	 * Saveフォーマット値
	 *	@see SaveBitmap()
	 */
	enum EBitmapFormat
	{
		BMP,		///< BMP
		JPEG,		///< JPEG
		JPG = JPEG,	///< JPEG
		GIF,		///< GIF
		TIFF,		///< TIFF
		TIF = TIFF,	///< TIFF
		PNG,		///< PNG
	};

	/**
	 * [作成] 画像ファイル作成.
	 *		ビットマップを BMP, JPEG などのファイルにします。
	 *	@param lpsz ファイル名。
	 *	@param bmp ビットマップ。
	 *	@param fmt フォーマット。省略するとBMPです。
	 *	@param jpegQuality JPEGのクオリティ(0～100)。省略すると80。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool SaveBitmap(LPCWSTR lpsz, Gdiplus::Bitmap& bmp, EBitmapFormat fmt = BMP, LONG jpegQuality = 80)
	{
		Startup();
		LPCWSTR lpFmtDes;
		Gdiplus::EncoderParameters encoderParameters;
		Gdiplus::EncoderParameters* pEncoderParameters = NULL;
		switch( fmt )
		{
		case BMP:
		default:	lpFmtDes = L"BMP";	break;
		case GIF:	lpFmtDes = L"GIF";	break;
		case TIFF:	lpFmtDes = L"TIFF";	break;
		case PNG:	lpFmtDes = L"PNG";	break;
		case JPEG:	
			lpFmtDes = L"JPEG";	
			encoderParameters.Count = 1;
			encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality; 
			encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
			encoderParameters.Parameter[0].NumberOfValues = 1;
			encoderParameters.Parameter[0].Value = &jpegQuality;	//JPEGクオリティ：0～100
			pEncoderParameters = &encoderParameters;
			break;
		}		
		UINT numEncoders;
		UINT size = 0;
		Gdiplus::GetImageEncodersSize(&numEncoders, &size);
		CWorkMem work(size);
		Gdiplus::ImageCodecInfo* pEncoders = reinterpret_cast<Gdiplus::ImageCodecInfo*>(work.Ref());
		Gdiplus::GetImageEncoders(numEncoders, size, pEncoders);
		loop ( i, numEncoders )
		{
			if ( STRLIB::Compare(pEncoders[i].FormatDescription, lpFmtDes) == 0 )
			{
				return bmp.Save(lpsz, &(pEncoders[i].Clsid), pEncoderParameters) == Gdiplus::Ok;
			}
		}
		return false;
	}

	/**
	 * [作成] 画像ファイル作成.
	 *		ビットマップを BMP, JPEG などのファイルにします。
	 *	@param lpsz ファイル名。
	 *	@param hBmp ビットマップ。
	 *	@param fmt フォーマット。省略するとBMPです。
	 *	@param jpegQuality JPEGのクオリティ(0～100)。省略すると80。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool SaveBitmap(LPCWSTR lpsz, HBITMAP hBmp, EBitmapFormat fmt = BMP, LONG jpegQuality = 80)
	{
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(hBmp, h);
		return SaveBitmap(lpsz, bm, fmt, jpegQuality);
	}

private:

	//GDI+ Shotdown処理用サブクラス
	class CReject
	{
	public:	
		~CReject(void)
		{
			if ( s_uGdiToken != 0 )
			{
				Gdiplus::GdiplusShutdown(s_uGdiToken);
				s_uGdiToken = 0;
			}
		}
	};

	HDC											m_hDC;				///< デバイスコンテキスト
	CPointerHandleT<Gdiplus::Graphics>			m_pGraphics;		///< グラフィックオブジェクト
	CPointerHandleT<Gdiplus::ImageAttributes>	m_pImageAttr;		///< イメージ属性
	//
	CPointerHandleT<Gdiplus::StringFormat>		m_pStringFormat;	///< 文字フォーマットオブジェクト
	CPointerHandleT<Gdiplus::Font>				m_pFont;			///< フォントオブジェクト
	CPointerHandleT<Gdiplus::Brush>				m_pBrush;			///< ブラシオブジェクト
	CPointerHandleT<Gdiplus::Pen>				m_pPen;				///< ペンオブジェクト
//	Gdiplus::Region			m_region;		///< リージョン
//	Gdiplus::GraphicsPath	m_graphicsPath;	///< パス？

	/// DT → StrintFormat
	static Gdiplus::StringFormat* ms_DtToStringFormat(DWORD drawTextStyle)
	{
		Gdiplus::StringFormat* P = new Gdiplus::StringFormat();
		Gdiplus::StringAlignment alignX = Gdiplus::StringAlignmentNear;
		if ( (drawTextStyle & DT_CENTER) != 0 )
		{
			alignX = Gdiplus::StringAlignmentCenter;
		}
		else if ( (drawTextStyle & DT_RIGHT) != 0 )
		{
			alignX = Gdiplus::StringAlignmentFar;
		}
		P->SetAlignment(alignX);
		Gdiplus::StringAlignment alignY = Gdiplus::StringAlignmentNear;
		if ( (drawTextStyle & DT_VCENTER) != 0 )
		{
			alignY = Gdiplus::StringAlignmentCenter;
		}
		else if ( (drawTextStyle & DT_BOTTOM) != 0 )
		{
			alignY = Gdiplus::StringAlignmentFar;
		}
		P->SetLineAlignment(alignY);
		//
		int formatFlags = 0;
		if ( (drawTextStyle & DT_SINGLELINE) == 0 )
		{
			formatFlags |= Gdiplus::StringFormatFlagsNoClip;
		}
		P->SetFormatFlags(formatFlags);
		//
		Gdiplus::StringTrimming trim = Gdiplus::StringTrimmingNone;
		if ( (drawTextStyle & DT_END_ELLIPSIS) != 0 )
		{
			trim = Gdiplus::StringTrimmingEllipsisCharacter;
		}
		if ( (drawTextStyle & DT_WORD_ELLIPSIS) != 0 )
		{
			trim = Gdiplus::StringTrimmingEllipsisWord;
		}
		if ( (drawTextStyle & DT_PATH_ELLIPSIS) != 0 )
		{
			trim = Gdiplus::StringTrimmingEllipsisPath;
		}
		P->SetTrimming(trim);
		//
		if ( (drawTextStyle & DT_NOPREFIX) != 0 )
		{
			P->SetHotkeyPrefix(Gdiplus::HotkeyPrefixNone);
		}
		else
		{
			P->SetHotkeyPrefix(Gdiplus::HotkeyPrefixShow);
		}
		return P;
	}
	// 変換
	template<typename T> Gdiplus::REAL _(T t) const
	{
		return static_cast<Gdiplus::REAL>(t);
	}
	// π
	static double PI(void)
	{
		return 3.141592653589793;
	}
	// 角度 -> ラジアン
	static double ToRadian(double angle)
	{
		return angle * PI() / 180.0;
	}
	// ラジアン -> 角度
	static double ToAngle(double radian)
	{
		return radian * 180.0 / PI();
	}
	// 結果判断
	bool m_Ret(Gdiplus::Status s) const
	{
		return s == Gdiplus::Ok;
	}
	// 結果判断
	template<typename TYP>
	bool m_Ret(CPointerHandleT<TYP>  P) const
	{
		return P->GetLastStatus() == Gdiplus::Ok;
	}
	/**
	 * 描画.
	 */
	bool m_DrawImage(Gdiplus::Bitmap& bm, Gdiplus::PointF* pPos, int posLength) const
	{
		ASSERT( ! ::IsBadReadPtr(pPos, sizeof(Gdiplus::PointF) * posLength) );
		if ( ! m_pImageAttr.IsNull() )
		{
			Gdiplus::Status r = m_pGraphics->DrawImage( &bm, pPos, posLength, 0, 0, _(bm.GetWidth()), _(bm.GetHeight()), Gdiplus::UnitPixel, m_pImageAttr);
			return m_Ret(r);
		}
		return m_Ret(m_pGraphics->DrawImage(&bm, pPos, posLength));
	}
};



}; // TNB



#ifdef __TnbNEEDPOPNEW__
 #pragma pop_macro("new")
#endif
