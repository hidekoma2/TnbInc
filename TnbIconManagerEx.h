#pragma once
/**
 *@file
 * ビットマップアイコン管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapImage.h"
#include "TnbIconManager.h"



//TNB Library
namespace TNB
{



/**@ingroup ICON BITMAP
 * 拡張アイコン管理クラス
 *
 *		本クラスを使用してアイコンをロードすると、開放すべきアイコンの開放漏れを防ぎます。
 *		また、リソースIDとハンドルを関連付けて記憶しておくため、キャッシュ的な使い方が可能です。
 *
 *		カーソルもアイコンと同様に本クラスで管理することが可能です。
 *
 *		本クラスは、ビットマップからアイコン作成（ CreateIcon() ）、カーソル作成（ CreateCursor() ）が増えている以外、 
 *		CIconManager とまったく同じです。
 *
 * @note	本クラスのインスタンスで取得した アイコンハンドルは、使用後、 APIの DestroyIcon() を使用せず、
 *			Destroy() を使用し破棄します(インスタンスを削除してもハンドルは破棄されます) 。
 *
 *	@par必要ファイル
 *			TnbIconManagerEx.h
 *
 *	@date 07/10/30 新規作成
 *	@date 07/11/16 アイコンに使ったHBITMAPを保持する必要なし
 */
class CIconManagerEx : public CIconManager
{
	DEFSUPER(CIconManager);
	/// 作成
	HICON m_Create(ICONINFO& _ii)
	{
		HICON hIcon = ::CreateIconIndirect(&_ii);
		if ( hIcon != NULL )
		{
			_super::Attach(hIcon);
		}
		return hIcon;
	}
	/// 作成
	HICON m_Create(ICONINFO& _ii, CBitmapHandle bmp, COLORREF maskColor, BYTE alpha)
	{
		CBitmapImage bi;
		bi.Set(bmp);
		CBitmapHandle bmpMask = bi.CreateMaskBitmap(false, maskColor);
		HRGN rgn = bi.CreateRgn(maskColor);
		if ( rgn == NULL )
		{
			return NULL;
		}
		RECT rect = { 0, 0, bi.GetSize().cx, bi.GetSize().cy };
		HDC dc = bi.GetDC();
		::ExtSelectClipRgn(dc, rgn, RGN_DIFF);
		::SetBkColor(dc, RGB(0, 0, 0)); // XOR されるから 0にしておく
		::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		bi.ReleaseDC();
		if ( alpha != 0xFF )
		{
			bi.CustomizeAlpha(alpha, RGB(0, 0, 0), 0);
		}
		_ii.hbmColor = bi.GetBitmapHandle();
		_ii.hbmMask = bmpMask;
		_DeleteObject(rgn);
		return m_Create(_ii);
	}
public:

	/// コンストラクタ
	CIconManagerEx(void) : _super()
	{
	}

	/// デストラクタ
	virtual ~CIconManagerEx(void)
	{
	}

	/**
	 * [作成] ビットマップから作成.
	 *		指定のビットマップからアイコンを作成します。
	 *	@note ビットマップからアイコンを作るには、 CBitmapImage が使用できます。
	 *	@param bmpColor	XOR ビットマスク（画像）用のビットマップを指定します。
	 *	@param bmpMask	AND ビットマスク（透過）用のビットマップを指定します。
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー
	 */
	HICON CreateIcon(CBitmapHandle bmpColor, CBitmapHandle bmpMask)
	{
		ICONINFO ii = { true, 0, 0, bmpMask, bmpColor };
		ASSERTLIB( ii.fIcon && ii.yHotspot == 0 && ii.hbmMask == bmpMask.operator->() );
		return m_Create(ii);
	}

	/**
	 * [作成] ビットマップから作成.
	 *		指定のビットマップからアイコンを作成します。
	 *	@param bmp ビットマップ 。
	 *	@param maskColor マスク色を指定します。 
	 *						省略すると、BITMAPの左下の色をマスク色とします。\n
	 *						CLR_INVALID を指定すると透過色なしでアイコンを作成します。
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー
	 */
	HICON CreateIcon(CBitmapHandle bmp, COLORREF maskColor = CLR_AUTOSELECT)
	{
		ICONINFO ii = { true, 0, 0 };
		ASSERTLIB( ii.fIcon && ii.yHotspot == 0 );
		return m_Create(ii, bmp, maskColor, 0xFF);
	}

	/**
	 * [作成] ビットマップから作成.
	 *		指定のビットマップからカーソルを作成します。
	 *	@param xHotspot	カーソルのホットスポットの X 座標を指定します。
	 *	@param yHotspot	カーソルのホットスポットの Y 座標を指定します。
	 *	@param bmpColor	XOR ビットマスク（画像）用のビットマップ ハンドルを指定します。
	 *	@param bmpMask	AND ビットマスク（透過）用のビットマップ ハンドルを指定します。
	 *	@retval NULL以外 カーソルハンドル（アイコンハンドル）。 APIの DestroyIcon() DestroyCursor() は使用しないでください。
	 *	@retval NULL エラー
	 */
	HCURSOR CreateCursor(DWORD xHotspot, DWORD yHotspot, CBitmapHandle bmpColor, CBitmapHandle bmpMask)
	{
		ICONINFO ii = { false, xHotspot, yHotspot, bmpMask, bmpColor };
		ASSERTLIB( ! ii.fIcon && ii.yHotspot == yHotspot && ii.hbmMask == bmpMask.operator->() );
		return m_Create(ii);
	}

	/**
	 * [作成] ビットマップから作成.
	 *		指定のビットマップからカーソルを作成します。
	 *	@param xHotspot	カーソルのホットスポットの X 座標を指定します。
	 *	@param yHotspot	カーソルのホットスポットの Y 座標を指定します。
	 *	@param bmp ビットマップ ハンドルを指定します。
	 *	@param parsent カーソル表示時の濃さ(単位;%)。 省略なら指定のビットマップのアルファチャネルがそのまま適応されます。
	 *				0 を指定すると、半透明しなくなります。
	 *	@param maskColor マスク色を指定します。 
	 *						省略すると、BITMAPの左下の色をマスク色とします。\n
	 *						CLR_INVALID を指定すると透過色なしでアイコンを作成します。
	 *	@retval NULL以外 カーソルハンドル（アイコンハンドル）。 APIの DestroyIcon() DestroyCursor() は使用しないでください。
	 *	@retval NULL エラー
	 */
	HCURSOR CreateCursor(DWORD xHotspot, DWORD yHotspot, CBitmapHandle bmp, int parsent = 100, COLORREF maskColor = CLR_AUTOSELECT)
	{
		ICONINFO ii = { false, xHotspot, yHotspot };
		ASSERTLIB( ! ii.fIcon && ii.yHotspot == yHotspot );
		return m_Create(ii, bmp, maskColor, static_cast<BYTE>(parsent * 255 / 100));
	}
};



};///TNB

