#pragma once
/**
 *@file
 * ���J�o���[�t�@�C���Y�֌W�̃w�b�_
 *
 *		�󋵂ɉ����āA�t�@�C�������̏�Ԃɖ߂����Ƃ��o����N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFileName.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ���J�o���[�t�@�C���Y�N���X
 *
 *		�󋵂ɉ����āA�t�@�C�������̏�Ԃɖ߂����Ƃ��o����N���X������܂��B
 *
 *		
 *
 *	@par�K�v�t�@�C��
 *			TnbRecoveryFiles.h
 *
 *	@date 09/03/03	�V�K�쐬
 */
class CRecoveryFiles
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param isDefaultRecoveryMode �f�X�g���N�^���̓���B true;���J�o���[ / �ȗ� or false;�A�����W�B
	 */
	CRecoveryFiles(bool isDefaultRecoveryMode = false) : m_isDefaultRecoveryMode(isDefaultRecoveryMode)
	{
	}

	/// �f�X�g���N�^
	~CRecoveryFiles(void)
	{
		if ( m_isDefaultRecoveryMode )
		{
			RecoverAll();
		}
		else
		{
			UnregisterAll();
		}
	}

	/**
	 * [�ݒ�] �o�^.
	 *		�w��t�@�C���𕡐����A�t�@�C�������L�����܂��B
	 *	@note �����o�^�\�ł��B
	 *	@param lpszFile �t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Register(LPCTSTR lpszFile)
	{
		CFileName fn(lpszFile);
		if ( fn.IsExist() )
		{
			CStr bak;
			loop ( i, 0x10000 )
			{
				bak = fn.GetFullName() + CStr::Fmt(_T(".%04X.bak"), i);
				if ( ! m_Exist(bak) )
				{
					break;
				}
			}
			if ( fn.CopyTo(bak) )
			{
				TParam p(fn.GetFullName(), bak);
				p.backHandle = ::CreateFile(bak, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
				m_names.Add(p);
				return true;
			}
		}
		return false;
	}

	/**
	 * [�ݒ�] �o�^����.
	 *	@note �o�^�����������o�b�N�A�b�v�t�@�C�����폜���A�N���X�̊Ǘ�������O���܂��B
	 *	@retval true �o�b�N�A�b�v�t�@�C���̑|���ɐ����B
	 *	@retval false �����������Ȃ��o�b�N�A�b�v�t�@�C�����������B
	 */
	bool UnregisterAll(void)
	{
		bool r = true;
		loop ( i, m_names.GetSize() )
		{
			m_names[i].backHandle.Null();
			r &= !! ::DeleteFile(m_names[i].backName);
		}
		m_names.RemoveAll();
		return r;
	}

	/**
	 * [�ݒ�] ���J�o�[.
	 *	@note �o�^�������̏�ԂɃt�@�C����߂��܂��B
	 *	@note �N���X�̊Ǘ�������O���܂��B
	 *	@retval true �S�����ɖ߂���
	 *	@retval false �������߂��Ȃ����̂��������i�I���W�i���� bak �t�@�C���Ƃ��Ďc���Ă���j
	 */
	bool RecoverAll(void)
	{
		bool r = true;
		//== ���tmp�t�@�C���쐬
		loop ( i, m_names.GetSize() )
		{
			CStr reg = m_names[i].registName;
			if ( m_Exist(reg) )
			{
				CStr tmp;
				loop ( ii, 0x10000 )
				{
					tmp = reg + CStr::Fmt(_T(".%04X.tmp"), ii);
					if ( ! m_Exist(tmp) )
					{
						break;
					}
				}
				if ( ! ::MoveFile(reg, tmp) )
				{
					_GetLastError("MoveFile");
					m_names[i].tempName.Empty();
					r = false;
				}
				else
				{
					m_names[i].tempName = tmp;
				}
			}
		}
		//== tmp�t�@�C�����o�������̂�bak���I���W�i���Ƀ��l�[�����Atmp���폜�B
		loop ( i, m_names.GetSize() )
		{
			m_names[i].backHandle.Null();
			if ( ::MoveFile(m_names[i].backName, m_names[i].registName) )
			{
				if ( ! m_names[i].tempName.IsEmpty() )
				{
					VERIFY( ::DeleteFile(m_names[i].tempName) );
				}
			}
			else
			{
				// �Ӑ}�I�� bak �t�@�C�����������ꍇ�Ȃ�.
				r = false;
			}
		}
		m_names.RemoveAll();
		return r;
	}

private:
	/// �t�@�C���͑��݁H
	bool m_Exist(LPCTSTR lpszName) const
	{
		WIN32_FIND_DATA t;
		HANDLE h = ::FindFirstFile(lpszName, &t);
		if ( h != INVALID_HANDLE_VALUE )
		{
			::FindClose(h);
			return true;
		}
		return false;
	}
	/// �p�����[�^
	struct TParam
	{
		CStr			registName;	///< �o�^��
		CStr			backName;	///< �o�b�N�A�b�v��
		CHandleHandle	backHandle;	///< �o�b�N�A�b�v�t�@�C���̃n���h��
		CStr			tempName;	///< �e���|������
		TParam(LPCTSTR reg = NULL, LPCTSTR tmp = NULL) : registName(reg), backName(tmp)
		{
		}
	};
	CVectorT<TParam>	m_names;					///< ���O�Q
	bool				m_isDefaultRecoveryMode;	///< �f�X�g���N�^���̓���B true;���J�o���[ false;�A�����W
	friend class CRecoveryFilesTest;
};



}; // TNB

