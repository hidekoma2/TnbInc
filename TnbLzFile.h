#pragma once
/**
 *@file
 * LZ�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbReader.h"



#include <lzexpand.h>
#pragma comment(lib,"lz32.lib")



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * LZ�t�@�C���ǂݍ��݃N���X
 *
 *		�t�@�C����̓ǂݍ��݂��T�|�[�g����N���X�ł��B
 *		Microsoft �t�@�C�����k���[�e�B���e�B�iCOMPRESS.EXE�j�ň��k���ꂽ�t�@�C�����ǂݍ��߂܂��B
 *
 *	@todo ���k���ꂽ�t�@�C���̓W�J���삪���m�F�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbLzFile.h
 *
 *	@date 07/01/22 �V�K�쐬
 */
class CLzFileReader : public IReader
{
	INT		m_iHandle;		///< �n���h��
	CStr	m_strName;		///< �t�@�C����

	/// �N���[�Y
	void m_Close(void)
	{
		m_strName.Empty();
		if ( m_iHandle >= 0 )
		{
			::LZClose(m_iHandle);
			m_iHandle = -1;
		}
	}

public:

	/// �R���X�g���N�^
	CLzFileReader(void) : m_iHandle(-1)
	{
	}

	/// �f�X�g���N�^
	virtual ~CLzFileReader(void)
	{
		m_Close();
	}

	/**
	 * [�ݒ�] �I�[�v��
	 *	@param lpszLzFile Compress.exe�ň��k���ꂽ�t�@�C�������A�ʏ�̃t�@�C�����B
	 *	@param boIsShare �ȗ���true�Ȃ�A���L���[�h�ŃI�[�v�����܂��B
	 *	@retval true �I�[�v������
	 *	@retval false �I�[�v�����s
	 */
	bool Open(LPCTSTR lpszLzFile, bool boIsShare = true)
	{
		CStr s = lpszLzFile;
		Close();
		CWorkMemT<OFSTRUCT> work;
		INT r = ::LZOpenFile(
					s.GetBuffer(), work, static_cast<WORD>(OF_READ | (boIsShare ? OF_SHARE_DENY_NONE : OF_SHARE_EXCLUSIVE)));
		if ( r >= 0 )
		{
			m_strName = lpszLzFile;
			m_iHandle = r;
		}
		return r >= 0;
	}

	/**
	 * [����] �R�s�[
	 *	@note �R�s�[���́A Open() �Ŏw�肵�Ă��������B
	 *	@param lpszDstFile ������t�@�C�����B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Copy(LPCTSTR lpszDstFile)
	{
		if ( ! CanRead() ){ return false; }
		CWorkMemT<OFSTRUCT> work;
		CStr s = lpszDstFile;
		INT r = ::LZOpenFile(s.GetBuffer(), work, OF_CREATE);
		if ( r >= 0 )
		{
			if ( ::LZCopy(m_iHandle, r) >= 0 )
			{
				return true;
			}
		}		
		return false;
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_strName;
	}

	/**
	 * [����] �N���[�Y
	 *	@note �g�p���Ă����e���|�����t�@�C�����폜�B
	 */
	virtual void Close(void)
	{
		m_Close();
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return m_iHandle >= 0;
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		LONGLONG c = Seek(0, CURRENT);
		LONGLONG r = Seek(0, END);
		Seek(c, TOP);
		return r;
	}

	/**
	 * [����] �V�[�N.
	 *		�A�N�Z�X�|�C���^���ړ����܂��B
	 *	@param llOffset �A�N�Z�X�|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�A�N�Z�X�|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		 return ::LZSeek(m_iHandle, ToInt(llOffset), eSeek);
	}

	 /**
	 * [�擾] �ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B iSize ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		INT r = ::LZRead(m_iHandle, static_cast<char *>(_P), ToInt(size));
		if ( r < 0 )
		{
			throw CReadFailureException(ERROR_ACCESS_DENIED);
		}
		return r;
	}

	/**
	 * [�擾] ���k�O���O�擾
	 *	@param lpszLzFile Compress.exe�ň��k���ꂽ�t�@�C�������A�ʏ�̃t�@�C�����B
	 *	@retval Empty������ �G���[�B
	 *	@retval �������� �t�@�C�����B
	 */
	static CStr GetExpandedName(LPCTSTR lpszLzFile)
	{
		CStr strSrc = lpszLzFile;
		CStr strDst;
		if ( ::GetExpandedName(strSrc.GetBuffer(), strDst.GetBuffer(MAX_PATH)) >= 0 )
		{
			strDst.ReleaseBuffer();
			return strDst;
		}
		return CStr();
	}
};



}; // TNB
