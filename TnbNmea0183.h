#pragma once
/**
 *@file
 * NMEA-0183 関係のヘッダ.
 *
 *		米国海洋電子機器協会（National Marine Electronics Association）が定めた通信フォーマットを扱います。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"
#include "TnbStrVector.h"
#include "TnbReport.h"
#include "TnbQueueingReport.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * NMEA-0183 レポートクラス.
 *
 *		米国海洋電子機器協会（National Marine Electronics Association）が定めた通信フォーマットを扱います。
 *
 *		IReport から読みだしたデータを解析します。
 *
 *	@note GGA(Global Positioning System Fix Data)と GSV(Satellites in View)、 ZDA(Time＆Date)は、本クラスにおいて解析し、
 *		保持し、ゲッターメンバーにより取得することが出来ます。それ以外は、 {@link CNmea0183::IListener リスナー }を使うことで、
 *		容易に情報を取り出すことが出来ます。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		// RS232C の例
 *		CRs232c rs232c;
 *		rs232c.SetParameter(1, 38400, 8, CRs232c::EP_NonParity, CRs232c::ES_OneStopBit);
 *		CCommunicationReporter rep(&rs232c);
 *		CNmea0183 nmea;
 *		nmea.SetEnvironment(&rep);
 *
 *		rep.Start(); //開始		
 *
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbNmea0183.h
 * 
 *	@date 09/01/21	新規作成
 *	@date 09/01/27	排他追加
 *	@date 09/02/05	解析処理修正
 *	@date 09/02/20	GPRMC 対応
 */
class CNmea0183
{
public:

	/**@ingroup REPORT
	 * NMEA-0183 のライン情報.
	 *
	 *		CNmea0183 の通知用情報クラスです。
	 *
	 *	@par必要ファイル
	 *			TnbNmea0183.h
	 * 
	 *	@date 09/01/21	新規作成
	 */
	class CLineInfo
	{
	public:
		/// コンストラクタ
		CLineInfo(void)
		{
		}

		/**
		 * コンストラクタ
		 *	@param vs ライン
		 */
		CLineInfo(const CStrVector& vs) : m_astr(vs) 
		{
		}

		/**
		 * [取得] パラメータ数取得.
		 *	@return 数.
		 */
		size_t GetSize(void) const
		{
			return m_astr.GetSize();
		}

		/**
		 * [取得] 名前取得.
		 *	@note GetString(0) と同じ動作をします。
		 *	@return 名前
		 */
		LPCTSTR GetName(void) const
		{
			return m_astr.IsEmpty() ? _T("") : m_astr[0];
		}

		/**
		 * [取得] 文字型取得.
		 *		指定のインデックスの情報を文字列として取り出します。
		 *	@param index インデックス。GetSize() の戻り値未満の値を指定します。
		 *	@return 文字
		 */
		LPCTSTR GetString(INDEX index) const
		{
			return m_astr.IsInRange(index) ? m_astr[index] : _T("");
		}

		/**
		 * [取得] int型取得.
		 *		指定のインデックスの情報を数値(int)として取り出します。
		 *	@param index インデックス。GetSize() の戻り値未満の値を指定します。
		 *	@return 数値
		 */
		int GetInt(INDEX index) const
		{
			return m_astr.IsInRange(index) ? m_astr[index].ToInt() : 0;
		}

		/**
		 * [取得] double型取得.
		 *		指定のインデックスの情報を数値(double)として取り出します。
		 *	@param index インデックス。GetSize() の戻り値未満の値を指定します。
		 *	@return 数値
		 */
		double GetDouble(INDEX index) const
		{
			return m_astr.IsInRange(index) ? m_astr[index].ToDouble() : 0.0;
		}

