#pragma once
/**
 *@file
 * ��������z��Ǘ��֌W�̃w�b�_
 *
 *		������̔z��i�A�������������z�u�j���Ǘ����܂��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * ������z��Ǘ��N���X
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 */
typedef TNB::CVectorT< CStr > CStrVector;



/**@ingroup COLLECT
 * ������z��Ǘ��N���X
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 */
typedef TNB::CVectorT< CAscii > CAsciiVector;



/**@ingroup COLLECT
 * ������z��Ǘ��N���X
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 */
typedef TNB::CVectorT< CUnicode > CUnicodeVector;



/**@ingroup STRING
 * [�ϊ�] ASCII�����񁨕����P�ʔz��
 *	@note �w���ASCII��������AWORD �x�N�^�ɕϊ����܂��B
 *		��o�C�g�����A��o�C�g���������ꂼ���v�f�Ŋi�[���܂��B
 *		�I�[�����i���O�j�͓���܂���B
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param lpszAscii �ϊ�������
 *	@return WORD �z��
 */
inline CWordVector StringToWordVector(LPCSTR lpszAscii)
{
	CWordVector vw;
	LPCSTR P = lpszAscii;
	while ( P != NULL && *P != 0 )
	{
		char c = *P++;
		BYTE b = c;
		if ( STRLIB::GetCharSize(c) == 2 )
		{
			BYTE d = *P++;
			vw.Add(static_cast<WORD>((b << 8) | d));
		}
		else
		{
			vw.Add(b);
		}
	}
	return vw;
}



/**@ingroup STRING
 * [�ϊ�] UNICODE�����񁨕����P�ʔz��
 *	@note �w���UNICODE��������AWORD �x�N�^�ɕϊ����܂��B
 *		�I�[�����i���O�j�͓���܂���B
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param lpszUnicode �ϊ�������
 *	@return WORD �z��
 */
inline CWordVector StringToWordVector(LPCWSTR lpszUnicode)
{
	CWordVector vw;
	vw.SetElements(STRLIB::GetLen(lpszUnicode), reinterpret_cast<const WORD*>(lpszUnicode));
	return vw;
}



/**@ingroup STRING
 * [�ϊ�] CStrVector �� CStr 
 *	@note ������z���A�����A��̕�����ɂ��܂��B�I�[�����i���O�j�͓���܂���B
 *	@note �~����������������Ɋ܂܂�Ă���ƁA����ɏ����ł��܂���B
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param vs ������z��B
 *	@param sepaChar �~���L�����N�^�B�ȗ������ ^�B
 *	@return ������B
 */
inline CStr StrVectorToStr(const CStrVector& vs, TCHAR sepaChar = '^')
{
	CStr str;
	loop ( i, vs.GetSize() )
	{
		if ( i != 0 )
		{
			str += sepaChar;
		}
		str += vs[i];
	}
	return str;
}

/**@ingroup STRING
 * [�ϊ�] CStr �� CStrVector 
 *	@note StrVectorToStr() �ŘA��������������A������z��ɕ������܂��B
 *	@note �~����������������Ɋ܂܂�Ă���ƁA����ɏ����ł��܂���B
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param lpsz StrVectorToStr() �ŘA������������B
 *	@param sepaChar �~���L�����N�^�B�ȗ������ ^�B
 *	@return ������B
 */
inline CStrVector StrToStrVector(LPCTSTR lpsz, TCHAR sepaChar = '^')
{
	CStrVector vs;
	if ( STRLIB::GetLen(lpsz) > 0 )
	{
		INDEX index = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(lpsz, sepaChar, index);
			if ( r < 0 )
			{
				break;
			}
			CStr s;
			s.SetFromLeft(&lpsz[index], static_cast<size_t>(r - index));
			vs.Add(s);
			index = static_cast<INDEX>(r + 1);
		}
		vs.Add(CStr(&lpsz[index]));
	}
	return vs;
}

/**@ingroup STRING
 * [�ϊ�] CStrVector �� CStr 
 *	@note ������z���A�����A��̕�����ɂ��܂��B
 *		�I�[�����i���O�j�͓���܂���B
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param vs ������z��B
 *	@param sepaChar �~���L�����N�^�B�ȗ������ ^�B
 *	@param escChar ���ʃL�����N�^�B�ȗ������ ���B
 *	@return ������B
 */
inline CStr StrVectorToStrEx(const CStrVector& vs, TCHAR sepaChar = '^', TCHAR escChar = '\\')
{
	CStr str;
	TCHAR aEsc[2] = { escChar, 0 };
	TCHAR aEsc2[3] = { escChar, escChar, 0 };
	TCHAR aSepa[3] = { escChar, sepaChar, 0 };
	loop ( i, vs.GetSize() )
	{
		if ( i != 0 )
		{
			str += aSepa;
		}
		CStr s = vs[i];
		s.Replace(aEsc, aEsc2);
		str += s;
	}
	return str;
}

