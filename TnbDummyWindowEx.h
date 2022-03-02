#pragma once
/**
 *@file
 * 拡張ダミーウィンドウ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDummyWindow.h"
#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * 拡張ダミーウィンドウクラス
 *
 *		簡易的なウィンドウを作成します。 MFC を使用していません。
 *		また、メッセージキューを別スレッドに作成します。
 *
 *	@par必要ファイル
 *			TnbDummyWindowEx.h
 *
 *	@date 11/08/31	新規作成
 *	@date 11/09/07	VC++2005 対応
 */
class CDummyWindowEx : public CDummyWindow
{
	DEFSUPER(CDummyWindow);
public:

	/**
	 * コンストラクタ.
	 *	@param cmc 内部で使用するメッセージコード。省略すると 0xBFFF が使用されます。
	 */
	CDummyWindowEx(UINT cmc = 0xBFFF)
	{
		m_in.m_pPartner = this;
		m_in.m_createMessage = cmc;
	}

	/**
	 * [操作] ウィンドウの作成（テンプレート使用）.
	 *		リソースのテンプレートを使用してウィンドウを作成します。
	 *	@note 一つのインスタンスで管理できるウィンドウは一つだけです。
	 *		複数作成しようとすると、先のウィンドウが破棄されます。
	 *	@param I リスナー
	 *	@param templateId ダイアログのリソース点テンプレートＩＤを指定します。
	 *	@param hParent ダミー。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool CreateByTemplate(CDummyWindowEx::IListener* I, UINT templateId, HWND hParent = NULL)
	{
		Destroy();
		m_in.m_pListener = I;
		m_in.m_templateId = templateId;
		m_thread.SetListener(&m_in);
		#ifdef _DEBUG
			m_thread.Start(_T("CDummyWindowEx::MessagePump"));
		#else
			m_thread.Start();
		#endif
		m_thread.PostMessage(m_in.m_createMessage);
		loop ( i, 10 )
		{
			if ( IsWindow() )
			{
				return true;
			}
			::Sleep(100);
		}
		Destroy();
		return false;
	}

	/**
	 * [操作] ウィンドウの破棄.
	 */
	void Destroy(void)
	{
		m_thread.Stop();
	}

private:
	/// 現在、未サポートです。
	bool Create(CDummyWindow::IListener* I, LPCTSTR lpszTitle = NULL, LPCTSTR lpszClassName = NULL, DWORD dwStyle = WS_OVERLAPPEDWINDOW, HMENU hMenu = NULL, HWND hParent = NULL);
	/// 現在、未サポートです。
	void SetBrush(HBRUSH b);

	/// 内部管理
	class CInner : public CThreadDispatch::IListener
	{
	public:
		CDummyWindowEx*				m_pPartner;			///< パートナー
		CDummyWindow::IListener*	m_pListener;		///< リスナー記憶用
		UINT						m_templateId;		///< テンプレートＩＤ
		UINT						m_createMessage;	///< 作成メッセージコード
		/**
		 * [通知] 開始通知.
		 *		受信スレッドが開始されると、通知されます。
		 *	@note CThreadDispatch::Start() から返る前に作成されたスレッド上から呼ばれます。
		 */
		virtual void OnThreadDispatchSetup(void) 
		{
		}
		/**
		 * [通知] メッセージ受信.
		 *		最後は必ず nMsg に WM_QUIT が入って通知されます。
		 *	@param nMsg メッセージ
		 *	@param wParam WPARAM
		 *	@param lParam LPARAM
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			if ( nMsg == m_createMessage )
			{
				m_pPartner->_super::CreateByTemplate(m_pListener, m_templateId, NULL);
			}
			else if ( nMsg == WM_QUIT )
			{
				m_pPartner->_super::Destroy();
			}
			return true;
		}
	};

	CThreadDispatch		m_thread;	///< メッセージキュー
	CInner				m_in;		///< 内部管理
};



};///TNB
