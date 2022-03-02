#pragma once
/**
 *@file
 * �X���b�h�Ԓl����ւ��֌W�̃w�b�_
 *
 *		�X���b�h�ԂŔC�ӂ̌^���m�̓���ւ����s�Ȃ��N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"



namespace TNB
{



/**@ingroup SYNC
 * �X���b�h�Ԓl����ւ��e���v���[�g�N���X.
 *
 *		�X���b�h�ԂŔC�ӂ̌^���m�̓���ւ����e�Ղɂł��܂��B
 *
 *	@note TYP �� BYTE �Ȃǂɂ��āA�P���ɓ�̃X���b�h�̓�������邱�Ƃɂ��g�p�\�ł��B
 *
 *	@param TYP �v�f�̌^�B�R�s�[�@�\�������Ă���K�v������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbExchanger.h
 *
 *	@date 06/09/19 �V�K
 */
template<typename TYP>
class CExchangerT
{
	LONG			m_lCount;		///< �i���J�E���^
	CSyncSemaphore	m_syncSema;		///< �i���r��
	CSyncEvent		m_syncEvent1;	///< �u���b�N�r���P
	CSyncEvent		m_syncEvent2;	///< �u���b�N�r���Q
	volatile TYP	m_tParam1st;	///< �����p�����[�^�l�P
	volatile TYP	m_tParam2nd;	///< �����p�����[�^�l�Q

public:

	/// �R���X�g���N�^
	CExchangerT(void)
		: m_lCount(0), m_syncSema(NULL, 2), m_syncEvent1(false, true), m_syncEvent2(false, true)
	{
	}

	/**
	 * [�擾] �l����.
	 *	@note	�ʃX���b�h�ŁA���I�u�W�F�N�g�� �{���\�b�h���R�[�������܂ŁA�u���b�N����܂��B
	 *			�ʃX���b�h�ŃR�[�������ƁA�����Ŏw�肵���l���{���\�b�h�̖߂�l�ɂȂ�܂��B
	 *	@param t �l�B
	 *	@return ���������l�B
	 */
	TYP Exchange(const TYP& t)
	{
		EXCLUSIVE(&m_syncSema);
		TYP r;
		LONG l = ::InterlockedIncrement(&m_lCount);
		if ( l == 1 )
		{
			m_tParam1st = t;
			m_syncEvent2.Lock();
			m_syncEvent1.Unlock();
			r = m_tParam2nd;
		}
		else
		{
			m_lCount = 0;
			m_tParam2nd = t;
			m_syncEvent2.Unlock();
			m_syncEvent1.Lock();
			r = m_tParam1st;
		}
		return r;
	}
};



}; // TNB
