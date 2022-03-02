#pragma once
/**
 *@file
 * ��񑊌݃A�N�Z�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAccessor.h"
#include "TnbSerializer.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * ��񑊌݃A�N�Z�X�N���X
 *
 *		IAccessor �����N���X�̃A�N�Z�X�⏕�N���X�ł��B
 *
 *		IAccessor �ւ̃A�N�Z�X�iQuery / Write�j���s���ϐ��ƊǗ��p�̃L�[��
 *		�ꌳ�ɊǗ����邱�Ƃ��o����悤�ɂȂ�܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		void CFoo::Init(void)
 *		{
 *			CInifileAccessor ini;
 *			CMutualAccessor a(&ini, CMutualAccessor::QUERY);
 *			BackupRestore(a);
 *		}
 *
 *		void CFoo::Exit(void)
 *		{
 *			CInifileAccessor ini;
 *			CMutualAccessor a(&ini, CMutualAccessor::WRITE);
 *			BackupRestore(a);
 *		}
 *		
 *		bool CFoo::BuckupRestore(CMutualAccessor& _a)
 *		{
 *			CMutualAccessor::CSubSection sub(&_a, _T("Settings"));
 *			//
 *			bool r = _a.Works("DeviceNumber", m_wDeviceNumber, 1);
 *			r &= _a.Works("ModelNo", m_wModelNo, 3);
 *			r &= _a.Works("HardVersion", m_wHardVersion, 3);
 *			//
 *			CStrVector vs;
 *			vs.Add("NoConnect");
 *			vs.Add("Connect");
 *			r &= _a.Works("Mode", m_dwMode, vs, 0);
 *			return r;
 *		}
 *
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbMutualAccessor.h
 *
 *	@date 08/04/01 �V�K�쐬
 *	@date 08/04/15 OnFalse() �ǉ��B CSubSection�N���X �ǉ��B
 *	@date 10/01/15 �\���X�V�B
 *	@date 11/02/11 double �ɂ��Ή��B
 */
class CMutualAccessor
{
public:

	/**@ingroup ACCESSOR
	 * ��񑊌݃A�N�Z�X�̃��[�J�[�C���^�[�t�F�[�X
	 *
	 *		CMutualAccessor �̃T�|�[�g�C���^�[�t�F�[�X�ł��B
	 *
	 *		���̃C���^�[�t�F�[�X�������Ă���I�u�W�F�N�g�� CMutualAccessor::Works() �ŏ������邱�Ƃ��o���܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMutualAccessor.h
	 *
	 *	@date 10/01/15 �V�K�쐬
	 */
	struct IWorker
	{
		/// �f�X�g���N�^
		virtual ~IWorker(void) {}

		/**
		 * [����]
		 *	@param[in,out] sec �Z�N�V����.
		 *	@param[in] lpszKey �L�[.
		 *	@param[in] isQuery true �Ȃ� QUERY���[�h�Afalse �Ȃ�WRITE���[�h�B
		 *	@retval true �����B
		 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
		 */
		virtual bool MutualAccessWorks(IAccessor::CSection& sec, LPCTSTR lpszKey, bool isQuery) = 0;

		#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
			virtual bool Works(IAccessor::CSection& sec, LPCTSTR lpszKey, bool isQuery)
			{
				return MutualAccessWorks(sec, lpszKey, isQuery);
			}
		#endif
	};

	/**@ingroup ACCESSOR
	 * ��񑊌݃A�N�Z�X�T�u�Z�N�V�����N���X
	 *
	 *		CMutualAccessor �̃T�|�[�g�N���X�ł��B
	 *
	 *	@see CMutualAccessor
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMutualAccessor.h
	 *
	 *	@date 08/04/15 �V�K�쐬
	 */
	class CSubSection
	{
	public:
		/**
		 * �R���X�g���N�^
		 *	@param P CMutualAccessor �̃|�C���^
		 *	@param lpszSub �T�u�Z�N�V������
		 */
		CSubSection(CMutualAccessor* P, LPCTSTR lpszSub) : m_pMu(P), m_success(false)
		{
			m_success = m_pMu->InSubSection(lpszSub);
		}
		/// �f�X�g���N�^
		~CSubSection(void)
		{
			if ( m_success )
			{
				m_pMu->PreviousSubSection();
			}
		}
	private:
		CMutualAccessor*	m_pMu;		///< ���� CMutualAccessor
		bool				m_success;	///< InSubSection������������
	};


