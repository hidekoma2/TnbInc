#pragma once
/**
 *@file
 * RS232Cのアクセス関係のヘッダ
 *
 *		シリアルの詳細設定は、 OnRs232cOpening() メソッドをオーバーライドして対応できます。
 *
 *	@note	マクロ "_TnbRS232C_EnumExistPorts_DISABLE" が定義されていると、 CRs232c::EnumExistPorts() , CRs232c::EnumExistPortDeviceNames(),
 *			CRs232c::EnumExistPortsByUsb() が使用禁止になります。その代わり、 TnbRegistryAccessor.h は不必要になります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifdef _WIN32_WCE
 #define _TnbRS232C_EnumExistPorts_DISABLE
#endif
#include "TnbCommunication.h"
#include "TnbStrEx.h"
#ifndef _TnbRS232C_EnumExistPorts_DISABLE
 #include "TnbRegistryAccessor.h"
 #include <SetupApi.h>
 #pragma comment(lib, "setupapi.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * RS232Cアクセスクラス
 *
 *		シリアルアクセスを行うクラスです。
 *
 *	@note 通信設定は、 SetParameter() を使って指定します。
 *		 より細かい設定は、本クラスを継承し、 OnRs232cOpening() を実装します。
 *		受信バッファサイズの変更は Win32API の ::SetupComm() を使用します。
 *
 *	@par必要ファイル
 *			TnbRs232c.h
 * 
 *	@date 06/01/01	新規作成
 *	@date 06/05/11	ICommunication 実装
 *	@date 06/07/14	Open時デフォルトでフローOFF設定を行なうようにした。
 *					CTS信号によるConnect/Disconnectイベントを上げるようにした。
 *					IsConnect()メソッドを追加。
 *	@date 06/08/31	CommViewer に対応。
 *	@date 06/09/08	Purge() / IsAlive() / IsConnect() 追加。 CanAccess() 廃止。
 *	@date 07/02/09	親クラス変更。
 *	@date 07/06/11	親クラスの Start() メソッドに対応。
 *	@date 07/09/13	親クラスの変更。
 *	@date 08/11/07	OnRs232cOpening() メソッド追加。
 *	@date 09/10/01	構成。 GetParameter() 追加。
 *	@date 09/10/22	ポートを文字列指定可能に。
 *	@date 09/11/11	SetParameter() の仕様拡張。
 *	@date 10/02/17	EnumExistPortDeviceNames() 追加。
 *	@date 10/05/28	SetReceiveTimeout() を実装。
 *	@date 10/06/29	SetReceiveTimeout() をダミー化。信号線制御メソッド用意。
 *	@date 11/02/03	SetCheckCtsMode() のデフォルトを false に。
 *	@date 11/03/15	Close() 後もプロパティのタイプID,名を変更しないようにした。
 *	@date 11/07/12	信号線操作メソッドの改修。
 *	@date 12/02/17	EnumExistPortDeviceNames() で USB の VID, PID の値も得られるようにした。
 *	@date 12/10/22	EnumExistPortsByUsb() を新規。
 *	@date 14/07/30	EnumExist～() の結果を sort するようにした。
 */
class CRs232c : public ICommunication, CCopyImpossible
{
public:

	/**@ingroup COMMUNICATION
	 * RS232C ポート管理
	 *
	 *	@par必要ファイル
	 *			TnbRs232c.h
	 * 
	 *	@date 09/10/22	新規
	 */
	class CPort
	{
	public:

		/// コンストラクタ
		CPort(void)
		{
		}

		/**
		 * コンストラクタ.
		 *		"COMx" と言うポート名になります。
		 *	@param portNo ポートナンバー。
		 */
		CPort(int portNo)
		{
			#ifndef _WIN32_WCE
				m_portName.Format(_T("\\\\.\\COM%d"), portNo);
			#else
				m_portName.Format(_T("COM%d:"), portNo);
			#endif
		}

		/**
		 * コンストラクタ.
		 *	@param lpszPortName ポート名。通常、 ASCII 3文字、数値、になります。
		 */
		CPort(LPCTSTR lpszPortName) : m_portName(lpszPortName)
		{
		}

		#ifdef __AFX_H__

		/**
		 * コンストラクタ.
		 *	@param portName ポート名。通常、 ASCII 3文字、数値、になります。
		 */
		CPort(const CString& portName) : m_portName(portName)
		{
		}

		#endif //__AFX_H__

		/**
		 * [取得] ポート名取得.
		 *	@return ポート名
		 */
		const CStr& GetPortName(void) const
		{
			return m_portName;
		}

		/**
		 * [取得] 数値取得.
		 *	@return 数値
		 */
		int GetNumber(void) const
		{
			LPCTSTR P1 = m_portName;
			LPCTSTR P2 = m_Find(P1);
			if ( P2 != NULL )
			{
				return _ttoi(P2);
			}
			return -1;
		}

