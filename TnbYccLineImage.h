#pragma once
/**
 *@file
 * YCbCr関係のヘッダ
 *
 *	YCbCr の画像を扱うためのサポートクラスがあります。
 *
 *		現在 YCbCr 4:2:2 (8bit) の管理クラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #define _MUL 0x20000 //(131072)
 #define _(X) (int(X * _MUL))
#endif



/**@ingroup BITMAP
 * YCbCrラインイメージ管理.
 *		YCbCr 4:2:2 の画像の一ラインを扱うためのサポートクラスです。
 *
 *	@par必要ファイル
 *			TnbYccLineImage.h
 *
 *	@date 09/07/28	新規作成
 */
class CYcc422LineImage
{
public:

	/**
	 * ペアピクセル情報.
	 *		2ピクセルセットで管理します。
	 *	
	 *	@par必要ファイル
	 *			TnbYccLineImage.h
	 *
	 *	@date 09/07/28	新規作成
	 */
	class CPairPixel
	{
	public:

		/**
		 * コンストラクタ.
		 *		全要素０のインスタンスになります。
		 */
		CPairPixel(void)
		{
			Zero(*this);
		}

		/**
		 * コンストラクタ.
		 *	@param color1 左のピクセル色
		 *	@param color2 右のピクセル色
		 */
		CPairPixel(COLORREF color1, COLORREF color2)
		{
			SetRgb(color1, color2);
		}

		/**
		 * コンストラクタ.
		 *	@param rgb1 左のピクセル色
		 *	@param rgb2 右のピクセル色
		 */
		CPairPixel(const RGBQUAD& rgb1, const RGBQUAD& rgb2)
		{
			SetRgb(rgb1, rgb2);
		}

		/**
		 * [設定] Cb「色差(青)」設定.
		 *	@param cb Cb 
		 */
		void SetCb(BYTE cb)
		{
			m_cb = cb;
		}

		/**
		 * [取得] Cb「色差(青)」取得.
		 *	@return Cb
		 */
		BYTE GetCb(void) const
		{
			return m_cb;
		}

		/**
		 * [設定] Y1「輝度(左)」設定.
		 *	@param y1 Y1
		 */
		void SetY1(BYTE y1)
		{
			m_y1 = y1;
		}

		/**
		 * [取得] Y1「輝度(左)」取得.
		 *	@return Y1
		 */
		BYTE GetY1(void) const
		{
			return m_y1;
		}

		/**
		 * [設定] Cr「色差(赤)」設定.
		 *	@param cr Cr
		 */
		void SetCr(BYTE cr)
		{
			m_cr = cr;
		}

		/**
		 * [取得] Cr「色差(赤)」取得.
		 *	@return Cr
		 */
		BYTE GetCr(void) const
		{
			return m_cr;
		}

		/**
		 * [設定] Y2「輝度(右)」設定.
		 *	@param y2 Y2
		 */
		void SetY2(BYTE y2)
		{
			m_y2 = y2;
		}

		/**
		 * [取得] Y2「輝度(右)」取得.
		 *	@return Y2
		 */
		BYTE GetY2(void) const
		{
			return m_y2;
		}

		/**
		 * [設定] Y Cb Cr 設定.
		 *	@param y1 Y1
		 *	@param y2 Y2
		 *	@param cb Cb
		 *	@param cr Cr
		 */
		void SetYcc(BYTE y1, BYTE y2, BYTE cb, BYTE cr)
		{
			m_y1 = y1;
			m_y2 = y2;
			m_cb = cb;
			m_cr = cr;
		}

		/**
		 * [取得] RGB取得.
		 *	@param[out] _rgb1 左のピクセル色
		 *	@param[out] _rgb2 右のピクセル色
		 */
		void GetRgb(RGBQUAD& _rgb1, RGBQUAD& _rgb2) const
		{
			int y1 = static_cast<int>(m_y1) - 16;
			int y2 = static_cast<int>(m_y2) - 16;
			int cb = static_cast<int>(m_cb) - 128;
			int cr = static_cast<int>(m_cr) - 128;
			//
			_rgb1.rgbRed		= m_byte((_(1.16780) * y1                   + _(1.60070) * cr) / _MUL);
			_rgb1.rgbGreen		= m_byte((_(1.16780) * y1 - _(0.39290) * cb - _(0.81532) * cr) / _MUL);
			_rgb1.rgbBlue		= m_byte((_(1.16780) * y1 + _(2.02320) * cb                  ) / _MUL);
			_rgb1.rgbReserved	= 0;
			//
			_rgb2.rgbRed		= m_byte((_(1.16780) * y2                   + _(1.60070) * cr) / _MUL);
			_rgb2.rgbGreen		= m_byte((_(1.16780) * y2 - _(0.39290) * cb - _(0.81532) * cr) / _MUL);
			_rgb2.rgbBlue		= m_byte((_(1.16780) * y2 + _(2.02320) * cb                  ) / _MUL);
			_rgb2.rgbReserved	= 0;
		}

		/**
		 * [取得] RGB取得.
		 *	@param[out] _color1 左のピクセル色が格納されます。
		 *	@param[out] _color2 右のピクセル色が格納されます。
		 */
		void GetRgb(COLORREF& _color1, COLORREF& _color2) const
		{
			RGBQUAD rgb1;
			RGBQUAD rgb2;
			GetRgb(rgb1, rgb2);
			_color1 = RGB(rgb1.rgbRed, rgb1.rgbGreen, rgb1.rgbBlue);
			_color2 = RGB(rgb2.rgbRed, rgb2.rgbGreen, rgb2.rgbBlue);
		}

