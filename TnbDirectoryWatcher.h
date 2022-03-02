#pragma once
/**
 *@file
 * ファイル監視関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ディレクトリ監視クラス
 *
 *		指定のディレクトリの変化を監視するためのクラスです。
 *
 *		指定の変化があるとリスナーに通知されます。
 *		その際、どんな変化があったかはわかりませんので、変化内容を知りたい場合、
 *		監視フラグ毎に本クラスのインスタンスを複数作る必要があります
 *		（リスナーにはフラグ情報が来るので、一つのリスナーで対応可能です）。
 *
 *	@see http://nienie.com/~masapico/api_FindFirstChangeNotification.html
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbDirectoryWatcher.h
 *
 *	@date 06/12/01 新規
 *	@date 10/03/19	ReferThreadStatus() 追加。
 */
class CDirectoryWatcher : TNB::CThread::IRunner
{
public:

	/**@ingroup FILE
	 * ディレクトリ監視のリスナーインターフェース
	 *
	 *	@par必要ファイル
	 *			TnbDirectoryWatcher.h
	 *
	 *	@date 06/12/01 新規作成。
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/**
		 * [通知] 変化通知
		 *	@note 複数の条件を指定した場合、どの条件かは、わからないので注意。
		 *	@param dwFlags CDirectoryWatcher::Start() の dwFlags で指定したの値。
		 *					変化した内容ではないことに注意。
		 *	@retval true 監視継続
		 *	@retval false 監視停止
		 */
		virtual bool OnDirectoryChange(DWORD dwFlags) = 0;
	};

private:

	HANDLE			m_hNotify;		///< 監視ハンドル
	DWORD			m_dwFlags;		///< 監視フラグ
	IListener*		m_pListener;	///< リスナー
	TNB::CThread	m_thread;		///< 監視スレッド

	/** 
	 * [動作] スレッド処理本体
	 *	@return ０。
	 */
	virtual DWORD Run(void)
	{
		while ( m_hNotify != NULL && IsRunnable() )
		{
			DWORD r = ::WaitForSingleObject(m_hNotify, INFINITE);
			if ( r != WAIT_OBJECT_0 || m_hNotify == NULL )
			{
				break;
			}
			if ( ! m_pListener->OnDirectoryChange(m_dwFlags) )
			{
				break;
			}
			if ( ! ::FindNextChangeNotification(m_hNotify) )
			{
				ASSERT(false);
				break;
			}
		}
		if ( m_hNotify != NULL )
		{
			::FindCloseChangeNotification(m_hNotify);
			m_hNotify = NULL;
		}
		return 0;
	}

public:

	/// コンストラクタ
	CDirectoryWatcher(void) : m_hNotify(NULL), m_pListener(NULL)
	{
	}

	/// デストラクタ
	virtual ~CDirectoryWatcher(void)
	{
		Stop();
	}

	/**
	 * [開始] 監視開始
	 *	@param lpszPath	監視を行うディレクトリのディレクトリ名
	 *	@param boWithSubDir tureを指定すると指定ディレクトリ以下のサブディレクトリ内も監視対象になります。
	 *	@param dwFlags どのようなイベントを監視するかを指定するフラグ。									\n
	 *						FilterFlagsに指定できる値は、以下のフラグの論理和である。					\n
	 *						FILE_NOTIFY_CHANGE_FILE_NAME	ファイルの追加・削除、ファイル名の変更		\n
	 *						FILE_NOTIFY_CHANGE_DIR_NAME	ディレクトリの追加・削除、ディレクトリ名の変更	\n
	 *						FILE_NOTIFY_CHANGE_ATTRIBUTES	属性の変更									\n
	 *						FILE_NOTIFY_CHANGE_SIZE	サイズの変更										\n
	 *						FILE_NOTIFY_CHANGE_LAST_WRITE	最終書き込み時刻の変更						\n
	 *						FILE_NOTIFY_CHANGE_SECURITY	セキュリティ属性の変更
	 *	@param pListener リスナー。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Start(LPCTSTR lpszPath, bool boWithSubDir, DWORD dwFlags, IListener* pListener)
	{
		Stop();
		m_hNotify = ::FindFirstChangeNotification(lpszPath, boWithSubDir, dwFlags);
		if ( m_hNotify == NULL )
		{
			return false;
		}
		m_dwFlags = dwFlags;
		m_pListener = pListener;
		m_thread.SetRunner(this);
		if ( ! m_thread.Start(_T("Directory監視")) )
		{
			Stop();
			return false;
		}
		return true;
	}

	/**
	 * [停止] 監視停止
	 *	@note 監視開始していない場合、何もしません。
	 */
	void Stop(void)
	{
		if ( m_hNotify != NULL )
		{
			HANDLE h = m_hNotify;
			m_hNotify = NULL;
			::FindCloseChangeNotification(h);
		}
		m_thread.Stop();
	}

	/**
	 * [参照] 監視スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}
};



}; //TNB