/**@ingroup STRING
 * [�ϊ�] CStr �� CStrVector 
 *	@note StrVectorToStrEx() �ŘA��������������A������z��ɕ������܂��B
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param lpsz StrVectorToStrEx() �ŘA������������B
 *	@param sepaChar �~���L�����N�^�B�ȗ������ ^�B
 *	@param escChar ���ʃL�����N�^�B�ȗ������ ���B
 *	@return ������B
 */
inline CStrVector StrToStrVectorEx(LPCTSTR lpsz, TCHAR sepaChar = '^', TCHAR escChar = '\\')
{
	CStrVector vs;
	if ( lpsz != NULL && STRLIB::GetLen(lpsz) > 0 )
	{
		TCHAR aEsc[2] = { escChar, 0 };
		TCHAR aEsc2[3] = { escChar, escChar, 0 };
		TCHAR aSepa[3] = { escChar, sepaChar, 0 };
		INDEX index = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(lpsz, aSepa, index);
			if ( r < 0 )
			{
				break;
			}
			CStr s;
			s.SetFromLeft(&lpsz[index], static_cast<size_t>(r - index));
			s.Replace(aEsc2, aEsc);
			vs.Add(s);
			index = static_cast<INDEX>(r + 2);
		}
		CStr s = CStr(&lpsz[index]);
		s.Replace(aEsc2, aEsc);
		vs.Add(s);
	}
	return vs;
}

/**@ingroup STRING
 * [�ǉ�] ������ǉ�����I�y���[�^. 
 *		�� �ŕ������ Vector ������I�y���[�^�ł��B
 *	@note ���p�̓\�[�X�̉ǐ��𗎂Ƃ��̂ŁA����
 *	@par�g�p��
 *	\code
 *		// vs[0] == "ABC", vs[1] == "DEF", vs[2] == "GHI"
 *		CStrVector vs = CStr(_T("ABC")) % _T("DEF") % _T("GHI");
 *	\endcode
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param s1 �z��P�̖ڂ̕�����B
 *	@param s2 �z��Q�̖ڂ̕�����B
 *	@return ������z��B
 */
inline CStrVector operator%(const CStr& s1, const CStr& s2)
{
	CStrVector vs;
	vs.Add(s1);
	vs.Add(s2);
	return vs;
}

/**@ingroup STRING
 * [�ǉ�] ������ǉ�����I�y���[�^. 
 *		�� �ŕ������ Vector ������I�y���[�^�ł��B
 *	@note ���p�̓\�[�X�̉ǐ��𗎂Ƃ��̂ŁA����
 *	@par�g�p��
 *	\code
 *		// vs[0] == "ABC", vs[1] == "DEF", vs[2] == "GHI"
 *		CStrVector vs = CStr(_T("ABC")) % _T("DEF") % _T("GHI");
 *	\endcode
 *	@par�K�v�t�@�C��
 *			TnbStrVector.h
 *	@param vs ������z��B
 *	@param s �A������̕�����B
 *	@return ������z��B
 */
inline CStrVector operator%(const CStrVector& vs, const CStr& s)
{
	CStrVector v = vs;
	v.Add(s);
	return v;
}

#ifdef __AFX_H__

	/**@ingroup COLLECT
	 * CString�z��Ǘ��N���X
	 *	@note MFC�v���W�F�N�g�̎��̂ݎg�p�\�ł��B
	 *	@par�K�v�t�@�C��
	 *			TnbStrVector.h
	 */
	typedef TNB::CVectorT<CString> CStringVector;

	/**@ingroup COLLECT
	 * [����] CStringArray���� CStrVector �փR�s�[
	 *	@note MFC�v���W�F�N�g�̏ꍇ�̂ݎg�p�̉\�ł��B
	 *	@par�K�v�t�@�C��
	 *			TnbStrVector.h
	 *	@param	astr ���� CStringArray 
	 *	@return CStrVector
	 */
	inline CStrVector ToStrVector(const CStringArray& astr)
	{
		CStrVector vs;
		size_t size = static_cast<size_t>(astr.GetSize());
		vs.SetSize(size);
		loop ( i, size )
		{
			vs.Set(i, CStr(astr[i]));
		}
		return vs;
	}

#endif



/**
 * [�o��] �f�o�b�O�g���[�X.
 *	@note �����[�X�r���h���͉������܂���B
 *	@param vs ������Q
 */
#ifdef _DEBUG
inline void TRACE_SV(const CStrVector& vs) 
{
	loop ( i, vs )
	{
		::OutputDebugString(vs[i] + _T("\n"));
	}
}
#else
#define TRACE_SV(XX)
#endif



}; // TNB

