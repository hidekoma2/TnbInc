#pragma once
/**
 *@file
 * フォントハンドル関係のヘッダ
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		一つの HFONT をハンドルします。 auto_ptr クラスのようなものです。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// HFONTハンドル破棄クラス
struct TPhDeleteFontHandle
{
	void operator()(HFONT h) 
	{
	  #ifdef _DEBUG
		if ( ::GetObjectType(h) == NULL )
		{
			ASSERT0( false, "~CFontHandle", "先に HFONTが破棄されてしまっています。" );
		}
		else if ( ! ::DeleteObject(h) )
		{
			ASSERT1( false, "~CFontHandle", "HFONT の破棄に失敗しました。\ncode = %d", ::GetLastError() );
		}
	  #else
		::DeleteObject(h);
	  #endif
	}
};

#endif // _TnbDOXYGEN



/**@ingroup WINAPI
 * HFONT型ハンドルハンドル
 *
 *		HFONT 、一つを管理します。参照する本インスタンスがなくなると、自動的に DeleteObject() します。
 *
 *	@par注意
 *		一度本クラスで管理したら、 HFONT として取り出したものを一時的に以外には使用しないでください。
 *		特に HFONT を介しての複製は問題が発生します。本クラスの管理下からはずす場合、 Detach() を使用します。
 *		\code
 *		CFontHandle fh;
 *		fh.Set(90, "tahoma");
 *		HFONT h1 = fh;				// fh で管理している HFONT を参照。
 *		{
 *			CFontHandle temp = h1;	// ハンドルを記憶
 *		}							// デストラクタ時に h1 のハンドルを破棄。		
 *		HFONT h2 = fh;				// bh で管理している HFONT を参照。
 *									// しかし temp のデストラクタ時に 破棄されているので、
 *									// ここでは無効なハンドルが h2 に代入される。
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbFontHandle.h
 *
 *	@date 10/05/13	新規作成。
 *	@date 11/08/16	キャラセットを DEFAULT_CHARSET から SHIFTJIS_CHARSET へ（ただし SJISプロジェクトの場合）
 *	@date 13/02/13	NONCLIENTMETRICS 構造体が WINVER 0x600 から大きくなっている問題に対処
 *	@date 14/03/28	SetToWindow() を新規追加。
 */
class CFontHandle : public CPointerHandleBaseT<HFONT, TPhDeleteFontHandle, NULL> 
{
	typedef CPointerHandleBaseT<HFONT, TPhDeleteFontHandle, NULL> _super;	///< 親クラス名再宣言
	DEFSUPER(_super);
public:

	/**
	 * コンストラクタ.
	 *		NULL状態のビットマップ（HFONT を管理していない)オブジェクトが作成されます。
	 */
	CFontHandle(void) : _super() 
	{
	}

	/**
	 * コピーコンストラクタ.
	 *		コピー元に有効な HFONTがある場合、本インスタンスでも、同じ HFONT を参照するようになります。
	 *	@param other コピー元
	 */
	CFontHandle(const CFontHandle& other) : _super(other) 
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		指定のフォントハンドルを保持するオブジェクトが作成されます。 
	 *		指定したハンドルは本インスタンスが管理します。破棄しないでください。
	 *	@param hFont フォントハンドル。
	 */
	CFontHandle(HFONT hFont) : _super(hFont) 
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		指定の属性からフォントを作成し、それを本インスタンスで管理します。
	 *	@param attr フォント属性.
	 */
	CFontHandle(const LOGFONT& attr)
	{
		HFONT f = ::CreateFontIndirect(&attr);
		_super::operator=(f);
	}

