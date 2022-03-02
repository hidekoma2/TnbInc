#pragma once
/**
 *@file
 * Double Null Terminate(DNT)�^�����񑀍�֌W�̃w�b�_
 *
 *		Windows API �̈����A�߂�l�ɂ���A�����̕�������������@�̈�A
 *		�u��0���d�؂�A��0��0���I�[�v�Ƃ���������������N���X�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * Double Null Terminate(DNT)�^������Ǘ�
 *
 *		Windows API �̈����A�߂�l�ɂ���A�����̕�������������@�̈�A
 *		�u��0���d�؂�A��0��0���I�[�v�Ƃ���������������N���X�ł��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		CStrVector vstrSecNames;
 *		CDoubleNullTerminateStr dntstr;
 *		DWORD dwRc = ::GetPrivateProfileSectionNames( //DNT�^�Ŏ󂯂�		
 *		                   dntstr.GetBuffer(65536),
 *		                   65536 , _T("Abc.ini") );
 *		dntstr.ReleaseBuffer();
 *		if ( dwRc > 0 )
 *		{
 *			vstrSecNames = dntstr.ToStrVector(); //������z��̕���		
 *		}
 *		return vstrSecNames;
 *	</pre></td></tr></table>
 *
 *	@note	printf�n�� Format() Fmt() �� ��s �ɑ΂��āA���̂܂ܓn���܂���B
 *			CStr �ɕϊ����Ă���n���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbDntStr.h
 * 
 *	@date 06/05/07 �V�K�쐬
 *	@date 06/05/19 IComparableT �����B
 */
class CDoubleNullTerminateStr : public IComparableT<TNB::CDoubleNullTerminateStr>
{
	CVectorT<TCHAR> m_vtcText;			///< ���
public:

	/**
	 * �R���X�g���N�^
	 *	@param lpszStr �i�[���镶����
	 */
	CDoubleNullTerminateStr(LPCTSTR lpszStr = NULL)
	{
		Set(lpszStr);
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param d �R�s�[��
	 */
	CDoubleNullTerminateStr(const CDoubleNullTerminateStr& d) : m_vtcText(d.m_vtcText) 
	{
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CDoubleNullTerminateStr& t) const
	{
		return STRLIB::Compare(ToString(), t.ToString());
	}

	/**
	 * [�ݒ�] ��
	 */
	void Empty(void)
	{
		m_vtcText.RemoveAll();
		m_vtcText.Add(0);
		m_vtcText.Add(0);
	}

	/**
	 * [�ݒ�] ���
	 *	@param lpszzDntStr �i�[���� DNT�^������
	 *	@param tcSepaChar �~���L�����B�ȗ�����ƂO�ł��B
	 */
	void SetDntStr(LPCTSTR lpszzDntStr, TCHAR tcSepaChar = 0)
	{
		if ( lpszzDntStr != NULL )
		{
			LPCTSTR P = lpszzDntStr;
			int i = 0;
			while ( P[i] != tcSepaChar || P[i + 1] != tcSepaChar )
			{
				i++;
			}
			m_vtcText.SetElements(i + 2, lpszzDntStr);
			if ( tcSepaChar != 0 )
			{
				loop ( i, m_vtcText.GetSize() )
				{
					if ( m_vtcText[i] == tcSepaChar )
					{
						m_vtcText.Set(i, 0);
					}
				}
			}
		}
		else
		{
			Empty();
		}
	}

	/**
	 * [�ݒ�] ���
	 *	@param lpszStr �i�[���镶����
	 */
	void Set(LPCTSTR lpszStr)
	{
		Empty();
		Add(lpszStr);
	}

	/**
	 * [�ݒ�] ���
	 *	@note �z���DNT�^�Ŋi�[���܂��B
	 *	@param vstr ������� CStrVector �̃C���X�^���X
	 */
	void Set(const CStrVector& vstr)
	{
		Empty();
		Add(vstr);
	}

	/**
	 * [�ݒ�] �ǉ�.
	 *		�ێ����Ă��� DNT�^������Ɏw��̒ʏ�̕������ǉ����܂��B
	 *	@param lpszStr �i�[���镶����
	 */
	void Add(LPCTSTR lpszStr)
	{
		if ( lpszStr != NULL )
		{
			size_t iStrSize = STRLIB::GetLen(lpszStr);
			if ( iStrSize > 0 )
			{
				size_t iVecSize = m_vtcText.GetSize();
				if ( iVecSize > 2 )
				{
					//Empty����Ȃ���NULL����c���B
					m_vtcText.SetSize(iVecSize - 1);
					m_vtcText.AddElements(iStrSize, lpszStr);
				}
				else
				{
					m_vtcText.SetElements(iStrSize, lpszStr);
				}
				m_vtcText.Add(0);
				m_vtcText.Add(0);
			}
		}
	}

	/**
	 * [�ݒ�] �ǉ�.
	 *		�ێ����Ă��� DNT�^������ɁA�z����e��DNT�^�Ŋi�[���܂��B
	 *	@param vstr �ǉ����� CStrVector �̃C���X�^���X
	 */
	void Add(const CStrVector& vstr)
	{
		loop ( i, vstr.GetSize() )
		{
			Add(vstr[i]);
		}
	}

	/**
	 * [�擾] DNT�^������擾 
	 *	@return DNT�^������̎Q��
	 */
	operator LPCTSTR(void) const
	{
		return m_vtcText.ReferBuffer();
	}

	/**
	 * [�擾] DNT�^�����񐔎��
	 *	@return DNT�^�����񐔁iNULL��܂ޒ����j
	 */
	size_t GetSize(void) const
	{
		return m_vtcText.GetSize();
	}

	/**
	 * [�擾] ������z��擾.
	 *		�ێ����Ă��� DNT�^��������I�[�ŕ������A CStrVector �ɕϊ����Ԃ��܂��B
	 *	@return CStrVector
	 */
	CStrVector ToStrVector(void) const
	{
		CStrVector vstr;
		LPCTSTR P = m_vtcText.ReferBuffer();
		while ( true )
		{
			vstr.Add(P);
			P += STRLIB::GetLen(P) + 1;
			if ( *P == 0 )
			{
				break;
			}
		}
		return vstr;
	}

	/**
	 * [�擾] ������擾.
	 *		�ێ����Ă��� DNT�^������̎d�؂蕶�����w��̕����ɒu�������A
	 *		�ʏ�̕�����ɕϊ����Ԃ��܂��B
	 *	@param tcSepaChar �d�؂蕶���A�ȗ������ '|' �ɂȂ�܂��B
	 *	@return �ϊ���̕��ʂ̕�����B
	 */
	CStr ToString(TCHAR tcSepaChar = '|') const
	{
		CStr strResult;
		CStrVector vstr = ToStrVector();
		loop ( i, vstr.GetSize() )
		{
			strResult += vstr[i];
			strResult += tcSepaChar;
		}
		strResult += tcSepaChar;
		return strResult;
	}

	/**
	 * [����] �������݃o�b�t�@�v��.
	 *		�K�v�T�C�Y�̏�������ł��悢�o�b�t�@��Ԃ��܂��B
	 *	@note DNT�^���������������ł��������B
	 *	@note �g�p��A�K�� ReleaseBuffer() ���R�[�����Ă��������B
	 *	@param iLength �K�v�Ȓ���
	 *	@return �������݉\�A�h���X
	 */
	LPTSTR GetBuffer(int iLength)
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
		CDoubleNullTerminateStr str;
		str.SetDntStr(m_vtcText.ReferBuffer());
		m_vtcText = str.m_vtcText;
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
