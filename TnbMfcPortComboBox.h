#pragma once
/**
 *@file
 * シリアルポート選択コンボボックス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbRs232c.h"
#include "TnbSimpleMap.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * シルアルポートComboBoxコントロール
 *
 *
 *
 *	@par使い方
 *		リソースエディタでコンボボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CComboBox）を本クラスに置き換えて使用します。\n
 *		コンボボックスのプロパティは、ドロップダウンリストにしておいてください。
 *	
 *	@par使用例
 *	\code
 *		#include <dbt.h>
 *
 *		class CFooDlg : public CDialog
 *		{
 *				:
 *			CPortComboBox m_comboPort;
 *				:
 *			// 生成されたメッセージ マップ関数
 *			//{{AFX_MSG(CGpsReporterDlg)
 *				:
 *			afx_msg BOOL OnDeviceChange(UINT, DWORD);
 *			//}}AFX_MSG
 *			DECLARE_MESSAGE_MAP()
 *		}
 *
 *
 *		BEGIN_MESSAGE_MAP(CFooDlg, CDialog)
 *			//{{AFX_MSG_MAP(CFooDlg)
 *				:
 *			ON_WM_DEVICECHANGE()
 *				:
 *			//}}AFX_MSG_MAP
 *		END_MESSAGE_MAP()
 *
 *		BOOL CFooDlg::OnInitDialog()
 *		{
 *				:
 *			m_comboPort.ResetDevice();
 *				:
 *		}
 *
 *		BOOL CFooDlg::OnDeviceChange(UINT type, DWORD data)
 *		{
 *			if ( type == DBT_DEVICEARRIVAL || type == DBT_DEVICEREMOVECOMPLETE )
 *			{
 *				CWaitCursor aa;
 *				m_comboPort.ResetDevice();
 *			}
 *			return TRUE;
 *		}
 *
 *	\endcode
 *
 *	@par必要ファイル
 *			TnbMfcPortComboBox.h
 * 
 *	@date 10/02/18	新規作成
 *	@date 14/07/14	AddExtendPort() を新規追加
 *	@date 18/11/05	IsEffectiveness() を新規追加
 */
class CPortComboBox : public CComboBox
{
	DEFSUPER(CComboBox);
public:

	/**
	 * コンストラクタ.
	 *	@param isFullText true なら”製造者 description (COMポート番号)”になります。省略すると ”COMポート番号” になります。
	 */
	CPortComboBox(bool isFullText = false) : m_isFullText(isFullText)
	{
	}

	/**
	 * [設定] 拡張ポート名設定.
	 *	@note この名前を設定すると選択肢に現れ、Port 0 として機能するようになります。
	 *	@attention このメソッドを使った場合、必ず選択されているポートが０か否か確認してからシリアルポートを使用してください。
	 */
	void AddExtendPort(LPCTSTR lpszName)
	{
		m_extPort = lpszName;
	}

	/**
	 * [取得] ポート情報取得.
	 *		選択しているポートを返します。
	 *	@return ポート
	 */
	operator CRs232c::CPort(void) const
	{
		return GetCurPort();
	}

	/**
	 * [取得] ポート情報取得.
	 *		選択しているポートを返します。
	 *	@return ポート
	 */
	int GetCurPort(void) const
	{
		int f = _super::GetCurSel();
		if ( f >= 0 )
		{
			f = _super::GetItemData(f);
		}
		return f;
	}

	/**
	 * [選択] ポート選択.
	 *		指定のポートを選択します。
	 *	@param port ポート
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetCurPort(int port)
	{
		loop ( i, _super::GetCount() )
		{
			if ( ToInt(_super::GetItemData(i)) == port )
			{
				bool r = (_super::SetCurSel(i) != CB_ERR);
				if ( r )
				{
					m_toolTip.UpdateTipText(m_descList[_super::GetItemData(_super::GetCurSel())], this);
				}
				return r;
			}
		}
		return false;
	}

	/**
	 * [選択] 選択.
	 *	@param nSelect 選択する文字列を 0 から始まるインデックスで指定します。
	 *	@retval CB_ERR以外 成功。
	 *	@retval CB_ERR  失敗。
	 */
	int SetCurSel(int nSelect)
	{
		int r = _super::SetCurSel(nSelect);
		int f = _super::GetCurSel();
		if ( f >= 0 )
		{
			f = _super::GetItemData(f);
			m_toolTip.UpdateTipText(m_descList[f], this);
		}
		return r;
	}

