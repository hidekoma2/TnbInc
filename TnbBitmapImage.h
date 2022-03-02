#pragma once
/**
 *@file
 * �r�b�g�}�b�v�C���[�W�Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapHandle.h"
#ifndef _WIN32_WCE
 #pragma comment(lib, "Msimg32.lib")
#endif
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #ifndef AC_SRC_ALPHA
  #define AC_SRC_ALPHA                0x01
 #endif
#endif



//TNB Library
namespace TNB
{



/**@ingroup BITMAP
 * �r�b�g�}�b�v�C���[�W�Ǘ��N���X
 *
 *		�r�b�g�}�b�v�C���[�W����Ǘ����܂��B�f�o�C�X�R���e�L�X�g �Ƃ̐e�a�������A�C���[�W�̕ҏW���e�Ղł��B
 *
 *	@note
 *		�e�� Set() ���g���A�C���X�^���X�������C���[�W���w�肵�܂��B\n
 *		�e��}���֐�( Insert() , InsertOnAlphaBlend(), InsertOnTransparent(), InsertOnSemitransparect() )���g���A
 *		�ʃC���[�W���摜��}���A�����o���܂��B\n
 *		Cut() �ŃC���[�W�̔C�ӂ̉ӏ���؂�o���܂��B\n
 *		Draw() �ȂǂŁA�w��̃f�o�C�X�R���e�L�X�g�ɕ`�悪�\�ł��B\n
 *		GetDC() , ReleaseDC() �ŁA�f�o�C�X�R���e�L�X�g�Ƃ��ăC���[�W���������Ƃ��\�ł��B
 *
 *	@note MFC ���g������ł́A CBitmapDC �N���X���g�p�ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapImage.h
 *
 *	@date 07/10/03 �V�K�쐬
 *	@date 07/10/19 InsertOnSemitransparect() �A���ߐF�w��\�ɂ����B Tile() �ǉ��B
 *	@date 07/10/22 Attach() , Detach() �ǉ��B�����Ǘ��Ɠ����n���h����n���ꂽ���Ɉȏ�ɂȂ�Ȃ��悤�ɂ����B
 *					GradationFill() �ǉ��B
 *	@date 07/10/27 Draw() �� POINT �w��\�ɂ����B
 *	@date 07/10/27 CRawData �ŊǗ�����f�[�^��24bit����32bit�ɕύX�B
 *	@date 07/10/29 CreateMaskBitmap() , CustomizeAlpha() ��ǉ��B
 *	@date 07/10/30 CaptureClient() , CaptureWindow() , SetStretchBltMode() ��ǉ��B
 *	@date 07/11/06 ChangePixelColor() �ǉ��B
 *	@date 07/11/13 Save() �ǉ��B Tile() �p�~�A DrawTile() �ǉ��B Set() �ɐF�w���ǉ��B
 *	@date 07/11/21 �R�s�[�R���X�g���N�^�̃o�O�C���i�����ɂȂ�HDC���g�p����\�����������j�B
 *	@date 07/11/23 SetFromClient() �ɔ͈͎w��\���\�b�h��ǉ��B CustomizeAlpha() ���C���B
 *	@date 07/12/04 Work�p��DC�̎����������P�B
 *	@date 08/01/22 SetFromClient() �̑�R�����̊g���B
 *	@date 08/03/12 GradationFill() �ɑ�4�����ǉ��B Semitransparect �n�֐��� parsent �Ӗ��g���B
 *	@date 08/07/02 BIT���쏈���œK���BCE�Ή��B
 *	@date 09/07/29 CalcAdjustRect() �V�K�B Cut() �Ő��������摜�̃T�C�Y���s���������B
 *	@date 09/08/26 DIB ���ӎ����āA�œK���B
 *	@date 11/10/11 CreateRotateImage() , CreateReverseImage() , enum EDirection �A �V�K�ǉ��B
 *	@date 12/03/01 const �� GetRgbDataPtr() ��ǉ��B
 *	@date 12/03/21 GradationFill() �̃G���[�`�F�b�N�����B
 *	@date 14/12/09 SetEx() �V�K�BRGB ������Ȃ�����ȉ摜���쐬����ׂɒǉ��B
 *	@date 15/05/01 CreateGrayscaleBitmap() �V�K�B
 *	@date 19/06/14 CreateDilationBitmap() , CreateErosionBitmap() , MedianFilter() �V�K�B
 *	@date 19/06/30 Paint() �V�K�B
 */
class CBitmapImage
{
public:

	/**@ingroup BITMAP
	 * �r�b�g�}�b�v���f�[�^�Ǘ��N���X
	 *
	 *		�r�b�g�}�b�v�̐��f�[�^����Ǘ����܂��B
	 *		
	 *	@note ���f�[�^�͏��������܂���B
	 *
	 *	@deprecated �p�~�\��ł��B CBitmapImage::GetRgbDataPtr() ���g�p���Ă��������B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbBitmapImage.h
	 *
	 *	@date 07/10/03 �V�K�쐬
	 *	@date 07/10/23 �\�����v.�������ނ�Ȃ��B
	 */
	class CRawData
	{
		/// �f�[�^�Ǘ��p�����[�^
		struct TParam
		{
			SIZE				size;			///< �摜�T�C�Y
			CWorkMemT<RGBQUAD>	work;			///< ������
			TParam(const SIZE& s) : size(s), work(s.cx * s.cy)
			{
			}
			TParam(void)
			{
				size.cx = size.cy = 0;
			}
		};
		CPointerHandleT<TParam>	m_pParam;		///< ���f�[�^�Ǘ�
	public:
		/// �R���X�g���N�^
		CRawData(void) : m_pParam(new TParam()) { }
		/**
		 * �R���X�g���N�^
		 *	@param s �T�C�Y�B
		 */
		CRawData(const SIZE& s) : m_pParam(new TParam(s)) { }
		/**
		 * [�m�F] �L���m�F.
		 *	@retval true �C���X�^���X���f�[�^�L���B
		 *	@retval false �����B
		 */
		bool IsValid(void) const { return m_pParam->size.cx != 0; }
		/**
		 * [�擾] �T�C�Y�擾
		 *	@return �T�C�Y�B
		 */
		const SIZE& GetSize(void) const { return m_pParam->size; }
		/**
		 * [�擾] �ꃉ�C��BYTE���擾
		 *	@return BYTE���B
		 */
		size_t GetLineBytes(void) const { return m_pParam->size.cx * sizeof(RGBQUAD); }
		/**
		 * [�擾] �f�[�^�Q��.
		 *	@note �f�[�^�́A�������珇�Ɋi�[����Ă��܂��B
		 *	@note �ʏ�A operator[] , Rgb() ���g���āA�ꃉ�C�����A��|�C���g���A�h���X���擾���Ďg�p���܂��B
		 *	@retval NULL �f�[�^�Ȃ��B
		 *	@retval NULL�ȊO �f�[�^�A�h���X�B
		 */
		RGBQUAD* Refer(void) { return m_pParam->work; }
		/**
		 * [�擾] �f�[�^�Q��.
		 *	@note �f�[�^�́A�������珇�Ɋi�[����Ă��܂��B
		 *	@note �ʏ�A operator[] ���g���āA�ꃉ�C�����擾���Ďg�p���܂��B
		 *	@retval NULL �f�[�^�Ȃ��B
		 *	@retval NULL�ȊO �f�[�^�A�h���X�B
		 */
		const RGBQUAD* Refer(void) const { return m_pParam->work; }
		/**
		 * [�擾] ���C���f�[�^�Q��.
		 *	@note �f�[�^�́A�����珇�Ɋi�[����Ă��܂��B
		 *	@param y ���C���B ��ԏオ�O�ł��B
		 *	@retval NULL �f�[�^�Ȃ��i���C�����͈͊O�j�B
		 *	@retval NULL�ȊO �f�[�^�A�h���X�B
		 */
		RGBQUAD* operator[](INDEX y)
		{
			if ( y >= static_cast<UINT>(m_pParam->size.cy) ) { return NULL; }
			return Refer() + (m_pParam->size.cy - y - 1) * m_pParam->size.cx;
		}
		/**
		 * [�擾] ���C���f�[�^�Q��.
		 *	@note �f�[�^�́A�����珇�Ɋi�[����Ă��܂��B
		 *	@param y ���C���B ��ԏオ�O�ł��B
		 *	@retval NULL �f�[�^�Ȃ��i���C�����͈͊O�j�B
		 *	@retval NULL�ȊO �f�[�^�A�h���X�B
		 */
		const RGBQUAD* operator[](INDEX y) const
		{
			if ( y >= static_cast<UINT>(m_pParam->size.cy) ) { return NULL; }
			return Refer() + (m_pParam->size.cy - y - 1) * m_pParam->size.cx;
		}
	};