		/**
		 * [取得] プリフィックス取得
		 *	@return プリフィックス
		 */
		CStr GetPrefix(void) const
		{
			CStr s;
			LPCTSTR P1 = m_portName;
			LPCTSTR P2 = m_Find(P1);
			if ( P2 != NULL )
			{
				s = m_portName.Left(P2 - P1);
			}
			return s;
		}

		/**
		 * [確認] 有効確認.
		 *	@retval true 有効
		 *	@retval false 無効
		 */
		bool IsValid(void) const
		{
			return ! m_portName.IsEmpty();
		}

	private:
		/// 切れ目検索 数値文字を探す
		LPCTSTR m_Find(LPCTSTR lpsz) const
		{
			while ( (*lpsz) != 0 )
			{
				if ( (*lpsz) >= '0' && (*lpsz) <= '9' )
				{
					return lpsz;
				}
				lpsz++;
			}
			return NULL;
		}
		CStr m_portName;
	};

	/// パリティ設定値
	enum EParity
	{
		Parity_Non		= NOPARITY,		///< なしパリティ
		Parity_Odd		= ODDPARITY,	///< 奇数パリティ
		Parity_Even		= EVENPARITY,	///< 偶数パリティ
		Parity_Mark		= MARKPARITY,	///< マークパリティ
		Parity_Space	= SPACEPARITY,	///< 空白パリティ
		//
		#ifndef _TnbDOXYGEN	//Document作成用シンボル
		EP_NonParity	= NOPARITY,		///< なしパリティ (旧シンボル)
		EP_OddParity	= ODDPARITY,	///< 奇数パリティ (旧シンボル)
		EP_EvenParity	= EVENPARITY,	///< 偶数パリティ (旧シンボル)
		EP_MarkParity	= MARKPARITY,	///< マークパリティ (旧シンボル)
		EP_SpaceParity	= SPACEPARITY,	///< 空白パリティ (旧シンボル)
		#endif // _TnbDOXYGEN
	};
	/// ストップビット設定値
	enum EStopBits
	{
		StopBits_1		= ONESTOPBIT,	///< 1bit
		StopBits_1_5	= ONE5STOPBITS,	///< 1.5bit
		StopBits_2		= TWOSTOPBITS,	///< 2bit
		//
		#ifndef _TnbDOXYGEN	//Document作成用シンボル
		ES_OneStopBit	= ONESTOPBIT,	///< 1bit (旧シンボル)
		ES_One5StopBits	= ONE5STOPBITS,	///< 1.5bit (旧シンボル)
		ES_TwoStopBits	= TWOSTOPBITS,	///< 2bit (旧シンボル)
		#endif // _TnbDOXYGEN
	};

	
	//--------------------------------


	/// コンストラクタ
	CRs232c(void) : m_property(_T("RS232C"), TNBVIW_KIND_RS232C), m_hComPort(INVALID_HANDLE_VALUE)
		, m_dwBaudRate(9600), m_bByteSize(7), m_eParity(Parity_Non)
		, m_eStopBits(StopBits_1), m_boCanBulkSend(true), m_boIsConnect(false), m_isCheckCts(false)
	{
		ASSERT( m_property.receiveTimeout == 0 );
	}

	/// デストラクタ
	~CRs232c(void)
	{
		CRs232c::Close();
	}

	/**
	 * [取得] オープン設定取得.
	 *		SetParameter() で設定した値を取得できます。
	 *	@param[out] _comPort ポートナンバー。
	 *	@param[out] _dwBaudRate 転送速度[bps]
	 *	@param[out] _bByteSize キャラクタビット[bits]
	 *	@param[out] _eParity パリティ（偶数／奇数）
	 *	@param [out] _eStopBits ストップビット[bits]
	 */
	void GetParameter(CPort& _comPort, DWORD& _dwBaudRate, BYTE& _bByteSize, EParity& _eParity, EStopBits& _eStopBits) const
	{
		_comPort = m_comPort;
		_dwBaudRate = m_dwBaudRate,
		_bByteSize = m_bByteSize;
		_eParity = m_eParity;
		_eStopBits = m_eStopBits;
	}

