#pragma once
/**
 *@file
 * ファイル関係のヘッダ
 *
 *		ファイル名管理、ファイルハンドル管理のクラス、
 *		さらに簡易書込みクラス、簡易読み込みクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbWriter.h"
#include "TnbTime.h"
#include "TnbHandleHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * 抽象ファイルクラス
 *
 *	@see CFileReader , CFileWriter のためのクラスです。
 *
 *	@date 06/05/01 新規作成
 *	@date 09/04/24 Hideクラスへ。
 *	@date 11/08/30 排他オプションの設定、 ReadOnly オープンなら 読込排他のみ設定するように変更。
 */
class CAbstractFile
{
public:
	/// オープンモード
	enum EMode
	{
		READ,		///< 読み込みオープン(既存でなければエラーになります)
		CREATE,		///< 新規読み書きオープン(既存ファイルは消されます)
		ACCESS,		///< 読み書きオープン(既存でなければ作成されます)
		WRITE,		///< 書き込みオープン(既存でなければ作成されます)
	};

	/// コンストラクタ
	CAbstractFile(void) : m_boCanWrite(false), m_attributes(0)
	{
	}

	/// デストラクタ
	virtual ~CAbstractFile(void)
	{
		Close();
	}

	/**
	 * [操作] クローズ
	 *	@note 複製している場合、複製したすべてのハンドルが閉じないと実際のファイルは
	 *			クローズされません。
	 */
	void Close(void)
	{
		SetHandle(INVALID_HANDLE_VALUE);
	}

	/**
	 * [設定] 属性設定.
	 *	@note CreateFile 時の属性値の設定が可能です。
	 *	@param attr 属性。
	 */
	void SetCreatedAttributes(DWORD attr)
	{
		m_attributes = attr;
	}

