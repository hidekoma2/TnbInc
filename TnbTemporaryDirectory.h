#pragma once
/**
 *@file
 * �e���|�����f�B���N�g���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTemporaryFile.h"
#include "TnbFileName.h"


#ifndef _WIN32_WCE
 #include <SHLWAPI.h>
 #pragma comment(lib, "SHLWAPI.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �e���|�����f�B���N�g���Ǘ��e���v���[�g�N���X
 *
 *		�ꎞ�I�ɃN���A�ȃf�B���N�g�����쐬���܂��B
 *		�s�K�v���i�f�X�g���N�^����AFree() ���j�ɁA�S�t�@�C�����폜���܂��B
 *
 *	@note ��C���X�^���X�ɕt���A�Ǘ�����f�B���N�g���͈�����ł��B
 *
 *	@param PFIX �e���|�����t�@�C���̃v���t�B�b�N�X�B�ʏ�ꕶ����ASCII�R�[�h���w�肵�܂��B
 *				�ő�S�����܂ŋL�q�\�ł����B���Ԃ��t�ɂȂ邱�Ƃɒ��ӁB\n
 *				ex) CTemporaryDirectoryT<'ABCD'> �Ȃ�A'�`��DCBA0000.tmp��' �Ƃ����f�B���N�g�����ɂȂ�܂��B 
 *
 *	@par�K�v�t�@�C��
 *			TnbTemporaryDirectory.h
 *
 *	@date 06/12/13 �V�K�쐬
 */
template<DWORD PFIX>
class CTemporaryDirectoryT : CCopyImpossible
{
	CStr m_strDir;		///< �Ǘ�����f�B���N�g����

	/// ���ʂ̃t�H���_���ꏏ�ɍ폜
	bool m_Remove(LPCTSTR lpszDir)
	{
		if ( ::RemoveDirectory(lpszDir) )
		{
			return true;
		}
		CFileName fn(lpszDir);
		return fn.Remove(false, true);
	}

public:

	/// �R���X�g���N�^
	CTemporaryDirectoryT(void)
	{
	}

	/// �f�X�g���N�^
	~CTemporaryDirectoryT(void)
	{
		if ( ! m_strDir.IsEmpty() )
		{
			m_Remove(m_strDir);
			m_strDir.Empty();
		}
	}

	/**
	 * [�擾] �e���|�����f�B���N�g�����擾
	 *	@return �t���p�X�f�B���N�g�����B empty��ԂȂ�쐬���Ă��Ȃ���ԁB
	 */
	CStr GetName(void) const
	{
		return m_strDir;
	}

	/**
	 * [�m�F] �e���|�����f�B���N�g���L���m�F
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool HasThat(void) const
	{
		return ! m_strDir.IsEmpty();
	}

	/**
	 * [����] ���.
	 *		�쐬�����e���|�����f�B���N�g�����폜���܂��B
	 *		�쐬���Ă��Ȃ���Ή������܂���B
	 *	@throw CDeleteFailureException �e���|�����t�@�C�����g���Ă���ƃX���[����܂��B
	 */
	void Free(void)
	{
		if ( ! m_strDir.IsEmpty() )
		{
			if ( ! m_Remove(m_strDir) )
			{
				throw CDeleteFailureException(::GetLastError());
			}
			m_strDir.Empty();
		}
	}

	/**
	 * [����] ���.
	 *		�e���|�����f�B���N�g���̊Ǘ����I�����܂��B
	 *		�쐬�����e���|�����f�B���N�g���͍폜���܂���B
	 */
	void Detach(void)
	{
		m_strDir.Empty();
	}

	/**
	 * [����] �쐬.
	 *		�e���|�����f�B���N�g�����Ƃ��̃f�B���N�g�����쐬���܂��B
	 *	@note �쐬�����e���|�����f�B���N�g��������ΐ�ɉ�����܂��B
	 *	@return �e���|�����f�B���N�g����
	 *	@throw CIoFailureException �t�@�C�����쎞�ɃG���[����������ƁA�X���[����܂��B
	 */
	CStr Create(void)
	{
		Free();
		CTemporaryFileT<PFIX> temp;
		CStr s = temp.Create();
		temp.Free();
		if ( ! ::CreateDirectory(s, NULL) )
		{
			s.Empty();
		}
		else
		{
			m_strDir = s;
			s += _T("\\");
		}
		return s;
	}
};



/**@ingroup FILE
 * �e���|�����f�B���N�g���Ǘ��e���v���[�g
 *
 *	@note �v���t�B�b�N�X�� tnb �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTemporaryDirectory.h
 *
 *	@date 07/01/16 �V�K�쐬
 */
typedef CTemporaryDirectoryT<'bnt'> CTemporaryDirectory;



}; // TNB
