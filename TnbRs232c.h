#pragma once
/**
 *@file
 * RS232C�̃A�N�Z�X�֌W�̃w�b�_
 *
 *		�V���A���̏ڍאݒ�́A OnRs232cOpening() ���\�b�h���I�[�o�[���C�h���đΉ��ł��܂��B
 *
 *	@note	�}�N�� "_TnbRS232C_EnumExistPorts_DISABLE" ����`����Ă���ƁA CRs232c::EnumExistPorts() , CRs232c::EnumExistPortDeviceNames(),
 *			CRs232c::EnumExistPortsByUsb() ���g�p�֎~�ɂȂ�܂��B���̑���A TnbRegistryAccessor.h �͕s�K�v�ɂȂ�܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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
 * RS232C�A�N�Z�X�N���X
 *
 *		�V���A���A�N�Z�X���s���N���X�ł��B
 *
 *	@note �ʐM�ݒ�́A SetParameter() ���g���Ďw�肵�܂��B
 *		 ���ׂ����ݒ�́A�{�N���X���p�����A OnRs232cOpening() ���������܂��B
 *		��M�o�b�t�@�T�C�Y�̕ύX�� Win32API �� ::SetupComm() ���g�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbRs232c.h
 * 
 *	@date 06/01/01	�V�K�쐬
 *	@date 06/05/11	ICommunication ����
 *	@date 06/07/14	Open���f�t�H���g�Ńt���[OFF�ݒ���s�Ȃ��悤�ɂ����B
 *					CTS�M���ɂ��Connect/Disconnect�C�x���g���グ��悤�ɂ����B
 *					IsConnect()���\�b�h��ǉ��B
 *	@date 06/08/31	CommViewer �ɑΉ��B
 *	@date 06/09/08	Purge() / IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *	@date 07/02/09	�e�N���X�ύX�B
 *	@date 07/06/11	�e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 07/09/13	�e�N���X�̕ύX�B
 *	@date 08/11/07	OnRs232cOpening() ���\�b�h�ǉ��B
 *	@date 09/10/01	�\���B GetParameter() �ǉ��B
 *	@date 09/10/22	�|�[�g�𕶎���w��\�ɁB
 *	@date 09/11/11	SetParameter() �̎d�l�g���B
 *	@date 10/02/17	EnumExistPortDeviceNames() �ǉ��B
 *	@date 10/05/28	SetReceiveTimeout() �������B
 *	@date 10/06/29	SetReceiveTimeout() ���_�~�[���B�M�������䃁�\�b�h�p�ӁB
 *	@date 11/02/03	SetCheckCtsMode() �̃f�t�H���g�� false �ɁB
 *	@date 11/03/15	Close() ����v���p�e�B�̃^�C�vID,����ύX���Ȃ��悤�ɂ����B
 *	@date 11/07/12	�M�������상�\�b�h�̉��C�B
 *	@date 12/02/17	EnumExistPortDeviceNames() �� USB �� VID, PID �̒l��������悤�ɂ����B
 *	@date 12/10/22	EnumExistPortsByUsb() ��V�K�B
 *	@date 14/07/30	EnumExist�`() �̌��ʂ� sort ����悤�ɂ����B
 */
class CRs232c : public ICommunication, CCopyImpossible
{
public:

	/**@ingroup COMMUNICATION
	 * RS232C �|�[�g�Ǘ�
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbRs232c.h
	 * 
	 *	@date 09/10/22	�V�K
	 */
	class CPort
	{
	public:

		/// �R���X�g���N�^
		CPort(void)
		{
		}

		/**
		 * �R���X�g���N�^.
		 *		"COMx" �ƌ����|�[�g���ɂȂ�܂��B
		 *	@param portNo �|�[�g�i���o�[�B
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
		 * �R���X�g���N�^.
		 *	@param lpszPortName �|�[�g���B�ʏ�A ASCII 3�����A���l�A�ɂȂ�܂��B
		 */
		CPort(LPCTSTR lpszPortName) : m_portName(lpszPortName)
		{
		}

		#ifdef __AFX_H__

		/**
		 * �R���X�g���N�^.
		 *	@param portName �|�[�g���B�ʏ�A ASCII 3�����A���l�A�ɂȂ�܂��B
		 */
		CPort(const CString& portName) : m_portName(portName)
		{
		}

