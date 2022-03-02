#pragma once
/**
 *@file
 * 最新 WINVER 関係のヘッダ.
 *
 *	XP 以降のみ有効な API を使用しているクラスを使用する場合、、
 *		以下のように、他のIncludeより先に TnbHotWinver.h をインクルードしてください。
 *		\code
 *		// stdafx.h : 標準のシステム インクルード ファイル、
 *		//            または参照回数が多く、かつあまり変更されない
 *		//            プロジェクト専用のインクルード ファイルを記述します。
 *			: 
 *			:
 *		#if _MSC_VER > 1000
 *		#pragma once
 *		#endif // _MSC_VER > 1000
 *
 *		#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。
 *
 *		#include <TnbHotWinver.h>		//★★★★ ここに追加する。
 *
 *		#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
 *		#include <afxext.h>         // MFC の拡張部分
 *			:
 *			:
 *		\endcode
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#if _MSC_VER < 1300
 #define CFileDialog CFileDialog2
#endif

#if defined(WINVER)
 #if WINVER < 0x0501
  #undef WINVER
 #endif
#endif

#if ! defined(WINVER)
 #define WINVER 0x0501
#endif
#define _WIN32_WINNT 0x0501

#endif


