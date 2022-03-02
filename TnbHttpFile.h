#pragma once
/**
 *@file
 * HTTP�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbInternetHandle.h"
#include "TnbReader.h"



//TNB Library
namespace TNB
{



/**@ingroup INTERNET FILE
 * HTTP�t�@�C���ǂݍ��݃N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbHttpFile.h
 *
 *	@date 06/10/13 �V�K
 */
class CHttpFileReader : public IReader
{
	CInternetHandle m_hWininet;	///< �Z�b�V�����n���h��
	CInternetHandle m_hFile;	///< �P�t�@�C���n���h��
	CStr			m_strName;	///< �I�[�v����

public:
	
	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_strName;
	}

	/**
	 * [�ݒ�] �t�@�C���I�[�v��.
	 *		�w�肳�ꂽ�t�@�C�����I�[�v������B 
	 *	@param lpszAgent	HTTP�ɂ��C���^�[�l�b�g�ɃA�N�Z�X����ۂ̃G�[�W�F���g���B
	 *	@param lpszUrl		�I�[�v������t�@�C���̖��O�B
	 *	@param dwContext	�R�[���o�b�N�ɓn���A�v���P�[�V������`�̒l�B�ȗ��\�B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s
	 */
	bool Open(LPCTSTR lpszAgent, LPCTSTR lpszUrl, DWORD_PTR dwContext = 0)
	{
		Close();
		if ( ! m_hWininet.Open(lpszAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0) )
		{
			return false;
		}
		const DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD;
		m_hFile = m_hWininet.OpenUrl(lpszUrl, NULL, 0, dwFlags, dwContext);
		m_strName = lpszUrl;
		return m_hFile.IsValid();
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
	 * [�擾] �t�@�C���ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		DWORD dwRead;
		if ( m_hFile.ReadFile(_P, size, &dwRead) )
		{
			return dwRead;
		}
		throw CReadFailureException(::GetLastError());
	}

	/**
	 * [����] �V�[�N.
	 *		�t�@�C���|�C���^���ړ����܂��B
	 *	@param llOffset �t�@�C���|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�t�@�C���|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		LARGE_INTEGER r;
		r.QuadPart = llOffset;
		if ( r.HighPart == 0 )
		{
			r.LowPart = m_hFile.SetFilePointer(r.LowPart, 0, eSeek, 0);
			if ( r.LowPart == INVALID_SET_FILE_POINTER )
			{
				r.QuadPart = -1;
			}
		}
		else
		{
			r.QuadPart = -1;
		}
		return r.QuadPart;
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		LONGLONG l = Seek(0, CURRENT);
		LONGLONG r = Seek(0, END);
		if ( l >= 0 )
		{
			Seek(l, TOP);
		}
		return r;
	}

	/**
	 * [�ݒ�] �t�@�C���N���[�Y.
	 */
	void Close(void)
	{
		m_hFile.Close();
		m_hWininet.Close();
	}

	/**
	 * [�擾] �t�@�C���n���h���擾
	 *	@note Open() �Ŏ擾�����t�@�C���̃n���h�����Ԃ�܂��B�N���[�Y���Ă���ƁANULL�̃n���h�����Ԃ�܂��B 
	 *	@return �n���h��
	 */
	CInternetHandle& GetHandle(void)
	{
		return m_hFile;
	}
};



}; // TNB


 
