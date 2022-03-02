#pragma once
/**
 *@file
 * ログファイル作成関係のヘッダ.
 *
 *		ログファイル作成関係のクラス、マクロが定義されています。		
 *
 *	@note マクロ "_TnbLOG_DISABLE" が定義されていると、 TLOG 関係の行がコメント扱いになります。
 *
 *	@note マクロ "_TnbLOG_MULTIFILE" が定義されていると、ログファイルを指定の数だけ退避するようになります。
 *
 *	@note マクロ "_TnbTraceLogger_LogFileName" に CStr foo(INDEX) 型の関数名を定義しておくと、log出力先を変更できます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 *	
 *	@see LOG
 */

/**@defgroup LOG			ログ処理関係
 *
 *	EXE と同じフォルダに ☆.exe を ☆.t.log（デバッグ版の場合、☆.pc名.t.log） に変えたファイルログを出力します。
 *
 *	@note マクロ "_TnbLOG_DISABLE" が定義されていると、 TLOG 関係の行がコメント扱いになります。
 *
 *	@note マクロ "_TnbLOG_MULTIFILE" が定義されていると、ログファイルを指定の数だけ退避するようになります。
 *		\code
 *			ex) foo.exe の場合
 *				foo.t.log ← 最新
 *				foo.t1.log ← 一つ前
 *				foo.t2.log ← その前
 *					：
 *				…といった具合。
 *		\endcode
 *
 *	@par 定義マクロ例
 *		\code
 *		//● ログ出力項目を指定します。 スペース区切りで複数指定します。
 *		//		trace  トレースログを出力します。（def.）
 *		//		poll   ポーリングログを出力します。
 *		//		func   関数ログを出力します。
 *		//		off    ログを出しません。これをつけないと、エラーログはや開始/終了ログは記録される
 *		#define _TnbTraceLogger_Types "trace func"
 *		//● ログファイルの最大サイズを KB単位で指定します。
 *		#define _TnbTraceLogger_LogSize 128
 *		//● ログファイルの退避数を指定します。
 *		#define _TnbTraceLogger_FileCount 5
 *		//● 時間の表記型を指定します。
 *		//		off    出力しない
 *		//		normal YY/MM/DD-HH:MM:SS.SSS (def.)
 *		//		full   YY/MM/DD-HH:MM:SS.SSS(TTTTTTTT)
 *		//		tick   TTTTTTTT
 *		//			※ TTTTTTTT は 一つ上のLOGからの ms
 *		#define _TnbTraceLogger_Time "full"
 *		//● スレッドIDの有無を指定します。
 *		//		off    出力しません。
 *		//		on     出力します。
 *		#define _TnbTraceLogger_Thread "off"
 *		//● ダンプ表示の有無を指定します。
 *		//		0      出力しません。
 *		//		1      最初の16Byteまでを出力します。 (def.)
 *		//		2      指定全サイズを出力します。
 *		#define _TnbTraceLogger_Dump 2
 *		\endcode
 *
 *	@par 定義ファイル「TnbLogger.ini」例
 *		\code
 *		[Trace]
 *		Types=
 *		;● ログ出力項目を指定します。 スペース区切りで複数指定します。
 *		; trace  トレースログを出力します。（def.）
 *		; poll   ポーリングログを出力します。
 *		; func   関数ログを出力します。
 *		; off    ログを出しません。これをつけないと、エラーログは開始/終了ログは記録される
 *		
 *		LogSize=
 *		;● ログファイルの最大サイズを KB単位で指定します。
 *		; 省略すると規定値です。(def.64KB)
 *		
 *		FileCount=
 *		;● ログファイルの退避数を指定します。 (_TnbLOG_MULTIFILE が宣言されている場合のみ)
 *		; 省略すると規定値です。(def.5)
 *		
 *		Time=
 *		;● 時間の表記型を指定します。
 *		; off    出力しない
 *		; normal YY/MM/DD-HH:MM:SS.SSS (def.)
 *		; full   YY/MM/DD-HH:MM:SS.SSS(TTTTTTTT)
 *		; tick   TTTTTTTT
 *		;			* TTTTTTTT は 一つ上のLOGからの ms
 *		
 *		Thread=
 *		;● スレッドIDの有無を指定します。
 *		; off     出力しません。
 *		; on      出力します。 (def.)
 *		
 *		Dump=
 *		;● ダンプ表示の有無を指定します。
 *		; 0      出力しません。
 *		; 1      最初の16Byteまでを出力します。 (def.)
 *		; 2      指定全サイズを出力します。
 *		\endcode
 *		※def.は値がない場合に選択されます。ただし、コンパイル時に以下のマクロで規定を変更することも出来ます。
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
 #pragma message("message : TLOG系は有効になっています(SingleFileType)。")
#else
 #pragma message("message : TLOG系は有効になっています(MultiFileType)。")
#endif



#ifndef _TnbDOXYGEN	//Document作成用シンボル



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

// 旧表記
#ifdef _TnbTraceLogger_FileMaxSize
 #undef _TnbTraceLogger_LogSize
 #define _TnbTraceLogger_LogSize _TnbTraceLogger_FileMaxSize
#endif

#ifdef _TnbLOG_OUTPUT_FUNC
 #undef _TnbTraceLogger_Types
 #define _TnbTraceLogger_Types "trace func"
#endif


/**@ingroup LOG
 * ログファイル作成クラス
 * 
 *	@attention 本クラスは直接使わずに、定義されているマクロを使用してください。
 *
 *	@see TLOG マクロ
 *
 *		実行経過をファイルに書き出すクラスです。
 *
 *	@date 05/01/01	新規作成
 *	@date 06/08/02	TNBLIB化
 *	@date 12/11/21	マルチファイルタイプ、新規追加
 *	@date 14/03/04	ThreadIdの有無スイッチ追加。ファイルパス名ユーザー定義可能に。
 *					シングルファイルタイプの時、折り返し時にゴミをクリアするようにした。
 */
