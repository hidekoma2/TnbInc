#pragma once
/**
 *@file
 * �Ȉ� SQL �A�N�Z�X�֌W�̃w�b�_
 *
 *		SQL �A�N�Z�X���邽�߂̃N���X�Q������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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
 #include "minodbc.h" // �v Mimer SDK�B
 #pragma comment(lib, "mimodbcp.lib")
 #pragma comment(lib, "minodbcw.lib")
 #pragma comment(lib, "minsql32.lib")
#endif



namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// PointerHandleBase�p SQLHSTMT Free�N���X
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
			ASSERT0( false, "", "��� CTinySqlAccessor �� Disconnect ���Ă��邩 Stop ���Ă��邩�j������Ă��܂��B" );
		}
	}
};

#endif //_TnbDOXYGEN



/**@ingroup DBASE
 * �Ȉ� SQL �A�N�Z�T�N���X
 *
 *		�f�[�^�x�[�X�ɐڑ����A SQL �����s���邱�Ƃ��o���܂��B
 *		�߂�l���擾���邱�Ƃ��o���܂��B
 *
 *	@par�g�p��(SELECT)
 *	\code
 *		CTinySqlAccessor sql;
 *		// test-db �Ƃ����f�[�^�x�[�X�ɐڑ�
 *		bool r = sql.Connect(_T("test-db"),_T("user"),_T("pass"));
 *		CTinySqlAccessor::CReturnCode rc = sql.GetLastReturnCode();
 *		if ( r ) {
 *			// SQL �������s
 *			CTinySqlAccessor::CStatement stmt = sql.CreateStatement();
 *			rc = sql.GetLastReturnCode();
 *			if ( stmt.IsValid() ){
 *				r = stmt.Execute(_T("SELECT * FROM Table_1"));
 *				rc = stmt.GetLastReturnCode();
 *				if ( r ) {
 *					// ���ʃZ�b�g���擾
 *					CTinySqlAccessor::CResultSet rs = stmt.GetResultSet();
 *					ASSERT( rs.IsValid() );
 *					INDEX idx = rs.FindColumn(_T("SENSOR_TIME")); // SENSOR_TIME �Ƃ������x���̃J�����̃C���f�b�N�X���擾.
 *					while ( rs.Next() ) {
 *						int s1 = rs.GetInt(_T("SENSOR_ID");	// SENSOR_ID �Ƃ������x���̃J�������琔�l�擾
 *						CStr tm = rs.GetString(idx);		// SENSOR_TIME �Ƃ������x���̃J�������玞�ԏ��𕶎���Ŏ擾
 *						CStr s2 = rs.GetString(4);			// �J�����S���當����擾
 *						if ( rs.WasNull() ) {
 *							//�J�����S�� NULL ���I
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
 *	@par�g�p��(INSERT)
 *	\code
 *		CTinySqlAccessor sql;
 *		// test-db �Ƃ����f�[�^�x�[�X�ɐڑ�
 *		bool r = sql.Connect(_T("test-db"),_T("user"),_T("pass"));
 *		CTinySqlAccessor::CReturnCode rc = sql.GetLastReturnCode();
 *		sql.SetAutoCommit(false); //�����R�~�b�g����
 *		if ( r ) {
 *			// SQL �������s
 *			CTinySqlAccessor::CPreparedStatement stmt = sql.PreparedStatement(_T("INSERT INTO Table_1 VALUES(?,?)"));
 *			rc = sql.GetLastReturnCode();
 *			if ( stmt.IsValid() ){
 *				stmt.SetString(2, _T("ABC"));	// ��ڂ� �H �� 'ABC'
 *				loop ( i, 2 ) {
 *					stmt.SetInt(1, 100 + 100);		// ��ڂ� �H �� 100 + i
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
 *		@note Start�AStop���� ���n���h��(SQLHENV), DBC�n���h��(SQLHDBC) ���\�z�A�j�����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTinySqlAccessor.h
 * 
 *	@date 11/05/16	�V�K�쐬
 *	@date 12/05/02	�Ǎ����̌^�`�F�b�N�������B
 *	@date 12/06/06	Type_Bit �ɑΉ��B
 *	@date 12/10/17	SetQueryTimeout() �V�K�B
 */
class CTinySqlAccessor : CCopyImpossible
{
public:

	/// �l���
	enum EType
	{
		Type_Null			= SQL_TYPE_NULL,		///< �l�Ȃ�
		Type_Binary			= SQL_BINARY,			///< �o�C�i��
		Type_AsciiString	= SQL_CHAR,				///< ASCII������
		Type_UnicodeString	= SQL_WCHAR,			///< UNICODE������
		Type_DoubleFloat	= SQL_DOUBLE,			///< 64bit double float ����
		Type_Float			= SQL_FLOAT,			///< 32bit float ����
		Type_Integer		= SQL_INTEGER,			///< 32bit signed ����
		Type_Short			= SQL_SMALLINT,			///< 16bit signed ����
		Type_TimeStamp		= SQL_TYPE_TIMESTAMP,	///< �^�C���X�^���v
		Type_Bit			= SQL_BIT				///< 1bit �l	
	};

	/**@ingroup DBASE
	 * �Ȉ� SQL �A�N�Z�T�p�߂�l�Ǘ�.
	 *
	 *		CTinySqlAccessor �̃��\�b�h�̖߂�l�ł��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	�V�K�쐬
	 */
	class CReturnCode
	{
	public:

		/**
		 * �R�[�h�Q.
		 *		GetDetialCode() �̖߂�l�Ŏg�p����܂��B
		 */
		struct TCodes
		{
			CStr		sqlState;		///< ���
			SQLINTEGER	nativeError;	///< �G���[
			CStr		errorMessage;	///< �G���[���b�Z�[�W
			SQLINTEGER	plm_Rownumber;	///< �H
			/// �R���X�g���N�^
			TCodes(void) : nativeError(0), plm_Rownumber(0)
			{
			}
		};

