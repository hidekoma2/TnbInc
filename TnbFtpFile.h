#pragma once
/**
 *@file
 * FTP ファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbInternetHandle.h"
#include "TnbWriter.h"
#include "TnbDummyCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup INTERNET FILE
 * FTPセッション管理クラス
 *
 *		サーバ接続とカレントディレクトリを管理します。カレントディレクトリ上の
 *		操作（デリート、リネームなど）をサポートします。
 *
 *	@par必要ファイル
 *			TnbFtpFile.h
 *
 *	@date 06/10/03 新規
 *	@date 12/02/09 CSftpSession とメソッド名をあわせた
 */
class CFtpSession
{
public:

	///コンストラクタ
	CFtpSession(void)
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CFtpSession(const CFtpSession& other) : m_hWininet(other.m_hWininet), m_hSession(other.m_hSession) 
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CFtpSession& operator=(const CFtpSession& other)
	{
		m_hWininet = other.m_hWininet;
		m_hSession = other.m_hSession;
		return *this;
	}

	/**
	 * [確認] ハンドル有効確認
	 *	@retval ture 有効。
	 *	@retval false 無効(開いていないか、失敗したか)。
	 */
	bool IsValid(void) const
	{
		return m_hSession.IsValid();
	}

	/**
	 * [設定] オープン.
	 *		WININET呼び出し用のハンドルと、指定サーバのセッションハンドルを作成します。 
	 *		本クラスのメソッドを使用するには、まずオープンする必要があります。
	 *	@param lpszAgent		HTTPによりインターネットにアクセスする際のエージェント名。
	 *	@param lpszServerName	接続先のホスト名またはドット表記のIPアドレス。
	 *	@param lpszUserName		接続の際に使用されるユーザ名。NULLを指定した場合デフォルト(FTPなら"anonymous")が使用される。
	 *	@param lpszPassword		接続の際に使用されるパスワード。UserNameとPasswordが共にNULLの場合、Passwordはe-mailアドレスが使用される。
	 *	@retval true 成功。本クラスのハンドルが有効になる。
	 *	@retval false 失敗。本クラスのハンドルは無効になる。
	 */
	bool Open(LPCTSTR lpszAgent, LPCTSTR lpszServerName, LPCTSTR lpszUserName = NULL, LPCTSTR lpszPassword = NULL)
	{
		Close();
		if ( ! m_hWininet.Open(lpszAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0) )
		{
			return false;
		}
		m_hSession = m_hWininet.Connect(lpszServerName, INTERNET_DEFAULT_FTP_PORT,
							lpszUserName, lpszPassword, INTERNET_SERVICE_FTP, 0, 0);
		return m_hSession.IsValid();
	}

	/**
	 * [設定] クローズ.
	 *		WININET呼び出し用のハンドルと、指定サーバのセッションハンドルを閉じます。 
	 */
	void Close(void)
	{
		m_hWininet.Close();
		m_hSession.Close();
	}

	/**
	 * [取得] セッションハンドル取得
	 *	@return ハンドル
	 */
	operator CInternetHandle&(void)
	{
		return m_hSession;
	}

