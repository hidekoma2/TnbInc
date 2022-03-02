#pragma once
/**
 *@file
 * ���T�C�Y�_�C�A���O�֌W�̃w�b�_
 *
 *		���T�C�Y�@�\�����_�C�A���O�̃x�[�X��񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



/// MFC static �����N�΍�
#define classCResizeDialog classCDialog



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ���T�C�Y�_�C�A���O�x�[�X�N���X
 *
 *		inline �ȃR���g���[���ł��B
 *
 *		�{�N���X�� CDialog �̑���Ɍp������ƁA�T�C�Y�ύX�\�ȃ_�C�A���O�ɂȂ�܂��B
 *
 *		CDialog ���g�����_�C�A���O�����v�̂Ń��\�[�X���쐬���N���X������A CDialog
 *		��{�N���X�ɒu��������`�̎������@��z�肵�Ă��܂��B
 *
 *		�_�C�A���O�̃��\�[�X�́A�u���E��(Border)�v���u�T�C�Y�ύX�g�v�ɂ��A�u�����X�N���[���o�[(Horizontal Scrollbar)�v
 *		�u�����X�N���[���o�[(Vertical Scrollbar)�v�Ƀ`�F�b�N��t���Ȃ�(false�ɂ���)�悤�ɂ��܂��B
 *
 *		�܂��A OnPaint�ŕ`�悵�Ă�����̂́A CPaintOrgDC ���g�� DC ���擾���Ă��������B
 *
 *		��́A���ʂ̃_�C�A���O�Ɠ����ł��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		//= �錾(�w�b�_�t�@�C��)
 *			class CXxxDlg : public CResizeDialog	// �� CDialog �� CResizeDialog �ɕύX�B		
 *					// ���� �uCDialog�v �����l
 *			{
 *					:
 *				virtual void OnPrintClient(CDC* pDC);	// WM_PRINT �ɑΉ�����ꍇ�A�ǉ�
 *					:
 *
 *		//= �ݒ�(�\�[�X�t�@�C��) WM_PRINT �ɑΉ�����ꍇ
 *			
 *			void CXxxDlg::OnPaint() 
 *			{
 *				CPaintOrgDC dc(this);	// �� �f�o�C�X�R���e�L�X�g�̎擾
 *				OnPrintClient(&dc);
 *			}
 *			void CXxxDlg::OnPrintClient(CDC* pDC)
 *			{
 *				m_bmps.Draw(*pDC);	// ��͕��ʂ�
 *			}
 *				:
 *
 *		//= �ݒ�(�\�[�X�t�@�C��) WM_PRINT �ɑΉ����Ȃ��ꍇ
 *			
 *			void CXxxDlg::OnPaint() 
 *			{
 *				CPaintOrgDC dc(this);	// �� �f�o�C�X�R���e�L�X�g�̎擾
 *				m_bmps.Draw(dc);	// ��͕��ʂ�
 *			}
 *				:
 *
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcResizeDialog.h
 * 
 *	@date 07/10/18 �V�K�쐬
 *	@date 07/10/22 �X�N���[���֌W���b�Z�[�W�A�N����������悤�ɏC���B
 *	@date 07/10/24 �X�N���[���֌W���b�Z�[�W�A�C���B�z�C�[���ɂ��Ή��B GetOriginalSize() �ǉ��B
 *	@date 07/10/26 WM_PAINT �������C���B �e���v���[�g����߁ACDialog�p���ցB
 *	@date 07/10/27 OnOffsetPaint��p�~�B �c�X�N���[���o�[���o�Ȃ��o�O���C���B
 *	@date 07/10/29 SetScrollOrg() ��ǉ��B
 *	@date 07/10/31 WM_PRINT , WM_PRINTCLIENT �ɑΉ��B OnPrintClient() �ǉ��B
 *	@date 07/11/02 SCROLL�o�[���o�Ȃ����Ƃ��܂�ɂ������B�����I�ɕ\�� true ����悤�ɂ����B
 *	@date 07/11/06 SetOriginalSize() �̈����� Window �T�C�Y�p�������B Client �T�C�Y�w�胁�\�b�h���V�K�B
 *	@date 08/01/29 �t���[���A�e�[�}�ύX�ɑΉ��B
 *	@date 08/02/01 �X�N���[���o�[���Ȃ��̂݁A WM_MOUSEWHELL �X���[����悤�ɂ����B
 *				   SetScrollOrg() �ɑ������ǉ��B CPaintOrgDC �N���X��ǉ��B
 *	@date 08/04/11 �N���C�A���g���X�N���[���o�[��菬�����Ȃ�����X�N���[���o�[��HIDE�ɂ���悤�ɂ����B
 */
