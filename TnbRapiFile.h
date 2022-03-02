#pragma once
/**
 *@file
 * RAPI CE�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbFile.h"
#include "TnbRapi.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// PointerHandleBase�p RAPI�n���h�� Close�N���X
struct TPhRapiCloseHandle
{
	void operator()(HANDLE P) { if ( CRapi::IsSupported() ) { ::CeCloseHandle(P); } }
};

/// RAPI HANDLE�^�n���h���n���h��
typedef TNB::CPointerHandleBaseT<HANDLE, TPhRapiCloseHandle, (INT_PTR)INVALID_HANDLE_VALUE> CRapiHandleHandle;

/// Api�e���v���[�g
template<typename TYP>
class CRapiFileApiT : public TYP
{
public:
	/// �R���X�g���N�^
	CRapiFileApiT(void)
	{
		CRapi::Startup();
	}
	/**
	 * [�擾] �t�@�C���n���h���擾
	 *	@return �n���h��
	 */
	virtual HANDLE GetHandle(void) const
	{
		return m_hhFile;
	}
protected:
	/**
	 * [�ݒ�] �t�@�C���n���h���ݒ�
	 *	@param h  �n���h��
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
	mutable CRapiHandleHandle	m_hhFile;		///< �t�@�C���n���h��
};

// RAPI �t�@�C���n���h���Ǘ��N���X
//typedef CRapiFileApiT<CFileHandle> CRapiFileHandle;

#endif // _TnbDOXYGEN



/**@ingroup RAPI FILE
 * RAPI �t�@�C���ǂݍ��݃N���X
 *
 *	@note �C���X�^���X�̕�������ƕ����������ׂẴC���X�^���X�œ����t�@�C���n���h����
 *		�g���悤�ɂȂ�܂��B���ׂẴC���X�^���X��Close���Ȃ��ƃt�@�C���n���h�����N���[�Y
 *		����Ȃ��Ȃ�܂��B
 *
 *	@note RAPI �̐ڑ���Ԃ� CRapi::IsConnected() �Ŋm�F�ł��܂��B
 *
 *	@attention 2GByte�ȏ�͈�x�ɓǂݍ��߂܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbRapiFile.h
 *			\n Windows Mobile �� PocketPC �� SDK �� ./Activesync/inc/rapi.h �����K�v�ł��B
 *
 *	@date 09/04/24 �V�K�쐬
 */
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
typedef CRapiFileApiT<CFileReader> CRapiFileReader;
#else
 class CRapiFileReader : public CFileReader
 {typedef aa __a;};
#endif



/**@ingroup RAPI FILE
 * RAPI �t�@�C���������݃N���X
 *
 *	@note �C���X�^���X�̕�������ƕ����������ׂẴC���X�^���X�œ����t�@�C���n���h����
 *		�g���悤�ɂȂ�܂��B���ׂẴC���X�^���X��Close���Ȃ��ƃt�@�C���n���h�����N���[�Y
 *		����Ȃ��Ȃ�܂��B
 *
 *	@note RAPI �̐ڑ���Ԃ� CRapi::IsConnected() �Ŋm�F�ł��܂��B
 *
 *	@attention 2GByte�ȏ�͈�x�ɓǂݍ��߂܂���B
 *	@attention 2GByte�ȏ�͈�x�ɏ������߂܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbRapiFile.h
 *			\n Windows Mobile �� PocketPC �� SDK �� ./Activesync/inc/rapi.h �����K�v�ł��B
 *
 *	@date 09/04/24 �V�K�쐬
 */
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
typedef CRapiFileApiT<CFileWriter> CRapiFileWriter;
#else
 class CRapiFileWriter : public CFileWriter
 {typedef aa __a;};
#endif




}; // TNB


 
