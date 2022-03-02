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



#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �I�[�o�[���b�v���͊Ǘ��N���X.
 *
 *		API ::ReadFile() �ɂ��񓯊��̓Ǎ��݂��T�|�[�g���܂��BHID�ȂǏ�񂪂Ȃ����Ƀ��b�N������f�o�C�X����̓Ǎ��݂ɗL���ł��B
 *		
 *
 *	@par�K�v�t�@�C��
 *			TnbOverlapReader.h
 *
 *	@date 07/09/13 �V�K�쐬
 *	@date 08/12/19 IsReading() �V�K�B
 *	@date 09/12/21 �r���A�ǉ��B
 */
class COverlapReader : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	COverlapReader(void) : m_hFile(INVALID_HANDLE_VALUE), m_hEvent(NULL), m_isOverLapping(false)
	{
		Zero(m_overLapped);
	}

	/// �f�X�g���N�^
	~COverlapReader(void)
	{
		Detach();
		if ( m_hEvent != NULL )
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL; 
		}
	}

	/**
	 * [�ݒ�] �A�^�b�`.
	 *	@param[in] hFile FILE_FLAG_OVERLAPPED �t���ŃI�[�v�������n���h���B
	 *	@param[in] size �T�C�Y�B
	 */
	void Attach(HANDLE hFile, size_t size)
	{
		EXCLUSIVE( &m_sync );
		m_Cancel();
		m_hFile = hFile;
		m_work.Resize(size);
	}

	/**
	 * [�ݒ�] �f�^�b�`.
	 */
	void Detach(void)
	{
		EXCLUSIVE( &m_sync );
		m_Cancel();
		m_hFile = INVALID_HANDLE_VALUE;
		m_work.Free();
	}

	/**
	 * [�Ǎ�] �Ǎ���.
	 *		Event��Wait�̕t���� ::ReadFile �ł��B
	 *	@param[out] _P �ǂݍ��ރ|�C���^�����w�肵�܂��B Attach() �Ŏw�肵���T�C�Y���K�v�ł��B
	 *	@param[in] wait �҂�����(ms)�B INFINITE ���w�肷��ƁA���ʂ��A��܂� Wait���܂��B
	 *	@retval INVALID_SIZE �G���[�B ::GetLastError() �ŃG���[�ڍׂ��킩��܂��B
	 *	@retval 0 �f�[�^�����B
	 *	@retval ��L�ȊO �����B���l�͓Ǎ��݃T�C�Y�B
	 */
	size_t Read(LPVOID _P, DWORD wait = 0)
	{
		EXCLUSIVE( &m_sync );
		size_t r = 0;
		if ( m_hFile != INVALID_HANDLE_VALUE && m_work.GetSize() > 0 )
		{
			if ( m_isOverLapping )
			{
				// �ǂݍ��ݑҋ@��
				r = m_GetResult(wait);
			}
			else
			{
				// �V�K
				r = m_Read(m_work.GetSize(), m_work.Ref());
				if ( r == 0 )
				{
					r = m_GetResult(wait);
				}
			}
			if ( r != INVALID_SIZE && r != 0 )
			{
				memcpy(_P, m_work.Ref(), m_work.GetSize());
			}
		}
		return r;
	}

