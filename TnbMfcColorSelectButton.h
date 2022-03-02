#pragma once
/**
 *@file
 * �J���[�I���{�^���֌W�̃w�b�_
 *
 *		�J���[��I������{�^����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcDrawingButton.h"
#include "TnbFontHandle.h"
#include "TnbAccessor.h"



//TNB Library
namespace TNB {
namespace MFC {


	
/**
 * �J���[�I���{�^�� �I��ύX.
 *	@see CColorSelectButton
 */
#define CSBN_SELCHANGE 10



/**@ingroup MFCCONTROL
 * �J���[�I���{�^���R���g���[��
 *
 *	@note �J���[����I�����邱�Ƃ��o����{�^���R���g���[���ɂȂ�܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń{�^���R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCButton�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@note �J���[���ύX�����ƁA CSBN_SELCHANGE ���e�ɒʒm����܂��B
 *		\code
 *		BEGIN_MESSAGE_MAP(CFooDlg, CDialog)
 *		//{{AFX_MSG_MAP(CFooDlg)
 *					:
 *			ON_CONTROL(CSBN_SELCHANGE, IDC_BUTTON_COLOR, OnColorButtonSelChange)
 *					:
 *		//}}AFX_MSG_MAP
 *		END_MESSAGE_MAP()
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcColorSelectButton.h
 * 
 *	@date 10/05/24 �V�K�쐬
 *	@date 16/06/28 �f�t�H���g�J���[�������ɂȂ�悤�Ɏd�l�ύX�B
 *	@date 16/06/28 �f�t�H���g�J���[���w��� IAccessor �ɕۑ������悤�ɋ@�\�ǉ��B
 */
class CColorSelectButton : public CDrawingButton
{
	DEFSUPER(CDrawingButton);
public:

	/// �R���X�g���N�^
	CColorSelectButton(void) : m_pAccessor(NULL)
	{
		m_face.SetSelectColor(RGB(255, 255, 255));
		m_default.Resize(DEFAULTMAX);
		loop ( i, DEFAULTMAX )
		{
			BYTE r = down_cast<BYTE>((i & _BIT(0)) == 0 ? 255 : 0);
			BYTE g = down_cast<BYTE>((i & _BIT(1)) == 0 ? 255 : 0);
			BYTE b = down_cast<BYTE>((i & _BIT(2)) == 0 ? 255 : 0);
			m_default[i] = RGB(r, g, b);
		}
	}

	/**
	 * [�ݒ�] �A�N�Z�T�[�w��.
	 *	@param pAccessor �A�N�Z�T�[�B�w�肵���A�N�Z�T�[�͔j�����Ȃ��ł��������B
	 *	@param lpszAccessorSectionName �A�N�Z�T�[�̃Z�N�V������
	 */
	void SetAccessor(IAccessor* pAccessor, LPCTSTR lpszAccessorSectionName)
	{
		m_pAccessor = pAccessor;
		m_accessorSectionName = lpszAccessorSectionName;
		IAccessor::CSection sec(m_pAccessor, m_accessorSectionName);
		loop ( i, DEFAULTMAX )
		{
			CStr key = CStr::Fmt(_T("c%d"), i + 1);
			m_default[i] = sec.QueryDword(key, m_default[i]);
		}
	}

	/**
	 * [�ݒ�] �I��F�ݒ�.
	 *	@param c �F
	 *	@return �ݒ�O�̐F
	 */
	COLORREF SetSelectColor(COLORREF c)
	{
		COLORREF r = m_face.GetSelectColor();
		m_face.SetSelectColor(c);
		m_InsertDefaultColor(c);
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			_super::SetNormalDrawer(m_face);
			_super::RedrawWindow();
		}
		return r;
	}

	/**
	 * [�擾] �I��F�擾.
	 *	@return �F
	 */
	COLORREF GetSelectColor(void) const
	{
		return m_face.GetSelectColor();
	}

	/**
	 * [�ݒ�] �f�t�H���g�F�ݒ�.
	 *	@param index �C���f�b�N�X�B0�`7 ���w��\�B
	 *	@param c �F
	 *	@return �ݒ�O�̐F
	 */
	COLORREF SetDefaultColor(INDEX index, COLORREF c)
	{
		if ( index >= 0 && index < DEFAULTMAX )
		{
			COLORREF r = m_default[index];
			m_default[index] = c;
			return r;
		}
		return INVALID_INDEX;
	}

	/**
	 * [�擾] �f�t�H���g�F�擾.
	 *	@param index �C���f�b�N�X�B0�`7 ���w��\�B
	 *	@return �F
	 */
	COLORREF GetDefaultColor(INDEX index) const
	{
		if ( index >= 0 && index < DEFAULTMAX )
		{
			return m_default[index];
		}
		return INVALID_INDEX;
	}

