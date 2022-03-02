#pragma once
/**
 *@file
 * HEX�f�[�^����Edit�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbClipboard.h"
#include "TnbBitmapImage.h"
#include "TnbMfcBitmapDC.h"
#include "TnbStrOperator.h"
#include "TnbDummyCollection.h"
#include "TnbSelectedBeltDrawer.h"
#include "TnbFontHandle.h"
#include "TnbDcSelectAssistant.h"
#include <imm.h>
#ifndef _WIN32_WCE
 #pragma comment(lib,"imm32.lib")
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * HEX�f�[�^����Edit�R���g���[���e���v���[�g
 *
 *		1byte ���ɃX�y�[�X���󂭂̂ŁA�X�̃f�[�^�����₷���ҏW�ł��܂��B
 *
 *	@note �ҏW���A�e�ɒʒm����� WM_COMMAND �ɂ́A�ȉ��̂��̂�����܂��B
 *		- EN_HSCROLL
 *		- EN_VSCROLL
 *		- EN_SELCHANGE
 *		- EN_CHANGE
 *		- EN_SETFOCUS
 *		- EN_KILLFOCUS
 *
 *	@note WM_PASTE , WM_COPY , WM_CUT �ɂ��Ή����Ă��܂��B
 *
 *	@note �X�[�p�[�N���X�� EDIT�R���g���[��( CEdit )�ł͂Ȃ��̂ŁA�����Ӊ������B
 *	
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���i�G�f�B�b�g�R���g���[���ł��j�𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic or CEdit�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@param WIDTH �\����BYTE���B 1�ȏ���w�肷��ƃ}���`���C���ɂȂ�܂��B-1 �Ȃ�V���O�����C���B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditHexData.h
 * 
 *	@date 10/04/01 �V�K�쐬
 *	@date 10/05/18 EN_CHANGE �̔��s�^�C�~���O�C���B�J���b�g�\���C���B
 *	@date 10/06/22 �e���L�[�̓��͂Ƀo�O�L��B�C���B
 *	@date 11/10/19 Disable ��Ԃ̃R���g���[���̕\�L���C���B
 *	@date 15/05/07 �y�[�X�g(ReplaceSel())������ a�`f �����������͂���Ȃ��o�O���C���B
 */
template<int WIDTH = -1>
class CEditHexDataT : public CWnd
{
	DEFSUPER(CWnd);
public: 

	/// �R���X�g���N�^.
	CEditHexDataT(void) : m_maxLength(-1), m_isInsertMode(false), m_isEnableNotify(false)
		, m_selectCaretIndex(-1), m_isCaletVisidle(false), m_isShowCaretAlways(false)
	{
		ASSERT( WIDTH != 0 );
		m_margin.x = 3;
		m_margin.y = 2;
		m_textColor = ::GetSysColor(COLOR_WINDOWTEXT);
		m_backColor = ::GetSysColor(COLOR_WINDOW);
		m_activeHighlightColor = ::GetSysColor(COLOR_HIGHLIGHT);
		m_inactiveHighlightColor = ::GetSysColor(COLOR_BTNSHADOW);
		m_hasCaret = false;
	}
	
	/**
	 * [�ݒ�] �}�[�W���ݒ�.
	 *		����̃}�[�W���i�����̕\�������܂ł̋�ԁj���w��ł��܂��B
	 *	@note ���ݒ�̏ꍇ�A �� 3�A�c 2 dot �ł��B
	 *	@param p �}�[�W��
	 */
	void SetMargins(const POINT& p)
	{
		m_margin = p;
		m_Redraw();
	}

	/**
	 * [�ݒ�] �C���T�[�g���[�h�ݒ�.
	 *		�{�R���g���[���́A�C���T�[�g���[�h�Ə㏑�����[�h������܂��B
	 *	@note �f�t�H���g�ł͏㏑�����[�h�ł��B ���[�U��[insert]�L�[�ł�����؂�ւ��邱�Ƃ��o���܂��B
	 *	@param isEnable true �Ȃ�C���T�[�g���[�h�Bfalse �Ȃ�㏑�����[�h�ł��B
	 */
	void SetInsertMode(bool isEnable)
	{
		m_isInsertMode = isEnable;
		m_Redraw();
	}

	/**
	 * [�擾] �C���T�[�g���[�h�擾.
	 *		�{�R���g���[���́A�C���T�[�g���[�h�Ə㏑�����[�h������܂��B
	 *	@retval true �C���T�[�g���[�h
	 *	@retval false �㏑�����[�h
	 */
	bool IsInsertMode(void) const
	{
		return m_isInsertMode;
	}

	/**
	 * [�ݒ�] �ő咷�ݒ�.
	 *		���͂ł���BYTE�����w��ł��܂��B�w�肵�Ȃ��ꍇ�A�����ɓ��͂ł��܂��B
	 *	@note �}���`���C���̏ꍇ�A�ő咷���w�肷��ƁA�T�C�Y�Œ�ɂȂ�܂��B
	 *	@note ���͏���Empty�ɂȂ�܂��B
	 *	@param maxLength �ő咷�B�ȗ�����ƁA�����ɂȂ�܂��B
	 *	@param defaultData �}���`���C���̏ꍇ�A�f�t�H���g�̒l���w��ł��܂��B
	 */
	void SetLimitLength(int maxLength = -1, BYTE defaultData = 0)
	{
		m_maxLength = maxLength;
		m_defaultData = defaultData;
		m_SetData(CByteVector());
	}

	/**
	 * [�擾] �}���`���C���m�F.
	 *	@retval true �}���`���C�����[�h
	 *	@retval false �V���O�����C�����[�h
	 */
	bool IsMultiLineMode(void) const
	{
		return WIDTH > 0;
	}

	/**
	 * [�擾] ���̓f�[�^�擾.
	 *	@return ���̓f�[�^.
	 */
	const CByteVector& GetData(void) const
	{
		return m_data;
	}

	/**
	 * [�ݒ�] �f�[�^�ݒ�.
	 *	@param vb �f�[�^.
	 */
	void SetData(const CByteVector& vb)
	{
		m_SetData(vb);
	}

	/**
	 * [�擾] ���s���擾.
	 *		�N���C�A���g�̈�ɕ\���ł���s����Ԃ��܂��B
	 *	@return �s��
	 */
	int GetVisibleLineCount(void) const
	{
		if ( IsMultiLineMode() )
		{
			CRect rc;
			_super::GetClientRect(rc);
			return (rc.Height() + m_charSize.cy - 1) / m_charSize.cy;
		}
		return 1;
	}

