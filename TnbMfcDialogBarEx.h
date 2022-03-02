#pragma once
/**
 *@file
 * ダイアログバー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ダイアログバー拡張ベースクラス.
 *
 *		ダイアログバーを作成する場合、 CDialogBar の代わりにこれを使用すると、
 *		Dialog同様の Class Wizerd を用いた編集や、サイズの調整をサポートできます。
 *
 *	@par使用例
 *		まず、リソースでダイアログを作成します。
 *		Class Wizerd で CDialog を親クラスにしたサブクラスを作成します。
 *		そのサブクラス .h / .cpp の CDialog をすべて CDialogBarEx に置き換えます。\n
 *		OnInitDialog() は呼ばれません。代わりに BOOL OnInitDialogBar(void) を実装してください。
 *
 *	@par必要ファイル
 *			TnbMfcDialogBarEx.h
 * 
 *	@date 08/09/20 新規作成
 *	@date 10/05/19 コメント整理	
 */
class CDialogBarEx : public CDialogBar
{
	DEFSUPER(CDialogBar);
public:

	/// コンストラクタ
	CDialogBarEx(...) : m_bChangeDockedSize(false), m_sizeDocked(0, 0), m_sizeFloating(0, 0)
	{
	}

	/// デストラクタ
	virtual ~CDialogBarEx()
	{
	}

	/**
	 * [作成] ダイアログバー作成.
	 *	@param pParentWnd 親ウィンドウ
	 *	@param lpszTemplateName リソーステンプレート名.
	 *	@param nStyle スタイル
	 *	@param nID コントロールID
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID)
	{
		if ( ! _super::Create(pParentWnd, lpszTemplateName, nStyle, nID) )
		{
			return FALSE;
		}
		m_bChangeDockedSize = false;
		m_sizeFloating = m_sizeDocked = m_sizeDefault;
		return OnInitDialogBar();
	}

	/**
	 * [作成] ダイアログバー作成.
	 *	@param pParentWnd 親ウィンドウ
	 *	@param nIDTemplate リソーステンプレートID.
	 *	@param nStyle スタイル
	 *	@param nID コントロールID
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL Create(CWnd * pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
	{
		return Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, nID);
	}

	/**
	 * [設定] ドッキング時サイズモード設定.
	 *	@param isResized false ならデフォルトの大きさ、 true ならリサイズした大きさになります。
	 */
	void SetDockedSizeMode(bool isResized)
	{
		m_bChangeDockedSize = isResized;
	}

protected:

	/**
	 * [通知] ダイアログバー初期化通知
	 *	@note ダイアログバーが作成された時通知されます。これをオーバーロードすることで、
	 *			コントロール等の初期化が出来ます。
	 *			オーバーロードした際、最初にこのメソッドをコールしてください。
	 *	@retval true 成功。
	 *	@retval false 失敗。ダイアログバーは作成できません。
	 */
	virtual BOOL OnInitDialogBar(void)
	{
		UpdateData(FALSE);
		return TRUE;
	}

	/**
	 * [計算] サイズ計算 
	 *	@note 直接使用することはありません。
	 *	@param nLength 寸法（nMode により、垂直、水平のいずれかをさす）。
	 *	@param nMode モード。 LM_STRETCH , LM_HORZ などを OR で複数指定できます。
	 *	@return サイズ。
	 */
	CSize CalcDynamicLayout(int nLength, DWORD nMode)
	{
		if ( (nMode & LM_VERTDOCK) != 0 || (nMode & LM_HORZDOCK) != 0 )
		{
			if ( (nMode & LM_STRETCH) != 0 )
			{
				return CSize((nMode & LM_HORZ) ? 32767 : m_sizeDocked.cx,
							(nMode & LM_HORZ) ? m_sizeDocked.cy : 32767);
			}
			else
			{
				return m_sizeDocked;
			}
		}
		if ( (nMode & LM_MRUWIDTH) != 0 )
		{
			return m_sizeFloating;
		}
		if ( (nMode & LM_LENGTHY) != 0 )
		{
			return CSize(m_sizeFloating.cx, (m_bChangeDockedSize) ?
						m_sizeFloating.cy = m_sizeDocked.cy = nLength :
						m_sizeFloating.cy = nLength);
		}
		return CSize((m_bChangeDockedSize) ?
						m_sizeFloating.cx = m_sizeDocked.cx = nLength :
						m_sizeFloating.cx = nLength, m_sizeFloating.cy);
	}

	/**
	 * [計算] 水平方向サイズ計算 
	 *	@note 直接使用することはありません。
	 *	@param bStretch バーをフレームのサイズに拡大するかどうかを示します。
	 *					バーがドッキング バーではない (ドッキングが利用できない) 場合は、パラメータ bStretch を 0 以外に設定し、
	 *					ドッキングまたはフリー (ドッキングが利用できる) の場合は、0 を設定します。	
	 *	@param bHorz バーが水平方向か垂直方向かを示します。バーが水平方向のときはパラメータ bHorz を 0 以外に設定し、
	 *					垂直方向のときは 0 を設定します。
	 *	@return サイズ。
	 */
	CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
	{
		return _super::CalcFixedLayout(bStretch, bHorz);
	}

	/**
	 * [処理] 状態更新通知
	 *	@note 本クラスではコントロールが有効になるように設定しています。
	 *	@note 直接使用することはありません。
	 *	@param pTarget メインウィンドウのポインタ。 
	 *	@param bDisableIfNoHndler ダミー（値は参照しません）
	 */
	void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CDialogBar::OnUpdateCmdUI(pTarget, false);
	}

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	//{{AFX_VIRTUAL(CTrickDialogBar)
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTrickDialogBar)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	#endif // _TnbDOXYGEN

	CSize m_sizeDocked;			///< ドッキング時のサイズ
	CSize m_sizeFloating;		///< フローティング時のサイズ
	BOOL m_bChangeDockedSize;	/**<ドッキング時のサイズモード。
									false ならデフォルトの大きさ、 true ならリサイズした大きさになります。*/
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル

inline void CDialogBarEx::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);
	_super::DoDataExchange(pDX);
}

#ifdef __MSVCPP6__

inline const AFX_MSGMAP* PASCAL CDialogBarEx::_GetBaseMessageMap()
{
	return &CDialogBar::messageMap; 
}

inline const AFX_MSGMAP* CDialogBarEx::GetMessageMap() const
{
	return &CDialogBarEx::messageMap; 
}

_declspec(selectany) AFX_COMDAT AFX_DATADEF const AFX_MSGMAP CDialogBarEx::messageMap =
{
	&CDialogBarEx::_GetBaseMessageMap, &CDialogBarEx::_messageEntries[0] 
};

_declspec(selectany) AFX_COMDAT const AFX_MSGMAP_ENTRY CDialogBarEx::_messageEntries[] =
{
	{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
};

#else // __MSVCPP6__

	inline const AFX_MSGMAP* CDialogBarEx::GetMessageMap() const
		{ return GetThisMessageMap(); }
	inline const AFX_MSGMAP* PASCAL CDialogBarEx::GetThisMessageMap()
	{
		static const AFX_MSGMAP_ENTRY _messageEntries[] =
		{
			{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } 
		}; 
		static const AFX_MSGMAP messageMap = 
		{ &CDialogBar::GetThisMessageMap, &_messageEntries[0] }; 
		return &messageMap; 
	}								  

#endif // __MSVCPP6__

#endif // _TnbDOXYGEN



}; //MFC
}; //TNB


