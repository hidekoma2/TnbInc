#pragma once
/**
 *@file
 * �����񑀍�֌W�̃w�b�_
 *
 *		������𑀍삷��֐��Q������܂��B	
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"
#include "TnbBlockLinkVector.h"
#include "TnbCollectionAdapter.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * �����񑀍�N���X
 *
 *		�����񑀍�֐��Q�� �N���X�֐��Ŏ����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbStrOperator.h
 * 
 *	@date 06/05/05	�V�K�쐬
 *	@date 06/10/27	�e���v���[�g��
 *	@date 07/07/07	�N���X���ύX�B
 */
template<typename TYP>
class CStrOperatorT
{
public:

	/**
	 * [����] ���s�ʒu����.
	 *		�w�蕶���񂩂���s�̈ʒu���������܂�
	 *	@note ��r(0x0D), ��n(0x0A), ��r��n(0x0D,0x0A) ��������Ή����܂��B
	 *	@param lpsz ������i�J�n�|�C���^�j
	 *	@retval NULL ���s�Ȃ�
	 *	@retval NULL�ȊO ���s�̈ʒu�B
	 */
	static const TYP* FindCr(const TYP* lpsz)
	{
		if ( lpsz != NULL )
		{
			while ( *lpsz != 0 )
			{
				if ( *lpsz == 0x0D || *lpsz == 0x0A )
				{
					return lpsz;
				}
				lpsz += STRLIB::GetCharSize(*lpsz);
			}
		}
		return NULL;
	}

	/**
	 * [�擾] ��s���o��
	 *	@note �w��f�[�^�����s�����o���܂�
	 *	@param[out]	_strLine �擾������s
	 *	@param[in]	lpsz �f�[�^�̓�
	 *	@return ���s�̎��̃|�C���^�B���Ȃ�NULL���Ԃ�܂��B
	 */
	static const TYP* GetLine(CStrT<TYP>& _strLine, const TYP* lpsz)
	{
		_strLine.Empty();
		const TYP* P = NULL;
		if ( lpsz != NULL )
		{
			P = FindCr(lpsz);
			if ( P == NULL )
			{
				//������Ȃ������B�S������s
				_strLine = lpsz;
			}
			else
			{
				INT_PTR iLen = P - lpsz;
				if ( iLen > 0 )
				{
					_strLine.SetFromLeft(lpsz, iLen);
				}
				if ( *P++ == 0x0D )
				{
					if ( *P == 0x0A )
					{
						P++;
					}
				}
			}
		}
		return P;
	}

	/**
	 * [�擾] �e�s�A��
	 *	@note �e�s�����s�ŘA�����܂��B
	 *	@param vs �e�s���
	 *	@param lpszCR ���s�R�[�h�B�ȗ������ 0x0D, 0x0A �ɂȂ�܂��B
	 *	@return �A������
	 */
	static CStrT<TYP> ConnectLines(const CVectorT<CStrT<TYP> >& vs, const TYP* lpszCR = NULL)
	{
		CStrT<TYP> CR = lpszCR;
		if ( lpszCR == NULL )
		{
			CR = "\r\n";
		}
		CStrT<TYP> s;
		loop ( i, vs.GetSize() )
		{
			if ( i != 0 )
			{
				s += CR;
			}
			s += vs[i];
		}
		return s;	
	}

	/**
	 * [�擾] ���s������
	 *	@note �w��f�[�^�����s���Ƃɕ������܂��B
	 *	@param lpsz	�f�[�^�̓�
	 *	@return ������������
	 */
	static CVectorT<CStrT<TYP> > SeparateLine(const TYP* lpsz)
	{
		CBlockLinkVectorT<CStrT<TYP> > vs;
		CStrT<TYP> s;
		const TYP* P = lpsz;
		while ( P != NULL )
		{
			P = GetLine(s, P);
			vs.Add(s);
		}
		CVectorT<CStrT<TYP> > v;
		v.Copy(vs);
		return v;
	}

