#pragma once
/**
 *@file
 * 実数管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



#include <float.h>
#include <math.h>



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * 実数管理クラス
 *
 *		double と状態とパラメータを持ちます。状態は値なし、無効値、エラー状態を保持します。
 *		パラメータは16個のフラグです。
 *
 *		実数管理クラス同士を計算すると、状態とパラメータも計算されます。たとえば、
 *		無効な値と他の状態の値を計算すると、結果は無効な値になります。
 *		パラメータは、 or され、結果とします。
 *
 *	@note パラメータは本クラスでは or 計算する以外、使用していません。
 *			使用するプロジェクトで任意です。
 *
 *	@par対応関数
 *		<table><tr><td><pre>
 *		CRealNumber	acos(CRealNumber _X);
 *		CRealNumber	asin(CRealNumber _X);
 *		CRealNumber	atan(CRealNumber _X);
 *		CRealNumber	atan2(CRealNumber _Y, CRealNumber _X);
 *		CRealNumber	cos(CRealNumber _X);
 *		CRealNumber	cosh(CRealNumber _X);
 *		CRealNumber	exp(CRealNumber _X);
 *		CRealNumber	fabs(CRealNumber _X);
 *		CRealNumber	fmod(CRealNumber _X, CRealNumber _Y);
 *		CRealNumber	log(CRealNumber _X);
 *		CRealNumber	log10(CRealNumber _X);
 *		CRealNumber	pow(CRealNumber _X, CRealNumber _Y);
 *		CRealNumber	sin(CRealNumber _X);
 *		CRealNumber	sinh(CRealNumber _X);
 *		CRealNumber	tan(CRealNumber _X);
 *		CRealNumber	tanh(CRealNumber _X);
 *		CRealNumber	sqrt(CRealNumber _X);
 *		CRealNumber	ceil(CRealNumber _X);
 *		CRealNumber	floor(CRealNumber _X);
 *		CRealNumber	frexp(CRealNumber _X, __out int * _Y);
 *		CRealNumber	_hypot(CRealNumber _X, CRealNumber _Y);
 *		CRealNumber	_j0(CRealNumber _X );
 *		CRealNumber	_j1(CRealNumber _X );
 *		CRealNumber	_jn(int _X, CRealNumber _Y);
 *		CRealNumber	ldexp(CRealNumber _X, int _Y);
 *		CRealNumber	modf(CRealNumber _X, __out CRealNumber * _Y);
 *		CRealNumber	_y0(CRealNumber _X);
 *		CRealNumber	_y1(CRealNumber _X);
 *		CRealNumber	_yn(int _X, CRealNumber _Y);
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbRealNumber.h
 *
 *	@date 06/11/18	新規作成
 */
class CRealNumber
{
public:

	/// 状態
	enum EStatus
	{
		ES_Valid,				///< 有効状態
		ES_Empty,				///< Empty状態
		ES_Invalid,				///< 無効状態
		ES_ZeroDivied,			///< Zero Divied状態
		ES_OverFlow,			///< Overflow状態
	};

private:

	double	m_doValue;			///< 実際の値
	WORD	m_wStatus;			///< 値の状態情報
	WORD	m_wParam;			///< 値の付加情報

	/**
	 * [確認] 値チェック.
	 *		指定された値をチェックし、状態を再設定します。
	 *	@param[in] value 値。
	 *	@param[in,out] _status 状態。
	 */
	static void ms_CheckValue(double value, WORD& _status)
	{
		if ( _status == ES_Valid )
		{
			if ( ::_isnan(value) != 0 )
			{
				//異常な値だ
				_status = ES_Invalid;
			}
			else if ( ::_finite(value) == 0 )
			{
				//オーバーフローを入れておく
				_status = ES_OverFlow;
			}
		}
	}

	/**
	 * [計算] 二つの状態計算.
	 *		右辺と左辺の状態を計算し、一つにします。
	 *	@param left 左辺の状態
	 *	@param right 右辺の状態
	 *	@return あわせ込んだ状態。
	 */
	static WORD ms_CalcStatus(WORD left, WORD right)
	{
		if ( left == ES_Empty || right == ES_Empty )
		{
			// 片方でもエンプティならエンプティ
			left = ES_Empty;
		}
		else if ( left == ES_Invalid || right == ES_Invalid )
		{
			// 片方でも不正なら結果は不正
			left = ES_Invalid;
		}
		else if ( left == ES_Valid )
		{
			//左辺が有効なら結果は右辺の状態
			left = right;
		}
		else if ( right == ES_Valid )
		{
			//右辺が有効なら結果は左辺の状態
		}
		else
		{
			//何らかのエラーだ
			left = ES_Invalid;
		}
		return left;
	}

