#pragma once
/**
 *@file
 * �\�P�b�g(UDP�֌W) �̃A�N�Z�X�֌W�̃w�b�_
 *
 *		UDP �̃N���X������܂��B
 *		��M�����́A���X�i�[��o�^���邱�Ƃōs���܂��B
 *
 *	@note Winsock ���g�p���Ă��܂��B
 *
 *	@note	�}�N�� "_TnbWINSOCK2_ENABLE" ����`����Ă���ƁA Winsock2.0 ���g�p���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSocket.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT SOCKET
 * SocketUPD�N���X
 *	
 *		UDP�\�P�b�g��p�ӂ��܂��B
 *
 *		��M�� CAbstractSocket::IListener �� IReport::IListener �̎����N���X���쐬�� SetSocketListener() , SetListener() ���g���o�^���܂��B
 *		�����o�^���Ă���ꍇ�A��M���A��� CAbstractSocket::IListener �ɒʒm����܂��B
 *
 *	@note ���M�T�C�Y���ƂɎ�M����܂�( TCP �ƈقȂ�A�A������邱�Ƃ͂���܂��� )�B
 *
 *	@note ReportEvent_Connect �C�x���g�́AStart ���A ReportEvent_Disconnect �C�x���g�́AStop���O�ɒʒm����܂��B
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbSocketUdp.h
 * 
 *	@date 06/07/06	�V�K�쐬
 *	@date 06/08/31	CommViewer �ɑΉ��B
 *	@date 06/09/09	IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *					ECE_Connect , ECE_Disconnect �C�x���g�A�ǉ��B
 *	@date 06/10/24	Magicpacket�Ή��B
 *	@date 07/06/11	�e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 09/07/31	ECE_Idle �C�x���g�ɑΉ��B
 *	@date 09/08/27	SetReceiveThreadPriority() �ǉ��B�\���ύX�BIsAlive() �̖߂�l�C���B
 *					GetIdleInterval() ��0�̎��A select ������ recv ����悤�ɂ����i���x���҂����߁j
 *	@date 09/09/04	GetReceivedBufferSize(), SetReceivedBufferSize() �ǉ��B
 *	@date 09/09/10	SetReceiveThreadPriority() �p�~�A GetReceiveThreadStatusGet() �ǉ��B
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 *	@date 10/08/05	ITester �p�~�B
 *	@date 11/06/14	GetReportID() ���\�b�h�̖߂�l�ύX�B
 *	@date 12/03/30	�啝���C�B
 */
