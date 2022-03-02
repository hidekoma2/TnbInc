#pragma once
/**
 *@file
 * カスタム描画抽象ListCtrl関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * カスタム描画抽象ListCtrlコントロール
 *
 *	@note Reportタイプの一行全選択スタイルをサポートします。
 *
 *	@note OnBeginItemPaint() , OnEndItemPaint() , OnItemPainting() , OnItemPainted()
 *			の４つを実装する必要があります。
 *
 *	@par必要ファイル
 *			TnbMfcAbstractCustomListCtrl.h
 * 
 *	@date 07/12/07	新規作成
 *	@date 11/03/04	ReportViewの時、表示範囲に無い、サブアイテムの表示要求をしないようにした。
 *	@date 11/09/28	背景描画処理を最適化、 OnDrawBackground() を追加。
 *	@date 15/01/27	SetEraseBkGndOffset() を追加。
 */
class CAbstractCustomListCtrl : public CListCtrl
{
	DEFSUPER(CListCtrl);
public:

	/// コンストラクタ
	CAbstractCustomListCtrl(void) : _super(), m_isReportView(false)
		, m_countOfSelected(0), m_itemOfSelected(-1), m_eraseBkGndOffset(2)
	{
	}
	
	/**
	 * [取得] 選択アイテム取得.
	 *	@note 単一選択スタイル専用です。
	 *	@retval マイナス 選択なし
	 *	@retval ０以上 インデックス
	 */
	int GetSelectedItem(void) const
	{
		return _super::GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	}

