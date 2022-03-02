#pragma once
/**
 *@file
 * BASE64�^�Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbVector.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * BASE64��������ϊ����ۃN���X
 *
 *		MIME BASE64 ������ƃo�C�i���̑��ݕϊ����T�|�[�g���钊�ۃN���X�ł��B
 *
 *	@note ���̂܂܎g�p���邱�Ƃ͂���܂���B
 *
 *	@see CBase64Str
 *
 *	@par�K�v�t�@�C��
 *			TnbBase64Str.h
 * 
 *	@date 06/10/11	���ۉ����č쐬�B
 */
template<typename TYP>
class CAbstractBase64T : CCopyImpossible
{
public:
	/// �萔
	enum{ 
		PADDING = 0x10000		///< �p�e�B���O�R�[�h
	};

private:

	const TYP* m_lpszResult;	///< ��͏I���|�C���^

	/*
	 * BASE64�����𐔒l�ɕϊ�
	 *	@param c BASE64�����B
	 *	@retval -1		Base64�̕����ł͂Ȃ�����
	 *	@retval PADDING	�p�f�B���O�����������B
	 *	@retval 0�`63	BASE64�������B
	 */
	virtual int m_Base64ToInt(TYP c) = 0;

	/*
	 * ���l��BASE64�����ɕϊ�
	 *	@param i ���l�BPADDING �� 0�`63�B
	 *	@retval -1 �͈̓G���[(ASSERT����܂�)
	 *	@retval 0�ȏ� BASE64�����B
	 */
	virtual TYP m_IntToBase64(int i) = 0;

	/*
	 * �p�f�B���O��\��BASE64����
	 *	@retval BASE64�����B
	 */
	TYP m_PaddingCode(void)
	{
		return m_IntToBase64(PADDING);
	}

	/*
	 * �󔒕������X�L�b�v
	 */
	const TYP* m_SkipSpace(const TYP* P)
	{
		while ( *P == ' ' || *P == '\r' || *P == '\n' || *P == '\t' )
		{
			P++;
		}
		return P;
	}

public:

	/// �R���X�g���N�^
	CAbstractBase64T(void) : m_lpszResult(NULL)
	{
	}

	/**
	 * [�ϊ�] �f�R�[�h
	 *	@note NULL�I�[��BASE64�ȊO�̕����������܂Ńf�R�[�h���܂��B
	 *		�f�[�^��4�̔{���łȂ��ꍇ�A�G���[�ɂȂ�܂��B
	 *	@note �X�y�[�X�A�^�u�A���s�R�[�h�͖������ăf�R�[�h���܂��B
	 *	@param lpszBase64 BASE64�t�H�[�}�b�g�̃e�L�X�g�B
	 *	@return �f�R�[�h����
	 */
	CVectorT<BYTE> Decode(const TYP* lpszBase64)
	{
		m_lpszResult = NULL;
		CByteVector vb;
		const TYP* P = m_SkipSpace(lpszBase64);
		INT_PTR iLen = STRLIB::GetLen(P);
		while ( iLen > 0 )
		{
			const TYP* Q = P;
			if ( iLen < 4 )
			{
				if ( 0 <= m_Base64ToInt(*P) )
				{
					vb.Invalid();
				}
				break;
			}
			int r1 = m_Base64ToInt(*P);
			if ( r1 < 0 )
			{
				break;	//����
			}
			P = m_SkipSpace(P + 1);
			int r2 = m_Base64ToInt(*P);
			P = m_SkipSpace(P + 1);
			int r3 = m_Base64ToInt(*P);
			P = m_SkipSpace(P + 1);
			int r4 = m_Base64ToInt(*P);
			P = m_SkipSpace(P + 1);
			if ( r1 >= 0x100 || r2 >= 0x100 || r2 < 0 || r3 < 0 || r4 < 0 )
			{
				vb.Invalid();
				break;
			}
			vb.Add(static_cast<BYTE>(((r1<<2)&0xFC) | ((r2>>4)&0x03)));
			if ( r3 < 0x0100 )
			{
				vb.Add(static_cast<BYTE>(((r2<<4)&0xF0) | ((r3>>2)&0x0F)));
				if ( r4 < 0x0100 )
				{
					vb.Add(static_cast<BYTE>(((r3<<6)&0xC0) | ((r4)&0x3F)));
				}
			}
			iLen -= (P-Q);
			if ( r3 >= 0x0100 || r4 >= 0x0100 )
			{
				break;
			}
		}
		m_lpszResult = P;
		return vb;
	}

