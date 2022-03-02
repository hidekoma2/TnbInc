#pragma once
/**
 *@file
 * 描画情報ListCtrl関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcAbstractCustomListCtrl.h"
#include "TnbMfcAbstractDrawingCtrl.h"
#include "TnbMfcDrawingHeaderCtrl.h"
#include "TnbMfcBitmapDC.h"
#include "TnbPartsDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報ListCtrlコントロール
 *
 *		行単位で表示する描画情報を指定出来ます。
 *
 *	@note アイテムの高さを変更するには、フォントを設定してください。
 *	@note ドラッグ＆ドロップをサポートするなら、 CDragDrawingListCtrl を使用してください。
 *	@note ヘッダの高さを変更するには、ヘッダのフォントを設定してください。
 *
 *	@note 描画時、 {@link IDrawable::DrawEx() DrawEx()メソッド} を使用しています。 lParam には、アイテムNoが入ります。
 *		lParam の値を変更するには、 CalcDrawParam() を継承します。
 *
 *	@par使い方
 *		リソースエディタでリストコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListCtrl）を本クラスに置き換えて使用します。\n
 *		リストコントロールのプロパティは、 レポートにしておいてください。
 *		（本クラスはレポート形式のみサポートしています）
 *
 *	@note プロパティで「ヘッダをソートしない」にするとマウスによるカラムの幅変更も禁止しています。
 *		この状態で幅変更をさせたい場合、 GetHeaderCtrl()->EnableChangeWidth(true); とします。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingListCtrl.h
 * 
 *	@date 07/12/07	新規作成
 *	@date 08/01/18	絶対INDEX -> itemNOテーブル追加。
 *	@date 08/06/17	カラム無し時の自動幅調整に対応。
 *	@date 08/09/16	CheckBoxありをサポート。
 *	@date 10/01/06	テキストの場合、表示位置をカラムの設定と同じになるようにした。
 *	@date 10/01/07	Destroy時のマーク関係の初期化漏れ修正。
 *	@date 10/02/26	描画時、 IDrawable::DrawEx() を使うように変更。 CalcDrawParam() 追加。
 *	@date 11/09/28	背景描画を親クラスに任せた。 OnDrawBackground() を追加。
 *	@date 14/12/14	SetItemText() 時に描画情報をクリアするように修正。
 *	@date 15/04/01	SetTextEndEllipsisMode() を追加。
 */
class CDrawingListCtrl : public CAbstractCustomListCtrl, public CAbstractDrawingCtrl
{
	DEFSUPER(CAbstractCustomListCtrl);
	typedef CAbstractDrawingCtrl _mark;					///< SelectMark管理名再宣言
	typedef CAutoVectorT<IDrawable::Ptr> CDrawersVector;	///< 描画情報の配列型
	typedef CAutoVectorT<CDrawersVector> CDrawersVectors;	///< 描画情報の二次元配列型
public:

	/// コンストラクタ
	CDrawingListCtrl(void) : _super(), m_pPartsDrawer(NULL), m_withDtEndEllipsis(false)
	{
	}

	/// デストラクタ
	~CDrawingListCtrl(void)
	{
		m_ResetContent();
	}

	/**
	 * [設定] 幅自動調整モード設定.
	 *	@note デフォルトでは true です。
	 *	@param b true にすると、幅をクライアントと同じになるように調整します(横スクロールバーが出ない)。
	 *			 false はオリジナルの CListCtrl と同じ挙動になります。
	 */
	void SetAdjustWidthMode(bool b = true)
	{
		m_headerCtrl.SetAdjustWidthMode(b);
	}

	/**
	 * [設定] テキスト省略表示モード設定.
	 *	@note デフォルトでは false です。
	 *	@param f true すると、カラムに入りきらない文字の最後を ... にします(CListCtrl と同じ)。
	 */
	void SetTextEndEllipsisMode(bool f = false)
	{
		m_withDtEndEllipsis = f;
	}

