#pragma once
/**
 *@file
 * �y��ListBox�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcListBoxCp.h"
#include "TnbMfcDelayedRedraw.h"
#include "TnbPointerHandle.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * LightListBox�R���g���[��
 *
 *		�����̍s�������ɂ��Ă�A�y�� ListBox�R���g���[���ł��B
 *
 *		Ctrl+A , Ctrl+C ���g���� ListBox�R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�{�b�N�X�̃v���p�e�B�́A�u�I�[�i�[�`��(O)�v���u�Œ�v��
 *		�����񂠂�̃`�F�b�N�� OFF �ɂ��Ă����Ă��������B
 *		�X�� LBS_NODATA �̃X�^�C�������Ēu���Ă��������iCE �̏ꍇ�́A��������Ȃ��ł��������j�B
 *
 *	@attention LBS_NODATA �� VC6 �̏ꍇ�AIDE ����͂��̃X�^�C�������邱�Ƃ͏o���܂���B
 *				���� rc �t�@�C�����J���ҏW���Ă��������j�B
 *
 *	@note �X�N���[������ƁA�e�� WM_VSCROLL �� SendMessage ���܂��BLPARAM �ɂ� �{�R���g���[���� HWND ��n���܂��B
 *		���̂��߁A MFC �� CWnd::OnVScroll() �̑�O������ CScrollBar* �ɂ͖{�N���X�̃C���X�^���X���n��܂��B
 *		CScrollBar* �Ƃ��đ��삵�Ȃ��悤�ɒ��ӂ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcLightListBox.h
 * 
 *	@date 10/11/01	�V�K�쐬�B
 *	@date 10/11/30	CE �p�J�X�^�}�C�Y�B�w�i�`�揈���C���B
 *	@date 11/03/08	���������O�L���[�ɕύX�B
 *	@date 11/03/16	�L���[�̊Ǘ��𒲐��B
 *	@date 11/06/17	CE��Delete�������C���B
 *	@date 12/11/05	�f�t�H���g�̍s���� 10���s�ɒጸ�B
 *	@date 14/01/10	GetText() ��ǉ��B�uLBS_NODATA ������� LBS_HASSTRING �X�^�C���������Ȃ��̂� LB_GETTEXT ��4byte ���������Ȃ��v �΍�B
 */
class CLightListBox : public CDelayedRedrawAddinT<CListBoxCp>
{
	DEFSUPER(CDelayedRedrawAddinT<CListBoxCp>);
public:

	/// �f�[�^�Ǘ��^
	typedef CRingQueueT<CArrayPtrHandleT<TCHAR> > CDatas;
	
	/// �R���X�g���N�^
	CLightListBox(void) : _super(), m_itemHeight(-1), m_margin(0)
	{
		m_clientSize.cx = -1;
		SetMaxLine(10 * 10000);
	}

