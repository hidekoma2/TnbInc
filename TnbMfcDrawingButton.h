#pragma once
/**
 *@file
 * �`����{�^���֌W�̃w�b�_
 *
 *		�`������g�p�����{�^����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPartsDrawable.h"
#include "TnbMfcAbstractButton.h"
#include "TnbMfcBitmapDC.h"
#include "TnbWndTextDrawer.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����{�^���R���g���[��
 *
 *	@note ���\�[�X�G�f�B�^�ł̐ݒ�̃R���g���[���ɂȂ�܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^�������W�I�{�^�����`�F�b�N�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@see CAbstractButton
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingButton.h
 * 
 *	@date 07/11/24 �V�K�쐬
 *	@date 07/12/20 �R�X�e�[�g�`�F�b�N�{�b�N�X�̃v�b�V���{�^���^�ɂ��Ή��iCheck����Draw���n�[�t�g�[���ŕ\���j�B
 *	@date 07/12/20 �v�b�V���{�^���̓�����C���B
 */
class CDrawingButton : public CAbstractButton
{
	DEFSUPER(CAbstractButton);
	typedef CDrawTextBase _text;
	enum { TIMERID_DRAW = 1 };
	/// �R���g���[����ޒl
	enum EKind
	{
		BUTTON		= DFCS_BUTTONPUSH,		///< �{�^��
		CHECKBOX	= DFCS_BUTTONCHECK,		///< �`�F�b�N�{�b�N�X
		CB3STATE	= DFCS_BUTTON3STATE,	///< 3�X�e�[�g�`�F�b�N�{�b�N�X
		RADIOBUTTON	= DFCS_BUTTONRADIO,		///< ���W�I�{�^��
	};
	bool					m_boIsAuto;				///< �I�[�g�i�R���g���[���̃X�^�C��(�r���ύX���T�|�[�g)�j
	int						m_checkState;			///< �`�F�b�N��� BST_UNCHECKED�Ȃ�
	CBitmapHandle			m_buttonBitmap;			///< ��ʂ̉摜
	CBitmapHandle			m_lastBitmap;			///< �Ō�ɕ\�������摜
	int						m_bitmapChangeTime;
	// �`�F�b�N�ύX�i�O���[�v���̑��̂��I�t�ɂ���j
	void m_SetCheck(int c)
	{
		if ( c < 0 || c > 2 ) { c = 1;/*0�ȊO�Ȃ�1�ɂ��Ă���*/ }
		if ( m_checkState != c )
		{
			m_buttonBitmap.Null();
			m_checkState = c;
			_super::Invalidate();
		}
		if ( m_kind == RADIOBUTTON )
		{
			if ( c == BST_CHECKED )
			{
				CWnd* pParent = _super::GetParent();
				HWND hWnd = m_hWnd;
				while ( true )
				{
					HWND h = ::GetNextDlgGroupItem(*pParent, hWnd, true);
					if ( h == NULL || h == m_hWnd ) { break; }
					::SendMessage(h, BM_SETCHECK, BST_UNCHECKED, 0); 
					hWnd = h;
				}
				_super::ModifyStyle(0, WS_TABSTOP);
			}
			else
			{
				_super::ModifyStyle(WS_TABSTOP, 0);
			}
		}
	}
	EKind				m_kind;					///< �R���g���[�����(�r���ύX���T�|�[�g)
	LONG				m_originalStyle;		///< �I���W�i���X�^�C��
	LONG				m_originalExStyle;		///< �I���W�i���g���X�^�C��
	IPartsDrawable*		m_pPartsDraw;			///< �R���g���[���̕`��IF
	IDrawable::Ptr		m_pNormalDraw;			///< �m�[�}�����̕\�����
	IDrawable::Ptr		m_pDisableDraw;			///< Disable���̕\�����
protected:

	/**
	 * [�擾] ��ގ擾.
	 *	@return ���
	 */
	EKind GetKind(void) const { return m_kind; }

