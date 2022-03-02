#pragma once
/**
 *@file
 * MCI 関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include <mmsystem.h>
#include <digitalv.h>
#pragma comment(lib, "winmm.lib")



//TNB Library
namespace TNB
{



/**@ingroup AUDIO ANIMATE
 * MCI プレイヤークラス.
 *
 *		MCI を使用して、 MP3 ファイルや MIDI ファイルを再生します。
 *
 *	@todo 特定の Windows7 64bitの環境において、CLOSE 時に imjptip.dll で Access Violation が発生する。原因不明。
 *
 *	@par必要ファイル
 *			TnbMciPlayer.h
 *
 *	@date 10/05/25	新規作成
 *	@date 10/06/25	MIDI だけでなく、 MCI 関係なら再生できるように変更。
 *	@date 13/01/16	ヘッダ独立。動画ファイルにも暫定対応。
 */
class CMciPlayer : public CCopyImpossible
{
public:

	/// コンストラクタ
	CMciPlayer(void) : m_isPlayed(false), m_lastError(0), m_hParent(NULL), m_wDeviceId(0), m_hViewWnd(NULL), m_isPaused(false)
	{
	}

	/// デストラクタ
	~CMciPlayer(void)
	{
		Close();
	}

	/**
	 * [設定] 親ウィンドウ設定.
	 *		親ウィンドウを指定すると、そのウィンドウに MM_MCINOTIFY が通知されます。
	 *	@param hWnd 親ウィンドウ。 NULL を指定すると解除。
	 */
	void SetParentWnd(HWND hWnd)
	{
		m_hParent = hWnd;
	}

	/**
	 * [取得] デバイス ID 取得.
	 *	@retval 0 取得失敗。未オープン。
	 *	@retval 0以外 取得成功。値はデバイス ID。
	 */
	MCIDEVICEID GetDeviceId() const
	{
		return m_wDeviceId;
	}

	/**
	 * [設定] MIDI ファイルオープン.
	 *	@param lpszFileName ファイル名.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool OpenMidi(LPCTSTR lpszFileName)
	{
		LPCTSTR lpszDeviceType = reinterpret_cast<LPCTSTR>(MCI_DEVTYPE_SEQUENCER);
		return Open(lpszFileName, lpszDeviceType, true);
	}

	/**
	 * [設定] MP3 ファイルオープン.
	 *	@param lpszFileName ファイル名.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool OpenMp3(LPCTSTR lpszFileName)
	{
		LPCTSTR lpszDeviceType = _T("MPEGVideo");
		return Open(lpszFileName, lpszDeviceType, false);
	}

	/**
	 * [設定] WAVE ファイルオープン.
	 *	@param lpszFileName ファイル名.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool OpenWave(LPCTSTR lpszFileName)
	{
		LPCTSTR lpszDeviceType = reinterpret_cast<LPCTSTR>(MCI_DEVTYPE_WAVEFORM_AUDIO);
		return Open(lpszFileName, lpszDeviceType, true);
	}

	/**
	 * [設定] AVI ファイルオープン.
	 *	@note mpeg ファイルも対応。
	 *	@param lpszFileName ファイル名.
	 *	@param hViewWnd 表示ウィンドウハンドル。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool OpenAvi(LPCTSTR lpszFileName, HWND hViewWnd)
	{
		LPCTSTR lpszDeviceType = _T("MPEGVideo");	// _T("avivideo");
		return Open(lpszFileName, lpszDeviceType, false, hViewWnd);
	}

	/**
	 * [設定] クローズ.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Close(void)
	{
		bool r = true;
		if ( m_wDeviceId != 0 )
		{
			r = m_Cmd(MCI_CLOSE);
			if ( r )
			{
				m_isPaused = false;
				m_isPlayed = false;
				m_wDeviceId = 0;
				m_hViewWnd = NULL;
			}
		}
		return r;
	}

	/**
	 * [取得] オープン確認.
	 *	@retval true オープン済み.
	 *	@retval false 未オープン.
	 */
	bool IsOpened(void) const
	{
		return GetDeviceId() != 0;
	}

	/**
	 * [取得] 全演奏時間.
	 *		オープンした曲の全演奏時間を返します。
	 *	@param _ms 取得した時間(ms)が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool GetLength(DWORD& _ms) const
	{
		return m_GetStatus(_ms, MCI_STATUS_LENGTH);
	}

	/**
	 * [取得] モード取得.
	 *	@retval MCI_MODE_NOT_READY	エラー。
	 *	@retval MCI_MODE_STOP		停止中。
	 *	@retval MCI_MODE_PLAY		演奏中。
	 *	@retval MCI_MODE_PAUSE		一時停止中。
	 *	@retval MCI_MODE_SEEK		シーク中。
	 */
	DWORD GetMode(void) const
	{
		DWORD mode;
		if ( m_GetStatus(mode, MCI_STATUS_MODE) )
		{
			return mode;
		}
		return MCI_MODE_NOT_READY;
	}

