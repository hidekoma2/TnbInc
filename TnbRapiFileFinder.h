#pragma once
/**
 *@file
 * RAPI ファイル検索関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFileFinder.h"
#include "TnbStr.h"
#include "TnbRapi.h"



//TNB Library
namespace TNB
{



/**@ingroup RAPI FILE
 * RAPI ファイル検索クラス
 *
 *		指定のディレクトリからファイルを検索するためのクラスです。
 *
 *	@note RAPI の接続状態は CRapi::IsConnected() で確認できます。
 *
 *	@par必要ファイル
 *			TnbRapiFileFinder.h
 *			\n Windows Mobile や PocketPC の SDK の ./Activesync/inc/rapi.h 等も必要です。
 *
 *	@date 09/04/23 新規
 */
class CRapiFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// コンストラクタ
	CRapiFileFinder(void) : _super(), m_hFind(INVALID_HANDLE_VALUE)
	{
		CRapi::Startup();
	}

	/// デストラクタ
	virtual ~CRapiFileFinder(void)
	{
		Finish();
	}

protected:

	/**
	 * [通知] 検索開始通知
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@param[in] lpszName 検索パス指定。ワイルドカードによる指定が必要です。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnStart(WIN32_FIND_DATA& _data, LPCTSTR lpszName)
	{
		if ( ! CRapi::IsSupported() ) { return false; }
		Finish();
		CE_FIND_DATA ceFd;
		m_hFind = ::CeFindFirstFile(CUnicode(lpszName), &ceFd);
		bool r = (m_hFind != INVALID_HANDLE_VALUE);
		if ( r )
		{
			m_CeFindDataToWin32(_data, ceFd);
		}
		return r;
	}

	/**
	 * [通知] 次検索通知
	 *	@note Find() で true の場合、次にこのメソッドを使用します。その後繰り返して
	 *			本メソッド使うことで複数の情報を得ることが出来ます。
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnNext(WIN32_FIND_DATA& _data)
	{
		if ( ! CRapi::IsSupported() ) { return false; }
		if ( m_hFind != INVALID_HANDLE_VALUE )
		{
			CE_FIND_DATA ceFd;
			bool r = !! ::CeFindNextFile(m_hFind, &ceFd);
			if ( r )
			{
				m_CeFindDataToWin32(_data, ceFd);
			}
			return r;
		}
		return false;
	}

	/**
	 * [通知] 検索終了通知
	 *	@note 検索中の場合、検索を停止します（ OnNext() メソッドは false を返すようになります）。
	 */
	virtual void OnFinish(void)
	{
		if ( CRapi::IsSupported() && m_hFind != INVALID_HANDLE_VALUE )
		{
			::CeFindClose(m_hFind);
			m_hFind = INVALID_HANDLE_VALUE;
		}
	}

private:
	HANDLE				m_hFind;	///< 検索ハンドル
	/// 構造体変換
	void m_CeFindDataToWin32(WIN32_FIND_DATA& _winFd, const CE_FIND_DATA& ceFd)
	{
		Zero(_winFd);
		_winFd.dwFileAttributes = ceFd.dwFileAttributes;
		_winFd.ftCreationTime = ceFd.ftCreationTime;
		_winFd.ftLastAccessTime = ceFd.ftLastAccessTime;
		_winFd.ftLastWriteTime = ceFd.ftLastWriteTime;
		_winFd.nFileSizeHigh = ceFd.nFileSizeHigh;
		_winFd.nFileSizeLow = ceFd.nFileSizeLow;
		CStr s = ceFd.cFileName;
		STRLIB::Copy(_winFd.cFileName, s);
	}
	friend class CCeFileFinderTest;
};



}; //TNB



