#pragma once
/**
 *@file
 * ���I�֐��Ǘ��֌W�̃w�b�_
 *
 *		DLL���̊֐��𓮓I�ɌĂяo�����s���邱�Ƃ��ł���N���X���L��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup ETC
 * ���I�֐��Ǘ��N���X�e���v���[�g
 *
 *		DLL���̊֐��𓮓I�ɌĂяo�����s���邱�Ƃ��e�Ղɂł��܂��B
 *
 *	@param FUNC �֐��^�B����ȊO�͕ۏ؊O�B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *
 *		// �֐��̌^��錾
 *		typedef LONG (WINAPI* P_InterlockedCompareExchange)(LONG*, LONG, LONG);
 *		// ���I�Ǘ��N���X�錾���֐��ǂݍ���
 *		CDynamicFuncT<P_InterlockedCompareExchange> func1("kernel32.dll", "InterlockedCompareExchange");		
 *		// ���s
 *		LONG l1 = 0;
 *		LONG l2 = func1()(&l1, 1, 0); //�󊇌ʂ���͂��ނ̂��|�C���g		
 *	
 *		// �錾�ƌĂяo������s�ōs�Ȃ����Ƃ��o���܂��B
 *		typedef HANDLE (WINAPI* P_IcmpCreateFile)(VOID);
 *		HANDLE h = CDynamicFuncT<P_IcmpCreateFile>("ICMP.dll", "IcmpCreateFile")()();
 *		</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbDynamicFunc.h
 *
 *	@date 06/09/21 �V�K�쐬
 *	@date 08/01/29 LoadFunc �ǉ�
 */
template<typename FUNC>
class CDynamicFuncT : CCopyImpossible
{
	HMODULE	m_hModule;			///< ���W���[���n���h��
	FUNC	m_pFunc;			///< �֐��A�h���X
	void m_Unload(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
		m_pFunc	= NULL;
	}
public:

	/**
	 * �R���X�g���N�^
	 *	@note LoadFunc() ���g���A�g�p����֐���ݒ肷��K�v������܂��B
	 */
	CDynamicFuncT(void) : m_hModule(NULL), m_pFunc(NULL)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param lpszDllName �R�[���������֐��̊܂܂��Dll���B
	 *	@param lpszFuncName �R�[���������֐����B
	 *	@throw CNullPointerException �w��̊֐���������Ȃ����ɃX���[����܂��B
	 */
	CDynamicFuncT(LPCSTR lpszDllName, LPCSTR lpszFuncName)
		: m_hModule(NULL), m_pFunc(NULL)
	{
		if ( ! LoadFunc(lpszDllName, lpszFuncName) )
		{
			throw CNullPointerException();
		}
	}

	/**
	 * [�ݒ�] �֐��Ǎ���.
	 *	@param lpszDllName �R�[���������֐��̊܂܂��Dll���B
	 *	@param lpszFuncName �R�[���������֐����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool LoadFunc(LPCSTR lpszDllName, LPCSTR lpszFuncName)
	{
		m_Unload();
		m_hModule = ::LoadLibraryA(lpszDllName);
		if ( m_hModule != NULL )
		{
			m_pFunc = reinterpret_cast<FUNC>(::GetProcAddressA(m_hModule, lpszFuncName));
		}
		return (m_pFunc != NULL);
	}

	/**
	 * �f�X�g���N�^
	 */
	~CDynamicFuncT(void)
	{
		m_Unload();
	}

	/**
	 * [�m�F] �֐��A�h���X�������Ă��邩
	 *	@retval true �����Ă���
	 *	@retval flase �����Ă��Ȃ��B
	 */
	bool HasFunc(void) const
	{
		return (m_pFunc != NULL);
	}

	/**
	 * [�擾] �֐��A�h���X�擾.
	 *	@note HasFunc() �̖߂�l�� true �̎��A�������܂��B
	 *	@return �֐��A�h���X
	 *	@throw CNullPointerException �w��̊֐���������Ȃ����ɃX���[����܂��B
	 */
	FUNC operator()(void) const
	{
		if ( m_pFunc == NULL )
		{
			throw CNullPointerException();
		}
		return m_pFunc;
	}
};



/**@ingroup ETC
 * ���I�֐��A�h���X�擾�֐�
 *
 *	@param[out] _pFnc FUNC�֐��^�̎��́i�|�C���^�j�B
 *	@param[in] hInstance �C���X�^���X�n���h���B
 *	@param[in] lpszName �֐����B
 *	@retval true �擾����
 *	@retval false ���s
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *
 *		// �֐��̌^�ƃ|�C���^��錾
 *		LONG (*pGetData)(LONG);
 *		// ���I�Ǘ��N���X�錾���֐��ǂݍ���
 *		GetFuncAddress(pGetData, hModule, "GetData");		
 *		// ���s
 *		LONG l = pGetData(0);		
 *		</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbDynamicFunc.h
 *
 *	@date 06/12/08 �V�K�쐬
 */
template<typename FUNC>
bool GetFuncAddress(FUNC& _pFnc, HINSTANCE hInstance, LPCSTR lpszName)
{
	_pFnc = reinterpret_cast<FUNC>(::GetProcAddressA(hInstance, lpszName));
	return _pFnc != NULL;
}



}; // TNB
