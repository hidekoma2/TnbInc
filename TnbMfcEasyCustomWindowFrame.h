#pragma once
/**
 *@file
 * �ȒP�ȃJ�X�^���E�B���h�E�ݒ�֌W
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCustomWindowFrameMaster.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB 
{



/**@ingroup WINAPI
 * �ȒP�ȃJ�X�^���E�B���h�E�t���[���N���X.
 *
 *		�悭�g����ݒ���܂Ƃ߂������̃N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEasyCustomWindowFrame.h
 *
 *	@date 10/05/20 �V�K�쐬
 */
class CEasyCustomWindowFrame
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param isDisableVista true ���w�肷��ƁA VISTA�ȍ~�̏ꍇ�A�t�b�N���܂���B
	 *						false ���w�肷��ƁA VISTA�ȍ~�ł��A�t�b�N���܂��B
	 */
	CEasyCustomWindowFrame(bool isDisableVista = false)
		: m_window(::GetSysColor(COLOR_BTNTEXT), ::GetSysColor(COLOR_BTNFACE))
		, m_activeCaption(::GetSysColor(COLOR_CAPTIONTEXT), ::GetSysColor(COLOR_ACTIVECAPTION))
		, m_inactiveCaption(::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTION))
		, m_baseColor(RGB(255, 255, 255)), m_isDisableVista(isDisableVista)
	{
	}

	/**
	 * [�ݒ�] �E�B���h�E�F�ݒ�.
	 *	@note  Hook() �Ŏw�肷��t���[���摜�̃x�[�X�F( SetBaseColor() �Ŏw�肵���F) ��w�i�F�ɒu�������܂��B
	 *	@param text �����F
	 *	@param back �w�i�F
	 */
	void SetWindowColor(COLORREF text, COLORREF back)
	{
		m_window.text = text;
		m_window.back = back;
	}

	/**
	 * [�ݒ�] �x�[�X�F�ݒ�.
	 *	@param c �x�[�X�F
	 */
	void SetBaseColor(COLORREF c)
	{
		m_baseColor = c;
	}

	/**
	 * [�ݒ�] �L���v�V�����F�ݒ�.
	 *	@param text �����F
	 *	@param back �w�i�F
	 *	@param isActive true �Ȃ�A�N�e�B�u�̃L���v�V�����A false �Ȃ��A�N�e�B�u�̃L���v�V�����̐ݒ���s���B
	 */
	void SetCaptionColor(COLORREF text, COLORREF back, bool isActive)
	{
		if ( isActive )
		{
			m_activeCaption.text = text;
			m_activeCaption.back = back;
		}
		else
		{
			m_inactiveCaption.text = text;
			m_inactiveCaption.back = back;
		}
	}

	/**
	 * [�ݒ�] �J�X�^���E�B���h�E�t���[��.
	 *	
	 *
	 *	@param frame �t���[���B�F��ς������Ƃ����(255, 255, 255)�ŕ`��B
	 *	@param button �V�X�e���{�^���B�Ԃ��x�[�X�ŕ`��BRGB�̑���ŐƉ��F���쐬���܂��B
	 *	@param num �V�X�e���{�^���̉摜���B
	 */
	void Hook(CBitmapHandle frame, CBitmapHandle button, int num = 4)
	{
		CCustomWindowFrameMaster cwfm;
		CBitmapImage bi;
		CWindowCaptionDrawer ttd(NULL);
		ttd.SetBarColor(m_activeCaption.back, m_inactiveCaption.back);
		ttd.SetTextColor(m_activeCaption.text, m_inactiveCaption.text);
		cwfm.SetTitleBitmap(ttd);
		bi.Set(frame);
		bi.ChangePixelColor(m_baseColor, m_window.back);
		cwfm.SetFrameBitmap(bi);
		cwfm.SetFrameTransColor(CLR_AUTOSELECT);
		bi.Set(button);
		bi.ChangePixelColor(RGB(0, 0, 0), m_window.back);
		cwfm.SetCloseButtonBitmap(CPoint(CalcScaleX(-6), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		bi.Set(button);
		bi.ChangePixelColor(RGB(255, 0, 0), RGB(230, 0, 0), RGB(0, 255, 0));
		bi.ChangePixelColor(RGB(0, 0, 0), m_window.back);
		cwfm.SetMaximumButtonBitmap(CPoint(CalcScaleX(-23), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		cwfm.SetRestoreButtonBitmap(CPoint(CalcScaleX(-23), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		bi.Set(button);
		bi.ChangePixelColor(RGB(0, 0, 255), RGB(0, 255, 0), RGB(255, 0, 0));
		bi.ChangePixelColor(RGB(0, 0, 0), m_window.back);
		cwfm.SetMinimumButtonBitmap(CPoint(CalcScaleX(-40), CalcScaleY(5)), bi, num, CLR_AUTOSELECT);
		cwfm.SetTextColor(m_window.text);
		cwfm.Hook(m_isDisableVista);
	}

private:

	/// �y�A���
	struct TPair
	{
		COLORREF	text;	///< �����F
		COLORREF	back;	///< �w�i�F
		/// �R���X�g���N�^
		TPair(COLORREF t, COLORREF b) : text(t), back(b)
		{
		}
	};
	COLORREF	m_baseColor;		///< �x�[�X�F
	TPair		m_window;			///< �E�B���h�E�F
	TPair		m_activeCaption;	///< �A�N�e�B�u�L���v�V�����F
	TPair		m_inactiveCaption;	///< ��A�N�e�B�u�L���v�V�����F
	bool		m_isDisableVista;	///< VISTA �̏ꍇ����
};



}; // TNB
