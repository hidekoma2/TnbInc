#pragma once
/**
 *@file
 * ���O�t�@�C���쐬�֌W�̃w�b�_.
 *
 *		���O�t�@�C���쐬�֌W�̃N���X�A�}�N������`����Ă��܂��B		
 *
 *	@note �}�N�� "_TnbLOG_DISABLE" ����`����Ă���ƁA TLOG �֌W�̍s���R�����g�����ɂȂ�܂��B
 *
 *	@note �}�N�� "_TnbLOG_MULTIFILE" ����`����Ă���ƁA���O�t�@�C�����w��̐������ޔ�����悤�ɂȂ�܂��B
 *
 *	@note �}�N�� "_TnbTraceLogger_LogFileName" �� CStr foo(INDEX) �^�̊֐������`���Ă����ƁAlog�o�͐��ύX�ł��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 *	
 *	@see LOG
 */

/**@defgroup LOG			���O�����֌W
 *
 *	EXE �Ɠ����t�H���_�� ��.exe �� ��.t.log�i�f�o�b�O�ł̏ꍇ�A��.pc��.t.log�j �ɕς����t�@�C�����O���o�͂��܂��B
 *
 *	@note �}�N�� "_TnbLOG_DISABLE" ����`����Ă���ƁA TLOG �֌W�̍s���R�����g�����ɂȂ�܂��B
 *
 *	@note �}�N�� "_TnbLOG_MULTIFILE" ����`����Ă���ƁA���O�t�@�C�����w��̐������ޔ�����悤�ɂȂ�܂��B
 *		\code
 *			ex) foo.exe �̏ꍇ
 *				foo.t.log �� �ŐV
 *				foo.t1.log �� ��O
 *				foo.t2.log �� ���̑O
 *					�F
 *				�c�Ƃ�������B
 *		\endcode
 *
 *	@par ��`�}�N����
 *		\code
 *		//�� ���O�o�͍��ڂ��w�肵�܂��B �X�y�[�X��؂�ŕ����w�肵�܂��B
 *		//		trace  �g���[�X���O���o�͂��܂��B�idef.�j
 *		//		poll   �|�[�����O���O���o�͂��܂��B
 *		//		func   �֐����O���o�͂��܂��B
 *		//		off    ���O���o���܂���B��������Ȃ��ƁA�G���[���O�͂�J�n/�I�����O�͋L�^�����
 *		#define _TnbTraceLogger_Types "trace func"
 *		//�� ���O�t�@�C���̍ő�T�C�Y�� KB�P�ʂŎw�肵�܂��B
 *		#define _TnbTraceLogger_LogSize 128
 *		//�� ���O�t�@�C���̑ޔ𐔂��w�肵�܂��B
 *		#define _TnbTraceLogger_FileCount 5
 *		//�� ���Ԃ̕\�L�^���w�肵�܂��B
 *		//		off    �o�͂��Ȃ�
 *		//		normal YY/MM/DD-HH:MM:SS.SSS (def.)
 *		//		full   YY/MM/DD-HH:MM:SS.SSS(TTTTTTTT)
 *		//		tick   TTTTTTTT
 *		//			�� TTTTTTTT �� ����LOG����� ms
 *		#define _TnbTraceLogger_Time "full"
 *		//�� �X���b�hID�̗L�����w�肵�܂��B
 *		//		off    �o�͂��܂���B
 *		//		on     �o�͂��܂��B
 *		#define _TnbTraceLogger_Thread "off"
 *		//�� �_���v�\���̗L�����w�肵�܂��B
 *		//		0      �o�͂��܂���B
 *		//		1      �ŏ���16Byte�܂ł��o�͂��܂��B (def.)
 *		//		2      �w��S�T�C�Y���o�͂��܂��B
 *		#define _TnbTraceLogger_Dump 2
 *		\endcode
 *
 *	@par ��`�t�@�C���uTnbLogger.ini�v��
 *		\code
 *		[Trace]
 *		Types=
 *		;�� ���O�o�͍��ڂ��w�肵�܂��B �X�y�[�X��؂�ŕ����w�肵�܂��B
 *		; trace  �g���[�X���O���o�͂��܂��B�idef.�j
 *		; poll   �|�[�����O���O���o�͂��܂��B
 *		; func   �֐����O���o�͂��܂��B
 *		; off    ���O���o���܂���B��������Ȃ��ƁA�G���[���O�͊J�n/�I�����O�͋L�^�����
 *		
 *		LogSize=
 *		;�� ���O�t�@�C���̍ő�T�C�Y�� KB�P�ʂŎw�肵�܂��B
 *		; �ȗ�����ƋK��l�ł��B(def.64KB)
 *		
 *		FileCount=
 *		;�� ���O�t�@�C���̑ޔ𐔂��w�肵�܂��B (_TnbLOG_MULTIFILE ���錾����Ă���ꍇ�̂�)
 *		; �ȗ�����ƋK��l�ł��B(def.5)
 *		
 *		Time=
 *		;�� ���Ԃ̕\�L�^���w�肵�܂��B
 *		; off    �o�͂��Ȃ�
 *		; normal YY/MM/DD-HH:MM:SS.SSS (def.)
 *		; full   YY/MM/DD-HH:MM:SS.SSS(TTTTTTTT)
 *		; tick   TTTTTTTT
 *		;			* TTTTTTTT �� ����LOG����� ms
 *		
 *		Thread=
 *		;�� �X���b�hID�̗L�����w�肵�܂��B
 *		; off     �o�͂��܂���B
 *		; on      �o�͂��܂��B (def.)
 *		
 *		Dump=
 *		;�� �_���v�\���̗L�����w�肵�܂��B
 *		; 0      �o�͂��܂���B
 *		; 1      �ŏ���16Byte�܂ł��o�͂��܂��B (def.)
 *		; 2      �w��S�T�C�Y���o�͂��܂��B
 *		\endcode
 *		��def.�͒l���Ȃ��ꍇ�ɑI������܂��B�������A�R���p�C�����Ɉȉ��̃}�N���ŋK���ύX���邱�Ƃ��o���܂��B
 */



