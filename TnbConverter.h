#pragma once
/**
 *@file
 * ���ϊ��֌W�̃w�b�_
 *
 *		����^���炠��^�ɕϊ����邽�߂̃N���X������܂��B
 *		�R���N�V�����̗v�f���A�ꊇ�ŕϊ����邱�Ƃ��o���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"
#include "TnbPair.h"



namespace TNB
{



/**@ingroup COLLECT
 * ���ϊ��x�[�X�N���X
 *
 *		�w��� SRC�^ �� �w��� DST�^�ɕϊ�����N���X�̃x�[�X�ł��B
 *		Converter() �� �I�[�o�[���C�h���āA���҂���ϊ��N���X��錾���g�p���܂��B
 *
 *	@par�g�p��
 *		\code
 *		class CMyConv : public CConverterT<int, CStr> 
 *		{
 *			//CStr��int�̃R���o�[�^
 *			virtual void  Converter(int& r, const CStr& src)			
 *			{
 *				r = src.ToInt();
 *			}
 *	 	};
 *		\endcode
 *
 *		SRC �^������ IConstCollectionT ��ADST�^�����A ICollectionT ���w�肷�邱�ƂŁA
 *		�R���N�V�������̏���S�ĕϊ����邱�Ƃ��o���܂�(���ꂪ�{�N���X�̖ړI)�B
 *
 *		�܂��A�R���N�V�����̃l�X�g���ϊ��\�ł��B
 *
 *		�Ȃ��A CMapT �̏ꍇ�A�L�[�͌^�͓����A�l�̌^�� SRC �� DST �̂��̂��w�肵�܂��B
 *		
 *	@par�g�p��
 *		\code
 *		CVectorT<CMapT<char, int> > vmapi;
 *		CVectorT<CMapT<char, CStr> > vmaps;
 *				;
 *		CMyConv().Convert(vmapi, vmaps);	///CStr��int�̃R���o�[�^�ŕϊ��\
 *		\endcode
 *	
 *	@note SRC,DST �ɁA �R���N�V�����^ �� �y�A�^ ���w��ł��܂���B
 *
 *	@param DST �ϊ���̌^
 *	@param SRC �ϊ��O�̌^
 *
 *	@par�K�v�t�@�C��
 *			TnbConverter.h
 *
 *	@date 06/09/12 �V�K�쐬�B
 *	@date 16/06/16 �o�O�C���B
 */
template<typename DST, typename SRC>
class CConverterT
{
protected:

	/**
	 * [�ϊ�] �ϊ���
	 *	@note ������SRC�^����DST�^���쐬���郁�\�b�h����������
	 *	@param[out] _dst �ϊ���
	 *	@param[in] src �ϊ���
	 *	@throw CTnbException �ϊ����s�\�̏ꍇ�A�X���[���邱�Ƃ��o���܂��B
	 */
	virtual void Converter(DST& _dst, const SRC& src) = 0;

public:

	/**
	 * [�ϊ�] �P�v�f�ϊ�
	 *	@param[out] _dst �ϊ���
	 *	@param[in] src �ϊ���
	 *	@throw CTnbException �ϊ����s�\���������߂ɁA�X���[����邱�Ƃ�����܂��B
	 */
	void Convert(DST& _dst, const SRC& src)
	{
		Converter(_dst, src);
	}

	/**
	 * [�ϊ�] �R���N�V�����ϊ�
	 *	@param[out] _dst �ϊ���
	 *	@param[in] src �ϊ���
	 *	@throw CEmptyException �R���N�V�����ɒǉ��s�\�ł��邱�Ƃ��������߂ɁA�X���[����邱�Ƃ�����܂��B
	 *	@throw CTnbException �ϊ����s�\���������߂ɁA�X���[����邱�Ƃ�����܂��B
	 */
	template<typename D, typename S>
	void Convert(ICollectionT<D>& _dst, const IConstCollectionT<S>& src)
	{
		EXCLUSIVE2(&_dst, &src);
		size_t l = src.GetSize();
		if ( l == 0 )
		{
			_dst.RemoveAll();
			return;
		}
		bool canRefMode = true;
		try
		{
			if ( _dst.SetSize(l) )
			{
				_dst.Ref(0);
			}
			else
			{
				canRefMode = false;
			}
		}
		catch(CNotSupportException& e)
		{
			e.OnCatch();
			canRefMode = false;
		}
		//
		if ( canRefMode )
		{
			loop ( i, l )
			{
				Convert(_dst.Ref(i), src.At(i)); 
			}
		}
		else
		{
			_dst.RemoveAll();
			D d;
			loop ( i, l )
			{
				Convert(d, src.At(i));
				if ( _dst.Add(d) == INVALID_INDEX )
				{
					throw CEmptyException();
				}
			}
		}
	}

	/**
	 * [�ϊ�] �y�A�^�ϊ�.
	 *	@note �l�̌^�̂ݕϊ����܂��B�L�[�͓������e�ɂȂ�܂��B
	 *	@param[out] _dst �ϊ���
	 *	@param[in] src �ϊ���
	 *	@throw CTnbException �ϊ����s�\���������߂ɁA�X���[����邱�Ƃ�����܂��B
	 */
	template<typename T, typename D, typename S>
	void Convert(CPairT<T, D>& _dst, const CPairT<T, S>& src)
	{
		_dst.first = src.first;
		Convert(_dst.second, src.second);
	}
};



}; // TNB
