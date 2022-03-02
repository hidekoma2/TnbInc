#pragma once
/**
 *@file
 * API�t�b�N�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSimpleVector.h"
#include "TnbWindowsVersion.h"





//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * API�t�b�N�Ǘ�
 *
 *		�v���Z�X�Ƀ��[�h����Ă��� DLL�̃R�[������t�b�N���邱�Ƃ��o���܂��B
 *		���̃v���Z�X�ɂ͉e����^���܂���B
 *
 *		��Ƀf�o�O�p�ɏ���������肷�邱�Ƃ�z�肵�Ă��܂��B
 *		
 *	@note	�N���X���\�b�h Replace() ���g���Ƃ����ւ��܂��i�߂����Ƃ͂ł��܂���j�B
 *	@par�g�p��
 *	<table><tr><td><pre>
 *		typedef int (WINAPI* P_MessageBoxA)(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
 *		P_MessageBoxA g_pApiMsgBox;		//�������A�h���X�L��
 *		//MessageBoxA �̑���ɌĂԊ֐�
 *		int WINAPI MyMessageBoxA(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
 *		{
 *			return g_pApiMsgBox(hwnd, "�t�b�N����", "--", MB_ICONEXCLAMATION);
 *		}
 *		void foo(void)
 *		{
 *			g_pApiMsgBox = CApiHookerT<P_MessageBoxA>::Replace("USER32.DLL", "MessageBoxA", MyMessageBoxA);
 *			::MessageBoxA(hwnd, "���̂܂�", "!!", MB_OK | MB_ICONERROR);	//�u�t�b�N�����v�Ƃ����_�C�A���O���ł�B		
 *		}
 *	</pre></td></tr>
 *
 *	@note	�����o Hook() ���g���ƁA Unhook() �Ō��ɖ߂����Ƃ��o���܂��B�ꎞ�I�Ȑ؂�ւ��Ɏg�p�ł��܂��B
 *	@par�g�p��
 *	<table><tr><td><pre>
 *		typedef int (WINAPI* P_MessageBoxA)(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
 *		//MessageBoxA �̑���ɌĂԊ֐�
 *		int WINAPI MyMessageBoxA(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
 *		{
 *			return MessageBoxW(hwnd, L"�t�b�N����", L"--", MB_ICONEXCLAMATION);
 *		}
 *		void foo(void)
 *		{
 *			CApiHookerT<P_MessageBoxA> hooker;
 *			hooker.Hook("USER32.DLL", "MessageBoxA", MyMessageBoxA);
 *			::MessageBoxA(hwnd, "���̂܂�", "!!", MB_OK | MB_ICONERROR);	//�u�t�b�N�����v�Ƃ����_�C�A���O���ł�B		
 *			hooker.Unhook();
 *			::MessageBoxA(hwnd, "���̂܂�", "!!", MB_OK | MB_ICONERROR);	//�u���̂܂܁v�Ƃ����_�C�A���O���ł�B		
 *		}
 *	</pre></td></tr>
 *
 *	@see http://www003.upp.so-net.ne.jp/kish/prog/apihook.html
 *
 *	@param PFNC �֐��̃A�h���X�^
 *	
 *	@par�K�v�t�@�C��
 *			TnbApiHooker.h
 *
 *	@date 06/12/18 �V�K�쐬�B
 *	@date 09/04/10 DLL����DLL�̃e�[�u�����ΏۂƂ����B
 */
template<class PFNC>
class CApiHookerT : CCopyImpossible
{
public:
	
	/**
	 * [���s] �����ύX
	 *	@note ���̃N���X���\�b�h�ŕύX����ƌ��ɖ߂��܂���B
	 *	@param lpszDllName ���[�h���Ă���DLL���BNULL���w�肷��Ƃ��ׂĂ�DLL�������Ώۂɂ��܂��B
	 *	@param lpszFuncName ���[�h���Ă���DLL���̊֐����B
	 *	@param pFnc �ύX���鏈���̏����ꂽ�֐��B
	 *	@param hModule ���W���[���n���h��(�ȗ�����ƃv���Z�X�̃n���h��( GetInstanceHandleByTnb(EI_Process) �̖߂�l)�ɂȂ�܂�)�B
	 *	@retval NULL �G���[�B
	 *	@retval NULL�ȊO �����B�l�́A�ύX�O�̏����A�h���X�B
	 */
	static PFNC Replace(LPCSTR lpszDllName, LPCSTR lpszFuncName, PFNC pFnc, HMODULE hModule = NULL)
	{
		CApiHookerT a;
		bool r = a.Hook(lpszDllName, lpszFuncName, pFnc, hModule);
		return r ? a() : NULL;
	}