	/**
	 * [確認] 値チェック.
	 *		指定された値をチェックし、状態を再設定します。
	 */
	void m_CheckValue(void)
	{
		ms_CheckValue(m_doValue, m_wStatus);
	}

	/**
	 * コンストラクタ
	 *	@note 指定の状態になります。
	 */
	CRealNumber(double value, int status, int param) 
		: m_doValue(value), m_wStatus(static_cast<WORD>(status)), m_wParam(static_cast<WORD>(param))
	{
	}

public:

	/**
	 * コンストラクタ
	 *	@note 空状態になります。
	 */
	CRealNumber(void) : m_doValue(0), m_wStatus(ES_Empty), m_wParam(0)
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元。
	 */
	CRealNumber(const CRealNumber& other) 
		: m_doValue(other.m_doValue), m_wStatus(other.m_wStatus), m_wParam(other.m_wParam)
	{
	}

	/**
	 * 代入コンストラクタ
	 *	@note 指定の値を保持します。
	 *	@param value 値。
	 */
	CRealNumber(double value) : m_doValue(value), m_wStatus(ES_Valid), m_wParam(0)
	{
		m_CheckValue();
	}

	/**
	 * 代入コンストラクタ
	 *	@note 指定の値を保持します。
	 *	@param value 値。
	 */
	CRealNumber(int value) : m_doValue(value), m_wStatus(ES_Valid), m_wParam(0)
	{
	}

