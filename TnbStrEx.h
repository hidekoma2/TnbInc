#pragma once
/**
 *@file
 * 文字列処理関係のヘッダ
 *
 *		CStrT クラスを使った、汎用的な関数が有ります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user, "T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * [取得] PC名取得
 *	@note WindowCEでは、Empty文字が得られます。
 *	@return PC名取得
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CStr GetComputerName(void)
{
	CStr str;
	#ifndef _WIN32_WCE
		DWORD dwLength = MAX_COMPUTERNAME_LENGTH + 1;
		::GetComputerName(str.GetBuffer(dwLength), &dwLength);
		str.ReleaseBuffer();
	#else
		str = _T("CE");
	#endif
	return str;
}



/**@ingroup STRING
 * [取得] プロセスのパス取得.
 *		現在のプロセスのパスを取得します。
 *	@return パス(文字列の最後にパス区切り文字はつきません)
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CStr GetProcessPath(void)
{
	CStr str;
	DWORD dwRc = ::GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);
	str.ReleaseBuffer();
	if ( dwRc != 0 )
	{
//		INT_PTR p1 = str.ReverseFind('\\');
//		INT_PTR p2 = str.ReverseFind('/');
//		INT_PTR p0 = (p1 < p2) ? p2 : p1;
		INT_PTR p = str.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return str.Left(p);
		}
	}
	return str;
}



/**@ingroup STRING
 * [取得] プロセスネーム取得.
 *		現在のプロセスネームを取得します。
 *	@return ネーム
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CStr GetProcessName(void)
{
	CStr str;
	DWORD dwRc = ::GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);
	str.ReleaseBuffer();
	if ( dwRc != 0 )
	{
//		INT_PTR p1 = str.ReverseFind('\\');
//		INT_PTR p2 = str.ReverseFind('/');
//		INT_PTR p0 = (p1 < p2) ? p2 : p1;
		INT_PTR p = str.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return str.Mid(static_cast<INDEX>(p + 1));
		}
	}
	return str;
}



/**@ingroup STRING
 * [取得] リソース文字列取得
 *	@param nID Windowsの文字列リソースID
 *	@param hInst インスタンスハンドル。
 *			省略すると、 GetInstanceHandle(EI_String) で得られるハンドルを使用します。
 *			MFCプロジェクトでない時、 SetInstanceHandle(EI_String) で設定されたハンドルを使用します。
 *	@return 文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CStr LoadStr(UINT nID, HINSTANCE hInst = NULL)
{
	if ( hInst == NULL )
	{
		hInst = GetInstanceHandleByTnb(EI_String);
	}
	CStr str;
	int iLen = 256;
	while ( true )
	{
		int r = ::LoadString(hInst, nID, str.GetBuffer(iLen), iLen);
		str.ReleaseBuffer();
		if ( r == 0 )
		{
			str.Empty();
			break;
		}
		if ( r < iLen - 2 )
		{
			break;
		}
		iLen *= 2;
	}
	return str;
}



/**@ingroup STRING
 * [変換] 文字変換(ASCII/SJIS用)
 *	@note 全角、半角、かな、カナ、大文字、小文字の変換。
 *	@see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/nls_5s2v.asp
 *	@param lpsz 元の文字列
 *	@param dwFlags 目的とする変換<BR>
 *					LCMAP_LOWERCASE	- 英大文字を小文字へ変換	<BR>
 *					LCMAP_FULLWIDTH	- 半角文字を全角文字へ変換	<BR>
 *					LCMAP_HALFWIDTH	- 全角文字を半角文字へ変換	<BR>
 *					LCMAP_HIRAGANA	- カタカナをひらがなへ変換	<BR>
 *					LCMAP_KATAKANA	- ひらがなをカタカナへ変換	<BR>
 *					LCMAP_UPPERCASE	- 英小文字を大文字へ変換	<BR>
 *					LCMAP_LOWERCASE	- 英大文字を小文字へ変換	
 *	@return 変換後の文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CAscii LocalMapString(LPCSTR lpsz, DWORD dwFlags)
{
	// 半角→全角変換で最大2倍のサイズになる
	size_t iLen = (STRLIB::GetLen(lpsz) + 1) * 2;
	CWorkMemT<char> work(iLen);
	int iRc = ::LCMapStringA(::GetUserDefaultLCID(),
				dwFlags,
				lpsz, -1,	// 入力文字列と長さ(NULL終端なら長さに-1指定可能)
				work, ToInt(iLen));// 変換後の文字列を格納するバッファとバッファ長
	if ( iRc <= 0 )
	{
		work.Free();
	}
	return CAscii(work);
}



/**@ingroup STRING
 * [変換] 文字変換(UNICODE用)
 *	@note 全角、半角、かな、カナ、大文字、小文字の変換。
 *	@see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/nls_5s2v.asp
 *	@param lpsz 元の文字列
 *	@param dwFlags 目的とする変換<BR>
 *					LCMAP_LOWERCASE	- 英大文字を小文字へ変換	<BR>
 *					LCMAP_FULLWIDTH	- 半角文字を全角文字へ変換	<BR>
 *					LCMAP_HALFWIDTH	- 全角文字を半角文字へ変換	<BR>
 *					LCMAP_HIRAGANA	- カタカナをひらがなへ変換	<BR>
 *					LCMAP_KATAKANA	- ひらがなをカタカナへ変換	<BR>
 *					LCMAP_UPPERCASE	- 英小文字を大文字へ変換	<BR>
 *					LCMAP_LOWERCASE	- 英大文字を小文字へ変換	
 *	@return 変換後の文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CUnicode LocalMapString(LPCWSTR lpsz, DWORD dwFlags)
{
	size_t iLen = STRLIB::GetLen(lpsz) + 1;
	CWorkMemT<WCHAR> work(iLen);
	int iRc = ::LCMapStringW(::GetUserDefaultLCID(),
				dwFlags,
				lpsz, -1,	// 入力文字列と長さ(NULL終端なら長さに-1指定可能)
				work, ToInt(iLen));// 変換後の文字列を格納するバッファとバッファ長
	if ( iRc <= 0 )
	{
		work.Free();
	}
	return CUnicode(work);
}



/**@ingroup STRING
 * [変換] 小文字変換(ASCII/SJIS用)
 *	@note 小文字へ変換した文字列を作成。
 *	@param lpsz 元の文字列
 *	@return 変換後の文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CAscii LowerString(LPCSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_LOWERCASE);
}



/**@ingroup STRING
 * [変換] 小文字変換(UNICODE用)
 *	@note 小文字へ変換した文字列を作成。
 *	@param lpsz 元の文字列
 *	@return 変換後の文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CUnicode LowerString(LPCWSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_LOWERCASE);
}



/**@ingroup STRING
 * [変換] 大文字変換(ASCII/SJIS用)
 *	@note 大文字へ変換した文字列を作成。
 *	@param lpsz 元の文字列
 *	@return 変換後の文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CAscii UpperString(LPCSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_UPPERCASE);
}



/**@ingroup STRING
 * 大文字変換(UNICODE用)
 *	@note 大文字へ変換した文字列を作成。
 *	@param lpsz 元の文字列
 *	@return 変換後の文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CUnicode UpperString(LPCWSTR lpsz)
{
	return LocalMapString(lpsz, LCMAP_UPPERCASE);
}



/**@ingroup STRING
 * [変換] SystemErrorコード文字列化
 *	@param dwError GetLastError() の戻り値など、SystemErrorコード。
 *	@param withRowValue true なら生数値も括弧閉じで文字列の最後に付加します。
 *	@return 文字列
 *	@par必要ファイル
 *			TnbStrEx.h
 */
inline CStr SystemErrorToMessageText(DWORD dwError, bool withRowValue = false)
{
	LPTSTR lpBuffer = NULL;
	const DWORD dwFlag	= FORMAT_MESSAGE_ALLOCATE_BUFFER 
						| FORMAT_MESSAGE_FROM_SYSTEM
						| FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD r = ::FormatMessage( 
					dwFlag,
					NULL,
					dwError,
					LANG_USER_DEFAULT,
					reinterpret_cast<LPTSTR>(&lpBuffer),
					0,
					NULL);
	CStr strErrMessage;
	if ( r == 0 )
	{
		strErrMessage = _T("Unknown");
	}
	else
	{
		strErrMessage = lpBuffer;
		strErrMessage.TrimRight(_T("\n\r"));
		::LocalFree(lpBuffer);
	}
	if ( withRowValue )
	{
		strErrMessage += CStr::Fmt(_T("(%d)"), dwError);
	}
	return strErrMessage;
}



}; //TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")

