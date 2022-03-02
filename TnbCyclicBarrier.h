#pragma once
/**
 *@file
 * �o���A�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup SYNC
 * �o���A�N���X
 *
 *		�w��̐��i�p�[�e�B���j�� Wait �ɓ���܂Ńo���A����i�i�s��j�ށj�N���X�ł��B
 *
 *	@note �p�[�e�B�����Q�Ȃ�΁A CExchanger ���g�p�����ق����A�y�ʂł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCyclicBarrier.h
 *
 *	@date 06/04/20 �V�K�쐬
 */
class CCyclicBarrier : CCopyImpossible
{
public:

	/**@ingroup SYNC
	 * �o���A�N���X�̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *		�萔��Wait������������̒ʒm�p�C���^�[�t�F�[�X�ł��B
	 *
	 *	@see CCyclicBarrier::CCyclicBarrier()
	 *	@see CCyclicBarrier::Init()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbCyclicBarrier.h
	 *
	 *	@date 06/04/20 �V�K�쐬
	 */
	struct IListener
	{
		/**�f�X�g���N�^*/
		virtual ~IListener(void) {}

		/** 
		 * [�ʒm] ��������ʒm
		 *
		 *		�萔��Wait����������Ƃ��Ɏ��s����܂��B
		 *		���̊֐����烊�^�[������܂ŁA���ꂼ���Wait()���烊�^�[������܂���B
		 *	@note ���̊֐������s�����X���b�h�͈�ԍŏ��� Wait()���ꂽ�X���b�h�ɂȂ�܂��B
		 *			�܂�A�s��ƍl���ď������������Ă��������B
		 */
		virtual void OnGathered(void) = 0;
	};

private:

	int				m_iParties;				///< �p�[�e�B��
	IListener*		m_piListener;			///< ���X�i�[
	CSyncEvent*		m_psyncEvent;			///< EVENT�z��p�|�C���^
	LONG			m_lWaitingCount;		///< Wait�J�n�X���b�h��
	CSyncSection	m_syncParam;			///< �p�����[�^�r���p
	CSyncSemaphore*	m_psyncSema;			///< Wait�r���p
	LONG			m_lWaitExecutingCount;	///< Wait�֐����s���J�E���^
	bool			m_boIsReset;			///< ���Z�b�g�t���O

public:

	/**
	 * �R���X�g���N�^
	 *	@param iParties �p�[�e�B�̐��B�ȗ�����ƂQ�ł��B
	 *	@param I ���X�i�[�B�ȗ��ł��܂��B
	 */
	explicit CCyclicBarrier(int iParties = 2, IListener* I = NULL)
	{
		m_psyncEvent = NULL;
		m_psyncSema	= NULL;
		m_iParties = 0;
		m_piListener = NULL;
		m_boIsReset	= false;
		m_lWaitingCount = 0;
		m_lWaitExecutingCount = 0;
		Init(iParties, I);
	}

	/**
	 * �f�X�g���N�^
	 *	@note �ҋ@����Wait()������ꍇ�A����������������\��������܂��B
	 */
	~CCyclicBarrier(void)
	{
		Reset();
		while ( m_lWaitExecutingCount != 0 )
		{
			::Sleep(10); 
		}
		if ( m_psyncSema != NULL )
		{
			delete m_psyncSema;
		}
		if ( m_psyncEvent != NULL )
		{
			delete[] m_psyncEvent;
		}
	}
	
	/**
	 * [�ݒ�] ������
	 *	@note �ҋ@����Wait()������ꍇ�A����������������\��������܂��B
	 *	@note �ҋ@����Wait()�� -1 �Ń��^�[�����܂��B
	 *	@param iParties �p�[�e�B�̐��i2�ȏ���w��)�B�ȗ�����ƂQ�ł��B
	 *	@param I ���X�i�[�B�ȗ��ł��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�iiParties �̒l���Q�������w�肳��Ă���j�B
	 */
	bool Init(int iParties = 2, IListener* I = NULL)
	{
		if ( iParties < 2 )
		{
			ASSERT0(false, "CCyclicBarrier::Init()", "�p�[�e�B�����ُ�ł��B");
			return false;
		}
		Reset();
		while ( m_lWaitExecutingCount != 0 )
		{
			::Sleep(10); 
		}
		if ( m_psyncSema != NULL )
		{
			delete m_psyncSema;
		}
		m_psyncSema = new CSyncSemaphore(NULL, iParties);
		//
		if ( m_iParties != iParties )
		{
			if ( m_psyncEvent != NULL )
			{
				delete[] m_psyncEvent;
			}
			m_psyncEvent = new CSyncEvent[iParties];
			m_iParties = iParties;
		}
		m_piListener = I;
		loop ( i, m_iParties )
		{
			m_psyncEvent[i].ToLock();
		}
		m_lWaitingCount = 0;
		return true;
	}

	/**
	 * [����] ���Z�b�g
	 *	@note Wait�������̃X���b�h������ꍇ�A����������������\��������܂��B
	 *	@note �ҋ@���� Wait()�� -1 �Ń��^�[�����܂��B
	 */
	void Reset(void)
	{
		m_boIsReset = true;
		for ( int i = m_iParties - 1; i >= 0; i-- )
		{
			m_psyncEvent[i].Unlock();
		}
	}

	/**
	 * [�r��] �E�F�C�g
	 *	@note ���ׂẴp�[�e�B��Wait���Ăяo���܂őҋ@���܂��B
	 *	@retval �O�ȏ� �����C���f�b�N�X
	 *	@retval �O���� �G���[
	 */
	int Wait(void)
	{
		::InterlockedIncrement(&m_lWaitExecutingCount);
		LONG lNo = 0;
		int iRc = 0;
		SYNCBLOCK(m_psyncSema)
		{
			SYNCBLOCK(&m_syncParam)
			{
				lNo = m_lWaitingCount++;
				if ( lNo == 0 )
				{
					//== TopWaiter
					m_boIsReset = false;
					loop ( i, m_iParties )
					{
						m_psyncEvent[i].ToLock();
					}
				}
			}
			//
			if ( lNo == 0 )
			{
				//== TopWaiter
				for ( int i = 1; i < m_iParties; i++ )
				{
					m_psyncEvent[i].Lock();	//����Waiter�҂�
				}
				if ( ! m_boIsReset )
				{
					EXCLUSIVE(&m_syncParam);
					if ( m_piListener != NULL )
					{
						m_piListener->OnGathered();
					}
					m_psyncEvent[0].Unlock();
					m_lWaitingCount = 0;
					iRc = 0;
				}
				else
				{
					iRc = -1;
				}
				break;
			}
			else if ( m_iParties > lNo )
			{
				//== �㑱��Waiter
				m_psyncEvent[lNo].Unlock();
				m_psyncEvent[0].Lock();	// TopWaiter�҂�
				iRc = (m_boIsReset) ? -1 : lNo;
				break;
			}
			else
			{
				ASSERTLIB(false);	//���Ȃ��͂�
			}
		} 
		::InterlockedDecrement(&m_lWaitExecutingCount);
		return iRc;
	}

	/**
	 * [�擾] �ҋ@��
	 *	@return Wait()�ő҂��Ă��鐔
	 */
	int GetNumberWaiting(void) { return m_lWaitingCount; }

	/**
	 * [�擾] �p�[�e�B��
	 *	@return �p�[�e�B�̐�
	 */
	int GetParties(void) { return m_iParties; }
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
