#pragma once
/**
 *@file
 * �����N�e�L�X�g�֌W�̃w�b�_
 *
 *		�n�C�p�[�����N�e�L�X�g��񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcAbstractButton.h"
#include "TnbMfcCursorControl.h"
#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �����N�e�L�X�g�R���g���[��
 *
 *		inline �ȃR���g���[���ł��B
 *
 *		�}�E�X�z�o�[��K��ςݏ�ԂŐF��ύX�ł��郊���N�e�L�X�g���쐬�ł��܂��B
 *
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�ACStatic �Ɋ��蓖�Č�A
 *		�{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@see CAbstractButton
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcLinkText.h
 * 
 *	@date 07/10/19 �V�K�쐬
 *	@date 07/10/26 �e�L�X�g�\�������𒊏ۉ����� CTextDrawer �쐬�B
 *	@date 07/11/06 AdjustSize() �ǉ�
 *	@date 07/11/07 �A���_�[�o�[�̗L���A�w��\�ɁB
 */
class CLinkText : public CCursorControlAddinT<CAbstractButton>, public CDrawTextBase
{
	DEFSUPER(CCursorControlAddinT<CAbstractButton>);
	typedef CDrawTextBase _text;
	TColor			m_hoverColors;			///< �z�o�[��ԐF
	TColor			m_visitColors;			///< ��x�J������ԐF
	COLORREF		m_focusMarkColor;		///< �t�H�[�J�X�}�[�N
	bool			m_boIsVisit;			///< true �Ȃ�K��ς�
	bool			m_boIsUnderlineHoverOnly;///< true �Ȃ�z�o�[���̂݃A���_�[���C��
	/// 
	CFont* m_SetFont(CDC* pDC, CFont& f, bool withUnderline)
	{
		LOGFONT logFont;
		_super::GetFont()->GetLogFont(&logFont);
		logFont.lfUnderline = withUnderline;	//�A���_�[���C��
		f.CreateFontIndirect(&logFont);
		return pDC->SelectObject(&f);
	}

protected:

	/**
	 * [�ʒm] �`��
	 *	@param pDC �`����CDC�B
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		CString str;
		GetWindowText(str);
		if ( str.IsEmpty() )
		{
			return;
		}
		bool withUnderline = false;
		TColor gc = m_normalColors;
		TColor* pc = &m_normalColors;
		switch ( _super::GetButtonState() )
		{
		case ES_Pushed:	// �������
			withUnderline = true;
		case ES_Normal:	// �W�����
		default:
			if ( m_boIsVisit )
			{
				pc = &m_visitColors;
			}
			break;
		case ES_Hover:	// �z�o�[���
			pc = &m_hoverColors;
			withUnderline = true;
			break;
		case ES_Disable:// �������
			gc.forward = CLR_INVALID;
			pc = &gc;
			break;
		}
		if ( ! m_boIsUnderlineHoverOnly )
		{ 
			withUnderline = true;
		}
		CFont font;
		CFont* pOldFont = m_SetFont(pDC, font, withUnderline);
		RECT rect;
		_super::GetClientRect(&rect);
		UINT fmt = CTextDrawer::StaticToDrawStyle(_super::GetStyle()) | DT_NOPREFIX;
		pDC->SetBkMode(TRANSPARENT);		// �o�b�N�J���[�͕ύX�Ȃ�
		_text::DrawText(pDC, rect, fmt, *pc, str);
		if ( HasFocus() )
		{
			MFCLIB::DrawDottedBox(pDC, rect, m_focusMarkColor);
		}
		pDC->SelectObject(pOldFont);
	}

	/**
	 * [�ʒm] ��ԕω��ʒm.
	 *	@note ���݂̏�Ԃ��擾���܂�
	 *	@param state ��ԁB
	 */
	virtual void OnChangeState(EState state)
	{
		MFCLIB::DrawParent(this);
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
		if ( message == WM_ERASEBKGND ) { return 0; }
		LRESULT r = _super::WindowProc(message, wParam, lParam);
		if ( message == WM_SETFOCUS || message == WM_KILLFOCUS || message == WM_ACTIVATE )
		{
			MFCLIB::DrawParent(this);
		}
		return r;
	}
	
public:

	/// �R���X�g���N�^
	CLinkText(void) : m_boIsVisit(false), m_focusMarkColor(RGB(0, 0, 0)), m_boIsUnderlineHoverOnly(false)
	{
	}

	/// �f�X�g���N�^
	virtual ~CLinkText(void)
	{
	}

	/**
	 * [����] ��񕡐�.
	 *	@note �ێ����Ă�����𕡐����܂��B
	 *	@param other �R�s�[��.
	 *	@return �����̎Q��
	 */
	CLinkText& operator=(const CLinkText& other)
	{
		_super::operator=(other);
		_text::operator=(other);
		m_hoverColors = other.m_hoverColors;
		m_visitColors = other.m_visitColors;
		m_focusMarkColor = other.m_focusMarkColor;
		m_boIsUnderlineHoverOnly = other.m_boIsUnderlineHoverOnly;
		return *this;
	}

	/**
	 * [�ݒ�] �K��ς݃t���O�ݒ�.
	 *		�K��ςݏ�Ԃɂ��邱�Ƃ��o���܂��B���ʏ�ԂƂ́A�F���قȂ邾���ł��B
	 *	@param r �ݒ�t���O
	 */
	void SetVisitFlag(bool r = true)
	{
		m_boIsVisit = r;
		RedrawWindow();
	}
	
