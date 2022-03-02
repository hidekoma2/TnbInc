#pragma once
/**
 *@file
 * カスタム描画抽象コントロール関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * カスタム描画抽象コントロール
 *
 *	@par必要ファイル
 *			TnbMfcAbstractCustomDraw.h
 * 
 *	@date 08/09/11	新規作成
 */
class CAbstractCustomDraw
{
protected:

	/// 表示用パラメータ
	struct TDrawParam
	{
		HDC			dc;			///< デバイスコンテキスト
		CRect		rect;		///< 描画範囲
		DWORD_PTR	itemNo;		///< アイテムナンバー
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
	 * [確認] サブアイテム概念の有無
	 *	@retval ture サブアイテムあり。
	 *	@retval false ない。
	 */
	virtual bool HasSubItem(void) const = 0;

	/**
	 * [確認] アイテムRECT計算
	 *	@note _pa.itemNo, _pa.subItemNo などから、 _pa.rect の設定を行ってください。
	 *	@note _pa.state の調整も可能です。
	 *	@param[in,out] _pa 描画情報
	 *	@param[in] isSub true なら subアイテム用の計算をする。
	 */
	virtual void OnCalcItemRect(TDrawParam& _pa, bool isSub) = 0;

	/**
	 * [通知] for notifications from parent
	 *	@note 宛先(hwndFrom)を確認の上サブクラスの OnChildNotify メソッドからコールしてください。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE	処理済。サブクラスの OnCildNotify クラスはそのまま TRUE でリターンしてください。
	 *	@retval FALSE	未処理。
	 */
	BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_NOTIFY )
		{
			LPNMHDR lpNmHdr = reinterpret_cast<LPNMHDR>(lParam);
			if ( lpNmHdr->code == NM_CUSTOMDRAW ) 
			{
				LPNMLVCUSTOMDRAW lpLvCd = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
				*_pResult = m_OnCustomDraw(lpLvCd);
				return TRUE;
			}
		} 
		return FALSE;
	}

private:

	/// Item描画サブ
	LRESULT m_OnItemPaintSub(LPNMLVCUSTOMDRAW P, int subItemNo)
	{
		TDrawParam pa;
		pa.dc			= P->nmcd.hdc;
		pa.itemNo		= P->nmcd.dwItemSpec;
		pa.subItemNo	= subItemNo;
		pa.state		= P->nmcd.uItemState;
		pa.textColor	= P->clrText;
		pa.backColor	= P->clrTextBk;
		pa.rect			= P->nmcd.rc;
		OnCalcItemRect(pa, true);
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
			OnBeginItemPaint(P->nmcd.hdc);
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_POSTPAINT:
			OnEndItemPaint(P->nmcd.hdc) ;
			return CDRF_DODEFAULT;
		case CDDS_ITEMPOSTPAINT:
			{
				TDrawParam pa;
				pa.dc			= P->nmcd.hdc;
				pa.itemNo		= P->nmcd.dwItemSpec;
				pa.subItemNo	= 0;
				pa.state		= P->nmcd.uItemState;
				pa.textColor	= P->clrText;
				pa.backColor	= P->clrTextBk;
				pa.rect			= P->nmcd.rc;
				OnCalcItemRect(pa, false);
				OnItemPainted(&pa);
			}
			break;
		case CDDS_ITEMPREPAINT:
			if ( ! HasSubItem() )
			{
				return m_OnItemPaintSub(P, 0) | CDRF_NOTIFYPOSTPAINT;
			}
			return CDRF_NOTIFYSUBITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
			return m_OnItemPaintSub(P, P->iSubItem);
		}
		return CDRF_DODEFAULT;
	}

public:

	/// コンストラクタ
	CAbstractCustomDraw(void)
	{
	}
};



}; // MFC
}; // TNB
