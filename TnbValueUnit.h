#pragma once
/**
 *@file
 * 単位付値管理関係のヘッダ
 *
 *		現在、{@link TNB::CValueDistance 距離}、{@link TNB::CValueArea 面積}、{@link TNB::CValueCubic 体積}、
 *		{@link TNB::CValueWeight 重量}、{@link TNB::CValueTemperature 温度}、{@link TNB::CValueTime 時間}、
 *		{@link TNB::CValueTime2 時間^2}、{@link TNB::CValueFrequency 周波数}、{@link TNB::CValueDegree 角度}、
 *		{@link TNB::CUnitPressure 圧力}、{@link TNB::CUnitRatio 割合}、
 *		{@link TNB::CValueAmpere 電流(アンペア)}、{@link TNB::CValueVolt 電圧(ボルト)}
 *		の単位付値クラスが存在します。
 *
 *	@note	マクロ "_TnbUnit_SpecialChar_DISABLE" が定義されていると、 UNICODE でも
 *			特殊文字(SJISに変換できない文字)を使わないようになります。
 *
 *	@see http://homepage2.nifty.com/NG/unit/index.htm
 *	@see http://homepage2.nifty.com/NG/unit/e90608kj.pdf
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbRealNumber.h"
#include "TnbException.h"



//TNB Library
namespace TNB	
{



/// 円周率
#define _PAI 3.14159265358979323846264338327950288 

#ifndef _TnbDOXYGEN	//Document作成用シンボル
#if defined(_UNICODE) && ! defined(_TnbUnit_SpecialChar_DISABLE)
	#define _S2		L"\x00B2"
	#define _M3		L"\x33A5"
	#define _S3		L"\x00B3"
	#define _LITRE	L"\x2113"
	#define _DL		L"\x3397"
#else
	#define _S2		_T("^2")
	#define _M3		_T("m^3")
	#define _S3		_T("^3")
	#define _LITRE	_T("l")
	#define _DL		_T("dl")
#endif
#endif //_TnbDOXYGEN



/**@ingroup VALUEUNIT
 * 単位管理抽象クラス
 *
 *		単位管理クラスはこのクラスを継承して作成。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CAbstractUnit
{
public:
	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	/// 定数
	enum {
		RASING = 1,				///< 乗数値
		EXCODE = _BIT(29),		///< 拡張単位コード
		PFCODE = _BIT(30),		///< 接頭単位コード
		TNBLIB_VALUEUNIT = -1	///< 識別子
	};
	#endif
};



/**@ingroup VALUEUNIT
 * 単位情報テーブル型
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 */
struct TUnitParam
{
	LPCTSTR lpszMarkString;		///< 単位記号文字列
	LPCTSTR lpszJapanString;	///< 単位日本語文字列
	double	doCoefficient;		///< 変換係数
	double	doRevise;			///< 誤差係数
};



/**@ingroup VALUEUNIT
 * 接頭単位管理クラス
 *
 *		接頭単位を扱うクラスはこのクラスを継承して作成。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 *	@date 10/11/18 2進接頭辞、追加
 */
class CPfUnit
{
public:
	/// 接頭単位コード.
	enum EPfUnitCode
	{
		DEF = _BIT(30),		///< -
		YOTTA,				///< ヨタ		(1.0E+24)
		ZETTA,				///< ゼタ		(1.0E+21)
		EXA,				///< エクサ		(1.0E+18)
		PETA,				///< ぺタ		(1.0E+15)
		TERA,				///< テラ		(1.0E+12)
		GIGA,				///< ギガ		(1.0E+9)
		MEGA,				///< メガ		(1.0E+6)
		KILO,				///< キロ		(1.0E+3)
		DECI,				///< デシ		(1.0E-1)
		CENTI,				///< センチ		(1.0E-2)
		MILLI,				///< ミリ		(1.0E-3)
		MICRO,				///< マイクロ	(1.0E-6)
		NANO,				///< ナノ		(1.0E-9)
		PICO,				///< ピコ		(1.0E-12)
		FEMTO,				///< フェムト	(1.0E-15)
		ATTO,				///< アト		(1.0E-18)
		ZEPTO,				///< ゼプト		(1.0E-21)
		YOCTO,				///< ヨクト		(1.0E-24)
		//
		YOBI,				///< ヨビ		(2^80)
		ZEBI,				///< ゼビ		(2^70)
		EXBI,				///< エクスビ	(2^60)
		PEBI,				///< ぺビ		(2^50)
		TEBI,				///< テビ		(2^40)
		GIBI,				///< ギビ		(2^30)
		MEBI,				///< メビ		(2^20)
		KIBI,				///< キビ		(2^10)

		KIRO = KILO,				///< キロ		(1.0E+3)
	};

	/**
	 * [取得] 接頭単位コードパラメータ取得
	 *	@param code 接頭単位コード
	 *	@return パラメータ
	 *	@throw CNoSuchTypeException コード値異常の時、スローされます。
	 */
	static const TUnitParam* GetPfParam(EPfUnitCode code)
	{
		const static TUnitParam A[] = 
		{
			//	単位文字列		日本語				係数
			{	_T(""),			_T(""),				1.0			},	// -
			{	_T("Y"),		_T("ヨタ"),			1.0E-24		},	// yotta
			{	_T("Z"),		_T("ゼタ"),			1.0E-21		},	// zetta
			{	_T("E"),		_T("エクサ"),		1.0E-18		},	// exa
			{	_T("P"),		_T("ぺタ"),			1.0E-15		},	// peta
			{	_T("T"),		_T("テラ"),			1.0E-12		},	// tera
			{	_T("G"),		_T("ギガ"),			1.0E-9		},	// giga
			{	_T("M"),		_T("メガ"),			1.0E-6		},	// mega
			{	_T("k"),		_T("キロ"),			1.0E-3		},	// kilo
			{	_T("d"),		_T("デシ"),			1.0E+1		},	// deci
			{	_T("c"),		_T("センチ"),		1.0E+2		},	// centi
			{	_T("m"),		_T("ミリ"),			1.0E+3		},	// milli
			{	_T("μ"),		_T("マイクロ"),		1.0E+6		},	// micro
			{	_T("n"),		_T("ナノ"),			1.0E+9		},	// nano
			{	_T("p"),		_T("ピコ"),			1.0E+12		},	// pico
			{	_T("f"),		_T("フェムト"),		1.0E+15		},	// femto
			{	_T("a"),		_T("アト"),			1.0E+18		},	// atto
			{	_T("z"),		_T("ゼプト"),		1.0E+21		},	// zepto
			{	_T("y"),		_T("ヨクト"),		1.0E+24		},	// yocto
			//
			{	_T("Yi"),		_T("ヨビ"),			1.0 / 1208925819614629174706176.0	},
			{	_T("Zi"),		_T("ゼビ"),			1.0 / 1180591620717411303424.0 },
			{	_T("Ei"),		_T("エクスビ"),		1.0 / (1ui64 << 60)	} ,
			{	_T("Pi"),		_T("ペビ"),			1.0 / (1ui64 << 50) },
			{	_T("Ti"),		_T("テビ"),			1.0 / (1ui64 << 40) },
			{	_T("Gi"),		_T("ギビ"),			1.0 / (1ui64 << 30) },
			{	_T("Mi"),		_T("メビ"),			1.0 / (1ui64 << 20) },
			{	_T("Ki"),		_T("キビ"),			1.0 / (1ui64 << 10)	},
		};
		int i = code - DEF;		
		if ( i >= 0 && i < countof(A) )
		{
			return &A[i];
		}
		throw CNoSuchTypeException();
	}
};



