#pragma once
/**
 *@file
 * ���@��͊֌W�̃w�b�_
 *
 *		���@����͂��邽�߂̃N���X������܂��B	
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup GRAMMAR
 * ���@��̓e���v���[�g
 *
 *		��̃|�C���^���Ǘ����A������̉�͂��T�|�[�g���܂��B
 *		���̍ہA���s���l�����邽�߁A�������̍s�ԍ���m�邱�Ƃ��o���܂��B
 *
 *	@param TYP �ʏ�A char(ASCII/SJIS) �� WCHAR(UNICODE) ���w�肵�܂��B
 *	@param ITE TYP�^�������C�e���[�^�^���w�肵�܂��B�ȗ������ const TYP* �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbGrammarAnalyzer.h
 * 
 *	@date 07/07/07	�V�K�쐬
 *	@date 07/07/13	���͂̓C�e���[�^�ɂ����B
 */
template<typename TYP = TCHAR, typename ITE = const TYP*>
class CGrammarAnalyzerT
{
public:

	/**@ingroup GRAMMAR
	 * ���@��͗p�t�H�[�}�b�g�C���^�[�t�F�[�X
	 *
	 *		���̃C���^�[�t�F�[�X�����������N���X���A CGrammarAnalyzerT �ɓn�����ƂŁA
	 *		������x���R�ȃt�H�[�}�b�g�ɑΉ��\�ł��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbGrammarAnalyzer.h
	 * 
	 *	@date 07/07/07	�V�K�쐬
	 */
	struct IFormat
	{
		/// �f�X�g���N�^
		virtual ~IFormat(void){}

		/**
		 * [�m�F] �u�����N��������.
		 *		�u�����N�������`���郁�\�b�h�ł��B
		 *	@note �f�t�H���g�ł́A �X�y�[�X�ƃ^�u���u�����N�Ƃ��Ă��܂��B
		 *	@param c	�`�F�b�N�Ώە���
		 *	@retval true �u�����N������
		 *	@retval false �Ⴄ
		 */
		virtual bool IsBlankChar(TYP c) const
		{
			return (c == ' ' || c == '\t');
		}

		/**
		 * [�m�F] �g�[�N����؂蕶������.
		 *		�g�[�N����؂蕶�����`���郁�\�b�h�ł��B
		 *	@note �f�t�H���g�ł́A��0 = , . [] <> {} () + - * / ; ! �X�y�[�X�A�^�u�A���s����؂蕶���Ƃ��Ă��܂��B
		 *	@param c	�`�F�b�N�Ώە���
		 *	@retval true �g�[�N����؂蕶����
		 *	@retval false �Ⴄ
		 */
		virtual bool IsTokenPeriodChar(TYP c) const
		{
			switch ( c )
			{
			case 0:				case '=':			case ' ':			case '\t':	
			case ',':			case 0x0D:			case 0x0A:			case '/':	
			case '*':			case '(':			case ')':			case ']':	
			case '[':			case ':':			case '{':			case '}':	
			case '!':			case '+':			case '-':			case '<':	
			case '>':			case ';':			case '\"':			case '.':
			case 0x1A:
				return true;
			}
			return false;
		}
		
		/**
		 * [�m�F] �I�[�����`�F�b�N
		 *		�I�[�������`���郁�\�b�h�ł��B
		 *	@note �f�t�H���g�ł́A��0 ���I�[�����Ƃ��Ă��܂��B
		 *	@param c	�`�F�b�N�Ώە���
		 *	@retval true �e�L�X�g�I�[������
		 *	@retval false �Ⴄ
		 */
		virtual bool IsEndChar(TYP c) const
		{
			return c == 0;
		}

		/**
		 * [�m�F] �n�[�����`�F�b�N.
		 *		�u���b�N�̎n�[�������`���郁�\�b�h�ł��B�n�[������������A�I�[������Ԃ��܂��B
		 *	@note �f�t�H���g�ł́A '(' �` ')' , '[' �` ']' , '{' �` '}' ���`���Ă��܂��B
		 *	@param c	�`�F�b�N�Ώە���
		 *	@retval 0 �ΏۊO
		 *	@retval 0�ȊO �I�[����
		 */
		virtual TYP CheckBlockStartChar(TYP c) const
		{
			TYP r = 0;
			switch ( c )
			{
			case '(':	r = ')';	break;
			case '[':	r = ']';	break;
			case '{':	r = '}';	break;
			}
			return r;
		}

