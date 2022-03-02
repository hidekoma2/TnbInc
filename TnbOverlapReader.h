#pragma once
/**
 *@file
 * ファイル関係のヘッダ
 *
 *		ファイル名管理、ファイルハンドル管理のクラス、
 *		さらに簡易書込みクラス、簡易読み込みクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * オーバーラップ入力管理クラス.
 *
 *		API ::ReadFile() による非同期の読込みをサポートします。HIDなど情報がない時にロックさえるデバイスからの読込みに有効です。
 *		
 *
 *	@par必要ファイル
 *			TnbOverlapReader.h
 *
 *	@date 07/09/13 新規作成
 *	@date 08/12/19 IsReading() 新規。
 *	@date 09/12/21 排他、追加。
 */
class COverlapReader : CCopyImpossible
{
public:

	/// コンストラクタ
	COverlapReader(void) : m_hFile(INVALID_HANDLE_VALUE), m_hEvent(NULL), m_isOverLapping(false)
	{
		Zero(m_overLapped);
	}

	/// デストラクタ
	~COverlapReader(void)
	{
		Detach();
		if ( m_hEvent != NULL )
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL; 
		}
	}

	/**
	 * [設定] アタッチ.
	 *	@param[in] hFile FILE_FLAG_OVERLAPPED 付きでオープンしたハンドル。
	 *	@param[in] size サイズ。
	 */
	void Attach(HANDLE hFile, size_t size)
	{
		EXCLUSIVE( &m_sync );
		m_Cancel();
		m_hFile = hFile;
		m_work.Resize(size);
	}

	/**
	 * [設定] デタッチ.
	 */
	void Detach(void)
	{
		EXCLUSIVE( &m_sync );
		m_Cancel();
		m_hFile = INVALID_HANDLE_VALUE;
		m_work.Free();
	}

	/**
	 * [読込] 読込み.
	 *		EventとWaitの付いた ::ReadFile です。
	 *	@param[out] _P 読み込むポインタをを指定します。 Attach() で指定したサイズが必要です。
	 *	@param[in] wait 待ち時間(ms)。 INFINITE を指定すると、結果が帰るまで Waitします。
	 *	@retval INVALID_SIZE エラー。 ::GetLastError() でエラー詳細がわかります。
	 *	@retval 0 データ無し。
	 *	@retval 上記以外 成功。数値は読込みサイズ。
	 */
	size_t Read(LPVOID _P, DWORD wait = 0)
	{
		EXCLUSIVE( &m_sync );
		size_t r = 0;
		if ( m_hFile != INVALID_HANDLE_VALUE && m_work.GetSize() > 0 )
		{
			if ( m_isOverLapping )
			{
				// 読み込み待機中
				r = m_GetResult(wait);
			}
			else
			{
				// 新規
				r = m_Read(m_work.GetSize(), m_work.Ref());
				if ( r == 0 )
				{
					r = m_GetResult(wait);
				}
			}
			if ( r != INVALID_SIZE && r != 0 )
			{
				memcpy(_P, m_work.Ref(), m_work.GetSize());
			}
		}
		return r;
	}

