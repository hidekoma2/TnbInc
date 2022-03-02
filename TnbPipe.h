#pragma once
/**
 *@file
 * �p�C�v�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * �p�C�v�A�N�Z�X�N���X
 *
 *		�p�C�v���g�p�����ʐM���\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPipe.h
 * 
 *	@date 07/01/23	�V�K�쐬
 *	@date 07/02/09	�e�N���X�ύX�B
 *	@date 07/06/11	�e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 07/09/13	�e�N���X�ύX�B
 *	@date 10/05/28	SetReceiveTimeout() �������B
 *	@date 12/01/26	GetReceivingDataSize() �̖߂�l���C���B
 *	@data 16/05/13	���O�t���N���C�A���g�����B�T�[�o�[�͖��m�F�B
 */
class CPipe : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);

public:

	/// �R���X�g���N�^
	CPipe(void) 
		: m_hRead(INVALID_HANDLE_VALUE), m_hWrite(INVALID_HANDLE_VALUE), m_dwPipeBufferSize(0), m_property(_T("Pipe"), TNBVIW_KIND_PIPE)
	{
		m_isActiveClient = false;
	}

	/// �f�X�g���N�^
	virtual ~CPipe(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �p�C�v�o�b�t�@�T�C�Y�ݒ�
	 *	@param dwSize �p�C�v�̃o�b�t�@�̃T�C�Y���o�C�g�P�ʂŎw�肵�܂��B\n
	 *				�V�X�e���͂��̒l���g���āA�K�؂ȃo�b�t�@�����O���J�j�Y�������肵�܂��B\n
	 *				���O�Ȃ��p�C�v�̏ꍇ�͏ȗ����邩�A0 ���w�肷��ƁA����̃o�b�t�@�T�C�Y�����蓖�Ă��܂��B
	 */
	void SetPipeBufferSize(DWORD dwSize = 0)
	{
		m_dwPipeBufferSize = dwSize;
	}

	/**
	 * [�擾] ���\�擾.
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property;
	}

	/**
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@return ��� false�B
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return false;
	}

	/**
	 * [�ݒ�] �p�C�v���ݒ�.
	 *		�{���\�b�h���g�p���邱�ƂŖ��O�t���p�C�v���g�����Ƃ��o���܂��B Open() �̑O�Ɏg�p���Ă��������B
	 *	@param lpszPipeName �p�C�v���B�K�� "\\\\.\\pipe\\" �Ŏn�܂閼�O�ɂ��Ă��������B
	 *	@param isClient true �Ȃ疼�O�t���p�C�v�N���C�A���g�A false �Ȃ�T�[�o�[���I�[�v�����܂��B
	 */
	void SettingNamedPipe(LPCTSTR lpszPipeName, bool isClient)
	{
		m_pipeName = lpszPipeName;
		m_isModeClient = isClient;
	}

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void)
	{
		Close();
		//
		if ( m_pipeName.IsEmpty() )
		{
			//-- �������p�C�v�I�[�v��
			SECURITY_ATTRIBUTES sa;
			sa.lpSecurityDescriptor = NULL;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = TRUE;
			return !! ::CreatePipe(&m_hRead, &m_hWrite, &sa, m_dwPipeBufferSize);
		}
		if ( m_isModeClient )
		{
			//== ���O�t���p�C�v�N���C�A���g�I�[�v��(�����ł̓I�[�v�����Ȃ�)
			m_isActiveClient = true;
			m_hRead = INVALID_HANDLE_VALUE;
			m_hWrite = INVALID_HANDLE_VALUE;
			return true;
		}
		//== ���O�t���p�C�v�T�[�o�[�I�[�v��
		m_hRead = ::CreateNamedPipe(m_pipeName, 
			PIPE_ACCESS_DUPLEX, //�I�[�v�����[�h
			PIPE_TYPE_BYTE | PIPE_WAIT, //�p�C�v���[�h
			1, //�p�C�v�ɑ΂���ő�C���X�^���X
			m_dwPipeBufferSize,//�o�̓o�b�t�@
			m_dwPipeBufferSize,//���̓o�b�t�@
			0,//�^�C���A�E�g
			NULL);
		m_hWrite = INVALID_HANDLE_VALUE;
		return m_hRead != INVALID_HANDLE_VALUE;
	}

	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
		if ( m_hRead != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hRead);
			m_hRead = INVALID_HANDLE_VALUE;
		}
		if ( m_hWrite != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hWrite);
			m_hWrite = INVALID_HANDLE_VALUE;
		}
		m_isActiveClient = false;
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	virtual bool IsOpened(void) const
	{
		if ( m_isActiveClient )
		{
			return true;
		}
		return m_hRead != INVALID_HANDLE_VALUE;
	}

	/**
	 * [����] ���M.
	 *	@param size �T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval ��L�ȊO ���M�f�[�^�T�C�Y�B
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		m_OnActiveClient();
		//
		DWORD dwWriten;
		if ( ::WriteFile(GetWriteHandle(), P, down_cast<DWORD>(size), &dwWriten, NULL) )
		{
			return dwWriten;
		}
		return INVALID_SIZE;
	}

	/**
	 * [����] ��M.
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@param[in] size �T�C�Y�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval 0 ��M�f�[�^�Ȃ��B
	 *	@retval ��L�ȊO ��M�f�[�^�T�C�Y�B
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		size_t l = GetReceivingDataSize();
		if ( l == 0 )
		{
			return 0;
		}
		if ( l != INVALID_SIZE )
		{
			if ( size < l )
			{
				l = size;
			}
			DWORD dwReadBytes = 0;
			if ( !! ::ReadFile(m_hRead, _P, down_cast<DWORD>(l), &dwReadBytes, NULL) )
			{
				return dwReadBytes;
			}
			_GetLastError("ReadFile");
		}
		return INVALID_SIZE;
	}

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@retval INVALID_SIZE �s���B
	 *	@retval ��L�ȊO ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		m_OnActiveClient();
		if ( m_hRead == INVALID_HANDLE_VALUE )
		{
			return INVALID_SIZE;
		}
		//
		DWORD dwAvail;
		if ( ::PeekNamedPipe(m_hRead, NULL, 0, NULL, &dwAvail, NULL) )
		{
			return dwAvail;
		}
		if ( ! m_pipeName.IsEmpty() && m_isModeClient )
		{
			m_hRead = INVALID_HANDLE_VALUE;
		}
		_GetLastError("PeekNamedPipe");
		return INVALID_SIZE;
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B
	 *	@retval CONNECTING	�I�[�v�����ڑ����
	 *	@retval CLOSED		�N���[�Y���	
	 */
	virtual EState Check(void) const
	{
		m_OnActiveClient();
		return IsConnect() ? CONNECTING : CLOSED;
	}

	/**
	 * [����] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void) 
	{
	}

	/**
	 * [�擾] �ǂݍ��݃n���h���擾
	 *	@return �ǂݍ��݃n���h��
	 */
	HANDLE GetReadHandle(void) const
	{
		return m_hRead;
	}

	/**
	 * [�擾] �������݃n���h���擾
	 *	@return �������݃n���h��
	 */
	HANDLE GetWriteHandle(void) const
	{
		if ( m_pipeName.IsEmpty() )
		{
			return m_hWrite;
		}
		return m_hRead;
	}

private:
	// ���O�t���p�C�v�N���C�A���g���A�N�e�B�u�̎��̏���
	void m_OnActiveClient(void) const
	{
		if ( m_isActiveClient && ! m_pipeName.IsEmpty() && m_isModeClient )
		{
			if ( m_hRead == INVALID_HANDLE_VALUE )
			{
				// �p�C�v�N���C�A���g���I�[�v��
				m_hRead = ::CreateFile(m_pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			}
		}
	}

	CStr				m_pipeName;			///< �p�C�v��
	bool				m_isModeClient;		///< ���O�t���p�C�v�N���C�A���g���[�h�H
	bool				m_isActiveClient;	///< ���O�t���p�C�v�N���C�A���g���[�h�A�N�e�B�u���H
	mutable HANDLE		m_hRead;			///< �ǂݍ��݃n���h��
	HANDLE				m_hWrite;			///< �������݃n���h��
	DWORD				m_dwPipeBufferSize;	///< �o�b�t�@�T�C�Y
	TProperty			m_property;			///< �{�N���X�̐��\
};



}; // TNB