	/**
	 * [�ݒ�] �ő僉�C�����ݒ�.
	 *	@note �f�t�H���g�� 10���s�ł��B
	 *	@param maxLine �ő僉�C���B -1 �Ȃ�A�ő僉�C���̃`�F�b�N�����܂���(�������܂���)�B
	 */
	void SetMaxLine(int maxLine)
	{
		_super::SetMaxLine(maxLine);
		m_lineDatas.SetQueueSize(maxLine);
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
	 * [�ǉ�] ������ǉ�.
	 *		�w�肵���F�ŕ������ǉ����܂��B
	 *	@note �ʃX���b�h����ł��ǉ����\�ł��B
	 *	@param nIndex �_�~�[�ł��B��ɕ�����̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 */
	void PostString(int nIndex, LPCTSTR lpszItem)
	{
		LPTSTR P = CloneString(lpszItem);
		if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
		{
			delete P;
		}
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		�������ǉ����܂��B
	 *	@note �ʃX���b�h����̎��s�́A�\�z�O�̓�������邱�Ƃ�����܂��B PostString() ���g�p���Ă��������B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�}�����ꂽ�C���f�b�N�X��Ԃ��܂��B
	 */
	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		LPTSTR P = CloneString(lpszItem);
		return m_InsertString(nIndex, P);
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		��������Ō�ɒǉ����܂��B
	 *	@note �ʃX���b�h����̎��s�́A�\�z�O�̓�������邱�Ƃ�����܂��B PostString() ���g�p���Ă��������B
	 *	@param lpszItem �ǉ����镶����B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�}�����ꂽ�C���f�b�N�X��Ԃ��܂��B
	 */
	int AddString(LPCTSTR lpszItem)
	{
		return InsertString(-1, lpszItem);
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
		if ( m_lineDatas.IsInRange(nIndex) )
		{
			STRLIB::Copy(lpszBuffer, m_lineDatas[nIndex]);
			return STRLIB::GetLen(m_lineDatas[nIndex]);
		}
		return LB_ERR;
	}

	/**
	 * [�擾] ������擾.
	 *	@param[in] nIndex �擾���镶����� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *	@param[out] rString ��������i�[����܂��B
	 */
	virtual void GetText(int nIndex, CString& rString) const
	{
		if ( m_lineDatas.IsInRange(nIndex) )
		{
			rString = m_lineDatas[nIndex];
		}
	}

	/**
	 * [�ݒ�] �S������ݒ�.
	 *	@param datas �S�f�[�^
	 *	@param index �\�����C���B�ȗ�����ƍŏI�s��\�����܂��B
	 */
	void SetAllString(const CDatas& datas, int index = -1)
	{
		int l = ToInt(datas.GetQueueSize());
		if ( l != GetMaxLine() )
		{
			_super::SetMaxLine(l);
		}
		m_lineDatas = datas;
		m_Settle(ToInt((index < 0) ? (datas.GetSize() - 1) : index));
	}

	/**
	 * [�擾] �S������擾.
	 *	@return �S�f�[�^
	 */
	const CDatas& GetAllString(void) const
	{
		return m_lineDatas;
	}

	/**
	 * [�쐬] ������N���[���쐬.
	 *		�w��̕�����𕡐����܂��B
	 *	@param lpszText ������
	 *	@return ��������������̃|�C���^�B ����Ȃ��Ȃ����� delete[] ���邱�ƁB
	 */
	static LPTSTR CloneString(LPCTSTR lpszText)
	{
		size_t len = STRLIB::GetLen(lpszText) + 1;
		LPTSTR P = new TCHAR[len];
		MemCopy(P, lpszText, len);
		return P;
	}

protected:

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
		UINT index = lpDrawItemStruct->itemID;
		if ( ! m_lineDatas.IsInRange(index) )
		{
			return;
		}
		LPTSTR P = m_lineDatas[index];
		HDC dc = lpDrawItemStruct->hDC;
		COLORREF textColor = 0;
		HBRUSH backBrush = NULL;
		if ( (lpDrawItemStruct->itemState & ODS_SELECTED) == 0 )
		{
			textColor = ::GetSysColor(COLOR_WINDOWTEXT);
			backBrush = ::GetSysColorBrush(COLOR_WINDOW);
		}
		else
		{
			textColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			backBrush = ::GetSysColorBrush(COLOR_HIGHLIGHT);
		}
		COLORREF currentTextColor = ::SetTextColor(dc, textColor);
		int currentBkMode = ::SetBkMode(dc, TRANSPARENT);
		::FillRect(dc, &(lpDrawItemStruct->rcItem), backBrush);
		RECT rect = lpDrawItemStruct->rcItem;
		rect.top += m_margin;
		::DrawText(dc, P, -1, &rect, DT_SINGLELINE | DT_NOPREFIX);
		if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
		{
			::SetTextColor(dc, 0);
			::DrawFocusRect(dc, &(lpDrawItemStruct->rcItem));	//�j��
		}
		::SetBkMode(dc, currentBkMode);
		::SetTextColor(dc, currentTextColor);
	}