#include "TnbDump.h"
#include "TnbInifileAccessor.h"
#include "TnbFile.h"
#include "TnbAsyncWriter.h"
#include "TnbSingleton.h"
#include "TnbStrEx.h"



//TNB Library
namespace TNB
{



#ifndef _TnbLOG_DISABLE


#ifndef _TnbLOG_MULTIFILE
 #pragma message("message : TLOG�n�͗L���ɂȂ��Ă��܂�(SingleFileType)�B")
#else
 #pragma message("message : TLOG�n�͗L���ɂȂ��Ă��܂�(MultiFileType)�B")
#endif



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��



#ifndef _TnbTraceLogger_Types
 #define _TnbTraceLogger_Types "trace"
#endif

#ifndef _TnbTraceLogger_LogSize
 #define _TnbTraceLogger_LogSize 64
#endif

#ifndef _TnbTraceLogger_FileCount
 #define _TnbTraceLogger_FileCount 5
#endif

#ifndef _TnbTraceLogger_Time
 #define _TnbTraceLogger_Time "normal"
#endif

#ifndef _TnbTraceLogger_Thread
 #define _TnbTraceLogger_Thread "on"
#endif

#ifndef _TnbTraceLogger_Dump
 #define _TnbTraceLogger_Dump 1
#endif

#ifndef _TnbTraceLogger_LogFileName
 #define _TnbTraceLogger_LogFileName ms_MakeLogfilePath
#endif

// ���\�L
#ifdef _TnbTraceLogger_FileMaxSize
 #undef _TnbTraceLogger_LogSize
 #define _TnbTraceLogger_LogSize _TnbTraceLogger_FileMaxSize
#endif

#ifdef _TnbLOG_OUTPUT_FUNC
 #undef _TnbTraceLogger_Types
 #define _TnbTraceLogger_Types "trace func"
#endif


/**@ingroup LOG
 * ���O�t�@�C���쐬�N���X
 * 
 *	@attention �{�N���X�͒��ڎg�킸�ɁA��`����Ă���}�N�����g�p���Ă��������B
 *
 *	@see TLOG �}�N��
 *
 *		���s�o�߂��t�@�C���ɏ����o���N���X�ł��B
 *
 *	@date 05/01/01	�V�K�쐬
 *	@date 06/08/02	TNBLIB��
 *	@date 12/11/21	�}���`�t�@�C���^�C�v�A�V�K�ǉ�
 *	@date 14/03/04	ThreadId�̗L���X�C�b�`�ǉ��B�t�@�C���p�X�����[�U�[��`�\�ɁB
 *					�V���O���t�@�C���^�C�v�̎��A�܂�Ԃ����ɃS�~���N���A����悤�ɂ����B
 */
class CTraceLogger
{
public:

	/// �g���[�X�^�C�v
	enum EType
	{
		Type_Absolute	= _BIT(0),	///< ���
		Type_Error		= _BIT(1),	///< �G���[�֌W
		Type_Trace		= _BIT(2),	///< �g���[�X���b�Z�[�W�֌W
		Type_Polling	= _BIT(3),	///< �|�[�����O�m�F�֌W 
		Type_Function	= _BIT(4),	///< �֐��̏o����
	};

	/// �X���b�hID�\�L
	enum EThread
	{
		Thread_Off = 0,			///< �o�͂��Ȃ�
		Thread_On,				///< �o�͂���
	};

	/// ���ԕ\�L
	enum ETime
	{
		Time_Off = 0,			///< �o�͂��Ȃ�
		Time_Normal,			///< YY/MM/DD-HH:MM:SS.SSS
		Time_Full,				///< YY/MM/DD-HH:MM:SS.SSS(TTTTTTTT)
		Time_Tick,				///< TTTTTTTT
	};