	/**
	 * [設定] オープン設定.
	 *		RS232Cの設定を行います。通常、 Open の前に使用します。
	 *	@note Open 前に実行した場合必ず成功します。
	 *	@note オープン中、本メソッドでCOMポートナンバーを変更した場合、クローズされます。
	 *	@note オープン中、本メソッドを使用した場合、エラーになる可能性もあります。
	 *	@param comPort COMポートナンバー。 数値でも文字列でも指定可能です。
	 *	@param dwBaudRate 転送速度[bps]
	 *	@param bByteSize キャラクタビット[bits]
	 *	@param eParity パリティ（偶数／奇数）
	 *	@param eStopBits ストップビット[bits]
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD SetParameter(CPort comPort, DWORD dwBaudRate, BYTE bByteSize, EParity eParity, EStopBits eStopBits)
	{
		if ( m_comPort.GetNumber() != comPort.GetNumber() )
		{
			Close();
			m_comPort = comPort;
		}
		m_dwBaudRate = dwBaudRate,
		m_bByteSize = bByteSize;
		m_eParity = eParity;
		m_eStopBits = eStopBits;
		if ( m_hComPort != INVALID_HANDLE_VALUE )
		{
			return m_PortSetting(m_hComPort);
		}
		return 0;
	}

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@return false 失敗。サポートできない時もこれが返ります。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return false;
	}

	/**
	 * [設定] 送信設定.
	 *		送信設定をおこないます。デフォルトはバルク(一括)モードです。
	 *	@param canBulk ture ならバルク(一括)モードで送信します。 false なら、一バイトずつ送信します。
	 */
	void SetSendMode(bool canBulk)
	{
		m_boCanBulkSend = canBulk;
	}

	/**
	 * [設定] CTSチェック設定.
	 *		CTS信号が立っている時だけ接続とするモードと、CTS信号を無視するモードを選択できます。
	 *	@param r ture なら CTS信号をチェックします。デフォルトはチェックしません。
	 */
	void SetCheckCtsMode(bool r = true)
	{
		m_isCheckCts = r;
	}
	
