#pragma once
/**
 *@file
 * ソケットのアクセス関係のヘッダ
 *
 *		サーバとクライアントのクラスがあります。
 *		受信処理は、リスナーを登録することで行います。
 *		サーバはマルチクライアントをサポートしています。
 *
 *	@todo IPv4 のみ対応。 IPv6 は未対応。
 *
 *	@note Winsock を使用しています。
 *
 *	@note	マクロ "_TnbWINSOCK2_ENABLE" が定義されていると、 Winsock2.0 を使用します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAbstractReport.h"
#include "TnbThread.h"
#include "TnbVector.h"
#include "TnbIcmp.h"



//TNB Library
namespace TNB
{



/// SOCKETの配列型宣言
typedef TNB::CVectorT<SOCKET> CSocketVector;



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 _SELECTANY bool s_boIsInitSock = false;
 _SELECTANY int s_iMaxUdpDg = 0;
#endif



 /**@ingroup REPORT SOCKET
 * Socket共通アドレス管理クラス
 *
 *		一対のポート No と IP アドレスを管理します。
 *
 *	@note IP アドレスを文字列で登録している場合、アドレス取得時に gethostbyname() を使って処理しています。
 *
 *	@note 本クラスが引数や戻り値で扱う DWORD 型の IP アドレス、WORD 型のポート No は、
 *		Host Byte Order になります。socket の関数に使用する場合、注意してください。
 *		値が Network Byte Order の場合、明記しています。
 *
 *	@par必要ファイル
 *			TnbSocket.h
 * 
 *	@date 09/08/27	新規作成
 */
class CSocketAddress
{
public:

	/**
	 * コンストラクタ
	 *	@param portNo ポート No。省略すると、０になります。
	 *	@param ipAddress IP アドレス。省略すると、 INADDR_ANY になります。
	 */
	CSocketAddress(WORD portNo = 0, DWORD ipAddress = 0) : m_portNo(portNo), m_isNameMode(false)
	{
		m_inaddr.s_addr = ::htonl(ipAddress);
	}

	/**
	 * コンストラクタ.
	 *	@note IPアドレスは、 "127.0.0.1" や "LD04001" などが指定できます。
	 *	@param portNo ポートNo
	 *	@param lpszIpAddressName IPアドレス名。
	 */
	CSocketAddress(WORD portNo, LPCTSTR lpszIpAddressName) : m_portNo(portNo), m_isNameMode(true)
	{
		m_inaddr.s_addr = 0;
		m_ipAddressName = lpszIpAddressName;
	}

	/**
	 * [設定] アドレス設定.
	 *		IPアドレスとポートNoを設定します。
	 *	@param portNo ポート No。
	 *	@param ipAddress IP アドレス。省略すると、 INADDR_ANY になります。
	 */
	void Set(WORD portNo, DWORD ipAddress = 0)
	{
		m_isNameMode = false;
		m_inaddr.s_addr = ::htonl(ipAddress);
		m_ipAddressName.Empty();
		m_portNo = portNo;
	}

	/**
	 * [設定] アドレス設定.
	 *		IPアドレスとポートNoを設定します。
	 *	@note IPアドレスは、 "127.0.0.1" や "LD04001" などが指定できます。
	 *	@param portNo ポートNo
	 *	@param lpszIpAddressName IPアドレス名。
	 */
	void Set(WORD portNo, LPCTSTR lpszIpAddressName)
	{
		m_isNameMode = true;
		m_inaddr.s_addr = 0;
		m_portNo = portNo;
		m_ipAddressName = lpszIpAddressName;
	}

	/**
	 * [設定] アドレス設定.
	 *		IPアドレスとポートNoを設定します。
	 *	@param sockaddr Network Byte Order のポート No と IP アドレス
	 *				（SOCKADDR_IN の sin_addr メンバと sin_port メンバのみ参照します）。
	 */
	void Set(const SOCKADDR_IN& sockaddr)
	{
		m_isNameMode = false;
		m_inaddr = sockaddr.sin_addr;
		m_ipAddressName.Empty();
		m_portNo = ::ntohs(sockaddr.sin_port);
	}

	/**
	 * [取得] IPアドレス取得.
	 *	@param[out] _inaddr IP アドレスが Network Byte Order で格納されます。
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
				// ドット形式でないとき
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
	 * [取得] IP アドレス＆ポート No 取得.
	 *	@param[out] _sockaddr IP アドレスとポート No が Network Byte Order で格納されます。
	 *				（SOCKADDR_IN の sin_addr メンバと sin_port メンバのみ更新します）。
	 */
	void Get(SOCKADDR_IN& _sockaddr) const
	{
		Get(_sockaddr.sin_addr);
		_sockaddr.sin_port = ::htons(m_portNo);
	}

