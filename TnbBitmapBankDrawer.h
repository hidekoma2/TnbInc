#pragma once
/**
 *@file
 * ビットマップバンク描画情報関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBankDrawer.h"
#include "TnbBitmapImage.h"
#include "TnbBitmapDrawer.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE BITMAP
 * ビットマップバンク描画情報クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		LPARAM により、描画情報を管理します。
 *
 *	@par必要ファイル
 *			TnbBitmapBankDrawable.h
 *
 *	@date 08/10/06 新規作成
 */
class CBitmapBankDrawer : public CBankDrawer
{
	DEFSUPER(CBankDrawer);
public:

	/// コンストラクタ
	CBitmapBankDrawer(void)
	{
	}

	/**
	 * コンストラクタ
	 *	@param bmp ビットマップ
	 *	@param num 分割数。bmp を分解して保持します。
	 *	@param color 透過する色。 CLR_INVALID を指定すると、透過色はなし。
	 */
	CBitmapBankDrawer(CBitmapHandle bmp, size_t num = 1, COLORREF color = CLR_INVALID)
	{
		CBitmapImage bi = bmp;
		SIZE sz = bi.GetSize();
		size_t w = sz.cx / num;
		loop ( i, num )
		{
			RECT rc = { ToInt(i * w), 0, ToInt(i * w + w), sz.cy };
			m_phDraws.Add(new CSemitransparentBitmapDrawer(bi.Cut(rc), 100, color));
		}
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		CBitmapBankDrawer* P = new CBitmapBankDrawer(*this);
		return P;
	}
};



};///TNB