		/**
		 * [作成] 一行文字列作成
		 *	@param withCheckSum trueならチェックサムも追加します。
		 *	@return 文字列
		 */
		CStr MakeLineString(bool withCheckSum = true) const
		{
			CStr s;
			loop ( i, m_astr.GetSize() )
			{
				if ( i != 0 )
				{
					s += _T(",");
				}
				s += m_astr[i];
			}
			if ( withCheckSum )
			{
				BYTE sum = 0;
				loop ( i, s.GetLength() )
				{
					sum ^= s[i];
				}
				s += CStr::Fmt(_T("*%02X"), sum);
			}
			return _T("$") + s;
		}
	private:
		CStrVector		m_astr;		///< ライン
	};

	/// Global Positioning System Fix Data
	struct TGpGga
	{
		WORD	hour;				///< 測位時刻(UTC) 時間
		WORD	minute;				///< 測位時刻(UTC) 分
		WORD	second;				///< 測位時刻(UTC) 秒
		WORD	ms;					///< 測位時刻(UTC) ミリ秒
		bool	isNorth;			///< 北緯 南緯
		double	latitude;			///< 緯度
		bool	isEast;				///< 東経 西経
		double	longitude;			///< 経度
		WORD	gpsQuality;			///< GPSクオリティ 0=受信不能 1=単独測位 2=DGPS
		WORD	satelliteNumber;	///< 受信衛星数
		double	hdop;				///< HDOP
		double	height;				///< 平均海水面からのアンテナ高度(m)
		double	altitudeDifference;	///< 平均高度差
		double	dgpsData;			///< DGPSデータのエイジ(秒)
		WORD	dgpsId;				///< DGPS基準局のID
	};

	/// Satellites in View
	struct TGpGsv
	{
		WORD	satelliteNumber;	///< 衛星番号
		WORD	angleOfElevation;	///< 仰角
		WORD	direction;			///< 方位
		WORD	snr;				///< SNR(デシベル)
	};

	/// Recommended Minimum Specific GNSS Data
	struct TGpRmc
	{
		bool		isValid;		///< 有効無効
		SYSTEMTIME	utc;			///< 測位時刻(UTC)
		bool		isNorth;		///< 北緯 南緯
		double		latitude;		///< 緯度
		bool		isEast;			///< 東経 西経
		double		longitude;		///< 経度
		double		speed;			///< 対地速度(ノット)
		double		degree;			///< 進行方向
	};

	/// 結果
	enum EErrorCode
	{
		EC_UnknownDatas = 1,		///< 不明なデータ(SkipData)。lParamは長さ。
		EC_CheckSumError,			///< チェックサムエラー。HIWORD(lParam)は受信SUM、LOWORD(lParam) は計算SUM。
		EC_Connect,					///< 接続。lParam は０。
		EC_Disconnect,				///< 切断。lParam は０。
		EC_End,						///< 終了。クローズされた。lParam は０。
	};

