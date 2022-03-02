#pragma once
/**
 *@file
 * POP3(Post Office Protocol version 3)�֌W�̃w�b�_
 *
 *	@note Winsock ���g�p���Ă��܂��B
 *
 *	@note	�}�N�� "_TnbWINSOCK2_ENABLE" ����`����Ă���ƁA Winsock2.0 ���g�p���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSocket.h"
#include "TnbMailContent.h"
#include "TnbMd5Computer.h"
#include "TnbQueueingReporter.h"
#include "TnbListQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup MAIL
 * Mail �p�N���C�A���g�x�[�X�N���X
 *
 *	@note ���̂܂܎g�����Ƃ͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbPop3Client.h
 * 
 *	@date 06/09/28 CPop3Client �𒊏ۉ����č쐬�B
 *	@date 07/06/15 �ʐM�N���X�������ł���悤�Ή�
 *	@date 08/07/31 ���M�����R�}���h�̃R�[���o�b�N��ǉ�
 *	@date 09/09/15 TnbStream �N���X���� QueueingReporter �N���X�֊����B���\�b�h�̐����B
 */
class CAbstractMailClient
{
public:

	/**@ingroup MAIL
	 * Mail�p���X�i�[�C���^�[�t�F�[�X
	 *
	 *		�T�[�o����̃��X�|���X��������E�����Ƃ��o���܂��B
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}
		/**
		 * [�ʒm] ���X�|���X��M.
		 *	@param lpszRes ���X�|���X������
		 */
		virtual void OnMailResponse(LPCSTR lpszRes) = 0;
		/**
		 * [�ʒm] �R�}���h���M.
		 *	@param lpszCmd �R�}���h������
		 */
		virtual void OnMailCommand(LPCSTR lpszCmd) = 0;
	};


	//---------------------------------


	/// �R���X�g���N�^
	CAbstractMailClient(void) : m_socket(false), m_piRep(&m_socket), m_pListener(NULL), m_responseTimeout(5000)
	{
		m_queue.SetEnvironment(m_piRep, &m_inner);
	}

	/// �f�X�g���N�^
	virtual ~CAbstractMailClient(void)
	{
		Disconnect();
	}

	/**
	 * [�擾] �G���[������
	 *	@return ������
	 */
	LPCSTR GetErrorText(void) const
	{
		return m_ascErrorText;
	}

	/**
	 * [�ݒ�] ���X�i�[�ݒ�
	 *	@param pListener ���X�i�[�B�ȗ�����ƃ��X�i�[�����ɂȂ�܂��B
	 */
	void SetListener(IListener* pListener = NULL)
	{
		m_pListener = pListener;
	}

	/**
	 * [�ݒ�] �ʐM�ݒ�.
	 *	@note Winsock ���g�p���ĒʐM���s�Ȃ����߂ɁA�T�[�o���A�|�[�g�ԍ����w�肵�܂��B
	 *	@note �{���\�b�h�����s����� SetReportInterface() �̐ݒ�͖����ɂȂ�܂��B
	 *	@param lpszName �T�[�o���B
	 *	@param wPort �|�[�g�i���o�[�B
	 */
	void SetServerName(LPCTSTR lpszName, WORD wPort)
	{
		Disconnect();
		m_socket.SetServer(CSocketAddress(wPort, lpszName));
		SetReportInterface(&m_socket);
	}

	/**
	 * [�ݒ�] �ʐM�ݒ�
	 *	@note �w��� IReport ���g�p���ĒʐM���s�Ȃ��܂��B
	 *	@note �{���\�b�h�����s����� SetServerName() �̐ݒ�͖����ɂȂ�܂��B
	 *	@param pReport �ʐM�C���^�[�t�F�[�X�B{@link IReport::Start() Start()}
	 *				�ŒʐM�J�n�ł���悤�ɐݒ肵�Ă����K�v������܂��B
	 */
	void SetReportInterface(IReport* pReport)
	{
		Disconnect();
		m_piRep = (pReport != NULL) ? pReport : m_piRep;
		m_inner.Empty();
		m_queue.SetEnvironment(m_piRep, &m_inner);
	}

	/**
	 * [�ݒ�] ���X�|���X�҂�����.
	 *	���X�|���X��҂��Ԃ�ݒ�ł��܂��B�f�t�H���g�ł� 5000ms �ł��B
	 *	@param tm ���X�|���X�҂����ԁB
	 */
	void SetResponseTimeout(DWORD tm)
	{
		m_responseTimeout = tm;
	}

	/**
	 * [�ؒf] ���[���T�[�o�ؒf.
	 *		"QUIT"�𓊂��ASocket����܂��B
	 *	@note �ڑ����ĂȂ���Ή������܂���B
	 *	@retval true ����ؒf�B
	 *	@retval false �ُ�ؒf�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool Disconnect(void)
	{
		bool r = true;
		if ( m_piRep->IsConnect() )
		{
			CAscii a;
			r = QuitCommand(a);
			loop ( i, 10 )
			{
				if ( ! m_piRep->IsConnect() )
				{
					break;
				}
				::Sleep(100);
			}
			m_piRep->Stop();
			m_inner.Empty();
		}
		return r;
	}

	/**
	 * [�ڑ�] ���[���T�[�o�ڑ�
	 *	@note �T�[�o�ɐڑ����A���[�U�F�؂܂ōs�Ȃ��܂��B
	 *	@retval true �ڑ������B
	 *	@retval false �ڑ����s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool Connect(void)
	{
		Disconnect();
		m_ascErrorText.Empty();
		if ( ! m_piRep->Start() )
		{
			SetErrorText("Server Not Found.");
			return false;
		}
		if ( ! OnConnect() )
		{
			m_piRep->Stop();
			return false;
		}
		return true;
	}

protected:

	/**
	 * [�m�F] ���X�|���X�`�F�b�N
	 *	@param a ��M����������
	 *	@retval true ����D
	 *	@retval false �ُ�
	 */
	virtual bool IsValidResponse(const CAscii& a) const = 0;

	/**
	 * [�ݒ�] �I���R�}���h
	 *	@note Command() ���g�����Ƃ�z��B
	 *	@param[out] _res ���X�|���X�������i�[�B
	 *	@retval true �����B
	 *	@retval false ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	virtual bool QuitCommand(CAscii& _res) = 0;

	/**
	 * [�ݒ�] �ڑ�������
	 *	@note �ʏ�A�F�؏������s�Ȃ�
	 *	@retval Empty��� �����B
	 *	@retval �����L�� �G���[�B������� GetErrorText() �Ŏ擾�\�ɂȂ�B
	 */
	virtual bool OnConnect(void) = 0;

	/**
	 * [��M] ���X�|���X��M
	 *	@param[out] _res ��M�����񂪊i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B������� GetErrorText() �Ŏ擾�\�ɂȂ�B
	 */
	bool Read(CAscii& _res)
	{
		bool r = m_inner.Read(_res, m_responseTimeout);
		if ( r )
		{
			TRACE1( "Mail-Command Response = [%s]\n", _res );
			if ( m_pListener != NULL )
			{
				m_pListener->OnMailResponse(_res);
			}
		}
		else
		{
			SetErrorText("Timeout.");
		}
		return r;
	}

	/**
	 * [���M] �R�}���h���M
	 *	@param lpszCmd �R�}���h�B���s�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Write(LPCSTR lpszCmd)
	{
		if ( m_piRep != NULL )
		{
			CAscii a = lpszCmd;
			a += "\r\n";
			LPCSTR lpsz = a;
			return m_piRep->Send(a.GetLength(), lpsz);
		}
		return false;
	}

	/**
	 * �R�}���h���M�����X�|���X��M
	 *	@param[out] _res ���X�|���X�����񂪊i�[����܂��B
	 *	@param[in] lpszCmd �R�}���h(���s�͂���܂���)
	 *	@retval true �����B
	 *	@retval false ���s�B������� GetErrorText() �Ŏ擾�\�ɂȂ�B
	 */
	bool Command(CAscii& _res, LPCSTR lpszCmd)
	{
		Write(lpszCmd);
		if ( m_pListener != NULL )
		{
			m_pListener->OnMailCommand(lpszCmd);
		}
		if ( ! Read(_res) )
		{
			return false;
		}
		if ( ! IsValidResponse(_res) )
		{
			SetErrorText(_res); //��M��������G���[������Ƃ���
			_res.Empty();
			return false;
		}
		return true;
	}

	/**
	 * �R�}���h���M�����X�|���X��M
	 *	@note ���X�|���X�̕����񂪗v��Ȃ��ꍇ�A������g���܂��B
	 *	@param[in] lpszCmd �R�}���h(���s�͂���܂���)
	 *	@retval true �����B
	 *	@retval false ���s�B������� GetErrorText() �Ŏ擾�\�ɂȂ�B
	 */
	bool Command(LPCSTR lpszCmd)
	{
		CAscii s;
		return Command(s, lpszCmd);
	}

	/**
	 * [�Ǎ�] �s���I�h�s�܂œǂݍ���
	 *	@param[out] _lines ���X�|���X�����B�s���I�h�s�͊܂݂܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B������� GetErrorText() �Ŏ擾�\�ɂȂ�B
	 */
	bool ReadLines(CAsciiVector& _lines)
	{
		_lines.RemoveAll();
		CAscii res;
		while ( true )
		{
			bool r = m_inner.Read(res, m_responseTimeout);
			if ( ! r )
			{
				SetErrorText("Timeout.");
				return false;
			}
			if ( res == "." )
			{
				break;
			}
			_lines.Add(res);
		}
		return true;
	}

	/**
	 * [�m�F] �ڑ��m�F
	 *	@retval true �����B
	 *	@retval false ���s�B������� GetErrorText() �Ŏ擾�\�ɂȂ�B
	 */
	bool IsConnect(void)
	{
		if ( ! m_piRep->IsConnect() )
		{
			m_ascErrorText = "Server Not Connected.";
			return false;
		}
		return true;
	}

	/**
	 * [�ݒ�] �G���[������
	 *	@param lpsz ������
	 */
	void SetErrorText(LPCSTR lpsz)
	{
		m_ascErrorText = lpsz;
	}

