#pragma once
/**
 *@file
 * �e���|�����ʐM�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCommunication.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * �e���|�����ʐM�N���X
 *
 *		ICommunication ���������Ă��܂��B
 *		���M�Ŗ{�N���X�̕ێ�����Byte��؂�̃������Ƀv�[�����A��M�ł��̃���������ǂݏo���܂��B
 *
 *	@note ��� �ʐM���W���[���̒P�̃e�X�g�Ɏg�p����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTemporaryCommunicator.h
 * 
 *	@date 06/05/31 �V�K�쐬
 *	@date 06/06/08 ���O�ύX
 *	@date 06/07/07 InterfaceID�T�O�A�ǉ�
 *	@date 06/08/31 CommViewer �ɑΉ��B
 *	@date 06/09/09 IsAlive() / IsConnect() �ǉ��B CanAccess() �p�~�B
 *					ECE_Connect , ECE_Disconnect �C�x���g�A�ǉ��B
 *	@date 07/02/09 �e�N���X�ύX�B
 *	@date 07/05/10 �N���X���A�ύX�B
 *	@date 07/06/11 �e�N���X�� Start() ���\�b�h�ɑΉ��B
 *	@date 07/09/13 �e�N���X�ύX�B
 *	@date 08/12/08 ��M�^�C�~���O��ς�����悤�ɂ����B
 *	@date 10/05/28 SetReceiveTimeout() �������B
 *	@date 10/08/24 �\���ύX�B��M�^�C�~���O�����R���B
 *	@date 11/07/06 ���O�ύX�B SetSendErrorMode() �ǉ��B
 *	@date 11/07/07 �I�[�v����Ԃ�p��.
 *	@date 12/03/15 ReferSentRowData() ��V�K�B
 */
