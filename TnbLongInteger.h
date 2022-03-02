#pragma once
/**
 *@file
 * 多倍長整数関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbComparable.h"
#include "TnbSerializer.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * 多倍長整数管理
 *
 *		64bit 以上の整数を扱うことが出来ます。
 *
 *	@note メモリには、リトルエンディアンで格納されています。
 *
 *	@param PLACE 桁数。２以上を指定できます。 PLACE x 32 bit までの値を扱えます。
 *
 *	@note 最小値は内部管理の関係で、最大値 に -1 をかけた値になります。
 *
 *	@note PLACE の異なるクラス間では、計算できません。
 *
 *	@par必要ファイル
 *			TnbLongInteger.h
 *
 *	@date 09/10/28 新規
 */
template<size_t PLACE = 8>
class CLongIntegerT : public IComparableT< TNB::CLongIntegerT<PLACE> >, public ISerializable
{
public:
	
	/**
	 * コンストラクタ.
	 *	@note 保持している値は 0 になります。
	 */
	CLongIntegerT(void)
	{
		Zero();
	}

	/**
	 * コピーコンストラクタ.
	 *	@param other コピー元.
	 */
	CLongIntegerT(const CLongIntegerT& other)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] = other.m_num[i];
		}
	}

	/**
	 * 代入コンストラクタ.
	 *	@param val 初期値.
	 */
	CLongIntegerT(LONGLONG val)
	{
		operator=(val);
	}

	/**
	 * [代入] ゼロ代入.
	 *	@note 保持している値は 0 になります。
	 */
	void Zero(void)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] = 0;
		}
	}

	/**
	 * [代入] 値代入.
	 *		文字列で、値を代入します。
	 *	@note 文字列の頭に空白はつけないでください。
	 *	\code
	 *		CLongIntegerT<4> val;
	 *		val.SetString("-123456789");
	 *	\endcode
	 *	@param lpsz 文字列.
	 *	@return 使用した数.
	 */
	size_t SetString(LPCTSTR lpsz)
	{
		Zero();
		CStr str = lpsz;
		size_t len = str.GetLength();
		bool isMinus = false;
		loop ( i, len )
		{
			if ( i == 0 && str[i] == '-' )
			{
				isMinus = true;
			}
			else if ( str[i] < '0' || str[i] > '9' )
			{
				len = i;
				break;
			}
		}
		size_t ret = len;
		if ( isMinus )
		{
			len--;
			str = str.Mid(1, len);
		}
		else
		{
			str = str.Left(len);
		}
		INDEX cnt = 0;
		while ( len > 0 )
		{
			CStr s = str.Right(9);
			CLongIntegerT x(s.ToInt());
			loop ( i, cnt )
			{
				x *= INT_VALUE_MAX;
			}
			operator+=(x);
			len -= s.GetLength();
			str = str.Left(len);
			cnt++;
		}
		if ( isMinus )
		{
			*this = operator-();
		}
		return ret;
	}

	/**
	 * [代入] 値代入.
	 *		HEX文字列で、値を代入します。
	 *	@note マイナス指定は出来ません。
	 *	@note 文字列の頭に '0x' をつけないでください。
	 *	@note 文字列の頭に頭に空白はつけないでください。
	 *	\code
	 *		CLongIntegerT<4> val;
	 *		val.SetHexString("8000000080e412ca");
	 *	\endcode
	 *	@param lpsz 文字列.
	 *	@return 使用した数.
	 */
	size_t SetHexString(LPCTSTR lpsz)
	{
		Zero();
		size_t len = STRLIB::GetLen(lpsz);
		loop ( i, len )
		{
			if ( STRLIB::HexCharToInt(lpsz[i]) < 0 )
			{
				len = i;
				break;
			}
		}
		loop( i, len )
		{
			size_t j = i / 8;
			if ( j >= DWORD_LEN )
			{
				break;
			}
			int c = STRLIB::HexCharToInt(lpsz[len - 1 - i]);
			DWORD k = (c & 0xFF) << ((i & 7) * 4);
			m_num[j] |= k;
		}
		return len;
	}

	/**
	 * [設定] 生データ設定.
	 *		BYTEの配列を数値として設定します。
	 *	@note 本クラスはリトルエンディアンで格納されています。
	 *	@note サイズは byte 指定可能ですが、通常、 GetBytesCount() で得られる値と同じ長さにします。
	 *			短い場合、上位は０になります。
	 *	@param size 生データサイズ.
	 *	@param lpRawData 生データ.
	 */
	void SetRawData(size_t size, LPCVOID lpRawData)
	{
		Zero();
		if ( size > BYTE_LEN )
		{
			size = BYTE_LEN;
		}
		const BYTE* P1 = static_cast<const BYTE*>(lpRawData);
		BYTE* P2 = reinterpret_cast<BYTE*>(m_num);
		loop ( i, size )
		{
			*P2++ = *P1++;
		}
	}

	/**
	 * [取得] 生データ取得.
	 *		管理している数値をBYTEの配列として取得します。
	 *	@note 取得サイズは、 GetBytesCount() で得られる値と同じ長さになります。
	 *	@param[out] _rawData 生データが格納されます。
	 */
	void GetRawData(CWorkMem& _rawData) const
	{
		_rawData.Resize(BYTE_LEN);
		const BYTE* P1 = reinterpret_cast<const BYTE*>(m_num);
		BYTE* P2 = _rawData.Ref();
		loop ( i, BYTE_LEN )
		{
			*P2++ = *P1++;
		}
	}

	/**
	 * [参照] 生データ参照.
	 *		管理している数値をBYTEの配列として参照します。
	 *	@note サイズは、 GetBytesCount() で得られる値です。
	 *	@return 生データアドレス。
	 */
	const BYTE* RefRawData(void) const
	{
		return reinterpret_cast<const BYTE*>(m_num);
	}

	/**
	 * [取得] ビット取得.
	 *	@param bitNo ビットNo。 ０～ GetBitsCount() 未満を指定できます。
	 *	@retval true 指定ビットは１。
	 *	@retval false 指定ビットは０。
	 */
	bool GetBit(INDEX bitNo) const
	{
		size_t l = bitNo / 32;
		if ( l < DWORD_LEN )
		{
			return (m_num[l] & _BIT(bitNo % 32)) != 0;
		}
		return false;
	}

	/**
	 * [設定] ビット設定.
	 *	@param bitNo ビットNo。 ０～ GetBitsCount() 未満を指定できます。
	 *	@param v ビット状態
	 */
	void SetBit(INDEX bitNo, bool v = true)
	{
		size_t l = bitNo / 32;
		if ( l < DWORD_LEN )
		{
			if ( v )
			{
				m_num[l] |= _BIT(bitNo % 32);
			}
			else
			{
				m_num[l] &= ~_BIT(bitNo % 32);
			}
		}
	}

	/**
	 * コピーオペレータ.
	 *	@param other コピー元.
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator=(const CLongIntegerT& other)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] = other.m_num[i];
		}
		return *this;
	}

	/**
	 * 代入オペレータ.
	 *	@param val 値.
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator=(LONGLONG val)
	{
		LONGLONG* P =reinterpret_cast<LONGLONG*>(m_num);
		*P = val;
		DWORD u = (val < 0) ? DWORD_MAX : 0;
		for ( size_t i = 2 ; i < DWORD_LEN; i++ )
		{
			m_num[i] = u;
		}
		return *this;
	}

	/**
	 * [計算] プラス符号.
	 *		そのままの値を返します。
	 *	@return 値.
	 */
	CLongIntegerT operator+(void) const
	{
		return *this;
	}

	/**
	 * [計算] マイナス符号.
	 *		保持している値に -1 をかけた値を返します。
	 *	@return 符号反転値.
	 */
	CLongIntegerT operator-(void) const
	{
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = ~m_num[i];
		}
		if ( x.m_num[0] == DWORD_MAX )
		{
			//桁上がり
			x.m_num[0] = 0;
			for ( size_t i = 1; i < DWORD_LEN; i++ )
			{
				if ( x.m_num[i] == DWORD_MAX )
				{
					x.m_num[i] = 0;
				}
				else
				{
					x.m_num[i]++;
					break;
				}
			}
		}
		else
		{
			x.m_num[0]++;
		}
		return x;
	}

	/**
	 * [計算] ビット反転.
	 *	@return ビット反転した値.
	 */
	CLongIntegerT operator~(void) const
	{
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = ~m_num[i];
		}
		return x;
	}

	/**
	 * [代入] 加算代入.
	 *		保持している値に指定の値を加え、それを保持します。
	 *	@param val 加算値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator+=(const CLongIntegerT& val)
	{
		return operator-=(-val);
	}

	/**
	 * [代入] 減算代入.
	 *		保持している値に指定の値を引き、それを保持します。
	 *	@param val 減算値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator-=(const CLongIntegerT& val)
	{
		if ( this != &val )
		{
			loop ( i, DWORD_LEN )
			{
				bool isBorrow = true;
				if ( m_num[i] >= val.m_num[i] )
				{
					isBorrow = false;
				}
				m_num[i] -= val.m_num[i];
				size_t j = i + 1;
				while ( isBorrow && j < DWORD_LEN )
				{
					if ( m_num[j] >= 1 )
					{
						isBorrow = false;
					}
					m_num[j]--;
					j++;
				}
			}
		}
		else 
		{
			Zero();
		}
		return *this;
	}

	/**
	 * [代入] 乗算代入.
	 *		保持している値に指定の値をかけ、それを保持します。
	 *	@param val 乗算値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator*=(const CLongIntegerT& val)
	{
		*this = operator*(*this, val);
		return *this;
	}

	/**
	 * [代入] 除算代入.
	 *		保持している値を指定の値で割り、それを保持します。
	 *	@param val 除算値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator/=(const CLongIntegerT& val)
	{
		*this = operator/(*this, val);
		return *this;
	}

	/**
	 * [代入] 余り算代入.
	 *		保持している値を指定の値で割り、余りの値を保持します。
	 *	@param val 除算値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator%=(const CLongIntegerT& val)
	{
		*this = operator%(*this, val);
		return *this;
	}

	/**
	 * [代入] AND 算代入.
	 *		保持している値と指定の値と AND し、それを保持します。
	 *	@param val 値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator&=(const CLongIntegerT& val)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] &= val.m_num[i];
		}
		return *this;
	}

	/**
	 * [代入] OR 算代入.
	 *		保持している値と指定の値と OR し、それを保持します。
	 *	@param val 値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator|=(const CLongIntegerT& val)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] |= val.m_num[i];
		}
		return *this;
	}

	/**
	 * [代入] XOR 算代入.
	 *		保持している値と指定の値と XOR し、それを保持します。
	 *	@param val 値
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator^=(const CLongIntegerT& val)
	{
		loop ( i, DWORD_LEN )
		{
			m_num[i] ^= val.m_num[i];
		}
		return *this;
	}

	/**
	 * [代入] 左シフト代入.
	 *		保持している値を指定の数だけ左シフトした値を保持します。
	 *	@param val シフト数
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator<<=(size_t val)
	{
		while ( val > 0 )
		{
			if ( val >= 32 )
			{
				for ( int i = DWORD_LEN - 1; i > 0; i-- )
				{
					m_num[i] = m_num[i - 1];
				}
				val -= 32;
				m_num[0] = 0;
			}
			else 
			{
				for ( int i = DWORD_LEN - 1; i > 0; i-- )
				{
					m_num[i] = (m_num[i] << 1) | ((m_num[i - 1] & 0x80000000) ? 1 : 0);
				}
				m_num[0] <<= 1;
				val--;
			}
		}
		return *this;
	}

	/**
	 * [代入] 右シフト代入.
	 *		保持している値を指定の数だけ右シフトした値を保持します。
	 *	@param val シフト数
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator>>=(size_t val)
	{
		while ( val > 0 )
		{
			if ( val >= 32 )
			{
				loop ( i, DWORD_LEN - 1 )
				{
					m_num[i] = m_num[i + 1];
				}
				m_num[DWORD_LEN - 1] = 0;
				val -= 32;
			}
			else 
			{
				loop ( i, DWORD_LEN - 1 )
				{
					m_num[i] = (m_num[i] >> 1) | ((m_num[i + 1] & 1) ? 0x80000000 : 0);
				}
				m_num[DWORD_LEN - 1] >>= 1;
				val--;
			}
		}
		return *this;
	}

	/**
	 * [計算] 左シフト.
	 *		保持している値を指定の数だけ左シフトした値を返します。
	 *	@param val シフト数
	 *	@return 値.
	 */
	CLongIntegerT operator<<(size_t val) const
	{
		CLongIntegerT x = *this;
		x.operator<<=(val);
		return x;
	}

	/**
	 * [計算] 右シフト.
	 *		保持している値を指定の数だけ右シフトした値を返します。
	 *	@param val シフト数
	 *	@return 値.
	 */
	CLongIntegerT operator>>(size_t val) const
	{
		CLongIntegerT x = *this;
		x.operator>>=(val);
		return x;
	}

	/**
	 * [代入] デクリメント.
	 *		保持している値を -1 します。
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator--(void)
	{
		m_num[0]--;
		if ( m_num[0] == DWORD_MAX )
		{
			for ( size_t i = 1; i < DWORD_LEN; i++ )
			{
				m_num[i]--;
				if ( m_num[i] != DWORD_MAX )
				{
					break;
				}
			}
		}
		return *this;
	}

	/**
	 * [代入] デクリメント.
	 *		保持している値を返し、保持している値を -1 します。
	 *	@return 計算前の値。
	 */
	CLongIntegerT operator--(int)
	{
		CLongIntegerT x = *this;
		operator--();
		return x;
	}

	/**
	 * [代入] インクリメント.
	 *		保持している値を +1 します。
	 *	@return 自分の参照.
	 */
	CLongIntegerT& operator++(void)
	{
		m_num[0]++;
		if ( m_num[0] == 0 )
		{
			for ( size_t i = 1; i < DWORD_LEN; i++ )
			{
				m_num[i]++;
				if ( m_num[i] != 0 )
				{
					break;
				}
			}
		}
		return *this;
	}

	/**
	 * [代入] インクリメント.
	 *		保持している値を返し、保持している値を +1 します。
	 *	@return 計算前の値。
	 */
	CLongIntegerT operator++(int)
	{
		CLongIntegerT x = *this;
		operator++();
		return x;
	}

	/**
	 * [計算] 加算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a + b の値。
	 */
	friend CLongIntegerT operator+(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator+=(b);
		return x;
	}

	/**
	 * [計算] 減算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a - b の値。
	 */
	friend CLongIntegerT operator-(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator-=(b);
		return x;
	}

	/**
	 * [計算] 乗算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a × b の値。
	 */
	friend CLongIntegerT operator*(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CWorkMemT<DWORD> w(DWORD_LEN * 2);
		w.Fill(0);
		const BYTE* pV1 = reinterpret_cast<const BYTE*>(a.m_num);
		const BYTE* pV2 = reinterpret_cast<const BYTE*>(b.m_num);
		BYTE* pWork = reinterpret_cast<BYTE*>(w.Ref());
		loop ( i, BYTE_LEN )
		{
			if ( pV2[i] != 0 )
			{
				loop ( j, BYTE_LEN )
				{
					if ( pV1[j] != 0 )
					{
						size_t ix = i + j;
						BYTE* P = &pWork[ix];
						DWORD kk = pV1[j] * pV2[i] + *P;
						*P++ = static_cast<BYTE>(kk & 0xFF);
						ix++;
						kk /= 0x100;
						while ( (kk != 0) && (ix < (BYTE_LEN * 2)) )
						{
							kk += *P;
							*P++ = static_cast<BYTE>(kk & 0xFF);
							ix++;
							kk /= 0x100;
						}
					}
				}
			}
		}
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = w[i];
		}
		return x;
	}

	/**
	 * [計算] 除算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a ÷ b の商。
	 */
	friend CLongIntegerT operator/(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT quotient;
		CLongIntegerT remainder;
		m_Divide(a, b, quotient, remainder);
		return quotient;
	}

	/**
	 * [計算] 余り算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a ÷ b の余り。
	 */
	friend CLongIntegerT operator%(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT quotient;
		CLongIntegerT remainder;
		m_Divide(a, b, quotient, remainder);
		return remainder;
	}

	/**
	 * [計算] AND 算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a and b の値。
	 */
	friend CLongIntegerT operator&(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator&=(b);
		return x;
	}

	/**
	 * [計算] OR 算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a or b の値。
	 */
	friend CLongIntegerT operator|(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator|=(b);
		return x;
	}

	/**
	 * [計算] XOR 算.
	 *	@param a 値１
	 *	@param b 値２
	 *	@return a xor b の値。
	 */
	friend CLongIntegerT operator^(const CLongIntegerT& a, const CLongIntegerT& b)
	{
		CLongIntegerT x = a;
		x.operator^=(b);
		return x;
	}

	/**
	 * [計算] 絶対値.
	 *	@param val 値.
	 *	@return val の絶対値。
	 */
	friend CLongIntegerT abs(const CLongIntegerT& val)
	{
		if ( val.IsMinus() )
		{
			return -val;
		}
		return val;
	}

	/**
	 * [計算] 除算.
	 *		保持している値を指定の除算値で割り、結果を返します。
	 *	@param[out] _quotient 計算結果の商。
	 *	@param[out] _remainder 計算結果の余り。
	 *	@param[in] val 除算値.
	 *	@retval true 成功.
	 *	@retval false 失敗（0割エラー）.
	 */
	bool Divide(CLongIntegerT& _quotient, CLongIntegerT& _remainder, const CLongIntegerT& val) const
	{
		return m_Divide(*this, val, _quotient, _remainder);
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CLongIntegerT<PLACE>& t) const
	{
		INT_PTR sign1 = IsMinus() ? -1 : 1;
		INT_PTR sign2 = t.IsMinus() ? -1 : 1;
		if ( sign1 == sign2 )
		{
			for ( int i = DWORD_LEN - 1; i >= 0; i-- )
			{
				if ( m_num[i] > t.m_num[i] )
				{
					return sign1;
				}
				else if ( m_num[i] < t.m_num[i] )
				{
					return -sign1;
				}
			}
		}
		else 
		{
			return sign1;
		}
		return 0;
	}

	/**
	 * [確認] マイナス確認.
	 *		保持している値の確認をします。
	 *	@retval true マイナス（０未満）。
	 *	@retval false プラス（０以上）。
	 */
	bool IsMinus(void) const
	{
		return (m_num[DWORD_LEN - 1] & 0x80000000) != 0;
	}

	/**
	 * [確認] ゼロ確認.
	 *		保持している値の確認をします。
	 *	@retval true ゼロ。
	 *	@retval false ゼロ以外
	 */
	bool IsZero(void) const
	{
		loop ( i, DWORD_LEN )
		{
			if ( m_num[i] != 0 )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [確認] 保持値確認.
	 *		保持している値が int で表現可能か、確認をします。
	 *	@retval true 表現可能。
	 *	@retval false 不可能。
	 */
	bool CanExpressInt(void) const
	{
		return m_CanExpress(1);
	}

	/**
	 * [取得] int 取得.
	 *	@note CanExpressInt() の結果が true でない場合、取得できる値は不定です。
	 *	@return 値
	 */
	int ToInt(void) const
	{
		return m_num[0];
	}

	/**
	 * [確認] 保持値確認.
	 *		保持している値が LONGLONG で表現可能か、確認をします。
	 *	@retval true 表現可能。
	 *	@retval false 不可能。
	 */
	bool CanExpressLonglong(void) const
	{
		if ( DWORD_LEN > 2 )
		{
			return m_CanExpress(2);
		}
		return true;
	}

	/**
	 * [取得] LONGLONG 取得.
	 *	@note CanExpressLonglong() の結果が true でない場合、取得できる値は不定です。
	 *	@return 値
	 */
	LONGLONG ToLonglong(void) const
	{
		LONGLONG r = m_num[1];
		r <<= 32;
		return r | m_num[0];
	}
	
	/**
	 * [取得] 有効 bit 数取得.
	 *	@note 保持している値がマイナスの場合、 GetBitsCount() と同じ値になります。
	 *	@return bit数
	 */
	size_t GetValidBitsCount(void) const
	{
		loop_dn ( i, DWORD_LEN )
		{
			if ( m_num[i] != 0 )
			{
				loop_dn ( j, 32 )
				{
					if ( (m_num[i] & _BIT(j)) != 0 )
					{
						return i * 32 + j + 1;
					}
				}
			}
		}
		return 0;
	}

	/**
	 * [取得] HEX文字列化.
	 *		保持している値を HEX 文字列化します。
	 *	@param isUpperCase false を指定すると小文字、 省略 か true を指定すると大文字で作成します。
	 *	@return 文字列.
	 */
	CStr ToHexString(bool isUpperCase = true) const
	{
		CStr s;
		LPCTSTR lpszFmt = isUpperCase ? _T("%08X") :  _T("%08x");
		loop ( i, DWORD_LEN )
		{
			s += CStr::Fmt(lpszFmt, m_num[DWORD_LEN - 1 - i]);
		}
		return s;
	}

	/**
	 * [取得] 文字列化.
	 *		保持している値を文字列化します。
	 *	@return 文字列.
	 */
	CStr ToString(void) const
	{
		CLongIntegerT x = *this;
		bool isMinus = x.IsMinus();
		if ( isMinus )
		{
			x = -x;
		}
		CStr s;
		while ( x > 0 )
		{
			s = CStr::Fmt(_T("%09d"), (x % INT_VALUE_MAX).ToInt()) + s;
			x /= INT_VALUE_MAX;
		}
		s.TrimLeft('0');
		if ( s.IsEmpty() )
		{
			return _T("0");
		}
		if ( isMinus )
		{
			return _T("-") + s;
		}
		return s;
	}

	/**
	 * [処理] シリアライズ
	 *	@param[out] _sr シリアライザー。 
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		size_t l = DWORD_LEN;
		_sr << l << l;
		loop ( i, l )
		{
			_sr << m_num[i];
		}
	}

	/**
	 * [処理] デシリアライズ
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		Zero();
		size_t l1, l2;
		ds >> l1 >> l2;
		if ( l1 != l2 || l1 != DWORD_LEN )
		{
			throw CInvalidParamException();
		}
		loop ( i, l1 )
		{
			ds >> m_num[i];
		}
	}

	/**
	 * [取得] bit 数取得.
	 *	@return bit数
	 */
	static size_t GetBitsCount(void)
	{
		return BIT_LEN;
	}

	/**
	 * [取得] byte 数取得.
	 *	@return byte数
	 */
	static size_t GetBytesCount(void)
	{
		return BYTE_LEN;
	}

	/**
	 * [取得] 最大桁数取得.
	 *		10進数で表せる桁数を返します。 例えば、 4 なら 9999 ～ -9999 は表すことが出来ます。
	 *	@return 桁数.
	 */
	static size_t GetMaxPlace(void)
	{
		return GetMax().ToString().GetLength() - 1;
	}

	/**
	 * [取得] 最大値取得.
	 *		管理できる最大の値を返します。
	 *	@return 値.
	 */
	static CLongIntegerT GetMax(void)
	{
		CLongIntegerT x;
		loop ( i, DWORD_LEN )
		{
			x.m_num[i] = DWORD_MAX;
		}
		x.m_num[DWORD_LEN - 1] = 0x7FFFFFFF;
		return x;
	}

	/**
	 * [取得] 最小値取得.
	 *		管理できる最小の値を返します。
	 *	@return 値.
	 */
	static CLongIntegerT GetMin(void)
	{
		CLongIntegerT x;
		x.m_num[0] = 1;
		x.m_num[DWORD_LEN - 1] = 0x80000000;
		return x;
	}

	/**
	 * [取得] 変換値取得.
	 *		文字列から値を作成します。
	 *	@note 文字列の頭に空白はつけないでください。
	 *	\code
	 *		CLongIntegerT<4> val = CLongIntegerT<4>::FromString("-123456789");
	 *	\endcode
	 *	@param lpsz 文字列.
	 *	@return 値.
	 */
	static CLongIntegerT FromString(LPCTSTR lpsz)
	{
		CLongIntegerT x;
		x.SetString(lpsz);
		return x;
	}

	/**
	 * [取得] 変換値取得.
	 *		HEX文字列から値を作成します。
	 *	@note マイナス指定は出来ません。
	 *	@note 文字列の頭に '0x' をつけないでください。
	 *	@note 文字列の頭に頭に空白はつけないでください。
	 *	\code
	 *		CLongIntegerT<4> val = CLongIntegerT<4>::FromHexString("8000000080e412ca");
	 *	\endcode
	 *	@param lpsz 文字列.
	 *	@return 値.
	 */
	static CLongIntegerT FromHexString(LPCTSTR lpsz)
	{
		CLongIntegerT x;
		x.SetHexString(lpsz);
		return x;
	}

private:

	/// 割り算
	static bool m_Divide(const CLongIntegerT& a, const CLongIntegerT& b, CLongIntegerT& _quotient, CLongIntegerT& _remainder)
	{
		_quotient.Zero();
		_remainder.Zero();
		if ( a.IsZero() )
		{
			return true;
		}
		if ( b.IsZero() )
		{
			return false ;
		}
		CLongIntegerT aa = abs(a);
		CLongIntegerT bb = abs(b);
		if ( aa < bb )
		{
			_remainder = a;
			return true;
		}
		INT_PTR cnt = aa.GetValidBitsCount() - bb.GetValidBitsCount();
		ASSERT( cnt >= 0 );
		bb <<= cnt;
		for ( INT_PTR i = 0; i <= cnt; i++ )
		{
			_quotient <<= 1;
			if ( aa >= bb )
			{
				aa -= bb;
				_quotient.m_num[0] |= 1;
			}
			bb >>= 1;
		}
		if ( a.IsMinus() != b.IsMinus() )
		{
			_quotient = -_quotient;
		}
		_remainder = aa;
		if ( a.IsMinus() )
		{
			_remainder = -_remainder;
		}
		return true;
	}
	/// 表現可能？
	bool m_CanExpress(INDEX idx) const
	{
		DWORD n = m_num[idx];
		if ( n == 0 || n != DWORD_MAX )
		{
			for ( size_t i = idx + 1; i < DWORD_LEN; i++ )
			{
				if ( n != m_num[i] )
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
	#ifdef _TnbDOXYGEN	//Document作成用シンボル
	INT_PTR operator- (const TYP &t) const
	{
		return 0;
	}
	#endif // _TnbDOXYGEN
	/// 定数
	enum
	{
		DWORD_LEN = PLACE,			///< DWORD での長さ
		BYTE_LEN = PLACE * 4,		///< BYTE での長さ
		BIT_LEN = PLACE * 32,		///< BIT での長さ
		INT_VALUE_MAX = 1000000000,	///< int の表現可能値（9桁）
	};
	DWORD	m_num[DWORD_LEN];	///< データ
	friend class CLongIntegerTest;
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル

template<> class CLongIntegerT<0> 
{
	CLongIntegerT(void); // PLACE に 0 は指定できません。
};

template<> class CLongIntegerT<1> 
{
	CLongIntegerT(void); // PLACE に 1 は指定できません。
};

#endif // _TnbDOXYGEN



/**@ingroup VALUE
 * 256bit 整数管理
 *
 *		256bit の整数を扱うことが出来ます。
 *
 *	@note メモリには、リトルエンディアンで格納されています。
 *
 *	@par必要ファイル
 *			TnbLongInteger.h
 *
 *	@date 09/10/28 新規
 */
typedef CLongIntegerT<8> INT256;



}; // TNB