class CSocketUdp : public CAbstractSocket, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractSocket);
public:

	/// �R���X�g���N�^
	CSocketUdp(void) : _super(), m_isStopping(false), m_soData(INVALID_SOCKET), m_isBroadcast(-1), m_bufferSize(0), m_isAddressBroadcast(false)
	{
		m_threadReceiver.SetRunner(this);
		m_addrFrom.Init();
	}

	/// �f�X�g���N�^
	virtual ~CSocketUdp(void)
	{
		Stop();
	}

	/**
	 * [�擾] �R�~���j�P�[�V������.
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ���O
	 */
	virtual CStr GetReportName(void) const 
	{
		return CStr::Fmt(_T("SocketUDP[%u]:"), m_baseAddress.GetPortNo());
	}

	/**
	 * [�擾] �R�~���j�P�[�V����ID.
	 *		TNBVIW_KIND_??? + PORT ��Ԃ��܂��B
	 *	@note �J���p�̃��\�b�h�ł��B
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const 
	{
		return TNBVIW_KIND_SOCK_U + m_baseAddress.GetPortNo();
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
	 * [�擾] UDP���M�ő�T�C�Y�擾.
	 *	@attention �{���\�b�h�̒l�Ɋւ�炸�A�l�b�g���[�N�� MTU (Maximum Transmission Unit) ��
	 *				������u���[�h�L���X�g �f�[�^�O�����̑��M�͂��E�߂ł��܂���B
	 *	@return �T�C�Y
	 */
	WORD GetMaxDatagram(void)
	{
		WORD w = m_GetMaxUdpDg(m_soData);
		if ( w == 0 )
		{
			w = 1500;
		}
		return w;
	}

	/**
	 * [�ݒ�] �x�[�X�A�h���X�ݒ�.
	 *		��M�Ɏg�p���� IP �A�h���X(INADDR_ANY �ł���)�A�|�[�gNo ���w�肵�܂��B
	 *		�|�[�gNo �� UDP �w�b�_�̑��M���|�[�gNo�ɂ��g���܂��B
	 *	@attention Start() �O�Ɏg�p���Ă��������B
	 *	@note �|�[�gNo �ɂO���w�肷��Ƒ��M��p�ɂȂ�܂��i��M Event �͔������܂���)�B
	 *	@param address	�A�h���X
	 */
	void SetBaseAddress(const CSocketAddress& address)
	{
		StartUp();
		Stop();
		m_baseAddress = address;
	}

	/**
	 * [�擾] �x�[�X�A�h���X�擾.
	 *	@return ��M�Ɏg�p����A�h���X
	 */
	const CSocketAddress& GetBaseAddress(void) const
	{
		return m_baseAddress;
	}

	/**
	 * [���s] �J�n
	 *	@note	SetBaseAddress() �Őݒ��A�{���\�b�h���g�p���܂��B
	 *	@retval true	�����B����M�\�B
	 *	@retval false	���s�B
	 */
	virtual bool Start(void)
	{
		StartUp();
		Stop();
		// �����p�\�P�b�g�\���̂̏�����
		TSockAddr addr;
		addr.Init();
		addr.sin_family = AF_INET;			// TCP/IP���w��
		m_baseAddress.Get(addr);
		// UDP �I�[�v��
		SOCKET so = ::socket(AF_INET, SOCK_DGRAM, 0);
		if ( so == SOCKET_ERROR ) 
		{
			TRACE1( "::socket() error = %d\n", ::WSAGetLastError() );
			return false;
		}
		if ( ::bind(so, addr, addr.GetSize()) == SOCKET_ERROR )
		{
			TRACE1( "::bind() error = %d\n", ::WSAGetLastError() );
			::closesocket(so);
			return false;
		}
		m_soData = so;
		m_isBroadcast = -1;
		// ��M�X���b�h�J�n
		if ( m_baseAddress.GetPortNo() != 0 )
		{
			m_bufferSize = GetMaxDatagram();
			ASSERTLIB(m_bufferSize > 0);		
			delete m_lpcBuffer;
			m_lpcBuffer = new char[m_bufferSize];
			ASSERTLIB(m_lpcBuffer != NULL);		
			#ifdef _DEBUG
				m_threadReceiver.Start(CStr::Fmt(_T("SocketUDP[%u]�Ď�"), m_baseAddress.GetPortNo()));
			#else
				m_threadReceiver.Start();
			#endif
		}
		return true;
	}

	/**
	 * [�m�F] Alive�`�F�b�N.
	 *		Start() ���\�b�h�����s����Ă����Ԃ��A�`�F�b�N���܂��B
	 *	@retval true	�J�n���Ă���B
	 *	@retval	false	��~���Ă���B
	 */
	virtual bool IsAlive(void) const 
	{
		return m_soData != INVALID_SOCKET; 
	}
	
	/**
	 * [�m�F] �ڑ��`�F�b�N.
	 *	@note �{�N���X�̏ꍇ�AStart() ���������Ă�����ڑ��������ɂ��Ă��܂��B
	 *	@retval true	�ڑ����B
	 *	@retval	false	�ؒf�B
	 */
	virtual bool IsConnect(void) const
	{
		return m_soData != INVALID_SOCKET; 
	}

	/**
	 * [�擾] ���M��A�h���X�擾.
	 *		SetAddressee() �� SetBroadcastPort() �Őݒ肵���A�h���X�������܂��B
	 *	@return �A�h���X
	 */
	const CSocketAddress& GetAddressee(void) const
	{
		return m_addressee;
	}

	/**
	 * [�ݒ�] ���M��A�h���X�w��
	 *	@note Start() �̌�A Send() �O�ɖ{���\�b�h�ő��M���ݒ肷��K�v������܂��B
	 *	@param address	�����IP�A�h���X&�|�[�gNo
	 *	@retval true	�����B
	 *	@retval	false	���s(���O�������ł��Ȃ�)
	 */
	bool SetAddressee(const CSocketAddress& address)
	{
		m_addressee = address;
		m_isAddressBroadcast = false;
		return m_SetAddress(address);
	}

	/**
	 * [�ݒ�] �u���[�h�o���h���M�w��
	 *	@note Start() �̌�A Send() �O�ɖ{���\�b�h�ő��M���ݒ肷��K�v������܂��B
	 *	@param portNo	���M��|�[�gNo
	 *	@param address �A�h���X�B�ȗ������ INADDR_BROADCAST
	 */
	void SetBroadcastPort(WORD portNo, DWORD address = INADDR_BROADCAST)
	{
		CSocketAddress a(portNo, address);
		m_addressee = a;
		m_isAddressBroadcast = true;
		m_SetAddress(a);
	}

	/**
	 * [���s] ���M
	 *	@note	�{���\�b�h���g���O�ɁA��x SetAddressee() �� SetBroadcastPort() �ő��M���ݒ肷��K�v������܂��B
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		_super::m_OnSend(dataSize, lpcvData);
		return m_Send(m_soData, dataSize, lpcvData);
	}

	/**
	 * [���s] �ؒf
	 */
	virtual void Stop(void)
	{
		m_isStopping = true;
		m_CloseDataSocket();
		m_threadReceiver.Stop();
		if ( IsConnect() )
		{
			m_CloseDataSocket();
		}
		m_isStopping = false;
	}

	/**
	 * [�Q��] ��M�X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadReceiver;
	}

	/**
	 * [�Q��] ��M�X���b�h��ԎQ��.
	 *	@note �����\�b�h���ł��B ReferThreadStatus() ���g���Ă��������B
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& GetReceiveThreadStatus(void)
	{
		return m_threadReceiver;
	}

	/**
	 * [�擾] ��M�o�b�t�@�T�C�Y�擾.
	 *	@retval 0 �擾���s�B Start ���Ă��Ȃ��A�ȂǁB
	 *	@retval 1�ȏ� �擾�����B�l�́A�o�b�t�@�T�C�Y�B
	 */
	size_t GetReceivedBufferSize(void) const
	{
		int len = 0;
		int lenlen = sizeof(len);
		int r = ::getsockopt(m_soData, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&len), &lenlen);
		return static_cast<size_t>((r == SOCKET_ERROR) ? 0 : len);
	}

	/**
	 * [�ݒ�] ��M�o�b�t�@�T�C�Y�ݒ�.
	 *	@param size ��M�o�b�t�@�T�C�Y
	 *	@retval true �����B
	 *	@retval false ���s�B Start ���Ă��Ȃ��A�ȂǁB
	 */
	bool SetReceivedBufferSize(size_t size)
	{
		if ( size < INT_MAX )
		{
			int len = ToInt(size);
			int r = ::setsockopt(m_soData, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&len), sizeof(len));
			if ( r != SOCKET_ERROR )
			{
				return size == GetReceivedBufferSize();
			}
		}
		return false;
	}

	/**
	 * [���M] Wake up on LAN
	 *	@param pbMacAddr 6BYTE��MAC�A�h���X�B
	 *	@retval true	�����B
	 *	@retval	false	���s�B
	 */
	static bool WakeupOnLan(const BYTE* pbMacAddr)
	{
		CSocketUdp udp;
		udp.Start();
		udp.SetBroadcastPort(7);
		ASSERTLIB( udp.GetAddressee().GetPortNo() == 7 );
		const int MACSIZE = 6;
		const int MACLOOP = 16;
		char sendbuf[MACLOOP * MACSIZE + MACSIZE];
		//	���M�f�[�^�쐬
		::FillMemory(&sendbuf[0], MACSIZE, 0xFF);
		loop ( i, MACLOOP )
		{
			::CopyMemory(&sendbuf[MACSIZE + i * MACSIZE], pbMacAddr, MACSIZE);
		}
		return udp.Send(MACLOOP * MACSIZE + MACSIZE, sendbuf);
	}