	/**
	 * [�擾] �s���擾.
	 *		�S�f�[�^�̍s����Ԃ��܂��B
	 *	@return �s��
	 */
	int GetLineCount(void) const
	{
		if ( IsMultiLineMode() )
		{
			return (m_data.GetSize() + WIDTH - 1) / WIDTH;
		}
		return 1;
	}

	/**
	 * [�擾] ���ŏ�s�擾.
	 *		�\�����Ă���ŏ�̍s����Ԃ��܂��B
	 *	@return �s���B��ԏオ�O�B
	 */
	int GetFirstVisibleLine(void) const
	{
		if ( IsMultiLineMode() )
		{
			return m_viewPos.y / m_charSize.cy;
		}
		return 0;
	}

	/**
	 * [�ݒ�] ���ŏ�s�ݒ�.
	 *		��ԏ�ɕ\������s���w�肵�܂��B
	 *	@param line �s���B
	 */
	void SetFirstVisibleLine(int line)
	{
		if ( IsMultiLineMode() )
		{
			int y = line;
			int l = GetLineCount() - GetVisibleLineCount() + 1;
			if ( y < 0 )
			{
				y = 0;
			}
			else if ( y > l )
			{
				y = l;
			}
			y *= m_charSize.cy;
			if ( y != m_viewPos.y )
			{
				m_viewPos.y = y;
				m_Redraw();
				NotifyToParent(EN_VSCROLL);
			}
		}
	}

	/**
	 * [�ݒ�] �I���ʒu�ݒ�.
	 *		�w��̃C���f�b�N�X�ɃJ���b�g�ʒu���ړ����܂��B
	 *	@param index �C���f�b�N�X
	 */
	void SetSel(int index)
	{
		m_ReleaseSelect();
		m_SetCaretIndex(index);
		m_Redraw();
	}

	/**
	 * [�ݒ�] �I���ʒu�ݒ�.
	 *		�w��̃C���f�b�N�X�Ԃ�I����Ԃɂ��܂��B
	 *	@param startIndex �I���J�n�C���f�b�N�X
	 *	@param endIndex �I���I���C���f�b�N�X
	 */
	void SetSel(int startIndex, int endIndex)
	{
		if ( startIndex == endIndex )
		{
			m_ReleaseSelect();
			m_SetCaretIndex(startIndex);
			m_Redraw();
			return;
		}
		if ( endIndex < 0 )
		{
			endIndex = m_GetIndexMax();
		}
		m_ReleaseSelect();
		m_SetCaretIndex(startIndex, false, true);
		ASSERT( m_caretDrawPos.x >= 0 );
		m_SetSelect(m_caretIndex, m_caretDrawPos);
		m_SetCaretIndex(endIndex);
		m_Redraw();
	}

	/**
	 * [�擾] �I���ʒu�擾.
	 *		�J���b�g�̈ʒu��A�I��͈͂��擾���܂��B
	 *	@note _startIndex �� _endIndex �������ꍇ�A���I���Ɣ��f�ł��܂��B
	 *	@param[out] _startIndex �I����Ԃ̏ꍇ�A�I���J�n�ʒu���Ԃ�܂��B�ʏ�̏ꍇ�A�J���b�g�̈ʒu������܂��B
	 *	@param[out] _endIndex �I����Ԃ̏ꍇ�A�I���I���ʒu���Ԃ�܂��B�ʏ�̏ꍇ�A�J���b�g�̈ʒu������܂��B
	 */
	void GetSel(int& _startIndex, int& _endIndex) const
	{
		if ( m_selectCaretIndex < 0 )
		{
			// ���I�����
			_startIndex = m_caretIndex;
			_endIndex = m_caretIndex;
		}
		else
		{
			// �I�����
			_startIndex = m_selectCaretIndex;
			_endIndex = m_caretIndex;
		}
	}

	/**
	 * [�u��] �f�[�^�u��.
	 *		�I������Ă��镔�����w��̃f�[�^�ɒu�����܂��B
	 *		�I������Ă��Ȃ��ꍇ�A�C���T�[�g���[�h�̏ꍇ�J���b�g�̈ʒu����}�����܂��B�㏑�����[�h�̏ꍇ�A�㏑�����܂��B
	 *	@note ������́A 0�`9, A�`F, a�`f �ȊO�͖������܂��B
	 *	@param lpszText ������
	 */
	void ReplaceSel(LPCTSTR lpszText)
	{
		CString s = lpszText;
		if ( ! s.IsEmpty() )
		{
			bool nowInsertMode = m_isInsertMode;
			if ( m_selectCaretIndex >= 0 )
			{
				m_isInsertMode = true;
			}
			m_isEnableNotify = false;
			_super::SetRedraw(false);
			s.MakeUpper(); // �啶����
			loop ( i, s.GetLength() )
			{
				m_Input(s.GetAt(i), false);
			}
			_super::SetRedraw(true);
			m_isEnableNotify = true;
			m_isInsertMode = nowInsertMode;
			NotifyToParent(EN_SELCHANGE);
			NotifyToParent(EN_CHANGE);
		}
		else
		{
			m_DeleteSelectedData();
			m_ReleaseSelect();
		}
		m_Redraw();
	}

	/**
	 * [�u��] �f�[�^�u��.
	 *		�I������Ă��镔�����w��̃f�[�^�ɒu�����܂��B
	 *		�I������Ă��Ȃ��ꍇ�A�C���T�[�g���[�h�̏ꍇ�J���b�g�̈ʒu����}�����܂��B�㏑�����[�h�̏ꍇ�A�㏑�����܂��B
	 *	@param vb �f�[�^.
	 */
	void ReplaceSel(const CByteVector& vb)
	{
		ReplaceSel(CStrOperator::BinaryToHexString(vb));
	}