	/**
	 * [設定] チェックボックス追加
	 *	@note リストの左端にチェックボックスをつけます。
	 *	@param P チェックボックス描画用。 DFCS_BUTTONCHECK , DFCS_FLAT の DFCS_CHECKED ありなしが使用されます。
	 */
	void AddCheckBox(IPartsDrawable* P)
	{
		m_pPartsDrawer = P;
		DWORD ss = _super::GetExtendedStyle();
		_super::SetExtendedStyle(ss | LVS_EX_CHECKBOXES);
	}

	/**
	 * [取得] アイテムの描画情報取得.
	 *	@param itemNo アイテムNO.
	 *	@param subItemNo サブアイテムNO.
	 *	@retval NULL エラー.
	 *	@retval NULL以外 描画情報。delete してはいけません。
	 */
	IDrawable* GetItemDrawer(int itemNo, int subItemNo)
	{
		if ( itemNo >= 0 && itemNo < GetItemCount() )
		{
			if ( subItemNo >= 0 && subItemNo < GetHeaderCtrl()->GetItemCount() )
			{
				try
				{
					return m_drawersGlid[itemNo][subItemNo];
				}
				catch( CTnbException& e )
				{
					e.OnCatch();
				}
			}
		}
		return NULL;
	}

	/**
	 * [設定] アイテムの文字列設定.
	 *	@param itemNo アイテムNO.
	 *	@param subItemNo サブアイテムNO.
	 *	@param lpszText テキスト。
	 *	@retval FALSE エラー。
	 *	@retval TRUE 成功。
	 */
	BOOL SetItemText(int itemNo, int subItemNo, LPCTSTR lpszText)
	{
		BOOL r = _super::SetItemText(itemNo, subItemNo, lpszText);
		if ( r )
		{
			try
			{
				m_drawersGlid[itemNo][subItemNo].Null();
			}
			catch( CTnbException& e )
			{
				e.OnCatch();
			}
		}
		return r;
	}

	/**
	 * [設定] アイテムの描画情報設定.
	 *	@note ItemDataには絶対INDEXが格納されます。
	 *	@param itemNo アイテムNO.
	 *	@param subItemNo サブアイテムNO.
	 *	@param draw 描画情報。 複製を記憶しますので、 draw は破棄してかまいません。
	 *	@param isInsert 省略か false なら、上書き。 true なら挿入します。その際 subItemNo は無視します。
	 *	@param lpszWidth 幅設定用文字列。 省略すると、 draw から幅を取得し、使用します。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。数値は、追加したアイテムNo。
	 */
	int SetDrawItem(int itemNo, int subItemNo, const IDrawable& draw, bool isInsert = false, LPCTSTR lpszWidth = NULL)
	{
		CSize size;
		if ( ! draw.GetSize(size) )
		{
			return -1;
		}
		CSize sz(0, 0);
		CString str;
		if ( lpszWidth == NULL )
		{
			CDC* pDC = _super::GetDC();
			CFont* pOldFont = pDC->SelectObject(_super::GetFont());
			while ( sz.cx < size.cx )
			{
				str += _T("W");
				CTextDrawer::CalcTextSize(sz, *pDC, 0, str);
			}
			str = str.Left(str.GetLength() - 1);
			sz.cx = 0;
			while ( sz.cx < size.cx )
			{
				str += _T("l");
				CTextDrawer::CalcTextSize(sz, *pDC, 0, str);
			}
			str = str.Left(str.GetLength() - 1);
			pDC->SelectObject(pOldFont);
			_super::ReleaseDC(pDC);
		}
		else
		{
			str = lpszWidth;
		}
		_super::SetRedraw(false);
		if ( isInsert )
		{
			itemNo = _super::InsertItem(itemNo, str);
			if ( itemNo >= 0 )
			{
				subItemNo = 0;
				INDEX n = m_FindNoUseAbsoluteIndex();
				_super::SetItemData(itemNo, n);
				m_drawersGlid.Insert(itemNo, CDrawersVector());
				m_drawersLine.Insert(itemNo, IDrawable::Ptr());
				m_absoluteIndexs.Insert(itemNo, n);
				m_ClearIndexCache();
			}
		}
		else if ( subItemNo == 0 && itemNo >= _super::GetItemCount() )
		{
			itemNo = _super::InsertItem(itemNo, str);
			if ( itemNo >= 0 )
			{
				INDEX n = m_FindNoUseAbsoluteIndex();
				_super::SetItemData(itemNo, n);
				m_absoluteIndexs[itemNo] = n;
				m_ClearIndexCache();
			}
		}
		else
		{
			if ( ! _super::SetItemText(itemNo, subItemNo, str) )
			{
				itemNo = -1;
			}
		}
		_super::SetRedraw(true);
		if ( itemNo >= 0 )
		{
			m_drawersGlid[itemNo][subItemNo] = draw.Clone();
			_super::Invalidate();
		}
		m_ViewIndexCache();
		return itemNo;
	}

