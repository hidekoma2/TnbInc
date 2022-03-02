#pragma once
/**
 *@file
 * JIS�^������Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * JIS�^������Ǘ�
 *
 *		JIS�^�̕��������ێ�����N���X�ł��B
 *		ASCII,UNICODE�Ƃ̑��ݕϊ��p�̃��\�b�h������Ă��܂��B
 *
 *	@note	printf�n�� Format() Fmt() �� ��s �ɑ΂��āA���̂܂ܓn���܂���B
 *			CStr �ɕϊ����Ă���n���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbJisStr.h
 * 
 *	@date 06/09/27	�V�K�쐬�B
 *	@date 08/08/06	�\�[�X�����B Vector ���g��Ȃ��悤�ɕύX�B
 *	@date 16/07/26	warring �𐮗��B
 */
class CJisStr : public IComparableT<TNB::CJisStr>
{
	CAscii	m_vtcText;			///< ���
	enum { ESC = 0x1B };
	/// �������[�hByte�`�F�b�N
	static bool IsKanji(int c)
	{
		return ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC));
	}
	/// ���p�R�[�h�`�F�b�N
	static bool IsHankaku(int c)
	{
		return (c >= 0xa0 && c <= 0xdf);
	}
	/*
	 * [�ϊ�] ���p�J�i��S�p�J�i�ɕϊ�.
	 *	@param[out] _zenkaku �ϊ���̑S�p���i�[
	 *	@param[in] P ���p�J�i�̃|�C���^�B���̕������m�F���܂��B
	 *	@retval 1 ���_�A�����_�����ϊ�
	 *	@retval 2 ���_�A�����_����ϊ�
	 */
	static int ms_HanToZen(WORD& _zenkaku, const BYTE* P)
	{
		const static WORD z[64] =
		{
			0x2121, 0x2123, 0x2156, 0x2157, 0x2122, 0x2126, 0x2572, 0x2521,
			0x2523, 0x2525, 0x2527, 0x2529, 0x2563, 0x2565, 0x2567, 0x2543,
			0x213c, 0x2522, 0x2524, 0x2526, 0x2528, 0x252a, 0x252b, 0x252d,
			0x252f, 0x2531, 0x2533, 0x2535, 0x2537, 0x2539, 0x253b, 0x253d,
			0x253f, 0x2541, 0x2544, 0x2546, 0x2548, 0x254a, 0x254b, 0x254c,
			0x254d, 0x254e, 0x254f, 0x2552, 0x2555, 0x2558, 0x255b, 0x255e,
			0x255f, 0x2560, 0x2561, 0x2562, 0x2564, 0x2566, 0x2568, 0x2569,
			0x256a, 0x256b, 0x256c, 0x256d, 0x256f, 0x2573, 0x212b, 0x212c
		};
		/// ���_�A�����_�^
		struct TMark
		{
			BYTE han;
			WCHAR zen;
		};
		/// ���_�e�[�u��
		const static TMark marks1[] =
		{
			{0xb3, 0x2574}, {0xb6, 0x252c}, {0xb7, 0x252e}, {0xb8, 0x2530},
			{0xb9, 0x2532}, {0xba, 0x2534}, {0xbb, 0x2536}, {0xbc, 0x2538},
			{0xbd, 0x253a}, {0xbe, 0x253c}, {0xbf, 0x253e}, {0xc0, 0x2540},
			{0xc1, 0x2542}, {0xc2, 0x2545}, {0xc3, 0x2547}, {0xc4, 0x2549},
			{0xca, 0x2550}, {0xcb, 0x2553}, {0xcc, 0x2556}, {0xcd, 0x2559},
			{0xce, 0x255c}, {0, 0}
		};
		/// �����_�e�[�u��
		const static TMark marks2[] = 
		{
			{0xca, 0x2551}, {0xcb, 0x2554}, {0xcc, 0x2557}, {0xcd, 0x255a},
			{0xce, 0x255d}, {0, 0}
		};
		if ( P[1] == 0xde/*������*/ )
		{
			loop ( i, countof(marks1) )
			{
				if ( P[0] == marks1[i].han )
				{
					_zenkaku = marks1[i].zen;
					return 2;
				}
			}
		}
		else if ( P[1] == 0xdf/*��������*/ )
		{
			loop ( i, countof(marks2) )
			{
				if ( P[0] == marks2[i].han )
				{
					_zenkaku = marks2[i].zen;
					return 2;
				}
			}
		}
		int r = P[0] - 0xa0;
		ASSERT( r >= 0 && r < countof(z) );
		_zenkaku = z[r];
		return 1;
	}
	/*
	 * [�ϊ�] SJIS��2�o�C�g��JIS�ɕϊ�
	 *	@param[in,out] ph ���BYTE
	 *	@param[in,out] pl ����BYTE
	 */
	static void ms_SjisToJis(UINT *ph, UINT *pl)
	{
		*ph = *ph & 0xff;
		*pl = *pl & 0xff;
		if ( *ph <= 0x9F ) 
		{
			if ( *pl < 0x9F )
			{
				*ph = (*ph << 1) - 0xE1;
			}
			else 
			{
				*ph = (*ph << 1) - 0xE0;
			}
		}
		else
		{
			if ( *pl < 0x9F )
			{
				*ph = (*ph << 1) - 0x161;
			}
			else
			{
				*ph = (*ph << 1) - 0x160;
			}
		}
		if ( *pl < 0x7F )
		{
			*pl -= 0x1F;
		} 
		else if ( *pl < 0x9F )
		{
			*pl -= 0x20;
		}
		else 
		{
			*pl -= 0x7E;
		}
	}
	/*
	 * [�ϊ�] SJIS��ISO2022JP.
	 *		sjis_iso2022jp - SJIS��ISO-2022-JP(JIS)�ɕϊ� (RFC 1468)
	 *	@param[in] pSjis ����SJIS������B
	 *	@param[out] pIso2022 �ϊ�����ISO2022JP������B�o�b�t�@�͌��̕������4�{�K�v�B
	 *	@return �ϊ���̕������B
	 */
	static INT_PTR ms_SjisToIso2022jp(LPCSTR pSjis, LPSTR pIso2022)
	{
		const BYTE* p = reinterpret_cast<const BYTE*>(pSjis);
		BYTE* r = reinterpret_cast<BYTE*>(pIso2022);
		bool isKanji = false;
		UINT c = 0;
		while ( (c = *(p++)) != '\0') 
		{
			if ( ! isKanji )
			{
				if ( IsKanji(c) || IsHankaku(c) )
				{
					*(r++) = ESC;
					*(r++) = '$';
					*(r++) = 'B';
					if ( IsKanji(c) )
					{
						UINT d = *(p++);
						ms_SjisToJis(&c, &d);
						*(r++) = static_cast<BYTE>(c);
						*(r++) = static_cast<BYTE>(d);
					}
					else
					{
						WORD e;
						int rc = ms_HanToZen(e, p - 1);
						if ( rc != 1 )
						{
							p++;
						}
						*(r++) = static_cast<BYTE>((e >> 8) & 0xff);
						*(r++) = static_cast<BYTE>(e & 0xff);
					}
					isKanji = true;
				} 
				else 
				{
					*(r++) = static_cast<BYTE>(c);
				}
			}
			else
			{
				if ( ! IsKanji(c) && ! IsHankaku(c) )
				{
					*(r++) = ESC;
					*(r++) = '(';
					*(r++) = 'B';
					*(r++) = static_cast<BYTE>(c);
					isKanji = false;
				}
				else 
				{
					if ( IsKanji(c) )
					{
						UINT d = *(p++);
						ms_SjisToJis(&c, &d);
						*(r++) = static_cast<BYTE>(c);
						*(r++) = static_cast<BYTE>(d);
					}
					else
					{
						WORD e;
						int rc = ms_HanToZen(e, p - 1);
						if (rc != 1) 
						{
							p++;
						}
						*(r++) = static_cast<BYTE>((e >> 8) & 0xff);
						*(r++) = static_cast<BYTE>(e & 0xff);
					}
				}
			}
		}
		if ( isKanji ) 
		{
			*(r++) = ESC;
			*(r++) = '(';
			*(r++) = 'B';
		}
		*r = '\0';
		return r - reinterpret_cast<BYTE*>(pIso2022);
	}
	/*
	 * [�ϊ�] JIS��2�o�C�g��SJIS�ɕϊ�
	 *	@param[in,out] ph ���BYTE
	 *	@param[in,out] pl ����BYTE
	 */
	static void ms_JisToSjis(UINT* ph, UINT* pl)
	{
		if ( (*ph & 1) != 0 )
		{
			if ( *pl < 0x60 )
			{
				*pl += 0x1F;
			}
			else
			{
				*pl += 0x20;
			}
		}
		else
		{
			*pl += 0x7E;
		}
		if ( *ph < 0x5F )
		{
			*ph = (*ph + 0xE1) >> 1;
		} 
		else 
		{
			*ph = (*ph + 0x161) >> 1;
		}
	}
	/**
	 * [�ϊ�] ISO-2022-JP(JIS)��SJIS�ɕϊ� (RFC 1468)
	 *	@param[in] pIso2022 ����ISO2022JP������B
	 *	@param[out] pSjis �ϊ�����SJIS������B
	 *	@return �ϊ���̕������B
	 */
	static INT_PTR ms_Iso2022jpToSjis(LPCSTR pIso2022, LPSTR pSjis)
	{
		LPCSTR p = pIso2022;
		LPSTR r = pSjis;
		int j = 0;
		bool jiskanji = false;
		bool hankaku = false;
		while ( *p != '\0')
		{
			j++;
			UINT c = *(p++);
			UINT d;
			if ( c == ESC )
			{
				if ( *p == '\0' )
				{
					break;
				}
				if ( (c = *(p++)) == '$' )
				{
					if ( (c = *(p++)) == '@' || c == 'B' )
					{
						jiskanji = true;
					}
					else 
					{
						*(r++) = ESC;
						*(r++) = '$';
						if ( c != '\0' )
						{
							*(r++) = static_cast<BYTE>(c);
						}
					}
				}
				else if ( c == '(' )
				{
					if ( (c = *(p++)) == 'H' || c == 'J' || c == 'B' )
					{
						jiskanji = false;
					}
					else
					{
						jiskanji = false;
						*(r++) = ESC;
						*(r++) = '(';
						if ( c != '\0' )
						{
							*(r++) = static_cast<BYTE>(c);
						}
					}
				}
				else if ( c == '*' )
				{
					if ( (c = *(p++)) == 'B' )
					{
						hankaku = false;
					}
					else if ( c == 'I' )
					{
						hankaku = false;
					}
				}
				else if ( hankaku && c == 'N' )
				{
					c = *(p++);
					*(r++) = static_cast<BYTE>(c + 0x80);
				}
				else if ( c == 'K' )
				{
					jiskanji = true;
				}
				else if ( c == 'H' )
				{
					jiskanji = false;
				}
				else
				{
					*(r++) = ESC;
					if ( c != '\0' )
					{
						*(r++) = static_cast<BYTE>(c);
					}
				}
			}
			else if ( jiskanji && (c == '\r' || c == '\n') )
			{
				jiskanji = false;
				*(r++) = static_cast<BYTE>(c);
			}
			else if ( jiskanji && c >= 0x21 && c <= 0x7E )
			{
				if ( *p == '\0' )
				{
					break;
				}
				if ( (d = *(p++)) >= 0x21 && d <= 0x7E )
				{
					ms_JisToSjis(&c, &d);
				}
				*(r++) = static_cast<BYTE>(c);
				if ( d != '\0' )
				{
					*(r++) = static_cast<BYTE>(d);
				}
			}
			else if ( c >= 0xA1 && c <= 0xFE )
			{
				if ( *p == '\0' )
				{
					break;
				}
				if ( (d = *(p++)) >= 0xA1 && d <= 0xFE )
				{
					d &= 0x7E;
					c &= 0x7E;
					ms_JisToSjis(&c, &d);
				}
				*(r++) = static_cast<BYTE>(c);
				if ( d != '\0' )
				{
					*(r++) = static_cast<BYTE>(d);
				}
			}
			else if ( c == 0x0E )
			{
				while ( *p != '\0' && *p != 0x0F && *p != '\r' && *p != '\n' && *p != ESC )
				{
					*(r++) = static_cast<BYTE>(*(p++) + 0x80);
				}
				if ( *p == 0x0F )
				{
					p++;
				}
			}
			else
			{
				*(r++) = static_cast<BYTE>(c);
			}
		}
		*r = '\0';
		return r - pSjis;
	}