	//----


	/// �R���X�g���N�^
	CApiHookerT(void) : m_pOrignalFnc(NULL), m_hModule(NULL)
	{
	}

	/**
	 * [���s] �����t�b�N
	 *	@note ��C���X�^���X�ŁA�t�b�N�o����͈̂�����ł��B
	 *	@note ���Ƀt�b�N���Ă���ƃA���t�b�N���Ă���t�b�N�������܂��B
	 *	@param lpszDllName ���[�h���Ă��� DLL���BNULL ���w�肷��Ƃ��ׂĂ� DLL�������Ώۂɂ��܂��B
	 *	@param lpszFuncName ���[�h���Ă��� DLL���̊֐����B
	 *	@param pFnc �ύX���鏈���̏����ꂽ�֐��B
	 *	@param hModule ���W���[���n���h��(�ȗ�����ƃv���Z�X�̃n���h��( GetInstanceHandleByTnb(EI_Process) �̖߂�l)�ɂȂ�܂�)�B
	 *	@retval true �t�b�N�����B
	 *	@retval false �G���[�B
	 */
	bool Hook(LPCSTR lpszDllName, LPCSTR lpszFuncName, PFNC pFnc, HMODULE hModule = NULL)
	{
		Unhook();
		CWindowsVersion wv;
		if ( ! wv.IsWow64() )
		{
			if ( hModule == NULL )
			{
				hModule = TNB::GetInstanceHandleByTnb(EI_Process);
			}
			m_modules.RemoveAll();
			m_pOrignalFnc = m_Replace(hModule, lpszDllName, lpszFuncName, pFnc);
			if ( m_pOrignalFnc != NULL )
			{
				m_hModule		= hModule;
				m_strDllName	= lpszDllName;
				m_strFuncName	= lpszFuncName;
				return true;
			}
		}
		return false;
	}

	/**
	 * [���s] �����A���t�b�N
	 *	@note �t�b�N���Ă��Ȃ��Ɖ������܂���B
	 *	@retval true �A���t�b�N�����B
	 *	@retval false �G���[�B
	 */
	bool Unhook(void)
	{
		if ( m_pOrignalFnc != NULL )
		{
			LPCSTR lpszFuncName = m_strFuncName;
			m_modules.RemoveAll();
			if ( m_Replace(m_hModule, m_strDllName, lpszFuncName, m_pOrignalFnc) == NULL )
			{
				return false;
			}
			m_pOrignalFnc = NULL;
			m_strDllName.Empty();
			m_strFuncName.Empty();
			m_hModule = NULL;
		}
		return true;
	}

	/**
	 * [�擾] �t�b�N�O�̏����A�h���X
	 *	@retval NULL �t�b�N���Ă��Ȃ��B
	 *	@retval NULL�ȊO �ύX�O�̏����A�h���X�B
	 */
	PFNC operator()(void)
	{
		return m_pOrignalFnc;
	}

private:
	PFNC					m_pOrignalFnc;	///< �t�b�N�����̃A�h���X
	HMODULE					m_hModule;		///< �t�b�N�������W���[��
	CSimpleAscii			m_strDllName;	///< �t�b�N����DLL��
	CSimpleAscii			m_strFuncName;	///< �t�b�N�����֐���
	CSimpleVectorT<HMODULE>	m_modules;		///< ���W���[���Ǘ�

	/// DOS�w�b�_�ʒu�v�Z
	LPVOID m_DosHead(IMAGE_DOS_HEADER* pDos, DWORD offset)
	{
		return reinterpret_cast<LPVOID>(reinterpret_cast<DWORD>(pDos) + offset);
	}

