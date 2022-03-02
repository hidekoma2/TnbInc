#pragma once
/**
 *@file
 * ビットマップワイパー関係のヘッダ
 *
 *	各種ワイパーがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapAnimater.h"



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * 縮小ワイプ処理クラス
 *
 *	先の画像を、二倍から指定の大きさにしきます。
 *
 *	@par必要ファイル
 *			TnbBitmapWipers.h
 *
 *	@date 07/11/30 新規作成
 */
class CShrinkWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
public:
	
	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const { return new CShrinkWiper(); }

	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵.
	 *	@param overBmp 先絵.元絵と同じ大きさ以外は結果保障外になります。
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		const SIZE& size = bi.GetSize();
		int cx = size.cx * (max - par) / max;
		int cy = size.cy * (max - par) / max;
		bi.Insert(-cx, -cy, overBmp, SRCCOPY, size.cx + cx * 2, size.cy + cy * 2);
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * スライドワイプ処理クラス
 *
 *	先の画像を、指定の方向からスライドして中央に来るようにします。
 *
 *	@par必要ファイル
 *			TnbBitmapWipers.h
 *
 *	@date 07/11/30 新規作成
 */
class CSlideWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
	UINT	m_pos;	///< ポジション
public:
	
	/**
	 * コンストラクタ
	 *	@param uPos どちらの方向からスライドしてくるか。 １～４、６～９。３が右下。８が上中。
	 */
	CSlideWiper(UINT uPos = 5) : m_pos(uPos)
	{
	}

	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const { return new CSlideWiper(m_pos); }

	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵.
	 *	@param overBmp 先絵.元絵と同じ大きさ以外は結果保障外になります。
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		const SIZE& size = bi.GetSize();
		int cx = size.cx * (max - par) / max;
		int cy = size.cy * (max - par) / max;
		if ( m_pos >= 7 ) 
		{
			cy = -cy; 
		}
		else if ( m_pos >= 4 ) 
		{
			cy = 0;
		}
		switch ( m_pos % 3 )
		{
		case 2: cx = 0; break;
		case 1: cx = -cx; break;
		}
		bi.Insert(cx, cy, overBmp, SRCCOPY);
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * マスク型抽象ワイプ処理クラス
 *
 *		マスクパターンを使って、切り替えるワイパーの抽象クラスです。
 *
 *	@note Clone() と CreateMaskRgn() を実装する必要があります。
 *
 *	@par必要ファイル
 *			TnbBitmapWipers.h
 *
 *	@date 07/12/04 新規作成
 */
class CAbstractMaskWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
protected:
	/**
	 * [作成] マスク領域作成. 
	 *		par が 0なら領域なし、 par が max なら 全領域
	 *	@param size サイズ.
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@retval NULL 領域なし
	 *	@retval NULL以外 領域
	 */
	virtual HRGN CreateMaskRgn(const SIZE& size, UINT par, UINT max) const = 0;

public:
	/**
	 * [変換] ワイプ. 
	 *		baseBmp の上に overBmp を書いた物を返します。
	 *		par が 0なら baseBmp そのもの、 par が max なら overBmp そのものになります
	 *	@param baseBmp 元絵.
	 *	@param overBmp 先絵.
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@return 変換後の絵
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		if ( ! bi.IsEmpty() )
		{
			HRGN hRgn = CreateMaskRgn(bi.GetSize(), par, max);
			if ( hRgn != NULL )
			{
				HDC dc = bi.GetDC();
				::SelectClipRgn(dc, hRgn);
				CBitmapImage b(overBmp);
				b.Draw(dc, 0, 0);
				::SelectClipRgn(dc, NULL);
				bi.ReleaseDC();
				_DeleteObject(hRgn);
			}
		}
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * スライドマスクワイプ処理クラス
 *
 *	先の画像を、指定の方向から見えてくるようになります。
 *
 *	@par必要ファイル
 *			TnbBitmapWipers.h
 *
 *	@date 07/12/04 新規作成
 */
class CSlideMaskWiper : public CAbstractMaskWiper
{
	DEFSUPER(CAbstractMaskWiper);
	UINT	m_pos;	/// 2,4,6,8
protected:
	/**
	 * [作成] マスク領域作成. 
	 *		par が 0なら領域なし、 par が max なら 全領域
	 *	@param size サイズ.
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@retval NULL 領域なし
	 *	@retval NULL以外 領域
	 */
	virtual HRGN CreateMaskRgn(const SIZE& size, UINT par, UINT max) const
	{
		HRGN rgn = ::CreateRectRgn(0, 0, size.cx, size.cy);
		int cx = size.cx * (max - par) / max;
		int cy = size.cy * (max - par) / max;
		if ( m_pos >= 7 ) 
		{
			cy = -cy; 
		}
		else if ( m_pos >= 4 ) 
		{
			cy = 0;
		}
		switch ( m_pos % 3 )
		{
		case 2: cx = 0; break;
		case 1: cx = -cx; break;
		}
		::OffsetRgn(rgn, cx, cy);
		return rgn;
	}

public:

