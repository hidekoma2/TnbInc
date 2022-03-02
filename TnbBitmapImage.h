#pragma once
/**
 *@file
 * ビットマップイメージ管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapHandle.h"
#ifndef _WIN32_WCE
 #pragma comment(lib, "Msimg32.lib")
#endif
#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #ifndef AC_SRC_ALPHA
  #define AC_SRC_ALPHA                0x01
 #endif
#endif



//TNB Library
namespace TNB
{



/**@ingroup BITMAP
 * ビットマップイメージ管理クラス
 *
 *		ビットマップイメージを一つ管理します。デバイスコンテキスト との親和が高く、イメージの編集が容易です。
 *
 *	@note
 *		各種 Set() を使い、インスタンスが扱うイメージを指定します。\n
 *		各種挿入関数( Insert() , InsertOnAlphaBlend(), InsertOnTransparent(), InsertOnSemitransparect() )を使い、
 *		別イメージを画像を挿入、合成出来ます。\n
 *		Cut() でイメージの任意の箇所を切り出せます。\n
 *		Draw() などで、指定のデバイスコンテキストに描画が可能です。\n
 *		GetDC() , ReleaseDC() で、デバイスコンテキストとしてイメージを扱うことが可能です。
 *
 *	@note MFC が使える環境では、 CBitmapDC クラスも使用できます。
 *
 *	@par必要ファイル
 *			TnbBitmapImage.h
 *
 *	@date 07/10/03 新規作成
 *	@date 07/10/19 InsertOnSemitransparect() 、透過色指定可能にした。 Tile() 追加。
 *	@date 07/10/22 Attach() , Detach() 追加。自分管理と同じハンドルを渡された時に以上にならないようにした。
 *					GradationFill() 追加。
 *	@date 07/10/27 Draw() に POINT 指定可能にした。
 *	@date 07/10/27 CRawData で管理するデータを24bitから32bitに変更。
 *	@date 07/10/29 CreateMaskBitmap() , CustomizeAlpha() を追加。
 *	@date 07/10/30 CaptureClient() , CaptureWindow() , SetStretchBltMode() を追加。
 *	@date 07/11/06 ChangePixelColor() 追加。
 *	@date 07/11/13 Save() 追加。 Tile() 廃止、 DrawTile() 追加。 Set() に色指定を追加。
 *	@date 07/11/21 コピーコンストラクタのバグ修正（無効になるHDCを使用する可能性があった）。
 *	@date 07/11/23 SetFromClient() に範囲指定可能メソッドを追加。 CustomizeAlpha() を修正。
 *	@date 07/12/04 Work用のDCの持ち方を改善。
 *	@date 08/01/22 SetFromClient() の第３引数の拡張。
 *	@date 08/03/12 GradationFill() に第4引数追加。 Semitransparect 系関数の parsent 意味拡張。
 *	@date 08/07/02 BIT操作処理最適化。CE対応。
 *	@date 09/07/29 CalcAdjustRect() 新規。 Cut() で生成した画像のサイズが不正だった。
 *	@date 09/08/26 DIB を意識して、最適化。
 *	@date 11/10/11 CreateRotateImage() , CreateReverseImage() , enum EDirection 、 新規追加。
 *	@date 12/03/01 const の GetRgbDataPtr() を追加。
 *	@date 12/03/21 GradationFill() のエラーチェック強化。
 *	@date 14/12/09 SetEx() 新規。RGB がいらない巨大な画像を作成する為に追加。
 *	@date 15/05/01 CreateGrayscaleBitmap() 新規。
 *	@date 19/06/14 CreateDilationBitmap() , CreateErosionBitmap() , MedianFilter() 新規。
 *	@date 19/06/30 Paint() 新規。
 */
class CBitmapImage
{
public:

	/**@ingroup BITMAP
	 * ビットマップ生データ管理クラス
	 *
	 *		ビットマップの生データを一つ管理します。
	 *		
	 *	@note 生データは初期化しません。
	 *
	 *	@deprecated 廃止予定です。 CBitmapImage::GetRgbDataPtr() を使用してください。
	 *
	 *	@par必要ファイル
	 *			TnbBitmapImage.h
	 *
	 *	@date 07/10/03 新規作成
	 *	@date 07/10/23 構造改革.複製をむりなく。
	 */
	class CRawData
	{
		/// データ管理パラメータ
		struct TParam
		{
			SIZE				size;			///< 画像サイズ
			CWorkMemT<RGBQUAD>	work;			///< メモリ
			TParam(const SIZE& s) : size(s), work(s.cx * s.cy)
			{
			}
			TParam(void)
			{
				size.cx = size.cy = 0;
			}
		};
		CPointerHandleT<TParam>	m_pParam;		///< 生データ管理
	public:
		/// コンストラクタ
		CRawData(void) : m_pParam(new TParam()) { }
		/**
		 * コンストラクタ
		 *	@param s サイズ。
		 */
		CRawData(const SIZE& s) : m_pParam(new TParam(s)) { }
		/**
		 * [確認] 有効確認.
		 *	@retval true インスタンス内データ有効。
		 *	@retval false 無効。
		 */
		bool IsValid(void) const { return m_pParam->size.cx != 0; }
		/**
		 * [取得] サイズ取得
		 *	@return サイズ。
		 */
		const SIZE& GetSize(void) const { return m_pParam->size; }
		/**
		 * [取得] 一ラインBYTE数取得
		 *	@return BYTE数。
		 */
		size_t GetLineBytes(void) const { return m_pParam->size.cx * sizeof(RGBQUAD); }
		/**
		 * [取得] データ参照.
		 *	@note データは、左下から順に格納されています。
		 *	@note 通常、 operator[] , Rgb() を使って、一ラインずつ、一ポイントずつアドレスを取得して使用します。
		 *	@retval NULL データなし。
		 *	@retval NULL以外 データアドレス。
		 */
		RGBQUAD* Refer(void) { return m_pParam->work; }
		/**
		 * [取得] データ参照.
		 *	@note データは、左下から順に格納されています。
		 *	@note 通常、 operator[] を使って、一ラインずつ取得して使用します。
		 *	@retval NULL データなし。
		 *	@retval NULL以外 データアドレス。
		 */
		const RGBQUAD* Refer(void) const { return m_pParam->work; }
		/**
		 * [取得] ラインデータ参照.
		 *	@note データは、左から順に格納されています。
		 *	@param y ライン。 一番上が０です。
		 *	@retval NULL データなし（ラインが範囲外）。
		 *	@retval NULL以外 データアドレス。
		 */
		RGBQUAD* operator[](INDEX y)
		{
			if ( y >= static_cast<UINT>(m_pParam->size.cy) ) { return NULL; }
			return Refer() + (m_pParam->size.cy - y - 1) * m_pParam->size.cx;
		}
		/**
		 * [取得] ラインデータ参照.
		 *	@note データは、左から順に格納されています。
		 *	@param y ライン。 一番上が０です。
		 *	@retval NULL データなし（ラインが範囲外）。
		 *	@retval NULL以外 データアドレス。
		 */
		const RGBQUAD* operator[](INDEX y) const
		{
			if ( y >= static_cast<UINT>(m_pParam->size.cy) ) { return NULL; }
			return Refer() + (m_pParam->size.cy - y - 1) * m_pParam->size.cx;
		}
	};

