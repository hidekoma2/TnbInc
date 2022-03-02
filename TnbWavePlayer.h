#pragma once
/**
 *@file
 * WAVE再生関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * WAVE再生クラス
 *	
 *	@par必要ファイル
 *			TnbWavePlayer.h
 *
 *	@date 08/08/12	新規作成
 */
class CWavePlayer : CCopyImpossible
{
public:
	/// コンストラクタ
	CWavePlayer(void) : m_bankNumber(0), m_hWave(NULL), m_currentBank(0), m_pBuffers(NULL), m_pSyncBank(NULL), m_hasBuffer(false)
	{
	}

	/// デストラクタ
	~CWavePlayer(void)
	{
		Close();
	}

	/// クローズ
	void Close(void)
	{
		if ( m_hWave != NULL )
		{
			::waveOutReset(m_hWave);
			::waveOutClose(m_hWave);
			m_hWave = NULL;
		}
		if ( m_pBuffers != NULL )
		{
			delete[] m_pBuffers;
			m_pBuffers = NULL;
		}
		if ( m_pSyncBank != NULL )
		{
			delete m_pSyncBank;
			m_pSyncBank = NULL;
		}
		m_bankNumber = 0;
		m_currentBank = 0;
	}

	/** 
	 * [初期] オープン
	 *	@param format フォーマット
	 *	@param bankNumber バンク数
	 *	@param bankSize 各バンクサイズ
	 *	@retval MMSYSERR_NOERROR 成功
	 *	@retval MMSYSERR_NOERROR以外 失敗
	 */
	MMRESULT Open(const WAVEFORMATEX& format, int bankNumber, UINT bankSize)
	{
		Close();
		m_bankNumber = bankNumber;
		m_pSyncBank = new CSyncSemaphore(NULL, bankNumber);
		MMRESULT mr = ::waveOutOpen(&m_hWave, WAVE_MAPPER, &format,
							reinterpret_cast<DWORD_PTR>(ms_WaveCallback),
							reinterpret_cast<DWORD_PTR>(m_pSyncBank), CALLBACK_FUNCTION);
		if ( mr == MMSYSERR_NOERROR )
		{
			m_pBuffers = new CBuffer[m_bankNumber];
			loop ( i, m_bankNumber )
			{
				mr = m_pBuffers[i].Init(m_hWave, bankSize);
				if ( mr != MMSYSERR_NOERROR )
				{
					break;
				}
			}
			m_hasBuffer = false;
		}
		if ( mr != MMSYSERR_NOERROR )
		{
			Close();
		}
		return mr;
	}

	/**
	 * [書込] 再生
	 *	@note バッファが一杯なれば再生しますが、一杯にならなければ、再生しません。
	 *	@note バッファイに空きがなければ、関数をリターンしません。
	 *	@param[out] _isPlayed true なら再生処理実行、 falseならバッファリング処理を実施。
	 *	@param[in] pData データ
	 *	@param[in] size サイズ
	 *	@retval MMSYSERR_NOERROR 成功
	 *	@retval MMSYSERR_NOERROR以外 失敗
	 */
	MMRESULT Write(bool& _isPlayed, const BYTE* pData, size_t size)
	{
		if ( m_bankNumber == 0 ) { return MMSYSERR_ERROR; }
		size_t nWritten = 0;
		while ( size > 0 ) 
		{
			if ( ! m_hasBuffer )
			{
				m_pSyncBank->Lock();
				m_hasBuffer = true;
			}
			bool isPlayed;
			MMRESULT r = m_pBuffers[m_currentBank].Write(isPlayed, nWritten, pData, size);
			if ( r != MMSYSERR_NOERROR )
			{
				return r;
			}
			if ( isPlayed )
			{
				m_hasBuffer = false;
				m_currentBank = (m_currentBank + 1) % m_bankNumber;
			}
			size -= nWritten;
			pData += nWritten;
		}
		return MMSYSERR_NOERROR;
	}

	/**
	 * [書込] 再生
	 *	@note バッファに、データがあれば再生し、バッファをクリアします。
	 *	@retval MMSYSERR_NOERROR 成功
	 *	@retval MMSYSERR_NOERROR以外 失敗
	 */
	MMRESULT Flush(void)
	{
		MMRESULT r = MMSYSERR_NOERROR;
		if ( m_bankNumber != 0 )
		{
			if ( m_hasBuffer )
			{
				r = m_pBuffers[m_currentBank].Flush();
				m_hasBuffer = false;
			}
			m_currentBank = (m_currentBank + 1) % m_bankNumber;
		}
		return r;
	}

	/**
	 * [設定] クリア
	 */
	void Clear(void)
	{
		loop ( i, m_bankNumber )
		{
			m_pBuffers[i].Clear();
		}
		m_currentBank = 0;
	}
	
