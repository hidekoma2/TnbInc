#pragma once
/**
 *@file
 * �t�@�C���ʐM�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFile.h"
#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION FILE
 * �t�@�C���ʐM�N���X
 *
 *		��̃t�@�C�����g�����A�v���Z�X�ԒʐM���T�|�[�g���܂��B
 *
 *		�t�@�C���̓ǂݏ����́A CFileCommunicator::IWorker �����������N���X�ōs���܂��B
 *		���Ƃ��āA CFileCommunicationWorker ���p�ӂ���Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileCommunicator.h
 * 
 *	@date 07/09/13	�V�K�쐬
 *	@date 10/03/24	�P���̓����]���ɕύX�B
 *	@date 10/05/28 SetSendTimeout() ��V�K�B SetReceiveTimeout() �������B
 */
class CFileCommunicator : public ICommunication
{
	DEFSUPER(ICommunication);
public:

	/**@ingroup COMMUNICATION FILE
	 * �t�@�C���ʐM���[�J�[�C���^�[�t�F�[�X.
	 *
	 *		CFileCommunicator �Ńt�@�C���̓ǂݏ����Ɏg�p���܂��B
	 *
	 *	@note CFileCommunicator �̃C���X�^���X�Ɩ{�C���^�[�t�F�[�X�̃C���X�^���X�𓯂��X�R�[�v�ɂ����ꍇ�A 
	 *		CFileCommunicator �̃C���X�^���X������ɐ錾���Ă������Ɓi��ɍ\�z����A��ɔj������邽�߁j�B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbFileCommunicator.h
	 * 
	 *	@date 10/03/24	�V�K�쐬
	 */
	struct IWorker
	{
		/// �f�X�g���N�^
		virtual ~IWorker(void) {}

		/**
		 * [����] �I�[�v��.
		 *		�I�[�v�����A�R�[������܂��B
		 *	@retval true ����
		 *	@retval false ���s
		 */
		virtual bool OnOpen(void) = 0;

		/**
		 * [����] �ǂݍ���.
		 *		����M�̍ۃR�[������܂��B
		 *	@param size �ǂݍ��݃T�C�Y�B
		 *	@param _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
		 *	@retval INVALID_SIZE �I�[�v�����s�B
		 *	@retval 0 �ǂݍ��߂Ȃ������B
		 *	@retval ��L�ȊO �����B�l�͓ǂݍ��߂��T�C�Y�B
		 */
		virtual size_t Read(size_t size, LPVOID _P) =0;

		/**
		 * [����] ��������.
		 *		��񑗐M�̍ۃR�[������܂��B
		 *	@param size �������݃T�C�Y�B
		 *	@param P �������݃f�[�^�B
		 *	@retval INVALID_SIZE �I�[�v�����s�B
		 *	@retval ��L�ȊO �����B�l�͏������߂��T�C�Y�B
		 */
		virtual size_t Write(size_t size, LPCVOID P) = 0;

		/**
		 * [����] �p�[�W.
		 *		�ʐM�̃S�~����
		 *	@retval true �����B
		 *	@retval false ���s.
		 */
		virtual bool Purge(void) = 0;
	};


	//--------------------------------------


	/// �R���X�g���N�^ 
	CFileCommunicator(void) : m_pWorker(NULL), m_sendTimeout(1000)
		, m_property(_T("File"), TNBVIW_KIND_FILE, 1000)
	{
	}

