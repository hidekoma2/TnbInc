#pragma once
/**
 *@file
 * DirectX関係のヘッダ
 *
 *	DirectX 7 以上の SDK が必須です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"



#ifndef INITGUID
 #define INITGUID
#endif
#include <ddraw.h> // SDK
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")



//TNB Library
namespace TNB{
namespace DX
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
/// PointerHandleBase用 DIRECTDRAWSURFACE7 オブジェクト Releaseクラス
struct TPhReleaseSurface
{
	void operator()(LPDIRECTDRAWSURFACE7 P) { P->Release(); }
};
typedef TNB::CPointerHandleBaseT<LPDIRECTDRAWSURFACE7, TPhReleaseSurface, NULL> CSurfaceHandle;
#endif



/**@ingroup DIRECTX
 * DirectDraw 管理.
 *
 *
 *	@par必要ファイル
 *			TnbDxDraw.h
 *			\n DirectX 7 以上の SDK も必要です。
 *
 *	@date 09/07/23	新規作成
 *	@date 10/02/16	SetOffset() で、X Y 別々の倍率を指定できるようにした。
 */
class CDirectDraw : CCopyImpossible
{
public:

	/**
	 * バッファ管理型.
	 *	@see GetBuffer()
	 */
	struct TBuffer
	{
		SIZE	size;				///< バッファサイズ
		int		pitch;				///< 次の行の先頭までの距離
		int		bitsPixel;			///< 一ピクセルの bit数。 8, 16, 32。
		/// 画像の左上のポインタ
		union
		{
			BYTE*		pB8;	///< bitsPixel が８の時
			WORD*		pB16;	///< bitsPixel が16の時
			RGBTRIPLE*	pB24;	///< bitsPixel が24の時
			RGBQUAD*	pB32;	///< bitsPixel が32の時
		};
		/**
		 * [取得] ポジション計算.
		 *	@param x X座標
		 *	@param y Y座標
		 *	@return ポジション
		 */
		int Pos(int x, int y) const
		{
			return x + y * pitch;
		}
	};

	/**@ingroup DIRECTX
	 * DirectDraw サーフェス管理.
	 *	
	 * 
	 *	@note 複製が可能ですが、コピー先も同じサーフェスを参照するものになります。
	 *
	 *	@par必要ファイル
	 *			TnbDxDraw.h
	 *			\n DirectX 7 以上の SDK も必要です。
	 *
	 *	@date 09/07/23	新規作成
	 */
	class CSurface
	{
	public:

		/// デストラクタ
		~CSurface()
		{
			m_ReleaseRent();
		}

		/// コンストラクタ.
		CSurface(void) : m_isRentBuffer(false), m_hDC(NULL)
		{
			m_CheckSpec();
		}

		/**
		 * コンストラクタ.
		 *	@note 作成したサーフェスを指定することで,本クラスで管理することが出来ます。
		 *	@param pSurface サーフェスポインタ。このサーフェスは本クラスが管理するので、 Releaseしないでください。
		 */
		CSurface(LPDIRECTDRAWSURFACE7 pSurface) : m_hpSurface(pSurface)
		{
			m_CheckSpec();
		}

		/**
		 * コピーコンストラクタ.
		 *	@attention const オブジェクトはコピー元に使えません。
		 *	@param other コピー元
		 */
		CSurface(CSurface& other)
		{
			operator=(other);
		}

		/**
		 * コピーオペレータ.
		 *	@attention const オブジェクトはコピー元に使えません。
		 *	@param other コピー元
		 *	@return 自分の参照.
		 */
		CSurface& operator=(CSurface& other)
		{
			other.m_ReleaseRent();
			m_hpSurface = other.m_hpSurface;
			m_primarySize = other.m_primarySize;
			m_bitsPixel = other.m_bitsPixel;
			m_isRentBuffer = false;
			m_hDC = NULL;
			return *this;
		}

