#pragma once
/**
 *@file
 * NMEA-0183 �֌W�̃w�b�_.
 *
 *		�č��C�m�d�q�@�틦��iNational Marine Electronics Association�j����߂��ʐM�t�H�[�}�b�g�������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"
#include "TnbStrVector.h"
#include "TnbReport.h"
#include "TnbQueueingReport.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * NMEA-0183 ���|�[�g�N���X.
 *
 *		�č��C�m�d�q�@�틦��iNational Marine Electronics Association�j����߂��ʐM�t�H�[�}�b�g�������܂��B
 *
 *		IReport ����ǂ݂������f�[�^����͂��܂��B
 *
 *	@note GGA(Global Positioning System Fix Data)�� GSV(Satellites in View)�A ZDA(Time��Date)�́A�{�N���X�ɂ����ĉ�͂��A
 *		�ێ����A�Q�b�^�[�����o�[�ɂ��擾���邱�Ƃ��o���܂��B����ȊO�́A {@link CNmea0183::IListener ���X�i�[ }���g�����ƂŁA
 *		�e�Ղɏ������o�����Ƃ��o���܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		// RS232C �̗�
 *		CRs232c rs232c;
 *		rs232c.SetParameter(1, 38400, 8, CRs232c::EP_NonParity, CRs232c::ES_OneStopBit);
 *		CCommunicationReporter rep(&rs232c);
 *		CNmea0183 nmea;
 *		nmea.SetEnvironment(&rep);
 *
 *		rep.Start(); //�J�n		
 *
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbNmea0183.h
 * 
 *	@date 09/01/21	�V�K�쐬
 *	@date 09/01/27	�r���ǉ�
 *	@date 09/02/05	��͏����C��
 *	@date 09/02/20	GPRMC �Ή�
 */
class CNmea0183
{
public:

	/**@ingroup REPORT
	 * NMEA-0183 �̃��C�����.
	 *
	 *		CNmea0183 �̒ʒm�p���N���X�ł��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbNmea0183.h
	 * 
	 *	@date 09/01/21	�V�K�쐬
	 */
	class CLineInfo
	{
	public:
		/// �R���X�g���N�^
		CLineInfo(void)
		{
		}

		/**
		 * �R���X�g���N�^
		 *	@param vs ���C��
		 */
		CLineInfo(const CStrVector& vs) : m_astr(vs) 
		{
		}

		/**
		 * [�擾] �p�����[�^���擾.
		 *	@return ��.
		 */
		size_t GetSize(void) const
		{
			return m_astr.GetSize();
		}

		/**
		 * [�擾] ���O�擾.
		 *	@note GetString(0) �Ɠ�����������܂��B
		 *	@return ���O
		 */
		LPCTSTR GetName(void) const
		{
			return m_astr.IsEmpty() ? _T("") : m_astr[0];
		}

		/**
		 * [�擾] �����^�擾.
		 *		�w��̃C���f�b�N�X�̏��𕶎���Ƃ��Ď��o���܂��B
		 *	@param index �C���f�b�N�X�BGetSize() �̖߂�l�����̒l���w�肵�܂��B
		 *	@return ����
		 */
		LPCTSTR GetString(INDEX index) const
		{
			return m_astr.IsInRange(index) ? m_astr[index] : _T("");
		}

		/**
		 * [�擾] int�^�擾.
		 *		�w��̃C���f�b�N�X�̏��𐔒l(int)�Ƃ��Ď��o���܂��B
		 *	@param index �C���f�b�N�X�BGetSize() �̖߂�l�����̒l���w�肵�܂��B
		 *	@return ���l
		 */
		int GetInt(INDEX index) const
		{
			return m_astr.IsInRange(index) ? m_astr[index].ToInt() : 0;
		}

		/**
		 * [�擾] double�^�擾.
		 *		�w��̃C���f�b�N�X�̏��𐔒l(double)�Ƃ��Ď��o���܂��B
		 *	@param index �C���f�b�N�X�BGetSize() �̖߂�l�����̒l���w�肵�܂��B
		 *	@return ���l
		 */
		double GetDouble(INDEX index) const
		{
			return m_astr.IsInRange(index) ? m_astr[index].ToDouble() : 0.0;
		}

