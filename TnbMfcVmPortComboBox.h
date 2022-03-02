#pragma once
/**
 *@file
 * ���z�}�V���V���A���|�[�g�I���R���{�{�b�N�X�֌W�̃w�b�_
 *		
 *		�f�o�O�A�J���p�ł��B���z�}�V���Ƃ̃V���A���ʐM�������Ȃ����߂̎d�g�݂�p�ӂ��܂��B�f�o�b�O�r���h�̍ۂ̂݁A
 *		�d�g�݂�����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcPortComboBox.h"
#include "TnbRs232c.h"
#include "TnbPipe.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ���z�|�[�g�t���A�V���A���|�[�gComboBox�R���g���[��.
 *	
 *		�f�o�O�A�J���p�ł��B CVmRs232c �Ƒ΂Ŏg�p���Ă��������B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃR���{�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCComboBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		�R���{�{�b�N�X�̃v���p�e�B�́A�h���b�v�_�E�����X�g�ɂ��Ă����Ă��������B
 */
class CVmPortComboBox : public CPortComboBox
{
	DEFSUPER(CPortComboBox);
public:

	/// �R���X�g���N�^
	CVmPortComboBox(void)
	{
		#ifdef _DEBUG
			AddExtendPort(_T("pipe\\vm"));
		#endif
	}

protected:

	/**
	 * [�ݒ�] �g���|�[�g���ݒ�.
	 *	@note �{�N���X�ł͎g�p����K�v������܂���B
	 *	@param lpszName �_�~�[
	 */
	void AddExtendPort(LPCTSTR lpszName)
	{
		_super::AddExtendPort(lpszName);
	}
};



}; // MFC



/**@ingroup COMMUNICATION
 * RS232C�A�N�Z�X�N���X
 *
 *		�f�o�O�A�J���p�ł��B CVmPortComboBox �Ƒ΂Ŏg�p���Ă��������B
 *
 *		�V���A���A�N�Z�X���s���N���X�ł��BPort�ԍ����O�ȊO�̎��A���ʂ̃V���A�����I�[�v�����܂����A
 *		Port���O�̏ꍇ�̓l�[���t���p�C�v�gvmware-serial-port�h���g�p����p�ɂȂ��Ă��܂��B
 *
 *		@note Virtual PC �̐ݒ�� COM1 �� "\\.\pipe\vmware-serial-port"�ƌ������O�t���p�C�v���w��
 *		���Ă������ƂŁA���z�}�V����� CVmRs232c �ƃz�X�g�}�V����� CVmRs232c �ƒʐM���o���܂��B
 */
class CVmRs232c : public ICommunication
{
public:

	/// �R���X�g���N�^
	CVmRs232c(void)
	{
		m_pComm = &m_rs232c;
	}

