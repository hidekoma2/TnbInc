#pragma once
/**
 *@file
 * ���[���R���e���c�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"
#include "TnbStrOperator.h"
#include "TnbJisStr.h"
#include "TnbBase64Str.h"
#include "TnbTime.h"
#include "TnbFile.h"
#include "TnbMap.h"
#include "TnbStrEx.h"



//TNB Library
namespace TNB
{



/**@ingroup MAIL
 * ���[���A�h���X�Ǘ��N���X
 *
 *	@note ��g�̃��[���A�h���X�Ɩ��O���Ǘ����Ă��܂��B
 *
 *	@note �����\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMailContent.h
 * 
 *	@date 06/10/06 �V�K�B
 *	@date 08/07/30 Sender�\�L�T�|�[�g�B
 */
class CMailAddress
{
	CAscii		m_ascAddress;		///< �A�h���X(ASCII�̂�)
	CAscii		m_ascName;			///< �Ȃ܂�
public:

	/// �R���X�g���N�^
	CMailAddress(void) 
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param ascAddress �A�h���X(ASCII)
	 *	@param ascName ���O�B�ȗ��\�B
	 */
	CMailAddress(CAscii ascAddress, CAscii ascName = "") : m_ascAddress(ascAddress), m_ascName(ascName)
	{
	}

	/**
	 * [�擾] �A�h���X�擾
	 *	@return �A�h���X
	 */
	CAscii GetAddress(void) const
	{
		return m_ascAddress;
	}

	/**
	 * [�擾] ���O�擾
	 *	@return ���O
	 */
	CAscii GetName(void) const
	{
		return m_ascName;
	}

	/**
	 * [�ݒ�] �A�h���X�ݒ�
	 *	@param ascAddress �A�h���X(ASCII)
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetAddress(const CAscii& ascAddress)
	{
		m_ascAddress = ascAddress;
		return IsValid();
	}

	/**
	 * [�ݒ�] ���O�ݒ�
	 *	@param ascName ���O
	 */
	void SetName(const CAscii& ascName)
	{
		m_ascName = ascName;
	}

	/**
	 * [�m�F] ������m�F
	 *	@retval true �{�C���X�^���X�������Ă�����͗L���B
	 *	@retval false �����i���L��j
	 */
	bool IsValid(void) const
	{
		if ( ! m_ascAddress.IsEmpty() && STRLIB::IsAsciiOnly(m_ascAddress) )
		{
			return true;
		}
		return false;
	}
	
	/**
	 * [�擾] ����\�L.
	 *	@note ���O ���A�h���X�� �Ƃ����`�̕������Ԃ��܂��B���O���Ȃ��ꍇ�A�A�h���X�̂ݕԂ��܂��i�����Ȃ��j�B
	 *	@return �\�L
	 */
	CAscii GetSenderPath(void) const
	{
		if ( m_ascName.IsEmpty() )
		{
			return m_ascAddress;
		}
		return CAscii::Fmt("%s <%s>", m_ascName, m_ascAddress);
	}

	/**
	 * [�擾] ����\�L
	 *	@param path ���O ���A�h���X�� �Ƃ����`�̕���������Ă����܂��B�������Ȃ��ꍇ�A�G���[�ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetSenderPath(LPCSTR path)
	{
		CAscii s = path;
		INT_PTR f0 = s.Find('<');
		INT_PTR f1 = s.Find('>', f0);
		if ( f0 >= 0 && f1 > f0 )
		{
			m_ascName = s.Left(f0);
			m_ascName.TrimRight(' ');
			m_ascAddress = s.Mid(f0 + 1, (f1 - f0) - 1);
			return true;
		}
		return false;
	}
};



/**@ingroup MAIL
 * ���[�����e�Ǘ��N���X
 *
 *		���o�ҁA����ҁA�����A���e���Ǘ����܂��B
 *		�Ǘ���񂩂烁�[�����M�p�̃e�L�X�g���쐬���邱�Ƃ��\�ł�
 *		(��M�́A{@link TNB::CPop3Client POP3�N���X}���p�ӂ��Ă���܂�)�B
 *		��M�����e�L�X�g�𕪐͂��A�{�N���X�ɋL�����邱�Ƃ��\�ł�
 *		(���M�́A{@link TNB::CSmtpClient SMTP�N���X}���p�ӂ��Ă���܂�)�B
 *
 *	@note �C���X�^���X�̕������\�ł��B
 *
 *	@todo ��̓f�[�^�̃G���[���̏����A�y�ђʒm���������ł��B
 *
 *	@par�g�p��
 *	<table>
 *	<tr><td><pre>
 *		CMailContent mail;
 *		mail.SetFromMailAddr("boss��tnb.co.jp", "�ڂ�");		
 *		mail.AddToMailAddr("oyabun��tnb.co.jp", "����Ԃ�");
 *		mail.AddCcMailAddr("master��tnb.co.jp", "�}�X�^�[");
 *		mail.SetSubject("��̌�");
 *		mail.AddSendTextData("����ɂ���\r\n��̌��A�ǂ��ł����H");
 *		TRACE1("%s\n", mail.GetContentForSend());
 *	</pre></td></tr>
 *	<tr><td><pre>
 *		From: =?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss��tnb.co.jp>
 *		To: =?ISO-2022-JP?B?GyRCJCokZCRWJHMbKEI=?= <oyabun��tnb.co.jp>		
 *		Cc: =?ISO-2022-JP?B?GyRCJV4lOSU/ITwbKEI=?= <master��tnb.co.jp>
 *		Subject: =?ISO-2022-JP?B?GyRCTmMkTjdvGyhC?=
 *		MIME-Version: 1.0
 *		X-Mailer: TNB Tiny Mailer
 *		Content-Type: text/plain; charset="ISO-2022-JP"
 *		Content-Transfer-Encoding: 7bit
 *
 *		.$B$3$s$K$A$o.(B
 *		.$BNc$N7o!"$I$&$G$9$+!).(B
 *		�D
 *
 *	</pre></td></tr>
 *	</table>
 *
 *	@par�K�v�t�@�C��
 *			TnbMailContent.h
 * 
 *	@date 06/10/05 �쐬�����쐬
 *	@date 06/10/06 SJIS������̂܂�BASE64�����Ă����B�C���B
 *	@date 06/10/30 �ǂݍ���(���)�A�����B
 *	@date 06/10/31 �ǂݍ���(���)���ʁA�擾���\�b�h�����B
 */
class CMailContent
{
	CAscii						m_ascMailerName;	///< ���[���[��
	CMailAddress				m_addrFrom;			///< ���M�҃��[���A�h���X
	CMailAddress				m_addrReply;		///< ���v���C���[���A�h���X
	enum{ TO = 0, CC = 1, BCC = 2, KINDMAX = 3 };
	CVectorT<CMailAddress>		m_aaddrTo[KINDMAX];	///< ���� [0];To [1];Cc [2];Bcc �B
	CAscii						m_ascSubject;		///< �^�C�g��
	CAsciiVector				m_vascContents;		///< ���e�e�L�X�g
	CMapT<CAscii,CByteVector>	m_mapFileDatas;		///< �t�@�C�����e���t�@�C����,���e��
	CAsciisMap					m_mapUnknownCmd;	///< ��͎��A�s���ȃR�}���h&�p�����[�^

