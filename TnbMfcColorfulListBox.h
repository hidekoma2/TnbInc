#pragma once
/**
 *@file
 * �J���t���\���@�\�tListBox�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerVector.h"
#include "TnbMfcListBoxCp.h"
#include "TnbMfcBitmapDC.h"
#include "TnbDcSelectAssistant.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �J���t��ListBox�R���g���[��
 *
 *		�ꕶ���P�ʂŐF���w��o���� ListBox�R���g���[���ł��B
 *
 *		Ctrl+A , Ctrl+C ���g���� ListBox�R���g���[���ł��B
 *
 *	@note �I���s�́A�����F�Ɣw�i�F�����]���\������܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�{�b�N�X�̃v���p�e�B�́A�u�I��(C)�v���u�g���v�ɁA�u�I�[�i�[�`��(O)�v���u�ρv��
 *		�����񂠂�̃`�F�b�N��ON�ɂ��Ă����Ă��������B
 *	
 *		�w�b�_�t�@�C��
 *		\code
 *		class CFooDlg : public CDialog
 *		{
 *				;
 *			CColorfulListBox m_listColorful;		//�� CListBox �� CColorfulListBox �ɏ�������
 *				;
 *		};
 *		\endcode
 *		
 *		\code
 *		void CFooDlg::Foo(void)
 *		{
 *			CColorfulListBox::CColor cc;
 *			cc.Add(4, 2, RGB(255, 0, 0), RGB(0, 255, 0)); // 4�����ڂ���2�������̐F���w��
 *			cc.Add(8, 2, RGB(255, 255, 0), RGB(0, 255, 0)); // 8�����ڂ���2�������̐F���w��
 *			m_listColorful.SetCurrentColor(cc);	// �F�ݒ�
 *			m_listColorful.AddString("1234567890abcdefghijklmn");
 *			m_listColorful.AddString("��������������������");
 *			m_listColorful.AddString("1234n");
 *			m_listColorful.AddString("1����!");
 *			cc.RemoveAll();
 *			cc.Add(5, 3, RGB(255, 255, 0), RGB(0, 0, 0)); // 5�����ڂ���3�������̐F���w��
 *			cc.Add(9, 5, RGB(255, 255, 255), RGB(255, 0, 0)); // 9�����ڂ���5�������̐F���w��
 *			m_listColorful.SetCurrentColor(cc);	// �F�Đݒ�
 *			m_listColorful.AddString("1234567890abcdefghijklmn");
 *			m_listColorful.AddString("��������������������");
 *			m_listColorful.AddString("1234n");
 *			m_listColorful.AddString("1����!");
 *		}
 *		\endcode
 *
 *	@note �X�N���[������ƁA�e�� WM_VSCROLL �� SendMessage ���܂��BLPARAM �ɂ� �{�R���g���[���� HWND ��n���܂��B
 *		���̂��߁A MFC �� CWnd::OnVScroll() �̑�O������ CScrollBar* �ɂ͖{�N���X�̃C���X�^���X���n��܂��B
 *		CScrollBar* �Ƃ��đ��삵�Ȃ��悤�ɒ��ӂ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcColorfulListBox.h
 * 
 *	@date 09/06/02	�V�K�쐬
 */
class CColorfulListBox : public CListBoxCp
{
	DEFSUPER(CListBoxCp);
public:
	
