#pragma once
/**
 *@file
 * 簡易 SQL アクセス関係のヘッダ
 *
 *		SQL アクセスするためのクラス群があります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifdef __TNBPROCESSHANDLE_H__
 #ifndef _TnbPROCESS_PipeInOut_ENABLE
  #error Please define _TnbPROCESS_PipeInOut_ENABLE first rather than you include TnbProcessHandle.h. 
 #endif
#endif

#ifndef _TnbDOXYGEN	//Document作成用シンボル
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
 * ODBC コンフィグレーションクラス
 *
 *		ODBC データソース アドミニストレータの操作を行うことが出来るクラスです。
 *
 *	@par必要ファイル
 *			TnbTinyMsSqlAccessor.h
 * 
 *	@date 12/01/20	新規作成
 *	@date 12/06/12	システム DSN → ユーザー DSN に変更。
 */
class COdbcConfiguration
{
public:
	
	/**
	 * [追加] ユーザー DSN 追加.
	 *	@param lpszDriver ドライバー名
	 *	@param lpszDsn DSN名
	 *	@param lpszServer サーバー名
	 *	@param lpszPath パス名
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [削除] ユーザー DSN 削除.
	 *	@param lpszDriver ドライバー名
	 *	@param lpszDsn DSN名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool RemoveUserDsn(LPCTSTR lpszDriver, LPCTSTR lpszDsn)
	{
		CDoubleNullTerminateStr dnt;
		dnt.Add(CStr::Fmt(_T("DSN=%s"), lpszDsn));
		return ConfigDataSource(ODBC_REMOVE_DSN, lpszDriver, dnt);
	}

	/**
	 * [確認] ユーザー DSN 確認
	 *	@param lpszDriver ドライバー名
	 *	@param lpszDsn DSN名
	 *	@retval true その DSN はある。
	 *	@retval false ない。
	 */
	static bool HasUserDsn(LPCTSTR lpszDriver, LPCTSTR lpszDsn)
	{
		CDoubleNullTerminateStr dnt;
		dnt.Add(CStr::Fmt(_T("DSN=%s"), lpszDsn));
		return ConfigDataSource(ODBC_CONFIG_DSN, lpszDriver, dnt);
	}

	/**
	 * [設定] データソース設定.
	 *	@param request リクエスト。 ODBC_ADD_DSN や ODBC_REMOVE_DSN など。
	 *	@param lpszDriver ドライバー名
	 *	@param attributes 設定内容
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [取得] ラストエラー文字列取得.
	 *	@param[out] pErrorCodes エラーコード格納用。 NULL 指定可能です。 
	 *	@return エラー文字列取得.
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
 * 簡易 MS SQL アクセサクラス
 *
 *		AttachDataFile() を使ってアタッチした MDF ファイルに接続し、 SQL を実行することが出来ます。
 *		戻り値を取得することが出来ます。
 *
 *	@parアタッチ例
 *	\code
 *			// testdb1.mdf を XYZ と言う名前でアタッチ
 *			CTinyMsSqlAccessor::AttachDataFile("XYZ", "c:\\home\\temp\\testdb1.mdf", "SQL Server Native Client 10.0");
 *	\endcode
 *	@parデタッチ例
 *	\code
 *			// XYZ と言う名前でデタッチ
 *			CTinyMsSqlAccessor::DetachDataFile("XYZ", "SQL Server Native Client 10.0");
 *	\endcode
 *	@par使用例
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
 *	@par必要ファイル
 *			TnbTinyMsSqlAccessor.h
 * 
 *	@date 12/01/20	新規作成
 *	@date 12/01/26	AttachDataFile() のエラーチェックを強化。
 */
class CTinyMsSqlAccessor : public CTinySqlAccessor
{
	DEFSUPER(CTinySqlAccessor);
public:

	/**
	 * [設定] 接続.
	 *		指定の設定で、データベースと接続します。
	 *	@note Start() していない場合、スタートします。
	 *	@param lpszManageName 管理名。AttachDataFile() を使い、MDF ファイルに管理名を付けてアタッチしておく必要があります。
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool ConnectDataFile(LPCTSTR lpszManageName = NULL)
	{
		return _super::Connect(lpszManageName, _T(""), _T(""));
	}


	//-----


	/**
	 * [設定] データファイルのアタッチ.
	 *		指定のデータファイルを指定の管理名でデータベースに登録します。
	 *	@note 一度アタッチすると、デタッチするまで、残ります。
	 *	@attention sqlcmd の応答文字が不明のため、本メソッド実行中にダイアログが出て、ユーザに是非を判断させる可能性があります。
	 *	@param lpszManageName 管理名。
	 *	@param lpszMdfPath MDF ファイルのパス。
	 *	@param lpszDriver ドライバー名。
	 *	@param lpszServer サーバー名。省略すると .￥SQLEXPRESS になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
			s.Format(_T("SQLツールが以下のメッセージを返しました。\n正常なら「はい」、失敗なら「いいえ」を選択してください。\n\n-----\n\n%s"), CStr(ss));
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
	 * [設定] データファイルのデタッチ.
	 *		AttachDataFile() でアタッチしたものをデタッチします。
	 *	@param lpszManageName 管理名。
	 *	@param lpszDriver ドライバー名。
	 *	@param lpszServer サーバー名。省略すると .￥SQLEXPRESS になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
	 * [確認] データファイルのアタッチ済み？.
	 *	@note AttachDataFile() でアタッチしたもの以外も指定の名前があれば、アタッチ済みになります。
	 *	@param lpszManageName 管理名。
	 *	@param lpszDriver ドライバー名。
	 *	@retval true アタッチ済み。
	 *	@retval false 否。
	 */
	static bool IsDataFileAttached(LPCTSTR lpszManageName, LPCTSTR lpszDriver)
	{
		return COdbcConfiguration::HasUserDsn(lpszDriver, lpszManageName);
	}

	/**
	 * [設定] データファイルのバックアップ.
	 *	@param lpszBackupFileName バックアップファイル名。
	 *	@param lpszManageName 管理名。
	 *	@param lpszServer サーバー名。省略すると .￥SQLEXPRESS になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
		return r.Find("正常に処理") != INVALID_INDEX;
	}

	/**
	 * [設定] データファイルのリストア.
	 *	@param lpszBackupFileName バックアップファイル名。
	 *	@param lpszManageName 管理名。
	 *	@param lpszServer サーバー名。省略すると .￥SQLEXPRESS になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
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
		return r.Find("正常に処理") != INVALID_INDEX;
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
			// 文字があればそれはエラー。
			TRACE1("%s\n", CStr(r));
			return false;
		}
		return true;
	}
};



} // TNB
