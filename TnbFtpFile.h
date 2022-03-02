#pragma once
/**
 *@file
 * FTP �t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbInternetHandle.h"
#include "TnbWriter.h"
#include "TnbDummyCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup INTERNET FILE
 * FTP�Z�b�V�����Ǘ��N���X
 *
 *		�T�[�o�ڑ��ƃJ�����g�f�B���N�g�����Ǘ����܂��B�J�����g�f�B���N�g�����
 *		����i�f���[�g�A���l�[���Ȃǁj���T�|�[�g���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFtpFile.h
 *
 *	@date 06/10/03 �V�K
 *	@date 12/02/09 CSftpSession �ƃ��\�b�h�������킹��
 */
class CFtpSession
{
public:

	///�R���X�g���N�^
	CFtpSession(void)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CFtpSession(const CFtpSession& other) : m_hWininet(other.m_hWininet), m_hSession(other.m_hSession) 
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CFtpSession& operator=(const CFtpSession& other)
	{
		m_hWininet = other.m_hWininet;
		m_hSession = other.m_hSession;
		return *this;
	}

	/**
	 * [�m�F] �n���h���L���m�F
	 *	@retval ture �L���B
	 *	@retval false ����(�J���Ă��Ȃ����A���s������)�B
	 */
	bool IsValid(void) const
	{
		return m_hSession.IsValid();
	}

	/**
	 * [�ݒ�] �I�[�v��.
	 *		WININET�Ăяo���p�̃n���h���ƁA�w��T�[�o�̃Z�b�V�����n���h�����쐬���܂��B 
	 *		�{�N���X�̃��\�b�h���g�p����ɂ́A�܂��I�[�v������K�v������܂��B
	 *	@param lpszAgent		HTTP�ɂ��C���^�[�l�b�g�ɃA�N�Z�X����ۂ̃G�[�W�F���g���B
	 *	@param lpszServerName	�ڑ���̃z�X�g���܂��̓h�b�g�\�L��IP�A�h���X�B
	 *	@param lpszUserName		�ڑ��̍ۂɎg�p����郆�[�U���BNULL���w�肵���ꍇ�f�t�H���g(FTP�Ȃ�"anonymous")���g�p�����B
	 *	@param lpszPassword		�ڑ��̍ۂɎg�p�����p�X���[�h�BUserName��Password������NULL�̏ꍇ�APassword��e-mail�A�h���X���g�p�����B
	 *	@retval true �����B�{�N���X�̃n���h�����L���ɂȂ�B
	 *	@retval false ���s�B�{�N���X�̃n���h���͖����ɂȂ�B
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
	 * [�ݒ�] �N���[�Y.
	 *		WININET�Ăяo���p�̃n���h���ƁA�w��T�[�o�̃Z�b�V�����n���h������܂��B 
	 */
	void Close(void)
	{
		m_hWininet.Close();
		m_hSession.Close();
	}

	/**
	 * [�擾] �Z�b�V�����n���h���擾
	 *	@return �n���h��
	 */
	operator CInternetHandle&(void)
	{
		return m_hSession;
	}

