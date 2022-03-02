#pragma once
/**
 *@file
 * �A�[�J�C�u�t�@�C�������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbArchiverDllHandler.h"
#include "TnbFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE ARCHIVEFILE
 * �A�[�J�C�u���t�@�C�������N���X
 *
 *		�w��̃A�[�J�C�u�t�@�C������t�@�C�����������邽�߂̃N���X�ł��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		C7zDllHandler arc;
 *		CArchiveFileFinder ff(&arc);
 *		if ( ff.Find("c:\\$NtUninstallKB_Vol1.7z", "*.*", 0, ff.FILE_ONLY) )
 *		{
 *			TRACE1("%s\n", ff.GetFindingPath());
 *			do
 *			{
 *				TRACE1("  found = %s\n", ff->cFileName);
 *			} while ( ff.Next() );
 *		}
 *		</pre></td></tr></table>
 *		�� CFileFinder ���l�� iterator ���g�p�����������\�ł��B
 *	@note iterator ���쎞�A���� CArchiveFileFinder ���g�p����܂��B
 *
 *	@note UNICODE �v���W�F�N�g�ł� OEM�R�[�h�œ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiveFileFinder.h
 *
 *	@todo ���� FileFinder �ɔ�ׁA�t�H���_�̊T�O���Ȃ��B
 *
 *	@date 06/12/14 �V�K
 *	@date 08/03/27 ������
 */
class CArchiveFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
	CArchiverDllHandler*	m_pDll;			///< �A�[�J�C�oDLL�Ǘ�
	CAscii					m_strArcFile;	///< �����ΏۃA�[�J�C�u
	DWORD					m_dwMode;		///< ���[�h
	HARC					m_hArc;			///< �����p�n���h��
	TIndividualInfo			m_foundInfo;

	/**
	 * [�ϊ�] Arc�p�������� Win32�����֕ύX
	 *	@note �����͈ȉ��̂悤�ɕϊ�����܂��B\n
	 *		FILE_ATTRIBUTE_READONLY		<- FA_RDONLY(�ǂݍ��ݐ�p�t�@�C��)\n
	 *		FILE_ATTRIBUTE_HIDDEN		<- FA_HIDDEN(�s�������t�@�C��)\n
	 *		FILE_ATTRIBUTE_SYSTEM		<- FA_SYSTEM(�V�X�e���t�@�C��)\n
	 *		FILE_ATTRIBUTE_DIRECTORY	<- FA_DIREC(�f�B���N�g��)\n
	 *		FILE_ATTRIBUTE_ARCHIVE		<- FA_ARCH(�ۑ��r�b�g)\n
	 *		FILE_ATTRIBUTE_ENCRYPTED	<- FA_ENCRYPTED(�p�X���[�h�ی삳�ꂽ�t�@�C��)\n
	 *		FILE_ATTRIBUTE_TEMPORARY	<- FA_LABEL(�{�����[�����x��)\n
	 *		FILE_ATTRIBUTE_NORMAL		<- ��L�̂�����ł��Ȃ���
	 *	@param aa Arc�p�����l
	 *	@return Win32�����l
	 */
	static DWORD ms_ArcAttrToWin32Attr(int aa)
	{
		DWORD r = 0;
		if ( (aa & FA_RDONLY) != 0 )	{ r |= FILE_ATTRIBUTE_READONLY;		}
		if ( (aa & FA_HIDDEN) != 0 )	{ r |= FILE_ATTRIBUTE_HIDDEN;		}
		if ( (aa & FA_SYSTEM) != 0 )	{ r |= FILE_ATTRIBUTE_SYSTEM;		}
		if ( (aa & FA_LABEL) != 0 )		{ r |= FILE_ATTRIBUTE_TEMPORARY;	}
		if ( (aa & FA_DIREC) != 0 )		{ r |= FILE_ATTRIBUTE_DIRECTORY;	}
		if ( (aa & FA_ARCH) != 0 )		{ r |= FILE_ATTRIBUTE_ARCHIVE;		}
		if ( (aa & FA_ENCRYPTED) != 0 )	{ r |= FILE_ATTRIBUTE_ENCRYPTED;	}
		if ( r == 0 )					{ r |= FILE_ATTRIBUTE_NORMAL;		}
		return r;
	}

	/**
	 * [�쐬] �������쐬
	 *	@note �C���X�^���X���̏��( m_foundInfo )�ƁA �A�[�J�C�o��������擾���܂��B
	 *	@param[out] _win32Info ��񏑍��݁B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool m_MakeWin32Info(WIN32_FIND_DATA& _win32Info)
	{
		Zero(_win32Info);
		try
		{
			//-- �����ϊ�
			int arcAttr = 0;
			m_pDll->Func(arcAttr, "GetAttribute", m_hArc);
			_win32Info.dwFileAttributes = ms_ArcAttrToWin32Attr(arcAttr);
			//-- ��������
			BOOL r;
			if ( m_pDll->QueryFunctionList(ISARC_GET_WRITE_TIME_EX) )
			{
				m_pDll->Func(r, "GetWriteTimeEx", m_hArc, &(_win32Info.ftLastWriteTime));
			}
			else
			{
				//_win32Info.ftLastWriteTime
				//m_foundInfo.wDate , m_foundInfo.wTime;
			}
			_win32Info.ftCreationTime = _win32Info.ftLastWriteTime;
			_win32Info.ftLastAccessTime = _win32Info.ftLastWriteTime;
			if ( m_pDll->QueryFunctionList(ISARC_GET_CREATE_TIME_EX) )
			{
				m_pDll->Func(r, "GetCreateTimeEx", m_hArc, &(_win32Info.ftCreationTime));
			}
			if ( m_pDll->QueryFunctionList(ISARC_GET_ACCESS_TIME_EX) )
			{
				m_pDll->Func(r, "GetAccessTimeEx", m_hArc, &(_win32Info.ftLastAccessTime));
			}
			//-- �T�C�Y
			_win32Info.nFileSizeHigh = 0;
			_win32Info.nFileSizeLow = m_foundInfo.dwOriginalSize;
			//-- �\��	
			_win32Info.dwReserved0 = 0;
			_win32Info.dwReserved1 = 0;
			//-- ���O
			CStr s = m_foundInfo.szFileName;
			STRLIB::Copy(_win32Info.cFileName, s.Left(MAX_PATH));
			_win32Info.cAlternateFileName[0] = 0;
			return true;
		}
		catch(CNullPointerException& e)
		{
			e.OnCatch();
		}
		catch(...)
		{
			ASSERTLIB(false);
			throw;
		}
		return false;
	}

public:

	/// �R���X�g���N�^
	CArchiveFileFinder(void) : _super(), m_pDll(NULL), m_hArc(NULL), m_dwMode(0)
	{
	}

	/// �f�X�g���N�^
	virtual ~CArchiveFileFinder(void)
	{
		OnFinish();
	}

	/**
	 * [�ݒ�] ������
	 *	@note �C���X�^���X�쐬��A�����̑O�Ɉ�x�͂��̃��\�b�h�����s���܂��B
	 *	@param pDll �A�[�J�C�oDLL�Ǘ��N���X�̃C���X�^���X�B
	 *				�{�C���X�^���X�Ŏg�p���A�j�����Ă͂����܂���B
	 *	@param lpszArcFileName �Ώۂ̃A�[�J�C�u�t�@�C�����B
	 *	@param dwMode		�A�[�J�C�o��ɂ��B�ȗ�����ƂO�B
	 *	@retval true �����B
	 *	@retval false ���s�B�������A�[�J�C�o���w�肳��Ă��Ȃ����A�A�[�J�C�u�t�@�C����������Ȃ��B
	 */
	bool Initialize(CArchiverDllHandler* pDll, LPCTSTR lpszArcFileName, DWORD dwMode = 0)
	{
		Finish();
		if ( pDll == NULL ) 
		{
			ASSERT0( false, "CArchiveFileFinder()", "�������A�[�J�C�o�Ǘ��N���X���w�肳��Ă��܂���B" );
			return false;
		}
		if ( ! pDll->IsReady() ) 
		{
			ASSERT0( false, "CArchiveFileFinder()", "�g�p�\�ȃA�[�J�C�o�Ǘ��N���X���w�肳��Ă��܂���B" );
			return false;
		}
		int r = static_cast<int>(::GetFileAttributes(lpszArcFileName));
		if ( r < 0 )
		{
			_GetLastError("GetFileAttributes");
			return false;
		}
		m_pDll = pDll;
		m_strArcFile = lpszArcFileName;
		m_dwMode = dwMode;
		return true;
	}
	
	/**
	 * [�擾] �������Q��
	 *	@note Get() �ɔ�ׁA�C���X�^���X�ŋL�����Ă���f�[�^��Ԃ������Ȃ̂ŁA�����ł��B
	 *	@retval NULL ���s
	 *	@retval NULL�ȊO �t�@�C�����̎Q�ƁB
	 */
	const TIndividualInfo* RefPtr(void) const
	{
		if ( m_hArc != NULL )
		{
			return &m_foundInfo;
		}
		return NULL;
	}

	/**
	 * [�擾] �g�p�A�[�J�C�u�t�@�C�����A�擾
	 *	@note Find() �œn�����A�[�J�C�u�t�@�C������Ԃ������ł��B
	 *	@return �t�@�C�����B
	 */
	CAscii GetTargetArchiveFileName(void) const
	{
		return m_strArcFile;
	}

	/**
	 * [�擾] �g�p�A�[�J�C�oDLL�Ǘ��N���X�A�Q��
	 *	@note �R���X�g���N�^�Ŏw�肵���A�[�J�C�oDll�Ǘ��N���X�̃A�h���X��Ԃ������ł��B
	 *	@return �Q�ƁB
	 */
	CArchiverDllHandler* GetDllHandler(void) const
	{
		return m_pDll;
	}

