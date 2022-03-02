#pragma once
/**
 *@file
 * ���A�N�Z�X�֌W�̃w�b�_
 *
 *	@note	�}�N�� "_TnbACCESSOR_Tree_ENABLE" ����`����Ă���ƁA IAccessor::CSection::GetKeyMap() , 
 *			IAccessor::CSection::GetKeyTree() , CAbstractAccessor::GetKeyTree()
 *			���g�p�\�ɂ��܂��iTnbMap.h TnbTree.h ���K�v�ɂȂ�܂��j�B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"
#include "TnbCollection.h"
#include "TnbStrOperator.h"
#include "TnbSerializeAdapter.h"
#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #define _TnbACCESSOR_Tree_ENABLE ///< �@
#endif
#ifdef _TnbACCESSOR_Tree_ENABLE
 #include "TnbTree.h"
 #include "TnbMap.h"
#endif



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * ���A�N�Z�X�C���^�[�t�F�[�X.
 *
 *		�Z�N�V�����ŋ�؂�ꂽ���ɕ����̃L�[�ƒl�����`�̏����Ǘ�����C���^�[�t�F�[�X�ł��B
 *
 *		�R���X�g���N�^�Ȃǂŏ��̃x�[�X���w�肵�A
 *		operator[]() �� �Z�N�V�������A�N�Z�X�N���X({@link IAccessor::CSection CSection})���擾���A
 *		Query�`() �A Write�`() �ȂǂŃL�[���ɃA�N�Z�X���܂��B
 *
 *	@note	�{�C���^�[�t�F�[�X�ɂ� QueryValue() , WriteValue() �ȂǁA
 *			�Z�N�V�������w�肵�L�[���𑀍삷�郁�\�b�h������܂����A
 *			��{�I�� {@link IAccessor::CSection CSection} ���g�p���Ă��������B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		CInifileAccessor ini;
 *		CInifileAccessor::CSection sec1 = ini["SEC1"];
 *		CInifileAccessor::CSection sec2 = ini["SEC2"];
 *		sec1.WriteString("KEY1", "VALUE1"); //�Z�N�V����[SEC1]�̃L�[[KEY1]��"VALUE1"����		
 *		sec1.WriteString("KEY2", "VALUE2"); //���Z�N�V�����ɃL�[[KEY1]��"VALUE1"����
 *		sec2.WriteString("KEY9", "VALUE9"); //�Z�N�V����[SEC2]�̃L�[[KEY9]��"VALUE9"����	
 *				;
 * 		CStrVector vs = sec1.EnumKeyNames(); //�Z�N�V����[SEC1]���̃L�[����S�擾
 *		</pre></td></tr></table>
 *
 *	@see	SetExceptionMode() �ɂ��A��񂪂Ȃ�������̌^���قȂ鎞�A 
 *			CNoSuchTypeException �� CEmptyException ���X���[�����邱�Ƃ��o���܂��B
 *			�ݒ肵�Ă��Ȃ����̓X���[���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbAccessor.h
 *
 *	@date 06/05/12 �V�K�쐬
 *	@date 06/06/08 �����o���ύX�B CSection �N���X�A�ǉ��B
 *	@date 06/10/31 �����o���C�B�L�[�A�N�Z�X�֌W�� CSection �N���X�Ɉړ��B
 *	@date 08/04/04 ���l�ɕϊ����AEMPTY�����Ȃ� Default��Ԃ��悤�ɂ����B
 */
struct IAccessor
{
	/**
	 * �l�^�̎��.
	 *	IAccessor �� IAccessor::CValue �Ŏg���^�̎�ނł��B
	 */
	enum EKind
	{
		EK_Nothing		= 'n',	///< ���݂��Ȃ�
		EK_String		= 's',	///< ������
		EK_PluralStrings= 'p',	///< �����̕�����Q
		EK_Binary		= 'b',	///< �o�C�i��
		EK_Dword		= 'd',	///< 32Bit Unsigned Integer
		EK_Longlong		= 'i',	///< 64Bit Signed Integer
	};

	/**@ingroup ACCESSOR
	 * ���A�N�Z�X�̔ėp�l�ێ��N���X.
	 *
	 *	{@link IAccessor::EKind EKind}�Œ�`�����^�ƒl��ێ��ł��܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbAccessor.h
	 *
	 *	@date 06/05/12 �V�K�쐬
	 */
	class CValue
	{
		EKind		m_eKind;		///< �ێ�����
		CStr		m_strData;		///< ������,�����̕�����Q���
		LONGLONG	m_llData;		///< Integer ���
		CByteVector	m_vbData;		///< �o�C�i�����
	public:
	
		/**
		 * �R���X�g���N�^ 
		 *	@note ��񖳂��I�u�W�F�N�g�ɂȂ�܂��B
		 */
		CValue(void) : m_eKind(EK_Nothing) 
		{
		}

		/**
		 * �R���X�g���N�^ 
		 *	@param lpsz ������B���̕�������������I�u�W�F�N�g�ɂȂ�܂��B
		 */
		CValue(LPCTSTR lpsz) : m_eKind(EK_String), m_strData(lpsz)
		{
		}