	/**@ingroup REPORT
	 * NMEA-0183 のリスナーインターフェース.
	 *
	 *		CNmea0183 の通知用インターフェースです。
	 *
	 *		OnNmeaReceivedLine() にはチェックサムが正しいものだけが通知されます。
	 *
	 *	@par必要ファイル
	 *			TnbNmea0183.h
	 * 
	 *	@date 09/01/21	新規作成
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}
		/**
		 * [通知] 正常ライン取得通知.
		 *	@note チェックサムが正常なものだけ通知されます。
		 *	@param li ライン.
		 */
		virtual void OnNmeaReceivedLine(const CLineInfo& li) = 0;
		/**
		 * [通知] エラー通知.
		 *	@param e エラー内容.
		 *	@param lParam パラメータ。
		 */
		virtual void OnNmeaError(EErrorCode e, LPARAM lParam) = 0;
	};


	//----------------------

	
	/// コンストラクタ
	CNmea0183(void)
	{
	}

	/**
	 * [設定] 環境設定.
	 *	@param pReport レポート。
	 *	@param pListener リスナー。省略可能。
	 */
	void SetEnvironment(IReport* pReport, IListener* pListener = NULL)
	{
		m_queue.SetEnvironment(pReport, &m_inner);
		m_inner.m_pListener = pListener;
	}

	/**
	 * [取得] ZDAデータ取得.
	 *	@param[out] _tm ZDAデータ(ローカルタイム)。ここに情報が格納されます。
	 *	@param[in] tick 受信してからの時間(ms)
	 *	@retval false tick時間内にデータは受信されていない
	 *	@retval true tick時間内にデータあり。
	 */
	bool GetZdaData(SYSTEMTIME& _tm, DWORD tick = 2000) const
	{
		if ( ExistGps() )
		{
			EXCLUSIVE(&m_inner.m_dataSync);
			FILETIME ft1;
			FILETIME ft2;
			if ( ::SystemTimeToFileTime(&m_inner.m_gpZda, &ft1) && ::FileTimeToLocalFileTime(&ft1, &ft2) )
			{
				::FileTimeToSystemTime(&ft2, &_tm);
				DWORD r = ::GetTickCount() - m_inner.m_tickZda;
				return (r < tick);
			}
		}
		return false;
	}

	/**
	 * [取得] GGAデータ取得.
	 *	@param[out] _gga GGAデータ。ここに情報が格納されます。
	 *	@param[in] tick 受信してからの時間(ms)
	 *	@retval false tick時間内にデータは受信されていない
	 *	@retval true tick時間内にデータあり。
	 */
	bool GetGgaData(TGpGga& _gga, DWORD tick = 2000) const
	{
		if ( ExistGps() )
		{
			EXCLUSIVE(&m_inner.m_dataSync);
			_gga = m_inner.m_gpGga;
			DWORD r = ::GetTickCount() - m_inner.m_tickGga;
			return (r < tick);
		}
		return false;
	}

	/**
	 * [取得] GSVデータ取得.
	 *	@param[out] _gsvs GSVデータ配列。ここに情報が格納されます。
	 *	@param[in] tick 受信してからの時間(ms)
	 *	@retval false tick時間内にデータは受信されていない
	 *	@retval true tick時間内にデータあり。
	 */
	bool GetGsvData(CVectorT<TGpGsv>& _gsvs, DWORD tick = 2000) const
	{
		if ( ExistGps() )
		{
			EXCLUSIVE(&m_inner.m_dataSync);
			_gsvs = m_inner.m_gpGsvs;
			DWORD r = ::GetTickCount() - m_inner.m_tickGsv;
			#ifdef _DEBUG
				if ( r < tick )
				{
					loop ( i, _gsvs.GetSize() )
					{
						TRACE2("Nmea;GSV[%d] 衛星No=%d, ", i + 1, _gsvs[i].satelliteNumber);
						TRACE3("仰角=%d, 方位=%d, SNR=%d\n", _gsvs[i].angleOfElevation, _gsvs[i].direction, _gsvs[i].snr);
					}
				}
			#endif
			return (r < tick);
		}
		return false;
	}

	/**
	 * [取得] RMCデータ取得.
	 *	@param[out] _rmc RMCデータ。ここに情報が格納されます。
	 *	@param[in] tick 受信してからの時間(ms)
	 *	@retval false tick時間内にデータは受信されていない
	 *	@retval true tick時間内にデータあり。
	 */
	bool GetRmcData(TGpRmc& _rmc, DWORD tick = 2000) const
	{
		if ( ExistGps() )
		{
			EXCLUSIVE(&m_inner.m_dataSync);
			_rmc = m_inner.m_gpRmc;
			DWORD r = ::GetTickCount() - m_inner.m_tickRmc;
			return (r < tick);
		}
		return false;
	}

	/**
	 * [確認] GPS確認.
	 *	@retval true GPS接続有り。
	 *	@retval false GPSなし。開始していない。　
	 */
	virtual bool ExistGps(void) const
	{
		if ( ! m_queue.IsAlive() )
		{
			//開始していない
			return false;
		}
		return m_queue.IsConnect();
	}

