#pragma once
/**
 *@file
 * テキスト付自由型ビットマップボタン関係のヘッダ
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
 * テキスト付自由型ビットマップボタンコントロール
 *
 *		ビットマップの形に合わせたボタンを作成することが出来ます。
 *
 *		CRgnButton の機能に、テキストも表示できるように拡張したものです。
 *		基本的な使い方は、スーパークラスの CRgnButton を参照してください。
 *
 *		文字列設定は、 SetWindowText() か WM_SETTEXT を使用します。
 *		フォント設定は、 SetFont() か WM_SETFONT を使用します。
 *		色指定は、 SetTextColor(), SetTextDisableColor() を使用します。
 *
 *	@par使い方
 *		リソースエディタでボタンかスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton or CStatic）を本クラスに置き換えて使用します。
 *
 *	@par必要ファイル
 *			TnbMfcRgnButtonTx.h
 * 
 *	@see CAbstractButton
 *
 *	@date 07/10/05 新規作成
 *	@date 07/10/11 押下時のOFFSETを親で管理するようにした。
 *	@date 07/10/26 文字表示に CTextDrawer を使用するようにした。
 *	@date 07/11/30 コメント追加、修正。
 *	@date 07/12/06 テキスト色設定が反映されないバグを修正。
 *	@date 08/12/09 SetTextPositionOffset() 追加。
 *	@date 10/05/12 文字列管理を改善。
 */
class CRgnButtonTx : public CRgnButton, public CDrawTextBase
{
	DEFSUPER(CRgnButton);
	typedef CDrawTextBase _text;
public:

	/// コンストラクタ
	CRgnButtonTx(void) : _super(), m_textOffset(0, 0)
	{
	}

	/// デストラクタ
	~CRgnButtonTx(void)
	{
	}

	/**
	 * [複製] 情報複製.
	 *	@note 保持している情報を複製します。文字列情報は複製しません。
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CRgnButtonTx& operator=(const CRgnButtonTx& other)
	{
		_super::operator=(other);
		_text::operator=(other);
		m_textOffset = other.m_textOffset;
		return *this;
	}

	/**
	 * [設定] 文字位置補正.
	 *	@param pos 文字位置補正
	 */
	void SetTextPositionOffset(const POINT& pos)
	{
		m_textOffset = pos;
	}

#ifdef _TnbDOXYGEN	//Document作成用シンボル
	/**
	 * [設定] 文字列設定.
	 *	@note 本クラスは、メッセージ(WM_SETTEXT)にも対応しています。
	 *	@param lpszString 文字列。
	 */
	void SetWindowText(LPCTSTR lpszString);

	/**
	 * [設定] フォント指定.
	 *	@note 本クラスは、メッセージ(WM_SETFONT)にも対応しています。
	 *	@param pFont 新しいフォントを指定します。
	 *	@param bRedraw TRUE の場合は CWnd オブジェクトを再描画します。
	 */
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
#endif

protected:

	/**
	 * [設定] ショートカットキー設定.
	 *	@note 本クラスでは、本メソッドは無効です。設定する場合、通常のボタンと同じように、
	 *			ボタン文字列に & キーワードを入れてください。
	 *	@param key ダミー
	 */
	void SetShortcutKey(TCHAR key)
	{
	}

	/**
	 * [通知] 描画中.
	 *		Bitmap表示の前後にコールされます。
	 *	@param[in,out] _pos 表示位置。
	 *	@param[in] pDC 描画先のCDC。
	 *	@param[in] boIsFirst Bitmap表示前は true、表示後は false で通知されます。
	 */
	virtual void OnDrawingButton(CPoint& _pos, CDC* pDC, bool boIsFirst)
	{
		if ( boIsFirst || m_strText.IsEmpty() )
		{
			return;
		}
		CFont* pOldFont = pDC->SelectObject(_super::GetFont());
		CRect rect;
 		_super::GetClientRect(rect);
		pDC->SetBkMode(TRANSPARENT);		// バックカラーは変更なし
		if ( _super::GetButtonState() == ES_Pushed )
		{
			rect.OffsetRect(_pos);
		}
		rect.OffsetRect(m_textOffset);
		DWORD dt = DT_CENTER | DT_VCENTER;
		_text::DrawTextEx(pDC, rect, dt, (_super::GetButtonState() != ES_Disable), m_strText);
		pDC->SelectObject(pOldFont);
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		_super::GetWindowText(m_strText);
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
		if ( message == WM_SETTEXT )
		{
			m_strText = reinterpret_cast<LPCTSTR>(lParam);
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	CPoint			m_textOffset;		///< 文字の位置ずれ
	CString			m_strText;			///< テキスト内容
};



}; //MFC
}; //TNB



