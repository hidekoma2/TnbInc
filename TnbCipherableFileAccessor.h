#pragma once
/**
 *@file
 * 暗号ファイル情報アクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBytesCipher.h"
#include "TnbTemporaryAccessor.h"
#include "TnbFileName.h"
#include "TnbFile.h"
#include "TnbSerializeAdapter.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR CIPHER
 * 暗号ファイル情報アクセスクラス
 *
 *		セクションで区切られた中に複数のキーと値を持つ形の情報をメモリで管理し、
 *		任意のタイミングで、ファイルに保存しておき、次回情報を復元することが
 *		出来るクラスです。このファイルは、暗号化されており、また、CRCも付加して
 *		あるため、保存情報を参照禁止、改ざん禁止したい場合に使用できます。
 *
 *		コンストラクタや SetBase() で情報のベースを指定し、
 *		operator[]() で セクション情報アクセスクラス({@link IAccessor::CSection CSection})を取得し、
 *		Query～() 、 Write～() などでキー情報にアクセスします。
 *
 *	@attention Write～() で情報を変更しても、 Flush() をコールしない限りファイルに書き出しません。
 *
 *	@note	本インターフェースにも QueryValue() , WriteValue() など、
 *			セクションを指定しキー情報を操作するメソッドもありますが、
 *			基本的に {@link IAccessor::CSection CSection} を使用してください。
 *
 *	@see	IIAccessor を実装しています。
 *
 *	@par必要ファイル
 *			TnbCipherableFileAccessor.h
 *
 *	@date 06/06/12 新規作成
 */
class CCipherableFileAccessor : public CTemporaryAccessor
{
	DEFSUPER(CTemporaryAccessor);
	CStr			m_strFileName;		///< ファイル名
	CStr			m_strHeadText;		///< ヘッダ識別子
	CBytesCipher		m_cipher;		///< 暗号処理クラス

	/**
	 * ファイル読み込み
	 *	@param lpszFile ファイル名
	 */
	void m_Read(LPCTSTR lpszFile)
	{
		_super::DeleteAllSection();
		CVectorT<BYTE> vb;
		TNB::LoadCollection(lpszFile, m_strHeadText, vb);
		vb = m_cipher.Decode(vb);
		if ( ! vb.IsValid() )
		{
			throw CReadFailureException(ERROR_ACCESS_DENIED);
		}
		CDeserializeAdapter sr(&vb);
		sr >> CSerializeTag(_T("CiphFA"));
		sr >> _super::m_mapSec;
	}

	/**
	 * ファイル書き出し
	 *	@param lpszFile ファイル名
	 */
	void m_Write(LPCTSTR lpszFile)
	{
		CVectorT<BYTE> vb;
		CSerializeAdapter sr(&vb);
		sr << CSerializeTag(_T("CiphFA"));
		sr << _super::m_mapSec;
		TNB::SaveCollection(lpszFile, m_strHeadText, m_cipher.Encode(vb));
	}

public:
	
	/** 
	 * コンストラクタ
	 *	@param lpszFileName ファイル名
	 *	@param lpszHeadText ヘッダ文字列
	 *	@param lpszKey 暗号キー
	 *	@throw CNotSupportException		指定ファイルのヘッダ形状が異なるとスローされます。
	 *	@throw CInvalidParamException	指定ファイルのファイル長さが異なるとスローされます。
	 *	@throw CNoSuchTypeException		指定ファイルのチェックサムが異なるとスローされます。
	 *	@throw CReadFailureException	指定ファイルの読み込みに失敗するとスローされます。 
	 */
	explicit CCipherableFileAccessor(LPCTSTR lpszFileName, LPCTSTR lpszHeadText, LPCTSTR lpszKey) : _super()
	{
		SetBase(lpszFileName, lpszHeadText, lpszKey);
	}

