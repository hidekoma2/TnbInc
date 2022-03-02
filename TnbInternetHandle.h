#pragma once
/**
 *@file
 * INTERNETハンドル関係のヘッダ
 *
 *		WININETの APIを ラップしたクラス。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDynamicFunc.h"
#include "TnbPointerHandle.h"
#include "TnbStrVector.h"



#include <Wininet.h>
#pragma comment(lib,"Wininet.lib")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 /// PointerHandleBase用Destoryクラス
 struct TPhInternetCloseHandle
 {
	void operator()(HINTERNET P) { ::InternetCloseHandle(P); }
 };
#endif



/**@ingroup INTERNET
 * HINTERNET型ハンドルハンドラ―クラス
 *
 *		この型に ::InternetOpen(),::InternetOpenUrl() で破棄するハンドルを渡しておくと、
 *		「領域」を参照する本クラスががなくなり次第、
 *		自動的に ::InternetCloseHandle() します。
 *
 *	@par必要ファイル
 *			TnbInternetHandle.h
 */
typedef TNB::CPointerHandleBaseT<HINTERNET, TPhInternetCloseHandle> CInternetHandleHandle;



/**@ingroup INTERNET
 * INTERNETハンドル管理クラス
 *
 *	HINTERNETにかかわるAPIをラップしただけのクラスです。
 *
 *	@par必要ファイル
 *			TnbInternetHandle.h
 *
 *	@date 06/10/03 新規
 *	@date 19/01/25 メソッド追加.
 */
class CInternetHandle
{
	CInternetHandleHandle m_handle;	///< HINTERNETハンドラ

public:

	/**
	 * コンストラクタ
	 *	@param h ハンドル
	 */
	CInternetHandle(HINTERNET h = NULL) : m_handle(h) 
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CInternetHandle(const CInternetHandle& other) : m_handle(other.m_handle) 
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CInternetHandle& operator=(const CInternetHandle& other)
	{
		m_handle = other.m_handle;
		return *this;
	}

	/**
	 * [設定] クローズ.
	 *		保持しているハンドルをクローズします。
	 *	@note コピーなどしてハンドルを共有している場合、クローズしません。
	 */
	void Close(void)
	{
		m_handle.Null();
	}

	/**
	 * [確認] ハンドル有効確認
	 *	@retval ture 有効。
	 *	@retval false 無効(開いていないか、失敗したか)。
	 */
	bool IsValid(void) const
	{
		return ! m_handle.IsNull();
	}

