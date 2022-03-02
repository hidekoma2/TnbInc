#pragma once
/**
 *@file
 * �g�� SetRedraw �֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �g�� SetRedraw �T�|�[�g�N���X
 *
 *		WM_SETREDRAW ���b�Z�[�W���J�E���^���ɂ���A�h�C���ł��B
 *
 *		GetRedraw() , SetRedrawAbsolutely() ���ǉ�����܂��B
 *
 *	@note WM_SETREDRAW ���g������܂��B
 *				- lParam == SET_REDRAW_COOKIE && wParam == 100 �Ȃ� �`�悵�Ȃ��J�E���^�� 0 �ɐݒ�B
 *				- lParam == SET_REDRAW_COOKIE && wParam == 101 �Ȃ� �`�悵�Ȃ��J�E���^�擾�B
 *				- wParam == 0 �Ȃ� �`�悵�Ȃ��J�E���^��+1�B
 *				- wParam == 1 �Ȃ� �`�悵�Ȃ��J�E���^�� -1�B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSetRedrawEx.h
 * 
 *	@date 10/06/01	�V�K�쐬
 */
template<typename TYP>
class CSetRedrawExAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// �V���{����`
	enum
	{
		SET_REDRAW_COOKIE = 'tsRx',		///< �N�b�L�[
	};

	/**
	 * [�ݒ�] �ĕ`�拖�E�֎~�ݒ�.
	 *	@note ������A�h�I������ƁA�J�E���^���ɂȂ�܂��B�܂�A�֎~���R��s������A
	 *		�����R�K���Ȃ��ƁA�ĕ`�悳���悤�ɂȂ�܂���B
	 *	@param bRedraw TURE �Ȃ�ĕ`��������܂��B FALSE �Ȃ�֎~���܂��B
	 */
	void SetRedraw(BOOL bRedraw = TRUE)
	{
		_super::SetRedraw(bRedraw);
	}

	/**
	 * [�ݒ�] �ĕ`�拖��.
	 *	@note �J�E���^�����������A���Ȃ炸�ĕ`�悳���悤�ɂ��܂��B
	 */
	void SetRedrawAbsolutely(void)
	{
		ASSERT( ::IsWindow(_super::GetSafeHwnd()) );
		::SendMessage(_super::GetSafeHwnd(), WM_SETREDRAW, 100, SET_REDRAW_COOKIE);
	}

	/**
	 * [�擾] �ĕ`���Ԏ擾.
	 *	@retval TRUE �ĕ`�拖��ԁB
	 *	@retval FALSE �ĕ`��֎~��ԁB
	 */
	BOOL GetRedraw(void) const
	{
		LRESULT r = ::SendMessage(_super::GetSafeHwnd(), WM_SETREDRAW, 101, SET_REDRAW_COOKIE);
		return r == 0;
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
		if ( message == WM_SETREDRAW )
		{
			if ( lParam == SET_REDRAW_COOKIE )
			{
				if ( wParam == 100 )
				{
					// �`�悵�Ȃ��J�E���^ = 0�B
					m_count = 0;
					return _super::WindowProc(message, TRUE, 0);
				}
				else if ( wParam == 101 )
				{
					// �`�悵�Ȃ��J�E���^�擾�B
					return m_count;
				}
				else
				{
					return 0;
				}
			}
			if ( wParam == 0 )
			{
				// �`�悵�Ȃ��J�E���^ +1
				m_count++;
				if ( m_count == 1 )
				{
					return _super::WindowProc(message, FALSE, 0);
				}
				return 0;
			}
			else
			{
				// �`�悵�Ȃ��J�E���^ -1
				if ( m_count > 0 )
				{
					m_count--;
					if ( m_count == 0 )
					{
						return _super::WindowProc(message, TRUE, 0);
					}
				}
				return 0;
			}
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
		m_count = 0;
		_super::PreSubclassWindow();
	}
	/**
	 * [�ʒm] special pre-creation and window rect adjustment hooks.
	 *		Create���R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param cs �\�z�f�[�^.
	 *	@return ����.
	 */
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
	{
		m_count = 0;
		return _super::PreCreateWindow(cs);
	}
	/**
	 * [�ʒm] �j���I���ʒm
	 */
	virtual void PostNcDestroy(void) 
	{
		m_count = 0;
		_super::PostNcDestroy();
	}

private:
	int			m_count;	///< �`�悵�Ȃ��J�E���^�B 0 �Ȃ�`�拖���� 1 �ȏ�Ȃ�`���~
};



}; // MFC
}; // TNB
