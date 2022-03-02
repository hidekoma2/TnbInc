#pragma once
/**
 *@file
 * フォルダロック関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFileLocker.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * フォルダロック管理クラス
 *
 *	本クラスは、インスタンス一つに付き一つだけ、ディレクトリをLockすることが出来ます。
 *
 *	@todo 未実装。
 *
 *	@par必要ファイル
 *			TnbDirectoryLocker.h
 *
 *	@date 08/03/26 新規作成
 */
class CDirectoryLocker : CCopyImpossible
{
	typedef CPointerHandleT<CFileLocker> CFileLockerHandle;
	CSimpleVectorT<CFileLockerHandle> m_v;

public:

	/// コンストラクタ
	CDirectoryLocker(void)
	{
	}

	/// デストラクタ
	~CDirectoryLocker(void)
	{
		Unlock();
	}

	/** 
	 * [排他] ロック
	 *	@param lpszDir ロックするディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Lock(LPCTSTR lpszDir)
	{
		Unlock();




	}

	/**
	 * [排他] アンロック
	 *	@see Lock
	 */
	void Unlock(void)
	{
		m_v.RemoveAll();
	}
};



}; // TNB