	/**
	 * [�ϊ�] �f�R�[�h
	 *	@note NULL�I�[��BASE64�ȊO�̕����������܂Ńf�R�[�h���܂��B
	 *		�f�[�^��4�̔{���łȂ��ꍇ�A�G���[�ɂȂ�܂��B
	 *	@note �X�y�[�X�A�^�u�A���s�R�[�h�͖������ăf�R�[�h���܂��B
	 *	@attention �G���R�[�h�O���e�L�X�g�łȂ��f�[�^�́A�{�֐��Ńf�R�[�h���Ȃ��ŉ������B
	 *	@param lpszBase64 BASE64�t�H�[�}�b�g�̃e�L�X�g�B
	 *	@return �f�R�[�h���ʁB
	 */
	CAscii DecodeForStr(const TYP* lpszBase64)
	{
		CAscii asc;
		CByteVector vb = Decode(lpszBase64);
		if ( ! vb.IsEmpty() )
		{
			vb.Add(0);	//NULL�I�[�ǉ�
			asc = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
		}
		return asc;
	}

	/**
	 * [�擾] �f�R�[�h�I���A�h���X
	 *	@note Decode() / DecodeForStr() �Ő�����A�f�R�[�h�̒�~�����A�h���X�𓾂邱�Ƃ��o���܂��B
	 *	@return �A�h���X
	 */
	const TYP* GetDecodeEndPtr(void) const
	{
		return m_lpszResult;
	}
	
	/**
	 * [�ϊ�] �G���R�[�h
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@return �G���R�[�h���ʁB
	 */
	virtual CStrT<TYP> Encode(size_t size, LPCVOID P)
	{
		CStrT<TYP> strRc;
		const BYTE* B = static_cast<const BYTE*>(P);
		int r;
		while ( size > 0 )
		{			
			if ( size == 1 )
			{
				r = B[0]>>2;
				strRc += m_IntToBase64(r);
				r = ((B[0]<<4)&0x30);
				strRc += m_IntToBase64(r);
				strRc += m_PaddingCode();
				strRc += m_PaddingCode();
				break;
			}
			else if ( size == 2 )
			{
				r = B[0]>>2;
				strRc += m_IntToBase64(r);
				r = ((B[0]<<4)&0x30) | ((B[1]>>4)&0x0F);
				strRc += m_IntToBase64(r);
				r = ((B[1]<<2)&0x3C);
				strRc += m_IntToBase64(r);
				strRc += m_PaddingCode();
				break;
			}
			r = B[0]>>2;
			strRc += m_IntToBase64(r);
			r = ((B[0]<<4)&0x30) | ((B[1]>>4)&0x0F);
			strRc += m_IntToBase64(r);
			r = ((B[1]<<2)&0x3C) | ((B[2]>>6)&0x03);
			strRc += m_IntToBase64(r);
			r = B[2]&0x3F;
			strRc += m_IntToBase64(r);
			size -= 3;
			B += 3;
		}
		return strRc;
	}

	/**
	 * [�ϊ�] �G���R�[�h
	 *	@param lpsz �f�[�^
	 *	@return �G���R�[�h���ʁB
	 */
	CStrT<TYP> Encode(LPCSTR lpsz)
	{
		return Encode(STRLIB::GetLen(lpsz), lpsz);
	}

	/**
	 * [�ϊ�] �G���R�[�h
	 *	@param c �f�[�^
	 *	@return �G���R�[�h���ʁB
	 */
	CStrT<TYP> Encode(const ISequenceCollectionT<BYTE>& c)
	{
		return Encode(c.GetSize(), c.ReferBuffer());
	}
};



/**@ingroup STRING
 * MIME BASE64�^������ϊ�
 *
 *		MIME BASE64 ������ƃo�C�i���̑��ݕϊ����T�|�[�g����N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBase64Str.h
 * 
 *	@date 06/09/27	�V�K�쐬
 *	@date 06/10/11	���ۉ������N���X���p��
 */
class CBase64Str : public CAbstractBase64T<char>
{
	DEFSUPER(CAbstractBase64T<char>);

