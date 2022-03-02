#pragma once
/**
 *@file
 * �d���Ǘ��ʒm�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



#ifndef __AFX_H__
 #error
#endif



//TNB Library
namespace TNB {
namespace MFC {


/**@ingroup WINAPI
 * �d���Ǘ��A�h�C���N���X
 *
 *		�{�N���X�� �g�p���邱�ƂŁA �R�[���o�b�N�֐��ɂ�鉞�����\�ɂȂ�܂��B
 *
 *	@note �V���b�g�_�E���́u CPowerManager::Shutdown(); �v�Ŏ��s���܂��B
 *	@note �T�X�y���h�́u CPowerManager::Suspend(); �v�Ŏ��s���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPowerListener.h
 *
 *	@date 06/10/24�G�V�K�쐬�B
 */
template<typename TYP = CDialog>
class CPowerListenerAddinT : public TYP
{
	DEFSUPER(TYP);
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
		if ( message == WM_QUERYENDSESSION )
		{
			return CanLogoff();
		}
		else if ( message == WM_ENDSESSION )
		{
			if ( wParam != 0 )
			{
				OnLogoff();
			}
			else
			{
				OnCancelLogoff();
			}
			return 0;
		}
		else if ( message == WM_POWERBROADCAST )
		{
			switch ( wParam )
			{
			case PBT_APMQUERYSUSPEND:	// �T�X�y���h�₢���킹
				return CanSuspend() ? TRUE : BROADCAST_QUERY_DENY;
			case PBT_APMQUERYSUSPENDFAILED:	//�T�X�y���h�L�����Z��
				OnCancelSuspend();
				break;
			case PBT_APMSUSPEND:		//�T�X�y���h�J�n�B���ۂł��܂���B
				OnSuspend();
				break;
			case PBT_APMRESUMESUSPEND:	//���W���[���B
				OnResumeSuspend();
				break;
			}
			return 0;
		}
		return _super::WindowProc(message, wParam, lParam);
	}
public:

	/// �R���X�g���N�^
	CPowerListenerAddinT(void) : _super()
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param lpszTemplateName �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X����\�� null �ŏI��镶����B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	CPowerListenerAddinT(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
		: _super(lpszTemplateName, pParentWnd)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param nIDTemplate �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X�� ID �ԍ��B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	CPowerListenerAddinT(UINT nIDTemplate, CWnd* pParentWnd = NULL) 
		: _super(nIDTemplate, pParentWnd)
	{
	}

	/// �f�X�g���N�^
	virtual ~CPowerListenerAddinT(void)
	{
		::SetThreadExecutionState(ES_CONTINUOUS);
	}

	/**
	 * [�ݒ�] �T�X�y���h���[�h�ی�
	 *	@note �����쎞�Ԃɂ��T�X�y���h�ɓ���̂�h���܂��B
	 */
	void SaveSuspendMode(void)
	{
		::SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS);
	}
	
	/**
	 * [�ݒ�] �T�X�y���h���[�h�ی����
	 */
	void NormalSuspendMode(void)
	{
		::SetThreadExecutionState(ES_CONTINUOUS);
	}
	
	/**
	 * [�ʒm] ���OOFF�\���H.
	 *		�d��OFF���ȂǁA ���OOFF���s����O�ɖ₢���킹�����܂��B
	 *	@retval true ���OOFF�\.
	 *	@retval false �s�\.
	 */
	virtual bool CanLogoff(void) = 0;

	/**
	 * [�ʒm] ���OOFF�L�����Z��.
	 *		���OOFF���������f���ꂽ���ɒʒm����܂��B
	 */
	virtual void OnCancelLogoff(void) = 0;

	/**
	 * [�ʒm] ���OOFF�J�n.
	 *		���OOFF�������J�n����钼�O�ɒʒm����܂��B
	 */
	virtual void OnLogoff(void) = 0;

	/**
	 * [�ʒm] �T�X�y���h�\���H.
	 *		�T�X�y���h�A�n�C�o�l�[�V�������s����O�ɖ₢���킹�����܂��B
	 *	@retval true �T�X�y���h�\.
	 *	@retval false �s�\.
	 */
	virtual bool CanSuspend(void) = 0;

	/**
	 * [�ʒm] �T�X�y���h�L�����Z��.
	 *		�T�X�y���h�A�n�C�o�l�[�V���������f���ꂽ���ɒʒm����܂��B
	 */
	virtual void OnCancelSuspend(void) = 0;

	/**
	 * [�ʒm] �T�X�y���h�J�n.
	 *		�T�X�y���h�A�n�C�o�l�[�V�������J�n����钼�O�ɒʒm����܂��B
	 */
	virtual void OnSuspend(void) = 0;

	/**
	 * [�ʒm] ���W���[��.
	 *		�T�X�y���h�A�n�C�o�l�[�V�������畜�A�������ɒʒm����܂��B
	 */
	virtual void OnResumeSuspend(void) = 0;
};



}; //MFC
}; //TNB


