#pragma once
/**
 *@file
 * ウィンドウズバージョン関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #define _OSVERPACK(BASE, MAJOR, MINOR, IS64) \
				MAKELONG(MAKEWORD(MINOR, MAJOR), MAKEWORD(BASE, IS64))
#endif



/**@ingroup WINAPI
 * ウィンドウズバージョン情報
 *
 *	@par必要ファイル
 *			TnbWindowsVersion.h
 *
 *	@date 08/03/19 新規作成。
 */
class CWindowsVersion
{
public:
	/// Platformタイプ
	enum EPlatformType
	{
		WIN3_BASE = VER_PLATFORM_WIN32s,			///< Windows 3.1
		WIN9x_BASE = VER_PLATFORM_WIN32_WINDOWS,	///< Windows 9xベース
		WINNT_BASE = VER_PLATFORM_WIN32_NT,			///< Windows NT ベース
	};
	/// OS バージョン
	enum EOsVersion
	{
		UNKNOWN 	= _OSVERPACK(WIN3_BASE,  0,  0, 0),		///< 不明
		WINNT351	= _OSVERPACK(WINNT_BASE, 3, 51, 0),		///< Windows NT 3.51
		WIN95		= _OSVERPACK(WIN9x_BASE, 4,  0, 0),		///< Windows 95
		WIN98		= _OSVERPACK(WIN9x_BASE, 4, 10, 0),		///< Windows 98
		WINME		= _OSVERPACK(WIN9x_BASE, 4, 90, 0),		///< Windows ME
		WINNT4		= _OSVERPACK(WINNT_BASE, 4,  0, 0),		///< Windows NT 4.0
		WIN2000		= _OSVERPACK(WINNT_BASE, 5,  0, 0),		///< Windows 2000
		WINXP		= _OSVERPACK(WINNT_BASE, 5,  1, 0),		///< Windows XP
		WINXP64		= _OSVERPACK(WINNT_BASE, 5,  2, 1),		///< Windows XP (64bit)
		WINVISTA	= _OSVERPACK(WINNT_BASE, 6,  0, 0),		///< Windows Vista
		WIN7		= _OSVERPACK(WINNT_BASE, 6,  1, 0),		///< Windows 7
		WINVISTA_64	= _OSVERPACK(WINNT_BASE, 6,  0, 1),		///< Windows Vista (64bit)
		WIN7_64		= _OSVERPACK(WINNT_BASE, 6,  1, 1),		///< Windows 7 (64bit)
//		WINS2003	= _OSVERPACK(WINNT_BASE, 5,  2, 0),		///< Windows Server 2003
//		WINS2008	= _OSVERPACK(WINNT_BASE, 6,  0, 0),		///< Windows Server 2008
//		WINS2008R2	= _OSVERPACK(WINNT_BASE, 6,  1, 0),		///< Windows Server 2008 R2
	};

	/**
	 * コンストラクタ.
	 *	@note OSバージョン情報を取得しています。
	 */
	CWindowsVersion(void)
	{
		OSVERSIONINFO* P = reinterpret_cast<OSVERSIONINFO*>(&m_info);
		::ZeroMemory(&m_info, sizeof(OSVERSIONINFOEX));
		m_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		if ( ! ::GetVersionEx(P) )
		{
			::ZeroMemory(&m_info, sizeof(OSVERSIONINFOEX));
			m_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			::GetVersionEx(P);
		}
		m_boIsWow64 = m_IsWow64();
	}

	/**
	 * [確認] 64bitチェック
	 *	@retval false 32bitOS
	 *	@retval true 64bitOS
	 */
	bool IsWow64(void) const
	{
		return m_boIsWow64;
	}

	/**
	 * [取得] OSバージョン取得
	 *	@return バージョン
	 */
	EOsVersion GetOsVersion(void) const
	{
		return m_GetOsVersion();
	}

	/**
	 * [取得] マイナーバージョン取得.
	 *	@retval 0	Windows 95
	 *	@retval 10	Windows 98
	 *	@retval 90	Windows Me
	 *	@retval 51	Windows NT 3.51
	 *	@retval 0	Windows NT 4.0
	 *	@retval 0	Windows 2000
	 *	@retval 1	Windows XP
	 *	@retval 2	Windows Server 2003
	 *	@retval 0	Windows 7
	 */ 
	DWORD GetMinorVersion(void) const
	{
		return m_info.dwMinorVersion;
	}

