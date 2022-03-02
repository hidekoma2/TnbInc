#pragma once
/**
 *@file
 * テンポラリディレクトリ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTemporaryFile.h"
#include "TnbFileName.h"


#ifndef _WIN32_WCE
 #include <SHLWAPI.h>
 #pragma comment(lib, "SHLWAPI.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * テンポラリディレクトリ管理テンプレートクラス
 *
 *		一時的にクリアなディレクトリを作成します。
 *		不必要時（デストラクタ時や、Free() 時）に、全ファイルを削除します。
 *
 *	@note 一インスタンスに付き、管理するディレクトリは一つだけです。
 *
 *	@param PFIX テンポラリファイルのプリフィックス。通常一文字のASCIIコードを指定します。
 *				最大４文字まで記述可能ですが。順番が逆になることに注意。\n
 *				ex) CTemporaryDirectoryT<'ABCD'> なら、'～￥DCBA0000.tmp￥' というディレクトリ名になります。 
 *
 *	@par必要ファイル
 *			TnbTemporaryDirectory.h
 *
 *	@date 06/12/13 新規作成
 */
template<DWORD PFIX>
class CTemporaryDirectoryT : CCopyImpossible
{
	CStr m_strDir;		///< 管理するディレクトリ名

	/// 下位のフォルダも一緒に削除
	bool m_Remove(LPCTSTR lpszDir)
	{
		if ( ::RemoveDirectory(lpszDir) )
		{
			return true;
		}
		CFileName fn(lpszDir);
		return fn.Remove(false, true);
	}

public:

	/// コンストラクタ
	CTemporaryDirectoryT(void)
	{
	}

	/// デストラクタ
	~CTemporaryDirectoryT(void)
	{
		if ( ! m_strDir.IsEmpty() )
		{
			m_Remove(m_strDir);
			m_strDir.Empty();
		}
	}

	/**
	 * [取得] テンポラリディレクトリ名取得
	 *	@return フルパスディレクトリ名。 empty状態なら作成していない状態。
	 */
	CStr GetName(void) const
	{
		return m_strDir;
	}

	/**
	 * [確認] テンポラリディレクトリ有無確認
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool HasThat(void) const
	{
		return ! m_strDir.IsEmpty();
	}

	/**
	 * [操作] 解放.
	 *		作成したテンポラリディレクトリを削除します。
	 *		作成していなければ何もしません。
	 *	@throw CDeleteFailureException テンポラリファイルが使われているとスローされます。
	 */
	void Free(void)
	{
		if ( ! m_strDir.IsEmpty() )
		{
			if ( ! m_Remove(m_strDir) )
			{
				throw CDeleteFailureException(::GetLastError());
			}
			m_strDir.Empty();
		}
	}

	/**
	 * [操作] 解放.
	 *		テンポラリディレクトリの管理を終了します。
	 *		作成したテンポラリディレクトリは削除しません。
	 */
	void Detach(void)
	{
		m_strDir.Empty();
	}

	/**
	 * [操作] 作成.
	 *		テンポラリディレクトリ名とそのディレクトリを作成します。
	 *	@note 作成したテンポラリディレクトリがあれば先に解放します。
	 *	@return テンポラリディレクトリ名
	 *	@throw CIoFailureException ファイル操作時にエラーが発生すると、スローされます。
	 */
	CStr Create(void)
	{
		Free();
		CTemporaryFileT<PFIX> temp;
		CStr s = temp.Create();
		temp.Free();
		if ( ! ::CreateDirectory(s, NULL) )
		{
			s.Empty();
		}
		else
		{
			m_strDir = s;
			s += _T("\\");
		}
		return s;
	}
};



/**@ingroup FILE
 * テンポラリディレクトリ管理テンプレート
 *
 *	@note プリフィックスは tnb になります。
 *
 *	@par必要ファイル
 *			TnbTemporaryDirectory.h
 *
 *	@date 07/01/16 新規作成
 */
typedef CTemporaryDirectoryT<'bnt'> CTemporaryDirectory;



}; // TNB
