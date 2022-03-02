#pragma once
/**
 *@file
 * USB HID�̃A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCommunication.h"
#include "TnbVector.h"
#include "TnbUsbHidPathName.h"
#include "TnbOverlapReader.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION USB
 * USB-HID�A�N�Z�X�N���X
 *
 *		HID�f�o�C�X�̓��o�͂��T�|�[�g���܂��B
 *	@note �I�[�v����ɁA�f�o�C�X�������ꂽ�ꍇ�A 
 *			IsConnect() �� false ���Ԃ�܂��i IsOpened() �� true �j�B
 *
 *	@see CUsbHidFinder �ɂ��HID�l�[�����擾���Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbUsbHid.h
 * 
 *	@date 06/09/04	�V�K�쐬
 *	@date 06/09/09	IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *					ECE_Connect , ECE_Disconnect �C�x���g�A�ǉ��B
 *	@date 06/12/08	�w��f�o�C�X�̑��݃`�F�b�N���\�b�h�A�ǉ��B
 *	@date 07/02/09	�e�N���X�ύX�B
 *	@date 07/04/20	Read�Ń��b�N����Close�o���Ȃ��̂����P�B
 *	@date 07/04/25	�����@�\�͕ʃN���X�ցBDevicePathName��Open����悤�ɕύX�B
 *	@date 07/06/11	�e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 07/09/13	�e�N���X�ύX�B
 *	@date 08/02/12	Report ID�̐ݒ�\�ɁB
 *	@date 09/03/19	�ǂݍ��݂� COverlapReader ���g���悤�ɏC���B
 *	@date 09/04/09	SetReceiveTimeout() ��V�K�B
 *	@date 09/05/15	Send �̖߂�l���������Ă��s���l�������B�C���B
 *	@date 10/03/17	GetString�n���\�b�h�ǉ��B�@
 *	@date 10/05/28	SetReceiveTimeout() �����ǁB
 *	@date 10/11/26	IsConnect() �̉��ǁB
 *	@date 11/10/18	SetWithTimeoutSendMode() ��V�K�B
 *	@date 11/11/29	SetSendTimeout() ��V�K�B SetWithTimeoutSendMode() ��p�~�B
 *	@date 12/03/16	�n���h���N���[�Y���������P�B
 *	@date 13/01/09	�ǂݍ��݂���L����Ă���f�o�C�X�ł������t���ŃI�[�v���ł���悤�ɂ����B
 */
class CUsbHid : public ICommunication, CUsbHidApi, CCopyImpossible
{
	DEFSUPER(ICommunication);
public:

	/// �R���X�g���N�^ 
	explicit CUsbHid(void) 
		: m_hUpPipe(INVALID_HANDLE_VALUE), m_hDownPipe(INVALID_HANDLE_VALUE), m_sendTimeout(0)
		, m_reportId(0), m_property(_T("HID"), TNBVIW_KIND_USB, 100)
	{
		ASSERT( m_property.receiveTimeout == 100 );
	}

	/// �f�X�g���N�^
	virtual ~CUsbHid(void)
	{
		Close();
	}

