#pragma once
/**
 *@file
 * �x���ĕ`��R���g���[�������֌W�̃w�b�_
 *
 *		�����̏��̍X�V�̍ہA�`��̃^�C�~���O��x�点�āA�܂Ƃ߂ĕ`�悷��i�������y���o����j�@�\��񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcSetRedrawEx.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �x���ĕ`��A�h�C��
 *
 *		inline �ȃR���g���[���ł��B
 *
 *		�����̏��̍X�V�̍ہA�`��̃^�C�~���O��x�点�āA�܂Ƃ߂ĕ`�悷��i�������y���o����j�@�\��񋟂��܂��B
 *
 *	@param TYP �v�f�̌^�BCWnd �̃T�u�N���X�ł���K�{������܂��B
 *
 *	@par�g�p��
 *		\code
 *		//= �^�錾 (typedef���Ă����Ȃ��� Wizard�ŃG���[�ɐ���܂�)
 *		typedef CDelayedRedrawAddinT<CStatic> CDrStatic;	
 *
 *		//= �_�C�A���O�N���X�錾
 *	 	CDrStatic	m_text;		// <- ���X CStatic�����������o������������		
 *
 *		\endcode
 *		
 *	@par�K�v�t�@�C��
 *			TnbMfcDelayedRedraw.h
 * 
 *	@date 10/02/04 �V�K�쐬
 *	@date 10/11/01 CWnd::RedrawWindow() �̃t���O�A�ύX�iCE�΍�j�B
 */
template<typename TYP = CStatic>
class CDelayedRedrawAddinT : public CSetRedrawExAddinT<TYP>
{
	DEFSUPER(CSetRedrawExAddinT<TYP>);
public:

	/// �R���X�g���N�^
	CDelayedRedrawAddinT(void) : m_postDrawTiming(200), m_lastDrawInterval(100)
	{
	}

	/**
	 * [�ݒ�] �^�C�~���O�ݒ�.
	 *	@param postDrawTiming ���e�X�V�������Ă���ԁA�ĕ`�悷��^�C�~���O(ms)�B(def.200)
	 *	@param lastDrawInterval ���e�X�V���Ђƒi�������Ɣ��f���鎞��(ms)�B(def.100)
	 */
	void SetTiming(DWORD postDrawTiming, DWORD lastDrawInterval)
	{
		m_postDrawTiming = postDrawTiming;
		m_lastDrawInterval = lastDrawInterval;
	}
	
	/**
	 * [�ݒ�] �x���ĕ`��w��.
	 *		���e�X�V���A���̑O�ɂ��̃��\�b�h���R�[�����Ēu���܂��B
	 */
	void DelayedRedraw(void)
	{
		if ( _super::GetRedraw() )
		{
			_super::SetRedraw(FALSE);
		}
		_super::SetTimer(TIMERID_LAST, m_lastDrawInterval, NULL);
		if ( ! m_isOnRedrawTimer )
		{
			m_isOnRedrawTimer = true;
			_super::SetTimer(TIMERID_POST, m_postDrawTiming, NULL);
		}
	}

	/**
	 * [�ݒ�] �E�B���h�E�^�C�g���ݒ�.
	 *	@param lpszString ������.
	 */
	void SetWindowText(LPCTSTR lpszString)
	{
		DelayedRedraw();
		_super::SetWindowText(lpszString);
	}

	/**
	 * [����] �ĕ`��.
	 *	@note �x���`�悹���A�����ɕ`��w�����܂��B
	 */
	void UpdateWindow(void)
	{
		m_isOnRedrawTimer = false;
		_super::KillTimer(TIMERID_POST);
		_super::KillTimer(TIMERID_LAST);
		_super::SetRedraw(TRUE);
		_super::Invalidate(FALSE);
		_super::UpdateWindow();
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		m_isOnRedrawTimer = false;
		_super::PreSubclassWindow();
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
		if ( message == WM_TIMER )
		{
			if ( wParam == TIMERID_POST )
			{
				_super::KillTimer(TIMERID_POST);
				m_isOnRedrawTimer = false;
				_super::SetRedraw(TRUE);
				_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				_super::SetRedraw(FALSE);
				_super::SetTimer(TIMERID_LAST, m_lastDrawInterval, NULL);
			}
			else if ( wParam == TIMERID_LAST )
			{
				// �X�V�����Ȃ��� 100ms������
				_super::KillTimer(TIMERID_LAST);
				_super::SetRedraw(TRUE);
				_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	enum
	{
		TIMERID_POST = 20001,		///< ���X�`��^�C�}�[
		TIMERID_LAST = 20002,		///< �ŏI�`��^�C�}�[
	};
	bool	m_isOnRedrawTimer;		///< �ĕ`��^�C�}�[ON�H
	DWORD	m_postDrawTiming;		///< ���X�`�悷��^�C�~���O
	DWORD	m_lastDrawInterval;		///< �ŏI�`�掞��
};



}; //MFC
}; //TNB