protected:

	/**
	 * [�ʒm] �����J�n�ʒm
	 *	@param[out] _data ���������ꍇ�A������񂪊i�[����܂��B
	 *	@param[in] lpszName �����p�X�w��B���C���h�J�[�h�ɂ��w�肪�K�v�ł��B
	 *	@retval true ��ȏ㔭���B
	 *	@retval false �������B
	 */
	virtual bool OnStart(WIN32_FIND_DATA& _data, LPCTSTR lpszName)
	{
		if ( m_pDll != NULL )
		{
			Finish();
			m_hArc = m_pDll->OpenArchive(m_strArcFile, m_dwMode);
			if ( m_hArc != NULL )
			{
				int r;
				m_pDll->Func(r, "FindFirst", m_hArc, lpszName, &m_foundInfo);
				if ( r == 0 )
				{
					return m_MakeWin32Info(_data);
				}
			}
		}
		return false;
	}

	/**
	 * [�ʒm] �������ʒm
	 *	@note Find() �� true �̏ꍇ�A���ɂ��̃��\�b�h���g�p���܂��B���̌�J��Ԃ���
	 *			�{���\�b�h�g�����Ƃŕ����̏��𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _data ���������ꍇ�A������񂪊i�[����܂��B
	 *	@retval true ��ȏ㔭���B
	 *	@retval false �������B
	 */
	virtual bool OnNext(WIN32_FIND_DATA& _data)
	{
		if ( m_pDll != NULL && m_hArc != NULL )
		{
			int r;
			m_pDll->Func(r, "FindNext", m_hArc, &m_foundInfo);
			if ( r == 0 )
			{
				return m_MakeWin32Info(_data);
			}
		}
		return false;
	}

	/**
	 * [�ʒm] �����I���ʒm
	 *	@note �������̏ꍇ�A�������~���܂��i OnNext() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	virtual void OnFinish(void)
	{
		if ( m_pDll != NULL && m_hArc != NULL )
		{
			m_pDll->CloseArchive(m_hArc);
			m_hArc = NULL;
		}
	}
};



}; // TNB
