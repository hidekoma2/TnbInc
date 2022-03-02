#pragma once
/**
 *@file
 * 数字羅列関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSet.h"
#include "TnbStrVector.h"
#include "TnbStrOperator.h"



//TNB Library
namespace TNB
{



/**
 * [設定] 文字列設定.
 *	@param[out] _v 変換した数値の羅列がセットされます。
 *	@param[in] lpsz 設定文字列。[1,4-7] と言うように記述できます。
 *	@param[in] period 敷居文字
 */
template<typename TYP>
inline void StringTo(CSingleSetT<TYP>& _v, LPCTSTR lpsz, TCHAR period = _T(','))
{
	CStr s = lpsz;
	s.Replace(_T(" "), _T(""));
	CStrVector vs = CStrOperator::SeparatePeriod(s, period);
	_v.RemoveAll();
	loop ( i, vs )
	{
		CStrVector v = CStrOperator::SeparatePeriod(vs[i], _T('-'));
		if ( v.GetSize() == 1 )
		{
			if ( ! v[0].IsEmpty() && STRLIB::HexCharToInt(v[0][0]) >= 0 )
			{
				_v.Insert(static_cast<TYP>(v[0].ToDword(0, 0)));
			}
		}
		else if ( v.GetSize() >= 2 )
		{
			if ( ! v[0].IsEmpty() && STRLIB::HexCharToInt(v[0][0]) >= 0 )
			{
				if ( ! v[1].IsEmpty() && STRLIB::HexCharToInt(v[1][0]) >= 0 )
				{
					TYP t1 = static_cast<TYP>(v[0].ToDword(0, 0));
					TYP t2 = static_cast<TYP>(v[1].ToDword(0, 0));
					if ( t1 > t2 )
					{
						Swap(t1, t2);
					}
					loop ( j, (t2 - t1) + 1 )
					{
						_v.Insert(static_cast<TYP>(t1 + j));
					}
				}
			}
		}
	}
}

/**
 * [変換] 文字列化.
 *	@param v 数値の羅列を指定します。
 *	@param period 敷居文字
 *	@return 文字列. 連番の場合、[1,4-7] と言うように表記します。 
 */
template<typename TYP>
inline CStr ToString(const CSingleSetT<TYP>& v, TCHAR period = _T(','))
{
	CStrAdder s;
	INDEX idx = 0;
	while ( v.IsInRange(idx) )
	{
		TYP v1 = v.At(idx);
		s.AddFormat(_T("%d"), v1);
		idx++;
		if ( ! v.IsInRange(idx) )
		{
			break;
		}
		if ( v1 + 1 == v.At(idx) )
		{
			//連番
			v1++;
			idx++;
			if ( v.IsInRange(idx) && v1 + 1 == v.At(idx) )
			{
				do
				{
					v1 = v.At(idx);
					idx++;
				}
				while ( v.IsInRange(idx) && v1 + 1 == v.At(idx) );
				s.AddFormat(_T("-%d%c"), v1, period);
			}
			else
			{
				s.AddFormat(_T("%c%d%c"), period, v1, period);
			}
		}
		else
		{
			s.Add(period);
		}
	}
	return CStr(s).TrimRight(period);
}



/**
 * [設定] ビットフィールド設定.
 *		立っているとビット番号の羅列を作成します。
 *	@param[out] _v 変換した数値の羅列がセットされます。
 *	@param[in] bf ビットフィールド
 */
template<typename TYP>
inline void BitfieldTo(CSingleSetT<TYP>& _v, ULONGLONG bf)
{
	_v.RemoveAll();
	loop ( i, 64 )
	{
		if ( (bf & _BIT(i)) != 0 )
		{
			_v.Insert(static_cast<TYP>(i));
		}
	}
}

/**
 * [変換] ビットフィールド化.
 *	@param v 立てたいビット番号の羅列を指定します（０～６３）。
 *	@return ビットフィールド. 
 */
template<typename TYP>
inline ULONGLONG ToBitfield(const CSingleSetT<TYP>& v)
{
	ULONGLONG r = 0;
	loop ( i, v )
	{
		r |= _BIT(v[i]);
	}
	return r;
}



}; // TNB


