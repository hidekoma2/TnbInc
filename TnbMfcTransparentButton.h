#pragma once
/**
 *@file
 * ���R�^�����{�^���֌W�̃w�b�_
 *
 *		�����ȃ{�^����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcAbstractButton.h"
#include "TnbMfcCursorControl.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL 
 * ���R�^�����{�^���R���g���[��
 *
 *		�����ȃ{�^����񋟂��܂��B���C���_�C�A���O�̔w�i�����߂���̂ŁA�w�i���N���b�N����悤�ȏ������o���܂��B
 *
 *	@note �}�E�X���z�o�����O����ƃ}�E�X�J�[�\�����ω����܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^�����X�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton or CStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@see CAbstractButton
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcTransparentButton.h
 * 
 *	@date 09/04/14 �V�K�쐬
 */
class CTransparentButton : public CCursorControlAddinT<CAbstractButton>
{
	DEFSUPER(CCursorControlAddinT<CAbstractButton>);
public:

	/// �R���X�g���N�^
	CTransparentButton(void) : m_hRgn(::CreateRectRgn(0, 0, 0, 0)), m_hoverParsent(-1)
	{
	}

	/// �f�X�g���N�^
	~CTransparentButton(void)
	{
		if ( m_hRgn != NULL )
		{
			_DeleteObject(m_hRgn);
			m_hRgn = NULL;
		}
	}

	/**
	 * [�ݒ�] �{�^���T�C�Y�ݒ�.
	 *	@note �{�^���͋�`�ɂȂ�܂��B
	 *	@param sz �T�C�Y
	 */
	void SetButtonSize(const SIZE& sz)
	{
		HRGN rgn = ::CreateRectRgn(0, 0, sz.cx, sz.cy);
		SetButtonRgn(rgn);
		_DeleteObject(rgn);
	}

	/**
	 * [�ݒ�] �{�^���̈�ݒ�.
	 *	@note �{�^���͎��R�`�ɂȂ�܂��B
	 *	@param rgn �̈�B�ݒ��j�����Ă��܂��܂���B
	 */
	void SetButtonRgn(HRGN rgn)
	{
		HRGN h = ::CreateRectRgn(0, 0, 0, 0);
		::CombineRgn(h, rgn, NULL, RGN_COPY);
		::CombineRgn(m_hRgn, rgn, NULL, RGN_COPY);
		CRect rc;
		::GetRgnBox(m_hRgn, rc);
		_super::SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOZORDER);
		_super::SetWindowRgn(h, true);
		m_hoverFill.Empty();
	}

	/**
	 * [�ݒ�] �t�H�[�J�X�F�ݒ�.
	 *	@param c �J���[�B�@
	 */
	void SetFocusColor(COLORREF c = CLR_INVALID)
	{
		if ( m_focusBrush.GetSafeHandle() != NULL )
		{
			m_focusBrush.DeleteObject();
		}
		if ( IS_RGBVALUE(c) )
		{
			m_focusBrush.CreateHatchBrush(HS_DIAGCROSS, c);
		}
	}

	/**
	 * [�ݒ�] �z�o�[�J���[�ݒ�.
	 *	@param parsent �Z���B 100 �� color ���̂��́B -1 �ŁA�z�o�[�w�����
	 *	@param color �J���[�B
	 */
	void SetHoverFillColor(int parsent, COLORREF color = RGB(0, 0, 0))
	{
		m_hoverParsent = parsent;
		m_hoverColor = color;
		m_hoverFill.Empty();
	}

protected:
	HRGN			m_hRgn;			///< �R���g���[�����[�W����
	CBrush			m_focusBrush;	///< �t�H�[�J�X�u���V
	COLORREF		m_hoverColor;	///< �z�o�[�J���[
	int				m_hoverParsent;	///< �z�o�[�J���[�����x
	CBitmapImage	m_hoverFill;	///< �z�o�[�p

	/**
	 * [�ʒm] �`��
	 *	@param pDC �`����CDC�B
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		switch ( _super::GetButtonState() )
		{
		case ES_Pushed:
		case ES_Hover:
			if ( m_hoverParsent >= 0 )
			{
				if ( m_hoverFill.IsEmpty() )
				{
					CRect rc;
					GetClientRect(rc);
					m_hoverFill.Set(rc.Width(), rc.Height(), m_hoverColor);
				}
				::SelectClipRgn(pDC->GetSafeHdc(), m_hRgn);
				m_hoverFill.AlphaBlend(*pDC, 0, 0, m_hoverParsent);
				::SelectClipRgn(pDC->GetSafeHdc(), NULL);
			}
			break;
		}
		if ( HasFocus() )
		{
			if ( m_focusBrush.GetSafeHandle() != NULL )
			{
				pDC->SetBkMode(TRANSPARENT);
				::FrameRgn(pDC->GetSafeHdc(), m_hRgn, m_focusBrush, 1, 1);
			}
		}
	}

	/**
	 * [�ʒm] ��ԕω��ʒm.
	 *	@note ���݂̏�Ԃ��擾���܂�
	 *	@param state ��ԁB
	 */
	virtual void OnChangeState(EState state)
	{
		MFCLIB::DrawParent(this, false);
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
		if ( message == WM_DESTROY )
		{
			if ( m_hRgn != NULL )
			{
				_DeleteObject(m_hRgn);
				m_hRgn = NULL;
			}
			if ( m_focusBrush.GetSafeHandle() != NULL )
			{
				m_focusBrush.DeleteObject();
			}
		}
		if ( message == WM_ERASEBKGND )
		{
			return 0;
		}
		LRESULT r = _super::WindowProc(message, wParam, lParam);
		if ( message == WM_SETFOCUS || message == WM_KILLFOCUS || message == WM_ACTIVATE || message == WM_MOUSELEAVE )
		{
			MFCLIB::DrawParent(this, false);
		}
		if ( message >= WM_LBUTTONDOWN && message <= WM_MOUSELAST )
		{
			MFCLIB::DrawParent(this, true);
		}
		return r;
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		CRect rc;
		_super::GetWindowRect(&rc);
		SetButtonSize(rc.Size());
		_super::PreSubclassWindow();
	}
};



}; //MFC
}; //TNB



