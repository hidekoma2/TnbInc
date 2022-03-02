#pragma once
/**
 *@file
 * TNBライブラリの定義ヘッダ
 *
 *	すべてのヘッダにインクルードされています。
 *
 *	例外 "TnbNewDebug.h" , "TnbNewPlacement.h" ,"TnbNewThrow.h"
 *
 *	@note	マクロ "_TnbUSINGNAMESPACE_DISABLE" が定義されていると、
 *			TNB 以下の using namespace を行ないません。
 *
 * \code
 * -------------------------------------------------------------------
 *	
 *	　TNB Library Project For Win32
 *	 
 *	　Copyright (C) 2001-2019 by TNB製作所 - Hide Komatsu
 *	 
 *	　上記者は、TNB Library（以下、本ライブラリと記す）の著作権を保持しま
 *	　すが、以下の条件を満たす場合に限り、本ライブラリの使用・複製・改変・
 *	　再配布することを無償で許諾します。
 *	　(1) 本ライブラリをソースコードの形で利用する場合は、上記の著作権表
 *	　　　示、この利用条件、および下記の無保証規定が、そのままの形でソー
 *	　　　スコード中に含まれていること。
 *	　(2) 本ライブラリを使用したバイナリ形式（本ライブラリを再利用が出来
 *	　　　ないあらゆる形を含む）で再配布する場合には、再配布に伴うドキュ
 *	　　　メント（利用者マニュアルなど）に、上記の著作権表示、この利用条
 *	　　　件および下記の無保証規定を掲載すること。
 *	　(3) 本ライブラリの利用により直接的または間接的に生じるいかなる損害
 *	　　　からも、上記著作権者を免責すること。
 *	 
 *	　本ライブラリは，無保証で提供しているものです。上記著作権者は、本ラ
 *	　イブラリに関して、いかなる保証も行ないません。また、本ライブラリの
 *	　利用により直接的または間接的に生じたいかなる損害に関しても、その責
 *	　任を負いません。
 *	
 * -------------------------------------------------------------------
 * \endcode
 */



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#pragma warning( disable : 4239 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4097 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4456 )
#pragma warning( disable : 4840 )
#pragma warning( disable : 4458 )
#pragma warning( disable : 4457 )

#ifdef _TnbPGRELIEF	//Document作成用シンボル
	#include <afxwin.h>
	#include <afxext.h>
#endif

#include <windows.h>
#include <winnt.h>
#include <tchar.h>
#include <stdio.h>
#include <limits.h>
#ifndef _WIN32_WCE
 #if _MSC_VER < 1923
 //====VC6.0以下
  #include <typeinfo.h>	//	typeid(this).name();
 #else
  #include <vcruntime_typeinfo.h>
 #endif
 #define GetProcAddressA GetProcAddress
#endif

#ifdef _TnbDEBUG_ON
 #define TTRACE0(XX)			TRACE0(XX)
 #define TTRACE1(XX,P1)			TRACE1(XX,P1)
 #define TTRACE2(XX,P1,P2)		TRACE2(XX,P1,P2)
 #define TTRACE3(XX,P1,P2,P3)	TRACE3(XX,P1,P2,P3)
 #define TTRACE1A(XX,P1)		TRACE1A(XX,P1)
 #define TTRACE2A(XX,P1,P2)		TRACE2A(XX,P1,P2)
 #define TTRACE3A(XX,P1,P2,P3)	TRACE3A(XX,P1,P2,P3)
#else
 #define TTRACE0(XX)
 #define TTRACE1(XX,P1)
 #define TTRACE2(XX,P1,P2)
 #define TTRACE3(XX,P1,P2,P3)
 #define TTRACE1A(XX,P1)
 #define TTRACE2A(XX,P1,P2)
 #define TTRACE3A(XX,P1,P2,P3)
#endif

#ifndef __AFX_H__
 #ifndef _DEBUG
  #define DEBUG_ONLY(XX)
  #define TRACE0(XX)
  #define TRACE1(XX,P1)
  #define TRACE2(XX,P1,P2)
  #define TRACE3(XX,P1,P2,P3)
 #else
  #define DEBUG_ONLY(XX)	(XX)
  #define TRACE0(XX)		::OutputDebugString(_T(XX))
  #define TRACE1(XX,P1)		do { TCHAR* _b=new TCHAR[1024]; TNB::STRLIB::PrintF(_b,1024,_T(XX),P1); ::OutputDebugString(_b); delete[] _b; } while ( false )
  #define TRACE2(XX,P1,P2)		do { TCHAR* _b=new TCHAR[1024]; TNB::STRLIB::PrintF(_b,1024,_T(XX),P1,P2); ::OutputDebugString(_b); delete[] _b; } while ( false )
  #define TRACE3(XX,P1,P2,P3)	do { TCHAR* _b=new TCHAR[1024]; TNB::STRLIB::PrintF(_b,1024,_T(XX),P1,P2,P3); ::OutputDebugString(_b); delete[] _b; } while ( false )
 #endif
 #ifndef ASSERT
  #define ASSERT(XX)
 #endif
 #ifndef VERIFY
  #define VERIFY(XX) (XX)
 #endif
#endif


