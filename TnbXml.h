#pragma once
/**
 *@file
 * XML関係のヘッダ
 *
 *		XML パーサをラップしたクラスがあります。
 *
 *	@note	マクロ "_TnbXML_MsXml6_ENABLE" が定義されていると、 MSXML6.dll を使用します。
 *			MSXML6.dll を使用しないと、 CXmlDocument::SaveFileEx() メソッドは使用できません。
 *	
 *	@note	VC++6 の場合、古い MSXML6.dll(2019/02/09以前) をインポートしないとリンクエラーになります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#ifndef _TnbDOXYGEN	//Document作成用シンボル
  #ifndef _TnbXML_MsXml6_ENABLE
	#import <msxml.dll> rename("value", "Value")
  #else
	#include "TnbDynamicFunc.h"
    #import "msxml6.dll" rename("value", "Value")
    #define MSXML MSXML2
  #endif
#endif

#if ! defined(__MSVCPP6__) && ! defined(_UNICODE)
 #pragma comment(lib,"comsuppw.lib")
 #pragma comment(lib,"comsuppwd.lib")
#endif



//TNB Library
namespace TNB
{



/**@ingroup XML
 * XML ノードポインタハンドルクラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
typedef MSXML::IXMLDOMNodePtr CXmlNodePtr;



/**@ingroup XML
 * XML ノードリスト管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは複数のノードをまとめて管理します。
 *
 *	@note Element, TEXT, COMMENT などのノードを追加できます。
 *
 *	@note	インスタンスは CXmlElement::GetChildNodes() より作成されます。
 *			直接を作ることはありません。
 *	
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlNodeList
{
public:

	/// ノード種類型
	enum ENodeType
	{
		ELEMENT,	///< Element
		TEXT,		///< 文字列
		COMMENT,	///< コメント
	};

private:

	MSXML::IXMLDOMNodeListPtr	m_pNodes;	///< ノードリストクラスのポインタハンドル
	CXmlNodePtr					m_pParent;	///< 親のポインタハンドル
	MSXML::IXMLDOMDocumentPtr	m_pDoc;		///< DOCのポインタハンドル

	/// 新しいノード作成
	CXmlNodePtr m_MakeNewNode(ENodeType type, LPCTSTR text)
	{
		if ( m_pDoc == NULL )
		{
			m_pDoc = m_pParent->GetownerDocument();
		}
		CXmlNodePtr n = NULL;
		_bstr_t str = text;
		switch ( type )
		{
		case ELEMENT:
			n = m_pDoc->createElement(str);
			break;
		case TEXT:
			n = m_pDoc->createTextNode(str);
			break;
		case COMMENT:
		default:
			n = m_pDoc->createComment(str);
			break;
		}
		return n;
	}

public:

	/** 
	 * コンストラクタ
	 *	@param na IXMLDOMNodeList のポインタハンドル。
	 *	@param pp na の Parent ノードのポインタハンドル。
	 *	@param dc na の Document のポインタハンドル。省略すると、 pp から取得します。
	 */
	CXmlNodeList(MSXML::IXMLDOMNodeListPtr na, CXmlNodePtr pp, MSXML::IXMLDOMDocumentPtr dc = NULL)
		: m_pNodes(na), m_pParent(pp), m_pDoc(dc)
	{
	}

	/**
	 * [確認] 有効確認.
	 *		本インスタンスが有効な情報を保持しているか、確認できます。
	 *	@retval true 有効。
	 *	@retval false 無効。
	 */
	bool IsValid(void) const
	{
		return m_pNodes != NULL;
	}

	/** 
	 * [取得] ポインタハンドル取得.
	 *		本インスタンスで管理している IXMLDOMNodeList ポインタハンドルを得ることが出来ます。
	 *	@note 直接 XMLパーサのメソッドを使用できます。
	 *	@return IXMLDOMNodeList ポインタハンドル
	 */
	MSXML::IXMLDOMNodeListPtr operator->(void)
	{
		return m_pNodes;
	}

	/** 
	 * [取得] 管理ノード数取得.
	 *		本インスタンスで管理しているノード数を取得することが出来ます。
	 *	@return ノード数。
	 */
	size_t GetLength(void) const
	{
		return m_pNodes->Getlength();
	}

	/**
	 * [取得] ノード取得.
	 *		本インスタンスで管理しているノードを取得することが出来ます。
	 *	@param index インデックス。０～ GetLength() -1 まで。
	 *	@return ノード。 CXmlNode , CXmlElement で受けることも出来ます。
	 */
	CXmlNodePtr GetNode(INDEX index) const
	{
		return m_pNodes->Getitem(static_cast<long>(index));
	}

	/** 
	 * [取得] ノードタイプ取得.
	 *		本インスタンスで管理しているノードのタイプを取得します。
	 *	@param index インデックス。０～ GetLength()-1 まで。
	 *	@retval MSXML::NODE_INVALID					無効
	 *	@retval MSXML::NODE_ELEMENT					要素
	 *	@retval MSXML::NODE_ATTRIBUTE				属性
	 *	@retval MSXML::NODE_TEXT					文字列
	 *	@retval MSXML::NODE_CDATA_SECTION			CDATA
	 *	@retval MSXML::NODE_ENTITY_REFERENCE		？
	 *	@retval MSXML::NODE_ENTITY					？
	 *	@retval MSXML::NODE_PROCESSING_INSTRUCTION	？
	 *	@retval MSXML::NODE_COMMENT					コメント
	 *	@retval MSXML::NODE_DOCUMENT				ドキュメント
	 *	@retval MSXML::NODE_DOCUMENT_TYPE			？
	 *	@retval MSXML::NODE_DOCUMENT_FRAGMENT		？
	 *	@retval MSXML::NODE_NOTATION				特殊
	 */
	MSXML::DOMNodeType GetNodeType(INDEX index) const
	{
		return GetNode(index)->GetnodeType();
	}

	/**
	 * [追加] ノード追加.
	 *		本インスタンスで管理しているノード群の最尾後に指定のノードを追加します。
	 *	@param newItem 追加するノード。
	 *	@return ノード(追加された実態のノードの参照)。 CXmlNode , CXmlElement などで受けることも出来ます。
	 */
	CXmlNodePtr AppendNode(CXmlNodePtr newItem)
	{
		return m_pParent->appendChild(newItem);
	}

	/**
	 * [追加] ノード追加.
	 *		本インスタンスで管理しているノード群の最尾後に指定のノードを追加します。
	 *	@param type 追加するタイプ。
	 *	@param text 追加する文字列。
	 *	@return ノード(追加された実態のノードの参照)。 CXmlNode , CXmlElement で受けることも出来ます。
	 */
	CXmlNodePtr AppendNode(ENodeType type, LPCTSTR text)
	{
		return AppendNode(m_MakeNewNode(type, text));
	}

	/**
	 * [追加] ノード追加.
	 *		本インスタンスで管理しているノード群の指定の箇所に指定のノードを追加します。
	 *	@param index 追加するインデックス。追加したノードのインデックスになります。
	 *	@param newItem 追加するノード。
	 *	@return ノード(追加された実態のノードの参照)。 CXmlNode , CXmlElement で受けることも出来ます。
	 */
	CXmlNodePtr InsertNode(INDEX index, CXmlNodePtr newItem)
	{
		if ( GetLength() > index )
		{
			return m_pParent->insertBefore(newItem, GetNode(index).GetInterfacePtr());
		}
		return m_pParent->appendChild(newItem);
	}

	/**
	 * [追加] ノード追加.
	 *		本インスタンスで管理しているノード群の指定の箇所に指定のノードを追加します。
	 *	@param index 追加するインデックス。追加したノードのインデックスになります。
	 *	@param type 追加するタイプ。
	 *	@param text 追加する文字列。
	 *	@return ノード(追加された実態のノードの参照)。 CXmlNode , CXmlElement で受けることも出来ます。
	 */
	CXmlNodePtr InsertNode(INDEX index, ENodeType type, LPCTSTR text)
	{
		return InsertNode(index, m_MakeNewNode(type, text));
	}

	/**
	 * [操作] ノード入れ替え.
	 *		本インスタンスで管理しているノード群の２つの指定のノードを入れ替えます。
	 *	@param index1 入れ替え対象のノードのインデックスになります。
	 *	@param index2 入れ替え対象のノードのインデックスになります。
	 */
	void ReplaceNode(INDEX index1, INDEX index2)
	{
		m_pParent->replaceChild(GetNode(index1), GetNode(index2));
	}

	/**
	 * [削除] ノード削除.
	 *	@param index 削除するインデックス。
	 */
	void RemoveNode(INDEX index)
	{
		m_pParent->removeChild(GetNode(index));
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML	属性管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは属性一つを管理します。
 *
 *	@note	CXmlElement::GetAttribute() , CXmlElement::GetAttributeByName() , CXmlElement::SetAttribute() で作成します。
 *			通常直接インスタンスを作ることはありません。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlAttribute
{
	MSXML::IXMLDOMAttributePtr m_pAttr;	///< IXMLDOMAttribute ポインタハンドル

public:

	/**
	 * コントラクタ
	 *	@param p IXMLDOMAttribute ポインタハンドル
	 */
	CXmlAttribute(MSXML::IXMLDOMAttributePtr p) : m_pAttr(p)
	{
	}

	/**
	 * [確認] 有効確認.
	 *		本インスタンスが有効な情報を保持しているか、確認できます。
	 *	@retval true 有効。
	 *	@retval false 無効。
	 */
	bool IsValid(void) const
	{
		return m_pAttr != NULL;
	}

	/** 
	 * [取得] ポインタハンドル取得.
	 *		本インスタンスで管理している IXMLDOMAttribute ポインタハンドルを得ることが出来ます。
	 *	@note 直接 XMLパーサのメソッドを使用できます。
	 *	@return IXMLDOMAttribute ポインタハンドル
	 */
	MSXML::IXMLDOMAttributePtr operator->(void)
	{
		return m_pAttr;
	}

	/**
	 * [取得] 名前取得.
	 *	@return 名前
	 */
	CStr GetName(void) const
	{
		_bstr_t r = m_pAttr->Getname();
		return CStr(LPCTSTR(r));
	}
 
	/**
	 * [取得] 値取得.
	 *	@return 値
	*/
	CStr GetValue(void) const
	{
		_bstr_t r = m_pAttr->GetValue();
		return CStr(LPCTSTR(r));
	}

	/**
	 * [設定] 値設定.
	 *	@note 本インスタンスの値を設定したら実態の属性が変化します。
	 *	@param text 値
	 */
	void PutValue(LPCTSTR text)
	{
		m_pAttr->PutValue(text);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML ノード管理基礎クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *	@note 基礎クラスです。直接使用しません。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CAbstractXmlNode
{
	HRESULT		m_initRes;	///< CoInitializeの戻り値
protected:

	CXmlNodePtr m_pNode;	///< ノードクラスのポインタハンドル

	/** 
	 * コンストラクタ
	 *	@note 直接作成できない用に protected 宣言にしています。
	 *	@param p ノードポインタハンドル
	 */
	CAbstractXmlNode(CXmlNodePtr p) : m_pNode(p)
	{
		m_initRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	}

	/// デストラクタ
	virtual ~CAbstractXmlNode(void)
	{
		if ( m_initRes == S_OK )
		{
			::CoUninitialize();
		}
	}

public:

	/** 
	 * [取得] ポインタハンドル取得.
	 *		本インスタンスで保持している {@link TNB::CXmlNodePtr CXmlNodePtr} を得ることが出来ます。
	 *	@note 直接 XMLパーサのメソッドを使用できます。
	 *	@return ノードポインタハンドル
	 */
	CXmlNodePtr operator->(void)
	{
		return m_pNode;
	}

	/**
	 * [確認] 有効確認.
	 *		本インスタンスが有効な情報を保持しているか、確認できます。
	 *	@retval true 有効。
	 *	@retval false 無効。
	 */
	bool IsValid(void) const
	{
		return m_pNode != NULL;
	}

	/**
	 * [設定] 無効化.
	 */
	void Invalid(void)
	{
		m_pNode = NULL;
	}

	/** 
	 * [取得] 名前取得.
	 *		本インスタンスで保持しているノードの名前を取得します。
	 *	@note ノードのタイプによって名前の意味は異なります。
	 *	@return 名前。
	 */
	CStr GetNodeName(void) const
	{
		if ( ! IsValid() ) { return CStr(); }
		_bstr_t r = m_pNode->GetnodeName();
		return LPCTSTR(r);
	}

	/** 
	 * [取得] 値取得.
	 *		本インスタンスで保持しているノードの値を取得します。
	 *	@note ノードのタイプによって値の意味は異なります。
	 *	@return 値。
	 */
	CStr GetNodeValue(void) const
	{
		if ( ! IsValid() ) { return CStr(); }
		_bstr_t r = m_pNode->GetnodeValue();
		return LPCTSTR(r);
	}

	/**
	 * [削除] 自分自身を削除.
	 *		以下にぶら下がるノードがあれば、それも消えます。
	 *	@note 本インスタンスは無効状態になります。以降の操作が出来ません。
	 */
	void Remove(void)
	{
		m_pNode->GetparentNode()->removeChild(m_pNode);
		m_pNode = NULL;
	}

	/** 
	 * [取得] 親のノード取得.
	 *	@note 親のノードがない場合、無効な CXmlNode が返ります。
	 *	@return ノード。
	 */
	CXmlNodePtr GetParentNode(void) const
	{
		return IsValid() ? m_pNode->GetparentNode() : NULL;
	}	

	/** 
	 * [取得] 隣のノード取得.
	 *	@note 隣のノードがない場合、無効な CXmlNode が返ります。
	 *	@return ノード。
	 */
	CXmlNodePtr GetNextSibling(void) const
	{
		return IsValid() ? m_pNode->GetnextSibling() : NULL;
	}	

	/**
	 * [作成] ルートからの Element名.
	 *		自分自身がある階層を、Element 名の連結で表します。
	 *	@par ex)
	 *		/root/sub1/sub2 
	 *	@return Element階層名
	 */
	CStr GetElementFullPath(void) const
	{
		CStr str;
		if ( IsValid() ) 
		{
			CXmlNodePtr n = m_pNode->GetparentNode();
			while ( n != NULL )
			{
				if ( n->GetnodeType() == MSXML::NODE_DOCUMENT )
				{
					break;
				}
				str = CStr::Fmt(_T("/%s%s"), LPCTSTR(n->GetnodeName()), str);
				n = n->GetparentNode();
			}
		}
		return str;
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML ノード管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスではノード(Element, Text, Attribute,など)一つを管理します。
 *
 *	@note インスタンスは基本的に CXmlNodeList などから作成し、直接インスタンスを作ることはありません。
 *
 *	@note 子ノード操作、属性操作はできません。 CXmlElement に代入して行ってください。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlNode : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
public:

	/** 
	 * コンストラクタ
	 *	@param p ノードポインタハンドル
	 */
	CXmlNode(CXmlNodePtr p) : _super(p)
	{
	}

	/** 
	 * [取得] ノードタイプ取得.
	 *		本インスタンスで保持しているノードのタイプを取得します。
	 *	@retval MSXML::NODE_INVALID					無効
	 *	@retval MSXML::NODE_ELEMENT					要素
	 *	@retval MSXML::NODE_ATTRIBUTE				属性
	 *	@retval MSXML::NODE_TEXT					文字列
	 *	@retval MSXML::NODE_CDATA_SECTION			CDATA
	 *	@retval MSXML::NODE_ENTITY_REFERENCE		？
	 *	@retval MSXML::NODE_ENTITY					？
	 *	@retval MSXML::NODE_PROCESSING_INSTRUCTION	(最初の <?xml?> ノード )
	 *	@retval MSXML::NODE_COMMENT					コメント
	 *	@retval MSXML::NODE_DOCUMENT				ドキュメント
	 *	@retval MSXML::NODE_DOCUMENT_TYPE			？
	 *	@retval MSXML::NODE_DOCUMENT_FRAGMENT		？
	 *	@retval MSXML::NODE_NOTATION				特殊
	 */
	MSXML::DOMNodeType GetNodeType(void) const
	{
		return IsValid() ? m_pNode->GetnodeType() : MSXML::DOMNodeType(MSXML::NODE_INVALID);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML Text 管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは Text ノードを一つを管理します。
 *
 *	@note インスタンスは基本的に CXmlElement::AppendChildText() の戻り値や CXmlNodeList から作成したり、 
 *			CXmlNode を代入宣言します。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlText : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
public:

	/** 
	 * コンストラクタ
	 *	@note 無効なインスタンスに成ります。
	 */
	CXmlText(void) : _super(NULL)
	{
	}

	/** 
	 * コンストラクタ
	 *	@note 指定のノードが Text ではない時、無効なインスタンスに成ります。
	 *	@param n ノード管理クラス
	 */
	CXmlText(CXmlNode n) : _super(NULL)
	{
		if ( n.GetNodeType() == MSXML::NODE_TEXT )
		{
			m_pNode = n.operator ->();
		}
	}

	/**
	 * [代入] 代入
	 *	@param n ノード管理
	 *	@return 自分の参照
	 */
	CXmlText& operator=(CXmlNode n)
	{
		if ( n.GetNodeType() == MSXML::NODE_TEXT )
		{
			m_pNode = n.operator ->();
		}
		else
		{
			m_pNode = NULL;
		}
		return *this;
	}

	/** 
	 * [取得] TEXT 取得.
	 *		本インスタンスで保持しているテキスト内容を取得します。
	 *	@note GetNodeValue() と同じ動きです。
	 *	@return 値。
	 */
	CStr GetText(void) const
	{
		return GetNodeValue();
	}

	/**
	 * [変更] 名前変更.
	 *		本インスタンスで保持するテキストを変更します。
	 *	@param newName 新しい名前
	 */
	void Rename(LPCTSTR newName)
	{
		if ( GetText().IsEqual(newName) )
		{
			return;
		}
		CXmlNodePtr n = m_pNode->GetownerDocument()->createTextNode(newName);
		m_pNode->GetparentNode()->replaceChild(n, m_pNode);
		m_pNode = n;
	}

	/** 
	 * [作成] クローン作成.
	 *		本インスタンスで保持しているノードを複製します。
	 *	@note 通常、複製したノードは、他のElementノードに AppendChildNode() します。 
	 *	@return ノード(追加された実態のノードの参照)。
	 */
	CXmlText Clone(void) const
	{
		return m_pNode->cloneNode(VARIANT_FALSE);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML Comment 管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは Comment ノードを一つを管理します。
 *
 *	@note インスタンスは基本的に CXmlElement::AppendChildComment() の戻り値や CXmlNodeList から作成したり、 
 *			CXmlNode を代入宣言します。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlComment : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
public:

	/** 
	 * コンストラクタ
	 *	@note 無効なインスタンスに成ります。
	 */
	CXmlComment(void) : _super(NULL)
	{
	}

	/** 
	 * コンストラクタ
	 *	@note 指定のノードが Text ではない時、無効なインスタンスに成ります。
	 *	@param n ノード管理クラス
	 */
	CXmlComment(CXmlNode n) : _super(NULL)
	{
		if ( n.GetNodeType() == MSXML::NODE_COMMENT )
		{
			m_pNode = n.operator ->();
		}
	}

	/**
	 * [代入] 代入
	 *	@param n ノード管理
	 *	@return 自分の参照
	 */
	CXmlComment& operator=(CXmlNode n)
	{
		if ( n.GetNodeType() == MSXML::NODE_COMMENT )
		{
			m_pNode = n.operator ->();
		}
		else
		{
			m_pNode = NULL;
		}
		return *this;
	}

	/** 
	 * [取得] Comment 取得.
	 *		本インスタンスで保持しているコメント内容を取得します。
	 *	@note GetNodeValue() と同じ動きです。
	 *	@return 値。
	 */
	CStr GetComment(void) const
	{
		return GetNodeValue();
	}

	/**
	 * [変更] 名前変更.
	 *		本インスタンスで保持するコメントを変更します。
	 *	@param newName 新しい名前
	 */
	void Rename(LPCTSTR newName)
	{
		if ( GetComment().IsEqual(newName) )
		{
			return;
		}
		CXmlNodePtr n = m_pNode->GetownerDocument()->createComment(newName);
		m_pNode->GetparentNode()->replaceChild(n, m_pNode);
		m_pNode = n;
	}

	/** 
	 * [作成] クローン作成.
	 *		本インスタンスで保持しているノードを複製します。
	 *	@note 通常、複製したノードは、他のElementノードに AppendChildNode() します。 
	 *	@return ノード(追加された実態のノードの参照)。
	 */
	CXmlComment Clone(void) const
	{
		return m_pNode->cloneNode(VARIANT_FALSE);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML ProcessingInstruction 管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは ProcessingInstruction ノードを一つを管理します。
 *
 *	@note インスタンスは基本的に CXmlDocument::GetProcessingInstruction() から作成されます。
 *
 *	@note 子ノードは、通常ありません。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlProcessingInstruction : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
protected:

	MSXML::IXMLDOMNamedNodeMapPtr	m_pAttr;	///< ノード名前マップクラスのポインタハンドル

public:

	/** 
	 * コンストラクタ
	 *	@note 無効なインスタンスに成ります。
	 */
	CXmlProcessingInstruction(void) : _super(NULL), m_pAttr(NULL)
	{
	}

	/** 
	 * コンストラクタ
	 *	@note 指定のノードが PROCESSING INSTRUCTION ではない時、無効なインスタンスに成ります。
	 *	@param n ノード管理クラス
	 */
	CXmlProcessingInstruction(CXmlNode n) : _super(NULL), m_pAttr(NULL)
	{
		if ( n.GetNodeType() == MSXML::NODE_PROCESSING_INSTRUCTION )
		{
			m_pNode = n.operator ->();
			m_pAttr = m_pNode->Getattributes();
		}
	}

	/**
	 * [代入] 代入
	 *	@param n ノード管理
	 *	@return 自分の参照
	 */
	CXmlProcessingInstruction& operator=(CXmlNode n)
	{
		if ( n.GetNodeType() == MSXML::NODE_PROCESSING_INSTRUCTION )
		{
			m_pNode = n.operator ->();
			m_pAttr = m_pNode->Getattributes();
		}
		else
		{
			m_pNode = NULL;
			m_pAttr = NULL;
		}
		return *this;
	}

	/** 
	 * [取得] 属性数取得.
	 *		本インスタンスで保持しているノードの保持する属性数を取得することが出来ます。
	 *	@return 属性数。
	 */
	long GetAttributeLength(void) const
	{
		if ( m_pAttr != NULL )
		{
			return m_pAttr->Getlength();
		}
		return 0;
	}

	/**
	 * [取得] 属性取得.
	 *		本インスタンスで保持しているノードの保持する属性をインデックス指定で取得することが出来ます。
	 *	@param index インデックス。０～ GetAttributeLength()-1 まで。
	 *	@return 属性。
	 */
	CXmlAttribute GetAttribute(INDEX index) const
	{
		if ( m_pAttr != NULL )
		{
			return m_pAttr->Getitem(static_cast<long>(index));
		}
		return CXmlAttribute(NULL);
	}

	/**
	 * [取得] 属性取得.
	 *		本インスタンスで保持しているノードの保持する属性を名前指定で取得することが出来ます。
	 *	@param name 名前。
	 *	@return 属性。指定の名前の属性がない場合、無効な属性管理インスタンスが返ります。
	 */
	CXmlAttribute GetAttributeByName(LPCTSTR name) const
	{
		if ( m_pAttr != NULL )
		{
			return m_pAttr->getNamedItem(name);
		}
		return CXmlAttribute(NULL);
	}

	/**
	 * [取得] 属性値取得.
	 *		本インスタンスで保持しているノードの保持する指定の属性の値を取得することが出来ます。
	 *	@param name 名前。
	 *	@return 値。指定の属性がない場合、Empty文字列が返ります。
	 */
	CStr GetAttributeValue(LPCTSTR name) const
	{
		CXmlAttribute a = GetAttributeByName(name);
		return a.IsValid() ? a.GetValue() : CStr();
	}

	/**
	 * [設定] 属性設定.
	 *		本インスタンスで保持しているノードに属性を追加できます。
	 *	@note 同じ名前なら値が更新されます。
	 *	@param name 名前。
	 *	@param value 値。
	 *	@return 実際に登録した属性を保持する属性管理インスタンス。
	 */
	CXmlAttribute SetAttribute(LPCTSTR name, LPCTSTR value)
	{
		if ( IsValid() && m_pAttr != NULL )
		{
			MSXML::IXMLDOMDocumentPtr pDoc = m_pNode->GetownerDocument();
			MSXML::IXMLDOMAttributePtr p = pDoc->createAttribute(name);
			p->PutValue(value);
			return m_pAttr->setNamedItem(p);
		}
		return CXmlAttribute(NULL);
	}

	/**
	 * [設定] 属性削除.
	 *		本インスタンスで保持している、指定の名前のノードを削除します。
	 *	@param name 名前。
	 *	@retval true 削除成功。
	 *	@retval false 失敗 or 指定の名の属性なし。
	 */
	bool RemoveAttribute(LPCTSTR name)
	{
		if ( m_pAttr != NULL )
		{
			CXmlNodePtr p = m_pAttr->removeNamedItem(name);
			return p != NULL;
		}
		return false;
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML Element ノード管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは Element ノードを一つを管理します。
 *
 *	@note	インスタンスは基本的に CXmlElement::AppendChildElement() の戻り値や CXmlNodeList から作成したり、 
 *			CXmlNode を代入宣言します。
 *
 *	@note 子ノード操作は、 CXmlNodeList を取得して行います。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlElement : public CXmlProcessingInstruction
{
	DEFSUPER(CXmlProcessingInstruction);
public:

	/** 
	 * コンストラクタ
	 *	@note 無効なインスタンスに成ります。
	 */
	CXmlElement(void) : _super()
	{
	}

	/** 
	 * コンストラクタ
	 *	@note 指定のノードが Element ノードではない時、無効なインスタンスに成ります。
	 *	@param n ノード管理クラス
	 */
	CXmlElement(CXmlNode n) : _super()
	{
		if ( n.GetNodeType() == MSXML::NODE_ELEMENT )
		{
			m_pNode = n.operator ->();
			m_pAttr = m_pNode->Getattributes();
		}
	}

	/**
	 * [代入] 代入
	 *	@param n ノード管理
	 *	@return 自分の参照
	 */
	CXmlElement& operator=(CXmlNode n)
	{
		if ( n.GetNodeType() == MSXML::NODE_ELEMENT )
		{
			m_pNode = n.operator ->();
			m_pAttr = m_pNode->Getattributes();
		}
		else
		{
			m_pNode = NULL;
			m_pAttr = NULL;
		}
		return *this;
	}

	/**
	 * [変更] 名前変更.
	 *		本インスタンスで保持しているノードの名前を変更します。
	 *	@param newName 新しい名前
	 */
	void Rename(LPCTSTR newName)
	{
		if ( GetNodeName().IsEqual(newName) )
		{
			return;
		}
		CXmlNodePtr n = m_pNode->GetownerDocument()->createElement(newName);
		// 属性引越し
		MSXML::IXMLDOMNamedNodeMapPtr pOldAttrList = m_pNode->Getattributes();
		if ( NULL != pOldAttrList )				
		{
			MSXML::IXMLDOMNamedNodeMapPtr pNewAttrList = n->Getattributes();
			MSXML::IXMLDOMAttributePtr pAttr;
			while ( (pAttr = pOldAttrList->nextNode()) != NULL )
			{
				pNewAttrList->setNamedItem(pAttr->cloneNode(VARIANT_TRUE));
			}
		}
		// ノード引越し
		MSXML::IXMLDOMNodePtr pChild;
		while ( (pChild = m_pNode->GetfirstChild()) != NULL )
		{
			n->appendChild(pChild);
		}
		// 入れ替え
		MSXML::IXMLDOMNodePtr p = m_pNode->GetparentNode();
		p->replaceChild(n, m_pNode);
		m_pNode = n;
	}

	/** 
	 * [確認] 子ノード有無確認.
	 *		本インスタンスで保持しているノードの下にぶら下がるノードの有無を確認します。
	 *	@retval true 子ノードあり。
	 *	@retval false 子ノードなし。
	 */
	bool HasChildNodes(void) const
	{
		if ( ! IsValid() ) { return false; }
		return VARIANT_FALSE != m_pNode->hasChildNodes();
	}

	/** 
	 * [取得] 最初の子ノード取得.
	 *	@note なければ、無効な ノードが返ります。
	 *	@return ノード。
	 */
	CXmlNode GetFirstChild(void) const
	{
		return m_pNode->GetfirstChild();
	}

	/** 
	 * [取得] 子ノード群取得.
	 *		本インスタンスで保持しているノードの下にぶら下がるノード一覧を取得します。
	 *	@return ノード一覧。
	 */
	CXmlNodeList GetChildNodes(void) const
	{
		return CXmlNodeList(m_pNode->GetchildNodes(), m_pNode);
	}

	/** 
	 * [検索] 条件一致ノード検索.
	 *		本インスタンスで保持しているドキュメントの下にぶら下がるノードで検索式に合うノード一覧を作成します。
	 *	@param expression 検索式(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath式</a>)
	 *	@return ノード一覧。
	 */
	CXmlNodeList SelectNodes(LPCTSTR expression) const
	{
		return CXmlNodeList(m_pNode->selectNodes(expression), m_pNode);
	}

	/** 
	 * [検索] 条件一致ノード検索.
	 *		本インスタンスで保持しているドキュメントの下にぶら下がるノードで検索式に合うノードを取得します。
	 *	@param expression 検索式(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath式</a>)
	 *	@return ノード。
	 */
	CXmlNode SelectSingleNode(LPCTSTR expression) const
	{
		return CXmlNode(m_pNode->selectSingleNode(expression));
	}
	
	/** 
	 * [取得] 親 Element ノード取得.
	 *		本インスタンスで保持しているノードの親 Element ノードを取得します。
	 *	@return Element ノード。親がない場合、無効状態のインスタンスが返ります。
	 */
	CXmlElement GetParentNode(void) const
	{
		return m_pNode->GetparentNode();
	}

	/** 
	 * [作成] クローン作成.
	 *		本インスタンスで保持しているノードを複製します。
	 *	@note 通常、複製したノードは、他のElementノードに AppendChildNode() します。 
	 *	@param boIsDeep 省略するか true なら、本インスタンスが保持しているノード以下、すべてを複製します。
	 *					false なら、保持しているノードのみ複製します。
	 *	@return ノード(追加された実態のノードの参照)。
	 */
	CXmlNode Clone(bool boIsDeep = true) const
	{
		return m_pNode->cloneNode(boIsDeep ? VARIANT_TRUE : VARIANT_FALSE);
	}

	/**
	 * [追加] コメント子ノード追加.
	 *		本インスタンスで管理している子ノード群の最尾後に指定のノードを追加します。
	 *	@param text 追加するコメント文字列。
	 *	@return ノード(追加された実態のノードの参照)。
	 */
	CXmlComment AppendChildComment(LPCTSTR text)
	{
		return GetChildNodes().AppendNode(CXmlNodeList::COMMENT, text);
	}

	/**
	 * [追加] テキスト子ノード追加.
	 *		本インスタンスで管理している子ノード群の最尾後に指定のノードを追加します。
	 *	@param text 追加するテキスト文字列。
	 *	@return ノード(追加された実態のノードの参照)。 
	 */
	CXmlText AppendChildText(LPCTSTR text)
	{
		return GetChildNodes().AppendNode(CXmlNodeList::TEXT, text);
	}

	/**
	 * [追加] Element 子ノード追加.
	 *		本インスタンスで管理している子ノード群の最尾後に指定のノードを追加します。
	 *	@param text 追加する Element 文字列。
	 *	@return ノード(追加された実態のノードの参照)。
	 */
	CXmlElement AppendChildElement(LPCTSTR text)
	{
		return GetChildNodes().AppendNode(CXmlNodeList::ELEMENT, text);
	}

	/**
	 * [追加] 子ノード追加.
	 *		本インスタンスで管理している子ノード群の最尾後に指定のノードを追加します。
	 *	@attention 追加するノードが、すでにドキュメント下にある場合、複製されずに移動になります。
	 *	@note a に CXmlDocumentFragment インスタンスを指定した場合、ぶら下がっているノードがすべて本インスタンス管理ノード下に移動し、
	 *			CXmlDocumentFragment インスタンスは空っぽになります。
	 *	@param a 追加する ノード。 
	 *	@return ノード(追加された実態のノードの参照)。
	 */
	CXmlNode AppendChildNode(CAbstractXmlNode& a)
	{
		return m_pNode->appendChild(a.operator ->());
	}

private:
	friend class CXmlTest;
};
	
	
	
/**@ingroup XML
 * XML DocumentFragment ノード管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは DocumentFragment ノードを一つを管理します。
 *
 *	@note インスタンスは CXmlDocument::CreateDocumentFragment() から作成します。
 *
 *	@note 子ノード操作は、 CXmlNodeList を取得して行います。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 */
class CXmlDocumentFragment : public CXmlElement
{
	DEFSUPER(CXmlElement);
public:

	/** 
	 * コンストラクタ
	 *	@note 指定のノードが Element ノードではない時、無効なインスタンスに成ります。
	 *	@param n ノード管理クラス
	 */
	CXmlDocumentFragment(MSXML::IXMLDOMDocumentFragmentPtr n) : _super()
	{
		if ( n->GetnodeType() == MSXML::NODE_DOCUMENT_FRAGMENT )
		{
			m_pNode = n;
			m_pAttr = m_pNode->Getattributes();
		}
	}
};



/**@ingroup XML
 * XML ドキュメント管理クラス
 *
 *		XML の管理クラスの一つ。XMLパーサのクラスをラップしていています。
 *
 *		本クラスのインスタンスでは ドキュメント一つを管理します。
 *
 *	@note 子ノードは直接追加できません。 GetRootElement() を用いて、 CXmlElement インスタンスを取得し、
 *		これを操作します。
 *
 *	@note 本インスタンスから作成されたノードなどのインスタンスが生きている間は、本インスタンスを
 *			破棄しない様にしてください。
 *
 *	@attention 異なる Document間でのノードの移動は出来ません。
 *	
 *	@attention	本クラスのインスタンスから抽出されたインスタンスは、本インスタンスにリンクしており、
 *				操作すると即影響を受けます。
 *
 *	@throw _com_error 失敗するとスローされます。
 *
 *	@par必要ファイル
 *			TnbXml.h
 *
 *	@date 07/05/21 新規
 *	@date 08/04/16 デストラクタの処理を改善
 */
class CXmlDocument
{
	MSXML::IXMLDOMDocumentPtr	m_pDoc;		///< DOCクラスポインタ
	HRESULT						m_initRes;	///< CoInitializeの戻り値
	/// 読み込んであった情報を破棄します。
	void m_Release(void)
	{
		m_pDoc = NULL;
	}
	/// 初期化
	void m_Init(void)
	{
		m_Release();
#ifndef _TnbXML_MsXml6_ENABLE
		MSXML::IXMLDOMDocumentPtr p(__uuidof(MSXML::DOMDocument));
#else
		MSXML::IXMLDOMDocumentPtr p(__uuidof(MSXML::DOMDocument60));
#endif
		m_pDoc = p;
		p->put_async(VARIANT_FALSE);
	}
	/**
	 * 読込.
	 *	@note 先に読み込んであった情報は破棄されます。
	 *	@param text 文字列
	 *	@param isFile true;lpszはファイル名指定、false;lpszはXML形式で書かれた文字列指定
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_Load(LPCTSTR text, bool isFile)
	{
		m_Init();
		_bstr_t bstr = text;
		VARIANT_BOOL varResult;
		if ( isFile )
		{
			varResult = m_pDoc->load(_variant_t(bstr));
		}
		else
		{
			varResult = m_pDoc->loadXML(bstr);
		}
		bool r = ( VARIANT_FALSE != varResult );
		if ( ! r )
		{
			Invalid();
		}
		return r;
	}

public:

	/// コンストラクタ
	CXmlDocument(void) : m_pDoc(NULL)
	{
		m_initRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	}

	/// デストラクタ
	virtual ~CXmlDocument(void)
	{
		m_Release();
		if ( m_initRes == S_OK )
		{
			::CoUninitialize();
		}
	}

	/**
	 * [確認] 有効確認.
	 *		本インスタンスが有効な情報を保持しているか、確認できます。
	 *	@retval true 有効。
	 *	@retval false 無効。
	 */
	bool IsValid(void) const
	{
		return m_pDoc != NULL;
	}

	/**
	 * [設定] 無効化.
	 *		本インスタンスの保持している情報を無効にします。
	 */
	void Invalid(void)
	{
		m_pDoc = NULL;
	}

	/**
	 * [取得] エラーコード
	 *	@return エラーコード
	 */
	DWORD GetLastError(void) const
	{
		return m_pDoc->GetparseError()->GeterrorCode();
	}

	/**
	 * [取得] エラーコード文字列
	 *	@return エラーコード文字列
	 */
	CStr GetLastErrorString(void) const
	{
		CStr r;
		MSXML::IXMLDOMParseErrorPtr pError = m_pDoc->GetparseError();
		if ( pError->GeterrorCode() != 0 )
		{
			_bstr_t bstr = pError->Getreason();
			r = LPCTSTR(bstr);
		}
		return r;
	}

	/**
	 * [操作] 初期化
	 *	@note まっさらな XML を作成し、 Element を一つ作成します。
	 *	@param lpszRootElementName ルート Element ノード名。
	 *	@param lpszComment コメント。省略すると ルートにコメントは作成されません。
	 *	@param lpszTarget ターゲット名。省略するとProcessingInstructionは作成されません。
	 *	@param lpszData データ名。省略するとProcessingInstructionは作成されません。
	 */
	void Initialize(LPCTSTR lpszRootElementName, LPCTSTR lpszComment = NULL, LPCTSTR lpszTarget = NULL, LPCTSTR lpszData = NULL)
	{
		m_Init();
		if ( lpszTarget != NULL && lpszData != NULL )
		{
			CXmlNodePtr pNode = m_pDoc->createProcessingInstruction(lpszTarget, lpszData);
			m_pDoc->appendChild(pNode);
		}
		if ( lpszComment != NULL )
		{
			CXmlNodePtr pNode = m_pDoc->createComment(lpszComment);
			m_pDoc->appendChild(pNode);
		}
		CXmlNodePtr pNode = m_pDoc->createElement(lpszRootElementName);
		m_pDoc->appendChild(pNode);
	}

	/**
	 * [操作] 初期化
	 *	@note まっさらな XML を作成し、 Element を一つ作成します。
	 *	@param lpszRootElementName ルート Element ノード名。
	 *	@param lpszComment コメント。省略すると ルートにコメントは作成されません。
	 */
	void InitializeXml(LPCTSTR lpszRootElementName, LPCTSTR lpszComment = NULL)
	{
		m_Init();
		CXmlNodePtr pNode = m_pDoc->createProcessingInstruction(_T("xml"), _T("version='1.0'"));
		m_pDoc->appendChild(pNode);
		if ( lpszComment != NULL )
		{
			CXmlNodePtr pNode = m_pDoc->createComment(lpszComment);
			m_pDoc->appendChild(pNode);
		}
		pNode = m_pDoc->createElement(lpszRootElementName);
		m_pDoc->appendChild(pNode);
	}

	/**
	 * [設定] XMLファイル読込
	 *	@param lpszFileName ファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool LoadFile(LPCTSTR lpszFileName)
	{
		return m_Load(lpszFileName, true);
	}

	/**
	 * [設定] XMLテキスト読込
	 *	@param lpszXml テキスト
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool LoadXml(LPCTSTR lpszXml)
	{
		return m_Load(lpszXml, false);
	}

	/**
	 * [作成] XMLファイル作成
	 *	@param lpszFileName ファイル名
	 *	@param boIsUtf8 false なら Shift-JISで作成されます。 
	 *					省略か ture なら UTF-8 で作成されます。
	 *					ただし、ProcessingInstructionがない場合、 必ずUTF-8で作成されます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SaveFile(LPCTSTR lpszFileName, bool boIsUtf8 = true) const
	{
		_bstr_t bstr = lpszFileName;
		CXmlProcessingInstruction pi = GetProcessingInstruction();
		if ( pi.IsValid() )
		{
			pi.SetAttribute(_T("encoding"), (boIsUtf8 ? _T("UTF-8") : _T("SJIS")));
		}
		return SUCCEEDED(m_pDoc->save(bstr));
	}


#ifdef _TnbXML_MsXml6_ENABLE

	/**
	 * [作成] XMLファイル作成
	 *	@note 改行やインデント付きで出力されます。
	 *	@note ProcessingInstruction ノードは必ず出力されます。
	 *	@todo コメントノードが出力されません。
	 *
	 *	@param lpszFileName ファイル名。既存のファイル名を指定すると削除して作成します。追記はされません。
	 *	@param boIsUtf8 false なら Shift-JISで作成されます。 
	 *					省略か ture なら UTF-8 で作成されます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SaveFileEx(LPCTSTR lpszFileName, bool boIsUtf8 = true) const
	{
		IStreamPtr stream = NULL;
		try
		{
			typedef HRESULT (WINAPI *P_SHCreateStreamOnFileW)(LPCWSTR, DWORD, IStream**);
			CDynamicFuncT<P_SHCreateStreamOnFileW>("shlwapi.dll", "SHCreateStreamOnFileW")()(
								CUnicode(lpszFileName),
								STGM_READWRITE | STGM_SHARE_DENY_WRITE | STGM_CREATE,
								&stream
			);
		}
		catch(CNullPointerException& e)
		{
			e.OnCatch();
			return false;
		}
		if ( stream == NULL )
		{
			return false;
		}
//		MSXML2::IMXWriterPtr wrt(__uuidof(MSXML2::MXXMLWriter));
		MSXML2::IMXWriterPtr wrt(__uuidof(MSXML2::MXXMLWriter60));
		wrt->put_version(L"1.0");
		wrt->put_encoding(boIsUtf8 ? L"UTF-8" : L"SJIS");
		wrt->put_indent(VARIANT_TRUE);
		wrt->Putstandalone(VARIANT_TRUE);
		wrt->put_output(_variant_t(IUnknownPtr(stream).GetInterfacePtr()));
		MSXML2::ISAXXMLReaderPtr rdr(__uuidof(MSXML2::SAXXMLReader60));
//		MSXML2::ISAXXMLReaderPtr rdr(__uuidof(MSXML2::SAXXMLReader));
		rdr->putContentHandler(MSXML2::ISAXContentHandlerPtr(wrt));
		_variant_t vinput = IUnknownPtr(m_pDoc).GetInterfacePtr();
		return SUCCEEDED(rdr->parse(vinput));
	}

#endif


	/**
	 * [作成] XMLテキスト作成
	 *	@attention	encoding の属性は書き込まれません。もし、この文字列をファイルに書き出したい場合、
	 *				UTF-8 に変換してから書き出す必要があります（encoding のデフォルトは UTF-8だから）。
	 *	@return 文字列は XMLテキスト。失敗の場合Empty状態文字列が返ります。
	 */
	CStr ToString(void) const
	{
		_bstr_t str = m_pDoc->Getxml();
		return CStr(LPCTSTR(str));
	}

	/** 
	 * [取得] 子ノード群取得.
	 *		本インスタンスで保持しているドキュメントの直下にぶら下がるノード一覧を取得します。
	 *	@return ノード一覧。
	 */
	CXmlNodeList GetChildNodes(void) const
	{
		return CXmlNodeList(m_pDoc->GetchildNodes(), m_pDoc, m_pDoc);
	}

	/** 
	 * [取得] ProcessingInstruction ノード取得.
	 *		本インスタンスで保持しているドキュメントの直下にぶら下がる ProcessingInstruction ノード取得します。
	 *	@return ProcessingInstruction 管理インスタンス
	 */
	CXmlProcessingInstruction GetProcessingInstruction(void) const
	{
		const CXmlNodeList& list = GetChildNodes();
		loop ( i, list.GetLength() )
		{
			if ( list.GetNodeType(i) == MSXML::NODE_PROCESSING_INSTRUCTION )
			{
				return list.GetNode(i);
			}
		}
		return CXmlProcessingInstruction();
	}

	/** 
	 * [取得] Element ノード取得.
	 *		本インスタンスで保持しているドキュメントの直下にぶら下がる 唯一の Element ノード取得します。
	 *	@return Element ノード管理
	 */
	CXmlElement GetRootElement(void) const
	{
		const CXmlNodeList& list = GetChildNodes();
		loop ( i, list.GetLength() )
		{
			if ( list.GetNodeType(i) == MSXML::NODE_ELEMENT )
			{
				return list.GetNode(i);
			}
		}
		return CXmlElement();
	}
	
	/**
	 * [作成] DocumentFragment作成.
	 *	@return DocumentFragment
	 */
	CXmlDocumentFragment CreateDocumentFragment(void) const
	{
		return CXmlDocumentFragment(m_pDoc->createDocumentFragment());
	}

	/** 
	 * [検索] 条件一致ノード検索.
	 *		本インスタンスで保持しているドキュメントの下にぶら下がる Element ノードで条件に合うノード一覧を作成します。
	 *	@param name ノード名。
	 *	@return ノード一覧。
	 */
	CXmlNodeList GetElementsByTagName(LPCTSTR name) const
	{
		return CXmlNodeList(m_pDoc->getElementsByTagName(name), m_pDoc, m_pDoc);
	}

	/** 
	 * [検索] 条件一致ノード検索.
	 *		本インスタンスで保持しているドキュメントの下にぶら下がるノードで検索式に合うノード一覧を作成します。
	 *	@param expression 検索式(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath式</a>)
	 *	@return ノード一覧。
	 */
	CXmlNodeList SelectNodes(LPCTSTR expression) const
	{
		return CXmlNodeList(m_pDoc->selectNodes(expression), m_pDoc, m_pDoc);
	}

	/** 
	 * [検索] 条件一致ノード検索.
	 *		本インスタンスで保持しているドキュメントの下にぶら下がるノードで検索式に合うノードを取得します。
	 *	@param expression 検索式(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath式</a>)
	 *	@return ノード。
	 */
	CXmlNode SelectSingleNode(LPCTSTR expression) const
	{
		return CXmlNode(m_pDoc->selectSingleNode(expression));
	}

private:
	friend class CXmlTest;
};



}; // TNB


