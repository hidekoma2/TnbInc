#pragma once
/**
 *@file
 * ソケット(UDP関係) のアクセス関係のヘッダ
 *
 *		UDP のクラスがあります。
 *		受信処理は、リスナーを登録することで行います。
 *
 *	@note Winsock を使用しています。
 *
 *	@note	マクロ "_TnbWINSOCK2_ENABLE" が定義されていると、 Winsock2.0 を使用します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSocket.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT SOCKET
 * SocketUPDクラス
 *	
 *		UDPソケットを用意します。
 *
 *		受信は CAbstractSocket::IListener か IReport::IListener の実装クラスを作成し SetSocketListener() , SetListener() を使い登録します。
 *		両方登録している場合、受信時、先に CAbstractSocket::IListener に通知されます。
 *
 *	@note 送信サイズごとに受信されます( TCP と異なり、連結されることはありません )。
 *
 *	@note ReportEvent_Connect イベントは、Start 時、 ReportEvent_Disconnect イベントは、Stop直前に通知されます。
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbSocketUdp.h
 * 
 *	@date 06/07/06	新規作成
 *	@date 06/08/31	CommViewer に対応。
 *	@date 06/09/09	IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *					ECE_Connect , ECE_Disconnect イベント、追加。
 *	@date 06/10/24	Magicpacket対応。
 *	@date 07/06/11	親クラスの Start() メソッドに対応。
 *	@date 09/07/31	ECE_Idle イベントに対応。
 *	@date 09/08/27	SetReceiveThreadPriority() 追加。構成変更。IsAlive() の戻り値修正。
 *					GetIdleInterval() が0の時、 select せずに recv するようにした（速度を稼ぐため）
 *	@date 09/09/04	GetReceivedBufferSize(), SetReceivedBufferSize() 追加。
 *	@date 09/09/10	SetReceiveThreadPriority() 廃止、 GetReceiveThreadStatusGet() 追加。
 *	@date 10/03/19	ReferThreadStatus() 追加。
 *	@date 10/08/05	ITester 廃止。
 *	@date 11/06/14	GetReportID() メソッドの戻り値変更。
 *	@date 12/03/30	大幅改修。
 */