	/**
	 * [取得] ホスト名取得.
	 *		DNS から保持している IP アドレスのホスト名を取得します。
	 *	@note 保持している IP アドレスが、 INADDR_ANY の場合、自分のホスト名が取得されます。
	 *	@retval Empty 見つからなかった。
	 *	@retval 上記以外 ホスト名。
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
	 * [取得] IP アドレス文字列取得.
	 *	@param withDns true なら DNS からホスト名を取得を試みます。取得できなかった場合、数値の羅列("127.0.0.1") を返します。
	 *					false なら DNS に問い合わせせず、数値の羅列を返します。
	 *	@return IPアドレス文字列
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
	 * [取得] IP アドレス取得.
	 *	@return IP アドレス。
	 */
	DWORD GetAddress(void) const
	{
		in_addr addr;
		Get(addr);
		return ::htonl(addr.s_addr);
	}

	/**
	 * [取得] ポートNo取得.
	 *	@return ポートNo
	 */
	WORD GetPortNo(void) const
	{
		return m_portNo;
	}

	/**
	 * [確認] ブロードキャスト？.
	 *	@retval true 保持してるアドレスはブロードキャスト.
	 *	@retval false 否
	 */
	bool IsBroadcast(void) const
	{
		return GetAddress() == INADDR_BROADCAST;
	}

	/**
	 * [変換] バイトオーダー変換(host to network)
	 *		unsigned long (32bit) を Host Byte Order から Network Byte Order に変換します。
	 *	@note 「Host TO Network Long」の略です。
	 *	@param hostlong Host Byte Order の値
	 *	@return Network Byte Order の値
	 */
	static DWORD htonl(DWORD hostlong)
	{
		return ::htonl(hostlong);
	}

	/**
	 * [変換] バイトオーダー変換(host to network)
	 *		unsigned short (16bit) を Host Byte Order から Network Byte Order に変換します。
	 *	@note 「Host TO Network Short」の略です。
	 *	@param hostshort Host Byte Order の値
	 *	@return Network Byte Order の値
	 */
	static WORD htons(WORD hostshort)
	{
		return ::htons(hostshort);
	}

	/**
	 * [変換] バイトオーダー変換(network to host)
	 *		unsigned long (32bit) を Network Byte Order から Host Byte Order に変換します。
	 *	@note 「Network TO Host Long」の略です。
	 *	@param netlong  Network Byte Order の値
	 *	@return  Host Byte Order の値
	 */
	static DWORD ntohl(DWORD netlong)
	{
		return ::ntohl(netlong);
	}

	/**
	 * [変換] バイトオーダー変換(network to host)
	 *		unsigned short (16bit) を Network Byte Order から Host Byte Order に変換します。
	 *	@note 「Network TO Host Short」の略です。
	 *	@param netshort  Network Byte Order の値
	 *	@return  Host Byte Order の値
	 */
	static DWORD ntohs(DWORD netshort)
	{
		return ::ntohl(netshort);
	}

private:
	bool		m_isNameMode;		///< true なら m_addrName で管理、 false なら m_inaddr で管理
	IN_ADDR		m_inaddr;			///< IPアドレス値(network byte order)
	CAscii		m_ipAddressName;	///< IPアドレス名
	WORD		m_portNo;			///< ポートNo(host byte order)
};



/**@ingroup REPORT SOCKET
 * Socket共通受信イベント管理クラス
 *
 *		リスナーの OnSocketEvent() メソッドで通知される情報です。
 *		データ（長さとアドレス）かイベントのいずれかを保持します。
 *
 *	@see CAbstractSocket::IListener::OnSocketEvent()
 *
 *	@par必要ファイル
 *			TnbSocket.h
 * 
 *	@date 06/04/01	新規作成
 *	@date 06/07/07	InterfaceID概念、追加
 *	@date 09/08/27	アドレス指定に CSocketAddress を使うように変更。
 *	@date 11/06/14 GetInterfaceId() メソッドの戻り値変更。
 */
class CSocketEvent : public CReportEvent
{
	DEFSUPER(CReportEvent);
	SOCKET						m_soData;	///< SOCKET
	class CAbstractSocket *		m_pcSocket;	///< SOCKET Objectの参照
public:

	/**
	 * コンストラクタ
	 *	@param pcSocket ソケットクラスのポインタ
	 *	@param so ソケット
	 *	@param eEvent イベントコード（省略可能）
	 *	@param dwError エラーコード（省略可能）
	 */
	CSocketEvent(CAbstractSocket* pcSocket, SOCKET so, EReportEvent eEvent = ReportEvent_Non, DWORD dwError = 0);

