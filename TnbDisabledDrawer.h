#pragma once
/**
 *@file
 * Disabled�`��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * Disabled�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�`����� Disabled���ɕ`�悵�܂��B
 *	
 *	@note �`�悵���������O���[�ɂȂ�܂��̂ŁA �A�C�R���╶���Ȃǂ̕`�����p�ӂ���K�v������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDisabledDrawer.h
 *
 *	@date 08/05/21 �V�K�쐬
 */
class CDisabledDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
	IDrawable::Ptr	m_pDraw;	///< �`����
	/// �R�[���o�b�N
	static BOOL CALLBACK s_DrawIconProc(HDC hDC, LPARAM lParam, WPARAM wParam, int cx, int cy)
	{
		const CDisabledDrawer* P = reinterpret_cast<const CDisabledDrawer*>(lParam);
		P->m_pDraw->Draw(hDC, 0, 0);
		return TRUE;
	}
public:

	/**
	 * �R���X�g���N�^
	 *	@param draw �`����B
	 */
	CDisabledDrawer(const IDrawable& draw)
	{
		m_pDraw = draw.Clone();
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CDisabledDrawer(*m_pDraw);
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
		return m_pDraw->GetSize(_size);
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
		return m_pDraw->Resize(size);
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
		if ( m_pDraw->GetSize(sz) )
		{
			::DrawState(dc, NULL, &s_DrawIconProc, reinterpret_cast<LPARAM>(this), 0, x, y, sz.cx, sz.cy, DST_COMPLEX | DSS_DISABLED);
		}
	}
};



};///TNB

