#pragma once
/**
 *@file
 * TNB���C�u�����̒�`�w�b�_
 *
 *	���ׂẴw�b�_�ɃC���N���[�h����Ă��܂��B
 *
 *	��O "TnbNewDebug.h" , "TnbNewPlacement.h" ,"TnbNewThrow.h"
 *
 *	@note	�}�N�� "_TnbUSINGNAMESPACE_DISABLE" ����`����Ă���ƁA
 *			TNB �ȉ��� using namespace ���s�Ȃ��܂���B
 *
 * \code
 * -------------------------------------------------------------------
 *	
 *	�@TNB Library Project For Win32
 *	 
 *	�@Copyright (C) 2001-2019 by TNB���쏊 - Hide Komatsu
 *	 
 *	�@��L�҂́ATNB Library�i�ȉ��A�{���C�u�����ƋL���j�̒��쌠��ێ�����
 *	�@�����A�ȉ��̏����𖞂����ꍇ�Ɍ���A�{���C�u�����̎g�p�E�����E���ρE
 *	�@�Ĕz�z���邱�Ƃ𖳏��ŋ������܂��B
 *	�@(1) �{���C�u�������\�[�X�R�[�h�̌`�ŗ��p����ꍇ�́A��L�̒��쌠�\
 *	�@�@�@���A���̗��p�����A����щ��L�̖��ۏ؋K�肪�A���̂܂܂̌`�Ń\�[
 *	�@�@�@�X�R�[�h���Ɋ܂܂�Ă��邱�ƁB
 *	�@(2) �{���C�u�������g�p�����o�C�i���`���i�{���C�u�������ė��p���o��
 *	�@�@�@�Ȃ�������`���܂ށj�ōĔz�z����ꍇ�ɂ́A�Ĕz�z�ɔ����h�L��
 *	�@�@�@�����g�i���p�҃}�j���A���Ȃǁj�ɁA��L�̒��쌠�\���A���̗��p��
 *	�@�@�@������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁB
 *	�@(3) �{���C�u�����̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹�Q
 *	�@�@�@������A��L���쌠�҂�Ɛӂ��邱�ƁB
 *	 
 *	�@�{���C�u�����́C���ۏ؂Œ񋟂��Ă�����̂ł��B��L���쌠�҂́A�{��
 *	�@�C�u�����Ɋւ��āA�����Ȃ�ۏ؂��s�Ȃ��܂���B�܂��A�{���C�u������
 *	�@���p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��A���̐�
 *	�@�C�𕉂��܂���B
 *	
 * -------------------------------------------------------------------
 * \endcode
 */



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

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

#ifdef _TnbPGRELIEF	//Document�쐬�p�V���{��
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
 //====VC6.0�ȉ�
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

 #define ASSERTLIB(XX) ASSERT0(XX, "TNB Library", "TNB Library �̃o�O�̉\��������܂��B")
 #define VERIFYLIB(XX) ASSERTLIB(XX)
 #undef ASSERT
 #define ASSERT(XX) ASSERT1(XX, NULL, "[%s]\n\n��L�ӏ��ŃA�T�[�g���܂����B", _T(#XX))
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

// �v���Z�X�ŗB��錾
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
 //====VC6.0�ȉ�
 #define __MSVCPP6__
 #define __noop (void)0
 #define for if ( false ); else for // for���̕ϐ��錾��W���ɂ���
 #define DEFSUPER(XX) typedef XX _super; typedef XX __super
 #define _deprecated
 #define DEFPARENTLISTENER(PARENT, INST) typedef IListener INST
 #if defined(UNICODE) || defined(_UNICODE)
  #ifndef _USRDLL
   #pragma comment(linker, "/entry:\"wWinMainCRTStartup\"")
  #endif
 #endif
