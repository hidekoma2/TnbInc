#pragma once
/**
 *@file
 * 入力Desktop名関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * 入力Desktop名クラス
 *
 *		入力Desktopの名前を扱うクラスです。
 *
 *	@par必要ファイル
 *			TnbInputDesktopName.h
 *
 *	@date 08/04/17 新規作成
 */
class CInputDesktopName
{
public:
	/// コンストラクタ
	CInputDesktopName(void)
	{
		TCHAR name[100] = {0};
		DWORD w;
		HDESK h = ::GetThreadDesktop(::GetCurrentThreadId());
		::GetUserObjectInformation(h, UOI_NAME, name, 100, &w);
		m_currentDesktopName = name;
	}

	/**
	 * [取得] カレントDesktop名取得
	 *	@return カレントDesktop名前。
	 */
	LPCTSTR GetCurrentDesktopName(void) const
	{
		return m_currentDesktopName;
	}

	/**
	 * [取得] 現在の入力Desktop名取得
	 *	@param[out] _str 現在の入力Desktop名前が格納されます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetNowInputDesktopName(CSimpleStr& _str) const
	{
		TCHAR name[100] = {0};
		DWORD w;
		HDESK h = ::OpenInputDesktop(0, FALSE, DESKTOP_ENUMERATE);
		BOOL r = ::GetUserObjectInformation(h, UOI_NAME, name, 100, &w);
		::CloseDesktop(h);
		_str = name;
		return !! r;
	}

	/**
	 * [確認] カレントが現在の入力Desktopになっているか
	 *	@retval true なっている
	 *	@retval false なっていない
 	 */
	bool IsEqualThreadDesktop(void) const
	{
		CSimpleStr str;
		if ( GetNowInputDesktopName(str) )
		{
			return _tcscmp(str, m_currentDesktopName) == 0;
		}
		return false;
	}

private:
	CSimpleStr m_currentDesktopName;
};



}; // TNB

