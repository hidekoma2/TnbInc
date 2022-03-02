#pragma once
/**
 *@file
 * CopyData���b�Z�[�W�֌W�̃w�b�_
 *
 *		WM_COPYDATA�Ȃǂ��g�����E�B���h�E�ԒʐM�������T�|�[�g���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * �R�s�[�f�[�^���b�Z�[�W�N���X
 *
 *		ICommunication ���������Ă��܂��B
 *		WM_COPYDATA ���b�Z�[�W���g���A�E�B���h�E�ԂŃf�[�^�𑗎�M���邱�Ƃ��o���܂��B
 *
 *	@note ��M�������@�\�����邽�߂ɁA�e��WindowProc ���t�b�N���A
 *			�{�N���X�� RelayEvent() ���R�[������K�v���L��܂��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *	 LRESULT CDialupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)		
 *	 {
 *		LRESULT res;
 *		if ( m_copyDataMessage.RelayEvent(message, wParam, lParam, res) ) {
 *			return res;
 *		}
 *	 	return CDialog::WindowProc(message, wParam, lParam);
 *	 }
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbCopyDataMessage.h
 * 
 *	@date 08/03/24 �V�K�쐬
 *	@date 10/05/28 SetReceiveTimeout() �������B
 */
class CCopyDataMessage : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);
	_super::TProperty	m_property;			///< �v���p�e�B
protected:
	CVectorT<BYTE>		m_vbReceivedData;	///< ��M�����f�[�^�B
	HWND				m_hPartnerWnd;		///< ���M��̃E�B���h�E
	ULONG_PTR			m_dwCookie;			///< ���ʎq

public:

	/** 
	 * �R���X�g���N�^ 
	 */
	CCopyDataMessage(void) : m_property(_T("CopyDataMsg"), TNBVIW_KIND_TEMP), m_dwCookie(0), m_hPartnerWnd(NULL)
	{
	}

	/**
	 * [�擾] ���\�擾
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property; 
	}

	/**
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@return ��� false �B
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return false;
	}

	/**
	 * [�ݒ�] �I�[�v���ݒ�
	 *	@param hPartnerWnd ���M��E�B���h�E�n���h��
	 *	@param dwCookie ���ʎq
	 */
	void SetParamater(HWND hPartnerWnd, ULONG_PTR dwCookie = 0)
	{
		m_hPartnerWnd = hPartnerWnd;
		m_dwCookie = dwCookie;
	}

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B SetParamater() �Őݒ肵�����M�悪���݂��Ȃ��ꍇ�A���s���܂��B
	 */
	virtual bool Open(void) 
	{
		return IsOpened();
	}
	
	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void) 
	{
		m_vbReceivedData.RemoveAll();
		return; 
	}
	
	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����Ă���B
	 *	@retval false ���s�B SetParamater() �Őݒ肵�����M�悪���݂��Ȃ��ꍇ�A���s���܂��B
	 */
	virtual bool IsOpened(void) const 
	{
		return !! ::IsWindow(m_hPartnerWnd);
	}

	/**
	 * [����] ���M.
	 *	@param size �T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval ��L�ȊO ���M�f�[�^�T�C�Y�B
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( IsOpened() )
		{
			COPYDATASTRUCT data;
			data.dwData = m_dwCookie;
			data.cbData = ToDword(size);
			data.lpData = const_cast<PVOID>(P);
			LPARAM l = reinterpret_cast<LPARAM>(&data);
			DWORD_PTR result = 0;
			if ( ::SendMessageTimeout(m_hPartnerWnd, WM_COPYDATA, 0, l, SMTO_NORMAL, 1000, &result) )
			{
				return result;
			}
		}
		return INVALID_SIZE;
	}

	/**
	 * [����] ��M.
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@return ��M�f�[�^�T�C�Y�B
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		size_t l = m_vbReceivedData.GetElements(size, static_cast<BYTE*>(_P));
		m_vbReceivedData.RemoveElements(0, l);
		return l;
	}

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@return ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		return m_vbReceivedData.GetSize();
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B
	 *	@retval CONNECTING	�I�[�v�����ڑ����
	 *	@retval CLOSED		�N���[�Y���		 
	 */
	virtual EState Check(void) const
	{
		if ( ::IsWindow(m_hPartnerWnd) )
		{
			return CONNECTING; 
		}
		return CLOSED;
	}

	/**
	 * [����] �p�[�W.
	 */
	virtual void Purge(void)
	{
		m_vbReceivedData.RemoveAll();
	}

	/**
	 * [����] �C�x���g���p
	 *	@note �e�E�B���h�E�� WindowProc() ���烁�b�Z�[�W�𗬂��Ă��������B
	 *	@param[in] message �e�_�C�A���O����M�������b�Z�[�W
	 *	@param[in] wParam �e�_�C�A���O����M�������b�Z�[�W��WPARAM
	 *	@param[in] lParam �e�_�C�A���O����M�������b�Z�[�W��LPARAM
	 *	@param[out] _result �{�֐��� true �̍ہA���U���g�Ƃ��Ďg���l���i�[����B
	 *	@retval true �������{�B _result �Ƀ��U���g�l���i�[����Ă���B
	 *	@retval false �����Ȃ��������B
	 */
	bool RelayEvent(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& _result)
	{
		if ( message == WM_COPYDATA )
		{
			COPYDATASTRUCT* pData = reinterpret_cast<COPYDATASTRUCT*>(lParam);
			if ( pData->dwData == m_dwCookie )
			{
				size_t size = pData->cbData;
				PVOID P = pData->lpData;
				_result = m_vbReceivedData.AddElements(size, static_cast<const BYTE*>(P));
				return true;
			}
		}
		return false;
	}
};



