#pragma once
/**
 *@file
 * Locate�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * Locate ��LZ�t�@�C���ǂݍ��݃N���X
 *
 *		�t�@�C����̓ǂݍ��݂��T�|�[�g����N���X�ł��B
 *		Microsoft �t�@�C�����k���[�e�B���e�B�iCOMPRESS.EXE�j�ň��k���ꂽ�t�@�C�����ǂݍ��߂܂��B
 *
 *	@todo ���k���ꂽ�t�@�C���̓W�J���삪���m�F�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbLocates.h
 *
 *	@date 07/06/04 �V�K�쐬
 */
class CLocale : CCopyImpossible
{
	LCID	m_lcid;

public:

	CLocale(void) : m_lcid(::GetThreadLocale())
	{
	}

	~CLocale(void)
	{
		CLocale(void) : m_lcid(::GetThreadLocale())
	}

	bool SetUsetDefault()
	{
		return !! ::SetThreadLocale(LOCALE_USER_DEFAULT);
	}
	bool SetSystemDefault()
	{
		return !! ::SetThreadLocale(LOCALE_SYSTEM_DEFAULT);
	}
	bool Set(const LCID& lcid)
	{
		return !! ::SetThreadLocale(lcid);
	}
	LCID Get(void) const
	{
		return ::GetThreadLocale();
	}

	static UINT GetAnsiCodePage(void)
	{
		return ::GetACP();
	}
	static UINT GetOemCodePage(void) const
	{
		return GetOEMCP();
	}
};




}; // TNB