	/// コンストラクタ
	CBitmapImage(void) : m_bmp(), m_pRgb(NULL), m_backDC(NULL), m_backBmp(NULL)
		, m_hWnd(NULL), m_nStretchMode(DEFAULT_STRETCHMODE)
	{
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * 代入コンストラクタ
	 *	@param bmp ビットマップ。
	 */
	CBitmapImage(CBitmapHandle bmp) : m_bmp(), m_pRgb(NULL), m_backDC(NULL), m_backBmp(NULL)
		, m_hWnd(NULL), m_nStretchMode(DEFAULT_STRETCHMODE)
	{
		m_Set(bmp);
	}
	
	/**
	 * コピーコンストラクタ
	 *	@note	本クラスのコピーは、コピー元、コピー先で同じハンドルを共有することになります。
	 *			コピー後、編集目的の場合、 Clone() や Set() を使ってください。
	 *	@param other コピー元。
	 */
	CBitmapImage(const CBitmapImage& other) : m_bmp(other.m_bmp), m_pRgb(other.m_pRgb), m_backDC(NULL), m_backBmp(NULL)
		, m_hWnd(other.m_hWnd), m_size(other.m_size), m_nStretchMode(other.m_nStretchMode)
	{
	}

	/**
	 * コピーオペレータ
	 *	@note	本クラスのコピーは、コピー元、コピー先で同じハンドルを共有することになります。
	 *			コピー後、編集目的の場合、 Clone() や Set() を使ってください。
	 *	@param other コピー元。
	 *	@return 自分の参照。
	 */
	CBitmapImage& operator=(const CBitmapImage& other)
	{
		m_ReleaseDC();
		m_bmp = other.m_bmp;
		m_pRgb = other.m_pRgb;
		m_size = other.m_size;
		m_hWnd = other.m_hWnd;
		m_nStretchMode = other.m_nStretchMode;
		return *this;
	}

	/// デストラクタ
	virtual ~CBitmapImage(void)
	{
		Empty();
	}

	/**
	 * [取得] HBITMAP 取得
	 *	@note 本インスタンスで管理しているハンドルを取得できます。 Deleteしてはいけません。
	 *	@return HBITMAP。 Empty状態の場合、 NULL 状態のハンドルが返ります。
	 */
	operator CBitmapHandle(void)
	{
		return m_bmp;
	}

	/**
	 * [取得] ビットマップハンドル取得
	 *	@note 本インスタンスで管理しているハンドルを取得できます。 Deleteしてはいけません。
	 *	@return ビットマップハンドル。 本インスタンスが Empty状態の場合、 NULL 状態のハンドルが返ります。
	 */
	CBitmapHandle GetBitmapHandle(void)
	{
		return m_bmp;
	}

	/**
	 * [設定] ワークデバイスコンテキスト設定.
	 *	@note 設定しない場合、デスクトップ(NULL)にになっています。
	 *	@param h ワークウィンドウハンドル。 
	 */
	void SetWorkWnd(HWND h)
	{
		m_hWnd = h;
	}

	/**
	 * [設定] ビットマップ伸縮モード設定.
	 *	@note API ::StretchBlt 関数のビットマップ伸縮モードを設定します。
	 *	@param nStretchMode 伸縮モード。以下の値から選択可能です。
	 *			<table><tr><td>
	 *			値											</td><td>
	 *					説明								</td></tr><tr><td>
	 *			STRETCH_ANDSCANS							</td><td>
	 *					論理 AND 演算子を使って、残す点の色と取り除く点の色を組み合わせます。
	 *					ビットマップがモノクロームのビットマップである場合、白のピクセルが消され、
	 *					 黒のピクセルが残ります。			</td></tr><tr><td>
	 *			STRETCH_DELETESCANS							</td><td>
	 *					ピクセルを削除します。取り除く点の情報を何らかの形で維持せようとはせず、
	 *					単純にそれらの点を削除します。		</td></tr><tr><td>
	 *			STRETCH_HALFTONE							</td><td>
	 *					コピー元長方形内のピクセルをコピー先長方形内のピクセルブロックに関連付けます。
	 *					コピー先ブロックの平均的な色は、コピー元のピクセルの色に近い色になります。 
	 *					Windows 95/98 では、この値はサポートされていません。</td></tr><tr><td>
	 *			STRETCH_ORSCANS								</td><td>
	 *					論理 OR 演算子を使って、残す点の色と取り除く点の色を組み合わせます。
	 *					ビットマップがモノクロームのビットマップである場合、黒のピクセルが消され、
	 *					白のピクセルが残ります。			</td></tr>
	 */
	void SetStretchBltMode(int nStretchMode)
	{
		m_nStretchMode = nStretchMode;
		if ( m_backDC != NULL )
		{
			::SetStretchBltMode(m_backDC, m_nStretchMode);
			::SetBrushOrgEx(m_backDC, 0, 0, NULL);
		}
	}
 
	/**
	 * [設定] 破棄.
	 *		本インスタンスで持っているイメージを破棄します。本インスタンスは Empty状態になります。
	 */
	void Empty(void)
	{
		m_ReleaseDC();
		m_bmp.Null();
		m_pRgb = NULL;
	}

	/**
	 * [確認] Empty状態確認.
	 *	@retval true Empty状態。イメージは持っていません。
	 *	@retval false イメージを持っています。
	 */
	bool IsEmpty(void) const
	{
		return m_bmp.IsNull();
	}

	/**
	 * [取得] イメージサイズ取得.
	 *	@note Empty状態でも最後に扱っていた情報が戻ります。必ず、 IsEmpty() で確認してから取得してください。
	 *	@return サイズ。
	 */
	const SIZE& GetSize(void) const
	{
		ASSERT0( ! IsEmpty(), "CBitmapImage::GetSize()", "画像を持っていないオブジェクトからサイズを取得しようとしています。" );
		return m_size;
	}

	/**
	 * [取得] BITMAPINFOHEADER 取得.
	 *	@param[out] _head BITMAPINFOHEADER 格納先。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GetBitmapInfo(BITMAPINFOHEADER& _head) const
	{
		BITMAP bp;
		if ( ::GetObject(m_bmp, sizeof(BITMAP), &bp) > 0 )
		{
			::ZeroMemory(&_head, sizeof(BITMAPINFOHEADER));
			_head.biSize		= sizeof(BITMAPINFOHEADER);
			_head.biWidth		= bp.bmWidth;
			_head.biHeight		= bp.bmHeight;
			_head.biPlanes		= bp.bmPlanes;
			_head.biBitCount	= bp.bmBitsPixel;
			_head.biCompression	= BI_RGB;
			_head.biSizeImage	= bp.bmWidthBytes * bp.bmHeight;
			return true;
		}
		return false;
	}

	/**
	 * [複製] クローン作成
	 *	@note 本クラスは、複製が可能です。ただし、同じイメージ情報を共有してしまいます。
	 *			本メソッドを使うと、別のイメージ情報になります。
	 *	@return 複製。
	 */
	CBitmapImage Clone(void) const
	{
		CBitmapImage bi;
		bi.Set(m_bmp);
		return bi;
	}

	/**
	 * [設定] アタッチ.
	 *		指定のハンドルを本クラスで共有します。 Set() に比べ(複製しない分)高速ですが、
	 *		同じハンドルを共有するため、一方が変更すると影響を受けてしまいます。
	 *	@param bmp ビットマップ。 
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Attach(CBitmapHandle bmp)
	{
		return m_Set(bmp);
	}

	/**
	 * [設定] デタッチ.
	 *		本インスタンスで使用しているビットマップハンドルを返し、 Empty 状態になります。
	 *	@note Attach 後、本メソッドを使わず、インスタンスを破棄しても問題ありません。 
	 *	@return 本インスタンスが保持していたビットマップハンドル。 
	 */
	CBitmapHandle Detach(void)
	{
		CBitmapHandle h = m_bmp;
		Empty();
		return h;
	}

	/**
	 * [設定] イメージ設定.
	 *	@note 本インスタンスで保持しているイメージを破棄し、指定のサイズのビットマップのイメージを保持します。
	 *	@param cx ビットマップの幅。
	 *	@param cy ビットマップの高さ。
	 *	@param color 塗りつぶし色。省略すると、塗りつぶしません。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool Set(int cx, int cy, COLORREF color = CLR_INVALID)
	{
		CWorkDC workDC(m_hWnd);
		RGBQUAD* pRgb = NULL;
		HBITMAP h = ms_CreateBitmap(pRgb, workDC, cx, cy);
		if ( h != NULL && m_Set(h, pRgb) )
		{
			ASSERT( m_pRgb != NULL );
			if ( Fill(color) ) 
			{
				return true;
			}
		}
		Empty();
		return false;
	}

	/**
	 * [設定] 拡張イメージ設定.
	 *	@note 本インスタンスで保持しているイメージを破棄し、指定のサイズのビットマップのイメージを保持します。
	 *		その際指定のウィンドウのデバイスコンテキストの色形式でビットマップを作成します。
	 *	@param hDC デバイスコンテキスト
	 *	@param cx ビットマップの幅。
	 *	@param cy ビットマップの高さ。
	 *	@param color 塗りつぶし色。省略すると、塗りつぶしません。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool SetEx(HDC hDC, int cx, int cy, COLORREF color = CLR_INVALID)
	{
		HBITMAP h = ::CreateCompatibleBitmap(hDC, cx, cy);
		bool r = ( h != NULL && m_Set(h) );
		if ( r )
		{
			r = Fill(color); 
		}
		return r;
	}

	/**
	 * [設定] イメージ設定.
	 *		本インスタンスで保持しているイメージを破棄し、指定のビットマップのイメージを複製し保持します。
	 *	@note 元のビットマップを変更しても影響ありません。
	 *	@param bmp ビットマップ。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool Set(CBitmapHandle bmp)
	{
		RGBQUAD* pRgb = NULL;
		return m_Set(CBitmapHandle::Create32bitDibSection(bmp, pRgb), pRgb);
	}

	/**
	 * [設定] イメージ設定.
	 *	@note 本インスタンスで保持しているイメージを破棄し、指定のビットマップのイメージを保持します。
	 *	@param bmpimg ビットマップイメージ。
	 *	@param cx ビットマップの幅。省略するとイメージ情報のままの幅になります。
	 *	@param cy ビットマップの高さ。省略するとイメージ情報のままの高さになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool Set(const CBitmapImage& bmpimg, int cx = 0, int cy = 0)
	{
		if ( cx == 0 && cy == 0 ) 
		{
			return Set(bmpimg.m_bmp); 
		}
		bmpimg.m_CheckSize(cx, cy);
		if ( Set(cx, cy) )
		{
			HDC dc = GetDC();
			if ( dc != NULL )
			{
				bool r = bmpimg.StretchBlt(dc, 0, 0, SRCCOPY, cx, cy);
				ReleaseDC();
				return r; 
			}
		}
		Empty();
		return false;
	}

	/**
	 * [設定] 生データからイメージ設定.
	 *	@param rawData 生データ管理クラス。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 *	@deprecated 廃止予定のメソッドです。
	 */
	bool _deprecated Set(const CRawData& rawData)
	{
		Empty();
		if ( rawData.IsValid() )
		{
			const SIZE& size = rawData.GetSize();
			if ( Set(size.cx, size.cy) )
			{
				ASSERT( m_pRgb != NULL );
				MemCopy(m_pRgb, rawData.Refer(), m_size.cy * m_size.cx);
			}
		}
		return ! m_bmp.IsNull();
	}

	/**
	 * [設定] DCからイメージ設定.
	 *	@note 本インスタンスで保持しているイメージを破棄し、指定のデバイスコンテキストのイメージを保持します。
	 *	@param hdc 取り込み元デバイスコンテキスト。
	 *	@param rect hdc のどの位置を取り込むか指定します。
	 *	@param cx ビットマップの幅。省略するとイメージ情報のままの幅になります。
	 *	@param cy ビットマップの高さ。省略するとイメージ情報のままの高さになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool SetFromDC(HDC hdc, const RECT& rect, int cx = 0, int cy = 0) 
	{
		Empty();
		SIZE s;
		if ( ms_CheckSize(s, rect) && Set(s.cx, s.cy) )
		{
			HDC dc = GetDC();
			if ( dc != NULL )
			{
				if ( cx <= 0 || cy <= 0 )
				{
					::BitBlt(dc, 0, 0, s.cx, s.cy, hdc, rect.left, rect.top, SRCCOPY);	//DCから取り出し
				}
				else
				{
					::StretchBlt(dc, 0, 0, cx, cy, hdc, rect.left, rect.top, s.cx, s.cy, SRCCOPY);	//DCから取り出し
				}
				ReleaseDC();
				return true;
			}
		}
		Empty();
		return false;
	}

	/**
	 * [作成] ICONからイメージ設定.
	 *	@param hIcon アイコン。
	 *	@param backColor 透過色のエリアの色指定。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool SetFromIcon(HICON hIcon, COLORREF backColor)
	{
		Empty();
		ICONINFO ii;
		::GetIconInfo(hIcon, &ii);
		CBitmapImage biColor;
		CBitmapImage biMask;
		if ( ! biColor.Set(ii.hbmColor) || ! biMask.Set(ii.hbmMask) )
		{
			return false;
		}
		const SIZE& size = biColor.GetSize();
		bool r = Set(size.cx, size.cy, backColor);
		r &= Insert(0, 0, biMask, SRCAND);
		r &= Insert(0, 0, biColor, SRCPAINT);
		if ( ! r ) 
		{ 
			Empty();
		}
		return r;
	}

	/**
	 * [設定] クライアントからイメージ設定.
	 *		指定のウィンドウのクライアント領域の指定範囲のイメージを取り込みます。
	 *	@note WM_PRINTCLIENT に対応している必要があります。
	 *	@note CE ではイメージは取り込めませんが、成功でリターンします。
	 *	@param hWnd ウィンドウハンドル
	 *	@param rect 範囲。
	 *	@param backColor 背景色。クリッピング範囲外の色を指定します。 
	 *					CLR_AUTOSELECT なら、 WM_ERASEBKGND を使い、先に背景を描画します。
	 *					省略すると、背景を描画処理しません。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool SetFromClient(HWND hWnd, const RECT& rect, COLORREF backColor = CLR_INVALID)
	{
		if ( Set(rect.right - rect.left, rect.bottom - rect.top, backColor) )
		{
#ifndef _WIN32_WCE
			HDC dc = GetDC();
			if ( dc != NULL )
			{
				::SetWindowOrgEx(dc, rect.left, rect.top, NULL);
				WPARAM w = reinterpret_cast<WPARAM>(dc);
				if ( backColor == CLR_AUTOSELECT )
				{
					::SendMessage(hWnd, WM_ERASEBKGND, w, 0);
				}
				::SendMessage(hWnd, WM_PRINTCLIENT, w, PRF_CLIENT);
				ReleaseDC();
				return true;
			}
#else
			return true;
#endif
		}
		Empty();
		return false;
	}

	/**
	 * [設定] クライアントからイメージ設定.
	 *		指定のウィンドウのクライアント領域のイメージを取り込みます。
	 *	@note WM_PRINTCLIENT に対応している必要があります。
	 *	@param hWnd ウィンドウハンドル
	 *	@param backColor 背景色。クリッピング範囲外の色を指定します。 
	 *					CLR_AUTOSELECT なら、 WM_ERASEBKGND を使い、先に背景を描画します。
	 *					省略すると、背景を描画処理しません。
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool SetFromClient(HWND hWnd, COLORREF backColor = CLR_INVALID)
	{
		RECT rect;
		if ( ::GetClientRect(hWnd, &rect) )
		{
			return SetFromClient(hWnd, rect, backColor);
		}
		Empty();
		return false;
	}

	#ifndef _WIN32_WCE
	/**
	 * [設定] ウィンドウからイメージ設定.
	 *		指定のウィンドウ領域のイメージを取り込みます。
	 *	@note WM_PRINT に対応している必要があります。
	 *	@param hWnd ウィンドウハンドル
	 *	@param backColor 背景色。クリッピング範囲外の色を指定します。
	 *	@param mode 取り込みモード。以下の値の組み合わせを指定します。
	 *		<table><tr><td>
	 *		マスク										</td><td>
	 *					説明							</td></tr><tr><td>
	 *		PRF_CHECKVISIBLE							</td><td>
	 *					見えている状態のときのみ描画	</td></tr><tr><td>
	 *		PRF_CHILDREN								</td><td>
	 *					見えている子ウィンドウのみ描画	</td></tr><tr><td>
	 *		PRF_CLIENT									</td><td>
	 *					クライアント領域を描画			</td></tr><tr><td>
	 *		PRF_ERASEBKGND								</td><td>
	 *					描画前に背景を再描画			</td></tr><tr><td>
	 *		PRF_NONCLIENT								</td><td>
	 *					非クライアント領域を描画		</td></tr><tr><td>
	 *		PRF_OWNED									</td><td>
	 *					所有する全てのウィンドウ		</td></tr>
	 *		</table>
	 *	@retval true 成功。
	 *	@retval false 失敗。Empty状態になります。
	 */
	bool SetFromWindow(HWND hWnd, COLORREF backColor = CLR_INVALID,
						LPARAM mode = PRF_CLIENT | PRF_ERASEBKGND | PRF_NONCLIENT | PRF_OWNED)
	{
		RECT rect;
		if ( ::GetWindowRect(hWnd, &rect) && Set(rect.right - rect.left, rect.bottom - rect.top, backColor) )
		{
			HDC dc = GetDC();
			if ( dc != NULL )
			{
				::SendMessage(hWnd, WM_PRINT, reinterpret_cast<WPARAM>(dc), mode);
				ReleaseDC();
				return true;
			}
		}
		Empty();
		return false;
	}

	/**
	 * [挿入] イメージ挿入.
	 *	@note 本インスタンスで保持しているイメージの上に、指定のビットマップのイメージを追加します。
	 *	@note WIN32API DrawState を使用しています。
	 *	@param x 挿入X座標。
	 *	@param y 挿入Y座標。
	 *	@param hBitmap 挿入画像。
	 *	@param uExFlag 挿入フラグ。以下の3種、使用できます。\n
	 *				省略か DSS_NORMAL - - - イメージをそのまま描画します。 \n
	 *				DSS_DISABLED - - - イメージを浮き上がらせます（エンボス）。\n
	 *				DSS_UNION - - - イメージをディザリングします。 
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool InsertEx(int x, int y, HBITMAP hBitmap, UINT uExFlag = DSS_NORMAL)
	{
		HDC dc = GetDC();
		if ( dc != NULL ) 
		{
			LPARAM l = reinterpret_cast<LPARAM>(hBitmap);
			bool r = !! ::DrawState(dc, NULL, NULL, l, 0, x, y, 0, 0, DST_BITMAP | uExFlag);
			ReleaseDC();
			return r;
		}
		return false; 
	}
	#endif

	/**
	 * [挿入] イメージ挿入.
	 *	@note 本インスタンスで保持しているイメージの上に、指定のビットマップのイメージを追加します。
	 *	@note cx , cy を指定すると、これを使うと、拡大縮小して挿入することが可能です。また、
	 *			負の数を指定すると、反転して挿入することが出来ます。ただし、挿入座標を軸に反転しますので、
	 *			挿入位置に注意してください。
	 *	@note WIN32API BitBlt、StretchBlt を使用しています。
	 *	@param x 挿入X座標。
	 *	@param y 挿入Y座標。
	 *	@param bmpimg 挿入ビットマップイメージ。
	 *	@param raster オペレーションコード。省略すると、 SRCCOPY になります。\n
	 *			::BitBlt の dwRop のコードを指定します。以下一般的なコードです\n
	 *			<table><tr><td>
	 *			コード								</td><td>
	 *					説明						</td></tr><tr><td>
	 *			BLACKNESS				</td><td>
	 *					物理パレットのインデックス 0 に対応する色（既定の物理パレットでは黒）で、コピー先の長方形を塗りつぶします。 </td></tr><tr><td>
	 *			DSTINVERT				</td><td>
	 *					コピー先長方形の色を反転します。 </td></tr><tr><td>
	 *			MERGECOPY				</td><td>
	 *					論理 AND 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			MERGEPAINT				</td><td>
	 *					論理 OR 演算子を使って、コピー元の色を反転した色と、コピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			NOTSRCCOPY				</td><td>
	 *					コピー元の色を反転して、コピー先へコピーします。 </td></tr><tr><td>
	 *			NOTSRCERASE					</td><td>
	 *					論理 OR 演算子を使って、コピー元の色とコピー先の色を組み合わせ、さらに反転します。 </td></tr><tr><td>
	 *			PATCOPY					</td><td>
	 *					指定したパターンをコピー先へコピーします。 </td></tr><tr><td>
	 *			PATINVERT				</td><td>
	 *					論理 XOR 演算子を使って、指定したパターンの色と、コピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			PATPAINT				</td><td>
	 *					論理 OR 演算子を使って、指定したパターンの色と、コピー元の色を反転した色を組み合わせます。さらに論理 OR 演算子を使って、その結果と、コピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCAND				</td><td>
	 *					論理 AND 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCCOPY					</td><td>
	 *					コピー元長方形をコピー先長方形へそのままコピーします。 </td></tr><tr><td>
	 *			SRCERASE				</td><td>
	 *					論理 AND 演算子を使って、コピー先の色を反転した色と、コピー元の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCINVERT				</td><td>
	 *					論理 XOR 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCPAINT				</td><td>
	 *					論理 OR 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			WHITENESS				</td><td>
	 *					物理パレットのインデックス 1 に対応する色（既定の物理パレットでは白）で、コピー先の長方形を塗りつぶします。 </td></tr>
	 *		</table>
	 *	@param cx 挿入幅。省略するとイメージ情報のままの幅になります。これを使うと、拡大縮小して挿入することが可能です。
	 *	@param cy 挿入高さ。省略するとイメージ情報のままの高さになります。これを使うと、拡大縮小して挿入することが可能です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Insert(int x, int y, const CBitmapImage& bmpimg, DWORD raster = SRCCOPY, int cx = 0, int cy = 0)
	{
		if ( ! IsEmpty() && ! bmpimg.IsEmpty() )
		{
			HDC dc = GetDC();
			if ( dc != NULL ) 
			{
				bool r = bmpimg.StretchBlt(dc, x, y, raster, cx, cy);
				ReleaseDC();
				return r;
			}
		}
		return false;
	}

	/**
	 * [挿入] 透過処理付イメージ挿入.
	 *	@note 本インスタンスで保持しているイメージの上に、指定のビットマップのイメージを追加します。
	 *	@note WIN32API TransparentBlt を使用しています。
	 *	@param x 挿入X座標。
	 *	@param y 挿入Y座標。
	 *	@param bmpimg 挿入ビットマップイメージ。
	 *	@param color 透過させるカラーを指定します。 
	 *						省略すると、 bmpimg の左下の色を透過色とします。\n
	 *						CLR_INVALID を指定すると透過処理なしで挿入します。
	 *	@param cx 挿入幅。省略するとイメージ情報のままの幅になります。これを使うと、拡大縮小して挿入することが可能です。
	 *	@param cy 挿入高さ。省略するとイメージ情報のままの高さになります。これを使うと、拡大縮小して挿入することが可能です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool InsertOnTransparent(int x, int y, const CBitmapImage& bmpimg, COLORREF color = CLR_AUTOSELECT, int cx = 0, int cy = 0)
	{
		if ( ! IsEmpty() && ! bmpimg.IsEmpty() )
		{
			HDC dc = GetDC();
			if ( dc != NULL )
			{
				bool r = bmpimg.TransparentBit(dc, x, y, color, cx, cy);
				ReleaseDC();
				return r;
			}
		}
		return false;
	}

	/**
	 * [挿入] 半透過処理付イメージ挿入.
	 *	@note 本インスタンスで保持しているイメージの上に、指定のビットマップのイメージを追加します。
	 *	@note WIN32API AlphaBlend を使用しています。
	 *	@param x 挿入X座標。
	 *	@param y 挿入Y座標。
	 *	@param bmpimg 挿入ビットマップイメージ。
	 *	@param parsent 挿入画像の濃さ(単位;%) 省略なら100% 。
	 *				-1 を指定すると、 保持しているイメージのピクセル毎のアルファチャネルを使用します。
	 *	@param cx 挿入幅。省略するとイメージ情報のままの幅になります。これを使うと、拡大縮小して挿入することが可能です。
	 *	@param cy 挿入高さ。省略するとイメージ情報のままの高さになります。これを使うと、拡大縮小して挿入することが可能です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool InsertOnAlphaBlend(int x, int y, const CBitmapImage& bmpimg, int parsent = 100, int cx = 0, int cy = 0)
	{
		if ( ! IsEmpty() && ! bmpimg.IsEmpty() )
		{
			HDC dc = GetDC();
			if ( dc != NULL )
			{
				bool r = bmpimg.AlphaBlend(dc, x, y, parsent, cx, cy);
				ReleaseDC();
				return r;
			}
		}
		return false;
	}

	/**
	 * [挿入] 半透明イメージ挿入(透過色付).
	 *	@note 本インスタンスで保持しているイメージの上に、指定のビットマップのイメージを追加します。
	 *	@todo 一ドット毎に計算しているため、処理速度は速くありません。
	 *
	 *	@param x 挿入X座標。
	 *	@param y 挿入Y座標。
	 *	@param bmpimg 挿入ビットマップイメージ。
	 *	@param parsent 挿入画像の濃さ(単位;%) 省略なら100%。
	 *				-1 を指定すると、 保持しているイメージのピクセル毎のアルファチャネルを使用します。
	 *	@param color 透過させるカラーを指定します。 
	 *						省略すると、 bmpimg の左下の色を透過色とします。\n
	 *						CLR_INVALID を指定すると透過処理なしで挿入します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool InsertOnSemitransparect(int x, int y, const CBitmapImage& bmpimg, int parsent = 100, COLORREF color = CLR_AUTOSELECT)
	{
		if ( ! IsEmpty() && ! bmpimg.IsEmpty() )
		{
			HDC dc = GetDC();
			if ( dc != NULL )
			{
				bool r = bmpimg.SemitransparectBlt(dc, x, y, parsent, color);
				ReleaseDC();
				return r;
			}
		}
		return false;
	}

	/**
	 * [処理] 四角描画.
	 *	@note 本インスタンスで保持しているイメージの上に、指定の色で指定の座標に四角を描きます。
	 *	@param color カラー。
	 *	@param rect 座標。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Fill(COLORREF color, const RECT& rect)
	{
		HDC dc = GetDC();
		if ( dc != NULL )
		{
			COLORREF c = ::GetBkColor(dc);
			::SetBkColor(dc, color);
			bool r = !! ::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
			::SetBkColor(dc, c);
			ReleaseDC();
			return r;
		}
		return false;
	}

	/**
	 * [処理] 四角描画.
	 *	@note 本インスタンスで保持しているイメージを、指定の色で塗りつぶします。
	 *	@param color カラー。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Fill(COLORREF color)
	{
		RECT rect = { 0, 0, m_size.cx, m_size.cy };
		return Fill(color, rect);
	}

	/// 方向
	enum EDirection
	{
		Vertical,		///< 垂直
		Horizontal		///< 水平
	};

	/**
	 * [処理] グラデーション描画.
	 *	@note 本インスタンスで保持しているイメージの上に、指定の色でグラデーションを描きます。
	 *	@param color1 左/下カラー。
	 *	@param color2 右/上カラー。
	 *	@param direction 方向。 Vertical か Horizontal を指定します。
	 *	@param pRect 範囲を指定します。省略すると全体が対象になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GradationFill(COLORREF color1, COLORREF color2, EDirection direction, const RECT* pRect = NULL)
	{
		HDC dc = GetDC();
		if ( dc != NULL )
		{
			SIZE size = m_size;
			if ( pRect != NULL )
			{
				size.cx = pRect->right - pRect->left;
				size.cy = pRect->bottom - pRect->top;
			}
			int c = 0;
			if ( direction == Horizontal )
			{
				c = size.cx;
				size.cy = 1;
			}
			else
			{
				c = size.cy;
				size.cx = 1;
			}
			if ( size.cx > 0 && size.cy > 0 )
			{
				CBitmapImage bi;
				bi.Set(size.cx, size.cy);
				ASSERT( bi.m_pRgb != NULL );
				int r1 = GetRValue(color1);
				int g1 = LOBYTE((color1 >> 8) & 0xFF);
				int b1 = GetBValue(color1);
				int r2 = GetRValue(color2);
				int g2 = LOBYTE((color2 >> 8) & 0xFF);
				int b2 = GetBValue(color2);
				int a = c - 1;
				if ( a <= 0 ) { a = 1; }
				loop ( i, c )
				{
					bi.m_pRgb[i].rgbRed = static_cast<BYTE>((r1 * (a - i) / a) + (r2 * i / a));
					bi.m_pRgb[i].rgbGreen = static_cast<BYTE>((g1 * (a - i) / a) + (g2 * i / a));
					bi.m_pRgb[i].rgbBlue = static_cast<BYTE>((b1 * (a - i) / a) + (b2 * i / a));
				}
				bool r = false;
				if ( pRect != NULL )
				{
					::SetBrushOrgEx(dc, pRect->left, pRect->top, NULL);
					r = bi.DrawTile(dc, *pRect);
					::SetBrushOrgEx(dc, 0, 0, NULL);
				}
				else
				{
					RECT rect = { 0, 0, m_size.cx, m_size.cy };
					r = bi.DrawTile(dc, rect);
				}
				ReleaseDC();
				return r;
			}
		}
		return false;
	}

	/**
	 * [処理] グラデーション描画.
	 *	@note 本インスタンスで保持しているイメージの上に、指定の色でグラデーションを描きます。
	 *	@param color1 左/下カラー。
	 *	@param color2 右/上カラー。
	 *	@param boIsHorizontal true なら水平(左から右)のグラデーションになります。
	 *	@param pRect 範囲を指定します。省略すると全体が対象になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GradationFill(COLORREF color1, COLORREF color2, bool boIsHorizontal, const RECT* pRect = NULL)
	{
		EDirection d = boIsHorizontal ? Horizontal : Vertical;
		return GradationFill(color1, color2, d, pRect);
	}

	/**
	 * [処理] グラデーション描画.
	 *	@note 本インスタンスで保持しているイメージの上に、指定の色でグラデーションを描きます。
	 *	@param pVertex 三角形の頂点を定義する、各 TRIVERTEX 構造体の配列へのポインタを指定します。 
	 *	@param dwNumVertex 頂点の数を指定します。 
	 *	@param pMesh GRADIENT_RECT 構造体の配列を指定します。 
	 *	@param dwNumMesh pMesh パラメータの要素数を指定します。 
	 *	@param boIsHorizontal true なら水平(左から右)のグラデーションになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool GradientFill(const PTRIVERTEX pVertex, DWORD dwNumVertex, const PGRADIENT_RECT pMesh, DWORD dwNumMesh, bool boIsHorizontal)
	{
		HDC dc = GetDC();
		if ( dc != NULL )
		{
			DWORD dwMode = boIsHorizontal ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V;
			bool r = !! ::GradientFill(dc, pVertex, dwNumVertex, pMesh, dwNumMesh, dwMode);
			ReleaseDC();
			return r;
		}
		return false;
	}

	/**
	 * [処理] アルファチャネル操作.
	 *	@note 本インスタンスで保持しているイメージのアルファチャネルを操作します。
	 *	@param alpha 設定するアルファ値
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool CustomizeAlpha(BYTE alpha)
	{
		if ( m_CheckDib() )
		{
			loop ( i, m_size.cx * m_size.cy )
			{
				m_pRgb[i].rgbReserved = alpha;
			}
			return true;
		}
		return false;
	}

	/**
	 * [処理] アルファチャネル操作.
	 *	@note 本インスタンスで保持しているイメージのアルファチャネルを操作します。
	 *	@param alpha 設定するアルファ値
	 *	@param colorEx 別のアルファ値に設定したいカラーを指定します。
	 *	@param alphaEx colorExの色に設定するアルファ値。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool CustomizeAlpha(BYTE alpha, COLORREF colorEx, BYTE alphaEx)
	{
		if ( m_CheckDib() )
		{
			colorEx = (colorEx & 0x0000ff00) | (colorEx & 0x00ff0000) >> 16 | (colorEx & 0x000000ff) << 16;
			RGBQUAD* pRgba = m_pRgb;
			loop ( i, m_size.cx * m_size.cy )
			{
				if ( (*(reinterpret_cast<COLORREF*>(pRgba)) & 0x00FFFFFF) == colorEx )
				{
					pRgba->rgbReserved = alphaEx;
				}
				else
				{
					pRgba->rgbReserved = alpha;
				}
				pRgba++;
			}
			return true;
		}
		return false;
	}

	/**
	 * [処理] アルファチャネル操作.
	 *	@note 本インスタンスで保持しているイメージのアルファチャネルを操作します。
	 *	@param bmp アルファ値元（REDの要素を使用）
	 *	@retval true 成功。
	 *	@retval false 失敗。同じ大きさの画像でない時も失敗します。
	 */
	bool CustomizeAlpha(CBitmapHandle bmp)
	{
		CBitmapImage bi;
		if ( ! bi.Set(bmp) )
		{
			return false;
		}
		if ( ! m_CheckDib() || ! bi.m_CheckDib() )
		{
			return false; 
		}
		if ( bi.m_size.cx < m_size.cx || bi.m_size.cy < m_size.cy )
		{
			return false; 
		}
		loop ( y, m_size.cy )
		{
			RGBQUAD* pSrc = bi.GetRgbDataPtr(y);
			RGBQUAD* pBase = GetRgbDataPtr(y);
			loop ( x, m_size.cx )
			{
				pBase->rgbReserved = pSrc->rgbRed;
				pSrc++;
				pBase++;
			}
		}
		return true;
	}

	/**
	 * [処理] カラー変更.
	 *	@note 本インスタンスで保持しているイメージの指定の色を指定の色に置き換えます。
	 *	@param targetColor 変更したい色。
	 *	@param drawColor 変更後の色。
	 *	@retval マイナス 失敗。
	 *	@retval 0以上 変更したPixel数。
	 */
	int ChangePixelColor(COLORREF targetColor, COLORREF drawColor)
	{
		if ( m_CheckDib() )
		{
			int rc = 0;
			targetColor = (targetColor & 0x0000ff00) | (targetColor & 0x00ff0000) >> 16 | (targetColor & 0x000000ff) << 16;
			drawColor = (drawColor & 0x0000ff00) | (drawColor & 0x00ff0000) >> 16 | (drawColor & 0x000000ff) << 16;
			COLORREF* pRgba = reinterpret_cast<COLORREF*>(m_pRgb);
			loop ( i, m_size.cx * m_size.cy )
			{
				if ( ((*pRgba) & 0x00FFFFFF) == targetColor )
				{
					*pRgba = drawColor;
					rc++;
				}
				pRgba++;
			}
			return rc;
		}
		return -1;
	}

	/**
	 * [処理] カラー変更.
	 *		元画像の色から RGBの要素比率で色を変更できます。
	 *	@note 元と同じ絵にするなら RGB(255,0,0), RGB(0,255,0),  RGB(0,0,255) を指定することになります。
	 *	@param r 赤の要素比率。
	 *	@param g 緑の要素比率。
	 *	@param b 青の要素比率。
	 *	@retval false 失敗。
	 *	@retval true 成功。
	 */
	bool ChangePixelColor(COLORREF r, COLORREF g, COLORREF b)
	{
		struct TRGB
		{
			BYTE rgbRed;
			BYTE rgbGreen;
			BYTE rgbBlue;
			BYTE rgbReserved;
		};
		if ( m_CheckDib() )
		{
			TRGB* R = reinterpret_cast<TRGB*>(&r);
			TRGB* G = reinterpret_cast<TRGB*>(&g);
			TRGB* B = reinterpret_cast<TRGB*>(&b);
			RGBQUAD* P = m_pRgb;
			loop ( i, m_size.cx * m_size.cy )
			{
				int rr = P->rgbRed * R->rgbRed + P->rgbGreen * R->rgbGreen + P->rgbBlue * R->rgbBlue;
				int gg = P->rgbRed * G->rgbRed + P->rgbGreen * G->rgbGreen + P->rgbBlue * G->rgbBlue;
				int bb = P->rgbRed * B->rgbRed + P->rgbGreen * B->rgbGreen + P->rgbBlue * B->rgbBlue;
				P->rgbRed = static_cast<BYTE>(min(rr / 255, 255));
				P->rgbGreen = static_cast<BYTE>(min(gg / 255, 255));
				P->rgbBlue = static_cast<BYTE>(min(bb / 255, 255));
				P++;
			}
			return true;
		}
		return false;
	}

	/**
	 * [取得] イメージ取り出し.
	 *	@note 本インスタンスで保持しているイメージから、指定の矩形範囲を取り出します。
	 *	@param rect 取り出し範囲。
	 *	@param cx 取り出し後の幅。省略するとイメージ情報のままの幅になります。これを使うと、拡大縮小して取り出すことが可能です。
	 *	@param cy 取り出し後の高さ。省略するとイメージ情報のままの高さになります。これを使うと、拡大縮小して取り出すことが可能です。
	 *	@return ビットマップイメージ。 失敗した場合、Empty状態のものが返ります。
	 */
	CBitmapImage Cut(const RECT& rect, int cx = 0, int cy = 0) const
	{
		CBitmapImage bi;
		SIZE s;
		if ( ms_CheckSize(s, rect) )
		{
			ms_CheckSize(cx, cy, s);
			CWorkDC workDC(m_hWnd);
			CTempDC dc(workDC, m_bmp);
			bi.Set(cx, cy);
			HDC tempDc = bi.GetDC();
			if ( tempDc != NULL )
			{
				bool r = !! ::StretchBlt(tempDc, 0, 0, cx, cy, dc, rect.left, rect.top, s.cx, s.cy, SRCCOPY);
				bi.ReleaseDC();
				if ( ! r )
				{
					bi.Empty();
				}
			}
			else
			{
				bi.Empty();
			}
		}
		return bi;
	}

	/**
	 * [取得]デバイスコンテキストハンドル取得.
	 *		本インスタンスが保持しているイメージが書かれているデバイスコンテキスト を返します。
	 *		このデバイスコンテキスト を描画すると、本インスタンスが保持している イメージに反映されます。
	 *	@note GetDC() して ReleaseDC() するまで、他のメソッドは失敗します。
	 *	@retval NULL エラー。イメージを保持していません。
	 *	@retval NULL以外 デバイスコンテキスト。描画後、必ず、 本オブジェクトの ReleaseDC() を実行してください。
	 */
	HDC GetDC(void)
	{
		if ( ! IsEmpty() && m_backDC == NULL )
		{
			CWorkDC workDC(m_hWnd);
			m_backDC = ::CreateCompatibleDC(workDC);
			m_backBmp = ::SelectObject(m_backDC, m_bmp);
			::SetStretchBltMode(m_backDC, m_nStretchMode);
			::SetBrushOrgEx(m_backDC, 0, 0, NULL);
			return m_backDC;
		}
		return NULL;
	}

	/**
	 * [設定] デバイスコンテキストハンドル返却.
	 *		GetDC() の後、必ず本メソッドを使用してください。
	 *	@retval true 成功。
	 *	@retval false 失敗。 GetDC() していません。
	 */
	bool ReleaseDC(void)
	{
		if ( m_backDC != NULL )
		{
			m_ReleaseDC();
			return true;
		}
		return false;
	}

	/**
	 * [作成] マスクパターンビットマップ作成.
	 *	@param boIsReverse false なら指定色の部分が 0 それ以外は 1 のモノクロームビットマップが作られます。
	 *						true なら指定色の部分が 1 それ以外は 0 のモノクロームビットマップが作られます。
	 *	@param transColor マスクする色を指定します。省略すると、保持しているイメージの左下の色を指定色とします。
	 *	@retval NULL エラー。
	 *	@retval NULL以外 モノクロームビットマップ
	 */
	HBITMAP CreateMaskBitmap(bool boIsReverse = false, COLORREF transColor = CLR_AUTOSELECT) const
	{
		HRGN rgn = CreateRgn(transColor);
		if ( rgn != NULL ) 
		{
			HBITMAP hMonoBitmap = ::CreateBitmap(m_size.cx, m_size.cy, 1, 1, NULL);
			CTempBitmapDC hdc(m_hWnd, hMonoBitmap);
			RGBQUAD aColors[2] = { {0, 0, 0, 0}, {255, 255, 255, 0} };
			::SetDIBColorTable(hdc, 0, 2, aColors);
			RECT rect = { 0, 0, m_size.cx, m_size.cy };
			if ( ! boIsReverse )
			{
				::FillRect(hdc, &rect, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
				::FillRgn(hdc, rgn, static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
			}
			else
			{
				::FillRect(hdc, &rect, static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
				::FillRgn(hdc, rgn, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
			}
			_DeleteObject(rgn);
			return hMonoBitmap;
		}
		return NULL;
	}

	/**
	 * [作成] 生データ作成.
	 *		本オブジェクトが保持する生データをもつオブジェクトを作成します。
	 *	@return 生データ管理クラス。 本関数がエラーの場合、無効状態の CRawData が返ります。
	 *	@deprecated 廃止予定のメソッドです。
	 */
	CRawData _deprecated CreateRawData(void) const
	{
		if ( ! IsEmpty() && m_backDC == NULL )
		{
			if ( m_pRgb == NULL )
			{
				RGBQUAD* pRgb = NULL;
				CBitmapHandle b = CBitmapHandle::Create32bitDibSection(m_bmp, pRgb);
				if ( pRgb != NULL )
				{
					CRawData data(m_size);
					MemCopy(data.Refer(), pRgb, m_size.cx * m_size.cy);
					return data;
				}
			}
			else
			{
				CRawData data(m_size);
				MemCopy(data.Refer(), m_pRgb, m_size.cx * m_size.cy);
				return data;
			}
		}
		return CRawData();
	}

	/**
	 * [作成] リージョン作成.
	 *		ビットマップ形状に合わせてリージョンを作成する.
	 *	@param transColor 透過色を指定します。 
	 *						省略すると、本オブジェクトが保持しているBITMAPの左下の色を透過色とします。\n
	 *						CLR_INVALID を指定するとビットマップの大きさの四角のリージョンを作成します。
	 *	@retval NULL エラー。
	 *	@retval NULL以外 リージョン。 使用後は ::DeleteObject() で破棄してください。
	 */
	HRGN CreateRgn(COLORREF transColor = CLR_AUTOSELECT) const
	{
		if ( transColor == CLR_INVALID )
		{
			return ::CreateRectRgn(0, 0, m_size.cx, m_size.cy);
		}
		CBitmapImage bi(*this);
		if ( ! bi.m_CheckDib() )
		{
			return NULL; 
		}
		HRGN hRgn = NULL;	// リージョンハンドル
		transColor = (transColor & 0xff00ff00) |
					(transColor & 0x00ff0000) >> 16 |
					(transColor & 0x000000ff) << 16;
		//
		CWorkMem rgbWork(sizeof(RGNDATAHEADER) + sizeof(RECT) * bi.m_size.cx * bi.m_size.cy);
		LPRGNDATA pRgnData = reinterpret_cast<LPRGNDATA>(rgbWork.Ref());
		LPRECT pRect = reinterpret_cast<LPRECT>(pRgnData->Buffer);
		int nCntRect = 0;	// 四角形リージョンの数
		loop_dn ( y, bi.m_size.cy )
		{
			const COLORREF* pScanData = reinterpret_cast<const COLORREF*>(bi.GetRgbDataPtr(y));
			if ( transColor == CLR_AUTOSELECT )
			{
				transColor = pScanData[0];
			}
			// 一行分の透明／非透明の変化点を調査する
			loop ( x, bi.m_size.cx )
			{
				if ( pScanData[x] == transColor )
				{
					continue;
				}
				// その位置から透明までの長さをカウントする
				pRect->left = down_cast<long>(x);
				for ( ; x < static_cast<size_t>(bi.m_size.cx); x++ )
				{
					if ( pScanData[x] == transColor ) { break; }
				}
				// その情報を四角形情報として追加する
				pRect->right	= static_cast<LONG>(x);
				pRect->top		= static_cast<LONG>(y);
				pRect->bottom	= static_cast<LONG>(y + 1);
				pRect++;
				nCntRect++;
			}
		}
		// 書きためた四角形情報を元にリージョンを作成する
		pRgnData->rdh.dwSize			= sizeof(RGNDATAHEADER);
		pRgnData->rdh.iType				= RDH_RECTANGLES;
		pRgnData->rdh.nRgnSize			= sizeof(RGNDATAHEADER) + sizeof(RECT) * nCntRect;
		pRgnData->rdh.nCount			= nCntRect;
		pRgnData->rdh.rcBound.top		= 0;
		pRgnData->rdh.rcBound.left		= 0;
		pRgnData->rdh.rcBound.bottom	= bi.m_size.cy;
		pRgnData->rdh.rcBound.right		= bi.m_size.cx;
		hRgn = ::ExtCreateRegion(NULL, pRgnData->rdh.nRgnSize, pRgnData);
		if ( hRgn == NULL )
		{
			_GetLastError("ExtCreateRegion");
		}
		return hRgn;
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
		HBITMAP h = m_bmp;
		return CBitmapHandle::Draw(hdc, h, x, y);
	}

	/**
	 * [処理] イメージ描画.
	 *		指定のデバイスコンテキストの指定の位置に、本インスタンスで持っているイメージを描画します。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param po 描画位置(左上が0,0)。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Draw(HDC hdc, const POINT& po) const
	{
		return Draw(hdc, po.x, po.y);
	}

	/**
	 * [処理] イメージタイル描画.
	 *		指定のデバイスコンテキストの指定の範囲に、本インスタンスで持っているイメージを敷き詰めます。
	 *	@note 画像は拡大されません。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param rect 描画範囲(左上が0,0)。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool DrawTile(HDC hdc, const RECT& rect) const
	{
		if ( ! IsEmpty() )
		{
			HBRUSH brush = ::CreatePatternBrush(m_bmp);
			bool r = !! ::FillRect(hdc, &rect, brush);
			_DeleteObject(brush);
			return r;
		}
		return false;
	}

	/**
	 * [表示] イメージ描画.
	 *	@note 本インスタンスで保持しているイメージを、指定デバイスコンテキストに描画します。
	 *	@note WIN32API BitBlt を使用しています。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param x 描画X位置(左が0)。
	 *	@param y 描画Y位置(上が0)。
	 *	@param raster オペレーションコード。省略すると、 SRCCOPY になります。\n
	 *			::BitBlt の dwRop のコードを指定します。以下一般的なコードです\n
	 *			<table><tr><td>
	 *			コード								</td><td>
	 *					説明						</td></tr><tr><td>
	 *			BLACKNESS				</td><td>
	 *					物理パレットのインデックス 0 に対応する色（既定の物理パレットでは黒）で、コピー先の長方形を塗りつぶします。 </td></tr><tr><td>
	 *			DSTINVERT				</td><td>
	 *					コピー先長方形の色を反転します。 </td></tr><tr><td>
	 *			MERGECOPY				</td><td>
	 *					論理 AND 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			MERGEPAINT				</td><td>
	 *					論理 OR 演算子を使って、コピー元の色を反転した色と、コピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			NOTSRCCOPY				</td><td>
	 *					コピー元の色を反転して、コピー先へコピーします。 </td></tr><tr><td>
	 *			NOTSRCERASE					</td><td>
	 *					論理 OR 演算子を使って、コピー元の色とコピー先の色を組み合わせ、さらに反転します。 </td></tr><tr><td>
	 *			PATCOPY					</td><td>
	 *					指定したパターンをコピー先へコピーします。 </td></tr><tr><td>
	 *			PATINVERT				</td><td>
	 *					論理 XOR 演算子を使って、指定したパターンの色と、コピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			PATPAINT				</td><td>
	 *					論理 OR 演算子を使って、指定したパターンの色と、コピー元の色を反転した色を組み合わせます。さらに論理 OR 演算子を使って、その結果と、コピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCAND				</td><td>
	 *					論理 AND 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCCOPY					</td><td>
	 *					コピー元長方形をコピー先長方形へそのままコピーします。 </td></tr><tr><td>
	 *			SRCERASE				</td><td>
	 *					論理 AND 演算子を使って、コピー先の色を反転した色と、コピー元の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCINVERT				</td><td>
	 *					論理 XOR 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			SRCPAINT				</td><td>
	 *					論理 OR 演算子を使って、コピー元の色とコピー先の色を組み合わせます。 </td></tr><tr><td>
	 *			WHITENESS				</td><td>
	 *					物理パレットのインデックス 1 に対応する色（既定の物理パレットでは白）で、コピー先の長方形を塗りつぶします。 </td></tr>
	 *		</table>
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool BitBlt(HDC hdc, int x, int y, DWORD raster = SRCCOPY) const
	{
		if ( ! IsEmpty() )
		{
			CTempBitmapDC dc(m_hWnd, m_bmp);
			return !! ::BitBlt(hdc, x, y, m_size.cx, m_size.cy, dc, 0, 0, raster);
		}
		return false;
	}

	/**
	 * [表示] イメージ描画.
	 *	@note 本インスタンスで保持しているイメージを、指定デバイスコンテキストに描画します。
	 *	@note cx , cy を指定すると、これを使うと、拡大縮小して描画することが可能です。また、
	 *			負の数を指定すると、反転して挿入することが出来ます。ただし、挿入座標を軸に反転しますので、
	 *			挿入位置に注意してください。
	 *	@note WIN32API StretchBlt を使用しています。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param x 描画X位置(左が0)。
	 *	@param y 描画Y位置(上が0)。
	 *	@param raster オペレーションコード。省略すると、 SRCCOPY になります。 
	 *					コードは、 BitBlt() のオペレーションコードを参照してください。
	 *	@param cx 描画幅。省略するとイメージ情報のままの幅になります。これを使うと、拡大縮小して表示することが可能です。
	 *	@param cy 描画高さ。省略するとイメージ情報のままの高さになります。これを使うと、拡大縮小して表示することが可能です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool StretchBlt(HDC hdc, int x, int y, DWORD raster = SRCCOPY, int cx = 0, int cy = 0) const
	{
		if ( ! IsEmpty() )
		{
			m_CheckSize(cx, cy);
			CTempBitmapDC dc(m_hWnd, m_bmp);
			return !! ::StretchBlt(hdc, x, y, cx, cy, dc, 0, 0, m_size.cx, m_size.cy, raster);
		}
		return false;
	}

	/**
	 * [表示] 透過処理付イメージ描画.
	 *	@note 本インスタンスで保持しているイメージを、指定のデバイスコンテキストにイメージを表示します。
	 *	@note WIN32API TransparentBlt を使用しています。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param x 描画X座標。
	 *	@param y 描画Y座標。
	 *	@param color 透過させるカラーを指定します。 
	 *					省略すると、本オブジェクトが保持しているビットマップの左下の色を透過色とします。\n
	 *					CLR_INVALID を指定すると透過処理なしで描画します。
	 *	@param cx 表示幅。省略するとイメージ情報のままの幅になります。これを使うと、拡大縮小して表示することが可能です。
	 *	@param cy 表示高さ。省略するとイメージ情報のままの高さになります。これを使うと、拡大縮小して表示することが可能です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool TransparentBit(HDC hdc, int x, int y, COLORREF color = CLR_AUTOSELECT, int cx = 0, int cy = 0) const
	{
		if ( ! IsEmpty() )
		{
			m_CheckSize(cx, cy);
			CTempBitmapDC dc(m_hWnd, m_bmp);
			color = m_CheckColor(dc, color);
			if ( ! IS_RGBVALUE(color) )
			{
				return !! ::StretchBlt(hdc, x, y, cx, cy, dc, 0, 0, m_size.cx, m_size.cy, SRCCOPY);
			}
			return !! ::TransparentBlt(hdc, x, y, cx, cy, dc, 0, 0, m_size.cx, m_size.cy, color);
		}
		return false;
	}

	/**
	 * [描画] 半透明処理付イメージ描画.
	 *	@note 本インスタンスで保持しているイメージを、指定のデバイスコンテキストに描画します。
	 *	@note WIN32API AlphaBlend を使用しています。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param x 描画X座標。
	 *	@param y 描画Y座標。
	 *	@param parsent 描画画像の濃さ(単位;%) 省略なら100% 。
	 *				-1 を指定すると、 保持しているイメージのピクセル毎のアルファチャネルを使用します。
	 *	@param cx 表示幅。省略するとイメージ情報のままの幅になります。これを使うと、拡大縮小して表示することが可能です。
	 *	@param cy 表示高さ。省略するとイメージ情報のままの高さになります。これを使うと、拡大縮小して表示することが可能です。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AlphaBlend(HDC hdc, int x, int y, int parsent = 100, int cx = 0, int cy = 0) const
	{
		if ( ! IsEmpty() )
		{
			m_CheckSize(cx, cy);
			CTempBitmapDC dc(m_hWnd, m_bmp);
			BLENDFUNCTION blend;
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			if ( parsent >= 0 )
			{
				blend.SourceConstantAlpha = static_cast<BYTE>(parsent * 255 / 100);
				blend.AlphaFormat = 0;
			}
			else
			{
				blend.SourceConstantAlpha = 255;
				blend.AlphaFormat = AC_SRC_ALPHA; //アルファチャネル使用
			}
			#if ! defined(_WIN32_WCE) || (_WIN32_WCE > 0x500) 
				return !! ::AlphaBlend(hdc, x, y, cx, cy, dc, 0, 0, m_size.cx, m_size.cy, blend);
			#endif
		}
		return false;
	}

	/**
	 * [挿入] 半透明,透過イメージ描画.
	 *	@note 本インスタンスで保持しているイメージを、指定のデバイスコンテキストに描画します。
	 *	@attention 一ドット毎に計算しているため、処理速度は速くありません。
	 *	@param hdc 描画対象のデバイスコンテキスト。
	 *	@param x 描画X座標。
	 *	@param y 描画Y座標。
	 *	@param parsent 描画画像の濃さ(単位;%) 省略なら100%。
	 *				-1 を指定すると、 保持しているイメージのピクセル毎のアルファチャネルを使用します。
	 *	@param color 透過させるカラーを指定します。 
	 *					省略すると、本オブジェクトが保持しているビットマップの左下の色を透過色とします。\n
	 *					CLR_INVALID を指定すると透過処理なしで描画します。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SemitransparectBlt(HDC hdc, int x, int y, int parsent = 100, COLORREF color = CLR_AUTOSELECT) const
	{
		if ( parsent >= 0 )
		{
			if ( parsent >= 100 )
			{
				return TransparentBit(hdc, x, y, color);
			}
			else if ( color == CLR_INVALID )
			{
				return AlphaBlend(hdc, x, y, parsent);
			}
		}
		//
		CBitmapImage biBase;
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		biBase.SetFromDC(hdc, rc);
		CBitmapImage biOver;
		biOver.Set(*this);
		if ( ! biBase.m_CheckDib() || ! biOver.m_CheckDib() )
		{
			return false;
		}
		if ( parsent >= 0 )
		{
			parsent = parsent * 255 / 100;
		}
		loop ( py, biOver.m_size.cy )
		{
			const RGBQUAD* pOver = biOver.GetRgbDataPtr(py);
			RGBQUAD* pBase = biBase.GetRgbDataPtr(py);
			if ( py == 0 && color == CLR_AUTOSELECT )
			{
				color = RGB(pOver->rgbRed, pOver->rgbGreen, pOver->rgbBlue);
			}
			DWORD tmp;
			loop ( px, biOver.m_size.cx )
			{
				if ( ! IS_RGBVALUE(color) || color != RGB(pOver->rgbRed, pOver->rgbGreen, pOver->rgbBlue) )
				{
					int d = parsent;
					if ( d < 0 )
					{
						d = pOver->rgbReserved;
					}
					tmp = (pBase->rgbBlue * (255 - d)) + (pOver->rgbBlue * d);
					pBase->rgbBlue = static_cast<BYTE>((tmp > 255 * 255) ? 255 : (tmp / 255));
					tmp = (pBase->rgbGreen * (255 - d)) + (pOver->rgbGreen * d);
					pBase->rgbGreen = static_cast<BYTE>((tmp > 255 * 255) ? 255 : (tmp / 255));
					tmp = (pBase->rgbRed * (255 - d)) + (pOver->rgbRed * d);
					pBase->rgbRed = static_cast<BYTE>((tmp > 255 * 255) ? 255 : (tmp / 255));
				}
				pBase++;
				pOver++;
			}
		}
		return biBase.BitBlt(hdc, x, y);
	}

	/**
	 * [確認] RGBデータ有無.
	 *	@retval true 有り
	 *	@retval false なし. ToHaveRgbData() を使って、RGBデータを持たせることが出来ます。
	 */
	bool HasRgbData(void) const
	{
		return m_pRgb != NULL;
	}

	/**
	 * [設定] RGBデータ準備.
	 *	@note RGBデータを持たせることが出来ます。 GetRgbDataPtr() で得たアドレスで、直接、絵を編集できます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool ToHaveRgbData(void)
	{
		if ( m_pRgb == NULL )
		{
			m_CheckDib();
		}
		return m_pRgb != NULL;
	}

	/**
	 * [取得] RGBデータアドレス取得.
	 *		RGBデータのアドレスを得ることで、直接絵を編集することが出来ます。
	 *	@note RGBデータを持っていないと失敗します( ToHaveRgbData() を使って、RGBデータを持たせることが出来ます）。
	 *	@param y 指定の Y のラインのアドレス。 省略(-1)なら、データ全体(画像の縦 - 1 を指定したのと同じアドレス) 。
	 *	@retval NULL 失敗。。
	 *	@retval NULL以外 RGBデータアドレス
	 */
	RGBQUAD* GetRgbDataPtr(INT_PTR y = -1)
	{
		if ( m_pRgb != NULL )
		{
			if ( y < 0 )
			{
				return m_pRgb;
			}
			INT_PTR yy = (m_size.cy - 1) - y;
			return &m_pRgb[yy * m_size.cx];
		}
		return NULL;
	}

	/**
	 * [取得] RGBデータアドレス取得.
	 *		RGBデータのアドレスを得ることで、直接絵を編集することが出来ます。
	 *	@note RGBデータを持っていないと失敗します( ToHaveRgbData() を使って、RGBデータを持たせることが出来ます）。
	 *	@param y 指定の Y のラインのアドレス。 省略(-1)なら、データ全体(画像の縦 - 1 を指定したのと同じアドレス) 。
	 *	@retval NULL 失敗。。
	 *	@retval NULL以外 RGBデータアドレス
	 */
	const RGBQUAD* GetRgbDataPtr(INT_PTR y = -1) const
	{
		if ( m_pRgb != NULL )
		{
			if ( y < 0 )
			{
				return m_pRgb;
			}
			INT_PTR yy = (m_size.cy - 1) - y;
			return &m_pRgb[yy * m_size.cx];
		}
		return NULL;
	}

	/**
	 * [作成] 回転画像作成.
	 *		時計周りに90度傾く画像を作成します。
	 *	@note HasRgbData() で true を返すオブジェクトでないと失敗します。
	 *	@return ビットマップイメージ。 失敗した場合、Empty状態のものが返ります。
	 */
	CBitmapImage CreateRotateImage(void) const
	{
		if ( m_pRgb == NULL )
		{
			return CBitmapHandle();
		}
		const SIZE& sz = m_size;
		CBitmapImage bi;
		bi.Set(sz.cy, sz.cx);
		bi.ToHaveRgbData();
		loop ( y1, sz.cy )
		{
			INDEX x2 = y1;
			const RGBQUAD* p1 = &m_pRgb[y1 * sz.cx];
			loop ( x1, sz.cx )
			{
				INDEX y2 = x1;
				RGBQUAD* p2 = bi.GetRgbDataPtr(y2);
				p2[x2] = *p1++;
			}
		}
		return bi;
	}

	/**
	 * [作成] 反転画像作成.
	 *		反転画像を作成します。
	 *	@note HasRgbData() で true を返すオブジェクトでないと失敗します。
	 *	@param isVertical true なら上下に反転します。
	 *	@param isHorizontal true なら左右に反転します。
	 *	@return ビットマップイメージ。 失敗した場合、Empty状態のものが返ります。
	 */
	CBitmapImage CreateReverseImage(bool isVertical, bool isHorizontal) const
	{
		if ( m_pRgb == NULL )
		{
			return CBitmapHandle();
		}
		const SIZE& sz = m_size;
		CBitmapImage bi;
		bi.Set(sz.cx, sz.cy);
		bi.ToHaveRgbData();
		loop ( y1, sz.cy )
		{
			INDEX y2 = isVertical ? (sz.cy - 1) - y1 : y1;
			const RGBQUAD* p1 = &m_pRgb[y1 * sz.cx];
			if ( isHorizontal )
			{
				loop_dn ( x1, sz.cx )
				{
					bi.m_pRgb[y2 * sz.cx + x1] = *p1++;
				}
			}
			else
			{
				loop ( x1, sz.cx )
				{
					bi.m_pRgb[y2 * sz.cx + x1] = *p1++;
				}
			}
		}
		return bi;
	}

	/**
	 * [作成] グレイスケール画像作成.
	 *		グレイスケール画像を作成します。
	 *	@note HasRgbData() で true を返すオブジェクトでないと失敗します。
	 *	@return ビットマップイメージ。 失敗した場合、Empty状態のものが返ります。
	 */
	CBitmapImage CreateGrayscaleBitmap(void) const
	{
		if ( m_pRgb == NULL )
		{
			return CBitmapHandle();
		}
		const SIZE& sz = m_size;
		CBitmapImage bi;
		bi.Set(sz.cx, sz.cy);
		bi.ToHaveRgbData();
		loop ( y1, sz.cy )
		{
			const RGBQUAD* p = &m_pRgb[y1 * sz.cx];
			loop ( x1, sz.cx )
			{
				BYTE r = p->rgbRed;
				BYTE g = p->rgbGreen;
				BYTE b = p->rgbBlue;
				int a = (r * 30 + g * 59 + b * 11) / 100;
				if ( a > 255 )
				{
					a = 255;
				}
				RGBQUAD q;
				q.rgbRed = static_cast<BYTE>(a);
				q.rgbGreen = static_cast<BYTE>(a);
				q.rgbBlue = static_cast<BYTE>(a);
				bi.m_pRgb[y1 * sz.cx + x1] = q;
				p++;
			}
		}
		return bi;
	}

	/**
	 * [作成] 膨張画像作成.
	 *		膨張画像を作成します。GREEN要素だけみるのでグレイスケールか二値の画像を対象とします。
	 *	@note HasRgbData() で true を返すオブジェクトでないと失敗します。
	 *	@return ビットマップイメージ。 失敗した場合、Empty状態のものが返ります。
	 */
	CBitmapImage CreateDilationBitmap(void) const
	{
		return m_DilationErosion(true);
	}
	
	/**
	 * [作成] 収縮画像作成.
	 *		収縮画像を作成します。GREEN要素だけみるのでグレイスケールか二値の画像を対象とします。
	 *	@note HasRgbData() で true を返すオブジェクトでないと失敗します。
	 *	@return ビットマップイメージ。 失敗した場合、Empty状態のものが返ります。
	 */
	CBitmapImage CreateErosionBitmap(void) const
	{
		return m_DilationErosion(false);
	}

	/**
	 * [作成] メディアンフィルター画像作成.
	 *		メディアンフィルター画像を作成します。
	 *	@note HasRgbData() で true を返すオブジェクトでないと失敗します。
	 *	@return ビットマップイメージ。 失敗した場合、Empty状態のものが返ります。
	 */
	CBitmapImage MedianFilter(void) const
	{
		if ( m_pRgb == NULL )
		{
			return CBitmapHandle();
		}
		CBitmapImage br;
		SIZE sz = m_size;
		br.Set(sz.cx, sz.cy);
		br.ToHaveRgbData();
		BYTE vb[9];
		loop ( y, sz.cy )
		{
			const RGBQUAD* po = GetRgbDataPtr(y);
			const RGBQUAD* po1 = GetRgbDataPtr(y - 1);// y=0の時期待外の値が返るが以降でカバーしてる
			const RGBQUAD* po2 = GetRgbDataPtr(y + 1);// y=sz.cy-1の時期待外の値が返るが以降でカバーしてる
			RGBQUAD* pr = br.GetRgbDataPtr(y);
			loop ( x, sz.cx )
			{
				if ( x == 0 || y == 0 || static_cast<int>(x) == sz.cx - 1 || static_cast<int>(y) == sz.cy - 1 )
				{
					pr[x] = po[x];
				}
				else
				{
					vb[0] = po1[x - 1].rgbBlue;
					vb[1] = po1[x].rgbBlue;
					vb[2] = po1[x + 1].rgbBlue;
					vb[3] = po[x - 1].rgbBlue;
					vb[4] = po[x].rgbBlue;
					vb[5] = po[x + 1].rgbBlue;
					vb[6] = po2[x - 1].rgbBlue;
					vb[7] = po2[x].rgbBlue;
					vb[8] = po2[x + 1].rgbBlue;
					::qsort(vb, 9, sizeof(BYTE), ms_DeSub);
					pr[x].rgbBlue = vb[5];
					//
					vb[0] = po1[x - 1].rgbRed;
					vb[1] = po1[x].rgbRed;
					vb[2] = po1[x + 1].rgbRed;
					vb[3] = po[x - 1].rgbRed;
					vb[4] = po[x].rgbRed;
					vb[5] = po[x + 1].rgbRed;
					vb[6] = po2[x - 1].rgbRed;
					vb[7] = po2[x].rgbRed;
					vb[8] = po2[x + 1].rgbRed;
					::qsort(vb, 9, sizeof(BYTE), ms_DeSub);
					pr[x].rgbRed = vb[5];
					//
					vb[0] = po1[x - 1].rgbGreen;
					vb[1] = po1[x].rgbGreen;
					vb[2] = po1[x + 1].rgbGreen;
					vb[3] = po[x - 1].rgbGreen;
					vb[4] = po[x].rgbGreen;
					vb[5] = po[x + 1].rgbGreen;
					vb[6] = po2[x - 1].rgbGreen;
					vb[7] = po2[x].rgbGreen;
					vb[8] = po2[x + 1].rgbGreen;
					::qsort(vb, 9, sizeof(BYTE), ms_DeSub);
					pr[x].rgbGreen = vb[5];
				}
			}
		}
		return br;
	}

	/**
	 * [描画] ペイント.
	 *	@note HasRgbData() で true を返すオブジェクトでないと失敗します。
	 *	@param x X座標
	 *	@param y Y座標
	 *	@param c カラー
	 *	@retval ペイントしたPIXEL数
	 */
	DWORD Paint(int x, int y, COLORREF c)
	{
		if ( ! HasRgbData() )
		{
			return 0;
		}
		if ( x < 0 || y < 0 || x >= m_size.cx || y >= m_size.cy )
		{
			return 0;
		}
		y = (m_size.cy - 1) - y;
		TPaintInfo pi;
		pi.size = m_size;
		pi.count = 0;
		pi.pRgb = m_pRgb;
		pi.rgbBase = m_pRgb[x + y * m_size.cx];
		pi.rgb.rgbBlue = GetBValue(c);
		pi.rgb.rgbRed = GetRValue(c);
		pi.rgb.rgbGreen = GetGValue(c);
		if ( ! pi.IsEqualBase(pi.rgb) )
		{
			while ( true )
			{
				DWORD i = m_PaintSub(x, y, &pi);
				if ( i == DWORD_MAX ) break;
				x = i & 0xFFFF;
				y = i / 0x10000;
			}
		}
		return pi.count;
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
		return CBitmapHandle::Save(lpszFileName, m_bmp, bitsPixel);
	}


	//-----------------


	/**
	 * [読込] BMPファイル読込み.
	 *	@param lpszFileName ファイル名。
	 *	@param cx 読み込みXサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param cy 読み込みYサイズを指定できます。省略すると、ファイルの情報のままになります。
	 *	@param fuLoad 読込みオプションを指定できます。省略可能。\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL 失	 *	@retval NULL以外 ビットマップハンドル。
	 *	@deprecated CBitmapHandle::Load() を使用してください。
	 */
	static HBITMAP _deprecated Load(LPCTSTR lpszFileName, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		return CBitmapHandle::Load(lpszFileName, cx, cy, fuLoad);
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
	 *	@retval NULL以外 ビットマップハンドル。
	 *	@deprecated CBitmapHandle::Load() を使用してください。
	 */
	static HBITMAP _deprecated Load(UINT uBitmapResourceId, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		return CBitmapHandle::Load(uBitmapResourceId, cx, cy, fuLoad);
	}

	/**
	 * [取得] ビットマップサイズ取得. 
	 *	@param[out] _size サイズ格納。
	 *	@param[in] hBitmap ビットマップハンドル。一時的にしか使用しません。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static bool GetBitmapSize(SIZE& _size, HBITMAP hBitmap)
	{
		BITMAP bm;
		if ( ::GetObject(hBitmap, sizeof(BITMAP), &bm) > 0 )
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
	 * [作成] 画像縮小拡大
	 *	@note DPI にあわせて画像の大きさを調整します。ベース96DPIです。
	 *	@param bmp 元ビットマップ
	 *	@return 変換後のビットマップ
	 */
	static CBitmapHandle ChangeByDpi(CBitmapHandle bmp)
	{
		SIZE orgSize;
		if ( bmp.GetSize(orgSize) )
		{
			SIZE sz = { CalcScaleX(orgSize.cx), CalcScaleY(orgSize.cy) };
			if ( sz.cx != orgSize.cx || sz.cy != orgSize.cy )
			{
				CBitmapImage bi;
				bi.Set(sz.cx, sz.cy);
				bi.Insert(0, 0, bmp, 13369376, sz.cx, sz.cy);
				return bi;
			}
		}
		return bmp;
	}

	/**
	 * [計算] 表示位置調整.
	 * 		指定の大きさのエリアに、比率を変えずに画面の中央に隙間無く配置するための範囲を計算します。
	 *	@param[out] _rect 配置する範囲が格納されます。
	 *	@param[in] dstSize 配置先のサイズ.
	 *	@param[in] srcSize 元の情報のサイズ
	 */
	static void CalcAdjustRect(RECT& _rect, const SIZE& dstSize, const SIZE& srcSize)
	{
		int ly = srcSize.cy * dstSize.cx / srcSize.cx;
		if ( dstSize.cy >= ly )
		{
			int d = (dstSize.cy - ly) / 2;
			::SetRect(&_rect, 0, d, dstSize.cx, ly + d);
			return;
		}
		int lx = srcSize.cx * dstSize.cy / srcSize.cy;
		int d = (dstSize.cx - lx) / 2;
		::SetRect(&_rect, d, 0, lx + d, dstSize.cy);
	}

private:

	CBitmapHandle		m_bmp;			///< ビットマップ
	RGBQUAD*			m_pRgb;			///< DIB アドレス
	SIZE				m_size;			///< サイズ
	HWND				m_hWnd;			///< ワーク用
	HDC					m_backDC;		///< GetDC 用
	HGDIOBJ				m_backBmp;		///< GetDC 用
	int					m_nStretchMode;	///< Stretchモード

	/// ワーク用デバイスコンテキスト管理
	class CWorkDC
	{
		HWND	m_hWnd;
		HDC		m_dc;
	public:
		CWorkDC(HWND hWnd) : m_hWnd(hWnd), m_dc(::GetDC(hWnd))
		{
			ASSERT0( m_dc != NULL, "GetDC", "失敗しました。" );
		}
		~CWorkDC(void)
		{
			if ( ! ::ReleaseDC(m_hWnd, m_dc) ) { ASSERT0( false, "ReleaseDC", "失敗しました。" ); }
		}
		operator HDC(void) { return m_dc; }
	};
	/// テンポラリデバイスコンテキスト管理
	class CTempDC
	{
		HDC		m_dc;
		HGDIOBJ	m_bmp;
	public:
		CTempDC(HDC dc, HBITMAP bmp) : m_dc(::CreateCompatibleDC(dc)), m_bmp(::SelectObject(m_dc, bmp))
		{
			ASSERT0( m_dc != NULL, "CreateCompatibleDC", "失敗しました。" );
			ASSERT0( m_bmp != NULL, "SelectObject", "失敗しました。" );
		}
		~CTempDC(void)
		{
			if ( ::SelectObject(m_dc, m_bmp) == NULL ) { ASSERT0( false, "SelectObject", "失敗しました。" ); }
			if ( ! ::DeleteDC(m_dc) ) { ASSERT0( false, "DeleteDC", "失敗しました。" ); }
		}
		operator HDC(void) { return m_dc; }
	};
	/// テンポラリビットマップデバイスコンテキスト管理
	class CTempBitmapDC
	{
		CWorkDC	m_workDC;
		CTempDC	m_tempDC;
	public:
		CTempBitmapDC(HWND hWnd, HBITMAP bmp) : m_workDC(hWnd), m_tempDC(m_workDC, bmp) { }
		operator HDC(void) { return m_tempDC; }
	};

	/// ペイント情報
	struct TPaintInfo
	{
		DWORD			count;		///< 塗った数
		SIZE			size;		///< ビットマップサイズ
		RGBQUAD*		pRgb;		///< ビットマップ
		RGBQUAD			rgbBase;	///< RGBベース（塗られる色）
		RGBQUAD			rgb;		///< RGB（塗る色）
		/**
		 * [比較] RGBベースと同じ？
		 *	@param q RGB
		 *	@retval true 同じ
		 *	@retval false 異なる
		 */
		bool IsEqualBase(const RGBQUAD& q) const
		{
			if ( rgbBase.rgbBlue != q.rgbBlue ) return false;
			if ( rgbBase.rgbRed != q.rgbRed ) return false;
			if ( rgbBase.rgbGreen != q.rgbGreen ) return false;
			return true;
		}
		/**
		 * [比較] RGBベースと同じ？
		 *	@param x X座標
		 *	@param y Y座標
		 *	@retval true 同じ
		 *	@retval false 異なる
		 */
		bool IsEqualBase(DWORD x, DWORD y) const
		{
			return IsEqualBase(pRgb[x + y * size.cx]);
		}
	};

	#ifndef _WIN32_WCE
		enum { DEFAULT_STRETCHMODE = STRETCH_HALFTONE };
	#else
		enum { DEFAULT_STRETCHMODE = BILINEAR };
	#endif

	//小さいほうを選択
	static size_t ms_ChoiceMin(size_t a, size_t b)
	{
		return (a < b) ? a : b;
	}
	// Bitmapヘッダ設定
	static void ms_SetBitmapInfoHeader(BITMAPINFOHEADER& _bi, int cx, int cy, WORD bit = 32)
	{
		Zero(_bi);
		_bi.biSize			= sizeof(BITMAPINFOHEADER);
		_bi.biWidth			= cx;
		_bi.biHeight		= cy;
		_bi.biPlanes		= 1;
		_bi.biBitCount		= bit;
		_bi.biCompression	= BI_RGB;
	}
	// 空のビットマップ作成
	static HBITMAP ms_CreateBitmap(RGBQUAD*& _pRgb, HDC dc, int cx, int cy)
	{
		_pRgb = NULL;
		if ( cx > 0 && cy > 0 )
		{
			BITMAPINFOHEADER bm = { 0 };
			ms_SetBitmapInfoHeader(bm, cx, cy, 32);
			return ::CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bm), DIB_RGB_COLORS, reinterpret_cast<void **>(&_pRgb), NULL, 0);
		}
		return NULL;
	}
	// RECT → SIZE 
	static bool ms_CheckSize(SIZE& _size, const RECT& rect)
	{
		_size.cx = rect.right - rect.left;
		_size.cy = rect.bottom - rect.top;
		return (_size.cx > 0 && _size.cy > 0);
	}
	static void ms_CheckSize(int& _cx, int& _cy, const SIZE& s)
	{
		if ( _cx <= 0 ) { _cx = s.cx; }
		if ( _cy <= 0 ) { _cy = s.cy; }
	}
	void m_CheckSize(int& _cx, int& _cy) const
	{
		ms_CheckSize(_cx, _cy, m_size);
	}
	// リリース
	void m_ReleaseDC(void)
	{
		if ( m_backDC != NULL )
		{
			if ( ::SelectObject(m_backDC, m_backBmp) == NULL )
			{
				ASSERT0( false, "SelectObject", "失敗しました。" );
			}
			if ( ! ::DeleteDC(m_backDC) )
			{
				ASSERT0( false, "DeleteDC", "失敗しました。" );
			}
			m_backDC = NULL;
		}
	}
	/// 左下のカラー取得
	COLORREF m_CheckColor(HDC hdc, COLORREF color) const
	{
		if ( color == CLR_AUTOSELECT )
		{
			color = ::GetPixel(hdc, 0, m_size.cy - 1);
		}
		return color;
	}
	//メンバに記憶
	bool m_Set(CBitmapHandle bmp, RGBQUAD* pRgb = NULL)
	{
		if ( bmp == m_bmp )
		{
			return true;
		}
		Empty();
		if ( bmp.GetSize(m_size) )
		{
			m_bmp = bmp;
			m_pRgb = pRgb;
			return true;
		}
		return false;
	}

	/// ソート判定関数. @note s_ExpansionContraction() 用
	static int ms_DeSub(const void* a, const void* b)
	{
		return *(BYTE*)a - *(BYTE*)b;
	}

	/// X座標の値取得. @note s_ExpansionContraction() 用
	static inline BYTE ms_GG(const RGBQUAD* p, int cx, int x)
	{
		return (x < 0 || x >= cx) ? static_cast<BYTE>(0) : p[x].rgbGreen;
	}

	/// 最大値取得
	static inline BYTE ms_max(size_t len, BYTE* p)
	{
		BYTE r = p[0];
		loop ( i, len - 1 )
		{
			if ( r < p[i + 1] ) r = p[i + 1];
		}
		return r;
	}

	/// 最小値取得
	static inline BYTE ms_min(size_t len, BYTE* p)
	{
		BYTE r = p[0];
		loop ( i, len - 1 )
		{
			if ( r > p[i + 1] ) r = p[i + 1];
		}
		return r;
	}

	/**
	 * [処理] 膨張/収縮処理.
	 *	@param isDilation trueなら膨張 falseなら収縮.
	 *	@return ビットマップ
	 */
	CBitmapImage m_DilationErosion(bool isDilation = true) const
	{
		if ( m_pRgb == NULL )
		{
			return CBitmapHandle();
		}
		SIZE sz = m_size;
		CBitmapImage b2;
		b2.Set(sz.cx, sz.cy);
		b2.ToHaveRgbData();
		//ダミーライン作成
		CWorkMemT<RGBQUAD> dp(sz.cx);
		RGBQUAD rgb = { 0 };
		dp.Fill(rgb);
		//
		loop ( y, sz.cy )
		{
			RGBQUAD* q = b2.GetRgbDataPtr(y);
			const RGBQUAD* p0 = (y == 0) ? dp : GetRgbDataPtr(y - 1);
			const RGBQUAD* p1 = GetRgbDataPtr(y);
			const RGBQUAD* p2 = (static_cast<int>(y) == sz.cy - 1) ? dp : GetRgbDataPtr(y + 1);
			loop ( x, sz.cx )
			{
				BYTE b[9];
				b[0] = ms_GG(p0, sz.cx, x - 1);
				b[1] = ms_GG(p0, sz.cx, x + 0);
				b[2] = ms_GG(p0, sz.cx, x + 1);
				b[3] = ms_GG(p1, sz.cx, x - 1);
				b[4] = ms_GG(p1, sz.cx, x + 0);
				b[5] = ms_GG(p1, sz.cx, x + 1);
				b[6] = ms_GG(p2, sz.cx, x - 1);
				b[7] = ms_GG(p2, sz.cx, x + 0);
				b[8] = ms_GG(p2, sz.cx, x + 1);
				BYTE r = isDilation ? ms_max(9, b) : ms_min(9, b);
				q[x].rgbGreen = r;
				q[x].rgbBlue = r;
				q[x].rgbRed = r;
			}
		}
		return b2;
	}

	/**
	 * [処理] ペイントサブ.
	 *	@param x X座標
	 *	@param y Y座標
	 *	@param p ペイント情報
	 */	
	DWORD m_PaintSub(int x, int y, TPaintInfo* p)
	{
		if ( ! p->IsEqualBase(x, y) )
		{
			return DWORD_MAX;
		}
		int i, j;
		for ( i = 1;; i++ )
		{
			if ( p->size.cx <= x + i || ! p->IsEqualBase(x + i, y) )
			{
				break;
			}
		}
		for ( j = 1;; j++ )
		{
			if ( 0 > x - j || ! p->IsEqualBase(x - j, y) )
			{
				break;
			}
		}
		RGBQUAD* q = &(p->pRgb[y * p->size.cx]);
		loop ( xx , j + i - 1 )
		{
			q[x - j + 1 + xx] = p->rgb;
		}
		p->count += j + i - 1;
		//
		x = x - j + 1;
		j = j + i - 1;
		DWORD l = 0;
		if ( p->size.cy > y + 1 )
		{
			for( i = 0; i < j;)
			{
				if ( ! p->IsEqualBase(x + i, y + 1) )
				{
					for( ;i < j; i++ )
					{
						if ( p->IsEqualBase(x + i, y + 1) )
							break;
					}
				} 
				else
				{
					for( ; i < j; i++ )
					{
						if ( ! p->IsEqualBase(x + i, y + 1) )
							break;
					}
					l++;
				}
			}
		}
		if ( 0 <= y - 1 )
		{
			for ( i = 0; i < j; )
			{
				if ( ! p->IsEqualBase(x + i, y - 1) )
				{
					for ( ; i < j; i++ )
					{
						if ( p->IsEqualBase(x + i, y - 1) )
							break;
					}
				} 
				else
				{
					for( ; i < j; i++ )
					{
						if ( ! p->IsEqualBase(x + i, y - 1) )
							break;
					}
					l++;
				}
			}
		}
		DWORD k = 0;
		for ( i = 0; i < j; i++ )
		{
			if ( p->size.cy > y + 1 && p->IsEqualBase(x + i, y + 1) )
			{
				if ( ++k >= l ) 
				{
					return (x + i) + (y + 1) * 0x10000;
				}
				int xx = x + i;
				int yy = y + 1;
				while ( true )
				{
					DWORD i = m_PaintSub(xx, yy, p);
					if ( i == DWORD_MAX ) break;
					xx = i & 0xFFFF;
					yy = i / 0x10000;
				}
			}
			if ( 0 <= y - 1 && p->IsEqualBase(x + i, y - 1) )
			{
				if ( ++k >= l )
				{
					return (x + i) + (y - 1) * 0x10000;
				}
				int xx = x + i;
				int yy = y - 1;
				while ( true )
				{
					DWORD i = m_PaintSub(xx, yy, p);
					if ( i == DWORD_MAX ) break;
					xx = i & 0xFFFF;
					yy = i / 0x10000;
				}
			}
		}
		return DWORD_MAX;
	}

	/// m_pRgb が有効化チェックし、NGなら用意する。
	bool m_CheckDib(void)
	{
		if ( ! IsEmpty() && m_pRgb == NULL )
		{
			CBitmapImage bi;
			bi.Set(*this);
			if ( bi.m_pRgb == NULL )
			{
				BITMAP bp;
				if ( ::GetObject(bi.m_bmp, sizeof(BITMAP), &bp) > 0 && bp.bmBitsPixel == 32 )
				{
					bi.m_pRgb = static_cast<RGBQUAD*>(bp.bmBits);
				}
			}
			if ( bi.m_pRgb == NULL )
			{
				bi.Set(*this, m_size.cx, m_size.cy);
				ASSERT( bi.m_pRgb != NULL );
			}
			*this = bi;
		}
		return (m_pRgb != NULL);
	}
};



};///TNB

