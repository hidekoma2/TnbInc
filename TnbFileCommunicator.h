#pragma once
/**
 *@file
 * ファイル通信関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFile.h"
#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION FILE
 * ファイル通信クラス
 *
 *		二つのファイルを使った、プロセス間通信をサポートします。
 *
 *		ファイルの読み書きは、 CFileCommunicator::IWorker を実装したクラスで行います。
 *		一例として、 CFileCommunicationWorker が用意されています。
 *
 *	@par必要ファイル
 *			TnbFileCommunicator.h
 * 
 *	@date 07/09/13	新規作成
 *	@date 10/03/24	単発の同期転送に変更。
 *	@date 10/05/28 SetSendTimeout() を新規。 SetReceiveTimeout() を実装。
 */
class CFileCommunicator : public ICommunication
{
	DEFSUPER(ICommunication);
public:

	/**@ingroup COMMUNICATION FILE
	 * ファイル通信ワーカーインターフェース.
	 *
	 *		CFileCommunicator でファイルの読み書きに使用します。
	 *
	 *	@note CFileCommunicator のインスタンスと本インターフェースのインスタンスを同じスコープにおく場合、 
	 *		CFileCommunicator のインスタンスよりも先に宣言しておくこと（先に構築され、後に破棄されるため）。
	 *
	 *	@par必要ファイル
	 *			TnbFileCommunicator.h
	 * 
	 *	@date 10/03/24	新規作成
	 */
	struct IWorker
	{
		/// デストラクタ
		virtual ~IWorker(void) {}

		/**
		 * [処理] オープン.
		 *		オープン時、コールされます。
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		virtual bool OnOpen(void) = 0;

		/**
		 * [処理] 読み込み.
		 *		情報受信の際コールされます。
		 *	@param size 読み込みサイズ。
		 *	@param _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
		 *	@retval INVALID_SIZE オープン失敗。
		 *	@retval 0 読み込めなかった。
		 *	@retval 上記以外 成功。値は読み込めたサイズ。
		 */
		virtual size_t Read(size_t size, LPVOID _P) =0;

		/**
		 * [処理] 書き込み.
		 *		情報送信の際コールされます。
		 *	@param size 書き込みサイズ。
		 *	@param P 書き込みデータ。
		 *	@retval INVALID_SIZE オープン失敗。
		 *	@retval 上記以外 成功。値は書き込めたサイズ。
		 */
		virtual size_t Write(size_t size, LPCVOID P) = 0;

		/**
		 * [処理] パージ.
		 *		通信のゴミを浄化
		 *	@retval true 成功。
		 *	@retval false 失敗.
		 */
		virtual bool Purge(void) = 0;
	};


	//--------------------------------------


	/// コンストラクタ 
	CFileCommunicator(void) : m_pWorker(NULL), m_sendTimeout(1000)
		, m_property(_T("File"), TNBVIW_KIND_FILE, 1000)
	{
	}

	/// デストラクタ
	virtual ~CFileCommunicator(void)
	{
		Close();
	}

	/**
	 * [設定] ワーカーの指定.
	 *	@note 本クラスのインスタンスとワーカーのインスタンスを同じスコープにおく場合、 
	 *		ワーカーのほうを上に宣言しておくこと（先に構築され、後に破棄されるため）。
	 *	@param pWorker ワーカー。
	 */
	void SetWorker(IWorker* pWorker)
	{
		m_pWorker = pWorker;
	}

	/**
	 * [設定] 送信待ち時間設定.
	 *		ファイルがオープンできない時、リトライし続ける時間を指定します。
	 *	@param to タイムアウト(ms)
	 */
	void SetSendTimeout(DWORD to)
	{
		m_sendTimeout = to;
	}

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@return 常に true。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		m_property.receiveTimeout = to;
		return true;
	}

	/**
	 * [取得] 性能取得.
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property;
	}

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		if ( m_pWorker == NULL )
		{
			return false;
		}
		return m_pWorker->OnOpen();
	}

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	virtual bool IsOpened(void) const
	{
		return true;
	}

	/**
	 * [確認] 送信可能確認.
	 *	@retval true 送信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanSend(void) const
	{
		return (m_pWorker != NULL);
	}

	/**
	 * [確認] 受信可能確認.
	 *	@retval true 受信可能。
	 *	@retval false 不可能。
	 */
	virtual bool CanReceive(void) const
	{
		return (m_pWorker != NULL);
	}

	/**
	 * [処理] 送信.
	 *	@param size サイズ。
	 *	@param P データ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 上記以外 送信データサイズ。
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		size_t r = INVALID_SIZE;
		if ( m_pWorker != NULL )
		{
			CTickCount tick;
			do
			{
				size_t rr = m_pWorker->Write(size, P);
				if ( rr != INVALID_SIZE )
				{
					r = rr;
					break;
				}
				::Sleep(RETRY_INTERVAL);
			}
			while ( ! tick.IsPassed(m_sendTimeout) );
		}
		return r;
	}

	/**
	 * [処理] 受信.
	 *	@param[in] size サイズ。
	 *	@param[out] _P データ。size 分のメモリは確保しておく必要があります。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 0 受信データなし。
	 *	@retval 上記以外 受信データサイズ。
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		size_t r = 0;
		if ( m_pWorker != NULL )
		{
			CTickCount tick;
			do
			{
				size_t rr = m_pWorker->Read(size, _P);
				if ( rr != INVALID_SIZE )
				{
					r = rr;
					break;
				}
				::Sleep(RETRY_INTERVAL);
			}
			while ( ! tick.IsPassed(m_property.receiveTimeout) );
		}
		return r;
	}

	/**
	 * [確認] 受信データ確認.
	 *	@retval INVALID_SIZE 不明。
	 *	@retval 上記以外 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		return INVALID_SIZE;
	}

	/**
	 * [確認] チェック.
	 *		状態を確認します。
	 *	@retval CONNECTING	オープン＆接続状態
	 *	@retval CLOSED		クローズ状態	
	 */
	virtual EState Check(void) const
	{
		return IsConnect() ? CONNECTING : CLOSED;
	}

	/**
	 * [処理] パージ.
	 *		通信のゴミを浄化
	 */
	virtual void Purge(void) 
	{
		if ( m_pWorker != NULL )
		{
			CTickCount tick;
			do
			{
				if ( m_pWorker->Purge() )
				{
					break;
				}
				::Sleep(RETRY_INTERVAL);
			}
			while ( ! tick.IsPassed(m_property.receiveTimeout) );
		}
	}