	/**
	 * [�ݒ�] �A���_�[�o�[�ݒ�.
	 *	@param r true �Ȃ�}�E�X����ɂ��鎞(�z�o�[��ԁA�������)�̎��̂݁A�A���_�[�o�[���ł�悤�ɂȂ�܂��B
	 *			false �Ȃ�A��ɃA���_�[�o�[���o�܂��B�ݒ肵�Ȃ��ꍇ�A��ɃA���_�[�o�[���ł܂��B
	 */
	void SetUnderlineMode(bool r)
	{
		m_boIsUnderlineHoverOnly = r;
	}

	/**
	 * [�ݒ�] �z�o�[��ԃe�L�X�g�F�w��.
	 *	@param color1 �O�̕����̐F
	 *	@param color2 ���̕����̐F�B�ȗ��\�B
	 */
	void SetTextHoverColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_hoverColors.forward = color1;
		m_hoverColors.back = color2;
	}

	/**
	 * [�ݒ�] �K��ςݏ�ԃe�L�X�g�F�w��.
	 *	@param color1 �O�̕����̐F
	 *	@param color2 ���̕����̐F�B�ȗ��\�B
	 */
	void SetTextVisitColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_visitColors.forward = color1;
		m_visitColors.back = color2;
	}

	/**
	 * [�ݒ�] �t�H�[�J�X�}�[�N�F�w��.
	 *	@param color �F
	 */
	void SetFocusMarkColor(COLORREF color)
	{
		m_focusMarkColor = color;
	}

	/**
	 * [�ݒ�] �T�C�Y����.
	 *		���ݎw�肳��Ă���t�H���g�A������A�N���C�A���g�T�C�Y����A
	 *		�K�����N���C�A���g�T�C�Y�ɕύX���܂��B
	 *	@param margin �}�[�W���B�K�v�ȃN���C�A���g�T�C�Y�ɉ�����T�C�Y�ł��B
	 *			�v���X�w�肷��ƁA�ʒu��ς����ɁA���A�����̂ݕύX����܂��B\n
	 *			�}�C�i�X�w�肷��ƁA�ʒu�A���A������ύX���܂��B\n
	 *			AdjustSize(10) �G�ʒu��ς����ɁA����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B\n
	 *			AdjustSize(-5) �G�ʒu���c������� 5 �ړ����A����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B
	 */
	void AdjustSize(int margin = 2)
	{
		CString str;
		GetWindowText(str);
		if ( str.IsEmpty() ) { return; }
		UINT fmt = CTextDrawer::StaticToDrawStyle(_super::GetStyle()) | DT_NOPREFIX;
		CDC* pDC = GetDC();
		CFont font;
		CFont* pOldFont = m_SetFont(pDC, font, true);
		MFCLIB::AdjustClientSize(pDC, fmt, str, margin);
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}

	/**
	 * [�ݒ�] �V���[�g�J�b�g�L�[�ݒ�.
	 *	@note �{�N���X�ł́A�{���\�b�h�͖����ł��B
	 *	@param key �_�~�[
	 */
	void SetShortcutKey(TCHAR key)
	{
	}
};



/**@ingroup MFCCONTROL
 * �n�C�p�[�����N�e�L�X�g�R���g���[��
 *
 *		inline �ȃR���g���[���ł��B
 *
 *		�}�E�X�z�o�[��K��ςݏ�ԂŐF��ύX�ł��郊���N�e�L�X�g���쐬�ł��܂��B
 *
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�ACStatic �Ɋ��蓖�Č�A
 *		�{�N���X�ɒu�������Ďg�p���܂��B
 *
 *		�N���b�N�͐e�ɒʒm�����A SetTarget() �Őݒ肵��URL��t�@�C�����I�[�v�����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcLinkText.h
 * 
 *	@date 07/10/19 �V�K�쐬
 *	@date 07/11/08 �t�H�[�J�X������Ƃ��Ƀ��^�[���������Ȃ��̂����P�B
 *	@date 07/11/15 �t�H�[�J�X�ړ������b�Z�[�W�ɏC���B
 */
class CHyperLinkText : public CLinkText
{
	DEFSUPER(CLinkText);
	CString			m_action;		///< �Ώ�
	CString			m_target;		///< �Ώ�
protected:

	/**
	 * [�ʒm] �N���b�N.
	 *		BN_CLICKED ��e�ɒʒm����O�ɃR�[������܂��B
	 */
	virtual void OnClickButton(void)
	{
		_super::SpecialCursor(::LoadCursor(NULL, IDC_WAIT));
		SHELLEXECUTEINFO sei;
		::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
		sei.cbSize = sizeof(SHELLEXECUTEINFO);	// Set Size
		sei.lpVerb = m_action;					// Set Verb
		sei.lpFile = m_target;					// Set Target
		sei.nShow = SW_SHOWNORMAL;				// Show Normal
		::ShellExecuteEx(&sei);
		_super::SetVisitFlag();
		_super::ResetCursor();
	}
	
public:

	/// �R���X�g���N�^
	CHyperLinkText(void) : m_action(_T("open"))
	{
		_super::SetTextColor(RGB(0, 0, 255));
		_super::SetTextHoverColor(RGB(255, 128, 0));
		_super::SetTextVisitColor(RGB(200, 0, 0));
	}

	/**
	 * [�ݒ�] �^�[�Q�b�g�ݒ�.
	 *	@param lpszTarget �^�[�Q�b�g�BURL ��t�@�C�������w�肵�܂��B
	 */
	void SetTarget(LPCTSTR lpszTarget)
	{
		m_target = lpszTarget;
	}

	/**
	 * [�ݒ�] �A�N�V�����ݒ�.
	 *	@note �f�t�H���g�ł� open ���w�肳��Ă��܂��B
	 *		���ɁA edit , explore , find , print , properties �Ȃǂ���B
	 *	@param lpszAction �A�N�V�����B
	 */
	void SetAction(LPCTSTR lpszAction)
	{
		m_action = lpszAction;
	}
};



}; //MFC
}; //TNB


