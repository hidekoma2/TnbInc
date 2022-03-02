#pragma once
/**
 *@file
 * �����񑀍색�C�u�����֌W�̃w�b�_
 *
 *		CRT �̊֐����p�b�N���A�I�[�o�[���[�h���Achar �ł� WCHAR �ł�����
 *		���O�Ŏg����悤�ɂ������̂�����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



#include <stdlib.h>
#include <WinNls.h>



//TNB Library
namespace TNB {
namespace STRLIB 
{

	/** @addtogroup	STRING
	 * @{
	 */

	/**
	 * [�v�Z] �����񒷌v�Z(ASCII/SJIS�p)
	 *	@param lpsz ������
	 *	@return ������
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline size_t GetLen(LPCSTR lpsz)
	{
		size_t r = 0;
		LPCSTR P = lpsz;
		if ( P != NULL )
		{
			while ( *P++ != 0 )
			{
				r++;
			}
			ASSERTLIB(r == strlen(lpsz));
		}
		return r;
	}

	/**
	 * [�v�Z] �����񒷌v�Z(UNICDE�p)
	 *	@param lpsz ������
	 *	@return ������
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline size_t GetLen(LPCWSTR lpsz)
	{
		size_t r = 0;
		LPCWSTR P = lpsz;
		if ( P != NULL )
		{
			while ( *P++ != 0 )
			{
				r++;
			}
			ASSERTLIB(r == wcslen(lpsz));
		}
		return r;
	}

	/**
	 * [����] ������R�s�[(ASCII/SJIS�p)
	 *	@note �̈悪�d�Ȃ��Ă���ꍇ�A����͖��ۏ؂ł��B
	 *	@param[out] _dst	�R�s�[��
	 *	@param[in] src	�R�s�[��
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline void Copy(LPSTR _dst, LPCSTR src)
	{
		ASSERT(! ::IsBadWritePtr(_dst, GetLen(src)));
		do
		{
			*_dst = *src++;
		}
		while ( *_dst++ != 0 );
	}

	/**
	 * [����] ������R�s�[(UNICODE�p)
	 *	@note �̈悪�d�Ȃ��Ă���ꍇ�A����͖��ۏ؂ł��B
	 *	@param[out] _dst	�R�s�[��
	 *	@param[in] src	�R�s�[��
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline void Copy(LPWSTR _dst, LPCWSTR src)
	{
		ASSERT(! ::IsBadWritePtr(_dst, GetLen(src) * sizeof(WCHAR)));
		do
		{
			*_dst = *src++;
		}
		while ( *_dst++ != 0 );
	}

	/** 
	 * [��r] �������r(ASCII/SJIS�p)
	 *	@param P1 ��r�P
	 *	@param P2 ��r�Q
	 *	@param len ��r����T�C�Y�B�ȗ����}�C�i�X�Ȃ�NULL�������I�Ɍv�Z����܂��B
	 *	@param dwCmpFlags ��r�I�v�V����(�ȉ��̒l�̘_���a) <BR>
	 *			NORM_IGNORECASE	�啶��/����������ʂ��Ȃ�					<BR>
	 *			NORM_IGNOREKANATYPE	�Ђ炪��/�J�^�J�i����ʂ��Ȃ�			<BR>
	 *			NORM_IGNORENONSPACE	�ꏊ�����Ȃ���������ʂ��Ȃ�			<BR>
	 *			NORM_IGNORESYMBOLS	�L���𖳎�����							<BR>
	 *			NORM_IGNOREWIDTH	1�o�C�g�����Ƃ���Ɠ���2�o�C�g��������ʂ��Ȃ�<BR>
	 *			SORT_STRINGSORT	��Ǔ_���L���Ƃ��Ĉ���						<BR>
	 *	@retval ���O P1���P2�̂ق����傫��
	 *	@retval �O	 P1��P2�Ɠ���
	 *	@retval ���O P1���P2�̂ق���������
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline int Compare(LPCSTR P1, LPCSTR P2, INT_PTR len = -1, DWORD dwCmpFlags = 0)
	{
		if ( P2 == NULL ){ P2 = ""; }
		int l = ToInt(len);
		#ifndef _WIN32_WCE
			return ::CompareStringA(LOCALE_USER_DEFAULT, dwCmpFlags, P1, l, P2, l) - CSTR_EQUAL;
		#else
			DWORD f = NORM_IGNOREKANATYPE | NORM_IGNORENONSPACE | NORM_IGNORESYMBOLS;
			if ( (dwCmpFlags & f) != 0 )
			{
				ASSERT0(false, "STRLIB::Compare()", "�ꕔ�̃t���O�͖�������܂�");
			}
			if ( (dwCmpFlags & NORM_IGNORECASE) != 0 )
			{
				return (l < 0) ? ::_stricmp(P1, P2) : ::_strnicmp(P1, P2, l);
			}
			return (l < 0) ? ::strcmp(P1, P2) : ::strncmp(P1, P2, l);
		#endif
	}

	/** 
	 * [��r] �������r(UNICODE�p)
	 *	@param P1 ��r�P
	 *	@param P2 ��r�Q
	 *	@param len ��r����T�C�Y�B�ȗ����}�C�i�X�Ȃ�NULL�������I�Ɍv�Z����܂��B
	 *	@param dwCmpFlags ��r�I�v�V���� 
	 *	@retval ���O P1���P2�̂ق����傫��
	 *	@retval �O	 P1��P2�Ɠ���
	 *	@retval ���O P1���P2�̂ق���������
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline int Compare(LPCWSTR P1, LPCWSTR P2, INT_PTR len = -1, DWORD dwCmpFlags = 0)
	{
		if ( P2 == NULL ){ P2 = L""; }
		int l = ToInt(len);
		return ::CompareStringW(LOCALE_USER_DEFAULT, dwCmpFlags, P1, l, P2, l) - CSTR_EQUAL;
	}

	/**
	 * [����] ��������(ASCII/SJIS�p)
	 *	@param lpszText �����Ώە�����
	 *	@param c ��������
	 *	@param iFromIndex �J�n�C���f�b�N�X�B�ȗ������0�ł��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCSTR lpszText, char c, INDEX iFromIndex = 0)
	{
		LPCSTR P = ::strchr(&lpszText[iFromIndex], c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [����] ��������(UNICODE�p)
	 *	@param lpszText �����Ώە�����
	 *	@param c ��������
	 *	@param iFromIndex �J�n�C���f�b�N�X�B�ȗ������0�ł��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCWSTR lpszText, WCHAR c, INDEX iFromIndex = 0)
	{
		LPCWSTR P = ::wcschr(&lpszText[iFromIndex], c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [����] �����Z�b�g����(ASCII/SJIS�p)
	 *	@param lpszText �����Ώە�����
	 *	@param lpszMark ���������Z�b�g
	 *	@param iFromIndex �J�n�C���f�b�N�X�B�ȗ������0�ł��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOneOf(LPCSTR lpszText, LPCSTR lpszMark, INDEX iFromIndex = 0)
	{
		size_t l = GetLen(&lpszText[iFromIndex]);
		size_t r = ::strcspn(&lpszText[iFromIndex], lpszMark);
		return (r >= l) ? -1 : r + iFromIndex;
	}

	/**
	 * [����] �����Z�b�g����(UNICODE�p)
	 *	@param lpszText �����Ώە�����
	 *	@param lpszMark ���������Z�b�g
	 *	@param iFromIndex �J�n�C���f�b�N�X�B�ȗ������0�ł��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOneOf(LPCWSTR lpszText, LPCWSTR lpszMark, INDEX iFromIndex = 0)
	{
		size_t l = GetLen(&lpszText[iFromIndex]);
		size_t r = ::wcscspn(&lpszText[iFromIndex], lpszMark);
		return (r >= l) ? -1 : r + iFromIndex;
	}

	/**
	 * [����] �~����������(ASCII/SJIS�p)
	 *	@param lpszText �����Ώە�����
	 *	@param c ��������
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR ReverseIndexOf(LPCSTR lpszText, char c)
	{
		LPCSTR P = ::strrchr(lpszText, c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [����] �~����������(UNICODE�p)
	 *	@param lpszText �����Ώە�����
	 *	@param c ��������
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR ReverseIndexOf(LPCWSTR lpszText, char c)
	{
		LPCWSTR P = ::wcsrchr(lpszText, c);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [����] �����񌟍�(ASCII/SJIS�p)
	 *	@param lpszText �����Ώە�����
	 *	@param lpszWord ����������
	 *	@param iFromIndex �J�n�C���f�b�N�X�B�ȗ������0�ł��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCSTR lpszText, LPCSTR lpszWord, INDEX iFromIndex = 0)
	{
		LPCSTR P = strstr(&lpszText[iFromIndex], lpszWord);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [����] �����񌟍�(UNICDE�p)
	 *	@param lpszText �����Ώە�����
	 *	@param lpszWord ����������
	 *	@param iFromIndex �J�n�C���f�b�N�X�B�ȗ������0�ł��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR IndexOf(LPCWSTR lpszText, LPCWSTR lpszWord, INDEX iFromIndex = 0)
	{
		LPCWSTR P = wcsstr(&lpszText[iFromIndex], lpszWord);
		return (P == NULL) ? (-1) : (P - lpszText);
	}

	/**
	 * [����] �p�X��؂茟��(ASCII/SJIS�p)
	 *	@param lpszText �����Ώۃt�@�C����������
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR PathIndexOf(LPCSTR lpszText)
	{
		INT_PTR p1 = ReverseIndexOf(lpszText, '\\');
		INT_PTR p2 = ReverseIndexOf(lpszText, '/');
		return (p1 < p2) ? p2 : p1;
	}

	/**
	 * [����] �p�X��؂茟��(UNICODE�p)
	 *	@param lpszText �����Ώۃt�@�C����������
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline INT_PTR PathIndexOf(LPCWSTR lpszText)
	{
		INT_PTR p1 = ReverseIndexOf(lpszText, '\\');
		INT_PTR p2 = ReverseIndexOf(lpszText, '/');
		return (p1 < p2) ? p2 : p1;
	}

	/**
	 * [�擾] �����̃T�C�Y(ASCII/SJIS�p)
	 *	@note SJIS�̏ꍇ�A�S�p�`�F�b�N�̔��f���o���܂��B
	 *	@param c ����
	 *	@retval 2 ��o�C�g�����̈�Byte��
	 *	@retval 1 ���̑�
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline int GetCharSize(char c)
	{
		return ::IsDBCSLeadByte(c) ? 2 : 1;
	}

	/**
	 * [�擾] �����̃T�C�Y(UNICODE�p)
	 *	@param c ����
	 *	@return �P�A�Œ�B
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline int GetCharSize(WCHAR c)
	{
		return 1; 
	}

	/**
	 * [�ϊ�] INT�ϊ�(ASCII/SJIS�p).
	 *	@param lpsz ������B
	 *	@param iBase �x�[�X�ɂȂ�l�B���� 10 �� 16 ���w��B 0���w�肷��ƁA 
	 *					lpsz �̕\�L�ɏ]���i�擪��0x,0X�Ȃ�16�i��,0�Ȃ�8�i��,����ȊO��10�i���j�B
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline int ToInt(LPCSTR lpsz, int iBase = 10) 
	{
		return strtol(lpsz, NULL, iBase);
	}

	/**
	 * [�ϊ�] INT�ϊ�(UNICODE�p).
	 *	@param lpsz ������B
	 *	@param iBase �x�[�X�B10��16�B 0���w�肷��ƁA lpsz �̕\�L�ɏ]���B 
	 *						�擪��0x,0X�Ȃ�16�i��,0�Ȃ�8�i��,����ȊO��10�i���B
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline int ToInt(LPCWSTR lpsz, int iBase = 10)
	{
		return wcstol(lpsz, NULL, iBase); 
	}

	/**
	 * [�ϊ�] INT�ϊ�(ASCII/SJIS�p).
	 *	@param lpsz ������B
	 *	@param iBase �x�[�X�ɂȂ�l�B���� 10 �� 16 ���w��B 0���w�肷��ƁA 
	 *					lpsz �̕\�L�ɏ]���i�擪��0x,0X�Ȃ�16�i��,0�Ȃ�8�i��,����ȊO��10�i���j�B
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline DWORD ToDword(LPCSTR lpsz, int iBase = 10)
	{
		return strtoul(lpsz, NULL, iBase);
	}

	/**
	 * [�ϊ�] INT�ϊ�(UNICODE�p).
	 *	@param lpsz ������B
	 *	@param iBase �x�[�X�ɂȂ�l�B���� 10 �� 16 ���w��B 0���w�肷��ƁA 
	 *					lpsz �̕\�L�ɏ]���i�擪��0x,0X�Ȃ�16�i��,0�Ȃ�8�i��,����ȊO��10�i���j�B
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline DWORD ToDword(LPCWSTR lpsz, int iBase = 10)
	{
		return wcstoul(lpsz, NULL, iBase);
	}

	/**
	 * [�ϊ�] double�ϊ�(ASCII/SJIS�p)
	 *	@param lpsz ������
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline double ToDouble(LPCSTR lpsz) 
	{
		return strtod(lpsz, NULL); 
	}

	/**
	 * [�ϊ�] double�ϊ�(UNICODE�p)
	 *	@param lpsz ������
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline double ToDouble(LPCWSTR lpsz) 
	{ 
		return wcstod(lpsz, NULL); 
	}

	/**
	 * [�ϊ�] LONGLONG�ϊ�(ASCII/SJIS�p)
	 *	@param lpsz ������
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline LONGLONG ToLonglong(LPCSTR lpsz)
	{
		return _atoi64(lpsz); 
	}

	/**
	 * [�ϊ�] LONGLONG�ϊ�(UNICODE�p)
	 *	@param lpsz ������
	 *	@return ���l
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline LONGLONG ToLonglong(LPCWSTR lpsz)
	{
		return _wtoi64(lpsz);
	}

	/**
	 * [���f] ASCII�I�����[���f
	 *	@note	ASCII�R�[�h�����̕����񂩁A�ۂ����f���܂��B
	 *	@param lpsz ������
	 *	@retval true ASCII�݂̂��B
	 *	@retval false SJIS�Ȃǂ��܂܂��B
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline bool IsAsciiOnly(LPCSTR lpsz)
	{
		while ( *lpsz != 0 )
		{
			if ( GetCharSize(*lpsz++) != 1 )
			{
				return false;// ASCII�ȊO����
			}
		}
		return true;
	}

	/**
	 * [�ϊ�] HEX�������l�ϊ�
	 *	@note '0' �` 'F' �� 0x00 �` 0x0F
	 *	@param c HEX����
	 *	@retval 0�ȏ�	�ϊ�����
	 *	@retval 0����	�ϊ����s(�m��Ȃ�����)
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline int HexCharToInt(int c)
	{
		int r = -1;
		if ( c >= 'a' && c <= 'f' )
		{
			r = (c - 'a') + 10;
		}
		else if ( c >= 'A' && c <= 'F' )
		{
			r = (c - 'A') + 10;
		}
		else if ( c >= '0' && c <= '9' )
		{
			r = c - '0';
		}
		return r;
	}

	/**
	 * [�ϊ�] ���lHEX�����ϊ�
	 *	@note 0x00 �` 0x0F �� '0' �` 'F'
	 *	@param c ���l�B���ʂSBit�̂ݎg�p���܂��B
	 *	@return HEX����
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	inline char IntToHexChar(int c)
	{
		return "0123456789ABCDEF"[c & 0x000F];
	}

	/**
	 * [�ϊ�] �G�X�P�[�v�����\�L�ϊ�
	 *	@note '��x�`' �Ƃ���������𕶎��R�[�h�ɕϊ����܂��B
	 *	@param[in,out] _lpsz '��' ���w���Ă���|�C���^�B�ϊ���������ƁA���̕����̎��̃|�C���^�ɂȂ�܂��B
	 *	@retval 0�ȏ�	�ϊ�����
	 *	@retval 0����	�ϊ����s(�m��Ȃ�����) _lpsz �͕ω����܂���B
	 *	@par�K�v�t�@�C��
	 *			TnbStrLib.h
	 */
	template<typename TYP>
	inline int EscCharToInt(const TYP*& _lpsz)
	{
		if ( *_lpsz != '\\' ){ return -1; }
		int r = -1;
		const TYP* P = _lpsz;
		P++;
		TYP c = *P++;
		switch ( c )
		{
		case 't':	// �^�u����
			r = '\t';
			break;
		case 'n':	// ���s����
			r = '\n';
			break;
		case 'r':	// �L�����b�W���^�[��
			r = '\r';
			break;
		case 'e':
			r = '\x1B';
			break;
		case '\\':
			r = '\\';
			break;
		case 'x':	// \xhh		16�i���l��������
		case 'u':	// \uhhhh	16�i���l��������
			loop( i, (c == 'x') ? 2 : 4 )
			{
				int c = STRLIB::HexCharToInt(*P);
				if ( c < 0 ) { break; }
				if ( r < 0 ) { r = 0; }
				r *= 16;
				r += c;
				P++;
			}
			break;
		case '0':
			if ( *P >= '0' && *P <= '9' )
			{
				loop ( i, 3 )
				{
					if ( *P < '0' || *P > '7' ) { break; }
					if ( r < 0 ) { r = 0; }
					r *= 8;
					r += (*P - '0');
					P++;
				}
			}
			break;
		default:
			break;
		}
		if ( r >= 0 )
		{
			_lpsz = P;
		}
		return r;
	}

}; // STRLIB



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")