	/**
	 * [設定] 選択アイテム設定.
	 *	@note 単一選択スタイル専用です。
	 *	@param item インデックス
	 */
	void SetSelectedItem(int item)
	{
		if ( item >= 0 )
		{
			_super::SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			_super::EnsureVisible(item, false);
		}
		else
		{
			int i = GetSelectedItem();
			if ( i >= 0 )
			{
				_super::SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}

	/**
	 * [設定] BGクリア調整値設定.
	 *	@param f 調整値。デフォルトは２です。
	 */
	void SetEraseBkGndOffset(int f = 2)
	{
		m_eraseBkGndOffset = f;
	}

protected:

	/// 表示用パラメータ
	struct TDrawParam
	{
		HDC			dc;			///< デバイスコンテキスト
		CRect		rect;		///< 描画範囲
		DWORD		itemNo;		///< アイテムナンバー
		DWORD		subItemNo;	///< サブアイテムナンバー
		UINT		state;	/**< 状態.
							 *	以下の値が OR された状態で設定されています。\n
							 *			<table><tr><td>
							 *			値											</td><td>
							 *					説明								</td></tr><tr><td>
							 *			CDIS_CHECKED								</td><td>
							 *					アイテムはチェックされています。	</td></tr><tr><td>
							 *			CDIS_DEFAULT								</td><td>
							 *					アイテムはデフォルト状態です。		</td></tr><tr><td>
							 *			CDIS_DISABLED								</td><td>
							 *					アイテムは使用不可になっています。	</td></tr><tr><td>
							 *			CDIS_FOCUS									</td><td>
							 *					アイテムはフォーカスされています。	</td></tr><tr><td>
							 *			CDIS_GRAYED									</td><td>
							 *					アイテムは灰色表示になっています。	</td></tr><tr><td>
							 *			CDIS_HOT									</td><td>
							 *					アイテムはhot状態になっています。	</td></tr><tr><td>
							 *			CDIS_INDETERMINATE							</td><td>
							 *					アイテムはinterminate状態になっています。</td></tr><tr><td>
							 *			CDIS_MARKED									</td><td>
							 *					アイテムはマークされています。		</td></tr><tr><td>
							 *			CDIS_SELECTED								</td><td>
							 *					アイテムは選択されています。		</td></tr>*/
		COLORREF	textColor;	///< テキストカラー
		COLORREF	backColor;	///< バックカラー
	};

	/**
	 * [通知] 描画開始.
	 *		一連の描画の開始を通知します。
	 *	@note dc は以下、 OnItemPainting() OnItemPainted() OnEndItemPaint() と続く一連の描画で共通で
	 *			使うデバイスコンテキストです。 OnItemPainting() などでデバイスコンテキストの
	 *			状態変更して戻すタイミングがない場合、本メソッドで記憶し、 OnEndItemPaint()
	 *			で元に戻すようにします。
	 *	@param dc デバイスコンテキスト。
	 */
	virtual void OnBeginItemPaint(HDC dc) = 0;

	/**
	 * [通知] 描画終了.
	 *		一連の描画の終了を通知します。
	 *	@see OnBeginItemPaint()
	 *	@param dc デバイスコンテキスト。
	 */
	virtual void OnEndItemPaint(HDC dc) = 0;

	/**
	 * [通知] アイテム描画.
	 *	@note サブアイテム毎に通知されます。
	 *	@param[in,out] pParam 描画用情報。範囲やアイテムNOが入っています。
	 *	@retval CDRF_NOTIFYITEMDRAW 通常の描画を行う(本メソッドでは何もしていない)。
	 *	@retval CDRF_NEWFONT フォントや文字色を変更した時返します。
	 *	@retval CDRF_SKIPDEFAULT 本メソッドで描画ずみ。
	 */
	virtual LRESULT OnItemPainting(TDrawParam* pParam) = 0;

	/**
	 * [通知] アイテム描画終わり.
	 *	@note サブアイテム描画が終わったあと、アイテム毎に通知されます。
	 *	@param[in] pParam 描画用情報。 rect は、サブアイテムも含む範囲。
	 */
	virtual void OnItemPainted(const TDrawParam* pParam) = 0;

	/**
	 * [通知] 選択変更通知.
	 *	@note 選択状態が変化した時に通知されます。
	 *	@param item 選択されているアイテムNO（複数ある場合、一番若いNO）。 -1 なら未選択。
	 */
	virtual void OnSelectItemChanged(int item) { }

	/**
	 * [通知] 背景描画通知.
	 *	@note 背景を描画するタイミングで通知されます。
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 描画範囲
	 */
	virtual void OnDrawBackground(HDC dc, const RECT& rect)
	{
		::FillRect(dc, &rect, ::GetSysColorBrush(COLOR_WINDOW));
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
				LPNMLISTVIEW lpLv = reinterpret_cast<LPNMLISTVIEW>(lParam);
				if ( lpLv->hdr.code == NM_CUSTOMDRAW ) 
				{
					LPNMLVCUSTOMDRAW lpLvCd = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
					*_pResult = m_OnCustomDraw(lpLvCd);
					return TRUE;
				}
				else if ( lpLv->hdr.code == LVN_ITEMCHANGED )
				{
					SetTimer(TIMERID_CHANGED, 50, NULL);
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
		case WM_ERASEBKGND:
			{
				CHeaderCtrl* H = GetHeaderCtrl();
				if ( H !=NULL )
				{
					HDC hdc = reinterpret_cast<HDC>(wParam);
					CRect clientRect;
					_super::GetClientRect(clientRect);
					CRect rect = clientRect;
					int headerHeight = 0;
					CRect rc;
					H->GetClientRect(&rc);
					headerHeight = rc.Height();
					rect.top += headerHeight;
					if ( _super::GetItemCount() != 0 )
					{
						CRect rc;
						_super::GetItemRect(_super::GetTopIndex(), &rc, LVIR_BOUNDS);
						OnDrawBackground(hdc, CRect(0, headerHeight, rect.Width(), rc.top));
						int y = _super::GetItemCount() - _super::GetTopIndex();
//						rect.top += y * rc.Height() + 1 + 1;
						rect.top += y * rc.Height() + m_eraseBkGndOffset;
						//
						if ( _super::GetItemRect (0, rc, LVIR_BOUNDS) )
						{
							if ( rc.right < clientRect.right)
							{
								rc.left = rc.right;
								rc.right = clientRect.right;
								rc.top = clientRect.top;
								rc.bottom = rect.top;
								OnDrawBackground(hdc, rc);
							}
						}
					}
					OnDrawBackground(hdc, rect);
				}
				return 0;
			}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			Invalidate();
			break;
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
			{
				SetRedraw(FALSE);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				SetRedraw(TRUE);
				return r;
			}
			break;
		case WM_TIMER:
			if ( wParam == TIMERID_CHANGED )
			{
				KillTimer(TIMERID_CHANGED);
				int count = _super::GetSelectedCount();
				int item = _super::GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
				if ( m_countOfSelected != count || m_itemOfSelected != item )
				{
					m_countOfSelected = count;
					m_itemOfSelected = item;
					OnSelectItemChanged(item);
				}
			}
			break;
		case WM_DESTROY:
			KillTimer(TIMERID_CHANGED);
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
		ASSERT0( (_super::GetStyle() & LVS_OWNERDRAWFIXED) == 0, "AbstractCustomListCtrl", "Ownerデータは OFFにしてください。" );
		_super::PreSubclassWindow();
		_super::SetExtendedStyle(LVS_EX_FULLROWSELECT);
	}

private:
	enum { TIMERID_CHANGED = 100 };
	/// Item描画サブ
	LRESULT m_OnItemPaintSub(LPNMLVCUSTOMDRAW P, int subItemNo)
	{
		TDrawParam pa;
		pa.dc			= P->nmcd.hdc;
		pa.itemNo		= ToDword(P->nmcd.dwItemSpec);
		pa.subItemNo	= subItemNo;
		pa.state		= P->nmcd.uItemState;
		pa.textColor	= P->clrText;
		pa.backColor	= P->clrTextBk;
		_super::GetSubItemRect(pa.itemNo, subItemNo, LVIR_BOUNDS, pa.rect);
		if ( m_isReportView && subItemNo == 0 )
		{
			pa.rect.right = pa.rect.left + _super::GetColumnWidth(subItemNo);
		}
		if ( m_isReportView && (pa.rect.right < 0 || pa.rect.left > m_clientWidth) )
		{
			// クライアント内にかかっていない
			return CDRF_SKIPDEFAULT;
		}
		if ( (_super::GetItemState(pa.itemNo, LVIS_SELECTED) & LVIS_SELECTED) != 0 )
		{
			pa.state |= CDIS_SELECTED;
		}
		else
		{
			pa.state &= ~CDIS_SELECTED;
		}
		LRESULT r = OnItemPainting(&pa);
		P->clrText = pa.textColor;
		P->clrTextBk = pa.backColor;
		return r;
	}
	/// カスタムドロー通知対応
	LRESULT m_OnCustomDraw(LPNMLVCUSTOMDRAW P)
	{
		switch ( P->nmcd.dwDrawStage )
		{
		case CDDS_PREPAINT:
			m_isReportView = (GetStyle() & LVS_TYPEMASK) == LVS_REPORT;
			m_clientWidth = 0x8000;
			if ( m_isReportView )
			{
				CRect rc;
				GetClientRect(rc);
				m_clientWidth = rc.right;
			}
			OnBeginItemPaint(P->nmcd.hdc);
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_POSTPAINT:
			OnEndItemPaint(P->nmcd.hdc) ;
			return CDRF_DODEFAULT;
		case CDDS_ITEMPOSTPAINT:
			{
				TDrawParam pa;
				pa.dc			= P->nmcd.hdc;
				pa.itemNo		= ToDword(P->nmcd.dwItemSpec);
				pa.subItemNo	= 0;
				pa.state		= P->nmcd.uItemState;
				pa.textColor	= P->clrText;
				pa.backColor	= P->clrTextBk;
				_super::GetItemRect(pa.itemNo, pa.rect, LVIR_BOUNDS);
				OnItemPainted(&pa);
			}
			break;
		case CDDS_ITEMPREPAINT:
			if ( ! m_isReportView )
			{
				return m_OnItemPaintSub(P, 0) | CDRF_NOTIFYPOSTPAINT;
			}
			return CDRF_NOTIFYSUBITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
			return m_OnItemPaintSub(P, P->iSubItem);
		}
		return CDRF_DODEFAULT;
	}

	bool	m_isReportView;			///< ビューモード？
	int		m_countOfSelected;		///< 選択数
	int		m_itemOfSelected;		///< 選択itemNo
	LONG	m_clientWidth;			///< クライアント領域幅
	int		m_eraseBkGndOffset;		///< BGクリア時の調整値

#ifdef _TnbDOXYGEN	//Document作成用シンボル
	public:
	/**
	 * [取得] アイテム数取得
	 *	@return アイテム数
	 */
	int GetItemCount() const;
	/**
	 * [取得] アイテム検索
	 *	@param nItem 検索を開始するインデックス。 -1 なら最初からになります。
	 *	@param nFlags 以下のいずれかを指定します。
 	 *		<table>
	 *		<tr><td>値</td>	<td>説明</td></tr>
	 *					<tr><td>
	 *		LVNI_ABOVE 		</td><td>
	 *				指定したアイテムの上にあるアイテムを検索します。</td>
	 *					</tr><tr><td>
	 *		LVNI_ALL		</td><td>
	 *				インデックス順で次のアイテムを検索します (既定値)。</td>
	 *					</tr><tr><td>
	 *		LVNI_BELOW		</td><td>
	 *				指定したアイテムの下にあるアイテムを検索します。</td>
	 *					</tr><tr><td>
	 *		LVNI_TOLEFT		</td><td>
	 *				指定したアイテムの左にあるアイテムを検索します。</td>
	 *					</tr><tr><td>
	 *		LVNI_TORIGHT		</td><td>
	 *				指定したアイテムの右にあるアイテムを検索します。</td>
	 *					</tr>
	 *		</table>
	 *		以下のフラグも合わせて指定できます。
 	 *		<table>
	 *		<tr><td>値</td>	<td>説明</td></tr>
	 *					<tr><td>
	 *		LVNI_DROPHILITED 		</td><td>
	 *				アイテムに LVIS_DROPHILITED 状態フラグが設定されている。</td>
	 *					</tr><tr><td>
	 *		LVNI_FOCUSED		</td><td>
	 *				アイテムに LVIS_FOCUSED 状態フラグが設定されている。</td>
	 *					</tr><tr><td>
	 *		LVNI_SELECTED		</td><td>
	 *				アイテムに LVIS_SELECTED 状態フラグが設定されている。</td>
	 *					</tr>
	 *		</table>
	 *	@retval マイナス 未発見。
	 *	@retval 0以上 発見。数値はインデックスNo。
	 */
	int GetNextItem(int nItem, int nFlags) const;
	/**
	 * [取得] 選択アイテム検索.
	 *	@retval NULL 未発見。
	 *	@retval NULL以外 発見。 GetNextSelectedItem() で使用できます。
	 */
	POSITION GetFirstSelectedItemPosition() const;
	/**
	 * [取得] 選択アイテム検索.
	 *	@param[in,out] _pos 前の GetNextSelectedItem() または 
	 *					GetFirstSelectedItemPosition() の呼び出しで返された 
	 *					POSITION 値への参照。この値は、今回の呼び出しによって
	 *					次の位置に更新されます。
	 *	@note 本関数を使う前に _pos 値が NULL 以外であることを確認する必要があります。
	 *	@retval 発見したインデックスNo。
	 */
	int GetNextSelectedItem(POSITION& _pos) const;
	/**
	 * [取得] 一番上表示のアイテム
	 *	@return 一番上に表示されているアイテムのインデックス
	 */
	int GetTopIndex() const;
	/**
	 * [取得] 垂直方向に表示されるアイテム数
	 *	@return 表示領域の垂直方向に入るアイテム数を返します。
	 */
	int GetCountPerPage() const;
	/**
	 * [設定] アイテム状態設定
	 *	@param nItem インデックス
	 *	@param nState {@link CAbstractCustomListCtrl::EState 状態ビット} の新しい値。
	 *	@param nMask どの {@link CAbstractCustomListCtrl::EState 状態ビット} を変更するかを指定する値。
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOL SetItemState(int nItem, UINT nState, UINT nMask);
	/**
	 * [取得] アイテム状態取得
	 *	@param nItem インデックス
	 *	@param nMask どの {@link CAbstractCustomListCtrl::EState 状態ビット} を返すか指定する値。
	 *	@return 指定アイテムの {@link CAbstractCustomListCtrl::EState 状態ビット} 。
	 */
	UINT GetItemState(int nItem, UINT nMask) const;
	/**
	 * [取得] 選択アイテム数取得.
	 *	@return 選択されているアイテム数を返します。
	 */
	UINT GetSelectedCount() const;
//	CString GetItemText(int nItem, int nSubItem) const;
//	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
//	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
//	BOOL SetItemData(int nItem, DWORD dwData);
//	DWORD GetItemData(int nItem) const;
	/**
	 * アイテム状態値.
	 *	@note 実際は define で宣言されています。
	 */
	enum EState
	{
		LVIS_ACTIVATING,	///< Not currently supported.
		LVIS_CUT,			///< The item is marked for a cut-and-paste operation.
		LVIS_DROPHILITED,	///< The item is highlighted as a drag-and-drop target.
		LVIS_FOCUSED,		///< The item has the focus, so it is surrounded by a standard focus rectangle. Although more than one item may be selected, only one item can have the focus.
		LVIS_OVERLAYMASK,	///< The item's overlay image index is retrieved by a mask.
		LVIS_SELECTED,		///< The item is selected. The appearance of a selected item depends on whether it has the focus and also on the system colors used for selection.
		LVIS_STATEIMAGEMASK	///< The item's state image index is retrieved by a mask.
	};
#endif
};



}; // MFC
}; // TNB
