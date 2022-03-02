#pragma once
/**
 *@file
 * 拡張iniファイルアクセス関係のヘッダ
 *		
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifdef _WIN32_WCE
 #error not support "ExInifileAccessor"
#endif // _WIN32_WCE



#include "TnbInifileAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * 拡張iniファイルアクセスクラス
 *
 *		iniファイルにアクセスする関数群です。CInifileAccessorと異なり、型情報も管理しているため、
 *		互換が有りません(キーの値に型のPrefixをいれて方の管理をしています)。
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
 *	@date 06/05/16 新規作成
 *	@date 06/07/13 キー、値に =(イコール)、改行が含まれていてもOKになるようにした。
 *	@date 06/10/31 IAccessor の改修に対応。
 */
class CExInifileAccessor : public CInifileAccessor
{
	DEFSUPER(CInifileAccessor);
	enum { E_BufferSize = 65536 };		///< 取得最大サイズ
	/**
	 * [確認] 種類チェック
	 *	@param lpsz 文字列
	 *	@return 種類
	 */
	static EKind ms_CheckKind(LPCTSTR lpsz)
	{
		if ( lpsz[0] != '_' || lpsz[2] != '_' )
		{
			return EK_Nothing;
		}
		switch ( lpsz[1] )
		{
		case EK_String:			// 文字列
		case EK_PluralStrings:	// 複数の文字列群
		case EK_Binary:			// バイナリ
		case EK_Dword:			// 32Bit Unsigned
		case EK_Longlong:		// 64Bit Signed
			break;
		default:
			return EK_Nothing;
		}
		return static_cast<EKind>(lpsz[1]);
	}

	/**
	 * 書込み用文字列作成
	 *	@param lpsz オリジナル文字列
	 *	@return 書込み用文字列
	 */
	static CStr ms_ToWritingStr(LPCTSTR lpsz)
	{
		CStr s = lpsz;
		if ( s.IsEmpty() )
		{
			s = _T("@");
		}
		else
		{
			s.Replace(_T("\\"), _T("\\\\"));
			s.Replace(_T("\n"), _T("\\n"));
			s.Replace(_T("\r"), _T("\\r"));
			s.Replace(_T("="), _T("\\-"));
			s.Replace(_T(" "), _T("\\_"));
		}
		return s;
	}

	/**
	 * オリジナル文字列作成
	 *	@param lpsz 書込み用文字列
	 *	@return オリジナル文字列
	 */
	static CStr ms_ToOriginalStr(LPCTSTR lpsz)
	{
		CStr s = lpsz;
		s.Replace(_T("\\_"), _T(" "));
		s.Replace(_T("\\n"), _T("\n"));
		s.Replace(_T("\\r"), _T("\r"));
		s.Replace(_T("\\-"), _T("="));
		s.Replace(_T("\\\\"), _T("\\"));
		return s;
	}

	/**
	 * [追加] 文字列情報記録
	 *	@param eKind 書き込む種類
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param lpszData キーの値（文字列化したもの）
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_Write(EKind eKind, LPCTSTR lpszSectionName, LPCTSTR lpszKey, LPCTSTR lpszData)
	{
		CStr s = CStr::Fmt(_T("_%c_%s"), eKind, lpszData);
		return !! ::WritePrivateProfileString(
							lpszSectionName,
							ms_ToWritingStr(lpszKey),
							ms_ToWritingStr(s),
							GetBaseFileName()
		);
	}

	/**
	 * [取得] 文字列情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 */
	CStr m_Query(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CStr strValue;
		::GetPrivateProfileString(
								lpszSectionName,
								ms_ToWritingStr(lpszKey),
								_T(""),
								strValue.GetBuffer(E_BufferSize),
								E_BufferSize,
								GetBaseFileName());
		strValue.ReleaseBuffer();
		strValue = ms_ToOriginalStr(strValue); 
		return strValue.Mid(3);
	}

public:

	/**
	 * コンストラクタ
	 *	@param lpszFile ファイル名 。省略すると、 MakeInifilePath() の結果が使われます。
	 */
	explicit CExInifileAccessor(LPCTSTR lpszFile = NULL) : _super(lpszFile)
	{
	}

	/**
	 * [取得] キー名一覧取得
	 *	@param lpszSectionName セクション名
	 *	@return 名前一覧。無効状態なら未対応。
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		CStrVector vstr = _super::EnumKeyNames(lpszSectionName);
		loop ( i, vstr.GetSize() )
		{
			vstr[i] = ms_ToOriginalStr(vstr[i]);
		}
		return vstr;
	}

	/**
	 * [取得] 情報取種取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return 情報種。ない場合、EK_Nothing になります。
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		TCHAR atcBuf[16];
		DWORD dwRc = ::GetPrivateProfileString(
								lpszSectionName,
								ms_ToWritingStr(lpszKey),
								_T(""),
								atcBuf,
								16,
								GetBaseFileName());
		if ( dwRc < 3 )
		{
			return EK_Nothing;
		}
		return ms_CheckKind(atcBuf);
	}

	/**
	 * [取得] 情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 *	@throw CEmptyException	情報がない時、スローされます。
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CValue v;
		CStr str = m_Query(lpszSectionName, lpszKey);
		switch ( GetKeyKind(lpszSectionName, lpszKey) )
		{
		case EK_Nothing:
			break;
		case EK_String:
			v = CValue(str);
			break;
		case EK_PluralStrings:
			v = CValue(StrToStrVectorEx(str));
			break;
		case EK_Binary:
			v = CValue(CStrOperator::HexStringToBinary(str));
			break;
		case EK_Dword:
			v = CValue(static_cast<DWORD>(str.ToLonglong()));
			break;
		case EK_Longlong:
			v = CValue(str.ToLonglong());
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
	 *	@throw CTnbException 設定失敗時、スローされる可能性があります。
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value) 
	{
		EKind kind = value.GetKind();
		if ( kind == EK_Nothing )
		{
			return _super::WriteValue(lpszSectionName, lpszKey, value);
		}
		return m_Write(kind, lpszSectionName, lpszKey, value.QueryString());
	}
};



}; // TNB