	/**
	 * [�쐬] �����[�g�f�B���N�g���쐬.
	 *	@param lpszRemoteDir	�쐬����f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool CreateRemoteDir(LPCTSTR lpszRemoteDir)
	{
		return m_hSession.CreateDirectory(lpszRemoteDir);
	}

	/**
	 * [�폜] �����[�g�f�B���N�g���폜.
	 *	@param lpszRemoteDir	�폜����f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool RemoveRemoteDir(LPCTSTR lpszRemoteDir)
	{
		return m_hSession.RemoveDirectory(lpszRemoteDir);
	}

	/**
	 * [�擾] �J�����g�����[�g�f�B���N�g���擾.
	 *		FTP�Z�b�V�������ł̃J�����g�f�B���N�g�����擾����B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param[out]		_lpszCurrentRemoteDir	�J�����g�f�B���N�g���B
	 *	@param[in,out]	_length		_lpszCurrentRemoteDir �̃T�C�Y�B�֐��̎��s��A�ݒ肳�ꂽ�f�[�^�̃T�C�Y�܂��͕K�v�ȃT�C�Y���Ԃ����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool GetCurrentRemoteDir(LPTSTR _lpszCurrentRemoteDir, DWORD& _length)
	{
		return m_hSession.GetCurrentDirectory(_lpszCurrentRemoteDir, &_length);
	}

	/**
	 * [�ݒ�] �J�����g�����[�g�f�B���N�g���ݒ�.
	 *	@param lpszRemoteDir	�ύX��̃f�B���N�g�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetCurrentRemoteDir(LPCTSTR lpszRemoteDir)
	{
		return m_hSession.SetCurrentDirectory(lpszRemoteDir);
	}	

	/**
	 * [�擾] �����[�g�t�@�C���擾.
	 *	@param lpszNewLocalFile	�擾��̃��[�J���t�@�C�����B
	 *	@param lpszRemoteFile	�擾���̃����[�g�t�@�C�����B
	 *	@param fFailIfExists	lpszNewLocalFile �Ŏw�肳�ꂽ�t�@�C�������Ƀ��[�J���ɑ��݂���Ƃ��ɁA�G���[�ɂ��邩�ۂ���\���^�U�l�B
	 *	@param 	dwFlagsAndAttributes	�]�����ʂ̃t�@�C���ɕt�^���鑮���BFILE_ATTRIBUTE_*���w��\�B�ڍׂ�CreateFile���Q�ƁB
	 *	@param 	dwFlags	�t�@�C���̎擾�̕��@���w�肷��t���O�B��q�̕\���Q�ƁB
	 *					<BR>		�܂����̂����ꂩ�̃t���O���w�肷��B
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	�\���`����ASCII�Ƃ��ē]�����s���B
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	�\���`����IMAGE�Ƃ��ē]�����s���B
	 *					<BR>		FTP_TRANSFER_TYPE_UNKNOWN	�f�t�H���g�̓]�����@���g�p����B(�o�C�i���]��)
	 *					<BR>		INTERNET_FLAGS_TRANSFER_ASCII	ASCII�t�@�C����]������B
	 *					<BR>		INTERNET_FLAGS_TRANSFER_BINARY	�o�C�i���t�@�C����]������B
	 *					<BR>
	 *					<BR>		���ɁA�ȉ��̃t���O�̑g�ݍ��킹��_���a�Ŏw�肷��B 
	 *					<BR>		INTERNET_FLAG_DONT_CACHE	
	 *					<BR>		INTERNET_FLAG_HYPERLINK	
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	
	 *					<BR>		INTERNET_FLAG_MUST_CACHE_REQUEST	
	 *					<BR>		INTERNET_FLAG_NEED_FILE	
	 *					<BR>		INTERNET_FLAG_NO_CACHE_WRITE	
	 *					<BR>		INTERNET_FLAG_RELOAD	
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	
	 *	@param 	dwContext	�R�[���o�b�N�֐��֓n��32bit�l�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool GetRemoteFile(LPCTSTR lpszNewLocalFile, LPCTSTR lpszRemoteFile, BOOL fFailIfExists, DWORD dwFlagsAndAttributes, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return m_hSession.GetFile(lpszRemoteFile, lpszNewLocalFile, fFailIfExists, dwFlagsAndAttributes, dwFlags, dwContext);
	}

	/**
	 * [���M] ���[�J���t�@�C�����M.
	 *	@param lpszNewRemoteFile	�]����̃����[�g�t�@�C�����B
	 *	@param lpszLocalFile	�]�����̃��[�J���t�@�C�����B
	 *	@param dwFlags	�]�����@���w�肷��t���O�B��q�̕\���Q�ƁB
	 *					<BR>		�ȉ��̃t���O�̑g�ݍ��킹�ł���B 
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
	 *	@param dwContext	�A�v���P�[�V������`�̃R�[���o�b�N�֐��ɓn��32bit�l�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool PutLocalFile(LPCTSTR lpszNewRemoteFile, LPCTSTR lpszLocalFile, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return m_hSession.PutFile(lpszLocalFile, lpszNewRemoteFile, dwFlags, dwContext);
	}

	/**
	 * [�폜] �����[�g�t�@�C���폜.
	 *	@param lpszRemoteFile	�폜����t�@�C���̃t�@�C�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool DeleteRemoteFile(LPCTSTR lpszRemoteFile)
	{
		return m_hSession.DeleteFile(lpszRemoteFile);
	}

	/**
	 * [�ύX] ���O�ύX.
	 *		�����[�g�t�@�C�����A�����[�g�f�B���N�g�����̕ύX���o���܂��B
	 *	@param lpszOldName	���̃����[�g�t�@�C�����B
	 *	@param lpszNewName	�ύX��̃����[�g�t�@�C�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	*/
	bool RenameRemoteFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
	{
		return m_hSession.RenameFile(lpszOldName, lpszNewName);
	}

private:
	CInternetHandle m_hWininet;		///< WININET �n���h��
	CInternetHandle m_hSession;		///< �Z�b�V�����n���h��
};