protected:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/**
	 * SOCKET�̐ڑ����擾
	 *	@param[in]	so		�_�~�[(�\�P�b�g)
	 *	@param[out]	_inaddr	�A�h���X
	 *	@param[out]	_wPort	�|�[�g
	 *	@retval true �A�h���X�ƃ|�[�g�擾�����B
	 *	@retval false ���s�B
	 */
	virtual bool m_GetPeerName(CSocketAddress& _address, SOCKET so)
	{
		_address.Set(m_addrFrom);
		return true;
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
		BOOL isAddressBroadcast = m_isAddressBroadcast;
		if ( m_isBroadcast != (!! isAddressBroadcast) )
		{
			::setsockopt(so, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&isAddressBroadcast), sizeof(isAddressBroadcast));
			m_isBroadcast = !! isAddressBroadcast;
		}
		int iRc = ::sendto(so, static_cast<const char*>(lpcvData), ToInt(dataSize), 0, m_sockaddrPartner, TSockAddr::GetSize());
		TRACE3( "::sendto(%u) result = %d error = %d\n", dataSize, iRc, ::WSAGetLastError() );
		return iRc == ToInt(dataSize);
	}
	#endif // _TnbDOXYGEN

private:
	/// �\�P�b�g�N���[�Y
	void m_CloseDataSocket(void)
	{
		if ( m_soData != INVALID_SOCKET )
		{
			SOCKET so = m_soData;
			m_soData = INVALID_SOCKET;
			TRACE1( "SocketUDP[%u]:Closed\n", m_baseAddress.GetPortNo() );
			::closesocket(so);
		}
	}
	/** 
	 * �X���b�h�����{��
	 *	@retval �O�Œ�B
	 */
	virtual DWORD Run(void)
	{
		if ( m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Connect)) )
		{
			CTickCount tickIdle;
			while ( IsRunnable() )
			{
				int	iInterval = _super::GetIdleInterval();
				if ( iInterval > 0 )
				{
					fd_set tFdSet;
					FD_ZERO(&tFdSet);
					FD_SET(m_soData, &tFdSet);
					int iSelTime = iInterval;
					timeval tTimeVal = { iSelTime / 1000, iSelTime % 1000 };
					::select(ToInt(m_soData + 1), &tFdSet, NULL, NULL, &tTimeVal);
					if ( ! FD_ISSET(m_soData, &tFdSet) )
					{
						//== �f�[�^�Ȃ�
						if ( iInterval > 0 && tickIdle.IsPassedAndReset(iInterval) )
						{
							if ( ! m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Idle)) )
							{
								break;	//�ؒf
							}
						}
						continue;
					}
				}
				TTRACE1( "SocketUDP[%u]:��M\n", m_baseAddress.GetPortNo() );
				int iFromLen = m_addrFrom.GetSize();
				int iLen = ::recvfrom(m_soData, m_lpcBuffer, ToInt(m_bufferSize), 0, m_addrFrom, &iFromLen);
				if ( iLen == SOCKET_ERROR )
				{
					if ( ! m_isStopping )
					{
						int iError = ::WSAGetLastError();
						TRACE2( "::recv(%d) error = %d\n", m_soData, iError );
						if ( ! m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Error, iError)) )
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				else if ( iLen != 0 )
				{
					CSocketEvent s(this, m_soData);
					s.SetData(iLen, m_lpcBuffer);
					if ( ! m_OnSocketEvent(s) )
					{
						break;
					}
				}
				else
				{
					//�f�[�^����
				}
				tickIdle.Reset();
			}
			m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Disconnect));
		}
		m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_End));
		return 0;
	}
	CThread			m_threadReceiver;		///< ��M�X���b�h
	SOCKET			m_soData;				///< �f�[�^�p�\�P�b�g
	CSocketAddress	m_baseAddress;			///< �x�[�X�A�h���X�i��M�A���M���|�[�g�j
	CSocketAddress	m_addressee;			///< ���M��A�h���X
	bool			m_isAddressBroadcast;	///< ���M��A�h���X�̓u���[�h�L���X�g�p�H
	TSockAddr		m_addrFrom;				///< ��M�����A�h���X&�|�[�g
	BOOL			m_isBroadcast;			///< SOCKET �� Broadcast�t���O�Z�b�g�ς݁H
	size_t			m_bufferSize;			///< ��M�o�b�t�@�T�C�Y
	bool			m_isStopping;			///< Stop������
};



