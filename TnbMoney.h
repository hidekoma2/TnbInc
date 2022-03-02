#pragma once
/**
 *@file
 * �d�݁A�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * �d�݁A�����Ǘ�.
 *		�e��d�݁A�����̖������Ǘ����܂��B
 *
 *	@param TYP �v�f�̌^�B
 *
 *	@par�K�v�t�@�C��
 *			TnbMoney.h
 *
 *	@date 16/04/15 �V�K
 *	@date 16/07/26 Initialize() �A���Z������\�b�h��V�K�ǉ��B
 */
class CMoney
{
public:

	enum EKind
	{
		Coin1,			///< ��~��
		Coin5,			///< �܉~��
		Coin10,			///< �\�~��
		Coin50,			///< �܏\�~��
		Coin100,		///< �S�~��
		Coin500,		///< �ܕS�~��
		Bill1000,		///< ��~�D
		Bill2000,		///< ���~�D
		Bill5000,		///< �ܐ�~�D
		Bill10000,		///< �ꖜ�~�D
		KIND_MAX
	};

	/**
	 * �R���X�g���N�^.
	 *		�O�ɂȂ�܂��B
	 */
	CMoney(void)
	{
		m_Init();
	}

	/**
	 * [�ݒ�] ������.
	 *		���z���O�ɂ��܂��B
	 */
	void Initialize(void)
	{
		m_Init();
	}

	/**
	 * [�擾] �X���햇���擾
	 *	@param k ����
	 *	@return ����
	 */
	DWORD Get(EKind k) const
	{
		return m_number[k];
	}

	/**
	 * [�ݒ� �X���햇���ݒ�
	 *	@param k ����
	 *	@param c ����
	 */
	void Set(EKind k, DWORD c)
	{
		m_number[k] = c;
	}

	/**
	 * [�ݒ�] ���
	 *	@param other �����
	 */
	void Set(const CMoney& other)
	{
		memcpy(m_number, other.m_number, sizeof(m_number));
	}

	/**
	 * [�ݒ�] ���z�ݒ�.
	 *	@param[in] amount ���z.
	 */
	void SetAmount(DWORD amount)
	{
		m_Init();
		for ( int i = KIND_MAX - 1; i >= 0; i-- )
		{
			DWORD r = amount / GetKindAmound(i);
			m_number[i] = r;
			amount -= GetKindAmound(i) * r;
		}
		ASSERT( amount == 0 );
	}

	/**
	 * [���] ���Z���.
	 *	@param other ���Z���z.
	 *	@return �����̎Q��
	 */
	CMoney& operator+=(const CMoney& other) 
	{
		for ( int i = 0; i < KIND_MAX; i++ )
		{
			m_number[i] += other.m_number[i];
		}
		return *this;
	}

	/**
	 * [�擾] ���z�擾
	 *	@return ���z
	 */
	DWORD GetAmount(void) const
	{
		DWORD r = 0;
		for ( int i = 0; i < KIND_MAX; i++ )
		{
			r += m_number[i] * GetKindAmound(i);
		}
		return r;
	}

	/**
	 * [�m�F] �x�����\�H
	 *	@param[in] other �x���z���
	 *	@retval true �\.
	 *	@retval false �s�\
	 */
	bool CanMakePayment(const CMoney& other) const
	{
		bool r = true;
		for ( int i = 0; i < KIND_MAX; i++ )
		{
			r &= (m_number[i] >= other.m_number[i]);
		}
		return r;
	}

	/**
	 * [�m�F] �x�����\�H
	 *	@param[in] amount �x�����z
	 *	@retval true �\.
	 *	@retval false �s�\
	 */
	bool CanMakePayment(DWORD amount) const
	{
		DWORD a = GetAmount();
		return (a >= amount);
	}

	/**
	 * [�m�F] �x����
	 *	@param other �x���z���
	 *	@retval true �����B�{�I�u�W�F�N�g���̏�񂪎x������̏��ɂȂ��Ă��܂��B
	 *	@retval false ���s
	*/
	bool MakePayment(const CMoney& other)
	{
		if ( CanMakePayment(other) )
		{
			for ( int i = 0; i < KIND_MAX; i++ )
			{
				ASSERT( m_number[i] >= other.m_number[i] );
				m_number[i] -= other.m_number[i];
			}
			return true;
		}
		return false;
	}

	/**
	 * [�m�F] �x��������m�F.
	 *		�w��̋��z�𕥂��ꍇ�A���ꂼ��̋��킪�����ɂȂ邩�A�`�F�b�N����
	 *	@param[out] _pay �x�����\�̏ꍇ�A�x��������B����K�{�̏ꍇ�A�v���������B
	 *	@param[in] amount �x�������z
	 *	@retval 0 �x�����\.
	 *	@retval 1�ȏ� ����K�{�i�ׂ����̂������ĕ����Ȃ��j.
	 *	@retval -1 �x�����s�\�i���z����Ȃ��j
	 */
	int CheckMakePaymentKind(CMoney& _pay, DWORD amount)
	{
		_pay.m_Init();
		if ( CanMakePayment(amount) )
		{
			loop_dn( i, KIND_MAX )
			{
				DWORD r = min(amount / GetKindAmound(i), m_number[i]);
				_pay.m_number[i] = r;
				amount -= GetKindAmound(i) * r;
			}
			if ( amount == 0 )
			{
				return 0;
			}
			// �����邪�B���������肪�K�v
			for ( int i = 0; i < KIND_MAX; i++ )
			{
				if ( GetKindAmound(i) >= amount && m_number[i] > 0 )
				{
					DWORD d = GetKindAmound(i) - amount;
					_pay.SetAmount(d);
					break;
				}
			}
			return 1;
		}
		return -1; 
	}

	/**
	 * [�\��] �f�o�O�p�\��.
	 */
	void Dump(void) const
	{
		TCHAR work[256];
		for ( int i = 0; i < KIND_MAX; i++ )
		{
			if ( m_number[i] > 0 )
			{
				wsprintf(work, _T("%d�~%s = %d��\n"), GetKindAmound(i), GetKindAmound(i) >= 1000 ? _T("�D") : _T("��"), m_number[i]);
				OutputDebugString(work);
			}
		}
		wsprintf(work, _T("�v%d�~\n"), GetAmount());
		OutputDebugString(work);
	}

	/**
	 * ������z�擾.
	 *		MoneyKind �Ƌ��z���֘A�t����.
	 *	@param kind ����
	 *	@return ���z
	 */
	static DWORD GetKindAmound(int kind)
	{
		static const DWORD aKind[KIND_MAX] =
		{
			1,			///< ��~��
			5,			///< �܉~��
			10,			///< �\�~��
			50,			///< �܏\�~��
			100,		///< �S�~��
			500,		///< �ܕS�~��
			1000,		///< ��~�D
			2000,		///< ���~�D
			5000,		///< �ܐ�~�D
			10000,		///< �ꖜ�~�D
		};
		ASSERT( aKind[Bill10000] == 10000 );
		return aKind[kind];
	}

private:
	void m_Init(void)
	{
		Zero(m_number);
	}

	DWORD	m_number[KIND_MAX];	///< �e���킲�Ƃ̐�
};



}; // TNB