	/**
	 * [�ϊ�] ��p��BASE64�^����f�R�[�h.
	 *	�y=?ISO-2022-JP?B?�`�`�`?=�z�̕������f�R�[�h���A���́A���̂܂܁B
	 *	@param ascSrc ���f�[�^
	 *	@return �f�R�[�h����
	 */
	CAscii m_FromBase64(const CAscii& ascSrc) const
	{
		CAscii ascRc;
		const int TAGSIZE = sizeof("=?ISO-2022-JP?B?") - 1;
		CBase64Str base64;
		CAscii asc = ascSrc;
		while ( ! asc.IsEmpty() )
		{
			INT_PTR f = asc.Find("=?");
			if ( f < 0 )
			{	
				break;
			}
			INT_PTR e = asc.Find("?=", f);
			if ( e < 0 )
			{
				break;
			}
			if ( f > 0 )
			{
				ascRc += asc.Left(f);
			}
			if ( asc.Mid(f, TAGSIZE).IsEqualNoCase("=?ISO-2022-JP?B?") )
			{
				CJisStr jis;
				jis.SetOfJis(base64.DecodeForStr(asc.Mid(f + TAGSIZE, e - f - TAGSIZE)));
				ascRc += jis.ToAscii();
			}
			else
			{
				ascRc += asc.Left(e + 2);
			}
			asc = asc.Mid(e + 2);
		}
		ascRc += asc;
		asc.Empty();
		return ascRc;
	}

	/**
	 * [�ϊ�] ��p��BASE64�^�ɃG���R�[�h.
	 *   =?�R�[�h�`��?�^�C�v?�^�C�g��?=
	 *   | |         ||     ||       |_ ?= �I���R�[�h
	 *   | |         ||     ||_ �G���R�[�h�^�C�g��������
	 *   | |         ||     |_ ? ��؂�R�[�h
	 *   | |         ||_ BASE64 �́uB�v
	 *   | |         |_ ? ��؂�R�[�h
	 *   | |_ BASE64 �́uiso-2022-jp�v
	 *   |_ =? �J�n�R�[�h
	 *	@note 30�����ȏ�̏ꍇ�A���s����ŕ������č쐬���܂��B
	 *	@param ascSrc �ϊ��O
	 *	@return �ϊ���
	 */
	CAscii m_ToBase64(const CAscii& ascSrc) const
	{
		CAscii ascRc;
		if ( ! ascSrc.IsEmpty() )
		{
			CAscii asc = ascSrc;
			while ( asc.GetLength() > 30 )
			{
				CAscii a = asc.Left(30);
				if ( STRLIB::IsAsciiOnly(a) && a.Find(',') < 0 )
				{
					ascRc += CAscii::Fmt("%s\r\n ", a);
				}
				else
				{
					ascRc += CAscii::Fmt("=?ISO-2022-JP?B?%s?=\r\n ", CBase64Str().Encode(CJisStr::ToJis(a)));
				}
				asc = asc.Mid(a.GetLength());
			}
			if ( STRLIB::IsAsciiOnly(asc) && asc.Find(',') < 0 )
			{
				ascRc += asc;
			}
			else
			{
				ascRc += CAscii::Fmt("=?ISO-2022-JP?B?%s?=", CBase64Str().Encode(CJisStr::ToJis(asc)));
			}
		}
		return ascRc;
	}

