#pragma once
/**
 *@file
 *	サブアイテム編集リストコントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcDrawingListCtrl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * サブアイテム編集リスト専用エディットインターフェース.
 *
 *		サブアイテム編集リストで使用するエディタを定義するインターフェースです。
 *
 *	@see CSubEditListCtrl
 *
 *	@note SELC = SubEditListCtrl
 *
 *	@par必要ファイル
 *			TnbMfcSubEditListCtrl.h
 * 
 *	@date 12/02/16	インターフェース化
 */
struct ISelcEditFp
{
	/// デストラクタ
	virtual ~ISelcEditFp(void) {}
	/**
	 * [設定] 開始表示位置指定.
	 *	@param rect 表示位置
	 *	@param clientWidth クライアントの幅
	 */
	virtual void SetStartRect(const RECT& rect, int clientWidth) = 0;
	/**
	 * [取得] CEdit取得.
	 *	@return CEditのポインタ
	 */
	virtual CEdit* GetEdit(void) = 0;
	/**
	 * [通知] 編集終了通知.
	 */
	virtual void OnEditEnd(void) = 0;
};

/**@ingroup MFCCONTROL
 * サブアイテム編集リスト専用エディットコントロールテンプレート.
 *
 *		サブアイテム編集リストで使用するエディタコントロールを定義するテンプレートです。
 *	
 *	@see CSubEditListCtrl
 *
 *	@note SELC = SubEditListCtrl
 *
 *	@par必要ファイル
 *			TnbMfcSubEditListCtrl.h
 * 
 *	@date 11/07/04	新規作成
 */
