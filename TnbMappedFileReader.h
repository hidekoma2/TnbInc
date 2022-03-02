#pragma once
/**
 *@file
 * �}�b�v�h�t�@�C���Ǎ��݊֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbReader.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �}�b�v�h�t�@�C�����[�_�[�N���X
 *
 *		�t�@�C�����������Ƀ}�b�v���A�N�Z�X�ł��܂��i�|�C���^�ŎQ�Ɖ\�j�B
 *
 *	@par�K�v�t�@�C��
 *			TnbMappedFileReader.h
 *
 *	@date 08/08/21 �V�K�쐬
 */
class CMappedFileReader : public IReader
{
public:

	/// �R���X�g���N�^
	CMappedFileReader(void) : m_readPos(0), m_fileSize(0), m_pView(NULL), m_hFile(INVALID_HANDLE_VALUE), m_hMapping(NULL)
	{
	}

	/// �f�X�g���N�^
	~CMappedFileReader(void)
	{
		Close();
	}

	/**
	 * [����] �I�[�v��
	 *	@note ���łɃI�[�v�����Ă�����Close���Ă���I�[�v�����܂�
	 *	@param fileName �t�@�C�����B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(LPCTSTR fileName)
	{
		Close();
		m_hFile = ::CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if ( m_hFile != INVALID_HANDLE_VALUE )
		{
			m_fileSize = ::GetFileSize(m_hFile, NULL);
			m_hMapping = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			if ( m_hMapping != NULL )
			{
				m_pView = static_cast<const BYTE*>(::MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, 0));
				if ( m_pView != NULL )
				{
					m_fileName = fileName;
					return true;
				}
			}
		}
		Close();
		return false;
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_fileName;
	}

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void)
	{
		if ( m_pView != NULL )
		{
			::UnmapViewOfFile(m_pView);
			m_pView = NULL;
		}
		if ( m_hMapping != NULL )
		{
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
		}
		if ( m_hFile != INVALID_HANDLE_VALUE) 
		{
			::CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
		m_readPos = 0;
		m_fileSize = 0;
		m_fileName.Empty();
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return m_pView != NULL;
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		return m_fileSize;
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
		LONGLONG l = m_fileSize;
		LONGLONG p = llOffset;
		switch ( eSeek )
		{
		case END:
			p += l;
			break;
		case CURRENT:
			p += m_readPos;
			break;
		}
		if ( p < 0 )
		{
			return -1;
		}
		else if ( p > l )
		{
			p = l;
		}
		m_readPos = static_cast<INDEX>(p);
		return p;
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
		size_t s = size;
		size_t l = m_fileSize - m_readPos;
		if ( s > l )
		{
			s = l;
		}
		MemCopy(static_cast<BYTE*>(_P), &m_pView[m_readPos], s);
		m_readPos += s;
		return s;
	}

	/**
	 * [�Q��] �f�[�^�A�h���X�Q��.
	 *	@retval NULL �I�[�v�����Ă��Ȃ�
	 *	@retval NULL�ȊO �f�[�^�̐擪�A�h���X
	 */
	const BYTE* Refer(void) const
	{
		return m_pView;
	}

	/**
	 * [�擾] �f�[�^�擾.
	 *	@param index �C���f�b�N�X�B0�ȏ�A GetSize()�̒l�������w��ł��܂��B
	 *	@retval NULL �I�[�v�����Ă��Ȃ�
	 *	@retval NULL�ȊO �f�[�^�̐擪�A�h���X
	 */
	BYTE operator[](INDEX index) const
	{
		if ( index < m_fileSize )
		{
			return m_pView[index];
		}
		return NULL;
	}

private:
	const BYTE*			m_pView;		///< �}�b�s���O�p
	HANDLE				m_hFile;		///< �}�b�s���O�p
	HANDLE				m_hMapping;		///< �}�b�s���O�p
	size_t				m_fileSize;		///< �t�@�C���T�C�Y
	CSimpleStr			m_fileName;		///< �t�@�C����
	mutable INDEX		m_readPos;		///< �Ǎ��݈ʒu
};



}; //TNB
