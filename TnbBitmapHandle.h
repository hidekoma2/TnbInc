#pragma once
/**
 *@file
 * �r�b�g�}�b�v�n���h���֌W�̃w�b�_
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		��� HBITMAP ���n���h�����܂��B auto_ptr �N���X�̂悤�Ȃ��̂ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// HBITMAP�n���h���j���N���X
struct TPhDeleteBitmapHandle
{
	void operator()(HBITMAP h) 
	{
	  #ifdef _DEBUG
		if ( ::GetObjectType(h) == NULL )
		{
			ASSERT0( false, "~CBitmapHandle", "��� HBITMAP���j������Ă��܂��Ă��܂��B" );
		}
		else if ( ! ::DeleteObject(h) )
		{
			ASSERT1( false, "~CBitmapHandle", "HBITMAP �̔j���Ɏ��s���܂����B\ncode = %d", ::GetLastError() );
		}
	  #else
		::DeleteObject(h);
	  #endif
	}
};

#endif // _TnbDOXYGEN



/**@ingroup BITMAP
 * HBITMAP�^�n���h���n���h��
 *
 *		HBITMAP �A����Ǘ����܂��B�Q�Ƃ���{�C���X�^���X���Ȃ��Ȃ�ƁA�����I�� DeleteObject() ���܂��B
 *
 *	@par�|�C���g
 *		�{�N���X�������Ɏ��֐��̏ꍇ�A HBITMAP , CBitmapHandle �ȊO�ɁA
 *		BITMAP�t�@�C�����i������j�A BITMAP ���\�[�XID ���w�肷�邱�Ƃ��ł��܂��B
 *		<table><tr><td><pre>
 *
 *		//= �錾
 *		void Foo(CBitmapHandle bmp); // ���������֐��̏ꍇ�E�E�E
 *
 *		//= �g�p��
 *		Foo("c:\\BMP\\BackTop1.bmp");  // BackTop1.bmp�t�@�C����ǂݍ���� Foo() ���R�[�����܂��B
 *		Foo(IDB_BACKTOP1");  // ���\�[�X�� IDB_BACKTOP1 ��ǂݍ���� Foo() ���R�[�����܂��B
 *		</pre></td></tr>
 *
 *	@par����
 *		��x�{�N���X�ŊǗ�������A HBITMAP �Ƃ��Ď��o�������̂��ꎞ�I�ɈȊO�ɂ͎g�p���Ȃ��ł��������B
 *		���� HBITMAP ����Ă̕����͖�肪�������܂��B�{�N���X�̊Ǘ�������͂����ꍇ�A Detach() ���g�p���܂��B
 *		<table><tr><td><pre>
 *
 *		CBitmapHandle bh(IDB_PIC1);   // ���\�[�X����Ǎ���
 *		HBITMAP h1 = bh;              // bh �ŊǗ����Ă��� HBITMAP ���Q�ƁB
 *		{
 *			CBitmapHandle temp = h1;  // �n���h�����L��
 *		}                             // �f�X�g���N�^���� h1 �̃n���h����j���B		
 *		HBITMAP h2 = bh;              // bh �ŊǗ����Ă��� HBITMAP ���Q�ƁB
 *		                              // ������ temp �̃f�X�g���N�^���� �j������Ă���̂ŁA
 *		                              // �����ł͖����ȃn���h���� h2 �ɑ�������B
 *		</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapHandle.h
 *
 *	@date 06/09/29 �e���v���[�g���B
 *	@date 07/11/03 �p�����A�����o�ǉ��B
 *	@date 07/11/12 �����Ǘ������݂�Desktop�ݒ�̉e�����󂯂Ȃ��悤�ɏC���B
 *	@date 07/11/13 Save() �ǉ��B
 *	@date 07/11/15 SetClone() �ǉ��B
 *	@date 08/04/07 LoadOem() �ǉ��B �t�@�C����Load���鎞���A INSTANCE �n���h�����w�肷��悤�ɂ����B
 *	@date 08/07/02 Create32bitDibSection() �ǉ��BCE�Ή��B
 *	@date 10/09/15 SaveMemory() �ǉ�
 */
class CBitmapHandle : public CPointerHandleBaseT<HBITMAP, TPhDeleteBitmapHandle, NULL> 
{
	typedef CPointerHandleBaseT<HBITMAP, TPhDeleteBitmapHandle, NULL> _super;	///< �e�N���X���Đ錾
	DEFSUPER(_super);
public:

