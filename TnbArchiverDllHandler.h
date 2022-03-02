#pragma once
/**
 *@file
 * アーカイバDLL関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbDynamicFunc.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#ifndef ARC_DECSTRACT
	//[7ZIP]の提供ヘッダ
	#include "other/7-zip32.h"
#endif
#endif //_TnbDOXYGEN



#pragma pack(push, 1)
/**@ingroup ARCHIVEFILE
 * アーカイバDLL用情報構造体
 *
 *	@par必要ファイル
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/14 新規作成
 */
struct TIndividualInfo
{
	DWORD	dwOriginalSize;			///< ファイルのサイズ
 	DWORD	dwCompressedSize;		///< 圧縮後のサイズ
	DWORD	dwCRC;					///< 格納ファイルのチェックサム
	UINT	uFlag;					///< 処理結果
	UINT	uOSType;				///< 書庫作成に使われた OS
	WORD	wRatio;					///< 圧縮率
	WORD	wDate;					///< 格納ファイルの日付(DOS 形式)
	WORD 	wTime;					///< 格納ファイルの時刻(〃)
	char	szFileName[FNAME_MAX32 + 1];	///< 格納ファイル名
	char	dummy1[3];				///< ダミー
	char	szAttribute[8];			///< 格納ファイルの属性(書庫固有)
	char	szMode[8];				///< 格納ファイルの格納モード(〃)
	#ifndef _UNICODE
	/// コンストラクタ
	TIndividualInfo(void)
	{
		ASSERT0( sizeof(TNB::TIndividualInfo) == sizeof(INDIVIDUALINFO), "TIndividualInfo()",
					"ローカルで宣言している型(TNB::TIndividualInfo)と\n"
					"インクルードしている型(INDIVIDUALINFO)で差異があります。\n");
	}
	#endif
};
#pragma pack(pop)



/**@ingroup ARCHIVEFILE
 * アーカイバDLL管理クラス
 *
 *		アーカイバDLLの Load/Free、関数コールを容易にするためのクラスです。
 *
 *	@throw CNotSupportException サポートしていない関数を実行すると、スローされます。
 *	@throw CNullPointerException 指定のDLLがない場合、スローされます。
 *
 *	@note UNICODE プロジェクトでも OEMコードで動きます。
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@see http://www.geocities.jp/matsuhp/pc/archive.html
 *
 *	@par必要ファイル
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/12 新規作成
 *	@date 10/10/04 構成改善
 */
class CArchiverDllHandler : CCopyImpossible
{
public:

	/// コマンドの ID
	enum ECommand
	{
		EC_Delete,		///< 削除
		EC_Extract,		///< 抽出
		EC_ExtractDir,	///< 抽出(Directory付き)
		EC_Attract,		///< 格納
	};

	/**
	 * コンストラクタ
	 *	@param lpszDllName アーカイバのDLL名。
	 *	@param lpszPrifix アーカイバ種のプリフィックス。
	 */
	CArchiverDllHandler(LPCSTR lpszDllName, LPCSTR lpszPrifix)
		: m_hModule(NULL), m_strPrifix(lpszPrifix), m_hWnd(NULL)
	{
		m_Init(lpszDllName);
	}

	/// デストラクタ
	virtual ~CArchiverDllHandler(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	/**
	 * [設定] 関連 Windowハンドル
	 *	@param hWnd	アーカイバdll を呼び出すアプリのウィンドウハンドル。
	 *				ウィンドウが存在しないコンソールアプリの場合や、
	 *				指定する必要のない場合は NULL を渡します。
	 */
	void SetParent(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	 /**
	 * [確認] DLL準備確認
	 *	@retval true 準備OK。
	 *	@retval false DLLが見つからなかった。他のメソッドを使用すると throw される可能性があります。
	 */
	bool IsReady(void) const
	{
		return m_hModule != NULL;
	}

	/**
	 * [実行] DLL関数実行
	 *	@param[out] _r 戻り値
	 *	@param[in] lpszFuncName 関数名
	 *	@return r の参照
	 *	@throw CTnbException 指定の関数が使用できない時、スローされます。
	 */
	template<typename R>
	R Func(R& _r, LPCSTR lpszFuncName)
	{
		R (WINAPI *pFunc0)(void); 
		_r = m_GetFuncAdd(pFunc0, lpszFuncName)();
		return _r;
	}

	/**
	 * [実行] DLL関数実行
	 *	@param[out] _r 戻り値
	 *	@param[in] lpszFuncName 関数名
	 *	@param[in] t1 第一引数
	 *	@return r の参照
	 *	@throw CTnbException 指定の関数が使用できない時、スローされます。
	 */
	template<typename R, typename T1>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1)
	{
		R (WINAPI *pFunc1)(T1 t1);
		_r = m_GetFuncAdd(pFunc1, lpszFuncName)(t1);
		return _r;
	}

	/**
	 * [実行] DLL関数実行
	 *	@param[out] _r 戻り値
	 *	@param[in] lpszFuncName 関数名
	 *	@param[in] t1 第一引数
	 *	@param[in] t2 第二引数
	 *	@return r の参照
	 *	@throw CTnbException 指定の関数が使用できない時、スローされます。
	 */
	template<typename R, typename T1, typename T2>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1, T2 t2)
	{
		R (WINAPI *pFunc2)(T1, T2);
		_r = m_GetFuncAdd(pFunc2, lpszFuncName)(t1, t2);
		return _r;
	}

