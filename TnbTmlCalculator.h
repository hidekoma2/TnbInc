#pragma once
/**
 *@file
 * TinyMacroLang �v�Z�֌W�̃w�b�_
 *	
 *		TML�̌v�Z�̃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */




#include "TnbStrEx.h"
#include "TnbTmlResult.h"
#include "TnbTmlValue.h"
#include "TnbTmlTextEscape.h"
#include <math.h>



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang �֐��g���C���^�[�t�F�[�X
 *
 *		����IF�����������N���X�̃C���X�^���X�� CTmlCalculator / CTmlParser �ɗ^���邱�ƂŁA
 *		�@�\���g���ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlCalculator.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 07/07/20 ���P
 */
struct ITmlExpander
{
	/// �f�X�g���N�^
	virtual ~ITmlExpander(void)
	{
	}

	/**
	 * [����] �g���ϐ��A�֐��̏���
	 *	@param[out] _val �����������̒l
	 *	@param[in] lpszToken ���������g�[�N��(�g�[�N��������NULL�I�[)
	 *	@param[in] aValues ���������g�[�N���̈����A����ѓ��e�B
	 *	@param[in] boIsTestMode true�Ȃ�e�X�g���[�h�i���@�`�F�b�N�̂ݍs���j�B
	 *	@retval Result_Nop ���������B
	 *	@retval Result_Success ���������B
	 *	@retval ��L�ȊO �G���[�����B
	 */
	virtual CTmlResult ExpandVariable(CTmlValueCarrier& _val, LPCSTR lpszToken, const CTmlValuesVector& aValues, bool boIsTestMode) = 0;

	/** 
	 * [����] �ύX(���)�R�[���o�b�N
	 *	@param lpszName ������ꂽ���O
	 *	@param val ������ꂽ�l
	 *	@return ���ʁB
	 */
	virtual CTmlResult OnChangeValue(LPCSTR lpszName, const CTmlValue& val) = 0;
};



/**@ingroup TML
 * TinyMacroLang �v�Z
 *
 *	���������͂����l�ɕϊ����邱�Ƃ��ł��܂��B
 *
 *		�֐��A�ϐ��́A ITmlExpander �����������N���X�� SetExpander() �œo�^���邱�ƂŁA�g���ł��܂��B
 *
 *	\code
 *		CTmlCalculator calc;
 *		CTmlValue val;
 *		CTmlResult r = calc.TextToValue(val, "(10 + 2) * 2");
 *		ASSERT( val.GetInteger() == (10 + 2) * 2 );
 *	\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlCalculator.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 07/07/20 ���P
 */
class CTmlCalculator : public CTml, CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CTmlCalculator(void) : m_boIsTestMode(false), m_piExpander(NULL), m_dwTickCountBase(::GetTickCount())
	{
	}

	/**
	 * [�ݒ�] �e�X�g���[�h�ݒ�.
	 *	@param boIsTestMode true �Ȃ�e�X�g���[�h�ɂȂ�܂��B 
	 */
	void SetTestMode(bool boIsTestMode)
	{
		m_boIsTestMode = boIsTestMode;
	}

	/**
	 * [�擾] �e�X�g���[�h��Ԏ擾
	 *	@retval true �e�X�g���[�h���B
	 *	@retval false �ʏ�B
	 */
	bool IsTestMode(void) const
	{
		return m_boIsTestMode;
	}

	/**
	 * [�ݒ�] �g�������w��
	 *	@param piExpander �C���^�[�t�F�C�X�̃|�C���^�B�ȗ��ŉ����B
	 */
	void SetExpander(ITmlExpander* piExpander = NULL)
	{
		m_piExpander = piExpander;
	}

	/**
	 * [�Q��] ���C���h�J�[�h���U���g.
	 *		���C���h�J�[�h���U���g�̎Q�Ƃ𓾂��܂��B
	 *	@return ���U���g�z��̎Q�Ƃł��B��{�I�ɕύX�֎~�ł��B
	 */
	CVectorT<CByteVector>& ReferElementWildResult(void)
	{
		return m_aabWildResult;
	}

	/**
	 * [����] �v�Z.
	 *		�ϐ��A�l�Ȃǂ̌v�Z�̏����ꂽ���������͂��A�v�Z���A��̒l�ɂ��܂��B
	 *		ex) �h(12+4)�h �� 16 �ɕϊ��B
	 *	@note �v�Z���̌�ɁA�Ȃ��̕���������ƁA�uResult_UnfinishedExpression�v��Ԃ��܂��B
	 *	@param[out] _answer ����
	 *	@param[in] lpsz ������B
	 *	@retval Result_Success �����B
	 *	@retval ����ȊO �G���[�B�iResult_Nop �͕Ԃ�܂���j
	 */
	CTmlResult TextToValue(CTmlValue& _answer, LPCSTR lpsz)
	{
		CTmlGrammar gram(lpsz, NULL);
		CTmlResult r = TextToValue(_answer, gram);
		if ( r.IsSuccess() )
		{
			if ( gram.PeekNextParts().GetKind() != CTmlGrammar::FINAL )
			{
				r = Result_UnfinishedExpression;
			}
		}
		return r;
	}

	/**
	 * [����] �v�Z.
	 *		�ϐ��A�l�Ȃǂ̌v�Z�̏����ꂽ���������͂��A�v�Z���A��̒l�ɂ��܂��B
	 *		ex) �h(12+4)�h �� 16 �ɕϊ��B
	 *	@param[out] _answer ����
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@retval Result_Success �����B
	 *	@retval ����ȊO �G���[�B�iResult_Nop �͕Ԃ�܂���j
	 */
	CTmlResult TextToValue(CTmlValue& _answer, CTmlGrammar& _gram)
	{
		return m_TextToValue(_answer, _gram);
	}

private:
	ITmlExpander*			m_piExpander;		///< �g���֐��C���^�[�t�F�C�X
	bool					m_boIsTestMode;		///< �e�X�g���[�h
	CVectorT<CByteVector>	m_aabWildResult;	///< ���C���h�J�[�h
	DWORD					m_dwTickCountBase;	///< tickcount�␳�l

	/**
	 * �l�����Z(�{��)�^�C�v�̃`�F�b�N.
	 *		== != <= >= + - * / < > and or
	 *	@param[out] _iAriType ���Z�^�C�v�B
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@return ���ʁB
	 */
	CTmlResult m_CheckArithmeticType(int& _iAriType, CTmlGrammar& _gram)
	{
		LPCSTR P = _gram.GetPointer();
		char s0 = *P;
		char s1 = *(P + 1);
		int l = 0;
		_iAriType = 0;
		if ( s0 == '+' && s1 == '+' )
		{
			_iAriType = '++';
			l = 2;
		}
		else if ( s0 == '-' && s1 == '-' )
		{
			_iAriType = '--';
			l = 2;
		}
		else if ( s0 == '+' || s0 == '-' || s0 == '*' || s0 == '/' || s0 == '<' || s0 == '>' || s0 == '=' )
		{
			if ( s1 == '=' )
			{
				_iAriType = s0 * 0x100 + s1;
				l = 2;
			} 
			else
			{
				_iAriType = s0;
				l = 1;
			}
		} 
		else if ( s0 == '!' && s1 == '=' )
		{
			_iAriType = s0 * 0x100 + s1;
			l = 2;
		}
		else if ( s0 == 'a' && s1 == 'n' && *(P + 2) == 'd' )
		{
			_iAriType = 'and';
			l = 3;
		} 
		else if ( s0 == 'o' && s1 == 'r' )
		{
			_iAriType = 'or';
			l = 2;
		}
		if ( l > 0 )
		{
			CTmlGrammar gg = _gram;
			if ( _gram.StepPointer(l) != l )
			{
				return CTmlResult(Operation_Calc, Result_UnknownError).SetLine(gg);
			}
			return Result_Success;
		}
		return Result_Nop;
	}

	/**
	 * ���Z�^�C�v�̃��x����Ԃ�
	 *	@param iAriType ���Z�^�C�v�B
	 *	@return ���Z�^�C�v�̃��x���B�傫���قǗD��x�����B 1 �� ����֌W�B
	 */
	int m_GetArithmeticTypeLevel(int iAriType)
	{
		switch ( iAriType )
		{
		case '=':
		case '+=':
		case '-=':
		case '*=':
		case '/=':
			return 1;
		case 'or':
			return 2;
		case 'and':
			return 3;
		case '==':
		case '!=':
			return 4;
		case '<':
		case '<=':
			return 4;
		case '>':
		case '>=':
			return 4;
		case '+':
		case '-':
			return 7;
		case '*':
		case '/':
			return 8;
		case '++':
		case '--':
			return 9;
		default:
			ASSERTLIB(false);
			break;
		}
		return 0;
	}

	/**
	 * �w��̏ꏊ�̕����񂩂�l��T���o���܂��B
	 *	@param[out] _n ���������l�B
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@retval Result_Success �����B
	 *	@retval ����ȊO �G���[�B�iResult_Nop �͕Ԃ�܂���j
	 */
	CTmlResult m_FindValue(CTmlValueCarrier& _n, CTmlGrammar& _gram)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		//
		CTmlResult res;
		CTmlValueCarrier val;
		CAscii s = _gram.PeekNextParts().GetString();
		if ( s == "+" || s == "-" )
		{
			CTmlValue ii;
			res = m_TextToValue(ii, _gram);
			if ( res.IsError() )
			{
				return res;
			}
			val = ii;
		}
		else
		{
			if ( s == "(" )
			{
				_gram.GetNextParts();
				CTmlValue ii;
				res = m_TextToValue(ii, _gram);
				if ( res.IsError() )
				{
					return res;
				}
				val = ii;
				CTmlGrammar gg = _gram;
				if ( _gram.GetNextParts().GetAt() != ')' )
				{
					// `)'������܂���
					return CTmlResult(Operation_Calc, Result_NotCloseParentheses).SetLine(gg);
				}
			}
			else 
			{
				res = m_TokenToOneValue(val, _gram);
				if ( res.IsError() )
				{
					return res;
				}
			}
			int a = 0;
			CTmlGrammar gramBack = _gram;
			m_CheckArithmeticType(a, gramBack);
			if ( a == '++' || a == '--' )
			{
				CTmlValueCarrier dummy;
				res = m_CalcValueAndValue(a, val, dummy);
				if ( res.IsError() )
				{
					return res.SetLine(gramBack);
				}
				_gram = gramBack;
			}
		}
		_n = val;
		return Result_Success;
	}

	/**
	 * ������(�ϐ�)����̒l��
	 *	@param[out] _n �߂�l�p�B�f�[�^�^�A���l�^�̂����ꂩ���Ԃ�܂��B
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@retval Result_Success �����B
	 *	@retval ����ȊO �G���[�B�iResult_Nop �͕Ԃ�܂���j
	 */
	CTmlResult m_TokenToOneValue(CTmlValueCarrier& _n, CTmlGrammar& _gram)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlResult res;
		//�����̓f�[�^�H
		res = m_SearchDataInToken(_n, _gram);
		if ( ! res.IsNop() )
		{
			return res;	//���������s
		}
		//�����͐��l�H
		res = m_SearchIntegerInToken(_n, _gram);
		if ( ! res.IsNop() )
		{
			return res;	//���������s
		}
		//����͕ϐ��H
		res = m_SearchVariableInToken(_n, _gram);
		if ( ! res.IsNop() )
		{
			return res;	//���������s
		}
		//�Ȃɂ��Ȃ����� ��������
		_n = 0;
		CTmlResult r(Operation_Calc, Result_UnknownKeyword);
		r.SetErrorString(_gram.GetNextParts().GetString());
		return r.SetLine(_gram);
	}

	/**
	 *	������\�L���f�[�^��
	 *	@param[out] _n �l�B
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@return ���ʁB
	 */
	CTmlResult m_SearchDataInToken(CTmlValueCarrier& _n, CTmlGrammar& _gram)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar gg = _gram;
		const CTmlGrammar::CParts& parts = _gram.GetNextParts();
		if ( parts.GetKind() == CTmlGrammar::STRING )
		{
			CTmlTextEscape txt;
			CTmlResult r = txt.SetData(parts.GetString());
			if ( r.IsError() )
			{
				r.SetLine(gg);
				return r;
			}
			_n = txt.ToValue();
			return Result_Success;
		}
		_n = 0;
		_gram = gg;
		return Result_Nop;
	}

	/**
	 *	���l������𐔎���.
	 *		���� 0x �̏ꍇ16�i��
	 *	@param[out] _n ���l�B
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@retval Result_Success �����A��������B
	 *	@retval Result_Nop �����Ȃ��B
	 *	@todo ���ݐ����̂ݑΉ��B
	 */
	CTmlResult m_SearchIntegerInToken(CTmlValueCarrier& _n, CTmlGrammar& _gram)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar gg = _gram;
		//
		int	flag = 1;
		int count = 0;
		bool isFloat = false;
		int	value = 0;
		double floatValue = 0.0;
		CTmlGrammar::CParts parts = _gram.GetNextParts();
		for ( ; parts.IsEqualString("-") || parts.IsEqualString("+"); parts = _gram.GetNextParts() )
		{
			if ( parts.IsEqualString("-") )
			{
				flag = -1;
			}
		}
		LPCSTR P = parts.GetString();
		if ( P[0]=='0' && (P[1] == 'x' || P[1] == 'X') )
		{
			P += 2;
			while ( true )
			{
				int c = (*P) | 0x20;
				if ( (c < '0' || c > '9') && (c < 'a' || c > 'f') )
				{
					break;
				}
				if ( c >= 'a' && c <= 'f' )
				{
					value = value * 16 + (c - 'a') + 10;
				}
				else
				{
					value = value * 16 + c - '0';
				}
				P++;
				count++;
			}
		}
		else if ( P[0]=='\'' )
		{
			P++;
			count++;
			while ( true )
			{
				if ( *P == 0 )
				{
					// `''������܂���
					return CTmlResult(Operation_Calc, Result_NotCloseParentheses).SetLine(gg);
				}
				if ( *P == '\'' )
				{
					break;
				}
				value = (value * 0x100) + *P;
				P++; 
				count++;
			}
		}
		else 
		{
			while ( true )
			{
				if ( *P < '0' || *P > '9' )
				{
					break;
				}
				value = (value * 10) + *P - '0';
				P++; 
				count++;
			}
			//-- �����_���`�F�b�N
			{
				CTmlGrammar gg = _gram;
				if ( _gram.GetNextParts().GetAt() == '.' )
				{
					CTmlGrammar gg = _gram;
					parts = _gram.GetNextParts();
					P = parts.GetString();
					if ( *P >= '0' && *P <= '9' )
					{
						int i = 0;
						double po = 1;
						while ( true )
						{
							if ( *P < '0' || *P > '9' )
							{
								break;
							}
							i = (i * 10) + *P - '0';
							P++; 
							po *= 10.0;
						}
						if ( ! isFloat )
						{
							isFloat = true;
							floatValue = value;
						}
						double ii = i;
						floatValue += ii / po;
					}
					else
					{
						_gram = gg;
					}
				}
				else
				{
					_gram = gg;
				}
			}
			//-- �w�������`�F�b�N
			if ( *P == 'e' || *P == 'E' )
			{
				parts = _gram.GetNextParts();
				if ( parts.IsEqualString("+") || parts.IsEqualString("-") )
				{
					bool isPlus = parts.GetAt() == '+';
					parts = _gram.GetNextParts();
					P = parts.GetString();
					if ( *P >= '0' && *P <= '9' )
					{
						int i = 0;
						while ( true )
						{
							if ( *P < '0' || *P > '9' )
							{
								break;
							}
							i = (i * 10) + *P - '0';
							P++; 
						}
						if ( ! isFloat )
						{
							isFloat = true;
							floatValue = value;
						}
						double ii = i;
						floatValue *= pow(10.0, isPlus ? ii : -ii);
					}
				}
				else
				{
					// `E(�w��)�̌�Ƀv���X�}�C�i�X���Ȃ�
					return CTmlResult(Operation_Calc, Result_UnfinishedExpression).SetLine(gg);
				}
			}
		}
		if ( count == 0 )
		{
			/*���l�͂Ȃ�����*/
			_gram = gg;
			_n = 0;
			return Result_Nop;
		}
		if ( isFloat )
		{
			_n = floatValue * flag;
		}
		else
		{
			_n = value * flag;
		}
		return Result_Success;
	}

	/**
	 * �ϐ����e���������i�֐����s).
	 *	@param[in] _n �ϐ����e�l.
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@return ���ʁB
	 */
	CTmlResult m_SearchVariableInToken(CTmlValueCarrier& _n, CTmlGrammar& _gram)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar gg = _gram;
		_n = 0;
		//�֐����擾
		CTmlGrammar::CParts parts = _gram.GetNextParts();
		CAscii originalToken = parts.GetString();
		CAscii token = LocalMapString(originalToken, LCMAP_LOWERCASE);
		if ( parts.GetKind() != CTmlGrammar::TOKEN || token.IsEmpty() )
		{
			_gram = gg;
			return Result_Nop;
		}
		//
		CTmlValuesVector aValues;
		CTmlGrammar backGram = _gram;
		parts = _gram.GetNextParts();
		if ( parts.GetKind() == CTmlGrammar::STRING )
		{
			//foo"�`" �Ƃ����`��
			CTmlTextEscape txt;
			CTmlResult r = txt.SetData(parts.GetString());
			if ( r.IsError() )
			{
				r.SetLine(backGram);
				return r;
			}
			CTmlValueCarrier a;
			a = txt.ToValue();
			aValues.Add(a());
		}
		else if ( parts.GetAt() == '(' )
		{
			//foo(�`�`)�ƌ����`��
			CTmlGrammar bg = _gram;
			if ( _gram.GetNextParts().GetAt() == ')' )
			{
				//��������
			}
			else
			{
				_gram = bg;
				while ( true )
				{
					CTmlValue a;
					CTmlResult res = m_TextToValue(a, _gram);
					if ( res.IsError() )
					{
						return res;
					}
					aValues.Add(a);
					if ( _gram.PeekNextParts().GetAt() != ',' )
					{
						break;
					}
					_gram.GetNextParts();
				}
				if ( _gram.GetNextParts().GetAt() != ')' )
				{
					//���ĂȂ��I
					_n = 0;
					return CTmlResult(Operation_CheckFunc, Result_NotCloseParentheses).SetLine(backGram);
				}
			}
		}
		else
		{
			//���ʏȗ�
			_gram = backGram;
		}
		//
		CTmlResult res = Result_Nop;
		if ( m_piExpander != NULL )
		{
			//�g��IF���w�肳��Ă���Ȃ���s
			res = m_piExpander->ExpandVariable(_n, token, aValues, IsTestMode());
		}
		if ( res.IsNop() )
		{
			//�V�X�e���ϐ�/�֐�
			res = m_SearchSystemVariable(_n, token, aValues);
		}
		if( res.IsError() )
		{
			res.SetErrorString(parts.GetString());
			res.SetLine(gg);
			return res;		//�G���[
		}
		if ( res.IsSuccess() )
		{
			return Result_Success;	//����
		}
		_gram = gg;
		return Result_Nop;
	}

	/**
	 * �w��̌v�Z��Ōv�Z���܂��B
	 *	@param[in] iAriType ���Z�^�C�v
	 *	@param[in,out] a	����
	 *	@param[in,out] b	�E��
	 *	@retval Result_Success �����B
	 *	@retval ����ȊO �G���[�B�iResult_Nop �͕Ԃ�܂���j
	 */
	CTmlResult m_CalcValueAndValue(int iAriType, CTmlValueCarrier& a, CTmlValueCarrier& b)
	{
		if ( IsTestMode() )
		{
			return Result_Success;
		}
		//
		#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
		#if defined(_TnbDEBUG_ON) && defined(_DEBUG)
			#define XXX( STR ){	::OutputDebugStringA(						\
					CAscii::Fmt("calc ([%s]%s %s %s)\n",					\
					a.GetExName(), a().GetString(), STR, b().GetString() ) ); }	
		#else
			#define XXX( STR )
		#endif
		#endif
		CTmlResult res;
		CTmlValue tvResult;
		switch ( iAriType )
		{
		case '=':
		case '+=':
		case '-=':
		case '*=':
		case '/=':
		case '--':
		case '++':
			{
				const CAscii& strName = a.GetExName();
				switch ( iAriType )
				{
				case '++':
				case '--':
					if ( strName.Compare("��BEFORE OPE") == 0 )
					{
						if ( ! b().HasData() )
						{
							b() = b() + (iAriType == '++' ? +1 : -1);
						}
						tvResult = b();
						a = b();
					}
					else
					{
						tvResult = a();
						if ( ! a().HasData() )
						{
							a() = a() + (iAriType == '++' ? +1 : -1);
						}
					}
					break;
				case '=':
					XXX( "=" );
					a() = b();
					tvResult = a();
					break;
				case '+=':
					XXX( "+=" );
					a() = a() + b();
					tvResult = a();
					break;
				case '-=':
					XXX( "-=" );
					a() = a() - b();
					tvResult = a();
					break;
				case '*=':
					XXX( "*=" );
					a() = a() * b();
					tvResult = a();
					break;
				default:// '/='
					XXX( "/=" );
					if ( b() == 0 )
					{
						TTRACE0("   ���O�Ŋ��낤�Ƃ��Ă܂�\n");
						return CTmlResult(Operation_Calc, Result_ZeroDivide);
					}
					a() = a() / b();
					tvResult = a();
					break;
				}
				//
				if ( ! strName.IsEmpty() )
				{
					res = m_OnChangeValue(strName, a());
				}
			}
			break;
		case 'or':
			XXX( "or" );
			tvResult = ( a().GetInteger() | b().GetInteger() );
			break;
		case 'and':
			XXX( "and" );
			tvResult = ( a().GetInteger() & b().GetInteger() );
			break;
		case '==':
			XXX( "==" );
			tvResult = ( a().Compare(m_aabWildResult, b()) == 0 );
			break;
		case '!=':
			XXX( "!=" );
			tvResult = ( a().Compare(m_aabWildResult, b()) != 0 );
			break;
		case '<':
			XXX( "<" );
			tvResult = ( a() < b() );
			break;
		case '<=':
			XXX( "<=" );
			tvResult = ( a() <= b() );
			break;
		case '>':
			XXX( ">" );
			tvResult = ( a() > b() );
			break;
		case '>=':
			XXX( ">=" );
			tvResult = ( a() >= b() );
			break;
		case '+':
			XXX( "+" );
			tvResult = ( a() + b() );
			break;
		case '-':
			XXX( "-" );
			tvResult = ( a() - b() );
			break;
		case '*':
			XXX( "*" );
			tvResult = ( a() * b() );
			break;
		case '/':
			XXX( "/" );
			if ( b()==0 )
			{
				TTRACE0("   ���O�Ŋ��낤�Ƃ��Ă܂�\n");
				return CTmlResult(Operation_Calc, Result_ZeroDivide);
			}
			else
			{
				tvResult = ( a() / b() );
			}
			break;
		default:
			ASSERT(false);
			break;
		}
		a = tvResult;
		#undef XXX
		return res;
	}

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	#define _CHECKPARAM_RETURN( PA, NU )														\
			TTRACE1A("found System Var [%s]\n", lpszToken);										\
			if ( PA.GetSize() != NU ) { return CTmlResult::CheckParamNum(PA.GetSize(), NU); }	\
			if ( IsTestMode() ) { return Result_Success;}

	#define IsTOKEN( XX ) ( STRLIB::Compare(lpszToken, XX)==0 )

	#endif //_TnbDOXYGEN

	/** 
	 * �V�X�e���ϐ��A�֐��̏���.
	 *	@param[out] _n �����������̒l
	 *	@param[in] lpszToken ���������g�[�N��(�g�[�N��������NULL�I�[) 
	 *	@param[in] aValues ���������g�[�N���̈���
	 *	@return ���ʁB �������Ȃ���΁A Result_Nop ���Ԃ�܂��B
	 *	@retval Result_Success �����B
	 *	@retval Result_Nop �����Ȃ��B
	 *	@retval ����ȊO �G���[�B
	 */
	CTmlResult m_SearchSystemVariable(CTmlValueCarrier& _n, LPCSTR lpszToken, const CTmlValuesVector& aValues)
	{
		if ( IsTOKEN("h") )
		{
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			CTmlTextEscape txt;
			CTmlResult r = txt.SetHexToData(aValues[0].GetString());
			if ( r.IsError() )
			{
				return r;
			}
			_n = txt.ToValue();
			return Result_Success;
		}
		else if ( IsTOKEN("tickcount") )
		{
			//�~���b�J�E���^
			_CHECKPARAM_RETURN( aValues , 0 );
			//
			_n = ToInt(::GetTickCount() - m_dwTickCountBase);
			_n.Set("��tickcount");	//�R�[���o�b�N�w��
			return Result_Success;
		}
		else if ( IsTOKEN("wildres") )
		{
			//���C���h�J�[�h�̌���
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			int iIndex = aValues[0].GetInteger();
			CTmlValue v;
			if ( iIndex < ToInt(m_aabWildResult.GetSize()) )
			{
				v.SetData(m_aabWildResult[iIndex].GetSize(), m_aabWildResult[iIndex].ReferBuffer());
			}
			_n = v;
			return Result_Success;
		}
		else if ( IsTOKEN("pausebox") )
		{
			//�ėp�O���[�o���֐��u�_�C�A���O�\���v
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			CStr str = aValues[0].GetString();
			AfxMessageBox(str, MB_ICONINFORMATION);
			_n = 0;
			return Result_Success;
		}
		else if ( IsTOKEN("yesnobox") )
		{
			//�ėp�O���[�o���֐��u�_�C�A���O�\���v
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			CStr str = aValues[0].GetString();
			int r = AfxMessageBox(CStr(aValues[0].GetString()), MB_YESNO | MB_ICONINFORMATION);
			_n = (r == IDYES);
			return Result_Success;
		}
		else if ( IsTOKEN("length") )
		{
			//�ėp�O���[�o���֐��u�f�[�^���𓾂�v
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			_n = aValues[0].GetInteger();
			return Result_Success;
		}
		else if ( IsTOKEN("compare") )
		{
			//�ėp�O���[�o���֐��u��r�v
			_CHECKPARAM_RETURN( aValues , 2 );
			//
			_n = ( aValues[0] == aValues[1] );
			return Result_Success;
		}
		else if ( IsTOKEN("mid") )
		{
			//�ėp�O���[�o���֐��u�f�[�^�̈ꕔ��؂肾���v���C���h�J�[�h�͂Ȃ��Ȃ�܂��B
			if ( aValues.GetSize() == 2 )
			{
				if ( IsTestMode() ){ return Result_Success; }
				CByteVector vb = aValues[0].GetData();
				CTmlValue v;
				v = vb.Mid(aValues[1].GetInteger());
				_n = v;
				return Result_Success;
			}
			else if ( aValues.GetSize() == 3 )
			{
				if ( IsTestMode() ){ return Result_Success; }
				CByteVector vb = aValues[0].GetData();
				CTmlValue v;
				v = vb.Mid(aValues[1].GetInteger(), aValues[2].GetInteger());
				_n = v;
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 2);
		}
		else if ( IsTOKEN("todata") )
		{
			//�ėp�O���[�o���֐��u�f�[�^�ɕϊ�(sprintf)�v
			size_t si = aValues.GetSize();
			if ( si == 1 )
			{
				if ( IsTestMode() ){ return Result_Success; }
				CTmlValue v;
				v.SetString(CAscii::Fmt("%d", aValues[0].GetInteger()));
				_n = v;
				return Result_Success;
			}
			if ( si <= 6 )		//�l5�܂őΉ�
			{
				if ( IsTestMode() ){ return Result_Success; }
				//
				CAscii strRes;
				CAscii strForm = aValues[0].GetString();
				switch ( si )
				{
				case 2:
					strRes.Format(strForm, aValues[1].GetInteger());
					break;
				case 3:
					strRes.Format(strForm, aValues[1].GetInteger(), aValues[2].GetInteger());
					break;
				case 4:
					strRes.Format(strForm, aValues[1].GetInteger(), aValues[2].GetInteger(), aValues[3].GetInteger());
					break;
				case 5:
					strRes.Format(strForm, aValues[1].GetInteger(), aValues[2].GetInteger(), aValues[3].GetInteger(), aValues[4].GetInteger());
					break;
				case 6:
					strRes.Format(strForm, aValues[1].GetInteger(), aValues[2].GetInteger(), aValues[3].GetInteger(), aValues[4].GetInteger(), aValues[5].GetInteger());
					break;
				}
				CTmlValue v;
				v.SetString(strRes);
				_n = v;
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 7);
		}
		else if ( IsTOKEN("tovalue") )
		{
			//�ėp�O���[�o���֐��u���l�ɕϊ�(atoi)�v
			_CHECKPARAM_RETURN( aValues , 1 );
			_n = STRLIB::ToInt(aValues[0].GetString());
			return Result_Success;
		}
		else if ( IsTOKEN("getat") )
		{
			//�ėp�O���[�o���֐��u�f�[�^�̈�ӏ����o���v
			_CHECKPARAM_RETURN( aValues , 2 );
			CByteVector vb = aValues[0].GetData();
			INDEX ind = aValues[1].GetInteger();
			if ( vb.IsInRange(ind) )
			{
				_n = static_cast<int>(vb[ind]);
			}
			else
			{
				_n = 0;
			}
			return Result_Success;
		}
#if 0
		else if ( IsTOKEN("getCurrentTimeString") )
		{
			//�ėp�O���[�o���֐��u���݂̎���(strftime)�v
			TTRACE1("found System Func [%s]\n",lpszToken);
			if ( aValues.GetSize()==0 )
			{
				if ( IsTestMode() ){return Result_Success;}
				//
				CTime t = CTime::GetCurrentTime();
				CTmlValue v;
				v = t.Format( "%y%m%d%H%M%S" );
				tvxObject = v;
				return Result_Success;
			}
			else if ( aValues.GetSize()==1 )
			{
				if ( IsTestMode() ){return Result_Success;}
				//
				CTime t = CTime::GetCurrentTime();
				CTmlValue v;
				v = t.Format( aValues[0].GetString() );
				tvxObject = v;
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
#endif
		//
		_n = 0;
		return Result_Nop;
	}

	#undef IsTOKEN
	#undef _CHECKPARAM_RETURN

	/** 
	 * �ύX(���)����<BR>
	 *	�{�N���X�ŏ�����������΁A�g��IF���R�[�����܂��B	
	 *	@param lpszName ������ꂽ���O
	 *	@param value ������ꂽ�l
	 *	@retval Result_Success �����B
	 *	@retval ����ȊO �G���[�B�iResult_Nop �͕Ԃ�܂���j
	 */
	CTmlResult m_OnChangeValue(LPCSTR lpszName, const CTmlValue& value)
	{
		if ( m_piExpander != NULL )
		{
			CTmlResult r = m_piExpander->OnChangeValue(lpszName, value);
			if ( ! r.IsNop() )
			{
				return r;
			}
		}
		//
		if ( STRLIB::Compare(lpszName, "��tickcount") == 0 )
		{
			m_dwTickCountBase = ::GetTickCount() - value.GetInteger();
			return Result_Success;
		}
		return Result_Success;
	}

	/**
	 * [����] �v�Z.
	 *		�ϐ��A�l�Ȃǂ̌v�Z�̏����ꂽ���������͂��A�v�Z���A��̒l�ɂ��܂��B
	 *
	 *		ex) �h(12+4)�h �� 16 �ɕϊ��B
	 *	@param [out] _answer ����
	 *	@param[in,out] _gram �������B�������A�|�C���^�͈ړ����Ă��܂��B
	 *	@retval Result_Success �����B
	 *	@retval ����ȊO �G���[�B�iResult_Nop �͕Ԃ�܂���j
	 */
	CTmlResult m_TextToValue(CTmlValue& _answer, CTmlGrammar& _gram)
	{
		LPCSTR s = _gram.GetPointer();
		CTmlGrammar gg = _gram;
		CTmlValueCarrier leftSideValue;
		CTmlResult res;
		//���L���`�F�b�N
		if ( s[0] == '-' || s[0] == '+' )
		{
			// �����Ȃ� - �� + �Ȃ瓪�ɂO��������̂Ƃ���
			leftSideValue = 0;
			if ( s[0] == s[1] )
			{
				// ++ ,--�Ȃ�����Ԃɂ��Ă���
				leftSideValue.Set("��BEFORE OPE");
			}
		}
		else
		{
			res = m_FindValue(leftSideValue, _gram);
			if ( res.IsError() )
			{
				return res;
			}
		}
		//
		while ( true )
		{
			int iAriType = 0;
			res = m_CheckArithmeticType(iAriType, _gram);
			if ( ! res.IsSuccess() )
			{
				ASSERT( iAriType == 0 );
				LPCSTR P = _gram.GetPointer();
				if ( *P != 0 && *P != ')' )
				{
					CTmlGrammar backGram = _gram;
					CTmlValueCarrier dummy;
					res = m_FindValue(dummy, _gram); 
					if ( res.GetResult() == Result_Success )
					{
						return CTmlResult(Operation_Calc, Result_UnfinishedExpression).SetLine(backGram);
					}
					_gram = backGram;
				}
				break;
			}
			int iLevel = m_GetArithmeticTypeLevel(iAriType);
			CTmlValueCarrier rightSideValue;
			res = m_FindValue(rightSideValue, _gram); 
			if ( res.IsError() )
			{
				return res;
			}
			//
			while ( true )
			{
				int a = 0;
				CTmlGrammar backGram = _gram;
				res = m_CheckArithmeticType(a, _gram);
				if ( ! res.IsSuccess() )
				{
					ASSERT( a == 0 );
					break;
				}
				int l = m_GetArithmeticTypeLevel(a);
				if ( l == 1 )
				{
					// ����֌W�͉E�D��
					if ( iLevel > l )
					{
						_gram = backGram;
						break;
					}
				}
				else
				{
					// ���D��
					if ( iLevel >= l )
					{
						_gram = backGram;
						break;
					}
				}
				//
				CTmlValueCarrier temp;
				res = m_FindValue(temp, _gram);
				if ( res.IsError() )
				{
					return res;
				}
				res = m_CalcValueAndValue(a, rightSideValue, temp);
				if ( res.IsError() )
				{
					return res.SetLine(_gram);
				}
			}
			res = m_CalcValueAndValue(iAriType, leftSideValue, rightSideValue);
			if ( res.IsError() )
			{
				return res.SetLine(_gram);
			}
		}
		_answer = leftSideValue();
		return Result_Success;
	}

	friend class CTmlCalculatorTest;
};



}; // TNB

