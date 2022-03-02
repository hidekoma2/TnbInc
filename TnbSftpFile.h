#pragma once
/**
 *@file
 * SSH FTP 関係ヘッダ
 *
 *	@note WinSCP が必要です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include <shlobj.h>
#include "TnbConsoleHooker.h"
#include "TnbFileFinder.h"
#include "TnbStrEx.h"
#include "TnbFileName.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * SSH FTPセッション管理クラス
 *
 *		サーバ接続とファイルの転送を管理します。また、デリート、リネームをサポートします。
 *
 *	@note WinSCP がインストールされている PC でのみ動作します。言語は英語か日本語のみ対応しています。
 *		WinSCP はデフォルトの場所にインストールしているか、パスが通っている必要があります。
 *
 *	@attention WinSCP 4.36 / 4.37 専用です。
 *
 *	@see CFtpSession
 *
 *	@par必要ファイル
 *			TnbSftpFile.h
 *
 *	@date 12/02/07 新規作成
 *	@date 12/02/07 OpenSSH → WinSCP に変更
 *	@date 12/05/21 WinSCP 4.37 にも対応。
 */
class CSftpSession : CCopyImpossible
{
public:

	/**
	 * コンストラクタ.
	 *	@param lpszWinScpPath WinSCP のパス。省略すると ≪Program Files≫￥WinSCP になります。
	 */
	CSftpSession(LPCTSTR lpszWinScpPath = NULL)
	{
		m_winScpPath = lpszWinScpPath;
		if ( m_winScpPath.IsEmpty() )
		{
			TCHAR buf[MAX_PATH];
			#ifdef CSIDL_PROGRAM_FILESX86
				if ( ! SUCCEEDED(::SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, buf)) )
			#endif
			{
				::SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buf);
			}
			CFileName fn = buf;
			m_winScpPath = fn.GetFullShortName();
		}
		m_winScpPath.TrimRight(_T("\\"));
		m_winScpPath += _T("\\");
	}

	/// デストラクタ
	~CSftpSession(void)
	{
		Close();
	}

	/**
	 * [設定] オープン.
	 *		本クラスのメソッドを使用するには、まずオープンする必要があります。
	 *	@param lpszServerName	接続先のホスト名またはドット表記のIPアドレス。
	 *	@param lpszUserName		接続の際に使用されるユーザ名。
	 *	@param lpszPassword		接続の際に使用されるパスワード。
	 *	@param to タイムアウト時間(ms)。省略すると 10秒
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool Open(LPCTSTR lpszServerName, LPCTSTR lpszUserName, LPCTSTR lpszPassword, DWORD to = 10000)
	{
		if ( STRLIB::GetLen(lpszUserName) == 0 || STRLIB::GetLen(lpszPassword) == 0 )
		{
			return false;
		}
		Close();
		CStr cl = m_winScpPath + _T("WinSCP\\WinSCP.com");
		STARTUPINFO si = { 0 };
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USECOUNTCHARS;
		si.wShowWindow = SW_HIDE;
		si.dwXCountChars = 256;
		si.dwYCountChars = 10000;
		if ( ! m_exe.Create(NULL, cl, si) )
		{
			cl = _T("WinSCP.com");
			m_exe.Create(NULL, cl, si);
		}
		if ( m_exe.IsAlive() )
		{
			::Sleep(1000);
			if ( m_sftp.Hook(m_exe.GetProcessId()) )
			{
				if ( m_sftp.WaitString(_T("winscp>"), to) )
				{
					CStrVector vs = CStr(_T("not exist"))		% _T("not exist")					// 0
								% _T("Session started.")		% _T("セッションを開始しました")	// 1
								% _T("Authentication failed") 	% _T("認証に失敗")					// 2
								% _T("to the cache?")			% _T("ホスト鍵をキャッシュに追加")	// 3
								% _T("(U)pdate")				% _T("更新((U))");					// 4
					cl.Format(_T("open %s:%s@%s\r"), lpszUserName, lpszPassword, lpszServerName);
					m_sftp.SendString(cl);
					int r = m_sftp.WaitStringEx(vs, to);
					r = (r >= 0) ? (r / 2) : r;
					if ( r == 4/*更新*/ )
					{
						m_sftp.SendString(_T("u"));
						r = m_sftp.WaitStringEx(vs, 1000);
						r = (r >= 0) ? (r / 2) : r;
					}
					if ( r == 3/*CopyKey*/ )
					{
						m_sftp.SendString(_T("y"));
						r = m_sftp.WaitStringEx(vs, 1000);
						r = (r >= 0) ? (r / 2) : r;
					}
					if ( r == 1 )
					{
						// 接続成功
						m_currentLocalDir = ms_GetCurrentDir();
						cl.Format(_T("lcd %s\r"), ms_DQ(m_currentLocalDir));
						if ( SftpCommand(cl) && SftpCommand(_T("option confirm off\r")) )
						{
							return true;
						}
					}
				}
				m_sftp.SendString(_T("exit\r"));
				m_sftp.GetString();
				m_sftp.Unhook();
				m_sftp.Terminate();
			}
			m_exe.Terminate();
		}
		return false;
	}

	/**
	 * [設定] クローズ.
	 *	@note オープンしていない時は何もしません。
	 */
	void Close(void)
	{
		if ( m_sftp.IsHooked() )
		{
			m_sftp.SendString(_T("close\r"));
			m_sftp.SendString(_T("exit\r"));
			m_sftp.GetString();
			m_sftp.Unhook();
			m_exe.WaitForExit(5000);
			m_exe.Terminate();
		}
	}

	/**
	 * [確認] ハンドル有効確認
	 *	@retval ture 有効。
	 *	@retval false 無効(開いていないか、失敗したか)。
	 */
	bool IsValid(void) const
	{
		return m_sftp.IsAlived();
	}

	/**
	 * [作成] リモートディレクトリ作成.
	 *	@param lpszRemoteDir	作成するリモートディレクトリ名。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool CreateRemoteDir(LPCTSTR lpszRemoteDir)
	{
		CStr cl;
		cl.Format(_T("mkdir %s\r"), ms_DQ(lpszRemoteDir));
		return SftpCommand(cl);
	}

	/**
	 * [削除] リモートディレクトリ削除.
	 *	@param lpszRemoteDir	削除するリモートディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool RemoveRemoteDir(LPCTSTR lpszRemoteDir)
	{
		CStr cl;
		cl.Format(_T("rmdir %s\r"), ms_DQ(lpszRemoteDir));
		return SftpCommand(cl);
	}

	/**
	 * [取得] カレントリモートディレクトリ取得.
	 *		FTPセッション中でのカレントディレクトリを取得する。 
	 *	@param[out]		_currentRemoteDir	カレントディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool GetCurrentRemoteDir(CStr& _currentRemoteDir)
	{
		CStr cl = _T("pwd\r");
		if ( SftpCommand(cl) )
		{
			CStrVector log = GetLastString();
			loop ( i, log )
			{
				if ( ! log[i].IsEmpty() && log[i][0] == '/' )
				{
					_currentRemoteDir = log[i];
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * [設定] カレントリモートディレクトリ設定.
	 *	@param lpszRemoveDir	変更先のディレクトリ名。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool SetCurrentRemoteDir(LPCTSTR lpszRemoveDir)
	{
		CStr cl;
		cl.Format(_T("cd %s\r"), ms_DQ(lpszRemoveDir));
		return SftpCommand(cl);
	}

	/**
	 * [取得] リモートファイル取得.
	 *	@param lpszNewLocalFile	取得先のローカルファイル名。
	 *	@param lpszRemoteFile	取得元のリモートファイル名。
	 *	@param to タイムアウト時間(ms)。省略すると 10秒
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool GetRemoteFile(LPCTSTR lpszNewLocalFile, LPCTSTR lpszRemoteFile, DWORD to = 10000)
	{
		if ( ! AdjustCurrentLocalDir() )
		{
			return false;
		}
		CStr cl;
		cl.Format(_T("get -preservetime %s %s\r"), ms_DQ(lpszRemoteFile), ms_DQ(lpszNewLocalFile));
		return SftpCommand(cl, to);
	}

	/**
	 * [送信] ローカルファイル送信.
	 *	@param lpszNewRemoteFile	転送先のリモートファイル名。
	 *	@param lpszLocalFile	転送元のローカルファイル名。
	 *	@param to タイムアウト時間(ms)。省略すると 10秒
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool PutLocalFile(LPCTSTR lpszNewRemoteFile, LPCTSTR lpszLocalFile, DWORD to = 10000)
	{
		if ( ! AdjustCurrentLocalDir() )
		{
			return false;
		}
		CStr cl;
		cl.Format(_T("put -preservetime %s %s\r"), ms_DQ(lpszLocalFile), ms_DQ(lpszNewRemoteFile));
		return SftpCommand(cl, to);
	}

	/// 同期モード
	enum ESyncMode
	{
		SyncMode_Local,		///< リモートからローカルへの同期
		SyncMode_Remote,	///< ローカルからリモートへの同期
		SyncMode_Both,		///< 両方
	};

	/// 同期基準
	enum ESyncCriteria
	{
		SyncCriteria_Time,	///< 時間
		SyncCriteria_Size,	///< ファイルサイズ
		SyncCriteria_Both,	///< 両方
		SyncCriteria_Non,	///< なし
	};

	/**
	 * [同期] ローカルとリモートの同期.
	 *	@param lpszRemoteDir リモートディレクトリ名。
	 *	@param lpszLocalDir ローカルディレクトリ名。
	 *	@param mode 同期モード。
	 *	@param criteria 同期基準。
	 *	@param to タイムアウト時間(ms)。省略すると 10秒
	 *	@param lpszOption オプション。たとえば、”-delete”など。省略可能。
	 *	@retval true 成功。成功したファイル名、数は、 GetSynchronizedFiles() で得られます。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool Synchronize(LPCTSTR lpszRemoteDir, LPCTSTR lpszLocalDir, ESyncMode mode, ESyncCriteria criteria = SyncCriteria_Both, DWORD to = 10000, LPCTSTR lpszOption = NULL)
	{
		m_synchronizedFiles.RemoveAll();
		if ( ! AdjustCurrentLocalDir() )
		{
			return false;
		}
		CStr s1;
		switch ( mode )
		{
		case SyncMode_Local:	s1 = _T("local");	break;
		case SyncMode_Remote:	s1 = _T("remote");	break;
		case SyncMode_Both:		s1 = _T("both");	break;
		default:
			return false;
		}
		CStr s2;
		switch ( criteria )
		{
		case SyncCriteria_Time:	s2 = _T("-criteria=time");	break;
		case SyncCriteria_Size:	s2 = _T("-criteria=size");	break;
		case SyncCriteria_Both:	s2 = _T("-criteria=both");	break;
		case SyncCriteria_Non:								break;
		default:
			return false;
		}
		CStr s3 = lpszOption;
		//
		CStr cl;
		cl.Format(_T("synchronize %s %s %s %s %s\r"), s1, s2, s3, ms_DQ(lpszLocalDir), ms_DQ(lpszRemoteDir));
		if ( ! SftpCommand(cl, to) )
		{
			return false;
		}
		const CStrVector& log = GetLastString();
		CStr last;
		loop ( i, log )
		{
			const CStr& ss = log[i];
			INT_PTR f = ss.Find(_T('|'));
			if ( f >= 0 )
			{
				CStr s = ss.Left(f).TrimRight(_T(' '));
				if ( last != s )
				{
					m_synchronizedFiles.Add(s);
					last = s;
				}
			}
		}
		return true;
	}

	/**
	 * [取得] 同期結果ファイル名取得.
	 *		Synchronize() の結果、同期されたファイル名、ディレクトリ名が格納されます。
	 *	@note パス名が長い場合、省略されることがあります。
	 *	@return ファイル名。
	 */
	const CStrVector& GetSynchronizedFiles(void) const
	{
		return m_synchronizedFiles;
	}

	/**
	 * [削除] 指定リモートファイル削除.
	 *	@param lpszFileName	削除するリモートファイル名。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool DeleteRemoteFile(LPCTSTR lpszFileName)
	{
		CStr cl;
		cl.Format(_T("rm %s\r"), ms_DQ(lpszFileName));
		return SftpCommand(cl);
	}

	/**
	 * [変更] 名前変更.
	 *		リモートファイル名、リモートディレクトリ名の変更が出来ます。
	 *	@param lpszOldName	元のリモートファイル名。
	 *	@param lpszNewName	変更後のリモートファイル名。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool RenameRemoteFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
	{
		CStr cl;
		cl.Format(_T("rename %s %s\r") , ms_DQ(lpszOldName), ms_DQ(lpszNewName));
		return SftpCommand(cl);
	}

	/**
	 * [取得] ファイル一覧.
	 *	@param[out] _ls	ファイル一覧結果格納。
	 *	@param[in] lpszRemoteDir	リモートディレクトリ。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool EnumRemoteFiles(CWorkMemT<WIN32_FIND_DATA>& _ls, LPCTSTR lpszRemoteDir = NULL)
	{
		CStr cl;
		cl.Format(_T("ls %s\r"), ms_DQ(lpszRemoteDir));
		if ( SftpCommand(cl) )
		{
			CVectorT<WIN32_FIND_DATA> ff;
			WIN32_FIND_DATA f = { 0 };
			SYSTEMTIME st = { 0 };
			CStrVector log = GetLastString();
			loop ( i, log )
			{
				CStr s = log[i];
				while ( s.Replace(_T("  "), _T(" ")) > 0 );
				CStrVector vs = CStrOperator::SeparatePeriod(s, ' ', false);
				if ( vs.GetSize() >= 11 )
				{
					// WinSCP 4.36
					// 0            1 2         3      4             5 6   7   8       9    10
					// -rwx------   1 h-komatsu Domain Users      7534 Dec 22  9:04:25 2011 SILV ER.bmp
					const CStr& attb = vs[0];
					CStrVector mvs = CStrOperator::SeparatePeriod(vs[8], ':', false);
					if ( mvs.GetSize() == 3 && attb.GetLength() >= 4 )
					{
						f.dwFileAttributes = (attb[0] == 'd') ? FILE_ATTRIBUTE_DIRECTORY : 0;
						if ( attb[1] !='r' )
						{
							f.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
						}
						st.wYear = down_cast<WORD>(vs[9].ToInt());
						st.wMonth = down_cast<WORD>(ms_ToMonth(vs[6]));
						st.wDay = down_cast<WORD>(vs[7].ToInt());
						st.wHour = down_cast<WORD>(mvs[0].ToInt());
						st.wMinute = down_cast<WORD>(mvs[1].ToInt());
						st.wSecond = down_cast<WORD>(mvs[2].ToInt());
						::FILETIME ft;
						::SystemTimeToFileTime(&st, &ft);
						::LocalFileTimeToFileTime(&ft, &f.ftCreationTime);
						f.ftLastAccessTime = f.ftCreationTime;
						f.ftLastWriteTime = f.ftCreationTime;
						f.nFileSizeLow = vs[5].ToInt();
						//
						CStr txt;
						txt.Format(_T("%d:%02d:%02d %4d "), st.wHour, st.wMinute, st.wSecond, st.wYear);
						int l = ToInt(s.Find(txt));
						ASSERT( l != INVALID_INDEX );
						txt = s.Mid(l + txt.GetLength() );
						ASSERT( txt.GetLength() < MAX_PATH );
						STRLIB::Copy(f.cFileName, txt);
						ff.Add(f);
						continue;
					}
				}
				if ( vs.GetSize() >= 8 )
				{
					// WinSCP 4.37
					// 0            1                    2        3   4  5        6    7
					// -rw-rw-rw-   0                    10000000 May 21 12:43:01 2012 B.dat
					// Drw-rw-rw-   0                           0 May 21 12:43:25 2012 ..
					const CStr& attb = vs[0];
					CStrVector mvs = CStrOperator::SeparatePeriod(vs[5], ':', false); //時分秒
					if ( mvs.GetSize() == 3 && attb.GetLength() >= 4 )
					{
						f.dwFileAttributes = ((attb[0]|0x40) == 'd') ? FILE_ATTRIBUTE_DIRECTORY : 0;
						if ( (attb[1]|0x40) !='r' )
						{
							f.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
						}
						st.wYear = down_cast<WORD>(vs[6].ToInt());
						st.wMonth = down_cast<WORD>(ms_ToMonth(vs[3]));
						st.wDay = down_cast<WORD>(vs[4].ToInt());
						st.wHour = down_cast<WORD>(mvs[0].ToInt());
						st.wMinute = down_cast<WORD>(mvs[1].ToInt());
						st.wSecond = down_cast<WORD>(mvs[2].ToInt());
						::FILETIME ft;
						::SystemTimeToFileTime(&st, &ft);
						::LocalFileTimeToFileTime(&ft, &f.ftCreationTime);
						f.ftLastAccessTime = f.ftCreationTime;
						f.ftLastWriteTime = f.ftCreationTime;
						f.nFileSizeLow = vs[2].ToInt();
						//
						CStr txt;
						txt.Format(_T("%d:%02d:%02d %4d "), st.wHour, st.wMinute, st.wSecond, st.wYear);
						int l = ToInt(s.Find(txt));
						ASSERT( l != INVALID_INDEX );
						txt = s.Mid(l + txt.GetLength() );
						ASSERT( txt.GetLength() < MAX_PATH );
						STRLIB::Copy(f.cFileName, txt);
						ff.Add(f);
						continue;
					}
				}
			}
			_ls.Resize(ff);
			loop ( i, ff )
			{
				_ls[i] = ff[i];
			}
			return true;
		}
		return false;
	}

	/**
	 * [確認] ファイル有無.
	 *	@param[out] _fd	ファイル情報格納。
	 *	@param[in] lpszRemoteFile	リモートファイル/ディレクトリ。ワイルドカードは使用禁止。
	 *	@retval true あり。
	 *	@retval false 無し or 失敗。理由は GetLastString() で得られます。
	 */
	bool ExistRemoteFile(WIN32_FIND_DATA& _fd, LPCTSTR lpszRemoteFile)
	{
		CStr rf = lpszRemoteFile;
		rf += _T("*"); //ワイルドにする
		CWorkMemT<WIN32_FIND_DATA> ls;
		if ( EnumRemoteFiles(ls, rf) )
		{
			loop ( i, ls.GetSize() )
			{
				if ( STRLIB::Compare(ls[i].cFileName, lpszRemoteFile, -1, NORM_IGNORECASE) == 0 )
				{
					_fd = ls[i];
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * [確認] ファイル有無.
	 *	@param lpszRemoteFile	リモートファイル/ディレクトリ。ワイルドカードは使用禁止。
	 *	@retval true あり。
	 *	@retval false 無し or 失敗。理由は GetLastString() で得られます。
	 */
	bool ExistRemoteFile(LPCTSTR lpszRemoteFile)
	{
		WIN32_FIND_DATA fd;
		return ExistRemoteFile(fd, lpszRemoteFile);
	}

	/**
	 * [取得] 取得文字列取得.
	 *		各関数の実行時、得られた文字列を返します。
	 *	@return 文字列
	 */
	const CStrVector& GetLastString(void) const
	{
		return m_sftp.GetLastString();
	}

	/**
	 * [設定] 取得文字列設定.
	 *		GetLastString() で得られる文字列を設定します。
	 *	@param vs 文字列
	 */
	void SetLastString(const CStrVector& vs)
	{
		m_sftp.SetLastString(vs);
	}

	/**
	 * [設定] 強制終了 WinSCP.
	 *		プログラムが異常終了した際、 WinScp が残ってしまうケースがあります。
	 *		これは winscp を強制終了するメソッドです。
	 */
	static void TerminateWinscp(void)
	{
		CWorkMemT<DWORD> pids;
		loop ( j, 3 )
		{
			if ( CProcessHandle::EnumProcesses(pids, _T("WinSCP.com")) )
			{
				loop ( i, pids.GetSize() )
				{
					CProcessHandle::TerminateProcessId(pids[i]);
				}
			}
			if ( CProcessHandle::EnumProcesses(pids, _T("WinSCP.exe")) )
			{
				loop ( i, pids.GetSize() )
				{
					CProcessHandle::TerminateProcessId(pids[i]);
				}
			}
		}
	}

protected:

	/**
	 * [実行] SFTP コマンドライン実行.
	 *	@param lpszCommandLine コマンドライン
	 *	@param to タイムアウト
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool SftpCommand(LPCTSTR lpszCommandLine, DWORD to = 1000)
	{
		CStr cl = lpszCommandLine;
		cl.Trim(_T(' '));
		if ( m_sftp.SendString(cl) )
		{
			CStrVector vs = CStr(_T("(A)bort,")) % _T("中止((A))") % _T("No session.") % _T("セッションなし") % _T("winscp>");
			int r = m_sftp.WaitStringEx(vs, to);
			r = (r >= 0) ? (r / 2) : r;
			if ( r == 2/*winscp>*/ )
			{
				return true;
			}
			if ( r == 1 )
			{
				Close();
			}
			else if ( r == 0/*abort*/ )
			{
				m_sftp.SendString(_T("a"));
				m_sftp.WaitString(_T("winscp>"), 1000);
			}
		}
		return false;
	}

	/**
	 * [実行] カレントローカルディレクトリ調整.
	 *		WinSCP と本プロセスのカレントディレクトリを同期します。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は GetLastString() で得られます。
	 */
	bool AdjustCurrentLocalDir(void)
	{
		CStr cp = ms_GetCurrentDir();
		if ( m_currentLocalDir != cp )
		{
			CStr cl;
			cl.Format(_T("lcd %s\r"), ms_DQ(cp));
			if ( ! SftpCommand(cl) )
			{
				return false;
			}
			m_currentLocalDir = cp;
		}
		return true;
	}

private:
	/// カレントローカルディレクトリ取得
	static CStr ms_GetCurrentDir(void)
	{
		CStr str;
		DWORD dwRc = ::GetCurrentDirectory(MAX_PATH, str.GetBuffer(MAX_PATH));
		str.ReleaseBuffer();
		ASSERTLIB( dwRc != 0 );
		IgnoreUnusedValue (dwRc);
		return str;	
	}
	/// ダブルクオート文字列化
	static CStr ms_DQ(LPCTSTR lpsz)
	{
		CStr s = lpsz;
		if ( ! s.IsEmpty() )
		{
			s = s.Sandwich(_T("\""), _T("\""));
		}
		return s;
	}
	/// 月文字列→数値
	static int ms_ToMonth(LPCTSTR lpsz)
	{
		const LPCTSTR m[] =
		{
			_T("jan"), _T("feb"), _T("mar"), _T("apr"), 
			_T("may"), _T("jun"), _T("jul"), _T("aug"), 
			_T("sep"), _T("oct"), _T("nov"), _T("dec")
		};
		CStr s = LowerString(lpsz);
		loop ( i, countof(m) )
		{
			if ( s.IsEqual(m[i]) )
			{
				return ToInt(i + 1);
			}
		}
		return -1;
	}
	CProcessHandle	m_exe;					///< WinSCP.com 管理
	CConsoleHooker	m_sftp;					///< SFTP コマンド管理
	CStr			m_currentLocalDir;		///< カレントローカルディレクトリ
	CStr			m_winScpPath;			///< WinSCPパス
	CStrVector		 m_synchronizedFiles;	///< 同期結果ファイル
	friend class CSftpSessionTest;
};