#if defined(_DEBUG)
 #if !defined(_WIN32_WCE)
  #include <crtdbg.h>
  #define ASSERT_REP(func,msg)	_CrtDbgReport(_CRT_ASSERT,__FILE__,__LINE__,func,msg)
  #define ASSERT_CHK(func,msg) \
		do { if ( ASSERT_REP(func,msg)==1 ) { DebugBreak(); } } while ( false )
  #define ASSERT0(expr,func,msg) \
		do { if ( !(expr) ) { ASSERT_CHK(func,msg); } } while ( false )
  #define ASSERT1(expr,func,msg,p1) \
		do { if ( !(expr) ) { char* _b=new char[1024]; STRLIB::PrintF(_b,1024,msg,p1); ASSERT_CHK(func,_b); delete[] _b; } } while ( false )
  #define ASSERT2(expr,func,msg,p1,p2) \
		do { if ( !(expr) ) { char* _b=new char[1024]; STRLIB::PrintF(_b,1024,msg,p1,p2); ASSERT_CHK(func,_b); delete[] _b; } } while ( false )
  #define ASSERT3(expr,func,msg,p1,p2,p3) \
		do { if ( !(expr) ) { char* _b=new char[1024]; STRLIB::PrintF(_b,1024,msg,p1,p2,p3); ASSERT_CHK(func,_b); delete[] _b; } } while ( false )
 #else
  #ifdef __AFX_H__
   #include <altcecrt.h>
   #define ASSERT_REP(func,msg)	(OutputDebugString(msg), OutputDebugString(_T("\r\n")), ::AfxAssertFailedLine(__FILE__,__LINE__))
  #else
   #define ASSERT_REP(func ,msg) (OutputDebugString(msg), OutputDebugString(_T("\r\n")), DBGCHK(func, 0), 1)
  #endif
  #define ASSERT_CHK(func,msg) \
		do { if ( ASSERT_REP(func,msg)==1 ) { DebugBreak(); } } while ( false )
  #define ASSERT0(expr,func,msg) \
		do { if ( !(expr) ) { ASSERT_CHK(func,_T(msg)); } } while ( false )
  #define ASSERT1(expr,func,msg,p1) \
		do { if ( !(expr) ) { TCHAR* _b=new TCHAR[1024]; STRLIB::PrintF(_b,1024,_T(msg),p1); ASSERT_CHK(func,_b); delete[] _b; } } while ( false )
  #define ASSERT2(expr,func,msg,p1,p2) \
		do { if ( !(expr) ) { TCHAR* _b=new TCHAR[1024]; STRLIB::PrintF(_b,1024,_T(msg),p1,p2); ASSERT_CHK(func,_b); delete[] _b; } } while ( false )
  #define ASSERT3(expr,func,msg,p1,p2,p3) \
		do { if ( !(expr) ) { TCHAR* _b=new TCHAR[1024]; STRLIB::PrintF(_b,1024,_T(msg),p1,p2,p3); ASSERT_CHK(func,_b); delete[] _b; } } while ( false )
 #endif

 #define ASSERTLIB(XX) ASSERT0(XX, "TNB Library", "TNB Library のバグの可能性があります。")
 #define VERIFYLIB(XX) ASSERTLIB(XX)
 #undef ASSERT
 #define ASSERT(XX) ASSERT1(XX, NULL, "[%s]\n\n上記箇所でアサートしました。", _T(#XX))
 #if !defined(_WIN32_WCE)
  #define TRACE1A(XX,P1)		do { char* _b=new char[1024]; TNB::STRLIB::PrintF(_b,1024,XX,P1); ::OutputDebugStringA(_b); delete[] _b; } while ( false )
  #define TRACE2A(XX,P1,P2)		do { char* _b=new char[1024]; TNB::STRLIB::PrintF(_b,1024,XX,P1,P2); ::OutputDebugStringA(_b); delete[] _b; } while ( false )
  #define TRACE3A(XX,P1,P2,P3)	do { char* _b=new char[1024]; TNB::STRLIB::PrintF(_b,1024,XX,P1,P2,P3); ::OutputDebugStringA(_b); delete[] _b; } while ( false )
 #else
  #define TRACE1A(XX,P1)
  #define TRACE2A(XX,P1,P2)
  #define TRACE3A(XX,P1,P2,P3)
 #endif
#else
 #define ASSERT0(expr,func,msg)
 #define ASSERT1(expr,func,msg,p1)
 #define ASSERT2(expr,func,msg,p1,p2)
 #define ASSERT3(expr,func,msg,p1,p2,p3)
 #define ASSERTLIB(XX)
 #define VERIFYLIB(XX) (XX)
 #define TRACE1A(XX,P1)
 #define TRACE2A(XX,P1,P2)
 #define TRACE3A(XX,P1,P2,P3)
#endif

#if defined(_DEBUG)
 #define _GetLastError(XX) TNB::_LastErrorTrace(_T(__FILE__),__LINE__,_T(XX))
#else
 #define _GetLastError(XX) ::GetLastError()
#endif

#define _BDMSG0(x)	#x
#define _BDMSG(x)	_BDMSG0(x)

// プロセスで唯一宣言
#define _SELECTANY _declspec(selectany) 

#ifndef countof
 #define countof(X) (sizeof(X) / sizeof((X)[0]))
#endif

/*
VC6.0 : 1200
VC7.0 : 1300
VC7.1 : 1310
VC8.0 : 1400
*/
#if _MSC_VER < 1300
 //====VC6.0以下
 #define __MSVCPP6__
 #define __noop (void)0
 #define for if ( false ); else for // for内の変数宣言を標準にする
 #define DEFSUPER(XX) typedef XX _super; typedef XX __super
 #define _deprecated
 #define DEFPARENTLISTENER(PARENT, INST) typedef IListener INST
 #if defined(UNICODE) || defined(_UNICODE)
  #ifndef _USRDLL
   #pragma comment(linker, "/entry:\"wWinMainCRTStartup\"")
  #endif
 #endif
#else
 //====VC7.0以上
 #define DEFSUPER(XX) typedef XX _super
 #define _deprecated __declspec(deprecated)
 #define DEFPARENTLISTENER(PARENT, INST) typedef PARENT::IListener INST
 #if defined(__AFX_H__) && defined(_UNICODE) && defined(_DEBUG)
  //== MFC & UNICODE & DEBUG
  #include <wchar.h>
  #undef TRACE0
  #undef TRACE1
  #undef TRACE2
  #undef TRACE3
  #define TRACE0(XX)		 ::OutputDebugStringW(_T(XX))
  #define TRACE1(XX,P1)			do { WCHAR* _b=new WCHAR[1024]; TNB::STRLIB::PrintF(_b,1024,_T(XX),P1); ::OutputDebugStringW(_b); delete[] _b; } while ( false )
  #define TRACE2(XX,P1,P2)		do { WCHAR* _b=new WCHAR[1024]; TNB::STRLIB::PrintF(_b,1024,_T(XX),P1,P2); ::OutputDebugStringW(_b); delete[] _b; } while ( false )
  #define TRACE3(XX,P1,P2,P3)	do { WCHAR* _b=new WCHAR[1024]; TNB::STRLIB::PrintF(_b,1024,_T(XX),P1,P2,P3); ::OutputDebugStringW(_b); delete[] _b; } while ( false )
 #endif
 #if _MSC_VER >= 1400
  //===VC8.0以上
  #pragma warning ( disable : 4996 )
 #endif
