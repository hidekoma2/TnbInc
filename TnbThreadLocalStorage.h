#pragma once 
/**
 *@file
 * �X���b�h���[�J���̈�Ǘ��֌W�̃w�b�_
 *
 *		��̃C���X�^���X�����A�ʁX�̃X���b�h����A�N�Z�X���Ă�
 *		������Ȃ��X���b�h���[�J���ȗ̈���Ǘ�����N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#ifndef TLS_OUT_OF_INDEXES
 #define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)
#endif
#endif



/**@ingroup THREAD
 * �X���b�h���[�J���̈�e���v���[�g�N���X
 *	
 *		��̃C���X�^���X�����A�ʁX�̃X���b�h����A�N�Z�X���Ă�
 *		������Ȃ��X���b�h���[�J���ȕϐ����Ǘ����܂��B
 *
 *	@note �e�X���b�h�Ŏg�p����ہA�n�߂͕K���O�ɂȂ��Ă��܂��B�w��ł��܂���B
 *
 *	@note ��{�^�ȊO�́A CThreadLocalStorageT �Ȃǂ��g�p���܂��B
 *
 *	@param TYP �T�C�Y�� sizeof(LPVOID) �ȉ��̊�{�^�ϐ��B
 *
 *	@par�K�v�t�@�C��
 *			TnbThreadLocalStorage.h
 *
 *	@date 06/10/23 �V�K
 */
template<typename TYP>
class CThreadLocalStorageT
{
	DWORD			m_dwTlsIndex;	///< TLS �C���f�b�N�X

public:

	/// �R���X�g���N�^
	CThreadLocalStorageT(void)
	{
		ASSERT1( sizeof(LPVOID) >= sizeof(TYP),
				"CThreadLocalStorageT", "�g�p�o���Ȃ��^(%s)���w�肳��Ă��܂�", typeid(TYP).name() );
		m_dwTlsIndex = ::TlsAlloc();
		if ( m_dwTlsIndex == TLS_OUT_OF_INDEXES )
		{
			throw CEmptyException();
		}
	}

	/// �f�X�g���N�^
	~CThreadLocalStorageT(void)
	{
		::TlsFree(m_dwTlsIndex);
	}

	/**
	 * [�擾] �l�擾
	 *	@note Thread���ɊǗ����قȂ�܂��B�O���A�����X���b�h�� Set() �����l�����܂��B
	 *	@return �l�B
	 */
	TYP Get(void) const
	{
		return reinterpret_cast<TYP>(::TlsGetValue(m_dwTlsIndex));
	}

	/**
	 * [�ݒ�] �l�ݒ�
	 *	@note Thread���ɊǗ����قȂ�܂��B�ݒ肵�Ȃ��ƁA�l�͂O�ɂȂ��Ă��܂��B
	 *	@param t �l�B
	 */
	void Set(const TYP& t)
	{
		::TlsSetValue(m_dwTlsIndex, reinterpret_cast<LPVOID>(t));
	}

	/**
	 * [�擾] TLS�C���f�b�N�X�擾
	 *	@note �ʏ�A TLS�C���f�b�N�X���ӎ�����K�v�͂���܂���B
	 *	@return TLS�C���f�b�N�X
	 */
	DWORD GetTlsIndex(void) const
	{
		return m_dwTlsIndex;
	}
};


#if 0
class CLastErrorManage
{
	CThreadLocalStorageT<BYTE> m_tlsError;	///< �G���[�R�[�h

public:

	/// �R���X�g���N�^
	CLastErrorManage(void)
	{
	}

	DWORD GetLastError(void) const
	{
		return m_tlsError.Get();
	}

	void SetLastError(DWORD dwError=::GetLastError())
	{
		m_tlsError.Set(dwError);
	}
};
#endif



}; //TNB






