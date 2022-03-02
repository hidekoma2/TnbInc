#pragma once
/**
 *@file
 * Windows�N���b�v�{�[�h�֌W�̃w�b�_
 *
 *	@note	�}�N�� "_TnbCLIPBOARD_OtherType_ENABLE" ����`����Ă���ƁA������ȊO��
 *			�N���b�v���������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifdef _TnbCLIPBOARD_OtherType_ENABLE
 #include "TnbDntStr.h"
 #include "TnbMap.h"
#else
 #include "TnbStr.h"
#endif
#include "TnbBitmapHandle.h"
#include <shlobj.h>
#include <wingdi.h>
#ifndef _WIN32_WCE
 #pragma comment(lib,"gdi32.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup CLIPBOARD
 * �N���b�v�{�[�h�A�N�Z�X�N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbClipboard.h
 * 
 *	@date 06/06/13	�V�K�쐬
 *	@date 06/06/26	GetAll , SetAll ,�V�K
 *	@date 07/05/10	�N���X���ύX�B
 *	@date 09/02/13	�r�b�g�}�b�v�̃A�N�Z�T�ǉ��B
 *	@date 09/10/29	�č\���B
 *	@date 10/04/14	���[�U�f�[�^�Ή��B
 */
class CClipboard : CCopyImpossible
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param hWnd �e�E�B���h�E�n���h���B�ȗ����邱�Ƃ��o���܂��B
	 *	@param dwOpenTimeout �N���b�v�{�[�h�I�[�v�����̃^�C���A�E�g�B
	 *						�w��ms�̊ԁA���g���C���܂��B
	 *	@param boHasErrBox true���w�肷��ƃG���[���Ƀ_�C�A���O���o���܂��B
	 *						�ȗ�����Əo���܂���B
	 */	
	explicit CClipboard(HWND hWnd = NULL, DWORD dwOpenTimeout = 500, bool boHasErrBox = false)
		: m_hWnd(hWnd), m_dwOpenTimeout(dwOpenTimeout), m_boHasErrBox(boHasErrBox)
	{
	}

	/**
	 * [�o�^] �E�B���h�E�n���h���ݒ�
	 *	@param hWnd �e�E�B���h�E�n���h���B
	 */	
	void SetHandle(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	/**
	 * [����] ��.
	 *		�N���b�v�{�[�h����ɂ��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Empty(void)
	{
		bool boRc = false;
		if ( m_Open(true) )
		{
			m_Close();
			boRc = true;
		}
		return boRc;
	}

	/**
	 * [�m�F] ���݊m�F.
	 *		���݂̃N���b�v�{�[�h�Ƀf�[�^�����邩�m�F���܂��B
	 *	@param uFormat �N���b�v�{�[�h�t�H�[�}�b�gID�B�e�L�X�g���w�肵�����ꍇ�ȗ��ł��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool IsAvailable(UINT uFormat = 0) const
	{
		bool boRc = false;
		if ( uFormat == 0 )
		{
			uFormat = CBFMT_TEXT;
		}
		if ( m_Open() )
		{
			boRc = !! ::IsClipboardFormatAvailable(uFormat);
			m_Close();
		}
		return boRc;
	}

	/**
	 * [�擾] �f�[�^�^���擾
	 *	@note uFormat �� 0xC000�`0xFFFF �ȊO�Ȃ玸�s���܂��B
	 *	@param uFormat �f�[�^�^
	 *	@return �^��
	 */
	static CStr GetFormatName(UINT uFormat)
	{
		CStr str;
		int r = ::GetClipboardFormatName(uFormat, str.GetBuffer(1024), 1024);
		_GetLastError("GetClipboardFormatName");
		str.ReleaseBuffer();
		if ( r == 0 )
		{
			str.Empty();
		}
		return str;
	}

	/**
	 * [�o�^] ���[�U��`�f�[�^�^�o�^.
	 *	@param lpszDataTypeName �f�[�^�^��
	 *	@return �f�[�^�^ID�B
	 */
	static UINT RegisterUserDataType(LPCTSTR lpszDataTypeName)
	{
		return ::RegisterClipboardFormat(lpszDataTypeName);
	}

	/**
	 * [�擾] ������擾.
	 *		�N���b�v�{�[�h���當������擾���܂��B
	 *	@return ������B�Ȃ������ꍇEmpty���Ԃ�܂��B
	 */
	CStr GetString(void) const
	{
		CStr strRc;
		if ( m_Open() )
		{
			if ( ::IsClipboardFormatAvailable(CBFMT_TEXT) )
			{
				HANDLE hMem = ::GetClipboardData(CBFMT_TEXT);
				if ( hMem != NULL )
				{
					strRc = static_cast<LPCTSTR>(GlobalLock(hMem));
					GlobalUnlock(hMem);
				}
			}
			m_Close();
		}
		return strRc;
	}

	/**
	 * [�ݒ�] ������o�^.
	 *		�N���b�v�{�[�h�ɕ������o�^���܂��B
	 *	@param lpszText ������B
	 *	@param isAdd true �Ȃ猻�݂̃N���b�v�{�[�h�����N���A�����A�ǉ����܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetString(LPCTSTR lpszText, bool isAdd = false)
	{
		if ( ! m_Open(! isAdd) )
		{
			return false;
		}
		size_t iStrLen = STRLIB::GetLen(lpszText);
		HGLOBAL hMem = m_AllocAndCopy((iStrLen + 1) * sizeof(TCHAR), lpszText);
		bool boRc = m_SetData(CBFMT_TEXT, hMem);
		m_Close();
		return boRc;
	}

	/**
	 * [�擾] ���[�U��`�f�[�^�擾.
	 *		�N���b�v�{�[�h����A�w��̃��[�U�f�[�^�����o���܂��B
	 *	@note RegisterUserDataType() �Ń��[�U��`��o�^���Ă����K�v������܂��B
	 *	@param[out] _data �f�[�^
	 *	@param[in] dataTypeId �f�[�^�^ID( RegisterUserDataType() �̖߂�l)�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetUserData(CWorkMem& _data, UINT dataTypeId) const
	{
		bool r = false;
		if ( m_Open() )
		{
			if ( ::IsClipboardFormatAvailable(dataTypeId) )
			{
				HGLOBAL h = ::GetClipboardData(dataTypeId);
				size_t len = ::GlobalSize(h);
				_data.Reset(len, static_cast<BYTE*>(GlobalLock(h)));
				r = true;
				GlobalUnlock(h);
			}
			m_Close();
		}
		return r;
	}
	
	/**
	 * [�ݒ�] ���[�U��`�f�[�^�ݒ�.
	 *		�N���b�v�{�[�h�ɁA�w��̃��[�U�f�[�^��ݒ肵�܂��B
	 *	@note RegisterUserDataType() �Ń��[�U��`��o�^���Ă����K�v������܂��B
	 *	@param dataTypeId �f�[�^�^ID( RegisterUserDataType() �̖߂�l)�B
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param pData �f�[�^
	 *	@param isAdd true �Ȃ猻�݂̃N���b�v�{�[�h�����N���A�����A�ǉ����܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetUserData(UINT dataTypeId, size_t dataSize, LPCVOID pData, bool isAdd = false)
	{
		if ( ! m_Open(! isAdd) )
		{
			return false;
		}
		HGLOBAL h = m_AllocAndCopy(dataSize, pData);
		bool r = m_SetData(dataTypeId, h);
		m_Close();
		return r;
	}

	#ifndef _WIN32_WCE

	/**
	 * [�擾] �r�b�g�}�b�v�擾.
	 *		�N���b�v�{�[�h����r�b�g�}�b�v���擾���܂��B
	 *	@retval NULL �Ȃ�����
	 *	@retval NULL�ȊO �����B�l�� HBITMAP�B
	 */
	HBITMAP GetBitmap(void) const
	{
		HBITMAP rc = NULL;
		if ( m_Open() )
		{
			if ( ::IsClipboardFormatAvailable(CF_BITMAP) )
			{
				HBITMAP hBmp = static_cast<HBITMAP>(::GetClipboardData(CF_BITMAP));
				if ( hBmp != NULL )
				{
					rc = static_cast<HBITMAP>(::CopyImage(hBmp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
				}
			}
			m_Close();
		}
		return rc;
	}

	/**
	 * [�ݒ�] �r�b�g�}�b�v�o�^.
	 *		�N���b�v�{�[�h�Ƀr�b�g�}�b�v��o�^���܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param isAdd true �Ȃ猻�݂̃N���b�v�{�[�h�����N���A�����A�ǉ����܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetBitmap(HBITMAP bmp, bool isAdd = false)
	{
		if ( ! m_Open(! isAdd) )
		{
			return false;
		}
		BITMAPINFOHEADER bmi;
		RGBQUAD* pRgb = NULL;
		HBITMAP hBmp = CBitmapHandle::Create32bitDibSection(bmp, pRgb, &bmi);
		if ( hBmp == NULL )
		{
			return false;
		}
		HGLOBAL h = ::GlobalAlloc(GHND, sizeof(bmi) + bmi.biWidth * bmi.biHeight * sizeof(RGBQUAD));
		BYTE* B = static_cast<BYTE*>(::GlobalLock(h));
		MemCopy(B, &bmi, sizeof(bmi));
		MemCopy(&B[sizeof(bmi)], pRgb, bmi.biWidth * bmi.biHeight * sizeof(RGBQUAD));
		::GlobalUnlock(h);
		bool boRc = m_SetData(CF_DIB, h);
		_DeleteObject(hBmp);
		m_Close();
		return boRc;
	}

	#endif // _WIN32_WCE

	#ifdef _TnbCLIPBOARD_OtherType_ENABLE

	#ifndef _WIN32_WCE

	/**
	 * [�擾] �h���b�v���擾.
	 *	@param[out] _vstrFiles �t�@�C����
	 *	@param[out] _dwEffect �G�t�F�N�g���	
	 *				DROPEFFECT_COPY	/ DROPEFFECT_MOVE / DROPEFFECT_LINK ��AND���Z���A
	 *				0�ȊO�Ȃ�A���̃G�t�F�N�g��Ԃł��邱�Ƃ������܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetDropInfo(CStrVector& _vstrFiles, DWORD& _dwEffect) const
	{
		bool boRc = false;
		_vstrFiles.RemoveAll();
		_dwEffect = 0;
		if ( m_Open() )
		{
			if ( ::IsClipboardFormatAvailable(CF_HDROP) )
			{
				HDROP hDrop = static_cast<HDROP>(::GetClipboardData(CF_HDROP));
				TCHAR atcBuf[MAX_PATH];
				int iCount = ToInt(::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL));
				for ( int i = 0; i < iCount; i++ )
				{
					::DragQueryFile(hDrop, i, atcBuf, MAX_PATH);
					_vstrFiles.Add(atcBuf);
				}
				//
				UINT uFmt = ::RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
				HANDLE hDropEffect = ::GetClipboardData(uFmt);
				if ( hDropEffect != NULL )
				{
					_dwEffect = *static_cast<DWORD*>(::GlobalLock(hDropEffect));
					::GlobalUnlock(hDropEffect);
					boRc = true;
				}
			}
			m_Close();
		}
		return boRc;
	}

	/**
	 * [�ݒ�] �t�@�C���h���b�v���ݒ�.
	 *		�w�肵���t�@�C�������R�s�[/�J�b�g��Ԃɂ��܂��B���݂��Ȃ��t�@�C�����ƁA
	 *		�y�[�X�g���ɃG���[�ɂȂ�܂��B
	 *	@param vstrFiles �t�@�C�����B�t���p�X�Ŏw�肷��悤�ɂ��Ă��������B
	 *	@param dwEffect �G�t�F�N�g���B
	 *				DROPEFFECT_COPY	�� DROPEFFECT_MOVE �� DROPEFFECT_LINK ���w�肵�܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetDropInfo(const CStrVector& vstrFiles, DWORD dwEffect = DROPEFFECT_COPY)
	{
		if ( ! m_Open(true) )
		{
			return false;
		}
		bool boRc = false;
		//Double Null Term Str �쐬
		CDoubleNullTerminateStr dnt;
		dnt.Set(vstrFiles);
		//CF_HDROP���쐬
		DROPFILES dfs = { sizeof(dfs) };
		#ifdef _UNICODE
			dfs.fWide = 1;
		#endif
		size_t size = dnt.GetSize() * sizeof(TCHAR);
		HGLOBAL hDrop = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DROPFILES) + size);
		BYTE* B = static_cast<BYTE*>(::GlobalLock(hDrop));
		MemCopy(B, reinterpret_cast<const BYTE*>(&dfs), sizeof(DROPFILES));
		B += sizeof(DROPFILES);
		MemCopy(B, reinterpret_cast<const BYTE*>(LPCTSTR(dnt)), size);
		::GlobalUnlock(hDrop);
		//Preferred DropEffect���쐬
		HGLOBAL hDropEffect = m_AllocAndCopy(sizeof(DWORD), &dwEffect);
		//�N���b�v�{�[�h�Ƀf�[�^���Z�b�g
		UINT uFmt = ::RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
		if ( ::SetClipboardData(CF_HDROP, hDrop) == NULL )
		{
			::GlobalFree(hDropEffect);
			::GlobalFree(hDrop);
		}
		else if ( ::SetClipboardData(uFmt, hDropEffect) == NULL )
		{
			::GlobalFree(hDropEffect);
		}
		else
		{
			boRc = true;
		}
		m_Close();
		return boRc;
	}

	#endif // _WIN32_WCE

	/// �f�[�^�^�ƃf�[�^�̃}�b�v�^.
	typedef CMapT< UINT, CVectorT<BYTE> > CDataMap;

	/**
	 * [�擾] �S���擾.
	 *	@note �e�L�X�g��DIB�A���I�v�V�����̏����}�b�v�ɂ��ĕԂ��܂��B
	 *	@param[out] _data �f�[�^�^�ƃf�[�^�̃}�b�v
	 *	@param[in] boIsWithMetafile true ���w�肷��ƁA�g�����^�t�@�C�������܂߂܂��B
	 *	@param[in] boIsWithRegData true���w�肷��ƁA�o�^�f�[�^�����܂߂܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetAll(CDataMap& _data, bool boIsWithMetafile = false, bool boIsWithRegData = false) const
	{
		bool boRc = false;
		_data.RemoveAll();
		if ( ! m_Open() )
		{
			return false;
		}
		UINT uFormat = 0;
		while ( true )
		{
			UINT r = ::EnumClipboardFormats(uFormat);
			if ( r == 0 )
			{
				DWORD dwError = _GetLastError("EnumClipboardFormats");
				if ( dwError == NO_ERROR )
				{
					boRc = true; 
				}
				else
				{
					_data.RemoveAll(); 
				}
				break;
			}
			//
			bool boIsValid = false;
			switch ( r )
			{
			case CBFMT_TEXT:	//CF_TEXT CF_UNICODETEXT CF_OEMTEXT �Ɠ���
			case CF_DIB:		//CF_BITMAP CF_PALETTE CF_DIBV5 �Ɠ���
				boIsValid = true;
				break;
#ifndef _WIN32_WCE
			case CF_ENHMETAFILE:	//CF_METAFILEPICT �Ɠ���
				boIsValid = boIsWithMetafile;
				break;
#endif
			default:
				if ( boIsWithRegData )
				{
					boIsValid = (r >= 0x8000);
				}
				break;
			}
			//
			if ( boIsValid )
			{
				CVectorT<BYTE> v;
				HANDLE hMem = ::GetClipboardData(r);
#ifndef _WIN32_WCE
				if ( r == CF_ENHMETAFILE )
				{
					HENHMETAFILE h = (HENHMETAFILE)hMem;
					UINT l = ::GetEnhMetaFileBits(h, 0, NULL);
					::GetEnhMetaFileBits(h, l, v.GetBuffer(l));
					v.ReleaseBuffer();
				}
				else
#endif
				{
					v.SetElements(static_cast<size_t>(::GlobalSize(hMem)), static_cast<BYTE*>(GlobalLock(hMem)));
					GlobalUnlock(hMem);
				}
				_data[r] = v;
				TRACE3("  Found Clip = 0x%04X[%s] len=%d \n", r, GetFormatName(r), v.GetSize());
			}
			else
			{
				TRACE2("  Drop Clip = 0x%04X[%s]\n", r, GetFormatName(r));
			}
			uFormat = r;
		}
		m_Close();
		return boRc;
	}

	/**
	 * [�ݒ�] �S���ݒ�.
	 *	@note �e�L�X�g�ACF_DIB�ACF_ENHMETAFILE�A���̑��o�^Data����Clip�֐ݒ肵�܂��B
	 *	@param data �f�[�^�^�ƃf�[�^�̃}�b�v
	 *	@retval true �S������
	 *	@retval false ���s
	 */
	bool SetAll(const CDataMap& data)
	{
		if ( ! m_Open(true) )
		{
			return false;
		}
		bool boRc = true;
		UINT uFormat;
		CVectorT<BYTE> v;
		loop ( i, data.GetSize() )
		{
			VERIFYLIB(data.Get(i, uFormat, v));
			bool boIsValid = false;
			switch ( uFormat )
			{
			case CF_TEXT:
			case CF_UNICODETEXT:
			case CF_OEMTEXT:
			case CF_DIB:		//CF_BITMAP CF_PALETTE CF_DIBV5 �Ɠ���
				boIsValid = true;
				break;
#ifndef _WIN32_WCE
			case CF_ENHMETAFILE:	//CF_METAFILEPICT �Ɠ���
				{
					HENHMETAFILE h = ::SetEnhMetaFileBits(ToDword(v.GetSize()), v.ReferBuffer());
					boRc &= m_SetData(uFormat, h);
				}
				break;
#endif
			default:
				break;
			}
			if ( uFormat >= 0x8000 || boIsValid )
			{

#ifdef _WIN32_WCE
				if ( uFormat == CF_TEXT )
				{
					LPCSTR P = reinterpret_cast<LPCSTR>(v.ReferBuffer());
					CUnicode u = P;
					size_t l = u.GetLength() * 2;
					LPCWSTR lpsz = u;
					MemCopy(v.GetBuffer(l), lpsz, l);
					v.ReleaseBuffer();
					uFormat = CF_UNICODETEXT;
				}
#endif
				HGLOBAL hMem = m_AllocAndCopy(v.GetSize(), v.ReferBuffer());
				boRc &= m_SetData(uFormat, hMem);
			}			
		}
		m_Close();
		return boRc;
	}

	#endif // _TnbCLIPBOARD_OtherType_ENABLE

private:

	#ifdef _UNICODE
		enum { CBFMT_TEXT = CF_UNICODETEXT };
	#else	
		enum { CBFMT_TEXT = CF_TEXT };
	#endif

	HWND	m_hWnd;			///< �I�[�i�[�n���h��
	DWORD	m_dwOpenTimeout;///< Open���̃^�C���A�E�gms
	bool	m_boHasErrBox;	///< �G���[���_�C�A���O���o�����ۂ��B

	/// ���b�Z�[�W�{�b�N�X�\��
	void m_MsgBox(LPCTSTR lpsz) const
	{
		if ( m_boHasErrBox )
		{
			TRACE1("%s\n", lpsz);
			::MessageBox(NULL, lpsz, _T("TNB::CClipboard"), MB_OK); 
		}
	}

	/**
	 * �I�[�v��
	 *	@param boWithEmpty true �Ȃ�I�[�v����AEmpty���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool m_Open(bool boWithEmpty = false) const
	{
		DWORD dwTick = ::GetTickCount();
		do
		{
			if ( ::OpenClipboard(m_hWnd) )
			{
				if ( boWithEmpty && ! ::EmptyClipboard() )
				{
					::CloseClipboard();
					m_MsgBox(_T("�N���b�v�E�{�[�h����t�ł�"));
					return false;
				}
				return true;
			}
			::Sleep(1);
		}
		while ( (::GetTickCount() - dwTick) < m_dwOpenTimeout );
		m_MsgBox(_T("�N���b�v�E�{�[�h�����p�ł��܂���B"));
		return false;
	}

	/// �N���[�Y
	void m_Close(void) const
	{
		::CloseClipboard(); 
	}

	/**
	 * GlobalMemory�m�ہ����e�i�[ 
	 *	@param size	���e�T�C�Y
	 *	@param P	���e�A�h���X
	 *	@return �m�ہ����e�i�[�����������n���h��
	 */
	HGLOBAL m_AllocAndCopy(size_t size, LPCVOID P)
	{
		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, size);
		if ( hMem == NULL ) 
		{
			m_MsgBox(_T("����������t�ł�"));
		}
		else
		{
			::CopyMemory(GlobalLock(hMem), P, size);
			GlobalUnlock(hMem);
		}
		return hMem;
	}

	/**
	 * �N���b�v�փZ�b�g 
	 *	@param uFormat	�f�[�^�^
	 *	@param hMem		�������n���h��
	 *	@retval true �Z�b�g����
	 *	@retval false ���s�BhMem �J���B
	 */
	bool m_SetData(UINT uFormat, HGLOBAL hMem)
	{
		if ( hMem == NULL )
		{
			return false;
		}
		if ( ::SetClipboardData(uFormat, hMem) == NULL )
		{
			::GlobalFree(hMem);
			return false;
		}
		return true;
	}
};



}; // TNB