class CTraceLogger
{
public:

	/// トレースタイプ
	enum EType
	{
		Type_Absolute	= _BIT(0),	///< 絶対
		Type_Error		= _BIT(1),	///< エラー関係
		Type_Trace		= _BIT(2),	///< トレースメッセージ関係
		Type_Polling	= _BIT(3),	///< ポーリング確認関係 
		Type_Function	= _BIT(4),	///< 関数の出入り
	};

	/// スレッドID表記
	enum EThread
	{
		Thread_Off = 0,			///< 出力しない
		Thread_On,				///< 出力する
	};

	/// 時間表記
	enum ETime
	{
		Time_Off = 0,			///< 出力しない
		Time_Normal,			///< YY/MM/DD-HH:MM:SS.SSS
		Time_Full,				///< YY/MM/DD-HH:MM:SS.SSS(TTTTTTTT)
		Time_Tick,				///< TTTTTTTT
	};

	/**@ingroup LOG
	 * ログファイル作成補助クラス
	 * 
	 *		（開発用）実行経過をファイルに書き出すクラスです。
	 *
	 *	@note 直接使わず、マクロを使用します。
	 *
	 *	@see TFUNC マクロ
	 *
	 *	@par必要ファイル
	 *			TnbTraceLogger.h
	 *
	 *	@date 06/08/02	新規作成
	 */
	class CFuncer
	{
	public:
		/**
		 * コンストラクタ
		 *	@param lpszFunctionName 関数名+パラメータ表示用フォーマット
		 *	@param ... パラメタ
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
		 * [表示] パラメータ表示.
		 *		コンストラクタで表示仕切れない場合、このメソッドを使い、追記することが出来ます。
		 *	@param lpszFormatText パラメータ表示用フォーマット
		 *	@param ... パラメタ
		 */ 
		void Param(LPCTSTR lpszFormatText, ...)
		{
			#ifndef _WIN32_WCE
			ASSERT0(! m_functionName.IsEmpty(),
					CAscii(m_functionName), "リザルト表示後にパラメータを表示しようとしています。");
			#endif
			CStr s;
			va_list args;	va_start(args, lpszFormatText);
			s.FormatV(lpszFormatText, args);
			CTraceLogger::Write(CTraceLogger::Type_Function, _T(">InParam ") + s);
		}
		/**
		 * [表示] リザルト表示.
		 *		本メソッドを使うと、デストラクト時に表示される内容にリザルトを追記することが可能です。
		 *	@param lpszFormatText リザルト表示用フォーマット
		 *	@param ... パラメタ
		 */ 
		void Result(LPCTSTR lpszFormatText, ...)
		{
			#ifndef _WIN32_WCE
			ASSERT0(! m_functionName.IsEmpty(),
					CAscii(m_functionName), "リザルトを二回以上表示しようとしています。");
			#endif
			CStr s;
			va_list args;	va_start(args, lpszFormatText);
			s.FormatV(lpszFormatText, args);
			m_resultText.Format(_T(">Out[%s()] %s"), m_functionName, s);
			m_functionName.Empty();
		}
		/// デストラクタ
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
		/// 関数名取得
		LPCTSTR GetFunctionName(void) const
		{
			return m_functionName;
		}