/**@ingroup COMMUNICATION
 * �R�s�[�f�[�^�|�X�g���b�Z�[�W�N���X
 *
 *		ICommunication ���������Ă��܂��B
 *		PostMessage �ŔC�ӂ̃��b�Z�[�W��p���A�E�B���h�E�ԂŃf�[�^�𑗎�M���邱�Ƃ��o���܂��B
 *
 *	@attention ���ʎq�̏��2bit�́A�Ǘ��Ɏg�p���Ă��܂��܂��B
 *
 *	@note ��M�������@�\�����邽�߂ɁA�e��WindowProc ���t�b�N���A
 *			�{�N���X�� RelayEvent() ���R�[������K�v���L��܂��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *	 LRESULT CDialupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)		
 *	 {
 *		LRESULT res;
 *		if ( m_copyDataPostMessage.RelayEvent(message, wParam, lParam, res) ) {
 *			return res;
 *		}
 *	 	return CDialog::WindowProc(message, wParam, lParam);
 *	 }
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbCopyDataMessage.h
 * 
 *	@date 08/03/24 �V�K�쐬
 */
class CCopyDataPostMessage : public CCopyDataMessage
{
	DEFSUPER(CCopyDataMessage);
	UINT		m_msg;			///< ���b�Z�[�W
	enum
	{
		COOKIE_MASK		= 0x3FFFFFFF,	///< COOKIE�}�X�N
		HASBYTE_MASK	= 0xC0000000,	///< BYTE���}�X�N
		HASBYTE_SHIFT	= 30,			///< BYTE���V�t�g�l
		HAS1BYTE		= 0x00000000,	///< BYTE���(1byte)
		HAS2BYTE		= 0x40000000,	///< BYTE���(2bytes)
		HAS3BYTE		= 0x80000000,	///< BYTE���(3bytes)
		HAS4BYTE		= 0xC0000000,	///< BYTE���(4bytes)
	};
public:

	/// �R���X�g���N�^
	CCopyDataPostMessage(void) : _super(), m_msg(0)
	{
	}

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B SetMessage() �����ݒ�̏ꍇ��A SetParamater() �Őݒ肵�����M�悪���݂��Ȃ��ꍇ�A���s���܂��B
	 */
	virtual bool Open(void) 
	{
		return m_msg != 0 && IsOpened();
	}
		
	/**
	 * [�ݒ�] ���b�Z�[�W�ݒ�
	 *	@param msg ����M�Ɏg�����b�Z�[�W�B
	 */
	void SetMessage(UINT msg)
	{
		m_msg = msg;
	}

	/**
	 * [�ݒ�] ���b�Z�[�W�ݒ�
	 *	@param msg ����M�Ɏg�����ʎq�B
	 */
	void SetMessage(LPCTSTR msg)
	{
		m_msg = ::RegisterWindowMessage(msg);
	}

	/**
	 * [����] ���M.
	 *	@param size �T�C�Y�B
	 *	@param P �f�[�^�B
	 *	@retval INVALID_SIZE �G���[�B
	 *	@retval ��L�ȊO ���M�f�[�^�T�C�Y�B
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( ! IsOpened() )
		{
			return INVALID_SIZE;
		}
		ULONG_PTR cookie = m_dwCookie & COOKIE_MASK;
		const BYTE* B = static_cast<const BYTE*>(P);
		INDEX pos = 0;
		loop ( i, size / 4 )
		{
			const DWORD* W = reinterpret_cast<const DWORD*>(&B[pos]);
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS4BYTE, *W);
			pos += 4;
		}
		switch ( size & 3 )
		{
		case 0:
		default:
			break;
		case 1:
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS1BYTE, B[pos]);
			break;
		case 2:
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS2BYTE, B[pos] | (B[pos + 1] << 8));
			break;
		case 3:
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS3BYTE, B[pos] | (B[pos + 1] << 8) | (B[pos + 2] << 16));
			break;
		}
		return size;
	}

	/**
	 * [����] ���b�Z�[�W���p.
	 *	@note �e�E�B���h�E�� WindowProc() ���烁�b�Z�[�W�𗬂��Ă��������B
	 *	@param[in] message �e�_�C�A���O����M�������b�Z�[�W
	 *	@param[in] wParam �e�_�C�A���O����M�������b�Z�[�W��WPARAM
	 *	@param[in] lParam �e�_�C�A���O����M�������b�Z�[�W��LPARAM
	 *	@param[out] _result �{�֐��� true �̍ہA���U���g�Ƃ��Ďg���l���i�[����B
	 *	@retval true �������{�B _result �Ƀ��U���g�l���i�[����Ă���B
	 *	@retval false �����Ȃ��������B
	 */
	bool RelayEvent(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& _result)
	{
		if ( message == m_msg )
		{
			if ( (wParam & COOKIE_MASK) == (m_dwCookie & COOKIE_MASK) )
			{
				int len = (ToInt(wParam & HASBYTE_MASK) >> HASBYTE_SHIFT) + 1;
				if ( len >= 1 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>(lParam & 0xFF));
				}
				if ( len >= 2 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>((lParam >> 8) & 0xFF));
				}
				if ( len >= 3 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>((lParam >> 16) & 0xFF));
				}
				if ( len >= 4 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>((lParam >> 24) & 0xFF));
				}
				return true;
			}
		}
		return false;
	}
};



}; // TNB

