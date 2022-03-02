#pragma once
/**
 *@file
 * ROW�w�b�_�R���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbWindowProcedureHooker.h"
#include "TnbBitmapImage.h"
#include "TnbDrawable.h"
#include "TnbMfcBitmapDC.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ROW�w�b�_�R���g���[��
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcRowHeaderCtrl.h
 * 
 *	@date 11/10/31	�V�K�쐬
 *	@date 14/04/07	WM_SIZE, WM_MOVE �ɑΉ��B
 *	@date 14/04/25	InsertItem(IDrawable&) ��ǉ��B
 *	@date 14/05/23	�\���͈͂̏C���B
 *	@date 14/05/26	HitTest() ��ǉ��B SetPartialView() ��V�K�B
 *	@date 14/10/15	GetHeaderWidth() ��V�K�B
 *	@date 15/10/01	�J�������ύX���Ƀo�f�B�ɉ��X�N���[���o�[�������[�h�Ȃ̂ɏo�Ă��܂��̂�(�Ƃ肠����)���P
 */
class CRowHeaderCtrl : public CWnd
{
	DEFSUPER(CWnd);
public:

	/// �R���X�g���N�^
	CRowHeaderCtrl(void) : m_pBuddyCtrl(NULL), m_headerWidth(0), m_offset(2, 0)
		, m_textColor(::GetSysColor(COLOR_WINDOWTEXT)), m_backColor(::GetSysColor(COLOR_BTNFACE))
		, m_hCursor(NULL), m_isWidthVariable(true), m_textStyle(DT_LEFT | DT_VCENTER), m_hasCapture(false)
		, m_isPartialOk(false)
	{
	}

	/**
	 * [�ݒ�] �e�L�X�g�\���X�^�C���ݒ�.
	 *	@note �f�t�H���g�� DT_LEFT | DT_VCENTER �ł��B
	 *	@param ts �X�^�C���B
	 */
	void SetTextStyle(DWORD ts)
	{
		m_textStyle = ts;
	}

	/**
	 * [�ݒ�] ���[�\���ݒ�.
	 *		�X�N���[���ɂ���đS�̂������Ȃ��A�C�e���̕\�����@���w�肵�܂��B
	 *	@param isPartialOk true �Ȃ猇���Ă��Ă��\�����܂��B false �Ȃ�\�����܂���i�f�t�H���g�j�B
	 */
	void SetPartialView(bool isPartialOk = false)
	{
		m_isPartialOk = isPartialOk;
	}

	/**
	 * [�ݒ�] �e�L�X�g�J���[�ݒ�.
	 *	@note �e�J�����̕`�����ݒ肵�Ă��Ȃ��ꍇ�A�ݒ肳��Ă���t�H���g�Ɛݒ肳��Ă��镶����\�����܂��B
	 *			���̍ێg����F�ł��B
	 *	@param color �e�L�X�g�J���[�B
	 */
	void SetTextColor(COLORREF color)
	{
		m_textColor = color;
	}
	
	/**
	 * [�ݒ�] �w�b�_�ݒ� 
	 *	@param pDrawer �w�b�_�`����B�ȗ�����ƁA�Ǝ��̕`��i�����ƉE���ɐF���]���j�ŕ`�悵�܂��B�����͕\�����܂���B
	 */
	void SetHeaderDrawer(const IDrawable* pDrawer = NULL)
	{
		m_phHeaderDrawer.Null();
		if ( pDrawer != NULL )
		{
			m_phHeaderDrawer = pDrawer->Clone();
		}
		Invalidate(FALSE);
	}

	/**
	 * [�ݒ�] �w�i�ݒ� 
	 *	@param color �w�i�F�B�A�C�e���̂Ȃ��Ƃ����h��F���w�肵�܂��B
	 *	@param pBkDrawer �w�i�`����B�ȗ�����ƁA�Ǝ��̕`��i�����ƉE���ɐF���]���j�ŕ`�悵�܂��B
	 *	@param offsetX �w�i�ƃA�C�e���� X �̃I�t�Z�b�g���w�肵�܂��B�ȗ�����ƂQ�ł��B
	 *	@param offsetY �w�i�ƃA�C�e���� Y �̃I�t�Z�b�g���w�肵�܂��B�ȗ�����ƂO�ł��B
	 */
	void SetBackground(COLORREF color, const IDrawable* pBkDrawer = NULL, int offsetX = 2, int offsetY = 0)
	{
		m_backColor = color;
		m_phBkDrawer.Null();
		m_offset.x = offsetX;
		m_offset.y = offsetY;
		if ( pBkDrawer != NULL )
		{
			m_phBkDrawer = pBkDrawer->Clone();
		}
		if ( ::IsWindow(_super::GetSafeHwnd()) )
		{
			_super::Invalidate(FALSE);
		}
	}

