#pragma once
/**
 *@file
 * 立体描画情報ボタン関係のヘッダ
 *
 *		描画情報の形状にあわせたグラフィカルなボタンを提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapDrawer.h"
#include "TnbMfcRgnButton.h"
#include "TnbSolidBitmapMaker.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL BITMAP
 * 立体描画情報ボタンコントロール
 *
 *		本クラスに描画情報を設定( SetAllDrawers() )すると、
 *		「通常状態」「無効状態」「押下状態」「ホバー状態」「フォーカス状態」の状態を作成し、
 *		使用します。その際、ボタンらしく、立体（でっばり or へっこみ）化します
 *		（そのため、元描画情報から縦横、それぞれ +3 dot 多きボタンになります）。
 *
 *		- [通常状態]\n	描画情報を少し暗くした画像を使用します。
 *		- [無効状態]\n	描画情報をグレー化した画像を使用します。
 *		- [ホバー状態]
 *		- [押下状態]\n	描画情報、そのまま使用します。
 *		- [フォーカス状態]\n	ホバー状態と同じ画像を使用します。
 *
 *	@note 有効、無効、表示、非表示は、 MFC の CButton と同じように操作できます。
 *
 *	@par使い方
 *		リソースエディタでボタンかスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CButton or CStatic）を本クラスに置き換えて使用します。
 *		\code
 *			[.h ファイル]
 *
 *			class CFooDlg : public CDialog {
 *					;
 *				CSolidDrawingButton	m_buttonExit; // CButton から変更。
 *
 *			[.cpp ファイル]
 *
 *			BOOL CFooDlg::OnInitDialog() {
 *					;
 *				CFont f;
 *				f.CreatePointFont(220, "Smudger LET");	// フォント作成	
 *				CRect rc;
 *				m_buttonCancel.GetWindowRect(rc);
 *				CTextDrawer td (rc.Size(), f, RGB(0, 1, 170), "EXIT");	//コントロールの大きさを使用して描画情報作成
 *				//	//CTextDrawer td (f, RGB(0, 1, 170), "EXIT");	←フォントの大きさから描画情報作成もできる)
 *				//	//td.AdjustSize(); 
 *				m_buttonExit.SetAllDrawers(td, RGB(255, 0, 255)); // 設定
 *					;
 *
 *		\endcode	
 *
 *	@see CAbstractButton
 *
 *	@par必要ファイル
 *			TnbMfcSolidDrawingButton.h
 * 
 *	@date 09/11/12 新規作成
 */
class CSolidDrawingButton : public CRgnButton
{
	DEFSUPER(CRgnButton);
public:

	/**
	 * [設定] ビットマップ設定.
	 *		一つの描画情報を指定すると、そこから、「通常状態」「ホバー状態」「押下状態」[無効状態」「フォーカス状態」の
	 *		ボタンを作成します。
	 *	@note ダイアログ表示後、イベント発生時に、本メソッドを使ってビットマップを再設定することも可能です。
	 *	@param draw 描画情報。本メソッド内で、この描画情報から画像を作成しますので、描画情報は破棄してもかまいません。
	 *	@param transColor 透過色指定.
	 *	@param persent 「通常状態」の色落ち割合。０なら「フォーカス状態」と同じ明るさになります。100なら、真っ黒になってしまいます。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetAllDrawers(const IDrawable& draw, COLORREF transColor, int persent = 30)
	{
		CBitmapImage base = CBitmapDrawer::ToBitmap(draw, transColor);
		CSize sz = base.GetSize();
		//
		HRGN hRgn = base.CreateRgn(transColor);
		CSolidBitmapMaker sbm;
		sbm.SetTransColor(transColor);
		sbm.SetBaseColor(transColor);
		//
		CBitmapImage normalBmp;					// ノーマル
		{
			CBitmapImage mask;
			mask.Set(sz.cx, sz.cy, RGB(0, 0, 0));
			normalBmp.Set(base);
			HDC hDC = normalBmp.GetDC();
			::SelectClipRgn(hDC, hRgn);
			mask.AlphaBlend(hDC, 0, 0, persent);
			normalBmp.ReleaseDC();
			normalBmp = sbm.Make(normalBmp);
		}
		CBitmapImage hoverBmp = sbm.Make(base);	// ホバー＆フォーカス
		CBitmapImage pressBmp = sbm.Make(base, true);// プレス
		CBitmapImage disableBmp;					// 無効
		{
			CBitmapImage b;
			b.Set(base);
			b.ChangePixelColor(RGB(85,85,85), RGB(85,85,85), RGB(85,85,85));
			disableBmp.Set(base);
			HDC hDC = disableBmp.GetDC();
			::SelectClipRgn(hDC, hRgn);
			b.BitBlt(hDC, 0, 0);
			disableBmp.ReleaseDC();
			disableBmp = sbm.Make(disableBmp);
		}
		_DeleteObject(hRgn);
		//
		sz = hoverBmp.GetSize();
		CBitmapImage bi;
		bi.Set(sz.cx * 5, sz.cy);
		bi.Insert(sz.cx * 0, 0, normalBmp);
		bi.Insert(sz.cx * 1, 0, hoverBmp);
		bi.Insert(sz.cx * 2, 0, pressBmp);
		bi.Insert(sz.cx * 3, 0, disableBmp);
		bi.Insert(sz.cx * 4, 0, hoverBmp);
		return _super::SetAllBitmaps(bi, transColor);
	}

private:
	/// [設定] ベースビットマップ指定.
	bool SetBitmap(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT);
	/// [設定] 押下状態ビットマップ指定.
	void SetPushedBitmap(CBitmapHandle bmp);
	/// [設定] 無効状態ビットマップ指定.
	void SetDisableBitmap(CBitmapHandle bmp);
	/// [設定] ホバー状態ビットマップ指定.
	void SetHoverBitmap(CBitmapHandle bmp);
	/// [設定] ビットマップ指定.
	bool SetAllBitmaps(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT);
};



}; //MFC
}; //TNB