#endif

#ifdef _MANAGED
 #pragma warning ( disable : 4793 )
 #pragma comment(lib,"user32.lib")
#endif

#ifdef _WIN32_WCE
 #ifndef WM_PRINTCLIENT
  #define WM_PRINTCLIENT -1
 #endif
#endif

#ifndef WM_THEMECHANGED
 #define WM_THEMECHANGED 0x031A
#endif

#ifndef DWORD_MAX
 #define DWORD_MAX 0xffffffffUL
#endif

#ifndef DWORD_PTR
 #define DWORD_PTR ULONG_PTR
#endif

#ifndef	INVALID_SET_FILE_POINTER
 #define INVALID_SET_FILE_POINTER 0xFFFFFFFF
#endif

#ifndef _WIN64
 typedef unsigned long ULONG_PTR, *PULONG_PTR;
#endif

#ifndef _BASETSD_H_
 typedef int INT_PTR, *PINT_PTR;
#endif

#ifndef WS_EX_LAYERED
 #define WS_EX_LAYERED 0x80000
#endif

typedef size_t INDEX;
#define INVALID_SIZE ((size_t)(-1)) 
#define INVALID_INDEX INVALID_SIZE 
inline bool IsValid(size_t sz) { return sz != INVALID_SIZE; }
inline bool IsValid(HANDLE h) { return h != INVALID_HANDLE_VALUE; }
template<typename TYP> inline bool IsInvalid(TYP t) { return ! IsValid(t); }
#ifndef CLR_INVALID
 #define CLR_INVALID	0xFFFFFFFF
#endif
#define CLR_AUTOSELECT	0xFEFFFFFF
#define IS_RGBVALUE(RGB) (((RGB)&0xFF000000)==0)

#endif //_TnbDOXYGEN



/** 
 * ビルドメッセージ表示
 *	@par使用例
 *		\code
 *			#pragma MESSAGE("コンパイル中") 
 *		\endcode
 *	@param d ビルド中に表示したい文言
 */
#define MESSAGE(d)	message(__FILE__"("_BDMSG(__LINE__)") : message:" d)



/** 
 * BIT演算
 *	@param X 唯一立てたいBitナンバー（０～63）。int 型の変数も可能。
 *	@note 括弧内に定数リテラル入れると定数になります（コンパイル時に値が確定するため）。
 *	@return (1<<X)の定数
 */
#define _BIT(X) (1ui64<<(X))



/** 
 * 二進数表記定数
 *	@param X 二進数表記のリテラル(変数は不可)。11桁(0x7FF)まで対応（12桁以上は warning C4309 が発生）。
 *	@note	0,1以外を括弧内に入れるとエラー、または結果が不明になります。
 *	@note 結果は定数になります（コンパイル時に値が確定するため）。
 *	@par使用例
 *		\code 
 *		int x = _BIN(00011101); // x は 0x1D (29) になる。 
 *		\endcode
 */
#define _BIN(X) (TBinaryNotation<0##X>::VALUE)



/** 
 * loop構文.
 *		0 から CNT-1 まで繰り返します。
 *	@par使用例1
 *		\code 
 *		loop ( i, 10 ){ Foo(i); }	//Fooが 0～9 の10回コールされる。 
 *		\endcode
 *	@par使用例2
 *		\code 
 *		CByteVector vb;
 *		vb.SetSize(5);
 *		loop ( i, vb ){ // operator size_t() が効く
 *			vb[i] = i;	//Fooが 0～4の5回コールされる。 
 *		}
 *		\endcode
 *	@param VAR ループで使う変数の名前。size_t 型で宣言されます。
 *	@param CNT 回数。一度しか評価されません。 ループ中、size_t 型の VAR_size と言う名で保持されます。
 */
#define loop(VAR, CNT) for ( size_t VAR = 0, VAR##_size = CNT; VAR < VAR##_size; VAR++ )

/** 
 * loop構文.
 *		CNT-1 から 0 まで繰り返します。
 *	@par使用例
 *		\code 
 *		loop ( i, 10 ){ Foo(i); }	//Fooが 9～0 の10回コールされる。 
 *		\endcode
 *	@param VAR ループで使う変数の名前。size_t 型で宣言されます。
 *	@param CNT 回数。一度しか評価されません。 ループ中、size_t 型の VAR_size と言う名で保持されます。
 */
#define loop_dn(VAR, CNT) for ( size_t VAR##_r = 0, VAR##_size = CNT, VAR = CNT - 1; VAR##_r < VAR##_size; VAR--, VAR##_r++ )



