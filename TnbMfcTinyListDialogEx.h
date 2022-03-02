#pragma once
/**
 *@file
 * List�E�B���h�E�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcTinyListDialog.h"
#include "TnbThreadDispatch.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �g���ȈՃ��X�g�E�B���h�E
 *
 *		�s�P�ʂŐF���w��o���� ListBox�R���g���[������ʈ�ʂɂ����[�h���X�_�C�A���O�ł��B�B
 *
 *		CTinyListDialog �ɔ�ׁA�{��ʂ̃��b�Z�[�W�L���[��ʓr�p�ӂ��Ă��邽�߁A
 *		���C���_�C�A���O�Ń��b�Z�[�W�|���v�𓮂������ɒ����ԃ��O���o�͂��Ă��A
 *		�{��ʂ̍X�V���s���܂��B
 *
 *	@note �e�E�B���h�E�̕\����\�����j���[ID�� Create() �Ŏw�肷��ƁAShow Hide �ɂ�莩���I�ɕ\���`�F�b�N�� on off ���܂��B
 *			�������A���j���[�A�C�e���I������ Show Hide �͎������ł��܂���B�e�_�C�A���O�̃n���h���� 
 *			xxx.ShowWindow(CTinyListDialog::TOGGLE); �Ƃ��Ă��������B
 *			\code
 *				// CTinyListDialogEx m_log;
 *				void CFooDlg::OnIniDialog(void)
 *				{
 *						;
 *					m_log.Create(CSize(100, 100), GetSafeHwnd(), "title", m_hIcon, ID_MENU_LIST_VIEW);
 *						;
 *				}
 *				void CFooDlg::OnMenuListView(void)
 *				{
 *					m_log.ShowWindow(CTinyListDialog::TOGGLE);
 *				}
 *			\endcode
 *			���j���[ID���w�肵�Ă��Ȃ��ꍇ�A PostString() �ŕ\�������悤�ɂȂ�܂��B
 *
 *	@note �ȉ��̂悤�Ƀ��C����ʂ� LOG �֐���p�ӂ��Ă����ƕ֗��ł��B
 *			\code
 *				enum ELogType
 *				{
 *					RECV	= RGB(204, 255, 204),
 *					SEND	= RGB(204, 255, 255),
 *					NORMAL	= RGB(255, 255, 255),
 *					ERR		= RGB(255,153,153),
 *				};
 *
 *				void CFooDlg::Log(ELogType tt, LPCTSTR lpszFmt, ...)
 *				{
 *					va_list args;
 *					va_start(args, lpszFmt);
 *					CString s;
 *					s.FormatV(lpszFmt, args); 
 *					va_end(args);
 *					m_log.PostString(-1, s, RGB(0, 0, 0), tt);
 *				}
 *
 *				void CFooDlg::Log(LPCTSTR lpszFmt, ...)
 *				{
 *					va_list args;
 *					va_start(args, lpszFmt);
 *					CString s;
 *					s.FormatV(lpszFmt, args); 
 *					va_end(args);
 *					m_log.PostString(-1, s, RGB(0, 0, 0), NORMAL);
 *				}
 *			\endcode
 *
 *	@note �p������ ���j���[���ڂ�ύX���邱�Ƃ��o���܂��B
 *
 *	@see CColorListBox
 *	@see CTinyListDialog
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcTinyListDialogEx.h
 * 
 *	@date 11/02/03	�V�K�쐬
 */
class CTinyListDialogEx : public CTinyListDialog, CThreadDispatch::IListener
{
	DEFSUPER(CTinyListDialog);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param isColorMode true �Ȃ�J���[���[�h�B false �Ȃ甒�����[�h�ł��B
	 */
	CTinyListDialogEx(bool isColorMode = true) : _super(isColorMode)
	{
	}

	/**
	 * [�쐬] �E�B���h�E�쐬.
	 *	@note ��\����Ԃō쐬����܂��B
	 *	@param size �E�B���h�E�傫��
	 *	@param hFont �t�H���g
	 *	@param hParentWnd �e�E�B���h�E
	 *	@param lpszTitle �E�B���h�E�^�C�g��
	 *	@param hIcon �E�B���h�E�A�C�R��
	 *	@param parentMenuItem �e�E�B���h�E�̕\����\�����j���[�B�ȗ��\�B
	 *	@retval true �쐬����
	 *	@retval false ���s
	 */
	bool Create(const CSize& size, HFONT hFont, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		m_size = size;
		m_hFont = hFont;
		m_hParentWnd = hParentWnd;
		m_title = lpszTitle;
		m_hIcon = hIcon;
		m_parentMenuItem = parentMenuItem;
		m_thread.SetListener(this);
		#ifdef _DEBUG
			m_thread.Start(_T("TinyListDialogEx::MessagePump"));
		#else
			m_thread.Start();
		#endif
		return IsWindow();
	}

	/**
	 * [�쐬] �E�B���h�E�쐬.
	 *	@note ��\����Ԃō쐬����܂��B
	 *	@param size �E�B���h�E�傫��
	 *	@param hParentWnd �e�E�B���h�E
	 *	@param lpszTitle �E�B���h�E�^�C�g��
	 *	@param hIcon �E�B���h�E�A�C�R��
	 *	@param parentMenuItem �e�E�B���h�E�̕\����\�����j���[�B�ȗ��\�B
	 *	@retval true �쐬����
	 *	@retval false ���s
	 */
	bool Create(const CSize& size, HWND hParentWnd, LPCTSTR lpszTitle, HICON hIcon, UINT parentMenuItem = 0)
	{
		HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hParentWnd, WM_GETFONT, 0, 0));
		return Create(size, hFont, hParentWnd, lpszTitle, hIcon, parentMenuItem);
	}

	/**
	 * [����] �E�B���h�E�̔j��.
	 */
	void Destroy(void)
	{
		_super::Destroy();
		m_thread.Stop();
	}

protected:

	/**
	 * [�ʒm] �J�n�ʒm.
	 *	��M�X���b�h���J�n�����ƁA�ʒm����܂��B
	 */
	virtual void OnThreadDispatchSetup(void) 
	{
		_super::Create(m_size, m_hFont, m_hParentWnd, m_title, m_hIcon, m_parentMenuItem);
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
		return true;
	}

private:
	CSize				m_size;				///< �T�C�Y
	HFONT				m_hFont;			///< �t�H���g
	HWND				m_hParentWnd;		///< �e�E�B���h�E
	CStr				m_title;			///< �^�C�g��
	HICON				m_hIcon;			///< �A�C�R��
	UINT				m_parentMenuItem;	///< �e�̃��j���[�A�C�e��
	CThreadDispatch		m_thread;			///< �X���b�h
};



}; // MFC
}; // TNB
