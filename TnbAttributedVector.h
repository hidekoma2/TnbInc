#pragma once
/**
 *@file
 * �z��^���Ǘ��֌W�̃w�b�_
 *
 *		�^�̔z��i�A�������������z�u�j���Ǘ����܂��B
 *		��������A�����Ĕz�u���Ă��邽�߁A����API�ɔz��Ƃ��ēn���̂��e�Ղł��B
 *
 *		���I�ɔz��𑝂₷���Ƃ��ł��܂�(�������̍Ĕz�u����������\��������܂�)�B
 *		�e�v�f�Ƀp�����[�^(����)�����邱�Ƃ��\�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * �z��^���Ǘ��e���v���[�g
 *
 *		�^�̔z����Ǘ����܂��B
 *
 *	@note	�e�v�f�ɑ����l�i�C�ӂ̌^�j����ɂ��邱�Ƃ��o���܂��B
 *			���̒l�́AAppend�n �� Copy�n ���g���Ēǉ��A��������ƁA ATT() �ɂȂ�܂��B
 *			List ���m�̘A���A�����́A�I�y���[�^���g���Ă��������B
 *
 *	@note	AttributeVector �̃V���A���C�Y�A�f�V���A���C�Y�́A�K�� AttributeVector
 *			�ōs���Ă��������B������񂪂��邽�߁A���̃R���N�V�����ƌ݊����L��܂���B
 *
 *	@param TYP �ێ�����^�i�R�s�[�\�̌^�Ɍ���j
 *	@param ATT TYP �Ƒ΂ɕێ����鑮���^�i�R�s�[�\�̌^�Ɍ���j
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbAttributedVector.h
 *
 *	@date 07/06/19 �V�K�쐬
 *	@date 10/02/03 �\���ύX
 *	@date 11/04/19 SetElements() �̃I�[�o�[���C�h�B�������C���B
 */
template<typename TYP, typename ATT = DWORD>
class CAttributedVectorT : public CVectorT<TYP>
{
	DEFSUPER(CVectorT<TYP>);
public:

	/// �R���X�g���N�^
	CAttributedVectorT(void) : _super()
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CAttributedVectorT(const CAttributedVectorT& other) : _super(other), m_vAttr(other.m_vAttr)
	{
	}