private:
	// �C���i�[�N���X
	class CInner : public CQueueingReporter::IListener
	{
	public:
		/// [�폜] �o�b�t�@�N���A
		void Empty(void)
		{
			m_recvLines.RemoveAll();
		}

		/**
		 * [�擾] ��M���ǂݍ���.
		 *	@param[out] _res ��M��񂪊i�[����܂��B
		 *	@param timeout ��񂪂Ȃ��ꍇ�A�ő�҂���(ms)
		 */
		bool Read(CAscii& _res, DWORD time)
		{
			CTickCount tc;
			do
			{
				if ( ! m_recvLines.IsEmpty() )
				{
					_res = m_recvLines.Take();
					return true;
				}
				::Sleep(100);
			}
			while ( ! tc.IsPassed(time) );
			return false;
		}
	protected:
		// �C�x���g
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			ASSERT( ! ev.HasData() );
			return true;
		}
		// jyy�^
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c)
		{
			if ( c.IsEmpty() )
			{
				return 0;
			}
			BYTE b = c.At(0);
			if ( b == 0 || b == 0x0A )
			{
				return 1;
			}
			loop ( i, c.GetSize() )
			{
				b = c.At(i);
				if ( b == 0 || b == 0x0D )
				{
					CAscii a;
					LPSTR lpsz = a.GetBuffer(i + 1);
					loop ( j, i )
					{
						lpsz[j] = c.At(j);
					}
					lpsz[i] = 0;
					a.ReleaseBuffer();
					m_recvLines.Add(a);
					return i + 1;
				}
			}
			return 0;
		}
	private:
		CListQueueT<CAscii> m_recvLines;	///< ��M���
	};

	CSocketClient		m_socket;			///< �\�P�b�g
	IReport*			m_piRep;			///< �ʐM�h�e
	CQueueingReporter	m_queue;			///< ��M�L���[
	CInner				m_inner;			///< ������M�N���X
	CAscii				m_ascErrorText;		///< �G���[����
	IListener*			m_pListener;		///< ���X�i�[
	DWORD				m_responseTimeout;	///< ���X�|���X�^�C���A�E�g(ms)
};



