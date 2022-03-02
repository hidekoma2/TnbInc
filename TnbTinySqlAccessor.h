#pragma once
/**
 *@file
 * 簡易 SQL アクセス関係のヘッダ
 *
 *		SQL アクセスするためのクラス群があります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include <TnbStrOperator.h>
#include <TnbHandleHandle.h>
#include <TnbPointerVector.h>



#ifndef _WIN32_WCE
 #include <sql.h>
 #include <sqltypes.h>
 #include <sqlucode.h>
 #pragma comment(lib, "odbc32.lib")
#else
 #include "minodbc.h" // 要 Mimer SDK。
 #pragma comment(lib, "mimodbcp.lib")
 #pragma comment(lib, "minodbcw.lib")
 #pragma comment(lib, "minsql32.lib")
#endif



namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/// PointerHandleBase用 SQLHSTMT Freeクラス
struct TPhFreeStmtHandle
{
	void operator()(SQLHSTMT h) 
	{
		__try 
		{
			::SQLFreeHandle(SQL_HANDLE_STMT, h); 
		}
		__except( EXCEPTION_EXECUTE_HANDLER ) 
		{
			ASSERT0( false, "", "先に CTinySqlAccessor が Disconnect しているか Stop しているか破棄されています。" );
		}
	}
};

#endif //_TnbDOXYGEN



/**@ingroup DBASE
 * 簡易 SQL アクセサクラス
 *
 *		データベースに接続し、 SQL を実行することが出来ます。
 *		戻り値を取得することが出来ます。
 *
 *	@par使用例(SELECT)
 *	\code
 *		CTinySqlAccessor sql;
 *		// test-db というデータベースに接続
 *		bool r = sql.Connect(_T("test-db"),_T("user"),_T("pass"));
 *		CTinySqlAccessor::CReturnCode rc = sql.GetLastReturnCode();
 *		if ( r ) {
 *			// SQL 文を実行
 *			CTinySqlAccessor::CStatement stmt = sql.CreateStatement();
 *			rc = sql.GetLastReturnCode();
 *			if ( stmt.IsValid() ){
 *				r = stmt.Execute(_T("SELECT * FROM Table_1"));
 *				rc = stmt.GetLastReturnCode();
 *				if ( r ) {
 *					// 結果セットを取得
 *					CTinySqlAccessor::CResultSet rs = stmt.GetResultSet();
 *					ASSERT( rs.IsValid() );
 *					INDEX idx = rs.FindColumn(_T("SENSOR_TIME")); // SENSOR_TIME というラベルのカラムのインデックスを取得.
 *					while ( rs.Next() ) {
 *						int s1 = rs.GetInt(_T("SENSOR_ID");	// SENSOR_ID というラベルのカラムから数値取得
 *						CStr tm = rs.GetString(idx);		// SENSOR_TIME というラベルのカラムから時間情報を文字列で取得
 *						CStr s2 = rs.GetString(4);			// カラム４から文字列取得
 *						if ( rs.WasNull() ) {
 *							//カラム４が NULL だ！
 *							TRACE1(" id = %d, <null>\n", s1);
 *						}
 *						else {
 *							TRACE2(" id = %d, [%s]\n", s1, s2);
 *						}
 *					}
 *				}
 *			}
 *		}
 *		TRACE1("ret = %s\n", rc.GetErrorString());
 *		return;
 *	\endcode
 *
 *	@par使用例(INSERT)
 *	\code
 *		CTinySqlAccessor sql;
 *		// test-db というデータベースに接続
 *		bool r = sql.Connect(_T("test-db"),_T("user"),_T("pass"));
 *		CTinySqlAccessor::CReturnCode rc = sql.GetLastReturnCode();
 *		sql.SetAutoCommit(false); //自動コミット無効
 *		if ( r ) {
 *			// SQL 文を実行
 *			CTinySqlAccessor::CPreparedStatement stmt = sql.PreparedStatement(_T("INSERT INTO Table_1 VALUES(?,?)"));
 *			rc = sql.GetLastReturnCode();
 *			if ( stmt.IsValid() ){
 *				stmt.SetString(2, _T("ABC"));	// 二つ目の ？ に 'ABC'
 *				loop ( i, 2 ) {
 *					stmt.SetInt(1, 100 + 100);		// 一つ目の ？ に 100 + i
 *					r = stmt.Execute();
 *					rc = stmt.GetLastReturnCode();
 *					if ( ! r ) {
 *						break;
 *					}
 *				}
 *			}
 *		}
 *		TRACE1("ret = %s\n", rc.GetErrorString());
 *		return;
 *	\endcode
 *
 *		@note Start、Stop時に 環境ハンドル(SQLHENV), DBCハンドル(SQLHDBC) を構築、破棄しています。
 *
 *	@par必要ファイル
 *			TnbTinySqlAccessor.h
 * 
 *	@date 11/05/16	新規作成
 *	@date 12/05/02	読込時の型チェックを強化。
 *	@date 12/06/06	Type_Bit に対応。
 *	@date 12/10/17	SetQueryTimeout() 新規。
 */
class CTinySqlAccessor : CCopyImpossible
{
public:

	/// 値種類
	enum EType
	{
		Type_Null			= SQL_TYPE_NULL,		///< 値なし
		Type_Binary			= SQL_BINARY,			///< バイナリ
		Type_AsciiString	= SQL_CHAR,				///< ASCII文字列
		Type_UnicodeString	= SQL_WCHAR,			///< UNICODE文字列
		Type_DoubleFloat	= SQL_DOUBLE,			///< 64bit double float 実数
		Type_Float			= SQL_FLOAT,			///< 32bit float 実数
		Type_Integer		= SQL_INTEGER,			///< 32bit signed 整数
		Type_Short			= SQL_SMALLINT,			///< 16bit signed 整数
		Type_TimeStamp		= SQL_TYPE_TIMESTAMP,	///< タイムスタンプ
		Type_Bit			= SQL_BIT				///< 1bit 値	
	};

	/**@ingroup DBASE
	 * 簡易 SQL アクセサ用戻り値管理.
	 *
	 *		CTinySqlAccessor のメソッドの戻り値です。
	 *
	 *	@par必要ファイル
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	新規作成
	 */
	class CReturnCode
	{
	public:

		/**
		 * コード群.
		 *		GetDetialCode() の戻り値で使用されます。
		 */
		struct TCodes
		{
			CStr		sqlState;		///< 状態
			SQLINTEGER	nativeError;	///< エラー
			CStr		errorMessage;	///< エラーメッセージ
			SQLINTEGER	plm_Rownumber;	///< ？
			/// コンストラクタ
			TCodes(void) : nativeError(0), plm_Rownumber(0)
			{
			}
		};

		/// コード群配列管理
		typedef CVectorT<TCodes> CCodeVector;

		/**
		 * コンストラクタ.
		 *		「成功」の状態を持つインスタンスになります。
		 */
		CReturnCode(void) : m_type(0), m_handle(0), m_returnCode(SQL_SUCCESS)
		{
		}

		/**
		 * コンストラクタ.
		 *		指定の状態を持つインスタンスになります。
		 *	@param type ハンドルタイプ.
		 *	@param handle ハンドル
		 *	@param rc SQL API からのリターンコード.
		 *	@param isSilent true ならデバッグ表示をしません。
		 */
		CReturnCode(SQLSMALLINT type, SQLHANDLE handle, SQLRETURN rc, bool isSilent = false) : m_type(type), m_handle(handle), m_returnCode(rc)
		{
			if ( ! isSilent )
			{
				Dump();
			}
		}