/**@ingroup REPORT SOCKET
 * �g�� Socket UPD �ʐM
 *
 *		����M�f�[�^�̐擪 6byte �� TDataHeader (�A�h���X�iIP�A�h���X�A�|�[�gNo�j)���܂ނ悤�ɂȂ�܂��B
 *		���M�f�[�^�擪�� TDataHeader �ŁA���M����w��ł��܂��i�O�̏ꍇ�A SetAddressee() �̎w��ʂ�j�B
 *		��M���A�f�[�^�擪�� TDataHeader ���i�[����܂��B���ۂ̃f�[�^�� 6byte �ڂ���ɂȂ�܂��B
 *
 *	@note �A�h���X�́A���g���G���f�B�A���Ŋi�[����܂��B
 *
 *		���� CSocketUdp �N���X�Ɠ����ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSocketUdp.h
 * 
 *	@date 12/03/30	�V�K�쐬
 */
class CSocketUdpEx : public CSocketUdp
{
	DEFSUPER(CSocketUdp);
public:

	#pragma pack(push)
	#pragma pack(1)
	/**@ingroup REPORT SOCKET
	 * �g�� Socket UDP �ʐM�f�[�^�w�b�_.
	 *
	 *		����M�����f�[�^�̐擪�ɂ��ꂪ�܂܂�܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbSocketUdp.h
	 * 
	 *	@date 12/03/30	�V�K�쐬
	 */
	struct TDataHeader
	{
		DWORD			ipAddress;		///< IP�A�h���X
		WORD			portNo;			///< �|�[�gNo
		/**
		 * �R���X�g���N�^.
		 *	@param pn �|�[�gNo
		 *	@param ip IP�A�h���X
		 */
		TDataHeader(WORD pn = 0, DWORD ip = 0) : ipAddress(ip), portNo(pn)
		{
		}
		/**
		 * ����R���X�g���N�^.
		 *	@param addr �A�h���X�B
		 */
		TDataHeader(const CSocketAddress& addr)
		{
			operator=(addr);
		}
		/**
		 * [�擾] �A�h���X�擾.
		 *	@return �A�h���X
		 */
		operator CSocketAddress() const
		{
			return CSocketAddress(portNo, ipAddress);
		}
		/**
		 * [���] ����I�y���[�^.
		 *	@param addr �A�h���X
		 */
		void operator=(const CSocketAddress& addr)
		{
			ipAddress = addr.GetAddress();
			portNo = addr.GetPortNo();
		}
	};
	#pragma pack(pop)


