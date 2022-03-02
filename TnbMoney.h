#pragma once
/**
 *@file
 * d‰İA†•¼ŠÖŒW‚Ìƒwƒbƒ_
 *
 *	@see TnbDef.h —˜—pğŒA‚¨‚æ‚Ñ–³•ÛØ‹K’è
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * d‰İA†•¼ŠÇ—.
 *		Šeíd‰İA†•¼‚Ì–‡”‚ğŠÇ—‚µ‚Ü‚·B
 *
 *	@param TYP —v‘f‚ÌŒ^B
 *
 *	@par•K—vƒtƒ@ƒCƒ‹
 *			TnbMoney.h
 *
 *	@date 16/04/15 V‹K
 *	@date 16/07/26 Initialize() A‰ÁZ‘ã“üƒƒ\ƒbƒh‚ğV‹K’Ç‰ÁB
 */
class CMoney
{
public:

	enum EKind
	{
		Coin1,			///< ˆê‰~‹Ê
		Coin5,			///< ŒÜ‰~‹Ê
		Coin10,			///< \‰~‹Ê
		Coin50,			///< ŒÜ\‰~‹Ê
		Coin100,		///< •S‰~‹Ê
		Coin500,		///< ŒÜ•S‰~‹Ê
		Bill1000,		///< ç‰~D
		Bill2000,		///< “ñç‰~D
		Bill5000,		///< ŒÜç‰~D
		Bill10000,		///< ˆê–œ‰~D
		KIND_MAX
	};

	/**
	 * ƒRƒ“ƒXƒgƒ‰ƒNƒ^.
	 *		‚O‚É‚È‚è‚Ü‚·B
	 */
	CMoney(void)
	{
		m_Init();
	}

	/**
	 * [İ’è] ‰Šú‰».
	 *		‹àŠz‚ğ‚O‚É‚µ‚Ü‚·B
	 */
	void Initialize(void)
	{
		m_Init();
	}

	/**
	 * [æ“¾] ŒÂX‹àí–‡”æ“¾
	 *	@param k ‹àí
	 *	@return –‡”
	 */
	DWORD Get(EKind k) const
	{
		return m_number[k];
	}

	/**
	 * [İ’è ŒÂX‹àí–‡”İ’è
	 *	@param k ‹àí
	 *	@param c –‡”
	 */
	void Set(EKind k, DWORD c)
	{
		m_number[k] = c;
	}

	/**
	 * [İ’è] ‘ã“ü
	 *	@param other ‘ã“üŒ³
	 */
	void Set(const CMoney& other)
	{
		memcpy(m_number, other.m_number, sizeof(m_number));
	}

	/**
	 * [İ’è] ‹àŠzİ’è.
	 *	@param[in] amount ‹àŠz.
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
	 * [‘ã“ü] ‰ÁZ‘ã“ü.
	 *	@param other ‰ÁZ‹àŠz.
	 *	@return ©•ª‚ÌQÆ
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
	 * [æ“¾] ‹àŠzæ“¾
	 *	@return ‹àŠz
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
	 * [Šm”F] x•¥‚¢‰Â”\H
	 *	@param[in] other x•¥Šzî•ñ
	 *	@retval true ‰Â”\.
	 *	@retval false •s‰Â”\
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
	 * [Šm”F] x•¥‚¢‰Â”\H
	 *	@param[in] amount x•¥‹àŠz
	 *	@retval true ‰Â”\.
	 *	@retval false •s‰Â”\
	 */
	bool CanMakePayment(DWORD amount) const
	{
		DWORD a = GetAmount();
		return (a >= amount);
	}

	/**
	 * [Šm”F] x•¥‚¢
	 *	@param other x•¥Šzî•ñ
	 *	@retval true ¬Œ÷B–{ƒIƒuƒWƒFƒNƒg“à‚Ìî•ñ‚ªx•¥‚¢Œã‚Ìî•ñ‚É‚È‚Á‚Ä‚¢‚Ü‚·B
	 *	@retval false ¸”s
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
	 * [Šm”F] x•¥‚¢‹àíŠm”F.
	 *		w’è‚Ì‹àŠz‚ğ•¥‚¤ê‡A‚»‚ê‚¼‚ê‚Ì‹àí‚ª‰½–‡‚É‚È‚é‚©Aƒ`ƒFƒbƒN‚·‚é
	 *	@param[out] _pay x•¥‚¢‰Â”\‚Ìê‡Ax•¥‚¢‹àíB‚¨‚Â‚è•K{‚Ìê‡A—v‹‚¨‚Â‚è‹àíB
	 *	@param[in] amount x•¥‚¢‹àŠz
	 *	@retval 0 x•¥‚¢‰Â”\.
	 *	@retval 1ˆÈã ‚¨‚Â‚è•K{i×‚©‚¢‚Ì‚ª–³‚­‚Ä•¥‚¦‚È‚¢j.
	 *	@retval -1 x•¥‚¢•s‰Â”\i‹àŠz‘«‚è‚È‚¢j
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
			// •¥‚¦‚é‚ªB‚µ‚©‚µ‚¨‚Â‚è‚ª•K—v
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
	 * [•\¦] ƒfƒoƒO—p•\¦.
	 */
	void Dump(void) const
	{
		TCHAR work[256];
		for ( int i = 0; i < KIND_MAX; i++ )
		{
			if ( m_number[i] > 0 )
			{
				wsprintf(work, _T("%d‰~%s = %dƒ–\n"), GetKindAmound(i), GetKindAmound(i) >= 1000 ? _T("D") : _T("‹Ê"), m_number[i]);
				OutputDebugString(work);
			}
		}
		wsprintf(work, _T("Œv%d‰~\n"), GetAmount());
		OutputDebugString(work);
	}

	/**
	 * ‹àí‹àŠzæ“¾.
	 *		MoneyKind ‚Æ‹àŠz‚ğŠÖ˜A•t‚¯‚é.
	 *	@param kind ‹àí
	 *	@return ‹àŠz
	 */
	static DWORD GetKindAmound(int kind)
	{
		static const DWORD aKind[KIND_MAX] =
		{
			1,			///< ˆê‰~‹Ê
			5,			///< ŒÜ‰~‹Ê
			10,			///< \‰~‹Ê
			50,			///< ŒÜ\‰~‹Ê
			100,		///< •S‰~‹Ê
			500,		///< ŒÜ•S‰~‹Ê
			1000,		///< ç‰~D
			2000,		///< “ñç‰~D
			5000,		///< ŒÜç‰~D
			10000,		///< ˆê–œ‰~D
		};
		ASSERT( aKind[Bill10000] == 10000 );
		return aKind[kind];
	}

private:
	void m_Init(void)
	{
		Zero(m_number);
	}

	DWORD	m_number[KIND_MAX];	///< Še‹àí‚²‚Æ‚Ì”
};



}; // TNB