/**@ingroup FILE
 * SFTP ファイル検索クラス
 *
 *		指定のディレクトリからファイルを検索するためのクラスです。
 *		
 *	@note 検索時、ディレクトリ情報、[.][..]は、除外されます。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		CFileFinder ff;
 *		if ( ff.Start(_T(".\\*.*")) )
 *		{
 *			do
 *			{
 *				TRACE1("  found = %s\n", ff->cFileName);
 *			} while ( ff.Next() );
 *		}
 *		</pre></td></tr></table>
 *
 *	@par使用例(iterator使用)
 *		<table><tr><td><pre>
 *		CFileFinder ff;
 *		if ( ff.Start(_T(".\\*.*")) )
 *		{
 *			for ( CFileFinder::iterator i = ff.begin(); i != ff.end(); i++ )
 *			{
 *				TRACE1("  found = %s\n", i->cFileName);
 *			}
 *		}
 *		</pre></td></tr></table>
 *	@note iterator 操作時、元の CFileFinder が使用されます。
 *
 *	@par必要ファイル
 *			TnbSftpFileFinder.h
 *
 *	@date 12/02/08 新規作成
 */
class CSftpFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// コンストラクタ
	CSftpFileFinder(void) : _super()
	{
	}

	/**
	 * [設定] 初期化
	 *	@note インスタンス作成後、検索の前に一度はこのメソッドを実行します。
	 *	@param pSftp オープンしたセッションハンドル。
	 */
	void Initialize(CSftpSession* pSftp)
	{
		Finish();
		m_pSftp = pSftp;
	}

	/// デストラクタ
	virtual ~CSftpFileFinder(void)
	{
		OnFinish();
	}

