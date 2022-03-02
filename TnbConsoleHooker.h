#pragma once
/**
 *@file
 * �R���\�[���t�b�J�[�w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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

#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
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
 * �R���\�[���t�b�J�[�N���X
 *
 *		�R���\�[���A�v���P�[�V�������t�b�N���A�R�}���h���C���̓��o�͂�{�N���X�ŃR���g���[�����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbConsoleHooker.h
 * 
 *	@date 12/02/02 �V�K�쐬
 */
class CConsoleHooker : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CConsoleHooker(void) : m_processId(0), m_isHooked(false), m_hStdOutput(NULL), m_hStdInput(NULL)
	{
		m_rep.SetKind(_T("ConsoleHooker"));
	}

	/// �f�X�g���N�^
	~CConsoleHooker(void)
	{
		Unhook();
	}

	/**
	 * [�m�F] �t�b�N�ς݁H
	 *	@retval true �t�b�N�ς݁B Execute() �g�p�����u�t�b�N�ς݁v�ɂȂ�B
	 *	@retval false �ہB
	 */
	bool IsHooked(void) const
	{
		return m_isHooked || m_proc.IsAlive();
	}

	/**
	 * [�m�F] �v���Z�X�����m�F.
	 *		�t�b�N���Ă���v���Z�X�������Ă��邩�m�F�ł��܂��B
	 *	@retval true �����Ă���B
	 *	@retval false ��
	 */
	bool IsAlived(void) const
	{
		return (m_processId != 0 && ::GetProcessVersion(m_processId) != 0);
	}

	/**
	 * [�擾] �v���Z�XID�擾.
	 *		�Ǘ����Ă���v���Z�XID��Ԃ��܂��B
	 *	@return �v���Z�XID
	 */
	DWORD GetProcessId(void) const
	{
		return m_processId;
	}

	/**
	 * [�ݒ�] �t�b�N.
	 *	@param processId �v���Z�XID
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool Hook(DWORD processId)
	{
		m_processName.Format(_T("%d"), processId);
		return m_Hook(processId);
	}

	/**
	 * [�ݒ�] �t�b�N.
	 *	@note ���Ɏ��s����Ă���R���\�[���A�v�����t�b�N���܂��B
	 *	@param lpszProcessName �v���Z�X���iexe���j
	 *	@retval true ����.
	 *	@retval false ���s.
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
	 * [�ݒ�] ���s.
	 *	@note �V�K�ɃR���\�[���A�v�����s���t�b�N���܂��B
	 *	@param lpszProcessName �v���Z�X���iexe���j
	 *	@param isShow false �Ȃ��\���ŋN���B
	 *	@retval true ����.
	 *	@retval false ���s.
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
	 * [�ݒ�] �^�[�~�l�[�g.
	 *		Execute() �Ŏ��s�����A�v�����I�����܂��B
	 */
	void Terminate(void)
	{
		Unhook();
		m_proc.Terminate();
	}

	/**
	 * [�ݒ�] �A���t�b�N.
	 *		Hook() ���������܂��B
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
	 * [����] �I���҂�.
	 *		�t�b�N���Ă���R���\�[���v���Z�X�̏I���҂�
	 *	@param to �^�C���A�E�g(ms)
	 *	@retval true �I�����Ă��� or ���ԓ��ɏI������
	 *	@retval false �I�����Ȃ��B
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
	 * [���M] �����񑗐M.
	 *	@note �uEnter�v�� '��r' ���g���܂��B
	 *	@param lpsz ������
	 *	@retval true ����
	 *	@retval false ���s
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
	 * [�擾] �R���\�[��������擾.
	 *	@return ������
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
				work.Fill(0); //�I�[
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
	 * [�擾] �w�蕶����҂�.
	 *	@note �{���\�b�h���Ɏ擾����������� GetLastString() �œ����܂��B
	 *	@param lpsz �҂�������
	 *	@param to �^�C���A�E�g(ms)
	 *	@retval true �҂������񂪗���
	 *	@retval false �^�C���A�E�g.exe�I��
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
	 * [�擾] �w�蕶����҂�.
	 *	@note �{���\�b�h���Ɏ擾����������� GetLastString() �œ����܂��B
	 *	@param words �҂�������Q
	 *	@param to �^�C���A�E�g(ms)
	 *	@retval 0�ȏ� �҂������񂪗����B���l�͌��o���� words �̃C���f�b�N�X�B
	 *	@retval -1 �^�C���A�E�g
	 *	@retval -2 exe�I��
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
	 * [�擾] �w�蕶����҂�.
	 *	@note �{���\�b�h���Ɏ擾����������� GetLastString() �œ����܂��B
	 *	@param lpszWord1 �҂�������P
	 *	@param lpszWord2 �҂�������Q
	 *	@param to �^�C���A�E�g(ms)
	 *	@retval 0 �҂�������P�������B
	 *	@retval 1 �҂�������Q�������B
	 *	@retval -1 �^�C���A�E�g
	 *	@retval -2 exe�I��
	 */
	int WaitStringEx(LPCTSTR lpszWord1, LPCTSTR lpszWord2, DWORD to)
	{
		CStrVector vs;
		vs.Add(lpszWord1);
		vs.Add(lpszWord2);
		return WaitStringEx(vs, to);
	}

	/**
	 * [�擾] �擾������擾.
	 *		WaitString() , WaitStringEx() ���Ɏ擾�����������Ԃ��܂��B
	 *	@return ������
	 */
	const CStrVector& GetLastString(void) const
	{
		return m_lastStrings;
	}

	/**
	 * [�ݒ�] �擾������ݒ�.
	 *		GetLastString() �œ����镶�����ݒ肵�܂��B
	 *	@param vs ������
	 */
	void SetLastString(const CStrVector& vs)
	{
		m_lastStrings = vs;
	}

private:
	/// �t�b�N
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
	/// �A�C�h���҂�
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
	/// �ꕶ�����M
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
	CStr			m_processName;	///< �v���Z�X��
	DWORD			m_processId;	///< �v���Z�XID
	bool			m_isHooked;		///< �t�b�N�ς�
	HANDLE			m_hStdOutput;	///< STDOUT
	HANDLE			m_hStdInput;	///< STDIN
	CPipe			m_pipeIn;		///< IN PIPE
	CPipe			m_pipeOut;		///< OUT PIPE
	CProcessHandle	m_proc;			///< �v���Z�X�Ǘ�
	CStrVector		m_lastStrings;	///< �擾�ŏI������Q
	CReportViewerEx	m_rep;			///< ���|�[�^
};



}; // TNBLIB
