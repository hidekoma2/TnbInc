#pragma once
/**
 *@file
 * ツールチップル処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleMap.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ツールチップ制御クラス
 *
 *		CDialog ベースのクラスにアドインすることで、ツールチップ制御を容易にします。
 *
 *		これを使うと、コントロールIDと同じ文字列リソースがツールチップとして表示されます
 *		（また、リソースとは別の文字列も表示可能）。
 *
 *	@param TYP 要素の型。 CDialog のサブクラスである必要があります。
 *
 *	@par使用例
 *		<table><tr><td><pre> 
 *
 *		</pre></td></tr></table>
 *		
 *	@par必要ファイル
 *			TnbMfcToolTip.h
 * 
 *	@date 09/04/13	新規作成
 *	@date 09/04/22	CSimpleVector から CSimpleMap に変更。
 */
template<typename TYP = CDialog>
class CToolTipAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// コンストラクタ
	CToolTipAddinT(UINT id, CWnd* pParent) : _super(id, pParent), m_pActiveWnd(NULL), m_activeId(-1)
	{
	}

	/**
	 * [参照] ツールチップコントロール参照.
	 *	@return ツールチップコントロール.
	 */
	CToolTipCtrl& ReferToolTipCtrl(void)
	{
		m_TipCreate();
		return m_toolTip;
	}

	/**
	 * [追加] ツールチップ追加.
	 *	@param P ウィンドウ
	 *	@param lpsz 表示する文字列。省略すると、 P のコントロールID と同じIDの文字列リソースを使います。
	 */
	void AddToolTip(CWnd* P, LPCTSTR lpsz = NULL)
	{
		if ( P != NULL )
		{
			m_TipCreate();
			m_toolTip.AddTool(P);
		}
		m_strMap[P->GetDlgCtrlID()] = lpsz;
	}

	/**
	 * [追加] ツールチップ追加.
	 *	@param ctrlId コントロール ID。
	 *	@param lpsz 表示する文字列。省略すると、 ctrlId と同じIDの文字列リソースを使います。
	 */
	void AddToolTip(UINT ctrlId, LPCTSTR lpsz = NULL)
	{
		CWnd* P = GetDlgItem(ctrlId);
		if ( P != NULL )
		{
			m_TipCreate();
			m_toolTip.AddTool(P);
		}
		m_strMap[ctrlId] = lpsz;
	}

protected:
	/**
	 * [通知] for translating Windows messages in main message pump
	 *	@param pMsg メッセージ
	 *	@retval TRUE ディスパッチできない場合
	 *	@retval FALSE ディスパッチする必要がある場合
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		m_toolTip.RelayEvent(pMsg);
		return _super::PreTranslateMessage(pMsg);
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
		case WM_INITDIALOG:
			m_TipCreate();
			m_toolTip.Activate(TRUE);
//			m_toolTip.AddTool(this);
			break;
		case WM_DESTROY:
			m_toolTip.DestroyWindow();
			break;
		case 0x36A:
			if ( m_pActiveWnd != NULL )
			{
				INT_PTR i = m_TestNowPosItem(m_pActiveWnd);
				if ( i < 0 )
				{
					i = m_pActiveWnd->GetDlgCtrlID();
				}
				if ( i != m_activeId )
				{
					m_toolTip.Pop();
				}
			}
			break;
		case WM_NOTIFY:
			{
				NMHDR* pNMHDR = reinterpret_cast<NMHDR*>(lParam);
				if ( pNMHDR->code == TTN_POP )
				{
					m_pActiveWnd = NULL;
					m_activeId = -1;
				}
				else if ( pNMHDR->code == TTN_NEEDTEXT )
				{
					UINT_PTR id = pNMHDR->idFrom;
					TOOLTIPTEXT* P = reinterpret_cast<TOOLTIPTEXT *>(pNMHDR);
					if ( (P->uFlags & TTF_IDISHWND) != 0 )
					{
						CWnd* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(id));
						INT_PTR i = m_TestNowPosItem(pWnd);
						if ( i < 0 )
						{
							id = pWnd->GetDlgCtrlID();
						}
						else
						{
							id = i;
						}
						if ( id != 0 )
						{
							CSimpleStr s;
							if ( m_Find(s, id) )
							{
								m_workStr = s;
								P->lpszText = m_workStr;
								P->hinst = 0;
							}
							else
							{
								P->lpszText = MAKEINTRESOURCE(id);
								P->hinst = AfxGetResourceHandle();
							}
							m_pActiveWnd = pWnd;
							m_activeId = id;
							return TRUE ;
						}
					}
				}
			}
			break;
		default:
//			TRACE1("msg = 0x%X\n", message);
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	CToolTipCtrl						m_toolTip;		///< ツールチップス
	CSimpleStr							m_workStr;		///< ワーク用
	CSimpleMapT<UINT_PTR, CSimpleStr>	m_strMap;			///< 文字列
	CWnd*								m_pActiveWnd;	///< Tip表示中対象Window
	INT_PTR								m_activeId;		///< Tip表示中ID
	/// TIPコントロール作成
	void m_TipCreate(void)
	{
		if ( ! ::IsWindow(m_toolTip) )
		{
			m_toolTip.Create(this, TTS_ALWAYSTIP);
		}
	}
	/// 現在の座標のID
	INT_PTR m_TestNowPosItem(CWnd* pWnd)
	{
		TOOLINFO ti = { sizeof(TOOLINFO) };
		POINT po = { 0, 0 };
		::GetCursorPos(&po);
		pWnd->ScreenToClient(&po);
		return pWnd->OnToolHitTest(po, &ti);
	}
	/// 検索
	bool m_Find(CSimpleStr&_s, UINT_PTR id)
	{
		if ( m_strMap.HasKey(id) )
		{
			_s = m_strMap[id];
			return ! _s.IsEmpty();
		}
		return false;
	}
};



}; //MFC
}; //TNB



