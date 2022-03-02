#pragma once
/**
 *@file
 * AVI�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapHandle.h"
#include <vfw.h>
#pragma comment(lib, "Vfw32.lib")



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * AVI���[�J�[�N���X.
 *
 *		BITMAP���w�肵�AAVI�t�@�C�����쐬���܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *
 *		CAviMaker avi;
 *		avi.Open(CSize(48, 48), "c:\\aa.avi", 1, 10);	//1�b�Ԃ�10��		
 *		avi.Add(IDB_FRAME1);
 *		avi.Add(IDB_FRAME2);
 *		avi.Add(IDB_FRAME3);
 *		avi.Stop();
 *
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbAviMaker.h
 *
 *	@date 07/11/01 �V�K�쐬
 */
class CAviMaker : CCopyImpossible
{
	BITMAPINFOHEADER	m_bmpinfo;				///< BITMAP���
	PAVISTREAM			m_pSourceStream;		///< �\�[�X�X�g���[��
	PAVISTREAM			m_pCompressedStream;	///< ���k�X�g���[��
	PAVIFILE			m_pAviFile;				///< AVI�t�@�C���Ǘ�
	COMPVARS			m_compvars;				///< COMPVARS
	UINT				m_index;				///< �C���f�b�N�X
public:

	/// �R���X�g���N�^
	CAviMaker(void) : m_pSourceStream(NULL), m_pCompressedStream(NULL), m_pAviFile(NULL), m_index(0)
	{
		m_compvars.cbSize = 0;
	}

	/// �f�X�g���N�^
	virtual ~CAviMaker()
	{
		Close();
	}

	/**
	 * [��~] �X�g�b�v. 
	 *	@note �쐬���̏ꍇ�A�I�����AAVI�t�@�C�����쐬���܂��B
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
	 * [�J�n] �I�[�v��. 
	 *	@note �쐬���̏ꍇ�A�I�����AAVI�t�@�C�����쐬���܂��B
	 *	@param size AVI�t�@�C���T�C�Y�B Add�Œǉ����� �r�b�g�}�b�v�̃T�C�Y������ɍ��킹��K�v������܂��B
	 *	@param fileName �쐬����t�@�C�����B
	 *	@param scale �X�P�[���B
	 *	@param rate ���[�g�B scale �b�Ԃ� rate ���̃A�j���ɂȂ�܂��B
	 *	@param boIsComp true �Ȃ爳�k���[�h�I���̂��߃_�C�A���O���\������܂��B false �Ȃ�񈳏k���[�h�ɂȂ�܂��B
	 *	@retval true �����B�쐬���ɂȂ�A Add() ���L���ɂȂ�܂��B
	 *	@retval false ���s�B
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
	 * [�ǉ�] �t���[���ǉ�. 
	 *	@param bmp �r�b�g�}�b�v�B 
	 *	@retval true �����B
	 *	@retval false ���s�B
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