/**@ingroup INTERNET FILE
 * FTP�t�@�C���ǂݍ��݃N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbFtpFile.h
 *
 *	@date 07/01/15 �V�K
 */
class CFtpFileReader : public IReader
{
	DEFSUPER(IReader);
	mutable CFtpSession				m_hSession;		///< �Z�b�V�����n���h��
	mutable CPointerHandleT<DWORD>	m_phdwPointer;	///< �t�@�C���|�C���^
	mutable CInternetHandle			m_hFile;		///< �P�t�@�C���n���h��
	CStr							m_strFileName;	///< Open�t�@�C����
	DWORD							m_dwAccess;		///< Open�A�N�Z�X
	DWORD							m_dwFlags;		///< Open�t���O
	DWORD_PTR						m_dwContext;	///< Open�R���e�L�X�g
	friend class CFtpFileWriter;					///< �t�����h�N���X�錾

	/**
	 * �w��T�C�Y�󍞂݂���
	 *	@note FilePointer �� ���炷�̂��ړI
	 *	@retval �}�C�i�X �G���[
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�ǂݍ��߂��T�C�Y�B
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
	 * [�ݒ�] �|�C���^�ݒ�
	 *	@param dw �ݒ肷�鐔
	 */
	void m_SetFilePointer(DWORD dw) const
	{
		m_phdwPointer = new DWORD(dw);
	}

	/**
	 * [�擾] �|�C���^�擾
	 *	@return �|�C���^
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
	 * [�ݒ�] Open�p�ݒ�
	 *	@param lpszFileName	�I�[�v������t�@�C���̖��O�B
	 *	@param dwAccess �A�N�Z�X�t���O�B
	 *	@param dwFlags	�t�@�C���]���̎�ʂ�\���t���O�B�B
	 *	@param dwContext	�R�[���o�b�N�ɓn���A�v���P�[�V������`�̒l�B�ȗ��\�B
	 */
	void m_SetParamForOpen(LPCTSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext)
	{
		m_strFileName	= lpszFileName;
		m_dwAccess		= GENERIC_READ;
		m_dwFlags		= dwFlags;
		m_dwContext		= dwContext;
	}

