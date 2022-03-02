#pragma once
/**
 *@file
 * パーツ描画情報のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #define DFCS_FOCUS			0x10000
#endif



/**@ingroup WINAPI
 * WINDOWパーツ描画インターフェース
 *
 *	ウィンドウのタイトルバー、フレームコントロール、フォーカス枠を書くインターフェースを持ちます。
 *
 *	@par必要ファイル
 *			TnbPartsDrawable.h
 * 
 *	@date 07/10/24 新規作成
 */
struct IPartsDrawable
{
	/// デストラクタ
	virtual ~IPartsDrawable(void) { }

	/**
	 * [描画] 長方形の辺を描画.
	 *	@param hdc デバイスコンテキストのハンドルを指定します。 
	 *	@param lprc 長方形の論理座標を保持する 1 個の RECT 構造体へのポインタを指定します。
	 *	@param edge 辺の内側と外側の描画方法を指定します。内側の境界フラグから 1 つを、外側の境界フラグからもう 1 つを選び、組み合わせて指定します。
	 *	@param grfFlags 境界の種類を指定します。
	 *	@retval true 成功。
	 *	@retval false 失敗。 ::GetLastError() でエラー情報を取得できます。
	 */
	virtual bool DrawEdge(HDC hdc, LPCRECT lprc, UINT edge, UINT grfFlags) const = 0;

	/**
	 * [描画] ボタンコントロール描画.
	 *		指定されたタイプとスタイルを備える、ボタンコントロールを描画します。
	 *	@param[in] hdc		コントロールの描画に使いたいデバイスコンテキストのハンドルを指定します。 
	 *						NULL を指定すると、実際に描画する大きさを lprc に返します。
	 *	@param[in,out] _lprc	長方形の論理座標を保持する 1 個の RECT 構造体へのポインタを指定します。 
	 *	@param[in] uState	ボタンコントロールの状態を指定します。\n
	 *			以下の値を指定します。
	 *			<table><tr><td>
	 *			値									</td><td>
	 *					意味						</td></tr><tr><td>
	 *			DFCS_BUTTON3STATE						</td><td>
	 *					3 ステートボタン（オン、グレイ表示、オフ）  </td></tr><tr><td>
	 *			DFCS_BUTTONCHECK						</td><td>
	 *					チェックボックス </td></tr><tr><td>
	 *			DFCS_BUTTONPUSH						</td><td>
	 *					プッシュボタン </td></tr><tr><td>
	 *			DFCS_BUTTONRADIO						</td><td>
	 *					オプションボタン </td></tr><tr><td>
	 *			DFCS_BUTTONRADIOIMAGE						</td><td>
	 *					オプションボタンのイメージ（正方形とは異なるイメージ） </td></tr><tr><td>
	 *			DFCS_BUTTONRADIOMASK						</td><td>
	 *					オプションボタンのマスク（正方形とは異なるマスク） </td></tr>
	 *			</table>
	 *			さらに、コントロールの状態を設定するために、次の値の 1 つまたは複数を指定することもできます。 		
	 *			<table><tr><td>
	 *			値									</td><td>
	 *					意味						</td></tr><tr><td>
	 *			DFCS_CHECKED						</td><td>
	 *					チェックされているボタンを描画します。 </td></tr><tr><td>
	 *			DFCS_FLAT							</td><td>
	 *					（凸凹の表現のない）平らなデザインのボタンを描画します。</td></tr><tr><td>
	 *			DFCS_HOT							</td><td>
	 *					ボタンをホットトラッキング表示します（マウスやキーボードの操作に応じて、ボタンを変化させます）。 </td></tr><tr><td>
	 *			DFCS_INACTIVE						</td><td>
	 *					無効な（淡色表示の）ボタンを描画します。 </td></tr><tr><td>
	 *			DFCS_MONO							</td><td>
	 *					モノクロームの境界を持つボタンを描画します。 </td></tr><tr><td>
	 *			DFCS_PUSHED							</td><td>
	 *					押された状態のボタンを描画します。 </td></tr><tr><td>
	 *			DFCS_FOCUS							</td><td>
	 *					フォーカスのある状態のボタンを描画します（※WIN32APIではないシンボルです）。</td></tr><tr><td>
	 *			DFCS_TRANSPARENT					</td><td>
	 *					背景をそのままにします（透過表現）。 </td></tr>
	 *			</table>
	 *	@retval true 成功。
	 *	@retval false 失敗。 ::GetLastError() でエラー情報を取得できます。
	 */
	virtual bool DrawButtonControl(HDC hdc, LPRECT _lprc, UINT uState) const = 0;
	