protected:
	enum { RETRY_INTERVAL = 10 };
	DWORD			m_sendTimeout;		///< 送信タイムアウト
	IWorker*		m_pWorker;			///< ワーカー
	TProperty		m_property;			///< 本クラスの性能
};



/**@ingroup COMMUNICATION FILE
 * ファイル通信ワーカークラス
 *
 *		二つのファイルを使った、プロセス間通信をサポートします。 
 *
 *		本クラスでは、以下の仕様で通信サポートします。
 *		- 送信(書き込み)時は、先に送信した情報が相手に受信されていなくても、上書きします。
 *		- 受信(読み込み)時、送信時、共に、他のプロセスが使用しているときは、1秒、リトライします。
 *
 *	@par必要ファイル
 *			TnbFileCommunicator.h
 * 
 *	@date 07/09/13	新規作成
 *	@date 10/03/24	単発の同期転送に変更。
 */
class CFileCommunicationWorker : public CFileCommunicator::IWorker
{
public:

	/**
	 * [設定] 対象ファイル名設定.
	 *	@param lpszReadFile 読み込み対象ファイル名.
	 *	@param lpszWriteFile 書き込み対象ファイル名.
	 *	@return 常にtrue
	 */
	bool SetFileName(LPCTSTR lpszReadFile, LPCTSTR lpszWriteFile)
	{
		m_readFile = lpszReadFile;
		m_writeFile = lpszWriteFile;
		return true;
	}

	/**
	 * [処理] オープン.
	 *		オープン時、コールされます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnOpen(void)
	{
		Purge();
		bool isEmpty = m_readFile.IsEmpty() || m_writeFile.IsEmpty();
		return ! isEmpty;
	}

	/**
	 * [取得] 読み込み対象オープン.
	 *	@param size 読み込みサイズ。
	 *	@param _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@retval INVALID_SIZE オープン失敗。
	 *	@retval 0 読み込めなかった。
	 *	@retval 上記以外 成功。値は読み込めたサイズ。
	 */
	virtual size_t Read (size_t size, LPVOID _P)
	{
		CFileReader fr;
		if ( fr.Open(m_readFile, false) )
		{
			size_t r = fr.Read(size, _P);
			fr.Close();
			::DeleteFile(m_readFile);
			return r;
		}
		DWORD e = ::GetLastError();
		return (e == ERROR_FILE_NOT_FOUND) ? 0 : INVALID_SIZE;
	}

	/**
	 * [取得] 書き込み対象オープン.
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@retval INVALID_SIZE オープン失敗。
	 *	@retval 上記以外 成功。値は書き込めたサイズ。
	 */
	virtual size_t Write(size_t size, LPCVOID P)
	{
		CFileWriter fw;
		if ( fw.New(m_writeFile, false) )
		{
			try
			{
				fw.Write(size, P);
				return size;
			}
			catch ( CWriteFailureException& e )
			{
				e.OnCatch();
			}
		}
		return INVALID_SIZE;
	}

	/**
	 * [処理] パージ.
	 *		通信のゴミを浄化
	 *	@retval true 成功。
	 *	@retval false 失敗.
	 */
	virtual bool Purge(void)
	{
		::DeleteFile(m_writeFile);
		BOOL r = ::DeleteFile(m_readFile + _T("__"));
		r &= ::DeleteFile(m_readFile);
		return !! r;
	}

protected:

	CStr		m_readFile;		///< 読み込み対象ファイル名
	CStr		m_writeFile;	///< 書き込み対象ファイル名
};



}; // TNB