		/**
		 * [登録] アタッチ.
		 *	@note 先に保持しているサーフェスがあれば、破棄してからアタッチします。
		 *	@note 作成したサーフェスを指定することで,本クラスで管理することが出来ます。
		 *	@param pSurface サーフェスポインタ。このサーフェスは本クラスが管理するので、 Releaseしないでください。
		 */
		void Attach(LPDIRECTDRAWSURFACE7 pSurface)
		{
			m_ReleaseRent();
			m_hpSurface = pSurface;
			m_CheckSpec();
		}

		/**
		 * [作成] 作成.
		 *	@note 先に保持しているサーフェスがあれば、破棄してから作成します。
		 *	@param pDraw 有効な DirectDraw のポインタ
		 *	@param pDdsd サーフェス作成用情報
		 *	@retval DD_OK 成功。作成したサーフェスを保持します。
		 *	@retval 上記以外 失敗。
		 */
		HRESULT Create(IDirectDraw7* pDraw, LPDDSURFACEDESC2 pDdsd)
		{
			Destroy();
			LPDIRECTDRAWSURFACE7 P;
			HRESULT r = pDraw->CreateSurface(pDdsd, &P, NULL);
			if ( r == DD_OK )
			{
				m_hpSurface = P;
				m_CheckSpec();
			}
			return r;
		}

		/**
		 * [破棄] 破棄.
		 *		保持しているサーフェスを破棄します。
		 *		保持していなければ何もしません。　
		 */
		void Destroy(void)
		{
			m_ReleaseRent();
			m_hpSurface = NULL;
		}

		/**
		 * [確認] 有効確認.
		 *	@retval true 有効。サーフェスを保持しています。
		 *	@retval false 無効。サーフェスを保持していません。
		 */
		bool IsValid(void) const
		{
			return ! m_hpSurface.IsNull();
		}

		/**
		 * [取得] サイズ取得.
		 *		保持しているサーフェスのサイズを返します。
		 *	@return サイズ
		 */
		const SIZE& GetSize(void) const
		{
			return m_primarySize;
		}

		/**
		 * [参照] 参照.
		 *	@note DirectDraw の API の引数に渡すためにサーフェスのポインタを返します。
		 *	@return サーフェスポインタ
		 */
		operator LPDIRECTDRAWSURFACE7(void)
		{
			DEBUG_ONLY( m_ReleaseRent() );
			return m_hpSurface;
		}

		/**
		 * [参照] 参照.
		 *	@note DirectDraw の IDirectDrawSurface のメソッドを実行するためにサーフェスのポインタを返します。
		 *	@return サーフェスポインタ
		 */
		LPDIRECTDRAWSURFACE7 operator->(void)
		{
			DEBUG_ONLY( m_ReleaseRent() );
			return m_hpSurface;
		}

		/**
		 * [描画] BltFast.
		 *	@param x		転送先X座標
		 *	@param y		転送先Y座標 
		 *	@param lpDdsSrc	転送元サーフェス
		 *	@param srcRect	転送元の範囲
		 *	@param dwTrans	転送フラグ
		 *	@retval DD_OK 成功。
		 *	@retval 上記以外 失敗。
		 */
		HRESULT BltFast(int x, int y, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			CRect rc = srcRect;
			HRESULT hr = m_hpSurface->BltFast(x, y, lpDdsSrc, &rc, DDBLTFAST_WAIT | dwTrans);
			if ( hr == DDERR_SURFACELOST )
			{
				m_hpSurface->Restore();
				hr = m_hpSurface->BltFast(x, y, lpDdsSrc, &rc, DDBLTFAST_WAIT | dwTrans);
			}
			return hr;
		}

