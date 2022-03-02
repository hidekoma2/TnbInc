#pragma once
/**
 *@file
 * �X�v���b�V���_�C�A���O�֌W�w�b�_ �t�@�C��
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcModelessDialog.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �X�v���b�V���E�B���h�E�Ǘ��e���v���[�g.
 *	
 *		�w��̃_�C�A���O�x�[�X�E�B���h�E�� Modeless �ŕ\�����܂��B
 *		Show() , Hide() , Destroy() ���\�b�h�ɂ��e�Ղɕ\��(�쐬)�A��\���A�j���ł���悤�ɂȂ�܂��B
 *
 *	@param DLG MFC �� CDialog ���p�������N���X�^
 *	@param HASTASK true �Ȃ�^�X�N�o�[�Ɍ���܂�(def.)�B false �Ȃ猻��܂���B
 *
 *	@note CModelessDialogT ���g�p���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSplashDialog.h
 */
template<typename DLG, bool HASTASK = true>
class CSplashDialogT : public CModelessDialogT<DLG, HASTASK>
{
};



}; // MFC
}; // TNB
