#pragma once
/**
 *@file
 * �J���[�\���@�\�tListBox�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcListBoxCp.h"
#include "TnbPointerHandle.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �J���[ListBox�R���g���[��
 *
 *		�s�P�ʂŐF���w��o���� ListBox�R���g���[���ł��B�B
 *
 *		Ctrl+A , Ctrl+C ���g���� ListBox�R���g���[���ł��B
 *
 *	@note �I���s�́A�����F�Ɣw�i�F�����]���\������܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�{�b�N�X�̃v���p�e�B�́A�u�I��(C)�v���u�g���v�ɁA�u�I�[�i�[�`��(O)�v���u�ρv��
 *		�����񂠂�̃`�F�b�N�� OFF �ɂ��Ă����Ă��������B
 *
 *	@note �X�N���[������ƁA�e�� WM_VSCROLL �� SendMessage ���܂��BLPARAM �ɂ� �{�R���g���[���� HWND ��n���܂��B
 *		���̂��߁A MFC �� CWnd::OnVScroll() �̑�O������ CScrollBar* �ɂ͖{�N���X�̃C���X�^���X���n��܂��B
 *		CScrollBar* �Ƃ��đ��삵�Ȃ��悤�ɒ��ӂ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcColorListBox.h
 * 
 *	@date 06/11/08	�V�K�쐬
 *	@date 07/10/02	�w�i�̎w����\�ɂ����B
 *	@date 08/11/19	SetPostViewMode() , PostString() ��V�K�ǉ��B
 *	@date 09/04/30	PostString���A���`��&�X�N���[���ł͂Ȃ��A�����x���o����悤�ɂ����B
 *	@date 09/11/12	�\�������B
 *	@date 11/03/02	�}�����J�E���^��p�~�B
 *	@date 11/09/13	SetBackColor() �ǉ��B
 */
class CColorListBox : public CListBoxCp
{
	DEFSUPER(CListBoxCp);
public:
	
	/// �R���X�g���N�^
	CColorListBox(void) : _super(), m_itemHeight(-1), m_itemWidth(-1), m_margin(0), m_isView(true)
		, m_currentTextColor(::GetSysColor(COLOR_WINDOWTEXT)), m_currentBackColor(::GetSysColor(COLOR_WINDOW))
	{
	}