	/**
	 * [設定] アイテム描画情報追加.
	 *	@note ItemDataには絶対INDEXが格納されます。
	 *	@param draw 描画情報。 複製を記憶しますので、 draw は破棄してかまいません。
	 *	@retval -1 エラー。
	 *	@retval 0以上 成功。数値は itemNo。
	 */
	int AddDrawItem(const IDrawable& draw)
	{
		int l = _super::GetItemCount();
		return SetDrawItem(l, 0, draw);
	}

	/**
	 * [設定] アイテム描画情報挿入.
	 *	@note ItemDataには絶対INDEXが格納されます。
	 *	@param itemNo 挿入場所。
	 *	@param draw 描画情報。 複製を記憶しますので、 draw は破棄してかまいません。
	 *	@retval -1 エラー。
	 *	@retval 0以上 成功。数値は itemNo。
	 */
	int InsertDrawItem(int itemNo, const IDrawable& draw)
	{
		return SetDrawItem(itemNo, 0, draw, true);
	}

	/**
	 * [設定] アイテム入れ替え
	 *	@param itemNo1 入れ替え対象 ItemNO 一つ目。 
	 *	@param itemNo2 入れ替え対象 ItemNO 二つ目。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SwapItem(int itemNo1, int itemNo2)
	{
		int l = _super::GetItemCount();
		if ( itemNo1 < 0 || itemNo1 >= l ) { return false; }
		if ( itemNo2 < 0 || itemNo2 >= l ) { return false; }
		//
		_super::SetRedraw(false);
		m_SwapItem(itemNo1, itemNo2);	// 情報入れ替え
		_super::SetRedraw(true);
		_super::Invalidate();
		m_ViewIndexCache();
		return true;
	}
		
	/**
	 * [設定] アイテム入れ替え.
	 *	@param fromItemNo 移動対象 ItemNO。 
	 *	@param toItemNo 挿入先ItemNO。
	 *	@retval マイナス 失敗
	 *	@retval 0以上 成功。数値は、挿入先のItemNo
	 */
	int MoveItem(int fromItemNo, int toItemNo)
	{
		int l = _super::GetItemCount();
		if ( fromItemNo < 0 || fromItemNo >= l ) { return -1; }
		if ( toItemNo < 0 || toItemNo >= l + 1 ) { return -1; }
		if ( fromItemNo == toItemNo )//|| fromItemNo == toItemNo + 1 )
		{
			return fromItemNo; //何もせずOKじゃ
		}
		_super::SetRedraw(false);
		int it = -1;
		if ( fromItemNo > toItemNo )
		{
			for ( int i = fromItemNo; i >= toItemNo + 1; i-- )
			{
				m_SwapItem(i, i - 1);
			}
			it = toItemNo;
		}
		else
		{
			for ( int i = fromItemNo; i <= toItemNo - 2; i++ )
			{
				m_SwapItem(i, i + 1);
			}
			it = toItemNo - 1;
		}
		_super::SetRedraw(true);
		m_ViewIndexCache();
		return it;
	}