///TNB Library
namespace TNB
{



/**
 * [複製] メモリコピー
 *	@note 書込み先の型を基準にコピーします。
 *	@note 領域が重なっている場合、動作は未保証です。
 *	@param _pDst	コピー先
 *	@param pSrc	コピー元
 *	@param len	コピー長さ。 _pDstのサイズ× len Byteがコピーされます。
 *	@par必要ファイル
 *			TnbDef.h
 */
template<typename T> 
inline void MemCopy(T* _pDst, const void* pSrc, size_t len)
{
	if ( len != 0 )
	{
		ASSERT( ! ::IsBadReadPtr(pSrc, sizeof(T) * len) );
		ASSERT( ! ::IsBadWritePtr(_pDst, sizeof(T) * len) );
		const T* P = static_cast<const T*>(pSrc);
		while ( (len--) > 0 )
		{
			*_pDst++ = *P++; 
		}
	}
}



/**@ingroup VALUE
 * [設定] ゼロクリア.
 *	@param value ゼロクリア対象変数
 *	@par必要ファイル
 *			TnbDef.h
 */
template<typename V> 
inline void Zero(V& value)
{
	BYTE* B = reinterpret_cast<BYTE*>(&value);
	size_t l = sizeof(V);
	loop ( i, l )
	{
		*B++ = 0;
	}
}



/**@ingroup VALUE
 * [確認] 範囲チェック.
 *		指定の value が ０以上、 size 未満であるかチェックします。
 *	@param value チェック対象値
 *	@param size サイズ
 *	@retval true 範囲内
 *	@retval false 範囲外
 *	@par必要ファイル
 *			TnbDef.h
 */
inline bool IsInRange(INDEX value, size_t size)
{
	return value < size;
}



/**@ingroup VALUE
 * [宣言] 参照しない値宣言.
 *	@param value 対象変数
 *	@par必要ファイル
 *			TnbDef.h
 */
template<typename T> inline void IgnoreUnusedValue(const T& value)
{
}



#ifndef _TnbDOXYGEN	//Document作成用シンボル

// オーバーフローチェック付き、ダウンキャスト
template<typename TYP>
class down_cast
{
public:
	template<typename SRC>
	down_cast(SRC src) : m_typ(static_cast<TYP>(src))
	{
		ASSERT1( (SRC)(m_typ) == src, "down_cast<>", "オーバーフローしました。\nsource = %d", src );
	}
	down_cast(float src) : m_typ(static_cast<TYP>(src))
	{
		ASSERT1( (LONGLONG)(m_typ) == LONGLONG(src), "down_cast<>", "オーバーフローしました。\nsource = %f", src );
	}
	down_cast(double src) : m_typ(static_cast<TYP>(src))
	{
		ASSERT1( (LONGLONG)(m_typ) == LONGLONG(src), "down_cast<>", "オーバーフローしました。\nsource = %f", src );
	}
	operator TYP(void) const
	{
		return m_typ;
	}
private:
	TYP m_typ;
};

/// コンストオブジェクト化
template<typename T> inline const T* Const(T* P)
{
	return const_cast<const T*>(P);
}

/// int 化
template<typename T> inline int ToInt(T t)
{
	return down_cast<int>(t);
}

/// int 化
template<> inline int ToInt(LONGLONG ll)
{
	return down_cast<int>(ll);
}

/// WORD化
template<typename T> inline WORD ToWord(T t)
{
	return down_cast<WORD>(t);
}

/// DWORD化
template<typename T> inline DWORD ToDword(T t)
{
	return down_cast<DWORD>(t);
}

/// INDEX 化
template<typename T> inline INDEX ToIndex(T t)
{
	return down_cast<INDEX>(t);
}

/// 強制代入
template<typename T1, typename T2> inline void _deprecated ForceSet(T1& t1, const T2& t2)
{
	t1 = (const T1)(t2);
}

#ifdef __MSVCPP6__
/// インターロック比較チェンジ.
inline LONG InterlockedCompareExchange(LONG* _plDes, LONG lExchange, LONG lComperand)
{
	typedef LONG (WINAPI* P_CompExc)(LONG*, LONG, LONG);
	P_CompExc pCompExc = reinterpret_cast<P_CompExc>(&::InterlockedCompareExchange);
	return pCompExc(_plDes, lExchange, lComperand);
}
#endif

/// BIN表記用テンプレート
template<UINT n> struct TBinaryNotation {
	enum {
		BIT = n % 8u,
		VALUE = BIT + (TBinaryNotation<n / 8u>::VALUE << 1u)
	};
};
template<> struct TBinaryNotation<0> {
	enum {BIT = 0, VALUE = 0};
};

#ifdef __DEVICERESOLUTIONAWARE_H__
/// DPIX計算
inline int CalcScaleX(int x) { return DRA::SCALEX(x); }
inline int CalcScaleX(int x, HDC dc) { return DRA::SCALEX(x); }
/// DPIY計算
inline int CalcScaleY(int y) { return DRA::SCALEY(y); }
inline int CalcScaleY(int y, HDC dc) { return DRA::SCALEY(y); }
#else
/// DPIX計算
inline int CalcScaleX(int x, HDC dc) 
{
	return static_cast<int>(x * ::GetDeviceCaps(dc, LOGPIXELSX) / 96.0);
}
/// DPIX計算
inline int CalcScaleX(int x) 
{
	HDC dc = ::GetDC(NULL);
	int r = CalcScaleX(x, dc); 
	::ReleaseDC(NULL, dc);
	return r;
}
/// DPIY計算
inline int CalcScaleY(int y, HDC dc)
{
	return static_cast<int>(y * ::GetDeviceCaps(dc, LOGPIXELSY) / 96.0);
}
/// DPIY計算
inline int CalcScaleY(int y)
{
	HDC dc = ::GetDC(NULL);
	int r = CalcScaleY(y, dc);
	::ReleaseDC(NULL, dc);
	return r;
}
#endif
/// DPI計算
inline void CalcScale(RECT& _rc)
{
	_rc.left = CalcScaleX(_rc.left);
	_rc.right = CalcScaleX(_rc.right);
	_rc.top = CalcScaleY(_rc.top);
	_rc.bottom = CalcScaleY(_rc.bottom);
}
/// DPI計算
inline void CalcScale(SIZE& _sz)
{
	_sz.cx = CalcScaleX(_sz.cx);
	_sz.cy = CalcScaleY(_sz.cy);
}
/// DPI計算
inline void CalcScale(POINT& _po)
{
	_po.x = CalcScaleX(_po.x);
	_po.y = CalcScaleY(_po.y);
}

#endif //_TnbDOXYGEN



/**
 * [ETC] コピー不可能スーパークラス.
 *		このクラスを継承するとコピー不可能のインスタンスが作成できます。
 *	@par必要ファイル
 *			TnbDef.h
 *	@date 06/04/01	新規作成。
 */
class CCopyImpossible
{
	/**
	 * コピーコンストラクタ
	 *	@param T コピー元
	 */
	CCopyImpossible(const CCopyImpossible& T);
	/**
	 * [代入] コピーオペレータ
	 *	@param T コピー元
	 */
	void operator=(const CCopyImpossible& T);
public:
	/** デフォルトコンストラクタ */
	CCopyImpossible(void) {}
	/** デストラクタ */
	~CCopyImpossible(void) {}
};



/**@ingroup MEMORY
 * ワークメモリテンプレート.
 *
 *		ワークメモリ確保します。簡易的にメモリを確保できます。
 *		デストラクタで自動的に解放するためメモリリークの心配がありません。
 *	
 *	@par必要ファイル
 *			TnbDef.h
 *	@date 06/09/19	新規
 *	@date 10/01/26	Swap() 追加。
 *	@date 11/08/02	Resize() の動作を改善。
 */
template<typename TYP>
class CWorkMemT : CCopyImpossible
{
	size_t	m_size;			///< サイズ
	TYP*	m_lpBuf;		///< 管理ポインタ
public:
	/**
	 * コンストラクタ.
	 *	@note メモリは確保しません。
	 */
	CWorkMemT(void) : m_size(0), m_lpBuf(NULL) {}
	/**
	 * コンストラクタ
	 *	@param l 確保長さ
	 */
	CWorkMemT(size_t l) : m_size(l), m_lpBuf(new TYP[(l == 0) ? 1 : l]) 
	{}
	/// デストラクタ
	~CWorkMemT(void) { Free(); }
	/// [設定] 解放.
	///		メモリを開放し、サイズを０にします。
	void Free(void)
	{
		if ( m_lpBuf != NULL )
		{
			delete[] m_lpBuf;
			m_lpBuf = NULL;
			m_size = 0;
		}
	}
	/**
	 * [取得] サイズ取得
	 *	@return サイズ
	 */
	size_t GetSize(void) const { return m_size; }
	/**
	 * [設定] サイズ再設定
	 *	@note 確保していたメモリは一度解放されます。
	 *	@note 同じ長さの場合、何もしません。
	 *	@param l 確保長さ
	 */
	void Resize(size_t l)
	{
		if ( l != m_size )
		{
			Free();
			m_size = l;
			if ( l > 0 )
			{
				m_lpBuf = new TYP[l];
			}
		}
	}
	/**
	 * [設定] 再設定
	 *	@note 確保していたメモリは一度解放されます。
	 *	@param l 確保長さ
	 *	@param P データ
	 */
	void Reset(size_t l, const TYP* P)
	{
		Resize(l);
		MemCopy(m_lpBuf, P, l);
	}
	/**
	 * [取得] ポインタ取得
	 *	@retval NULL 未確保。 
	 *	@retval NULL以外 ポインタ
	 */
	operator const TYP*(void) const { return m_lpBuf; }
	/**
	 * [取得] ポインタ取得
	 *	@retval NULL 未確保。
	 *	@retval NULL以外 ポインタ
	 */
	operator TYP*(void) { return m_lpBuf; }
	/**
	 * [取得] ポインタ取得
	 *	@retval NULL 未確保。
	 *	@retval NULL以外 ポインタ
	 */
	const TYP* Ref(void) const { return m_lpBuf; }
	/**
	 * [取得] ポインタ取得
	 *	@retval NULL 未確保。
	 *	@retval NULL以外 ポインタ
	 */
	TYP* Ref(void) { return m_lpBuf; }
	/**
	 * [設定] 全情報代入.
	 *		全要素に代入します。
	 *	@param other 情報元
	 */
	void Fill(const TYP& other)
	{
		loop ( i ,m_size )
		{
			m_lpBuf[i] = other;
		}
	}
	/**
	 * [設定] スワップ.
	 *		自分と相手のデータを入れ替えます。
	 *	@param[out] _other スワップ相手.
	 */
	void Swap(CWorkMemT& _other)
	{
		TNB::Swap(m_size, _other.m_size);
		TNB::Swap(m_lpBuf, _other.m_lpBuf);
	}
};



/**@ingroup MEMORY
 * ワークメモリ.
 *
 *		簡易的に指定長さのBYTEのメモリを確保できます。
 *		デストラクタで自動的に解放するためメモリリークの心配がありません。
 *	
 *	@par必要ファイル
 *			TnbDef.h
 *
 *	@date 06/09/19	新規
 */
typedef CWorkMemT<BYTE> CWorkMem;



/**@ingroup STRING
 * 簡易文字列管理クラス.
 *
 *		代入した文字列を管理するだけの簡単な文字列クラスです。
 *		デストラクタで自動的に解放するためメモリリークの心配がありません。
 *
 *	@par必要ファイル
 *			TnbDef.h
 *
 *	@date 06/12/18	新規
 */
class CSimpleStr
{
	CWorkMemT<TCHAR> m_str;
public:
	/**
	 * コンストラクタ
	 *	@param lpsz 初期代入文字列
	 */
	CSimpleStr(LPCTSTR lpsz = NULL)
	{
		operator=(lpsz);
	}
	/**
	 * コンストラクタ
	 *	@param other 初期代入文字列
	 */
	CSimpleStr(const CSimpleStr& other)
	{
		operator=(other.m_str.Ref());
	}
	/**
	 * [代入] 代入.
	 *	@note 今保持している文字列を解放して指定の文字列を記憶。
	 *	@param other 代入文字列
	 *	@return 自分の参照
	 */
	CSimpleStr& operator=(const CSimpleStr& other)
	{
		return operator=(other.m_str.Ref());
	}
	/**
	 * [代入] 代入.
	 *	@note 今保持している文字列を解放して指定の文字列を記憶。
	 *	@param lpsz 代入文字列
	 *	@return 自分の参照
	 */
	CSimpleStr& operator=(LPCTSTR lpsz)
	{
		if ( lpsz == NULL )
		{
			m_str.Free();
		}
		else
		{
			size_t l = _tcslen(lpsz) + 1;
			m_str.Resize(l);
			LPTSTR P = m_str;
			MemCopy(P, lpsz, l);
		}
		return *this;
	}
	/**
	 * [確認] 空チェック
	 *	@retval true 空っぽ
	 *	@retval false 文字列あり。
	 */
	bool IsEmpty(void) const
	{
		LPCTSTR P = m_str;
		return P == NULL || *P == 0;
	}
	/**
	 * [操作] 空化
	 */
	void Empty(void) { m_str.Free(); }
	/**
	 * [取得] 文字列参照
	 *	@return アドレス。文字列を持っていない場合、NULL が返ります。
	 */
	LPCTSTR Ref(void) const { return m_str; }
	/**
	 * [取得] 文字列参照
	 *	@return アドレス。文字列を持っていない場合、NULL が返ります。
	 */
	operator LPCTSTR(void) const { return m_str; }
	/**
	 * [取得] 文字列参照
	 *	@return アドレス。文字列を持っていない場合、NULL が返ります。
	 */
	operator LPTSTR(void) { return m_str; }
};



/**@ingroup STRING
 * 簡易文字列管理クラス.
 *
 *		代入した文字列を管理するだけの簡単な文字列クラスです。
 *		デストラクタで自動的に解放するためメモリリークの心配がありません。
 *
 *	@note ASCII/SJIS専用です
 *	
 *	@par必要ファイル
 *			TnbDef.h
 *
 *	@date 06/12/18	新規
 */
class CSimpleAscii
{
	CWorkMemT<char> m_str;
public:
	/**
	 * コンストラクタ
	 *	@param lpsz 初期代入文字列
	 */
	CSimpleAscii(LPCSTR lpsz = NULL)
	{
		if ( lpsz != NULL ) { operator=(lpsz); }
	}
	/**
	 * コンストラクタ
	 *	@param other 初期代入文字列
	 */
	CSimpleAscii(const CSimpleAscii& other)
	{
		operator=(other.m_str.Ref());
	}
	/**
	 * [代入] 代入.
	 *	@note 今保持している文字列を解放して指定の文字列を記憶。
	 *	@param other 代入文字列
	 *	@return 自分の参照
	 */
	CSimpleAscii& operator=(const CSimpleAscii& other)
	{
		return operator=(other.m_str.Ref());
	}
	/**
	 * [代入] 代入.
	 *	@note 今保持している文字列を解放して指定の文字列を記憶。
	 *	@param lpsz 代入文字列
	 *	@return 自分の参照
	 */
	CSimpleAscii& operator=(LPCSTR lpsz)
	{
		if ( lpsz == NULL )
		{
			m_str.Free();
		}
		else
		{
			size_t l = strlen(lpsz) + 1;
			m_str.Resize(l);
			LPSTR P = m_str;
			MemCopy(P, lpsz, l);
		}
		return *this;
	}
	/**
	 * [確認] 空チェック
	 *	@retval true 空っぽ
	 *	@retval false 文字列あり。
	 */
	bool IsEmpty(void) const 
	{
		LPCSTR P = m_str;
		return P == NULL || *P == 0;
	}
	/**
	 * [操作] 空化
	 */
	void Empty(void) { m_str.Free(); }
	/**
	 * [取得] 文字列参照
	 *	@return アドレス。文字列を持っていない場合、NULL が返ります。
	 */
	LPCSTR Ref(void) const { return m_str; }
	/**
	 * [取得] 文字列参照
	 *	@return アドレス。文字列を持っていない場合、NULL が返ります。
	 */
	operator LPCSTR(void) const { return m_str; }
	/**
	 * [取得] 文字列参照
	 *	@return アドレス。文字列を持っていない場合、NULL が返ります。
	 */
	operator LPSTR(void) { return m_str; }
};



/**@ingroup VALUE
 * [変換] スワッパー.
 *		同じ型の変数同士の値の内容を交換します。
 *		コピー機能が使えるクラスでも使用可能。
 *	@param t1 変数の参照
 *	@param t2 変数の参照
 *
 *	@par必要ファイル
 *			TnbDef.h
 *	@date 06/07/20	新規
 */
template<typename T> void Swap(T& t1, T& t2)
{
	T x = t1;
	t1 = t2;
	t2 = x;
}



/**@ingroup STRING
 * [変換] SystemErrorコード文字列化.
 *	@note SystemErrorコードをテキストに変換.
 *	@param[out] _str テキストが格納されます。
 *	@param[in] dwError ::GetLastError() の戻り値など、SystemErrorコード。
 *	@par必要ファイル
 *			TnbDef.h
 *	@date 09/10/23	新規
 */
inline void SystemErrorToMessageText(CSimpleStr& _str, DWORD dwError)
{
	LPTSTR lpMsgBuf = NULL;
	LPTSTR lpBuff = reinterpret_cast<LPTSTR>(&lpMsgBuf);
	DWORD flag = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD r = ::FormatMessage(flag, NULL, dwError, LANG_USER_DEFAULT, lpBuff, 0, NULL);
	if ( r == 0 )
	{
		_str = _T("Unknown");
	}
	else
	{
		_str = lpMsgBuf;
		LPTSTR P = _str;
		size_t l = _tcslen(P);
		loop ( i, l )
		{
			if ( *P == '\n' || *P == '\r' )
			{
				*P = ' ';
			}
			P++;
		}
		::LocalFree(lpMsgBuf);
	}
}



/**@ingroup VALUE
 * [変換] エンディアン変換.
 *	@param val 値
 *	@return エンディアン変換した値
 *	@par必要ファイル
 *			TnbDef.h
 *	@date 08/09/08 新規
 */
inline WORD SwapEndian(WORD val)
{
	WORD ret;
	*(reinterpret_cast<BYTE*>(&ret) + 0) = *(reinterpret_cast<BYTE*>(&val) + 1);
	*(reinterpret_cast<BYTE*>(&ret) + 1) = *(reinterpret_cast<BYTE*>(&val) + 0);
	return ret;
}



/**@ingroup VALUE
 * [変換] エンディアン変換.
 *	@param val 値
 *	@return エンディアン変換した値
 *	@par必要ファイル
 *			TnbDef.h
 *	@date 08/09/08 新規
 */
inline DWORD SwapEndian(DWORD val)
{
	DWORD ret;
	*(reinterpret_cast<BYTE*>(&ret) + 0) = *(reinterpret_cast<BYTE*>(&val) + 3);
	*(reinterpret_cast<BYTE*>(&ret) + 1) = *(reinterpret_cast<BYTE*>(&val) + 2);
	*(reinterpret_cast<BYTE*>(&ret) + 2) = *(reinterpret_cast<BYTE*>(&val) + 1);
	*(reinterpret_cast<BYTE*>(&ret) + 3) = *(reinterpret_cast<BYTE*>(&val) + 0);
	return ret;
}



/**@ingroup VALUE
 * [変換] エンディアン変換.
 *	@param val 値
 *	@return エンディアン変換した値
 *	@par必要ファイル
 *			TnbDef.h
 *	@date 09/10/26 新規
 */
inline ULONGLONG SwapEndian(ULONGLONG val)
{
	ULONGLONG ret = 0;
	ret = (ULONGLONG) (SwapEndian((DWORD)(val >> 32))) << 32;
	ret |= (SwapEndian((DWORD)(val & 0x00000000FFFFFFFF)));
	return ret;
}



/// 文字列操作ライブラリ
namespace STRLIB
{
	/** 
	 * [作成] 書式付き文字列作成(ASCII/SJIS用)
	 *	@param[out]	_pWork 作成場所
	 *	@param[in]	iLen pWorkのサイズ(文字数)
	 *	@param[in]	lpFmt 書式指定文字列
	 *	@param[in]	V 省略可能な引数
	 *	@retval true 成功
	 *	@retval false iLenでは足りない
	 *	@par必要ファイル
	 *			TnbDef.h
	 */
	inline bool VPrintF(LPSTR _pWork, size_t iLen, LPCSTR lpFmt, va_list V)
	{
		*_pWork = 0;
		#if _MSC_VER < 1400 || defined(_WIN32_WCE)
			return vsprintf(_pWork, lpFmt, V) >= 0;
		#else
			return vsprintf_s(_pWork, iLen, lpFmt, V) >= 0;
		#endif
	}

