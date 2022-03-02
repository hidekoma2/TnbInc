#pragma once
/**
 *@file
 * MFC�R���g���[�����ʂ̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDynamicFunc.h"
#include "TnbTextDrawer.h"
#include "TnbBitmapImage.h"


#ifndef __AFX_H__
 #error
#endif



//TNB Library
namespace TNB {
namespace MFC {


	
/// MFC���ʃ��C�u����
namespace MFCLIB 
{

	/** @addtogroup	MFC
	 * @{
	 */

	/**
	 * [�ϊ�] Static�X�^�C����DrawText�X�^�C���ɕϊ�
	 *	@param staticStyle Static�X�^�C��
	 *	@return DrawText�X�^�C��
	 *	@deprecated CTextDrawer::StaticToDrawStyle() ���g�p���Ă��������B
	 */
	inline UINT _deprecated StaticToDrawStyle(DWORD staticStyle)
	{
		return CTextDrawer::StaticToDrawStyle(staticStyle);
	}

	/**
	 * [�ݒ�] �R���g���[���ʒu�ݒ�.
	 *		�R���g���[�����A(�e�E�B���h�E��)�w��̃N���C�A���g���W�Ɉړ����܂��B
	 *	@param pWnd �R���g���[���B
	 *	@param x �N���C�A���g X���W�B-1�Ȃ�ړ��Ȃ��B
	 *	@param y �N���C�A���g Y���W�B-1�Ȃ�ړ��Ȃ��B
	 *	@param boIsRepaint �E�B���h�E���ĕ`�悷�邩�ǂ������w�肵�܂��B
	 *					true ���w�肷��ƁA�E�B���h�E�� WM_PAINT ���b�Z�[�W�������܂��B
	 *					false ���w�肷��ƁA�ĕ`��͍s���܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	inline bool SetControlPos(CWnd* pWnd, int x, int y, bool boIsRepaint = false)
	{
		CWnd* pParent = pWnd->GetParent();
		if ( pParent == NULL ) { return false; }
		CRect rc;
		pWnd->GetWindowRect(&rc);
		pParent->ScreenToClient(&rc);
		int xx = (x < 0) ? rc.left : x;
		int yy = (y < 0) ? rc.top : y;
		pWnd->MoveWindow(xx, yy, rc.Width(), rc.Height(), boIsRepaint);
		return true;
	}

	/**
	 * [�擾] �R���g���[���ʒu�擾.
	 *		�R���g���[����(�e�E�B���h�E��)�N���C�A���g���W���擾���܂��B
	 *	@param pWnd �R���g���[���B
	 *	@param[out] _po ���W
	 *	@retval true �����B _po �ɍ��W������܂��B
	 *	@retval false ���s�B
	 */
	inline bool GetControlPos(CWnd* pWnd, POINT& _po)
	{
		_po.x = _po.y = 0;
		if ( ! ::IsWindow(pWnd->GetSafeHwnd()) ) { return false; }
		CWnd* pParent = pWnd->GetParent();
		if ( pParent == NULL ) { return false; }
		CRect rc;
		pWnd->GetWindowRect(&rc);
		pParent->ScreenToClient(&rc);
		_po = rc.TopLeft();
		return true;
	}

	/**
	 * [�ݒ�] �N���C�A���g�T�C�Y�ݒ�.
	 *		�w��̃E�B���h�E�̃E�B���h�E�T�C�Y���w��̃N���C�A���g�T�C�Y�ɂȂ�悤�ɕύX���܂��B
	 *	@param pWnd �E�B���h�E�B
	 *	@param cx ���̑傫���B-1�Ȃ�ύX�Ȃ��B
	 *	@param cy �c�̑傫���B-1�Ȃ�ύX�Ȃ��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	inline bool ChangeClientSize(CWnd* pWnd, int cx = -1, int cy = -1)
	{
		CRect rcWin;
		CRect rcCli;
		pWnd->GetWindowRect(&rcWin);
		pWnd->GetClientRect(&rcCli);
		int wx = rcWin.Width() - rcCli.Width();
		int wy = rcWin.Height() - rcCli.Height();
		int xx = (cx < 0) ? rcCli.Width() : cx;
		int yy = (cy < 0) ? rcCli.Height() : cy;
		return !! pWnd->SetWindowPos(NULL, 0, 0, wx + xx, wy + yy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	/**
	 * [�擾] �N���C�A���g�ʒu�擾.
	 *		�w��̃E�B���h�E�̍��ォ��N���C�A���g�̍���̂����Ԃ��܂��B
	 *	@param pWnd �E�B���h�E�B
	 *	@return �ʒu�B
	 */
	inline CPoint GetClientPoint(CWnd* pWnd)
	{
		CRect winRc;
		pWnd->GetWindowRect(&winRc);
		CRect cliRc;
		pWnd->GetClientRect(&cliRc);
		pWnd->ClientToScreen(&cliRc);
		return cliRc.TopLeft() - winRc.TopLeft();
	}

	/**
	 * [�\��] ���\��.
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param rect �\���͈�
	 *	@param pen �y��
	 */
	inline void DrawBox(CDC* pDC, const RECT& rect, CPen& pen)
	{
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right - 1, rect.top);
		pDC->LineTo(rect.right - 1, rect.bottom - 1);
		pDC->LineTo(rect.left, rect.bottom - 1);
		pDC->LineTo(rect.left, rect.top);
		pDC->SelectObject(pOldPen);
	}

	/**
	 * [�\��] ���\��.
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param rect �\���͈�
	 *	@param nPenStyle �y���̃X�^�C��
	 *	@param nWidth �y���̕�
	 *	@param color �����F
	 */
	inline void DrawBox(CDC* pDC, const RECT& rect, int nPenStyle, int nWidth, COLORREF color)
	{
		CPen pen;
		pen.CreatePen(nPenStyle, nWidth, color);
		DrawBox(pDC, rect, pen);
	}

	/**
	 * [�\��] �j�����\��.
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param rect �\���͈�
	 *	@param color �F
	 */
	inline void DrawDottedBox(CDC* pDC, const RECT& rect, COLORREF color)
	{
		DrawBox(pDC, rect, PS_DOT, 1, color);
	}

	/**
	 * [�\��] �e�\��.
	 *		�w��̃R���g���[���͈̔͂̐e���ĕ`�悵�܂��B
	 *	@param pWndControl �R���g���[���B
	 *	@param boIsRedraw true �Ȃ� ::RedrawWindow() , false �Ȃ� InvalidateRect() ���g�p���܂��B
	 */
	inline void DrawParent(CWnd* pWndControl, bool boIsRedraw = true)
	{
		if ( pWndControl != NULL )
		{
			CRect rect;
			pWndControl->GetWindowRect(&rect);
			CWnd* pWnd = pWndControl->GetParent();
			if ( pWnd != NULL )
			{
				pWnd->ScreenToClient(&rect);
				if ( boIsRedraw )
				{
					pWnd->RedrawWindow(&rect);
				}
				else
				{
					pWnd->InvalidateRect(&rect);
				}
			}
		}
	}

	/**
	 * [�ݒ�] �N���C�A���g�T�C�Y����.
	 *		�w��̃N���C�A���g�̃T�C�Y�𕶎����DRAW�X�^�C�����璲�����܂��B
	 *	@param pWnd �f�o�C�X�R���e�L�X�g�̃E�B���h�E
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param drawStyle Draw�X�^�C��
	 *	@param lpsz ������
	 *	@param margin �}�[�W���B�K�v�ȃN���C�A���g�T�C�Y�ɉ�����T�C�Y�ł��B
	 *			�v���X�w�肷��ƁA�ʒu��ς����ɁA���A�����̂ݕύX����܂��B\n
	 *			�}�C�i�X�w�肷��ƁA�ʒu�A���A������ύX���܂��B\n
	 *			AdjustSize(10) �G�ʒu��ς����ɁA����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B\n
	 *			AdjustSize(-5) �G�ʒu���c������� 5 �ړ����A����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B
	 */
	inline void AdjustClientSize(CWnd* pWnd, CDC* pDC, UINT drawStyle, LPCTSTR lpsz, int margin = 2)
	{
		CString str = lpsz;
		if ( str.IsEmpty() ) { return; }
		CRect rect;
		pWnd->GetClientRect(&rect);
		CSize sz(rect.Width(), 0);
		if ( CTextDrawer::CalcTextSize(sz, *pDC, drawStyle, str) )
		{
			if ( margin >= 0 )
			{
				ChangeClientSize(pWnd, sz.cx + margin, sz.cy + margin);
			}
			else
			{
				CPoint po;
				ChangeClientSize(pWnd, sz.cx - margin * 2, sz.cy - margin * 2);
				GetControlPos(pWnd, po);
				SetControlPos(pWnd, po.x + margin, po.y + margin);
			}
		}
	}