private:
	/// インナークラス
	struct TInner : CQueueingReport::IListener
	{
		DEFPARENTLISTENER(CNmea0183, IParentListener);
		//-
		IParentListener*	m_pListener;	///< リスナー
		//- 解析用
		bool				m_isAnalying;	///< 解析中
		BYTE				m_checkSum;		///< 解析中のチェックサム値
		size_t				m_checkSize;	///< 解析中の文字数
		CStrVector			m_queStr;		///< 解析中のライン。
		//- 受信情報
		CSyncSection		m_dataSync;		///< データ排他
		SYSTEMTIME			m_gpZda;		///< GPZDAデータ
		DWORD				m_tickZda;		///< GPZDAデータ取得時間
		TGpGga				m_gpGga;		///< GPGGAデータ
		DWORD				m_tickGga;		///< GPGGAデータ取得時間
		CVectorT<TGpGsv>	m_gpGsvs;		///< GPGSVデータ
		CVectorT<TGpGsv>	m_gpGsvsTmp;		///< GPGSVデータ
		DWORD				m_tickGsv;		///< GPGSVデータ取得時間
		TGpRmc				m_gpRmc;		///< GPRMCデータ
		DWORD				m_tickRmc;		///< GPRMCデータ取得時間
		/// コンストラクタ
		TInner(void) : m_isAnalying(false), m_pListener(NULL)
		{
			DWORD w = ::GetTickCount();
			w += 0x80000000;
			m_tickZda = w;
			m_tickGga = w;
			m_tickGsv = w;
		}
		/// 一行受信をリスナーに通知
		void NotifyNmeaReceivedLine(const CLineInfo& li)
		{
			if ( STRLIB::Compare(li.GetName(), _T("GPZDA")) == 0 )
			{
				//== Time & Date
				SYSTEMTIME sm;
				sm.wYear = static_cast<WORD>(li.GetInt(4));
				sm.wMonth = static_cast<WORD>(li.GetInt(2));
				sm.wDay = static_cast<WORD>(li.GetInt(3));
				sm.wDayOfWeek = 0;
				double tm = li.GetDouble(1);
				sm.wHour = static_cast<WORD>(int(tm / 10000) % 100);
				sm.wMinute = static_cast<WORD>(int(tm / 100) % 100);
				sm.wSecond = static_cast<WORD>(int(tm) % 100);
				sm.wMilliseconds = static_cast<WORD>(int(tm * 1000) % 1000);
				EXCLUSIVE(&m_dataSync);
				m_tickZda = ::GetTickCount();
				m_gpZda = sm;
			}
			else if ( STRLIB::Compare(li.GetName(), _T("GPRMC")) == 0 )
			{
				//== GNSS Data
				TGpRmc g;
				// 測位時刻(UTC)
				double tm = li.GetDouble(1);
				g.utc.wHour = static_cast<WORD>(int(tm / 10000) % 100);
				g.utc.wMinute = static_cast<WORD>(int(tm / 100) % 100);
				g.utc.wSecond = static_cast<WORD>(int(tm) % 100);
				g.utc.wMilliseconds = static_cast<WORD>(int(tm * 1000) % 1000);
				// 有効無効
				g.isValid = (li.GetString(2)[0] == 'A');
				// 緯度（北緯 南緯）
				g.latitude = li.GetDouble(3);
				g.isNorth = (li.GetString(4)[0] == 'N');
				// 経度 (東経 西経)
				g.longitude = li.GetDouble(5);
				g.isEast = (li.GetString(6)[0] == 'E');
				// 対地速度(ノット)
				g.speed = li.GetDouble(7);
				// 進行方向
				g.degree = li.GetDouble(8);
				//
				int sm = li.GetInt(9);
				int y = sm % 100;
				g.utc.wYear = static_cast<WORD>(y + ((y < 50) ? 2000 : 1900));
				g.utc.wMonth = static_cast<WORD>((sm / 100) % 100);
				g.utc.wDay = static_cast<WORD>((sm / 10000) % 100);
				g.utc.wDayOfWeek = 0;
				//
				EXCLUSIVE(&m_dataSync);
				m_tickRmc = ::GetTickCount();
				m_gpRmc = g;
			}
			else if ( STRLIB::Compare(li.GetName(), _T("GPGGA")) == 0 )
			{
				//== Global Positioning System Fix Data
				TGpGga g;
				// 測位時刻(UTC)
				double tm = li.GetDouble(1);
				g.hour = static_cast<WORD>(int(tm / 10000) % 100);
				g.minute = static_cast<WORD>(int(tm / 100) % 100);
				g.second = static_cast<WORD>(int(tm) % 100);
				g.ms = static_cast<WORD>(int(tm * 1000) % 1000);
				// 緯度（北緯 南緯）
				g.latitude = li.GetDouble(2);
				g.isNorth = (li.GetString(3)[0] == 'N');
				// 経度 (東経 西経)
				g.longitude = li.GetDouble(4);
				g.isEast = (li.GetString(5)[0] == 'E');
				//
				g.gpsQuality = static_cast<WORD>(li.GetInt(6));			// GPSクオリティ 0=受信不能 1=単独測位 2=DGPS
				g.satelliteNumber = static_cast<WORD>(li.GetInt(7));		// 受信衛星数
				g.hdop = li.GetDouble(8);				// HDOP
				g.height = li.GetDouble(9);				// 平均海水面からのアンテナ高度(m)
				if ( li.GetString(10)[0] == 'f' )
				{
					//feetだったのでmに変換
					g.height *= 0.3048;
				}
				g.altitudeDifference = li.GetDouble(11);// 平均高度差(m)
				if ( li.GetString(12)[0] == 'f' )
				{
					//feetだったのでmに変換
					g.altitudeDifference *= 0.3048;
				}
				g.dgpsData = li.GetDouble(13);			// DGPSデータのエイジ(秒)
				g.dgpsId = static_cast<WORD>(li.GetInt(14));				// DGPS基準局のID
				//
				EXCLUSIVE(&m_dataSync);
				m_tickGga = ::GetTickCount();
				m_gpGga = g;
			}
			else if ( STRLIB::Compare(li.GetName(), _T("GPGSV")) == 0 )
			{
				//== Satellites in View
				int all = li.GetInt(1); // 全メッセージ数
				int no = li.GetInt(2); // メッセージ番号
//				int sate = li.GetInt(3); //衛星数
				EXCLUSIVE(&m_dataSync);
				if ( no == 1 )
				{
					m_gpGsvsTmp.RemoveAll();
				}
				loop ( i, 4 )
				{
					INDEX j = 4 + i * 4;
					if ( li.GetSize() <= j + 3 )
					{
						break;
					}
					TGpGsv g;
					g.satelliteNumber = static_cast<WORD>(li.GetInt(j));
					g.angleOfElevation = static_cast<WORD>(li.GetInt(j + 1));
					g.direction = static_cast<WORD>(li.GetInt(j + 2));
					g.snr = static_cast<WORD>(li.GetInt(j + 3));
					m_gpGsvsTmp.Add(g);
				}
				if ( all == no )
				{
					EXCLUSIVE(&m_dataSync);
//					ASSERT( sate == m_gpGsvs.GetSize() ); //いっちするはず
					m_tickGsv = ::GetTickCount();
					m_gpGsvs = m_gpGsvsTmp;
				}
			}
			if ( m_pListener != NULL )
			{
				m_pListener->OnNmeaReceivedLine(li);
			}
		}
		/// エラーをリスナーに通知
		void NotifyNmeaError(EErrorCode e, LPARAM lParam)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnNmeaError(e, lParam);
			}
		}
		/**
		 * [通知] イベント発生
		 *	@param ev 受信した内容（イベントのみ）。
		 *	@retval true	正常
		 *	@retval false	切断要求
		 */
		virtual bool OnReportEvent(const CReportEvent& ev)
		{
			ASSERT ( ev.HasEvent() );
			switch ( ev.GetEvent() )
			{
			case ReportEvent_Disconnect:
				NotifyNmeaError(EC_Disconnect, 0);
				break;
			case ReportEvent_Connect:
				NotifyNmeaError(EC_Connect, 0);
				break;
			case ReportEvent_End:
				NotifyNmeaError(EC_End, 0);
				break;
			}
			return true;
		}
		/**
		 * [変換] データ受信
		 *	@param[in] boIsLast true なら最後のデータ。
		 *	@param[in] c 変換元RAWデータ(受信データ)。
		 *	@return 処理したデータ数
		 */
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c)
		{
			size_t len = c.GetSize();
			if ( ! m_isAnalying )
			{
				//=== ヘッダ検索中
				if ( len > 0 )
				{
					BYTE b = c.At(0);
					if ( b == '\n' || b == '\r' || b == 0 )
					{
						return 1;
					}
					if ( b != '$' )
					{
						// ヘッダ文字'$'じゃない
						for ( size_t i = 1; i < len; i++ )
						{
							if ( c.At(i) == '$' )
							{
								TRACE1("Nmea;skip %d\n", i);
								NotifyNmeaError(EC_UnknownDatas, i);
								return i;
							}
						}
						TRACE1("Nmea;skip %d\n", len);
						NotifyNmeaError(EC_UnknownDatas, len);
						return len;
					}
				}
				if ( len < 1 + 1 + 3 )
				{
					return 0; // 絶対足りない
				}
				m_isAnalying = true;
				m_checkSum = 0;
				m_checkSize = 1;
				m_queStr.RemoveAll();
				TRACE0("Nmea;found header'$'\n");
				return 1; // '$' をスキップ
			}
			//=== ライン解析中
			loop ( i, len )
			{
				BYTE b = c.At(i);
				if ( b == '\n' || b == '$' || b == 0 )
				{
					// 異常
					TRACE0("Nmea;found known char\n");
					m_isAnalying = false;
					NotifyNmeaError(EC_UnknownDatas, i + 1);
					return i + 1;
				}
				else if ( b == '*' && (len - i) < 3 )
				{
					return 0; //足りない
				}
				else if ( b == ',' || b == '*' )
				{
					// , か * の場合
					CStr s;
					loop ( j, i )
					{
						s += c.At(j);
						m_checkSum ^= c.At(j);
						m_checkSize++;
					}
					TRACE2("(%d) %s\n", m_queStr.GetSize(), s);
					m_queStr.Add(s);
					if ( b == ',' )
					{
						// , の場合
						m_checkSum ^= b;
						m_checkSize++;
						return i + 1;
					}
					// * の場合
					if ( len >= 3 )
					{
						int h = STRLIB::HexCharToInt(c.At(i + 1));
						int l = STRLIB::HexCharToInt(c.At(i + 2));
						if ( h < 0 || l < 0 )
						{
							TRACE1("Nmea;invalid checksum char skip %d\n", m_checkSize + 1);
							NotifyNmeaError(EC_UnknownDatas, m_checkSize + 1);
							m_isAnalying = false;
							return i + 1;
						}
						BYTE sum = static_cast<BYTE>(h << 4 | l);
						if ( m_checkSum == sum )
						{
							//発見！
							TRACE0("Nmea;found valid line!!\n");
							CLineInfo li(m_queStr);
							NotifyNmeaReceivedLine(li);
						}
						else
						{
							TRACE1("Nmea;checksum error!! skip %d\n", m_checkSize + 3);
							if ( m_pListener != NULL )
							{
								m_pListener->OnNmeaError(EC_CheckSumError, MAKELONG(m_checkSum, sum));
							}
						}
						m_isAnalying = false;
						return i + 3;
					}
					return i;
				}
			}
			return 0;
		}
	};

	//- 読込み用
	CQueueingReport	m_queue;		///< 受信キュー
	TInner			m_inner;		///< インナー
	friend class CNmea0183Test;
};



}; // TNB

