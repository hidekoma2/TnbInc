#pragma once
/**
 *@file
 * �t�@�C���I���_�C�A���O�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#if (_WIN32_WINNT >= 0x0500) && defined(CFileDialog) && defined(__MSVCPP6__)

#undef CFileDialog

// commdlg.h �� OPENFILENAME �Ɠ���
struct TOpenFileName
{
   DWORD        lStructSize;
   HWND         hwndOwner;
   HINSTANCE    hInstance;
   LPCTSTR       lpstrFilter;
   LPTSTR        lpstrCustomFilter;
   DWORD        nMaxCustFilter;
   DWORD        nFilterIndex;
   LPTSTR        lpstrFile;
   DWORD        nMaxFile;
   LPTSTR        lpstrFileTitle;
   DWORD        nMaxFileTitle;
   LPCTSTR       lpstrInitialDir;
   LPCTSTR       lpstrTitle;
   DWORD        Flags;
   WORD         nFileOffset;
   WORD         nFileExtension;
   LPCTSTR       lpstrDefExt;
   LPARAM       lCustData;
   LPOFNHOOKPROC lpfnHook;
   LPCTSTR       lpTemplateName;
};

// AfxDlgs.h �̂���Ɠ���
class CFileDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CFileDialog)

public:
// Attributes
	TOpenFileName m_ofn; // open file parameter block

// Constructors
	CFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

// Operations
	virtual int DoModal();

	// Helpers for parsing file name after successful return
	// or during Overridable callbacks if OFN_EXPLORER is set
	CString GetPathName() const;  // return full path and filename
	CString GetFileName() const;  // return only filename
	CString GetFileExt() const;   // return only ext
	CString GetFileTitle() const; // return file title
	BOOL GetReadOnlyPref() const; // return TRUE if readonly checked

	// Enumerating multiple file selections
	POSITION GetStartPosition() const;
	CString GetNextPathName(POSITION& pos) const;

	// Helpers for custom templates
	void SetTemplate(UINT nWin3ID, UINT nWin4ID);
	void SetTemplate(LPCTSTR lpWin3ID, LPCTSTR lpWin4ID);

	// Other operations available while the dialog is visible
	CString GetFolderPath() const; // return full path
	void SetControlText(int nID, LPCSTR lpsz);
	void HideControl(int nID);
	void SetDefExt(LPCSTR lpsz);

// Overridable callbacks
protected:
	friend UINT CALLBACK _AfxCommDlgProc(HWND, UINT, WPARAM, LPARAM);
	virtual UINT OnShareViolation(LPCTSTR lpszPathName);
	virtual BOOL OnFileNameOK();
	virtual void OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);

	// only called back if OFN_EXPLORER is set
	virtual void OnInitDone();
	virtual void OnFileNameChange();
	virtual void OnFolderChange();
	virtual void OnTypeChange();

// Implementation
#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bOpenFileDialog;       // TRUE for file open, FALSE for file save
	CString m_strFilter;          // filter string
						// separate fields with '|', terminate with '||\0'
	TCHAR m_szFileTitle[64];       // contains file title after return
	TCHAR m_szFileName[_MAX_PATH]; // contains full path name after return

	OPENFILENAME*  m_pofnTemp;

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
#endif // _WIN32_WINNT >= 0x0500
#endif //_TnbDOXYGEN



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL FILE
 * �t�@�C���I���R�����_�C�A���O
 *
 *		������Ǘ��o�b�t�@���g�����܂��B
 *		�����I�����s�Ȃ��ۂɗL���B
 *
 *	@par�g�p�� - �ۑ���t�@�C�����w�肵�����ꍇ
 *		\code
 *		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
 *		LPCTSTR lpszFilter = _T("FISID�t�@�C�� (*.fisid)|*.fisid|���ׂẴt�@�C�� (*.*)|*.*||");
 *		CFileDialogEx d(FALSE, _T("*.fisid"), _T(""), dwFlags, lpszFilter, this);
 *		if ( d.DoModal() != IDOK )
 *		{
 *			return;
 *		}
 *		CString fileName = d.GetPathName();
 *			:
 *		\endcode
 *
 *	@par�g�p�� - �Ǎ����t�@�C�����w�肵�����ꍇ
 *		\code
 *		DWORD dwFlags = OFN_HIDEREADONLY;
 *		LPCTSTR lpszFilter = _T("FISID�t�@�C�� (*.fisid)|*.fisid|���ׂẴt�@�C�� (*.*)|*.*||");
 *		CFileDialogEx d(TRUE, _T("*.fisid"), _T(""), dwFlags, lpszFilter, this);
 *		if ( d.DoModal() != IDOK )
 *		{
 *			return;
 *		}
 *		CString fileName = d.GetPathName();
 *			:
 *		\endcode
 *
 *	@attention VC++6 �̊��ŁASDK �����Ă���ꍇ�A CFileDialog ���w�b�_�ƃ����^�C����
 *		���ꂪ�������Ă��܂����߁A�ŏ��� TnbHotWinver.h ���C���N���[�h���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcFileDialogEx.h
 * 
 *	@date 06/12/01	�V�K�쐬
 *	@date 10/01/18	�R�����g�ǉ�
 *	@date 10/04/19	�E���̃��T�C�Y�{�b�N�X��HIDE�ɂ���悤�ɂ����B
 *	@date 10/07/12	��������Ńf�X�g���N�^�ŗ�O���������錏���C���B
 *	@date 14/05/25	SetInitialPath() ��V�K�ǉ��I
 */