#ifndef _WIN32_WCE
	/**
	 * [�ݒ�] �N���C�A���g�T�C�Y����.
	 *		�w��̃N���C�A���g�̃T�C�Y�𕶎����DRAW�X�^�C�����璲�����܂��B
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param drawStyle Draw�X�^�C��
	 *	@param lpsz ������
	 *	@param margin �}�[�W���B�K�v�ȃN���C�A���g�T�C�Y�ɉ�����T�C�Y�ł��B
	 *			�v���X�w�肷��ƁA�ʒu��ς����ɁA���A�����̂ݕύX����܂��B\n
	 *			�}�C�i�X�w�肷��ƁA�ʒu�A���A������ύX���܂��B\n
	 *			AdjustSize(10) �G�ʒu��ς����ɁA����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B\n
	 *			AdjustSize(-5) �G�ʒu���c������� 5 �ړ����A����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B
	 */
	inline void AdjustClientSize(CDC* pDC, UINT drawStyle, LPCTSTR lpsz, int margin = 2)
	{
		AdjustClientSize(pDC->GetWindow(), pDC, drawStyle, lpsz, margin);
	}
#endif

	/**
	 * [�擾] �e�摜�擾.
	 *		�w��̃R���g���[���͈̔͂̐e�̉摜���擾���܂��B
	 *	@param pWndControl �R���g���[���B
	 *	@param backColor �w�i�F�B�N���b�s���O�͈͊O�̐F���w�肵�܂��B 
	 *					CLR_AUTOSELECT �Ȃ�A WM_ERASEBKGND ���g���A��ɔw�i��`�悵�܂��B
	 *					�ȗ�����ƁA�w�i��`�揈�����܂���B
	 *	@return �摜
	 */
	inline CBitmapHandle GetParentImage(CWnd* pWndControl, COLORREF backColor = CLR_INVALID)
	{
		CBitmapHandle b;
		if ( pWndControl != NULL )
		{
			CWnd* pWnd = pWndControl->GetParent();
			if ( pWnd != NULL )
			{
				CRect rc;
				pWndControl->GetClientRect(rc);
				pWndControl->ClientToScreen(rc);
				pWnd->ScreenToClient(rc);
				CBitmapImage bi;
				bi.SetFromClient(*pWnd, rc, backColor);
				b = bi.GetBitmapHandle();
			}
		}
		return b;
	}

	/**
	 * [�ݒ�] STATIC ������ݒ聕�T�C�Y����.
	 *		�w��̕�������R���g���[���֐ݒ肵�ASTATIC�̃X�^�C�����l�����R���g���[���̃T�C�Y�𒲐����܂��B
	 *	@param pWnd STATIC�R���g���[��.
	 *	@param lpsz ������
	 *	@param margin �}�[�W���B�K�v�ȃN���C�A���g�T�C�Y�ɉ�����T�C�Y�ł��B
	 *			�v���X�w�肷��ƁA�ʒu��ς����ɁA���A�����̂ݕύX����܂��B\n
	 *			�}�C�i�X�w�肷��ƁA�ʒu�A���A������ύX���܂��B\n
	 *			AdjustSize(10) �G�ʒu��ς����ɁA����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B\n
	 *			AdjustSize(-5) �G�ʒu���c������� 5 �ړ����A����������ŏ��̕��A������ +10�����T�C�Y��ݒ肵�܂��B
	 */
	inline void SetTextForStatic(CWnd* pWnd, LPCTSTR lpsz, int margin = 2)
	{
		CString str = lpsz;
		pWnd->SetWindowText(str);
		if ( str.IsEmpty() ) { return; }
		UINT drawStyle = CTextDrawer::StaticToDrawStyle(pWnd->GetStyle());
		CDC* pDC = pWnd->GetDC();
		CGdiObject* pOld = pDC->SelectObject(pWnd->GetFont());
		AdjustClientSize(pWnd, pDC, drawStyle, lpsz, margin);
		pDC->SelectObject(pOld);
		pWnd->ReleaseDC(pDC);
	}

	/**
	 * [�ݒ�] ���[�W�����ݒ�.
	 *		WindowOrg �̐ݒ�ɍl������ SelectClipRgn�ł��B
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param rgn ���[�W����
	 *	@param mode ���s���鑀����w�肵�܂��B���̒l�̂����ꂩ�ɂȂ�܂��B
	 *		<table>
	 *		<tr><td>�l</td>	<td>����</td>
	 *		</tr>
	 *		<tr><td>RGN_AND</td>	
	 *						<td>�V�����N���b�v�̈�́A���݂̃N���b�v�̈�� pRgn �Ŏ��ʂ����
	 *						�̈�̏d�Ȃ荇�����̈���������܂��B</td>
	 *		</tr>
	 *		<tr><td>RGN_COPY</td>
	 *						<td>�V�����N���b�v�̈�́ApRgn �Ŏ��ʂ����̈�̃R�s�[�ł��B
	 *						pRgn �Ŏ��ʂ����̈悪 NULL �̂Ƃ��́A�V�����N���b�v�̈��
	 *						����̃N���b�v�̈� (NULL �̈�) �ɂȂ�܂��B</td>
	 *		</tr>
	 *		<tr><td>RGN_DIFF</td>
	 *						<td>�V�����N���b�v�̈�́ApRgn �Ŏ��ʂ����̈悩�猻�݂�
	 *						�N���b�v�̈�����O�����̈���������܂��B</td>
	 *		</tr>
	 *		<tr><td>RGN_OR</td>
	 *						<td>�V�����N���b�v�̈�́A���݂̃N���b�v�̈�� pRgn ��
	 *						���ʂ����̈���������܂��B</td>
	 *		</tr>
	 *		<tr><td>RGN_XOR</td>
	 *						<td>�V�����N���b�v�̈�́A���݂̃N���b�v�̈�� pRgn ��
	 *						���ʂ����̈���������A�d�Ȃ荇�����̈�����O���܂��B</td>
	 *		</tr>
	 *		</table>
	 *	@return �߂�l�͗̈�̌^��Ԃ��܂��B
	 *	@retval COMPLEXREGION	�V�����N���b�v�̈�ɂ́A�d�Ȃ荇�������E��������܂��B
	 *	@retval ERROR			�f�o�C�X �R���e�L�X�g�܂��͗̈悪�����ł��B
	 *	@retval NULLREGION		�V�����N���b�v�̈�͋�ł��B
	 *	@retval SIMPLEREGION	�V�����N���b�v�̈�ɂ͏d�Ȃ荇�����E��������܂���B
	 */
	inline int SelectClipRgnOrg(CDC* pDC, HRGN rgn, int mode = RGN_COPY)
	{
		int r = ERROR;
		CPoint offset = pDC->GetWindowOrg();
		if ( offset.x == 0 && offset.y == 0 )
		{
			r = ::ExtSelectClipRgn(*pDC, rgn, mode);
		}
		else
		{
			HRGN tmp= ::CreateRectRgn(0, 0, 0, 0);
			::CombineRgn(tmp, rgn, NULL, RGN_COPY);
			::OffsetRgn(tmp, -offset.x, -offset.y);
			r = ::ExtSelectClipRgn(*pDC, tmp, mode);
			_DeleteObject(tmp);
		}
		return r;
	}

	/**
	 * �A�N�V�����t���O�l.
	 *		SetLyeredWindow() �Ŏg�p���܂��B
	 */
	enum ELayeredAction
	{
		E_ColorKey			= _BIT(0),	///< �����F�Ƃ��� crKey ���g���܂��B
		E_Alpha				= _BIT(1),	///< bAlpha ���g���āA���C���[�h�E�B���h�E�̕s�����x�����肵�܂��B
		E_SetLayeredStyle	= _BIT(31),	///< WS_EX_LAYERED �X�^�C����t�����܂��B
	};

	/**
	 * [�ݒ�] SetLayeredWindow.
	 *		API �� SetLayeredWindowAttributes ���g�p���܂��B
	 *	@param pWnd �Ώۂ̃n���h�����w�肵�܂��B WS_EX_LAYERED �w�肪�Ȃ��ꍇ�A�t�����܂��B
	 *	@param crKey ���C���[�h�E�B���h�E�̍쐬���Ɏg�������̃J���[�L�[���w�肵�܂��B
	 *				�E�B���h�E�ɂ���ĕ`�悳��邱�̐F�̃s�N�Z���͂��ׂē����ɂȂ�܂��B
	 *	@param bAlpha ���C���[�h�E�B���h�E�̕s�����x�������A���t�@�l���w�肵�܂��B
	 *				0 ���w�肷��ƁA�E�B���h�E�͊��S�ɓ����ɂȂ�܂��B255 ���w�肷��ƁA�E�B���h�E�͕s�����ɂȂ�܂��B
	 *	@param dwFlags �A�N�V�����t���O���w�肵�܂��B ELayeredAction �̒l��g�ݍ��킹�Ďg���܂��B 
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	inline bool SetLayeredWindow(CWnd* pWnd, COLORREF crKey = 0, BYTE bAlpha = 255, DWORD dwFlags = E_Alpha | E_SetLayeredStyle)
	{
		bool r = false;
		if ( (dwFlags & E_SetLayeredStyle) != 0 )
		{
			dwFlags &= ~E_SetLayeredStyle;
			pWnd->ModifyStyleEx(0, WS_EX_LAYERED);
		}
		try
		{
			typedef BOOL (__stdcall *PSETLAYEREDATTR)(HWND, COLORREF, BYTE, DWORD); 
			r = !! CDynamicFuncT<PSETLAYEREDATTR>("user32.dll", "SetLayeredWindowAttributes")()
										(pWnd->m_hWnd, crKey, bAlpha, dwFlags);
		}
		catch (CNullPointerException& e)
		{
			e.OnCatch();
			r = false;
		}
		return r;
	}

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
		enum 
		{
			_TnbProcessingFlag  = WS_SYSMENU,
			_TnbWndLong		= GWL_STYLE
		};
		
	#endif

	/**
	 * [�ݒ�] �����σt���O����.
	 *		HWND �̏����σt���O��ݒ肵�܂��B
	 *	@note CPositionAdjuster �� CDrawing* �Ƃ����R���g���[���Ŏg�p����܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 *	@param isEnable �����σt���O
	 */
	inline void SetProcessingFlag(HWND hWnd, bool isEnable)
	{
		LONG l = ::GetWindowLong(hWnd, _TnbWndLong);
		if ( isEnable )
		{
			l |= _TnbProcessingFlag;
		}
		else
		{
			l &= ~_TnbProcessingFlag;
		}
		::SetWindowLong(hWnd, _TnbWndLong, l);
		ASSERT( l == ::GetWindowLong(hWnd, _TnbWndLong) );
	}

	/**
	 * [�擾] �����σt���O�擾.
	 *		HWND �̏����σt���O���擾���܂��B
	 *	@note CPositionAdjuster �� CDrawing* �Ƃ����R���g���[���Ŏg�p����܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 *	@retval true ������
	 *	@retval false ������
	 */
	inline bool IsProcessingFlag(HWND hWnd)
	{
		LONG l = ::GetWindowLong(hWnd, _TnbWndLong);
		return (l & _TnbProcessingFlag) != 0;
	}

	/**
	 * [����] ���b�Z�[�W�|���v
	 */
	inline void PumpMessage(void)
	{
		MSG msg;
		while ( ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		{
			AfxGetThread()->PumpMessage();
		}
	}

	/**
	 * [����] WM_COMMAND���M.
	 *		�R���g���[�����e��ʂ� WM_COMMAND �𓊂��鏈�����s���܂��B
	 *	@param pCtrl �R���g���[��
	 *	@param cmd �ʒm����R�[�h
	 *	@return ����
	 */
	inline LRESULT SendCommandMessage(CWnd* pCtrl, UINT cmd)
	{
		CWnd* pParent = pCtrl->GetParent();
		if ( pParent != NULL && ::IsWindow(pParent->GetSafeHwnd()) )
		{
			WPARAM wp = MAKEWPARAM(pCtrl->GetDlgCtrlID(), cmd);
			LPARAM lp = reinterpret_cast<LPARAM>(pCtrl->GetSafeHwnd());
			return pParent->SendMessage(WM_COMMAND, wp, lp);
		}
		return 0;
	}

	/**
	 * @}
	 */
}; //MFCLIB



