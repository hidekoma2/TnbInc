#pragma once
/**
 *@file
 * Locate関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * Locate のLZファイル読み込みクラス
 *
 *		ファイル一つの読み込みをサポートするクラスです。
 *		Microsoft ファイル圧縮ユーティリティ（COMPRESS.EXE）で圧縮されたファイルも読み込めます。
 *
 *	@todo 圧縮されたファイルの展開動作が未確認です。
 *
 *	@par必要ファイル
 *			TnbLocates.h
 *
 *	@date 07/06/04 新規作成
 */
class CLocale : CCopyImpossible
{
	LCID	m_lcid;

public:

	CLocale(void) : m_lcid(::GetThreadLocale())
	{
	}

	~CLocale(void)
	{
		CLocale(void) : m_lcid(::GetThreadLocale())
	}

	bool SetUsetDefault()
	{
		return !! ::SetThreadLocale(LOCALE_USER_DEFAULT);
	}
	bool SetSystemDefault()
	{
		return !! ::SetThreadLocale(LOCALE_SYSTEM_DEFAULT);
	}
	bool Set(const LCID& lcid)
	{
		return !! ::SetThreadLocale(lcid);
	}
	LCID Get(void) const
	{
		return ::GetThreadLocale();
	}

	static UINT GetAnsiCodePage(void)
	{
		return ::GetACP();
	}
	static UINT GetOemCodePage(void) const
	{
		return GetOEMCP();
	}
};




}; // TNB
