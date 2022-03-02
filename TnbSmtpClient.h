#pragma once
/**
 *@file
 * SMTP(Simple Mail Transfer Protocol)�֌W�̃w�b�_
 *
 *	@note Winsock ���g�p���Ă��܂��B
 *
 *	@note	�}�N�� "_TnbWINSOCK2_ENABLE" ����`����Ă���ƁA Winsock2.0 ���g�p���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPop3Client.h"



//TNB Library
namespace TNB
{



/**@ingroup MAIL
 * SMTP(Simple Mail Transfer Protocol)�v���g�R�������N���X
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		CPop3Client pop3;	//POP Before SMTP �p
 *		pop3.SetServerName(_T("Localhost"));
 *		pop3.SetUser(_T("master"), _T("master"));
 *		pop3.SetKind(pop3.APOP);
 *	
 *		CMailContent mail;	//���[�����e
 *		mail.SetFromMailAddr("boss@tnb.co.jp", "�ڂ�");		
 *		mail.AddToMailAddr("oyabun@tnb.co.jp", "����Ԃ�");
 *		mail.AddCcMailAddr("master@tnb.co.jp", "�}�X�^�[");
 *		mail.SetSubject("��̌�");
 *		mail.AddSendTextData("����ɂ���\r\n��̌��A�ǂ��ł����H");		
 *
 *		CSmtpClient smtp;
 *		smtp.SetServerName(_T("localhost"));
 *		smtp.SetDomain(_T("boss@tnb.co.jp"));
 *		smtp.SetPopBeforeMode(&pop3);
 *		smtp.Send(mail);
 *	</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbSmtpClient.h
 * 
 *	@date 06/09/28 �b��쐬(�Ƃ肠�����A�����)
 *	@date 06/09/29 POP Before SMTP �A�ǉ��B
 *	@date 06/10/03 BASE64 �ϊ��Ή��B�e�L�X�g�����Ȃ烁�[���Ƃ��Ďg�p�\�ɁB
 *	@date 06/10/05 �}���`�p�[�c�Ή��B
 *	@date 07/06/15 �ʐM�N���X�������ł���悤�Ή�
 *	@date 08/07/31 ExtendedHelloMode() ��ǉ��B
 *	@date 09/09/15	�e�N���X�̐����ɒǏ]�B
 */
class CSmtpClient : public CAbstractMailClient
{
	DEFSUPER(CAbstractMailClient);
public:

	/// �R���X�g���N�^
	CSmtpClient(void) : _super(), m_pPop3(NULL), m_isEhloMode(true)
	{
	}

	/// �f�X�g���N�^
	virtual ~CSmtpClient(void)
	{
		Disconnect();
	}

	/**
	 * [�ݒ�] ExtendedHelloMode�ݒ�
	 *	@note �f�t�H���g�ł� true �iEHLO���g����j�ł��B
	 *	@param enable true �Ȃ� EHLO �A false �Ȃ� HELO ���g���܂��B
	 */
	void SetExtendedHelloMode(bool enable)
	{
		m_isEhloMode = enable;
	}

	/**
	 * [�擾] �ڑ��������̕�����.
	 *	@note	Connect�ɐ�������ƁA�����������Ă���B��������̊֐��œ��邱�Ƃ��\�B
	 *			Disconnect���Ă����ڑ�����܂œ��������񂪓����܂�
	 *	@return ������
	 */
	CAscii GetOpeningMessage(void) const
	{
		return m_ascOpeningMessage;
	}

	/**
	 * [�ݒ�] �ʐM�ݒ�.
	 *	@note Winsock ���g�p���ĒʐM���s�Ȃ����߂ɁA�T�[�o���A�|�[�g�ԍ����w�肵�܂��B
	 *	@note �{���\�b�h�����s����� SetReportInterface() �̐ݒ�͖����ɂȂ�܂��B
	 *	@param lpszName �T�[�o���B
	 *	@param wPort �|�[�g�i���o�[�B�ȗ������25�B
	 */
	void SetServerName(LPCTSTR lpszName, WORD wPort = 25)
	{
		_super::SetServerName(lpszName, wPort);
	}

	/**
	 * [�ݒ�] ���M���ݒ�
	 *	@note ���M�̂��߂ɕK�{�Ȑݒ�ł��B
	 *	@param lpszDomain �h���C�����iHELO�R�}���h�Ŏg�p�j�B
	 */
	void SetDomain(LPCTSTR lpszDomain)
	{
		m_ascDomain = lpszDomain;
	}

