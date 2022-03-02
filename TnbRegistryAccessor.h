#pragma once
/**
 *@file
 * ���W�X�g���A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAccessor.h"
#include "TnbDntStr.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #ifndef REG_QWORD
  #define REG_QWORD (11) // 64-bit number
 #endif
#endif



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
/// PointerHandleBase�pDestory�N���X
struct TPhRegCloseKey
{
	void operator()(HKEY P) { ::RegCloseKey(P); }
};
#endif



/**@ingroup ACCESSOR
 * HKEY�^�n���h���e���v���[�g
 *
 *		���̌^�� RegOpenKeyEx���ARegCloseKey() �Ŕj������n���h����n���Ă����ƁA
 *		�u�̈�v���Q�Ƃ���{�N���X�����Ȃ��Ȃ莟��A
 *		�����I�� RegCloseKey ���܂��B
 *
 *	@see http://www.microsoft.com/japan/developer/library/jpwinpf/_win32_RegCloseKey.htm
 *
 *	@par�K�v�t�@�C��
 *			TnbRegistryAccessor.h
 */
typedef TNB::CPointerHandleBaseT<HKEY, TPhRegCloseKey> CRegKeyHandle;



/**@ingroup ACCESSOR
 * ���W�X�g���A�N�Z�X�N���X
 *	
 *		���W�X�g����ǂݏ������邽�߂̃N���X�ł��B
 *
 *		�{�N���X�ł́A�T�u�L�[���u�Z�N�V�����v�ƌĂ�ł��܂��B
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
 *			TnbRegistryAccessor.h
 * 
 *	@date 06/05/20	�V�K�쐬
 *	@date 06/06/08 �����o���ύX�B
 *	@date 06/10/31 IInformation �̉��C�ɑΉ��B
 *	@date 07/05/10 �N���X���ύX�B
 *	@date 08/04/15 �������݌n�C���B
 *	@date 08/10/15 SHLWAPI.dll(SHDeleteKey()) ���g��Ȃ��悤�ɕύX�B
 *	@date 09/12/01 IsValid() �V�K�B�\�������B
 */
class CRegistryAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
public:

	/**
	 * �R���X�g���N�^ 
	 *	@param hhParentKey �Ώۂ̐e�n���h���B�ʏ�AHKEY_LOCAL_MACHINE�Ȃǂ̃V���{�����w�肵�܂��B
	 *	@param dwRegSam �A�N�Z�X���B�ȗ������ KEY_ALL_ACCESS �ɂȂ�܂��B ���A KEY_READ �Ȃǂ�����܂��B
	 */
	explicit CRegistryAccessor(const CRegKeyHandle& hhParentKey, REGSAM dwRegSam = KEY_ALL_ACCESS) 
		: m_hhParentKey(hhParentKey), m_dwRegSam(dwRegSam)
	{
	}

	/**
	 * �R���X�g���N�^ 
	 *	@param hhParentKey �Ώۂ̐e�n���h���B�ʏ�AHKEY_LOCAL_MACHINE�Ȃǂ̃V���{�����w�肵�܂��B
	 *	@param lpszSectionName �Z�N�V�������B
	 *	@param dwRegSam �A�N�Z�X���B�ȗ������ KEY_ALL_ACCESS �ɂȂ�܂��B ���A KEY_READ �Ȃǂ�����܂��B
	 */
	CRegistryAccessor(const CRegKeyHandle& hhParentKey, LPCTSTR lpszSectionName, REGSAM dwRegSam = KEY_ALL_ACCESS) 
	{
		CRegistryAccessor rg(hhParentKey, dwRegSam);
		m_hhParentKey = rg.GetHandle(lpszSectionName);
		m_dwRegSam = dwRegSam;
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CRegistryAccessor(void) 
	{
	}

	/**
	 * [�ݒ�] �x�[�X�w��
	 *	@par�Ώ�
	 *			�S��
	 *	@param hhParentKey �Ώۂ̐e�n���h���B�ʏ�AHKEY_LOCAL_MACHINE�Ȃǂ̃V���{�����w�肵�܂��B
	 */
	void SetBase(const CRegKeyHandle& hhParentKey)
	{
		m_hhParentKey = hhParentKey;
		m_strSection.Empty();
		m_hhKey.Null();
	}

	/**
	 * [�擾] �e�n���h���擾
	 *	@return �Ώۃn���h��
	 */
	const CRegKeyHandle& GetBase(void) const
	{
		return m_hhParentKey;
	}

	/**
	 * [�擾] �L���m�F.
	 *	@retval true �L���B
	 *	@retval false �����B
	 */
	bool IsValid(void) const
	{
		HKEY h = m_hhParentKey;
		return h != NULL;
	}

	/**
	 * [�擾] �Ώۃn���h���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@return �Ώۃn���h��
	 */
	const CRegKeyHandle& GetHandle(LPCTSTR lpszSectionName) const
	{
		m_SetSectionName(lpszSectionName);
		m_Create();
		return m_hhKey;
	}

	/**
	 * [�擾] �^�C�v���擾
	 *	@return �^�C�v��
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Registry");
	}

	/**
	 * [����] �t���b�V��.
	 *	@note ��������ɃL���b�V��������Ώ����o���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool Flush(void)
	{
		if ( m_hhKey.IsNull() )
		{
			return false;
		}
		return ::RegFlushKey(m_hhKey) == ERROR_SUCCESS; 
	}

	/**
	 * [�擾] �Z�N�V�������ꗗ�擾
	 *	@param lpszSectionName �Z�N�V�������B NULL���w�肷��ƁAROOT�̃Z�N�V�����ꗗ�������܂��B
	 *	@return �Z�N�V�������ꗗ
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const
	{
		if ( lpszSectionName == NULL || *lpszSectionName == 0 )
		{
			return m_EnumSections(m_hhParentKey);
		}
		m_SetSectionName(lpszSectionName);
		if ( m_Open() != 0 )
		{
			return CStrVector(); 
		}
		return m_EnumSections(m_hhKey);
	}

	/**
	 * [�폜] �w��Z�N�V�����폜
	 *	@param lpszSectionName �Z�N�V�������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		m_Close();
		DWORD r = ms_RegDeleteKeyNT(m_hhParentKey, lpszSectionName);
		::SetLastError(r);
		return r == ERROR_SUCCESS;
	}

	/**
	 * [�擾] ���O�ꗗ�擾
	 *	@note �{�N���X�ł� pure�֐��ł��B
	 *	@param lpszSectionName �Z�N�V������
	 *	@return ���O�ꗗ�B������ԂȂ疢�Ή��B
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		m_SetSectionName(lpszSectionName);
		CStrVector vstrRc;
		if ( m_Open() != 0 )
		{
			return vstrRc; 
		}
		LONG lRc;
		DWORD dwIndex = 0;
		CStr strName;
		DWORD dwNameLength;
		while ( true )
		{
			dwNameLength = E_BufferSize;
			lRc = ::RegEnumValue(
						m_hhKey,			// �₢���킹�Ώۂ̃L�[�̃n���h��
						dwIndex,			// �擾����ׂ����W�X�g���G���g���̃C���f�b�N�X�ԍ�
						strName.GetBuffer(dwNameLength),
											// ���W�X�g���G���g�������i�[�����o�b�t�@
						&dwNameLength,		// ���W�X�g���G���g�����o�b�t�@�̃T�C�Y
						0,					// �\��ς�
						NULL,				// ���W�X�g���G���g���̃f�[�^�̃^�C�v
						NULL,				// ���W�X�g���G���g���̃f�[�^���i�[�����o�b�t�@
						NULL				// �f�[�^�o�b�t�@�̃T�C�Y
			);
			strName.ReleaseBuffer();
			if ( lRc == ERROR_NO_MORE_ITEMS )
			{
				break;
			}	
			if ( lRc != ERROR_SUCCESS )
			{
				vstrRc.Invalid();
				return vstrRc;
			}
			vstrRc.Add(strName);
			dwIndex++;
		}
		return vstrRc;
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
		if ( m_Open() != 0 )
		{
			return EK_Nothing; 
		}
		DWORD dwType;
		DWORD dwLength;
		LONG lRc = ::RegQueryValueEx(
						m_hhKey,			// �L�[�̃n���h��
						lpszKey,			// ���W�X�g���G���g����
						NULL,				// �\��ς�
						&dwType,			// �f�[�^�^���i�[�����o�b�t�@
						NULL,				// �f�[�^���i�[�����o�b�t�@
						&dwLength			// �f�[�^�o�b�t�@�̃T�C�Y
		);
		EKind eKind = EK_Nothing;
		if ( lRc == ERROR_SUCCESS )
		{
			switch ( dwType )
			{
			case REG_QWORD:// 64 �r�b�g�l
				eKind = EK_Longlong;
				break;
			case REG_DWORD:// 32 �r�b�g�l
				eKind = EK_Dword;
				break;
			case REG_MULTI_SZ:// NULL �ŏI��镡���̕����񂩂�Ȃ� 1 �̔z��
				eKind = EK_PluralStrings;
				break;
 			case REG_EXPAND_SZ:// ���ϐ��̓W�J�O�̕\�L�i"%PATH%" �Ȃǁj��ێ����Ă���ANULL �ŏI��镶����
				eKind = EK_String;
				break;
			case REG_SZ:// NULL �ŏI��镶����
				eKind = EK_String;
				break;
			case REG_BINARY:// Free form binary
				eKind = EK_Binary;
				break;
			}
		}
		return eKind;
	}

	/**
	 * [�擾] ���擾
	 *	@param lpszSectionName �Z�N�V������
	 *	@param lpszKey �L�[��
	 *	@return �L�[�̒l
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		CValue v;
		DWORD dwGetType;
		CByteVector vb;
		switch ( GetKeyKind(lpszSectionName, lpszKey) )
		{
		case EK_Nothing://���݂��Ȃ�
			break;
		case EK_String://������
			vb = m_Query(lpszKey, REG_SZ, REG_EXPAND_SZ, &dwGetType);
			if ( vb.IsValid() )
			{
				CStr str = reinterpret_cast<LPCTSTR>(vb.ReferBuffer());
				#ifndef _WIN32_WCE
					if ( dwGetType == REG_EXPAND_SZ )
					{
						// ���ϐ��̓W�J�O�̕\�L�i"%PATH%" �Ȃǁj��ێ����Ă���ANULL �ŏI��镶����
						DWORD l = ::ExpandEnvironmentStrings(str, NULL, 0);
						if ( l > 0 )
						{
							CStr s;
							::ExpandEnvironmentStrings(str, s.GetBuffer(l + 1), l + 1);
							s.ReleaseBuffer();
							str = s;
						}
					}
				#endif
				v = CValue(str);
			}
			break;
		case EK_PluralStrings://�����̕�����Q
			vb = m_Query(lpszKey, REG_MULTI_SZ, REG_MULTI_SZ);
			if ( vb.IsValid() )
			{
				CDoubleNullTerminateStr dntstr;
				dntstr.SetDntStr(reinterpret_cast<LPCTSTR>(vb.ReferBuffer()));
				v = CValue(dntstr.ToStrVector());
			}
			break;
		case EK_Binary://�o�C�i��
			vb = m_Query(lpszKey, REG_BINARY, REG_BINARY);
			if ( vb.IsValid() )
			{
				v = CValue(vb);
			}			
			break;
		case EK_Dword://32Bit Unsigned Integer
			vb = m_Query(lpszKey, REG_DWORD, REG_DWORD);
			if ( vb.IsValid() )
			{
				ASSERTLIB(sizeof(DWORD) == vb.GetSize());
				v = CValue(*(reinterpret_cast<const DWORD*>(vb.ReferBuffer())));
			}
			break;
		case EK_Longlong://64Bit Signed Integer
			vb = m_Query(lpszKey, REG_QWORD, REG_QWORD);
			if ( vb.IsValid() )
			{
				ASSERTLIB(sizeof(LONGLONG) == vb.GetSize());
				v = CValue(*(reinterpret_cast<const LONGLONG*>(vb.ReferBuffer())));
			}
			break;
		default:
			ASSERT(false);
			break;
		}
		return v;
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
		m_SetSectionName(lpszSectionName);
		bool r = false;
		switch ( value.GetKind() )
		{
		case EK_Nothing://���݂��Ȃ�
			r = m_Delete(lpszKey);
			break;
		case EK_String://������
			{
				CStr s = value.QueryString();
				r = m_Write(lpszKey, REG_SZ, LPCTSTR(s), (s.GetLength() + 1) * sizeof(TCHAR));
			}
			break;
		case EK_PluralStrings://�����̕�����Q
			{
				CDoubleNullTerminateStr dntstr;
				dntstr.Set(value.QueryPluralStrings());
				r = m_Write(lpszKey, REG_MULTI_SZ, LPCTSTR(dntstr), ToDword(dntstr.GetSize()) * sizeof(TCHAR));
			}
			break;
		case EK_Binary://�o�C�i��
			{
				const CByteVector& vb = value.QueryData();
				r = m_Write(lpszKey, REG_BINARY, vb.ReferBuffer(), ToDword(vb.GetSize()));
			}
			break;
		case EK_Dword://32Bit Unsigned Integer
			{
				DWORD dw = value.QueryDword();
				r = m_Write(lpszKey, REG_DWORD, &dw, sizeof(DWORD));
			}
			break;
		case EK_Longlong://64Bit Signed Integer
			{
				LONGLONG ll = value.QueryLonglong();
				r = m_Write(lpszKey, REG_QWORD, &ll, sizeof(LONGLONG));
			}
			break;
		default:
			ASSERT(false);
			break;
		}
		return r;
	}

private:
	enum { E_BufferSize = 1024 };			///< �擾�ő�T�C�Y
	REGSAM					m_dwRegSam;		///< �A�N�Z�X��
	mutable CRegKeyHandle	m_hhParentKey;	///< �Ώۂ̐e�n���h��
	mutable CStr			m_strSection;	///< �Ώۂ̃T�u�L�[
	mutable CRegKeyHandle	m_hhKey;		///< �Ώۂ̃T�u�L�[�n���h��
	/// �n���h������
	void m_Close(void) const
	{
		m_hhKey.Null();
	}
	/**
	 * �N���G�[�g
	 *	@retval 0 ����
	 *	@retval 0�ȊO �G���[�B
	 */
	LONG m_Create(void) const
	{
		if ( m_Open() == 0 )
		{
			return 0;
		}
		HKEY hKey;
		DWORD dwDisposition;
		LONG lRc = ::RegCreateKeyEx(
						m_hhParentKey,	// �J���Ă���e�L�[�̃n���h��
						m_strSection,	// �J���ׂ��T�u�L�[�̖��O
						0,				// �\��ς�
						NULL,			// �N���X��
						0,				// ���ʂȃI�v�V����
						m_dwRegSam,		// �Z�L�����e�B�A�N�Z�X�}�X�N
						NULL,			// �p���̎w��
						&hKey,			// [out] �J�����Ƃɐ��������T�u�L�[�̃n���h��
						&dwDisposition	// [out] �������ǂ��������l
		);
		if ( lRc == ERROR_SUCCESS )
		{
			m_hhKey = hKey;
		}
		else
		{
			m_hhKey.Null();
		}
		return lRc;
	}
	/**
	 * �I�[�v��
	 *	@retval 0 �����B
	 *	@retval 0�ȊO �G���[�B
	 */
	LONG m_Open(void) const
	{
		if ( ! m_hhKey.IsNull() )
		{
			return 0;
		}
		HKEY hKey;
		LONG lRc = ::RegOpenKeyEx(
						m_hhParentKey,	// �J���Ă���e�L�[�̃n���h��
						m_strSection,	// �J���ׂ��T�u�L�[�̖��O
						0,				// �\��ς�
						m_dwRegSam,		// �Z�L�����e�B�A�N�Z�X�}�X�N
						&hKey			// �J�����Ƃɐ��������T�u�L�[�̃n���h��
		);
		if ( lRc == ERROR_SUCCESS )
		{
			m_hhKey = hKey;
		}
		else
		{
			m_hhKey.Null();
		}
		return lRc;
	}
	/// �Z�N�V�������ݒ�
	void m_SetSectionName(LPCTSTR lpszSectionName) const
	{
		if ( m_strSection != lpszSectionName )
		{
			m_Close();
			m_strSection = lpszSectionName;
		}
	}
	/**
	 * [�擾] �Z�N�V�������ꗗ�擾
	 *	@return �Z�N�V�������ꗗ
	 */
	CStrVector m_EnumSections(HKEY hKey) const
	{
		CStrVector vstrRc;
		LONG lRc;
		DWORD dwIndex = 0;
		CStr strName;
		DWORD dwNameLength;
		FILETIME tFileTime;
		while ( true )
		{
			dwNameLength = E_BufferSize;
			lRc = ::RegEnumKeyEx(
							hKey,				// �񋓂���ׂ��L�[�̃n���h��
							dwIndex,			// �T�u�L�[�̃C���f�b�N�X�ԍ�
							strName.GetBuffer(dwNameLength + 1),
												// �T�u�L�[�����i�[�����o�b�t�@
							&dwNameLength,		// �T�u�L�[���o�b�t�@�̃T�C�Y
							NULL,				// �\��ς�
							NULL,				// �N���X�����i�[�����o�b�t�@
							NULL,				// �N���X������o�b�t�@�̃T�C�Y
							&tFileTime			// �ŏI�������ݎ���
			);
			strName.ReleaseBuffer();
			if ( lRc == ERROR_NO_MORE_ITEMS )
			{
				break;
			}
			if ( lRc != ERROR_SUCCESS )
			{
				vstrRc.Invalid();
				return vstrRc;
			}
			vstrRc.Add(strName);
			dwIndex++;
		}
		return vstrRc;
	}
	/**
	 * �l�ǂݍ���
	 *	@param lpszKey �ǂݍ��ރL�[��
	 *	@param[in]	dwTyp1 �ǂݍ��݋�����^�C�v
	 *	@param[in]	dwTyp2 �ǂݍ��݋�����^�C�v
	 *	@param[out]	_pdwGetType �ǂݍ��񂾃^�C�v�B����Ȃ��ꍇ�ANULL���w�肷��B
	 *	@return �ǂݍ��񂾃o�C�i��
	 */
	CByteVector m_Query(LPCTSTR lpszKey, DWORD dwTyp1, DWORD dwTyp2, DWORD* _pdwGetType = NULL) const
	{
		CByteVector vbResult;
		if ( m_Open() != 0 )
		{
			vbResult.Invalid();
			return vbResult; 
		}
		DWORD dwType;
		DWORD dwLength;
		LONG lRc = ::RegQueryValueEx(
						m_hhKey,			// �L�[�̃n���h��
						lpszKey,			// ���W�X�g���G���g����
						NULL,				// �\��ς�
						&dwType,			// �f�[�^�^���i�[�����o�b�t�@
						NULL,				// �f�[�^���i�[�����o�b�t�@
						&dwLength			// �f�[�^�o�b�t�@�̃T�C�Y
		);
		if ( lRc != ERROR_SUCCESS )
		{
			vbResult.Invalid();
			return vbResult;
		}
		if ( _pdwGetType != NULL )
		{
			*_pdwGetType = dwType;
		}
		if ( dwType != dwTyp1 && dwType != dwTyp2 )
		{
			vbResult.Invalid();
			return vbResult;
		}
		if ( dwLength > 0 )
		{
			lRc = ::RegQueryValueEx(
							m_hhKey,				// �L�[�̃n���h��
							lpszKey,				// ���W�X�g���G���g����
							NULL,					// �\��ς�
							&dwType,				// �f�[�^�^���i�[�����o�b�t�@
							vbResult.GetBuffer(dwLength),	// �f�[�^���i�[�����o�b�t�@
							&dwLength				// �f�[�^�o�b�t�@�̃T�C�Y
			);
			vbResult.ReleaseBuffer();
			if ( lRc != ERROR_SUCCESS )
			{
				vbResult.Invalid();
			}
		}
		return vbResult;
	}
	/**
	 * �l��������
	 *	@param[in]	lpszKey �������ރL�[��
	 *	@param[in]	dwTyp �������ރ^�C�v
	 *	@param[in]	lpcData �������ރf�[�^�B
	 *	@param[in]	dwLen �������ރf�[�^���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool m_Write(LPCTSTR lpszKey, DWORD dwTyp, LPCVOID lpcData, size_t dwLen)
	{
		LONG lRc = 0xFFFFFFFF;
		if ( m_Create() == 0 )
		{
			lRc = ::RegSetValueEx(m_hhKey, lpszKey, NULL, dwTyp, static_cast<const BYTE*>(lpcData), ToDword(dwLen));
		}
		return (lRc == ERROR_SUCCESS);
	}
	/**
	 * [�폜] �w��L�[�폜
	 *	@param lpszKey �L�[���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool m_Delete(LPCTSTR lpszKey)
	{
		LONG lRc = 0xFFFFFFFF;
		if ( m_Open() == 0 )
		{
			lRc = ::RegDeleteValue(m_hhKey, lpszKey);
		}
		return lRc == ERROR_SUCCESS;
	}
	/**
	 * [�폜] �ċA���W�X�g���폜
	 *	@see http://support.microsoft.com/kb/142491/ja
	 *	@param hStartKey �ΏۃL�[
	 *	@param pKeyName �폜�l�[��
	 *	@return ���U���g
	 */
	static DWORD ms_RegDeleteKeyNT(HKEY hStartKey , LPCTSTR pKeyName)
	{
		DWORD dwRtn = 0;
		const int MAX_KEY_LENGTH = 256;
		CWorkMemT<TCHAR> subKey(MAX_KEY_LENGTH); // (256) this should be dynamic.
		HKEY hKey = NULL;
		if ( pKeyName != NULL && STRLIB::GetLen(pKeyName) > 0 )
		{
			dwRtn = ::RegOpenKeyEx(hStartKey, pKeyName, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hKey);
			if ( dwRtn == ERROR_SUCCESS )
			{
				while ( dwRtn == ERROR_SUCCESS )
				{
					DWORD dwSubKeyLength = MAX_KEY_LENGTH;
					dwRtn = ::RegEnumKeyEx(
									hKey,
									0,       // always index zero
									subKey,
									&dwSubKeyLength,
									NULL,
									NULL,
									NULL,
									NULL
								);
					if ( dwRtn == ERROR_NO_MORE_ITEMS )
					{
						::RegCloseKey(hKey);
						hKey = NULL;
						dwRtn = ::RegDeleteKey(hStartKey, pKeyName);
						break;
					}
					else if ( dwRtn == ERROR_SUCCESS )
					{
						dwRtn = ms_RegDeleteKeyNT(hKey, subKey);
					}
				}
				if ( hKey != NULL )
				{
					::RegCloseKey(hKey);
					hKey= NULL;
				}
			}
		}
		else
		{
			 dwRtn = ERROR_BADKEY;
		}
		return dwRtn;
	}
};



}; // TNB