	/**
	 * [送信] データ送信
	 *	@note 送信元にデータを送信します。
	 *	@param dataSize 送信するデータ長。
	 *	@param lpcvData 送信するデータ。
	 *	@retval true	成功。
	 *	@retval	false	失敗。
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData) const;

	/**
	 * [取得] インターフェースID取得
	 *	@note 自動的に値が決められてしまいますが、一つの接続中、値が変わることは有りません。
	 *		別の接続と値が重複することも有りません。
	 *	@note	Socket 系では、インターフェースIDは、ユーザは指定できません。
	 *			SOCKET ハンドル + 0x80000000の値になります。
	 *	@return ID
	 */
	virtual DWORD GetInterfaceId(void) const
	{
		return static_cast<DWORD>(m_soData) | 0x80000000;
	}

	/**
	 * [取得] 送信元情報取得
	 *	@note Dataを持っている場合、送信した相手の情報を得ることが出来ます。
	 *	@param[out]	_address アドレス
	 *	@retval true アドレスとポート取得成功。
	 *	@retval false 失敗。
	 */
	bool GetPeerName(CSocketAddress& _address) const;

	/**
	 * [取得] 受信側情報取得
	 *	@param[out]	_address アドレス
	 *	@retval true アドレスとポート取得成功。
	 *	@retval false 失敗。
	 */
	bool GetSockName(CSocketAddress& _address) const;

	/**
	 * [取得] ソケット取得
	 *	@return ソケット
	 */
	SOCKET GetSocket(void) const
	{
		return m_soData;
	}
};



/**@ingroup REPORT SOCKET
 * Socket共通クラス
 *	
 *		ServerとClientの抽象的なクラスです。
 *		ユーザが直接使うことは有りません。
 *
 *	@date 06/04/01	新規作成
 *	@date 06/05/11	IReport 実装
 *	@date 06/06/22	ping処理を CPing クラスに委譲。
 *	@date 06/07/07	SOCKADDR_INをラッピング
 *	@date 07/02/09	親クラス変更。
 *	@date 09/08/27	構成変更。
 *	@date 09/08/27	アドレス指定に CSocketAddress を使うように変更。
 *	@date 12/03/13	Socket共通リスナーを先にコールするように変更。
 *	@date 12/03/30	SetSocketListener() に virtual を付加。
 */
class CAbstractSocket : public CAbstractReport
{
	DEFSUPER(CAbstractReport);
	friend class CSocketEvent;		///< フレンドクラス定義
public:

	/**@ingroup REPORT SOCKET
	 * Socket共通のリスナーインターフェース
	 *
	 *		CAbstractSocket のサブクラスの通知用インターフェースです。
	 *
	 *	@see SetSocketListener()
	 *
	 *	@par必要ファイル
	 *			TnbSocket.h
	 * 
	 *	@date 06/04/01	新規作成
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] イベント発生.
		 *		Openしたデバイスからデータを受信をした時など、コールされます。
		 *	@note IReport のリスナーより先に通知されます。
		 *	@note 自分で Disconnect() をしても、ReportEvent_Disconnect が通知されます。
		 *	@note CSocketEvent に Send() メソッドが有ります。受信して即送信する場合に使用できます。
		 *	@note	本クラスのイベントは「{@link TNB::ReportEvent_Error ReportEvent_Error}」
		 *			「{@link TNB::ReportEvent_Connect ReportEvent_Connect}」
		 *			「{@link TNB::ReportEvent_Disconnect ReportEvent_Disconnect}」です。
		 *	@param ev 受信した内容（データかイベント）。
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnSocketEvent(const CSocketEvent& ev) = 0;
	};


	//----------------------------


	/// コンストラクタ 
	CAbstractSocket(void) : _super(), m_piSocketListner(NULL), m_boIsDelay(true)
	{
		m_lpcBuffer = new char[E_BufferSize];
	}

	/// デストラクタ
	virtual ~CAbstractSocket(void)
	{
		delete[] m_lpcBuffer;
	}

	/**
	 * [設定] リスナー登録
	 *	@note リスナーを登録しないと受信情報が受けられません
	 *	@note 登録できる CAbstractSocket::IListener は一つだけです。
	 *	@param piSocketListener リスナー
	 */
	virtual void SetSocketListener(IListener* piSocketListener)
	{
		m_piSocketListner = piSocketListener;
	}

