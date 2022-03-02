#pragma once
/**
 *@file
 * �t�@�C���֌W�̃w�b�_
 *
 *		�t�@�C�����Ǘ��A�t�@�C���n���h���Ǘ��̃N���X�A
 *		����ɊȈՏ����݃N���X�A�ȈՓǂݍ��݃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbWriter.h"
#include "TnbTime.h"
#include "TnbHandleHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ���ۃt�@�C���N���X
 *
 *	@see CFileReader , CFileWriter �̂��߂̃N���X�ł��B
 *
 *	@date 06/05/01 �V�K�쐬
 *	@date 09/04/24 Hide�N���X�ցB
 *	@date 11/08/30 �r���I�v�V�����̐ݒ�A ReadOnly �I�[�v���Ȃ� �Ǎ��r���̂ݐݒ肷��悤�ɕύX�B
 */
class CAbstractFile
{
public:
	/// �I�[�v�����[�h
	enum EMode
	{
		READ,		///< �ǂݍ��݃I�[�v��(�����łȂ���΃G���[�ɂȂ�܂�)
		CREATE,		///< �V�K�ǂݏ����I�[�v��(�����t�@�C���͏�����܂�)
		ACCESS,		///< �ǂݏ����I�[�v��(�����łȂ���΍쐬����܂�)
		WRITE,		///< �������݃I�[�v��(�����łȂ���΍쐬����܂�)
	};

	/// �R���X�g���N�^
	CAbstractFile(void) : m_boCanWrite(false), m_attributes(0)
	{
	}

	/// �f�X�g���N�^
	virtual ~CAbstractFile(void)
	{
		Close();
	}

	/**
	 * [����] �N���[�Y
	 *	@note �������Ă���ꍇ�A�����������ׂẴn���h�������Ȃ��Ǝ��ۂ̃t�@�C����
	 *			�N���[�Y����܂���B
	 */
	void Close(void)
	{
		SetHandle(INVALID_HANDLE_VALUE);
	}

	/**
	 * [�ݒ�] �����ݒ�.
	 *	@note CreateFile ���̑����l�̐ݒ肪�\�ł��B
	 *	@param attr �����B
	 */
	void SetCreatedAttributes(DWORD attr)
	{
		m_attributes = attr;
	}

	/**
	 * [����] ���[�h�t�I�[�v��
	 *	@note	����ȃI�[�v��������ȊO�́A�T�u�N���X�� Open�n���\�b�h���g�p���Ă��������B 
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_createfile.asp
	 *	@param lpszName	�I�[�v������t�@�C�����B
	 *	@param eMode	�I�[�v�����[�h�B�ȗ��Ȃ� access �ɂȂ�܂��B
	 *	@param isShare �ȗ���true�Ȃ�A���L���[�h�ŃI�[�v�����܂��B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s ( GetLastError() �ɂ��ڍ׎擾�\)
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
		case READ:		// �ǂݍ��݃I�[�v��(�����łȂ���΃G���[�ɂȂ�܂�)
			dwDesiredAccess			= GENERIC_READ;
			dwCreationDisposition	= OPEN_EXISTING;
			dwShareMode	= isShare ? (FILE_SHARE_READ) : 0;
			m_boCanWrite = false;
			break;
		case WRITE:		// �������݃I�[�v��(�����łȂ���΍쐬����܂�)
			dwDesiredAccess			= GENERIC_WRITE;
			dwCreationDisposition	= OPEN_ALWAYS;
			dwShareMode	= isShare ? (FILE_SHARE_WRITE) : 0;
			break;
		case ACCESS:	// �ǂݏ����I�[�v��(�����łȂ���΍쐬����܂�)
			dwDesiredAccess			= GENERIC_WRITE | GENERIC_READ;
			dwCreationDisposition	= OPEN_ALWAYS;
			dwShareMode	= isShare ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : 0;
			break;
		case CREATE:	// �V�K�������݃I�[�v��(�����t�@�C���͏�����܂�)
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
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	bool CanRead(void) const
	{
		return GetHandle() != INVALID_HANDLE_VALUE;
	}

	/**
	 * [�m�F] �����݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	bool CanWrite(void) const
	{
		return CanRead() && m_boCanWrite;
	}

	/**
	 * [�擾] �t�@�C�����擾
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileInformationByHandle.asp
	 *	@param[out] _lpFileInformation �擾���̊i�[�ꏊ
	 *	@retval true �擾����
	 *	@retval false ���s
	 */
	bool GetInformation(LPBY_HANDLE_FILE_INFORMATION _lpFileInformation) const
	{
		bool r = !! ApiGetFileInformationByHandle(GetHandle(), _lpFileInformation);
		return r;
	}