	/**
	 * [�擾] �I���W�i���X�^�C���擾.
	 *	@return BUTTON�X�^�C��.
	 */
	LONG GetOriginalStyle(void) const { return m_originalStyle; }

	/**
	 * [�擾] �p�[�c�`��C���^�[�t�F�[�X�擾.
	 *	@return �p�[�c�`��C���^�[�t�F�[�X
	 */
	IPartsDrawable* ReferPartsDraw(void) { return m_pPartsDraw; }

	/**
	 * [�m�F] �{�^���^�C�v�擾
	 *	@retval true �v�b�V���{�^���^�C�v.
	 *	@retval false �ہB
	 */
	bool IsPushButtonStyle(void) const
	{
		return (m_kind == BUTTON || (m_originalStyle & BS_PUSHLIKE) != 0);
	}
	
	/**
	 * [�擾] ��s����.
	 *	@note �`�F�b�N�{�b�N�X�A���W�I�{�^���̏�t�����t���̈ʒu���v�Z���鎞�Ɏg�p����܂��B
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@return ����
	 */
	virtual int GetLineHeight(CDC* pDC)
	{
		return 16;
	}

	/**
	 * [����] �`��.
	 *	@param pDC �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnDrawButton(CDC* pDC)
	{
		if ( ! m_buttonBitmap.IsNull() )
		{
			m_buttonBitmap.Draw(*pDC);
			return;
		}
		MakeButtonImage(pDC);
		_super::KillTimer(TIMERID_DRAW);
		if ( m_lastBitmap.IsNull() || m_bitmapChangeTime == 0 )
		{
			m_buttonBitmap.Draw(*pDC);
			m_lastBitmap = m_buttonBitmap;
		}
		else
		{
			CBitmapImage bi;
			VERIFYLIB( bi.Set(m_buttonBitmap) );
			VERIFYLIB( bi.InsertOnAlphaBlend(0, 0, m_lastBitmap, 50) );//50%����
			bi.Draw(*pDC);
			m_lastBitmap = m_buttonBitmap;
			_super::SetTimer(TIMERID_DRAW, m_bitmapChangeTime, NULL);
		}
	}

	/**
	 * [����] �`��.
	 *	@param pDC �f�o�C�X�R���e�L�X�g�B
	 */
	void MakeButtonImage(CDC* pDC)
	{
		CBitmapImage bi;
		bi.Set(MFCLIB::GetParentImage(this, CLR_AUTOSELECT));
		CBitmapDC dc(&bi);
		dc.SetTextColor(pDC->GetTextColor());
		CWnd* pWnd = _super::GetParent();
		if ( pWnd != NULL )
		{
			pWnd->SendMessage(WM_CTLCOLORBTN,
				reinterpret_cast<WPARAM>(dc.GetSafeHdc()), reinterpret_cast<LPARAM>(m_hWnd));
		}
		const IDrawable* pDraw = m_pNormalDraw;
		UINT uState = 0;
		#ifdef DFCS_TRANSPARENT
			uState |= DFCS_TRANSPARENT;
		#endif
		switch ( _super::GetButtonState() )
		{
		case ES_Pushed:
			uState |= DFCS_PUSHED;
			break;
		case ES_Hover:
			#ifdef DFCS_HOT
				uState |= DFCS_HOT;
			#endif
			break;
		case ES_Disable:
			uState |= DFCS_INACTIVE;
			if ( ! m_pDisableDraw.IsNull() )
			{
				pDraw = m_pDisableDraw;
			}
			break;
		}
		if ( HasFocus() )
		{
			uState |= DFCS_FOCUS;
		}
		//
		bool isPushType = IsPushButtonStyle();
		if ( m_kind == BUTTON )
		{
			uState |= DFCS_BUTTONPUSH;
		}
		else
		{
			EKind kind = m_kind;
			if ( kind == CB3STATE && m_checkState == BST_CHECKED )
			{
				kind = CHECKBOX;
			}
			if ( (m_originalStyle & BS_PUSHLIKE) != 0)
			{
				uState |= DFCS_BUTTONPUSH;
			}
			else
			{
				uState |= kind;
			}
			if ( kind == CB3STATE )
			{
				if ( m_checkState == BST_INDETERMINATE )
				{
					uState |= DFCS_CHECKED;
				}
			}
			else if ( m_checkState == BST_CHECKED )
			{
				uState |= DFCS_CHECKED;
			}
		}
		#ifdef DFCS_FLAT
			if ( (m_originalStyle & BS_FLAT) != 0 )
			{
				uState |= DFCS_FLAT;
			}
		#endif
		CRect rc;
		_super::GetClientRect(rc);
		if ( isPushType )
		{
			m_pPartsDraw->DrawButtonControl(dc, rc, uState);
		}
		else
		{
			CRect r = rc;
			m_pPartsDraw->DrawButtonControl(NULL, r, uState);
			int l = (GetLineHeight(pDC) - r.Height() + 1) / 2;
			switch( m_originalStyle & 0x0C00 )
			{
			case BS_TOP:/*0x00000400L*/
				r.OffsetRect(0, l);
				break;
			case BS_BOTTOM:/*0x00000800L*/
				r.OffsetRect(0, rc.Height() - r.bottom - l);
				break;
			default:
			case BS_VCENTER:/*0x00000C00L*/	
				r.OffsetRect(0, (rc.Height() - r.bottom) / 2);
				break;
			}
			if ( (m_originalStyle & BS_LEFTTEXT) != 0 || (m_originalExStyle & WS_EX_RIGHT) != 0 )
			{
				//����
				r.left = rc.right - r.right;
				r.right = rc.right;
				m_pPartsDraw->DrawButtonControl(dc, r, uState);
				rc.right -= r.Width() + 3;
			}
			else
			{
				//�E��
				m_pPartsDraw->DrawButtonControl(dc, r, uState);
				rc.left += r.Width() + 3;
			}
		}
		if ( pDraw != NULL )
		{
			SIZE size;
			pDraw->GetSize(size);
			int x = 0;
			int y = (rc.Height() - size.cy) / 2;
			if ( isPushType )
			{
				x = (rc.Width() - size.cx) / 2;
				if ( _super::GetButtonState() == ES_Pushed )
				{
					POINT po;
					m_pPartsDraw->GetPushedOffset(po);
					rc.OffsetRect(po);
				}
			}
			else if ( (m_originalExStyle & WS_EX_RIGHT) != 0 )
			{
				x = rc.Width() - size.cx;
			}
			pDraw->Draw(dc, rc.left + x, rc.top + y);
			//
			if ( isPushType && m_kind == CB3STATE && m_checkState == BST_INDETERMINATE )
			{
				CRect rc;
				_super::GetClientRect(rc);
				CBitmapImage bi;
				bi.Set(rc.Width(), rc.Height());
				{
					CBitmapDC d(&bi);
					m_pPartsDraw->DrawButtonControl(d, rc, uState);
				}
				bi.AlphaBlend(dc, 0, 0, 50);
			}
			//
			if ( HasFocus() )
			{
				if ( isPushType )
				{
					POINT po;
					m_pPartsDraw->GetEdgeOffset(po);
					rc.DeflateRect(po.x + 1, po.y + 1);
					if ( _super::GetButtonState() == ES_Pushed )
					{
						CPoint po;
						m_pPartsDraw->GetPushedOffset(po);
						rc.OffsetRect(-po);
					}
				}
				else
				{
					rc.left += x;
					rc.top += y;
					rc.right = rc.left + size.cx;
					rc.bottom = rc.top + size.cy;
				}
				m_pPartsDraw->DrawFocusRect(dc, &rc);
			}
		}
		CBitmapImage* P = dc.Detach();
		m_buttonBitmap = P->GetBitmapHandle();
	}