		/**
		 * [取得] 戻り値取得.
		 *		SQL API からの戻り値そのものを返します。
		 *	@retval SQL_SUCCESS 成功。
		 *	@retval SQL_SUCCESS_WITH_INFO 成功。
		 *	@retval SQL_ERROR 失敗。 GetDetialCode() で詳細が取得できます。
		 *	@retval SQL_INVALID_HANDLE ハンドル不正
		 *	@retval その他 その他のエラー。
		 */
		SQLRETURN GetReturnCode(void) const
		{
			return m_returnCode;
		}

		/**
		 * [確認] 成功か？
		 *	@retval true 成功.
		 *	@retval false エラー
		 */
		operator bool(void) const
		{
			return ! HasError();
		}

		/**
		 * [確認] 失敗か？
		 *	@retval false 成功.
		 *	@retval true エラー
		 */
		bool HasError(void) const
		{
			return (m_returnCode != SQL_SUCCESS && m_returnCode != SQL_SUCCESS_WITH_INFO);
		}

		/**
		 * [取得] エラー文字列取得.
		 *	@note 本インスタンスが「成功」の場合、Empty 文字列が返ります。
		 *	@return エラー文字列。
		 */
		CStr GetErrorString(void) const
		{
			CStr s;
			switch ( m_returnCode )
			{
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:
				break;
			case SQL_NO_DATA:
				s = _T("No Data");
				break;
			case SQL_ERROR:
				s = _T("Error");
				break;
			case SQL_INVALID_HANDLE:
				s = _T("Invalid Handle");
				break;
			case SQL_NEED_DATA:
				s = _T("Need Data");
				break;
			default:
				s = _T("Unknown Error");
				break;
			}
			return s;
		}

		/**
		 * [取得] 詳細情報取得.
		 *	@param withDiagnostics true なら診断もおこなう。 default は false。
		 *	@return 詳細情報配列。
		 */
		CCodeVector GetDetialCode(bool withDiagnostics = false) const
		{
			CCodeVector codes;
			SQLRETURN rc = SQL_SUCCESS;
			SQLSMALLINT record = 1;
			while ( rc == SQL_SUCCESS )
			{
				TCodes c;
				SQLSMALLINT emLen = 0;
				rc = ::SQLGetDiagRec(m_type, m_handle, record, GetSqlChar(c.sqlState, MAX_PATH), &c.nativeError
														, GetSqlChar(c.errorMessage, MAX_PATH), MAX_PATH, &emLen);
				c.sqlState.ReleaseBuffer();
				c.errorMessage.ReleaseBuffer();
				if ( rc == SQL_SUCCESS )
				{
					if ( withDiagnostics )
					{
						m_GetDiagField(record, SQL_DIAG_ROW_NUMBER, &c.plm_Rownumber, SQL_IS_INTEGER);
					}
					codes.Add(c);
				}
				record++;
			}
			return codes;
		}

		/**
		 * [確認] 成功確認.
		 *	@note SQL API からの戻り値の判断をします。
		 *	@param r SQL API からの戻り値.
		 *	@retval true 成功.
		 *	@retval false エラー
		 */
		static bool IsReturnCodeSuccess(SQLRETURN r)
		{
			switch ( r )
			{
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:
				return true;
			default:
				break;
			}
			return false;
		}

		/**
		 * [出力] ダンプ.
		 *		インスタンス内容を Outputに出力します。
		 *	@note デバッグ用メソッドです。リリースビルドでは何もしません。
		 */
		void Dump(void)
		{
			#ifdef _TnbDEBUG_ON
			if ( m_returnCode != SQL_SUCCESS )
			{
				TTRACE2("*SqlReturn %d[%s]\n", m_returnCode, GetErrorString());
				CVectorT<TCodes> v = GetDetialCode(true);
				loop ( i, v )
				{
					const TCodes& t = v[i];
					TTRACE2("No%d; state[%s]", i + 1, t.sqlState);
					TTRACE3(", e=%d<%s>, %d\n", t.nativeError, t.errorMessage, t.plm_Rownumber);
				}
			}
			#endif
		}

	private:
		/// 診断情報取得
		SQLRETURN m_GetDiagField(SQLSMALLINT iRecord, SQLSMALLINT fDiagField, SQLPOINTER rgbDiagInfo, SQLSMALLINT cbDiagInfoMax, SQLSMALLINT* pcbDiagInfo = NULL) const
		{
			return ::SQLGetDiagField(m_type, m_handle, iRecord, fDiagField, rgbDiagInfo, cbDiagInfoMax, pcbDiagInfo);
		}
		SQLSMALLINT		m_type;			///< ハンドルタイプ
		SQLHANDLE		m_handle;		///< ハンドル
		SQLRETURN		m_returnCode;	///< 戻り値
	};


	//-----------------------------


	/**@ingroup DBASE
	 * 簡易 SQL アクセサ用 STMT ハンドルハンドル.
	 *		SQLHSTMT の管理するハンドルクラスです。
	 *
	 *	@par必要ファイル
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	新規作成
	 */
	typedef TNB::CPointerHandleBaseT<SQLHSTMT, TPhFreeStmtHandle> CStmtHandle;


	//-----------------------------


	/**@ingroup DBASE
	 * 簡易 SQL アクセサ用カラムインデックス.
	 *
	 *		CResultSet クラスのゲッターメソッドなどのインデックス指定に使用します。
	 *		通常意識してインスタンスを作成しません。
	 *
	 *	\code
	 *		CTinySqlAccessor sql;
	 *		sql.Connect(_T("test-db"),_T("user"),_T("pass"));
	 *		CTinySqlAccessor::CStatement stmt = sql.CreateStatement();
	 *		stmt.execute(_T("SELECT ～"));
	 *		CTinySqlAccessor::CResultSet rs = stmt.GetResultSet();
	 *
	 *		int s1 = rs.GetInt(_T("SENSOR_ID");	//ここで暗黙的に CColumnIndex が使用される
	 *		CStr s2 = rs.GetString(4);			//			〃
	 *	\endcode
	 *
	 *	@par必要ファイル
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	新規作成
	 */
	class CColumnIndex
	{
	public:

		/**
		 * コンストラクタ.
		 *	@param number 最初の列は１、二番目は２を指定します。
		 */
		CColumnIndex(INDEX number) : m_number(number)
		{
		}

		/**
		 * コンストラクタ.
		 *	@param lpszLabel ラベル名を指定します。。
		 */
		CColumnIndex(LPCTSTR lpszLabel) : m_label(lpszLabel), m_number(0)
		{
		}

		/**
		 * [確認] ラベル指定か？
		 *	@retval true ラベル指定.
		 *	@retval false 番号指定
		 */
		bool HasLabel(void) const
		{
			return ! m_label.IsEmpty();
		}

		/**
		 * [取得] ラベル.
		 *	@return ラベル.
		 */
		LPCTSTR GetLabel(void) const
		{
			return m_label;
		}

		/**
		 * [取得] 番号.
		 *	@return 番号.
		 */
		INDEX GetNumber(void) const
		{
			return m_number;
		}

	private:
		INDEX	m_number;	///< 番号
		CStr	m_label;	///< ラベル
	};


	//-----------------------------


