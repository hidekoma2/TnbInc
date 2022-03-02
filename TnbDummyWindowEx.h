#pragma once
/**
 *@file
 * �g���_�~�[�E�B���h�E�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDummyWindow.h"
#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �g���_�~�[�E�B���h�E�N���X
 *
 *		�ȈՓI�ȃE�B���h�E���쐬���܂��B MFC ���g�p���Ă��܂���B
 *		�܂��A���b�Z�[�W�L���[��ʃX���b�h�ɍ쐬���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDummyWindowEx.h
 *
 *	@date 11/08/31	�V�K�쐬
 *	@date 11/09/07	VC++2005 �Ή�
 */
class CDummyWindowEx : public CDummyWindow
{
	DEFSUPER(CDummyWindow);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param cmc �����Ŏg�p���郁�b�Z�[�W�R�[�h�B�ȗ������ 0xBFFF ���g�p����܂��B
	 */
	CDummyWindowEx(UINT cmc = 0xBFFF)
	{
		m_in.m_pPartner = this;
		m_in.m_createMessage = cmc;
	}

	/**
	 * [����] �E�B���h�E�̍쐬�i�e���v���[�g�g�p�j.
	 *		���\�[�X�̃e���v���[�g���g�p���ăE�B���h�E���쐬���܂��B
	 *	@note ��̃C���X�^���X�ŊǗ��ł���E�B���h�E�͈�����ł��B
	 *		�����쐬���悤�Ƃ���ƁA��̃E�B���h�E���j������܂��B
	 *	@param I ���X�i�[
	 *	@param templateId �_�C�A���O�̃��\�[�X�_�e���v���[�g�h�c���w�肵�܂��B
	 *	@param hParent �_�~�[�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool CreateByTemplate(CDummyWindowEx::IListener* I, UINT templateId, HWND hParent = NULL)
	{
		Destroy();
		m_in.m_pListener = I;
		m_in.m_templateId = templateId;
		m_thread.SetListener(&m_in);
		#ifdef _DEBUG
			m_thread.Start(_T("CDummyWindowEx::MessagePump"));
		#else
			m_thread.Start();
		#endif
		m_thread.PostMessage(m_in.m_createMessage);
		loop ( i, 10 )
		{
			if ( IsWindow() )
			{
				return true;
			}
			::Sleep(100);
		}
		Destroy();
		return false;
	}

	/**
	 * [����] �E�B���h�E�̔j��.
	 */
	void Destroy(void)
	{
		m_thread.Stop();
	}

private:
	/// ���݁A���T�|�[�g�ł��B
	bool Create(CDummyWindow::IListener* I, LPCTSTR lpszTitle = NULL, LPCTSTR lpszClassName = NULL, DWORD dwStyle = WS_OVERLAPPEDWINDOW, HMENU hMenu = NULL, HWND hParent = NULL);
	/// ���݁A���T�|�[�g�ł��B
	void SetBrush(HBRUSH b);

	/// �����Ǘ�
	class CInner : public CThreadDispatch::IListener
	{
	public:
		CDummyWindowEx*				m_pPartner;			///< �p�[�g�i�[
		CDummyWindow::IListener*	m_pListener;		///< ���X�i�[�L���p
		UINT						m_templateId;		///< �e���v���[�g�h�c
		UINT						m_createMessage;	///< �쐬���b�Z�[�W�R�[�h
		/**
		 * [�ʒm] �J�n�ʒm.
		 *		��M�X���b�h���J�n�����ƁA�ʒm����܂��B
		 *	@note CThreadDispatch::Start() ����Ԃ�O�ɍ쐬���ꂽ�X���b�h�ォ��Ă΂�܂��B
		 */
		virtual void OnThreadDispatchSetup(void) 
		{
		}
		/**
		 * [�ʒm] ���b�Z�[�W��M.
		 *		�Ō�͕K�� nMsg �� WM_QUIT �������Ēʒm����܂��B
		 *	@param nMsg ���b�Z�[�W
		 *	@param wParam WPARAM
		 *	@param lParam LPARAM
		 *	@retval true	����
		 *	@retval false	�ؒf�v��
		 */
		virtual bool OnThreadDispatchReceive(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			if ( nMsg == m_createMessage )
			{
				m_pPartner->_super::CreateByTemplate(m_pListener, m_templateId, NULL);
			}
			else if ( nMsg == WM_QUIT )
			{
				m_pPartner->_super::Destroy();
			}
			return true;
		}
	};

	CThreadDispatch		m_thread;	///< ���b�Z�[�W�L���[
	CInner				m_in;		///< �����Ǘ�
};



};///TNB