	/**
	 * [�ʒm] �N���b�N.
	 *		BN_CLICKED ��e�ɒʒm����O�ɃR�[������܂��B
	 */
	virtual void OnClickButton(void)
	{
		if ( m_boIsAuto )
		{
			int c = BST_CHECKED;
			if ( m_kind != RADIOBUTTON )
			{
				switch( m_checkState )
				{
				default:
					ASSERT( false );
					//�ȉ��֑���
				case BST_UNCHECKED:
					c = BST_CHECKED;
					break;
				case BST_CHECKED:
					c = (m_kind == CB3STATE) ? BST_INDETERMINATE : BST_UNCHECKED;
					break;
				case BST_INDETERMINATE:
					c = BST_UNCHECKED;
					break;
				};
			}
			m_SetCheck(c);
		}
	}

	/**
	 * [�ʒm] ��ԕω��ʒm.
	 *	@note ���݂̏�Ԃ��擾���܂�
	 *	@param state ��ԁB
	 */
	virtual void OnChangeState(EState state)
	{
		m_buttonBitmap.Null();
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
		switch( message )
		{
		case WM_TIMER:
			if ( wParam == TIMERID_DRAW )
			{
				KillTimer(TIMERID_DRAW);
				CClientDC dc(this);
				m_lastBitmap.Draw(dc);
			}
			break;
		case WM_GETDLGCODE:
			if ( m_kind == RADIOBUTTON )
			{
				return DLGC_RADIOBUTTON | DLGC_BUTTON;
			}
			else if ( m_kind != BUTTON )
			{
				return DLGC_BUTTON;
			}
			break;
		case BM_GETSTATE:
			return _super::WindowProc(message, wParam, lParam) | m_checkState;
		case BM_GETCHECK:
			return m_checkState;
		case BM_SETCHECK:
			m_SetCheck(ToInt(wParam));
			return 0;
		case BM_SETSTYLE:
			if ( lParam != 0 ) { RedrawWindow(); }
			return 0;
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
		m_checkState = BST_UNCHECKED;
		m_originalStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		m_originalExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		m_boIsAuto = false;
		bool canSendDblClk = ((m_originalStyle & BS_NOTIFY) != 0);
		switch ( m_originalStyle & BS_TYPEMASK )
		{
		case BS_AUTOCHECKBOX:
			m_boIsAuto = true;
			m_kind = CHECKBOX;	//�`�F�b�N�{�b�N�X
			break;
		case BS_CHECKBOX:
			m_kind = CHECKBOX;	//�`�F�b�N�{�b�N�X
			break;
		case BS_AUTO3STATE:
			m_boIsAuto = true;
			m_kind = CB3STATE;	//3�X�e�[�g�`�F�b�N�{�b�N�X
			break;
		case BS_3STATE:
			m_kind = CB3STATE;	//3�X�e�[�g�`�F�b�N�{�b�N�X
			break;
		case BS_AUTORADIOBUTTON:
			m_boIsAuto = true;
			m_kind = RADIOBUTTON;	//���W�I�{�^��
			break;
		case BS_RADIOBUTTON:
			canSendDblClk = true;
			m_kind = RADIOBUTTON;	//���W�I�{�^��
			break;
		case BS_DEFPUSHBUTTON:
			{
				CWnd* P = _super::GetParent();
				if ( P != NULL ) { P->SendMessage(DM_SETDEFID, _super::GetDlgCtrlID(), 0); }
			}
			m_kind = BUTTON;		//���ʂ̃{�^��
			break;
		default:
			m_kind = BUTTON;		//���ʂ̃{�^��
			break;
		}
		_super::SetSendDblClkMode(canSendDblClk);
		_super::PreSubclassWindow();
		m_buttonBitmap.Null();
	}

public:

	/// �R���X�g���N�^
	CDrawingButton(void) : _super(), m_pPartsDraw(NULL), m_bitmapChangeTime(50)
	{
		SetPartsDrawer(NULL);
	}

	/**
	 * [�ݒ�] �p�[�c�`��w��.
	 *	@param pPartsDraw �p�[�c�`��B�Q�Ƃ̂݋L�����܂��B�j�����Ȃ��ł��������B
	 *					���w�莞��A NULL �w�莞�́A�W���̃R���g���[����\�����܂��B
	 *	@note �{�N���X�ł́A IPartsDrawable �� GetPushedOffset() , DrawFrameControl() , DrawFocusRect() ���g�p���܂��B
	 */
	void SetPartsDrawer(IPartsDrawable* pPartsDraw = NULL)
	{
		if ( pPartsDraw != NULL )
		{
			m_pPartsDraw = pPartsDraw;
		}
		else
		{
			static CStandardPartsDrawer s_partsDrawer;
			m_pPartsDraw = &s_partsDrawer;
		}
	}

	/**
	 * [�ݒ�] �r�b�g�}�b�v�؂�ւ����Ԑݒ�.
	 *		��Ԃ��ς���ăr�b�g�}�b�v��ύX���鎞�A�f�t�H���g�ł́A�؂�ւ��O�̃r�b�g�}�b�v�ƁA
	 *		�؂�ւ���̃r�b�g�}�b�v��50�������̉摜�� 30ms �b�\������悤�ɂȂ��Ă��܂��i���̂���
	 *		�ӂ���Ɛ؂�ւ��悤�Ɍ����܂��j�B���̐؂�ւ����Ԃ�{���\�b�h�Ŏw�肷�邱�Ƃ��o���܂��B
	 *	@note �����摜�͈ꖇ�����p�ӂ��Ȃ����߁A100ms���炢������ł��B
	 *	@param ms �؂�ւ����ԁi�P�� ms�j�B
	 */
	void SetBitmapChangeTime(UINT ms)
	{
		m_bitmapChangeTime = ms;
	}

	/**
	 * [�ݒ�] �ʏ펞�̕`����w��.
	 *	@param draw �`����B�N���[�����L�����܂��̂ŁA�j�����Ă����܂��܂���B
	 */
	void SetNormalDrawer(const IDrawable& draw)
	{
		m_pNormalDraw = draw.Clone();
		if ( ::IsWindow(*this) )
		{
			m_buttonBitmap.Null();
		}
	}

	/**
	 * [�ݒ�] Disable���̕`����w��.
	 *	@param draw �`����B�N���[�����L�����܂��̂ŁA�j�����Ă����܂��܂���B
	 */
	void SetDisableDrawer(const IDrawable& draw)
	{
		m_pDisableDraw = draw.Clone();
		if ( ::IsWindow(*this) )
		{
			m_buttonBitmap.Null();
		}
	}

	/**
	 * [�ݒ�] �T�C�Y����.
	 *		�R���g���[���̑傫�������݂̐ݒ�ɓK����(�ŏ���)�傫���ɕύX���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AdjustSize(void)
	{
		if ( m_kind == BUTTON || (m_originalStyle & BS_PUSHLIKE) != 0 )
		{
			if ( ! m_pNormalDraw.IsNull() )
			{
				SIZE sz;
				m_pNormalDraw->GetSize(sz);
				POINT po;
				m_pPartsDraw->GetEdgeOffset(po);
				return MFCLIB::ChangeClientSize(this, sz.cx + po.x * 2, sz.cy + po.y * 2);
			}
			return true; //�����Ȃ�
		}
		//
		UINT uState = 0;
		switch ( m_kind )
		{
		case RADIOBUTTON:	uState = DFCS_BUTTONRADIO;	break;
		case CHECKBOX:		uState = DFCS_BUTTONCHECK;	break;
		case CB3STATE:		uState = DFCS_BUTTON3STATE;	break;
		default:	
			ASSERTLIB( false );	
			break;
		}
		CRect rc;
		_super::GetClientRect(rc);
		m_pPartsDraw->DrawButtonControl(NULL, rc, uState);
		SIZE size = { 0, 0 };
		if ( ! m_pNormalDraw.IsNull() )
		{
			m_pNormalDraw->GetSize(size);
		}
		size.cx += rc.Width() + 3;
		size.cy = max(size.cy, rc.Height());
		return MFCLIB::ChangeClientSize(this, size.cx, size.cy);
	}

	/**
	 * [�擾] �`�F�b�N��Ԏ擾.
	 *	@retval BST_UNCHECKED		�{�^�����I�t�̏��
	 *	@retval BST_CHECKED			�{�^�����I���̏��
	 *	@retval BST_INDETERMINATE	�{�^���̏�Ԃ��s�m�� (�{�^���� BS_3STATE �X�^�C���܂��� BS_AUTO3STATE �X�^�C�����ݒ肳��Ă���Ƃ�����)
 	 */
	int GetCheck(void) const
	{
		return static_cast<int>(::SendMessage(m_hWnd, BM_GETCHECK, 0, 0)); 
	}
	
	/**
	 * [�ݒ�] �`�F�b�N��Ԑݒ�.
	 *	@param nCheck �ȉ��̃p�����[�^���w��ł��܂��B\n
	 *		<table><tr><td>
	 *		�p�����[�^							</td><td>
	 *					����					</td></tr><tr><td>
	 *		BST_UNCHECKED						</td><td>
	 *					�{�^�����I�t�̏��		</td></tr><tr><td>
	 *		BST_CHECKED							</td><td>
	 *					�{�^�����I���̏��		</td></tr><tr><td>
	 *		BST_INDETERMINATE					</td><td>
	 *					�{�^���̏�Ԃ��s�m��	</td></tr>
	 *		</table>
 	 */
	void SetCheck(int nCheck)
	{
		::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0); 
	}
};