template<typename TYP>
class CSelcEditFpT : public TYP, public ISelcEditFp
{
	DEFSUPER(TYP);
public:
	/// コンストラクタ
	CSelcEditFpT(void) : m_rect(0)
	{
	}
	/**
	 * [設定] 開始表示位置指定.
	 *	@param rect 表示位置
	 *	@param clientWidth クライアントの幅
	 */
	void SetStartRect(const RECT& rect, int clientWidth)
	{
		m_rect = rect;
		m_clientWidth = clientWidth;
	}
	/**
	 * [取得] CEdit取得.
	 *	@return CEditのポインタ
	 */
	CEdit* GetEdit(void)
	{
		return this;
	}
	/**
	 * [通知] 編集終了通知.
	 */
	virtual void OnEditEnd(void) {}
protected:
	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *		ListBox 関係のメッセージをフックしています。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == WM_WINDOWPOSCHANGING )
		{
			WINDOWPOS* P = reinterpret_cast<WINDOWPOS*>(lParam);
			P->y = m_rect.top;
			P->cy = m_rect.Height();
			if ( (P->flags & (SWP_NOSIZE | SWP_NOMOVE)) == 0 )
			{
				P->x = m_rect.left;
				if ( P->cx < m_rect.Width() )
				{
					P->cx = m_rect.Width();
				}
				if ( (P->x) + (P->cx) > m_clientWidth )
				{
					// はみ出る
					int d = m_clientWidth - (P->cx);
					if ( d >= 0 )
					{
						P->x = d;
					}
					else
					{
						P->x = 0;
						P->cx = m_clientWidth;
					}
				}
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
private:
	CRect	m_rect;			///< 位置
	int		m_clientWidth;
};



/**@ingroup MFCCONTROL
 * サブアイテム編集リストコントロール.
 *
 *		リストコントロールのサブアイテムを選択することが出来るようになります。
 *		また、サブアイテムの編集も可能です。
 *
 *	@par使い方
 *		リソースエディタで List Ctrl コントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListCtrl）を本クラスに置き換えて使用します。\n
 *		各アイテムを編集可能にする場合、リストコントロールのプロパティの「ラベルの編集」にチェックをつけておいてください。
 *
 *	@note プロパティで「ヘッダをソートしない」にするとマウスによるカラムの幅変更も禁止しています。 
 *			この状態で幅変更をさせたい場合、 GetHeaderCtrl()->EnableChangeWidth(true); とします。
 *
 *	@note 単一選択(LVS_SINGLESEL)のスタイルを付加します。複数の選択は出来ません。
 *
 *	@note 継承し GetSubItemEditControl() をオーバーライドすることで任意の Editコントロールを
 *			任意のアイテムで使用できます。
 *		\code
 *		// ex) サブアイテム０は編集不可。２，３は独自の編集。他は標準。	
 *		class CMySubEditListCtrl : public CSubEditListCtrl
 *		{
 *			DEFSUPER(CSubEditListCtrl);
 *		protected:
 *			class CMyEdit : public CSelcEditFpT<CEditHexByte>
 *			{
 *				virtual void OnEditEnd(void) 
 *				{
 *					CEditHexByte::OnEditEnd();
 *				}
 *			};
 *			virtual ISelcEditFp* GetSubItemEditControl(int item, int subItem)
 *			{
 *				if ( subItem == 0 ){ return NULL; }
 *				if ( subItem == 2 || subItem == 3 ) return _super::GetSubItemEditControl(item, subItem);
 *				return &m_edit;
 *			}
 *			CMyEdit m_edit;
 *		};
 *		\endcode
 *
 *	@note ラベルの編集が終わった時に、 LVN_ENDLABELEDIT が親ウィンドウに通知されますが、
 *			決定、キャンセルの判断は以下のようにします（CListCtrl でも同じ）。
 *	\code
 *		void CFooDlg::OnEndlabeleditListSubedit(NMHDR* pNMHDR, LRESULT* pResult) 
 *		{
 *			LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
 *			if ( (pDispInfo->item.mask & LVIF_TEXT) != 0 )
 *			{
 *				// 変更
 *				TRACE1( "text = [%s]\n", m_myListCtrl.GetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem) );
 *			}
 *			*pResult = 0;
 *		}
 *	\endcode
 *
 *	@todo 本クラスは CDrawingListCtrl を継承していますが、 選択マーク、フォーカスマークは変更不可能です。
 *		SetDefaultMarkDrawer() , SetSelectMarkDrawer() , SetFocusMarkDrawer() を使用しても変更できません。
 *
 *	@par必要ファイル
 *			TnbMfcSubEditListCtrl.h
 * 
 *	@date 11/07/04	新規作成
 *	@date 11/09/15	同アイテム間で異なるサブアイテムをクリックした時に編集開始しないようにした。
 *			自動テキスト編集モードの場合でも LVN_ENDLABELEDIT を通知するように変更。
 *	@date 11/09/19	上を改善。編集開始時、対象のサブアイテムを表示するようにした。 EDIT の大きさ可変に対応。
 *	@date 11/11/08	カーソルの左右を押下した時、親に WM_NOTIFY が行かないを修正。
 *	@date 12/02/29	新 EditLabel() を用意。 自動テキスト編集モード時、編集中にクリックすると異なるサブアイテムが変更されないように修正。
 */
class CSubEditListCtrl : public CDrawingListCtrl
{
	DEFSUPER(CDrawingListCtrl);
public:

	/// コンストラクタ
	CSubEditListCtrl(void) : m_currentSubItem(-1), m_pSubclassEdit(NULL), m_isAutoEdit(false), m_editingSubItem(-1)
	{
	}

	/**
	 * [設定] 自動テキスト編集モード.
	 *	@note デフォルトは false です。
	 *	@param isEnable true なら編集結果を自動的に List Ctrl に反映させます。 false ならしません。
	 */
	void SetAutoEditMode(bool isEnable)
	{
		m_isAutoEdit = isEnable;
	}

	/**
	 * [設定] サブアイテムラベル編集開始.
	 *	@param iItem アイテム.
	 *	@param iSubItem サブアイテム
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool EditSubItemLabel(int iItem, int iSubItem)
	{
		m_currentSubItem = iSubItem;
		return _super::EditLabel(iItem) != NULL;
	}

	/**
	 * [設定] アイテムラベル編集開始.
	 *	@retval true 成功
	 *	@retval false 失敗。選択されていない時は失敗します。
	 */
	bool EditLabel(void)
	{
		int iItem = _super::GetSelectedItem();
		int iSubItem = m_currentSubItem;
		if ( iItem >= 0 && iSubItem >=0 )
		{
			return _super::EditLabel(iItem) != NULL;
		}
		return false;
	}

	/**
	 * [確認] 編集中確認.
	 *	@retval true ラベル編集中.
	 *	@retval false 否
	 */
	bool IsEditingLabel(void) const
	{
		return m_editingSubItem >= 0;
	}

	/**
	 * [取得] 選択アイテム取得.
	 *	@note 単一選択スタイル専用です。
	 *	@retval マイナス 選択なし
	 *	@retval ０以上 インデックス
	 */
	int GetSelectedItem(void) const
	{
		return _super::GetSelectedItem();
	}

	/**
	 * [取得] 選択サブアイテムNo取得.
	 *	@return サブアイテムNo
	 */
	int GetSelectedSubItem(void) const
	{
		return m_currentSubItem;
	}

	/**
	 * [設定] 選択アイテム設定.
	 *	@note 単一選択スタイル専用です。
	 *	@param item アイテムＮｏ
	 *	@param subItem サブアイテムＮｏ
	 */
	void SetSelectedItem(int item, int subItem)
	{
		m_currentSubItem = subItem;
		_super::SetSelectedItem(item);
	}

	/**
	 * [設定] アイテム表示.
	 *		指定のアイテムが見えるようにスクロールします。
	 *	@param item アイテムＮｏ
	 *	@param subItem サブアイテムＮｏ
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL EnsureVisibleEx(int item, int subItem)
	{
		BOOL r = _super::EnsureVisible(item, FALSE);
		m_ViewSubItem(subItem);
		return r;
	}

	/**
	 * [設定] デフォルトのマーク描画指定.
	 *		フォーカスマーク、選択マークはWindow標準と同じ様にします。
	 *	@param base ベースの色。省略すると Windows標準と同じ様にします。
	 */
	void SetDefaultMarkDrawer(COLORREF base = ::GetSysColor(COLOR_WINDOW))
	{
		_super::SetDefaultMarkDrawer(base);
		_super::SetSelectMarkDrawer(
			CMySelectDrawer(this, ::GetSysColor(COLOR_HIGHLIGHT), base, true),
			CMySelectDrawer(this, ::GetSysColor(COLOR_BTNFACE), base, false)
		);
		_super::SetFocusMarkDrawer(CMyFoucsDrawer(this));
		CTextDrawer tx(*GetFont(), RGB(0, 0, 0), _T(""), DT_END_ELLIPSIS);
		_super::SetTextDrawer(tx);
	}

	/**
	 * [設定] 選択マーク描画指定.
	 *	@note 本クラスでは、必ず失敗します。
	 *	@param draw Resize() が有効な選択マークの描画情報。
	 *	@param type 選択マークのタイプ。省略すると、 OVERLAP になります。
	 *	@return false 失敗。
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw, ESelectMarkType type = OVERLAP)
	{
		return false;
	}
	
	/**
	 * [設定] 選択マーク描画指定.
	 *	@note 本クラスでは、必ず失敗します。
	 *	@param draw1 Resize() が有効なアクティブ時の選択マークの描画情報。
	 *	@param draw2 Resize() が有効な非アクティブ時の選択マークの描画情報。
	 *	@param type 選択マークのタイプ。省略すると、 OVERLAP になります。
	 *	@return false 失敗。
	 */
	bool SetSelectMarkDrawer(const IDrawable& draw1, const IDrawable& draw2, ESelectMarkType type = OVERLAP)
	{
		return false;
	}
	
	/**
	 * [設定] フォーカスマーク描画指定.
	 *	@note 本クラスでは、必ず失敗します。
	 *	@param draw Resize() が有効なフォーカスマークの描画情報。
	 *	@return false 失敗。
	 */
	bool SetFocusMarkDrawer(const IDrawable& draw)
	{
		return false;
	}

	/**
	 * [取得] Editコントロール取得.
	 *	@retval NULL 失敗
	 *	@retval NULL以外 editのポインタ
	 */
	CEdit* GetEditControl(void) const
	{
		if ( m_pSubclassEdit != NULL )
		{
			return m_pSubclassEdit->GetEdit();
		}
		return NULL;
	}
	
protected:

	/**
	 * [参照] サブアイテム編集コントロール取得.
	 *	@param item アイテムＮｏ
	 *	@param subItem サブアイテムＮｏ
	 *	@retval NULL 編集不可
	 *	@retval NULL以外 編集用の EDIT コントロール
	 */
	virtual ISelcEditFp* GetSubItemEditControl(int item, int subItem)
	{
		return &m_myEdit;
	}

	/**
	 * [通知] アイテム描画.
	 *	@note サブアイテム毎に通知されます。
	 *	@param[in,out] P 描画用情報。範囲やアイテムNOが入っています。
	 *	@retval CDRF_NOTIFYITEMDRAW 通常の描画を行う(本メソッドでは何もしていない)。
	 *	@retval CDRF_NEWFONT フォントや文字色を変更した時返します。
	 *	@retval CDRF_SKIPDEFAULT 本メソッドで描画ずみ。
	 */
	virtual LRESULT OnItemPainting(TDrawParam* P)
	{
		m_drawingSubItem = P->subItemNo;
		return _super::OnItemPainting(P);
	}

	/**
	 * [通知] for notifications from parent
	 *	@note
	 *		CWndのメソッドをオーバーライドしています。
	 *		メッセージ受信したらコールされます。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE	処理済。
	 *	@retval FALSE	未処理。
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_NOTIFY )
		{
			LPNMHDR lpNmHdr = reinterpret_cast<LPNMHDR>(lParam);
			if ( lpNmHdr->hwndFrom == m_hWnd)
			{
				NMLVDISPINFO* lpLv = reinterpret_cast<NMLVDISPINFO*>(lParam);
				if ( lpLv->hdr.code == LVN_BEGINLABELEDIT )
				{
					LVITEM& item = lpLv->item;
					if ( m_currentSubItem >= 0 ) 
					{
						item.iSubItem = m_currentSubItem;
						m_editingSubItem = m_currentSubItem;
						if ( _super::GetItemDrawer(item.iItem, item.iSubItem) != NULL )
						{
							// 描画情報がある枠
							*_pResult = TRUE;
							return TRUE;
						}
						ISelcEditFp* pIEdit = GetSubItemEditControl(item.iItem, item.iSubItem);
						if ( pIEdit == NULL )
						{
							*_pResult = TRUE;
							return TRUE;
						}
						CEdit* pEdit = pIEdit->GetEdit();
						m_ViewSubItem(m_currentSubItem);
						CRect rect;
						_super::GetSubItemRect(item.iItem, m_currentSubItem, LVIR_LABEL, rect);
						CSize sz(max(rect.Width() + 2, 50), rect.Height() + 3);
						CRect clRc;
						GetClientRect(clRc);
						CPoint pos = rect.TopLeft();
						pos.x -= 2;
						pos.y -= 2;
						if ( pos.x + sz.cx > clRc.right )
						{
							pos.x = clRc.right - sz.cx;
						}
						if ( pos.x < 0 )
						{
							pos.x = 0;
						}
						CEdit* pNowEdit = _super::GetEditControl();
						pNowEdit->SetWindowText(GetItemText(item.iItem, m_currentSubItem));
						pIEdit->SetStartRect(CRect(pos, sz), clRc.Width());
						pEdit->SubclassWindow(pNowEdit->m_hWnd);
						m_pSubclassEdit = pIEdit;
					}
					return FALSE;
				}
				else if ( lpLv->hdr.code == LVN_ENDLABELEDIT )
				{
					if ( m_pSubclassEdit != NULL )
					{
						CEdit* pEd = m_pSubclassEdit->GetEdit();
						LVITEM& item = lpLv->item;
						if ( m_editingSubItem >= 0 && pEd->GetSafeHwnd() != NULL ) 
						{
							item.iSubItem = m_editingSubItem;
							m_editingSubItem = -1;
							if ( m_isAutoEdit && (item.mask & LVIF_TEXT) != 0 )
							{
								m_pSubclassEdit->OnEditEnd();
								CString s;
								pEd->GetWindowText(s);
								_super::SetItemText(item.iItem, item.iSubItem, s);
							}
							pEd->UnsubclassWindow();
							m_pSubclassEdit = NULL;
						}
					}
					return FALSE;
				}
			}
		} 
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *		ListBox 関係のメッセージをフックしています。
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
		case CMySelectDrawer::WM_SELC_DRAWINGSUBITEM:
			return m_drawingSubItem;
			break;
		case WM_KEYDOWN:
			if ( wParam == VK_F2 )
			{
				if ( m_currentSubItem >= 0 )
				{
					_super::EditLabel(_super::GetSelectedItem());
				}
			}
			else if ( wParam == VK_RIGHT || wParam == VK_LEFT )
			{
				CWnd* P = _super::GetParent();
				if ( P != NULL )
				{
					// 親に通知
					NMLVKEYDOWN nm;
					nm.hdr.code = LVN_KEYDOWN;
					nm.hdr.hwndFrom = _super::GetSafeHwnd();
					nm.hdr.idFrom = _super::GetDlgCtrlID();
					nm.wVKey = static_cast<WORD>(wParam);
					nm.flags = 0;
					P->SendMessage(WM_NOTIFY, nm.hdr.idFrom, reinterpret_cast<LPARAM>(&nm));
				}
				CHeaderCtrl* H = _super::GetHeaderCtrl();
				int m = H->GetItemCount();
				int c = m_currentSubItem;
				if ( wParam == VK_RIGHT )
				{
					if ( m_currentSubItem < 0 )
					{
						m_currentSubItem = 0;
					}
					else if ( m_currentSubItem < m - 1 )
					{
						m_currentSubItem++;
					}
				}
				else //if ( wParam == VK_LEFT )
				{
					if ( m_currentSubItem < 0 )
					{
						m_currentSubItem = m - 1;
					}
					else if ( m_currentSubItem > 0 )
					{
						m_currentSubItem--;
					}
				}
				if ( c != m_currentSubItem )
				{
					m_ViewSubItem(m_currentSubItem);
					CRect rc;
					_super::GetItemRect(GetSelectedItem(), rc, LVIR_BOUNDS);
					_super::InvalidateRect(rc, FALSE);
				}
				return 0;
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				LVHITTESTINFO ht;
				ht.pt = CPoint(lParam);
				if ( _super::SubItemHitTest(&ht) >= 0 )
				{
					if ( m_currentSubItem != ht.iSubItem )
					{
						m_currentSubItem = ht.iSubItem;
						DWORD ff = (_super::GetStyle() & LVS_EDITLABELS);	//編集スタイル状態取得
						_super::ModifyStyle(ff, 0);	//編集スタイルOFF。
						LRESULT r = _super::WindowProc(message, wParam, lParam);
						_super::ModifyStyle(0, ff);	//編集スタイル元へ。
						CRect rc;
						_super::GetItemRect(ht.iItem, rc, LVIR_BOUNDS);
						_super::InvalidateRect(rc, FALSE);
						return r;
					}
				}
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
		_super::PreSubclassWindow();
		_super::ModifyStyle(0, LVS_SINGLESEL);
		_super::SetExtendedStyle(LVS_EX_FULLROWSELECT | _super::GetExtendedStyle());
		_super::SetAdjustWidthMode(false);
		SetDefaultMarkDrawer();
	}

private:
	/// [設定] アイテムラベル編集開始.
	//	@note 使用禁止
	CEdit* EditLabel(int item);

	/// 指定のサブアイテムが見えるようにする
	void m_ViewSubItem(int subItem)
	{
		CRect rc;
		//見えている範囲
		GetClientRect(rc);
		int cx1 = rc.Width();
		int x1 = GetScrollPos(SB_HORZ);
		//見せたいアイテムの範囲
		CHeaderCtrl* H = _super::GetHeaderCtrl();
		H->GetItemRect(subItem, rc);
		int cx2 = rc.Width();
		int x2 = rc.left;
		//調整
		int xx = x1;
		if ( (x1 + cx1) < (x2 + cx2) )
		{
			xx = (x2 + cx2) - cx1;
		}
		if ( x1 > x2 )
		{
			xx = x2;
		}
		Scroll(CSize(xx - x1, 0));
	}
	/// 選択描画
	class CMySelectDrawer : public CSelectedBeltDrawer
	{
		DEFSUPER(CSelectedBeltDrawer);
	public:
		enum
		{
			WM_SELC_DRAWINGSUBITEM = WM_APP,
		};
		CMySelectDrawer(CSubEditListCtrl* H, COLORREF beltColor, COLORREF backColor, bool isTextInvert = true)
			: m_pCtrl(H), _super(beltColor, backColor, isTextInvert)
		{
		}
		// [作成] クローン作成.
		virtual IDrawable* Clone(void) const
		{
			CMySelectDrawer* P = new CMySelectDrawer(m_pCtrl, 0, 0, false);
			*P = *this;
			return P;
		}
		// [描画] 描画.
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			if ( m_pCtrl->SendMessage(WM_SELC_DRAWINGSUBITEM) == m_pCtrl->GetSelectedSubItem() )
			{
				_super::Draw(dc, x , y);
			}
		}
	private:
		CSubEditListCtrl*	m_pCtrl; ///< 親コントロール
	};

	/// フォーカス描画
	class CMyFoucsDrawer : public CNullDrawer
	{
	public:
		CMyFoucsDrawer(CSubEditListCtrl* H) : m_pCtrl(H)
		{
		}
		// [作成] クローン作成.
		virtual IDrawable* Clone(void) const
		{
			return new CMyFoucsDrawer(m_pCtrl);
		}
		// [描画] 描画.
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			CHeaderCtrl* H = m_pCtrl->GetHeaderCtrl();
			if ( H != NULL )
			{
				CRect rc;
				H->GetItemRect(m_pCtrl->GetSelectedSubItem(), rc);
				rc.OffsetRect(x, y);
				rc.bottom = rc.top + m_size.cy;
				::DrawFocusRect(dc, rc);
			}
		}
	private:
		CSubEditListCtrl*	m_pCtrl; ///< 親コントロール
	};

	class CMyEdit : public CSelcEditFpT<CEdit>
	{
	};

	CMyEdit			m_myEdit;			///< 自前のEditコントロール
	ISelcEditFp*	m_pSubclassEdit;	///< サブクラス化しているEditコントロール
	bool			m_isAutoEdit;		///< 自動テキスト更新モード
	int				m_currentSubItem;	///< 選択中サブアイテムNo
	int				m_drawingSubItem;	///< 描画中サブアイテムNo
	int				m_editingSubItem;	///< 編集中のサブアイテムNo
};



}; // MFC
}; // TNB
