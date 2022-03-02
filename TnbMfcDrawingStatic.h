#pragma once
/**
 *@file
 * �`����\���֌W�̃w�b�_
 *
 *		�`�����\�����邽�߂̃R���g���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapDrawer.h"
#include "TnbMfcRgnControl.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����\���R���g���[���N���X
 *
 *		�`�����\������R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingStatic.h
 * 
 *	@date 07/11/14 �V�K�쐬
 *	@date 07/12/19 Add �n���\�b�h�� isAdjust �����A�ǉ��B
 *	@date 09/07/24 SetDrawingObject() �̓��������������B
 *	@date 10/04/05 �\�����P�B�ĕ`��t���O��ǉ��B
 *	@date 11/03/15 �T�u�N���X�����Ɍ��N���X���摜�������Ă�����j������悤�ɏC���B
 *	@date 14/04/07 WM_SIZE �ɑΉ��B
 */
class CDrawingStatic : public CRgnControlAddinT<CWnd>
{
	DEFSUPER(CRgnControlAddinT<CWnd>);
public:

	/// �f�X�g���N�^
	virtual ~CDrawingStatic(void)
	{
	}

	/// �R���X�g���N�^
	CDrawingStatic(void) : _super()
	{
	}

	/// ����R���X�g���N�^
	CDrawingStatic(const IDrawable& draw) : _super()
	{
		m_pDrawing = draw.Clone();
	}

	/**
	 * �R�s�[�I�y���[�^.
	 *	@param other �R�s�[���B
	 *	@return �����̎Q��
	 */
	CDrawingStatic& operator=(const CDrawingStatic& other)
	{
		if ( ! other.m_pDrawing.IsNull() )
		{
			m_pDrawing = other.m_pDrawing->Clone();
		}
		m_Resize();
		return *this;
	}

	/**
	 * [�ݒ�] �`����ݒ�.
	 *	@param draw �`����. �{�I�u�W�F�N�g�ɂ̓N���[�����L�������̂ŁA draw �͔j�����Ă����܂��܂���B
	 *	@param isClientSizeAdjust true ���ȗ��Ȃ� �w��� draw �̑傫���ɃN���C�A���g�𒲐����܂��B
	 *					false �Ȃ�N���C�A���g�̑傫���� draw �ɐݒ肵�܂��i�N���C�A���g�̑傫���͕ς��܂���j�B
	 *	@param isRedraw true �Ȃ�ĕ`�悵�܂��B false ���ȗ��Ȃ�Ȃɂ����܂���B
	 */
	void SetDrawingObject(const IDrawable& draw, bool isClientSizeAdjust = true, bool isRedraw = false)
	{
		m_pDrawing = draw.Clone();
		if ( isClientSizeAdjust )
		{
			m_Resize();
		}
		else if ( m_pDrawing != NULL )
		{
			CRect rc;
			_super::GetClientRect(&rc);
			m_pDrawing->Resize(rc.Size());
		}
		if ( isRedraw )
		{
			CClientDC dc(this);
			m_Draw(&dc);
		}
	}

	/**
	 * [�ݒ�] �`����ݒ�.
	 *	@param bmp �r�b�g�}�b�v
	 *	@param isAdjust true ���ȗ��Ȃ� �w��� bmp �̑傫���ɃN���C�A���g�𒲐����܂��B
	 *					false �Ȃ�N���C�A���g�̑傫���͕ς��܂���B
	 *	@param isRedraw true �Ȃ�ĕ`�悵�܂��B false ���ȗ��Ȃ�Ȃɂ����܂���B
	 */
	void SetDrawingBitmap(CBitmapHandle bmp, bool isAdjust = true, bool isRedraw = false)
	{
		SetDrawingObject(CBitmapDrawer(bmp), isAdjust ,isRedraw);
	}

	/**
	 * [�ݒ�] �N���C�A���g���[�W�����ݒ�.
	 *	@param color �R���g���[���O�ɂ���F�B�ȗ�����ƁA�����̐F��ΏېF�Ƃ��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetClientRgnEx(COLORREF color = CLR_AUTOSELECT)
	{
		if ( m_pDrawing.IsNull() ) { return false; }
		if ( color == CLR_INVALID )
		{
			HRGN rgn = NULL;
			return !! SetClientRgn(rgn);
		}
		CBitmapImage bi(CBitmapDrawer::ToBitmap(*m_pDrawing, color));
		HRGN rgn = bi.CreateRgn(color);
		if ( ! SetClientRgn(rgn) )
		{
			_DeleteObject(rgn);
			return false;
		}
		return true;
	}

protected:

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
		case WM_ERASEBKGND:
			TTRACE0("WM_ERASEBKGND\n");
			return 0;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				m_Draw(pDC);
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
				m_Draw(&dc);
			}
			return 0;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			return 0;
		case WM_SIZE:
			if ( ! m_pDrawing.IsNull() )
			{
				CRect rc;
				_super::GetClientRect(&rc);
				m_pDrawing->Resize(rc.Size());
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
		HBITMAP hBmp = reinterpret_cast<HBITMAP>(_super::SendMessage(STM_GETIMAGE, IMAGE_BITMAP));
		if ( hBmp != NULL )
		{
			::DeleteObject(hBmp);
		}
		HICON hIcon = reinterpret_cast<HICON>(_super::SendMessage(STM_GETIMAGE, IMAGE_ICON));
		if ( hIcon != NULL )
		{
			::DeleteObject(hIcon);
		}
		_super::ModifyStyle(0, SS_OWNERDRAW);
		_super::PreSubclassWindow();
		m_Resize();
	}

private:
	IDrawable::Ptr	m_pDrawing;		///< �ێ�����`����
	/// ���T�C�Y
	void m_Resize(void)
	{
		if ( m_pDrawing.IsNull() ) { return; }
		SIZE size;
		if ( m_pDrawing->GetSize(size) )
		{
			MFCLIB::ChangeClientSize(this, size.cx, size.cy);
		}
	}
	/// �`��.
	void m_Draw(CDC* pDC)
	{
		if ( ! m_pDrawing.IsNull() )
		{
			if ( m_clientRgn != NULL )
			{
				MFCLIB::SelectClipRgnOrg(pDC, m_clientRgn);
			}
			else
			{
#ifndef _WIN32_WCE
				//@ 100316 ����Ȃ��H CE�ł͂����NG
				CRect rc;
				_super::GetClientRect(&rc);
				HRGN rgn = ::CreateRectRgnIndirect(&rc);
				::ExtSelectClipRgn(*pDC, rgn, RGN_AND);
				_DeleteObject(rgn);
#endif
			}
			CWnd* pWnd = _super::GetParent();
			if ( pWnd != NULL )
			{
				pWnd->SendMessage(WM_CTLCOLORSTATIC,
					reinterpret_cast<WPARAM>(pDC->GetSafeHdc()), reinterpret_cast<LPARAM>(m_hWnd));
			}
			m_pDrawing->Draw(*pDC);
		}
	}
};



}; //MFC
}; //TNB



