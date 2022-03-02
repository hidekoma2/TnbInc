#pragma once
/**
 *@file
 * �|�[�����O�֌W�̃w�b�_.
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup THREAD
 * �|�[�����O�g���K�[�N���X.
 *
 *		�w��̃|�[�g�C���^�[�t�F�[�X���Ď����A�ω������烊�X�i�[�ɒʒm���܂��B
 *
 *	@param TYP �f�[�^�̌^�B�ʏ�ABYTE WORD DWORD �̂����ꂩ���g���܂��B
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbPollingTrigger.h
 * 
 *	@date 09/02/17 �V�K�쐬
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 */
template<typename TYP = BYTE>
class CPollingTriggerT : public CThread::IRunner
{
public:

	/**@ingroup THREAD
	 * �|�[�����O�g���K�[�|�[�g�C���^�[�t�F�[�X.
	 *
	 *		CPollingTriggerT �Ńf�[�^���|�[�����O����|�[�g���������邽�߂Ɏg���܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbPollingTrigger.h
	 * 
	 *	@date 09/02/17 �V�K�쐬
	 */
	struct IPort
	{
		/// �f�X�g���N�^
		virtual ~IPort(void) {}
		/**
		 * [�擾] �|�[�g�f�[�^�擾
		 *	@param[out] _dat �擾�f�[�^
		 *	@retval 0 ����
		 *	@retval 0�ȊO ���s�B���X�i�[�� OnPollingError() �Œʒm����܂��B
		 */
		virtual DWORD GetPortData(TYP& _dat) = 0;
	};

	/**@ingroup THREAD
	 * �|�[�����O�g���K�[���X�i�[�C���^�[�t�F�[�X.
	 *
	 *		CPollingTriggerT �Ńf�[�^����M(�m��)���A�ʒm���邽�߂Ɏg���܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbPollingTrigger.h
	 * 
	 *	@date 09/02/17 �V�K�쐬
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}
		/**
		 * [�ʒm] �g���K�[�ω��ʒm
		 *	@param changeBit �ω�����BIT��1�ɁB
		 *	@param dataBit �ω����BIT�B
		 */
		virtual void OnPollingBitChanged(TYP changeBit, TYP dataBit) = 0;
		/**
		 * [�ʒm] �G���[�ʒm
		 *	@param error �G���[�R�[�h�B
		 */
		virtual void OnPollingError(DWORD error) = 0;
	};


	//---------------------------------------


	/// �R���X�g���N�^
	CPollingTriggerT(void) : m_pListener(NULL), m_interval(100), m_chatter(3), m_chatCount(0)
	{
	}

	/// �f�X�g���N�^
	virtual ~CPollingTriggerT(void)
	{
		m_thread.Stop();
	}

	/**
	 * [�ݒ�] �|�[�����O�J�n
	 *	@retval true ����
	 *	@retval false ���s.
	 */
	bool Start(void)
	{
		m_thread.Stop();
		m_thread.SetRunner(this);
		return m_thread.Start(_T("PollingTigger"));
	}

	/// [�ݒ�] �N���[�Y.
	void Stop(void)
	{
		m_thread.Stop();
	}

	/**
	 * [�ݒ�] �|�[�����O�ݒ�
	 *	@note �|�[�g�ƃ��X�i�[�Ɛݒ�Ԋu���w�肵�܂��B
	 *	@param P �|�[�g
	 *	@param L ���X�i�[
	 *	@param interval �`�F�b�N�Ԋu(ms)
	 *	@param chatter �m�肷�鐔�B
	 */
	void SetParameter(IPort* P, IListener* L, DWORD interval = 100, DWORD chatter = 3)
	{
		Stop();
		m_pPort = P;
		m_pListener = L;
		m_chatData.Resize(chatter);
		loop ( i, m_chatData.GetSize() )
		{
			m_chatData[i] = 0;
		}
		m_interval = interval;
		m_chatter = chatter;
		m_chatCount = chatter;
		m_fixData = 0;
	}

	/**
	 * [�Q��] �Ď��X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}

protected:
	/// �X���b�h
	virtual DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			::Sleep(m_interval);
			TYP b;
			DWORD r = m_pPort->GetPortData(b);
			if ( r != 0 )
			{
				if ( m_pListener != NULL )
				{
					m_pListener->OnPollingError(r);
				}
			}
			else
			{
				m_Input(b);
			}
		}
		return 0;
	}
	/**
	 * ��M����.
	 *	@note �`���^�����O����
	 *	@param b ���̓f�[�^
	 */
	void m_Input(TYP b)
	{
		if ( m_chatter > 1 )
		{
			TYP cf = 0;	// �ω��t���O �ŏI�I�� 0 �Ȃ�m��
			loop ( i, m_chatter - 1 )
			{
				TYP t = m_chatData[(m_chatCount - i) % m_chatter];
				cf |= TYP(t ^ b); //���ق̂���BIT�͂P�ɂȂ�B
			}
			TYP da = TYP((m_fixData & cf) | (b & ~cf));	//�m�肵�����̂��i�[�B����ȊO�͑O�̂܂�
			TYP ch = TYP(m_fixData ^ da);				// �ω������� BIT���P�ɂȂ�B
			m_fixData = da;
			if ( ch != 0 && m_pListener != NULL )
			{
				//TRACE2("  �ω�;[%02X] �m��;[%02X]\n", ch, da);
				m_pListener->OnPollingBitChanged(ch, da);
			}
			m_chatCount++;
			m_chatData[m_chatCount % m_chatter] = b;
		}
		else
		{
			TYP ch = TYP(m_fixData ^ b);				// �ω������� BIT���P�ɂȂ�B
			m_fixData = b;
			if ( ch != 0 && m_pListener != NULL )
			{
				m_pListener->OnPollingBitChanged(ch, b);
			}
		}
	}

private:
	CThread				m_thread;		///< �X���b�h
	IPort*				m_pPort;		///< �|�[�g
	IListener*			m_pListener;	///< ���X�i�[
	DWORD				m_interval;		///< �C���^�[�o��
	DWORD				m_chatter;		///< �`���^�m�萔
	CWorkMemT<TYP>		m_chatData;		///< �`���^�p�f�[�^
	DWORD				m_chatCount;	///< �`���^�p�J�E���g
	TYP					m_fixData;		///< �`���^�㌈��l
	friend class CPollingTriggerTest;
};



/**@ingroup THREAD
 * �|�[�����O�g���K�[�N���X.
 *
 *		�w��̃|�[�g�C���^�[�t�F�[�X���Ď����A�ω������烊�X�i�[�ɒʒm���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPollingTrigger.h
 * 
 *	@date 09/02/17 �V�K�쐬
 */
typedef CPollingTriggerT<BYTE> CPollingTrigger;



}; //TNB


