#pragma once
/**
 *@file
 * SSH FTP �֌W�w�b�_
 *
 *	@note WinSCP ���K�v�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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
 * SSH FTP�Z�b�V�����Ǘ��N���X
 *
 *		�T�[�o�ڑ��ƃt�@�C���̓]�����Ǘ����܂��B�܂��A�f���[�g�A���l�[�����T�|�[�g���܂��B
 *
 *	@note WinSCP ���C���X�g�[������Ă��� PC �ł̂ݓ��삵�܂��B����͉p�ꂩ���{��̂ݑΉ����Ă��܂��B
 *		WinSCP �̓f�t�H���g�̏ꏊ�ɃC���X�g�[�����Ă��邩�A�p�X���ʂ��Ă���K�v������܂��B
 *
 *	@attention WinSCP 4.36 / 4.37 ��p�ł��B
 *
 *	@see CFtpSession
 *
 *	@par�K�v�t�@�C��
 *			TnbSftpFile.h
 *
 *	@date 12/02/07 �V�K�쐬
 *	@date 12/02/07 OpenSSH �� WinSCP �ɕύX
 *	@date 12/05/21 WinSCP 4.37 �ɂ��Ή��B
 */
class CSftpSession : CCopyImpossible
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@param lpszWinScpPath WinSCP �̃p�X�B�ȗ������ ��Program Files�⁏WinSCP �ɂȂ�܂��B
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

	/// �f�X�g���N�^
	~CSftpSession(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �I�[�v��.
	 *		�{�N���X�̃��\�b�h���g�p����ɂ́A�܂��I�[�v������K�v������܂��B
	 *	@param lpszServerName	�ڑ���̃z�X�g���܂��̓h�b�g�\�L��IP�A�h���X�B
	 *	@param lpszUserName		�ڑ��̍ۂɎg�p����郆�[�U���B
	 *	@param lpszPassword		�ڑ��̍ۂɎg�p�����p�X���[�h�B
	 *	@param to �^�C���A�E�g����(ms)�B�ȗ������ 10�b
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
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
								% _T("Session started.")		% _T("�Z�b�V�������J�n���܂���")	// 1
								% _T("Authentication failed") 	% _T("�F�؂Ɏ��s")					// 2
								% _T("to the cache?")			% _T("�z�X�g�����L���b�V���ɒǉ�")	// 3
								% _T("(U)pdate")				% _T("�X�V((U))");					// 4
					cl.Format(_T("open %s:%s@%s\r"), lpszUserName, lpszPassword, lpszServerName);
					m_sftp.SendString(cl);
					int r = m_sftp.WaitStringEx(vs, to);
					r = (r >= 0) ? (r / 2) : r;
					if ( r == 4/*�X�V*/ )
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
						// �ڑ�����
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
	 * [�ݒ�] �N���[�Y.
	 *	@note �I�[�v�����Ă��Ȃ����͉������܂���B
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
	 * [�m�F] �n���h���L���m�F
	 *	@retval ture �L���B
	 *	@retval false ����(�J���Ă��Ȃ����A���s������)�B
	 */
	bool IsValid(void) const
	{
		return m_sftp.IsAlived();
	}

	/**
	 * [�쐬] �����[�g�f�B���N�g���쐬.
	 *	@param lpszRemoteDir	�쐬���郊���[�g�f�B���N�g�����B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool CreateRemoteDir(LPCTSTR lpszRemoteDir)
	{
		CStr cl;
		cl.Format(_T("mkdir %s\r"), ms_DQ(lpszRemoteDir));
		return SftpCommand(cl);
	}

	/**
	 * [�폜] �����[�g�f�B���N�g���폜.
	 *	@param lpszRemoteDir	�폜���郊���[�g�f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool RemoveRemoteDir(LPCTSTR lpszRemoteDir)
	{
		CStr cl;
		cl.Format(_T("rmdir %s\r"), ms_DQ(lpszRemoteDir));
		return SftpCommand(cl);
	}

	/**
	 * [�擾] �J�����g�����[�g�f�B���N�g���擾.
	 *		FTP�Z�b�V�������ł̃J�����g�f�B���N�g�����擾����B 
	 *	@param[out]		_currentRemoteDir	�J�����g�f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
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
	 * [�ݒ�] �J�����g�����[�g�f�B���N�g���ݒ�.
	 *	@param lpszRemoveDir	�ύX��̃f�B���N�g�����B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool SetCurrentRemoteDir(LPCTSTR lpszRemoveDir)
	{
		CStr cl;
		cl.Format(_T("cd %s\r"), ms_DQ(lpszRemoveDir));
		return SftpCommand(cl);
	}

	/**
	 * [�擾] �����[�g�t�@�C���擾.
	 *	@param lpszNewLocalFile	�擾��̃��[�J���t�@�C�����B
	 *	@param lpszRemoteFile	�擾���̃����[�g�t�@�C�����B
	 *	@param to �^�C���A�E�g����(ms)�B�ȗ������ 10�b
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
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
	 * [���M] ���[�J���t�@�C�����M.
	 *	@param lpszNewRemoteFile	�]����̃����[�g�t�@�C�����B
	 *	@param lpszLocalFile	�]�����̃��[�J���t�@�C�����B
	 *	@param to �^�C���A�E�g����(ms)�B�ȗ������ 10�b
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
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

	/// �������[�h
	enum ESyncMode
	{
		SyncMode_Local,		///< �����[�g���烍�[�J���ւ̓���
		SyncMode_Remote,	///< ���[�J�����烊���[�g�ւ̓���
		SyncMode_Both,		///< ����
	};

	/// �����
	enum ESyncCriteria
	{
		SyncCriteria_Time,	///< ����
		SyncCriteria_Size,	///< �t�@�C���T�C�Y
		SyncCriteria_Both,	///< ����
		SyncCriteria_Non,	///< �Ȃ�
	};

	/**
	 * [����] ���[�J���ƃ����[�g�̓���.
	 *	@param lpszRemoteDir �����[�g�f�B���N�g�����B
	 *	@param lpszLocalDir ���[�J���f�B���N�g�����B
	 *	@param mode �������[�h�B
	 *	@param criteria ������B
	 *	@param to �^�C���A�E�g����(ms)�B�ȗ������ 10�b
	 *	@param lpszOption �I�v�V�����B���Ƃ��΁A�h-delete�h�ȂǁB�ȗ��\�B
	 *	@retval true �����B���������t�@�C�����A���́A GetSynchronizedFiles() �œ����܂��B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
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
	 * [�擾] �������ʃt�@�C�����擾.
	 *		Synchronize() �̌��ʁA�������ꂽ�t�@�C�����A�f�B���N�g�������i�[����܂��B
	 *	@note �p�X���������ꍇ�A�ȗ�����邱�Ƃ�����܂��B
	 *	@return �t�@�C�����B
	 */
	const CStrVector& GetSynchronizedFiles(void) const
	{
		return m_synchronizedFiles;
	}

	/**
	 * [�폜] �w�胊���[�g�t�@�C���폜.
	 *	@param lpszFileName	�폜���郊���[�g�t�@�C�����B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool DeleteRemoteFile(LPCTSTR lpszFileName)
	{
		CStr cl;
		cl.Format(_T("rm %s\r"), ms_DQ(lpszFileName));
		return SftpCommand(cl);
	}

	/**
	 * [�ύX] ���O�ύX.
	 *		�����[�g�t�@�C�����A�����[�g�f�B���N�g�����̕ύX���o���܂��B
	 *	@param lpszOldName	���̃����[�g�t�@�C�����B
	 *	@param lpszNewName	�ύX��̃����[�g�t�@�C�����B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool RenameRemoteFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
	{
		CStr cl;
		cl.Format(_T("rename %s %s\r") , ms_DQ(lpszOldName), ms_DQ(lpszNewName));
		return SftpCommand(cl);
	}

	/**
	 * [�擾] �t�@�C���ꗗ.
	 *	@param[out] _ls	�t�@�C���ꗗ���ʊi�[�B
	 *	@param[in] lpszRemoteDir	�����[�g�f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
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
					CStrVector mvs = CStrOperator::SeparatePeriod(vs[5], ':', false); //�����b
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
	 * [�m�F] �t�@�C���L��.
	 *	@param[out] _fd	�t�@�C�����i�[�B
	 *	@param[in] lpszRemoteFile	�����[�g�t�@�C��/�f�B���N�g���B���C���h�J�[�h�͎g�p�֎~�B
	 *	@retval true ����B
	 *	@retval false ���� or ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool ExistRemoteFile(WIN32_FIND_DATA& _fd, LPCTSTR lpszRemoteFile)
	{
		CStr rf = lpszRemoteFile;
		rf += _T("*"); //���C���h�ɂ���
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
	 * [�m�F] �t�@�C���L��.
	 *	@param lpszRemoteFile	�����[�g�t�@�C��/�f�B���N�g���B���C���h�J�[�h�͎g�p�֎~�B
	 *	@retval true ����B
	 *	@retval false ���� or ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool ExistRemoteFile(LPCTSTR lpszRemoteFile)
	{
		WIN32_FIND_DATA fd;
		return ExistRemoteFile(fd, lpszRemoteFile);
	}

	/**
	 * [�擾] �擾������擾.
	 *		�e�֐��̎��s���A����ꂽ�������Ԃ��܂��B
	 *	@return ������
	 */
	const CStrVector& GetLastString(void) const
	{
		return m_sftp.GetLastString();
	}

	/**
	 * [�ݒ�] �擾������ݒ�.
	 *		GetLastString() �œ����镶�����ݒ肵�܂��B
	 *	@param vs ������
	 */
	void SetLastString(const CStrVector& vs)
	{
		m_sftp.SetLastString(vs);
	}

	/**
	 * [�ݒ�] �����I�� WinSCP.
	 *		�v���O�������ُ�I�������ہA WinScp ���c���Ă��܂��P�[�X������܂��B
	 *		����� winscp �������I�����郁�\�b�h�ł��B
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
	 * [���s] SFTP �R�}���h���C�����s.
	 *	@param lpszCommandLine �R�}���h���C��
	 *	@param to �^�C���A�E�g
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
	 */
	bool SftpCommand(LPCTSTR lpszCommandLine, DWORD to = 1000)
	{
		CStr cl = lpszCommandLine;
		cl.Trim(_T(' '));
		if ( m_sftp.SendString(cl) )
		{
			CStrVector vs = CStr(_T("(A)bort,")) % _T("���~((A))") % _T("No session.") % _T("�Z�b�V�����Ȃ�") % _T("winscp>");
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
	 * [���s] �J�����g���[�J���f�B���N�g������.
	 *		WinSCP �Ɩ{�v���Z�X�̃J�����g�f�B���N�g���𓯊����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� GetLastString() �œ����܂��B
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
	/// �J�����g���[�J���f�B���N�g���擾
	static CStr ms_GetCurrentDir(void)
	{
		CStr str;
		DWORD dwRc = ::GetCurrentDirectory(MAX_PATH, str.GetBuffer(MAX_PATH));
		str.ReleaseBuffer();
		ASSERTLIB( dwRc != 0 );
		IgnoreUnusedValue (dwRc);
		return str;	
	}
	/// �_�u���N�I�[�g������
	static CStr ms_DQ(LPCTSTR lpsz)
	{
		CStr s = lpsz;
		if ( ! s.IsEmpty() )
		{
			s = s.Sandwich(_T("\""), _T("\""));
		}
		return s;
	}
	/// �������񁨐��l
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
	CProcessHandle	m_exe;					///< WinSCP.com �Ǘ�
	CConsoleHooker	m_sftp;					///< SFTP �R�}���h�Ǘ�
	CStr			m_currentLocalDir;		///< �J�����g���[�J���f�B���N�g��
	CStr			m_winScpPath;			///< WinSCP�p�X
	CStrVector		 m_synchronizedFiles;	///< �������ʃt�@�C��
	friend class CSftpSessionTest;
};



/**@ingroup FILE
 * SFTP �t�@�C�������N���X
 *
 *		�w��̃f�B���N�g������t�@�C�����������邽�߂̃N���X�ł��B
 *		
 *	@note �������A�f�B���N�g�����A[.][..]�́A���O����܂��B
 *
 *	@par�g�p��
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
 *	@par�g�p��(iterator�g�p)
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
 *	@note iterator ���쎞�A���� CFileFinder ���g�p����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSftpFileFinder.h
 *
 *	@date 12/02/08 �V�K�쐬
 */
class CSftpFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// �R���X�g���N�^
	CSftpFileFinder(void) : _super()
	{
	}

	/**
	 * [�ݒ�] ������
	 *	@note �C���X�^���X�쐬��A�����̑O�Ɉ�x�͂��̃��\�b�h�����s���܂��B
	 *	@param pSftp �I�[�v�������Z�b�V�����n���h���B
	 */
	void Initialize(CSftpSession* pSftp)
	{
		Finish();
		m_pSftp = pSftp;
	}

	/// �f�X�g���N�^
	virtual ~CSftpFileFinder(void)
	{
		OnFinish();
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
	 * [�ʒm] �������ʒm
	 *	@note Find() �� true �̏ꍇ�A���ɂ��̃��\�b�h���g�p���܂��B���̌�J��Ԃ���
	 *			�{���\�b�h�g�����Ƃŕ����̏��𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _data ���������ꍇ�A������񂪊i�[����܂��B
	 *	@retval true ��ȏ㔭���B
	 *	@retval false �������B
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
	 * [�ʒm] �����I���ʒm
	 *	@note �������̏ꍇ�A�������~���܂��i OnNext() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	virtual void OnFinish(void)
	{
		m_list.Resize(0);
		m_pos = 0;
	}

private:
	CSftpSession*					m_pSftp;		///< SFTP �Z�b�V����
	CWorkMemT<WIN32_FIND_DATA>		m_list;
	INDEX							m_pos;
	friend class CSftpFileFinderTest;
};



}; // TNB


/*
	WinSCP �̃R�}���h���t�@�����X
	http://sourceforge.jp/projects/winscp/wiki/script_commands
  �L���b�V���L�[�̏ꏊ
	HKEY_CURRENT_USER\Software\Martin Prikryl\WinSCP 2\SshHostKeys
  */