	/**
	 * [設定] ベース指定
	 *	@param lpszFileName ファイル名
	 *	@param lpszHeadText ヘッダ文字列
	 *	@param lpszKey 暗号キー
	 *	@throw CNotSupportException		指定ファイルのヘッダ形状が異なるとスローされます。
	 *	@throw CInvalidParamException	指定ファイルのファイル長さが異なるとスローされます。
	 *	@throw CNoSuchTypeException		指定ファイルのチェックサムが異なるとスローされます。
	 *	@throw CReadFailureException	指定ファイルの読み込みに失敗するとスローされます。 
	 */
	void SetBase(LPCTSTR lpszFileName, LPCTSTR lpszHeadText, LPCTSTR lpszKey)
	{
		m_cipher.SetKey(lpszKey);
		m_cipher.Compression(true);
		m_strFileName = lpszFileName;
		m_strHeadText= lpszHeadText;
		CFileName fn = lpszFileName;
		if ( fn.IsExist() )
		{
			m_Read(fn);
		}
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("CipherableFile");
	}

	/**
	 * [操作] フラッシュ.
	 *	@note コンストラクタや SetBase で指定したファイルに変更を書き出します。
	 *		このメソッドをコールしない限り、ファイルは変化しません。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool Flush(void)
	{
		CFileName fn = m_strFileName;
		try
		{
			m_Write(fn);
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			return false;
		}
		catch(...)
		{
			return false;
		}
		return true;
	}
};



/**@ingroup VALUE
 * [追加] シリアライズ
 *	@param[out] _sr シリアライザー
 *	@param[in] t データ
 *	@return ISerializer の参照
 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
 */
inline ISerializer& operator<<(ISerializer& _sr, const IAccessor::CValue& t)
{
	DWORD dwKind = t.GetKind();
	_sr << dwKind;
	//
	switch ( dwKind )
	{
	case IAccessor::EK_Nothing:// 存在しない
		break;
	case IAccessor::EK_String:	// 文字列
		_sr << t.QueryString();
		break;
	case IAccessor::EK_PluralStrings:	// 複数の文字列群
		_sr << t.QueryPluralStrings();
		break;
	case IAccessor::EK_Binary:		// バイナリ
		_sr << t.QueryData();
		break;
	case IAccessor::EK_Dword:		// 32Bit Unsigned Integer
		_sr << t.QueryDword();
		break;
	case IAccessor::EK_Longlong:	// 64Bit Signed Integer
		_sr << t.QueryLonglong();
		break;
	default:
		ASSERTLIB(false);
		break;
	}
	return _sr;
}

/**@ingroup VALUE
 * [取得] デシリアライズ
 *	@param[in] ds デシリアライザー
 *	@param[out] _t データ
 *	@return IDeserializer の参照
 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, IAccessor::CValue& _t)
{
	DWORD dwKind;
	ds >> dwKind;
	//
	switch ( dwKind )
	{
	case IAccessor::EK_Nothing:// 存在しない
		_t = IAccessor::CValue();
		break;
	case IAccessor::EK_String:	// 文字列
		{
			CStr s;
			ds >> s;
			_t = IAccessor::CValue(s);
		}
		break;
	case IAccessor::EK_PluralStrings:	// 複数の文字列群
		{
			CStrVector vs;
			ds >> vs;
			_t = IAccessor::CValue(vs);
		}
		break;
	case IAccessor::EK_Binary:		// バイナリ
		{
			CByteVector vb;
			ds >> vb;
			_t = IAccessor::CValue(vb);
		}
		break;
	case IAccessor::EK_Dword:		// 32Bit Unsigned Integer
		{
			DWORD dw;
			ds >> dw;
			_t = IAccessor::CValue(dw);
		}
		break;
	case IAccessor::EK_Longlong:		// 64Bit Signed Integer
		{
			LONGLONG ll;
			ds >> ll;
			_t = IAccessor::CValue(ll);
		}
		break;
	default:
		ASSERTLIB(false);
		_t = IAccessor::CValue();
		break;
	}
	return ds;
}



};//TNB