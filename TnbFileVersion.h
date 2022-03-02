#pragma once
/**
 *@file
 * ファイルバージョン関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



#ifndef _WIN32_WCE
 #include <Winver.h>
 #pragma comment(lib,"version.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ファイルバージョン取得クラス.
 *
 *	リソースにて付加されている情報を取得することが出来ます。
 *	ファイル指定することで、自プロセス以外の情報も取得できます。
 *
 *	@note Versionリソースの "SpecialBuild" に文字列を入れると、スペシャルビルドフラグが立ちます。
 *	@note Versionリソースの "PrivateBuild" に文字列を入れると、プライベートビルドフラグが立ちます。
 *
 *	@par必要ファイル
 *			TnbFileVersion.h
 *
 *	@date 06/09/19 新規作成
 *	@date 08/04/11 UNICODEリソースチェック。 WritePush() 追加。
 */
class CFileVersion
{
public:

	/// コンストラクタ
	CFileVersion(void) : m_pFileInfo(NULL), m_pLangCodePage(NULL), m_langCodePageSize(0)
	{
	}

	/**
	 * [確認] 有効か
	 *	@retval true Openに成功している
	 *	@retval false Openしていない。
	 */
	bool IsValid(void) const
	{
		return m_pFileInfo != NULL;
	}

	/**
	 * [設定] オープン
	 *	@param lpszFile 対象のファイル名。省略すると自プロセスが対象になります。
	 *	@param hInst 対象のインスタンス。 lpszFile を NULL にします。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Open(LPCTSTR lpszFile = NULL, HINSTANCE hInst = NULL)
	{
		CStr strFile;
		if ( lpszFile != NULL )
		{
			strFile = lpszFile;
		}
		else
		{
			::GetModuleFileName(hInst, strFile.GetBuffer(MAX_PATH), MAX_PATH);
			strFile.ReleaseBuffer();
		}
		DWORD dummy;
		DWORD size = ::GetFileVersionInfoSize(strFile.GetBuffer(), &dummy);
		if ( size != 0)
		{
			m_workMem.Resize(size);
			ZeroMemory(m_workMem, size);
			if ( ::GetFileVersionInfo(strFile.GetBuffer(), 0, size, m_workMem) )
			{
				UINT uLen;
				if ( ::VerQueryValue(m_workMem, _T("\\"), reinterpret_cast<LPVOID*>(&m_pFileInfo), &uLen) )
				{
					if ( m_pFileInfo->dwSignature == VS_FFI_SIGNATURE )
					{
						if ( ::VerQueryValue(m_workMem,
							_T("\\VarFileInfo\\Translation"),
							reinterpret_cast<LPVOID*>(&m_pLangCodePage),
							&m_langCodePageSize) )
						{
							m_langCodePageSize /= sizeof(DWORD);
							strFile.ReleaseBuffer();
							m_fileName = strFile;
							return true;
						}
					}
				}
			}
		}
		Close();
		return false;
	}

	/**
	 * [設定] クローズ
	 */
	void Close(void)
	{
		m_workMem.Free();
		m_pFileInfo = NULL;
		m_pLangCodePage = NULL;
		m_langCodePageSize = 0;
		m_fileName.Empty();
	}

	/**
	 * [取得] 情報取得
	 *	@retval NULL以外 情報
	 *	@retval NULL 失敗（オープンしていないと失敗します）
	 */
	const VS_FIXEDFILEINFO* GetFileInfo(void) const
	{
		return m_pFileInfo;
	}

	/**
	 * [取得] 情報取得.
	 *	@note 取得した情報を書き換えて、 WritePush() することで、情報を書き換えることが出来ます。
	 *	@retval NULL以外 情報
	 *	@retval NULL 失敗（オープンしていないと失敗します）
	 */
	VS_FIXEDFILEINFO* GetFileInfo(void)
	{
		return m_pFileInfo;
	}