	/**
	 * [�ݒ�] ���_���X�g�R���g���[���ݒ�.
	 *	@param pCtrl ���X�g�R���g���[���B�R���g���[���̈ʒu�A�傫�����ύX����܂��B
	 *	@param headerWidth Row�w�b�_�̕��i�s�N�Z���j�B
	 *	@param isWidthVariable true �Ȃ�ω\�ɁB false �Ȃ�Œ�ɁB
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetBuddy(CListCtrl* pCtrl, size_t headerWidth, bool isWidthVariable = false)
	{
		if ( m_pBuddyCtrl == NULL )
		{
			if ( pCtrl != NULL && ::IsWindow(pCtrl->GetSafeHwnd()) )
			{
				m_myHooker.m_hParent = GetSafeHwnd();
				m_myHooker.Attach(pCtrl->GetSafeHwnd());
				pCtrl->ModifyStyle(WS_BORDER, 0);
				pCtrl->ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);
				SetWindowPos(pCtrl, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				bool isVisible = (GetStyle() & WS_VISIBLE) != 0;
				ShowWindow(isVisible ? SW_SHOW : SW_HIDE);
				SetFont(pCtrl->GetFont());
				//
				m_headerWidth = down_cast<int>(headerWidth);
				m_isWidthVariable = isWidthVariable;
				m_pBuddyCtrl = pCtrl;
				m_params.SetSize(pCtrl->GetItemCount());
				m_RewidthHeader();
				return true;
			}
		}
		return false;
	}

	/**
	 * [�ݒ�] �w�b�_���ݒ�.
	 *	@param headerWidth Row�w�b�_�̕��i�s�N�Z���j�B
	 *	@param isWidthVariable true �Ȃ�ω\�ɁB false �Ȃ�Œ�ɁB
	 */
	void SetHeaderWidth(size_t headerWidth, bool isWidthVariable = false)
	{
		m_headerWidth = down_cast<int>(headerWidth);
		m_isWidthVariable = isWidthVariable;
		m_RewidthHeader();
	}

	/**
	 * [�擾] �w�b�_���擾.
	 *	@return Row�w�b�_�̕��i�s�N�Z���j�B
	 */
	size_t GetHeaderWidth(void) const
	{
		return m_headerWidth;
	}

	/**
	 * [�擾] �A�C�e�����擾
	 *	@note ���_�̃��X�g�R���g���[���� GetItemCount() �Ɠ����l�ɂȂ�܂��B
	 *	@return �A�C�e����
	 */
	size_t GetItemCount(void) const
	{
		if ( m_pBuddyCtrl != NULL )
		{
			ASSERT( ToInt(m_params.GetSize()) == m_pBuddyCtrl->GetItemCount() );
		}
		return m_params.GetSize();
	}

	/**
	 * [�ݒ�] �A�C�e���`����ݒ�.
	 *	@param item �A�C�e���C���f�b�N�X.
	 *	@param draw �`����B�N���[�����L�����܂��̂ŁA�j�����Ă��܂��܂���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetItemDrawer(int item, const IDrawable& draw)
	{
		if ( m_params.IsInRange(item) )
		{
			m_params[item].phDraw = draw.Clone();
			Invalidate(FALSE);
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �A�C�e���������ݒ�.
	 *	@param item �A�C�e���C���f�b�N�X.
	 *	@param lpszText �������
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetItemText(int item, LPCTSTR lpszText)
	{
		if ( m_params.IsInRange(item) )
		{
			m_params[item].phDraw.Null();
			m_params[item].text = lpszText;
			Invalidate(FALSE);
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �A�C�e���}��.
	 *	@note ���_�̃��X�g�R���g���[���� InsertItem() ���A�C�e��������͋�ŃR�[�����A lpszItem �� row�w�b�_�̕�����Ƃ��Ďg�p���܂��B
	 *	@param item �A�C�e���C���f�b�N�X.
	 *	@param lpszItem ������
	 *	@retval �}�C�i�X ���s
	 *	@retval 0�ȏ� �����B�l�́A�}�������A�C�e���C���f�b�N�X�B
	 */
	int InsertItem(int item, LPCTSTR lpszItem)
	{
		if ( m_pBuddyCtrl != NULL )
		{
			int r = m_pBuddyCtrl->InsertItem(item, _T(""));
			if ( r >= 0 )
			{
				SetItemText(r, lpszItem);
			}
			return r;
		}
		return -1;
	}

