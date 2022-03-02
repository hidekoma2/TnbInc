#pragma once
/**
 *@file
 * テンポラリバイト関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTemporaryCommunicator.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * テンポラリバイトクラス
 *
 *		CTemporaryCommunicator を使用してください。
 *
 *	@par必要ファイル
 *			TnbTemporaryBytes.h
 * 
 *	@date 11/07/06 CTemporaryCommunicator に名前を変更。それに伴い、本クラス名は typedef 化
 */
typedef CTemporaryCommunicator CTemporaryBytes;



}; // TNB