/**@ingroup MFCCONTROL DRAWABLE
 * �`����{�^���R���g���[��
 *
 *		inline �ȃR���g���[���ł��B
 *
 *		���\�[�X�G�f�B�^�Ń{�^�������W�I�{�^�����`�F�b�N�{�b�N�X�R���g���[���𒣂�A�N���X�Ɋ��蓖�Č�A
 *		�{�N���X�ɒu�������Ďg�p���܂��B
 *
 *		���\�[�X�ŏ����ꂽ�������\�����܂��B
 *
 *	@note ���\�[�X�G�f�B�^�ł̐ݒ�̂܂܂̃{�^���R���g���[���ɂȂ�܂��B
 *
 *	@note SetPartsDrawer() , SetNormalDrawer() , SetDisableDrawer() ���w�肵�Ȃ��ƁA
 *		���߂��邾���̂����̕��ʂ̃{�^���R���g���[���ɂȂ�܂��B
 *
 *	@see CAbstractButton
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingButton.h
 * 
 *	@date 07/11/24 �V�K�쐬
 */
class CDrawingButtonTx : public CDrawingButton
{
	DEFSUPER(CDrawingButton);
protected:
	COLORREF		m_color;	///< �����̐F
	CWndTextDrawer*	m_pText;	///< �����\��
	/// �����\���ݒ�
	void SetTextDrawer(CWndTextDrawer* D)
	{
		if ( m_pText != NULL )
		{
			delete m_pText;
			m_pText = NULL;
		}
		m_pText = D;
		if ( m_pText != NULL )
		{
			m_pText->SetTextStyle(CTextDrawer::ButtonToDrawStyle(_super::GetOriginalStyle()));
			SetTextColor(m_color);
		}
	}
	/**
	 * [�擾] ��s����.
	 *	@note �`�F�b�N�{�b�N�X�A���W�I�{�^���̏�t�����t���̈ʒu���v�Z���鎞�Ɏg�p����܂��B
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@return ����
	 */
	virtual int GetLineHeight(CDC* pDC)
	{
		CFont* pFont = _super::GetFont();
		if ( pFont != NULL )
		{
			CFont* old = pDC->SelectObject(pFont);
			TEXTMETRIC t;
			pDC->GetTextMetrics(&t);
			pDC->SelectObject(old);
			return t.tmHeight + t.tmExternalLeading * 2;
		}
		return 16; //�K��
	}

