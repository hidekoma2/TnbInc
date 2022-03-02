#pragma once
/**
 *@file
 * �R���p���[�^�֌W�̃w�b�_
 *
 *		��r���邽�߂̃C���^�[�t�F�[�X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbComparable.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * �R���p���[�^�C���^�[�t�F�[�X.
 *
 *		��r�@�\( IComparableT �̃T�u�N���X����{�^)�������Ȃ��^���m���r���邽�߂́A
 *		�C���^�[�t�F�[�X�ł��B
 *		��r������ CompareTo() �Ɏ������܂��B 
 *
 *		��r�@�\�������Ă���^�ł��A���̃C���^�[�t�F�[�X�̃T�u�N���X���쐬���A
 *		�w��\�ȃ��\�b�h�ɈϏ����邱�ƂŁA��r���@��ύX���邱�Ƃ��o���܂��B
 *
 *	@note �^�ɑ΂���f�t�H���g�̃R���p���[�^���쐬���邱�Ƃ��o���܂�( GetDefault() )�B
 *
 *	@param TYP �v�f�̌^�B
 *
 *	@par�K�v�t�@�C��
 *			TnbComparator.h
 *
 *	@date 06/07/04 IConstCollectionT ���番���B
 *	@date 06/09/13 IsEqualTo() �ǉ��B
 */
template<typename TYP>
struct IComparatorT
{
	/// �f�X�g���N�^
	virtual ~IComparatorT(void) {}

	/**
	 * [�m�F] ��r
	 *	@param t1 ��r�ΏۂP
	 *	@param t2 ��r�ΏۂQ
	 *	@retval �O		t1 �� t2 �Ɠ���
	 *	@retval �P�ȏ�	t1 �� t2 ���傫��
	 *	@retval �O����	t1 �� t2 ��菬����
	 */
	virtual INT_PTR CompareTo(const TYP& t1, const TYP& t2) const = 0;

	/**
	 * [�m�F] ��r
	 *	@param t1 ��r�ΏۂP
	 *	@param t2 ��r�ΏۂQ
	 *	@retval true t1 �� t2 �Ɠ���
	 *	@retval false t1 �� t2 �ƈقȂ�
	 */
	virtual bool IsEqualTo(const TYP& t1, const TYP& t2) const
	{
		return CompareTo(t1, t2) == 0;
	}

	/**
	 * [�쐬] �ėp�R���p���[�^�擾.
	 *		TYP �p�̃R���p���[�^���쐬���܂��B
	 *	@note	��{�^�A IComparatorT �����^�A CStr�n�^�ȊO�̏ꍇ�A
	 *			��r���Ă��u�����v��Ԃ��R���p���[�^���쐬����Ă��܂��܂��B
	 *			���̏ꍇ�A�{���\�b�h���g�킸�A IComparatorT ���������č쐬���Ă��������B
	 *	@return �ėp�R���p���[�^
	 */
	static IComparatorT& GetDefault(void);
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��



/**@ingroup VALUE
 * ���Q�Ǘ��C���^�[�t�F�[�X�̃R���p���[�^�[�����쐬�N���X.
 */
template<typename TYP>
class CAutoComparatorT
{
	// IComparableT�C���^�[�t�F�[�X�����^�p
	template<typename T>
	struct TMyCompCompareT : IComparatorT<TYP>
	{
		INT_PTR CompareTo(const TYP& t1, const TYP& t2) const
		{
			//�|�C���g;�b�^�L���X�g��p���邱�ƂŌ^�ɉ������L���X�g���s����B
			return ((const IComparableT<T>*)&t1)->Compare(t2);
		}
		bool IsEqualTo(const TYP& t1, const TYP& t2) const
		{
			//�|�C���g;�b�^�L���X�g��p���邱�ƂŌ^�ɉ������L���X�g���s����B
			return ((const IComparableT<T>*)&t1)->IsEqual(t2);
		}
	};

