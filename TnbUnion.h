#pragma once
/**
 *@file
 * UNION�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * UNION�Ǘ��N���X
 *
 *		Set() ���\�b�h�ɂ��A�ǂ�Ȍ^�̒l�ł��i�[���Ă������Ƃ��o���܂��B
 *		Get() ���\�b�h�Ŏ��o�����́A Set() �������Ɠ����^�ł���K�v������܂��B
 *
 *	@note �C���X�^���X�𕡐����Ă��A�^�̎��Ԃ͈�Ȃ̂ŁA�ύX����ƁA��������
 *			�C���X�^���X�A�S���ɉe��������܂��B
 *
 *	@par�g�p��
 *		\code
 *		int i = 99;
 *		CStr s = "A";
 *		CString ss ="X"; 
 *		CUnion u(i);
 *		CUnion u2(u);
 *		TRACE1("%s\n", u.GetTypeName());
 *		TRACE1("%s\n", u2.GetTypeName());
 *		u.Set(ss);
 *		TRACE1("%s\n", u.GetTypeName());
 *		u.Set(s);
 *		u.Set(s);
 *		VERIFY( u.Get(s) );
 *		VERIFY( ! u2.Get(s) );
 *		TRACE1("%s\n", u.GetTypeName());
 *		TRACE1("%s\n", u2.GetTypeName());
 *		TRACE1("%d\n", u.Ref((CStr*)0).GetLength());
 *		TRACE1("%s\n", u.Ref((CStr*)0));
 *		TRACE1("%d\n", u2.Ref((int*)0));
 *		u.Set(i);
 *		u.Get(i);
 *		TRACE1("%s\n", u.GetTypeName());
 *		TRACE1("%s\n", u2.GetTypeName());
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbUnion.h
 *
 *	@date 06/09/25 �V�K
 *	@date 07/01/25 Release�r���h�̏ꍇ�A�^������T�C�Y�݂̂ɂ����B
 *	@date 07/02/28 ToString ��IF��p�ӁB
 */
class CUnion
{
public:

	///	UNION�Ǘ��N���X�p�t�@���N�V�����C���^�[�t�F�[�X
	struct IFuncBase
	{
		/// �f�X�g���N�^
		virtual ~IFuncBase(void) {}
		/// �f�X�g���N�g���s
		virtual void Destractor(LPVOID V) = 0;
	};


	//-----------------


	/** 
	 * �R���X�g���N�^.
	 *	�����ێ����Ȃ��C���X�^���X�쐬�B
	 */
	CUnion(void) : m_hpHead(new THead) 
	{
	}