	/**
	 * [設定] SOCKET初期化.
	 *		WSAStartup() コールします。すでにコールしている場合は、何もしません。
	 *	@note	通常、ソケット使用時に自動的に使用されますので、コールする必要は有りません。
	 *			TNB Library 以外のソケット関係を先に使用する場合、WSAStartup() の代わりに
	 *			使用してください。
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
				static CCleanup s_cleanup; // Cleanup用インスタンス
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
	 * [取得] 自ＩＰアドレス取得.
	 *		自分のＩＰアドレスを取得します。２つ以上ある場合があります。
	 *	@return 自分のＩＰアドレス
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

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	/// SOCKADDR_INラッピングクラス
	struct TSockAddr : SOCKADDR_IN
	{
		/// 初期化
		void Init(void) { memset(this, 0, sizeof(SOCKADDR_IN)); }
		/// 構造体サイズ
		static int GetSize(void) { return sizeof(SOCKADDR_IN); }
		/// アドレス要求
		operator LPSOCKADDR(void) { return reinterpret_cast<LPSOCKADDR>(this); }
	};

	enum
	{
		#ifdef _TnbWINSOCK2_ENABLE
		 E_DefaultVersion	= WINSOCK_VERSION,	///< Winsockのバージョン
		#else
		 E_DefaultVersion	= MAKEWORD(1, 1),	///< Winsockのバージョン
		#endif
		E_BufferSize		= 10000,	///< TCPの１受信最大サイズ		
		E_RetryDelay		= 2000,		///< リトライまでのms		
		E_SelectTimeout		= 2000,		///< selectのタイムアウトms	
		E_ConnectTimeout	= 2000,		///< connectのタイムアウトms	
	};
	TSockAddr		m_sockaddrPartner;	///< ソケットアドレス
	IListener *		m_piSocketListner;	///< リスナー
	char *			m_lpcBuffer;		///< 受信用バッファ	
	bool			m_boIsDelay;		///< trueなら遅延送信。falseなら即送信。

	/// WinsockCleanup処理クラス
	class CCleanup
	{
	public:~CCleanup(void) { ::WSACleanup(); }
	};

	/**
	 * IP計算
	 *	@param[out] _dwIP 変換後の IP。
	 *	@param[in] lpszAddress アドレス文字列。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool ms_AddressToIp(IN_ADDR& _addr, LPCTSTR lpszAddress)
	{
		StartUp();
		CSocketAddress a(0, lpszAddress);
		a.Get(_addr);
		return (_addr.s_addr != INADDR_NONE);
	}

	/**
	 * アドレス設定.
	 *		メンバにアドレス、ポートNOを設定します。
	 *	@param lpszAddress IPアドレス
	 *	@param wPortNo ポートNO
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	bool m_SetAddress(LPCTSTR lpszAddress, WORD wPortNo)
	{
		// ソケット構造体の初期化
		m_sockaddrPartner.Init();
		m_sockaddrPartner.sin_family = AF_INET;			// TCP/IPを指定
		CSocketAddress a(wPortNo, lpszAddress);
		a.Get(m_sockaddrPartner);
		return m_sockaddrPartner.sin_addr.s_addr != INADDR_NONE;
	}

	/**
	 * アドレス設定.
	 *		メンバにアドレス、ポートNo を設定します。
	 *	@param address IPアドレス & ポートNo
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	bool m_SetAddress(const CSocketAddress& address)
	{
		// ソケット構造体の初期化
		m_sockaddrPartner.Init();
		m_sockaddrPartner.sin_family = AF_INET;			// TCP/IPを指定
		address.Get(m_sockaddrPartner);
		return m_sockaddrPartner.sin_addr.s_addr != INADDR_NONE;
	}

	/**
	 * 受信.
	 *		クラス内のバッファに受信します。
	 *	@param so ソケット
	 *	@retval true	成功
	 *	@retval false	失敗（切断要求）
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
			return false;	//切断
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
				return false;	//切断
			}
		}
		return true;
	}

	/**
	 * 送信遅延モード設定
	 *	@param so ソケット
	 *	@param boIsDelay trueなら遅延送信。falseなら即送信。
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
	 * 送信
	 *	@param so ソケット
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
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
	 * SOCKETの接続情報取得
	 *	@param[out]	_addres	アドレス&ポート
	 *	@param[in]	so		ソケット
	 *	@retval true アドレスとポート取得成功。
	 *	@retval false 失敗。
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
	 * コールバック通知実行
	 *	@param e 通知する内容
	 *	@retval true	正常
	 *	@retval false	切断要求
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
	 * UDP送信最大サイズ取得
	 *	@return サイズ
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



/// コンストラクタ
inline CSocketEvent::CSocketEvent(CAbstractSocket* pcSocket, SOCKET so, EReportEvent eEvent, DWORD dwError)
	: _super(static_cast<IReport*>(pcSocket), eEvent, dwError)
{
	m_soData	= so;
	m_pcSocket	= pcSocket;
}

// データ送信
inline bool CSocketEvent::Send(size_t dataSize, LPCVOID lpcvData) const
{
	return m_pcSocket->Send(dataSize, lpcvData);
//	return m_pcSocket->m_Send(m_soData, dataSize, lpcvData);
}

//情報取得
inline bool CSocketEvent::GetPeerName(CSocketAddress& _address) const
{
	return m_pcSocket->m_GetPeerName(_address, m_soData);
}

//情報取得
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
 * Socketクライアントクラス
 *	
 *		クライアント用のソケットを用意します。
 *		定期的にServerを探す自動接続モードもあります。
 *
 *		受信は CAbstractSocket::IListener のサブクラスを登録し行います。
 *		( IReport::IListener でも受信は出来ますが基本機能しかありません)
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbSocket.h
 * 
 *	@date 06/04/01	新規作成
 *	@date 06/04/27	メソッドでの切断でもDisconnectイベントがあがるようにした。
 *	@date 06/06/07	connectのタイムアウト時間を調整。
 *	@date 06/06/22	pingによる切断監視を追加。
 *	@date 06/07/31	「Start時にサーバアドレスを得ていたため、そこでつながっていないと二度とつながらない」現象を修正。
 *	@date 06/08/31	CommViewer に対応。
 *	@date 06/09/08	Idleイベントサポート。 IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *	@date 07/02/23	Connect時、名前が解決できないと、自動モードなのにつながらなくなるのを修正。
 *	@date 07/06/11	親クラスの Start() メソッドに対応。
 *	@date 09/02/27	リトライ待ち処理を改善。
 *	@date 09/08/27	SetReceiveThreadPriority() 追加。構成変更。
 *	@date 09/08/27	アドレス指定に CSocketAddress を使うように変更。
 *	@date 09/09/10	SetReceiveThreadPriority() 廃止、 GetReceiveThreadStatusGet() 追加。
 *	@date 10/03/19	ReferThreadStatus() 追加。
 *	@date 10/08/05	ITester 廃止。
 *	@date 11/06/14	GetReportID() メソッドの戻り値変更。
 */
class CSocketClient : public CAbstractSocket, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractSocket);
public:

	/**
	 * コンストラクタ
	 *	@param isAutoReconnect 自動接続モード。
	 *							false なら切断したら、切断のままになります。
	 *							省略か true なら切断しても自動的にリトライします。
	 */
	explicit CSocketClient(bool isAutoReconnect = true) 
		: _super(), m_soData(INVALID_SOCKET), m_isAutoReconnect(isAutoReconnect), m_wServerPortNo(0)
	{
		m_threadReceiver.SetRunner(this);
	}

	/// デストラクタ
	virtual ~CSocketClient(void)
	{
		m_CloseDataSocket();
		m_threadReceiver.Stop();
	}

	/**
	 * [設定] 送信遅延モード設定
	 *	@note 接続する前に設定する必要があります。
	 *	@note デフォルトでは遅延送信モードです。
	 *	@param boIsDelay trueなら遅延送信。falseなら即送信。
	 */
	void SetDelayMode(bool boIsDelay)
	{
		m_boIsDelay = boIsDelay;
	}

	/**
	 * [取得] コミュニケーション名.
	 *	@note 開発用のメソッドです。
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const 
	{
		return CStr::Fmt(_T("SocketClient[%u]:"), m_soData);
	}

	/**
	 * [取得] コミュニケーションID.
	 *		TNBVIW_KIND_??? + PORT を返します。
	 *	@note 開発用のメソッドです。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const 
	{
		return TNBVIW_KIND_SOCK_C + (m_soData & 0xFFFF);
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
	 * [設定] サーバ設定
	 *	@param address	IPアドレス＆ポートNo
	 */
	void SetServer(const CSocketAddress& address)
	{
		StartUp();
		Stop();
		m_strServerAddress = address.GetAddressString(false);
		m_wServerPortNo = address.GetPortNo();
	}

	/**
	 * [実行] 開始
	 *	@note 先に SetServer() でサーバの設定を行う必要があります。
	 *	@retval true	成功。ただし自動接続モードの場合、現状コネクトしていない可能性有り。
	 *	@retval false	コネクト失敗。自動接続モードの場合、スレッドの起動にも失敗している場合。
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
			m_threadReceiver.Start(CStr::Fmt(_T("Client(p=%d)監視"), m_wServerPortNo));
		#else
			m_threadReceiver.Start();
		#endif
		return true;
	}

	/**
	 * [実行] 開始
	 *	@note SetServer() と Start() を実行しています。
	 *	@param address	IPアドレス＆ポートNO
	 *	@retval true	成功。ただし自動接続モードの場合、現状コネクトしていない可能性有り。
	 *	@retval false	コネクト失敗。自動接続モードの場合、スレッドの起動にも失敗している場合。
	 */
	bool Connect(const CSocketAddress& address)
	{
		SetServer(address);
		return Start();
	}

	/**
	 * [確認] Aliveチェック.
	 *		Connect() メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const
	{
		return m_threadReceiver.IsAlive(); 
	}
	
	/**
	 * [確認] 接続確認
	 *	@retval true	接続中
	 *	@retval false	切断中
	 */
	virtual bool IsConnect(void) const
	{
		return m_soData != INVALID_SOCKET; 
	}

	/**
	 * [実行] 送信
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Send(size_t dataSize, LPCVOID lpcvData)
	{
		TTRACE1("CSocket[Client%d]:Send\n", m_soData);
		_super::m_OnSend(dataSize, lpcvData);
		return m_Send(m_soData, dataSize, lpcvData);
	}

	/**
	 * [実行] 切断
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
	 * [実行] 切断
	 */
	void Disconnect(void)
	{
		Stop(); 
	}

	/**
	 * [設定] 受信スレッド状態種取得.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadReceiver;
	}

#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// [実行] 開始. 旧タイプの指定方法です。 廃止予定です。
	bool Connect(LPCTSTR lpszAddress, WORD wPortNo)
	{
		SetServer(CSocketAddress(wPortNo, lpszAddress));
		return Start();
	}
#endif

private:
	TNB::CThread	m_threadReceiver;	///< 受信スレッド
	bool			m_isAutoReconnect;	///< 自動接続モード
	SOCKET			m_soData;			///< データ用ソケット
	CStr			m_strServerAddress;	///< サーバアドレス控え
	WORD			m_wServerPortNo;	///< サーバポート控え

	/**
	 * タイムアウト付きコネクト
	 *	@param so ソケット。
	 *	@param dwTimeout タイムアウト(ms)
	 *	@retval true	サーバに接続成功
	 *	@retval false	サーバ見つからず
 	 */
	bool m_ConnectWithTimeout(SOCKET so, DWORD dwTimeout)
	{
		DWORD dwOpt = 1;	//非ブロッキングに設定
		::ioctlsocket(so, FIONBIO, &dwOpt);
		int iCntRst = ::connect(so, m_sockaddrPartner, m_sockaddrPartner.GetSize());
		if ( iCntRst == SOCKET_ERROR ) 
		{
			DWORD dwError = ::WSAGetLastError();
			if ( dwError != WSAEWOULDBLOCK )
			{
				TRACE2("::connect(%d) error = %d\n", so, dwError);
				dwOpt = 0;	//ブロッキングに設定
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
				dwOpt = 0;	//ブロッキングに設定
				::ioctlsocket(so, FIONBIO, &dwOpt);
				return false;
			}		
		}
		dwOpt = 0;	//ブロッキングに設定
		::ioctlsocket(so, FIONBIO, &dwOpt);
		return true;
	}

	/**
	 * コネクト処理
	 *	@note SOCKETをつくりConnectを試みます。
	 *		成功するとm_soDataに有効なソケットが入ります。
	 *	@retval true	サーバに接続成功
	 *	@retval false	サーバ見つからず
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

	/// ソケットクローズ
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
	 * スレッド処理本体
	 *	@retval ０固定。
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
							break;	//切断
						}
					}
					if ( iInterval > 0 && tickIdle.IsPassedAndReset(iInterval) )
					{
						if ( ! m_OnSocketEvent(CSocketEvent(this, m_soData, ReportEvent_Idle)) )
						{
							break;	//切断
						}
					}
					continue;
				}
				if ( ! m_Recv(m_soData) )
				{
					break;	//切断
				}
				TTRACE1("CSocket[Client%d]:受信成功\n", m_soData);
				tickIdle.Reset();
			}
			//
			TTRACE1("CSocket[Client%d]:受信停止\n", m_soData);
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
 * Socketサーバクラス
 *	
 *		サーバ用にソケットを用意します。
 *
 *		受信は CAbstractSocket::IListener のサブクラスを登録し行います。
 *		( IReport::IListener でも受信は出来ますが基本機能しかありません)
 *
 *	@note IReport::ITester を登録しても、受信テストは出来ません
 *			( OnCommTestReceive() はコールされません）。
 *
 *	@note	アイドルイベントは、接続されている全クライアントからの受信がなくなって 数ms経った時に、
 *			全接続に対して、アイドルイベントが通知されます。各接続毎ではないことにご注意ください。
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbSocket.h
 * 
 *	@date 06/04/01	新規作成
 *	@date 06/04/27	接続可能数をつけた。メソッドでの切断でも
 *					Disconnectイベントがあがるようにした。
 *	@date 06/06/22	pingによる切断監視を追加。
 *	@date 06/08/31	CommViewer に対応。
 *	@date 06/09/08	Idleイベントサポート。 IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *	@date 07/06/11	親クラスの Start() メソッドに対応。
 *	@date 09/01/15	一度必ずListenエラーが発生するのを修正。
 *	@date 09/02/27	リトライ待ち処理を改善。
 *	@date 09/08/27	SetReceiveThreadPriority() 追加。構成変更。
 *	@date 09/08/27	アドレス指定に CSocketAddress を使うように変更。
 *	@date 09/09/10	SetReceiveThreadPriority() 廃止、 GetReceiveThreadStatusGet() 追加。
 *	@date 10/03/19	ReferThreadStatus() 追加。
 *	@date 11/06/14	GetReportID() メソッドの戻り値変更。
 */
