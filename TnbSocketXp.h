#pragma once
/**
 *@file
 * �\�P�b�g�̃A�N�Z�X�֌W�̃w�b�_
 *
 *		�T�[�o�ƃN���C�A���g�̃N���X������܂��B
 *		��M�����́A���X�i�[��o�^���邱�Ƃōs���܂��B
 *		�T�[�o�̓}���`�N���C�A���g���T�|�[�g���Ă��܂��B
 *
 *	@todo IPv4 �̂ݑΉ��B IPv6 �͖��Ή��B
 *
 *	@note Winsock ���g�p���Ă��܂��B
 *
 *	@note	�}�N�� "_TnbWINSOCK2_ENABLE" �̒�`���K�{�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _TnbWINSOCK2_ENABLE
 #error
#endif

#include "TnbSocket.h"



//TNB Library
namespace TNB
{



/**@ingroup REPORT SOCKET
 * XP�ȏ�p Socket�T�|�[�g�N���X
 *	
 *		XP�ȏ�ŃT�|�[�g���Ă��� API ���Ǘ����Ă��܂��B
 * 
 *	@par�K�v�t�@�C��
 *			TnbSocketXp.h
 * 
 *	@date 10/02/03	�V�K�쐬
 */
class CSocketXp
{
public:

	/// �R���X�g���N�^
	CSocketXp(void) : m_isInit(true), m_lpfnTransmitPackets(NULL)
	{
	}

	/**
	 * [���M] ���M.
	 *	@param so �\�P�b�g
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	bool Send(SOCKET so, size_t dataSize, LPCVOID lpcvData)
	{
		if ( m_Init(so) )
		{
			TRANSMIT_PACKETS_ELEMENT packets;
			packets.dwElFlags = TP_ELEMENT_MEMORY;
			packets.cLength = dataSize;
			packets.pBuffer = const_cast<void* const>(lpcvData);
			return !! m_lpfnTransmitPackets(so, &packets, 1, 0, NULL, 0);
		}
		int r =  ::send(so, static_cast<const char*>(lpcvData), dataSize, 0);
		TRACE2("::send(%u) result = %d\n", dataSize, r);
		return r == ToInt(dataSize);
	}

private:
	bool					m_isInit;				///< �������t���O
	LPFN_TRANSMITPACKETS	m_lpfnTransmitPackets;	///< �֐��A�h���X
	/// ������
	bool m_Init(SOCKET so)
	{
		if ( ! m_isInit )
		{
			m_isInit = true;
			DWORD dwBytes;
			GUID guidTransmitPackets = WSAID_TRANSMITPACKETS;
			::WSAIoctl(so, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidTransmitPackets, sizeof(GUID), &m_lpfnTransmitPackets, sizeof(LPVOID), &dwBytes, NULL, NULL);
		}
		return m_lpfnTransmitPackets != NULL;
	}
};



/**@ingroup REPORT SOCKET
 * Socket�N���C�A���g�N���X
 *	
 *		�N���C�A���g�p�̃\�P�b�g��p�ӂ��܂��B
 *		����I��Server��T�������ڑ����[�h������܂��B
 *
 *		��M�� CAbstractSocket::IListener �̃T�u�N���X��o�^���s���܂��B
 *		( IReport::IListener �ł���M�͏o���܂�����{�@�\��������܂���)
 *
 *	@par�K�v�t�@�C��
 *			TnbSocket.h
 * 
 *	@date 10/02/03	�V�K�쐬
 */
class CSocketClientXp : public CSocketClient
{
	DEFSUPER(CSocketClient);
protected:
	
	/**
	 * ���M
	 *	@param so �\�P�b�g
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool m_Send(SOCKET so, size_t dataSize, LPCVOID lpcvData)
	{
		_super::m_OnSend(dataSize, lpcvData);
		if ( so == INVALID_SOCKET )
		{
			return false;
		}
		return m_xp.Send(so, dataSize, lpcvData);
	}
private:
	CSocketXp	m_xp;
};



/**@ingroup REPORT SOCKET
 * Socket�T�[�o�N���X
 *	
 *		�T�[�o�p�Ƀ\�P�b�g��p�ӂ��܂��B
 *
 *		��M�� CAbstractSocket::IListener �̃T�u�N���X��o�^���s���܂��B
 *		( IReport::IListener �ł���M�͏o���܂�����{�@�\��������܂���)
 *
 *	@note IReport::ITester ��o�^���Ă��A��M�e�X�g�͏o���܂���
 *			( OnCommTestReceive() �̓R�[������܂���j�B
 *
 *	@note	�A�C�h���C�x���g�́A�ڑ�����Ă���S�N���C�A���g����̎�M���Ȃ��Ȃ��� ��ms�o�������ɁA
 *			�S�ڑ��ɑ΂��āA�A�C�h���C�x���g���ʒm����܂��B�e�ڑ����ł͂Ȃ����Ƃɂ����ӂ��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbSocket.h
 * 
 *	@date 10/02/03	�V�K�쐬
 */
class CSocketServerXp : public CSocketServer
{
	DEFSUPER(CSocketServer);
protected:
	
	/**
	 * ���M
	 *	@param so �\�P�b�g
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param lpcvData �f�[�^
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool m_Send(SOCKET so, size_t dataSize, LPCVOID lpcvData)
	{
		_super::m_OnSend(dataSize, lpcvData);
		if ( so == INVALID_SOCKET )
		{
			return false;
		}
		return m_xp.Send(so, dataSize, lpcvData);
	}
private:
	CSocketXp	m_xp;
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #define CSocketClient CSocketClientXp
 #define CSocketServer CSocketServerXp
#endif



};