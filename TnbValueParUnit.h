#pragma once
/**
 *@file
 * 単位付値管理関係のヘッダ
 *
 *		「単位/単位」という形の単位を管理するクラスがあります。
 *
 *		現在、 {@link TNB::CValueSpeed 速度(距離/時間)}、{@link TNB::CValueAcceleration 加速度(距離/時間^2)}、
 *		{@link TNB::CValueDensity2 面積密度(重量/面積)}、{@link TNB::CValueDensity3 体積密度(重量/体積)}、
 *		{@link TNB::CValueFlux 流量(体積/時間)}、
 *		の単位付値クラスが用意してあります。
 *
 *		また、上記以外の組み合わせも CValueParT テンプレートクラスにて対応することが可能です。
 *
 *	@note	マクロ "_TnbUnit_SpecialChar_DISABLE" が定義されていると、 UNICODE でも
 *			特殊文字(SJISに変換できない文字)を使わないようになります。
 *
 *	@see http://homepage2.nifty.com/NG/unit/index.htm
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbValueUnit.h"



//TNB Library
namespace TNB	
{



/**@ingroup VALUEUNIT
 * 単位付値管理テンプレートクラス
 *
 *		型に単位管理クラスを２つ指定することで、その単位と値を扱うクラスになります。
 *
 *	@note 継承を使わず、テンプレートによる多体化を行なっています。
 *
 *	@param U1	分子になる単位管理クラス。これが分子単位コードになります。
 *	@param U2	分母になる単位管理クラス。これが分母単位コードになります。
 *	@param EX	ベース専用の単位管理クラス。これが拡張単位コードになります。
 *
 *	@note 分子単位コードは、U1 の単位コードか、{@link CPfUnit::EPfUnitCode 接頭単位コード}を指定できます。
 *	@note 分母単位コードは、U2 の単位コードか、{@link CPfUnit::EPfUnitCode 接頭単位コード}を指定できます。
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
template<typename U1, typename U2, typename EX>
class CValueParUnitT : public U1, public U2, public EX, public CPfUnit
{
	DEFSUPER(CPfUnit);
protected:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	/**
	 * 単位コード管理クラス.
	 *	拡張単位コードを管理します。
	 */
	class CExCode
	{
		int		m_code;		///< 単位コード
	public:
		typedef typename EX::EUnitCode EExCode;
		CExCode(void) : m_code(0) {}
		CExCode(EExCode code) : m_code(code) {}
		int ToInt(void) const { return m_code; };
	};

	/**
	 * 単位コード管理クラス.
	 *	分子単位コードか、接頭単位コードか、拡張単位コードを管理します。
	 */
	class CNumeCode
	{
		int		m_code;		///< 単位コード
	public:
		typedef typename U1::EUnitCode EU1Code;
		typedef typename EX::EUnitCode EExCode;
		typedef typename CValueUnitT<U1>::CCode CVuU1Code;
		CNumeCode(void) : m_code(0) {}
		CNumeCode(EU1Code code) : m_code(code) {}
		CNumeCode(EPfUnitCode code) : m_code(code) {}
		CNumeCode(CVuU1Code code) : m_code(code.ToInt()) {}
		explicit CNumeCode(CExCode code) : m_code(code.ToInt()) {}
		operator EU1Code(void) const { return EU1Code(HasPfCode() ? 0 : m_code); }
		operator EPfUnitCode(void) const { return EPfUnitCode(HasPfCode() ? m_code : 0); }
		operator EExCode(void) const { return EExCode(HasExCode() ? m_code : 0); }
		bool operator==(const CNumeCode& c) const { return m_code == c.m_code; }
		bool operator!=(const CNumeCode& c) const { return m_code != c.m_code; }
		bool HasPfCode(void) const { return (m_code & PFCODE) != 0; }
		bool HasExCode(void) const { return (m_code & U1::EXCODE) != 0; }
		int ToInt(void) const { return m_code; };
		EU1Code GetU1Code(void) const { return EU1Code(m_code); };
		EExCode GetExCode(void) const { return EExCode(m_code); };
		CVuU1Code GetCode(void) const 
		{
			CVuU1Code c;
			c.SetInt(m_code);
			return c;
		};
	};
	
	/**
	 * 単位コード管理クラス.
	 *	分母単位コードか、接頭単位コードを管理します。
	 */
	class CDenoCode
	{
		int		m_code;		///< 単位コード
	public:
		typedef typename U2::EUnitCode EU2Code;
		typedef typename CValueUnitT<U2>::CCode CVuU2Code;
		CDenoCode(void) : m_code(0) {}
		CDenoCode(EU2Code code) : m_code(code) {}
		CDenoCode(EPfUnitCode code) : m_code(code) {}
		CDenoCode(CVuU2Code code) : m_code(code.ToInt()) {}
		operator EU2Code(void) const { return EU2Code(HasPfCode() ? 0 : m_code); }
		operator EPfUnitCode(void) const { return EPfUnitCode(HasPfCode() ? m_code : 0); }
		bool operator==(const CDenoCode& c) const { return m_code == c.m_code; }
		bool operator!=(const CDenoCode& c) const { return m_code != c.m_code; }
		bool HasPfCode(void) const { return (m_code & U2::PFCODE) != 0; }
		int ToInt(void) const { return m_code; };
		EU2Code GetU2Code(void) const { return EU2Code(m_code); };
		CVuU2Code GetCode(void) const 
		{
			CVuU2Code c;
			c.SetInt(m_code);
			return c;
		};
	};

	#endif //_TnbDOXYGEN