		/// �R�[�h�Q�z��Ǘ�
		typedef CVectorT<TCodes> CCodeVector;

		/**
		 * �R���X�g���N�^.
		 *		�u�����v�̏�Ԃ����C���X�^���X�ɂȂ�܂��B
		 */
		CReturnCode(void) : m_type(0), m_handle(0), m_returnCode(SQL_SUCCESS)
		{
		}

		/**
		 * �R���X�g���N�^.
		 *		�w��̏�Ԃ����C���X�^���X�ɂȂ�܂��B
		 *	@param type �n���h���^�C�v.
		 *	@param handle �n���h��
		 *	@param rc SQL API ����̃��^�[���R�[�h.
		 *	@param isSilent true �Ȃ�f�o�b�O�\�������܂���B
		 */
		CReturnCode(SQLSMALLINT type, SQLHANDLE handle, SQLRETURN rc, bool isSilent = false) : m_type(type), m_handle(handle), m_returnCode(rc)
		{
			if ( ! isSilent )
			{
				Dump();
			}
		}

		/**
		 * [�擾] �߂�l�擾.
		 *		SQL API ����̖߂�l���̂��̂�Ԃ��܂��B
		 *	@retval SQL_SUCCESS �����B
		 *	@retval SQL_SUCCESS_WITH_INFO �����B
		 *	@retval SQL_ERROR ���s�B GetDetialCode() �ŏڍׂ��擾�ł��܂��B
		 *	@retval SQL_INVALID_HANDLE �n���h���s��
		 *	@retval ���̑� ���̑��̃G���[�B
		 */
		SQLRETURN GetReturnCode(void) const
		{
			return m_returnCode;
		}

		/**
		 * [�m�F] �������H
		 *	@retval true ����.
		 *	@retval false �G���[
		 */
		operator bool(void) const
		{
			return ! HasError();
		}

		/**
		 * [�m�F] ���s���H
		 *	@retval false ����.
		 *	@retval true �G���[
		 */
		bool HasError(void) const
		{
			return (m_returnCode != SQL_SUCCESS && m_returnCode != SQL_SUCCESS_WITH_INFO);
		}

		/**
		 * [�擾] �G���[������擾.
		 *	@note �{�C���X�^���X���u�����v�̏ꍇ�AEmpty �����񂪕Ԃ�܂��B
		 *	@return �G���[������B
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
		 * [�擾] �ڍ׏��擾.
		 *	@param withDiagnostics true �Ȃ�f�f�������Ȃ��B default �� false�B
		 *	@return �ڍ׏��z��B
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
		 * [�m�F] �����m�F.
		 *	@note SQL API ����̖߂�l�̔��f�����܂��B
		 *	@param r SQL API ����̖߂�l.
		 *	@retval true ����.
		 *	@retval false �G���[
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
		 * [�o��] �_���v.
		 *		�C���X�^���X���e�� Output�ɏo�͂��܂��B
		 *	@note �f�o�b�O�p���\�b�h�ł��B�����[�X�r���h�ł͉������܂���B
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
		/// �f�f���擾
		SQLRETURN m_GetDiagField(SQLSMALLINT iRecord, SQLSMALLINT fDiagField, SQLPOINTER rgbDiagInfo, SQLSMALLINT cbDiagInfoMax, SQLSMALLINT* pcbDiagInfo = NULL) const
		{
			return ::SQLGetDiagField(m_type, m_handle, iRecord, fDiagField, rgbDiagInfo, cbDiagInfoMax, pcbDiagInfo);
		}
		SQLSMALLINT		m_type;			///< �n���h���^�C�v
		SQLHANDLE		m_handle;		///< �n���h��
		SQLRETURN		m_returnCode;	///< �߂�l
	};


	//-----------------------------


	/**@ingroup DBASE
	 * �Ȉ� SQL �A�N�Z�T�p STMT �n���h���n���h��.
	 *		SQLHSTMT �̊Ǘ�����n���h���N���X�ł��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	�V�K�쐬
	 */
	typedef TNB::CPointerHandleBaseT<SQLHSTMT, TPhFreeStmtHandle> CStmtHandle;


	//-----------------------------


	/**@ingroup DBASE
	 * �Ȉ� SQL �A�N�Z�T�p�J�����C���f�b�N�X.
	 *
	 *		CResultSet �N���X�̃Q�b�^�[���\�b�h�Ȃǂ̃C���f�b�N�X�w��Ɏg�p���܂��B
	 *		�ʏ�ӎ����ăC���X�^���X���쐬���܂���B
	 *
	 *	\code
	 *		CTinySqlAccessor sql;
	 *		sql.Connect(_T("test-db"),_T("user"),_T("pass"));
	 *		CTinySqlAccessor::CStatement stmt = sql.CreateStatement();
	 *		stmt.execute(_T("SELECT �`"));
	 *		CTinySqlAccessor::CResultSet rs = stmt.GetResultSet();
	 *
	 *		int s1 = rs.GetInt(_T("SENSOR_ID");	//�����ňÖٓI�� CColumnIndex ���g�p�����
	 *		CStr s2 = rs.GetString(4);			//			�V
	 *	\endcode
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	�V�K�쐬
	 */
	class CColumnIndex
	{
	public:

		/**
		 * �R���X�g���N�^.
		 *	@param number �ŏ��̗�͂P�A��Ԗڂ͂Q���w�肵�܂��B
		 */
		CColumnIndex(INDEX number) : m_number(number)
		{
		}

		/**
		 * �R���X�g���N�^.
		 *	@param lpszLabel ���x�������w�肵�܂��B�B
		 */
		CColumnIndex(LPCTSTR lpszLabel) : m_label(lpszLabel), m_number(0)
		{
		}

