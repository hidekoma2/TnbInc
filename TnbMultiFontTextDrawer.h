#pragma once
/**
 *@file
 * マルチフォントテキスト描画関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTextDrawer.h"
#include "TnbVector.h"
#include "TnbStrLib.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * マルチフォントテキスト描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		フォントと色と文字列をワンセットに、複数指定し、描画することが出来ます。
 *		その際、ベースラインをそろえて表示します。
 *
 *	@todo 現在、改行(複数行)は未対応です。
 *
 *	@par必要ファイル
 *			TnbMultiFontTextDrawer.h
 *
 *	@date 07/12/15 新規作成
 *	@date 08/03/23 コンストラクタを追加。
 */
class CMultiFontTextDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
protected:
	/// パート情報
	struct TPart
	{
		CVectorT<TCHAR>	str;			///< 文字列
		HFONT			font;			///< フォント
		COLORREF		textColor;		///< テキストカラー
	};
	CVectorT<TPart>		m_aParts;		///< 情報
	COLORREF			m_backColor;	///< 背景カラー
	DWORD				m_drawStyle;	///< DrawTextスタイル
	//
	mutable bool			m_boIsCalced;	///< 計算済み
	mutable CVectorT<POINT>	m_aOffsets;		///< 各PartのOFFSET
	mutable SIZE			m_partsSize;	///< 全Partの大きさ
	/// 計算
	bool m_calc(HDC dc, DWORD style) const
	{
		if ( m_boIsCalced ) { return true; }
		UINT un = DT_PATH_ELLIPSIS | DT_END_ELLIPSIS | DT_WORD_ELLIPSIS;
		DWORD st = style & (0xFFFFFFF0 - un);
		m_aOffsets.RemoveAll();
		m_partsSize.cx = 0;
		m_partsSize.cy = 0;
		int h = 0;
		HGDIOBJ oldFont = ::GetCurrentObject(dc, OBJ_FONT);
		loop ( i, m_aParts.GetSize() )
		{
			const TPart& pa = m_aParts.At(i);
			::SelectObject(dc, pa.font);
			TEXTMETRIC tTextMetric;
			if ( ! ::GetTextMetrics(dc, &tTextMetric) )
			{
				return false;
			}
			SIZE sz = { 0, 0 };
			if ( ! CTextDrawer::CalcTextSize(sz, dc, st, pa.str.ReferBuffer()) )
			{
				return false;
			}
			ASSERT( sz.cy == tTextMetric.tmHeight );
			int l = tTextMetric.tmAscent;//アンダーのサイズ
			h = max(h, l);
			m_aOffsets.Add(CPoint(m_partsSize.cx, -l));
			m_partsSize.cx += sz.cx;
			m_partsSize.cy = max(m_partsSize.cy, tTextMetric.tmHeight);
		}
		::SelectObject(dc, oldFont);
		int x = 0;
		int y = 0;
		switch( style & 0x03 )
		{
		case DT_CENTER:
			x = (m_size.cx - m_partsSize.cx) / 2;
			break;
		case DT_RIGHT:
			x = m_size.cx - m_partsSize.cx;
			break;
		}
		if ( x < 0 )
		{
			x = 0;
		}
		switch( style & 0x0C )
		{
		case DT_VCENTER:
			y = (m_size.cy - m_partsSize.cy) / 2;
			break;
		case DT_BOTTOM:
			y = m_size.cy - m_partsSize.cy;
			break;
		}
		if ( y < 0 )
		{
			y = 0;
		}
		loop ( i, m_aOffsets.GetSize() )
		{
			m_aOffsets[i].x += x;
			m_aOffsets[i].y += y + h;
		}
		m_boIsCalced = true;
		return true;
	}
public:

	/**
	 * コンストラクタ
	 *	@param size	サイズ
	 *	@param backColor 背景色
	 *	@param drawStyle DrawTextスタイル指定。	省略すると 「DT_CENTER | DT_VCENTER」
	 */
	CMultiFontTextDrawer(const SIZE& size, COLORREF backColor = CLR_INVALID, DWORD drawStyle = DT_CENTER | DT_VCENTER)
		: _super(size), m_backColor(backColor), m_drawStyle(drawStyle | DT_SINGLELINE), m_boIsCalced(false)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 *	@param backColor 背景色
	 *	@param drawStyle DrawTextスタイル指定。	省略すると 「DT_CENTER | DT_VCENTER」
	 */
	CMultiFontTextDrawer(COLORREF backColor = CLR_INVALID, DWORD drawStyle = DT_CENTER | DT_VCENTER)
		: _super(), m_backColor(backColor), m_drawStyle(drawStyle | DT_SINGLELINE), m_boIsCalced(false)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		CMultiFontTextDrawer* P =new CMultiFontTextDrawer(m_size, m_backColor, m_drawStyle);
		P->m_aParts = m_aParts;
		return P;
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
		if ( m_calc(dc, m_drawStyle) )
		{
			RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
			COLORREF oldColor = ::GetBkColor(dc);
			int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
			if ( IS_RGBVALUE(m_backColor) )
			{
				::SetBkColor(dc, m_backColor);
				::SetBkMode(dc, OPAQUE);
			}
			HGDIOBJ oldFont = ::GetCurrentObject(dc, OBJ_FONT);
			loop ( i, m_aParts.GetSize() )
			{
				const TPart& pa = m_aParts.At(i);
				::SelectObject(dc, pa.font);
				const POINT& offset = m_aOffsets[i];
				RECT r = rc;
				r.left += offset.x;
				r.top += offset.y;
				if ( r.right > r.left && r.bottom > r.top )
				{
					CTextDrawer::DrawTextRect(dc, r, m_drawStyle & 0xFFFFFFF0, pa.textColor, pa.str.ReferBuffer());
				}
			}
			::SelectObject(dc, oldFont);
			::SetBkMode(dc, oldBkMode);
			::SetBkColor(dc, oldColor);
		}
	}

	/**
	 * [追加] 情報追加.
	 *	@param str 文字列。
	 *	@param font 文字フォント。使用終わるまで破棄しないでください。
	 *	@param color 文字色。
	 *	@retval IVALID_INDEX エラー。
	 *	@retval 0以上 成功。値はインデックス。
	 */
	INDEX Add(LPCTSTR str, HFONT font, COLORREF color)
	{
		m_boIsCalced = false;
		TPart p;
		p.font = font;
		p.textColor = color;
		LPTSTR P = p.str.GetBuffer(_tcslen(str) + 1);
		STRLIB::Copy(P, str);
		p.str.ReleaseBuffer();
		return m_aParts.Add(p);
	}

	/**
	 * [設定] サイズ調整.
	 *		現在のサイズと文字列とDrawTextスタイルを使って、大きさを調整します。
	 *	@param dc デバイスコンテキスト。計算に使用します。描画はしません。
	 *	@param margin マージン。必要なサイズに加えるサイズです。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AdjustSize(HDC dc, int margin = 1)
	{
		m_boIsCalced = false;
		if ( ! m_calc(dc, m_drawStyle & 0xFFFFFFF0) )
		{
			return false;
		}
		ASSERTLIB( m_boIsCalced );
		m_size.cx = m_partsSize.cx + margin * 2;
		m_size.cy = m_partsSize.cy + margin * 2;
		m_boIsCalced = false; //計算フラグOFFにする
		return true;
	}
};



}; //TNB