	/**@ingroup DBASE
	 * 簡易 SQL アクセサ用結果セット管理.
	 *
	 *		CTinySqlAccessor::CStatement::GetResultSet() などの戻り値で使われます。
	 *		使用前に、 IsValid() で正しい情報を持っているか、確認してください。
	 *
	 *	@see CTinySqlAccessor
	 *
	 *	@par必要ファイル
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	新規作成
	 *	@date 12/01/26	値取得に SQLGetData() を使用するように変更。
	 */
	class CResultSet
	{
	public:

		/// カラム情報
		struct TColumnInformation
		{
			CStr		name;				///< 名称
			SQLSMALLINT dataType;			///< 型
			SQLULEN		size;				///< 型サイズ
			SQLSMALLINT decimalDigits;		///< 小数点情報
			bool		isNullable;			///< Nullが有効か？
		};

		/**
		 * コンストラクタ.
		 *	@param hStmt SQL ステートメントハンドル 
		 */
		CResultSet(CStmtHandle hStmt) : m_hStmt(hStmt), m_isBinding(false), m_isLastNull(false)
		{
		}

		/**
		 * コピーコンストラクタ.
		 *	@param other コピー元 
		 */
		CResultSet(const CResultSet& other) : m_hStmt(other.m_hStmt), m_isBinding(false), m_isLastNull(false)
		{
		}

		/**
		 * コピーオペレータ.
		 *	@param other コピー元
		 *	@return 自分の参照.
		 */
		CResultSet& operator=(const CResultSet& other)
		{
			m_hStmt = other.m_hStmt;
			m_isBinding = false;
			m_isLastNull = false;
			return *this;
		}

		/**
		 * [確認] 正しい情報？
		 *	@retval true 正しい情報を持っている
		 *	@retval false 否。
		 */
		bool IsValid(void) const
		{
			return ! m_hStmt.IsNull();
		}

		/**
		 * [設定] 使用不可.
		 *		保持しているハンドルを破棄します。
		 */
		void Destroy(void)
		{
			m_hStmt = NULL;
		}

		/**
		 * [設定] 次読み出し.
		 *	@note 初期状態では、最初の行の前を挿している状態になっています。必ず最初に本メソッドを読んでからゲッターを使用してください。
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool Next(void)
		{
			if ( ! Binding() )
			{
				return false;
			}
			if ( ! m_Rst(::SQLFetch(m_hStmt)) )
			{
				return false;
			}
			loop ( i, m_bindWorks.GetSize() )
			{
				TBindWork &b = m_bindWorks[i];
				SQLLEN ll;
				if ( b.dataType == SQL_VARBINARY )
				{
					ll = b.size;
					b.work.Resize(ll);
					if ( ! m_Rst(::SQLGetData(m_hStmt, down_cast<SQLUSMALLINT>(i + 1), Type_Binary, b.work.Ref(), ll, &b.length)) )
					{
						return false;
					}
					b.dataType = Type_Binary;
				}
				else
				{
					if ( b.size == 0x7FFFFFFF )
					{
						BYTE tmp;
						RETCODE rc = ::SQLGetData(m_hStmt, down_cast<SQLUSMALLINT>(i + 1), SQL_C_CHAR, &tmp, 1, &ll);
						if ( rc < 0 )
						{
							return m_Rst(rc);
						}
					}
					else
					{
						ll = b.size;
					}
					ll = ll * 2 + 2;
					b.work.Resize(ll);
					SQLSMALLINT cc = (sizeof(TCHAR) == 1) ? SQL_C_CHAR : SQL_WCHAR;
					if ( ! m_Rst(::SQLGetData(m_hStmt, down_cast<SQLUSMALLINT>(i + 1), b.dataType, b.work.Ref(), ll, &b.length), true) )
					{
						if ( ! m_Rst(::SQLGetData(m_hStmt, down_cast<SQLUSMALLINT>(i + 1), cc, b.work.Ref(), ll, &b.length)) )
						{
							return false;
						}
						b.dataType = cc;
					}
				}
			}
			return true;
		}

		/**
		 * [確認] NULL確認.
		 *		最後にゲットした値が、 NULL であったかどうか、判断します。
		 *	@retval true NULLだった。
		 *	@retval false 否。
		 */
		bool WasNull(void) const
		{
			return m_isLastNull;
		}

		/**
		 * [取得] 文字列取得.
		 *	@param index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@return 値。 値が NULL の場合、 Empty 文字列が返され、直後の WasNull() で true が返ります。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		CStr GetString(CColumnIndex index)
		{
			CStr r;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [取得] 文字列取得.
		 *	@param index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@return 値。 値が NULL の場合、 Empty 文字列が返され、直後の WasNull() で true が返ります。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		CUnicode GetUnicodeString(CColumnIndex index)
		{
			CUnicode r;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [取得] 文字列取得.
		 *	@param index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@return 値。 値が NULL の場合、 Empty 文字列が返され、直後の WasNull() で true が返ります。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		CAscii GetAsciiString(CColumnIndex index)
		{
			CAscii r;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [取得] double 値取得.
		 *	@param index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@return 値。 値が NULL の場合、 0.0 が返され、直後の WasNull() で true が返ります。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		double GetDouble(CColumnIndex index)
		{
			double r = 0.0;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [取得] Float 値取得.
		 *	@param index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@return 値。 値が NULL の場合、 0.0 が返され、直後の WasNull() で true が返ります。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		float GetFloat(CColumnIndex index)
		{
			float r = 0.0;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [取得] int 値取得.
		 *	@param index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@return 値。 値が NULL の場合、 0 が返され、直後の WasNull() で true が返ります。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		int GetInt(CColumnIndex index)
		{
			int r = 0;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [取得] タイムスタンプ値取得.
		 *	@param[out] _v 値が格納されます。 値が NULL の場合、全て ０になり、直後の WasNull() で true が返ります。
		 *	@param[in] index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		void GetTime(SYSTEMTIME& _v, CColumnIndex index)
		{
			Zero(_v);
			m_Check(index).Get(_v);
		}

		/**
		 * [取得] バイナリデータ取得.
		 *	@param[out] _v 値が格納されます。 値が NULL の場合、サイズが０になり、直後の WasNull() で true が返ります。
		 *	@param[in] index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@throw CNotSupportException 型が違う(変換できない)。
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		void GetBinary(CWorkMem& _v, CColumnIndex index)
		{
			_v.Resize(0);
			TBindWork& B = m_Check(index);
			_v.Reset(B.length, B.work.Ref());
		}

		/**
		 * [取得] カラム情報取得.
		 *	@param index カラムインデックス。数値でもラベル名でも指定可能。数値の場合、一番目は１、二番目は２・・・を指定します。
		 *	@return カラム情報
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		const TColumnInformation& GetColumnInformation(CColumnIndex index)
		{
			Binding();
			return m_bindWorks[m_Find(index)];
		}

		/**
		 * [取得] ラベル名検索.
		 *		指定のラベルのカラムインデックス(数値)を検索します。
		 *	@param lpszLabel ラベル名
		 *	@retval 0 見つからなかった。
		 *	@retval 1以上 カラムインデックス(数値)。
		 */
		INDEX FindColumn(LPCTSTR lpszLabel)
		{
			Binding();
			loop ( i, m_bindWorks.GetSize() )
			{
				if ( m_bindWorks[i].name.CompareNoCase(lpszLabel) == 0 )
				{
					return i + 1;
				}
			}
			return 0;
		}