	/** 
	 * [作成] 書式付き文字列作成(ASCII/SJIS用)
	 *	@param[out]	_pWork 作成場所
	 *	@param[in]	iLen pWorkのサイズ(文字数)
	 *	@param[in]	lpFmt 書式指定文字列
	 *	@param[in]	... 省略可能な引数
	 *	@retval true 成功
	 *	@retval false iLenでは足りない
	 *	@par必要ファイル
	 *			TnbDef.h
	 */
	inline bool PrintF(LPSTR _pWork, size_t iLen, LPCSTR lpFmt, ...)
	{
		va_list args;
		va_start(args, lpFmt);
		bool r = VPrintF(_pWork, iLen, lpFmt, args);
		va_end(args);
		return r;
	}

	/** 
	 * [作成] 書式付き文字列作成(UNICODE用)
	 *	@param[out]	_pWork 作成場所
	 *	@param[in]	iLen pWorkのサイズ(文字数)
	 *	@param[in]	lpFmt 書式指定文字列
	 *	@param[in]	V 省略可能な引数
	 *	@retval true 成功
	 *	@retval false iLenでは足りない
	 *	@par必要ファイル
	 *			TnbDef.h
	 */
	inline bool VPrintF(LPWSTR _pWork, size_t iLen, LPCWSTR lpFmt, va_list V)
	{
		*_pWork = 0;
		#if _MSC_VER < 1400 || defined(_WIN32_WCE)
			return vswprintf(_pWork, lpFmt, V) >= 0;
		#else
			return vswprintf_s(_pWork, iLen, lpFmt, V) >= 0;
		#endif
	}