protected:

	/**
	 * [通知] 読込み.
	 *	@param[in] hFile ハンドル。
	 *	@param[in] size サイズ。
	 *	@param[out] _P 読込みバッファ。
	 *	@param[in,out] overLapped オーバーラップ情報。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnRead(HANDLE hFile, LPVOID _P, size_t size, OVERLAPPED& overLapped)
	{
		DWORD d;
		bool r = !! ::ReadFile(hFile, _P, size, &d, &overLapped);
		return r && (size == d); 
	}

private:

	/**
	 * [読込] 読込み.
	 *	@param[in] hFile FILE_FLAG_OVERLAPPED 付きでオープンしたハンドル。
	 *	@param[in] size サイズ。
	 *	@param[out] _P 読込みバッファ。
	 *	@retval 0 非同期読込み中。
	 *	@retval INVALID_SIZE 読み込みエラー。
	 *	@retval 上記以外 読込み成功。
	 */
	size_t m_Read(size_t size, LPVOID _P)
	{
		m_Cancel();
		Zero(m_overLapped);
		if ( m_hEvent == NULL )
		{
			m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
			::ResetEvent(m_hEvent);
		}
		m_overLapped.hEvent = m_hEvent; 
		DWORD work = ULONG_MAX;
		if ( OnRead(m_hFile, _P, size, m_overLapped) )
		{
			// 成功
			return size;
		}
		else if ( ::GetLastError() == ERROR_IO_PENDING )
		{
			// 非同期読み取り操作の結果をチェックする。
			::Sleep(0);
			if( ::GetOverlappedResult(m_hFile, &m_overLapped, &work, FALSE) )
			{
				return work;
			}
			else if ( ::GetLastError() == ERROR_IO_INCOMPLETE )
			{
				m_isOverLapping = true;
				return 0;
			}
		}
		return INVALID_SIZE;
	}
	/**
	 * [取得] 結果取得.
	 *	@param[in] wait 待ち時間(ms)。 INFINITE を指定すると、結果が帰るまで Waitします。
	 *	@retval 0 タイムアウト（まだ結果無し）。
	 *	@retval INVALID_SIZE 読み込みエラー。
	 *	@retval 上記以外 読込み成功。
	 */
	size_t m_GetResult(DWORD wait = 0)
	{
		if ( ! m_isOverLapping )
		{
			return INVALID_SIZE;
		}
		size_t result = INVALID_SIZE;
		DWORD r = 0;
		switch ( ::WaitForSingleObject(m_overLapped.hEvent, wait) )
		{
		case WAIT_OBJECT_0:
			if ( ::GetOverlappedResult(m_hFile, &m_overLapped, &r, FALSE) )
			{
				result = r; // 成功
				m_Cancel();
			}
			break;
		case WAIT_TIMEOUT:
			result = 0;
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
		default:
			m_Cancel();
			break;
		}
		return result;
	}
	/**
	 * [取得] 結果取得キャンセル.
	 *	@note 以降 m_GetResult() は失敗します。
	 */
	void m_Cancel(void)
	{
		if ( m_isOverLapping )
		{
			::CancelIo(m_hFile);
			m_isOverLapping = false;
		}
	}
	HANDLE			m_hEvent;		///< イベントハンドル
	bool			m_isOverLapping;///< オーバーラップ中
	OVERLAPPED		m_overLapped;	///< オーバーラップ管理
	HANDLE			m_hFile;		///< 読込みハンドル
	CWorkMem		m_work;			///< 読込みワーク
	CSyncSection	m_sync;
};



/**@ingroup WINAPI
 * オーバーラップ入力管理クラス.
 *
 *		API ::DeviceIoControl() による非同期の読込みをサポートします。読み込む情報がない時にロックさえるデバイスからの読込みに有効です。
 *		
 *	@par必要ファイル
 *			TnbOverlapReader.h
 *
 *	@date 07/09/13 新規作成
 *	@date 08/12/19 IsReading() 新規。
 */
class COverlapIoReader : public COverlapReader
{
	DEFSUPER(COverlapReader);
public:

	/// コンストラクタ
	COverlapIoReader(void) : _super(), m_dwIoControlCode(0)
	{
	}

	/**
	 * [設定] コントロールコード設定.
	 *	@note DeviceIoControl() の 第二引数を指定できます。
	 *	@param code コントロールコード
	 */
	void SetIoControlCode(DWORD code)
	{
		m_dwIoControlCode = code;
	}

protected:
	/**
	 * [通知] 読込み.
	 *	@param[in] hFile ハンドル。
	 *	@param[in] size サイズ。
	 *	@param[out] _P 読込みバッファ。
	 *	@param[in,out] overLapped オーバーラップ情報。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnRead(HANDLE hFile, LPVOID _P, size_t size, OVERLAPPED& overLapped)
	{
		DWORD dwByteReturn;
		BOOL ret = ::DeviceIoControl(hFile, m_dwIoControlCode, _P, size, _P, size, &dwByteReturn, &overLapped);
		return !! ret;
	}
private:
	DWORD m_dwIoControlCode;		///< DeviceIoControlの第二引数
};



}; // TNB