class CSocketServer : public CAbstractSocket, TNB::CThread::IRunner
{
	DEFSUPER(CAbstractSocket);
public:

	/// コンストラクタ
	CSocketServer(void) : _super(), m_soListen(INVALID_SOCKET), m_connectMax(1), m_wOpenPortNo(0)
	{
		m_threadReceiver.SetRunner(this);
	}

	/// デストラクタ
	virtual ~CSocketServer(void)
	{
		Close(); 
	}

	/**
	 * [設定] 送信遅延モード設定
	 *	@note 接続する前に設定する必要があります。
	 *	@note デフォルトでは遅延送信モードです。
	 *	@param boIsDelay trueなら遅延送信。falseなら即送信。
	 */
	void SetDelayMode(bool boIsDelay)
	{
		m_boIsDelay = boIsDelay;
	}

	/**
	 * [取得] コミュニケーション名.
	 *	@note 開発用のメソッドです。
	 *	@return 名前
	 */
	virtual CStr GetReportName(void) const 
	{
		return CStr::Fmt(_T("SocketServer[%08X]:"), this);
	}

	/**
	 * [取得] コミュニケーションID.
	 *		TNBVIW_KIND_??? + PORT を返します。
	 *	@note 開発用のメソッドです。
	 *	@return ID
	 */
	virtual DWORD GetReportID(void) const 
	{
		return TNBVIW_KIND_SOCK_S + (reinterpret_cast<DWORD>(this) & 0xFFFF);
	}

