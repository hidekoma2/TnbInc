#pragma once
/**
 *@file
 * ビットマップハンドル関係のヘッダ
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		一つの HBITMAP をハンドルします。 auto_ptr クラスのようなものです。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"
#ifndef _WIN32_WCE
 #pragma comment(lib, "gdi32.lib")
#else
 #define LR_CREATEDIBSECTION 0
#endif



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// HBITMAPハンドル破棄クラス
struct TPhDeleteBitmapHandle
{
	void operator()(HBITMAP h) 
	{
	  #ifdef _DEBUG
		if ( ::GetObjectType(h) == NULL )
		{
			ASSERT0( false, "~CBitmapHandle", "先に HBITMAPが破棄されてしまっています。" );
		}
		else if ( ! ::DeleteObject(h) )
		{
			ASSERT1( false, "~CBitmapHandle", "HBITMAP の破棄に失敗しました。\ncode = %d", ::GetLastError() );
		}
	  #else
		::DeleteObject(h);
	  #endif
	}
};

#endif // _TnbDOXYGEN



/**@ingroup BITMAP
 * HBITMAP型ハンドルハンドル
 *
 *		HBITMAP 、一つを管理します。参照する本インスタンスがなくなると、自動的に DeleteObject() します。
 *
 *	@parポイント
 *		本クラスを引数に持つ関数の場合、 HBITMAP , CBitmapHandle 以外に、
 *		BITMAPファイル名（文字列）、 BITMAP リソースID を指定することもできます。
 *		<table><tr><td><pre>
 *
 *		//= 宣言
 *		void Foo(CBitmapHandle bmp); // こういう関数の場合・・・
 *
 *		//= 使用時
 *		Foo("c:\\BMP\\BackTop1.bmp");  // BackTop1.bmpファイルを読み込んで Foo() をコールします。
 *		Foo(IDB_BACKTOP1");  // リソースの IDB_BACKTOP1 を読み込んで Foo() をコールします。
 *		</pre></td></tr>
 *
 *	@par注意
 *		一度本クラスで管理したら、 HBITMAP として取り出したものを一時的に以外には使用しないでください。
 *		特に HBITMAP を介しての複製は問題が発生します。本クラスの管理下からはずす場合、 Detach() を使用します。
 *		<table><tr><td><pre>
 *
 *		CBitmapHandle bh(IDB_PIC1);   // リソースから読込み
 *		HBITMAP h1 = bh;              // bh で管理している HBITMAP を参照。
 *		{
 *			CBitmapHandle temp = h1;  // ハンドルを記憶
 *		}                             // デストラクタ時に h1 のハンドルを破棄。		
 *		HBITMAP h2 = bh;              // bh で管理している HBITMAP を参照。
 *		                              // しかし temp のデストラクタ時に 破棄されているので、
 *		                              // ここでは無効なハンドルが h2 に代入される。
 *		</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbBitmapHandle.h
 *
 *	@date 06/09/29 テンプレート化。
 *	@date 07/11/03 継承し、メンバ追加。
 *	@date 07/11/12 内部管理を現在のDesktop設定の影響を受けないように修正。
 *	@date 07/11/13 Save() 追加。
 *	@date 07/11/15 SetClone() 追加。
 *	@date 08/04/07 LoadOem() 追加。 ファイルをLoadする時も、 INSTANCE ハンドルを指定するようにした。
 *	@date 08/07/02 Create32bitDibSection() 追加。CE対応。
 *	@date 10/09/15 SaveMemory() 追加
 */
class CBitmapHandle : public CPointerHandleBaseT<HBITMAP, TPhDeleteBitmapHandle, NULL> 
{
	typedef CPointerHandleBaseT<HBITMAP, TPhDeleteBitmapHandle, NULL> _super;	///< 親クラス名再宣言
	DEFSUPER(_super);
public:

	/**
	 * コンストラクタ.
	 *		NULL状態のビットマップ（HBITMAP を管理していない)オブジェクトが作成されます。
	 */
	CBitmapHandle(void) : _super() 
	{
	}

	/**
	 * コピーコンストラクタ.
	 *		コピー元に有効な HBITMAPがある場合、本インスタンスでも、
	 *		そこを参照するようになります。
	 *	@param other コピー元
	 */
	CBitmapHandle(const CBitmapHandle& other) : _super(other) 
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		HBITMAP を保持するオブジェクトが作成されます。
	 *	@param P HBITMAP。
	 */
	CBitmapHandle(HBITMAP P) : _super(P) 
	{
	}