		/**
		 * [�쐬] ��s������쐬
		 *	@param withCheckSum true�Ȃ�`�F�b�N�T�����ǉ����܂��B
		 *	@return ������
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
		CStrVector		m_astr;		///< ���C��
	};

	/// Global Positioning System Fix Data
	struct TGpGga
	{
		WORD	hour;				///< ���ʎ���(UTC) ����
		WORD	minute;				///< ���ʎ���(UTC) ��
		WORD	second;				///< ���ʎ���(UTC) �b
		WORD	ms;					///< ���ʎ���(UTC) �~���b
		bool	isNorth;			///< �k�� ���
		double	latitude;			///< �ܓx
		bool	isEast;				///< ���o ���o
		double	longitude;			///< �o�x
		WORD	gpsQuality;			///< GPS�N�I���e�B 0=��M�s�\ 1=�P�Ƒ��� 2=DGPS
		WORD	satelliteNumber;	///< ��M�q����
		double	hdop;				///< HDOP
		double	height;				///< ���ϊC���ʂ���̃A���e�i���x(m)
		double	altitudeDifference;	///< ���ύ��x��
		double	dgpsData;			///< DGPS�f�[�^�̃G�C�W(�b)
		WORD	dgpsId;				///< DGPS��ǂ�ID
	};

	/// Satellites in View
	struct TGpGsv
	{
		WORD	satelliteNumber;	///< �q���ԍ�
		WORD	angleOfElevation;	///< �p
		WORD	direction;			///< ����
		WORD	snr;				///< SNR(�f�V�x��)
	};

	/// Recommended Minimum Specific GNSS Data
	struct TGpRmc
	{
		bool		isValid;		///< �L������
		SYSTEMTIME	utc;			///< ���ʎ���(UTC)
		bool		isNorth;		///< �k�� ���
		double		latitude;		///< �ܓx
		bool		isEast;			///< ���o ���o
		double		longitude;		///< �o�x
		double		speed;			///< �Βn���x(�m�b�g)
		double		degree;			///< �i�s����
	};

	/// ����
	enum EErrorCode
	{
		EC_UnknownDatas = 1,		///< �s���ȃf�[�^(SkipData)�BlParam�͒����B
		EC_CheckSumError,			///< �`�F�b�N�T���G���[�BHIWORD(lParam)�͎�MSUM�ALOWORD(lParam) �͌v�ZSUM�B
		EC_Connect,					///< �ڑ��BlParam �͂O�B
		EC_Disconnect,				///< �ؒf�BlParam �͂O�B
		EC_End,						///< �I���B�N���[�Y���ꂽ�BlParam �͂O�B
	};

	/**@ingroup REPORT
	 * NMEA-0183 �̃��X�i�[�C���^�[�t�F�[�X.
	 *
	 *		CNmea0183 �̒ʒm�p�C���^�[�t�F�[�X�ł��B
	 *
	 *		OnNmeaReceivedLine() �ɂ̓`�F�b�N�T�������������̂������ʒm����܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbNmea0183.h
	 * 
	 *	@date 09/01/21	�V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}
		/**
		 * [�ʒm] ���탉�C���擾�ʒm.
		 *	@note �`�F�b�N�T��������Ȃ��̂����ʒm����܂��B
		 *	@param li ���C��.
		 */
		virtual void OnNmeaReceivedLine(const CLineInfo& li) = 0;
		/**
		 * [�ʒm] �G���[�ʒm.
		 *	@param e �G���[���e.
		 *	@param lParam �p�����[�^�B
		 */
		virtual void OnNmeaError(EErrorCode e, LPARAM lParam) = 0;
	};


	//----------------------

	
	/// �R���X�g���N�^
	CNmea0183(void)
	{
	}

	/**
	 * [�ݒ�] ���ݒ�.
	 *	@param pReport ���|�[�g�B
	 *	@param pListener ���X�i�[�B�ȗ��\�B
	 */
	void SetEnvironment(IReport* pReport, IListener* pListener = NULL)
	{
		m_queue.SetEnvironment(pReport, &m_inner);
		m_inner.m_pListener = pListener;
	}

