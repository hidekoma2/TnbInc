#pragma once
/**
 *@file
 * レイアウトダイアログ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbLayout.h"


/// MFC static リンク対策
#define classCLayoutDialog classCDialog



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL LAYOUT
 * コントロールレイアウトアイテムクラス.
 *
 *		CGroupLayout クラスなどと連携し、ダイアログ上のコントロールのレイアウトを設定、調整するクラスです。
 *
 *	@see CLayoutDialog
 *
 *	@par必要ファイル
 *			TnbMfcLayoutDialog.h
 *
 *	@date 08/07/17 新規作成
 */
class CControlLayoutItem : public ILayout
{
	DEFSUPER(ILayout);
public:

	/**
	 * コンストラクタ.
	 *	@param hWnd 管理ウィンドウ
	 */
	CControlLayoutItem(HWND hWnd) : m_hWnd(hWnd)
	{
		ms_GetSize(m_hWnd, m_size);
	}

	/**
	 * コンストラクタ.
	 *	@param hWnd 管理ウィンドウ
	 *	@param sz 最小サイズ
	 */
	CControlLayoutItem(HWND hWnd, const SIZE& sz) : m_hWnd(hWnd), m_size(sz)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		return new CControlLayoutItem(m_hWnd);
	}

	/**
	 * [取得] サイズ取得.
	 *		現在のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		return ms_GetSize(m_hWnd, _size);
	}

	/**
	 * [取得] 最小サイズ取得.
	 *		最小のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		_size = m_size;
		return true;
	}

	/**
	 * [設定] サイズ設定.
	 *		現在のサイズを変更することが出来ます。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。
	 */
	virtual bool Resize(const SIZE& size)
	{
		::SetWindowPos(m_hWnd, NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		return true;
	}

	/**
	 * [処理] 決定.
	 *	@note オブジェクトが管理しているサイズと指定のサイズにレイアウトします。
	 *	@param x  X座標。-1なら指定なし。
	 *	@param y  Y座標。-1なら指定なし。この場合 wParam に関係なく、Zオーダーの変更はしません。 lParam ポインタ先の値も変更しません。
	 *	@param wParam W パラメータ。 'HWND' の場合、Zオーダーの変更をします。
	 *	@param lParam L パラメータ。 wParam が 'HWND' の場合、 HWND のポインタを保持しています。
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		if ( y >= 0 && wParam == 'HWND' )
		{
			//-- Zオーダー変更
			HWND* pPrevHwnd = reinterpret_cast<HWND*>(lParam);
			ASSERT( ! ::IsBadWritePtr(pPrevHwnd, sizeof(HWND)) );
			::SetWindowPos(m_hWnd, *pPrevHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
			*pPrevHwnd = m_hWnd;
		}
		ms_MoveControlPos(m_hWnd, x, y);
	}

	/**
	 * [作成] レイアウトアイテム作成.
	 *	@note CControlLayoutItem のショートネームです。
	 *	@param pWnd 管理ウィンドウ
	 *	@return レイアウトアイテム
	 */
	friend CControlLayoutItem Li(CWnd* pWnd);

	/**
	 * [作成] レイアウトアイテム作成
	 *	@note CControlLayoutItem のショートネームです。
	 *	@param ctrlId コントロールID
	 *	@param pParent 親ウィンドウ
	 *	@return レイアウトアイテム
	 */
	friend CControlLayoutItem Li(UINT ctrlId, CWnd* pParent);

private:
	// コントロールのクライアント座標、サイズ取得
	static bool ms_GetControlPos(POINT& _pos, HWND hWnd)
	{
		HWND hParent = ::GetParent(hWnd);
		RECT rc;
		if ( hParent != NULL && ::GetWindowRect(hWnd, &rc) )
		{
			POINT po = { rc.left, rc.top };
			if ( ::ScreenToClient(hParent, &po) )
			{
				_pos = po;
				return true;
			}
		}
		return false; 
	}
	/*
	 * コントロール位置設定.
	 *		コントロールを、(親ウィンドウの)指定のクライアント座標に移動します。
	 *	@param hWnd コントロール。
	 *	@param x クライアント X座標。-1なら移動なし。
	 *	@param y クライアント Y座標。-1なら移動なし。
	 */
	static bool ms_MoveControlPos(HWND hWnd, int x, int y)
	{
		POINT po;
		if ( ms_GetControlPos(po, hWnd) )
		{
			int xx = (x < 0) ? po.x : x;
			int yy = (y < 0) ? po.y : y;
			::SetWindowPos(hWnd, NULL, xx, yy, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
			return true;
		}
		return false;
	}
	/// サイズ取得
	static bool ms_GetSize(HWND hWnd, SIZE& _size)
	{
		RECT rc;
		if ( ::GetWindowRect(hWnd, &rc) )
		{
			_size.cx = rc.right - rc.left;
			_size.cy = rc.bottom - rc.top;
			return true;
		}
		_size.cx = 0;
		_size.cy = 0;
		return false;
	}
	HWND	m_hWnd;		///< 管理ウィンドウ
	SIZE	m_size;		///< 最小サイズ
};



// レイアウトアイテム作成
inline CControlLayoutItem Li(CWnd* pWnd)
{
	ASSERT( pWnd != NULL );
	return CControlLayoutItem(pWnd->GetSafeHwnd());
}

// レイアウトアイテム作成
inline CControlLayoutItem Li(UINT ctrlId, CWnd* pParent)
{
	return Li(pParent->GetDlgItem(ctrlId));
}



/**@ingroup MFCCONTROL LAYOUT
 * レイアウトダイアログクラス.
 *
 *		CGroupLayout クラスなどと連携し、ダイアログ上のコントロールのレイアウトを設定、調整するクラスです。
 *
 *		本クラスを CDialog の代わりに継承すると、リレイアウト可能なダイアログになります。
 *		CDialog を使ったダイアログを作る要領でリソースを作成しクラス生成後、 CDialog
 *		を本クラスに置き換える形の実装方法を想定しています。
 *
 *	@par使用例 (リサイズに対応する場合)
 *		\code
 *		//= 宣言(ヘッダファイル)
 *		class CXxxDlg : public CLayoutDialog	// ← CDialog を CLayoutDialog に変更。		
 *				// 他の 「CDialog」 も同様
 *		{
 *				:
 *
 *			CStatic	m_txtFirst;	
 *			CStatic	m_txtSecond;
 *			CEdit	m_editFirst;
 *			CEdit	m_editSecond;
 *
 *				:
 *		}
 *
 *		//= 初期化関数
 *		BOOL CXxxDlg::OnInitDialog() 
 *		{
 *				:
 *
 *			// レイアウト設定
 *			CBoxLayout lay;
 *
 *				:
 *
 *			SetLayout(lay);
 *			// ChangeMinimumSize(); // 必要最小サイズにする場合はこの行を有効にする		
 *
 *				:
 *		}
 *		\endcode
 *
 *	@par使用例 (レイアウトするだけの場合)
 *		\code
 *		//= 初期化関数
 *		BOOL CXxxDlg::OnInitDialog() 
 *		{
 *				:
 *
 *			// レイアウト設定
 *			CBoxLayout lay;
 *
 *				:
 *
 *			CLayoutDialog::Decide(this, lay);	//レイアウト
 *
 *				:
 *		}
 *		\endcode
 *
 *	@todo 親を template にすると、リソースエディタが ダイアログであることを
 *			認識できなくなってしまうため、 現在 親は CDialog 固定です。
 *
 *	@par必要ファイル
 *			TnbMfcLayoutDialog.h
 *
 *	@date 08/07/17 新規作成
 */
class CLayoutDialog : public CDialog
{
	DEFSUPER(CDialog);
public:

	/// コンストラクタ
	CLayoutDialog(void) : _super()
	{
	}

	/**
	 * コンストラクタ
	 *	@param lpszTemplateName ダイアログ ボックス テンプレート リソース名を表す null で終わる文字列。
	 *	@param pParentWnd ダイアログ オブジェクトを所有する CWnd 型の親ウィンドウ
	 *					またはオーナー ウィンドウオブジェクトへのポインタ。
	 *					NULL の場合は、ダイアログ オブジェクトの親ウィンドウがアプリケーションのメイン ウィンドウになります。
	 */
	CLayoutDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
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
	CLayoutDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) 
		: _super(nIDTemplate, pParentWnd)
	{
	}

	/**
	 * [設定] リサイズモード設定.
	 *	@note デフォルトでは、水平方向、垂直方向共に true が指定されています。
	 *	@param canHorizontalResize false を指定すると水平方向のサイズ変更が出来なくなります。
	 *	@param canVerticalResize false を指定すると垂直方向のサイズ変更が出来なくなります。
	 */
	void SetResizeMode(bool canHorizontalResize = true, bool canVerticalResize = true)
	{
		m_param.canHorizontalResize = canHorizontalResize;
		m_param.canVerticalResize = canVerticalResize;
	}

	/**
	 * [設定] レイアウト設定.
	 *	@note 指定のダイアログのアイテムを、オブジェクトが管理しているレイアウトにします。
	 *	@param[in,out] lay レイアウト。実行後、サイズがダイアログのクライアントサイズに変更されています。
	 *					クローンが記憶されているので破棄してかまいません。
	 *	@param zorderTop Zオーダの先頭を指定します。通常 HWND_TOP を指定します。 
	 *					HWND_TOPMOST , HWND_NOTOPMOST , HWND_BOTTOMを指定すると、Zオーダの設定をしません。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetLayout(ILayout& lay, HWND zorderTop = HWND_TOP)
	{
		m_pLayout = lay.Clone();
		if ( ! m_pLayout.IsNull() && Decide(this, *m_pLayout, zorderTop) )
		{
			CRect winRc;
			_super::GetWindowRect(winRc);
			return !! _super::SetWindowPos(NULL, 0, 0, winRc.Width(), winRc.Height(), SWP_NOMOVE | SWP_NOZORDER);
		}
		return false;
	}

	/**
	 * [設定] サイズ変更.
	 *	@param sz クライアントサイズ
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool ChangeClientSize(const SIZE& sz)
	{
		CRect winRc;
		CRect cliRc;
		_super::GetWindowRect(winRc);
		_super::GetClientRect(cliRc);
		CSize s = winRc.Size() - cliRc.Size();
		return !! _super::SetWindowPos(NULL, 0, 0, sz.cx + s.cx, sz.cy + s.cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	/**
	 * [設定] 最小サイズ化.
	 *		SetLayout で設定したレイアウトで必要な最小サイズにクライアントサイズを変更します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool ChangeMinimumSize(void)
	{
		if ( ! m_pLayout.IsNull() )
		{
			CSize sz;
			if ( m_pLayout->GetMinimumSize(sz) )
			{
				return ChangeClientSize(sz);
			}
		}
		return false;
	}

	/**
	 * [処理] 決定.
	 *	@note SetLayout() で登録したレイアウトの Decide() を実行します。
	 *	@param x  X座標。-1なら指定なし。
	 *	@param y  Y座標。-1なら指定なし。
	 *	@param wParam W パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 *	@param lParam L パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 */
	void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		if ( ! m_pLayout.IsNull() )
		{
			m_pLayout->Decide(x, y, wParam, lParam);
		}
	}

	/**
	 * [設定] レイアウト指定.
	 *	@param[in] rect レイアウト範囲.
	 *	@param[in,out] lay レイアウト。実行後、サイズが pWnd のクライアントサイズに変更されています。
	 *	@param zorderTop Zオーダの先頭を指定します。通常 HWND_TOP を指定します。 
	 *					HWND_TOPMOST , HWND_NOTOPMOST , HWND_BOTTOMを指定すると、Zオーダの設定をしません。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool Decide(const RECT& rect, ILayout& lay, HWND zorderTop = HWND_TOP)
	{
		WPARAM wParam = 0;
		LPARAM lParam = 0;
		if ( zorderTop != HWND_NOTOPMOST && zorderTop != HWND_BOTTOM && zorderTop != HWND_TOPMOST)
		{
			wParam = 'HWND';
			lParam = reinterpret_cast<LPARAM>(&zorderTop);
		}
		SIZE sz = { rect.right - rect.left, rect.bottom - rect.top };
		bool r = lay.Resize(sz);
		lay.Decide(rect.left, rect.top, wParam, lParam);
		return r;
	}

	/**
	 * [設定] レイアウト指定.
	 *	@param[in] pWnd ダイアログ.
	 *	@param[in,out] lay レイアウト。実行後、サイズが pWnd のクライアントサイズに変更されています。
	 *	@param zorderTop Zオーダの先頭を指定します。通常 HWND_TOP を指定します。 
	 *					HWND_TOPMOST , HWND_NOTOPMOST , HWND_BOTTOMを指定すると、Zオーダの設定をしません。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool Decide(CWnd* pWnd, ILayout& lay, HWND zorderTop = HWND_TOP)
	{
		bool r = false;
		if ( ::IsWindow(pWnd->GetSafeHwnd()) )
		{
			CRect rc;
			pWnd->GetClientRect(&rc);
			r = Decide(rc, lay, zorderTop);
			pWnd->Invalidate(FALSE);
		}
		return r;
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
		if ( ! m_pLayout.IsNull() )
		{
			if ( message == WM_SIZE )
			{
				int h = HIWORD(lParam);
				int w = LOWORD(lParam);
				m_pLayout->Resize(CSize(w, h));
				m_pLayout->Decide(0, 0, 0, 0);
				_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
			}
			#ifndef _WIN32_WCE
			{
				if ( message == WM_GETMINMAXINFO )
				{
					MINMAXINFO* P = reinterpret_cast<MINMAXINFO*>(lParam);
					CRect winRc;
					CRect cliRc;
					_super::GetWindowRect(winRc);
					_super::GetClientRect(cliRc);
					CSize sz;
					if ( m_pLayout->GetMinimumSize(sz) )
					{
						CSize s = winRc.Size() - cliRc.Size();
						P->ptMinTrackSize.x = sz.cx + s.cx;
						P->ptMinTrackSize.y = sz.cy + s.cy;
						if ( ! m_param.canHorizontalResize )
						{
							P->ptMaxTrackSize.x = P->ptMinTrackSize.x;
						}
						if ( ! m_param.canVerticalResize )
						{
							P->ptMaxTrackSize.y = P->ptMinTrackSize.y;
						}
					}
				}
			}
			#endif
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// パラメータ
	struct TParam
	{
		bool	canHorizontalResize;		///< 水平幅のリサイズ可能
		bool	canVerticalResize;			///< 垂直高のりサイズ可能
		/// コンストラクタ
		TParam(void) : canHorizontalResize(true), canVerticalResize(true)
		{}
	};
	TParam			m_param;				///< パラメタ
	ILayout::Ptr	m_pLayout;				///< レイアウト
};



}; // MFC
}; // TNB