	/**
	 * [�ݒ�] POP Before SMTP �ݒ�
	 *	@param pPop3 �F�ؐݒ肵���APOP3�N���C�A���g�BNULL�Ȃ�ݒ�����B
	 *	@param dwWait �F�،��Wait(ms)�B�ȗ������300ms�ɂȂ�܂��B
	 */
	void SetPopBeforeMode(CPop3Client* pPop3, DWORD dwWait = 300)
	{
		m_pPop3 = pPop3;
		m_dwPop3Wait = dwWait;
	}

	/**
	 * [�ݒ�] ���M�҃A�h���X�ݒ�
	 *	@attention Send() �œK�p���� CMailContent �N���X�Őݒ肳��Ă�����̂ƈقȂ鑗�M�҂ɂ������ꍇ�݂̂Ɏg�p���܂��B
	 *	@param lpszFromMailAddr �A�h���X
	 */
	void SetFromMailAddr(LPCTSTR lpszFromMailAddr)
	{
		m_ascFromMailAddr = lpszFromMailAddr;
	}
	
	/**
	 * [�ݒ�] ���於�ݒ菉����
	 */
	void InitToMailAddr(void)
	{
		m_vascToMailAddrs.RemoveAll();
	}

	/**
	 * [�ݒ�] ���於�ݒ�
	 *	@attention Send() �œK�p���� CMailContent �N���X�Őݒ肳��Ă�����̂ƈقȂ鈶��ɑ��肽���ꍇ�݂̂Ɏg�p���܂��B
	 *	@param lpszToAddr ����A�h���X(Ascii�����̂�)�B
	 */
	void AddToAddr(LPCTSTR lpszToAddr)
	{
		m_vascToMailAddrs.Add(lpszToAddr);
	}

	/**
	 * [���M] ���[�����M.
	 *		���[���̐ݒ�A���M���A����ȂǁA��ɐݒ肵�Ă����K�v������܂��B
	 *	@param mail ���M���e�B���M���A����ȂǁA�ݒ芮�����Ă���I�u�W�F�N�g���w�肵�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Send(const CMailContent& mail)
	{
		//���͂�����Ă���H
		if ( ! mail.IsCompletesForSend() )
		{
			SetErrorText("���ݒ荀�ڗL��");
			return false;
		}
		//POP Before SMTP
		if ( m_pPop3 != NULL )
		{
			if ( ! m_pPop3->Connect() )
			{
				SetErrorText(m_pPop3->GetErrorText());
				return false;
			}
			m_pPop3->Disconnect();
			::Sleep(m_dwPop3Wait);
		}
		//�{�̎��o���Ă���
		CAscii ascMain = mail.GetContentForSend();
		//�ڑ�
		if ( ! IsConnect() )
		{
			if ( ! Connect() )
			{
				return false;
			}
		}
		//MAIL FROM�R�}���h���s
		CAscii ascFromAddr = m_ascFromMailAddr;
		if ( ascFromAddr.IsEmpty() )
		{
			ascFromAddr = mail.ReferFromMailAddr().GetAddress();
		}
		if ( ! Command(CAscii::Fmt("MAIL FROM:<%s>", ascFromAddr)) )
		{
			return false;
		}
		//RCPT TO�R�}���h���s
		CAsciiVector vascToAddrs = m_vascToMailAddrs;
		if ( vascToAddrs.IsEmpty() )
		{
			vascToAddrs = mail.GetAddrsForSend();
		}
		loop ( i, vascToAddrs.GetSize() )
		{
			if ( ! Command(CAscii::Fmt("RCPT TO:<%s>", vascToAddrs[i])) )
			{
				return false;
			}
		}
		//DATA�R�}���h���s
		if ( ! Command("DATA") )
		{
			return false;
		}
		//�{�̑��M
		return Command(ascMain);
	}

protected:

	/**
	 * [�m�F] ���X�|���X�`�F�b�N
	 *	@param a ��M����������
	 *	@retval true ����D
	 *	@retval false �ُ�
	 */
	virtual bool IsValidResponse(const CAscii& a) const
	{
		if ( a.IsEmpty() ) 
		{
			return false;
		}
		return (a[0] == '2' || a[0] == '3');
	}