		/**
		 * [�m�F] �I�[�����`�F�b�N.
		 *		�u���b�N�̏I�[�������`���郁�\�b�h�ł��B�n�[�����`�F�b�N���\�b�h�Ɠ����������`���Ă��������B
		 *	@note �f�t�H���g�ł́A '(' �` ')' , '[' �` ']' , '{' �` '}' ���`���Ă��܂��B
		 *	@param c	�`�F�b�N�Ώە���
		 *	@retval 0 �ΏۊO
		 *	@retval 0�ȊO �I�[����
		 */
		virtual bool IsBlockCloseChar(TYP c) const
		{
			switch ( c )
			{
			case ')':	break;
			case ']':	break;
			case '}':	break;
			default:
				return false;
			}
			return true;
		}

		/**
		 * [�m�F] ������n�I�[�����`�F�b�N.
		 *		������̎n�I�[�������`���郁�\�b�h�ł��B�n�[��������ƁA�r���A�ʂ̎n�I�[�����Ƃ���
		 *		��`���Ă��镶���������Ă��A�n�[�Ɠ���������������܂ŁA������Ƃ��܂��B
		 *	@note �f�t�H���g�ł́A �V���O���R�[�e�[�V�����A�_�u���R�[�e�[�V�������`���Ă��܂��B
		 *	@param c	�`�F�b�N�Ώە���
		 *	@retval true ������n�I�[������
		 *	@retval false �Ⴄ
		 */
		virtual bool IsStringMarkChar(TYP c) const
		{
			return (c == '\"' || c == '\"');
		}

		/**
		 * [�擾] ����������ꕶ���`�F�b�N.
		 *		������Ɋ܂܂��A����L�@���`���郁�\�b�h�ł��B�G�X�P�[�v�V�[�P���X�Ȃǂ��`�ł��܂��B
		 *	@note �f�t�H���g�ł́A'��n','��r','����','��"'�ɑΉ����Ă��܂��B
		 *	@param[out] _str �ϊ�����������B
		 *	@param[in] ite �`�F�b�N�Ώۃf�[�^
		 *	@retval 0���� �G���[�B
		 *	@retval 0 ���ꕶ���Ȃ��B
		 *	@retval 1�ȏ� ������Ƃ��Ĉ���������
		 */
		virtual int CheckStringSpecialWord(CStrT<TYP>& _str, ITE ite) const
		{
			if ( *ite != '\\' )
			{
				return 0;
			}
			_str.Empty();
			ite++;
			switch ( *ite )
			{
			case '\\':
			case '\042':
				_str = "\\";
				_str += *ite;
				break;
			case 'n':
				_str = "\n";
				break;
			case 'r':
				_str = "\r";
				break;
			default:
				return 0;
			}
			return 2;
		}

		/**
		 * [�擾] �R�����g�`�F�b�N.
		 *		�R�����g�Ƃ��Ė�������J�n������A����сA�I�[���`���܂��B
		 *	@note �f�t�H���g�ł́A'/��' �` '��/' �ԁA'//' �` ���s�Ԃ́A�R�����g�Ɣ��f���܂��B
		 *	@param[out] _str �I�[������
		 *	@param[in] ite �`�F�b�N�Ώۃf�[�^
		 *	@retval 0 �R�����g�Ȃ�
		 *	@retval 1�ȏ� _str �̕����܂ŃX�L�b�v
		 *	@retval -1 ���s�܂ŃX�L�b�v
		 */
		virtual int CheckComment(CStrT<TYP>& _str, ITE ite) const
		{
			if ( *ite != '/' )
			{
				return 0;
			}
			ite++;
			if ( *ite == '/' )
			{
				return -1;
			}
			if ( *ite == '*' )
			{
				_str = "*/";
				return 2;
			}
			return 0;
		}
	};

