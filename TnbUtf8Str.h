#pragma once
/**
 *@file
 * Unicode-Translation Format-8(UTF8)�^������Ǘ��֌W�̃w�b�_
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
 * Unicode-Translation-Format-8(UTF8)�^������Ǘ�
 *
 *		UTF8�^�̕��������ێ�����N���X�ł��B
 *		ASCII,UNICODE�Ƃ̑��ݕϊ��p�̃��\�b�h������Ă��܂��B
 *
 *	@note	printf�n�� Format() Fmt() �� ��s �ɑ΂��āA���̂܂ܓn���܂���B
 *			CStr �ɕϊ����Ă���n���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbUft8Str.h
 * 
 *	@date 06/05/18	�V�K�쐬
 *	@date 06/05/19 IComparableT �����B
 */
class CUtf8Str : public IComparableT<TNB::CUtf8Str>
{
	CVectorT<char> m_vtcText;			///< ���
public:

	/// �R���X�g���N�^
	CUtf8Str(void)
	{
		Empty();
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param uft8 �R�s�[��
	 */
	CUtf8Str(const CUtf8Str& uft8) : m_vtcText(uft8.m_vtcText) 
	{
	}

	/**
	 * ����R���X�g���N�^(UTF8�p)
	 *	@param lpszUtf8 �������UTF8�^������
	 */
	CUtf8Str(LPCSTR lpszUtf8)
	{
		SetOfUtf8(lpszUtf8);
	}

	/**
	 * ����R���X�g���N�^(ASCII�p)
	 *	@param ascii �������ASCII������
	 */
	CUtf8Str(const CAscii& ascii)
	{
		SetOfAscii(ascii);
	}

	/**
	 * ����R���X�g���N�^(UNICODE�p)
	 *	@param unicode �������UNICODE������
	 */
	CUtf8Str(const CUnicode& unicode)
	{
		SetOfUnicode(unicode);
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CUtf8Str& operator=(const CUtf8Str& other)
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
	virtual INT_PTR Compare(const CUtf8Str& t) const
	{
		return STRLIB::Compare(ReferBuffer(), t.ReferBuffer());
	}

	/**
	 * [�ݒ�] ��
	 */
	void Empty(void)
	{
		m_vtcText.RemoveAll();
		m_vtcText.Add(0);
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
	 * [�ݒ�] ���(UTF8�p)
	 *	@param lpszUtf8 �������UTF8�^������
	 */
	void SetOfUtf8(LPCSTR lpszUtf8)
	{
		if ( lpszUtf8 == NULL )
		{
			Empty();
		}
		else
		{
			size_t iSize = STRLIB::GetLen(lpszUtf8);
			MemCopy(m_vtcText.GetBuffer(iSize + 1), lpszUtf8, iSize + 1);
			m_vtcText.ReleaseBuffer();
		}
	}

	/**
	 * [�ݒ�] ���(UNICODE�p)
	 *	@param lpszUnicode �������UNICODE������
	 */
	void SetOfUnicode(LPCWSTR lpszUnicode)
	{
		TTRACE0("UNICODE ������� UTF8 ������ɕϊ�\n");
		if ( lpszUnicode != NULL )
		{
			size_t iSize = STRLIB::GetLen(lpszUnicode);
			if ( iSize > 0 )
			{
				int nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpszUnicode, -1, NULL, 0, NULL, NULL);
				::WideCharToMultiByte(CP_UTF8, 0, lpszUnicode, -1, m_vtcText.GetBuffer(nLen), nLen, NULL, NULL);
				m_vtcText.ReleaseBuffer();
				return;
			}
		}
		Empty();
	}

	/**
	 * [�ݒ�] ���(ASCII�p)
	 *	@param lpszAscii �������ASCII������
	 */
	void SetOfAscii(LPCSTR lpszAscii)
	{
		SetOfUnicode(CUnicode(lpszAscii));
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^
	 *	@note �����̎����Ă��镶����Ɏw�蕶�����A�����܂��B
	 *	@param uft8 �v���X���
	 *	@return �����̎Q��
	 */
	CUtf8Str& operator+=(const CUtf8Str& uft8)
	{
		m_vtcText.SetSize(GetLength());
		m_vtcText += uft8.m_vtcText;
		return *this;
	}

	/**
	 * [�쐬] ���Z�I�y���[�^
	 *	@note �����̎����Ă��镶����Ǝw�蕶�����A��������������쐬���܂��B
	 *	@param uft8 ���Z������
	 *	@return �V����UTF8������
	 */
	CUtf8Str operator+(const CUtf8Str& uft8) const
	{
		CUtf8Str newUTF8 = *this;
		newUTF8 += uft8;
		return newUTF8;
	}

	/**
	 * [�擾] UTF8�����񒷂� 
	 *	@return UTF8�����񒷂�
	 */
	size_t GetLength(void) const
	{
		return STRLIB::GetLen(ReferBuffer());
	}

	/**
	 * [�擾] UNICODE������擾 
	 *	@return UNICODE������
	 */
	CUnicode ToUnicode(void) const
	{
		TTRACE0("UTF8 ������� UNICODE ������ɕϊ�\n");
		LPCSTR P = m_vtcText.ReferBuffer();
		int nLen = ::MultiByteToWideChar(CP_UTF8, 0, P, -1, NULL, 0); 
		CUnicode uni;
		::MultiByteToWideChar(CP_UTF8, 0, P, -1, uni.GetBuffer(nLen), nLen); 
		uni.ReleaseBuffer();
		return uni;
	}

	/**
	 * [�擾] ASCII������擾 
	 *	@return ASCII(SJIS)������
	 */
	CAscii ToAscii(void) const
	{
		return CAscii(ToUnicode()); 
	}

	/**
	 * [�擾] ������擾 
	 *	@return ������(ASCII/SJIS��UNICODE)
	 */
	CStr ToStr(void) const
	{
		return CStr(ToUnicode());
	}

	/**
	 * [�擾] UTF8������Q�Ǝ擾 
	 *	@return UTF8������̐擪�|�C���^
	 */
	LPCSTR ReferBuffer(void) const
	{
		return m_vtcText.ReferBuffer();
	}

	/**
	 * [�擾] UTF8������Q�Ǝ擾 
	 *	@return UTF8������̐擪�|�C���^
	 */
	operator LPCSTR (void) const
	{
		return ReferBuffer();
	}

	/**
	 * [����] �������݃o�b�t�@�v��
	 *	@note UTF8�^���������������ł��������B
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
		CUtf8Str s;
		s.SetOfUtf8(m_vtcText.ReferBuffer());
		m_vtcText = s.m_vtcText;
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
