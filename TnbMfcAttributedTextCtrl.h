#pragma once
/**
 *@file
 * 属性付きのテキスト
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAttributedStr.h"
#include "TnbThread.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * テキストコントロール
 *
 *		一文字ごとに属性をつけることのできる、テキストです。
 *		（属性は、現在、ノーマル、反転、点滅のみです。）
 *
 *	@par使い方
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcAttributedTextCtrl.h
 * 
 *	@date 06/10/10 新規作成
 *	@date 07/10/31 WM_PRINTCLIENT 対応。
 */
class CAttributedTextCtrl : public CStatic, TNB::CThread::IRunner
{
	DEFSUPER(CStatic);
public:

	/// 文字属性
	enum EAttr
	{
		NORMAL,		///< 標準
		REVERSE,	///< 反転
		BLINK		///< 点滅
	};

	typedef CAttributedStrT<EAttr>	CAttrStr;		///< 属性付き文字列

private:
	
	CThread			m_thread;				///< Blink用スレッド
	bool			m_boIsNormalForBlink;	///< Blink用フラグ
	CFont*			m_pfont;				///< 使用するフォント
	CRect			m_rect;					///< コントロールの大きさ
	CPoint			m_pointMargin;			///< 表示開始位置
	COLORREF		m_colorBack;			///< 背景色
	COLORREF		m_colorNormal;			///< 標準文字色
	CSyncCounter	m_syncDraw;				///< 描画排他
	enum {
		BLINK_TIMING = 500					///< ブリンクタイミング
	};
	CAttrStr				m_content;		///< 表示情報
	CVectorT<INDEX>			m_vdwTopPoss;	///< 行トップポジション

	/**
	 * 各行のポジション計算
	 *	@note m_content を変更したら必ずこれを実行
	 */
	void m_ResearchContent(void)
	{
		m_vdwTopPoss.RemoveAll();
		m_vdwTopPoss.Add(0);
		loop ( i, m_content.GetLength() )
		{
			if ( m_content.GetAt(i) == '\r' )
			{
				if ( m_content.GetAt(i + 1) == '\n' )
				{
					m_vdwTopPoss.Add(i + 2);
				}
				else
				{
					m_vdwTopPoss.Add(i + 1);
				}
			}
		}
	}

	/**
	 * 描画
	 *	@param dc 対象のDC
	 *	@param boIsForBlink trueならBlink再描画用です（一度全体を消すことをしない）。
	 */
	void m_Draw(CDC* dc, bool boIsForBlink = false)
	{
		dc->SaveDC();
		CFont* pOldFont = dc->SelectObject(m_pfont);

		dc->IntersectClipRect(m_rect);

		if ( ! boIsForBlink )
		{
			dc->FillSolidRect(m_rect, m_colorBack);
		}
		LOGFONT tFont;
		m_pfont->GetLogFont(&tFont);
		dc->SetBkMode(OPAQUE/*TRANSPARENT*/);
		loop ( i, m_vdwTopPoss.GetSize() )
		{
			CRect r = m_rect;
			r.OffsetRect(m_pointMargin);
			r.OffsetRect(0, abs(static_cast<LONG>(tFont.lfHeight * i)));
			TCHAR c[3] = {0};
			UINT uChar;
			INDEX j = m_vdwTopPoss[i];
			while ( j < m_content.GetLength() )
			{
				EAttr a = m_content.GetAttribute(j);
				c[0] = m_content.GetAt(j++);
				if ( c[0] == '\n' || c[0] == 0 )
				{
					break;
				}
				if ( a == BLINK )
				{
					a = m_boIsNormalForBlink ? NORMAL : REVERSE;
				}
				if ( a == NORMAL )
				{
					dc->SetTextColor(m_colorNormal);
					dc->SetBkColor(m_colorBack);
				}
				else
				{
					dc->SetTextColor(m_colorBack);
					dc->SetBkColor(m_colorNormal);
				}
				uChar = c[0] & 0xff;
				if ( STRLIB::GetCharSize(c[0]) == 2 )
				{
					//- SJISだ
					c[1] = m_content.GetAt(j);
					uChar = (uChar << 8) | (c[1] & 0xFF);
					j++;
					dc->DrawText(c, 2, r, 0);
				}
				else
				{
					//- ASCIIだ
					dc->DrawText(c, 1, r, 0);
				}
				//
				ABC abc;
				VERIFY(dc->GetCharABCWidths(uChar, uChar, &abc));
				r.OffsetRect(abc.abcA + abc.abcB + abc.abcC, 0);
			}
		}
		dc->SelectObject(pOldFont);
		dc->RestoreDC(-1);
	}