	/// �ύX
	PFNC m_Replace(HMODULE hModule, LPCSTR lpszDllName, LPCSTR lpszFuncName, PFNC pFnc)
	{
		IMAGE_DOS_HEADER* pDOS = reinterpret_cast<IMAGE_DOS_HEADER*>(hModule);
		ASSERT( ! ::IsBadReadPtr(pDOS, sizeof(IMAGE_DOS_HEADER)) );
		if ( pDOS->e_magic != IMAGE_DOS_SIGNATURE )
		{
			return NULL;
		}
		IMAGE_NT_HEADERS* pPE = static_cast<IMAGE_NT_HEADERS*>(m_DosHead(pDOS, pDOS->e_lfanew));
		if ( pPE->Signature != IMAGE_NT_SIGNATURE )
		{
			return NULL;
		}
		IMAGE_DATA_DIRECTORY* pDataDir 
					= reinterpret_cast<IMAGE_DATA_DIRECTORY*>(pPE->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT);
		if ( pDataDir->VirtualAddress == 0 || pDataDir->Size == 0 )
		{
			return NULL;
		}
		m_modules.Add(hModule);
		PFNC pResultFnc = NULL;
		//-- ����DLL������
		// import DLL table
		IMAGE_IMPORT_DESCRIPTOR* pImpDsc = static_cast<IMAGE_IMPORT_DESCRIPTOR*>(m_DosHead(pDOS, pDataDir->VirtualAddress));
		for ( ; pImpDsc->FirstThunk != NULL; pImpDsc++ )
		{
			LPCSTR lpszName = static_cast<LPCSTR>(m_DosHead(pDOS, pImpDsc->Name));
			TTRACE1A("Found DLL = [%s]\n", lpszName);
			HMODULE h = ::GetModuleHandleA(lpszName);
			if ( h != NULL )
			{
				bool isFound = false;
				loop ( i, m_modules.GetSize() )
				{
					if ( m_modules[i] == h )
					{
						isFound = true;
						break;
					}
				}
				if ( ! isFound )
				{
					PFNC PF = m_Replace(h, lpszDllName, lpszFuncName, pFnc);
					if ( PF != NULL )
					{
						if ( pResultFnc != NULL && pResultFnc != PF )
						{
							ASSERTLIB( false );
						}
						pResultFnc = PF;
					}
				}
			}
		}
		//
		pImpDsc = static_cast<IMAGE_IMPORT_DESCRIPTOR*>(m_DosHead(pDOS, pDataDir->VirtualAddress));
		for ( ; pImpDsc->FirstThunk != NULL; pImpDsc++ )
		{
			LPCSTR lpszName = static_cast<LPCSTR>(m_DosHead(pDOS, pImpDsc->Name));
			TTRACE1A("Found DLL = [%s]\n", lpszName);
			if ( lpszDllName != NULL && stricmp(lpszName, lpszDllName) != 0 )
			{
				continue;
			}
			//-- �Ώۂ�DLL����
			// address table
			IMAGE_THUNK_DATA* pIAT = static_cast<IMAGE_THUNK_DATA*>(m_DosHead(pDOS, pImpDsc->FirstThunk));
			// name table
			IMAGE_THUNK_DATA* pINT = static_cast<IMAGE_THUNK_DATA*>(m_DosHead(pDOS, pImpDsc->OriginalFirstThunk));
			for ( ; pIAT->u1.Function != NULL; pIAT++, pINT++ ) 
			{
				if ( IMAGE_SNAP_BY_ORDINAL(pINT->u1.Ordinal) )
				{
					continue;
				}
				IMAGE_IMPORT_BY_NAME* pImpName = static_cast<IMAGE_IMPORT_BY_NAME*>(m_DosHead(pDOS, pINT->u1.AddressOfData));
				LPCSTR lpszName = reinterpret_cast<LPCSTR>(pImpName->Name);
				TTRACE1A("  - Found Func = [%s]\n", lpszName);
				if( strcmp(lpszName, lpszFuncName) == 0 ) 
				{
					//-- �Ώۂ̊֐�����
					DWORD oldProt;
					::VirtualProtect(&pIAT->u1.Function, sizeof(DWORD), PAGE_READWRITE, &oldProt);
					PFNC PF = reinterpret_cast<PFNC>(pIAT->u1.Function);
					ASSERTLIB( PF != NULL );
					pIAT->u1.Function = reinterpret_cast<DWORD>(pFnc);
					DWORD dummy;
					::VirtualProtect(&pIAT->u1.Function, sizeof(DWORD), oldProt, &dummy);
					if ( pResultFnc != NULL && pResultFnc != PF )
					{
						ASSERTLIB( false );
					}
					pResultFnc = PF;
					break;
				}
			}
			if ( lpszDllName != NULL )
			{
				break;
			}
		}
		return pResultFnc;
	}
};



};//TNB