	/**
	 * [取得] 性能取得
	 *	@note 正確な情報は Open後に取得できます。
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property;
	}

	/**
	 * [取得] ハンドル取得
	 *	@retval NULL オープンしていない。
	 *	@retval NULL以外 ハンドル
	 */
	HANDLE GetHandle(void) const
	{
		return m_hComPort;
	}

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool Open(void)
	{
		Close();
		if ( m_Open() == ERROR_SUCCESS )
		{
			m_property.typeName = CStr::Fmt(_T("RS232C(%s%d)"), m_comPort.GetPrefix(), m_comPort.GetNumber());		// タイプ名。
			m_property.typeId = TNBVIW_KIND_RS232C + (m_comPort.GetNumber() & 0xFFFF);
			return true;
		}
		return false;
	}

	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void)
	{
		if ( m_hComPort != INVALID_HANDLE_VALUE )
		{
			// ポートの設定を取得
			HANDLE h = m_hComPort;
			m_hComPort = INVALID_HANDLE_VALUE;
			DCB	tDCB;
			if ( ::GetCommState(h, &tDCB) )
			{
				tDCB.fRtsControl = RTS_CONTROL_DISABLE;
				tDCB.fDtrControl = DTR_CONTROL_DISABLE;
				// ポートの状態を設定
				::SetCommState(h, &tDCB);
			}
			::CloseHandle(h);
			m_boIsConnect = false;
		}
	}

	/**
	 * [確認] オープン確認.
	 *	@retval true オープン中。
	 *	@retval false クローズ中。
	 */
	virtual bool IsOpened(void) const
	{
		return m_hComPort != INVALID_HANDLE_VALUE;
	}

	/**
	 * [確認] 接続確認.
	 *	@note Check() の結果を返します。
	 *	@retval true 接続中。
	 *	@retval false 切断中。
	 */
	virtual bool IsConnect(void) const
	{
		return m_boIsConnect;
	}

	/**
	 * [確認] 受信データ確認.
	 *	@retval INVALID_SIZE 不明。
	 *	@retval 上記以外 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		// 現在の状態を取得。
		DWORD dwErrors;
		COMSTAT tComStat;
		if ( ! ::ClearCommError(m_hComPort, &dwErrors, &tComStat) )
		{
			_GetLastError("ClearCommError");
			return INVALID_SIZE;
		}
		// 現在受信しているDataを取得
		return tComStat.cbInQue;
	}

	/**
	 * [確認] チェック.
	 *		状態を確認します。定期的に呼び出す必要があります。
	 *	@note CONNECTING は、 CTS 信号により判断しますので、 SetCheckCtsMode() で true を指定している必要があります。
	 *	@retval OPENNING	オープン状態
	 *	@retval CONNECTING	オープン＆接続状態
	 *	@retval CLOSED		クローズ状態	
	 */
	virtual EState Check(void) const
	{
		if ( IsOpened() )
		{
			DWORD dwStat;
			if ( ! ::GetCommModemStatus(m_hComPort, &dwStat) )
			{
				_GetLastError("GetCommModemStatus");
			}
			else if ( m_isCheckCts )
			{
				m_boIsConnect = (dwStat & MS_CTS_ON) != 0;
				return m_boIsConnect ? CONNECTING : OPENNING;
			}
			else
			{
				m_boIsConnect = true;
				return CONNECTING;
			}
		}
		return CLOSED;
	}

	/**
	 * [処理] 送信.
	 *	@param size サイズ。
	 *	@param P データ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 上記以外 送信データサイズ。０の可能性もあります。
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( ! IsOpened() )
		{
			return INVALID_SIZE;
		}
		if ( m_boCanBulkSend )
		{
			//=== 一括送信
			const BYTE* B = static_cast<const BYTE*>(P);
			size_t r = 0;
			size_t writtenSize = 0;
			while ( true )
			{
				if ( ! WritePort(writtenSize, size, B, m_hComPort) )
				{
					return INVALID_SIZE;
				}
				if ( writtenSize == 0 )
				{
					break;
				}
				r += writtenSize;
				if ( size <= writtenSize )
				{
					break;
				}
				size -= writtenSize;
				B += writtenSize;
			}
			return r;
		}
		//=== 一バイトずつ送信
		const BYTE* B = static_cast<const BYTE*>(P);
		for ( UINT i = 0; i < size; i++ )
		{
			int iErrCnt = 10;			// エラーカウント
			size_t writtenSize = 0;
			while ( writtenSize == 0 && iErrCnt > 0 )
			{
				if ( ! WritePort(writtenSize, 1, &B[i], m_hComPort) )
				{
					// WriteFile関数がエラーを返した
					return INVALID_SIZE;
				}
				if ( writtenSize != 1 )
				{
					iErrCnt --;
					Sleep(1);
				}
			}
			if ( iErrCnt < 1 )
			{
				return INVALID_SIZE;
			}
		}
		return size;
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
		DWORD dwErrors;
		COMSTAT tComStat;
		if ( ! ::ClearCommError(m_hComPort, &dwErrors, &tComStat) )
		{
			_GetLastError("ClearCommError");
			return INVALID_SIZE;
		}
		if ( size > tComStat.cbInQue )
		{
			size = tComStat.cbInQue;
		}
		if ( size == 0 )
		{
			return 0;
		}
		size_t redSize  = 0;
		if ( ReadPort(redSize, _P, size, m_hComPort) )
		{
			return redSize;
		}
		return INVALID_SIZE;
	}

	/**
	 * [設定] パージ.
	 *		通信のゴミを浄化
	 */
	virtual void Purge(void)
	{
		::PurgeComm(m_hComPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	}

	/**
	 * [操作] パディング読み込み
	 *	@note オープンしているポートで空読み込みをします。
	 *	@param dwTime 受信情報なしの時間指定。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool PaddingReceive(DWORD dwTime)
	{
		if ( ! IsOpened() )
		{
			return false;
		}
		//
		bool boResult = false;
		DWORD dwLoopCount = 0;
		//
		while ( true )
		{
			// 現在の状態を取得。
			BOOL boIsClearComm;
			DWORD dwErrors;
			COMSTAT tComStat;
			// エラー状態リセット&受信文字数確認
			boIsClearComm = ::ClearCommError(m_hComPort, &dwErrors, &tComStat);
			if ( ! boIsClearComm )
			{
				boResult = false;
				break;
			}
			//
			// 現在受信しているDataを取得
			DWORD dwReceivedDataCount = tComStat.cbInQue;
			//
			if ( dwReceivedDataCount == 0 )
			{
				dwLoopCount++;
				if ( dwLoopCount * 10 > dwTime )
				{
					boResult = true;
					break;
				}
			}
			else
			{
				dwLoopCount = 0;
				//
				CWorkMem tmp(dwReceivedDataCount);
				// RS232Cからの読み込み
				size_t redSize = 0;
				if ( ! ReadPort(redSize, tmp.Ref(), tmp.GetSize(), m_hComPort) )
				{
					//失敗
					boResult = false;
					break;
				}
				if ( redSize == 0 )
				{
					boResult = true;
					break;
				}
			}
			::Sleep(10);
		}
		//
		return boResult;
	}

	/// 信号線状態.
	enum
	{
		LOW	 = true,	///< LOW (ON)
		HI = false		///< HI (OFF)
	};

	/**
	 * [設定] DTR 設定.
	 *		DTR(data-terminal-ready)信号の状態を設定します。
	 *	@note JIS での名称は「ER（データ端末レディ）」。
	 *	@par クロスケーブルの場合
	 *		\code
	 *			DTR(4) ----------> DSR(6)
	 *			DSR(6) <---------- DTR(4)
	 *		\endcode
	 *	@param isLow 状態。 CRs232c::LOW か CRs232c::HI を指定。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetDtrSignal(bool isLow)
	{
		return !! ::EscapeCommFunction(m_hComPort, isLow ? SETDTR : CLRDTR);
	}

	/**
	 * [設定] RTS 設定.
	 *		RTS(request-to-send)信号の状態を設定します。
	 *	@note JIS での名称は「RS（送信要求）」。
	 *	@par クロスケーブルの場合
	 *		\code
	 *			RTS(7) ----------> CTS(8)
	 *			CTS(8) <---------- RTS(7)
	 *		\endcode
	 *	@param isLow 状態。 CRs232c::LOW か CRs232c::HI を指定。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetRtsSignal(bool isLow)
	{
		return !! ::EscapeCommFunction(m_hComPort, isLow ? SETRTS : CLRRTS);
	}

	/**
	 * [取得] CTS 確認.
	 *		CTS(clear-to-send)信号の状態を確認します。
	 *	@note JIS での名称は「CS（送信可）」。
	 *	@par クロスケーブルの場合
	 *		\code
	 *			RTS(7) ----------> CTS(8)
	 *			CTS(8) <---------- RTS(7)
	 *		\endcode
	 *	@retval true LOW(ON)
	 *	@retval false HI(OFF)
	 */
	bool IsCtsSignalLow(void) const
	{
		DWORD w;
		if ( ::GetCommModemStatus(m_hComPort, &w) )
		{
			return (w & MS_CTS_ON) != 0;
		}
		return false;
	}

	/**
	 * [取得] DSR 確認.
	 *		DSR(data-set-ready)信号の状態を確認します。
	 *	@note JIS での名称は「DR（データセットレディ）」。
	 *	@par クロスケーブルの場合
	 *		\code
	 *			DTR(4) ----------> DSR(6)
	 *			DSR(6) <---------- DTR(4)
	 *		\endcode
	 *	@retval true LOW(ON)
	 *	@retval false HI(OFF)
	*/
	bool IsDsrSignalLow(void) const
	{
		DWORD w;
		if ( ::GetCommModemStatus(m_hComPort, &w) )
		{
			return (w & MS_DSR_ON) != 0;
		}
		return false;
	}

	/**
	 * [取得] RING 確認.
	 *		RING 信号の状態を確認します。
	 *	@note 別表記として「RI」。
	 *	@retval true LOW(ON)
	 *	@retval false HI(OFF)
	*/
	bool IsRingSignalLow(void) const
	{
		DWORD w;
		if ( ::GetCommModemStatus(m_hComPort, &w) )
		{
			return (w & MS_RING_ON) != 0;
		}
		return false;
	}

	/**
	 * [取得] RLSD 確認.
	 *		RLSD(receive-line-signal-detect)信号の状態を確認します。
	 *	@note 別表記として「DCD」。
	 *	@retval true LOW(ON)
	 *	@retval false HI(OFF)
	*/
	bool IsRlsdSignalLow(void) const
	{
		DWORD w;
		if ( ::GetCommModemStatus(m_hComPort, &w) )
		{
			return (w & MS_RLSD_ON) != 0;
		}
		return false;
	}

	/**
	 * [取得] CTS 確認.
	 *	@note IsCtsSignalLow() を使用してください。
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	 */
	bool IsCtsSignalOn(void) const
	{
		return IsCtsSignalLow();
	}

	/**
	 * [取得] DSR 確認.
	 *	@note IsDsrSignalLow() を使用してください。
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	*/
	bool IsDsrSignalOn(void) const
	{
		return IsDsrSignalLow();
	}

	/**
	 * [取得] RING 確認.
	 *	@note IsRingSignalLow() を使用してください。
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	*/
	bool IsRingSignalOn(void) const
	{
		return IsRingSignalLow();
	}

	/**
	 * [取得] RLSD 確認.
	 *	@note IsRlsdSignalLow() を使用してください。
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	*/
	bool IsRlsdSignalOn(void) const
	{
		return IsRlsdSignalLow();
	}

	/**
	 * [確認] 指定ポートは開けるか
	 *	@param comPort COMポートナンバー。 数値でも文字列でも指定可能です。
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	static DWORD CanPortBeOpened(CPort comPort)
	{
		// COMポート名作成
		CStr strPortName = comPort.GetPortName();
		// ポートを開く
		HANDLE hComPort = ::CreateFile(
						strPortName,					// ファイルの名前のアドレス
						GENERIC_READ | GENERIC_WRITE,	// アクセス (読み取りおよび書き込み) モード
						0,								// 共有モード
						NULL,							// セキュリティ記述子のアドレス
						OPEN_EXISTING,					// 作成方法
						FILE_ATTRIBUTE_NORMAL,			// ファイル属性
						NULL							// コピーする属性付きファイルのハンドル
		);
		// ポートを開く関数が異常を返した場合
		if ( hComPort == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateFile");
		}
		::CloseHandle(hComPort);
		return 0;
	}

	#ifndef _TnbRS232C_EnumExistPorts_DISABLE

	/// デバイス情報
	///	@note vid, pid は USB デバイスでない場合は all 0 になります。
	struct TDevice : IComparableT<TDevice>
	{
		WORD	port;			///< ポートNo
		CStr	description;	///< 説明
		CStr	manufacturer;	///< 製造業者
		WORD	vid;			///< VendorID (USB)
		WORD	pid;			///< ProductID (USB)
		/// コンストラクタ
		TDevice(void) : port(0), vid(0), pid(0)
		{
		}
		/**
		 * [取得] 文字列化.
		 *	@note メーカー、説明、ポートNoを含んだ文字列を返します。
		 *	@return 文字列.
		 */
		CStr ToString(void) const
		{
			if ( description.Find(manufacturer) == 0 )
			{
				return CStr::Fmt(_T("%s (COM%d)"), description, port);
			}
			return CStr::Fmt(_T("%s %s (COM%d)"), manufacturer, description, port);
		}
		/**
		 * [確認] 比較
		 *	@param t 比較対象
		 *	@retval ０		自分は t と同じ
		 *	@retval １以上	自分は t より大きい
		 *	@retval ０未満	自分は t より小さい
		 */
		virtual INT_PTR Compare(const TDevice& t) const
		{
			return port - t.port;
		}
	};

	/// デバイス情報配列.
	typedef CVectorT<TDevice> TDeviceVector;

	/**
	 * [取得] 存在するPort一覧
	 *	@return 存在するポートNo、名前一覧（Sizeが２でport内容が 1,4 なら "COM1" "COM4"が使用可能）
	 */
	static TDeviceVector EnumExistPortDeviceNames(void)
	{
		CMyFinder ff;
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS);
		return ff.Execute(reg[_T("SYSTEM\\CurrentControlSet\\Enum")]);
	}

	/**
	 * [取得] 存在するPort一覧
	 *	@return 存在するポートNo一覧（Sizeが２で内容が 1,4 なら "COM1" "COM4"が使用可能）
	 */
	static CWordVector EnumExistPorts(void)
	{
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS);
		CRegistryAccessor::CSection regsub = reg[_T("HARDWARE\\DEVICEMAP\\SERIALCOMM")];
		CStrVector vstrNames = regsub.EnumKeyNames();
		//
		CWordVector vw;
		loop ( i, vstrNames.GetSize() )
		{
			CStr str = regsub.QueryString(vstrNames[i]);
			if ( str.GetLength() >= 4 )
			{
				int iPort = str.ToInt(3);
				if ( iPort >= 0 && iPort <= 256 )
				{
					vw.Add(static_cast<WORD>(iPort));
				}
			}
		}
		vw.Sort();
		return vw;
	}

	/**
	 * [取得] USBシリアルPort一覧.
	 *	@param vid	VendorID (USB)
	 *	@param pid	ProductID (USB)
	 *	@return 存在するポートNo一覧（Sizeが２で内容が 1,4 なら "COM1" "COM4"が使用可能）
	 */
	static CWordVector EnumExistPortsByUsb(WORD vid, WORD pid)
	{
		CWordVector vw;
		TDeviceVector vd = EnumExistPortDeviceNames();
		loop ( i, vd )
		{
			const TDevice& d = vd[i];
			if ( d.vid == vid && d.pid == pid )
			{
				vw.Add(d.port);
			}
		}
		vw.Sort();
		return vw;
	}

	#endif

