#pragma once
/**
 *@file
 * �ʐM�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbReportViewer.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * �ʐM�C���^�[�t�F�[�X
 *
 *		���M�A��M���T�|�[�g����C���^�[�t�F�[�X��񋟂��܂��B		
 *
 *	@par�K�v�t�@�C��
 *			TnbCommunication.h
 * 
 *	@date 07/09/13 �V�K�쐬
 *	@date 10/05/28 SetReceiveTimeout() ��V�K�B
 */
struct ICommunication
{
	/**
	 * ���
	 */
	enum EState
	{
		OPENNING,	///< �I�[�v�����
		CONNECTING,	///< �I�[�v�����ڑ����
		CLOSED,		///< �N���[�Y���	
	};

	/**
	 * ���\�v���p�e�B�^.
	 */
	struct TProperty
	{
		CStr		typeName;				///< �^�C�v���B
		DWORD		typeId;					///< �^�C�vID�B
		DWORD		receiveTimeout;			///< ��M�҂����ԁB INFINITE �Ȃ�u���b�N�����B
		size_t		sendPacketSize;			///< ���M�p�P�b�g�T�C�Y�BINVALID_SIZE �Ȃ琧���Ȃ��B
		size_t		receivePacketSize;		///< ��M�p�P�b�g�T�C�Y�BINVALID_SIZE �Ȃ琧���Ȃ��B
		/// �R���X�g���N�^
		TProperty(LPCTSTR lpszName, DWORD id, DWORD to = 0, size_t sendSize = INVALID_SIZE, size_t recvSize = INVALID_SIZE)
			: typeName(lpszName), typeId(id), receiveTimeout(to), sendPacketSize(sendSize), receivePacketSize(INVALID_SIZE)
		{}//�f�t�H���g�́A��M�҂����ԂOms�A����M�p�P�b�g�T�C�Y�A�Ȃ��B
	};

	/// �f�X�g���N�^
	virtual ~ICommunication(void) {}

