#pragma once
/**
 *@file
 * GDI+ �֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifdef new
 #pragma push_macro("new")
 #undef new
 #define __TnbNEEDPOPNEW__
#endif



#include "TnbPointerHandle.h"
#include "TnbStrLib.h"
#include <math.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 // HWND�ƃ��X�i�[��MAP
 _SELECTANY ULONG_PTR s_uGdiToken = 0; 
#endif



/**@ingroup GDIPLUS BITMAP
 * GDI+ �Ǘ��N���X
 *
 *	@todo ��肩��
 *
 *	@attention �� UNICODE�v���W�F�N�g�ł��A�{�N���X�̕������ UNICODE �Ŏw�肷��K�v������܂��B
 *				CUnicode ���A�g�p���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbGdiPlus.h
 * 
 *	@date 07/10/29 �V�K�쐬�B
 *	@date 08/07/14 ��]�`�揈�������P�B
 *	@date 09/01/14 Jpeg�̃N�I���e�B�w��A�ǉ��B
 *	@date 09/02/02 new�}�N��(VC6)�΍�B
 *	@date 11/08/11 DT_* �X�^�C���̏������c���t�ɂȂ��Ă����̂��C���B
 *	@date 19/03/27 DrawLine() �ǉ��B
 */
class CGdiPlus
{
public:

	/**@ingroup GDIPLUS BITMAP
	 * GDI+�p���W�^
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbGdiPlus.h
	 * 
	 *	@date 08/07/14 �V�K�쐬�B
	 */
	class CPos : public Gdiplus::PointF
	{
		DEFSUPER(Gdiplus::PointF);
		// �ϊ�
		template<typename T> Gdiplus::REAL _(T t) const
		{
			return static_cast<Gdiplus::REAL>(t);
		}
	public:
		/**
		 * �R���X�g���N�^.
		 *	@param x X���W
		 *	@param y Y���W
		 */
		CPos(double x, double y) : _super(_(x), _(y)) { }
		/**
		 * �R���X�g���N�^.
		 *	@param pos ���W
		 */
		CPos(const POINT& pos) : _super(_(pos.x), _(pos.y)) { }
		/**
		 * �R���X�g���N�^.
		 *	@param sz ���W
		 */
		CPos(const SIZE& sz) : _super(_(sz.cx), _(sz.cy)) { }
	};


	//-----------------------------


	/// �f�X�g���N�^
	virtual ~CGdiPlus(void)
	{
	}

	/**
	 * �R���X�g���N�^.
	 */
	CGdiPlus(void) : m_hDC(NULL)
	{
		Startup();
	}
	
	/**
	 * �R���X�g���N�^.
	 *	@param hdc �f�o�C�X�R���e�L�X�g�n���h��
	 */
	CGdiPlus(HDC hdc) : m_hDC(hdc)
	{
		Startup();
		m_pGraphics = Gdiplus::Graphics::FromHDC(hdc);
		m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	}
	
	/**
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��
	 */
	CGdiPlus(const CGdiPlus& other) : m_hDC(NULL)
	{
		operator=(other);
	}

	/**
	 * �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CGdiPlus& operator=(const CGdiPlus& other)
	{
		if ( this != &other )
		{
			m_pGraphics.Null();
			if ( m_hDC != NULL )
			{
				m_pGraphics = Gdiplus::Graphics::FromHDC(m_hDC);
				m_pGraphics->SetTextRenderingHint(other.m_pGraphics->GetTextRenderingHint());
			}
			m_pFont = other.m_pFont;
			m_pBrush = other.m_pBrush;
			m_pPen = other.m_pPen;
			m_pStringFormat = other.m_pStringFormat;
		}
		return *this;
	}

	/**
	 * [�ݒ�] �f�o�C�X�R���e�L�X�g�ݒ�.
	 *		�{�N���X�ň����f�o�C�X�R���e�L�X�g���w�肵�܂��B
	 *	@param hdc �f�o�C�X�R���e�L�X�g
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetDC(HDC hdc)
	{
		m_pGraphics.Null();
		m_hDC = hdc;
		m_pGraphics = Gdiplus::Graphics::FromHDC(hdc);
		m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		return m_Ret(m_pGraphics);
	}

	/**
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�{�N���X�ň����t�H���g���w�肵�܂��B
	 *	@param hFont �t�H���g�n���h��
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetFont(HFONT hFont)
	{
		m_pFont = new Gdiplus::Font(m_hDC, hFont);
		return m_Ret(m_pFont);
	}

	/**
	 * �t�H���g�X�^�C���l.
	 *	@see SetFont()
	 */
	enum EFontStyle
	{
		REGULAR		= Gdiplus::FontStyleRegular,		///< �ʏ�
		BOLD		= Gdiplus::FontStyleBold,			///< ����
		ITALIC		= Gdiplus::FontStyleItalic,			///< �΂�
		UNDERLINE	= Gdiplus::FontStyleUnderline,		///< ����
		STRIKEOUT	= Gdiplus::FontStyleStrikeout,		///< Strikeout
	};