protected:

	/**
	 * [通知] 検索開始通知
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@param[in] lpszName 検索パス指定。ワイルドカードによる指定が必要です。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnStart(WIN32_FIND_DATA& _data, LPCTSTR lpszName)
	{
		CStr s = lpszName;
		s.Replace(_T('\\'), _T('/'));
		s.Replace(_T("//"), _T("/"));
		Finish();
		if ( m_pSftp != NULL )
		{
			if ( m_pSftp->EnumRemoteFiles(m_list, s) )
			{
				m_pos = 0;
				return OnNext(_data);
			}
		}
		return false;
	}

	/**
	 * [通知] 次検索通知
	 *	@note Find() で true の場合、次にこのメソッドを使用します。その後繰り返して
	 *			本メソッド使うことで複数の情報を得ることが出来ます。
	 *	@param[out] _data 発見した場合、発見情報が格納されます。
	 *	@retval true 一つ以上発見。
	 *	@retval false 未発見。
	 */
	virtual bool OnNext(WIN32_FIND_DATA& _data)
	{
		if ( m_list.GetSize() > m_pos )
		{
			_data = m_list[m_pos];
			m_pos++;
			return true;
		}
		return false;
	}

	/**
	 * [通知] 検索終了通知
	 *	@note 検索中の場合、検索を停止します（ OnNext() メソッドは false を返すようになります）。
	 */
	virtual void OnFinish(void)
	{
		m_list.Resize(0);
		m_pos = 0;
	}

private:
	CSftpSession*					m_pSftp;		///< SFTP セッション
	CWorkMemT<WIN32_FIND_DATA>		m_list;
	INDEX							m_pos;
	friend class CSftpFileFinderTest;
};



}; // TNB


/*
	WinSCP のコマンドリファレンス
	http://sourceforge.jp/projects/winscp/wiki/script_commands
  キャッシュキーの場所
	HKEY_CURRENT_USER\Software\Martin Prikryl\WinSCP 2\SshHostKeys
  */
