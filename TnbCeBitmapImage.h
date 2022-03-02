#pragma once
/**
 *@file
 * CE��p �r�b�g�}�b�v�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeBitmapImage.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include <initguid.h>
#include <imaging.h>
#include "TnbBitmapImage.h"
#include "TnbStr.h"



//TNB Library
namespace TNB{



/**@ingroup WINCE BITMAP
 * �r�b�g�}�b�v�Ǘ�(CE��p)
 *
 *		���̃N���X�́A CE ���L�̃r�b�g�}�b�v�֌W���T�|�[�g������̂ł��B
 *
 *		�F�X�ȃr�b�g�}�b�v�̃Z�[�u�A���[�h���T�|�[�g���Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCeBitmapImage.h
 *
 *	@date 09/08/11 �V�K�쐬
 *	@date 09/08/17 Save���A�㉺���]���܂��Ă����̂��C���B
 */
class CCeBitmapImage
{
public:

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
	 * [�쐬] �摜�t�@�C���Ǎ���.
	 *		BMP, JPEG �Ȃǂ̃t�@�C����ǂݍ��݁A HBITMAP ���쐬���܂��B
	 *	@param lpszFileName �t�@�C�����B
	 *	@retval NULL�ȊO �����B
	 *	@retval NULL ���s�B
	 */
	static HBITMAP LoadBitmap(LPCTSTR lpszFileName)
	{
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		CBitmapHandle bmp;
		IImagingFactory* pFactory = NULL;
		IImage* pImage = NULL;
		IBitmapImage* pBitmap = NULL;
		ImageInfo info;
		HRESULT hr = ::CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER
							, IID_IImagingFactory, reinterpret_cast<LPVOID*>(&pFactory));
		if ( SUCCEEDED(hr) )
		{
			hr = pFactory->CreateImageFromFile(CUnicode(lpszFileName), &pImage);
		}
		if ( SUCCEEDED(hr) )
		{
			pImage->GetImageInfo(&info);
			hr = pFactory->CreateBitmapFromImage(pImage, info.Width, info.Height
							, PixelFormat24bppRGB, InterpolationHintDefault, &pBitmap);
		}
		if ( pImage != NULL )
		{
			pImage->Release();
			pImage = NULL;
		}
		if ( pFactory != NULL )
		{
			pFactory->Release();
			pFactory = NULL;
		}
		BitmapData lockdata;
		if ( SUCCEEDED(hr) )
		{
			hr = pBitmap->LockBits(NULL, ImageLockModeRead, PixelFormat32bppARGB, &lockdata);
		}
		if ( SUCCEEDED(hr) )
		{
			SIZE sz = { info.Width, info.Height };
			CBitmapImage::CRawData raw(sz);
			const RGBQUAD* pSrc = static_cast<const RGBQUAD*>(lockdata.Scan0);
			loop ( y, info.Height )
			{
				RGBQUAD* pDst = raw[y];
				loop ( x, info.Width )
				{
					*pDst++ = *pSrc++;
				}
			}
			pBitmap->UnlockBits(&lockdata);
			CBitmapImage bi;
			bi.Set(raw);
			bmp = bi.GetBitmapHandle();
		}
		if ( pBitmap != NULL )
		{
			pBitmap->Release();
			pBitmap = NULL;
		}
		return bmp.Detach();
	}

	/**
	 * [�쐬] �摜�t�@�C���쐬.
	 *		�r�b�g�}�b�v�� BMP, JPEG �Ȃǂ̃t�@�C���ɂ��܂��B
	 *	@param lpszFileName �t�@�C�����B
	 *	@param hBmp �r�b�g�}�b�v�B
	 *	@param fmt �t�H�[�}�b�g�B�ȗ������BMP�ł��B
	 *	@param quality JPEG�̃N�I���e�B(0�`100)�B�ȗ������80�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool SaveBitmap(LPCTSTR lpszFileName, HBITMAP hBmp, EBitmapFormat fmt = BMP, long quality = 80)
	{
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		CBitmapHandle bb;
		bb.SetClone(hBmp);
		CBitmapImage bm = bb;
		if ( bm.IsEmpty() )
		{
			return false;
		}
		::DeleteFile(lpszFileName);
		IImagingFactory* pFactory = NULL;
		IImageEncoder* pEncoder = NULL;
		IImageSink* pImageSink = NULL;
		HRESULT hr = ::CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER
							, IID_IImagingFactory, reinterpret_cast<LPVOID*>(&pFactory));
		if ( SUCCEEDED(hr) )
		{
			ImageCodecInfo* pCodecInfo = NULL;
			UINT count;
			hr = pFactory->GetInstalledEncoders(&count, &pCodecInfo);
			if ( SUCCEEDED(hr) )
			{
				LPCWSTR lpFmtDes = NULL;
				switch( fmt )
				{
				case BMP:
				default:	lpFmtDes = L"BMP";	break;
				case GIF:	lpFmtDes = L"GIF";	break;
				case TIFF:	lpFmtDes = L"TIFF";	break;
				case PNG:	lpFmtDes = L"PNG";	break;
				case JPEG:	lpFmtDes = L"JPEG";	break;
				}
				CLSID encoderClassId;
				loop ( i, count ) 
				{
					if ( STRLIB::Compare(pCodecInfo[i].FormatDescription, lpFmtDes) == 0 )
					{
						encoderClassId = pCodecInfo[i].Clsid;
						break;
					}
				}
				ImageFormatJPEG;
				hr = pFactory->CreateImageEncoderToFile(&encoderClassId, CUnicode(lpszFileName), &pEncoder);
				free(pCodecInfo);
			}
		}
		if ( SUCCEEDED(hr) && fmt == JPEG )
		{
			EncoderParameters ep;
			ep.Count = 1;
			ep.Parameter[0].Guid = EncoderQuality ;
			ep.Parameter[0].Type = 4/*EncoderParameterValueTypeLong*/;
			ep.Parameter[0].NumberOfValues = 1;
			ep.Parameter[0].Value = &quality;	//JPEG�N�I���e�B�F0�`100
			hr = pEncoder->SetEncoderParameters(&ep);
		}
		if ( SUCCEEDED(hr) )
		{
			hr = pEncoder->GetEncodeSink(&pImageSink);
		}
		if ( SUCCEEDED(hr) )
		{
			ImageInfo imageInfo;
			imageInfo.Width = bm.GetSize().cx;
			imageInfo.Height = bm.GetSize().cy;
			imageInfo.RawDataFormat = ImageFormatMemoryBMP;
			imageInfo.Flags |= SinkFlagsTopDown | SinkFlagsFullWidth;
			imageInfo.Xdpi = 96;
			imageInfo.Ydpi = 96;
			imageInfo.PixelFormat = PixelFormat32bppARGB;
			imageInfo.Flags |= SinkFlagsHasAlpha;
			hr = pImageSink->BeginSink(&imageInfo, NULL);
		}
		if ( SUCCEEDED(hr) )
		{
			ColorPalette pal = { 0 };
			hr = pImageSink->SetPalette(&pal);
		}
		if ( SUCCEEDED(hr) )
		{
			BitmapData bmpData;
			bmpData.Height = bm.GetSize().cy;
			bmpData.Width = bm.GetSize().cx;
			CBitmapImage::CRawData raw = bm.CreateRawData();
			{
				// �㉺���]
				size_t h = bm.GetSize().cy;
				size_t w = bm.GetSize().cx;
				CWorkMemT<RGBQUAD> buf(w);
				size_t j = h - 1;
				loop ( i, h / 2 )
				{
					MemCopy(buf.Ref(), raw[i], w);
					MemCopy(raw[i], raw[j], w);
					MemCopy(raw[j], buf.Ref(), w);
					j--;
				}
			}
			bmpData.Scan0 =raw.Refer();
			bmpData.PixelFormat = PixelFormat32bppARGB;
			UINT bitsPerLine = bm.GetSize().cx * 4 * 8;
			UINT bitAlignment = sizeof(LONG) * 8;
			UINT bitStride = bitAlignment * (bitsPerLine / bitAlignment);
			if ( (bitsPerLine % bitAlignment) != 0 )
			{
				bitStride += bitAlignment;
			}
			bmpData.Stride = bitStride / 8;
			RECT rect = { 0, 0, bmpData.Width, bmpData.Height };
			hr = pImageSink->PushPixelData(&rect, &bmpData, TRUE);
			pImageSink->EndSink(S_OK);
			pImageSink->Release();
		}
		if ( pEncoder != NULL )
		{
			pEncoder->TerminateEncoder();
			pEncoder->Release();
			pEncoder = NULL;
		}
		if ( pFactory != NULL )
		{
			pFactory->Release();
			pFactory = NULL;
		}
		bm.Detach();
		return true;
	}
};



};//TNB