		/**
		 * �R���X�g���N�^ 
		 *	@param vstr ������Q�B���̕����̕�����Q���������I�u�W�F�N�g�ɂȂ�܂��B
		 */
		CValue(const CStrVector& vstr) : m_eKind(EK_PluralStrings), m_strData(StrVectorToStrEx(vstr)) 
		{
		}

		/**
		 * �R���X�g���N�^ 
		 *	@param c �f�[�^�B���̃o�C�i�����������I�u�W�F�N�g�ɂȂ�܂��B
		 */
		CValue(const IConstCollectionT<BYTE>& c) : m_eKind(EK_Binary)
		{
			m_vbData.Copy(c);
		}

		/**
		 * �R���X�g���N�^ 
		 *		�w��̃o�C�i�����������I�u�W�F�N�g�ɂȂ�܂��B
		 *	@param len �f�[�^�T�C�Y.
		 *	@param P �f�[�^
		 */
		CValue(size_t len, LPCVOID P) : m_eKind(EK_Binary)
		{
			m_vbData.CopyElements(len, static_cast<const BYTE*>(P));
		}

		/**
		 * �R���X�g���N�^ 
		 *	@param dw �f�[�^�B����32Bit Unsigned Integer���������I�u�W�F�N�g�ɂȂ�܂��B
		 */
		CValue(DWORD dw) : m_eKind(EK_Dword), m_llData(dw) 
		{
		}

		/**
		 * �R���X�g���N�^ 
		 *	@param ll �f�[�^�B����64Bit Signed Integer���������I�u�W�F�N�g�ɂȂ�܂��B
		 */
		CValue(LONGLONG ll) : m_eKind(EK_Longlong), m_llData(ll)
		{
		}

		/**
		 * [�擾] �����擾
		 *	@return ����
		 */
		EKind GetKind(void) const { return m_eKind; }

		/**
		 * [�m�F] ���L���m�F
		 *	@retval true ���L��
		 *	@retval false ��񖳂�
		 */
		bool IsNull(void) const { return m_eKind == EK_Nothing; }

		/**
		 * [�擾] ��������擾
		 *	@note �^���قȂ��Ă��ϊ����Ď��o���܂�
		 *	@return �l
		 */
		CStr QueryString(void) const
		{
			CStr s;
			switch ( m_eKind )
			{
			case EK_String:			// ������
			case EK_PluralStrings:	// �����̕�����Q
				s = m_strData;
				break;
			case EK_Binary:			// �o�C�i��
				s = CStrOperator::BinaryToHexString(m_vbData);
				break;
			case EK_Dword:			// 32Bit
				s = CStr::Fmt(_T("%u"), static_cast<DWORD>(m_llData));
				break;
			case EK_Longlong:		// 64Bit
				s = CStr::Fmt(_T("%I64d"), m_llData);
				break;
			case EK_Nothing:
				break;
			}	
			return s;
		}

		/**
		 * [�擾] ������Q���擾
		 *	@note �^���قȂ��Ă��ϊ����Ď��o���܂�
		 *	@return �l
		 */
		CStrVector QueryPluralStrings(void) const
		{
			CStrVector vstr;
			if ( m_eKind == EK_PluralStrings || m_eKind == EK_String )
			{
				vstr = StrToStrVectorEx(m_strData);
			}
			return vstr;
		}

		/**
		 * [�擾] �o�C�i�����擾
		 *	@note �^���قȂ��Ă��ϊ����Ď��o���܂�
		 *	@return �l
		 */
		CByteVector QueryData(void) const
		{
			if ( m_eKind != EK_Binary )
			{
				return CStrOperator::HexStringToBinary(QueryString());
			}
			return m_vbData;
		}

		/**
		 * [�擾] ���l���擾
		 *	@note �^���قȂ��Ă��ϊ����Ď��o���܂�
		 *	@return �l
		 */
		DWORD QueryDword(void) const
		{
			return static_cast<DWORD>(QueryLonglong()); 
		}

		/**
		 * [�擾] ���l���擾
		 *	@note �^���قȂ��Ă��ϊ����Ď��o���܂�
		 *	@return �l
		 */
		LONGLONG QueryLonglong(void) const
		{
			if ( m_eKind == EK_Longlong || m_eKind == EK_Dword )
			{
				return m_llData;
			}
			return QueryString().ToLonglong(); 
		}
	};

	#ifdef _TnbACCESSOR_Tree_ENABLE

	/// �L�[�ƒl�̃}�b�v�^�錾
	typedef CMapT<CStr, CValue, LPCTSTR> CKeyMap;

	/// �L�[�ƒl�̃c���[�^�錾
	typedef CTreeT<CStr, CValue, LPCTSTR> CKeyTree;

	#endif
	