	/**
	 * [実行] DLL関数実行
	 *	@param[out] _r 戻り値
	 *	@param[in] lpszFuncName 関数名
	 *	@param[in] t1 第一引数
	 *	@param[in] t2 第二引数
	 *	@param[in] t3 第三引数
	 *	@return r の参照
	 *	@throw CTnbException 指定の関数が使用できない時、スローされます。
	 */
	template<typename R, typename T1, typename T2, typename T3>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1, T2 t2, T3 t3)
	{
		R (WINAPI *pFunc3)(T1, T2, T3);
		_r = m_GetFuncAdd(pFunc3, lpszFuncName)(t1, t2, t3);
		return _r;
	}

	/**
	 * [実行] DLL関数実行
	 *	@param[out] _r 戻り値
	 *	@param[in] lpszFuncName 関数名
	 *	@param[in] t1 第一引数
	 *	@param[in] t2 第二引数
	 *	@param[in] t3 第三引数
	 *	@param[in] t4 第四引数
	 *	@return r の参照
	 *	@throw CTnbException 指定の関数が使用できない時、スローされます。
	 */
	template<typename R, typename T1, typename T2, typename T3, typename T4>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1, T2 t2, T3 t3, T4 t4)
	{
		R (WINAPI *pFunc4)(T1, T2, T3, T4);
		_r = m_GetFuncAdd(pFunc4, lpszFuncName)(t1, t2, t3, t4);
		return _r;
	}

	/**
	 * [確認] 動作中確認
	 *	@note	現在 アーカイバDLL が動作中か否かを得ます。
	 *			アプリケーション側は必ず圧縮／解凍等のファイルアクセスを伴うAPIを
	 *			実行する前には、これにより実行可能かをチェックする必要があります。
	 *	@note APIを示す一様な数値 = ISARC_GET_RUNNING(8)	
	 *	@retval true 実行中です。
	 *	@retval false 実行中ではない（実行可能）。
	 */
	bool IsRunning(void)
	{
		BOOL r;
		return !! Func(r, "GetRunning");
	}

	/**
	 * [取得] 結果
	 *	@return 結果の文字列。
	 */
	CAscii GetResultString(void)
	{
		m_strResult.ReleaseBuffer();
		return m_strResult;
	}

	/**
	 * [処理] コマンド処理.
	 *		圧縮・解凍等を行います。
	 *	@note 結果は、 GetResultString() で得ることが出来ます。
	 *	@note APIを示す一様な数値 = ISARC(0)
	 *	@param lpszCmdLine アーカイバdll に渡すコマンド文字列。
	 *	@param dwOutputSize アーカイバdll が結果を返すためのサイズ。
	 *						結果の文字列は、 GetResultString() で得られます
	 *	@retval 0 正常終了
	 *	@retval 0以外 エラー発生
	 */
	int Command(LPCSTR lpszCmdLine, const DWORD dwOutputSize)
	{
		int r;
		return Func(r, "", m_hWnd, lpszCmdLine, m_strResult.GetBuffer(dwOutputSize), dwOutputSize);
	}

	/**
	 * [取得] DLLのバージョン取得.
	 *		現在のバージョンの上位 2 ワードに 100 を掛けたものが返ってきます。\n
	 *		    例) 2.30 -> 230
	 *	@note APIを示す一様な数値 = ISARC_GET_VERSION(1)	
	 *	@return バージョン。
	 */
	WORD GetVersion(void)
	{
		WORD r;
		return Func(r, "GetVersion");
	}

	/**
	 * [取得] DLLのサブバージョン取得.
	 *		現在のバージョンの下位 2 ワードに 100 を掛けたものが返ってきます。\n
	 *			例) 4.01 -> 401
	 *	@note アーカイバDLLによってサポートしていない可能性があります。
	 *	@note APIを示す一様な数値 = 94	
	 *	@return サブバージョン。
	 */
	WORD GetSubVersion(void)
	{
		WORD r;
		return Func(r, "GetSubVersion");
	}

	/**
	 * [取得] DLLのバージョン取得.	
	 *		文字列で得ることが出来ます。\n
	 *			例) "4.01.00.01"
	 *	@return バージョン文字列。
	 */
	CStr GetVersionString(void)
	{
		CStr s;
		WORD w = GetVersion();
		try
		{
			WORD v = GetSubVersion();
			s.Format(_T("%d.%02d.%02d.%02d"), w / 100, w % 100, v / 100, v % 100);
		}
		catch(CNotSupportException& e)
		{
			e.OnCatch();
			s.Format(_T("%d.%02d"), w / 100, w % 100);
		}
		return s;
	}

	/**
	 * [設定] オプション画面
	 *	@note 解凍のオプション指定ダイアログの表示。
	 *	@note APIを示す一様な数値 = ISARC_CONFIG_DIALOG(17)	
	 *	@param[out] _lpszOptionBuffer アーカイブ種によるオプション。省略すると NULL。
	 *	@param[in] iMode アーカイブ種によるモード。省略すると 0。
	 *	@retval ERROR_ALREADY_RUNNING	アーカイブdll が既に実行中の時。
	 *	@retval TRUE					設定が成功した時。
	 *	@retval FALSE					設定が出来なかった（されなかった）時。または取消終了時。
	 */
	DWORD ConfigDialog(LPSTR _lpszOptionBuffer = NULL, const int iMode = 0)
	{
		BOOL r;
		return Func(r, "ConfigDialog", m_hWnd, _lpszOptionBuffer, iMode);
	}

	/**
	 * [確認] ファイル形式確認
	 *	@note	指定ファイルがサポートしているアーカイブファイルとして
	 *			正しいかどうかを返します。
	 *	@note APIを示す一様な数値 = ISARC_CHECK_ARCHIVE(16)
	 *	@param lpszArcFile チェックすべきアーカイブファイルのファイル名。
	 *	@param iMode アーカイブ種による。省略すると０。
	 *	@retval true 正しいアーカイブファイルの時。
	 *	@retval false ファイルが不正の時。
	 */
	bool CheckArchive(LPCSTR lpszArcFile, const int iMode)
	{
		BOOL r;
		return !! Func(r, "CheckArchive", lpszArcFile, iMode);
	}

	/**
	 * [確認] アーカイブ数確認
	 *	@note 指定された書庫ファイルに格納されているファイル数を得ます。
	 *	@note APIを示す一様な数値 = ISARC_GET_FILE_COUNT(18)
	 *	@param lpszArcFile 格納ファイル数を得たい書庫ファイル名。
	 *	@retval -1 エラー。
	 *	@retval 0以上 格納ファイル数。
	 */
	int GetFileCount(LPCSTR lpszArcFile)
	{
		int r;
		return Func(r, "GetFileCount", lpszArcFile);
	}

	/**
	 * [確認] ファンクションチェック.
	 *		指定された API が 7-zip32.dll で使用可能かどうかを得ます。
	 *		アプリケーション側は目的の機能（API）が有効であるかどうか、
	 *		事前にチェックするようにしてください。
	 *	@note APIを示す一様な数値 = ISARC_QUERY_FUNCTION_LIST(18)
	 *	@param iFunction API を示す一意な数値。
	 *			具体的な数値については 7-zip32.h などアーカイバのヘッダを参照して下さい。
	 *	@retval true 使用可能。
	 *	@retval false 使用出来ない、または iFunction は無効な値。
	 */
	bool QueryFunctionList(const int iFunction)
	{
		BOOL r;
		return !! Func(r, "QueryFunctionList", iFunction);
	}

	/**
	 * [処理] アーカイブOpen.
	 *		指定ファイルが アーカイバdll に有効な書庫ファイルかどうか調べ、
	 *		有効であればオープンし、一意の数値（ハンドル）を返します。
	 *		以後の書庫ファイルへのアクセスはこのハンドルでおこないます。
	 *		終了の際は、 CloseArchive() で閉じてください。
	 *	@note APIを示す一様な数値 = ISARC_OPEN_ARCHIVE(23)
	 *	@param lpszArcFile アーカイブファイル名。
	 *	@param dwMode		アーカイバ種による。省略すると０。
	 *	@retval NULL エラー。
	 *	@retval NULL以外 成功。
	 */
	HARC OpenArchive(LPCSTR lpszArcFile, const DWORD dwMode = 0)
	{
		HARC r;
		return Func(r, "OpenArchive", m_hWnd, lpszArcFile, dwMode);
	}

	/**
	 * [処理] アーカイブClose.
	 *		OpenArchive() で割り付けたハンドルを解放する。
	 *	@note APIを示す一様な数値 = ISARC_CLOSE_ARCHIVE(24)
	 *	@param hArc OpenArchive() で返されたハンドル。
	 *	@retval 0 成功
	 *	@retval -1 異常時
	 */
	int CloseArchive(HARC hArc)
	{
		int r;
		return Func(r, "CloseArchive", hArc);
	}

	/**
	 * [作成] コマンド文字列作成
	 *	@param cmd コマンド種。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@return コマンドと ％s を３つ含んだ文字列。Emptyならエラー。
	 */
	virtual CAscii MakeCommand(ECommand cmd, LPCSTR lpszOption = NULL)
	{
		CAscii strOpt = lpszOption;
		if ( ! strOpt.IsEmpty() )
		{
			strOpt += " ";
		}
		CAscii strResult;
		CAscii strCmd = MakeCommandOption(cmd);
		if ( strCmd.IsEmpty() )
		{
			return strResult;
		}
		strResult.Format("%s%s\042%%s\042 \042%%s\\\042 \042%%s\042", strCmd, strOpt);
		return strResult;
	}

	/**
	 * [作成] コマンドオプション文字列作成.
	 *	@param cmd コマンド種。
	 *	@return コマンド種に対応したオプション文字列。Emptyならエラー。
	 */
	virtual CAscii MakeCommandOption(ECommand cmd) = 0;

	/**
	 * [実行] コマンド実行
	 *	@note 結果は、 GetResultString() で得ることが出来ます。
	 *	@param cmd コマンド種。指定できるのは以下の４つです。
	 *				EC_Delete		削除	\n
	 *				EC_Extract		抽出	\n
	 *				EC_ExtractDir	抽出(Directory付き)	\n
	 *				EC_Attract		格納	\n
	 *	@param lpszArcFile 対象アーカイブ名。
	 *	@param lpszPath 対象パス。
	 *	@param lpszFilter 対象ファイル(ワイルドカード可能)。
	 *	@param lpszOption 追加オプション（省略可能）。
	 *	@retval 0 正常終了
	 *	@retval 0以外 エラー発生
	 */
	int Execute(ECommand cmd, LPCSTR lpszArcFile, LPCSTR lpszPath, LPCSTR lpszFilter, LPCSTR lpszOption = NULL)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// 削除
		case EC_Extract: // 抽出
		case EC_ExtractDir: // 抽出(Directory付き)
		case EC_Attract: // 格納
			strCmd = MakeCommand(cmd, lpszOption);
			break;
		default:
			ASSERT(false);
			break;
		}
		if ( strCmd.IsEmpty() )
		{
			m_strResult.ReleaseBuffer();
			m_strResult.Empty();
			return -1;
		}
		CAscii s = CAscii::Fmt(strCmd, lpszArcFile, lpszPath, lpszFilter);
		s.Replace("\\\\","\\");
		return Command(s, RESULTMAX);
	}

	/**
	 * [実行] メモリに抽出
	 *	@note 結果は、 GetResultString() で得ることが出来ます。
	 *	@param[in] lpszArcFile 対象アーカイブファイル名。
	 *	@param[in] lpszName 対象ファイル。
	 *	@param[out] _lpBuf 抽出メモリ。
	 *	@param[in,out] dwSize lpBuf のサイズ。 実行後は抽出したサイズが格納されます。
	 *	@param[in] lpszOption 追加オプション（省略可能）。
	 *	@retval 0 正常終了
	 *	@retval 0以外 エラー発生
	 */
	int ExtractMem(LPCSTR lpszArcFile, LPCSTR lpszName, BYTE* _lpBuf, size_t& dwSize, LPCSTR lpszOption = NULL)
	{
		ASSERT( ! ::IsBadWritePtr(_lpBuf, dwSize) );
		CAscii strCmd = CAscii::Fmt("%s \042%s\042 \042%s\042", CAscii(lpszOption), lpszArcFile, lpszName);
		strCmd.TrimLeft();
		DWORD dwOriginalSize = 0;
		int (WINAPI* pFunc)(const HWND, LPCSTR, BYTE*, const DWORD, time_t*, WORD*, DWORD*);
		int r = m_GetFuncAdd(pFunc, "ExtractMem")
					(m_hWnd, strCmd, _lpBuf, static_cast<DWORD>(dwSize), NULL, NULL, &dwOriginalSize);
		dwSize = dwOriginalSize;
		return r;
	}

	/**
	 * [実行] メモリから格納
	 *	@note 結果は、 GetResultString() で得ることが出来ます。
	 *	@param[in] lpszArcFile 対象アーカイブ。
	 *	@param[in] lpszName 格納ファイル名。
	 *	@param[in] time 格納時間。
	 *	@param[in] wAttr 格納属性。
	 *	@param[in] lpBuf 格納内容。
	 *	@param[in,out] _dwSize lpBuf のサイズ。処理後は圧縮後のサイズが格納されます。
	 *	@param[in] lpszOption 追加オプション（省略可能）。
	 *	@retval 0 正常終了
	 *	@retval 0以外 エラー発生
	 */
	int CompressMem(LPCSTR lpszArcFile, LPCSTR lpszName, time_t time, WORD wAttr,
										const BYTE* lpBuf, DWORD& _dwSize, LPCSTR lpszOption = NULL)
	{
		ASSERT( ! ::IsBadReadPtr(lpBuf, _dwSize) );
		CAscii strCmd = CAscii::Fmt("%s \042%s\042 \042%s\042", CAscii(lpszOption), lpszArcFile, lpszName);
		strCmd.TrimLeft();
		DWORD dwCompreSize = 0;
		int (WINAPI* pFunc)
					(const HWND, LPCSTR, const BYTE*, const DWORD, const time_t*, const WORD*, DWORD*);
		int r = m_GetFuncAdd(pFunc, "CompressMem")
					(m_hWnd, strCmd, lpBuf, _dwSize, &time, &wAttr, &dwCompreSize);
		_dwSize = dwCompreSize;
		return r;
	}

