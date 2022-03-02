#pragma once
/**
 *@file
 * コンソールフッカーヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include <Wincon.h>
#ifdef __TNBPROCESSHANDLE_H__
 #ifndef _TnbPROCESS_PipeInOut_ENABLE
  #error Please define _TnbPROCESS_PipeInOut_ENABLE first rather than you include TnbProcessHandle.h. 
 #endif
 #ifndef _TnbPROCESS_EnumProcesses_ENABLE
  #error Please define _TnbPROCESS_EnumProcesses_ENABLE first rather than you include TnbProcessHandle.h. 
 #endif
#endif

#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #define _TnbPROCESS_PipeInOut_ENABLE
 #define _TnbPROCESS_EnumProcesses_ENABLE
#endif
#include "TnbProcessHandle.h"
#include "TnbStrVector.h"
#include "TnbStrOperator.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * コンソールフッカークラス
 *
 *		コンソールアプリケーションをフックし、コマンドラインの入出力を本クラスでコントロールします。
 *
 *	@par必要ファイル
 *			TnbConsoleHooker.h
 * 
 *	@date 12/02/02 新規作成
 */
class CConsoleHooker : CCopyImpossible
{
public:

	/// コンストラクタ
	CConsoleHooker(void) : m_processId(0), m_isHooked(false), m_hStdOutput(NULL), m_hStdInput(NULL)
	{
		m_rep.SetKind(_T("ConsoleHooker"));
	}

	/// デストラクタ
	~CConsoleHooker(void)
	{
		Unhook();
	}

	/**
	 * [確認] フック済み？
	 *	@retval true フック済み。 Execute() 使用時も「フック済み」になる。
	 *	@retval false 否。
	 */
	bool IsHooked(void) const
	{
		return m_isHooked || m_proc.IsAlive();
	}

	/**
	 * [確認] プロセス生存確認.
	 *		フックしているプロセスが生きているか確認できます。
	 *	@retval true 生きている。
	 *	@retval false 否
	 */
	bool IsAlived(void) const
	{
		return (m_processId != 0 && ::GetProcessVersion(m_processId) != 0);
	}

	/**
	 * [取得] プロセスID取得.
	 *		管理しているプロセスIDを返します。
	 *	@return プロセスID
	 */
	DWORD GetProcessId(void) const
	{
		return m_processId;
	}

	/**
	 * [設定] フック.
	 *	@param processId プロセスID
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Hook(DWORD processId)
	{
		m_processName.Format(_T("%d"), processId);
		return m_Hook(processId);
	}

	/**
	 * [設定] フック.
	 *	@note 既に実行されているコンソールアプリをフックします。
	 *	@param lpszProcessName プロセス名（exe名）
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Hook(LPCTSTR lpszProcessName)
	{
		m_processName = lpszProcessName;
		DWORD id = 0;
		CWorkMemT<DWORD> ids;
		CTickCount tc;
		do
		{
			if ( CProcessHandle::EnumProcesses(ids, lpszProcessName) )
			{
				if ( ids.GetSize() > 0 )
				{
					id = ids[0];
					break;
				}
			}
		}
		while ( ! tc.IsPassed(2000) );
		return m_Hook(id);
	}

	/**
	 * [設定] 実行.
	 *	@note 新規にコンソールアプリ実行しフックします。
	 *	@param lpszProcessName プロセス名（exe名）
	 *	@param isShow false なら非表示で起動。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Execute(LPCTSTR lpszProcessName, bool isShow = false)
	{		
		Unhook();
		m_pipeOut.Open();
		m_pipeIn.Open();
		if ( m_proc.Create(lpszProcessName, &m_pipeIn, &m_pipeOut, NULL, isShow) )
		{
			m_processId = m_proc.GetProcessId();
			CStr s = lpszProcessName;
			s = m_processName.FindCut(' ');
			if ( s.IsEmpty() )
			{
				s = lpszProcessName;
			}
			int r = ToInt(s.ReverseFindOneOf(_T("\\/")));
			m_processName = (r < 0) ? s : s.Mid(r + 1);
			return true;
		}
		return false;
	}

	/**
	 * [設定] ターミネート.
	 *		Execute() で実行したアプリを終了します。
	 */
	void Terminate(void)
	{
		Unhook();
		m_proc.Terminate();
	}

	/**
	 * [設定] アンフック.
	 *		Hook() を解除します。
	 */
	void Unhook(void)
	{
		m_pipeIn.Close();
		m_pipeOut.Close();
		if ( m_isHooked )
		{
			::FreeConsole();
			m_hStdOutput = NULL;
			m_hStdInput = NULL;
			m_isHooked = false;
		}
	}

