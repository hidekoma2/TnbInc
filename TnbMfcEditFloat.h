#pragma once
/**
 *@file
 * 小数点付き数値専用入力Edit関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcEditValue.h"
#include <math.h>
#include <wchar.h>



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN

/// 10の累乗定数のマクロ群
template<UINT n> struct TPowerValue {
	enum {
		VALUE = TPowerValue<n - 1>::VALUE * 10
	};
};
template<> struct TPowerValue<0> {
	enum {VALUE = 1};
};
#define _POW10(X) (TPowerValue<X>::VALUE)

#endif //_TnbDOXYGEN



/**@ingroup MFCCONTROL
 * 小数点付き数値専用Editコントロールテンプレート
 *
 *		数値で入出力できます。
 *		範囲を指定できます。
 *		ステップ数を指定できます（ただし基点は０）。初期値は最小値です( POW が 3 なら 0.001)。
 *		スピンコントロールをつけることが出来ます。
 *
 *	@par使い方
 *		リソースエディタでエディトボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CEdit）を本クラスに置き換えて使用します。
 *
 *	@param POW 少数点以下の桁数.
 *
 *	@par必要ファイル
 *			TnbMfcEditFloat.h
 * 
 *	@date 08/01/02	新規作成
 *	@date 08/03/18	範囲チェック方法修正
 *	@date 08/04/15	SetValue() 時に範囲チェックを追加。
 */
template<int POW = 1>
class CEditFloatT : public CEditValueT<double, 0, 0>
{
	typedef CEditValueT<double, 0, 0> _super;
protected:
	enum { POWER = _POW10(POW) };
	/**
	 * [確認] 入力許可チェック
	 *	@param[in,out] _nChar WM_CHAR の WPARAM
	 *	@param[in,out] _lFlags WM_CHAR の LPARAM
	 *	@param[in] strNowText 入力済みの文字列
	 *	@retval true 入力を認める。
	 *	@retval false 入力を認めない。
	 */
	virtual bool IsValidChar(UINT& _nChar, LONG& _lFlags, const CString& strNowText)
	{
		if ( _nChar == '.' && strNowText.Find('.') < 0 )
		{
			return true;
		}
		return _super::IsValidChar(_nChar, _lFlags, strNowText);
	}

	/**
	 * [通知] 入力時にコール
	 *	@note なにもしない。
	 */
	virtual void OnUpdateChar(void)
	{
	}

	/**
	 * [通知] フォーカスを失った時にコール
	 *	@note	範囲チェックし、超えていたら丸めちゃう
	 */
	virtual void OnEditEnd(void)
	{
		CString s = _super::GetText();
		if ( ! s.IsEmpty() )
		{
			double d = _tcstod(s, NULL);
			double dd = d;
			double mx = static_cast<double>(m_xMax) / POWER;
			double mn = static_cast<double>(m_xMin) / POWER;
			if ( d > mx )
			{
				d = mx;
			}
			else if ( d < mn )
			{
				d = mn;
			}
			LONGLONG i = ToInteger(s);
			LONGLONG ii = AdjustValue(i);
			_super::SetText(ToString(ii));
			if ( i != ii || d != dd )
			{
				::MessageBeep(MB_ICONASTERISK); //入力制限より内容が変化した時
			}
		}
	}

	/** 
	 * [変換] 数値から文字列作成.
	 *	@note 内部用のメソッドです。
	 *	@param ll 数値
	 *	@return 文字列
	 */
	virtual CString ToString(LONGLONG ll) const
	{
		double r = static_cast<double>(ll) / POWER;
		CString str;
		if ( m_iZeroPlace > 0 )
		{
			CString s;
			s.Format(_T("%%0%d.%df"), m_iZeroPlace, POW); 
			str.Format(s, r);
		}
		else
		{
			CString s;
			s.Format(_T("%%.%df"), POW); 
			str.Format(s, r);
		}
		return str;
	}
	
	/** 
	 * [変換] 文字列から数値作成.
	 *	@note 内部用のメソッドです。
	 *	@param s 文字列
	 *	@return 数値
	 */
	virtual LONGLONG ToInteger(LPCTSTR s) const
	{
		double d = _tcstod(s, NULL) * POWER;
		d += (d < 0.0) ? -0.5 : 0.5;
		return static_cast<LONGLONG>(d);
	}

public:
	
	/// コンストラクタ
	CEditFloatT(void)
	{
		m_xMin = 0;
		m_xMax = 0;
		m_step = 1;
	}

	/**
	 * [変換] 値文字列化
	 *	@param xValue 変換する値
	 *	@return 変換後の文字列
	 */
	virtual CString ValueToString(double xValue) const
	{
		double d = xValue * POWER;
		d += (d < 0.0) ? -0.5 : 0.5;
		LONGLONG l = AdjustValue(static_cast<LONGLONG>(d));
		return ToString(l);
	}

	/**
	 * [設定] 値設定
	 *	@param xValue 設定する値
	 */
	virtual void SetValue(double xValue)
	{
		_super::SetText(ValueToString(xValue));
	}

	/**
	 * [取得] 値取得
	 *	@param[out] _xValue 設定されている値
	 *	@retval true 値有り。
	 *	@return false Empty。
	 */
	virtual bool GetValue(double& _xValue) const
	{
		CString s = _super::GetText();
		if ( s.IsEmpty() )
		{
			return false;
		}
		_xValue = static_cast<double>(AdjustValue(ToInteger(s))) / POWER;
		return true;
	}
	
	/**
	 * [設定] 範囲指定
	 *	@param xMin 最小値
	 *	@param xMax 最大値
	 *	@param step ステップ数。省略すると最小値( POW が 3 なら 0.001)になります。
	 */
	virtual void SetRange(double xMin, double xMax, double step = 0.0)
	{
		m_xMin = static_cast<LONGLONG>(xMin * POWER);
		m_xMax = static_cast<LONGLONG>(xMax * POWER);
		if ( m_xMin > m_xMax )
		{
			Swap(m_xMin, m_xMax);
		}
		if ( step <= 0.0 )
		{
			step = 1.0 / POWER;
		}
		m_step = static_cast<int>(step * POWER);
	}
};



}; //MFC
}; //TNB

