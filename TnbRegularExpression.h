#pragma once
/**
 *@file
 * 正規表現関係のヘッダ
 *
 *		正規表現を扱うためのクラスがあります。	
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



#ifndef _TnbDOXYGEN	//Document作成用シンボル
/// 本クラス確認用 TRACE マクロ
#if 0
#define _REGEX_TRACE0(S)			TRACE0(S)
#define _REGEX_TRACE1(S,P1)			TRACE1(S, P1)
#define _REGEX_TRACE2(S,P1,P2)		TRACE2(S,P1,P2)
#define _REGEX_TRACE3(S,P1,P2,P3)	TRACE3(S,P1,P2,P3)
#else
#define _REGEX_TRACE0(S)
#define _REGEX_TRACE1(S,P1)
#define _REGEX_TRACE2(S,P1,P2)
#define _REGEX_TRACE3(S,P1,P2,P3)
#endif
#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup REGEX
 * 正規表現管理クラステンプレート
 *
 *		一つの正規表現をコンパイルした物を管理します。
 *		それを使って、文字列等と比較、検索、置換することが可能です。
 *
 *
 *	\par 正規表現構文の要約:
 *	\par ・文字
 *		x　			文字 x												\n
 *		￥￥　		バックスラッシュ文字								\n
 *		￥0n　		8 進値 0n を持つ文字 (0 <= n <= 7)					\n
 *		￥0nn　		8 進値 0nn を持つ文字 (0 <= n <= 7)					\n
 *		￥0mnn　	8 進値 0mnn を持つ文字 (0 <= m <= 3、0 <= n <= 7)	\n
 *		￥xhh　		16 進値 0xhh を持つ文字								\n
 *		￥uhhhh　	16 進値 0xhhhh を持つ文字							\n
 *		￥t　		タブ文字 ('￥x09')									\n
 *		￥n　		改行文字 ('￥x0A')									\n
 *		￥r　		キャリッジリターン文字 ('￥x0D')					\n
 *		￥f　		用紙送り文字 ('￥x0C')								\n
 *		￥e　		エスケープ文字 ('￥x1B')
 *
 *	\par ・定義済みの文字クラス
 *		.　		任意の文字											\n
 *		￥d　 	数字: [0-9]											\n
 *		￥D　 	数字以外: [^0-9]									\n
 *		￥s　 	空白文字: [ ￥t￥n￥x0B￥f￥r]						\n
 *		￥S　 	非空白文字: [^￥s]									\n
 *		￥w　 	単語構成文字: [a-zA-Z_0-9]							\n
 *		￥W　 	非単語文字: [^￥w]									
 *
 *	\par ・文字クラス
 *		[abc]　			a、b、または c (単純クラス)					\n
 *		[^abc]　		a、b、c 以外の文字 (否定)					\n
 *		[a-zA-Z]　		a ～ z または A ～ Z (範囲)					\n
 *		[a-d[m-p]]　	a ～ d、または m ～ p: [a-dm-p] (結合)		\n
 *		[a-z&&[def]]　	d、e、f (交差) 								\n
 *		[a-z&&[^bc]]　	b と c を除く a ～ z: [ad-z] (減算)			\n
 *		[a-z&&[^m-p]]　	m ～ p を除く a ～ z: [a-lq-z] (減算)		\n
 *		※文字列クラス内では、数量子,論理演算子,前方参照は、
 *		  ただの文字として認識されます。
 *
 *	\par ・最長一致数量子
 *		X?　		X、1 回または 0 回								\n
 *		X＊　		X、0 回以上										\n
 *		X＋　		X、1 回以上										\n
 *		X{n}　		X、n 回											\n
 *		X(n,}　		X、n 回以上										\n
 *		X{n,m}　	X、n 回以上、m 回以下						
 *
 *	\par ・最短一致数量子
 *		X??　		X、1 回または 0 回								\n
 *		X＊?　		X、0 回以上										\n
 *		X＋?　		X、1 回以上										\n
 *		X{n}?　		X、n 回											\n
 *		X(n,}?		X、n 回以上										\n
 *		X{n,m}?　	X、n 回以上、m 回以下							\n
 *		※これらは、 X が任意の文字(ピリオド)を指定した時のみ、有効です。
 *
 *	\par ・論理演算子
 *		XY　		X の直後に Y									\n
 *		X|Y　		X または Y										\n
 *		(X)			グループ（前方参照あり）						\n
 *		(?:X)		グループ（前方参照なし）						\n
 *		X(?=Y)		肯定先読み										\n
 *		X(?!Y)		否定先読み										\n
 *		(?<=X)Y		肯定後読み										\n
 *		(?<!X)Y		否定後読み										\n
 *
 *	\par ・前方参照
 *		￥n　	マッチした n 番目の前方参照を行う正規表現グループ
 *
 *	\par 正規表現グループと前方参照:
 *		前方参照を行う正規表現グループには、左から右方向に左丸括弧を数えることによって、
 *		番号が付けられます。たとえば、表現 ((A)(B(C))) は、次の 4 つのグループに分類さ
 *		れます。			\n
 *		1　	((A)(B(C)))		\n
 *		2　	(A)				\n
 *		3　	(B(C))			\n
 *		4　	(C)				\n
 *		グループ 0 は、常に表現全体を表します。
 *
 *	@param TYP char (ASCII/SJIS) か WCHAR (UNICODE) を指定します。
 *	@param ITE TYPを扱うイテレータ。省略すると、 const TYP* になります。
 *
 *	\par使用例
 *		@code
 *		// a,b,またはc であることを調査
 *		VERIFY( re.SetPattern("[abc]") );
 *		VERIFY( re.Matches("c") );
 *
 *		// ':'を敷居にしてテキストを分割
 *		VERIFY( re.SetPattern(":") );
 *		CAsciiVector va = re.Split("boo:and:foo", NULL, 3) );
 *
 *		// 小文字のアルファベットを検索
 *		VERIFY( re.SetPattern("[a-z]") );
 *		VERIFY( re.Find("123r789").foundIndex == 3 );
 *
 *		// 行の先頭がjavaという文字で始まっているか検索
 *		VERIFY( re.SetPattern("^java") );
 *		VERIFY( re.Find("javava").foundIndex, 0 );
 *		VERIFY( re.Find("a java").foundIndex, INVALID_INDEX );
 *		@endcode
 *
 *	@todo 現在 ”.＊//” のように、最長一致数量子の後に同じ文字があると、失敗します。
 *		 ”.＊(?://)”のように記述が必要です。
 *
 *	@par必要ファイル
 *			TnbRegularExpression.h
 * 
 *	@date 07/09/01 新規作成
 *	@date 07/09/25 「グループに数量子をつけた直後に前方参照」に、対応。
 *	@date 07/09/29 グループを含んだグループに数量子をつけた時の動きを Java と同じにした。
 *	@date 08/02/13 (?:X) が未対応だった。対応。
 */
template<typename TYP, typename ITE = const TYP*>
class CRegularExpressionT
{
public:

