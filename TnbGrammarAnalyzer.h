#pragma once
/**
 *@file
 * 文法解析関係のヘッダ
 *
 *		文法を解析するためのクラスがあります。	
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup GRAMMAR
 * 文法解析テンプレート
 *
 *		一つのポインタを管理し、文字列の解析をサポートします。
 *		その際、改行も考慮するため、処理中の行番号を知ることが出来ます。
 *
 *	@param TYP 通常、 char(ASCII/SJIS) か WCHAR(UNICODE) を指定します。
 *	@param ITE TYP型を扱うイテレータ型を指定します。省略すると const TYP* になります。
 *
 *	@par必要ファイル
 *			TnbGrammarAnalyzer.h
 * 
 *	@date 07/07/07	新規作成
 *	@date 07/07/13	入力はイテレータにした。
 */
template<typename TYP = TCHAR, typename ITE = const TYP*>
class CGrammarAnalyzerT
{
public:

	/**@ingroup GRAMMAR
	 * 文法解析用フォーマットインターフェース
	 *
	 *		このインターフェースを実装したクラスを、 CGrammarAnalyzerT に渡すことで、
	 *		ある程度自由なフォーマットに対応可能です。
	 *
	 *	@par必要ファイル
	 *			TnbGrammarAnalyzer.h
	 * 
	 *	@date 07/07/07	新規作成
	 */
	struct IFormat
	{
		/// デストラクタ
		virtual ~IFormat(void){}

		/**
		 * [確認] ブランク文字判定.
		 *		ブランク文字を定義するメソッドです。
		 *	@note デフォルトでは、 スペースとタブをブランクとしています。
		 *	@param c	チェック対象文字
		 *	@retval true ブランク文字だ
		 *	@retval false 違う
		 */
		virtual bool IsBlankChar(TYP c) const
		{
			return (c == ' ' || c == '\t');
		}

		/**
		 * [確認] トークン区切り文字判定.
		 *		トークン区切り文字を定義するメソッドです。
		 *	@note デフォルトでは、￥0 = , . [] <> {} () + - * / ; ! スペース、タブ、改行を区切り文字としています。
		 *	@param c	チェック対象文字
		 *	@retval true トークン区切り文字だ
		 *	@retval false 違う
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
		 * [確認] 終端文字チェック
		 *		終端文字を定義するメソッドです。
		 *	@note デフォルトでは、￥0 を終端文字としています。
		 *	@param c	チェック対象文字
		 *	@retval true テキスト終端文字だ
		 *	@retval false 違う
		 */
		virtual bool IsEndChar(TYP c) const
		{
			return c == 0;
		}

		/**
		 * [確認] 始端文字チェック.
		 *		ブロックの始端文字を定義するメソッドです。始端文字だったら、終端文字を返します。
		 *	@note デフォルトでは、 '(' ～ ')' , '[' ～ ']' , '{' ～ '}' を定義しています。
		 *	@param c	チェック対象文字
		 *	@retval 0 対象外
		 *	@retval 0以外 終端文字
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
		 * [確認] 終端文字チェック.
		 *		ブロックの終端文字を定義するメソッドです。始端文字チェックメソッドと同じ文字を定義してください。
		 *	@note デフォルトでは、 '(' ～ ')' , '[' ～ ']' , '{' ～ '}' を定義しています。
		 *	@param c	チェック対象文字
		 *	@retval 0 対象外
		 *	@retval 0以外 終端文字
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
		 * [確認] 文字列始終端文字チェック.
		 *		文字列の始終端文字を定義するメソッドです。始端を見つけると、途中、別の始終端文字として
		 *		定義している文字があっても、始端と同じ文字が見つかるまで、文字列とします。
		 *	@note デフォルトでは、 シングルコーテーション、ダブルコーテーションを定義しています。
		 *	@param c	チェック対象文字
		 *	@retval true 文字列始終端文字だ
		 *	@retval false 違う
		 */
		virtual bool IsStringMarkChar(TYP c) const
		{
			return (c == '\"' || c == '\"');
		}