	/// �p�[�c��ރR�[�h
	enum EPartsKind
	{
		TOKEN,				///< �g�[�N��
		CHARACTOR,			///< ����
		STRING,				///< ������
		PERIOD_CHAR,		///< �g�[�N����؂蕶��
		OPEN_CHAR,			///< �u���b�N�J�n����
		CLOSE_CHAR,			///< �u���b�N�I�[����
		FINAL,				///< �I���
		ERROR_STRING,		///< ������G���[
		ERROR_DEPTH,		///< �[���G���[
	};

	/**@ingroup GRAMMAR
	 * ���@��͗p�p�[�c�Ǘ��N���X
	 *
	 *		CGrammarAnalyzerT::GetNextParts() �� CGrammarAnalyzerT::PeekNextParts() �œ����܂��B
	 *		��ނƓ��e�A����сA���ꂪ�������s�Ɨ���Ǘ����܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbGrammarAnalyzer.h
	 * 
	 *	@date 07/07/07	�V�K�쐬
	 */
	class CParts
	{
	public:

		/// �R���X�g���N�^
		CParts(void) : m_kind(FINAL), m_str(), m_line(0), m_raw(0)
		{
		}

		/**
		 * �R���X�g���N�^
		 *	@param kind ���
		 *	@param s ���e
		 *	@param l �s
		 *	@param r ��
		 */
		CParts(EPartsKind kind, const CStrT<TYP>& s, INT_PTR l, INT_PTR r) 
			: m_kind(kind), m_str(s), m_line(l), m_raw(r)
		{
		}

		/**
		 * �R���X�g���N�^
		 *	@param kind ���
		 *	@param c ���e
		 *	@param l �s
		 *	@param r ��
		 */
		CParts(EPartsKind kind, TYP c, INT_PTR l, INT_PTR r)
			: m_kind(kind), m_str(), m_line(l), m_raw(r)
		{
			TYP at[2] = {c, 0};
			m_str = at;
		}

		/**
		 * �R�s�[�R���X�g���N�^
		 *	@param other �R�s�[��
		 */
		CParts(const CParts& other) 
			: m_kind(other.m_kind), m_str(other.m_str), m_line(other.m_line), m_raw(other.m_raw)
		{
		}

		/**
		 * �R�s�[�R���X�g���N�^
		 *	@param other �R�s�[��
		 *	@return �������g
		 */
		CParts& operator=(const CParts& other)
		{
			m_kind = other.m_kind;
			m_str = other.m_str;
			m_line = other.m_line;
			m_raw = other.m_raw;
			return *this;
		}
 
		/**
		 * [�擾] ���
		 *	@return ���
		 */
		EPartsKind GetKind(void) const
		{
			return m_kind;
		}

		/**
		 * [��r] ���e��r
		 *	@param lpsz ��r����
		 *	@retval true ��v
		 *	@retval false �s��v
		 */
		bool IsEqualString(LPCSTR lpsz) const
		{
			return m_str.IsEqual(lpsz);
		}

		/**
		 * [�擾] ���e
		 *	@return ���e
		 */
		const TYP* GetString(void) const
		{
			return m_str;
		}

		/**
		 * [�擾] ���e.
		 *	@param index �C���f�b�N�X
		 *	@return ���e
		 */
		TYP GetAt(INDEX index = 0) const
		{
			return (m_str.GetLength() > index) ? m_str[index] : static_cast<TYP>(0);
		}

		/**
		 * [�擾] �s�ԍ��擾
		 *	@return �s�ԍ�
		 */
		INT_PTR GetLine(void) const
		{
			return m_line;
		}

		/**
		 * [�擾] ��ԍ��擾
		 *	@return ��ԍ�
		 */
		INT_PTR GetRaw(void) const
		{
			return m_raw;
		}