	/**
	 * オプション.
	 *	マッチング方法を指定できます。
	 *	@see SetOption
	 */
	enum EOption
	{
		IGNORECASE = _BIT(0),		///< 「欧文の大文字と小文字を区別しない」オプション
//		MULTILINE = _BIT(1),		///< 「複数行モードを有効」オプション
	};

private:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	/// チェッカー抽象クラス
	class CAbstractChecker
	{
	public:
		/// グループ用イテレータペア
		struct TPairIterator
		{
			ITE		top;			///< 開始イテレータ
			ITE		end;			///< 番兵イテレータ
			bool	boValidEnd;		///< true なら番兵有効
			/// コンストラクタ
			TPairIterator(ITE s = ITE(), ITE e = ITE(), bool r = false)
				: top(s), end(e), boValidEnd(r)
			{
			}
			/// 番兵セット
			void SetEnd(ITE e)
			{
				end = e;
				boValidEnd = true;
			}
		};
		/// チェック用静的パラメータ
		struct TStaticParam
		{
			ITE						end;				///< 番兵イテレータ
			CVectorT<TPairIterator>	groupIterators;		///< グループ情報
			ITE						tempEnd;			///< 暫定番兵
			ITE						outsizeEnd;			///< 範囲外イテレータ
			DWORD					option;				////< オプション
			/// コンストラクタ
			TStaticParam(void) : end(ITE()), tempEnd(ITE()), option(0)
			{
			}
			size_t GetSize(void) const
			{
				return groupIterators.GetSize();
			}
			TPairIterator At(INDEX index) const
			{
				const TPairIterator& ii = groupIterators.At(index);
				TPairIterator r((index == 0 && outsizeEnd != ITE()) ? outsizeEnd : ii.top);
				r.SetEnd(ii.boValidEnd ? ii.end : tempEnd);
				return r;
			}
			/// グループ名群、取得
			CVectorT< CStrT<TYP> > ToStrings(void) const
			{
				CVectorT< CStrT<TYP> > vs;
				loop ( i, GetSize() )
				{
					const TPairIterator& ii = At(i);
					vs.Add(CStrT<TYP>::FromIterator(ii.top, ii.end));
				}
				return vs;
			}
		};
		/// チェック用パラメータ
		struct TParam
		{
			ITE				cur;	///< カレントイテレータ
			ITE				top;	///< 開始イテレータ
			TStaticParam*	pt;		///< 番兵やフラグ、グループ名
			/// コンストラクタ
			TParam(ITE c, ITE t, ITE e, TStaticParam* p) : cur(c), top(t), pt(p) 
			{
				pt->end = e;
				pt->tempEnd = e;
				pt->outsizeEnd = ITE();
			}
			/// @return カレントイテレータが番兵か￥０を指していたら true。
			bool IsTerminate(void) const
			{
				return (cur == pt->end) ? true : (*cur == 0);
			}
			/// @return カレントイテレータから一文字取得。
			size_t GetWord(WORD& _ch)
			{
				if ( IsTerminate() ){ return INVALID_SIZE; }
				WORD c = *cur;
				size_t l = STRLIB::GetCharSize(*cur);
				if ( l == 2 )
				{
					ASSERT( sizeof(TYP) == 1 );
					cur++;
					if ( IsTerminate() ){ return INVALID_SIZE; }
					c = static_cast<WORD>(((c & 0xFF) << 8) | ((*cur) & 0xFF));
				}
				_ch = c;
				return l;
			}
			/// カレントイテレータStep
			void StepCur(size_t l)
			{
				if ( l != INVALID_SIZE )
				{
					loop ( i, l )
					{
						ASSERTLIB( cur != pt->end );
						cur++;
					}
				}
			}
			/// 比較
			bool EqualChar(TYP c1, TYP c2)
			{
				if ( (pt->option & IGNORECASE) != 0 )
				{
					if ( c1 >= 'A' && c1 <= 'Z' ) { c1 |= 0x20; }
					if ( c2 >= 'A' && c2 <= 'Z' ) { c2 |= 0x20; }
				}
				return c1 == c2;
			}
		};
		/// デストラクタ
		virtual ~CAbstractChecker(void){}
		/**
		 * [比較] チェック処理.
		 *	@param pi パラメータ
		 *	@retval INVALID_SIZE 非一致。
		 *	@retval 0以上 一致。数値は一致した数。
		 */
		virtual size_t Check(TParam ip) const = 0;
	};

	/// IChecker ポインタのハンドル型宣言
	typedef CPointerHandleT<CAbstractChecker>	CCheckerPtr;

	/// IChecker ポインタのハンドル型の配列型宣言
	typedef CVectorT<CCheckerPtr>		CCheckerPtrsVector;

	//================================================

	/** 「指定文字と完全一致」チェッカー
	 *	@note 指定した文字列が全部あったら一致とする。
 	 */
	class CEqualChar : public CAbstractChecker
	{
		WORD	m_char;		///< 有効文字
	public:
		/// コンストラクタ
		CEqualChar(WORD c) : m_char(c)
		{
		}
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "正規表現 比較 ; EqualChar [0x%X]\n", m_char );
			WORD c;
			size_t l = ip.GetWord(c);
			if ( l == INVALID_SIZE ){ return INVALID_SIZE; }
			bool r = ip.EqualChar(static_cast<TYP>(c), static_cast<TYP>(m_char));
			return r ? l : INVALID_SIZE;
		}
	};

	/** 「指定文字コード範囲」チェッカー
	 *	@note 指定したtop,bottm内なら一致とする。
	 *	@note リバースフラグあり。
 	 */
	class CRangeChar : public CAbstractChecker
	{
		WORD	m_top;			///< 有効文字コード上限
		WORD	m_bottom;		///< 有効文字コード下限
		bool	m_reverse;		///< 条件反転フラグ
	public:
		/// コンストラクタ
		CRangeChar(WORD top, WORD bottom, bool boIsReverse = false) 
			: m_top(top), m_bottom(bottom), m_reverse(boIsReverse)
		{
			if ( m_top > m_bottom ) { Swap(m_top, m_bottom); }
		}
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE3( "正規表現 比較 ; RangeChar ['%c' - '%c' (%d)]\n", m_top, m_bottom, m_reverse );
			WORD c;
			size_t l = ip.GetWord(c);
			if ( l == INVALID_SIZE ){ return INVALID_SIZE; }
			if ( (ip.pt->option & IGNORECASE) != 0 )
			{
				if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
				{
					c |= 0x20;
					return ((c < (m_top|0x20) || c > (m_bottom|0x20)) ^ m_reverse) ? INVALID_SIZE : l;
				}
			}
			return ((c < m_top || c > m_bottom) ^ m_reverse) ? INVALID_SIZE : l;
		}
	};

	/** 「どんな文字でも一致」チェッカー
 	 */
	class CAnythingChar : public CAbstractChecker
	{
	public:
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE0( "正規表現 比較 ; AnythingChar\n" );
			return ip.IsTerminate() ? INVALID_SIZE : (STRLIB::GetCharSize(*ip.cur));
		}
	};

	/** 「先頭」チェッカー
	 */
	class CTop : public CAbstractChecker
	{
	public:
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE0( "正規表現 比較 ; Top\n" );
			return (ip.cur == ip.top) ? 0 : INVALID_SIZE;
		}
	};

	/** 「末端」チェッカー
	 */
	class CEnd : public CAbstractChecker
	{
	public:
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE0( "正規表現 比較 ; End\n" );
			return ip.IsTerminate() ? 0 : INVALID_SIZE;
		}
	};

	/** 「前方参照一致」チェッカー
	*/
	class CFrontWord : public CAbstractChecker
	{
		INDEX		m_index;	///< 抽出No
	public:
		CFrontWord(INDEX i) : m_index(i)
		{
		}
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "正規表現 比較 ; FrontWord (index = %d)\n", m_index );
			if ( ip.pt->groupIterators.GetSize() <= m_index )
			{
				return INVALID_SIZE;
			}
			const TPairIterator& ii = ip.pt->groupIterators.At(m_index);
			ITE is = ii.top;
			ITE ie = ii.boValidEnd ? ii.end : ip.pt->tempEnd;