	/**
	 * [描画] フォーカス枠描画.
	 *		フォーカスが存在することをユーザーに示すために使われる長方形の枠を描画します。
	 *	@param hdc		デバイスコンテキストのハンドルを指定します。 
	 *	@param lpRect		長方形の論理座標を保持する 1 個の RECT 構造体へのポインタを指定します。 
	 *	@retval true 成功。
	 *	@retval false 失敗。 ::GetLastError() でエラー情報を取得できます。
	 */
	virtual bool DrawFocusRect(HDC hdc, LPCRECT lpRect) const = 0;

	/**
	 * [取得] 押下状態のずれ.
	 *		押下した時に、コントロールの上に表示する内容をずらす方向を得ることが出来ます。
	 *	@param[out] _po ずれ
	 */
	virtual void GetPushedOffset(POINT& _po) const = 0;

	/**
	 * [取得] エッジの幅、高さ.
	 *		エッジの幅、高さを返します。
	 *	@param[out] _po 幅、高さ
	 */
	virtual void GetEdgeOffset(POINT& _po) const = 0;
};



/**@ingroup WINAPI
 * 標準WINDOWパーツ描画クラス
 *
 *	WINAPI の ::DrawFrameControl(), ::DrawFocusRect() をラップしています。
 *
 *	@par必要ファイル
 *			TnbPartsDrawable.h
 * 
 *	@date 07/11/23 新規作成
 */
class CStandardPartsDrawer : public IPartsDrawable 
{
	DEFSUPER(IPartsDrawable);
public:
	/**
	 * [描画] 長方形の辺を描画.
	 *	@param hdc デバイスコンテキストのハンドルを指定します。 
	 *	@param lprc 長方形の論理座標を保持する 1 個の RECT 構造体へのポインタを指定します。
	 *	@param edge 辺の内側と外側の描画方法を指定します。内側の境界フラグから 1 つを、外側の境界フラグからもう 1 つを選び、組み合わせて指定します。
	 *	@param grfFlags 境界の種類を指定します。
	 *	@retval true 成功。
	 *	@retval false 失敗。 ::GetLastError() でエラー情報を取得できます。
	 */
	virtual bool DrawEdge(HDC hdc, LPCRECT lprc, UINT edge, UINT grfFlags) const
	{
		RECT rc = *lprc;
		return !! ::DrawEdge(hdc, &rc, edge, grfFlags);
	}

	/**
	 * [描画] ボタンコントロール描画.
	 *		指定されたタイプとスタイルを備える、ボタンコントロールを描画します。
	 *	@param[in] hdc		コントロールの描画に使いたいデバイスコンテキストのハンドルを指定します。 
	 *						NULL を指定すると、実際に描画する大きさを lprc に返します。
	 *	@param[in,out] _lprc	長方形の論理座標を保持する 1 個の RECT 構造体へのポインタを指定します。 
	 *	@param[in] uState	ボタンコントロールの状態を指定します。 IPartsDrawable::DrawButtonControl() 参照。
	 *	@note DFCS_FOCUS は無視されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。 ::GetLastError() でエラー情報を取得できます。
	 */
	virtual bool DrawButtonControl(HDC hdc, LPRECT _lprc, UINT uState) const
	{
		switch ( uState & 0xFF )
		{
		case DFCS_BUTTON3STATE:
		case DFCS_BUTTONCHECK:
		case DFCS_BUTTONRADIO:
			_lprc->right = _lprc->left + 13;
			_lprc->bottom = _lprc->top + 13;
			break;
		}
		if ( hdc == NULL ) { return true; }
		return !! ::DrawFrameControl(hdc, _lprc, DFC_BUTTON, uState);
	}
	
	/**
	 * [描画] フォーカス枠描画.
	 *		フォーカスが存在することをユーザーに示すために使われる長方形の枠を描画します。
	 *	@param hdc		デバイスコンテキストのハンドルを指定します。 
	 *	@param lpRect		長方形の論理座標を保持する 1 個の RECT 構造体へのポインタを指定します。 
	 *	@retval true 成功。
	 *	@retval false 失敗。 ::GetLastError() でエラー情報を取得できます。
	 */
	virtual bool DrawFocusRect(HDC hdc, LPCRECT lpRect) const
	{
		return !! ::DrawFocusRect(hdc, lpRect);
	}

	/**
	 * [取得] 押下状態のずれ.
	 *		押下した時に、コントロールの上に表示する内容をずらす方向を得ることが出来ます。
	 *	@param[out] _po ずれ
	 */
	virtual void GetPushedOffset(POINT& _po) const
	{
		_po.x = 1;
		_po.y = 1;
	}

	/**
	 * [取得] エッジの幅、高さ.
	 *		エッジの幅、高さを返します。
	 *	@param[out] _po 幅、高さ
	 */
	virtual void GetEdgeOffset(POINT& _po) const
	{
		_po.x = ::GetSystemMetrics(SM_CXEDGE);
		_po.y = ::GetSystemMetrics(SM_CYEDGE);
	}
};



}; //MFC
}; //TNB