		/**
		 * [�擾] ������.
		 *	@note �J���p�ł��B
		 *	@return ������
		 */
		CStr ToString(void) const
		{
			CStr str;
			switch ( m_kind )
			{
			case TOKEN:			str = _T("�g�[�N��");			break;
			case CHARACTOR:		str = _T("����");				break;
			case STRING:		str = _T("������");				break;
			case PERIOD_CHAR:	str = _T("�g�[�N����؂蕶��");	break;
			case OPEN_CHAR:		str = _T("�u���b�N�J�n����");	break;
			case CLOSE_CHAR:	str = _T("�u���b�N�I�[����");	break;
			case FINAL:			str = _T("�I���");				break;
			default:			str = _T("�G���[");				break;
			}
			return CStr::Fmt(_T("%s : [%s] (%d,%d)"), str, CStr(m_str), m_line, m_raw);
		}

	private:
		EPartsKind	m_kind;		///< ���
		CStrT<TYP>	m_str;		///< ���e
		INT_PTR		m_line;		///< �����s
		INT_PTR		m_raw;		///< ������
	};

	/**
	 * �R���X�g���N�^
	 *	@param is �J�n�C�e���[�^�B�ȗ��\�B
	 *	@param ie �ԕ��C�e���[�^�B�ȗ��\�B
	 *	@param line �s�ԍ��B�ȗ��\�B
	 */
	CGrammarAnalyzerT(ITE is = ITE(), ITE ie = ITE(), INT_PTR line = 1) 
		: m_param(is, ie, line), m_pFormat(&m_defaultFormat)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CGrammarAnalyzerT(const CGrammarAnalyzerT& other) 
		: m_param(other.m_param), m_pFormat(other.m_pFormat), m_strName(other.m_strName)
	{
		if ( other.m_pFormat == &other.m_defaultFormat )
		{
			m_pFormat = &m_defaultFormat;
		}
	}

	/**
	 * �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CGrammarAnalyzerT& operator=(const CGrammarAnalyzerT& other)
	{
		m_param = other.m_param;
		m_pFormat = other.m_pFormat;
		m_strName = other.m_strName;
		if ( other.m_pFormat == &other.m_defaultFormat )
		{
			m_pFormat = &m_defaultFormat;
		}
		return *this;
	}

	/**
	 * [�ݒ�] �t�H�[�}�b�^�ݒ�.
	 *	@param P �t�H�[�}�b�^�BNULL���w�肷��ƁA�f�t�H���g�ɖ߂�܂��B
	 */
	void SetFormat(const IFormat* P)
	{
		m_pFormat = (P == NULL) ? &m_defaultFormat : P;
	}

	/**
	 * [�ݒ�] ���O�ݒ�
	 *	@note �C�ӂ̖��O����L�������邱�Ƃ��o���܂��B
	 *	@param name ���O�B
	 */
	void SetName(LPCTSTR name)
	{
		m_strName = name;
	}

	/**
	 * [�擾] ���O�擾
	 *	@note SetGrammarName �Őݒ肵�����O���擾�ł��܂��B
	 *	@return ���O�B
	 */
	CStr GetName(void) const
	{
		return m_strName;
	}

	/**
	 * [�擾] �s�ԍ��擾
	 *	@return �s�ԍ�
	 */
	UINT_PTR GetLine(void) const
	{
		return m_param.line;
	}

	/**
	 * [�擾] ��ԍ��擾
	 *	@return ��ԍ�
	 */
	UINT_PTR GetRaw(void) const
	{
		return m_param.raw;
	}

	/**
	 * [�擾] �|�W�V�����擾
	 *	@return �|�W�V�����ԍ�
	 */
	UINT_PTR GetPos(void) const
	{
		return m_param.pos;
	}

	/**
	 * [�擾] ���݂̃C�e���[�^
	 *	@note ���݂̃C�e���[�^�𓾂邱�Ƃ��o���܂��B
	 *	@return �C�e���[�^
	 */
	ITE GetPointer(void)
	{
		m_SkipBlankChar();
		return ite();
	}

	/**
	 * [�擾] �[���擾
	 *	@return �[��
	 */
	INT_PTR GetDepth(void) const
	{
		return m_param.strDepth.GetLength();
	}