	/**
	 * �J���t��ListBox�R���g���[���̃J���[���.
	 *	
	 *		���Ƃ��āA�x�[�X�����F�A�x�[�X�w�i�F�������Ă��܂��B
	 *		����ȊO�ɁA�w��͈݂͂̂ɕ����F�A�w�i�F���w��ł��܂��B
	 */
	class CColor
	{
	public:
		/// �p�����[�^
		struct TParam
		{
			size_t		len;		///< ����
			COLORREF	textColor;	///< �����F
			COLORREF	backColor;	///< �w�i�F
		};
		/**
		 * �R���X�g���N�^
		 *	@note �x�[�X�����F�A�x�[�X�w�i�F�̓V�X�e���J���[�Ɠ����ɂȂ�܂��B
		 */
		CColor(void) : m_textColor(::GetSysColor(COLOR_WINDOWTEXT)), m_backColor(::GetSysColor(COLOR_WINDOW))
		{
		}
		/**
		 * �R���X�g���N�^
		 *	@param textColor �x�[�X�����F
		 *	@param backColor �x�[�X�w�i�F
		 */
		CColor(COLORREF textColor, COLORREF backColor) : m_textColor(textColor), m_backColor(backColor)
		{
		}
		/**
		 * [�m�F] ����ہH
		 *	@retval true �����.
		 *	@retval false ���L��
		 */
		bool IsEmpty(void) const
		{
			return m_infos.IsEmpty();
		}
		/**
		 * [����] �S����.
		 */
		void RemoveAll(void)
		{
			m_infos.RemoveAll();
			m_params.RemoveAll();
		}
		/**
		 * [�ǉ�] �F���ǉ�.
		 *	@param pos �|�W�V�����B��ԍ�(�擪)���O�ł��B
		 *	@param len ����.
		 *	@param textColor �����F�B�ȗ�����ƃx�[�X�̕����F�ɂȂ�܂��B
		 *	@param backColor �w�i�F�B�ȗ�����ƃx�[�X�J�w�i�F�ɂȂ�܂��B
		 */
		void Add(INDEX pos, size_t len, COLORREF textColor = CLR_INVALID, COLORREF backColor = CLR_INVALID)
		{
			if ( ! IS_RGBVALUE(textColor) )
			{
				textColor = m_textColor;
			}
			if ( ! IS_RGBVALUE(backColor) )
			{
				backColor = m_backColor;
			}
			if ( pos + len >= m_infos.GetSize() )
			{
				TInfo pa(m_textColor, m_backColor);
				loop ( i, (pos + len) - m_infos.GetSize() )
				{
					m_infos.Add(pa);
				}
			}
			TInfo pa(textColor, backColor);
			loop ( i, len )
			{
				m_infos.Set(pos + i, pa);
			}
			m_params.RemoveAll();
		}
		/**
		 * [�擾] ���ꗗ�擾.
		 *	@note ���[���珇�ɓ������������Ă��邩�ƌ�����񂪓����܂��B�Ⴆ�΁A {5, ��}, {3, ��} �ƌ������Ȃ�A
		 *		�Ԃ��T�����A�����Đ��R�����ƌ������ł��B
		 *	@note CColorfulListBox ���Ŏg�p���܂��B�g�����Ƃ͂قƂ�ǂ���܂���B
		 *	@return ���ꗗ�B
		 */
		const CVectorT<TParam>& GetParam(void) const
		{
			if ( m_params.IsEmpty() )
			{
				if ( m_infos.IsEmpty() )
				{
					TParam pa = { 100000, m_textColor, m_backColor };
					m_params.Add(pa);
				}
				else
				{
					size_t ll = 0;
					loop ( i, m_infos.GetSize() - 1 )
					{
						ll++;
						if ( m_infos[i].textColor != m_infos[i + 1].textColor || m_infos[i].backColor != m_infos[i + 1].backColor )
						{
							//�Ⴄ
							TParam pa = { ll, m_infos[i].textColor, m_infos[i].backColor };
							m_params.Add(pa);
							ll = 0;
						}
					}
					ll++;
					INDEX i = m_infos.GetSize() - 1;
					TParam pa = { ll, m_infos[i].textColor, m_infos[i].backColor };
					m_params.Add(pa);
					TParam pa2 = { 100000, m_textColor, m_backColor };
					m_params.Add(pa2);
				}
			}
			return m_params;
		}

	private:
		/// �p�����[�^
		struct TInfo
		{
			COLORREF	textColor;	///< �����F
			COLORREF	backColor;	///< �w�i�F
			/// �R���X�g���N�^
			TInfo(COLORREF text = 0, COLORREF back = 0)
				: textColor(text), backColor(back)
			{
			}
		};
		CVectorT<TInfo>				m_infos;
		mutable CVectorT<TParam>	m_params;
		COLORREF					m_textColor;	///< �x�[�X�̕����F
		COLORREF					m_backColor;	///< �x�[�X�̔w�i�F
	};

	/// �R���X�g���N�^
	CColorfulListBox(void)
		: _super(), m_iItemHeight(-1), m_iItemWidth(-1), m_margin(0), m_isView(true), m_postCount(-1), m_oneCharWidth(-1)
	{
	}

