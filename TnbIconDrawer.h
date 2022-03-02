#pragma once
/**
 *@file
 * �A�C�R���`����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDrawable.h"



//TNB Library
namespace TNB
{




/**@ingroup DRAWABLE
 * �A�C�R���`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�A�C�R����`�悵�܂��B
 *
 *	@note �w�肳�ꂽ�A�C�R���n���h���́A�{�N���X�j�����ɉ������܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbIconDrawer.h
 *
 *	@date 08/04/18 �V�K�쐬
 *	@date 08/09/26 ���T�C�Y�����A�Ή�
 *	@date 14/10/15 GetSize() �Ńn���h�����[�N���Ă����B�C���B
 */
class CIconDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
protected:
	HICON 			m_icon;			///< �A�C�R��
	bool			m_canResize;	///< ���T�C�Y�\�H
	mutable	SIZE	m_size;			///< �T�C�Y
public:

	/**
	 * �R���X�g���N�^
	 *	@param icon �A�C�R���B
	 *	@param canResize ���T�C�Y�\�ɂ��邩�H true �Ȃ烊�T�C�Y�ɑΉ����܂��B
	 */
	CIconDrawer(HICON icon, bool canResize = true) : m_icon(icon), m_canResize(canResize)
	{
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CIconDrawer* p = new CIconDrawer(m_icon, m_canResize);
		p->m_size = m_size;
		return p;
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
		if ( m_size.cx == 0 )
		{
			ICONINFO ii;
			if ( ::GetIconInfo(m_icon, &ii) )
			{
				HBITMAP h = ii.hbmColor;
				if ( h == NULL )
				{
					h = ii.hbmMask;
				}
				BITMAP bm;
				if ( ::GetObject(h, sizeof(BITMAP), &bm) > 0 )
				{
					m_size.cx = bm.bmWidth;
					m_size.cy = bm.bmHeight;
				}
				::DeleteObject(ii.hbmColor);
				::DeleteObject(ii.hbmMask);
			}
		}
		_size = m_size;
		return m_size.cx != 0;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�T�C�Y�ύX�̓T�|�[�g���܂���B
	 */
	virtual bool Resize(const SIZE& size)
	{
		if ( m_canResize )
		{
			m_size = size;
			return true;
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
		SIZE sz;
		GetSize(sz);
		if ( m_canResize )
		{
			::DrawIconEx(dc, x, y, m_icon, sz.cx, sz.cy, 0, NULL, DI_NORMAL);
		}
		else
		{
			::DrawIconEx(dc, x, y, m_icon, 0, 0, 0, NULL, DI_NORMAL);
		}
	}
};



};