	/**
	 * [取得] 情報取得(文字列)
	 *	@param lpszType 取得情報種。以下の文字列の内いずれかを指定します。<BR>
	 *					"CompanyName"		- 会社名				<BR>
	 *					"FileDescription"	- 説明					<BR>
	 *					"FileVersion"		- ファイル・バージョン	<BR>
	 *					"InternalName"		- 内部名				<BR>
	 *					"LegalCopyright"	- 著作権				<BR>
	 *					"OriginalFileName"	- 正式ファイル名		<BR>
	 *					"ProductName"		- 製品名				<BR>
	 *					"ProductVersion"	- 製品バージョン		<BR>
	 *					"Comments"			- コメント				<BR>
	 *					"LegalTrademarks"	- 商標					<BR>
	 *					"PrivateBuild"		- ﾌﾟﾗｲﾍﾞｰﾄ・ﾋﾞﾙﾄﾞ情報	<BR>
	 *					"SpecialBuild"		- ｽﾍﾟｼｬﾙ・ﾋﾞﾙﾄﾞ情報
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を返します。
	 *	@retval 文字列 成功
	 *	@retval Empty状態 失敗（オープンしていないと失敗します）
	 */
	CStr Get(LPCTSTR lpszType, WORD wLanguage = 0) const
	{
		LPVOID P = m_GetTextPtr(lpszType, wLanguage);
		if ( P != NULL )
		{
			if ( HasUnicodeType() )
			{
				return CStr(reinterpret_cast<LPCWSTR>(P));
			}
			else
			{
				return CStr(reinterpret_cast<LPCSTR>(P));
			}
		}
		return CStr();
	}

	/**
	 * [取得] About画面用文字列取得.
	 *	@param lpszVerText バージョン文字表記。
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を返します。
	 *	@return 改行含んだ文字列
	 */
	CStr GetAboutText(LPCTSTR lpszVerText, WORD wLanguage = 0) const
	{
		if ( ! IsValid() ){ return CStr(); }
		return CStr::Fmt(_T("%s %s %s\n\n%s")
			, Get(_T("FileDescription"))
			, lpszVerText
			, GetFileVersionText()
			, GetCopyrightText()
		);
	}

	/**
	 * [取得] 著作権文字列取得.
	 *	@note "LegalCopyright" と "CompanyName" を連結した文字列を返します。
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を返します。
	 *	@return 著作権文字列
	 */
	CStr GetCopyrightText(WORD wLanguage = 0) const
	{
		if ( ! IsValid() ){ return CStr(); }
		return CStr::Fmt(_T("%s %s"),
				Get(_T("LegalCopyright"), wLanguage),
				Get(_T("CompanyName"), wLanguage));
	}

	/**
	 * [取得] ファイルバージョン文字列取得.
	 *	@note "FileVersion" とフラグから作成した文字列を返します。
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を返します。
	 *	@return 文字列
	 */
	CStr GetFileVersionText(WORD wLanguage = 0) const
	{
		return m_GetVersionText(_T("FileVersion"), wLanguage);
	}

	/**
	 * [取得] プロダクトバージョン文字列取得.
	 *	@note "ProductVersion" とフラグから作成した文字列を返します。
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を返します。
	 *	@return 文字列
	 */
	CStr GetProductVersionText(WORD wLanguage = 0) const
	{
		return m_GetVersionText(_T("ProductVersion"), wLanguage);
	}

	/**
	 * [取得] スペシャルビルドチェック.
	 *	@retval true スペシャルビルド
	 *	@retval false 否
	 */
	bool IsSpecialBuild(void) const
	{
		if ( IsValid() )
		{
			DWORD f = m_pFileInfo->dwFileFlags & m_pFileInfo->dwFileFlagsMask;
			return (f & VS_FF_SPECIALBUILD) != 0;
		}
		return false;
	}

	/**
	 * [取得] プライベートビルドチェック.
	 *	@retval true プライベートビルド
	 *	@retval false 否
	 */
	bool IsPrivateBuild(void) const
	{
		if ( IsValid() )
		{
			DWORD f = m_pFileInfo->dwFileFlags & m_pFileInfo->dwFileFlagsMask;
			return (f & VS_FF_PRIVATEBUILD) != 0;
		}
		return false;
	}

#ifndef _WIN32_WCE

