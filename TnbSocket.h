#pragma once
/**
 *@file
 * �\�P�b�g�̃A�N�Z�X�֌W�̃w�b�_
 *
 *		�T�[�o�ƃN���C�A���g�̃N���X������܂��B
 *		��M�����́A���X�i�[��o�^���邱�Ƃōs���܂��B
 *		�T�[�o�̓}���`�N���C�A���g���T�|�[�g���Ă��܂��B
 *
 *	@todo IPv4 �̂ݑΉ��B IPv6 �͖��Ή��B
 *
 *	@note Winsock ���g�p���Ă��܂��B
 *
 *	@note	�}�N�� "_TnbWINSOCK2_ENABLE" ����`����Ă���ƁA Winsock2.0 ���g�p���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAbstractReport.h"
#include "TnbThread.h"
#include "TnbVector.h"
#include "TnbIcmp.h"



//TNB Library
namespace TNB
{



/// SOCKET�̔z��^�錾
typedef TNB::CVectorT<SOCKET> CSocketVector;



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 _SELECTANY bool s_boIsInitSock = false;
 _SELECTANY int s_iMaxUdpDg = 0;
#endif



 /**@ingroup REPORT SOCKET
 * Socket���ʃA�h���X�Ǘ��N���X
 *
 *		��΂̃|�[�g No �� IP �A�h���X���Ǘ����܂��B
 *
 *	@note IP �A�h���X�𕶎���œo�^���Ă���ꍇ�A�A�h���X�擾���� gethostbyname() ���g���ď������Ă��܂��B
 *
 *	@note �{�N���X��������߂�l�ň��� DWORD �^�� IP �A�h���X�AWORD �^�̃|�[�g No �́A
 *		Host Byte Order �ɂȂ�܂��Bsocket �̊֐��Ɏg�p����ꍇ�A���ӂ��Ă��������B
 *		�l�� Network Byte Order �̏ꍇ�A���L���Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSocket.h
 * 
 *	@date 09/08/27	�V�K�쐬
 */
class CSocketAddress
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param portNo �|�[�g No�B�ȗ�����ƁA�O�ɂȂ�܂��B
	 *	@param ipAddress IP �A�h���X�B�ȗ�����ƁA INADDR_ANY �ɂȂ�܂��B
	 */
	CSocketAddress(WORD portNo = 0, DWORD ipAddress = 0) : m_portNo(portNo), m_isNameMode(false)
	{
		m_inaddr.s_addr = ::htonl(ipAddress);
	}

	/**
	 * �R���X�g���N�^.
	 *	@note IP�A�h���X�́A "127.0.0.1" �� "LD04001" �Ȃǂ��w��ł��܂��B
	 *	@param portNo �|�[�gNo
	 *	@param lpszIpAddressName IP�A�h���X���B
	 */
	CSocketAddress(WORD portNo, LPCTSTR lpszIpAddressName) : m_portNo(portNo), m_isNameMode(true)
	{
		m_inaddr.s_addr = 0;
		m_ipAddressName = lpszIpAddressName;
	}

	/**
	 * [�ݒ�] �A�h���X�ݒ�.
	 *		IP�A�h���X�ƃ|�[�gNo��ݒ肵�܂��B
	 *	@param portNo �|�[�g No�B
	 *	@param ipAddress IP �A�h���X�B�ȗ�����ƁA INADDR_ANY �ɂȂ�܂��B
	 */
	void Set(WORD portNo, DWORD ipAddress = 0)
	{
		m_isNameMode = false;
		m_inaddr.s_addr = ::htonl(ipAddress);
		m_ipAddressName.Empty();
		m_portNo = portNo;
	}

	/**
	 * [�ݒ�] �A�h���X�ݒ�.
	 *		IP�A�h���X�ƃ|�[�gNo��ݒ肵�܂��B
	 *	@note IP�A�h���X�́A "127.0.0.1" �� "LD04001" �Ȃǂ��w��ł��܂��B
	 *	@param portNo �|�[�gNo
	 *	@param lpszIpAddressName IP�A�h���X���B
	 */
	void Set(WORD portNo, LPCTSTR lpszIpAddressName)
	{
		m_isNameMode = true;
		m_inaddr.s_addr = 0;
		m_portNo = portNo;
		m_ipAddressName = lpszIpAddressName;
	}

	/**
	 * [�ݒ�] �A�h���X�ݒ�.
	 *		IP�A�h���X�ƃ|�[�gNo��ݒ肵�܂��B
	 *	@param sockaddr Network Byte Order �̃|�[�g No �� IP �A�h���X
	 *				�iSOCKADDR_IN �� sin_addr �����o�� sin_port �����o�̂ݎQ�Ƃ��܂��j�B
	 */
	void Set(const SOCKADDR_IN& sockaddr)
	{
		m_isNameMode = false;
		m_inaddr = sockaddr.sin_addr;
		m_ipAddressName.Empty();
		m_portNo = ::ntohs(sockaddr.sin_port);
	}

	/**
	 * [�擾] IP�A�h���X�擾.
	 *	@param[out] _inaddr IP �A�h���X�� Network Byte Order �Ŋi�[����܂��B
	 */
	void Get(IN_ADDR& _inaddr) const
	{
		if ( ! m_isNameMode )
		{
			_inaddr = m_inaddr;
		}
		else
		{
			DWORD ad = ::inet_addr(m_ipAddressName);
			if ( ad == INADDR_NONE )
			{
				// �h�b�g�`���łȂ��Ƃ�
				LPHOSTENT ptHost = ::gethostbyname(m_ipAddressName);
				if ( ptHost == NULL )
				{
					TRACE2( "::gethostbyname(%s) error = %d\n", CStr(m_ipAddressName), ::WSAGetLastError() );
					ad = INADDR_NONE;
				}
				else
				{
					ad = *(reinterpret_cast<DWORD*>(ptHost->h_addr));
				}
			}
			_inaddr.s_addr = ad;
		}
	}

	/**
	 * [�擾] IP �A�h���X���|�[�g No �擾.
	 *	@param[out] _sockaddr IP �A�h���X�ƃ|�[�g No �� Network Byte Order �Ŋi�[����܂��B
	 *				�iSOCKADDR_IN �� sin_addr �����o�� sin_port �����o�̂ݍX�V���܂��j�B
	 */
	void Get(SOCKADDR_IN& _sockaddr) const
	{
		Get(_sockaddr.sin_addr);
		_sockaddr.sin_port = ::htons(m_portNo);
	}

	/**
	 * [�擾] �z�X�g���擾.
	 *		DNS ����ێ����Ă��� IP �A�h���X�̃z�X�g�����擾���܂��B
	 *	@note �ێ����Ă��� IP �A�h���X���A INADDR_ANY �̏ꍇ�A�����̃z�X�g�����擾����܂��B
	 *	@retval Empty ������Ȃ������B
	 *	@retval ��L�ȊO �z�X�g���B
	 */
	CStr GetHostName(void) const
	{
		IN_ADDR inaddr;
		Get(inaddr);
		hostent* P = ::gethostbyaddr(reinterpret_cast<const char*>(&inaddr), sizeof(inaddr), AF_INET);
		if ( P != NULL )
		{
			return CStr(P->h_name);
		}
		return _T("");
	}

	/**
	 * [�擾] IP �A�h���X������擾.
	 *	@param withDns true �Ȃ� DNS ����z�X�g�����擾�����݂܂��B�擾�ł��Ȃ������ꍇ�A���l�̗���("127.0.0.1") ��Ԃ��܂��B
	 *					false �Ȃ� DNS �ɖ₢���킹�����A���l�̗����Ԃ��܂��B
	 *	@return IP�A�h���X������
	 */
	CStr GetAddressString(bool withDns = true) const
	{
		if ( ! m_isNameMode )
		{
			if ( withDns && m_inaddr.s_addr != INADDR_ANY)
			{
				hostent* P = ::gethostbyaddr(reinterpret_cast<const char*>(&m_inaddr), sizeof(m_inaddr), AF_INET);
				if ( P != NULL )
				{
					return CStr(P->h_name);
				}
			}
			return CStr(::inet_ntoa(m_inaddr));
		}
		else
		{
			return CStr(m_ipAddressName);
		}
	}

	/**
	 * [�擾] IP �A�h���X�擾.
	 *	@return IP �A�h���X�B
	 */
	DWORD GetAddress(void) const
	{
		in_addr addr;
		Get(addr);
		return ::htonl(addr.s_addr);
	}

	/**
	 * [�擾] �|�[�gNo�擾.
	 *	@return �|�[�gNo
	 */
	WORD GetPortNo(void) const
	{
		return m_portNo;
	}

	/**
	 * [�m�F] �u���[�h�L���X�g�H.
	 *	@retval true �ێ����Ă�A�h���X�̓u���[�h�L���X�g.
	 *	@retval false ��
	 */
	bool IsBroadcast(void) const
	{
		return GetAddress() == INADDR_BROADCAST;
	}

	/**
	 * [�ϊ�] �o�C�g�I�[�_�[�ϊ�(host to network)
	 *		unsigned long (32bit) �� Host Byte Order ���� Network Byte Order �ɕϊ����܂��B
	 *	@note �uHost TO Network Long�v�̗��ł��B
	 *	@param hostlong Host Byte Order �̒l
	 *	@return Network Byte Order �̒l
	 */
	static DWORD htonl(DWORD hostlong)
	{
		return ::htonl(hostlong);
	}

	/**
	 * [�ϊ�] �o�C�g�I�[�_�[�ϊ�(host to network)
	 *		unsigned short (16bit) �� Host Byte Order ���� Network Byte Order �ɕϊ����܂��B
	 *	@note �uHost TO Network Short�v�̗��ł��B
	 *	@param hostshort Host Byte Order �̒l
	 *	@return Network Byte Order �̒l
	 */
	static WORD htons(WORD hostshort)
	{
		return ::htons(hostshort);
	}

	/**
	 * [�ϊ�] �o�C�g�I�[�_�[�ϊ�(network to host)
	 *		unsigned long (32bit) �� Network Byte Order ���� Host Byte Order �ɕϊ����܂��B
	 *	@note �uNetwork TO Host Long�v�̗��ł��B
	 *	@param netlong  Network Byte Order �̒l
	 *	@return  Host Byte Order �̒l
	 */
	static DWORD ntohl(DWORD netlong)
	{
		return ::ntohl(netlong);
	}

	/**
	 * [�ϊ�] �o�C�g�I�[�_�[�ϊ�(network to host)
	 *		unsigned short (16bit) �� Network Byte Order ���� Host Byte Order �ɕϊ����܂��B
	 *	@note �uNetwork TO Host Short�v�̗��ł��B
	 *	@param netshort  Network Byte Order �̒l
	 *	@return  Host Byte Order �̒l
	 */
	static DWORD ntohs(DWORD netshort)
	{
		return ::ntohl(netshort);
	}

private:
	bool		m_isNameMode;		///< true �Ȃ� m_addrName �ŊǗ��A false �Ȃ� m_inaddr �ŊǗ�
	IN_ADDR		m_inaddr;			///< IP�A�h���X�l(network byte order)
	CAscii		m_ipAddressName;	///< IP�A�h���X��
	WORD		m_portNo;			///< �|�[�gNo(host byte order)
};



