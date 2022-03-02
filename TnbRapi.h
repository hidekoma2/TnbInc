#pragma once
/**
 *@file
 * RAPI �֌W�̃w�b�_
 *
 *	@note Windows Mobile �� PocketPC �� SDK ���K�v�ł��B���� SDK �̃C���X�g�[�����́A�u./Activesync/inc�v�Ɓu./Activesync/lib�v��
 *		���ꂼ��C���N���[�h�p�X�ƁA���C�u�����p�X�ɒǉ����Ă����K�v������܂��i�v���W�F�N�g�ݒ�ł��AVC++ �̊��ݒ�ł��ǂ���ł�OK�j�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifdef _WIN32_WCE
	#error TnbRapi.h is only supported on Windows NT Series platforms.
#endif // _WIN32_WCE

#include "TnbDef.h"
#include <rapi.h>
#pragma comment(lib,"rapi.lib")
#ifdef __MSVCPP6__
	#pragma comment(lib, "delayimp.lib")
	#pragma comment(linker,"/DELAYLOAD:rapi.dll")
#endif



//TNB Library
namespace TNB
{



/**
	@defgroup RAPI			�����[�gAPI�֌W		
		@ingroup WINAPI
			
		ActiveSync ���Őڑ����ꂽ Windows CE �������[�g�ő��삷��N���X�Q�ł��B

		PC ��ɂ���t�@�C���̂悤�ɃA�N�Z�X���邱�Ƃ��o���܂��B
		
		@note VC++ 6 �̏ꍇ�A rapi.dll �͒x�����[�h�����悤�ɂȂ��Ă��܂��B VC++ 6 �ȊO�́A�e�v���W�F�N�g�Ŏw�肷��K�v������܂��B
 */



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 // �������ς݃t���O
 _SELECTANY RAPIINIT* s_pRapiInit = NULL;
#endif



/**@ingroup RAPI
 * RAPI �Ǘ��N���X
 *
 *		RAPI �̏������A�j�����Ǘ����܂��B�܂��ARAPI �ڑ���Ԃ��m�F���邱�Ƃ��o���܂��B
 *
 *	@note �C���X�^���X�͍쐬���邱�Ƃ͏o���܂���B
 * 
 *	@par�K�v�t�@�C��
 *			TnbRapi.h
 *			\n Windows Mobile �� PocketPC �� SDK �� ./Activesync/inc/rapi.h �����K�v�ł��B
 *
 *	@date 09/04/24 �V�K�쐬
 *	@date 09/06/30 Shutdown() �ǉ��B
 *	@date 09/09/01 rapi.dll �̂Ȃ����ł��N���ł���悤�� rapi.dll ��x�����[�h�ɂ����B
 */
class CRapi
{
public:

	/**
	 * [�ݒ�] RAPI ������.
	 *	@note �v���O�����ň�x�����R�[������K�v������܂��B
	 *			�����I�ɃR�[������܂����A���Ԃ������邽�߁A
	 *			�v���O�����̍ŏ��ň�x�R�[�����Ă������ق����悢��������܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool Startup(void)
	{
		LONG* P = reinterpret_cast<LONG*>(&s_pRapiInit);
		if ( InterlockedCompareExchange(P, 1, NULL) == NULL )
		{
			HMODULE hModule = ::LoadLibraryA("rapi.dll");
			if ( hModule == NULL )
			{
				s_pRapiInit = NULL;
			}
			else
			{
				::FreeLibrary(hModule);
				ASSERT( s_pRapiInit != NULL );
				RAPIINIT* P = new RAPIINIT;
				P->cbSize = sizeof(RAPIINIT);
				P->heRapiInit = NULL;
				P->hrRapiInit = NULL;
				static CReject s_reject;
				::OutputDebugString(_T("::CeRapiInitEx() ��\n"));
				HRESULT hr = ::CeRapiInitEx(P);
				::OutputDebugString(_T("::CeRapiInitEx() ����\n"));
				if ( FAILED(hr) )
				{
					delete P;
					s_pRapiInit = NULL;
				}
				else
				{
					s_pRapiInit = P;
				}
			}
		}
		while ( reinterpret_cast<UINT_PTR>(s_pRapiInit) == 1 )
		{
			::Sleep(10);
		} 
		return s_pRapiInit != NULL;
	}

	/**
	 * [�m�F] �T�|�[�g�m�F.
	 *	@note �V�X�e���� RAPI ���T�|�[�g���Ă��邩(�C���X�g�[������Ă��邩)�A�m�F�ł��܂��B
	 *	@retval true �T�|�[�g
	 *	@retval false ���T�|�[�g(rapi.dll���Ȃ��A�Ȃ�)
	 */
	static bool IsSupported(void)
	{
		Startup();
		return (s_pRapiInit != NULL);
	}	

	/**
	 * [�m�F] �ڑ��m�F.
	 *	@note ���ڑ��̏ꍇ�A �ڑ������܂ŁA time �҂��Ƃ��o���܂��B
	 *	@param time �ڑ��҂��^�C��(ms)�B�ȗ�����ƂOms�B
	 *	@retval true �ڑ�
	 *	@retval false ���ڑ�
	 */
	static bool IsConnected(DWORD time = 0)
	{
		if ( IsSupported() )
		{
			DWORD dwWaitRet = ::WaitForSingleObject(s_pRapiInit->heRapiInit, time);
			return (dwWaitRet == WAIT_OBJECT_0);
		}
		return false;
	}	

	/**
	 * [�ݒ�] RAPI ����.
	 *		Startup()�ŏ��������� RAPI ���������܂��B
	 *	@note �ʏ�g�p����K�v�͂���܂���B DLL ���Ŏg�p���Ă���ꍇ�ȂǁA�u::CeRapiUninit() ���v��
	 *		�������ł܂�ꍇ�A�����I�Ɏg�p���܂��B
	 *		
	 */
	static void Shutdown(void)
	{
		while ( reinterpret_cast<UINT_PTR>(s_pRapiInit) == 1 )
		{
			::Sleep(10);
		} 
		if ( s_pRapiInit != NULL )
		{
			::OutputDebugString(_T("::CeRapiUninit() ��\n"));
			::CeRapiUninit();
			::OutputDebugString(_T("::CeRapiUninit() ����\n"));
			delete s_pRapiInit;
			s_pRapiInit = NULL;
		}
	}

private:
	/// �R���X�g���N�^
	CRapi(void) {}
	/// �j���p�N���X
	class CReject
	{
	public:
		/// �f�X�g���N�^
		~CReject(void)
		{
			Shutdown();
		}
	};
};



}; //TNB