	/**
	 * [�擾] �t�@�C���T�C�Y�擾
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileSize.asp
	 *	@retval �O�ȏ� �t�@�C���T�C�Y
	 *	@retval �}�C�i�X �擾���s ( GetLastError() �ɂ��ڍ׎擾�\)
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
	 * [����] �V�[�N.
	 *		�t�@�C���|�C���^���ړ����܂��B
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetFilePointer.asp
	 *	@param llOffset �t�@�C���|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�t�@�C���|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s ( GetLastError() �ɂ��ڍ׎擾�\)
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
	 * [�擾] �t�@�C���ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
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
	 * [�擾] �t�@�C���n���h���擾
	 *	@return �n���h��
	 */
	operator HANDLE(void) const
	{
		return GetHandle();
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	LPCTSTR GetOpenName(void) const
	{
		return m_strName;
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

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

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
	mutable CHandleHandle	m_hhFile;		///< �t�@�C���n���h��
	CStr					m_strName;		///< �I�[�v������(�g�p�Ƃ���)�t�@�C����
	bool					m_boCanWrite;	///< Write�������肩�H
	DWORD					m_attributes;	///< API CreateFile()�� dwFlagsAndAttributes�B
};



/**@ingroup FILE
 * �t�@�C���ǂݍ��݃N���X
 *
 *	@note �C���X�^���X�̕�������ƕ����������ׂẴC���X�^���X�œ����t�@�C���n���h����
 *		�g���悤�ɂȂ�܂��B���ׂẴC���X�^���X��Close���Ȃ��ƃt�@�C���n���h�����N���[�Y
 *		����Ȃ��Ȃ�܂��B
 *
 *	@attention 2GByte�ȏ�͈�x�ɓǂݍ��߂܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbFile.h
 *
 *	@date 06/05/01 �V�K�쐬
 *	@date 06/08/24 Read() �̖߂�l��ύX�BReadExactly() ��V�K�B
 *	@date 06/10/02 ���ۉ����č쐬���� IReader �������B
 *	@date 06/12/19 �V IReader �ɍ��킹�ă��t�@�N�^�����O�B
 *	@date 09/04/24 �x�[�X�N���X�ύX�B
 *	@date 10/11/30 ReadAll() ��V�K�B
 */
class CFileReader : public IReader, public CAbstractFile
{
	DEFSUPER(IReader);
	typedef CAbstractFile _base;
public:

	/// �R���X�g���N�^
	CFileReader(void) : _base() 
	{
	}

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void)
	{
		_base::Close();
	}

	/**
	 * [����] �I�[�v��
	 *	@param lpszName �t�@�C����
	 *	@param boIsShare �ȗ���true�Ȃ�A���L���[�h�ŃI�[�v�����܂��B
	 *	@param boDummy �_�~�[�ł��B�ȗ����Ă��������B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s ( GetLastError() �ɂ��ڍ׎擾�\)
	 */
	bool Open(LPCTSTR lpszName, bool boIsShare = true, bool boDummy = false)
	{
		return _base::OpenOnMode(lpszName, _base::READ, boIsShare);
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return _base::GetOpenName();
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return _base::CanRead();
	}

	/**
	 * [����] �V�[�N.
	 *		�t�@�C���|�C���^���ړ����܂��B
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetFilePointer.asp
	 *	@param llOffset �t�@�C���|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�t�@�C���|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s ( GetLastError() �ɂ��ڍ׎擾�\)
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return _base::Seek(llOffset, eSeek);
	}

	/**
	 * [�擾] �t�@�C���T�C�Y�擾
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileSize.asp
	 *	@retval �O�ȏ� �t�@�C���T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		return _base::GetSize();
	}

	/**
	 * [�擾] �t�@�C���ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		return _base::Read(size, _P);
	}

	/**
	 * [�擾] �t�@�C���S�ǂݍ���.
	 *		�w��̃t�@�C���̓��e�����ׂēǂݍ��݂܂��B
	 *	@param[out] _content �t�@�C���̓��e���i�[����܂��B
	 *	@param[in] lpszFileName �t�@�C�����B
	 *	@param[in] fr ���[�_�[�B�ʏ�ȗ����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
 * �t�@�C���������݃N���X
 *
 *	@note �C���X�^���X�̕�������ƕ����������ׂẴC���X�^���X�œ����t�@�C���n���h����
 *		�g���悤�ɂȂ�܂��B���ׂẴC���X�^���X��Close���Ȃ��ƃt�@�C���n���h�����N���[�Y
 *		����Ȃ��Ȃ�܂��B
 *
 *	@attention 2GByte�ȏ�͈�x�ɓǂݍ��߂܂���B
 *	@attention 2GByte�ȏ�͈�x�ɏ������߂܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbFile.h
 *
 *	@date 06/05/01 �V�K�쐬
 *	@date 06/08/30 ISerializer �����B
 *	@date 06/10/02 ���ۉ����č쐬���� IReader �������B
 *	@date 06/12/19 �V IWriter �ɍ��킹�ă��t�@�N�^�����O�B
 *	@date 09/04/24 �x�[�X�N���X�ύX�B
 *	@date 10/11/30 �N���X���\�b�h NewFile() �ǉ��B
 *	@date 11/09/16 New() �̋��L�t���O���@�\���Ă��Ȃ������̂��C���B
 */
class CFileWriter : public IWriter, public CAbstractFile
{
	DEFSUPER(IWriter);
	typedef CAbstractFile _base;
public:

	/// �R���X�g���N�^
	CFileWriter(void) : _base()
	{
	}

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void)
	{
		_base::Close();
	}

	/**
	 * [����] �V�K�I�[�v��.
	 *	@note �����̏ꍇ�A�폜���ĐV�K�ō쐬���܂��B
	 *	@param lpszName �t�@�C�����B
	 *	@param boIsShare true �Ȃ狤�L���[�h�ŃI�[�v�����܂��B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s ( GetLastError() �ɂ��ڍ׎擾�\)
	 */
	bool New(LPCTSTR lpszName, bool boIsShare = true)
	{
		return _base::OpenOnMode(lpszName, _base::CREATE, boIsShare);
	}

	/**
	 * [����] �I�[�v��.
	 *	@note �������Ȃ��Ȃ�V�K�ō쐬���܂��B
	 *	@param lpszName �t�@�C�����B
	 *	@param boIsShare �ȗ���true�Ȃ�A���L���[�h�ŃI�[�v�����܂��B
	 *	@param boIsBottom �I�[�v�����Atrue �Ȃ�t�@�C���|�C���^���Ō�Ɉړ����܂��B
	 *					false�Ȃ�擪�̂܂܂ł��B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s ( GetLastError() �ɂ��ڍ׎擾�\)
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
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return _base::GetOpenName();
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return _base::CanRead();
	}

	/**
	 * [�m�F] �����݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanWrite(void) const
	{
		return _base::CanWrite();
	}

	/**
	 * [����] �V�[�N.
	 *		�t�@�C���|�C���^���ړ����܂��B
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetFilePointer.asp
	 *	@param llOffset �t�@�C���|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�t�@�C���|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s ( GetLastError() �ɂ��ڍ׎擾�\)
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return _base::Seek(llOffset, eSeek);
	}

	/**
	 * [�擾] �t�@�C���T�C�Y�擾
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_GetFileSize.asp
	 *	@retval �O�ȏ� �t�@�C���T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		return _base::GetSize();
	}

	/**
	 * [�擾] �t�@�C���ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		return _base::Read(size, _P);
	}

	/**
	 * [�ݒ�] �t�@�C�����t�ݒ�
	 *	@param timeCreation �쐬���t
	 *	@param timeLastAccess �ŏI�A�N�Z�X���t
	 *	@param timeLastWrite �����ݓ��t
	 *	@throw CWriteFailureException �R���X�g���N�^ throw ��L���ɂ��Ă��āA�����݂Ɏ��s����ƁA�X���[����܂��B
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
	 * [�ݒ�] EOF���w��.
	 *		���݂̃t�@�C���|�C���^�̈ʒu��EOF�ɂ��܂��B
	 *	@see http://msdn.microsoft.com/library/ja/jpfileio/html/_win32_SetEndOfFile.asp
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void SetEnd(void)
	{
		if ( ! ApiSetEndOfFile(GetHandle()) )
		{
			throw CWriteFailureException(::GetLastError()); 
		}
	}

	/**
	 * [�ۑ�] �t�@�C����������
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��Bsize ���傫�߂��Ă��G���[�ɂȂ�܂��B
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
	 * [�ۑ�] �t�@�C���쐬.
	 *		�t�@�C�����ƁA���e���w�肵�A�t�@�C�����쐬���܂��B
	 *	@param lpszFileName �t�@�C�����B
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@param fw ���C�^�[�B�ʏ�ȗ����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�ۑ�] �t�@�C���ǉ�.
	 *		�t�@�C�����ƁA���e���w�肵�A�t�@�C�����쐬���܂��B
	 *	@param lpszFileName �t�@�C�����B
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@param fw ���C�^�[�B�ʏ�ȗ����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
 * [�ۑ�] BYTE�R���N�V������������
 *	@note �w�b�_�ƃ`�F�b�N�T�������ăt�@�C���֏������݂܂��B
 *	@note CByteVector�ȂǂɃV���A���C�Y�����f�[�^���t�@�C���ɏ������ގ��Ɏg�p���邱�Ƃ�z�肵�Ă��܂��B
 *	@param wr �������ݏ����̏o�����AWriter�B
 *	@param lpszHead �w�b�_���ʖ��B
 *	@param c �������݃f�[�^�B
 *	@throw CIoFailureException �t�@�C���A�N�Z�X�Ɏ��s����ƁA�X���[����܂��B
 */
inline void SaveCollection(IWriter& wr, LPCTSTR lpszHead, const ISequenceCollectionT<BYTE>& c)
{
	//- Head������
	CAscii asc = lpszHead;
	size_t dwLen = asc.GetLength() + 1;
	wr.Write(sizeof(DWORD), &dwLen);
	wr.Write(sizeof(DWORD), &dwLen);
	wr.Write(dwLen, LPCSTR(asc));
	//- SUM�v�Z
	size_t size = c.GetSize();
	WORD wSum = 0;
	const BYTE* B = c.ReferBuffer();
	for ( size_t i = 0; i < size; i++ )
	{
		wSum = static_cast<WORD>(wSum + *B++);
	}
	//- ������
	wr.Write(sizeof(size_t), &size);
	wr.Write(size, c.ReferBuffer());
	wr.Write(sizeof(WORD), &wSum);
}



/**@ingroup FILE
 * [�ۑ�] BYTE�R���N�V������������
 *	@note �w�b�_�ƃ`�F�b�N�T�������ăt�@�C���֏������݂܂��B
 *	@note CByteVector�ȂǂɃV���A���C�Y�����f�[�^���t�@�C���ɏ������ގ��Ɏg�p���邱�Ƃ�z�肵�Ă��܂��B
 *	@param lpszFile �t�@�C�����B
 *	@param lpszHead �w�b�_���ʖ��B
 *	@param c �������݃f�[�^�B
 *	@throw CIoFailureException �t�@�C���A�N�Z�X�Ɏ��s����ƁA�X���[����܂��B
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
 * [�ۑ�] �R���N�V�����ǂݍ���
 *	@note SaveCollection() �ŏ������񂾂��̂�ǂݍ��݂܂��B
 *	@param[in] rr �ǂݍ��ݏ������o���� Reader�B
 *	@param[in] lpszHead �w�b�_���ʖ��B
 *	@param[out] _vb �ǂݍ��݁B
 *	@throw CNotSupportException		�w�b�_�`�󂪈قȂ�ƃX���[����܂��B
 *	@throw CInvalidParamException	�t�@�C���������قȂ�ƃX���[����܂��B
 *	@throw CNoSuchTypeException		�`�F�b�N�T�����قȂ�ƃX���[����܂��B
 *	@throw CIoFailureException		�t�@�C���A�N�Z�X�Ɏ��s����ƁA�X���[����܂��B
 */
inline void LoadCollection(const IReader& rr, LPCTSTR lpszHead, CVectorT<BYTE>& _vb)
{
	//- Head�ǂݍ���
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
	//- SUM�v�Z
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
 * [�ۑ�] �R���N�V�����ǂݍ���
 *	@note SaveCollection() �ŏ������񂾂��̂�ǂݍ��݂܂��B
 *	@param[in] lpszFile �t�@�C�����B
 *	@param[in] lpszHead �w�b�_���ʖ��B
 *	@param[out] _vb �ǂݍ��݁B
 *	@throw CNotSupportException		�w�b�_�`�󂪈قȂ�ƃX���[����܂��B
 *	@throw CInvalidParamException	�t�@�C���������قȂ�ƃX���[����܂��B
 *	@throw CNoSuchTypeException		�`�F�b�N�T�����قȂ�ƃX���[����܂��B
 *	@throw CIoFailureException		�t�@�C���A�N�Z�X�Ɏ��s����ƁA�X���[����܂��B
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