	/**
	 * [操作] モード付オープン
	 *	@note	特殊なオープンをする以外は、サブクラスの Open系メソッドを使用してください。 
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_createfile.asp
	 *	@param lpszName	オープンするファイル名。
	 *	@param eMode	オープンモード。省略なら access になります。
	 *	@param isShare 省略かtrueなら、共有モードでオープンします。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗 ( GetLastError() により詳細取得可能)
	 */
	bool OpenOnMode(LPCTSTR lpszName, EMode eMode = ACCESS, bool isShare = true)
	{
		Close();
		m_strName = lpszName;
		m_boCanWrite = true;
		DWORD dwDesiredAccess;
		DWORD dwShareMode;
		DWORD dwCreationDisposition;
		switch ( eMode )
		{
		default:
		case READ:		// 読み込みオープン(既存でなければエラーになります)
			dwDesiredAccess			= GENERIC_READ;
			dwCreationDisposition	= OPEN_EXISTING;
			dwShareMode	= isShare ? (FILE_SHARE_READ) : 0;
			m_boCanWrite = false;
			break;
		case WRITE:		// 書き込みオープン(既存でなければ作成されます)
			dwDesiredAccess			= GENERIC_WRITE;
			dwCreationDisposition	= OPEN_ALWAYS;
			dwShareMode	= isShare ? (FILE_SHARE_WRITE) : 0;
			break;
		case ACCESS:	// 読み書きオープン(既存でなければ作成されます)
			dwDesiredAccess			= GENERIC_WRITE | GENERIC_READ;
			dwCreationDisposition	= OPEN_ALWAYS;
			dwShareMode	= isShare ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : 0;
			break;
		case CREATE:	// 新規書き込みオープン(既存ファイルは消されます)
			dwDesiredAccess			= GENERIC_WRITE | GENERIC_READ;
			dwCreationDisposition	= CREATE_ALWAYS;
			dwShareMode	= isShare ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : 0;
			break;
		}
		SetHandle(ApiCreateFile(lpszName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, m_attributes, NULL));
		if ( GetHandle() != INVALID_HANDLE_VALUE )
		{
			return true;
		}
		return false;
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	bool CanRead(void) const
	{
		return GetHandle() != INVALID_HANDLE_VALUE;
	}

	/**
	 * [確認] 書込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	bool CanWrite(void) const
	{
		return CanRead() && m_boCanWrite;
	}

	/**
	 * [取得] ファイル情報取得
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileInformationByHandle.asp
	 *	@param[out] _lpFileInformation 取得情報の格納場所
	 *	@retval true 取得成功
	 *	@retval false 失敗
	 */
	bool GetInformation(LPBY_HANDLE_FILE_INFORMATION _lpFileInformation) const
	{
		bool r = !! ApiGetFileInformationByHandle(GetHandle(), _lpFileInformation);
		return r;
	}

	/**
	 * [取得] ファイルサイズ取得
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileSize.asp
	 *	@retval ０以上 ファイルサイズ
	 *	@retval マイナス 取得失敗 ( GetLastError() により詳細取得可能)
	 */
	LONGLONG GetSize(void) const
	{
		::SetLastError(NO_ERROR);
		ULARGE_INTEGER r;
		r.LowPart = ApiGetFileSize(GetHandle(), &r.HighPart);
		if ( ::GetLastError() != NO_ERROR )
		{
			return -1;
		}
		return static_cast<LONGLONG>(r.QuadPart);
	}

	/**
	 * [操作] シーク.
	 *		ファイルポインタを移動します。
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetFilePointer.asp
	 *	@param llOffset ファイルポインタの移動数。
	 *	@param eSeek 移動開始ファイルポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗 ( GetLastError() により詳細取得可能)
	 */
	LONGLONG Seek(LONGLONG llOffset, IReader::ESeekMode eSeek = IReader::TOP) const
	{
		LARGE_INTEGER r;
		r.QuadPart = llOffset;
		r.LowPart = ApiSetFilePointer(GetHandle(), r.LowPart, &r.HighPart, eSeek);
		if ( r.LowPart == INVALID_SET_FILE_POINTER )
		{
			if ( ::GetLastError() != NO_ERROR )
			{
				r.QuadPart = -1;
			}
		}
		return r.QuadPart;
	}

	/**
	 * [取得] ファイル読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	size_t Read(size_t size, LPVOID _P) const
	{
		if ( size > ULONG_MAX )
		{
			size = ULONG_MAX;
		}
		DWORD dwRead;
		BOOL boRc = ApiReadFile(GetHandle(), _P, ToDword(size), &dwRead, NULL);
		if ( ! boRc )
		{
			throw CReadFailureException(::GetLastError());
		}
		return dwRead;
	}

	/**
	 * [取得] ファイルハンドル取得
	 *	@return ハンドル
	 */
	operator HANDLE(void) const
	{
		return GetHandle();
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	LPCTSTR GetOpenName(void) const
	{
		return m_strName;
	}

	/**
	 * [取得] ファイルハンドル取得
	 *	@return ハンドル
	 */
	virtual HANDLE GetHandle(void) const
	{
		return m_hhFile;
	}
	
protected:
	/**
	 * [設定] ファイルハンドル設定
	 *	@param h  ハンドル
	 */
	virtual void SetHandle(HANDLE h)
	{
		m_hhFile = h;
	}

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	/// API
	virtual BOOL ApiGetFileInformationByHandle(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInformation) const
	{
		return ::GetFileInformationByHandle(hFile, lpFileInformation);
	}
	/// API
	virtual HANDLE ApiCreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
	{
		return ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	/// API
	virtual DWORD ApiGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh) const
	{
		return ::GetFileSize(hFile, lpFileSizeHigh);
	}
	/// API
	virtual DWORD ApiSetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod) const
	{
		return ::SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
	}
	/// API
	virtual BOOL ApiReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) const
	{
		return ::ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	}
	/// API
	virtual BOOL ApiSetFileTime(HANDLE hFile, CONST ::FILETIME *lpCreationTime, CONST ::FILETIME *lpLastAccessTime, CONST ::FILETIME *lpLastWriteTime)
	{
		return ::SetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
	}
	/// API
	virtual BOOL ApiSetEndOfFile(HANDLE hFile)
	{
		return ::SetEndOfFile(hFile);
	}
	/// API
	virtual BOOL ApiWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		return ::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}

	#endif // _TnbDOXYGEN

private:
	mutable CHandleHandle	m_hhFile;		///< ファイルハンドル
	CStr					m_strName;		///< オープンした(使用とした)ファイル名
	bool					m_boCanWrite;	///< Write属性ありか？
	DWORD					m_attributes;	///< API CreateFile()の dwFlagsAndAttributes。
};



