#pragma once
/**
 *@file
 * 小数点付き数値専用入力Edit関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcEditValue.h"
#include <math.h>
#include <wchar.h>



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 砂時計カーソル.
 *
 *		MFC の CWaitCursor を拡張したものです。
 *
 *	@par必要ファイル
 *			TnbMfcWaitCursor.h
 * 
 *	@date 14/11/19	新規作成
 */
class CWaitCursorEx : CCopyImpossible
{
public:

	/**
	 * コンストラクタ.
	 *		砂時計カーソル化します。
	 */
	CWaitCursorEx(void) : m_pCursor(new CWaitCursor)
	{
	}

	/**
	 * デストラクタ.
	 *		通常のカーソルに戻します。
	 */
	~CWaitCursorEx(void)
	{
		Restore();
	}

	/**
	 * [設定] 変更.
	 *		砂時計カーソル化します。
	 */
	void Change(void)
	{
		Restore();
		m_pCursor = new CWaitCursor();
	}

	/**
	 * [設定] 復元.
	 *		通常のカーソルに戻します。
	 */
	void Restore(void)
	{
		if ( m_pCursor != NULL )
		{
			delete m_pCursor;
			m_pCursor = NULL;
		}
	}

private:
	CWaitCursor*	m_pCursor;	///< インスタンス
};



};
};
