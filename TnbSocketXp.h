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
 *	@note	マクロ "_TnbWINSOCK2_ENABLE" の定義が必須です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _TnbWINSOCK2_ENABLE
 #error
#endif

#include "TnbSocket.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT SOCKET
 * XP以上用 Socketサポートクラス
 *	
 *		XP以上でサポートしている API を管理しています。
 * 
 *	@par必要ファイル
 *			TnbSocketXp.h
 * 
 *	@date 10/02/03	新規作成
 */
class CSocketXp
{
public:

	/// コンストラクタ
	CSocketXp(void) : m_isInit(true), m_lpfnTransmitPackets(NULL)
	{
	}

	/**
	 * [送信] 送信.
	 *	@param so ソケット
	 *	@param dataSize データサイズ
	 *	@param lpcvData データ
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	bool Send(SOCKET so, size_t dataSize, LPCVOID lpcvData)
	{
		if ( m_Init(so) )
		{
			TRANSMIT_PACKETS_ELEMENT packets;
			packets.dwElFlags = TP_ELEMENT_MEMORY;
			packets.cLength = dataSize;
			packets.pBuffer = const_cast<void* const>(lpcvData);
			return !! m_lpfnTransmitPackets(so, &packets, 1, 0, NULL, 0);
		}
		int r =  ::send(so, static_cast<const char*>(lpcvData), dataSize, 0);
		TRACE2("::send(%u) result = %d\n", dataSize, r);
		return r == ToInt(dataSize);
	}

private:
	bool					m_isInit;				///< 初期化フラグ
	LPFN_TRANSMITPACKETS	m_lpfnTransmitPackets;	///< 関数アドレス
	/// 初期化
	bool m_Init(SOCKET so)
	{
		if ( ! m_isInit )
		{
			m_isInit = true;
			DWORD dwBytes;
			GUID guidTransmitPackets = WSAID_TRANSMITPACKETS;
			::WSAIoctl(so, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidTransmitPackets, sizeof(GUID), &m_lpfnTransmitPackets, sizeof(LPVOID), &dwBytes, NULL, NULL);
		}
		return m_lpfnTransmitPackets != NULL;
	}
};



/**@ingroup REPORT SOCKET
 * Socketクライアントクラス
 *	
 *		クライアント用のソケットを用意します。
 *		定期的にServerを探す自動接続モードもあります。
 *
 *		受信は CAbstractSocket::IListener のサブクラスを登録し行います。
 *		( IReport::IListener でも受信は出来ますが基本機能しかありません)
 *
 *	@par必要ファイル
 *			TnbSocket.h
 * 
 *	@date 10/02/03	新規作成
 */
class CSocketClientXp : public CSocketClient
{
	DEFSUPER(CSocketClient);
protected:
	
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
		_super::m_OnSend(dataSize, lpcvData);
		if ( so == INVALID_SOCKET )
		{
			return false;
		}
		return m_xp.Send(so, dataSize, lpcvData);
	}
private:
	CSocketXp	m_xp;
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
 *	@par必要ファイル
 *			TnbSocket.h
 * 
 *	@date 10/02/03	新規作成
 */
class CSocketServerXp : public CSocketServer
{
	DEFSUPER(CSocketServer);
protected:
	
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
		_super::m_OnSend(dataSize, lpcvData);
		if ( so == INVALID_SOCKET )
		{
			return false;
		}
		return m_xp.Send(so, dataSize, lpcvData);
	}
private:
	CSocketXp	m_xp;
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #define CSocketClient CSocketClientXp
 #define CSocketServer CSocketServerXp
#endif



};