	/**
	 * [�擾] ���\�擾.
	 *	@note ���m�ȏ��� Open��Ɏ擾�ł��܂��B
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property;
	}

	/**
	 * [����] �I�[�v��.
	 *	@note true ���Ԃ��Ă��A��M�ł��Ȃ��i CanReceive() �� false ��Ԃ��j�ꍇ������܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void)
	{
		Close();
		// �f�o�C�X�̃I�[�v��
		DWORD x = (m_sendTimeout != 0) ? FILE_FLAG_OVERLAPPED : 0;
		HANDLE hDown = ::CreateFile(
						m_hidName.GetPathName(), GENERIC_WRITE,
						(FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, x, 0);
		if ( hDown == INVALID_HANDLE_VALUE )
		{
			_GetLastError("CreateFile");
			::SetLastError(ERROR_NOT_FOUND);
			return false;
		}
		HANDLE hUp = ::CreateFile (
						m_hidName.GetPathName(), (GENERIC_READ | GENERIC_WRITE),
						(FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		if ( hUp == INVALID_HANDLE_VALUE )
		{
			_GetLastError("CreateFile");
			// Read ���o���Ȃ��Ă��ȉ��ɑ����B
		}
		m_hDownPipe = hDown;
		m_hUpPipe = hUp;
		//
		TRACE0( "CHid::Open���܂���\n" );
		m_property.typeName = CStr::Fmt(_T("HID-V%04X-P%04X"), m_hidName.GetVendorId(), m_hidName.GetProductId());	// �^�C�v���B
		m_property.typeId = TNBVIW_KIND_USB + (reinterpret_cast<ULONG_PTR>(this) & 0xFFFF);
		m_property.receivePacketSize = m_hidName.GetInputReportByteLength() - 1;
		m_property.sendPacketSize = m_hidName.GetOutputReportByteLength() - 1;
		m_reader.Attach(m_hUpPipe, m_property.receivePacketSize + 1);
		return true;
	}

	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
		m_reader.Detach();
		//Port�̃N���[�Y
		if ( m_hUpPipe != INVALID_HANDLE_VALUE )
		{
			m_CloseHandle(m_hUpPipe);
			m_hUpPipe = INVALID_HANDLE_VALUE;
		}
		if ( m_hDownPipe != INVALID_HANDLE_VALUE )
		{
			m_CloseHandle(m_hDownPipe);
			m_hDownPipe = INVALID_HANDLE_VALUE;
		}
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	virtual bool IsOpened(void) const
	{
		return m_hDownPipe != INVALID_HANDLE_VALUE; 
	}

	/**
	 * [�m�F] �ڑ��m�F.
	 *	@retval true �ڑ����B
	 *	@retval false �ؒf���B
	 */
	virtual bool IsConnect(void) const
	{
		GetReceiveBufferCount();
		if ( ::GetLastError() == ERROR_DEVICE_NOT_CONNECTED )
		{
			return false;
		}
		return IsOpened();
	}

	/**
	 * [�m�F] ���M�\�m�F.
	 *	@retval true ���M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanSend(void) const
	{
		return (m_hDownPipe != INVALID_HANDLE_VALUE) && (m_hidName.GetOutputReportByteLength() > 0);
	}

	/**
	 * [�m�F] ��M�\�m�F.
	 *	@retval true ��M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanReceive(void) const
	{
		return (m_hUpPipe != INVALID_HANDLE_VALUE) && (m_hidName.GetInputReportByteLength() > 0);
	}

	/**
	 * [�ݒ�] ���|�[�gID�ݒ�.
	 *	@param id ���M���|�[�gID�A�w��B
	 */
	void SetReportId(BYTE id)
	{
		m_reportId = id;
	}

