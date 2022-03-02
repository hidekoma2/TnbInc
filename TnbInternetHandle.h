#pragma once
/**
 *@file
 * INTERNET�n���h���֌W�̃w�b�_
 *
 *		WININET�� API�� ���b�v�����N���X�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDynamicFunc.h"
#include "TnbPointerHandle.h"
#include "TnbStrVector.h"



#include <Wininet.h>
#pragma comment(lib,"Wininet.lib")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 /// PointerHandleBase�pDestory�N���X
 struct TPhInternetCloseHandle
 {
	void operator()(HINTERNET P) { ::InternetCloseHandle(P); }
 };
#endif



/**@ingroup INTERNET
 * HINTERNET�^�n���h���n���h���\�N���X
 *
 *		���̌^�� ::InternetOpen(),::InternetOpenUrl() �Ŕj������n���h����n���Ă����ƁA
 *		�u�̈�v���Q�Ƃ���{�N���X�����Ȃ��Ȃ莟��A
 *		�����I�� ::InternetCloseHandle() ���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbInternetHandle.h
 */
typedef TNB::CPointerHandleBaseT<HINTERNET, TPhInternetCloseHandle> CInternetHandleHandle;



/**@ingroup INTERNET
 * INTERNET�n���h���Ǘ��N���X
 *
 *	HINTERNET�ɂ������API�����b�v���������̃N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbInternetHandle.h
 *
 *	@date 06/10/03 �V�K
 *	@date 19/01/25 ���\�b�h�ǉ�.
 */
class CInternetHandle
{
	CInternetHandleHandle m_handle;	///< HINTERNET�n���h��

public:

	/**
	 * �R���X�g���N�^
	 *	@param h �n���h��
	 */
	CInternetHandle(HINTERNET h = NULL) : m_handle(h) 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CInternetHandle(const CInternetHandle& other) : m_handle(other.m_handle) 
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CInternetHandle& operator=(const CInternetHandle& other)
	{
		m_handle = other.m_handle;
		return *this;
	}

	/**
	 * [�ݒ�] �N���[�Y.
	 *		�ێ����Ă���n���h�����N���[�Y���܂��B
	 *	@note �R�s�[�Ȃǂ��ăn���h�������L���Ă���ꍇ�A�N���[�Y���܂���B
	 */
	void Close(void)
	{
		m_handle.Null();
	}

	/**
	 * [�m�F] �n���h���L���m�F
	 *	@retval ture �L���B
	 *	@retval false ����(�J���Ă��Ȃ����A���s������)�B
	 */
	bool IsValid(void) const
	{
		return ! m_handle.IsNull();
	}