	/**
	 * [変更] 情報変更.
	 *	@note ファイルの変更はしません。インスタンス内の情報を変更します。ファイルに反映させるには WritePush() を使います。
	 *	@attention 新規に設定することや、今の情報よりも長い文字列は設定できません。
	 *	@param lpszType 変更情報種。
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を変更します。
	 *	@param lpszValue 変更する文字列。
	 *	@retval マイナス エラー
	 *	@retval 0以上 変更した文字列長.
	 */
	INT_PTR Modify(LPCTSTR lpszType, LPCTSTR lpszValue, WORD wLanguage = 0)
	{
		INT_PTR res = -1;
		if ( IsValid() )
		{
			LPVOID P = m_GetTextPtr(lpszType, wLanguage);
			if ( P != NULL )
			{
				if ( HasUnicodeType() )
				{
					LPWSTR lpsz = reinterpret_cast<LPWSTR>(P);
					res = STRLIB::GetLen(lpsz);
					CUnicode val = lpszValue;
					STRLIB::Copy(lpsz, val.Left(res));
				}
				else
				{
					LPSTR lpsz = reinterpret_cast<LPSTR>(P);
					res = STRLIB::GetLen(lpsz);
					CAscii val = lpszValue;
					STRLIB::Copy(lpsz, val.Left(res));
				}
			}
		}
		return res;
	}

	/**
	 * [変更] ファイルバージョン変更.
	 *	@note ファイルの変更はしません。インスタンス内の情報を変更します。ファイルに反映させるには WritePush() を使います。
	 *	@param v1 バージョン１
	 *	@param v2 バージョン２
	 *	@param v3 バージョン３
	 *	@param v4 バージョン４
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を変更します。
	 *	@retval マイナス エラー
	 *	@retval 0以上 変更した文字列長.
	 */
	INT_PTR ModifyFileVersion(WORD v1, WORD v2, WORD v3, WORD v4, WORD wLanguage = 0)
	{
		if ( IsValid() )
		{
			CStr s;
			s.Format(_T("%d,%d,%d,%d"), v1, v2, v3, v4);
			m_pFileInfo->dwFileVersionMS = MAKELONG(v2, v1);
			m_pFileInfo->dwFileVersionLS = MAKELONG(v4, v3);
			return Modify(_T("FileVersion"), s, wLanguage);
		}
		return -1;
	}

	/**
	 * [変更] プロダクトバージョン変更.
	 *	@note ファイルの変更はしません。インスタンス内の情報を変更します。ファイルに反映させるには WritePush() を使います。
	 *	@param v1 バージョン１
	 *	@param v2 バージョン２
	 *	@param v3 バージョン３
	 *	@param v4 バージョン４
	 *	@param wLanguage 言語コード。省略すると最初に見つけた情報を変更します。
	 *	@retval マイナス エラー
	 *	@retval 0以上 変更した文字列長.
	 */
	INT_PTR ModifyProductVersion(WORD v1, WORD v2, WORD v3, WORD v4, WORD wLanguage = 0)
	{
		if ( IsValid() )
		{
			CStr s;
			s.Format(_T("%d,%d,%d,%d"), v1, v2, v3, v4);
			m_pFileInfo->dwProductVersionMS = MAKELONG(v2, v1);
			m_pFileInfo->dwProductVersionLS = MAKELONG(v4, v3);
			return Modify(_T("ProductVersion"), s, wLanguage);
		}
		return -1;
	}

	/**
	 * [設定] 書き込み.
	 *	@note Modify() や ModifyFileVersion() , GetFileInfo() などにより変更した情報をファイルに書き出します。
	 *	@retval true 成功
	 *	@retval false エラー
	 */
	bool WritePush(void)
	{
		if ( IsValid() )
		{
			HANDLE h = ::BeginUpdateResource(m_fileName, FALSE);
			if ( h != NULL )
			{
				bool r = !! ::UpdateResource(h, 
									RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
									m_workMem.Ref(), ToDword(m_workMem.GetSize()));
				r &= !! ::EndUpdateResource(h, FALSE);
				return r;
			}
		}
		return false;
	}

#endif // _WIN32_WCE

protected:

	/**
	 * [通知] GetVersionText コール通知.
	 *	@param[in,out] _str バージョン文字列。 編集することで、 GetVersionText の戻り値を加工できます。
	 *	@param[in] flag フラグ。 VS_FF_DEBUG などのフラグがはいってます。
	 *	@param[in] wLanguage ランゲージ。
	 */
	virtual void OnGetVerisonText(CStr& _str, DWORD flag, WORD wLanguage) const
	{
		if ( (flag & VS_FF_SPECIALBUILD) != 0 )
		{
			_str += _T(" ");
			_str += Get(_T("SpecialBuild"), wLanguage);
		}
		if ( (flag & VS_FF_PRIVATEBUILD) != 0 )
		{
			_str += _T(" ");
			_str += Get(_T("PrivateBuild"), wLanguage);
		}
		if ( (flag & VS_FF_PRERELEASE) != 0 )
		{
			_str += _T(" [PreRelease版]");
		}
		if ( (flag & VS_FF_DEBUG) != 0 )
		{
			_str += _T(" ≪Debug版≫");
		}
	}

private:
	/// 言語コードページ型
	struct TLangAndCodePage
	{
		WORD wLanguage;	///< 言語
		WORD wCodePage;	///< コードページ
	};
	mutable CWorkMem	m_workMem;			///< ワーク
	VS_FIXEDFILEINFO*	m_pFileInfo;		///< ワーク内のFIXEDFILEINFOのポインタ
	TLangAndCodePage*	m_pLangCodePage;	///< ワーク内の言語コードページのポインタ
	UINT				m_langCodePageSize;	///< 言語コードページの数
	CStr				m_fileName;			///< 扱っているファイル名
	/// ポインタ取得
	LPVOID m_GetTextPtr(LPCTSTR lpszType, WORD wLanguage = 0) const
	{
		if ( IsValid() )
		{
			// 各言語とコードページのファイルの説明を読み取る。
			for ( UINT i = 0; i < m_langCodePageSize; i++ )
			{
				if ( wLanguage == 0 || m_pLangCodePage[i].wLanguage == wLanguage )
				{
					CStr strKey;
					strKey.Format(
							_T("\\StringFileInfo\\%04x%04x\\%s"),
							m_pLangCodePage[i].wLanguage,		//Lang
							m_pLangCodePage[i].wCodePage,		//Code
							lpszType);
					// i 番目の言語とコードページのファイルの説明を取得する。
					UINT dmy;
					if ( HasUnicodeType() )
					{
						CUnicode s = strKey;
						LPWSTR P;
						if ( ::VerQueryValueW(m_workMem,
								s.GetBuffer(),
								reinterpret_cast<LPVOID*>(&P),
								&dmy) )
						{
							return P;
						}
					}
					else
					{
						#ifndef _WIN32_WCE
						CAscii s = strKey;
						LPSTR P;
						if ( ::VerQueryValueA(m_workMem,
								s.GetBuffer(),
								reinterpret_cast<LPVOID*>(&P),
								&dmy) )
						{
							return P;
						}
						#endif
					}
				}
			}
		}
		return NULL;
	}

	/**
	 * [確認] UNICODEリソース確認
	 *	@retval true UNICODE リソースだ
	 *	@retval false ASCI リソースか、 OPENしていない。
	 */
	bool HasUnicodeType(void) const
	{
		bool r = false;
		if ( m_pFileInfo != NULL )
		{
			switch ( m_pFileInfo->dwFileOS )
			{
			case VOS_DOS_WINDOWS16:
			case VOS_OS216_PM16:
				break;
			default:
				r = true;
				break;
			}
		}
		return r;
	}

	/// バージョン文字作成
	CStr m_GetVersionText(LPCTSTR lpszVer, WORD wLanguage = 0) const
	{
		CStr s;
		if ( IsValid() )
		{
			s = Get(lpszVer, wLanguage);
			DWORD f = m_pFileInfo->dwFileFlags & m_pFileInfo->dwFileFlagsMask;
			OnGetVerisonText(s, f, wLanguage);
		}
		return s;
	}
};



}; // TNB



#ifdef _TnbDOXYGEN	//Document作成用シンボル

/**
 * ファイルバージョン情報構造体.
 *	@see https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms646997%28v=vs.85%29.aspx
 */
struct VS_FIXEDFILEINFO 
{ 
};

#endif // _TnbDOXYGEN


