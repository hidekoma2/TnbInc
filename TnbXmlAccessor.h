#pragma once
/**
 *@file
 * XML�A�N�Z�T�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbXml.h"
#include "TnbAccessor.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR XML
 * XML�A�N�Z�T�[�N���X.
 *
 *		XML�t�@�C���ɃA�N�Z�X���ăf�[�^���i�[���邽�߂̃N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbXmlAccessor.h
 *
 *	@date 08/04/15 �V�K�쐬
 */
class CXmlAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
	CXmlElement			m_masterElement;///< �ǉ���̐e Element�B
	mutable CXmlElement	m_element;		///< �A�N�Z�X�Ώۂ� Element�B
	mutable CStr		m_strSection;	///< �Ώۂ̃T�u�L�[

	/**
	 * �Z�N�V�������ݒ�
	 *	@param lpszSectionName �Z�N�V�������B ���̖��O�� Element��T���L�����܂��B
	 *	@param isCreateMode true �Ȃ�A�w��̃Z�N�V���������Ȃ���� Element���쐬���܂��B
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
				m_element = m_masterElement.SelectSingleNode(sec);	//����ő��v���H
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

	/// �R���X�g���N�^ 
	CXmlAccessor(void) : m_masterElement(), m_strSection(_T("//"))
	{
	}

	/**
	 * �R���X�g���N�^ 
	 *	@param other �G�������g
	 */
	explicit CXmlAccessor(const CXmlElement& other) 
		: m_masterElement(other), m_strSection(_T("//"))
	{
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CXmlAccessor(void) 
	{
		m_element.Invalid();
		m_masterElement.Invalid();
	}

	/**
	 * [�ݒ�] �ΏۃG�������g�w��
	 *	@param other �G�������g
	 */
	void SetBase(const CXmlElement& other)
	{
		m_masterElement = other;
		m_strSection = _T("//");
	}

	/**
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("XML");
	}

	/**
	 * [����] �t���b�V��.
	 *	@note �{�N���X�ł͉������܂���B
	 *	@return true ����
	 */
	virtual bool Flush(void)
	{
		return true;
	}

	/**
	 * [�擾] �Z�N�V�������ꗗ�擾
	 *	@param lpszSectionName �Z�N�V�������B NULL���w�肷��ƁAROOT�̃Z�N�V�����ꗗ�������܂��B
	 *	@return �Z�N�V�������ꗗ
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
	 * [�폜] �w��Z�N�V�����폜
	 *	@param lpszSectionName �Z�N�V�������B
	 *	@retval true ����
	 *	@retval false ���s
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
	 * [�擾] ���O�ꗗ�擾
	 *	@note �{�N���X�ł� pure�֐��ł��B
	 *	@param lpszSectionName �Z�N�V������
	 *	@return ���O�ꗗ�B������ԂȂ疢�Ή��B
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
	 * [�擾] �����擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return ����B�Ȃ��ꍇ�AEK_Nothing �ɂȂ�܂��B
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
	 * [�擾] ���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
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
	 * [�ݒ�] ���ݒ�
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param value �ݒ肷��l�BEK_Nothing �̏ꍇ�A�L�[���폜���܂��B
	 *	@retval true ����
	 *	@retval false ���s
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



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