		/**
		 * [描画] クリッピング BltFast.
		 *		クリッピング処理付きの BltFase です。
		 *	@param x		転送先X座標
		 *	@param y		転送先Y座標 
		 *	@param lpDdsSrc	転送元サーフェス
		 *	@param srcRect	転送元の範囲
		 *	@param dwTrans	転送フラグ
		 *	@retval DD_OK 成功。
		 *	@retval 上記以外 失敗。
		 */
		HRESULT BltFastClip(int x, int y, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			RECT rc = srcRect;
			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;
			// 指定範囲を超えた画像を切り取る
			if ( x < 0 )
			{
				rc.left += (-x);
				x = 0;
			}
			else if ( m_primarySize.cx < x + width )
			{
				rc.right -= (x + width - m_primarySize.cx);
			}
			if ( y < 0)
			{
				rc.top += (-y);
				y = 0;
			}
			else if ( m_primarySize.cy < y + height)
			{
				rc.bottom -= (y + height - m_primarySize.cy);
			}
			//描画
			return BltFast(x, y, lpDdsSrc, rc, dwTrans);
		}

		/**
		 * [描画] Blt.
		 *	@param dstRect	転送先範囲
		 *	@param lpDdsSrc	転送元サーフェス
		 *	@param srcRect	転送元の範囲
		 *	@param dwTrans	転送フラグ
		 *	@retval DD_OK 成功。
		 *	@retval 上記以外 失敗。
		 */
		HRESULT Blt(const RECT& dstRect, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			CRect rc1 = dstRect;
			CRect rc2 = srcRect;
			HRESULT hr = m_hpSurface->Blt(&rc1, lpDdsSrc, &rc2, DDBLT_WAIT | dwTrans, NULL);
			if ( hr == DDERR_SURFACELOST )
			{
				m_hpSurface->Restore();
				hr = m_hpSurface->Blt(&rc1, lpDdsSrc, &rc2, DDBLT_WAIT | dwTrans, NULL);
			}
			return hr;
		}

		/**
		 * [描画] クリッピング Blt.
		 *		クリッピング処理付きの Blt です。
		 *	@param dstRect	転送先範囲
		 *	@param lpDdsSrc	転送元サーフェス
		 *	@param srcRect	転送元の範囲
		 *	@param dwTrans	転送フラグ
		 *	@retval DD_OK 成功。
		 *	@retval 上記以外 失敗。
		 */
		HRESULT BltClip(const RECT& dstRect, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			RECT dstRc = dstRect;
			RECT srcRc = srcRect;
			SIZE dstSz = { dstRc.right - dstRc.left, dstRc.bottom - dstRc.top };
			SIZE srcSz = { srcRc.right - srcRc.left, srcRc.bottom - srcRc.top };
			const LONG par = 1024;
			LONG parHeight = dstSz.cx * par / srcSz.cx;
			LONG parWidth = dstSz.cy * par / srcSz.cy;
			// 指定範囲を超えた画像を切り取る
			int w1 = dstRc.right - m_clipRect.right;
			if ( w1 > 0 )
			{
				srcRc.right -= w1 * par / parWidth;
				dstRc.right -= w1;
			}
			int w2 = dstRc.left - m_clipRect.left;
			if ( w2 < 0)
			{
				srcRc.left -= w2 * par / parWidth;
				dstRc.left -= w2;
			}
			int h1 = dstRc.bottom - m_clipRect.bottom;
			if ( h1 > 0 )
			{
				srcRc.bottom -= h1 * par / parHeight;
				dstRc.bottom -= h1;
			}
			int h2 = dstRc.top - m_clipRect.top;
			if ( h2 < 0)
			{
				srcRc.top -= h2 * par / parHeight;
				dstRc.top -= h2;
			}
			//
			return Blt(dstRc, lpDdsSrc, srcRc, dwTrans);
		}

		/**
		 * [取得] HDC 取得.
		 *		サーフェスの HDC を取得できます。
		 *	@note  ReleaseBuffer() も実行されますので、 GetBuffer() の戻り値が無効になります。
		 *	@attention GetDC() と ReleaseDC() の間は極力処理を短くなるように心がけてください。
		 *	@retval NULL 失敗。先の GetDC() 後、 ReleaseDC() していない場合も失敗します。
		 *	@retval NULL以外 成功。値は HDC。使用後、破棄せず、 ReleaseDC() をコールしてください。
		 */
		HDC GetDC(void)
		{
			if ( IsValid() && m_hDC == NULL )
			{
				ReleaseBuffer();
				if ( m_hpSurface->GetDC(&m_hDC) == DD_OK )
				{
					return m_hDC;
				}
			}
			return NULL;
		}

