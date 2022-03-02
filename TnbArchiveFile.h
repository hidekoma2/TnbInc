#pragma once
/**
 *@file
 * �A�[�J�C�u�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbArchiver.h"
#include "TnbTemporaryDirectory.h"
#include "TnbFile.h"
#include "TnbArchiveFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE ARCHIVEFILE
 * �A�[�J�C�u�t�@�C���ǂݍ��݃N���X
 *
 *		�A�[�J�C�u�t�@�C�����̃t�@�C����̓ǂݍ��݂��T�|�[�g����N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiveFile.h
 *
 *	@date 06/12/19 �V�K�쐬
 *	@date 08/03/27 �t�H���_���쐬����悤�ɂ���
 *	@date 10/10/04 �W�J���A�T�u�t�H���_���܂�œW�J����悤��
 *					�ύX�i�Ή����Ă��Ȃ��A�[�J�C�o�����邽�߁j
 */
class CArchiveFileReader : public IReader
{
	DEFSUPER(IReader);
public:

	/// �R���X�g���N�^
	CArchiveFileReader(void)
	{
	}

	/// �f�X�g���N�^
	virtual ~CArchiveFileReader(void)
	{
		m_Close();
	}

	/**
	 * [�ݒ�] �I�[�v��
	 *	@note ���Ɉ�J���Ă�����N���[�Y���Ă���w��̃t�@�C�����J���܂��B
	 *	@param arcdll �L���ȃA�[�J�C�oDLL
	 *	@param lpszArcFile �A�[�J�C�u�t�@�C��
	 *	@param lpszName �A�[�J�C�u�t�@�C�����̃t�@�C���B
	 *	@param lpszOpt �A�[�J�C�u���o�����̃I�v�V�����B
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool Open(CArchiverDllHandler& arcdll, LPCTSTR lpszArcFile, LPCTSTR lpszName, LPCTSTR lpszOpt = NULL)
	{
		Close();
		if ( ! arcdll.IsReady() || arcdll.IsRunning() )
		{
			return false;
		}
		CArchiver arc(&arcdll);
		arc.SetTargetArchiveFile(lpszArcFile);
		CStr strPath = m_tempDir.Create();
		arc.SetWorkDirectory(strPath);
		bool r = (arc.ExtractWithDirectory(lpszName, lpszOpt) == 0);
		if ( r )
		{
			CStr ss = strPath + _T("\\") + lpszName;
			ss.Replace(_T("\\\\"), _T("\\"));
			r = m_reader.Open(ss);
		}
		if ( ! r )
		{
			m_tempDir.Free();
		}
		return r;
	}

	/**
	 * [�ݒ�] �I�[�v��
	 *	@param ff ��������
	 *	@param lpszOpt �A�[�J�C�u���o�����̃I�v�V�����B
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool Open(const CArchiveFileFinder& ff, LPCTSTR lpszOpt = NULL)
	{
		const TIndividualInfo* P = ff.RefPtr();
		if ( ff.IsFinding() && P != NULL )
		{
			#ifdef _UNICODE
				return Open(*ff.GetDllHandler(), CStr(ff.GetTargetArchiveFileName()), CStr(P->szFileName), lpszOpt);
			#else
				return Open(*ff.GetDllHandler(), ff.GetTargetArchiveFileName(), P->szFileName, lpszOpt);
			#endif
		}
		return false;
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@note �����閼�O�́A�e���|�����t�@�C�����ɂȂ�܂��B
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_reader.GetOpenName();
	}

	/**
	 * [����] �N���[�Y
	 *	@note �g�p���Ă����e���|�����t�@�C�����폜�B
	 */
	virtual void Close(void)
	{
		m_Close();
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return m_reader.CanRead();
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		return m_reader.GetSize();
	}

	/**
	 * [����] �V�[�N.
	 *		�A�N�Z�X�|�C���^���ړ����܂��B
	 *	@param llOffset �A�N�Z�X�|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�A�N�Z�X�|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return m_reader.Seek(llOffset, eSeek);
	}

	 /**
	 * [�擾] �ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		return m_reader.Read(size, _P);
	}

private:
	///�N���[�Y
	void m_Close(void)
	{
		m_reader.Close();
		m_tempDir.Free();
	}
	CFileReader					m_reader;	///< �t�@�C���ǂݍ���
	CTemporaryDirectoryT<'CRA'>	m_tempDir;	///< �e���|����

	friend class CArchiveFileReaderTest;
};



}; // TNB
