#pragma once
/**
 *@file
 * YCbCr�֌W�̃w�b�_
 *
 *	YCbCr �̉摜���������߂̃T�|�[�g�N���X������܂��B
 *
 *		���� YCbCr 4:2:2 (8bit) �̊Ǘ��N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #define _MUL 0x20000 //(131072)
 #define _(X) (int(X * _MUL))
#endif



/**@ingroup BITMAP
 * YCbCr���C���C���[�W�Ǘ�.
 *		YCbCr 4:2:2 �̉摜�̈ꃉ�C�����������߂̃T�|�[�g�N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbYccLineImage.h
 *
 *	@date 09/07/28	�V�K�쐬
 */
class CYcc422LineImage
{
public:

	/**
	 * �y�A�s�N�Z�����.
	 *		2�s�N�Z���Z�b�g�ŊǗ����܂��B
	 *	
	 *	@par�K�v�t�@�C��
	 *			TnbYccLineImage.h
	 *
	 *	@date 09/07/28	�V�K�쐬
	 */
	class CPairPixel
	{
	public:

		/**
		 * �R���X�g���N�^.
		 *		�S�v�f�O�̃C���X�^���X�ɂȂ�܂��B
		 */
		CPairPixel(void)
		{
			Zero(*this);
		}

		/**
		 * �R���X�g���N�^.
		 *	@param color1 ���̃s�N�Z���F
		 *	@param color2 �E�̃s�N�Z���F
		 */
		CPairPixel(COLORREF color1, COLORREF color2)
		{
			SetRgb(color1, color2);
		}

		/**
		 * �R���X�g���N�^.
		 *	@param rgb1 ���̃s�N�Z���F
		 *	@param rgb2 �E�̃s�N�Z���F
		 */
		CPairPixel(const RGBQUAD& rgb1, const RGBQUAD& rgb2)
		{
			SetRgb(rgb1, rgb2);
		}

		/**
		 * [�ݒ�] Cb�u�F��(��)�v�ݒ�.
		 *	@param cb Cb 
		 */
		void SetCb(BYTE cb)
		{
			m_cb = cb;
		}

		/**
		 * [�擾] Cb�u�F��(��)�v�擾.
		 *	@return Cb
		 */
		BYTE GetCb(void) const
		{
			return m_cb;
		}

		/**
		 * [�ݒ�] Y1�u�P�x(��)�v�ݒ�.
		 *	@param y1 Y1
		 */
		void SetY1(BYTE y1)
		{
			m_y1 = y1;
		}

		/**
		 * [�擾] Y1�u�P�x(��)�v�擾.
		 *	@return Y1
		 */
		BYTE GetY1(void) const
		{
			return m_y1;
		}

		/**
		 * [�ݒ�] Cr�u�F��(��)�v�ݒ�.
		 *	@param cr Cr
		 */
		void SetCr(BYTE cr)
		{
			m_cr = cr;
		}

		/**
		 * [�擾] Cr�u�F��(��)�v�擾.
		 *	@return Cr
		 */
		BYTE GetCr(void) const
		{
			return m_cr;
		}

		/**
		 * [�ݒ�] Y2�u�P�x(�E)�v�ݒ�.
		 *	@param y2 Y2
		 */
		void SetY2(BYTE y2)
		{
			m_y2 = y2;
		}

		/**
		 * [�擾] Y2�u�P�x(�E)�v�擾.
		 *	@return Y2
		 */
		BYTE GetY2(void) const
		{
			return m_y2;
		}

		/**
		 * [�ݒ�] Y Cb Cr �ݒ�.
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
		 * [�擾] RGB�擾.
		 *	@param[out] _rgb1 ���̃s�N�Z���F
		 *	@param[out] _rgb2 �E�̃s�N�Z���F
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
		 * [�擾] RGB�擾.
		 *	@param[out] _color1 ���̃s�N�Z���F���i�[����܂��B
		 *	@param[out] _color2 �E�̃s�N�Z���F���i�[����܂��B
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
		 * [�ݒ�] RGB�ݒ�.
		 *	@param rgb1 ���̃s�N�Z���F
		 *	@param rgb2 �E�̃s�N�Z���F
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
		 * [�ݒ�] RGB�ݒ�.
		 *	@param color1 ���̃s�N�Z���F
		 *	@param color2 �E�̃s�N�Z���F
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
		// Byte�ݒ�
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


	/// �R���X�g���N�^
	CYcc422LineImage(void) : m_pBuffer(NULL)
	{
	}

	/**
	 * [�ݒ�] �o�b�t�@�A�^�b�`.
	 *	@param pBuffer �o�b�t�@
	 */
	void Attach(LPVOID pBuffer)
	{
		m_pBuffer = static_cast<BYTE*>(pBuffer);
	}

	/**
	 * [�擾] �s�N�Z���擾.
	 *	@param x X���W�B�������A�����̂ݎw��\�ł��B
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO �����B�A�h���X�́A�y�A�s�N�Z�����B�@
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
	 * [�擾] �s�N�Z���擾.
	 *	@param x X���W�B�������A�����̂ݎw��\�ł��B
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO �����B�A�h���X�́A�y�A�s�N�Z�����B�@
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
	 * [�ݒ�] �s�N�Z���ݒ�.
	 *	@param x X���W�B�������A�����̂ݎw��\�ł��B
	 *	@param pixel �y�A�s�N�Z�����.
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
	 * [�擾] �s�N�Z���擾.
	 *	@param P �f�[�^�A�h���X
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO �����B�A�h���X�́A�y�A�s�N�Z�����B�@
	 */
	static const CPairPixel* GetPixel(LPCVOID P)
	{
		return static_cast<const CPairPixel*>(P);
	}

	/**
	 * [�擾] �s�N�Z���擾.
	 *	@param P �f�[�^�A�h���X
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO �����B�A�h���X�́A�y�A�s�N�Z�����B�@
	 */
	static CPairPixel* GetPixel(LPVOID P)
	{
		return static_cast<CPairPixel*>(P);
	}

private:
	C_ASSERT( sizeof(CPairPixel) == 4 );
	 BYTE*	m_pBuffer;		///< �o�b�t�@
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #undef _MUL
 #undef _
#endif



}; // TNB




