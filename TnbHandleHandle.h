#pragma once
/**
 *@file
 * �n���h���n���h���֌W�̃w�b�_
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		��̃n���h�����n���h�����܂��B auto_ptr �N���X�̂悤�Ȃ��̂ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// PointerHandleBase�p �ėp�n���h��Close�N���X
struct TPhCloseHandle
{
	void operator()(HANDLE P) { ::CloseHandle(P); }
};
/// PointerHandleBase�p �ėp�n���h��Delete�N���X
struct TPhDeleteGdiObject
{
	void operator()(HGDIOBJ P) { _DeleteObject(P); }
};

#endif // _TnbDOXYGEN



/**@ingroup WINAPI
 * HANDLE�^�n���h���n���h��
 *
 *		HANDLE �A����Ǘ����܂��B�Q�Ƃ���{�C���X�^���X���Ȃ��Ȃ�ƁA�����I�� CloseHandle() ���܂��B
 *
 *	@note Null ��Ԃ̒l�́A INVALID_HANDLE_VALUE (-1) �ɂȂ�܂��B NULL (0) �ł͂���܂���B
 *
 *	@see http://www.microsoft.com/japan/developer/library/jpwinpf/_win32_closehandle.htm
 *
 *	@par�K�v�t�@�C��
 *			TnbHandleHandle.h
 */
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
typedef TNB::CPointerHandleBaseT<HANDLE, TPhCloseHandle, (INT_PTR)INVALID_HANDLE_VALUE> CHandleHandle;
#else
 class CHandleHandle : public CPointerHandleBaseT<HANDLE, TPhCloseHandle, (INT_PTR)INVALID_HANDLE_VALUE>
 {typedef aa __a;};
#endif



/**@ingroup WINAPI
 * HBRUSH�^�n���h���n���h��
 *
 *		HBRUSH �A����Ǘ����܂��B�Q�Ƃ���{�C���X�^���X���Ȃ��Ȃ�ƁA�����I�� DeleteObject() ���܂��B
 *
 *	@see http://www.microsoft.com/japan/developer/library/jpwinpf/_win32_destroyobject.htm
 *
 *	@par�K�v�t�@�C��
 *			TnbHandleHandle.h
 */
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
typedef TNB::CPointerHandleBaseT<HBRUSH, TPhDeleteGdiObject> CBrushHandle;
#else
 class CBrushHandle : public CPointerHandleBaseT<HBRUSH, TPhDeleteGdiObject>
 {typedef aa __a;};
#endif



};//TNB