	/**
	 * [作成] リモートディレクトリ作成.
	 *	@param lpszRemoteDir	作成するディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool CreateRemoteDir(LPCTSTR lpszRemoteDir)
	{
		return m_hSession.CreateDirectory(lpszRemoteDir);
	}

	/**
	 * [削除] リモートディレクトリ削除.
	 *	@param lpszRemoteDir	削除するディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool RemoveRemoteDir(LPCTSTR lpszRemoteDir)
	{
		return m_hSession.RemoveDirectory(lpszRemoteDir);
	}

	/**
	 * [取得] カレントリモートディレクトリ取得.
	 *		FTPセッション中でのカレントディレクトリを取得する。 
	 *	@note FTPセッションのハンドルを使用。
	 *	@param[out]		_lpszCurrentRemoteDir	カレントディレクトリ。
	 *	@param[in,out]	_length		_lpszCurrentRemoteDir のサイズ。関数の実行後、設定されたデータのサイズまたは必要なサイズが返される。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GetCurrentRemoteDir(LPTSTR _lpszCurrentRemoteDir, DWORD& _length)
	{
		return m_hSession.GetCurrentDirectory(_lpszCurrentRemoteDir, &_length);
	}

	/**
	 * [設定] カレントリモートディレクトリ設定.
	 *	@param lpszRemoteDir	変更先のディレクトリ名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetCurrentRemoteDir(LPCTSTR lpszRemoteDir)
	{
		return m_hSession.SetCurrentDirectory(lpszRemoteDir);
	}	

	/**
	 * [取得] リモートファイル取得.
	 *	@param lpszNewLocalFile	取得先のローカルファイル名。
	 *	@param lpszRemoteFile	取得元のリモートファイル名。
	 *	@param fFailIfExists	lpszNewLocalFile で指定されたファイルが既にローカルに存在するときに、エラーにするか否かを表す真偽値。
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
	bool GetRemoteFile(LPCTSTR lpszNewLocalFile, LPCTSTR lpszRemoteFile, BOOL fFailIfExists, DWORD dwFlagsAndAttributes, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return m_hSession.GetFile(lpszRemoteFile, lpszNewLocalFile, fFailIfExists, dwFlagsAndAttributes, dwFlags, dwContext);
	}

	/**
	 * [送信] ローカルファイル送信.
	 *	@param lpszNewRemoteFile	転送先のリモートファイル名。
	 *	@param lpszLocalFile	転送元のローカルファイル名。
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
	bool PutLocalFile(LPCTSTR lpszNewRemoteFile, LPCTSTR lpszLocalFile, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return m_hSession.PutFile(lpszLocalFile, lpszNewRemoteFile, dwFlags, dwContext);
	}

	/**
	 * [削除] リモートファイル削除.
	 *	@param lpszRemoteFile	削除するファイルのファイル名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool DeleteRemoteFile(LPCTSTR lpszRemoteFile)
	{
		return m_hSession.DeleteFile(lpszRemoteFile);
	}

	/**
	 * [変更] 名前変更.
	 *		リモートファイル名、リモートディレクトリ名の変更が出来ます。
	 *	@param lpszOldName	元のリモートファイル名。
	 *	@param lpszNewName	変更後のリモートファイル名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	*/
	bool RenameRemoteFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
	{
		return m_hSession.RenameFile(lpszOldName, lpszNewName);
	}

private:
	CInternetHandle m_hWininet;		///< WININET ハンドル
	CInternetHandle m_hSession;		///< セッションハンドル
};



/**@ingroup INTERNET FILE
 * FTPファイル読み込みクラス
 *
 *	@par必要ファイル
 *			TnbFtpFile.h
 *
 *	@date 07/01/15 新規
 */
class CFtpFileReader : public IReader
{
	DEFSUPER(IReader);
	mutable CFtpSession				m_hSession;		///< セッションハンドル
	mutable CPointerHandleT<DWORD>	m_phdwPointer;	///< ファイルポインタ
	mutable CInternetHandle			m_hFile;		///< １ファイルハンドル
	CStr							m_strFileName;	///< Openファイル名
	DWORD							m_dwAccess;		///< Openアクセス
	DWORD							m_dwFlags;		///< Openフラグ
	DWORD_PTR						m_dwContext;	///< Openコンテキスト
	friend class CFtpFileWriter;					///< フレンドクラス宣言

	/**
	 * 指定サイズ空込みこみ
	 *	@note FilePointer を ずらすのが目的
	 *	@retval マイナス エラー
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		読み込めたサイズ。
	 */
	size_t m_Padding(size_t size) const
	{
		try
		{
			CDummyCollectionT<BYTE> dc;
			return ReadInto(dc, size);
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
		}
		return INVALID_SIZE;
	}

	/**
	 * [設定] ポインタ設定
	 *	@param dw 設定する数
	 */
	void m_SetFilePointer(DWORD dw) const
	{
		m_phdwPointer = new DWORD(dw);
	}

	/**
	 * [取得] ポインタ取得
	 *	@return ポインタ
	 */
	DWORD m_GetFilePointer(void) const
	{
		if ( m_phdwPointer.IsNull() )
		{
			return 0;
		}
		return *m_phdwPointer;
	}

	/**
	 * [設定] Open用設定
	 *	@param lpszFileName	オープンするファイルの名前。
	 *	@param dwAccess アクセスフラグ。
	 *	@param dwFlags	ファイル転送の種別を表すフラグ。。
	 *	@param dwContext	コールバックに渡すアプリケーション定義の値。省略可能。
	 */
	void m_SetParamForOpen(LPCTSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext)
	{
		m_strFileName	= lpszFileName;
		m_dwAccess		= GENERIC_READ;
		m_dwFlags		= dwFlags;
		m_dwContext		= dwContext;
	}

	/**
	 * [設定] ファイルオープン
	 *	@note SetParamForOpen() によって設定された内容で Openします。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗
	*/
	bool m_Reopen(void) const
	{
		CInternetHandle& h = m_hSession;
		if ( h.IsValid() )
		{
			m_hFile.Close();
			m_hFile = h.OpenFile(m_strFileName, m_dwAccess, m_dwFlags, m_dwContext);
			m_SetFilePointer(0);
			return m_hFile.IsValid();
		}
		return false;
	}