	/**@ingroup LOG
	 * ���O�t�@�C���쐬�⏕�N���X
	 * 
	 *		�i�J���p�j���s�o�߂��t�@�C���ɏ����o���N���X�ł��B
	 *
	 *	@note ���ڎg�킸�A�}�N�����g�p���܂��B
	 *
	 *	@see TFUNC �}�N��
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTraceLogger.h
	 *
	 *	@date 06/08/02	�V�K�쐬
	 */
	class CFuncer
	{
	public:
		/**
		 * �R���X�g���N�^
		 *	@param lpszFunctionName �֐���+�p�����[�^�\���p�t�H�[�}�b�g
		 *	@param ... �p�����^
		 */ 
		CFuncer(LPCTSTR lpszFunctionName, ...)
		{
			CStr s;
			va_list args;	
			va_start(args, lpszFunctionName);
			s.FormatV(lpszFunctionName, args);
			m_functionName = s.FindCut('(');
			if ( s.Find('(') == INVALID_INDEX )
			{
				s += _T("()");
			}
			CTraceLogger::Write(CTraceLogger::Type_Function, CStr::Fmt(_T(">In[%s]"), s));
		}
		/**
		 * [�\��] �p�����[�^�\��.
		 *		�R���X�g���N�^�ŕ\���d�؂�Ȃ��ꍇ�A���̃��\�b�h���g���A�ǋL���邱�Ƃ��o���܂��B
		 *	@param lpszFormatText �p�����[�^�\���p�t�H�[�}�b�g
		 *	@param ... �p�����^
		 */ 
		void Param(LPCTSTR lpszFormatText, ...)
		{
			#ifndef _WIN32_WCE
			ASSERT0(! m_functionName.IsEmpty(),
					CAscii(m_functionName), "���U���g�\����Ƀp�����[�^��\�����悤�Ƃ��Ă��܂��B");
			#endif
			CStr s;
			va_list args;	va_start(args, lpszFormatText);
			s.FormatV(lpszFormatText, args);
			CTraceLogger::Write(CTraceLogger::Type_Function, _T(">InParam ") + s);
		}
		/**
		 * [�\��] ���U���g�\��.
		 *		�{���\�b�h���g���ƁA�f�X�g���N�g���ɕ\���������e�Ƀ��U���g��ǋL���邱�Ƃ��\�ł��B
		 *	@param lpszFormatText ���U���g�\���p�t�H�[�}�b�g
		 *	@param ... �p�����^
		 */ 
		void Result(LPCTSTR lpszFormatText, ...)
		{
			#ifndef _WIN32_WCE
			ASSERT0(! m_functionName.IsEmpty(),
					CAscii(m_functionName), "���U���g����ȏ�\�����悤�Ƃ��Ă��܂��B");
			#endif
			CStr s;
			va_list args;	va_start(args, lpszFormatText);
			s.FormatV(lpszFormatText, args);
			m_resultText.Format(_T(">Out[%s()] %s"), m_functionName, s);
			m_functionName.Empty();
		}
		/// �f�X�g���N�^
		~CFuncer(void)
		{
			if ( ! m_resultText.IsEmpty() )
			{
				CTraceLogger::Write(CTraceLogger::Type_Function, m_resultText);
			}
			else if ( ! m_functionName.IsEmpty() )
			{
				CStr s;
				s.Format(_T(">Out[%s()]"), m_functionName);
				CTraceLogger::Write(CTraceLogger::Type_Function, s);
			}
			m_functionName.Empty();
		}
		/// �֐����擾
		LPCTSTR GetFunctionName(void) const
		{
			return m_functionName;
		}

	private:
		CStr m_functionName;	///< �֐���
		CStr m_resultText;		///< ���ʕ�����
	};


	//-----------------


	///�f�X�g���N�^
	virtual ~CTraceLogger(void)
	{
		m_Stop();
	}

	/**
	 * [�ݒ�] LOG�L���ݒ�.
	 *		�ꎞ�I�ɂƂ߂邱�Ƃ��\�ł��B
	 *	@param isEnable true�Ȃ珑���o����܂��Bfalse�Ȃ珑���o���܂���(�̂Ă��܂�)�B
	 */
	static void EnableOutput(bool isEnable)
	{
		CTraceLogger* P = GetInstance();
		P->m_PauseMode(! isEnable);
		if ( ! isEnable )
		{
			P->m_Stop();
		}
	}
	
