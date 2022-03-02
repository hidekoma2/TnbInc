#pragma once
/**
 *@file
 * ��������Edit�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include <imm.h>
#ifndef _WIN32_WCE
 #pragma comment(lib,"imm32.lib")
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ASCII������pEdit�R���g���[��
 *
 *		�Q�o�C�g�R�[�h�̓��͂��֎~�ł��܂��B
 *		�p�����āA m_strValidChars ������������ƗL���ȕ������w��ł��܂��B
 *		IsValidChar() ���I�[�o�[���C�h���邱�ƂŁA���ڍׂȓ��͐������o���܂��B
 *		�܂��A���͂��ꂽ�����������ւ��邱�Ƃ��o���܂��B
 *		OnUpdateChar() ���I�[�o�[���C�h���邱�ƂŁA�ύX�C�x���g���n���h���o���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃG�f�B�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCEdit�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditAscii.h
 * 
 *	@date 06/01/01	�V�K�쐬
 *	@date 06/11/17	CStr �̎g�p���� CString �ɐ؂�ւ��B
 *	@date 07/12/19	Cut&Paste�𐧌��\�ɁBBeep���[�h���p�ӁB
 *	@date 08/05/15	TAB���͂𐮗��B
 *	@date 09/09/16	CE �΍�B
 */
class CEditAscii : public CEdit
{
	DEFSUPER(CEdit);
public:

	/// �R���X�g���N�^
	CEditAscii(void) : _super(), m_boCanCutPaste(true), m_boCanCallUpdate(true), m_boIsPasting(false)
		, m_strValidChars(	_T("!\x22#$%&'()=~|")
							_T("1234567890-^\\")
							_T("@[;:],./")
							_T("`{+*}<>?_")
							_T("abcdefghijklmnopqrstuvwxyz")
							_T("ABCDEFGHIJKMNLOPQRSTUVWXYZ")
							_T(" ") )
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param lpszValidChars �L�������Q�w��B
	 */
	CEditAscii(LPCTSTR lpszValidChars) : _super(), m_boCanCutPaste(true), m_boCanCallUpdate(true), m_boIsPasting(false)
		, m_strValidChars(lpszValidChars)
	{
	}

	/// �f�X�g���N�^
	virtual ~CEditAscii() 
	{
	}

	/**
	 * [�擾] ���͕����擾
	 *	@return ������
	 */
	CString GetText(void) const
	{
		CString s;
		_super::GetWindowText(s);
		return s;
	}

	/**
	 * [�ݒ�] ������ݒ�
	 *	@param lpszText �ݒ肷�镶��
	 *	@param boIsNomoveSel true�Ȃ�SEL�̈ʒu��ς��Ȃ��悤�ɂ���B
	 *						false�Ȃ�SEL�͏����������(���ʂ�SetWindowText�������Ɠ�������)�B
	 */
	void SetText(LPCTSTR lpszText, bool boIsNomoveSel = false)
	{
		if ( ! boIsNomoveSel )
		{
			_super::SetWindowText(lpszText);
		}
		else
		{
			int iCurSel = LOWORD(_super::GetSel());
			_super::SetWindowText(lpszText);
			_super::SetSel(iCurSel, iCurSel);
		}
	}

	/**
	 * [�ݒ�] ���͉\�����ݒ�.
	 *	@param chars ���͉\����
	 */
	void SetValidChars(LPCTSTR chars)
	{
		m_strValidChars = chars;
	}

	/**
	 * [�擾] ���͉\�����擾.
	 *	@return ���͉\����
	 */
	CString GetValidChars(void) const
	{
		return m_strValidChars;
	}

