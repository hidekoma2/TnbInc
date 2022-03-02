#pragma once
/**
 *@file
 * �������ςȂ��\�̃{�^���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



#ifdef _WIN32_WCE
 #error not support "PressButton"
#endif // _WIN32_WCE



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #ifndef BN_PUSHED
  #define BN_PUSHED BN_HILITE
 #endif
 #ifndef BN_UNPUSHED
  #define BN_UNPUSHED BN_UNHILITE 
 #endif
#endif //_TnbDOXYGEN



/**@ingroup MFCCONTROL
 * �{�^���R���g���[��
 *
 *			�{�^����������A���̂܂ܗ������A�ړ������邱�ƂŁA�������ςȂ���
 *			�\���\�ȃ{�^���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^�������W�I�{�^�����`�F�b�N�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton�j��{�N���X�ɒu�������Ďg�p���܂��B
 *		�v���X���A BN_PUSHED �A�����[�X���A BN_UNPUSHED �A�e�E�B���h�E�� WM_COMMAND �Œʎ�����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPressButton.h
 * 
 *	@date 06/09/26 �V�K�쐬
 */
class CPressButton : public CButton
{
	DEFSUPER(CButton);
	COLORREF	m_colorBack;		///< �w�i�̐F
	bool		m_boIsPushed;		///< 

	void PostCommand(UINT uCmd)
	{
		MFCLIB::SendCommandMessage(this, uCmd);
//		CWnd* pcWnd = GetParent();
//		if ( pcWnd != NULL )
//		{
//			pcWnd->PostMessage(WM_COMMAND, uCmd * 0x10000 | GetDlgCtrlID(), 
//												reinterpret_cast<LPARAM>(m_hWnd));
//		}
	}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return _super::OnCommand(wParam, lParam);
	}

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
			if ( ! m_boIsPushed )
			{
				m_boIsPushed = true;
				RedrawWindow();
				PostCommand(BN_PUSHED);
			}
			return 0;
		}
		else if ( message == WM_LBUTTONUP )
		{
			if ( m_boIsPushed )
			{
				m_boIsPushed = false;
				RedrawWindow();
				PostCommand(BN_UNPUSHED);
				PostCommand(BN_CLICKED);				
			}
			return 0;
		}
		else if ( message == WM_LBUTTONDBLCLK )
		{
			if ( ! m_boIsPushed )
			{
				PostCommand(BN_PUSHED);
			}
			m_boIsPushed = false;
			RedrawWindow();
			PostCommand(BN_UNPUSHED);
			PostCommand(BN_DOUBLECLICKED);				
			return 0;
		}
		else if ( message == WM_SETFOCUS || message == WM_KILLFOCUS )
		{
			UpdateWindow();
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		UINT uiDefBtnSte = _super::GetButtonStyle() & 0xFF;
		if ( uiDefBtnSte == BS_PUSHBUTTON || uiDefBtnSte == BS_DEFPUSHBUTTON )
		{
			_super::SetButtonStyle(BS_AUTOCHECKBOX);
			_super::ModifyStyle(0, BS_OWNERDRAW);
		}
		_super::PreSubclassWindow();
	}

	/**
	 * [�ʒm] �`��.
	 *		�`�悷��^�C�~���O�ŃR�[�������B
	 *	@note CButton �� virtual ���\�b�h
	 *	@param lpDrawItemStruct �`�悷�邽�߂̏��
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if ( lpDrawItemStruct->CtlType != ODT_BUTTON || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
		dc->SaveDC();
		UINT uItemState = lpDrawItemStruct->itemState;

		//== �{�^���t���[���`��
		UINT uState = DFCS_BUTTONPUSH | 0x0800/*DFCS_TRANSPARENT*/;
		if ( m_boIsPushed )
		{
			uState |= DFCS_PUSHED;
		}
		dc->DrawFrameControl(&(lpDrawItemStruct->rcItem), DFC_BUTTON, uState);
		//===== ���gFILL
		if ( ToInt(m_colorBack) >= 0 )
		{
			CRect cRect = lpDrawItemStruct->rcItem;
			cRect.DeflateRect(2, 2, 2, 2);
			dc->FillSolidRect(cRect, m_colorBack);
		}
		//== ����
		CString strText;
		GetWindowText(strText);
		if ( ! strText.IsEmpty() )
		{
			CRect cRect = lpDrawItemStruct->rcItem;
			cRect.DeflateRect(4, 4, 4, 4);
			if ( m_boIsPushed )
			{
				cRect.OffsetRect(1, 1);
			}
			dc->SetBkMode(TRANSPARENT);		// �o�b�N�J���[�͕ύX�Ȃ�
			if ( (uItemState & ODS_DISABLED) != 0 )
			{
				dc->SetTextColor(GetSysColor(COLOR_GRAYTEXT));	//�O���C
			}
			dc->DrawText(strText, cRect, DT_CENTER | DT_VCENTER);
		}	
//		//== �t�H�[�J�X�_��
//		if ( ::GetFocus() == m_hWnd )
//		{
//			CRect cRect = lpDrawItemStruct->rcItem;
//			cRect.DeflateRect(3,3,3,3);
//			if ( m_boIsPushed )
//			{
//				cRect.OffsetRect(1,1);
//			}
//			dc->DrawFocusRect(&cRect);
//		}
		dc->RestoreDC(-1);
	}

public:

	/// �R���X�g���N�^
	CPressButton(void)
		: m_colorBack(COLORREF(-1))
		, m_boIsPushed(false)
	{
	}
	
	/**
	 * [�ݒ�] �{�^���F
	 *	@note �{�^���̕\�ʂ̐F���w��ł��܂��B
	 *	@param color �J���[�B�ȗ�����ƁA�V�X�e���J���[�ɂȂ�܂��B
	 */
	void SetBkColor(COLORREF color = -1)
	{
		m_colorBack = color;
	}

	/**
	 * [�擾] �{�^�����
	 *	@retval true �������
	 *	@retval false �񉟉����
	 */
	bool IsPushed(void) const
	{
		return m_boIsPushed;
	}

	/**
	 * [�ݒ�] �{�^���������
	 */
	void Push(void)
	{
		PostMessage(WM_LBUTTONDOWN);
	}

	/**
	 * [�ݒ�] �{�^���񉟉����
	 */
	void Unpush(void)
	{
		PostMessage(WM_LBUTTONUP);
	}

};



}; //MFC
}; //TNB