	/**
	 * [�擾] ZDA�f�[�^�擾.
	 *	@param[out] _tm ZDA�f�[�^(���[�J���^�C��)�B�����ɏ�񂪊i�[����܂��B
	 *	@param[in] tick ��M���Ă���̎���(ms)
	 *	@retval false tick���ԓ��Ƀf�[�^�͎�M����Ă��Ȃ�
	 *	@retval true tick���ԓ��Ƀf�[�^����B
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
	 * [�擾] GGA�f�[�^�擾.
	 *	@param[out] _gga GGA�f�[�^�B�����ɏ�񂪊i�[����܂��B
	 *	@param[in] tick ��M���Ă���̎���(ms)
	 *	@retval false tick���ԓ��Ƀf�[�^�͎�M����Ă��Ȃ�
	 *	@retval true tick���ԓ��Ƀf�[�^����B
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
	 * [�擾] GSV�f�[�^�擾.
	 *	@param[out] _gsvs GSV�f�[�^�z��B�����ɏ�񂪊i�[����܂��B
	 *	@param[in] tick ��M���Ă���̎���(ms)
	 *	@retval false tick���ԓ��Ƀf�[�^�͎�M����Ă��Ȃ�
	 *	@retval true tick���ԓ��Ƀf�[�^����B
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
						TRACE2("Nmea;GSV[%d] �q��No=%d, ", i + 1, _gsvs[i].satelliteNumber);
						TRACE3("�p=%d, ����=%d, SNR=%d\n", _gsvs[i].angleOfElevation, _gsvs[i].direction, _gsvs[i].snr);
					}
				}
			#endif
			return (r < tick);
		}
		return false;
	}

	/**
	 * [�擾] RMC�f�[�^�擾.
	 *	@param[out] _rmc RMC�f�[�^�B�����ɏ�񂪊i�[����܂��B
	 *	@param[in] tick ��M���Ă���̎���(ms)
	 *	@retval false tick���ԓ��Ƀf�[�^�͎�M����Ă��Ȃ�
	 *	@retval true tick���ԓ��Ƀf�[�^����B
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
	 * [�m�F] GPS�m�F.
	 *	@retval true GPS�ڑ��L��B
	 *	@retval false GPS�Ȃ��B�J�n���Ă��Ȃ��B�@
	 */
	virtual bool ExistGps(void) const
	{
		if ( ! m_queue.IsAlive() )
		{
			//�J�n���Ă��Ȃ�
			return false;
		}
		return m_queue.IsConnect();
	}

private:
	/// �C���i�[�N���X
	struct TInner : CQueueingReport::IListener
	{
		DEFPARENTLISTENER(CNmea0183, IParentListener);
		//-
		IParentListener*	m_pListener;	///< ���X�i�[
		//- ��͗p
		bool				m_isAnalying;	///< ��͒�
		BYTE				m_checkSum;		///< ��͒��̃`�F�b�N�T���l
		size_t				m_checkSize;	///< ��͒��̕�����
		CStrVector			m_queStr;		///< ��͒��̃��C���B
		//- ��M���
		CSyncSection		m_dataSync;		///< �f�[�^�r��
		SYSTEMTIME			m_gpZda;		///< GPZDA�f�[�^
		DWORD				m_tickZda;		///< GPZDA�f�[�^�擾����
		TGpGga				m_gpGga;		///< GPGGA�f�[�^
		DWORD				m_tickGga;		///< GPGGA�f�[�^�擾����
		CVectorT<TGpGsv>	m_gpGsvs;		///< GPGSV�f�[�^
		CVectorT<TGpGsv>	m_gpGsvsTmp;		///< GPGSV�f�[�^
		DWORD				m_tickGsv;		///< GPGSV�f�[�^�擾����
		TGpRmc				m_gpRmc;		///< GPRMC�f�[�^
		DWORD				m_tickRmc;		///< GPRMC�f�[�^�擾����
		/// �R���X�g���N�^
		TInner(void) : m_isAnalying(false), m_pListener(NULL)
		{
			DWORD w = ::GetTickCount();
			w += 0x80000000;
			m_tickZda = w;
			m_tickGga = w;
			m_tickGsv = w;
		}
		/// ��s��M�����X�i�[�ɒʒm
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
				// ���ʎ���(UTC)
				double tm = li.GetDouble(1);
				g.utc.wHour = static_cast<WORD>(int(tm / 10000) % 100);
				g.utc.wMinute = static_cast<WORD>(int(tm / 100) % 100);
				g.utc.wSecond = static_cast<WORD>(int(tm) % 100);
				g.utc.wMilliseconds = static_cast<WORD>(int(tm * 1000) % 1000);
				// �L������
				g.isValid = (li.GetString(2)[0] == 'A');
				// �ܓx�i�k�� ��܁j
				g.latitude = li.GetDouble(3);
				g.isNorth = (li.GetString(4)[0] == 'N');
				// �o�x (���o ���o)
				g.longitude = li.GetDouble(5);
				g.isEast = (li.GetString(6)[0] == 'E');
				// �Βn���x(�m�b�g)
				g.speed = li.GetDouble(7);
				// �i�s����
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
				// ���ʎ���(UTC)
				double tm = li.GetDouble(1);
				g.hour = static_cast<WORD>(int(tm / 10000) % 100);
				g.minute = static_cast<WORD>(int(tm / 100) % 100);
				g.second = static_cast<WORD>(int(tm) % 100);
				g.ms = static_cast<WORD>(int(tm * 1000) % 1000);
				// �ܓx�i�k�� ��܁j
				g.latitude = li.GetDouble(2);
				g.isNorth = (li.GetString(3)[0] == 'N');
				// �o�x (���o ���o)
				g.longitude = li.GetDouble(4);
				g.isEast = (li.GetString(5)[0] == 'E');
				//
				g.gpsQuality = static_cast<WORD>(li.GetInt(6));			// GPS�N�I���e�B 0=��M�s�\ 1=�P�Ƒ��� 2=DGPS
				g.satelliteNumber = static_cast<WORD>(li.GetInt(7));		// ��M�q����
				g.hdop = li.GetDouble(8);				// HDOP
				g.height = li.GetDouble(9);				// ���ϊC���ʂ���̃A���e�i���x(m)
				if ( li.GetString(10)[0] == 'f' )
				{
					//feet�������̂�m�ɕϊ�
					g.height *= 0.3048;
				}
				g.altitudeDifference = li.GetDouble(11);// ���ύ��x��(m)
				if ( li.GetString(12)[0] == 'f' )
				{
					//feet�������̂�m�ɕϊ�
					g.altitudeDifference *= 0.3048;
				}
				g.dgpsData = li.GetDouble(13);			// DGPS�f�[�^�̃G�C�W(�b)
				g.dgpsId = static_cast<WORD>(li.GetInt(14));				// DGPS��ǂ�ID
				//
				EXCLUSIVE(&m_dataSync);
				m_tickGga = ::GetTickCount();
				m_gpGga = g;
			}
			else if ( STRLIB::Compare(li.GetName(), _T("GPGSV")) == 0 )
			{
				//== Satellites in View
				int all = li.GetInt(1); // �S���b�Z�[�W��
				int no = li.GetInt(2); // ���b�Z�[�W�ԍ�
//				int sate = li.GetInt(3); //�q����
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
//					ASSERT( sate == m_gpGsvs.GetSize() ); //����������͂�
					m_tickGsv = ::GetTickCount();
					m_gpGsvs = m_gpGsvsTmp;
				}
			}
			if ( m_pListener != NULL )
			{
				m_pListener->OnNmeaReceivedLine(li);
			}
		}
		/// �G���[�����X�i�[�ɒʒm
		void NotifyNmeaError(EErrorCode e, LPARAM lParam)
		{
			if ( m_pListener != NULL )
			{
				m_pListener->OnNmeaError(e, lParam);
			}
		}
		/**
		 * [�ʒm] �C�x���g����
		 *	@param ev ��M�������e�i�C�x���g�̂݁j�B
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
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
		 * [�ϊ�] �f�[�^��M
		 *	@param[in] boIsLast true �Ȃ�Ō�̃f�[�^�B
		 *	@param[in] c �ϊ���RAW�f�[�^(��M�f�[�^)�B
		 *	@return ���������f�[�^��
		 */
		virtual size_t OnReportData(bool boIsLast, const IConstCollectionT<BYTE>& c)
		{
			size_t len = c.GetSize();
			if ( ! m_isAnalying )
			{
				//=== �w�b�_������
				if ( len > 0 )
				{
					BYTE b = c.At(0);
					if ( b == '\n' || b == '\r' || b == 0 )
					{
						return 1;
					}
					if ( b != '$' )
					{
						// �w�b�_����'$'����Ȃ�
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
					return 0; // ��Α���Ȃ�
				}
				m_isAnalying = true;
				m_checkSum = 0;
				m_checkSize = 1;
				m_queStr.RemoveAll();
				TRACE0("Nmea;found header'$'\n");
				return 1; // '$' ���X�L�b�v
			}
			//=== ���C����͒�
			loop ( i, len )
			{
				BYTE b = c.At(i);
				if ( b == '\n' || b == '$' || b == 0 )
				{
					// �ُ�
					TRACE0("Nmea;found known char\n");
					m_isAnalying = false;
					NotifyNmeaError(EC_UnknownDatas, i + 1);
					return i + 1;
				}
				else if ( b == '*' && (len - i) < 3 )
				{
					return 0; //����Ȃ�
				}
				else if ( b == ',' || b == '*' )
				{
					// , �� * �̏ꍇ
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
						// , �̏ꍇ
						m_checkSum ^= b;
						m_checkSize++;
						return i + 1;
					}
					// * �̏ꍇ
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
							//�����I
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

	//- �Ǎ��ݗp
	CQueueingReport	m_queue;		///< ��M�L���[
	TInner			m_inner;		///< �C���i�[
	friend class CNmea0183Test;
};



}; // TNB

