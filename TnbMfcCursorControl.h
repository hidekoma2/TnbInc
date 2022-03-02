#pragma once
/**
 *@file
 * �J�[�\���R���g���[�������֌W�̃w�b�_
 *
 *		�R���g���[���̏�ԂŃ}�E�X�J�[�\����ω������邽�߂̋@�\��񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



#ifndef __AFX_H__
 #error
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �}�E�X�J�[�\������N���X
 *
 *		inline �ȃR���g���[���ł��B
 *
 *		�{�N���X���g�p����ƁA�w��̃R���g���[���̏�Ԃɂ��}�E�X�J�[�\����ω������邱�Ƃ��o���܂��B
 *
 *	@param TYP �v�f�̌^�BCWnd �̃T�u�N���X�ł���K�{������܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		//= �^�錾 (typedef���Ă����Ȃ��� Wizard�ŃG���[�ɐ���܂�)
 *		typedef CCursorControlAddinT<CStatic> CCcStatic;	
 *
 *		//= �_�C�A���O�N���X�錾
 *	 	CCcStatic	m_text;		// <- ���X CStatic�����������o������������		
 *
 *		//= �ݒ�(OnInitDialog()�Ȃ�)
 *		m_text.SetHoverCursor(::LoadCursor(NULL, IDC_HAND));
 *		</pre></td></tr></table>
 *		
 *	@par�K�v�t�@�C��
 *			TnbMfcCursorControl.h
 * 
 *	@date 07/10/19 �V�K�쐬
 *	@date 07/10/30 �`���c�L���Ȃ������B
 */
template<typename TYP = CStatic>
class CCursorControlAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// �R���X�g���N�^
	CCursorControlAddinT(void) : m_normalCursor(NULL), m_hoverCursor(NULL), m_pushedCursor(NULL)
		, m_boHasHover(false), m_boIsPushed(false), m_boIsEnable(true)
	{
		#ifdef IDC_HAND
			m_hoverCursor = ::LoadCursor(NULL, IDC_HAND);
		#endif
	}

	/**
	 * [�ݒ�] �z�o�[��Ԃ̃J�[�\���ݒ�.
	 *	@param h �J�[�\���B�{�C���X�^���X���j�������܂Ŕj�����Ȃ��ł��������B
	 */
	void SetHoverCursor(HCURSOR h)
	{
		m_hoverCursor = h;
	}

	/**
	 * [�ݒ�] ������Ԃ̃J�[�\���ݒ�.
	 *	@param h �J�[�\���B�{�C���X�^���X���j�������܂Ŕj�����Ȃ��ł��������B
	 */
	void SetPushedCursor(HCURSOR h)
	{
		m_pushedCursor = h;
	}

	/**
	 * [�ݒ�] �J�[�\���ݒ�R�s�[.
	 *		���̃R���g���[���̐ݒ���R�s�[���܂��B
	 *	@param other ���̃R���g���[���B
	 */
	template<typename T>
	void CopyCursorState(const CCursorControlAddinT<T>& other)
	{
		m_hoverCursor = other.m_hoverCursor;
		m_pushedCursor = other.m_pushedCursor;
	}

	/**
	 * [�ݒ�] ����J�[�\�����[�h.
	 *		�{�N���X�̃J�[�\���ύX�@�\���ꎞ�I�ɒ�~���A�w��̃J�[�\���ɂ��܂��B
	 *	@note ���ɖ߂��ɂ́A ResetCursor() ���g�p���܂��B
	 *	@param h �J�[�\���B
	 */
	void SpecialCursor(HCURSOR h)
	{
		m_boIsEnable = false;
		::SetCursor(h);
	}

	/**
	 * [�ݒ�] ����J�[�\�����[�h����.
	 *		SpecialCursor() �̐ݒ���������܂��B
	 */
	void ResetCursor(void)
	{
		m_boIsEnable = true;
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
		switch ( message )
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if ( ! m_boIsPushed )
			{
				m_boIsPushed = true;
				m_CheckNowState();
			}
			break;
		case WM_LBUTTONUP:
			if ( m_boIsPushed )
			{
				m_boIsPushed = false;
				m_CheckNowState();
			}
			break;
		case WM_MOUSEMOVE:
		case WM_MOUSEHOVER:
			if ( ! m_boHasHover )
			{
				m_boHasHover = true;
				m_HoverSub();
			}
			m_CheckNowState();
			break;
		case WM_MOUSELEAVE:
			if ( m_boHasHover )
			{
				m_boHasHover = false;
				m_CheckNowState();
			}
			break;
		case WM_ENABLE:
			m_CheckNowState();
			break;
		case WM_DESTROY:
			m_boHasHover = false;
			m_boIsPushed = false;
			m_CheckNowState();
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	HCURSOR				m_normalCursor;	///< �W����Ԃ̃J�[�\��
	HCURSOR				m_hoverCursor;	///< �z�o�[��Ԃ̃J�[�\��
	HCURSOR				m_pushedCursor;	///< ������Ԃ̃J�[�\��
	bool				m_boHasHover;	///< true �Ȃ�z�o�[���
	bool				m_boIsPushed;	///< true �Ȃ牟�����
	bool				m_boIsEnable;	///< true �Ȃ�@�\����
	/// ���̃z�o�[�C�x���g�����炤���߂̏���
	void m_HoverSub(void)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		::_TrackMouseEvent(&tme);
	}
	/// ���݂̏�ԃ`�F�b�N���J�[�\���ύX	
	void m_CheckNowState(void)
	{
		if ( ! m_boIsEnable ) { return; }
		if ( m_normalCursor == NULL )
		{
			m_normalCursor = ::GetCursor();
			::SetClassLongPtr(m_hWnd, GCLP_HCURSOR, NULL); //Win�Ɋ֘A���Ă��� Cursor��NULL��
		}
		HCURSOR h = m_normalCursor;
		if ( _super::IsWindowEnabled() )
		{
			if ( m_boIsPushed && m_pushedCursor != NULL )
			{
				h = m_pushedCursor;
			}
			else if ( m_boHasHover && m_hoverCursor != NULL )
			{
				h = m_hoverCursor;
			}
		}
		::SetCursor(h);
	}
};



}; //MFC
}; //TNB



