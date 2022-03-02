#pragma once
/**
 *@file
 * MD5(message-digest algorithm)�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	namespace MD5
	{
		//RSA Data Security �̃\�[�X
		#include "other/MD5.h"
	};
#endif



/**@ingroup CIPHER
 * MD5(message-digest algorithm)�v�Z�N���X.
 *
 *	�w����� 128BIT�̃n�b�V���R�[�h���v�Z���܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		CStr s = CMd5Computer::ComputeHash("abc").GetString();
 *		ASSERT( s == "900150983cd24fb0d6963f7d28e17f72" );
 *		</pre></td></tr></table>
 *
 *	@see http://www.faqs.org/rfcs/rfc1321.html
 *
 *	@par�K�v�t�@�C�� 
 *			TnbMd5Computer.h
 *
 *	@date 06/09/26 �V�K�쐬
 *	@date 06/09/27 Update��2�ȏ�s�Ȃ��\�������邱�Ƃ��l���B
 */
class CMd5Computer : CCopyImpossible
{
	MD5::MD5_CTX	m_context;
public:

	/**@ingroup CIPHER
	 * Md5�����N���X�̌��ʌ^.
	 *	@see CMd5Computer::Compute()
	 */
	struct TResult
	{
		/// �n�b�V�����
		BYTE hash[16];
		/// �n�b�V�����(������)�擾
		CStr GetString(void)
		{
			CStr s;
			loop ( i, 16 )
			{
				s += CStr::Fmt(_T("%02x"), hash[i]);
			}
			return s;
		}
	};

	/// �R���X�g���N�^
	CMd5Computer(void)
	{
		MD5::MD5Init(&m_context);
	}

	/**
	 * [�ݒ�] ���[�h�}�W�b�N�l�ݒ�
	 *	@param a �}�W�b�N A
	 *	@param b �}�W�b�N B
	 *	@param c �}�W�b�N C
	 *	@param d �}�W�b�N D
	 */
	void SetLoadMagic(DWORD a, DWORD b, DWORD c, DWORD d)
	{
		m_context.state[0] = a;
		m_context.state[1] = b;
		m_context.state[2] = c;
		m_context.state[3] = d;
	}

	/**
	 * [�ݒ�] �Ώۃf�[�^�ǉ�
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 */
	void Update(size_t size, LPCVOID P)
	{
		MD5::MD5Update(&m_context, static_cast<const BYTE*>(P), size);
	}

	/**
	 * [�ݒ�] �Ώۃf�[�^�ǉ�
	 *	@param lpszText ASCII������
	 */
	void Update(LPCSTR lpszText)
	{
		Update(STRLIB::GetLen(lpszText), lpszText);
	}

	/**
	 * [�v�Z] �n�b�V���擾
	 *	@note Update() �Œǉ������f�[�^�̃n�b�V�����v�Z���ĕԂ��܂��B
	 *	@return ����
	 */
	TResult GetHash(void)
	{
		TResult t;
		MD5::MD5Final(t.hash, &m_context);
		MD5::MD5Init(&m_context);
		return t;
	}

	/**
	 * [�v�Z] �n�b�V���v�Z
	 *	@param size �f�[�^�T�C�Y
	 *	@param P �f�[�^
	 *	@return ����
	 */
	static TResult ComputeHash(size_t size, LPCVOID P)
	{
		TResult t;
		MD5::MD5_CTX context;
		MD5::MD5Init(&context);
		MD5::MD5Update(&context, static_cast<const BYTE*>(P), size);
		MD5::MD5Final(t.hash, &context);
		return t;
	}

	/**
	 * [�v�Z] �n�b�V���v�Z
	 *	@param lpszText ASCII������
	 *	@return ����
	 */
	static TResult ComputeHash(LPCSTR lpszText)
	{
		return ComputeHash(STRLIB::GetLen(lpszText), lpszText);
	}
#if 0
	/**
	 *	[�v�Z] HMAC_MD5�v�Z.
	 *		MD5�̃_�C�W�F�X�g�𐶐�����
	 *	@param sizeInput Input�f�[�^�T�C�Y
	 *	@param pInput Input�f�[�^
	 *	@param sizeKey Key�f�[�^�T�C�Y
	 *	@param pKey Key�f�[�^
	 *	@return ����
	 */
	static TResult ComputeHmacMd5(size_t sizeInput, LPCVOID pInput, size_t sizeKey, LPCVOID pKey)
	{
		TResult t;
		BYTE ipad[64];
		BYTE opad[64];
		if ( sizeKey>64 ) 
		{
			// �L�[��64�o�C�g���傫���ꍇ�̓L�[�̃_�C�W�F�X�g���L�[�ɂ���
			t		= ComputeHash(sizeKey, pKey);
			pKey	= t.hash;
			sizeKey = 16;
		}
		// MD5(key XOR opad, MD5(key XOR ipad, input)) �̌v�Z
		::FillMemory(ipad, sizeof(ipad), 0x36);
		::FillMemory(opad, sizeof(opad), 0x5c);
		const BYTE* B = static_cast<const BYTE*>(pKey);
		for ( size_t i = 0; i < sizeKey; i++ )
		{
			ipad[i] ^= B[i];
			opad[i] ^= B[i];
		}
		CMd5Computer m;
		m.Update(64, ipad);
		m.Update(sizeInput, pInput);
		t = m.GetHash();
		m.Update(64, opad);
		m.Update(16, t.hash);
		return m.GetHash();
	}

	/**
	 *	[�v�Z] HMAC_MD5�v�Z.
	 *		MD5�̃_�C�W�F�X�g�𐶐�����
	 *	@param lpszInput Input�f�[�^
	 *	@param lpszKey Key�f�[�^
	 *	@return ����
	 */
	static TResult ComputeHmacMd5(LPCSTR lpszInput, LPCSTR lpszKey)
	{
		return ComputeHmacMd5(STRLIB::GetLen(lpszInput), lpszInput, STRLIB::GetLen(lpszKey), lpszKey);
	}
#endif
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
