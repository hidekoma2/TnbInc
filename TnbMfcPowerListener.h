#pragma once
/**
 *@file
 * 電源管理通知処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



#ifndef __AFX_H__
 #error
#endif



//TNB Library
namespace TNB {
namespace MFC {


/**@ingroup WINAPI
 * 電源管理アドインクラス
 *
 *		本クラスを 使用することで、 コールバック関数による応答が可能になります。
 *
 *	@note シャットダウンは「 CPowerManager::Shutdown(); 」で実行します。
 *	@note サスペンドは「 CPowerManager::Suspend(); 」で実行します。
 *
 *	@par必要ファイル
 *			TnbMfcPowerListener.h
 *
 *	@date 06/10/24；新規作成。
 */
template<typename TYP = CDialog>
class CPowerListenerAddinT : public TYP
{
	DEFSUPER(TYP);
protected:

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == WM_QUERYENDSESSION )
		{
			return CanLogoff();
		}
		else if ( message == WM_ENDSESSION )
		{
			if ( wParam != 0 )
			{
				OnLogoff();
			}
			else
			{
				OnCancelLogoff();
			}
			return 0;
		}
		else if ( message == WM_POWERBROADCAST )
		{
			switch ( wParam )
			{
			case PBT_APMQUERYSUSPEND:	// サスペンド問い合わせ
				return CanSuspend() ? TRUE : BROADCAST_QUERY_DENY;
			case PBT_APMQUERYSUSPENDFAILED:	//サスペンドキャンセル
				OnCancelSuspend();
				break;
			case PBT_APMSUSPEND:		//サスペンド開始。拒否できません。
				OnSuspend();
				break;
			case PBT_APMRESUMESUSPEND:	//レジューム。
				OnResumeSuspend();
				break;
			}
			return 0;
		}
		return _super::WindowProc(message, wParam, lParam);
	}
public:

	/// コンストラクタ
	CPowerListenerAddinT(void) : _super()
	{
	}

	/**
	 * コンストラクタ
	 *	@param lpszTemplateName ダイアログ ボックス テンプレート リソース名を表す null で終わる文字列。
	 *	@param pParentWnd ダイアログ オブジェクトを所有する CWnd 型の親ウィンドウ
	 *					またはオーナー ウィンドウオブジェクトへのポインタ。
	 *					NULL の場合は、ダイアログ オブジェクトの親ウィンドウがアプリケーションのメイン ウィンドウになります。
	 */
	CPowerListenerAddinT(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
		: _super(lpszTemplateName, pParentWnd)
	{
	}

	/**
	 * コンストラクタ
	 *	@param nIDTemplate ダイアログ ボックス テンプレート リソースの ID 番号。
	 *	@param pParentWnd ダイアログ オブジェクトを所有する CWnd 型の親ウィンドウ
	 *					またはオーナー ウィンドウオブジェクトへのポインタ。
	 *					NULL の場合は、ダイアログ オブジェクトの親ウィンドウがアプリケーションのメイン ウィンドウになります。
	 */
	CPowerListenerAddinT(UINT nIDTemplate, CWnd* pParentWnd = NULL) 
		: _super(nIDTemplate, pParentWnd)
	{
	}

	/// デストラクタ
	virtual ~CPowerListenerAddinT(void)
	{
		::SetThreadExecutionState(ES_CONTINUOUS);
	}

	/**
	 * [設定] サスペンドモード保護
	 *	@note 無操作時間によりサスペンドに入るのを防ぎます。
	 */
	void SaveSuspendMode(void)
	{
		::SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS);
	}
	
	/**
	 * [設定] サスペンドモード保護解除
	 */
	void NormalSuspendMode(void)
	{
		::SetThreadExecutionState(ES_CONTINUOUS);
	}
	
	/**
	 * [通知] ログOFF可能か？.
	 *		電源OFF時など、 ログOFFが行われる前に問い合わせが来ます。
	 *	@retval true ログOFF可能.
	 *	@retval false 不可能.
	 */
	virtual bool CanLogoff(void) = 0;

	/**
	 * [通知] ログOFFキャンセル.
	 *		ログOFF処理が中断された時に通知されます。
	 */
	virtual void OnCancelLogoff(void) = 0;

	/**
	 * [通知] ログOFF開始.
	 *		ログOFF処理が開始される直前に通知されます。
	 */
	virtual void OnLogoff(void) = 0;

	/**
	 * [通知] サスペンド可能か？.
	 *		サスペンド、ハイバネーションが行われる前に問い合わせが来ます。
	 *	@retval true サスペンド可能.
	 *	@retval false 不可能.
	 */
	virtual bool CanSuspend(void) = 0;

	/**
	 * [通知] サスペンドキャンセル.
	 *		サスペンド、ハイバネーションが中断された時に通知されます。
	 */
	virtual void OnCancelSuspend(void) = 0;

	/**
	 * [通知] サスペンド開始.
	 *		サスペンド、ハイバネーションが開始される直前に通知されます。
	 */
	virtual void OnSuspend(void) = 0;

	/**
	 * [通知] レジューム.
	 *		サスペンド、ハイバネーションから復帰した時に通知されます。
	 */
	virtual void OnResumeSuspend(void) = 0;
};



}; //MFC
}; //TNB


