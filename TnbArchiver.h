#pragma once
/**
 *@file
 * �A�[�J�C�o�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbArchiverDllHandler.h"
#include "TnbStrVector.h"
#include "TnbTemporaryFile.h"



//TNB Library
namespace TNB
{



/**@ingroup ARCHIVEFILE
 * �A�[�J�C�o�N���X
 *
 *		���k/�W�J���T�|�[�g����N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiver.h
 *
 *	@date 06/12/14 �V�K�쐬
 *	@date 09/07/16 ��ƃf�B���N�g���������ݒ肷��悤�ɂ����B
 */
class CArchiver
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param pDll �A�[�J�C�oDLL�Ǘ��N���X�̃C���X�^���X�B
	 *				�{�C���X�^���X�Ŏg�p���A�j�����Ă͂����܂���B
	 *	@throw CNullPointerException �g�p�ł��Ȃ��A�[�J�C�o�Ǘ��N���X���w�肷��ƁA�X���[����܂��B
	 */
	CArchiver(CArchiverDllHandler* pDll) : m_pDll(pDll)
	{
		ASSERT0( pDll != NULL, "CArchiver()", "�������A�[�J�C�o�Ǘ��N���X���w�肳��Ă��܂���B" );
		if ( pDll == NULL ) 
		{
			throw CNullPointerException();
		}
		ASSERT0( pDll->IsReady(), "CArchiver()", "�g�p�\�ȃA�[�J�C�o�Ǘ��N���X���w�肳��Ă��܂���B" );
		if ( ! pDll->IsReady() ) 
		{
			throw CNullPointerException(); 
		}
		::GetTempPathA(MAX_PATH, m_strWorkDir.GetBuffer(MAX_PATH));
		m_strWorkDir.ReleaseBuffer();
	}
	
	/**
	 * [�擾] �ΏۃA�[�J�C�u�擾
	 *	@note SetTargetArchiveFile() �Őݒ肵���t�@�C�������擾�ł��܂��B
	 *	@return �A�[�J�C�u��
	 */
	CStr GetTargetArchiveFile(void) const
	{
		CStr s = m_strArcFile;
		return s;
	}

	/**
	 * [�ݒ�] �ΏۃA�[�J�C�u�w��
	 *	@param lpszArcFile �ΏۃA�[�J�C�u	
	 *	@retval ture �w��t�@�C�����Ȃ����A�������A�[�J�C�u�̏ꍇ�B
	 *	@retval false �w��t�@�C�������݂��A�������Ȃ��A�[�J�C�u�̏ꍇ�B
	 */
	bool SetTargetArchiveFile(LPCTSTR lpszArcFile)
	{
		bool r = false;
		if ( ToInt(::GetFileAttributes(lpszArcFile)) == -1 )
		{
			r = (::GetLastError() == ERROR_FILE_NOT_FOUND);
		}
		else if ( m_pDll->CheckArchive(CAscii(lpszArcFile), 0) )
		{
			r = true;
		}
		if ( r )
		{
			m_strArcFile = lpszArcFile;
		}
		return r;
	}

	/**
	 * [�ݒ�] ��ƃf�B���N�g���w��
	 *	@note �ݒ肵�Ȃ��ꍇ�A�V�X�e���̃e���|�����f�B���N�g�����g�p����܂��B
	 *	@param lpszWorkDir �f�B���N�g��
	 */
	void SetWorkDirectory(LPCTSTR lpszWorkDir)
	{
		m_strWorkDir = lpszWorkDir;
	}

	/**
	 * [����] �폜.
	 *		�A�[�J�C�u�t�@�C������폜���܂��B
	 *	@param lpszName �폜���閼�O�B���C���h�J�[�h���\�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int Delete(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( m_strArcFile.IsEmpty() )
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_Delete, lpszName, lpszOption);
	}

	/**
	 * [����] �폜.
	 *		�A�[�J�C�u�t�@�C������폜���܂��B
	 *	@param vs �폜���閼�O�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int Delete(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( m_strArcFile.IsEmpty() ) 
		{
			return -1; 
		}
		return m_ExecList(m_pDll->EC_Delete, vs, lpszOption);
	}

	/**
	 * [����] �擾.
	 *		�A�[�J�C�u�t�@�C������t�@�C����W�J���܂��B
	 *	@param lpszName �W�J���閼�O�B���C���h�J�[�h���\�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int Extract(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() )
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_Extract, lpszName, lpszOption);
	}

	/**
	 * [����] �W�J.
	 *		�A�[�J�C�u�t�@�C������t�@�C����W�J���܂��B
	 *	@param vs �폜���閼�O�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int Extract(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() ) 
		{
			return -1; 
		}
		return m_ExecList(m_pDll->EC_Extract, vs, lpszOption);
	}

	/**
	 * [����] �W�J
	 *	@param[in] lpszName �Ώۃt�@�C���B
	 *	@param[in,out] _vb �ǂݏo���T�C�Y��ݒ肵�ċN���܂��B ���s��͒��o�����T�C�Y�ɂȂ��Ă��܂��B
	 *	@param[in] lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����I��
	 *	@retval 0�ȊO �G���[����
	 */
	int ExtractMem(LPCTSTR lpszName, CByteVector& _vb, LPCTSTR lpszOption = NULL)
	{
		int r = -1;
		if ( ! m_strArcFile.IsEmpty() ) 
		{
			size_t dwSize = _vb.GetSize();
			#ifdef _UNICODE
				int r = m_pDll->ExtractMem(m_strArcFile, CAscii(lpszName), _vb.GetBuffer(), dwSize, CAscii(lpszOption));
			#else
				int r = m_pDll->ExtractMem(m_strArcFile, lpszName, _vb.GetBuffer(), dwSize, lpszOption);
			#endif
			_vb.ReleaseBuffer();
			_vb.SetSize((r == 0) ? dwSize : 0);
		}
		return r;
	}

	/**
	 * [����] �W�J.
	 *		�A�[�J�C�u�t�@�C������t�@�C����W�J���܂��B
	 *	@param lpszName �W�J���閼�O�B���C���h�J�[�h���\�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int ExtractWithDirectory(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() ) 
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_ExtractDir, lpszName, lpszOption);
	}

	/**
	 * [����] �W�J.
	 *		�A�[�J�C�u�t�@�C������t�@�C����W�J���܂��B
	 *	@param vs �폜���閼�O�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int ExtractWithDirectory(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() ) 
		{
			return -1; 
		}
		return m_ExecList(m_pDll->EC_ExtractDir, vs, lpszOption);
	}

	/**
	 * [����] �i�[.
	 *		�A�[�J�C�u�t�@�C���փt�@�C�����i�[���܂��B
	 *	@param lpszName �i�[���閼�O�B���C���h�J�[�h���\�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int Attract(LPCTSTR lpszName, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() )
		{
			return -1; 
		}
		return m_Exec(m_pDll->EC_Attract, lpszName, lpszOption);
	}

	/**
	 * [����] �i�[.
	 *		�A�[�J�C�u�t�@�C���փt�@�C�����i�[���܂��B
	 *	@param vs �폜���閼�O�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s�B�G���[�R�[�h�B
	 */
	int Attract(const CStrVector& vs, LPCTSTR lpszOption = NULL)
	{
		if ( ! m_IsReady() )
		{
			return -1;
		}
		return m_ExecList(m_pDll->EC_Attract, vs, lpszOption);
	}

	/**
	 * [����] ����������i�[
	 *	@param[in] lpszName �i�[�t�@�C�����B
	 *	@param[in] time �i�[���ԁB
	 *	@param[in] wAttr �i�[�����B
	 *	@param[in] vb �i�[���e�B
	 *	@param[in] lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����I��
	 *	@retval 0�ȊO �G���[����
	 */
	int AttractMem(LPCTSTR lpszName, time_t time, WORD wAttr, const CByteVector& vb, LPCTSTR lpszOption = NULL)
	{
		int r = -1;
		if ( ! m_strArcFile.IsEmpty() )
		{
			DWORD dwSize = ToDword(vb.GetSize());
			#ifdef _UNICODE
				r = m_pDll->CompressMem(m_strArcFile, CAscii(lpszName), time, wAttr, vb.ReferBuffer(), dwSize, CAscii(lpszOption));
			#else
				r = m_pDll->CompressMem(m_strArcFile, lpszName, time, wAttr, vb.ReferBuffer(), dwSize, lpszOption);
			#endif
		}
		return r;
	}
private:
	CArchiverDllHandler*	m_pDll;			///< �A�[�J�C�oDLL�Ǘ�
	CAscii					m_strArcFile;	///< �A�[�J�C�u�t�@�C����
	CAscii					m_strWorkDir;	///< ��ƃf�B���N�g��

	/// �ݒ�m�F
	bool m_IsReady(void) const
	{
		return (! m_strArcFile.IsEmpty()) && (! m_strWorkDir.IsEmpty());
	}

	/// ���s
	int m_Exec(CArchiverDllHandler::ECommand cmd, LPCTSTR lpszName, LPCTSTR lpszOption)
	{
		#ifdef _UNICODE
			return m_pDll->Execute(cmd, m_strArcFile, m_strWorkDir, CAscii(lpszName), CAscii(lpszOption));
		#else
			return m_pDll->Execute(cmd, m_strArcFile, m_strWorkDir, lpszName, lpszOption);
		#endif
	}

	/// ���X�|���X�t�@�C�����s
	int m_ExecList(CArchiverDllHandler::ECommand cmd, const CStrVector& vs, LPCTSTR lpszOption)
	{
		CAscii strContent;
		if ( lpszOption != NULL )
		{
			strContent += lpszOption;
			strContent += "\r\n";
		}
		loop ( i, vs.GetSize() )
		{
			strContent += CAscii(vs[i]);
			strContent += "\r\n";
		}
		CTemporaryFile tmp;
		CStr strName = tmp.Create(strContent.GetLength(), strContent.begin());
		int r = m_Exec(cmd, "@" + strName, NULL);
		tmp.Free();
		return r;
	}
};



}; // TNB
