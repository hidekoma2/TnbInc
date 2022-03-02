#pragma once
/**
 *@file
 * �R���g���[���ʒu�̒����֌W�̃w�b�_
 *
 *		IDrawable �𗘗p�����R���g���[�������p�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * �R���g���[���ʒu�����p�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂����A�`�悵�܂���B
 *		�`��ʒu���𗘗p���āA�R���g���[���̈ʒu�𐧌䂵�܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�R���g���[�����A�w��̈ʒu�Ɉړ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPositionAdjuster.h
 *
 *	@date 07/12/05 �V�K�쐬
 *	@date 08/09/19 MFCLIB::SetProcessingFlag() ���g���悤�ɂ����B
 */
class CPositionAdjuster : public IDrawable
{
	DEFSUPER(IDrawable);
protected:

	CWnd*	m_pWnd;			///< �R���g���[��
	bool	m_boCanResize;	///< true �Ȃ烊�T�C�Y�Ή�
	bool	m_boAlways;		///< true �Ȃ� DC ��NULL �ȊO�ł��z�u�ύX
public:

	/**
	 * �R���X�g���N�^
	 *	@param pWnd �ΏۃE�B���h�E(�R���g���[��)�B
	 *	@param canResize �ȗ��� true �Ȃ烊�T�C�Y�Ή��B false �Ȃ烊�T�C�Y�͋֎~���܂��B
	 *	@param boAlways	true �Ȃ� DC ��NULL �ȊO�ł��z�u�ύX�B�ȗ��� false �Ȃ� dc �� NULL �̏ꍇ�̂ݒ������܂��B
	 */
	CPositionAdjuster(CWnd*	pWnd, bool canResize = true, bool boAlways = false)
		: m_pWnd(pWnd), m_boCanResize(canResize), m_boAlways(boAlways)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CPositionAdjuster(m_pWnd, m_boCanResize, m_boAlways);
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
		if ( m_pWnd != NULL )
		{
			CRect rc;
			m_pWnd->GetWindowRect(&rc);
			_size.cx = rc.Width();
			_size.cy = rc.Height();
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note �{�N���X�͊Ǘ����Ă���Window�̃T�C�Y��ύX���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�E�B���h�E����������Ă��܂���B
	 */
	virtual bool Resize(const SIZE& size)
	{
		if ( m_boCanResize && m_pWnd != NULL )
		{
			return !! m_pWnd->SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER);
		}
		return false;
	}
	
	/**
	 * [�`��] �`��.
	 *		�`�悹���A�w��̃R���g���[�����w��̈ʒu�Ɉړ������܂��B
	 *	@note �{�N���X�̕`��́A dc �� NULL �̎��̂ݓ��삵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		if ( m_boAlways || dc == NULL )
		{
			CRect rc;
			m_pWnd->GetWindowRect(&rc);
			m_pWnd->MoveWindow(x, y, rc.Width(), rc.Height(), false);
			m_pWnd->RedrawWindow();
			MFCLIB::SetProcessingFlag(m_pWnd->GetSafeHwnd(), true);
		}
	}
};



};///TNB

