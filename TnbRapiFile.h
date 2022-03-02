#pragma once
/**
 *@file
 * RAPI CEファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbFile.h"
#include "TnbRapi.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// PointerHandleBase用 RAPIハンドル Closeクラス
struct TPhRapiCloseHandle
{
	void operator()(HANDLE P) { if ( CRapi::IsSupported() ) { ::CeCloseHandle(P); } }
};

/// RAPI HANDLE型ハンドルハンドル
typedef TNB::CPointerHandleBaseT<HANDLE, TPhRapiCloseHandle, (INT_PTR)INVALID_HANDLE_VALUE> CRapiHandleHandle;

/// Apiテンプレート
template<typename TYP>
class CRapiFileApiT : public TYP
{
public:
	/// コンストラクタ
	CRapiFileApiT(void)
	{
		CRapi::Startup();
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
	/// API
	virtual BOOL ApiGetFileInformationByHandle(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION P) const
	{
		if ( ! CRapi::IsSupported() ) { return FALSE; }
		BOOL r = TRUE;
		Zero(*P);
		P->dwFileAttributes = ::GetFileAttributes(GetOpenName());
		r &= ::CeGetFileTime(hFile, &P->ftCreationTime, &P->ftLastAccessTime, &P->ftLastWriteTime);
		P->nFileSizeLow = ::CeGetFileSize(hFile, &P->nFileSizeHigh);
		::SetLastError(::CeGetLastError());
		return r;
	}
	/// API
	virtual HANDLE ApiCreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
	{
		if ( ! CRapi::IsSupported() ) { return INVALID_HANDLE_VALUE; }
		HANDLE h = ::CeCreateFile(CUnicode(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		::SetLastError(::CeGetLastError());
		return h;
	}
	/// API
	virtual DWORD ApiGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh) const
	{
		if ( ! CRapi::IsSupported() ) { return 0; }
		DWORD r = ::CeGetFileSize(hFile, lpFileSizeHigh);
		::SetLastError(::CeGetLastError());
		return r;
	}
	/// API
	virtual DWORD ApiSetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod) const
	{
		if ( ! CRapi::IsSupported() ) { return 0; }
		DWORD r = ::CeSetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
		::SetLastError(::CeGetLastError());
		return r;
	}
	/// API
	virtual BOOL ApiReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) const
	{
		if ( ! CRapi::IsSupported() ) { return FALSE; }
		BOOL r = ::CeReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
		::SetLastError(::CeGetLastError());
		return r;
	}
	/// API
	virtual BOOL ApiSetFileTime(HANDLE hFile, CONST FILETIME *lpCreationTime, CONST FILETIME *lpLastAccessTime, CONST FILETIME *lpLastWriteTime)
	{
		if ( ! CRapi::IsSupported() ) { return FALSE; }
		FILETIME f1 = *lpCreationTime;
		FILETIME f2 = *lpLastAccessTime;
		FILETIME f3 = *lpLastWriteTime;
		BOOL r = ::CeSetFileTime(hFile, &f1, &f2, &f3);
		::SetLastError(::CeGetLastError());
		return r;
	}
	/// API
	virtual BOOL ApiSetEndOfFile(HANDLE hFile)
	{
		if ( ! CRapi::IsSupported() ) { return FALSE; }
		BOOL r = ::CeSetEndOfFile(hFile);
		::SetLastError(::CeGetLastError());
		return r;
	}
	/// API
	virtual BOOL ApiWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		if ( ! CRapi::IsSupported() ) { return FALSE; }
		BOOL r = ::CeWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
		::SetLastError(::CeGetLastError());
		return r;
	}
private:
	mutable CRapiHandleHandle	m_hhFile;		///< ファイルハンドル
};

// RAPI ファイルハンドル管理クラス
//typedef CRapiFileApiT<CFileHandle> CRapiFileHandle;

#endif // _TnbDOXYGEN



/**@ingroup RAPI FILE
 * RAPI ファイル読み込みクラス
 *
 *	@note インスタンスの複製すると複製したすべてのインスタンスで同じファイルハンドルを
 *		使うようになります。すべてのインスタンスでCloseしないとファイルハンドルがクローズ
 *		されなくなります。
 *
 *	@note RAPI の接続状態は CRapi::IsConnected() で確認できます。
 *
 *	@attention 2GByte以上は一度に読み込めません。
 *
 *	@par必要ファイル
 *			TnbRapiFile.h
 *			\n Windows Mobile や PocketPC の SDK の ./Activesync/inc/rapi.h 等も必要です。
 *
 *	@date 09/04/24 新規作成
 */
#ifndef _TnbDOXYGEN	//Document作成用シンボル
typedef CRapiFileApiT<CFileReader> CRapiFileReader;
#else
 class CRapiFileReader : public CFileReader
 {typedef aa __a;};
#endif



/**@ingroup RAPI FILE
 * RAPI ファイル書き込みクラス
 *
 *	@note インスタンスの複製すると複製したすべてのインスタンスで同じファイルハンドルを
 *		使うようになります。すべてのインスタンスでCloseしないとファイルハンドルがクローズ
 *		されなくなります。
 *
 *	@note RAPI の接続状態は CRapi::IsConnected() で確認できます。
 *
 *	@attention 2GByte以上は一度に読み込めません。
 *	@attention 2GByte以上は一度に書き込めません。
 *
 *	@par必要ファイル
 *			TnbRapiFile.h
 *			\n Windows Mobile や PocketPC の SDK の ./Activesync/inc/rapi.h 等も必要です。
 *
 *	@date 09/04/24 新規作成
 */
#ifndef _TnbDOXYGEN	//Document作成用シンボル
typedef CRapiFileApiT<CFileWriter> CRapiFileWriter;
#else
 class CRapiFileWriter : public CFileWriter
 {typedef aa __a;};
#endif




}; // TNB


 