		/**
		 * [取得] カラム数取得.
		 *	@note Next() を一度でもコールしないと、正しい値が返りません。
		 *	@return カラム数.
		 */
		size_t GetColumnCount(void) const
		{
			return m_bindWorks.GetSize();
		}

		/**
		 * [取得] 戻り値詳細取得.
		 *	@return 戻り値.
		 */
		CReturnCode GetLastReturnCode(void) const
		{
			return m_lastReturnCode;
		}

	protected:
		/// バインド(現在はカラム情報取得のみ)
		bool Binding(void)
		{
			if ( m_isBinding )
			{
				return m_Rst();
			}
			//== カラム数取得
			SQLSMALLINT columnCount;
			SQLRETURN r = ::SQLNumResultCols(m_hStmt, &columnCount);
			if ( ! m_Rst(r, true) )
			{
				return false;
			}
			//== カラム情報取得
			m_bindWorks.Resize(columnCount);
			loop ( i, columnCount )
			{
				SQLSMALLINT nameLen;
				TBindWork& b = m_bindWorks[i];
				SQLSMALLINT nullable;
				r = ::SQLDescribeCol(m_hStmt,					// [in] ステートメント
								down_cast<SQLUSMALLINT>(i + 1),	// [in] カラム番号(１～)
								GetSqlChar(b.name, 100),		// [out] カラム名
								100,							// [in] カラム名領域
								&nameLen,						// [out] カラム名長
								&b.dataType,					// [out] 型
								&b.size,						// [out] 型サイズ
								&b.decimalDigits,				// [out] 小数点情報
								&nullable);						// [out] NULLが有効か
				b.isNullable = (nullable != 0);
				b.name.ReleaseBuffer();
				if ( ! m_Rst(r) )
				{
					return false;
				}
				TTRACE3("Col %d-[%s], type=%d, ", i + 1, b.name, b.dataType);
				TTRACE3("size=%d, dec=%d, nullable=%d\n", b.size, b.decimalDigits, b.isNullable);
			}
			m_isBinding = true;
			return true;
		}

	private:
		/// バインド用ワーク.
		struct TBindWork : TColumnInformation
		{
			CWorkMem			work;		///< Bind 領域
			SQLLEN				length;		///< Fetch 結果値
			/**
			 * [取得] 文字列取得.
			 *	@param[out] _value 文字列が格納されます。
			 *	@retval true 成功。
			 *	@retval false 値をもっていない。
			 *	@throw CNotSupportException 型が違う(変換できない)。
			 */
			bool Get(CUnicode& _value) const
			{
				if ( length >= 0 )
				{
					CStr s;
					if ( m_Get(s) )
					{
						_value = s;
						return true;
					}
					throw CNotSupportException();
				}
				return false;
			}
			/**
			 * [取得] 文字列取得.
			 *	@param[out] _value 文字列が格納されます。
			 *	@retval true 成功。
			 *	@retval false 値をもっていない。
			 *	@throw CNotSupportException 型が違う(変換できない)。
			 */
			bool Get(CAscii& _value) const
			{
				if ( length >= 0 )
				{
					CStr s;
					if ( m_Get(s) )
					{
						_value = s;
						return true;
					}
					throw CNotSupportException();
				}
				return false;
			}
			/**
			 * [取得] double float 値取得.
			 *	@param[out] _value 値が格納されます。
			 *	@retval true 成功。
			 *	@retval false 値をもっていない。
			 *	@throw CNotSupportException 型が違う(変換できない)。
			 */
			bool Get(double& _value) const
			{
				if ( length >= 0 )
				{
					if ( dataType == Type_DoubleFloat )
					{
						if ( length < sizeof(double) )
						{
							throw CNotSupportException();
						}
						_value = *reinterpret_cast<const double*>(work.Ref());
						return true;
					}
					else if ( dataType == Type_Float )
					{
						if ( length < sizeof(float) )
						{
							throw CNotSupportException();
						}
						_value = *reinterpret_cast<const float*>(work.Ref());
						return true;
					}
					else
					{
						CStr s;
						if ( m_Get(s) )
						{
							_value = s.ToDouble();
							return true;
						}
					}
					throw CNotSupportException();
				}
				return false;;
			}
			/**
			 * [取得] float 値取得.
			 *	@param[out] _value 値が格納されます。
			 *	@retval true 成功。
			 *	@retval false 値をもっていない。
			 *	@throw CNotSupportException 型が違う(変換できない)。
			 */
			bool Get(float& _value) const
			{
				double d;
				if ( Get(d) )
				{
					_value = static_cast<float>(d);
					return true;
				}
				return false;;
			}
			/**
			 * [取得] integer 値取得.
			 *	@param[out] _value 値が格納されます。
			 *	@retval true 成功。
			 *	@retval false 値をもっていない。
			 *	@throw CNotSupportException 型が違う(変換できない)。
			 */
			bool Get(int& _value) const
			{
				if ( length >= 0 )
				{
					if ( dataType == Type_Integer )
					{
						if ( length < sizeof(int) )
						{
							throw CNotSupportException();
						}
						_value = *reinterpret_cast<const int*>(work.Ref());
						return true;
					}
					else if ( dataType == Type_Short )
					{
						if ( length < sizeof(short) )
						{
							throw CNotSupportException();
						}
						_value = *reinterpret_cast<const short*>(work.Ref());
						return true;
					}
					else if ( dataType == Type_Bit )
					{
						if ( length < sizeof(BYTE) )
						{
							throw CNotSupportException();
						}
						_value = work[0];
						return true;
					}
					else
					{
						CStr s;
						if ( m_Get(s) )
						{
							_value = s.ToInt();
							return true;
						}
					}
					throw CNotSupportException();
				}
				return false;;
			}
			/**
			 * [取得] タイム取得.
			 *	@param[out] _time 値が格納されます。
			 *	@retval true 成功。
			 *	@retval false 値をもっていない。
			 *	@throw CNotSupportException 型が違う(変換できない)。
			 */
			bool Get(SYSTEMTIME& _time) const
			{
				if ( length >= 0 )
				{
					if ( dataType == Type_TimeStamp )
					{
						if ( length < sizeof(TIMESTAMP_STRUCT) )
						{
							throw CNotSupportException();
						}
						const TIMESTAMP_STRUCT* T = reinterpret_cast<const TIMESTAMP_STRUCT*>(work.Ref());
						Zero(_time);
						_time.wYear = T->year;
						_time.wMonth = T->month;
						_time.wDay = T->day;
						_time.wHour = T->hour;
						_time.wMinute = T->minute;
						_time.wSecond = T->second;
						return true;
					}
					throw CNotSupportException();
				}
				return false;;
			}
			/**
			 * [取得] 文字列化
			 *	@note 持っている情報を文字列化します。
			 *	@return 文字列
			 */
			CStr ToString(void) const
			{
				CStr s;
				try
				{
					if ( ! Get(s) )
					{
						s = _T("NULL");
					}
				}
				catch ( CNotSupportException& e )
				{
					e.OnCatch();
					s = _T("Unknown");
				}
				return s;
			}
		private:
			/**
			 * [取得] 文字列取得.
			 *	@param[out] _value 文字列が格納されます。
			 *	@retval true 成功。
			 *	@retval false 値をもっていない。
			 *	@throw CNotSupportException 型が違う(変換できない)。
			 */
			bool m_Get(CStr& _value) const
			{
				if ( length >= 0 )
				{
					if ( dataType == Type_TimeStamp )
					{
						CSystemTime st;
						Get(st);
						_value = SystemTimeToString(st);
						return true;
					}
					else if ( dataType == Type_Integer || dataType == Type_Short )
					{
						int v;
						Get(v);
						_value.Format(_T("%d"), v);
						return true;
					}
					else if ( dataType == Type_DoubleFloat || dataType == Type_Float )
					{
						double v;
						Get(v);
						_value.Format(_T("%.2f"), v);
					}
					else if ( dataType == Type_Binary )
					{
						_value = CStrOperator::BinaryToHexString(length, work.Ref());
						return true;
					}
					else if ( dataType == Type_Bit )
					{
						_value.Format(_T("%u"), work[0]);
						return true;
					}
					else if ( dataType == Type_AsciiString )
					{
						CAscii a;
						a.SetFromLeft(reinterpret_cast<LPCSTR>(work.Ref()), length);
						_value = a;
					}
					else if ( dataType == Type_UnicodeString )
					{
						CUnicode u;
						u.SetFromLeft(reinterpret_cast<LPCWSTR>(work.Ref()), length);
						_value = u;
					}
					else
					{
						_value.SetFromLeft(reinterpret_cast<LPCTSTR>(work.Ref()), length);
					}
					return true;
				}
				return false;
			}
		};