/**@ingroup FILE
 * ファイル読み込みクラス
 *
 *	@note インスタンスの複製すると複製したすべてのインスタンスで同じファイルハンドルを
 *		使うようになります。すべてのインスタンスでCloseしないとファイルハンドルがクローズ
 *		されなくなります。
 *
 *	@attention 2GByte以上は一度に読み込めません。
 *
 *	@par必要ファイル
 *			TnbFile.h
 *
 *	@date 06/05/01 新規作成
 *	@date 06/08/24 Read() の戻り値を変更。ReadExactly() を新規。
 *	@date 06/10/02 抽象化して作成した IReader を実装。
 *	@date 06/12/19 新 IReader に合わせてリファクタリング。
 *	@date 09/04/24 ベースクラス変更。
 *	@date 10/11/30 ReadAll() を新規。
 */
class CFileReader : public IReader, public CAbstractFile
{
	DEFSUPER(IReader);
	typedef CAbstractFile _base;
public:

	/// コンストラクタ
	CFileReader(void) : _base() 
	{
	}

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void)
	{
		_base::Close();
	}

	/**
	 * [操作] オープン
	 *	@param lpszName ファイル名
	 *	@param boIsShare 省略かtrueなら、共有モードでオープンします。
	 *	@param boDummy ダミーです。省略してください。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗 ( GetLastError() により詳細取得可能)
	 */
	bool Open(LPCTSTR lpszName, bool boIsShare = true, bool boDummy = false)
	{
		return _base::OpenOnMode(lpszName, _base::READ, boIsShare);
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return _base::GetOpenName();
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return _base::CanRead();
	}

	/**
	 * [操作] シーク.
	 *		ファイルポインタを移動します。
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetFilePointer.asp
	 *	@param llOffset ファイルポインタの移動数。
	 *	@param eSeek 移動開始ファイルポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗 ( GetLastError() により詳細取得可能)
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return _base::Seek(llOffset, eSeek);
	}

	/**
	 * [取得] ファイルサイズ取得
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileSize.asp
	 *	@retval ０以上 ファイルサイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		return _base::GetSize();
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
		return _base::Read(size, _P);
	}

	/**
	 * [取得] ファイル全読み込み.
	 *		指定のファイルの内容をすべて読み込みます。
	 *	@param[out] _content ファイルの内容が格納されます。
	 *	@param[in] lpszFileName ファイル名。
	 *	@param[in] fr リーダー。通常省略します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool ReadAll(CByteVector& _content, LPCTSTR lpszFileName, CFileReader& fr = CFileReader())
	{
		if ( fr.Open(lpszFileName) )
		{
			try
			{
				_content = fr.ReadInto();
				return true;
			}
			catch ( ... )
			{
			}
		}
		return false;
	}
};



/**@ingroup FILE
 * ファイル書き込みクラス
 *
 *	@note インスタンスの複製すると複製したすべてのインスタンスで同じファイルハンドルを
 *		使うようになります。すべてのインスタンスでCloseしないとファイルハンドルがクローズ
 *		されなくなります。
 *
 *	@attention 2GByte以上は一度に読み込めません。
 *	@attention 2GByte以上は一度に書き込めません。
 *
 *	@par必要ファイル
 *			TnbFile.h
 *
 *	@date 06/05/01 新規作成
 *	@date 06/08/30 ISerializer 実装。
 *	@date 06/10/02 抽象化して作成した IReader を実装。
 *	@date 06/12/19 新 IWriter に合わせてリファクタリング。
 *	@date 09/04/24 ベースクラス変更。
 *	@date 10/11/30 クラスメソッド NewFile() 追加。
 *	@date 11/09/16 New() の共有フラグが機能していなかったのを修正。
 */
class CFileWriter : public IWriter, public CAbstractFile
{
	DEFSUPER(IWriter);
	typedef CAbstractFile _base;
public:

	/// コンストラクタ
	CFileWriter(void) : _base()
	{
	}

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void)
	{
		_base::Close();
	}

	/**
	 * [操作] 新規オープン.
	 *	@note 既存の場合、削除して新規で作成します。
	 *	@param lpszName ファイル名。
	 *	@param boIsShare true なら共有モードでオープンします。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗 ( GetLastError() により詳細取得可能)
	 */
	bool New(LPCTSTR lpszName, bool boIsShare = true)
	{
		return _base::OpenOnMode(lpszName, _base::CREATE, boIsShare);
	}

	/**
	 * [操作] オープン.
	 *	@note 既存しないなら新規で作成します。
	 *	@param lpszName ファイル名。
	 *	@param boIsShare 省略かtrueなら、共有モードでオープンします。
	 *	@param boIsBottom オープン時、true ならファイルポインタを最後に移動します。
	 *					falseなら先頭のままです。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗 ( GetLastError() により詳細取得可能)
	 */
	bool Open(LPCTSTR lpszName, bool boIsShare = true, bool boIsBottom = true)
	{
		bool r =_base::OpenOnMode(lpszName, _base::ACCESS, boIsShare);
		if ( r && boIsBottom )
		{
			Seek(0, END);
		}
		return r;
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return _base::GetOpenName();
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return _base::CanRead();
	}

	/**
	 * [確認] 書込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanWrite(void) const
	{
		return _base::CanWrite();
	}

	/**
	 * [操作] シーク.
	 *		ファイルポインタを移動します。
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetFilePointer.asp
	 *	@param llOffset ファイルポインタの移動数。
	 *	@param eSeek 移動開始ファイルポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗 ( GetLastError() により詳細取得可能)
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return _base::Seek(llOffset, eSeek);
	}

	/**
	 * [取得] ファイルサイズ取得
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileSize.asp
	 *	@retval ０以上 ファイルサイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		return _base::GetSize();
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
		return _base::Read(size, _P);
	}

	/**
	 * [設定] ファイル日付設定
	 *	@param timeCreation 作成日付
	 *	@param timeLastAccess 最終アクセス日付
	 *	@param timeLastWrite 書込み日付
	 *	@throw CWriteFailureException コンストラクタ throw を有効にしていて、書込みに失敗すると、スローされます。
	 */
	void SetTime(const CFileTimeEx& timeCreation, const CFileTimeEx& timeLastAccess, const CFileTimeEx& timeLastWrite)
	{
		BOOL r = ApiSetFileTime(GetHandle(), &timeCreation, &timeLastAccess, &timeLastWrite);
		if ( ! r )
		{ 
			throw CWriteFailureException(::GetLastError());
		}
	}

	/**
	 * [設定] EOFを指定.
	 *		現在のファイルポインタの位置をEOFにします。
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetEndOfFile.asp
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	virtual void SetEnd(void)
	{
		if ( ! ApiSetEndOfFile(GetHandle()) )
		{
			throw CWriteFailureException(::GetLastError()); 
		}
	}

	/**
	 * [保存] ファイル書き込み
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。size が大き過ぎてもエラーになります。
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		if ( size >= ULONG_MAX )
		{
			throw CWriteFailureException(ERROR_BAD_LENGTH);
		}
		DWORD dwWrite;
		bool boRc = !! ApiWriteFile(GetHandle(), P, ToDword(size), &dwWrite, NULL);
		DWORD dwError = boRc ? 0 : ::GetLastError();
		if ( ! boRc || size != dwWrite )
		{
			throw CWriteFailureException(dwError);
		}
	}

	/**
	 * [保存] ファイル作成.
	 *		ファイル名と、内容を指定し、ファイルを作成します。
	 *	@param lpszFileName ファイル名。
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@param fw ライター。通常省略します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool NewFile(LPCTSTR lpszFileName, size_t size, LPCVOID P, CFileWriter& fw = CFileWriter())
	{
		if ( fw.New(lpszFileName, false) )
		{
			try
			{
				fw.Write(size, P);
				return true;
			}
			catch ( ... )
			{
			}
		}
		return false;
	}

	/**
	 * [保存] ファイル追加.
	 *		ファイル名と、内容を指定し、ファイルを作成します。
	 *	@param lpszFileName ファイル名。
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@param fw ライター。通常省略します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool AddFile(LPCTSTR lpszFileName, size_t size, LPCVOID P, CFileWriter& fw = CFileWriter())
	{
		if ( fw.Open(lpszFileName, false, true/*bottom*/) )
		{
			try
			{
				fw.Write(size, P);
				return true;
			}
			catch ( ... )
			{
			}
		}
		return false;
	}
};



/**@ingroup FILE
 * [保存] BYTEコレクション書き込み
 *	@note ヘッダとチェックサムをつけてファイルへ書き込みます。
 *	@note CByteVectorなどにシリアライズしたデータをファイルに書き込む時に使用することを想定しています。
 *	@param wr 書き込み準備の出来た、Writer。
 *	@param lpszHead ヘッダ識別名。
 *	@param c 書き込みデータ。
 *	@throw CIoFailureException ファイルアクセスに失敗すると、スローされます。
 */
