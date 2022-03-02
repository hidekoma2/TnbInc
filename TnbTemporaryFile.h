#pragma once
/**
 *@file
 * テンポラリファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * テンポラリファイル管理テンプレートクラス
 *
 *		メモリの内容を一時的にファイルにして操作するためのクラスです。
 *		指定のデータ内容のファイルをテンポラリに作成し、不必要時（デストラクタ時や、
 *		Free() , Release() 時）にファイルを削除します。
 *
 *	@note 一インスタンスに付き、管理するファイルは一つだけです。
 *
 *	@param PFIX テンポラリファイルのプリフィックス。通常一文字のASCIIコードを指定します。
 *				最大４文字まで記述可能ですが。順番が逆になることに注意。\n
 *				ex) CTemporaryFileT<'ABCD'> なら、'～￥DCBA0000.tmp' というファイル名になります。 
 *
 *	@par必要ファイル
 *			TnbTemporaryFile.h
 *
 *	@date 06/12/13 新規作成
 *	@date 08/10/30 SetTempraryFolder() 新規追加。
 */
template<DWORD PFIX>
class CTemporaryFileT : CCopyImpossible
{
	CStr		m_tempFolder;	///< 対象フォルダ名
	CStr		m_targetFile;	///< 管理ファイル名
	bool m_Create(void)
	{
		Free();
		CStr strPath;
		if ( m_tempFolder.IsEmpty() )
		{
			::GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));
			strPath.ReleaseBuffer();
		}
		else
		{
			strPath = m_tempFolder;
		}
		const static TCHAR tc[5] = { (PFIX&0xFF), ((PFIX>>8)&0xFF), ((PFIX>>16)&0xFF), ((PFIX>>24)&0xFF), 0 };
		::GetTempFileName(strPath, tc, 0, m_targetFile.GetBuffer(MAX_PATH));
		m_targetFile.ReleaseBuffer();
		return ! m_targetFile.IsEmpty();
	}