/**@ingroup MFC
 * �R���g���[���ꊇ�����N���X
 *
 *		�p�����Ďg�p���܂��B OnFound() �ɏ����������������܂��B
 *		�_�C�A���O�̃n���h����n���ƁA����ȉ��̃R���g���[�������ׂČ������A
 *		�R�[���o�b�N���܂��B
 * 
 *	@par�K�v�t�@�C��
 *			TnbMfcCommon.h
 * 
 *	@date 07/11/08 �V�K�쐬
 */
class CFindDescendants
{
public:
	/**
	 * [�ʒm] ����.
	 *	@param hWnd ���������E�B���h�E.
	 *	@retval true �p���B
	 *	@retval false �������~.
	 */
	virtual bool OnFound(HWND hWnd) = 0;

	/**
	 * [���s] �����J�n.
	 *	@param hWnd ��������e�E�B���h�E.
	 *	@param bDeep �������郌�x�����w�肵�܂��Btrue �̏ꍇ�͂��ׂĂ̎q�E�B���h�E���������܂��B
	 *					false �̏ꍇ�͒��n�̎q�E�B���h�E�݂̂��������܂��B
	 *	@retval true �S�����I��.
	 *	@retval false �������~���ꂽ.
	 */
	bool Execute(HWND hWnd, bool bDeep = false)
	{
		for (HWND hChild = ::GetTopWindow(hWnd); hChild != NULL; hChild = ::GetNextWindow(hChild, GW_HWNDNEXT))
		{
			if ( ! OnFound(hChild) )
			{
				return false;
			}
			if (bDeep && ::GetTopWindow(hChild) != NULL)
			{
				// send to child windows after parent
				if ( ! Execute(hChild, bDeep) )
				{
					return false;
				}
			}
		}
		return true;
	}
};



