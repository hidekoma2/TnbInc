#pragma once
/**
 *@file
 * �h���b�O�t���`����ListCtrl�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcDrawingListCtrl.h"
#include "TnbMfcDrag.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �h���b�O�t���`����ListCtrl�R���g���[��
 *
 *		�s�P�ʂŕ\������`������w��o���܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�R���g���[���̃v���p�e�B�́u�I�[�i�[�`��(O)�v���u�ρv�ɂ��Ă����Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDragDrawingListCtrl.h
 * 
 *	@date 07/12/07 �V�K�쐬
 *	@date 08/01/16 �h���b�O�ɂ��X�N���[�����x�𒲐��B
 */
class CDragDrawingListCtrl : public CDragAddinT<CDrawingListCtrl>
{
	DEFSUPER(CDragAddinT<CDrawingListCtrl>);
	int		m_itemHeight;		///< �A�C�e���̍���
	int		m_headerHeight;		///< �w�b�_�[�̍���
	int		m_ctrlId;			///< ���z�R���g���[��ID
	int		m_scrollCount;		///< �h���b�O�X�N���[���J�E���^
	/// �A�C�e���̍����擾
	int GetItemHeight(void)
	{
		if ( m_itemHeight < 0 )
		{
			CRect rc;
			GetItemRect(0, &rc, LVIR_BOUNDS);
			m_itemHeight = rc.Height();
		}
		return m_itemHeight;
	}
	/// �w�b�_�[�̍����擾
	int GetHeaderHeight(void)
	{
		if ( m_headerHeight < 0 )
		{
			CRect rc;
			GetHeaderCtrl()->GetClientRect(rc);
			m_headerHeight = rc.Height();
		}
		return m_headerHeight;
	}

protected:

	/**
	 * [�擾] ���z�R���g���[��ID�x�[�X�擾.
	 *	@return �x�[�X�B
	 */
	int GetVirtualCtrlID(void) const
	{
		return m_ctrlId;
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
		if ( message == WM_LBUTTONDOWN )
		{
			CPoint po;
			::GetCursorPos(&po);
			ScreenToClient(&po);
			if ( _super::HitTest(po, NULL) >= 0 )
			{
				CListCtrl::WindowProc(message, wParam, lParam);
			}
		}
		else if ( IsNofityMessage(message) && wParam == static_cast<WPARAM>(MAKELONG(CANDROP, GetDragGroupID())) )
		{
			if ( ! _super::CanDropping() )
			{
				return false;
			}
			TDropParam* P = reinterpret_cast<TDropParam*>(lParam);	//�N���C�A���g���W���i�[����Ă���
			if ( P->point.y < GetHeaderHeight() + 2 )
			{
				return false;
			}
			int h = GetItemHeight();
			CRect rc;
			_super::GetClientRect(&rc);
			LONG scrollHeight = 0;
			if ( P->point.y < GetHeaderHeight() + 2 + h )
			{
				if ( _super::GetTopIndex() > 0 && ++m_scrollCount > 4 )
				{
					scrollHeight = -h;
				}
			}
			else if ( rc.bottom - h < P->point.y )
			{
				int i1 = _super::GetItemCount() - _super::GetTopIndex();
				int i2 = _super::GetCountPerPage();
				if ( i1 > i2 && ++m_scrollCount > 4 )
				{
					scrollHeight = h;
				}
			}
			if ( scrollHeight != 0 )
			{
				m_scrollCount = 0;
				_super::Scroll(CSize(0, scrollHeight));
				_super::Invalidate(false);
			}
			int r = _super::HitTest(P->point, NULL);
			if ( r >= 0 )
			{
				RECT rc;
				_super::GetItemRect(r, &rc, LVIR_BOUNDS);
				_DeleteObject(P->hRgn);
				P->hRgn = ::CreateRectRgnIndirect(&rc);
				ASSERT1( m_ctrlId + r < 0x10000, "CDrawingListCtrlDg", "���z�R���g���[��ID�� WORD �ŕ\���ł��Ȃ��l�ɂȂ��Ă��܂��܂���(%u)�B", m_ctrlId + r );
				P->ctrlID = static_cast<WORD>(m_ctrlId + r);
				return true;
			}
			return false;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] �h���b�O�J�n.
	 *	@param[in,out] _rc �A�C�e���͈́Bin�� ::GetWindowRect() �����͈͂������Ă��܂��iWindow���W�j�B
	 *	@param[in] po �}�E�X�̃|�W�V�����iWindow���W�j�B
	 *	@retval 0 �h���b�N�o���Ȃ�.
	 *	@retval 1�ȏ� �R���g���[��ID
	 */
	virtual int OnBeginDrag(RECT& _rc, const POINT& po)
	{
		CPoint p = po;
		ScreenToClient(&p);
		int r = HitTest(p, NULL);
		if ( r >= 0 )
		{
			if ( ! ::DragDetect(m_hWnd, p) )
			{
				return 0;
			}
//			UINT s = LVIS_SELECTED | LVIS_FOCUSED;
//			SetItemState(r, s, s);
			GetItemRect(r, &_rc, LVIR_BOUNDS);
			ClientToScreen(&_rc);
			//void SetDraggingBitmap(CBitmapHandle bmp)
			return m_ctrlId + r;
		}
		return 0;
	}	


public:

	/// �R���X�g���N�^
	CDragDrawingListCtrl(void) 
		: _super(), m_ctrlId(0x8000), m_itemHeight(-1), m_headerHeight(-1), m_scrollCount(0)
	{
	}

	/// �f�X�g���N�^
	~CDragDrawingListCtrl(void)
	{
	}

	/**
	 * [�ݒ�] ���z�R���g���[��ID�x�[�X�ݒ�.
	 *	@note ���ۂ̃R���g���[��ID�ł͂Ȃ��A���z�I�ɃR���g���[��ID���g���A
	 *			�eItem�̎��ʂ�����悤�ɂ��܂��B
	 *			�x�[�X�{���X�g�Ɋ܂܂�鐔�� WORD �ŕ\���ł���͈͂�
	 *			�����Ȃ��悤�ɂ��Ă��������B
	 *	@note �f�t�H���g�ł́A 0x8000 �ɂȂ��Ă��܂��B
	 *	@param base �x�[�X�B
	 */
	void SetVirtualCtrlID(int base)
	{
		m_ctrlId = base;
	}
};



}; // MFC
}; // TNB