class CFileDialogEx : public CFileDialog
{
	DEFSUPER(CFileDialog);
	enum { E_BufferLength = 100000 };
public:

	/**
	 * �R���X�g���N�^
	 *	@param bOpenFileDialog	[�t�@�C�����J��] �_�C�A���O �{�b�N�X���\�z����Ƃ��� TRUE�A
	 *							[�t�@�C������t���ĕۑ�] �_�C�A���O �{�b�N�X���\�z����Ƃ��� FALSE ��ݒ肵�܂��B
	 *	@param lpszDefExt	����̃t�@�C���g���q�B�t�@�C�����p�̃G�f�B�b�g �{�b�N�X�ɁA
	 *						���[�U�[���g���q��t�����Ƀt�@�C��������͂����Ƃ��́A
	 *						lpszDefExt �Ɏw�肵���g���q�������I�Ƀt�@�C�����ɒǉ�����܂��B
	 *						���̃p�����[�^�� NULL ���w�肷��ƁA�g���q�͒ǉ�����܂���B
	 *	@param lpszFileName	�t�@�C�����p�̃G�f�B�b�g �{�b�N�X�ɏ����\�������t�@�C�����B
	 *						NULL ���w�肷��ƁA�����\������܂���B
	 *	@param dwFlags	�_�C�A���O �{�b�N�X���J�X�^�}�C�Y���邽�߂̃t���O�� 1 �ȏ�g�ݍ��킹�B\n
	 *		<table>
	 *		<tr><td>�V���{��</td><td>���e</td></tr><tr><td>
	 *		OFN_READONLY	</td><td>
	 *			�ǂ݂Ƃ��p�̃`�F�b�N�̐ݒ�E�擾</td></tr><tr><td>
	 *		OFN_OVERWRITEPROMPT</td><td>
	 *			�d�ˏ����̊m�F�_�C�A���O�̕\��</td></tr><tr><td>
	 *		OFN_HIDEREADONLY</td><td>
	 *			�ǂ݂Ƃ��p�`�F�b�N�{�b�N�X��\�����Ȃ�</td></tr><tr><td>
	 *		OFN_NOCHANGEDIR</td><td>
	 *			�J�����g�f�B���N�g����ύX���Ȃ�</td></tr><tr><td>
	 *		OFN_SHOWHELP</td><td>
	 *			�w���v�{�^����\������</td></tr><tr><td>
	 *		OFN_ENABLEHOOK</td><td>
	 *			�t�b�N�֐����g��</td></tr><tr><td>
	 *		OFN_ENABLETEMPLATE</td><td>
	 *			�_�C�A���O�e���v���[�g���g��</td></tr><tr><td>
	 *		OFN_ENABLETEMPLATEHANDLE</td><td>
	 *			�_�C�A���O�e���v���[�g�n���h�����g��</td></tr><tr><td>
	 *		OFN_NOVALIDATE</td><td>
	 *			�s���t�@�C���������i�t�b�N�֐��g�p���j</td></tr><tr><td>
	 *		OFN_ALLOWMULTISELECT</td><td>
	 *			�����t�@�C���̑I���B�p�X�Ɗe�t�@�C�����͋󔒂ŋ�؂���</td></tr><tr><td>
	 *		OFN_EXTENSIONDIFFERENT</td><td>
	 *			���[�U��lpstrDefExt�ƈقȂ�g���q����͂���</td></tr><tr><td>
	 *		OFN_PATHMUSTEXIST</td><td>
	 *			�s���m�ȃp�X���͂��x������_�C�A���O��\������</td></tr><tr><td>
	 *		OFN_FILEMUSTEXIST</td><td>
	 *			�����̃t�@�C���݂̂̓��͂�����</td></tr><tr><td>
	 *		OFN_CREATEPROMPT</td><td>
	 *			�V�����t�@�C�����쐬����̂����m�F����</td></tr><tr><td>
	 *		OFN_SHAREAWARE</td><td>
	 *			�l�b�g���[�N�V�F�A�ᔽ�̃t�@�C����������</td></tr><tr><td>
	 *		OFN_NOREADONLYRETURN</td><td>
	 *			�Ԃ��ꂽ�t�@�C���͓ǂ݂Ƃ��p�ł͂Ȃ�</td></tr><tr><td>
	 *		OFN_NOTESTFILECREATE</td><td>
	 *			�l�b�g���[�N�֘A�i�߂�l�j</td></tr><tr><td>
	 *		OFN_NONETWORKBUTTON</td><td>
	 *			�l�b�g���[�N�{�^�����B��</td></tr><tr><td>
	 *		OFN_NOLONGNAMES</td><td>
	 *			�������O���g��Ȃ�</td></tr><tr><td>
	 *		OFN_EXPLORER</td><td>
	 *			�G�N�X�v���[�����̊O��</td></tr><tr><td>
	 *		OFN_NODEREFERENCELINKS</td><td>
	 *			�V���[�g�J�b�g�t�@�C�������̂܂ܕԂ�</td></tr>
 	 *		</table>
	 *
	 *	@param lpszFilter	�t�@�C������肷�邽�߂̃t�B���^���w�肷�镶����̑g�ݍ��킹�B
	 *						�t�B���^���w�肷��ƁA[�t�@�C����] �{�b�N�X�ɂ͑I�����ꂽ�t�@�C���������\�������悤�ɂȂ�܂��B
	 *	@param pParentWnd	�e�E�B���h�E�܂��̓I�[�i�[ �E�B���h�E�ւ̃|�C���^�B
	 *	@param bVistaStyle TRUE �Ȃ�@VISTA�X�^�C���ŕ\���B
	 */
	CFileDialogEx(BOOL bOpenFileDialog,
			LPCTSTR lpszDefExt = NULL,
			LPCTSTR lpszFileName = NULL,
			DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			LPCTSTR lpszFilter = NULL,
			CWnd* pParentWnd = NULL,
			BOOL bVistaStyle = FALSE
#if _MSC_VER < 1400
	) : _super(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd) 
#else
	) : _super(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, 0, bVistaStyle) 
