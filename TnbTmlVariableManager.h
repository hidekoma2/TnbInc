#pragma once
/**
 *@file
 * TinyMacroLang �ϐ��Ǘ��֌W�̃w�b�_
 *	
 *		TML�̕ϐ��Ǘ��̃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTmlValue.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang �ϐ��Ǘ�
 *
 *		���x���������ĕϐ���/�z�񖼁A����сA���e���Ǘ����܂��B
 *	@note ���[�J���G���A�ł̐錾�ȂǂŁA�������Ԃ̈قȂ���̂��Ǘ����邽�߂Ƀ��x���Ǘ����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlVariableManager.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
class CTmlVariableManager
{
public:

	/// TinyMacroLang �ϐ����Ɠ��e�̌^
	struct TVarValue
	{
		CAscii		name;	///< �ϐ���
		CTmlValue	value;	///< ���e
	};

	/// �ϐ����Ɠ��e�̔z��^
	typedef CVectorT<TVarValue> CVaVaVector;

	/// �ϐ����Ɠ��e�̓񎟌��z��^
	typedef CVectorT<CVaVaVector> CVaVaVectors;

	/// TinyMacroLang �z�񖼂Ɠ��e�^
	struct TArrayValue
	{
		CAscii				name;	///< �z��
		CTmlValuesVector	values;	///< ���e
	};

	/// �z�񖼂Ɠ��e�̔z��^
	typedef CVectorT<TArrayValue> CArVaVector;

	/// �z�񖼂Ɠ��e�̓񎟌��z��^
	typedef CVectorT<CArVaVector> CArVaVectors;


	//-----------------------------


	/// �R���X�g���N�^
	CTmlVariableManager(void)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��
	 */
	CTmlVariableManager(const CTmlVariableManager& other) : m_aVava(other.m_aVava), m_aArva(other.m_aArva)
	{
	}

	/**
	 * [�擾] �ϐ����Q��.
	 *		�ϐ����Ɠ��e�̓񎟌��z��̎Q��
	 *	@return �ϐ����
 	 */
	CVaVaVectors& ReferVarArrays(void)
	{
		return m_aVava;
	}

	/**
	 * [�擾] �z����Q��.
	 *		�z�񖼂Ɠ��e�̓񎟌��z��̎Q��
	 *	@return �ϐ����
	 */
	CArVaVectors& ReferArrayArrays(void)
	{
		return m_aArva;
	}

	/**
	 * [�擾] ���݂̃��x���擾.
	 * @return ���݂̃��x��
	 */
	INT_PTR GetLevel(void) const
	{
		return m_aVava.GetSize();
	}

	/**
	 * [����] �ϐ����̌���
	 *		�ϐ����ɑ΂ɂȂ�l�Ǘ��̃|�C���^��Ԃ��܂��B
	 *	@param lpszName �ϐ���
	 *	@retval NULL �ϐ����Ȃ��B
	 *	@retval NULL�ȊO �l�Ǘ��̃|�C���^�B
	 */
	CTmlValue* FindValue(LPCSTR lpszName)
	{
		INT_PTR iDim1 = m_aVava.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			CVaVaVector& va = m_aVava.Ref(i);
			INT_PTR iDim2 = va.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				TVarValue& T = va.Ref(j);
				if ( T.name.Compare(lpszName) == 0 )
				{
					return &T.value;
				}
			}
		}
		return NULL;
	}

	/**
	 * [����] �z�񖼂̌���
	 *		�z�񖼂ɑ΂ɂȂ�z��Ǘ��̃|�C���^��Ԃ��܂��B
	 *	@param lpszName �ϐ���
	 *	@retval NULL �ϐ����Ȃ��B
	 *	@retval NULL�ȊO �z��Ǘ��̃|�C���^�B
	 */
	CTmlValuesVector* FindArray(LPCSTR lpszName)
	{
		INT_PTR iDim1 = m_aArva.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			CArVaVector& va = m_aArva.Ref(i);
			INT_PTR iDim2 = va.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				TArrayValue& T = va.Ref(j);
				if ( T.name.Compare(lpszName) == 0 )
				{
					return &T.values;
				}
			}
		}
		return NULL;
	}

	/** 
	 * [�ݒ�] Empty
	 */
	void Empty(void)
	{
		m_aVava.RemoveAll();
		m_aArva.RemoveAll();
	}

	/**
	 * [�ݒ�] ���x���{�P.
	 */
	void IncLevel(void)
	{
		m_aVava.Add(CVaVaVector());
		m_aArva.Add(CArVaVector());
		//
		TTRACE1("CTmlVariableManager::���x��++ NowLvl=%d\n", GetLevel());
	}

	/**
	 * [�ݒ�] ���x���|�P.
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool DecLevel(void)
	{
		INT_PTR l = m_aVava.GetSize();
		if ( l <= 0 )
		{
			return false;
		}
		#ifdef _DEBUG
			CAscii s;
			{
				CVaVaVector& va = m_aVava.Ref(l - 1);
				INT_PTR iDim = va.GetSize();
				for( INT_PTR i = 0; i < iDim; i++ )
				{
					s.Format( "    del-> �ϐ� %s (=%s)\n",
									va[i].name,
									va[i].value.GetString() 
					);
					::OutputDebugStringA( s );
				}
			}
			//
			{
				CArVaVector& va = m_aArva.Ref(l - 1);
				INT_PTR iDim = va.GetSize();
				for( INT_PTR i = 0; i < iDim; i++ )
				{
					s.Format( "    del-> �z�� %s(%d)\n",
									va[i].name,
									va[i].values.GetSize() 
					);
					::OutputDebugStringA( s );
				}
			}

		#endif
		//
		m_aVava.SetSize(l - 1);
		m_aArva.SetSize(l - 1);
		TTRACE1( "CTmlVariableManager::���x��-- NowLvl=%d\n", GetLevel() );
		return true;
	}

	/**
	 * [�ݒ�] ���x���w��.
	 *		���x�����w��̈ʒu�։�����B
	 *	@param level ���x���B		
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool DecLevels(INT_PTR level)
	{
		while( level < GetLevel() )
		{
			if ( ! DecLevel() )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [����] �����x���Ɏw�薼���邩.
	 *	@note �ϐ����A�z�񖼁A�����������܂��B
	 *	@param lpszName �������O�B
	 *	@retval true ����B
	 *	@retval false �Ȃ��B
	 */
	bool HasNameInNowLevel(LPCSTR lpszName) const
	{
		size_t l = m_aVava.GetSize();
		if ( l > 0 )
		{
			const CVaVaVector& va = m_aVava.At(l - 1);
			loop ( i, va.GetSize() )
			{
				if ( va[i].name.Compare(lpszName) == 0 )
				{
					return true;
				}
			}
			const CArVaVector& aa = m_aArva.At(l - 1);
			loop ( i, aa.GetSize() )
			{
				if ( aa[i].name.Compare(lpszName) == 0 )
				{
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * [�ǉ�] �ϐ����ǉ�.
	 *	@note ���̃��x���ɕϐ���ǉ����܂��B
	 *	@param lpszName �ϐ����B
	 *	@param v �����l�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddValue(LPCSTR lpszName, const CTmlValue& v)
	{
		size_t l = m_aVava.GetSize();
		if ( l == 0 )
		{
			return false;
		}
		TVarValue vv;
		vv.name = lpszName;
		vv.value = v;
		m_aVava.Ref(l - 1).Add(vv);
		return true;
	}

	/**
	 * [�ǉ�] �z����ǉ�.
	 *	@note ���̃��x���ɔz���ǉ����܂��B
	 *	@param lpszName �ϐ����B
	 *	@param size �z��T�C�Y
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddArray(LPCSTR lpszName, size_t size)
	{
		size_t l = m_aArva.GetSize();
		if ( l == 0 || size == 0 )
		{
			return false;
		}
		TArrayValue d;
		d.name = lpszName;
		d.values.SetSize(size);
		m_aArva.Ref(l - 1).Add(d);
		return true;
	}

private:
	CVaVaVectors		m_aVava;		///< �ϐ����Ɠ��e�̓񎟌��z��
	CArVaVectors		m_aArva;		///< �z�񖼂Ɠ��e�̓񎟌��z��
};



}; // TNB