	/**@ingroup ACCESSOR
	 * �Z�N�V�������A�N�Z�X�N���X
	 *
	 *		IAccessor �̃T�u�N���X�ɂ��쐬�����N���X�ł��B
	 *		�{�C���X�^���X�� IAccessor �̃T�u�N���X�̎w��̃Z�N�V�����̏��ɃA�N�Z�X���܂��B
	 *
	 *	@see IAccessor
	 *
	 *	@attention �{�N���X�͍쐬���� IAccessor �̃T�u�N���X�̃C���X�^���X�̎Q�Ƃ������Ă��܂��B
	 *				�K���{�N���X�̗L���̊Ԃ́A�쐬���̃C���X�^���X�͔j�����Ȃ��悤�ɂ��Ă��������B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbAccessor.h
	 *
	 *	@date 06/06/08 �V�K�쐬
	 *	@date 06/10/31 �����o�ǉ�
	 *	@date 09/04/02 HWND �ɂ��Ή�
	 *	@date 09/11/12 WriteData() ��ǉ��B
	 *	@date 10/05/17 �V���A���C�U�[��ǉ��B
	 */
	class CSection
	{
		IAccessor*		m_piAccessor;	///< ���N���X�̎Q��
		CStr			m_strSection;	///< �����Z�N�V������
		/**
		 * CValue�`�F�b�N
		 *	@retval ture NULL �� �����񂩂����
		 *	@retval false �l����
		 */
		bool m_IsEmpty(const CValue& v) const
		{
			bool r = v.IsNull();
			if ( ! r && v.GetKind() == EK_String )
			{
				r = v.QueryString().IsEmpty();
			}
			return r;
		}
		/** �R���X�g���N�^ */
		CSection(void);
	public:

		/**
		 * ����R���X�g���N�^
		 *	@param I IAccessor �����C���X�^���X�I�u�W�F�N�g
		 *	@param lpszSectionName �{�N���X�ň����Z�N�V������
		 *	@throw CNullPointerException IAccessor�N���X���w�肵�Ȃ��ƃX���[����܂��B
		 */
		CSection(const IAccessor* I, LPCTSTR lpszSectionName) 
			: m_piAccessor(const_cast<IAccessor *>(I)), m_strSection(lpszSectionName)
		{
			if ( I == NULL )
			{
				ASSERT0( false, "CSection::CSection()", "IAccessor ���w�肳��Ă��܂���B" );
				throw CNullPointerException();
			}
		}

		/**
		 * [�擾] �Z�N�V�������擾
		 *	@return �Z�N�V������
		 */
		CStr GetSectionName(void) const
		{
			return m_strSection;
		}

		/**
		 * [�m�F] �Q�Ɖ\�H
		 *		���̃Z�N�V���������݂��ǂݍ��߂�A�m�F�ł��܂��B
		 *	@retval true ���݁B
		 *	@retval false �Ȃ��B
		 */
		bool CanQuery(void) const
		{
			CStr s = m_strSection;
			INT_PTR d = s.ReverseFind('\\');
			if ( d >= 0 )
			{
				s = s.Left(d);
			}
			CStrVector vs = m_piAccessor->EnumSectionNames();
			return vs.Find(s) != INVALID_INDEX;
		}

		/**
		 * [�ݒ�] �ΏۃZ�N�V�����ύX.
		 *		�w��̃T�u�Z�N�V������ǉ����A�擾�Ώۂ����̃Z�N�V�������ɂ��܂��B
		 *	@param lpszSubName �T�u�Z�N�V�������B
		 *	@retval true ����
		 *	@retval false ���s�i���O�Ɂ����܂܂�Ă���A�Ȃǁj
		 */
		bool InSubSection(LPCTSTR lpszSubName)
		{
			if ( STRLIB::GetLen(lpszSubName) > 0 && STRLIB::IndexOf(lpszSubName, '\\') < 0 )
			{
				if ( ! m_strSection.IsEmpty() )
				{
					m_strSection += _T('\\');
				}
				m_strSection += lpszSubName;
				return true;
			}
			return false;
		}

		/**
		 * [�ݒ�] �ΏۃZ�N�V�����ύX.
		 *		�擾�Ώۂ����̃Z�N�V�������ɂɂ��܂��B
		 *	@note IsSubSection() �Ƒ΂ɂȂ�悤�Ɏg�p���܂��B
		 */
		void PreviousSubSection(void)
		{
			INT_PTR d = m_strSection.ReverseFind('\\');
			if ( d >= 0 )
			{
				m_strSection = m_strSection.Left(d);
			}
			else
			{
				m_strSection.Empty();
			}
		}

		/**
		 * [�Q��] Accessor�C���^�[�t�F�[�X�Q��
		 *	@return IAccessor
		 */
		const IAccessor* ReferAccessor(void) const
		{
			return m_piAccessor;
		}

		/**
		 * [�擾] �Z�N�V�������ꗗ�擾
		 *	@return �Z�N�V�������ꗗ
		 */
		CStrVector EnumSectionNames(void) const
		{
			return m_piAccessor->EnumSectionNames(m_strSection);
		}

		/**
		 * [�擾] �L�[���ꗗ�擾
		 *	@return �L�[���ꗗ
		 */
		CStrVector EnumKeyNames(void) const
		{
			return m_piAccessor->EnumKeyNames(m_strSection);
		}

		/**
		 * [�폜] �S�L�[�폜
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool DeleteAllKeys(void)
		{
			return m_piAccessor->DeleteSection(m_strSection);
		}

		/**
		 * [�擾] �����擾
		 *	@param lpszKey �L�[��
		 *	@return ����B�Ȃ��ꍇ�AEK_Nothing �ɂȂ�܂��B
		 */
		EKind GetKeyKind(LPCTSTR lpszKey) const
		{
			return m_piAccessor->GetKeyKind(m_strSection, lpszKey);
		}

