#pragma once
/**
 *@file
 * XML�t�@�C���A�N�Z�T�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbXmlAccessor.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR XML
 * XML�t�@�C���A�N�Z�T�[�N���X.
 *
 *		XML�t�@�C���ɃA�N�Z�X���ăf�[�^���i�[���邽�߂̃N���X�ł��B
 *
 *	@todo �G���[�������s�\��
 *
 *	@par�K�v�t�@�C��
 *			TnbXmlAccessor.h
 *
 *	@date 08/04/22 �V�K�쐬
 */
class CXmlFileAccessor : public CXmlAccessor
{
	DEFSUPER(CXmlAccessor);
	CStr			m_strFileName;		///< �t�@�C����
	CXmlDocument	m_xml;				///< XML

public:
	
	/** 
	 * �R���X�g���N�^
	 */
	explicit CXmlFileAccessor(void) : _super()
	{
	}

	/** 
	 * �R���X�g���N�^
	 *	@param lpszFileName �t�@�C����
	 *	@param lpszRootText �w�b�_������
	 */
	explicit CXmlFileAccessor(LPCTSTR lpszFileName, LPCTSTR lpszRootText) : _super()
	{
		SetBase(lpszFileName, lpszRootText);
	}

	/// �f�X�g���N�^
	virtual ~CXmlFileAccessor(void)
	{
	}

	/**
	 * [�ݒ�] �x�[�X�w��
	 *	@param lpszFileName �t�@�C����
	 *	@param lpszRootText �w�b�_������
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
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("XmlFile");
	}

	/**
	 * [����] �t���b�V��.
	 *	@note �R���X�g���N�^�� SetBase �Ŏw�肵���t�@�C���ɕύX�������o���܂��B
	 *		���̃��\�b�h���R�[�����Ȃ�����A�t�@�C���͕ω����܂���B
	 *	@retval true ����
	 *	@retval false ���s
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



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