	/**
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�{�N���X�ň����t�H���g���w�肵�܂��B
	 *	@param lpszFontName �t�H���g��
	 *	@param size �T�C�Y
	 *	@param style �t�H���g�X�^�C���B {@link TNB::CGdiPlus::EFontStyle EFontStyle} ���畡���uor�v�Ŏw��ł��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetFont(LPCWSTR lpszFontName, float size, int style = REGULAR)
	{
		Gdiplus::FontFamily fontfamily(lpszFontName);
		m_pFont = new Gdiplus::Font(&fontfamily, size, style, Gdiplus::UnitPixel);
		return m_Ret(m_pFont);
	}

	/**
	 * [�ݒ�] �u���V�ݒ�.
	 *		�{�N���X�ň����u���V���w�肵�܂��B
	 *	@note �w��F�P��̃u���V���p�ӂ���܂��B
	 *	@param color �u���V�̐F
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetBrush(COLORREF color)
	{
		Gdiplus::Color c;
		c.SetFromCOLORREF(color);
		m_pBrush = new Gdiplus::SolidBrush(c);
		return m_Ret(m_pBrush);
	}

	/**
	 * [�ݒ�] �u���V�ݒ�.
	 *		�{�N���X�ň����u���V���w�肵�܂��B
	 *	@note �w��̃r�b�g�}�b�v���̃u���V���p�ӂ���܂��B
	 *	@param bmp �u���V�̖͗l�ɂ���r�b�g�}�b�v�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetBrush(HBITMAP bmp)
	{
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		if ( bm.GetLastStatus() == Gdiplus::Ok )
		{
			m_pBrush = new Gdiplus::TextureBrush(&bm);
			return m_Ret(m_pBrush);
		}
		m_pBrush.Null();
		return false;
	}

//LinearGradientBrush
//HatchBrush

	
	/**
	 * [�ݒ�] �y���ݒ�.
	 *		�{�N���X�ň����y�����w�肵�܂��B
	 *	@note �w��F�P��̃y�����p�ӂ���܂��B
	 *	@param color �y���̐F
	 *	@param width ��
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetPen(COLORREF color, float width)
	{
		Gdiplus::Color co;
		co.SetFromCOLORREF(color);
		m_pPen = new Gdiplus::Pen(co, width);
		return m_Ret(m_pPen);
	}

	/**
	 * �e�L�X�g�����_�����O�q���g�l.
	 *	@see SetTextRenderingHint()
	 */
	enum ETextRenderingHint
	{
		SYSTEM_DEFAULT	= Gdiplus::TextRenderingHintSystemDefault,				///< �V�X�e���f�t�H���g
		BITPERPIXEL		= Gdiplus::TextRenderingHintSingleBitPerPixelGridFit,	///< �r�b�g�}�b�v
		ANTIALIAS		= Gdiplus::TextRenderingHintAntiAliasGridFit,			///< �A���`�G�C���A�X
		CLEARTYPE		= Gdiplus::TextRenderingHintClearTypeGridFit			///< �N���A�^�C�v
	};