	/**
	 * [����] �������݈˗�.
	 *	@param type ���O�^�C�v
	 *	@param lpszFormatText ���O���e
	 *	@param va �p�����^
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool Write(EType type, LPCTSTR lpszFormatText, va_list va)
	{
		CTraceLogger* P = GetInstance();
		if ( ! P->m_CanLogout(type) ){ return true; }
		CStr s;
		s.FormatV(lpszFormatText, va);
		return P->m_PostTraceInfo(type, CAscii(s), NULL);
	}

	/**
	 * [����] �������݈˗�.
	 *	@param type ���O�^�C�v
	 *	@param lpszFormatText ���O���e
	 *	@param ... �p�����^
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool Write(EType type, LPCTSTR lpszFormatText, ...)
	{
		va_list args;	va_start(args, lpszFormatText);
		return Write(type, lpszFormatText, args);
	}

	/**
	 * [����] �������݈˗�.
	 *	@param type ���O�^�C�v
	 *	@param vb ���O���e�iDump������e�j
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool Write(EType type, const CByteVector &vb)
	{
		bool boRc = true;
		CTraceLogger* P = GetInstance();
		if ( ! P->m_CanLogout(type) ){ return true; }
		if ( P->m_outputDumpLevel > 0 )
		{
			boRc = P->m_PostTraceInfo(type, NULL, &vb);
		}
		return boRc;
	}

	/**
	 * [����] �������݈˗�.
	 *	@param type ���O�^�C�v
	 *	@param vb ���O���e�iDump������e�j
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool Write(EType type, size_t size, LPCVOID P)
	{
		bool r;
		if ( ! ::IsBadReadPtr(P, size) )
		{
			CByteVector b;
			b.SetElements(size, static_cast<const BYTE*>(P));
			r = Write(type, b);
		}
		else
		{
			r = Write(type, _T("�A�N�Z�X�ł��Ȃ��̈��Dump���悤���܂����B"));
		}
		return r;
	}

private:

	/// �V���O���g���R���X�g���N�^
	SINGLETON_CONSTRUCTOR(CTraceLogger)
	{
		m_outputTypes		= 0;
		m_maxLogSize		= 0;
		m_maxMultiFileCount = 0;
		m_outputThread		= Thread_On;
		m_outputTime		= Time_Normal;
		m_outputDumpLevel	= 1;
		m_offset			= 0;
		m_isInitLastTick	= false;
		m_isPause			= false;
	}

	/**
	 * [�쐬] ini�t�@�C���p�X�쐬.
	 *		���s�t�@�C������ '��.exe' �� '��.t.log' �� '��.t1.log' �Ȃǂɕς������̂��쐬���܂��B
	 *	@param index �C���f�b�N�X�B 0 �Ȃ琔���Ȃ��B�P�ȏ�Ȃ琔�l������܂��B
	 *	@return ini�t�@�C����FullPath
	 */
	static CStr ms_MakeLogfilePath(INDEX index = 0)
	{
		CStr str;
		DWORD dwRc = ::GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);
		str.ReleaseBuffer();
		ASSERTLIB(dwRc != 0);
		//
		CStr strWork;
		strWork = str.Left(str.ReverseFind('.'));
		#ifdef _DEBUG
			strWork += _T(".");
			strWork += TNB::GetComputerName();
		#endif
		if ( index == 0 )
		{
			strWork += _T(".t.log");
		}
		else
		{
			strWork += CStr::Fmt(_T(".t%d.log"), index);
		}
		return strWork;
	}