	/**
	 * [取得] メジャーバージョン取得.
	 *	@retval 4	Windows 95 / 98 / Me
	 *	@retval 3	Windows NT 3.51
	 *	@retval 4	Windows NT 4.0
	 *	@retval 5	Windows 2000 / XP / Server 2003
	 *	@retval 6	Windows Vista / 7
	 */
	DWORD GetMajorVersion(void) const
	{
		return m_info.dwMajorVersion;
	}

	/**
	 * [取得] プラットフォーム
	 *	@return プラットフォーム
	 */
	EPlatformType GetPlatform(void) const
	{
		return static_cast<EPlatformType>(m_info.dwPlatformId);
	}

	/**
	 * [取得] 追加文字列.
	 *	@note サービスパックがインストールされている場合は、その情報を示す文字列が格納されます（例：“Service Pack 3”）。
	 *			それ以外の場合は空文字列になります。
	 *	@return 文字列。
	 */
	LPCTSTR GetAdditionalString(void) const
	{
		return m_info.szCSDVersion;
	}

	/**
	 * [取得] サービスパックバージョン
	 *	@return 上位WORDメジャー、下位WORDにマイナーバージョン
	 */
	DWORD GetServicePackVersion(void) const
	{
		return MAKELONG(m_info.wServicePackMinor, m_info.wServicePackMajor);
	}

	/**
	 * [取得] ビルドナンバー
	 *	@return ビルドナンバー
	 */
	DWORD GetBuildNumber(void) const
	{
		return m_info.dwBuildNumber;
	}

	/**
	 * [確認] Windows98 SE チェック
	 *	@retval true Window98SEだ
	 *	@retval false それ以外
	 */
	bool IsWindows98SE(void) const
	{
		if ( m_GetOsVersion() == WIN98 )
		{
			return (m_info.szCSDVersion[0] == 'A' && m_info.szCSDVersion[1] == 0);
		}
		return false;
	}

	/**
	 * [取得] OSバージョン文字列取得.
	 *	@retval NULL 不明バージョン
	 *	@retval NULL以外 文字列
	 */
	LPCTSTR GetOsVersionString(void) const
	{
		if ( IsWindows98SE() )
		{
			return _T("Windows 98SE");
		}
		switch ( m_GetOsVersion() )
		{
		case WINNT351:		return _T("Windows NT 3.51");
		case WIN95:			return _T("Windows 95");
		case WIN98:			return _T("Windows 98");
		case WINME:			return _T("Windows ME");
		case WINNT4:		return _T("Windows NT 4.0");
		case WIN2000:		return _T("Windows 2000");
		case WINXP:			return _T("Windows XP");
		case WINVISTA:		return _T("Windows Vista");
		case WIN7:			return _T("Windows 7");
		case WINXP64:		return _T("Windows XP64");
		case WINVISTA_64:	return _T("Windows Vista 64");
		case WIN7_64:		return _T("Windows 7 64");
		default:
			break;
		}
		return NULL;
	}
private:
	OSVERSIONINFOEX	m_info;		///< バージョン情報
	bool			m_boIsWow64;///< 64bitフラグ
	/// バージョン取得
	EOsVersion m_GetOsVersion(void) const
	{
		EOsVersion v = static_cast<EOsVersion>(
			_OSVERPACK(GetPlatform(), GetMajorVersion(), GetMinorVersion(), m_boIsWow64));
		switch ( v )
		{
		case WINNT351:
		case WIN95:
		case WIN98:
		case WINME:
		case WINNT4:
		case WIN2000:
		case WINXP:
		case WINVISTA:
		case WINXP64:
		case WINVISTA_64:
		case WIN7_64:
			break;
		default:
			v = UNKNOWN;
			break;
		}
		return v;
	}
	/**
	 * [確認] 64bitチェック
	 *	@retval false 32bitOS
	 *	@retval true 64bitOS
	 */
	bool m_IsWow64(void) const
	{
		typedef BOOL (WINAPI *P_ISWOW64PROCESS)(HANDLE, PBOOL);
		P_ISWOW64PROCESS pIsWow64Process 
			= reinterpret_cast<P_ISWOW64PROCESS>(::GetProcAddress(::GetModuleHandleA("kernel32"), "IsWow64Process"));
		BOOL bIsWow64 = FALSE;
		if ( NULL != pIsWow64Process )
		{
			VERIFY( pIsWow64Process(::GetCurrentProcess(), &bIsWow64) );
		}
		return !! bIsWow64;
	}

};



};//TNB

