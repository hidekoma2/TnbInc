#pragma once
/**
 *@file
 * レジストリアクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAccessor.h"
#include "TnbDntStr.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル
 #ifndef REG_QWORD
  #define REG_QWORD (11) // 64-bit number
 #endif
#endif



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
/// PointerHandleBase用Destoryクラス
struct TPhRegCloseKey
{
	void operator()(HKEY P) { ::RegCloseKey(P); }
};
#endif



/**@ingroup ACCESSOR
 * HKEY型ハンドルテンプレート
 *
 *		この型に RegOpenKeyEx等、RegCloseKey() で破棄するハンドルを渡しておくと、
 *		「領域」を参照する本クラスががなくなり次第、
 *		自動的に RegCloseKey します。
 *
 *	@see http://www.microsoft.com/japan/developer/library/jpwinpf/_win32_RegCloseKey.htm
 *
 *	@par必要ファイル
 *			TnbRegistryAccessor.h
 */
typedef TNB::CPointerHandleBaseT<HKEY, TPhRegCloseKey> CRegKeyHandle;



/**@ingroup ACCESSOR
 * レジストリアクセスクラス
 *	
 *		レジストリを読み書きするためのクラスです。
 *
 *		本クラスでは、サブキーを「セクション」と呼んでいます。
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
 *			TnbRegistryAccessor.h
 * 
 *	@date 06/05/20	新規作成
 *	@date 06/06/08 メンバ名変更。
 *	@date 06/10/31 IInformation の改修に対応。
 *	@date 07/05/10 クラス名変更。
 *	@date 08/04/15 書き込み系修正。
 *	@date 08/10/15 SHLWAPI.dll(SHDeleteKey()) を使わないように変更。
 *	@date 09/12/01 IsValid() 新規。構成整理。
 */
class CRegistryAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
public:

	/**
	 * コンストラクタ 
	 *	@param hhParentKey 対象の親ハンドル。通常、HKEY_LOCAL_MACHINEなどのシンボルを指定します。
	 *	@param dwRegSam アクセス権。省略すると KEY_ALL_ACCESS になります。 他、 KEY_READ などがあります。
	 */
	explicit CRegistryAccessor(const CRegKeyHandle& hhParentKey, REGSAM dwRegSam = KEY_ALL_ACCESS) 
		: m_hhParentKey(hhParentKey), m_dwRegSam(dwRegSam)
	{
	}

	/**
	 * コンストラクタ 
	 *	@param hhParentKey 対象の親ハンドル。通常、HKEY_LOCAL_MACHINEなどのシンボルを指定します。
	 *	@param lpszSectionName セクション名。
	 *	@param dwRegSam アクセス権。省略すると KEY_ALL_ACCESS になります。 他、 KEY_READ などがあります。
	 */
	CRegistryAccessor(const CRegKeyHandle& hhParentKey, LPCTSTR lpszSectionName, REGSAM dwRegSam = KEY_ALL_ACCESS) 
	{
		CRegistryAccessor rg(hhParentKey, dwRegSam);
		m_hhParentKey = rg.GetHandle(lpszSectionName);
		m_dwRegSam = dwRegSam;
	}

	/**
	 * デストラクタ
	 */
	virtual ~CRegistryAccessor(void) 
	{
	}

	/**
	 * [設定] ベース指定
	 *	@par対象
	 *			全体
	 *	@param hhParentKey 対象の親ハンドル。通常、HKEY_LOCAL_MACHINEなどのシンボルを指定します。
	 */
	void SetBase(const CRegKeyHandle& hhParentKey)
	{
		m_hhParentKey = hhParentKey;
		m_strSection.Empty();
		m_hhKey.Null();
	}

	/**
	 * [取得] 親ハンドル取得
	 *	@return 対象ハンドル
	 */
	const CRegKeyHandle& GetBase(void) const
	{
		return m_hhParentKey;
	}

	/**
	 * [取得] 有効確認.
	 *	@retval true 有効。
	 *	@retval false 無効。
	 */
	bool IsValid(void) const
	{
		HKEY h = m_hhParentKey;
		return h != NULL;
	}

	/**
	 * [取得] 対象ハンドル取得
	 *	@param lpszSectionName セクション名
	 *	@return 対象ハンドル
	 */
	const CRegKeyHandle& GetHandle(LPCTSTR lpszSectionName) const
	{
		m_SetSectionName(lpszSectionName);
		m_Create();
		return m_hhKey;
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Registry");
	}

	/**
	 * [操作] フラッシュ.
	 *	@note メモリ上にキャッシュがあれば書き出します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool Flush(void)
	{
		if ( m_hhKey.IsNull() )
		{
			return false;
		}
		return ::RegFlushKey(m_hhKey) == ERROR_SUCCESS; 
	}

	/**
	 * [取得] セクション名一覧取得
	 *	@param lpszSectionName セクション名。 NULLを指定すると、ROOTのセクション一覧が得られます。
	 *	@return セクション名一覧
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const
	{
		if ( lpszSectionName == NULL || *lpszSectionName == 0 )
		{
			return m_EnumSections(m_hhParentKey);
		}
		m_SetSectionName(lpszSectionName);
		if ( m_Open() != 0 )
		{
			return CStrVector(); 
		}
		return m_EnumSections(m_hhKey);
	}

	/**
	 * [削除] 指定セクション削除
	 *	@param lpszSectionName セクション名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		m_Close();
		DWORD r = ms_RegDeleteKeyNT(m_hhParentKey, lpszSectionName);
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [取得] 名前一覧取得
	 *	@note 本クラスでは pure関数です。
	 *	@param lpszSectionName セクション名
	 *	@return 名前一覧。無効状態なら未対応。
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		m_SetSectionName(lpszSectionName);
		CStrVector vstrRc;
		if ( m_Open() != 0 )
		{
			return vstrRc; 
		}
		LONG lRc;
		DWORD dwIndex = 0;
		CStr strName;
		DWORD dwNameLength;
		while ( true )
		{
			dwNameLength = E_BufferSize;
			lRc = ::RegEnumValue(
						m_hhKey,			// 問い合わせ対象のキーのハンドル
						dwIndex,			// 取得するべきレジストリエントリのインデックス番号
						strName.GetBuffer(dwNameLength),
											// レジストリエントリ名が格納されるバッファ
						&dwNameLength,		// レジストリエントリ名バッファのサイズ
						0,					// 予約済み
						NULL,				// レジストリエントリのデータのタイプ
						NULL,				// レジストリエントリのデータが格納されるバッファ
						NULL				// データバッファのサイズ
			);
			strName.ReleaseBuffer();
			if ( lRc == ERROR_NO_MORE_ITEMS )
			{
				break;
			}	
			if ( lRc != ERROR_SUCCESS )
			{
				vstrRc.Invalid();
				return vstrRc;
			}
			vstrRc.Add(strName);
			dwIndex++;
		}
		return vstrRc;
	}

	/**
	 * [取得] 情報取種取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return 情報種。ない場合、EK_Nothing になります。
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		m_SetSectionName(lpszSectionName);
		if ( m_Open() != 0 )
		{
			return EK_Nothing; 
		}
		DWORD dwType;
		DWORD dwLength;
		LONG lRc = ::RegQueryValueEx(
						m_hhKey,			// キーのハンドル
						lpszKey,			// レジストリエントリ名
						NULL,				// 予約済み
						&dwType,			// データ型が格納されるバッファ
						NULL,				// データが格納されるバッファ
						&dwLength			// データバッファのサイズ
		);
		EKind eKind = EK_Nothing;
		if ( lRc == ERROR_SUCCESS )
		{
			switch ( dwType )
			{
			case REG_QWORD:// 64 ビット値
				eKind = EK_Longlong;
				break;
			case REG_DWORD:// 32 ビット値
				eKind = EK_Dword;
				break;
			case REG_MULTI_SZ:// NULL で終わる複数の文字列からなる 1 つの配列
				eKind = EK_PluralStrings;
				break;
 			case REG_EXPAND_SZ:// 環境変数の展開前の表記（"%PATH%" など）を保持している、NULL で終わる文字列
				eKind = EK_String;
				break;
			case REG_SZ:// NULL で終わる文字列
				eKind = EK_String;
				break;
			case REG_BINARY:// Free form binary
				eKind = EK_Binary;
				break;
			}
		}
		return eKind;
	}

	/**
	 * [取得] 情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CValue v;
		DWORD dwGetType;
		CByteVector vb;
		switch ( GetKeyKind(lpszSectionName, lpszKey) )
		{
		case EK_Nothing://存在しない
			break;
		case EK_String://文字列
			vb = m_Query(lpszKey, REG_SZ, REG_EXPAND_SZ, &dwGetType);
			if ( vb.IsValid() )
			{
				CStr str = reinterpret_cast<LPCTSTR>(vb.ReferBuffer());
				#ifndef _WIN32_WCE
					if ( dwGetType == REG_EXPAND_SZ )
					{
						// 環境変数の展開前の表記（"%PATH%" など）を保持している、NULL で終わる文字列
						DWORD l = ::ExpandEnvironmentStrings(str, NULL, 0);
						if ( l > 0 )
						{
							CStr s;
							::ExpandEnvironmentStrings(str, s.GetBuffer(l + 1), l + 1);
							s.ReleaseBuffer();
							str = s;
						}
					}
				#endif
				v = CValue(str);
			}
			break;
		case EK_PluralStrings://複数の文字列群
			vb = m_Query(lpszKey, REG_MULTI_SZ, REG_MULTI_SZ);
			if ( vb.IsValid() )
			{
				CDoubleNullTerminateStr dntstr;
				dntstr.SetDntStr(reinterpret_cast<LPCTSTR>(vb.ReferBuffer()));
				v = CValue(dntstr.ToStrVector());
			}
			break;
		case EK_Binary://バイナリ
			vb = m_Query(lpszKey, REG_BINARY, REG_BINARY);
			if ( vb.IsValid() )
			{
				v = CValue(vb);
			}			
			break;
		case EK_Dword://32Bit Unsigned Integer
			vb = m_Query(lpszKey, REG_DWORD, REG_DWORD);
			if ( vb.IsValid() )
			{
				ASSERTLIB(sizeof(DWORD) == vb.GetSize());
				v = CValue(*(reinterpret_cast<const DWORD*>(vb.ReferBuffer())));
			}
			break;
		case EK_Longlong://64Bit Signed Integer
			vb = m_Query(lpszKey, REG_QWORD, REG_QWORD);
			if ( vb.IsValid() )
			{
				ASSERTLIB(sizeof(LONGLONG) == vb.GetSize());
				v = CValue(*(reinterpret_cast<const LONGLONG*>(vb.ReferBuffer())));
			}
			break;
		default:
			ASSERT(false);
			break;
		}
		return v;
	}

	/**
	 * [設定] 情報設定
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param value 設定する値。EK_Nothing の場合、キーを削除します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		m_SetSectionName(lpszSectionName);
		bool r = false;
		switch ( value.GetKind() )
		{
		case EK_Nothing://存在しない
			r = m_Delete(lpszKey);
			break;
		case EK_String://文字列
			{
				CStr s = value.QueryString();
				r = m_Write(lpszKey, REG_SZ, LPCTSTR(s), (s.GetLength() + 1) * sizeof(TCHAR));
			}
			break;
		case EK_PluralStrings://複数の文字列群
			{
				CDoubleNullTerminateStr dntstr;
				dntstr.Set(value.QueryPluralStrings());
				r = m_Write(lpszKey, REG_MULTI_SZ, LPCTSTR(dntstr), ToDword(dntstr.GetSize()) * sizeof(TCHAR));
			}
			break;
		case EK_Binary://バイナリ
			{
				const CByteVector& vb = value.QueryData();
				r = m_Write(lpszKey, REG_BINARY, vb.ReferBuffer(), ToDword(vb.GetSize()));
			}
			break;
		case EK_Dword://32Bit Unsigned Integer
			{
				DWORD dw = value.QueryDword();
				r = m_Write(lpszKey, REG_DWORD, &dw, sizeof(DWORD));
			}
			break;
		case EK_Longlong://64Bit Signed Integer
			{
				LONGLONG ll = value.QueryLonglong();
				r = m_Write(lpszKey, REG_QWORD, &ll, sizeof(LONGLONG));
			}
			break;
		default:
			ASSERT(false);
			break;
		}
		return r;
	}

private:
	enum { E_BufferSize = 1024 };			///< 取得最大サイズ
	REGSAM					m_dwRegSam;		///< アクセス権
	mutable CRegKeyHandle	m_hhParentKey;	///< 対象の親ハンドル
	mutable CStr			m_strSection;	///< 対象のサブキー
	mutable CRegKeyHandle	m_hhKey;		///< 対象のサブキーハンドル
	/// ハンドル閉じる
	void m_Close(void) const
	{
		m_hhKey.Null();
	}
	/**
	 * クリエート
	 *	@retval 0 成功
	 *	@retval 0以外 エラー。
	 */
	LONG m_Create(void) const
	{
		if ( m_Open() == 0 )
		{
			return 0;
		}
		HKEY hKey;
		DWORD dwDisposition;
		LONG lRc = ::RegCreateKeyEx(
						m_hhParentKey,	// 開いている親キーのハンドル
						m_strSection,	// 開くべきサブキーの名前
						0,				// 予約済み
						NULL,			// クラス名
						0,				// 特別なオプション
						m_dwRegSam,		// セキュリティアクセスマスク
						NULL,			// 継承の指定
						&hKey,			// [out] 開くことに成功したサブキーのハンドル
						&dwDisposition	// [out] 既存かどうか示す値
		);
		if ( lRc == ERROR_SUCCESS )
		{
			m_hhKey = hKey;
		}
		else
		{
			m_hhKey.Null();
		}
		return lRc;
	}
	/**
	 * オープン
	 *	@retval 0 成功。
	 *	@retval 0以外 エラー。
	 */
	LONG m_Open(void) const
	{
		if ( ! m_hhKey.IsNull() )
		{
			return 0;
		}
		HKEY hKey;
		LONG lRc = ::RegOpenKeyEx(
						m_hhParentKey,	// 開いている親キーのハンドル
						m_strSection,	// 開くべきサブキーの名前
						0,				// 予約済み
						m_dwRegSam,		// セキュリティアクセスマスク
						&hKey			// 開くことに成功したサブキーのハンドル
		);
		if ( lRc == ERROR_SUCCESS )
		{
			m_hhKey = hKey;
		}
		else
		{
			m_hhKey.Null();
		}
		return lRc;
	}
	/// セクション名設定
	void m_SetSectionName(LPCTSTR lpszSectionName) const
	{
		if ( m_strSection != lpszSectionName )
		{
			m_Close();
			m_strSection = lpszSectionName;
		}
	}
	/**
	 * [取得] セクション名一覧取得
	 *	@return セクション名一覧
	 */
	CStrVector m_EnumSections(HKEY hKey) const
	{
		CStrVector vstrRc;
		LONG lRc;
		DWORD dwIndex = 0;
		CStr strName;
		DWORD dwNameLength;
		FILETIME tFileTime;
		while ( true )
		{
			dwNameLength = E_BufferSize;
			lRc = ::RegEnumKeyEx(
							hKey,				// 列挙するべきキーのハンドル
							dwIndex,			// サブキーのインデックス番号
							strName.GetBuffer(dwNameLength + 1),
												// サブキー名が格納されるバッファ
							&dwNameLength,		// サブキー名バッファのサイズ
							NULL,				// 予約済み
							NULL,				// クラス名が格納されるバッファ
							NULL,				// クラス文字列バッファのサイズ
							&tFileTime			// 最終書き込み時刻
			);
			strName.ReleaseBuffer();
			if ( lRc == ERROR_NO_MORE_ITEMS )
			{
				break;
			}
			if ( lRc != ERROR_SUCCESS )
			{
				vstrRc.Invalid();
				return vstrRc;
			}
			vstrRc.Add(strName);
			dwIndex++;
		}
		return vstrRc;
	}
	/**
	 * 値読み込み
	 *	@param lpszKey 読み込むキー名
	 *	@param[in]	dwTyp1 読み込み許可するタイプ
	 *	@param[in]	dwTyp2 読み込み許可するタイプ
	 *	@param[out]	_pdwGetType 読み込んだタイプ。いらない場合、NULLを指定する。
	 *	@return 読み込んだバイナリ
	 */
	CByteVector m_Query(LPCTSTR lpszKey, DWORD dwTyp1, DWORD dwTyp2, DWORD* _pdwGetType = NULL) const
	{
		CByteVector vbResult;
		if ( m_Open() != 0 )
		{
			vbResult.Invalid();
			return vbResult; 
		}
		DWORD dwType;
		DWORD dwLength;
		LONG lRc = ::RegQueryValueEx(
						m_hhKey,			// キーのハンドル
						lpszKey,			// レジストリエントリ名
						NULL,				// 予約済み
						&dwType,			// データ型が格納されるバッファ
						NULL,				// データが格納されるバッファ
						&dwLength			// データバッファのサイズ
		);
		if ( lRc != ERROR_SUCCESS )
		{
			vbResult.Invalid();
			return vbResult;
		}
		if ( _pdwGetType != NULL )
		{
			*_pdwGetType = dwType;
		}
		if ( dwType != dwTyp1 && dwType != dwTyp2 )
		{
			vbResult.Invalid();
			return vbResult;
		}
		if ( dwLength > 0 )
		{
			lRc = ::RegQueryValueEx(
							m_hhKey,				// キーのハンドル
							lpszKey,				// レジストリエントリ名
							NULL,					// 予約済み
							&dwType,				// データ型が格納されるバッファ
							vbResult.GetBuffer(dwLength),	// データが格納されるバッファ
							&dwLength				// データバッファのサイズ
			);
			vbResult.ReleaseBuffer();
			if ( lRc != ERROR_SUCCESS )
			{
				vbResult.Invalid();
			}
		}
		return vbResult;
	}
	/**
	 * 値書き込み
	 *	@param[in]	lpszKey 書き込むキー名
	 *	@param[in]	dwTyp 書き込むタイプ
	 *	@param[in]	lpcData 書き込むデータ。
	 *	@param[in]	dwLen 書き込むデータ長。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool m_Write(LPCTSTR lpszKey, DWORD dwTyp, LPCVOID lpcData, size_t dwLen)
	{
		LONG lRc = 0xFFFFFFFF;
		if ( m_Create() == 0 )
		{
			lRc = ::RegSetValueEx(m_hhKey, lpszKey, NULL, dwTyp, static_cast<const BYTE*>(lpcData), ToDword(dwLen));
		}
		return (lRc == ERROR_SUCCESS);
	}
	/**
	 * [削除] 指定キー削除
	 *	@param lpszKey キー名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_Delete(LPCTSTR lpszKey)
	{
		LONG lRc = 0xFFFFFFFF;
		if ( m_Open() == 0 )
		{
			lRc = ::RegDeleteValue(m_hhKey, lpszKey);
		}
		return lRc == ERROR_SUCCESS;
	}
	/**
	 * [削除] 再帰レジストリ削除
	 *	@see http://support.microsoft.com/kb/142491/ja
	 *	@param hStartKey 対象キー
	 *	@param pKeyName 削除ネーム
	 *	@return リザルト
	 */
	static DWORD ms_RegDeleteKeyNT(HKEY hStartKey , LPCTSTR pKeyName)
	{
		DWORD dwRtn = 0;
		const int MAX_KEY_LENGTH = 256;
		CWorkMemT<TCHAR> subKey(MAX_KEY_LENGTH); // (256) this should be dynamic.
		HKEY hKey = NULL;
		if ( pKeyName != NULL && STRLIB::GetLen(pKeyName) > 0 )
		{
			dwRtn = ::RegOpenKeyEx(hStartKey, pKeyName, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hKey);
			if ( dwRtn == ERROR_SUCCESS )
			{
				while ( dwRtn == ERROR_SUCCESS )
				{
					DWORD dwSubKeyLength = MAX_KEY_LENGTH;
					dwRtn = ::RegEnumKeyEx(
									hKey,
									0,       // always index zero
									subKey,
									&dwSubKeyLength,
									NULL,
									NULL,
									NULL,
									NULL
								);
					if ( dwRtn == ERROR_NO_MORE_ITEMS )
					{
						::RegCloseKey(hKey);
						hKey = NULL;
						dwRtn = ::RegDeleteKey(hStartKey, pKeyName);
						break;
					}
					else if ( dwRtn == ERROR_SUCCESS )
					{
						dwRtn = ms_RegDeleteKeyNT(hKey, subKey);
					}
				}
				if ( hKey != NULL )
				{
					::RegCloseKey(hKey);
					hKey= NULL;
				}
			}
		}
		else
		{
			 dwRtn = ERROR_BADKEY;
		}
		return dwRtn;
	}
};



}; // TNB
