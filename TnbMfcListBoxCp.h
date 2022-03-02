#pragma once
/**
 *@file
 * �R�s�[�@�\�tListBox�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbClipboard.h"
#include "TnbStrAdder.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ������R�s�[�@�\�tListBox�R���g���[��
 *
 *		Ctrl+A , Ctrl+C ���g����ListBox�R���g���[���ɂȂ�܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�{�b�N�X�̃v���p�e�B�́A�u�I��(C)�v���u�g���v�ɂ��Ă����Ă��������B
 * 
 *	@note �X�N���[������ƁA�e�� WM_VSCROLL �� SendMessage ���܂��BLPARAM �ɂ� �{�R���g���[���� HWND ��n���܂��B
 *		���̂��߁A MFC �� CWnd::OnVScroll() �̑�O������ CScrollBar* �ɂ͖{�N���X�̃C���X�^���X���n��܂��B
 *		CScrollBar* �Ƃ��đ��삵�Ȃ��悤�ɒ��ӂ��Ă��������B�܂��A WPARAM �ɂ́A���݂� TopIndex ���͂����Ă��܂����A
 *		65535 �s�𒴂���ƁA�������Ȓl�ɂȂ�܂��̂ŁA GetTopIndex() �Ō��݂̕\���ʒu���m�F����悤�ɂ��Ă��������B
 *		
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcListBoxCp.h
 * 
 *	@date 06/01/01	�V�K�쐬
 *	@date 09/10/05	PostString() �ǉ�
 *	@date 10/02/15	�ő僉�C�����ݒ�\��.
 *	@date 10/05/20	Ctrl+A �� LBN_SELCHANGE ��ʒm����悤�ɂ����B
 *	@date 11/03/02	�ő僉�C�����̃f�t�H���g��ύX�B 
 *	@date 11/07/20	�X�N���[��������e�� WM_VSCROLL �� SendMessage ����悤�ɂ����B LPARAM �ɂ� �{�R���g���[���� HWND�B
 *	@date 14/01/10	GetText() ��ǉ��BLBS_HASSTRING �X�^�C���������Ȃ��T�u�N���X�΍�B
 */
class CListBoxCp : public CListBox
{
	DEFSUPER(CListBox);
public:

	/// �R���X�g���N�^
	CListBoxCp(void) : m_maxLine(20000)
	{
	}

	/**
	 * [�ݒ�] �ő僉�C�����ݒ�.
	 *	@note �f�t�H���g�� 20000 �s�ł��B
	 *	@param maxLine �ő僉�C���B -1 �Ȃ�A�ő僉�C���̃`�F�b�N�����܂���(�������܂���)�B
	 */
	void SetMaxLine(int maxLine)
	{
		m_maxLine = maxLine;
	}

	/**
	 * [�擾] �ő僉�C�����擾.
	 *	@retval 0���� �ő僉�C���A���w��.
	 *	@retval 0�ȏ� 
	 */
	int GetMaxLine(void) const
	{
		return m_maxLine;
	}

	/**
	 * [�I��] �S���ڑI��.
	 */
	void AllSetSel(void)
	{
		int iItemCount = _super::GetCount();
		if ( iItemCount > 0 )
		{
			_super::SelItemRange(TRUE, 0, iItemCount - 1);
		}
	}

	/**
	 * [����] ������擾.
	 *		LIST ���̕���������s�ŘA�����Ԃ��܂��B
	 *	@param boIsSelectOnly true �Ȃ�I������Ă��鍀�ڂ݂̂����W���܂��B
	 *	@return ������B
	 */
	CString ToString(bool boIsSelectOnly = true)
	{
		CStrAdder strText;
		int iItemCount = _super::GetCount();
		if ( 0 < iItemCount )
		{
			CString s;
			for ( int i = 0; i < iItemCount; i++ )
			{
				if ( ! boIsSelectOnly || _super::GetSel(i) > 0 )
				{
					//�I������Ă���
					GetText(i, s);
					strText += s;
					strText += _T("\r\n");
				}
			}
		}
		return CString(strText);
	}

