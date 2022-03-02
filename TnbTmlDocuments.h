#pragma once
/**
 *@file
 * TinyMacroLang ドキュメント管理関係のヘッダ
 *	
 *		TMLの変数管理のクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTmlValue.h"
#include "TnbTmlCalculator.h"
#include "TnbTmlResult.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang ドキュメント管理
 *
 * コンテンツ管理します<BR>
 *	コンテンツ(ファイルの内容など)を、レベル管理します。
 *
 *	@par必要ファイル
 *			TnbTmlDocuments.h
 *
 *	@date 06/01/01 新規作成
 *	@date 07/07/20 改善
 */
class CTmlDocuments : public CTml, CCopyImpossible
{
public:

	/**
	 * ファイル情報.
	 *	@see FindFileName
	 */
	struct TFileInfo
	{
		/// 関数情報*
		struct TFunc
		{
			CAscii			strFuncName;	///< 関数名
			CAsciiVector	strParamNames;	///< 引数名
			INT_PTR			iOffset;		///< 関数開始のOffset(｛の次の位置を指している)
			INT_PTR			iLine;			///< 関数開始のLine(｛の次の位置を指している)
			/// コンストラクタ
			TFunc(void) : iOffset(0), iLine(0){}
			/// コピーコンストラクタ
			TFunc(const TFunc& other)
				: strFuncName(other.strFuncName), strParamNames(other.strParamNames), iOffset(other.iOffset), iLine(other.iLine)
			{
			}
			/// コピーオペレータ
			TFunc& operator=(const TFunc& other)
			{
				strFuncName = other.strFuncName;
				strParamNames = other.strParamNames;
				iOffset = other.iOffset;
				iLine = other.iLine;
				return *this;
			}
		};
		CAscii			strName;	///< includeしたファイル名
		CAscii			abContents;	///< ファイル内容＋NULL終端
		CVectorT<TFunc>	atFunc;		///< 関数情報
		/// コンストラクタ
		TFileInfo(void) {}
		/// コピーコンストラクタ
		TFileInfo(const TFileInfo& other) : strName(other.strName), abContents(other.abContents), atFunc(other.atFunc)
		{
		}
		/// コピーオペレータ
		TFileInfo& operator=(const TFileInfo& other)
		{
			strName = other.strName;
			abContents = other.abContents;
			atFunc = other.atFunc;
			return *this;
		}
	};


	/// コンストラクタ
	CTmlDocuments(void)
	{
	}

	/// Empty
	void Empty(void)
	{
		m_aaFileInfo.RemoveAll();
		TTRACE0( "TML::CDocuments::初期化\n" );
	}

	/**
	 * [取得] 現在レベル取得
	 *	@return レベル。
	 */
	INT_PTR GetLevel(void) const
	{
		return m_aaFileInfo.GetSize();
	}

	/**@ingroup TML
	 * TinyMacroLang 検索結果管理
	 */
	struct TFuncInfoRes
	{
		LPCSTR	lpszFuncAdds;					///< 関数のあるアドレス(関数の頭の { の直後)
		CAscii	strFileName;					///< ファイル名	
		INT_PTR	iFuncLine;						///< 関数のあるファイ行
		const CAsciiVector*	pastrParamNames;	///< 関数の引数名(Arrayの参照)
		/// コンストラクタ
		TFuncInfoRes(void) : lpszFuncAdds(NULL), iFuncLine(0), pastrParamNames(NULL)
		{
		}
		/**
		 * [確認] データを持っているか？
		 *	@retval true データあり。
		 *	@retval false なし。
		 */
		bool HasData(void) const
		{
			return lpszFuncAdds != NULL;
		}
	};