	/**
	 * [�ݒ�] ������.
	 *		�A�v���P�[�V�������g�p����WININET�̏��������s���AWININET�Ăяo���p�̃n���h�����쐬����B 
	 *	@param lpszAgent		HTTP�ɂ��C���^�[�l�b�g�ɃA�N�Z�X����ۂ̃G�[�W�F���g���B
	 *	@param dwAccessType		�A�N�Z�X���@���w�肷��t���O�B
	 *					<BR>	INTERNET_OPEN_TYPE_DIRECT		�S�Ẵz�X�g�������[�J���ɉ�������B
	 *					<BR>	INTERNET_OPEN_TYPE_PROXY		�v���N�V�o�C�p�X���X�g���^�����Ă��Ȃ��ꍇ��v���N�V��ʂ����ɖ��O�������s����ꍇ�������āA�v���̓v���N�V�ɓn���B�v�����v���N�V�ɓn����Ȃ��P�[�X�̏ꍇ�́A�{�֐���INTERNET_OPEN_TYPE_DIRECT�Ƃ��ē��삷��B
	 *					<BR>	INTERNET_OPEN_TYPE_PRECONFIG	���W�X�g���ɕێ�����Ă���ݒ�𗘗p����B
	 *	@param lpszProxy		�v���N�V�ɂ��A�N�Z�X���w�肳��Ă���ꍇ�ɁA�v���N�V�T�[�o�����w�肷��BNULL�̏ꍇ�A���W�X�g������v���N�V��񂪎擾�����B�v���g�R�����ƂɈقȂ�v���N�V���w�肵�����ꍇ�Ȃǂ̎w��ɂ��Ă�SDK�̃h�L�������g���Q�ƁB
	 *	@param lpszProxyBypass	(�I�v�V������)���[�J���ł̊��m�̃z�X�g���܂���IP�A�h���X�̃��X�g�B�����̃z�X�g�ɑ΂���v���̓v���N�V��ʂ����ɍs����B���̃��X�g�ɂ̓��C���h�J�[�h���܂߂邱�Ƃ��ł���B
	 *	@param dwFlags			Win32�C���^�[�l�b�g�֐��̐U�镑�����w�肷��t���O�B��q�̕\���Q�ƁB
	 *					<BR>	INTERNET_FLAG_OFFLINE	�L���b�V������̂݃f�[�^�擾���s����B
	 *					<BR>	INTERNET_FLAG_ASYNC	�񓯊��ŏ������s����B
	 *	@retval true �����B�{�N���X�̃n���h�����L���ɂȂ�B
	 *	@retval false ���s�B�{�N���X�̃n���h���͖����ɂȂ�B
	 */
	bool Open(LPCTSTR lpszAgent, DWORD dwAccessType, LPCTSTR lpszProxy = NULL, 
				LPCTSTR lpszProxyBypass = NULL, DWORD dwFlags = 0)
	{
		m_handle = ::InternetOpen(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
		return ! m_handle.IsNull();
	}

	/**
	 * [�ڑ�] �w�薼�T�[�o�ڑ�.
	 *		ServerName�Ŏw�肳�ꂽ�C���^�[�l�b�g��̃T�[�o�ɐڑ����A�n���h����Ԃ��B 
	 *	@note	::InternetOpen() �ŕԂ��ꂽ�n���h�����g���܂��B
	 *	@param lpszServerName	�ڑ���̃z�X�g���܂��̓h�b�g�\�L��IP�A�h���X�B
	 *	@param tPort	�ڑ�����TCP/IP�|�[�g�B����̒l�ɂ��Ă͉��\���Q�ƁB�܂��A0���w�肵���ꍇ�A�f�t�H���g�̃|�[�g���g�p�����B
	 *					<BR>	INTERNET_DEFAULT_FTP_PORT		FTP
	 *					<BR>	INTERNET_DEFAULT_GOPHER_PORT	Gopher
	 *					<BR>	INTERNET_DEFAULT_HTTP_PORT		HTTP
	 *					<BR>	INTERNET_DEFAULT_HTTPS_PORT		HTTPS
	 *	@param lpszUserName	�ڑ��̍ۂɎg�p����郆�[�U���BHTTP�ȊO�̏ꍇ�́ANULL���w�肵���ꍇ�f�t�H���g(FTP�Ȃ�"anonymous")���g�p�����B
	 *	@param lpszPassword	�ڑ��̍ۂɎg�p�����p�X���[�h�BFTP��UserName��Password������NULL�̏ꍇ�APassword��e-mail�A�h���X���g�p�����B
	 *	@param dwService	�A�N�Z�X����T�[�r�X�̃^�C�v�B
	 *					<BR>	INTERNET_SERVICE_FTP			FTP
	 *					<BR>	INTERNET_SERVICE_GOPHER			Gopher
	 *					<BR>	INTERNET_SERVICE_HTTP			HTTP
	 *	@param dwFlags	�T�[�r�X�ŗL�̃t���O�l�BService��INTERNET_SERVICE_FTP�̏ꍇ�AFlags�ɂ͈ȉ��̂����ꂩ�̒l���w��\�B 
	 *					<BR>	INTERNET_CONNECT_FLAG_PASSIVE	�p�b�V�u���[�h�Őڑ�����B
	 *	@param dwContext	�R�[���o�b�N�֐��ɓn�����A�v���P�[�V������`��32bit�l�B
	 *	@return �C���^�[�l�b�g�n���h���B IsValid() �ō��۔��肷��B
	 */
	CInternetHandle Connect(LPCTSTR lpszServerName, INTERNET_PORT tPort, LPCTSTR lpszUserName,
			LPCTSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::InternetConnect(m_handle, lpszServerName, tPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);	
	}

	/**
	 * [�ڑ�] �w��URL�ڑ�.
	 *		�w�肳�ꂽURL���I�[�v�����A�ڑ����m�����A�f�[�^�ǂݍ��݂̏������s���A�n���h����Ԃ��B 
	 *	@note	::InternetOpen() �ŕԂ��ꂽ�n���h�����g���܂��B
	 *	@param lpszUrl			�I�[�v������URL�B
	 *	@param lpszHeaders		HTTP�T�[�o�ɑ���w�b�_�BNULL���w��\�B
	 *	@param dwHeadersLength	Headers�̕������B-1���w�肵���ꍇ�A�����I�Ɍv�Z�����B
	 *	@param dwFlags	�I�[�v���̎�ʂ��w�肷��r�b�g�}�X�N�l�B��q�̕\���Q�ƁB
	 *					<BR>	INTERNET_FLAG_RELOAD	���[�J���̃L���b�V���𖳎����A��ɃT�[�o����f�[�^���擾����B
	 *					<BR>	INTERNET_FLAG_DONT_CACHE	���[�J���y�уQ�[�g�E�F�C�Ƀf�[�^���L���b�V�����Ȃ��B
	 *					<BR>	INTERNET_FLAG_RAW_DATA	���̃f�[�^��Ԃ�(FTP�̏ꍇWIN32_FIND_DATA�AGopher�̏ꍇGOPHER_FIND_DATA)�B���̃t���O���w�肳��Ă��Ȃ��ꍇ�A�f�B���N�g���̈ꗗ��HTML�t�H�[�}�b�g�ŕԋp����B(->)
	 *					<BR>	INTERNET_FLAG_SECURE	SSL��PCT���g�p����B(HTTP�̂�)
	 *					<BR>	INTERNET_FLAG_EXISTING_CONNECT	�\�Ȍ�������̃T�[�o�Ƃ̐ڑ����ė��p����B
	 *	@param 	dwContext	�R�[���o�b�N�֐��ɓn���A�v���P�[�V������`��32bit�l�B
	 *	@return �C���^�[�l�b�g�n���h���B IsValid() �ō��۔��肷��B
	 */
	CInternetHandle OpenUrl(LPCTSTR lpszUrl, LPCTSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::InternetOpenUrl(m_handle, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
	}

	/**
	 * [�쐬] HTTP���N�G�X�g�̍쐬.
	 *	@param lpszVerb �v���Ɏg�p���铮���BNULL�̏ꍇ�AGET�Ƃ݂Ȃ����B
	 *	@param lpszObjectName �����̑Ώ̂ƂȂ�I�u�W�F�N�g�̖��O�B
	 *	@param lpszVersion HTTP�̃o�[�W�����BNULL�̏ꍇ�AHTTP/1.0���g�p�����B
	 *	@param lpszReferrer ObjectName�Ŏw�肳�ꂽ�I�u�W�F�N�g�̈ʒu(URL)�BNULL���w��\�B
	 *	@param dwFlags �v���Ɋւ���t���O�B
	 *					<BR>	INTERNET_FLAG_RELOAD	���[�J���̃L���b�V���𖳎����A��ɃT�[�o����f�[�^���擾����B
	 *					<BR>	INTERNET_FLAG_DONT_CACHE	���[�J���y�уQ�[�g�E�F�C�Ƀf�[�^���L���b�V�����Ȃ��B
	 *					<BR>	INTERNET_FLAG_RAW_DATA	���̃f�[�^��Ԃ�(FTP�̏ꍇWIN32_FIND_DATA�AGopher�̏ꍇGOPHER_FIND_DATA)�B���̃t���O���w�肳��Ă��Ȃ��ꍇ�A�f�B���N�g���̈ꗗ��HTML�t�H�[�}�b�g�ŕԋp����B(->)
	 *					<BR>	INTERNET_FLAG_SECURE	SSL��PCT���g�p����B(HTTP�̂�)
	 *					<BR>	INTERNET_FLAG_EXISTING_CONNECT	�\�Ȍ�������̃T�[�o�Ƃ̐ڑ����ė��p����B
	 *	@param dwContext �R�[���o�b�N�֐��ɓn���A�v���P�[�V������`��32bit�l�B
	 *	@return �C���^�[�l�b�g�n���h���B IsValid() �ō��۔��肷��B
	 */
	CInternetHandle HttpOpenRequest(LPCTSTR lpszVerb, LPCTSTR lpszObjectName, LPCTSTR lpszVersion, LPCTSTR lpszReferrer, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::HttpOpenRequest(m_handle, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, NULL, dwFlags, dwContext);

    }

	/**
	 * [�쐬] HTTP���N�G�X�g�̍쐬.
	 *	@param lpszVerb �v���Ɏg�p���铮���BNULL�̏ꍇ�AGET�Ƃ݂Ȃ����B
	 *	@param lpszObjectName �����̑Ώ̂ƂȂ�I�u�W�F�N�g�̖��O�B
	 *	@param lpszVersion HTTP�̃o�[�W�����BNULL�̏ꍇ�AHTTP/1.0���g�p�����B
	 *	@param lpszReferrer ObjectName�Ŏw�肳�ꂽ�I�u�W�F�N�g�̈ʒu(URL)�BNULL���w��\�B
	 *	@param acceptTypes ���f�B�A�̃^�C�v������������̔z��B
	 *	@param dwFlags �v���Ɋւ���t���O�B
	 *					<BR>	INTERNET_FLAG_RELOAD	���[�J���̃L���b�V���𖳎����A��ɃT�[�o����f�[�^���擾����B
	 *					<BR>	INTERNET_FLAG_DONT_CACHE	���[�J���y�уQ�[�g�E�F�C�Ƀf�[�^���L���b�V�����Ȃ��B
	 *					<BR>	INTERNET_FLAG_RAW_DATA	���̃f�[�^��Ԃ�(FTP�̏ꍇWIN32_FIND_DATA�AGopher�̏ꍇGOPHER_FIND_DATA)�B���̃t���O���w�肳��Ă��Ȃ��ꍇ�A�f�B���N�g���̈ꗗ��HTML�t�H�[�}�b�g�ŕԋp����B(->)
	 *					<BR>	INTERNET_FLAG_SECURE	SSL��PCT���g�p����B(HTTP�̂�)
	 *					<BR>	INTERNET_FLAG_EXISTING_CONNECT	�\�Ȍ�������̃T�[�o�Ƃ̐ڑ����ė��p����B
	 *	@param dwContext �R�[���o�b�N�֐��ɓn���A�v���P�[�V������`��32bit�l�B
	 *	@return �C���^�[�l�b�g�n���h���B IsValid() �ō��۔��肷��B
	 */
	CInternetHandle HttpOpenRequest(LPCTSTR lpszVerb, LPCTSTR lpszObjectName, LPCTSTR lpszVersion, LPCTSTR lpszReferrer, const CStrVector& acceptTypes, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		CWorkMemT<LPCTSTR> a(acceptTypes.GetSize() + 1);
		loop ( i, acceptTypes )
		{
			a[i] = acceptTypes.ReferBuffer()->ReferBuffer();
		}
		a[acceptTypes.GetSize()] = NULL;
		return ::HttpOpenRequest(m_handle, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, a, dwFlags, dwContext);
    }

	/**
	 * [���M] HTTP���N�G�X�g�𑗐M.
	 *	@param lpszHeaders �ǉ��̃w�b�_�BNULL���w��\�B
	 *	@param dwHeadersLength Headers�̃T�C�Y�B-1���w�肵���ꍇ�A�����I�Ɍv�Z�����B
	 *	@param lpOptional �v���w�b�_��ɒ��ڑ��M�����I�v�V�����̃f�[�^�B�ʏ�POST��PUT�Ŏg�p�����BNULL���w��\�B
	 *	@param dwOptionalLength Optional�̃T�C�Y�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool HttpSendRequest(LPCTSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
	{
		return !! ::HttpSendRequest(m_handle, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
	}

	/**
	 * [�擾] HTTP���N�G�X�g�Ɋ֘A������̎擾.
	 *	@param dwInfoLevel 	�擾���鑮����A�v���̕ύX�Ȃǂ̃��x����\���t���O�B
	 *	@param lpBuffer �擾���ʁB
	 *	@param lpdwBufferLength Buf�̃T�C�Y�B�֐��̎��s����ۂ̃T�C�Y�܂��͕K�v�ȃT�C�Y���Ԃ����B
	 *	@param lpdwIndex ����̃w�b�_�����݂���ꍇ�ɂǂ̃w�b�_�������ʂ��邽�߂̃C���f�b�N�X�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool HttpQueryInfo(DWORD dwInfoLevel, LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex)
	{
		return !! ::HttpQueryInfo(m_handle, dwInfoLevel, lpBuffer, lpdwBufferLength, lpdwIndex);
	}

	/**
	 * [�擾] �I�v�V�������擾.
	 *		�w�肳�ꂽ�n���h���ɑ΂���C���^�[�l�b�g�̃I�v�V���������擾����B 
	 *	@note ���̎擾���̃C���^�[�l�b�g�̃n���h���B
	 *	@param[in] dwOption	�ǂ̏����擾���邩�����ʂ���t���O�B
	 *					<BR>	INTERNET_OPTION_CALLBACK			hInternet�Ɋ֘A�t�����Ă���R�[���o�b�N�֐��ւ̃|�C���^���Ԃ����B
	 *					<BR>	INTERNET_OPTION_CONNECT_TIMEOUT		�ڑ��v���̃^�C���A�E�g����(�~���b)���Ԃ����B
	 *					<BR>	INTERNET_OPTION_CONNECT_RETRIES		�ڑ��̍ő僊�g���C�񐔁B
	 *					<BR>	INTERNET_OPTION_CONNECT_BACKOFF		���g���C�܂łɈ�莞�ԑҋ@����ۂ̑ҋ@���ԁB���o�[�W�����ł͖������B
	 *					<BR>	INTERNET_OPTION_CONTROL_SEND_TIMEOUT	��f�[�^�̑��M�v���̃^�C���A�E�g����(�~���b)�B���o�[�W�����ł�FTP�̂ݗL���B
	 *					<BR>	INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT	��f�[�^�̎�M�v���̃^�C���A�E�g����(�~���b)�B���o�[�W�����ł�FTP�̂ݗL���B
	 *					<BR>	INTERNET_OPTION_DATA_SEND_TIMEOUT	�f�[�^�̑��M�v���̃^�C���A�E�g����(�~���b)�B
	 *					<BR>	INTERNET_OPTION_DATA_RECEIVE_TIMEOUT	�f�[�^�̎�M�v���̃^�C���A�E�g����(�~���b)�B
	 *					<BR>	INTERNET_OPTION_HANDLE_TYPE			�n���h���̃^�C�v�B
	 *					<BR>	INTERNET_OPTION_CONTEXT_VALUE		hInternet�Ɋ֘A�t�����Ă���Context�̒l�B
	 *					<BR>	INTERNET_OPTION_READ_BUFFER_SIZE	�ǂݍ��ݗp�o�b�t�@�T�C�Y�B
	 *					<BR>	INTERNET_OPTION_WRITE_BUFFER_SIZE	�������ݗp�o�b�t�@�T�C�Y�B
	 *					<BR>	INTERNET_OPTION_ASYNC_PRIORITY		hInternet�ɂ��񓯊��_�E�����[�h�ɂ�����D��x�B
	 *					<BR>	INTERNET_OPTION_PARENT_HANDLE		�e�n���h���B
	 *					<BR>	INTERNET_OPTION_KEEP_CONNECTION		�i���I�ڑ����g�p���邩�ǂ�����\���t���O�B
	 *					<BR>	INTERNET_OPTION_USERNAME			���[�U���B
	 *					<BR>	INTERNET_OPTION_PASSWORD			�p�X���[�h�B
	 *					<BR>	INTERNET_OPTION_REQUEST_FLAGS		���݂̃_�E�����[�h�Ɋւ����ԃt���O�B
	 *					<BR>	INTERNET_OPTION_EXTENDED_ERROR		WinSock�̃G���[�R�[�h�B
	 *					<BR>	INTERNET_OPTION_SECURITY_CERTIFICATE_STRUCT	SSL/PCT�p�̏ؖ�����\��INTERNET_CERTIFICATE_INFO�\���̂̃f�[�^�B
	 *					<BR>	INTERNET_OPTION_SECURITY_CERTIFICATE	SSL/PCT�p�̏ؖ�����\��������B
	 *					<BR>	INTERNET_OPTION_CACHE_STREAM_HANDLE	�Í��p���̃T�C�Y�B
	 *					<BR>	INTERNET_OPTION_SECURITY_FALGS		�Z�L�����e�B�Ɋւ���r�b�g�}�X�N�l�B
	 *					<BR>	INTERNET_OPTION_DATAFILE_NAME		�_�E�����[�h���̃t�@�C�����B
	 *					<BR>	INTERNET_OPTION_URL					�_�E�����[�h���̃��\�[�X��URL���B
	 *					<BR>	INTERNET_OPTION_REFRESH				���W�X�g����������ēǂݍ��݉\���ǂ����B
	 *					<BR>	INTERNET_OPTION_PROXY				�v���N�V����\��INTERNET_PROXY_INFO�\���̂̃f�[�^�B
	 *					<BR>	INTERNET_OPTION_VERSION				WININET�̃o�[�W������\��INTERNET_VERSION_INFO�\���̂̃f�[�^�B
	 *					<BR>	INTERNET_OPTION_USER_AGENT			�G�[�W�F���g���B
	 *	@param[out] _lpBuffer	�擾���ʂ̏����i�[����o�b�t�@�B
	 *	@param[in,out] _lpdwBufferLength _lpBuffer�̃T�C�Y�B�֐��̎��s��A�ݒ肳�ꂽ�f�[�^�̃T�C�Y�܂��͕K�v�ȃT�C�Y���Ԃ����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool QueryOption(DWORD dwOption, LPVOID _lpBuffer, LPDWORD _lpdwBufferLength) const
	{
		return !! ::InternetQueryOption(m_handle, dwOption, _lpBuffer, _lpdwBufferLength);
	}

	/**
	 * [�擾] �t�@�C���ǂݍ���.
	 *		�w�肳�ꂽ�C���^�[�l�b�g��̃t�@�C������f�[�^��ǂݍ��݁ABuf�ɕԂ��B 
	 *	@note ::InternetOpenUrl()�A::FtpOpenFile()�A::GopherOpenFile()�A::HttpOpenRequest() �̂����ꂩ�ŕԂ��ꂽ�n���h�����g�p�B
	 *	@note �ǂݍ��݂��t�@�C���̏I�[�܂ŒB�����ꍇ�A�߂�l�� true ���Ԃ���AlpdwNumberOfBytesRead �ɂ̓[�����ݒ肳���B 
	 *	@param[out] _lpBuffer	�ǂݍ��݌��ʂ̃f�[�^�B
	 *	@param[in]	dwNumberOfBytesToRead	lpBuffer�̃T�C�Y�B
	 *	@param[out]	_lpdwNumberOfBytesRead	���ۂɓǂݍ��܂ꂽ�f�[�^�̃T�C�Y�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool ReadFile(LPVOID _lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD _lpdwNumberOfBytesRead) const
	{
		return !! ::InternetReadFile(m_handle, _lpBuffer, dwNumberOfBytesToRead, _lpdwNumberOfBytesRead);
	}

	/**
	 * [�ݒ�] �t�@�C���|�C���^�ݒ�.
	 *		InternetReadFile�ł̃t�@�C���ǂݍ��݈ʒu��ݒ肷��B 
	 *	@note ::InternetOpenUrl()�A::HttpOpenRequest(GET�܂���HEAD���g�p��HttpSendRequest�ɓn�����Ƃ�)�ŕԂ��ꂽ�n���h�����g�p�B
	 *	@note ::FtpOpenFile()�A::GopherOpenFile() �̃n���h���ł́A�{���\�b�h�͎��s���܂��B
	 *	@param lDistanceToMove	�ړ��ʁB
	 *	@param pReserved �_�~�[�B NULL ���w��B
	 *	@param dwMoveMethod	�ړ����@��\���^�U�l�B
	 *					<BR>	FILE_BEGIN	Distance�̓t�@�C���̐擪����̈ʒu��\���B
	 *					<BR>	FILE_CURRENT	Distance�͌��݂̃t�@�C���ʒu����̑��Έʒu��\���B
	 *					<BR>	FILE_END	Distance�̓t�@�C���̏I�[����̈ʒu��\���B���݂̃t�@�C���T�C�Y���s���̏ꍇ�A���̃t���O�͎��s����B
	 *	@param dwContext	0�łȂ���΂Ȃ�Ȃ��B
	 *	@retval INVALID_SET_FILE_POINTER ���s�B
	 *	@retval ����ȊO �����i�l�͈ړ���̃t�@�C���|�C���^�j�B
	 */
	DWORD SetFilePointer(LONG lDistanceToMove, LONG* pReserved, DWORD dwMoveMethod, DWORD_PTR dwContext = 0) const
	{
		return ::InternetSetFilePointer(m_handle, lDistanceToMove, pReserved, dwMoveMethod, dwContext);
	}

	/**
	 * [�ݒ�] �I�v�V�������ݒ�
	 *	@param dwOption	�ݒ肷����̎�ʁB
	 *					<BR>	INTERNET_OPTION_CALLBACK			�R�[���o�b�N�֐��̃A�h���X��ݒ肷��B
	 *					<BR>	INTERNET_OPTION_CONNECT_TIMEOUT		�C���^�[�l�b�g�ւ̐ڑ��̃^�C���A�E�g���Ԃ�ݒ肷��B
	 *					<BR>	INTERNET_OPTION_CONNECT_RETRIES		�C���^�[�l�b�g�ւ̐ڑ��̃��g���C�񐔁B
	 *					<BR>	INTERNET_OPTION_CONNECT_BACKOFF		���g���C���̑ҋ@���ԁB
	 *					<BR>	INTERNET_OPTION_CONTROL_SEND_TIMEOUT	��f�[�^�̑��M�v���̃^�C���A�E�g���ԁB���o�[�W�����ł�FTP�̂ݗL���B
	 *					<BR>	INTERNET_OPTION_CONTROL_RECIEVE_TIMEOUT	��f�[�^�̎�M�v���̃^�C���A�E�g���ԁB���o�[�W�����ł�FTP�̂ݗL���B
	 *					<BR>	INTERNET_OPTION_DATA_SEND_TIMEOUT	�f�[�^�̑��M�v���̃^�C���A�E�g���ԁB
	 *					<BR>	INTERNET_OPTION_DATA_RECEIVE_TIMEOUT	�f�[�^�̎�M�v���̃^�C���A�E�g���ԁB
	 *					<BR>	INTERNET_OPTION_ASYNC_PRIORITY		�񓯊��_�E�����[�h���̗D��x�B���o�[�W�����ł͖������B
	 *					<BR>	INTERNET_OPTION_CONTEXT_VALUE		hInternet�Ɋ֘A�t����ꂽContext�̒l�B
	 *					<BR>	INTERNET_OPTION_REFRESH				���W�X�g����������ēǂݍ��݉\���ǂ����B
	 *					<BR>	INTERNET_OPTION_PROXY				�v���N�V����\��INTERNET_PROXY_INFO�\���́B
	 *					<BR>	INTERNET_OPTION_USER_AGENT			�G�[�W�F���g���B
	 *					<BR>	INTERNET_OPTION_USERNAME			���[�U���B
	 *					<BR>	INTERNET_OPTION_PASSWORD			�p�X���[�h�B
	 *					<BR>	INTERNET_READ_BUFFER_SIZE			�ǂݍ��݃o�b�t�@���B
	 *					<BR>	INTERNET_WRITE_BUFFER_SIZE			FtpPutFile�ȂǂŎg�p����鏑�����݃o�b�t�@���B
	 *					<BR>	INTERNET_OPTION_WRITE_DATA			�f�[�^�������ݎ��Ɏg�p�����o�b�t�@�̃T�C�Y�B
	 *	@param lpBuffer	�ݒ肷����B
	 *	@param dwBufferLength	lpBuffer �̃T�C�Y�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetOption(DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength)
	{
		return !! ::InternetSetOption(m_handle, dwOption, lpBuffer, dwBufferLength);
	}

	/**
	 * [�ݒ�] �R�[���o�b�N�ݒ�.
	 *		hInternet�Ŏ��ʂ����C���^�[�l�b�g�̎g�p���ɂ�����e��C�x���g�̒ʒm�Ȃǂ�
	 *		�g�p����R�[���o�b�N�֐���ݒ肷��B 
	 *	@see http://nienie.com/~masapico/api_InternetSetStatusCallback.html
	 *	@param	pInetProc	�ݒ肷��R�[���o�b�N�֐��B
	 *	@retval NULL ���s�B
	 *	@retval �{�֐����s�O�ɐݒ肳��Ă����R�[���o�b�N�֐��ւ̃|�C���^
	*/
	INTERNET_STATUS_CALLBACK SetStatusCallback(INTERNET_STATUS_CALLBACK pInetProc)
	{
		return ::InternetSetStatusCallback(m_handle, pInetProc);
	}

	/**
	 * [����] �������s.
	 *		::FtpFindFirstFile() �܂��� ::GopherFindFirstFile() �ŊJ�n���ꂽ�t�@�C�������𑱍s���A
	 *		���̃t�@�C�����擾����B 
	 *	@note ::FtpFindFirstFile() �܂��� ::GopherFindFirstFile() �ŕԂ��ꂽ�����n���h�����g�p�B
	 *	@param pFindData �������ʁBFTP�̏ꍇWIN32_FIND_DATA�ŁAGopher�̏ꍇGOPHER_FIND_DATA�ł���B
	 *	@retval true �����B
	 *	@retval false ���s�B�������A�t�@�C���̌��������������ꍇ�A�G���[�Ƃ���ERROR_NO_MORE_FILES���ݒ肳���
	*/
	bool FindNextFile(LPVOID pFindData) const
	{
		return !! ::InternetFindNextFile(m_handle, pFindData);
	}

	/**
	 * [�o��] �t�@�C����������.
	 *		�C���^�[�l�b�g��̃t�@�C���ɑ΂��āA�w�肳�ꂽ�f�[�^���������ށB 
	 *	@note ::FtpOpenFile() �܂��� ::HttpSendRequestEx() �ŕԂ��ꂽ�A�������ݐ�����ʂ���n���h���B
	 *	@param lpBuffer	�������ރf�[�^�B
	 *	@param dwNumberOfBytesToWrite	�������ރo�C�g���B
	 *	@param lpdwNumberOfBytesWritten	���ۂɏ������܂ꂽ�f�[�^�̃T�C�Y�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	*/
	bool WriteFile(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten)
	{
		return !! ::InternetWriteFile(m_handle, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
	}
	
	/**
	 * [�쐬] �w��f�B���N�g���쐬.
	 *		FTP�ɂ���āADirName�Ŏw�肵���f�B���N�g���������[�g�ɍ쐬����B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszDirName	�쐬����f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool CreateDirectory(LPCTSTR lpszDirName)
	{
		return !! ::FtpCreateDirectory(m_handle, lpszDirName);
	}

	/**
	 * [�폜] �w��t�@�C���폜.
	 *		FileName�Ŏw�肳�ꂽ�����[�g�̃t�@�C�����폜����B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszFileName	�폜����t�@�C���̃t�@�C�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool DeleteFile(LPCTSTR lpszFileName)
	{
		return !! ::FtpDeleteFile(m_handle, lpszFileName);
	}

	/**
	 * [����] �����J�n.
	 *		hFtpSession�Ŏw�肳�ꂽFTP�Z�b�V�����ɂ����āA�t�@�C������уf�B���N�g���G���g���̌������J�n����B 
	 *	@note FtpFindFirstFile�͈��FTP�Z�b�V�����ɂ���x�������s�\�ł���B 
	 *	@note �m������Ă���FTP�Z�b�V�����̃n���h�����g�p�B�B
	 *	@param lpszSearchFile	��������f�B���N�g���p�X���܂��̓t�@�C�����BNULL���w�肵���ꍇ�A�T�[�o��̃J�����g�f�B���N�g���̍ŏ��̃t�@�C�����{�֐��Ō��������B
	 *	@param lpFindFileData	�������ʁB
	 *	@param dwFlags	�t���O�B OpenUrl() ���Q�ƁB
	 *	@param dwContext �R�[���o�b�N�֐��ɓn���A�v���P�[�V������`��32bit�l�B
	 *	@return �����̑��s�Ɏg�p����񋓃n���h���B���s����ƁA�s���ȏ�Ԃ̃n���h�����Ԃ�܂��B
	 */
	CInternetHandle FindFirstFile(LPCTSTR lpszSearchFile, LPWIN32_FIND_DATA lpFindFileData, DWORD dwFlags, DWORD_PTR dwContext = 0) const
	{
		return ::FtpFindFirstFile(m_handle, lpszSearchFile, lpFindFileData, dwFlags, dwContext);
	}

	/**
	 * [�擾] �J�����g�f�B���N�g���擾.
	 *		FTP�Z�b�V�������ł̃J�����g�f�B���N�g�����擾����B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param[out]		_lpszCurrentDirectory	�J�����g�f�B���N�g���B
	 *	@param[in,out]	_lpdwLength		_lpszCurrentDirectory�̃T�C�Y�B�֐��̎��s��ACurDir�ɐݒ肳�ꂽ�f�[�^�̃T�C�Y�܂��͕K�v�ȃT�C�Y���Ԃ����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool GetCurrentDirectory(LPTSTR _lpszCurrentDirectory, LPDWORD _lpdwLength) const
	{
		return !! ::FtpGetCurrentDirectory(m_handle, _lpszCurrentDirectory, _lpdwLength);
	}

	/**
	 * [�擾] ���[�J���֓Ǎ���.
	 *		FTP�Z�b�V�����ɂ����āA�t�@�C���������[�g���烍�[�J���֓]������B 
	 *	@note 	FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszRemoteFile	�擾���̃t�@�C�����B
	 *	@param lpszLocalFile	�]����̃t�@�C�����B
	 *	@param fFailIfExists	LocalFileName�Ŏw�肳�ꂽ�t�@�C�������Ƀ��[�J���ɑ��݂���Ƃ��ɁA�G���[�ɂ��邩�ۂ���\���^�U�l�B
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
	bool GetFile(LPCTSTR lpszRemoteFile, LPCTSTR lpszLocalFile, BOOL fFailIfExists, 
				DWORD dwFlagsAndAttributes, DWORD dwFlags, DWORD_PTR dwContext = 0) const
	{
		return !! ::FtpGetFile(m_handle, lpszRemoteFile, lpszLocalFile, fFailIfExists, dwFlagsAndAttributes, dwFlags, dwContext);
	}

	/**
	 * [�擾] �t�@�C���T�C�Y�擾.
	 *		FTP�Z�b�V�����ɂ����āA�t�@�C���T�C�Y���擾����B 
	 *	@note 	FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param[out] _lpdwFileSizeHigh �T�C�Y�̏��LONG�B
	 *	@return �T�C�Y�̉���LONG
	 */
	DWORD GetFileSize(LPDWORD _lpdwFileSizeHigh) const
	{
		typedef DWORD (WINAPI* P)(HINTERNET, LPDWORD);
		try
		{
			return CDynamicFuncT<P>("Wininet.dll", "FtpGetFileSize")()(m_handle, _lpdwFileSizeHigh);
		}
		catch(CNullPointerException& e)
		{
			ASSERT0(false, "CInternetHandle::GetFileSize()", 
				"API FtpGetFileSize()��������܂���B\nWininet.dll ���Â��\��������܂��B");
			e.OnCatch();
		}
		return 0;
	}

	/**
	 * [�擾] �t�@�C���I�[�v��.
	 *		FTP�Z�b�V������FileName�Ŏw�肳�ꂽ�t�@�C�����I�[�v������B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszFileName	�I�[�v������t�@�C���̖��O�B
	 *	@param dwAccess	�I�[�v�����̃A�N�Z�X�̎�ʁB��q�̕\���Q�ƁB
	 *					<BR>		GENERIC_READ	�ǂݎ��A�N�Z�X�ŊJ���B
	 *					<BR>		GENERIC_WRITE	�������݃A�N�Z�X�ŊJ���B
	 *	@param dwFlags	�t�@�C���]���̎�ʂ�\���t���O�B��q�̕\���Q�ƁB
	 *					<BR>		�܂��ȉ��̂����ꂩ���w�肷��B 
	 *					<BR>		FTP_TRANSFER_TYPE_ASCII	ASCII�]���B
	 *					<BR>		FTP_TRANSFER_TYPE_BINARY	�o�C�i���]���B
	 *					<BR>		FTP_TRANSFER_TYPE_UNKNOWN	�f�t�H���g�ł�FTP_TRANSFER_TYPE_BINARY�B
	 *					<BR>
	 *					<BR>		���ɁA�ȉ��̔C�ӂ̑g�ݍ��킹��_���a�ɂ���Ďw�肷��B 
	 *					<BR>		INTERNET_FLAG_HYPERLINK	�ēǂݍ��݂��ׂ����ǂ����̔��f���ɗL�������E�ŏI�X�V�������T�[�o����Ԃ���Ȃ��ꍇ�����I�ɍēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_MAKE_PERSISTENT	�L���b�V���ɉi���I�v�f�Ƃ��Ēǉ�����B����ɂ��A�ʏ�̃L���b�V���̃N���[���A�b�v�A�������`�F�b�N�A���݂͍s���Ȃǂ̍ۂɂ��̗v�f���폜����Ȃ��B
	 *					<BR>		INTERNET_FLAG_RELOAD	�v�������t�@�C���A�I�u�W�F�N�g�A�f�B���N�g�����X�g���L���b�V������擾����̂łȂ��A�����I�ɃT�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTP���\�[�X���T�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_NEED_FILE	�t�@�C�����L���b�V���ł��Ȃ��ꍇ�Ɉꎞ�t�@�C�����쐬����B
	 *	@param dwContext	�R�[���o�b�N�ɓn���A�v���P�[�V������`�̒l�B
	 *	@return �C���^�[�l�b�g�n���h���B IsValid() �ō��۔��肷��B
	*/
	CInternetHandle OpenFile(LPCTSTR lpszFileName, DWORD dwAccess, DWORD dwFlags, DWORD_PTR dwContext = 0)
	{
		return ::FtpOpenFile(m_handle, lpszFileName, dwAccess, dwFlags, dwContext);
	}

	/**
	 * [���M] ���[�J������t�@�C�����M.
	 *		FTP�Z�b�V�����ŁALocalFileName�Ŏw�肳�ꂽ���[�J���̃t�@�C���������[�g�֓]������B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszLocalFile	�]�����̃t�@�C�����B
	 *	@param lpszNewRemoteFile	�]����̃t�@�C�����B
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
	bool PutFile(LPCTSTR lpszLocalFile, LPCTSTR lpszNewRemoteFile, DWORD dwFlags, DWORD_PTR dwContext)
	{
		return !! ::FtpPutFile(m_handle, lpszLocalFile, lpszNewRemoteFile, dwFlags, dwContext);
	}

	/**
	 * [�폜] �f�B���N�g���폜.
	 *		�w�肳�ꂽFTP�Z�b�V�����ŁADirName�Ŏw�肳�ꂽ�����[�g�̃f�B���N�g�����폜����B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszDirName	�폜����f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool RemoveDirectory(LPCTSTR lpszDirName)
	{
		return !! ::FtpRemoveDirectory(m_handle, lpszDirName);
	}

	/**
	 * [�ύX] ���O�ύX.
	 *		�w�肵��FTP�Z�b�V�����ɂ����āAOldFileName�Ŏw�肳�ꂽ�����[�g�̃t�@�C���̖��O��NewFileName�Ŏw�肳�ꂽ���O�ɕύX����B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszOldFileName	���̃t�@�C�����B
	 *	@param lpszNewFileName	�V�K�t�@�C�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	*/
	bool RenameFile(LPCTSTR lpszOldFileName, LPCTSTR lpszNewFileName)
	{
		return !! ::FtpRenameFile(m_handle, lpszOldFileName, lpszNewFileName);
	}

	/**
	 * [�ݒ�] �J�����g�f�B���N�g���ݒ�.
	 *		�w�肵��FTP�Z�b�V�����ŁA�����[�g�̃J�����g�f�B���N�g����DirName�Ŏw�肵���f�B���N�g���ɕύX����B 
	 *	@note FTP�Z�b�V�����̃n���h�����g�p�B
	 *	@param lpszDirName	�ύX��̃f�B���N�g�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetCurrentDirectory(LPCTSTR lpszDirName)
	{
		return !! ::FtpSetCurrentDirectory(m_handle, lpszDirName);
	}

	/**
	 * [����] �C���^�[�l�b�g���p�\����.
	 *		�C���^�[�l�b�g�ւ̐ڑ������݂邱�Ƃɂ��A���݂̊����C���^�[�l�b�g�����p�\�Ȋ����ǂ����𒲍�����B 
	 *	@retval true �ڑ��\�B
	 *	@retval false �s�\�B
	 */
	static bool AttemptConnect(void)
	{
		return ::InternetAttemptConnect(0) == ERROR_SUCCESS;
	}

	/**
	 * [�ϊ�] URL�ϊ�.
	 *		Flags�Ŏw�肳�ꂽ�ϊ����@�ɂ��������ĕϊ����܂��B
	 *	@todo API�̓������ǂ��킩��Ȃ��B�g�p�ɂ͒��ӁB
	 *
	 *	@param[out]		_lpszDstUrl �ϊ����URL�B
	 *	@param[in,out]	_dwDstSize	_lpszDstUrl�̃T�C�Y�B�֐��̎��s��A
	 *					���ۂɐݒ肳�ꂽ�f�[�^�̃T�C�Y�܂��͕K�v�ȃT�C�Y���Ԃ����B�B
	 *	@param[in]	lpszUrl		�ϊ�����URL�B
	 *	@param[in]	dwFlags		�ϊ����@�B
	 *					\n	ICU_DECODE				%XX�̌`���̃G�X�P�[�v�V�[�P���X�𕶎��ɕϊ�����B
	 *					\n	ICU_NO_ENCODE			���ꕶ�����G�X�P�[�v�V�[�P���X�ɕϊ����Ȃ��B
	 *					\n	ICU_NO_META				���^�V�[�P���X(".."�Ȃ�)��URL����폜���Ȃ��B
	 *					\n	ICU_ENCODE_SPACES_ONLY	�󔒂̂݃G���R�[�h����B
	 *					\n	ICU_BROWSER_MODE		# �� ?�Ȃǂ̌�ɑ����������G���R�[�h�E�f�R�[�h���Ȃ��B�܂��A?�̌�̘A������󔒂��폜���Ȃ��B
	 *											���̃t���O���w�肳��Ă��Ȃ��ꍇ�AURL�̓G���R�[�h����A�A������󔒂͍폜�����B
	 *					\n	��Flags�ɂǂ̃t���O���w�肳��Ă��Ȃ��ꍇ�A�֐��͑S�Ă̓��ꕶ���⃁�^�V�[�P���X���G�X�P�[�v�V�[�P���X�ɕϊ�����B
	 *	@param[in]	lpszRelativeUrl	�g�ݍ��킹�鑊�Ύw���URL�B�ȗ��\�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool CanonicalizeUrl(LPTSTR _lpszDstUrl, DWORD& _dwDstSize, LPCTSTR lpszUrl, DWORD dwFlags, LPCTSTR lpszRelativeUrl = NULL)
	{
		BOOL boRc = FALSE;
		if ( lpszRelativeUrl == NULL )
		{
			boRc = ::InternetCanonicalizeUrl(lpszUrl, _lpszDstUrl, &_dwDstSize, dwFlags);
		}
		else
		{
			boRc = ::InternetCombineUrl(lpszUrl, lpszRelativeUrl, _lpszDstUrl, &_dwDstSize, dwFlags);
		}
		return !! boRc;
	}
};



}; // TNB


 
