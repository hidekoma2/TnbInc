#pragma once
/**
 *@file
 *@ingroup NEWDEL
 * �J���p��������ԊǗ��֌W�̃w�b�_.
 *
 *	@attention �{�w�b�_�́A�C���N���[�h���邾���ŉe��������܂��B
 *
 *		�{�w�b�_���C���N���[�h����ƁAnew / malloc() ��u�������A
 *		�v���Z�X�I�����ɁA���������������m�F���邱�Ƃ��o���܂��B
 *		�܂��Anew / malloc() �ł̃������m�ێ��s���ɁA std::bad_alloc ��O�X���[����悤�ɂȂ�܂��B
 *
 *	@note	�}�N�� "_TnbNEW_Log_ON" ����`����Ă���ƁAnew / delete �̃��O���\������܂��B
 *			���O�ɏ����ꂽ�������m�ۃi���o�[����A�s���A�N�Z�X�̗̈���m��ł��܂��B
 *	
 *	@note	TnbDef.h �̓C���N���[�h���Ă��܂���B
 *
 *	@attention �f�o�b�O�o�[�W�����ȊO�ł͋@�\���܂���B
 *	@note placement new �́u PlacementNew() �v���g���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#if defined(_DEBUG) && !defined(_WIN32_WCE)



#include "TnbNewPlacement.h"



#include <malloc.h>
#include <windows.h>
#include <crtdbg.h>
#include <new>
#include <memory>
#include <stdio.h>



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��


#if _MSC_VER < 1400
 #define _TnbStrFmt sprintf
#else
 #define _TnbStrFmt sprintf_s
#endif

namespace TNB {
///TNB 'New-Delete' Library
namespace NEW {

	// Hook�A�h���X
	_declspec(selectany) _CRT_ALLOC_HOOK gpAllocHook = NULL;
	// �����`�F�b�N�N���X
	struct CCrtDbgAutoCheck
	{
		// �R���X�g���N�^
		CCrtDbgAutoCheck(void)
		{
			int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			tmpFlag |= _CRTDBG_LEAK_CHECK_DF;	//�I������ LeakCheck���o��
			_CrtSetDbgFlag(tmpFlag);
			#ifdef _TnbNEW_Log_ON
				if ( gpAllocHook == NULL )
				{
					//�������m��/����������ɃR�[�������֐��w��B
					gpAllocHook = _CrtSetAllocHook(MyAllocHook);
				}
			#endif
		}
		//�������m��/����������ɃR�[�������֐��w��B
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

	// �������m��
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
			_CrtDbgReport(_CRT_ERROR, lpszFileName, nLine, NULL, "�������m�ۂɎ��s���܂����B");
			throw std::bad_alloc();
		}
		return P;
	}
};//NEW
};//TNB



/// new�I�[�o�[���[�h
inline LPVOID __cdecl operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	return TNB::NEW::MemAlloc(nSize, lpszFileName, nLine);
}



/// delete �I�[�o�[���[�h
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



#endif	//Document�쐬�p�V���{��



#endif	//_DEBUG