		/**
		 * [�m�F] ��񑶍݊m�F
		 *	@param lpszKey �L�[��
		 *	@retval true ��񂠂�B
		 *	@retval false ��񖳂��B
		 */
		bool HasKey(LPCTSTR lpszKey) const
		{
			return GetKeyKind(lpszKey) != EK_Nothing;
		}

		/**
		 * [�폜] �w��L�[�폜
		 *	@param lpszKey �L�[���B
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool DeleteKey(LPCTSTR lpszKey)
		{
			return m_piAccessor->WriteValue(m_strSection, lpszKey, CValue());
		}

		/**
		 * [�ǉ�] ���L�^
		 *	@param lpszKey �L�[��
		 *	@param value �L�[�̒l
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteValue(LPCTSTR lpszKey, const CValue& value)
		{
			return m_piAccessor->WriteValue(m_strSection, lpszKey, value);
		}

		/**
		 * [�擾] ���擾
		 *	@param lpszKey �L�[��
		 *	@return �L�[�̒l
		 */
		CValue QueryValue(LPCTSTR lpszKey) const
		{
			return m_piAccessor->QueryValue(m_strSection, lpszKey);
		}
		
		/**
		 * [�ǉ�] ��������L�^
		 *	@param lpszKey �L�[��
		 *	@param lpszValue �L�[�̒l
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteString(LPCTSTR lpszKey, LPCTSTR lpszValue)
		{
			return WriteValue(lpszKey, CValue(lpszValue));
		}

		/**
		 * [�擾] ��������擾
		 *	@param lpszKey �L�[��
		 *	@param lpszDefault �L�[���Ȃ������̒l��Ԃ��܂��B�ȗ������ Empty������ł��B
		 *	@return �L�[�̒l
		 */
		CStr QueryString(LPCTSTR lpszKey, LPCTSTR lpszDefault = NULL) const
		{
			const CValue& v = QueryValue(lpszKey);
			return v.IsNull() ? lpszDefault : v.QueryString();
		}

		/**
		 * [�ǉ�] ������Q���L�^
		 *	@param lpszKey �L�[��
		 *	@param vs �L�[�̒l
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WritePluralStrings(LPCTSTR lpszKey, const CStrVector& vs)
		{
			return WriteValue(lpszKey, CValue(vs));
		}

		/**
		 * [�擾] ������Q���擾
		 *	@param lpszKey �L�[��
		 *	@return �L�[�̒l
		 */
		CStrVector QueryPluralStrings(LPCTSTR lpszKey) const
		{
			return QueryValue(lpszKey).QueryPluralStrings();
		}

		/**
		 * [�ǉ�] �o�C�i�����L�^
		 *	@param lpszKey �L�[��
		 *	@param c �L�[�̒l
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteData(LPCTSTR lpszKey, const IConstCollectionT<BYTE>& c)
		{
			return WriteValue(lpszKey, CValue(c));
		}

		/**
		 * [�ǉ�] �o�C�i�����L�^
		 *	@param lpszKey �L�[��
		 *	@param len �L�[�̒l(�f�[�^�T�C�Y)
		 *	@param P �L�[�̒l(�f�[�^)
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteData(LPCTSTR lpszKey, size_t len, LPCVOID P)
		{
			return WriteValue(lpszKey, CValue(len, P));
		}

		/**
		 * [�擾] �o�C�i�����擾
		 *	@param lpszKey �L�[��
		 *	@return �L�[�̒l
		 */
		CByteVector QueryData(LPCTSTR lpszKey) const
		{
			return QueryValue(lpszKey).QueryData();
		}

		/**
		 * [�ǉ�] ���l���L�^
		 *	@param lpszKey �L�[��
		 *	@param dwValue �L�[�̒l
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteDword(LPCTSTR lpszKey, DWORD dwValue)
		{
			return WriteValue(lpszKey, CValue(dwValue));
		}

		/**
		 * [�擾] ���l���擾
		 *	@param lpszKey �L�[��
		 *	@param dwDefault �L�[���Ȃ������̒l��Ԃ��܂��B�ȗ�����ƂO�ł��B
		 *	@return �L�[�̒l
		 */
		DWORD QueryDword(LPCTSTR lpszKey, DWORD dwDefault = 0) const
		{
			const CValue& v = QueryValue(lpszKey);
			return m_IsEmpty(v) ? dwDefault : v.QueryDword();
		}

		/**
		 * [�ǉ�] ���l���L�^
		 *	@param lpszKey �L�[��
		 *	@param llValue �L�[�̒l
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteLonglong(LPCTSTR lpszKey, LONGLONG llValue)
		{
			return WriteValue(lpszKey, CValue(llValue));
		}

		/**
		 * [�擾] ���l���擾
		 *	@param lpszKey �L�[��
		 *	@param llDefault �L�[���Ȃ������̒l��Ԃ��܂��B�ȗ�����ƂO�ł��B
		 *	@return �L�[�̒l
		 */
		LONGLONG QueryLonglong(LPCTSTR lpszKey, LONGLONG llDefault = 0) const
		{
			const CValue& v = QueryValue(lpszKey);
			return m_IsEmpty(v) ? llDefault : v.QueryLonglong();
		}