/**@ingroup REPORT SOCKET
 * Socket���ʎ�M�C�x���g�Ǘ��N���X
 *
 *		���X�i�[�� OnSocketEvent() ���\�b�h�Œʒm�������ł��B
 *		�f�[�^�i�����ƃA�h���X�j���C�x���g�̂����ꂩ��ێ����܂��B
 *
 *	@see CAbstractSocket::IListener::OnSocketEvent()
 *
 *	@par�K�v�t�@�C��
 *			TnbSocket.h
 * 
 *	@date 06/04/01	�V�K�쐬
 *	@date 06/07/07	InterfaceID�T�O�A�ǉ�
 *	@date 09/08/27	�A�h���X�w��� CSocketAddress ���g���悤�ɕύX�B
 *	@date 11/06/14 GetInterfaceId() ���\�b�h�̖߂�l�ύX�B
 */
class CSocketEvent : public CReportEvent
{
	DEFSUPER(CReportEvent);
	SOCKET						m_soData;	///< SOCKET
	class CAbstractSocket *		m_pcSocket;	///< SOCKET Object�̎Q��
public:

	/**
	 * �R���X�g���N�^
	 *	@param pcSocket �\�P�b�g�N���X�̃|�C���^
	 *	@param so �\�P�b�g
	 *	@param eEvent �C�x���g�R�[�h�i�ȗ��\�j
	 *	@param dwError �G���[�R�[�h�i�ȗ��\�j
	 */
	CSocketEvent(CAbstractSocket* pcSocket, SOCKET so, EReportEvent eEvent = ReportEvent_Non, DWORD dwError = 0);

	/**
	 * [���M] �f�[�^���M
	 *	@note ���M���Ƀf�[�^�𑗐M���܂��B
	 *	@param dataSize ���M����f�[�^���B
	 *	@param lpcvData ���M����f�[�^�B
	 *	@retval true	�����B
	 *	@retval	false	���s�B
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData) const;

	/**
	 * [�擾] �C���^�[�t�F�[�XID�擾
	 *	@note �����I�ɒl�����߂��Ă��܂��܂����A��̐ڑ����A�l���ς�邱�Ƃ͗L��܂���B
	 *		�ʂ̐ڑ��ƒl���d�����邱�Ƃ��L��܂���B
	 *	@note	Socket �n�ł́A�C���^�[�t�F�[�XID�́A���[�U�͎w��ł��܂���B
	 *			SOCKET �n���h�� + 0x80000000�̒l�ɂȂ�܂��B
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const
	{
		return static_cast<DWORD>(m_soData) | 0x80000000;
	}

	/**
	 * [�擾] ���M�����擾
	 *	@note Data�������Ă���ꍇ�A���M��������̏��𓾂邱�Ƃ��o���܂��B
	 *	@param[out]	_address �A�h���X
	 *	@retval true �A�h���X�ƃ|�[�g�擾�����B
	 *	@retval false ���s�B
	 */
	bool GetPeerName(CSocketAddress& _address) const;

	/**
	 * [�擾] ��M�����擾
	 *	@param[out]	_address �A�h���X
	 *	@retval true �A�h���X�ƃ|�[�g�擾�����B
	 *	@retval false ���s�B
	 */
	bool GetSockName(CSocketAddress& _address) const;

	/**
	 * [�擾] �\�P�b�g�擾
	 *	@return �\�P�b�g
	 */
	SOCKET GetSocket(void) const
	{
		return m_soData;
	}
};



/**@ingroup REPORT SOCKET
 * Socket���ʃN���X
 *	
 *		Server��Client�̒��ۓI�ȃN���X�ł��B
 *		���[�U�����ڎg�����Ƃ͗L��܂���B
 *
 *	@date 06/04/01	�V�K�쐬
 *	@date 06/05/11	IReport ����
 *	@date 06/06/22	ping������ CPing �N���X�ɈϏ��B
 *	@date 06/07/07	SOCKADDR_IN�����b�s���O
 *	@date 07/02/09	�e�N���X�ύX�B
 *	@date 09/08/27	�\���ύX�B
 *	@date 09/08/27	�A�h���X�w��� CSocketAddress ���g���悤�ɕύX�B
 *	@date 12/03/13	Socket���ʃ��X�i�[���ɃR�[������悤�ɕύX�B
 *	@date 12/03/30	SetSocketListener() �� virtual ��t���B
 */