	/**
	 * [検索] 関数検索.
	 *		レベルの高い(後に追加された)コンテンツから順に検索します。
	 *	@param lpszFunc 関数名
	 *	@return 関数情報。この HasData()の結果がfalseなら未発見。
	 */
	TFuncInfoRes FindFunc(LPCSTR lpszFunc) const
	{
		INT_PTR iDim1 = m_aaFileInfo.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			const CFileInfoArray& fiv = m_aaFileInfo.At(i);
			INT_PTR iDim2 = fiv.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				const TFileInfo& T = fiv.At(j);
				INT_PTR iDimF = T.atFunc.GetSize();
				for( INT_PTR f = 0; f < iDimF; f++ )
				{
					const TFileInfo::TFunc& X = T.atFunc[f];
					if ( X.strFuncName.Compare(lpszFunc) == 0 )
					{
						//あった
						TFuncInfoRes t;
						t.strFileName		= T.strName;
						t.lpszFuncAdds		= T.abContents.begin();
						t.lpszFuncAdds		+= X.iOffset;
						t.iFuncLine			= X.iLine;
						t.pastrParamNames	= &X.strParamNames;
						return t;
					}
				}
			}
		}
		TFuncInfoRes t;
		return t;
	}

	/**
	 * [検索] ファイル検索.
	 *		保持しているファイルに、指定のファイル名があるかチェックします。。
	 *	@param lpszFile ファイル名
	 *	@retval NULL 未発見。
	 *	@retval NULL以外 ファイル情報の参照ポインタ。
	 */
	const TFileInfo* FindFileName(LPCSTR lpszFile) const
	{
		INT_PTR iDim1 = m_aaFileInfo.GetSize();
		for( INT_PTR i = 0; i < iDim1; i++ )
		{
			const CFileInfoArray& fiv = m_aaFileInfo.At(i);
			INT_PTR iDim2 = fiv.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				const TFileInfo &T = fiv.At(j);
				if ( T.strName.Compare(lpszFile) == 0 )
				{
					return &T;
				}
			}
		}
		return NULL;
	}

	/**
	 * [操作] レベル＋１
	 */
	void IncLevel(void)
	{
		CFileInfoArray A;
		m_aaFileInfo.Add(A);
		TTRACE1( "TML::CDocuments::レベル++ NowLvl=%d\n", GetLevel() );
	}

	/**
	 * [操作] レベル－１
	 *	@retval ture 成功。
	 *	@retval false 失敗。
	 */
	bool DecLevel(void)
	{
		INT_PTR l = m_aaFileInfo.GetSize();
		if ( l <= 0 )
		{
			return false;
		}
		//
		m_aaFileInfo.SetSize( l - 1 );
		TTRACE1( "TML::CDocuments::レベル-- NowLvl=%d\n", GetLevel() );
		return true;
	}

	/**
	 * [設定] レベル指定.
	 *		レベルを指定の位置へ下げる。
	 *	@param level レベル。		
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool DecLevels(INT_PTR level)
	{
		while( level < GetLevel() )
		{
			if ( ! DecLevel() )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [追加] ファイル情報追加.
	 *		今のレベルにファイルを追加する
	 *	@param[out] _tFuncInfo 追加後の、実行すべき情報。
	 *	@param[in] lpszName	ファイル名
	 *	@param[in] lpszData データ内容
	 *	@return 結果。
	 */
	CTmlResult Add(TFuncInfoRes& _tFuncInfo, LPCSTR lpszName, LPCSTR lpszData)
	{
		size_t l = m_aaFileInfo.GetSize();
		if ( l == 0 )
		{
			//マクロコンテンツレベルが異常です。
			CTmlResult r(Operation_Non, Result_InvalidMacroContentsLevel);
			return r;
		}
		if ( FindFileName(lpszName) != NULL )
		{
			//既に登録されている（二回同じインクルードファイル指定自体はエラーではない）。
			TTRACE1( "[%s]は既に登録されています。\n", lpszName );
			return Result_Nop;
		}
		// 情報構築
		TFileInfo tFileInfo;
		tFileInfo.strName = lpszName;
		tFileInfo.abContents = lpszData;
		//内容チェック
		CTmlResult r = m_AnalyzeFile(tFileInfo);
		if ( r.IsError() )
		{
			return r;
		}
		CFileInfoArray &A = m_aaFileInfo.Ref(l - 1);
		A.Add(tFileInfo);
		_tFuncInfo.strFileName		= lpszName;
		_tFuncInfo.lpszFuncAdds		= tFileInfo.abContents;
		_tFuncInfo.iFuncLine		= 1;
		_tFuncInfo.pastrParamNames	= NULL;
		return Result_Success;
	}

