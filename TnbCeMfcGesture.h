#pragma once
/**
 *@file
 * CE �^�b�v���z�[���h�W�F�X�`���[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _WIN32_WCE
	#error TnbCeMfcGesture.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL WINCE 
 * �^�b�v���z�[���h�W�F�X�`���[�֎~�A�h�C��(CE��p).
 *	
 *		CE �ł̓^�b�v���ăz�[���h����ƁA���邭��łĂ��܂��B
 *		����𐧌�����(�łȂ��悤�ɂ���)�A�h�C���ł��B
 *	
 *	@par�K�v�t�@�C��
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	�V�K�쐬
 */
template<typename WND>
class CCeDisableGestureAddinT : public WND
{
	DEFSUPER(WND);
protected:

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
		if ( message == WM_LBUTTONDOWN )
		{
			return _super::DefWindowProc(message, wParam, lParam);
		}
		return _super::WindowProc(message, wParam, lParam);
	}
};



/**@ingroup WINCE 
 * �^�b�v���z�[���h�W�F�X�`���[�֎~ Dialog (CE��p).
 *	
 *		CE �ł̓^�b�v���ăz�[���h����ƁA���邭��łĂ��܂��B
 *		����𐧌�����(�łȂ��悤�ɂ���) CDialog �ł��B
 *	
 *	@par�K�v�t�@�C��
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	�V�K�쐬
 */
class CDialogDg : public CDialog
{
	DEFSUPER(CDialog);
public:

	/**
	 * �R���X�g���N�^
	 *	@param lpszTemplateName �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X����\�� null �ŏI��镶����B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	explicit CDialogDg(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL)
		: CDialog(lpszTemplateName, pParentWnd)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param nIDTemplate �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X�� ID �ԍ��B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	explicit CDialogDg(UINT nIDTemplate, CWnd* pParentWnd = NULL)
		: CDialog(nIDTemplate, pParentWnd)
	{
	}

protected:
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
		if ( message == WM_LBUTTONDOWN )
		{
			return _super::DefWindowProc(message, wParam, lParam);
		}
		return _super::WindowProc(message, wParam, lParam);
	}
};



/**@ingroup WINCE 
 * �^�b�v���z�[���h�W�F�X�`���[�֎~ Edit (CE��p).
 *	
 *		CE �ł̓^�b�v���ăz�[���h����ƁA���邭��łĂ��܂��B
 *		����𐧌�����(�łȂ��悤�ɂ���) CEdit �ł��B
 *	
 *	@par�K�v�t�@�C��
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	�V�K�쐬
 */
typedef CCeDisableGestureAddinT<CEdit> CEditDg;



/**@ingroup WINCE 
 * �^�b�v���z�[���h�W�F�X�`���[�֎~ ComboBox (CE��p).
 *	
 *		CE �ł̓^�b�v���ăz�[���h����ƁA���邭��łĂ��܂��B
 *		����𐧌�����(�łȂ��悤�ɂ���) CComboBox �ł��B
 *	
 *	@par�K�v�t�@�C��
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	�V�K�쐬
 */
typedef CCeDisableGestureAddinT<CComboBox> CComboBoxDg;



/**@ingroup WINCE 
 * �^�b�v���z�[���h�W�F�X�`���[�֎~ SpinButtonCtrl (CE��p).
 *	
 *		CE �ł̓^�b�v���ăz�[���h����ƁA���邭��łĂ��܂��B
 *		����𐧌�����(�łȂ��悤�ɂ���) CSpinButtonCtrl �ł��B
 *	
 *	@par�K�v�t�@�C��
 *			TnbCeMfcGesture.h
 * 
 *	@date 09/09/16	�V�K�쐬
 */
typedef CCeDisableGestureAddinT<CSpinButtonCtrl> CSpinButtonCtrlDg;



}; //MFC
}; //TNB
