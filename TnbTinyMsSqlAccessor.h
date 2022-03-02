#pragma once
/**
 *@file
 * �Ȉ� SQL �A�N�Z�X�֌W�̃w�b�_
 *
 *		SQL �A�N�Z�X���邽�߂̃N���X�Q������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifdef __TNBPROCESSHANDLE_H__
 #ifndef _TnbPROCESS_PipeInOut_ENABLE
  #error Please define _TnbPROCESS_PipeInOut_ENABLE first rather than you include TnbProcessHandle.h. 
 #endif
#endif

#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #define _TnbPROCESS_PipeInOut_ENABLE
#endif
#include <TnbProcessHandle.h>
#include <TnbTinySqlAccessor.h>
#include <TnbDntStr.h>
#include <TnbFileName.h>
#include <odbcinst.h>
#pragma comment(lib, "odbccp32.lib")



namespace TNB
{



/**@ingroup DBASE
 * ODBC �R���t�B�O���[�V�����N���X
 *
 *		ODBC �f�[�^�\�[�X �A�h�~�j�X�g���[�^�̑�����s�����Ƃ��o����N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTinyMsSqlAccessor.h
 * 
 *	@date 12/01/20	�V�K�쐬
 *	@date 12/06/12	�V�X�e�� DSN �� ���[�U�[ DSN �ɕύX�B
 */
class COdbcConfiguration
{
public:
	
	/**
	 * [�ǉ�] ���[�U�[ DSN �ǉ�.
	 *	@param lpszDriver �h���C�o�[��
	 *	@param lpszDsn DSN��
	 *	@param lpszServer �T�[�o�[��
	 *	@param lpszPath �p�X��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool AddUserDsn(LPCTSTR lpszDriver, LPCTSTR lpszDsn, LPCTSTR lpszServer, LPCTSTR lpszPath)
	{
		CDoubleNullTerminateStr dnt;
		dnt.Add(CStr::Fmt(_T("DSN=%s"), lpszDsn));
		dnt.Add(CStr::Fmt(_T("SERVER=%s"), lpszServer));
		dnt.Add(_T("Trusted_Connection=Yes"));
		dnt.Add(CStr::Fmt(_T("Database=%s"), lpszPath));
		return ConfigDataSource(ODBC_ADD_DSN, lpszDriver, dnt);
	}

	/**
	 * [�폜] ���[�U�[ DSN �폜.
	 *	@param lpszDriver �h���C�o�[��
	 *	@param lpszDsn DSN��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool RemoveUserDsn(LPCTSTR lpszDriver, LPCTSTR lpszDsn)
	{
		CDoubleNullTerminateStr dnt;
		dnt.Add(CStr::Fmt(_T("DSN=%s"), lpszDsn));
		return ConfigDataSource(ODBC_REMOVE_DSN, lpszDriver, dnt);
	}

	/**
	 * [�m�F] ���[�U�[ DSN �m�F
	 *	@param lpszDriver �h���C�o�[��
	 *	@param lpszDsn DSN��
	 *	@retval true ���� DSN �͂���B
	 *	@retval false �Ȃ��B
	 */
	static bool HasUserDsn(LPCTSTR lpszDriver, LPCTSTR lpszDsn)
	{
		CDoubleNullTerminateStr dnt;
		dnt.Add(CStr::Fmt(_T("DSN=%s"), lpszDsn));
		return ConfigDataSource(ODBC_CONFIG_DSN, lpszDriver, dnt);
	}

	/**
	 * [�ݒ�] �f�[�^�\�[�X�ݒ�.
	 *	@param request ���N�G�X�g�B ODBC_ADD_DSN �� ODBC_REMOVE_DSN �ȂǁB
	 *	@param lpszDriver �h���C�o�[��
	 *	@param attributes �ݒ���e
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool ConfigDataSource(WORD request, LPCTSTR lpszDriver, const CDoubleNullTerminateStr& attributes)
	{
		BOOL r = ::SQLConfigDataSource(NULL, request, lpszDriver, attributes);
		if ( ! r )
		{
			GetLastErrors();
		}
		return !! r;
	}

	/**
	 * [�擾] ���X�g�G���[������擾.
	 *	@param[out] pErrorCodes �G���[�R�[�h�i�[�p�B NULL �w��\�ł��B 
	 *	@return �G���[������擾.
	 */
	static CStrVector GetLastErrors(CDwordVector* pErrorCodes = NULL)
	{
		CStrVector vs;
		WORD count = 1;
		DWORD errorCode;
		const WORD errorMsgMax = 300;
		TCHAR errorMsg[errorMsgMax + 1];
		if ( pErrorCodes != NULL )
		{
			pErrorCodes->RemoveAll();
		}
		while ( true )
		{
			RETCODE rc = ::SQLInstallerError(count++, &errorCode, errorMsg, errorMsgMax, NULL);
			if ( rc == SQL_NO_DATA )
			{
				break;
			}
			if ( rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO )
			{
				TRACE1("%s\n", CStr(errorMsg));
				vs.Add(CStr(errorMsg));
				if ( pErrorCodes != NULL )
				{
					pErrorCodes->Add(errorCode);
				}
			}
		}
		return vs;
	}
};



