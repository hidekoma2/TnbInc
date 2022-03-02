#pragma once
/**
 *@file
 *	�^�u�R���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbTextDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �^�u�R���g���[��
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń^�u�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCTabCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@see CCustomWindowFrame ���g���ꍇ�A�^�u�R���g���[���͖{�N���X���g���K�v������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcTabCtrlEx.h
 * 
 *	@date 09/04/22	�V�K�쐬
 *	@date 12/02/29	SetItemText() �V�K�B
 */
class CTabCtrlEx : public CTabCtrl
{
	DEFSUPER(CTabCtrl);
public:

	/// �R���X�g���N�^
	CTabCtrlEx(void) : m_selectTextColor(CLR_INVALID), m_selectBackColor(CLR_INVALID), m_isAttachedSpin(false)
	{
	}

	/**
	 * [�ݒ�] �I���^�u�J���[�ݒ�.
	 *	@param text �e�L�X�g�F
	 *	@param back �w�i�F
	 */
	void SetSelectedColor(COLORREF text, COLORREF back = CLR_INVALID)
	{
		m_selectTextColor = text;
		m_selectBackColor = back;
	}

	/**
	 * [�擾] Item������擾.
	 *	@param nItem �^�u�� 0 ����n�܂�C���f�b�N�X�B
	 *	@return ������B
	 */
	CString GetItemText(int nItem) const
	{
		TC_ITEM	tci;
		CString s;
		tci.mask = TCIF_TEXT;
		tci.pszText = s.GetBuffer(512);
		tci.cchTextMax = 511;
		if ( _super::GetItem(nItem, &tci) )
		{
			s.ReleaseBuffer();
		}
		else
		{
			s.ReleaseBuffer();
			s.Empty();
		}
		return s;
	}

	/**
	 * [�ݒ�] Item������ݒ�.
	 *	@param nItem �^�u�� 0 ����n�܂�C���f�b�N�X�B
	 *	@param lpszText ������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetItemText(int nItem, LPCTSTR lpszText)
	{
		TC_ITEM	tci;
		CString s = lpszText;
		tci.mask = TCIF_TEXT;
		tci.pszText = s.GetBuffer(512);
		tci.cchTextMax = 511;
		BOOL r = _super::SetItem(nItem, &tci);
		s.ReleaseBuffer();
		return !! r;
	}

	/**
	 * [�擾] �X�s���{�^���擾.
	 *	@note �^�u�R���g���[���ɂ��Ă���X�s���{�^����Ԃ��܂��B
	 *	@return �X�s���{�^��
	 */
	CSpinButtonCtrl* GetSpinButtonCtrl(void)
	{
		if ( ! m_isAttachedSpin && ! ::IsWindow(m_spin) )
		{
			m_spin.SubclassWindow(GetDlgItem(1)->GetSafeHwnd());
		}
		return &m_spin;
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		m_isAttachedSpin = false;
		_super::ModifyStyle(0, TCS_OWNERDRAWFIXED);
		_super::PreSubclassWindow();
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
		if ( message == WM_CREATE )
		{
			m_isAttachedSpin = false;
		}
		if ( message == WM_DESTROY )
		{
			if ( ::IsWindow(m_spin) )
			{
				m_spin.UnsubclassWindow();
			}
		}
		if ( ! m_isAttachedSpin && ! ::IsWindow(m_spin) )
		{
			HWND h = GetDlgItem(1)->GetSafeHwnd();
			if ( h != NULL )
			{
				m_spin.SubclassWindow(h);
				m_isAttachedSpin = true;
			}
		}
		if ( message == WM_ERASEBKGND )
		{
			HDC dc = reinterpret_cast<HDC>(wParam);
			LRESULT res = _super::GetParent()->SendMessage(WM_CTLCOLORBTN, reinterpret_cast<WPARAM>(dc), _super::GetDlgCtrlID());
			HBRUSH b = reinterpret_cast<HBRUSH>(res);
			if ( b != NULL )
			{
				CRect rc;
				GetClientRect(rc);
				POINT po;
				MFCLIB::GetControlPos(this, po);
				::SetBrushOrgEx(dc, po.x, po.y, NULL);
				::FillRect(dc, &rc, b);
				return FALSE;
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
	
	/**
	 * [�ʒm] �I�[�i�[�h���[����.
	 * 		�I�[�i�[�h���[���R�[������܂��B
	 *	@param lpDrawItemStruct �`����.
	 */
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if ( lpDrawItemStruct->CtlType != ODT_TAB || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		const CString& s = GetItemText(lpDrawItemStruct->itemID);
		RECT rc = lpDrawItemStruct->rcItem;
		HDC dc = lpDrawItemStruct->hDC;
		LRESULT res = _super::GetParent()->SendMessage(WM_CTLCOLORBTN, reinterpret_cast<WPARAM>(dc), _super::GetDlgCtrlID());
		HBRUSH b = reinterpret_cast<HBRUSH>(res);
		COLORREF c = ::GetTextColor(dc);
		if ( (lpDrawItemStruct->itemState & ODS_SELECTED) != 0 )
		{
			if ( IS_RGBVALUE(m_selectBackColor) )
			{
				CBrush br;
				br.CreateSolidBrush(m_selectBackColor);
				::FillRect(dc, &rc, br);
			}
			else
			{
				::FillRect(dc, &rc, b);
			}
			if ( IS_RGBVALUE(m_selectTextColor) )
			{
				c = m_selectTextColor;
			}
		}
		else
		{
			::FillRect(dc, &rc, b);
		}
		::SetBkMode(dc, TRANSPARENT);
		rc.top += 5;
//		rc.bottom -= 3;//���t���̏ꍇ

		if ( false )
		{
			LOGFONT lf;
			CFont* pFont = GetFont();
			pFont->GetLogFont(&lf);
			lf.lfEscapement = 900;
			CStr s = "@";
			s += lf.lfFaceName;
			_tcscpy(lf.lfFaceName, s);
			CFont f;
			f.CreateFontIndirect(&lf);
			HGDIOBJ h = ::SelectObject(dc, f);
			CTextDrawer::DrawTextRect(dc, rc, DT_CENTER, c, s);
			::SelectObject(dc, h);
			return; 
		}


		CTextDrawer::DrawTextRect(dc, rc, DT_CENTER, c, s);
	}

private:
	COLORREF		m_selectTextColor;		///< �I�𕶎��F
	COLORREF		m_selectBackColor;		///< �I��w�i�F
	CSpinButtonCtrl	m_spin;
	bool			m_isAttachedSpin;
};



}; // MFC
}; // TNB