	/**
	 * [�ݒ�] �ő僉�C�����ݒ�.
	 *	@note �f�t�H���g�� 20000 �s�ł��B
	 *	@param maxLine �ő僉�C���B
	 */
	void SetMaxLine(int maxLine)
	{
		m_lineDatas.SetQueueSize(maxLine + 1);
		_super::SetMaxLine(maxLine);
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *		������̂Ȃ��Ƃ���̐F���w�肵�܂��B
	 *	@param color �w�i�F�w��(COLORREF)�B�ȗ�����ƁA�f�t�H���g�i�V�X�e���j�̐F�ŕ`�悳��܂��B
	 */
	void SetBackColor(COLORREF color = CLR_AUTOSELECT)
	{
		if ( m_backBrush.GetSafeHandle() != NULL )
		{
			m_backBrush.DeleteObject();
		}
		if ( IS_RGBVALUE(color) )
		{
			m_backBrush.CreateSolidBrush(color);
			ASSERT( m_backBrush.GetSafeHandle() != NULL );
		}
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *		�ǉ����镶����̐F���w�肵�܂��B
	 *	@param textColor �����F�w��(COLORREF)�B
	 *	@param backColor �w�i�F�w��(COLORREF)�B
	 */
	void SetCurrentColor(COLORREF textColor, COLORREF backColor)
	{
		m_currentTextColor = textColor;
		m_currentBackColor = backColor;
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *		�w��s�̐F���w�肵�܂��B
	 *	@param nIndex �C���f�b�N�X
	 *	@param textColor �����F�w��(COLORREF)
	 *	@param backColor �w�i�F�w��(COLORREF)
	 */
	void SetColor(int nIndex, COLORREF textColor, COLORREF backColor)
	{
		if ( m_lineDatas.IsInRange(nIndex) )
		{
			TParam& p = m_lineDatas[nIndex];
			p.textColor = textColor;
			p.backColor = backColor;
			RedrawWindow();
		}
	}

	/**
	 * [�ݒ�] �}�[�W���ݒ�
	 *	@param m �㉺�̃}�[�W��
	 */
	void SetMargin(DWORD m)
	{
		m_margin = m;
		m_itemHeight = -1;
	}

	/**
	 * [�ݒ�] PostString�\�����[�h�ݒ�.
	 *	@note PostString() ���s���A�ǉ����������񂪕\������悤�ɃX�N���[�������邩�ۂ��A�ݒ�ł��܂��B
	 *	@param isView true �Ȃ�ǉ������񂪕\�������悤�ɃX�N���[�����܂��B false �Ȃ�X�N���[�����܂���B
	 */
	void SetPostViewMode(bool isView)
	{
		m_isView = isView;
	}
	
	/**
	 * [�ǉ�] ������ǉ�.
	 *		�w�肵���F�ŕ������ǉ����܂��B
	 *	@note �ʃX���b�h����ł��ǉ����\�ł��B
	 *	@note �F���w�肷��ƁA SetCurrentColor() �Ŏw�肵���̂Ɠ������ʂ��c��܂��B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 *	@param textColor �����F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�����F�ɂȂ�܂��B
	 *	@param backColor �w�i�F�w��(COLORREF)�B�ȗ������ꍇ�A�Ō�ɐݒ肳�ꂽ�w�i�F�ɂȂ�܂��B
	 */
	void PostString(int nIndex, LPCTSTR lpszItem, COLORREF textColor = 0, COLORREF backColor = 0)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			TParam* P = new TParam(lpszItem, textColor, backColor);
			if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
			{
				delete P;
			}
		}
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		�������ǉ����܂��B�F�� SetCurrentColor() �� PostString() �Őݒ肵�����̂ɂȂ�܂��B
	 *	@note �ʃX���b�h����̎��s�́A�\�z�O�̓�������邱�Ƃ�����܂��B PostString() ���g�p���Ă��������B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�}�����ꂽ�C���f�b�N�X��Ԃ��܂��B
	 */
	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		return _super::InsertString(nIndex, lpszItem);
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		��������Ō�ɒǉ����܂��B�F�� SetCurrentColor() �� PostString() �Őݒ肵�����̂ɂȂ�܂��B
	 *	@note �ʃX���b�h����̎��s�́A�\�z�O�̓�������邱�Ƃ�����܂��B PostString() ���g�p���Ă��������B
	 *	@param lpszItem �}�����镶����B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�}�����ꂽ�C���f�b�N�X��Ԃ��܂��B
	 */
	int AddString(LPCTSTR lpszItem)
	{
		return _super::AddString(lpszItem);
	}

	/**
	 * [�ʒm] �I�[�i�[�h���[�ʒm.
	 *	@param lpDrawItemStruct �h���[�v�����
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		if ( lpDrawItemStruct->CtlType != ODT_LISTBOX || lpDrawItemStruct->hwndItem != _super::GetSafeHwnd() )
		{
			return;
		}
		UINT nIndex = lpDrawItemStruct->itemID;
		if ( ! m_lineDatas.IsInRange(nIndex) )
		{
			return;
		}
		const TParam& p = m_lineDatas[nIndex];
		CString strText = p.viewItem;
		COLORREF textColor = p.textColor;
		COLORREF backColor = p.backColor;
		if ( (lpDrawItemStruct->itemState & ODS_SELECTED) != 0 )
		{
			Swap(textColor, backColor);
		}
		//
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		//
		COLORREF currenttextColor = dc.SetTextColor(textColor);
		int currentBkMode = dc.SetBkMode(TRANSPARENT);
		//
		dc.FillSolidRect(&(lpDrawItemStruct->rcItem), backColor);
		RECT rect = lpDrawItemStruct->rcItem;
		rect.top += m_margin;
		dc.DrawText(strText, &rect, DT_SINGLELINE | DT_NOPREFIX);
		if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
		{
			dc.DrawFocusRect(&(lpDrawItemStruct->rcItem));	//�j��
		}
		dc.SetBkMode(currentBkMode);
		dc.SetTextColor(currenttextColor);
		dc.Detach();
	}

	/**
	 * [�ʒm] �I�[�i�[�h���[�v�Z�ʒm.
	 *	@param lpMeasureItemStruct �h���[�v�����
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		if ( lpMeasureItemStruct->CtlType != ODT_LISTBOX || ToInt(lpMeasureItemStruct->CtlID) != _super::GetDlgCtrlID() )
		{
			return;
		}
		if ( m_itemHeight < 0 )
		{
			LOGFONT lf;
			GetFont()->GetLogFont(&lf);
			m_itemHeight = lf.lfHeight;
			if ( m_itemHeight < 0 )
			{
				m_itemHeight = -m_itemHeight;
			}
			m_itemHeight += m_margin * 2;
		}
		lpMeasureItemStruct->itemHeight = m_itemHeight;
	}

protected:

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *		ListBox �֌W�̃��b�Z�[�W���t�b�N���Ă��܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		int index = ToInt(wParam);
		LPTSTR lpsz = reinterpret_cast<LPTSTR>(lParam);
		//
		switch ( message )
		{
		case WM_PAINT:
			if ( GetCount() == 0 )
			{
				CPaintDC dc(this);
				return 0;
			}
			break;
		case WM_ERASEBKGND: //�w�i
			{
				CDC dc;
				dc.Attach(reinterpret_cast<HDC>(wParam));
				CRect rect;
				GetClientRect(rect);
				int y = (GetCount() - GetTopIndex()) * m_itemHeight;
				int h = rect.Height();
				if( h > y )
				{
					if ( y < 0 )
					{
						y = 0;
					}
					CBrush* B = &m_backBrush;
					if ( B->GetSafeHandle() == NULL )
					{
						B = dc.GetCurrentBrush();
					}
					dc.FillRect(CRect(0, y, rect.Width(), h), B);
				}
				dc.Detach();
			}
			return 0;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			RedrawWindow();
			break;
		case LB_GETTEXTLEN: //�����񒷎擾
			// @param index INDEX 
			// @return ����
			if ( m_lineDatas.IsInRange(index) )
			{
				return m_lineDatas[index].item.GetLength();
			}
			return LB_ERR;
		case LB_GETTEXT: //������擾
			// @param[in] index INDEX 
			// @param[out] lParam �i�[��
			if ( m_lineDatas.IsInRange(index) )
			{
				CStr s = m_lineDatas[index].item;
				STRLIB::Copy(lpsz, s);
				return s.GetLength();
			}
			return LB_ERR;
		case LB_DELETESTRING: //�폜
			// @param index INDEX 
			if ( index == 0 )
			{
				m_lineDatas.TakeElements(1);
			}
			else if ( m_lineDatas.IsInRange(index) )
			{
				m_lineDatas.Remove(index);
			}
			break;
		case LB_ADDSTRING: //�ǉ�
			// @param lParam ������A�h���X 
			// @note ���ۃR���g���[���ɐݒ肷��̂� "" 
			{
				ASSERTLIB( ToInt(m_lineDatas.GetSize()) == _super::GetCount() );
				TParam p(lpsz, m_currentTextColor, m_currentBackColor);
				m_lineDatas.Add(p);
				m_CheckWidth(p.item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_INSERTSTRING: //�}��
			// @param index INDEX 
			// @param lParam ������A�h���X 
			// @note ���ۃR���g���[���ɐݒ肷��̂� "" 
			if ( index < 0 )
			{
				TParam p(lpsz, m_currentTextColor, m_currentBackColor);
				m_lineDatas.Add(p);
				m_CheckWidth(p.item);
			}
			else if ( IsInRange(index, m_lineDatas.GetSize() + 1) )
			{
				TParam p(lpsz, m_currentTextColor, m_currentBackColor);
				m_lineDatas.Insert(index, p);
				m_CheckWidth(p.item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_RESETCONTENT: //�S����
			m_lineDatas.RemoveAll();
			m_itemWidth = -1;
			_super::SetHorizontalExtent(m_itemWidth);
			break;
		case WM_CLB_POSTSTRING:
			{
				TParam* P = reinterpret_cast<TParam*>(lParam);
				if ( P->textColor != P->backColor )
				{
					m_currentTextColor = P->textColor;
					m_currentBackColor = P->backColor;
				}
				if ( ::IsWindow(m_hWnd) )
				{
					LRESULT r = _super::InsertString(index, P->item);
					if ( m_isView )
					{
						_super::SetTopIndex(ToInt(r));
					}
				}
				delete P;
				return 0;
			}
			break;
		case WM_DESTROY:
			{
				MSG msg;
				while ( ::PeekMessage(&msg, m_hWnd, WM_CLB_POSTSTRING, WM_CLB_POSTSTRING, PM_REMOVE) )
				{
					if ( msg.message == WM_CLB_POSTSTRING )
					{
						TParam* P = reinterpret_cast<TParam*>(msg.lParam);
						delete P;
					}
				}
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		#ifdef _DEBUG
			DWORD dwStyle = _super::GetStyle();
			ASSERT( dwStyle & LBS_EXTENDEDSEL );
			ASSERT( m_lineDatas.GetSize() == 0 );
			#ifndef _WIN32_WCE
				ASSERT( (dwStyle & LBS_OWNERDRAWVARIABLE) != 0 );
//				ASSERT( (dwStyle & LBS_HASSTRINGS) == 0 );
			#endif
		#endif
		_super::PreSubclassWindow();
		m_lineDatas.SetQueueSize(_super::GetMaxLine() + 1);
	}

	/**
	 * [�ʒm] �j���I����ʒm
	 */
	virtual void PostNcDestroy(void) 
	{
		m_lineDatas.RemoveAll();
		_super::PostNcDestroy();
	}

private:
	/// ��s���^
	struct TParam
	{
		CStr		item;		///< ������
		CStr		viewItem;	///<
		COLORREF	textColor;	///< �����F
		COLORREF	backColor;	///< �w�i�F
		TParam(void){}
		TParam(LPCTSTR lpsz, COLORREF ct, COLORREF cb) : item(lpsz), textColor(ct), backColor(cb) 
		{
			viewItem = item;
			const int TABSTOP = 4;
			while ( true )
			{
				INT_PTR tabPos = viewItem.Find('\t');
				if ( tabPos < 0)
				{
					break;
				}
				CStr s = viewItem.Mid(tabPos + 1);
				viewItem = viewItem.Left(tabPos) 
						+ CStr::Lineup(' ', TABSTOP - (tabPos % TABSTOP)) + s;
			}
		}
	};
	int						m_itemHeight;			///< Item�̍���
	int						m_itemWidth;			///< Item�̕�
	CBrush					m_backBrush;			///< �A�C�e���̂Ȃ��Ƃ���̃J���[
	COLORREF				m_currentTextColor;		///< ���ǉ�����J���[
	COLORREF				m_currentBackColor;		///< ���ǉ�����J���[
	CRingQueueT<TParam>		m_lineDatas;			///< �\�������̓��e
	DWORD					m_margin;				///< �\���J�n�ʒu
	bool					m_isView;				///< �ǉ����\�����邩�H
	//
	DWORD					m_lastPostIndex;		///< �ŏI�ǉ��ʒu

	/**
	 * �R���g���[���̕��̒���
	 */
	void m_CheckWidth(LPCTSTR lpszItem)
	{
		CDC* pDC = GetDC();
		CFont* pFont = _super::GetFont();
		CFont* pFontOld = pDC->SelectObject(pFont);
		const CSize& size = pDC->GetTextExtent(lpszItem);
		if ( m_itemWidth < size.cx )
		{
			m_itemWidth = size.cx;
			_super::SetHorizontalExtent(m_itemWidth);
		}
		pDC->SelectObject(pFontOld);
		ReleaseDC(pDC);
	}
};



}; // MFC
}; // TNB
