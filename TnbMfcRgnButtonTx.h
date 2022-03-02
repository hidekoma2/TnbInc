#pragma once
/**
 *@file
 * �e�L�X�g�t���R�^�r�b�g�}�b�v�{�^���֌W�̃w�b�_
 *
 *		�r�b�g�}�b�v�̌`��ɂ��킹���O���t�B�J���ȃ{�^����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcRgnButton.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL BITMAP
 * �e�L�X�g�t���R�^�r�b�g�}�b�v�{�^���R���g���[��
 *
 *		�r�b�g�}�b�v�̌`�ɍ��킹���{�^�����쐬���邱�Ƃ��o���܂��B
 *
 *		CRgnButton �̋@�\�ɁA�e�L�X�g���\���ł���悤�Ɋg���������̂ł��B
 *		��{�I�Ȏg�����́A�X�[�p�[�N���X�� CRgnButton ���Q�Ƃ��Ă��������B
 *
 *		������ݒ�́A SetWindowText() �� WM_SETTEXT ���g�p���܂��B
 *		�t�H���g�ݒ�́A SetFont() �� WM_SETFONT ���g�p���܂��B
 *		�F�w��́A SetTextColor(), SetTextDisableColor() ���g�p���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^�����X�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton or CStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcRgnButtonTx.h
 * 
 *	@see CAbstractButton
 *
 *	@date 07/10/05 �V�K�쐬
 *	@date 07/10/11 ��������OFFSET��e�ŊǗ�����悤�ɂ����B
 *	@date 07/10/26 �����\���� CTextDrawer ���g�p����悤�ɂ����B
 *	@date 07/11/30 �R�����g�ǉ��A�C���B
 *	@date 07/12/06 �e�L�X�g�F�ݒ肪���f����Ȃ��o�O���C���B
 *	@date 08/12/09 SetTextPositionOffset() �ǉ��B
 *	@date 10/05/12 ������Ǘ������P�B
 */
class CRgnButtonTx : public CRgnButton, public CDrawTextBase
{
	DEFSUPER(CRgnButton);
	typedef CDrawTextBase _text;
public:

	/// �R���X�g���N�^
	CRgnButtonTx(void) : _super(), m_textOffset(0, 0)
	{
	}

	/// �f�X�g���N�^
	~CRgnButtonTx(void)
	{
	}

	/**
	 * [����] ��񕡐�.
	 *	@note �ێ����Ă�����𕡐����܂��B��������͕������܂���B
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CRgnButtonTx& operator=(const CRgnButtonTx& other)
	{
		_super::operator=(other);
		_text::operator=(other);
		m_textOffset = other.m_textOffset;
		return *this;
	}

	/**
	 * [�ݒ�] �����ʒu�␳.
	 *	@param pos �����ʒu�␳
	 */
	void SetTextPositionOffset(const POINT& pos)
	{
		m_textOffset = pos;
	}

#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/**
	 * [�ݒ�] ������ݒ�.
	 *	@note �{�N���X�́A���b�Z�[�W(WM_SETTEXT)�ɂ��Ή����Ă��܂��B
	 *	@param lpszString ������B
	 */
	void SetWindowText(LPCTSTR lpszString);

	/**
	 * [�ݒ�] �t�H���g�w��.
	 *	@note �{�N���X�́A���b�Z�[�W(WM_SETFONT)�ɂ��Ή����Ă��܂��B
	 *	@param pFont �V�����t�H���g���w�肵�܂��B
	 *	@param bRedraw TRUE �̏ꍇ�� CWnd �I�u�W�F�N�g���ĕ`�悵�܂��B
	 */
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
#endif

protected:

	/**
	 * [�ݒ�] �V���[�g�J�b�g�L�[�ݒ�.
	 *	@note �{�N���X�ł́A�{���\�b�h�͖����ł��B�ݒ肷��ꍇ�A�ʏ�̃{�^���Ɠ����悤�ɁA
	 *			�{�^��������� & �L�[���[�h�����Ă��������B
	 *	@param key �_�~�[
	 */
	void SetShortcutKey(TCHAR key)
	{
	}

	/**
	 * [�ʒm] �`�撆.
	 *		Bitmap�\���̑O��ɃR�[������܂��B
	 *	@param[in,out] _pos �\���ʒu�B
	 *	@param[in] pDC �`����CDC�B
	 *	@param[in] boIsFirst Bitmap�\���O�� true�A�\����� false �Œʒm����܂��B
	 */
	virtual void OnDrawingButton(CPoint& _pos, CDC* pDC, bool boIsFirst)
	{
		if ( boIsFirst || m_strText.IsEmpty() )
		{
			return;
		}
		CFont* pOldFont = pDC->SelectObject(_super::GetFont());
		CRect rect;
 		_super::GetClientRect(rect);
		pDC->SetBkMode(TRANSPARENT);		// �o�b�N�J���[�͕ύX�Ȃ�
		if ( _super::GetButtonState() == ES_Pushed )
		{
			rect.OffsetRect(_pos);
		}
		rect.OffsetRect(m_textOffset);
		DWORD dt = DT_CENTER | DT_VCENTER;
		_text::DrawTextEx(pDC, rect, dt, (_super::GetButtonState() != ES_Disable), m_strText);
		pDC->SelectObject(pOldFont);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		_super::GetWindowText(m_strText);
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
		if ( message == WM_SETTEXT )
		{
			m_strText = reinterpret_cast<LPCTSTR>(lParam);
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	CPoint			m_textOffset;		///< �����̈ʒu����
	CString			m_strText;			///< �e�L�X�g���e
};



}; //MFC
}; //TNB