	/**
	 * [削除] アイテム描画情報削除.
	 *	@param item itemNo。
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL DeleteItem(int item)
	{
		BOOL r = _super::DeleteItem(item);
		if ( r )
		{
			m_drawersGlid.Remove(item);
			m_drawersLine.Remove(item);
			m_absoluteIndexs.Remove(item);
			m_ClearIndexCache();
			m_ViewIndexCache();
		}
		return r;
	}

	/**
	 * [削除] 全アイテム削除.
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL DeleteAllItems(void)
	{
		BOOL r = _super::DeleteAllItems();
		m_ResetContent();
		m_ViewIndexCache();
		return r;
	}

	/**
	 * [取得] 絶対Index から ItemNo 取得
	 *	@param index 絶対INDEX
	 *	@retval -1 見つからない。
	 *	@retval ０以上 ItemNo。
	 */
	int AbsoluteIndexToItemNo(INDEX index) const
	{
		return m_AbsoluteIndexToItemNo(index);
	}

	/**
	 * [取得] ItemNo から 絶対 Index 取得
	 *	@param itemNo アイテムNO。
	 *	@retval INVALID_INDEX 見つからない。
	 *	@retval INVALID_INDEX以外 絶対INDEX。
	 */
	INDEX ItemNoToAbsoluteIndex(int itemNo) const
	{
		if ( m_absoluteIndexs.IsInRange(itemNo) )
		{
			return m_absoluteIndexs[itemNo];
		}
		return INVALID_INDEX;
	}

	/**
	 * [取得] ヘッダコントロール参照.
	 *	@retval NULL ヘッダはありません。
	 *	@retval NULL以外 ヘッダコントロールのポインタ。
	 */
	CDrawingHeaderCtrl* GetHeaderCtrl(void)
	{
		m_CheckHeaderCtrl();
		return m_headerCtrl.IsWindow() ? &m_headerCtrl : NULL;
	}

	/**
	 * [追加] カラム追加.
	 *	@param nCol 新しい列のインデックス
	 *	@param pColumn 新しい列の属性情報
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。値は、新しい列のインデックス。
	 */
	int InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{
		m_CheckHeaderCtrl();
		int r = _super::InsertColumn(nCol, pColumn);
		return r;
	}