	/**
	 * [設定] 接続可能数.
	 *		一つのサーバに接続できる数を制限します。
	 *		デフォルトは一つです。
	 *	@note	サーバをオープンしてからも数の変更が可能です。
	 *			ただし、現在の接続数より小さくしたとき、切断はされません。
	 *	@param max 接続可能数
	 */
	void SetConnectMax(size_t max)
	{
		if ( max >= 1 )
		{
			m_connectMax = max; 
		}
	}

	/**
	 * [設定] サーバ設定
	 *	@param address ポートNO＆待ち受けるアドレス。
	 */
	void SetParameter(const CSocketAddress& address)
	{
		StartUp();
		Stop();
		m_wOpenPortNo = address.GetPortNo();
		m_strAwaitAddress = address.GetAddressString(false);
	}

	/**
	 * [実行] 開始.
	 *		サーバをオープンし通信を開始します。
	 *	@note	SetParameter() で設定後、本メソッドを使用します。
	 *	@retval true	成功
	 *	@retval false	失敗
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
			m_threadReceiver.Start(CStr::Fmt(_T("Server(p=%d)監視"), m_wOpenPortNo));
		#else
			m_threadReceiver.Start();
		#endif
		return true;
	}

	/**
	 * [実行] オープン.
	 *		サーバをオープンし通信を開始します。
	 *	@param address ポートNO＆待ち受けるアドレス。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	bool Open(const CSocketAddress& address)
	{
		SetParameter(address);
		return Start();
	}

	/**
	 * [取得] 接続ソケット一覧と数を取得
	 *	@param[out]	P	ソケット一覧。NULLなら格納しません。
	 *	@return 接続ソケット数
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
	 * [確認] 指定ソケットの接続確認
	 *	@param so ソケット
	 *	@retval true	接続中
	 *	@retval false	切断中
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
	 * [実行] データ送信
	 *	@param so ソケット
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	bool Send(SOCKET so, size_t dataSize, LPCVOID lpcvData)
	{
		TTRACE1("CSocket[Server%d]:Send\n", so);
		_super::m_OnSend(dataSize, lpcvData);
		return m_Send(so, dataSize, lpcvData);
	}

	/**
	 * [取得] 指定ソケットの接続情報取得
	 *	@param[out]	_address アドレス&ポート
	 *	@param[in]	so ソケット
	 *	@retval true	アドレスとポート取得成功。
	 *	@retval false	失敗。
	 */
	bool GetPeerName(CSocketAddress& _address, SOCKET so)
	{
		return m_GetPeerName(_address, so);
	}