#else
 //====VC7.0�ȏ�
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
  //===VC8.0�ȏ�
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
 * �r���h���b�Z�[�W�\��
 *	@par�g�p��
 *		\code
 *			#pragma MESSAGE("�R���p�C����") 
 *		\endcode
 *	@param d �r���h���ɕ\������������
 */
#define MESSAGE(d)	message(__FILE__"("_BDMSG(__LINE__)") : message:" d)



/** 
 * BIT���Z
 *	@param X �B�ꗧ�Ă���Bit�i���o�[�i�O�`63�j�Bint �^�̕ϐ����\�B
 *	@note ���ʓ��ɒ萔���e���������ƒ萔�ɂȂ�܂��i�R���p�C�����ɒl���m�肷�邽�߁j�B
 *	@return (1<<X)�̒萔
 */
#define _BIT(X) (1ui64<<(X))



/** 
 * ��i���\�L�萔
 *	@param X ��i���\�L�̃��e����(�ϐ��͕s��)�B11��(0x7FF)�܂őΉ��i12���ȏ�� warning C4309 �������j�B
 *	@note	0,1�ȊO�����ʓ��ɓ����ƃG���[�A�܂��͌��ʂ��s���ɂȂ�܂��B
 *	@note ���ʂ͒萔�ɂȂ�܂��i�R���p�C�����ɒl���m�肷�邽�߁j�B
 *	@par�g�p��
 *		\code 
 *		int x = _BIN(00011101); // x �� 0x1D (29) �ɂȂ�B 
 *		\endcode
 */
#define _BIN(X) (TBinaryNotation<0##X>::VALUE)



/** 
 * loop�\��.
 *		0 ���� CNT-1 �܂ŌJ��Ԃ��܂��B
 *	@par�g�p��1
 *		\code 
 *		loop ( i, 10 ){ Foo(i); }	//Foo�� 0�`9 ��10��R�[�������B 
 *		\endcode
 *	@par�g�p��2
 *		\code 
 *		CByteVector vb;
 *		vb.SetSize(5);
 *		loop ( i, vb ){ // operator size_t() ������
 *			vb[i] = i;	//Foo�� 0�`4��5��R�[�������B 
 *		}
 *		\endcode
 *	@param VAR ���[�v�Ŏg���ϐ��̖��O�Bsize_t �^�Ő錾����܂��B
 *	@param CNT �񐔁B��x�����]������܂���B ���[�v���Asize_t �^�� VAR_size �ƌ������ŕێ�����܂��B
 */
#define loop(VAR, CNT) for ( size_t VAR = 0, VAR##_size = CNT; VAR < VAR##_size; VAR++ )

/** 
 * loop�\��.
 *		CNT-1 ���� 0 �܂ŌJ��Ԃ��܂��B
 *	@par�g�p��
 *		\code 
 *		loop ( i, 10 ){ Foo(i); }	//Foo�� 9�`0 ��10��R�[�������B 
 *		\endcode
 *	@param VAR ���[�v�Ŏg���ϐ��̖��O�Bsize_t �^�Ő錾����܂��B
 *	@param CNT �񐔁B��x�����]������܂���B ���[�v���Asize_t �^�� VAR_size �ƌ������ŕێ�����܂��B
 */
#define loop_dn(VAR, CNT) for ( size_t VAR##_r = 0, VAR##_size = CNT, VAR = CNT - 1; VAR##_r < VAR##_size; VAR--, VAR##_r++ )



///TNB Library
namespace TNB
{



/**
 * [����] �������R�s�[
 *	@note �����ݐ�̌^����ɃR�s�[���܂��B
 *	@note �̈悪�d�Ȃ��Ă���ꍇ�A����͖��ۏ؂ł��B
 *	@param _pDst	�R�s�[��
 *	@param pSrc	�R�s�[��
 *	@param len	�R�s�[�����B _pDst�̃T�C�Y�~ len Byte���R�s�[����܂��B
 *	@par�K�v�t�@�C��
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
 * [�ݒ�] �[���N���A.
 *	@param value �[���N���A�Ώەϐ�
 *	@par�K�v�t�@�C��
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
 * [�m�F] �͈̓`�F�b�N.
 *		�w��� value �� �O�ȏ�A size �����ł��邩�`�F�b�N���܂��B
 *	@param value �`�F�b�N�Ώےl
 *	@param size �T�C�Y
 *	@retval true �͈͓�
 *	@retval false �͈͊O
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 */
inline bool IsInRange(INDEX value, size_t size)
{
	return value < size;
}



/**@ingroup VALUE
 * [�錾] �Q�Ƃ��Ȃ��l�錾.
 *	@param value �Ώەϐ�
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 */
template<typename T> inline void IgnoreUnusedValue(const T& value)
{
}



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

// �I�[�o�[�t���[�`�F�b�N�t���A�_�E���L���X�g
template<typename TYP>
class down_cast
{
public:
	template<typename SRC>
	down_cast(SRC src) : m_typ(static_cast<TYP>(src))
	{
		ASSERT1( (SRC)(m_typ) == src, "down_cast<>", "�I�[�o�[�t���[���܂����B\nsource = %d", src );
	}
	down_cast(float src) : m_typ(static_cast<TYP>(src))
	{
		ASSERT1( (LONGLONG)(m_typ) == LONGLONG(src), "down_cast<>", "�I�[�o�[�t���[���܂����B\nsource = %f", src );
	}
	down_cast(double src) : m_typ(static_cast<TYP>(src))
	{
		ASSERT1( (LONGLONG)(m_typ) == LONGLONG(src), "down_cast<>", "�I�[�o�[�t���[���܂����B\nsource = %f", src );
	}
	operator TYP(void) const
	{
		return m_typ;
	}
private:
	TYP m_typ;
};

/// �R���X�g�I�u�W�F�N�g��
template<typename T> inline const T* Const(T* P)
{
	return const_cast<const T*>(P);
}

/// int ��
template<typename T> inline int ToInt(T t)
{
	return down_cast<int>(t);
}

/// int ��
template<> inline int ToInt(LONGLONG ll)
{
	return down_cast<int>(ll);
}

/// WORD��
template<typename T> inline WORD ToWord(T t)
{
	return down_cast<WORD>(t);
}

/// DWORD��
template<typename T> inline DWORD ToDword(T t)
{
	return down_cast<DWORD>(t);
}

/// INDEX ��
template<typename T> inline INDEX ToIndex(T t)
{
	return down_cast<INDEX>(t);
}

/// �������
template<typename T1, typename T2> inline void _deprecated ForceSet(T1& t1, const T2& t2)
{
	t1 = (const T1)(t2);
}

#ifdef __MSVCPP6__
/// �C���^�[���b�N��r�`�F���W.
inline LONG InterlockedCompareExchange(LONG* _plDes, LONG lExchange, LONG lComperand)
{
	typedef LONG (WINAPI* P_CompExc)(LONG*, LONG, LONG);
	P_CompExc pCompExc = reinterpret_cast<P_CompExc>(&::InterlockedCompareExchange);
	return pCompExc(_plDes, lExchange, lComperand);
}
#endif

/// BIN�\�L�p�e���v���[�g
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
/// DPIX�v�Z
inline int CalcScaleX(int x) { return DRA::SCALEX(x); }
inline int CalcScaleX(int x, HDC dc) { return DRA::SCALEX(x); }
/// DPIY�v�Z
inline int CalcScaleY(int y) { return DRA::SCALEY(y); }
inline int CalcScaleY(int y, HDC dc) { return DRA::SCALEY(y); }
#else
/// DPIX�v�Z
inline int CalcScaleX(int x, HDC dc) 
{
	return static_cast<int>(x * ::GetDeviceCaps(dc, LOGPIXELSX) / 96.0);
}
/// DPIX�v�Z
inline int CalcScaleX(int x) 
{
	HDC dc = ::GetDC(NULL);
	int r = CalcScaleX(x, dc); 
	::ReleaseDC(NULL, dc);
	return r;
}
/// DPIY�v�Z
inline int CalcScaleY(int y, HDC dc)
{
	return static_cast<int>(y * ::GetDeviceCaps(dc, LOGPIXELSY) / 96.0);
}
/// DPIY�v�Z
inline int CalcScaleY(int y)
{
	HDC dc = ::GetDC(NULL);
	int r = CalcScaleY(y, dc);
	::ReleaseDC(NULL, dc);
	return r;
}
#endif
/// DPI�v�Z
inline void CalcScale(RECT& _rc)
{
	_rc.left = CalcScaleX(_rc.left);
	_rc.right = CalcScaleX(_rc.right);
	_rc.top = CalcScaleY(_rc.top);
	_rc.bottom = CalcScaleY(_rc.bottom);
}
/// DPI�v�Z
inline void CalcScale(SIZE& _sz)
{
	_sz.cx = CalcScaleX(_sz.cx);
	_sz.cy = CalcScaleY(_sz.cy);
}
/// DPI�v�Z
inline void CalcScale(POINT& _po)
{
	_po.x = CalcScaleX(_po.x);
	_po.y = CalcScaleY(_po.y);
}

#endif //_TnbDOXYGEN



/**
 * [ETC] �R�s�[�s�\�X�[�p�[�N���X.
 *		���̃N���X���p������ƃR�s�[�s�\�̃C���X�^���X���쐬�ł��܂��B
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *	@date 06/04/01	�V�K�쐬�B
 */
class CCopyImpossible
{
	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param T �R�s�[��
	 */
	CCopyImpossible(const CCopyImpossible& T);
	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param T �R�s�[��
	 */
	void operator=(const CCopyImpossible& T);
public:
	/** �f�t�H���g�R���X�g���N�^ */
	CCopyImpossible(void) {}
	/** �f�X�g���N�^ */
	~CCopyImpossible(void) {}
};



/**@ingroup MEMORY
 * ���[�N�������e���v���[�g.
 *
 *		���[�N�������m�ۂ��܂��B�ȈՓI�Ƀ��������m�ۂł��܂��B
 *		�f�X�g���N�^�Ŏ����I�ɉ�����邽�߃��������[�N�̐S�z������܂���B
 *	
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *	@date 06/09/19	�V�K
 *	@date 10/01/26	Swap() �ǉ��B
 *	@date 11/08/02	Resize() �̓�������P�B
 */
template<typename TYP>
class CWorkMemT : CCopyImpossible
{
	size_t	m_size;			///< �T�C�Y
	TYP*	m_lpBuf;		///< �Ǘ��|�C���^
public:
	/**
	 * �R���X�g���N�^.
	 *	@note �������͊m�ۂ��܂���B
	 */
	CWorkMemT(void) : m_size(0), m_lpBuf(NULL) {}
	/**
	 * �R���X�g���N�^
	 *	@param l �m�ے���
	 */
	CWorkMemT(size_t l) : m_size(l), m_lpBuf(new TYP[(l == 0) ? 1 : l]) 
	{}
	/// �f�X�g���N�^
	~CWorkMemT(void) { Free(); }
	/// [�ݒ�] ���.
	///		���������J�����A�T�C�Y���O�ɂ��܂��B
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
	 * [�擾] �T�C�Y�擾
	 *	@return �T�C�Y
	 */
	size_t GetSize(void) const { return m_size; }
	/**
	 * [�ݒ�] �T�C�Y�Đݒ�
	 *	@note �m�ۂ��Ă����������͈�x�������܂��B
	 *	@note ���������̏ꍇ�A�������܂���B
	 *	@param l �m�ے���
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
	 * [�ݒ�] �Đݒ�
	 *	@note �m�ۂ��Ă����������͈�x�������܂��B
	 *	@param l �m�ے���
	 *	@param P �f�[�^
	 */
	void Reset(size_t l, const TYP* P)
	{
		Resize(l);
		MemCopy(m_lpBuf, P, l);
	}
	/**
	 * [�擾] �|�C���^�擾
	 *	@retval NULL ���m�ہB 
	 *	@retval NULL�ȊO �|�C���^
	 */
	operator const TYP*(void) const { return m_lpBuf; }
	/**
	 * [�擾] �|�C���^�擾
	 *	@retval NULL ���m�ہB
	 *	@retval NULL�ȊO �|�C���^
	 */
	operator TYP*(void) { return m_lpBuf; }
	/**
	 * [�擾] �|�C���^�擾
	 *	@retval NULL ���m�ہB
	 *	@retval NULL�ȊO �|�C���^
	 */
	const TYP* Ref(void) const { return m_lpBuf; }
	/**
	 * [�擾] �|�C���^�擾
	 *	@retval NULL ���m�ہB
	 *	@retval NULL�ȊO �|�C���^
	 */
	TYP* Ref(void) { return m_lpBuf; }
	/**
	 * [�ݒ�] �S�����.
	 *		�S�v�f�ɑ�����܂��B
	 *	@param other ���
	 */
	void Fill(const TYP& other)
	{
		loop ( i ,m_size )
		{
			m_lpBuf[i] = other;
		}
	}
	/**
	 * [�ݒ�] �X���b�v.
	 *		�����Ƒ���̃f�[�^�����ւ��܂��B
	 *	@param[out] _other �X���b�v����.
	 */
	void Swap(CWorkMemT& _other)
	{
		TNB::Swap(m_size, _other.m_size);
		TNB::Swap(m_lpBuf, _other.m_lpBuf);
	}
};



/**@ingroup MEMORY
 * ���[�N������.
 *
 *		�ȈՓI�Ɏw�蒷����BYTE�̃��������m�ۂł��܂��B
 *		�f�X�g���N�^�Ŏ����I�ɉ�����邽�߃��������[�N�̐S�z������܂���B
 *	
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *
 *	@date 06/09/19	�V�K
 */
typedef CWorkMemT<BYTE> CWorkMem;



/**@ingroup STRING
 * �ȈՕ�����Ǘ��N���X.
 *
 *		���������������Ǘ����邾���̊ȒP�ȕ�����N���X�ł��B
 *		�f�X�g���N�^�Ŏ����I�ɉ�����邽�߃��������[�N�̐S�z������܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *
 *	@date 06/12/18	�V�K
 */
class CSimpleStr
{
	CWorkMemT<TCHAR> m_str;
public:
	/**
	 * �R���X�g���N�^
	 *	@param lpsz �������������
	 */
	CSimpleStr(LPCTSTR lpsz = NULL)
	{
		operator=(lpsz);
	}
	/**
	 * �R���X�g���N�^
	 *	@param other �������������
	 */
	CSimpleStr(const CSimpleStr& other)
	{
		operator=(other.m_str.Ref());
	}
	/**
	 * [���] ���.
	 *	@note ���ێ����Ă��镶�����������Ďw��̕�������L���B
	 *	@param other ���������
	 *	@return �����̎Q��
	 */
	CSimpleStr& operator=(const CSimpleStr& other)
	{
		return operator=(other.m_str.Ref());
	}
	/**
	 * [���] ���.
	 *	@note ���ێ����Ă��镶�����������Ďw��̕�������L���B
	 *	@param lpsz ���������
	 *	@return �����̎Q��
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
	 * [�m�F] ��`�F�b�N
	 *	@retval true �����
	 *	@retval false �����񂠂�B
	 */
	bool IsEmpty(void) const
	{
		LPCTSTR P = m_str;
		return P == NULL || *P == 0;
	}
	/**
	 * [����] ��
	 */
	void Empty(void) { m_str.Free(); }
	/**
	 * [�擾] ������Q��
	 *	@return �A�h���X�B������������Ă��Ȃ��ꍇ�ANULL ���Ԃ�܂��B
	 */
	LPCTSTR Ref(void) const { return m_str; }
	/**
	 * [�擾] ������Q��
	 *	@return �A�h���X�B������������Ă��Ȃ��ꍇ�ANULL ���Ԃ�܂��B
	 */
	operator LPCTSTR(void) const { return m_str; }
	/**
	 * [�擾] ������Q��
	 *	@return �A�h���X�B������������Ă��Ȃ��ꍇ�ANULL ���Ԃ�܂��B
	 */
	operator LPTSTR(void) { return m_str; }
};



/**@ingroup STRING
 * �ȈՕ�����Ǘ��N���X.
 *
 *		���������������Ǘ����邾���̊ȒP�ȕ�����N���X�ł��B
 *		�f�X�g���N�^�Ŏ����I�ɉ�����邽�߃��������[�N�̐S�z������܂���B
 *
 *	@note ASCII/SJIS��p�ł�
 *	
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *
 *	@date 06/12/18	�V�K
 */
class CSimpleAscii
{
	CWorkMemT<char> m_str;
public:
	/**
	 * �R���X�g���N�^
	 *	@param lpsz �������������
	 */
	CSimpleAscii(LPCSTR lpsz = NULL)
	{
		if ( lpsz != NULL ) { operator=(lpsz); }
	}
	/**
	 * �R���X�g���N�^
	 *	@param other �������������
	 */
	CSimpleAscii(const CSimpleAscii& other)
	{
		operator=(other.m_str.Ref());
	}
	/**
	 * [���] ���.
	 *	@note ���ێ����Ă��镶�����������Ďw��̕�������L���B
	 *	@param other ���������
	 *	@return �����̎Q��
	 */
	CSimpleAscii& operator=(const CSimpleAscii& other)
	{
		return operator=(other.m_str.Ref());
	}
	/**
	 * [���] ���.
	 *	@note ���ێ����Ă��镶�����������Ďw��̕�������L���B
	 *	@param lpsz ���������
	 *	@return �����̎Q��
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
	 * [�m�F] ��`�F�b�N
	 *	@retval true �����
	 *	@retval false �����񂠂�B
	 */
	bool IsEmpty(void) const 
	{
		LPCSTR P = m_str;
		return P == NULL || *P == 0;
	}
	/**
	 * [����] ��
	 */
	void Empty(void) { m_str.Free(); }
	/**
	 * [�擾] ������Q��
	 *	@return �A�h���X�B������������Ă��Ȃ��ꍇ�ANULL ���Ԃ�܂��B
	 */
	LPCSTR Ref(void) const { return m_str; }
	/**
	 * [�擾] ������Q��
	 *	@return �A�h���X�B������������Ă��Ȃ��ꍇ�ANULL ���Ԃ�܂��B
	 */
	operator LPCSTR(void) const { return m_str; }
	/**
	 * [�擾] ������Q��
	 *	@return �A�h���X�B������������Ă��Ȃ��ꍇ�ANULL ���Ԃ�܂��B
	 */
	operator LPSTR(void) { return m_str; }
};



/**@ingroup VALUE
 * [�ϊ�] �X���b�p�[.
 *		�����^�̕ϐ����m�̒l�̓��e���������܂��B
 *		�R�s�[�@�\���g����N���X�ł��g�p�\�B
 *	@param t1 �ϐ��̎Q��
 *	@param t2 �ϐ��̎Q��
 *
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *	@date 06/07/20	�V�K
 */
template<typename T> void Swap(T& t1, T& t2)
{
	T x = t1;
	t1 = t2;
	t2 = x;
}



/**@ingroup STRING
 * [�ϊ�] SystemError�R�[�h������.
 *	@note SystemError�R�[�h���e�L�X�g�ɕϊ�.
 *	@param[out] _str �e�L�X�g���i�[����܂��B
 *	@param[in] dwError ::GetLastError() �̖߂�l�ȂǁASystemError�R�[�h�B
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *	@date 09/10/23	�V�K
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
 * [�ϊ�] �G���f�B�A���ϊ�.
 *	@param val �l
 *	@return �G���f�B�A���ϊ������l
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *	@date 08/09/08 �V�K
 */
inline WORD SwapEndian(WORD val)
{
	WORD ret;
	*(reinterpret_cast<BYTE*>(&ret) + 0) = *(reinterpret_cast<BYTE*>(&val) + 1);
	*(reinterpret_cast<BYTE*>(&ret) + 1) = *(reinterpret_cast<BYTE*>(&val) + 0);
	return ret;
}



/**@ingroup VALUE
 * [�ϊ�] �G���f�B�A���ϊ�.
 *	@param val �l
 *	@return �G���f�B�A���ϊ������l
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *	@date 08/09/08 �V�K
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
 * [�ϊ�] �G���f�B�A���ϊ�.
 *	@param val �l
 *	@return �G���f�B�A���ϊ������l
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 *	@date 09/10/26 �V�K
 */
inline ULONGLONG SwapEndian(ULONGLONG val)
{
	ULONGLONG ret = 0;
	ret = (ULONGLONG) (SwapEndian((DWORD)(val >> 32))) << 32;
	ret |= (SwapEndian((DWORD)(val & 0x00000000FFFFFFFF)));
	return ret;
}



/// �����񑀍색�C�u����
namespace STRLIB
{
	/** 
	 * [�쐬] �����t��������쐬(ASCII/SJIS�p)
	 *	@param[out]	_pWork �쐬�ꏊ
	 *	@param[in]	iLen pWork�̃T�C�Y(������)
	 *	@param[in]	lpFmt �����w�蕶����
	 *	@param[in]	V �ȗ��\�Ȉ���
	 *	@retval true ����
	 *	@retval false iLen�ł͑���Ȃ�
	 *	@par�K�v�t�@�C��
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
	 * [�쐬] �����t��������쐬(ASCII/SJIS�p)
	 *	@param[out]	_pWork �쐬�ꏊ
	 *	@param[in]	iLen pWork�̃T�C�Y(������)
	 *	@param[in]	lpFmt �����w�蕶����
	 *	@param[in]	... �ȗ��\�Ȉ���
	 *	@retval true ����
	 *	@retval false iLen�ł͑���Ȃ�
	 *	@par�K�v�t�@�C��
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
	 * [�쐬] �����t��������쐬(UNICODE�p)
	 *	@param[out]	_pWork �쐬�ꏊ
	 *	@param[in]	iLen pWork�̃T�C�Y(������)
	 *	@param[in]	lpFmt �����w�蕶����
	 *	@param[in]	V �ȗ��\�Ȉ���
	 *	@retval true ����
	 *	@retval false iLen�ł͑���Ȃ�
	 *	@par�K�v�t�@�C��
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
	 * [�쐬] �����t��������쐬(UNICODE�p)
	 *	@param[out]	_pWork �쐬�ꏊ
	 *	@param[in]	iLen pWork�̃T�C�Y(������)
	 *	@param[in]	lpFmt �����w�蕶����
	 *	@param[in]	... �ȗ��\�Ȉ���
	 *	@retval true ����
	 *	@retval false iLen�ł͑���Ȃ�
	 *	@par�K�v�t�@�C��
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
	 * [�쐬] ���ʗp������쐬(ASCII�p)
	 *		32bit��������ӂ̕�����ɕϊ����܂��B
	 *	@param[out] _pWork �쐬�ꏊ�B10�������A�K�v�ł��B
	 *	@param[in] dw 32bit�̐��l�B
	 *	@par�K�v�t�@�C��
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
	 * [�쐬] ���ʗp������쐬(UNICODE�p)
	 *		32bit��������ӂ̕�����ɕϊ����܂��B
	 *	@param[out] _pWork �쐬�ꏊ�B10�������A�K�v�ł��B
	 *	@param[in] dw 32bit�̐��l�B
	 *	@par�K�v�t�@�C��
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



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// Trace�p
inline DWORD _LastErrorTrace(LPCTSTR lpszFile, int line, LPCTSTR lpsz)
{
	DWORD dwErr = ::GetLastError();
	if ( dwErr != ERROR_SUCCESS )
	{
		CSimpleStr s;
		SystemErrorToMessageText(s, dwErr);
		TRACE2( "%s(%d): ", lpszFile, line );
		TRACE3( "API[%s()]���s Last Error Code = 0x%08x(%s)\n", lpsz, dwErr, s.Ref() );
		::SetLastError(dwErr);
	}
	return dwErr;
}

inline BOOL _DeleteObject(HGDIOBJ h)
{
  #ifdef _DEBUG
	if ( ::GetObjectType(h) == NULL )
	{
		ASSERT0( false, "DeleteObject", "�j�����ꂽGDI�I�u�W�F�N�g��j�����悤�Ƃ��Ă��܂��B" );
		return false;
	}
	BOOL r = ::DeleteObject(h);
	ASSERT1( r, "DeleteObject", "GDI�I�u�W�F�N�g�̔j���Ɏ��s���܂����B\ncode = %d\ntype = 0x%08X", ::GetLastError() );
	return r;
  #else
	return ::DeleteObject(h);
  #endif
}


#endif // _TnbDOXYGEN



///TNB HASH�֌W
namespace HASH
{
	/**
	 * [�v�Z] �n�b�V���l�v�Z.
	 *	@param iLen ����
	 *	@param P �|�C���^
	 *	@return �n�b�V���l
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
 * �C���X�^���X�n���h����.
 *		TNB Library �Ŏg�p����n���h���̎�ށB
 *	@see SetInstanceHandle() , GetInstanceHandle()
 */
enum EInstanceType
{
	EI_Process,	///< �v���Z�X�p
	EI_Bitmap,	///< �r�b�g�}�b�v���\�[�X�p
	EI_Icon,	///< �A�C�R�����\�[�X�p
	EI_String,	///< �����񃊃\�[�X�p
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	struct TTnbParam
	{
		LONG_PTR*		plNullBase;		///< NULL�G���A
		LONG_PTR		alNullBase[8];	///< NULL�G���A
		HINSTANCE	ahInstances[4];	///< �C���X�^���X�n���h�� EInstanceType�̐��ɂ��킹��
	};
	_SELECTANY TTnbParam	g_tnbParam = { g_tnbParam.alNullBase };
	_SELECTANY TTnbParam*	g_pTnbParam = &g_tnbParam;

	#define g_plNullBase g_pTnbParam->plNullBase

	// NULL�G���A�m�F�}�N��
	#define ASEERT_NULLBASE ASSERTLIB((g_plNullBase[0]|g_plNullBase[1]|g_plNullBase[2]|g_plNullBase[3]|	\
						g_plNullBase[4]|g_plNullBase[5]|g_plNullBase[6]|g_plNullBase[7])==0);

#endif //_TnbDOXYGEN


	
/**
 * [�ݒ�] ���C�u����������.
 *		TNB Library �����������܂��B
 *	@note	���EXE��A���DLL�������� TNB Library ���g�p����ꍇ�A������ĂԕK�v�͂���܂���B 
 *			EXE �� DLL �A�o���Ŏg���ꍇ�A���ꂼ��̊J�n���ɁA��x�������ꂼ��R�[������K�v������܂��B
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 */
inline void InitializeTnbLibrary(void)
{
	if ( g_pTnbParam == &g_tnbParam )
	{
		TCHAR buf[20] = _T("_!TnbLib!_");	//10����
		STRLIB::IdToString(&buf[10], ::GetCurrentProcessId());
		HANDLE hMap = ::CreateFileMapping(reinterpret_cast<HANDLE>(-1), NULL, PAGE_READWRITE, 0, sizeof(TTnbParam), buf);
		ASSERT0( hMap != NULL, "InitializeTnbLibrary", "�Ǘ����������m�ۂł��܂���ł����B" );
		bool isFirst = (::GetLastError() != ERROR_ALREADY_EXISTS);
		g_pTnbParam = static_cast<TTnbParam*>(::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		ASSERT0( g_pTnbParam != NULL, "InitializeTnbLibrary", "�Ǘ����������m�ۂł��܂���ł����B" );
		if ( isFirst ) 
		{
			g_pTnbParam->plNullBase = g_pTnbParam->alNullBase;
		}
	}
}



/**@ingroup WINAPI
 * [�ݒ�] �C���X�^���X�n���h���w��.
 *		TNB Library �Ŏg�p����C���X�^���X�n���h���̎w������܂��B
 *	@param type �^�C�v
 *	@param hInstance �C���X�^���X�n���h�� 
 *	@par�K�v�t�@�C��
 *			TnbDef.h
 */
inline void SetInstanceHandleByTnb(EInstanceType type, HINSTANCE hInstance)
{
	g_pTnbParam->ahInstances[type] = hInstance;
}



/**@ingroup WINAPI
 * [�擾] �C���X�^���X�n���h���擾.
 *		TNB Library �Ŏg�p����C���X�^���X�n���h�����擾���܂��B
 *		SetInstanceHandle() �Őݒ肵�Ă��Ȃ��ꍇ�ANULL
 *		�iMFC�v���W�F�N�g�̏ꍇ�AAfxGetInstanceHandle() �̃n���h���j���Ԃ�܂��B
 *	@param type �^�C�v
 *	@return �C���X�^���X�n���h�� 
 *	@par�K�v�t�@�C��
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