	/**
	 * [追加] カラム追加.
	 *	@param nCol 新しい列のインデックス
	 *	@param lpszColumnHeading 列ヘッダーを持つ文字列。
	 *	@param nFormat 列の配置を指定する整数。次の値のどれか 1 つを指定します。
	 *					LVCFMT_LEFT、LVCFMT_RIGHT、または LVCFMT_CENTER。
	 *	@param nWidth ピクセル単位の列の幅。このパラメータに -1 を指定すると、列の幅は設定されません。
	 *	@param nSubItem 列に関連付けられたサブアイテムのインデックス。
	 *					このパラメータに -1 を指定すると、列に関連付けられるサブアイテムはありません。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。値は、新しい列のインデックス。
	 */
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1)
	{
		m_CheckHeaderCtrl();
		int r = _super::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
		if ( r == 0 )
		{
			HDITEM hi;
			hi.mask = HDI_FORMAT;
			if ( m_headerCtrl.GetItem(nCol, &hi) )
			{
				hi.fmt = (hi.fmt & ~LVCFMT_JUSTIFYMASK) | nFormat;
				hi.mask = HDI_FORMAT;
				m_headerCtrl.SetItem(nCol, &hi);
			}
		}
		//m_headerCtrl.Reset();
		return r;
	}

	/**
	 * [追加] カラム追加.
	 *	@param nCol 新しい列のインデックス
	 *	@param nWidth ピクセル単位の列の幅。このパラメータに -1 を指定すると、列の幅は設定されません。
	 *	@param draw 表示する描画情報。複製を保持しますので、 draw は破棄してもかまいません。
	 *	@retval マイナス エラー。
	 *	@retval 0以上 成功。値は、新しい列のインデックス。
	 */
	int InsertColumn(int nCol, int nWidth, const IDrawable& draw)
	{
		m_CheckHeaderCtrl();
		int r = _super::InsertColumn(nCol, _T(""), LVCFMT_LEFT, nWidth, -1);
		if ( r >= 0 )
		{
			m_headerCtrl.SetColumnDrawer(nCol, draw);
			//m_headerCtrl.Reset();
		}
		return r;
	}

	/**
	 * [設定] 上書き描画情報設定.
	 *	@note 全サブアイテム描画後に、アイテム一列に対して描画する設定を行います。
	 *	@param itemNo アイテムNO.
	 *	@param draw 描画情報。 複製を記憶しますので、 draw は破棄してかまいません。
	 *	@retval false エラー。
	 *	@retval true 成功。
	 */
	bool SetItemOverDrawer(int itemNo, const IDrawable& draw)
	{
		SIZE size;
		if ( draw.GetSize(size) )
		{
			m_drawersLine[itemNo] = draw.Clone();
			return true;
		}
		return false;
	}

	/**
	 * [設定] 背景色設定.
	 *	@param color 色
	 *	@return 常に TRUE
	 */
	BOOL SetBkColor(COLORREF color)
	{
		SetBackColor(color);
		return TRUE;
	}
	
	/**
	 * [設定] アイテム文字列情報挿入.
	 *	@param itemNo 挿入場所。
	 *	@param lpszItem 描画文字列情報。
	 *	@retval -1 エラー。
	 *	@retval 0以上 成功。数値は itemNo。
	 */
	int InsertItem(int itemNo, LPCTSTR lpszItem)
	{
		return _super::InsertItem(itemNo, lpszItem);
	}

	/**
	 * [設定] アイテム状態設定
	 *	@param nItem インデックス
	 *	@param nState {@link CAbstractCustomListCtrl::EState 状態ビット} の新しい値。
	 *	@param nMask  どの {@link CAbstractCustomListCtrl::EState 状態ビット} を変更するかを指定する値。
	 *	@retval TRUE 成功.
	 *	@retval FALSE 失敗.
	 */
	BOOL SetItemState(int nItem, UINT nState, UINT nMask)
	{
		return _super::SetItemState(nItem, nState, nMask);
	}

