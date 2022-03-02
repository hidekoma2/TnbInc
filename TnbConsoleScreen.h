#pragma once
/**
 *@file
 * コンソール画面関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"
#include <io.h>
#include <fcntl.h>



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * コンソール画面管理クラス.
 *
 *		コンソール画面を開き、文字を表示することが出来ます。
 *
 *	@note 主に開発用を想定しています。
 *
 *	@todo 現在、表示のみサポートします。テキストコピーも出来ません。
 *
 *	@par必要ファイル
 *			TnbConsoleScreen.h
 *
 *	@date 09/08/25 新規作成
 */
class CConsoleScreen : CCopyImpossible
{
public:

	/// コンストラクタ
	CConsoleScreen(void) : m_console(-1)
	{
	}

	/// デストラクタ
	~CConsoleScreen(void)
	{
		Close();
	}

	/**
	 * [構築] コンソールオープン.
	 *	@note 既に開いていたら一度閉じます。
	 *	@retval true 成功
	 *	@retval false 失敗。
	 */
	bool Open(void)
	{
		Close();
		::AllocConsole();
		HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
		m_console = ::_open_osfhandle(reinterpret_cast<INT_PTR>(h), _O_TEXT);
		if ( m_console < 0 )
		{
			::FreeConsole();
			return false;
		}
		*stdout = *::_fdopen(m_console, "w");
		::setvbuf(stdout, NULL, _IONBF, 0);
		return true;
	}

	/**
	 * [破棄] コンソールクローズ.
	 *	@note 開いていなければ、何もしません。
	 */
	void Close(void)
	{
		if ( m_console >= 0 )
		{
			_close(m_console);
			::FreeConsole();
			m_console = -1;
		}
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン
	 *	@retval false クローズ
	 */
	bool IsOpened(void) const
	{
		return m_console >= 0;
	}

	/**
	 * [表示] 文字列表示.
	 *	@param lpFmt 文字列.
	 *	@param ... パラメータ
	 *	@retval マイナス エラー。
	 *	@retval プラス 成功。数値は書き込まれた文字数。
	 */
	int Print(LPCTSTR lpFmt, ...)
	{
		int r = -1;
		if ( IsOpened() )
		{
			va_list args;
			va_start(args, lpFmt);
			r = _vftprintf(stdout, lpFmt, args);
			va_end(args);
		}
		return r;
	}

private:
	int		m_console;
};



}; // TNB
