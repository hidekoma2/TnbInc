#pragma once
/**
 *@file
 * UNION�o�����[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbUnion.h"
#include "TnbValueUnit.h"
#include "TnbValueParUnit.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUEUNIT
 * UNION �P�ʕt�l�Ǘ��N���X
 *
 *		Set() ���\�b�h�ɂ��A�ǂ�Ȍ^�̒l�ł��i�[���Ă������Ƃ��o���܂��B
 *		Get() ���\�b�h�Ŏ��o�����́A Set() �������Ɠ����^�ł���K�v������܂��B
 *	
 *	@note �R�s�[�R���X�g���N�^�A�R�s�[�I�y���[�^���g�p�\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbUnionValueUnit.h
 *
 *	@date 07/01/30 �V�K
 */
class CUnionValueUnit
{
public:

	///	�t�@���N�V�����x�[�X�N���X
	struct IFuncBaseVu : CUnion::IFuncBase
	{
		/// ���l�擾
		virtual CRealNumber GetValue(LPVOID V) const = 0;
		/// �P�ʋL���擾
		virtual CStr GetUnitMarkString(LPVOID V) const = 0;
	};

private:

	/// ��pUnion�N���X
	class CUnionVu : public CUnion
	{
		DEFSUPER(CUnion);
		///	�t�@���N�V�����N���X
		template<typename T>
		struct CFuncT : CUnionValueUnit::IFuncBaseVu
		{
			virtual void Destractor(LPVOID V)
			{
				delete static_cast<T*>(V);
			}
			virtual CRealNumber GetValue(LPVOID V) const
			{
				return static_cast<T*>(V)->GetValue();
			}
			virtual CStr GetUnitMarkString(LPVOID V) const
			{
				return static_cast<T*>(V)->GetUnitMarkString();
			}
		};
	public:
		/// �R���X�g���N�^.
		CUnionVu(void) : _super() {}
//		/**
//		 * �R���X�g���N�^.
//		 *	T �^�� t �̒l�����C���X�^���X�쐬�B
//		 */
//		template<typename T>
//		CUnionVu(const T& t, DWORD dwParam=0) : m_hpHead(new THead)
//		{
//			m_hpHead->Set(t, dwParam, new CFuncT<T>);
//		}
		/// �R�s�[�R���X�g���N�^.
		CUnionVu(const CUnionVu& other) : _super(other) {}
		/// ���.
		template<typename T>
		void Set(const T& t, DWORD dwParam = 0)
		{
			m_hpHead = new THead;
			m_hpHead->Set(t, dwParam, new CFuncT<T>);
		}
		/// �l�擾
		CRealNumber GetValue(void) const
		{
			return reinterpret_cast<IFuncBaseVu*>(m_hpHead->m_pFuncBase)->GetValue(m_hpHead->m_pVal);
		}
		/// �P�ʋL���擾
		CStr GetUnitMarkString(void) const
		{
			return reinterpret_cast<IFuncBaseVu*>(m_hpHead->m_pFuncBase)->GetUnitMarkString(m_hpHead->m_pVal);
		}
	};

	CUnionVu	m_uni;
	CStr		m_strFormat;
	CStr		m_strInvalid;

public:

	/// �R���X�g���N�^
	CUnionValueUnit(void) : m_uni()
	{
	}

//	/*
//	 * �R���X�g���N�^
//	 *	@note T �^�� ValueUnit �n�N���X�iCAbstractUnit�̃T�u�N���X�j�Ɍ���܂��B
//	 *	@param t ����l
//	 */
//	template<typename T>
//	CUnionValueUnit(const T& t) : m_uni(t, T::KINDCODE)
//	{
//		ASSERT( T::TNBLIB_VALUEUNIT == -1 );
//	}

	/// �f�X�g���N�^
	~CUnionValueUnit(void)
	{
	}

	/**
	 * [�ݒ�] ��.
	 *	�����ێ����Ȃ��C���X�^���X��
	 */
	void Empty(void)
	{
		m_uni.Empty();
	}

	/**
	 * [�ݒ�] ���.
	 *	T �^�� t �̒l�����C���X�^���X�ɂȂ�܂��B
	 *	@note T �^�� ValueUnit �n�N���X�iCAbstractUnit�̃T�u�N���X�j�Ɍ���܂��B
	 *	@param t ����l�B
	 *	@param lpszFormat ���l�����̃t�H�[�}�b�g�B"%f" ���܂܂�Ă���K�v������܂��B
	 *	@param lpszInvalid �������ُ�̏ꍇ�̕\�������B
	 */
	template<typename T>
	void Set(const T& t, LPCTSTR lpszFormat = NULL, LPCTSTR lpszInvalid = NULL)
	{
		ASSERT( T::TNBLIB_VALUEUNIT == -1 );
		m_uni.Set(t, T::KINDCODE);
		m_strFormat = lpszFormat;
		m_strInvalid = (lpszInvalid == NULL ) ? _T("?") : lpszInvalid;
	}

	/**
	 * [�擾] �P�ʎ�R�[�h�擾.
	 *	@return �P�ʎ�R�[�h
	 */
	DWORD GetUnitKindCode(void) const
	{
		return m_uni.GetParam();
	}

	/**
	 * [�擾] �擾.
	 *	T �^�� t �̒l�����C���X�^���X����A�l�𓾂܂��B�B
	 *	@note T �^�́A����R���X�g���N�g / Put() �������Ɠ����^�ł���K�v������܂��B
	 *	@param[out] _t �擾�I�u�W�F�N�g
	 *	@retval true ����
	 *	@retval false ���s(�^���قȂ�)
	 */
	template<typename T>
	bool Get(T& _t) const
	{
		if ( T::KINDCODE != m_uni.GetParam() )
		{
			return false;	//�^���قȂ�
		}
		return m_uni.Get(_t);
	}

	/**
	 * [�擾] �Q�Ǝ擾.
	 *	T �^�� t �̒l�����C���X�^���X�̎��l�̎Q�Ƃ𓾂܂��B�B
	 *	@note T �^�́A����R���X�g���N�g / Put() �������Ɠ����^�ł���K�v������܂��B
	 *	@param P �擾�������^�̃|�C���^���w�肵�Ă��������i�A�N�Z�X���Ȃ��̂�NULL�ō\���܂���j�B
	 *	@throw CNoSuchTypeException �C���X�^���X�����^�� T �^���قȂ�ꍇ�A�X���[����܂��B
	 *	@return �Q��
	 */
	template<typename T>
	const T& Ref(T* P) const 
	{
		if ( T::KINDCODE != m_uni.GetParam() )
		{
			throw CNoSuchTypeException();	//�^���قȂ�
		}
		return m_uni.Ref(P);
	}

	/**
	 * [�擾] ������擾
	 *	@return ������B
	 */
	CStr ToString(void) const
	{
		CStr strRet;
		CRealNumber r = GetValue();
		if ( ! r.IsEmpty() )
		{
			CStr s = r.IsValid() ? r.ToString(m_strFormat) : m_strInvalid;
			strRet = CStr::Fmt(_T("%s %s"), s, m_uni.GetUnitMarkString());
		}
		return strRet;
	}

	/**
	 * [�擾] �l�擾
	 *	@return �l�B
	 */
	CRealNumber GetValue(void) const
	{
		return m_uni.GetValue();
	}

	/**
	 * [�擾] �P�ʋL���擾
	 *	@return ������B
	 */
	CStr GetUnitMarkString(void) const
	{
		return m_uni.GetUnitMarkString();
	}
};



};//TNB



