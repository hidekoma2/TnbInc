#pragma once
/**
 *@file
 * NULL��ԗL��^�֌W�̃w�b�_
 *
 *		NULL��Ԃ������Ȃ��^��NULL��Ԃ�t������e���v���[�g�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSerializer.h"
#include "TnbComparator.h"



namespace TNB
{



/**@ingroup VALUE
 * NULL��ԗL�艻�e���v���[�g�N���X.
 *
 *		NULL��Ԃ������Ȃ��^��NULL��Ԃ�ێ��ł���@�\��t������e���v���[�g�ł��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *
 *			CNullableT<int> x;
 *			x = 5;
 *			int i = x;
 *			x++;
 *			bool bbbb = (x.At() < 5);		//�v�f�̌^�Ƃ̔�r���́AAt()���g�p
 *			x.IsNull();						//NULL��
 *		</pre></td></tr>
 *
 *	@note ISerializable �������Ă��܂��B�R���N�V�����̗v�f�Ɏg�p���Ă��V���A���C�Y���\�ł��B
 *
 *	@param TYP �v�f�̌^�B
 *
 *	@par�K�v�t�@�C��
 *			TnbNullable.h
 *
 *	@date 06/09/12 �V�K
 */
template<typename TYP>
class CNullableT : public IComparableT< TNB::CNullableT<TYP> >, public ISerializable
{
	bool	m_boIsNull;
	TYP		m_typ;

public:

	/**
	 * �R���X�g���N�^.
	 *	@note NULL��Ԃ̃I�u�W�F�N�g�ɂȂ�܂��B
	 */
	CNullableT(void) : m_typ(TYP()) , m_boIsNull(true) 
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param t �R�s�[��
	 */
	CNullableT(const CNullableT<TYP>& t) : m_typ(t.m_typ) , m_boIsNull(t.m_boIsNull) 
	{
	}

	/**
	 * [���] ����R���X�g���N�^
	 *	@param t �v�f
	 */
	CNullableT(const TYP& t) : m_typ(t) , m_boIsNull(false) 
	{
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CNullableT& operator=(const CNullableT& other)
	{
		m_typ = other.m_typ;
		m_boIsNull = other.m_boIsNull;
		return *this;
	}

	/**
	 * [���] ���
	 *	@param t �v�f
	 *	@return �����̎Q��
	 */
	CNullableT& operator=(const TYP& t)
	{
		m_typ = t;
		m_boIsNull = false;
		return *this;
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CNullableT<TYP>& t) const
	{
		if ( m_boIsNull )
		{
			return t.m_boIsNull ? 0 : -1;
		}
		if ( t.m_boIsNull ){ return 1; } 
		return IComparatorT<TYP>::GetDefault().CompareTo(m_typ, t.m_typ);
	}

	/**
	 * [�擾] �Q�Ǝ擾
	 *	@return �v�f�̎Q�ƁB�ύX����ƁA�{�C���X�^���X�ɉe��������܂��B
	 *	@throw CNullPointerException NULL��Ԃł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP& At(void)
	{
		if ( m_boIsNull ){ throw CNullPointerException(); }
		return m_typ;
	}

	/**
	 * [�擾] �l�Q�Ǝ擾
	 *	@return �v�f�̎Q��
	 *	@throw CNullPointerException NULL��Ԃł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& At(void) const
	{
		if ( m_boIsNull ){ throw CNullPointerException(); }
		return m_typ; 
	}

	/**
	 * [�擾] �Q�Ǝ擾
	 *	@return �v�f�̎Q�ƁB�ύX����ƁA�{�C���X�^���X�ɉe��������܂��B
	 *	@throw CNullPointerException NULL��Ԃł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	operator TYP&(void) { return At(); }

	/**
	 * [�擾] �l�Q�Ǝ擾
	 *	@return �v�f�̎Q��
	 *	@throw CNullPointerException NULL��Ԃł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	operator const TYP&(void) const { return At(); }

	/**
	 * [�擾] �l�擾
	 *	@return �v�f
	 *	@throw CNullPointerException NULL��Ԃł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP operator+(void) const
	{
		return At(); 
	}
	
	/**
	 * [�擾] �}�C�i�X�P��
	 *	@return �}�C�i�X�P���̌���
	 *	@throw CNullPointerException NULL��Ԃł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP operator-(void) const
	{
		return -At(); 
	}

	/**
	 * [�m�F] NULL��Ԋm�F
	 *	@retval true NULL��ԁB���̏�Ԃŗv�f�ɃA�N�Z�X����ƁA�X���[����܂��B
	 *	@retval false ��NULL��ԁB
	 */
	bool IsNull(void) const { return m_boIsNull; }

	/**
	 * [�ݒ�] NULL��ԉ�
	 */
	void Null(void)
	{
		m_typ = TYP();
		m_boIsNull = true; 
	}

	/**
	 * [����] �V���A���C�Y.
	 *	@note �w��̃V���A���C�U�[�ɖ{�I�u�W�F�N�g�̕ێ����Ă�������o�͂��܂��B
	 *	@param[out] _sr �V���A���C�U�[
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << m_boIsNull << m_typ;
	}

	/**
	 * [����] �f�V���A���C�Y.
	 *	@note �w��̃f�V���A���C�U�[����ɖ{�I�u�W�F�N�g�ɏ�����荞�݂܂��B
	 *	@param[in] ds �f�V���A���C�U�[
	 */
	virtual void Deserializer(const IDeserializer& ds)
	{
		ds >> m_boIsNull >> m_typ;
	}
};



}; // TNB