		/**
		 * [�m�F] ���x���w�肩�H
		 *	@retval true ���x���w��.
		 *	@retval false �ԍ��w��
		 */
		bool HasLabel(void) const
		{
			return ! m_label.IsEmpty();
		}

		/**
		 * [�擾] ���x��.
		 *	@return ���x��.
		 */
		LPCTSTR GetLabel(void) const
		{
			return m_label;
		}

		/**
		 * [�擾] �ԍ�.
		 *	@return �ԍ�.
		 */
		INDEX GetNumber(void) const
		{
			return m_number;
		}

	private:
		INDEX	m_number;	///< �ԍ�
		CStr	m_label;	///< ���x��
	};


	//-----------------------------


	/**@ingroup DBASE
	 * �Ȉ� SQL �A�N�Z�T�p���ʃZ�b�g�Ǘ�.
	 *
	 *		CTinySqlAccessor::CStatement::GetResultSet() �Ȃǂ̖߂�l�Ŏg���܂��B
	 *		�g�p�O�ɁA IsValid() �Ő��������������Ă��邩�A�m�F���Ă��������B
	 *
	 *	@see CTinySqlAccessor
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	�V�K�쐬
	 *	@date 12/01/26	�l�擾�� SQLGetData() ���g�p����悤�ɕύX�B
	 */
	class CResultSet
	{
	public:

		/// �J�������
		struct TColumnInformation
		{
			CStr		name;				///< ����
			SQLSMALLINT dataType;			///< �^
			SQLULEN		size;				///< �^�T�C�Y
			SQLSMALLINT decimalDigits;		///< �����_���
			bool		isNullable;			///< Null���L�����H
		};

		/**
		 * �R���X�g���N�^.
		 *	@param hStmt SQL �X�e�[�g�����g�n���h�� 
		 */
		CResultSet(CStmtHandle hStmt) : m_hStmt(hStmt), m_isBinding(false), m_isLastNull(false)
		{
		}

		/**
		 * �R�s�[�R���X�g���N�^.
		 *	@param other �R�s�[�� 
		 */
		CResultSet(const CResultSet& other) : m_hStmt(other.m_hStmt), m_isBinding(false), m_isLastNull(false)
		{
		}

		/**
		 * �R�s�[�I�y���[�^.
		 *	@param other �R�s�[��
		 *	@return �����̎Q��.
		 */
		CResultSet& operator=(const CResultSet& other)
		{
			m_hStmt = other.m_hStmt;
			m_isBinding = false;
			m_isLastNull = false;
			return *this;
		}

		/**
		 * [�m�F] ���������H
		 *	@retval true ���������������Ă���
		 *	@retval false �ہB
		 */
		bool IsValid(void) const
		{
			return ! m_hStmt.IsNull();
		}

		/**
		 * [�ݒ�] �g�p�s��.
		 *		�ێ����Ă���n���h����j�����܂��B
		 */
		void Destroy(void)
		{
			m_hStmt = NULL;
		}

		/**
		 * [�ݒ�] ���ǂݏo��.
		 *	@note ������Ԃł́A�ŏ��̍s�̑O��}���Ă����ԂɂȂ��Ă��܂��B�K���ŏ��ɖ{���\�b�h��ǂ�ł���Q�b�^�[���g�p���Ă��������B
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
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
		 * [�m�F] NULL�m�F.
		 *		�Ō�ɃQ�b�g�����l���A NULL �ł��������ǂ����A���f���܂��B
		 *	@retval true NULL�������B
		 *	@retval false �ہB
		 */
		bool WasNull(void) const
		{
			return m_isLastNull;
		}

		/**
		 * [�擾] ������擾.
		 *	@param index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@return �l�B �l�� NULL �̏ꍇ�A Empty �����񂪕Ԃ���A����� WasNull() �� true ���Ԃ�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		CStr GetString(CColumnIndex index)
		{
			CStr r;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [�擾] ������擾.
		 *	@param index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@return �l�B �l�� NULL �̏ꍇ�A Empty �����񂪕Ԃ���A����� WasNull() �� true ���Ԃ�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		CUnicode GetUnicodeString(CColumnIndex index)
		{
			CUnicode r;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [�擾] ������擾.
		 *	@param index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@return �l�B �l�� NULL �̏ꍇ�A Empty �����񂪕Ԃ���A����� WasNull() �� true ���Ԃ�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		CAscii GetAsciiString(CColumnIndex index)
		{
			CAscii r;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [�擾] double �l�擾.
		 *	@param index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@return �l�B �l�� NULL �̏ꍇ�A 0.0 ���Ԃ���A����� WasNull() �� true ���Ԃ�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		double GetDouble(CColumnIndex index)
		{
			double r = 0.0;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [�擾] Float �l�擾.
		 *	@param index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@return �l�B �l�� NULL �̏ꍇ�A 0.0 ���Ԃ���A����� WasNull() �� true ���Ԃ�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		float GetFloat(CColumnIndex index)
		{
			float r = 0.0;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [�擾] int �l�擾.
		 *	@param index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@return �l�B �l�� NULL �̏ꍇ�A 0 ���Ԃ���A����� WasNull() �� true ���Ԃ�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		int GetInt(CColumnIndex index)
		{
			int r = 0;
			m_Check(index).Get(r);
			return r;
		}

		/**
		 * [�擾] �^�C���X�^���v�l�擾.
		 *	@param[out] _v �l���i�[����܂��B �l�� NULL �̏ꍇ�A�S�� �O�ɂȂ�A����� WasNull() �� true ���Ԃ�܂��B
		 *	@param[in] index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		void GetTime(SYSTEMTIME& _v, CColumnIndex index)
		{
			Zero(_v);
			m_Check(index).Get(_v);
		}

		/**
		 * [�擾] �o�C�i���f�[�^�擾.
		 *	@param[out] _v �l���i�[����܂��B �l�� NULL �̏ꍇ�A�T�C�Y���O�ɂȂ�A����� WasNull() �� true ���Ԃ�܂��B
		 *	@param[in] index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		void GetBinary(CWorkMem& _v, CColumnIndex index)
		{
			_v.Resize(0);
			TBindWork& B = m_Check(index);
			_v.Reset(B.length, B.work.Ref());
		}

		/**
		 * [�擾] �J�������擾.
		 *	@param index �J�����C���f�b�N�X�B���l�ł����x�����ł��w��\�B���l�̏ꍇ�A��Ԗڂ͂P�A��Ԗڂ͂Q�E�E�E���w�肵�܂��B
		 *	@return �J�������
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		const TColumnInformation& GetColumnInformation(CColumnIndex index)
		{
			Binding();
			return m_bindWorks[m_Find(index)];
		}

		/**
		 * [�擾] ���x��������.
		 *		�w��̃��x���̃J�����C���f�b�N�X(���l)���������܂��B
		 *	@param lpszLabel ���x����
		 *	@retval 0 ������Ȃ������B
		 *	@retval 1�ȏ� �J�����C���f�b�N�X(���l)�B
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
		 * [�擾] �J�������擾.
		 *	@note Next() ����x�ł��R�[�����Ȃ��ƁA�������l���Ԃ�܂���B
		 *	@return �J������.
		 */
		size_t GetColumnCount(void) const
		{
			return m_bindWorks.GetSize();
		}