private:
	typedef CVectorT<TFileInfo> CFileInfoArray;			///< 一つのファイルの配列型
	typedef CVectorT<CFileInfoArray> CFileInfoArrays;	///< 一つのファイルの二次元配列型
	CFileInfoArrays m_aaFileInfo;						///< 一つのファイルの配列

	/**
	 * ファイル内容を解析します。
	 *	@param[in,out] _tFileInfo INでファイル内容 Outで関数情報
	 *	@return 結果。
	*/
	CTmlResult m_AnalyzeFile(TFileInfo& _tFileInfo)
	{
		CTmlResult res;
		CTmlGrammar gram(_tFileInfo.abContents.begin(), _tFileInfo.abContents.end(), 1);
		CStr strToken;
		CTmlGrammar::CParts parts;
		//
		while(true)
		{
			parts = gram.GetNextParts();
			if ( parts.GetKind() == CTmlGrammar::FINAL )
			{
				break;// 終わり
			}
			if ( parts.GetKind() == CTmlGrammar::ERROR_DEPTH )
			{
				res = Result_InvalidParenthesesPosition;// 括弧エラー
				break;
			}
			if ( parts.GetKind() == CTmlGrammar::OPEN_CHAR )
			{
				if ( ! gram.SkipoutBlock() )
				{
					res = Result_NotCloseParentheses;//閉じてない
					break;
				}
				continue;
			}
			if ( parts.GetKind() == CTmlGrammar::STRING )
			{
				continue;
			}
			if ( parts.GetKind() == CTmlGrammar::CLOSE_CHAR )
			{
				ASSERTLIB( false );
			}
			CAscii s = parts.GetString();
			if ( s == ";" )
			{
				continue;
			}
			if ( s == "}" )
			{
				//突然閉じた
				ASSERTLIB( false );
				continue;
			}
			if ( s.Compare("func") != 0 )
			{
				continue;
			}
			//
			parts = gram.GetNextParts();
			if ( parts.GetKind() != CTmlGrammar::TOKEN )
			{
				res = Result_NotName;
				break;
			}
			CAscii funcName = parts.GetString();
			parts = gram.GetNextParts();
			if ( parts.GetAt() != '(' )
			{
				res = Result_NotFoundParentheses;//関数名の後に ( がない
				break;
			}
			//
			TFileInfo::TFunc tFunc;
			tFunc.strFuncName = funcName;
			while(true)
			{
				parts = gram.PeekNextParts();
				if ( parts.GetKind() != CTmlGrammar::TOKEN )
				{
					break;
				}
				gram.GetNextParts();
				CAscii s = parts.GetString();
				tFunc.strParamNames.Add(s);
				parts = gram.PeekNextParts();
				if ( parts.GetAt() != ',' )
				{
					break;
				}
				gram.GetNextParts();
			}	
			if ( ! gram.SkipoutBlock() )
			{
				res = Result_NotCloseParentheses;//閉じてない
				break;
			}
			parts = gram.GetNextParts();
			if ( parts.GetAt() != '{' )
			{
				res = Result_NotFoundParentheses;// { がない
				break;
			}
			tFunc.iOffset	= gram.GetPos();
			tFunc.iLine		= gram.GetLine();
			if ( ! gram.SkipoutBlock() )
			{
				res = Result_NotCloseParentheses;//閉じてない
				break;
			}
			_tFileInfo.atFunc.Add(tFunc);
		}
		if ( res.IsError() )
		{
			CTmlResult r(Scene_Func, Operation_FindingFunc, res.GetResult());
			r.SetLine(_tFileInfo.strName, parts.GetLine());
			res = r;
		}
		return res;
	}

	friend class CTmlDocumentsTest;
};



}; // TNB
