#pragma once
/**
 *@file
 * ICMP処理関係のヘッダ
 *
 *		PINGなどが有ります。
 *	
 *	@note	マクロ "_TnbWINSOCK2_ENABLE" が定義されていると、 Winsock2.0 を使用します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbException.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#ifndef _TnbWINSOCK2_ENABLE
 #include <Winsock.h>
 #define SD_BOTH         0x02
 #ifndef _WIN32_WCE
  #pragma comment(lib,"Wsock32.lib")
 #else
  #pragma comment(lib,"Winsock.lib")
 #endif
#else
 #pragma message("message : Winsock2.0 を使用します。")
 #include <Winsock2.h>
 #define _WSPIAPI_COUNTOF
 #include <Ws2Tcpip.h>
 #ifndef _WIN32_WCE
  #pragma comment(lib,"WS2_32.lib")
 #else
  #pragma comment(lib,"WS2.lib")
 #endif
 #include <mswsock.h>
#endif

#ifndef _WIN32_WCE
	#define IPAddr ULONG		// An IP address.
	#include <IPEXPORT.H>
  #if 0
	struct IP_OPTION_INFORMATION {
		UCHAR	Ttl;				// Time To Live
		UCHAR	Tos;				// Type Of Service
		UCHAR	Flags;				// IP header flags
		UCHAR	OptionsSize;		// Size in bytes of options data
		PUCHAR	OptionsData;		// Pointer to options data
	};
	struct ICMP_ECHO_REPLY {
		IPAddr	Address;			// Replying address
		ULONG	Status;				// Reply IP_STATUS
		ULONG	RoundTripTime;		// RTT in milliseconds
		USHORT	DataSize;			// Reply data size in bytes
		USHORT	Reserved;			// Reserved for system use
		PVOID	Data;				// Pointer to the reply data
		struct IP_OPTION_INFORMATION Options; // Reply options
	};
  #endif
#else
	#include <Icmpapi.h>
#endif

#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup SOCKET
 * ICMP API管理クラス
 *
 *		ICMP.dllをロードし、関数を実行することが出来ます。
 *
 *	@par必要ファイル
 *			TnbIcmp.h
 *
 *	@date 06/06/22 新規
 *	@date 09/09/04 構成整理
 */
class CIcmpFunction : CCopyImpossible
{
public:

	/// コンストラクタ
	CIcmpFunction(void) : m_hModule(NULL), m_pCreateFile(NULL), m_pCloseHandle(NULL), m_pSendEcho(NULL)
	{
	}

	/// デストラクタ
	~CIcmpFunction(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	/**
	 * [確認] ロード済み確認
	 *	@retval true LoadModule 成功済み。
	 *	@retval false 未ロード。
	 */
	bool HasModule(void) const
	{
		return m_hModule != NULL;
	}

	/**
	 * [確認] モジュールロード.
	 *		ICMP.dll をロードします。
	 *	@retval true 成功（すでにロード済みも含む）。
	 *	@retval false 失敗。
	 */
	bool LoadModule(void)
	{
		if ( m_hModule != NULL ){ return true; }
		#ifndef _WIN32_WCE
			m_hModule = ::LoadLibraryW(L"ICMP.dll");
		#else
			m_hModule = ::LoadLibraryW(L"Iphlpapi.dll");
		#endif
		if ( m_hModule != NULL )
		{
			m_pCreateFile = reinterpret_cast<P_IcmpCreateFile>(::GetProcAddressA(m_hModule, "IcmpCreateFile"));
			m_pCloseHandle = reinterpret_cast<P_IcmpCloseHandle>(::GetProcAddressA(m_hModule, "IcmpCloseHandle"));
			m_pSendEcho = reinterpret_cast<P_IcmpSendEcho>(::GetProcAddressA(m_hModule, "IcmpSendEcho"));
		}
		return m_pCreateFile != NULL && m_pCloseHandle != NULL && m_pSendEcho != NULL;
	}

	/**
	 * [実行] IcmpCreateFileの実行
	 *	@see http://msdn.microsoft.com/library/en-us/iphlp/iphlp/icmpcreatefile.asp
	 *	@return ハンドル
	 *	@throw CNullPointerException モジュールがロードされていない時、スローされます。
	 */
	HANDLE CreateFile(void)
	{
		if ( m_pCreateFile == NULL ){ throw CNullPointerException(); }
		return m_pCreateFile();
	}

	/**
	 * [実行] IcmpCloseHandleの実行
	 *	@see http://msdn.microsoft.com/library/en-us/iphlp/iphlp/icmpclosehandle.asp
	 *	@param hHandle 本クラスの CreateFile でオープンしたハンドル。 
	 *	@retval true 成功
	 *	@retval false 失敗
	 *	@throw CNullPointerException モジュールがロードされていない時、スローされます。
	 */
	bool CloseHandle(HANDLE hHandle)
	{
		if ( m_pCloseHandle == NULL ){ throw CNullPointerException(); }
		return !! m_pCloseHandle(hHandle);
	}

	/**
	 * [実行] IcmpSendEchoの実行
	 *	@see http://msdn.microsoft.com/library/en-us/iphlp/iphlp/icmpsendecho.asp
	 *	@param icmpHandle			MSDN 参照
	 *	@param destinationAddress	MSDN 参照
	 *	@param pRequestData			MSDN 参照
	 *	@param requestSize			MSDN 参照
	 *	@param pRequestOptions		MSDN 参照
	 *	@param pReplyBuffer			MSDN 参照
	 *	@param replySize			MSDN 参照
	 *	@param timeout				MSDN 参照
	 *	@return 数。MSDN 参照。
	 *	@throw CNullPointerException モジュールがロードされていない時、スローされます。
	 */
	DWORD SendEcho(
				HANDLE					icmpHandle,
				IPAddr					destinationAddress,
				LPVOID					pRequestData,
				WORD					requestSize,
				IP_OPTION_INFORMATION*	pRequestOptions,
				LPVOID					pReplyBuffer,
				DWORD					replySize,
				DWORD					timeout )
	{
		if ( m_pSendEcho == NULL ){ throw CNullPointerException(); }
		return m_pSendEcho(
							icmpHandle,
							destinationAddress,
							pRequestData,
							requestSize,
							pRequestOptions,
							pReplyBuffer,
							replySize,
							timeout 
		);
	}

private:
	typedef HANDLE (WINAPI *P_IcmpCreateFile)(VOID);
	typedef BOOL (WINAPI *P_IcmpCloseHandle)(HANDLE);
	typedef DWORD (WINAPI *P_IcmpSendEcho)(HANDLE, IPAddr, LPVOID, WORD, IP_OPTION_INFORMATION*, void*, DWORD, DWORD);
	HMODULE					m_hModule;			///< モジュールハンドル
	P_IcmpCreateFile		m_pCreateFile;		///< 「CreateFile」の関数アドレス
	P_IcmpCloseHandle		m_pCloseHandle;		///< 「CloseHandle」の関数アドレス
	P_IcmpSendEcho			m_pSendEcho;		///< 「SendEcho」の関数アドレス
};



/**@ingroup SOCKET
 * PING処理クラス
 *
 *	@par必要ファイル
 *			TnbIcmp.h
 *
 *	@date 06/06/22	新規作成.
 *	@date 10/08/09	ConnectionCheck() 廃止。
 */
class CPing : CCopyImpossible
{
public:

	/// コンストラクタ
	CPing(void) : m_hPing(INVALID_HANDLE_VALUE), m_buf(BUFFER_LEN)
	{
	}

	/// デストラクタ
	~CPing(void)
	{
		Close();
	}

	/// [操作] ハンドルクローズ
	void Close(void)
	{
		if ( m_hPing != INVALID_HANDLE_VALUE )
		{
			m_icmp.CloseHandle(m_hPing);
			m_hPing = INVALID_HANDLE_VALUE;
		}
	}

	/**
	 * [実行] ECHO送信
	 *	@note 一度実行すると、 Close()を実行するか、デストラクタまで、
	 *			ハンドルはオープンしたままになります。
	 *	@param ipAddr IPアドレス
	 *	@param timeout タイムアウト(ms)。省略すると1s。
	 *	@param tryCount トライ回数。省略すると5回。
	 *	@retval -2 ICMPモジュールが見つからないエラー。
	 *	@retval -1 ICMPのハンドルが使用できないエラー。
	 *	@retval 0 応答無し
	 *	@retval 1以上 応答の数(最大でトライ回数になります)
	 */
	int Echo(IPAddr ipAddr, DWORD timeout = 1000, int tryCount = 5)
	{
		int r = m_Open();
		if ( r < 0 )
		{
			return r;
		}
		int c = 0;
		loop ( i, tryCount )
		{
			if ( m_SendEcho(ipAddr, timeout) )
			{
				c++;
			}
		}
		return c;
	}

private:
	enum 
	{
		BUFFER_LEN = sizeof(ICMP_ECHO_REPLY) + sizeof(DWORD)
	};
	/// ハンドルオープン
	int m_Open(void)
	{
		if ( m_hPing == INVALID_HANDLE_VALUE )
		{
			if ( ! m_icmp.LoadModule() )
			{ 
				return -1; //ICMPのハンドルが使用できないエラー。
			}
			m_hPing = m_icmp.CreateFile();
			if ( m_hPing == INVALID_HANDLE_VALUE )
			{
				return -2; //ICMPモジュールが見つからないエラー
			}
		}
		return 0; //成功
	}
	/**
	 * ECHO送信
	 *	@note オープンしていることが前提。
	 *	@param ipAddr IPアドレス
	 *	@param dwTimeout タイムアウト(ms)。
	 *	@retval true 応答あり
	 *	@retval false 応答無し
	 */
	bool m_SendEcho(IPAddr ipAddr, DWORD timeout)
	{
		DWORD dummy;		///< ワーク
		DWORD dwRetVal = m_icmp.SendEcho(
				m_hPing, ipAddr, &dummy, sizeof(DWORD),
				NULL, m_buf.Ref(), BUFFER_LEN, timeout
		);
		return dwRetVal != 0;
	}
	CIcmpFunction	m_icmp;			///< ICMP API管理クラス
	HANDLE			m_hPing;		///< PING用ハンドル
	CWorkMemT<char>	m_buf;			///< ワーク
};



}; //TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