	/**
	 * [�擾] �u���b�N����
	 *	@retval 0 �u���b�N�Ȃ�
	 *	@retval 0�ȊO �u���b�N�J�n����
	 */
	TYP GetBlockChar(void) const
	{
		int l = m_param.strDepth.GetLength();
		return (l > 0) ? m_param.strDepth.GetAt(l - 1) : 0;
	}

	/**
	 * [�擾] ���̃p�[�c�擾
	 *	@note ������|�C���^�́A�p�[�c�̌�������Ă��܂��B
	 *			�������AFINAL�̏ꍇ������|�C���^�͓����܂���B
	 *	@param boIsToken ture ���ȗ��Ȃ� TOEKN �Ŏ��o���܂��Bfalse �Ȃ� CHARACTOR �ňꕶ���������o���܂��B
	 *	@return �p�[�c�B
	 */
	CParts GetNextParts(bool boIsToken = true)
	{
		CParts p;
		m_SkipBlankChar();
		if ( ! m_IsEnd() )
		{
			TYP c = *ite();
			UINT_PTR line = GetLine();
			UINT_PTR raw = GetRaw();
			if ( m_pFormat->CheckBlockStartChar(c) != 0 )
			{
				// �u���b�N�J�n����
				p = CParts(OPEN_CHAR, c, line, raw);
				inc();
				m_param.strDepth += c;
			}
			else if ( m_pFormat->IsBlockCloseChar(c) )
			{
				//�u���b�N�I�[����
				if ( m_param.strDepth.IsEmpty() )
				{
					p = CParts(ERROR_DEPTH, c, line, raw);
				}
				else
				{
					p = CParts(CLOSE_CHAR, c, line, raw);
					inc();
					m_param.strDepth.DeleteLast();
				}
			}
			else if ( m_pFormat->IsStringMarkChar(c) )
			{
				// ������
				CStrT<TYP> str;
				if ( m_GetString(str) < 0 )
				{
					//error
					p = CParts(ERROR_STRING, str, line, raw);
				}
				else 
				{
					p = CParts(STRING, str, line, raw);
				}
			}
			else if ( m_pFormat->IsTokenPeriodChar(c) )
			{
				// �g�[�N����؂蕶��
				p = CParts(PERIOD_CHAR, c, line, raw);
				inc();
			}
			else if ( boIsToken )
			{
				// �g�[�N��
				CStrT<TYP> str;
				m_GetToken(str);
				p = CParts(TOKEN, str, line, raw);
			}
			else
			{
				CStrT<TYP> str;
				str += c;
				if ( STRLIB::GetCharSize(c) == 2 )
				{
					inc();
					str += *ite();
				}
				inc();
				p = CParts(CHARACTOR, str, line, raw);
			}
		}
		return p;
	}

	/** 
	 * [����] �u���b�N����X�e�b�v�A�E�g.
	 *	@retval false �G���[�B������|�C���^�͓����܂���B
	 *	@retval true �����B������|�C���^�̓u���b�N�I�[�����̎��Ɉړ����܂��B
	 */
	bool SkipoutBlock(void)
	{
		if ( m_param.strDepth.IsEmpty() )
		{
			return false;
		}
		INT_PTR l = m_param.strDepth.GetLength();
		TYP lc = m_pFormat->CheckBlockStartChar(m_param.strDepth.GetAt(l - 1));
		if ( m_SkipToNextChar(lc) < 0 )
		{
			return false;
		}
		inc();	//�I�[�̎���
		m_param.strDepth.DeleteLast();
		return true;
	}

	/**
	 * [�m�F] ���̃p�[�c�m�F
	 *	@note ������|�C���^�͓����܂���B
	 *	@param boIsToken ture ���ȗ��Ȃ� TOEKN �Ŏ��o���܂��Bfalse �Ȃ� CHARACTOR �ňꕶ���������o���܂��B
	 *	@return �p�[�c�B
	 */
	CParts PeekNextParts(bool boIsToken = true)
	{
		TParam pm = m_param;
		CParts r = GetNextParts(boIsToken);
		m_param = pm;
		return r;
	}