		/**
		 * カラム検索
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		INDEX m_Find(const CColumnIndex& index) const
		{
			if ( index.HasLabel() )
			{
				//== ラベル指定
				loop ( i, m_bindWorks.GetSize() )
				{
					if ( m_bindWorks[i].name.CompareNoCase(index.GetLabel()) == 0 )
					{
						return i;
					}
				}
				throw CNotFoundException();
			}
			//== 番号指定
			INDEX i = index.GetNumber();
			if ( m_bindWorks.GetSize() > i - 1 )
			{
				return i - 1;
			}
			throw CIndexOutOfBoundsException();
		}
		/**
		 * カラムチェック.
		 *	@throw CIndexOutOfBoundsException インデックス指定が異常。
		 *	@throw CNotFoundException 指定のラベルは見つからない。
		 */
		TBindWork& m_Check(const CColumnIndex& index)
		{
			INDEX i = m_Find(index);
			TBindWork& B = m_bindWorks[i];
			m_isLastNull = (B.length < 0); //マイナスは NULL ってこと
			return B;
		}
		// 戻り値処理
		bool m_Rst(SQLRETURN rc = 0, bool isSilent = false)
		{
			m_lastReturnCode = CReturnCode(SQL_HANDLE_STMT, m_hStmt, rc, isSilent);
			return CReturnCode::IsReturnCodeSuccess(rc);
		}
		bool							m_isBinding;		///< Bind済み？
		CStmtHandle						m_hStmt;			///< ステートメントハンドル
		CWorkMemT<TBindWork>			m_bindWorks;		///< バインドワーク
		bool							m_isLastNull;		///< 最後のGetの内容
		CReturnCode						m_lastReturnCode;	///< 最後の戻り値
		friend class CResultSetTest;
	};


	//-------------------------------


	/**@ingroup DBASE
	 * 簡易 SQL アクセサ用ステートメント.
	 *
	 *		CTinySqlAccessor::CreateStatement() の戻り値で作成できます（直接インスタンスを作成しても意味がありません）。
	 *		使用前に、 IsValid() で有効なインスタンスになっているか、確認してください。
	 *
	 *	@note コピーすると、同じデータベースへの接続を持つ新しいステートメントになります。
	 *
	 *	@see CTinySqlAccessor
	 *
	 *	@par必要ファイル
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	新規作成
	 *	@date 12/06/05	コピーコンストラクタ、コピーオペレータ、仕様変更。
	 */
	class CStatement
	{
	public:

		/**
		 * コンストラクタ.
		 *	@param hDbc コネクションハンドル
		 *	@param tm タイムアウト(s)
		 */
		CStatement(SQLHDBC hDbc, DWORD tm = 0) : m_hDbc(hDbc), m_timeout(tm)
		{
		}

		/**
		 * コピーコンストラクタ.
		 *	@param other コピー元
		 */
		CStatement(const CStatement& other) 
			: m_hDbc(other.m_hDbc), m_timeout(other.m_timeout), m_hStmt(other.m_hStmt), m_lastReturnCode(other.m_lastReturnCode)
		{
		}

		/**
		 * コピーオペレータ.
		 *	@param other コピー元
		 *	@return 自分の参照
		 */
		CStatement& operator=(const CStatement& other)
		{
			m_hDbc = other.m_hDbc;
			m_timeout = other.m_timeout;
			m_hStmt = other.m_hStmt;
			m_lastReturnCode = other.m_lastReturnCode;
			return *this;
		}

		/**
		 * [確認] 有効確認.
		 *	@retval true 有効なインスタンス。
		 *	@retval false 否。
		 */
		bool IsValid(void)
		{
			m_Init();
			return ! m_hStmt.IsNull();
		}

		/**
		 * [設定] 使用不可.
		 *		保持しているハンドルを破棄します。
		 */
		void Destroy(void)
		{
			m_hStmt = NULL;
		}

		/**
		 * [設定] 実行.
		 *		指定の SQL を実行します。結果セットは GetResultSet() で取得できます。
		 *	@param lpszSql SQL 文
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool Execute(LPCTSTR lpszSql)
		{
			m_Init();
			//== SQL文実行
			CStr sql = lpszSql;
			SQLRETURN r = ::SQLExecDirect(m_hStmt, GetSqlChar(sql), SQL_NTS);
			return m_Rst(r);
		}

		/**
		 * [取得] 結果セット取得.
		 *		Execute() の結果を管理している CResultSet を返します。
		 *	@return 結果セット。 CResultSet::IsValid() で正しい情報を持っているか、確認してください。
		 */
		CResultSet GetResultSet(void) const
		{
			return CResultSet(m_hStmt);
		}

		/**
		 * [取得] 戻り値詳細取得.
		 *	@return 戻り値.
		 */
		CReturnCode GetLastReturnCode(void) const
		{
			return m_lastReturnCode;
		}

		/**
		 * [取得] タイムアウト取得
		 *	@retval DWORD_MAX 取得エラー
		 *	@retval 上記以外 取得成功。値はQUERYタイムアウト値(s)。
		 */
		DWORD GetQueryTimeout(void)
		{
			SQLULEN tm;
			SQLRETURN r = ::SQLGetStmtAttr(m_hStmt, SQL_ATTR_QUERY_TIMEOUT, static_cast<SQLPOINTER>(&tm), SQL_IS_UINTEGER, NULL);
			if ( m_Rst(r) )
			{
				return static_cast<DWORD>(tm);
			}
			return DWORD_MAX;
		}

