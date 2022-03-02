#pragma once
/**
 *@file
 * イベントロガー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSingleton.h"
#include "TnbStrEx.h"
#include "TnbRegistryAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup LOG
 * イベントロガー
 * 
 *		システムのイベントに記録します。
 *
 *	@note シングルトンで作成されています。
 *
 *	@par ★必須★ プロジェクトの rc2 ファイルに以下を追加する
 *		\code
 *		#include "TnbEventLogger.rc"
 *		\endcode
 *
 *	@par使用例
 *		\code
 *		CEventLogger* P = CEventLogger::GetInstance();
 *		P->SetSourceName(_T("TnbTestProg"));
 *		P->RegistEventSource();
 *		int i;		
 *
 *				;
 *
 *		P->Information(4, "ABCD", _T("ABC %d D"), i);	// データ付き情報、出力
 *		P->Warning(0, _T("ABCD"));						// 警告、出力
 *
 *				;
 *
 *		P->UnregistEventSource();
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbEventLogger.h
 *
 *	@date 09/11/02	新規作成
 */
class CEventLogger
{
	/// コンストラクタ
	SINGLETON_CONSTRUCTOR(CEventLogger)
	{
		m_sourceName = GetProcessName();
	}
public:

	/**
	 * [設定] ソース名設定.
	 *	@note 設定しない場合、実行ファイル名がソース名になります。
	 *	@note RegistEventSource() を実行する前にこのメソッドを使用してください。
	 *	@param lpszSourceName ソース名。
	 */
	void SetSourceName(LPCTSTR lpszSourceName)
	{
		m_sourceName = lpszSourceName;
	}

	/**
	 * [取得] ソース名取得.
	 *	@note SetSourceName() で設定していない場合、実行ファイル名がソース名になります。
	 *	@return ソース名。
	 */
	LPCTSTR GetSourceName(void) const
	{
		return m_sourceName;
	}

	/**
	 * [登録] イベントソース登録.
	 *		ソース名をイベントソースに登録します。登録しないと、イベントビュワーで「説明」が正しく表示されません。
	 *	@note イベントビュワーが開いていると、登録したモジュールは削除できなくなります。
	 *	@note レジストリに登録するため、クラス終了後も、登録されたままになります。解除する場合、 UnregistEventSource() を使用します。
	 *	@param lpszModuleName イベントソースをもつ、モジュール名( exe / dll 名)を指定します。省略すると、自分の exe になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool RegistEventSource(LPCTSTR lpszModuleName = NULL)
	{
		HMODULE h = NULL;
		if ( lpszModuleName != NULL )
		{
			h = ::LoadLibrary(lpszModuleName);
		}
		CStr path;
		DWORD dwRc = ::GetModuleFileName(h, path.GetBuffer(MAX_PATH), MAX_PATH);
		path.ReleaseBuffer();
		if ( dwRc > 0 )
		{
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_KeyName());
			CRegistryAccessor::CSection sec = reg[m_sourceName];
			sec.WriteString(_T("EventMessageFile"), path);
		}
		return (dwRc > 0);
	}

	/**
	 * [登録] イベントソース登録解除.
	 *	@note RegistEventSource() 実行後、クラス終了後も、登録されたままになります。解除ために、このメソッドを使用します。
	 *	@note 解除する前に、 SetSourceName() でソース名を変えないようにしてください。
	 */
	void UnregistEventSource(void)
	{
		if ( ! m_sourceName.IsEmpty() )
		{
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_KeyName());
			reg.DeleteSection(m_sourceName);
		}
	}

	/**
	 * [出力] 情報出力.
	 *	@param eventId イベントID。 0～31 で指定します。
	 *	@param lpszFmt 書式指定文字列。 printf() 準拠です。
	 *	@param ... 可変個引数。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Information(DWORD eventId, LPCTSTR lpszFmt, ...) const
	{
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_INFORMATION_TYPE, eventId, 0, NULL, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [出力] 情報出力(データ付き).
	 *	@param eventId イベントID。 0～31 で指定します。
	 *	@param dataSize データサイズ。
	 *	@param lpData データ。
	 *	@param lpszFmt 書式指定文字列。 printf() 準拠です。
	 *	@param ... 可変個引数。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Information(DWORD eventId, size_t dataSize, LPCVOID lpData, LPCTSTR lpszFmt, ...) const
	{
		ASSERT( ! ::IsBadReadPtr(lpData, dataSize) );
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_INFORMATION_TYPE, eventId, dataSize, lpData, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [出力] 警告出力.
	 *	@param eventId イベントID。 0～31 で指定します。
	 *	@param lpszFmt 書式指定文字列。 printf() 準拠です。
	 *	@param ... 可変個引数。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Warning(DWORD eventId, LPCTSTR lpszFmt, ...) const
	{
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_WARNING_TYPE, eventId, 0, NULL, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [出力] 警告出力(データ付き).
	 *	@param eventId イベントID。 0～31 で指定します。
	 *	@param dataSize データサイズ。
	 *	@param lpData データ。
	 *	@param lpszFmt 書式指定文字列。 printf() 準拠です。
	 *	@param ... 可変個引数。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Warning(DWORD eventId, size_t dataSize, LPCVOID lpData, LPCTSTR lpszFmt, ...) const
	{
		ASSERT( ! ::IsBadReadPtr(lpData, dataSize) );
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_WARNING_TYPE, eventId, dataSize, lpData, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [出力] エラー出力.
	 *	@param eventId イベントID。 0～31 で指定します。
	 *	@param lpszFmt 書式指定文字列。 printf() 準拠です。
	 *	@param ... 可変個引数。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Error(DWORD eventId, LPCTSTR lpszFmt, ...) const
	{
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_ERROR_TYPE, eventId, 0, NULL, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [出力] エラー出力(データ付き).
	 *	@param eventId イベントID。 0～31 で指定します。
	 *	@param dataSize データサイズ。
	 *	@param lpData データ。
	 *	@param lpszFmt 書式指定文字列。 printf() 準拠です。
	 *	@param ... 可変個引数。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Error(DWORD eventId, size_t dataSize, LPCVOID lpData, LPCTSTR lpszFmt, ...) const
	{
		ASSERT( ! ::IsBadReadPtr(lpData, dataSize) );
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_ERROR_TYPE, eventId, dataSize, lpData, lpszFmt, args);
		va_end(args);
		return r;
	}

private:
	// キー名
	LPCTSTR m_KeyName(void) const
	{
		return _T("SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application");
	}
	// 書き込み
	bool m_Write(WORD type, DWORD eventId, size_t binarySize, LPCVOID lpBinary, LPCTSTR lpszFmt, va_list V) const
	{
		bool r = false;
		CStr str;
		str.FormatV(lpszFmt, V);
		LPCTSTR lpszMsg = str;
		HANDLE h = ::RegisterEventSource(NULL, m_sourceName);
		if ( h == NULL )
		{
			_GetLastError("RegisterEventSource");
			return false;
		}
		r = !! ::ReportEvent(h, type, 0, eventId, NULL, 1, static_cast<DWORD>(binarySize), &lpszMsg, const_cast<LPVOID>(lpBinary));
		if ( ! r )
		{
			_GetLastError("ReportEvent");
		}
		::DeregisterEventSource(h);
		return r;
	}

	CStr		m_sourceName;	///< ソース名
};



}; // TNB