/**@ingroup VALUEUNIT
 * 単位付値管理テンプレートクラス
 *
 *		型に単位管理クラスを指定することで、その単位と値を扱うクラスになります。
 *
 *	@note 継承を使わず、テンプレートによる多体化を行なっています。
 *
 *	@param U 単位管理クラス。これにより単位種が決まります。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
template<typename U>
class CValueUnitT : public U, public CPfUnit
{
public:

	/**@ingroup VALUEUNIT
	 * 単位コード管理クラス
	 *
	 *	@note	メソッドの引数に本クラスが指定されているものは、
	 *			U::EUnitCode か {@link CPfUnit::EPfUnitCode 接頭単位コード}を指定できます。
	 *			ただし、単位種によっては 接頭単位コードが使用が適さないものも
	 *			あります（例えば「温度」など）。
	 */
	class CCode
	{
		int		m_code;		///< 単位コード
	public:
		typedef typename U::EUnitCode EUnitCode;	///< 入れ子型名再定義
		/// コンストラクタ
		CCode(void) : m_code(0) {}
		/**
		 * コンストラクタ
		 *	@param code 単位コード
		 */
		CCode(EUnitCode code) : m_code(code) {}
		/**
		 * コンストラクタ
		 *	@param code 接頭単位コード
		 */
		CCode(EPfUnitCode code) : m_code(code) {}
		/**
		 * [取得] 単位コード取得
		 *	@return 単位コード
		 */
		operator EUnitCode(void) const { return EUnitCode(HasPfCode() ? 0 : m_code); }
		/**
		 * [取得] 接頭単位コード取得
		 *	@return 接頭単位コード
		 */
		operator EPfUnitCode(void) const { return EPfUnitCode(HasPfCode() ? m_code : 0); }
		/**
		 * [確認] 比較
		 *	@param c 比較対称
		 *	@retval true 一致
		 *	@retval false 不一致
		 */
		bool operator==(const CCode& c) const { return m_code == c.m_code; }
		/**
		 * [確認] 比較
		 *	@param c 比較対称
		 *	@retval true 不一致
		 *	@retval false 一致
		 */
		bool operator!=(const CCode& c) const { return m_code != c.m_code; }
		/**
		 * [確認] 接頭単位コード保持？
		 *	@retval true SIコード保持。
		 *	@retval false 否。
		 */
		bool HasPfCode(void) const { return (m_code & PFCODE) != 0; }
		/**
		 * [取得] intで取得
		 *	@return コード
		 */
		int ToInt(void) const { return m_code; };
		/**
		 * [設定] intで設定
		 *	@param i コード
		 */
		void SetInt(int i) { m_code = i; }
	};

private:

	CRealNumber			m_value;	///< 値
	CCode				m_code;		///< 単位コード
	
	void m_Test(const CAbstractUnit* u){}
	void m_Test(LPCVOID u){ ASSERT(false); }

	/// 誤差係数取得
	double m_GetRevise(const CCode& c) const
	{
		const TUnitParam* P = c.HasPfCode() ? GetPfParam(c) : U::GetParam(c);
		return P->doRevise;
	}

	/// 変換係数取得	
	double m_GetCoefficient(const CCode& c) const
	{
		double r;
		if ( c.HasPfCode() )
		{
			double a = GetPfParam(c)->doCoefficient;
			r = 1;
			loop ( i, U::RASING )
			{
				r *= a;
			}
		}
		else
		{
			r = U::GetParam(c)->doCoefficient;
		}
		return r;
	}