	/** 
	 * [����] �X�e�b�v.
	 *		�w�萔�A�|�C���^��i�߂܂��B�������A�w�萔�i�߂��Ƃ��낪�g�[�N���̓r���Ȃ�A
	 *		�g�[�N���̏I���܂Ői�߂Ă��܂��܂��B
	 *	@param step �X�e�b�v���������B
	 *	@return �X�e�b�v�������B
	 */
	INT_PTR StepPointer(INT_PTR step)
	{
		INT_PTR top = m_param.pos;		// ���݂̃|�W�V����
		while ( (m_param.pos - top) < step )
		{
			GetNextParts();
		}
		return m_param.pos - top;
	}

private:

	/// �p�����[�^
	struct TParam
	{
		ITE			iteNow;		///< ���݂̃C�e���[�^
		ITE			iteEnd;		///< �ԕ��C�e���[�^
		INT_PTR		pos;		///< ���݂̃|�W�V����
		INT_PTR		line;		///< ���݂̍s�ԍ�
		INT_PTR		raw;		///< ���݂̗�ԍ�
		CStrT<TYP>	strDepth;	///< �l�X�g�[��
		// �R���X�g���N�^
		TParam(ITE is = ITE(), ITE ie = ITE(), INT_PTR l = 1) : iteNow(is), iteEnd(ie), pos(0), line(l), raw(0), strDepth()
		{
		}
		// �R�s�[�R���X�g���N�^
		TParam(const TParam& other)
			: iteNow(other.iteNow), iteEnd(other.iteEnd), pos(other.pos)
			, line(other.line), raw(other.raw), strDepth(other.strDepth)
		{
		}
		// �R�s�[�I�y���[�^
		TParam& operator=(const TParam& other)
		{
			iteNow = other.iteNow;
			iteEnd = other.iteEnd;
			pos = other.pos;
			line = other.line;
			raw = other.raw;
			strDepth = other.strDepth;
			return *this;
		}
	};

	TParam			m_param;				///< �p�����[�^
	IFormat			m_defaultFormat;		///< �f�t�H���g�̃t�H�[�}�b�g
	const IFormat*	m_pFormat;				///< �g�p����t�H�[�}�b�^
	CStr			m_strName;				///< �C�ӂ̖��O

	/// �V���[�g�J�b�g
	ITE ite(void){ return m_param.iteNow; }
	void inc(void) { m_param.iteNow++; m_param.pos++; m_param.raw++;} 
	void inc(INT_PTR i) { m_param.iteNow += i; m_param.pos += i; m_param.raw += i; } 

	/// �X�e�b�v�J�E���g�p
	class CStepCnt
	{
		UINT_PTR m_bak;
	public:
		CStepCnt(const TParam& t) : m_bak(t.pos) {}
		UINT_PTR rst(const TParam& t){ return t.pos - m_bak; }
	};

	/**
	 * [�m�F] �I�[�`�F�b�N.
	 *	@note ���݂̃C�e���[�^���I�[���`�F�b�N�B
	 *	@retval true �I�[�������B
	 *	@retval false �Ⴄ�B
	 */
	bool m_IsEnd(void)
	{
		if ( ite() != m_param.iteEnd && ! m_pFormat->IsEndChar(*ite()) )
		{
			return false;
		}
		return true;
	}

	/**
	 * [����] ���s�X�L�b�v.
	 *	@note ���݂̃C�e���[�^�����s�������Ă�����A���̌�ֈړ��B
	 *	@return ������������
	 */
	INT_PTR m_SkipCr(void)
	{
		CStepCnt cnt(m_param);
		ASSERTLIB( ! m_IsEnd() );
		TYP c = *ite();
		if ( c == 0x0D )
		{
			m_param.line++;
			inc();
			if ( *ite() == 0x0A )
			{
				inc();
			}
			m_param.raw = 0;
		}
		else if ( c == 0x0A )
		{
			m_param.line++;
			inc();
			m_param.raw = 0;
		}
		return cnt.rst(m_param);
	}