		/**
		 * [�擾] �߂�l�ڍ׎擾.
		 *	@return �߂�l.
		 */
		CReturnCode GetLastReturnCode(void) const
		{
			return m_lastReturnCode;
		}

	protected:
		/// �o�C���h(���݂̓J�������擾�̂�)
		bool Binding(void)
		{
			if ( m_isBinding )
			{
				return m_Rst();
			}
			//== �J�������擾
			SQLSMALLINT columnCount;
			SQLRETURN r = ::SQLNumResultCols(m_hStmt, &columnCount);
			if ( ! m_Rst(r, true) )
			{
				return false;
			}
			//== �J�������擾
			m_bindWorks.Resize(columnCount);
			loop ( i, columnCount )
			{
				SQLSMALLINT nameLen;
				TBindWork& b = m_bindWorks[i];
				SQLSMALLINT nullable;
				r = ::SQLDescribeCol(m_hStmt,					// [in] �X�e�[�g�����g
								down_cast<SQLUSMALLINT>(i + 1),	// [in] �J�����ԍ�(�P�`)
								GetSqlChar(b.name, 100),		// [out] �J������
								100,							// [in] �J�������̈�
								&nameLen,						// [out] �J��������
								&b.dataType,					// [out] �^
								&b.size,						// [out] �^�T�C�Y
								&b.decimalDigits,				// [out] �����_���
								&nullable);						// [out] NULL���L����
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
		/// �o�C���h�p���[�N.
		struct TBindWork : TColumnInformation
		{
			CWorkMem			work;		///< Bind �̈�
			SQLLEN				length;		///< Fetch ���ʒl
			/**
			 * [�擾] ������擾.
			 *	@param[out] _value �����񂪊i�[����܂��B
			 *	@retval true �����B
			 *	@retval false �l�������Ă��Ȃ��B
			 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
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
			 * [�擾] ������擾.
			 *	@param[out] _value �����񂪊i�[����܂��B
			 *	@retval true �����B
			 *	@retval false �l�������Ă��Ȃ��B
			 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
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
			 * [�擾] double float �l�擾.
			 *	@param[out] _value �l���i�[����܂��B
			 *	@retval true �����B
			 *	@retval false �l�������Ă��Ȃ��B
			 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
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
			 * [�擾] float �l�擾.
			 *	@param[out] _value �l���i�[����܂��B
			 *	@retval true �����B
			 *	@retval false �l�������Ă��Ȃ��B
			 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
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
			 * [�擾] integer �l�擾.
			 *	@param[out] _value �l���i�[����܂��B
			 *	@retval true �����B
			 *	@retval false �l�������Ă��Ȃ��B
			 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
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
			 * [�擾] �^�C���擾.
			 *	@param[out] _time �l���i�[����܂��B
			 *	@retval true �����B
			 *	@retval false �l�������Ă��Ȃ��B
			 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
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
			 * [�擾] ������
			 *	@note �����Ă�����𕶎��񉻂��܂��B
			 *	@return ������
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
			 * [�擾] ������擾.
			 *	@param[out] _value �����񂪊i�[����܂��B
			 *	@retval true �����B
			 *	@retval false �l�������Ă��Ȃ��B
			 *	@throw CNotSupportException �^���Ⴄ(�ϊ��ł��Ȃ�)�B
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
		 * �J��������
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		INDEX m_Find(const CColumnIndex& index) const
		{
			if ( index.HasLabel() )
			{
				//== ���x���w��
				loop ( i, m_bindWorks.GetSize() )
				{
					if ( m_bindWorks[i].name.CompareNoCase(index.GetLabel()) == 0 )
					{
						return i;
					}
				}
				throw CNotFoundException();
			}
			//== �ԍ��w��
			INDEX i = index.GetNumber();
			if ( m_bindWorks.GetSize() > i - 1 )
			{
				return i - 1;
			}
			throw CIndexOutOfBoundsException();
		}
		/**
		 * �J�����`�F�b�N.
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X�w�肪�ُ�B
		 *	@throw CNotFoundException �w��̃��x���͌�����Ȃ��B
		 */
		TBindWork& m_Check(const CColumnIndex& index)
		{
			INDEX i = m_Find(index);
			TBindWork& B = m_bindWorks[i];
			m_isLastNull = (B.length < 0); //�}�C�i�X�� NULL ���Ă���
			return B;
		}
		// �߂�l����
		bool m_Rst(SQLRETURN rc = 0, bool isSilent = false)
		{
			m_lastReturnCode = CReturnCode(SQL_HANDLE_STMT, m_hStmt, rc, isSilent);
			return CReturnCode::IsReturnCodeSuccess(rc);
		}
		bool							m_isBinding;		///< Bind�ς݁H
		CStmtHandle						m_hStmt;			///< �X�e�[�g�����g�n���h��
		CWorkMemT<TBindWork>			m_bindWorks;		///< �o�C���h���[�N
		bool							m_isLastNull;		///< �Ō��Get�̓��e
		CReturnCode						m_lastReturnCode;	///< �Ō�̖߂�l
		friend class CResultSetTest;
	};


	//-------------------------------


	/**@ingroup DBASE
	 * �Ȉ� SQL �A�N�Z�T�p�X�e�[�g�����g.
	 *
	 *		CTinySqlAccessor::CreateStatement() �̖߂�l�ō쐬�ł��܂��i���ڃC���X�^���X���쐬���Ă��Ӗ�������܂���j�B
	 *		�g�p�O�ɁA IsValid() �ŗL���ȃC���X�^���X�ɂȂ��Ă��邩�A�m�F���Ă��������B
	 *
	 *	@note �R�s�[����ƁA�����f�[�^�x�[�X�ւ̐ڑ������V�����X�e�[�g�����g�ɂȂ�܂��B
	 *
	 *	@see CTinySqlAccessor
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	�V�K�쐬
	 *	@date 12/06/05	�R�s�[�R���X�g���N�^�A�R�s�[�I�y���[�^�A�d�l�ύX�B
	 */
	class CStatement
	{
	public:

		/**
		 * �R���X�g���N�^.
		 *	@param hDbc �R�l�N�V�����n���h��
		 *	@param tm �^�C���A�E�g(s)
		 */
		CStatement(SQLHDBC hDbc, DWORD tm = 0) : m_hDbc(hDbc), m_timeout(tm)
		{
		}

		/**
		 * �R�s�[�R���X�g���N�^.
		 *	@param other �R�s�[��
		 */
		CStatement(const CStatement& other) 
			: m_hDbc(other.m_hDbc), m_timeout(other.m_timeout), m_hStmt(other.m_hStmt), m_lastReturnCode(other.m_lastReturnCode)
		{
		}

		/**
		 * �R�s�[�I�y���[�^.
		 *	@param other �R�s�[��
		 *	@return �����̎Q��
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
		 * [�m�F] �L���m�F.
		 *	@retval true �L���ȃC���X�^���X�B
		 *	@retval false �ہB
		 */
		bool IsValid(void)
		{
			m_Init();
			return ! m_hStmt.IsNull();
		}

		/**
		 * [�ݒ�] �g�p�s��.
		 *		�ێ����Ă���n���h����j�����܂��B
		 */
		void Destroy(void)
		{
			m_hStmt = NULL;
		}

		/**
		 * [�ݒ�] ���s.
		 *		�w��� SQL �����s���܂��B���ʃZ�b�g�� GetResultSet() �Ŏ擾�ł��܂��B
		 *	@param lpszSql SQL ��
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 */
		bool Execute(LPCTSTR lpszSql)
		{
			m_Init();
			//== SQL�����s
			CStr sql = lpszSql;
			SQLRETURN r = ::SQLExecDirect(m_hStmt, GetSqlChar(sql), SQL_NTS);
			return m_Rst(r);
		}

		/**
		 * [�擾] ���ʃZ�b�g�擾.
		 *		Execute() �̌��ʂ��Ǘ����Ă��� CResultSet ��Ԃ��܂��B
		 *	@return ���ʃZ�b�g�B CResultSet::IsValid() �Ő��������������Ă��邩�A�m�F���Ă��������B
		 */
		CResultSet GetResultSet(void) const
		{
			return CResultSet(m_hStmt);
		}

		/**
		 * [�擾] �߂�l�ڍ׎擾.
		 *	@return �߂�l.
		 */
		CReturnCode GetLastReturnCode(void) const
		{
			return m_lastReturnCode;
		}

		/**
		 * [�擾] �^�C���A�E�g�擾
		 *	@retval DWORD_MAX �擾�G���[
		 *	@retval ��L�ȊO �擾�����B�l��QUERY�^�C���A�E�g�l(s)�B
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
		 * [�ݒ�] �N���[�Y�J�[�\��.
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 */
		virtual bool CloseCursor(void) 
		{
			return m_Rst(::SQLCloseCursor(m_hStmt));
		}

	protected:

		/// [�m�F] ��������
		///	@retval true ����
		/// @retval false ���s
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
		/// [�m�F] �߂�l����
		///	@param rc SQL API�̖߂�l
		///	@retval true ����
		/// @retval false ���s
		bool m_Rst(SQLRETURN rc = 0)
		{
			m_lastReturnCode = CReturnCode(SQL_HANDLE_STMT, m_hStmt, rc);
			return CReturnCode::IsReturnCodeSuccess(rc);
		}
		SQLHDBC			m_hDbc;				///< DBC �n���h��
		CStmtHandle		m_hStmt;			///< �X�e�[�g�����g�n���h��
		DWORD			m_timeout;			///< �^�C���A�E�g(s)
		CReturnCode		m_lastReturnCode;	///< �Ō�̖߂�l
	};


	//---------------------------


	/**@ingroup DBASE
	 * �Ȉ� SQL �A�N�Z�T�p�v���y�A�h�X�e�[�g�����g.
	 *
	 *		CTinySqlAccessor::PreparedStatement() �̖߂�l�ō쐬�ł��܂��i���ڃC���X�^���X���쐬���Ă��Ӗ�������܂���j�B
	 *		�g�p�O�ɁA IsValid() �ŗL���ȃC���X�^���X�ɂȂ��Ă��邩�A�m�F���Ă��������B
	 *
	 *	@note �R�s�[����ƁA�����f�[�^�x�[�X�ւ̐ڑ������V�����X�e�[�g�����g�ɂȂ�܂��B
	 *		CTinySqlAccessor::PreparedStatement() �œn���� SQL �͈����p����܂��B
	 *
	 *	@see CTinySqlAccessor
	 *
	 *	@todo DEFAULT �̃p�����[�^�w�肷�邽�߂́A SetDefault() ���������ł��B
	 *			SQL �� 'DEFAULT' �\�L����K�v������܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTinySqlAccessor.h
	 * 
	 *	@date 11/05/16	�V�K�쐬
	 *	@date 12/01/26	MSSQL 2008 �� target �� SetNull() �A�����B
	 *	@date 12/10/29	SetInt() �� short �Ɋۂ߂���o�O���C���B
	 */
	class CPreparedStatement : public CStatement
	{
		DEFSUPER(CStatement);
	public:

		/**
		 * �R���X�g���N�^.
		 *	@param hDbc �R�l�N�V�����n���h��
		 *	@param lpszSql SQL ��
		 *	@param tm �^�C���A�E�g(s)
		 */
		CPreparedStatement(SQLHDBC hDbc, LPCTSTR lpszSql, DWORD tm = 0) : _super(hDbc, tm), m_sql(lpszSql), m_isPrepared(false)
		{
		}

		/**
		 * �R�s�[�R���X�g���N�^.
		 *	@param other �R�s�[��
		 */
		CPreparedStatement(const CPreparedStatement& other) : _super(other), m_sql(other.m_sql), m_isPrepared(false)
		{
		}

		/**
		 * �R�s�[�I�y���[�^.
		 *	@param other �R�s�[��
		 *	@return �����̎Q��
		 */
		CPreparedStatement& operator=(const CPreparedStatement& other)
		{
			_super::operator=(other);
			m_sql = other.m_sql;			// SQL ��
			m_isPrepared = false;
			m_bindParams.RemoveAll();	// �o�C���h���[�N
			return *this;
		}

		/**
		 * [�ݒ�] ���s.
		 *		�w��� SQL �����s���܂��B���ʃZ�b�g�� GetResultSet() �Ŏ擾�ł��܂��B
		 *	@note �ʏ�A�g���܂���B
		 *	@param lpszSql SQL ��
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 */
		bool ExecuteDirect(LPCTSTR lpszSql)
		{
			return _super::Execute(lpszSql);
		}

		/**
		 * [�ݒ�] ���s.
		 *		SQL �����s���܂��B���ʃZ�b�g�� GetResultSet() �Ŏ擾�ł��܂��B
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
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
		 * [�ݒ�] NULL �l�ݒ�.
		 *	@param parameterIndex �p�����^�C���f�b�N�X�B�ŏ��̃p�����^�͂P�A��Ԗڂ��Q�ɂȂ�܂��B
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 */
		bool SetNull(INDEX parameterIndex)
		{
			return m_BindParam(parameterIndex, SQL_C_NUMERIC, SQL_NUMERIC, SQL_NULL_DATA, NULL);
		}

		/**
		 * [�ݒ�] int �l�ݒ�.
		 *	@param parameterIndex �p�����^�C���f�b�N�X�B�ŏ��̃p�����^�͂P�A��Ԗڂ��Q�ɂȂ�܂��B
		 *	@param value �l
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 */
		bool SetInt(INDEX parameterIndex, int value)
		{
			return m_BindParam(parameterIndex, SQL_C_SLONG, SQL_INTEGER, sizeof(value), &value);
		}

		/**
		 * [�ݒ�] double �l�ݒ�.
		 *	@param parameterIndex �p�����^�C���f�b�N�X�B�ŏ��̃p�����^�͂P�A��Ԗڂ��Q�ɂȂ�܂��B
		 *	@param value �l
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 */
		bool SetDouble(INDEX parameterIndex, double value)
		{
			return m_BindParam(parameterIndex, SQL_C_DOUBLE, SQL_DOUBLE, sizeof(value), &value);
		}

		/**
		 * [�ݒ�] �o�C�i���ݒ�.
		 *	@param parameterIndex �p�����^�C���f�b�N�X�B�ŏ��̃p�����^�͂P�A��Ԗڂ��Q�ɂȂ�܂��B
		 *	@param size �o�C�i���f�[�^����.(long �ŕ\���ł��钷���܂�)
		 *	@param P �o�C�i���f�[�^
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
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
		 * [�ݒ�] ������ݒ�.
		 *	@param parameterIndex �p�����^�C���f�b�N�X�B�ŏ��̃p�����^�͂P�A��Ԗڂ��Q�ɂȂ�܂��B
		 *	@param lpsz ������B(long �ŕ\���ł��钷���܂�)
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
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
		 * [�ݒ�] ������ݒ�.
		 *	@note �����񒷂��O�̏ꍇ�A NULL ��ݒ肵�܂��B
		 *	@param parameterIndex �p�����^�C���f�b�N�X�B�ŏ��̃p�����^�͂P�A��Ԗڂ��Q�ɂȂ�܂��B
		 *	@param lpsz ������B(long �ŕ\���ł��钷���܂�)
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
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
		 * [�ݒ�] �^�C���X�^���v�ݒ�.
		 *	@param parameterIndex �p�����^�C���f�b�N�X�B�ŏ��̃p�����^�͂P�A��Ԗڂ��Q�ɂȂ�܂��B
		 *	@param time ����
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
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
		 * [�ݒ�] �N���[�Y�J�[�\��.
		 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
		 */
		virtual bool CloseCursor(void) 
		{
			m_isPrepared = false;
			return _super::CloseCursor();
		}
	private:
		// �g�p�֎~�錾
		bool Execute(LPCTSTR lpszSql);
		// �o�C���h���[�N�^
		struct TBindWork
		{
			CWorkMem			work;		///< Bind �̈�
			SQLLEN				length;		///< Fetch ���ʒl
		};
		// �v���y�A����
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
		// �o�C���h����
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
		CStr						m_sql;			///< SQL ��
		bool						m_isPrepared;	///< �v���y�A�ς݁H
		CPointerVectorT<TBindWork>	m_bindParams;	///< �o�C���h���[�N
	};


	//-------------------------------


	/**
	 * �f�X�g���N�^.
	 *	@note �f�X�g���N�^��A�{�N���X���琶�������N���X�̃C���X�^���X�������������Ȃ��Ȃ�܂��B
	 */
	~CTinySqlAccessor(void)
	{
		Stop();
	}

	/**
	 * �R���X�g���N�^.
	 */
	CTinySqlAccessor(void) : m_hEnv(NULL), m_hDbc(NULL), m_isConnecting(false), m_timeout(0)
	{
	}

	/**
	 * [�m�F] �J�n���Ă���H.
	 *		Start() �� Connect() �����s����ƁASQL�A�N�Z�X�������J�n����܂��B
	 *	@retval true �J�n���Ă���
	 *	@retval false ���Ă��Ȃ�
	 */
	bool IsStarted(void) const
	{
		return m_hEnv != NULL;
	}

	/**
	 * [�ݒ�] ��~.
	 *		Start() �� Connect() �����s����ƁASQL�A�N�Z�X�������J�n����܂��B
	 *		���̃��\�b�h���R�[������ƁA��~���܂��B
	 *		�܂��A�ڑ�����Ă����ꍇ�A�ؒf���s���܂��B
	 *	@note ��~����ƁA�{�N���X���琶�������N���X�̃C���X�^���X�������������Ȃ��Ȃ�܂��B
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Stop(void)
	{
		if ( IsStarted() )
		{
			Disconnect();
			// DB�ڑ��p�n���h���J��
			SQLRETURN r = ::SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
			if ( ! m_Rst(SQL_HANDLE_DBC, m_hDbc, r) )
			{
				return false;
			}
			m_hDbc = NULL;
			// ���p�̃n���h���J��
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
	 * [�ݒ�] �J�n.
	 *	@note �{���\�b�h���g��Ȃ��Ă��A Connect() ���g�����ƂŁA�����I��SQL�A�N�Z�X�������J�n����܂��B
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Start(void)
	{
		Stop();
		//== ODBC���n���h���擾
		SQLRETURN r = ::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
		if ( ! m_CheckReturnCode(r) )
		{
			m_hEnv = NULL;
		}
		else
		{
			//== ODBC�o�[�W�����ݒ�
			r = ::SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
			if ( m_CheckReturnCode(r) )
			{
				//== ODBC�f�[�^�x�[�X�n���h���擾
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
	 * [�m�F] �ڑ������H
	 *	@retval true �ڑ����B
	 *	@retval false �ہB
	 */
	bool IsConnecting(void) const
	{
		return m_isConnecting;
	}

	/**
	 * [�ݒ�] �ؒf.
	 *		Connect() �ōs�����ڑ���ؒf���܂��B
	 *	@note �ؒf�O�ɃR�~�b�g���܂��̂ŁA�ύX�𖳌��ɂ������ꍇ�A��� Rollback() ���R�[������K�v������܂��B
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Disconnect(void)
	{
		if ( ! IsConnecting() )
		{
			return m_Rst();
		}
		// �R�~�b�g
		SQLRETURN r = ::SQLEndTran(SQL_HANDLE_DBC, m_hDbc, SQL_COMMIT);
		// DB �f�B�X�R�l�N�g
		r = ::SQLDisconnect(m_hDbc);
		if ( m_Rst(SQL_HANDLE_DBC, m_hDbc, r) )
		{
			m_isConnecting = false;
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �ڑ�.
	 *		�w��̐ݒ�ŁA�f�[�^�x�[�X�Ɛڑ����܂��B
	 *	@note Start() ���Ă��Ȃ��ꍇ�A�X�^�[�g���܂��B
	 *	@param lpszDataBaseName �f�[�^�x�[�X��.
	 *	@param lpszUserName ���[�U�[��
	 *	@param lpszPassword �p�X���[�h
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Connect(LPCTSTR lpszDataBaseName, LPCTSTR lpszUserName, LPCTSTR lpszPassword)
	{
		if ( m_PreConnect() )
		{
			//== �f�[�^�x�[�X�ڑ�
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
	 * [�ݒ�] �ڑ�.
	 *		�w��̐ݒ�ŁA�f�[�^�x�[�X�Ɛڑ����܂��B
	 *	@note Start() ���Ă��Ȃ��ꍇ�A�X�^�[�g���܂��B
	 *	@param lpszConnect �ڑ���.
	 *	@param options �I�v�V����.
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Connect(LPCTSTR lpszConnect, SQLUSMALLINT options = SQL_DRIVER_NOPROMPT)
	{
		if ( m_PreConnect() )
		{
			CStr cs = lpszConnect;
			//== �f�[�^�x�[�X�ڑ�
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
	 * [�ݒ�] �ڑ�.
	 *		�w��̐ݒ�ŁA�f�[�^�x�[�X�Ɛڑ����܂��B
	 *	@note Start() ���Ă��Ȃ��ꍇ�A�X�^�[�g���܂��B
	 *	@param lpszDriverName �h���C�o�[��.
	 *	@param lpszDataBaseName �f�[�^�x�[�X��.
	 *	@param lpszUserName ���[�U�[��
	 *	@param lpszPassword �p�X���[�h
	 *	@param options �I�v�V����.
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Connect(LPCTSTR lpszDriverName, LPCTSTR lpszDataBaseName, LPCTSTR lpszUserName, LPCTSTR lpszPassword, SQLUSMALLINT options = SQL_DRIVER_NOPROMPT)
	{
		//== ���O�쐬
		CStr cs;
		cs.Format(_T("DRIVER={%s};Database=%s;UID=%s;PWD=%s;"), lpszDriverName, lpszDataBaseName, lpszUserName, lpszPassword);
		return Connect(cs, options);
	}

	/**
	 * [�ݒ�] �^�C���A�E�g�ݒ�.
	 *	@param tm QUERY�^�C���A�E�g�l(s)
	 */
	void SetQueryTimeout(DWORD tm)
	{
		m_timeout = tm;
	}

	/**
	 * [�쐬] �X�e�[�g�����g�쐬.
	 *	@return �X�e�[�g�����g�B CStatement::IsValid() �Ő����̗L�����킩��܂��B
	 */
	CStatement CreateStatement(void)
	{
		return CStatement(m_hDbc, m_timeout);
	}

	/**
	 * [�쐬] �v���y�A�h�X�e�[�g�����g�쐬.
	 *	@param lpszSql SQL ��
	 *	@return �v���y�A�h �X�e�[�g�����g�B CStatement::IsValid() �Ő����̗L�����킩��܂��B
	 */
	CPreparedStatement PreparedStatement(LPCTSTR lpszSql)
	{
		return CPreparedStatement(m_hDbc, lpszSql, m_timeout);
	}

	/**
	 * [�擾] �����R�~�b�g��Ԋm�F
	 *	@note Start() �� Connect() ���Ă���K�v������܂��B
	 *	@retval true �����R�~�b�g��Ԃ��L���B
	 *	@retval false �����R�~�b�g��Ԃ������A���A�G���[�ł��B�G���[���ۂ��� GetLastReturnCode() �œ����܂��B
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
	 * [�ݒ�] �����R�~�b�g�ݒ�.
	 *	@note Start() �� Connect() ���Ă���K�v������܂��B
	 *	@param isEnable true �Ȃ玩���R�~�b�g��L�����܂��B false �Ȃ疳���ɂ��܂��B
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool SetAutoCommit(bool isEnable)
	{
		DWORD l = isEnable ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF;
		SQLRETURN r = ::SQLSetConnectAttr(m_hDbc, SQL_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(l)), 0);
		return m_Rst(SQL_HANDLE_DBC, m_hDbc, r);
	}
	
	/**
	 * [�ݒ�] �R�~�b�g.
	 *	@note Start() �� Connect() ���Ă���K�v������܂��B
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Commit(void)
	{
		SQLRETURN r = ::SQLEndTran(SQL_HANDLE_DBC, m_hDbc, SQL_COMMIT);
		return m_Rst(SQL_HANDLE_DBC, m_hDbc, r);
	}

	/**
	 * [�ݒ�] ���[���o�b�N.
	 *	@note Start() �� Connect() ���Ă���K�v������܂��B
	 *	@retval true �����B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 *	@retval false ���s�B�ڍׂ� GetLastReturnCode() �œ����܂��B
	 */
	bool Rollback(void)
	{
		SQLRETURN r = ::SQLEndTran(SQL_HANDLE_DBC, m_hDbc, SQL_ROLLBACK);
		return m_Rst(SQL_HANDLE_DBC, m_hDbc, r);
	}

	/**
	 * [�擾] �߂�l�ڍ׎擾.
	 *	@return �߂�l.
	 */
	CReturnCode GetLastReturnCode(void) const
	{
		return m_lastReturnCode;
	}

	/**
	 * [�ϊ�] ���ԕ�����.
	 *		SYSTEMTIME �� Timestamp �Ŏg�p���镶����(YYYY-MM-DD HH:MM:SS)�ɕϊ����܂��B
	 *	@param time ����.
	 *	@return ������B
 	 */
	static CStr SystemTimeToString(const SYSTEMTIME& time)
	{
		return CStr::Fmt(_T("%4d-%02d-%02d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	}


	//----------------


	/**
	 * [�ϊ�] ������|�C���^�擾.
	 *		CStr ���� SQLTCHAR �̃|�C���^���擾���܂�( CStr::GetBuffer() ���g�p )�B
	 *	@note �ʏ�g���K�v����܂���B
	 *	@param[in,out] _s �ΏہB
	 *	@param[in] len �o�b�t�@�T�C�Y�B
	 *	@return ������|�C���^�B �g�p��A _s �ɑ΂� CStr::ReleaseBuffer() ���g�p���Ă��������B
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
	/// �ڑ�
	bool m_PreConnect(void)
	{
		if ( ! IsStarted() && ! Start() )
		{
			return false;
		}
		return Disconnect();
	}
	// �߂�l�`�F�b�N
	bool m_CheckReturnCode(SQLRETURN r) const
	{
		return CReturnCode::IsReturnCodeSuccess(r);
	}
	// �߂�l����
	bool m_Rst(SQLSMALLINT type = 0, SQLHANDLE handle = NULL, SQLRETURN rc = 0)
	{
		m_lastReturnCode = CReturnCode(type, handle, rc);
		return m_CheckReturnCode(rc);
	}

	SQLHENV		m_hEnv;				///< ���n���h��
	SQLHDBC		m_hDbc;				///< DBC �n���h��
	DWORD		m_timeout;			///< �^�C���A�E�g(s)
	bool		m_isConnecting;		///< �ڑ���
	CReturnCode	m_lastReturnCode;	///< �Ō�̖߂�l
};



}; // TNB