		/**
		 * [設定] RGB設定.
		 *	@param rgb1 左のピクセル色
		 *	@param rgb2 右のピクセル色
		 */
		void SetRgb(const RGBQUAD& rgb1, const RGBQUAD& rgb2)
		{
			int R1 = rgb1.rgbRed;
			int G1 = rgb1.rgbGreen;
			int B1 = rgb1.rgbBlue;
			int R2 = rgb2.rgbRed;
			int G2 = rgb2.rgbGreen;
			int B2 = rgb2.rgbBlue;
			int y1 = ( _(0.25600) * R1 + _(0.50267) * G1 + _(0.09760) * B1 +  _(16) + (_MUL / 2)) / _MUL;
			int y2 = ( _(0.25600) * R2 + _(0.50267) * G2 + _(0.09760) * B2 +  _(16) + (_MUL / 2)) / _MUL;
			int cb = (-_(0.14779) * R1 - _(0.29020) * G1 + _(0.43790) * B1 + _(128) + (_MUL / 2)) / _MUL;
			int cr = ( _(0.43790) * R1 - _(0.36670) * G1 - _(0.07120) * B1 + _(128) + (_MUL / 2)) / _MUL;
			SetYcc(down_cast<BYTE>(y1), down_cast<BYTE>(y2), down_cast<BYTE>(cb), down_cast<BYTE>(cr));
		}

		/**
		 * [設定] RGB設定.
		 *	@param color1 左のピクセル色
		 *	@param color2 右のピクセル色
		 */
		void SetRgb(COLORREF color1, COLORREF color2)
		{
			RGBQUAD rgb1;
			RGBQUAD rgb2;
			rgb1.rgbRed		= GetRValue(color1);
			rgb1.rgbGreen	= GetGValue(color1);
			rgb1.rgbBlue	= GetBValue(color1);
			rgb1.rgbReserved = 0;
			rgb2.rgbRed		= GetRValue(color2);
			rgb2.rgbGreen	= GetGValue(color2);
			rgb2.rgbBlue	= GetBValue(color2);
			rgb2.rgbReserved = 0;
			SetRgb(rgb1, rgb2);
		}

	private:
		// Byte設定
		BYTE m_byte(int p) const
		{
			if ( p < 0 )
			{
				return 0;
			}
			else if ( p > 255 )
			{
				return 255;
			}
			return static_cast<BYTE>(p);
		}
		BYTE m_cb;	///< Cb
		BYTE m_y1;	///< Y1
		BYTE m_cr;	///< Cr
		BYTE m_y2;	///< Y2
	};


	//-------------


	/// コンストラクタ
	CYcc422LineImage(void) : m_pBuffer(NULL)
	{
	}

	/**
	 * [設定] バッファアタッチ.
	 *	@param pBuffer バッファ
	 */
	void Attach(LPVOID pBuffer)
	{
		m_pBuffer = static_cast<BYTE*>(pBuffer);
	}

	/**
	 * [取得] ピクセル取得.
	 *	@param x X座標。ただし、偶数のみ指定可能です。
	 *	@retval NULL 失敗
	 *	@retval NULL以外 成功。アドレスは、ペアピクセル情報。　
	 */
	const CPairPixel* GetPixel(int x) const
	{
		ASSERT( m_pBuffer != NULL );
		ASSERT( (x & 1) == 0 );
		LPCVOID P1 = &m_pBuffer[(x / 2) * 4];
		const CPairPixel* P = static_cast<const CPairPixel*>(P1);
		return P;
	}

	/**
	 * [取得] ピクセル取得.
	 *	@param x X座標。ただし、偶数のみ指定可能です。
	 *	@retval NULL 失敗
	 *	@retval NULL以外 成功。アドレスは、ペアピクセル情報。　
	 */
	CPairPixel* GetPixel(int x)
	{
		ASSERT( m_pBuffer != NULL );
		ASSERT( (x & 1) == 0 );
		LPVOID P1 = &m_pBuffer[(x / 2) * 4];
		CPairPixel* P = static_cast<CPairPixel*>(P1);
		return P;
	}
	
	/**
	 * [設定] ピクセル設定.
	 *	@param x X座標。ただし、偶数のみ指定可能です。
	 *	@param pixel ペアピクセル情報.
	 */
	void SetPixel(int x, const CPairPixel& pixel)
	{
		ASSERT( m_pBuffer != NULL );
		ASSERT( (x & 1) == 0 );
		LPVOID P1 = &m_pBuffer[(x / 2) * 4];
		CPairPixel* P = static_cast<CPairPixel*>(P1);
		if ( P != NULL )
		{
			*P = pixel;
		}
	}

	/**
	 * [取得] ピクセル取得.
	 *	@param P データアドレス
	 *	@retval NULL 失敗
	 *	@retval NULL以外 成功。アドレスは、ペアピクセル情報。　
	 */
	static const CPairPixel* GetPixel(LPCVOID P)
	{
		return static_cast<const CPairPixel*>(P);
	}

	/**
	 * [取得] ピクセル取得.
	 *	@param P データアドレス
	 *	@retval NULL 失敗
	 *	@retval NULL以外 成功。アドレスは、ペアピクセル情報。　
	 */
	static CPairPixel* GetPixel(LPVOID P)
	{
		return static_cast<CPairPixel*>(P);
	}

private:
	C_ASSERT( sizeof(CPairPixel) == 4 );
	 BYTE*	m_pBuffer;		///< バッファ
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #undef _MUL
 #undef _
#endif



}; // TNB