class CSocketUdp : public CAbstractSocket, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractSocket);
public:

	/// コンストラクタ
	CSocketUdp(void) : _super(), m_isStopping(false), m_soData(INVALID_SOCKET), m_isBroadcast(-1), m_bufferSize(0), m_isAddressBroadcast(false)
	{
		m_threadReceiver.SetRunner(this);
		m_addrFrom.Init();
	}

	/// デストラクタ
	virtual ~CSocketUdp(void)
	{
		Stop();
	}

	/**
	 * [取得] コミュニケーション名.
	 *	@note 開発用のメソッドです。
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const 
	{
		return CStr::Fmt(_T("SocketUDP[%u]:"), m_baseAddress.GetPortNo());
	}

	/**
	 * [取得] コミュニケーションID.
	 *		TNBVIW_KIND_??? + PORT を返します。
	 *	@note 開発用のメソッドです。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const 
	{
		return TNBVIW_KIND_SOCK_U + m_baseAddress.GetPortNo();
	}

	/**
	 * [取得] ソケット取得
	 *	@retval INVALID_SOCKET		接続していない
	 *	@retval INVALID_SOCKET以外	ソケット
	 */
	SOCKET GetSocket(void) const
	{
		return m_soData;
	}

	/**
	 * [取得] UDP送信最大サイズ取得.
	 *	@attention 本メソッドの値に関わらず、ネットワークの MTU (Maximum Transmission Unit) を
	 *				超えるブロードキャスト データグラムの送信はお薦めできません。
	 *	@return サイズ
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
	 * [設定] ベースアドレス設定.
	 *		受信に使用する IP アドレス(INADDR_ANY でも可)、ポートNo を指定します。
	 *		ポートNo は UDP ヘッダの送信元ポートNoにも使われます。
	 *	@attention Start() 前に使用してください。
	 *	@note ポートNo に０を指定すると送信専用になります（受信 Event は発生しません)。
	 *	@param address	アドレス
	 */
	void SetBaseAddress(const CSocketAddress& address)
	{
		StartUp();
		Stop();
		m_baseAddress = address;
	}

	/**
	 * [取得] ベースアドレス取得.
	 *	@return 受信に使用するアドレス
	 */
	const CSocketAddress& GetBaseAddress(void) const
	{
		return m_baseAddress;
	}

	/**
	 * [実行] 開始
	 *	@note	SetBaseAddress() で設定後、本メソッドを使用します。
	 *	@retval true	成功。送受信可能。
	 *	@retval false	失敗。
	 */
	virtual bool Start(void)
	{
		StartUp();
		Stop();
		// 自分用ソケット構造体の初期化
		TSockAddr addr;
		addr.Init();
		addr.sin_family = AF_INET;			// TCP/IPを指定
		m_baseAddress.Get(addr);
		// UDP オープン
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
		// 受信スレッド開始
		if ( m_baseAddress.GetPortNo() != 0 )
		{
			m_bufferSize = GetMaxDatagram();
			ASSERTLIB(m_bufferSize > 0);		
			delete m_lpcBuffer;
			m_lpcBuffer = new char[m_bufferSize];
			ASSERTLIB(m_lpcBuffer != NULL);		
			#ifdef _DEBUG
				m_threadReceiver.Start(CStr::Fmt(_T("SocketUDP[%u]監視"), m_baseAddress.GetPortNo()));
			#else
				m_threadReceiver.Start();
			#endif
		}
		return true;
	}

	/**
	 * [確認] Aliveチェック.
	 *		Start() メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const 
	{
		return m_soData != INVALID_SOCKET; 
	}
	
	/**
	 * [確認] 接続チェック.
	 *	@note 本クラスの場合、Start() が成功していたら接続中扱いにしています。
	 *	@retval true	接続中。
	 *	@retval	false	切断。
	 */
	virtual bool IsConnect(void) const
	{
		return m_soData != INVALID_SOCKET; 
	}

	/**
	 * [取得] 送信先アドレス取得.
	 *		SetAddressee() か SetBroadcastPort() で設定したアドレスが得られます。
	 *	@return アドレス
	 */
	const CSocketAddress& GetAddressee(void) const
	{
		return m_addressee;
	}

	/**
	 * [設定] 送信先アドレス指定
	 *	@note Start() の後、 Send() 前に本メソッドで送信先を設定する必要があります。
	 *	@param address	相手のIPアドレス&ポートNo
	 *	@retval true	成功。
	 *	@retval	false	失敗(名前が解決できない)
	 */
	bool SetAddressee(const CSocketAddress& address)
	{
		m_addressee = address;
		m_isAddressBroadcast = false;
		return m_SetAddress(address);
	}

	/**
	 * [設定] ブロードバンド送信指定
	 *	@note Start() の後、 Send() 前に本メソッドで送信先を設定する必要があります。
	 *	@param portNo	送信先ポートNo
	 *	@param address アドレス。省略すると INADDR_BROADCAST
	 */
	void SetBroadcastPort(WORD portNo, DWORD address = INADDR_BROADCAST)
	{
		CSocketAddress a(portNo, address);
		m_addressee = a;
		m_isAddressBroadcast = true;
		m_SetAddress(a);
	}

	/**
	 * [実行] 送信
	 *	@note	本メソッドを使う前に、一度 SetAddressee() か SetBroadcastPort() で送信先を設定する必要があります。
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		_super::m_OnSend(dataSize, lpcvData);
		return m_Send(m_soData, dataSize, lpcvData);
	}

	/**
	 * [実行] 切断
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
	 * [参照] 受信スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadReceiver;
	}

	/**
	 * [参照] 受信スレッド状態参照.
	 *	@note 旧メソッド名です。 ReferThreadStatus() を使ってください。
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& GetReceiveThreadStatus(void)
	{
		return m_threadReceiver;
	}

	/**
	 * [取得] 受信バッファサイズ取得.
	 *	@retval 0 取得失敗。 Start していない、など。
	 *	@retval 1以上 取得成功。値は、バッファサイズ。
	 */
	size_t GetReceivedBufferSize(void) const
	{
		int len = 0;
		int lenlen = sizeof(len);
		int r = ::getsockopt(m_soData, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&len), &lenlen);
		return static_cast<size_t>((r == SOCKET_ERROR) ? 0 : len);
	}

	/**
	 * [設定] 受信バッファサイズ設定.
	 *	@param size 受信バッファサイズ
	 *	@retval true 成功。
	 *	@retval false 失敗。 Start していない、など。
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
	 * [送信] Wake up on LAN
	 *	@param pbMacAddr 6BYTEのMACアドレス。
	 *	@retval true	成功。
	 *	@retval	false	失敗。
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
		//	送信データ作成
		::FillMemory(&sendbuf[0], MACSIZE, 0xFF);
		loop ( i, MACLOOP )
		{
			::CopyMemory(&sendbuf[MACSIZE + i * MACSIZE], pbMacAddr, MACSIZE);
		}
		return udp.Send(MACLOOP * MACSIZE + MACSIZE, sendbuf);
	}

protected:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	/**
	 * SOCKETの接続情報取得
	 *	@param[in]	so		ダミー(ソケット)
	 *	@param[out]	_inaddr	アドレス
	 *	@param[out]	_wPort	ポート
	 *	@retval true アドレスとポート取得成功。
	 *	@retval false 失敗。
	 */
	virtual bool m_GetPeerName(CSocketAddress& _address, SOCKET so)
	{
		_address.Set(m_addrFrom);
		return true;
	}
	/**
	 * 送信
	 *	@param so ソケット
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
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
	/// ソケットクローズ
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
	 * スレッド処理本体
	 *	@retval ０固定。
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
						//== データなし
						if ( iInterval > 0 && tickIdle.IsPassedAndReset(iInterval) )
						{
							if ( ! m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Idle)) )
							{
								break;	//切断
							}
						}
						continue;
					}
				}
				TTRACE1( "SocketUDP[%u]:受信\n", m_baseAddress.GetPortNo() );
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
					//データ無し
				}
				tickIdle.Reset();
			}
			m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Disconnect));
		}
		m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_End));
		return 0;
	}
	CThread			m_threadReceiver;		///< 受信スレッド
	SOCKET			m_soData;				///< データ用ソケット
	CSocketAddress	m_baseAddress;			///< ベースアドレス（受信、送信元ポート）
	CSocketAddress	m_addressee;			///< 送信先アドレス
	bool			m_isAddressBroadcast;	///< 送信先アドレスはブロードキャスト用？
	TSockAddr		m_addrFrom;				///< 受信したアドレス&ポート
	BOOL			m_isBroadcast;			///< SOCKET に Broadcastフラグセット済み？
	size_t			m_bufferSize;			///< 受信バッファサイズ
	bool			m_isStopping;			///< Stop処理中
};



/**@ingroup REPORT SOCKET
 * 拡張 Socket UPD 通信
 *
 *		送受信データの先頭 6byte に TDataHeader (アドレス（IPアドレス、ポートNo）)を含むようになります。
 *		送信データ先頭の TDataHeader で、送信先を指定できます（０の場合、 SetAddressee() の指定通り）。
 *		受信時、データ先頭に TDataHeader が格納されます。実際のデータは 6byte 目からになります。
 *
 *	@note アドレスは、リトルエンディアンで格納されます。
 *
 *		他は CSocketUdp クラスと同じです。
 *
 *	@par必要ファイル
 *			TnbSocketUdp.h
 * 
 *	@date 12/03/30	新規作成
 */