protected:

	/**
	 * [�ʒm] �Ǎ���.
	 *	@param[in] hFile �n���h���B
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �Ǎ��݃o�b�t�@�B
	 *	@param[in,out] overLapped �I�[�o�[���b�v���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnRead(HANDLE hFile, LPVOID _P, size_t size, OVERLAPPED& overLapped)
	{
		DWORD d;
		bool r = !! ::ReadFile(hFile, _P, size, &d, &overLapped);
		return r && (size == d); 
	}

private:

	/**
	 * [�Ǎ�] �Ǎ���.
	 *	@param[in] hFile FILE_FLAG_OVERLAPPED �t���ŃI�[�v�������n���h���B
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �Ǎ��݃o�b�t�@�B
	 *	@retval 0 �񓯊��Ǎ��ݒ��B
	 *	@retval INVALID_SIZE �ǂݍ��݃G���[�B
	 *	@retval ��L�ȊO �Ǎ��ݐ����B
	 */
	size_t m_Read(size_t size, LPVOID _P)
	{
		m_Cancel();
		Zero(m_overLapped);
		if ( m_hEvent == NULL )
		{
			m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
			::ResetEvent(m_hEvent);
		}
		m_overLapped.hEvent = m_hEvent; 
		DWORD work = ULONG_MAX;
		if ( OnRead(m_hFile, _P, size, m_overLapped) )
		{
			// ����
			return size;
		}
		else if ( ::GetLastError() == ERROR_IO_PENDING )
		{
			// �񓯊��ǂݎ�葀��̌��ʂ��`�F�b�N����B
			::Sleep(0);
			if( ::GetOverlappedResult(m_hFile, &m_overLapped, &work, FALSE) )
			{
				return work;
			}
			else if ( ::GetLastError() == ERROR_IO_INCOMPLETE )
			{
				m_isOverLapping = true;
				return 0;
			}
		}
		return INVALID_SIZE;
	}
	/**
	 * [�擾] ���ʎ擾.
	 *	@param[in] wait �҂�����(ms)�B INFINITE ���w�肷��ƁA���ʂ��A��܂� Wait���܂��B
	 *	@retval 0 �^�C���A�E�g�i�܂����ʖ����j�B
	 *	@retval INVALID_SIZE �ǂݍ��݃G���[�B
	 *	@retval ��L�ȊO �Ǎ��ݐ����B
	 */
	size_t m_GetResult(DWORD wait = 0)
	{
		if ( ! m_isOverLapping )
		{
			return INVALID_SIZE;
		}
		size_t result = INVALID_SIZE;
		DWORD r = 0;
		switch ( ::WaitForSingleObject(m_overLapped.hEvent, wait) )
		{
		case WAIT_OBJECT_0:
			if ( ::GetOverlappedResult(m_hFile, &m_overLapped, &r, FALSE) )
			{
				result = r; // ����
				m_Cancel();
			}
			break;
		case WAIT_TIMEOUT:
			result = 0;
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
		default:
			m_Cancel();
			break;
		}
		return result;
	}
	/**
	 * [�擾] ���ʎ擾�L�����Z��.
	 *	@note �ȍ~ m_GetResult() �͎��s���܂��B
	 */
	void m_Cancel(void)
	{
		if ( m_isOverLapping )
		{
			::CancelIo(m_hFile);
			m_isOverLapping = false;
		}
	}
	HANDLE			m_hEvent;		///< �C�x���g�n���h��
	bool			m_isOverLapping;///< �I�[�o�[���b�v��
	OVERLAPPED		m_overLapped;	///< �I�[�o�[���b�v�Ǘ�
	HANDLE			m_hFile;		///< �Ǎ��݃n���h��
	CWorkMem		m_work;			///< �Ǎ��݃��[�N
	CSyncSection	m_sync;
};



/**@ingroup WINAPI
 * �I�[�o�[���b�v���͊Ǘ��N���X.
 *
 *		API ::DeviceIoControl() �ɂ��񓯊��̓Ǎ��݂��T�|�[�g���܂��B�ǂݍ��ޏ�񂪂Ȃ����Ƀ��b�N������f�o�C�X����̓Ǎ��݂ɗL���ł��B
 *		
 *	@par�K�v�t�@�C��
 *			TnbOverlapReader.h
 *
 *	@date 07/09/13 �V�K�쐬
 *	@date 08/12/19 IsReading() �V�K�B
 */
class COverlapIoReader : public COverlapReader
{
	DEFSUPER(COverlapReader);
public:

	/// �R���X�g���N�^
	COverlapIoReader(void) : _super(), m_dwIoControlCode(0)
	{
	}

	/**
	 * [�ݒ�] �R���g���[���R�[�h�ݒ�.
	 *	@note DeviceIoControl() �� ���������w��ł��܂��B
	 *	@param code �R���g���[���R�[�h
	 */
	void SetIoControlCode(DWORD code)
	{
		m_dwIoControlCode = code;
	}

protected:
	/**
	 * [�ʒm] �Ǎ���.
	 *	@param[in] hFile �n���h���B
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �Ǎ��݃o�b�t�@�B
	 *	@param[in,out] overLapped �I�[�o�[���b�v���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnRead(HANDLE hFile, LPVOID _P, size_t size, OVERLAPPED& overLapped)
	{
		DWORD dwByteReturn;
		BOOL ret = ::DeviceIoControl(hFile, m_dwIoControlCode, _P, size, _P, size, &dwByteReturn, &overLapped);
		return !! ret;
	}
private:
	DWORD m_dwIoControlCode;		///< DeviceIoControl�̑�����
};



}; // TNB
