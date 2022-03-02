#pragma once
/**
 *@file
 * 期間管理関係のヘッダ
 *
 *		二つの時間をペアで管理するクラスが有ります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbRange.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * 期間管理クラス
 *
 *		二つの時間をペアで管理するクラスです。
 *
 *	@note First値,Last値は CFileTimeEx を使用します。
 *
 *	@note Diff値 は LONGLONG を使用します。単位はミリ秒になります。
 *
 *	@par必要ファイル
 *			TnbRangeTime.h
 *
 *	@date 06/10/13	新規作成
 */
typedef CRangeT<CFileTimeEx,LONGLONG> CRangeTime;



}; /// TNB

