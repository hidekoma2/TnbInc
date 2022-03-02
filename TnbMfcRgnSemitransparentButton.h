#pragma once
/**
 *@file
 * 自由型半透明ビットマップボタン関係のヘッダ
 *
 *		ビットマップの形状にあわせたグラフィカルなボタンを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcRgnButton.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL BITMAP
 * 自由型半透明ビットマップボタンコントロール
 *
 *		ビットマップの形に合わせたボタンを作成することが出来ます。任意の状態を半透明にすることも出来ます。
 *		基本的な使い方は、スーパークラスの CRgnButton を参照してください。
 *
 *	@note	半透明指定はビット単位で行われます。 bit0が「通常状態」、bit1が「ホバー状態」、bit2が「押下状態」、
 *			bit3が[無効状態」、	bit4が「フォーカス状態」に割り当てられており、 1 になっていると、半透明になります。
 *			例えば、 8 を指定すると、「無効状態」のボタンだけが半透明に成ります。
 *
 *	@attention 親のダイアログが WM_PRINTCLIENT メッセージを正しく処理している必要があります。
 *
 *	@attention 初めての描画の際に、コントロール下の背景と半透明合成を行い、保持します。そのため、
 *				ボタンのビットマップや背景が途中で変化させると、期待する表示にはなりません。その場合、
 *				全ビットマップボタンを再設定し、 ReCalc() を使用することで、再合成を行うことが出来ます。
 *
 *	@par使い方
 *		リソースエディタでボタンかスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton or CStatic）を本クラスに置き換えて使用します。
 *
 *	@param MODE 半透明指定。 省略すると 0x1F になります。
 *	@param PAR 半透明度。 ０%だと背景のみ、 100% だと透明になりません。
 *
 *	@see CAbstractButton
 *
 *	@par必要ファイル
 *			TnbMfcRgnSemitransparentButton.h
 * 
 *	@date 07/10/24 新規作成
 *	@date 07/10/30 実用化。ただし Windows2000以降のみ動作。
 *	@date 07/10/30 高速化。
 */
template<BYTE MODE = 0x1F, DWORD PAR = 50>
class CRgnSemitransparentButtonT : public CRgnButton
{
	DEFSUPER(CRgnButton);
	BYTE		m_semitransMode;		///< 透過する状態指定 bitで指定
	DWORD		m_parsent;				///< 透明度
	bool		m_boIsDoGetParentImage;	///< 透過画像作成指示フラグ
	bool		m_boIsDoIniting;		///< 初期化中フラグ
	/// 半透明合成
	void m_Compose(CBitmapHandle& _hh, const CBitmapImage& parentBitmap)
	{
		CBitmapImage bi;
		bi.Attach(_hh);
		bi.InsertOnAlphaBlend(0, 0, parentBitmap, 100 - m_parsent);
		_hh = bi.GetBitmapHandle();
	}
	/// 半透明合成
	void m_AllComp(const CBitmapImage& parentBitmap)
	{
		loop ( i, 4 )
		{
			if ( (m_semitransMode & _BIT(i)) != 0 )
			{
				m_Compose(_super::m_bmpButtons[i], parentBitmap);
			}
		}
		if ( (m_semitransMode & _BIT(4)) != 0 && ! IS_RGBVALUE(m_focusMaskColor) )
		{
			CBitmapHandle h = m_focusBitmap.GetBitmapHandle();
			m_Compose(h, parentBitmap);
			m_focusBitmap.Attach(h);
		}
	}
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
		if ( message == WM_PAINT )
		{
			if ( m_boIsDoIniting ) { return 0; }
			if ( m_boIsDoGetParentImage )
			{
				LRESULT rr = _super::WindowProc(message, wParam, lParam);
				m_boIsDoIniting = true;
				m_boIsDoGetParentImage = false;
#if 0
				CRect rect;
				GetWindowRect(&rect);
				CWnd* pWnd = GetParent();
				ASSERTLIB( pWnd != NULL );
				DWORD dwMod = 0;
				if ( (pWnd->GetExStyle() & WS_EX_LAYERED) == 0 )
				{
					MFCLIB::SetLayeredWindow(pWnd, 0, 255, MFCLIB::E_Alpha | MFCLIB::E_SetLayeredStyle);
					dwMod = WS_EX_LAYERED;
				}
				pWnd->ScreenToClient(&rect);
				pWnd->RedrawWindow(&rect);
				CDC* pDC = pWnd->GetDC();
				ASSERT( pDC != NULL );
				CBitmapImage parentBitmap;
				parentBitmap.Set(*pDC, rect);
				pWnd->ReleaseDC(pDC);
				m_AllComp(parentBitmap);
				pWnd->ModifyStyleEx(dwMod, 0);
#else
				CWnd* pWnd = _super::GetParent();
				CBitmapImage bi;
				bi.SetFromClient(*pWnd);
				CRect rect;
				_super::GetWindowRect(rect);
				pWnd->ScreenToClient(rect);
				m_AllComp(bi.Cut(rect));
#endif
				m_boIsDoIniting = false;
				return rr;
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
public:

	/**
	 * コンストラクタ
	 *	@param semitransMode 半透明指定。
					省略すると、テンプレートで指定した値になります。
	 *	@param parsent 半透明度。 ０%だと背景のみ、 100% だと透明になりません。
					省略すると、テンプレートで指定した値になります。
	 */
	CRgnSemitransparentButtonT(BYTE semitransMode = MODE, DWORD parsent = PAR) 
		: _super(), m_boIsDoGetParentImage(true), m_boIsDoIniting(false)
		, m_semitransMode(semitransMode), m_parsent(parsent)
	{
	}

	/**
	 * [設定] 再合成.
	 *	@note	初めての描画の際に、コントロール下の背景と半透明合成を行い、保持します。そのため、
	 *			ボタンのビットマップや背景が途中で変化させると、期待する表示にはなりません。その場合、
	 *			全ビットマップボタンを再設定し、 本メソッドを使用することで、再合成を行うことが出来ます。
	 */
	void ReCalc(void)
	{
		m_boIsDoGetParentImage = true;
	}
};



}; //MFC
}; //TNB



