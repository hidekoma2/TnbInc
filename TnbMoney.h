#pragma once
/**
 *@file
 * 硬貨、紙幣関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * 硬貨、紙幣管理.
 *		各種硬貨、紙幣の枚数を管理します。
 *
 *	@param TYP 要素の型。
 *
 *	@par必要ファイル
 *			TnbMoney.h
 *
 *	@date 16/04/15 新規
 *	@date 16/07/26 Initialize() 、加算代入メソッドを新規追加。
 */
class CMoney
{
public:

	enum EKind
	{
		Coin1,			///< 一円玉
		Coin5,			///< 五円玉
		Coin10,			///< 十円玉
		Coin50,			///< 五十円玉
		Coin100,		///< 百円玉
		Coin500,		///< 五百円玉
		Bill1000,		///< 千円札
		Bill2000,		///< 二千円札
		Bill5000,		///< 五千円札
		Bill10000,		///< 一万円札
		KIND_MAX
	};

	/**
	 * コンストラクタ.
	 *		０になります。
	 */
	CMoney(void)
	{
		m_Init();
	}

	/**
	 * [設定] 初期化.
	 *		金額を０にします。
	 */
	void Initialize(void)
	{
		m_Init();
	}

	/**
	 * [取得] 個々金種枚数取得
	 *	@param k 金種
	 *	@return 枚数
	 */
	DWORD Get(EKind k) const
	{
		return m_number[k];
	}

	/**
	 * [設定 個々金種枚数設定
	 *	@param k 金種
	 *	@param c 枚数
	 */
	void Set(EKind k, DWORD c)
	{
		m_number[k] = c;
	}

	/**
	 * [設定] 代入
	 *	@param other 代入元
	 */
	void Set(const CMoney& other)
	{
		memcpy(m_number, other.m_number, sizeof(m_number));
	}

	/**
	 * [設定] 金額設定.
	 *	@param[in] amount 金額.
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
	 * [代入] 加算代入.
	 *	@param other 加算金額.
	 *	@return 自分の参照
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
	 * [取得] 金額取得
	 *	@return 金額
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
	 * [確認] 支払い可能？
	 *	@param[in] other 支払額情報
	 *	@retval true 可能.
	 *	@retval false 不可能
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
	 * [確認] 支払い可能？
	 *	@param[in] amount 支払金額
	 *	@retval true 可能.
	 *	@retval false 不可能
	 */
	bool CanMakePayment(DWORD amount) const
	{
		DWORD a = GetAmount();
		return (a >= amount);
	}

	/**
	 * [確認] 支払い
	 *	@param other 支払額情報
	 *	@retval true 成功。本オブジェクト内の情報が支払い後の情報になっています。
	 *	@retval false 失敗
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
	 * [確認] 支払い金種確認.
	 *		指定の金額を払う場合、それぞれの金種が何枚になるか、チェックする
	 *	@param[out] _pay 支払い可能の場合、支払い金種。おつり必須の場合、要求おつり金種。
	 *	@param[in] amount 支払い金額
	 *	@retval 0 支払い可能.
	 *	@retval 1以上 おつり必須（細かいのが無くて払えない）.
	 *	@retval -1 支払い不可能（金額足りない）
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
			// 払えるが。しかしおつりが必要
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
	 * [表示] デバグ用表示.
	 */
	void Dump(void) const
	{
		TCHAR work[256];
		for ( int i = 0; i < KIND_MAX; i++ )
		{
			if ( m_number[i] > 0 )
			{
				wsprintf(work, _T("%d円%s = %dヶ\n"), GetKindAmound(i), GetKindAmound(i) >= 1000 ? _T("札") : _T("玉"), m_number[i]);
				OutputDebugString(work);
			}
		}
		wsprintf(work, _T("計%d円\n"), GetAmount());
		OutputDebugString(work);
	}

	/**
	 * 金種金額取得.
	 *		MoneyKind と金額を関連付ける.
	 *	@param kind 金種
	 *	@return 金額
	 */
	static DWORD GetKindAmound(int kind)
	{
		static const DWORD aKind[KIND_MAX] =
		{
			1,			///< 一円玉
			5,			///< 五円玉
			10,			///< 十円玉
			50,			///< 五十円玉
			100,		///< 百円玉
			500,		///< 五百円玉
			1000,		///< 千円札
			2000,		///< 二千円札
			5000,		///< 五千円札
			10000,		///< 一万円札
		};
		ASSERT( aKind[Bill10000] == 10000 );
		return aKind[kind];
	}

private:
	void m_Init(void)
	{
		Zero(m_number);
	}

	DWORD	m_number[KIND_MAX];	///< 各金種ごとの数
};



}; // TNB