/**@ingroup MFC
 * �e�L�X�g�`��x�[�X�N���X
 *
 *		�e�L�X�g��`�悷��R���g���[���ɖ{�N���X���p�����Ă���܂��B
 *		���[�U�����ڎg�����Ƃ͂���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcCommon.h
 * 
 *	@date 07/10/24 �V�K�쐬
 */
class CDrawTextBase
{
protected:
	/// �J���[�Z�b�g�^
	struct TColor
	{
		COLORREF	forward;		///< �O�̕����̐F
		COLORREF	back;			///< ���̕����̐F
		/// �R���X�g���N�^
		TColor(void) : forward(::GetSysColor(COLOR_WINDOWTEXT)), back(CLR_INVALID) { }
		/// �R���X�g���N�^ @param c �O�̕����̐F
		TColor(COLORREF c) : forward(c), back(CLR_INVALID) { }
	};
	TColor			m_normalColors;		///< �W����ԐF
	TColor			m_disableColors;	///< ������ԐF
	CPoint			m_offset;			///< �����ƕ����̂���

	/**
	 * [�\��] �����\��.
	 *		�w��̃J���[�ŕ\�����܂��B�����ƕ����̂���́A SetTextDrawOffset() �Őݒ肵�����̂��g���܂��B
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param rect �\���͈�
	 *	@param drawStyle Draw�X�^�C��
	 *	@param c �J���[���
	 *	@param str �\������
	 */
	virtual void DrawText(CDC* pDC, const RECT& rect, UINT drawStyle, const TColor& c, LPCTSTR str)
	{
		if ( (drawStyle & DT_VCENTER) != 0 )
		{
			drawStyle &= ~DT_VCENTER;
			int h = rect.bottom - rect.top;
			CSize sz(rect.right - rect.left, h);
			CTextDrawer::CalcTextSize(sz, *pDC, drawStyle, str);
			CRect rc = rect;
			rc.top += (h - sz.cy) / 2;
			CTextDrawer::DrawTextRect(*pDC, rc, drawStyle, m_offset, c.forward, c.back, str);
			return;
		}
		CTextDrawer::DrawTextRect(*pDC, rect, drawStyle, m_offset, c.forward, c.back, str);
	}