	/**
	 * [�m�F] �t�H�[�J�X�̗L��
	 *	@retval true �t�H�[�J�X����B
	 *	@retval false �t�H�[�J�X�Ȃ��B
	 */
	virtual bool HasFocus(void) const
	{
		return _super::IsPushButtonStyle() ? _super::HasFocus() : false;
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		CRect rect;
		_super::GetClientRect(&rect);
		CWndTextDrawer* pText = NULL;
		if ( ! _super::IsPushButtonStyle() )
		{
			IPartsDrawable* pParts = _super::ReferPartsDraw();
			CRect r = rect;
			pParts->DrawButtonControl(NULL, r, _super::GetKind());
			rect.right -= r.Width() + 3;
			pText = new CWndTextDrawer(rect.Size(), *this, m_color, 0, 2, 2, pParts);
		}
		else
		{
			POINT po;
			_super::ReferPartsDraw()->GetEdgeOffset(po);
			pText = new CWndTextDrawer(rect.Size(), *this, m_color, 0, po.x + 2, po.y + 2, NULL);
		}
		SetTextDrawer(pText);
	}
public:

	/// �R���X�g���N�^
	CDrawingButtonTx(void) : m_color(CLR_AUTOSELECT), m_pText(NULL)
	{
	}

	/// �f�X�g���N�^
	~CDrawingButtonTx(void)
	{
		if ( m_pText != NULL )
		{
			delete m_pText;
			m_pText = NULL;
		}
	}