		/**
		 * [設定] クローズカーソル.
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		virtual bool CloseCursor(void) 
		{
			return m_Rst(::SQLCloseCursor(m_hStmt));
		}

	protected:

		/// [確認] 準備処理
		///	@retval true 成功
		/// @retval false 失敗
		bool m_Init(void)
		{
			if ( m_hStmt.IsNull() )
			{
				SQLHSTMT hStmt = NULL;
				SQLRETURN r = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &hStmt);
				m_lastReturnCode = CReturnCode(SQL_HANDLE_DBC, m_hDbc, r);
				if ( ! m_lastReturnCode )
				{
					return false;
				}
				m_hStmt = hStmt;
				SQLULEN tm = m_timeout;
				::SQLSetStmtAttr(m_hStmt, SQL_ATTR_QUERY_TIMEOUT, reinterpret_cast<SQLPOINTER>(tm), 0);
				ASSERT( GetQueryTimeout() == m_timeout );
			}
			return true;
		}
		/// [確認] 戻り値処理
		///	@param rc SQL APIの戻り値
		///	@retval true 成功
		/// @retval false 失敗
		bool m_Rst(SQLRETURN rc = 0)
		{
			m_lastReturnCode = CReturnCode(SQL_HANDLE_STMT, m_hStmt, rc);
			return CReturnCode::IsReturnCodeSuccess(rc);
		}
		SQLHDBC			m_hDbc;				///< DBC ハンドル
		CStmtHandle		m_hStmt;			///< ステートメントハンドル
		DWORD			m_timeout;			///< タイムアウト(s)
		CReturnCode		m_lastReturnCode;	///< 最後の戻り値
	};


	//---------------------------


	/**@ingroup DBASE
	 * 簡易 SQL アクセサ用プリペアドステートメント.
	 *
	 *		CTinySqlAccessor::PreparedStatement() の戻り値で作成できます（直接インスタンスを作成しても意味がありません）。
	 *		使用前に、 IsValid() で有効なインスタンスになっているか、確認してください。
	 *
	 *	@note コピーすると、同じデータベースへの接続を持つ新しいステートメントになります。
	 *		CTinySqlAccessor::PreparedStatement() で渡した SQL は引き継がれます。
	 *
	 *	@see CTinySqlAccessor
	 *
	 *	@todo DEFAULT のパラメータ指定するための、 SetDefault() が未実装です。
	 *			SQL に 'DEFAULT' 表記する必要があります。
	 *
	 *	@par必要ファイル
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	新規作成
	 *	@date 12/01/26	MSSQL 2008 を target に SetNull() 、実装。
	 *	@date 12/10/29	SetInt() で short に丸められるバグを修正。
	 */
	class CPreparedStatement : public CStatement
	{
		DEFSUPER(CStatement);
	public:

		/**
		 * コンストラクタ.
		 *	@param hDbc コネクションハンドル
		 *	@param lpszSql SQL 文
		 *	@param tm タイムアウト(s)
		 */
		CPreparedStatement(SQLHDBC hDbc, LPCTSTR lpszSql, DWORD tm = 0) : _super(hDbc, tm), m_sql(lpszSql), m_isPrepared(false)
		{
		}

		/**
		 * コピーコンストラクタ.
		 *	@param other コピー元
		 */
		CPreparedStatement(const CPreparedStatement& other) : _super(other), m_sql(other.m_sql), m_isPrepared(false)
		{
		}

		/**
		 * コピーオペレータ.
		 *	@param other コピー元
		 *	@return 自分の参照
		 */
		CPreparedStatement& operator=(const CPreparedStatement& other)
		{
			_super::operator=(other);
			m_sql = other.m_sql;			// SQL 文
			m_isPrepared = false;
			m_bindParams.RemoveAll();	// バインドワーク
			return *this;
		}

		/**
		 * [設定] 実行.
		 *		指定の SQL を実行します。結果セットは GetResultSet() で取得できます。
		 *	@note 通常、使いません。
		 *	@param lpszSql SQL 文
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool ExecuteDirect(LPCTSTR lpszSql)
		{
			return _super::Execute(lpszSql);
		}

		/**
		 * [設定] 実行.
		 *		SQL を実行します。結果セットは GetResultSet() で取得できます。
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool Execute(void)
		{
			if ( ! m_Prepare() )
			{
				return false;
			}
			SQLRETURN r = ::SQLExecute(m_hStmt);
			return m_Rst(r);
		}

		/**
		 * [設定] NULL 値設定.
		 *	@param parameterIndex パラメタインデックス。最初のパラメタは１、二番目が２になります。
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool SetNull(INDEX parameterIndex)
		{
			return m_BindParam(parameterIndex, SQL_C_NUMERIC, SQL_NUMERIC, SQL_NULL_DATA, NULL);
		}

		/**
		 * [設定] int 値設定.
		 *	@param parameterIndex パラメタインデックス。最初のパラメタは１、二番目が２になります。
		 *	@param value 値
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool SetInt(INDEX parameterIndex, int value)
		{
			return m_BindParam(parameterIndex, SQL_C_SLONG, SQL_INTEGER, sizeof(value), &value);
		}

		/**
		 * [設定] double 値設定.
		 *	@param parameterIndex パラメタインデックス。最初のパラメタは１、二番目が２になります。
		 *	@param value 値
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool SetDouble(INDEX parameterIndex, double value)
		{
			return m_BindParam(parameterIndex, SQL_C_DOUBLE, SQL_DOUBLE, sizeof(value), &value);
		}

		/**
		 * [設定] バイナリ設定.
		 *	@param parameterIndex パラメタインデックス。最初のパラメタは１、二番目が２になります。
		 *	@param size バイナリデータ長さ.(long で表現できる長さまで)
		 *	@param P バイナリデータ
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool SetBinary(INDEX parameterIndex, size_t size, LPCVOID P)
		{
			SQLLEN l = down_cast<SQLLEN>(size);
			if ( l == 0 )
			{
				return SetNull(parameterIndex);
			}
			return m_BindParam(parameterIndex, SQL_C_BINARY, SQL_BINARY, l, P);
		}

		/**
		 * [設定] 文字列設定.
		 *	@param parameterIndex パラメタインデックス。最初のパラメタは１、二番目が２になります。
		 *	@param lpsz 文字列。(long で表現できる長さまで)
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool SetString(INDEX parameterIndex, LPCSTR lpsz)
		{
			SQLLEN l = down_cast<SQLLEN>(STRLIB::GetLen(lpsz));
			if ( l == 0 )
			{
				return SetNull(parameterIndex);
			}
			return m_BindParam(parameterIndex, SQL_C_CHAR, SQL_CHAR, l, lpsz);
		}

		/**
		 * [設定] 文字列設定.
		 *	@note 文字列長が０の場合、 NULL を設定します。
		 *	@param parameterIndex パラメタインデックス。最初のパラメタは１、二番目が２になります。
		 *	@param lpsz 文字列。(long で表現できる長さまで)
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool SetString(INDEX parameterIndex, LPCWSTR lpsz)
		{
			SQLLEN l = down_cast<SQLLEN>(STRLIB::GetLen(lpsz) * 2);
			if( l == 0 )
			{
				return SetNull(parameterIndex);
			}
			return m_BindParam(parameterIndex, SQL_C_WCHAR, SQL_WCHAR, l, lpsz);
		}

		/**
		 * [設定] タイムスタンプ設定.
		 *	@param parameterIndex パラメタインデックス。最初のパラメタは１、二番目が２になります。
		 *	@param time 時間
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		bool SetTime(INDEX parameterIndex, const SYSTEMTIME& time)
		{
			TIMESTAMP_STRUCT ts;
			Zero(ts);
			ts.year = time.wYear;
			ts.month = time.wMonth;
			ts.day = time.wDay;
			ts.hour = time.wHour;
			ts.minute = time.wMinute;
			ts.second = time.wSecond;
			return m_BindParam(parameterIndex, SQL_C_TIMESTAMP, SQL_C_TIMESTAMP, sizeof(ts), &ts);
		}

//		bool SetDefault(INDEX parameterIndex)
//		{
//			return m_BindParam(parameterIndex, SQL_C_DEFAULT, SQL_BINARY, SQL_DEFAULT_PARAM, NULL);
//		}

		/**
		 * [設定] クローズカーソル.
		 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
		 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
		 */
		virtual bool CloseCursor(void) 
		{
			m_isPrepared = false;
			return _super::CloseCursor();
		}
	private:
		// 使用禁止宣言
		bool Execute(LPCTSTR lpszSql);
		// バインドワーク型
		struct TBindWork
		{
			CWorkMem			work;		///< Bind 領域
			SQLLEN				length;		///< Fetch 結果値
		};
		// プレペア処理
		bool m_Prepare(void)
		{
			m_Init();
			if ( ! m_isPrepared )
			{
				SQLRETURN r = ::SQLPrepare(m_hStmt, GetSqlChar(m_sql), SQL_NTS);
				m_sql.ReleaseBuffer();
				if ( ! m_Rst(r) )
				{
					return false;
				}
				m_isPrepared = true;
			}
			return true;
		}
		// バインド処理
		bool m_BindParam(INDEX parameterIndex, SQLSMALLINT fCType, SQLSMALLINT fSqlType, SQLLEN len, LPCVOID P)
		{
			if ( ! m_Prepare() )
			{
				return false;
			}
			if ( m_bindParams.GetSize() <= parameterIndex )
			{
				m_bindParams.SetSize(parameterIndex + 1);
			}
			TBindWork* W = m_bindParams.Get(parameterIndex);
			if ( W == NULL )
			{
				W = new TBindWork;
				m_bindParams.Set(parameterIndex, W);
			}
			else if ( W->length == len )
			{
				if ( len > 0 && ToInt(W->work.GetSize()) == len )
				{
					memcpy(W->work.Ref(), P, len);
				}
				return true;
			}
			W->length = len;
			SQLRETURN r;
			if ( fCType == SQL_C_TIMESTAMP )
			{
				W->work.Reset(len, static_cast<const BYTE*>(P));
				r = ::SQLBindParameter(m_hStmt, down_cast<SQLUSMALLINT>(parameterIndex)
						, SQL_PARAM_INPUT, fCType, fSqlType, 23/*TIMESTAMP_PRECISION*/, 0, W->work.Ref(), 0, &W->length);
			}
			else if ( len >= 0 )
			{
				W->work.Reset(len, static_cast<const BYTE*>(P));
				r = ::SQLBindParameter(m_hStmt, down_cast<SQLUSMALLINT>(parameterIndex)
						, SQL_PARAM_INPUT, fCType, fSqlType, len, 0, W->work.Ref(), len, &W->length);
			}
			else
			{
				W->work.Resize(1);
				r = ::SQLBindParameter(m_hStmt, down_cast<SQLUSMALLINT>(parameterIndex)
						, SQL_PARAM_INPUT, fCType, fSqlType, 13, 6, NULL, 0, &W->length);
			}
			return m_Rst(r);
		}
		CStr						m_sql;			///< SQL 文
		bool						m_isPrepared;	///< プリペア済み？
		CPointerVectorT<TBindWork>	m_bindParams;	///< バインドワーク
	};


