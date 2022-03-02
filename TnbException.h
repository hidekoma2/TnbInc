#pragma once
/**
 *@file
 * 例外状態管理関係のヘッダ
 *
 *		TNB Libraryで発生する例外クラスのBaseクラスを宣言しています。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrLib.h"
#include "TnbPointerHandle.h"



#include <tchar.h>



//TNB Library
namespace TNB
{



/**@ingroup EXCEPTION
 * 例外ベースクラス
 *
 *	@note 例外処理はこのクラスをベースにします。
 *
 *	@date 06/05/03	新規作成。
 */
class CTnbException
{
	CArrayPtrHandleT<TCHAR>	m_hpText;
	DWORD					m_dwParam;
public:
	/**
	 * コンストラクタ
	 *	@param lpszMsg 文字列
	 *	@param dwParam パラメータ
	 */
	explicit CTnbException(LPCTSTR lpszMsg, DWORD dwParam = 0)
	{
		if ( lpszMsg == NULL ){ lpszMsg = _T(""); }
		TRACE1("throw Tnb-Exception;[%s]\n", lpszMsg);
		size_t iSize = STRLIB::GetLen(lpszMsg);
		m_hpText = new TCHAR[iSize + 1];
		STRLIB::Copy(m_hpText, lpszMsg);
		m_dwParam = dwParam;
	}
	/**
	 * [取得] パラメータ取得
	 *	@return パラメータ
	 */
	DWORD GetParam(void) const { return m_dwParam; }
	/**
	 * [取得] 例外文字列取得
	 *	@return 文字列
	 */
	virtual LPCTSTR GetString(void) const { return m_hpText; }
	/**
	 * [表示] 内容表示
	 *	@note catch したとき、このメソッドをコールするようにしてください
	 *	@note デバグビルドの時、内容を表示します。リリース版のときは、何もしません。
	 */
	void OnCatch(void) const
	{
		TRACE1("catch Tnb-Exception;[%s]\n", GetString());
	}
};



/**@ingroup EXCEPTION
 * INDEX範囲外例外
 */
class CIndexOutOfBoundsException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CIndexOutOfBoundsException(DWORD dwParam = 0) : _super(_T("INDEXが範囲外です。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * 追加失敗例外
 */
class CAppendFailureException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CAppendFailureException(DWORD dwParam = 0) : _super(_T("追加に失敗しました。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * 取得要素(空き)無し例外
 */
class CEmptyException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CEmptyException(DWORD dwParam = 0) : _super(_T("空です。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * データ足らず発生例外
 */
class CShortOfDataException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CShortOfDataException(DWORD dwParam = 0) : _super(_T("データが足りません。"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * 長さ異常発生例外
 */
class CInvalidLengthException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CInvalidLengthException(DWORD dwParam = 0) : _super(_T("長さが異常です。"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * タイプ相違例外
 */
class CNoSuchTypeException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CNoSuchTypeException(DWORD dwParam = 0) : _super(_T("タイプが異なります。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * パラメータ不正例外
 */
class CInvalidParamException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CInvalidParamException(DWORD dwParam = 0) : _super(_T("パラメータが不正です。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * NULLポインタ例外
 */
class CNullPointerException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CNullPointerException(DWORD dwParam = 0) : _super(_T("NULLポインタです。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * サポート外例外
 */
class CNotSupportException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CNotSupportException(DWORD dwParam = 0) : _super(_T("サポートされていません。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * 見付からない例外
 */
class CNotFoundException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CNotFoundException(DWORD dwParam = 0) : _super(_T("見付かりません。"), dwParam) {}
};



/**@ingroup EXCEPTION
 * 読み書き失敗発生例外
 *	@note Paramにはシステムエラーコードを持ちます。
 */
class CIoFailureException : public CTnbException
{
	DEFSUPER(CTnbException);
public:
	/// コンストラクタ
	CIoFailureException(LPCTSTR lpszMsg = NULL, DWORD dwParam = 0)
		: _super((lpszMsg == NULL) ? _T("入出力失敗です。") : lpszMsg, dwParam) {}
}; 



/**@ingroup EXCEPTION
 * オープン失敗発生例外
 *	@note Paramにはシステムエラーコードを持ちます。
 */
class COpenFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// コンストラクタ
	COpenFailureException(DWORD dwParam) : _super(_T("オープン失敗です。"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * 読み込み失敗発生例外
 *	@note Paramにはシステムエラーコードを持ちます。
 */
class CReadFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// コンストラクタ
	CReadFailureException(DWORD dwParam) : _super(_T("読み込み失敗です。"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * 書き込み失敗発生例外
 *	@note Paramにはシステムエラーコードを持ちます。
 */
class CWriteFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// コンストラクタ
	CWriteFailureException(DWORD dwParam) : _super(_T("書き込み失敗です。"), dwParam) {}
}; 



/**@ingroup EXCEPTION
 * 削除失敗発生例外
 *	@note Paramにはシステムエラーコードを持ちます。
 */
class CDeleteFailureException : public CIoFailureException
{
	DEFSUPER(CIoFailureException);
public:
	/// コンストラクタ
	CDeleteFailureException(DWORD dwParam) : _super(_T("削除失敗です。"), dwParam) {}
}; 



}; //TNB