inline void SaveCollection(IWriter& wr, LPCTSTR lpszHead, const ISequenceCollectionT<BYTE>& c)
{
	//- Head書込み
	CAscii asc = lpszHead;
	size_t dwLen = asc.GetLength() + 1;
	wr.Write(sizeof(DWORD), &dwLen);
	wr.Write(sizeof(DWORD), &dwLen);
	wr.Write(dwLen, LPCSTR(asc));
	//- SUM計算
	size_t size = c.GetSize();
	WORD wSum = 0;
	const BYTE* B = c.ReferBuffer();
	for ( size_t i = 0; i < size; i++ )
	{
		wSum = static_cast<WORD>(wSum + *B++);
	}
	//- 書込み
	wr.Write(sizeof(size_t), &size);
	wr.Write(size, c.ReferBuffer());
	wr.Write(sizeof(WORD), &wSum);
}



/**@ingroup FILE
 * [保存] BYTEコレクション書き込み
 *	@note ヘッダとチェックサムをつけてファイルへ書き込みます。
 *	@note CByteVectorなどにシリアライズしたデータをファイルに書き込む時に使用することを想定しています。
 *	@param lpszFile ファイル名。
 *	@param lpszHead ヘッダ識別名。
 *	@param c 書き込みデータ。
 *	@throw CIoFailureException ファイルアクセスに失敗すると、スローされます。
 */
inline void SaveCollection(LPCTSTR lpszFile, LPCTSTR lpszHead, const ISequenceCollectionT<BYTE>& c)
{
	CFileWriter fw;
	if ( ! fw.New(lpszFile, false) )
	{
		throw COpenFailureException(::GetLastError());
	}
	SaveCollection(fw, lpszHead, c);
}



/**@ingroup FILE
 * [保存] コレクション読み込み
 *	@note SaveCollection() で書き込んだものを読み込みます。
 *	@param[in] rr 読み込み準備が出来た Reader。
 *	@param[in] lpszHead ヘッダ識別名。
 *	@param[out] _vb 読み込み。
 *	@throw CNotSupportException		ヘッダ形状が異なるとスローされます。
 *	@throw CInvalidParamException	ファイル長さが異なるとスローされます。
 *	@throw CNoSuchTypeException		チェックサムが異なるとスローされます。
 *	@throw CIoFailureException		ファイルアクセスに失敗すると、スローされます。
 */
inline void LoadCollection(const IReader& rr, LPCTSTR lpszHead, CVectorT<BYTE>& _vb)
{
	//- Head読み込み
	{
		CAscii asc;
		DWORD dwLen1;
		rr.ReadExactly(sizeof(DWORD), &dwLen1);
		DWORD dwLen2;
		rr.ReadExactly(sizeof(DWORD), &dwLen2);
		if ( dwLen1 != dwLen2 )
		{
			throw CNotSupportException(); 
		}
		rr.ReadExactly(dwLen1, asc.GetBuffer(dwLen1));
		asc.ReleaseBuffer();
		if ( ! asc.IsEqual(CAscii(lpszHead)) )
		{
			throw CNotSupportException(); 
		}
	}
	//
	size_t size;
	rr.ReadExactly(sizeof(size_t), &size);
	rr.ReadExactly(size, _vb.GetBuffer(size));
	_vb.ReferBuffer();
	WORD wFileSum;
	rr.ReadExactly(sizeof(WORD), &wFileSum);
	//- SUM計算
	WORD wSum = 0;
	const BYTE* B = _vb.ReferBuffer();
	for ( size_t i = 0; i < size; i++ )
	{
		wSum = static_cast<WORD>(wSum + *B++);
	}
	if ( wFileSum != wSum )
	{
		throw CNoSuchTypeException(); 
	}
}



/**@ingroup FILE
 * [保存] コレクション読み込み
 *	@note SaveCollection() で書き込んだものを読み込みます。
 *	@param[in] lpszFile ファイル名。
 *	@param[in] lpszHead ヘッダ識別名。
 *	@param[out] _vb 読み込み。
 *	@throw CNotSupportException		ヘッダ形状が異なるとスローされます。
 *	@throw CInvalidParamException	ファイル長さが異なるとスローされます。
 *	@throw CNoSuchTypeException		チェックサムが異なるとスローされます。
 *	@throw CIoFailureException		ファイルアクセスに失敗すると、スローされます。
 */
inline void LoadCollection(LPCTSTR lpszFile, LPCTSTR lpszHead, CVectorT<BYTE>& _vb)
{
	CFileReader fr;
	if ( ! fr.Open(lpszFile) )
	{
		throw COpenFailureException(::GetLastError());
	}
	LoadCollection(fr, lpszHead, _vb);
}



}; // TNB