	/**
	 * ���O�A�E�g�ł��邩�m�F
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	bool m_CanLogout(EType type)
	{
		EXCLUSIVE(&m_sync);
		if ( m_isPause )
		{
			return false;
		}
		if ( ! m_file.IsAlive() )
		{
			if ( ! m_Start() )
			{
				return false; 
			}
		}
		return (m_outputTypes & type) != 0;
	}

	/// ��~
	void m_Stop()
	{
		if ( m_file.IsAlive() )
		{
			m_PostTraceInfo(Type_Absolute, "----------<<�I��>>----------", NULL);
		}
		m_file.Stop();
		m_fw.Close();
	}

	/**
	 * �J�n
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_Start(void)
	{
		m_Stop();
		CInifileAccessor ini(TNB::GetProcessPath() + _T("\\TnbLogger.ini"));
		CInifileAccessor::CSection sec = ini[_T("Trace")];
		m_outputTypes = Type_Trace | Type_Error | Type_Absolute;
		{
			CStr s = sec.QueryString(_T("Types"));
			if ( s.IsEmpty() )
			{
				s = _T(_TnbTraceLogger_Types);
			}
			if ( s.Find(_T("err")) >= 0 ){
				m_outputTypes = Type_Error | Type_Absolute;
			}
			if ( s.Find(_T("trace")) >= 0 ){
				m_outputTypes |= Type_Trace;
			}
			if ( s.Find(_T("poll")) >= 0 ){
				m_outputTypes |= Type_Polling;
			}
			if ( s.Find(_T("func")) >= 0 ){
				m_outputTypes |= Type_Function;
			}
			if ( s.Find(_T("off")) >= 0 ){
				m_outputTypes = 0;
			}
		}
		if ( m_outputTypes == 0 )
		{
			return true;
		}
		m_maxLogSize = sec.QueryDword(_T("LogSize"), _TnbTraceLogger_LogSize);
		if ( m_maxLogSize < 1 )
		{
			m_maxLogSize = 1;
		}
		m_maxMultiFileCount = sec.QueryDword(_T("FileCount"), _TnbTraceLogger_FileCount);
		if ( m_maxMultiFileCount < 2 )
		{
			m_maxMultiFileCount = 2;
		}
		m_maxLogSize *= 1024;
		m_outputTime = Time_Normal;
		{
			CStr s = sec.QueryString(_T("Time"));
			if ( s.IsEmpty() )
			{
				s = _T(_TnbTraceLogger_Time);
			}
			if ( s.Find(_T("off")) >= 0 ){
				m_outputTime = Time_Off;
			}
			else if ( s.Find(_T("normal")) >= 0 ){
				m_outputTime = Time_Normal;
			}
			else if ( s.Find(_T("full")) >= 0 ){
				m_outputTime = Time_Full;
			}
			else if ( s.Find(_T("tick")) >= 0 ){
				m_outputTime = Time_Tick;
			}
		}
		m_outputThread = Thread_On;
		{
			CStr s = sec.QueryString(_T("Thread"));
			if ( s.IsEmpty() )
			{
				s = _T(_TnbTraceLogger_Thread);
			}
			if ( s.Find(_T("off")) >= 0 ){
				m_outputThread = Thread_Off;
			}
			else if ( s.Find(_T("on")) >= 0 ){
				m_outputThread = Thread_On;
			}
		}
		m_outputDumpLevel = sec.QueryDword(_T("Dump"), _TnbTraceLogger_Dump);
		//
		if ( ! m_fw.Open(_TnbTraceLogger_LogFileName(0), true, true) )
		{
			return false;
		}
		m_offset = static_cast<INDEX>(m_fw.GetSize());
		#ifndef _TnbLOG_MULTIFILE
			if ( m_offset == 0 )
			{
				//�V�K�����	
				m_offset = HEADSIZE + 2;
				if ( ! m_WritePos(m_fw, m_offset) )
				{
					return false;
				}
			}
			else
			{
				CByteVector vb;
				try
				{
					m_fw.Seek(0);
					vb = m_fw.ReadExactly(HEADSIZE);
				}
				catch(CTnbException& e)
				{
					e.OnCatch();
					return false;
				}
				catch(...)
				{
					ASSERTLIB(false);
					return false;
				}
				vb.Add(0);
				m_offset = atoi(reinterpret_cast<const char*>(vb.ReferBuffer()));
				m_fw.Seek(m_offset);
			}
		#endif
		if ( ! m_file.SetWriter(&m_fw) )
		{
			return false;
		}
		if ( ! m_file.Start() )
		{
			return false;
		}
		m_file.SetPriority(THREAD_PRIORITY_LOWEST/*2��*/);
		m_PostTraceInfo(Type_Absolute, "----------<<�N��>>----------", NULL);
		return true;
	}

	/**
	 * �|�W�V����������
	 *	@param [in,out] _fw Writer�B
	 *	@param [in] dwPos �|�W�V����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_WritePos(IWriter& _fw, INDEX dwPos)
	{
		if ( ! _fw.CanWrite() )
		{
			return false;
		}
		DWORD dw = ToDword(dwPos);
		char acFmt[HEADSIZE * 2];
		char acBuf[HEADSIZE * 2];
		sprintf(acFmt, "%%0%dd\r\n", HEADSIZE);
		sprintf(acBuf, acFmt, dw);
		_fw.Seek(0);
		try
		{
			_fw.Write(HEADSIZE + 2, acBuf);
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			return false;
		}
		catch(...)
		{
			ASSERTLIB(false);
			return false;
		}
		return true;
	}

	/**
	 * ��s������
	 *	@param [in,out] _fw Writer�B
	 *	@param [in] lpszLine �������ݕ�����(ASCII)
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_WriteLine(IWriter& _fw, LPCSTR lpszLine)
	{
		ASSERT( &_fw == &m_fw );
		if ( ! m_fw.CanWrite() )
		{
			return false;
		}
		//
		#ifdef _DEBUG
			::OutputDebugString(CStr(lpszLine).Sandwich(_T("TLOG:["), _T("]\n")));
		#endif
		#ifndef _TnbLOG_MULTIFILE
			LPCSTR lpszRc = "\r\n@@\r\n";
			m_fw.Seek(m_offset);
		#else
			LPCSTR lpszRc = "\r\n";
		#endif
		size_t dwTextSize = STRLIB::GetLen(lpszLine);
		try
		{
			m_fw.Write(dwTextSize, lpszLine);
			m_fw.Write(STRLIB::GetLen(lpszRc), lpszRc);
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			return false;
		}
		catch(...)
		{
			ASSERTLIB(false);
			return false;
		}
		m_offset += dwTextSize + 2;
		#ifndef _TnbLOG_MULTIFILE
			if ( m_offset > m_maxLogSize )
			{
				m_offset = HEADSIZE + 2;
				m_fw.SetEnd();
			}
			return m_WritePos(m_fw, m_offset);
		#else
			if ( m_offset > m_maxLogSize )
			{
				// ���l�[�����邼
				EXCLUSIVE(&m_sync);
				m_fw.Close();
				::DeleteFile(_TnbTraceLogger_LogFileName(m_maxMultiFileCount)); //�Ȃ��ꍇ������
				loop_dn( i, m_maxMultiFileCount)
				{
					CStr s1 = _TnbTraceLogger_LogFileName(i + 1);
					CStr s2 = _TnbTraceLogger_LogFileName(i);
					::MoveFile(s2, s1);
				}
				if ( ! m_fw.New(_TnbTraceLogger_LogFileName(0), true) )
				{
					return false;
				}
				m_offset = 0;
			}
			return true;
		#endif
	}

	/**
	 * �����ݏ����R�}���h�N���X
	 */
	class CParamWriteCmd : public CAsyncWriter::ICommand
	{
		CTraceLogger*	m_pLogger;		///< CTraceLogger�̎Q��
	public:
		EType		type;				///< �^�C�v
		bool		hasText;			///< true�Ȃ�e�L�X�g
		CAscii		strText;			///< �e�L�X�g
		CByteVector	vbBinary;			///< �o�C�i��
		SYSTEMTIME	tSysTime;			///< �V�X�e������
		DWORD		dwTick;				///< Tick
		DWORD		dwThreadId;			///< ThreadID

		/// �R���X�g���N�^
		CParamWriteCmd(CTraceLogger* P)
		{
			m_pLogger = P;
		}

		/** 
		 * [����] ����
		 *	@param fw �I�[�v�����Ă���Writer���n����܂��B
		 *	@return false�Ȃ�X���b�h�I�������܂��B
		 */
		virtual bool Exec(IWriter& fw)
		{
			if ( ! m_pLogger->m_isInitLastTick )
			{
				m_pLogger->m_isInitLastTick = true;
				m_pLogger->m_lastTick = dwTick;
			}
			DWORD dwTickDiff = dwTick - m_pLogger->m_lastTick;
			CAscii strThread;
			if ( m_pLogger->m_outputThread != Thread_Off )
			{
				strThread.Format("[%08X] ", dwThreadId);
			}
			CAscii strLine;
			switch ( m_pLogger->m_outputTime )
			{
			case Time_Off:	//�o�͂��Ȃ�
				break;
			case Time_Normal:	//YY/MM/DD-HH:MM:SS.SSS
			default:
				#ifndef _WIN32_WCE
					strLine.Format("%02d/%02d/%02d-%02d:%02d:%02d.%03d",
							tSysTime.wYear%100,
							tSysTime.wMonth,
							tSysTime.wDay,
							tSysTime.wHour,
							tSysTime.wMinute,
							tSysTime.wSecond,
							tSysTime.wMilliseconds
					);
				#else
					strLine.Format("%02d/%02d-%02d:%02d:%02d",
							tSysTime.wMonth,
							tSysTime.wDay,
							tSysTime.wHour,
							tSysTime.wMinute,
							tSysTime.wSecond
					);
				#endif
				break;
			case Time_Full:	//YY/MM/DD-HH:MM:SS.SSS(TTTTTTTT)
				strLine.Format("%02d/%02d/%02d-%02d:%02d:%02d.%03d(%8d)",
						tSysTime.wYear%100,
						tSysTime.wMonth,
						tSysTime.wDay,
						tSysTime.wHour,
						tSysTime.wMinute,
						tSysTime.wSecond,
						tSysTime.wMilliseconds,
						dwTickDiff
				);
				break;
			case Time_Tick:	//TTTTTTTT
				strLine.Format("(%8d)", dwTickDiff);
				break;
			}
			m_pLogger->m_lastTick = dwTick;
			//
			if ( type == Type_Error )
			{
				strLine += "*Err* ";
			}
			else
			{
				strLine += " ";
			}
			//
			if ( hasText )
			{
				strLine += strText;
				m_pLogger->m_WriteLine(fw, strThread + strLine);
			}
			else if ( (m_pLogger->m_outputDumpLevel) > 0 )
			{
				CAscii s;
				s.Format(" Bin(size=%d)", vbBinary.GetSize());
				strLine += s;
				m_pLogger->m_WriteLine(fw, strThread + strLine);
				//
				size_t l = vbBinary.GetSize();
				if ( (m_pLogger->m_outputDumpLevel) == 1 && l > 16 )
				{
					l = 16;
				}
				CStrVector vs = DumpData(l, vbBinary.ReferBuffer());
				loop ( i, vs.GetSize() )
				{
					m_pLogger->m_WriteLine(fw, CAscii(vs[i]));
				}
			}
			return true;
		}
	};

	/**
	 * EventLog���Post.
	 *		EventLog�������ݗp�̏����܂Ƃ߁A��p�X���b�h�ɑ΂�PostMessage���܂��B
	 *	@param type ���O�^�C�v
	 *	@param lpszText �e�L�X�g�f�[�^�BNULL�Ȃ�o�C�i���w��i pvb ��NULL�ȊO�j
	 *	@param pvb �o�C�i���f�[�^�BNULL�Ȃ�e�L�X�g�w��( lpszText��NULL�ȊO)
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_PostTraceInfo(EType type, LPCSTR lpszText, const CByteVector* pvb)
	{
		if ( m_isPause ) { return true; }
		//
		CParamWriteCmd* P = new CParamWriteCmd(this);
		P->dwTick		= ::GetTickCount();
		P->dwThreadId	= ::GetCurrentThreadId();
		P->type			= type;
		if ( lpszText != NULL )
		{
			P->hasText = true;
			P->strText = lpszText;
		}
		else if ( pvb != NULL )
		{
			P->hasText = false;
			P->vbBinary = *pvb;
		}
		else
		{
			delete P;	//�������J���B
			return false;
		}
		::GetLocalTime(&(P->tSysTime));
		return m_file.Command(P);
	}

	/// �ꎞ��~���[�h�ݒ�
	void m_PauseMode(bool isPause)
	{
		m_isPause = isPause;
	}

	enum { HEADSIZE = 12 };					//�w�b�_�̌���
	bool			m_isPause;				///< ���O�ꎞ��~
	DWORD			m_outputTypes;			///< ���ۂɃt�@�C�������o���^�C�v�X(INI)
	DWORD			m_maxLogSize;			///< ���O�̍ő�T�C�Y(INI)
	DWORD			m_maxMultiFileCount;	///< ���O�̍ő�ޔ��t�@�C����(INI)
	EThread			m_outputThread;			///< ThreadId�̕\��(INI)
	ETime			m_outputTime;			///< Tick�J�E���^�̕\��(INI)
	int				m_outputDumpLevel;		///< Dump�̕\�� 0;���Ȃ� 1;16byte�܂� 2;Full(INI)
	bool			m_isInitLastTick;		///< Tick�l�̏������t���O
	DWORD			m_lastTick;				///< Tick�l�T��
	CFileWriter		m_fw;					///< ������
	CAsyncWriter	m_file;					///< BG���C�^�[
	INDEX			m_offset;				///< �I�t�Z�b�g
	CSyncSection	m_sync;					///< �r��
	friend class CTraceLogger::CParamWriteCmd;
};


#define TFUNC					CTraceLogger::CFuncer _TnbFunc_
#define TFUNC_PARAM				_TnbFunc_.Param
#define TFUNC_RESULT			_TnbFunc_.Result



#endif // _TnbDOXYGEN



#ifdef _TnbDOXYGEN



/**@ingroup LOG
 * �֐����O.
 *	@note �֐��̓�����ɋL�q���܂��B���^�[�����ɂ��֐��������O����܂��B
 *		���O�o�̓^�C�v�� func ���܂܂�Ă���Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@par�g�p��
 *		\code
 *		int Foo(int iIndex, LPCTSTR lpsz)
 *		{
 *			TFUNC("Foo(%d,%s)", iIndex, lpsz);
 *			TFUNC_PARAM("iIndex = %d", iIndex);		//������ʂŏ������Ƃ��\�B
 *			int r;
 *			     ;
 *			TFUNC_RESULT("%d", r);
 *			return r;
 *		};
 *		\endcode
 *
 *	@par���O��
 *		\code
 *		[00000190] 06/08/02-15:02:23 >In[Foo(1,ABCDE)]		
 *		[00000190] 06/08/02-15:02:23 >InParam[iIndex=1]
 *		[00000190] 06/08/02-15:02:43 >Out[Foo()] 5
 *		\endcode
 *	@param lpszFormatText �����w�蕶����B printf() �����ł��B
 *	@param ... �ό����B 
 */
