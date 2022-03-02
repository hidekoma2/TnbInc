#pragma once
/**
 *@file
 * TinyMacroLang 計算関係のヘッダ
 *	
 *		TMLの計算のクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
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
 * TinyMacroLang 関数拡張インターフェース
 *
 *		このIFを実装したクラスのインスタンスを CTmlCalculator / CTmlParser に与えることで、
 *		機能を拡張できます。
 *
 *	@par必要ファイル
 *			TnbTmlCalculator.h
 *
 *	@date 06/01/01 新規作成
 *	@date 07/07/20 改善
 */
struct ITmlExpander
{
	/// デストラクタ
	virtual ~ITmlExpander(void)
	{
	}

	/**
	 * [処理] 拡張変数、関数の処理
	 *	@param[out] _val 処理した時の値
	 *	@param[in] lpszToken 発見したトークン(トークンだけでNULL終端)
	 *	@param[in] aValues 発見したトークンの引数、および内容。
	 *	@param[in] boIsTestMode trueならテストモード（文法チェックのみ行う）。
	 *	@retval Result_Nop 処理無し。
	 *	@retval Result_Success 処理成功。
	 *	@retval 上記以外 エラー発生。
	 */
	virtual CTmlResult ExpandVariable(CTmlValueCarrier& _val, LPCSTR lpszToken, const CTmlValuesVector& aValues, bool boIsTestMode) = 0;

	/** 
	 * [処理] 変更(代入)コールバック
	 *	@param lpszName 代入された名前
	 *	@param val 代入された値
	 *	@return 結果。
	 */
	virtual CTmlResult OnChangeValue(LPCSTR lpszName, const CTmlValue& val) = 0;
};



/**@ingroup TML
 * TinyMacroLang 計算
 *
 *	文字列を解析し数値に変換することができます。
 *
 *		関数、変数は、 ITmlExpander を実装したクラスを SetExpander() で登録することで、拡張できます。
 *
 *	\code
 *		CTmlCalculator calc;
 *		CTmlValue val;
 *		CTmlResult r = calc.TextToValue(val, "(10 + 2) * 2");
 *		ASSERT( val.GetInteger() == (10 + 2) * 2 );
 *	\endcode
 *
 *	@par必要ファイル
 *			TnbTmlCalculator.h
 *
 *	@date 06/01/01 新規作成
 *	@date 07/07/20 改善
 */
class CTmlCalculator : public CTml, CCopyImpossible
{
public:

	/// コンストラクタ
	CTmlCalculator(void) : m_boIsTestMode(false), m_piExpander(NULL), m_dwTickCountBase(::GetTickCount())
	{
	}

	/**
	 * [設定] テストモード設定.
	 *	@param boIsTestMode true ならテストモードになります。 
	 */
	void SetTestMode(bool boIsTestMode)
	{
		m_boIsTestMode = boIsTestMode;
	}

	/**
	 * [取得] テストモード状態取得
	 *	@retval true テストモード中。
	 *	@retval false 通常。
	 */
	bool IsTestMode(void) const
	{
		return m_boIsTestMode;
	}

	/**
	 * [設定] 拡張処理指定
	 *	@param piExpander インターフェイスのポインタ。省略で解除。
	 */
	void SetExpander(ITmlExpander* piExpander = NULL)
	{
		m_piExpander = piExpander;
	}

	/**
	 * [参照] ワイルドカードリザルト.
	 *		ワイルドカードリザルトの参照を得られます。
	 *	@return リザルト配列の参照です。基本的に変更禁止です。
	 */
	CVectorT<CByteVector>& ReferElementWildResult(void)
	{
		return m_aabWildResult;
	}

	/**
	 * [処理] 計算.
	 *		変数、値などの計算の書かれた文字列を解析し、計算し、一つの値にします。
	 *		ex) ”(12+4)” を 16 に変換。
	 *	@note 計算式の後に、なぞの文字があると、「Result_UnfinishedExpression」を返します。
	 *	@param[out] _answer 結果
	 *	@param[in] lpsz 文字列。
	 *	@retval Result_Success 成功。
	 *	@retval それ以外 エラー。（Result_Nop は返りません）
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
	 * [処理] 計算.
	 *		変数、値などの計算の書かれた文字列を解析し、計算し、一つの値にします。
	 *		ex) ”(12+4)” を 16 に変換。
	 *	@param[out] _answer 結果
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@retval Result_Success 成功。
	 *	@retval それ以外 エラー。（Result_Nop は返りません）
	 */
	CTmlResult TextToValue(CTmlValue& _answer, CTmlGrammar& _gram)
	{
		return m_TextToValue(_answer, _gram);
	}

private:
	ITmlExpander*			m_piExpander;		///< 拡張関数インターフェイス
	bool					m_boIsTestMode;		///< テストモード
	CVectorT<CByteVector>	m_aabWildResult;	///< ワイルドカード
	DWORD					m_dwTickCountBase;	///< tickcount補正値