class CTemporaryCommunicator : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);
public:

	/// �R���X�g���N�^ 
	CTemporaryCommunicator(void) : m_property(_T("TemporaryCommunicator"), TNBVIW_KIND_TEMP), m_isOpened(false), m_isSendError(false)
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
	 * [�ݒ�] �p�P�b�g�T�C�Y�ݒ�.
	 *		���M�ł���T�C�Y���M�ł���T�C�Y���w��ł��܂��B���\�ɉe�����܂��B
	 *	@param receivePacketSize
	 *	@param sendPacketSize
	 */
	void SetPacketSize(DWORD receivePacketSize, DWORD sendPacketSize)
	{
		m_property.receivePacketSize = receivePacketSize;
		m_property.sendPacketSize = sendPacketSize;
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
	 * [����] �I�[�v��.
	 *	@return true
	 */
	virtual bool Open(void)
	{ 
		Purge();
		m_isOpened = true;
		return true;
	}
	
	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
		Purge();
		m_isOpened = false;
	}
	
	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@return true
	 */
	virtual bool IsOpened(void) const
	{
		return m_isOpened;
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
		if ( ! m_isOpened || m_isSendError )
		{
			return INVALID_SIZE;
		}
		CByteVector vb;
		vb.SetElements(size, static_cast<const BYTE*>(P));
		EXCLUSIVE( &m_sentData );
		m_sentRowData.Add(vb);
		m_sentData.Append(vb);
		return size;
	}

	/**
	 * [����] ��M.
	 *	@param[in] size �T�C�Y�B
	 *	@param[out] _P �f�[�^�Bsize ���̃������͊m�ۂ��Ă����K�v������܂��B
	 *	@return ��M�f�[�^�T�C�Y�B
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( m_isOpened )
		{
			INDEX idx = m_Find();
			if ( idx != INVALID_INDEX )
			{
				TParam& p = m_receiveReserves[idx];
				BYTE* B = static_cast<BYTE*>(_P);
				size_t l = min(p.data.GetSize(), size);
				MemCopy(B, p.data.ReferBuffer(), l);
				if ( l >= p.data.GetSize() )
				{
					m_receiveReserves.Remove(idx);
				}
				else
				{
					p.data.RemoveElements(0, l);
				}
				return l;
			}
		}
		return 0;
	}

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@return ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( m_isOpened )
		{
			INDEX idx = m_Find();
			if ( idx != INVALID_INDEX )
			{
				return m_receiveReserves[idx].data.GetSize();
			}
		}
		return 0;
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B
	 *	@return CONNECTING	�I�[�v�����ڑ����
	 *	@return CLOSED	�N���[�Y
	 */
	virtual EState Check(void) const 
	{ 
		return m_isOpened ? CONNECTING : CLOSED;
	}

	/**
	 * [����] �p�[�W.
	 */
	virtual void Purge(void)
	{
		EXCLUSIVE2( &m_receiveReserves, &m_sentData );
		m_receiveReserves.RemoveAll();
		m_sentData.RemoveAll();
		m_sentRowData.RemoveAll();
	}

	/**
	 * [����] ��M���ɒǉ�.
	 *		Receive() �Ŏ�M�����f�[�^��ǉ����܂��B
	 *	@param size �f�[�^�T�C�Y�B
	 *	@param pData �f�[�^�B
	 *	@param delay �x��(ms)�B��M�����܂ł̎��Ԃ��w��o���܂��B
	 */
	void AddReceivingData(size_t size, LPCVOID pData, DWORD delay = 0)
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( size > 0 )
		{
			TParam p;
			p.tick = ::GetTickCount();
			p.delay = delay;
			p.data.SetElements(size, static_cast<const BYTE*>(pData));
			m_receiveReserves.Add(p);
		}
	}

	/**
	 * [����] ��M���ɒǉ�.
	 *		Receive() �Ŏ�M�����f�[�^��ǉ����܂��B
	 *	@param data �f�[�^�B
	 *	@param delay �x��(ms)�B��M�����܂ł̎��Ԃ��w��o���܂��B
	 */
	void AddReceivingData(const IConstCollectionT<BYTE>& data, DWORD delay = 0)
	{
		EXCLUSIVE( &m_receiveReserves );
		if ( data.GetSize() > 0 )
		{
			TParam p;
			p.tick = ::GetTickCount();
			p.delay = delay;
			p.data.Copy(data);
			m_receiveReserves.Add(p);
		}
	}

	/**
	 * [�擾] ���M���Q��.
	 *		Send() �ő��M�����f�[�^���擾�ł��܂��B�f�[�^�͘A������Ă��܂��B
	 *	@note �ύX����ꍇ�A Lock ���čs���悤�ɂ��Ă��������B
	 *	@return �f�[�^�B
	 */
	CByteVector& ReferSentData(void)
	{
		return m_sentData;
	}

	/**
	 * [�擾] ���M���Q��.
	 *		Send() �ő��M�����f�[�^���擾�ł��܂��B
	 *	@note �ύX����ꍇ�A Lock ���čs���悤�ɂ��Ă��������B
	 *	@return �f�[�^�B
	 */
	CVectorT<CByteVector>& ReferSentRowData(void)
	{
		return m_sentRowData;
	}

	/**
	 * [�ݒ�] ���M�G���[���[�h�ݒ�.
	 *	@param isSendError true ���w�肷��ƁA Send() �� INVALID_SIZE ��Ԃ��悤�ɂȂ�܂��B
	 *						false �Ȃ� Send() �͐��퓮�삵�܂��B
	 */
	void SetSendErrorMode(bool isSendError)
	{
		m_isSendError = isSendError;
	}

private:

	INDEX m_Find(void) const
	{
		loop ( i, m_receiveReserves )
		{
			const TParam& p = m_receiveReserves[i];
			DWORD t = ::GetTickCount() - p.tick;
			if ( p.delay == 0 || p.delay <= t )
			{
				return i;
			}
		}
		return INVALID_INDEX;
	}

	
	/// �p�����[�^
	struct TParam
	{
		DWORD		tick;		///< �o�^����TICK
		DWORD		delay;		///< �x������
		CByteVector	data;		///< �f�[�^
	};
	CVectorT<TParam>		m_receiveReserves;	///< ��M�\��
	CByteVector				m_sentData;			///< ���M�����f�[�^�B
	CVectorT<CByteVector>	m_sentRowData;		///< ���M�����f�[�^�B
	_super::TProperty		m_property;			///< �v���p�e�B
	bool					m_isSendError;		///< ���M�̓G���[�H
	bool					m_isOpened;			///< �I�[�v����ԁH
};



}; // TNB