	/**
	 * �A�h���X�\�L�ϊ�
	 *	@note "���O <�A�h���X>" �Ƃ����\�L�ɂ��܂��B
	 *	@param addr �A�h���X���
	 *	@return ���[���p�\�L
	 */
	CAscii m_FmtAddr(const CMailAddress& addr) const
	{
		return CAscii::Fmt("%s <%s>", m_ToBase64(addr.GetName().TrimRight()), addr.GetAddress());
	}

	/**
	 * ��ؕ�����쐬
	 *	@note �R�[�����ɈقȂ镶������쐬����̂ŁA�擾���ċL�����Ďg���悤�ɂ��Ă��������B
	 *	@return ������
	 */
	CAscii m_MakeMultiPartString(void) const
	{
		CSystemTime time = CSystemTime::GetCurrent();
		return CAscii::Fmt("-----MultiPart_%s_", CBase64Str().Encode(CAscii(time.Format())));
	}

	//-- �ȉ��f�[�^��͗p

	/*
	 * �w�b�_����.
	 *	�P�j��s������܂ł��w�b�_�ƔF��.
	 *	�Q�j�󕶎��Ŏn�߂�s����̍s�̑����Ƃ���.
	 *	�R�jascContent ����w�b�_�����폜�B
	 *	@param[in,out] _ascContent �R���e���c
	 *	@return �w�b�_
	 */
	CAsciiVector m_SeparateHead(CAsciiVector& _ascContent)
	{
		CAsciiVector vaHead;
		size_t i = 0;
		while ( true )
		{
			if ( i >= _ascContent.GetSize() )
			{
				// �Ō�܂ōs�����B
				ASSERT(false);//�S���w�b�_�H���Ƃ�������
				_ascContent.RemoveAll();
				break;
			}
			const CAscii& asc = _ascContent[i];
			if ( asc.IsEmpty() )
			{
				_ascContent = _ascContent.Mid(i + 1);
				break;
			}
			if ( asc[0] == ' ' || asc[0] == '\t' )
			{
				//= ����s�A�O�̃R�}���h�ɘA��
				if ( ! vaHead.IsEmpty() )
				{
					CAscii a = asc;
//					a.TrimLeft(' ').TrimLeft('\t');
					a.TrimLeft(" \t");
					vaHead[vaHead.GetSize() - 1] += a;
				}
				else
				{
					ASSERT(false); // �������邵���E�E�E
				}
			}
			else if ( asc.Find(':') >= 1 )
			{
				//= �p�����[�^����
				vaHead.Add(asc);
			}
			else
			{
				//= ����ȊO.. �������邵��...
				ASSERT(false); 
			}
			i++;
		}
		return vaHead;
	}

	/**
	 * �A�h���X(From,To �Ȃ�)�̍s�����
	 *	@note "=?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss@tnb.co.jp>,aaaa <sub@tnb.co.jp>" �̂悤�ȃf�[�^
	 */
	CVectorT<CMailAddress> m_AnalyzAddrLine(const CAscii& aParam)
	{
		CVectorT<CMailAddress> vAddr;
		CAsciiVector va = TNB::CAsciiOperator::SeparatePeriod(aParam, ',');
		loop ( i, va.GetSize() )
		{
			CAscii asc = m_FromBase64(va[i]);
			INT_PTR f = asc.ReverseFind('<');
			if ( f >= 0 )
			{
				INT_PTR e = asc.Find('>', f);
				if ( e >= 0 )
				{
					TRACE2A("Found: MailAddr[%s] Name[%s]\n", asc.Mid(f + 1, e - f - 1), asc.Left(f).TrimRight());
					vAddr.Add(CMailAddress(asc.Mid(f + 1, e - f - 1), asc.Left(f).TrimRight()));
					continue;
				}
			}
			vAddr.Add(CMailAddress("", asc));
		}
		return vAddr;
	}

