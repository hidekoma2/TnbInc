#pragma once
/**
 *@file
 * HASH�l�ێ��֌W�̃w�b�_
 *
 *		�ʏ�̃I�u�W�F�N�g��HASH�l��t������e���v���[�g�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSerializer.h"
#include "TnbComparator.h"



namespace TNB
{



/**@ingroup VALUE
 * HASH�l�ێ��e���v���[�g�N���X.
 *
 *		�ʏ�̃I�u�W�F�N�g��HASH�l��t������e���v���[�g�ł��B��ɁA�傫�Ȍ^���R���N�V������
 *		�g�p����ꍇ�A�n�b�V����t�����邱�ƂŁA�������xUP�����҂ł��܂��B
 *
 *	@par�n�b�V���l�F
 *		HASH�l�͈ȉ��̓���������܂��BHashCalc() ���쐬����ꍇ�A�ȉ��̋K���ɏ]���Ă��������B 
 *
 *		<table><tr><td><pre>
 *		�v���Z�X���s���ɁA�ω��̂Ȃ��I�u�W�F�N�g�́A���̐�����Ԃ��B
 *
 *		�������e�̃I�u�W�F�N�g�ł��A�قȂ�v���Z�X�Ȃ瓯�������Ƃ͌���Ȃ��B
 *
 *		�Q�̃I�u�W�F�N�g�̓��e���قȂ�ꍇ�A���ꂼ��̃n�b�V���l�͕K����v���Ȃ��B
 *
 *		�Q�̃I�u�W�F�N�g�̓��e�������ꍇ�ł��A���ꂼ��̃n�b�V���l����v����Ƃ͌���Ȃ��B	
 *		�������A�ɗ͓����l�ɂȂ�悤�ȃn�b�V���l�ł��邱�Ƃ��D�܂����B
 *
 *		�قȂ�^���m�ł́A�n�b�V���l�͈Ӗ��������Ȃ��B 
 *		</pre></td></tr></table>
 *
 *	@par�g�p��
 *		CStr �n�� CalcHash() ���\�b�h�����ɗp�ӂ��Ă��邽�߈ȉ��̂悤�g�p�ł��܂��B
 *		<table><tr><td><pre>
 *
 *		CVectorT< CHashableT<CStr> > vhstr;		//�錾
 *				;
 *		int f1 = vhstr.Find(CStr("ABCDE"));		
 *		</pre></td></tr></table>
 *
 *	@par�n�b�V���l�v�Z�쐬��
 *		���̂��̂��g�p�������ꍇ�A�ȉ��̂悤�� CalcHash() ���\�b�h���A TNB::HASH �l�[���X�y�[�X����
 *		�쐬���܂��B
 *		<table><tr><td><pre>
 *
 *		namespace TNB
 *		{
 *		  namespace HASH
 *		  {
 *		    // �n�b�V���l�v�Z.
 *		    inline DWORD CalcHash(const TFoo& str)
 *		    { �` }
 *		  };
 *		};
 *		</pre></td></tr></table>
 *
 *	@note ISerializable �������Ă��܂��B�V���A���C�Y�\�ł��B
 *
 *	@param TYP �v�f�̌^�B
 *
 *	@par�K�v�t�@�C��
 *			TnbHashable.h
 *
 *	@date 06/09/12 �V�K
 */
template<typename TYP>
class CHashableT : public IComparableT< TNB::CHashableT<TYP> >, public ISerializable
{
	DWORD	m_dwHash;	///< �n�b�V���l
	TYP		m_typ;		///< �l

public:

	/**
	 * �R���X�g���N�^.
	 */
	CHashableT(void) : m_typ(TYP()), m_dwHash(HASH::CalcHash(m_typ))
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@note �R�s�[���̃n�b�V���l�������p���܂��B
	 *	@param t �R�s�[��
	 */
	CHashableT(const CHashableT<TYP>& t) : m_typ(t.m_typ), m_dwHash(t.m_dwHash)
	{
	}

	/**
	 * [���] ����R���X�g���N�^
	 *	@note �v�f�̒l����n�b�V���l�̌v�Z�����Ă��܂��B
	 *	@param t �v�f
	 */
	CHashableT(const TYP& t) : m_typ(t)
	{
		m_dwHash = HASH::CalcHash(m_typ);
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@note �R�s�[���̃n�b�V���l�������p���܂��B
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CHashableT& operator=(const CHashableT<TYP>& other)
	{
		m_typ = other.m_typ;
		m_dwHash = other.m_dwHash;
		return *this;
	}

	/**
	 * [���] ����I�y���[�^
	 *	@note �v�f�̒l����n�b�V���l�̌v�Z�����Ă��܂��B
	 *	@param t �v�f
	 *	@return �����̎Q��
	 */
	CHashableT& operator=(const TYP& t)
	{
		m_typ = t;
		m_dwHash = HASH::CalcHash(m_typ);
		return *this;
	}

	/**
	 * [�擾] HASH�l�擾
	 *	@return �n�b�V���l
	 */
	DWORD GetHashCode(void) const
	{
		return m_dwHash;
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CHashableT<TYP>& t) const
	{
		return IComparatorT<TYP>::GetDefault().CompareTo(m_typ, t.m_typ);
	}

	/**
	 * [�m�F] ��r
	 *	@note �܂��A�n�b�V���l�̔�r���s���A�����Ȃ���ۂ̒l���r���܂��B
	 *	@param t ��r�Ώ�
	 *	@retval true ������ t �Ɠ���
	 *	@retval false ������ t �ƈقȂ�
	 */
	virtual bool IsEqual(const CHashableT<TYP>& t) const 
	{
		if ( m_dwHash != t.m_dwHash )
		{
			return false;	//�n�b�V���l���قȂ�̂ŁA��ΈႤ
		}
		return IComparatorT<TYP>::GetDefault().IsEqualTo(m_typ, t.m_typ);
	}

	/**
	 * [�擾] �l�Q�Ǝ擾
	 *	@return �v�f�̎Q��
	 */
	const TYP& At(void) const
	{
		return m_typ; 
	}

	/**
	 * [�擾] �l�Q�Ǝ擾
	 *	@return �v�f�̎Q��
	 */
	operator const TYP&(void) const
	{
		return m_typ; 
	}

	/**
	 * [����] �V���A���C�Y.
	 *	@note �w��̃V���A���C�U�[�ɖ{�I�u�W�F�N�g�̕ێ����Ă�������o�͂��܂��B
	 *	@param[out] _sr �V���A���C�U�[
	 */
	virtual void Serialize(ISerializer& _sr) const	
	{
		_sr << m_typ;
	}

	/**
	 * [����] �f�V���A���C�Y.
	 *	@note �w��̃f�V���A���C�U�[����ɖ{�I�u�W�F�N�g�ɏ�����荞�݂܂��B
	 *	@param[in] ds �f�V���A���C�U�[
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> m_typ;
		m_dwHash = HASH::CalcHash(m_typ);
	}
};



namespace HASH
{
	/**
	 * [HASH] �n�b�V���l�v�Z.
	 *	@note �{ DWORD �p�́A�e�X�g�B
	 *	@param dw
	 *	@return �n�b�V���l
	 */
	inline DWORD CalcHash(DWORD dw)
	{
		return dw;
	}
};


}; // TNB