private:
	
	CRealNumber		m_value;			///< 値
	CNumeCode		m_numeCode;			///< 分子/拡張単位コード
	CDenoCode		m_denoCode;			///< 分母単位コード

	/// 変換
	void m_ToUnit(CNumeCode code)
	{
		if ( m_numeCode != code )
		{
			ToNormalUnit();
			if ( code.HasExCode() )
			{
				if ( m_numeCode.ToInt() != 0 )
				{
					CValueUnitT<U1> u1;
					u1.Set(m_value, m_numeCode.GetU1Code());
					m_value = u1.GetValue(U1::EUnitCode(0));
				}
				if ( m_denoCode.ToInt() != 0 )
				{
					CValueUnitT<U2> u2;
					u2.Set(1, U2::EUnitCode(0));
					m_value *= u2.GetValue(m_denoCode.GetU2Code());
					m_denoCode = U2::EUnitCode(0);
				}
				const TUnitParam* P = EX::GetParam(code.GetExCode());
				m_value *= P->doCoefficient;
				m_value += P->doRevise;
			}
			else
			{
				CValueUnitT<U1> u;
				u.Set(m_value, m_numeCode.GetU1Code());
				u.ToUnit(code.GetU1Code());
				m_value = u.GetValue();
			}
			m_numeCode = code;
		}
	}