	/** 
	 * [作成] 書式付き文字列作成(UNICODE用)
	 *	@param[out]	_pWork 作成場所
	 *	@param[in]	iLen pWorkのサイズ(文字数)
	 *	@param[in]	lpFmt 書式指定文字列
	 *	@param[in]	... 省略可能な引数
	 *	@retval true 成功
	 *	@retval false iLenでは足りない
	 *	@par必要ファイル
	 *			TnbDef.h
	 */
	inline bool PrintF(LPWSTR _pWork, size_t iLen, LPCWSTR lpFmt, ...)
	{
		va_list args;
		va_start(args, lpFmt);
		bool r = VPrintF(_pWork, iLen, lpFmt, args);
		va_end(args);
		return r;
	}
	
	/** 
	 * [作成] 識別用文字列作成(ASCII用)
	 *		32bit文字を一意の文字列に変換します。
	 *	@param[out] _pWork 作成場所。10文字分、必要です。
	 *	@param[in] dw 32bitの数値。
	 *	@par必要ファイル
	 *			TnbDef.h
	 */
	inline void IdToString(LPSTR _pWork, DWORD dw)
	{
		while( dw != 0 )
		{
			*_pWork++ = static_cast<char>((dw % 26) + 'A');
			dw /= 26;
		}
		*_pWork = 0;
	}
	
