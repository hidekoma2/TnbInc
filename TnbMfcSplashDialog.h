#pragma once
/**
 *@file
 * スプラッシュダイアログ関係ヘッダ ファイル
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcModelessDialog.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * スプラッシュウィンドウ管理テンプレート.
 *	
 *		指定のダイアログベースウィンドウを Modeless で表示します。
 *		Show() , Hide() , Destroy() メソッドにより容易に表示(作成)、非表示、破棄できるようになります。
 *
 *	@param DLG MFC の CDialog を継承したクラス型
 *	@param HASTASK true ならタスクバーに現れます(def.)。 false なら現れません。
 *
 *	@note CModelessDialogT を使用してください。
 *
 *	@par必要ファイル
 *			TnbMfcSplashDialog.h
 */
template<typename DLG, bool HASTASK = true>
class CSplashDialogT : public CModelessDialogT<DLG, HASTASK>
{
};



}; // MFC
}; // TNB
