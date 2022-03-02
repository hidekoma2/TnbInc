#pragma once
/**
 *@file
 * HTTPファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbInternetHandle.h"
#include "TnbReader.h"



//TNB Library
namespace TNB
{



/**@ingroup INTERNET FILE
 * HTTPファイル読み込みクラス
 *
 *	@par必要ファイル
 *			TnbHttpFile.h
 *
 *	@date 06/10/13 新規
 */
class CHttpFileReader : public IReader
{
	CInternetHandle m_hWininet;	///< セッションハンドル
	CInternetHandle m_hFile;	///< １ファイルハンドル
	CStr			m_strName;	///< オープン名

public:
	
	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_strName;
	}

	/**
	 * [設定] ファイルオープン.
	 *		指定されたファイルをオープンする。 
	 *	@param lpszAgent	HTTPによりインターネットにアクセスする際のエージェント名。
	 *	@param lpszUrl		オープンするファイルの名前。
	 *	@param dwContext	コールバックに渡すアプリケーション定義の値。省略可能。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗
	 */
	bool Open(LPCTSTR lpszAgent, LPCTSTR lpszUrl, DWORD_PTR dwContext = 0)
	{
		Close();
		if ( ! m_hWininet.Open(lpszAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0) )
		{
			return false;
		}
		const DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD;
		m_hFile = m_hWininet.OpenUrl(lpszUrl, NULL, 0, dwFlags, dwContext);
		m_strName = lpszUrl;
		return m_hFile.IsValid();
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return m_hFile.IsValid();
	}

	/**
	 * [取得] ファイル読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		DWORD dwRead;
		if ( m_hFile.ReadFile(_P, size, &dwRead) )
		{
			return dwRead;
		}
		throw CReadFailureException(::GetLastError());
	}

	/**
	 * [操作] シーク.
	 *		ファイルポインタを移動します。
	 *	@param llOffset ファイルポインタの移動数。
	 *	@param eSeek 移動開始ファイルポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		LARGE_INTEGER r;
		r.QuadPart = llOffset;
		if ( r.HighPart == 0 )
		{
			r.LowPart = m_hFile.SetFilePointer(r.LowPart, 0, eSeek, 0);
			if ( r.LowPart == INVALID_SET_FILE_POINTER )
			{
				r.QuadPart = -1;
			}
		}
		else
		{
			r.QuadPart = -1;
		}
		return r.QuadPart;
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		LONGLONG l = Seek(0, CURRENT);
		LONGLONG r = Seek(0, END);
		if ( l >= 0 )
		{
			Seek(l, TOP);
		}
		return r;
	}

	/**
	 * [設定] ファイルクローズ.
	 */
	void Close(void)
	{
		m_hFile.Close();
		m_hWininet.Close();
	}

	/**
	 * [取得] ファイルハンドル取得
	 *	@note Open() で取得したファイルのハンドルが返ります。クローズしていると、NULLのハンドルが返ります。 
	 *	@return ハンドル
	 */
	CInternetHandle& GetHandle(void)
	{
		return m_hFile;
	}
};



}; // TNB


 