	/// �f�X�g���N�^
	virtual ~CAttributedVectorT(void)
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CAttributedVectorT& operator=(const CAttributedVectorT& other)
	{
		_super::operator=(other);
		m_vAttr = other.m_vAttr;
		return *this;
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^.
	 *		���C���X�^���X�ŊǗ����Ă���f�[�^�Ɏw��̃f�[�^��
	 *		�ǉ����܂��B
	 *	@param other �ǉ��f�[�^
	 *	@return �����̎Q��
	 */
	CAttributedVectorT& operator+=(const CAttributedVectorT& other)
	{
		_super::operator+=(other);
		m_vAttr += other.m_vAttr;
		return *this;
	}

	/**
	 * [�쐬] �A��
	 *	@note	���C���X�^���X�ŊǗ����Ă���f�[�^�Ɏw��̃f�[�^��
	 *			�A�������A CVectorT ���쐬���܂��B
	 *			���C���X�^���X�͕ω����܂���B
	 * @param other �A������f�[�^
	 * @return �V�K�C���X�^���X
	 */
	CAttributedVectorT operator+(const CAttributedVectorT& other) const
	{
		CAttributedVectorT av;
		CVectorT<TYP>& v1 = av;
		const CVectorT<TYP>& v2 = *this;
		v1 = v2 + other;
		av.m_vAttr = m_vAttr + other.m_vAttr;
		return av;
	}

	/**
	 * [�擾] �f�[�^�Q�Ǝ擾.
	 *	@return �f�[�^�z��̎Q��
	 */
	virtual const CVectorT<TYP>& Refer(void) const
	{
		return *this;
	}

	/**
	 * [�폜] �v�f��폜
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval true	�폜����
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	virtual bool Remove(INDEX index)
	{
		return _super::Remove(index) && m_vAttr.Remove(index);
	}

	/**
	 * [�ݒ�] �����v�f�ݒ�.
	 *		�Ǘ����Ă���o�b�t�@���J�����A�w��̃f�[�^���R�s�[���܂��B
	 *	@param size �f�[�^���B
	 *	@param P �R�s�[������e���w��B�ȗ������ TYP()���������܂��B
	 *	@retval INVALID_SIZE	�G���[�B
	 *	@retval ����ȊO		�����������B
	 */
	virtual size_t SetElements(size_t size, const TYP* P = NULL)
	{
		size_t r = _super::SetElements(size, P);
		m_vAttr.RemoveAll();
		m_vAttr.SetSize(_super::GetSize());
		return r;
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A SetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		return Set(index, t, 0);
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool SetEx(INDEX index, const TYP& t)
	{
		return SetEx(index, t, 0);
	}

	/**
	 * [�폜] �S�v�f�폜 
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool RemoveAll(void)
	{
		return _super::RemoveAll() && m_vAttr.RemoveAll();
	}

	/**
	 * [�ݒ�] �v�f�̓���ւ�
	 *	@note �|�C���^���������ւ��Ă��܂��B�v�f�̃R�s�[�����͔������܂���B
	 *	@param index1 �C���f�b�N�X�P
	 *	@param index2 �C���f�b�N�X�Q
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual void Swap(INDEX index1, INDEX index2)
	{
		_super::Swap(index1, index2);
		m_vAttr.Swap(index1, index2);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �v�f�B
	 *	@param attribute �����l�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX Add(const TYP& t, const ATT& attribute)
	{
		INDEX r1 = _super::Add(t);
		INDEX r2 = m_vAttr.Add(attribute);
		if ( IsInvalid(r1)  || IsInvalid(r2) )
		{
			return INVALID_INDEX;
		}
		ASSERTLIB( r1 == r2 );
		return r1;
	}

	/**
	 * [�ǉ�] �v�f��ǉ�
	 *	@param t �v�f�B
	 *	@param attribute �����l�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX AddEx(const TYP& t, const ATT& attribute)
	{
		EXCLUSIVE(this);
		return Add(t, attribute);
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A SetEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@param attribute �����l�B
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	bool Set(INDEX index, const TYP& t, const ATT& attribute)
	{
		return _super::Set(index, t) && m_vAttr.Set(index, attribute);
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@param attribute �����l�B
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	bool SetEx(INDEX index, const TYP& t, const ATT& attribute)
	{
		EXCLUSIVE(this);
		return Set(index, t, attribute);
	}

	/**
	 * [�擾] �v�f�̑����l�A�擾
	 *	@param index �C���f�b�N�X
	 *	@return �����l
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	ATT GetAttribute(INDEX index) const
	{
		EXCLUSIVE(this);
		return m_vAttr.Get(index);
	}

	/**
	 * [�ݒ�] �v�f�̑����l�A�ݒ�
	 *	@param index �C���f�b�N�X
	 *	@param attribute �����l�B
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	bool SetAttribute(INDEX index, const ATT& attribute)
	{
		return m_vAttr.SetEx(index, attribute);
	}

	/**
	 * [�ǉ�] �v�f��}��.
	 *		�w��̃C���f�b�N�X�ɗv�f��ǉ����܂��B
	 *	@param index �C���f�b�N�X
	 *	@param t �v�f
	 *	@param attribute �����l�B
	 *	@retval true	�}������
	 *	@retval false	���s�i�C���f�b�N�X�����j
	 */
	bool Insert(INDEX index, const TYP& t, const ATT& attribute = ATT())
	{
		EXCLUSIVE(this);
		return _super::Insert(inex, t) && m_vAttr.Insert(index, attribute);
	}

	/**
	 * [����] �T�C�Y�w��
	 *	@note �傫�����邱�Ƃ����������邱�Ƃ��o���܂��B
	 *	@note �傫���������A���̃f�[�^��TYP()�Ŗ��߂��܂��B
	 *	@param size �T�C�Y
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool SetSize(size_t size)
	{
		return _super::SetSize(size) && m_vAttr.SetSize(size);
	}

	/**
	 * [����] �V���A���C�Y
	 *	@note CAttributeVectorT �̃V���A���C�Y�f�[�^�́A�K���A����CAttributeVectorT�Ńf�V���A���C�Y���Ă��������B
	 *	@param[out] _sr �V���A���C�U�[�B ����ɑ΂������o�̓��e��ǉ����Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_super::Serialize(_sr);
		_sr << m_vAttr;
	}

	/**
	 * [����] �f�V���A���C�Y
	 *	@note CAttributeVectorT �̃V���A���C�Y�f�[�^�́A�K���A���� CAttributeVectorT �Ńf�V���A���C�Y���Ă��������B
	 *	@param ds �f�V���A���C�U�[�B���ꂩ�烁���o�̓��e�����o���Ă��������B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		_super::Deserialize(ds);
		ds >> m_vAttr;
	}

private:
	CVectorT<ATT>		m_vAttr;	///< �����̔z��
};



}; // TNB