class CAbstractSocket : public CAbstractReport
{
	DEFSUPER(CAbstractReport);
	friend class CSocketEvent;		///< �t�����h�N���X��`
public:

	/**@ingroup REPORT SOCKET
	 * Socket���ʂ̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		CAbstractSocket �̃T�u�N���X�̒ʒm�p�C���^�[�t�F�[�X�ł��B
	 *
	 *	@see SetSocketListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbSocket.h
	 * 
	 *	@date 06/04/01	�V�K�쐬
	 */
	struct IListener
	{
		/** �f�X�g���N�^ */
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �C�x���g����.
		 *		Open�����f�o�C�X����f�[�^����M���������ȂǁA�R�[������܂��B
		 *	@note IReport �̃��X�i�[����ɒʒm����܂��B
		 *	@note ������ Disconnect() �����Ă��AReportEvent_Disconnect ���ʒm����܂��B
		 *	@note CSocketEvent �� Send() ���\�b�h���L��܂��B��M���đ����M����ꍇ�Ɏg�p�ł��܂��B
		 *	@note	�{�N���X�̃C�x���g�́u{@link TNB::ReportEvent_Error ReportEvent_Error}�v
		 *			�u{@link TNB::ReportEvent_Connect ReportEvent_Connect}�v
		 *			�u{@link TNB::ReportEvent_Disconnect ReportEvent_Disconnect}�v�ł��B
		 *	@param ev ��M�������e�i�f�[�^���C�x���g�j�B
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnSocketEvent(const CSocketEvent& ev) = 0;
	};


	//----------------------------


	/// �R���X�g���N�^ 
	CAbstractSocket(void) : _super(), m_piSocketListner(NULL), m_boIsDelay(true)
	{
		m_lpcBuffer = new char[E_BufferSize];
	}

	/// �f�X�g���N�^
	virtual ~CAbstractSocket(void)
	{
		delete[] m_lpcBuffer;
	}

	/**
	 * [�ݒ�] ���X�i�[�o�^
	 *	@note ���X�i�[��o�^���Ȃ��Ǝ�M��񂪎󂯂��܂���
	 *	@note �o�^�ł��� CAbstractSocket::IListener �͈�����ł��B
	 *	@param piSocketListener ���X�i�[
	 */
	virtual void SetSocketListener(IListener* piSocketListener)
	{
		m_piSocketListner = piSocketListener;
	}

	/**
	 * [�ݒ�] SOCKET������.
	 *		WSAStartup() �R�[�����܂��B���łɃR�[�����Ă���ꍇ�́A�������܂���B
	 *	@note	�ʏ�A�\�P�b�g�g�p���Ɏ����I�Ɏg�p����܂��̂ŁA�R�[������K�v�͗L��܂���B
	 *			TNB Library �ȊO�̃\�P�b�g�֌W���Ɏg�p����ꍇ�AWSAStartup() �̑����
	 *			�g�p���Ă��������B
	 */
	static void StartUp(void)
	{
		if ( s_boIsInitSock )
		{
			return;
		}
		s_boIsInitSock = true;
		#ifndef _WIN32_WCE
		SOCKET so = ::socket(AF_INET, SOCK_STREAM, 0);
		if ( so != SOCKET_ERROR )
		{
			::closesocket(so);
			return;
		}
		if ( ::WSAGetLastError() == WSANOTINITIALISED )
		#endif //_WIN32_WCE
		{
			WSADATA wsaData;
			if ( ::WSAStartup(E_DefaultVersion, &wsaData) == SOCKET_ERROR )
			{
				ASSERT1( false, "WSAStartup", "error = %d\n", ::WSAGetLastError() );
			}
			else
			{
				static CCleanup s_cleanup; // Cleanup�p�C���X�^���X
				if ( LOBYTE(wsaData.wVersion) == 1 )
				{
					s_iMaxUdpDg = wsaData.iMaxUdpDg;
				}
				else
				{
					s_iMaxUdpDg = 0;
				}
			}
		}
	}

	/**
	 * [�擾] ���h�o�A�h���X�擾.
	 *		�����̂h�o�A�h���X���擾���܂��B�Q�ȏ゠��ꍇ������܂��B
	 *	@return �����̂h�o�A�h���X
	 */
	static CDwordVector GetMyAddress(void)
	{
		CDwordVector v;
		CAscii na;
		::gethostname(na.GetBuffer(1000), 1000);
		na.ReleaseBuffer();
		hostent* pHostEnt = ::gethostbyname(na);
		if ( pHostEnt != NULL )
		{
			loop ( i, 1000 )
			{
				DWORD* p = reinterpret_cast<DWORD*>(pHostEnt->h_addr_list[i]);
				if ( p == NULL )
				{
					break;
				}
				v.Add(SwapEndian(*p));
			}
		}
		return v;
	}

protected:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	/// SOCKADDR_IN���b�s���O�N���X
	struct TSockAddr : SOCKADDR_IN
	{
		/// ������
		void Init(void) { memset(this, 0, sizeof(SOCKADDR_IN)); }
		/// �\���̃T�C�Y
		static int GetSize(void) { return sizeof(SOCKADDR_IN); }
		/// �A�h���X�v��
		operator LPSOCKADDR(void) { return reinterpret_cast<LPSOCKADDR>(this); }
	};

	enum
	{
		#ifdef _TnbWINSOCK2_ENABLE
		 E_DefaultVersion	= WINSOCK_VERSION,	///< Winsock�̃o�[�W����
		#else
		 E_DefaultVersion	= MAKEWORD(1, 1),	///< Winsock�̃o�[�W����
		#endif
		E_BufferSize		= 10000,	///< TCP�̂P��M�ő�T�C�Y		
		E_RetryDelay		= 2000,		///< ���g���C�܂ł�ms		
		E_SelectTimeout		= 2000,		///< select�̃^�C���A�E�gms	
		E_ConnectTimeout	= 2000,		///< connect�̃^�C���A�E�gms	
	};
	TSockAddr		m_sockaddrPartner;	///< �\�P�b�g�A�h���X
	IListener *		m_piSocketListner;	///< ���X�i�[
	char *			m_lpcBuffer;		///< ��M�p�o�b�t�@	
	bool			m_boIsDelay;		///< true�Ȃ�x�����M�Bfalse�Ȃ瑦���M�B

	/// WinsockCleanup�����N���X
	class CCleanup
	{
	public:~CCleanup(void) { ::WSACleanup(); }
	};

	/**
	 * IP�v�Z
	 *	@param[out] _dwIP �ϊ���� IP�B
	 *	@param[in] lpszAddress �A�h���X������B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool ms_AddressToIp(IN_ADDR& _addr, LPCTSTR lpszAddress)
	{
		StartUp();
		CSocketAddress a(0, lpszAddress);
		a.Get(_addr);
		return (_addr.s_addr != INADDR_NONE);
	}

	/**
	 * �A�h���X�ݒ�.
	 *		�����o�ɃA�h���X�A�|�[�gNO��ݒ肵�܂��B
	 *	@param lpszAddress IP�A�h���X
	 *	@param wPortNo �|�[�gNO
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool m_SetAddress(LPCTSTR lpszAddress, WORD wPortNo)
	{
		// �\�P�b�g�\���̂̏�����
		m_sockaddrPartner.Init();
		m_sockaddrPartner.sin_family = AF_INET;			// TCP/IP���w��
		CSocketAddress a(wPortNo, lpszAddress);
		a.Get(m_sockaddrPartner);
		return m_sockaddrPartner.sin_addr.s_addr != INADDR_NONE;
	}

	/**
	 * �A�h���X�ݒ�.
	 *		�����o�ɃA�h���X�A�|�[�gNo ��ݒ肵�܂��B
	 *	@param address IP�A�h���X & �|�[�gNo
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool m_SetAddress(const CSocketAddress& address)
	{
		// �\�P�b�g�\���̂̏�����
		m_sockaddrPartner.Init();
		m_sockaddrPartner.sin_family = AF_INET;			// TCP/IP���w��
		address.Get(m_sockaddrPartner);
		return m_sockaddrPartner.sin_addr.s_addr != INADDR_NONE;
	}

	/**
	 * ��M.
	 *		�N���X���̃o�b�t�@�Ɏ�M���܂��B
	 *	@param so �\�P�b�g
	 *	@retval true	����
	 *	@retval false	���s�i�ؒf�v���j
	 */
	bool m_Recv(SOCKET so)
	{
		int iLen = ::recv(so, m_lpcBuffer, E_BufferSize, 0);
		if ( iLen == SOCKET_ERROR )
		{
			int iError = ::WSAGetLastError();
			TRACE2( "::recv(%d) error = %d\n", so, iError );
			if ( iError != WSAENOTSOCK/*10038*/ && iError != WSAESHUTDOWN/*10058*/ )
			{
				m_OnSocketEvent(CSocketEvent(this, so, ReportEvent_Error, iError));
			}
			return false;	//�ؒf
		}
		else if ( iLen == 0 )
		{
			TTRACE0("CSocket:Closed Connection\n");
			return false;
		}
		else
		{
			CSocketEvent s(this, so);
			s.SetData(iLen, m_lpcBuffer);
			if ( ! m_OnSocketEvent(s) )
			{
				return false;	//�ؒf
			}
		}
		return true;
	}

	/**
	 * ���M�x�����[�h�ݒ�
	 *	@param so �\�P�b�g
	 *	@param boIsDelay true�Ȃ�x�����M�Bfalse�Ȃ瑦���M�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool ms_SetDelayMode(SOCKET so, bool boIsDelay)
	{
		BOOL boValue = ! boIsDelay;
		int iRc = ::setsockopt(
							so,
							IPPROTO_TCP,
							TCP_NODELAY,
							reinterpret_cast<const char*>(&boValue),
							sizeof(BOOL));
		#ifdef _DEBUG
			if ( iRc != 0) { TRACE2( "::setsockopt(%d) error = %d\n ", so, ::WSAGetLastError()); } 
		#endif
		{
			BOOL yes = TRUE;
			::setsockopt(so, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes));
		}
		return iRc == 0;
	}

	/**
	 * ���M
	 *	@param so �\�P�b�g
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool m_Send(SOCKET so, size_t dataSize, LPCVOID lpcvData)
	{
		if ( so == INVALID_SOCKET )
		{
			return false;
		}
		int iRc = ::send(so, static_cast<const char*>(lpcvData), ToInt(dataSize), 0);
		TRACE2("::send(%u) result = %d\n", dataSize, iRc);
		return iRc == ToInt(dataSize);
	}

	/**
	 * SOCKET�̐ڑ����擾
	 *	@param[out]	_addres	�A�h���X&�|�[�g
	 *	@param[in]	so		�\�P�b�g
	 *	@retval true �A�h���X�ƃ|�[�g�擾�����B
	 *	@retval false ���s�B
	 */
	virtual bool m_GetPeerName(CSocketAddress& _address, SOCKET so)
	{
		TSockAddr addr;
		int iLen = addr.GetSize();
		if ( ::getpeername(so, addr, &iLen) == SOCKET_ERROR )
		{
			TRACE2( "::getpeername(%d) error = %d\n", so, ::WSAGetLastError() );
			return false;
		}
		_address.Set(addr);
		return true;
	}

	/**
	 * �R�[���o�b�N�ʒm���s
	 *	@param e �ʒm������e
	 *	@retval true	����
	 *	@retval false	�ؒf�v��
	 */
	virtual bool m_OnSocketEvent(CSocketEvent& e)
	{
		bool r = true;
		if ( m_piSocketListner != NULL )
		{
			r &= m_piSocketListner->OnSocketEvent(e);
		}
		r &= _super::m_OnEvent(e);
		return r;
	}

	/**
	 * UDP���M�ő�T�C�Y�擾
	 *	@return �T�C�Y
	 */
	WORD m_GetMaxUdpDg(SOCKET s)
	{
		ASSERTLIB(s_iMaxUdpDg >= 0);
		#ifdef _TnbWINSOCK2_ENABLE
			if ( s_iMaxUdpDg == 0 )
			{
				int iOptVal;
				int iOptLen = sizeof(iOptVal);
				int r = ::getsockopt(s, SOL_SOCKET, SO_MAX_MSG_SIZE, reinterpret_cast<char*>(&iOptVal), &iOptLen);
				return static_cast<WORD>((r == SOCKET_ERROR) ? 0 : iOptVal);
			}
		#endif
		return static_cast<WORD>(s_iMaxUdpDg);
	}
	#endif // _TnbDOXYGEN
};



/// �R���X�g���N�^
inline CSocketEvent::CSocketEvent(CAbstractSocket* pcSocket, SOCKET so, EReportEvent eEvent, DWORD dwError)
	: _super(static_cast<IReport*>(pcSocket), eEvent, dwError)
{
	m_soData	= so;
	m_pcSocket	= pcSocket;
}

// �f�[�^���M
inline bool CSocketEvent::Send(size_t dataSize, LPCVOID lpcvData) const
{
	return m_pcSocket->Send(dataSize, lpcvData);
//	return m_pcSocket->m_Send(m_soData, dataSize, lpcvData);
}

//���擾
inline bool CSocketEvent::GetPeerName(CSocketAddress& _address) const
{
	return m_pcSocket->m_GetPeerName(_address, m_soData);
}

//���擾
inline bool CSocketEvent::GetSockName(CSocketAddress& _address) const
{
	CAbstractSocket::TSockAddr addr;
	int iLen = addr.GetSize();
	if ( ::getsockname(m_soData, addr, &iLen) == SOCKET_ERROR )
	{
		TRACE2( "::getpeername(%d) error = %d\n", m_soData, ::WSAGetLastError() );
		return false;
	}
	_address.Set(addr);
	return true;
}



/**@ingroup REPORT SOCKET
 * Socket�N���C�A���g�N���X
 *	
 *		�N���C�A���g�p�̃\�P�b�g��p�ӂ��܂��B
 *		����I��Server��T�������ڑ����[�h������܂��B
 *
 *		��M�� CAbstractSocket::IListener �̃T�u�N���X��o�^���s���܂��B
 *		( IReport::IListener �ł���M�͏o���܂�����{�@�\��������܂���)
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbSocket.h
 * 
 *	@date 06/04/01	�V�K�쐬
 *	@date 06/04/27	���\�b�h�ł̐ؒf�ł�Disconnect�C�x���g��������悤�ɂ����B
 *	@date 06/06/07	connect�̃^�C���A�E�g���Ԃ𒲐��B
 *	@date 06/06/22	ping�ɂ��ؒf�Ď���ǉ��B
 *	@date 06/07/31	�uStart���ɃT�[�o�A�h���X�𓾂Ă������߁A�����łȂ����Ă��Ȃ��Ɠ�x�ƂȂ���Ȃ��v���ۂ��C���B
 *	@date 06/08/31	CommViewer �ɑΉ��B
 *	@date 06/09/08	Idle�C�x���g�T�|�[�g�B IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *	@date 07/02/23	Connect���A���O�������ł��Ȃ��ƁA�������[�h�Ȃ̂ɂȂ���Ȃ��Ȃ�̂��C���B
 *	@date 07/06/11	�e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 09/02/27	���g���C�҂����������P�B
 *	@date 09/08/27	SetReceiveThreadPriority() �ǉ��B�\���ύX�B
 *	@date 09/08/27	�A�h���X�w��� CSocketAddress ���g���悤�ɕύX�B
 *	@date 09/09/10	SetReceiveThreadPriority() �p�~�A GetReceiveThreadStatusGet() �ǉ��B
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 *	@date 10/08/05	ITester �p�~�B
 *	@date 11/06/14	GetReportID() ���\�b�h�̖߂�l�ύX�B
 */
class CSocketClient : public CAbstractSocket, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractSocket);
public:

	/**
	 * �R���X�g���N�^
	 *	@param isAutoReconnect �����ڑ����[�h�B
	 *							false �Ȃ�ؒf������A�ؒf�̂܂܂ɂȂ�܂��B
	 *							�ȗ��� true �Ȃ�ؒf���Ă������I�Ƀ��g���C���܂��B
	 */
	explicit CSocketClient(bool isAutoReconnect = true) 
		: _super(), m_soData(INVALID_SOCKET), m_isAutoReconnect(isAutoReconnect), m_wServerPortNo(0)
	{
		m_threadReceiver.SetRunner(this);
	}

	/// �f�X�g���N�^
	virtual ~CSocketClient(void)
	{
		m_CloseDataSocket();
		m_threadReceiver.Stop();
	}

	/**
	 * [�ݒ�] ���M�x�����[�h�ݒ�
	 *	@note �ڑ�����O�ɐݒ肷��K�v������܂��B
	 *	@note �f�t�H���g�ł͒x�����M���[�h�ł��B
	 *	@param boIsDelay true�Ȃ�x�����M�Bfalse�Ȃ瑦���M�B
	 */
	void SetDelayMode(bool boIsDelay)
	{
		m_boIsDelay = boIsDelay;
	}

	/**
	 * [�擾] �R�~���j�P�[�V������.
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const 
	{
		return CStr::Fmt(_T("SocketClient[%u]:"), m_soData);
	}

	/**
	 * [�擾] �R�~���j�P�[�V����ID.
	 *		TNBVIW_KIND_??? + PORT ��Ԃ��܂��B
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const 
	{
		return TNBVIW_KIND_SOCK_C + (m_soData & 0xFFFF);
	}

	/**
	 * [�擾] �\�P�b�g�擾
	 *	@retval INVALID_SOCKET		�ڑ����Ă��Ȃ�
	 *	@retval INVALID_SOCKET�ȊO	�\�P�b�g
	 */
	SOCKET GetSocket(void) const
	{
		return m_soData;
	}

	/**
	 * [�ݒ�] �T�[�o�ݒ�
	 *	@param address	IP�A�h���X���|�[�gNo
	 */
	void SetServer(const CSocketAddress& address)
	{
		StartUp();
		Stop();
		m_strServerAddress = address.GetAddressString(false);
		m_wServerPortNo = address.GetPortNo();
	}

	/**
	 * [���s] �J�n
	 *	@note ��� SetServer() �ŃT�[�o�̐ݒ���s���K�v������܂��B
	 *	@retval true	�����B�����������ڑ����[�h�̏ꍇ�A����R�l�N�g���Ă��Ȃ��\���L��B
	 *	@retval false	�R�l�N�g���s�B�����ڑ����[�h�̏ꍇ�A�X���b�h�̋N���ɂ����s���Ă���ꍇ�B
	 */
	virtual bool Start(void)
	{
		StartUp();
		Stop();
		if ( ! m_SetAddress(m_strServerAddress, m_wServerPortNo) )
		{
			if ( ! m_isAutoReconnect )
			{
				return false;
			}
		}
		if ( ! m_isAutoReconnect )
		{
			if ( ! m_ConnectSub() )
			{
				return false;
			}
		}
		#ifdef _DEBUG
			m_threadReceiver.Start(CStr::Fmt(_T("Client(p=%d)�Ď�"), m_wServerPortNo));
		#else
			m_threadReceiver.Start();
		#endif
		return true;
	}

	/**
	 * [���s] �J�n
	 *	@note SetServer() �� Start() �����s���Ă��܂��B
	 *	@param address	IP�A�h���X���|�[�gNO
	 *	@retval true	�����B�����������ڑ����[�h�̏ꍇ�A����R�l�N�g���Ă��Ȃ��\���L��B
	 *	@retval false	�R�l�N�g���s�B�����ڑ����[�h�̏ꍇ�A�X���b�h�̋N���ɂ����s���Ă���ꍇ�B
	 */
	bool Connect(const CSocketAddress& address)
	{
		SetServer(address);
		return Start();
	}

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		Connect() ���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const
	{
		return m_threadReceiver.IsAlive(); 
	}
	
	/**
	 * [�m�F] �ڑ��m�F
	 *	@retval true	�ڑ���
	 *	@retval false	�ؒf��
	 */
	virtual bool IsConnect(void) const
	{
		return m_soData != INVALID_SOCKET; 
	}

	/**
	 * [���s] ���M
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		TTRACE1("CSocket[Client%d]:Send\n", m_soData);
		_super::m_OnSend(dataSize, lpcvData);
		return m_Send(m_soData, dataSize, lpcvData);
	}

	/**
	 * [���s] �ؒf
	 */
	virtual void Stop(void)
	{
		if ( IsConnect() )
		{
			m_CloseDataSocket();
		}
		m_threadReceiver.Stop();
	}

	/**
	 * [���s] �ؒf
	 */
	void Disconnect(void)
	{
		Stop(); 
	}

	/**
	 * [�ݒ�] ��M�X���b�h��Ԏ�擾.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadReceiver;
	}

#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// [���s] �J�n. ���^�C�v�̎w����@�ł��B �p�~�\��ł��B
	bool Connect(LPCTSTR lpszAddress, WORD wPortNo)
	{
		SetServer(CSocketAddress(wPortNo, lpszAddress));
		return Start();
	}
#endif

private:
	TNB::CThread	m_threadReceiver;	///< ��M�X���b�h
	bool			m_isAutoReconnect;	///< �����ڑ����[�h
	SOCKET			m_soData;			///< �f�[�^�p�\�P�b�g
	CStr			m_strServerAddress;	///< �T�[�o�A�h���X�T��
	WORD			m_wServerPortNo;	///< �T�[�o�|�[�g�T��

	/**
	 * �^�C���A�E�g�t���R�l�N�g
	 *	@param so �\�P�b�g�B
	 *	@param dwTimeout �^�C���A�E�g(ms)
	 *	@retval true	�T�[�o�ɐڑ�����
	 *	@retval false	�T�[�o�����炸
 	 */
	bool m_ConnectWithTimeout(SOCKET so, DWORD dwTimeout)
	{
		DWORD dwOpt = 1;	//��u���b�L���O�ɐݒ�
		::ioctlsocket(so, FIONBIO, &dwOpt);
		int iCntRst = ::connect(so, m_sockaddrPartner, m_sockaddrPartner.GetSize());
		if ( iCntRst == SOCKET_ERROR ) 
		{
			DWORD dwError = ::WSAGetLastError();
			if ( dwError != WSAEWOULDBLOCK )
			{
				TRACE2("::connect(%d) error = %d\n", so, dwError);
				dwOpt = 0;	//�u���b�L���O�ɐݒ�
				::ioctlsocket(so, FIONBIO, &dwOpt);
				return false;
			}
			fd_set tFdSet;
			FD_ZERO(&tFdSet);
			FD_SET(so, &tFdSet);
			timeval tTimeVal = { dwTimeout / 1000, dwTimeout % 1000 };
			::select(FD_SETSIZE, NULL, &tFdSet, NULL, &tTimeVal);
			if ( ! FD_ISSET(so, &tFdSet) )
			{
				dwOpt = 0;	//�u���b�L���O�ɐݒ�
				::ioctlsocket(so, FIONBIO, &dwOpt);
				return false;
			}		
		}
		dwOpt = 0;	//�u���b�L���O�ɐݒ�
		::ioctlsocket(so, FIONBIO, &dwOpt);
		return true;
	}

	/**
	 * �R�l�N�g����
	 *	@note SOCKET������Connect�����݂܂��B
	 *		���������m_soData�ɗL���ȃ\�P�b�g������܂��B
	 *	@retval true	�T�[�o�ɐڑ�����
	 *	@retval false	�T�[�o�����炸
 	 */
	bool m_ConnectSub(void)
	{
		if ( m_soData != INVALID_SOCKET )
		{
			return true;
		}
		SOCKET so = ::socket(AF_INET, SOCK_STREAM, 0);
		if ( so == SOCKET_ERROR ) 
		{
			TRACE1( "::socket() error = %d\n", ::WSAGetLastError() );
			return false;
		}
		if ( ! ms_AddressToIp(m_sockaddrPartner.sin_addr, m_strServerAddress) )
		{
			::shutdown(so, SD_BOTH);
			::closesocket(so);
			return false;
		}
		if ( ! m_ConnectWithTimeout(so, E_ConnectTimeout) )
		{
			::shutdown(so, SD_BOTH);
			::closesocket(so);
			return false;
		}
		TTRACE1("CSocket[Client%d]:Connected\n", so);
		m_soData = so;
		if ( ! m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Connect)) )
		{
			m_CloseDataSocket();
			return false;
		}
		ms_SetDelayMode(m_soData, m_boIsDelay);
		return true;
	}