	//-------------------------------


	/**
	 * デストラクタ.
	 *	@note デストラクタ後、本クラスから生成したクラスのインスタンスも正しく動かなくなります。
	 */
	~CTinySqlAccessor(void)
	{
		Stop();
	}

	/**
	 * コンストラクタ.
	 */
	CTinySqlAccessor(void) : m_hEnv(NULL), m_hDbc(NULL), m_isConnecting(false), m_timeout(0)
	{
	}

	/**
	 * [確認] 開始している？.
	 *		Start() か Connect() を実行すると、SQLアクセス準備が開始されます。
	 *	@retval true 開始している
	 *	@retval false していない
	 */
	bool IsStarted(void) const
	{
		return m_hEnv != NULL;
	}

	/**
	 * [設定] 停止.
	 *		Start() か Connect() を実行すると、SQLアクセス準備が開始されます。
	 *		このメソッドをコールすると、停止します。
	 *		また、接続されていた場合、切断も行います。
	 *	@note 停止すると、本クラスから生成したクラスのインスタンスも正しく動かなくなります。
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Stop(void)
	{
		if ( IsStarted() )
		{
			Disconnect();
			// DB接続用ハンドル開放
			SQLRETURN r = ::SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
			if ( ! m_Rst(SQL_HANDLE_DBC, m_hDbc, r) )
			{
				return false;
			}
			m_hDbc = NULL;
			// 環境用のハンドル開放
			r = ::SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
			if ( ! m_Rst(SQL_HANDLE_ENV, m_hEnv, r) )
			{
				return false;
			}
			m_hEnv = NULL;
		}
		return m_Rst();
	}

	/**
	 * [設定] 開始.
	 *	@note 本メソッドを使わなくても、 Connect() を使うことで、自動的にSQLアクセス準備が開始されます。
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Start(void)
	{
		Stop();
		//== ODBC環境ハンドル取得
		SQLRETURN r = ::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
		if ( ! m_CheckReturnCode(r) )
		{
			m_hEnv = NULL;
		}
		else
		{
			//== ODBCバージョン設定
			r = ::SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
			if ( m_CheckReturnCode(r) )
			{
				//== ODBCデータベースハンドル取得
				r = ::SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
			}
			if ( ! m_CheckReturnCode(r) )
			{
				::SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
				m_hEnv = NULL;
			}
		}
		return m_Rst(SQL_HANDLE_ENV, m_hEnv, r);
	}

	/**
	 * [確認] 接続中か？
	 *	@retval true 接続中。
	 *	@retval false 否。
	 */
	bool IsConnecting(void) const
	{
		return m_isConnecting;
	}