	/**
	 * [実行] 指定ソケットの切断
	 *	@param so Sソケット
	 */
	void Disconnect(SOCKET so)
	{
		m_CloseDataSocket(so);
	}

	/**
	 * [確認] Aliveチェック.
	 *		Open() メソッドが実行されている状態か、チェックします。
	 *	@retval true	開始している。
	 *	@retval	false	停止している。
	 */
	virtual bool IsAlive(void) const
	{
		return m_threadReceiver.IsAlive();
	}

	/**
	 * [確認] 接続チェック
	 *	@retval true	一つ以上接続中。
	 *	@retval	false	一つも接続していない。
	 */
	virtual bool IsConnect(void) const
	{
		return GetConnectedSockets() != 0; 
	}

	/**
	 * [実行] 全部に送信.
	 *		接続しているすべてに送信します。
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
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
	 * [実行] 全ソケットの切断.
	 *		接続しているすべてを一度切断します。すぐに接続待ちしますので、
	 *		すぐに接続される可能性も有ります。
	 */
	void Disconnect(void)
	{
		m_CloseAllDataSockets();
		m_OpenListenSocket();
	}

	/**
	 * [実行] サーバをクローズ
	 */
	virtual void Stop(void)
	{
		Close();
	}

	/**
	 * [実行] サーバをクローズ
	 */
	void Close(void)
	{
		TTRACE0("CSocket[Server]:サーバクローズ\n");
		m_CloseAllDataSockets();
		m_CloseListenSocket();
		m_threadReceiver.Stop();
	}