		/**
		 * [取得] HDC 返却.
		 *		GetDC() で得た HDC は使えなくなります。
		 */
		void ReleaseDC(void)
		{
			if ( IsValid() && m_hDC != NULL )
			{
				m_hpSurface->ReleaseDC(m_hDC);
				m_hDC = NULL;
			}
		}

		/**
		 * [取得] バッファ取得.
		 *		サーフェスのバッファを取得できます。
		 *	@note  ReleaseDC() も実行されますので、 GetDC() で得た HDC は無効になります。
		 *	@attention GetBuffer() と ReleaseBuffer() の間は極力処理を短くなるように心がけてください。
		 *	@retval NULL 失敗。先の GetBuffer() 後、 ReleaseDC() していない場合も失敗します。
		 *	@retval NULL以外 成功。値はバッファ管理のポインタです。使用後、破棄せず、 ReleaseBuffer() をコールしてください。
		 */
		const TBuffer* GetBuffer(void)
		{
			if ( IsValid() && ! m_isRentBuffer )
			{
				ReleaseDC();
				DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
				if ( m_hpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK )
				{
					m_buffer.size = m_primarySize;
					m_buffer.pitch = ddsd.lPitch / m_bitsPixel * 8;
					m_buffer.bitsPixel = m_bitsPixel;
					m_buffer.pB8 = static_cast<BYTE*>(ddsd.lpSurface);
					m_isRentBuffer = true;
					return &m_buffer;
				}
			}
			return NULL;
		}

		/**
		 * [取得] バッファ返却.
		 *		GetBuffer() で得たバッファ情報のポインタは使えなくなります。
		 */
		void ReleaseBuffer(void)
		{
			if ( IsValid() && m_isRentBuffer )
			{
				m_hpSurface->Unlock(NULL);
				m_isRentBuffer = false;
			}
		}

		/**
		 * [設定] クリッピング範囲設定.
		 *		クリッピング範囲を設定します。
		 *	@note 通常使いません。
		 *	@param rect 範囲
		 */
		void SetClipRect(const RECT& rect)
		{
			m_clipRect = rect;
		}

	private:

		/// サーフェススペック取得
		void m_CheckSpec(void)
		{
			if ( IsValid() )
			{
				DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
				DDPIXELFORMAT ddpf = { sizeof(DDPIXELFORMAT) };
				if ( m_hpSurface->GetSurfaceDesc(&ddsd) == DD_OK && m_hpSurface->GetPixelFormat(&ddpf) == DD_OK )
				{
					DWORD f = DDSD_WIDTH | DDSD_HEIGHT;
					if ( (ddsd.dwFlags & f) == f )
					{
						m_primarySize.cx = ddsd.dwWidth;
						m_primarySize.cy = ddsd.dwHeight;
						::SetRect(&m_clipRect, 0, 0, m_primarySize.cx, m_primarySize.cy);
						m_bitsPixel = ddpf.dwRGBBitCount;
						return;
					}
				}
			}
			m_primarySize.cx = 0;
			m_primarySize.cy = 0;
			::SetRectEmpty(&m_clipRect);
			m_bitsPixel = 0;
		}
		/// 貸し出しものを開放
		void m_ReleaseRent(void)
		{
			ReleaseDC();
			ReleaseBuffer();
		}
		//
		CSurfaceHandle	m_hpSurface;	///< オブジェクト
		SIZE			m_primarySize;			///< サイズ
		int				m_bitsPixel;	///< ピクセルビット数
		//
		RECT			m_clipRect;		///< クリップ範囲
		//
		bool			m_isRentBuffer;	///< 貸し出し中フラグ
		HDC				m_hDC;			///< DC ハンドル
		TBuffer			m_buffer;		///< GetBuffer用リザルト
	};


