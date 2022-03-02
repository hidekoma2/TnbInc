#pragma once
/**
 *@file
 * Windowsクリップボード関係のヘッダ
 *
 *	@note	マクロ "_TnbCLIPBOARD_OtherType_ENABLE" が定義されていると、文字列以外の
 *			クリップ情報も扱えます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
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
 * クリップボードアクセスクラス
 *
 *	@par必要ファイル
 *			TnbClipboard.h
 * 
 *	@date 06/06/13	新規作成
 *	@date 06/06/26	GetAll , SetAll ,新規
 *	@date 07/05/10	クラス名変更。
 *	@date 09/02/13	ビットマップのアクセサ追加。
 *	@date 09/10/29	再構成。
 *	@date 10/04/14	ユーザデータ対応。
 */
class CClipboard : CCopyImpossible
{
public:

	/**
	 * コンストラクタ
	 *	@param hWnd 親ウィンドウハンドル。省略することも出来ます。
	 *	@param dwOpenTimeout クリップボードオープン時のタイムアウト。
	 *						指定msの間、リトライします。
	 *	@param boHasErrBox trueを指定するとエラー時にダイアログを出します。
	 *						省略すると出しません。
	 */	
	explicit CClipboard(HWND hWnd = NULL, DWORD dwOpenTimeout = 500, bool boHasErrBox = false)
		: m_hWnd(hWnd), m_dwOpenTimeout(dwOpenTimeout), m_boHasErrBox(boHasErrBox)
	{
	}