		#endif //__AFX_H__

		/**
		 * [�擾] �|�[�g���擾.
		 *	@return �|�[�g��
		 */
		const CStr& GetPortName(void) const
		{
			return m_portName;
		}

		/**
		 * [�擾] ���l�擾.
		 *	@return ���l
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
		 * [�擾] �v���t�B�b�N�X�擾
		 *	@return �v���t�B�b�N�X
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
		 * [�m�F] �L���m�F.
		 *	@retval true �L��
		 *	@retval false ����
		 */
		bool IsValid(void) const
		{
			return ! m_portName.IsEmpty();
		}

	private:
		/// �؂�ڌ��� ���l������T��
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

	/// �p���e�B�ݒ�l
	enum EParity
	{
		Parity_Non		= NOPARITY,		///< �Ȃ��p���e�B
		Parity_Odd		= ODDPARITY,	///< ��p���e�B
		Parity_Even		= EVENPARITY,	///< �����p���e�B
		Parity_Mark		= MARKPARITY,	///< �}�[�N�p���e�B
		Parity_Space	= SPACEPARITY,	///< �󔒃p���e�B
		//
		#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
		EP_NonParity	= NOPARITY,		///< �Ȃ��p���e�B (���V���{��)
		EP_OddParity	= ODDPARITY,	///< ��p���e�B (���V���{��)
		EP_EvenParity	= EVENPARITY,	///< �����p���e�B (���V���{��)
		EP_MarkParity	= MARKPARITY,	///< �}�[�N�p���e�B (���V���{��)
		EP_SpaceParity	= SPACEPARITY,	///< �󔒃p���e�B (���V���{��)
		#endif // _TnbDOXYGEN
	};
	/// �X�g�b�v�r�b�g�ݒ�l
	enum EStopBits
	{
		StopBits_1		= ONESTOPBIT,	///< 1bit
		StopBits_1_5	= ONE5STOPBITS,	///< 1.5bit
		StopBits_2		= TWOSTOPBITS,	///< 2bit
		//
		#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
		ES_OneStopBit	= ONESTOPBIT,	///< 1bit (���V���{��)
		ES_One5StopBits	= ONE5STOPBITS,	///< 1.5bit (���V���{��)
		ES_TwoStopBits	= TWOSTOPBITS,	///< 2bit (���V���{��)
		#endif // _TnbDOXYGEN
	};

	
	//--------------------------------


	/// �R���X�g���N�^
	CRs232c(void) : m_property(_T("RS232C"), TNBVIW_KIND_RS232C), m_hComPort(INVALID_HANDLE_VALUE)
		, m_dwBaudRate(9600), m_bByteSize(7), m_eParity(Parity_Non)
		, m_eStopBits(StopBits_1), m_boCanBulkSend(true), m_boIsConnect(false), m_isCheckCts(false)
	{
		ASSERT( m_property.receiveTimeout == 0 );
	}

	/// �f�X�g���N�^
	~CRs232c(void)
	{
		CRs232c::Close();
	}

	/**
	 * [�擾] �I�[�v���ݒ�擾.
	 *		SetParameter() �Őݒ肵���l���擾�ł��܂��B
	 *	@param[out] _comPort �|�[�g�i���o�[�B
	 *	@param[out] _dwBaudRate �]�����x[bps]
	 *	@param[out] _bByteSize �L�����N�^�r�b�g[bits]
	 *	@param[out] _eParity �p���e�B�i�����^��j
	 *	@param [out] _eStopBits �X�g�b�v�r�b�g[bits]
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
	 * [�ݒ�] �I�[�v���ݒ�.
	 *		RS232C�̐ݒ���s���܂��B�ʏ�A Open �̑O�Ɏg�p���܂��B
	 *	@note Open �O�Ɏ��s�����ꍇ�K���������܂��B
	 *	@note �I�[�v�����A�{���\�b�h��COM�|�[�g�i���o�[��ύX�����ꍇ�A�N���[�Y����܂��B
	 *	@note �I�[�v�����A�{���\�b�h���g�p�����ꍇ�A�G���[�ɂȂ�\��������܂��B
	 *	@param comPort COM�|�[�g�i���o�[�B ���l�ł�������ł��w��\�ł��B
	 *	@param dwBaudRate �]�����x[bps]
	 *	@param bByteSize �L�����N�^�r�b�g[bits]
	 *	@param eParity �p���e�B�i�����^��j
	 *	@param eStopBits �X�g�b�v�r�b�g[bits]
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
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
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@return false ���s�B�T�|�[�g�ł��Ȃ��������ꂪ�Ԃ�܂��B
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return false;
	}

	/**
	 * [�ݒ�] ���M�ݒ�.
	 *		���M�ݒ�������Ȃ��܂��B�f�t�H���g�̓o���N(�ꊇ)���[�h�ł��B
	 *	@param canBulk ture �Ȃ�o���N(�ꊇ)���[�h�ő��M���܂��B false �Ȃ�A��o�C�g�����M���܂��B
	 */
	void SetSendMode(bool canBulk)
	{
		m_boCanBulkSend = canBulk;
	}