	/**
	 * �R���X�g���N�^.
	 *		NULL��Ԃ̃r�b�g�}�b�v�iHBITMAP ���Ǘ����Ă��Ȃ�)�I�u�W�F�N�g���쐬����܂��B
	 */
	CBitmapHandle(void) : _super() 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *		�R�s�[���ɗL���� HBITMAP������ꍇ�A�{�C���X�^���X�ł��A
	 *		�������Q�Ƃ���悤�ɂȂ�܂��B
	 *	@param other �R�s�[��
	 */
	CBitmapHandle(const CBitmapHandle& other) : _super(other) 
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		HBITMAP ��ێ�����I�u�W�F�N�g���쐬����܂��B
	 *	@param P HBITMAP�B
	 */
	CBitmapHandle(HBITMAP P) : _super(P) 
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w��r�b�g�}�b�v���\�[�X��ǂݍ��݁A�������ێ�����I�u�W�F�N�g���쐬����܂��B
	 *		�ǂݍ��ݎ��s�����ꍇ�ANULL��Ԃ̃r�b�g�}�b�v�I�u�W�F�N�g���쐬����܂��B
	 *	@note �C���X�^���X�n���h���́A GetInstanceHandle(EI_Bitmap) �̒l���g�p����܂��B
	 *	@param uBitmapResourceId �r�b�g�}�b�v���\�[�XID
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 */
	CBitmapHandle(UINT uBitmapResourceId, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		_super::operator=(Load(uBitmapResourceId, cx, cy, fuLoad));
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w��t�@�C����ǂݍ��݁A�������ێ�����I�u�W�F�N�g���쐬����܂��B
	 *		�t�@�C���ǂݍ��ݎ��s�����ꍇ�ANULL��Ԃ̃r�b�g�}�b�v�I�u�W�F�N�g���쐬����܂��B
	 *	@param lpszFile �t�@�C�����B
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 */
	CBitmapHandle(LPCTSTR lpszFile, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		_super::operator=(Load(lpszFile, cx, cy, fuLoad));
	}

	#ifdef __AFX_H__
	/**
	 * ����R���X�g���N�^.
	 *		�w��t�@�C����ǂݍ��݁A�������ێ�����I�u�W�F�N�g���쐬����܂��B
	 *		�t�@�C���ǂݍ��ݎ��s�����ꍇ�ANULL��Ԃ̃r�b�g�}�b�v�I�u�W�F�N�g���쐬����܂��B
	 *	@param strFile �t�@�C�����B
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 */
	CBitmapHandle(const CString& strFile, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		_super::operator=(Load(strFile.operator LPCTSTR(), cx, cy, fuLoad));
	}
	#endif

	/**
	 * [�擾] �r�b�g�}�b�v�T�C�Y�擾. 
	 *		�{�I�u�W�F�N�g���ێ����Ă���r�b�g�}�b�v�̑傫����Ԃ��܂��B
	 *	@param[out] _size �T�C�Y�i�[�B
	 *	@retval true �����B
	 *	@retval false ���s�B�ێ����Ă��܂���B
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
	 * [�擾] �f�^�b�`.
	 *		�{�C���X�^���X���j�����Ă��n���h����j�����Ȃ��悤�Ɏ��o���܂��B
	 *		�{�C���X�^���X�� NULL��ԂɂȂ�܂��B
	 *	@retval NULL �n���h���������Ă��Ȃ��B
	 *	@retval NULL�ȊO HBITMAP�B �g�p��́A�����I�� ::DeleteObject() ���Ă��������B
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
	 * [�ݒ�] HBITMAP�Z�b�g.
	 *		�w��� HBITMAP �̕�����{�I�u�W�F�N�g�ŊǗ����܂��B
	 *	@note HBITMAP �̔j���̃^�C�~���O�� CBitmapHandle �ɊǗ����������Ȃ����A�g�p���܂��B
	 *	@param hBmp HBITMAP�B
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
		HBITMAP h = *this;
		return Draw(hdc, h, x, y);
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
		return Save(lpszFileName, *this, bitsPixel);
	}


	//-----------------


	/**
	 * [�쐬] 32bit�r�b�g�}�b�v�쐬.
	 *	@note ���̃r�b�g�}�b�v�̑傫���A�G������ DIB�Z�N�V�����̃r�b�g�}�b�v���쐬���܂��B
	 *	@param[in] hbm ���̃r�b�g�}�b�v�B
	 *	@param[out] _pRgb �r�b�g�}�b�v�̃A�h���X���Ԃ�܂��B
	 *	@param[out] pHeader �w�b�_��񂪕Ԃ�܂��B�ȗ��\�B
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO �r�b�g�}�b�v�n���h���B �g�p��́A�����I�� ::DeleteObject() ���Ă��������B
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
	 * [�Ǎ�] ���\�[�X�Ǎ���
	 *	@note �C���X�^���X�n���h���́A GetInstanceHandle(EI_Bitmap) �̒l���g�p����܂��B
	 *	@param uBitmapResourceId �r�b�g�}�b�v���\�[�XID
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO �r�b�g�}�b�v�n���h���B �g�p��́A�����I�� ::DeleteObject() ���Ă��������B
	 */
	static HBITMAP Load(UINT uBitmapResourceId, int cx = 0, int cy = 0, UINT fuLoad = LR_CREATEDIBSECTION)
	{
		HINSTANCE hIns = GetInstanceHandleByTnb(EI_Bitmap);
		LPCTSTR lpId = MAKEINTRESOURCE(uBitmapResourceId);
		return static_cast<HBITMAP>(::LoadImage(hIns, lpId, IMAGE_BITMAP, cx, cy, fuLoad));
	}

	/**
	 * [�Ǎ�] BMP�t�@�C���Ǎ���.
	 *	@param lpszFileName �t�@�C�����B
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA�t�@�C���̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO �r�b�g�}�b�v�n���h���B �g�p��́A�����I�� ::DeleteObject() ���Ă��������B
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
	 * [�Ǎ�] OEM���\�[�X�Ǎ���
	 *	@param uOemBitmapResourceId OEM�r�b�g�}�b�v���\�[�XID
	 *	@param cx �ǂݍ���X�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param cy �ǂݍ���Y�T�C�Y���w��ł��܂��B�ȗ�����ƁA���\�[�X�̏��̂܂܂ɂȂ�܂��B
	 *	@param fuLoad �Ǎ��݃I�v�V�������w��ł��܂��B�ȗ��\�B\n
	 *			-> http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpwinui/html/_win32_loadimage.asp
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO �r�b�g�}�b�v�n���h���B �g�p��A ::DeleteObject() ����K�v�͂���܂���B
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
	 * [����] �C���[�W�`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̈ʒu�ɁA�w��̃C���[�W�r�b�g�}�b�v��`�悵�܂��B
	 *	@param hdc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param hBmp �r�b�g�}�b�v�B
	 *	@param x �`��X�ʒu(����0)�B�ȗ�����ƂO�B
	 *	@param y �`��Y�ʒu(�オ0)�B�ȗ�����ƂO�B
	 *	@retval true �����B
	 *	@retval false ���s�B
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
	 * [�쐬] BMP�t�@�C���C���[�W�쐬.
	 *	@note CE �ł� bitsPixel �͖�������K���R�Q�ɂȂ�܂��B
	 *	@param[out] _save �ۑ���B
	 *	@param[in] bmp �r�b�g�}�b�v
	 *	@param[in] bitsPixel ��h�b�g�̃r�b�g���B1,4,8,16,24,32 �̂����ꂩ���w��ł��܂��B 0�Ȃ� �r�b�g�}�b�v�̏��܂܂ɂȂ�܂��B 
	 *	@retval ERROR_SUCCESS �����B
	 *	@retval ��L�ȊO ���s�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	#ifndef _WIN32_WCE
	static DWORD SaveMemory(CWorkMem& _save, HBITMAP bmp, WORD bitsPixel = 0)
	{
		BITMAP bp;
		if ( ::GetObject(bmp, sizeof(BITMAP), &bp) <= 0 ) 
		{
			return _GetLastError("GetObject"); 
		}
		// BITMAP��񐶐�
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
		// �摜�f�[�^�o��
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
		// �w�b�_
		LPCVOID P1 = &fh;
		size_t l1 = sizeof(fh);
		// BITMAP���
		LPCVOID P2 = pih;
		size_t l2 = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * paletCount;
		// �摜�f�[�^
		LPCVOID P3 = data.Ref();
		size_t l3 = pih->biSizeImage;
		// �o��
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
		// �w�b�_
		LPCVOID P1 = &fh;
		size_t l1 = sizeof(fh);
		// BITMAP���
		LPCVOID P2 = &header;
		size_t l2 = sizeof(header);
		// �摜�f�[�^
		LPCVOID P3 = pRgb;
		size_t l3 = len;
		// �o��
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
	 * [�쐬] BMP�t�@�C���쐬.
	 *	@note CE �ł� bitsPixel �͖�������K���R�Q�ɂȂ�܂��B
	 *	@param lpszFileName �t�@�C�����B
	 *	@param bmp �r�b�g�}�b�v
	 *	@param bitsPixel ��h�b�g�̃r�b�g���B1,4,8,16,24,32 �̂����ꂩ���w��ł��܂��B 0�Ȃ� �r�b�g�}�b�v�̏��܂܂ɂȂ�܂��B 
	 *	@retval ERROR_SUCCESS �����B
	 *	@retval ��L�ȊO ���s�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	static DWORD Save(LPCTSTR lpszFileName, HBITMAP bmp, WORD bitsPixel = 0)
	{
		CWorkMem m;
		DWORD r = SaveMemory(m, bmp, bitsPixel);
		if ( r != ERROR_SUCCESS )
		{
			return r;
		}
		// �t�@�C���o�̓I�[�v��
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