	/**
	 * BASE64�����𐔒l�ɕϊ�
	 *	@param c BASE64�����B
	 *	@retval -1		Base64�̕����ł͂Ȃ�����
	 *	@retval PADDING	�p�f�B���O�����������B
	 *	@retval 0�`63	BASE64�������B
	 */
	virtual int m_Base64ToInt(char c)
	{
		int r = -1;
		if ( 'A' <= c && c <= 'Z' )
		{
			r = c - 'A';
		}
		else if ( 'a' <= c && c <= 'z' )
		{
			r = (c - 'a') +26;
		}
		else if ( '0' <= c && c <= '9' ) 
		{
			r = (c - '0') +52;
		}
		else if ( '+' == c )
		{
			r = 62;
		}
		else if ( '/' == c ) 
		{
			r = 63;
		}
		else if ( '=' == c )
		{
			r = PADDING;
		}
		ASSERTLIB(r < 0 || m_IntToBase64(r) == c);
		return r;
	}

	/**
	 * ���l��BASE64�����ɕϊ�
	 *	@param i ���l�BPADDING �� 0�`63�B
	 *	@retval -1 �͈̓G���[(ASSERT����܂�)
	 *	@retval -1�ȊO BASE64�����B
	 */
	virtual char m_IntToBase64(int i)
	{
		const char* acAlpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		ASSERTLIB(STRLIB::GetLen(acAlpha) == 64);
		if ( i == PADDING )
		{
			return '=';
		}
		else if ( i >= 0 && i < 64 )
		{
			return acAlpha[i];
		}
		ASSERTLIB(false);
		return -1;
	}
};



/**@ingroup STRING
 * DQ�����̎�����������ϊ�
 *
 *		DQ�̕����̎�����������ƃo�C�i���̑��ݕϊ����T�|�[�g����N���X�ł��B
 *
 *	@note BASE64 �̎g�p����65�̕������Ђ炪�ȂɕύX���������̂��̂ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBase64Str.h
 * 
 *	@date 06/10/11	�V�K�쐬
 */
class CDqSaveStr : public CAbstractBase64T<WCHAR>
{
	DEFSUPER(CAbstractBase64T<WCHAR>);

	const WCHAR* m_list(void)
	{
		return 	L"��������������������"
				L"���������������Ă�"
				L"�Ȃɂʂ˂̂͂Ђӂւ�"
				L"�܂݂ނ߂�������"
				L"����킪����������"
				L"�������������Âłǂ�"
				L"�тԂׂ�";
	}

	/**
	 * BASE64�����𐔒l�ɕϊ�
	 *	@param c BASE64�����B
	 *	@retval -1		Base64�̕����ł͂Ȃ�����
	 *	@retval PADDING	�p�f�B���O�����������B
	 *	@retval 0�`63	BASE64�������B
	 */
	virtual int m_Base64ToInt(WCHAR c)
	{
		const WCHAR* aa = m_list();
		if ( L'��' == c )
		{
			return PADDING;
		}
		loop ( i, 64 )
		{
			if ( *aa++ == c )
			{
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	/**
	 * ���l��BASE64�����ɕϊ�
	 *	@param i ���l�BPADDING �� 0�`63�B
	 *	@retval 0xFFFF �͈̓G���[(ASSERT����܂�)
	 *	@retval 0xFFFF�ȊO BASE64�����B
	 */
	virtual WCHAR m_IntToBase64(int i)
	{
		const WCHAR* aa = m_list();
		ASSERTLIB(STRLIB::GetLen(aa) == 64);
		if ( i == PADDING )
		{
			return L'��';
		}
		else if ( i >= 0 && i < 64 )
		{
			return aa[i];
		}
		ASSERTLIB(false);
		return 0xFFFF;
	}
public:
#if 0
	/**
	 * [�ϊ�] �G���R�[�h
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@return �G���R�[�h���ʁB
	 */
	CUnicode Encode(size_t size, LPCVOID P)
	{
		CUnicode r;
		CUnicode s = _super::Encode(size, P);
		while ( ! s.IsEmpty() )
		{
			r += s.Left(3);
			s = s.Mid(3);
			if ( s.IsEmpty() )
			{
				break;
			}
			r += ' ';
			r += s.Left(3);
			s = s.Mid(3);
			if ( s.IsEmpty() )
			{
				break;
			}
			r += ' ';
			r += s.Left(4);
			s = s.Mid(4);
			if ( s.IsEmpty() )
			{
				break;
			}
			r += L"\r\n";
		}
		return r;
	}
#endif
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")