	/**
	 * [�ʒm] �I�[�i�[�h���[�v�Z�ʒm.
	 *	@param lpMeasureItemStruct �h���[�v�����
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
	}

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
		switch ( message )
		{
		case WM_SETFONT:
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_itemHeight = -1;
				m_Settle();
				return r;
			}
			break;
		case WM_ERASEBKGND: //�w�i
			{
				if ( m_clientSize.cx < 0 )
				{
					CRect rc;
					_super::GetClientRect(rc);
					m_clientSize.cy = rc.Height();
					m_clientSize.cx = rc.Width();
				}
				HDC dc = reinterpret_cast<HDC>(wParam);
				int y = (_super::GetCount() - _super::GetTopIndex()) * m_itemHeight;
				int h = m_clientSize.cy;
				if( h > y )
				{
					if ( y < 0 )
					{
						y = 0;
					}
					HBRUSH b = static_cast<HBRUSH>(::GetCurrentObject(dc, OBJ_BRUSH));
					::FillRect(dc, CRect(0, y, m_clientSize.cx, h), b);
				}
			}
			return 0;
		case WM_SIZE:
			m_clientSize.cy = HIWORD(lParam);
			m_clientSize.cx = LOWORD(lParam);
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			_super::RedrawWindow();
			break;
		case WM_CLB_POSTSTRING:
			{
				LPTSTR P = reinterpret_cast<LPTSTR>(lParam);
				m_InsertString(index, P);
				return 0;
			}
			break;
		case LB_GETTEXTLEN: //�����񒷎擾
			// @param index INDEX 
			// @return ����
			if ( m_lineDatas.IsInRange(index) )
			{
				return STRLIB::GetLen(m_lineDatas[index]);
			}
			return LB_ERR;
		case LB_GETTEXT: //������擾
			// @param[in] index INDEX 
			// @param[out] lParam �i�[��
			if ( m_lineDatas.IsInRange(index) )
			{
				STRLIB::Copy(reinterpret_cast<TCHAR*>(lParam), m_lineDatas[index]);
				return STRLIB::GetLen(m_lineDatas[index]);
			}
			return LB_ERR;
		case LB_DELETESTRING:
			if ( lParam != MAGIC_LPARAM )
			{
				if ( m_lineDatas.IsInRange(index) )
				{
					m_lineDatas.Remove(index);
					m_Settle();
				}
				return 0;
			}
			break;
		case LB_RESETCONTENT:
			if ( lParam != MAGIC_LPARAM )
			{
				if ( m_lineDatas.GetSize() > 0 )
				{
					m_lineDatas.RemoveAll();
					m_Settle();
				}
			}
			break;
		case WM_DESTROY:
			{
				UINT wm = WM_CLB_POSTSTRING;
				MSG msg;
				while ( ::PeekMessage(&msg, _super::GetSafeHwnd(), wm, wm, PM_REMOVE) )
				{
					if ( msg.message == wm )
					{
						LPTSTR P = reinterpret_cast<LPTSTR>(msg.lParam);
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
			ASSERT( (dwStyle & LBS_OWNERDRAWFIXED) != 0 );
			ASSERT( (dwStyle & LBS_HASSTRINGS) == 0 );
			ASSERT( (dwStyle & LBS_MULTICOLUMN) == 0 );
			ASSERT( m_lineDatas.GetSize() == 0 );
			#ifndef _WIN32_WCE
				ASSERT( (dwStyle & LBS_NODATA) != 0 );
			#endif
		#endif
		_super::PreSubclassWindow();
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
	enum { 
		MAGIC_LPARAM = 'Tllb'		// CE�p
	};
	// �m��
	void m_Settle(int index = -1)
	{
		int l = ToInt(m_lineDatas.GetSize());
		#ifndef _WIN32_WCE
			_super::SendMessage(LB_SETCOUNT, l ,0);
		#else
			if ( l == 0 )
			{
				_super::SendMessage(LB_RESETCONTENT, 0, MAGIC_LPARAM);
			}
			else
			{
				size_t ll = _super::GetCount();
				int d = ll - l;
				if ( d < 0 )
				{
					loop ( i, -d )
					{
						_super::SendMessage(LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("")));
					}
				}
				else if ( d > 0 )
				{
					loop ( i, d )
					{
						_super::SendMessage(LB_DELETESTRING, 0, MAGIC_LPARAM);
					}
				}
			}
			ASSERT( l == _super::GetCount() );
		#endif
		if ( index >= 0 )
		{
			_super::SetTopIndex(index);
		}
		if ( m_itemHeight < 0 )
		{
			LOGFONT lf;
			_super::GetFont()->GetLogFont(&lf);
			m_itemHeight = lf.lfHeight;
			if ( m_itemHeight < 0 )
			{
				m_itemHeight = -m_itemHeight;
			}
			m_itemHeight += m_margin * 2;
			_super::SetItemHeight(0, m_itemHeight);
		}
		_super::DelayedRedraw();
	}
	/// �}��
	/// lpszItem ; new �Ŋm�ۂ��������� ms_Close() �ŕ�������������B
	int m_InsertString(int index, LPTSTR lpszItem)
	{
		CArrayPtrHandleT<TCHAR> p = lpszItem;
		INDEX r = m_lineDatas.Add(p);
		if ( r == INVALID_INDEX )
		{
			int ml = _super::GetMaxLine();
			int ll = (ml > 1000) ? 20 : 5;
			m_lineDatas.TakeElements(ll);
			r = m_lineDatas.Add(p);
		}
		if ( r != INVALID_INDEX )
		{
			m_Settle(ToInt(r));
			return ToInt(r);
		}
		return -1;
	}
	int		m_itemHeight;	///< Item�̍���
	CDatas	m_lineDatas;	///< �\�������̓��e
	DWORD	m_margin;		///< �\���J�n�ʒu
	SIZE	m_clientSize;	///< �N���C�A���g�T�C�Y
};



}; //MFC
}; //TNB