	/**
	 * [処理] 終了待ち.
	 *		フックしているコンソールプロセスの終了待ち
	 *	@param to タイムアウト(ms)
	 *	@retval true 終了している or 時間内に終了した
	 *	@retval false 終了しない。
	 */
	bool WaitForExit(DWORD to)
	{
		CTickCount tc;
		do
		{
			if ( ::GetProcessVersion(m_processId) == 0 )
			{
				return true;
			}
			::Sleep(1);
		}
		while ( ! tc.IsPassed(to) );
		return false;
	}

	/**
	 * [送信] 文字列送信.
	 *	@note 「Enter」は '￥r' を使います。
	 *	@param lpsz 文字列
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SendString(LPCTSTR lpsz)
	{
		if ( m_isHooked )
		{
			CStr s = lpsz;
			size_t len = s.GetLength();
			loop ( i, len )
			{
				if ( ! m_SendChar(s[i]) )
				{
					return false;
				}
			}
			return true;
		}
		CAscii a = lpsz;
		a.Replace('\r', '\n');
		LPCSTR P = a;
		size_t l = a.GetLength();
		return (m_pipeIn.Send(l, P) == l);
	}

	/**
	 * [取得] コンソール文字列取得.
	 *	@return 文字列
	 */
	CStrVector GetString(void)
	{
		CStrVector vs;
		if ( m_isHooked )
		{
			m_WaitIdle();
			CONSOLE_SCREEN_BUFFER_INFO sb;
			if ( ::GetConsoleScreenBufferInfo(m_hStdOutput, &sb) )
			{
				size_t len = sb.dwCursorPosition.Y * sb.dwSize.X + sb.dwCursorPosition.X;
				//
				COORD coord = { 0, 0 };
				size_t sz = len;
				CWorkMemT<TCHAR> buf(sb.dwSize.X + 1);
				DWORD dw;
				while ( sz > 0 )
				{
					buf.Fill(0);
					size_t z = min(sb.dwSize.X, ToInt(sz));
					::ReadConsoleOutputCharacter(m_hStdOutput, buf, ToDword(z), coord, &dw);
					coord.Y++;
					sz -= z;
					CStr s = buf.Ref();
					s.TrimRight(' ');
					vs.Add(s);
					TRACE2("%s [%s]\n", m_processName, s);
					m_rep.ToViewer(CStr::Fmt(_T("%s [%s]\n"), m_processName, s));
				}
				coord.Y = 0;
				::FillConsoleOutputCharacter(m_hStdOutput, ' ', ToDword(len), coord, &dw);
				::SetConsoleCursorPosition(m_hStdOutput, coord);
			}
		}
		else
		{
			size_t l = m_pipeOut.GetReceivingDataSize();
			if ( l > 0 && l != INVALID_SIZE )
			{
				CWorkMemT<char> work(l + 1);
				work.Fill(0); //終端
				m_pipeOut.Receive(l, work);
				CAsciiVector va = CAsciiOperator::SeparateLine(work);
				loop ( i, va )
				{
					vs.Add(CStr(va[i]));
					TRACE2("%s [%s]\n", m_processName, CStr(va[i]));
					m_rep.ToViewer(CStr::Fmt(_T("%s [%s]\n"), m_processName, CStr(va[i])));
				}
			}
		}
		return vs;
	}

	/**
	 * [取得] 指定文字列待ち.
	 *	@note 本メソッド中に取得した文字列は GetLastString() で得られます。
	 *	@param lpsz 待ち文字列
	 *	@param to タイムアウト(ms)
	 *	@retval true 待ち文字列が来た
	 *	@retval false タイムアウト.exe終了
	 */
	bool WaitString(LPCTSTR lpsz, DWORD to)
	{
		m_lastStrings.RemoveAll();
		CStr a = lpsz;
		CTickCount tc;
		do
		{
			if ( ::GetProcessVersion(m_processId) == 0 )
			{
				return false;
			}
			CStrVector vs = GetString();
			m_lastStrings.Append(vs);
			loop ( i, vs )
			{
				if ( vs[i].Find(a) != INVALID_INDEX )
				{
					return true;
				}
			}
			::Sleep(1);
		}
		while ( ! tc.IsPassed(to) );
		return false;
	}

	/**
	 * [取得] 指定文字列待ち.
	 *	@note 本メソッド中に取得した文字列は GetLastString() で得られます。
	 *	@param words 待ち文字列群
	 *	@param to タイムアウト(ms)
	 *	@retval 0以上 待ち文字列が来た。数値は検出した words のインデックス。
	 *	@retval -1 タイムアウト
	 *	@retval -2 exe終了
	 */
	int WaitStringEx(const CStrVector& words, DWORD to)
	{
		m_lastStrings.RemoveAll();
		CTickCount tc;
		do
		{
			if ( ::GetProcessVersion(m_processId) == 0 )
			{
				return -2;
			}
			CStrVector vs = GetString();
			m_lastStrings.Append(vs);
			loop ( j, words )
			{
				loop ( i, vs )
				{
					if ( vs[i].Find(words[j]) != INVALID_INDEX )
					{
						return ToInt(j);
					}
				}
			}
			::Sleep(1);
		}
		while ( ! tc.IsPassed(to) );
		return -1;
	}