protected:

	/**
	 * [通知] オープン通知.
	 *		オープン時、コールされます。
	 *	@param[in] hComPort ハンドル
	 *	@param[in,out] _tDCB 現在の DCB が渡されます。変更することで設定を変更できます。 
	 *	@param[in,out] _tCommTimeOuts 現在の COMMTIMEOUTS が渡されます。変更することで設定を変更できます。
	 *	@retval true オープン処理継続。
	 *	@retval false オープン処理中止（Openが失敗します）。
	 */
	virtual bool OnRs232cOpening(HANDLE hComPort, DCB& _tDCB, COMMTIMEOUTS& _tCommTimeOuts)
	{
		return true;
	}

	/**
	 * [通知] ポート作成.
	 *		ポート作成時、コールされます。
	 *	@param lpszPortName ポート名
	 *	@retval INVALID_HANDLE_VALUE 失敗。
	 *	@retval 上記以外 成功。値はハンドル。使用後は CloseHandle() してください。
	 */
	virtual HANDLE CreatePort(LPCTSTR lpszPortName) const
	{
		DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;	// アクセス (読み取りおよび書き込み) モード
		DWORD flag = FILE_ATTRIBUTE_NORMAL;
		HANDLE h = ::CreateFile(lpszPortName, desiredAccess, 0, NULL, OPEN_EXISTING, flag, NULL);
		_GetLastError("CreateFile");
		return h;
	}

	/**
	 * [通知] ポート書き込み.
	 *		ポート書き込み時、コールされます。
	 *	@param[out] _writtenSize 書き込んだサイズが格納されます。
	 *	@param[in] len 書き込みデータサイズ
	 *	@param[in] P 書き込みデータ
	 *	@param[in] h ハンドル
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool WritePort(size_t& _writtenSize, size_t len, LPCVOID P, HANDLE h) const
	{
		DWORD dwWritten = 0;
		if ( ! ::WriteFile(h, P, ToDword(len), &dwWritten, NULL) )
		{
			_GetLastError("WriteFile");
			return false;
		}
		_writtenSize = dwWritten;
		return true;
	}

	/**
	 * [通知] ポート読み込み.
	 *		ポート読み込み時、コールされます。
	 *	@param[out] _redSize 書き込んだサイズが格納されます。
	 *	@param[out] _P 読み込みデータが格納されます。
	 *	@param[in] len 読み込みデータ最大
	 *	@param[in] h ハンドル
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool ReadPort(size_t& _redSize, LPVOID _P, size_t len, HANDLE h) const
	{
		DWORD dwRed = 0;
		if ( ! ::ReadFile(h, _P, ToDword(len), &dwRed, NULL) )
		{
			_GetLastError("ReadFile");
			return false;
		}
		_redSize = dwRed;
		return true;
	}

private:

	/**
	 * RS232Cをオープン
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD m_Open(void)
	{
		if ( ! m_comPort.IsValid() )
		{
			return ERROR_INVALID_PARAMETER;
		}
		// ポートを開く
		HANDLE hComPort = CreatePort(m_comPort.GetPortName());
		// ポートを開く関数が異常を返した場合
		if ( hComPort == INVALID_HANDLE_VALUE )
		{
			return ::GetLastError();
		}
		::PurgeComm(hComPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		// ポートの設定を取得
		DWORD r = m_PortSetting(hComPort);
		if ( r != 0 )
		{
			::CloseHandle(hComPort);
			return r;
		}
		//
		COMSTAT tComStat;
		::ClearCommError(hComPort, &r, &tComStat);
		::SetCommMask(hComPort, 0);
		::PurgeComm(hComPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		m_hComPort = hComPort;
		return 0;
	}

	/**
	 * RS232Cポート設定
	 *	@retval 0	成功。
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD m_PortSetting(HANDLE hComPort)
	{
		DCB	tDCB;
		COMMTIMEOUTS tCommTimeOuts;
		if ( ! ::GetCommState(hComPort, &tDCB) || ! ::GetCommTimeouts(hComPort, &tCommTimeOuts) )
		{
			return _GetLastError("GetCommState/GetCommTimeouts");
		}
		// ポートを設定
		tDCB.BaudRate	= m_dwBaudRate;
		tDCB.ByteSize	= m_bByteSize;
		tDCB.Parity		= static_cast<BYTE>(m_eParity);
		tDCB.StopBits	= static_cast<BYTE>(m_eStopBits);
		if ( m_eParity == EP_NonParity )
		{
			tDCB.fParity = 0;
		}
		else
		{
			tDCB.fParity = 1;
		}	
		//ソフトフロー状態にする。
		tCommTimeOuts.ReadIntervalTimeout			= 0xFFFFFFFF;
		tCommTimeOuts.ReadTotalTimeoutMultiplier	= 0;
		tCommTimeOuts.ReadTotalTimeoutConstant		= 0;
		tCommTimeOuts.WriteTotalTimeoutMultiplier	= 1;
		tCommTimeOuts.WriteTotalTimeoutConstant		= 1000; //1s
		tDCB.fRtsControl		= RTS_CONTROL_DISABLE;
		tDCB.fDtrControl		= DTR_CONTROL_DISABLE;
		tDCB.fOutxCtsFlow		= FALSE;				//ClearToSend監視しない
		tDCB.fOutxDsrFlow		= FALSE;				//DataSetReady監視しない
		tDCB.fDsrSensitivity	= FALSE;				//DSR監視しない
		tDCB.fTXContinueOnXoff	= FALSE;				//XOffCharチェックなし
		tDCB.fInX				= FALSE;				//XON/XOFFフロー制御なし
		tDCB.fOutX				= FALSE;				//XON/XOFFフロー制御なし
		if ( ! OnRs232cOpening(hComPort, tDCB, tCommTimeOuts) )
		{
			return ERROR_NOT_READY; //ユーザ停止
		}
		// ポートの状態を設定
		if ( ! ::SetCommState(hComPort, &tDCB) || ! ::SetCommTimeouts(hComPort, &tCommTimeOuts) )
		{
			return _GetLastError("SetCommState/SetCommTimeouts");
		}
		return 0;
	}
	TProperty		m_property;			///< 性能
	HANDLE			m_hComPort;			///< ハンドル
	CPort			m_comPort;			///< 対象COMポート
	DWORD			m_dwBaudRate;		///< COMポート用設定；レート
	BYTE			m_bByteSize;		///< COMポート用設定；サイズ
	EParity			m_eParity;			///< COMポート用設定；パリティ
	EStopBits		m_eStopBits;		///< COMポート用設定；ストップビット
	mutable bool	m_boIsConnect;		///< CTSによる接続イベント処理用
	bool			m_boCanBulkSend;	///< 送信時、一括で送る？
	bool			m_isCheckCts;		///< CTSチェックを行う？

	#ifndef _TnbRS232C_EnumExistPorts_DISABLE

	/// EnumExistPortDeviceNames() 用検索クラス
	class CMyFinder : private CDeepAccessFinder
	{
		DEFSUPER(CDeepAccessFinder);
	public:
		// 実行
		TDeviceVector Execute(IAccessor::CSection& section)
		{
			TDeviceVector vd;
			m_foundDriver.RemoveAll();
			m_foundPort.RemoveAll();
			_super::Execute(section);
			size_t l = m_foundDriver.GetSize();
			ASSERT( l == m_foundPort.GetSize() );
			if ( l > 0 )
			{
				const int BUF_MAX = 200;
				TCHAR buf[BUF_MAX];
				DWORD len = 0;
				BYTE* B = reinterpret_cast<BYTE*>(buf);
				SP_DEVINFO_DATA dev = { sizeof(SP_DEVINFO_DATA) };
				HDEVINFO hDevInfo = ::SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
				for ( int i = 0; ::SetupDiEnumDeviceInfo(hDevInfo, i, &dev); i++ )
				{
					if ( ::SetupDiGetDeviceRegistryProperty(hDevInfo, &dev, SPDRP_DRIVER, NULL, B, BUF_MAX, &len) )
					{
						INDEX f = m_foundDriver.Find(buf);
						if ( f != INVALID_INDEX )
						{
							TDevice t;
							if ( ::SetupDiGetDeviceRegistryProperty(hDevInfo, &dev, SPDRP_DEVICEDESC, NULL, B, BUF_MAX, &len) )
							{
								t.description = buf;
							}
							if ( ::SetupDiGetDeviceRegistryProperty(hDevInfo, &dev, SPDRP_MFG, NULL, B, BUF_MAX, &len) )
							{
								t.manufacturer = buf;
							}
							if ( ::SetupDiGetDeviceRegistryProperty(hDevInfo, &dev, SPDRP_HARDWAREID, NULL, B, BUF_MAX, &len) )
							{
								CStr s = LowerString(buf);	//USB\Vid_067b&Pid_2303&Rev_0300
								INT_PTR f1 = s.Find(_T("vid_"));
								if ( f1 >= 0 )
								{
									t.vid = down_cast<WORD>(STRLIB::ToInt(s.Mid(f1 + 4, 4), 16));
								}
								INT_PTR f2 = s.Find(_T("pid_"));
								if ( f2 >= 0 )
								{
									t.pid = down_cast<WORD>(STRLIB::ToInt(s.Mid(f2 + 4, 4), 16));
								}
							}
							t.port = m_foundPort[f];
							vd.Add(t);
						}
					}
				}
				VERIFY ( ::SetupDiDestroyDeviceInfoList(hDevInfo) );
			}
			vd.Sort();
			return vd;
		}
	protected:
		virtual EResult OnFindSection(LPCTSTR lpszSection, IAccessor::CSection& sec, int depth)
		{
			if ( STRLIB::Compare(lpszSection, _T("Device Parameters")) == 0 )
			{
				if ( sec.HasKey(_T("PortName")) )
				{
					CStr s = sec.QueryString(_T("PortName"));
					if ( s.Find(_T("COM")) == 0 )
					{
						m_foundPort.Add(static_cast<WORD>(s.ToInt(3)));
						IAccessor::CSection s = sec;
						s.PreviousSubSection();
						m_foundDriver.Add(s.QueryString(_T("Driver")));
					}
					return Previous;
				}
			}
			return Next;
		}
	private:
		CWordVector	m_foundPort;
		CStrVector	m_foundDriver;
	};

	#endif

	friend class CRs232cTest;		///< フレンドクラス宣言
};



/// ハードフロー制御タイプ
/// @date 19/04/27  DSR, DTR は制御対象外にした
class CRs232cHf : public CRs232c
{
protected:
	/**
	 * [通知] オープン通知.
	 *		オープン時、コールされます。
	 *	@param[in] hComPort ハンドル
	 *	@param[in,out] _tDCB 現在の DCB が渡されます。変更することで設定を変更できます。 
	 *	@param[in,out] _tCommTimeOuts 現在の COMMTIMEOUTS が渡されます。変更することで設定を変更できます。
	 *	@retval true オープン処理継続。
	 *	@retval false オープン処理中止（Openが失敗します）。
	 */
	virtual bool OnRs232cOpening(HANDLE hComPort, DCB& _tDCB, COMMTIMEOUTS& _tCommTimeOuts)
	{
		_tCommTimeOuts.WriteTotalTimeoutMultiplier = 1;
		_tCommTimeOuts.WriteTotalTimeoutConstant = 1000;
		//
//		_tDCB.fOutxDsrFlow		= TRUE;
//		_tDCB.fDtrControl		= DTR_CONTROL_HANDSHAKE;
		_tDCB.fOutxCtsFlow		= TRUE;
		_tDCB.fRtsControl		= RTS_CONTROL_HANDSHAKE;
		_tDCB.fDsrSensitivity	= FALSE;
		//
		_tDCB.fTXContinueOnXoff = FALSE;
		_tDCB.fOutX = FALSE;
		_tDCB.fInX = FALSE;
		return true;
	}
};



}; // TNB