	/// �\�P�b�g�N���[�Y
	void m_CloseDataSocket(void)
	{
		if ( m_soData != INVALID_SOCKET )
		{
			SOCKET so = m_soData;
			m_soData = INVALID_SOCKET;
			TTRACE1("CSocket[Client%d]:Closed\n", so);
			m_OnSocketEvent(CSocketEvent(this, so, ReportEvent_Disconnect));
			::shutdown(so, SD_BOTH);
			::closesocket(so);
		}
	}

	/** 
	 * �X���b�h�����{��
	 *	@retval �O�Œ�B
	 */
	virtual DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			if ( ! m_ConnectSub() )
			{
				loop ( i, 20 )
				{
					if ( ! IsRunnable() )
					{
						break;
					}
					::Sleep(E_RetryDelay / 20);
				}
				continue;
			}
			//
			CTickCount tickIdle;
			CTickCount tickConCheck;
			while ( IsRunnable() )
			{
				fd_set tFdSet;
				FD_ZERO(&tFdSet);
				FD_SET(m_soData, &tFdSet);
				int iSelTime = E_SelectTimeout;
				int	iInterval = _super::GetIdleInterval();
				if ( iInterval > 0 )
				{
					iSelTime = iInterval;
				}
				timeval tTimeVal = { iSelTime / 1000, iSelTime % 1000 };
				::select(ToInt(m_soData + 1), &tFdSet, NULL, NULL, &tTimeVal);
				if ( ! FD_ISSET(m_soData, &tFdSet) )
				{
					if ( tickConCheck.IsPassedAndReset(E_SelectTimeout) )
					{
						CSocketAddress adds;
						if ( ! m_GetPeerName(adds, m_soData) )
						{
							break;	//�ؒf
						}
					}
					if ( iInterval > 0 && tickIdle.IsPassedAndReset(iInterval) )
					{
						if ( ! m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Idle)) )
						{
							break;	//�ؒf
						}
					}
					continue;
				}
				if ( ! m_Recv(m_soData) )
				{
					break;	//�ؒf
				}
				TTRACE1("CSocket[Client%d]:��M����\n", m_soData);
				tickIdle.Reset();
			}
			//
			TTRACE1("CSocket[Client%d]:��M��~\n", m_soData);
			m_CloseDataSocket();
			if ( ! m_isAutoReconnect )
			{
				break;
			}
			loop ( i, 20 )
			{
				if ( ! IsRunnable() )
				{
					break;
				}
				::Sleep(E_RetryDelay / 20);
			}
		}
		m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_End));
		return 0;
	}
};



