#pragma once
/**
 *@file
 * TinyMacroLang 変数管理関係のヘッダ
 *	
 *		TMLの変数管理のクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFilePathManager.h"
#include "TnbTmlCalculator.h"
#include "TnbTmlDocuments.h"
#include "TnbTmlVariableManager.h"



//TNB Library
namespace TNB
{



/**@defgroup TML	TinyMacroLang
 *
 *	\par
 *		マクロ言語を扱うクラスがあります。
 *
 *		関数定義を拡張するために、 ITmlExpander を実装したクラス作成し、
 *		CTmlParser に渡します( CTmlParser::SetExpander() )。
 *		実行進捗は、 CTmlParser::IListener を実装したクラスを作成し、
 *		CTmlParser に渡します( CTmlParser::SetListener() )。
 */



/**@ingroup TML
 * TinyMacroLang パーサー
 *	
 *	@par必要ファイル
 *			TnbTmlParser.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
class CTmlParser : public CTml, ITmlExpander
{
public:

	/**@ingroup TML
	 * TinyMacroLang リスナー
	 *
	 *	@see SetListener()
	 *
	 *	@par必要ファイル
	 *			TnbTmlParser.h
	 *
	 *	@date 06/01/01 新規作成
	 *	@date 09/08/18 改新
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] 継続確認通知.
		 *		処理を続けてよいか問い合わせの通知です。
		 *	@note すぐに応答を返さないことで、Pause的な動きが実現できます。
		 *		中止なら処理の結果が ETR_UserInterruption になります。
		 *	@retval true 処理続行。
		 *	@retval false 中断要求。
		 */
		virtual bool IsExecutionContinued(void) = 0;

		/**
		 * [通知] ログ出力通知.
		 *		LOGに出力する内容を通知します。
		 *	@param lpszLog 内容
		 *	@param boHasErrMsg エラーメッセージならtrue
		 */
		virtual void OutputLog(LPCTSTR lpszLog, bool boHasErrMsg) = 0;
	};

	/**
	 * LOG表示レベル.
	 *	@see SetLogLevel
	 */
	enum ELogLevel
	{
		ELL_ABSOLUTE,		///< 絶対
		ELL_FUNC,			///< 関数/命令
		ELL_BRANCH,			///< 分岐
		ELL_DETAILS,		///< 詳細
	};


	//--------------------------------


	/// コンストラクタ 
	CTmlParser(void) : m_piListener(NULL), m_piExpander(NULL), m_eLogLevel(ELL_ABSOLUTE), m_iLogLastLine(0)
	{
		m_calc.SetExpander(this);
		m_variable.IncLevel();
		m_documents.IncLevel();
	}

	/// デストラクタ
	~CTmlParser(void)
	{
	}

	/** 
	 * [設定] テストモード設定/解除.
	 *	@note テストモード＝文法チェックモードです。このモードで実行すると、
	 *			全ステートメントがtrueとして実行されます。つまり、全条件を、
	 *			実行し、文法のチェックが行えます。
	 *	@param boIsTestMode trueならテストモード。
	 */
	void SetTestMode(bool boIsTestMode)
	{
		m_calc.SetTestMode(boIsTestMode);
	}

	/**
	 * [取得] テストモード状態取得
	 *	@retval true テストモード中。
	 *	@retval false 通常。
	 */
	bool IsTestMode(void) const
	{
		return m_calc.IsTestMode();
	}

	/**
	 * [設定] クリア.
	 *		持しているコンテンツ、変数内容をクリアし、レベルを１にします。
	 */
	void Empty(void)
	{
//		::PlaySound(NULL, NULL, 0);
		m_documents.Empty();
		m_documents.IncLevel();
		m_variable.Empty();
		m_variable.IncLevel();
		m_cInterruptInfo.Empty();
		m_strLogLastName.Empty();
		m_iLogLastLine = 0;
	}

	/**
	 * [設定] リスナー用インターフェイス設定.
	 *	@param piListener リスナーの参照。省略すると解除になります。
	 */
	void SetListener(IListener* piListener = NULL)
	{
		m_piListener = piListener;
	}

	/**
	 * [設定] 拡張関数のインターフェイス設定.
	 *	@param piExpander 拡張オブジェクトの参照。省略すると解除になります。
	 */
	void SetExpander(ITmlExpander* piExpander = NULL)
	{
		m_piExpander = piExpander;
	}

	/** 
	 * [設定] ログレベル、設定.
	 *	@param eLevel レベル。
	 */
	void SetLogLevel(ELogLevel eLevel)
	{
		m_eLogLevel = eLevel;
	}

	/**
	 * [追加] マクロファイルの追加.
	 *		変数レベルと､コンテンツレベルは、変更せず使用されます。
	 *	@param lpszName ファイル名
	 *	@param lpszPath パス名（省略なら、プロセスパスからの相対になります）
	 *	@return 結果。
	 */
	CTmlResult AddMacroFile(LPCTSTR lpszName, LPCTSTR lpszPath = NULL)
	{
		CTmlResult trRet;
		//
		CFilePathManager pathMan;
		CFileReader reader = pathMan.Open(lpszName, lpszPath);
		if ( reader.CanRead() )
		{
			try
			{
				CByteVector vc = reader.ReadExactly();
				vc.Add(0);
				return AddMacroContents(reader.GetOpenName(), reinterpret_cast<LPCSTR>(vc.ReferBuffer()));
			}
			catch ( CTnbException& e )
			{
				e;
				trRet = CTmlResult(Operation_FileAccess, Result_ReadFailure);
			}
		}
		else
		{
			trRet = CTmlResult(Operation_FileAccess, Result_OpenFailure);
		}
		m_MessageBug( "ファイル'%s'が読み込めません。", CAscii(lpszName));
		reader.Close();
		return trRet;
	}
	
	/**
	 * [追加] マクロコンテンツの追加.
	 *		変数レベルと､コンテンツレベルは、変更せず使用されます。
	 *	@param lpszName ファイル名(登録用)
	 *	@param lpszData コンテンツ内容
	 *	@return 結果。
	 */
	CTmlResult AddMacroContents(LPCTSTR lpszName, LPCSTR lpszData)
	{
		CTmlDocuments::TFuncInfoRes tFunc;
		CTmlResult res = m_documents.Add(tFunc, CAscii(lpszName), lpszData);
		if ( res.IsError() )
		{
			return res;
		}
		else if ( res.IsNop() )
		{
			return Result_Success;	//同じファイルを読み込んでいた。
		}
		//
		if ( ! tFunc.HasData() )
		{
			ASSERTLIB( false );	//おかしい、ありえない？！
			return Result_Nop;
		}
		//あった
		CTmlGrammar gram(tFunc.lpszFuncAdds, NULL, tFunc.iFuncLine);
		gram.SetName(CStr(tFunc.strFileName));
		//	
		CTmlValue tvResult;
		return ExecMacro(gram, tvResult);
	}

	/**
	 * [設定] マクロレベル＋１.
	 *		AddMacro～()で追加するレベルを指定します。マクロ追加の前に＋１しておくと、
	 *		Decするだけで、追加されたマクロを破棄することができます。
	 *		マクロ実行中には実行しないでください。
	 */
	void IncMacroLevel(void)
	{
		m_documents.IncLevel();
		m_variable.IncLevel();
	}

	/**
	 * [設定] マクロレベル－１.
	 *		AddMacro～()で追加するレベルを指定します。マクロ追加の前に＋１しておくと、
	 *		Decするだけで、追加されたマクロを破棄することができます。
	 *		マクロ実行中には実行しないでください。
	 */
	void DecMacroLevel(void)
	{
		m_variable.DecLevel();
		m_documents.DecLevel();
	}

	/**
	 * [確認] 指定関数の有無チェック.
	 *		指定の関数が登録されているか。
	 *	@param lpszFuncName 検索する関数名
	 *	@retval true 指定の関数がある。
	 *	@retval false 無い。
	 */
	bool ExistFunction(LPCSTR lpszFuncName) const
	{
		CTmlDocuments::TFuncInfoRes tFunc;
		tFunc = m_documents.FindFunc(lpszFuncName);
		return tFunc.HasData();
	}

	/**
	 * [処理] 指定関数の実行.
	 *		テキスト位置情報を退避して実行し、終了後元に戻しています。
	 *	@param[out] _answer 結果の値。
	 *	@param lpszFuncName 関数名
	 *	@return 結果
	 */
	CTmlResult ExecFunction(CTmlValue& _answer, LPCSTR lpszFuncName)
	{
		CTmlValuesVector aValues;
		return ExecFunction(_answer, lpszFuncName, aValues);
	}

	/**
	 * [処理] 指定関数の実行.
	 *		テキスト位置情報を退避して実行し、終了後元に戻しています。
	 *	@param[out] _answer 結果の値。
	 *	@param lpszFuncName 関数名
	 *	@param aValues 引数
	 *	@return 結果
	 */
	CTmlResult ExecFunction(CTmlValue& _answer, LPCSTR lpszFuncName, const CTmlValuesVector& aValues)
	{
		CTmlDocuments::TFuncInfoRes tFunc;
		tFunc = m_documents.FindFunc(lpszFuncName);
		if ( ! tFunc.HasData() )
		{
			return Result_Nop;
		}
		//あった
		m_variable.IncLevel();
		if ( tFunc.pastrParamNames == NULL )
		{
			//パラメータなし
			if ( aValues.GetSize()!=0 )
			{
				//引数があるのでエラー
				m_variable.DecLevel();
				m_MessageBug("引数のない func '%s'に引数を渡しています。", lpszFuncName);
				return CTmlResult(Operation_CheckFunc, Result_UnnecessaryParam);
			}
		}
		else
		{
			size_t dim = tFunc.pastrParamNames->GetSize();
			if ( dim < aValues.GetSize() )
			{
				//引数が多すぎ
				m_MessageBug("func '%s'(引数は %d個)に、%d個のに引数を渡しています。",
							lpszFuncName,
							dim,
							aValues.GetSize()

				);
				m_variable.DecLevel();
				return CTmlResult(Operation_CheckFunc, Result_ManyParam);
			}
			//引数変数、追加
			for ( size_t i = 0; i < dim; i++ )
			{
				CTmlValue value;
				if ( aValues.GetSize() > i )
				{
					value = aValues.At(i);
				}
				m_variable.AddValue(tFunc.pastrParamNames->At(i), value);
			}
		}
		//
		CTmlGrammar gram(tFunc.lpszFuncAdds, NULL, tFunc.iFuncLine);
		gram.SetName(CStr(tFunc.strFileName));
		CTmlResult res = ExecMacro(gram, _answer);
		m_variable.DecLevel();
		//
		if ( ! res.IsSuccess() )
		{
			if ( CAscii(res.GetFuncName()).IsEmpty() )
			{
				//なければ設定
				res.SetFuncName(lpszFuncName);
			}
		}
		return res;
	}

	/**
	 * [処理] マクロ実行.
	 *		文解析＆実行。 ブロック終了文字か NIL文字まで処理します。
	 *	@param[out] _answer 結果の値。
	 *	@param[in,out] _gram マクロ情報。
	 *	@return 動いた文字数 マイナスならエラー(ETmCTmlResult)
	 */
	CTmlResult ExecMacro(CTmlGrammar& _gram, CTmlValue& _answer)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar backGram = _gram;
		CTmlGrammar::CParts parts;
		//
		CTmlResult res;
		_answer = 0;
		//
		CNestInfo cNest;
		INT_PTR iStartVarLevel = m_variable.GetLevel();
		INT_PTR iStartContentsLevel = m_documents.GetLevel();
		//
		while ( true )
		{
			backGram = _gram;
			//割り込みチェック
			{
				CTmlResult r = m_CheckInterrupt();
				if ( ! r.IsSuccess() )
				{
					res = r;
					break;
				}
			}
			//
			parts = _gram.PeekNextParts();
			CTmlGrammar::EPartsKind kind = parts.GetKind();
			CAscii token = parts.GetString();
			if ( kind == CTmlGrammar::ERROR_STRING )
			{
				//エラー
				res = Result_NotCloseParentheses;
				res.SetOperation(Operation_CheckStatement);
				res.SetLine(backGram);
				m_MessageBug(res, "文字列表記にエラーがあります。");
				break;
			}
			else if ( token == ";" )
			{
				_gram.GetNextParts();
				continue;
			}
			else if ( token == "{" )
			{
				_gram.GetNextParts();
				cNest.Inc(cNest.EK_Non);
				m_variable.IncLevel();
				m_documents.IncLevel();
				continue;
			}
			else if ( token == "}" || kind == CTmlGrammar::FINAL )
			{
				if ( cNest.IsTopDepth() )
				{
					m_MessageLog(backGram, ELL_DETAILS, "深さ０で閉じ文字がありました。関数を終わります。");
					break;
				} 
				else if ( cNest.GetNowKind() == cNest.EK_While )
				{
					m_MessageLog(backGram, ELL_BRANCH, "whileネストの'}'がありました。while文へ戻ります。");
					_gram = cNest.GetWhileInfo();
					cNest.Dec();
					m_variable.DecLevel();
					m_documents.DecLevel();
				}
				else
				{
					_gram.GetNextParts();
					cNest.Dec();
					m_variable.DecLevel();
					m_documents.DecLevel();
				}
				continue;
			}
			//
			backGram = _gram;
			EStatementResultType stateResType;
			CTmlResult r = m_SearchAndExecCommand(_gram, cNest, _answer, stateResType);
			//
			if ( r.IsSuccess() )
			{
				//	ステートメント処理、あり
				if ( stateResType == ESRT_Return/*return文*/ )
				{
					TTRACE0(_T("return文なので関数終了\n"));
					break;
				}
				else if ( stateResType == ESRT_Function/*func文*/ )
				{
					if ( cNest.IsTopDepth() )
					{
						m_MessageLog(_gram, ELL_DETAILS, "深さ０で'func'がありました。関数を終わります。");
						break;
					}
					else
					{
						res = Result_InvalidWordPosition;
						res.SetScene(Scene_Func);
						res.SetOperation(Operation_CheckStatement);
						res.SetLine(backGram);
						m_MessageBug(res, "func文が発見されました。ここには func宣言は記述できません。");
						break;
					}
				}
			}
			else if ( r.IsError() )
			{
				//エラー
				res = r;
				break;
			}
			else
			{
				//ステートメントではなかったので、数式？
				CTmlValue v;
				r = m_calc.TextToValue(v, _gram);
				if ( r.IsSuccess() )
				{
					/*数式だった*/
				}
				else if ( r.IsError() )
				{
					//エラー
					res = r;
					break;
				}
				else
				{
					/*謎*/
					CAscii str = backGram.PeekNextParts().GetString();
					CTmlResult r(Scene_Non, Operation_CheckStatement, Result_UnknownKeyword);
					r.SetErrorString(str);
					r.SetLine(backGram);
					m_MessageBug(r, "謎の文 '%s...'  があります。", str.Left(32));
					return r;
				}
			}
		}
		//
		// 変数レベルを関数開始時に戻す
		m_variable.DecLevels(iStartVarLevel);
		m_documents.DecLevels(iStartContentsLevel);
		//
		if ( res.IsError() )
		{
			if ( res.GetFileLine()==0 )
			{
				//エラー行ファイル名が未設定なら、
				res.SetLine(_gram);
			}
			return res;
		}
		return Result_Success;
	}