public:

	/// 単位種コード
	enum { KINDCODE = EX::KINDCODE };

	/**
	 * コンストラクタ.
	 *		単位は分子分母はそれぞれの基準単位、値は empty になります。
	 */
	CValueParUnitT(void) : _super(), m_value(), m_numeCode(), m_denoCode()
	{
	}

	/**
	 * コンストラクタ.
	 *		指定の値のインスタンスを作成します。
	 *	@param v 値。
	 */
	CValueParUnitT(const CRealNumber& v) 
			: _super(), m_value(v), m_numeCode(), m_denoCode()
	{
	}

	/**
	 * コンストラクタ.
	 *		指定の値、単位のインスタンスを作成します。
	 *	@param v 値。
	 *	@param code 拡張単位コード。
	 */
	CValueParUnitT(const CRealNumber& v, CExCode code) 
			: _super(), m_value(v), m_numeCode(code), m_denoCode()
	{
	}

	/**
	 * コンストラクタ.
	 *		指定の値、単位のインスタンスを作成します。
	 *	@param v 値。
	 *	@param c1 分子の単位コード。
	 *	@param c2 分母の単位コード。
	 */
	CValueParUnitT(const CRealNumber& v, CNumeCode c1, CDenoCode c2) 
			: _super(), m_value(v), m_numeCode(c1), m_denoCode(c2)
	{
	}

	/**
	 * [代入] 値、単位設定
	 *		指定の値を記憶します。単位は変化しません。
	 *	@param v 値。
	 */
	void Set(const CRealNumber& v)
	{
		m_value		= v;
	}

	/**
	 * [代入] 値、単位設定
	 *		指定の値、単位を記憶します。
	 *	@param v 値。
	 *	@param code 拡張単位コード。
	 */
	void Set(const CRealNumber& v, CExCode code)
	{
		m_value		= v;
		m_numeCode	= CNumeCode(code);
		m_denoCode	= CDenoCode();
	}

	/**
	 * [代入] 値、単位設定
	 *		指定の値、単位を記憶します。
	 *	@param v 値。
	 *	@param c1 分子単位コード。
	 *	@param c2 分母単位コード。
	 */
	void Set(const CRealNumber& v, CNumeCode c1, CDenoCode c2)
	{
		m_value		= v;
		m_numeCode	= c1;
		m_denoCode	= c2;
	}

	/**
	 * [取得] 文字列で取得 
	 *	@note 指定のフォーマットで、現在の単位文字列付きで文字列に変換します。\n
	 *		ex) "5cm/s" 
	 *	@param lpszFormat printf()で double 一つを表示する場合のフォーマットを指定。省略すると "%f" になります。
	 *	@return 文字列 
	 */
	CStr ToString(LPCTSTR lpszFormat = NULL) const
	{
		return m_value.ToString(lpszFormat) + GetUnitMarkString();
	}

	/**
	 * [取得] 文字列で取得 (拡張単位指定あり)
	 *	@note 指定のフォーマットで、指定の単位に変換して文字列に変換します。\n
	 *		ex) "5cm/s" 
	 *	@param code 拡張単位コード。
	 *	@param lpszFormat printf()で double 一つを表示する場合のフォーマットを指定。省略すると "%f" になります。
	 *	@return 文字列 
	 */
	CStr ToString(CExCode code, LPCTSTR lpszFormat = NULL) const
	{
		CValueParUnitT u = *this;
		u.ToUnit(code);
		return u.ToString(lpszFormat);
	}

	/**
	 * [取得] 文字列で取得 (拡張単位指定あり)
	 *	@note 指定のフォーマットで、指定の単位に変換して文字列に変換します。\n
	 *		ex) "5cm/s" 
	 *	@param c1 分子単位コード。
	 *	@param c2 分母単位コード。
	 *	@param lpszFormat printf()で double 一つを表示する場合のフォーマットを指定。省略すると "%f" になります。
	 *	@return 文字列。
	 */
	CStr ToString(CNumeCode c1, CDenoCode c2, LPCTSTR lpszFormat = NULL) const
	{
		CValueParUnitT u = *this;
		u.ToUnit(c1, c2);
		return u.ToString(lpszFormat);
	}

	/**
	 * [確認] 単位コード確認.
	 *		現在保持している単位コードが、拡張単位コードか否かを確認します。
	 *	@retval true 拡張単位コードを保持している。
	 *	@retval false 保持していない。
	 */
	bool HasExCode(void) const
	{
		return m_numeCode.HasExCode();
	}

	/**
	 * [取得] 単位コード取得.
	 *		現在保持している分子単位コードか、拡張単位コードを取得します。
	 *	@return 単位コード。
	 */
	CNumeCode GetUnitCode(void) const
	{
		return m_numeCode;
	}

	/**
	 * [取得] 分母単位コード取得.
	 *		現在保持している分母単位コードを得ます。 
	 *		本インスタンスが拡張単位を保持している場合、戻り値は無意味です。
	 *	@return 単位コード。
	 */
	CDenoCode GetDenoUnitCode(void) const
	{
		return m_denoCode;
	}

	/**
	 * [取得] 単位マーク文字列取得.
	 *	@note 現在保持している単位のマーク文字列を返します。
	 *	@return 単位コード。
	 */
	CStr GetUnitMarkString(void) const
	{
		CStr s;
		if ( m_numeCode.HasExCode() )
		{
			s = CValueUnitT<EX>::GetUnitMarkString(m_numeCode.GetExCode());
		}
		else
		{
			s = CValueUnitT<U1>::GetUnitMarkString(m_numeCode.GetU1Code());
			s += _T("/") + CValueUnitT<U2>::GetUnitMarkString(m_denoCode.GetU2Code());
		}
		return s;
	}

	/**
	 * [取得] 単位マーク文字列取得.
	 *	@note 現在保持している単位のマーク文字列を返します。
	 *	@return 単位コード。
	 */
	CStr GetUnitJapanString(void) const
	{
		CStr s;
		if ( m_numeCode.HasExCode() )
		{
			s = CValueUnitT<EX>::GetUnitJapanString(m_numeCode.GetExCode());
		}
		else
		{
			s = CValueUnitT<U1>::GetUnitJapanString(m_numeCode.GetU1Code());
			s += _T(" 毎 ") + CValueUnitT<U2>::GetUnitJapanString(m_denoCode.GetU2Code());
		}
		return s;
	}

	/**
	 * [変換] 単位変更 (基本単位へ).
	 *	@note 保持している単位が拡張単位の場合、基本単位に変更します。値も計算されます。
	 */
	void ToNormalUnit(void)
	{
		if ( m_numeCode.HasExCode() )
		{
			//今、EXコードなら基本単位に変換
			const TUnitParam* P = EX::GetParam(m_numeCode.GetExCode());
			m_value -= P->doRevise;
			m_value /= P->doCoefficient;
			m_numeCode = U1::EUnitCode(0);
			m_denoCode = U2::EUnitCode(0);
		}
	}

	/**
	 * [変換] 単位変更.
	 *	@note 保持している単位を変更します。値も計算されます。
	 *	@param code 拡張単位コード。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& ToUnit(CExCode code)
	{
		m_ToUnit(CNumeCode(code));
		return *this;
	}

	/**
	 * [変換] 単位変更.
	 *	@note 保持している単位を変更します。値も計算されます。
	 *	@param c1 分子単位コード。
	 *	@param c2 分母単位コード。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& ToUnit(CNumeCode c1, CDenoCode c2)
	{
		m_ToUnit(c1);
		return ToDenoUnit(c2);
	}

	/**
	 * [変換] 単位変更.
	 *	@note 保持している分母の単位だけを変更します。値も計算されます。
	 *	@param code 分母単位コード。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& ToDenoUnit(CDenoCode code)
	{
		if ( m_numeCode.HasExCode() )
		{
			const TUnitParam* P = EX::GetParam(m_numeCode.GetExCode());
			m_value -= P->doRevise;
			m_value /= P->doCoefficient;
			m_numeCode = U1::EUnitCode(0);
			m_denoCode = U2::EUnitCode(0);
		}
		if ( m_denoCode != code )
		{
			CValueUnitT<U2> u;
			u.Set(1, code.GetU2Code());
			u.ToUnit(m_denoCode.GetU2Code());
			m_value *= u.GetValue();
			m_denoCode = code;
		}
		return *this;
	}

	/**
	 * [取得] 値取得.
	 *		現在保持している単位コードのまま、値を返します。
	 *	@return 値。
	 */
	CRealNumber GetValue(void) const
	{
		return m_value;
	}

	/**
	 * [取得] 値取得 (単位指定).
	 *		保持している値を、指定単位に計算して返します。
	 *	@param code 拡張単位コード。
	 *	@return 値。
	 */
	CRealNumber GetValue(CExCode code) const
	{
		CValueParUnitT r = *this;
		r.ToUnit(code);
		return r.GetValue();
	}

	/**
	 * [取得] 値取得 (単位指定).
	 *		保持している値を、指定単位に計算して返します。
	 *	@param c1 分子単位コード。
	 *	@param c2 分母単位コード。
	 *	@return 値。
	 */
	CRealNumber GetValue(CNumeCode c1, CDenoCode c2) const
	{
		CValueParUnitT r = *this;
		r.ToUnit(c1, c2);
		return r.GetValue();
	}

	/**
	 * [計算] 単位付値加算代入.
	 *		単位を考慮し値を加算代入します。
	 *	@param u 単位付値。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& operator+=(const CValueParUnitT& u)
	{
		m_value += u.GetValue(m_numeCode, m_denoCode);
		return *this;
	}

	/**
	 * [計算] 加算.
	 *		単位を考慮し値を加算します。
	 *	@param u 単位付値。
	 *	@return 加算結果。
	 */
	CValueParUnitT operator+(const CValueParUnitT& u) const
	{
		CValueParUnitT r = *this;
		return r += u;
	}

	/**
	 * [計算] 加算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& operator+=(const CRealNumber& v)
	{
		m_value += v;
		return *this;
	}

	/**
	 * [計算] 加算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueParUnitT operator+(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r += v;
	}

	/**
	 * [計算] 加算.
	 *	@param v 値。
	 *	@param u 単位付値。
	 *	@return 単位付値。
	 */
	friend CValueParUnitT operator+(const CRealNumber& v, const CValueParUnitT& u)
	{
		return u + v;
	}

	/**
	 * [計算] 単位付値減算代入.
	 *		単位を考慮し値を減算代入します。
	 *	@param u 単位付値。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& operator-=(const CValueParUnitT& u)
	{
		m_value -= u.GetValue(m_numeCode, m_denoCode);
		return *this;
	}

	/**
	 * [計算] 単位付値減算.
	 *		単位を考慮し値を減算します。
	 *	@param u 単位付値。
	 *	@return 減算結果。
	 */
	CValueParUnitT operator-(const CValueParUnitT& u) const
	{
		CValueParUnitT r = *this;
		return r -= u;
	}

	/**
	 * [計算] 減算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& operator-=(const CRealNumber& v)
	{
		m_value -= v;
		return *this;
	}

	/**
	 * [計算] 減算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueParUnitT operator-(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r -= v;
	}

	/**
	 * [計算] 減算.
	 *	@param v 値。
	 *	@param u 単位付値。
	 *	@return 単位付値。
	 */
	friend CValueParUnitT operator-(const CRealNumber& v, const CValueParUnitT& u)
	{
		CValueParUnitT r = u;
		u.m_value = v - u.m_value;
		return *this;
	}

	/**
	 * [計算] 掛算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& operator*=(const CRealNumber& v)
	{
		m_value *= v;
		return *this;
	}

	/**
	 * [計算] 掛算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueParUnitT operator*(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r *= v;
	}

	/**
	 * [計算] 掛算.
	 *	@param v 値。
	 *	@param u 単位付値。
	 *	@return 単位付値。
	 */
	friend CValueParUnitT operator*(const CRealNumber& v, const CValueParUnitT& u)
	{
		return u * v;
	}

	/**
	 * [計算] 掛算 (分子単位 = 本クラス × 分母単位).
	 *	@param t 分母単位の値
	 *	@return 分子単位の値
	 */
	CValueUnitT<U1> operator*(const CValueUnitT<U2>& t) const
	{
		CValueParUnitT v = *this;
		v.ToNormalUnit();
		v.ToDenoUnit(t.GetUnitCode());
		CValueUnitT<U1> r;
		r.Set(v.GetValue() * t.GetValue(), v.GetUnitCode().GetCode());
		return r;
	}

	/**
	 * [計算] 掛算 (分子単位 = 分母単位 × 本クラス)
	 *	@param t 分母単位の値
	 *	@param s 本クラスの値
	 *	@return 分子単位の値
	 */
	friend CValueUnitT<U1> operator*(const CValueUnitT<U2>& t, const CValueParUnitT& s)
	{
		return s * t;
	}

	/**
	 * [計算] 割算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueParUnitT& operator/=(const CRealNumber& v)
	{
		m_value /= v;
		return *this;
	}

	/**
	 * [計算] 割算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueParUnitT operator/(const CRealNumber& v) const
	{
		CValueParUnitT r = *this;
		return r /= v;
	}
	
	/**
	 * [計算] 割算 (分母単位 = 分子単位 ÷ 本クラス).
	 *	@note 分子単位を本クラスの単位で割ると、分母単位になります。
	 *	@param d 分子単位の値
	 *	@param s 本クラスの値
	 *	@return 分母単位の値
	 */
	friend CValueUnitT<U2> operator/(const CValueUnitT<U1>& d, const CValueParUnitT& s)
	{
		CValueUnitT<U2>::CCode tu = s.GetDenoUnitCode().GetCode();
		CValueUnitT<U2> r;
		r.Set(d.GetValue() / s.GetValue(d.GetUnitCode(), tu), tu);
		return r;
	}

	/**
	 * [計算] 割算 (値 = 本クラス ÷ 本クラス).
	 *	@note 同じ単位系同士の割り算は、単位がなくなります。
	 *	@param u 本クラスの値。
	 *	@return 値（単位なし）
	 */
	CRealNumber operator/(const CValueParUnitT& u) const
	{
		return m_value / u.GetValue(m_numeCode, m_denoCode);
	}

	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じ
	 *	@retval false	自分は t と異なる
	 */
	bool operator==(const CValueParUnitT& t) const { return m_value == t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と異なる
	 *	@retval false	自分は t と同じ
	 */
	bool operator!=(const CValueParUnitT& t) const { return m_value != t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t より小さい
	 *	@retval false	自分は t と同じか大きい
	 */
	bool operator<(const CValueParUnitT& t) const { return m_value < t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じか小さい
	 *	@retval false	自分は t より大きい
	 */
	bool operator<=(const CValueParUnitT& t) const { return m_value <= t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t より大きい
	 *	@retval false	自分は t と同じか小さい
	 */
	bool operator>(const CValueParUnitT& t) const { return m_value > t.GetValue(m_numeCode, m_denoCode); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じか大きい
	 *	@retval false	自分は t より小さい
	 */
	bool operator>=(const CValueParUnitT& t) const { return m_value >= t.GetValue(m_numeCode, m_denoCode); }


	//-----------------------


	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static CStr GetUnitKindString(void)
	{
		return EX::GetKindString();
	}

	/**
	 * [取得] 単位種コード取得
	 *	@return 単位種コード
	 */
	static DWORD GetUnitKindCode(void)
	{
		return EX::KINDCODE;
	}
};



/**@ingroup VALUEUNIT
 * 単位管理(速度)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitSpeed : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("速度"); }

	/// 単位種コード
	enum { KINDCODE = 'SP' };

	/// 速度拡張単位コード
	enum EUnitCode
	{
		KPH = EXCODE,		///< km/h
		FPM,				///< ft/min
		KT,					///< ノット
	};

	/**
	 * [取得] コードパラメータ取得
	 *	@param code 単位コード
	 *	@return パラメータ
	 *	@throw CNoSuchTypeException コード値異常の時、スローされます。
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		const static TUnitParam A[] = 
		{
			//	単位文字列		日本語					係数
			{	_T("kph"),		_T("時速"),				3600.0 * 1.0E-3		},	// km/h
			{	_T("fpm"),		_T("fpm"),				60.0 * 3.28084		},	// ft/min
			{	_T("kt"),		_T("ノット"),			3600.0 / 1852.0		},	// kt
		};
		int c = code - EXCODE;
		if ( c >= 0 && c < countof(A) )
		{
			return &A[c];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 速度管理クラス
 *
 *	@note 分子単位が {@link CUnitDistance::EUnitCode 距離}、分母単位が {@link CUnitTime::EUnitCode 時間} となります。
 *				それぞれ {@link CPfUnit::EPfUnitCode 接頭単位コード} も使用できます。
 *				また、一つの{@link CUnitSpeed::EUnitCode 拡張単位コード}で、単位を指定することも出来ます。
 *	@note 基準単位は、m/s({@link CUnitDistance::M CValueDistance::M}, {@link CUnitTime::SEC CValueTime::SEC}) です。
 *
 *	@see 使用できるメソッドは CValueParUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3m/s
 *		CValueSpeed d1(123.3); //コンストラクタで単位省略すると基本単位 (m/s) になります。		
 *		TRACE1("%s\n", d1.ToString()); // m/sで表示
 *		// 9.2 ピコメートル/s
 *		CValueSpeed d2(9.2, CValueDistance::PICO, CValueTime::SEC);
 *		TRACE1("%s\n", d2.ToString()); // pm/s で表示
 *		// 1.2km/h
 *		d2.Set(9.2, CValueSpeed::KPH);
 *		TRACE1("%s\n", d2.ToString(CValueSpeed::KT)); //ノットで表示
 *		TRACE1("%s\n", d2.ToString(CValueDistance::KM, CValueTime::SEC)); // km/sで表示
 *		// 100km/h
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString(); // km/hで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueParUnitT<CUnitDistance, CUnitTime, CUnitSpeed> CValueSpeed;

/**@ingroup VALUEUNIT
 * [計算] 割算 (速度 = 距離 ÷ 時間).
 *	@param d 距離
 *	@param t 時間
 *	@return 速度
 */
inline CValueSpeed operator/(const CValueDistance& d, const CValueTime& t)
{
	return CValueSpeed(d.GetValue() / t.GetValue(), d.GetUnitCode(), t.GetUnitCode());
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (距離 = 速度 ÷ 周波数).
 *	@param s 速度
 *	@param f 周波数
 *	@return 距離
 */
inline CValueDistance operator/(const CValueSpeed& s, const CValueFrequency& f)
{
	CRealNumber one(1.0);
	return s * (one / f);
}



/**@ingroup VALUEUNIT
 * 単位管理(加速度)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitAcceleration : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("加速度"); }

	/// 単位種コード
	enum { KINDCODE = 'AC' };

	/// 加速度拡張単位コード
	enum EUnitCode
	{
		GAL = EXCODE,		///< Gal			1 Gal = 1 cm/s2 = 0.01 m/s2
		G					///< 重力加速度		1 G = 9.80665 m/s2
	};

	/**
	 * [取得] コードパラメータ取得
	 *	@param code 単位コード
	 *	@return パラメータ
	 *	@throw CNoSuchTypeException コード値異常の時、スローされます。
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		const static TUnitParam A[] = 
		{
			//	単位文字列		日本語					係数
			{	_T("Gal"),		_T("ガル"),				100				},	// Gal
			{	_T("G"),		_T("重力加速度"),		1.0 / 9.80665	},	// G
		};
		int c = code - EXCODE;
		if ( c >= 0 && c < countof(A) )
		{
			return &A[c];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 加速度管理クラス
 *
 *	@note 分子単位が {@link CUnitDistance::EUnitCode 距離}、分母単位が {@link CUnitTime2::EUnitCode 時間^2} となります。
 *				それぞれ {@link CPfUnit::EPfUnitCode 接頭単位コード} も使用できます。
 *				また、一つの{@link CUnitAcceleration::EUnitCode 拡張単位コード}で、単位を指定することも出来ます。
 *	@note 基準単位は、m/s^2({@link CUnitDistance::M CValueDistance::M}, {@link CUnitTime2::SEC2 CValueTime2::SEC2}) です。
 *
 *	@see 使用できるメソッドは CValueParUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3m/s^2
 *		CValueAcceleration d1(123.3); //コンストラクタで単位省略すると基本単位 (m/s^2) になります。		
 *		TRACE1("%s\n", d1.ToString()); // m/s^2で表示
 *		// 9.2 ピコメートル/s^2
 *		CValueAcceleration d2(9.2, CValueDistance::PICO, CValueTime2::SEC2);
 *		TRACE1("%s\n", d2.ToString()); // pm/s^2 で表示
 *		// 1.2G
 *		d2.Set(9.2, CValueAcceleration::G);
 *		TRACE1("%s\n", d2.ToString(CValueAcceleration::GAL)); // Galで表示
 *		TRACE1("%s\n", d2.ToString(CValueDistance::KM, CValueTime2::SEC2)); // km/s^2で表示
 *		// 100G
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString(); // Gで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueParUnitT<CUnitDistance, CUnitTime2, CUnitAcceleration> CValueAcceleration;

/**@ingroup VALUEUNIT
 * [計算] 掛算 (加速度 = 速度 × 周波数) .
 *	@param s 速度
 *	@param f 周波数
 *	@return 加速度(単位は m/sec^2 になります)
 */
inline CValueAcceleration operator*(const CValueSpeed& s, const CValueFrequency& f)
{
	return CValueAcceleration(s.GetValue(s.M, s.SEC) * f.GetValue(f.HZ), CValueSpeed::M, CValueTime2::SEC2);
}

/**@ingroup VALUEUNIT
 * [計算] 掛算 (加速度 = 周波数 × 速度) .
 *	@param f 周波数
 *	@param s 速度
 *	@return 加速度(単位は m/sec^2 になります)
 */
inline CValueAcceleration operator*(const CValueFrequency& f, const CValueSpeed& s)
{
	return s * f;
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (加速度 = 距離 ÷ 時間^2) .
 *	@param d 距離
 *	@param t 時間^2
 *	@return 加速度
 */
inline CValueAcceleration operator/(const CValueDistance& d, const CValueTime2& t)
{
	return CValueAcceleration(d.GetValue() / t.GetValue(), d.GetUnitCode(), t.GetUnitCode());
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (加速度 = 速度 ÷ 時間) .
 *	@param s 速度
 *	@param t 時間
 *	@return 加速度 (単位は m/sec^2 になります)
 */
inline CValueAcceleration operator/(const CValueSpeed& s, const CValueTime& t)
{
	return CValueAcceleration(s.GetValue(s.M, s.SEC) / t.GetValue(t.SEC), CValueSpeed::M, CValueTime2::SEC2);
}

/**@ingroup VALUEUNIT
 * [計算] 掛算 (速度 = 加速度 × 時間) .
 *	@param a 加速度
 *	@param t 時間
 *	@return 速度 (単位は m/s になります)
 */
inline CValueSpeed operator*(const CValueAcceleration& a, const CValueTime& t)
{
	return CValueSpeed(a.GetValue(a.M, a.SEC2) * t.GetValue(t.SEC), a.M, t.SEC);
}

/**@ingroup VALUEUNIT
 * [計算] 掛算 (速度 = 時間 × 加速度) .
 *	@param t 時間
 *	@param a 加速度
 *	@return 速度 (単位は m/s になります)
 */
inline CValueSpeed operator*(const CValueTime& t, const CValueAcceleration& a)
{
	return a * t;
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (速度 = 加速度 ÷ 周波数) .
 *	@param a 加速度
 *	@param f 周波数
 *	@return 速度 (単位は m/s になります)
 */
inline CValueSpeed operator/(const CValueAcceleration& a, const CValueFrequency& f)
{
	return CValueSpeed(a.GetValue(a.M, a.SEC2) / f.GetValue(f.HZ), a.M, CValueSpeed::SEC);
}



/**@ingroup VALUEUNIT
 * 単位管理(面積密度)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/22 新規作成
 */
class CUnitDensity2 : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("面積"); }

	/// 単位種コード
	enum { KINDCODE = '2D' };

	/// 面積密度拡張単位コード
	enum EUnitCode {};

	/**
	 * [取得] コードパラメータ取得
	 *	@param code 単位コード
	 *	@return パラメータ
	 *	@throw CNoSuchTypeException コード値異常の時、スローされます。
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 面積密度管理クラス
 *
 *	@note 分子単位が {@link CUnitWeight::EUnitCode 重量}、分母単位が {@link CUnitArea::EUnitCode 面積} となります。
 *				それぞれ {@link CPfUnit::EPfUnitCode 接頭単位コード} も使用できます。
 *	@note 基準単位は、g/㎡({@link CUnitWeight::G CValueWeight::G}, {@link CUnitArea::M2 CValueArea::M2}) です。
 *
 *	@see 使用できるメソッドは CValueParUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3 g/㎡
 *		CValueDensity2 d1(123.3); //コンストラクタで単位省略すると基本単位 (g/㎡) になります。		
 *		TRACE1("%s\n", d1.ToString()); // g/㎡で表示
 *		// 9.2 ピコg/㎡
 *		CValueDensity2 d2(9.2, CValueWeight::PICO, CUnitArea::M2);
 *		TRACE1("%s\n", d2.ToString()); // pg/㎡ で表示
 *		TRACE1("%s\n", d2.ToString(CValueWeight::KG, CValueArea::M2)); // kg/㎡で表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/22 新規作成
 */
typedef CValueParUnitT<CUnitWeight, CUnitArea, CUnitDensity2> CValueDensity2;

/**@ingroup VALUEUNIT
 * [計算] 割算 (面積密度 = 重量 ÷ 面積).
 *	@param d 重量
 *	@param a 面積
 *	@return 面積密度
 */
inline CValueDensity2 operator /(const CValueWeight& d, const CValueArea& a)
{
	return CValueDensity2(d.GetValue() / a.GetValue(), d.GetUnitCode(), a.GetUnitCode());
}



/**@ingroup VALUEUNIT
 * 単位管理(体積密度)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitDensity3 : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("体積密度"); }

	/// 単位種コード
	enum { KINDCODE = 'DE' };

	/// 体積密度拡張単位コード
	enum EUnitCode {};

	/**
	 * [取得] コードパラメータ取得
	 *	@param code 単位コード
	 *	@return パラメータ
	 *	@throw CNoSuchTypeException コード値異常の時、スローされます。
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 体積密度管理クラス
 *
 *	@note 分子単位が {@link CUnitWeight::EUnitCode 重量}、分母単位が {@link CUnitCubic::EUnitCode 体積} となります。
 *				それぞれ {@link CPfUnit::EPfUnitCode 接頭単位コード} も使用できます。
 *	@note 基準単位は、g/m^3({@link CUnitWeight::G CValueWeight::G}, {@link CUnitCubic::M3 CValueCubic::M3}) です。
 *
 *	@see 使用できるメソッドは CValueParUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3 g/m^3
 *		CValueDensity3 d1(123.3); //コンストラクタで単位省略すると基本単位 (g/m^3) になります。		
 *		TRACE1("%s\n", d1.ToString()); // g/m^3で表示
 *		// 9.2 ピコg/m^3
 *		CValueDensity3 d2(9.2, CValueWeight::PICO, CValueCubic::M3);
 *		TRACE1("%s\n", d2.ToString()); // pg/m^3 で表示
 *		TRACE1("%s\n", d2.ToString(CValueWeight::KG, CValueCubic::M3)); // kg/m^3で表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueParUnitT<CUnitWeight, CUnitCubic, CUnitDensity3> CValueDensity3;

/**@ingroup VALUEUNIT
 * [計算] 割算 (体積密度 = 重量 ÷ 体積).
 *	@param d 重量
 *	@param t 体積
 *	@return 体積密度
 */
inline CValueDensity3 operator /(const CValueWeight& d, const CValueCubic& t)
{
	return CValueDensity3(d.GetValue() / t.GetValue(), d.GetUnitCode(), t.GetUnitCode());
}



/**@ingroup VALUEUNIT
 * 単位管理(流量)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitFlux : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("流量"); }

	/// 単位種コード
	enum { KINDCODE = 'FL' };

	/// 密度拡張単位コード
	enum EUnitCode {};

	/**
	 * [取得] コードパラメータ取得
	 *	@param code 単位コード
	 *	@return パラメータ
	 *	@throw CNoSuchTypeException コード値異常の時、スローされます。
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 流量管理クラス
 *
 *	@note 分子単位が {@link CUnitCubic::EUnitCode 体積}、分母単位が {@link CUnitTime::EUnitCode 時間} となります。
 *				それぞれ {@link CPfUnit::EPfUnitCode 接頭単位コード} も使用できます。
 *	@note 基準単位は、m^3/s({@link CUnitCubic::M3 CValueCubic::M3}, {@link CUnitTime::SEC CValueTime::SEC}) です。
 *
 *	@see 使用できるメソッドは CValueParUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3 m^3/s
 *		CValueFlux d1(123.3); //コンストラクタで単位省略すると基本単位 (m^3/s) になります。		
 *		TRACE1("%s\n", d1.ToString()); // m^3/sで表示
 *		// 9.2 mm^3/ms
 *		CValueFlux d2(9.2, CValueCubic::MM3, CValueTime::MSEC);
 *		TRACE1("%s\n", d2.ToString()); // mm^3/ms で表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/18 新規作成
 */
typedef CValueParUnitT<CUnitCubic, CUnitTime, CUnitFlux> CValueFlux;

/**@ingroup VALUEUNIT
 * [計算] 割算 (流量 = 体積 ÷ 時間).
 *	@param c 体積
 *	@param t 時間
 *	@return 流量
 */
inline CValueFlux operator /(const CValueCubic& c, const CValueTime& t)
{
	return CValueFlux(c.GetValue() / t.GetValue(), c.GetUnitCode(), t.GetUnitCode());
}



//-------------------



/**@ingroup VALUEUNIT
 * 単位管理(Par用ダミー)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/11 新規作成
 */
class CUnitParDummy : public CAbstractUnit
{
public:

	/// 単位種コード
	enum { KINDCODE = 0 };

	/// ダミー拡張単位コード
	enum EUnitCode	{};

	/**
	 * [取得] コードパラメータ取得
	 *	@param code 単位コード
	 *	@throw CNoSuchTypeException 必ず、スローされます。
	 *	@return 帰りません。
	 */
	static const TUnitParam* GetParam(EUnitCode code)
	{
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 汎用管理クラス
 *
 *	@par必要ファイル
 *			TnbValueParUnit.h
 * 
 *	@date 07/01/11 新規作成
 */
template <typename U1, typename U2>
class CValueParT : public CValueParUnitT<U1, U2, CUnitParDummy>
{
	typedef CValueParUnitT<U1, U2, CUnitParDummy> _super;	///< 親クラス名再定義
	DEFSUPER(_super);
public:

	/// 単位種コード
	enum { KINDCODE = U1::KINDCODE * 0x1000 + U2::KINDCODE };

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static CStr GetKindString(void)
	{
		CStr s;
		s.Format(_T("%s 毎 %s"), U1::GetKindString(), U2::GetKindString());
		return s; 
	}

	/**
	 * コンストラクタ.
	 *		単位はコード０、値は empty になります。
	 */
	CValueParT(void) : _super()
	{
	}

	/**
	 * コンストラクタ.
	 *		指定の値、単位のインスタンスを作成します。
	 *	@param v 値。
	 *	@param c1 分子の単位コード。
	 *	@param c2 分母の単位コード。
	 */
	CValueParT(const CRealNumber& v, CNumeCode c1, CDenoCode c2) 
			: _super(v, c1, c2)
	{
	}

	/**
	 * [計算] 割算 (本クラス = 分子単位 ÷ 分母単位).
	 *	@param u1 分子単位の値。
	 *	@param u2 分母単位の値。
	 *	@return 計算結果を持つ、本クラスのインスタンス。
	 */
	friend CValueParT operator /(const CValueUnitT<U1>& u1, const CValueUnitT<U2>& u2)
	{
		return CValueParT(u1.GetValue() / u2.GetValue(), u1.GetUnitCode(), u2.GetUnitCode());
	}
};



}; // TNB