	/**
	 * [�ݒ�] �e�L�X�g�����_�����O�q���g�ݒ�.
	 *		�{�N���X�ň����e�L�X�g�����_�����O�q���g���w�肵�܂��B
	 *	@param hint �q���g�l
	 *	@param boIsGridFit �q���g�l���A BITPERPIXEL �A ANTIALIAS �̏ꍇ�A false ���w�肷��ƁAGridfit���܂���B
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g���w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool SetTextRenderingHint(ETextRenderingHint hint, bool boIsGridFit = false)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		int i = hint;
		if ( ! boIsGridFit && (hint == BITPERPIXEL || hint == ANTIALIAS) )
		{
			i++;
		}
		Gdiplus::TextRenderingHint h = static_cast<Gdiplus::TextRenderingHint>(i);
		return m_Ret(m_pGraphics->SetTextRenderingHint(h));
	}	

	/**
	 * [�ݒ�] �e�L�X�g�X�^�C���ݒ�.
	 *		DrawText ����ێg�p����X�^�C�����w�肵�܂��B
	 *	@param drawTextStyle DT_ �Ŏn�܂�V���{���̑g�ݍ��킹���w��ł��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetTextStyle(DWORD drawTextStyle)
	{
		m_pStringFormat = ms_DtToStringFormat(drawTextStyle);
		return m_Ret(m_pStringFormat);
	}

	/**
	 * [�`��] �e�L�X�g�`��.
	 *	@note SetFont() , SetBrush() ������Ă���K�v������܂��B
	 *			�܂��A SetTextRenderingHint() , SetTextStyle() �ŕ\���ݒ肪�\�ł��B
	 *	@param rc �\���ʒu
	 *	@param lpsz �\��������
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawText(const RECT& rc, LPCWSTR lpsz) const
	{
		if ( m_pGraphics.IsNull() || m_pFont.IsNull() || m_pBrush.IsNull() ) { return false; }
		Gdiplus::RectF rcf(_(rc.left), _(rc.top), _(rc.right - rc.left + 1), _(rc.bottom - rc.top + 1));
		Gdiplus::Status r = m_pGraphics->DrawString(lpsz, -1, m_pFont, rcf, m_pStringFormat, m_pBrush);
		return m_Ret(r);
	}

	/**
	 * [�`��] �e�L�X�g�`��.
	 *	@note SetFont() , SetBrush() ������Ă���K�v������܂��B
	 *			�܂��A SetTextRenderingHint() , SetTextStyle() �ŕ\���ݒ肪�\�ł��B
	 *	@param po �\���ʒu
	 *	@param lpsz �\��������
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawText(const CGdiPlus::CPos& po, LPCWSTR lpsz) const
	{
		if ( m_pGraphics.IsNull() || m_pFont.IsNull() || m_pBrush.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->DrawString(lpsz, -1, m_pFont, po, m_pStringFormat, m_pBrush);
		return m_Ret(r);
	}

	/**
	 * �C���[�W����A���S���Y�����[�h�l.
	 *	@see SetImageInterpolationMode()
	 */
	enum EInterpolationMode
	{
		BICUBIC			= Gdiplus::InterpolationModeBicubic,			///< �o�O����Ԃ��w�肵�܂��B���O�t�B���^�����͎��s����܂���B���̃��[�h�́A�C���[�W�����̃T�C�Y�� 25% �ȉ��ɂ���悤�ȏk�������ɂ͓K���Ă��܂���B
		BILINEAR		= Gdiplus::InterpolationModeBilinear,			///< �o�ꎟ��Ԃ��w�肵�܂��B���O�t�B���^�����͎��s����܂���B���̃��[�h�́A�C���[�W�����̃T�C�Y�� 50% �ȉ��ɂ���悤�ȏk�������ɂ͓K���Ă��܂���B
//		I_DEFAULT		= Gdiplus::InterpolationModeDefault,			///< ����̃��[�h���w�肵�܂��B
		HIGH			= Gdiplus::InterpolationModeHighQuality,		///< ���i����Ԃ��w�肵�܂��B
		HQ_BICUBIC		= Gdiplus::InterpolationModeHighQualityBicubic,	///< ���i���o�O����Ԃ��w�肵�܂��B���O�t�B���^�������K�p����A���i���̏k�����������s����܂��B���̃��[�h���g�p����ƁA�ϊ���̃C���[�W�����i���ɂȂ�܂��B
		HQ_BILINEAR		= Gdiplus::InterpolationModeHighQualityBilinear,///< ���i���o�ꎟ��Ԃ��w�肵�܂��B���O�t�B���^�������K�p����A���i���̏k�����������s����܂��B 
//		I_INVALID		= Gdiplus::InterpolationModeInvalid,			///< �����ȃ��[�h���w�肵�܂��B 
		LOW				= Gdiplus::InterpolationModeLowQuality,			///< ��i����Ԃ��w�肵�܂��B
		NEAREST_NEIGHBOR = Gdiplus::InterpolationModeNearestNeighbor,	///< �ŋߖT��Ԃ��w�肵�܂��B
	};
	
	/**
	 * [�ݒ�] �C���[�W�`��A���S���Y���ݒ�.
	 *		�C���[�W���g��܂��͉�]����Ƃ��̃A���S���Y�����w�肵�܂��B 
	 *	@param mode ���[�h�l�B
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool SetImageInterpolationMode(EInterpolationMode mode)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->SetInterpolationMode(static_cast<Gdiplus::InterpolationMode>(mode));
		return m_Ret(r);
	}

	/**
	 * [�ݒ�] �C���[�W�`�摮���N���A.
	 *	@note SetImageTransparentColor() �̐ݒ�𖳌��ɂ��܂��B
	 */
	void ClearImageAttribute(void)
	{
		m_pImageAttr = NULL;
	}