	//---


	/// �R���X�g���N�^
	CSocketUdpEx(void)
	{
	}

	/**
	 * [�ݒ�] �u���[�h�o���h���M�w��
	 *	@note �擪6byte �� TDataHeader (���M��A�h���X(IP�A�h���X�A�|�[�gNo))�����Ă����K�v������܂��B
	 *		IP�A�h���X�� 0.0.0.0 �̏ꍇ�A255.255.255.255 �ɒu�������܂��B
	 *		���ۂɑ��M�����f�[�^�� offset 6 ����u���悤�ɂ��܂��B
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool Broadcast(size_t dataSize, LPCVOID lpcvData)
	{
		_super::m_OnSend(dataSize, lpcvData);
		C_ASSERT( sizeof(TDataHeader) == 6 );
		if ( dataSize < sizeof(TDataHeader) )
		{
			return false;
		}
		const TDataHeader* p = static_cast<const TDataHeader*>(lpcvData);
		TDataHeader h = AdjustDataHeader(*p);
		if ( p->ipAddress == 0 )
		{
			_super::SetBroadcastPort(h.portNo);
		}
		else
		{
			_super::SetBroadcastPort(h.portNo, p->ipAddress);
		}
		const BYTE* b = static_cast<const BYTE*>(lpcvData);
		return m_Send(_super::GetSocket(), dataSize - sizeof(TDataHeader), &b[sizeof(TDataHeader)]);
	}

	/**
	 * [���s] ���M
	 *	@note �擪6byte �� TDataHeader (���M��A�h���X(IP�A�h���X�A�|�[�gNo))�����Ă����K�v������܂��B
	 *		���ۂɑ��M�����f�[�^�� offset 6 ����u���悤�ɂ��܂��B
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		_super::m_OnSend(dataSize, lpcvData);
		C_ASSERT( sizeof(TDataHeader) == 6 );
		if ( dataSize < sizeof(TDataHeader) )
		{
			return false;
		}
		const TDataHeader* p = static_cast<const TDataHeader*>(lpcvData);
		TDataHeader h = AdjustDataHeader(*p);
		CSocketAddress sa(h.portNo, h.ipAddress);
		//
		EXCLUSIVE( &m_syncSend );
		_super::SetAddressee(sa);
		const BYTE* b = static_cast<const BYTE*>(lpcvData);
		return m_Send(_super::GetSocket(), dataSize - sizeof(TDataHeader), &b[sizeof(TDataHeader)]);
	}

	/**
	 * [�擾] �f�[�^�w�b�_����.
	 *		���f�[�^�w�b�_���� IP�A�h���X�A�|�[�gNo���O�̏ꍇ�A�{�C���X�^���X�ŋL�����Ă��鑗�M��̏��ɂ������̂�Ԃ��܂��B
	 *	@param h ���f�[�^�w�b�_
	 *	@return �f�[�^�w�b�_
	 */
	TDataHeader AdjustDataHeader(const TDataHeader& h) const
	{
		TDataHeader dh = h;
		if ( dh.ipAddress == 0 )
		{
			dh.ipAddress = _super::GetAddressee().GetAddress();;
		}
		if ( dh.portNo == 0 )
		{
			dh.portNo = _super::GetAddressee().GetPortNo();
		}
		return dh;
	}

protected:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/**
	 * �R�[���o�b�N�ʒm���s
	 *	@param e �ʒm������e
	 *	@retval true	����
	 *	@retval false	�ؒf�v��
	 */
	virtual bool m_OnSocketEvent(CSocketEvent& e)
	{
		CWorkMem w;
		CSocketEvent se = e;
		if ( se.HasData() )
		{
			w.Resize(sizeof(TDataHeader) + se.GetDataSize());
			BYTE* b = w.Ref();
			TDataHeader* h = reinterpret_cast<TDataHeader*>(b);
			CSocketAddress sa;
			e.GetPeerName(sa);
			h->ipAddress = sa.GetAddress();
			h->portNo = sa.GetPortNo();
			memcpy(&b[sizeof(TDataHeader)], e.GetData(), e.GetDataSize());
			se.SetData(w.GetSize(), w);
		}
		return _super::m_OnSocketEvent(se);
	}
	#endif // _TnbDOXYGEN

private:
	CSyncSection		m_syncSend;		///< ���M����
//	DWORD				m_defaultAddr;	///< �f�t�H���g�A�h���X
//	WORD				m_defaultPort;	///< �f�t�H���g�|�[�g
};



};