	/**
	 * [設定] フォント設定.
	 *		指定の属性からフォントを作成し、それを本インスタンスで管理します。
	 *	@param attr フォント属性.
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Set(const LOGFONT& attr)
	{
		HFONT f = ::CreateFontIndirect(&attr);
		_super::operator=(f);
		return ! _super::IsNull();
	}

	/**
	 * [設定] フォント設定.
	 *		指定の属性からフォントを作成し、それを本インスタンスで管理します。
	 *	@note 高さ（attr.lfHeoght）は 1/10 ポイント単位で解釈されます。
	 *	@param attr フォント属性.
	 *	@param dc 高さを論理単位に変換するために使用するデバイスコンテキスト。 NULL を指定すると画面デバイスを使います。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Set(const LOGFONT& attr, HDC dc)
	{
		HDC hDC = dc;
		if ( hDC == NULL )
		{
			hDC = ::GetDC(NULL);
		}
		LOGFONT lf = attr;
		POINT pt;
		pt.y = ::GetDeviceCaps(hDC, LOGPIXELSY) * lf.lfHeight;
		pt.y /= 720;    // 72 points/inch, 10 decipoints/point
		::DPtoLP(hDC, &pt, 1);
		POINT ptOrg = { 0, 0 };
		::DPtoLP(hDC, &ptOrg, 1);
		lf.lfHeight = -abs(pt.y - ptOrg.y);
		if ( dc == NULL )
		{
			::ReleaseDC(NULL, hDC);
		}
		return Set(lf);
	}

	/**
	 * [設定] フォント設定.
	 *		指定のサイズとフォント名からフォントを作成し、それを本インスタンスで管理します。
	 *	@param pointSize フォントサイズ（1/10 ポイント単位）.
	 *	@param lpszFaceName フォント名。
	 *	@param dc 高さを論理単位に変換するために使用するデバイスコンテキスト。 NULL を指定すると画面デバイスを使います。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Set(int pointSize, LPCTSTR lpszFaceName, HDC dc = NULL)
	{
		LOGFONT attr = { 0 };
		attr.lfCharSet = DEFAULT_CHARSET;
		#ifndef _UNICODE
			attr.lfCharSet = SHIFTJIS_CHARSET;
		#endif
		attr.lfHeight = pointSize;
		lstrcpyn(attr.lfFaceName, lpszFaceName, countof(attr.lfFaceName));
		return Set(attr, dc);
	}

	/**
	 * [設定] フォント設定.
	 *		指定のウィンドウに設定されているフォントを読み出し、複製を本インスタンスで管理します。
	 *	@param hWnd ウィンドウハンドル。
	 *	@param mul 倍率
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Set(HWND hWnd, double mul = 1.0)
	{
		HFONT h = reinterpret_cast<HFONT>(::SendMessage(hWnd, WM_GETFONT, 0, 0));
		if ( h == NULL )
		{
			h = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
		}
		return SetClone(h, mul);
	}

	/**
	 * [設定] フォント設定.
	 *		指定のフォントを太文字にしたフォントを作成し、本インスタンスで管理します。
	 *	@param hFont フォントハンドル
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetAsBold(HFONT hFont)
	{
		Null();
		LOGFONT attr;
		if ( GetAttributes(attr, hFont) )
		{
			attr.lfWeight = FW_BOLD;
			return Set(attr);
		}
		return false;
	}

	/**
	 * [設定] フォント設定.
	 *		指定のフォントハンドルを複製したものを本オブジェクトで管理します。
	 *	@param hFont フォント。
	 *	@param mul 倍率
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetClone(HFONT hFont, double mul = 1.0)
	{
		Null();
		LOGFONT attr;
		if ( GetAttributes(attr, hFont) )
		{
			attr.lfHeight = static_cast<long>(attr.lfHeight * mul);
			attr.lfWidth = static_cast<long>(attr.lfWidth * mul); 
			return Set(attr);
		}
		return false;
	}

	#ifndef _WIN32_WCE

	/**
	 * [設定] キャプションバーフォント設定.
	 *		キャプションバーと同じフォントを作成し、本インスタンスで管理します。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetSystemCaptionFont(void)
	{
		NONCLIENTMETRICS ncm;
		VERIFY( ms_GetNonClientMetrics(ncm) );
		#ifndef _UNICODE
			ncm.lfCaptionFont.lfCharSet = SHIFTJIS_CHARSET;
		#endif
		return Set(ncm.lfCaptionFont);  //タイトルバーのLOGFONT
	}

	/**
	 * [設定] メニューフォント設定.
	 *		メニューと同じフォントを作成し、本インスタンスで管理します。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetSystemMenuFont(void)
	{
		NONCLIENTMETRICS ncm;
		VERIFY( ms_GetNonClientMetrics(ncm) );
		#ifndef _UNICODE
			ncm.lfMenuFont.lfCharSet = SHIFTJIS_CHARSET;
		#endif
		return Set(ncm.lfMenuFont);  //メニューのLOGFONT
	}

	#endif //_WIN32_WCE

	/**
	 * [設定] デタッチ.
	 *		本インスタンスが破棄してもハンドルを破棄しないように取り出します。
	 *		本インスタンスは NULL状態になります。
	 *	@retval NULL ハンドルを持っていない。
	 *	@retval NULL以外 HFONT。 使用後は、明示的に ::DeleteObject() してください。
	 */
	HFONT Detach(void)
	{
		LOGFONT attr;
		if ( GetAttributes(attr) )
		{
			HFONT f = ::CreateFontIndirect(&attr);
			Null();
			return f;
		}
		return NULL;
	}

	/**
	 * [取得] 属性取得.
	 *		本インスタンスが管理しているフォントの属性を返します。
	 *	@param[out] _attr 取得した属性が格納されます。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool GetAttributes(LOGFONT& _attr) const
	{
		HFONT f = *this;
		return GetAttributes(_attr, f);
	}

	/**
	 * [取得] ウィンドウへ設定.
	 *		本インスタンスが管理しているフォントをウィンドウに設定します。
	 *	@param hWnd ウィンドウハンドル
	 *	@param isRedraw true ならすぐに再描画
	 */
	void SetToWindow(HWND hWnd, bool isRedraw = true) const
	{
		ASSERT(::IsWindow(hWnd));
		HFONT f = *this;
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)f, isRedraw);
	}

	/**
	 * [取得] 属性取得.
	 *		指定のフォントの属性を返します。
	 *	@param[out] _attr 取得した属性が格納されます。
	 *	@param[in] hFont フォント。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	static bool GetAttributes(LOGFONT& _attr, HFONT hFont)
	{
		return ::GetObject(hFont, sizeof(LOGFONT), &_attr) != 0;
	}

private:

	#ifndef _WIN32_WCE

	/// SystemParametersInfo 実行
	static  bool ms_GetNonClientMetrics(NONCLIENTMETRICS& _ncm)
	{
		_ncm.cbSize = sizeof(NONCLIENTMETRICS);
		bool r = !! ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, _ncm.cbSize, &_ncm, 0);
		#if(WINVER >= 0x0600)
			if ( ! r )
			{
				_ncm.cbSize -= sizeof(int);
				r = !! ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, _ncm.cbSize, &_ncm, 0);
			}
		#endif
		return r;
	}

	#endif
};



};//TNB