void TFUNC(LPCTSTR lpszFormatText, ...);

/**@ingroup LOG
 * �ǉ��������O.
 *		���O�o�̓^�C�v�� func ���܂܂�Ă���Ȃ�o�͂���܂��B
 *	@see TFUNC
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param lpszFormatText �����w�蕶����B printf() �����ł��B
 *	@param ... �ό����B 
 */
void TFUNC_PARAM(LPCTSTR lpszFormatText, ...);

/**@ingroup LOG
 * ���U���g���O.
 *		���O�o�̓^�C�v�� func ���܂܂�Ă���Ȃ�o�͂���܂��B
 *	@note �{�}�N�����g���ƁA�֐����^�[�����ɕ\���������e�Ƀ��U���g��ǋL���邱�Ƃ��\�ł��B
 *	@see TFUNC
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param lpszFormatText �����w�蕶����B printf() �����ł��B
 *	@param ... �ό����B 
 */
void TFUNC_RESULT(LPCTSTR lpszFormatText, ...);



#endif //_TnbDOXYGEN



/**@ingroup LOG
 * ��Ώo�̓��O.
 *		���O�o�̓^�C�v�� off �ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param lpszFormatText �����w�蕶����B printf() �����ł��B
 *	@param ... �ό����B
 */
inline void TLOG_AB(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Absolute, lpszFormatText, args);
}