	/**
	 * [�擾] ���\�擾.
	 *	@note ���m�ȏ��� Open��Ɏ擾�ł��܂��B
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const = 0;

	/**
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@retval true �����B
	 *	@retval false ���s�B�T�|�[�g�ł��Ȃ��������ꂪ�Ԃ�܂��B
	 */
	virtual bool SetReceiveTimeout(DWORD to) = 0;

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void) = 0;

	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void) = 0;

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	virtual bool IsOpened(void) const = 0;

	/**
	 * [�m�F] �ڑ��m�F.
	 *	@retval true �ڑ����B
	 *	@retval false �ؒf���B
	 */
	virtual bool IsConnect(void) const { return IsOpened(); }

	/**
	 * [�m�F] ���M�\�m�F.
	 *	@retval true ���M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanSend(void) const { return IsOpened(); }

	/**
	 * [�m�F] ��M�\�m�F.
	 *	@retval true ��M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanReceive(void) const { return IsOpened(); }

	/**
	 * [����] ���M.
	 *	@param size �T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval ��L�ȊO ���M�f�[�^�T�C�Y�B
	 */
	virtual size_t Send(size_t size, LPCVOID P) = 0;

	/**
	 * [����] ��M.
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval 0 ��M�f�[�^�Ȃ��B
	 *	@retval ��L�ȊO ��M�f�[�^�T�C�Y�B
	 */
	virtual size_t Receive(size_t size, LPVOID _P) = 0;

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@note �{�N���X�ł͕s����Ԃ��܂��B
	 *	@retval INVALID_SIZE �s���B
	 *	@retval ��L�ȊO ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const { return INVALID_SIZE; }

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B
	 *	@note �{�N���X�ł� IsOpened() , IsConnect() ���g�p���܂��B
	 *	@retval OPENNING	�I�[�v�����
	 *	@retval CONNECTING	�I�[�v�����ڑ����
	 *	@retval CLOSED		�N���[�Y���	
	 */
	virtual EState Check(void) const { return IsOpened() ? (IsConnect() ? CONNECTING : OPENNING) : CLOSED; }

	/**
	 * [����] �p�[�W.
	 *		�ʐM�̃S�~����
	 *	@note �{�N���X�ł͂Ȃɂ����܂���B
	 */
	virtual void Purge(void) {}
};



#ifndef _WIN32_WCE

/**@ingroup COMMUNICATION
 * EVENT�AWait���ԕt�� ReadFile�֐�
 *
 *		Event��Wait�̕t���� ::ReadFile �ł��B
 *
 *	@deprecated �p�~�\��ł��B
 *
 *	@attention �^�C���A�E�g�������ACancelIO���邽�߁A�^�C�~���O�ɂ���ẮA
 *				��񂪌�������\��������܂��B�A�������K�v�ȃf�[�^�ł͂�����g�p���Ȃ��ŁA
 *				COverlapReader ���g�p���܂��B
 *	@par�K�v�t�@�C��
 *			TnbCommunication.h
 *	@date 07/09/13 �V�K�쐬
 *	@param[in] hFile �t�@�C���n���h���BReadFile�o����n���h���ł���K�v������܂��B
 *	@param[in] hEvent �C�x���g�BNULL ���w�肷�邱�Ƃ��o���܂��B
 *	@param[in] wait �ő�Wait����(�P�� ms)���w�肵�܂��B
 *	@param[in] size �Ǎ��݃T�C�Y�����Ă��܂��B
 *	@param[out] _P �ǂݍ��ރ|�C���^�����w�肵�܂��B
 *	@retval INVALID_SIZE �G���[�B ::GetLastError() �ŃG���[�ڍׂ��킩��܂��B
 *	@retval 0 �f�[�^�����B
 *	@retval ��L�ȊO �����B���l�͓Ǎ��݃T�C�Y�B
 */
inline size_t BlockingReadFile(HANDLE hFile, HANDLE hEvent, DWORD wait, size_t size, LPVOID _P)
{
	OVERLAPPED overLapped = { 0 }; 
	HANDLE hMyEvent = NULL;
	if ( hEvent == NULL )
	{
		hMyEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
		::ResetEvent(hMyEvent);
		hEvent = hMyEvent;
	}
	overLapped.hEvent = hEvent; 
	DWORD dwRc = ULONG_MAX;
	DWORD work = ULONG_MAX;
	if ( ::ReadFile(hFile, _P, ToDword(size), &dwRc, &overLapped) )
	{
		// ����
	}
	else if ( ::GetLastError() == ERROR_IO_PENDING )
	{
		// �񓯊��ǂݎ�葀��̌��ʂ��`�F�b�N����B
		::Sleep(0);
		if( ::GetOverlappedResult(hFile, &overLapped, &work, FALSE) )
		{
			dwRc = work; // ����
		}
		else if ( ::GetLastError() == ERROR_IO_INCOMPLETE )
		{
			switch ( ::WaitForSingleObject(overLapped.hEvent, wait) )
			{
			case WAIT_OBJECT_0:
				if ( ::GetOverlappedResult(hFile, &overLapped, &work, FALSE) )
				{
					dwRc = work; // ����
				}
				break;
			case WAIT_TIMEOUT:
				dwRc = 0;
				break;
			case WAIT_ABANDONED:
			case WAIT_FAILED:
			default:
				break;
			}
		}
	}
	::CancelIo(hFile);
	if ( hMyEvent != NULL )
	{
		::CloseHandle(hMyEvent);
	}
	return dwRc;
}

#else // _WIN32_WCE

inline size_t BlockingReadFile(HANDLE hFile, HANDLE hEvent, DWORD wait, size_t size, LPVOID _P)
{
	DWORD dwRc = ULONG_MAX;
	if ( ::ReadFile(hFile, _P, ToDword(size), &dwRc, NULL) )
	{
		return dwRc;
	}
	_GetLastError("ReadFile");
	return INVALID_SIZE;
}

#endif // _WIN32_WCE




}; // TNB