/**@ingroup REPORT SOCKET
 * Socket�T�[�o�N���X
 *	
 *		�T�[�o�p�Ƀ\�P�b�g��p�ӂ��܂��B
 *
 *		��M�� CAbstractSocket::IListener �̃T�u�N���X��o�^���s���܂��B
 *		( IReport::IListener �ł���M�͏o���܂�����{�@�\��������܂���)
 *
 *	@note IReport::ITester ��o�^���Ă��A��M�e�X�g�͏o���܂���
 *			( OnCommTestReceive() �̓R�[������܂���j�B
 *
 *	@note	�A�C�h���C�x���g�́A�ڑ�����Ă���S�N���C�A���g����̎�M���Ȃ��Ȃ��� ��ms�o�������ɁA
 *			�S�ڑ��ɑ΂��āA�A�C�h���C�x���g���ʒm����܂��B�e�ڑ����ł͂Ȃ����Ƃɂ����ӂ��������B
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbSocket.h
 * 
 *	@date 06/04/01	�V�K�쐬
 *	@date 06/04/27	�ڑ��\���������B���\�b�h�ł̐ؒf�ł�
 *					Disconnect�C�x���g��������悤�ɂ����B
 *	@date 06/06/22	ping�ɂ��ؒf�Ď���ǉ��B
 *	@date 06/08/31	CommViewer �ɑΉ��B
 *	@date 06/09/08	Idle�C�x���g�T�|�[�g�B IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *	@date 07/06/11	�e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 09/01/15	��x�K��Listen�G���[����������̂��C���B
 *	@date 09/02/27	���g���C�҂����������P�B
 *	@date 09/08/27	SetReceiveThreadPriority() �ǉ��B�\���ύX�B
 *	@date 09/08/27	�A�h���X�w��� CSocketAddress ���g���悤�ɕύX�B
 *	@date 09/09/10	SetReceiveThreadPriority() �p�~�A GetReceiveThreadStatusGet() �ǉ��B
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 *	@date 11/06/14	GetReportID() ���\�b�h�̖߂�l�ύX�B
 */