/**@ingroup LOG
 * �G���[�o�̓��O.
 *		���O�o�̓^�C�v�� off �ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param lpszFormatText �����w�蕶����B printf() �����ł��B
 *	@param ... �ό����B 
 */
inline void TLOG_ERR(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Error, lpszFormatText, args);
}

/**@ingroup LOG
 * �G���[�o�̓_���v���O.
 *		���O�o�̓^�C�v�� off �ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param size �_���v�T�C�Y�B
 *	@param P �_���v�A�h���X�B
 */
inline void TLOG_ERRDUMP(size_t size, LPCVOID P)
{
	CTraceLogger::Write(CTraceLogger::Type_Error, size, P);
}

/**@ingroup LOG
 * �G���[�o�̓_���v���O.
 *		���O�o�̓^�C�v�� off �ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param vb �_���v���� CByteVector �I�u�W�F�N�g�B
 */
inline void TLOG_ERRDUMPV(const CByteVector& vb)
{
	CTraceLogger::Write(CTraceLogger::Type_Error, vb);
}

/**@ingroup LOG
 * �g���[�X�o�̓��O.
 *		���O�o�͂�Off�ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param lpszFormatText �����w�蕶����B printf() �����ł��B
 *	@param ... �ό����B 
 */
inline void TLOG(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Trace, lpszFormatText, args);
}