	/**
	 * [��r] ��r
	 *	@note ���݂̃C�e���[�^�ƕ�������r�B�����񒷕��̂݁A�`�F�b�N���܂��B
	 *	@param str ��r������B
	 *	@retval true ��v�B
	 *	@retval false �s��v�B
	 */
	bool m_IsEqual(const CStrT<TYP>& str)
	{
		ITE p = ite();
		loop ( i, str.GetLength() )
		{
			if ( str[i] != *p )
			{
				return false;
			}
			p++;
		}
		return true;
	}

	/**
	 * [����] �u�����N�����X�L�b�v.
	 *	@note m_SkipBlankChar() �̃T�u�ł��B
	 *	@param r -1 ���s�܂ŃX�L�b�v�B�^ 1�ȏ� str �̕����܂ŃX�L�b�v
	 *	@param str �X�L�b�v��̕�����
	 */
	void m_SkipBlankCharSub(INT_PTR r, const CStrT<TYP>& str)
	{
		ASSERTLIB( r != 0 );
		if ( r > 0 )
		{
			inc(r);
		}
		while ( true )
		{
			if ( r > 0 && m_IsEqual(str) )
			{
				// str �Ɠ���
				inc(str.GetLength());
				break;
			}
			if ( m_SkipCr() > 0 )
			{
				if ( r < 0 )
				{
					break;
				}
			}
			else if ( m_IsEnd() )
			{
				break;
			}
			else
			{
				TYP c = *ite();
				inc(STRLIB::GetCharSize(c));
			}
		}
	}

	/**
	 * [����] �u�����N�����X�L�b�v.
	 *	@note �u�����N�����A����сA�R�����g�A����сA���s���X�L�b�v���܂��B
	 *	@return ������������
	 */
	INT_PTR m_SkipBlankChar(void)
	{
		CStepCnt cnt(m_param);
		CStrT<TYP> str;
		while ( true )
		{
			if ( m_IsEnd() )
			{
				break;
			}
			INT_PTR r = m_pFormat->CheckComment(str, ite());
			if ( r != 0 )
			{
				m_SkipBlankCharSub(r, str);
			}
			else if ( m_SkipCr() > 0 )
			{
				;
			}
			else if ( m_pFormat->IsBlankChar(*ite()) )
			{
				inc();
			}
			else
			{
				break;
			}
		}
		return cnt.rst(m_param);
	}

	/**
	 * [�擾] �g�[�N���擾.
	 *	@note �g�[�N����؂蕶�����ŏ��ɂ���ƁA���̕��������Ԃ��܂��B
	 *	@param[out] _token �擾�g�[�N��
	 *	@return ������������
	 */
	INT_PTR m_GetToken(CStrT<TYP>& _token)
	{
		CStepCnt cnt(m_param);
		ASSERTLIB( ! m_IsEnd() );
		_token.Empty();
		while ( true )
		{
			TYP c = *ite();
			if ( STRLIB::GetCharSize(c) == 2 )
			{
				_token += c;
				inc();
				_token += *ite();
			}
			else
			{
				if ( m_pFormat->IsTokenPeriodChar(c) )
				{
					if ( cnt.rst(m_param) == 0 )
					{
						_token += c;
						inc();
					}
					break;
				}
				_token += c;
			}
			inc();
		}
//		TRACE1("GrammarAnalyzer FoundToken = [%s]\n", CStr(_token));
		ASSERT( cnt.rst(m_param) != 0 );
		return cnt.rst(m_param);
	}

	/**
	 * [����] �g�[�N���X�L�b�v.
	 *	@note �g�[�N����؂蕶�����ŏ��ɂ���ƁA���̕��������X�L�b�v���܂��B
	 *	@return ������������
	 */
	INT_PTR m_SkipToken(void)
	{
		CStrT<TYP> s;
		return m_GetToken(s);
	}

