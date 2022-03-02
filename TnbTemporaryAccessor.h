#pragma once
/**
 *@file
 * �e���|�������A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAccessor.h"
#include "TnbMap.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * �e���|�������A�N�Z�X�N���X
 *
 *		�Z�N�V�����ŋ�؂�ꂽ���ɕ����̃L�[�ƒl�����`�̏����������ŊǗ�����N���X�ł��B
 *		�v���Z�X���I��������A���͔j������Ă��܂��܂��i�c��܂���j�B
 *
 *		�R���X�g���N�^�� SetBase() �ŏ��̃x�[�X���w�肵�A
 *		operator[]() �� �Z�N�V�������A�N�Z�X�N���X({@link IAccessor::CSection CSection})���擾���A
 *		Query�`() �A Write�`() �ȂǂŃL�[���ɃA�N�Z�X���܂��B
 *
 *	@note	�{�C���^�[�t�F�[�X�ɂ� QueryValue() , WriteValue() �ȂǁA
 *			�Z�N�V�������w�肵�L�[���𑀍삷�郁�\�b�h������܂����A
 *			��{�I�� {@link IAccessor::CSection CSection} ���g�p���Ă��������B
 *
 *	@see	IAccessor ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTemporaryAccessor.h
 *
 *	@date 06/06/09 �V�K�쐬
 *	@date 06/10/31 IInformation �̉��C�ɑΉ��B
 *	@date 07/05/10 �N���X���ύX�B
 */
class CTemporaryAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
	typedef CMapT<CStr, CValue, LPCTSTR> CKeyMap;		///< KEY�̃}�b�v�^���錾

	mutable CStr			m_strSecName;				///< �A�N�e�B�u�̃Z�N�V������		
	mutable CKeyMap*		m_pmapKey;					///< �A�N�e�B�u�̃Z�N�V�����}�b�v�̎Q��

protected:

	mutable CMapT<CStr, CKeyMap, LPCTSTR> m_mapSec;	///< �Z�N�V�����}�b�v

private:

	/**
	 * �Q�ƃ��Z�b�g
	 *	@note �Z�N�V�����}�b�v�𑀍삵�����R�[�����܂��B
	 */
	void m_Reset(void) const
	{
		m_pmapKey = NULL;
	}

	/**
	 * �L�[�}�b�v�̃A�h���X�擾
	 *	@retval NULL �A�N�e�B�u�̃Z�N�V�����͂Ȃ�
	 *	@retval NULL�ȊO �L�[�}�b�v�̃A�h���X
	 */
	CKeyMap* m_CheckKeyMapP(void) const
	{
		if ( m_pmapKey == NULL && m_mapSec.HasKey(m_strSecName) )
		{
			m_pmapKey = &(m_mapSec[m_strSecName]);
		}
		return m_pmapKey;
	}

	/**
	 * �L�[�}�b�v�̃A�h���X�擾
	 *	@note �Ȃ��ꍇ�A�g�����܂��B
	 *	@return �L�[�}�b�v�̃A�h���X
	 */
	CKeyMap* m_GetKeyMapP(void)
	{
		if ( m_pmapKey == NULL )
		{
			m_pmapKey = &(m_mapSec[m_strSecName]);
		}
		return m_pmapKey;
	}

	/**
	 * �Z�N�V�������Z�b�g
	 */
	void m_SetSectionName(LPCTSTR lpszSectionName) const
	{
		if ( m_strSecName != lpszSectionName )
		{
			m_Reset();
			m_strSecName = lpszSectionName;
		}
	}

	/**
	 * �w��L�[�l�ݒ�
	 *	@param lpszKey �L�[
	 *	@param v �l
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool m_Write(LPCTSTR lpszKey, const CValue& v)
	{
		CKeyMap* P = m_GetKeyMapP();
		if ( P != NULL )
		{
			(*P)[lpszKey] = v;
			return true;
		}
		return false;
	}

	/**
	 * [�폜] �w��L�[�폜
	 *	@param lpszKey �L�[���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_Delete(LPCTSTR lpszKey)
	{
		CKeyMap* P = m_CheckKeyMapP();
		return (P != NULL) ? P->RemoveKey(lpszKey) : false;
	}	

public:
	
	/// �R���X�g���N�^
	CTemporaryAccessor(void) : _super(), m_pmapKey(NULL)
	{
	}

	/**
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Temp");
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
		CStrVector vs = m_mapSec.EnumKeys();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		if ( ! ss.IsEmpty() )
		{
			loop ( i, vs.GetSize() )
			{
				CStr s;
				if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
				{
					s = vs[i].Mid(len);
					if ( ! s.IsEmpty() && s[0] == '\\' )
					{
						s = s.Mid(1);
					}
					else
					{
						s.Empty();
					}
				}
				vs[i] = s;
			}
		}
		CStrVector vstrSecNames;
		loop ( i, vs.GetSize() )
		{
			if ( ! vs[i].IsEmpty() )
			{
				CStr s = vs[i];
				INDEX d = s.Find(_T('\\'));
				if ( d != INVALID_INDEX )
				{
					s = s.Left(d);
				}
				if ( vstrSecNames.Find(s) == INVALID_INDEX )
				{
					vstrSecNames.Add(s);
				}
			}
		}
		return vstrSecNames;
	}

	/**
	 * [�폜] �w��Z�N�V�����폜
	 *	@param lpszSectionName �Z�N�V�������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		m_Reset();
		CStrVector vs = m_mapSec.EnumKeys();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		loop ( i, vs.GetSize() )
		{
			if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
			{
				if ( ! m_mapSec.RemoveKey(vs[i]) )
				{
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * [�擾] �L�[���ꗗ�擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return �L�[���ꗗ�B
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		m_SetSectionName(lpszSectionName);
		CKeyMap* P = m_CheckKeyMapP();
		return (P == NULL) ? CStrVector() : P->EnumKeys();
	}

	/**
	 * [�擾] �����擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return ����B�Ȃ��ꍇ�AEK_Nothing �ɂȂ�܂��B
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		m_SetSectionName(lpszSectionName);
		CKeyMap* P = m_CheckKeyMapP();
		if ( P == NULL )
		{
			return EK_Nothing;
		}
		CValue v;
		return (P->Lookup(lpszKey, v)) ? v.GetKind() : EK_Nothing;
	}

	/**
	 * [�擾] ���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		m_SetSectionName(lpszSectionName);
		CKeyMap* P = m_CheckKeyMapP();
		CValue v;
		if ( P != NULL )
		{
			P->Lookup(lpszKey, v);
		}
		return v;
	}

	/**
	 * [�ݒ�] ���ݒ�
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param value �ݒ肷��l
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		m_SetSectionName(lpszSectionName);
		if ( value.GetKind() == EK_Nothing )
		{
			return m_Delete(lpszKey); 
		}
		return m_Write(lpszKey, value);
	}
};



}; // TNB
