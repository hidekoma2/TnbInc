#pragma once
/**
 *@file
 * ������ݒ�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ������ݒ�T�|�[�g�N���X
 *
 *		���̃X���b�h��������Ȃ� SetWindowText �ł���悤�ɂ���A�h�C���ł��B
 *
 *	@note SetWindowText �̒���ɁA GetWindowText() ���s���ƁA���҂̕����񂪎擾�ł��Ȃ����Ƃ�
 *		����܂�(PostMessage�ŏ�������ł��邽�߂ɁA�x�������邽�߂ł�)�B
 *
 *	@par�g�p��
 *		\code
 *		//= �錾
 *		typedef CSetTextAddinT<CStatic> CStaticSt;
 *				;
 *		class CXXXX : public CDialog
 *		{
 *				;
 *			CStaticSt	m_staticText1;		// <- ���X CStatic�����������o������������
 *				;
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSetText.h
 * 
 *	@date 09/11/05	�V�K�쐬
 */
template<typename WND = CStatic>
class CSetTextAddinT : public WND
{
	DEFSUPER(WND);
public:
	
	/// �R���X�g���N�^
	CSetTextAddinT(void) : m_postCount(0), m_isValid(false)
	{
	}

	/**
	 * [�ݒ�] ������ݒ�.
	 *	@note �ʃX���b�h����g�p����ƁA Post���b�Z�[�W�𓊂��܂��B�B
	 *	@param lpszString ������B
	 */
	void SetWindowText(LPCTSTR lpszString)
	{
		if ( ::IsWindow(m_hWnd) && m_isValid )
		{
			TTRACE2("SetWindowText tid = %X, %X\n", m_mainThreadId, ::GetCurrentThreadId());
			if ( m_mainThreadId != ::GetCurrentThreadId() )
			{
				CString* P = new CString(lpszString);
				m_postCount++;
				if ( ! _super::PostMessage(WM_TNB_POSTTEXT, 0, reinterpret_cast<LPARAM>(P)) )
				{
					delete P;
					m_postCount--;
				}
				return;
			}
			_super::SetWindowText(lpszString);
		}
	}

protected:

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd �̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_TNB_POSTTEXT:
			{
				CString* P = reinterpret_cast<CString*>(lParam);
				if ( ::IsWindow(m_hWnd) )
				{
					_super::SetWindowText(P->operator LPCTSTR());
				}
				delete P;
				m_postCount--;
				return 0;
			}
			break;
		case WM_DESTROY:
			{
				MSG msg;
				m_isValid = false;
				while ( ::PeekMessage(&msg, m_hWnd, WM_TNB_POSTTEXT, WM_TNB_POSTTEXT, PM_REMOVE) )
				{
					if ( msg.message == WM_TNB_POSTTEXT )
					{
						CString* P = reinterpret_cast<CString*>(msg.lParam);
						delete P;
						m_postCount--;
					}
				}
				ASSERT( m_postCount == 0 );
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
		m_isValid = true;
		m_mainThreadId = ::GetCurrentThreadId();
		_super::PreSubclassWindow();
	}

private:
	enum { WM_TNB_POSTTEXT = WM_APP };
	size_t			m_postCount;		///< �}�����J�E���^
	DWORD			m_mainThreadId;		///< �X���b�hID
	bool			m_isValid;			///< �L���t���O
};



}; // MFC
}; // TNB