/**@ingroup DBASE
 * �Ȉ� MS SQL �A�N�Z�T�N���X
 *
 *		AttachDataFile() ���g���ăA�^�b�`���� MDF �t�@�C���ɐڑ����A SQL �����s���邱�Ƃ��o���܂��B
 *		�߂�l���擾���邱�Ƃ��o���܂��B
 *
 *	@par�A�^�b�`��
 *	\code
 *			// testdb1.mdf �� XYZ �ƌ������O�ŃA�^�b�`
 *			CTinyMsSqlAccessor::AttachDataFile("XYZ", "c:\\home\\temp\\testdb1.mdf", "SQL Server Native Client 10.0");
 *	\endcode
 *	@par�f�^�b�`��
 *	\code
 *			// XYZ �ƌ������O�Ńf�^�b�`
 *			CTinyMsSqlAccessor::DetachDataFile("XYZ", "SQL Server Native Client 10.0");
 *	\endcode
 *	@par�g�p��
 *	\code
 *		CTinyMsSqlAccessor sql;
 *		bool r = sql.ConnectDataFile("XYZ");
 *		CTinyMsSqlAccessor::CReturnCode rc = sql.GetLastReturnCode();
 *		if ( r ) {
 *			;
 *		}
 *		TRACE1("ret = %s\n", rc.GetErrorString());
 *		return;
 *	\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbTinyMsSqlAccessor.h
 * 
 *	@date 12/01/20	�V�K�쐬
 *	@date 12/01/26	AttachDataFile() �̃G���[�`�F�b�N�������B
 */
class CTinyMsSqlAccessor : public CTinySqlAccessor
{
	DEFSUPER(CTinySqlAccessor);
public:

	/**
	 * [�ݒ�] �ڑ�.
	 *		�w��̐ݒ�ŁA�f�[�^�x�[�X�Ɛڑ����܂��B
	 *	@note Start() ���Ă��Ȃ��ꍇ�A�X�^�[�g���܂��B
	 *	@param lpszManageName �Ǘ����BAttachDataFile() ���g���AMDF �t�@�C���ɊǗ�����t���ăA�^�b�`���Ă����K�v������܂��B
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool ConnectDataFile(LPCTSTR lpszManageName = NULL)
	{
		return _super::Connect(lpszManageName, _T(""), _T(""));
	}


	//-----


	/**
	 * [�ݒ�] �f�[�^�t�@�C���̃A�^�b�`.
	 *		�w��̃f�[�^�t�@�C�����w��̊Ǘ����Ńf�[�^�x�[�X�ɓo�^���܂��B
	 *	@note ��x�A�^�b�`����ƁA�f�^�b�`����܂ŁA�c��܂��B
	 *	@attention sqlcmd �̉����������s���̂��߁A�{���\�b�h���s���Ƀ_�C�A���O���o�āA���[�U�ɐ���𔻒f������\��������܂��B
	 *	@param lpszManageName �Ǘ����B
	 *	@param lpszMdfPath MDF �t�@�C���̃p�X�B
	 *	@param lpszDriver �h���C�o�[���B
	 *	@param lpszServer �T�[�o�[���B�ȗ������ .��SQLEXPRESS �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool AttachDataFile(LPCTSTR lpszManageName, LPCTSTR lpszMdfPath, LPCTSTR lpszDriver, LPCTSTR lpszServer = NULL)
	{
		CStr server = lpszServer;
		if ( server.IsEmpty() )
		{
			server = _T(".\\SQLEXPRESS");
		}
		CFileName fn1 = lpszMdfPath;
		CFileName fn2 = fn1.InsertAtEndOfName(_T("_log"), _T("ldf"));
		//
		DetachDataFile(lpszManageName, lpszDriver, lpszServer);
		//
		CStr cl;
		// sqlcmd -S .\SQLEXPRESS -Q "sp_attach_db 'test','C:\Home\temp\TestDb1.mdf','C:\Home\temp\TestDb1_log.ldf';"
		cl.Format(_T("sqlcmd -S %s -Q \"sp_attach_db '%s','%s','%s';\"")
			, server, lpszManageName, fn1.GetFullName(), fn2.GetFullName());
		CAscii ss;
		bool r = ms_Execute(ss, cl);
		if ( r && ! ss.IsEmpty() )
		{
			MB_OK;
			CStr s;
			s.Format(_T("SQL�c�[�����ȉ��̃��b�Z�[�W��Ԃ��܂����B\n����Ȃ�u�͂��v�A���s�Ȃ�u�������v��I�����Ă��������B\n\n-----\n\n%s"), CStr(ss));
			if ( ::MessageBox(NULL, s, _T("AttachDataFile"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO )
			{
				r = false;
			}
		}
		if ( ! r )
		{
			CStr cl2;
			cl2.Format(_T("sqlcmd -S %s -Q \"drop database %s;\""), server, lpszManageName);
			if ( ! ms_Execute(cl2) )
			{
				return false;
			}
			if ( ! ms_Execute(cl) )
			{
				return false;
			}
		}
		return COdbcConfiguration::AddUserDsn(lpszDriver, lpszManageName, server, lpszManageName);
	}

	/**
	 * [�ݒ�] �f�[�^�t�@�C���̃f�^�b�`.
	 *		AttachDataFile() �ŃA�^�b�`�������̂��f�^�b�`���܂��B
	 *	@param lpszManageName �Ǘ����B
	 *	@param lpszDriver �h���C�o�[���B
	 *	@param lpszServer �T�[�o�[���B�ȗ������ .��SQLEXPRESS �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool DetachDataFile(LPCTSTR lpszManageName, LPCTSTR lpszDriver, LPCTSTR lpszServer = NULL)
	{
		bool r = COdbcConfiguration::RemoveUserDsn(lpszDriver, lpszManageName);
		// sqlcmd -S .\SQLEXPRESS -Q "sp_detach_db 'test'';"
		CStr server = lpszServer;
		if ( server.IsEmpty() )
		{
			server = _T(".\\SQLEXPRESS");
		}
		CStr cl;
		cl.Format(_T("sqlcmd -S %s -Q \"sp_detach_db '%s';\""), server, lpszManageName);
		return ms_Execute(cl) & r;
	}

	/**
	 * [�m�F] �f�[�^�t�@�C���̃A�^�b�`�ς݁H.
	 *	@note AttachDataFile() �ŃA�^�b�`�������̈ȊO���w��̖��O������΁A�A�^�b�`�ς݂ɂȂ�܂��B
	 *	@param lpszManageName �Ǘ����B
	 *	@param lpszDriver �h���C�o�[���B
	 *	@retval true �A�^�b�`�ς݁B
	 *	@retval false �ہB
	 */
	static bool IsDataFileAttached(LPCTSTR lpszManageName, LPCTSTR lpszDriver)
	{
		return COdbcConfiguration::HasUserDsn(lpszDriver, lpszManageName);
	}