	/**
	 * [�ݒ�] �A�C�e���}��.
	 *	@note ���_�̃��X�g�R���g���[���� InsertItem() ���A�C�e��������͋�ŃR�[�����A lpszItem �� row�w�b�_�̕�����Ƃ��Ďg�p���܂��B
	 *	@param item �A�C�e���C���f�b�N�X.
	 *	@param draw �`����B�N���[�����L�����܂��̂ŁA�j�����Ă��܂��܂���B
	 *	@retval �}�C�i�X ���s
	 *	@retval 0�ȏ� �����B�l�́A�}�������A�C�e���C���f�b�N�X�B
	 */
	int InsertItem(int item, const IDrawable& draw)
	{
		int r = InsertItem(item, _T(""));
		if ( r >= 0 )
		{
			SetItemDrawer(r, draw);
		}
		return r;
	}

	/**
	 * [�擾] �q�b�g�e�X�g.
	 *		�w��̍��W�̃A�C�e���ԍ���Ԃ��܂��B
	 *	@param pt �N���C�A���g���W
	 *	@retval -99 �w�b�_��
	 *	@retval �}�C�i�X �A�C�e���Ȃ�
	 *	@retval ��L�ȊO �A�C�e���ԍ�
	 */
	int HitTest(const CPoint& pt) const
	{
		int r = -1;
		if ( m_pBuddyCtrl != NULL )
		{
			CRect rc;
			GetClientRect(rc);
			if ( rc.PtInRect(pt) )
			{
				CHeaderCtrl* pHead = m_pBuddyCtrl->GetHeaderCtrl();
				if ( pHead != NULL )
				{
					pHead->GetWindowRect(rc);
					if ( pt.y >= rc.Height() )
					{
						r = m_pBuddyCtrl->HitTest(CPoint(0, pt.y));
					}
					else
					{
						r = -99; //�@�w�b�_��
					}
				}
			}
		}
		return r;
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
		switch ( message )
		{
		case CMyHooker::WM_RHC_HOOKMESSAGE:
			{
				CMyHooker::TMsg* P = reinterpret_cast<CMyHooker::TMsg*>(lParam);
				if ( P != NULL )
				{
					m_BuddyWindowProc(P->message, P->wParam, P->lParam, P->lResult);
				}
			}
			break;
		case WM_SIZE:
			m_RewidthHeader();
			break;
		case WM_MOVE:
			m_RewidthHeader();
			break;
		case WM_ERASEBKGND:
			return 0;
		case WM_PAINT:
			m_Draw(CPaintDC(this));
			return 0;
		case WM_LBUTTONDOWN:
			if ( m_isWidthVariable )
			{
				CPoint pos(lParam);
				if ( m_IsDragSpace(pos) )
				{
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
					::SetCapture(GetSafeHwnd());
					m_hasCapture = true;
					m_captureStartPos = CPoint(lParam);
					m_captureStartHeaderWidth = m_headerWidth;
				}
			}
			break;
		case WM_LBUTTONUP:
			if ( m_hasCapture )
			{
				::ReleaseCapture();
				m_hasCapture = false;
			}
			break;
		case WM_RBUTTONDOWN:
			if ( m_hasCapture )
			{
				::ReleaseCapture();
				m_hasCapture = false;
				m_headerWidth = m_captureStartHeaderWidth;
				m_RewidthHeader();
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_isWidthVariable )
			{
				if ( m_hCursor == NULL )
				{
					m_hCursor = ::GetCursor();
				}
				CPoint pos(lParam);
				if ( m_hasCapture )
				{
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
					int x = m_captureStartHeaderWidth + pos.x - m_captureStartPos.x;
					CRect rc;
					GetClientRect(rc);
					int l = rc.Width() - 5;
					if ( x > l )
					{
						x = l;
					}
					if ( x < 5 )
					{
						x = 5;
					}
					m_headerWidth = x;
					m_RewidthHeader();
					if ( m_pBuddyCtrl != NULL )
					{
						m_pBuddyCtrl->GetHeaderCtrl()->RedrawWindow();	//�J�����̍ĕ`��ɂ��ăJ����������������
					}
				}
				else if ( m_IsDragSpace(pos) )
				{
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
				}
				else
				{
					::SetCursor(m_hCursor);
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
		m_pBuddyCtrl = NULL;
		m_hCursor = NULL;
		_super::ModifyStyle(0, SS_NOTIFY);
		_super::PreSubclassWindow();
	}

private:
	// �w�b�_�̕��ύX
	void m_RewidthHeader(void)
	{
		if ( m_pBuddyCtrl != NULL )
		{
			CRect rc;
			GetClientRect(rc);
			ClientToScreen(rc);
			GetParent()->ScreenToClient(rc);
			rc.left += m_headerWidth;
			m_pBuddyCtrl->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
		}
	}
	// �h���b�N��ԁH
	bool m_IsDragSpace(const POINT& pos) const
	{
		if ( m_pBuddyCtrl != NULL )
		{
			CRect headerRect;
			CHeaderCtrl* pHead = m_pBuddyCtrl->GetHeaderCtrl();
			pHead->GetClientRect(headerRect);
			if ( pos.x >= m_headerWidth - 5 && pos.x <= m_headerWidth )
			{
				if ( pos.y < headerRect.Height() )
				{
					return true;
				}
			}
		}
		return false;
	}
			
	// �w�i�`��
	void m_BkDraw(HDC hdc, const CRect& rect)
	{
		if ( m_phBkDrawer != NULL )
		{
			m_phBkDrawer->Resize(rect.Size());
			m_phBkDrawer->Draw(hdc, rect.left, rect.top);
		}
		else
		{
			CRect r = rect;
			r.top = r.bottom - 1;
			::InvertRect(hdc, r);
			r = rect;
			r.left = r.right - 1;
			::InvertRect(hdc, r);
		}
	}
	// �S�̕`��
	void m_Draw(HDC hdc)
	{
		CRect clientRect;
		GetClientRect(clientRect);
		CBitmapImage bi;
		bi.Set(m_headerWidth, clientRect.Height(), m_backColor);
		if ( m_pBuddyCtrl != NULL )
		{
			CBitmapDC dc(&bi);
			CDcSelectAssistant dsa(dc);
			dsa.SetBkMode(TRANSPARENT);
			dsa.SelectFont(GetFont());
			dsa.SetTextColor(m_textColor);
			{
				// �w�b�_��
				CRect headerRect;
				CHeaderCtrl* pHead = m_pBuddyCtrl->GetHeaderCtrl();
				pHead->GetClientRect(headerRect);
				CRect r = headerRect;
				r.left = 0;
				r.right = m_headerWidth;
				m_BkDraw(dc, r);
				if ( ! m_phHeaderDrawer.IsNull() )
				{				
					m_phHeaderDrawer->Resize(r.Size());
					m_phHeaderDrawer->Draw(dc, r.left, r.top);
				}
			}
			int count = m_pBuddyCtrl->GetItemCount();
			int cpp = m_pBuddyCtrl->GetCountPerPage();
			int si = m_pBuddyCtrl->GetTopIndex();
			loop ( i, cpp + (m_isPartialOk ? 1 : 0) )
			{
				if ( si >= count )
				{
					break;
				}
				CRect rc;
				m_pBuddyCtrl->GetItemRect(si, rc, LVIR_BOUNDS);
				rc.left = 0;
				rc.right = m_headerWidth;
				m_BkDraw(dc, rc);
				rc.left += m_offset.x;
				rc.top += m_offset.y;
				HRGN hRgn = ::CreateRectRgnIndirect(&rc);
				::SelectClipRgn(dc, hRgn);
				if ( m_params.IsInRange(si) )
				{
					TParam& prm = m_params[si];
					if ( prm.phDraw.IsNull() )
					{
						::DrawText(dc, prm.text, -1, rc, m_textStyle);
					}
					else
					{
						prm.phDraw->Resize(rc.Size());
						prm.phDraw->Draw(dc, rc.left, rc.top);
					}
				}
				::SelectClipRgn(dc, NULL);
				_DeleteObject(hRgn);	
				si++;
			}
		}
		bi.Draw(hdc);
	}
	// ���_�̃v���V�[�W��
	void m_BuddyWindowProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT lResult)
	{
		switch ( message )
		{
		case WM_KEYDOWN:
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
		case WM_SIZE:
			{
				CRect rc;
				GetClientRect(rc);
				rc.right = m_headerWidth;
				InvalidateRect(rc, FALSE);
			}
			break;
		case WM_SHOWWINDOW:
			_super::ShowWindow((wParam != FALSE) ? SW_SHOW : SW_HIDE);
			break;
		case LVM_INSERTITEM:
			if ( lResult >= 0 )
			{
				m_params.Insert(lResult, TParam());
			}
			break;
		case LVM_DELETEITEM:
			if ( lResult != 0 )
			{
				m_params.Remove(wParam);
			}
			break;
		case LVM_DELETEALLITEMS:
			if ( lResult != 0 )
			{
				m_params.RemoveAll();
			}
			break;
		}
	}

	/**
	 * ���_�̃v���V�[�W���t�b�J�[.
	 */
	class CMyHooker : public CWindowProcedureHooker
	{
		DEFSUPER(CWindowProcedureHooker);
	public:
		enum
		{
			WM_RHC_HOOKMESSAGE = WM_APP,	///< �t�b�N���b�Z�[�W�ʒm
		};
		/// �^�O
		struct TMsg
		{
			LRESULT	lResult;		///< ����
			UINT	message;		///< ���b�Z�[�W
			WPARAM	wParam;			///< WPARAM
			LPARAM	lParam;			///< LPARAM
		};
		HWND	m_hParent;			///< �t�b�N����
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
			switch ( message )
			{
			case WM_SHOWWINDOW:
			case WM_KEYDOWN:
			case WM_VSCROLL:
			case WM_MOUSEWHEEL:
			case WM_SIZE:
			case LVM_INSERTITEM:
			case LVM_DELETEITEM:
			case LVM_DELETEALLITEMS:
				{
					LRESULT r = _super::WindowProc(message, wParam, lParam);
					TMsg msg = { 0 };
					msg.message = message;
					msg.wParam = wParam;
					msg.lParam = lParam;
					msg.lResult = r;
					::SendMessage(m_hParent, WM_RHC_HOOKMESSAGE, NULL, reinterpret_cast<LPARAM>(&msg));
					return r;
				}
				break;
			default:
				break;
			}
			return _super::WindowProc(message, wParam, lParam);
		}
	};

	/// �p�����^
	struct TParam
	{
		CStr			text;		///< ������
		IDrawable::Ptr	phDraw;		///< �`����
	};

	TParam				m_headParam;				///< �w�b�_�̃p�����^
	CVectorT<TParam>	m_params;					///< �A�C�e���p�����[�^�Q
	COLORREF			m_textColor;				///< �����F
	IDrawable::Ptr		m_phHeaderDrawer;			///< �w�b�_�`��
	IDrawable::Ptr		m_phBkDrawer;				///< �A�C�e���w�i�`��
	COLORREF			m_backColor;				///< �w�i�F
	CMyHooker			m_myHooker;					///< �o�f�B�v���V�[�W���t�b�J�[
	CListCtrl*			m_pBuddyCtrl;				///< �o�f�B�R���g���[��
	int					m_headerWidth;				///< ROW �w�b�_�̕�
	CPoint				m_offset;					///< �A�C�e���`��I�t�Z�b�g
	DWORD				m_textStyle;				///< �����\���X�^�C��
	bool				m_isWidthVariable;			///< ���͉ρH
	HCURSOR				m_hCursor;					///< �J�[�\��
	bool				m_hasCapture;				///< �L���v�`���[���H
	CPoint				m_captureStartPos;			///< �J�n���̍��W
	int					m_captureStartHeaderWidth;	///< �J�n���̕�
	bool				m_isPartialOk;				///< ���[�ȕ\�������H
};



}; // MFC
}; // TNB