	/**
	 * スレッド処理関数
	 *	@note ブリンクタイミングで、フラグを反転しています。
	 *			描画中でなけれ描画します。
	 *	@retval ０（固定)。
	 */
	DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			::Sleep(BLINK_TIMING);
			m_boIsNormalForBlink = ! m_boIsNormalForBlink;
			if ( m_syncDraw.Lock(0) )
			{
				if ( ::IsWindow(GetSafeHwnd()) )
				{
					HDC hdc = ::GetDC(m_hWnd);
					if ( hdc != NULL )
					{
						CDC dc;
						dc.Attach(hdc);
						m_Draw(&dc, true);
						hdc = dc.Detach();
						::ReleaseDC(m_hWnd, dc); 
					}
				}
				m_syncDraw.Unlock();
			}
		}
		return 0;
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
		if ( message == WM_PAINT )
		{
			CPaintDC dc(this); // 描画用のデバイス コンテキスト
			if ( m_syncDraw.Lock(0) )
			{
				m_Draw(&dc);
				m_syncDraw.Unlock();
			}
			return 0;
		}
		else if ( message == WM_PRINTCLIENT )
		{
//			if ( (lParam & PRF_CLIENT) != 0 )
			{
				CDC* pDc = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				if ( m_syncDraw.Lock(0) )
				{
					m_Draw(pDc);
					m_syncDraw.Unlock();
				}
			}
			return 0;
		}
		else if ( message == WM_SETTEXT )
		{
			CAttrStr s(reinterpret_cast<LPCTSTR>(lParam));
			SetContent(s);
		}
		else if ( message == WM_DESTROY )
		{
			EXCLUSIVE(&m_syncDraw);
			return _super::WindowProc(message, wParam, lParam);
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
		_super::GetClientRect(m_rect);
		_super::PreSubclassWindow();
		CString s;
		_super::GetWindowText(s);
		m_content = s.operator LPCTSTR();
		m_ResearchContent();
		//
		CWnd* P = GetParent();
		if ( P != NULL )
		{
			m_pfont = P->GetFont();
		}
		m_thread.Start(_T("CAttributeTextCtrl Blink用"));
	}

public:

	/// コンストラクタ
	CAttributedTextCtrl(void)
		: _super()
		, m_colorBack(::GetSysColor(COLOR_MENU))
		, m_colorNormal(::GetSysColor(COLOR_MENUTEXT))
		, m_boIsNormalForBlink(false)
		, m_pointMargin(0, 0)
	{
		m_thread.SetRunner(this);
	}

	/// デストラクタ
	~CAttributedTextCtrl(void)
	{
		m_thread.Stop();
	}

	/**
	 * [設定] マージン設定
	 *	@param x 左端のマージン
	 *	@param y 上端のマージン
	 */
	void SetMargin(int x, int y)
	{
		m_pointMargin.x = x;
		m_pointMargin.y = y;
	}

	/**
	 * [設定] フォント設定
	 *	@note MFC の CWnd::SetFont() と同じです。
	 *	@attention 設定したフォントは破棄しないで下さい。
	 *	@param pFont フォントの参照。
	 *	@param bRedraw TRUE の場合は CWnd オブジェクトを再描画します。
	 */
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE)
	{
		m_pfont = pFont;
		_super::SetFont(pFont, bRedraw);
	}

	/**
	 * [取得] 内容取得
	 *	@return 属性付き文字列
	 */
	const CAttrStr& GetContent(void) const
	{
		return m_content;
	}

	/**
	 * [設定] 内容一括設定
	 *	@param as 属性付き文字列
	 */
	void SetContent(const CAttrStr& as)
	{
		m_content = as;
		m_ResearchContent();
		RedrawWindow();
	}

	/**
	 * [設定] 内容連結
	 *	@param as 連結する属性付き文字列
	 */
	void AddContent(const CAttrStr& as)
	{
		m_content += as;
		m_ResearchContent();
		RedrawWindow();
	}

	/**
	 * [設定] 内容挿入
	 *	@param iIndex 挿入するインデックス
	 *	@param as 連結する属性付き文字列
	 */
	void InsertContent(int iIndex, const CAttrStr& as)
	{
		m_content.Insert(iIndex, as);
		m_ResearchContent();
		RedrawWindow();
	}

	/**
	 * [設定] 内容変更
	 *	@param iIndex 変更するインデックス
	 *	@param c 文字コード
	 *	@param attr 属性
	 */
	void SetAt(int iIndex, TCHAR c, EAttr attr = NORMAL)
	{
		m_content.SetAt(iIndex, c, attr);
	}
};



}; //MFC
}; //TNB