class CResizeDialog : public CDialog
{
	DEFSUPER(CDialog);
	enum { DEFAULT_LINESTEP = 20 };
public:

	/**@ingroup MFCCONTROL
	 * ���T�C�Y�_�C�A���O�p PaintDC �N���X
	 *
	 *	@see CResizeDialog
	 *	@par�K�v�t�@�C��
	 *			TnbMfcResizeDialog.h
	 * 
	 *	@date 08/02/01 �V�K�쐬
	 */
	class CPaintOrgDC : public CPaintDC
	{
		DEFSUPER(CPaintDC);
		CResizeDialog*	m_pResizeDlg;
	public:
		/**
		 * �R���X�g���N�^
		 *	@param pWnd CResizeDialog�̃C���X�^���X
		 */
		explicit CPaintOrgDC(CResizeDialog* pWnd) : _super(pWnd), m_pResizeDlg(pWnd)
		{
			m_pResizeDlg->SetScrollOrg(this, true);
		}
		/// �f�X�g���N�^
		~CPaintOrgDC(void)
		{
			m_pResizeDlg->SetScrollOrg(this, false);
		}
	};

	/// �R���X�g���N�^
	CResizeDialog(void) 
		: _super(), m_originalSize(0, 0), m_lineStep(DEFAULT_LINESTEP)
		, m_onThemeChanged(false), m_canSizeDisp(true)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param lpszTemplateName �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X����\�� null �ŏI��镶����B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	CResizeDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
		: _super(lpszTemplateName, pParentWnd), m_originalSize(0, 0), m_lineStep(DEFAULT_LINESTEP)
		, m_onThemeChanged(false), m_canSizeDisp(true)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param nIDTemplate �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X�� ID �ԍ��B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	CResizeDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) 
		: _super(nIDTemplate, pParentWnd), m_originalSize(0, 0), m_lineStep(DEFAULT_LINESTEP)
		, m_onThemeChanged(false), m_canSizeDisp(true)
	{
	}

	/**
	 * [�擾] �X�N���[���ʒu.
	 *	@return ����[�ɕ\������Ă���N���C�A���g�̍��W�B
	 */
	CPoint GetScrollPoint(void) const
	{
		return CPoint(m_horizontalParam.GetPos(), m_verticalParam.GetPos());
	}

	/**
	 * [�ݒ�] �f�o�C�X�R���e�L�X�g�̌��_�ݒ�A����.
	 *		�f�o�C�X �R���e�L�X�g�̃E�B���h�E�̌��_���X�N���[���ʒu�ɂ��킹�܂��B
	 *	@note �ݒ�Ɖ������Z�b�g�Ŏg�p���܂��B
	 *	@param pDc �f�o�C�X�R���e�L�X�g�B
	 *	@param isBegin �ȗ�����ƌ��_�ݒ肵�܂��B false ���w�肷��Ɖ������܂��B
	 */
	void SetScrollOrg(CDC* pDc, bool isBegin = true) const
	{
		int cx = m_horizontalParam.GetPos();
		int cy = m_verticalParam.GetPos();
		if ( isBegin )
		{
			pDc->OffsetWindowOrg(cx, cy);
		}
		else
		{
			pDc->OffsetWindowOrg(-cx, -cy);
		}
	}

	/**
	 * [�擾] �I���W�i���E�B���h�E�T�C�Y�擾.
	 *		�I���W�i���̃T�C�Y�i�E�B���h�E�̍ő�T�C�Y�j�𓾂邱�Ƃ��o���܂��B
	 *	@note	�T�C�Y�ύX���Ă����Ԃł́AGetWindowRect() �ŁA�E�B���h�E�̖{���̑傫����m�邱�Ƃ��o���܂���B
	 *			�{���\�b�h�́A�T�C�Y�ύX���Ă��Ă��A�I���W�i���̑傫���𓾂邱�Ƃ��o���܂��B
	 *	@return �T�C�Y.
	 */
	CSize GetOriginalWindowSize(void) const
	{
		return GetOriginalSize() + m_offsetWinToClient;
	}

	/**
	 * [�擾] �I���W�i���N���C�A���g�T�C�Y�擾.
	 *		�I���W�i���̃T�C�Y�i�N���C�A���g�̍ő�T�C�Y�j�𓾂邱�Ƃ��o���܂��B
	 *	@note	�T�C�Y�ύX���Ă����Ԃł́AGetClientRect() �ŁA�N���C�A���g�̖{���̑傫����m�邱�Ƃ��o���܂���B
	 *			�{���\�b�h�́A�T�C�Y�ύX���Ă��Ă��A�I���W�i���̑傫���𓾂邱�Ƃ��o���܂��B
	 *	@return �T�C�Y.
	 */
	const CSize& GetOriginalSize(void) const
	{
		return m_originalSize;
	}

	/**
	 * [�ݒ�] �I���W�i���E�B���h�E�T�C�Y�ݒ�.
	 *		�I���W�i���̃T�C�Y�i�E�B���h�E�̍ő�T�C�Y�j��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note	�ʏ�A���\�[�X���玩���I�ɃT�C�Y�𓾂܂��B
	 *	@param size �T�C�Y.
	 */
	void SetOriginalWindowSize(const SIZE& size)
	{
		m_CheckOffset();
		UINT r = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
		m_canSizeDisp = false;
		_super::SetWindowPos(NULL, 0, 0, size.cx, size.cy, r);
		m_canSizeDisp = true;
		CRect rc;
		GetClientRect(&rc);
		m_originalSize = rc.Size();
		m_InitScrollInfo();
	}

	/**
	 * [�ݒ�] �I���W�i���N���C�A���g�T�C�Y�ݒ�.
	 *		�I���W�i���̃T�C�Y�i�N���C�A���g�̍ő�T�C�Y�j��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note	�ʏ�A���\�[�X���玩���I�ɃT�C�Y�𓾂܂��B
	 *	@param size �T�C�Y.
	 */
	void SetOriginalSize(const SIZE& size)
	{
		m_CheckOffset();
		m_originalSize = size;
		CSize s = size;
		s += m_offsetWinToClient;
		UINT r = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
		m_canSizeDisp = false;
		_super::SetWindowPos(NULL, 0, 0, s.cx, s.cy, r);
		m_canSizeDisp = true;
		m_InitScrollInfo();
	}

	/**
	 * [�ݒ�] �I���W�i���N���C�A���g�T�C�Y�ύX.
	 *		�I���W�i���̃T�C�Y�i�N���C�A���g�̍ő�T�C�Y�j��ύX���邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y.
	 */
	void ChangeOriginalSize(const SIZE& size)
	{
		int dx = 0;
		int dy = 0;
		if ( m_horizontalParam.GetMax() < size.cx )
		{
			dx -= size.cx - m_horizontalParam.GetMax();
		}
		if ( m_verticalParam.GetMax() < size.cy )
		{
			dy -= size.cy - m_verticalParam.GetMax();
		}
		m_OnHScroll(SB_LEFT, 0, NULL);
		m_OnVScroll(SB_TOP, 0, NULL);
		m_horizontalParam.ShowBar(*this, SB_HORZ, false);
		m_verticalParam.ShowBar(*this, SB_VERT, false);
		//
		m_CheckOffset();
		m_originalSize = size;
		CSize s = size;
		s += m_offsetWinToClient;
		UINT r = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
		_super::SetWindowPos(NULL, 0, 0, s.cx, s.cy, r);
		m_InitScrollInfo();
		//
		_super::ScrollWindow(dx, 0);
		_super::ScrollWindow(0, dy);
	}

	/**
	 * [�ݒ�] ���C���X�N���[���ʐݒ�.
	 *		�X�N���[���o�[�̖��{�^�������������ɃX�N���[������ʂ��w�肵�܂��B
	 *	@note �f�t�H���g�ł́A20�ł��B
	 *	@param s �X�N���[����.
	 */
	void SetLineStep(int s)
	{
		m_lineStep = s;
	}

	/**
	 * [�ݒ�] �I���W�i���E�B���h�E�T�C�Y��.
	 *		�E�B���h�E�T�C�Y���I���W�i���߂��܂�
	 */
	void ResetWindowsSize(void)
	{
		m_OnHScroll(SB_LEFT, 0, NULL);
		m_OnVScroll(SB_TOP, 0, NULL);
		SIZE sz;
		SetOriginalWindowSize(sz);
		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	/**
	 * [�ʒm] WM_PRINTCLIENT ���b�Z�[�W��M.
	 *		WM_PRINTCLIENT ���b�Z�[�W��M������R�[������܂��B
	 *	@note �ʏ�AOnPaint() �ŕ`�悵�Ă�����e�Ɠ������̂�`���܂��B
	 *	@note SetScrollOrg() �͎g�p���܂���B
	 *	@param pDC �f�o�C�X�R���e�L�X�g�B
	 */	
	virtual void OnPrintClient(CDC* pDC)
	{
	}

protected:
	/// Win��Client�̍����v�Z
	void m_CheckOffset(void)
	{
		if ( m_originalSize.cx == 0 )
		{
			CRect cliRc;
			_super::GetClientRect(&cliRc);
			CRect winRc;
			_super::GetWindowRect(&winRc);
			m_originalSize = cliRc.Size();
			m_offsetWinToClient = winRc.Size() - cliRc.Size();
		}
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
		LRESULT r = 0;
		switch ( message )
		{
		case WM_NCLBUTTONDBLCLK:
			TTRACE0("WM_NCLBUTTONDBLCLK\n");
			if ( wParam == HTCAPTION )
			{
				CSize s = GetOriginalWindowSize();
				_super::SetWindowPos(NULL, 0, 0, s.cx, s.cy, SWP_NOMOVE | SWP_NOZORDER);
				return 0;
			}
			break;
		case WM_SIZE:
			TTRACE0("WM_SIZE\n");
			r = _super::WindowProc(message, wParam, lParam);
			if ( m_canSizeDisp )
			{
				m_OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
			}
			return r;
		case WM_GETMINMAXINFO:
			TTRACE0("WM_GETMINMAXINFO\n");
			m_CheckOffset();
			if ( m_originalSize.cx != 0 )
			{
				LPMINMAXINFO P = reinterpret_cast<LPMINMAXINFO>(lParam);
				P->ptMaxTrackSize.x = m_originalSize.cx + m_offsetWinToClient.cx;
				P->ptMaxTrackSize.y = m_originalSize.cy + m_offsetWinToClient.cy;
			}
			break;
		case WM_HSCROLL:
			TTRACE2("WM_HSCROLL %d 0x%08X\n", LOWORD(wParam), lParam);
			m_OnHScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
			break;
		case WM_VSCROLL:
			TTRACE2("WM_VSCROLL %d 0x%08X\n", LOWORD(wParam), lParam);
			m_OnVScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
			break;
		case WM_MOUSEWHEEL:
			TTRACE0("WM_MOUSEWHEEL\n");
			if ( m_OnMouseWheel(LOWORD(wParam), static_cast<short>(HIWORD(wParam)), lParam) )
			{
				return TRUE;
			}
			break;
		case WM_PRINT:
			TTRACE1("WM_PRINT 0x%08X\n", lParam);
			r = _super::WindowProc(message, wParam, lParam & ~(PRF_OWNED | PRF_CHILDREN));
			if ( (lParam & PRF_CHILDREN) != 0 )
			{
				CRect rectWin;
				_super::GetWindowRect(rectWin);
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				CPoint orgOrg = pDC->GetWindowOrg();
				CWnd* pWnd = CWnd::GetWindow(GW_CHILD);
				while ( pWnd != NULL )
				{
					CRect rect;
					pWnd->GetWindowRect(rect);
					CSize po = rect.TopLeft() - rectWin.TopLeft();
					pDC->SetWindowOrg(orgOrg - po);
					pWnd->SendMessage(WM_PRINT, wParam, lParam | PRF_CHECKVISIBLE);
					pWnd = pWnd->GetNextWindow();
				}
				pDC->SetWindowOrg(orgOrg);
				if ( (lParam & PRF_NONCLIENT) != 0 )
				{
					SendMessage(message, wParam, PRF_NONCLIENT);
				}
			}
			return r;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				SetScrollOrg(pDC);
				OnPrintClient(pDC);
				SetScrollOrg(pDC, false);
			}
			break;
		case WM_INITDIALOG:
			TTRACE0("WM_INITDIALOG\n");
			r = _super::WindowProc(message, wParam, lParam);
			m_InitScrollInfo();
			return r;
		case WM_THEMECHANGED:
			TTRACE0("WM_THEMECHANGED\n");
			m_onThemeChanged = true;
			break;
		case WM_EXITSIZEMOVE:
			TTRACE0("WM_EXITSIZEMOVE\n");
			m_onThemeChanged = false;
			break;
		case WM_WINDOWPOSCHANGED:
			TTRACE0("WM_WINDOWPOSCHANGED\n");
			{
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				WINDOWPOS* W = reinterpret_cast<WINDOWPOS*>(lParam);
				if ( m_onThemeChanged && (W->flags & SWP_FRAMECHANGED) != 0 )
				{
					CRect cliRc;
					_super::GetClientRect(&cliRc);
					CRect winRc;
					_super::GetWindowRect(&winRc);
					CSize sz = m_offsetWinToClient;
					m_offsetWinToClient = winRc.Size() - cliRc.Size();
					if ( m_horizontalParam.IsEnable() )
					{
						m_offsetWinToClient.cx -= ::GetSystemMetrics(SM_CYHSCROLL);
					}
					if ( m_verticalParam.IsEnable() )
					{
						m_offsetWinToClient.cy -= ::GetSystemMetrics(SM_CXHSCROLL);
					}
					m_OnSize(0, cliRc.right, cliRc.bottom);
				}
				return r;
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// �p�����[�^
	class CBarParam
	{
		int		m_pos;				///< ���݈ʒu
		int		m_max;				///< �l�`�w�ʒu
		int		m_page;				///< �y�[�W�T�C�Y
		bool	m_boIsEnable;			///< �R���g���[�����(true�Ȃ�Enable)
	public:
		/// �R���X�g���N�^
		CBarParam(void) : m_boIsEnable(false), m_pos(0), m_max(0), m_page(0) { }
		/// �|�W�V�����擾
		int GetPos(void) const { return m_boIsEnable ? m_pos : 0; }
		/// MAX�擾
		int GetMax(void) const { return m_max; }
		/// �R���g���[����Ԏ擾 (true�Ȃ�Enable)
		bool IsEnable(void) const { return m_boIsEnable; }
		/// MAX�ݒ�
		void SetMax(int max)
		{
			m_pos = 0;
			m_max = max;
			m_page = max;
		}
		/// �y�[�W�Z�b�g�B�߂�Ƃ��Č��݂̈ʒu����̃f���^�B
		int SetPage(int pa)
		{
			m_page = pa;
			int delta = 0;
			if ( m_pos + m_page > m_max )
			{
				delta = m_pos - (m_max - m_page);
				m_pos = m_max - m_page;
			}
			return delta;
		}
		/// �o�[�\���E��\��
		void ShowBar(HWND h, int nBar, bool isEnable)
		{
			m_boIsEnable = isEnable;
			if ( m_boIsEnable )
			{
				SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL, 0, m_max - 1, m_page, m_pos, 0 };
				ASSERTLIB( si.cbSize == sizeof(SCROLLINFO) && si.nPos == m_pos );
				::SetScrollInfo(h, nBar, &si, TRUE);
				::ShowScrollBar(h, nBar, TRUE);
			}
			else
			{
				::ShowScrollBar(h, nBar, FALSE);
//				::InvalidateRect(h, NULL, false);
			}
		}
		/// �X�N���[���B�߂�l�̓f���^
		int Scroll(UINT nSBCode, UINT nPos, int lineStep) const
		{
			int delta = 0;
			switch ( nSBCode )
			{
			case SB_LEFT: // SB_TOP
				delta = -m_pos;
				break;
			case SB_LINERIGHT: // SB_LINEDOWN
			case SB_PAGERIGHT: // SB_PAGEDOWN
				if ( m_pos + m_page < m_max )
				{
					int r = (nSBCode == SB_LINERIGHT) ? lineStep : m_page;
					delta = min(r, m_max - (m_pos + m_page));
				}
				break;
			case SB_LINELEFT: // SB_LINEUP
			case SB_PAGELEFT: // SB_PAGEUP
				if ( m_pos > 0 )
				{
					int r = (nSBCode == SB_LINELEFT) ? lineStep : m_page;
					delta = -min(r, m_pos);
				}
				break;
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				delta = static_cast<int>(nPos) - m_pos;
				break;
			default:
				break;
			}
			if ( m_pos + delta < 0 )
			{
				delta = -m_pos;
			}
			if ( m_pos + m_page + delta > m_max )
			{
				delta = m_max - (m_pos + m_page);
			}
			return delta;
		}
		/// �|�W�V�����ړ�
		int AddPos(int delta)
		{
			m_pos += delta;
			return m_pos;
		}
	};
	CSize		m_originalSize;		///< �I���W�i���̃N���C�A���g�T�C�Y
	CSize		m_offsetWinToClient;///< Window�T�C�Y��Client�T�C�Y�̍�
	CBarParam	m_horizontalParam;	///< �����p�����[�^
	CBarParam	m_verticalParam;	///< �����p�����[�^
	int			m_lineStep;			///< ���C���X�N���[�����̈ړ���
	bool		m_onThemeChanged;	///< �e�[�}�`�F���W���t���O
	bool		m_canSizeDisp;		///< �T�C�Y�ύX�C�x���g����

	/// �X�N���[���o�[��񏉊���
	void m_InitScrollInfo(void)
	{
		_super::ShowScrollBar(SB_HORZ, FALSE);
		_super::ShowScrollBar(SB_VERT, FALSE);
		CRect rect;
		_super::GetClientRect(rect);
		m_horizontalParam.SetMax(rect.Width());
		m_verticalParam.SetMax(rect.Height());
	}
	/**
	 * �T�C�Y�ύX�ʒm����
	 *	@param nType	�T�C�Y�ύX�̃^�C�v
	 *	@param cx		�N���C�A���g�̈�̐V������
	 *	@param cy		�N���C�A���g�̈�̐V��������
	 */
	void m_OnSize(WPARAM nType, int cx, int cy)
	{
		if ( m_originalSize.cx != 0 )
		{
			CRect rect;
			_super::GetWindowRect(rect);
			bool isEnable = !! (m_originalSize + m_offsetWinToClient != rect.Size());
			if ( isEnable )
			{
				if ( (rect.Height() - m_offsetWinToClient.cy) <= ::GetSystemMetrics(SM_CYHSCROLL) )
				{
					isEnable = false;
				}
			}
			_super::ScrollWindow(m_horizontalParam.SetPage(cx), m_verticalParam.SetPage(cy));
			m_horizontalParam.ShowBar(*this, SB_HORZ, isEnable);
			m_verticalParam.ShowBar(*this, SB_VERT, isEnable);
		}
	}
	/** 
	 * �����X�N���[������
	 *	@param nSBCode		�X�N���[���v���������X�N���[�� �o�[�̃R�[�h
	 *	@param nPos			�X�N���[�� �{�b�N�X�̈ʒu
	 *	@param hScrollBar	�R���g���[���ւ̃n���h��
	 */
	void m_OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
	{
		if ( hScrollBar == NULL )
		{
			int delta = m_horizontalParam.Scroll(nSBCode, nPos, m_lineStep);
			if ( delta != 0 )
			{
				_super::SetScrollPos(SB_HORZ, m_horizontalParam.AddPos(delta), TRUE);
				_super::ScrollWindow(-delta, 0);
			}
		}
	}
	/** 
	 * �����X�N���[������
	 *	@param nSBCode		�X�N���[���v���������X�N���[�� �o�[�̃R�[�h
	 *	@param nPos			�X�N���[�� �{�b�N�X�̈ʒu
	 *	@param hScrollBar	�R���g���[���ւ̃n���h��
 	 */
	void m_OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
	{
		if ( hScrollBar == NULL )
		{
			int delta = m_verticalParam.Scroll(nSBCode, nPos, m_lineStep);
			if ( delta != 0 )
			{
				_super::SetScrollPos(SB_VERT, m_verticalParam.AddPos(delta), TRUE);
				_super::ScrollWindow(0, -delta);
			}
		}
	}
	/** 
	 * �z�C�[������
	 *	@param fFlags �t���O
	 *	@param delta �ړ���
	 *	@param lParam �|�W�V�����B
	 *	@retval true ��������
	 *	@retval false �Ȃɂ����Ă��Ȃ�
 	 */
	bool m_OnMouseWheel(UINT fFlags, short delta, LPARAM lParam)
	{
		if ( ! m_verticalParam.IsEnable() ) { return false; }
		int d = 0;
		if ( delta < 0)
		{
			d = m_verticalParam.Scroll(SB_LINEDOWN, 0, (-delta) / 10 + 1);
		}
		else if ( delta > 0 )
		{
			d = m_verticalParam.Scroll(SB_LINEUP, 0, (delta) / 10 + 1);
		}
		if ( d != 0 )
		{
			_super::SetScrollPos(SB_VERT, m_verticalParam.AddPos(d), TRUE);
			_super::ScrollWindow(0, -d);
		}
		return true;
	}
};



}; //MFC
}; //TNB