	/**
	 * [�ݒ�] �J�b�g�y�[�X�g���[�h.
	 *	@param r true �Ȃ�y�[�X�g�A�J�b�g�������܂��B
	 *			false �Ȃ狖���܂���B
	 *			�R���X�g���N�^���A�u���v�ɂȂ��Ă��܂��B
	 */
	void SetCutPasteMode(bool r)
	{
		m_boCanCutPaste = r;
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@note IME�𖳌������Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		::ImmAssociateContext(_super::m_hWnd, NULL);	// IME���͋֎~
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
		if ( message == WM_SETTEXT )
		{
			LRESULT l = _super::WindowProc(message, wParam, lParam);
//			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(_super::GetDlgCtrlID(), EN_UPDATE), (LPARAM)m_hWnd);
			MFCLIB::SendCommandMessage(this, EN_UPDATE);
			return l;
		}
		else if ( message == WM_KEYDOWN )
		{
			if ( wParam == VK_TAB && ::GetAsyncKeyState(VK_CONTROL) < 0 )
			{
				return 0;
			}
		}
		else if ( message == WM_CHAR )
		{
			if ( wParam == VK_TAB )
			{
				return 0;
			}
			UINT wp = down_cast<UINT>(wParam);
			LONG lp = down_cast<LONG>(lParam);
			if ( ! IsValidChar(wp, lp, GetText()) )
			{
				OnInputInvalidChar();
				return 0;
			}
		}
		else if ( message == WM_PASTE )
		{
			if ( ! m_boCanCutPaste )
			{
				return 0;
			}
			m_boIsPasting = true;
			_super::SetRedraw(FALSE);
			LRESULT l = _super::WindowProc(message, wParam, lParam);
			m_boIsPasting = false;
			_super::SetRedraw(TRUE);
			_super::Invalidate();
			return l;
		}
		else if ( message == WM_CONTEXTMENU )
		{
			if ( ! m_boCanCutPaste )
			{
				return 0;
			}
		}
		else if ( message == WM_LBUTTONDOWN )
		{
			#ifdef _WIN32_WCE
				return _super::DefWindowProc(message, wParam, lParam);
			#endif
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] for notifications from parent
	 *	@note
	 *		CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *		���b�Z�[�W��M������R�[������܂��B
	 *		WM_COMMAND�̏������t�b�N���Ă��܂��B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE	�����ρB
	 *	@retval FALSE	�������B
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_COMMAND && lParam == reinterpret_cast<LPARAM>(m_hWnd) )
		{
			if ( m_boCanCallUpdate )
			{
				m_boCanCallUpdate = false;
				int ctrlID = _super::GetDlgCtrlID();
				if ( wParam == MAKEWPARAM(ctrlID, EN_UPDATE) )
				{
					if ( m_boIsPasting )
					{
						OnPasteChar();
					}
					OnUpdateChar();
				}
				else if ( wParam == MAKEWPARAM(ctrlID, EN_SETFOCUS) )
				{
					OnEditStart();
				}
				else if ( wParam == MAKEWPARAM(ctrlID, EN_KILLFOCUS) )
				{
					OnEditEnd();
				}
				m_boCanCallUpdate = true;
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [�ʒm] �y�[�X�g���ꂽ��
	 */
	virtual void OnPasteChar(void)
	{
		CString s = GetText();
		LONG lTemp = 0;
		UINT nChar;
		//
		int i = 0;
		bool boHasInvalidChar = false;
		while ( i < s.GetLength() )
		{
			nChar = s[i];
			_super::SetSel(i, i);
			if ( ! IsValidChar(nChar, lTemp, s.Left(i)) )
			{
				s.Delete(i);
				boHasInvalidChar = true;
			}
			else
			{
				s.SetAt(i, static_cast<TCHAR>(nChar));
				i++;
			}
		}
		if ( boHasInvalidChar )
		{
			OnInputInvalidChar();
		}
		SetText(s);
		_super::SetSel(i, i);
	}

	/**
	 * [�m�F] ���͋��`�F�b�N
	 *	@param[in,out] _nChar WM_CHAR �� WPARAM
	 *	@param[in,out] _lFlags WM_CHAR �� LPARAM
	 *	@param[in] strNowText ���͍ς݂̕�����
	 *	@retval true ���͂�F�߂�B
	 *	@retval false ���͂�F�߂Ȃ��B
	 */
	virtual bool IsValidChar(UINT& _nChar, LONG& _lFlags, const CString& strNowText)
	{
		bool r = (_lFlags & 1) != 0;
		if ( r && _nChar == 0x03 )	//Ctrl + C
		{
			return true;
		}
		if ( m_boCanCutPaste )
		{
			if ( r && _nChar == 0x18 )	//Ctrl + X
			{
				return true;
			}
			if ( r && _nChar == 0x16 )	//Ctrl + V
			{
				return true;
			}
		}
		if ( _nChar == '\b' || _nChar == VK_RETURN )
		{
			return true;
		}
		if ( m_strValidChars.Find(static_cast<TCHAR>(_nChar)) >= 0 )
		{
			return true;
		}
		return false;
	}

	/**
	 * [�ʒm] �֎~�������͔������ɒʒm.
	 */
	virtual void OnInputInvalidChar(void)
	{
		::MessageBeep(MB_OK);
	}

	/**
	 * [�ʒm] ���͎��ɒʒm
	 *	@note	�p�����Ė{���\�b�h�ɏ������L�q���܂��B
	 *			�{�֐����� GetText() �Ō��݂̕������擾�� SetText() ���g���Đݒ肷�邱�Ƃ��\�B
	 */
	virtual void OnUpdateChar(void) 
	{
	}

	/**
	 * [�ʒm] �t�H�[�J�X�𓾂����ɒʒm
	 *	@note	�p�����Ė{���\�b�h�ɏ������L�q���܂��B
	 *			�{�֐����� GetText() �Ō��݂̕������擾�� SetText() ���g���Đݒ肷�邱�Ƃ��\�B
	 */
	virtual void OnEditStart(void) 
	{
	}

	/**
	 * [�ʒm] �t�H�[�J�X�����������ɒʒm
	 *	@note	�p�����Ė{���\�b�h�ɏ������L�q���܂��B
	 *			�{�֐����� GetText() �Ō��݂̕������擾�� SetText() ���g���Đݒ肷�邱�Ƃ��\�B
	 */
	virtual void OnEditEnd(void) 
	{
	}

private:
	bool m_boCanCutPaste;		///< Paste�ACut�������邩�H
	CString m_strValidChars;	///< �L�������Q
	bool m_boCanCallUpdate;		///< OnUpdateChar()���R�[�����邩�H
	bool m_boIsPasting;			///< OnPasteChar()���R�[�����邩�H
};



/**@ingroup MFCCONTROL
 * HEX�����I�����[Edit�R���g���[��
 *
 *		0�`9�AA�`F�܂ł̂ݓ��͂��\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 09/04/06	�V�K�쐬
 */
class CEditHex : public CEditAscii
{
	DEFSUPER(CEditAscii);
public:

	/// �R���X�g���N�^
	CEditHex(void) : _super()
	{
		_super::SetValidChars(_T("1234567890") _T("abcdef") _T("ABCDEF"));
	}

protected:
	/**
	 * [�m�F] ���͋��`�F�b�N
	 *	@param[in,out] _nChar WM_CHAR �� WPARAM
	 *	@param[in,out] _lFlags WM_CHAR �� LPARAM
	 *	@param[in] strNowText ���͍ς݂̕�����
	 *	@retval true ���͂�F�߂�B
	 *	@retval false ���͂�F�߂Ȃ��B
	 */
	virtual bool IsValidChar(UINT& _nChar, LONG& _lFlags, const CString& strNowText)
	{
		if ( _nChar >= 'a' && _nChar <= 'f' )
		{
			_nChar -= ('a' - 'A');
		}
		return _super::IsValidChar(_nChar, _lFlags, strNowText);
	}
};



/**@ingroup MFCCONTROL
 * ASCII�t�@�C����Edit�R���g���[��
 *
 *		0�`9�Aa�`z�A�L���̂ݓ��͂��\�ł��B�t�@�C�����Ɏg���Ȃ� \ / : * ? " < > | �͋֎~����܂��B
 *		�S�p���֎~����܂��B�����ӁB
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 14/12/24	�V�K�쐬
 */
class CEditFileName : public CEditAscii
{
	DEFSUPER(CEditAscii);
public:

	/// �R���X�g���N�^
	CEditFileName(void) : _super()
	{
		_super::SetValidChars(
							_T("!#$%&'()=~")
							_T("1234567890-^")
							_T("@[;],.")
							_T("`{+}_")
							_T("abcdefghijklmnopqrstuvwxyz")
							_T("ABCDEFGHIJKMNLOPQRSTUVWXYZ")
							_T(" ") );
	}
};



}; //MFC
}; //TNB
