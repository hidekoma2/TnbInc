#pragma once
/**
 *@file
 * ヘッダ固定ListCtrl関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcDrawingListCtrl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ヘッダロックリストコントロール.
 *		ヘッダのカラム幅の変更を禁止したリストコントロールです。
 *
 *	@par使い方
 *		リソースエディタでリストコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListCtrl）を本クラスに置き換えて使用します。\n
 *		リストコントロールのプロパティは、 レポートにしておいてください。
 *		（本クラスはレポート形式のみサポートしています）	
 */
template<typename TYP = CListCtrl>
class CHeaderlockListCtrlT : public TYP
{
	DEFSUPER(TYP);
public:

	/** 
	 * [挿入] カラム追加.
	 *	@note CListCtrl::InsertColumn() と同じです。
	 *	@param nCol カラム
	 *	@param lpszColumnHeading カラム名
	 *	@param nFormat 表示フォーマット.
	 *	@param nWidth 幅
	 *	@param nSubItem サブアイテム
	 *	@return 挿入位置
	 */
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1)
	{
		int r = _super::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
		if ( r == 0 )
		{
			CHeaderCtrl* pHeaderCtrl = _super::GetHeaderCtrl();
			if ( pHeaderCtrl != NULL )
			{
				HDITEM hi;
				hi.mask = HDI_FORMAT;
				if ( pHeaderCtrl->GetItem(nCol, &hi) )
				{
					hi.fmt = (hi.fmt & ~LVCFMT_JUSTIFYMASK) | nFormat;
					hi.mask = HDI_FORMAT;
					pHeaderCtrl->SetItem(nCol, &hi);
				}
			}
		}
		return r;
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
	 * [設定] 幅自動調整モード設定.
	 *	@note デフォルトでは true です。
	 *	@param b true すると、幅をクライアントと同じになるように調整します(横スクロールバーが出ない)。\n
	 */
	void SetAdjustWidthMode(bool b = true)
	{
		m_headerCtrl.SetAdjustWidthMode(b);
	}

	/**
	 * [設定] 幅変更モード設定.
	 *	@note デフォルトで ture です。
	 *	@param b true を指定すると、マウスにより各カラムの幅を調整できます。 false なら、それを禁止します。
	 */
	void EnableChangeWidth(bool b = true)
	{
		m_headerCtrl.EnableChangeWidth(b);
	}

	/**
	 * [取得] ヘッダコントロール参照.
	 *	@retval NULL ヘッダはありません。
	 *	@retval NULL以外 ヘッダコントロールのポインタ。
	 */
	CDrawingHeaderCtrl* GetHeaderCtrl(void)
	{
		return &m_headerCtrl;
	}

protected:
	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		ASSERT0( (_super::GetStyle() & LVS_TYPEMASK) == LVS_REPORT, "CMyListCtrl", "サポートできるのは Reportタイプのみです。" );
		CHeaderCtrl* pHeaderCtrl = _super::GetHeaderCtrl();
		if ( pHeaderCtrl != NULL )
		{
			m_headerCtrl.SubclassWindow(*pHeaderCtrl);
			m_headerCtrl.EnableChangeWidth(true);
		}
		_super::PreSubclassWindow();
	}

private:
	CDrawingHeaderCtrl m_headerCtrl;	///< ヘッダーコントロール

};



/**@ingroup MFCCONTROL
 * ヘッダロックリストコントロール.
 *		ヘッダのカラム幅の変更を禁止したリストコントロールです。
 *
 *	@par使い方
 *		リソースエディタでリストコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CListCtrl）を本クラスに置き換えて使用します。\n
 *		リストコントロールのプロパティは、 レポートにしておいてください。
 *		（本クラスはレポート形式のみサポートしています）	
 */
typedef CHeaderlockListCtrlT<> CHeaderlockListCtrl;



}; //MFC
}; //TNB