	//------------------------
		
	
	/// コンストラクタ
	CDirectDraw(void) : m_pDraw(NULL), m_isFull(false), m_pClip(NULL), m_hDC(NULL), m_pGetBuffSurface(NULL)
	{
		m_primarySize.cx = 0;
		m_primarySize.cy = 0;
		m_backSize.cx = 0;
		m_backSize.cy = 0;
		m_offset.x = 0;
		m_offset.y = 0;
		m_magnific.x = 1;
		m_magnific.y = 1;
	}

	/// デストラクタ
	~CDirectDraw(void)
	{
		Destroy();
	}

	/**
	 * [設定] フルスクリーンモード設定.
	 *		フルスクリーンモードとウィンドウモードを切替えます。
	 *	@note 初期状態ではウィンドウモードです。
	 *	@note Create() 前でも後でも機能します。
	 *	@param isFull true を指定するとフルスクリーンモード。 false を指定するとウィンドウモードになります。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetFullScreenMode(bool isFull)
	{
		if ( m_isFull != isFull )
		{
			if ( m_pDraw != NULL )
			{
				m_DestroySurface();
				m_isFull = isFull;
				return m_CreateSurface();
			}
			m_isFull = isFull;
		}
		return true;
	}

	/**
	 * [設定] サイズ設定.
	 *		プライマリサーフェスとバックバッファサーフェスのサイズを設定します。
	 *	@note Create() 後でも機能します。
	 *	@param size プライマリサーフェスとバックバッファサーフェスのサイズ。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetSize(const SIZE& size)
	{
		return SetSize(size, size);
	}

	/**
	 * [設定] サイズ設定.
	 *		プライマリサーフェスとバックバッファサーフェスのサイズを設定します。
	 *	@note Create() 後でも機能します。
	 *	@param primarySize プライマリサーフェスのサイズ
	 *	@param backSize バックバッファサーフェスのサイズ。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetSize(const SIZE& primarySize, const SIZE& backSize)
	{
		m_primarySize = primarySize;
		m_backSize = backSize;
		if ( m_pDraw != NULL )
		{
			m_DestroySurface();
			return m_CreateSurface();
		}
		return true;
	}

	/**
	 * [作成] 作成.
	 *		DirectDraw を作成します。
	 *	@note SetSize() にて、先にサイズを指定しておく必要があります。
	 *	@param hWnd 関連するウィンドウハンドル.
	 *	@param bitsPixel １ピクセルのビット数。省略すると、現在のディスプレイの設定が使われます( GetBitsPixel() で得ることが可能 )。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Create(HWND hWnd, int bitsPixel = -1)
	{
		Destroy();
		m_hWnd = hWnd;
		if ( bitsPixel < 0 )
		{
			DEVMODE dm = { sizeof(DEVMODE) };
			if ( ! ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
			{
				return false;
			}
			bitsPixel = dm.dmBitsPerPel;
		}
		m_bitsPixel = bitsPixel;
		C_ASSERT( DD_OK == 0 );
		if ( ::DirectDrawCreateEx(NULL, reinterpret_cast<void**>(&m_pDraw), IID_IDirectDraw7, NULL) != DD_OK )
		{
			return false;
		}
		if ( ! m_CreateSurface() )
		{
			Destroy();
			return false;
		}
		return true;
	}

	/**
	 * [破棄] 破棄.
	 *		DirectDraw オブジェクトを破棄します。
	 *	@note 作成していなければ何もしません。
	 */
	void Destroy(void)
	{
		if ( m_backbufferSurface.IsValid() && m_hDC != NULL )
		{
			m_backbufferSurface->ReleaseDC(m_hDC);
			m_hDC = NULL;
		}
		m_DestroySurface();
		m_Release(m_pDraw);
	}

