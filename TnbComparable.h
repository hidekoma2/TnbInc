#pragma once
/**
 *@file
 * ��r�@�\�֌W�̃w�b�_
 *
 *		��r���邽�߂̃C���^�[�t�F�[�X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * ��r�@�\�C���^�[�t�F�[�X.
 *
 *		���̃C���^�[�t�F�[�X���p�����A Compare() ���I�[�o�[���[�h���邱�ƂŁA
 *		���̌^�ɔ�r�@�\���������邱�Ƃ��o���܂�( == �� <= �ȂǂŔ�r�\)�B
 *
 *	@par�g�p��
 *		��r���ʂ� Compare() �Ɏ������܂��B
 *	@par
 *		<table><tr><td><pre>
 *
 *		struct TMyType : IComparableT<TMyType>
 *		{
 *		    int i;
 *		    INT_PTR Compare(const TMyType& t) const		
 *		    {
 *		        return i - t.i;
 *		    }
 *		};		
 *		</pre></td></tr></table>
 *
 *	@param TYP ��������N���X���w�肵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbComparable.h
 *
 *	@date 06/05/01 �V�K
 *	@date 06/09/13 IsEqual() �� virtual �錾�ɕύX�B
 */
template<typename TYP>
struct IComparableT
{
	/// �f�X�g���N�^
	virtual ~IComparableT(void) {}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const TYP& t) const = 0;

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval true ������ t �Ɠ���
	 *	@retval false ������ t �ƈقȂ�
	 */
	virtual bool IsEqual(const TYP& t) const { return Compare(t) == 0; }
	
	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	INT_PTR operator-(const TYP& t) const { return Compare(t); }
	
	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ���
	 *	@retval false	������ t �ƈقȂ�
	 */
	bool operator==(const TYP& t) const { return IsEqual(t); }
	
	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �ƈقȂ�
	 *	@retval false	������ t �Ɠ���
	 */
	bool operator!=(const TYP& t) const { return ! IsEqual(t); }
	
	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t ��菬����
	 *	@retval false	������ t �Ɠ������傫��
	 */
	bool operator<(const TYP& t) const { return Compare(t) < 0; }
	
	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ�����������
	 *	@retval false	������ t ���傫��
	 */
	bool operator<=(const TYP& t) const { return Compare(t) <= 0; }
	
	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t ���傫��
	 *	@retval false	������ t �Ɠ�����������
	 */
	bool operator>(const TYP& t) const { return Compare(t) > 0; }
	
	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval true	������ t �Ɠ������傫��
	 *	@retval false	������ t ��菬����
	 */
	bool operator>=(const TYP& t) const { return Compare(t) >= 0; }
};



};
