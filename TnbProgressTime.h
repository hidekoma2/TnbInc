#pragma once
/**
 *@file
 * �i�����Ԋ֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTickCount.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * �i�����ԊǗ��N���X
 *
 *		�S�̂̑傫�������A�����A�i����ʒm���邱�ƂŁA�����悻�̎c�莞�Ԃ��v�Z���܂��B
 *	
 *	@note	�����������Ԃ���A�S�̂̎��Ԃ�����o���A�c�莞�Ԃ������Ă��܂��̂ŁA
 *			�i���̐i�݂ɂ΂�������鎞�A���l�͂��Ȃ肠��ӂ�ɂȂ�܂��B
 *
 *
 *	@par�g�p��
 *		\code
 *		 	CProgressTime prog;
 *			CTickCount tt;
 *			loop ( i, 20 )
 *			{
 *				prog += 5;
 *				Sleep(500);
 *				TRACE2("%3d ; �c�� %f�b\n", i, prog.EstimateRemainingSecond());	
 *			}
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbProgressTime.h
 *
 *	@date 07/01/19	�V�K�쐬
 *	@date 11/03/07	�v�Z���@�����P�B���\�b�h���P�B
 */
class CProgressTime
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@param ri �i�݋�ă`�F�b�N�Ԋu(�b)�B�ȗ������5�b�ł��B
	 */
	CProgressTime(DWORD ri = 5) : m_recheckInverval(ri * 1000)
	  , m_total(0), m_pos(0), m_lastPos(0), m_parSec(0.0), m_lastParSec(0.0), m_previousResult(-1)
	{
	}

	/**
	 * [�J�n] �J�n.
	 *	@param total �S�̒l
	 */
	void Start(LONGLONG total = 100)
	{
		m_total = total;
		m_pos = 0;
		m_lastPos = 0;
		m_parSec = 0.0;
		m_lastParSec = 0.0;
		m_tick.Reset();
		m_lastTick.Reset();
		m_previousResult = -1;
		m_previousTick.Reset();
	}

	/**
	 * [�ݒ�] ���W���[��.
	 *	@note �����ԋ󂢂����ɍČv�Z�����邽�߂Ɏg�p���܂��B
	 */
	void Resume(void)
	{
		m_parSec = 0.0;
		m_lastParSec = 0.0;
		m_lastTick.Reset();
		m_previousResult = -1;
		m_previousTick.Reset();
	}

	/**
	 * [�ݒ�] �i��
	 *	@note �i���l���w�肵�܂��B
	 *	@param pos �i���l�B�ʏ�A�S�̒l�ȉ��̒l�ł��B
	 */
	void SetPos(LONGLONG pos)
	{
		m_pos = pos;
	}

	/**
	 * [�ݒ�] �i���A�S�̒l�ݒ�
	 *	@note �S�̒l���ύX����ꍇ�̂݁A������g�p���܂��B
	 *	@param pos �i���l�B�ʏ�A�S�̒l�ȉ��̒l�ł��B
	 *	@param total �S�̒l�B
	 */
	void Set(LONGLONG pos, LONGLONG total)
	{
		m_pos = pos;
		m_total = total;
	}

	/**
	 * [�ݒ�] �i��
	 *	@note �w����Z���A�i�����܂��B
	 *	@param d ���Z��
	 */
	void operator+=(LONGLONG d)
	{
		m_pos += d;
	}

	/**
	 * [�ݒ�] �i��
	 *	@note �{�P�A�i�����܂��B
	 */
	void operator++(void)
	{
		operator+=(1);
	}

	/**
	 * [�ݒ�] �i��
	 *	@note �{�P�A�i�����܂��B
	 */
	void operator++(int)
	{
		operator+=(1);
	}

	/**
	 * [�擾] �i���l�擾
	 *	@return �i���l
	 */
	LONGLONG GetPos(void) const
	{
		return m_pos;
	}

	/**
	 * [�擾] �S�̒l�擾
	 *	@return �S�̒l
	 */
	LONGLONG GetTotal(void) const
	{
		return m_total;
	}

	/**
	 * [�擾] �o�ߊ����擾
	 *	@return ����
	 */
	double GetRate(void) const
	{
		if ( m_total <= 0 )
		{
			return 0.0;
		}
		double p = double(m_pos) / double(m_total);
		if ( p > 1.0 )
		{
			p = 1.0;
		}
		return p;
	}

	/**
	 * [�v�Z] �c�莞�Ԍv�Z
	 *	@note Start���Ă���̎��ԂƁA�S�̒����ƌ��݂̐i������A�c�莞�Ԃ��T�Z���܂��B
	 *	@retval �O�ȏ� �T�Z�����c��b�B
	 *	@retval �}�C�i�X �v�Z���ȂǁA�T�Z�ł��Ȃ������B
	 */
	int EstimateRemainingSecond(void)
	{
		int pr = m_previousResult;
		int r = m_Calc();
		m_previousResult = r;
		if ( r <= 0 )
		{
			return r;
		}
		if ( pr < 0 || m_previousTick.IsPassedAndReset(1000) )
		{
			// �ȑO�̂����� or 1�b�ȏ�o�߂��Ă���B
			return r;
		}
		int rr = (r + pr) / 2;
		return rr;
	}

	/**
	 * [�v�Z] �c�莞�Ԍv�Z
	 *	@note Start���Ă���̎��ԂƁA�S�̒����ƌ��݂̐i������A�c�莞�Ԃ��T�Z���܂��B
	 *	@retval �O�ȏ� �T�Z�����c��~���b�B
	 *	@retval �}�C�i�X �v�Z���ȂǁA�T�Z�ł��Ȃ������B
	 *	@deprecated ���x�͕b�ł��B EstimateRemainingSecond() ���g�p���Ă��������B
	 */
	int _deprecated EstimateRemainingTime(void)
	{
		return EstimateRemainingSecond() * 1000;
	}

private:
	// �c�莞�Ԍv�Z
	int m_Calc(void)
	{
		if ( m_pos >= m_total )
		{
			return 0;
		}
		if ( m_pos == 0 )
		{
			return 0;
		}
		DWORD lp = m_lastTick.GetPassedCount();
		if ( lp >= m_recheckInverval )
		{
			//X�b������
			m_lastTick.Reset();
			double d = static_cast<double>(m_pos - m_lastPos);		//����X�b�ŕω�������
			d *= 1000.0;
			d /= lp;		//��b�Ԃő����鐔
			m_lastParSec = m_parSec;
			m_parSec = d;
			m_lastPos = m_pos;
		}
		if ( m_parSec > 0.0 )
		{
			double r = static_cast<double>(m_total - m_pos);	//�c��̐�
			if ( m_lastParSec > 0.0 )
			{
				r /= (m_parSec + m_lastParSec) / 2.0;
			}
			else
			{
				r /= m_parSec;
			}
			return int(r) + 1;
		}
		//
		DWORD passed = m_tick.GetPassedCount();
		UINT r = static_cast<UINT>(passed * m_total / m_pos - passed);
		if ( r < m_recheckInverval )
		{
			return r / 1000 + 1;
		}
		return -1;
	}

	CTickCount	m_tick;				///< �J�E���^
	LONGLONG	m_total;			///< �S�̒l
	LONGLONG	m_pos;				///< �i���l
	//
	CTickCount	m_lastTick;			///< ��O�̃J�E���^
	LONGLONG	m_lastPos;			///< ��O�̐i���l
	double		m_parSec;			///< ��b�Ԃɐi�񂾐�
	double		m_lastParSec;		///< ��O�̈�b�Ԃɐi�񂾐�
	DWORD		m_recheckInverval;	///< �i���Ń`�F�b�N�Ԋu(ms)
	//
	CTickCount	m_previousTick;		///< �Ō�̃J�E���^
	int			m_previousResult;	///< �Ō�̃��U���g
};



};//TNB

//
// 10���I��������A5�b��������
// �c���90���B���āA�㉽�b������H
//		�����A45�b�B
//	 10% : 5�b   100% : 50�b
//
// 50���I��������A30�b��������
// �c���50���B���āA�㉽�b������H
//		�����A30�b�B
//	 50% : 30�b   100% : 60�b
//
// Step2�G
// �ŋ߂�10�����I���̂�5�b��������5*100

