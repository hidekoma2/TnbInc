#pragma once
/**
 *@file
 * �t�H���g�n���h���֌W�̃w�b�_
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		��� HFONT ���n���h�����܂��B auto_ptr �N���X�̂悤�Ȃ��̂ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// HFONT�n���h���j���N���X
struct TPhDeleteFontHandle
{
	void operator()(HFONT h) 
	{
	  #ifdef _DEBUG
		if ( ::GetObjectType(h) == NULL )
		{
			ASSERT0( false, "~CFontHandle", "��� HFONT���j������Ă��܂��Ă��܂��B" );
		}
		else if ( ! ::DeleteObject(h) )
		{
			ASSERT1( false, "~CFontHandle", "HFONT �̔j���Ɏ��s���܂����B\ncode = %d", ::GetLastError() );
		}
	  #else
		::DeleteObject(h);
	  #endif
	}
};

#endif // _TnbDOXYGEN



/**@ingroup WINAPI
 * HFONT�^�n���h���n���h��
 *
 *		HFONT �A����Ǘ����܂��B�Q�Ƃ���{�C���X�^���X���Ȃ��Ȃ�ƁA�����I�� DeleteObject() ���܂��B
 *
 *	@par����
 *		��x�{�N���X�ŊǗ�������A HFONT �Ƃ��Ď��o�������̂��ꎞ�I�ɈȊO�ɂ͎g�p���Ȃ��ł��������B
 *		���� HFONT ����Ă̕����͖�肪�������܂��B�{�N���X�̊Ǘ�������͂����ꍇ�A Detach() ���g�p���܂��B
 *		\code
 *		CFontHandle fh;
 *		fh.Set(90, "tahoma");
 *		HFONT h1 = fh;				// fh �ŊǗ����Ă��� HFONT ���Q�ƁB
 *		{
 *			CFontHandle temp = h1;	// �n���h�����L��
 *		}							// �f�X�g���N�^���� h1 �̃n���h����j���B		
 *		HFONT h2 = fh;				// bh �ŊǗ����Ă��� HFONT ���Q�ƁB
 *									// ������ temp �̃f�X�g���N�^���� �j������Ă���̂ŁA
 *									// �����ł͖����ȃn���h���� h2 �ɑ�������B
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbFontHandle.h
 *
 *	@date 10/05/13	�V�K�쐬�B
 *	@date 11/08/16	�L�����Z�b�g�� DEFAULT_CHARSET ���� SHIFTJIS_CHARSET �ցi������ SJIS�v���W�F�N�g�̏ꍇ�j
 *	@date 13/02/13	NONCLIENTMETRICS �\���̂� WINVER 0x600 ����傫���Ȃ��Ă�����ɑΏ�
 *	@date 14/03/28	SetToWindow() ��V�K�ǉ��B
 */
class CFontHandle : public CPointerHandleBaseT<HFONT, TPhDeleteFontHandle, NULL> 
{
	typedef CPointerHandleBaseT<HFONT, TPhDeleteFontHandle, NULL> _super;	///< �e�N���X���Đ錾
	DEFSUPER(_super);
public:

