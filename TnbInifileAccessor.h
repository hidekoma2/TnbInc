#pragma once
/**
 *@file
 * iniファイルアクセス関係のヘッダ
 *		
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE

#include "TnbAccessor.h"
#include "TnbDntStr.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * iniファイルアクセスクラス
 *
 *		iniファイルにアクセスする関数群です。
 *		APIの GetPrivateProfile～ 関数 SetPrivateProfile～ とファイル互換が有ります。
 *		
 *		ただし、すべてにおいて、文字列として処理しています。
 *
 *		コンストラクタや SetBase() で情報のベースを指定し、
 *		operator[]() で セクション情報アクセスクラス({@link IAccessor::CSection CSection})を取得し、
 *		Query～() 、 Write～() などでキー情報にアクセスします。
 *
 *	@note	本インターフェースにも QueryValue() , WriteValue() など、
 *			セクションを指定しキー情報を操作するメソッドもありますが、
 *			基本的に {@link IAccessor::CSection CSection} を使用してください。
 *
 *	@see	IAccessor を実装しています。
 *
 *	@par必要ファイル
 *			TnbInifileAccessor.h
 *
 *	@date 06/01/01 新規作成
 *	@date 06/04/20 CString未使用に
 *	@date 06/05/08 CDoubleNullTerminateStrを使用するようにした。
 *	@date 06/05/16 IAccessor を実装するようにした。
 *	@date 06/06/08 メンバ名変更。
 *	@date 06/10/31 IInformation の改修に対応。
 *	@date 07/05/10 クラス名変更。
 *	@date 08/04/03 デフォルト値の扱いを修正。
 *	@date 09/07/28 NewBaseFile() 追加。
 *	@date 11/09/15 書き込む際、先に読み出し、同じなら何もしないようにした。
 *	@date 11/11/07 上記対応にバグあり。一文字の時に、正しく書き込めない可能性があった。
 */
class CInifileAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
public:

	/**
	 * コンストラクタ
	 *	@param lpszFile ファイル名 。省略すると、 MakeInifilePath() の結果が使われます。
	 */
	explicit CInifileAccessor(LPCTSTR lpszFile = NULL) : _super()
	{
		CInifileAccessor::SetBase(lpszFile); 
	}

	/**
	 * [設定] 対象ファイル指定
	 *	@param lpszFile ファイル名
	 */
	void SetBase(LPCTSTR lpszFile = NULL)
	{
		m_strFile = (lpszFile == NULL) ? MakeInifilePath() : lpszFile;
	}

	/**
	 * [作成] iniファイルパス作成.
	 *		実行ファイル名の .exeを .iniに変えたものを作成します。
	 *	@return iniファイルのFullPath
	 */
	static CStr MakeInifilePath(void)
	{
		CStr str = _super::MakeDefineFilePath();
		str += _T(".ini");
		return str;
	}

	/**
	 * [取得] 対象ファイル取得
	 *	@return ファイル名
	 */
	CStr GetBaseFileName(void) const
	{
		return m_strFile;
	}

	/**
	 * [確認] 対象ファイル存在確認.
	 *		対象の ini ファイルが、現在存在しているか、確認できます。
	 *	@note ファイルの有無のみ、チェックしています。
	 *	@retval true あり。
	 *	@retval false 無し。
	 */
	bool ExistBaseFile(void) const
	{
		DWORD r = ::GetFileAttributes(m_strFile);
		return ToInt(r) >= 0;
	}

	/**
	 * [作成] 対象ファイル作成.
	 *		対象ファイルを新規します。今までの情報はすべてなくなります。
	 *	@note 通常、使う必要はありませんが、全部消したい場合、 UNICODE 形式にしたい場合に使います。
	 *	\code
	 *		CInifileAccessor ini;
	 *		if ( ! ini.ExistBaseFile() )
	 *		{
	 *			ini.NewBaseFile(true); //unicode のiniファイルにする
	 *		}
	 *	\endcode
	 *	@param isUnicode false なら ASCII(S-JIS)、 true なら UNICODE で対象ファイルを新規作成します。
	 *	@retval true 成功.
	 *	@retval false 失敗
	 */
	bool NewBaseFile(bool isUnicode = false)
	{
		bool r = false;
		HANDLE h = ::CreateFile(m_strFile, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, 0, NULL);
		// 書込み
		if ( h != INVALID_HANDLE_VALUE )
		{
			if ( isUnicode )
			{
				BYTE buf[2] = { 0xFF, 0xFE };
				DWORD writed;
				if ( ::WriteFile(h, buf, 2, &writed, NULL) && 2 == writed )
				{
					r = true;
				}
			}
			else
			{
				r = true;
			}
			::CloseHandle(h);
		}
		return r;
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Inifile");
	}

	/**
	 * [操作] フラッシュ.
	 *	@note メモリ上にキャッシュがあれば書き出します。
	 *	@return true （成功）
	 */
	virtual bool Flush(void) 
	{
		::WritePrivateProfileString(NULL, NULL, NULL, m_strFile);	//戻り値は必ずfalse(仕様)なので見ない
		return true;
	}

	/**
	 * [取得] セクション名一覧取得
	 *	@param lpszSectionName セクション名。 NULLを指定すると、ROOTのセクション一覧が得られます。
	 *	@return セクション名一覧
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const
	{
		CStrVector vs;
		CDoubleNullTerminateStr dntstr;
		DWORD dwRc = ::GetPrivateProfileSectionNames(
							dntstr.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		dntstr.ReleaseBuffer();
		if ( dwRc <= 0 )
		{
			return vs;
		}
		vs = dntstr.ToStrVector();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		if ( ! ss.IsEmpty() )
		{
			loop ( i, vs.GetSize() )
			{
				CStr s;
				if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
				{
					s = vs[i].Mid(len);
					if ( ! s.IsEmpty() && s[0] == '\\' )
					{
						s = s.Mid(1);
					}
					else
					{
						s.Empty();
					}
				}
				vs[i] = s;
			}
		}
		CStrVector vstrSecNames;
		loop ( i, vs.GetSize() )
		{
			if ( ! vs[i].IsEmpty() )
			{
				CStr s = vs[i];
				INDEX d = s.Find(_T('\\'));
				if ( d != INVALID_INDEX )
				{
					s = s.Left(d);
				}
				if ( vstrSecNames.Find(s) == INVALID_INDEX )
				{
					vstrSecNames.Add(s);
				}
			}
		}
		return vstrSecNames;
	}

	/**
	 * [削除] 指定セクション削除
	 *	@param lpszSectionName セクション名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		CStrVector vs;
		CDoubleNullTerminateStr dntstr;
		DWORD dwRc = ::GetPrivateProfileSectionNames(
							dntstr.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		dntstr.ReleaseBuffer();
		if ( dwRc <= 0 )
		{
			return false;
		}
		vs = dntstr.ToStrVector();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		loop ( i, vs.GetSize() )
		{
			if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
			{
				if ( ! ::WritePrivateProfileString(vs[i], NULL, NULL, m_strFile) )
				{
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * [取得] 名前一覧取得
	 *	@param lpszSectionName セクション名
	 *	@return 名前一覧。無効状態なら未対応。
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		CStrVector vstrKeyNames;
		CDoubleNullTerminateStr dntstr;
		DWORD dwRc = ::GetPrivateProfileSection(
							lpszSectionName,
							dntstr.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		dntstr.ReleaseBuffer();
		if ( dwRc > 0 )
		{
			CStrVector vstr = dntstr.ToStrVector();
			loop ( i, vstr.GetSize() )
			{
				vstrKeyNames.Add(vstr[i].FindCut('='));
			}
		}
		return vstrKeyNames;
	}

	/**
	 * [取得] 情報取種取得
	 *	@note 本クラスでは EK_String か EK_Nothing になります。
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return 情報種。ない場合、EK_Nothing になります。
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CStr dmy;
		if ( ! m_QueryString(dmy, lpszSectionName, lpszKey) )
		{
			return EK_Nothing;
		}
		return EK_String;
	}

	/**
	 * [取得] 情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CStr s;
		if ( m_QueryString(s, lpszSectionName, lpszKey) )
		{
			return CValue(s);
		}
		return CValue();
	}

	/**
	 * [設定] 情報設定
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param value 設定する値。EK_Nothing の場合、キーを削除します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 *	@throw CTnbException 設定失敗時、スローされる可能性があります。
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		if ( value.GetKind() == EK_Nothing )
		{
			return m_Delete(lpszSectionName, lpszKey);
		}
		return m_WriteString(lpszSectionName, lpszKey, value.QueryString());
	}

private:
	enum { E_BufferSize = 65536 };		///< 取得最大サイズ

	/**
	 * [取得] 文字列情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param lpszDefault キーがない時この値を返します。 NULLは指定できません。
	 *	@return キーの値
	 */
	CStr m_GetString(LPCTSTR lpszSectionName, LPCTSTR lpszKey, LPCTSTR lpszDefault) const
	{
		ASSERT( lpszDefault != NULL );
		CStr strValue;
		::GetPrivateProfileString(
							lpszSectionName,
							lpszKey,
							lpszDefault,
							strValue.GetBuffer(E_BufferSize),
							E_BufferSize,
							m_strFile);
		strValue.ReleaseBuffer();
		return strValue;
	}

	/**
	 * [取得] 文字列情報取得
	 *	@param[out] _r 取得文字列
	 *	@param[in] lpszSectionName セクション名
	 *	@param[in] lpszKey キー名
	 *	@retval true 取得成功
	 *	@retval false 指定のセクション、キーがなかった。
	 */
	bool m_QueryString(CStr& _r, LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		_r = m_GetString(lpszSectionName, lpszKey, _T("*1"));
		if ( _r.Compare(_T("*1")) == 0 )
		{
			_r = m_GetString(lpszSectionName, lpszKey, _T("*2"));
			if ( _r.Compare(_T("*2")) == 0 )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [追加] 文字列情報記録
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param lpszData キーの値
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_WriteString(LPCTSTR lpszSectionName, LPCTSTR lpszKey, LPCTSTR lpszData)
	{
		DWORD len = down_cast<DWORD>(STRLIB::GetLen(lpszData));
		CWorkMemT<TCHAR> work(len + 2);
		DWORD r = ::GetPrivateProfileString(lpszSectionName, lpszKey, _T(""), work, len + 2, m_strFile);
		if ( r == len && STRLIB::Compare(lpszData, work) == 0 )
		{
			return true;
		}
		return !! ::WritePrivateProfileString(lpszSectionName, lpszKey, lpszData, m_strFile);
	}

	/**
	 * [削除] 指定キー削除
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_Delete(LPCTSTR lpszSectionName, LPCTSTR lpszKey)
	{
		if ( GetKeyKind(lpszSectionName, lpszKey) != EK_Nothing )
		{
			return !! ::WritePrivateProfileString(lpszSectionName, lpszKey, NULL, m_strFile);
		}
		return false;
	}

	CStr		m_strFile;				///< 扱うファイル名
	CStr		m_strSection;			///< 扱うセクション名
};



}; // TNB

#else // _WIN32_WCE

#include "TnbCeInifileAccessor.h"

#endif // _WIN32_WCE