	/**
	 * [設定] ポインタインクリメント
	 *	@param dw 足す数
	 */
	void m_AddFilePointer(DWORD dw) const
	{
		if ( m_phdwPointer.IsNull() )
		{
			m_phdwPointer = new DWORD(dw);
		}
		else
		{
			*m_phdwPointer += dw;
		}
	}

public:

	/**
	 * [設定] ファイルオープン.
	 *		FTPセッションのFileNameで指定されたファイルをオープンする。 
	 *	@param sec			オープンしたセッションハンドル。
	 *	@param lpszFileName	オープンするファイルの名前。
	 *	@param dwFlags	ファイル転送の種別を表すフラグ。省略すると FTP_TRANSFER_TYPE_BINARY 。
	 *					<BR>		まず以下のいずれかを指定する。 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII転送。
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	バイナリ転送。
	 *					<BR>
	 *					<BR>		次に、以下の任意の組み合わせを論理和によって指定する。 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	再読み込みすべきかどうかの判断時に有効期限・最終更新時刻がサーバから返されない場合強制的に再読み込みする。
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	キャッシュに永続的要素として追加する。これにより、通常のキャッシュのクリーンアップ、整合性チェック、ごみは行きなどの際にこの要素が削除されない。
	 *					<BR>		INTERNET_FLAG_RELOAD	要求したファイル、オブジェクト、ディレクトリリストをキャッシュから取得するのでなく、強制的にサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTPリソースをサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_NEED_FILE	ファイルがキャッシュできない場合に一時ファイルを作成する。
	 *	@param dwContext	コールバックに渡すアプリケーション定義の値。省略可能。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗
	 */
	bool Open(CFtpSession& sec, LPCTSTR lpszFileName, DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 0)
	{
		Close();
		m_hSession = sec;
		m_SetParamForOpen(lpszFileName, GENERIC_READ, dwFlags, dwContext);
		return m_Reopen();
	}

	/**
	 * [取得] ファイル読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		DWORD dwRead;
		if ( m_hFile.ReadFile(_P, ToDword(size), &dwRead) )
		{
			m_AddFilePointer(dwRead);
			return dwRead;
		}
		throw CReadFailureException(::GetLastError());
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	LPCTSTR GetOpenName(void) const
	{
		return m_strFileName;
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return m_hFile.IsValid();
	}

	/**
	 * [操作] シーク.
	 *		ファイルポインタを移動します。
	 *	@attention	ファイルポインタの移動は、空読み込みすることで、行なっています。
	 *				その為、パフォーマンスは良くありません。また、現在のポインタより前に戻る場合、
	 *				再オープンされます。
	 *	@param llOffset ファイルポインタの移動数。
	 *	@param eSeek 移動開始ファイルポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗
	 */
	LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		INT_PTR r = -1;
		int size = ToInt(GetSize());
		int ofs = ToInt(llOffset);
		DWORD current = m_GetFilePointer();
		switch ( eSeek )
		{
		case TOP:
			if ( ofs >= 0 && ofs <= size )
			{
				r = ofs;
			}
			break;
		case CURRENT:
			r = current + ofs;
			if ( r > size )
			{
				r = -1;
			}
			break;
		case END:
			if ( (-ofs) >= 0 && (-ofs) <= size )
			{
				r = size + ofs;
			}
			break;
		default:
			ASSERT(false);
			break;
		}
		if ( r >= 0 )
		{
			INT_PTR i = r - current;	//現在の位置から正負の方向？
			if ( i < 0 )
			{
				//== 負ならReopenする。
				if ( ! m_Reopen() )
				{
					r = -1;
				}
				else
				{
					i = r;
				}
			}
			if ( i > 0 )
			{
				r = m_Padding(i);
			}
			if ( r >= 0 )
			{
				r = m_GetFilePointer();
			}
		}
		return r;
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	LONGLONG GetSize(void) const
	{
		::SetLastError(NO_ERROR);
		ULARGE_INTEGER r;
		r.LowPart = m_hFile.GetFileSize(&r.HighPart);
		if ( ::GetLastError() != NO_ERROR )
		{
			return -1;
		}
		return static_cast<LONGLONG>(r.QuadPart);
	}

	/**
	 * [設定] ファイルクローズ.
	 */
	void Close(void)
	{
		m_hFile.Close();
		m_hSession.Close();
	}
};



/**@ingroup INTERNET FILE
 * FTPファイル書き込みクラス
 *
 *	@par必要ファイル
 *			TnbFtpFile.h
 *
 *	@date 07/01/16 新規
 */