	/**
	 * [設定] 初期化.
	 *		アプリケーションが使用するWININETの初期化を行い、WININET呼び出し用のハンドルを作成する。 
	 *	@param lpszAgent		HTTPによりインターネットにアクセスする際のエージェント名。
	 *	@param dwAccessType		アクセス方法を指定するフラグ。
	 *					<BR>	INTERNET_OPEN_TYPE_DIRECT		全てのホスト名をローカルに解決する。
	 *					<BR>	INTERNET_OPEN_TYPE_PROXY		プロクシバイパスリストが与えられていない場合やプロクシを通さずに名前解決が行われる場合を除いて、要求はプロクシに渡す。要求がプロクシに渡されないケースの場合は、本関数はINTERNET_OPEN_TYPE_DIRECTとして動作する。
	 *					<BR>	INTERNET_OPEN_TYPE_PRECONFIG	レジストリに保持されている設定を利用する。
	 *	@param lpszProxy		プロクシによるアクセスが指定されている場合に、プロクシサーバ名を指定する。NULLの場合、レジストリからプロクシ情報が取得される。プロトコルごとに異なるプロクシを指定したい場合などの指定についてはSDKのドキュメントを参照。
	 *	@param lpszProxyBypass	(オプションの)ローカルでの既知のホスト名またはIPアドレスのリスト。これらのホストに対する要求はプロクシを通さずに行われる。このリストにはワイルドカードを含めることができる。
	 *	@param dwFlags			Win32インターネット関数の振る舞いを指定するフラグ。後述の表を参照。
	 *					<BR>	INTERNET_FLAG_OFFLINE	キャッシュからのみデータ取得が行われる。
	 *					<BR>	INTERNET_FLAG_ASYNC	非同期で処理が行われる。
	 *	@retval true 成功。本クラスのハンドルが有効になる。
	 *	@retval false 失敗。本クラスのハンドルは無効になる。
	 */
	bool Open(LPCTSTR lpszAgent, DWORD dwAccessType, LPCTSTR lpszProxy = NULL, 
				LPCTSTR lpszProxyBypass = NULL, DWORD dwFlags = 0)
	{
		m_handle = ::InternetOpen(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
		return ! m_handle.IsNull();
	}

	/**
	 * [接続] 指定名サーバ接続.
	 *		ServerNameで指定されたインターネット上のサーバに接続し、ハンドルを返す。 
	 *	@note	::InternetOpen() で返されたハンドルを使います。
	 *	@param lpszServerName	接続先のホスト名またはドット表記のIPアドレス。
	 *	@param tPort	接続するTCP/IPポート。既定の値については下表を参照。また、0を指定した場合、デフォルトのポートが使用される。
	 *					<BR>	INTERNET_DEFAULT_FTP_PORT		FTP
	 *					<BR>	INTERNET_DEFAULT_GOPHER_PORT	Gopher
	 *					<BR>	INTERNET_DEFAULT_HTTP_PORT		HTTP
	 *					<BR>	INTERNET_DEFAULT_HTTPS_PORT		HTTPS
	 *	@param lpszUserName	接続の際に使用されるユーザ名。HTTP以外の場合は、NULLを指定した場合デフォルト(FTPなら"anonymous")が使用される。
	 *	@param lpszPassword	接続の際に使用されるパスワード。FTPでUserNameとPasswordが共にNULLの場合、Passwordはe-mailアドレスが使用される。
	 *	@param dwService	アクセスするサービスのタイプ。
	 *					<BR>	INTERNET_SERVICE_FTP			FTP
	 *					<BR>	INTERNET_SERVICE_GOPHER			Gopher
	 *					<BR>	INTERNET_SERVICE_HTTP			HTTP
	 *	@param dwFlags	サービス固有のフラグ値。ServiceがINTERNET_SERVICE_FTPの場合、Flagsには以下のいずれかの値が指定可能。 
	 *					<BR>	INTERNET_CONNECT_FLAG_PASSIVE	パッシブモードで接続する。
	 *	@param dwContext	コールバック関数に渡されるアプリケーション定義の32bit値。
	 *	@return インターネットハンドル。 IsValid() で合否判定する。
	 */
	CInternetHandle Connect(LPCTSTR lpszServerName, INTERNET_PORT tPort, LPCTSTR lpszUserName,
			LPCTSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::InternetConnect(m_handle, lpszServerName, tPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);	
	}

	/**
	 * [接続] 指定URL接続.
	 *		指定されたURLをオープンし、接続を確立し、データ読み込みの準備を行い、ハンドルを返す。 
	 *	@note	::InternetOpen() で返されたハンドルを使います。
	 *	@param lpszUrl			オープンするURL。
	 *	@param lpszHeaders		HTTPサーバに送るヘッダ。NULLを指定可能。
	 *	@param dwHeadersLength	Headersの文字数。-1を指定した場合、自動的に計算される。
	 *	@param dwFlags	オープンの種別を指定するビットマスク値。後述の表を参照。
	 *					<BR>	INTERNET_FLAG_RELOAD	ローカルのキャッシュを無視し、常にサーバからデータを取得する。
	 *					<BR>	INTERNET_FLAG_DONT_CACHE	ローカル及びゲートウェイにデータをキャッシュしない。
	 *					<BR>	INTERNET_FLAG_RAW_DATA	生のデータを返す(FTPの場合WIN32_FIND_DATA、Gopherの場合GOPHER_FIND_DATA)。このフラグが指定されていない場合、ディレクトリの一覧をHTMLフォーマットで返却する。(->)
	 *					<BR>	INTERNET_FLAG_SECURE	SSLやPCTを使用する。(HTTPのみ)
	 *					<BR>	INTERNET_FLAG_EXISTING_CONNECT	可能な限り既存のサーバとの接続を再利用する。
	 *	@param 	dwContext	コールバック関数に渡すアプリケーション定義の32bit値。
	 *	@return インターネットハンドル。 IsValid() で合否判定する。
	 */
	CInternetHandle OpenUrl(LPCTSTR lpszUrl, LPCTSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::InternetOpenUrl(m_handle, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
	}

	/**
	 * [作成] HTTPリクエストの作成.
	 *	@param lpszVerb 要求に使用する動詞。NULLの場合、GETとみなされる。
	 *	@param lpszObjectName 動詞の対称となるオブジェクトの名前。
	 *	@param lpszVersion HTTPのバージョン。NULLの場合、HTTP/1.0が使用される。
	 *	@param lpszReferrer ObjectNameで指定されたオブジェクトの位置(URL)。NULLを指定可能。
	 *	@param dwFlags 要求に関するフラグ。
	 *					<BR>	INTERNET_FLAG_RELOAD	ローカルのキャッシュを無視し、常にサーバからデータを取得する。
	 *					<BR>	INTERNET_FLAG_DONT_CACHE	ローカル及びゲートウェイにデータをキャッシュしない。
	 *					<BR>	INTERNET_FLAG_RAW_DATA	生のデータを返す(FTPの場合WIN32_FIND_DATA、Gopherの場合GOPHER_FIND_DATA)。このフラグが指定されていない場合、ディレクトリの一覧をHTMLフォーマットで返却する。(->)
	 *					<BR>	INTERNET_FLAG_SECURE	SSLやPCTを使用する。(HTTPのみ)
	 *					<BR>	INTERNET_FLAG_EXISTING_CONNECT	可能な限り既存のサーバとの接続を再利用する。
	 *	@param dwContext コールバック関数に渡すアプリケーション定義の32bit値。
	 *	@return インターネットハンドル。 IsValid() で合否判定する。
	 */
	CInternetHandle HttpOpenRequest(LPCTSTR lpszVerb, LPCTSTR lpszObjectName, LPCTSTR lpszVersion, LPCTSTR lpszReferrer, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::HttpOpenRequest(m_handle, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, NULL, dwFlags, dwContext);

    }

	/**
	 * [作成] HTTPリクエストの作成.
	 *	@param lpszVerb 要求に使用する動詞。NULLの場合、GETとみなされる。
	 *	@param lpszObjectName 動詞の対称となるオブジェクトの名前。
	 *	@param lpszVersion HTTPのバージョン。NULLの場合、HTTP/1.0が使用される。
	 *	@param lpszReferrer ObjectNameで指定されたオブジェクトの位置(URL)。NULLを指定可能。
	 *	@param acceptTypes メディアのタイプを示す文字列の配列。
	 *	@param dwFlags 要求に関するフラグ。
	 *					<BR>	INTERNET_FLAG_RELOAD	ローカルのキャッシュを無視し、常にサーバからデータを取得する。
	 *					<BR>	INTERNET_FLAG_DONT_CACHE	ローカル及びゲートウェイにデータをキャッシュしない。
	 *					<BR>	INTERNET_FLAG_RAW_DATA	生のデータを返す(FTPの場合WIN32_FIND_DATA、Gopherの場合GOPHER_FIND_DATA)。このフラグが指定されていない場合、ディレクトリの一覧をHTMLフォーマットで返却する。(->)
	 *					<BR>	INTERNET_FLAG_SECURE	SSLやPCTを使用する。(HTTPのみ)
	 *					<BR>	INTERNET_FLAG_EXISTING_CONNECT	可能な限り既存のサーバとの接続を再利用する。
	 *	@param dwContext コールバック関数に渡すアプリケーション定義の32bit値。
	 *	@return インターネットハンドル。 IsValid() で合否判定する。
	 */
	CInternetHandle HttpOpenRequest(LPCTSTR lpszVerb, LPCTSTR lpszObjectName, LPCTSTR lpszVersion, LPCTSTR lpszReferrer, const CStrVector& acceptTypes, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		CWorkMemT<LPCTSTR> a(acceptTypes.GetSize() + 1);
		loop ( i, acceptTypes )
		{
			a[i] = acceptTypes.ReferBuffer()->ReferBuffer();
		}
		a[acceptTypes.GetSize()] = NULL;
		return ::HttpOpenRequest(m_handle, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, a, dwFlags, dwContext);
    }

	/**
	 * [送信] HTTPリクエストを送信.
	 *	@param lpszHeaders 追加のヘッダ。NULLを指定可能。
	 *	@param dwHeadersLength Headersのサイズ。-1を指定した場合、自動的に計算される。
	 *	@param lpOptional 要求ヘッダ後に直接送信されるオプションのデータ。通常POSTやPUTで使用される。NULLを指定可能。
	 *	@param dwOptionalLength Optionalのサイズ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool HttpSendRequest(LPCTSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
	{
		return !! ::HttpSendRequest(m_handle, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	}

	/**
	 * [取得] HTTPリクエストに関連する情報の取得.
	 *	@param dwInfoLevel 	取得する属性や、要求の変更などのレベルを表すフラグ。
	 *	@param lpBuffer 取得結果。
	 *	@param lpdwBufferLength Bufのサイズ。関数の実行後実際のサイズまたは必要なサイズが返される。
	 *	@param lpdwIndex 同一のヘッダが存在する場合にどのヘッダかを識別するためのインデックス。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool HttpQueryInfo(DWORD dwInfoLevel, LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex)
	{
		return !! ::HttpQueryInfo(m_handle, dwInfoLevel, lpBuffer, lpdwBufferLength, lpdwIndex);
	}

	/**
	 * [取得] オプション情報取得.
	 *		指定されたハンドルに対するインターネットのオプション情報を取得する。 
	 *	@note 情報の取得元のインターネットのハンドル。
	 *	@param[in] dwOption	どの情報を取得するかを識別するフラグ。
	 *					<BR>	INTERNET_OPTION_CALLBACK			hInternetに関連付けられているコールバック関数へのポインタが返される。
	 *					<BR>	INTERNET_OPTION_CONNECT_TIMEOUT		接続要求のタイムアウト時間(ミリ秒)が返される。
	 *					<BR>	INTERNET_OPTION_CONNECT_RETRIES		接続の最大リトライ回数。
	 *					<BR>	INTERNET_OPTION_CONNECT_BACKOFF		リトライまでに一定時間待機する際の待機時間。現バージョンでは未実装。
	 *					<BR>	INTERNET_OPTION_CONTROL_SEND_TIMEOUT	非データの送信要求のタイムアウト時間(ミリ秒)。現バージョンではFTPのみ有効。
	 *					<BR>	INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT	非データの受信要求のタイムアウト時間(ミリ秒)。現バージョンではFTPのみ有効。
	 *					<BR>	INTERNET_OPTION_DATA_SEND_TIMEOUT	データの送信要求のタイムアウト時間(ミリ秒)。
	 *					<BR>	INTERNET_OPTION_DATA_RECEIVE_TIMEOUT	データの受信要求のタイムアウト時間(ミリ秒)。
	 *					<BR>	INTERNET_OPTION_HANDLE_TYPE			ハンドルのタイプ。
	 *					<BR>	INTERNET_OPTION_CONTEXT_VALUE		hInternetに関連付けられているContextの値。
	 *					<BR>	INTERNET_OPTION_READ_BUFFER_SIZE	読み込み用バッファサイズ。
	 *					<BR>	INTERNET_OPTION_WRITE_BUFFER_SIZE	書き込み用バッファサイズ。
	 *					<BR>	INTERNET_OPTION_ASYNC_PRIORITY		hInternetによる非同期ダウンロードにおける優先度。
	 *					<BR>	INTERNET_OPTION_PARENT_HANDLE		親ハンドル。
	 *					<BR>	INTERNET_OPTION_KEEP_CONNECTION		永続的接続を使用するかどうかを表すフラグ。
	 *					<BR>	INTERNET_OPTION_USERNAME			ユーザ名。
	 *					<BR>	INTERNET_OPTION_PASSWORD			パスワード。
	 *					<BR>	INTERNET_OPTION_REQUEST_FLAGS		現在のダウンロードに関する状態フラグ。
	 *					<BR>	INTERNET_OPTION_EXTENDED_ERROR		WinSockのエラーコード。
	 *					<BR>	INTERNET_OPTION_SECURITY_CERTIFICATE_STRUCT	SSL/PCT用の証明書を表すINTERNET_CERTIFICATE_INFO構造体のデータ。
	 *					<BR>	INTERNET_OPTION_SECURITY_CERTIFICATE	SSL/PCT用の証明書を表す文字列。
	 *					<BR>	INTERNET_OPTION_CACHE_STREAM_HANDLE	暗号用鍵のサイズ。
	 *					<BR>	INTERNET_OPTION_SECURITY_FALGS		セキュリティに関するビットマスク値。
	 *					<BR>	INTERNET_OPTION_DATAFILE_NAME		ダウンロード時のファイル名。
	 *					<BR>	INTERNET_OPTION_URL					ダウンロード時のリソースのURL名。
	 *					<BR>	INTERNET_OPTION_REFRESH				レジストリから情報を再読み込み可能かどうか。
	 *					<BR>	INTERNET_OPTION_PROXY				プロクシ情報を表すINTERNET_PROXY_INFO構造体のデータ。
	 *					<BR>	INTERNET_OPTION_VERSION				WININETのバージョンを表すINTERNET_VERSION_INFO構造体のデータ。
	 *					<BR>	INTERNET_OPTION_USER_AGENT			エージェント名。
	 *	@param[out] _lpBuffer	取得結果の情報を格納するバッファ。
	 *	@param[in,out] _lpdwBufferLength _lpBufferのサイズ。関数の実行後、設定されたデータのサイズまたは必要なサイズが返される。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool QueryOption(DWORD dwOption, LPVOID _lpBuffer, LPDWORD _lpdwBufferLength) const
	{
		return !! ::InternetQueryOption(m_handle, dwOption, _lpBuffer, _lpdwBufferLength);
	}

	/**
	 * [取得] ファイル読み込み.
	 *		指定されたインターネット上のファイルからデータを読み込み、Bufに返す。 
	 *	@note ::InternetOpenUrl()、::FtpOpenFile()、::GopherOpenFile()、::HttpOpenRequest() のいずれかで返されたハンドルを使用。
	 *	@note 読み込みがファイルの終端まで達した場合、戻り値は true が返され、lpdwNumberOfBytesRead にはゼロが設定される。 
	 *	@param[out] _lpBuffer	読み込み結果のデータ。
	 *	@param[in]	dwNumberOfBytesToRead	lpBufferのサイズ。
	 *	@param[out]	_lpdwNumberOfBytesRead	実際に読み込まれたデータのサイズ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool ReadFile(LPVOID _lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD _lpdwNumberOfBytesRead) const
	{
		return !! ::InternetReadFile(m_handle, _lpBuffer, dwNumberOfBytesToRead, _lpdwNumberOfBytesRead);
	}

	/**
	 * [設定] ファイルポインタ設定.
	 *		InternetReadFileでのファイル読み込み位置を設定する。 
	 *	@note ::InternetOpenUrl()、::HttpOpenRequest(GETまたはHEADを使用しHttpSendRequestに渡したとき)で返されたハンドルを使用。
	 *	@note ::FtpOpenFile()、::GopherOpenFile() のハンドルでは、本メソッドは失敗します。
	 *	@param lDistanceToMove	移動量。
	 *	@param pReserved ダミー。 NULL を指定。
	 *	@param dwMoveMethod	移動方法を表す真偽値。
	 *					<BR>	FILE_BEGIN	Distanceはファイルの先頭からの位置を表す。
	 *					<BR>	FILE_CURRENT	Distanceは現在のファイル位置からの相対位置を表す。
	 *					<BR>	FILE_END	Distanceはファイルの終端からの位置を表す。現在のファイルサイズが不明の場合、このフラグは失敗する。
	 *	@param dwContext	0でなければならない。
	 *	@retval INVALID_SET_FILE_POINTER 失敗。
	 *	@retval それ以外 成功（値は移動後のファイルポインタ）。
	 */
	DWORD SetFilePointer(LONG lDistanceToMove, LONG* pReserved, DWORD dwMoveMethod, DWORD_PTR dwContext = 0) const
	{
		return ::InternetSetFilePointer(m_handle, lDistanceToMove, pReserved, dwMoveMethod, dwContext);
	}

	/**
	 * [設定] オプション情報設定
	 *	@param dwOption	設定する情報の種別。
	 *					<BR>	INTERNET_OPTION_CALLBACK			コールバック関数のアドレスを設定する。
	 *					<BR>	INTERNET_OPTION_CONNECT_TIMEOUT		インターネットへの接続のタイムアウト時間を設定する。
	 *					<BR>	INTERNET_OPTION_CONNECT_RETRIES		インターネットへの接続のリトライ回数。
	 *					<BR>	INTERNET_OPTION_CONNECT_BACKOFF		リトライ時の待機時間。
	 *					<BR>	INTERNET_OPTION_CONTROL_SEND_TIMEOUT	非データの送信要求のタイムアウト時間。現バージョンではFTPのみ有功。
	 *					<BR>	INTERNET_OPTION_CONTROL_RECIEVE_TIMEOUT	非データの受信要求のタイムアウト時間。現バージョンではFTPのみ有効。
	 *					<BR>	INTERNET_OPTION_DATA_SEND_TIMEOUT	データの送信要求のタイムアウト時間。
	 *					<BR>	INTERNET_OPTION_DATA_RECEIVE_TIMEOUT	データの受信要求のタイムアウト時間。
	 *					<BR>	INTERNET_OPTION_ASYNC_PRIORITY		非同期ダウンロード時の優先度。現バージョンでは未実装。
	 *					<BR>	INTERNET_OPTION_CONTEXT_VALUE		hInternetに関連付けられたContextの値。
	 *					<BR>	INTERNET_OPTION_REFRESH				レジストリから情報を再読み込み可能かどうか。
	 *					<BR>	INTERNET_OPTION_PROXY				プロクシ情報を表すINTERNET_PROXY_INFO構造体。
	 *					<BR>	INTERNET_OPTION_USER_AGENT			エージェント名。
	 *					<BR>	INTERNET_OPTION_USERNAME			ユーザ名。
	 *					<BR>	INTERNET_OPTION_PASSWORD			パスワード。
	 *					<BR>	INTERNET_READ_BUFFER_SIZE			読み込みバッファ長。
	 *					<BR>	INTERNET_WRITE_BUFFER_SIZE			FtpPutFileなどで使用される書き込みバッファ長。
	 *					<BR>	INTERNET_OPTION_WRITE_DATA			データ書き込み時に使用されるバッファのサイズ。
	 *	@param lpBuffer	設定する情報。
	 *	@param dwBufferLength	lpBuffer のサイズ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetOption(DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength)
	{
		return !! ::InternetSetOption(m_handle, dwOption, lpBuffer, dwBufferLength);
	}

	/**
	 * [設定] コールバック設定.
	 *		hInternetで識別されるインターネットの使用時における各種イベントの通知などに
	 *		使用するコールバック関数を設定する。 
	 *	@see http://nienie.com/~masapico/api_InternetSetStatusCallback.html
	 *	@param	pInetProc	設定するコールバック関数。
	 *	@retval NULL 失敗。
	 *	@retval 本関数実行前に設定されていたコールバック関数へのポインタ
	*/
	INTERNET_STATUS_CALLBACK SetStatusCallback(INTERNET_STATUS_CALLBACK pInetProc)
	{
		return ::InternetSetStatusCallback(m_handle, pInetProc);
	}

	/**
	 * [検索] 検索続行.
	 *		::FtpFindFirstFile() または ::GopherFindFirstFile() で開始されたファイル検索を続行し、
	 *		次のファイルを取得する。 
	 *	@note ::FtpFindFirstFile() または ::GopherFindFirstFile() で返された検索ハンドルを使用。
	 *	@param pFindData 検索結果。FTPの場合WIN32_FIND_DATAで、Gopherの場合GOPHER_FIND_DATAである。
	 *	@retval true 成功。
	 *	@retval false 失敗。ただし、ファイルの検索を完了した場合、エラーとしてERROR_NO_MORE_FILESが設定される
	*/
	bool FindNextFile(LPVOID pFindData) const
	{
		return !! ::InternetFindNextFile(m_handle, pFindData);
	}

	/**
	 * [出力] ファイル書き込み.
	 *		インターネット上のファイルに対して、指定されたデータを書き込む。 
	 *	@note ::FtpOpenFile() または ::HttpSendRequestEx() で返された、書き込み先を識別するハンドル。
	 *	@param lpBuffer	書き込むデータ。
	 *	@param dwNumberOfBytesToWrite	書き込むバイト数。
	 *	@param lpdwNumberOfBytesWritten	実際に書き込まれたデータのサイズ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	*/
	bool WriteFile(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten)
	{
		return !! ::InternetWriteFile(m_handle, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
	}
	
	/**
	 * [作成] 指定ディレクトリ作成.
	 *		FTPによって、DirNameで指定したディレクトリをリモートに作成する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param lpszDirName	作成するディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool CreateDirectory(LPCTSTR lpszDirName)
	{
		return !! ::FtpCreateDirectory(m_handle, lpszDirName);
	}

	/**
	 * [削除] 指定ファイル削除.
	 *		FileNameで指定されたリモートのファイルを削除する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param lpszFileName	削除するファイルのファイル名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool DeleteFile(LPCTSTR lpszFileName)
	{
		return !! ::FtpDeleteFile(m_handle, lpszFileName);
	}

	/**
	 * [検索] 検索開始.
	 *		hFtpSessionで指定されたFTPセッションにおいて、ファイルおよびディレクトリエントリの検索を開始する。 
	 *	@note FtpFindFirstFileは一つのFTPセッションにつき一度だけ実行可能である。 
	 *	@note 確立されているFTPセッションのハンドルを使用。。
	 *	@param lpszSearchFile	検索するディレクトリパス名またはファイル名。NULLを指定した場合、サーバ上のカレントディレクトリの最初のファイルが本関数で検索される。
	 *	@param lpFindFileData	検索結果。
	 *	@param dwFlags	フラグ。 OpenUrl() を参照。
	 *	@param dwContext コールバック関数に渡すアプリケーション定義の32bit値。
	 *	@return 検索の続行に使用する列挙ハンドル。失敗すると、不正な状態のハンドルが返ります。
	 */
	CInternetHandle FindFirstFile(LPCTSTR lpszSearchFile, LPWIN32_FIND_DATA lpFindFileData, DWORD dwFlags, DWORD_PTR dwContext = 0) const
	{
		return ::FtpFindFirstFile(m_handle, lpszSearchFile, lpFindFileData, dwFlags, dwContext);
	}

	/**
	 * [取得] カレントディレクトリ取得.
	 *		FTPセッション中でのカレントディレクトリを取得する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param[out]		_lpszCurrentDirectory	カレントディレクトリ。
	 *	@param[in,out]	_lpdwLength		_lpszCurrentDirectoryのサイズ。関数の実行後、CurDirに設定されたデータのサイズまたは必要なサイズが返される。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GetCurrentDirectory(LPTSTR _lpszCurrentDirectory, LPDWORD _lpdwLength) const
	{
		return !! ::FtpGetCurrentDirectory(m_handle, _lpszCurrentDirectory, _lpdwLength);
	}

	/**
	 * [取得] ローカルへ読込み.
	 *		FTPセッションにおいて、ファイルをリモートからローカルへ転送する。 
	 *	@note 	FTPセッションのハンドルを使用。
	 *	@param lpszRemoteFile	取得元のファイル名。
	 *	@param lpszLocalFile	転送先のファイル名。
	 *	@param fFailIfExists	LocalFileNameで指定されたファイルが既にローカルに存在するときに、エラーにするか否かを表す真偽値。
	 *	@param 	dwFlagsAndAttributes	転送結果のファイルに付与する属性。FILE_ATTRIBUTE_*が指定可能。詳細はCreateFileを参照。
	 *	@param 	dwFlags	ファイルの取得の方法を指定するフラグ。後述の表を参照。
	 *					<BR>		まず次のいずれかのフラグを指定する。
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	表現形式をASCIIとして転送を行う。
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	表現形式をIMAGEとして転送を行う。
	 *					<BR>		FTP_TRANSFER_TYPE_UNKNOWN	デフォルトの転送方法を使用する。(バイナリ転送)
	 *					<BR>		INTERNET_FLAGS_TRANSFER_ASCII	ASCIIファイルを転送する。
	 *					<BR>		INTERNET_FLAGS_TRANSFER_BINARY	バイナリファイルを転送する。
	 *					<BR>
	 *					<BR>		次に、以下のフラグの組み合わせを論理和で指定する。 
	 *					<BR>		INTERNET_FLAG_DONT_CACHE	
	 *					<BR>		INTERNET_FLAG_HYPERLINK	
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	
	 *					<BR>		INTERNET_FLAG_MUST_CACHE_REQUEST	
	 *					<BR>		INTERNET_FLAG_NEED_FILE	
	 *					<BR>		INTERNET_FLAG_NO_CACHE_WRITE	
	 *					<BR>		INTERNET_FLAG_RELOAD	
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	
	 *	@param 	dwContext	コールバック関数へ渡す32bit値。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GetFile(LPCTSTR lpszRemoteFile, LPCTSTR lpszLocalFile, BOOL fFailIfExists, 
				DWORD dwFlagsAndAttributes, DWORD dwFlags, DWORD_PTR dwContext = 0) const
	{
		return !! ::FtpGetFile(m_handle, lpszRemoteFile, lpszLocalFile, fFailIfExists, dwFlagsAndAttributes, dwFlags, dwContext);
	}

	/**
	 * [取得] ファイルサイズ取得.
	 *		FTPセッションにおいて、ファイルサイズを取得する。 
	 *	@note 	FTPセッションのハンドルを使用。
	 *	@param[out] _lpdwFileSizeHigh サイズの上位LONG。
	 *	@return サイズの下位LONG
	 */
	DWORD GetFileSize(LPDWORD _lpdwFileSizeHigh) const
	{
		typedef DWORD (WINAPI* P)(HINTERNET, LPDWORD);
		try
		{
			return CDynamicFuncT<P>("Wininet.dll", "FtpGetFileSize")()(m_handle, _lpdwFileSizeHigh);
		}
		catch(CNullPointerException& e)
		{
			ASSERT0(false, "CInternetHandle::GetFileSize()", 
				"API FtpGetFileSize()が見つかりません。\nWininet.dll が古い可能性があります。");
			e.OnCatch();
		}
		return 0;
	}

	/**
	 * [取得] ファイルオープン.
	 *		FTPセッションでFileNameで指定されたファイルをオープンする。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param lpszFileName	オープンするファイルの名前。
	 *	@param dwAccess	オープン時のアクセスの種別。後述の表を参照。
	 *					<BR>		GENERIC_READ	読み取りアクセスで開く。
	 *					<BR>		GENERIC_WRITE	書き込みアクセスで開く。
	 *	@param dwFlags	ファイル転送の種別を表すフラグ。後述の表を参照。
	 *					<BR>		まず以下のいずれかを指定する。 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII転送。
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	バイナリ転送。
	 *					<BR>		FTP_TRANSFER_TYPE_UNKNOWN	デフォルトではFTP_TRANSFER_TYPE_BINARY。
	 *					<BR>
	 *					<BR>		次に、以下の任意の組み合わせを論理和によって指定する。 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	再読み込みすべきかどうかの判断時に有効期限・最終更新時刻がサーバから返されない場合強制的に再読み込みする。
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	キャッシュに永続的要素として追加する。これにより、通常のキャッシュのクリーンアップ、整合性チェック、ごみは行きなどの際にこの要素が削除されない。
	 *					<BR>		INTERNET_FLAG_RELOAD	要求したファイル、オブジェクト、ディレクトリリストをキャッシュから取得するのでなく、強制的にサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTPリソースをサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_NEED_FILE	ファイルがキャッシュできない場合に一時ファイルを作成する。
	 *	@param dwContext	コールバックに渡すアプリケーション定義の値。
	 *	@return インターネットハンドル。 IsValid() で合否判定する。
	*/
	CInternetHandle OpenFile(LPCTSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::FtpOpenFile(m_handle, lpszFileName, dwAccess, dwFlags, dwContext);
	}

	/**
	 * [送信] ローカルからファイル送信.
	 *		FTPセッションで、LocalFileNameで指定されたローカルのファイルをリモートへ転送する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param lpszLocalFile	転送元のファイル名。
	 *	@param lpszNewRemoteFile	転送先のファイル名。
	 *	@param dwFlags	転送方法を指定するフラグ。後述の表を参照。
	 *					<BR>		以下のフラグの組み合わせである。 
	 *					<BR>		INTERNET_FLAG_DONT_CACHE	
	 *					<BR>		INTERNET_FLAG_HYPERLINK	
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	
	 *					<BR>		INTERNET_FLAG_MUST_CACHE_REQUEST	
	 *					<BR>		INTERNET_FLAG_NEED_FILE	
	 *					<BR>		INTERNET_FLAG_NO_CACHE_WRITE	
	 *					<BR>		INTERNET_FLAG_RELOAD	
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	
	 *					<BR>		INTERNET_FLAG_TRANSFER_ASCII	
	 *					<BR>		INTERNET_FLAG_TRANSFER_BINARY	
	 *	@param dwContext	アプリケーション定義のコールバック関数に渡す32bit値。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool PutFile(LPCTSTR lpszLocalFile, LPCTSTR lpszNewRemoteFile, DWORD dwFlags, DWORD_PTR dwContext)
	{
		return !! ::FtpPutFile(m_handle, lpszLocalFile, lpszNewRemoteFile, dwFlags, dwContext);
	}

	/**
	 * [削除] ディレクトリ削除.
	 *		指定されたFTPセッションで、DirNameで指定されたリモートのディレクトリを削除する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param lpszDirName	削除するディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool RemoveDirectory(LPCTSTR lpszDirName)
	{
		return !! ::FtpRemoveDirectory(m_handle, lpszDirName);
	}

	/**
	 * [変更] 名前変更.
	 *		指定したFTPセッションにおいて、OldFileNameで指定されたリモートのファイルの名前をNewFileNameで指定された名前に変更する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param lpszOldFileName	元のファイル名。
	 *	@param lpszNewFileName	新規ファイル名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	*/
	bool RenameFile(LPCTSTR lpszOldFileName, LPCTSTR lpszNewFileName)
	{
		return !! ::FtpRenameFile(m_handle, lpszOldFileName, lpszNewFileName);
	}

	/**
	 * [設定] カレントディレクトリ設定.
	 *		指定したFTPセッションで、リモートのカレントディレクトリをDirNameで指定したディレクトリに変更する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param lpszDirName	変更先のディレクトリ名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetCurrentDirectory(LPCTSTR lpszDirName)
	{
		return !! ::FtpSetCurrentDirectory(m_handle, lpszDirName);
	}

	/**
	 * [調査] インターネット利用可能調査.
	 *		インターネットへの接続を試みることにより、現在の環境がインターネットが利用可能な環境かどうかを調査する。 
	 *	@retval true 接続可能。
	 *	@retval false 不可能。
	 */
	static bool AttemptConnect(void)
	{
		return ::InternetAttemptConnect(0) == ERROR_SUCCESS;
	}

	/**
	 * [変換] URL変換.
	 *		Flagsで指定された変換方法にしたがって変換します。
	 *	@todo APIの動きが良くわからない。使用には注意。
	 *
	 *	@param[out]		_lpszDstUrl 変換後のURL。
	 *	@param[in,out]	_dwDstSize	_lpszDstUrlのサイズ。関数の実行後、
	 *					実際に設定されたデータのサイズまたは必要なサイズが返される。。
	 *	@param[in]	lpszUrl		変換元のURL。
	 *	@param[in]	dwFlags		変換方法。
	 *					\n	ICU_DECODE				%XXの形式のエスケープシーケンスを文字に変換する。
	 *					\n	ICU_NO_ENCODE			特殊文字をエスケープシーケンスに変換しない。
	 *					\n	ICU_NO_META				メタシーケンス(".."など)をURLから削除しない。
	 *					\n	ICU_ENCODE_SPACES_ONLY	空白のみエンコードする。
	 *					\n	ICU_BROWSER_MODE		# や ?などの後に続く文字をエンコード・デコードしない。また、?の後の連続する空白を削除しない。
	 *											このフラグが指定されていない場合、URLはエンコードされ、連続する空白は削除される。
	 *					\n	※Flagsにどのフラグも指定されていない場合、関数は全ての特殊文字やメタシーケンスをエスケープシーケンスに変換する。
	 *	@param[in]	lpszRelativeUrl	組み合わせる相対指定のURL。省略可能。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool CanonicalizeUrl(LPTSTR _lpszDstUrl, DWORD& _dwDstSize, LPCTSTR lpszUrl, DWORD dwFlags, LPCTSTR lpszRelativeUrl = NULL)
	{
		BOOL boRc = FALSE;
		if ( lpszRelativeUrl == NULL )
		{
			boRc = ::InternetCanonicalizeUrl(lpszUrl, _lpszDstUrl, &_dwDstSize, dwFlags);
		}
		else
		{
			boRc = ::InternetCombineUrl(lpszUrl, lpszRelativeUrl, _lpszDstUrl, &_dwDstSize, dwFlags);
		}
		return !! boRc;
	}
};



}; // TNB


 