	/**
	 * [設定] デバイス設定.
	 *		コントロール作成時、及び、トップウィンドウが WM_CHANGEDEVICE を受け取った時など、デバイス状態が変化した時に使用します。
	 *	@param isFullText true なら”製造者 description (COMポート番号)”になります。省略すると ”COMポート番号” になります。
	 */
	void ResetDevice(bool isFullText = false)
	{
		CRs232c::TDeviceVector vd = CRs232c::EnumExistPortDeviceNames();
		ResetDevice(vd, isFullText);
	}

	/**
	 * [設定] デバイス設定.
	 *		コントロール作成時、及び、トップウィンドウが WM_CHANGEDEVICE を受け取った時など、デバイス状態が変化した時に使用します。
	 *	@param vd デバイス情報
	 *	@param isFullText true なら”製造者 description (COMポート番号)”になります。省略すると ”COMポート番号” になります。
	 */
	void ResetDevice(const CRs232c::TDeviceVector& vd, bool isFullText = false)
	{
		m_isFullText = isFullText;
		_super::SetRedraw(FALSE);
		int f = GetCurPort();
		m_Reset(vd);
		SetCurPort(f);
		_super::SetRedraw(TRUE);
		_super::RedrawWindow();
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
		if ( ::IsWindow(m_toolTip) )
		{
			m_toolTip.RelayEvent(pMsg);
		}
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
		if ( message == WM_DESTROY )
		{
			m_descList.RemoveAll();
			m_toolTip.DestroyWindow();
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
		DWORD style = _super::GetStyle();
		ASSERT0( (style & 3) == CBS_DROPDOWNLIST, "CPortComboBox", "DropDownListになっていません" );
		_super::PreSubclassWindow();
//		m_Reset();
	}
	
	/**
	 * [通知] for notifications from parent
	 *	@note	CWndのメソッドをオーバーライドしています。
	 *			メッセージ受信したらコールされます。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE 処理済。
	 *	@retval FALSE 未処理。
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_COMMAND )
		{
			if ( lParam == reinterpret_cast<LPARAM>(_super::GetSafeHwnd()) && HIWORD(wParam) == CBN_SELCHANGE )
			{
				ASSERT ( LOWORD(wParam) == _super::GetDlgCtrlID() );
				int f = _super::GetCurSel();
				if ( f >= 0 )
				{
					f = _super::GetItemData(f);
					m_toolTip.UpdateTipText(m_descList[f], this);
				}
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [選択] 有効ポートチェック
	 *	@param port ポート番号
	 *	@retval true 有効
	 *	@retval false 無効（リストに含まれません）
	 */
	virtual bool IsEffectiveness(int port)
	{
		return true;
	}

private:

	void m_Reset(const CRs232c::TDeviceVector& vd)
	{
		_super::ResetContent();
		m_descList.RemoveAll();
		m_toolTip.DestroyWindow();
		m_toolTip.Create(this, TTS_ALWAYSTIP);
		bool hasExtPort = ! m_extPort.IsEmpty();
		if ( ! vd.IsEmpty() )
		{
			CStr s;
			loop ( i, vd )
			{
				int p = vd[i].port;
				if ( IsEffectiveness(p) )
				{
					s.Format(_T("COM%d"), p);
					if ( m_isFullText )
					{
						s += _T(": ") + vd[i].ToString();
					}
					int f = _super::AddString(s);
					_super::SetItemData(f, p);
					m_descList[p] = vd[i].ToString();
				}
			}
			_super::SetCurSel(0);
			m_toolTip.AddTool(this, m_descList[_super::GetItemData(0)]);
		}
		else if ( ! hasExtPort )
		{
			m_toolTip.AddTool(this, _T("一つも見つかりません"));
		}
		//
		if ( hasExtPort )
		{
			int f = _super::AddString(m_extPort);
			_super::SetItemData(f, 0);
			m_descList[0] = m_extPort;
		}
		m_toolTip.Activate(TRUE);
	}

	CSimpleMapT<int, CStr>	m_descList;		///< DESC一覧 int は port
	CToolTipCtrl			m_toolTip;		///< ツールチップ
	bool					m_isFullText;	///< フォーマット
	CStr					m_extPort;		///< 拡張ポート名
};



}; // MFC
}; // TNB