		/**
		 * [�ǉ�] �o�C�i�����L�^
		 *	@note �\���̂̃������C���[�W���L�^���܂��B
		 *	@attention �\���̂́A Plain Old Data (POD) �^�ł���K�v������܂��B
		 *	@param lpszKey �L�[��
		 *	@param t �L�[�̒l
		 *	@retval true ����
		 *	@retval false ���s
		 */
		template<typename TYP>
		bool WriteStruct(LPCTSTR lpszKey, const TYP& t)
		{
			CByteVector vb;
			vb.SetElements(sizeof(TYP), reinterpret_cast<const BYTE*>(&t));
			return WriteData(lpszKey, vb);
		}

		/**
		 * [�擾] �o�C�i�����擾
		 *	@note �\���̂̃������C���[�W���擾���܂��B
		 *	@attention �\���̂́A Plain Old Data (POD) �^�ł���K�v������܂��B
		 *	@param[in] lpszKey �L�[��
		 *	@param[out] _t �L�[�̒l
		 *	@retval true �����B
		 *	@retval false ���s�B���T�C�Y���قȂ�B
		 */
		template<typename TYP>
		bool QueryStruct(LPCTSTR lpszKey, TYP& _t) const
		{
			const CByteVector& v = QueryData(lpszKey);
			if ( v.GetSize() != sizeof(TYP) )
			{
				return false;
			}
			BYTE* B = reinterpret_cast<BYTE*>(&_t);
			MemCopy(B, v.ReferBuffer(), sizeof(TYP));
			return true;
		}

		/**
		 * [�ǉ�] �z��l�L�^
		 *	@note �z����L�^���܂��B�Z�N�V�����ȉ�����x�������A�z�񐔂ƒl���L�^���܂��B
		 *	@param vv �z��
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteVector(const CVectorT<CValue>& vv)
		{
			bool r = true;
			r &= m_piAccessor->DeleteSection(m_strSection);
			size_t len = vv.GetSize();
			r &= WriteLonglong(_T("size"), len);
			loop ( i, len )
			{
				r &= WriteValue(CStr::Fmt(_T("%d"), i + 1), vv[i]);
			}
			return r;
		}

		/**
		 * [�擾] �z��l�擾
		 *	@note WriteVector() �ɂ�鏑�����񂾔z��l���擾���܂��B
		 *	@return �z��
		 */
		CVectorT<CValue> QueryVector(void) const
		{
			CVectorT<CValue> vv;
			size_t len = QueryDword(_T("size"), 0);
			vv.SetSize(len);
			loop ( i, len )
			{
				vv[i] = QueryValue(CStr::Fmt(_T("%d"), i + 1));
			}
			return vv;
		}

		/**
		 * [�ǉ�] �V���A���C�U�u���f�[�^���L�^
		 *	@param lpszKey �L�[��
		 *	@param s �V���A���C�Y�\�̃f�[�^
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteSerializableData(LPCTSTR lpszKey, const ISerializable& s)
		{
			CByteVector vb;
			try
			{
				CSerializeAdapter sa(&vb);
				sa << s;
				return WriteData(lpszKey, vb);
			}
			catch ( ... )
			{
				return false;
			}
			return false;
		}

		/**
		 * [�擾] �V���A���C�U�u���f�[�^���擾
		 *	@param[in] lpszKey �L�[��
		 *	@param[out] _d �f�V���A���C�Y�������ʂ��i�[����܂��B
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool QuerySerializableData(LPCTSTR lpszKey, ISerializable& _d) const
		{
			CByteVector vb = QueryData(lpszKey);
			if ( ! vb.IsEmpty() )
			{
				try
				{
					CDeserializeAdapter da(&vb);
					da >> _d;
					return true;
				}
				catch ( ... )
				{
					return false;
				}
			}
			return false;
		}

		#ifdef __AFX_H__
		
		/**
		 * [�ǉ�] �E�B���h�E�e�L�X�g���L�^
		 *	@param lpszKey �L�[��
		 *	@param hWnd �E�B���h�E�n���h��
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool WriteWindowText(LPCTSTR lpszKey, HWND hWnd)
		{
			CStr s;
			int l = ::GetWindowTextLength(hWnd);
			if ( l > 0 )
			{
				::GetWindowText(hWnd, s.GetBuffer(l + 2), l + 1);
				s.ReleaseBuffer();
				return WriteString(lpszKey, s);
			}
			return DeleteKey(lpszKey);
		}

		/**
		 * [�擾] �E�B���h�E�e�L�X�g���擾
		 *	@param lpszKey �L�[��
		 *	@param hWnd �E�B���h�E�n���h��
		 *	@param lpszDefault �L�[���Ȃ������̒l��Ԃ��܂��B�ȗ������ Empty������ł��B
		 *	@retval true �����B
		 *	@retval false ���s�B
		 */
		bool QueryWindowText(LPCTSTR lpszKey, HWND hWnd, LPCTSTR lpszDefault = NULL) const
		{
			CStr s = QueryString(lpszKey, lpszDefault);
			return !! ::SetWindowText(hWnd, s);
		}