	/**
	 * [取得] プライマリサーフェスのサイズ取得.
	 *	@return サイズ
	 */
	const SIZE& GetPrimarySize(void) const
	{
		return m_primarySize;
	}

	/**
	 * [取得] バックバッファサーフェスのサイズ取得.
	 *	@return サイズ
	 */
	const SIZE& GetBackbufferSize(void) const
	{
		return m_backSize;
	}

	/**
	 * [取得] ピクセル BIT 数取得.
	 *	@note Create() 前は、システムの値、 Create() 後は、 DirectDraw から値を得ています。
	 *	@retval マイナス エラー。
	 *	@retval ０以上 成功。値は、１ピクセルの BIT 数です。
	 */
	int GetBitsPixel(void) const
	{
		if ( m_pDraw != NULL )
		{
			DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
			if ( m_pDraw->GetDisplayMode(&ddsd) == DD_OK )
			{
				return (ddsd.lPitch / ddsd.dwWidth) * 8;
			}
		}
		else
		{
			DEVMODE dm = { sizeof(DEVMODE) };
			if ( ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
			{
				return dm.dmBitsPerPel;
			}
		}
		return -1;
	}

	/**
	 * [設定] オフセット設定.
	 *		ウィンドウモード時に、表示するオフセットを指定できます。
	 *	@note オフセット設定後、再表示します。
	 *	@param x X オフセット。
	 *	@param y Y オフセット。
	 *	@param magnificX X 倍率。
	 *	@param magnificY Y 倍率。
	 */
	void SetOffset(int x, int y, int magnificX, int magnificY)
	{
		m_offset.x = x;
		m_offset.y = y;
		m_magnific.x = magnificX;
		if ( m_magnific.x < 1 )
		{
			m_magnific.x = 1;
		}
		m_magnific.y = magnificY;
		if ( m_magnific.y < 1 )
		{
			m_magnific.y = 1;
		}
		Redraw();
	}

	/**
	 * [設定] オフセット設定.
	 *		ウィンドウモード時に、表示するオフセットを指定できます。
	 *	@note オフセット設定後、再表示します。
	 *	@param x X オフセット。
	 *	@param y Y オフセット。
	 *	@param magnific 倍率。
	 */
	void SetOffset(int x = 0, int y = 0, int magnific = 1)
	{
		m_offset.x = x;
		m_offset.y = y;
		if ( magnific < 1 )
		{
			magnific = 1;
		}
		m_magnific.x = magnific;
		m_magnific.y = magnific;
		Redraw();
	}

	/**
	 * [表示] 再表示.
	 *	@note ウィンドウモードの場合のみ、再表示を行います。
	 *		通常、 WM_PAINT 時にコールします。
	 *	@note フルスクリーンモードの場合、何もしません。
	 *	@retval true 成功
	 *	@retval false 失敗。フルスクリーンモードの場合も false が返ります。
	 */
	bool Redraw(void)
	{
		if ( m_primarySurface.IsValid() && ! m_isFull )
		{
			POINT po = { 0, 0 };
			::ClientToScreen(m_hWnd, &po);
			RECT r2 = { 0, 0, m_primarySize.cx, m_primarySize.cy };
			RECT r1 = r2;
			::OffsetRect(&r1, po.x, po.y);
			r2.right /= m_magnific.x;
			r2.bottom /= m_magnific.y;
			::OffsetRect(&r2, m_offset.x, m_offset.y);
			m_pDraw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0); //同期待ち
//			return m_primarySurface->Blt(&r1, m_benchSurface, &r2, DDBLT_WAIT, NULL) == DD_OK; //こっちのほうが早いが画面外にいくと、おかしなところに表示されてしまう
			return m_primarySurface.BltClip(r1, m_benchSurface, r2) == DD_OK;			
		}
		return false;
	}