	private:
		CStr m_functionName;	///< 関数名
		CStr m_resultText;		///< 結果文字列
	};


	//-----------------


	///デストラクタ
	virtual ~CTraceLogger(void)
	{
		m_Stop();
	}

	/**
	 * [設定] LOG有効設定.
	 *		一時的にとめることが可能です。
	 *	@param isEnable trueなら書き出されます。falseなら書き出しません(捨てられます)。
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
	 * [処理] 書き込み依頼.
	 *	@param type ログタイプ
	 *	@param lpszFormatText ログ内容
	 *	@param va パラメタ
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [処理] 書き込み依頼.
	 *	@param type ログタイプ
	 *	@param lpszFormatText ログ内容
	 *	@param ... パラメタ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool Write(EType type, LPCTSTR lpszFormatText, ...)
	{
		va_list args;	va_start(args, lpszFormatText);
		return Write(type, lpszFormatText, args);
	}

	/**
	 * [処理] 書き込み依頼.
	 *	@param type ログタイプ
	 *	@param vb ログ内容（Dumpする内容）
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [処理] 書き込み依頼.
	 *	@param type ログタイプ
	 *	@param vb ログ内容（Dumpする内容）
	 *	@retval true 成功
	 *	@retval false 失敗
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
			r = Write(type, _T("アクセスできない領域をDumpしようしました。"));
		}
		return r;
	}

private:

	/// シングルトンコンストラクタ
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
	 * [作成] iniファイルパス作成.
	 *		実行ファイル名の '＊.exe' を '＊.t.log' や '＊.t1.log' などに変えたものを作成します。
	 *	@param index インデックス。 0 なら数字なし。１以上なら数値が入ります。
	 *	@return iniファイルのFullPath
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
	 * ログアウトできるか確認
	 *	@retval true 可能
	 *	@retval false 不可能
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

	/// 停止
	void m_Stop()
	{
		if ( m_file.IsAlive() )
		{
			m_PostTraceInfo(Type_Absolute, "----------<<終了>>----------", NULL);
		}
		m_file.Stop();
		m_fw.Close();
	}

	/**
	 * 開始
	 *	@retval true 成功
	 *	@retval false 失敗
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
				//新規じゃん	
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
		m_file.SetPriority(THREAD_PRIORITY_LOWEST/*2つ下*/);
		m_PostTraceInfo(Type_Absolute, "----------<<起動>>----------", NULL);
		return true;
	}

	/**
	 * ポジション書込み
	 *	@param [in,out] _fw Writer。
	 *	@param [in] dwPos ポジション
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * 一行書込み
	 *	@param [in,out] _fw Writer。
	 *	@param [in] lpszLine 書き込み文字列(ASCII)
	 *	@retval true 成功
	 *	@retval false 失敗
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
				// リネームするぞ
				EXCLUSIVE(&m_sync);
				m_fw.Close();
				::DeleteFile(_TnbTraceLogger_LogFileName(m_maxMultiFileCount)); //ない場合もある
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
	 * 書込み処理コマンドクラス
	 */
	class CParamWriteCmd : public CAsyncWriter::ICommand
	{
		CTraceLogger*	m_pLogger;		///< CTraceLoggerの参照
	public:
		EType		type;				///< タイプ
		bool		hasText;			///< trueならテキスト
		CAscii		strText;			///< テキスト
		CByteVector	vbBinary;			///< バイナリ
		SYSTEMTIME	tSysTime;			///< システム時間
		DWORD		dwTick;				///< Tick
		DWORD		dwThreadId;			///< ThreadID

		/// コンストラクタ
		CParamWriteCmd(CTraceLogger* P)
		{
			m_pLogger = P;
		}

		/** 
		 * [処理] 処理
		 *	@param fw オープンしてあるWriterが渡されます。
		 *	@return falseならスレッド終了させます。
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
			case Time_Off:	//出力しない
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
	 * EventLog情報Post.
	 *		EventLog書き込み用の情報をまとめ、専用スレッドに対しPostMessageします。
	 *	@param type ログタイプ
	 *	@param lpszText テキストデータ。NULLならバイナリ指定（ pvb はNULL以外）
	 *	@param pvb バイナリデータ。NULLならテキスト指定( lpszTextはNULL以外)
	 *	@retval true 成功
	 *	@retval false 失敗
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
			delete P;	//メモリ開放。
			return false;
		}
		::GetLocalTime(&(P->tSysTime));
		return m_file.Command(P);
	}

	/// 一時停止モード設定
	void m_PauseMode(bool isPause)
	{
		m_isPause = isPause;
	}

	enum { HEADSIZE = 12 };					//ヘッダの桁数
	bool			m_isPause;				///< ログ一時停止
	DWORD			m_outputTypes;			///< 実際にファイル書き出すタイプス(INI)
	DWORD			m_maxLogSize;			///< ログの最大サイズ(INI)
	DWORD			m_maxMultiFileCount;	///< ログの最大退避ファイル数(INI)
	EThread			m_outputThread;			///< ThreadIdの表示(INI)
	ETime			m_outputTime;			///< Tickカウンタの表示(INI)
	int				m_outputDumpLevel;		///< Dumpの表示 0;しない 1;16byteまで 2;Full(INI)
	bool			m_isInitLastTick;		///< Tick値の初期化フラグ
	DWORD			m_lastTick;				///< Tick値控え
	CFileWriter		m_fw;					///< 書込み
	CAsyncWriter	m_file;					///< BGライター
	INDEX			m_offset;				///< オフセット
	CSyncSection	m_sync;					///< 排他
	friend class CTraceLogger::CParamWriteCmd;
};


#define TFUNC					CTraceLogger::CFuncer _TnbFunc_
#define TFUNC_PARAM				_TnbFunc_.Param
#define TFUNC_RESULT			_TnbFunc_.Result



#endif // _TnbDOXYGEN



#ifdef _TnbDOXYGEN



/**@ingroup LOG
 * 関数ログ.
 *	@note 関数の入り口に記述します。リターン時にも関数名がログされます。
 *		ログ出力タイプに func が含まれているなら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@par使用例
 *		\code
 *		int Foo(int iIndex, LPCTSTR lpsz)
 *		{
 *			TFUNC("Foo(%d,%s)", iIndex, lpsz);
 *			TFUNC_PARAM("iIndex = %d", iIndex);		//引数を別で書くことも可能。
 *			int r;
 *			     ;
 *			TFUNC_RESULT("%d", r);
 *			return r;
 *		};
 *		\endcode
 *
 *	@parログ例
 *		\code
 *		[00000190] 06/08/02-15:02:23 >In[Foo(1,ABCDE)]		
 *		[00000190] 06/08/02-15:02:23 >InParam[iIndex=1]
 *		[00000190] 06/08/02-15:02:43 >Out[Foo()] 5
 *		\endcode
 *	@param lpszFormatText 書式指定文字列。 printf() 準拠です。
 *	@param ... 可変個引数。 
 */
void TFUNC(LPCTSTR lpszFormatText, ...);

/**@ingroup LOG
 * 追加引数ログ.
 *		ログ出力タイプに func が含まれているなら出力されます。
 *	@see TFUNC
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param lpszFormatText 書式指定文字列。 printf() 準拠です。
 *	@param ... 可変個引数。 
 */
void TFUNC_PARAM(LPCTSTR lpszFormatText, ...);

/**@ingroup LOG
 * リザルトログ.
 *		ログ出力タイプに func が含まれているなら出力されます。
 *	@note 本マクロを使うと、関数リターン時に表示される内容にリザルトを追記することが可能です。
 *	@see TFUNC
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param lpszFormatText 書式指定文字列。 printf() 準拠です。
 *	@param ... 可変個引数。 
 */
void TFUNC_RESULT(LPCTSTR lpszFormatText, ...);



#endif //_TnbDOXYGEN



/**@ingroup LOG
 * 絶対出力ログ.
 *		ログ出力タイプが off 以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param lpszFormatText 書式指定文字列。 printf() 準拠です。
 *	@param ... 可変個引数。
 */
inline void TLOG_AB(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Absolute, lpszFormatText, args);
}