	/**
	 * [����] �u���b�N�X�L�b�v.
	 *		���A������̎n�I�[�������A�u���b�N�̎n�[�����Ȃ�A���̕�����A�u���b�N���X�L�b�v���܂�
	 *		�i�r���A�u���b�N������΃X�L�b�v���܂��j�B
	 *	@return ������������
	 */
	INT_PTR m_SkipBlock(void)
	{
		CStepCnt cnt(m_param);
		if ( m_pFormat->IsStringMarkChar(*ite()) )
		{
			m_SkipString();
		}
		else
		{
			TYP c = m_pFormat->CheckBlockStartChar(*ite());
			if ( c != 0 )
			{
				inc();
				m_SkipToNextChar(c);
				if ( *ite() == c )
				{
					inc();
				}
			}
		}
		return cnt.rst(m_param);
	}

	/**
	 * [����] �w�蕶���܂ŃX�L�b�v.
	 *	@param cCloseChar �u�������v
	 *	@retval -1 �G���[�B������Ȃ��B������|�C���^�͓����܂���B
	 *	@retval 0�ȏ� ������������
	 */
	INT_PTR m_SkipToNextChar(TYP cCloseChar)
	{
		CStepCnt cnt(m_param);
		TParam pp = m_param;
		while ( true )
		{
			m_SkipBlankChar();
			if ( m_IsEnd() )
			{
				TRACE0("`('���Ԃ��Ă܂���\n");
				m_param = pp;
				return -1;
			}
			TYP c = *ite();
			if ( STRLIB::GetCharSize(c) == 2 )
			{
				inc(2);
			}
			else if ( c == cCloseChar )
			{
				break;
			} 
			else if ( m_SkipBlock() > 0 )
			{
				;
			}
			else
			{
				m_SkipToken();
			}
		}
		return cnt.rst(m_param);
	}

	/**
	 * [�擾] ������擾.
	 *		������n�I�[�����������Ă��鎞�A�����񂪎擾�ł��܂��B
	 *	@param[out] _str �擾������
	 *	@retval 0����	��������̃t�H�[�}�b�g�G���[�B�i�|�C���^�͓����܂��j 
	 *	@retval 0		������Ȃ�
	 *	@retval 0�ȏ�	���������i�擾������̒����{�Q�ɂȂ��Ă���)
	 */
	INT_PTR m_GetString(CStrT<TYP>& _str)
	{
		CStepCnt cnt(m_param);
		ASSERTLIB( ! m_IsEnd() );
		bool r = true;
		_str.Empty();
		TYP c = *ite();
		if ( ! m_pFormat->IsStringMarkChar(c) )
		{
			return 0;
		}
		inc();
		TYP endChar = c;
		while ( true )
		{
			if ( m_IsEnd() )
			{
				break;
			}
			c = *ite();
			inc();
			if ( c == endChar )
			{
				break;
			}
			_str += c;
			if ( STRLIB::GetCharSize(c) == 2 )
			{
				_str += *ite();
				inc();
			}
			CStrT<TYP> s;
			INT_PTR l = m_pFormat->CheckStringSpecialWord(s, ite());
			if ( l < 0 )
			{
				r = false;
				inc();
			}
			else if ( l > 0 )
			{
				_str += s;
				inc(l);
			}
		}
		return r ? cnt.rst(m_param) : -1;
	}

	/**
	 * [����] ������X�L�b�v.
	 *		������n�I�[�����������Ă��鎞�A��������X�L�b�v���܂��B
	 *	@retval 0����	��������̃t�H�[�}�b�g�G���[�B�i�|�C���^�͓����܂��j 
	 *	@retval 0		������Ȃ�
	 *	@retval 0�ȏ�	���������i�擾������̒����{�Q�ɂȂ��Ă���)
	 */
	INT_PTR m_SkipString(void)
	{
		CStrT<TYP> s;
		return m_GetString(s);
	}

	friend class CGrammarAnalyzerTest;
};



/**@ingroup GRAMMAR
 * ���@��̓N���X(ASCII/SJIS)
 *
 *	@par�K�v�t�@�C��
 *			TnbGrammarAnalyzer.h
 * 
 *	@date 07/07/07	�V�K�쐬
 */
typedef CGrammarAnalyzerT<> CGrammarAnalyzer;



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")