private:
	enum 
	{
		RESULTMAX = 1024 ///< 結果受け取りのサイズ 
	};
	// DLLの読み込み(最初の1回目のみ)とワークエリアの初期化を行います。
	bool m_Init(LPCSTR lpszDllName)
	{
		if ( m_hModule != NULL )
		{
			return true;
		}
		HINSTANCE h = ::LoadLibraryA(lpszDllName);
		if ( h == NULL )
		{
			return false;
		}
		// エクスポート関数の確認
		bool r = true;
		WORD (WINAPI *pGetVersion)(void);
		r &= GetFuncAddress(pGetVersion, h, m_strPrifix + "GetVersion");
		if ( r )
		{
			m_hModule = h;
		}
		else 
		{
			::FreeLibrary(h);
		}
		return r;
	}
	/**
	 * アドレス取得
	 *	@param[out] _t 取得アドレス格納
	 *	@param[in]	lpszFuncName 関数名
	 *	@return t の参照
	 */
	template<typename T>
	T& m_GetFuncAdd(T& _t, LPCSTR lpszFuncName)
	{
		if ( m_hModule == NULL )
		{
			throw CNullPointerException();
		}
		if ( ! GetFuncAddress(_t, m_hModule, m_strPrifix + lpszFuncName) )
		{
			throw CNotSupportException(); 
		}
		return _t;
	}
	HMODULE		m_hModule;			///< DLLのハンドル
	CAscii		m_strPrifix;		///< プリフィックス
	CAscii		m_strResult;		///< 結果格納用
	HWND		m_hWnd;				///< 使用HWND。
};