	/**
	 * [�\��] �����\��.
	 *		�w��̃J���[�ŕ\�����܂��B�����ƕ����̂���́A SetTextDrawOffset() �Őݒ肵�����̂��g���܂��B
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param rect �\���͈�
	 *	@param drawStyle Draw�X�^�C��
	 *	@param boIsEnable true �Ȃ� SetTextColor() �Ŏw�肵���F�A false �Ȃ� SetTextDisableColor() �Ŏw�肵���F�ŕ\���B
	 *	@param str �\������
	 */
	void DrawTextEx(CDC* pDC, const RECT& rect, UINT drawStyle, bool boIsEnable, LPCTSTR str)
	{
		DrawText(pDC, rect, drawStyle, (boIsEnable ? m_normalColors : m_disableColors), str);
	}

public:

	/// �R���X�g���N�^
	CDrawTextBase(void) : m_normalColors(), m_disableColors(CLR_INVALID), m_offset(0, 0)
	{
	}

	/**
	 * [�ݒ�] �e�L�X�g�I�t�Z�b�g�ݒ�.
	 *		�{�N���X�͓�̃e�L�X�g�����炵�ĕ\�����邱�ƂŁA���s���A���̊����o�����Ƃ��o���܂��B
	 *		�O�ƌ��e�L�X�g�̂�����w�肵�܂��B
	 *	@note �W���ł� 0,0 �ł��B
	 *	@param x �������̂���
	 *	@param y �c�����̂���
	 */
	void SetTextDeepOffset(int x, int y)
	{
		m_offset.x = x;
		m_offset.y = y;
	}

	/**
	 * [�ݒ�] �e�L�X�g�F�w��.
	 *	@param color1 �O�̕����̐F���w�肵�܂��B CLR_INVALID ���w�肷��ƁA �x�̂���O���[�̕����ɂȂ�܂��B
	 *	@param color2 ���̕����̐F���w�肵�܂��B�ȗ��\�B
	 */
	void SetTextColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_normalColors.forward = color1;
		m_normalColors.back = color2;
	}

	/**
	 * [�ݒ�] ������ԃe�L�X�g�F�w��.
	 *	@param color1 �O�̕����̐F���w�肵�܂��B CLR_INVALID ���w�肷��ƁA �x�̂���O���[�̕����ɂȂ�܂��B
	 *	@param color2 ���̕����̐F���w�肵�܂��B�B�ȗ��\�B
	 */
	void SetTextDisableColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_disableColors.forward = color1;
		m_disableColors.back = color2;
	}
};



}; //MFC
}; //TNB


