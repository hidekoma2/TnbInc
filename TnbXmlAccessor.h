#pragma once
/**
 *@file
 * XMLアクセサー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbXml.h"
#include "TnbAccessor.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR XML
 * XMLアクセサークラス.
 *
 *		XMLファイルにアクセスしてデータを格納するためのクラスです。
 *
 *	@par必要ファイル
 *			TnbXmlAccessor.h
 *
 *	@date 08/04/15 新規作成
 */
class CXmlAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
	CXmlElement			m_masterElement;///< 追加先の親 Element。
	mutable CXmlElement	m_element;		///< アクセス対象の Element。
	mutable CStr		m_strSection;	///< 対象のサブキー

	/**
	 * セクション名設定
	 *	@param lpszSectionName セクション名。 その名前の Elementを探し記憶します。
	 *	@param isCreateMode true なら、指定のセクション名がなければ Elementを作成します。
	 */
	bool m_SetSectionName(LPCTSTR lpszSectionName, bool isCreateMode) const
	{
		CStr sec = lpszSectionName;
		sec.Replace('\\', '/');
		if ( sec.IsEmpty() )
		{
			m_element = m_masterElement;
			m_strSection.Empty();
		}
		else if ( m_strSection != sec )
		{
			if ( ! isCreateMode )
			{
				m_element = m_masterElement.SelectSingleNode(sec);	//これで大丈夫か？
			}
			else
			{
				m_element = m_masterElement;
				while ( ! sec.IsEmpty() )
				{
					CStr s;
					INDEX idx = sec.Find('/');
					if ( idx == INVALID_INDEX )
					{
						s = sec;
						sec.Empty();
					}
					else
					{
						s = sec.Left(idx);
						sec = sec.Mid(idx + 1);
					}
					bool find = false;
					CXmlNodeList nlist = m_element.GetChildNodes();
					loop ( i, nlist.GetLength() )
					{
						if ( nlist.GetNodeType(i) == MSXML::NODE_ELEMENT )
						{
							CXmlElement e = nlist.GetNode(i);
							if ( e.GetNodeName() == s )
							{
								find = true;
								m_element = e;
								break;
							}
						}
					}
					if ( ! find )
					{
						m_element = m_element.AppendChildElement(s);
					}
				}
			}
		}
		TRACE2( "master,current =[%s],[%s]\n", m_masterElement.GetElementFullPath(), m_element.GetElementFullPath() );
		bool r = m_element.IsValid();
		if ( ! r )
		{
			m_strSection.Empty();
		}
		return r;
	}

public:

	/// コンストラクタ 
	CXmlAccessor(void) : m_masterElement(), m_strSection(_T("//"))
	{
	}

	/**
	 * コンストラクタ 
	 *	@param other エレメント
	 */
	explicit CXmlAccessor(const CXmlElement& other) 
		: m_masterElement(other), m_strSection(_T("//"))
	{
	}

	/**
	 * デストラクタ
	 */
	virtual ~CXmlAccessor(void) 
	{
		m_element.Invalid();
		m_masterElement.Invalid();
	}

	/**
	 * [設定] 対象エレメント指定
	 *	@param other エレメント
	 */
	void SetBase(const CXmlElement& other)
	{
		m_masterElement = other;
		m_strSection = _T("//");
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("XML");
	}

	/**
	 * [操作] フラッシュ.
	 *	@note 本クラスでは何もしません。
	 *	@return true 成功
	 */
	virtual bool Flush(void)
	{
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
		if ( m_SetSectionName(lpszSectionName, false) )
		{
			CXmlNodeList nlist = m_element.GetChildNodes();
			loop ( i, nlist.GetLength() )
			{
				if ( nlist.GetNodeType(i) == MSXML::NODE_ELEMENT )
				{
					CXmlElement e = nlist.GetNode(i);
					vs.Add(e.GetNodeName());
				}
			}
		}
		return vs;
	}

	/**
	 * [削除] 指定セクション削除
	 *	@param lpszSectionName セクション名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		if ( m_SetSectionName(lpszSectionName, false) )
		{
			m_element.Remove();
			return true;
		}
		return false;
	}

	/**
	 * [取得] 名前一覧取得
	 *	@note 本クラスでは pure関数です。
	 *	@param lpszSectionName セクション名
	 *	@return 名前一覧。無効状態なら未対応。
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		CStrVector vs;
		if ( m_SetSectionName(lpszSectionName, false) )
		{
			loop ( i, m_element.GetAttributeLength() )
			{
				CXmlAttribute a = m_element.GetAttribute(i);
				if ( a.IsValid() )
				{
					vs.Add(a.GetName());
				}
			}
		}
		return vs;
	}

	/**
	 * [取得] 情報取種取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return 情報種。ない場合、EK_Nothing になります。
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		EKind kind = EK_Nothing;
		if ( m_SetSectionName(lpszSectionName, false) )
		{
			CXmlAttribute a = m_element.GetAttributeByName(lpszKey);
			if ( a.IsValid() )
			{
				kind = EK_String;
			}
		}
		return kind;
	}

	/**
	 * [取得] 情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		if ( m_SetSectionName(lpszSectionName, false) )
		{
			CXmlAttribute a = m_element.GetAttributeByName(lpszKey);
			if ( a.IsValid() )
			{
				return CValue(a.GetValue());
			}
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
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		bool r = false;
		if ( value.GetKind() == EK_Nothing )
		{
			if ( m_SetSectionName(lpszSectionName, false) )
			{
				r = m_element.RemoveAttribute(lpszKey);
			}
		}
		else
		{
			if ( m_SetSectionName(lpszSectionName, true) )
			{
				CXmlAttribute a = m_element.SetAttribute(lpszKey, value.QueryString());
				r = a.IsValid();
			}
		}
		return r;
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
