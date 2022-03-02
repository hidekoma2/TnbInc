#pragma once
/**
 *@file
 * ���C�A�E�g�_�C�A���O�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbLayout.h"


/// MFC static �����N�΍�
#define classCLayoutDialog classCDialog



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL LAYOUT
 * �R���g���[�����C�A�E�g�A�C�e���N���X.
 *
 *		CGroupLayout �N���X�ȂǂƘA�g���A�_�C�A���O��̃R���g���[���̃��C�A�E�g��ݒ�A��������N���X�ł��B
 *
 *	@see CLayoutDialog
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcLayoutDialog.h
 *
 *	@date 08/07/17 �V�K�쐬
 */
class CControlLayoutItem : public ILayout
{
	DEFSUPER(ILayout);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param hWnd �Ǘ��E�B���h�E
	 */
	CControlLayoutItem(HWND hWnd) : m_hWnd(hWnd)
	{
		ms_GetSize(m_hWnd, m_size);
	}

	/**
	 * �R���X�g���N�^.
	 *	@param hWnd �Ǘ��E�B���h�E
	 *	@param sz �ŏ��T�C�Y
	 */
	CControlLayoutItem(HWND hWnd, const SIZE& sz) : m_hWnd(hWnd), m_size(sz)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		return new CControlLayoutItem(m_hWnd);
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		���݂̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		return ms_GetSize(m_hWnd, _size);
	}

	/**
	 * [�擾] �ŏ��T�C�Y�擾.
	 *		�ŏ��̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		_size = m_size;
		return true;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		���݂̃T�C�Y��ύX���邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B
	 */
	virtual bool Resize(const SIZE& size)
	{
		::SetWindowPos(m_hWnd, NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		return true;
	}

	/**
	 * [����] ����.
	 *	@note �I�u�W�F�N�g���Ǘ����Ă���T�C�Y�Ǝw��̃T�C�Y�Ƀ��C�A�E�g���܂��B
	 *	@param x  X���W�B-1�Ȃ�w��Ȃ��B
	 *	@param y  Y���W�B-1�Ȃ�w��Ȃ��B���̏ꍇ wParam �Ɋ֌W�Ȃ��AZ�I�[�_�[�̕ύX�͂��܂���B lParam �|�C���^��̒l���ύX���܂���B
	 *	@param wParam W �p�����[�^�B 'HWND' �̏ꍇ�AZ�I�[�_�[�̕ύX�����܂��B
	 *	@param lParam L �p�����[�^�B wParam �� 'HWND' �̏ꍇ�A HWND �̃|�C���^��ێ����Ă��܂��B
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		if ( y >= 0 && wParam == 'HWND' )
		{
			//-- Z�I�[�_�[�ύX
			HWND* pPrevHwnd = reinterpret_cast<HWND*>(lParam);
			ASSERT( ! ::IsBadWritePtr(pPrevHwnd, sizeof(HWND)) );
			::SetWindowPos(m_hWnd, *pPrevHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
			*pPrevHwnd = m_hWnd;
		}
		ms_MoveControlPos(m_hWnd, x, y);
	}

	/**
	 * [�쐬] ���C�A�E�g�A�C�e���쐬.
	 *	@note CControlLayoutItem �̃V���[�g�l�[���ł��B
	 *	@param pWnd �Ǘ��E�B���h�E
	 *	@return ���C�A�E�g�A�C�e��
	 */
	friend CControlLayoutItem Li(CWnd* pWnd);

	/**
	 * [�쐬] ���C�A�E�g�A�C�e���쐬
	 *	@note CControlLayoutItem �̃V���[�g�l�[���ł��B
	 *	@param ctrlId �R���g���[��ID
	 *	@param pParent �e�E�B���h�E
	 *	@return ���C�A�E�g�A�C�e��
	 */
	friend CControlLayoutItem Li(UINT ctrlId, CWnd* pParent);

private:
	// �R���g���[���̃N���C�A���g���W�A�T�C�Y�擾
	static bool ms_GetControlPos(POINT& _pos, HWND hWnd)
	{
		HWND hParent = ::GetParent(hWnd);
		RECT rc;
		if ( hParent != NULL && ::GetWindowRect(hWnd, &rc) )
		{
			POINT po = { rc.left, rc.top };
			if ( ::ScreenToClient(hParent, &po) )
			{
				_pos = po;
				return true;
			}
		}
		return false; 
	}
	/*
	 * �R���g���[���ʒu�ݒ�.
	 *		�R���g���[�����A(�e�E�B���h�E��)�w��̃N���C�A���g���W�Ɉړ����܂��B
	 *	@param hWnd �R���g���[���B
	 *	@param x �N���C�A���g X���W�B-1�Ȃ�ړ��Ȃ��B
	 *	@param y �N���C�A���g Y���W�B-1�Ȃ�ړ��Ȃ��B
	 */
	static bool ms_MoveControlPos(HWND hWnd, int x, int y)
	{
		POINT po;
		if ( ms_GetControlPos(po, hWnd) )
		{
			int xx = (x < 0) ? po.x : x;
			int yy = (y < 0) ? po.y : y;
			::SetWindowPos(hWnd, NULL, xx, yy, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
			return true;
		}
		return false;
	}
	/// �T�C�Y�擾
	static bool ms_GetSize(HWND hWnd, SIZE& _size)
	{
		RECT rc;
		if ( ::GetWindowRect(hWnd, &rc) )
		{
			_size.cx = rc.right - rc.left;
			_size.cy = rc.bottom - rc.top;
			return true;
		}
		_size.cx = 0;
		_size.cy = 0;
		return false;
	}
	HWND	m_hWnd;		///< �Ǘ��E�B���h�E
	SIZE	m_size;		///< �ŏ��T�C�Y
};



// ���C�A�E�g�A�C�e���쐬
inline CControlLayoutItem Li(CWnd* pWnd)
{
	ASSERT( pWnd != NULL );
	return CControlLayoutItem(pWnd->GetSafeHwnd());
}

// ���C�A�E�g�A�C�e���쐬
inline CControlLayoutItem Li(UINT ctrlId, CWnd* pParent)
{
	return Li(pParent->GetDlgItem(ctrlId));
}



/**@ingroup MFCCONTROL LAYOUT
 * ���C�A�E�g�_�C�A���O�N���X.
 *
 *		CGroupLayout �N���X�ȂǂƘA�g���A�_�C�A���O��̃R���g���[���̃��C�A�E�g��ݒ�A��������N���X�ł��B
 *
 *		�{�N���X�� CDialog �̑���Ɍp������ƁA�����C�A�E�g�\�ȃ_�C�A���O�ɂȂ�܂��B
 *		CDialog ���g�����_�C�A���O�����v�̂Ń��\�[�X���쐬���N���X������A CDialog
 *		��{�N���X�ɒu��������`�̎������@��z�肵�Ă��܂��B
 *
 *	@par�g�p�� (���T�C�Y�ɑΉ�����ꍇ)
 *		\code
 *		//= �錾(�w�b�_�t�@�C��)
 *		class CXxxDlg : public CLayoutDialog	// �� CDialog �� CLayoutDialog �ɕύX�B		
 *				// ���� �uCDialog�v �����l
 *		{
 *				:
 *
 *			CStatic	m_txtFirst;	
 *			CStatic	m_txtSecond;
 *			CEdit	m_editFirst;
 *			CEdit	m_editSecond;
 *
 *				:
 *		}
 *
 *		//= �������֐�
 *		BOOL CXxxDlg::OnInitDialog() 
 *		{
 *				:
 *
 *			// ���C�A�E�g�ݒ�
 *			CBoxLayout lay;
 *
 *				:
 *
 *			SetLayout(lay);
 *			// ChangeMinimumSize(); // �K�v�ŏ��T�C�Y�ɂ���ꍇ�͂��̍s��L���ɂ���		
 *
 *				:
 *		}
 *		\endcode
 *
 *	@par�g�p�� (���C�A�E�g���邾���̏ꍇ)
 *		\code
 *		//= �������֐�
 *		BOOL CXxxDlg::OnInitDialog() 
 *		{
 *				:
 *
 *			// ���C�A�E�g�ݒ�
 *			CBoxLayout lay;
 *
 *				:
 *
 *			CLayoutDialog::Decide(this, lay);	//���C�A�E�g
 *
 *				:
 *		}
 *		\endcode
 *
 *	@todo �e�� template �ɂ���ƁA���\�[�X�G�f�B�^�� �_�C�A���O�ł��邱�Ƃ�
 *			�F���ł��Ȃ��Ȃ��Ă��܂����߁A ���� �e�� CDialog �Œ�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcLayoutDialog.h
 *
 *	@date 08/07/17 �V�K�쐬
 */
class CLayoutDialog : public CDialog
{
	DEFSUPER(CDialog);
public:

	/// �R���X�g���N�^
	CLayoutDialog(void) : _super()
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param lpszTemplateName �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X����\�� null �ŏI��镶����B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	CLayoutDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
		: _super(lpszTemplateName, pParentWnd)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param nIDTemplate �_�C�A���O �{�b�N�X �e���v���[�g ���\�[�X�� ID �ԍ��B
	 *	@param pParentWnd �_�C�A���O �I�u�W�F�N�g�����L���� CWnd �^�̐e�E�B���h�E
	 *					�܂��̓I�[�i�[ �E�B���h�E�I�u�W�F�N�g�ւ̃|�C���^�B
	 *					NULL �̏ꍇ�́A�_�C�A���O �I�u�W�F�N�g�̐e�E�B���h�E���A�v���P�[�V�����̃��C�� �E�B���h�E�ɂȂ�܂��B
	 */
	CLayoutDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) 
		: _super(nIDTemplate, pParentWnd)
	{
	}

	/**
	 * [�ݒ�] ���T�C�Y���[�h�ݒ�.
	 *	@note �f�t�H���g�ł́A���������A������������ true ���w�肳��Ă��܂��B
	 *	@param canHorizontalResize false ���w�肷��Ɛ��������̃T�C�Y�ύX���o���Ȃ��Ȃ�܂��B
	 *	@param canVerticalResize false ���w�肷��Ɛ��������̃T�C�Y�ύX���o���Ȃ��Ȃ�܂��B
	 */
	void SetResizeMode(bool canHorizontalResize = true, bool canVerticalResize = true)
	{
		m_param.canHorizontalResize = canHorizontalResize;
		m_param.canVerticalResize = canVerticalResize;
	}

	/**
	 * [�ݒ�] ���C�A�E�g�ݒ�.
	 *	@note �w��̃_�C�A���O�̃A�C�e�����A�I�u�W�F�N�g���Ǘ����Ă��郌�C�A�E�g�ɂ��܂��B
	 *	@param[in,out] lay ���C�A�E�g�B���s��A�T�C�Y���_�C�A���O�̃N���C�A���g�T�C�Y�ɕύX����Ă��܂��B
	 *					�N���[�����L������Ă���̂Ŕj�����Ă��܂��܂���B
	 *	@param zorderTop Z�I�[�_�̐擪���w�肵�܂��B�ʏ� HWND_TOP ���w�肵�܂��B 
	 *					HWND_TOPMOST , HWND_NOTOPMOST , HWND_BOTTOM���w�肷��ƁAZ�I�[�_�̐ݒ�����܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetLayout(ILayout& lay, HWND zorderTop = HWND_TOP)
	{
		m_pLayout = lay.Clone();
		if ( ! m_pLayout.IsNull() && Decide(this, *m_pLayout, zorderTop) )
		{
			CRect winRc;
			_super::GetWindowRect(winRc);
			return !! _super::SetWindowPos(NULL, 0, 0, winRc.Width(), winRc.Height(), SWP_NOMOVE | SWP_NOZORDER);
		}
		return false;
	}

	/**
	 * [�ݒ�] �T�C�Y�ύX.
	 *	@param sz �N���C�A���g�T�C�Y
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool ChangeClientSize(const SIZE& sz)
	{
		CRect winRc;
		CRect cliRc;
		_super::GetWindowRect(winRc);
		_super::GetClientRect(cliRc);
		CSize s = winRc.Size() - cliRc.Size();
		return !! _super::SetWindowPos(NULL, 0, 0, sz.cx + s.cx, sz.cy + s.cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	/**
	 * [�ݒ�] �ŏ��T�C�Y��.
	 *		SetLayout �Őݒ肵�����C�A�E�g�ŕK�v�ȍŏ��T�C�Y�ɃN���C�A���g�T�C�Y��ύX���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool ChangeMinimumSize(void)
	{
		if ( ! m_pLayout.IsNull() )
		{
			CSize sz;
			if ( m_pLayout->GetMinimumSize(sz) )
			{
				return ChangeClientSize(sz);
			}
		}
		return false;
	}

	/**
	 * [����] ����.
	 *	@note SetLayout() �œo�^�������C�A�E�g�� Decide() �����s���܂��B
	 *	@param x  X���W�B-1�Ȃ�w��Ȃ��B
	 *	@param y  Y���W�B-1�Ȃ�w��Ȃ��B
	 *	@param wParam W �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 *	@param lParam L �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 */
	void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		if ( ! m_pLayout.IsNull() )
		{
			m_pLayout->Decide(x, y, wParam, lParam);
		}
	}

	/**
	 * [�ݒ�] ���C�A�E�g�w��.
	 *	@param[in] rect ���C�A�E�g�͈�.
	 *	@param[in,out] lay ���C�A�E�g�B���s��A�T�C�Y�� pWnd �̃N���C�A���g�T�C�Y�ɕύX����Ă��܂��B
	 *	@param zorderTop Z�I�[�_�̐擪���w�肵�܂��B�ʏ� HWND_TOP ���w�肵�܂��B 
	 *					HWND_TOPMOST , HWND_NOTOPMOST , HWND_BOTTOM���w�肷��ƁAZ�I�[�_�̐ݒ�����܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool Decide(const RECT& rect, ILayout& lay, HWND zorderTop = HWND_TOP)
	{
		WPARAM wParam = 0;
		LPARAM lParam = 0;
		if ( zorderTop != HWND_NOTOPMOST && zorderTop != HWND_BOTTOM && zorderTop != HWND_TOPMOST)
		{
			wParam = 'HWND';
			lParam = reinterpret_cast<LPARAM>(&zorderTop);
		}
		SIZE sz = { rect.right - rect.left, rect.bottom - rect.top };
		bool r = lay.Resize(sz);
		lay.Decide(rect.left, rect.top, wParam, lParam);
		return r;
	}

	/**
	 * [�ݒ�] ���C�A�E�g�w��.
	 *	@param[in] pWnd �_�C�A���O.
	 *	@param[in,out] lay ���C�A�E�g�B���s��A�T�C�Y�� pWnd �̃N���C�A���g�T�C�Y�ɕύX����Ă��܂��B
	 *	@param zorderTop Z�I�[�_�̐擪���w�肵�܂��B�ʏ� HWND_TOP ���w�肵�܂��B 
	 *					HWND_TOPMOST , HWND_NOTOPMOST , HWND_BOTTOM���w�肷��ƁAZ�I�[�_�̐ݒ�����܂���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	static bool Decide(CWnd* pWnd, ILayout& lay, HWND zorderTop = HWND_TOP)
	{
		bool r = false;
		if ( ::IsWindow(pWnd->GetSafeHwnd()) )
		{
			CRect rc;
			pWnd->GetClientRect(&rc);
			r = Decide(rc, lay, zorderTop);
			pWnd->Invalidate(FALSE);
		}
		return r;
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
		if ( ! m_pLayout.IsNull() )
		{
			if ( message == WM_SIZE )
			{
				int h = HIWORD(lParam);
				int w = LOWORD(lParam);
				m_pLayout->Resize(CSize(w, h));
				m_pLayout->Decide(0, 0, 0, 0);
				_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
			}
			#ifndef _WIN32_WCE
			{
				if ( message == WM_GETMINMAXINFO )
				{
					MINMAXINFO* P = reinterpret_cast<MINMAXINFO*>(lParam);
					CRect winRc;
					CRect cliRc;
					_super::GetWindowRect(winRc);
					_super::GetClientRect(cliRc);
					CSize sz;
					if ( m_pLayout->GetMinimumSize(sz) )
					{
						CSize s = winRc.Size() - cliRc.Size();
						P->ptMinTrackSize.x = sz.cx + s.cx;
						P->ptMinTrackSize.y = sz.cy + s.cy;
						if ( ! m_param.canHorizontalResize )
						{
							P->ptMaxTrackSize.x = P->ptMinTrackSize.x;
						}
						if ( ! m_param.canVerticalResize )
						{
							P->ptMaxTrackSize.y = P->ptMinTrackSize.y;
						}
					}
				}
			}
			#endif
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:

	/// �p�����[�^
	struct TParam
	{
		bool	canHorizontalResize;		///< �������̃��T�C�Y�\
		bool	canVerticalResize;			///< �������̂�T�C�Y�\
		/// �R���X�g���N�^
		TParam(void) : canHorizontalResize(true), canVerticalResize(true)
		{}
	};
	TParam			m_param;				///< �p�����^
	ILayout::Ptr	m_pLayout;				///< ���C�A�E�g
};



}; // MFC
}; // TNB