private:

	/**
	 * TinyMacroLang 割込関数情報管理.
	 *		配列で、発動時間と関数名を保持しています。
	 */
	class CInterruptFunc
	{
	public:
		/// コンストラクタ
		CInterruptFunc(void) : m_iLastAccessId(0)
		{
		}
		/// 初期化
		void Empty(void)
		{
			m_iLastAccessId = 0;
			m_astrFuncInfos.RemoveAll();
		}
		/**
		 * 関数登録.
		 *		関数の有無はチェックしてから登録してください。
		 *	@param lpszFuncName 登録する関数名
		 *	@param dwTick	ms後に動作するか？
		 *	@return InterruptId。KILL時に使用します。
		 */
		int Set(LPCSTR lpszFuncName, DWORD dwTick)
		{
			ASSERT( lpszFuncName != NULL && lpszFuncName[0]!=0 );
			INT_PTR iFindingId = -1;
			TFuncInfo T;
			T.dwTick = ::GetTickCount() + dwTick;
			T.strName = lpszFuncName;
			size_t len = m_astrFuncInfos.GetSize();
			for( size_t i = m_iLastAccessId; i < len; i++ )
			{
				if ( m_astrFuncInfos[i].strName.IsEmpty() )
				{
					m_astrFuncInfos[i] = T;
					iFindingId = i;
					break;
				}
			}
			if ( iFindingId < 0 )
			{
				for ( size_t i = 0; i < m_iLastAccessId && i < len; i++ )
				{
					if ( m_astrFuncInfos[i].strName.IsEmpty() )
					{
						m_astrFuncInfos[i] = T;
						iFindingId = i;
						break;
					}
				}
			}
			if ( iFindingId < 0 )
			{
				iFindingId = m_astrFuncInfos.Add(T);
			}
			TTRACE3( "TML::CInterruptFunc::Set() %dms後に関数[%s]を実行予定 ID=%d\n", dwTick, lpszFuncName, iFindingId );
			m_iLastAccessId = iFindingId + 1;
			return ToInt(iFindingId);
		}
		/**
		 * 解除
		 *	@param iInterruptId	Set()の戻り値を指定します。
		 */
		void Kill(int iInterruptId)
		{
			if ( ToInt(m_astrFuncInfos.GetSize()) > iInterruptId )
			{
				if ( ! m_astrFuncInfos[iInterruptId].strName.IsEmpty() )
				{
					m_astrFuncInfos[iInterruptId].strName.Empty();
					m_iLastAccessId = iInterruptId + 1;
					TTRACE1("CInterruptFunc::Kill() ID=%dの割り込み停止\n", iInterruptId );
				}
			}
		}
		/**
		 * [確認] チェック.
		 *		時間になった関数リストを返します。
		 *		登録は解除されます。
		 *	@return 時間になった関数名群
		 */
		CAsciiVector Check(void)
		{
			CAsciiVector vs;
			DWORD dwNow = ::GetTickCount();
			loop ( i, m_astrFuncInfos.GetSize() )
			{
				if ( m_astrFuncInfos[i].strName.IsEmpty() )
				{
					continue;
				}
				int r = static_cast<int>(dwNow - m_astrFuncInfos[i].dwTick);
				if ( r >= 0 )
				{
					TTRACE2( "TML::CInterruptFunc::Check() ID=%dの割込実行 Func=[%s]\n", i,
							m_astrFuncInfos[i].strName 
					);
					vs.Add(m_astrFuncInfos[i].strName);
					m_astrFuncInfos[i].strName.Empty();
					m_iLastAccessId = i + 1;
				}
			}
			return vs;
		}
	private:
		/// 関数情報
		struct TFuncInfo
		{
			DWORD dwTick;	///< 発動時間(開始までのmsではなく、開始TickCount値)
			CAscii strName;	///< 開始関数名
		};
		CVectorT<TFuncInfo> m_astrFuncInfos;	///< 関数情報
		size_t					m_iLastAccessId;	///< 最終アクセスID
	};

	/**
	 * TinyMacroLang ネスト情報管理.
	 *	 { } のネストを処理管理。ネスティングをサポート。
	 */
	class CNestInfo
	{
	public:
		/// ネスト種類
		enum EKind
		{
			EK_Non = 0,		///< なんでもないネスト
			EK_IfEqu,		///< if文(真)によるネスト
			EK_IfNeq,		///< if文(偽)によるネスト
			EK_Switch,		///< switchのネスト
			EK_While,		///< whileのネスト
		};

		/// コンストラクタ
		CNestInfo(void)
		{
			m_iDepth = 0;
			m_aKind.Add(EK_Non/*何でもないネス*/);
			TTRACE1("ネストレベル=0 NowLvl=%d\n", m_iDepth);
		}
		/** 今、トップ？ */
		bool IsTopDepth()
		{
			return m_iDepth == 0;
		}
		/** 現在の種類 */
		EKind GetNowKind(void)
		{
			if ( ToInt(m_aKind.GetSize()) <= m_iDepth )
			{
				return EK_Non;
			}
			return m_aKind.Ref(m_iDepth);
		}
		/** 一つ前の種類 */
		EKind GetLastKind(void)
		{
			if ( ToInt(m_aKind.GetSize()) <= (m_iDepth + 1) )
			{
				return EK_Non;
			}
			return m_aKind.Ref(m_iDepth + 1);
		}
		/** ネストをひとつ深くする */
		void Inc(EKind eKind)
		{
			m_iDepth++;
			m_aKind.SetSize(m_iDepth);
			m_aKind.Add(eKind);
			TTRACE1("ネストレベル++ NowLvl=%d\n", m_iDepth);
		}
		/** ネストをひとつ深くする(While専用) */
		void IncWhile(const CTmlGrammar& gram)
		{
			Inc(EK_While);
			TTRACE0("  →whileネストです\n");
			m_aWhileStartInfo.Insert(0, gram);
		}
		/** 現在whileなら情報を返す */
		CTmlGrammar GetWhileInfo(void)
		{
			if ( GetNowKind() == EK_While )
			{
				return m_aWhileStartInfo[0];
			}
			return CTmlGrammar();
		}
		/** ネストをひとつ浅くする */
		void Dec(void)
		{
			if ( m_iDepth > 0 )
			{
				if ( GetNowKind() == EK_While )
				{
					m_aWhileStartInfo.Remove(0);
				}
				m_iDepth--;
				//参照の可能性があるので、消さない
				TTRACE1("ネストレベル-- NowLvl=%d\n", m_iDepth);
			}
		}
	private:
		int						m_iDepth;			///< ネスト深さ
		CVectorT<EKind>			m_aKind;			///< ネスト深さの理由
		CVectorT<CTmlGrammar>	m_aWhileStartInfo;	///< while開始情報
	};

	CTmlDocuments			m_documents;		///< ドキュメント管理
	CTmlVariableManager		m_variable;			///< 変数管理
	CTmlCalculator				m_calc;				///< 処理用の計算クラス
	CInterruptFunc			m_cInterruptInfo;	///< 割り込み
	IListener*				m_piListener;		///< リスナー用インターフェイス
	ITmlExpander*				m_piExpander;		///< 拡張関数のインターフェイス
	ELogLevel				m_eLogLevel;		///< 現在のLOGの表示レベル
	CAscii					m_strLogLastName;	///< ラストLOG表示ネーム
	INT_PTR					m_iLogLastLine;		///< ラストLOG表示ライン

	/**
	 * 割り込みチェック.
	 *		ユーザからの割り込み(停止)と割り込み指定の処理を行う
	 */
	CTmlResult m_CheckInterrupt(void)
	{
		//リスナーチェック
		if ( m_piListener != NULL )
		{
			if ( ! m_piListener->IsExecutionContinued() )
			{
				m_MessageBug("中断します。");
				return Result_UserInterruption;
			}
		}
		//割込みチェック
		CAsciiVector astrFuncs = m_cInterruptInfo.Check();
		if ( astrFuncs.GetSize()>0 )
		{
			//割込みあり
			CTmlValuesVector tva;
			CTmlValue tvObject;
			loop ( i, astrFuncs.GetSize() )
			{
				TTRACE1( "時間が来たので [%s]を実行\n", astrFuncs[i] );
				CTmlResult res = ExecFunction(tvObject, astrFuncs[i], tva);
				if ( res.IsError() )
				{
					return res;
				}
			}
		}
		return Result_Success;
	}

	/**
	 * エラーメッセージ通知.
	 *		エラーが発生した時、原因を文字列で通知するために使います。
	 *		リザルト内容によっては出力しません。
	 *	@param lpszMessage エラー内容
	 *	@param ... パラメータ
	 */
	void m_MessageBug(LPCSTR lpszMessage, ...)
	{
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageBug(m_strLogLastName, m_iLogLastLine, str);
	}

	/**
	 * エラーメッセージ通知.
	 *		エラーが発生した時、原因を文字列で通知するために使います。
	 *		リザルト内容によっては出力しません。
	 *	@param res 結果。
	 *	@param lpszMessage エラー内容
	 *	@param ... パラメータ
	 */
	void m_MessageBug(const CTmlResult& res, LPCSTR lpszMessage, ...)
	{
		if ( res.GetResult() == Result_UserInterruption || res.GetResult() == Result_ProgramableInterruption )
		{
			return;
		}
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageBug(res.GetFileName(), res.GetFileLine(), str);
	}

	/**
	 * エラーメッセージ通知.
	 *		エラーが発生した時、原因を文字列で通知するために使います。
	 *		リザルト内容によっては出力しません。
	 *	@param lpszName 発生ファイル名。
	 *	@param iLine 発生行。
	 *	@param lpszMessage エラー内容
	 *	@param ... パラメータ
	 */
	void m_MessageBug(LPCSTR lpszName, INT_PTR iLine, LPCSTR lpszMessage)
	{
		CAscii str;
		if ( iLine > 0 )
		{
			str.Format("%s(%d): Error:%s", lpszName, iLine, lpszMessage);
		}
		else
		{
			str.Format("%s", lpszMessage);
		}
		m_strLogLastName = lpszName;
		m_iLogLastLine = iLine;
		#ifdef _DEBUG
			::OutputDebugStringA(str + "\n");
		#endif
		if ( m_piListener != NULL )
		{
			m_piListener->OutputLog(CStr(str), true);
		}
	}

	/**
	 * ログメッセージ通知
	 *		処理の流れを通知するために使います。マクロのデバグ用に使用します。
	 *		ログレベルによっては出力しません。
	 *	@param elLevel ログレベル。
	 *	@param lpszMessage エラー内容
	 *	@param ... パラメータ
	 */
	void m_MessageLog(ELogLevel elLevel, LPCSTR lpszMessage, ...)
	{
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageLog("", 0, elLevel, str);
	}

	/**
	 * ログメッセージ通知.
	 *		処理の流れを通知するために使います。マクロのデバグ用に使用します。
	 *		ログレベルによっては出力しません。
	 *	@param gram 発生位置。
	 *	@param elLevel ログレベル。
	 *	@param lpszMessage エラー内容
	 *	@param ... パラメータ
	 */
	void m_MessageLog(const CTmlGrammar& gram, ELogLevel elLevel, LPCSTR lpszMessage , ...)
	{
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageLog(CAscii(gram.GetName()), gram.GetLine(), elLevel, str);
	}

	/**
	 * ログメッセージ通知
	 *		処理の流れを通知するために使います。マクロのデバグ用に使用します。
	 *		ログレベルによっては出力しません。
	 *	@param lpszName 発生ファイル名。
	 *	@param iLine 発生行。
	 *	@param elLevel ログレベル。
	 *	@param lpszMessage エラー内容
	 *	@param ... パラメータ
	 */
	void m_MessageLog(LPCSTR lpszName, INT_PTR iLine, ELogLevel elLevel, LPCSTR lpszMessage)
	{
		#ifndef _DEBUG
			if ( m_eLogLevel<elLevel )
			{
				return;
			}
		#endif
		CAscii strLevel;
		switch(elLevel)
		{
		case ELL_DETAILS://詳細
			strLevel = "Info.";
			break;
		case ELL_BRANCH://分岐
			strLevel = "Branch";
			break;
		case ELL_FUNC:	//関数
			strLevel = "Func.";
			break;
		case ELL_ABSOLUTE://絶対
		default:
			strLevel = "Info.";
			break;
		}
		CAscii str;
		if ( iLine > 0 )
		{
			str.Format("%s(%d): %s:%s", lpszName, iLine, strLevel, lpszMessage);
		}
		else
		{
			str.Format(" %s:%s", strLevel, lpszMessage);
		}
		#ifdef _DEBUG
			::OutputDebugStringA(str + "\n");
			if ( m_eLogLevel < elLevel )
			{
				return;
			}
		#endif
		if ( m_piListener != NULL )
		{
			m_piListener->OutputLog(CStr(str), false);
		}
	}

	/**
	 * 変数一覧、内容表示.
	 *	変数の一覧を通知するために使います。マクロのデバグ用に使用します。
	 */
	void m_DumpVarDim(void)
	{
		CTmlVariableManager::CVaVaVectors& aaVava = m_variable.ReferVarArrays();
		CTmlVariableManager::CArVaVectors& aaDmva = m_variable.ReferArrayArrays();
		//
		INT_PTR iDim1 = aaVava.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			m_MessageLog(ELL_ABSOLUTE, "---変数内容(lvl=%d)", i);
			CTmlVariableManager::CVaVaVector& aa = aaVava.Ref(i);
			INT_PTR iDim2 = aa.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				CTmlVariableManager::TVarValue &T = aa.Ref(j);
				if ( T.value.HasData() )
				{
					//Data型
					m_MessageLog(ELL_ABSOLUTE, "%s = [%s](len=%d)",
								T.name,
								T.value.GetString(),
								T.value.GetInteger()
					);
				}
				else
				{
					//数値型
					m_MessageLog(ELL_ABSOLUTE, "%s = %d", T.name, T.value.GetInteger());
				}
			}
			CTmlVariableManager::CArVaVector& va = aaDmva.Ref(i);
			iDim2 = va.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				CTmlVariableManager::TArrayValue &T = va.Ref(j);
				INT_PTR iDim3 = T.values.GetSize();
				for( INT_PTR k = 0; k < iDim3; k++ )
				{
					if ( T.values[k].HasData() )
					{
						//Data型
						m_MessageLog(ELL_ABSOLUTE, "%s(%d) = [%s](len=%d)",
									T.name,
									k,
									T.values[k].GetString(),
									T.values[k].GetInteger()
						);
					}
					else
					{
						//数値型
						m_MessageLog(ELL_ABSOLUTE,"%s(%d) = %d",
									T.name, k, T.values[k].GetInteger());
					}
				}
			}
		}
	}

	/** 
	 * [処理] 拡張変数、関数の処理.
	 *	@note ITmlExpander の実装。
	 *	@param[out] _answer 処理した時の値
	 *	@param[in] lpszToken 発見したトークン(トークンだけでNULL終端)
	 *	@param[in] aValues 発見したトークンの引数、および内容。
	 *	@param[in] boIsTestMode trueならテストモード（文法チェックのみ行う）。
	 *	@return 結果。
	 */
	CTmlResult ExpandVariable(CTmlValueCarrier& _answer, LPCSTR lpszToken, const CTmlValuesVector &aValues, bool boIsTestMode)
	{
		if ( lpszToken == NULL )
		{
			//テスト用のLOG出力
			ASSERT( boIsTestMode );
			m_MessageBug(_answer().GetString());
			return Result_Success;
		}
		//本クラスが覚えてる変数をチェック
		CTmlValue* V = m_variable.FindValue(lpszToken);
		if ( V != NULL )
		{
			_answer.SetRef(*V);
			if ( aValues.GetSize() > 0 )
			{
				//パラメータがある
				return CTmlResult(Operation_CheckFunc, Result_UnnecessaryParam);
			}
			#ifdef _DEBUG
				_answer.Set(lpszToken);
			#endif
			return Result_Success;
		}
		//
		//本クラスが覚えてる配列をチェック
		CTmlValuesVector* A = m_variable.FindArray(lpszToken);
		if ( A != NULL )
		{
			if ( aValues.GetSize()==0 )
			{
				_answer = ToInt(A->GetSize());
				return Result_Success;
			}
			else if ( aValues.GetSize() == 1 )
			{
				int i = aValues[0].GetInteger();
				if ( i < 0 || i >= ToInt(A->GetSize()) )
				{
					return CTmlResult(Operation_CheckFunc, Result_OutOfRange);
				}
				_answer.SetRef(A->Ref(i));
				#ifdef _DEBUG
					CAscii s;
					s.Format("%s(%d)", lpszToken, i);
					_answer.Set(s);
				#endif
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
		//
		if ( m_piExpander != NULL )
		{
			CTmlResult r = m_piExpander->ExpandVariable(_answer, lpszToken, aValues, boIsTestMode);
			if ( ! r.IsNop() )
			{
				return r;
			}
		}
		//本クラスが知っている関数をチェック
		CTmlValue value;
		CTmlResult res = ExecFunction(value, lpszToken, aValues);
		if ( res.IsError() )
		{
			return res;
		}
		else if ( res.IsSuccess() )
		{
			_answer = value;
			return res;
		}
		//
		if ( STRLIB::Compare(lpszToken, "pause") == 0 )
		{
			//汎用グローバル関数 「指定時間停止」
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 1 )
			{
				if ( boIsTestMode ) { return Result_Success; }
				//
				int i = aValues[0].GetInteger();
				//	::Sleep( i );
				CTmlResult r;
				DWORD dwStart;
				DWORD dwNow;
				if ( i < 500 )
				{
					dwStart = ::GetTickCount();
					while ( true )
					{
						dwNow = ::GetTickCount();
						DWORD dw = i;
						if ( (dwNow - dwStart) >= dw ) { break; }
						r = m_CheckInterrupt();
						if ( ! r.IsSuccess() ) { return r; }
					}
				}
				else
				{
					dwStart = ::GetTickCount();
					while ( true )
					{
						dwNow = ::GetTickCount();
						DWORD dw = i;
						if ( (dwNow - dwStart) >= dw ) { break; }
						r = m_CheckInterrupt();
						if ( ! r.IsSuccess() ){ return r; }
						::Sleep(10);
					}
				}
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
		else if ( STRLIB::Compare(lpszToken, "setinterrupt") == 0 )
		{
			//汎用グローバル関数 「割り込み設定」
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 2 )
			{
				if ( boIsTestMode ){ return Result_Success; }
				//
				CAscii strFuncName = aValues[0].GetString();
				//
				CTmlDocuments::TFuncInfoRes tFunc;
				tFunc = m_documents.FindFunc(strFuncName);
				if ( ! tFunc.HasData() )
				{
					return CTmlResult(Operation_CheckFunc, Result_NotFoundFunction);
				}
				_answer = m_cInterruptInfo.Set(strFuncName, aValues[1].GetInteger());
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 2);

		}
		else if ( STRLIB::Compare(lpszToken, "killinterrupt") == 0 )
		{
			//汎用グローバル関数 「割り込み停止」
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 0 )
			{
				if ( boIsTestMode ) { return Result_Success; }
				m_cInterruptInfo.Empty();
				return Result_Success;
			}
			else if ( aValues.GetSize() == 1 )
			{
				if ( boIsTestMode ){ return Result_Success; }
				m_cInterruptInfo.Kill(aValues[0].GetInteger());
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
#if 0
		else if ( STRLIB::Compare((lpszToken, "playSound" ) == 0 || STRLIB::Compare(lpszToken, "loopSound") == 0 )
		{
			//汎用グローバル関数 「音」
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 1 )
			{
				if ( boIsTestMode ) { return ETR_Success; }
				CCurrentDirMan cDirMan;
				cDirMan.SetDirOfFile( m_cText.GetFileName() );
				//
				CString strWaveName = aValues[0].GetString();
				DWORD dwSound = SND_FILENAME | SND_NODEFAULT | SND_ASYNC;
				if ( lpszToken[0]=='l' )
				{
					dwSound |= SND_LOOP;
				}
				::PlaySound(strWaveName, NULL, dwSound);
				//
				return 1;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
		else if ( _tcscmp( lpszToken,"stopsound" )==0 )
		{
			//汎用グローバル関数 「音」
			TTRACE1("found System Func [%s]\n", lpszToken);
			if ( aValues.GetSize()==0 )
			{
				if ( boIsTestMode ){return ETR_Success;}
				//
				::PlaySound(NULL, NULL, 0);
				return 1;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 0);

		}
#endif
		//無ければ
		return Result_Nop;
	}

	/** 
	 * [処理] 変更(代入)コールバック。
	 *	@note IExpander の実装。
	 *	@param lpszName 代入された名前
	 *	@param value 代入された値
	 *	@return 結果。
	 */
	CTmlResult OnChangeValue(LPCSTR lpszName, const CTmlValue& value)
	{
		//無ければ
		if ( m_piExpander != NULL )
		{
			return m_piExpander->OnChangeValue(lpszName, value);
		}
		return Result_Nop;
	}

	/// ステートメント結果タイプ
	enum EStatementResultType
	{
		ESRT_Normal,	///< ノーマル
		ESRT_Return,	///< リターン
		ESRT_Function,	///< ファンクション
	};

	/**
	 * ステートメント検索＆実行
	 *	@param[in,out] _gram	分法
	 *	@param[in,out] _nest	｛、｝のネストカウンタ
	 *	@param[out] _answer	答えの値。
	 *	@param[out] _stateResType	ステートメント結果
	 *	@return 結果。
	 */
	CTmlResult m_SearchAndExecCommand(CTmlGrammar& _gram, CNestInfo& _nest, CTmlValue& _answer, EStatementResultType& _stateResType)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar backGram = _gram;
		CTmlGrammar::CParts parts;
		//
		bool boIsTestMode = m_calc.IsTestMode();
		//
		_stateResType = ESRT_Normal;
		//
		CTmlResult res;
		//
		parts = _gram.GetNextParts();
		if ( parts.GetKind() != CTmlGrammar::TOKEN )
		{
			/*トークンがない！！*/
			_gram = backGram;
			return Result_Nop;
		}
		CAscii token = parts.GetString();
		if ( token.IsEqual("func") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			_stateResType = ESRT_Function/*func*/;
		}
		else if ( token.IsEqual("return") || token.IsEqual("end") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			parts = _gram.PeekNextParts();
			if ( strchr(";{}/", parts.GetString()[0]) != NULL )
			{
				;
			}
			else
			{
				CTmlValue v;
				res = m_calc.TextToValue(v, _gram);
				if ( res.IsError() )
				{
					res.SetScene((token[0] == 'r') ? Scene_Return : Scene_End);
					m_MessageBug(res, "%s 文の引数式でエラーが発生しました。", token);
					return res;
				}
				_answer = v;
			}
			//
			if ( ! boIsTestMode )
			{
				//本番モード
				if ( token[0] == 'r' )
				{
					//return
					_stateResType = ESRT_Return/*return*/;
				}
				else
				{
					//end
					return CTmlResult(Scene_End, Operation_Non, Result_ProgramableInterruption);
				}
			}
			else
			{
				//テストモード
				_answer = 1;
			}
		}
		else if ( token.IsEqual("val") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			parts = _gram.GetNextParts();
			if ( parts.GetKind() != CTmlGrammar::TOKEN )
			{
				res = CTmlResult(Scene_Val, Operation_Non, Result_NotName).SetLine(backGram);
				m_MessageBug(res, "valの後に、変数名指定がありません。");
				return res;
			}
			CAscii valName = parts.GetString();
			CTmlValue firstValue;
			parts = _gram.PeekNextParts();
			if ( CAscii(parts.GetString()) == "=" )
			{
				_gram.GetNextParts();
				res = m_calc.TextToValue(firstValue, _gram);
				if ( res.IsError() )
				{
					res.SetScene(Scene_Val);
					res.SetLine(backGram);
					m_MessageBug(res, "valの変数初期化の式でエラーが発生しました。");
					return res;
				}
			} 
			else 
			{
				firstValue = 0;
			}
			//
			CTmlValueCarrier a;
			if ( m_variable.HasNameInNowLevel(valName) )
			{
				//同じレベルに同じ名前がある。
				res = CTmlResult(Scene_Val, Operation_Non, Result_AlreadyExistName);
				res.SetErrorString(valName);
				if ( ! m_calc.IsTestMode() )
				{
					return res;
				}
				//LOGへ
				res.SetLine(_gram);
				m_MessageBug(res.ToString());
			}
			else
			{
				if ( ! m_variable.AddValue(valName, firstValue) )
				{
					res = CTmlResult(Scene_Val, Operation_Non, Result_UnknownError).SetLine(backGram);
					m_MessageBug(res, "valの変数登録でなぞのエラーが発生しました。");
					return res;
				}
				m_MessageLog(backGram, ELL_DETAILS, "変数宣言 `%s' = [%s](%d)", valName, firstValue.GetString(), firstValue.GetInteger());
			}
		}
		else if ( token.IsEqual("array") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			parts = _gram.GetNextParts();
			if ( parts.GetKind() != CTmlGrammar::TOKEN )
			{
				res = CTmlResult(Scene_Array, Operation_Non, Result_NotName).SetLine(backGram);
				m_MessageBug(res, "arrayの後に、変数名指定がありません。");
				return res;
			}
			CAscii arrayName = parts.GetString();
			parts = _gram.PeekNextParts();
			if ( ! parts.IsEqualString("(") )
			{
				res = CTmlResult(Scene_Array, Operation_Non, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "arrayの配列名の後に括弧がありません。");
				return res;
			}
			else
			{
				CTmlValue sizeValue;
				res = m_calc.TextToValue(sizeValue, _gram);
				if ( res.IsError() )
				{
					res.SetScene(Scene_Array);
					res.SetLine(backGram);
					m_MessageBug(res, "arrayの配列サイズの式でエラーが発生しました。");
					return res;
				}
				CTmlValueCarrier a;
				if ( m_variable.HasNameInNowLevel(arrayName) )
				{
					//同じレベルに同じ名前がある。
					res = CTmlResult(Scene_Array, Operation_Non, Result_AlreadyExistName);
					res.SetErrorString(arrayName);
					if ( m_calc.IsTestMode() )
					{
						//LOGへ
//						res.SetLine(CAscii(_gram.GetName()), _gram.GetLine());
						res.SetLine(_gram);
						m_MessageBug(res.ToString());
					}
					else
					{
						return res;
					}
				}
				else
				{
					if ( ! m_variable.AddArray(arrayName, sizeValue.GetInteger()) )
					{
						res = CTmlResult(Scene_Array, Operation_Non, Result_OutOfRange).SetLine(backGram);
						m_MessageBug(res, "arrayの配列サイズが異常値です。");
						return res;
					}
					m_MessageLog(backGram, ELL_DETAILS, "配列宣言 `%s(%d)", arrayName, sizeValue.GetInteger());
				}
			}
		}
		else if ( token.IsEqual("if") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			CTmlValue boolValue;
			res = m_calc.TextToValue(boolValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_If);
				res.SetLine(backGram);
				m_MessageBug(res, "if文の条件式でエラーが発生しました。");
				return res;
			}
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_If, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "if文に`{'がありません。");
				return res;
			}
			//
			if ( ! boIsTestMode )
			{
				//本番モード
				if ( boolValue.GetInteger() != 0 )
				{
					m_MessageLog(ELL_BRANCH, "└条件に合いましたので以下を実行します");
					_nest.Inc(CNestInfo::EK_IfEqu);
					m_variable.IncLevel();
					m_documents.IncLevel();
				}
				else
				{
					m_MessageLog(ELL_BRANCH, "└条件に合いませんでした");
					_nest.Inc( CNestInfo::EK_IfNeq );
					_nest.Dec();
					_gram.SkipoutBlock();	//`}'の次に
				}
			}
			else
			{
				//テストモード
				m_MessageLog(ELL_BRANCH, "└テストモードなので以下を実行します");
				_nest.Inc(CNestInfo::EK_IfNeq);
				m_variable.IncLevel();
				m_documents.IncLevel();
			}
		} 
		else if ( token.IsEqual("else") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			parts = _gram.GetNextParts();
			if ( parts.IsEqualString("if") )
			{
				m_MessageLog(ELL_DETAILS, "else文に続いてif文がありました。");
				switch( _nest.GetLastKind() )
				{
				case CNestInfo::EK_IfEqu:
					m_MessageLog(ELL_BRANCH, "└条件に合いましたのでelse文を実行しません");
					{
						//if文の{までスキップ
						CTmlValue boolValue;
						res = m_calc.TextToValue(boolValue, _gram);
						if ( res.IsError() )
						{
							res.SetScene(Scene_If);
							m_MessageBug(res, "if文の条件式でエラーが発生しました。");
							return res;
						}
						parts = _gram.GetNextParts();
						if ( ! parts.IsEqualString("{") )
						{
							res = CTmlResult(Scene_If, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
							m_MessageBug(res, "if文に`{'がありません。");
							return res;
						}
						_gram.SkipoutBlock();	//`}'の次に
					}
					break;
				case CNestInfo::EK_IfNeq:
					m_MessageLog(ELL_BRANCH, "└条件に合わないのでelse の後の if文を実行します");
					break;
				default:
					res = CTmlResult(Scene_Else, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
					m_MessageBug(res, "if文がないのにelse文があります。");
					return res;
				}
			}
			else if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_Else, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "else文に`{'がありません。");
				return res;
			}
			else
			{
				switch( _nest.GetLastKind() )
				{
				case CNestInfo::EK_IfEqu:
					m_MessageLog(ELL_BRANCH, "└条件に合いましたのでelse文を実行しません");
					_gram.SkipoutBlock();	//`}'の次に
					break;
				case CNestInfo::EK_IfNeq:
					m_MessageLog(ELL_BRANCH,"└条件に合わないのでelse文を実行します");
					_nest.Inc(CNestInfo::EK_Non);
					m_variable.IncLevel();
					m_documents.IncLevel();
					break;
				default:
					res = CTmlResult(Scene_Else, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
					m_MessageBug(res, "if文がないのにelse文があります。");
					return res;
				}
			}
		}
		else if ( token.IsEqual("case") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			CTmlValue dummyValue;
			res = m_calc.TextToValue(dummyValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_Case);
				m_MessageBug(res, "case文の式でエラーが発生しました。");
				return res;
			}
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString(":") )
			{
				res = CTmlResult(Scene_Case, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "case文に`:'がありません。");
				return res;
			}
		} 
		else if ( token.IsEqual("default") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString(":") )
			{
				res = CTmlResult(Scene_Default, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "default文に`:'がありません。");
				return res;
			}
		}
		else if ( token.IsEqual("switch") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			CTmlValue caseValue;
			res = m_calc.TextToValue(caseValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_Switch);
				m_MessageBug(res, "switch文の条件式でエラーが発生しました。");
				return res;
			}
			m_MessageLog(ELL_DETAILS, "switch文の引数は[%s](%d)です。", caseValue.GetString(), caseValue.GetInteger());
			//
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_Switch, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "switch文に`{'がありません。");
				return res;
			}
			//switch()直後の位置を記憶
			CTmlGrammar backGram = _gram;
			//defaultの位置
			CTmlGrammar defaultGram;
			//-- case or default までスキップ
			bool boMatchCase = false;
			while ( true )
			{
				parts = _gram.GetNextParts();
				CAscii token = parts.GetString();
				if ( parts.GetKind() == CTmlGrammar::OPEN_CHAR )
				{
					//ブロック開始文字
					_gram.SkipoutBlock();
					continue;
				}
				else if ( token.IsEqual("}") )
				{
					break;
				}
				switch ( parts.GetKind() )
				{
				case CTmlGrammar::CLOSE_CHAR:		// ブロック終端文字
				case CTmlGrammar::FINAL:			// 終わり
				case CTmlGrammar::ERROR_STRING:	// 文字列エラー
				case CTmlGrammar::ERROR_DEPTH:		// 深さエラー
					res = CTmlResult(Scene_Switch, Operation_CheckStatement, Result_NotCloseParentheses).SetLine(backGram);
					m_MessageBug(res, "`{'が綴じてません。");
					return res;
				}
				if ( token.IsEqual("default") )
				{
					m_MessageLog(_gram, ELL_DETAILS, "`default文'を確認");
					parts = _gram.GetNextParts();
					if ( ! parts.IsEqualString(":") )
					{
						res = CTmlResult(Scene_Switch, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
						m_MessageBug(res, "switch文に`:'がありません。");
						return res;
					}
					defaultGram = _gram;	//記憶！
					continue;
				}
				else if ( token.IsEqual("case") )
				{
					m_MessageLog(_gram, ELL_DETAILS, "`case文'を確認");
					//
					CTmlValue v;
					res = m_calc.TextToValue(v, _gram);
					if ( res.IsError() )
					{
						res.SetScene(Scene_Case);
						m_MessageBug(res, "case文の式でエラーが発生しました。");
						return res;
					}
					parts = _gram.GetNextParts();
					if ( ! parts.IsEqualString(":") )
					{
						res = CTmlResult(Scene_Case, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
						m_MessageBug(res, "case文に`:'がありません。");
						return res;
					}
					if ( ! boIsTestMode && caseValue.Compare(m_calc.ReferElementWildResult(), v) )
					{
						//本番モード & 一致
						m_MessageLog(ELL_BRANCH, "一致するcase文がありました");
						boMatchCase = true;
						break;
					}
				}
			}
			//
			if ( ! boIsTestMode )
			{
				//本番モード
				if ( ! boMatchCase )
				{
					if ( defaultGram.GetPointer() != NULL )
					{
						//defaultを使う
						m_MessageLog(ELL_BRANCH, "一致するcase文がないのでdefault文以下を実行します");
						_gram = defaultGram;
						boMatchCase = true;
					}
					else
					{
						m_MessageLog(ELL_BRANCH,"switch構文内に一致する`case'`default'文がありませんでした");
						_gram.SkipoutBlock();	//`}'の次に
					}
				}
				if ( boMatchCase )
				{
					_nest.Inc(CNestInfo::EK_Switch);
					m_variable.IncLevel();
					m_documents.IncLevel();
				}
			}
			else
			{
				//テストモード
				_gram = backGram;
				_nest.Inc( CNestInfo::EK_Switch );
				m_variable.IncLevel();
				m_documents.IncLevel();
			}
		}
		else if ( token.IsEqual("while") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			CTmlValue boolValue;
			res = m_calc.TextToValue(boolValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_While);
				m_MessageBug(res, "while文の条件式でエラーが発生しました。");
				return res;
			}
			//
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_While, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "while文に`{'がありません。");
				return res;
			}
			if ( ! boIsTestMode )
			{
				//本番モード
				if ( boolValue.GetInteger() != 0 )
				{
					m_MessageLog(ELL_BRANCH, "└条件に合いましたので以下を実行します");
					_nest.IncWhile(_gram);
					m_variable.IncLevel();
					m_documents.IncLevel();
				}
				else
				{
					m_MessageLog(ELL_BRANCH, "└条件に合いませんでした");
					_gram.SkipoutBlock();	//`}'の次に
				}
			}
			else
			{
				//テストモード
				_nest.Inc(CNestInfo::EK_Switch);	//テストでは、switchとしておく
				m_variable.IncLevel();
				m_documents.IncLevel();
			}
		}
		else if ( token.IsEqual("break") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			if ( _nest.IsTopDepth() )
			{
				res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
				m_MessageBug(res, "`switch'か`while'文がないのに`break'文があります。");
				return res;
			}
			//
			CNestInfo cNestTmp = _nest;
			//
			CTmlGrammar bg = _gram;
			while ( true )
			{
				if ( cNestTmp.GetNowKind() == CNestInfo::EK_Switch || cNestTmp.GetNowKind() == CNestInfo::EK_While )
				{
					if ( ! _gram.SkipoutBlock() )
					{
						_gram = bg;
						res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_NotCloseParentheses).SetLine(backGram);
						m_MessageBug(res, "'{'が閉じていません。");
						return res;
					}
					cNestTmp.Dec();
					if ( ! boIsTestMode )
					{
						//本番モード
						_nest = cNestTmp;
						m_variable.DecLevel();
						m_documents.DecLevel();
					}
					else
					{
						//テストモード
						_gram = bg;
					}
					break;
				}
				else
				{
					if ( ! _gram.SkipoutBlock() )
					{
						_gram = bg;
						res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_NotCloseParentheses).SetLine(backGram);
						m_MessageBug(res, "'{'が閉じていません。");
						return res;
					}
					cNestTmp.Dec();
					if ( ! boIsTestMode )
					{
						//本番モード
						m_variable.DecLevel();
						m_documents.DecLevel();
					}
					if ( cNestTmp.IsTopDepth() )
					{
						_gram = bg;
						res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
						m_MessageBug(res, "`switch'か`while'文がないのに`break'文があります。");
						return res;
					}
				}
			}
		}
		else if ( token.IsEqual("include") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//
			CTmlValue v;
			res = m_calc.TextToValue(v, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_Include);
				m_MessageBug(res, "include文の条件式でエラーが発生しました。");
				return res;
			}
			//現在の実行行のPath
			CStr str = CFilePathManager::GetPathName(CStr(_gram.GetName()));
			res = AddMacroFile(CStr(v.GetString()), str);
			if ( ! res.IsSuccess() )
			{
				res.SetScene(Scene_Include);
				m_MessageBug(res, "include文の条件式でエラーが発生しました。");
				return res;
			}
			_answer = v;
		}
		else if ( token.IsEqual("vardump") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'文を確認", token);
			//**開発用です
			if ( ! boIsTestMode )
			{
				//本番モード
				m_DumpVarDim();
			}
		}
		else
		{
			//しない
			_gram = backGram;
			return Result_Nop;
		}
		//
//		cBackupLine.Destroy();
		return Result_Success;
	}

	friend class CTmlMacroTest;
};



}; // TNB