	/**
	 * [設定] 演奏開始.
	 *	@note 演奏開始位置を変えたい場合、 Seek() を使います。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Play(void)
	{
		MCI_PLAY_PARMS mciPlay = { 0 };
		bool r = m_Cmd(MCI_PLAY, 0, mciPlay);
		if ( r )
		{
			m_isPlayed = true;
			m_isPaused = false;
		}
		return r;
	}

	/**
	 * [設定] 演奏停止.
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Stop(void)
	{
		bool r = m_Cmd(MCI_STOP);
		if ( r )
		{
			m_isPlayed = false;
		}
		return r;
	}
	
	/**
	 * [設定] 演奏中断・再開.
	 *	@note 演奏していない時は、何もしませんが、成功で返ります。
	 *	@param isPause true なら中断 false なら再開します。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Pause(bool isPause = true)
	{
		bool r = true;
		if ( m_isPlayed )
		{
			r = m_Cmd(isPause ? MCI_PAUSE : MCI_PLAY);
			if ( r )
			{
				m_isPaused = isPause;
			}
		}
		return r;
	}

	/**
	 * [取得] 一時停止中？
	 *	@retval true 一時停止中。
	 *	@retval false 否。
	 */
	bool IsPaused(void) const
	{
		if ( m_isPlayed && GetMode() == MCI_MODE_PAUSE )
		{
			return true;
		}
		return false;
	}

	/**
	 * [設定] シーク.
	 *	@param pos 頭からの時間(ms).
	 *	@param isResume true ならシーク後、再生を再開します。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Seek(DWORD pos = 0, bool isResume = true)
	{
		if ( pos == 0 )
		{
			return m_Cmd(MCI_SEEK, MCI_SEEK_TO_START);
		}
		MCI_SEEK_PARMS mciSeek = { 0 };
		mciSeek.dwTo = pos;
		bool r = m_Cmd(MCI_SEEK, MCI_TO, mciSeek);
		if ( r && isResume )
		{
			r = m_Cmd(MCI_PLAY);
		}
		return r;
	}

	/**
	 * [設定] 音量設定.
	 *	@param vol 音量 0～1000
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool SetVolume(DWORD vol)
	{
		MCI_DGV_SETAUDIO_PARMS parm = { 0 };
		parm.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		parm.dwValue = vol;
		return m_Cmd(MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, parm);
	}

	/**
	 * [取得] 演奏位置取得.
	 *		演奏中の位置を返します。
	 *	@param _ms 取得した時間(ms)が格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool GetPlayingPosition(DWORD& _ms) const
	{
		return m_GetStatus(_ms, MCI_STATUS_POSITION);
	}

	/**
	 * [設定] 表示RECT設定.
	 *	@note 動画の場合のみ使用可能です。
	 *	@param rect 表示範囲
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool SetViewRect(const RECT& rect)
	{
		MCI_DGV_PUT_PARMS parms = { 0 };
		parms.rc = rect;
		return m_Cmd(MCI_PUT, MCI_DGV_RECT | MCI_DGV_PUT_DESTINATION, parms);
	}

	/**
	 * [取得] 画像大きさ取得.
	 *	@note 動画の場合のみ使用可能です。
	 *	@param[out] _size 成功すると、大きさが格納されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool GetSourceSize(SIZE& _size) const
	{
		MCI_ANIM_RECT_PARMS parms = { 0 };
		if ( m_Cmd(MCI_WHERE, MCI_ANIM_WHERE_SOURCE, parms) )
		{
			_size.cx = parms.rc.right - parms.rc.left;
			_size.cy = parms.rc.bottom - parms.rc.top;
			return true;
		}
		return false;
	}

	/**
	 * [取得] ラストエラーコード取得.
	 *	@return エラーコード. GetErrorString() で文字列に変換することが可能です。
	 */
	MCIERROR GetLastError(void) const
	{
		return m_lastError;
	}

	/**
	 * [取得] エラー文字列取得.
	 *	@param e エラーコード. 
	 *	@return 文字列。
	 */
	static CStr GetErrorString(MCIERROR e)
	{
		CStr s;
		if ( e != MMSYSERR_NOERROR )
		{
			::mciGetErrorString(e, s.GetBuffer(1000), 1000);
			s.ReleaseBuffer();
		}
		return s;
	}

protected:

	/**
	 * [設定] オープン.
	 *	@param lpszFileName ファイル名.
	 *	@param lpszDeviceType デバイスタイプ名.
	 *	@param isDeviceTypeId デバイスタイプがIDの場合 true を指定します。
	 *	@param hViewWnd 表示ウィンドウハンドル。
	 *	@retval true 成功。
	 *	@retval false 失敗。詳細は GetLastError() で得られます。
	 */
	bool Open(LPCTSTR lpszFileName, LPCTSTR lpszDeviceType, bool isDeviceTypeId, HWND hViewWnd = NULL)
	{
		Stop();
		if ( IsOpened() )
		{
			Close();
		}
		DWORD p1 = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT | (isDeviceTypeId ? MCI_OPEN_TYPE_ID : 0);
		DWORD p2 = 0;
		if ( hViewWnd == NULL )
		{
			Zero(m_openParms);
			m_openParms.lpstrDeviceType = lpszDeviceType;
			m_openParms.lpstrElementName = lpszFileName;
			p2 = reinterpret_cast<DWORD>(&m_openParms);
		}
		else
		{
			Zero(m_animOpenParms);
			m_animOpenParms.hWndParent = hViewWnd;
			m_animOpenParms.dwStyle = WS_CHILD;
			m_animOpenParms.lpstrDeviceType = lpszDeviceType;
			m_animOpenParms.lpstrElementName = lpszFileName;
			p1 |= (MCI_ANIM_OPEN_PARENT | MCI_ANIM_OPEN_WS);
			p2 = reinterpret_cast<DWORD>(&m_animOpenParms);
		}
		bool r = m_Rst(::mciSendCommand(NULL, MCI_OPEN, p1, p2));
		if ( r )
		{
			m_hViewWnd = hViewWnd;
			if ( hViewWnd == NULL )
			{
				m_wDeviceId = m_openParms.wDeviceID;
			}
			else
			{
				m_wDeviceId = m_animOpenParms.wDeviceID;
			}
		}
		if ( r )
		{
			MCI_SET_PARMS mciSet = { 0 };
			mciSet.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			r = m_Cmd(MCI_SET, MCI_SET_TIME_FORMAT, mciSet);
		}
		if ( r && m_hViewWnd != NULL )
		{
			RECT rc;
			::GetClientRect(m_hViewWnd, &rc);
			MCI_DGV_PUT_PARMS parms = { 0 };
			parms.rc = rc;
			r = m_Cmd(MCI_PUT, MCI_DGV_RECT | MCI_DGV_PUT_DESTINATION, parms);
		}
		return r;
	}

	/**
	 * [設定] ラストエラーコード設定.
	 *	@param e エラーコード. 
	 */
	void SetLastError(MCIERROR e)
	{
		m_lastError = e;
	}

private:
	/// 戻り値チェック
	bool m_RstFx(MCIERROR r) const
	{
		m_lastError = r;
		return r == 0;
	}
	/// 戻り値チェック
	bool m_Rst(MCIERROR r) const
	{
		m_lastError = r;
		if ( r != 0 )
		{
			TRACE2("MCIERROR = %d[%s]\n", r, GetErrorString(r));
		}
		return r == 0;
	}
	/// コマンド実行
	template<typename TYP>
	bool m_Cmd(UINT msg, DWORD param, TYP& typ) const
	{
		if ( IsOpened() )
		{
			if ( m_hParent != NULL )
			{
				typ.dwCallback = reinterpret_cast<DWORD>(m_hParent);
				param |= MCI_NOTIFY;
			}
			return m_Rst(::mciSendCommand(GetDeviceId(), msg, param, reinterpret_cast<DWORD>(&typ)));
		}
		return m_RstFx(MCIERR_DEVICE_NOT_READY);
	}
	/// コマンド実行
	bool m_Cmd(UINT msg, DWORD param = 0) const
	{
		MCI_GENERIC_PARMS p = { 0 };
		return m_Cmd(msg, param, p);
	}
	/// 状態取得
	bool m_GetStatus(DWORD& _status, DWORD item) const
	{
		MCI_STATUS_PARMS mciStatus = { 0 };
		mciStatus.dwItem = item;
		bool r = m_Cmd(MCI_STATUS, MCI_STATUS_ITEM, mciStatus);
		if ( r )
		{
			_status = mciStatus.dwReturn;
		}
		return r;
	}
	MCI_OPEN_PARMS			m_openParms;		///< オーディオオープンパラメータ
	MCI_ANIM_OPEN_PARMS		m_animOpenParms;	///< ビデオオープンパラメータ
	MCIDEVICEID				m_wDeviceId;		///< オープン中のデバイスID @note 0 なら未オープン
	HWND					m_hViewWnd;			///< 表示ウィンドウハンドル
	HWND					m_hParent;			///< Notify 先ウィンドウ
	bool					m_isPlayed;			///< 演奏中？
	bool					m_isPaused;			///< 一時停止中
	mutable MCIERROR		m_lastError;		///< エラーコード
};



}; //TNB