	/// �f�X�g���N�^
	~CColorfulListBox(void)
	{
		m_ColorsRemoveAll();
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *		�ǉ����镶����̐F���w�肵�܂��B
	 *	@param color �F�w��
	 */
	void SetCurrentColor(const CColorfulListBox::CColor& color)
	{
		m_currentColor = color;
	}

	/**
	 * [�ݒ�] �����F�ݒ�.
	 *		�w��s�̐F���w�肵�܂��B
	 *	@param nIndex �C���f�b�N�X
	 *	@param color �F�w��
	 */
	void SetColor(INDEX nIndex, const CColorfulListBox::CColor& color)
	{
		if ( m_colors.GetSize() > nIndex )
		{
			m_colors[nIndex]->color = color;
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
		m_iItemHeight = -1;
	}

	/**
	 * [�ݒ�] PostString�\�����[�h�ݒ�.
	 *	@note PostString() ���s���A�ǉ����������񂪕\������悤�ɃX�N���[�������邩�ۂ��A�ݒ�ł��܂��B
	 *	@param isView true �Ȃ�ǉ������񂪕\�������悤�ɃX�N���[�����܂��B false �Ȃ�X�N���[�����܂���B
	 *	@param isDelayMode true �Ȃ�X�N���[����x�������܂�(�����ɗʂ�\��������ꍇ�ɗL��)�B false �Ȃ烊�A���^�C���ŃX�N���[�����܂���B
	 */
	void SetPostViewMode(bool isView, bool isDelayMode = false)
	{
		m_isView = isView;
		m_postCount = isDelayMode ? 0 : -1;
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		�w�肵���F�ŕ������ǉ����܂��B
	 *	@note �ʃX���b�h����ł��ǉ����\�ł��B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 */
	void PostString(int nIndex, LPCTSTR lpszItem)
	{
		CColor c;
		PostString(nIndex, lpszItem, c);
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *		�w�肵���F�ŕ������ǉ����܂��B
	 *	@note �ʃX���b�h����ł��ǉ����\�ł��B
	 *	@note �F���w�肷��ƁA SetCurrentColor() �Ŏw�肵���̂Ɠ������ʂ��c��܂��B
	 *	@param nIndex �������}������ʒu�� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *					���̃p�����[�^�� -1 �̏ꍇ�A������̓��X�g�̍Ō�ɒǉ�����܂��B
	 *	@param lpszItem �}�����镶����B
	 *	@param color �F�w��B
	 */
	void PostString(int nIndex, LPCTSTR lpszItem, const CColorfulListBox::CColor& color)
	{
		if ( ! ::IsWindow(m_hWnd) )
		{ 
			return;
		}
		TParam* P = new TParam;
		P->item = lpszItem;
		P->color = color;
		if ( ! _super::PostMessage(WM_CLB_POSTSTRING, nIndex, reinterpret_cast<LPARAM>(P)) )
		{
			delete P;
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
		UINT nIndex = lpDrawItemStruct->itemID;
//		if ( static_cast<UINT>(m_linesString.GetSize()) <= nIndex )
		if ( static_cast<UINT>(m_colors.GetSize()) <= nIndex )
		{
			return;
		}
		CString strText = m_colors[nIndex]->item;
		CRect rcItem = lpDrawItemStruct->rcItem;
		const CColor& colors = m_colors[nIndex]->color;
		const CVectorT<CColor::TParam> vp = colors.GetParam();
		//
		CBitmapImage bi;
		bi.Set(rcItem.Width(), rcItem.Height());
		{
			CRect rect = rcItem;
			rect.OffsetRect(-rect.left, -rect.top);
			CRect rc2 = rect;
			//
			CBitmapDC dc(&bi);
			CDcSelectAssistant dca(dc);
			dca.SelectFont(_super::GetFont());
			dca.SetBkMode(TRANSPARENT);
			rect.top += m_margin;
			COLORREF colorText = 0;
			COLORREF colorBack = 0;
			loop ( i, vp.GetSize() )
			{
				const CColor::TParam& v = vp[i];
				CString s = strText.Left(ToInt(v.len));
				colorText = v.textColor;
				colorBack = v.backColor;
				if ( (lpDrawItemStruct->itemState & ODS_SELECTED) != 0 )
				{
					colorText = v.backColor;
					colorBack = v.textColor;
				}
				if ( s.IsEmpty() )
				{
					rect.right = rect.left;
				}
				else
				{
					dc.DrawText(s, &rect, DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT);
					dc.FillSolidRect(&rect, colorBack);
					dca.SetTextColor(colorText);
					dc.DrawText(s, &rect, DT_SINGLELINE | DT_NOPREFIX);
					rect.left = rect.right;
				}
				rect.right = lpDrawItemStruct->rcItem.right;
				if ( strText.GetLength() > ToInt(v.len) )
				{
					strText = strText.Mid(ToInt(v.len));
				}
				else
				{
					strText.Empty();
				}
			}
			dc.FillSolidRect(&rect, colorBack);
			if ( ! strText.IsEmpty() )
			{
				dca.SetTextColor(colorText);
				dc.DrawText(strText, &rect, DT_SINGLELINE | DT_NOPREFIX);
			}
			//
			if ( (lpDrawItemStruct->itemState & ODS_FOCUS) != 0 )
			{
				dca.SetTextColor(RGB(255, 255, 255));
				dc.DrawFocusRect(&rc2);	//�j��
			}
		}
		bi.Draw(lpDrawItemStruct->hDC, rcItem.left, rcItem.top);
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
		if ( m_iItemHeight < 0 )
		{
			LOGFONT lf;
			GetFont()->GetLogFont(&lf);
			m_iItemHeight = lf.lfHeight;
			if ( m_iItemHeight < 0 )
			{
				m_iItemHeight = -m_iItemHeight;
			}
			m_iItemHeight += m_margin * 2;
		}
		lpMeasureItemStruct->itemHeight = m_iItemHeight;
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
		int iIndex = ToInt(wParam);
		switch ( message )
		{
		case WM_ERASEBKGND: //�w�i
			{
				CDC dc;
				dc.Attach(reinterpret_cast<HDC>(wParam));
				CRect rect;
				GetClientRect(rect);
				int y = (GetCount() - GetTopIndex()) * m_iItemHeight;
				int h = rect.Height();
				if( h > y )
				{
					if ( y < 0 )
					{
						y = 0;
					}
					CBrush* B = dc.GetCurrentBrush();
					dc.FillRect(CRect(0, y, rect.Width(), h), B);
				}
				dc.Detach();
			}
			return 0;
		case WM_SETFONT:
			m_oneCharWidth = -1;
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			RedrawWindow();
			break;
		case LB_GETTEXTLEN: //�����񒷎擾
			// @param iIndex INDEX 
			// @return ����
			if ( IsInRange(iIndex, m_colors.GetSize()) )
			{
				return m_colors[iIndex]->item.GetLength();
			}
			return 0;
		case LB_GETTEXT: //������擾
			// @param[in] iIndex INDEX 
			// @param[out] lParam �i�[��
			if ( IsInRange(iIndex, m_colors.GetSize()) )
			{
				STRLIB::Copy(reinterpret_cast<TCHAR*>(lParam), m_colors[iIndex]->item);
			}
			return 0;
		case LB_DELETESTRING: //�폜
			// @param iIndex INDEX 
			if ( IsInRange(iIndex, m_colors.GetSize()) )
			{
//				m_linesString.RemoveAt(iIndex);
				delete m_colors[iIndex];
				m_colors.Remove(iIndex);
			}
			break;
		case LB_ADDSTRING: //�ǉ�
			// @param lParam ������A�h���X 
			// @note ���ۃR���g���[���ɐݒ肷��̂� "" 
			{
				ASSERTLIB( ToInt(m_colors.GetSize()) == _super::GetCount() );
//				INT_PTR i = m_linesString.Add(reinterpret_cast<LPCTSTR>(lParam));
				TParam* P = new TParam;
				P->item = reinterpret_cast<LPCTSTR>(lParam);
				P->color = m_currentColor;
				m_colors.Add(P);
				m_CheckWidth(P->item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_INSERTSTRING: //�}��
			// @param iIndex INDEX 
			// @param lParam ������A�h���X 
			// @note ���ۃR���g���[���ɐݒ肷��̂� "" 
			if ( iIndex < 0 )
			{
				ASSERTLIB( ToInt(m_colors.GetSize()) == _super::GetCount() );
				TParam* P = new TParam;
				P->item = reinterpret_cast<LPCTSTR>(lParam);
				P->color = m_currentColor;
				m_colors.Add(P);
				m_CheckWidth(P->item);
			}
			else if ( IsInRange(iIndex, m_colors.GetSize() + 1) )
			{
	//			m_linesString.InsertAt(iIndex, reinterpret_cast<LPCTSTR>(lParam));
				TParam* P = new TParam;
				P->item = reinterpret_cast<LPCTSTR>(lParam);
				P->color = m_currentColor;
				m_colors.Insert(iIndex, P);
				m_CheckWidth(P->item);
			}
			lParam = reinterpret_cast<LPARAM>(_T(""));
			break;
		case LB_RESETCONTENT: //�S����
//			m_linesString.RemoveAll();
//			m_colors.RemoveAll();
			m_ColorsRemoveAll();
			m_iItemWidth = -1;
			_super::SetHorizontalExtent(m_iItemWidth);
			break;
		case WM_TIMER:
			if ( wParam == TIMERID_SETTOPINDEX )
			{
				KillTimer(TIMERID_SETTOPINDEX);
				m_postCount = 0;
				_super::SetTopIndex(m_lastPostIndex);
			}
			break;
		case WM_CLB_POSTSTRING:
			{
				TParam* P = reinterpret_cast<TParam*>(lParam);
				if ( ! P->color.IsEmpty() )
				{
					m_currentColor = P->color;
				}
				if ( ::IsWindow(m_hWnd) )
				{
//					CStr s;
//					s.Format("%s      -(%d)", P->item, GetCount());
//					LRESULT r = _super::InsertString(iIndex, s);
					LRESULT r = _super::InsertString(iIndex, P->item);
					KillTimer(TIMERID_SETTOPINDEX);
					if ( m_isView )
					{
						if ( m_postCount < 0 )
						{
							_super::SetTopIndex(ToInt(r));
						}
						else if ( ++m_postCount > 4 )
						{
							m_postCount = 0;
							_super::SetTopIndex(ToInt(r));
						}
						else
						{
							m_lastPostIndex = ToInt(r);
							SetTimer(TIMERID_SETTOPINDEX, 100, NULL);
						}
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
			ASSERT( dwStyle & LBS_OWNERDRAWVARIABLE );
			ASSERT( dwStyle & LBS_EXTENDEDSEL );
			ASSERT( dwStyle & LBS_HASSTRINGS );
			ASSERT( m_colors.GetSize() == 0 );
		#endif
		_super::PreSubclassWindow();
	}
	/**
	 * �j��
	 */
	virtual void PostNcDestroy(void) 
	{
//		m_linesString.RemoveAll();
//		m_colors.RemoveAll();
		m_ColorsRemoveAll();
		_super::PostNcDestroy();
	}

private:
	enum { TIMERID_SETTOPINDEX = 1 };
	/// PostString �p�p�����[�^�^
	struct TParam
	{
		CString		item;		///< ������
		CColor		color;		///< �F
	};
	int						m_iItemHeight;			///< Item�̍���
	int						m_iItemWidth;			///< Item�̕�
	CColor					m_currentColor;			///< ���ǉ�����J���[
	CVectorT<TParam*>		m_colors;				///< �\�������̃J���[
//	CPointerVectorT<TParam>	m_colors;				///< �\�������̃J���[
//	CPointerVectorT<CColor>	m_colors;				///< �\�������̃J���[
//	CStringArray			m_linesString;			///< �\�������̓��e
	DWORD					m_margin;				///< �\���J�n�ʒu
	bool					m_isView;				///< �ǉ����\�����邩�H
	int						m_oneCharWidth;
	//
	DWORD					m_lastPostIndex;
	int						m_postCount;

	void m_ColorsRemoveAll(void)
	{
		loop ( i, m_colors )
		{
			delete m_colors[i];
		}
		m_colors.RemoveAll();
	}

	/**
	 * �R���g���[���̕��̒���
	 */
	void m_CheckWidth(const CString& strItem)
	{
		if ( m_oneCharWidth < 0 )
		{
			CDC* pDC = GetDC();
			CFont* pFont = _super::GetFont();
			CFont* pFontOld = pDC->SelectObject(pFont);
			m_oneCharWidth = pDC->GetTextExtent("W").cx;
			pDC->SelectObject(pFontOld);
			ReleaseDC(pDC);
		}
		int l = strItem.GetLength() * m_oneCharWidth;
		if ( m_iItemWidth < l )
		{
			m_iItemWidth = l;
			_super::SetHorizontalExtent(m_iItemWidth);
		}
	}
};



}; // MFC
}; // TNB