	/**
	 * [�ݒ�] �C���[�W�`�擧�ߐF�w��.
	 *		�C���[�W�`�掞�ɓ��ߏ������s�����Ƃ��o���܂��B
	 *	@note �F�� 3 �̗v�f (�ԁA�΁A��) ���A���ꂼ��Ή�����v�f�̏�ʂƉ��ʂ̃J���[ �L�[�̊Ԃɂ���ꍇ�A���̐F�͓����ɂȂ�܂��B
	 *	@see ClearImageAttribute
	 *	@param colorLow ���ʂ̃J���[ �L�[�l�B 
	 *	@param colorHigh ��ʂ̃J���[ �L�[�l�B 
	 */
	void SetImageTransparentColor(COLORREF colorLow, COLORREF colorHigh)
	{
		if ( m_pImageAttr.IsNull() )
		{
			m_pImageAttr = new Gdiplus::ImageAttributes();
		}
		Gdiplus::Color c1;
		Gdiplus::Color c2;
		c1.SetFromCOLORREF(colorLow);
		c2.SetFromCOLORREF(colorHigh);
		m_pImageAttr->SetColorKey(c1, c2);
	}

	/**
	 * [�ݒ�] �C���[�W�`�摮���Q��.
	 *	@note �჌�x�����\�b�h�ł��B�{�C���X�^���X�ŊǗ�����C���[�W�����̎Q�Ƃ�Ԃ����ƂŁA
	 *			���R�ɑ����̐ݒ�����邱�Ƃ��\�ł��iGDI+�̒m�����K�v�ł��j�B
	 *	@return Gdiplus::ImageAttribute �̃|�C���^.
	 */
	CPointerHandleT<Gdiplus::ImageAttributes> ReferImageAttribute(void)
	{
		if ( m_pImageAttr.IsNull() )
		{
			m_pImageAttr = new Gdiplus::ImageAttributes();
		}
		return m_pImageAttr;
	}

