#pragma once
/**
 *@file
 * ハンドルハンドル関係のヘッダ
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		一つのハンドルをハンドルします。 auto_ptr クラスのようなものです。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// PointerHandleBase用 汎用ハンドルCloseクラス
struct TPhCloseHandle
{
	void operator()(HANDLE P) { ::CloseHandle(P); }
};
/// PointerHandleBase用 汎用ハンドルDeleteクラス
struct TPhDeleteGdiObject
{
	void operator()(HGDIOBJ P) { _DeleteObject(P); }
};

#endif // _TnbDOXYGEN



/**@ingroup WINAPI
 * HANDLE型ハンドルハンドル
 *
 *		HANDLE 、一つを管理します。参照する本インスタンスがなくなると、自動的に CloseHandle() します。
 *
 *	@note Null 状態の値は、 INVALID_HANDLE_VALUE (-1) になります。 NULL (0) ではありません。
 *
 *	@see http://www.microsoft.com/japan/developer/library/jpwinpf/_win32_closehandle.htm
 *
 *	@par必要ファイル
 *			TnbHandleHandle.h
 */
#ifndef _TnbDOXYGEN	//Document作成用シンボル
typedef TNB::CPointerHandleBaseT<HANDLE, TPhCloseHandle, (INT_PTR)INVALID_HANDLE_VALUE> CHandleHandle;
#else
 class CHandleHandle : public CPointerHandleBaseT<HANDLE, TPhCloseHandle, (INT_PTR)INVALID_HANDLE_VALUE>
 {typedef aa __a;};
#endif



/**@ingroup WINAPI
 * HBRUSH型ハンドルハンドル
 *
 *		HBRUSH 、一つを管理します。参照する本インスタンスがなくなると、自動的に DeleteObject() します。
 *
 *	@see http://www.microsoft.com/japan/developer/library/jpwinpf/_win32_destroyobject.htm
 *
 *	@par必要ファイル
 *			TnbHandleHandle.h
 */
#ifndef _TnbDOXYGEN	//Document作成用シンボル
typedef TNB::CPointerHandleBaseT<HBRUSH, TPhDeleteGdiObject> CBrushHandle;
#else
 class CBrushHandle : public CPointerHandleBaseT<HBRUSH, TPhDeleteGdiObject>
 {typedef aa __a;};
#endif



};//TNB