protected:

	/**
	 * [通知] 描画開始.
	 *		一連の描画の開始を通知します。
	 *	@note dc は以下、OnItemPaint() OnEndItemPaint() と続く一連の描画で共通で
	 *			使うデバイスコンテキストです。OnItemPaint() でデバイスコンテキストの
	 *			状態変更して戻すタイミングがない場合、本メソッドで記憶し、 OnEndItemPaint()
	 *			で元に戻すようにします。
	 *	@param dc デバイスコンテキスト。
	 */
	virtual void OnBeginItemPaint(HDC dc)
	{
		_mark::BeginItemPaint();
		m_hasCheckBox = (GetExtendedStyle() & LVS_EX_CHECKBOXES) != 0;
		CDrawingHeaderCtrl* H = GetHeaderCtrl();
		if ( H != NULL )
		{
			CRect rc;
			H->GetClientRect(&rc);
			HRGN rgn = ::CreateRectRgnIndirect(&rc);
			::ExtSelectClipRgn(dc, rgn, RGN_DIFF);
			_DeleteObject(rgn);
		}
		if ( (_super::GetStyle() & LVS_NOCOLUMNHEADER) != 0 )
		{
			if ( m_headerCtrl.IsAdjustWidthMode() )
			{
				CRect rc;
				_super::GetClientRect(rc);
				int right = rc.right;
				int cnt = m_headerCtrl.GetItemCount();
				for ( int i = 0; i < cnt; i++ )
				{
					CRect subRc;
					m_headerCtrl.GetItemRect(i, subRc);
					rc.right = rc.left + subRc.Width();
					bool r = rc.right > right;
					if ( i + 1 == cnt )
					{
						r = rc.right != right;
					}
					if ( r )
					{
						HDITEM h;
						h.mask = HDI_WIDTH;
						h.cxy = right - rc.left;
						if ( h.cxy < 0 ) { h.cxy = 0; }
						m_headerCtrl.SetItem(i, &h);
						rc.right = right;
					}
					rc.left = rc.right;
				}
			}
		}
	}

	/**
	 * [通知] 描画終了.
	 *		一連の描画の終了を通知します。
	 *	@see OnBeginItemPaint()
	 *	@param dc デバイスコンテキスト。
	 */
	virtual void OnEndItemPaint(HDC dc)
	{
		if ( _mark::EndItemPaint() )
		{
			Invalidate();
		}
	}

	/**
	 * [取得] 描画用パラメータ計算.
	 *		描画情報の IDrawable::DrawEx() の lParam に渡す値を計算します。
	 *	@note 本メソッドでは、 itemNo を lParam としてます。
	 *	@param itemNo アイテム No.
	 *	@param subItemNo サブアイテム No.
	 *	@return パラメータ.
	 */
	virtual LPARAM CalcDrawParam(DWORD_PTR itemNo, DWORD subItemNo) const
	{
		return itemNo;
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
//		TRACE3("OnSubItemPaint[%d, %d, %08X]\n", pParam->itemNo, pParam->subItemNo, pParam->state);
		CRect rc = P->rect;
		rc.OffsetRect(-rc.TopLeft());
		CBitmapImage bi;
		bi.Set(rc.Width(), rc.Height(), _mark::GetBackColor());
		LPARAM lParam = CalcDrawParam(P->itemNo, P->subItemNo);
		if ( ! bi.IsEmpty() )
		{
			CBitmapDC dc(&bi);
			_mark::DrawBackground(dc, rc, lParam);
			int x = 0;
			if ( P->subItemNo == 0 && m_hasCheckBox && m_pPartsDrawer != NULL )
			{
				CRect r = rc;
				UINT s = DFCS_BUTTONCHECK | DFCS_FLAT;
				if ( _super::GetCheck(P->itemNo) )
				{
					s |= DFCS_CHECKED;
				}
				m_pPartsDrawer->DrawButtonControl(NULL, &r, s);
				r.OffsetRect(1, (rc.Height() - r.Height()) / 2);
				m_pPartsDrawer->DrawButtonControl(dc, &r, s);
				m_checkboxWidth = r.Width() + 2;
				x += m_checkboxWidth;
				rc.left += m_checkboxWidth;
				if ( rc.right < rc.left )
				{
					rc.right = rc.left;
				}
			}
			if ( _mark::GetSelectMarkType() == BACKGROUND )
			{
				m_DrawSelectMaker(dc, rc, P->state, lParam);
			}
			IDrawable* pDraw = GetItemDrawer(P->itemNo, P->subItemNo);
			if ( pDraw != NULL )
			{
				pDraw->Resize(rc.Size());
				pDraw->DrawEx(dc, x, 0, lParam);
			}
			else
			{
				CString str = GetItemText(P->itemNo, P->subItemNo);
				HDITEM hi;
				hi.mask = HDI_FORMAT;
				if ( m_headerCtrl.GetItem(P->subItemNo, &hi) )
				{
					DWORD st = _mark::GetTextDrawStyle() & (~0x03);
					switch ( hi.fmt & HDF_JUSTIFYMASK )
					{
					case HDF_RIGHT:
						st |= DT_RIGHT;
						break;
					case HDF_CENTER:
						st |= DT_CENTER;
						break;
					default:
						if ( m_withDtEndEllipsis )
						{
							st |= DT_END_ELLIPSIS;
						}
						break;
					}
					CSize sz = P->rect.Size();
					_mark::SetDefaultTextDrawer(this);
					_mark::DrawText(dc, CRect(x, 0, sz.cx, sz.cy), str, st, lParam);
				}
			}
			if ( _mark::GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(dc, rc, P->state, lParam);
			}
			dc.Draw(P->dc, P->rect.left, P->rect.top);
			if ( pDraw != NULL )
			{
				pDraw->DrawEx(NULL, P->rect.left, P->rect.top, lParam);
			}
		}
		return CDRF_SKIPDEFAULT;
	}

	/**
	 * [通知] アイテム描画終わり.
	 *	@note サブアイテム描画が終わったあと、アイテム毎に通知されます。
	 *	@note フォーカス線を描いています。
	 *	@param[in] P 描画用情報。 rect は、サブアイテムも含む範囲。
	 */
	virtual void OnItemPainted(const TDrawParam* P)
	{
		CRect rc = P->rect;
		if ( m_hasCheckBox && m_pPartsDrawer != NULL )
		{
			rc.left += m_checkboxWidth;
		}
		if ( ! m_drawersLine.IsEmpty() )
		{
			IDrawable* D = m_drawersLine[P->itemNo];
			if ( D != NULL )
			{
				D->Resize(rc.Size());
				D->Draw(P->dc, rc.left, rc.top);
			}
		}
		if ( (P->state & CDIS_FOCUS) != 0 )
		{
			_mark::DrawFocusMark(P->dc, rc, P->itemNo);
		}
	}

	/**
	 * [通知] 背景描画通知.
	 *	@note 背景を描画するタイミングで通知されます。
	 *	@param dc デバイスコンテキスト。
	 *	@param rect 描画範囲
	 */
	virtual void OnDrawBackground(HDC dc, const RECT& rect)
	{
		_mark::DrawBackColor(dc, rect);
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
		case LVM_DELETECOLUMN:
			{
				INDEX col = wParam;
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				if ( r != 0 )
				{
					loop ( i, m_drawersGlid )
					{
						m_drawersGlid[i].Remove(col);
					}
				}
				return r;
			}
			break;
		case WM_DESTROY:
			m_ResetContent();
			_mark::AllReset();
			if ( ::IsWindow(m_headerCtrl) )
			{
				m_headerCtrl.UnsubclassWindow();
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
		ASSERT0( (_super::GetStyle() & LVS_TYPEMASK) == LVS_REPORT, "DrawingListCtrl", "サポートできるのは Reportタイプのみです。" );
		m_CheckHeaderCtrl();
		_super::PreSubclassWindow();
	}

private:
	void m_CheckHeaderCtrl(void)
	{
		if ( ! ::IsWindow(m_headerCtrl) )
		{
			CHeaderCtrl* pHeaderCtrl = _super::GetHeaderCtrl();
			if ( pHeaderCtrl != NULL )
			{
				m_headerCtrl.SubclassWindow(*pHeaderCtrl);
			}
		}
	}
	CDrawingHeaderCtrl			m_headerCtrl;				///< ヘッダ部分のコントロール
	CDrawersVectors				m_drawersGlid;				///< それぞれの行、カラムの描画情報
	CDrawersVector				m_drawersLine;				///< それぞれの行の描画情報
	CAutoVectorT<INDEX>			m_absoluteIndexs;			///< 各アイテムの絶対インデックス
	mutable CAutoVectorT<INDEX>	m_indexCache;				///< 各絶対インデックスのアイテム
	IPartsDrawable*				m_pPartsDrawer;				///< パーツ描画(チェックボックス用)
	int							m_checkboxWidth;			///< チェックボックスの幅(ワーク)
	bool						m_hasCheckBox;				///< チェックボックスの有無(ワーク)
	bool						m_withDtEndEllipsis;		///< DT_END_Ellipsis あり？

	/// キャッシュ表示(開発用)
	void m_ViewIndexCache(void) const
	{
		#ifdef _TnbDEBUG_ON
		m_MakeIndexCache();
		TRACE1( "LL;cache 内容 (l = %d)\n", m_indexCache.GetSize() - 1 );
		loop ( i, m_indexCache.GetSize() )
		{
			if ( i != 0 )
			{
				TRACE2("LL;   絶対INDEX %d  ==  itemNo %d\n", i, m_indexCache[i] - 1);
			}
		}
		#endif
	}
	/// INDEXキャッシュクリア
	void m_ClearIndexCache(void)
	{
		m_indexCache.RemoveAll();
	}
	/// INDEXキャッシュ作成
	void m_MakeIndexCache(void) const
	{
		if ( m_indexCache.IsEmpty() )
		{
			m_indexCache.Add(0);
			loop ( i, m_absoluteIndexs.GetSize() )
			{
				INDEX idx = m_absoluteIndexs[i];
				m_indexCache[idx] = i + 1;
			}
		}
	}
	/// 未使用の絶対インデックス取得
	INDEX m_FindNoUseAbsoluteIndex(void) const
	{
		m_MakeIndexCache();
		INDEX r = m_indexCache.Find(0, 1);
		if ( r == INVALID_INDEX )
		{
			r = m_indexCache.GetSize();
		}
		return r;
	}
	int m_AbsoluteIndexToItemNo(INDEX index) const
	{
		m_MakeIndexCache();
		int r = static_cast<int>(m_indexCache[index]) - 1;
		ASSERT( r >= 0 );
		return r;
	}
	// アイテム全けし
	void m_ResetContent(void)
	{
		m_drawersGlid.RemoveAll();
		m_drawersLine.RemoveAll();
		m_absoluteIndexs.RemoveAll();
		m_ClearIndexCache();
	}
	/// 選択マーク描画
	void m_DrawSelectMaker(HDC dc, const RECT& rect, UINT itemState, LPARAM lParam)
	{
		if ( (itemState & CDIS_SELECTED) != 0 )
		{
			_mark::DrawSelectMark(dc, rect, ::GetFocus() == m_hWnd, lParam);
		}
	}
	/// アイテム入れ替え
	void m_SwapItem(int itemNo1, int itemNo2)
	{
		// 情報入れ替え
		Swap(m_drawersGlid[itemNo1], m_drawersGlid[itemNo2]);
		Swap(m_drawersLine[itemNo1], m_drawersLine[itemNo2]);
		Swap(m_absoluteIndexs[itemNo1], m_absoluteIndexs[itemNo2]);
		DWORD data = ToDword(_super::GetItemData(itemNo1));
		_super::SetItemData(itemNo1, _super::GetItemData(itemNo2));
		_super::SetItemData(itemNo2, data);
		UINT state = _super::GetItemState(itemNo1, UINT_MAX);
		_super::SetItemState(itemNo1, _super::GetItemState(itemNo2, UINT_MAX), UINT_MAX);
		_super::SetItemState(itemNo2, state, UINT_MAX);
		// 各カラム情報も入れ替え
		loop ( subItemNo, GetHeaderCtrl()->GetItemCount() )
		{
			int sin = ToInt(subItemNo);
			CString s = GetItemText(itemNo1, sin);
			SetItemText(itemNo1, sin, GetItemText(itemNo2, sin));
			SetItemText(itemNo2, sin, s);
		}
		m_ClearIndexCache();
	}

	CImageList* GetImageList(int nImageList) const;
	CImageList* SetImageList(CImageList* pImageList, int nImageListType);
	BOOL GetItem(LVITEM* pItem) const;
	BOOL SetItem(const LVITEM* pItem);
	BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
		int nImage, UINT nState, UINT nStateMask, LPARAM lParam);
	BOOL GetColumn(int nCol, LVCOLUMN* pColumn) const;
	BOOL SetColumn(int nCol, const LVCOLUMN* pColumn);
	BOOL GetOrigin(LPPOINT lpPoint) const;
	BOOL SetItemState(int nItem, LVITEM* pItem);
	BOOL SetBkImage(HBITMAP hbm, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
	BOOL SetBkImage(LPTSTR pszUrl, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
	BOOL SetBkImage(LVBKIMAGE* plvbkImage);
	int InsertItem(const LVITEM* pItem);
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	BOOL Update(int nItem);
	BOOL SortItems(PFNLVCOMPARE pfnCompare, DWORD dwData);
	int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState,
		UINT nStateMask, int nImage, LPARAM lParam);
};



}; // MFC
}; // TNB
