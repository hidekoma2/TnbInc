#pragma once
/**
 *@file
 * ビットマップ領域描画情報のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawable.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB 
{



/**@ingroup DRAWABLE BITMAP
 * ビットマップ領域描画クラス
 *
 *		一つのビットマップを3x3に分割し、隅、枠、中のパーツにし、それを描画に使用します。
 *
 *	@par必要ファイル
 *			TnbBitmapBorderDrawer.h
 * 
 *	@date 07/10/06	新規作成
 *	@date 10/04/21	元データより小さい大きさでの描画処理を改善
 */
class CBitmapBorderDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * コンストラクタ
	 *	@param bm 使用するビットマップ
	 */
	CBitmapBorderDrawer(CBitmapHandle bm)
	{
		m_bmps[0] = bm;
		m_ApartBitmap();
	}

	/**
	 * [取得] ビットマップ
	 *	@param idx 取得箇所。0;全体 1～9;テンキー並びで9箇所のビットマップ。
	 *	@return ビットマップ
	 */
	CBitmapHandle GetParts(int idx) const
	{
		if ( idx >= 0 && idx < 10 )
		{
			return m_bmps[idx].Clone();
		}
		return CBitmapHandle();
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const 
	{
		CBitmapBorderDrawer* P = new CBitmapBorderDrawer(*this);
		return P;
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@note 本クラスは何もしません。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		POINT po;
		::GetBrushOrgEx(dc, &po);
		const SIZE& winSize = m_size;
		SIZE sz = m_bmps[0].GetSize();
		sz.cx /= 3;
		sz.cy /= 3;
		bool isNarrow = (sz.cx * 2 > m_size.cx);	//狭い
		bool isLow = (sz.cy * 2 > m_size.cy);		//低い
		RECT rc;
		//===== 四済み
		//-- 左上
		::SetRect(&rc, 0, 0, min(sz.cx, m_size.cx / 2 + 1), min(sz.cy, m_size.cy / 2 + 1));
		if ( sz.cx == rc.right && sz.cy == rc.bottom )
		{
			m_bmps[7].Draw(dc, 0, 0);
		}
		else
		{
			m_bmps[7].Cut(rc).Draw(dc, 0, 0);
		}
		//-- 左下
		::SetRect(&rc, 0, 0, min(sz.cx, m_size.cx / 2 + 1), sz.cy);
		if ( sz.cx == rc.right )
		{
			m_Draw(dc, m_bmps[1], 0, winSize.cy - sz.cy, winSize);
		}
		else
		{
			m_Draw(dc, m_bmps[1].Cut(rc), 0, winSize.cy - sz.cy, winSize);
		}
		//-- 右上
		::SetRect(&rc, 0, 0, sz.cx, min(sz.cy, m_size.cy / 2 + 1));
		if ( sz.cy == rc.bottom )
		{
			m_Draw(dc, m_bmps[9], winSize.cx - sz.cx , 0, winSize);
		}
		else
		{
			m_Draw(dc, m_bmps[9].Cut(rc), winSize.cx - sz.cx , 0, winSize);
		}
		//-- 右下
		m_Draw(dc, m_bmps[3], winSize.cx - sz.cx , winSize.cy - sz.cy, winSize);
		//===== 辺
		if ( ! isNarrow )
		{
			//-- 上辺
			::SetRect(&rc, sz.cx, 0, winSize.cx - sz.cx, sz.cy);
			if ( rc.bottom > m_size.cy / 2 )
			{
				rc.bottom = m_size.cy / 2;
			}
			::SetBrushOrgEx(dc, po.x, 0, NULL);
			m_bmps[8].DrawTile(dc, rc);
			//-- 下辺
			::SetRect(&rc, sz.cx, winSize.cy - sz.cy, winSize.cx - sz.cx, winSize.cy);
			int t = rc.top;
			if ( rc.top < m_size.cy / 2 )
			{
				rc.top = m_size.cy / 2;
			}
			::SetBrushOrgEx(dc, po.x, t, NULL);
			m_bmps[2].DrawTile(dc, rc);
		}
		if ( ! isLow )
		{
			//-- 左辺
			::SetRect(&rc, 0, sz.cy, sz.cx, winSize.cy - sz.cy);
			if ( rc.right > m_size.cx / 2 )
			{
				rc.right = m_size.cx / 2;
			}
			::SetBrushOrgEx(dc, 0, po.y, NULL);
			m_bmps[4].DrawTile(dc, rc);
			//-- 右辺
			::SetRect(&rc, winSize.cx - sz.cx, sz.cy, winSize.cx, winSize.cy - sz.cy);
			int t = rc.left;
			if ( rc.left < m_size.cx / 2 )
			{
				rc.left = m_size.cx / 2;
			}
			::SetBrushOrgEx(dc, t, po.y, NULL);
			m_bmps[6].DrawTile(dc, rc);
		}
		//===== 中央
		if ( ! isNarrow && ! isLow )
		{
			::SetRect(&rc, sz.cx, sz.cy, winSize.cx - sz.cx, winSize.cy - sz.cy);
			::SetBrushOrgEx(dc, po.x, po.y, NULL);
			m_bmps[5].DrawTile(dc, rc);
		}
	}

private:
	/// m_bmps[0] を分解して、m_bmps[1～9]に設定
	void m_ApartBitmap(void)
	{
		CBitmapImage& bi = m_bmps[0];
		SIZE sz = bi.GetSize();
		sz.cx /= 3;
		sz.cy /= 3;
		RECT rc = { 0, 0, sz.cx, sz.cy };
		m_bmps[7] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[8] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[9] = bi.Cut(rc);
		::SetRect(&rc, 0, sz.cy, sz.cx, sz.cy * 2);
		m_bmps[4] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[5] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[6] = bi.Cut(rc);
		::SetRect(&rc, 0, sz.cy * 2, sz.cx, sz.cy * 3);
		m_bmps[1] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[2] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[3] = bi.Cut(rc);
	}
	void m_Draw(HDC dc, const CBitmapImage& bi, int px, int py, const SIZE& size) const
	{
		SIZE sz = bi.GetSize();
		RECT rc = { 0, 0, sz.cx, sz.cy };
		bool r = false;
		int dx = sz.cx - (size.cx / 2 + 1);
		if ( dx > 0 )
		{
			rc.left += dx;
			px += dx;
			r = true;
		}
		int dy = sz.cy - (size.cy / 2 + 1);
		if ( dy > 0 )
		{
			rc.top += dy;
			py += dy;
			r = true;
		}
		if ( r )
		{
			bi.Cut(rc).Draw(dc, px, py);
		}
		else
		{
			bi.Draw(dc, px, py);
		}
	}

	CBitmapImage	m_bmps[10];		///< 0;全体 1～9;テンキー並びで9箇所のビットマップ
};



}; //TNB