		#endif

		#ifdef _TnbACCESSOR_Tree_ENABLE

		/**
		 * [�擾] �S�l�擾
		 *	@note �S�L�[�ƒl���}�b�v�Ɋi�[���܂��B
		 *	@return �}�b�v
		 */
		CKeyMap GetKeyMap(void) const
		{
			CKeyMap map;
			const CStrVector& vstr = EnumKeyNames();
			loop ( i, vstr.GetSize() )
			{
				map[vstr[i]] = QueryValue(vstr[i]);
			}
			return map;
		}

		/**
		 * [�擾] �S�l�擾
		 *	@note �S�L�[�ƒl�A����ɃT�u�Z�N�V�����ȉ���Tree�Ɋi�[���܂��B
		 *	@param[out] _tree �c���[
		 */
		void GetKeyTree(CKeyTree& _tree) const
		{
			const CStrVector& vs = EnumSectionNames();
			loop ( i, vs.GetSize() )
			{
				CSection s = *this;
				s.InSubSection(vs[i]);
				s.GetKeyTree(_tree.Refer(vs[i]));
				s.PreviousSubSection();
			}
			const CKeyMap& mapKey = GetKeyMap();
			CStr key;
			CValue value;
			loop ( i, mapKey.GetSize() )
			{
				mapKey.Get(i, key, value);
				if ( key.IsEmpty() )
				{
					_tree.AtSelf() = value;
				}
				else
				{
					_tree.Add(key, value);
				}
			}
		}

		/**
		 * [�擾] �S�l�擾
		 *	@note �S�L�[�ƒl�A����ɃT�u�Z�N�V�����ȉ���Tree�Ɋi�[���܂��B
		 *			�l�͂��ׂĕ�������ɕϊ����܂��B
		 *	@param[out] _tree �c���[
		 */
		void GetKeyTree(CStrsTree& _tree) const
		{
			const CStrVector& vs = EnumSectionNames();
			loop ( i, vs.GetSize() )
			{
				CSection s = *this;
				s.InSubSection(vs[i]);
				s.GetKeyTree(_tree.Refer(vs[i]));
				s.PreviousSubSection();
			}
			const CKeyMap& mapKey = GetKeyMap();
			CStr key;
			CValue value;
			loop ( i, mapKey.GetSize() )
			{
				mapKey.Get(i, key, value);
				if ( key.IsEmpty() )
				{
					_tree.AtSelf() = value.QueryString();
				}
				else
				{
					_tree.Add(key, value.QueryString());
				}
			}
		}

		#endif
	};

	/// �f�X�g���N�^
	virtual ~IAccessor(void)
	{
	}

	/**
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const = 0;

	/**
	 * [����] �t���b�V��.
	 *	@note Write���������m�肳���܂��iSub�N���X�ɂ�蓮�삪���ƂȂ�܂��B�j�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool Flush(void) = 0;

	/**
	 * [�擾] �Z�N�V�������ꗗ�擾
	 *	@param lpszSectionName �Z�N�V�������B NULL���w�肷��ƁAROOT�̃Z�N�V�����ꗗ�������܂��B
	 *	@return �Z�N�V�������ꗗ
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const = 0;

	/**
	 * [�폜] �w��Z�N�V�����폜
	 *	@param lpszSectionName �Z�N�V�������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName) = 0;

	/**
	 * [�擾] �L�[���ꗗ�擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return ���O�ꗗ�B������ԂȂ疢�Ή��B
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const = 0;

	/**
	 * [�擾] �����擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return ����B�L�[�����݂��Ȃ��ꍇ�AEK_Nothing �ɂȂ�܂��B
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const = 0;

	/**
	 * [�擾] ���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const = 0;

	/**
	 * [�ݒ�] ���ݒ�
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@param value �ݒ肷��l�BEK_Nothing �̏ꍇ�A�L�[���폜���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value) = 0;
	
	/**
	 * [�ϊ�] �z��ϊ�.
	 *		IAccessor::CValue �̔z��ɕϊ����܂��B
	 *	@param vt �z��
	 *	@return IAccessor::CValue �̔z��
	 */
	template<typename TYP>
	static CVectorT<IAccessor::CValue> ToValueVector(const CVectorT<TYP>& vt)
	{
		CVectorT<IAccessor::CValue> vv;
		vv.SetSize(vt.GetSize());
		loop ( i, vt.GetSize() )
		{
			vv[i] = CValue(vt[i]);
		}
		return vv;
	}
};



/**@ingroup ACCESSOR
 * ���A�N�Z�X���ۃN���X.
 *
 *		�Z�N�V�����ŋ�؂�ꂽ���ɕ����̃L�[�ƒl�����`�̏����Ǘ����钊�ۃN���X�ł��B
 *
 *	@see IAccessor
 *
 *	@par�K�v�t�@�C��
 *			TnbAccessor.h
 *
 *	@date 08/04/16 IAccessor ���番���B
 */
class CAbstractAccessor : public IAccessor
{
	DEFSUPER(IAccessor);
public:

	/**
	 * [�擾] CSection�擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return �w��̃Z�N�V�������A�N�Z�X���� CSection
	 */
	CSection operator[](LPCTSTR lpszSectionName)
	{
		return CSection(this, lpszSectionName);
	}

	/**
	 * [�擾] CSection�擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return �w��̃Z�N�V�������A�N�Z�X���� CSection
	 */
	const CSection operator[](LPCTSTR lpszSectionName) const
	{
		return CSection(this, lpszSectionName);
	}

	/**
	 * [����] �t���b�V��.
	 *	@note ��ɐ������܂��B�{�N���X�ł͉������܂���B
	 *	@return true ����
	 */
	virtual bool Flush(void)
	{
		return true; 
	}

	/**
	 * [�폜] �S�Z�N�V�����폜
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool DeleteAllSection(void)
	{
		bool r = true;
		CStrVector vs = EnumSectionNames(NULL);
		loop ( i, vs.GetSize() )
		{
			r &= DeleteSection(vs[i]);
		}
		return r;
	}

	#ifdef _TnbACCESSOR_Tree_ENABLE

	/**
	 * [�擾] �S�l�擾
	 *	@note �S�L�[�ƒl�A����ɃT�u�Z�N�V�����ȉ���Tree�Ɋi�[���܂��B
	 *	@param[out] _tree �c���[
	 */
	void GetKeyTree(CKeyTree& _tree) const
	{
		const CStrVector& vstr = EnumSectionNames();
		CSection(this, _T("")).GetKeyTree(_tree);
		loop ( i, vstr.GetSize() )
		{
			CSection(this, vstr[i]).GetKeyTree(_tree.Refer(vstr[i]));
		}
	}

	/**
	 * [�擾] �S�l�擾
	 *	@note �S�L�[�ƒl�A����ɃT�u�Z�N�V�����ȉ���Tree�Ɋi�[���܂��B
	 *			�l�͂��ׂĕ�������ɕϊ����܂��B
	 *	@param[out] _tree �c���[
	 */
	void GetKeyTree(CStrsTree& _tree) const
	{
		const CStrVector& vstr = EnumSectionNames();
		CSection(this, _T("")).GetKeyTree(_tree);
		loop ( i, vstr.GetSize() )
		{
			CSection(this, vstr[i]).GetKeyTree(_tree.Refer(vstr[i]));
		}
	}

	#endif

	/**
	 * [�쐬] ��`�t�@�C�����쐬.
	 *		���s�t�@�C������ .exe���Ȃ����t���p�X��Ԃ��܂��B�B
	 *	@return �t���p�X
	 */
	static CStr MakeDefineFilePath(void)
	{
		CStr str;
		DWORD dwRc = ::GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);
		str.ReleaseBuffer();
		ASSERTLIB(dwRc != 0);
		IgnoreUnusedValue(dwRc);
		//
		CStr strWork;
		strWork = str.Left(str.ReverseFind('.'));
		#if defined(_DEBUG) && !defined(_WIN32_WCE)
			strWork += _T(".");
			CStr s;
			DWORD dwLength = MAX_COMPUTERNAME_LENGTH + 1;
			::GetComputerName(s.GetBuffer(dwLength), &dwLength);
			s.ReleaseBuffer();
			strWork += s;
		#endif
		return strWork;
	}
};



/**@ingroup ACCESSOR
 * �f�B�[�v�����N���X
 *
 *		�Z�N�V��������[���@�荞��Ō�������N���X�ł��B
 *		�{�N���X���p�����A OnFindSection() �ɏ������������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbAccessor.h
 *
 *	@date 10/02/17 �V�K�쐬
 */
class CDeepAccessFinder
{
public:

	/// �R���X�g���N�^
	CDeepAccessFinder(void) : m_depth(0)
	{
	}

	/**
	 * [���s] �����J�n.
	 *	@param section ��������Z�N�V�������w��.
	 */
	void Execute(IAccessor::CSection& section)
	{
		IAccessor::CSection sec = section;
		m_depth = 0;
		m_Find(sec);
	}

protected:

	/// ����
	enum EResult
	{
		Next,		///< ������
		Previous,	///< ���̊K�w��
		Stop,		///< ������~
	};

	/**
	 * [�ʒm] �Z�N�V��������.
	 *		�Z�N�V�����𔭌��������ɒʒm����܂��B
	 *	@note �{���\�b�h�� Next ��Ԃ��܂��B
	 *	@param lpszSection �����Z�N�V������.
	 *	@param sec �����Z�N�V�����A�N�Z�X�N���X.
	 *	@param depth �[��
	 *	@return ����
	 */
	virtual EResult OnFindSection(LPCTSTR lpszSection, IAccessor::CSection& sec, int depth)
	{
		return Next;
	}

private:
	/// ����
	EResult m_Find(IAccessor::CSection& sec)
	{
		m_depth++;
		CStrVector vs = sec.EnumSectionNames();
		loop ( i, vs )
		{
			IAccessor::CSection s = sec;
			if ( s.InSubSection(vs[i]) )
			{
				EResult r = OnFindSection(vs[i], s, m_depth);
				if ( r == Next )
				{
					r = m_Find(s);
				}
				if ( r == Previous )
				{
					break;
				}
				if ( r != Next )
				{
					m_depth--;
					return Stop;
				}
			}
		}
		m_depth--;
		return Next;
	}
	int		m_depth;		///< �[��
};