	/**
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@return ��� true�B
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		m_property.receiveTimeout = to;
		return true;
	}

	/**
	 * [����] ���M.
	 *	@note OutputReport���Ŋ���؂�Ȃ��f�[�^�T�C�Y��n���ƁA����Ȃ��Ƃ���͂O��Padding����܂��B
	 *	@param size �T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval ��L�ȊO ���M�f�[�^�T�C�Y�B
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( ! CanSend() )
		{
			return INVALID_SIZE;
		}
		size_t sizeOutputReport = m_hidName.GetOutputReportByteLength();
		size_t sentTotalSize = 0;		// ���M�ς݃f�[�^�T�C�Y���v
		size_t sendSize = 0;			// ���M�f�[�^�T�C�Y
		CWorkMem work(sizeOutputReport);
		work[0] = m_reportId;		// REPORT_ID
		const BYTE* lpbData = static_cast<const BYTE*>(P);
		//
		bool isSuccess = true;
		//
		while ( true )
		{
			if ( (size - sentTotalSize) < sizeOutputReport )
			{
				sendSize = size - sentTotalSize;
				memset(work, 0, sizeOutputReport);
			}
			else
			{
				sendSize = sizeOutputReport -1;
			}
			// ���M�f�[�^ �擪�͕K��REPORT_ID�l�Ƃ���i0x00 �Œ�j
			MemCopy(&work[1], &lpbData[sentTotalSize], sendSize);
			// �f�[�^���M(NG���̓��g���C����(10��܂�))
			bool r = false;
			loop ( i, 10 )
			{
				DWORD dwWriteLen = 0;
				if ( m_Write(m_hDownPipe, work, ToDword(sizeOutputReport), &dwWriteLen) )
				{
					if ( sizeOutputReport == dwWriteLen )
					{
						r = true; //����
						break;
					}
				}
				if ( _GetLastError("WriteFile") == ERROR_DEVICE_NOT_CONNECTED )
				{
					Close();
					break;
				}
			}
			if ( ! r )
			{
				//���M���s
				isSuccess = false;
				break;
			}
			sentTotalSize += sendSize;	// ���M�ς݃T�C�Y
			if ( size <= sentTotalSize )
			{
				break;
			}
		}
		return isSuccess ? size : INVALID_SIZE;
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
		if ( ! CanReceive() )
		{
			return INVALID_SIZE;
		}
		size_t sizeInputReport = m_property.receivePacketSize; 
		CWorkMem work(sizeInputReport + 1);
		size_t r = m_reader.Read(work.Ref(), m_property.receiveTimeout);
		if ( r != INVALID_SIZE && r != 0 )
		{
			r--;
			if ( r > size )
			{
				r = size;
			}
			MemCopy(static_cast<BYTE*>(_P), work.Ref() + 1, r);
		}
		else
		{
			if ( ::GetLastError() == ERROR_DEVICE_NOT_CONNECTED )
			{
				Close();
			}
		}
		return r;
	}

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@retval INVALID_SIZE �s���B
	 *	@retval ��L�ȊO ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		return INVALID_SIZE;
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B
	 *	@retval CONNECTING	�I�[�v�����ڑ����
	 *	@retval CLOSED		�N���[�Y���	
	 */
	virtual EState Check(void) const
	{
		return IsConnect() ? CONNECTING : CLOSED;
	}

	/**
	 * [����] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void) 
	{
		HidD_FlushQueue(m_hDownPipe);
	}

	/**
	 * [�ݒ�] USB-HID�f�o�C�X�w��.
	 *		�g�p����̃f�o�C�X���w�肵�܂��B
	 *	@param hidName HID���i�g�p����̂̓p�X�l�[���̂݁j�B CUsbHidFinder ���g���A���邱�Ƃ��o���܂��B
	 *	@retval ERROR_SUCCESS ����
	 *	@retval ERROR_SUCCESS�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD SetHidPathName(const CUsbHidPathName& hidName)
	{
		if ( hidName.IsEmpty() )
		{
			return ERROR_NOT_FOUND;
		}
		return SetHidPathName(hidName.GetPathName());
	}

	/**
	 * [�ݒ�] USB-HID�f�o�C�X�w��.
	 *		�g�p����̃f�o�C�X���w�肵�܂��B
	 *	@param lpszPathName �p�X�l�[���B CUsbHidFinder ���g���A���邱�Ƃ��o���܂��B
	 *	@retval ERROR_SUCCESS ����
	 *	@retval ERROR_SUCCESS�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD SetHidPathName(LPCTSTR lpszPathName)
	{
		Close();
		return m_hidName.SetPathName(lpszPathName);
	}		

	/**
	 * [�ݒ�] �^�C���A�E�g�t�����M���[�h.
	 *	@note �I�[�v������O�Ɏg�p���Ă��������B�I�[�v����Ɏg�p����ƃN���[�Y����܂��B
	 *	@note 0 �ȊO���w�肷��ƁA���M�̍ہA�I�[�o�[���b�v���[�h�ŃI�[�v�������f�o�C�X���g�p���܂��B
	 *		�f�t�H���g�� 0 �ł�(�ʏ�ɃI�[�v�������f�o�C�X���g�p����)�B
	 *	@param to ���M�^�C���A�E�g����(ms)�B
	 */
	void SetSendTimeout(DWORD to)
	{
		Close();
		m_sendTimeout = to;
	}