/**@ingroup MAIL
 * POP3(Post Office Protocol version 3)�N���C�A���g�����N���X
 *
 *	@par�g�p��
 *	\code
 *		CPop3Client pop3;
 *		// �T�[�o�ݒ�
 *		pop3.SetServerName(_T("Localhost"));
 *		pop3.SetUser(_T("master"), _T("master"));
 *		pop3.SetKind(pop3.APOP);
 *			;
 *		// �ڑ�
 *		pop3.Connect();
 *		TRACE1("[%s]\n", pop3.GetOpeningMessage());
 *			;
 *		CPop3Client::TParam t = pop3.GetMailCount();	//���[�����擾		
 *		CPop3Client::CParamVector vp = pop3.GetMailList();	// �e���[����No���T�C�Y
 *		CAscii ascUIDL = pop3.GetUidl(1); // No1�̃��[����UIDL�擾
 *		CAsciiVector v = pop3.GetMailContent(1); // No1�̃��[���̓��e�擾
 *		loop ( i, v.GetSize() )
 *		{
 *			CJisStr s;
 *			s.SetOfJis(v[i]);
 *			TRACE1("%s\n", s.ToAscii());
 *		}
 *		CMailContent mail;
 *		mail.SetContentFromReceive(v); // ���
 *	\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbPop3Client.h
 * 
 *	@date 06/09/27	�V�K�쐬
 *	@date 06/09/28	���ۉ������N���X���g���悤�ɕύX�B
 *	@date 07/06/15	�ʐM�N���X�������ł���悤�Ή�
 *	@date 09/09/15	�e�N���X�̐����ɒǏ]�B
 */