#ifndef _WIN32_WCE

/**@ingroup ACCESSOR
 * �E�B���h�E�̏�Ԃ�ۑ�.
 *		�w��� CSection �ɏ�Ԃ�ۑ����܂��B
 *		(���݂̓T�C�Y�ƈʒu�̂ݑΉ�)
 *
 *	@note	�ǂݏo���� RestoreWindowPlacement ���g���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbAccessor.h
 *
 *	@param[out] _sec IAccessor �̃T�u�N���X���瓾�� CSection �̎Q�ƁB
 *	@param[in] hWnd ��Ԃ�ۑ�����ΏہB
 *	@retval true ����
 *	@retval false ���s
 */
inline bool StoreWindowPlacement(IAccessor::CSection& _sec, HWND hWnd)
{
	bool boRc = true;
	WINDOWPLACEMENT t = { 0 };
	t.length = sizeof(WINDOWPLACEMENT);
	if ( ! ::GetWindowPlacement(hWnd, &t) ){ return false; }
	boRc &= _sec.WriteDword(_T("Valid"), 1);
	boRc &= _sec.WriteLonglong(_T("SX"), t.rcNormalPosition.left);
	boRc &= _sec.WriteLonglong(_T("SY"), t.rcNormalPosition.top);
	boRc &= _sec.WriteLonglong(_T("EX"), t.rcNormalPosition.right);
	boRc &= _sec.WriteLonglong(_T("EY"), t.rcNormalPosition.bottom);
	boRc &= _sec.WriteDword(_T("Show"), ::IsWindowVisible(hWnd));
	boRc &= _sec.WriteDword(_T("Maximize"), ::IsZoomed(hWnd));
	return boRc;
}



/**@ingroup ACCESSOR
 * �E�B���h�E�̏�Ԃ𕜌�.
 *		�w��� CSection �����Ԃ𕜌����܂��B
 *		(���݂̓T�C�Y�ƈʒu�̂ݑΉ�)
 *
 *	@note	�����o���� StoreWindowPlacement ���g���܂��B
 *	@note	��񂪂Ȃ��ꍇ�A�������܂���̂ŁA�{�֐����R�[���̑O��hWnd �ɑ΂�
 *			�f�t�H���g�̐ݒ���s���Ă����悤�ɂ��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbAccessor.h
 *
 *	@param[in] sec IAccessor �̃T�u�N���X���瓾�� CSection �̎Q�ƁB
 *	@param hWnd ��Ԃ𕜌�����ΏہB
 *	@param withWindowSize true �Ȃ� WindowSize�����X�g�A�Ώۂɂ���B
 *	@param withShowHide true �Ȃ� Show/Hide �����X�g�A�Ώ̂ɂ���B
 *	@retval true �ݒ�
 *	@retval false ������
 */
inline bool RestoreWindowPlacement(const IAccessor::CSection& sec, HWND hWnd, bool withWindowSize = true, bool withShowHide = false)
{
	if ( sec.QueryDword(_T("Valid"), 0) == 0 )
	{
		return false;
	}
	WINDOWPLACEMENT t = { 0 };
	t.length = sizeof(WINDOWPLACEMENT);
	if ( ! ::GetWindowPlacement(hWnd, &t) )
	{
		return false;
	}
	int width = t.rcNormalPosition.right - t.rcNormalPosition.left;
	int height = t.rcNormalPosition.bottom	- t.rcNormalPosition.top;
	t.rcNormalPosition.left		= static_cast<LONG>(sec.QueryLonglong(_T("SX"), 0));
	t.rcNormalPosition.top		= static_cast<LONG>(sec.QueryLonglong(_T("SY"), 0));
	if ( withWindowSize )
	{
		t.rcNormalPosition.right	= static_cast<LONG>(sec.QueryLonglong(_T("EX"), 100));
		t.rcNormalPosition.bottom	= static_cast<LONG>(sec.QueryLonglong(_T("EY"), 100));
	}
	else
	{
		t.rcNormalPosition.right = t.rcNormalPosition.left + width;
		t.rcNormalPosition.bottom = t.rcNormalPosition.top + height;
	}
	//
	DWORD cx = t.rcNormalPosition.right - t.rcNormalPosition.left;
	DWORD cy = t.rcNormalPosition.bottom - t.rcNormalPosition.top;
	if ( cx > 4000 || cy > 4000 )
	{
		return false;
	}
	t.flags		= WPF_SETMINPOSITION;
	t.showCmd	= SW_HIDE;
	::SetWindowPlacement(hWnd, &t);
	if ( withShowHide )
	{
		DWORD s = sec.QueryDword(_T("Show"), TRUE);
		::ShowWindow(hWnd, (s != 0) ? SW_SHOW : SW_HIDE);
	}
	if ( !! sec.QueryDword(_T("Maximize"), FALSE) )
	{
		::ShowWindow(hWnd, SW_MAXIMIZE);
	}
	return true;
}

#endif //_WIN32_WCE



}; // TNB
