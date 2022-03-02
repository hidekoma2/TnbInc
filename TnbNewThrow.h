#pragma once
/**
 *@file
 *@ingroup NEWDEL
 * メモリ確保例外関係のヘッダ.
 *
 *	@attention 本ヘッダは、インクルードするだけで影響があります。
 *
 *		本ヘッダをインクルードすると、new / malloc() でのメモリ確保失敗時に、
 *		std::bad_alloc 例外スローするようになります。
 *
 *	@note	デバッグビルド時、マクロ "_TnbNEW_Log_ON" が定義されていると、new / delete のログも表示されます。
 *	
 *	@note	TnbDef.h はインクルードしていません。
 *
 *	@note placement new は「 PlacementNew() 」を使います。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#ifdef _DEBUG

//=== デバッグビルド時

#include "TnbNewDebug.h"


#else

//=== リリースビルド時

#include <new.h>
#include <new>

namespace TNB{
namespace NEW
{

class CThrowNew
{
	/// メモリの割り当てに失敗した場合に呼び出す例外処理関数
	static int HandleProgramMemoryDepletion(size_t)
	{
		throw std::bad_alloc();
	}
public:
	CThrowNew(void)
	{
		::_set_new_handler(HandleProgramMemoryDepletion);
		::_set_new_mode(1);
	}
};

namespace
{
	CThrowNew s_cThrowNew;
};

}; //NEW
}; //TNB


#endif // _DEBUG

#endif //_TnbDOXYGEN
