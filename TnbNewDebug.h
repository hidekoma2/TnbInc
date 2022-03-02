#pragma once
/**
 *@file
 *@ingroup NEWDEL
 * 開発用メモリ状態管理関係のヘッダ.
 *
 *	@attention 本ヘッダは、インクルードするだけで影響があります。
 *
 *		本ヘッダをインクルードすると、new / malloc() を置き換え、
 *		プロセス終了時に、メモリ解放もれを確認することが出来ます。
 *		また、new / malloc() でのメモリ確保失敗時に、 std::bad_alloc 例外スローするようになります。
 *
 *	@note	マクロ "_TnbNEW_Log_ON" が定義されていると、new / delete のログも表示されます。
 *			ログに書かれたメモリ確保ナンバーから、不正アクセスの領域も確定できます。
 *	
 *	@note	TnbDef.h はインクルードしていません。
 *
 *	@attention デバッグバージョン以外では機能しません。
 *	@note placement new は「 PlacementNew() 」を使います。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#if defined(_DEBUG) && !defined(_WIN32_WCE)



#include "TnbNewPlacement.h"



#include <malloc.h>
#include <windows.h>
#include <crtdbg.h>
#include <new>
#include <memory>
#include <stdio.h>



#ifndef _TnbDOXYGEN	//Document作成用シンボル


#if _MSC_VER < 1400
 #define _TnbStrFmt sprintf
#else
 #define _TnbStrFmt sprintf_s
#endif

namespace TNB {
///TNB 'New-Delete' Library
namespace NEW {

	// Hookアドレス
	_declspec(selectany) _CRT_ALLOC_HOOK gpAllocHook = NULL;
	// 自動チェッククラス
	struct CCrtDbgAutoCheck
	{
		// コンストラクタ
		CCrtDbgAutoCheck(void)
		{
			int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			tmpFlag |= _CRTDBG_LEAK_CHECK_DF;	//終了時に LeakCheckを出力
			_CrtSetDbgFlag(tmpFlag);
			#ifdef _TnbNEW_Log_ON
				if ( gpAllocHook == NULL )
				{
					//メモリ確保/解放処理時にコールされる関数指定。
					gpAllocHook = _CrtSetAllocHook(MyAllocHook);
				}
			#endif
		}
		//メモリ確保/解放処理時にコールされる関数指定。
		static int __cdecl MyAllocHook(int iAllocType, LPVOID P, size_t nSize, 
										int iBlockType, long lRequestNo, const unsigned char* lpszFile, int iLine)
		{
			if ( iAllocType == _HOOK_FREE && iBlockType == _NORMAL_BLOCK )
			{
				long* L = static_cast<long*>(P);
				char buf[1024];
				_TnbStrFmt(buf, "mem{%d} Free / addr=0x%08X / size=%d\n", L[-2], P, L[-4]);
				::OutputDebugStringA(buf);
			}
			return TRUE;
		}
	};

	// メモリ確保
	inline LPVOID MemAlloc(size_t nSize, LPCSTR lpszFileName, int nLine)
	{
		static TNB::NEW::CCrtDbgAutoCheck sAutoCheck;
		LPVOID P = _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
		#ifdef _TnbNEW_Log_ON
			if ( P != NULL )
			{
				long lRequestNo = -1;
				::_CrtIsMemoryBlock(P, nSize, &lRequestNo, NULL, NULL);
				char buf[1024];
				_TnbStrFmt(buf, "%s(%d): mem{%d} alloc\n", lpszFileName, nLine, lRequestNo);
				::OutputDebugStringA(buf);
				_TnbStrFmt(buf, "mem{%d} Alloc / addr=0x%08X / size=%d\n", lRequestNo, P, nSize);
				::OutputDebugStringA(buf);
			}
			else
			{
				char buf[1024];
				_TnbStrFmt(buf, "%s(%d): mem.alloc error\n", lpszFileName, nLine);
				::OutputDebugStringA(buf);
			}
		#endif // _TnbNEW_Log_ON
		if ( P == NULL )
		{
			_CrtDbgReport(_CRT_ERROR, lpszFileName, nLine, NULL, "メモリ確保に失敗しました。");
			throw std::bad_alloc();
		}
		return P;
	}
};//NEW
};//TNB



/// newオーバーロード
inline LPVOID __cdecl operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	return TNB::NEW::MemAlloc(nSize, lpszFileName, nLine);
}



/// delete オーバーロード
inline void __cdecl operator delete(LPVOID P, LPCSTR lpszFileName, int nLine)
{
	_free_dbg(P, _NORMAL_BLOCK);
}



#ifndef DEBUG_NEW
 #define DEBUG_NEW new(__FILE__, __LINE__)
#endif
#ifndef new
 #define new DEBUG_NEW
#endif
#define malloc(SIZE) TNB::NEW::MemAlloc(SIZE, __FILE__, __LINE__)



#endif	//Document作成用シンボル



#endif	//_DEBUG