class CSocketServer : public CAbstractSocket, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractSocket);
public:

	/// �R���X�g���N�^
	CSocketServer(void) : _super(), m_soListen(INVALID_SOCKET), m_connectMax(1), m_wOpenPortNo(0)
	{
		m_threadReceiver.SetRunner(this);
	}

	/// �f�X�g���N�^
	virtual ~CSocketServer(void)
	{
		Close(); 
	}

	/**
	 * [�ݒ�] ���M�x�����[�h�ݒ�
	 *	@note �ڑ�����O�ɐݒ肷��K�v������܂��B
	 *	@note �f�t�H���g�ł͒x�����M���[�h�ł��B
	 *	@param boIsDelay true�Ȃ�x�����M�Bfalse�Ȃ瑦���M�B
	 */
	void SetDelayMode(bool boIsDelay)
	{
		m_boIsDelay = boIsDelay;
	}

	/**
	 * [�擾] �R�~���j�P�[�V������.
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const 
	{
		return CStr::Fmt(_T("SocketServer[%08X]:"), this);
	}

	/**
	 * [�擾] �R�~���j�P�[�V����ID.
	 *		TNBVIW_KIND_??? + PORT ��Ԃ��܂��B
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const 
	{
		return TNBVIW_KIND_SOCK_S + (reinterpret_cast<DWORD>(this) & 0xFFFF);
	}

	/**
	 * [�ݒ�] �ڑ��\��.
	 *		��̃T�[�o�ɐڑ��ł��鐔�𐧌����܂��B
	 *		�f�t�H���g�͈�ł��B
	 *	@note	�T�[�o���I�[�v�����Ă�������̕ύX���\�ł��B
	 *			�������A���݂̐ڑ�����菬���������Ƃ��A�ؒf�͂���܂���B
	 *	@param max �ڑ��\��
	 */
	void SetConnectMax(size_t max)
	{
		if ( max >= 1 )
		{
			m_connectMax = max; 
		}
	}

	/**
	 * [�ݒ�] �T�[�o�ݒ�
	 *	@param address �|�[�gNO���҂��󂯂�A�h���X�B
	 */
	void SetParameter(const CSocketAddress& address)
	{
		StartUp();
		Stop();
		m_wOpenPortNo = address.GetPortNo();
		m_strAwaitAddress = address.GetAddressString(false);
	}

	/**
	 * [���s] �J�n.
	 *		�T�[�o���I�[�v�����ʐM���J�n���܂��B
	 *	@note	SetParameter() �Őݒ��A�{���\�b�h���g�p���܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Start(void)
	{
		StartUp();
		Stop();
		if ( GetConnectedSockets() != 0 )
		{
			return false;
		}
		if ( m_strAwaitAddress.IsEmpty() )
		{
			m_strAwaitAddress = _T("0.0.0.0");
		}
		if ( ! m_SetAddress(m_strAwaitAddress, m_wOpenPortNo) )
		{
			return false;
		}
		if ( m_OpenListenSocket() == 10048 )
		{
			return false;
		}
		#ifdef _DEBUG
			m_threadReceiver.Start(CStr::Fmt(_T("Server(p=%d)�Ď�"), m_wOpenPortNo));
		#else
			m_threadReceiver.Start();
		#endif
		return true;
	}

	/**
	 * [���s] �I�[�v��.
	 *		�T�[�o���I�[�v�����ʐM���J�n���܂��B
	 *	@param address �|�[�gNO���҂��󂯂�A�h���X�B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool Open(const CSocketAddress& address)
	{
		SetParameter(address);
		return Start();
	}

	/**
	 * [�擾] �ڑ��\�P�b�g�ꗗ�Ɛ����擾
	 *	@param[out]	P	�\�P�b�g�ꗗ�BNULL�Ȃ�i�[���܂���B
	 *	@return �ڑ��\�P�b�g��
	 */
	INT_PTR GetConnectedSockets(CSocketVector* P = NULL) const
	{
		if ( P != NULL )
		{
			EXCLUSIVE(&m_syncSocket);
			*P = m_vsoData;
		}
		return m_vsoData.GetSize();
	}

	/**
	 * [�m�F] �w��\�P�b�g�̐ڑ��m�F
	 *	@param so �\�P�b�g
	 *	@retval true	�ڑ���
	 *	@retval false	�ؒf��
	 */
	bool IsConnect(SOCKET so) const
	{
		EXCLUSIVE(&m_syncSocket);
		loop ( i, m_vsoData.GetSize() )
		{
			if ( m_vsoData[i] == so ){ return true; }
		}
		return false;
	}

	/**
	 * [���s] �f�[�^���M
	 *	@param so �\�P�b�g
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool Send(SOCKET so, size_t dataSize, LPCVOID lpcvData)
	{
		TTRACE1("CSocket[Server%d]:Send\n", so);
		_super::m_OnSend(dataSize, lpcvData);
		return m_Send(so, dataSize, lpcvData);
	}

	/**
	 * [�擾] �w��\�P�b�g�̐ڑ����擾
	 *	@param[out]	_address �A�h���X&�|�[�g
	 *	@param[in]	so �\�P�b�g
	 *	@retval true	�A�h���X�ƃ|�[�g�擾�����B
	 *	@retval false	���s�B
	 */
	bool GetPeerName(CSocketAddress& _address, SOCKET so)
	{
		return m_GetPeerName(_address, so);
	}

	/**
	 * [���s] �w��\�P�b�g�̐ؒf
	 *	@param so S�\�P�b�g
	 */
	void Disconnect(SOCKET so)
	{
		m_CloseDataSocket(so);
	}

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		Open() ���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const
	{
		return m_threadReceiver.IsAlive();
	}

	/**
	 * [�m�F] �ڑ��`�F�b�N
	 *	@retval true	��ȏ�ڑ����B
	 *	@retval	false	����ڑ����Ă��Ȃ��B
	 */
	virtual bool IsConnect(void) const
	{
		return GetConnectedSockets() != 0; 
	}

	/**
	 * [���s] �S���ɑ��M.
	 *		�ڑ����Ă��邷�ׂĂɑ��M���܂��B
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		TTRACE0("CSocket[Server%d]:Send<All>\n");
		bool boRc = true;
		EXCLUSIVE(&m_syncSocket);
		loop ( i, m_vsoData.GetSize() )
		{
			boRc &= Send(m_vsoData[i], dataSize, lpcvData);
		}
		return boRc;
	}

	/**
	 * [���s] �S�\�P�b�g�̐ؒf.
	 *		�ڑ����Ă��邷�ׂĂ���x�ؒf���܂��B�����ɐڑ��҂����܂��̂ŁA
	 *		�����ɐڑ������\�����L��܂��B
	 */
	void Disconnect(void)
	{
		m_CloseAllDataSockets();
		m_OpenListenSocket();
	}

	/**
	 * [���s] �T�[�o���N���[�Y
	 */
	virtual void Stop(void)
	{
		Close();
	}

	/**
	 * [���s] �T�[�o���N���[�Y
	 */
	void Close(void)
	{
		TTRACE0("CSocket[Server]:�T�[�o�N���[�Y\n");
		m_CloseAllDataSockets();
		m_CloseListenSocket();
		m_threadReceiver.Stop();
	}

	/**
	 * [�Q��] ��M�X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadReceiver;
	}

private:
	TNB::CThread	m_threadReceiver;	///< ��M�X���b�h
	SOCKET			m_soListen;			///< Listen�p�\�P�b�g
	CSocketVector	m_vsoData;			///< �ڑ����Ă���\�P�b�g�̔z��
	CSyncSection	m_syncSocket;		///< �����I�u�W�F�N�g
	size_t			m_connectMax;		///< �ڑ��\�̐�
	WORD			m_wOpenPortNo;		///< �I�[�v���|�[�gNo
	CStr			m_strAwaitAddress;	///< �ڑ����A�h���X
	
	/**
	 * �S�f�[�^SOCKET�N���[�Y
	 *	@note �ڑ����Ă���f�[�^SOCKET�����ׂăN���[�Y���܂�
	 */
	void m_CloseAllDataSockets(void)
	{
		EXCLUSIVE(&m_syncSocket);
		size_t iLen = m_vsoData.GetSize();
		if ( iLen > 0 )
		{
			TTRACE0("CSocket[Server]:�S�N���[�Y\n");
			loop ( i, iLen )
			{
				if ( m_vsoData[i] != INVALID_SOCKET )
				{
					SOCKET so = m_vsoData[i];
					TTRACE1("CSocket[Server%d]:�N���[�Y\n", so);
					m_OnSocketEvent(CSocketEvent(this, so, ReportEvent_Disconnect));
					::shutdown(so, SD_BOTH);
					::closesocket(so);
				}
			}
			m_vsoData.RemoveAll();
		}
	}

	/**
	 * �w��f�[�^�\�P�b�g�N���[�Y
	 *	@param so �\�P�b�g
	 */
	void m_CloseDataSocket(SOCKET so)
	{
		EXCLUSIVE(&m_syncSocket);
		loop ( i, m_vsoData.GetSize() )
		{
			if ( m_vsoData[i] == so )
			{
				TTRACE1("CSocket[Server%d]:�N���[�Y\n", so);
				m_vsoData.Remove(i);
				m_OnSocketEvent(CSocketEvent(this, so, ReportEvent_Disconnect));
				::shutdown(so, SD_BOTH);
				::closesocket(so);
				m_OpenListenSocket();
				break;
			}
		}
	}

	/**
	 * ���X�i�[�\�P�b�g�I�[�v��
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B���l�̓G���[�R�[�h
	 */
	int m_OpenListenSocket(void)
	{
		if ( m_soListen == INVALID_SOCKET )
		{
			m_soListen = ::socket(AF_INET, SOCK_STREAM, 0); // TCP socket
			if ( m_soListen == INVALID_SOCKET )
			{
				int r = ::WSAGetLastError();
				ASSERT( r != 0 );
				TRACE1( "::socket() error = %d\n", r );
				return r;
			}
			if ( ::bind(m_soListen, m_sockaddrPartner, m_sockaddrPartner.GetSize()) == SOCKET_ERROR )
			{
				int r = ::WSAGetLastError();
				ASSERT( r != 0 );
				TRACE2( "::bind(%d) error = %d\n", m_soListen, r );
				return r;
			}
			TTRACE0("CSocket[Server]:���X�i�[SOCKET�I�[�v��\n");
			if ( ::listen(m_soListen, 5) == SOCKET_ERROR ) 
			{
				int r = ::WSAGetLastError();
				ASSERT( r != 0 );
				TRACE2( "::listen(%d) error = %d\n", m_soListen, r );
				m_CloseListenSocket();
				return r;
			}
		}
		return 0;
	}

	/// ���X�i�[�\�P�b�g�N���[�Y
	void m_CloseListenSocket(void)
	{
		if ( m_soListen != INVALID_SOCKET )
		{
			SOCKET so = m_soListen;
			m_soListen = INVALID_SOCKET;
			TTRACE0("CSocket[Server]:���X�i�[SOCKET�N���[�Y\n");
			::shutdown(so, SD_BOTH);
			::closesocket(so);
		}
	}

	/** 
	 * �X���b�h�����{��
	 *	@retval �O�Œ�B
	 */
	virtual DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			if ( m_OpenListenSocket() != 0 )
			{
				loop ( i, 20 )
				{
					if ( ! IsRunnable() )
					{
						break;
					}
					::Sleep(E_RetryDelay / 20);
				}
				continue;
			}
			//
			CTickCount tickIdle;
			CTickCount tickConCheck;
			while ( IsRunnable() )
			{
				fd_set tFdSet;
				FD_ZERO(&tFdSet);
				CSocketVector vsoBack;
				INT_PTR iConnectedCount = GetConnectedSockets(&vsoBack);
				if ( iConnectedCount > 0 )
				{
					loop ( i, iConnectedCount )
					{
						FD_SET(vsoBack[i], &tFdSet);
					}
				}
				if ( m_soListen != INVALID_SOCKET )
				{
					FD_SET(m_soListen, &tFdSet);
				}
				int iSelTime = E_SelectTimeout;
				int	iInterval = _super::GetIdleInterval();
				if ( iInterval > 0 )
				{
					iSelTime = iInterval;
				}
				timeval tTimeVal = { iSelTime / 1000, iSelTime % 1000 };
				if ( ::select(FD_SETSIZE, &tFdSet, NULL, NULL, &tTimeVal) == SOCKET_ERROR )
				{
					DWORD dwError = ::WSAGetLastError();
					TRACE1( "::select() error = %d\n", dwError );
					if ( dwError == WSAEINVAL )
					{
						::Sleep(E_SelectTimeout);
					}
					continue;
				}
				if ( FD_ISSET(m_soListen, &tFdSet) )
				{
					FD_CLR(m_soListen, &tFdSet);
					TSockAddr addrFrom;
					int iFromlen = addrFrom.GetSize();
					SOCKET so = ::accept(m_soListen, addrFrom, &iFromlen);
					if ( so == INVALID_SOCKET )
					{
						TRACE2( "::accept(%d) error = %d\n", m_soListen, ::WSAGetLastError() );
						m_CloseListenSocket();
					}
					else if ( m_vsoData.GetSize() >= m_connectMax )
					{
						TTRACE1( "CSocket[Server%d]:�ڑ��˗��A����\n", so );
						m_CloseListenSocket();
						::shutdown(so, SD_BOTH);
						::closesocket(so);
					}
					else
					{
						TTRACE1("CSocket[Server%d]:�ڑ�\n", so);
						ms_SetDelayMode(so, m_boIsDelay);
						{
							EXCLUSIVE(&m_syncSocket);
							m_vsoData.AddEx(so);
							if ( m_vsoData.GetSize() >= m_connectMax )
							{
								m_CloseListenSocket();
							}
						}
						if ( ! m_OnSocketEvent(CSocketEvent(this, so, ReportEvent_Connect)) )
						{
							TTRACE1("CSocket[Server%d]:�ؒf\n", so);
							m_CloseDataSocket(so);
							m_OpenListenSocket();
						}
					}
				}
				if ( iConnectedCount == 0 )
				{
					continue;
				}
				if ( tFdSet.fd_count == 0 )
				{
					if ( tickConCheck.IsPassedAndReset(E_SelectTimeout) )
					{
						CSocketAddress adds;
						loop ( i, iConnectedCount )
						{
							if ( ! m_GetPeerName(adds, vsoBack[i]) )
							{
								m_CloseDataSocket(vsoBack[i]);
								m_OpenListenSocket();
							}
						}
					}
					if ( iInterval > 0 && tickIdle.IsPassedAndReset(iInterval) )
					{
						loop ( i, iConnectedCount )
						{
							if ( ! m_OnSocketEvent(CSocketEvent(this, vsoBack[i], ReportEvent_Idle)) )
							{
								TTRACE1("CSocket[Server%d]:�ؒf\n", vsoBack[i]);
								m_CloseDataSocket(vsoBack[i]);
								m_OpenListenSocket();
							}
						}
					}
					continue;
				}
				//
				loop ( i, iConnectedCount )
				{
					if ( ! FD_ISSET(vsoBack[i], &tFdSet) )
					{
						continue;
					}
					if ( ! m_Recv(vsoBack[i]) )
					{
						TTRACE1("CSocket[Server%d]:�ؒf(Recv�ɂ��)\n", vsoBack[i]);
						m_CloseDataSocket(vsoBack[i]);
						m_OpenListenSocket();
					}
					else
					{
						TTRACE1("CSocket[Server%d]:�f�[�^��M\n", vsoBack[i]);
					}
				}
				tickIdle.Reset();
			}
		}
		m_OnSocketEvent(CSocketEvent(this, 0, ReportEvent_End));
		return 0;
	};
};



}; // TNB