	/**
	 * [処理] フィリップ.
	 *	@note フルスクリーンモードとウィンドウモードで動作が異なります。\n
	 *		フルスクリーンモードの場合、バックサーフェスとプライマリサーフェスを入れ替えます。\n
	 *		ウィンドウモードの場合、バックサーフェスの画像をプライマリサーフェスにコピーします。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Flip(void)
	{
		bool r = false;
		if ( m_primarySurface.IsValid() )
		{
			if ( m_isFull )
			{
				//== フルモード
				r = m_primarySurface->Flip(0, DDFLIP_WAIT) == DD_OK;
			}
			else
			{
				//== ウィンモード
				r = m_benchSurface->BltFast(0, 0, m_backbufferSurface, NULL, DDBLTFAST_WAIT) == DD_OK;
				if ( r )
				{
					r = Redraw();
				}
			}
		}
		return r;
	}

	/**
	 * [処理] 垂直同期待ち.
	 *
	 */
	void WaitForVerticalBlank(void)
	{
		if ( m_pDraw != NULL )
		{
			m_pDraw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0); //同期待ち
		}
	}

	/**
	 * [描画] バックバッファクリア
	 *	@note 真っ黒にします。
	 *	@retval true 成功
	 *	@retval false 失敗
 	 */
	bool ClearBackbufferSurface(void)
	{
		if ( m_primarySurface.IsValid() )
		{
			return m_backbufferSurface->BltFast(0, 0, m_baseSurface, NULL, DDBLTFAST_WAIT) == DD_OK;
		}
		return false;
	}

	/**
	 * [取得] バックバッファ取得.
	 *	@return バックバッファの参照.
	 */
	CSurface& GetBackbufferSurface(void)
	{
		return m_backbufferSurface;
	}

	CSurface CreateWorkSurfase(int height, int width)
	{
		CSurface surf;
		DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT ;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY; 
		ddsd.dwWidth = (height > 0) ? height : m_backSize.cx;
		ddsd.dwHeight = (width > 0) ? width : m_backSize.cy;
		surf.Create(m_pDraw, &ddsd);
		return surf;
	}