	/**
	 * [取得] 指定文字列待ち.
	 *	@note 本メソッド中に取得した文字列は GetLastString() で得られます。
	 *	@param lpszWord1 待ち文字列１
	 *	@param lpszWord2 待ち文字列２
	 *	@param to タイムアウト(ms)
	 *	@retval 0 待ち文字列１が来た。
	 *	@retval 1 待ち文字列２が来た。
	 *	@retval -1 タイムアウト
	 *	@retval -2 exe終了
	 */
	int WaitStringEx(LPCTSTR lpszWord1, LPCTSTR lpszWord2, DWORD to)
	{
		CStrVector vs;
		vs.Add(lpszWord1);
		vs.Add(lpszWord2);
		return WaitStringEx(vs, to);
	}

	/**
	 * [取得] 取得文字列取得.
	 *		WaitString() , WaitStringEx() 中に取得した文字列を返します。
	 *	@return 文字列
	 */
	const CStrVector& GetLastString(void) const
	{
		return m_lastStrings;
	}

	/**
	 * [設定] 取得文字列設定.
	 *		GetLastString() で得られる文字列を設定します。
	 *	@param vs 文字列
	 */
	void SetLastString(const CStrVector& vs)
	{
		m_lastStrings = vs;
	}

private:
	/// フック
	bool m_Hook(DWORD processId)
	{
		Unhook();
		if ( ::AttachConsole(processId) )
		{
			m_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
			m_hStdOutput = reinterpret_cast<HANDLE>(7);
			m_hStdInput = ::GetStdHandle(STD_INPUT_HANDLE);
			if ( m_hStdOutput != NULL && m_hStdInput != NULL )
			{
				m_processId = processId;
				m_isHooked = true;
				return true;
			}
		}
		_GetLastError("AttachConsole");
		return false;
	}
	/// アイドル待ち
	bool m_WaitIdle(DWORD to = 500)
	{
		ASSERT( m_isHooked );
		CONSOLE_SCREEN_BUFFER_INFO sb1;
		CONSOLE_SCREEN_BUFFER_INFO sb;
		if ( ! ::GetConsoleScreenBufferInfo(m_hStdOutput, &sb1) )
		{
			_GetLastError("GetConsoleScreenBufferInfo");
			return false;
		}
		while ( true )
		{
			CTickCount tc;
			bool isIdle = true;
			do
			{
				::Sleep(100);
				if ( ! ::GetConsoleScreenBufferInfo(m_hStdOutput, &sb) )
				{
					return false;
				}
				if ( sb1.dwCursorPosition.X != sb.dwCursorPosition.X || sb1.dwCursorPosition.Y != sb.dwCursorPosition.Y )
				{
					isIdle = false;
					break;
				}
			}
			while ( ! tc.IsPassed(to) );
			if ( isIdle )
			{
				return true;
			}
			sb1 = sb;
		}
		return false;
	}
	/// 一文字送信
	bool m_SendChar(TCHAR c)
	{
		ASSERT( m_isHooked );
		INPUT_RECORD ir[2] = { 0 };
		ir[0].EventType = KEY_EVENT;
		#ifndef _UNICODE
			ir[0].Event.KeyEvent.uChar.AsciiChar = c;
		#else
			ir[0].Event.KeyEvent.uChar.UnicodeChar = c;
		#endif
		ir[1] = ir[0];
		ir[0].Event.KeyEvent.bKeyDown = true;
		DWORD dw;
		if ( ::WriteConsoleInput(m_hStdInput, ir, 2, &dw) )
		{
			return dw == 2;
		}
		return false;
	}
	CStr			m_processName;	///< プロセス名
	DWORD			m_processId;	///< プロセスID
	bool			m_isHooked;		///< フック済み
	HANDLE			m_hStdOutput;	///< STDOUT
	HANDLE			m_hStdInput;	///< STDIN
	CPipe			m_pipeIn;		///< IN PIPE
	CPipe			m_pipeOut;		///< OUT PIPE
	CProcessHandle	m_proc;			///< プロセス管理
	CStrVector		m_lastStrings;	///< 取得最終文字列群
	CReportViewerEx	m_rep;			///< レポータ
};



}; // TNBLIB