public:

	/// コンストラクタ
	CTemporaryFileT(void)
	{
	}

	/**
	 * デストラクタ.
	 *		作成したテンポラリファイルがあれば削除します。
	 *	@note 削除に失敗する可能性もあるので、先に Free() などをしておくことをお勧めします。
	 */
	~CTemporaryFileT(void)
	{
		if ( ! m_targetFile.IsEmpty() )
		{
			::DeleteFile(m_targetFile);
			m_targetFile.Empty();
		}
	}

	/**
	 * [取得] テンポラリファイル名取得
	 *	@return フルパスファイル名。 empty状態なら作成していない状態。
	 */
	CStr GetName(void) const
	{
		return m_targetFile;
	}

	/**
	 * [確認] テンポラリファイル有無確認
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool HasThat(void) const
	{
		return ! m_targetFile.IsEmpty();
	}

	/**
	 * [操作] 解放.
	 *		作成したテンポラリファイルを削除します。
	 *		作成していなければ何もしません。
	 *	@throw CDeleteFailureException テンポラリファイルが使われているとスローされます。
	 */
	void Free(void)
	{
		if ( ! m_targetFile.IsEmpty() )
		{
			if ( ! ::DeleteFile(m_targetFile) )
			{
				throw CDeleteFailureException(_GetLastError("DeleteFile"));
			}
			m_targetFile.Empty();
		}
	}

	/**
	 * [操作] 解放.
	 *		テンポラリファイルの管理を終了します。
	 *		作成したテンポラリファイルは削除しません。
	 */
	void Detach(void)
	{
		m_targetFile.Empty();
	}

	/**
	 * [操作] 解放.
	 *		作成したテンポラリファイルをメモリに読み込み、削除します。
	 *		作成していなければ何もしません。
	 *	@return ファイル内容
	 *	@throw CIoFailureException ファイル操作時にエラーが発生すると、スローされます。
	 *	@throw CDeleteFailureException テンポラリファイルが使われているとスローされます。	
	 */
	CByteVector Release(void)
	{
		CByteVector vb;
		if ( ! m_targetFile.IsEmpty() )
		{
			HANDLE h = ::CreateFile(m_targetFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				throw COpenFailureException(_GetLastError("CreateFile"));
			}
			DWORD dw = 0;
			DWORD size = ::GetFileSize(h, &dw);
			if ( size > 0 )
			{
				vb.SetSize(size);
				BOOL boRc = ::ReadFile(h, vb.GetBuffer(size), size, &dw, NULL);
				if ( ! boRc || dw != size )
				{
					throw CReadFailureException(_GetLastError("ReadFile"));
				}
				vb.ReleaseBuffer();
			}
			::CloseHandle(h);
			Free();
		}
		return vb;
	}

	/**
	 * [操作] 作成.
	 *		テンポラリファイル名とサイズ0のテンポラリファイルを作成します。
	 *	@note 作成したテンポラリファイルがあれば先に解放します。
	 *	@return テンポラリファイル名
	 *	@throw CIoFailureException ファイル操作時にエラーが発生すると、スローされます。
	 */
	CStr Create(void)
	{
		m_Create();
		return m_targetFile;
	}

	/**
	 * [操作] 作成.
	 *		テンポラリファイル名と指定サイズのテンポラリファイルを作成します。
	 *	@note 作成したテンポラリファイルがあれば先に解放します。
	 *	@param size サイズ。
	 *	@return テンポラリファイル名
	 *	@throw CIoFailureException ファイル操作時にエラーが発生すると、スローされます。
	 */
	CStr Create(size_t size)
	{
		if ( m_Create() && size > 0 )
		{
			HANDLE h = ::CreateFile(m_targetFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				throw COpenFailureException(_GetLastError("CreateFile"));
			}
			LONG l = 0;
			::SetFilePointer(h, ToDword(size), &l, FILE_BEGIN);
			VERIFY( ::SetEndOfFile(h) );
			::CloseHandle(h);
		}
		return m_targetFile;
	}

	/**
	 * [操作] 作成.
	 *		テンポラリファイル名と指定データのテンポラリファイルを作成します。
	 *	@note 作成したテンポラリファイルがあれば先に解放します。
	 *	@param size データサイズ。
	 *	@param P データ。
	 *	@return テンポラリファイル名
	 *	@throw CIoFailureException ファイル操作時にエラーが発生すると、スローされます。
	 */
	CStr Create(size_t size, LPCVOID P)
	{
		if ( m_Create() && size >= 0 )
		{
			HANDLE h = ::CreateFile(m_targetFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
			if ( h == INVALID_HANDLE_VALUE )
			{
				throw COpenFailureException(_GetLastError("CreateFile"));
			}
			DWORD dw = 0;
			VERIFY( ::WriteFile(h, P, ToDword(size), &dw, NULL) );
			::CloseHandle(h);
		}
		return m_targetFile;
	}

	/**
	 * [操作] 作成.
	 *		テンポラリファイル名と指定データのテンポラリファイルを作成します。
	 *	@note 作成したテンポラリファイルがあれば先に解放します。
	 *	@param vb データ。
	 *	@return テンポラリファイル名
	 *	@throw CIoFailureException ファイル操作時にエラーが発生すると、スローされます。
	 */
	CStr Create(const ISequenceCollectionT<BYTE>& vb)
	{
		return Create(vb.GetSize(), vb.ReferBuffer());
	}

	/**
	 * [設定] テンポラリフォルダ指定.
	 *	@note 設定しない場合、システムのテンポラリフォルダに作成します。
	 *	@param pFolder パス。
	 */
	void SetTempraryFolder(LPCTSTR pFolder)
	{
		m_tempFolder = pFolder;
	}

	/**
	 * [操作] アタッチ.
	 *		指定のファイルをテンポラリファイルとします。
	 *	@note 作成したテンポラリファイルがあれば先に解放します。
	 *	@param lpszFile ファイル名(フルパス)。
	 *	@return テンポラリファイル名
	 *	@throw COpenFailureException ファイルが存在しないと、スローされます。
	 */
	CStr Attach(LPCSTR lpszFile)
	{
		if ( ToInt(::GetFileAttributes(lpszFile)) == -1 )
		{
			throw COpenFailureException(_GetLastError("GetFileAttributes"));
		}
		m_targetFile = lpszFile;
		return m_targetFile;
	}
};



/**@ingroup FILE
 * テンポラリファイル管理テンプレート
 *
 *	@note プリフィックスは tnb になります。
 *
 *	@par必要ファイル
 *			TnbTemporaryFile.h
 *
 *	@date 06/12/13 新規作成
 */
typedef CTemporaryFileT<'bnt'> CTemporaryFile;



}; // TNB