/**@ingroup LOG
 * エラー出力ログ.
 *		ログ出力タイプが off 以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param lpszFormatText 書式指定文字列。 printf() 準拠です。
 *	@param ... 可変個引数。 
 */
inline void TLOG_ERR(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Error, lpszFormatText, args);
}

/**@ingroup LOG
 * エラー出力ダンプログ.
 *		ログ出力タイプが off 以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param size ダンプサイズ。
 *	@param P ダンプアドレス。
 */
inline void TLOG_ERRDUMP(size_t size, LPCVOID P)
{
	CTraceLogger::Write(CTraceLogger::Type_Error, size, P);
}

/**@ingroup LOG
 * エラー出力ダンプログ.
 *		ログ出力タイプが off 以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param vb ダンプする CByteVector オブジェクト。
 */
inline void TLOG_ERRDUMPV(const CByteVector& vb)
{
	CTraceLogger::Write(CTraceLogger::Type_Error, vb);
}

/**@ingroup LOG
 * トレース出力ログ.
 *		ログ出力がOff以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param lpszFormatText 書式指定文字列。 printf() 準拠です。
 *	@param ... 可変個引数。 
 */
inline void TLOG(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Trace, lpszFormatText, args);
}

/**@ingroup LOG
 * トレース出力ダンプログ.
 *		ログ出力タイプが off err 以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param size ダンプサイズ。
 *	@param P ダンプアドレス。
 */
