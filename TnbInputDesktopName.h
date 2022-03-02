#pragma once
/**
 *@file
 * ����Desktop���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * ����Desktop���N���X
 *
 *		����Desktop�̖��O�������N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbInputDesktopName.h
 *
 *	@date 08/04/17 �V�K�쐬
 */
class CInputDesktopName
{
public:
	/// �R���X�g���N�^
	CInputDesktopName(void)
	{
		TCHAR name[100] = {0};
		DWORD w;
		HDESK h = ::GetThreadDesktop(::GetCurrentThreadId());
		::GetUserObjectInformation(h, UOI_NAME, name, 100, &w);
		m_currentDesktopName = name;
	}

	/**
	 * [�擾] �J�����gDesktop���擾
	 *	@return �J�����gDesktop���O�B
	 */
	LPCTSTR GetCurrentDesktopName(void) const
	{
		return m_currentDesktopName;
	}

	/**
	 * [�擾] ���݂̓���Desktop���擾
	 *	@param[out] _str ���݂̓���Desktop���O���i�[����܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetNowInputDesktopName(CSimpleStr& _str) const
	{
		TCHAR name[100] = {0};
		DWORD w;
		HDESK h = ::OpenInputDesktop(0, FALSE, DESKTOP_ENUMERATE);
		BOOL r = ::GetUserObjectInformation(h, UOI_NAME, name, 100, &w);
		::CloseDesktop(h);
		_str = name;
		return !! r;
	}

	/**
	 * [�m�F] �J�����g�����݂̓���Desktop�ɂȂ��Ă��邩
	 *	@retval true �Ȃ��Ă���
	 *	@retval false �Ȃ��Ă��Ȃ�
 	 */
	bool IsEqualThreadDesktop(void) const
	{
		CSimpleStr str;
		if ( GetNowInputDesktopName(str) )
		{
			return _tcscmp(str, m_currentDesktopName) == 0;
		}
		return false;
	}

private:
	CSimpleStr m_currentDesktopName;
};



}; // TNB

