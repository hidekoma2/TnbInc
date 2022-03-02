#pragma once
/**
 *@file
 * RAPI 関係のヘッダ
 *
 *	@note Windows Mobile や PocketPC の SDK が必要です。この SDK のインストール下の、「./Activesync/inc」と「./Activesync/lib」を
 *		それぞれインクルードパスと、ライブラリパスに追加しておく必要があります（プロジェクト設定でも、VC++ の環境設定でもどちらでもOK）。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
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
	@defgroup RAPI			リモートAPI関係		
		@ingroup WINAPI
			
		ActiveSync 等で接続された Windows CE をリモートで操作するクラス群です。

		PC 上にあるファイルのようにアクセスすることが出来ます。
		
		@note VC++ 6 の場合、 rapi.dll は遅延ロードされるようになっています。 VC++ 6 以外は、各プロジェクトで指定する必要があります。
 */



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 // 初期化済みフラグ
 _SELECTANY RAPIINIT* s_pRapiInit = NULL;
#endif



/**@ingroup RAPI
 * RAPI 管理クラス
 *
 *		RAPI の初期化、破棄を管理します。また、RAPI 接続状態を確認することが出来ます。
 *
 *	@note インスタンスは作成することは出来ません。
 * 
 *	@par必要ファイル
 *			TnbRapi.h
 *			\n Windows Mobile や PocketPC の SDK の ./Activesync/inc/rapi.h 等も必要です。
 *
 *	@date 09/04/24 新規作成
 *	@date 09/06/30 Shutdown() 追加。
 *	@date 09/09/01 rapi.dll のない環境でも起動できるように rapi.dll を遅延ロードにした。
 */
class CRapi
{
public:

	/**
	 * [設定] RAPI 初期化.
	 *	@note プログラムで一度だけコールする必要があります。
	 *			自動的にコールされますが、時間がかかるため、
	 *			プログラムの最初で一度コールしておいたほうがよいかもしれません。
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
				::OutputDebugString(_T("::CeRapiInitEx() 中\n"));
				HRESULT hr = ::CeRapiInitEx(P);
				::OutputDebugString(_T("::CeRapiInitEx() 完了\n"));
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
	 * [確認] サポート確認.
	 *	@note システムで RAPI をサポートしているか(インストールされているか)、確認できます。
	 *	@retval true サポート
	 *	@retval false 未サポート(rapi.dllがない、など)
	 */
	static bool IsSupported(void)
	{
		Startup();
		return (s_pRapiInit != NULL);
	}	

	/**
	 * [確認] 接続確認.
	 *	@note 未接続の場合、 接続されるまで、 time 待つことが出来ます。
	 *	@param time 接続待ちタイム(ms)。省略すると０ms。
	 *	@retval true 接続
	 *	@retval false 未接続
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
	 * [設定] RAPI 解除.
	 *		Startup()で初期化した RAPI を解除します。
	 *	@note 通常使用する必要はありません。 DLL 内で使用している場合など、「::CeRapiUninit() 中」で
	 *		処理が固まる場合、明示的に使用します。
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
			::OutputDebugString(_T("::CeRapiUninit() 中\n"));
			::CeRapiUninit();
			::OutputDebugString(_T("::CeRapiUninit() 完了\n"));
			delete s_pRapiInit;
			s_pRapiInit = NULL;
		}
	}

private:
	/// コンストラクタ
	CRapi(void) {}
	/// 破棄用クラス
	class CReject
	{
	public:
		/// デストラクタ
		~CReject(void)
		{
			Shutdown();
		}
	};
};



}; //TNB