inline void TLOG_DUMP(size_t size, LPCVOID P)
{
	CTraceLogger::Write(CTraceLogger::Type_Trace, size, P);
}

/**@ingroup LOG
 * トレース出力ダンプログ.
 *		ログ出力タイプが off err 以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param vb ダンプする CByteVector オブジェクト。
 */
inline void TLOG_DUMPV(const CByteVector& vb)
{
	CTraceLogger::Write(CTraceLogger::Type_Trace, vb);
}

/**@ingroup LOG
 * ポーリング出力ログ.
 *		ログ出力タイプに poll が含まれていたら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param lpszFormatText 書式指定文字列。 printf() 準拠です。
 *	@param ... 可変個引数。 
 */
inline void TLOG_POL(LPCTSTR lpszFormatText, ...)
{
	va_list args; va_start(args, lpszFormatText);
	CTraceLogger::Write(CTraceLogger::Type_Polling, lpszFormatText, args);
}

/**@ingroup LOG
 * LastError出力ログ.
 *		GetLastError() の結果が0以外で、ログ出力がOff以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@param lpszFunctionName 関数名。
 *	@return GetLastError() の値
 */
inline DWORD TLASTERROR(LPCTSTR lpszFunctionName)
{
	DWORD e = ::GetLastError();
	if ( e != ERROR_SUCCESS )
	{
		CSimpleStr s;
		SystemErrorToMessageText(s, e);
		LPCTSTR lpszErr = s;
		TLOG( _T("API[%s()]失敗 ErrorCode = 0x%08x(%s)"), lpszFunctionName, e, lpszErr);
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
 * ライン出力ログ.
 *		記述したソース名と行番号が出力されます。
 *		ログ出力タイプが off 以外なら出力されます。
 *	@par必要ファイル
 *		TnbTraceLogger.h
 *	@note 開発用です。リリース時、残さないようにします。
 */
#ifdef _DEBUG
	#define TLINE() CTraceLogger::Write(CTraceLogger::Type_Absolute, _T("** %s(%d):"), _T(__FILE__), __LINE__)
#else
	#define TLINE()
#endif



#else //_TnbLOG_DISABLE

	// TLOG無効用宣言
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



#ifndef _TnbDOXYGEN	//Document作成用シンボル

// 旧バージョンに対応
#define TFUNC_(XX)				TFUNC XX
#define TFUNC_PARAM_(XX)		TFUNC_PARAM XX
#define TFUNC_RESULT_(XX)		TFUNC_RESULT XX
#define TLOG_AB_(XX)			TLOG_AB XX
#define TLOG_ERR_(XX)			TLOG_ERR XX
#define TLOG_(XX)				TLOG XX
#define TLOG_POL_(XX)			TPOL XX

#endif



}; // TNB