/**@ingroup ARCHIVEFILE
 * UnZip32.dll 管理クラス
 *
 *		UnZip32.dll の Load/Free、関数コールを容易にするためのクラスです。
 *
 *	@note 実行するPCに UnZip32.dll がインストールされている必要があります（ IsReady() にて確認可能）。
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@todo 現在解凍のみ対応。
 *
 *	@par必要ファイル
 *			TnbArchiverDllHandler.h
 *
 *	@date 18/12/25 新規作成
 *	@date 19/01/03 クラス名変更
 */
class CUnZipDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// コンストラクタ
	CUnZipDllHandler(void) : _super("UnZip32.dll", "UnZip")
	{
	}

	/**
	 * [作成] コマンドオプション文字列作成.
	 *	@param cmd コマンド種。
	 *	@return コマンド種に対応したオプション文字列。Emptyならエラー。
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// 削除
			strCmd = "-d ";
			break;
		case EC_Extract: // 抽出
			strCmd = " ";
			break;
		case EC_ExtractDir: // 抽出(Directory付き)
			strCmd = " ";
			break;
		case EC_Attract: // 格納
			strCmd = "";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * 7-Zip32.dll 管理クラス
 *
 *		7-Zip32.dll の Load/Free、関数コールを容易にするためのクラスです。
 *
 *	@note 実行するPCに 7-Zip32.dll がインストールされている必要があります（ IsReady() にて確認可能）。
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par必要ファイル
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/12 新規作成
 */