	/**
	 * �w�b�_�̈�s����́B
	 *	@param aCmd �R�}���h�B
	 *	@param aParam �p�����[�^�B
	 *	@retval true ��͍ς�
	 *	@retval false ��͑ΏۊO�A��͎��s
	 */
	bool m_AnalyzHeadLine(const CAscii& aCmd, const CAscii& aParam)
	{
		bool boRc = false;
		if ( aCmd.IsEqual("from") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				ASSERT(vAddr.GetSize() == 1);
				boRc = SetFromMailAddr(vAddr[0]);
			}
		}
		else if ( aCmd.IsEqual("reply-to") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				ASSERT(vAddr.GetSize() == 1);
				boRc = SetReplyMailAddr(vAddr[0].GetAddress());
			}
		}
		else if ( aCmd.IsEqual("to") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				boRc = true;
				loop ( i, vAddr.GetSize() )
				{
					boRc &= AddToMailAddr(vAddr[i]);
				}
			}
		}
		else if ( aCmd.IsEqual("cc") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				boRc = true;
				loop ( i, vAddr.GetSize() )
				{
					boRc &= AddCcMailAddr(vAddr[i]);
				}
			}
		}
		else if ( aCmd.IsEqual("bcc") )
		{
			CVectorT<CMailAddress> vAddr = m_AnalyzAddrLine(aParam);
			if ( vAddr.GetSize() >= 1 )
			{
				boRc = true;
				loop ( i, vAddr.GetSize() )
				{
					boRc &= AddBccMailAddr(vAddr[i]);
				}
			}
		}
		else if ( aCmd.IsEqual("subject") )
		{
			CAscii a = m_FromBase64(aParam);
			TRACE1A("Found: Subject[%s]\n", a);
			SetSubject(a);
			boRc = true;
		}
		else if ( aCmd.IsEqual("x-mailer") )
		{
			TRACE1A("Found: X-Mailer[%s]\n", aParam);
			SetMailerName(aParam);
			boRc = true;
		}
		return boRc;
	}

	/**
	 * �p�����[�^�s���
	 *	@note [text/plain; charset="ISO-2022-JP"]��	
	 *			[text/plain],[charset],[ISO-2022-JP] �Ƃ����悤�ɕ������܂��B
	 *			���ł� BASE64�ł���΁A�f�R�[�h���܂��B
	 *			���ł� [0] [1] �͏����������܂��B
	 */
	CAsciiVector m_AnalyzParamLine(const CAscii& aParam)
	{
		CAsciiVector va;
		INT_PTR f = aParam.Find(';');
		if ( f < 0 )
		{
			va.Invalid();
			return va;
		}
		INT_PTR e = aParam.Find('=', f);
		if ( e < 0 )
		{
			va.Invalid();
			return va;
		}
		va.Add(LowerString(aParam.Left(f)));
		va.Add(LowerString(aParam.Mid(f + 1, e - f - 1).TrimLeft()));
		CAscii a = aParam.Mid(e + 1);
//		a.TrimLeft().TrimLeft('\042');
//		a.TrimRight().TrimRight('\042');
		a.Trim(" \042");
		va.Add(m_FromBase64(a));
		TRACE3A("Analyz Param: %s; %s=\042%s\042\n", va[0], va[1], va[2]);
		return va;
	}

	/**
	 * [���] ��M�f�[�^���
	 *	@param[in,out] _vaContent ��M���e�B
	 *	@param[out] _mapUnknownCmd �m��Ȃ��p�����[�^
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_SetContent(CAsciiVector& _vaContent, CAsciisMap& _mapUnknownCmd)
	{
		//= �w�b�_�𕪗�
		CAsciiVector vaHead = m_SeparateHead(_vaContent);
		//= �w�b�_�����
		loop ( i, vaHead.GetSize() )
		{
			const CAscii& a = vaHead[i]; 
			CAscii aParam;
			CAscii aCmd = LowerString(a.FindCut(':', &aParam));
			aParam.TrimLeft();
			TRACE2A("Found: Cmd[%s] = [%s]\n", aCmd, aParam);
			//
			if ( ! m_AnalyzHeadLine(aCmd, aParam) )
			{
				_mapUnknownCmd[aCmd] = aParam;
			}
		}
		//
		if ( ! _mapUnknownCmd.HasKey("content-type") )
		{
			return false;
		}
		CAsciiVector vaType = m_AnalyzParamLine(_mapUnknownCmd["content-type"]);
		if ( vaType.GetSize() == 3 && vaType[0].Find("multipart") == 0 && vaType[1].IsEqual("boundary") )
		{
			//== �}���`�p�[�g���I
			CAscii aBound = CAscii::Fmt("--%s", vaType[2]);
			CAscii aBoundLast = aBound + "--";
			CAsciisMap mapTempCmd;
			INDEX line = _vaContent.Find(aBound) + 1;
			bool boCanNext = true;
			while ( boCanNext )
			{
				INDEX f = _vaContent.Find(aBound, line);
				if ( f == INVALID_INDEX )
				{
					f = _vaContent.Find(aBoundLast, line);
					if ( f == INVALID_INDEX )
					{
						return false;
					}
					boCanNext = false;
				}
				mapTempCmd.RemoveAll();
				if ( ! m_SetContent(_vaContent.Mid(line, f - line), mapTempCmd) )	//�ċA�Ăяo��
				{
					return false;
				}
				line = f + 1;
			}
			return true;
		}
		if ( vaType.GetSize() == 3 && vaType[0].Find("text") == 0 && vaType[1].IsEqual("charset") )
		{
			//== �e�L�X�g�p�[�g���I
			ASSERT(! vaType[2].IsEmpty());
			ASSERT(! _mapUnknownCmd["content-transfer-encoding"].IsEmpty());
			CAscii aRc;
			CJisStr jis;
			loop ( i, _vaContent.GetSize() )
			{
				const CAscii& a = _vaContent[i];
				if ( a.IsEqual(".") )
				{
					break;
				}
				if ( i != 0 )
				{
					aRc += "\r\n";
				}
				if ( a.IsEqual("..") )
				{
					aRc += ".";
				}
				else
				{
					jis.SetOfJis(a);
					aRc += jis.ToAscii();
				}
			}
			AddSendTextData(aRc);
			return true;
		}
		if ( ! _mapUnknownCmd.HasKey("content-disposition") )
		{
			return false;
		}
		CAsciiVector vaDisp = m_AnalyzParamLine(_mapUnknownCmd["content-disposition"]);
		if ( vaDisp.GetSize() != 3 )
		{
			return false;
		}
		if ( vaDisp[0].IsEqual("attachment") && vaDisp[1].IsEqual("filename") )
		{
			ASSERT(_mapUnknownCmd["content-transfer-encoding"].IsEqualNoCase("base64"));
			CAscii a;
			loop ( i, _vaContent.GetSize() )
			{
				a += _vaContent[i];
			}
			CBase64Str base64;
			AddSendBinaryData(vaDisp[2], base64.Decode(a));
			return true;
		}
		return false;
	}

public:

	/// �R���X�g���N�^
	CMailContent(void) : m_ascMailerName("TNB Tiny Mailer") 
	{
	}

	/// �f�X�g���N�^
	~CMailContent(void)	{}

	/**
	 * [�ݒ�] �ݒ菉����.
	 *	@note �R���X�g���N�g������Ԃɖ߂��܂��B
	 */
	void Init(void)
	{
		CMailContent s;
		*this = s;
	}

	/**
	 * [�ݒ�] ���M���ݒ�
	 *	@note ���M�̂��߂ɕK�{�Ȑݒ�ł��B
	 *	@param ascFromAddr �A�h���X(Ascii�����̂�)�B
	 *	@param ascFromName ���[�U���B�ȗ��\�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetFromMailAddr(const CAscii& ascFromAddr, const CAscii& ascFromName = "")
	{
		m_addrFrom.SetAddress(ascFromAddr);
		m_addrFrom.SetName(ascFromName);
		return m_addrFrom.IsValid();
	}

	/**
	 * [�ݒ�] ���M���ݒ�
	 *	@note ���M�̂��߂ɕK�{�Ȑݒ�ł��B
	 *	@param addr ���[���A�h���X
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetFromMailAddr(const CMailAddress& addr)
	{
		m_addrFrom = addr;
		return m_addrFrom.IsValid();
	}

	/**
	 * [�ݒ�] ���v���C���ݒ�.
	 *	@note NULL���w�肷��Ɖ����ɂȂ�܂��B
	 *	@param ascReply ���v���C�惁�[���A�h���X(Ascii�����̂�)�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetReplyMailAddr(const CAscii& ascReply = "")
	{
		return m_addrReply.SetAddress(ascReply);
	}

	/**
	 * [�ݒ�] ���[���[���ݒ�
	 *	@note �ݒ肵�Ȃ��ƁhTNB Tiny Mailer�h�ɂȂ��Ă��܂��B
	 *	@param ascMailerName ���[�����B
	 */
	void SetMailerName(const CAscii& ascMailerName)
	{
		m_ascMailerName = ascMailerName;
	}

	/**
	 * [�ݒ�] ���於�ݒ菉����
	 *	@note AddToMailAddr() , AddCcMailAddr() , AddBccMailAddr() �Őݒ肵��������N���A���܂��B
	 */
	void InitToMailAddr(void)
	{
		m_aaddrTo[TO].RemoveAll();
		m_aaddrTo[CC].RemoveAll();
		m_aaddrTo[BCC].RemoveAll();
	}

	/**
	 * [�ݒ�] ���於�ݒ�
	 *	@note ���M�̂��߂ɕK�{�Ȑݒ�ł��B�K����l�͐ݒ肵�Ă��������B
	 *	@note �Ώۂ������l����ꍇ�A�{���\�b�h��l����g�p����K�v������܂��B
	 *	@param addr ����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddToMailAddr(const CMailAddress& addr)
	{
		if ( addr.IsValid() )
		{
			return m_aaddrTo[TO].Add(addr) != INVALID_INDEX;
		}
		return false;
	}

	/**
	 * [�ݒ�] ���於�ݒ�
	 *	@note ���M�̂��߂ɕK�{�Ȑݒ�ł��B�K����l�͐ݒ肵�Ă��������B
	 *	@note �Ώۂ������l����ꍇ�A�{���\�b�h��l����g�p����K�v������܂��B
	 *	@param ascToAddr ����A�h���X(Ascii�����̂�)�B
	 *	@param ascToName ���惆�[�U���B�ȗ��\�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddToMailAddr(const CAscii& ascToAddr, const CAscii& ascToName = "")
	{
		return AddToMailAddr(CMailAddress(ascToAddr, ascToName));
	}

	/**
	 * [�ݒ�] ���於�ݒ�(CC)
	 *	@note �Ώۂ������l����ꍇ�A�{���\�b�h��l����g�p����K�v������܂��B
	 *	@param addr ����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddCcMailAddr(const CMailAddress& addr)
	{
		if ( addr.IsValid() )
		{
			return m_aaddrTo[CC].Add(addr) != INVALID_INDEX;
		}
		return false;
	}

	/**
	 * [�ݒ�] ���於�ݒ�(CC)
	 *	@note �Ώۂ������l����ꍇ�A�{���\�b�h��l����g�p����K�v������܂��B
	 *	@param ascCcAddr ����A�h���X(Ascii�����̂�)�B
	 *	@param ascCcName ���惆�[�U���B�ȗ��\�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddCcMailAddr(const CAscii& ascCcAddr, const CAscii& ascCcName = "")
	{
		return AddCcMailAddr(CMailAddress(ascCcAddr, ascCcName));
	}

	/**
	 * [�ݒ�] ���於�ݒ�(BCC)
	 *	@note �Ώۂ������l����ꍇ�A�{���\�b�h��l����g�p����K�v������܂��B
	 *	@param addr ����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddBccMailAddr(const CMailAddress& addr)
	{
		if ( addr.IsValid() )
		{
			return m_aaddrTo[BCC].Add(addr) != INVALID_INDEX;
		}
		return false;
	}

	/**
	 * [�ݒ�] ���於�ݒ�(BCC)
	 *	@note �Ώۂ������l����ꍇ�A�{���\�b�h��l����g�p����K�v������܂��B
	 *	@param ascBccAddr ����A�h���X(Ascii�����̂�)�B
	 *	@param ascBccName ���惆�[�U���B�ȗ��\�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddBccMailAddr(const CAscii& ascBccAddr, const CAscii& ascBccName = "")
	{
		return AddBccMailAddr(CMailAddress(ascBccAddr, ascBccName));
	}

	/**
	 * [�ݒ�] �����ݒ�
	 *	@param ascSubject �����B
	 */
	void SetSubject(const CAscii& ascSubject)
	{
		m_ascSubject = ascSubject;
	}
	
	/**
	 * [�ݒ�] ���M���e������
	 *	@note AddSendTextData() , AddSendBinaryData() , AddSendFileData() �̐ݒ���N���A���܂��B
	 */
	void InitSendData(void)
	{
		m_vascContents.RemoveAll();
		m_mapFileDatas.RemoveAll();
	}

	/**
	 * [�ݒ�] ���M�e�L�X�g�ǉ�
	 *	@note ���M���e�Ƀe�L�X�g�ǉ����܂��B������{���\�b�h���g�p����ƁA
	 *			�A���ł͂Ȃ��A�ʂ̃p�[�g�Ƀe�L�X�g���ǉ�����܂��̂ł����ӂ��������B
	 *	@attention �e�s�́A70�������x�ɂȂ�悤�ɂ��Ă��������B
	 *	@note		���s�R�[�h�́A ��r��n ���g�p���܂��B
	 *	@param ascText �e�L�X�g�B
	 */
	void AddSendTextData(const CAscii& ascText)
	{
		m_vascContents.Add(ascText);		///< ���e�e�L�X�g
	}

	/**
	 * [�ݒ�] ���M�f�[�^�ǉ�
	 *	@note ���M���e�Ƀo�C�i���f�[�^�ǉ�
	 *	@param ascFileName �t�@�C����
	 *	@param vb ���e
	 */
	void AddSendBinaryData(const CAscii& ascFileName, const CByteVector& vb)
	{
		if ( ! ascFileName.IsEmpty() )
		{
			m_mapFileDatas[ascFileName] = vb;
		}
	}
	
	/**
	 * [�ݒ�] ���M�f�[�^�ǉ�
	 *	@note �w��t�@�C����Ǎ���ł�����A���M���e�Ƀo�C�i���f�[�^�Ƃ��Ēǉ��B
	 *	@param lpszFile �t�@�C����
	 *	@retval true ����
	 *	@retval false ���s(�w��t�@�C�����Ȃ��A�Ȃ�)
	 *	@throw CIoFailureException �t�@�C���ǂݍ��ݎ��ɃG���[����������ƁA�X���[����܂��B
	 */
	bool AddSendFileData(LPCTSTR lpszFile)
	{
		CStr fname = lpszFile;
		CFileReader file;
		if ( file.Open(fname) )
		{
			CByteVector vb = file.ReadInto();
			if ( ! vb.IsEmpty() )
			{
//				INT_PTR l = fname.ReverseFind('\\');
//				if ( l < 0 )
//				{
//					l = fname.ReverseFind('/');
//				}
				INT_PTR l = fname.ReverseFindOneOf(_T("\\/"));
				if ( l >= 0 )
				{
					fname = fname.Mid(l + 1);
				}
				m_mapFileDatas[CAscii(fname)] = vb;
				return true;
			}
		}
		return false;
	}

	//--------------------------------------------
	
	/**
	 * [�Q��] ���[���[���Q��
	 *	@return �i�[����Ă��郁�[�����B
	 */
	const CAscii& ReferMailerName(void) const
	{
		return m_ascMailerName;
	}

	/**
	 * [�Q��] �����Q��
	 *	@return �i�[����Ă��� �����B
	 */
	const CAscii& ReferSubject(void) const
	{
		return m_ascSubject;
	}
	
	/**
	 * [�Q��] ���M���[���A�h���X�Q��
	 *	@note ������̂� SetUser() �̑������Őݒ肵�������񂩁ASetContentFromReceive() �̉�͌��ʂł��B
	 *	@return �i�[���M���[���A�h���X���
	 */
	const CMailAddress& ReferFromMailAddr(void) const
	{
		return m_addrFrom;
	}

	/**
	 * [�Q��] ���於�Q��
	 *	@return �i�[����Ă��� TO�̈��惁�[���A�h���X���Q
	 */
	const CVectorT<CMailAddress>& ReferToMailAddr(void) const
	{
		return m_aaddrTo[TO];
	}

	/**
	 * [�Q��] ���於�Q��(CC)
	 *	@return �i�[����Ă��� CC�̈��惁�[���A�h���X���Q
	 */
	const CVectorT<CMailAddress>& ReferCcMailAddr(void) const
	{
		return m_aaddrTo[CC];
	}

	/**
	 * [�Q��] ���於�Q��(BCC)
	 *	@return �i�[����Ă��� BCC�̈��惁�[���A�h���X���Q
	 */
	const CVectorT<CMailAddress>& ReferBccMailAddr(void) const
	{
		return m_aaddrTo[BCC];
	}

	/**
	 * [�Q��] ���v���C���Q��.
	 *	@return ���v���C�惁�[���A�h���X(Ascii�����̂�)�B
	 */
	CAscii ReferReplyMailAddr(void) const
	{
		return m_addrReply.GetAddress();
	}

	/**
	 * [�Q��] �e�L�X�g�R���e���c�Q��
	 *	@return �i�[����Ă��� �e�L�X�g�{���� Vector�B
	 */
	const CAsciiVector& ReferTextDatas(void) const
	{
		return m_vascContents;
	}

	/**
	 * [�Q��] �o�C�i���R���e���c�Q��
	 *	@return �i�[����Ă��� �o�C�i�����ƃo�C�i�����e�� Map�B
	 */
	const CMapT<CAscii, CByteVector>& ReferBinaryDatas(void) const
	{
		return m_mapFileDatas;
	}

	//--------------------------------------------
	
	/**
	 * [�m�F] ���M�\�m�F.
	 *		���M���邽�߂̏��͐ݒ肳��Ă��邩�A�m�F���܂��B
	 *	@retval true �\�B
	 *	@retval false ����Ȃ��B
	 */
	bool IsCompletesForSend(void) const
	{
		if ( m_aaddrTo[TO].IsEmpty() )
		{
			return false;
		}
		if ( ! m_addrFrom.IsValid() )
		{
			return false; 
		}
		return true;
	}

	/**
	 * [�擾] ���M�p�A�h���X�ꗗ
	 *	@note AddToAddr() , AddCcAddr() , AddBccAddr() �Őݒ肵���A�h���X�ꗗ
	 *	@return �ꗗ
	 */
	CAsciiVector GetAddrsForSend(void) const
	{
		CAsciiVector vaRc;
		loop ( i, KINDMAX )
		{
			loop ( j, m_aaddrTo[i].GetSize() )
			{
				vaRc.Add(m_aaddrTo[i][j].GetAddress());
			}
		}
		return vaRc;
	}

	/**
	 * [�擾] ���M�p���[�����e�擾
	 *	@note �w�b�_�p�[�g�A�{�̃p�[�g�ȂǁA�A�����č쐬���܂��B
	 *	@return ���e�B
	 */
	CAscii GetContentForSend(void) const
	{
		//== �w�b�_�쐬 (BCC�͊܂܂Ȃ�
		CAscii ascHead;
		ascHead += CAscii::Fmt("From: %s\r\n", m_FmtAddr(m_addrFrom));
		for ( int i = TO; i <= CC; i++ )
		{
			if ( ! m_aaddrTo[i].IsEmpty() )
			{
				ascHead += (i == 0) ? ("To: ") : ("Cc: ");
				loop ( j, m_aaddrTo[i].GetSize() )
				{
					if ( j > 0 )
					{
						ascHead += ",\r\n ";
					}
					ascHead += CAscii::Fmt("%s", m_FmtAddr(m_aaddrTo[i][j]));
				}
				ascHead += "\r\n";
			}
		}
		if ( ! m_addrReply.GetAddress().IsEmpty() )
		{
			ascHead += CAscii::Fmt("Reply-To: %s\r\n", m_addrReply.GetAddress());
		}
		ascHead += CAscii::Fmt("Subject: %s\r\n", m_ToBase64(m_ascSubject));
		ascHead += "MIME-Version: 1.0\r\n";
		ascHead += CAscii::Fmt("X-Mailer: %s\r\n", m_ascMailerName);
		if ( m_vascContents.GetSize() <= 1 && m_mapFileDatas.GetSize() == 0 )
		{
			//== �e�L�X�g������I
			ascHead += "Content-Type: text/plain; charset=\042ISO-2022-JP\042\r\n";
			ascHead += "Content-Transfer-Encoding: 7bit\r\n";
			ascHead += "\r\n";
			//�{���쐬
			CAscii ascMain; 
			if ( ! m_vascContents.IsEmpty() )
			{
				ascMain = m_vascContents[0];
				ascMain.Replace("\n.", "\n..");
			}
			ascMain += "\r\n.";
			//�w�b�_ + �{��(JIS�ɕϊ����ĘA��)
			CAscii ascRc = ascHead + CJisStr::ToJis(ascMain).ReferBuffer();
			return ascRc;		
		}
		//== �}���`�p�[�g��
		CAscii ascPart = m_MakeMultiPartString();
		ascHead += "Content-Type: multipart/mixed;\r\n";
		ascHead += CAscii::Fmt(" boundary=\042%s\042\r\n", ascPart);
		ascHead += "Content-Transfer-Encoding: 7bit\r\n";
		ascHead += "\r\n";
		//- Text��
		loop ( i, m_vascContents.GetSize() )
		{
			ascHead += CAscii::Fmt("--%s\r\n", ascPart);
			ascHead += "Content-Type: text/plain; charset=\042ISO-2022-JP\042\r\n";
			ascHead += "Content-Transfer-Encoding: 7bit\r\n";
			ascHead += "\r\n";
			CAscii a = m_vascContents[i];
			a.Replace("\n.", "\n..");
			a += "\r\n";
			ascHead += CJisStr::ToJis(a);
		}
		//- Binary��
		loop ( i, m_mapFileDatas.GetSize() )
		{
			const CMapT<CAscii,CByteVector>::CConstKeysAdapter& vk = m_mapFileDatas.GetKeysAdapter();
			ascHead += CAscii::Fmt("--%s\r\n", ascPart);
			ascHead += "Content-Type: application/octet-stream\r\n";
			ascHead += "Content-Disposition: attachment;\r\n";
			ascHead += CAscii::Fmt(" filename=\042%s\042\r\n", m_ToBase64(vk.At(i)));
			ascHead += "Content-Transfer-Encoding: base64\r\n";
			ascHead += "\r\n";
			const CByteVector& vb = m_mapFileDatas[vk.At(i)];
			size_t size = vb.GetSize();
			const BYTE* B = vb.ReferBuffer();
			const size_t MAX = 76 * 3 / 4;	//�ϊ��オ76�����ɂȂ�悤��
			while ( size > MAX )
			{
				ascHead += CBase64Str().Encode(MAX, B);
				ascHead += "\r\n";
				B += MAX;
				size -= MAX;
			}
			ascHead += CBase64Str().Encode(size, B);
			ascHead += "\r\n";
		}
		//- �I�[
		ascHead += CAscii::Fmt("--%s--\r\n.", ascPart);
		return ascHead;
	}

	//--------------------------------------------

	/**
	 * [���] ��M�f�[�^���.
	 *		��͂��A�{�C���X�^���X�ɐݒ肵�܂��B
	 *	@note Refer�`() �Ƃ������̃��\�b�h�ŁA��͂������ʂ𓾂邱�Ƃ��o���܂��B
	 *	@note �{�N���X�̒m��Ȃ��R�}���h�́A �{���\�b�h������A
	 *		ReferUnknownCommandFromReceive() �ɂĎQ�Ƃ��邱�Ƃ��o���܂��BDate�Ȃǂ́A����Ɋ܂܂�܂��B
	 *	@param lpszContent ��M���e�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetContentFromReceive(LPCSTR lpszContent)
	{
		Init();
		CAsciiVector vaContent = TNB::CAsciiOperator::SeparateLine(lpszContent);
		if ( ! m_SetContent(vaContent, m_mapUnknownCmd) )
		{
			Init();
			return false;
		}
		return true;
	}

	/**
	 * [���] ��M�f�[�^���.
	 *		��͂��A�{�C���X�^���X�ɐݒ肵�܂��B
	 *	@note Refer�`() �Ƃ������̃��\�b�h�ŁA��͂������ʂ𓾂邱�Ƃ��o���܂��B
	 *	@note �{�N���X�̒m��Ȃ��R�}���h�́A �{���\�b�h������A
	 *		ReferUnknownCommandFromReceive() �ɂĎQ�Ƃ��邱�Ƃ��o���܂��BDate�Ȃǂ́A����Ɋ܂܂�܂��B
	 *	@param vascContent ��M���e�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetContentFromReceive(const CAsciiVector& vascContent)
	{
		Init();
		CAsciiVector vaContent = vascContent;
		if ( ! m_SetContent(vaContent, m_mapUnknownCmd) )
		{
			Init();
			return false;
		}
		return true;
	}

	/**
	 * [�擾] ��M�f�[�^��͌���
	 *		SetContentFromReceive() �ŉ�͒��A�{�N���X�̒m��Ȃ��R�}���h&�p�����[�^�𒊏o���Ă���܂��B
	 *		������Q�Ƃ��邱�Ƃ��o���܂��B
	 *	@note SetContentFromReceive() �����s����ƁA���͎c��܂���B
	 *	@return �R�}���h�ƃp�����[�^�̃}�b�v
	 */
	const CAsciisMap& ReferUnknownCommandFromReceive(void) const
	{
		return m_mapUnknownCmd;
	}
};



}; // TNB