	/**
	 * [登録] ウィンドウハンドル設定
	 *	@param hWnd 親ウィンドウハンドル。
	 */	
	void SetHandle(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	/**
	 * [操作] 空化.
	 *		クリップボードを空にします。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [確認] 存在確認.
	 *		現在のクリップボードにデータがあるか確認します。
	 *	@param uFormat クリップボードフォーマットID。テキストを指定したい場合省略できます。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [取得] データ型名取得
	 *	@note uFormat は 0xC000～0xFFFF 以外なら失敗します。
	 *	@param uFormat データ型
	 *	@return 型名
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
	 * [登録] ユーザ定義データ型登録.
	 *	@param lpszDataTypeName データ型名
	 *	@return データ型ID。
	 */
	static UINT RegisterUserDataType(LPCTSTR lpszDataTypeName)
	{
		return ::RegisterClipboardFormat(lpszDataTypeName);
	}

	/**
	 * [取得] 文字列取得.
	 *		クリップボードから文字列を取得します。
	 *	@return 文字列。なかった場合Emptyが返ります。
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
	 * [設定] 文字列登録.
	 *		クリップボードに文字列を登録します。
	 *	@param lpszText 文字列。
	 *	@param isAdd true なら現在のクリップボード情報をクリアせず、追加します。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [取得] ユーザ定義データ取得.
	 *		クリップボードから、指定のユーザデータを取り出します。
	 *	@note RegisterUserDataType() でユーザ定義を登録しておく必要があります。
	 *	@param[out] _data データ
	 *	@param[in] dataTypeId データ型ID( RegisterUserDataType() の戻り値)。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [設定] ユーザ定義データ設定.
	 *		クリップボードに、指定のユーザデータを設定します。
	 *	@note RegisterUserDataType() でユーザ定義を登録しておく必要があります。
	 *	@param dataTypeId データ型ID( RegisterUserDataType() の戻り値)。
	 *	@param dataSize データサイズ
	 *	@param pData データ
	 *	@param isAdd true なら現在のクリップボード情報をクリアせず、追加します。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [取得] ビットマップ取得.
	 *		クリップボードからビットマップを取得します。
	 *	@retval NULL なかった
	 *	@retval NULL以外 成功。値は HBITMAP。
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
	 * [設定] ビットマップ登録.
	 *		クリップボードにビットマップを登録します。
	 *	@param bmp ビットマップ。
	 *	@param isAdd true なら現在のクリップボード情報をクリアせず、追加します。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [取得] ドロップ情報取得.
	 *	@param[out] _vstrFiles ファイル名
	 *	@param[out] _dwEffect エフェクト情報	
	 *				DROPEFFECT_COPY	/ DROPEFFECT_MOVE / DROPEFFECT_LINK とAND演算し、
	 *				0以外なら、そのエフェクト状態であることを示します。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [設定] ファイルドロップ情報設定.
	 *		指定したファイル名をコピー/カット状態にします。実在しないファイルだと、
	 *		ペースト時にエラーになります。
	 *	@param vstrFiles ファイル名。フルパスで指定するようにしてください。
	 *	@param dwEffect エフェクト情報。
	 *				DROPEFFECT_COPY	か DROPEFFECT_MOVE か DROPEFFECT_LINK を指定します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetDropInfo(const CStrVector& vstrFiles, DWORD dwEffect = DROPEFFECT_COPY)
	{
		if ( ! m_Open(true) )
		{
			return false;
		}
		bool boRc = false;
		//Double Null Term Str 作成
		CDoubleNullTerminateStr dnt;
		dnt.Set(vstrFiles);
		//CF_HDROPを作成
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
		//Preferred DropEffectを作成
		HGLOBAL hDropEffect = m_AllocAndCopy(sizeof(DWORD), &dwEffect);
		//クリップボードにデータをセット
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

	/// データ型とデータのマップ型.
	typedef CMapT< UINT, CVectorT<BYTE> > CDataMap;

	/**
	 * [取得] 全情報取得.
	 *	@note テキストとDIB、他オプションの情報をマップにして返します。
	 *	@param[out] _data データ型とデータのマップ
	 *	@param[in] boIsWithMetafile true を指定すると、拡張メタファイル情報も含めます。
	 *	@param[in] boIsWithRegData trueを指定すると、登録データ情報も含めます。
	 *	@retval true 成功
	 *	@retval false 失敗
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
			case CBFMT_TEXT:	//CF_TEXT CF_UNICODETEXT CF_OEMTEXT と同期
			case CF_DIB:		//CF_BITMAP CF_PALETTE CF_DIBV5 と同期
				boIsValid = true;
				break;
#ifndef _WIN32_WCE
			case CF_ENHMETAFILE:	//CF_METAFILEPICT と同期
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
	 * [設定] 全情報設定.
	 *	@note テキスト、CF_DIB、CF_ENHMETAFILE、その他登録Data情報をClipへ設定します。
	 *	@param data データ型とデータのマップ
	 *	@retval true 全部成功
	 *	@retval false 失敗
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
			case CF_DIB:		//CF_BITMAP CF_PALETTE CF_DIBV5 と同期
				boIsValid = true;
				break;
#ifndef _WIN32_WCE
			case CF_ENHMETAFILE:	//CF_METAFILEPICT と同期
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

	HWND	m_hWnd;			///< オーナーハンドル
	DWORD	m_dwOpenTimeout;///< Open時のタイムアウトms
	bool	m_boHasErrBox;	///< エラー時ダイアログを出すか否か。

	/// メッセージボックス表示
	void m_MsgBox(LPCTSTR lpsz) const
	{
		if ( m_boHasErrBox )
		{
			TRACE1("%s\n", lpsz);
			::MessageBox(NULL, lpsz, _T("TNB::CClipboard"), MB_OK); 
		}
	}

	/**
	 * オープン
	 *	@param boWithEmpty true ならオープン後、Emptyします。
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
					m_MsgBox(_T("クリップ・ボードが一杯です"));
					return false;
				}
				return true;
			}
			::Sleep(1);
		}
		while ( (::GetTickCount() - dwTick) < m_dwOpenTimeout );
		m_MsgBox(_T("クリップ・ボードが利用できません。"));
		return false;
	}

	/// クローズ
	void m_Close(void) const
	{
		::CloseClipboard(); 
	}

	/**
	 * GlobalMemory確保＆内容格納 
	 *	@param size	内容サイズ
	 *	@param P	内容アドレス
	 *	@return 確保＆内容格納したメモリハンドル
	 */
	HGLOBAL m_AllocAndCopy(size_t size, LPCVOID P)
	{
		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, size);
		if ( hMem == NULL ) 
		{
			m_MsgBox(_T("メモリが一杯です"));
		}
		else
		{
			::CopyMemory(GlobalLock(hMem), P, size);
			GlobalUnlock(hMem);
		}
		return hMem;
	}

	/**
	 * クリップへセット 
	 *	@param uFormat	データ型
	 *	@param hMem		メモリハンドル
	 *	@retval true セット成功
	 *	@retval false 失敗。hMem 開放。
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
