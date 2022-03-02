#pragma once
/**
 *@file
 * IP HELPER API�֌W�̃w�b�_
 *
 *	@note Windows �� IPHELPER�|API ���g�p���Ă��܂��B
 *
 *	Windows SDK ���K�{�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbIcmp.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#include <iprtrmib.h>
 #ifndef UDP6_STATS
	//�Â�SDK���ƈȉ��̐錾���Ȃ��̂ŁA�b��錾
	typedef struct _MIBICMPSTATS_EX
	{
		DWORD		dwMsgs;
		DWORD		dwErrors;
		DWORD		rgdwTypeCount[256];
	} MIBICMPSTATS_EX, *PMIBICMPSTATS_EX;	
	typedef struct _MIB_ICMP_EX
	{
		MIBICMPSTATS_EX icmpInStats;
		MIBICMPSTATS_EX icmpOutStats;
	} MIB_ICMP_EX,*PMIB_ICMP_EX;
 #endif
#endif
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")



//TNB Library
namespace TNB
{



/**@ingroup RAS
 * IP HELPER�N���X
 *
 *		IP HELPER API�����b�v�����N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbIpHelper.h
 *
 *	@date 08/07/14�G�V�K�쐬�B
 */
class CIpHelper : CCopyImpossible
{
public:

	/**
	 * IP�A�h���X����MIB-INDEX���擾
	 *	@param lpszAddress �A�h���X�B"xx.xx.xx.xx"�ł����O��OK
	 *	@retval INVALID_INDEX	�G���[(�w��̃A�h���X���Ȃ���)�B
	 *	@retval INVALID_INDEX�ȊO	�����B�l��MIB����INDEX�B
	 */
	static INDEX GetMibIndex(LPCSTR lpszAddress)
	{
		if ( lpszAddress == NULL || lpszAddress[0] == 0 )
		{
			return INVALID_INDEX;
		} 
		DWORD dwIp = ::inet_addr(lpszAddress);
		if ( dwIp == INADDR_NONE )
		{
			// �h�b�g�`���łȂ��Ƃ�
			LPHOSTENT ptHost = ::gethostbyname(lpszAddress);
			if ( ptHost == NULL )
			{
				TRACE2A( "::gethostbyname(%s) error = %d\n", lpszAddress, ::WSAGetLastError() );
				return INVALID_INDEX;
			}
			dwIp = *(reinterpret_cast<DWORD*>(ptHost->h_addr));
		}
		//
		DWORD d = 0;
		::GetIpAddrTable(NULL, &d, FALSE);
		CWorkMem mem(d);
		MIB_IPADDRTABLE* P = reinterpret_cast<MIB_IPADDRTABLE*>(mem.Ref());
		if ( ::GetIpAddrTable(P, &d, FALSE) == NO_ERROR )
		{
			loop ( i, P->dwNumEntries )
			{
				if ( dwIp == P->table[i].dwAddr )
				{
					return P->table[i].dwIndex;
				}
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [�擾] MIB-INDEX�̏��擾
	 *	@param[in]	dwMibIndex	MIB-INDEX
	 *	@param[out] _tMibIfRow	���
	 *	@retval true �����BtMibIfRow�͗L���B
	 *	@retval false ���s�BtMibIfRow�͖����i�w���INDEX���Ȃ��ꍇ�j�B
	 */
	static bool GetMibIfRow(INDEX dwMibIndex, MIB_IFROW& _tMibIfRow)
	{
		DWORD d = 0;
		::GetIpAddrTable(NULL, &d, FALSE);
		CWorkMem mem(d);
		MIB_IFTABLE* P = reinterpret_cast<MIB_IFTABLE*>(mem.Ref());
		if ( ::GetIfTable(P, &d, FALSE) == NO_ERROR )
		{
			loop ( i, P->dwNumEntries )
			{
				if ( P->table[i].dwIndex == dwMibIndex )
				{
					_tMibIfRow = P->table[i];
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * [�擾] MIB-INDEX�̏��擾
	 *	@param lpszAddress �A�h���X�B"xx.xx.xx.xx"�ł����O��OK
	 *	@param[out] _tMibIfRow	���
	 *	@retval true �����BtMibIfRow�͗L���B
	 *	@retval false ���s�BtMibIfRow�͖����i�w���INDEX���Ȃ��ꍇ�j�B
	 */
	static bool GetMibIfRow(LPCSTR lpszAddress, MIB_IFROW& _tMibIfRow)
	{
		INDEX r = GetMibIndex(lpszAddress);
		if ( r != INVALID_INDEX )
		{
			return GetMibIfRow(r, _tMibIfRow);
		}
		return false;
	}

	/**
	 * [�擾] MAC�A�h���X�擾
	 *	@param[out] _mac �}�b�N�A�h���X�B
	 *	@param lpszAddress �A�h���X�B"xx.xx.xx.xx"�ł����O��OK
	 *	@retval false		�G���[(�w��̃A�h���X���Ȃ���)�B
	 *	@retval true		�����B
	 */
	static bool GetMacAddress(CWorkMem& _mac, LPCSTR lpszAddress)
	{
		IPAddr ipAddr = ::inet_addr(lpszAddress);
		if ( ipAddr == INADDR_NONE )
		{
			return false;
		}
		ULONG aulMac[2]	= { 0xFFFFFFFF, 0xFFFFFFFF };
		ULONG ulLen = 6;
		HRESULT	hr = ::SendARP(ipAddr, 0, aulMac, &ulLen);
		if ( FAILED(hr) )
		{
			return false;
		}
		CStr s;
		BYTE* pbHexMac = reinterpret_cast<BYTE*>(aulMac);
		_mac.Resize(ulLen);
		MemCopy(_mac.Ref(), pbHexMac, ulLen);
		return true;
	}
};



}; // TNB





#if 0

		// �M�����x���擾����

		IP_INTERFACE_INFO* pIfTable;
		ULONG dwOutBufLen;

		if ( ::GetInterfaceInfo( NULL, &dwOutBufLen ) == ERROR_INSUFFICIENT_BUFFER )
		{
			 pIfTable = (IP_INTERFACE_INFO*)malloc( dwOutBufLen );
		}

		DWORD dwRet = ::GetInterfaceInfo( pIfTable, &dwOutBufLen );
		if ( dwRet == NO_ERROR )
		{
			// pIfTable->NumAdapters���̃f�[�^���擾�ł����̂ŁA
			// �Y������A�_�v�^��pIfTable->Adapter[ index ].Name����
			// {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}�̕������擾
		}


		TCHAR szFileName[ MAX_PATH ];
		_stprintf( szFileName, _T( "\\\\.\\%s" ), szAdapterName );

		HANDLE hDevice = ::CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
									   NULL, OPEN_EXISTING, 0, NULL);




		#define MAX_BSSID 16
		#define BUFFER_SIZE ( sizeof(NDIS_WLAN_BSSID) * MAX_BSSID + sizeof(ULONG) )

		NDIS_802_11_BSSID_LIST* pBSSIDList = (NDIS_802_11_BSSID_LIST*)malloc( BUFFER_SIZE );
		if ( pBSSIDList == NULL )
		{
			return NULL;
		}

		DWORD nBytesReturned;
		DWORD dwIoControlCode;

		::ZeroMemory( pBSSIDList, BUFFER_SIZE );
		dwIoControlCode = OID_802_11_BSSID_LIST_SCAN;

		::DeviceIoControl( hDevice, IOCTL_NDIS_QUERY_GLOBAL_STATS,
						   &dwIoControlCode, sizeof(dwIoControlCode),
						   (LPVOID)NULL, 0,
						   &nBytesReturned, NULL );

		// wait
		::Sleep( 2000 );

		::ZeroMemory( pBSSIDList, BUFFER_SIZE );
		dwIoControlCode = OID_802_11_BSSID_LIST;

		if ( ::DeviceIoControl( hDevice, IOCTL_NDIS_QUERY_GLOBAL_STATS,
								&dwIoControlCode, sizeof(dwIoControlCode),
								pBSSIDList, sizeof(NDIS_802_11_BSSID_LIST) * MAX_BSSID,
								&nBytesReturned, NULL ) )
		{
			return pBSSIDList;
		}
		else
		{
			return NULL;
		}


		NDIS_WLAN_BSSID* pBssid= pBSSIDList->Bssid;
		for( int i = 0; i < pBSSIDList->NumberOfItems; i++ )
		{
			// �\��
			USES_CONVERSION;
			TRACE( _T("%s: %d\n"), A2T( (LPCSTR)pBssid->Ssid.Ssid ), pBssid->Rssi );
    
			pBssid = (NDIS_WLAN_BSSID*)( (BYTE*)pBssid + pBssid->Length );
		}
#endif