private:

	/// 全サーフェス破棄
	void m_DestroySurface(void)
	{
		m_Release(m_baseSurface);
		m_Release(m_backbufferSurface);
		m_Release(m_benchSurface);
		m_Release(m_primarySurface);
		m_Release(m_pClip);
	}

	// サーフェス作成
	bool m_CreateSurface(void)
	{
		m_DestroySurface();
		ASSERT( m_pDraw != NULL );
		DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
		HRESULT res = DD_OK;
		if ( m_isFull )
		{
			//== フル
			//協調レベル設定
			res |= m_pDraw->SetCooperativeLevel(m_hWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
			//画面設定
			res |= m_pDraw->SetDisplayMode(m_primarySize.cx, m_primarySize.cy, m_bitsPixel, 0, 0);
			//プライマリサーフェイス作成
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps =  DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
			ddsd.dwBackBufferCount = 1;
			res |= m_primarySurface.Create(m_pDraw, &ddsd);
			if ( res == DD_OK )
			{
				//バックバッファサーフェス作成
				DDSCAPS2 ddscaps = { 0 };
				ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
				LPDIRECTDRAWSURFACE7 pSur = NULL;
				res |= m_primarySurface->GetAttachedSurface(&ddscaps, &pSur);
				m_backbufferSurface.Attach(pSur);
			}
			ddsd.dwWidth = m_primarySize.cx;
			ddsd.dwHeight = m_primarySize.cy;
		}
		else
		{
			//== ウィン
			//協調レベル設定
			m_pDraw->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
			//画面設定
			DEVMODE dm = { sizeof(DEVMODE) };
			if ( ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
			{
				dm.dmBitsPerPel = m_bitsPixel;
				dm.dmFields = DM_BITSPERPEL;
				if ( ::ChangeDisplaySettings(&dm, CDS_FULLSCREEN/*一時的の意味で使う*/) != DISP_CHANGE_SUCCESSFUL )
				{
					res = DD_FALSE;
				}
			}
			else
			{
				res = DD_FALSE;
			}
			//クリッパー作成
			res |= m_pDraw->CreateClipper(0, &m_pClip, NULL);
			if ( res == DD_OK )
			{
				res |= m_pClip->SetHWnd(0, m_hWnd);
				//プライマリサーフェスの作成
				ddsd.dwFlags = DDSD_CAPS;
				ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
				res |= m_primarySurface.Create(m_pDraw, &ddsd);
			}
			if ( res == DD_OK )
			{
				res |= m_primarySurface->SetClipper(m_pClip);
				//クリッピング枠更新
				RECT nowClipRect;
				::GetClipCursor(&nowClipRect);
				::ClipCursor(NULL);
				RECT allClipRect;
				::GetClipCursor(&allClipRect);
				::ClipCursor(&nowClipRect);
				m_primarySurface.SetClipRect(allClipRect);
				//バックサーフェスの作成
				ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
				ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
				ddsd.dwWidth = m_backSize.cx;
				ddsd.dwHeight = m_backSize.cy;
				res |= m_backbufferSurface.Create(m_pDraw, &ddsd);
				//ベンチサーフェスの作成
				res |= m_benchSurface.Create(m_pDraw, &ddsd);
			}
		}
		//
		if ( res == DD_OK )
		{
			//ベースサーフェス
			ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY; 
			res |= m_baseSurface.Create(m_pDraw, &ddsd);
		}
		//
		if ( res != DD_OK )
		{
			m_DestroySurface();
			return false;
		}
		return true;
	}

	/// ポインタリリース
	template<typename TYP>
	void m_Release(TYP*& P)
	{
		if ( P != NULL )
		{
			P->Release();
			P = NULL;
		}
	}

	/// ポインタリリース
	void m_Release(CSurface& P)
	{
		P.Destroy();
	}

	HWND					m_hWnd;					///< ウィンドウハンドル
	SIZE					m_primarySize;			///< プライマリサイズ
	SIZE					m_backSize;				///< バックバッファ等サイズ
	int						m_bitsPixel;			///< 色深さ
	bool					m_isFull;				///< フルスクリーンか？
	POINT					m_offset;				///< ウィンドウモード用(オフセット)
	POINT					m_magnific;				///< ウィンドウモード用(倍率)
	LPDIRECTDRAW7			m_pDraw;				///< DirectDraw
	CSurface				m_primarySurface;		///< プライマリサーフェス
	CSurface				m_backbufferSurface;	///< バックバッファサーフェス(編集用)
	CSurface				m_benchSurface;			///< ベンチサーフェス(ウィンドウモード用)
													///< @note ウィンドウの場合、再描画が発生するのでプライマリに表示している画像と同じものを、
													///<	このベンチに入れておくようにしている。
	CSurface				m_baseSurface;			///< ベースサーフェス(クリア用)
													///< @note Flipした後、バックバッファをクリアするために使用する。
													///<	通常、真っ黒。
	LPDIRECTDRAWCLIPPER		m_pClip;				///< クリッパー
	//
	HDC						m_hDC;					///< NULLならGetDCは可能
	TBuffer					m_buffer;				///< GetBuffer用リザルト
	LPDIRECTDRAWSURFACE7	m_pGetBuffSurface;		///< 貸し出し中のサーフェス
};



};//DX
};//TNB


/**
  
	x2　以上の時、画面外に表示エリアがあると、おかしくなる。


 */


/*
	画面として、今、こんな感じ。
	
	表示している、「プライマリサーフェス」
	編集用の、「バックバッファサーフェス」
	ウィンドウモード専用に、表示内容の控え、「ベンチサーフェス」


	バックバッファをクリアするための、「ベースサーフェス」

	矩形パターンをおいておく、「パターンサーフェス」　数枚？

	背景パターンを並べる、「バックグラウンドサーフェス」　数枚？
		スクロールに対応？！






	ウィンドウモードの場合、オフセット表示が可能。



*/