	//----------------------------------------------------------------


	/// ���[�h
	enum EMode
	{
		QUERY,	///< QUERY���[�h
		WRITE,	///< WRITE���[�h
	};

	/**
	 * �R���X�g���N�^
	 *	@param pAccessor IAccessor �̃|�C���^�B NULL �͎w��ł��܂���B
	 *	@param mode �{�C���X�^���X�̃��[�h�B �ȗ��� QUERY �Ȃ� QUERY���[�h�A WRITE �Ȃ�WRITE���[�h���w��B
	 */
	CMutualAccessor(IAccessor* pAccessor, EMode mode = QUERY) 
		: m_sec(IAccessor::CSection(pAccessor, _T(""))), m_isQuery(mode == QUERY)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param sec �Z�N�V�����B
	 *	@param mode �{�C���X�^���X�̃��[�h�B �ȗ��� QUERY �Ȃ� QUERY���[�h�A WRITE �Ȃ�WRITE���[�h���w��B
	 */
	CMutualAccessor(IAccessor::CSection& sec, EMode mode = QUERY) 
		: m_sec(sec), m_isQuery(mode == QUERY)
	{
	}

	/**
	 * [�ݒ�] �ΏۃZ�N�V�����ύX.
	 *		�w��̃T�u�Z�N�V������ǉ����A�擾�Ώۂ����̃Z�N�V�������ɂ��܂��B
	 *	@param lpszSection �Z�N�V������
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool InSubSection(LPCTSTR lpszSection)
	{
		return m_Result(m_sec.InSubSection(lpszSection));
	}

	/**
	 * [�ݒ�] �ΏۃZ�N�V�����ύX.
	 *		�擾�Ώۂ����̃Z�N�V�������ɂɂ��܂��B
	 *	@note IsSubSection() �Ƒ΂ɂȂ�悤�Ɏg�p���܂��B
	 */
	void PreviousSubSection(void)
	{
		m_sec.PreviousSubSection();
	}

	/**
	 * [�m�F] ���[�h�m�F
	 *	@retval true QUERY���[�h
	 *	@retval false WRITE���[�h
	 */
	bool IsQueryMode(void) const
	{
		return m_isQuery; 
	}

	/**
	 * [�m�F] ���[�h�m�F
	 *	@retval false QUERY���[�h
	 *	@retval true WRITE���[�h
	 */
	bool IsWriteMode(void) const
	{
		return ! m_isQuery; 
	}

	/**
	 * [�Q��] CSection �Q��.
	 *	@note �{�N���X���g�p���Ă��� CSection ���Q�Ƃ��܂��B
	 *		Works() ���\�b�h�����ł͏����ł��Ȃ��ꍇ�A���[�h���m�F���Ė{���\�b�h���g���āA�������\�ł��B
	 *	@return CSection �B
	 */
	IAccessor::CSection* operator->(void)
	{
		return &m_sec;
	}

	/**
	 * [����] bool ��񏈗�.
	 *		bool ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _boValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B QUERY���[�h�̎��́A�l���s��������̏ꍇ( _boValue �� def ������܂�)�B\n
	 *						WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������B
	 */
	bool Works(LPCTSTR lpszKey, bool& _boValue, bool def = false)
	{
		CStrVector vs;
		vs.Add(_T("false"));
		vs.Add(_T("true"));
		DWORD w = _boValue;
		bool r = Works(lpszKey, w, vs, def);
		_boValue = (w != 0);
		return m_Result(r);
	}

	/**
	 * [����] BYTE ��񏈗�.
	 *		BYTE ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _bValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, BYTE& _bValue, BYTE def = 0)
	{
		if ( m_isQuery )
		{
			DWORD w = m_sec.QueryDword(lpszKey, def);
			_bValue = static_cast<BYTE>(w);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _bValue));
	}

	/**
	 * [����] WORD ��񏈗�.
	 *		WORD ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _wValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, WORD& _wValue, WORD def = 0)
	{
		if ( m_isQuery )
		{
			DWORD w = m_sec.QueryDword(lpszKey, def);
			_wValue = static_cast<WORD>(w);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _wValue));
	}

	/**
	 * [����] short ��񏈗�.
	 *		short ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _value �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, short& _value, short def = 0)
	{
		if ( m_isQuery )
		{
			DWORD w = m_sec.QueryDword(lpszKey, def);
			_value = static_cast<short>(w);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _value));
	}

	/**
	 * [����] DWORD ��񏈗�.
	 *		DWORD ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _dwValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, DWORD& _dwValue, DWORD def = 0)
	{
		if ( m_isQuery )
		{
			_dwValue = m_sec.QueryDword(lpszKey, def);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _dwValue));
	}

	/**
	 * [����] int ��񏈗�.
	 *		int ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _iValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, int& _iValue, int def = 0)
	{
		if ( m_isQuery )
		{
			LONGLONG l = m_sec.QueryLonglong(lpszKey, def);
			_iValue = static_cast<int>(l);
			return true;
		}
		return m_Result(m_sec.WriteLonglong(lpszKey, _iValue));
	}

	/**
	 * [����] long ��񏈗�.
	 *		long ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _lValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, long& _lValue, long def = 0)
	{
		if ( m_isQuery )
		{
			LONGLONG l = m_sec.QueryLonglong(lpszKey, def);
			_lValue = static_cast<long>(l);
			return true;
		}
		return m_Result(m_sec.WriteLonglong(lpszKey, _lValue));
	}

	/**
	 * [����] LONGLONG ��񏈗�.
	 *		LONGLONG ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _llValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, LONGLONG& _llValue, LONGLONG def = 0)
	{
		if ( m_isQuery )
		{
			_llValue = m_sec.QueryLonglong(lpszKey, def);
			return true;
		}
		return m_Result(m_sec.WriteLonglong(lpszKey, _llValue));
	}

	/**
	 * [����] LONGLONG ��񏈗�.
	 *		LONGLONG ���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _value �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, double& _value, double def = 0.0)
	{
		if ( m_isQuery )
		{
			CByteVector vb = m_sec.QueryData(lpszKey);
			_value = def;
			if ( vb.GetSize() == sizeof(double) )
			{
				_value = *reinterpret_cast<const double*>(vb.ReferBuffer());

			}
			return true;
		}
		return m_Result(m_sec.WriteData(lpszKey, CConstAdapterT<BYTE>(sizeof(double), &_value)));
	}

	/**
	 * [����] �������񏈗�.
	 *		��������� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _strValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, CStr& _strValue, LPCTSTR def = NULL)
	{
		if ( m_isQuery )
		{
			_strValue = m_sec.QueryString(lpszKey, def);
			return true;
		}
		return m_Result(m_sec.WriteString(lpszKey, _strValue));
	}

	/**
	 * [����] ���K�V�[�\���̏�񏈗�.
	 *		�\���̂� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in] size P �̃o�b�t�@�T�C�Y�B
	 *	@param[in,out] P �l�B QUERY���[�h�Ȃ�擾������������Ɋi�[�BWRITE���[�h�Ȃ炱�̏��������݁B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, size_t size, LPVOID P)
	{
		if ( m_isQuery )
		{
			CByteVector vb = m_sec.QueryData(lpszKey);
			memset(P, 0, size);
			if ( vb.GetSize() == size )
			{
				vb.GetElements(size, static_cast<BYTE*>(P));
			}
			return true;
		}
		return m_Result(m_sec.WriteData(lpszKey, CConstAdapterT<BYTE>(size, P)));
	}

	/**
	 * [����] �R���N�V������񏈗�.
	 *		�R���N�V�������� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _c �l�B QUERY���[�h�Ȃ�擾������������Ɋi�[�BWRITE���[�h�Ȃ炱�̏��������݁B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, ICollectionT<BYTE>& _c)
	{
		if ( m_isQuery )
		{
			CByteVector vb = m_sec.QueryData(lpszKey);
			_c.Copy(vb);
			return true;
		}
		return m_Result(m_sec.WriteData(lpszKey, _c));
	}

	/**
	 * [����] �V���A���C�U�u����񏈗�.
	 *		�V���A���C�U�u������ Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _s �l�B QUERY���[�h�Ȃ�擾������������Ɋi�[�BWRITE���[�h�Ȃ炱�̏��������݁B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, ISerializable& _s)
	{
		ASSERTLIB( false ); // ���̃��\�b�h�͐��ݓI�ȃo�O�̂���\��������܂��B
		CSubSection sub(this, lpszKey);
		bool r = false;
		try
		{
			CMySerializer sr(&m_sec);
			if ( m_isQuery )
			{
				sr >> _s;
			}
			else
			{
				m_sec.DeleteAllKeys();
				sr << _s;
			}
			r = true;
		}
		catch ( CNoSuchTypeException& e )
		{
			e.OnCatch();
		}
		return r;
	} 

	/**
	 * [����] DWORD ��񏈗�.
	 *		DWORD ����Query / Write ���܂��B���̍ہA�l�Ƒ΂ɂȂ镶����z����g���āA�����Ǘ����܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _dwValue �l�BQUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] vs �l��΂ɂȂ镶����z��B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B QUERY���[�h�̎��́A�l���s��������̏ꍇ( _dwValue �� def ������܂�)�B\n
	 *						WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������B
	 */
	bool Works(LPCTSTR lpszKey, DWORD& _dwValue, const CStrVector& vs, DWORD def = 0)
	{
		if ( m_isQuery )
		{
			_dwValue = def;
			if ( ! m_sec.HasKey(lpszKey) )
			{
				return true;
			}
			CStr s = m_sec.QueryString(lpszKey);
			INDEX r = vs.Find(s);
			if ( r != INVALID_INDEX )
			{
				_dwValue = r;
				return true;
			}
			return m_Result(false);
		}
		if ( vs.GetSize() > _dwValue )
		{
			return m_Result(m_sec.WriteString(lpszKey, vs[_dwValue]));
		}
		m_sec.DeleteKey(lpszKey);
		return m_Result(false);
	}

	/**
	 * [����] ���[�J�[��񏈗�.
	 *		���[�J�[����Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _worker ���[�J�[�B
	 *	@retval true �����B
	 *	@retval false ���s�BWRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������B
	 */
	bool Works(LPCTSTR lpszKey, IWorker& _worker)
	{
		bool r = _worker.Works(m_sec, lpszKey, m_isQuery);
		return m_Result(r);
	}

#ifdef __AFX_H__
	/**
	 * [����] �������񏈗�.
	 *		��������� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] _strValue �l�B QUERY���[�h�Ȃ�擾�����l������Ɋi�[�BWRITE���[�h�Ȃ炱�̒l�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, CString& _strValue, LPCTSTR def = NULL)
	{
		if ( m_isQuery )
		{
			_strValue = m_sec.QueryString(lpszKey, def);
			return true;
		}
		CStr s = _strValue;
		return m_Result(m_sec.WriteString(lpszKey, s));
	}

	/**
	 * [����] �E�B���h�E�e�L�X�g��񏈗�.
	 *		�E�B���h�E�e�L�X�g���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] hWnd �E�B���h�E�n���h���BQUERY���[�h�Ȃ�擾�����e�L�X�g�����̃E�B���h�E�ɐݒ�BWRITE���[�h�Ȃ炱�̃E�B���h�E�̃e�L�X�g�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���߂�l�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, HWND hWnd, LPCTSTR def = NULL)
	{
		if ( m_isQuery )
		{
			m_sec.QueryWindowText(lpszKey, hWnd, def);
			return true;
		}
		return m_Result(m_sec.WriteWindowText(lpszKey, hWnd));
	}

	/**
	 * [����] �R���{�{�b�N�X��񏈗�.
	 *		�R���{�{�b�N�X���� Query / Write ���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] comboBox �R���{�{�b�N�X�BQUERY���[�h�Ȃ�擾�����C���f�b�N�X�ŃR���{�{�b�N�X��I���BWRITE���[�h�Ȃ�I������Ă���C���f�b�N�X�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���C���f�b�N�X�Ƃ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, CComboBox& comboBox, int def = 0)
	{
		if ( m_isQuery )
		{
			comboBox.SetCurSel(m_sec.QueryDword(lpszKey, def));
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, comboBox.GetCurSel()));
	}

	/**
	 * [����] �{�^����񏈗�.
	 *		�{�^������ Query / Write ���܂��B�{�^���͒ʏ�u�`�F�b�N�{�b�N�X�v�̂��̂��g���܂��B
	 *	@param[in] lpszKey �L�[�B
	 *	@param[in,out] button �{�^���BQUERY���[�h�Ȃ�擾�����l�Ń{�^����Ԃ�ݒ�BWRITE���[�h�Ȃ�{�^����Ԃ��C���f�b�N�X�������݁B
	 *	@param[in] def QUERY���[�h�̂ݎg�p�B�w��̃L�[���Ȃ��ꍇ�A���̒l���{�^����ԂƂ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B WRITE���[�h�̏ꍇ�͏����݂Ɏ��s�������iQUERY���[�h�͎��s�͂���܂���j�B
	 */
	bool Works(LPCTSTR lpszKey, CButton& button, int def = BST_UNCHECKED)
	{
		if ( m_isQuery )
		{
			button.SetCheck(m_sec.QueryDword(lpszKey, def));
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, button.GetCheck()));
	}
#endif //__AFX_H__

protected:

	/**
	 * [�ʒm] �e�������s�ʒm.
	 *	@note �e���������s�������ɒʒm����܂��B���̃��\�b�h���p������ Throw ����ȂǁA������ύX�ł��܂��B
	 *	@note �{�N���X�ł́A�������܂���B
	 */
	virtual void OnFalse(void) const
	{
	}

private:

	bool					m_isQuery;		///< true �Ȃ� QUERY���[�h�Afalse �Ȃ�WRITE���[�h�B
	IAccessor::CSection		m_sec;			///< �Z�b�V����
	///> �߂�l�`�F�b�N
	bool m_Result(bool r) const
	{
		if ( ! r )
		{
			OnFalse();
		}
		return r;
	}

	// �V���A���C�U�[
	class CMySerializer : public ISerializer, public IDeserializer
	{
	public:
		/**
		 * �R���X�g���N�^.
		 *	@param P �Z�N�V����
		 */
		CMySerializer(IAccessor::CSection* P) : m_pSec(P), m_count(0)
		{
		}
		/**
		 * [�ǉ�] �ǉ�.
		 *	@param size �ǉ�����f�[�^�T�C�Y(�P��BYTE)
		 *	@param P �ǉ�����f�[�^
		 */
		virtual void AddRaw(size_t size, LPCVOID P)
		{
			m_pSec->WriteData(CStr::Fmt(_T("%04d"), m_count), CConstAdapterT<BYTE>(size, P));
			m_count++;
		}
		/**
		 * [�擾] �擾.
		 *	@param[in] size �擾����f�[�^�T�C�Y(�P��BYTE)
		 *	@param[out] _P �擾����f�[�^
		 */
		virtual void GetRaw(size_t size, LPVOID _P) const
		{
			CByteVector vb = m_pSec->QueryData(CStr::Fmt(_T("%04d"), m_count));
			if ( size != vb.GetSize() )
			{
				throw CNoSuchTypeException();
			}
			vb.GetElements(size, static_cast<BYTE*>(_P));
			m_count++;
		}
	private:
		mutable int				m_count;	///< �J�E���^
		IAccessor::CSection*	m_pSec;		///< �Z�N�V����
	};
};



}; // TNB