	/**
	 * [参照] 受信スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadReceiver;
	}

private:
	TNB::CThread	m_threadReceiver;	///< 受信スレッド
	SOCKET			m_soListen;			///< Listen用ソケット
	CSocketVector	m_vsoData;			///< 接続しているソケットの配列
	CSyncSection	m_syncSocket;		///< 同期オブジェクト
	size_t			m_connectMax;		///< 接続可能の数
	WORD			m_wOpenPortNo;		///< オープンポートNo
	CStr			m_strAwaitAddress;	///< 接続許可アドレス
	
	/**
	 * 全データSOCKETクローズ
	 *	@note 接続しているデータSOCKETをすべてクローズします
	 */
	void m_CloseAllDataSockets(void)
	{
		EXCLUSIVE(&m_syncSocket);
		size_t iLen = m_vsoData.GetSize();
		if ( iLen > 0 )
		{
			TTRACE0("CSocket[Server]:全クローズ\n");
			loop ( i, iLen )
			{
				if ( m_vsoData[i] != INVALID_SOCKET )
				{
					SOCKET so = m_vsoData[i];
					TTRACE1("CSocket[Server%d]:クローズ\n", so);
					m_OnSocketEvent(CSocketEvent(this, so, ReportEvent_Disconnect));
					::shutdown(so, SD_BOTH);
					::closesocket(so);
				}
			}
			m_vsoData.RemoveAll();
		}
	}

	/**
	 * 指定データソケットクローズ
	 *	@param so ソケット
	 */
	void m_CloseDataSocket(SOCKET so)
	{
		EXCLUSIVE(&m_syncSocket);
		loop ( i, m_vsoData.GetSize() )
		{
			if ( m_vsoData[i] == so )
			{
				TTRACE1("CSocket[Server%d]:クローズ\n", so);
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
	 * リスナーソケットオープン
	 *	@retval 0 成功
	 *	@retval 0以外 失敗。数値はエラーコード
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
			TTRACE0("CSocket[Server]:リスナーSOCKETオープン\n");
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

	/// リスナーソケットクローズ
	void m_CloseListenSocket(void)
	{
		if ( m_soListen != INVALID_SOCKET )
		{
			SOCKET so = m_soListen;
			m_soListen = INVALID_SOCKET;
			TTRACE0("CSocket[Server]:リスナーSOCKETクローズ\n");
			::shutdown(so, SD_BOTH);
			::closesocket(so);
		}
	}

	/** 
	 * スレッド処理本体
	 *	@retval ０固定。
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
						TTRACE1( "CSocket[Server%d]:接続依頼、拒否\n", so );
						m_CloseListenSocket();
						::shutdown(so, SD_BOTH);
						::closesocket(so);
					}
					else
					{
						TTRACE1("CSocket[Server%d]:接続\n", so);
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
							TTRACE1("CSocket[Server%d]:切断\n", so);
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
								TTRACE1("CSocket[Server%d]:切断\n", vsoBack[i]);
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
						TTRACE1("CSocket[Server%d]:切断(Recvによる)\n", vsoBack[i]);
						m_CloseDataSocket(vsoBack[i]);
						m_OpenListenSocket();
					}
					else
					{
						TTRACE1("CSocket[Server%d]:データ受信\n", vsoBack[i]);
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
