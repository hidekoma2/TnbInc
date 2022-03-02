#pragma once
/**
 *@file
 * ��b���σJ�E���^�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"
#include "TnbTickCount.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * ��b�ԕ��σJ�E���^�Ǘ��N���X
 *
 *		��b�ԂɎ��ۂ����񔭐����������J�E���g���邱�Ƃ��o���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbParSecondCounter.h
 *
 *	@date 09/08/26	�V�K�쐬
 */
class CParSecondCounter : CCopyImpossible
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@param second ���ς��Ƃ����(s)�B�ȗ������5�b�B
	 */
	CParSecondCounter(size_t second = 5)
	{
		Initialize(second);
	}

	/**
	 * [�ݒ�] ������.
	 *		�J�E���^�����������܂��B
	 *	@param second ���ς��Ƃ����(s)�B�ȗ������5�b�B
	 */
	void Initialize(size_t second = 5)
	{
		if ( second < 1 )
		{
			second = 1;
		}
		m_postCounts.Resize(second + 1);
		m_postCounts[0] = 0;
		m_validCount = 0;
		m_newestIndex = 0;
		m_nowTickSecond = 0;
		m_maxPostCount = 0;
		m_tick.Reset();
	}

	/**
	 * [�ݒ�] �|�X�g.
	 *		���ۂ����������ɂ�����R�[�����܂��B
	 *	@param c ���ې��B�ȗ�����ƂP���w�肳��܂��B
	 */
	void Post(DWORD c = 1)
	{
		EXCLUSIVE(&m_sync);
		DWORD t = m_tick.GetPassedCount() / 1000;
		if ( m_nowTickSecond == t )
		{
			m_postCounts[m_newestIndex] += c;
			return;
		}
		m_nowTickSecond = t;
		size_t count = m_postCounts.GetSize();
		if ( m_validCount < count )
		{
			m_validCount++;
		}
		if ( m_postCounts[m_newestIndex] > m_maxPostCount )
		{
			m_maxPostCount = m_postCounts[m_newestIndex];
		}
		m_newestIndex = (m_newestIndex + 1) % count;
		m_postCounts[m_newestIndex] = c;
	}

	/**
	 * [�擾] ���ώ擾.
	 *	@return ���ϐ�.
	 */
	double GetAverage(void) const
	{
		EXCLUSIVE(&m_sync);
		if ( m_validCount > 1 )
		{
			size_t count = m_postCounts.GetSize();
			DWORD r = 0;
			loop ( i, m_validCount - 1 )
			{
				r += m_postCounts[(count + count + m_newestIndex - i - 1) % count];
			}
			double a = r;
			return a / (m_validCount - 1);
		}
		return 0;
	}

	/**
	 * [�擾] �ő吔�擾.
	 *	@return �ő吔.
	 */
	DWORD GetMax(void) const
	{
		return m_maxPostCount;
	}

private:
	CWorkMemT<DWORD>	m_postCounts;		///< �|�X�g�������B�����b�B
	DWORD				m_validCount;		///< �L����
	DWORD				m_newestIndex;		///< ���݂̃C���f�b�N�X
	DWORD				m_nowTickSecond;	///< ���݂̕b
	DWORD				m_maxPostCount;		///< �ő吔
	CTickCount			m_tick;				///< TICK
	CSyncSection		m_sync;
};



};