#pragma once
/**
 *@file
 * ICMP�����֌W�̃w�b�_
 *
 *		PING�Ȃǂ��L��܂��B
 *	
 *	@note	�}�N�� "_TnbWINSOCK2_ENABLE" ����`����Ă���ƁA Winsock2.0 ���g�p���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbException.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#ifndef _TnbWINSOCK2_ENABLE
 #include <Winsock.h>
 #define SD_BOTH         0x02
 #ifndef _WIN32_WCE
  #pragma comment(lib,"Wsock32.lib")
 #else
  #pragma comment(lib,"Winsock.lib")
 #endif
#else
 #pragma message("message : Winsock2.0 ���g�p���܂��B")
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
 * ICMP API�Ǘ��N���X
 *
 *		ICMP.dll�����[�h���A�֐������s���邱�Ƃ��o���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbIcmp.h
 *
 *	@date 06/06/22 �V�K
 *	@date 09/09/04 �\������
 */
class CIcmpFunction : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CIcmpFunction(void) : m_hModule(NULL), m_pCreateFile(NULL), m_pCloseHandle(NULL), m_pSendEcho(NULL)
	{
	}

	/// �f�X�g���N�^
	~CIcmpFunction(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	/**
	 * [�m�F] ���[�h�ς݊m�F
	 *	@retval true LoadModule �����ς݁B
	 *	@retval false �����[�h�B
	 */
	bool HasModule(void) const
	{
		return m_hModule != NULL;
	}

	/**
	 * [�m�F] ���W���[�����[�h.
	 *		ICMP.dll �����[�h���܂��B
	 *	@retval true �����i���łɃ��[�h�ς݂��܂ށj�B
	 *	@retval false ���s�B
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
	 * [���s] IcmpCreateFile�̎��s
	 *	@see http://msdn.microsoft.com/library/en-us/iphlp/iphlp/icmpcreatefile.asp
	 *	@return �n���h��
	 *	@throw CNullPointerException ���W���[�������[�h����Ă��Ȃ����A�X���[����܂��B
	 */
	HANDLE CreateFile(void)
	{
		if ( m_pCreateFile == NULL ){ throw CNullPointerException(); }
		return m_pCreateFile();
	}

	/**
	 * [���s] IcmpCloseHandle�̎��s
	 *	@see http://msdn.microsoft.com/library/en-us/iphlp/iphlp/icmpclosehandle.asp
	 *	@param hHandle �{�N���X�� CreateFile �ŃI�[�v�������n���h���B 
	 *	@retval true ����
	 *	@retval false ���s
	 *	@throw CNullPointerException ���W���[�������[�h����Ă��Ȃ����A�X���[����܂��B
	 */
	bool CloseHandle(HANDLE hHandle)
	{
		if ( m_pCloseHandle == NULL ){ throw CNullPointerException(); }
		return !! m_pCloseHandle(hHandle);
	}

	/**
	 * [���s] IcmpSendEcho�̎��s
	 *	@see http://msdn.microsoft.com/library/en-us/iphlp/iphlp/icmpsendecho.asp
	 *	@param icmpHandle			MSDN �Q��
	 *	@param destinationAddress	MSDN �Q��
	 *	@param pRequestData			MSDN �Q��
	 *	@param requestSize			MSDN �Q��
	 *	@param pRequestOptions		MSDN �Q��
	 *	@param pReplyBuffer			MSDN �Q��
	 *	@param replySize			MSDN �Q��
	 *	@param timeout				MSDN �Q��
	 *	@return ���BMSDN �Q�ƁB
	 *	@throw CNullPointerException ���W���[�������[�h����Ă��Ȃ����A�X���[����܂��B
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
	HMODULE					m_hModule;			///< ���W���[���n���h��
	P_IcmpCreateFile		m_pCreateFile;		///< �uCreateFile�v�̊֐��A�h���X
	P_IcmpCloseHandle		m_pCloseHandle;		///< �uCloseHandle�v�̊֐��A�h���X
	P_IcmpSendEcho			m_pSendEcho;		///< �uSendEcho�v�̊֐��A�h���X
};



/**@ingroup SOCKET
 * PING�����N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbIcmp.h
 *
 *	@date 06/06/22	�V�K�쐬.
 *	@date 10/08/09	ConnectionCheck() �p�~�B
 */
class CPing : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CPing(void) : m_hPing(INVALID_HANDLE_VALUE), m_buf(BUFFER_LEN)
	{
	}

	/// �f�X�g���N�^
	~CPing(void)
	{
		Close();
	}

	/// [����] �n���h���N���[�Y
	void Close(void)
	{
		if ( m_hPing != INVALID_HANDLE_VALUE )
		{
			m_icmp.CloseHandle(m_hPing);
			m_hPing = INVALID_HANDLE_VALUE;
		}
	}

	/**
	 * [���s] ECHO���M
	 *	@note ��x���s����ƁA Close()�����s���邩�A�f�X�g���N�^�܂ŁA
	 *			�n���h���̓I�[�v�������܂܂ɂȂ�܂��B
	 *	@param ipAddr IP�A�h���X
	 *	@param timeout �^�C���A�E�g(ms)�B�ȗ������1s�B
	 *	@param tryCount �g���C�񐔁B�ȗ������5��B
	 *	@retval -2 ICMP���W���[����������Ȃ��G���[�B
	 *	@retval -1 ICMP�̃n���h�����g�p�ł��Ȃ��G���[�B
	 *	@retval 0 ��������
	 *	@retval 1�ȏ� �����̐�(�ő�Ńg���C�񐔂ɂȂ�܂�)
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
	/// �n���h���I�[�v��
	int m_Open(void)
	{
		if ( m_hPing == INVALID_HANDLE_VALUE )
		{
			if ( ! m_icmp.LoadModule() )
			{ 
				return -1; //ICMP�̃n���h�����g�p�ł��Ȃ��G���[�B
			}
			m_hPing = m_icmp.CreateFile();
			if ( m_hPing == INVALID_HANDLE_VALUE )
			{
				return -2; //ICMP���W���[����������Ȃ��G���[
			}
		}
		return 0; //����
	}
	/**
	 * ECHO���M
	 *	@note �I�[�v�����Ă��邱�Ƃ��O��B
	 *	@param ipAddr IP�A�h���X
	 *	@param dwTimeout �^�C���A�E�g(ms)�B
	 *	@retval true ��������
	 *	@retval false ��������
	 */
	bool m_SendEcho(IPAddr ipAddr, DWORD timeout)
	{
		DWORD dummy;		///< ���[�N
		DWORD dwRetVal = m_icmp.SendEcho(
				m_hPing, ipAddr, &dummy, sizeof(DWORD),
				NULL, m_buf.Ref(), BUFFER_LEN, timeout
		);
		return dwRetVal != 0;
	}
	CIcmpFunction	m_icmp;			///< ICMP API�Ǘ��N���X
	HANDLE			m_hPing;		///< PING�p�n���h��
	CWorkMemT<char>	m_buf;			///< ���[�N
};



}; //TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