	/**
	 * �R���X�g���N�^.
	 *		NULL��Ԃ̃r�b�g�}�b�v�iHFONT ���Ǘ����Ă��Ȃ�)�I�u�W�F�N�g���쐬����܂��B
	 */
	CFontHandle(void) : _super() 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *		�R�s�[���ɗL���� HFONT������ꍇ�A�{�C���X�^���X�ł��A���� HFONT ���Q�Ƃ���悤�ɂȂ�܂��B
	 *	@param other �R�s�[��
	 */
	CFontHandle(const CFontHandle& other) : _super(other) 
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w��̃t�H���g�n���h����ێ�����I�u�W�F�N�g���쐬����܂��B 
	 *		�w�肵���n���h���͖{�C���X�^���X���Ǘ����܂��B�j�����Ȃ��ł��������B
	 *	@param hFont �t�H���g�n���h���B
	 */
	CFontHandle(HFONT hFont) : _super(hFont) 
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w��̑�������t�H���g���쐬���A�����{�C���X�^���X�ŊǗ����܂��B
	 *	@param attr �t�H���g����.
	 */
	CFontHandle(const LOGFONT& attr)
	{
		HFONT f = ::CreateFontIndirect(&attr);
		_super::operator=(f);
	}

	/**
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�w��̑�������t�H���g���쐬���A�����{�C���X�^���X�ŊǗ����܂��B
	 *	@param attr �t�H���g����.
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool Set(const LOGFONT& attr)
	{
		HFONT f = ::CreateFontIndirect(&attr);
		_super::operator=(f);
		return ! _super::IsNull();
	}

	/**
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�w��̑�������t�H���g���쐬���A�����{�C���X�^���X�ŊǗ����܂��B
	 *	@note �����iattr.lfHeoght�j�� 1/10 �|�C���g�P�ʂŉ��߂���܂��B
	 *	@param attr �t�H���g����.
	 *	@param dc ������_���P�ʂɕϊ����邽�߂Ɏg�p����f�o�C�X�R���e�L�X�g�B NULL ���w�肷��Ɖ�ʃf�o�C�X���g���܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
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
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�w��̃T�C�Y�ƃt�H���g������t�H���g���쐬���A�����{�C���X�^���X�ŊǗ����܂��B
	 *	@param pointSize �t�H���g�T�C�Y�i1/10 �|�C���g�P�ʁj.
	 *	@param lpszFaceName �t�H���g���B
	 *	@param dc ������_���P�ʂɕϊ����邽�߂Ɏg�p����f�o�C�X�R���e�L�X�g�B NULL ���w�肷��Ɖ�ʃf�o�C�X���g���܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
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
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�w��̃E�B���h�E�ɐݒ肳��Ă���t�H���g��ǂݏo���A������{�C���X�^���X�ŊǗ����܂��B
	 *	@param hWnd �E�B���h�E�n���h���B
	 *	@param mul �{��
	 *	@retval true ����.
	 *	@retval false ���s.
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
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�w��̃t�H���g�𑾕����ɂ����t�H���g���쐬���A�{�C���X�^���X�ŊǗ����܂��B
	 *	@param hFont �t�H���g�n���h��
	 *	@retval true ����.
	 *	@retval false ���s.
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
	 * [�ݒ�] �t�H���g�ݒ�.
	 *		�w��̃t�H���g�n���h���𕡐��������̂�{�I�u�W�F�N�g�ŊǗ����܂��B
	 *	@param hFont �t�H���g�B
	 *	@param mul �{��
	 *	@retval true ����.
	 *	@retval false ���s.
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
	 * [�ݒ�] �L���v�V�����o�[�t�H���g�ݒ�.
	 *		�L���v�V�����o�[�Ɠ����t�H���g���쐬���A�{�C���X�^���X�ŊǗ����܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetSystemCaptionFont(void)
	{
		NONCLIENTMETRICS ncm;
		VERIFY( ms_GetNonClientMetrics(ncm) );
		#ifndef _UNICODE
			ncm.lfCaptionFont.lfCharSet = SHIFTJIS_CHARSET;
		#endif
		return Set(ncm.lfCaptionFont);  //�^�C�g���o�[��LOGFONT
	}

	/**
	 * [�ݒ�] ���j���[�t�H���g�ݒ�.
	 *		���j���[�Ɠ����t�H���g���쐬���A�{�C���X�^���X�ŊǗ����܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetSystemMenuFont(void)
	{
		NONCLIENTMETRICS ncm;
		VERIFY( ms_GetNonClientMetrics(ncm) );
		#ifndef _UNICODE
			ncm.lfMenuFont.lfCharSet = SHIFTJIS_CHARSET;
		#endif
		return Set(ncm.lfMenuFont);  //���j���[��LOGFONT
	}

	#endif //_WIN32_WCE

	/**
	 * [�ݒ�] �f�^�b�`.
	 *		�{�C���X�^���X���j�����Ă��n���h����j�����Ȃ��悤�Ɏ��o���܂��B
	 *		�{�C���X�^���X�� NULL��ԂɂȂ�܂��B
	 *	@retval NULL �n���h���������Ă��Ȃ��B
	 *	@retval NULL�ȊO HFONT�B �g�p��́A�����I�� ::DeleteObject() ���Ă��������B
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
	 * [�擾] �����擾.
	 *		�{�C���X�^���X���Ǘ����Ă���t�H���g�̑�����Ԃ��܂��B
	 *	@param[out] _attr �擾�����������i�[����܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool GetAttributes(LOGFONT& _attr) const
	{
		HFONT f = *this;
		return GetAttributes(_attr, f);
	}

	/**
	 * [�擾] �E�B���h�E�֐ݒ�.
	 *		�{�C���X�^���X���Ǘ����Ă���t�H���g���E�B���h�E�ɐݒ肵�܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 *	@param isRedraw true �Ȃ炷���ɍĕ`��
	 */
	void SetToWindow(HWND hWnd, bool isRedraw = true) const
	{
		ASSERT(::IsWindow(hWnd));
		HFONT f = *this;
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)f, isRedraw);
	}

	/**
	 * [�擾] �����擾.
	 *		�w��̃t�H���g�̑�����Ԃ��܂��B
	 *	@param[out] _attr �擾�����������i�[����܂��B
	 *	@param[in] hFont �t�H���g�B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	static bool GetAttributes(LOGFONT& _attr, HFONT hFont)
	{
		return ::GetObject(hFont, sizeof(LOGFONT), &_attr) != 0;
	}

private:

	#ifndef _WIN32_WCE

	/// SystemParametersInfo ���s
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