	/**
	 * 代入コンストラクタ.
	 *		指定ビットマップリソースを読み込み、それをを保持するオブジェクトが作成されます。
	 *		読み込み失敗した場合、NULL状態のビットマップオブジェクトが作成されます。
	 *	@note インスタンスハンドルは、 GetInstanceHandle(EI_Bitmap) の値が使用されます。
	 *	@param uBitmapResourceId ビットマップリソースID
	 *	@param cx 読み込みXサイズを指定できます。省略すると、リソースの情報のままになります。
	 *	@param cy 読み込みYサイズを指定できます。省略すると、リソースの情報のままになります。
	 *	@param fuLoad 読込みオプションを指定できます。省略可能。\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 */
	CBitmapHandle(UINT uBitmapResourceId, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		_super::operator=(Load(uBitmapResourceId, cx, cy, fuLoad));
	}

	/**
	 * 代入コンストラクタ.
	 *		指定ファイルを読み込み、それをを保持するオブジェクトが作成されます。
	 *		ファイル読み込み失敗した場合、NULL状態のビットマップオブジェクトが作成されます。
	 *	@param lpszFile ファイル名。
	 *	@param cx 読み込みXサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param cy 読み込みYサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param fuLoad 読込みオプションを指定できます。省略可能。\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 */
	CBitmapHandle(LPCTSTR lpszFile, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		_super::operator=(Load(lpszFile, cx, cy, fuLoad));
	}

	#ifdef __AFX_H__
	/**
	 * 代入コンストラクタ.
	 *		指定ファイルを読み込み、それをを保持するオブジェクトが作成されます。
	 *		ファイル読み込み失敗した場合、NULL状態のビットマップオブジェクトが作成されます。
	 *	@param strFile ファイル名。
	 *	@param cx 読み込みXサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param cy 読み込みYサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param fuLoad 読込みオプションを指定できます。省略可能。\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 */
	CBitmapHandle(const CString& strFile, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		_super::operator=(Load(strFile.operator LPCTSTR(), cx, cy, fuLoad));
	}
	#endif

	/**
	 * [取得] ビットマップサイズ取得. 
	 *		本オブジェクトが保持しているビットマップの大きさを返します。
	 *	@param[out] _size サイズ格納。
	 *	@retval true 成功。
	 *	@retval false 失敗。保持していません。
	 */
	bool GetSize(SIZE& _size) const
	{
		BITMAP bm;
		if ( ::GetObject(*this, sizeof(BITMAP), &bm) > 0 )
		{
			_size.cx = bm.bmWidth;
			_size.cy = bm.bmHeight;
			return true;
		}
		_GetLastError("GetObject");
		_size.cx = 0;
		_size.cy = 0;
		return false;
	}