/**@ingroup LOG
 * �g���[�X�o�̓_���v���O.
 *		���O�o�̓^�C�v�� off err �ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param size �_���v�T�C�Y�B
 *	@param P �_���v�A�h���X�B
 */
inline void TLOG_DUMP(size_t size, LPCVOID P)
{
	CTraceLogger::Write(CTraceLogger::Type_Trace, size, P);
}

/**@ingroup LOG
 * �g���[�X�o�̓_���v���O.
 *		���O�o�̓^�C�v�� off err �ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param vb �_���v���� CByteVector �I�u�W�F�N�g�B
 */
inline void TLOG_DUMPV(const CByteVector& vb)
{
	CTraceLogger::Write(CTraceLogger::Type_Trace, vb);
}

/**@ingroup LOG
 * �|�[�����O�o�̓��O.
 *		���O�o�̓^�C�v�� poll ���܂܂�Ă�����o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param lpszFormatText �����w�蕶����B printf() �����ł��B
 *	@param ... �ό����B 
 */
inline void TLOG_POL(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Polling, lpszFormatText, args);
}

/**@ingroup LOG
 * LastError�o�̓��O.
 *		GetLastError() �̌��ʂ�0�ȊO�ŁA���O�o�͂�Off�ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@param lpszFunctionName �֐����B
 *	@return GetLastError() �̒l
 */
inline DWORD TLASTERROR(LPCTSTR lpszFunctionName)
{
	DWORD e = ::GetLastError();
	if ( e != ERROR_SUCCESS )
	{
		CSimpleStr s;
		SystemErrorToMessageText(s, e);
		LPCTSTR lpszErr = s;
		TLOG( _T("API[%s()]���s ErrorCode = 0x%08x(%s)"), lpszFunctionName, e, lpszErr);
	}
	::SetLastError(e);
	return e;
}

#ifdef _DEBUG
#ifdef _GetLastError
#undef _GetLastError
#define _GetLastError(XX) TLASTERROR(_T(XX))
#endif
#endif

/**@ingroup LOG
 * ���C���o�̓��O.
 *		�L�q�����\�[�X���ƍs�ԍ����o�͂���܂��B
 *		���O�o�̓^�C�v�� off �ȊO�Ȃ�o�͂���܂��B
 *	@par�K�v�t�@�C��
 *		TnbTraceLogger.h
 *	@note �J���p�ł��B�����[�X���A�c���Ȃ��悤�ɂ��܂��B
 */
#ifdef _DEBUG
	#define TLINE() CTraceLogger::Write(CTraceLogger::Type_Absolute, _T("** %s(%d):"), _T(__FILE__), __LINE__)
#else
	#define TLINE()
#endif



#else //_TnbLOG_DISABLE

	// TLOG�����p�錾
	#define TFUNC			__noop
	#define TFUNC_PARAM		__noop
	#define TFUNC_RESULT	__noop
	#define TLOG_POL		__noop
	#define TLOG			__noop
	#define TLOG_DUMP		__noop
	#define TLOG_DUMPV		__noop
	#define TLOG_ERR		__noop
	#define TLOG_ERRDUMP	__noop
	#define TLOG_ERRDUMPV	__noop
	#define TLOG_AB			__noop
	#define TLASTERROR		__noop
	#define TLINE()

#endif //_TnbLOG_DISABLE



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

// ���o�[�W�����ɑΉ�
#define TFUNC_(XX)				TFUNC XX
#define TFUNC_PARAM_(XX)		TFUNC_PARAM XX
#define TFUNC_RESULT_(XX)		TFUNC_RESULT XX
#define TLOG_AB_(XX)			TLOG_AB XX
#define TLOG_ERR_(XX)			TLOG_ERR XX
#define TLOG_(XX)				TLOG XX
#define TLOG_POL_(XX)			TPOL XX

#endif



}; // TNB

