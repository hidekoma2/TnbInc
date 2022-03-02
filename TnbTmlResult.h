#pragma once
/**
 *@file
 * TinyMacroLang 結果関係のヘッダ
 *
 *		TMLの結果管理のクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbStaticMap.h"
#include "TnbGrammarAnalyzer.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang 文章解析
 *
 *	@par必要ファイル
 *			TnbTmlResult.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
typedef CGrammarAnalyzerT<char> CTmlGrammar;



/**@ingroup TML
 * TinyMacroLang コード管理
 *
 *	@par必要ファイル
 *			TnbTmlResult.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
class CTml
{
public:

	/// シーンコード
	enum EScene
	{
		Scene_Non,			///< 未定義
		Scene_Val,			///< val文処理中
		Scene_Array,		///< array文処理中
		Scene_Func,			///< func文処理中
		Scene_If,			///< if文処理中
		Scene_Else,			///< else文処理中
		Scene_Switch,		///< switch文処理中
		Scene_Case,			///< case文処理中
		Scene_Default,		///< default文処理中
		Scene_While,		///< while文処理中
		Scene_Break,		///< break文処理中
		Scene_Return,		///< return文処理中
		Scene_Include,		///< include文処理中
		Scene_End,			///< end文処理中
	};

	/// オペレーションコード
	enum EOperation
	{
		Operation_Non,				///< 未定義
		Operation_Text,				///< 文字列処理中
		Operation_Text_Esc,			///< 文字列 ￥? 処理中
		Operation_Text_EscHex,		///< 文字列 ￥x 処理中
	//	Operation_Text_EscInsert,	///< 文字列 ￥i() 処理中
		Operation_Calc,				///< 計算処理中
		Operation_CheckFunc,		///< 関数チェック中
		Operation_CheckStatement,	///< ステートメントチェック中
		Operation_FindingFunc,		///< 関数検索中
		Operation_FileAccess,		///< Fileアクセス中
	};

	/// リザルトコード
	enum EResult
	{
		Result_Success = 1,					///< 成功
		Result_Nop = 0,						///< 処理なし
		Result_UnknownError = -10000,		///< その他のエラー
		Result_InvalidEscSeq,				///< エスケープシーケンスが異常
		Result_InvalidHexNotation,			///< HEX表記が異常
		Result_NotFoundParentheses,			///< 括弧が必要なところにない
		Result_NotCloseParentheses,			///< 括弧が閉じていない
		Result_InvalidParenthesesPosition,	///< 括弧の位置がおかしい
		Result_UnfinishedExpression,		///< 計算式が途中で終わっている
		Result_ZeroDivide,					///< ０で割ろうとしてます
		Result_UnknownKeyword,				///< 知らないキーワード
		Result_InvalidWordPosition,			///< 単語位置が異常です（そこには存在しないはず）
		Result_NotName,						///< 名前がありません
		Result_AlreadyExistName,			///< 既にある名前です
		Result_UnnecessaryParam,			///< 引数は不必要です
		Result_ManyParam,					///< 引数が多すぎます
		Result_NotenoughParam,				///< 引数が足りません
		Result_OutOfRange,					///< 範囲外
		Result_InvalidMacroContentsLevel,	///< マクロコンテンツレベルが異常です
		Result_OpenFailure,					///< Open失敗
		Result_ReadFailure,					///< Read失敗
		Result_SendFailure,					///< Send失敗
		Result_InvalidState,				///< 状態エラー（今、実行できません)
		Result_NotFoundFunction,			///< 必要な関数がありません
		Result_UserInterruption,			///< ユーザによる中断
		Result_ProgramableInterruption,		///< プログラムによる中断
	};
};



/**@ingroup TML
 * TinyMacroLang 結果状態管理
 *
 * 結果を管理するクラス
 *		マイナスは、エラーコード。プラスは任意の値を保管できます。
 *		オペレータint があるので、通常はint としてアクセスできます。
 *
 *	@par必要ファイル
 *			TnbTmlResult.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
class CTmlResult : public CTml
{
public:
	
	/**
	 * コンストラクタ.
	 *		Success を持ちます。
	 */
	CTmlResult(void) : m_eResult(Result_Success), m_eScene(Scene_Non), m_eOperation(Operation_Non), m_iFileLine(0)
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CTmlResult(const CTmlResult& other) 
		: m_eResult(other.m_eResult), m_eScene(other.m_eScene), m_eOperation(other.m_eOperation), m_iFileLine(other.m_iFileLine)
		, m_strFuncName(other.m_strFuncName), m_strToken(other.m_strToken), m_strFileName(other.m_strFileName)
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		オペレーション＆結果代入。シーンコード」「エラーソース位置」「エラー内容」「発生関数名」は無しです。
	 *	@param eOperation	オペレーションコード
	 *	@param eResult		リザルトコード
	 */
	CTmlResult(EOperation eOperation, EResult eResult)
		: m_eResult(eResult), m_eScene(Scene_Non), m_eOperation(eOperation), m_iFileLine(0)
	{
	}

	/** 
	 * 代入コンストラクタ.
	 *		シーン＆オペレーション＆結果代入。「エラーソース位置」「エラー内容」「発生関数名」は無しです。
	 *	@param eScene		シーンコード
	 *	@param eOperation	オペレーションコード
	 *	@param eResult		リザルトコード
	 */
	CTmlResult(EScene eScene, EOperation eOperation, EResult eResult)
		: m_eResult(eResult), m_eScene(eScene), m_eOperation(eOperation), m_iFileLine(0)
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		結果代入。「オペレーションコード」「シーンコード」
	 *		「エラーソース位置」「エラー内容」「発生関数名」は無しです。
	 *	@param i 普通の int か、リザルトコード( EResult )
	 */
	CTmlResult(EResult i) : m_eResult(i), m_eScene(Scene_Non), m_eOperation(Operation_Non), m_iFileLine(0)
	{
	}

	/**
	 * コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照.
	 */
	CTmlResult& operator=(const CTmlResult& other) 
	{
		m_eResult = other.m_eResult;
		m_eScene = other.m_eScene;
		m_eOperation = other.m_eOperation;
		m_iFileLine = other.m_iFileLine;
		m_strFuncName = other.m_strFuncName;
		m_strToken = other.m_strToken;
		m_strFileName = other.m_strFileName;
		return *this;
	}

	/**
	 * [代入] int型値代入.
	 *		結果代入。
	 *		「オペレーションコード」「シーンコード」
	 *		「エラーソース位置」「エラー内容」「発生関数名」は無しです。
	 *	@param i 普通の int か、リザルトコード( EResult )
	 *	@return 自分の参照
	 */
	CTmlResult& operator=(EResult i)
	{
		*this = CTmlResult();
		m_eResult = i;
		return *this;
	}

	/**
	 * [設定] NOP設定.
	 *		結果を NOPにします。
	 */
	void SetNop(void)
	{
		*this = CTmlResult();
		m_eResult = Result_Nop;
	}

	/**
	 * [設定] SUCCESS設定.
	 *		結果を SUCCESSにします。
	 */
	void SetSuccess(void)
	{
		*this = CTmlResult();
		m_eResult = Result_Success;
	}

	/**
	 * [確認] SUCCESS 確認.
	 *	@retval true Success
	 *	@retval false Nop か エラー。
	 */
	bool IsSuccess(void) const
	{
		return m_eResult == Result_Success;
	}

	/**
	 * [確認] ERROR 確認.
	 *	@retval true エラー
	 *	@retval false Nop か Success。
	 */
	bool IsError(void) const
	{
		return m_eResult < 0;
	}

	/**
	 * [確認] NOP 確認.
	 *	@retval true Nop
	 *	@retval false Success か エラー。
	 */
	bool IsNop(void) const
	{
		return m_eResult == Result_Nop;
	}

	/**
	 * [設定] シーンコード設定
	 *	@param eScene シーンコード
	 */
	void SetScene(EScene eScene)
	{
		m_eScene = eScene;
	}
	
	/**
	 * [設定] オペレーションコード設定
	 *	@param eOperation オペレーションコード
	 */
	void SetOperation(EOperation eOperation)
	{
		m_eOperation = eOperation;
	}

	/**
	 * [設定] エラー発生関数名設定
	 *	@param lpszFunc エラー発生関数名
	 */
	void SetFuncName(LPCSTR lpszFunc)
	{
		m_strFuncName = lpszFunc;
	}

	/**
	 * [設定]　エラー内容設定
	 *	@param lpszString エラー内容
	 */
	void SetErrorString(LPCSTR lpszString)
	{
		m_strToken = lpszString;
	}

	/**
	 * [設定] エラーソースファイル名、行数設定
	 *	@param lpszFileName エラーソースファイル名
	 *	@param iFileLine エラー発生行番号
	 *	@return 自分の参照。
	 */
	CTmlResult& SetLine(LPCSTR lpszFileName, INT_PTR iFileLine)
	{
		m_iFileLine		= iFileLine;
		m_strFileName	= lpszFileName;
		return *this;
	}
	
	/**
	 * [設定] エラーソースファイル名、行数設定
	 *	@param gram 文字列解析クラス
	 *	@return 自分の参照。
	 */
	CTmlResult& SetLine(const CTmlGrammar& gram)
	{
		m_iFileLine		= gram.GetLine();
		m_strFileName	= gram.GetName();
		return *this;
	}

	/**
	 * [取得] リザルトコード取得.
	 *	@return リザルトコード
	 */
	EResult GetResult(void) const
	{
		return m_eResult;
	}

	/**
	 * [取得] シーンコード取得
	 *	@return シーンコード
	 */
	EScene GetScene(void) const
	{
		return m_eScene;
	}

	/**
	 * [取得] オペレーションコード取得
	 *	@return オペレーションコード
	 */
	EOperation GetOperation(void) const
	{
		return m_eOperation;
	}

	/**
	 * [取得] エラー発生関数名取得
	 *	@return エラー発生関数名
	 */
	LPCSTR GetFuncName(void) const
	{
		return m_strFuncName;
	}

	/**
	 * [取得] エラー内容取得
	 *	@return エラー内容
	 */
	LPCSTR GetErrorString(void) const
	{
		return m_strToken;
	}

	/**
	 * ｢取得｣ エラーソース行番号取得
	 *	@return エラーソース行番号
	 */
	INT_PTR GetFileLine(void) const 
	{
		return m_iFileLine;
	}

	/**
	 * [取得] エラーソースファイル名取得
	 *	@return エラーソースファイル名
	 */
	LPCSTR GetFileName(void) const
	{
		return m_strFileName;
	}

	/**
	 * [取得] 文字列取得.
	 *	@note 本インスタンスが保持している内容から、文字列を生成します。
	 *			ただしエラーコードが0以上の場合、"" を返します。
	 *	@return 文字列  \n
	 *		ex)
	 *			"エラーソース名(行): 関数名 シーン オペレーション リザルト エラー内容"
	 */
	CAscii ToString(void)
	{
		if ( ! IsError() )
		{
			return "";
		}
		//
		CAscii strFile;
		if ( ! m_strFileName.IsEmpty() )
		{
			strFile.Format("%s(%d):", m_strFileName, m_iFileLine);
		}
		//
		CAscii strFunc = GetFuncName();
		if ( ! strFunc.IsEmpty() )
		{
			strFunc = strFunc.Sandwich(" FuncName=[", "]");
		}
		//
		CAscii strScene = SceneToString(m_eScene);
		if ( ! strScene.IsEmpty() )
		{
			strScene = strScene.Sandwich(" Scene=[", "]");
		}
		//
		CAscii strOperation = OperationToString(m_eOperation);
		if ( ! strOperation.IsEmpty() )
		{
			strOperation = strOperation.Sandwich(" Operation=[", "]");
		}
		//
		CAscii strResult = ResultToString(m_eResult);
		if ( ! strResult.IsEmpty() )
		{
			strResult = strResult.Sandwich(" Result=[", "]");
		}
		//
		CAscii strError = GetErrorString();
		if ( ! strError.IsEmpty() )
		{
			strError = strError.Sandwich(" / [", "]");
		}
		//
		return strFile + strFunc + strScene + strOperation + strResult + strError;
	}

	/**
	 * [確認] パラメータの数と、期待の数のチェック.
	 *	@param iParamNum 実際のパラメータの数。
	 *	@param iExpectNum 期待するパラメータ数
	 *	@return 結果。
	 */
	static CTmlResult CheckParamNum(INT_PTR iParamNum, INT_PTR iExpectNum)
	{
		CTmlResult r;
		if ( iExpectNum == 0 && iParamNum > 0 )
		{
			//引数は不必要です。
			r = CTmlResult(Operation_CheckFunc, Result_UnnecessaryParam);
		}
		else if ( iParamNum < iExpectNum )
		{
			//引数が足りません。
			r = CTmlResult(Operation_CheckFunc, Result_NotenoughParam);
		}
		else if ( iParamNum > iExpectNum )
		{
			//引数が多すぎます。
			r = CTmlResult(Operation_CheckFunc, Result_ManyParam);
		}
		return r;
	}

	/**
	 * [変換] シーンコード→文字列
	 *	@param eScene シーンコード
	 *	@return 文字列
	 */
	static LPCSTR SceneToString(EScene eScene)
	{
		const static CStaticMapT<EScene, LPCSTR> t[] =
		{
			Scene_Non,			"",
			Scene_Val,			"val文処理中",
			Scene_Array,		"array文処理中",
			Scene_Func,			"func文処理中",
			Scene_If,			"if文処理中",
			Scene_Else,			"else文処理中",
			Scene_Switch,		"switch文処理中",
			Scene_Case,			"case文処理中",
			Scene_Default,		"default文処理中",
			Scene_While,		"while文処理中",
			Scene_Break,		"break文処理中",
			Scene_Return,		"return文処理中",
			Scene_Include,		"include文処理中",
			Scene_End,			"end文処理中",
		};
		return CStaticMapT<EScene, LPCSTR>::Find(eScene, t);
	}

	/**
	 * [変換] オペレーションコード→文字列
	 *	@param eOperation オペレーションコード
	 *	@return 文字列
	 */
	static LPCSTR OperationToString(EOperation eOperation)
	{
		const static CStaticMapT<EOperation, LPCSTR> t[] =
		{
			Operation_Non,				"",
			Operation_Text,				"文字列処理中",
			Operation_Text_Esc,			"文字列 \\? 処理中",
			Operation_Text_EscHex,		"文字列 \\x 処理中",
//			Operation_Text_EscInsert,	"文字列 \\i() 処理中",
			Operation_Calc,				"計算処理中",
			Operation_CheckFunc,		"関数チェック中",
			Operation_CheckStatement,	"ステートメントチェック中",
			Operation_FindingFunc,		"関数検索中",
			Operation_FileAccess,		"Fileアクセス中",
		};
		return CStaticMapT<EOperation, LPCSTR>::Find(eOperation, t);
	}
	
	/**
	 * [変換] リザルトコード→文字列
	 *	@param result リザルトコード
	 *	@return 文字列
	 */
	static LPCSTR ResultToString(INT_PTR result)
	{
		const static CStaticMapT<INT_PTR, LPCSTR> t[] =
		{
			Result_Success,						"",
			Result_UnknownError,				"その他のエラー",
			Result_InvalidEscSeq,				"エスケープシーケンスが異常",
			Result_InvalidHexNotation,			"HEX表記が異常",
			Result_NotFoundParentheses,			"括弧が必要なところにない",
			Result_NotCloseParentheses,			"括弧が閉じていない",
			Result_InvalidParenthesesPosition,	"括弧の位置がおかしい",
			Result_UnfinishedExpression,		"計算式が途中で終わっている",
			Result_ZeroDivide,					"０で割ろうとしてます",
			Result_UnknownKeyword,				"知らないキーワード",
			Result_InvalidWordPosition,			"単語位置が異常です（そこには存在しないはず）",
			Result_NotName,						"名前がありません",
			Result_AlreadyExistName,			"既にある名前です",
			Result_UnnecessaryParam,			"引数は不必要です",
			Result_ManyParam,					"引数が多すぎます",
			Result_NotenoughParam,				"引数が足りません",
			Result_OutOfRange,					"範囲外です",
			Result_InvalidMacroContentsLevel,	"マクロコンテンツレベルが異常",
			Result_OpenFailure,					"Open失敗",
			Result_ReadFailure,					"Read失敗",
			Result_SendFailure,					"Send失敗",
			Result_InvalidState,				"状態エラー（今、実行できません）",
			Result_NotFoundFunction,			"必要な関数がありません",
			Result_UserInterruption,			"中断",
			Result_ProgramableInterruption,		"終了",
		};
		return CStaticMapT<INT_PTR, LPCSTR>::Find(result, t);
	}

private:
	EResult		m_eResult;		///< リザルトコード
	EScene		m_eScene;		///< エラーのシーン
	EOperation	m_eOperation;	///< エラーのオペレーション
	CAscii		m_strFuncName;	///< エラー発生関数名
	CAscii		m_strToken;		///< エラー内容
	CAscii		m_strFileName;	///< エラーソースファイル名 (エラーソース位置)
	INT_PTR		m_iFileLine;	///< エラーソース行番号 (エラーソース位置)
};



}; // TNB
