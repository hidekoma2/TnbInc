#pragma once
/**
 *@file
 * XML�֌W�̃w�b�_
 *
 *		XML �p�[�T�����b�v�����N���X������܂��B
 *
 *	@note	�}�N�� "_TnbXML_MsXml6_ENABLE" ����`����Ă���ƁA MSXML6.dll ���g�p���܂��B
 *			MSXML6.dll ���g�p���Ȃ��ƁA CXmlDocument::SaveFileEx() ���\�b�h�͎g�p�ł��܂���B
 *	
 *	@note	VC++6 �̏ꍇ�A�Â� MSXML6.dll(2019/02/09�ȑO) ���C���|�[�g���Ȃ��ƃ����N�G���[�ɂȂ�܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
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
 * XML �m�[�h�|�C���^�n���h���N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
typedef MSXML::IXMLDOMNodePtr CXmlNodePtr;



/**@ingroup XML
 * XML �m�[�h���X�g�Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł͕����̃m�[�h���܂Ƃ߂ĊǗ����܂��B
 *
 *	@note Element, TEXT, COMMENT �Ȃǂ̃m�[�h��ǉ��ł��܂��B
 *
 *	@note	�C���X�^���X�� CXmlElement::GetChildNodes() ���쐬����܂��B
 *			���ڂ���邱�Ƃ͂���܂���B
 *	
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlNodeList
{
public:

	/// �m�[�h��ތ^
	enum ENodeType
	{
		ELEMENT,	///< Element
		TEXT,		///< ������
		COMMENT,	///< �R�����g
	};

private:

	MSXML::IXMLDOMNodeListPtr	m_pNodes;	///< �m�[�h���X�g�N���X�̃|�C���^�n���h��
	CXmlNodePtr					m_pParent;	///< �e�̃|�C���^�n���h��
	MSXML::IXMLDOMDocumentPtr	m_pDoc;		///< DOC�̃|�C���^�n���h��

	/// �V�����m�[�h�쐬
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
	 * �R���X�g���N�^
	 *	@param na IXMLDOMNodeList �̃|�C���^�n���h���B
	 *	@param pp na �� Parent �m�[�h�̃|�C���^�n���h���B
	 *	@param dc na �� Document �̃|�C���^�n���h���B�ȗ�����ƁA pp ����擾���܂��B
	 */
	CXmlNodeList(MSXML::IXMLDOMNodeListPtr na, CXmlNodePtr pp, MSXML::IXMLDOMDocumentPtr dc = NULL)
		: m_pNodes(na), m_pParent(pp), m_pDoc(dc)
	{
	}

	/**
	 * [�m�F] �L���m�F.
	 *		�{�C���X�^���X���L���ȏ���ێ����Ă��邩�A�m�F�ł��܂��B
	 *	@retval true �L���B
	 *	@retval false �����B
	 */
	bool IsValid(void) const
	{
		return m_pNodes != NULL;
	}

	/** 
	 * [�擾] �|�C���^�n���h���擾.
	 *		�{�C���X�^���X�ŊǗ����Ă��� IXMLDOMNodeList �|�C���^�n���h���𓾂邱�Ƃ��o���܂��B
	 *	@note ���� XML�p�[�T�̃��\�b�h���g�p�ł��܂��B
	 *	@return IXMLDOMNodeList �|�C���^�n���h��
	 */
	MSXML::IXMLDOMNodeListPtr operator->(void)
	{
		return m_pNodes;
	}

	/** 
	 * [�擾] �Ǘ��m�[�h���擾.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h�����擾���邱�Ƃ��o���܂��B
	 *	@return �m�[�h���B
	 */
	size_t GetLength(void) const
	{
		return m_pNodes->Getlength();
	}

	/**
	 * [�擾] �m�[�h�擾.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h���擾���邱�Ƃ��o���܂��B
	 *	@param index �C���f�b�N�X�B�O�` GetLength() -1 �܂ŁB
	 *	@return �m�[�h�B CXmlNode , CXmlElement �Ŏ󂯂邱�Ƃ��o���܂��B
	 */
	CXmlNodePtr GetNode(INDEX index) const
	{
		return m_pNodes->Getitem(static_cast<long>(index));
	}

	/** 
	 * [�擾] �m�[�h�^�C�v�擾.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h�̃^�C�v���擾���܂��B
	 *	@param index �C���f�b�N�X�B�O�` GetLength()-1 �܂ŁB
	 *	@retval MSXML::NODE_INVALID					����
	 *	@retval MSXML::NODE_ELEMENT					�v�f
	 *	@retval MSXML::NODE_ATTRIBUTE				����
	 *	@retval MSXML::NODE_TEXT					������
	 *	@retval MSXML::NODE_CDATA_SECTION			CDATA
	 *	@retval MSXML::NODE_ENTITY_REFERENCE		�H
	 *	@retval MSXML::NODE_ENTITY					�H
	 *	@retval MSXML::NODE_PROCESSING_INSTRUCTION	�H
	 *	@retval MSXML::NODE_COMMENT					�R�����g
	 *	@retval MSXML::NODE_DOCUMENT				�h�L�������g
	 *	@retval MSXML::NODE_DOCUMENT_TYPE			�H
	 *	@retval MSXML::NODE_DOCUMENT_FRAGMENT		�H
	 *	@retval MSXML::NODE_NOTATION				����
	 */
	MSXML::DOMNodeType GetNodeType(INDEX index) const
	{
		return GetNode(index)->GetnodeType();
	}

	/**
	 * [�ǉ�] �m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h�Q�̍Ŕ���Ɏw��̃m�[�h��ǉ����܂��B
	 *	@param newItem �ǉ�����m�[�h�B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B CXmlNode , CXmlElement �ȂǂŎ󂯂邱�Ƃ��o���܂��B
	 */
	CXmlNodePtr AppendNode(CXmlNodePtr newItem)
	{
		return m_pParent->appendChild(newItem);
	}

	/**
	 * [�ǉ�] �m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h�Q�̍Ŕ���Ɏw��̃m�[�h��ǉ����܂��B
	 *	@param type �ǉ�����^�C�v�B
	 *	@param text �ǉ����镶����B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B CXmlNode , CXmlElement �Ŏ󂯂邱�Ƃ��o���܂��B
	 */
	CXmlNodePtr AppendNode(ENodeType type, LPCTSTR text)
	{
		return AppendNode(m_MakeNewNode(type, text));
	}

	/**
	 * [�ǉ�] �m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h�Q�̎w��̉ӏ��Ɏw��̃m�[�h��ǉ����܂��B
	 *	@param index �ǉ�����C���f�b�N�X�B�ǉ������m�[�h�̃C���f�b�N�X�ɂȂ�܂��B
	 *	@param newItem �ǉ�����m�[�h�B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B CXmlNode , CXmlElement �Ŏ󂯂邱�Ƃ��o���܂��B
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
	 * [�ǉ�] �m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h�Q�̎w��̉ӏ��Ɏw��̃m�[�h��ǉ����܂��B
	 *	@param index �ǉ�����C���f�b�N�X�B�ǉ������m�[�h�̃C���f�b�N�X�ɂȂ�܂��B
	 *	@param type �ǉ�����^�C�v�B
	 *	@param text �ǉ����镶����B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B CXmlNode , CXmlElement �Ŏ󂯂邱�Ƃ��o���܂��B
	 */
	CXmlNodePtr InsertNode(INDEX index, ENodeType type, LPCTSTR text)
	{
		return InsertNode(index, m_MakeNewNode(type, text));
	}

	/**
	 * [����] �m�[�h����ւ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���m�[�h�Q�̂Q�̎w��̃m�[�h�����ւ��܂��B
	 *	@param index1 ����ւ��Ώۂ̃m�[�h�̃C���f�b�N�X�ɂȂ�܂��B
	 *	@param index2 ����ւ��Ώۂ̃m�[�h�̃C���f�b�N�X�ɂȂ�܂��B
	 */
	void ReplaceNode(INDEX index1, INDEX index2)
	{
		m_pParent->replaceChild(GetNode(index1), GetNode(index2));
	}

	/**
	 * [�폜] �m�[�h�폜.
	 *	@param index �폜����C���f�b�N�X�B
	 */
	void RemoveNode(INDEX index)
	{
		m_pParent->removeChild(GetNode(index));
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML	�����Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł͑�������Ǘ����܂��B
 *
 *	@note	CXmlElement::GetAttribute() , CXmlElement::GetAttributeByName() , CXmlElement::SetAttribute() �ō쐬���܂��B
 *			�ʏ풼�ڃC���X�^���X����邱�Ƃ͂���܂���B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlAttribute
{
	MSXML::IXMLDOMAttributePtr m_pAttr;	///< IXMLDOMAttribute �|�C���^�n���h��

public:

	/**
	 * �R���g���N�^
	 *	@param p IXMLDOMAttribute �|�C���^�n���h��
	 */
	CXmlAttribute(MSXML::IXMLDOMAttributePtr p) : m_pAttr(p)
	{
	}

	/**
	 * [�m�F] �L���m�F.
	 *		�{�C���X�^���X���L���ȏ���ێ����Ă��邩�A�m�F�ł��܂��B
	 *	@retval true �L���B
	 *	@retval false �����B
	 */
	bool IsValid(void) const
	{
		return m_pAttr != NULL;
	}

	/** 
	 * [�擾] �|�C���^�n���h���擾.
	 *		�{�C���X�^���X�ŊǗ����Ă��� IXMLDOMAttribute �|�C���^�n���h���𓾂邱�Ƃ��o���܂��B
	 *	@note ���� XML�p�[�T�̃��\�b�h���g�p�ł��܂��B
	 *	@return IXMLDOMAttribute �|�C���^�n���h��
	 */
	MSXML::IXMLDOMAttributePtr operator->(void)
	{
		return m_pAttr;
	}

	/**
	 * [�擾] ���O�擾.
	 *	@return ���O
	 */
	CStr GetName(void) const
	{
		_bstr_t r = m_pAttr->Getname();
		return CStr(LPCTSTR(r));
	}
 
	/**
	 * [�擾] �l�擾.
	 *	@return �l
	*/
	CStr GetValue(void) const
	{
		_bstr_t r = m_pAttr->GetValue();
		return CStr(LPCTSTR(r));
	}

	/**
	 * [�ݒ�] �l�ݒ�.
	 *	@note �{�C���X�^���X�̒l��ݒ肵������Ԃ̑������ω����܂��B
	 *	@param text �l
	 */
	void PutValue(LPCTSTR text)
	{
		m_pAttr->PutValue(text);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML �m�[�h�Ǘ���b�N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *	@note ��b�N���X�ł��B���ڎg�p���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CAbstractXmlNode
{
	HRESULT		m_initRes;	///< CoInitialize�̖߂�l
protected:

	CXmlNodePtr m_pNode;	///< �m�[�h�N���X�̃|�C���^�n���h��

	/** 
	 * �R���X�g���N�^
	 *	@note ���ڍ쐬�ł��Ȃ��p�� protected �錾�ɂ��Ă��܂��B
	 *	@param p �m�[�h�|�C���^�n���h��
	 */
	CAbstractXmlNode(CXmlNodePtr p) : m_pNode(p)
	{
		m_initRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	}

	/// �f�X�g���N�^
	virtual ~CAbstractXmlNode(void)
	{
		if ( m_initRes == S_OK )
		{
			::CoUninitialize();
		}
	}

public:

	/** 
	 * [�擾] �|�C���^�n���h���擾.
	 *		�{�C���X�^���X�ŕێ����Ă��� {@link TNB::CXmlNodePtr CXmlNodePtr} �𓾂邱�Ƃ��o���܂��B
	 *	@note ���� XML�p�[�T�̃��\�b�h���g�p�ł��܂��B
	 *	@return �m�[�h�|�C���^�n���h��
	 */
	CXmlNodePtr operator->(void)
	{
		return m_pNode;
	}

	/**
	 * [�m�F] �L���m�F.
	 *		�{�C���X�^���X���L���ȏ���ێ����Ă��邩�A�m�F�ł��܂��B
	 *	@retval true �L���B
	 *	@retval false �����B
	 */
	bool IsValid(void) const
	{
		return m_pNode != NULL;
	}

	/**
	 * [�ݒ�] ������.
	 */
	void Invalid(void)
	{
		m_pNode = NULL;
	}

	/** 
	 * [�擾] ���O�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̖��O���擾���܂��B
	 *	@note �m�[�h�̃^�C�v�ɂ���Ė��O�̈Ӗ��͈قȂ�܂��B
	 *	@return ���O�B
	 */
	CStr GetNodeName(void) const
	{
		if ( ! IsValid() ) { return CStr(); }
		_bstr_t r = m_pNode->GetnodeName();
		return LPCTSTR(r);
	}

	/** 
	 * [�擾] �l�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̒l���擾���܂��B
	 *	@note �m�[�h�̃^�C�v�ɂ���Ēl�̈Ӗ��͈قȂ�܂��B
	 *	@return �l�B
	 */
	CStr GetNodeValue(void) const
	{
		if ( ! IsValid() ) { return CStr(); }
		_bstr_t r = m_pNode->GetnodeValue();
		return LPCTSTR(r);
	}

	/**
	 * [�폜] �������g���폜.
	 *		�ȉ��ɂԂ牺����m�[�h������΁A����������܂��B
	 *	@note �{�C���X�^���X�͖�����ԂɂȂ�܂��B�ȍ~�̑��삪�o���܂���B
	 */
	void Remove(void)
	{
		m_pNode->GetparentNode()->removeChild(m_pNode);
		m_pNode = NULL;
	}

	/** 
	 * [�擾] �e�̃m�[�h�擾.
	 *	@note �e�̃m�[�h���Ȃ��ꍇ�A������ CXmlNode ���Ԃ�܂��B
	 *	@return �m�[�h�B
	 */
	CXmlNodePtr GetParentNode(void) const
	{
		return IsValid() ? m_pNode->GetparentNode() : NULL;
	}	

	/** 
	 * [�擾] �ׂ̃m�[�h�擾.
	 *	@note �ׂ̃m�[�h���Ȃ��ꍇ�A������ CXmlNode ���Ԃ�܂��B
	 *	@return �m�[�h�B
	 */
	CXmlNodePtr GetNextSibling(void) const
	{
		return IsValid() ? m_pNode->GetnextSibling() : NULL;
	}	

	/**
	 * [�쐬] ���[�g����� Element��.
	 *		�������g������K�w���AElement ���̘A���ŕ\���܂��B
	 *	@par ex)
	 *		/root/sub1/sub2 
	 *	@return Element�K�w��
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
 * XML �m�[�h�Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł̓m�[�h(Element, Text, Attribute,�Ȃ�)����Ǘ����܂��B
 *
 *	@note �C���X�^���X�͊�{�I�� CXmlNodeList �Ȃǂ���쐬���A���ڃC���X�^���X����邱�Ƃ͂���܂���B
 *
 *	@note �q�m�[�h����A��������͂ł��܂���B CXmlElement �ɑ�����čs���Ă��������B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlNode : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
public:

	/** 
	 * �R���X�g���N�^
	 *	@param p �m�[�h�|�C���^�n���h��
	 */
	CXmlNode(CXmlNodePtr p) : _super(p)
	{
	}

	/** 
	 * [�擾] �m�[�h�^�C�v�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̃^�C�v���擾���܂��B
	 *	@retval MSXML::NODE_INVALID					����
	 *	@retval MSXML::NODE_ELEMENT					�v�f
	 *	@retval MSXML::NODE_ATTRIBUTE				����
	 *	@retval MSXML::NODE_TEXT					������
	 *	@retval MSXML::NODE_CDATA_SECTION			CDATA
	 *	@retval MSXML::NODE_ENTITY_REFERENCE		�H
	 *	@retval MSXML::NODE_ENTITY					�H
	 *	@retval MSXML::NODE_PROCESSING_INSTRUCTION	(�ŏ��� <?xml?> �m�[�h )
	 *	@retval MSXML::NODE_COMMENT					�R�����g
	 *	@retval MSXML::NODE_DOCUMENT				�h�L�������g
	 *	@retval MSXML::NODE_DOCUMENT_TYPE			�H
	 *	@retval MSXML::NODE_DOCUMENT_FRAGMENT		�H
	 *	@retval MSXML::NODE_NOTATION				����
	 */
	MSXML::DOMNodeType GetNodeType(void) const
	{
		return IsValid() ? m_pNode->GetnodeType() : MSXML::DOMNodeType(MSXML::NODE_INVALID);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML Text �Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł� Text �m�[�h������Ǘ����܂��B
 *
 *	@note �C���X�^���X�͊�{�I�� CXmlElement::AppendChildText() �̖߂�l�� CXmlNodeList ����쐬������A 
 *			CXmlNode �����錾���܂��B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlText : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
public:

	/** 
	 * �R���X�g���N�^
	 *	@note �����ȃC���X�^���X�ɐ���܂��B
	 */
	CXmlText(void) : _super(NULL)
	{
	}

	/** 
	 * �R���X�g���N�^
	 *	@note �w��̃m�[�h�� Text �ł͂Ȃ����A�����ȃC���X�^���X�ɐ���܂��B
	 *	@param n �m�[�h�Ǘ��N���X
	 */
	CXmlText(CXmlNode n) : _super(NULL)
	{
		if ( n.GetNodeType() == MSXML::NODE_TEXT )
		{
			m_pNode = n.operator ->();
		}
	}

	/**
	 * [���] ���
	 *	@param n �m�[�h�Ǘ�
	 *	@return �����̎Q��
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
	 * [�擾] TEXT �擾.
	 *		�{�C���X�^���X�ŕێ����Ă���e�L�X�g���e���擾���܂��B
	 *	@note GetNodeValue() �Ɠ��������ł��B
	 *	@return �l�B
	 */
	CStr GetText(void) const
	{
		return GetNodeValue();
	}

	/**
	 * [�ύX] ���O�ύX.
	 *		�{�C���X�^���X�ŕێ�����e�L�X�g��ύX���܂��B
	 *	@param newName �V�������O
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
	 * [�쐬] �N���[���쐬.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�𕡐����܂��B
	 *	@note �ʏ�A���������m�[�h�́A����Element�m�[�h�� AppendChildNode() ���܂��B 
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B
	 */
	CXmlText Clone(void) const
	{
		return m_pNode->cloneNode(VARIANT_FALSE);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML Comment �Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł� Comment �m�[�h������Ǘ����܂��B
 *
 *	@note �C���X�^���X�͊�{�I�� CXmlElement::AppendChildComment() �̖߂�l�� CXmlNodeList ����쐬������A 
 *			CXmlNode �����錾���܂��B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlComment : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
public:

	/** 
	 * �R���X�g���N�^
	 *	@note �����ȃC���X�^���X�ɐ���܂��B
	 */
	CXmlComment(void) : _super(NULL)
	{
	}

	/** 
	 * �R���X�g���N�^
	 *	@note �w��̃m�[�h�� Text �ł͂Ȃ����A�����ȃC���X�^���X�ɐ���܂��B
	 *	@param n �m�[�h�Ǘ��N���X
	 */
	CXmlComment(CXmlNode n) : _super(NULL)
	{
		if ( n.GetNodeType() == MSXML::NODE_COMMENT )
		{
			m_pNode = n.operator ->();
		}
	}

	/**
	 * [���] ���
	 *	@param n �m�[�h�Ǘ�
	 *	@return �����̎Q��
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
	 * [�擾] Comment �擾.
	 *		�{�C���X�^���X�ŕێ����Ă���R�����g���e���擾���܂��B
	 *	@note GetNodeValue() �Ɠ��������ł��B
	 *	@return �l�B
	 */
	CStr GetComment(void) const
	{
		return GetNodeValue();
	}

	/**
	 * [�ύX] ���O�ύX.
	 *		�{�C���X�^���X�ŕێ�����R�����g��ύX���܂��B
	 *	@param newName �V�������O
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
	 * [�쐬] �N���[���쐬.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�𕡐����܂��B
	 *	@note �ʏ�A���������m�[�h�́A����Element�m�[�h�� AppendChildNode() ���܂��B 
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B
	 */
	CXmlComment Clone(void) const
	{
		return m_pNode->cloneNode(VARIANT_FALSE);
	}

private:
	friend class CXmlTest;
};



/**@ingroup XML
 * XML ProcessingInstruction �Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł� ProcessingInstruction �m�[�h������Ǘ����܂��B
 *
 *	@note �C���X�^���X�͊�{�I�� CXmlDocument::GetProcessingInstruction() ����쐬����܂��B
 *
 *	@note �q�m�[�h�́A�ʏ킠��܂���B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlProcessingInstruction : public CAbstractXmlNode
{
	DEFSUPER(CAbstractXmlNode);
protected:

	MSXML::IXMLDOMNamedNodeMapPtr	m_pAttr;	///< �m�[�h���O�}�b�v�N���X�̃|�C���^�n���h��

public:

	/** 
	 * �R���X�g���N�^
	 *	@note �����ȃC���X�^���X�ɐ���܂��B
	 */
	CXmlProcessingInstruction(void) : _super(NULL), m_pAttr(NULL)
	{
	}

	/** 
	 * �R���X�g���N�^
	 *	@note �w��̃m�[�h�� PROCESSING INSTRUCTION �ł͂Ȃ����A�����ȃC���X�^���X�ɐ���܂��B
	 *	@param n �m�[�h�Ǘ��N���X
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
	 * [���] ���
	 *	@param n �m�[�h�Ǘ�
	 *	@return �����̎Q��
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
	 * [�擾] �������擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̕ێ����鑮�������擾���邱�Ƃ��o���܂��B
	 *	@return �������B
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
	 * [�擾] �����擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̕ێ����鑮�����C���f�b�N�X�w��Ŏ擾���邱�Ƃ��o���܂��B
	 *	@param index �C���f�b�N�X�B�O�` GetAttributeLength()-1 �܂ŁB
	 *	@return �����B
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
	 * [�擾] �����擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̕ێ����鑮���𖼑O�w��Ŏ擾���邱�Ƃ��o���܂��B
	 *	@param name ���O�B
	 *	@return �����B�w��̖��O�̑������Ȃ��ꍇ�A�����ȑ����Ǘ��C���X�^���X���Ԃ�܂��B
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
	 * [�擾] �����l�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̕ێ�����w��̑����̒l���擾���邱�Ƃ��o���܂��B
	 *	@param name ���O�B
	 *	@return �l�B�w��̑������Ȃ��ꍇ�AEmpty�����񂪕Ԃ�܂��B
	 */
	CStr GetAttributeValue(LPCTSTR name) const
	{
		CXmlAttribute a = GetAttributeByName(name);
		return a.IsValid() ? a.GetValue() : CStr();
	}

	/**
	 * [�ݒ�] �����ݒ�.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�ɑ�����ǉ��ł��܂��B
	 *	@note �������O�Ȃ�l���X�V����܂��B
	 *	@param name ���O�B
	 *	@param value �l�B
	 *	@return ���ۂɓo�^����������ێ����鑮���Ǘ��C���X�^���X�B
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
	 * [�ݒ�] �����폜.
	 *		�{�C���X�^���X�ŕێ����Ă���A�w��̖��O�̃m�[�h���폜���܂��B
	 *	@param name ���O�B
	 *	@retval true �폜�����B
	 *	@retval false ���s or �w��̖��̑����Ȃ��B
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
 * XML Element �m�[�h�Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł� Element �m�[�h������Ǘ����܂��B
 *
 *	@note	�C���X�^���X�͊�{�I�� CXmlElement::AppendChildElement() �̖߂�l�� CXmlNodeList ����쐬������A 
 *			CXmlNode �����錾���܂��B
 *
 *	@note �q�m�[�h����́A CXmlNodeList ���擾���čs���܂��B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlElement : public CXmlProcessingInstruction
{
	DEFSUPER(CXmlProcessingInstruction);
public:

	/** 
	 * �R���X�g���N�^
	 *	@note �����ȃC���X�^���X�ɐ���܂��B
	 */
	CXmlElement(void) : _super()
	{
	}

	/** 
	 * �R���X�g���N�^
	 *	@note �w��̃m�[�h�� Element �m�[�h�ł͂Ȃ����A�����ȃC���X�^���X�ɐ���܂��B
	 *	@param n �m�[�h�Ǘ��N���X
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
	 * [���] ���
	 *	@param n �m�[�h�Ǘ�
	 *	@return �����̎Q��
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
	 * [�ύX] ���O�ύX.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̖��O��ύX���܂��B
	 *	@param newName �V�������O
	 */
	void Rename(LPCTSTR newName)
	{
		if ( GetNodeName().IsEqual(newName) )
		{
			return;
		}
		CXmlNodePtr n = m_pNode->GetownerDocument()->createElement(newName);
		// �������z��
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
		// �m�[�h���z��
		MSXML::IXMLDOMNodePtr pChild;
		while ( (pChild = m_pNode->GetfirstChild()) != NULL )
		{
			n->appendChild(pChild);
		}
		// ����ւ�
		MSXML::IXMLDOMNodePtr p = m_pNode->GetparentNode();
		p->replaceChild(n, m_pNode);
		m_pNode = n;
	}

	/** 
	 * [�m�F] �q�m�[�h�L���m�F.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̉��ɂԂ牺����m�[�h�̗L�����m�F���܂��B
	 *	@retval true �q�m�[�h����B
	 *	@retval false �q�m�[�h�Ȃ��B
	 */
	bool HasChildNodes(void) const
	{
		if ( ! IsValid() ) { return false; }
		return VARIANT_FALSE != m_pNode->hasChildNodes();
	}

	/** 
	 * [�擾] �ŏ��̎q�m�[�h�擾.
	 *	@note �Ȃ���΁A������ �m�[�h���Ԃ�܂��B
	 *	@return �m�[�h�B
	 */
	CXmlNode GetFirstChild(void) const
	{
		return m_pNode->GetfirstChild();
	}

	/** 
	 * [�擾] �q�m�[�h�Q�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̉��ɂԂ牺����m�[�h�ꗗ���擾���܂��B
	 *	@return �m�[�h�ꗗ�B
	 */
	CXmlNodeList GetChildNodes(void) const
	{
		return CXmlNodeList(m_pNode->GetchildNodes(), m_pNode);
	}

	/** 
	 * [����] ������v�m�[�h����.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̉��ɂԂ牺����m�[�h�Ō������ɍ����m�[�h�ꗗ���쐬���܂��B
	 *	@param expression ������(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath��</a>)
	 *	@return �m�[�h�ꗗ�B
	 */
	CXmlNodeList SelectNodes(LPCTSTR expression) const
	{
		return CXmlNodeList(m_pNode->selectNodes(expression), m_pNode);
	}

	/** 
	 * [����] ������v�m�[�h����.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̉��ɂԂ牺����m�[�h�Ō������ɍ����m�[�h���擾���܂��B
	 *	@param expression ������(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath��</a>)
	 *	@return �m�[�h�B
	 */
	CXmlNode SelectSingleNode(LPCTSTR expression) const
	{
		return CXmlNode(m_pNode->selectSingleNode(expression));
	}
	
	/** 
	 * [�擾] �e Element �m�[�h�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�̐e Element �m�[�h���擾���܂��B
	 *	@return Element �m�[�h�B�e���Ȃ��ꍇ�A������Ԃ̃C���X�^���X���Ԃ�܂��B
	 */
	CXmlElement GetParentNode(void) const
	{
		return m_pNode->GetparentNode();
	}

	/** 
	 * [�쐬] �N���[���쐬.
	 *		�{�C���X�^���X�ŕێ����Ă���m�[�h�𕡐����܂��B
	 *	@note �ʏ�A���������m�[�h�́A����Element�m�[�h�� AppendChildNode() ���܂��B 
	 *	@param boIsDeep �ȗ����邩 true �Ȃ�A�{�C���X�^���X���ێ����Ă���m�[�h�ȉ��A���ׂĂ𕡐����܂��B
	 *					false �Ȃ�A�ێ����Ă���m�[�h�̂ݕ������܂��B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B
	 */
	CXmlNode Clone(bool boIsDeep = true) const
	{
		return m_pNode->cloneNode(boIsDeep ? VARIANT_TRUE : VARIANT_FALSE);
	}

	/**
	 * [�ǉ�] �R�����g�q�m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���q�m�[�h�Q�̍Ŕ���Ɏw��̃m�[�h��ǉ����܂��B
	 *	@param text �ǉ�����R�����g������B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B
	 */
	CXmlComment AppendChildComment(LPCTSTR text)
	{
		return GetChildNodes().AppendNode(CXmlNodeList::COMMENT, text);
	}

	/**
	 * [�ǉ�] �e�L�X�g�q�m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���q�m�[�h�Q�̍Ŕ���Ɏw��̃m�[�h��ǉ����܂��B
	 *	@param text �ǉ�����e�L�X�g������B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B 
	 */
	CXmlText AppendChildText(LPCTSTR text)
	{
		return GetChildNodes().AppendNode(CXmlNodeList::TEXT, text);
	}

	/**
	 * [�ǉ�] Element �q�m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���q�m�[�h�Q�̍Ŕ���Ɏw��̃m�[�h��ǉ����܂��B
	 *	@param text �ǉ����� Element ������B
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B
	 */
	CXmlElement AppendChildElement(LPCTSTR text)
	{
		return GetChildNodes().AppendNode(CXmlNodeList::ELEMENT, text);
	}

	/**
	 * [�ǉ�] �q�m�[�h�ǉ�.
	 *		�{�C���X�^���X�ŊǗ����Ă���q�m�[�h�Q�̍Ŕ���Ɏw��̃m�[�h��ǉ����܂��B
	 *	@attention �ǉ�����m�[�h���A���łɃh�L�������g���ɂ���ꍇ�A�������ꂸ�Ɉړ��ɂȂ�܂��B
	 *	@note a �� CXmlDocumentFragment �C���X�^���X���w�肵���ꍇ�A�Ԃ牺�����Ă���m�[�h�����ׂĖ{�C���X�^���X�Ǘ��m�[�h���Ɉړ����A
	 *			CXmlDocumentFragment �C���X�^���X�͋���ۂɂȂ�܂��B
	 *	@param a �ǉ����� �m�[�h�B 
	 *	@return �m�[�h(�ǉ����ꂽ���Ԃ̃m�[�h�̎Q��)�B
	 */
	CXmlNode AppendChildNode(CAbstractXmlNode& a)
	{
		return m_pNode->appendChild(a.operator ->());
	}

private:
	friend class CXmlTest;
};
	
	
	
/**@ingroup XML
 * XML DocumentFragment �m�[�h�Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł� DocumentFragment �m�[�h������Ǘ����܂��B
 *
 *	@note �C���X�^���X�� CXmlDocument::CreateDocumentFragment() ����쐬���܂��B
 *
 *	@note �q�m�[�h����́A CXmlNodeList ���擾���čs���܂��B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 */
class CXmlDocumentFragment : public CXmlElement
{
	DEFSUPER(CXmlElement);
public:

	/** 
	 * �R���X�g���N�^
	 *	@note �w��̃m�[�h�� Element �m�[�h�ł͂Ȃ����A�����ȃC���X�^���X�ɐ���܂��B
	 *	@param n �m�[�h�Ǘ��N���X
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
 * XML �h�L�������g�Ǘ��N���X
 *
 *		XML �̊Ǘ��N���X�̈�BXML�p�[�T�̃N���X�����b�v���Ă��Ă��܂��B
 *
 *		�{�N���X�̃C���X�^���X�ł� �h�L�������g����Ǘ����܂��B
 *
 *	@note �q�m�[�h�͒��ڒǉ��ł��܂���B GetRootElement() ��p���āA CXmlElement �C���X�^���X���擾���A
 *		����𑀍삵�܂��B
 *
 *	@note �{�C���X�^���X����쐬���ꂽ�m�[�h�Ȃǂ̃C���X�^���X�������Ă���Ԃ́A�{�C���X�^���X��
 *			�j�����Ȃ��l�ɂ��Ă��������B
 *
 *	@attention �قȂ� Document�Ԃł̃m�[�h�̈ړ��͏o���܂���B
 *	
 *	@attention	�{�N���X�̃C���X�^���X���璊�o���ꂽ�C���X�^���X�́A�{�C���X�^���X�Ƀ����N���Ă���A
 *				���삷��Ƒ��e�����󂯂܂��B
 *
 *	@throw _com_error ���s����ƃX���[����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXml.h
 *
 *	@date 07/05/21 �V�K
 *	@date 08/04/16 �f�X�g���N�^�̏��������P
 */
class CXmlDocument
{
	MSXML::IXMLDOMDocumentPtr	m_pDoc;		///< DOC�N���X�|�C���^
	HRESULT						m_initRes;	///< CoInitialize�̖߂�l
	/// �ǂݍ���ł���������j�����܂��B
	void m_Release(void)
	{
		m_pDoc = NULL;
	}
	/// ������
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
	 * �Ǎ�.
	 *	@note ��ɓǂݍ���ł��������͔j������܂��B
	 *	@param text ������
	 *	@param isFile true;lpsz�̓t�@�C�����w��Afalse;lpsz��XML�`���ŏ����ꂽ������w��
	 *	@retval true ����
	 *	@retval false ���s
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

	/// �R���X�g���N�^
	CXmlDocument(void) : m_pDoc(NULL)
	{
		m_initRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	}

	/// �f�X�g���N�^
	virtual ~CXmlDocument(void)
	{
		m_Release();
		if ( m_initRes == S_OK )
		{
			::CoUninitialize();
		}
	}

	/**
	 * [�m�F] �L���m�F.
	 *		�{�C���X�^���X���L���ȏ���ێ����Ă��邩�A�m�F�ł��܂��B
	 *	@retval true �L���B
	 *	@retval false �����B
	 */
	bool IsValid(void) const
	{
		return m_pDoc != NULL;
	}

	/**
	 * [�ݒ�] ������.
	 *		�{�C���X�^���X�̕ێ����Ă�����𖳌��ɂ��܂��B
	 */
	void Invalid(void)
	{
		m_pDoc = NULL;
	}

	/**
	 * [�擾] �G���[�R�[�h
	 *	@return �G���[�R�[�h
	 */
	DWORD GetLastError(void) const
	{
		return m_pDoc->GetparseError()->GeterrorCode();
	}

	/**
	 * [�擾] �G���[�R�[�h������
	 *	@return �G���[�R�[�h������
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
	 * [����] ������
	 *	@note �܂������ XML ���쐬���A Element ����쐬���܂��B
	 *	@param lpszRootElementName ���[�g Element �m�[�h���B
	 *	@param lpszComment �R�����g�B�ȗ������ ���[�g�ɃR�����g�͍쐬����܂���B
	 *	@param lpszTarget �^�[�Q�b�g���B�ȗ������ProcessingInstruction�͍쐬����܂���B
	 *	@param lpszData �f�[�^���B�ȗ������ProcessingInstruction�͍쐬����܂���B
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
	 * [����] ������
	 *	@note �܂������ XML ���쐬���A Element ����쐬���܂��B
	 *	@param lpszRootElementName ���[�g Element �m�[�h���B
	 *	@param lpszComment �R�����g�B�ȗ������ ���[�g�ɃR�����g�͍쐬����܂���B
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
	 * [�ݒ�] XML�t�@�C���Ǎ�
	 *	@param lpszFileName �t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool LoadFile(LPCTSTR lpszFileName)
	{
		return m_Load(lpszFileName, true);
	}

	/**
	 * [�ݒ�] XML�e�L�X�g�Ǎ�
	 *	@param lpszXml �e�L�X�g
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool LoadXml(LPCTSTR lpszXml)
	{
		return m_Load(lpszXml, false);
	}

	/**
	 * [�쐬] XML�t�@�C���쐬
	 *	@param lpszFileName �t�@�C����
	 *	@param boIsUtf8 false �Ȃ� Shift-JIS�ō쐬����܂��B 
	 *					�ȗ��� ture �Ȃ� UTF-8 �ō쐬����܂��B
	 *					�������AProcessingInstruction���Ȃ��ꍇ�A �K��UTF-8�ō쐬����܂��B
	 *	@retval true ����
	 *	@retval false ���s
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
	 * [�쐬] XML�t�@�C���쐬
	 *	@note ���s��C���f���g�t���ŏo�͂���܂��B
	 *	@note ProcessingInstruction �m�[�h�͕K���o�͂���܂��B
	 *	@todo �R�����g�m�[�h���o�͂���܂���B
	 *
	 *	@param lpszFileName �t�@�C�����B�����̃t�@�C�������w�肷��ƍ폜���č쐬���܂��B�ǋL�͂���܂���B
	 *	@param boIsUtf8 false �Ȃ� Shift-JIS�ō쐬����܂��B 
	 *					�ȗ��� ture �Ȃ� UTF-8 �ō쐬����܂��B
	 *	@retval true ����
	 *	@retval false ���s
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
	 * [�쐬] XML�e�L�X�g�쐬
	 *	@attention	encoding �̑����͏������܂�܂���B�����A���̕�������t�@�C���ɏ����o�������ꍇ�A
	 *				UTF-8 �ɕϊ����Ă��珑���o���K�v������܂��iencoding �̃f�t�H���g�� UTF-8������j�B
	 *	@return ������� XML�e�L�X�g�B���s�̏ꍇEmpty��ԕ����񂪕Ԃ�܂��B
	 */
	CStr ToString(void) const
	{
		_bstr_t str = m_pDoc->Getxml();
		return CStr(LPCTSTR(str));
	}

	/** 
	 * [�擾] �q�m�[�h�Q�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̒����ɂԂ牺����m�[�h�ꗗ���擾���܂��B
	 *	@return �m�[�h�ꗗ�B
	 */
	CXmlNodeList GetChildNodes(void) const
	{
		return CXmlNodeList(m_pDoc->GetchildNodes(), m_pDoc, m_pDoc);
	}

	/** 
	 * [�擾] ProcessingInstruction �m�[�h�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̒����ɂԂ牺���� ProcessingInstruction �m�[�h�擾���܂��B
	 *	@return ProcessingInstruction �Ǘ��C���X�^���X
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
	 * [�擾] Element �m�[�h�擾.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̒����ɂԂ牺���� �B��� Element �m�[�h�擾���܂��B
	 *	@return Element �m�[�h�Ǘ�
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
	 * [�쐬] DocumentFragment�쐬.
	 *	@return DocumentFragment
	 */
	CXmlDocumentFragment CreateDocumentFragment(void) const
	{
		return CXmlDocumentFragment(m_pDoc->createDocumentFragment());
	}

	/** 
	 * [����] ������v�m�[�h����.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̉��ɂԂ牺���� Element �m�[�h�ŏ����ɍ����m�[�h�ꗗ���쐬���܂��B
	 *	@param name �m�[�h���B
	 *	@return �m�[�h�ꗗ�B
	 */
	CXmlNodeList GetElementsByTagName(LPCTSTR name) const
	{
		return CXmlNodeList(m_pDoc->getElementsByTagName(name), m_pDoc, m_pDoc);
	}

	/** 
	 * [����] ������v�m�[�h����.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̉��ɂԂ牺����m�[�h�Ō������ɍ����m�[�h�ꗗ���쐬���܂��B
	 *	@param expression ������(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath��</a>)
	 *	@return �m�[�h�ꗗ�B
	 */
	CXmlNodeList SelectNodes(LPCTSTR expression) const
	{
		return CXmlNodeList(m_pDoc->selectNodes(expression), m_pDoc, m_pDoc);
	}

	/** 
	 * [����] ������v�m�[�h����.
	 *		�{�C���X�^���X�ŕێ����Ă���h�L�������g�̉��ɂԂ牺����m�[�h�Ō������ɍ����m�[�h���擾���܂��B
	 *	@param expression ������(<a class="el" href="http://www.infoteria.com/jp/contents/xml-data/REC-xpath-19991116-jpn.htm">XPath��</a>)
	 *	@return �m�[�h�B
	 */
	CXmlNode SelectSingleNode(LPCTSTR expression) const
	{
		return CXmlNode(m_pDoc->selectSingleNode(expression));
	}

private:
	friend class CXmlTest;
};



}; // TNB