protected:

	/**
	 * [�ʒm] ���j���[�`��.
	 *	@param nIDCtl �R���g���[��
	 *	@param lpDrawItemStruct �`����
	 */
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if ( lpDrawItemStruct->CtlType == ODT_MENU )
		{
			HDC dc = lpDrawItemStruct->hDC;
			CRect rc = lpDrawItemStruct->rcItem;
			COLORREF color = lpDrawItemStruct->itemData;
			{
				CDcSelectAssistant dca(dc);
				bool isSel = ((lpDrawItemStruct->itemState & CDIS_SELECTED) != 0);
				::FillRect(dc, &rc, ::GetSysColorBrush(isSel ? COLOR_HIGHLIGHT : COLOR_MENU));
				CBrush brush(color);
				dca.SelectBrush(brush);
				CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
				dca.SelectPen(pen);
				rc.DeflateRect(3, 3);
				::Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
			}
		}
		_super::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
	
	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		SetBitmapChangeTime(0);
		_super::PreSubclassWindow();
		CRect rc;
		_super::GetClientRect(rc);
		CFontHandle f;
		f.Set(80, _T("MS Gothic"));
		COLORREF c = m_face.GetSelectColor();
		m_face.SetFont(f);
		m_face.Resize(rc.Size());
		m_face.SetTextColor(CLR_INVALID);
		m_face.SetSelectColor(CLR_INVALID);
		_super::SetDisableDrawer(m_face);
		m_face.SetTextColor(RGB(0, 0, 0));
		m_face.SetSelectColor(c);
		_super::SetNormalDrawer(m_face);
	}

	/**
	 * [�ʒm] �N���b�N.
	 *		BN_CLICKED ��e�ɒʒm����O�ɃR�[������܂��B
	 */
	virtual void OnClickButton(void)
	{
		_super::OnClickButton();
		CMenu menu;
		CRect rc;
		_super::GetWindowRect(&rc);
		menu.CreatePopupMenu();
		UINT pos = static_cast<UINT>(-1);
		loop ( i, DEFAULTMAX )
		{
			menu.InsertMenu(pos, MF_OWNERDRAW, ITEM_DEFAULTTOP + i, reinterpret_cast<LPCTSTR>(m_default[i]));
		}
		menu.InsertMenu(pos, MF_SEPARATOR);
		menu.InsertMenu(pos, 0, ITEM_OTHER, _T("���̑�..."));
		menu.TrackPopupMenu(0, rc.left, rc.bottom, this, NULL);
		return;
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
		if ( message == WM_DRAWITEM )
		{
			OnDrawItem(wParam, reinterpret_cast<LPDRAWITEMSTRUCT>(lParam));
		}
		else if ( message == WM_COMMAND )
		{
			if ( wParam == ITEM_OTHER )
			{
				CColorDialog dlg(m_face.GetSelectColor());
				if ( dlg.DoModal() == IDOK )
				{
					COLORREF c = dlg.GetColor();
					m_face.SetSelectColor(c);
					m_InsertDefaultColor(c);
					_super::SetNormalDrawer(m_face);
					_super::Invalidate();
					MFCLIB::SendCommandMessage(this, CSBN_SELCHANGE);
				}
			}
			else if ( wParam >= ITEM_DEFAULTTOP && ITEM_DEFAULTTOP + DEFAULTMAX > wParam )
			{
				COLORREF c = m_default[wParam - 2000];
				m_face.SetSelectColor(c);
				m_InsertDefaultColor(c);
				_super::SetNormalDrawer(m_face);
				_super::Invalidate();
				MFCLIB::SendCommandMessage(this, CSBN_SELCHANGE);
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// �f�t�H���g�J���[�ɑ}��
	void m_InsertDefaultColor(COLORREF c)
	{
		if ( m_default[0] == c )
		{
			return;
		}
		loop ( i, m_default.GetSize() )
		{
			if ( c == m_default[i] )
			{
				// �r���ɓ����F���������B
				loop_dn ( j, i )
				{
					m_default[j + 1] = m_default[j];
				}
				m_default[0] = c;
				m_WriteDefault();
				return;
			}
		}
		loop_dn ( i, m_default.GetSize() - 1)
		{
			m_default[i + 1] = m_default[i];
		}
		m_default[0] = c;
		m_WriteDefault();
	}

	/// �f�t�H���g(�A�N�Z�T�[��)������
	void m_WriteDefault(void)
	{
		if ( m_pAccessor == NULL )
		{
			return;
		}
		IAccessor::CSection sec(m_pAccessor, m_accessorSectionName);
		loop ( i, DEFAULTMAX )
		{
			CStr key = CStr::Fmt(_T("c%d"), i + 1);
			sec.WriteDword(key, m_default[i]);
		}
	}

	/// �{�^���t�F�C�X�`��
	class CFaceDrawer : public CNullDrawer
	{
	public:
		/// �R���X�g���N�^
		CFaceDrawer(CFontHandle font = NULL, COLORREF textColor = CLR_INVALID, COLORREF selectColor = CLR_INVALID)
			: m_font(font), m_selectColor(selectColor), m_textColor(textColor)
		{
		}
		// �I��F�擾
		COLORREF GetSelectColor(void) const
		{
			return m_selectColor;
		}
		// �I��F�ݒ�
		void SetSelectColor(COLORREF c)
		{
			m_selectColor = c;
		}
		// �����F�ݒ�
		void SetTextColor(COLORREF c)
		{
			m_textColor = c;
		}
		// �t�H���g�ݒ�
		void SetFont(CFontHandle font)
		{
			m_font = font;
		}
		/**
		 * [�쐬] �N���[���쐬.
		 *		�����Ɠ����������s���N���X���쐬���܂��B
		 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
		 */
		virtual IDrawable* Clone(void) const 
		{
			CFaceDrawer* P = new CFaceDrawer;
			*P = *this;
			return P;
		}
		/**
		 * [�`��] �`��.
		 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
		 *	@note �{�N���X�͉������܂���B
		 *	@param dc �f�o�C�X�R���e�L�X�g
		 *	@param x �w���W
		 *	@param y �x���W
		 */
		virtual void Draw(HDC dc, int x = 0, int y = 0) const
		{
			CDcSelectAssistant dca(dc);
			CRect rc(CPoint(x, y), m_size);
			dca.SelectFont(m_font);
			rc.DeflateRect(5, 5);
			if ( m_font != NULL )
			{
				CRect r = rc;
				CTextDrawer::DrawTextRect(r, dc, DT_RIGHT | DT_VCENTER, m_textColor, _T("��"));
				r.left -= 3;
				r.right = r.left + 2;
				::DrawEdge(dc, &r, EDGE_ETCHED, BF_LEFT | BF_RIGHT);
				rc.right = r.left - 4;
			}
			if ( IS_RGBVALUE(m_selectColor) )
			{
				CBrush brush(m_selectColor);
				CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
				dca.SelectBrush(brush);
				dca.SelectPen(pen);
				::Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
			}
		}
	private:
		COLORREF	m_selectColor;	///< �J���[�@CLR_INVALID�Ȃ�\�����Ȃ�
		COLORREF	m_textColor;	///< �J���[�@CLR_INVALID�Ȃ�G���{�X
		CFontHandle	m_font;			///< �t�H���g
	};

	enum
	{
		DEFAULTMAX = 8,			///< �f�t�H���g��
		ITEM_OTHER = 1000,		///< ���̑����j���[
		ITEM_DEFAULTTOP = 2000,	///< �f�t�H���g�g�b�v
	};
	CFaceDrawer				m_face;					///< �{�^���`����
	CWorkMemT<COLORREF>		m_default;				///< �f�t�H���g
	IAccessor*				m_pAccessor;			///< �A�N�Z�T
	CStr					m_accessorSectionName;	///< �A�N�Z�T�̃Z�N�V������
};



}; //MFC
}; //TNB



