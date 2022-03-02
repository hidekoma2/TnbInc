#pragma once
/**
 *@file
 * �e�L�X�g�t���i���R���g���[���֌W�̃w�b�_
 *
 *		�e�L�X�g�t���̐i���R���g���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbStr.h"
#include "TnbBitmapImage.h"
#include "TnbMfcBitmapDC.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �e�L�X�g�t���i���R���g���[���N���X
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ńv���O���X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCProgressCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcTextProgressCtrl.h
 * 
 *	@date 16/06/28 �V�K�쐬
 */
class CTextProgressCtrl : public CProgressCtrl
{
	DEFSUPER(CProgressCtrl);
public:

	/// �R���X�g���N�^
	CTextProgressCtrl(void) : m_barColor(::GetSysColor(COLOR_HIGHLIGHT)), m_backColor(::GetSysColor(COLOR_WINDOW))
	{
	}

	/**
	 * [�ݒ�] ������ݒ�.
	 *	@param lpszText ������
	 */
	void SetWindowText(LPCTSTR lpszText)
	{
		_super::SetWindowText(lpszText);	
		m_CreateImage();
		RedrawWindow();
	}

	/**
	 * [�ݒ�] �J���[�ݒ�.
	 *	@param barColor �����A�o�[�̐F
	 *	@param backColor �w�i�̐F
	 */
	void SetColor(COLORREF barColor, COLORREF backColor)
	{
		m_barColor = barColor;
		m_backColor = backColor;
		m_CreateImage();
		RedrawWindow();
	}	

	/**
	 * [�ݒ�] �t�H���g�ݒ�
	 *	@param pFont �t�H���g�B�w�肵���t�H���g�͔j�����Ȃ��ł��������B
	 *	@param bRedraw �E�B���h�E���ĕ`�悷��ꍇ�� TRUE�B����ȊO�̏ꍇ�� FALSE�B
	 */
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE)
	{
		_super::SetFont(pFont, FALSE);
		if ( bRedraw )
		{
			m_CreateImage();
			RedrawWindow();
		}
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		m_CreateImage();
		_super::PreSubclassWindow();
		RedrawWindow();
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
		case WM_PRINTCLIENT:
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				if ( pDC != NULL )
				{
					OnDrawProgressImage(pDC);
				}
			}
			return 0;
		case WM_PAINT:
			{
				CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
				OnDrawProgressImage(&dc);
			}
			return 0;
		case WM_SIZE:
			{
				CSize sz(lParam);
				m_CreateImage(sz.cx, sz.cy);
			}
			break;
		default:
			break;
		}		
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// �v���O���X�o�[�̃C���[�W�\��
	void OnDrawProgressImage(CDC* pDC)
	{
		CSize sz = m_image1.GetSize();
		//
		int f = _super::GetPos();
		int m1, m2;
		GetRange(m1, m2);
		int xx = f * sz.cx / (m2 - m1);
		CRect r1(0, 0, xx, sz.cy);
		m_image2.Cut(r1).Draw(pDC->GetSafeHdc());
		CRect r2(xx, 0, sz.cx, sz.cy);
		m_image1.Cut(r2).Draw(pDC->GetSafeHdc(), xx, 0);
	}

	/// �C���[�W�쐬�T�u
	void m_Sub(CBitmapImage& _bi, int cx, int cy, COLORREF c1, COLORREF c2)
	{
		_bi.Set(cx, cy, c2);
		CBitmapDC dc(&_bi);
		CFont* pFont = dc.SelectObject(GetFont());
		CString s;
		GetWindowText(s);
		dc.SetTextColor(c1);
		dc.SetBkColor(c2);
		CRect rc(0, 0, cx, cy);
		CTextDrawer::DrawTextRect(dc, rc, DT_CENTER | DT_VCENTER, c1, s);
		dc.SelectObject(pFont);
	}

	/// �C���[�W�쐬
	void m_CreateImage(int cx = 0, int cy = 0)
	{
		if ( cx == 0 || cy == 0 )
		{
			CRect rc;
			GetWindowRect(rc);
			cx = rc.Width();
			cy = rc.Height();
		}
		m_Sub(m_image1, cx, cy, m_barColor, m_backColor);
		m_Sub(m_image2, cx, cy, m_backColor, m_barColor);
	}

	COLORREF		m_backColor;	///< �w�i�F
	COLORREF		m_barColor;		///< �`��F
	CBitmapImage	m_image1;		///< �����C���[�W
	CBitmapImage	m_image2;		///< �����C���[�W
};



};
};