	/**
	 * [設定] 切断.
	 *		Connect() で行った接続を切断します。
	 *	@note 切断前にコミットしますので、変更を無効にしたい場合、先に Rollback() をコールする必要があります。
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Disconnect(void)
	{
		if ( ! IsConnecting() )
		{
			return m_Rst();
		}
		// コミット
		SQLRETURN r = ::SQLEndTran(SQL_HANDLE_DBC, m_hDbc, SQL_COMMIT);
		// DB ディスコネクト
		r = ::SQLDisconnect(m_hDbc);
		if ( m_Rst(SQL_HANDLE_DBC, m_hDbc, r) )
		{
			m_isConnecting = false;
			return true;
		}
		return false;
	}

	/**
	 * [設定] 接続.
	 *		指定の設定で、データベースと接続します。
	 *	@note Start() していない場合、スタートします。
	 *	@param lpszDataBaseName データベース名.
	 *	@param lpszUserName ユーザー名
	 *	@param lpszPassword パスワード
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Connect(LPCTSTR lpszDataBaseName, LPCTSTR lpszUserName, LPCTSTR lpszPassword)
	{
		if ( m_PreConnect() )
		{
			//== データベース接続
			CStr dn = lpszDataBaseName;
			CStr un = lpszUserName;
			CStr pw = lpszPassword;
			SQLRETURN r = ::SQLConnect(m_hDbc, GetSqlChar(dn), SQL_NTS, GetSqlChar(un), SQL_NTS, GetSqlChar(pw), SQL_NTS);
			if ( m_Rst(SQL_HANDLE_DBC, m_hDbc, r) )
			{
				m_isConnecting = true;
				return true;
			}
		}
		return false;
	}

	/**
	 * [設定] 接続.
	 *		指定の設定で、データベースと接続します。
	 *	@note Start() していない場合、スタートします。
	 *	@param lpszConnect 接続名.
	 *	@param options オプション.
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Connect(LPCTSTR lpszConnect, SQLUSMALLINT options = SQL_DRIVER_NOPROMPT)
	{
		if ( m_PreConnect() )
		{
			CStr cs = lpszConnect;
			//== データベース接続
			SQLHWND hWnd = ::GetDesktopWindow();
			CStr n;
			SQLSMALLINT length = 0;
			SQLRETURN r = ::SQLDriverConnect(m_hDbc, hWnd, GetSqlChar(cs), SQL_NTS, GetSqlChar(n, 256), 256, &length, options);
			if ( m_Rst(SQL_HANDLE_DBC, m_hDbc, r) )
			{
				m_isConnecting = true;
				return true;
			}
		}
		return false;
	}

	/**
	 * [設定] 接続.
	 *		指定の設定で、データベースと接続します。
	 *	@note Start() していない場合、スタートします。
	 *	@param lpszDriverName ドライバー名.
	 *	@param lpszDataBaseName データベース名.
	 *	@param lpszUserName ユーザー名
	 *	@param lpszPassword パスワード
	 *	@param options オプション.
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Connect(LPCTSTR lpszDriverName, LPCTSTR lpszDataBaseName, LPCTSTR lpszUserName, LPCTSTR lpszPassword, SQLUSMALLINT options = SQL_DRIVER_NOPROMPT)
	{
		//== 名前作成
		CStr cs;
		cs.Format(_T("DRIVER={%s};Database=%s;UID=%s;PWD=%s;"), lpszDriverName, lpszDataBaseName, lpszUserName, lpszPassword);
		return Connect(cs, options);
	}

	/**
	 * [設定] タイムアウト設定.
	 *	@param tm QUERYタイムアウト値(s)
	 */
	void SetQueryTimeout(DWORD tm)
	{
		m_timeout = tm;
	}

	/**
	 * [作成] ステートメント作成.
	 *	@return ステートメント。 CStatement::IsValid() で成功の有無がわかります。
	 */
	CStatement CreateStatement(void)
	{
		return CStatement(m_hDbc, m_timeout);
	}

	/**
	 * [作成] プリペアドステートメント作成.
	 *	@param lpszSql SQL 文
	 *	@return プリペアド ステートメント。 CStatement::IsValid() で成功の有無がわかります。
	 */
	CPreparedStatement PreparedStatement(LPCTSTR lpszSql)
	{
		return CPreparedStatement(m_hDbc, lpszSql, m_timeout);
	}

	/**
	 * [取得] 自動コミット状態確認
	 *	@note Start() か Connect() している必要があります。
	 *	@retval true 自動コミット状態が有効。
	 *	@retval false 自動コミット状態が無効、か、エラーです。エラーか否かは GetLastReturnCode() で得られます。
	 */
	bool IsAutoCommit(void)
	{
		DWORD l = DWORD_MAX;
		SQLINTEGER len = 0;
		SQLRETURN r = ::SQLGetConnectAttr(m_hDbc, SQL_AUTOCOMMIT, &l, sizeof(l), &len);
		if ( m_Rst(SQL_HANDLE_DBC, m_hDbc, r) )
		{
			return (l == SQL_AUTOCOMMIT_ON);
		}
		return false;
	}

	/**
	 * [設定] 自動コミット設定.
	 *	@note Start() か Connect() している必要があります。
	 *	@param isEnable true なら自動コミットを有効します。 false なら無効にします。
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool SetAutoCommit(bool isEnable)
	{
		DWORD l = isEnable ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF;
		SQLRETURN r = ::SQLSetConnectAttr(m_hDbc, SQL_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(l)), 0);
		return m_Rst(SQL_HANDLE_DBC, m_hDbc, r);
	}
	
	/**
	 * [設定] コミット.
	 *	@note Start() か Connect() している必要があります。
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Commit(void)
	{
		SQLRETURN r = ::SQLEndTran(SQL_HANDLE_DBC, m_hDbc, SQL_COMMIT);
		return m_Rst(SQL_HANDLE_DBC, m_hDbc, r);
	}

	/**
	 * [設定] ロールバック.
	 *	@note Start() か Connect() している必要があります。
	 *	@retval true 成功。詳細は GetLastReturnCode() で得られます。
	 *	@retval false 失敗。詳細は GetLastReturnCode() で得られます。
	 */
	bool Rollback(void)
	{
		SQLRETURN r = ::SQLEndTran(SQL_HANDLE_DBC, m_hDbc, SQL_ROLLBACK);
		return m_Rst(SQL_HANDLE_DBC, m_hDbc, r);
	}

	/**
	 * [取得] 戻り値詳細取得.
	 *	@return 戻り値.
	 */
	CReturnCode GetLastReturnCode(void) const
	{
		return m_lastReturnCode;
	}

	/**
	 * [変換] 時間文字列化.
	 *		SYSTEMTIME を Timestamp で使用する文字列(YYYY-MM-DD HH:MM:SS)に変換します。
	 *	@param time 時間.
	 *	@return 文字列。
 	 */
	static CStr SystemTimeToString(const SYSTEMTIME& time)
	{
		return CStr::Fmt(_T("%4d-%02d-%02d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	}


	//----------------


	/**
	 * [変換] 文字列ポインタ取得.
	 *		CStr から SQLTCHAR のポインタを取得します( CStr::GetBuffer() を使用 )。
	 *	@note 通常使う必要ありません。
	 *	@param[in,out] _s 対象。
	 *	@param[in] len バッファサイズ。
	 *	@return 文字列ポインタ。 使用後、 _s に対し CStr::ReleaseBuffer() を使用してください。
 	 */
	static SQLTCHAR* GetSqlChar(CStr& _s, size_t len = 0)
	{
		#ifndef _UNICODE
			return reinterpret_cast<SQLCHAR*>(_s.GetBuffer(len));
		#else
			return _s.GetBuffer(len);
		#endif
	}

private:
	/// 接続
	bool m_PreConnect(void)
	{
		if ( ! IsStarted() && ! Start() )
		{
			return false;
		}
		return Disconnect();
	}
	// 戻り値チェック
	bool m_CheckReturnCode(SQLRETURN r) const
	{
		return CReturnCode::IsReturnCodeSuccess(r);
	}
	// 戻り値処理
	bool m_Rst(SQLSMALLINT type = 0, SQLHANDLE handle = NULL, SQLRETURN rc = 0)
	{
		m_lastReturnCode = CReturnCode(type, handle, rc);
		return m_CheckReturnCode(rc);
	}

	SQLHENV		m_hEnv;				///< 環境ハンドル
	SQLHDBC		m_hDbc;				///< DBC ハンドル
	DWORD		m_timeout;			///< タイムアウト(s)
	bool		m_isConnecting;		///< 接続中
	CReturnCode	m_lastReturnCode;	///< 最後の戻り値
};



}; // TNB


