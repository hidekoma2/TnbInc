#pragma once
/**
 *@file
 * �|�C���^�n���h���֌W�̃w�b�_
 *
 *		RAII(Resource Acquisition Is Initializetion)
 *
 *		��̃|�C���^���n���h�����܂��B auto_ptr �N���X�̂悤�Ȃ��̂ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"
#pragma warning( disable : 4284 )



//TNB Library
namespace TNB
{



/**@ingroup MEMORY
 * �|�C���^�n���h���e���v���[�g�x�[�X�N���X
 *
 *	@param TYP �ێ�����^�B
 *	@param DELE	�j������N���X���w�肵�܂��B
 *	@param NUL	TYP�̋���ۂ̒l���w�肵�܂��B�f�t�H���g�͂O�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPointerHandle.h
 *
 *	@date	06/09/29 ���ۉ��B
 *	@date	07/01/24 NUL�̈����A�C���B�O���l�ɏo����悤�ɂ����B
 */
template<typename TYP, typename DELE, INT_PTR NUL = 0>
class CPointerHandleBaseT
{
	/// �Ǘ��p�̍\����
	struct THead
	{
		LONG	refs;			///< �Q�Ɛ�
		TYP		type;			///< �Ǘ�����TYP�|�C���^
		/**
		 * �R���X�g���N�^
		 *	@param t type�ɐݒ肷��l
		 *	@param l refs�ɐݒ肷��l
		 */
		THead(TYP t = reinterpret_cast<TYP>(NUL), LONG l = 1) : refs(l) , type(t) 
		{
		}
	};

	THead*	m_ptHead;			///< THead�̃A�h���X @note NULL�ɂȂ邱�Ƃ͂���܂���

	/// NULL��Ԃɂ���
	void m_SetHeadNull(void)
	{
		m_ptHead = reinterpret_cast<THead*>(g_plNullBase);
	}

	/// NULL��Ԃ�
	bool m_IsHeadNull(void)
	{
		return m_ptHead == reinterpret_cast<THead*>(g_plNullBase);
	}

	void m_Null(void)
	{
		if ( ! m_IsHeadNull() )
		{
			if ( ::InterlockedDecrement(&(m_ptHead->refs)) <= 0 )
			{
				if ( m_ptHead->type != reinterpret_cast<TYP>(NUL) )
				{
					DELE()(m_ptHead->type);
				}
				delete m_ptHead;
			}
			m_SetHeadNull();
		}
	}

public:

	/**
	 * �R���X�g���N�^.
	 *		TYP�|�C���^���Ǘ����Ă��Ȃ��C���X�^���X���쐬����܂��B
	 */
	CPointerHandleBaseT(void)
	{
		m_SetHeadNull(); 
		if ( NUL != 0 ) { m_ptHead = new THead; }
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w�肳�ꂽTYP�|�C���^���Ǘ�����C���X�^���X���쐬����܂��B
	 *		���̎��_�ł́ATYP�|�C���^�͐V�C���X�^���X�������Q�Ƃ��Ă��܂��B
	 *	@param t �l
	 */
	CPointerHandleBaseT(TYP t)
	{
		m_SetHeadNull();
		operator=(t);
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *		�R�s�[����TYP�|�C���^������ꍇ�A�V�K�쐬�̖{�C���X�^���X�ł��A
	 *		�������Q�Ƃ���悤�ɂȂ�܂��B
	 *	@param other �����^�̃C���X�^���X�B
	 */
	CPointerHandleBaseT(const CPointerHandleBaseT& other)
	{
		m_SetHeadNull();
		operator=(other);
	}

	/**
	 * �f�X�g���N�^.
	 *	�uTYP�|�C���^���Ǘ����Ă�����A�J�����܂��B
	 *	�������ATYP�|�C���^����������Q�Ƃ���Ă�����A�Q�Ƃ�����炷�����ł��B
	 */
	~CPointerHandleBaseT(void)
	{
		m_Null(); 
	}

	/**
	 * [���] ���.
	 *		�{�C���X�^���X�ŊǗ����Ă���TYP�|�C���^���J�����A
	 *		������TYP�|�C���^���A�{�C���X�^���X�ŊǗ�����悤�ɂȂ�܂��B
	 *	@param t �l
	 *	@return �����̎Q�ƁB
	 */
	CPointerHandleBaseT& operator=(TYP t)
	{
		if ( m_ptHead->type != t )
		{
			if ( t != reinterpret_cast<TYP>(NUL) )
			{
				m_Null();
				m_ptHead = new THead(t);
			}
			else
			{
				Null();
			}
		}
		return *this;
	}

	/**
	 * [���] �R�s�[�I�y���[�^.
	 *		�{�C���X�^���X�ŊǗ����Ă���TYP�|�C���^���J�����A
	 *		�R�s�[����TYP�|�C���^������ꍇ�A�{�C���X�^���X�ł��A
	 *		�������Q�Ƃ���悤�ɂȂ�܂��B
	 *	@param other �����^�̃C���X�^���X�B
	 *	@return �����̎Q�ƁB
	 */
	CPointerHandleBaseT& operator=(const CPointerHandleBaseT& other)
	{
		if ( m_ptHead != other.m_ptHead )
		{
			if ( other.IsNull() )
			{
				//t �͋�B
				Null();
			}
			else if ( InterlockedCompareExchange(&(other.m_ptHead->refs), 2, 1) == 1 )
			{
				//�O���P��������Q�ɂ��āA�V�����o�b�t�@�A�h���X�����L
				m_Null();
				m_ptHead = other.m_ptHead;
			}
			else if ( (other.m_ptHead->refs) >= 1 )
			{
				//�O���P�ȏゾ������{�P���āA�V�����o�b�t�@�A�h���X�����L
				m_Null();
				other.m_ptHead->refs++;	// �Q�Ɛ����v���C�X�P
				m_ptHead = other.m_ptHead;
			}
		}
		ASEERT_NULLBASE;
		return *this;
	}

	/**
	 * [�m�F] NULL�`�F�b�N
	 *	@retval true TYP�|�C���^����
	 *	@retval false TYP�|�C���^�L��
	 */
	bool IsNull(void) const 
	{
		return m_ptHead->type == reinterpret_cast<TYP>(NUL);
	}

	/**
	 * [�ݒ�] �Q�Ɛ��ݒ�
	 *	@note	�ʏ�g���K�v�͗L��܂���B��O�Ƃ��āA0�ȉ��̎Q�Ɛ��́A�Q�Ƃ𑝂₷���Ƃ�
	 *			�o���Ȃ��l�ɂȂ��Ă��邽�߁A�Ӑ}�I�Ɏg�p���邱�Ƃ��o���܂��B
	 *	@param iCount �ݒ肷��Q�Ɛ�
	 *	@return �ݒ�O�̎Q�Ɛ�
	 */
	int SetReferCount(int iCount)
	{
		if ( m_IsHeadNull() ){ return 0; }
		return ::InterlockedExchange(&(m_ptHead->refs), iCount);
	}

	/**
	 * [�擾] �Q�Ɛ��擾.
	 *		TYP�|�C���^���Q�Ƃ���Ă��鐔��m�邱�Ƃ��o���܂��B
	 *	@note	�ʏ�g���K�v�͗L��܂���B
	 *	@return �Q�Ɛ�
	 */
	int GetReferCount(void) const { return m_ptHead->refs; }

	/**
	 * [�ݒ�] �J��.
	 *		�{�C���X�^���X���Ǘ����Ă���TYP���J�����܂��B
	 *	@note	operator=(NUL) �Ɠ�������ł��B
	 *	@note TYP����������g���Ă�����A�Q�Ƃ�����炷�����ł��B
	 */
	void Null(void)
	{
		m_Null();
		if ( NUL != 0 )
		{
			m_ptHead = new THead;
		}
	}

	/**
	 * [�擾] TYP�̃|�C���^�擾.
	 *	@return TYP�̃|�C���^
	 */
	TYP* ReferP(void) { return &(m_ptHead->type); }

	/**
	 * [�擾] TYP�̃|�C���^�擾.
	 *	@return TYP�̃|�C���^
	 */
	TYP const* ReferP(void) const { return &(m_ptHead->type); }

	/**
	 * [�擾] TYP�擾.
	 *		�Ǘ����Ă���TYP��Ԃ��܂�
	 *	@return TYP	�̒l
	 */
	operator TYP(void) { return m_ptHead->type; }

	/**
	 * [�擾] TYP�擾.
	 *		�Ǘ����Ă���TYP��Ԃ��܂�
	 *	@return TYP	�̒l
	 */
	operator const TYP(void) const { return m_ptHead->type; }

	/**
	 * [�擾] TYP�擾.
	 *		�Ǘ����Ă���TYP��Ԃ��܂�
	 *	@return TYP	�̒l
	 */
	TYP operator->(void) { return m_ptHead->type; }

	/**
	 * [�擾] TYP�擾.
	 *		�Ǘ����Ă���TYP��Ԃ��܂�
	 *	@return TYP	�̒l
	 */
	const TYP operator->(void) const { return m_ptHead->type; }

	/**
	 * [��r] TYP��r.
	 *		�ێ����Ă��� TYP�Ɣ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true ����
	 *	@retval false �قȂ�
	 */
	bool operator==(TYP t) const { return m_ptHead->type == t; }

	/**
	 * [��r] TYP��r.
	 *		�ێ����Ă��� TYP�Ɣ�r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval false ����
	 *	@retval true �قȂ�
	 */
	bool operator!=(TYP t) const { return ! operator==(t); }

private:
	friend class CPointerHandleTest;	///< �t�����h�N���X�錾
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/// PointerHandleBase�p �V���O���|�C���^Destory�N���X
struct TPhDeleteSinglePtr
{
	template<typename TYP> void operator()(TYP* P) { delete P; }
};
/// PointerHandleBase�p �V���O���|�C���^Destory�N���X
struct TPhDeleteArrayPtr
{
	template<typename TYP> void operator()(TYP* P) { delete[] P; }
};

#endif // _TnbDOXYGEN



/**@ingroup MEMORY
 * �|�C���^�^�n���h���e���v���[�g
 *
 *		���̌^�� new �Ŋm�ۂ����|�C���^��n���Ă����ƁA
 *		�u�̈�v���Q�Ƃ���{�N���X�����Ȃ��Ȃ莟��A
 *		�����I�ɊJ�����܂��B
 *
 *		�uJava�̎Q�ƌ^�v��uC++/CLR�̃n���h���^�v��
 *		�����g�������\�ɂȂ�܂��B
 *
 *		�R�s�[�I�y���[�^�������Ă���̂ŁA�֐��̖߂�l�Ȃǂ�
 *		���g�p�ł��܂��B
 *
 *	@note ����prefix�́uhp�v�ł��B
 *
 *	@param TYP �ێ�����^
 *
 *	@par�K�v�t�@�C��
 *			TnbPointerHandle.h
 *
 *	@date	04/11/02 �V�K�쐬
 *	@date	06/04/24 �e���v���[�g��
 *	@date	06/05/18 �X���b�h�Z�[�u��
 *	@date	06/05/20 ReferP()�A�ǉ��B
 *	@date	06/09/05 Empty -> Null()�ɕύX�BNew()�A�ǉ��B
 *	@date	06/09/29 �x�[�X�N���X����쐬����悤�ɕύX�B
 */
template<typename TYP>
class CPointerHandleT : public CPointerHandleBaseT<TYP*, TPhDeleteSinglePtr>
{
	typedef CPointerHandleBaseT<TYP*, TPhDeleteSinglePtr> _super;	///< �e�N���X���Đ錾
	DEFSUPER(_super);
public:

	/**
	 * �R���X�g���N�^.
	 *		TYP�|�C���^���Ǘ����Ă��Ȃ��C���X�^���X���쐬����܂��B
	 */
	CPointerHandleT(void) : _super() 
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w�肳�ꂽTYP�|�C���^���Ǘ�����C���X�^���X���쐬����܂��B
	 *		���̎��_�ł́ATYP�|�C���^�͐V�C���X�^���X�������Q�Ƃ��Ă��܂��B
	 *	@param P new �Ŋm�ۂ���TYP�|�C���^�B
	 */
	CPointerHandleT(TYP* P) : _super(P) 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *		�R�s�[����TYP�|�C���^������ꍇ�A�V�K�쐬�̖{�C���X�^���X�ł��A
	 *		�������Q�Ƃ���悤�ɂȂ�܂��B
	 *	@param t �����^�̃C���X�^���X�B
	 */
	CPointerHandleT(const CPointerHandleT& t) : _super(t) 
	{
	}

	/**
	 * [�擾] TYP�|�C���^�擾.
	 *		�Ǘ����Ă���TYP�|�C���^��Ԃ��܂�
	 *	@retval NUL	 �Ǘ����Ă��Ȃ�
	 *	@retval NUL�ȊO TYP�|�C���^
	 */
	TYP& operator*(void) { return **_super::ReferP(); }

	/**
	 * [�擾] TYP�|�C���^�擾.
	 *		�Ǘ����Ă���TYP�|�C���^��Ԃ��܂�
	 *	@retval NUL	 �Ǘ����Ă��Ȃ�
	 *	@retval NUL�ȊO TYP�|�C���^
	 */
	const TYP& operator*(void) const { return **_super::ReferP(); }
};



/**@ingroup MEMORY
 * �|�C���^�^�n���h���e���v���[�g
 *
 *		���̌^�� new �Ŋm�ۂ����|�C���^��n���Ă����ƁA
 *		�u�̈�v���Q�Ƃ���{�N���X�����Ȃ��Ȃ莟��A
 *		�����I�ɊJ�����܂��B
 *
 *		�uJava�̎Q�ƌ^�v��uC++/CLR�̃n���h���^�v��
 *		�����g�������\�ɂȂ�܂��B
 *
 *		�R�s�[�I�y���[�^�������Ă���̂ŁA�֐��̖߂�l�Ȃǂ�
 *		���g�p�ł��܂��B
 *
 *		@par
 *			<table><tr><td><pre>
 *			
 *			CArrayPtrHandleT<BYTE> foo(DWORD dwSize)
 *			{
 *			    CArrayPtrHandleT<BYTE> hpBuff = new BYTE[dwSize];
 *			    DWORD dwReadBytes = 0;
 *			    BOOL boRc = ::ReadFile(m_hFile,hpBuff,dwSize,&dwReadBytes,NULL);		
 *			    if ( ! boRc ){ hpBuff.Empty(); }
 *			    return hpBuff;
 *			}
 *			</pre></td></tr></table>
 *
 *	@note ����prefix�́uhp�v�ł��B
 *
 *	@param TYP �ێ�����^
 *
 *	@par�K�v�t�@�C��
 *			TnbPointerHandle.h
 *
 *	@date	06/09/29 �ʃN���X�Ƃ��Ē�`�B
 */
template<typename TYP>
class CArrayPtrHandleT : public CPointerHandleBaseT<TYP*, TPhDeleteArrayPtr>
{
	typedef CPointerHandleBaseT<TYP*, TPhDeleteArrayPtr> _super;	///< �e�N���X���Đ錾
	DEFSUPER(_super);
public:

	/**
	 * �R���X�g���N�^.
	 *		TYP�|�C���^���Ǘ����Ă��Ȃ��C���X�^���X���쐬����܂��B
	 */
	CArrayPtrHandleT(void) : _super() 
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		�w�肳�ꂽTYP�|�C���^���Ǘ�����C���X�^���X���쐬����܂��B
	 *		���̎��_�ł́ATYP�|�C���^�͐V�C���X�^���X�������Q�Ƃ��Ă��܂��B
	 *	@param P new �Ŋm�ۂ���TYP�|�C���^�B
	 */
	CArrayPtrHandleT(TYP* P) : _super(P) 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *		�R�s�[����TYP�|�C���^������ꍇ�A�V�K�쐬�̖{�C���X�^���X�ł��A
	 *		�������Q�Ƃ���悤�ɂȂ�܂��B
	 *	@param t �����^�̃C���X�^���X�B
	 */
	CArrayPtrHandleT(const CArrayPtrHandleT& t) : _super(t) 
	{
	}

	/**
	 * [�擾] TYP�|�C���^�擾.
	 *		�Ǘ����Ă���TYP�|�C���^��Ԃ��܂�
	 *	@param ind �C���f�b�N�X�B
	 *	@retval NUL	 �Ǘ����Ă��Ȃ�
	 *	@retval NUL�ȊO TYP�|�C���^
	 */
	TYP& operator[](int ind) { return (*_super::ReferP())[ind]; }

	/**
	 * [�擾] TYP�|�C���^�擾.
	 *		�Ǘ����Ă���TYP�|�C���^��Ԃ��܂�
	 *	@param ind �C���f�b�N�X�B
	 *	@retval NUL	 �Ǘ����Ă��Ȃ�
	 *	@retval NUL�ȊO TYP�|�C���^
	 */
	const TYP& operator[](int ind) const { return (*_super::ReferP())[ind]; }
};



};//TNB