	/**
	 * [�ݒ�] �I�[�v���ݒ�.
	 *		RS232C�̐ݒ���s���܂��B�ʏ�A Open �̑O�Ɏg�p���܂��B
	 *	@note Open �O�Ɏ��s�����ꍇ�K���������܂��B
	 *	@note �I�[�v�����A�{���\�b�h��COM�|�[�g�i���o�[��ύX�����ꍇ�A�N���[�Y����܂��B
	 *	@note �I�[�v�����A�{���\�b�h���g�p�����ꍇ�A�G���[�ɂȂ�\��������܂��B
	 *	@param port COM�|�[�g�i���o�[�B ���l�ł�������ł��w��\�ł��B
	 *	@param dwBaudRate �]�����x[bps]
	 *	@param bByteSize �L�����N�^�r�b�g[bits]
	 *	@param eParity �p���e�B�i�����^��j
	 *	@param eStopBits �X�g�b�v�r�b�g[bits]
	 *	@retval 0	�����B
	 *	@retval 0�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD SetParameter(CRs232c::CPort port, DWORD dwBaudRate, BYTE bByteSize, CRs232c::EParity eParity, CRs232c::EStopBits eStopBits)
	{
		if ( port.GetPortName().Find(_T("\\.\\\\pipe\\")) == 0 || port.GetNumber() == 0 )
		{
			m_pComm = &m_pipe;
			m_pipe.SettingNamedPipe(_T("\\\\.\\pipe\\vmware-serial-port"), true);
			return 0;
		}
		m_pComm = &m_rs232c;
		return m_rs232c.SetParameter(port, dwBaudRate, bByteSize, eParity, eStopBits);
	}

	/**
	 * [�擾] ���\�擾.
	 *	@note ���m�ȏ��� Open��Ɏ擾�ł��܂��B
	 *	@return ���\���
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_pComm->GetProperty();
	}

	/**
	 * [�ݒ�] ��M�҂����Ԑݒ�.
	 *		Receive() ���s���A��M�ł���f�[�^���������ɑ҂��Ԃ�ݒ�ł��܂��B
	 *	@note ��������ƁA�v���p�e�B���ύX�����\��������܂��B
	 *	@param to �^�C���A�E�g���� (ms)�B 
	 *	@retval true �����B
	 *	@retval false ���s�B�T�|�[�g�ł��Ȃ��������ꂪ�Ԃ�܂��B
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return m_pComm->SetReceiveTimeout(to);
	}

	/**
	 * [����] �I�[�v��.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool Open(void)
	{
		return m_pComm->Open();
	}

	/**
	 * [����] �N���[�Y.
	 */
	virtual void Close(void)
	{
		m_pComm->Close();
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v�����B
	 *	@retval false �N���[�Y���B
	 */
	virtual bool IsOpened(void) const
	{
		return m_pComm->IsOpened();
	}

	/**
	 * [�m�F] �ڑ��m�F.
	 *	@retval true �ڑ����B
	 *	@retval false �ؒf���B
	 */
	virtual bool IsConnect(void) const
	{
		return m_pComm->IsConnect(); 
	}

	/**
	 * [�m�F] ���M�\�m�F.
	 *	@retval true ���M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanSend(void) const
	{ 
		return m_pComm->CanSend(); 
	}

	/**
	 * [�m�F] ��M�\�m�F.
	 *	@retval true ��M�\�B
	 *	@retval false �s�\�B
	 */
	virtual bool CanReceive(void) const
	{
		return m_pComm->CanReceive(); 
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
		return m_pComm->Send(size, P); 
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
		return m_pComm->Receive(size, _P); 
	}

	/**
	 * [�m�F] ��M�f�[�^�m�F.
	 *	@retval INVALID_SIZE �s���B
	 *	@retval ��L�ȊO ��M�\�f�[�^�T�C�Y�B
	 */
	virtual size_t GetReceivingDataSize(void) const 
	{
		return m_pComm->GetReceivingDataSize(); 
	}

	/**
	 * [�m�F] �`�F�b�N.
	 *		��Ԃ��m�F���܂��B
	 *	@retval OPENNING	�I�[�v�����
	 *	@retval CONNECTING	�I�[�v�����ڑ����
	 *	@retval CLOSED		�N���[�Y���	
	 */
	virtual EState Check(void) const 
	{
		return m_pComm->Check(); 
	}

	/**
	 * [����] �p�[�W.
	 *		�ʐM�̃S�~����
	 */
	virtual void Purge(void)
	{
		m_pComm->Purge(); 
	}

	/**
	 * [�Q��] RS232C�̃I�v�W�F�N�g�Q��
	 *	@return �Q��
	 */
	CRs232c& Rs232c(void)
	{
		return m_rs232c;
	}

	/**
	 * [�Q��] RS232C�̃I�v�W�F�N�g�Q��
	 *	@return �Q��
	 */
	CPipe& Pipe(void)
	{
		return m_pipe;
	}

private:
	ICommunication*	m_pComm;		///< �A�N�e�B�u�ȒʐM�I�u�W�F�N�g
	CRs232c			m_rs232c;		///< RS232C
	CPipe			m_pipe;			///< �p�C�v
};



}; //TNB