	/** 
	 * [作成] 識別用文字列作成(UNICODE用)
	 *		32bit文字を一意の文字列に変換します。
	 *	@param[out] _pWork 作成場所。10文字分、必要です。
	 *	@param[in] dw 32bitの数値。
	 *	@par必要ファイル
	 *			TnbDef.h
	 */
	inline void IdToString(LPWSTR _pWork, DWORD dw)
	{
		while( dw != 0 )
		{
			*_pWork++ = static_cast<WCHAR>((dw % 26) + 'A');
			dw /= 26;
		}
		*_pWork = 0;
	}
	
}; // STRLIB



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// Trace用
inline DWORD _LastErrorTrace(LPCTSTR lpszFile, int line, LPCTSTR lpsz)
{
	DWORD dwErr = ::GetLastError();
	if ( dwErr != ERROR_SUCCESS )
	{
		CSimpleStr s;
		SystemErrorToMessageText(s, dwErr);
		TRACE2( "%s(%d): ", lpszFile, line );
		TRACE3( "API[%s()]失敗 Last Error Code = 0x%08x(%s)\n", lpsz, dwErr, s.Ref() );
		::SetLastError(dwErr);
	}
	return dwErr;
}

inline BOOL _DeleteObject(HGDIOBJ h)
{
  #ifdef _DEBUG
	if ( ::GetObjectType(h) == NULL )
	{
		ASSERT0( false, "DeleteObject", "破棄されたGDIオブジェクトを破棄しようとしています。" );
		return false;
	}
	BOOL r = ::DeleteObject(h);
	ASSERT1( r, "DeleteObject", "GDIオブジェクトの破棄に失敗しました。\ncode = %d\ntype = 0x%08X", ::GetLastError() );
	return r;
  #else
	return ::DeleteObject(h);
  #endif
}


#endif // _TnbDOXYGEN



///TNB HASH関係
namespace HASH
{
	/**
	 * [計算] ハッシュ値計算.
	 *	@param iLen 長さ
	 *	@param P ポインタ
	 *	@return ハッシュ値
	 */
	inline DWORD CalcHash(size_t iLen, LPCVOID P)
	{
		const BYTE* B = static_cast<const BYTE*>(P);
		DWORD dwRc = 0;
		loop ( i, iLen )
		{
			dwRc += B[i] << ((i + B[0]) & 0x0F);
		}
		return dwRc;
	}
};



/**@ingroup WINAPI
 * インスタンスハンドル種.
 *		TNB Library で使用するハンドルの種類。
 *	@see SetInstanceHandle() , GetInstanceHandle()
 */
enum EInstanceType
{
	EI_Process,	///< プロセス用
	EI_Bitmap,	///< ビットマップリソース用
	EI_Icon,	///< アイコンリソース用
	EI_String,	///< 文字列リソース用
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル

