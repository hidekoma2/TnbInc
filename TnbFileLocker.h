#pragma once
/**
 *@file
 * �t�@�C�����b�N�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbHandleHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �t�@�C�����b�N�Ǘ��N���X
 *
 *	�{�N���X�́A�C���X�^���X��ɕt��������A�t�@�C����Lock���邱�Ƃ��o���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileLocker.h
 *
 *	@date 08/03/26 �V�K�쐬
 */
class CFileLocker : CCopyImpossible
{
	CHandleHandle	m_hhFile;		///< �t�@�C���n���h��
public:

	/// �R���X�g���N�^
	CFileLocker(void) :	m_hhFile(INVALID_HANDLE_VALUE)
	{
	}

	/// �f�X�g���N�^
	~CFileLocker(void)
	{
		Unlock();
	}

	/**
	 * [�m�F] ���b�N���
	 *	@retval true ���b�N��
	 *	@retval false ���b�N���łȂ�
	 */
	bool IsLock(void) const
	{
		return ! m_hhFile.IsNull();
	}

	/** 
	 * [�r��] ���b�N
	 *	@param lpszName ���b�N����t�@�C�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Lock(LPCTSTR lpszName)
	{
		Unlock();
		DWORD dwDesiredAccess = GENERIC_READ;
		DWORD dwShareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
		DWORD dwCreationDisp = OPEN_EXISTING;
		m_hhFile = ::CreateFile(lpszName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisp, 0, NULL);
		_GetLastError("CreateFile");
		if ( ! m_hhFile.IsNull() && ::LockFile(m_hhFile, 0, 0, DWORD_MAX, DWORD_MAX) )
		{
			return true;
		}
		m_hhFile.Null();
		return false;
	}

	/**
	 * [�r��] �A�����b�N
	 *	@see Lock
	 */
	void Unlock(void)
	{
		if ( ! m_hhFile.IsNull() )
		{
			::UnlockFile(m_hhFile, 0, 0, DWORD_MAX, DWORD_MAX);
			m_hhFile.Null();
		}
	}
};



}; // TNB