class CPop3Client : public CAbstractMailClient
{
	DEFSUPER(CAbstractMailClient);
public:
	
	/// �F�؎�ތ^
	enum EKind
	{
		POP3,	///< POP3
		APOP	///< APOP
	};

	/// POP3�v���g�R�������N���X�p���U���g�^
	struct TParam
	{
		int number;	///< ���[���i���o�[
		int size;	///< �T�C�Y
	};

	typedef CVectorT<TParam> CParamVector;	///< ���U���g�p�����[�^�̔z��^�錾


	//----------------------------------


	/**
	 * �R���X�g���N�^
	 *	@param kind �F�ؕ��@�B�ȗ������ POP3�ł�( SetKind() �ōĐݒ肷�邱�Ƃ��\)�B
	 */
	CPop3Client(EKind kind = POP3) : _super(), m_kind(kind) 
	{
	}

	/// �f�X�g���N�^
	virtual ~CPop3Client(void)
	{
		Disconnect();
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
	 *	@param port �|�[�g�i���o�[�B�ȗ������110�B
	 */
	void SetServerName(LPCTSTR lpszName, WORD port = 110)
	{
		_super::SetServerName(lpszName, port);
	}

	/**
	 * [�ݒ�] ���[�U�ݒ�
	 *	@param lpszUser ���[�U���B
	 *	@param lpszPassword �p�X���[�h�B
	 */
	void SetUser(LPCTSTR lpszUser, LPCTSTR lpszPassword)
	{
		m_ascUser = lpszUser;
		m_ascPassword = lpszPassword;
	}

	/**
	 * [�ݒ�] �F�ؕ��@�ݒ�
	 *	@param kind �F�ؕ��@�B
	 */
	void SetKind(EKind kind)
	{
		m_kind = kind;
	}

	/**
	 * [�v��] NOOP���M
	 *	@note	�ڑ��m�F�Ƃ��Ďg���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool RequestNoop(void)
	{
		if ( IsConnect() )
		{
			return Command("NOOP");
		}
		return false;
	}

	/**
	 * [�폜] ���[���폜�v��
	 *	@note	�w��� number�̃��[���폜�v�����o���܂��B���ۍ폜�����̂́A Disconnect �����Ƃ��ł��B
	 *			���̑O�ɁA CancelDelete() ���g�����ƂŁA�폜�����������Ƃ��o���܂��B
	 *	@param number �i���o�[�B GetMailList() �̖߂�z��� TParam::number �̂����ꂩ���w��\�ł��B
	 *	@retval true �v�������B
	 *	@retval false �v�����s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool RequestDelete(int number)
	{
		if ( IsConnect() )
		{
			return Command(CAscii::Fmt("DELE %d", number));
		}
		return false;
	}

	/**
	 * [�폜] ���[���폜�v��������
	 *	@note	�ڑ����Ă��獡�܂ł� RequestDelete() ��S�ăL�����Z�����܂��B
	 *	@retval true �v�������������B
	 *	@retval false �v�����������s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool CancelDelete(void)
	{
		if ( Connect() )
		{
			return Command("RSET");
		}
		return false;
	}

	/**
	 * [�擾] ���[�����擾
	 *	@note �T�[�o�̃��[�����A�T�C�Y���擾���܂��B
	 *	@param[out] _param �擾��񂪊i�[����܂��B TParam::number �Ƀ��[�����A TParam::size �ɃT�C�Y���i�[�B
	 *	@retval true �����B
	 *	@retval false ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool GetMailCount(TParam& _param)
	{
		_param.number = 0;
		_param.size = 0;
		if ( IsConnect() )
		{
			CAscii res;
			if ( Command(res, "STAT") )
			{
				m_GetTwoInt(_param, res, 4);
				return true;
			}
		}
		return false;
	}

	/**
	 * [�擾] ���[�����X�g�擾
	 *	@note �T�[�o�ɂ��郁�[���i���o�[�A�y�уT�C�Y�̃��X�g���擾���܂��B
	 *	@param[out] _list ���[�����X�g�B�e�v�f�� number �Ƀ��[���i���o�[�Asize �ɃT�C�Y���i�[�B
	 *	@retval true �����B
	 *	@retval false ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool GetMailList(CParamVector& _list)
	{
		_list.RemoveAll();
		if ( IsConnect() )
		{
			CAscii res;
			if ( Command(res, "LIST") )
			{
				size_t c = res.ToInt(4);
				CAsciiVector va;
				if ( ReadLines(va) )
				{
					if ( c == va.GetSize() )
					{
						loop ( i, va.GetSize() )
						{
							TParam t;
							m_GetTwoInt(t, va[i], 0);
							_list.Add(t);
						}
						return true;
					}
					SetErrorText("invalid list"); // �v���� ���擾�ł��Ȃ������B
				}
			}
		}
		return false;
	}

	/**
	 * [�擾] ���[���擾
	 *	@param[out] _mail �擾�������[�����e���i�[����܂��B
	 *	@param[in] number �i���o�[�B GetMailList() �̖߂�z��� TParam::number �̂����ꂩ���w��\�ł��B
	 *	@param[in] line ���o���s�B�ȗ�����ƁA�S�ē��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool GetMailContent(CAsciiVector& _mail, int number, int line = 0)
	{
		_mail.RemoveAll();
		if ( IsConnect() )
		{
			CAscii cmd;
			if ( line <= 0 )
			{
				cmd.Format("RETR %d", number);
			}
			else
			{
				cmd.Format("TOP %d %d", number, line);
			}
			if ( Command(cmd) )
			{
				return ReadLines(_mail);
			}
		}
		return false;
	}

	/**
	 * [�擾] UIDL�擾
	 *	@param[out] _uidl �擾���� UIDL �����񂪊i�[����܂��B
	 *	@param[in] number �i���o�[�B GetMailList() �̖߂�z��� TParam::number �̂����ꂩ���w��\�ł��B
	 *	@retval Empty��� ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 *	@retval true �����B
	 *	@retval false ���s�i���R�� GetErrorText() �Ŏ擾�\.�j�B
	 */
	bool GetUidl(CAscii& _uidl, int number)
	{
		_uidl.Empty();
		if ( IsConnect() )
		{
			CAscii cmd;
			cmd.Format("UIDL %d", number);
			CAscii res;
			if ( Command(res, cmd) )
			{
				if ( res.ToInt(4) == number )
				{
					INT_PTR p = res.ReverseFind(' ');
					_uidl = res.Mid(p + 1);
					return true;
				}
				SetErrorText("invalid uidl"); // �v���� uidl �ƈقȂ�B
			}
		}
		return false;
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
		return a.Find("+OK") == 0;
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
		bool r = Read(s);
		if ( r && s.GetAt(0) == '+' )
		{
			m_ascOpeningMessage = s.Mid(4);
			if ( m_kind == POP3 )
			{
				//== POP3
				if ( Command("USER " + m_ascUser) )
				{
					if ( Command("PASS " + m_ascPassword) )
					{
						return true;
					}
				}
			}
			else
			{
				//== APOP
				INT_PTR is = s.Find('<');
				INT_PTR ie = s.Find('>', is);
				if ( is >= 0 && ie > is )
				{
					CAscii a = s.Mid(is, ie - is + 1);
					CAscii h = CMd5Computer::ComputeHash(a + m_ascPassword).GetString();
					if ( Command(CAscii::Fmt("APOP %s %s", m_ascUser, h)) )
					{
						return true;
					}
				}
			}
		}
		return false;
	}

private:

	CAscii		m_ascOpeningMessage;	///< �ڑ��������̕���
	CAscii		m_ascUser;				///< ���[�U��
	CAscii		m_ascPassword;			///< �p�X���[�h
	EKind		m_kind;					///< �F�؎��

	/**
	 * ��������.
	 *	@note �w��ʒu���� �u���l�A�X�y�[�X�A���l�v�ƂȂ��Ă��镶���񂩂琔�l�Q�����o���܂��B
	 *	@param[out] _param ��͌��ʂ��i�[����܂��B
	 *	@param[in] asc ��������
	 *	@param[in] iOffset ��͊J�n�ʒu
	 */
	void m_GetTwoInt(TParam& _param, const CAscii& asc, int iOffset)
	{
		INT_PTR p = asc.ReverseFind(' ');
		_param.number = asc.ToInt(iOffset);
		_param.size = asc.ToInt(p + 1);
	}

};



}; // TNB



#if 0
USER�EPASS �R�}���h�ɂ���ă��[�U�F�؂��s���B 
LIST �R�}���h�Ń��[���ꗗ���󂯎��B 
���ꂩ��e���[���ɂ��āA 
RETR �R�}���h�Ń��[���̓��e���󂯎��A 
DELE �R�}���h�Ń��[�����T�[�o����폜����B 
QUIT �R�}���h�ŒʐM�I��


�T�[�o			<wait for connection on TCP port 110>
�N���C�A���g	<open connection>
�T�[�o			+OK POP3 server ready <1896.697170952@dbc.mtview.ca.us>
--
�N���C�A���g	USER ���[���A�J�E���g��<CRLF>
�T�[�o			+OK <CRLF>
�N���C�A���g	PASS �p�X���[�h<CRLF>
�T�[�o			+OK <CRLF>
--
�N���C�A���g	APOP mrose c4c9334bac560ecc979e58001b3e22fb
�T�[�o			+OK mrose's maildrop has 2 messages (320 octets)
--

�N���C�A���g	STAT
�T�[�o			+OK 2 320
�N���C�A���g	LIST
�T�[�o			+OK 2 messages (320 octets)
�T�[�o			1 120
�T�[�o			2 200
�T�[�o			.

�N���C�A���g	RETR 1
�T�[�o			+OK 120 octets
�T�[�o			<the POP3 server sends message 1>
�T�[�o			.

�N���C�A���g	DELE 1
�T�[�o			+OK message 1 deleted
�N���C�A���g	RETR 2
�T�[�o			+OK 200 octets
�T�[�o			<the POP3 server sends message 2>
�T�[�o			.
�N���C�A���g	DELE 2
�T�[�o			+OK message 2 deleted

�N���C�A���g	QUIT
�T�[�o			+OK dewey POP3 server signing off (maildrop empty)
�N���C�A���g	<close connection>
�T�[�o			<wait for next connection>
#endif