	/// �R���X�g���N�^
	CBitmapImage(void) : m_bmp(), m_pRgb(NULL), m_backDC(NULL), m_backBmp(NULL)
		, m_hWnd(NULL), m_nStretchMode(DEFAULT_STRETCHMODE)
	{
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * ����R���X�g���N�^
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	CBitmapImage(CBitmapHandle bmp) : m_bmp(), m_pRgb(NULL), m_backDC(NULL), m_backBmp(NULL)
		, m_hWnd(NULL), m_nStretchMode(DEFAULT_STRETCHMODE)
	{
		m_Set(bmp);
	}
	
	/**
	 * �R�s�[�R���X�g���N�^
	 *	@note	�{�N���X�̃R�s�[�́A�R�s�[���A�R�s�[��œ����n���h�������L���邱�ƂɂȂ�܂��B
	 *			�R�s�[��A�ҏW�ړI�̏ꍇ�A Clone() �� Set() ���g���Ă��������B
	 *	@param other �R�s�[���B
	 */
	CBitmapImage(const CBitmapImage& other) : m_bmp(other.m_bmp), m_pRgb(other.m_pRgb), m_backDC(NULL), m_backBmp(NULL)
		, m_hWnd(other.m_hWnd), m_size(other.m_size), m_nStretchMode(other.m_nStretchMode)
	{
	}

	/**
	 * �R�s�[�I�y���[�^
	 *	@note	�{�N���X�̃R�s�[�́A�R�s�[���A�R�s�[��œ����n���h�������L���邱�ƂɂȂ�܂��B
	 *			�R�s�[��A�ҏW�ړI�̏ꍇ�A Clone() �� Set() ���g���Ă��������B
	 *	@param other �R�s�[���B
	 *	@return �����̎Q�ƁB
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

	/// �f�X�g���N�^
	virtual ~CBitmapImage(void)
	{
		Empty();
	}

	/**
	 * [�擾] HBITMAP �擾
	 *	@note �{�C���X�^���X�ŊǗ����Ă���n���h�����擾�ł��܂��B Delete���Ă͂����܂���B
	 *	@return HBITMAP�B Empty��Ԃ̏ꍇ�A NULL ��Ԃ̃n���h�����Ԃ�܂��B
	 */
	operator CBitmapHandle(void)
	{
		return m_bmp;
	}

	/**
	 * [�擾] �r�b�g�}�b�v�n���h���擾
	 *	@note �{�C���X�^���X�ŊǗ����Ă���n���h�����擾�ł��܂��B Delete���Ă͂����܂���B
	 *	@return �r�b�g�}�b�v�n���h���B �{�C���X�^���X�� Empty��Ԃ̏ꍇ�A NULL ��Ԃ̃n���h�����Ԃ�܂��B
	 */
	CBitmapHandle GetBitmapHandle(void)
	{
		return m_bmp;
	}

	/**
	 * [�ݒ�] ���[�N�f�o�C�X�R���e�L�X�g�ݒ�.
	 *	@note �ݒ肵�Ȃ��ꍇ�A�f�X�N�g�b�v(NULL)�ɂɂȂ��Ă��܂��B
	 *	@param h ���[�N�E�B���h�E�n���h���B 
	 */
	void SetWorkWnd(HWND h)
	{
		m_hWnd = h;
	}

	/**
	 * [�ݒ�] �r�b�g�}�b�v�L�k���[�h�ݒ�.
	 *	@note API ::StretchBlt �֐��̃r�b�g�}�b�v�L�k���[�h��ݒ肵�܂��B
	 *	@param nStretchMode �L�k���[�h�B�ȉ��̒l����I���\�ł��B
	 *			<table><tr><td>
	 *			�l											</td><td>
	 *					����								</td></tr><tr><td>
	 *			STRETCH_ANDSCANS							</td><td>
	 *					�_�� AND ���Z�q���g���āA�c���_�̐F�Ǝ�菜���_�̐F��g�ݍ��킹�܂��B
	 *					�r�b�g�}�b�v�����m�N���[���̃r�b�g�}�b�v�ł���ꍇ�A���̃s�N�Z����������A
	 *					 ���̃s�N�Z�����c��܂��B			</td></tr><tr><td>
	 *			STRETCH_DELETESCANS							</td><td>
	 *					�s�N�Z�����폜���܂��B��菜���_�̏������炩�̌`�ňێ����悤�Ƃ͂����A
	 *					�P���ɂ����̓_���폜���܂��B		</td></tr><tr><td>
	 *			STRETCH_HALFTONE							</td><td>
	 *					�R�s�[�������`���̃s�N�Z�����R�s�[�撷���`���̃s�N�Z���u���b�N�Ɋ֘A�t���܂��B
	 *					�R�s�[��u���b�N�̕��ϓI�ȐF�́A�R�s�[���̃s�N�Z���̐F�ɋ߂��F�ɂȂ�܂��B 
	 *					Windows 95/98 �ł́A���̒l�̓T�|�[�g����Ă��܂���B</td></tr><tr><td>
	 *			STRETCH_ORSCANS								</td><td>
	 *					�_�� OR ���Z�q���g���āA�c���_�̐F�Ǝ�菜���_�̐F��g�ݍ��킹�܂��B
	 *					�r�b�g�}�b�v�����m�N���[���̃r�b�g�}�b�v�ł���ꍇ�A���̃s�N�Z����������A
	 *					���̃s�N�Z�����c��܂��B			</td></tr>
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
	 * [�ݒ�] �j��.
	 *		�{�C���X�^���X�Ŏ����Ă���C���[�W��j�����܂��B�{�C���X�^���X�� Empty��ԂɂȂ�܂��B
	 */
	void Empty(void)
	{
		m_ReleaseDC();
		m_bmp.Null();
		m_pRgb = NULL;
	}

	/**
	 * [�m�F] Empty��Ԋm�F.
	 *	@retval true Empty��ԁB�C���[�W�͎����Ă��܂���B
	 *	@retval false �C���[�W�������Ă��܂��B
	 */
	bool IsEmpty(void) const
	{
		return m_bmp.IsNull();
	}

	/**
	 * [�擾] �C���[�W�T�C�Y�擾.
	 *	@note Empty��Ԃł��Ō�Ɉ����Ă�����񂪖߂�܂��B�K���A IsEmpty() �Ŋm�F���Ă���擾���Ă��������B
	 *	@return �T�C�Y�B
	 */
	const SIZE& GetSize(void) const
	{
		ASSERT0( ! IsEmpty(), "CBitmapImage::GetSize()", "�摜�������Ă��Ȃ��I�u�W�F�N�g����T�C�Y���擾���悤�Ƃ��Ă��܂��B" );
		return m_size;
	}

	/**
	 * [�擾] BITMAPINFOHEADER �擾.
	 *	@param[out] _head BITMAPINFOHEADER �i�[��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �N���[���쐬
	 *	@note �{�N���X�́A�������\�ł��B�������A�����C���[�W�������L���Ă��܂��܂��B
	 *			�{���\�b�h���g���ƁA�ʂ̃C���[�W���ɂȂ�܂��B
	 *	@return �����B
	 */
	CBitmapImage Clone(void) const
	{
		CBitmapImage bi;
		bi.Set(m_bmp);
		return bi;
	}

	/**
	 * [�ݒ�] �A�^�b�`.
	 *		�w��̃n���h����{�N���X�ŋ��L���܂��B Set() �ɔ��(�������Ȃ���)�����ł����A
	 *		�����n���h�������L���邽�߁A������ύX����Ɖe�����󂯂Ă��܂��܂��B
	 *	@param bmp �r�b�g�}�b�v�B 
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Attach(CBitmapHandle bmp)
	{
		return m_Set(bmp);
	}

	/**
	 * [�ݒ�] �f�^�b�`.
	 *		�{�C���X�^���X�Ŏg�p���Ă���r�b�g�}�b�v�n���h����Ԃ��A Empty ��ԂɂȂ�܂��B
	 *	@note Attach ��A�{���\�b�h���g�킸�A�C���X�^���X��j�����Ă���肠��܂���B 
	 *	@return �{�C���X�^���X���ێ����Ă����r�b�g�}�b�v�n���h���B 
	 */
	CBitmapHandle Detach(void)
	{
		CBitmapHandle h = m_bmp;
		Empty();
		return h;
	}

	/**
	 * [�ݒ�] �C���[�W�ݒ�.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W��j�����A�w��̃T�C�Y�̃r�b�g�}�b�v�̃C���[�W��ێ����܂��B
	 *	@param cx �r�b�g�}�b�v�̕��B
	 *	@param cy �r�b�g�}�b�v�̍����B
	 *	@param color �h��Ԃ��F�B�ȗ�����ƁA�h��Ԃ��܂���B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
	 * [�ݒ�] �g���C���[�W�ݒ�.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W��j�����A�w��̃T�C�Y�̃r�b�g�}�b�v�̃C���[�W��ێ����܂��B
	 *		���̍ێw��̃E�B���h�E�̃f�o�C�X�R���e�L�X�g�̐F�`���Ńr�b�g�}�b�v���쐬���܂��B
	 *	@param hDC �f�o�C�X�R���e�L�X�g
	 *	@param cx �r�b�g�}�b�v�̕��B
	 *	@param cy �r�b�g�}�b�v�̍����B
	 *	@param color �h��Ԃ��F�B�ȗ�����ƁA�h��Ԃ��܂���B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
	 * [�ݒ�] �C���[�W�ݒ�.
	 *		�{�C���X�^���X�ŕێ����Ă���C���[�W��j�����A�w��̃r�b�g�}�b�v�̃C���[�W�𕡐����ێ����܂��B
	 *	@note ���̃r�b�g�}�b�v��ύX���Ă��e������܂���B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
	 */
	bool Set(CBitmapHandle bmp)
	{
		RGBQUAD* pRgb = NULL;
		return m_Set(CBitmapHandle::Create32bitDibSection(bmp, pRgb), pRgb);
	}

	/**
	 * [�ݒ�] �C���[�W�ݒ�.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W��j�����A�w��̃r�b�g�}�b�v�̃C���[�W��ێ����܂��B
	 *	@param bmpimg �r�b�g�}�b�v�C���[�W�B
	 *	@param cx �r�b�g�}�b�v�̕��B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B
	 *	@param cy �r�b�g�}�b�v�̍����B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
	 * [�ݒ�] ���f�[�^����C���[�W�ݒ�.
	 *	@param rawData ���f�[�^�Ǘ��N���X�B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
	 *	@deprecated �p�~�\��̃��\�b�h�ł��B
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
	 * [�ݒ�] DC����C���[�W�ݒ�.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W��j�����A�w��̃f�o�C�X�R���e�L�X�g�̃C���[�W��ێ����܂��B
	 *	@param hdc ��荞�݌��f�o�C�X�R���e�L�X�g�B
	 *	@param rect hdc �̂ǂ̈ʒu����荞�ނ��w�肵�܂��B
	 *	@param cx �r�b�g�}�b�v�̕��B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B
	 *	@param cy �r�b�g�}�b�v�̍����B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
					::BitBlt(dc, 0, 0, s.cx, s.cy, hdc, rect.left, rect.top, SRCCOPY);	//DC������o��
				}
				else
				{
					::StretchBlt(dc, 0, 0, cx, cy, hdc, rect.left, rect.top, s.cx, s.cy, SRCCOPY);	//DC������o��
				}
				ReleaseDC();
				return true;
			}
		}
		Empty();
		return false;
	}

	/**
	 * [�쐬] ICON����C���[�W�ݒ�.
	 *	@param hIcon �A�C�R���B
	 *	@param backColor ���ߐF�̃G���A�̐F�w��B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
	 * [�ݒ�] �N���C�A���g����C���[�W�ݒ�.
	 *		�w��̃E�B���h�E�̃N���C�A���g�̈�̎w��͈͂̃C���[�W����荞�݂܂��B
	 *	@note WM_PRINTCLIENT �ɑΉ����Ă���K�v������܂��B
	 *	@note CE �ł̓C���[�W�͎�荞�߂܂��񂪁A�����Ń��^�[�����܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 *	@param rect �͈́B
	 *	@param backColor �w�i�F�B�N���b�s���O�͈͊O�̐F���w�肵�܂��B 
	 *					CLR_AUTOSELECT �Ȃ�A WM_ERASEBKGND ���g���A��ɔw�i��`�悵�܂��B
	 *					�ȗ�����ƁA�w�i��`�揈�����܂���B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
	 * [�ݒ�] �N���C�A���g����C���[�W�ݒ�.
	 *		�w��̃E�B���h�E�̃N���C�A���g�̈�̃C���[�W����荞�݂܂��B
	 *	@note WM_PRINTCLIENT �ɑΉ����Ă���K�v������܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 *	@param backColor �w�i�F�B�N���b�s���O�͈͊O�̐F���w�肵�܂��B 
	 *					CLR_AUTOSELECT �Ȃ�A WM_ERASEBKGND ���g���A��ɔw�i��`�悵�܂��B
	 *					�ȗ�����ƁA�w�i��`�揈�����܂���B
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
	 * [�ݒ�] �E�B���h�E����C���[�W�ݒ�.
	 *		�w��̃E�B���h�E�̈�̃C���[�W����荞�݂܂��B
	 *	@note WM_PRINT �ɑΉ����Ă���K�v������܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 *	@param backColor �w�i�F�B�N���b�s���O�͈͊O�̐F���w�肵�܂��B
	 *	@param mode ��荞�݃��[�h�B�ȉ��̒l�̑g�ݍ��킹���w�肵�܂��B
	 *		<table><tr><td>
	 *		�}�X�N										</td><td>
	 *					����							</td></tr><tr><td>
	 *		PRF_CHECKVISIBLE							</td><td>
	 *					�����Ă����Ԃ̂Ƃ��̂ݕ`��	</td></tr><tr><td>
	 *		PRF_CHILDREN								</td><td>
	 *					�����Ă���q�E�B���h�E�̂ݕ`��	</td></tr><tr><td>
	 *		PRF_CLIENT									</td><td>
	 *					�N���C�A���g�̈��`��			</td></tr><tr><td>
	 *		PRF_ERASEBKGND								</td><td>
	 *					�`��O�ɔw�i���ĕ`��			</td></tr><tr><td>
	 *		PRF_NONCLIENT								</td><td>
	 *					��N���C�A���g�̈��`��		</td></tr><tr><td>
	 *		PRF_OWNED									</td><td>
	 *					���L����S�ẴE�B���h�E		</td></tr>
	 *		</table>
	 *	@retval true �����B
	 *	@retval false ���s�BEmpty��ԂɂȂ�܂��B
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
	 * [�}��] �C���[�W�}��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̃r�b�g�}�b�v�̃C���[�W��ǉ����܂��B
	 *	@note WIN32API DrawState ���g�p���Ă��܂��B
	 *	@param x �}��X���W�B
	 *	@param y �}��Y���W�B
	 *	@param hBitmap �}���摜�B
	 *	@param uExFlag �}���t���O�B�ȉ���3��A�g�p�ł��܂��B\n
	 *				�ȗ��� DSS_NORMAL - - - �C���[�W�����̂܂ܕ`�悵�܂��B \n
	 *				DSS_DISABLED - - - �C���[�W�𕂂��オ�点�܂��i�G���{�X�j�B\n
	 *				DSS_UNION - - - �C���[�W���f�B�U�����O���܂��B 
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�}��] �C���[�W�}��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̃r�b�g�}�b�v�̃C���[�W��ǉ����܂��B
	 *	@note cx , cy ���w�肷��ƁA������g���ƁA�g��k�����đ}�����邱�Ƃ��\�ł��B�܂��A
	 *			���̐����w�肷��ƁA���]���đ}�����邱�Ƃ��o���܂��B�������A�}�����W�����ɔ��]���܂��̂ŁA
	 *			�}���ʒu�ɒ��ӂ��Ă��������B
	 *	@note WIN32API BitBlt�AStretchBlt ���g�p���Ă��܂��B
	 *	@param x �}��X���W�B
	 *	@param y �}��Y���W�B
	 *	@param bmpimg �}���r�b�g�}�b�v�C���[�W�B
	 *	@param raster �I�y���[�V�����R�[�h�B�ȗ�����ƁA SRCCOPY �ɂȂ�܂��B\n
	 *			::BitBlt �� dwRop �̃R�[�h���w�肵�܂��B�ȉ���ʓI�ȃR�[�h�ł�\n
	 *			<table><tr><td>
	 *			�R�[�h								</td><td>
	 *					����						</td></tr><tr><td>
	 *			BLACKNESS				</td><td>
	 *					�����p���b�g�̃C���f�b�N�X 0 �ɑΉ�����F�i����̕����p���b�g�ł͍��j�ŁA�R�s�[��̒����`��h��Ԃ��܂��B </td></tr><tr><td>
	 *			DSTINVERT				</td><td>
	 *					�R�s�[�撷���`�̐F�𔽓]���܂��B </td></tr><tr><td>
	 *			MERGECOPY				</td><td>
	 *					�_�� AND ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			MERGEPAINT				</td><td>
	 *					�_�� OR ���Z�q���g���āA�R�s�[���̐F�𔽓]�����F�ƁA�R�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			NOTSRCCOPY				</td><td>
	 *					�R�s�[���̐F�𔽓]���āA�R�s�[��փR�s�[���܂��B </td></tr><tr><td>
	 *			NOTSRCERASE					</td><td>
	 *					�_�� OR ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�A����ɔ��]���܂��B </td></tr><tr><td>
	 *			PATCOPY					</td><td>
	 *					�w�肵���p�^�[�����R�s�[��փR�s�[���܂��B </td></tr><tr><td>
	 *			PATINVERT				</td><td>
	 *					�_�� XOR ���Z�q���g���āA�w�肵���p�^�[���̐F�ƁA�R�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			PATPAINT				</td><td>
	 *					�_�� OR ���Z�q���g���āA�w�肵���p�^�[���̐F�ƁA�R�s�[���̐F�𔽓]�����F��g�ݍ��킹�܂��B����ɘ_�� OR ���Z�q���g���āA���̌��ʂƁA�R�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCAND				</td><td>
	 *					�_�� AND ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCCOPY					</td><td>
	 *					�R�s�[�������`���R�s�[�撷���`�ւ��̂܂܃R�s�[���܂��B </td></tr><tr><td>
	 *			SRCERASE				</td><td>
	 *					�_�� AND ���Z�q���g���āA�R�s�[��̐F�𔽓]�����F�ƁA�R�s�[���̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCINVERT				</td><td>
	 *					�_�� XOR ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCPAINT				</td><td>
	 *					�_�� OR ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			WHITENESS				</td><td>
	 *					�����p���b�g�̃C���f�b�N�X 1 �ɑΉ�����F�i����̕����p���b�g�ł͔��j�ŁA�R�s�[��̒����`��h��Ԃ��܂��B </td></tr>
	 *		</table>
	 *	@param cx �}�����B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B������g���ƁA�g��k�����đ}�����邱�Ƃ��\�ł��B
	 *	@param cy �}�������B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B������g���ƁA�g��k�����đ}�����邱�Ƃ��\�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�}��] ���ߏ����t�C���[�W�}��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̃r�b�g�}�b�v�̃C���[�W��ǉ����܂��B
	 *	@note WIN32API TransparentBlt ���g�p���Ă��܂��B
	 *	@param x �}��X���W�B
	 *	@param y �}��Y���W�B
	 *	@param bmpimg �}���r�b�g�}�b�v�C���[�W�B
	 *	@param color ���߂�����J���[���w�肵�܂��B 
	 *						�ȗ�����ƁA bmpimg �̍����̐F�𓧉ߐF�Ƃ��܂��B\n
	 *						CLR_INVALID ���w�肷��Ɠ��ߏ����Ȃ��ő}�����܂��B
	 *	@param cx �}�����B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B������g���ƁA�g��k�����đ}�����邱�Ƃ��\�ł��B
	 *	@param cy �}�������B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B������g���ƁA�g��k�����đ}�����邱�Ƃ��\�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�}��] �����ߏ����t�C���[�W�}��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̃r�b�g�}�b�v�̃C���[�W��ǉ����܂��B
	 *	@note WIN32API AlphaBlend ���g�p���Ă��܂��B
	 *	@param x �}��X���W�B
	 *	@param y �}��Y���W�B
	 *	@param bmpimg �}���r�b�g�}�b�v�C���[�W�B
	 *	@param parsent �}���摜�̔Z��(�P��;%) �ȗ��Ȃ�100% �B
	 *				-1 ���w�肷��ƁA �ێ����Ă���C���[�W�̃s�N�Z�����̃A���t�@�`���l�����g�p���܂��B
	 *	@param cx �}�����B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B������g���ƁA�g��k�����đ}�����邱�Ƃ��\�ł��B
	 *	@param cy �}�������B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B������g���ƁA�g��k�����đ}�����邱�Ƃ��\�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�}��] �������C���[�W�}��(���ߐF�t).
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̃r�b�g�}�b�v�̃C���[�W��ǉ����܂��B
	 *	@todo ��h�b�g���Ɍv�Z���Ă��邽�߁A�������x�͑�������܂���B
	 *
	 *	@param x �}��X���W�B
	 *	@param y �}��Y���W�B
	 *	@param bmpimg �}���r�b�g�}�b�v�C���[�W�B
	 *	@param parsent �}���摜�̔Z��(�P��;%) �ȗ��Ȃ�100%�B
	 *				-1 ���w�肷��ƁA �ێ����Ă���C���[�W�̃s�N�Z�����̃A���t�@�`���l�����g�p���܂��B
	 *	@param color ���߂�����J���[���w�肵�܂��B 
	 *						�ȗ�����ƁA bmpimg �̍����̐F�𓧉ߐF�Ƃ��܂��B\n
	 *						CLR_INVALID ���w�肷��Ɠ��ߏ����Ȃ��ő}�����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �l�p�`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̐F�Ŏw��̍��W�Ɏl�p��`���܂��B
	 *	@param color �J���[�B
	 *	@param rect ���W�B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �l�p�`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W���A�w��̐F�œh��Ԃ��܂��B
	 *	@param color �J���[�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Fill(COLORREF color)
	{
		RECT rect = { 0, 0, m_size.cx, m_size.cy };
		return Fill(color, rect);
	}

	/// ����
	enum EDirection
	{
		Vertical,		///< ����
		Horizontal		///< ����
	};

	/**
	 * [����] �O���f�[�V�����`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̐F�ŃO���f�[�V������`���܂��B
	 *	@param color1 ��/���J���[�B
	 *	@param color2 �E/��J���[�B
	 *	@param direction �����B Vertical �� Horizontal ���w�肵�܂��B
	 *	@param pRect �͈͂��w�肵�܂��B�ȗ�����ƑS�̂��ΏۂɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �O���f�[�V�����`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̐F�ŃO���f�[�V������`���܂��B
	 *	@param color1 ��/���J���[�B
	 *	@param color2 �E/��J���[�B
	 *	@param boIsHorizontal true �Ȃ琅��(������E)�̃O���f�[�V�����ɂȂ�܂��B
	 *	@param pRect �͈͂��w�肵�܂��B�ȗ�����ƑS�̂��ΏۂɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool GradationFill(COLORREF color1, COLORREF color2, bool boIsHorizontal, const RECT* pRect = NULL)
	{
		EDirection d = boIsHorizontal ? Horizontal : Vertical;
		return GradationFill(color1, color2, d, pRect);
	}

	/**
	 * [����] �O���f�[�V�����`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̏�ɁA�w��̐F�ŃO���f�[�V������`���܂��B
	 *	@param pVertex �O�p�`�̒��_���`����A�e TRIVERTEX �\���̂̔z��ւ̃|�C���^���w�肵�܂��B 
	 *	@param dwNumVertex ���_�̐����w�肵�܂��B 
	 *	@param pMesh GRADIENT_RECT �\���̂̔z����w�肵�܂��B 
	 *	@param dwNumMesh pMesh �p�����[�^�̗v�f�����w�肵�܂��B 
	 *	@param boIsHorizontal true �Ȃ琅��(������E)�̃O���f�[�V�����ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �A���t�@�`���l������.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̃A���t�@�`���l���𑀍삵�܂��B
	 *	@param alpha �ݒ肷��A���t�@�l
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �A���t�@�`���l������.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̃A���t�@�`���l���𑀍삵�܂��B
	 *	@param alpha �ݒ肷��A���t�@�l
	 *	@param colorEx �ʂ̃A���t�@�l�ɐݒ肵�����J���[���w�肵�܂��B
	 *	@param alphaEx colorEx�̐F�ɐݒ肷��A���t�@�l�B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [����] �A���t�@�`���l������.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̃A���t�@�`���l���𑀍삵�܂��B
	 *	@param bmp �A���t�@�l���iRED�̗v�f���g�p�j
	 *	@retval true �����B
	 *	@retval false ���s�B�����傫���̉摜�łȂ��������s���܂��B
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
	 * [����] �J���[�ύX.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W�̎w��̐F���w��̐F�ɒu�������܂��B
	 *	@param targetColor �ύX�������F�B
	 *	@param drawColor �ύX��̐F�B
	 *	@retval �}�C�i�X ���s�B
	 *	@retval 0�ȏ� �ύX����Pixel���B
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
	 * [����] �J���[�ύX.
	 *		���摜�̐F���� RGB�̗v�f�䗦�ŐF��ύX�ł��܂��B
	 *	@note ���Ɠ����G�ɂ���Ȃ� RGB(255,0,0), RGB(0,255,0),  RGB(0,0,255) ���w�肷�邱�ƂɂȂ�܂��B
	 *	@param r �Ԃ̗v�f�䗦�B
	 *	@param g �΂̗v�f�䗦�B
	 *	@param b �̗v�f�䗦�B
	 *	@retval false ���s�B
	 *	@retval true �����B
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
	 * [�擾] �C���[�W���o��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W����A�w��̋�`�͈͂����o���܂��B
	 *	@param rect ���o���͈́B
	 *	@param cx ���o����̕��B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B������g���ƁA�g��k�����Ď��o�����Ƃ��\�ł��B
	 *	@param cy ���o����̍����B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B������g���ƁA�g��k�����Ď��o�����Ƃ��\�ł��B
	 *	@return �r�b�g�}�b�v�C���[�W�B ���s�����ꍇ�AEmpty��Ԃ̂��̂��Ԃ�܂��B
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
	 * [�擾]�f�o�C�X�R���e�L�X�g�n���h���擾.
	 *		�{�C���X�^���X���ێ����Ă���C���[�W��������Ă���f�o�C�X�R���e�L�X�g ��Ԃ��܂��B
	 *		���̃f�o�C�X�R���e�L�X�g ��`�悷��ƁA�{�C���X�^���X���ێ����Ă��� �C���[�W�ɔ��f����܂��B
	 *	@note GetDC() ���� ReleaseDC() ����܂ŁA���̃��\�b�h�͎��s���܂��B
	 *	@retval NULL �G���[�B�C���[�W��ێ����Ă��܂���B
	 *	@retval NULL�ȊO �f�o�C�X�R���e�L�X�g�B�`���A�K���A �{�I�u�W�F�N�g�� ReleaseDC() �����s���Ă��������B
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
	 * [�ݒ�] �f�o�C�X�R���e�L�X�g�n���h���ԋp.
	 *		GetDC() �̌�A�K���{���\�b�h���g�p���Ă��������B
	 *	@retval true �����B
	 *	@retval false ���s�B GetDC() ���Ă��܂���B
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
	 * [�쐬] �}�X�N�p�^�[���r�b�g�}�b�v�쐬.
	 *	@param boIsReverse false �Ȃ�w��F�̕����� 0 ����ȊO�� 1 �̃��m�N���[���r�b�g�}�b�v������܂��B
	 *						true �Ȃ�w��F�̕����� 1 ����ȊO�� 0 �̃��m�N���[���r�b�g�}�b�v������܂��B
	 *	@param transColor �}�X�N����F���w�肵�܂��B�ȗ�����ƁA�ێ����Ă���C���[�W�̍����̐F���w��F�Ƃ��܂��B
	 *	@retval NULL �G���[�B
	 *	@retval NULL�ȊO ���m�N���[���r�b�g�}�b�v
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
	 * [�쐬] ���f�[�^�쐬.
	 *		�{�I�u�W�F�N�g���ێ����鐶�f�[�^�����I�u�W�F�N�g���쐬���܂��B
	 *	@return ���f�[�^�Ǘ��N���X�B �{�֐����G���[�̏ꍇ�A������Ԃ� CRawData ���Ԃ�܂��B
	 *	@deprecated �p�~�\��̃��\�b�h�ł��B
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
	 * [�쐬] ���[�W�����쐬.
	 *		�r�b�g�}�b�v�`��ɍ��킹�ă��[�W�������쐬����.
	 *	@param transColor ���ߐF���w�肵�܂��B 
	 *						�ȗ�����ƁA�{�I�u�W�F�N�g���ێ����Ă���BITMAP�̍����̐F�𓧉ߐF�Ƃ��܂��B\n
	 *						CLR_INVALID ���w�肷��ƃr�b�g�}�b�v�̑傫���̎l�p�̃��[�W�������쐬���܂��B
	 *	@retval NULL �G���[�B
	 *	@retval NULL�ȊO ���[�W�����B �g�p��� ::DeleteObject() �Ŕj�����Ă��������B
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
		HRGN hRgn = NULL;	// ���[�W�����n���h��
		transColor = (transColor & 0xff00ff00) |
					(transColor & 0x00ff0000) >> 16 |
					(transColor & 0x000000ff) << 16;
		//
		CWorkMem rgbWork(sizeof(RGNDATAHEADER) + sizeof(RECT) * bi.m_size.cx * bi.m_size.cy);
		LPRGNDATA pRgnData = reinterpret_cast<LPRGNDATA>(rgbWork.Ref());
		LPRECT pRect = reinterpret_cast<LPRECT>(pRgnData->Buffer);
		int nCntRect = 0;	// �l�p�`���[�W�����̐�
		loop_dn ( y, bi.m_size.cy )
		{
			const COLORREF* pScanData = reinterpret_cast<const COLORREF*>(bi.GetRgbDataPtr(y));
			if ( transColor == CLR_AUTOSELECT )
			{
				transColor = pScanData[0];
			}
			// ��s���̓����^�񓧖��̕ω��_�𒲍�����
			loop ( x, bi.m_size.cx )
			{
				if ( pScanData[x] == transColor )
				{
					continue;
				}
				// ���̈ʒu���瓧���܂ł̒������J�E���g����
				pRect->left = down_cast<long>(x);
				for ( ; x < static_cast<size_t>(bi.m_size.cx); x++ )
				{
					if ( pScanData[x] == transColor ) { break; }
				}
				// ���̏����l�p�`���Ƃ��Ēǉ�����
				pRect->right	= static_cast<LONG>(x);
				pRect->top		= static_cast<LONG>(y);
				pRect->bottom	= static_cast<LONG>(y + 1);
				pRect++;
				nCntRect++;
			}
		}
		// �������߂��l�p�`�������Ƀ��[�W�������쐬����
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
	 * [����] �C���[�W�`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̈ʒu�ɁA�{�C���X�^���X�Ŏ����Ă���C���[�W��`�悵�܂��B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param x �`��X�ʒu(����0)�B�ȗ�����ƂO�B
	 *	@param y �`��Y�ʒu(�オ0)�B�ȗ�����ƂO�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Draw(HDC hdc, int x = 0, int y = 0) const
	{
		HBITMAP h = m_bmp;
		return CBitmapHandle::Draw(hdc, h, x, y);
	}

	/**
	 * [����] �C���[�W�`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̈ʒu�ɁA�{�C���X�^���X�Ŏ����Ă���C���[�W��`�悵�܂��B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param po �`��ʒu(���オ0,0)�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Draw(HDC hdc, const POINT& po) const
	{
		return Draw(hdc, po.x, po.y);
	}

	/**
	 * [����] �C���[�W�^�C���`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��͈̔͂ɁA�{�C���X�^���X�Ŏ����Ă���C���[�W��~���l�߂܂��B
	 *	@note �摜�͊g�傳��܂���B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param rect �`��͈�(���オ0,0)�B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�\��] �C���[�W�`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W���A�w��f�o�C�X�R���e�L�X�g�ɕ`�悵�܂��B
	 *	@note WIN32API BitBlt ���g�p���Ă��܂��B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param x �`��X�ʒu(����0)�B
	 *	@param y �`��Y�ʒu(�オ0)�B
	 *	@param raster �I�y���[�V�����R�[�h�B�ȗ�����ƁA SRCCOPY �ɂȂ�܂��B\n
	 *			::BitBlt �� dwRop �̃R�[�h���w�肵�܂��B�ȉ���ʓI�ȃR�[�h�ł�\n
	 *			<table><tr><td>
	 *			�R�[�h								</td><td>
	 *					����						</td></tr><tr><td>
	 *			BLACKNESS				</td><td>
	 *					�����p���b�g�̃C���f�b�N�X 0 �ɑΉ�����F�i����̕����p���b�g�ł͍��j�ŁA�R�s�[��̒����`��h��Ԃ��܂��B </td></tr><tr><td>
	 *			DSTINVERT				</td><td>
	 *					�R�s�[�撷���`�̐F�𔽓]���܂��B </td></tr><tr><td>
	 *			MERGECOPY				</td><td>
	 *					�_�� AND ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			MERGEPAINT				</td><td>
	 *					�_�� OR ���Z�q���g���āA�R�s�[���̐F�𔽓]�����F�ƁA�R�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			NOTSRCCOPY				</td><td>
	 *					�R�s�[���̐F�𔽓]���āA�R�s�[��փR�s�[���܂��B </td></tr><tr><td>
	 *			NOTSRCERASE					</td><td>
	 *					�_�� OR ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�A����ɔ��]���܂��B </td></tr><tr><td>
	 *			PATCOPY					</td><td>
	 *					�w�肵���p�^�[�����R�s�[��փR�s�[���܂��B </td></tr><tr><td>
	 *			PATINVERT				</td><td>
	 *					�_�� XOR ���Z�q���g���āA�w�肵���p�^�[���̐F�ƁA�R�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			PATPAINT				</td><td>
	 *					�_�� OR ���Z�q���g���āA�w�肵���p�^�[���̐F�ƁA�R�s�[���̐F�𔽓]�����F��g�ݍ��킹�܂��B����ɘ_�� OR ���Z�q���g���āA���̌��ʂƁA�R�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCAND				</td><td>
	 *					�_�� AND ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCCOPY					</td><td>
	 *					�R�s�[�������`���R�s�[�撷���`�ւ��̂܂܃R�s�[���܂��B </td></tr><tr><td>
	 *			SRCERASE				</td><td>
	 *					�_�� AND ���Z�q���g���āA�R�s�[��̐F�𔽓]�����F�ƁA�R�s�[���̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCINVERT				</td><td>
	 *					�_�� XOR ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			SRCPAINT				</td><td>
	 *					�_�� OR ���Z�q���g���āA�R�s�[���̐F�ƃR�s�[��̐F��g�ݍ��킹�܂��B </td></tr><tr><td>
	 *			WHITENESS				</td><td>
	 *					�����p���b�g�̃C���f�b�N�X 1 �ɑΉ�����F�i����̕����p���b�g�ł͔��j�ŁA�R�s�[��̒����`��h��Ԃ��܂��B </td></tr>
	 *		</table>
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�\��] �C���[�W�`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W���A�w��f�o�C�X�R���e�L�X�g�ɕ`�悵�܂��B
	 *	@note cx , cy ���w�肷��ƁA������g���ƁA�g��k�����ĕ`�悷�邱�Ƃ��\�ł��B�܂��A
	 *			���̐����w�肷��ƁA���]���đ}�����邱�Ƃ��o���܂��B�������A�}�����W�����ɔ��]���܂��̂ŁA
	 *			�}���ʒu�ɒ��ӂ��Ă��������B
	 *	@note WIN32API StretchBlt ���g�p���Ă��܂��B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param x �`��X�ʒu(����0)�B
	 *	@param y �`��Y�ʒu(�オ0)�B
	 *	@param raster �I�y���[�V�����R�[�h�B�ȗ�����ƁA SRCCOPY �ɂȂ�܂��B 
	 *					�R�[�h�́A BitBlt() �̃I�y���[�V�����R�[�h���Q�Ƃ��Ă��������B
	 *	@param cx �`�敝�B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B������g���ƁA�g��k�����ĕ\�����邱�Ƃ��\�ł��B
	 *	@param cy �`�捂���B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B������g���ƁA�g��k�����ĕ\�����邱�Ƃ��\�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�\��] ���ߏ����t�C���[�W�`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W���A�w��̃f�o�C�X�R���e�L�X�g�ɃC���[�W��\�����܂��B
	 *	@note WIN32API TransparentBlt ���g�p���Ă��܂��B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param x �`��X���W�B
	 *	@param y �`��Y���W�B
	 *	@param color ���߂�����J���[���w�肵�܂��B 
	 *					�ȗ�����ƁA�{�I�u�W�F�N�g���ێ����Ă���r�b�g�}�b�v�̍����̐F�𓧉ߐF�Ƃ��܂��B\n
	 *					CLR_INVALID ���w�肷��Ɠ��ߏ����Ȃ��ŕ`�悵�܂��B
	 *	@param cx �\�����B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B������g���ƁA�g��k�����ĕ\�����邱�Ƃ��\�ł��B
	 *	@param cy �\�������B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B������g���ƁA�g��k�����ĕ\�����邱�Ƃ��\�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�`��] �����������t�C���[�W�`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W���A�w��̃f�o�C�X�R���e�L�X�g�ɕ`�悵�܂��B
	 *	@note WIN32API AlphaBlend ���g�p���Ă��܂��B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param x �`��X���W�B
	 *	@param y �`��Y���W�B
	 *	@param parsent �`��摜�̔Z��(�P��;%) �ȗ��Ȃ�100% �B
	 *				-1 ���w�肷��ƁA �ێ����Ă���C���[�W�̃s�N�Z�����̃A���t�@�`���l�����g�p���܂��B
	 *	@param cx �\�����B�ȗ�����ƃC���[�W���̂܂܂̕��ɂȂ�܂��B������g���ƁA�g��k�����ĕ\�����邱�Ƃ��\�ł��B
	 *	@param cy �\�������B�ȗ�����ƃC���[�W���̂܂܂̍����ɂȂ�܂��B������g���ƁA�g��k�����ĕ\�����邱�Ƃ��\�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
				blend.AlphaFormat = AC_SRC_ALPHA; //�A���t�@�`���l���g�p
			}
			#if ! defined(_WIN32_WCE) || (_WIN32_WCE > 0x500) 
				return !! ::AlphaBlend(hdc, x, y, cx, cy, dc, 0, 0, m_size.cx, m_size.cy, blend);
			#endif
		}
		return false;
	}

	/**
	 * [�}��] ������,���߃C���[�W�`��.
	 *	@note �{�C���X�^���X�ŕێ����Ă���C���[�W���A�w��̃f�o�C�X�R���e�L�X�g�ɕ`�悵�܂��B
	 *	@attention ��h�b�g���Ɍv�Z���Ă��邽�߁A�������x�͑�������܂���B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param x �`��X���W�B
	 *	@param y �`��Y���W�B
	 *	@param parsent �`��摜�̔Z��(�P��;%) �ȗ��Ȃ�100%�B
	 *				-1 ���w�肷��ƁA �ێ����Ă���C���[�W�̃s�N�Z�����̃A���t�@�`���l�����g�p���܂��B
	 *	@param color ���߂�����J���[���w�肵�܂��B 
	 *					�ȗ�����ƁA�{�I�u�W�F�N�g���ێ����Ă���r�b�g�}�b�v�̍����̐F�𓧉ߐF�Ƃ��܂��B\n
	 *					CLR_INVALID ���w�肷��Ɠ��ߏ����Ȃ��ŕ`�悵�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�m�F] RGB�f�[�^�L��.
	 *	@retval true �L��
	 *	@retval false �Ȃ�. ToHaveRgbData() ���g���āARGB�f�[�^���������邱�Ƃ��o���܂��B
	 */
	bool HasRgbData(void) const
	{
		return m_pRgb != NULL;
	}

	/**
	 * [�ݒ�] RGB�f�[�^����.
	 *	@note RGB�f�[�^���������邱�Ƃ��o���܂��B GetRgbDataPtr() �œ����A�h���X�ŁA���ځA�G��ҏW�ł��܂��B
	 *	@retval true ����
	 *	@retval false ���s
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
	 * [�擾] RGB�f�[�^�A�h���X�擾.
	 *		RGB�f�[�^�̃A�h���X�𓾂邱�ƂŁA���ڊG��ҏW���邱�Ƃ��o���܂��B
	 *	@note RGB�f�[�^�������Ă��Ȃ��Ǝ��s���܂�( ToHaveRgbData() ���g���āARGB�f�[�^���������邱�Ƃ��o���܂��j�B
	 *	@param y �w��� Y �̃��C���̃A�h���X�B �ȗ�(-1)�Ȃ�A�f�[�^�S��(�摜�̏c - 1 ���w�肵���̂Ɠ����A�h���X) �B
	 *	@retval NULL ���s�B�B
	 *	@retval NULL�ȊO RGB�f�[�^�A�h���X
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
	 * [�擾] RGB�f�[�^�A�h���X�擾.
	 *		RGB�f�[�^�̃A�h���X�𓾂邱�ƂŁA���ڊG��ҏW���邱�Ƃ��o���܂��B
	 *	@note RGB�f�[�^�������Ă��Ȃ��Ǝ��s���܂�( ToHaveRgbData() ���g���āARGB�f�[�^���������邱�Ƃ��o���܂��j�B
	 *	@param y �w��� Y �̃��C���̃A�h���X�B �ȗ�(-1)�Ȃ�A�f�[�^�S��(�摜�̏c - 1 ���w�肵���̂Ɠ����A�h���X) �B
	 *	@retval NULL ���s�B�B
	 *	@retval NULL�ȊO RGB�f�[�^�A�h���X
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
	 * [�쐬] ��]�摜�쐬.
	 *		���v�����90�x�X���摜���쐬���܂��B
	 *	@note HasRgbData() �� true ��Ԃ��I�u�W�F�N�g�łȂ��Ǝ��s���܂��B
	 *	@return �r�b�g�}�b�v�C���[�W�B ���s�����ꍇ�AEmpty��Ԃ̂��̂��Ԃ�܂��B
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
	 * [�쐬] ���]�摜�쐬.
	 *		���]�摜���쐬���܂��B
	 *	@note HasRgbData() �� true ��Ԃ��I�u�W�F�N�g�łȂ��Ǝ��s���܂��B
	 *	@param isVertical true �Ȃ�㉺�ɔ��]���܂��B
	 *	@param isHorizontal true �Ȃ獶�E�ɔ��]���܂��B
	 *	@return �r�b�g�}�b�v�C���[�W�B ���s�����ꍇ�AEmpty��Ԃ̂��̂��Ԃ�܂��B
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
	 * [�쐬] �O���C�X�P�[���摜�쐬.
	 *		�O���C�X�P�[���摜���쐬���܂��B
	 *	@note HasRgbData() �� true ��Ԃ��I�u�W�F�N�g�łȂ��Ǝ��s���܂��B
	 *	@return �r�b�g�}�b�v�C���[�W�B ���s�����ꍇ�AEmpty��Ԃ̂��̂��Ԃ�܂��B
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
	 * [�쐬] �c���摜�쐬.
	 *		�c���摜���쐬���܂��BGREEN�v�f�����݂�̂ŃO���C�X�P�[������l�̉摜��ΏۂƂ��܂��B
	 *	@note HasRgbData() �� true ��Ԃ��I�u�W�F�N�g�łȂ��Ǝ��s���܂��B
	 *	@return �r�b�g�}�b�v�C���[�W�B ���s�����ꍇ�AEmpty��Ԃ̂��̂��Ԃ�܂��B
	 */
	CBitmapImage CreateDilationBitmap(void) const
	{
		return m_DilationErosion(true);
	}
	
	/**
	 * [�쐬] ���k�摜�쐬.
	 *		���k�摜���쐬���܂��BGREEN�v�f�����݂�̂ŃO���C�X�P�[������l�̉摜��ΏۂƂ��܂��B
	 *	@note HasRgbData() �� true ��Ԃ��I�u�W�F�N�g�łȂ��Ǝ��s���܂��B
	 *	@return �r�b�g�}�b�v�C���[�W�B ���s�����ꍇ�AEmpty��Ԃ̂��̂��Ԃ�܂��B
	 */
	CBitmapImage CreateErosionBitmap(void) const
	{
		return m_DilationErosion(false);
	}

	/**
	 * [�쐬] ���f�B�A���t�B���^�[�摜�쐬.
	 *		���f�B�A���t�B���^�[�摜���쐬���܂��B
	 *	@note HasRgbData() �� true ��Ԃ��I�u�W�F�N�g�łȂ��Ǝ��s���܂��B
	 *	@return �r�b�g�}�b�v�C���[�W�B ���s�����ꍇ�AEmpty��Ԃ̂��̂��Ԃ�܂��B
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
			const RGBQUAD* po1 = GetRgbDataPtr(y - 1);// y=0�̎����ҊO�̒l���Ԃ邪�ȍ~�ŃJ�o�[���Ă�
			const RGBQUAD* po2 = GetRgbDataPtr(y + 1);// y=sz.cy-1�̎����ҊO�̒l���Ԃ邪�ȍ~�ŃJ�o�[���Ă�
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
	 * [�`��] �y�C���g.
	 *	@note HasRgbData() �� true ��Ԃ��I�u�W�F�N�g�łȂ��Ǝ��s���܂��B
	 *	@param x X���W
	 *	@param y Y���W
	 *	@param c �J���[
	 *	@retval �y�C���g����PIXEL��
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
	 * [�쐬] BMP�t�@�C���쐬.
	 *	@note CE �ł� bitsPixel �͖�������K���R�Q�ɂȂ�܂��B
	 *	@param lpszFileName �t�@�C�����B
	 *	@param bitsPixel ��h�b�g�̃r�b�g���B1,4,8,16,24,32 �̂����ꂩ���w��ł��܂��B 0�Ȃ� �r�b�g�}�b�v�̏��܂܂ɂȂ�܂��B 
	 *	@retval ERROR_SUCCESS �����B
	 *	@retval ��L�ȊO ���s�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD Save(LPCTSTR lpszFileName, WORD bitsPixel = 0)
	{
		return CBitmapHandle::Save(lpszFileName, m_bmp, bitsPixel);
	}


	//-----------------


	/**
	 * [�Ǎ�] BMP�t�@�C���Ǎ���.
	 *	@param lpszFileName �t�@�C�����B
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL ��	 *	@retval NULL�ȊO �r�b�g�}�b�v�n���h���B
	 *	@deprecated CBitmapHandle::Load() ���g�p���Ă��������B
	 */
	static HBITMAP _deprecated Load(LPCTSTR lpszFileName, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		return CBitmapHandle::Load(lpszFileName, cx, cy, fuLoad);
	}

	/**
	 * [�Ǎ�] ���\�[�X�Ǎ���
	 *	@note �C���X�^���X�n���h���́A GetInstanceHandle(EI_Bitmap) �̒l���g�p����܂��B
	 *	@param uBitmapResourceId �r�b�g�}�b�v���\�[�XID
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO �r�b�g�}�b�v�n���h���B
	 *	@deprecated CBitmapHandle::Load() ���g�p���Ă��������B
	 */
	static HBITMAP _deprecated Load(UINT uBitmapResourceId, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		return CBitmapHandle::Load(uBitmapResourceId, cx, cy, fuLoad);
	}

	/**
	 * [�擾] �r�b�g�}�b�v�T�C�Y�擾. 
	 *	@param[out] _size �T�C�Y�i�[�B
	 *	@param[in] hBitmap �r�b�g�}�b�v�n���h���B�ꎞ�I�ɂ����g�p���܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�쐬] �摜�k���g��
	 *	@note DPI �ɂ��킹�ĉ摜�̑傫���𒲐����܂��B�x�[�X96DPI�ł��B
	 *	@param bmp ���r�b�g�}�b�v
	 *	@return �ϊ���̃r�b�g�}�b�v
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
	 * [�v�Z] �\���ʒu����.
	 * 		�w��̑傫���̃G���A�ɁA�䗦��ς����ɉ�ʂ̒����Ɍ��Ԗ����z�u���邽�߂͈̔͂��v�Z���܂��B
	 *	@param[out] _rect �z�u����͈͂��i�[����܂��B
	 *	@param[in] dstSize �z�u��̃T�C�Y.
	 *	@param[in] srcSize ���̏��̃T�C�Y
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

	CBitmapHandle		m_bmp;			///< �r�b�g�}�b�v
	RGBQUAD*			m_pRgb;			///< DIB �A�h���X
	SIZE				m_size;			///< �T�C�Y
	HWND				m_hWnd;			///< ���[�N�p
	HDC					m_backDC;		///< GetDC �p
	HGDIOBJ				m_backBmp;		///< GetDC �p
	int					m_nStretchMode;	///< Stretch���[�h

	/// ���[�N�p�f�o�C�X�R���e�L�X�g�Ǘ�
	class CWorkDC
	{
		HWND	m_hWnd;
		HDC		m_dc;
	public:
		CWorkDC(HWND hWnd) : m_hWnd(hWnd), m_dc(::GetDC(hWnd))
		{
			ASSERT0( m_dc != NULL, "GetDC", "���s���܂����B" );
		}
		~CWorkDC(void)
		{
			if ( ! ::ReleaseDC(m_hWnd, m_dc) ) { ASSERT0( false, "ReleaseDC", "���s���܂����B" ); }
		}
		operator HDC(void) { return m_dc; }
	};
	/// �e���|�����f�o�C�X�R���e�L�X�g�Ǘ�
	class CTempDC
	{
		HDC		m_dc;
		HGDIOBJ	m_bmp;
	public:
		CTempDC(HDC dc, HBITMAP bmp) : m_dc(::CreateCompatibleDC(dc)), m_bmp(::SelectObject(m_dc, bmp))
		{
			ASSERT0( m_dc != NULL, "CreateCompatibleDC", "���s���܂����B" );
			ASSERT0( m_bmp != NULL, "SelectObject", "���s���܂����B" );
		}
		~CTempDC(void)
		{
			if ( ::SelectObject(m_dc, m_bmp) == NULL ) { ASSERT0( false, "SelectObject", "���s���܂����B" ); }
			if ( ! ::DeleteDC(m_dc) ) { ASSERT0( false, "DeleteDC", "���s���܂����B" ); }
		}
		operator HDC(void) { return m_dc; }
	};
	/// �e���|�����r�b�g�}�b�v�f�o�C�X�R���e�L�X�g�Ǘ�
	class CTempBitmapDC
	{
		CWorkDC	m_workDC;
		CTempDC	m_tempDC;
	public:
		CTempBitmapDC(HWND hWnd, HBITMAP bmp) : m_workDC(hWnd), m_tempDC(m_workDC, bmp) { }
		operator HDC(void) { return m_tempDC; }
	};

	/// �y�C���g���
	struct TPaintInfo
	{
		DWORD			count;		///< �h������
		SIZE			size;		///< �r�b�g�}�b�v�T�C�Y
		RGBQUAD*		pRgb;		///< �r�b�g�}�b�v
		RGBQUAD			rgbBase;	///< RGB�x�[�X�i�h����F�j
		RGBQUAD			rgb;		///< RGB�i�h��F�j
		/**
		 * [��r] RGB�x�[�X�Ɠ����H
		 *	@param q RGB
		 *	@retval true ����
		 *	@retval false �قȂ�
		 */
		bool IsEqualBase(const RGBQUAD& q) const
		{
			if ( rgbBase.rgbBlue != q.rgbBlue ) return false;
			if ( rgbBase.rgbRed != q.rgbRed ) return false;
			if ( rgbBase.rgbGreen != q.rgbGreen ) return false;
			return true;
		}
		/**
		 * [��r] RGB�x�[�X�Ɠ����H
		 *	@param x X���W
		 *	@param y Y���W
		 *	@retval true ����
		 *	@retval false �قȂ�
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

	//�������ق���I��
	static size_t ms_ChoiceMin(size_t a, size_t b)
	{
		return (a < b) ? a : b;
	}
	// Bitmap�w�b�_�ݒ�
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
	// ��̃r�b�g�}�b�v�쐬
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
	// RECT �� SIZE 
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
	// �����[�X
	void m_ReleaseDC(void)
	{
		if ( m_backDC != NULL )
		{
			if ( ::SelectObject(m_backDC, m_backBmp) == NULL )
			{
				ASSERT0( false, "SelectObject", "���s���܂����B" );
			}
			if ( ! ::DeleteDC(m_backDC) )
			{
				ASSERT0( false, "DeleteDC", "���s���܂����B" );
			}
			m_backDC = NULL;
		}
	}
	/// �����̃J���[�擾
	COLORREF m_CheckColor(HDC hdc, COLORREF color) const
	{
		if ( color == CLR_AUTOSELECT )
		{
			color = ::GetPixel(hdc, 0, m_size.cy - 1);
		}
		return color;
	}
	//�����o�ɋL��
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

	/// �\�[�g����֐�. @note s_ExpansionContraction() �p
	static int ms_DeSub(const void* a, const void* b)
	{
		return *(BYTE*)a - *(BYTE*)b;
	}

	/// X���W�̒l�擾. @note s_ExpansionContraction() �p
	static inline BYTE ms_GG(const RGBQUAD* p, int cx, int x)
	{
		return (x < 0 || x >= cx) ? static_cast<BYTE>(0) : p[x].rgbGreen;
	}

	/// �ő�l�擾
	static inline BYTE ms_max(size_t len, BYTE* p)
	{
		BYTE r = p[0];
		loop ( i, len - 1 )
		{
			if ( r < p[i + 1] ) r = p[i + 1];
		}
		return r;
	}

	/// �ŏ��l�擾
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
	 * [����] �c��/���k����.
	 *	@param isDilation true�Ȃ�c�� false�Ȃ���k.
	 *	@return �r�b�g�}�b�v
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
		//�_�~�[���C���쐬
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
	 * [����] �y�C���g�T�u.
	 *	@param x X���W
	 *	@param y Y���W
	 *	@param p �y�C���g���
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

	/// m_pRgb ���L�����`�F�b�N���ANG�Ȃ�p�ӂ���B
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