		/**
		 * [取得] 文字列内特殊文字チェック.
		 *		文字列に含まれる、特殊記法を定義するメソッドです。エスケープシーケンスなどを定義できます。
		 *	@note デフォルトでは、'￥n','￥r','￥￥','￥"'に対応しています。
		 *	@param[out] _str 変換した文字列。
		 *	@param[in] ite チェック対象データ
		 *	@retval 0未満 エラー。
		 *	@retval 0 特殊文字なし。
		 *	@retval 1以上 文字列として扱う文字数
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
		 * [取得] コメントチェック.
		 *		コメントとして無視する開始文字列、および、終端を定義します。
		 *	@note デフォルトでは、'/＊' ～ '＊/' 間、'//' ～ 改行間は、コメントと判断します。
		 *	@param[out] _str 終端文字列
		 *	@param[in] ite チェック対象データ
		 *	@retval 0 コメントなし
		 *	@retval 1以上 _str の文字までスキップ
		 *	@retval -1 改行までスキップ
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

	/// パーツ種類コード
	enum EPartsKind
	{
		TOKEN,				///< トークン
		CHARACTOR,			///< 文字
		STRING,				///< 文字列
		PERIOD_CHAR,		///< トークン区切り文字
		OPEN_CHAR,			///< ブロック開始文字
		CLOSE_CHAR,			///< ブロック終端文字
		FINAL,				///< 終わり
		ERROR_STRING,		///< 文字列エラー
		ERROR_DEPTH,		///< 深さエラー
	};

	/**@ingroup GRAMMAR
	 * 文法解析用パーツ管理クラス
	 *
	 *		CGrammarAnalyzerT::GetNextParts() か CGrammarAnalyzerT::PeekNextParts() で得られます。
	 *		種類と内容、および、それがあった行と列を管理します。
	 *
	 *	@par必要ファイル
	 *			TnbGrammarAnalyzer.h
	 * 
	 *	@date 07/07/07	新規作成
	 */
	class CParts
	{
	public:

		/// コンストラクタ
		CParts(void) : m_kind(FINAL), m_str(), m_line(0), m_raw(0)
		{
		}

		/**
		 * コンストラクタ
		 *	@param kind 種類
		 *	@param s 内容
		 *	@param l 行
		 *	@param r 列
		 */
		CParts(EPartsKind kind, const CStrT<TYP>& s, INT_PTR l, INT_PTR r) 
			: m_kind(kind), m_str(s), m_line(l), m_raw(r)
		{
		}

		/**
		 * コンストラクタ
		 *	@param kind 種類
		 *	@param c 内容
		 *	@param l 行
		 *	@param r 列
		 */
		CParts(EPartsKind kind, TYP c, INT_PTR l, INT_PTR r)
			: m_kind(kind), m_str(), m_line(l), m_raw(r)
		{
			TYP at[2] = {c, 0};
			m_str = at;
		}

		/**
		 * コピーコンストラクタ
		 *	@param other コピー元
		 */
		CParts(const CParts& other) 
			: m_kind(other.m_kind), m_str(other.m_str), m_line(other.m_line), m_raw(other.m_raw)
		{
		}

		/**
		 * コピーコンストラクタ
		 *	@param other コピー元
		 *	@return 自分自身
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
		 * [取得] 種類
		 *	@return 種類
		 */
		EPartsKind GetKind(void) const
		{
			return m_kind;
		}

		/**
		 * [比較] 内容比較
		 *	@param lpsz 比較相手
		 *	@retval true 一致
		 *	@retval false 不一致
		 */
		bool IsEqualString(LPCSTR lpsz) const
		{
			return m_str.IsEqual(lpsz);
		}

		/**
		 * [取得] 内容
		 *	@return 内容
		 */
		const TYP* GetString(void) const
		{
			return m_str;
		}

		/**
		 * [取得] 内容.
		 *	@param index インデックス
		 *	@return 内容
		 */
		TYP GetAt(INDEX index = 0) const
		{
			return (m_str.GetLength() > index) ? m_str[index] : static_cast<TYP>(0);
		}

		/**
		 * [取得] 行番号取得
		 *	@return 行番号
		 */
		INT_PTR GetLine(void) const
		{
			return m_line;
		}

		/**
		 * [取得] 列番号取得
		 *	@return 列番号
		 */
		INT_PTR GetRaw(void) const
		{
			return m_raw;
		}

		/**
		 * [取得] 文字列化.
		 *	@note 開発用です。
		 *	@return 文字列
		 */
		CStr ToString(void) const
		{
			CStr str;
			switch ( m_kind )
			{
			case TOKEN:			str = _T("トークン");			break;
			case CHARACTOR:		str = _T("文字");				break;
			case STRING:		str = _T("文字列");				break;
			case PERIOD_CHAR:	str = _T("トークン区切り文字");	break;
			case OPEN_CHAR:		str = _T("ブロック開始文字");	break;
			case CLOSE_CHAR:	str = _T("ブロック終端文字");	break;
			case FINAL:			str = _T("終わり");				break;
			default:			str = _T("エラー");				break;
			}
			return CStr::Fmt(_T("%s : [%s] (%d,%d)"), str, CStr(m_str), m_line, m_raw);
		}

	private:
		EPartsKind	m_kind;		///< 種類
		CStrT<TYP>	m_str;		///< 内容
		INT_PTR		m_line;		///< 発生行
		INT_PTR		m_raw;		///< 発生列
	};

