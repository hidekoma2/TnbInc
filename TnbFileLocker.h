#pragma once
/**
 *@file
 * ファイルロック関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbHandleHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ファイルロック管理クラス
 *
 *	本クラスは、インスタンス一つに付き一つだけ、ファイルをLockすることが出来ます。
 *
 *	@par必要ファイル
 *			TnbFileLocker.h
 *
 *	@date 08/03/26 新規作成
 */
class CFileLocker : CCopyImpossible
{
	CHandleHandle	m_hhFile;		///< ファイルハンドル
public:

	/// コンストラクタ
	CFileLocker(void) :	m_hhFile(INVALID_HANDLE_VALUE)
	{
	}

	/// デストラクタ
	~CFileLocker(void)
	{
		Unlock();
	}

	/**
	 * [確認] ロック状態
	 *	@retval true ロック中
	 *	@retval false ロック中でない
	 */
	bool IsLock(void) const
	{
		return ! m_hhFile.IsNull();
	}

	/** 
	 * [排他] ロック
	 *	@param lpszName ロックするファイル名。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Lock(LPCTSTR lpszName)
	{
		Unlock();
		DWORD dwDesiredAccess = GENERIC_READ;
		DWORD dwShareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
		DWORD dwCreationDisp = OPEN_EXISTING;
		m_hhFile = ::CreateFile(lpszName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisp, 0, NULL);
		_GetLastError("CreateFile");
		if ( ! m_hhFile.IsNull() && ::LockFile(m_hhFile, 0, 0, DWORD_MAX, DWORD_MAX) )
		{
			return true;
		}
		m_hhFile.Null();
		return false;
	}

	/**
	 * [排他] アンロック
	 *	@see Lock
	 */
	void Unlock(void)
	{
		if ( ! m_hhFile.IsNull() )
		{
			::UnlockFile(m_hhFile, 0, 0, DWORD_MAX, DWORD_MAX);
			m_hhFile.Null();
		}
	}
};



}; // TNB
