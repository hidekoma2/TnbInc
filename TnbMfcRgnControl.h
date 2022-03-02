#pragma once
/**
 *@file
 * 領域指定コントロールクラス関係のヘッダ
 *
 *		四角以外のコントロールを作成するためのサポートクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 領域指定コントロールサポートクラス
 *
 *		inline なコントロールです。
 *
 *	@par必要ファイル
 *			TnbMfcRgnControl.h
 * 
 *	@date 07/12/12 抽象化して新規作成
 */
template<typename WND = CWnd>
class CRgnControlAddinT : public WND
{
	DEFSUPER(WND);
protected:

	HRGN			m_clientRgn;		///< クライアントリージョン

	/**
	 * [削除] クライアントリージョン破棄
	 */
	void DeleteClientRgnHandle(void)
	{
		::SetWindowRgn(m_hWnd, NULL, FALSE);
		if ( m_clientRgn != NULL )
		{
			_DeleteObject(m_clientRgn);
			m_clientRgn = NULL;
		}
	}

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
		switch ( message )
		{
		case WM_PRINT:
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				if ( m_clientRgn != NULL )
				{
					HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
					CPoint po = MFCLIB::GetClientPoint(this);
					::CombineRgn(rgn, m_clientRgn, NULL, RGN_COPY);
					::OffsetRgn(rgn, po.x, po.y);
					::SelectClipRgn(dc, rgn);
					LRESULT r = _super::WindowProc(message, wParam, lParam);
					_DeleteObject(rgn);
					return r;
				}
			}
			break;
#ifndef _WIN32_WCE
		case WM_NCPAINT:
			if ( m_clientRgn != NULL )
			{
				wParam = reinterpret_cast<WPARAM>(m_clientRgn);
			}
			break;
#endif
		case WM_DESTROY:
			DeleteClientRgnHandle();
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

public:

	/// コンストラクタ
	CRgnControlAddinT(void) : m_clientRgn(NULL)
	{
	}

	/// デストラクタ
	virtual ~CRgnControlAddinT(void)
	{
		DeleteClientRgnHandle();
	}

	/**
	 * [設定] ウィンドウリージョン設定.
	 *	@param rgn リージョン。
	 *	@retval true 成功。指定した rgn は本オブジェクトが管理するので、破棄しないでください（不必要になったら破棄します）。
	 *	@retval false 失敗。指定した rgn は有効なので、 DeleteObject する必要があります。
	 */
	bool SetClientRgn(HRGN rgn)
	{
		DeleteClientRgnHandle();
		m_clientRgn = ::CreateRectRgn(0, 0, 0, 0);
		::CombineRgn(m_clientRgn, rgn, NULL, RGN_COPY);
		bool r = !! ::SetWindowRgn(m_hWnd, rgn, TRUE);
		if ( ! r )
		{
			DeleteClientRgnHandle();
		}
		return r;
	}
};



}; //MFC
}; //TNB