public:

	/// �R���X�g���N�^
	CJisStr(void)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CJisStr(const CJisStr& other) : m_vtcText(other.m_vtcText)
	{
	}

	/**
	 * ����R���X�g���N�^(JIS�p)
	 *	@param lpszJis �������JIS������
	 */
	CJisStr(LPCSTR lpszJis)
	{
		SetOfJis(lpszJis);
	}

	/**
	 * ����R���X�g���N�^(ASCII�p)
	 *	@param ascii �������ASCII������
	 */
	CJisStr(const CAscii& ascii)
	{
		SetOfAscii(ascii);
	}

	/**
	 * ����R���X�g���N�^(UNICODE�p)
	 *	@param unicode �������UNICODE������
	 */
	CJisStr(const CUnicode& unicode)
	{
		SetOfUnicode(unicode);
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CJisStr& operator=(const CJisStr& other)
	{
		m_vtcText = other.m_vtcText;
		return *this;
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CJisStr& t) const
	{
		return STRLIB::Compare(ReferBuffer(), t.ReferBuffer());
	}

	/**
	 * [�ݒ�] ��
	 */
	void Empty(void)
	{
		m_vtcText.Empty();
	}

	/**
	 * [�m�F] ��
	 *	@retval true ��
	 *	@retval false �����L��
	 */
	bool IsEmpty(void) const
	{
		return GetLength() == 0;
	}

	/**
	 * [�ݒ�] ���(JIS�p)
	 *	@param lpszJis �������Jis�^������
	 */
	void SetOfJis(LPCSTR lpszJis)
	{
		if ( lpszJis == NULL )
		{
			Empty();
		}
		else
		{
			size_t iSize = STRLIB::GetLen(lpszJis);
			MemCopy(m_vtcText.GetBuffer(iSize + 1), lpszJis, iSize + 1);
			m_vtcText.ReleaseBuffer();
		}
	}

	/**
	 * [�ݒ�] ���(UNICODE�p)
	 *	@param lpszUnicode �������UNICODE������
	 */
	void SetOfUnicode(LPCWSTR lpszUnicode)
	{
		SetOfAscii(CAscii(lpszUnicode));
	}

	/**
	 * [�ݒ�] ���(ASCII�p)
	 *	@param lpszAscii �������ASCII������
	 */
	void SetOfAscii(LPCSTR lpszAscii)
	{
		TTRACE0("SJIS ������� JIS ������ɕϊ�\n");
		if ( lpszAscii != NULL )
		{
			size_t len = STRLIB::GetLen(lpszAscii);
			if ( len > 0 )
			{
				INT_PTR r = ms_SjisToIso2022jp(lpszAscii, m_vtcText.GetBuffer(len * 4 + 1));
				ASSERT( r < static_cast<INT_PTR>(len * 4 + 1));
				m_vtcText.ReleaseBuffer();
				return;
			}
		}
		Empty();
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^
	 *	@note �����̎����Ă��镶����Ɏw�蕶�����A�����܂��B
	 *	@param j �v���X���
	 *	@return �����̎Q��
	 */
	CJisStr& operator+=(const CJisStr& j)
	{
		m_vtcText += j.m_vtcText;
		return *this;
	}

	/**
	 * [�쐬] ���Z�I�y���[�^
	 *	@note �����̎����Ă��镶����Ǝw�蕶�����A��������������쐬���܂��B
	 *	@param j ���Z������
	 *	@return �V����JIS������
	 */
	CJisStr operator+(const CJisStr& j) const
	{
		CJisStr newJis = *this;
		newJis += j;
		return newJis;
	}

	/**
	 * [�擾] JIS�����񒷂� 
	 *	@return JIS�����񒷂�
	 */
	size_t GetLength(void) const
	{
		return STRLIB::GetLen(ReferBuffer());
	}

	/**
	 * [�擾] UNICODE������擾 
	 *	@return UNICODE������
	 */
	CUnicode ToUnicode(void) const { return CUnicode(ToAscii()); }

	/**
	 * [�擾] ASCII������擾 
	 *	@return ASCII(SJIS)������
	 */
	CAscii ToAscii(void) const
	{
		TTRACE0("Jis ������� S-JIS������ɕϊ�\n");
		CAscii asc;
		size_t len = m_vtcText.GetLength();
		size_t r = ms_Iso2022jpToSjis(m_vtcText, asc.GetBuffer(len));
		ASSERT( r < len );
		asc.ReleaseBuffer();
		return asc;
	}

	/**
	 * [�擾] ������擾 
	 *	@return ������(ASCII/SJIS��UNICODE)
	 */
	CStr ToStr(void) const { return CStr(ToAscii()); }

	/**
	 * [�擾] JIS������Q�Ǝ擾 
	 *	@return JIS������̐擪�|�C���^
	 */
	LPCSTR ReferBuffer(void) const
	{
		return m_vtcText;
	}

	/**
	 * [�擾] JIS������Q�Ǝ擾 
	 *	@return JIS������̐擪�|�C���^
	 */
	operator LPCSTR (void) const
	{
		return ReferBuffer();
	}

	/**
	 * [����] �������݃o�b�t�@�v��
	 *	@note JIS�^���������������ł��������B
	 *	@note �K�v�T�C�Y�̏�������ł��悢�o�b�t�@��Ԃ��܂��B
	 *	@note �g�p��A�K�� ReleaseBuffer() ���R�[�����Ă��������B
	 *	@param iLength �K�v�Ȓ���
	 *	@return �������݉\�A�h���X
	 */
	LPSTR GetBuffer(int iLength)
	{
		return m_vtcText.GetBuffer(iLength);
	}

	/**
	 * [����] ���蓖�Ă��o�b�t�@���J��.
	 *		GetBuffer() �Ŋ��蓖�Ă��o�b�t�@���J�����A�������{�N���X�Ɏ�荞�݂܂��B
	 *	@note GetBuffer()�Ŏ擾�����̈�ɏ������݂��I�������K�� ReleaseBuffer() ���Ă��������B
	 */
	void ReleaseBuffer(void)
	{
		m_vtcText.ReleaseBuffer();
		CJisStr s;
		s.SetOfJis(m_vtcText);
		m_vtcText = s.m_vtcText;
	}

	/**
	 * [�ϊ�] JIS�ɕϊ�
	 *	@param lpszAscii ASCII/SJIS������
	 *	@return JIS������B
	 */
	static CJisStr ToJis(LPCSTR lpszAscii)
	{
		CJisStr j;
		j.SetOfAscii(lpszAscii);
		return j;
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