//			if ( ! ii.boValidEnd )
//			{
//				CStrT<TYP> s = CStrT<TYP>::FromIterator(is, ie);
//				TRACE2( "[%s] [%s]\n", CStr(s), CStr(ip.cur) );
//			}
			size_t r = 0;
			while ( is != ie )
			{
				if ( *is == 0 ) { break; }
				if ( ip.cur == ip.pt->end ) { return INVALID_SIZE; }
				if ( ! ip.EqualChar(*is++, *ip.cur++) ){ return INVALID_SIZE; }
				r++;
			}
			return r;
		}
	};

	//================================================

	/** 「グループ管理」チェッカー
	 *	@note グループのはじめにこれをはさんでおく
	 *	@note 唯一のチェッカーが 成功したら、その文字列を保存する。
 	 */
	class CGroupChecker : public CAbstractChecker
	{
	public:
		enum EType
		{
			ET_Non,
			ET_RefGroup,		///< (regex)			グループ（前方参照あり）
			ET_NoRefGroup,		///< (?:regex)			グループ（前方参照なし）
			ET_AfterEqu,		///< regex(?=regex)		肯定先読み
			ET_AfterNeq,		///< regex(?!regex)		否定先読み
			ET_BeforeEqu,		///< (?<=regex)regex	肯定後読み
			ET_BeforeNeq,		///< (?<!regex)regex	否定後読み
		};
	private:
		EType	m_type;
		CCheckerPtr	m_checker;		///> チェッカー
	public:
		/// コンストラクタ
		CGroupChecker(CCheckerPtr cp, EType y = ET_RefGroup) : m_checker(cp), m_type(y)
		{
			ASSERTLIB( y != ET_Non );
		}
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "正規表現 比較 ; GroupChecker (%d)\n", m_type );
			if ( m_type == ET_RefGroup )	// (regex) グループ（前方参照あり）
			{
				INDEX insertIndex = ip.pt->groupIterators.GetSize();
				ip.pt->groupIterators.Add(TPairIterator(ip.cur, ip.pt->end));	//ダミーデータを追加しておく。
				size_t l = m_checker->Check(ip);
				if ( l != INVALID_SIZE )
				{
					ip.StepCur(l);
					ip.pt->groupIterators.Ref(insertIndex).SetEnd(ip.cur);
				}
				return l;
			}
			size_t l = m_checker->Check(ip);
			switch ( m_type )
			{
			case ET_RefGroup:	// (regex) グループ（前方参照あり）
				ip.StepCur(l);
				break;
			case ET_NoRefGroup:	// (?:regex) グループ（前方参照なし）
				ip.StepCur(l);
				break;
			case ET_AfterEqu:	// regex(?=regex) 肯定先読み
				l = (l != INVALID_SIZE) ? 0 : INVALID_SIZE;
				break;
			case ET_AfterNeq:	// regex(?!regex) 否定先読み
				l = (l == INVALID_SIZE) ? 0 : INVALID_SIZE;
				break;
			case ET_BeforeEqu:	// (?<=regex)regex 肯定後読み
			case ET_BeforeNeq:	// (?<!regex)regex 否定後読み
				if ( ip.pt->outsizeEnd != ITE() )
				{
					l = INVALID_SIZE;
				}
				else
				{
					if ( l != INVALID_SIZE )
					{
						ip.StepCur(l);
						ip.pt->outsizeEnd = ip.cur;
					}
					if ( m_type == ET_BeforeNeq )	// (?<!regex)regex 否定後読み
					{
						l = (l == INVALID_SIZE) ? 0 : INVALID_SIZE;
					}
				}
				break;
			default:
				ASSERTLIB( false );
				break;
			}
			return l;
		}
	};
	
	/** 「チェッカ一致箇所」チェッカー
	 *	@note 最後に一致したところを返す
	 */
	class CFindChecker : public CAbstractChecker
	{
		CCheckerPtr		m_loopChecker;	///< 繰り返しチェッカー
		CCheckerPtr		m_markChecker;	///< 末チェッカー
		int				m_min;			///< 最小連続数。
		int				m_max;			///< 最大連続数。
		bool			m_isMini;		///< true なら最単一致
	public:
		/// コンストラクタ
		CFindChecker(CCheckerPtr loopChk, int min, int max, bool mini)
			: m_loopChecker(loopChk), m_min(min), m_max(max), m_isMini(mini)
		{
		}
		void SetMarkChecker(CCheckerPtr cp)
		{
			ASSERTLIB( this != cp.operator->() );
			m_markChecker = cp;
		}
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			CVectorT<TPairIterator> bakGrpItes;
			ITE bakIte = ip.pt->tempEnd;
			size_t ns = ip.pt->groupIterators.GetSize();
			ip.pt->tempEnd = ip.cur;
			if ( m_markChecker.IsNull() )
			{
				_REGEX_TRACE2( "正規表現 比較 ; FindChecker(%d ～ %d) 「ENDかMAXまで」\n", m_min, m_max);
				INDEX findingIndex = 0;
				int foundCount = 0;
				while ( true )
				{
					if ( m_max >= 0 && foundCount == m_max ) { break; }
					if ( m_isMini && m_min <= foundCount ) { break; }
					size_t l = m_loopChecker->Check(ip);
					if ( ns < ip.pt->groupIterators.GetSize() )
					{
						if ( l != INVALID_SIZE )
						{
							bakGrpItes = ip.pt->groupIterators;
						}
						ip.pt->groupIterators.SetSize(ns);
					}
					if ( l == INVALID_SIZE ) { break; }
					ip.StepCur(l);
					ip.pt->tempEnd = ip.cur;
					foundCount++;
					findingIndex += l;
				}
				ip.pt->tempEnd = bakIte;
				if ( ! bakGrpItes.IsEmpty() )
				{
					ip.pt->groupIterators = bakGrpItes;
				}
				return (foundCount < m_min) ? INVALID_SIZE : findingIndex;
			}
			_REGEX_TRACE2( "正規表現 比較 ; FindChecker(%d ～ %d) 「次一致」\n", m_min, m_max);
			INDEX findingIndex = 0;
			INDEX foundIndex = INVALID_INDEX;
			int foundCount = 0;
			while ( ! ip.IsTerminate() )
			{
				if ( foundCount >= m_min )
				{
					size_t l = m_markChecker->Check(ip);
					if ( l != INVALID_SIZE )
					{
						foundIndex = findingIndex;
						if ( m_isMini ) { break; }
					}
					ns = ip.pt->groupIterators.GetSize();
				}
				if ( m_max >= 0 && foundCount == m_max ) { break; }
				size_t l = m_loopChecker->Check(ip);
				if ( ns < ip.pt->groupIterators.GetSize() )
				{
					if ( l != INVALID_SIZE )
					{
						bakGrpItes = ip.pt->groupIterators;
					}
					ip.pt->groupIterators.SetSize(ns);
				}
				if ( l == INVALID_SIZE ) { break; }
				ip.StepCur(l);
				ip.pt->tempEnd = ip.cur;
				foundCount++;
				findingIndex += l;
			}
			ip.pt->tempEnd = bakIte;
			if ( ! bakGrpItes.IsEmpty() )
			{
				ip.pt->groupIterators = bakGrpItes;
			}
			return (foundCount < m_min) ? INVALID_SIZE : foundIndex;
		}
	};

	/** 「チェッカーのどれかと一致」チェッカー
	 *	@note m_nextは未使用	
 	 */
	class CAnyChecker : public CAbstractChecker
	{
	protected:
		CCheckerPtrsVector	m_checkers;	///< チェッカ群
	public:
		/// このチェッカーは意味あるか？ trueなら無意味
		virtual bool IsMeaningless(void) const
		{
			return m_checkers.GetSize() == 1;
		}
		/// 最初のチェッカー取得
		virtual CCheckerPtr& Top(void)
		{
			return m_checkers[0];
		}
		/// チェッカー追加
		virtual void Add(CCheckerPtr chk)
		{
			m_checkers.Add(chk);
		}
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "正規表現 比較 ; AnyChecker(%d)\n", m_checkers.GetSize() );
			size_t r = INVALID_SIZE;
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l != INVALID_SIZE )
				{
					if ( r == INVALID_SIZE ) { r = 0; }
					if ( r < l ) { r = l; }
				}
			}
			return r;
		}
	};

	/** 「チェッカーのどれとも一致しない」チェッカー
	 *	@note m_nextは未使用	
 	 */
	class CNotAnyChecker : public CAnyChecker
	{
	public:
		/// このチェッカーは意味あるか？ trueなら無意味
		virtual bool IsMeaningless(void) const
		{
			return false;	//このクラスは絶対「意味がある」
		}
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "正規表現 比較 ; NotAnyChecker(%d)\n", m_checkers.GetSize() );
			if ( ip.IsTerminate() ){ return INVALID_SIZE; }
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l != INVALID_SIZE ) { return INVALID_SIZE; }
			}
			return STRLIB::GetCharSize(*ip.cur);
		}
	};

	/** 「チェッカー、どれも一致」チェッカー
	 *	@note 同じ箇所を複数のチェッカでチェックする。結果長も同じでないと一致としない。
	 *	@note m_nextは未使用	
	 */
	class CBesidesChecker : public CAnyChecker
	{
	public:
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "正規表現 比較 ; BesidesChecker(%d)\n", m_checkers.GetSize() );
			size_t r = INVALID_SIZE;
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l == INVALID_SIZE ) { return INVALID_SIZE; }
				if ( r == INVALID_SIZE )
				{
					r = l;
				}
				else if ( r != l )
				{
					return INVALID_SIZE;
				}
			}
			ip.StepCur(r);
			return r;
		}
	};

	/** 「連続チェック」チェッカー
 	 */
	class CLineChecker : public CAnyChecker
	{
	public:
		/// チェック
		virtual size_t Check(TParam ip) const 
		{
			_REGEX_TRACE1( "正規表現 比較 ; LineChecker(%d)\n", m_checkers.GetSize() );
			ASSERTLIB( m_checkers.GetSize() != 0 ); 
			size_t r = 0;	//一致したトータル長さ
			loop ( i, m_checkers.GetSize() )
			{
				size_t l = m_checkers[i]->Check(ip);
				if ( l == INVALID_SIZE ) { return INVALID_SIZE; }
				ip.StepCur(l);
				r += l;
			}
			return r;
		}
	};

	#endif // _TnbDOXYGEN

	//================================================

	CCheckerPtr		m_root;				///< メインチェッカ
	CStrT<TYP>		m_base;				///< 正規表現文字列
	CFindChecker*	m_pLastFindChecker;	///< 最後の検索チェッカー
	DWORD			m_option;			///< オプション

	/// 内部情報開放
	void m_RemoveAll(void)
	{
		m_root.Null();
		m_base.Empty(); 
		m_pLastFindChecker = NULL;
	}

	/*
	 * _lpsz は{ の次の文字を指している
	 */
	bool m_SubNum(int& _r, const WORD*& _lpsz)
	{
		_r = -1;
		const WORD* P =_lpsz;
		while( *P >= '0' && *P <= '9' )
		{
			if ( _r < 0 ) { _r = 0; }
			_r *= 10;
			_r += (*P - '0');
			P++;
		}
		if ( _r >= 0 )
		{
			_lpsz = P;
			return true;
		}
		return false;
	}

	/*
	 * _lpsz は{ の文字を指している
	 *	_checker には繰り返すチェッカーが入っている。
	 */
	bool m_SubCnt(int& _min, int& _max, const WORD*& _lpsz)
	{
		const WORD* P =_lpsz;
		int r;
		if ( *P++ == '{' )
		{
			if ( m_SubNum(r, P) )
			{
				if ( *P != ',' )
				{
					_min = r;
					_max = r;
				}
				else
				{
					_min = r;
					P++;
					if ( m_SubNum(r, P) )
					{
						_max = r;
					}
				}
				if ( *P == '}' )
				{
					_lpsz = P + 1;
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * 指定のAnyCheckerが意味あるかチェック
	 *	意味無いならその中のTOPのCheckerを返す。
	 */
	CCheckerPtr m_ChkChecker(CAnyChecker* P)
	{
		if ( P->IsMeaningless() )
		{
			CCheckerPtr c = P->Top();
			delete P;
			return c;
		}
		return P;
	}

	/*
	 * isAny が false なら
	 *		" " , ( ) 内のチェック用
	 *		@note ”か ) か \0 までをチェック
	 * isAny が true なら
	 *		[ ]内のチェック用
	 *		@note ]までをチェック
	 *
	 *	@param _lpsz ”か（か [ の次の文字ポインタ。成功時、終端の文字ポインタになっています。
	 */
	bool m_Sub(CCheckerPtr& _checker, const WORD*& _lpsz, bool isAny)
	{
		CFindChecker* pLastFindChecker = m_pLastFindChecker;
		m_pLastFindChecker = NULL;
		CAnyChecker* pCheckers = NULL;
		const WORD* P = _lpsz;
		if ( isAny )
		{
			if ( *P == '^' )
			{
				P++;
				pCheckers = new CNotAnyChecker();
			}
			else
			{
				pCheckers = new CAnyChecker();
			}
		}
		else
		{
			pCheckers = new CLineChecker();
		}
		bool isError = false;
		bool isFirst = true;
		WORD rangeChar = 0;
		while ( ! isError )
		{
			WORD c = *P++;
			CCheckerPtr chk;
			CGroupChecker::EType groupType = CGroupChecker::ET_Non;
			CFindChecker* pLastFind = NULL;
			if ( ! isAny )
			{
				//Any以外でのみ有効なもの
				// [～]ないではでは無効なもの
				switch ( c )
				{
				case '.':
					chk = new CAnythingChar();
					break;
				case '(':
					groupType = CGroupChecker::ET_RefGroup;
					if ( *P == '?' )
					{
						if ( P[1] == ':' )
						{
							groupType = CGroupChecker::ET_NoRefGroup;
							P += 2;
						}
						else if ( P[1] == '=' )
						{
							groupType = CGroupChecker::ET_AfterEqu;
							P += 2;
						}
						else if ( P[1] == '!' )
						{
							groupType = CGroupChecker::ET_AfterNeq;
							P += 2;
						}
						else if ( P[1] == '<' && P[2] == '=' )
						{
							groupType = CGroupChecker::ET_BeforeEqu;
							P += 3;
						}
						else if ( P[1] == '<' && P[2] == '!' )
						{
							groupType = CGroupChecker::ET_BeforeNeq;
							P += 3;
						}
					}
					if ( ! m_Sub(chk, P, false) || *P != ')' )
					{
						groupType = CGroupChecker::ET_Non;
						isError = true;
						break;
					}
					P++;
					pLastFind = m_pLastFindChecker;
					m_pLastFindChecker = NULL;
					break;
				case ')':
					_checker = m_ChkChecker(pCheckers);
					if ( pLastFindChecker != NULL )
					{
						pLastFindChecker->SetMarkChecker(_checker);
					}
					_lpsz = P - 1;
					return true;
				case '|':
					{
						CFindChecker* pCkBack = m_pLastFindChecker;
						m_pLastFindChecker = NULL;
						CCheckerPtr cp;
						if ( m_Sub(cp, P, false) && (*P == 0 || *P == ')') )
						{
							m_pLastFindChecker = pCkBack;
							CAnyChecker* a = new CAnyChecker();
							a->Add(m_ChkChecker(pCheckers));
							a->Add(cp);
							_checker = a;
							if ( pLastFindChecker != NULL )
							{
								pLastFindChecker->SetMarkChecker(_checker);
							}
							_lpsz = P;
							return true;
						}
					}
					isError = true;
					break;
				case '^':
					if ( isFirst )
					{
						chk = new CTop();
					}
					break;
				case '$':
					if ( *P == 0 )
					{
						chk = new CEnd();
					}
					break;
				default:
					break;
				}
			}
			if ( isError ) { break;	}
			//--
			if ( chk.IsNull() )
			{
				switch ( c )
				{
				case '[':
					if ( isFirst && isAny )
					{
						;
					}
					else if ( ! m_Sub(chk, P, true) || *P != ']' )
					{
						isError = true;
					}
					else
					{
						P++;
						pLastFind = m_pLastFindChecker;
						m_pLastFindChecker = NULL;
					}
					break;
				case ']':
					if ( isFirst && isAny )
					{
						break;
					}
					_checker = m_ChkChecker(pCheckers);
					if ( pLastFindChecker != NULL )
					{
						pLastFindChecker->SetMarkChecker(_checker);
					}
					_lpsz = P - 1;
					return true;
				case 0:
					_checker = m_ChkChecker(pCheckers);
					if ( pLastFindChecker != NULL )
					{
						pLastFindChecker->SetMarkChecker(_checker);
					}
					_lpsz = P - 1;
					return true;
				case '&':
					if ( isAny && *P == '&' )
					{
						P++;
						CFindChecker* pCkBack = m_pLastFindChecker;
						m_pLastFindChecker = NULL;
						CCheckerPtr cp;
						if ( m_Sub(cp, P, false) && *P == ']' )
						{
							m_pLastFindChecker = pCkBack;
							CAnyChecker* a = new CBesidesChecker();
							a->Add(m_ChkChecker(pCheckers));
							a->Add(cp);
							_checker = a;
							if ( pLastFindChecker != NULL )
							{
								pLastFindChecker->SetMarkChecker(_checker);
							}
							_lpsz = P;
							return true;
						}
						isError = true;
					}
					break;
				case '\\':
					ASSERTLIB( chk.IsNull() );
					c = *P++;
					switch ( c )
					{
					case 'd':	// 数字([0-9])
					case 'D':	//	数字以外([~0-9])
						chk = new CRangeChar('0', '9', (c == 'D'));
						break;
					case 's':	//	空白(\t\n\x0B\f\r)				※全角の含む？
					case 'S':	//	空白以外([~\s])
						{
							CAnyChecker* pAny = (c == 's') ? new CAnyChecker() : new CNotAnyChecker();
							const char ap[] = { ' ', '\t', '\n', '\x0B', '\f', '\r' };
							loop ( i, countof(ap) )
							{
								pAny->Add(new CEqualChar(ap[i]));
							}
							chk = pAny;
						}
						break;
					case 'w':	//	単語([a-zA-Z0-9_])
					case 'W':	//	単語以外([~\W])
						{
							CAnyChecker* pAny = (c == 'w') ? new CAnyChecker() : new CNotAnyChecker();
							pAny->Add(new CRangeChar('a', 'z'));
							pAny->Add(new CRangeChar('A', 'Z'));
							pAny->Add(new CRangeChar('0', '9'));
							pAny->Add(new CEqualChar('_'));
							chk = pAny;
						}
						break;
					default:
						P -= 2;
						int r = STRLIB::EscCharToInt(P);
						if ( r >= 0 )
						{
							c = static_cast<WORD>(r);
						}
						else
						{
							P += 2;
							if ( c >= '0' && c <= '9' )
							{
								chk = new CFrontWord(c - '0');
							}
						}
						break;
					}
					break;
				default:
					break;
				}
			}
			if ( isError ) { break;	}
			//--
			if ( chk.IsNull() )
			{
				if ( *P == '-' )
				{
					P++;
					WORD d = *P++;
					if ( d == '\\' )
					{
						--P;
						int r = STRLIB::EscCharToInt(P);
						if ( r >= 0 )
						{
							d = static_cast<WORD>(r);
						}
						else
						{
							isError = true;
						}
					}
					chk = new CRangeChar(c, d);
				}
				else
				{
					chk = new CEqualChar(c);
				}
			}
			if ( isError ) { break;	}
			//--
			if ( ! chk.IsNull() )
			{
				if ( rangeChar != 0 )
				{
					isError = true;
				}
				else if ( ! isAny )
				{
					int min = -1;
					int max = -1;
					switch ( *P )
					{
					case '*': //0個以上
						min = 0;
						P++;
						break;
					case '+': //1個以上
						min = 1;
						P++;
						break;
					case '?': //0個か1個
						min = 0;
						max = 1;
						P++;
						break;
					case '{': // {a} a個のならび、{a,} a個以上、{a,b} a個～b個。
						if ( ! m_SubCnt(min, max, P) )
						{
							isError = true;
							min = -1; //無駄な処理しないように
						}
						break;
					default:
						break;
					}
					if ( min >= 0 )
					{
						bool isMini = false;
						if ( *P == '?' )
						{
							P++;
							isMini = true;
						}
						if ( groupType != CGroupChecker::ET_Non )
						{
							chk = new CGroupChecker(chk, groupType);
							groupType = CGroupChecker::ET_Non;
						}
						pLastFind = new CFindChecker(chk, min, max, isMini);
						chk = pLastFind;
					}
				}
				if ( ! chk.IsNull() )
				{
					if ( groupType != CGroupChecker::ET_Non )
					{
						chk = new CGroupChecker(chk, groupType);
						groupType = CGroupChecker::ET_Non;
					}
					if ( m_pLastFindChecker != NULL )
					{
						m_pLastFindChecker->SetMarkChecker(chk);
					}
					m_pLastFindChecker = pLastFind;
					pLastFind = NULL;
					pCheckers->Add(chk);
					chk.Null();
				}
			}
			isFirst = false;
		}
		delete pCheckers;
		return false;
	}

	/**
	 * [比較] マッチ
	 *	@param[in] ip 対象情報。
	 *	@param[out] pGroupStr 文字列の配列を指定するとグループの文字列を得ることができます。
	 *				不一致でも一致したところまでは格納されます。
	 *	@retval INVALID_SIZE 不一致。
	 *	@retval INVALID_SIZE 以外 一致。
	 */
	size_t m_MatchSize(typename CAbstractChecker::TParam ip, CVectorT< CStrT<TYP> >* pGroupStr) const
	{
		if ( m_root.IsNull() )
		{
			return INVALID_SIZE;
		}
		size_t l = m_root->Check(ip);
		if ( ip.pt->outsizeEnd != ITE() )
		{
			l = INVALID_SIZE;
		}
		if ( pGroupStr != NULL )
		{
			*pGroupStr = ip.pt->ToStrings();
		}
		return l;
	}

public:

	/// コンストラクタ
	CRegularExpressionT(void) : m_option(0)
	{
	}

	/**
	 * [設定] オプション設定.
	 *	本パターンを使用する時のオプションを指定します。
	 *	@note 一度指定すると、インスタンスが有効中、状態を保持します。
	 *	@note インスタンス作成時は、 オプションが一つも設定されていない状態です。
	 *	@param dw {@link CRegularExpressionT::EOption オプション}を指定します。同時に複数指定する場合は | で連結します。
	 */
	void SetOption(DWORD dw)
	{
		m_option = dw;
	}
	
	/**
	 * [設定] パターン設定.
	 *	@param lpsz パターン
	 *	@retval true 成功。
	 *	@retval false 失敗（パターン文字列に誤りあり）。
	 */
	bool SetPattern(const TYP* lpsz)
	{
		m_RemoveAll();
		CWordVector vw = TNB::StringToWordVector(lpsz);
		vw.Add(0);
		vw.Add(0); //保険
		const WORD* P = vw.ReferBuffer();
		CCheckerPtr chk;
		if ( ! m_Sub(chk, P, false) )
		{
			m_RemoveAll();
			return false;
		}
		if ( *P != 0 )
		{
			m_RemoveAll();
			return false;
		}
		m_root = new CGroupChecker(chk);
		m_base = lpsz;
		return true;
	}

	/**
	 * [比較] 先頭マッチ
	 *	@note 本インスタンスが持っている条件と先頭マッチしているかチェックします。
	 *	@param[in] is 対象情報の開始イテレータ。
	 *	@param[in] ie 対象情報の番兵イテレータ。省略する場合、必ず NIL終端がある情報を使用してください。
	 *	@param[out] pGroupStr 文字列の配列を指定するとグループの文字列を得ることができます。
	 *				不一致でも一致したところまでは格納されます。省略することも出来ます。
	 *	@retval INVALID_SIZE 不一致。
	 *	@retval INVALID_SIZE 以外 一致。
	 */
	size_t LookingAt(ITE is, ITE ie = ITE(), CVectorT< CStrT<TYP> >* pGroupStr = NULL) const
	{
		CAbstractChecker::TStaticParam sp;
		sp.option = m_option;
		CAbstractChecker::TParam ip(is, is, ie, &sp);
		return m_MatchSize(ip, pGroupStr);
	}

	/**
	 * [比較] 全体マッチ
	 *	@note 本インスタンスが持っている条件と全体がマッチしているかチェックします。
	 *	@param[in] is 対象情報の開始イテレータ。
	 *	@param[in] ie 対象情報の番兵イテレータ。省略する場合、必ず NIL終端がある情報を使用してください。
	 *	@param[out] pGroupStr 文字列の配列を指定するとグループの文字列を得ることができます。
	 *				不一致でも一致したところまでは格納されます。省略することも出来ます。
	 *	@retval 一致。
	 *	@retval 不一致。
	 */
	bool Matches(ITE is, ITE ie = ITE(), CVectorT< CStrT<TYP> >* pGroupStr = NULL) const
	{
		CAbstractChecker::TStaticParam sp;
		sp.option = m_option;
		CAbstractChecker::TParam ip(is, is, ie, &sp);
		size_t l = m_MatchSize(ip, pGroupStr);
		if ( l == INVALID_SIZE )
		{
			return false;
		}
		ip.StepCur(l);
		return ip.IsTerminate();
	}

	/**@ingroup REGEX
	 * Find() メソッド用リザルト
	 *	@see Find の戻り値で使用されます。
	 *	@par必要ファイル
	 *			TnbRegularExpression.h
	 *	@date 07/09/01 新規作成
	 */
	struct TFindResult
	{
		INDEX		foundIndex;	///< マッチした位置
		size_t		foundSize;	///< マッチした長さ
		/// コンストラクタ
		TFindResult(void) : foundIndex(0), foundSize(0) {}
	};

	/**
	 * [検索] 検索
	 *	@note 最初にマッチした情報を返します。連続で検索する場合、 GetFinder() で CFinder を取得したほうが便利です。
	 *	@param[in] is 対象情報の開始イテレータ。
	 *	@param[in] ie 対象情報の番兵イテレータ。省略する場合、必ず NIL終端がある情報を使用してください。
	 *	@param[out] pGroupStr 文字列の配列を指定するとグループの文字列を得ることができます。省略可能。
	 *	@return リザルト。 foundSize が INVALID_SIZE なら見つからなかった。 
	 */
	TFindResult Find(ITE is, ITE ie = ITE(), CVectorT< CStrT<TYP> >* pGroupStr = NULL) const
	{
		CAbstractChecker::TStaticParam sp;
		sp.option = m_option;
		CAbstractChecker::TParam ip(is, is, ie, &sp);
		TFindResult r;
		if ( ! m_root.IsNull() )
		{
			while ( ! ip.IsTerminate() )
			{
				ip.top = is;
				sp.groupIterators.RemoveAll();
				sp.outsizeEnd = ITE();
				r.foundSize = m_root->Check(ip);
				if ( r.foundSize != INVALID_SIZE )
				{
					if ( sp.outsizeEnd != ITE() )
					{
						while ( is != ip.cur )
						{
							is++;
						}
						while ( is != sp.outsizeEnd )
						{
							is++;
							r.foundIndex++;
							r.foundSize--;
						}
					}
					if ( pGroupStr != NULL ) 
					{
						*pGroupStr = ip.pt->ToStrings();
					}
					return r;
				}
				if ( sp.outsizeEnd != ITE() )
				{
					while ( ip.cur != sp.outsizeEnd )
					{
						r.foundIndex++;
						ip.cur++;
					}
				}	
				if ( ! ip.IsTerminate() && STRLIB::GetCharSize(*ip.cur) == 2 )
				{
					r.foundIndex++;
					ip.cur++;
				}
				r.foundIndex++;
				ip.cur++;
			}
		}
		r.foundSize = INVALID_SIZE;
		r.foundIndex = INVALID_INDEX;
		if ( pGroupStr != NULL ) 
		{
			*pGroupStr = ip.pt->ToStrings();
		}
		return r;
	}

	/**
	 * [取得] 分割.
	 *	@note	このパターンのマッチに基づいて、指定された入力シーケンスを分割します。 
	 *			このメソッドから返される配列は、入力シーケンスの部分文字列で構成されます。
	 *			これらの部分文字列は、このパターンとマッチする別の部分シーケンスによって
	 *			分割された文字列か、入力シーケンスの終了によって分割された文字列です。
	 *			配列内の部分文字列は、入力シーケンス内で発生した順番に並んでいます。
	 *			このパターンが入力シーケンスの部分シーケンスにマッチしない場合、
	 *			返される配列は 1 つの要素、つまり文字列形式の入力シーケンスだけで構成されます。 
	 *	@note	limit パラメータは、このパターンの適用回数、つまり、返される配列の長さを制御します。
	 *			制限 n がゼロより大きい場合、このパターンは n - 1 回以下の回数が適用され、
	 *			配列の長さは n 以下になります。
	 *			配列の最後のエントリには、最後にマッチした区切り文字以降の入力シーケンスがすべて含まれます。
	 *			n が負の値の場合、このパターンの適用回数と配列の長さは制限されません。n がゼロの場合、
	 *			このパターンの適用回数と配列の長さは制限されませんが、後続の空の文字列は破棄されます。
	 *	@param is 対象情報の開始イテレータ。
	 *	@param ie 対象情報の番兵イテレータ。省略する場合、必ず NIL終端がある情報を使用してください。
	 *	@param limit 適用回数。省略すると０が適用されます。
	 *	@return 分割した文字列群。 
	 */
	CVectorT< CStrT<TYP> > Split(ITE is, ITE ie = ITE(), int limit = 0) const
	{
		CVectorT< CStrT<TYP> > vs;
		while ( limit <= 0 || static_cast<int>(vs.GetSize()) < limit - 1 )
		{
			TFindResult r = Find(is, ie);
			if ( r.foundIndex == INVALID_INDEX )
			{
				break;
			}
			CStrT<TYP> s; 
			loop ( i, r.foundIndex )
			{
				s += *is++;
			}
			vs.Add(s);
			loop ( i, r.foundSize )
			{
				is++;
			}
		}
		vs.Add(CStrT<TYP>::FromIterator(is, ie));
		if ( limit == 0 )
		{
			for ( INDEX i = vs.GetSize() - 1; i > 0; i-- )
			{
				if ( ! vs[i].IsEmpty() )
				{
					break;
				}
				vs.Remove(i);
			}
		}
		return vs;
	}

	/**@ingroup REGEX
	 * 正規表現ファインダクラス
	 *	
	 *	順次検索が可能。また、必要に応じて置換することが可能です。
	 *	
	 *	@par使用例
	 *		<table><tr><td><pre>
	 *	
	 *		CRegularExpressionT<char> re;
	 *		re.SetPattern("([ABC])-");
	 *		CRegularExpressionT<char>::CFinder dd = re.GetFinder("A-B-9C!C-!");
	 *		if ( dd.IsFinding() )
	 *		{
	 *			do{
	 *				TRACE2A("index = %d,[%s]\n", dd.GetFoundIndex(), dd.GetFoundString() );			
	 *				dd.Replace("ZZ\\1");
	 *				TRACE1A("[%s]\n", dd.GetString() );
	 *			}
	 *			while( dd.Next() );
	 *			TRACE1A("[%s]\n", dd.GetString() );
	 *		}
	 *	
	 *		</pre></td></tr></table>
	 *
	 *	@see CRegularExpressionT::GetFinder() により作成されます。
	 *
	 *	@par必要ファイル
	 *			TnbRegularExpression.h
	 *
	 *	@date 07/09/01 新規作成
	 */
	class CFinder
	{
		const CRegularExpressionT*	m_pRegEx;		///< パターン
		ITE							m_is;			///< 開始イテレータ
		ITE							m_ie;			///< 番兵イテレータ
		CStrT<TYP>					m_strDst;		///< 変換後文字列
		INDEX						m_index;		///< 検索INDEX
		CStrT<TYP>					m_foundString;	///< 発見した文字列
		INDEX						m_foundIndex;	///< 発見したINDEX
		CVectorT< CStrT<TYP> >		m_foundGroupStr;///< 発見したときのグループ文字列
	public:

		/**
		 * コンストラクタ
		 *	@note 通常、直接インスタンスを作らず、 CRegularExpressionT::GetFinder() を使用します。
		 *	@param P 正規表現管理クラスのインスタンス。
		 *	@param is 対象情報の開始イテレータ。
		 *	@param ie 対象情報の終了番兵イテレータ。省略するとデフォルトの番兵になります。
		 */
		CFinder(const CRegularExpressionT* P = NULL, ITE is = ITE(), ITE ie = ITE()) 
			: m_foundIndex(0), m_index(0), m_pRegEx(P), m_is(is), m_ie(ie)
		{
			Next();
		}

		/**
		 * [確認] 検索確認
		 *	@retval true 検索中（GetFound系、 Replace() メソッド有効）
		 *	@retval false 検索なし
		 */
		bool IsFinding(void) const
		{
			return m_pRegEx != NULL;
		}

		/**
		 * [検索] 次検索
		 *	@note 次に一致する場所を検索します。
		 *	@retval true 次検索成功（GetFound系、 Replace() メソッド有効）
		 *	@retval false 次なし
		 */
		bool Next(void)
		{
			if ( m_pRegEx != NULL )
			{
				m_strDst += m_foundString;
				m_foundString.Empty();
				TFindResult r = m_pRegEx->Find(m_is, m_ie, &m_foundGroupStr);
				if ( r.foundSize == INVALID_SIZE )
				{
					m_pRegEx = NULL;
				}
				else
				{
					m_strDst += CStrT<TYP>::FromIterator(m_is, m_ie, r.foundIndex);
					loop ( i, r.foundIndex ) { m_is++; }
					m_foundString = CStrT<TYP>::FromIterator(m_is, m_ie, r.foundSize);
					m_foundIndex = m_index + r.foundIndex;
					loop ( i, r.foundSize ) { m_is++; }
					m_index += r.foundIndex + r.foundSize;
				}
			}
			return m_pRegEx != NULL;
		}

		/**
		 * [置換] 置き換え.
		 *	@note マッチした文字列を指定の置換文字列で置換します。その際、以下のキーワードが使用できます。
		 *	\par ・キーワード
		 *			￥０～￥９　		マッチした文字列のブロックが挿入されます。	\n
		 *			￥￥　　　		￥文字が挿入されます。
		 *	@param lpsz 置換文字列。
		 *	@retval true 成功。
		 *	@retval false 失敗。
		 */
		bool Replace(const TYP* lpsz)
		{
			if ( ! IsFinding() ) { return false; }
			CStrT<TYP> s;
			while ( *lpsz != 0 )
			{
				if ( STRLIB::GetCharSize(*lpsz) == 2 )
				{
					s += *lpsz++;
					s += *lpsz++;
				}
				else if ( *lpsz != '\\' )
				{
					s += *lpsz++;
				}
				else
				{
					lpsz++;
					TYP c = *lpsz++;
					if ( c >= '0' && c <= '9' )
					{
						INDEX i = c - '0';
						if ( m_foundGroupStr.GetSize() > i )
						{
							s += m_foundGroupStr[i];
						}
					}
					else
					{
						s += c;
					}
				}
			}
			m_foundString = s;
			return true;
		}

		/**
		 * [取得] マッチした文字列取得.
		 *	@attention Replace() をすると、得られる文字列は置き換えたものなります。
		 *	@return 文字列
		 */
		CStrT<TYP> GetFoundString(void) const
		{
			return m_foundString;
		}

		/**
		 * [取得] マッチした文字列長取得.
		 *	@attention Replace() をすると、得られる文字列は置き換えたものなります。
		 *	@return 文字列長
		 */
		size_t GetFoundSize(void) const
		{
			return m_foundString.GetLength();
		}

		/**
		 * [取得] マッチしたグループ文字列取得.
		 *	@return グループ文字列
		 */
		const CVectorT< CStrT<TYP> >& GetFoundGroupString(void) const
		{
			return m_foundGroupStr;
		}

		/**
		 * [取得] マッチした位置取得.
		 *	@return 位置
		 */
		INDEX GetFoundIndex(void) const
		{
			return m_foundIndex;
		}

		/**
		 * [取得] 置換結果文字列取得.
		 *	@note 検索し置換した結果の文字列を取得できます。
		 *	最後までチェックしていなくても置換したところまでが取得できます。
		 *	@return 置換結果文字列
		 */
		CStrT<TYP> GetString(void) const
		{
			return m_strDst + m_foundString + CStrT<TYP>::FromIterator(m_is, m_ie);
		}
	};

	/**
	 * [取得] ファインダー取得.
	 *	@param is 対象情報の開始イテレータ。
	 *	@param ie 対象情報の終了番兵イテレータ。省略するとデフォルトの番兵になります。
	 *	@return ファインダー。 
	 */
	CFinder GetFinder(ITE is, ITE ie = ITE()) const
	{
		return CFinder(this, is, ie);
	}

	/**
	 * [置換] 置き換え.
	 *	@note マッチした文字列を指定の置換文字列で置換します。その際、以下のキーワードが使用できます。
	 *	\par ・キーワード
	 *			￥０～￥９　		マッチした文字列のブロックが挿入されます。	\n
	 *			￥￥　　　		￥文字が挿入されます。
	 *	@note 一つずつ確認しながら行う場合、 GetFinder() を使用します。
	 *	@param lpsz 置換文字列。
	 *	@param is 対象情報の開始イテレータ。
	 *	@param ie 対象情報の番兵イテレータ。省略する場合、必ず NIL終端がある情報を使用してください。
	 *	@return 置換結果文字列
	 */
	CStrT<TYP> ReplaceAll(const TYP* lpsz, ITE is, ITE ie = ITE()) const
	{
		CFinder f(this, is, ie);
		if ( f.IsFinding() )
		{
			do { f.Replace(lpsz); } while ( f.Next() );
		}
		return f.GetString();
	}

private:
	friend class CRegularExpressionTest;
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")


#if 0

正規表現グループと前方参照 

前方参照を行う正規表現グループには、左から右方向に左丸括弧を数えることによって、
番号が付けられます。たとえば、表現 ((A)(B(C))) は、次の 4 つのグループに分類さ
れます。

	1   ((A)(B(C)))
	2	(A)
	3	(B(C))
	4	(C)
グループ 0 は、常に表現全体を表します。


http://oraclesqlpuzzle.hp.infoseek.co.jp/regex/



#endif


#if 0

http://programnet.hp.infoseek.co.jp/practical/regex.html
郵便番号	　\d{3}-\d{4}
携帯番号	　090-\d{4}-\d{4}
電話番号	　\d{1,4}?-\d{1,4}?-\d{1,4}
生年月日	　\d{4}-\d{2}-\d{2}
メールアドレス	　[!#-9A-~]+@[a-z0-9-_]+\.+[a-z0-9-_]+\.+[a-z0-9-]



よくある勘違い その2
前述した通り、 ブラケットに囲まれている中ではほとんどのメタキャラ
クタはその特殊な意味を失います。 したがって、 [^(foo)]bar という
正規表現もまた、「fooではない文字列に続いてbarという文字列が続くもの」
 ではありません。「fでもoでも(でも)でもない文字に続いて barという
 文字列が続いたもの」 です。 []の内側では、文字列や(より小さな)
 正規表現要素をまとめるというカッコの 特別な意味は失われてしまうと
 いうことに注意してください。


正規表現の確認可能 HP
http://lab.moyo.biz/tools/regexp/index.jsp
#endif

#if 0
モード

複数行モード					＊	'^' と '$' が各行の始まりと終わりにマッチするようになります。 
.が改行にもマッチするモード		＊	'.' が改行文字にもマッチするようにします。
大文字と小文字を同一視モード	＊	大文字と小文字を区別しないようにします。







#endif