#endif
	{
		m_strNameBuffer = lpszFileName;
		m_ofn.nMaxFile = E_BufferLength;
		m_ofn.lpstrFile = m_strNameBuffer.GetBuffer(E_BufferLength + 1);
	}

	/**
	 * [�ݒ�] �ŏ��̃f�B���N�g���w��.
	 *	@param lpszPath �f�B���N�g��
	 *	@return �I���t�@�C�����B
	 */
	void SetInitialPath(LPCTSTR lpszPath)
	{
		m_initialPath = lpszPath;
		m_ofn.lpstrInitialDir = m_initialPath;
	}

	/**
	 * [�擾] �I���t���p�X���擾.
	 *	@note �����I��( OFN_ALLOWMULTISELECT �t���O)�������ꍇ�A����őI�������t�@�C���A�S���̃t���p�X���擾�ł��܂��B
	 *	@param[out] _astrNames �I�𕶎���
	 *	@return �I���t�@�C�����B
	 */
	INT_PTR GetPathNames(CStringArray& _astrNames)
	{
		_astrNames.RemoveAll();
		POSITION pos = GetStartPosition();
		while ( pos != NULL )
		{
			_astrNames.Add(GetNextPathName(pos));
		}
		return _astrNames.GetSize();
	}

	/**
	 * [�擾] �I���t���p�X���擾.
	 *	@note �����I��( OFN_ALLOWMULTISELECT �t���O)�������ꍇ�A GetPathNames() ���g�p���Ă��������B
	 *	@return �t���p�X�B
	 */
	CString GetPathName(void) const
	{
		return _super::GetPathName();
	}

protected:

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B������I�[�o�[���C�h���ď�����ǉ����܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == WM_INITDIALOG )
		{
			GetNextWindow(GW_HWNDPREV)->ShowWindow(SW_HIDE);
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	CString	m_strNameBuffer;			///< ���O�L���p�̃o�b�t�@
	CString m_initialPath;				///< �p�X�L���p
};



}; // MFC
}; // TNB
