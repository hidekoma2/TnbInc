#pragma once
/**
 *@file
 * �o���N�`����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDrawable.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * �o���N�`����N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		LPARAM �ɂ��A�`������Ǘ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBankDrawable.h
 *
 *	@date 08/10/06 �V�K�쐬
 */
class CBankDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
public:

	/// �R���X�g���N�^
	CBankDrawer(void)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param draw LPARAM 0 �̕`����B������{�N���X�ŕێ����܂��B
	 */
	CBankDrawer(const IDrawable& draw)
	{
		m_phDraws.Add(draw.Clone());
	}

	/**
	 * [�ǉ�] �ǉ�
	 *	@param draw �`����
	 *	@return �����̎Q��
	 */
	CBankDrawer& Add(const IDrawable& draw)
	{
		m_phDraws.Add(draw.Clone());
		return *this;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CBankDrawer* P = new CBankDrawer(*this);
		return P;
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		if ( m_phDraws.GetSize() > 0 )
		{
			return m_phDraws[0]->GetSize(_size);
		}
		return false;
	}
	
	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note �{�N���X�̓T�C�Y�ύX���T�|�[�g���܂���B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�T�C�Y�ύX�̓T�|�[�g���܂���B
	 */
	virtual bool Resize(const SIZE& size)
	{
		size_t num = m_phDraws.GetSize();
		if ( num > 0 )
		{
			bool r = true;
			loop ( i, num )
			{
				r &= m_phDraws[i]->Resize(size);
			}
			return r;
		}
		return false;
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		DrawEx(dc, x, y, 0);
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 *	@param lParam LPARAM�B�Ǘ����Ă���`����ɓn���܂��B
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		if ( m_phDraws.GetSize() <= static_cast<size_t>(lParam) )
		{
			lParam = 0;
		}
		m_phDraws[lParam]->DrawEx(dc, x, y, lParam);
	}
protected:
	CSimpleVectorT<IDrawable::Ptr>	m_phDraws;	///< �o���N�Ǘ�
};



};///TNB