class CFtpFileWriter : public IWriter
{
	DEFSUPER(IWriter);
	CFtpFileReader		m_ftp;	///< アクセス管理
public:

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void)
	{
		m_ftp.Close();
	}

	/**
	 * [取得] ファイルオープン.
	 *		FTPセッションのFileNameで指定されたファイルを書き込み可能でオープンする。 
	 *	@param sec			オープンしたセッションハンドル。
	 *	@param lpszFileName	オープンするファイルの名前。
	 *	@param dwFlags	ファイル転送の種別を表すフラグ。省略すると FTP_TRANSFER_TYPE_BINARY 。
	 *					<BR>		まず以下のいずれかを指定する。 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII転送。
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	バイナリ転送。
	 *					<BR>
	 *					<BR>		次に、以下の任意の組み合わせを論理和によって指定する。 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	再読み込みすべきかどうかの判断時に有効期限・最終更新時刻がサーバから返されない場合強制的に再読み込みする。
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	キャッシュに永続的要素として追加する。これにより、通常のキャッシュのクリーンアップ、整合性チェック、ごみは行きなどの際にこの要素が削除されない。
	 *					<BR>		INTERNET_FLAG_RELOAD	要求したファイル、オブジェクト、ディレクトリリストをキャッシュから取得するのでなく、強制的にサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTPリソースをサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_NEED_FILE	ファイルがキャッシュできない場合に一時ファイルを作成する。
	 *	@param dwContext	コールバックに渡すアプリケーション定義の値。省略可能。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗
	 */
	bool Open(CFtpSession& sec, LPCTSTR lpszFileName, DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 0)
	{
		Close();
		m_ftp.m_hSession = sec;
		m_ftp.m_SetParamForOpen(lpszFileName, GENERIC_READ | GENERIC_WRITE, dwFlags, dwContext);
		return m_ftp.m_Reopen();
	}

	/**
	 * [取得] ファイルオープン.
	 *		FTPセッションのFileNameで指定されたファイルを新規作成する。 
	 *	@param sec			オープンしたセッションハンドル。
	 *	@param lpszFileName	オープンするファイルの名前。
	 *	@param dwFlags	ファイル転送の種別を表すフラグ。省略すると FTP_TRANSFER_TYPE_BINARY 。
	 *					<BR>		まず以下のいずれかを指定する。 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII転送。
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	バイナリ転送。
	 *					<BR>
	 *					<BR>		次に、以下の任意の組み合わせを論理和によって指定する。 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	再読み込みすべきかどうかの判断時に有効期限・最終更新時刻がサーバから返されない場合強制的に再読み込みする。
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	キャッシュに永続的要素として追加する。これにより、通常のキャッシュのクリーンアップ、整合性チェック、ごみは行きなどの際にこの要素が削除されない。
	 *					<BR>		INTERNET_FLAG_RELOAD	要求したファイル、オブジェクト、ディレクトリリストをキャッシュから取得するのでなく、強制的にサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTPリソースをサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_NEED_FILE	ファイルがキャッシュできない場合に一時ファイルを作成する。
	 *	@param dwContext	コールバックに渡すアプリケーション定義の値。省略可能。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗
	 */
	bool New(CFtpSession& sec, LPCTSTR lpszFileName, DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 0)
	{
		Close();
		if ( ! sec.DeleteRemoteFile(lpszFileName) )
		{
			return false;
		}
		return Open(sec, lpszFileName, dwFlags, dwContext);
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	LPCTSTR GetOpenName(void) const
	{
		return m_ftp.m_strFileName;
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return m_ftp.m_hFile.IsValid();
	}

	/**
	 * [確認] 書込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanWrite(void) const
	{
		return CanRead() && (m_ftp.m_dwAccess & GENERIC_WRITE) != 0;
	}

	/**
	 * [取得] 読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		return m_ftp.Read(size, _P);
	}

	/**
	 * [設定] EOFを指定.
	 *	@throw CNotSupportException サポートしていないため、スローされます。
	 */
	virtual void SetEnd(void)
	{
		throw CNotSupportException();
	}

	/**
	 * [保存] ファイル書き込み
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		DWORD dwWriten;
		if ( m_ftp.m_hFile.WriteFile(P, ToDword(size), &dwWriten) )
		{
			m_ftp.m_AddFilePointer(dwWriten);
			if ( size == dwWriten )
			{
				return;
			}
		}
		throw CWriteFailureException(::GetLastError());
	}

	/**
	 * [操作] シーク.
	 *		ファイルポインタを移動します。
	 *	@attention	ファイルポインタの移動は、空読み込みすることで、行なっています。
	 *				その為、パフォーマンスは良くありません。また、現在のポインタより前に戻る場合、
	 *				再オープンされます。
	 *	@param llOffset ファイルポインタの移動数。
	 *	@param eSeek 移動開始ファイルポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗
	 */
	LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return m_ftp.Seek(llOffset, eSeek);
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		return m_ftp.GetSize();
	}
};



}; // TNB


 
