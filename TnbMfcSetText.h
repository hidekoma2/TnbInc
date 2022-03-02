#pragma once
/**
 *@file
 * 文字列設定関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 文字列設定サポートクラス
 *
 *		他のスレッドからも問題なく SetWindowText できるようにするアドインです。
 *
 *	@note SetWindowText の直後に、 GetWindowText() を行うと、期待の文字列が取得できないことが
 *		あります(PostMessageで書き込んでいるために、遅延があるためです)。
 *
 *	@par使用例
 *		\code
 *		//= 宣言
 *		typedef CSetTextAddinT<CStatic> CStaticSt;
 *				;
 *		class CXXXX : public CDialog
 *		{
 *				;
 *			CStaticSt	m_staticText1;		// <- 元々 CStaticだったメンバを書き換えた
 *				;
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbMfcSetText.h
 * 
 *	@date 09/11/05	新規作成
 */
template<typename WND = CStatic>
class CSetTextAddinT : public WND
{
	DEFSUPER(WND);
public:
	
	/// コンストラクタ
	CSetTextAddinT(void) : m_postCount(0), m_isValid(false)
	{
	}

	/**
	 * [設定] 文字列設定.
	 *	@note 別スレッドから使用すると、 Postメッセージを投げます。。
	 *	@param lpszString 文字列。
	 */
	void SetWindowText(LPCTSTR lpszString)
	{
		if ( ::IsWindow(m_hWnd) && m_isValid )
		{
			TTRACE2("SetWindowText tid = %X, %X\n", m_mainThreadId, ::GetCurrentThreadId());
			if ( m_mainThreadId != ::GetCurrentThreadId() )
			{
				CString* P = new CString(lpszString);
				m_postCount++;
				if ( ! _super::PostMessage(WM_TNB_POSTTEXT, 0, reinterpret_cast<LPARAM>(P)) )
				{
					delete P;
					m_postCount--;
				}
				return;
			}
			_super::SetWindowText(lpszString);
		}
	}

protected:

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *	@note CWnd のメソッドをオーバーライドしています。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_TNB_POSTTEXT:
			{
				CString* P = reinterpret_cast<CString*>(lParam);
				if ( ::IsWindow(m_hWnd) )
				{
					_super::SetWindowText(P->operator LPCTSTR());
				}
				delete P;
				m_postCount--;
				return 0;
			}
			break;
		case WM_DESTROY:
			{
				MSG msg;
				m_isValid = false;
				while ( ::PeekMessage(&msg, m_hWnd, WM_TNB_POSTTEXT, WM_TNB_POSTTEXT, PM_REMOVE) )
				{
					if ( msg.message == WM_TNB_POSTTEXT )
					{
						CString* P = reinterpret_cast<CString*>(msg.lParam);
						delete P;
						m_postCount--;
					}
				}
				ASSERT( m_postCount == 0 );
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_isValid = true;
		m_mainThreadId = ::GetCurrentThreadId();
		_super::PreSubclassWindow();
	}

private:
	enum { WM_TNB_POSTTEXT = WM_APP };
	size_t			m_postCount;		///< 挿入中カウンタ
	DWORD			m_mainThreadId;		///< スレッドID
	bool			m_isValid;			///< 有効フラグ
};



}; // MFC
}; // TNB