	/**
	 * �R���X�g���N�^.
	 *	T �^�� t �̒l�����C���X�^���X�쐬�B
	 *	@note T �^�̓R�s�[�@�\���K�v�ł��B
	 *	@param t ����l
	 *	@param dwParam �p�����[�^
	 */
	template<typename T>
	CUnion(const T& t, DWORD dwParam = 0) : m_hpHead(new THead)
	{
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/** 
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��
	 */
	CUnion(const CUnion& other) : m_hpHead(other.m_hpHead)
	{
	}

	/// �f�X�g���N�^
	~CUnion(void)
	{
		Empty();
	}

	/** 
	 * [���] �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CUnion& operator=(const CUnion& other)
	{
		m_hpHead = other.m_hpHead;
		return *this;
	}

	/**
	 * [�ݒ�] ��.
	 *	�����ێ����Ȃ��C���X�^���X��
	 */
	void Empty(void)
	{
		m_hpHead = new THead;
	}

	/**
	 * [�ݒ�] ���.
	 *	T �^�� t �̒l�����C���X�^���X�ɂȂ�܂��B
	 *	@note T �^�̓R�s�[�@�\���K�v�ł��B
	 *	@attention	�����^�ő�����Ă��������̍Ċm�ۂ��������܂��B
	 *				�{�N���X�����[�N�Ƃ��Ă͎g�p���Ȃ��ق����ǂ��ł��B
	 *	@param t ����l
	 *	@param dwParam �p�����[�^
	 */
	template<typename T>
	void Set(const T& t, DWORD dwParam = 0)
	{
		m_hpHead = new THead;
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/**
	 * [�擾] �p�����[�^�擾.
	 *	@return �p�����[�^
	 */
	DWORD GetParam(void) const
	{
		return m_hpHead->m_dwParam;
	}

	/**
	 * [�擾] �擾.
	 *	T �^�� t �̒l�����C���X�^���X����A�l�𓾂܂��B�B
	 *	@note T �^�́A����R���X�g���N�g / Set() �������Ɠ����^�ł���K�v������܂��B
	 *	@param[out] _t �擾�I�u�W�F�N�g
	 *	@retval true ����
	 *	@retval false ���s(�^���قȂ�)
	 */
	template<typename T>
	bool Get(T& _t) const
	{
		#ifdef _DEBUG
			if ( strcmp(m_hpHead->m_strType, typeid(T).name()) != 0 )
			{
				return false;	//�^���قȂ�
			}
		#endif
		if ( m_hpHead->m_dwSizeof != sizeof(T) )
		{
			return false;	//�^���قȂ�
		}
		_t = *(static_cast<const T*>(m_hpHead->m_pVal));
		return true;
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
		#ifdef _DEBUG
			if ( strcmp(m_hpHead->m_strType, typeid(T).name()) != 0 )
			{
				throw CNoSuchTypeException();	//�^���قȂ�
			}
		#endif
		if ( m_hpHead->m_dwSizeof != sizeof(T) )
		{
			throw CNoSuchTypeException();	//�^���قȂ�
		}
		return *(static_cast<const T*>(m_hpHead->m_pVal));
	}

	/**
	 * [�擾] �^���擾.
	 *	@note DEBUG �r���h�̎��̂ݗL���ł��B
	 *	@return �C���X�^���X���ێ����Ă���^����Ԃ��܂��B
	 */
	LPCSTR GetTypeName(void) const
	{
		#ifdef _DEBUG
			return m_hpHead->m_strType;
		#else
			return "";
		#endif
	}

protected:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	/// �l�Ǘ��N���X
	struct THead
	{
		IFuncBase*		m_pFuncBase;	///< �ێ����Ă���^�̃t�@���N�V����
		LPVOID			m_pVal;			///< �ێ����Ă���^�̃|�C���^
		DWORD			m_dwSizeof;		///< �ێ����Ă���^�̃T�C�Y
		DWORD			m_dwParam;		///< �ėp�p�����[�^�l
		#ifdef _DEBUG
			CSimpleAscii m_strType;		///< �ێ����Ă���^�̖��O
		#endif
		/// �R���X�g���N�^
		THead(void) : m_pFuncBase(NULL), m_pVal(NULL), m_dwSizeof(0), m_dwParam(0)
		{
		}
		/// �f�X�g���N�^
		~THead(void)
		{
			if ( m_pFuncBase != NULL )
			{
				m_pFuncBase->Destractor(m_pVal);
				delete m_pFuncBase;
				m_pFuncBase = NULL;
			}
		}
		/// �Z�b�g
		template<typename T>
		void Set(const T& t, DWORD dwParam, IFuncBase* P = NULL)
		{
			m_pFuncBase		= P;
			m_pVal			= new T(t);
			m_dwParam		= dwParam;
			m_dwSizeof		= sizeof(T);
			#ifdef _DEBUG
				m_strType	= typeid(T).name();
			#endif
		}
	};

	///	�f�X�g���N�g�p�N���X
	template<typename T>
	struct CFuncT : IFuncBase
	{
		virtual void Destractor(LPVOID V)
		{
			delete static_cast<T*>(V);
		}
	};

	#endif // _TnbDOXYGEN

	CPointerHandleT<THead> m_hpHead;	///< ���̎Q�ƃn���h��
};



/**@ingroup VALUE
 * UNION�Ǘ��g���N���X
 *
 *		Set() ���\�b�h�ɂ��A�ǂ�Ȍ^(ToString()���\�b�h�����^�Ɍ���)�̒l�ł��i�[���Ă������Ƃ��o���܂��B
 *		Get() ���\�b�h�Ŏ��o�����́A Set() �������Ɠ����^�ł���K�v������܂��B
 *		ToString() ���\�b�h�ŕ�����Ƃ��Ď��o���܂��B
 *	
 *	@note CUnion �Ƃ͈قȂ�A�i�[�ł���^�� ToString() �����^�݂̂ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbUnion.h
 *
 *	@date 07/02/28 �V�K
 */
class CUnionEx : public CUnion
{
	DEFSUPER(CUnion);
public:

	///	UNION�Ǘ��g���N���X�p�t�@���N�V�����C���^�[�t�F�[�X
	struct IFuncBaseEx : CUnion::IFuncBase
	{
		/// ������֕ϊ�
		virtual void ToString(CSimpleStr& _s, LPVOID V) = 0;
	};


	//------------


	/** 
	 * �R���X�g���N�^.
	 *	�����ێ����Ȃ��C���X�^���X�쐬�B
	 */
	CUnionEx(void) : _super() 
	{
	}

	/**
	 * �R���X�g���N�^.
	 *	T �^�� t �̒l�����C���X�^���X�쐬�B
	 *	@note T �^�̓R�s�[�@�\���K�v�ł��B
	 *	@param t ����l
	 *	@param dwParam �p�����[�^
	 */
	template<typename T>
	CUnionEx(const T& t, DWORD dwParam = 0) : m_hpHead(new THead)
	{
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/** 
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��
	 */
	CUnionEx(const CUnionEx& other) : _super(other)
	{
	}

	/**
	 * [�ݒ�] ���.
	 *	T �^�� t �̒l�����C���X�^���X�ɂȂ�܂��B
	 *	@note T �^�̓R�s�[�@�\���K�v�ł��B
	 *	@attention	�����^�ő�����Ă��������̍Ċm�ۂ��������܂��B
	 *				�{�N���X�����[�N�Ƃ��Ă͎g�p���Ȃ��ق����ǂ��ł��B
	 *	@param t ����l
	 *	@param dwParam �p�����[�^
	 */
	template<typename T>
	void Set(const T& t, DWORD dwParam = 0)
	{
		m_hpHead = new THead;
		m_hpHead->Set(t, dwParam, new CFuncT<T>);
	}

	/**
	 * [�擾] ������擾
	 *	@param[out] _s �擾����
	 *	@throw CNullPointerException ����������Ă��Ȃ��Ƃ��Ɏg�p����ƃX���[����܂��B
	 */
	void ToString(CSimpleStr& _s) const
	{
		if ( m_hpHead->m_pFuncBase == NULL )
		{
			throw CNullPointerException();
		}
		reinterpret_cast<IFuncBaseEx*>(m_hpHead->m_pFuncBase)->ToString(_s, m_hpHead->m_pVal);
	}

private:
	///	�t�@���N�V�����N���X
	template<typename T>
	struct CFuncT : CUnionEx::IFuncBaseEx
	{
		virtual void Destractor(LPVOID V)
		{
			delete static_cast<T*>(V);
		}
		virtual void ToString(CSimpleStr& _s, LPVOID V)
		{
			_s = static_cast<T*>(V)->ToString();
		}
	};
};



};//TNB



#if 0
	int i = 99;
	CStr s = "A";
	CString ss ="X"; 
	CUnion u(i);
	CUnion u2(u);
	TRACE1("%s\n",u.GetTypeName());
	TRACE1("%s\n",u2.GetTypeName());
	u.Put(ss);
	TRACE1("%s\n",u.GetTypeName());
	u.Put(s);
	u.Put(s);
	VERIFY( u.Get(s) );
	VERIFY( ! u2.Get(s) );
	TRACE1("%s\n",u.GetTypeName());
	TRACE1("%s\n",u2.GetTypeName());
	TRACE1("%d\n",u.Ref((CStr*)0).GetLength());
	TRACE1("%s\n",u.Ref((CStr*)0));
	TRACE1("%d\n",u2.Ref((int*)0));
	u.Put(i);
	u.Get(i);
	TRACE1("%s\n",u.GetTypeName());
	TRACE1("%s\n",u2.GetTypeName());
#endif