	/**
	 * [�ݒ�] �e�L�X�g�J���[�ݒ�
	 *	@note �f�t�H���g�ł́A�f�o�C�X�R���e�L�X�g�ɐݒ肳��Ă���F���g���܂�.
	 *	@param color �e�L�X�g�J���[.
	 */
	void SetTextColor(COLORREF color)
	{
		m_color = color;
		if ( m_pText != NULL )
		{
			m_pText->SetTextColor(m_color);
			_super::SetNormalDrawer(*m_pText);
			m_pText->SetTextColor(CLR_INVALID);
			_super::SetDisableDrawer(*m_pText);
			_super::Invalidate();
		}
	}

	/**
	 * [�ݒ�] �T�C�Y����.
	 *		�R���g���[���̑傫�������݂̐ݒ�ɓK����(�ŏ���)�傫���ɕύX���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AdjustSize(void)
	{
		if ( m_pText != NULL )
		{
			CRect rc;
			GetClientRect(&rc);
			// ������̕`��傫���v�Z
			CString s;
			_super::GetWindowText(s);
			DWORD style = CTextDrawer::ButtonToDrawStyle(_super::GetOriginalStyle());
			CDC* pDC = GetDC();
			CGdiObject* old = pDC->SelectObject(_super::GetFont());
			CSize textSize = rc.Size();
			CTextDrawer::CalcTextSize(textSize, *pDC, style, s);
			pDC->SelectObject(old);
			ReleaseDC(pDC);
			textSize.cx += 3;
			textSize.cy += 4;
			// �`����X�V
			CWndTextDrawer* pText = NULL;
			if ( ! _super::IsPushButtonStyle() )
			{
				IPartsDrawable* pParts = _super::ReferPartsDraw();
				pText = new CWndTextDrawer(textSize, *this, m_color, 0, 2, 2, pParts);
			}
			else
			{
				POINT po;
				_super::ReferPartsDraw()->GetEdgeOffset(po);
				pText = new CWndTextDrawer(textSize, *this, m_color, 0, po.x + 2, po.y + 2, NULL);
			}
			SetTextDrawer(pText);
			return _super::AdjustSize();
		}
		return false;
	}
};



}; //MFC
}; //TNB