	/**
	 * [����] �I�����ڃR�s�[.
	 *		LIST ���̕���������s�ŘA�����A�N���b�v�{�[�h�փR�s�[���܂��B
	 *	@param boIsSelectOnly true �Ȃ�I������Ă��鍀�ڂ݂̂����W���܂��B
	 */
	void Copy(bool boIsSelectOnly = true)
	{
		CString strText = ToString(boIsSelectOnly);
		if ( ! strText.IsEmpty() )
		{
			CClipboard clip(_super::m_hWnd);
			clip.SetString(strText);
		}
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@note �ʃX���b�h����ł��ǉ����\�ł��B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 */
	void PostString(int nIndex, LPCTSTR lpszItem)
	{
		if ( ! ::IsWindow(m_hWnd) ) { return; }
		CString* P = new CString(lpszItem);
		if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
		{
			delete P;
		}
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@note �ʃX���b�h����̎��s�́A�\�z�O�̓�������邱�Ƃ�����܂��B PostString() ���g�p���Ă��������B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�}�����ꂽ�C���f�b�N�X��Ԃ��܂��B
	 */
	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		m_CheckMaxLine();
		return _super::InsertString(nIndex, lpszItem);
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@note �ʃX���b�h����̎��s�́A�\�z�O�̓�������邱�Ƃ�����܂��B PostString() ���g�p���Ă��������B
	 *	@param lpszItem �}�����镶����B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�}�����ꂽ�C���f�b�N�X��Ԃ��܂��B
	 */
	int AddString(LPCTSTR lpszItem)
	{
		m_CheckMaxLine();
		return _super::AddString(lpszItem);
	}

	/**
	 * [�폜] ������폜.
	 *	@note �폜��A�\���ʒu�� top �ɖ߂��܂��B
	 *	@param nIndex �폜���镶����� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *	@retval LB_ERR �G���[�B
	 *	@retval ��L�ȊO �����B���l�́A�c��̃A�C�e����
	 */
	int DeleteString(UINT nIndex)
	{
		return _super::DeleteString(nIndex);
	}

	/**
	 * [�폜] ������폜.
	 *	@note �\���ʒu��ς����ɁA��������폜���܂��B
	 *	@param nIndex �폜���镶����� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *	@retval LB_ERR �G���[�B
	 *	@retval ��L�ȊO �����B���l�́A�c��̃A�C�e����
	 */
	int DeleteStringEx(UINT nIndex)
	{
		_super::SetRedraw(FALSE);
		int sel = _super::GetTopIndex();
		int r = _super::DeleteString(nIndex);
		_super::SetTopIndex(sel);
		_super::SetRedraw(TRUE);
		_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE);
		return r;
	}

	/**
	 * [�擾] ������擾.
	 *	@param[in] nIndex �擾���镶����� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *	@param[out] lpszBuffer ��������i�[����A�h���X���w�肵�܂��B 
	 *	@retval LB_ERR �G���[�B
	 *	@retval ��L�ȊO �����B���l�́A�擾�����������B
	 */
	virtual int GetText(int nIndex, LPTSTR lpszBuffer) const
	{
		return _super::GetText(nIndex, lpszBuffer);
	}

	/**
	 * [�擾] ������擾.
	 *	@param[in] nIndex �擾���镶����� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *	@param[out] rString ��������i�[����܂��B
	 */
	virtual void GetText(int nIndex, CString& rString) const
	{
		_super::GetText(nIndex, rString);
	}
	
protected:
	enum { WM_CLB_POSTSTRING = WM_APP }; ///< PostString �p���b�Z�[�W

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		#ifdef _DEBUG
			DWORD dwStyle = _super::GetStyle();
			ASSERT(dwStyle & LBS_EXTENDEDSEL);
		#endif
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
		switch ( message )
		{
		case WM_CHAR:
			if ( m_CheckCtrlKey(wParam, lParam) )
			{
				return TRUE;
			}
			break;
		case WM_CLB_POSTSTRING:
			{
				CString* P = reinterpret_cast<CString*>(lParam);
				if ( ::IsWindow(m_hWnd) )
				{
					int iIndex = ToInt(wParam);
					if ( m_maxLine >= 0 && _super::GetCount() >= m_maxLine )
					{
						_super::SetRedraw(FALSE);
						_super::DeleteString(0);
						LRESULT r = _super::InsertString(iIndex, *P);
						_super::SetTopIndex(ToInt(r));
						_super::SetRedraw(TRUE);
						_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE);
					}
					else
					{
						LRESULT r = _super::InsertString(iIndex, *P);
						_super::SetTopIndex(ToInt(r));
					}
				}
				delete P;
				return 0;
			}
			break;
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
			{
				_super::SetRedraw(FALSE);
				int top1 = _super::GetTopIndex();
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				int top2 = _super::GetTopIndex();
				_super::SetTopIndex(top1);
				_super::SetRedraw(TRUE);
				if ( top1 == top2 )
				{
					_super::RedrawWindow(NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE | RDW_ERASE);
				}
				else
				{
					_super::SetTopIndex(top2);
				}
				CWnd* pWnd = GetParent();
				if ( pWnd != NULL )
				{
					if ( message == WM_VSCROLL )
					{
						pWnd->SendMessage(WM_VSCROLL, wParam, reinterpret_cast<LPARAM>(GetSafeHwnd()));
					}
					else
					{
						WPARAM w = MAKELONG(SB_THUMBPOSITION, GetTopIndex());
						pWnd->SendMessage(WM_VSCROLL, w, reinterpret_cast<LPARAM>(GetSafeHwnd()));
					}
				}
				return r;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/**
	 * [�m�F] ���̓L�[�̃`�F�b�N.
	 *		Ctrl+A,Ctrl+C �̃`�F�b�N�����܂��B
	 *	@param wParam WM_CHAR ��WPARAM�B
	 *	@param lParam WM_CHAR ��LPARAM�B
	 *	@retval true �����ρB
	 *	@retval false �������B
	 */
	bool m_CheckCtrlKey(WPARAM wParam, LPARAM lParam)
	{
		LPARAM lp = lParam & 0x00FFFFFF;
		if ( lp == 0x002e0001 && wParam == 0x03 )
		{
			//Ctrl + C
			CWaitCursor a;
			Copy();
			return true;
		}
		if ( lp == 0x001e0001 && wParam == 0x01 )
		{
			//Ctrl + A
			CWaitCursor a;
			AllSetSel();
			MFCLIB::SendCommandMessage(this, LBN_SELCHANGE);
			return true;
		}
		return false;
	}
	/// �ǉ��ł��邩�H���E������Ȃ�Delete
	void m_CheckMaxLine(void)
	{
		if ( m_maxLine >= 0 && _super::GetCount() >= m_maxLine )
		{
			_super::DeleteString(0);
		}
	}
	int					m_maxLine;				///< �ő僉�C��
};



}; // MFC
}; // TNB