	/**
	 * [取得] デタッチ.
	 *		本インスタンスが破棄してもハンドルを破棄しないように取り出します。
	 *		本インスタンスは NULL状態になります。
	 *	@retval NULL ハンドルを持っていない。
	 *	@retval NULL以外 HBITMAP。 使用後は、明示的に ::DeleteObject() してください。
	 */
	HBITMAP Detach(void)
	{
		if ( IsNull() ) { return NULL; }
		#ifndef _WIN32_WCE
			HBITMAP h = static_cast<HBITMAP>(::CopyImage(*this, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
		#else
			RGBQUAD* pRgb;
			HBITMAP h = Create32bitDibSection(*this, pRgb);
		#endif
		Null();
		return h;
	}

	/**
	 * [設定] HBITMAPセット.
	 *		指定の HBITMAP の複製を本オブジェクトで管理します。
	 *	@note HBITMAP の破棄のタイミングを CBitmapHandle に管理させたくない時、使用します。
	 *	@param hBmp HBITMAP。
	 */
	void SetClone(HBITMAP hBmp)
	{
		#ifndef _WIN32_WCE
			_super::operator=(static_cast<HBITMAP>(::CopyImage(hBmp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION)));
		#else
			RGBQUAD* pRgb;
			_super::operator=(Create32bitDibSection(hBmp, pRgb));
		#endif
	}

	/**
	 * [処理] イメージ描画.
	 *		指定のデバイスコンテキストの指定の位置に、本インスタンスで持っているイメージを描画します。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param x 描画X位置(左が0)。省略すると０。
	 *	@param y 描画Y位置(上が0)。省略すると０。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Draw(HDC hdc, int x = 0, int y = 0) const
	{
		HBITMAP h = *this;
		return Draw(hdc, h, x, y);
	}

	/**
	 * [作成] BMPファイル作成.
	 *	@note CE では bitsPixel は無視され必ず３２になります。
	 *	@param lpszFileName ファイル名。
	 *	@param bitsPixel 一ドットのビット数。1,4,8,16,24,32 のいずれかが指定できます。 0なら ビットマップの情報ままになります。
	 *	@retval ERROR_SUCCESS 成功。
	 *	@retval 上記以外 失敗。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD Save(LPCTSTR lpszFileName, WORD bitsPixel = 0)
	{
		return Save(lpszFileName, *this, bitsPixel);
	}


	//-----------------


	/**
	 * [作成] 32bitビットマップ作成.
	 *	@note 元のビットマップの大きさ、絵を持つ DIBセクションのビットマップを作成します。
	 *	@param[in] hbm 元のビットマップ。
	 *	@param[out] _pRgb ビットマップのアドレスが返ります。
	 *	@param[out] pHeader ヘッダ情報が返ります。省略可能。
	 *	@retval NULL 失敗。
	 *	@retval NULL以外 ビットマップハンドル。 使用後は、明示的に ::DeleteObject() してください。
	 */
	static HBITMAP Create32bitDibSection(HBITMAP hbm, RGBQUAD*& _pRgb, BITMAPINFOHEADER* pHeader = NULL)
	{
		BITMAP bm;
		if ( ! ::GetObject(hbm, sizeof(BITMAP), &bm) )
		{
			return NULL;
		}
		HDC hdcSrc = ::CreateCompatibleDC(NULL);
		HDC hdcDst = ::CreateCompatibleDC(NULL);
		HGDIOBJ hbmOld = ::SelectObject(hdcSrc, hbm);
		BITMAPINFOHEADER bmi;
		Zero(bmi);
		bmi.biSize			= sizeof(BITMAPINFOHEADER);
		bmi.biWidth			= bm.bmWidth;
		bmi.biHeight		= bm.bmHeight;
		bmi.biPlanes		= 1;
		bmi.biBitCount		= 32;
		bmi.biCompression	= BI_RGB;
		HBITMAP hbmNew = ::CreateDIBSection(hdcSrc, reinterpret_cast<BITMAPINFO*>(&bmi), DIB_RGB_COLORS, reinterpret_cast<void **>(&_pRgb), NULL, 0);
		HGDIOBJ hbmOld2 = ::SelectObject(hdcDst, hbmNew);
		::BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);
		::SelectObject(hdcDst, hbmOld2);
		::SelectObject(hdcSrc, hbmOld);
		::DeleteDC(hdcSrc);
		::DeleteDC(hdcDst);
		if ( pHeader != NULL )
		{
			*pHeader = bmi;
		}
		return hbmNew;
	}

	/**
	 * [読込] リソース読込み
	 *	@note インスタンスハンドルは、 GetInstanceHandle(EI_Bitmap) の値が使用されます。
	 *	@param uBitmapResourceId ビットマップリソースID
	 *	@param cx 読み込みXサイズを指定できます。省略すると、リソースの情報のままになります。
	 *	@param cy 読み込みYサイズを指定できます。省略すると、リソースの情報のままになります。
	 *	@param fuLoad 読込みオプションを指定できます。省略可能。\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL 失敗。
	 *	@retval NULL以外 ビットマップハンドル。 使用後は、明示的に ::DeleteObject() してください。
	 */
	static HBITMAP Load(UINT uBitmapResourceId, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		HINSTANCE hIns = GetInstanceHandleByTnb(EI_Bitmap);
		LPCTSTR lpId = MAKEINTRESOURCE(uBitmapResourceId);
		return static_cast<HBITMAP>(::LoadImage(hIns, lpId, IMAGE_BITMAP, cx, cy, fuLoad));
	}

	/**
	 * [読込] BMPファイル読込み.
	 *	@param lpszFileName ファイル名。
	 *	@param cx 読み込みXサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param cy 読み込みYサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param fuLoad 読込みオプションを指定できます。省略可能。\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL 失敗。
	 *	@retval NULL以外 ビットマップハンドル。 使用後は、明示的に ::DeleteObject() してください。
	 */
	static HBITMAP Load(LPCTSTR lpszFileName, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		if ( lpszFileName == NULL || *lpszFileName == 0 )
		{
			return NULL;
		}
		HINSTANCE hIns = GetInstanceHandleByTnb(EI_Bitmap);
		#ifndef _WIN32_WCE
			return static_cast<HBITMAP>(::LoadImage(hIns, lpszFileName, IMAGE_BITMAP, cx, cy, fuLoad | LR_LOADFROMFILE));
		#else
			return static_cast<HBITMAP>(::LoadBitmap(hIns, lpszFileName));
		#endif
	}

	/**
	 * [読込] OEMリソース読込み
	 *	@param uOemBitmapResourceId OEMビットマップリソースID
	 *	@param cx 読み込みXサイズを指定できます。省略すると、リソースの情報のままになります。
	 *	@param cy 読み込みYサイズを指定できます。省略すると、リソースの情報のままになります。
	 *	@param fuLoad 読込みオプションを指定できます。省略可能。\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL 失敗。
	 *	@retval NULL以外 ビットマップハンドル。 使用後、 ::DeleteObject() する必要はありません。
	 */
	static HBITMAP LoadOem(UINT uOemBitmapResourceId, int cx = 0, int cy = 0, UINT fuLoad = 0)
	{
		LPCTSTR lpId = MAKEINTRESOURCE(uOemBitmapResourceId);
		#ifndef _WIN32_WCE
			fuLoad |= LR_SHARED;
		#endif
		return static_cast<HBITMAP>(::LoadImage(NULL, lpId, IMAGE_BITMAP, cx, cy, fuLoad));
	}

	/**
	 * [処理] イメージ描画.
	 *		指定のデバイスコンテキストの指定の位置に、指定のイメージビットマップを描画します。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param hBmp ビットマップ。
	 *	@param x 描画X位置(左が0)。省略すると０。
	 *	@param y 描画Y位置(上が0)。省略すると０。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool Draw(HDC hdc, HBITMAP hBmp, int x = 0, int y = 0)
	{
		#ifndef _WIN32_WCE
			LPARAM l = reinterpret_cast<LPARAM>(hBmp);
			bool r = !! ::DrawState(hdc, NULL, NULL, l, 0, x, y, 0, 0, DST_BITMAP | DSS_NORMAL);
			return r;
		#else
			BITMAP bm;
			if ( ::GetObject(hBmp, sizeof(BITMAP), &bm) > 0 )
			{
				HDC tempDC = ::CreateCompatibleDC(hdc);
				HGDIOBJ oldBmp = ::SelectObject(tempDC, hBmp);
				bool r =!! ::BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, tempDC, 0, 0, SRCCOPY);
				::SelectObject(tempDC, oldBmp);
				::DeleteDC(tempDC);
				return r;
			}
			return false;
		#endif
	}

	/**
	 * [作成] BMPファイルイメージ作成.
	 *	@note CE では bitsPixel は無視され必ず３２になります。
	 *	@param[out] _save 保存先。
	 *	@param[in] bmp ビットマップ
	 *	@param[in] bitsPixel 一ドットのビット数。1,4,8,16,24,32 のいずれかが指定できます。 0なら ビットマップの情報ままになります。 
	 *	@retval ERROR_SUCCESS 成功。
	 *	@retval 上記以外 失敗。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	#ifndef _WIN32_WCE
	static DWORD SaveMemory(CWorkMem& _save, HBITMAP bmp, WORD bitsPixel = 0)
	{
		BITMAP bp;
		if ( ::GetObject(bmp, sizeof(BITMAP), &bp) <= 0 ) 
		{
			return _GetLastError("GetObject"); 
		}
		// BITMAP情報生成
		BITMAPFILEHEADER fh = { 0 };
		LONG paletCount = 0;
		fh.bfType = 'B' + 'M' * 256;
		fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		CWorkMem workHead(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
		BITMAPINFOHEADER* pih = (BITMAPINFOHEADER*)workHead.Ref();
		ZeroMemory(pih, workHead.GetSize());
		pih->biSize			= sizeof(BITMAPINFOHEADER);
		pih->biWidth		= bp.bmWidth;
		pih->biHeight		= bp.bmHeight;
		pih->biPlanes		= bp.bmPlanes;
		pih->biBitCount		= (bitsPixel == 0) ? bp.bmBitsPixel : bitsPixel;
		pih->biCompression	= BI_RGB;
		pih->biSizeImage	= bp.bmWidthBytes * bp.bmHeight;
		switch(pih->biBitCount)
		{
		case 1:
			paletCount = 2;
			pih->biSizeImage = (((pih->biWidth / 4) + 3) & 0xfffffffc) * pih->biHeight;
			fh.bfOffBits += (sizeof(RGBQUAD) * 2);
			break;
		case 4:
			paletCount = 16;
			pih->biSizeImage = (((pih->biWidth / 2) + 3) & 0xfffffffc) * pih->biHeight;
			fh.bfOffBits += (sizeof(RGBQUAD) * 16);
			break;
		case 8:
			paletCount = 256;
			pih->biSizeImage = ((pih->biWidth + 3) & 0xfffffffc) * pih->biHeight;
			fh.bfOffBits += (sizeof(RGBQUAD) * 256);
			break;
		case 16:
			paletCount = 0;
			pih->biSizeImage = ((pih->biWidth * 2 + 3) & 0xfffffffc) * pih->biHeight;
			break;
		case 24:
			paletCount = 0;
			pih->biSizeImage = ((pih->biWidth * 3 + 3) & 0xfffffffc) * pih->biHeight;
			break;
		case 32:
			paletCount = 0;
			pih->biSizeImage = pih->biWidth * 4 * pih->biHeight;
			break;
		default:
			return ERROR_INVALID_PARAMETER;
		}
		// 画像データ出力
		CWorkMem data(pih->biSizeImage + 256 * sizeof(RGBQUAD));
		HDC dc = ::CreateCompatibleDC(NULL);
		HGDIOBJ old = ::SelectObject(dc, bmp);
		int r = ::GetDIBits(dc, bmp, 0, pih->biHeight, data.Ref(), reinterpret_cast<LPBITMAPINFO>(pih), DIB_RGB_COLORS);
		if ( r == 0 )
		{
			return _GetLastError("GetDIBits");
		}
		::SelectObject(dc, old);
		::DeleteDC(dc);
		fh.bfSize = fh.bfOffBits + pih->biSizeImage;
		// ヘッダ
		LPCVOID P1 = &fh;
		size_t l1 = sizeof(fh);
		// BITMAP情報
		LPCVOID P2 = pih;
		size_t l2 = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * paletCount;
		// 画像データ
		LPCVOID P3 = data.Ref();
		size_t l3 = pih->biSizeImage;
		// 出力
		_save.Resize(l1 + l2 + l3);
		BYTE* B = _save.Ref();
		MemCopy(B, P1, l1);
		B += l1;
		MemCopy(B, P2, l2);
		B += l2;
		MemCopy(B, P3, l3);
		B += l3;
		return ERROR_SUCCESS;
	}
	#else
	static DWORD SaveMemory(CWorkMem& _save, HBITMAP bmp, WORD bitsPixel = 0)
	{
		RGBQUAD* pRgb;
		BITMAPINFOHEADER header;
		BITMAPFILEHEADER fh;
		ZeroMemory(&fh, sizeof(fh));
		fh.bfType = 'B' + 'M' * 256;
		fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		HBITMAP hTmp = Create32bitDibSection(bmp, pRgb, &header);
		if ( hTmp == NULL )
		{
			return _GetLastError("GetObject");
		}
		size_t len = header.biWidth * 4 * header.biHeight;
		fh.bfSize = fh.bfOffBits + len;
		// ヘッダ
		LPCVOID P1 = &fh;
		size_t l1 = sizeof(fh);
		// BITMAP情報
		LPCVOID P2 = &header;
		size_t l2 = sizeof(header);
		// 画像データ
		LPCVOID P3 = pRgb;
		size_t l3 = len;
		// 出力
		_save.Resize(l1 + l2 + l3);
		BYTE* B = _save.Ref();
		MemCopy(B, P1, l1);
		B += l1;
		MemCopy(B, P2, l2);
		B += l2;
		MemCopy(B, P3, l3);
		B += l3;
		_DeleteObject(hTmp);
		return ERROR_SUCCESS;
	}
	#endif

	/**
	 * [作成] BMPファイル作成.
	 *	@note CE では bitsPixel は無視され必ず３２になります。
	 *	@param lpszFileName ファイル名。
	 *	@param bmp ビットマップ
	 *	@param bitsPixel 一ドットのビット数。1,4,8,16,24,32 のいずれかが指定できます。 0なら ビットマップの情報ままになります。 
	 *	@retval ERROR_SUCCESS 成功。
	 *	@retval 上記以外 失敗。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	static DWORD Save(LPCTSTR lpszFileName, HBITMAP bmp, WORD bitsPixel = 0)
	{
		CWorkMem m;
		DWORD r = SaveMemory(m, bmp, bitsPixel);
		if ( r != ERROR_SUCCESS )
		{
			return r;
		}
		// ファイル出力オープン
		HANDLE hFile = ::CreateFile(lpszFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		if ( hFile == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateFile");
		}
		DWORD tmp;
		if ( ! ::WriteFile(hFile, m.Ref(), ToDword(m.GetSize()), &tmp, NULL) )
		{
			DWORD r = _GetLastError("WriteFile");
			::CloseHandle(hFile);
			return r;
		}
		::CloseHandle(hFile);
		return ERROR_SUCCESS;	
	}
};



};//TNB