	/**
	 * [�擾] �g�[�N����؎擾
	 *	@note �w�蕶������w���؂蕶���ŕ��������o���܂��B
	 *	@param lpsz		������������
	 *	@param lpszMark		��؂蕶���Z�b�g
	 *	@param isCheckDc	true�Ȃ� �_�u���R�[�e�[�V�������l���B
	 *						���̊Ԃ̋�؂蕶���͋�؂蕶���Ƃ��Ĉ����܂���B
	 *						�܂��A�������Ă���_�u���R�[�e�[�V�������폜���܂��B
	 *						(�������A��؂蕶���ƃ_�u���R�[�e�[�V�����̊ԂɃX�y�[�X������ƁA�F������܂���)
	 *	@return ��������������
	 */
	static CVectorT<CStrT<TYP> > SeparatePeriod(const TYP* lpsz, const TYP* lpszMark, bool isCheckDc = true)
	{
		CBlockLinkVectorT<CStrT<TYP> > vs;
		CStrT<TYP> s;
		if ( lpsz[0] != 0 )
		{
			while ( true )
			{
				if ( isCheckDc && lpsz[0] == '\"' )
				{
					//�擪�� " �������B
					INT_PTR r = STRLIB::IndexOf(lpsz, '\"', 1);
					if ( r >= 0 )
					{
						//���� " �𔭌�
						bool isEnd = (lpsz[r + 1] == 0);
						if ( STRLIB::IndexOf(lpszMark, lpsz[r + 1]) >= 0 || isEnd )
						{
							// ,"�`", �𔭌�
							s.SetFromLeft(&lpsz[1], r - 1);
							lpsz += r + 2;
							vs.Add(s);
							if ( isEnd )
							{
								break;
							}
							continue;
						}
					}
				}
				INT_PTR r = STRLIB::IndexOneOf(lpsz, lpszMark);
				if ( r < 0 )
				{
					s = lpsz;
				}
				else
				{
					s.SetFromLeft(lpsz, r);
					lpsz += r + 1;
				}
				vs.Add(s);
				if ( r < 0 )
				{
					break;
				}
			}
		}
		CVectorT<CStrT<TYP> > v;
		v.Copy(vs);
		return v;
	}

	/**
	 * [�擾] �g�[�N����؎擾
	 *	@note �w�蕶������w���؂蕶���ŕ��������o���܂��B
	 *	@param lpsz		������������
	 *	@param mark		��؂蕶��
	 *	@param isCheckDc	true�Ȃ� �_�u���R�[�e�[�V�������l���B
	 *						���̊Ԃ̋�؂蕶���͋�؂蕶���Ƃ��Ĉ����܂���B
	 *						�܂��A�������Ă���_�u���R�[�e�[�V�������폜���܂��B
	 *						(�������A��؂蕶���ƃ_�u���R�[�e�[�V�����̊ԂɃX�y�[�X������ƁA�F������܂���)
	 *	@return ��������������
	 */
	static CVectorT<CStrT<TYP> > SeparatePeriod(const TYP* lpsz, TYP mark, bool isCheckDc = true)
	{
		TYP bb[2] = { mark, 0 };
		return SeparatePeriod(lpsz, bb, isCheckDc);
	}

	/**
	 * [�쐬] �o�C�i����HEX������
	 *	@note �w��f�[�^��HEX������\�L������ɕϊ����܂��B
	 *	@param c �f�[�^
	 *	@return HEX������
	 */
	static CStrT<TYP> BinaryToHexString(const IConstCollectionT<BYTE>& c)
	{
		size_t size = c.GetSize();
		CStrT<TYP> str;
		TYP* P = str.GetBuffer(size * 2 + 2);
		loop ( i, size )
		{
			const BYTE& b = c.At(i);
			*P++ = STRLIB::IntToHexChar(b >> 4);
			*P++ = STRLIB::IntToHexChar(b);
		}
		*P++ = 0;
		str.ReleaseBuffer();
		return str;
	}

