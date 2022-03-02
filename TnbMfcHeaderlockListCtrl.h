#pragma once
/**
 *@file
 * �w�b�_�Œ�ListCtrl�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcDrawingListCtrl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �w�b�_���b�N���X�g�R���g���[��.
 *		�w�b�_�̃J�������̕ύX���֎~�������X�g�R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�R���g���[���̃v���p�e�B�́A ���|�[�g�ɂ��Ă����Ă��������B
 *		�i�{�N���X�̓��|�[�g�`���̂݃T�|�[�g���Ă��܂��j	
 */
template<typename TYP = CListCtrl>
class CHeaderlockListCtrlT : public TYP
{
	DEFSUPER(TYP);
public:

	/** 
	 * [�}��] �J�����ǉ�.
	 *	@note CListCtrl::InsertColumn() �Ɠ����ł��B
	 *	@param nCol �J����
	 *	@param lpszColumnHeading �J������
	 *	@param nFormat �\���t�H�[�}�b�g.
	 *	@param nWidth ��
	 *	@param nSubItem �T�u�A�C�e��
	 *	@return �}���ʒu
	 */
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1)
	{
		int r = _super::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
		if ( r == 0 )
		{
			CHeaderCtrl* pHeaderCtrl = _super::GetHeaderCtrl();
			if ( pHeaderCtrl != NULL )
			{
				HDITEM hi;
				hi.mask = HDI_FORMAT;
				if ( pHeaderCtrl->GetItem(nCol, &hi) )
				{
					hi.fmt = (hi.fmt & ~LVCFMT_JUSTIFYMASK) | nFormat;
					hi.mask = HDI_FORMAT;
					pHeaderCtrl->SetItem(nCol, &hi);
				}
			}
		}
		return r;
	}

	/**
	 * [�擾] �I���A�C�e���擾.
	 *	@note �P��I���X�^�C����p�ł��B
	 *	@retval �}�C�i�X �I���Ȃ�
	 *	@retval �O�ȏ� �C���f�b�N�X
	 */
	int GetSelectedItem(void) const
	{
		return _super::GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	}

	/**
	 * [�ݒ�] �I���A�C�e���ݒ�.
	 *	@note �P��I���X�^�C����p�ł��B
	 *	@param item �C���f�b�N�X
	 */
	void SetSelectedItem(int item)
	{
		if ( item >= 0 )
		{
			_super::SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			_super::EnsureVisible(item, false);
		}
		else
		{
			int i = GetSelectedItem();
			if ( i >= 0 )
			{
				_super::SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}

	/**
	 * [�ݒ�] �������������[�h�ݒ�.
	 *	@note �f�t�H���g�ł� true �ł��B
	 *	@param b true ����ƁA�����N���C�A���g�Ɠ����ɂȂ�悤�ɒ������܂�(���X�N���[���o�[���o�Ȃ�)�B\n
	 */
	void SetAdjustWidthMode(bool b = true)
	{
		m_headerCtrl.SetAdjustWidthMode(b);
	}

	/**
	 * [�ݒ�] ���ύX���[�h�ݒ�.
	 *	@note �f�t�H���g�� ture �ł��B
	 *	@param b true ���w�肷��ƁA�}�E�X�ɂ��e�J�����̕��𒲐��ł��܂��B false �Ȃ�A������֎~���܂��B
	 */
	void EnableChangeWidth(bool b = true)
	{
		m_headerCtrl.EnableChangeWidth(b);
	}

	/**
	 * [�擾] �w�b�_�R���g���[���Q��.
	 *	@retval NULL �w�b�_�͂���܂���B
	 *	@retval NULL�ȊO �w�b�_�R���g���[���̃|�C���^�B
	 */
	CDrawingHeaderCtrl* GetHeaderCtrl(void)
	{
		return &m_headerCtrl;
	}

protected:
	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		ASSERT0( (_super::GetStyle() & LVS_TYPEMASK) == LVS_REPORT, "CMyListCtrl", "�T�|�[�g�ł���̂� Report�^�C�v�݂̂ł��B" );
		CHeaderCtrl* pHeaderCtrl = _super::GetHeaderCtrl();
		if ( pHeaderCtrl != NULL )
		{
			m_headerCtrl.SubclassWindow(*pHeaderCtrl);
			m_headerCtrl.EnableChangeWidth(true);
		}
		_super::PreSubclassWindow();
	}

private:
	CDrawingHeaderCtrl m_headerCtrl;	///< �w�b�_�[�R���g���[��

};



/**@ingroup MFCCONTROL
 * �w�b�_���b�N���X�g�R���g���[��.
 *		�w�b�_�̃J�������̕ύX���֎~�������X�g�R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�R���g���[���̃v���p�e�B�́A ���|�[�g�ɂ��Ă����Ă��������B
 *		�i�{�N���X�̓��|�[�g�`���̂݃T�|�[�g���Ă��܂��j	
 */
typedef CHeaderlockListCtrlT<> CHeaderlockListCtrl;



}; //MFC
}; //TNB



