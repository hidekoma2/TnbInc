#pragma once
/**
 *@file
 * XMLファイルアクセサー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbXmlAccessor.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR XML
 * XMLファイルアクセサークラス.
 *
 *		XMLファイルにアクセスしてデータを格納するためのクラスです。
 *
 *	@todo エラー処理が不十分
 *
 *	@par必要ファイル
 *			TnbXmlAccessor.h
 *
 *	@date 08/04/22 新規作成
 */
class CXmlFileAccessor : public CXmlAccessor
{
	DEFSUPER(CXmlAccessor);
	CStr			m_strFileName;		///< ファイル名
	CXmlDocument	m_xml;				///< XML

public:
	
	/** 
	 * コンストラクタ
	 */
	explicit CXmlFileAccessor(void) : _super()
	{
	}

	/** 
	 * コンストラクタ
	 *	@param lpszFileName ファイル名
	 *	@param lpszRootText ヘッダ文字列
	 */
	explicit CXmlFileAccessor(LPCTSTR lpszFileName, LPCTSTR lpszRootText) : _super()
	{
		SetBase(lpszFileName, lpszRootText);
	}

	/// デストラクタ
	virtual ~CXmlFileAccessor(void)
	{
	}

	/**
	 * [設定] ベース指定
	 *	@param lpszFileName ファイル名
	 *	@param lpszRootText ヘッダ文字列
	 */
	void SetBase(LPCTSTR lpszFileName, LPCTSTR lpszRootText)
	{
		m_strFileName = lpszFileName;
		if ( ! m_xml.LoadFile(lpszFileName) )
		{
			m_xml.InitializeXml(lpszRootText);
		}
		_super::SetBase(m_xml.GetRootElement());
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("XmlFile");
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
		#ifdef _TnbXML_MsXml6_ENABLE
			return m_xml.SaveFileEx(m_strFileName);
		#else
			return m_xml.SaveFile(m_strFileName);
		#endif
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
