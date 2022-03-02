#pragma once
/**
 *@file
 * APIフック関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSimpleVector.h"
#include "TnbWindowsVersion.h"





//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * APIフック管理
 *
 *		プロセスにロードされている DLLのコール先をフックすることが出来ます。
 *		他のプロセスには影響を与えません。
 *
 *		主にデバグ用に処理を横取りすることを想定しています。
 *		
 *	@note	クラスメソッド Replace() を使うとすげ替えます（戻すことはできません）。
 *	@par使用例
 *	<table><tr><td><pre>
 *		typedef int (WINAPI* P_MessageBoxA)(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
 *		P_MessageBoxA g_pApiMsgBox;		//旧処理アドレス記憶
 *		//MessageBoxA の代わりに呼ぶ関数
 *		int WINAPI MyMessageBoxA(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
 *		{
 *			return g_pApiMsgBox(hwnd, "フック成功", "--", MB_ICONEXCLAMATION);
 *		}
 *		void foo(void)
 *		{
 *			g_pApiMsgBox = CApiHookerT<P_MessageBoxA>::Replace("USER32.DLL", "MessageBoxA", MyMessageBoxA);
 *			::MessageBoxA(hwnd, "そのまま", "!!", MB_OK | MB_ICONERROR);	//「フック成功」というダイアログがでる。		
 *		}
 *	</pre></td></tr>
 *
 *	@note	メンバ Hook() を使うと、 Unhook() で元に戻すことが出来ます。一時的な切り替えに使用できます。
 *	@par使用例
 *	<table><tr><td><pre>
 *		typedef int (WINAPI* P_MessageBoxA)(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
 *		//MessageBoxA の代わりに呼ぶ関数
 *		int WINAPI MyMessageBoxA(HWND hwnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
 *		{
 *			return MessageBoxW(hwnd, L"フック成功", L"--", MB_ICONEXCLAMATION);
 *		}
 *		void foo(void)
 *		{
 *			CApiHookerT<P_MessageBoxA> hooker;
 *			hooker.Hook("USER32.DLL", "MessageBoxA", MyMessageBoxA);
 *			::MessageBoxA(hwnd, "そのまま", "!!", MB_OK | MB_ICONERROR);	//「フック成功」というダイアログがでる。		
 *			hooker.Unhook();
 *			::MessageBoxA(hwnd, "そのまま", "!!", MB_OK | MB_ICONERROR);	//「そのまま」というダイアログがでる。		
 *		}
 *	</pre></td></tr>
 *
 *	@see http://www003.upp.so-net.ne.jp/kish/prog/apihook.html
 *
 *	@param PFNC 関数のアドレス型
 *	
 *	@par必要ファイル
 *			TnbApiHooker.h
 *
 *	@date 06/12/18 新規作成。
 *	@date 09/04/10 DLL下のDLLのテーブルも対象とした。
 */
template<class PFNC>
class CApiHookerT : CCopyImpossible
{
public:
	
	/**
	 * [実行] 処理変更
	 *	@note このクラスメソッドで変更すると元に戻せません。
	 *	@param lpszDllName ロードしているDLL名。NULLを指定するとすべてのDLLを処理対象にします。
	 *	@param lpszFuncName ロードしているDLL内の関数名。
	 *	@param pFnc 変更する処理の書かれた関数。
	 *	@param hModule モジュールハンドル(省略するとプロセスのハンドル( GetInstanceHandleByTnb(EI_Process) の戻り値)になります)。
	 *	@retval NULL エラー。
	 *	@retval NULL以外 成功。値は、変更前の処理アドレス。
	 */
	static PFNC Replace(LPCSTR lpszDllName, LPCSTR lpszFuncName, PFNC pFnc, HMODULE hModule = NULL)
	{
		CApiHookerT a;
		bool r = a.Hook(lpszDllName, lpszFuncName, pFnc, hModule);
		return r ? a() : NULL;
	}


	//----


	/// コンストラクタ
	CApiHookerT(void) : m_pOrignalFnc(NULL), m_hModule(NULL)
	{
	}

	/**
	 * [実行] 処理フック
	 *	@note 一インスタンスで、フック出来るのは一つだけです。
	 *	@note 既にフックしているとアンフックしてからフック処理します。
	 *	@param lpszDllName ロードしている DLL名。NULL を指定するとすべての DLLを処理対象にします。
	 *	@param lpszFuncName ロードしている DLL内の関数名。
	 *	@param pFnc 変更する処理の書かれた関数。
	 *	@param hModule モジュールハンドル(省略するとプロセスのハンドル( GetInstanceHandleByTnb(EI_Process) の戻り値)になります)。
	 *	@retval true フック成功。
	 *	@retval false エラー。
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
	 * [実行] 処理アンフック
	 *	@note フックしていないと何もしません。
	 *	@retval true アンフック成功。
	 *	@retval false エラー。
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
	 * [取得] フック前の処理アドレス
	 *	@retval NULL フックしていない。
	 *	@retval NULL以外 変更前の処理アドレス。
	 */
	PFNC operator()(void)
	{
		return m_pOrignalFnc;
	}

private:
	PFNC					m_pOrignalFnc;	///< フックしたのアドレス
	HMODULE					m_hModule;		///< フックしたモジュール
	CSimpleAscii			m_strDllName;	///< フックしたDLL名
	CSimpleAscii			m_strFuncName;	///< フックした関数名
	CSimpleVectorT<HMODULE>	m_modules;		///< モジュール管理

	/// DOSヘッダ位置計算
	LPVOID m_DosHead(IMAGE_DOS_HEADER* pDos, DWORD offset)
	{
		return reinterpret_cast<LPVOID>(reinterpret_cast<DWORD>(pDos) + offset);
	}

	/// 変更
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
		//-- 下のDLLを検索
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
			//-- 対象のDLL発見
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
					//-- 対象の関数発見
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
