#pragma once
/**
 *@file
 * AVI関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapHandle.h"
#include <vfw.h>
#pragma comment(lib, "Vfw32.lib")



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * AVIメーカークラス.
 *
 *		BITMAPを指定し、AVIファイルを作成します。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *
 *		CAviMaker avi;
 *		avi.Open(CSize(48, 48), "c:\\aa.avi", 1, 10);	//1秒間に10枚		
 *		avi.Add(IDB_FRAME1);
 *		avi.Add(IDB_FRAME2);
 *		avi.Add(IDB_FRAME3);
 *		avi.Stop();
 *
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbAviMaker.h
 *
 *	@date 07/11/01 新規作成
 */
class CAviMaker : CCopyImpossible
{
	BITMAPINFOHEADER	m_bmpinfo;				///< BITMAP情報
	PAVISTREAM			m_pSourceStream;		///< ソースストリーム
	PAVISTREAM			m_pCompressedStream;	///< 圧縮ストリーム
	PAVIFILE			m_pAviFile;				///< AVIファイル管理
	COMPVARS			m_compvars;				///< COMPVARS
	UINT				m_index;				///< インデックス
public:

	/// コンストラクタ
	CAviMaker(void) : m_pSourceStream(NULL), m_pCompressedStream(NULL), m_pAviFile(NULL), m_index(0)
	{
		m_compvars.cbSize = 0;
	}

	/// デストラクタ
	virtual ~CAviMaker()
	{
		Close();
	}

	/**
	 * [停止] ストップ. 
	 *	@note 作成中の場合、終了し、AVIファイルを作成します。
	 */
	void Close(void)
	{
		if ( m_pCompressedStream != NULL )
		{
			::AVIStreamRelease(m_pCompressedStream);
			m_pCompressedStream = NULL;
		}
		if ( m_pSourceStream != NULL )
		{
			::AVIStreamRelease(m_pSourceStream);
			m_pSourceStream = NULL;
		}
		if (m_pAviFile!=NULL) {
			::AVIFileRelease(m_pAviFile);
			m_pAviFile=NULL;
		}
		if ( m_compvars.cbSize != 0 )
		{
			::ICCompressorFree(&m_compvars);
			m_compvars.cbSize = 0;
		}
		m_index = 0;
		::AVIFileExit();
	}

	/**
	 * [開始] オープン. 
	 *	@note 作成中の場合、終了し、AVIファイルを作成します。
	 *	@param size AVIファイルサイズ。 Addで追加する ビットマップのサイズもこれに合わせる必要があります。
	 *	@param fileName 作成するファイル名。
	 *	@param scale スケール。
	 *	@param rate レート。 scale 秒間に rate 毎のアニメになります。
	 *	@param boIsComp true なら圧縮モード選択のためダイアログが表示されます。 false なら非圧縮モードになります。
	 *	@retval true 成功。作成中になり、 Add() が有効になります。
	 *	@retval false 失敗。
	 */
	bool Open(const SIZE& size, LPCTSTR fileName, DWORD scale, DWORD rate, bool boIsComp = false)
	{
		Close();
		size_t imageBytes = (((size.cx * 24 + 31)& ~31) / 8) * size.cy;
		BITMAPINFOHEADER bi =
		{
			sizeof(BITMAPINFOHEADER), size.cx, size.cy, 1, 24, BI_RGB, ToDword(imageBytes), 0, 0, 0, 0
		};
		m_bmpinfo = bi;
		AVISTREAMINFO si = 
		{
			streamtypeVIDEO, comptypeDIB, 0, 0, 0, 0,
			scale, rate, 0, 0, 0, 0, DWORD_MAX, 0, {0, 0, size.cx, size.cy}, 0, 0, _T("VIDEO")
		};
		::AVIFileInit();
		if ( ::AVIFileOpen(&m_pAviFile, fileName, OF_CREATE | OF_WRITE | OF_SHARE_DENY_NONE, NULL) != 0 )
		{
			Close();
			return false;
		}
		if ( boIsComp )
		{
			memset(&m_compvars, 0, sizeof(COMPVARS));
			m_compvars.cbSize = sizeof(COMPVARS);
			m_compvars.dwFlags = ICMF_COMPVARS_VALID;
			m_compvars.fccHandler = comptypeDIB;
			m_compvars.lQ = ICQUALITY_DEFAULT;
			if ( ! ::ICCompressorChoose(NULL, ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME, &bi, NULL, &m_compvars, NULL) )
			{
				Close();
				return false;
			}
			si.fccHandler = m_compvars.fccHandler;
			AVICOMPRESSOPTIONS	opt;
			opt.fccType = streamtypeVIDEO;
			opt.fccHandler = m_compvars.fccHandler;
			opt.dwKeyFrameEvery = m_compvars.lKey;
			opt.dwQuality = m_compvars.lQ;
			opt.dwBytesPerSecond = m_compvars.lDataRate;
			opt.dwFlags = (m_compvars.lDataRate > 0 ? AVICOMPRESSF_DATARATE : 0) | (m_compvars.lKey > 0 ? AVICOMPRESSF_KEYFRAMES : 0);
			opt.lpFormat = NULL;
			opt.cbFormat = 0;
			opt.lpParms = m_compvars.lpState;
			opt.cbParms = m_compvars.cbState;
			opt.dwInterleaveEvery = 0;
			if ( ::AVIFileCreateStream(m_pAviFile, &m_pSourceStream, &si) != 0)
			{
				Close();
				return false;
			}
			if ( ::AVIMakeCompressedStream(&m_pCompressedStream, m_pSourceStream, &opt, NULL) != AVIERR_OK )
			{
				Close();
				return false;
			}
		}
		else
		{
			if ( ::AVIFileCreateStream(m_pAviFile, &m_pCompressedStream, &si) != 0 )
			{
				Close();
				return false;
			}
		}
		if ( ::AVIStreamSetFormat(m_pCompressedStream, 0, &bi, sizeof(BITMAPINFOHEADER)) != 0 )
		{
			Close();
			return false;
		}
		return true;
	}

	/**
	 * [追加] フレーム追加. 
	 *	@param bmp ビットマップ。 
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Add(CBitmapHandle bmp)
	{
		if ( m_pCompressedStream != NULL )
		{
			CWorkMem work(m_bmpinfo.biSizeImage);
			HDC dc = ::GetDC(NULL);
			int r = ::GetDIBits(dc, bmp, 0, m_bmpinfo.biHeight - 1, 
						work.Ref(), reinterpret_cast<BITMAPINFO*>(&m_bmpinfo), DIB_RGB_COLORS);
			::ReleaseDC(NULL, dc);
			if ( r == m_bmpinfo.biHeight - 1 )
			{
				if ( ::AVIStreamWrite(m_pCompressedStream, m_index++, 1, work.Ref(), down_cast<LONG>(work.GetSize()), AVIIF_KEYFRAME, NULL, NULL) == 0 )
				{
					return true;
				}
			}
		}
		return false;
	}
};



}; // TNB