	/**
	 * 代入コンストラクタ.
	 *	@note 文字列を数値に変換した値を保持します。Empty文字列なら、空状態になります。
	 *	@param str 文字列。
	 */
	CRealNumber(const CStr& str) : m_doValue(0), m_wStatus(ES_Empty), m_wParam(0)
	{
		if ( ! str.IsEmpty() )
		{
			m_doValue = str.ToDouble();
			m_wStatus = ES_Valid;
			m_CheckValue();
		}
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元。
	 *	@return 自分の参照。
	 */
	CRealNumber& operator=(const CRealNumber& other)
	{
		m_doValue = other.m_doValue;
		m_wStatus = other.m_wStatus;
		m_wParam = other.m_wParam;
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@note 指定の値を保持します。
	 *	@param value 値。
	 *	@return 自分の参照。
	 */
	CRealNumber& operator=(double value)
	{
		m_doValue = value;
		m_wStatus = ES_Valid;
		m_wParam = 0;
		m_CheckValue();
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@note 指定の値を保持します。
	 *	@param value 値。
	 *	@return 自分の参照。
	 */
	CRealNumber& operator=(int value)
	{
		m_doValue = value;
		m_wStatus = ES_Valid;
		m_wParam = 0;
		m_CheckValue();
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@note 文字列を数値に変換した値を保持します。Empty文字列なら、空状態になります。
	 *	@param str 文字列
	 *	@return 自分の参照。
	 */
	CRealNumber& operator=(const CStr& str)
	{
		if ( ! str.IsEmpty() )
		{
			m_doValue = str.ToDouble();
			m_wStatus = ES_Valid;
			m_CheckValue();
		}
		else
		{
			Empty();
		}
	}

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	//---------------------------------
	//
	//	代入系チーム
	//
	#define FUNCMAC2(EQ, DST, VL)										\
		CRealNumber& operator EQ (DST){									\
			m_doValue EQ VL.m_doValue;									\
			m_wStatus = ms_CalcStatus(m_wStatus, VL.m_wStatus);			\
			m_wParam |= VL.m_wParam;									\
			m_CheckValue();												\
			return *this;												\
		}

	#define FUNCMAC1(DST, VL)											\
		FUNCMAC2( +=, DST, VL );										\
		FUNCMAC2( -=, DST, VL );										\
		FUNCMAC2( *=, DST, VL );										\
		CRealNumber& operator/=(DST) {									\
			if ( VL.m_doValue != 0.0 ) {								\
				m_doValue /= VL.m_doValue;								\
				m_wStatus = ms_CalcStatus(m_wStatus, VL.m_wStatus);		\
				m_wParam |= VL.m_wParam;								\
			} else {													\
				m_doValue = 0;											\
				if ( VL.m_wStatus == ES_Empty ) {						\
					m_wStatus = ms_CalcStatus(m_wStatus, ES_Empty);		\
				} else {												\
					m_wStatus = ms_CalcStatus(m_wStatus, ES_ZeroDivied);\
				}														\
			}															\
			m_CheckValue();												\
			return *this;												\
		}

	FUNCMAC1( const CRealNumber& value, value );
	FUNCMAC1( double doValue, CRealNumber(doValue) );
	FUNCMAC1( int iValue, CRealNumber(iValue) );

	#undef FUNCMAC1
	#undef FUNCMAC2

	//-------------------------------------
	//
	//	演算の花道
	//	
	#define FUNCMAC2(EQ, DST, VL)										\
		CRealNumber	operator EQ (DST) const {							\
			CRealNumber r = *this;										\
			r EQ##= VL;													\
			return r;													\
		}

	#define FUNCMAC1(DST, VL)											\
		FUNCMAC2( +, DST, VL )											\
		FUNCMAC2( -, DST, VL )											\
		FUNCMAC2( *, DST, VL )											\
		FUNCMAC2( /, DST, VL )

	FUNCMAC1( const CRealNumber& value, value );
	FUNCMAC1( double doValue, CRealNumber(doValue) );
	FUNCMAC1( int iValue, CRealNumber(iValue) );

	#undef FUNCMAC1
	#undef FUNCMAC2

	#define FUNCMAC2(EQ,DST,VL)											\
		friend CRealNumber operator EQ (DST, const CRealNumber& value){	\
			CRealNumber r = VL;											\
			r EQ##= value;												\
			return r;													\
		}//---------------------------------------------------------------

	#define FUNCMAC1(DST, VL)											\
		FUNCMAC2( +, DST, VL );											\
		FUNCMAC2( -, DST, VL );											\
		FUNCMAC2( *, DST, VL );											\
		FUNCMAC2( /, DST, VL );

	FUNCMAC1( double doValue, doValue )
	FUNCMAC1( int iValue, iValue )

	#undef FUNCMAC1
	#undef FUNCMAC2

	//---------------------------------------------
	//
	//	関数
	//
	#define FUNCMAC1(CMD)											\
		friend CRealNumber CMD(const CRealNumber& value) {			\
			if ( value.m_wStatus == ES_Valid ) {					\
				CRealNumber r(::CMD(value.m_doValue),				\
								value.m_wStatus, value.m_wParam);	\
				r.m_CheckValue();									\
				return r;											\
			}														\
			return CRealNumber(0, value.m_wStatus, value.m_wParam);	\
		}															\
		friend double CMD(double value) { return ::CMD(value); }

	FUNCMAC1( acos );
	FUNCMAC1( asin );
	FUNCMAC1( atan );
	FUNCMAC1( cos );
	FUNCMAC1( cosh );
	FUNCMAC1( exp );
	FUNCMAC1( fabs );
	FUNCMAC1( log );
	FUNCMAC1( log10 );
	FUNCMAC1( sin );
	FUNCMAC1( sinh );
	FUNCMAC1( tan );
	FUNCMAC1( tanh );
	FUNCMAC1( sqrt );
	FUNCMAC1( ceil );
	FUNCMAC1( floor );
	FUNCMAC1( _j0 );
	FUNCMAC1( _j1 );
	FUNCMAC1( _y0 );
	FUNCMAC1( _y1 );

	#undef FUNCMAC1

	#define FUNCMAC1(CMD, DSP, VL)										\
		friend CRealNumber CMD(const CRealNumber& value, DSP) {			\
			double d = 0;												\
			if ( value.m_wStatus == ES_Valid && VL.m_wStatus == ES_Valid ){\
				d = ::CMD(value.m_doValue, VL.m_doValue);				\
			}															\
			CRealNumber r(d, ms_CalcStatus(value.m_wStatus, VL.m_wStatus),\
							value.m_wParam|VL.m_wParam);				\
			r.m_CheckValue();											\
			return r;													\
		}

	#define FUNCMAC2(CMD)												\
		FUNCMAC1( CMD, const CRealNumber& value2, value2 );				\
		FUNCMAC1( CMD, double doValue, CRealNumber(doValue) );			\
		FUNCMAC1( CMD, int iValue, CRealNumber(iValue) );				\
		friend double CMD(double v1, double v2) { return ::CMD(v1, v2); }

	FUNCMAC2( atan2 );
	FUNCMAC2( fmod );
	FUNCMAC2( pow );
	FUNCMAC2( _hypot );

	#undef FUNCMAC2
	#undef FUNCMAC1

	friend CRealNumber frexp(const CRealNumber& value, int* _Y)
	{
		CRealNumber r(::frexp(value.m_doValue, _Y), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double frexp(double value, int* _Y)
	{
		return ::frexp(value, _Y);
	}

	friend CRealNumber _jn(int _X, const CRealNumber& value)
	{
		CRealNumber r(::_jn(_X, value.m_doValue), value.m_wStatus,value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double _jn(int _X, double value)
	{
		return ::_jn(_X, value);
	}

	friend CRealNumber ldexp(const CRealNumber& value, int iValue)
	{
		CRealNumber r(::ldexp(value.m_doValue, iValue), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double ldexp(double value, int iValue)
	{
		return ::ldexp(value, iValue);
	}

	friend CRealNumber modf(const CRealNumber& value, double* _Y)
	{
		CRealNumber r(::modf(value.m_doValue, _Y), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double modf(double value, double* _Y)
	{
		return ::modf(value, _Y);
	}

	friend CRealNumber _yn(int _X, const CRealNumber& value)
	{
		CRealNumber r(::_yn(_X, value.m_doValue), value.m_wStatus, value.m_wParam);
		r.m_CheckValue();
		return r;
	}

	friend double _yn(int _X, double value)
	{
		return ::_yn(_X, value);
	}

	//---------------------------------------------
	//
	//	比較(有)
	//
	#define FUNCMAC2(EQ,TY,VL)										\
		friend bool operator EQ(TY VL##1, TY VL##2) {				\
			if ( VL##1.m_wStatus != ES_Valid ){ return false; }		\
			if ( VL##2.m_wStatus != ES_Valid ){ return false; }		\
			return VL##1.m_doValue EQ VL##2.m_doValue;				\
		}															\
		friend bool operator EQ(TY VL, double d) {					\
			return operator EQ(VL, CRealNumber(d));					\
		}															\
		friend bool operator EQ(TY VL, int d) {						\
			return operator EQ(VL, CRealNumber(d));					\
		}															
	#define FUNCMAC1(TY, VL)										\
		friend bool operator==(TY VL##1, TY VL##2) {				\
			if ( VL##1.m_wStatus != VL##2.m_wStatus ){ return false; }	\
			if ( VL##1.m_wStatus != ES_Valid && VL##1.m_wStatus == VL##2.m_wStatus ){ return true; }\
			return VL##1.m_doValue == VL##2.m_doValue;				\
		}															\
		friend bool operator==(TY VL, double d) {					\
			return operator==(VL, CRealNumber(d));					\
		}															\
		friend bool operator==(TY VL, int d) {						\
			return operator==(VL, CRealNumber(d));					\
		}															\
		friend bool operator!=(TY VL##1, TY VL##2) {				\
			return ! operator==(VL##1, VL##2);						\
		}															\
		friend bool operator!=(TY VL, double d) {					\
			return operator!=(VL, CRealNumber(d));					\
		}															\
		friend bool operator!=(TY VL, int d) {						\
			return operator!=(VL, CRealNumber(d));					\
		}															\
		FUNCMAC2( <, TY, VL );										\
		FUNCMAC2( <=, TY, VL );										\
		FUNCMAC2( >, TY, VL );										\
		FUNCMAC2( >=, TY, VL );

	FUNCMAC1( const CRealNumber&, value );

	#undef FUNCMAC1
	#undef FUNCMAC2

	#endif // _TnbDOXYGEN

	//------------------------------------------


	/**
	 * [取得] double型取得.
	 *		doubleの形式に抽出します。
	 *	@return 値
	 */
	double ToDouble(void) const
	{
		#ifdef _DEBUG
			if ( m_wStatus != ES_Valid )
			{
				ASSERT1(false, "CRealNumber::ToDouble()", "状態が VALID(0)以外です (r=%u) 。", m_wStatus);
				return 0;
			}
		#endif
		return m_doValue;
	}

	/**
	 * [取得] double型取得.
	 *		doubleの形式に抽出します。
	 *	@return 値
	 */
	operator double() const
	{
		return ToDouble();
	}

	/**
	 * [取得] int型取得.
	 *		intの形式に抽出します。
	 *	@return 値
	 */
	int ToInt(void) const
	{
		return TNB::ToInt(ToDouble());
	}

	/**
	 * [設定] Empty.
	 *		EMPTY 状態にします。
	 */
	void Empty(void)
	{
		m_doValue = 0;
		m_wStatus = ES_Empty;
		m_wParam = 0;
	}

	/**
	 * [確認] 有効確認
	 *	@retval true 値有効
	 *	@retval false 有効以外
	 */
	bool IsValid(void) const
	{
		return m_wStatus == ES_Valid;
	}

	/**
	 * [確認] EMPTY確認
	 *	@retval true EMPTY
	 *	@retval false EMPTY以外
	 */
	bool IsEmpty(void) const
	{
		return m_wStatus == ES_Empty;
	}

	/**
	 * [確認] 状態確認
	 *	@return 状態
	 */
	EStatus GetStatus(void) const
	{
		return static_cast<EStatus>(m_wStatus);
	}

	/**
	 * [設定] 状態設定
	 *	@note 値は０になります。パラメータは変化しません。
	 *	@param status 状態
	 */
	void SetStatus(EStatus status)
	{
		m_doValue = 0;
		m_wStatus = static_cast<WORD>(status);
	}

	/**
	 * [取得] パラメータ取得
	 *	@return パラメータ
	 */
	WORD GetParam(void) const
	{
		return m_wParam;
	}

	/**
	 * [取得] パラメータ参照
	 *	@note インスタンス内のパラメータの参照が得られます。変更も可能です。
	 *	@return パラメータ
	 */
	WORD& RefParam(void)
	{
		return m_wParam;
	}

	/**
	 * [設定] パラメータ設定
	 *	@param wParam パラメータ
	 */
	void SetParam(WORD wParam)
	{
		m_wParam = wParam;
	}

	/**
	 * [確認] 比較
	 *	@note 値が同じか、調べます。他、 == , != , <= などの比較式も使用可能です。 
	 *	@param r 比較対象値。
	 *	@retval true 同じ。
	 *	@retval true 異なる。
	 */
	bool IsEqual(const CRealNumber& r) const
	{
		return *this == r;
	}

	/**
	 * [確認] あいまい比較. 
	 *		値がほぼ同じか、調べます。
	 *	@note 実際の値を曖昧度で割り、小数点に以下を削除し比較しています。
	 *	@param r 比較対象値。
	 *	@param doDepth 曖昧度。 省略すると 0.01 が指定されます。つまり、小数点第２までで比較。
	 *	@retval true 同じ。
	 *	@retval true 異なる。
	 */
	bool IsAlmostEqual(const CRealNumber& r, double doDepth = 0.01) const
	{
		if ( m_wStatus != r.m_wStatus ){ return false; }
		if ( m_wStatus != ES_Valid && m_wStatus == r.m_wStatus ){ return true; }
		//TRACE2("IsAlmostEqual(%f,%f)\n",::floor(m_doValue * doDepth + 0.5) ,::floor(r.m_doValue * doDepth + 0.5));
		return ::floor(m_doValue / doDepth + 0.5) == ::floor(r.m_doValue / doDepth + 0.5);
	}

	/**
	 * [取得] 文字列作成
	 *	@param lpszFormat sprintf()のフォーマット。 ％f を必ず入れること。
	 *	@return 文字列。
	 */
	CStr ToString(LPCTSTR lpszFormat = NULL) const
	{
		CStr s;
		switch ( m_wStatus )
		{
		case ES_Valid: // 有効な値を保持
			if ( lpszFormat == NULL || lpszFormat[0] == 0 )
			{
				lpszFormat = _T("%f");
			}
			s.Format(lpszFormat, m_doValue);
			break;
		case ES_Empty: // 値なし
			break;
		case ES_Invalid: // 無効な値を保持
			s = _T("-Invalid-");
			break;
		case ES_ZeroDivied: // Zero Divied状態
			s = _T("-ZeroDivied-");
			break;
		case ES_OverFlow: // Overflow状態
			s = _T("-OverFlow-");
			break;
		default:
			ASSERT(false);
			break;
		}
		return s;
	}
};



}; // TNB