public:

	/**
	 * コンストラクタ.
	 *		単位は単位種の基準単位に。値は empty になります。
	 */
	CValueUnitT(void) : m_value(), m_code()
	{
		DEBUG_ONLY(m_Test(this));
	}

	/**
	 * コンストラクタ.
	 *		指定の値のインスタンスを作成します。単位は単位種の基準単位になります。
	 *	@param v 値。
	 */
	explicit CValueUnitT(const CRealNumber& v) : m_value(v), m_code()
	{
		DEBUG_ONLY(m_Test(this));
	}

	/**
	 * コンストラクタ.
	 *		指定の値、単位のインスタンスを作成します。
	 *	@param v 値。
	 *	@param code 単位コード。
	 */
	CValueUnitT(const CRealNumber& v, CCode code) : m_value(v), m_code(code)
	{
		DEBUG_ONLY(m_Test(this));
	}

	/**
	 * [代入] 値設定
	 *		指定の値を設定します。単位は変化しません。
	 *	@param v 値。
	 */
	void Set(const CRealNumber& v)
	{
		m_value	= v;
	}

	/**
	 * [代入] 値、単位設定
	 *		指定の値、単位を設定します。
	 *	@param v 値。
	 *	@param code 単位コード。
	 */
	void Set(const CRealNumber& v, CCode code)
	{
		m_value	= v;
		m_code	= code;
	}

	/**
	 * [取得] 文字列で取得 
	 *	@note 指定のフォーマットで、現在の単位文字列付きで文字列に変換します。\n
	 *		ex) "5cm" 
	 *	@param lpszFormat printf()で double 一つを表示する場合のフォーマットを指定。省略すると "%f" になります。
	 *	@return 文字列 
	 */
	CStr ToString(LPCTSTR lpszFormat = NULL) const
	{
		return m_value.ToString(lpszFormat) + GetUnitMarkString(m_code);
	}

	/**
	 * [取得] 文字列で取得（単位指定あり）
	 *	@note 指定のフォーマットで、指定の単位に変換して文字列に変換します。\n
	 *		ex) "5cm" 
	 *	@param code 単位コード。 
	 *	@param lpszFormat printf()で double 一つを表示する場合のフォーマットを指定。省略すると "%f" になります。
	 *	@return 文字列 
	 */
	CStr ToString(CCode code, LPCTSTR lpszFormat = NULL) const
	{
		CValueUnitT r = *this;
		r.ToUnit(code);
		return r.ToString(lpszFormat);
	}

	/**
	 * [取得] 単位コード取得.
	 *		現在保持している単位コード取得。
	 *	@return 単位コード。
	 */
	CCode GetUnitCode(void) const
	{
		return m_code;
	}

	/**
	 * [取得] 単位マーク文字列取得.
	 *		現在保持している単位の文字列を返します。
	 *	@return 単位マーク文字列。
	 */
	CStr GetUnitMarkString(void) const
	{
		return GetUnitMarkString(m_code);
	}

	/**
	 * [取得] 単位意味文字列取得.
	 *		現在保持している単位の文字列を返します。
	 *	@return 単位意味文字列。
	 */
	CStr GetUnitJapanString(void) const
	{
		return GetUnitJapanString(m_code);
	}

	/**
	 * [変換] 単位変更.
	 *	@note 保持している単位を変更します。値も計算されます。
	 *	@param code 単位コード。
	 *	@return 自分の参照。
	 */
	CValueUnitT& ToUnit(CCode code)
	{
		if ( m_code != code )
		{
			m_value -= m_GetRevise(m_code);
			m_value *= m_GetCoefficient(code) / m_GetCoefficient(m_code);
			m_value += m_GetRevise(code);
			m_code = code;
		}
		return *this;
	}

	/**
	 * [取得] 値取得.
	 *		現在の単位コードのまま、値を返します。
	 *	@return 値。
	 */
	CRealNumber GetValue(void) const
	{
		return m_value;
	}

	/**
	 * [取得] 値取得.
	 *		現在の単位コードのまま、値を返します。
	 *	@return 値。
	 */
	operator CRealNumber(void) const
	{
		return m_value;
	}

	/**
	 * [取得] 値取得(単位指定).
	 *		保持している値を、指定単位に計算して返します。
	 *	@param code 単位コード。
	 *	@return 値。
	 */
	CRealNumber GetValue(CCode code) const
	{
		CValueUnitT r = *this;
		r.ToUnit(code);
		return r.GetValue();
	}

	/**
	 * [計算] 単位付値加算代入.
	 *		単位を考慮し値を加算代入します。
	 *	@param u 単位付値。
	 *	@return 自分の参照。
	 */
	CValueUnitT& operator+=(const CValueUnitT& u)
	{
		m_value += u.GetValue(m_code);
		return *this;
	}

	/**
	 * [計算] 単位付値加算.
	 *		単位を考慮し値を加算します。
	 *	@param u 単位付値。
	 *	@return 加算した結果。
	 */
	CValueUnitT operator+(const CValueUnitT& u) const
	{
		CValueUnitT r = *this;
		return r += u;
	}

	/**
	 * [計算] 加算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueUnitT& operator+=(const CRealNumber& v)
	{
		m_value += v;
		return *this;
	}

	/**
	 * [計算] 加算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueUnitT operator+(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r += v;
	}

	/**
	 * [計算] 加算.
	 *	@param v 値。
	 *	@param u 単位付値。
	 *	@return 単位付値。
	 */
	friend CValueUnitT operator+(const CRealNumber& v, const CValueUnitT& u)
	{
		return u + v;
	}

	/**
	 * [計算] 単位付値減算代入.
	 *		単位を考慮し値を減算します。
	 *	@param u 単位付値。
	 *	@return 自分の参照。
	 */
	CValueUnitT& operator-=(const CValueUnitT& u)
	{
		m_value -= u.GetValue(m_code);
		return *this;
	}

	/**
	 * [計算] 単位付値減算.
	 *		単位を考慮し値を減算します。
	 *	@param u 単位付値。
	 *	@return 加算した結果
	 */
	CValueUnitT operator-(const CValueUnitT& u) const
	{
		CValueUnitT r = *this;
		return r -= u;
	}

	/**
	 * [計算] 減算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueUnitT& operator-=(const CRealNumber& v)
	{
		m_value -= v;
		return *this;
	}

	/**
	 * [計算] 減算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueUnitT operator-(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r -= v;
	}

	/**
	 * [計算] 減算.
	 *	@param v 値。
	 *	@param u 単位付値。
	 *	@return 単位付値。
	 */
	friend CValueUnitT operator-(const CRealNumber& v, const CValueUnitT& u)
	{
		CValueUnitT r = u;
		u.m_value = v - u.m_value;
		return *this;
	}

	/**
	 * [計算] 掛算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueUnitT& operator*=(const CRealNumber& v)
	{
		m_value *= v;
		return *this;
	}

	/**
	 * [計算] 掛算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueUnitT operator*(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r *= v;
	}

	/**
	 * [計算] 掛算.
	 *	@param v 値。
	 *	@param u 単位付値。
	 *	@return 単位付値。
	 */
	friend CValueUnitT operator*(const CRealNumber& v, const CValueUnitT& u)
	{
		return u * v;
	}

	/**
	 * [計算] 単位付値割算.
	 *	@note 同単位系同士の割り算は、単位がなくなります。
	 *	@param u 単位付値。
	 *	@return 値（単位なし）。
	 */
	CRealNumber operator/(const CValueUnitT& u) const
	{
		return m_value / u.GetValue(m_code);
	}

	/**
	 * [計算] 割算代入.
	 *	@param v 値。
	 *	@return 自分の参照。
	 */
	CValueUnitT& operator/=(const CRealNumber& v)
	{
		m_value /= v;
		return *this;
	}

	/**
	 * [計算] 割算.
	 *	@param v 値。
	 *	@return 単位付値。
	 */
	CValueUnitT operator/(const CRealNumber& v) const
	{
		CValueUnitT r = *this;
		return r /= v;
	}

	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じ
	 *	@retval false	自分は t と異なる
	 */
	bool operator==(const CValueUnitT& t) const { return m_value == t.GetValue(m_code); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と異なる
	 *	@retval false	自分は t と同じ
	 */
	bool operator!=(const CValueUnitT& t) const { return m_value != t.GetValue(m_code); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t より小さい
	 *	@retval false	自分は t と同じか大きい
	 */
	bool operator<(const CValueUnitT& t) const { return m_value < t.GetValue(m_code); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じか小さい
	 *	@retval false	自分は t より大きい
	 */
	bool operator<=(const CValueUnitT& t) const {return m_value <= t.GetValue(m_code); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t より大きい
	 *	@retval false	自分は t と同じか小さい
	 */
	bool operator>(const CValueUnitT& t) const { return m_value > t.GetValue(m_code); }
	
	/**
	 * [確認] 比較
	 *	@note 単位をそろえて比較します。
	 *	@param t 比較対象
	 *	@retval true	自分は t と同じか大きい
	 *	@retval false	自分は t より小さい
	 */
	bool operator>=(const CValueUnitT& t) const { return m_value >= t.GetValue(m_code); }


	//----------------


	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static CStr GetUnitKindString(void)
	{
		return U::GetKindString();
	}

	/**
	 * [取得] 単位種コード取得
	 *	@return 単位種コード
	 */
	static DWORD GetUnitKindCode(void)
	{
		return U::KINDCODE;
	}

	/**
	 * [取得] 単位マーク文字列取得.
	 *	@param code 単位コード。
	 *	@return 単位マーク文字列。
	 */
	static CStr GetUnitMarkString(CCode code)
	{
		CStr s;
		if ( code.HasPfCode() )
		{
			s = GetPfParam(code)->lpszMarkString;
			s += U::GetParam(U::EUnitCode(0))->lpszMarkString;
		}
		else
		{
			s = U::GetParam(code)->lpszMarkString;
		}
		return s;
	}

	/**
	 * [取得] 単位意味文字列取得.
	 *	@param code 単位コード。
	 *	@return 単位意味文字列。
	 */
	static CStr GetUnitJapanString(CCode code) 
	{
		CStr s;
		if ( code.HasPfCode() )
		{
			s = GetPfParam(code)->lpszJapanString;
			s += U::GetParam(U::EUnitCode(0))->lpszJapanString;
		}
		else
		{
			s = U::GetParam(code)->lpszJapanString;
		}
		return s;
	}
};



//--------------------------------



/**@ingroup VALUEUNIT
 * [作成] 文字列作成
 *	@param t 単位付値クラスのインスタンス。
 *	@param lpszFormat printf()で double 一つを表示する場合のフォーマットを指定。省略すると "%f" になります。
 *	@return 文字列
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
template<typename T>
inline CStr ToStringWithJapanUnit(const T& t, LPCTSTR lpszFormat = NULL)
{
	return t.GetValue().ToString(lpszFormat) + t.GetUnitJapanString();
}



/**@ingroup VALUEUNIT
 * [作成] 文字列作成
 *	@param t 単位付値クラスのインスタンス。
 *	@param lpszFormat printf()で double 一つを表示する場合のフォーマットを指定。省略すると "%f" になります。
 *	@return 文字列
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
template<typename T>
inline CStr ToStringWithUnitMark(const T& t, LPCTSTR lpszFormat = NULL)
{
	return t.GetValue().ToString(lpszFormat) + t.GetUnitMarkString();
}



/**@ingroup VALUEUNIT
 * [取得] 単位種文字列取得
 *	@param t 単位付値クラスのインスタンス。
 *	@return 単位種文字列
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
template<typename T>
inline CStr GetUnitKindString(const T& t)
{
	return T::GetKindString();
}

/**@ingroup VALUEUNIT
 * [取得] 単位種コード取得
 *	@param t 単位付値クラスのインスタンス。
 *	@return 単位種コード
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
template<typename T>
inline DWORD GetUnitKindCode(const T& t)
{
	return T::KINDCODE;
}



//--------------------------------



/**@ingroup VALUEUNIT
 * 単位管理(距離)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@note 天文単位;
 *			地球が太陽の周りを回る楕円軌道の長半径(簡単には太陽から地球までの平均的距離)が1天文単位
 *			として定義されている。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitDistance : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("距離"); }

	/// 単位種コード
	enum { KINDCODE = 'Di' };

	/// 距離単位コード
	enum EUnitCode
	{
		M,			///< メートル
		INCH,		///< インチ
		FEET,		///< フィート
		YARD,		///< ヤード
		MILE,		///< マイル
		KAIRI,		///< 海里
		SHAKU,		///< 尺
		KEN,		///< 間
		CHOU,		///< 町
		SATO,		///< 里
		ANG,		///< オングストローム
		AU,			///< 天文単位
		LY,			///< 光年
		PC,			///< パーセク

		MM		= CPfUnit::MILLI,	///< ミリメートル
		CM		= CPfUnit::CENTI,	///< センチメートル
		KM		= CPfUnit::KIRO,	///< キロメートル
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
			{	_T("m"),		_T("メートル"),			1.0			},	// m
			{	_T("in"),		_T("インチ"),			39.37		},	// inch
			{	_T("ft"),		_T("フィート"),			3.28084		},	// feet
			{	_T("yd"),		_T("ヤード"),			1.09361		},	// yard
			{	_T("mile"),		_T("マイル"),			0.000621	},	// mile
			{	_T("n.m."),		_T("海里"),				0.000539	},	// kairi
			{	_T("尺"),		_T("尺"),				3.3			},	// 尺
			{	_T("間"),		_T("間"),				3.3 / 6.0	},	// 間
			{	_T("町"),		_T("町"),				3.3	/ 360.0	},	// 町
			{	_T("里"),		_T("里"),				3.3 / 12960.0},	// 里
			{	_T("Å"),		_T("オングストローム"),	1.0E+10		},	// オングストローム
			{	_T("AU"),		_T("天文単位"),			6.68459E-12	},	// 天文単位
			{	_T("ly"),		_T("光年"),				1.05700E-16	},	// 光年
			{	_T("pc"),		_T("パーセク"),			3.24078E-17	},	// パーセク
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 距離管理クラス
 *
 *	@note 単位として {@link CUnitDistance::EUnitCode 距離単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、メートル({@link CUnitDistance::M CValueDistance::M}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3m
 *		CValueDistance d1(123.3); //コンストラクタで単位省略すると基本単位 (m) になります。		
 *		TRACE1("%s\n", d1.ToString()); //mで表示
 *		// 9.2ピコメートル
 *		CValueDistance d2(9.2, CValueDistance::PICO);
 *		TRACE1("%s\n", d2.ToString()); //pmで表示
 *		// 9.2ヤード
 *		d2.Set(9.2, CValueDistance::YARD);
 *		TRACE1("%s\n", d2.ToString(CValueDistance::M)); //mで表示
 *		// 1.2光年
 *		d2.Set(1.2, CValueDistance::LY);
 *		// 100光年
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueUnitT<CUnitDistance> CValueDistance;

/**@ingroup VALUEUNIT
 * [計算] 斜め距離計算 (√(水平距離^2 + 垂直距離^2))
 *	@param x 水平距離
 *	@param y 垂直距離
 *	@return 斜め距離
 */
inline CValueDistance CalcSlantingDistance(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	const CRealNumber& xx = x.GetValue();
	const CRealNumber& yy = y.GetValue(code);
	CRealNumber r = sqrt(xx * xx + yy * yy);
	return CValueDistance(r, code);
}

/**@ingroup VALUEUNIT
 * [計算] 円周囲長計算 (2 × π × 半径)
 *	@param r 半径距離
 *	@return 距離
 */
inline CValueDistance CalcCircleDistance(const CValueDistance& r)
{
	return r * 2 * _PAI;
}

/**@ingroup VALUEUNIT
 * [計算] 楕円周囲長計算 (π × √( 2 (水平半径^2 + 垂直半径^2))
 *	@param x 水平半径距離
 *	@param y 垂直半径距離
 *	@return 距離
 */
inline CValueDistance CalcEllipseDistance(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	const CRealNumber& xx = x.GetValue();
	const CRealNumber& yy = y.GetValue(code);
	CRealNumber r = _PAI * sqrt((xx * xx + yy * yy) * 2);
	return CValueDistance(r, code);
}

/**@ingroup VALUEUNIT
 * [計算] 楕円周囲長計算 (type2)
 *	@note	A = 半径１、B=半径２。　ただし、A > B とする。\n
 *			π・√(2 × (A^2 + B^2)) - (A - B)^2 ÷ 2.2
 *	@param x 水平半径距離
 *	@param y 垂直半径距離
 *	@return 距離
 */
inline CValueDistance CalcEllipseDistance_t2(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	CRealNumber A = x.GetValue();
	CRealNumber B = y.GetValue(code);
	if ( A < B )
	{
		Swap(A, B);
	}
	CRealNumber r = _PAI * sqrt((A * A + B * B) * 2 - (A - B) * (A - B) / 2.2);
	return CValueDistance(r, code);
}

/**@ingroup VALUEUNIT
 * [計算] 楕円周囲長計算 (type3)
 *	@note	A = 半径１、B=半径２。　ただし、A > B とする。\n
 *			2 × A × Π * (1 - (B^2 ÷ A^2)) ÷ 4 - (B^2 ÷ A^2))^2 × 3 ÷ 64 - (B^2 ÷ A^2))^3 × 5 ÷ 256)
 *	@param x 水平半径距離
 *	@param y 垂直半径距離
 *	@return 距離
 */
inline CValueDistance CalcEllipseDistance_t3(const CValueDistance& x, const CValueDistance& y)
{
	const CValueDistance::CCode& code = x.GetUnitCode();
	CRealNumber A = x.GetValue();
	CRealNumber B = y.GetValue(code);
	if ( A < B )
	{
		Swap(A, B);
	}
	if ( B == 0.0 )
	{
		return CValueDistance(A * 4.0, code);
	}
	CRealNumber a = 1 - (B * B) / (A * A);
	CRealNumber r = 2 * A * _PAI * (1 - a / 4 - (a * a) * 3 / 64 - (a * a * a) * 5 / 256);	
	return CValueDistance(r, code);
}



/**@ingroup VALUEUNIT
 * 単位管理(面積)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitArea : public CAbstractUnit
{
public:
	enum { RASING = 2 };///< 乗数値

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("面積"); }

	/// 単位種コード
	enum { KINDCODE = 'Ar' };

	/// 面積単位コード
	enum EUnitCode
	{
		M2,		///< 平方メートル
		A,		///< アール
		HA,		///< ヘクトアール
		INCH2,	///< 平方インチ
		YARD2,	///< 平方ヤード
		ACRE,	///< エーカー

		MM2		= CPfUnit::MILLI,	///< ミリ平方メートル
		CM2		= CPfUnit::CENTI,	///< センチ平方メートル
		KM2		= CPfUnit::KIRO,	///< キロ平方メートル
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
			//	単位文字列		日本語						係数
			{	_T("㎡"),		_T("平方メートル"),			1.0				},	// m2
			{	_T("a"),		_T("アール"),				0.01			},	// a
			{	_T("ha"),		_T("ヘクトアール"),			0.0001			},	// ha
			{	_T("in") _S2,	_T("平方インチ"),			1550.0			},	// inch2
			{	_T("yd") _S2,	_T("平方ヤード"),			1.19599			},	// yard2
			{	_T("acre"),		_T("エーカー"),				0.000247		},	// acre
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 面積管理クラス
 *
 *	@note 単位として {@link CUnitArea::EUnitCode 面積単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、平方メートル({@link CUnitArea::M2 CValueArea::M2}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3m2
 *		CValueArea d1(123.3); //コンストラクタで単位省略すると基本単位 (m2) になります。		
 *		TRACE1("%s\n", d1.ToString()); //m2で表示
 *		// 9.2μm2
 *		CValueArea d2(9.2, CValueArea::MICRO);
 *		TRACE1("%s\n", d2.ToString()); //μm2で表示
 *		// 9.2アール
 *		d2.Set(9.2, CValueArea::A);
 *		TRACE1("%s\n", d2.ToString(CValueArea::M2)); //m2で表示
 *		// 1.2エーカー
 *		d2.Set(1.2, CValueArea::ACRE);
 *		// 100エーカー
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueUnitT<CUnitArea> CValueArea;

/**@ingroup VALUEUNIT
 * [計算] 掛算 (面積 = 距離 × 距離) 
 *	@param d1 距離
 *	@param d2 距離
 *	@return 面積(単位は平方メートルで格納されます)。
 */
inline CValueArea operator*(const CValueDistance& d1, const CValueDistance& d2)
{
	return CValueArea(d1.GetValue(d1.M) * d2.GetValue(d2.M), CValueArea::M2);
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (距離 = 面積 ÷ 距離)
 *	@param a 面積
 *	@param d 距離
 *	@return 距離(単位はメートルで格納されます)。
 */
inline CValueDistance operator/(const CValueArea& a, const CValueDistance& d)
{
	return CValueDistance(a.GetValue(a.M2) / d.GetValue(d.M), CValueDistance::M);
}

/**@ingroup VALUEUNIT
 * [計算] 平方根 (距離 = √面積)
 *	@param a 面積
 *	@return 距離（単位はメートルになります）
 */
inline CValueDistance sqrt(const CValueArea& a)
{
	CRealNumber r = sqrt(a.GetValue(a.M2));
	return CValueDistance(r, CValueDistance::M);
}
	
/**@ingroup VALUEUNIT
 * [計算] 三角面積計算 (底辺 × 高さ ÷ 2)
 *	@param x 底辺距離
 *	@param y 高さ距離
 *	@return 面積（単位は ㎡ になります）
 */
inline CValueArea CalcTriangleArea(const CValueDistance& x, const CValueDistance& y)
{
	return x * y / 2.0;
}

/**@ingroup VALUEUNIT
 * [計算] 円面積計算 (π × 半径 × 半径)
 *	@param r 半径距離
 *	@return 面積（単位は m2 になります）
 */
inline CValueArea CalcCircleArea(const CValueDistance& r)
{
	return r * r * _PAI;
}

/**@ingroup VALUEUNIT
 * [計算] 楕円面積計算 (π × 水平半径 × 垂直半径)
 *	@param x 水平半径距離
 *	@param y 垂直半径距離
 *	@return 面積（単位は m2 になります）
 */
inline CValueArea CalcEllipseArea(const CValueDistance& x, const CValueDistance& y)
{
	return x * y * _PAI;
}



/**@ingroup VALUEUNIT
 * 単位管理(体積)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitCubic : public CAbstractUnit
{
public:
	enum { RASING = 3 };///< 乗数値

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("体積"); }

	/// 単位種コード
	enum { KINDCODE = 'Cu' };

	/// 体積単位コード
	enum EUnitCode
	{
		M3,		///< 立方メートル
		INCH3,	///< 立方インチ
		YARD3,	///< 立方ヤード
		BBL,	///< バレル
		LITRE,	///< リットル
		DL,		///< デシリットル
		CC,		///< cc

		MM3		= CPfUnit::MILLI,	///< ミリ立方メートル
		CM3		= CPfUnit::CENTI,	///< センチ立方メートル
		KM3		= CPfUnit::KIRO,	///< キロ立方メートル
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
			//	単位文字列		日本語						係数
			{	_M3,			_T("立方メートル"),			1.0						},	// m3
			{	_T("in") _S3,	_T("立方インチ"),			61023.6					},	// inch3
			{	_T("yd") _S3,	_T("立方ヤード"),			1.30795					},	// yard3
			{	_T("bbl"),		_T("バレル"),				6.28982					},	// bbl
			{	_LITRE,			_T("リットル"),				1000.0					},	// litre
			{	_DL,			_T("デシリットル"),			10000.0					},	// dl
			{	_T("cc"),		_T("cc"),					1000000.0				},	// cc
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 体積管理クラス
 *
 *	@note 単位として {@link CUnitCubic::EUnitCode 体積単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、立方メートル({@link CUnitCubic::M3 CValueCubic::M3}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3m3
 *		CValueCubic d1(123.3); //コンストラクタで単位省略すると基本単位 (m3) になります。		
 *		TRACE1("%s\n", d1.ToString()); //m3で表示
 *		// 9.2μm3
 *		CValueCubic d2(9.2, CValueCubic::MICRO);
 *		TRACE1("%s\n", d2.ToString()); //μm3で表示
 *		// 9.2リットル
 *		d2.Set(9.2, CValueCubic::LITRE);
 *		TRACE1("%s\n", d2.ToString(CValueCubic::M3)); //m3で表示
 *		// 1.2バレル
 *		d2.Set(1.2, CValueCubic::BBL);
 *		// 100バレル
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueUnitT<CUnitCubic> CValueCubic;

/**@ingroup VALUEUNIT
 * [計算] 掛算 (体積 = 面積 × 距離)
 *	@param a 面積
 *	@param d 距離
 *	@return 体(単位は平方メートルになります)
 */
inline CValueCubic operator*(const CValueArea& a, const CValueDistance& d)
{
	return CValueCubic(a.GetValue(a.M2) * d.GetValue(d.M), CValueCubic::M3);
}

/**@ingroup VALUEUNIT
 * [計算] 掛算 (体積 = 距離 × 面積)
 *	@param d 距離
 *	@param a 面積
 *	@return 体積(単位は平方メートルになります)
 */
inline CValueCubic operator*(const CValueDistance& d, const CValueArea& a)
{
	return a * d;
}



/**@ingroup VALUEUNIT
 * 単位管理(重量)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitWeight : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("重量"); }

	/// 単位種コード
	enum { KINDCODE = 'We' };

	/// 重量単位コード
	enum EUnitCode
	{
		G,				///< グラム
		T,				///< トン
		OZ,				///< オンス
		LB,				///< ポンド
		CT,				///< カラット
		KAN,			///< 貫
		MONME,			///< 匁

		MG		= CPfUnit::MILLI,	///< ミリグラム
		KG		= CPfUnit::KIRO,	///< キログラム
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
			{	_T("g"),		_T("グラム"),			1.0					},	// g
			{	_T("t"),		_T("トン"),				0.000001			},	// t
			{	_T("oz"),		_T("オンス"),			35.274 * 0.001		},	// oz
			{	_T("lb"),		_T("ポンド"),			2.20462291 * 0.001	},	// lb
			{	_T("ct"),		_T("カラット"),			1.0 / 0.2			},	// ct
			{	_T("貫"),		_T("貫"),				0.001 / 3.750		},	// kan
			{	_T("mon"),		_T("匁"),				1.0 / 3.750			},	//monme
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 重量管理クラス
 *
 *	@note 単位として {@link CUnitWeight::EUnitCode 重量単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、グラム({@link CUnitWeight::G CValueWeight::G}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3g
 *		CValueWeight d1(123.3); //コンストラクタで単位省略すると基本単位 (g) になります。		
 *		TRACE1("%s\n", d1.ToString()); //gで表示
 *		// 9.2ピコグラム
 *		CValueWeight d2(9.2, CValueWeight::PICO);
 *		TRACE1("%s\n", d2.ToString()); //pgで表示
 *		// 1.2カラット
 *		d2.Set(1.2, CValueWeight::CT);
 *		TRACE1("%s\n", d2.ToString(CValueWeight::G)); //gで表示
 *		// 100カラット
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //ct
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueUnitT<CUnitWeight> CValueWeight;



/**@ingroup VALUEUNIT
 * 単位管理(温度)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitTemperature : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("温度"); }

	/// 単位種コード
	enum { KINDCODE = 'Te' };

	/// 温度単位コード
	enum EUnitCode
	{
		C,				///< 摂氏(セルシウス)温度 ℃
		F,				///< 華氏温度
		K,				///< ケルビン

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
			//	単位文字列		日本語			係数		補正値
			{	_T("℃"),		_T("摂氏"),		1.0,		0		},	// C
			{	_T("゜F"),		_T("華氏"),		9.0 / 5.0,	32,		},	// F
			{	_T("K"),		_T("ケルビン"),	1.0,		273.15	},	// K
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 温度管理クラス
 *
 *	@note 単位として {@link CUnitTemperature::EUnitCode 温度単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、℃({@link CUnitTemperature::C CValueTemperature::C}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3℃
 *		CValueTemperature d1(123.3); //コンストラクタで単位省略すると基本単位 (℃) になります。		
 *		TRACE1("%s\n", d1.ToString()); //℃で表示
 *		// 9.2°F
 *		CValueTemperature d2(9.2, CValueTemperature::F);
 *		TRACE1("%s\n", d2.ToString()); //°Fで表示
 *		// 1.2ケルビン
 *		d2.Set(1.2, CValueTemperature::K);
 *		TRACE1("%s\n", d2.ToString(CValueTemperature::C)); //℃で表示
 *		// 100ケルビン
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); // Kで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueUnitT<CUnitTemperature> CValueTemperature;



/**@ingroup VALUEUNIT
 * 単位管理(時間)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitTime : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("時間"); }

	/// 単位種コード
	enum { KINDCODE = 'Ti' };

	/// 時間単位コード
	enum EUnitCode
	{
		SEC,				///< 秒
		MIN,				///< 分
		HOUR,				///< 時
		DAY,				///< 日
		WEEK,				///< 週

		MSEC		= CPfUnit::MILLI,	///< ミリ秒
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
			//	単位文字列		日本語			係数
			{	_T("s"),		_T("秒"),		1.0						},	// sec
			{	_T("m"),		_T("分"),		1.0 / 60.0				},	// min
			{	_T("h"),		_T("時"),		1.0 / 3600				},	// hour
			{	_T("day"),		_T("日"),		1.0 / (3600 * 24)		},	// day
			{	_T("week"),		_T("週"),		1.0 / (3600 * 24 * 7)	},	// week
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 時間管理クラス
 *
 *	@note 単位として {@link CUnitTime::EUnitCode 時間単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、秒({@link CUnitTime::SEC CValueTime::SEC}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3秒
 *		CValueTime d1(123.3); //コンストラクタで単位省略すると基本単位 (秒) になります。		
 *		TRACE1("%s\n", d1.ToString()); //秒で表示
 *		// 9.2ピコ秒
 *		CValueTime d2(9.2, CValueTime::PICO);
 *		TRACE1("%s\n", d2.ToString()); //ピコ秒で表示
 *		// 1.2WEEK
 *		d2.Set(1.2, CValueTime::WEEK);
 *		TRACE1("%s\n", d2.ToString(CValueTime::DAY)); //日で表示
 *		// 100WEEK
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //week
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueUnitT<CUnitTime> CValueTime;

/**@ingroup VALUEUNIT
 * 単位管理(時間^2)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitTime2 : public CAbstractUnit
{
public:
	enum { RASING = 2 };///< 乗数値

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("時間^2"); }

	/// 単位種コード
	enum { KINDCODE = 'Tt' };

	/// 時間^2単位コード
	enum EUnitCode
	{
		SEC2,				///< 秒^2
		MIN2,				///< 分^2
		HOUR2,				///< 時^2
		DAY2,				///< 日^2
		WEEK2,				///< 週^2

		MSEC2	= CPfUnit::MILLI,	///< ミリ秒^2
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
			//	単位文字列		日本語			係数
			{	_T("s^2"),		_T("秒^2"),		1.0								},	// sec
			{	_T("m^2"),		_T("分^2"),		(1.0 / 60.0) * (1.0 / 60.0)		},	// min
			{	_T("h^2"),		_T("時^2"),		(1.0 / 3600) * (1.0 / 3600)		},	// hour
			{	_T("day^2"),	_T("日^2"),		(1.0 / (3600 * 24))	* (1.0 / (3600 * 24))},	// day
			{	_T("week^2"),	_T("週^2"),		(1.0 / (3600 * 24 * 7)) * (1.0 / (3600 * 24 * 7))},	// week
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 時間^2管理クラス
 *
 *	@note 単位として {@link CUnitTime2::EUnitCode 時間^2単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、秒^2({@link CUnitTime2::SEC2 CValueTime2::SEC2}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3秒^2
 *		CValueTime2 d1(123.3); //コンストラクタで単位省略すると基本単位 (秒^2) になります。		
 *		TRACE1("%s\n", d1.ToString()); //秒^2で表示
 *		// 9.2ピコ秒^2
 *		CValueTime2 d2(9.2, CValueTime2::PICO);
 *		TRACE1("%s\n", d2.ToString()); //ピコ秒^2で表示
 *		// 1.2WEEK^2
 *		d2.Set(1.2, CValueTime2::WEEK2);
 *		TRACE1("%s\n", d2.ToString(CValueTime2::DAY2)); //日^2で表示
 *		// 100WEEK^2
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //week^2
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
typedef CValueUnitT<CUnitTime2> CValueTime2;

/**@ingroup VALUEUNIT
 * [計算] 掛算 (時間^2 = 時間 × 時間)
 *	@param t1 時間
 *	@param t2 時間
 *	@return 時間^2(単位はSEC^2になります)
 */
inline CValueTime2 operator*(const CValueTime& t1, const CValueTime& t2)
{
	return CValueTime2(t1.GetValue(t1.SEC) * t2.GetValue(t2.SEC), CValueTime2::SEC2);
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (時間 = 時間^2 ÷ 時間)
 *	@param t2 時間^2
 *	@param t 時間
 *	@return 時間(単位はSECになります)
 */
inline CValueTime operator/(const CValueTime2& t2, const CValueTime& t)
{
	return CValueTime(t2.GetValue(t2.SEC2) / t.GetValue(t.SEC), CValueTime::SEC);
}

/**@ingroup VALUEUNIT
 * [計算] 平方根 (時間 = √(時間^2))
 *	@param t2 時間^2
 *	@return 時間(単位はSECになります)
 */
inline CValueTime sqrt(const CValueTime2& t2)
{
	CRealNumber r = sqrt(t2.GetValue(t2.SEC2));
	return CValueTime(r, CValueTime::SEC);
}



/**@ingroup VALUEUNIT
 * 単位管理(周波数)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 06/12/26 新規作成
 */
class CUnitFrequency : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("周波数"); }

	/// 単位種コード
	enum { KINDCODE = 'Fr' };

	/// 周波数単位コード
	enum EUnitCode
	{
		HZ,				///< Hz

		KHZ		= CPfUnit::KIRO,	///< kHz
		MHZ		= CPfUnit::MEGA,	///< MHz
		GHZ		= CPfUnit::GIGA,	///< GHz
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
			//	単位文字列		日本語				係数
			{	_T("Hz"),		_T("ヘルツ"),		1.0			},	// Hz
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 周波数管理クラス
 *
 *	@note 単位として {@link CUnitFrequency::EUnitCode 周波数単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、Hz({@link CUnitFrequency::HZ CValueFrequency::HZ}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3Hz
 *		CValueFrequency d1(123.3); //コンストラクタで単位省略すると基本単位 (Hz) になります。		
 *		TRACE1("%s\n", d1.ToString()); //Hzで表示
 *		// 9.2kHz
 *		CValueFrequency d2(9.2, CValueFrequency::KHZ);
 *		TRACE1("%s\n", d2.ToString()); //kHzで表示
 *		// 1.2MHz
 *		d2.Set(1.2, CValueFrequency::MHZ);
 *		TRACE1("%s\n", d2.ToString(CValueFrequency::HZ)); //hzで表示
 *		// 100MHz
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //MHzで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 新規作成
 */
typedef CValueUnitT<CUnitFrequency> CValueFrequency;

/**@ingroup VALUEUNIT
 * [計算] 掛算 (値 = 周波数 × 時間)
 *	@param f 周波数
 *	@param t 時間
 *	@return 値
 */
inline CRealNumber operator*(const CValueFrequency& f, const CValueTime& t)
{
	return f.GetValue(f.HZ) * t.GetValue(t.SEC);
}

/**@ingroup VALUEUNIT
 * [計算] 掛算 (値 = 時間 × 周波数)
 *	@param t 時間
 *	@param f 周波数
 *	@return 値
 */
inline CRealNumber operator*(const CValueTime& t, const CValueFrequency& f)
{
	return f * t;
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (時間 = 値 ÷ 周波数)
 *	@param v 値
 *	@param f 周波数
 *	@return 時間(単位は秒になります)
 */
inline CValueTime operator/(const CRealNumber& v, const CValueFrequency& f)
{
	return CValueTime(v / f.GetValue(f.HZ), CValueTime::SEC);
}

/**@ingroup VALUEUNIT
 * [計算] 割算 (周波数 = 値 ÷ 時間)
 *	@param v 値
 *	@param t 時間単
 *	@return 周波数(単位はHzになります)
 */
inline CValueFrequency operator/(const CRealNumber& v, const CValueTime& t)
{
	return CValueFrequency(v / t.GetValue(t.SEC), CValueFrequency::HZ);
}



/**@ingroup VALUEUNIT
 * 単位管理(角度)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/18 新規作成
 *	@date 07/01/22 ラジアン追加
 */
class CUnitDegree : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("角度"); }

	/// 単位種コード
	enum { KINDCODE = 'De' };

	/// 角度単位コード
	enum EUnitCode
	{
		DEG,				///< 角度(度)
		DEGS,				///< 角度(秒)
		RAD,				///< ラジアン
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
			//	単位文字列		日本語			係数
			{	_T("°"),		_T("度"),		1.0			},	// DEG
			{	_T("\""),		_T("秒"),		60.0 * 60.0	},	// DEG(秒)
			{	_T(""),			_T(""),			_PAI / 180.0},	// RAD
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 角度管理クラス
 *
 *	@note 単位として {@link CUnitDegree::EUnitCode 角度単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、°(Degree)({@link CUnitDegree::DEG CValueDegree::DEG}) です。
 *	@note 本クラスのインスタンスは、三角関数「sin,sinh,cos,cosh,tan,tanh」に直接引数で渡すことが可能です
 *			（保持単位が角度の場合、ラジアン変換して計算します）。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3°
 *		CValueDegree d1(123.3); //コンストラクタで単位省略すると基本単位 (°) になります。		
 *		TRACE1("%s\n", d1.ToString()); //°で表示
 *		TRACE1("%f\n", sin(d1));		// sin(123.3°)を計算
 *		// 3453.4秒
 *		CValueDegree d2(2453.4, CValueDegree::DEGS);
 *		TRACE1("%s\n", d2.ToString()); //秒で表示
 *		// 1.2（RAD）
 *		d2.Set(1.2, CValueDegree::RAD);
 *		TRACE1("%s\n", d2.ToString(CValueDegree::DEG)); //°で表示
 *		// .5(RAD)
 *		d2.Set(.5); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //ラジアンで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 新規作成
 */
typedef CValueUnitT<CUnitDegree> CValueDegree;

/**@ingroup VALUEUNIT
 * [計算] sin (値 = sin(角度))
 *	@param d 角度
 *	@return 値
 */
inline CRealNumber sin(const CValueDegree& d) { return sin(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [計算] sinh (値 = sinh(角度))
 *	@param d 角度
 *	@return 値
 */
inline CRealNumber sinh(const CValueDegree& d) { return sinh(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [計算] cos (値 = cos(角度))
 *	@param d 角度
 *	@return 値
 */
inline CRealNumber cos(const CValueDegree& d) { return cos(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [計算] cosh (値 = cosh(角度))
 *	@param d 角度
 *	@return 値
 */
inline CRealNumber cosh(const CValueDegree& d) { return cosh(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [計算] tan (値 = tan(角度))
 *	@param d 角度
 *	@return 値
 */
inline CRealNumber tan(const CValueDegree& d) { return tan(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [計算] tanh (値 = tanh(角度))
 *	@param d 角度
 *	@return 値
 */
inline CRealNumber tanh(const CValueDegree& d) { return tanh(d.GetValue(d.RAD)); }

/**@ingroup VALUEUNIT
 * [計算] 角度(度,分,秒)代入.
 *	@note 負の角度を入れる場合、ddを負にしてください。
 *	@param[in] dd 度
 *	@param[in] mm 分
 *	@param[in] ss 秒
 *	@return 角度
 */
inline CValueDegree MakeDegree(int dd, int mm = 0, double ss = 0.0)
{
	if ( ss < 0.0 )
	{
		ss = -ss;
	}
	double r = abs(dd) * 60 * 60 + abs(mm * 60) + ss;
	if ( dd < 0 )
	{
		r = -r;
	}
	return CValueDegree(r, CValueDegree::DEGS);
}

/**@ingroup VALUEUNIT
 * [計算] 角度(度,分,秒)計算.
 *	@param[in] dd 度
 *	@param[in] mm 分
 *	@param[in] ss 秒
 *	@return 角度
 */
inline CValueDegree MakeDegree(BYTE dd, BYTE mm, double ss)
{
	double r = dd * 60 * 60 + mm * 60 + ss;
	return CValueDegree(r, CValueDegree::DEGS);
}

/**@ingroup VALUEUNIT
 * [計算] 直角三角形の高さ計算
 *	@param di 底辺長さ
 *	@param de 角度（底辺に接する直角以外の角度）
 *	@return 直角三角形の高さ
 */
inline CValueDistance CalcRightTriangleHeight(const CValueDistance& di, const CValueDegree& de)
{
	CValueDistance::CCode c = di.GetUnitCode();
	return CValueDistance(di.GetValue(c) * tan(de), c);
}

/**@ingroup VALUEUNIT
 * [計算] 角度の平均計算.
 *	@note 単位ベクトル合算法による計算。
 *	@param size 配列数
 *	@param pDegs 角度単位の配列
 *	@return 平均角 (単位 DEG)
 */
inline CValueDegree CalcDegreeAverage(size_t size, const CValueDegree* pDegs)
{
	CRealNumber x = 0;
	CRealNumber y = 0;
	loop ( i, size )
	{
		x += sin(pDegs[i]);
		y += cos(pDegs[i]);
	}
	CValueDegree d;
	d.Set(atan2(x, y), CValueDegree::RAD);
	return d;
}

/**@ingroup VALUEUNIT
 * [計算] 角度の平均計算.
 *	@note 単位ベクトル合算法による計算。
 *	@param d1 角度１
 *	@param d2 角度２
 *	@param r1 角度１の比率。省略すると 1.0。
 *	@param r2 角度２の比率。省略すると 1.0。
 *	@return 平均角
 */
inline CValueDegree CalcDegreeAverage(const CValueDegree& d1, const CValueDegree& d2, double r1 = 1.0, double r2 = 1.0)
{
	CValueDegree::CCode c = d1.GetUnitCode();
	CRealNumber x = sin(d1) * r1 + sin(d2) * r2;
	CRealNumber y = cos(d1) * r1 + cos(d2) * r2;
	CValueDegree d;
	d.Set(atan2(x, y), CValueDegree::RAD);
	d.ToUnit(c);
	return d;
}

/**@ingroup VALUEUNIT
 * [計算] 角度の開き計算.
 *	@note 必ず０～180°になります。
 *	@param d1 角度１
 *	@param d2 角度２
 *	@return 開き角度(単位 DEG)
 */
inline CValueDegree CalcDegreeDifference(const CValueDegree& d1, const CValueDegree& d2)
{
	CRealNumber v1 = d1.GetValue(CValueDegree::DEG);
	CRealNumber v2 = d2.GetValue(CValueDegree::DEG);
	CValueDegree d;
	v1 = fmod(v1 + 360.0 * 1000.0, 360.0);
	v2 = fmod(v2 + 360.0 * 1000.0, 360.0);
	v1 = fabs(v1 - v2);
	if ( v1 > 180.0 )
	{
		v1 = 360.0 - v1;
	}
	d.Set(v1, CValueDegree::DEG);
	return d;
}



/**@ingroup VALUEUNIT
 * 単位管理(圧力)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/18 新規作成
 */
class CUnitPressure : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("圧力"); }

	/// 単位種コード
	enum { KINDCODE = 'Pr' };

	/// 圧力単位コード
	enum EUnitCode
	{
		MMHG,		///< mmHg
		TORR,		///< Torr
		INHG,		///< inHg
		PA,			///< パスカル
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
			//	単位文字列		日本語						係数
			{	_T("mmHg"),		_T("水銀柱ミリメートル"),	1.0			},	// mmHg
			{	_T("Torr"),		_T("トル"),					1.0			},	// Torr
			{	_T("inHg"),		_T("水銀柱インチ"),			3.937E-02	},	// inHg
			{	_T("Pa"),		_T("パスカル"),				133.3224	},	// パスカル
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 圧力管理クラス
 *
 *	@note 単位として {@link CUnitPressure::EUnitCode 圧力単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、水銀柱mm({@link CUnitPressure::MMHG CValuePressure::MMHG}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3水銀柱mm
 *		CValuePressure d1(123.3); //コンストラクタで単位省略すると基本単位 (水銀柱mm) になります。		
 *		TRACE1("%s\n", d1.ToString()); //水銀柱mmで表示
 *		// 9.2Torr
 *		CValuePressure d2(9.2, CValuePressure::Torr);
 *		TRACE1("%s\n", d2.ToString()); //Torrで表示
 *		// 1.2Pa
 *		d2.Set(1.2, CValuePressure::PA);
 *		TRACE1("%s\n", d2.ToString(CValuePressure::MMHG)); //mmHgで表示
 *		// 100pa
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //paで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 新規作成
 */
typedef CValueUnitT<CUnitPressure> CValuePressure;



/**@ingroup VALUEUNIT
 * 単位管理(割合)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/18 新規作成
 */
class CUnitRatio : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("割合"); }

	/// 単位種コード
	enum { KINDCODE = 'Re' };

	/// 割合単位コード
	enum EUnitCode
	{
		PERCENT,				///< パーセント
		WARI,					///< 割
		DEC,					///< 小数点
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
			//	単位文字列		日本語				係数
			{	_T("%"),		_T("パーセント"),	1.0			},	// °
			{	_T("割"),		_T("割"),			0.1			},	// °
			{	_T(""),			_T(""),				0.01		},	// °
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 割合管理クラス
 *
 *	@note 単位として {@link CUnitRatio::EUnitCode 割合単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、パーセント({@link CUnitRatio::PERCENT CValueRatio::PERCENT}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3％
 *		CValueRatio d1(123.3); //コンストラクタで単位省略すると基本単位 (％) になります。		
 *		TRACE1("%s\n", d1.ToString()); //％で表示
 *		// 9.2割
 *		CValueRatio d2(9.2, CValueRatio::WARI);
 *		TRACE1("%s\n", d2.ToString()); //割で表示
 *		// 1.2
 *		d2.Set(1.2, CValueRatio::DEC);
 *		TRACE1("%s\n", d2.ToString(CValueRatio::PERCENT)); //％で表示
 *		// 100.0
 *		d2.Set(100.0); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); // 100.0 と表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/10 新規作成
 */
typedef CValueUnitT<CUnitRatio> CValueRatio;



/**@ingroup VALUEUNIT
 * 単位管理(電流)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 新規作成
 *	@date 09/02/10 Bi 追加
 */
class CUnitAmpere : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("電流"); }

	/// 単位種コード
	enum { KINDCODE = 'A' };

	/// 電流単位コード
	enum EUnitCode
	{
		A,				///< A (アンペア)
		BI,				///< Bi (ビオ)
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
			//	単位文字列		日本語				係数
			{	_T("A"),		_T("アンペア"),		1.0			},	// A
			{	_T("Bi"),		_T("ビオ"),			0.1			},	// Bi
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 電流管理クラス
 *
 *	@note 単位として {@link CUnitAmpere::EUnitCode 電流単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、アンペア({@link CUnitAmpere::A CValueAmpere::A}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3A
 *		CValueAmpere d1(123.3); //コンストラクタで単位省略すると基本単位 (A) になります。		
 *		TRACE1("%s\n", d1.ToString()); //Aで表示
 *		// 9.2μA
 *		CValueAmpere d2(9.2, CValueAmpere::MICRO);
 *		TRACE1("%s\n", d2.ToString()); //μAで表示
 *		// 0.2Bi
 *		d2.Set(0.2, CValueAmpere::BI);
 *		TRACE1("%s\n", d2.ToString(CValueAmpere::A)); //Aで表示
 *		// 100Bi
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //Biで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 新規作成
 */
typedef CValueUnitT<CUnitAmpere> CValueAmpere;



/**@ingroup VALUEUNIT
 * 単位管理(電圧)クラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 新規作成
 */
class CUnitVolt : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T("電圧"); }

	/// 単位種コード
	enum { KINDCODE = 'V' };

	/// 電圧単位コード
	enum EUnitCode
	{
		V,				///< V (ボルト)

		MV		= CPfUnit::MILLI,	///< mV (ミリボルト)
		KV		= CPfUnit::KIRO,	///< kV (キロボルト)
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
			//	単位文字列		日本語				係数
			{	_T("V"),		_T("ボルト"),		1.0			},	// V
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 電圧管理クラス
 *
 *	@note 単位として {@link CUnitVolt::EUnitCode 電圧単位コード} と 
 *					{@link CPfUnit::EPfUnitCode 接頭単位コード} が使用できます。
 *	@note {@link CPfUnit::EPfUnitCode 接頭単位コード}は、基準単位を基準にします。
 *	@note 基準単位は、ボルト({@link CUnitVolt::V CValueVolt::V}) です。
 *
 *	@see 使用できるメソッドは CValueUnitT を参照してください。
 *
 *	@par使用例
 *		\code
 *		// 123.3V
 *		CValueVolt d1(123.3); //コンストラクタで単位省略すると基本単位 (V) になります。		
 *		TRACE1("%s\n", d1.ToString()); //Vで表示
 *		// 9.2ピコV
 *		CValueVolt d2(9.2, CValueVolt::PICO);
 *		TRACE1("%s\n", d2.ToString(CValueVolt::MV)); //MVで表示
 *		// 100ピコV
 *		d2.Set(100); //単位を省略すると数値だけの変更になります。
 *		TRACE1("%s\n", d2.ToString()); //ピコVで表示
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/11 新規作成
 */
typedef CValueUnitT<CUnitVolt> CValueVolt;



/**@ingroup VALUEUNIT
 * 単位管理(ダミー)テンプレートクラス
 *
 *	@attention 単位管理用のクラスです。直接使うケースはありません。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/22 新規作成
 */
class CUnitDummy : public CAbstractUnit
{
public:

	/**
	 * [取得] 単位種文字列取得
	 *	@return 単位種文字列
	 */
	static LPCTSTR GetKindString(void) { return _T(""); }

	/// 単位種コード
	enum { KINDCODE = ' ' * 0x100 };

	/// 電圧単位コード
	enum EUnitCode
	{
		NON,				///< 単位なし
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
			//	単位文字列		日本語				係数
			{	_T(""),			_T(""),				1.0			},
		};
		if ( code >= 0 && code < countof(A) )
		{
			return &A[code];
		}
		throw CNoSuchTypeException();
	}
};

/**@ingroup VALUEUNIT
 * 単位なし管理クラス
 *
 *	@note	単位なしの値を扱いたい時に使用します。
 *
 *	@par必要ファイル
 *			TnbValueUnit.h
 * 
 *	@date 07/01/22 新規作成
 */
typedef CValueUnitT<CUnitDummy> CValueNonUnit;



#ifndef _TnbDOXYGEN	//Document作成用シンボル
	#undef _S2		
	#undef _M3		
	#undef _S3		
	#undef _LITRE	
	#undef _DL		
#endif



}; // TNB
