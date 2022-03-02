#pragma once
/**
 *@file
 * CSV�Ǘ��֌W�̃w�b�_
 *
 *		CSV�t�@�C����ǂݍ��݊Ǘ�����N���X���L��܂��B
 *		ASCII�݂̂ł��B������̂͐��l�݂̂ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBlocklinkVector.h"
#include "TnbFile.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �Ȉ�CSV�t�@�C���ǂݍ��݃N���X.
 *	
 *		ASCII �݂̂ŏ����ꂽ���l�݂̂�CSV�t�@�C���������œǂݍ��ނ��Ƃ��ł��܂��B
 *
 *	@date 19/06/15 �V�K�쐬
 */
class CTinyCsvReader
{
public:

	/**
	 * [�Ǎ�] CSV�ǂݍ���.
	 *	@param lpszFileName �t�@�C����.
	 *	@retval true ����. �C���X�^���X�Ƀf�[�^�������܂��B
	 *	@retval false ���s
	 */
	bool ReadCsv(LPCTSTR lpszFileName)
	{
		m_content.RemoveAll();
		CByteVector vb;
		if ( CFileReader::ReadAll(vb, lpszFileName) )
		{
			vb.Add(0);	//�I�[
			LPCSTR lpsz = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
			m_content = m_SeparateLine(lpsz);
			return true;
		}
		return false;
	}

	/**
	 * [�擾] �s���擾.
	 *	@return �ێ����Ă�����e�̍s�����擾���܂��B
	 */
	size_t GetLineCount(void) const
	{
		return m_content.GetSize();
	}
		
	/**
	 * [�擾] �f�[�^�擾
	 *	@note �w�胉�C���̃f�[�^�����o���܂��B
	 *	@param line �s��
	 *	@param emptyValue �l�Ȃ��̏ꍇ�̒l���w�肵�܂��B
	 *	@return �z��f�[�^.
	 */
	CVectorT<int> Get(INDEX line, int emptyValue) const
	{
		CBlockLinkVectorT<int> vs;
		if ( m_content.GetSize() > line )
		{
			LPCSTR lpsz = m_content[line].ReferBuffer();
			int s;
			if ( lpsz[0] != 0 )
			{
				while ( true )
				{
					INT_PTR r = m_FindComma(lpsz);
					if ( r < 0 )
					{
						s = (*lpsz == 0) ? emptyValue : ::atoi(lpsz);
					}
					else if ( r == 0 )
					{
						s = emptyValue;
						lpsz++;
					}
					else
					{
						s = ::atoi(lpsz);
						lpsz += r + 1;
					}
					vs.Add(s);
					if ( r < 0 )
					{
						break;
					}
				}
			}
		}
		CVectorT<int> v;
		v.Copy(vs);
		return v;
	}

	/**
	 * [�擾] �f�[�^�擾
	 *	@note �w�胉�C���̃f�[�^�����o���܂��B
	 *	@param line �s��
	 *	@param emptyValue �l�Ȃ��̏ꍇ�̒l���w�肵�܂��B
	 *	@return �z��f�[�^.
	 */
	CVectorT<double> Get(INDEX line, double emptyValue) const
	{
		CBlockLinkVectorT<double> vs;
		if ( m_content.GetSize() > line )
		{
			LPCSTR lpsz = m_content[line].ReferBuffer();
			double s;
			if ( lpsz[0] != 0 )
			{
				while ( true )
				{
					INT_PTR r = m_FindComma(lpsz);
					if ( r < 0 )
					{
						s = (*lpsz == 0) ? emptyValue : ::atof(lpsz);
					}
					else if ( r == 0 )
					{
						s = emptyValue;
						lpsz++;
					}
					else
					{
						s = ::atof(lpsz);
						lpsz += r + 1;
					}
					vs.Add(s);
					if ( r < 0 )
					{
						break;
					}
				}
			}
		}
		CVectorT<double> v;
		v.Copy(vs);
		return v;
	}

private:

	/**
	 * [����] ���s�ʒu����.
	 *		�w�蕶���񂩂���s�̈ʒu���������܂�
	 *	@note ��r(0x0D), ��n(0x0A), ��r��n(0x0D,0x0A) ��������Ή����܂��B
	 *	@param lpsz ������i�J�n�|�C���^�j
	 *	@retval NULL ���s�Ȃ�
	 *	@retval NULL�ȊO ���s�̈ʒu�B
	 */
	inline const char* m_FindCr(const char* lpsz) const
	{
		while ( *lpsz != 0 )
		{
			if ( *lpsz == 0x0D )
			{
				return lpsz;
			}
			lpsz ++;
		}
		return NULL;
	}

	/**
	 * [����] �J���}�ʒu����.
	 *		�w�蕶���񂩂���s�̈ʒu���������܂�
	 *	@note ��r(0x0D), ��n(0x0A), ��r��n(0x0D,0x0A) ��������Ή����܂��B
	 *	@param lpsz ������i�J�n�|�C���^�j
	 *	@retval NULL ���s�Ȃ�
	 *	@retval NULL�ȊO ���s�̈ʒu�B
	 */
	inline INT_PTR m_FindComma(const char* lpsz) const
	{
		INT_PTR r = 0;
		while ( *lpsz != 0 )
		{
			if ( *lpsz == ',' )
			{
				return r;
			}
			lpsz++;
			r++;
		}
		return -1;
	}

	/**
	 * [�擾] ��s���o��
	 *	@note �w��f�[�^�����s�����o���܂�
	 *	@param[out]	_strLine �擾������s
	 *	@param[in]	lpsz �f�[�^�̓�
	 *	@return ���s�̎��̃|�C���^�B���Ȃ�NULL���Ԃ�܂��B
	 */
	const char* m_GetLine(CStrT<char>& _strLine, const char* lpsz)
	{
		_strLine.Empty();
		const char* P = NULL;
		P = m_FindCr(lpsz);
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
				char* Q = _strLine.GetBuffer(iLen);
				memcpy(Q, lpsz, iLen);
				Q[iLen] = 0;
				_strLine.ReleaseBuffer();
			}
			if ( *P++ == 0x0D )
			{
				if ( *P == 0x0A )
				{
					P++;
				}
			}
		}
		return P;
	}

	/**
	 * [�擾] �����񖈂ɕ���
	 *	@param lpsz �f�[�^�̓�
	 *	@return ���s���Ƃɕ�������������̔z��
	 */
	CVectorT<CStrT<char> > m_SeparateLine(const char* lpsz)
	{
		CBlockLinkVectorT<CStrT<char> > vs;
		CStrT<char> s;
		const char* P = lpsz;
		while ( P != NULL )
		{
			P = m_GetLine(s, P);
			vs.Add(s);
		}
		CVectorT<CStrT<char> > v;
		v.Copy(vs);
		return v;
	}

	CStrVector m_content;
};



}; // TNB