	/// �f�X�g���N�^
	virtual ~CFileCommunicator(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] ���[�J�[�̎w��.
	 *	@note �{�N���X�̃C���X�^���X�ƃ��[�J�[�̃C���X�^���X�𓯂��X�R�[�v�ɂ����ꍇ�A 
	 *		���[�J�[�̂ق�����ɐ錾���Ă������Ɓi��ɍ\�z����A��ɔj������邽�߁j�B
	 *	@param pWorker ���[�J�[�B
	 */
	void SetWorker(IWorker* pWorker)
	{
		m_pWorker = pWorker;
	}

	/**
	 * [�ݒ�] ���M�҂����Ԑݒ�.
	 *		�t�@�C�����I�[�v���ł��Ȃ����A���g���C�������鎞�Ԃ��w�肵�܂��B
	 *	@param to �^�C���A�E�g(ms)
	 */
	void SetSendTimeout(DWORD to)
	{
		m_sendTimeout = to;
	}

	/**
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@return ��� true�B
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		m_property.receiveTimeout = to;
		return true;
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
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void)
	{
		if ( m_pWorker == NULL )
		{
			return false;
		}
		return m_pWorker->OnOpen();
	}

	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	virtual bool IsOpened(void) const
	{
		return true;
	}

	/**
	 * [�m�F] ���M�\�m�F.
	 *	@retval true ���M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanSend(void) const
	{
		return (m_pWorker != NULL);
	}

	/**
	 * [�m�F] ��M�\�m�F.
	 *	@retval true ��M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanReceive(void) const
	{
		return (m_pWorker != NULL);
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
		size_t r = INVALID_SIZE;
		if ( m_pWorker != NULL )
		{
			CTickCount tick;
			do
			{
				size_t rr = m_pWorker->Write(size, P);
				if ( rr != INVALID_SIZE )
				{
					r = rr;
					break;
				}
				::Sleep(RETRY_INTERVAL);
			}
			while ( ! tick.IsPassed(m_sendTimeout) );
		}
		return r;
	}

	/**
	 * [����] ��M.
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval 0 ��M�f�[�^�Ȃ��B
	 *	@retval ��L�ȊO ��M�f�[�^�T�C�Y�B
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		size_t r = 0;
		if ( m_pWorker != NULL )
		{
			CTickCount tick;
			do
			{
				size_t rr = m_pWorker->Read(size, _P);
				if ( rr != INVALID_SIZE )
				{
					r = rr;
					break;
				}
				::Sleep(RETRY_INTERVAL);
			}
			while ( ! tick.IsPassed(m_property.receiveTimeout) );
		}
		return r;
	}

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@retval INVALID_SIZE �s���B
	 *	@retval ��L�ȊO ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
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
		return IsConnect() ? CONNECTING : CLOSED;
	}

	/**
	 * [����] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void) 
	{
		if ( m_pWorker != NULL )
		{
			CTickCount tick;
			do
			{
				if ( m_pWorker->Purge() )
				{
					break;
				}
				::Sleep(RETRY_INTERVAL);
			}
			while ( ! tick.IsPassed(m_property.receiveTimeout) );
		}
	}

protected:
	enum { RETRY_INTERVAL = 10 };
	DWORD			m_sendTimeout;		///< ���M�^�C���A�E�g
	IWorker*		m_pWorker;			///< ���[�J�[
	TProperty		m_property;			///< �{�N���X�̐��\
};



/**@ingroup COMMUNICATION FILE
 * �t�@�C���ʐM���[�J�[�N���X
 *
 *		��̃t�@�C�����g�����A�v���Z�X�ԒʐM���T�|�[�g���܂��B 
 *
 *		�{�N���X�ł́A�ȉ��̎d�l�ŒʐM�T�|�[�g���܂��B
 *		- ���M(��������)���́A��ɑ��M������񂪑���Ɏ�M����Ă��Ȃ��Ă��A�㏑�����܂��B
 *		- ��M(�ǂݍ���)���A���M���A���ɁA���̃v���Z�X���g�p���Ă���Ƃ��́A1�b�A���g���C���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileCommunicator.h
 * 
 *	@date 07/09/13	�V�K�쐬
 *	@date 10/03/24	�P���̓����]���ɕύX�B
 */
class CFileCommunicationWorker : public CFileCommunicator::IWorker
{
public:

	/**
	 * [�ݒ�] �Ώۃt�@�C�����ݒ�.
	 *	@param lpszReadFile �ǂݍ��ݑΏۃt�@�C����.
	 *	@param lpszWriteFile �������ݑΏۃt�@�C����.
	 *	@return ���true
	 */
	bool SetFileName(LPCTSTR lpszReadFile, LPCTSTR lpszWriteFile)
	{
		m_readFile = lpszReadFile;
		m_writeFile = lpszWriteFile;
		return true;
	}

	/**
	 * [����] �I�[�v��.
	 *		�I�[�v�����A�R�[������܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnOpen(void)
	{
		Purge();
		bool isEmpty = m_readFile.IsEmpty() || m_writeFile.IsEmpty();
		return ! isEmpty;
	}

	/**
	 * [�擾] �ǂݍ��ݑΏۃI�[�v��.
	 *	@param size �ǂݍ��݃T�C�Y�B
	 *	@param _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@retval INVALID_SIZE �I�[�v�����s�B
	 *	@retval 0 �ǂݍ��߂Ȃ������B
	 *	@retval ��L�ȊO �����B�l�͓ǂݍ��߂��T�C�Y�B
	 */
	virtual size_t Read (size_t size, LPVOID _P)
	{
		CFileReader fr;
		if ( fr.Open(m_readFile, false) )
		{
			size_t r = fr.Read(size, _P);
			fr.Close();
			::DeleteFile(m_readFile);
			return r;
		}
		DWORD e = ::GetLastError();
		return (e == ERROR_FILE_NOT_FOUND) ? 0 : INVALID_SIZE;
	}

	/**
	 * [�擾] �������ݑΏۃI�[�v��.
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@retval INVALID_SIZE �I�[�v�����s�B
	 *	@retval ��L�ȊO �����B�l�͏������߂��T�C�Y�B
	 */
	virtual size_t Write(size_t size, LPCVOID P)
	{
		CFileWriter fw;
		if ( fw.New(m_writeFile, false) )
		{
			try
			{
				fw.Write(size, P);
				return size;
			}
			catch ( CWriteFailureException& e )
			{
				e.OnCatch();
			}
		}
		return INVALID_SIZE;
	}

	/**
	 * [����] �p�[�W.
	 *		�ʐM�̃S�~����
	 *	@retval true �����B
	 *	@retval false ���s.
	 */
	virtual bool Purge(void)
	{
		::DeleteFile(m_writeFile);
		BOOL r = ::DeleteFile(m_readFile + _T("__"));
		r &= ::DeleteFile(m_readFile);
		return !! r;
	}

protected:

	CStr		m_readFile;		///< �ǂݍ��ݑΏۃt�@�C����
	CStr		m_writeFile;	///< �������ݑΏۃt�@�C����
};



}; // TNB