	/**
	 * 四則演算(＋α)タイプのチェック.
	 *		== != <= >= + - * / < > and or
	 *	@param[out] _iAriType 演算タイプ。
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@return 結果。
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
	 * 演算タイプのレベルを返す
	 *	@param iAriType 演算タイプ。
	 *	@return 演算タイプのレベル。大きいほど優先度高い。 1 は 代入関係。
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
	 * 指定の場所の文字列から値を探し出します。
	 *	@param[out] _n 発見した値。
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@retval Result_Success 成功。
	 *	@retval それ以外 エラー。（Result_Nop は返りません）
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
					// `)'がありません
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
	 * 文字列(変数)を一つの値に
	 *	@param[out] _n 戻り値用。データ型、数値型のいずれかが返ります。
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@retval Result_Success 成功。
	 *	@retval それ以外 エラー。（Result_Nop は返りません）
	 */
	CTmlResult m_TokenToOneValue(CTmlValueCarrier& _n, CTmlGrammar& _gram)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlResult res;
		//そこはデータ？
		res = m_SearchDataInToken(_n, _gram);
		if ( ! res.IsNop() )
		{
			return res;	//成功か失敗
		}
		//そこは数値？
		res = m_SearchIntegerInToken(_n, _gram);
		if ( ! res.IsNop() )
		{
			return res;	//成功か失敗
		}
		//それは変数？
		res = m_SearchVariableInToken(_n, _gram);
		if ( ! res.IsNop() )
		{
			return res;	//成功か失敗
		}
		//なにもなかった おかしい
		_n = 0;
		CTmlResult r(Operation_Calc, Result_UnknownKeyword);
		r.SetErrorString(_gram.GetNextParts().GetString());
		return r.SetLine(_gram);
	}

	/**
	 *	文字列表記をデータに
	 *	@param[out] _n 値。
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@return 結果。
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
	 *	数値文字列を数字に.
	 *		頭に 0x の場合16進数
	 *	@param[out] _n 数値。
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@retval Result_Success 成功、処理あり。
	 *	@retval Result_Nop 処理なし。
	 *	@todo 現在整数のみ対応。
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
					// `''がありません
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
			//-- 小数点をチェック
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
			//-- 指数部をチェック
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
					// `E(指数)の後にプラスマイナスがない
					return CTmlResult(Operation_Calc, Result_UnfinishedExpression).SetLine(gg);
				}
			}
		}
		if ( count == 0 )
		{
			/*数値はなかった*/
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
	 * 変数内容引きだし（関数実行).
	 *	@param[in] _n 変数内容値.
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@return 結果。
	 */
	CTmlResult m_SearchVariableInToken(CTmlValueCarrier& _n, CTmlGrammar& _gram)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar gg = _gram;
		_n = 0;
		//関数名取得
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
			//foo"～" という形だ
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
			//foo(～～)と言う形だ
			CTmlGrammar bg = _gram;
			if ( _gram.GetNextParts().GetAt() == ')' )
			{
				//すぐ閉じた
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
					//閉じてない！
					_n = 0;
					return CTmlResult(Operation_CheckFunc, Result_NotCloseParentheses).SetLine(backGram);
				}
			}
		}
		else
		{
			//括弧省略
			_gram = backGram;
		}
		//
		CTmlResult res = Result_Nop;
		if ( m_piExpander != NULL )
		{
			//拡張IFが指定されているなら実行
			res = m_piExpander->ExpandVariable(_n, token, aValues, IsTestMode());
		}
		if ( res.IsNop() )
		{
			//システム変数/関数
			res = m_SearchSystemVariable(_n, token, aValues);
		}
		if( res.IsError() )
		{
			res.SetErrorString(parts.GetString());
			res.SetLine(gg);
			return res;		//エラー
		}
		if ( res.IsSuccess() )
		{
			return Result_Success;	//成功
		}
		_gram = gg;
		return Result_Nop;
	}

	/**
	 * 指定の計算種で計算します。
	 *	@param[in] iAriType 演算タイプ
	 *	@param[in,out] a	左辺
	 *	@param[in,out] b	右辺
	 *	@retval Result_Success 成功。
	 *	@retval それ以外 エラー。（Result_Nop は返りません）
	 */
	CTmlResult m_CalcValueAndValue(int iAriType, CTmlValueCarrier& a, CTmlValueCarrier& b)
	{
		if ( IsTestMode() )
		{
			return Result_Success;
		}
		//
		#ifndef _TnbDOXYGEN	//Document作成用シンボル
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
					if ( strName.Compare("％BEFORE OPE") == 0 )
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
						TTRACE0("   →０で割ろうとしてます\n");
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
				TTRACE0("   →０で割ろうとしてます\n");
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

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	#define _CHECKPARAM_RETURN( PA, NU )														\
			TTRACE1A("found System Var [%s]\n", lpszToken);										\
			if ( PA.GetSize() != NU ) { return CTmlResult::CheckParamNum(PA.GetSize(), NU); }	\
			if ( IsTestMode() ) { return Result_Success;}

	#define IsTOKEN( XX ) ( STRLIB::Compare(lpszToken, XX)==0 )

	#endif //_TnbDOXYGEN

	/** 
	 * システム変数、関数の処理.
	 *	@param[out] _n 処理した時の値
	 *	@param[in] lpszToken 発見したトークン(トークンだけでNULL終端) 
	 *	@param[in] aValues 発見したトークンの引数
	 *	@return 結果。 処理がなければ、 Result_Nop が返ります。
	 *	@retval Result_Success 成功。
	 *	@retval Result_Nop 処理なし。
	 *	@retval それ以外 エラー。
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
			//ミリ秒カウンタ
			_CHECKPARAM_RETURN( aValues , 0 );
			//
			_n = ToInt(::GetTickCount() - m_dwTickCountBase);
			_n.Set("％tickcount");	//コールバック指示
			return Result_Success;
		}
		else if ( IsTOKEN("wildres") )
		{
			//ワイルドカードの結果
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
			//汎用グローバル関数「ダイアログ表示」
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			CStr str = aValues[0].GetString();
			AfxMessageBox(str, MB_ICONINFORMATION);
			_n = 0;
			return Result_Success;
		}
		else if ( IsTOKEN("yesnobox") )
		{
			//汎用グローバル関数「ダイアログ表示」
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			CStr str = aValues[0].GetString();
			int r = AfxMessageBox(CStr(aValues[0].GetString()), MB_YESNO | MB_ICONINFORMATION);
			_n = (r == IDYES);
			return Result_Success;
		}
		else if ( IsTOKEN("length") )
		{
			//汎用グローバル関数「データ長を得る」
			_CHECKPARAM_RETURN( aValues , 1 );
			//
			_n = aValues[0].GetInteger();
			return Result_Success;
		}
		else if ( IsTOKEN("compare") )
		{
			//汎用グローバル関数「比較」
			_CHECKPARAM_RETURN( aValues , 2 );
			//
			_n = ( aValues[0] == aValues[1] );
			return Result_Success;
		}
		else if ( IsTOKEN("mid") )
		{
			//汎用グローバル関数「データの一部を切りだし」ワイルドカードはなくなります。
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
			//汎用グローバル関数「データに変換(sprintf)」
			size_t si = aValues.GetSize();
			if ( si == 1 )
			{
				if ( IsTestMode() ){ return Result_Success; }
				CTmlValue v;
				v.SetString(CAscii::Fmt("%d", aValues[0].GetInteger()));
				_n = v;
				return Result_Success;
			}
			if ( si <= 6 )		//値5個まで対応
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
			//汎用グローバル関数「数値に変換(atoi)」
			_CHECKPARAM_RETURN( aValues , 1 );
			_n = STRLIB::ToInt(aValues[0].GetString());
			return Result_Success;
		}
		else if ( IsTOKEN("getat") )
		{
			//汎用グローバル関数「データの一箇所取り出し」
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
			//汎用グローバル関数「現在の時間(strftime)」
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
	 * 変更(代入)処理<BR>
	 *	本クラスで処理が無ければ、拡張IFをコールします。	
	 *	@param lpszName 代入された名前
	 *	@param value 代入された値
	 *	@retval Result_Success 成功。
	 *	@retval それ以外 エラー。（Result_Nop は返りません）
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
		if ( STRLIB::Compare(lpszName, "％tickcount") == 0 )
		{
			m_dwTickCountBase = ::GetTickCount() - value.GetInteger();
			return Result_Success;
		}
		return Result_Success;
	}

	/**
	 * [処理] 計算.
	 *		変数、値などの計算の書かれた文字列を解析し、計算し、一つの値にします。
	 *
	 *		ex) ”(12+4)” を 16 に変換。
	 *	@param [out] _answer 結果
	 *	@param[in,out] _gram 文字情報。成功時、ポインタは移動しています。
	 *	@retval Result_Success 成功。
	 *	@retval それ以外 エラー。（Result_Nop は返りません）
	 */
	CTmlResult m_TextToValue(CTmlValue& _answer, CTmlGrammar& _gram)
	{
		LPCSTR s = _gram.GetPointer();
		CTmlGrammar gg = _gram;
		CTmlValueCarrier leftSideValue;
		CTmlResult res;
		//頭記号チェック
		if ( s[0] == '-' || s[0] == '+' )
		{
			// いきなり - か + なら頭に０があるものとする
			leftSideValue = 0;
			if ( s[0] == s[1] )
			{
				// ++ ,--なら特殊状態にしておく
				leftSideValue.Set("％BEFORE OPE");
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
					// 代入関係は右優先
					if ( iLevel > l )
					{
						_gram = backGram;
						break;
					}
				}
				else
				{
					// 左優先
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