	/**
	 * [�`��] �r�b�g�}�b�v�`��.
	 *	@param po �\���ʒu
	 *	@param bmp �r�b�g�}�b�v
	 *	@param mag �{���B�ȗ������ 1 �ɂȂ�܂��B
	 *	@param isFlipX true ���w�肷��� ���������ɔ��]���܂��B
	 *	@param isFlipY true ���w�肷��� ���������ɔ��]���܂��B
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawImage(const CGdiPlus::CPos& po, HBITMAP bmp, double mag = 1.0, bool isFlipX = false, bool isFlipY = false) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		Gdiplus::PointF pos[3];
		pos[0].X = _(po.X);
		pos[0].Y = _(po.Y);
		pos[1].X = _(po.X + bm.GetWidth() * mag);
		pos[1].Y = _(po.Y);
		pos[2].X = _(po.X);
		pos[2].Y = _(po.Y + bm.GetHeight() * mag);
		if ( isFlipX )
		{
			 pos[0].X = _(po.X + bm.GetWidth() * mag);		// ���������̔��]���������܂��B
			 pos[1].X = _(po.X);
			 pos[2].X = pos[0].X;
		}
		if ( isFlipY )
		{
			pos[0].Y = _(po.Y + bm.GetHeight() * mag);		// ���������̔��]���������܂��B
			pos[1].Y = pos[0].Y;
			pos[2].Y = _(po.Y);
		}
		return m_DrawImage(bm, pos, 3);
	}

	/**
	 * [�`��] �r�b�g�}�b�v�t�B�b�g�`��.
	 *		�w��̕\���ʒu�ɉ摜�̔䗦��ς����ɕ\�����܂��B
	 *	@param rc �\���ʒu
	 *	@param bmp �r�b�g�}�b�v
	 *	@param isFlipX true ���w�肷��� ���������ɔ��]���܂��B
	 *	@param isFlipY true ���w�肷��� ���������ɔ��]���܂��B
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawFitImage(const RECT& rc, HBITMAP bmp, bool isFlipX = false, bool isFlipY = false) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		double width = rc.right - rc.left;
		double height = rc.bottom - rc.top;
		double dx = width / bm.GetWidth();
		double dy = height / bm.GetHeight();
		CGdiPlus::CPos po(0, 0);
		double d = 1.0;
		if ( dx > dy )
		{
			//dy ���x�[�X�ɂ���
			d = dy;
			double x = (width - bm.GetWidth() * d);
			po.X = static_cast<float>(x / 2.0);
		}
		else
		{
			//dx ���x�[�X�ɂ���
			d = dx;
			double y = (height - bm.GetHeight() * d);
			po.Y = static_cast<float>(y / 2.0);
		}
		return DrawImage(po, bmp, d, isFlipX, isFlipY);
	}
	
	/**
	 * [�`��] �r�b�g�}�b�v��]�`��.
	 *	@param po �d�S�\���ʒu�B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param hotSpot �r�b�g�}�b�v�̏d�S�B�r�b�g�}�b�v�̑��΍��W���w�肵�܂��B
	 *	@param angle �p�x ( 0�`359��)
	 *	@param mag �{���B�ȗ������ 1 �ɂȂ�܂��B
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawRotateImage(const CGdiPlus::CPos& po, HBITMAP bmp, const CGdiPlus::CPos& hotSpot, double angle, double mag = 1.0) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE hPalette = NULL;
		Gdiplus::Bitmap bm(bmp, hPalette);
		Gdiplus::PointF pos[3];
		double restW = bm.GetWidth() - hotSpot.X;
		double restH = bm.GetHeight() - hotSpot.Y;
		double radian = ToRadian(angle);
		double sinVal = ::sin(radian);
		double cosVal = ::cos(radian);
		// ��]�E�g�� & ���s�ړ�
		pos[0].X = _(((hotSpot.X * -cosVal) - (hotSpot.Y * -sinVal)) * mag + po.X);
		pos[0].Y = _(((hotSpot.X * -sinVal) + (hotSpot.Y * -cosVal)) * mag + po.Y);
		pos[1].X = _(((restW * cosVal) - (hotSpot.Y * -sinVal)) * mag + po.X);
		pos[1].Y = _(((restW * sinVal) + (hotSpot.Y * -cosVal)) * mag + po.Y);
		pos[2].X = _(((hotSpot.X * -cosVal) - (restH * sinVal)) * mag + po.X);
		pos[2].Y = _(((hotSpot.X * -sinVal) + (restH * cosVal)) * mag + po.Y);
		//
		return m_DrawImage(bm, pos, 3);
	}

	/**
	 * [�`��] �r�b�g�}�b�v��]�`��.
	 *	@note �d�S�͉摜�̒��S�ɂȂ�܂��B
	 *	@param po �d�S�\���ʒu�B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param angle �p�x ( 0�`359��)
	 *	@param mag �{���B�ȗ������ 1 �ɂȂ�܂��B
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawRotateImage(const CGdiPlus::CPos& po, HBITMAP bmp, double angle, double mag = 1.0) const
	{
		BITMAP bm;
		if ( ::GetObject(bmp, sizeof(BITMAP), &bm) )
		{
			return DrawRotateImage(po, bmp, CPos(bm.bmWidth / 2.0, bm.bmHeight / 2.0), angle, mag);
		}
		return false;
	}

	/**
	 * [�`��] �r�b�g�}�b�v�X�Ε`��.
	 *	@param po �\���ʒu
	 *	@param bmp �r�b�g�}�b�v
	 *	@param diff X�����AY�����̂���
	 *	@param mag �{���B�ȗ������ 1 �ɂȂ�܂��B
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawSkewImage(const CGdiPlus::CPos& po, HBITMAP bmp, const CGdiPlus::CPos& diff, double mag = 1.0) const
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(bmp, h);
		Gdiplus::PointF pos[3];
		pos[0].X = _(po.X);
		pos[0].Y = _(po.Y);
		pos[1].X = _(po.X + bm.GetWidth() * mag);
		pos[1].Y = _(po.Y + diff.Y * mag);
		pos[2].X = _(po.X + diff.X * mag);
		pos[2].Y = _(po.Y + bm.GetHeight() * mag);
		//
		return m_DrawImage(bm, pos, 3);
	}

	/**
	 * �X���[�W���O���[�h�l.
	 *	@see SetSmoothingMode()
	 */
	enum ESmoothingMode
	{
//		S_INVALID		= Gdiplus::SmoothingModeInvalid,		///< �����ȃ��[�h���w�肵�܂��B
//		S_DEFAULT		= Gdiplus::SmoothingModeDefault,		///< �A���`�G�C���A�X�������Ȃ����Ƃ��w�肵�܂��B
		HIGH_SPEED		= Gdiplus::SmoothingModeHighSpeed,		///< �����ŁA��i���̃����_�����O���w�肵�܂��B
		HIGH_QUALITY	= Gdiplus::SmoothingModeHighQuality,	///< ���i���Œᑬ�ȃ����_�����O���w�肵�܂��B
		NONE			= Gdiplus::SmoothingModeNone,			///< �A���`�G�C���A�X�������Ȃ����Ƃ��w�肵�܂��B
		ANTI_ALIAS		= Gdiplus::SmoothingModeAntiAlias		///< �A���`�G�C���A�X�������ꂽ���^�����O���w�肵�܂��B
	};