#if 0
From:						SetFromMailAddr
Reply-To:					SetReplyMailAddr
To:							AddToMailAddr
Cc:							AddCcMailAddr
Bcc:						AddBccMailAddr
Subject:					SetSubject
X-Mailer:					SetMailerName
Content-Type:
Content-Disposition:
Content-Transfer-Encoding:
#endif

#if 0
From: =?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss@tnb.co.jp>
To: =?ISO-2022-JP?B?GyRCJCokZCRWJHMbKEI=?= <oyabun@tnb.co.jp>
Cc: =?ISO-2022-JP?B?GyRCJV4lOSU/ITwbKEI=?= <master@tnb.co.jp>
Subject: =?ISO-2022-JP?B?GyRCTmMkTjdvGyhC?=
MIME-Version: 1.0
Content-Type: text/plain; charset="ISO-2022-JP"
Content-Transfer-Encoding: 7bit
#endif

#if 0
From: =?ISO-2022-JP?B?GyRCJFwkORsoQg==?= <boss@tnb.co.jp>
To: =?ISO-2022-JP?B?GyRCJCokZCRWJHMbKEI=?= <oyabun@tnb.co.jp>
Cc: =?ISO-2022-JP?B?GyRCJV4lOSU/ITwbKEI=?= <master@tnb.co.jp>
Subject: =?ISO-2022-JP?B?GyRCTmMkTjdvGyhC?=
MIME-Version: 1.0
X-Mailer: TNB Tiny Mailer
Content-Type: multipart/mixed;
 boundary="-----MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_"

-------MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_
Content-Type: text/plain; charset="ISO-2022-JP"
Content-Transfer-Encoding: 7bit

 $B$3$s$K$A$o (B
 $BNc$N7o!"$I$&$G$9$+!) (B

-------MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_
Content-Type: application/octet-stream
Content-Disposition: attachment;
 filename="=?ISO-2022-JP?B?Q0NEGyRCJSslYSVpMytILxsoQl9XaW5kbw==?=.dat"
Content-Transfer-Encoding: base64

MTIzNDU=

-------MultiPart_MjAwNi0xMC0yNyAxNDo0NjozMg==_--
.
#endif