	/// [処理] 一時停止
	void Pause(void)
	{
		::waveOutPause(m_hWave);
	}

	/// [処理] 再スタート
	void Restart(void)
	{
		::waveOutRestart(m_hWave);
	}

	/// [再生] 停止
	void Reset(void)
	{
		::waveOutReset(m_hWave);
	}

	/**
	 * [設定] ボリューム設定.
	 *	@note 左右別々に設定は出来ません。
	 *	@param vol ボリューム ０～１２７
	 */
	void SetVolume(int vol)
	{
		DWORD	lvol = vol * 512;
		DWORD	rvol = vol * 512;
		::waveOutSetVolume(m_hWave, MAKELONG(lvol, rvol));
	}

	/**
	 * [取得] ボリューム取得.
	 *	@return ボリューム ０～１２７.
	 */
	int GetVolume(void) const 
	{
		DWORD	vol;
		::waveOutGetVolume(m_hWave, &vol);
		return LOWORD(vol) / 512;
	}

private:
	/// バッファ管理クラス
	class CBuffer
	{
		WAVEHDR		m_hdr;			///< WAVE情報
		HWAVEOUT	m_hWave;		///< ハンドル
		size_t		m_sentBytes;	///< 送信バイト数
	public:
		/// コンストラクタ
		CBuffer(void) : m_hWave(NULL), m_sentBytes(0)
		{
			Zero(m_hdr);
		}
		/// デストラクタ
		~CBuffer(void)
		{
			if ( m_hdr.lpData != NULL )
			{
				::waveOutUnprepareHeader(m_hWave, &m_hdr, sizeof(WAVEHDR));
				::LocalFree(m_hdr.lpData);
				m_hdr.lpData = NULL;
			}
		}
		/**
		 * [設定] 初期化
		 *	@param hWave WAVEハンドル
		 *	@param size バッファサイズ
		 *	@retval MMSYSERR_NOERROR 成功
		 *	@retval MMSYSERR_NOERROR以外 失敗
		 */
		MMRESULT Init(HWAVEOUT hWave, UINT size)
		{
			m_hWave = hWave;
			m_sentBytes = 0;
			Zero(m_hdr);
			m_hdr.lpData = static_cast<LPSTR>(::LocalAlloc(LMEM_FIXED, size));
			if ( m_hdr.lpData == NULL )
			{
				return MMSYSERR_ERROR;
			}
			m_hdr.dwBufferLength = size;
			return ::waveOutPrepareHeader(hWave, &m_hdr, sizeof(WAVEHDR));
		}
		/// [設定] フラッシュ
		MMRESULT Flush(void)
		{
			size_t s = m_hdr.dwBufferLength;
			m_hdr.dwBufferLength = m_sentBytes;
			MMRESULT r = ::waveOutWrite(m_hWave, &m_hdr, sizeof(WAVEHDR));
			m_sentBytes = 0;
			m_hdr.dwBufferLength = s;
			return r;
		}
		/// [設定]
		void Clear(void)
		{
			m_sentBytes = 0;
		}
		/**
		 * [書込] 再生
		 *	@param[out] _isPlayed true なら再生処理実行、 falseならバッファリング処理を実施。
		 *	@param[out] _writtenSize 書込みサイズ(size よリ小さくなります)
		 *	@param[in] pData データ
		 *	@param[in] size サイズ
		 *	@retval MMSYSERR_NOERROR 成功
		 *	@retval MMSYSERR_NOERROR以外 失敗
		 */
		MMRESULT Write(bool& _isPlayed, size_t& _writtenSize, const BYTE* pData, size_t size)
		{
			_writtenSize = min(m_hdr.dwBufferLength - m_sentBytes, size);
			::CopyMemory(m_hdr.lpData + m_sentBytes, pData, _writtenSize);
			m_sentBytes += _writtenSize;
			if ( m_sentBytes == m_hdr.dwBufferLength )
			{
				m_sentBytes = 0;
				_isPlayed = true;
				return ::waveOutWrite(m_hWave, &m_hdr, sizeof(WAVEHDR));
			}
			_isPlayed = false;
			return MMSYSERR_NOERROR;
		}
	};
	/// Callback Function
	static void CALLBACK ms_WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
	{
		if ( uMsg == WOM_DONE )
		{
			CSyncSemaphore* P = reinterpret_cast<CSyncSemaphore*>(dwUser);
			P->Unlock();
		}
	}
	CSyncSemaphore*	m_pSyncBank;		///< バンク管理用排他
	HWAVEOUT		m_hWave;			///< WAVEハンドル
	int				m_bankNumber;		///< バンク数
	int				m_currentBank;		///< 現在のバンク
	CBuffer*		m_pBuffers;			///< バッファ
	bool			m_hasBuffer;		///< バッファもち
};



}; //TNB
