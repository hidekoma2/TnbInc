#pragma once
/**
 *@file
 * パイプ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * パイプアクセスクラス
 *
 *		パイプを使用した通信が可能です。
 *
 *	@par必要ファイル
 *			TnbPipe.h
 * 
 *	@date 07/01/23	新規作成
 *	@date 07/02/09	親クラス変更。
 *	@date 07/06/11	親クラスの Start() メソッドに対応。
 *	@date 07/09/13	親クラス変更。
 *	@date 10/05/28	SetReceiveTimeout() を実装。
 *	@date 12/01/26	GetReceivingDataSize() の戻り値を修正。
 *	@data 16/05/13	名前付きクライアント実装。サーバーは未確認。
 */
class CPipe : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);

public:

	/// コンストラクタ
	CPipe(void) 
		: m_hRead(INVALID_HANDLE_VALUE), m_hWrite(INVALID_HANDLE_VALUE), m_dwPipeBufferSize(0), m_property(_T("Pipe"), TNBVIW_KIND_PIPE)
	{
		m_isActiveClient = false;
	}

	/// デストラクタ
	virtual ~CPipe(void)
	{
		Close();
	}

	/**
	 * [設定] パイプバッファサイズ設定
	 *	@param dwSize パイプのバッファのサイズをバイト単位で指定します。\n
	 *				システムはこの値を使って、適切なバッファリングメカニズムを決定します。\n
	 *				名前なしパイプの場合は省略するか、0 を指定すると、既定のバッファサイズが割り当てられます。
	 */
	void SetPipeBufferSize(DWORD dwSize = 0)
	{
		m_dwPipeBufferSize = dwSize;
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
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@return 常に false。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return false;
	}

	/**
	 * [設定] パイプ名設定.
	 *		本メソッドを使用することで名前付きパイプを使うことが出来ます。 Open() の前に使用してください。
	 *	@param lpszPipeName パイプ名。必ず "\\\\.\\pipe\\" で始まる名前にしてください。
	 *	@param isClient true なら名前付きパイプクライアント、 false ならサーバーをオープンします。
	 */
	void SettingNamedPipe(LPCTSTR lpszPipeName, bool isClient)
	{
		m_pipeName = lpszPipeName;
		m_isModeClient = isClient;
	}

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		Close();
		//
		if ( m_pipeName.IsEmpty() )
		{
			//-- 名無しパイプオープン
			SECURITY_ATTRIBUTES sa;
			sa.lpSecurityDescriptor = NULL;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = TRUE;
			return !! ::CreatePipe(&m_hRead, &m_hWrite, &sa, m_dwPipeBufferSize);
		}
		if ( m_isModeClient )
		{
			//== 名前付きパイプクライアントオープン(ここではオープンしない)
			m_isActiveClient = true;
			m_hRead = INVALID_HANDLE_VALUE;
			m_hWrite = INVALID_HANDLE_VALUE;
			return true;
		}
		//== 名前付きパイプサーバーオープン
		m_hRead = ::CreateNamedPipe(m_pipeName, 
			PIPE_ACCESS_DUPLEX, //オープンモード
			PIPE_TYPE_BYTE | PIPE_WAIT, //パイプモード
			1, //パイプに対する最大インスタンス
			m_dwPipeBufferSize,//出力バッファ
			m_dwPipeBufferSize,//入力バッファ
			0,//タイムアウト
			NULL);
		m_hWrite = INVALID_HANDLE_VALUE;
		return m_hRead != INVALID_HANDLE_VALUE;
	}

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
		if ( m_hRead != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hRead);
			m_hRead = INVALID_HANDLE_VALUE;
		}
		if ( m_hWrite != INVALID_HANDLE_VALUE )
		{
			::CloseHandle(m_hWrite);
			m_hWrite = INVALID_HANDLE_VALUE;
		}
		m_isActiveClient = false;
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	virtual bool IsOpened(void) const
	{
		if ( m_isActiveClient )
		{
			return true;
		}
		return m_hRead != INVALID_HANDLE_VALUE;
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
		m_OnActiveClient();
		//
		DWORD dwWriten;
		if ( ::WriteFile(GetWriteHandle(), P, down_cast<DWORD>(size), &dwWriten, NULL) )
		{
			return dwWriten;
		}
		return INVALID_SIZE;
	}

	/**
	 * [処理] 受信.
	 *	@param[out] _P データ。size 分のメモリは確保しておく必要があります。
	 *	@param[in] size サイズ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 0 受信データなし。
	 *	@retval 上記以外 受信データサイズ。
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		size_t l = GetReceivingDataSize();
		if ( l == 0 )
		{
			return 0;
		}
		if ( l != INVALID_SIZE )
		{
			if ( size < l )
			{
				l = size;
			}
			DWORD dwReadBytes = 0;
			if ( !! ::ReadFile(m_hRead, _P, down_cast<DWORD>(l), &dwReadBytes, NULL) )
			{
				return dwReadBytes;
			}
			_GetLastError("ReadFile");
		}
		return INVALID_SIZE;
	}

	/**
	 * [確認] 受信データ確認.
	 *	@retval INVALID_SIZE 不明。
	 *	@retval 上記以外 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		m_OnActiveClient();
		if ( m_hRead == INVALID_HANDLE_VALUE )
		{
			return INVALID_SIZE;
		}
		//
		DWORD dwAvail;
		if ( ::PeekNamedPipe(m_hRead, NULL, 0, NULL, &dwAvail, NULL) )
		{
			return dwAvail;
		}
		if ( ! m_pipeName.IsEmpty() && m_isModeClient )
		{
			m_hRead = INVALID_HANDLE_VALUE;
		}
		_GetLastError("PeekNamedPipe");
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
		m_OnActiveClient();
		return IsConnect() ? CONNECTING : CLOSED;
	}

	/**
	 * [処理] パージ.
	 *		通信のゴミを浄化
	 */
	virtual void Purge(void) 
	{
	}

	/**
	 * [取得] 読み込みハンドル取得
	 *	@return 読み込みハンドル
	 */
	HANDLE GetReadHandle(void) const
	{
		return m_hRead;
	}

	/**
	 * [取得] 書き込みハンドル取得
	 *	@return 書き込みハンドル
	 */
	HANDLE GetWriteHandle(void) const
	{
		if ( m_pipeName.IsEmpty() )
		{
			return m_hWrite;
		}
		return m_hRead;
	}

private:
	// 名前付きパイプクライアントがアクティブの時の処理
	void m_OnActiveClient(void) const
	{
		if ( m_isActiveClient && ! m_pipeName.IsEmpty() && m_isModeClient )
		{
			if ( m_hRead == INVALID_HANDLE_VALUE )
			{
				// パイプクライアントをオープン
				m_hRead = ::CreateFile(m_pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			}
		}
	}

	CStr				m_pipeName;			///< パイプ名
	bool				m_isModeClient;		///< 名前付きパイプクライアントモード？
	bool				m_isActiveClient;	///< 名前付きパイプクライアントモードアクティブ中？
	mutable HANDLE		m_hRead;			///< 読み込みハンドル
	HANDLE				m_hWrite;			///< 書き込みハンドル
	DWORD				m_dwPipeBufferSize;	///< バッファサイズ
	TProperty			m_property;			///< 本クラスの性能
};



}; // TNB