	struct TTnbParam
	{
		LONG_PTR*		plNullBase;		///< NULLエリア
		LONG_PTR		alNullBase[8];	///< NULLエリア
		HINSTANCE	ahInstances[4];	///< インスタンスハンドル EInstanceTypeの数にあわせる
	};
	_SELECTANY TTnbParam	g_tnbParam = { g_tnbParam.alNullBase };
	_SELECTANY TTnbParam*	g_pTnbParam = &g_tnbParam;

	#define g_plNullBase g_pTnbParam->plNullBase

	// NULLエリア確認マクロ
	#define ASEERT_NULLBASE ASSERTLIB((g_plNullBase[0]|g_plNullBase[1]|g_plNullBase[2]|g_plNullBase[3]|	\
						g_plNullBase[4]|g_plNullBase[5]|g_plNullBase[6]|g_plNullBase[7])==0);

#endif //_TnbDOXYGEN


	
/**
 * [設定] ライブラリ初期化.
 *		TNB Library を初期化します。
 *	@note	一つのEXEや、一つのDLL内だけで TNB Library を使用する場合、これを呼ぶ必要はありません。 
 *			EXE と DLL 、双方で使う場合、それぞれの開始時に、一度だけそれぞれコールする必要があります。
 *	@par必要ファイル
 *			TnbDef.h
 */
inline void InitializeTnbLibrary(void)
{
	if ( g_pTnbParam == &g_tnbParam )
	{
		TCHAR buf[20] = _T("_!TnbLib!_");	//10文字
		STRLIB::IdToString(&buf[10], ::GetCurrentProcessId());
		HANDLE hMap = ::CreateFileMapping(reinterpret_cast<HANDLE>(-1), NULL, PAGE_READWRITE, 0, sizeof(TTnbParam), buf);
		ASSERT0( hMap != NULL, "InitializeTnbLibrary", "管理メモリが確保できませんでした。" );
		bool isFirst = (::GetLastError() != ERROR_ALREADY_EXISTS);
		g_pTnbParam = static_cast<TTnbParam*>(::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		ASSERT0( g_pTnbParam != NULL, "InitializeTnbLibrary", "管理メモリが確保できませんでした。" );
		if ( isFirst ) 
		{
			g_pTnbParam->plNullBase = g_pTnbParam->alNullBase;
		}
	}
}



/**@ingroup WINAPI
 * [設定] インスタンスハンドル指定.
 *		TNB Library で使用するインスタンスハンドルの指定をします。
 *	@param type タイプ
 *	@param hInstance インスタンスハンドル 
 *	@par必要ファイル
 *			TnbDef.h
 */
inline void SetInstanceHandleByTnb(EInstanceType type, HINSTANCE hInstance)
{
	g_pTnbParam->ahInstances[type] = hInstance;
}



/**@ingroup WINAPI
 * [取得] インスタンスハンドル取得.
 *		TNB Library で使用するインスタンスハンドルを取得します。
 *		SetInstanceHandle() で設定していない場合、NULL
 *		（MFCプロジェクトの場合、AfxGetInstanceHandle() のハンドル）が返ります。
 *	@param type タイプ
 *	@return インスタンスハンドル 
 *	@par必要ファイル
 *			TnbDef.h
 */
inline HINSTANCE GetInstanceHandleByTnb(EInstanceType type = EI_Process)
{
	if ( g_pTnbParam->ahInstances[type] == NULL )
	{
		#ifdef __AFX_H__
			g_pTnbParam->ahInstances[type] = ::AfxGetInstanceHandle();
		#else
			g_pTnbParam->ahInstances[type] = g_pTnbParam->ahInstances[EI_Process];
		#endif
	}
	return g_pTnbParam->ahInstances[type];
}



///TNB MFC Library
namespace MFC	{};

///TNB DirectX Library
namespace DX	{};



}; // TNB



#ifndef _TnbUSINGNAMESPACE_DISABLE
 using namespace TNB;
 using namespace TNB::MFC;
 using namespace TNB::DX;
#endif