	/**
	 * [�ݒ�] CTS�`�F�b�N�ݒ�.
	 *		CTS�M���������Ă��鎞�����ڑ��Ƃ��郂�[�h�ƁACTS�M���𖳎����郂�[�h��I���ł��܂��B
	 *	@param r ture �Ȃ� CTS�M�����`�F�b�N���܂��B�f�t�H���g�̓`�F�b�N���܂���B
	 */
	void SetCheckCtsMode(bool r = true)
	{
		m_isCheckCts = r;
	}
	
	/**
	 * [�擾] ���\�擾
	 *	@note ���m�ȏ��� Open��Ɏ擾�ł��܂��B
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property;
	}

	/**
	 * [�擾] �n���h���擾
	 *	@retval NULL �I�[�v�����Ă��Ȃ��B
	 *	@retval NULL�ȊO �n���h��
	 */
	HANDLE GetHandle(void) const
	{
		return m_hComPort;
	}

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void)
	{
		Close();
		if ( m_Open() == ERROR_SUCCESS )
		{
			m_property.typeName = CStr::Fmt(_T("RS232C(%s%d)"), m_comPort.GetPrefix(), m_comPort.GetNumber());		// �^�C�v���B
			m_property.typeId = TNBVIW_KIND_RS232C + (m_comPort.GetNumber() & 0xFFFF);
			return true;
		}
		return false;
	}

	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
		if ( m_hComPort != INVALID_HANDLE_VALUE )
		{
			// �|�[�g�̐ݒ���擾
			HANDLE h = m_hComPort;
			m_hComPort = INVALID_HANDLE_VALUE;
			DCB	tDCB;
			if ( ::GetCommState(h, &tDCB) )
			{
				tDCB.fRtsControl = RTS_CONTROL_DISABLE;
				tDCB.fDtrControl = DTR_CONTROL_DISABLE;
				// �|�[�g�̏�Ԃ�ݒ�
				::SetCommState(h, &tDCB);
			}
			::CloseHandle(h);
			m_boIsConnect = false;
		}
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	virtual bool IsOpened(void) const
	{
		return m_hComPort != INVALID_HANDLE_VALUE;
	}

