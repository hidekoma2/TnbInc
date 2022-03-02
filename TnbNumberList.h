#pragma once
/**
 *@file
 * ��������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSet.h"
#include "TnbStrVector.h"
#include "TnbStrOperator.h"



//TNB Library
namespace TNB
{



/**
 * [�ݒ�] ������ݒ�.
 *	@param[out] _v �ϊ��������l�̗��񂪃Z�b�g����܂��B
 *	@param[in] lpsz �ݒ蕶����B[1,4-7] �ƌ����悤�ɋL�q�ł��܂��B
 *	@param[in] period �~������
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
 * [�ϊ�] ������.
 *	@param v ���l�̗�����w�肵�܂��B
 *	@param period �~������
 *	@return ������. �A�Ԃ̏ꍇ�A[1,4-7] �ƌ����悤�ɕ\�L���܂��B 
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
			//�A��
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
 * [�ݒ�] �r�b�g�t�B�[���h�ݒ�.
 *		�����Ă���ƃr�b�g�ԍ��̗�����쐬���܂��B
 *	@param[out] _v �ϊ��������l�̗��񂪃Z�b�g����܂��B
 *	@param[in] bf �r�b�g�t�B�[���h
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
 * [�ϊ�] �r�b�g�t�B�[���h��.
 *	@param v ���Ă����r�b�g�ԍ��̗�����w�肵�܂��i�O�`�U�R�j�B
 *	@return �r�b�g�t�B�[���h. 
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


