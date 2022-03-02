#pragma once
/**
 *@file
 * 通信関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbReportViewer.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * 通信インターフェース
 *
 *		送信、受信をサポートするインターフェースを提供します。		
 *
 *	@par必要ファイル
 *			TnbCommunication.h
 * 
 *	@date 07/09/13 新規作成
 *	@date 10/05/28 SetReceiveTimeout() を新規。
 */
struct ICommunication
{
	/**
	 * 状態
	 */
	enum EState
	{
		OPENNING,	///< オープン状態
		CONNECTING,	///< オープン＆接続状態
		CLOSED,		///< クローズ状態	
	};

	/**
	 * 性能プロパティ型.
	 */
	struct TProperty
	{
		CStr		typeName;				///< タイプ名。
		DWORD		typeId;					///< タイプID。
		DWORD		receiveTimeout;			///< 受信待ち時間。 INFINITE ならブロックされる。
		size_t		sendPacketSize;			///< 送信パケットサイズ。INVALID_SIZE なら制限なし。
		size_t		receivePacketSize;		///< 受信パケットサイズ。INVALID_SIZE なら制限なし。
		/// コンストラクタ
		TProperty(LPCTSTR lpszName, DWORD id, DWORD to = 0, size_t sendSize = INVALID_SIZE, size_t recvSize = INVALID_SIZE)
			: typeName(lpszName), typeId(id), receiveTimeout(to), sendPacketSize(sendSize), receivePacketSize(INVALID_SIZE)
		{}//デフォルトは、受信待ち時間０ms、送受信パケットサイズ、なし。
	};

	/// デストラクタ
	virtual ~ICommunication(void) {}

	/**
	 * [取得] 性能取得.
	 *	@note 正確な情報は Open後に取得できます。
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const = 0;

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@retval true 成功。
	 *	@retval false 失敗。サポートできない時もこれが返ります。
	 */
	virtual bool SetReceiveTimeout(DWORD to) = 0;

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void) = 0;

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void) = 0;

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	virtual bool IsOpened(void) const = 0;

	/**
	 * [確認] 接続確認.
	 *	@retval true 接続中。
	 *	@retval false 切断中。
	 */
	virtual bool IsConnect(void) const { return IsOpened(); }

	/**
	 * [確認] 送信可能確認.
	 *	@retval true 送信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanSend(void) const { return IsOpened(); }

	/**
	 * [確認] 受信可能確認.
	 *	@retval true 受信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanReceive(void) const { return IsOpened(); }

	/**
	 * [処理] 送信.
	 *	@param size サイズ。
	 *	@param P データ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 上記以外 送信データサイズ。
	 */
	virtual size_t Send(size_t size, LPCVOID P) = 0;

	/**
	 * [処理] 受信.
	 *	@param[in] size サイズ。
	 *	@param[out] _P データ。size 分のメモリは確保しておく必要があります。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 0 受信データなし。
	 *	@retval 上記以外 受信データサイズ。
	 */
	virtual size_t Receive(size_t size, LPVOID _P) = 0;

	/**
	 * [確認] 受信データ確認.
	 *	@note 本クラスでは不明を返します。
	 *	@retval INVALID_SIZE 不明。
	 *	@retval 上記以外 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const { return INVALID_SIZE; }

	/**
	 * [確認] チェック.
	 *		状態を確認します。
	 *	@note 本クラスでは IsOpened() , IsConnect() を使用します。
	 *	@retval OPENNING	オープン状態
	 *	@retval CONNECTING	オープン＆接続状態
	 *	@retval CLOSED		クローズ状態	
	 */
	virtual EState Check(void) const { return IsOpened() ? (IsConnect() ? CONNECTING : OPENNING) : CLOSED; }

	/**
	 * [処理] パージ.
	 *		通信のゴミを浄化
	 *	@note 本クラスではなにもしません。
	 */
	virtual void Purge(void) {}
};



#ifndef _WIN32_WCE

/**@ingroup COMMUNICATION
 * EVENT、Wait時間付き ReadFile関数
 *
 *		EventとWaitの付いた ::ReadFile です。
 *
 *	@deprecated 廃止予定です。
 *
 *	@attention タイムアウトした時、CancelIOするため、タイミングによっては、
 *				情報が欠落する可能性があります。連続性が必要なデータではこれを使用しないで、
 *				COverlapReader を使用します。
 *	@par必要ファイル
 *			TnbCommunication.h
 *	@date 07/09/13 新規作成
 *	@param[in] hFile ファイルハンドル。ReadFile出来るハンドルである必要があります。
 *	@param[in] hEvent イベント。NULL を指定することも出来ます。
 *	@param[in] wait 最大Wait時間(単位 ms)を指定します。
 *	@param[in] size 読込みサイズをしています。
 *	@param[out] _P 読み込むポインタをを指定します。
 *	@retval INVALID_SIZE エラー。 ::GetLastError() でエラー詳細がわかります。
 *	@retval 0 データ無し。
 *	@retval 上記以外 成功。数値は読込みサイズ。
 */
inline size_t BlockingReadFile(HANDLE hFile, HANDLE hEvent, DWORD wait, size_t size, LPVOID _P)
{
	OVERLAPPED overLapped = { 0 }; 
	HANDLE hMyEvent = NULL;
	if ( hEvent == NULL )
	{
		hMyEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
		::ResetEvent(hMyEvent);
		hEvent = hMyEvent;
	}
	overLapped.hEvent = hEvent; 
	DWORD dwRc = ULONG_MAX;
	DWORD work = ULONG_MAX;
	if ( ::ReadFile(hFile, _P, ToDword(size), &dwRc, &overLapped) )
	{
		// 成功
	}
	else if ( ::GetLastError() == ERROR_IO_PENDING )
	{
		// 非同期読み取り操作の結果をチェックする。
		::Sleep(0);
		if( ::GetOverlappedResult(hFile, &overLapped, &work, FALSE) )
		{
			dwRc = work; // 成功
		}
		else if ( ::GetLastError() == ERROR_IO_INCOMPLETE )
		{
			switch ( ::WaitForSingleObject(overLapped.hEvent, wait) )
			{
			case WAIT_OBJECT_0:
				if ( ::GetOverlappedResult(hFile, &overLapped, &work, FALSE) )
				{
					dwRc = work; // 成功
				}
				break;
			case WAIT_TIMEOUT:
				dwRc = 0;
				break;
			case WAIT_ABANDONED:
			case WAIT_FAILED:
			default:
				break;
			}
		}
	}
	::CancelIo(hFile);
	if ( hMyEvent != NULL )
	{
		::CloseHandle(hMyEvent);
	}
	return dwRc;
}

#else // _WIN32_WCE

inline size_t BlockingReadFile(HANDLE hFile, HANDLE hEvent, DWORD wait, size_t size, LPVOID _P)
{
	DWORD dwRc = ULONG_MAX;
	if ( ::ReadFile(hFile, _P, ToDword(size), &dwRc, NULL) )
	{
		return dwRc;
	}
	_GetLastError("ReadFile");
	return INVALID_SIZE;
}

#endif // _WIN32_WCE




}; // TNB