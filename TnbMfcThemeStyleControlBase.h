#pragma once
/**
 *@file
 * �e�[�}�X�^�C���p�[�c�`��R���g���[���x�[�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbThemeStylePartsDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {




/**@ingroup MFCCONTROL
 * �e�[�}�X�^�C���p�[�c�`��R���g���[���N���X
 *
 *		�e�[�}�X�^�C���p�[�c�`�悷��R���g���[���̍쐬���T�|�[�g����@�\�������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcThemeStyleControlBase.h
 * 
 *	@date 08/02/04	�V�K�쐬
 */
template<typename TYP>
class CThemeStyleControlBaseT : public TYP
{
protected:
	CThemeStylePartsDrawer	m_partsDrawer;		///< �e�[�}�p�[�c�`��

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == 0x031A/*WM_THEMECHANGED*/ )
		{
			m_partsDrawer.ThemeChanged();
		}
		return TYP::WindowProc(message, wParam, lParam);
	}
public:
	/// �R���X�g���N�^
	CThemeStyleControlBaseT(void)
	{
	}
};



}; // MFC
}; // TNB