	/**
	 * コンストラクタ
	 *	@param p どちらの方向から見えるようにするか。 １～４、６～９。３が右下。８が上中。
	 */
	CSlideMaskWiper(UINT p) : m_pos(p)
	{
	}

	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const 
	{
		return new CSlideMaskWiper(m_pos); 
	}
};



/**@ingroup ANIMATE
 * スライドマスクワイプ処理クラス
 *
 *	先の画像を、ランダムのポイントからチリチリと切り替わるようになります。
 *
 *	@par必要ファイル
 *			TnbBitmapWipers.h
 *
 *	@date 07/12/04 新規作成
 */
class CRandomMaskWiper : public CAbstractMaskWiper
{
	DEFSUPER(CAbstractMaskWiper);
	size_t				m_size;		///< 1以上。 この数×この数のマスクを作ります。
	mutable LPRGNDATA	m_pRgnData;	///< マスクデータ
	mutable CWorkMem	m_rgnWork;
protected:

	/**
	 * [作成] マスク領域作成. 
	 *		par が 0なら領域なし、 par が max なら 全領域
	 *	@param size サイズ.
	 *	@param par 進捗(分子)
	 *	@param max 全体(分母)
	 *	@retval NULL 領域なし
	 *	@retval NULL以外 領域
	 */
	virtual HRGN CreateMaskRgn(const SIZE& size, UINT par, UINT max) const
	{
		size_t len = m_size * m_size;
		if ( m_pRgnData == NULL )
		{
			m_rgnWork.Resize(sizeof(RGNDATAHEADER) + sizeof(RECT) * len);
			m_pRgnData = reinterpret_cast<LPRGNDATA>(m_rgnWork.Ref());
			LPRECT pRect = reinterpret_cast<LPRECT>(m_pRgnData->Buffer);
			int i = 0;
			loop ( y, m_size )
			{
				loop ( x, m_size )
				{
					::SetRect(&pRect[i++], ToInt(x), ToInt(y), ToInt(x + 1), ToInt(y + 1));
				}
			}
			loop ( i, len )
			{
				INDEX m = rand() % len;
				Swap(pRect[i], pRect[m]);
			}
			m_pRgnData->rdh.dwSize			= sizeof(RGNDATAHEADER);
			m_pRgnData->rdh.iType			= RDH_RECTANGLES;
			m_pRgnData->rdh.rcBound.left	= 0;
			m_pRgnData->rdh.rcBound.top		= 0;
			m_pRgnData->rdh.rcBound.right	= size.cx;
			m_pRgnData->rdh.rcBound.bottom	= size.cy;
		}
		//
		size_t c = len * par / max;
		if ( c == 0 ) 
		{
			return NULL; 
		}
		m_pRgnData->rdh.nRgnSize = ToDword(sizeof(RGNDATAHEADER) + sizeof(RECT) * c);
		m_pRgnData->rdh.nCount = ToDword(c);
		HRGN hTmpRgn = ::ExtCreateRegion(NULL, m_pRgnData->rdh.nRgnSize, m_pRgnData);
		HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
		loop ( x, (size.cx / m_size) + 1 )
		{
			::CombineRgn(hRgn, hRgn, hTmpRgn, RGN_OR);
			::OffsetRgn(hTmpRgn, ToInt(m_size), 0);
		}
		::CombineRgn(hTmpRgn, hRgn, NULL, RGN_COPY);
		loop ( y, (size.cy / m_size) )
		{
			::OffsetRgn(hTmpRgn, 0, ToInt(m_size));
			::CombineRgn(hRgn, hRgn, hTmpRgn, RGN_OR);
		}
		_DeleteObject(hTmpRgn);
		return hRgn;
	}

public:

	/**
	 * コンストラクタ
	 *	@param size ランダムタイルの大きさを指定します。通常指定する必要はありません。
	 */
	CRandomMaskWiper(size_t size = 32) : m_size(size), m_pRgnData(NULL)
	{
		ASSERT0( size > 1, "CRandomMaskWiper", "1より大きい数値を指定してください。" );
	}

	/// デストラクタ
	~CRandomMaskWiper(void)
	{
		if ( m_pRgnData != NULL )
		{
			m_pRgnData = NULL;
			m_rgnWork.Free();
		}
	}

	/**
 	 * [複製] クローン
	 *	@return クローン。不必要になったら delete する必要があります。
	 */
	virtual IBitmapWipable* Clone(void) const 
	{
		return new CRandomMaskWiper(m_size); 
	}
};



}; //TNB
