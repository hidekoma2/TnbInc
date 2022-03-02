#pragma once
/**
 *@file
 * FTPファイル検索関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFtpFile.h"
#include "TnbFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE INTERNET
 * FTPファイル検索クラス
 *
 *		指定のディレクトリからファイルを検索するためのクラスです。
 *
 *	@par必要ファイル
 *			TnbFtpFileFinder.h
 *
 *	@date 06/11/30 新規
 */
class CFtpFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// コンストラクタ
	CFtpFileFinder(void) : _super()
	{
	}

	/**
	 * [設定] 初期化
	 *	@note インスタンス作成後、検索の前に一度はこのメソッドを実行します。
	 *	@param sec			オープンしたセッションハンドル。
	 *	@param dwFlags	オープンの種別を指定するビットマスク値。後述の表を参照。
	 *					<BR>		INTERNET_FLAG_HYPERLINK		再読み込みすべきかどうかの判断時に有効期限・最終更新時刻がサーバから返されない場合強制的に再読み込みする。
	 *					<BR>		INTERNET_FLAG_NEED_FILE		ファイルがキャッシュできない場合に一時ファイルを作成する。
	 *					<BR>		INTERNET_FLAG_RELOAD		要求したファイル、オブジェクト、ディレクトリリストをキャッシュから取得するのでなく、強制的にサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTPリソースをサーバから再読み込みする。
	 *					<BR>		INTERNET_FLAG_NO_CACHE_WRITE キャッシュに書き込まない。
	 */
	void Initialize(CFtpSession& sec, DWORD dwFlags = 0)
	{
		Finish();
		m_hInet = sec;
		m_dwFlags = dwFlags;
	}

	/// デストラクタ
	virtual ~CFtpFileFinder(void)
	{
		OnFinish();
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
		Finish();
		if ( m_hInet.IsValid() )
		{
			m_hFind = m_hInet.FindFirstFile(lpszName, &_data, m_dwFlags, 0);
		}
		return !! m_hFind.IsValid();
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
		if ( m_hFind.IsValid() )
		{
			return !! m_hFind.FindNextFile(&_data);
		}
		return false;
	}

	/**
	 * [通知] 検索終了通知
	 *	@note 検索中の場合、検索を停止します（ OnNext() メソッドは false を返すようになります）。
	 */
	virtual void OnFinish(void)
	{
		m_hFind.Close();
	}

private:
	CInternetHandle		m_hInet;		///< インターネットハンドル
	CInternetHandle		m_hFind;		///< 検索ハンドル
	DWORD				m_dwFlags;		///< 検索フラグ
	friend class CFtpFileFinderTest;
};



}; //TNB