	// ���̑��̌^�p
	struct TMyCompElse : IComparatorT<TYP>
	{
		// SubSub �|�C���^��r
		template<typename T>
		INT_PTR m_SubSub(const T& t, LPCVOID t1, LPCVOID t2) const
		{
			return static_cast<const BYTE *>(t1) - static_cast<const BYTE *>(t2);
		}

		// SubSub ��r�ł��Ȃ�
		template<typename T>
		INT_PTR m_SubSub(const T& t, ...) const
		{
 #ifndef _WIN32_WCE
			ASSERT1(
				false,
				"�I�[�gComparator��",
				"��r�@�\�̂Ȃ�[%s]�^�� IComparator�w�薳���Ŕ�r���܂����B���ʂ͏�� EVEN �ɂȂ�܂��B",
				typeid(T).name()
			);
 #else
			ASSERT0(
				false,
				"�I�[�gComparator��",
				"��r�@�\�̂Ȃ��^�� IComparator�w�薳���Ŕ�r���܂����B���ʂ͏�� EVEN �ɂȂ�܂��B"
			);
 #endif // _WIN32_WCE
			return 0;
		}

		// <Sub> ���̑��̌^�B SubSub�Ɉ˗�
		template<typename T>
		INT_PTR m_Sub(const T& t1, const T& t2) const
		{
			return m_SubSub(t1, t1, t2);
		}
		
		// <Sub> CAscii�^��
		template<> INT_PTR m_Sub(const CAscii& i1, const CAscii& i2) const
		{
			return i1.Compare(i2);
		}

		// <Sub> CUnicode�^��
		template<> INT_PTR m_Sub(const CUnicode& i1, const CUnicode& i2) const
		{
			return i1.Compare(i2);
		}

		// <Sub> ��{�^��
		#define _COLLECTCOMP(TY)											\
			template<> INT_PTR m_Sub(const TY& i1, const TY& i2) const {	\
				if ( i1 == i2 ){ return 0; }								\
				return (i1 > i2) ? 1 : -1;									\
			}
		_COLLECTCOMP(bool)
		_COLLECTCOMP(BYTE)
		_COLLECTCOMP(char)
		_COLLECTCOMP(WORD)
		_COLLECTCOMP(short)
		_COLLECTCOMP(DWORD)
		_COLLECTCOMP(long)
		_COLLECTCOMP(UINT)
		_COLLECTCOMP(int)
		_COLLECTCOMP(ULONGLONG)
		_COLLECTCOMP(LONGLONG)
		_COLLECTCOMP(long double)
		_COLLECTCOMP(double)
		_COLLECTCOMP(float)
		#ifdef __AFX_H__
			_COLLECTCOMP(CString)
		#endif
		#undef _COLLECTCOMP

		// ��r�֐�
		INT_PTR CompareTo(const TYP& t1, const TYP& t2) const
		{
			return m_Sub(t1, t2);
		}
	};

public:

	/**
	 * [�쐬] IComparableT�p�R���p���[�^�[�����쐬.
	 *	@param P �_�~�[
	 *	@return �R���p���[�^�[
	 */
	template<typename T>
	static IComparatorT<TYP>& MakeAutoComp(const IComparableT<T>* P)
	{
		static TMyCompCompareT<T> s_comp;
		return s_comp;
	}

	/**
	 * [�쐬] �ėp�R���p���[�^�[�����쐬.
	 *	@param P �_�~�[
	 *	@return �R���p���[�^�[
	 */
	static IComparatorT<TYP>& MakeAutoComp(LPCVOID P)
	{
		static TMyCompElse s_comp;
		return s_comp;
	}
};

#endif	



template<typename TYP>
inline IComparatorT<TYP>& IComparatorT<TYP>::GetDefault(void)
{
	return CAutoComparatorT<TYP>::MakeAutoComp(static_cast<TYP*>(NULL));
};



};