	/**
	 * [�쐬] �o�C�i����HEX������
	 *	@note �w��f�[�^��HEX������\�L������ɕϊ����܂��B
	 *	@param dataSize �f�[�^�T�C�Y
	 *	@param P �f�[�^�A�h���X
	 *	@return HEX������
	 */
	static CStrT<TYP> BinaryToHexString(size_t dataSize, LPCVOID P)
	{
		CConstAdapterT<BYTE> cc(dataSize, static_cast<const BYTE*>(P));
		return BinaryToHexString(cc);
	}

	/**
	 * [�쐬] HEX�����񁨃o�C�i��
	 *	@note �w��f�[�^��HEX������\�L������ɕϊ����܂��B
	 *	@note �A���_�[�o�[�A�X�y�[�X�A�^�u�A���s�A�n�C�t���́A�������܂��B
	*	@note �r����HEX�ȊO�̕���������ƁA�����܂ł̏��������܂��B
	 *	@param lpszHex HEX������
	 *	@return �o�C�i���f�[�^�B
	 */
	static CByteVector HexStringToBinary(const TYP* lpszHex)
	{
		CByteVector vb;
		while ( true )
		{
			TYP c = *lpszHex++;
			if ( c == '_' || c ==' ' || c == '\t' || c =='\n' || c == '\r' || c =='-' )
			{
				continue;
			}
			int r = STRLIB::HexCharToInt(c);
			if ( r < 0 )
			{
				break;
			}
			vb.Add(static_cast<BYTE>(r));
		}
		CByteVector vbr;
		if ( ! vb.IsEmpty() )
		{
			INDEX startIndex = 0;
			size_t size = vb.GetSize();
			if ( (size & 1) != 0 )
			{
				vbr.Add(vb[0]);
				startIndex = 1;
			}
			for ( INDEX i = startIndex; i < size; i += 2 )
			{
				vbr.Add(static_cast<BYTE>((vb[i] << 4) | vb[i + 1]));
			}
		}
		return vbr;
	}

	/**
	 * �^�u�R�[�h���X�y�[�X�ɕϊ����܂��B<BR>
	 * ���s�͊܂܂Ȃ��悤�ɂ��Ă��������B
	 * @param lpsz ���̃e�L�X�g�B
	 * @param iTabStep �^�u�X�e�b�v���B�ȗ�����ƂS�ɂȂ�܂��B
	 * @return �^�O�R�[�h�̊܂܂Ȃ�������B
	 */
	static CStrT<TYP> TabToSpace(const TYP* lpsz, int iTabStep = 4)
	{
		CStrT<TYP> strWork;
		strWork = lpsz;
		while ( true )
		{
			INT_PTR iTabPosition = strWork.Find('\t');
			if ( iTabPosition < 0)
			{
				break;
			}
			strWork = 
					strWork.Left(iTabPosition) 
					+ CStrT<TYP>::Lineup(' ', iTabStep - (iTabPosition % iTabStep))
					+ &(strWork.operator const TYP*()[iTabPosition + 1]);
		}
		return strWork;
	}

private:
	friend class CStrOperatorTest;
};



/**
 * �ȈՕ����񑀍�֐��Q
 *
 *	@par�K�v�t�@�C��
 *			TnbStrOperator.h
 * 
 *	@date 06/10/27	�e���v���[�g��
 */
typedef CStrOperatorT<TCHAR> CStrOperator;

/**
 * �ȈՕ����񑀍�֐��Q(ASCII/SJIS)
 *
 *	@par�K�v�t�@�C��
 *			TnbStrOperator.h
 * 
 *	@date 06/10/27	�e���v���[�g��
 */
typedef CStrOperatorT<char> CAsciiOperator;

/**
 * �ȈՕ����񑀍�֐��Q(UNICODE)
 *
 *	@par�K�v�t�@�C��
 *			TnbStrOperator.h
 * 
 *	@date 06/10/27	�e���v���[�g��
 */
typedef CStrOperatorT<WCHAR> CUnicodeOperator;



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")