	/**
	 * [�ݒ�] �X���[�W���O���[�h�ݒ�.
	 *	@note �����A�Ȑ��A����ѓh��Ԃ��̈�̊p�ɑ΂��ăX���[�W���O (�A���`�G�C���A�X�Ƃ��Ă΂�܂�) ���g�p���邩�ǂ������w�肵�܂��B
	 *	@param mode ���[�h
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool SetSmoothingMode(ESmoothingMode mode)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->SetSmoothingMode(static_cast<Gdiplus::SmoothingMode>(mode));
		return m_Ret(r);
	}

	/**
	 * [�`��] ���C���`��.
	 *		�y���� SetPen() �Ŏw�肵�܂��B
	 *	@param s �n�_
	 *	@param e �I�_
	 *	@retval false ���s�B �f�o�C�X�R���e�L�X�g�Ȃǂ��w�肳��Ă��܂���B
	 *	@retval true �����B
	 */
	bool DrawLine(const CPos& s, const CPos& e)
	{
		if ( m_pGraphics.IsNull() ) { return false; }
		Gdiplus::Status r = m_pGraphics->DrawLine(m_pPen, s, e);
		return m_Ret(r);
	}

  /*
	bool DrawArc()
	bool DrawCurve()
	bool DrawEllipse()
	bool DrawPie()
	bool DrawPolygon()
	bool DrawRectangle()

	bool FillCurve()
	bool FillEllipse()
	bool FillPie()
	bool FillPolygon()
	bool FillRectangle()
	bool FillRegion()
	*/

	//------------------------------------

	/**
	 * [�ݒ�] GDI+ ������.
	 *	@note �v���O�����ň�x�����R�[������K�v������܂��B
	 *			�����I�ɃR�[������܂����A���Ԃ������邽�߁A
	 *			�v���O�����̍ŏ��ň�x�R�[�����Ă������ق����悢��������܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool Startup(void)
	{
		if ( s_uGdiToken == 0 )
		{
			static CReject r;
			Gdiplus::GdiplusStartupInput startupInput;
			Gdiplus::GdiplusStartup(&s_uGdiToken, &startupInput, NULL);
		}
		return s_uGdiToken != 0;
	}

	/**
	 * [�\��] ������\��.
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param hFont �t�H���g�B
	 *	@param drawTextStyle DT_ �Ŏn�܂�V���{���̑g�ݍ��킹���w��ł��܂��B
	 *	@param color �F�B
	 *	@param po ���W�B
	 *	@param lpsz ������B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool DrawAntialiasText(HDC dc, HFONT hFont, DWORD drawTextStyle, COLORREF color, const CGdiPlus::CPos& po, LPCWSTR lpsz)
	{
		CGdiPlus g(dc);
		g.SetFont(hFont);
		g.SetBrush(color);
		g.SetTextRenderingHint(ANTIALIAS);
		g.SetTextStyle(drawTextStyle);
		return g.DrawText(po, lpsz);
	}

	/**
	 * [�쐬] �摜�t�@�C���Ǎ���.
	 *		BMP, JPEG �Ȃǂ̃t�@�C����ǂݍ��݁A HBITMAP ���쐬���܂��B
	 *	@param lpsz �t�@�C�����B
	 *	@retval NULL�ȊO �����B
	 *	@retval NULL ���s�B
	 */
	static HBITMAP LoadBitmap(LPCWSTR lpsz)
	{
		Startup();
		Gdiplus::Bitmap bm(lpsz);
		if ( bm.GetLastStatus() == Gdiplus::Ok )
		{
			Gdiplus::Color c;
			c.SetFromCOLORREF(RGB(255, 255, 255));
			HBITMAP hBmp;
			if ( bm.GetHBITMAP(c, &hBmp) == Gdiplus::Ok )
			{
				return hBmp;
			}
		}
		return NULL;
	}

