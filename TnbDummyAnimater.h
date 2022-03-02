#pragma once
/**
 *@file
 * ダミーアニメ管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapImage.h"



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * ダミーアニメーター.
 *
 *		指定の大きさで簡単なアニメーションを作成します。
 *
 *	@par必要ファイル
 *			TnbDummyAnimater.h
 *
 *	@date 09/09/02 新規作成
 */
class CDummyAnimater
{
public:

	/// コンストラクタ
	CDummyAnimater(void)
	{
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * [作成] 新画像作成.
	 *	@param count 玉の数
	 *	@param cx 画像の幅
	 *	@param cy 画像の高さ
	 *	@param ph 玉の直径
	 */
	void NewImage(size_t count, int cx, int cy, int ph = 10)
	{
		m_size.cx = cx;
		m_size.cy = cy;
		//
		m_image.Set(m_size.cx, m_size.cy, RGB(0, 0, 0));
		m_blackImage.Set(m_size.cx, m_size.cy);
		m_blackImage.GradationFill(RGB(120, 0, 0), RGB(0, 0, 120), true);
		m_pieces.Resize(count);
		::srand(::GetTickCount());
		loop ( i, m_pieces.GetSize() )
		{
			m_pieces[i].Init(m_size, ph);
		}
		m_Remake();
	}

	/**
	 * [作成] 次画像作成.
	 *		次の画像を作成します。
	 */
	void Next(void)
	{
		loop ( i, m_pieces.GetSize() )
		{
			m_pieces[i].Next(m_size);
		}
		m_Remake();
	}

	/**
	 * [取得] 取得.
	 *		現在の画像を取得します。
	 *	@return 画像
	 */
	const CBitmapImage& Get(void) const
	{
		return m_image;
	}


private:
	// 画像再描画
	void m_Remake(void)
	{
		HDC dc = m_image.GetDC();
		m_blackImage.BitBlt(dc, 0, 0);
		loop ( i, m_pieces.GetSize() )
		{
			const TPiece& P = m_pieces[i];
			HBRUSH b = ::CreateSolidBrush(P.color);
//			RECT rc = { P.pos.x, P.pos.y, P.pos.x + P.size, P.pos.y + P.size };
//			::FillRect(dc, &rc, b);
			HGDIOBJ old = ::SelectObject(dc, b);
			::RoundRect(dc, P.pos.x, P.pos.y, P.pos.x + P.size, P.pos.y + P.size, P.size, P.size);
			::SelectObject(dc, old);
			_DeleteObject(b);
		}
		m_image.ReleaseDC();
	}
	/// コマ
	struct TPiece
	{
		POINT		pos;		///< 座標
		POINT		vector;		///< 方向
		COLORREF	color;		///< 色
		LONG		size;		///< サイズ
		void Init(const SIZE& sz, int sizeMax = 4)
		{
			sizeMax /= 2;
			size = (::rand() % sizeMax) + sizeMax;
			pos.x = ::rand() % (sz.cx - size);
			pos.y = ::rand() % (sz.cy - size);
			color = RGB(::rand() & 0xFF, ::rand() & 0xFF, ::rand() & 0xFF);
			vector.x = ((::rand() % 7) + 1) * ((::rand() & 1) == 0 ? 1 : -1);
			vector.y = ((::rand() % 7) + 1) * ((::rand() & 1) == 0 ? 1 : -1);
		}
		void Next(const SIZE& sz)
		{
			pos.x += vector.x;
			if ( pos.x < 0 ) 
			{
				pos.x = -pos.x;
				vector.x = -vector.x;
			}
			else if ( pos.x > sz.cx - size )
			{
				pos.x -= (pos.x - (sz.cx - size)) * 2;
				vector.x = -vector.x;
			}
			pos.y += vector.y;
			if ( pos.y < 0 )
			{
				pos.y = -pos.y;
				vector.y = -vector.y;
			}
			else if ( pos.y > sz.cy - size)
			{
				pos.y -= (pos.y - (sz.cy - size)) * 2;
				vector.y = -vector.y;
			}
		}

	};
	CWorkMemT<TPiece>	m_pieces;		///< コマ情報
	SIZE				m_size;			///< 画像サイズ
	CBitmapImage		m_blackImage;	///< 背景イメージ
	CBitmapImage		m_image;		///< イメージ
};



};