	/**
	 * [�ݒ�] �c�[���`�b�v�ǉ�.
	 *		���͍ς݂̃f�[�^�ʂ��c�[���`�b�v�ŕ\������悤�ɂ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AddToolTip(void)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			m_toolTip.Create(this, TTS_ALWAYSTIP);
			m_toolTip.AddTool(this, _T(""));
			m_toolTip.Activate(TRUE);
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] ��ɃJ���b�g�\��.
	 *	@note �t�H�[�J�X�����������A�J���b�g�̈ʒu���킩��悤�ɂ��܂��B
	 */
	void ShowCaretAlways(void)
	{
		m_isShowCaretAlways = true;
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *	@param color �F
	 *	@return �ݒ�O�̕����F
	 */
	COLORREF SetTextColor(COLORREF color)
	{
		COLORREF r = m_textColor;
		m_textColor = color;
		m_Redraw();
		return r;
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *	@param color �F
	 *	@return �ݒ�O�̔w�i�F
	 */
	COLORREF SetBkColor(COLORREF color)
	{
		COLORREF r = m_backColor;
		m_backColor = color;
		m_Redraw();
		return r;
	}

	/**
	 * [�ݒ�] �n�C���C�g�F�ݒ�.
	 *	@param activeColor �A�N�e�B�u���̔w�i�F
	 *	@param inactiveColor ��A�N�e�B�u���̔w�i�F
	 */
	void SetHighlightColor(COLORREF activeColor, COLORREF inactiveColor)
	{
		m_activeHighlightColor = activeColor;
		m_inactiveHighlightColor = inactiveColor;
	}

	/**
	 * [�쐬] �R���e�L�X�g���j���[�쐬.
	 *	@param[out] _menu �쐬�������j���[���i�[����܂��B
	 *	@param[in] canCopy �R�s�[�A�J�b�g���\�Ȃ� true ���w�肵�܂��B
	 *	@param[in] topMenuId ���j���[ID���w�肵�܂��B�w��� ID���؂���A+1���R�s�[�A+2���y�[�X�g�Ɋ��蓖�Ă��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool MakeContextMenu(CMenu& _menu, bool canCopy, int topMenuId)
	{
		if ( _menu.GetSafeHmenu() != NULL )
		{
			_menu.DestroyMenu();
		}
		BOOL r = TRUE;
		r &= _menu.CreatePopupMenu();
		CClipboard clipb;
		bool canPaste = clipb.IsAvailable();
		r &= _menu.AppendMenu(MF_STRING | (canCopy ? 0 : MF_GRAYED), topMenuId + 0, _T("�؂���(&T)"));
		r &= _menu.AppendMenu(MF_STRING | (canCopy ? 0 : MF_GRAYED), topMenuId + 1, _T("�R�s�[(&C)"));
		r &= _menu.AppendMenu(MF_STRING | (canPaste ? 0 : MF_GRAYED), topMenuId + 2, _T("�\��t��(&P)"));
		return !! r;
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
		m_hasCaret = false;
		::ImmAssociateContext(_super::GetSafeHwnd(), NULL);	// IME���͋֎~
		::SetClassLong(_super::GetSafeHwnd(), GCL_HCURSOR, reinterpret_cast<LONG>(::LoadCursor(NULL, IDC_IBEAM)));
		m_font.Set(_super::GetSafeHwnd());
		m_SetData(CByteVector());
		_super::PreSubclassWindow();
		m_isEnableNotify = true;
		m_CheckCharSize();
	}
		
	/**
	 * [�ʒm] for translating Windows messages in main message pump
	 *	@param pMsg ���b�Z�[�W
	 *	@retval TRUE �f�B�X�p�b�`�ł��Ȃ��ꍇ
	 *	@retval FALSE �f�B�X�p�b�`����K�v������ꍇ
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if ( ::IsWindow(m_toolTip) )
		{
			m_toolTip.RelayEvent(pMsg);
		}
		return _super::PreTranslateMessage(pMsg);
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
		switch ( message )
		{
		case WM_DESTROY:
			m_toolTip.DestroyWindow();
			m_isEnableNotify = false;
			break;
		case WM_SETFONT:
			{
				HFONT hFont = reinterpret_cast<HFONT>(wParam);
				if ( hFont != NULL )
				{
					m_font.SetClone(hFont);
					m_CheckCharSize();
					if ( LOWORD(lParam) != 0 )
					{
						m_Redraw();
					}
				}
			}
			break;
		case WM_PAINT:
			m_DrawClient(CPaintDC(this));
			return 0;
			break;
		case WM_SETTEXT:
			m_OnSetText(reinterpret_cast<LPCTSTR>(lParam));
			return 1;
			break;
		case WM_GETTEXTLENGTH:
			if ( m_dataString.GetLength() > 0 )
			{
				return (m_dataString.GetLength() - 1) * sizeof(TCHAR);
			}
			return 0;
			break;
		case WM_GETTEXT:
			if ( wParam > 0 && m_dataString.GetLength() > 0 )
			{
				LPTSTR pDst = reinterpret_cast<TCHAR*>(lParam);
				LPCTSTR pSrc = m_dataString;
				size_t n = m_dataString.GetLength() - 1; //�Ō��SP������
				size_t m = min(wParam, n);
				loop ( i, m )
				{
					*pDst++ = *pSrc++;
				}
				*pDst = 0;
				return m * sizeof(TCHAR);
			}
			return 0;
			break;
		case WM_SETFOCUS:
			m_Redraw();
			NotifyToParent(EN_SETFOCUS);
			return 0;
			break;
		case WM_KILLFOCUS:
			m_HideCaret();
			m_Redraw();
			NotifyToParent(EN_KILLFOCUS);
			return 0;
			break;
		case WM_KEYDOWN:
			return m_OnChar(wParam, lParam);
		case WM_KEYUP:
		case WM_CHAR:
			return 0;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			_super::SetFocus();
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			return m_OnMouse(message, wParam, lParam);
			break;
		case WM_MOUSEWHEEL:
			{
				short d = HIWORD(wParam);
				SetFirstVisibleLine(GetFirstVisibleLine() - (d / 120));
			}
			break;
		case WM_ERASEBKGND:
			if ( IsMultiLineMode() )
			{
				return 0;
			}
			if ( IsWindowEnabled() )
			{
				HDC hdc = reinterpret_cast<HDC>(wParam);
				CRect rect;
				GetClientRect(rect);
				CBrush b(m_backColor);
				::FillRect(hdc, &rect, b);
				return 0;
			}
			break;
		case WM_ENABLE:
			{
				CWnd* pParent = GetParent();
				if ( pParent != NULL )
				{
					CRect rect;
					GetClientRect(rect);
					ClientToScreen(rect);
					pParent->ScreenToClient(rect);
					pParent->InvalidateRect(rect, TRUE);
				}
				Invalidate(TRUE);
			}
			return 0;
			break;
		case WM_GETDLGCODE:
			return DLGC_WANTCHARS | DLGC_WANTARROWS;
			break;
		case WM_COPY:
		case WM_CUT:
			if ( m_selectCaretIndex >= 0 && m_caretIndex != m_selectCaretIndex )
			{
				CClipboard clipb;
				int x1 = m_GetCaretIndex() / 2;
				int x2 = m_selectCaretIndex / 2;
				CStr s = CStrOperator::BinaryToHexString(m_data.Mid(min(x1, x2), ::abs(x1 - x2)));
				clipb.SetString(s);
				if ( message == WM_CUT )
				{
					m_DeleteSelectedData();
					m_ReleaseSelect();
					m_Redraw();
				}
				return 1;
			}
			return 0;
			break;
		case WM_PASTE:
			ReplaceSel(CClipboard().GetString());
			return 1;
			break;
		case WM_CONTEXTMENU:
			{
				CMenu menu;
				bool canCopy = (m_selectCaretIndex >= 0);
				if ( MakeContextMenu(menu, canCopy, 100) )
				{
					CPoint po(lParam);
					if ( po.x == -1 && po.y == -1 )
					{
						po = _super::GetCaretPos();
						if ( po.x < 0 )
						{
							::GetCursorPos(&po);
						}
						else
						{
							_super::ClientToScreen(&po);
							po.y += m_charSize.cy;
						}
					} 
					TrackPopupMenuEx(menu, 0, po.x, po.y, _super::GetSafeHwnd(), NULL);
				}
				return 0;
			}
			break;
		case WM_COMMAND:
			{
				switch ( LOWORD(wParam) )
				{
				case 100:
					SendMessage(WM_CUT);
					break;
				case 101:
					SendMessage(WM_COPY);
					break;
				case 102:
					SendMessage(WM_PASTE);
					break;
				}
			}
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] �e�E�B���h�E�֒ʒm.
	 *		EN_UPDATE �ȂǁA�e�E�B���h�E�� WM_COMMAND ��ʒm���܂��B
	 *	@param notifyCode �R�[�h
	 */
	virtual void NotifyToParent(WORD notifyCode)
	{
		if ( m_isEnableNotify )
		{
			MFCLIB::SendCommandMessage(this, notifyCode);
		}
	}

	/**
	 * [�擾] �����F�擾.
	 *	@return �R���g���[���̖�����Ԃ��l�����������F
	 */
	COLORREF GetTextColor(void) const
	{
		return IsWindowEnabled() ? m_textColor : ::GetSysColor(COLOR_GRAYTEXT);
	}

	/**
	 * [�擾] �w�i�F�擾.
	 *	@return �R���g���[���̖�����Ԃ��l�������w�i�F
	 */
	COLORREF GetBkColor(void) const
	{
		return IsWindowEnabled() ? m_backColor : ::GetSysColor(COLOR_MENU);
	}

private:
	// �ĕ\��
	void m_Redraw(void)
	{
		_super::Invalidate();
	}

	// �����̑傫���`�F�b�N
	void m_CheckCharSize(void)
	{
		CClientDC dc(this);
		HGDIOBJ hOld = ::SelectObject(dc, m_font);
		m_charSize = dc.GetTextExtent(_T("8"));
		::SelectObject(dc, hOld);
	}

	/**
	 * [�擾] INDEX�ő�l.
	 *		���ݎw��\��INDEX�̍ő�l��Ԃ��܂��B
	 *	@return �ő�l�B
	 */
	int m_GetIndexMax(void) const
	{
		return m_data.GetSize() * 2;
	}

	/**
	 * [�ʒm] WM_SETTEXT �ʒm�@
	 *		�������HEX������Ƃ��ĉ�͂��A�{�R���g���[���̃f�[�^�Ƃ��܂��B
	 *	@param lpsz �ʒm���ꂽ������
	 */
	void m_OnSetText(LPCTSTR lpsz)
	{
		m_SetData(CStrOperator::HexStringToBinary(lpsz));
	}

	/**
	 * [�ݒ�] �f�[�^�ݒ�.
	 *	@note �J���b�g�ʒu�����������A�I����Ԃ��������܂��B
	 *	@param vb �f�[�^.
	 */
	void m_SetData(const CByteVector& vb)
	{
		m_data = vb;
		if ( m_maxLength > 0 && ToInt(m_data.GetSize()) > m_maxLength )
		{
			m_data.SetSize(m_maxLength);
		}
		m_MakeViewData();
		m_viewPos.x = 0;
		m_viewPos.y = 0;
		m_SetCaretIndex(0);
		m_ReleaseSelect();
		m_Redraw();
		NotifyToParent(EN_SELCHANGE);
		NotifyToParent(EN_CHANGE);
		NotifyToParent(EN_VSCROLL);
		NotifyToParent(EN_HSCROLL);
	}
	
	/**
	 * [�w��] �J���b�g�̈ʒu�w��
	 *	@param p �J���b�g�ʒu.
	 *	@param isNotify true �Ȃ�e�ɒʒm.
	 *	@param isCalcDrawPos true �Ȃ�A�����ɕ`��ʒu�̌v�Z���s��
	 */
	void m_SetCaretIndex(int p, bool isNotify = true, bool isCalcDrawPos = false)
	{
		if ( p < 0 )
		{
			p = 0;
		}
		int im = m_GetIndexMax();
		if ( p > im )
		{
			p = im;
		}
		bool r = (m_caretIndex != p);
		if ( r )
		{
			m_caretIndex = p;
			m_caretDrawPos.x = -1;
		}
		if ( isCalcDrawPos && m_caretDrawPos.x < 0 )
		{
			CClientDC dc(this);
			HGDIOBJ hOld = ::SelectObject(dc, m_font);
			m_caretDrawPos = m_IndexToClientPoint(dc, m_caretIndex);
			::SelectObject(dc, hOld);
		}
		if ( r )
		{
			m_isCaletVisidle = true;
			if ( isNotify )
			{
				NotifyToParent(EN_SELCHANGE);
			}
		}
	}

	// �J���b�g�̃C���f�b�N�X�擾
	int m_GetCaretIndex(void) const
	{
		return m_caretIndex;
	}

	/**
	 * [�ʒm] �}�E�X���b�Z�[�W�ʒm
	 *	@param message WM_LBUTTONUP, WM_LBUTTONDOWN, WM_MOUSEMOVE �̂����ꂩ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return 0
	 */
	LRESULT m_OnMouse(UINT message, WPARAM wParam, LPARAM lParam)
	{
		bool isCaptrued = (::GetCapture() == _super::GetSafeHwnd());
		if ( message == WM_LBUTTONUP )
		{
			// UP
			if ( isCaptrued )
			{
				::ReleaseCapture();
				if ( m_selectCaretIndex == m_GetCaretIndex() )
				{
					m_ReleaseSelect();
					m_Redraw();
				}
			}
			return 0;
		}
		if ( message == WM_MOUSEMOVE && ! isCaptrued )
		{
			return 0;
		}
		CPoint po(lParam);
		CClientDC dc(this);
		HGDIOBJ hOld = ::SelectObject(dc, m_font);
		int l = m_ClientPointToIndex(dc, po);
		::SelectObject(dc, hOld);
		if ( message == WM_LBUTTONDOWN )
		{
			bool isShift = (::GetAsyncKeyState(VK_SHIFT) < 0);
			if ( m_selectCaretIndex < 0 )
			{
				if ( isShift )
				{
					m_SetSelect(m_caretIndex, m_caretDrawPos);
					::SetCapture(_super::GetSafeHwnd());
				}
			}
			else
			{
				if ( ! isShift )
				{
					m_ReleaseSelect();
				}
			}
		}
		m_SetCaretIndex(l, true, true);
		if ( ! isCaptrued )
		{
			CPoint spo = po - m_margin;
			_super::ClientToScreen(&spo);
			if ( ::DragDetect(m_hWnd, spo) )
			{
				// �v���X�I
				m_SetSelect(m_caretIndex, m_caretDrawPos);
				::SetCapture(_super::GetSafeHwnd());
			}
		}
		_super::SetFocus();
		m_Redraw();
		return 0;
	}
	
	/**
	 * [�ʒm] WM_CHAR �ʒm�@
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return 0
	 */
	LRESULT m_OnChar(WPARAM wParam, LPARAM lParam)
	{
		if ( (lParam & _BIT(31)) != 0 )
		{
			//�����ꂽ���͂Ȃɂ����邱�Ƃ��Ȃ��B
			return 0;
		}
		bool isShift = (::GetAsyncKeyState(VK_SHIFT) < 0);
		bool isCtrl = (::GetAsyncKeyState(VK_CONTROL) < 0);
		//
		if ( isShift && wParam == VK_INSERT )
		{
			isShift = false;
			isCtrl = true;
			wParam = 'V';
		}
		if ( isCtrl )
		{
			switch ( wParam )
			{
			case 'A':
				SetSel(0, -1);
				break;
			case 'C':
				SendMessage(WM_COPY);
				break;
			case 'X':
				SendMessage(WM_CUT);
				break;
			case 'V':
				SendMessage(WM_PASTE);
				break;
			default:
				break;
			}
			return 0;
		}
		m_Input(wParam, isShift);
		return 0;
	}

	/**
	 * [�ǉ�] ���́@
	 *	@param ch �L�����R�[�h
	 *	@param isShift �V�t�g���
	 */
	void m_Input(WPARAM ch, bool isShift)
	{
		bool isWork = true;
		bool isSendChange = false;
		int nowPosition = m_GetCaretIndex();
		POINT nowDrawPos = m_caretDrawPos;
		//
		if ( ! IsMultiLineMode() )
		{
			// SingleLine
			switch( ch )
			{
			case VK_UP:
			case VK_PRIOR:
				ch = VK_HOME;
				break;
			case VK_DOWN:
			case VK_NEXT:
				ch = VK_END;
				break;
			}
		}
		//
		switch ( ch )
		{
		case VK_INSERT:
			m_isInsertMode = ! m_isInsertMode;
			NotifyToParent(EN_SELCHANGE);
			isWork = false;
			if ( m_hasCaret )
			{
				POINT p = _super::GetCaretPos();
				if ( m_isInsertMode )
				{
					_super::CreateSolidCaret(2, m_charSize.cy);
				}
				else
				{
					_super::CreateSolidCaret(m_charSize.cx, m_charSize.cy);
				}
				_super::ShowCaret();
				_super::SetCaretPos(p);
			}
			break;
		case VK_DELETE:
			if ( ! m_DeleteSelectedData() )
			{
				int x = m_GetCaretIndex() / 2;
				m_SetCaretIndex(x * 2);
				m_data.Remove(x);
				m_MakeViewData();
				isSendChange = true;
			}
			else
			{
				m_ReleaseSelect();
			}
			isShift = false;
			break;
		case VK_BACK:
			if ( ! m_DeleteSelectedData() )
			{
				if ( m_GetCaretIndex() == 0 )
				{
					break;
				}
				int x = (m_GetCaretIndex() - 1) / 2;
				m_SetCaretIndex(x * 2);
				m_data.Remove(x);
				m_MakeViewData();
				isSendChange = true;
			}
			else
			{
				m_ReleaseSelect();
			}
			isShift = false;
			break;
		case VK_HOME:
			m_SetCaretIndex(0);
			break;
		case VK_END:
			m_SetCaretIndex(m_GetIndexMax());
			break;
		case VK_PRIOR:
		case VK_NEXT:
			{
				int s = ((nowPosition & 1) == 0) ? 0 : 1;
				int w = WIDTH * 2 * (GetVisibleLineCount() - 1);
				if  ( ch == VK_PRIOR )
				{
					w = -w;
				}
				int n = nowPosition + w + s;
				m_SetCaretIndex(n);
			}
			break;
		case VK_UP:
			{
				int s = ((nowPosition & 1) == 0) ? 0 : 1;
				int n = nowPosition - WIDTH * 2 - s;
				if ( n >= 0 )
				{
					m_SetCaretIndex(n);
				}
			}
			break;
		case VK_DOWN:
			{
				int s = ((nowPosition & 1) == 0) ? 0 : 1;
				int n = nowPosition + WIDTH * 2 + s;
				if ( n < m_GetIndexMax() )
				{
					m_SetCaretIndex(n);
				}
			}
			break;
		case VK_RIGHT:
			if ( ! isShift && m_selectCaretIndex >= 0 )
			{
				// �I�����
				if ( m_selectCaretIndex < nowPosition )
				{
					break;
				}
			}
			if ( m_GetIndexMax() > nowPosition )
			{
				int s = ((nowPosition & 1) == 0) ? 2 : 1;
				m_SetCaretIndex(nowPosition + s);
			}
			break;
		case VK_LEFT:
			if ( ! isShift && m_selectCaretIndex >= 0 )
			{
				// �I�����
				if ( m_selectCaretIndex > nowPosition )
				{
					break;
				}
			}
			if ( nowPosition > 0 )
			{
				int s = ((nowPosition & 1) == 0) ? 2 : 1;
				m_SetCaretIndex(nowPosition - s);
			}
			break;
		default:
			if ( isShift )
			{
				isWork = false;
			}
			else
			{
				int c = -1;
				if ( ch >= VK_NUMPAD0 && ch <= VK_NUMPAD9 )
				{
					c = ch - VK_NUMPAD0;
				}
				else
				{
					c = STRLIB::HexCharToInt(ch);
				}
				if ( c < 0 )
				{
					isWork = false;
				}
				else
				{
					bool isDeleted = m_DeleteSelectedData();
					bool isLead = ((m_GetCaretIndex() & 1) == 0);
					INDEX x = m_GetCaretIndex() / 2;
					if ( m_data.GetSize() <= x )
					{
						//���ɂ���
						if ( m_maxLength > 0 && m_maxLength <= ToInt(m_data.GetSize()) )
						{
							break;
						}
						else
						{
							x = m_data.GetSize();
							m_data.SetSize(x + 1);
							m_data[x] = static_cast<BYTE>(c << 4);
							m_MakeViewData();
							isSendChange = true;
						}
					}
					else if ( isLead )
					{
						if ( m_isInsertMode || isDeleted )
						{
							if ( IsMultiLineMode() && m_maxLength > 0 )
							{
								// �}���`���C�������~�b�g
								m_data.Insert(x, static_cast<BYTE>(c << 4));
								m_MakeViewData();
								isSendChange = true;
							}
							else 
							{
								if ( m_maxLength > 0 && m_maxLength <= ToInt(m_data.GetSize()) )
								{
									break;
								}
								else
								{
									m_data.Insert(x, static_cast<BYTE>(c << 4));
									m_MakeViewData();
									isSendChange = true;
								}
							}
						}
						else
						{
							m_data[x] &= 0x0F;
							m_data[x] |= static_cast<BYTE>(c << 4);
							m_MakeViewData();
							isSendChange = true;
						}
					}
					else
					{
						m_data[x] &= 0xF0;
						m_data[x] |= static_cast<BYTE>(c);
						m_MakeViewData();
						isSendChange = true;
					}
					m_SetCaretIndex(m_GetCaretIndex() + 1);
				}
			}
			break;
		}
		if ( isWork )
		{
			if ( isShift && m_selectCaretIndex < 0 )
			{
				m_SetSelect(nowPosition, nowDrawPos);
			}
			if ( ! isShift && m_selectCaretIndex >= 0 )
			{
				m_ReleaseSelect();
			}
			m_Redraw();
		}
		if ( isSendChange )
		{
			NotifyToParent(EN_CHANGE);
		}
	}

	/**
	 * �w��̃N���C�A���g���W�� INDEX ��Ԃ��B
	 */
	int m_ClientPointToIndex(CDC& dc, const POINT& point)
	{
		CPoint po = point;
		po = po - m_margin + m_viewPos;
		if ( po.x < 0 )
		{
			po.x = 0;
		}
		if ( po.y < 0 )
		{
			po.y = 0;
		}
		int w = (po.x) / m_charSize.cx;
		int h = (po.y) / m_charSize.cy;
		if ( IsMultiLineMode() )
		{
			//== Multi Line
			w = w / 3 * 2;
			if ( w > WIDTH * 2 )
			{
				w = WIDTH * 2;
			}
			return w + h * (WIDTH * 2);
		}
		//== Single Line
		while ( m_dataString.GetLength() > w )
		{
			if ( po.x < dc.GetTextExtent(m_dataString.Left(w + 1)).cx )
			{
				break;
			}
			w++;
		}
		w = w / 3 * 2;
		if ( w > m_GetIndexMax() )
		{
			w = m_GetIndexMax();
		}
		return w;
	}

	/// �C���f�b�N�X���N���C�A���g���W�ɕϊ�
	CPoint m_IndexToClientPoint(CDC& dc, int index)
	{
		CPoint pos;
		if ( IsMultiLineMode() )
		{
			//== MultiLine
			int x = index % (WIDTH * 2);
			pos.x = (x + (x / 2)) * m_charSize.cx;
			pos.y = (index / (WIDTH * 2)) * m_charSize.cy;
			if ( m_maxLength > 0 )
			{
				//���~�b�g�L��
				if ( index == m_GetIndexMax() )
				{
					pos.x = (WIDTH * 3) * m_charSize.cx;
					pos.y -= m_charSize.cy;
				}
			}
		}
		else
		{
			//== SingleLine
			int l = index + (index / 2);
			CPoint p = dc.GetTextExtent(m_dataString.Left(l));
			pos.x = p.x;
			pos.y = 0;
		}
		return pos;
	}

	/**
	 * [�폜] �͈͓��폜.
	 *		�I������Ă���͈͂��폜���A�J���b�g�̈ʒu�𒲐����܂��B
	 *	@retval true �폜�������s�����B
	 *	@retval false �Ȃɂ����Ă��Ȃ��B
	 */
	bool m_DeleteSelectedData(void)
	{
		if ( m_selectCaretIndex >= 0 && m_caretIndex != m_selectCaretIndex )
		{
			int x1 = m_GetCaretIndex() / 2;
			int x2 = m_selectCaretIndex / 2;
			m_data.RemoveElements(min(x1, x2), ::abs(x1 - x2));
			m_MakeViewData();
			if ( m_GetCaretIndex() > m_selectCaretIndex )
			{
				m_SetCaretIndex(m_selectCaretIndex);
			}
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �\���p�f�[�^�X�V.
	 *		�\���p������ƃc�[���`�b�v���X�V���܂��B
	 */
	void m_MakeViewData(void)
	{
		if ( IsMultiLineMode() && m_maxLength > 0 )
		{
			// �}���`���C�������~�b�g
			if ( ToInt(m_data.GetSize()) > m_maxLength )
			{
				m_data.SetSize(m_maxLength);
			}
			CDummyCollectionT<BYTE> dc(m_defaultData);
			dc.SetSize(m_maxLength - m_data.GetSize());
			m_data.Append(dc);
			ASSERT( ToInt(m_data.GetSize()) == m_maxLength );
		}
		//
		TCHAR* P = m_dataString.GetBuffer(m_data.GetSize() * 3 + 1);
		loop ( i, m_data )
		{
			BYTE b = m_data[i];
			*P++ = STRLIB::IntToHexChar(b >> 4);
			*P++ = STRLIB::IntToHexChar(b & 0x0F);
			*P++ = ' ';
		}
		*P = '\0';
		m_dataString.ReleaseBuffer();
		if ( ::IsWindow(m_toolTip) )
		{
			m_toolTip.UpdateTipText(CStr::Fmt(_T("has %d byte(s)."), m_data.GetSize()), this);
		}
//		NotifyToParent(EN_CHANGE);
	}

	/**
	 * [�`��] �N���C�A���g�`��.
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 */
	void m_DrawClient(CDC& dc)
	{
		CPoint nowPos = m_viewPos;
		CRect rc;
		GetClientRect(rc);
		HGDIOBJ hOld = ::SelectObject(dc, m_font);
		dc.IntersectClipRect(rc);
		if ( m_caretDrawPos.x < 0 )
		{
			m_caretDrawPos = m_IndexToClientPoint(dc, m_caretIndex);
		}
		dc.SetBkMode(TRANSPARENT);
		if ( IsMultiLineMode() )
		{
			m_DrawMultiLineMode(dc, rc);
		}
		else
		{
			m_DrawSingleLineMode(dc, rc);
		}
		::SelectObject(dc, hOld);
		//
		if ( m_HasFocus() )
		{
			CPoint p = m_caretDrawPos - m_viewPos;
			if ( rc.PtInRect(p) && rc.PtInRect(p + m_charSize) )
			{
				p.x--;
			}
			else
			{
				p.x = 60000;
			}
			_super::SetCaretPos(p + m_margin);
			if ( ! m_hasCaret )
			{
				m_hasCaret = true;
				if ( m_isInsertMode )
				{
					_super::CreateSolidCaret(2, m_charSize.cy);
				}
				else
				{
					_super::CreateSolidCaret(m_charSize.cx, m_charSize.cy);
				}
				_super::ShowCaret();
				_super::SetCaretPos(p + m_margin);
			}
		}
		if ( nowPos.x != m_viewPos.x )
		{
			NotifyToParent(EN_HSCROLL);
		}
		if ( nowPos.y != m_viewPos.y )
		{
			NotifyToParent(EN_VSCROLL);
		}
	}

	/**
	 * [�`��] �}���`���C���N���C�A���g�`��.
	 *	@param targetDC �f�o�C�X�R���e�L�X�g
	 *	@param clientRect �N���C�A���g�͈�
	 */
	void m_DrawMultiLineMode(CDC& targetDC, const CRect& clientRect)
	{
		CRect rc = clientRect;
		rc.left += m_margin.x;
		rc.top += m_margin.y;
		//== �\���ʒu�␳
		if ( m_isCaletVisidle )
		{
			m_isCaletVisidle = false;
			CPoint p = m_IndexToClientPoint(targetDC, m_caretIndex);
			if ( rc.Width() - m_charSize.cx < p.x - m_viewPos.x )
			{
				m_viewPos.x = p.x - (rc.Width() - m_charSize.cx);
			}
			if ( p.x < m_viewPos.x )
			{
				m_viewPos.x = p.x;
			}
			int hh = rc.Height() / m_charSize.cy * m_charSize.cy;
			if ( hh - m_charSize.cy < p.y - m_viewPos.y )
			{
				m_viewPos.y = p.y - (hh - m_charSize.cy);
			}
			if ( p.y < m_viewPos.y )
			{
				m_viewPos.y = p.y;
			}
		}
		CBitmapImage bi;
		bi.Set(clientRect.Width(), clientRect.Height(), GetBkColor());
		//== ����ʂŉ摜�쐬
		{
			CBitmapDC dc(&bi);
			CDcSelectAssistant dca(dc);
			dc.SetWindowOrg(m_viewPos);
			dca.SelectFont(m_font);
			dca.SetTextColor(GetTextColor());
			dca.SetBkColor(GetBkColor());
			//== �����\��
			rc.right += m_viewPos.x;
			rc.bottom = rc.top + m_charSize.cy;
			int d = WIDTH * 3;
			loop ( i, m_dataString.GetLength() / d + 1 )
			{
				if ( dc.RectVisible(rc) )
				{
					CString ss = m_dataString.Mid(i * d, d);
					dc.DrawText(ss, rc, DT_LEFT);
				}
				rc.top += m_charSize.cy;
				rc.bottom += m_charSize.cy;
			}
			//== �I��\��
			bool hasFocus = m_HasFocus();
			if ( m_selectCaretIndex >= 0 && m_selectCaretIndex != m_caretIndex )
			{
				POINT p1 = m_selectCaretDrawPos;
				POINT p2 = m_caretDrawPos;
				if ( p1.y == p2.y )
				{
					if ( p1.x > p2.x )
					{
						Swap(p1.x, p2.x);
					}
					CRect rc(p1.x, p1.y, p2.x - m_charSize.cx, p1.y + m_charSize.cy);
					m_DrawSelectMark(dc, rc + m_margin, hasFocus);
				}
				else
				{
					if ( p1.y > p2.y )
					{
						Swap(p1, p2);
					}
					loop ( i, (p2.y - p1.y) / m_charSize.cy - 1 )
					{
						CPoint ps(0, p1.y + (i + 1) * m_charSize.cy);
						CSize sz(m_charSize.cx * (WIDTH * 3 - 1), m_charSize.cy);
						m_DrawSelectMark(dc, CRect(ps, sz) + m_margin, hasFocus);
					}
					CRect rc1(p1.x, p1.y, m_charSize.cx * (WIDTH * 3 - 1), p1.y + m_charSize.cy);
					m_DrawSelectMark(dc, rc1 + m_margin, hasFocus);
					CRect rc2(0, p2.y, p2.x - m_charSize.cx, p2.y + m_charSize.cy);
					m_DrawSelectMark(dc, rc2 + m_margin, hasFocus);
				}
			}
			else if ( m_isShowCaretAlways && ! hasFocus )
			{
				CRect rc(m_caretDrawPos, m_charSize);
				if ( m_isInsertMode )
				{
					rc.right = rc.left + 1;
				}
				rc.left--;
				rc += m_margin;
				m_DrawSelectMark(dc, rc, false);
			}
		}
		bi.Draw(targetDC, 0, 0);
	}

	/**
	 * [�`��] �V���O�����C���N���C�A���g�`��.
	 *	@param dca �f�o�C�X�R���e�L�X�g
	 *	@param clientRect �N���C�A���g�͈�
	 */
	void m_DrawSingleLineMode(CDC& dc, const CRect& clientRect)
	{
		CRect rc = clientRect;
		rc.left += m_margin.x;
		rc.top += m_margin.y;
		//== �\���ʒu�␳
		if ( m_isCaletVisidle )
		{
			m_isCaletVisidle = false;
			CPoint p = m_IndexToClientPoint(dc, m_caretIndex);
			if ( rc.Width() - m_charSize.cx < p.x - m_viewPos.x )
			{
				m_viewPos.x = (p.x - (rc.Width() - m_charSize.cx));
			}
			if ( p.x < m_viewPos.x )
			{
				m_viewPos.x = p.x;
			}
			m_viewPos.y = 0;
		}
		dc.SetWindowOrg(m_viewPos);
		//== �����\��
		rc.right += m_viewPos.x;
		dc.SetTextColor(GetTextColor());
		dc.SetBkColor(GetBkColor());
		dc.DrawText(m_dataString, rc, DT_LEFT);
		//== �I��\��
		if ( m_selectCaretIndex >= 0 && m_selectCaretIndex != m_caretIndex )
		{
			int x1 = m_selectCaretDrawPos.x;
			int x2 = m_caretDrawPos.x;
			if ( x1 > x2 )
			{
				Swap(x1, x2);
			}
			CRect rc(x1, 0, x2 - m_charSize.cx + 2, m_charSize.cy);
			m_DrawSelectMark(dc, rc + m_margin);
		}
		else if ( m_isShowCaretAlways && ! m_HasFocus() )
		{
			CRect rc(m_caretDrawPos, m_charSize);
			if ( m_isInsertMode )
			{
				rc.right = rc.left + 1;
			}
			rc.left--;
			rc += m_margin;
			::DrawFocusRect(dc, &rc);
		}
	}
	
	/**
	 * [�`��] �I���}�[�N�`��
	 *	@param dc �`��Ώ�.
	 *	@param rect �͈�
	 */
	void m_DrawSelectMark(CDC& dc, const CRect& rect, bool isActive = true)
	{
		COLORREF color = isActive ? m_activeHighlightColor : m_inactiveHighlightColor;
		CSelectedBeltDrawer d(rect.Size(), color, GetBkColor(), isActive);
		d.Draw(dc, rect.left, rect.top);
	}

	void m_SetSelect(int index, const POINT& drawPos)
	{
		m_selectCaretIndex = index;
		m_selectCaretDrawPos = drawPos;
		if ( (index & 1) != 0 )
		{
			// ��Ȃ�
			m_selectCaretIndex--;
			m_selectCaretDrawPos.x -= m_charSize.cx;
		}
	}

	void m_ReleaseSelect(void)
	{
		if ( m_selectCaretIndex >= 0 )
		{
			m_selectCaretIndex = -1;
		}
	}

	void m_HideCaret(void)
	{
		m_hasCaret = false;
		_super::HideCaret();
//		::DestroyCaret();
	}

	bool m_HasFocus(void) const
	{
		return (::GetFocus() == _super::GetSafeHwnd());
	}

	// �\���֌W
	CFontHandle		m_font;					///< �t�H���g
	CSize			m_charSize;				///< �����T�C�Y�B(���� Insert�p�J���b�g�̕��ɂ��g�p)
	CPoint			m_margin;				///< ����̕\���ʒu�}�[�W��
	CToolTipCtrl	m_toolTip;				///< �c�[���`�b�v
	COLORREF		m_textColor;			///< �����̐F
	COLORREF		m_backColor;			///< �w�i�̐F
	COLORREF		m_activeHighlightColor;	///< �A�N�e�B�u�n�C���C�g�F
	COLORREF		m_inactiveHighlightColor;///<��A�N�e�B�u�n�C���C�g�F
	// ��Ԋ֌W
	bool			m_isInsertMode;			///< �C���T�[�g���[�h
	int				m_maxLength;			///< ����BYTE�ő咷�B-1 �Ȃ疳���B
	bool			m_isEnableNotify;		///< �ʒm����
	// �f�[�^�֌W
	CByteVector		m_data;					///< �����Ă���f�[�^
	CString			m_dataString;			///< �f�[�^�𕶎��񉻂�������
	BYTE			m_defaultData;
	// �J���b�g�֌W
	int				m_caretIndex;			///< �J���b�g�ʒu�B(BYTE�ɕt�� INDEX 2��)
	CPoint			m_caretDrawPos;			///< �J���b�g�\�����W�B(�N���C�A���g���W�Bx �� -1 �Ȃ疢�v�Z��\��)
	int				m_selectCaretIndex;		///< �I���J�n�J���b�g�ʒu�B(-1 �Ȃ疢�I����Ԃ�\��)
	CPoint			m_selectCaretDrawPos;	///< �I���J�n�J���b�g�\�����W�B(�N���C�A���g���W)
	bool			m_isShowCaretAlways;	///< �J���b�g�ʒu����ɕ\��
	bool			m_hasCaret;				///< �J���b�g�������H
	// �X�N���[���֌W
	bool			m_isCaletVisidle;		///< �J���b�g��������悤�ɂ���B
	CPoint			m_viewPos;				///< �N���C�A���g�̕\���ʒu
};



/**@ingroup MFCCONTROL
 * �V���O�����C��HEX�f�[�^����Edit�R���g���[��
 *
 *		1byte ���ɃX�y�[�X���󂭂̂ŁA�X�̃f�[�^�����₷���ҏW�ł��܂��B
 *
 *	@note �x�[�X�� EDIT�R���g���[��( CEdit )�ł͂Ȃ��̂ŁA�����Ӊ������B
 *	
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���i�G�f�B�b�g�R���g���[���j�𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic or CEdit�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditHexData.h
 * 
 *	@date 10/04/01	�V�K�쐬
 */
class CEditHexData : public CEditHexDataT<>
{
};



/**@ingroup MFCCONTROL
 * �}���`���C��HEX�f�[�^����Edit�R���g���[��
 *
 *		1byte ���ɃX�y�[�X���󂭂̂ŁA�X�̃f�[�^�����₷���ҏW�ł��܂��B
 *		16byte���ɉ��s����܂��B
 *
 *	@note �x�[�X�� EDIT�R���g���[��( CEdit )�ł͂Ȃ��̂ŁA�����Ӊ������B
 *
 *	@attention ���̃R���g���[���͓����t�H���g���g�p���Ă��������B
 *	
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���i�G�f�B�b�g�R���g���[���j�𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic or CEdit�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditHexData.h
 * 
 *	@date 10/04/01	�V�K�쐬
 */
class CEditHexData16 : public CEditHexDataT<16>
{
};



}; //MFC
}; //TNB