	/**
	 * [���M] SetOutputReport
	 *	@param dataSize ���M����f�[�^���B GetOutputReportByteLength() �̖߂�l�Ɠ��������ł���K�v������܂��B
	 *	@param lpcvData ���M����f�[�^�B
	 *	@retval true	�����B
	 *	@retval	false	���s�B
	 */
	bool SetOutputReport(size_t dataSize, LPCVOID lpcvData)
	{
		if ( m_hidName.GetOutputReportByteLength() == dataSize )
		{
			return !! HidD_SetOutputReport(m_hDownPipe, const_cast<LPVOID>(lpcvData), ToDword(dataSize));
		}
		return false;
	}

	/**
	 * [��M] GetInputReport
	 *	@note �������񒷂́A GetInputReportByteLength() �̖߂�l�Ɠ����ɂȂ�܂��B
	 *	@retval Invalid��� ���s�B
	 *	@retval	����ȊO �����B���e�͎�M�f�[�^�B
	 */
	CByteVector GetInputReport(void) const
	{
		CByteVector vb;
		size_t l = m_hidName.GetInputReportByteLength();
		if ( l > 0 )
		{
			BOOL r = !! HidD_GetInputReport(m_hUpPipe, vb.GetBuffer(l), ToDword(l));
			vb.ReleaseBuffer();
			if ( ! r )
			{
				vb.Invalid();
			}
		}
		return vb;
	}

	/**
	 * [���M] SetFeature
	 *	@param dataSize ���M����f�[�^���B GetFeatureReportByteLength() �̖߂�l�Ɠ��������ł���K�v������܂��B
	 *	@param lpcvData ���M����f�[�^�B
	 *	@retval true	�����B
	 *	@retval	false	���s�B
	 */
	bool SetFeature(size_t dataSize, LPCVOID lpcvData)
	{
		if ( m_hidName.GetFeatureReportByteLength() == dataSize )
		{
			return !! HidD_SetFeature(m_hDownPipe, const_cast<LPVOID>(lpcvData), ToDword(dataSize));
		}
		return false;
	}

	/**
	 * [��M] GetFeature
	 *	@todo REPORT ID ���w��ł��Ȃ��Ƃ��������B
	 *	@note �������񒷂́A GetFeatureReportByteLength() �̖߂�l�Ɠ����ɂȂ�܂��B
	 *	@retval Invalid��� ���s�B
	 *	@retval	����ȊO �����B���e�͎�M�f�[�^�B
	 */
	CByteVector GetFeature(void) const
	{
		CByteVector vb;
		size_t l = m_hidName.GetFeatureReportByteLength();
		if ( l > 0 )
		{
			BOOL r = !! HidD_GetFeature(m_hDownPipe, vb.GetBuffer(l), ToDword(l));
			vb.ReleaseBuffer();
			if ( ! r )
			{
				vb.Invalid();
			}
		}
		return vb;
	}

	/**
	 * [�擾] �p�X�l�[�����擾
	 *	@note Open���Ă��Ȃ����̖߂�l�͕s��ł��B
	 *	@return �p�X�l�[�����B
	 */
	const CUsbHidPathName& GetName(void) const
	{
		return m_hidName;
	}

	/**
	 * [�擾] Feature���|�[�g���擾
	 *	@note Open���Ă��Ȃ����̖߂�l�͕s��ł��B
	 *	@return Feature���|�[�g���B
	 */
	size_t GetFeatureReportByteLength(void) const
	{
		return m_hidName.GetFeatureReportByteLength();
	}