class C7zDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// コンストラクタ
	C7zDllHandler(void) : _super("7-zip32.dll", "SevenZip")
	{
	}

	/**
	 * [確認] ファイル形式確認
	 *	@param lpszArcFile アーカイブファイル名
	 *	@retval ARCHIVETYPE_ZIP		ZIP形式
	 *	@retval ARCHIVETYPE_7Z		7Z形式
	 *	@retval 0					7-zip32.dll で利用できない形式
	 *	@retval -1					エラー
	 */
	int GetArchiveType(LPCSTR lpszArcFile)
	{
		int r;
		return Func(r, "GetArchiveType", lpszArcFile);
	}
	
	/**
	 * [設定] 使用文字設定
	 *	@note OEM(日本だと SJIS) と UTF-8 の切り替えが可能です。
	 *	@param boIsUtf8 true を設定すると UTF-8 が使用されます。
	 *					false を設定すると OEM コードセットが使用されます。
	 *	@return 常に true 。
	 */
	bool SetUnicodeMode(bool boIsUtf8)
	{
		BOOL r = boIsUtf8;
		return !! Func(r, "SetUnicodeMode", boIsUtf8);
	}

	/**
	 * [作成] コマンドオプション文字列作成
	 *	@param cmd コマンド種。
	 *	@return コマンド種に対応したオプション文字列。Emptyならエラー。
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// 削除
			strCmd = "d ";
			break;
		case EC_Extract: // 抽出
			strCmd = "e ";
			break;
		case EC_ExtractDir: // 抽出(Directory付き)
			strCmd = "x ";
			break;
		case EC_Attract: // 格納
			strCmd = "a ";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * UnLha32.dll 管理クラス
 *
 *		UnLha32.dll の Load/Free、関数コールを容易にするためのクラスです。
 *
 *	@note 使用するには、本ヘッダよりも先に UnLha32.h を include しておく必要があります。
 *	@note 実行するPCに UnLha32.dll がインストールされている必要があります（ IsReady() にて確認可能）。
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par必要ファイル
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/13 新規作成
 *	@date 10/10/04 オプション修正
 */
class CUnLhaDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// コンストラクタ
	CUnLhaDllHandler(void) : _super("UnLha32.dll", "Unlha")
	{
	}

	/**
	 * [作成] コマンドオプション文字列作成
	 *	@param cmd コマンド種。
	 *	@return コマンド種に対応したオプション文字列。Emptyならエラー。
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// 削除
			strCmd = "d ";
			break;
		case EC_Extract: // 抽出
			strCmd = "e ";
			break;
		case EC_ExtractDir: // 抽出(Directory付き)
			strCmd = "x ";
			break;
		case EC_Attract: // 格納
			strCmd = "a -r1 -x1 ";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * Cab32.dll 管理クラス
 *
 *		Cab32.dll の Load/Free、関数コールを容易にするためのクラスです。
 *
 *	@note 使用するには、本ヘッダよりも先に UnLha32.h を include しておく必要があります。
 *	@note 実行するPCに Cab32.dll がインストールされている必要があります（ IsReady() にて確認可能）。
 *
 *	@note キャビネットファイルの仕様では、空のフォルダを格納できません。
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par必要ファイル
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/13 新規作成
 *	@date 10/10/04 オプション修正
 */
class CCabDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// コンストラクタ
	CCabDllHandler(void) : _super("Cab32.dll", "Cab")
	{
	}

	/**
	 * [作成] コマンドオプション文字列作成
	 *	@param cmd コマンド種。
	 *	@return コマンド種に対応したオプション文字列。Emptyならエラー。
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// 削除		-- 未サポート
			break;
		case EC_Extract: // 抽出
			strCmd = "-x -j";
			break;
		case EC_ExtractDir: // 抽出(Directory付き)
			strCmd = "-x ";
			break;
		case EC_Attract: // 格納
			strCmd = "-a -r ";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * Tar32.dll 管理クラス
 *
 *		Tar32.dll の Load/Free、関数コールを容易にするためのクラスです。
 *
 *	@note 使用するには、本ヘッダよりも先に UnLha32.h を include しておく必要があります。
 *	@note 実行する PC に Tar32.dll がインストールされている必要があります（ IsReady() にて確認可能）。
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par必要ファイル
 *			TnbArchiverDllHandler.h
 *
 *	@date 10/10/01 新規作成
 *	@date 10/10/04 オプション修正
 */
class CTarDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// コンストラクタ
	CTarDllHandler(void) : _super("Tar32.dll", "Tar")
	{
	}

	/**
	 * [作成] コマンドオプション文字列作成
	 *	@param cmd コマンド種。
	 *	@return コマンド種に対応したオプション文字列。Emptyならエラー。
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// 削除		-- 未サポート
			break;
		case EC_Extract: // 抽出
			strCmd = "-x ";
			break;
		case EC_ExtractDir: // 抽出(Directory付き)
			strCmd = "-xf ";
			break;
		case EC_Attract: // 格納
			strCmd = "-cf ";
			break;
		default:
			break;
		}
		return strCmd;
	}
};



}; // TNB

#if 0
アーカイバDLL 	圧縮	N圧縮	解凍 	F解凍 	更新 	検査 	一覧 	SFX 	詳細 	削除 	注釈 	ヘ変 
------------------------------------------------------------------------------------------------------------
7-Zip32.DLL 	a		**** 	e		x 		u		t	 	l	 	**** 	**** 	d 		**** 	**** 
UNLHA32.DLL 	a		u		e		x	 	f	 	t	 	l	 	s	 	v	 	d 		c		y 
CAB32.DLL 		a 		**** 	x 		x 		**** 	c,t 	l 		f 		**** 	**** 	**** 	**** 
TAR32.DLL 		-cvf 	**** 	-xvf 	-xvf 	**** 	**** 	-tvf 	**** 	**** 	**** 	**** 	**** 
------------------------------------------------------------------------------------------------------------
UNZIP32.DLL 	**** 	**** 	-x,-xv 	-x,-xv 	**** 	-t 		-l,-lv 	**** 	-v		-d		-z,-Z 	**** 
ZIP32J.DLL 		-r 		-r 		**** 	**** 	-f,-u 	**** 	**** 	-sfx 	**** 	-d 		-z 		**** 
UNARJ32J.DLL 	**** 	**** 	e,ev 	x,xv 	**** 	t 		l,lv 	**** 	v 		**** 	**** 	**** 
UNRAR32.DLL 	**** 	**** 	-e 		-x 		**** 	**** 	-l 		**** 	-v		**** 	**** 	**** 
BGA32.DLL 		a 		**** 	x 		x		**** 	t 		l,lv 	s 		**** 	d 		**** 	**** 
YZ1.DLL 		c 		**** 	x 		x	 	**** 	**** 	**** 	**** 	**** 	**** 	**** 	**** 
UNGCA32.DLL 	**** 	**** 	e,ex 	e,ex 	**** 	t 		l	 	**** 	**** 	**** 	**** 	**** 
UNIMP32.DLL 	**** 	**** 	**** 	-o 		**** 	**** 	**** 	**** 	**** 	**** 	**** 	**** 
UNBEL32.DLL 	＜＜コマンドは不要＞＞ 
	備考：N圧縮:新規追加圧縮, F解凍:フォルダ解凍, SFX:自己解凍書庫を作成, へ変:ヘッダ変更, **** は未対応

#endif