	/**
	 * [�ݒ�] �I���R�}���h
	 *	@note Command() ���g�����Ƃ�z��B
	 *	@param[out] _res ���X�|���X�������i�[�B
	 *	@retval true �����B
	 *	@retval false ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	virtual bool QuitCommand(CAscii& _res)
	{
		return Command(_res, "QUIT");
	}

	/**
	 * [�ݒ�] �ڑ�������
	 *	@note �ʏ�A�F�؏������s�Ȃ�
	 *	@retval Empty��� �����B
	 *	@retval �����L�� �G���[�B������� GetErrorText() �Ŏ擾�\�ɂȂ�B
	 */
	virtual bool OnConnect(void)
	{
		CAscii s;
		if ( Read(s) && s.Find("220") == 0 )
		{
			m_ascOpeningMessage = s;
			CAscii hel = m_isEhloMode ? "EHLO" : "HELO";
			if ( Command(CAscii::Fmt("%s %s", hel, m_ascDomain)) )
			{
				//���s���邪��ǂ݂��Ă���
				while ( true )
				{
					if ( ! Read(s) )
					{
						break;
					}
				}
				return true;
			}
		}
		return false;
	}

private:

	CAscii					m_ascOpeningMessage;///< �ڑ��������̕���
	CAscii					m_ascDomain;		///< �h���C����
	CAscii					m_ascFromMailAddr;	///< ���M�҃��[���A�h���X(�����ݒ�p)
	CAsciiVector			m_vascToMailAddrs;	///< ���惁�[���A�h���X(�����ݒ�p)
	CPop3Client*			m_pPop3;			///< POP Before SMTP
	DWORD					m_dwPop3Wait;		///< �F�،��Wait
	bool					m_isEhloMode;		///< true;EHLO false HELO �g�p
};



}; // TNB



#if 0

�N���C�A���g	HELO <satoh@some.where.com><CRLF>
�T�[�o	250 OK
�N���C�A���g	MAIL FROM:<satoh@some.where.com><CRLF>
�T�[�o	250 OK
	
�N���C�A���g	RCPT TO:<smith@another.com> <CRLF>
�T�[�o	250 OK
	
�N���C�A���g	DATA <CRLF>
�T�[�o	354 Start mail input; end with <CRLF>.<CRLF>
�N���C�A���g	����ɂ���
�N���C�A���g	�Ƃ���A�E�E�E�Ƃ������ƂŁA
�N���C�A���g	�Ԏ������܂����Ă���܂�
�N���C�A���g	<CRLF>.<CRLF>
�T�[�o	250 OK
�N���C�A���g	QUIT<CRLF>
�T�[�o	250 OK

	send.print("From: "+sender+CRLF);
	send.print("To: " + receiver+CRLF);
	send.print("Subject: " + title+CRLF);
	send.print("Comment: Unauthenticated sender"+CRLF);
	send.print("X-Mailer: Tiny SMTP Program"+CRLF);
	send.print(CRLF);
	send.print(message+CRLF);
	send.print(".");
	send.print(CRLF);
	send.flush();
#endif

#if 0
�E��ʓI�ȃ��[���G���R�[�h�́ASubject �� BASE64�A�{���� JIS �ɂȂ���
    ����悤�Ɏv���B
    Outlook Express �ł́A�G���R�[�h�Ȃ��� S-JIS �̂܂܂ł�
    Subject�A�{���� BASE64 �ł����Ȃ��ǂ߂�B
    �C���^�[�l�b�g�֑��M���郁�[���́A���Ȃ��Ƃ� BASE64 �̃G���R�[�h��
    �����������S���낤�B

  �E���[���w�b�_��BASE64 Subject �`���́A���̂Ƃ���ł���B

    =?�R�[�h�`��?�^�C�v?�^�C�g��?=
    | |         ||     ||       |_ ?= �I���R�[�h
    | |         ||     ||_ �G���R�[�h�^�C�g��������
    | |         ||     |_ ? ��؂�R�[�h
    | |         ||_ BASE64 �́uB�v
    | |         |_ ? ��؂�R�[�h
    | |_ BASE64 �́uiso-2022-jp�v
    |_ =? �J�n�R�[�h

  �E���[���{����BASE64 �G���R�[�h�w��́A���̂Ƃ���ł���B

    MIME-Version: 1.0
    Content-Type: text/plain;
    	charset="iso-2022-jp"
    Content-Transfer-Encoding: base64
#endif