class CSocketUdpEx : public CSocketUdp
{
	DEFSUPER(CSocketUdp);
public:

	#pragma pack(push)
	#pragma pack(1)
	/**@ingroup REPORT SOCKET
	 * 拡張 Socket UDP 通信データヘッダ.
	 *
	 *		送受信されるデータの先頭にこれが含まれます。
	 *
	 *	@par必要ファイル
	 *			TnbSocketUdp.h
	 * 
	 *	@date 12/03/30	新規作成
	 */
	struct TDataHeader
	{
		DWORD			ipAddress;		///< IPアドレス
		WORD			portNo;			///< ポートNo
		/**
		 * コンストラクタ.
		 *	@param pn ポートNo
		 *	@param ip IPアドレス
		 */
		TDataHeader(WORD pn = 0, DWORD ip = 0) : ipAddress(ip), portNo(pn)
		{
		}
		/**
		 * 代入コンストラクタ.
		 *	@param addr アドレス。
		 */
		TDataHeader(const CSocketAddress& addr)
		{
			operator=(addr);
		}
		/**
		 * [取得] アドレス取得.
		 *	@return アドレス
		 */
		operator CSocketAddress() const
		{
			return CSocketAddress(portNo, ipAddress);
		}
		/**
		 * [代入] 代入オペレータ.
		 *	@param addr アドレス
		 */
		void operator=(const CSocketAddress& addr)
		{
			ipAddress = addr.GetAddress();
			portNo = addr.GetPortNo();
		}
	};
	#pragma pack(pop)


	//---


	/// コンストラクタ
	CSocketUdpEx(void)
	{
	}

	/**
	 * [設定] ブロードバンド送信指定
	 *	@note 先頭6byte に TDataHeader (送信先アドレス(IPアドレス、ポートNo))を入れておく必要があります。
	 *		IPアドレスが 0.0.0.0 の場合、255.255.255.255 に置き換えます。
	 *		実際に送信されるデータは offset 6 から置くようにします。
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
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
	 * [実行] 送信
	 *	@note 先頭6byte に TDataHeader (送信先アドレス(IPアドレス、ポートNo))を入れておく必要があります。
	 *		実際に送信されるデータは offset 6 から置くようにします。
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
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
	 * [取得] データヘッダ調整.
	 *		元データヘッダ内の IPアドレス、ポートNoが０の場合、本インスタンスで記憶している送信先の情報にしたものを返します。
	 *	@param h 元データヘッダ
	 *	@return データヘッダ
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

	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	/**
	 * コールバック通知実行
	 *	@param e 通知する内容
	 *	@retval true	正常
	 *	@retval false	切断要求
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
	CSyncSection		m_syncSend;		///< 送信同期
//	DWORD				m_defaultAddr;	///< デフォルトアドレス
//	WORD				m_defaultPort;	///< デフォルトポート
};



};