	/**
	 * [�擾] Manufacturer ������擾
	 *	@retval Empty��� ���s.
	 *	@retval ��L�ȊO �����B������� Manufacturer.
	 */
	CStr GetManufacturerString(void) const
	{
		CUnicode s;
		bool r = !! HidD_GetManufacturerString(m_hDownPipe, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [�擾] Product ������擾
	 *	@retval Empty��� ���s.
	 *	@retval ��L�ȊO �����B������� Product.
	 */
	CStr GetProductString(void) const
	{
		CUnicode s;
		bool r = !! HidD_GetProductString(m_hDownPipe, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [�擾] SerialNumber ������擾
	 *	@retval Empty��� ���s.
	 *	@retval ��L�ȊO �����B������� SerialNumber.
	 */
	CStr GetSerialNumberString(void) const
	{
		CUnicode s;
		bool r = !! HidD_GetSerialNumberString(m_hDownPipe, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [�擾] INDEXED ������擾
	 *	@param[in] index �C���f�b�N�X
	 *	@retval Empty��� ���s.
	 *	@retval ��L�ȊO �����B������� SerialNumber.
	 */
	CStr GetIndexedString(INDEX index) const
	{
		CUnicode s;
		bool r = !! HidD_GetIndexedString(m_hDownPipe, index, s.GetBuffer(256), 256 * sizeof(WCHAR));
		s.ReleaseBuffer();
		return r ? CStr(s) : _T("");
	}

	/**
	 * [�擾] ���̓o�b�t�@���擾.
	 *	@retval 0 ���s.
	 *	@retval 0�ȊO �����B�l�̓o�b�t�@��.
	 */
	DWORD GetReceiveBufferCount(void) const
	{
		ULONG numberBuffers;
		BOOLEAN r = HidD_GetNumInputBuffers(m_hDownPipe, &numberBuffers);
		return r ? numberBuffers : 0;
	}

	/**
	 * [�ݒ�] ���̓o�b�t�@���ݒ�.
	 *	@note XP �ł͍ő� 512 �A 2000 �ł� �ő� 200 �B�Œ�� 2 �B�f�t�H���g�� 32 �ł��B
	 *	@param c ���̓o�b�t�@��.
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetReceiveBufferCount(DWORD c)
	{
		return !! HidD_SetNumInputBuffers(m_hDownPipe, c);
	}

private:
	bool m_CloseHandle(HANDLE h)
	{
		DWORD f;
		if ( ! ::GetHandleInformation(h, &f) )
		{
			_GetLastError("GetHandleInformation");
			return false;
		}
		return !! ::CloseHandle(h);
	}
	bool m_Write(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
	{
		if ( m_sendTimeout == 0 )
		{
			return !! ::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL);
		}
		OVERLAPPED overLapped;
		Zero(overLapped);
		HANDLE hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); 
		::ResetEvent(hEvent);
		overLapped.hEvent = hEvent;
		BOOL r = ::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, &overLapped);
		if ( r )
		{
			// ����
		}
		else if ( ::GetLastError() == ERROR_IO_PENDING )
		{
			// �񓯊��ǂݎ�葀��̌��ʂ��`�F�b�N����B
			::Sleep(0);
			switch ( ::WaitForSingleObject(overLapped.hEvent, m_sendTimeout) )
			{
			case WAIT_OBJECT_0:
				r = ::GetOverlappedResult(hFile, &overLapped, lpNumberOfBytesWritten, FALSE);
				break;
			case WAIT_TIMEOUT:
			case WAIT_ABANDONED:
			case WAIT_FAILED:
			default:
				::CancelIo(hFile);
				r = FALSE;
				break;
			}
		}
		::CloseHandle(hEvent);
		return !! r;
	}

	HANDLE			m_hUpPipe;			///< ��M�p�n���h��
	HANDLE			m_hDownPipe;		///< ���M�p�n���h��
	CUsbHidPathName	m_hidName;			///< HID�l�[�����
	TProperty		m_property;			///< ���\
	BYTE			m_reportId;			///< ���M���郌�|�[�gID�B
	COverlapReader	m_reader;			///< �񓯊��ǂݍ��݁B
	DWORD			m_sendTimeout;		///< ���M���̃^�C���A�E�g
	friend class CUsbHidTest;
};



}; // TNB
