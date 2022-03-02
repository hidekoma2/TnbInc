#pragma once
/**
 *@file
 * �}�`�Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbVector.h"
#include "TnbPair.h"



//TNB Library
namespace TNB	
{



/**@ingroup VALUE COLLECT
 * �}�`�Ǘ��v�Z�N���X
 *
 *	@todo �������B
 *
 *		�A���������W�������A�}�`��\���܂��B
 *
 *	@param T �������Ǘ�����^�B
 *	@param TR �ʐς��Ǘ�����^�B�ȗ������ T ���w�肳��܂��B
 *
 *	@note �ʏ�A T �� double �� CValueDistance ��z�肵�Ă��܂��B 
 *			T �� CValueDistance �̏ꍇ�A TR �� CValueArea ��z�肵�Ă��܂��B
 *
 *	@note CVectorT �� TYP �� CPairT<T, T> �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFigurePointer.h
 * 
 *	@date 07/01/23 �V�K�쐬
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
template<typename T, typename TR = T>
class CFigurePointerT : public CVectorT<TNB::CPairT<T,T> >
{
	typedef CVectorT<TNB::CPairT<T,T> > _super; ///< �e�N���X���Ē�`
	DEFSUPER(_super);

	/**
	 * [�v�Z] �΂ߋ����v�Z (��(��������^2 + ��������^2))
	 *	@param x ��������
	 *	@param y ��������
	 *	@return �΂ߋ���
	 */
	friend double CalcSlantingDistance(const double& x, const double& y)
	{
		return ::sqrt(x * x + y * y);
	}

public:

	typedef TNB::CPairT<T,T> CPointer;	///< ���W�Ǘ��^ <BR> first = x���W <BR> second = y���W

	/// �R���X�g���N�^
	CFigurePointerT(void) : _super()
	{
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param t �v�f�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const CPointer& t)
	{
		return _super::Add(t);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A AddEx() ���g�p���Ă��������B
	 *	@param x x���W�B
	 *	@param y y���W�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX Add(const T& x, const T& y)
	{
		return _super::Add(CPointer(x, y));
	}
	
	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@param t �v�f�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX AddEx(const CPointer& t)
	{
		return _super::AddEx(t);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@param x x���W�B
	 *	@param y y���W�B
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	INDEX AddEx(const T& x, const T& y)
	{
		return _super::AddEx(CPointer(x, y));
	}
	
	/**
	 * [�擾] X���W�̎擾.
	 *	@param index �C���f�b�N�X
	 *	@return X���W�̕���
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	T X(INDEX index) const
	{
		return GetEx(index).first;
	}

	/**
	 * [�擾] Y���W�̎擾.
	 *	@param index �C���f�b�N�X
	 *	@return Y���W�̕���
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	T Y(INDEX index) const
	{
		return GetEx(index).second;
	}

	/**
	 * [����] �O�ڂ���l�p�`�쐬
	 *		�I�u�W�F�N�g�ɊO�ڂ���l�p�`�̍��W���擾���܂��B
	 *	@return Y�ŏ���Index
	 */
	int FindTop(void) const
	{
		int r = -1;
		T tMin(0);
		loop ( i, GetSize() )
		{
			const T& t = At(i).second;
			if ( tMin > t )
			{
				tMin = t;
				r = i;
			}
		}
		return r;
	}

	/**
	 * [�v�Z] �͈͓��ʐόv�Z.
	 *		�o�^���ꂽ���W��Top��Bottom���Ȃ��ň͂񂾖ʐς��v�Z���܂��B
	 *	@return �ʐ�(TR)
	 */
	TR CalcArea(void) const
	{
		EXCLUSIVE(this);
		TR rr;
		int size = GetSize();
		if ( size > 2 )
		{
			TR r(0);
			const CPointer* p = &At(0);
			const T& bx = p->first;
			const T& by = p->second;
			p = &At(1);
			for ( int i = 2; i < size; i++ )
			{
				const CPointer& a = At(i);
				r += (((p->first - bx) * (a.second - by)) - ((a.first - bx) * (p->second - by))) / 2;
				p = &a;
			}
			if ( r < TR(0) )
			{
				r *= -1;
			}
			rr = r;
		}
		return rr;
	}

	/**
	 * [�v�Z] �������v�Z.
	 *		�o�^���ꂽ���W���Ȃ����������v�Z���܂��B
	 *	@param boIsLoop true ���w�肷��ƍŏ��ƍŌ���Ȃ����������v�Z���܂��B
	 *	@return ����
	 */
	T CalcDistance(bool boIsLoop = false) const
	{
		EXCLUSIVE(this);
		T r(0);
		int size = GetSize();
		if ( size > 1 )
		{
			const CPointer* p = &At(0);
			for ( int i = 1; i < size; i++ )
			{
				const CPointer& a = At(i);
				r += CalcSlantingDistance(p->first - a.first, p->second - a.second);
				p = &a;
			}
			if ( boIsLoop )
			{
				const CPointer& a = At(0);
				r += CalcSlantingDistance(p->first - a.first, p->second - a.second);
			}
		}
		return r;
	}
};



}; //TNB