	/**
	 * コンストラクタ
	 *	@param is 開始イテレータ。省略可能。
	 *	@param ie 番兵イテレータ。省略可能。
	 *	@param line 行番号。省略可能。
	 */
	CGrammarAnalyzerT(ITE is = ITE(), ITE ie = ITE(), INT_PTR line = 1) 
		: m_param(is, ie, line), m_pFormat(&m_defaultFormat)
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
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
	 * コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
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
	 * [設定] フォーマッタ設定.
	 *	@param P フォーマッタ。NULLを指定すると、デフォルトに戻ります。
	 */
	void SetFormat(const IFormat* P)
	{
		m_pFormat = (P == NULL) ? &m_defaultFormat : P;
	}

	/**
	 * [設定] 名前設定
	 *	@note 任意の名前を一つ記憶させることが出来ます。
	 *	@param name 名前。
	 */
	void SetName(LPCTSTR name)
	{
		m_strName = name;
	}

	/**
	 * [取得] 名前取得
	 *	@note SetGrammarName で設定した名前を取得できます。
	 *	@return 名前。
	 */
	CStr GetName(void) const
	{
		return m_strName;
	}

	/**
	 * [取得] 行番号取得
	 *	@return 行番号
	 */
	UINT_PTR GetLine(void) const
	{
		return m_param.line;
	}

	/**
	 * [取得] 列番号取得
	 *	@return 列番号
	 */
	UINT_PTR GetRaw(void) const
	{
		return m_param.raw;
	}

	/**
	 * [取得] ポジション取得
	 *	@return ポジション番号
	 */
	UINT_PTR GetPos(void) const
	{
		return m_param.pos;
	}

	/**
	 * [取得] 現在のイテレータ
	 *	@note 現在のイテレータを得ることが出来ます。
	 *	@return イテレータ
	 */
	ITE GetPointer(void)
	{
		m_SkipBlankChar();
		return ite();
	}

	/**
	 * [取得] 深さ取得
	 *	@return 深さ
	 */
	INT_PTR GetDepth(void) const
	{
		return m_param.strDepth.GetLength();
	}

	/**
	 * [取得] ブロック文字
	 *	@retval 0 ブロックなし
	 *	@retval 0以外 ブロック開始文字
	 */
	TYP GetBlockChar(void) const
	{
		int l = m_param.strDepth.GetLength();
		return (l > 0) ? m_param.strDepth.GetAt(l - 1) : 0;
	}

	/**
	 * [取得] 次のパーツ取得
	 *	@note 文字列ポインタは、パーツの後をさしています。
	 *			ただし、FINALの場合文字列ポインタは動きません。
	 *	@param boIsToken ture か省略なら TOEKN で取り出します。false なら CHARACTOR で一文字だけ取り出します。
	 *	@return パーツ。
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
				// ブロック開始文字
				p = CParts(OPEN_CHAR, c, line, raw);
				inc();
				m_param.strDepth += c;
			}
			else if ( m_pFormat->IsBlockCloseChar(c) )
			{
				//ブロック終端文字
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
				// 文字列
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
				// トークン区切り文字
				p = CParts(PERIOD_CHAR, c, line, raw);
				inc();
			}
			else if ( boIsToken )
			{
				// トークン
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
	 * [処理] ブロックからステップアウト.
	 *	@retval false エラー。文字列ポインタは動きません。
	 *	@retval true 成功。文字列ポインタはブロック終端文字の次に移動します。
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
		inc();	//終端の次へ
		m_param.strDepth.DeleteLast();
		return true;
	}

	/**
	 * [確認] 次のパーツ確認
	 *	@note 文字列ポインタは動きません。
	 *	@param boIsToken ture か省略なら TOEKN で取り出します。false なら CHARACTOR で一文字だけ取り出します。
	 *	@return パーツ。
	 */
	CParts PeekNextParts(bool boIsToken = true)
	{
		TParam pm = m_param;
		CParts r = GetNextParts(boIsToken);
		m_param = pm;
		return r;
	}

	/** 
	 * [処理] ステップ.
	 *		指定数、ポインタを進めます。ただし、指定数進めたところがトークンの途中なら、
	 *		トークンの終わりまで進めてしまいます。
	 *	@param step ステップしたい数。
	 *	@return ステップした数。
	 */
	INT_PTR StepPointer(INT_PTR step)
	{
		INT_PTR top = m_param.pos;		// 現在のポジション
		while ( (m_param.pos - top) < step )
		{
			GetNextParts();
		}
		return m_param.pos - top;
	}

private:

	/// パラメータ
	struct TParam
	{
		ITE			iteNow;		///< 現在のイテレータ
		ITE			iteEnd;		///< 番兵イテレータ
		INT_PTR		pos;		///< 現在のポジション
		INT_PTR		line;		///< 現在の行番号
		INT_PTR		raw;		///< 現在の列番号
		CStrT<TYP>	strDepth;	///< ネスト深さ
		// コンストラクタ
		TParam(ITE is = ITE(), ITE ie = ITE(), INT_PTR l = 1) : iteNow(is), iteEnd(ie), pos(0), line(l), raw(0), strDepth()
		{
		}
		// コピーコンストラクタ
		TParam(const TParam& other)
			: iteNow(other.iteNow), iteEnd(other.iteEnd), pos(other.pos)
			, line(other.line), raw(other.raw), strDepth(other.strDepth)
		{
		}
		// コピーオペレータ
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

	TParam			m_param;				///< パラメータ
	IFormat			m_defaultFormat;		///< デフォルトのフォーマット
	const IFormat*	m_pFormat;				///< 使用するフォーマッタ
	CStr			m_strName;				///< 任意の名前

	/// ショートカット
	ITE ite(void){ return m_param.iteNow; }
	void inc(void) { m_param.iteNow++; m_param.pos++; m_param.raw++;} 
	void inc(INT_PTR i) { m_param.iteNow += i; m_param.pos += i; m_param.raw += i; } 

	/// ステップカウント用
	class CStepCnt
	{
		UINT_PTR m_bak;
	public:
		CStepCnt(const TParam& t) : m_bak(t.pos) {}
		UINT_PTR rst(const TParam& t){ return t.pos - m_bak; }
	};

	/**
	 * [確認] 終端チェック.
	 *	@note 現在のイテレータが終端かチェック。
	 *	@retval true 終端だった。
	 *	@retval false 違う。
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
	 * [処理] 改行スキップ.
	 *	@note 現在のイテレータが改行をさしていたら、その後へ移動。
	 *	@return 動いた文字数
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
	 * [比較] 比較
	 *	@note 現在のイテレータと文字列を比較。文字列長分のみ、チェックします。
	 *	@param str 比較文字列。
	 *	@retval true 一致。
	 *	@retval false 不一致。
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
	 * [処理] ブランク文字スキップ.
	 *	@note m_SkipBlankChar() のサブです。
	 *	@param r -1 改行までスキップ。／ 1以上 str の文字までスキップ
	 *	@param str スキップ先の文字列
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
				// str と同じ
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
	 * [処理] ブランク文字スキップ.
	 *	@note ブランク文字、および、コメント、および、改行をスキップします。
	 *	@return 動いた文字数
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
	 * [取得] トークン取得.
	 *	@note トークン区切り文字が最初にあると、その文字だけ返します。
	 *	@param[out] _token 取得トークン
	 *	@return 動いた文字数
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
	 * [操作] トークンスキップ.
	 *	@note トークン区切り文字が最初にあると、その文字だけスキップします。
	 *	@return 動いた文字数
	 */
	INT_PTR m_SkipToken(void)
	{
		CStrT<TYP> s;
		return m_GetToken(s);
	}

	/**
	 * [操作] ブロックスキップ.
	 *		今、文字列の始終端文字か、ブロックの始端文字なら、その文字列、ブロックをスキップします
	 *		（途中、ブロックがあればスキップします）。
	 *	@return 動いた文字数
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
	 * [操作] 指定文字までスキップ.
	 *	@param cCloseChar 「閉じ文字」
	 *	@retval -1 エラー。見つからない。文字列ポインタは動きません。
	 *	@retval 0以上 動いた文字数
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
				TRACE0("`('が綴じてません\n");
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
	 * [取得] 文字列取得.
	 *		文字列始終端文字をさしている時、文字列が取得できます。
	 *	@param[out] _str 取得文字列
	 *	@retval 0未満	文字列内のフォーマットエラー。（ポインタは動きます） 
	 *	@retval 0		文字列なし
	 *	@retval 0以上	動いた数（取得文字列の長さ＋２になっている)
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
	 * [処理] 文字列スキップ.
	 *		文字列始終端文字をさしている時、文字列をスキップします。
	 *	@retval 0未満	文字列内のフォーマットエラー。（ポインタは動きます） 
	 *	@retval 0		文字列なし
	 *	@retval 0以上	動いた数（取得文字列の長さ＋２になっている)
	 */
	INT_PTR m_SkipString(void)
	{
		CStrT<TYP> s;
		return m_GetString(s);
	}

	friend class CGrammarAnalyzerTest;
};



/**@ingroup GRAMMAR
 * 文法解析クラス(ASCII/SJIS)
 *
 *	@par必要ファイル
 *			TnbGrammarAnalyzer.h
 * 
 *	@date 07/07/07	新規作成
 */
typedef CGrammarAnalyzerT<> CGrammarAnalyzer;



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")

