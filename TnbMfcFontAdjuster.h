#pragma once
/**
 *@file
 * �t�H���g���ߊ֌W�̃w�b�_
 *
 *		�R���g���[���̍����ɒǏ]���ăt�H���g�̑傫���𒲐߂����邽�߂̋@�\��񋟂��܂��B
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
 * �t�H���g���߃N���X
 *
 *		inline �ȃR���g���[���ł��B
 *
 *		�{�N���X���g�p����ƁA�w��̃R���g���[���̍����ɂ��t�H���g�̑傫����ω������邱�Ƃ��o���܂��B
 *
 *	@param TYP �v�f�̌^�BCWnd �̃T�u�N���X�ł���K�v������܂��B
 *
 *	@par�g�p��
 *		\code
 *		//= �^�錾 (typedef���Ă����Ȃ��� Wizard�ŃG���[�ɐ���܂�)
 *		typedef CFontAdjusterAddinT<CStatic> CFaStatic;	
 *
 *		//= �_�C�A���O�N���X�錾
 *	 	CFaStatic	m_text;		// <- ���X CStatic�����������o������������		
 *
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcFontAdjuster.h
 * 
 *	@date 09/09/25 �V�K�쐬
 *	@date 09/10/22 �t�H���g�w��A�J���[�w�胁�\�b�h�ǉ��B
 */
template<typename TYP = CStatic>
class CFontAdjusterAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// �R���X�g���N�^
	CFontAdjusterAddinT(void) : m_isSafeFontSetting(false), m_textColor(CLR_INVALID)
	{
	}

	/**
	 * [�ݒ�] �t�H���g�T�C�Y���Z�b�g.
	 *		�ݒ肳�ꂽ�t�H���g�T�C�Y�ɖ߂��܂��B
	 *	@note ����A���̃E�B���h�E�T�C�Y�ƃt�H���g�T�C�Y����ɁA���T�C�Y���t�H���g�𒲐߂��܂��B
	 */
	void ResetFontSize(void)
	{
		m_height = m_GetClientHeight();
		m_Adjust();
	}

	/**
	 * [�ݒ�] �����F�w��.
	 *	@param c �F�B CLR_INVALID ���w�肷��ƁA�f�t�H���g�ɂȂ�܂��B
	 */
	void SetTextColor(COLORREF c = CLR_INVALID)
	{
		m_textColor = c;
	}
	
	/**
	 * [�ݒ�] �w�i�F�w��.
	 *	@param c �F�B CLR_INVALID ���w�肷��ƁA�f�t�H���g�ɂȂ�܂��B
	 */
	void SetBackColor(COLORREF c = CLR_INVALID)
	{
		if ( m_backBrush.GetSafeHandle() != NULL )
		{
			m_backBrush.DeleteObject();
		}
		if ( IS_RGBVALUE(c) )
		{
			m_backBrush.CreateSolidBrush(c);
		}
	}

	/**
	 * [�ݒ�] �t�H���g�����ݒ�.
	 *	@param logFont �t�H���g����
	 */
	void SetLogFont(const LOGFONT& logFont)
	{
		m_logFont = logFont;
		m_Adjust();
	}

	/**
	 * [�擾] �t�H���g�����擾.
	 *	@param[out] _logFont �t�H���g�������i�[�����B
	 */
	void GetLogFont(LOGFONT& _logFont) const
	{
		_logFont = m_logFont
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
		case WM_SETFONT:
			if ( ! m_isSafeFontSetting )
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_Check();
				return r;
			}
			break;
		case WM_SIZE:
			m_Adjust();
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] for notifications from parent
	 *	@note
	 *		CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE	�����ρB
	 *	@retval FALSE	�������B
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		switch ( message )
		{
		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			if ( IS_RGBVALUE(m_textColor) )
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				::SetBkMode(dc, TRANSPARENT);
				::SetTextColor(dc, m_textColor);
				if ( m_backBrush.GetSafeHandle() == NULL )
				{
					int f = COLOR_3DFACE;
					if ( message == WM_CTLCOLOREDIT )
					{
						f = COLOR_WINDOW;
					}
					m_backBrush.CreateSysColorBrush(f);
				}
				*_pResult = reinterpret_cast<LRESULT>(m_backBrush.GetSafeHandle());
				return true;
			}
			break;
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		m_Check();
	}

private:
	// �N���C�A���g�����擾
	int m_GetClientHeight(void)
	{
		CRect rc;
		_super::GetClientRect(rc);
		return rc.Height();
	}
	// ���݂̃T�C�Y�ƃt�H���g���m�F
	void m_Check(void)
	{
		m_height = m_GetClientHeight();
		Zero(m_logFont);
		CFont* pFont = _super::GetFont();
		if ( pFont != NULL )
		{
			pFont->GetLogFont(&m_logFont);
		}
		m_Adjust();
	}
	// ����
	void m_Adjust(void)
	{
		m_isSafeFontSetting = true;
		int height = m_GetClientHeight();
		{
			_super::SetFont(NULL);
			m_font.DeleteObject();
		}
		LOGFONT	lf = m_logFont;
		lf.lfHeight = m_logFont.lfHeight * height / m_height;
		m_font.CreateFontIndirect(&lf);
		_super::SetFont(&m_font);
		m_isSafeFontSetting = false;
	}
	LOGFONT		m_logFont;				///< �I���W�i���t�H���g
	int			m_height;				///< �I���W�i������
	CFont		m_font;					///< �g�p�t�H���g
	bool		m_isSafeFontSetting;	///< ���ȃt�H���g�ݒ蒆
	COLORREF	m_textColor;			///< �����J���[
	CBrush		m_backBrush;			///< �w�i�u���V
};



}; //MFC
}; //TNB