	/**
	 * [�ݒ�] �t�@�C���I�[�v��
	 *	@note SetParamForOpen() �ɂ���Đݒ肳�ꂽ���e�� Open���܂��B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s
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
	 * [�ݒ�] �|�C���^�C���N�������g
	 *	@param dw ������
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
	 * [�ݒ�] �t�@�C���I�[�v��.
	 *		FTP�Z�b�V������FileName�Ŏw�肳�ꂽ�t�@�C�����I�[�v������B 
	 *	@param sec			�I�[�v�������Z�b�V�����n���h���B
	 *	@param lpszFileName	�I�[�v������t�@�C���̖��O�B
	 *	@param dwFlags	�t�@�C���]���̎�ʂ�\���t���O�B�ȗ������ FTP_TRANSFER_TYPE_BINARY �B
	 *					<BR>		�܂��ȉ��̂����ꂩ���w�肷��B 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII�]���B
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	�o�C�i���]���B
	 *					<BR>
	 *					<BR>		���ɁA�ȉ��̔C�ӂ̑g�ݍ��킹��_���a�ɂ���Ďw�肷��B 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	�ēǂݍ��݂��ׂ����ǂ����̔��f���ɗL�������E�ŏI�X�V�������T�[�o����Ԃ���Ȃ��ꍇ�����I�ɍēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	�L���b�V���ɉi���I�v�f�Ƃ��Ēǉ�����B����ɂ��A�ʏ�̃L���b�V���̃N���[���A�b�v�A�������`�F�b�N�A���݂͍s���Ȃǂ̍ۂɂ��̗v�f���폜����Ȃ��B
	 *					<BR>		INTERNET_FLAG_RELOAD	�v�������t�@�C���A�I�u�W�F�N�g�A�f�B���N�g�����X�g���L���b�V������擾����̂łȂ��A�����I�ɃT�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTP���\�[�X���T�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_NEED_FILE	�t�@�C�����L���b�V���ł��Ȃ��ꍇ�Ɉꎞ�t�@�C�����쐬����B
	 *	@param dwContext	�R�[���o�b�N�ɓn���A�v���P�[�V������`�̒l�B�ȗ��\�B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s
	 */
	bool Open(CFtpSession& sec, LPCTSTR lpszFileName, DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 0)
	{
		Close();
		m_hSession = sec;
		m_SetParamForOpen(lpszFileName, GENERIC_READ, dwFlags, dwContext);
		return m_Reopen();
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
		DWORD dwRead;
		if ( m_hFile.ReadFile(_P, ToDword(size), &dwRead) )
		{
			m_AddFilePointer(dwRead);
			return dwRead;
		}
		throw CReadFailureException(::GetLastError());
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	LPCTSTR GetOpenName(void) const
	{
		return m_strFileName;
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return m_hFile.IsValid();
	}

	/**
	 * [����] �V�[�N.
	 *		�t�@�C���|�C���^���ړ����܂��B
	 *	@attention	�t�@�C���|�C���^�̈ړ��́A��ǂݍ��݂��邱�ƂŁA�s�Ȃ��Ă��܂��B
	 *				���ׁ̈A�p�t�H�[�}���X�͗ǂ�����܂���B�܂��A���݂̃|�C���^���O�ɖ߂�ꍇ�A
	 *				�ăI�[�v������܂��B
	 *	@param llOffset �t�@�C���|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�t�@�C���|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
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
			INT_PTR i = r - current;	//���݂̈ʒu���琳���̕����H
			if ( i < 0 )
			{
				//== ���Ȃ�Reopen����B
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
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
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
	 * [�ݒ�] �t�@�C���N���[�Y.
	 */
	void Close(void)
	{
		m_hFile.Close();
		m_hSession.Close();
	}
};



/**@ingroup INTERNET FILE
 * FTP�t�@�C���������݃N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbFtpFile.h
 *
 *	@date 07/01/16 �V�K
 */
class CFtpFileWriter : public IWriter
{
	DEFSUPER(IWriter);
	CFtpFileReader		m_ftp;	///< �A�N�Z�X�Ǘ�
public:

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void)
	{
		m_ftp.Close();
	}

	/**
	 * [�擾] �t�@�C���I�[�v��.
	 *		FTP�Z�b�V������FileName�Ŏw�肳�ꂽ�t�@�C�����������݉\�ŃI�[�v������B 
	 *	@param sec			�I�[�v�������Z�b�V�����n���h���B
	 *	@param lpszFileName	�I�[�v������t�@�C���̖��O�B
	 *	@param dwFlags	�t�@�C���]���̎�ʂ�\���t���O�B�ȗ������ FTP_TRANSFER_TYPE_BINARY �B
	 *					<BR>		�܂��ȉ��̂����ꂩ���w�肷��B 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII�]���B
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	�o�C�i���]���B
	 *					<BR>
	 *					<BR>		���ɁA�ȉ��̔C�ӂ̑g�ݍ��킹��_���a�ɂ���Ďw�肷��B 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	�ēǂݍ��݂��ׂ����ǂ����̔��f���ɗL�������E�ŏI�X�V�������T�[�o����Ԃ���Ȃ��ꍇ�����I�ɍēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	�L���b�V���ɉi���I�v�f�Ƃ��Ēǉ�����B����ɂ��A�ʏ�̃L���b�V���̃N���[���A�b�v�A�������`�F�b�N�A���݂͍s���Ȃǂ̍ۂɂ��̗v�f���폜����Ȃ��B
	 *					<BR>		INTERNET_FLAG_RELOAD	�v�������t�@�C���A�I�u�W�F�N�g�A�f�B���N�g�����X�g���L���b�V������擾����̂łȂ��A�����I�ɃT�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTP���\�[�X���T�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_NEED_FILE	�t�@�C�����L���b�V���ł��Ȃ��ꍇ�Ɉꎞ�t�@�C�����쐬����B
	 *	@param dwContext	�R�[���o�b�N�ɓn���A�v���P�[�V������`�̒l�B�ȗ��\�B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s
	 */
	bool Open(CFtpSession& sec, LPCTSTR lpszFileName, DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 0)
	{
		Close();
		m_ftp.m_hSession = sec;
		m_ftp.m_SetParamForOpen(lpszFileName, GENERIC_READ | GENERIC_WRITE, dwFlags, dwContext);
		return m_ftp.m_Reopen();
	}

