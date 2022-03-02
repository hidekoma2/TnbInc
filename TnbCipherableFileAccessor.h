#pragma once
/**
 *@file
 * �Í��t�@�C�����A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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
 * �Í��t�@�C�����A�N�Z�X�N���X
 *
 *		�Z�N�V�����ŋ�؂�ꂽ���ɕ����̃L�[�ƒl�����`�̏����������ŊǗ����A
 *		�C�ӂ̃^�C�~���O�ŁA�t�@�C���ɕۑ����Ă����A������𕜌����邱�Ƃ�
 *		�o����N���X�ł��B���̃t�@�C���́A�Í�������Ă���A�܂��ACRC���t������
 *		���邽�߁A�ۑ������Q�Ƌ֎~�A������֎~�������ꍇ�Ɏg�p�ł��܂��B
 *
 *		�R���X�g���N�^�� SetBase() �ŏ��̃x�[�X���w�肵�A
 *		operator[]() �� �Z�N�V�������A�N�Z�X�N���X({@link IAccessor::CSection CSection})���擾���A
 *		Query�`() �A Write�`() �ȂǂŃL�[���ɃA�N�Z�X���܂��B
 *
 *	@attention Write�`() �ŏ���ύX���Ă��A Flush() ���R�[�����Ȃ�����t�@�C���ɏ����o���܂���B
 *
 *	@note	�{�C���^�[�t�F�[�X�ɂ� QueryValue() , WriteValue() �ȂǁA
 *			�Z�N�V�������w�肵�L�[���𑀍삷�郁�\�b�h������܂����A
 *			��{�I�� {@link IAccessor::CSection CSection} ���g�p���Ă��������B
 *
 *	@see	IIAccessor ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCipherableFileAccessor.h
 *
 *	@date 06/06/12 �V�K�쐬
 */
class CCipherableFileAccessor : public CTemporaryAccessor
{
	DEFSUPER(CTemporaryAccessor);
	CStr			m_strFileName;		///< �t�@�C����
	CStr			m_strHeadText;		///< �w�b�_���ʎq
	CBytesCipher		m_cipher;		///< �Í������N���X

	/**
	 * �t�@�C���ǂݍ���
	 *	@param lpszFile �t�@�C����
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
	 * �t�@�C�������o��
	 *	@param lpszFile �t�@�C����
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
	 * �R���X�g���N�^
	 *	@param lpszFileName �t�@�C����
	 *	@param lpszHeadText �w�b�_������
	 *	@param lpszKey �Í��L�[
	 *	@throw CNotSupportException		�w��t�@�C���̃w�b�_�`�󂪈قȂ�ƃX���[����܂��B
	 *	@throw CInvalidParamException	�w��t�@�C���̃t�@�C���������قȂ�ƃX���[����܂��B
	 *	@throw CNoSuchTypeException		�w��t�@�C���̃`�F�b�N�T�����قȂ�ƃX���[����܂��B
	 *	@throw CReadFailureException	�w��t�@�C���̓ǂݍ��݂Ɏ��s����ƃX���[����܂��B 
	 */
	explicit CCipherableFileAccessor(LPCTSTR lpszFileName, LPCTSTR lpszHeadText, LPCTSTR lpszKey) : _super()
	{
		SetBase(lpszFileName, lpszHeadText, lpszKey);
	}

	/**
	 * [�ݒ�] �x�[�X�w��
	 *	@param lpszFileName �t�@�C����
	 *	@param lpszHeadText �w�b�_������
	 *	@param lpszKey �Í��L�[
	 *	@throw CNotSupportException		�w��t�@�C���̃w�b�_�`�󂪈قȂ�ƃX���[����܂��B
	 *	@throw CInvalidParamException	�w��t�@�C���̃t�@�C���������قȂ�ƃX���[����܂��B
	 *	@throw CNoSuchTypeException		�w��t�@�C���̃`�F�b�N�T�����قȂ�ƃX���[����܂��B
	 *	@throw CReadFailureException	�w��t�@�C���̓ǂݍ��݂Ɏ��s����ƃX���[����܂��B 
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
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("CipherableFile");
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
 * [�ǉ�] �V���A���C�Y
 *	@param[out] _sr �V���A���C�U�[
 *	@param[in] t �f�[�^
 *	@return ISerializer �̎Q��
 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
 */
inline ISerializer& operator<<(ISerializer& _sr, const IAccessor::CValue& t)
{
	DWORD dwKind = t.GetKind();
	_sr << dwKind;
	//
	switch ( dwKind )
	{
	case IAccessor::EK_Nothing:// ���݂��Ȃ�
		break;
	case IAccessor::EK_String:	// ������
		_sr << t.QueryString();
		break;
	case IAccessor::EK_PluralStrings:	// �����̕�����Q
		_sr << t.QueryPluralStrings();
		break;
	case IAccessor::EK_Binary:		// �o�C�i��
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
 * [�擾] �f�V���A���C�Y
 *	@param[in] ds �f�V���A���C�U�[
 *	@param[out] _t �f�[�^
 *	@return IDeserializer �̎Q��
 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, IAccessor::CValue& _t)
{
	DWORD dwKind;
	ds >> dwKind;
	//
	switch ( dwKind )
	{
	case IAccessor::EK_Nothing:// ���݂��Ȃ�
		_t = IAccessor::CValue();
		break;
	case IAccessor::EK_String:	// ������
		{
			CStr s;
			ds >> s;
			_t = IAccessor::CValue(s);
		}
		break;
	case IAccessor::EK_PluralStrings:	// �����̕�����Q
		{
			CStrVector vs;
			ds >> vs;
			_t = IAccessor::CValue(vs);
		}
		break;
	case IAccessor::EK_Binary:		// �o�C�i��
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