	/**
	 * [�쐬] �T���l�C���摜�t�@�C���Ǎ���.
	 *		�摜�t�@�C�����̃T���l�C����ǂݍ��݁A HBITMAP ���쐬���܂��B
	 *	@param lpsz �t�@�C�����B
	 *	@retval NULL�ȊO �����B
	 *	@retval NULL ���s�B�T���l�C�����Ȃ��ꍇ�����s�ɂȂ�܂��B
	 */
	static HBITMAP LoadThumbnail(LPCWSTR lpsz)
	{
		Startup();
		Gdiplus::Bitmap bmp(lpsz);
		if ( bmp.GetLastStatus() == Gdiplus::Ok )
		{
			UINT size = bmp.GetPropertyItemSize(PropertyTagThumbnailData);
			if ( size > 0 )
			{
				HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, size);
				BYTE* B = static_cast<BYTE *>(::GlobalLock(hGlobal));
				Gdiplus::PropertyItem* P = reinterpret_cast<Gdiplus::PropertyItem*>(B);
				bmp.GetPropertyItem(PropertyTagThumbnailData, size, P);
				TNB::MemCopy(&B[0], &B[16], size - 16);
				::GlobalUnlock(hGlobal);
				IStream* pis = NULL;
				CreateStreamOnHGlobal(hGlobal, TRUE, &pis);
				Gdiplus::Bitmap bm(pis);
				if ( bm.GetLastStatus() == Gdiplus::Ok )
				{
					Gdiplus::Color c;
					c.SetFromCOLORREF(RGB(0, 0, 0));
					HBITMAP hBmp;
					if ( bm.GetHBITMAP(c, &hBmp) == Gdiplus::Ok )
					{
						pis->Release();
						::GlobalFree(hGlobal);
						return hBmp;
					}
				}
				if ( pis != NULL )
				{
					pis->Release();
				}
				::GlobalFree(hGlobal);
			}
		}
		return NULL;
	}

	/**
	 * Save�t�H�[�}�b�g�l
	 *	@see SaveBitmap()
	 */
	enum EBitmapFormat
	{
		BMP,		///< BMP
		JPEG,		///< JPEG
		JPG = JPEG,	///< JPEG
		GIF,		///< GIF
		TIFF,		///< TIFF
		TIF = TIFF,	///< TIFF
		PNG,		///< PNG
	};

	/**
	 * [�쐬] �摜�t�@�C���쐬.
	 *		�r�b�g�}�b�v�� BMP, JPEG �Ȃǂ̃t�@�C���ɂ��܂��B
	 *	@param lpsz �t�@�C�����B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param fmt �t�H�[�}�b�g�B�ȗ������BMP�ł��B
	 *	@param jpegQuality JPEG�̃N�I���e�B(0�`100)�B�ȗ������80�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool SaveBitmap(LPCWSTR lpsz, Gdiplus::Bitmap& bmp, EBitmapFormat fmt = BMP, LONG jpegQuality = 80)
	{
		Startup();
		LPCWSTR lpFmtDes;
		Gdiplus::EncoderParameters encoderParameters;
		Gdiplus::EncoderParameters* pEncoderParameters = NULL;
		switch( fmt )
		{
		case BMP:
		default:	lpFmtDes = L"BMP";	break;
		case GIF:	lpFmtDes = L"GIF";	break;
		case TIFF:	lpFmtDes = L"TIFF";	break;
		case PNG:	lpFmtDes = L"PNG";	break;
		case JPEG:	
			lpFmtDes = L"JPEG";	
			encoderParameters.Count = 1;
			encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality; 
			encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
			encoderParameters.Parameter[0].NumberOfValues = 1;
			encoderParameters.Parameter[0].Value = &jpegQuality;	//JPEG�N�I���e�B�F0�`100
			pEncoderParameters = &encoderParameters;
			break;
		}		
		UINT numEncoders;
		UINT size = 0;
		Gdiplus::GetImageEncodersSize(&numEncoders, &size);
		CWorkMem work(size);
		Gdiplus::ImageCodecInfo* pEncoders = reinterpret_cast<Gdiplus::ImageCodecInfo*>(work.Ref());
		Gdiplus::GetImageEncoders(numEncoders, size, pEncoders);
		loop ( i, numEncoders )
		{
			if ( STRLIB::Compare(pEncoders[i].FormatDescription, lpFmtDes) == 0 )
			{
				return bmp.Save(lpsz, &(pEncoders[i].Clsid), pEncoderParameters) == Gdiplus::Ok;
			}
		}
		return false;
	}

	/**
	 * [�쐬] �摜�t�@�C���쐬.
	 *		�r�b�g�}�b�v�� BMP, JPEG �Ȃǂ̃t�@�C���ɂ��܂��B
	 *	@param lpsz �t�@�C�����B
	 *	@param hBmp �r�b�g�}�b�v�B
	 *	@param fmt �t�H�[�}�b�g�B�ȗ������BMP�ł��B
	 *	@param jpegQuality JPEG�̃N�I���e�B(0�`100)�B�ȗ������80�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool SaveBitmap(LPCWSTR lpsz, HBITMAP hBmp, EBitmapFormat fmt = BMP, LONG jpegQuality = 80)
	{
		HPALETTE h = NULL;
		Gdiplus::Bitmap bm(hBmp, h);
		return SaveBitmap(lpsz, bm, fmt, jpegQuality);
	}

private:

	//GDI+ Shotdown�����p�T�u�N���X
	class CReject
	{
	public:	
		~CReject(void)
		{
			if ( s_uGdiToken != 0 )
			{
				Gdiplus::GdiplusShutdown(s_uGdiToken);
				s_uGdiToken = 0;
			}
		}
	};

	HDC											m_hDC;				///< �f�o�C�X�R���e�L�X�g
	CPointerHandleT<Gdiplus::Graphics>			m_pGraphics;		///< �O���t�B�b�N�I�u�W�F�N�g
	CPointerHandleT<Gdiplus::ImageAttributes>	m_pImageAttr;		///< �C���[�W����
	//
	CPointerHandleT<Gdiplus::StringFormat>		m_pStringFormat;	///< �����t�H�[�}�b�g�I�u�W�F�N�g
	CPointerHandleT<Gdiplus::Font>				m_pFont;			///< �t�H���g�I�u�W�F�N�g
	CPointerHandleT<Gdiplus::Brush>				m_pBrush;			///< �u���V�I�u�W�F�N�g
	CPointerHandleT<Gdiplus::Pen>				m_pPen;				///< �y���I�u�W�F�N�g
//	Gdiplus::Region			m_region;		///< ���[�W����
//	Gdiplus::GraphicsPath	m_graphicsPath;	///< �p�X�H

	/// DT �� StrintFormat
	static Gdiplus::StringFormat* ms_DtToStringFormat(DWORD drawTextStyle)
	{
		Gdiplus::StringFormat* P = new Gdiplus::StringFormat();
		Gdiplus::StringAlignment alignX = Gdiplus::StringAlignmentNear;
		if ( (drawTextStyle & DT_CENTER) != 0 )
		{
			alignX = Gdiplus::StringAlignmentCenter;
		}
		else if ( (drawTextStyle & DT_RIGHT) != 0 )
		{
			alignX = Gdiplus::StringAlignmentFar;
		}
		P->SetAlignment(alignX);
		Gdiplus::StringAlignment alignY = Gdiplus::StringAlignmentNear;
		if ( (drawTextStyle & DT_VCENTER) != 0 )
		{
			alignY = Gdiplus::StringAlignmentCenter;
		}
		else if ( (drawTextStyle & DT_BOTTOM) != 0 )
		{
			alignY = Gdiplus::StringAlignmentFar;
		}
		P->SetLineAlignment(alignY);
		//
		int formatFlags = 0;
		if ( (drawTextStyle & DT_SINGLELINE) == 0 )
		{
			formatFlags |= Gdiplus::StringFormatFlagsNoClip;
		}
		P->SetFormatFlags(formatFlags);
		//
		Gdiplus::StringTrimming trim = Gdiplus::StringTrimmingNone;
		if ( (drawTextStyle & DT_END_ELLIPSIS) != 0 )
		{
			trim = Gdiplus::StringTrimmingEllipsisCharacter;
		}
		if ( (drawTextStyle & DT_WORD_ELLIPSIS) != 0 )
		{
			trim = Gdiplus::StringTrimmingEllipsisWord;
		}
		if ( (drawTextStyle & DT_PATH_ELLIPSIS) != 0 )
		{
			trim = Gdiplus::StringTrimmingEllipsisPath;
		}
		P->SetTrimming(trim);
		//
		if ( (drawTextStyle & DT_NOPREFIX) != 0 )
		{
			P->SetHotkeyPrefix(Gdiplus::HotkeyPrefixNone);
		}
		else
		{
			P->SetHotkeyPrefix(Gdiplus::HotkeyPrefixShow);
		}
		return P;
	}
	// �ϊ�
	template<typename T> Gdiplus::REAL _(T t) const
	{
		return static_cast<Gdiplus::REAL>(t);
	}
	// ��
	static double PI(void)
	{
		return 3.141592653589793;
	}
	// �p�x -> ���W�A��
	static double ToRadian(double angle)
	{
		return angle * PI() / 180.0;
	}
	// ���W�A�� -> �p�x
	static double ToAngle(double radian)
	{
		return radian * 180.0 / PI();
	}
	// ���ʔ��f
	bool m_Ret(Gdiplus::Status s) const
	{
		return s == Gdiplus::Ok;
	}
	// ���ʔ��f
	template<typename TYP>
	bool m_Ret(CPointerHandleT<TYP>  P) const
	{
		return P->GetLastStatus() == Gdiplus::Ok;
	}
	/**
	 * �`��.
	 */
	bool m_DrawImage(Gdiplus::Bitmap& bm, Gdiplus::PointF* pPos, int posLength) const
	{
		ASSERT( ! ::IsBadReadPtr(pPos, sizeof(Gdiplus::PointF) * posLength) );
		if ( ! m_pImageAttr.IsNull() )
		{
			Gdiplus::Status r = m_pGraphics->DrawImage( &bm, pPos, posLength, 0, 0, _(bm.GetWidth()), _(bm.GetHeight()), Gdiplus::UnitPixel, m_pImageAttr);
			return m_Ret(r);
		}
		return m_Ret(m_pGraphics->DrawImage(&bm, pPos, posLength));
	}
};



}; // TNB



#ifdef __TnbNEEDPOPNEW__
 #pragma pop_macro("new")
#endif