	/**
	 * [�擾] �t�@�C���I�[�v��.
	 *		FTP�Z�b�V������FileName�Ŏw�肳�ꂽ�t�@�C����V�K�쐬����B 
	 *	@param sec			�I�[�v�������Z�b�V�����n���h���B
	 *	@param lpszFileName	�I�[�v������t�@�C���̖��O�B
	 *	@param dwFlags	�t�@�C���]���̎�ʂ�\���t���O�B�ȗ������ FTP_TRANSFER_TYPE_BINARY �B
	 *					<BR>		�܂��ȉ��̂����ꂩ���w�肷��B 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII�]���B
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	�o�C�i���]���B
	 *					<BR>
	 *					<BR>		���ɁA�ȉ��̔C�ӂ̑g�ݍ��킹��_���a�ɂ���Ďw�肷��B 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	�ēǂݍ��݂��ׂ����ǂ����̔��f���ɗL�������E�ŏI�X�V�������T�[�o����Ԃ���Ȃ��ꍇ�����I�ɍēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	�L���b�V���ɉi���I�v�f�Ƃ��Ēǉ�����B����ɂ��A�ʏ�̃L���b�V���̃N���[���A�b�v�A�������`�F�b�N�A���݂͍s���Ȃǂ̍ۂɂ��̗v�f���폜����Ȃ��B
	 *					<BR>		INTERNET_FLAG_RELOAD	�v�������t�@�C���A�I�u�W�F�N�g�A�f�B���N�g�����X�g���L���b�V������擾����̂łȂ��A�����I�ɃT�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTP���\�[�X���T�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_NEED_FILE	�t�@�C�����L���b�V���ł��Ȃ��ꍇ�Ɉꎞ�t�@�C�����쐬����B
	 *	@param dwContext	�R�[���o�b�N�ɓn���A�v���P�[�V������`�̒l�B�ȗ��\�B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s
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
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	LPCTSTR GetOpenName(void) const
	{
		return m_ftp.m_strFileName;
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return m_ftp.m_hFile.IsValid();
	}

	/**
	 * [�m�F] �����݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanWrite(void) const
	{
		return CanRead() && (m_ftp.m_dwAccess & GENERIC_WRITE) != 0;
	}

	/**
	 * [�擾] �ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		return m_ftp.Read(size, _P);
	}

	/**
	 * [�ݒ�] EOF���w��.
	 *	@throw CNotSupportException �T�|�[�g���Ă��Ȃ����߁A�X���[����܂��B
	 */
	virtual void SetEnd(void)
	{
		throw CNotSupportException();
	}

	/**
	 * [�ۑ�] �t�@�C����������
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
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
	 * [����] �V�[�N.
	 *		�t�@�C���|�C���^���ړ����܂��B
	 *	@attention	�t�@�C���|�C���^�̈ړ��́A��ǂݍ��݂��邱�ƂŁA�s�Ȃ��Ă��܂��B
	 *				���ׁ̈A�p�t�H�[�}���X�͗ǂ�����܂���B�܂��A���݂̃|�C���^���O�ɖ߂�ꍇ�A
	 *				�ăI�[�v������܂��B
	 *	@param llOffset �t�@�C���|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�t�@�C���|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return m_ftp.Seek(llOffset, eSeek);
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		return m_ftp.GetSize();
	}
};



}; // TNB


 
