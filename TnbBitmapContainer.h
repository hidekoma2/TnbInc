#pragma once
/**
 *@file
 * ビットマップ描画コンテナ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawingContainer.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE BITMAP
 * ビットマップ描画コンテナクラス
 *
 *		{@link IDrawable 描画情報}(含むビットマップ)と座標を対に複数保持するクラスです。
 *
 *	@see CDrawingContainer
 *
 *	@date 07/11/01 新規作成
 *	@date 07/11/17 CDrawContainer と一本化。本クラスは廃止予定。
 */
class CBitmapContainer : public CDrawingContainer
{
public:
	/// コンストラクタ
	CBitmapContainer(void)
	{
	}
};



};///TNB