	/**
	 * [�ݒ�] �f�[�^�t�@�C���̃o�b�N�A�b�v.
	 *	@param lpszBackupFileName �o�b�N�A�b�v�t�@�C�����B
	 *	@param lpszManageName �Ǘ����B
	 *	@param lpszServer �T�[�o�[���B�ȗ������ .��SQLEXPRESS �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool BackupDataFile(LPCTSTR lpszBackupFileName, LPCTSTR lpszManageName, LPCTSTR lpszServer = NULL)
	{
		// sqlcmd -S .\SQLEXPRESS -Q "backup database testDB to disk = 'c:\aa';"
		CStr server = lpszServer;
		if ( server.IsEmpty() )
		{
			server = _T(".\\SQLEXPRESS");
		}
		CStr cl;
		cl.Format(_T("sqlcmd -S %s -Q \"backup database %s to disk ='%s' WITH INIT;\""), server, lpszManageName, lpszBackupFileName);
		CAscii r;
		ms_Execute(r, cl);
		return r.Find("����ɏ���") != INVALID_INDEX;
	}

	/**
	 * [�ݒ�] �f�[�^�t�@�C���̃��X�g�A.
	 *	@param lpszBackupFileName �o�b�N�A�b�v�t�@�C�����B
	 *	@param lpszManageName �Ǘ����B
	 *	@param lpszServer �T�[�o�[���B�ȗ������ .��SQLEXPRESS �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool RestoreDataFile(LPCTSTR lpszBackupFileName, LPCTSTR lpszManageName, LPCTSTR lpszServer = NULL)
	{
		// sqlcmd -S .\SQLEXPRESS -Q "restore database testDB from disk = 'c:\aa';"
		CStr server = lpszServer;
		if ( server.IsEmpty() )
		{
			server = _T(".\\SQLEXPRESS");
		}
		CStr cl;
		cl.Format(_T("sqlcmd -S %s -Q \"restore database %s from disk ='%s' WITH RECOVERY;\""), server, lpszManageName, lpszBackupFileName);
		CAscii r;
		ms_Execute(r, cl);
		return r.Find("����ɏ���") != INVALID_INDEX;
	}
private:
	static bool ms_Execute(CAscii& _res, LPCTSTR lpszCommandLine)
	{
		_res.Empty();
		CProcessHandle ph;
		CPipe pipeIn;
		CPipe pipeOut;
		VERIFY( pipeIn.Open() );
		VERIFY( pipeOut.Open() );
		if ( ! ph.Create(lpszCommandLine, &pipeIn, &pipeOut, NULL, FALSE) )
		{
			return false;
		}
		ph.WaitForExit(5000);
		CWorkMemT<char> a(500);
		a.Fill(0);
		if ( pipeOut.Receive(500, a) > 0 )
		{
			_res = a;
		}
		return true;
	}
	static bool ms_Execute(LPCTSTR lpszCommandLine)
	{
		CAscii r;
		if ( ! ms_Execute(r, lpszCommandLine))
		{
			return false;
		}
		if ( ! r.IsEmpty() )
		{
			// ����������΂���̓G���[�B
			TRACE1("%s\n", CStr(r));
			return false;
		}
		return true;
	}
};



} // TNB