	/**
	 * [�m�F] �ڑ��m�F.
	 *	@note Check() �̌��ʂ�Ԃ��܂��B
	 *	@retval true �ڑ����B
	 *	@retval false �ؒf���B
	 */
	virtual bool IsConnect(void) const
	{
		return m_boIsConnect;
	}

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@retval INVALID_SIZE �s���B
	 *	@retval ��L�ȊO ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		// ���݂̏�Ԃ��擾�B
		DWORD dwErrors;
		COMSTAT tComStat;
		if ( ! ::ClearCommError(m_hComPort, &dwErrors, &tComStat) )
		{
			_GetLastError("ClearCommError");
			return INVALID_SIZE;
		}
		// ���ݎ�M���Ă���Data���擾
		return tComStat.cbInQue;
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B����I�ɌĂяo���K�v������܂��B
	 *	@note CONNECTING �́A CTS �M���ɂ�蔻�f���܂��̂ŁA SetCheckCtsMode() �� true ���w�肵�Ă���K�v������܂��B
	 *	@retval OPENNING	�I�[�v�����
	 *	@retval CONNECTING	�I�[�v�����ڑ����
	 *	@retval CLOSED		�N���[�Y���	
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
	 * [����] ���M.
	 *	@param size �T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval ��L�ȊO ���M�f�[�^�T�C�Y�B�O�̉\��������܂��B
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( ! IsOpened() )
		{
			return INVALID_SIZE;
		}
		if ( m_boCanBulkSend )
		{
			//=== �ꊇ���M
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
		//=== ��o�C�g�����M
		const BYTE* B = static_cast<const BYTE*>(P);
		for ( UINT i = 0; i < size; i++ )
		{
			int iErrCnt = 10;			// �G���[�J�E���g
			size_t writtenSize = 0;
			while ( writtenSize == 0 && iErrCnt > 0 )
			{
				if ( ! WritePort(writtenSize, 1, &B[i], m_hComPort) )
				{
					// WriteFile�֐����G���[��Ԃ���
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
	 * [����] ��M.
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval 0 ��M�f�[�^�Ȃ��B
	 *	@retval ��L�ȊO ��M�f�[�^�T�C�Y�B
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
	 * [�ݒ�] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void)
	{
		::PurgeComm(m_hComPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	}

	/**
	 * [����] �p�f�B���O�ǂݍ���
	 *	@note �I�[�v�����Ă���|�[�g�ŋ�ǂݍ��݂����܂��B
	 *	@param dwTime ��M���Ȃ��̎��Ԏw��B
	 *	@retval true ����
	 *	@retval false ���s
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
			// ���݂̏�Ԃ��擾�B
			BOOL boIsClearComm;
			DWORD dwErrors;
			COMSTAT tComStat;
			// �G���[��ԃ��Z�b�g&��M�������m�F
			boIsClearComm = ::ClearCommError(m_hComPort, &dwErrors, &tComStat);
			if ( ! boIsClearComm )
			{
				boResult = false;
				break;
			}
			//
			// ���ݎ�M���Ă���Data���擾
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
				// RS232C����̓ǂݍ���
				size_t redSize = 0;
				if ( ! ReadPort(redSize, tmp.Ref(), tmp.GetSize(), m_hComPort) )
				{
					//���s
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

	/// �M�������.
	enum
	{
		LOW	 = true,	///< LOW (ON)
		HI = false		///< HI (OFF)
	};

	/**
	 * [�ݒ�] DTR �ݒ�.
	 *		DTR(data-terminal-ready)�M���̏�Ԃ�ݒ肵�܂��B
	 *	@note JIS �ł̖��̂́uER�i�f�[�^�[�����f�B�j�v�B
	 *	@par �N���X�P�[�u���̏ꍇ
	 *		\code
	 *			DTR(4) ----------> DSR(6)
	 *			DSR(6) <---------- DTR(4)
	 *		\endcode
	 *	@param isLow ��ԁB CRs232c::LOW �� CRs232c::HI ���w��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetDtrSignal(bool isLow)
	{
		return !! ::EscapeCommFunction(m_hComPort, isLow ? SETDTR : CLRDTR);
	}

	/**
	 * [�ݒ�] RTS �ݒ�.
	 *		RTS(request-to-send)�M���̏�Ԃ�ݒ肵�܂��B
	 *	@note JIS �ł̖��̂́uRS�i���M�v���j�v�B
	 *	@par �N���X�P�[�u���̏ꍇ
	 *		\code
	 *			RTS(7) ----------> CTS(8)
	 *			CTS(8) <---------- RTS(7)
	 *		\endcode
	 *	@param isLow ��ԁB CRs232c::LOW �� CRs232c::HI ���w��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetRtsSignal(bool isLow)
	{
		return !! ::EscapeCommFunction(m_hComPort, isLow ? SETRTS : CLRRTS);
	}

	/**
	 * [�擾] CTS �m�F.
	 *		CTS(clear-to-send)�M���̏�Ԃ��m�F���܂��B
	 *	@note JIS �ł̖��̂́uCS�i���M�j�v�B
	 *	@par �N���X�P�[�u���̏ꍇ
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
	 * [�擾] DSR �m�F.
	 *		DSR(data-set-ready)�M���̏�Ԃ��m�F���܂��B
	 *	@note JIS �ł̖��̂́uDR�i�f�[�^�Z�b�g���f�B�j�v�B
	 *	@par �N���X�P�[�u���̏ꍇ
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
	 * [�擾] RING �m�F.
	 *		RING �M���̏�Ԃ��m�F���܂��B
	 *	@note �ʕ\�L�Ƃ��āuRI�v�B
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
	 * [�擾] RLSD �m�F.
	 *		RLSD(receive-line-signal-detect)�M���̏�Ԃ��m�F���܂��B
	 *	@note �ʕ\�L�Ƃ��āuDCD�v�B
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
	 * [�擾] CTS �m�F.
	 *	@note IsCtsSignalLow() ���g�p���Ă��������B
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	 */
	bool IsCtsSignalOn(void) const
	{
		return IsCtsSignalLow();
	}

	/**
	 * [�擾] DSR �m�F.
	 *	@note IsDsrSignalLow() ���g�p���Ă��������B
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	*/
	bool IsDsrSignalOn(void) const
	{
		return IsDsrSignalLow();
	}

	/**
	 * [�擾] RING �m�F.
	 *	@note IsRingSignalLow() ���g�p���Ă��������B
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	*/
	bool IsRingSignalOn(void) const
	{
		return IsRingSignalLow();
	}

	/**
	 * [�擾] RLSD �m�F.
	 *	@note IsRlsdSignalLow() ���g�p���Ă��������B
	 *	@retval true ON(LOW)
	 *	@retval false OFF(HI)
	*/
	bool IsRlsdSignalOn(void) const
	{
		return IsRlsdSignalLow();
	}

	/**
	 * [�m�F] �w��|�[�g�͊J���邩
	 *	@param comPort COM�|�[�g�i���o�[�B ���l�ł�������ł��w��\�ł��B
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	static DWORD CanPortBeOpened(CPort comPort)
	{
		// COM�|�[�g���쐬
		CStr strPortName = comPort.GetPortName();
		// �|�[�g���J��
		HANDLE hComPort = ::CreateFile(
						strPortName,					// �t�@�C���̖��O�̃A�h���X
						GENERIC_READ | GENERIC_WRITE,	// �A�N�Z�X (�ǂݎ�肨��я�������) ���[�h
						0,								// ���L���[�h
						NULL,							// �Z�L�����e�B�L�q�q�̃A�h���X
						OPEN_EXISTING,					// �쐬���@
						FILE_ATTRIBUTE_NORMAL,			// �t�@�C������
						NULL							// �R�s�[���鑮���t���t�@�C���̃n���h��
		);
		// �|�[�g���J���֐����ُ��Ԃ����ꍇ
		if ( hComPort == INVALID_HANDLE_VALUE )
		{
			return _GetLastError("CreateFile");
		}
		::CloseHandle(hComPort);
		return 0;
	}

	#ifndef _TnbRS232C_EnumExistPorts_DISABLE

	/// �f�o�C�X���
	///	@note vid, pid �� USB �f�o�C�X�łȂ��ꍇ�� all 0 �ɂȂ�܂��B
	struct TDevice : IComparableT<TDevice>
	{
		WORD	port;			///< �|�[�gNo
		CStr	description;	///< ����
		CStr	manufacturer;	///< �����Ǝ�
		WORD	vid;			///< VendorID (USB)
		WORD	pid;			///< ProductID (USB)
		/// �R���X�g���N�^
		TDevice(void) : port(0), vid(0), pid(0)
		{
		}
		/**
		 * [�擾] ������.
		 *	@note ���[�J�[�A�����A�|�[�gNo���܂񂾕������Ԃ��܂��B
		 *	@return ������.
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
		 * [�m�F] ��r
		 *	@param t ��r�Ώ�
		 *	@retval �O		������ t �Ɠ���
		 *	@retval �P�ȏ�	������ t ���傫��
		 *	@retval �O����	������ t ��菬����
		 */
		virtual INT_PTR Compare(const TDevice& t) const
		{
			return port - t.port;
		}
	};

	/// �f�o�C�X���z��.
	typedef CVectorT<TDevice> TDeviceVector;

	/**
	 * [�擾] ���݂���Port�ꗗ
	 *	@return ���݂���|�[�gNo�A���O�ꗗ�iSize���Q��port���e�� 1,4 �Ȃ� "COM1" "COM4"���g�p�\�j
	 */
	static TDeviceVector EnumExistPortDeviceNames(void)
	{
		CMyFinder ff;
		CRegistryAccessor reg(HKEY_LOCAL_MACHINE, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS);
		return ff.Execute(reg[_T("SYSTEM\\CurrentControlSet\\Enum")]);
	}

	/**
	 * [�擾] ���݂���Port�ꗗ
	 *	@return ���݂���|�[�gNo�ꗗ�iSize���Q�œ��e�� 1,4 �Ȃ� "COM1" "COM4"���g�p�\�j
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
	 * [�擾] USB�V���A��Port�ꗗ.
	 *	@param vid	VendorID (USB)
	 *	@param pid	ProductID (USB)
	 *	@return ���݂���|�[�gNo�ꗗ�iSize���Q�œ��e�� 1,4 �Ȃ� "COM1" "COM4"���g�p�\�j
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
	 * [�ʒm] �I�[�v���ʒm.
	 *		�I�[�v�����A�R�[������܂��B
	 *	@param[in] hComPort �n���h��
	 *	@param[in,out] _tDCB ���݂� DCB ���n����܂��B�ύX���邱�ƂŐݒ��ύX�ł��܂��B 
	 *	@param[in,out] _tCommTimeOuts ���݂� COMMTIMEOUTS ���n����܂��B�ύX���邱�ƂŐݒ��ύX�ł��܂��B
	 *	@retval true �I�[�v�������p���B
	 *	@retval false �I�[�v���������~�iOpen�����s���܂��j�B
	 */
	virtual bool OnRs232cOpening(HANDLE hComPort, DCB& _tDCB, COMMTIMEOUTS& _tCommTimeOuts)
	{
		return true;
	}

	/**
	 * [�ʒm] �|�[�g�쐬.
	 *		�|�[�g�쐬���A�R�[������܂��B
	 *	@param lpszPortName �|�[�g��
	 *	@retval INVALID_HANDLE_VALUE ���s�B
	 *	@retval ��L�ȊO �����B�l�̓n���h���B�g�p��� CloseHandle() ���Ă��������B
	 */
	virtual HANDLE CreatePort(LPCTSTR lpszPortName) const
	{
		DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;	// �A�N�Z�X (�ǂݎ�肨��я�������) ���[�h
		DWORD flag = FILE_ATTRIBUTE_NORMAL;
		HANDLE h = ::CreateFile(lpszPortName, desiredAccess, 0, NULL, OPEN_EXISTING, flag, NULL);
		_GetLastError("CreateFile");
		return h;
	}

	/**
	 * [�ʒm] �|�[�g��������.
	 *		�|�[�g�������ݎ��A�R�[������܂��B
	 *	@param[out] _writtenSize �������񂾃T�C�Y���i�[����܂��B
	 *	@param[in] len �������݃f�[�^�T�C�Y
	 *	@param[in] P �������݃f�[�^
	 *	@param[in] h �n���h��
	 *	@retval true ����
	 *	@retval false ���s
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
	 * [�ʒm] �|�[�g�ǂݍ���.
	 *		�|�[�g�ǂݍ��ݎ��A�R�[������܂��B
	 *	@param[out] _redSize �������񂾃T�C�Y���i�[����܂��B
	 *	@param[out] _P �ǂݍ��݃f�[�^���i�[����܂��B
	 *	@param[in] len �ǂݍ��݃f�[�^�ő�
	 *	@param[in] h �n���h��
	 *	@retval true ����
	 *	@retval false ���s
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
	 * RS232C���I�[�v��
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD m_Open(void)
	{
		if ( ! m_comPort.IsValid() )
		{
			return ERROR_INVALID_PARAMETER;
		}
		// �|�[�g���J��
		HANDLE hComPort = CreatePort(m_comPort.GetPortName());
		// �|�[�g���J���֐����ُ��Ԃ����ꍇ
		if ( hComPort == INVALID_HANDLE_VALUE )
		{
			return ::GetLastError();
		}
		::PurgeComm(hComPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		// �|�[�g�̐ݒ���擾
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
	 * RS232C�|�[�g�ݒ�
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD m_PortSetting(HANDLE hComPort)
	{
		DCB	tDCB;
		COMMTIMEOUTS tCommTimeOuts;
		if ( ! ::GetCommState(hComPort, &tDCB) || ! ::GetCommTimeouts(hComPort, &tCommTimeOuts) )
		{
			return _GetLastError("GetCommState/GetCommTimeouts");
		}
		// �|�[�g��ݒ�
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
		//�\�t�g�t���[��Ԃɂ���B
		tCommTimeOuts.ReadIntervalTimeout			= 0xFFFFFFFF;
		tCommTimeOuts.ReadTotalTimeoutMultiplier	= 0;
		tCommTimeOuts.ReadTotalTimeoutConstant		= 0;
		tCommTimeOuts.WriteTotalTimeoutMultiplier	= 1;
		tCommTimeOuts.WriteTotalTimeoutConstant		= 1000; //1s
		tDCB.fRtsControl		= RTS_CONTROL_DISABLE;
		tDCB.fDtrControl		= DTR_CONTROL_DISABLE;
		tDCB.fOutxCtsFlow		= FALSE;				//ClearToSend�Ď����Ȃ�
		tDCB.fOutxDsrFlow		= FALSE;				//DataSetReady�Ď����Ȃ�
		tDCB.fDsrSensitivity	= FALSE;				//DSR�Ď����Ȃ�
		tDCB.fTXContinueOnXoff	= FALSE;				//XOffChar�`�F�b�N�Ȃ�
		tDCB.fInX				= FALSE;				//XON/XOFF�t���[����Ȃ�
		tDCB.fOutX				= FALSE;				//XON/XOFF�t���[����Ȃ�
		if ( ! OnRs232cOpening(hComPort, tDCB, tCommTimeOuts) )
		{
			return ERROR_NOT_READY; //���[�U��~
		}
		// �|�[�g�̏�Ԃ�ݒ�
		if ( ! ::SetCommState(hComPort, &tDCB) || ! ::SetCommTimeouts(hComPort, &tCommTimeOuts) )
		{
			return _GetLastError("SetCommState/SetCommTimeouts");
		}
		return 0;
	}
	TProperty		m_property;			///< ���\
	HANDLE			m_hComPort;			///< �n���h��
	CPort			m_comPort;			///< �Ώ�COM�|�[�g
	DWORD			m_dwBaudRate;		///< COM�|�[�g�p�ݒ�G���[�g
	BYTE			m_bByteSize;		///< COM�|�[�g�p�ݒ�G�T�C�Y
	EParity			m_eParity;			///< COM�|�[�g�p�ݒ�G�p���e�B
	EStopBits		m_eStopBits;		///< COM�|�[�g�p�ݒ�G�X�g�b�v�r�b�g
	mutable bool	m_boIsConnect;		///< CTS�ɂ��ڑ��C�x���g�����p
	bool			m_boCanBulkSend;	///< ���M���A�ꊇ�ő���H
	bool			m_isCheckCts;		///< CTS�`�F�b�N���s���H

	#ifndef _TnbRS232C_EnumExistPorts_DISABLE

	/// EnumExistPortDeviceNames() �p�����N���X
	class CMyFinder : private CDeepAccessFinder
	{
		DEFSUPER(CDeepAccessFinder);
	public:
		// ���s
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

	friend class CRs232cTest;		///< �t�����h�N���X�錾
};



/// �n�[�h�t���[����^�C�v
/// @date 19/04/27  DSR, DTR �͐���ΏۊO�ɂ���
class CRs232cHf : public CRs232c
{
protected:
	/**
	 * [�ʒm] �I�[�v���ʒm.
	 *		�I�[�v�����A�R�[������܂��B
	 *	@param[in] hComPort �n���h��
	 *	@param[in,out] _tDCB ���݂� DCB ���n����܂��B�ύX���邱�ƂŐݒ��ύX�ł��܂��B 
	 *	@param[in,out] _tCommTimeOuts ���݂� COMMTIMEOUTS ���n����܂��B�ύX���邱�ƂŐݒ��ύX�ł��܂��B
	 *	@retval true �I�[�v�������p���B
	 *	@retval false �I�[�v���������~�iOpen�����s���܂��j�B
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
