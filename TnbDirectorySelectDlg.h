#pragma once
/**
 *@file
 * �f�B���N�g���I���_�C�A���O�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include <shlobj.h>



//TNB Library
namespace TNB
{
	
	
	
/**@ingroup FILE MFCCONTROL
 * �f�B���N�g���I���_�C�A���O
 *	
 *		�f�B���N�g���I���_�C�A���O��\�����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDirectorySelectDlg.h
 *
 *	@date 08/03/26 �V�K�쐬
 */
class CDirectorySelectDlg
{
public:

	/**@ingroup FILE
	 * �f�B���N�g���I���_�C�A���O���X�i�[
	 *
	 *	@see CDirectorySelectDlg
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �_�C�A���O�������ʒm.
		 *	@note �_�C�A���O���\�������O�ɒʒm����܂��B
		 *	@param hWnd �t�H���_�I���_�C�A���O�̃E�B���h�E�n���h��
		 */
		virtual void OnDirectorySelectDlgInitialized(HWND hWnd) = 0;

		/**
		 * [�ʒm] �I��ύX�ʒm.
		 *	@note �_�C�A���O���\�������O�ɂ��ʒm�����\��������܂��B
		 *	@param hWnd �t�H���_�I���_�C�A���O�̃E�B���h�E�n���h��
		 *	@param lpszPath ���ݑI������Ă���PATH.
		 *	@retval true OK�{�^���� enable
		 *	@retval false OK�{�^���� disable
		 */
		virtual bool OnDirectorySelectDlgSelectChanged(HWND hWnd, LPCTSTR lpszPath) const = 0;
	};
	
	/// �R���X�g���N�^
	CDirectorySelectDlg(void) : m_piListener(NULL)
	{
		m_path[0] = 0;
	}

	/**
	 * [�ݒ�] ���X�i�[�ݒ�.
	 *	@note DoModal() �̑O�Ɏg�p���܂��B
	 *	@param I ���X�i�[
	 */
	void SetListener(IListener* I)
	{
		m_piListener = I;
	}

	/**
	 * [�擾] �I���p�X�擾.
	 *	@note DoModal() �̌�Ɏg�p���܂��B
	 *	@return �p�X�B
	 */
	LPCTSTR GetSelectedPath(void) const
	{
		return m_path;
	}

	/**
	 * [���s] �I����ʕ\��.
	 *	@param lpszDirectory �����I���t�H���_�w��
	 *	@param lpszTitle �^�C�g���w��i�^�C�g���o�[�̕�����ł͂���܂���j
	 *	@param uFlag �_�C�A���O�{�b�N�X�̃I�v�V�����t���O���w�肵�܂��B0�܂��͈ȉ��̒l�̑g�ݍ��킹���w�肵�܂��B
	 *		<table>
	 *		<tr><td>�V���{��</td><td>���e</td></tr><tr><td>
	 *		0x0001 (BIF_RETURNONLYFSDIRS)	</td><td>
	 *			�t�@�C���V�X�e���f�B���N�g���݂̂�Ԃ��܂��B����ȊO�̃A�C�e�����I������Ă���Ƃ��ɂ́A[OK]�{�^���͊D�F�\���ɂȂ�܂��B</td></tr><tr><td>
	 *		0x0002 (BIF_DONTGOBELOWDOMAIN)	</td><td>
	 *			�_�C�A���O�{�b�N�X�̃c���[�r���[�R���g���[���Ƀh���C�����x���̃l�b�g���[�N�t�H���_���܂߂Ȃ��悤�ɂ��܂��B</td></tr><tr><td>
	 *		0x0004 (BIF_STATUSTEXT)			</td><td>
	 *			�_�C�A���O�{�b�N�X�ɃX�e�[�^�X�̈��\�����܂��B�\���e�L�X�g��ݒ肷��ɂ́A�R�[���o�b�N�֐�����_�C�A���O�{�b�N�X�Ƀ��b�Z�[�W�𑗐M���܂��B</td></tr><tr><td>
	 *		0x0008 (BIF_RETURNFSANCESTORS)	</td><td>
	 *			�V�F���l�[���X�y�[�X�K�w	�\���̒��Ń��[�g�t�H���_�̉��ɂ���t�@�C���V�X�e���T�u�t�H���_�݂̂�Ԃ��܂��B����ȊO�̃A�C�e�����I������Ă���Ƃ��ɂ́A[OK]�{�^���͊D�F�\���ɂȂ�܂��B</td></tr><tr><td>
	 *		0x0010 (BIF_EDITBOX)			</td><td>
	 *			Version 4.71 �ȍ~�F ���[�U�[���A�C�e�������������ނ��Ƃ��ł���G�f�B�b�g�R���g���[����\�����܂��B</td></tr><tr><td>
	 *		0x0020 (BIF_VALIDATE)			</td><td>
	 *			Version 4.71 �ȍ~�F ���[�U�[���G�f�B�b�g�R���g���[���ɖ����Ȗ��O����͂����ꍇ�ɁA BFFM_VALIDATEFAILED ���b�Z�[�W�ƂƂ��ɃR�[���o�b�N�֐����Ăяo����܂��BBIF_EDITBOX�t���O���w�肳��Ă��Ȃ��ꍇ�́A���̃t���O�͖�������܂��B</td></tr><tr><td>
	 *		0x0040 (BIF_NEWDIALOGSTYLE)		</td><td>
	 *			Version 5.0 �ȍ~�F �V�������[�U�[�C���^�[�t�F�[�X���g�p���܂��B�]���̃_�C�A���O�{�b�N�X�����傫���A���T�C�Y�\�ȃ_�C�A���O�{�b�N�X���\������A�_�C�A���O�{�b�N�X�ւ̃h���b�O�A���h�h���b�v�A�t�H���_�̍Đ����A�V���[�g�J�b�g���j���[�A�V�����t�H���_�쐬�A�폜�A���̑��̃V���[�g�J�b�g���j���[�R�}���h���ǉ�����܂��B</td></tr><tr><td>
	 *		0x0050 (BIF_USENEWUI)			</td><td>
	 *			Version 5.0 �ȍ~�F �G�f�B�b�g�R���g���[�������A�V�������[�U�[�C���^�[�t�F�[�X���g�p���܂��B���̃t���O��BIF_EDITBOX|BIF_NEWDIALOGSTYLE�Ɠ����ł��B</td></tr><tr><td>
	 *		0x0080 (BIF_BROWSEINCLUDEURLS)	</td><td>
	 *			Version 5.0 �ȍ~�F URL��\�����邱�Ƃ��ł���悤�ɂ��܂��BBIF_USENEWUI��BIF_BROWSEINCLUDEFILES�������Ɏw�肳��Ă��Ȃ���΂Ȃ�܂���B�����̃t���O���ݒ肳��Ă���Ƃ��A�I�����ꂽ�A�C�e�����܂ރt�H���_���T�|�[�g����ꍇ�ɂ̂݁AURL���\������܂��B�A�C�e���̑�����₢���킹�邽�߂Ƀt�H���_��IShellFolder::GetAttributesOf ���\�b�h���Ăяo���ꂽ�Ƃ��ɁA�t�H���_�ɂ����SFGAO_FOLDER�����t���O���ݒ肳�ꂽ�ꍇ�ɂ̂݁AURL���\������܂��B</td></tr><tr><td>
	 *		0x0100 (BIF_UAHINT)				</td><td>
	 *			Version 6.0 �ȍ~�F �G�f�B�b�g�R���g���[���̑���ɁA�_�C�A���O�{�b�N�X�ɗp�@�q���g��ǉ����܂��BBIF_NEWDIALOGSTYLE�t���O�ƂƂ��Ɏw�肵�Ȃ���΂Ȃ�܂���B</td></tr><tr><td>
	 *		0x0200 (BIF_NONEWFOLDERBUTTON)	</td><td>
	 *			Version 6.0 �ȍ~�F �_�C�A���O�{�b�N�X�Ɂu�V�����t�H���_�v�{�^����\�����Ȃ��悤�ɂ��܂��BBIF_NEWDIALOGSTYLE�t���O�ƂƂ��Ɏw�肵�Ȃ���΂Ȃ�܂���B</td></tr><tr><td>
	 *		0x0400 (BIF_NOTRANSLATETARGETS)	</td><td>
	 *			Version 6.0 �ȍ~�F �I�����ꂽ�A�C�e�����V���[�g�J�b�g�ł���Ƃ��A���̃����N��ł͂Ȃ��A�V���[�g�J�b�g�t�@�C�����̂�PIDL��Ԃ��܂��B</td></tr><tr><td>
	 *		0x1000 (BIF_BROWSEFORCOMPUTER)	</td><td>
	 *			�R���s���[�^�݂̂�Ԃ��܂��B����ȊO�̃A�C�e�����I������Ă���Ƃ��ɂ́A[OK]�{�^���͊D�F�\���ɂȂ�܂��B</td></tr><tr><td>
	 *		0x2000 (BIF_BROWSEFORPRINTER)	</td><td>
	 *			�v�����^�݂̂�Ԃ��܂��B����ȊO�̃A�C�e�����I������Ă���Ƃ��ɂ́AOK �{�^���͊D�F�\���ɂȂ�܂��B</td></tr><tr><td>
	 *		0x4000 (BIF_BROWSEINCLUDEFILES)	</td><td>
	 *			Version 4.71 �ȍ~�F �t�H���_�ƃt�@�C����\�����܂��B</td></tr><tr><td>
	 *		0x8000 (BIF_SHAREABLE)			</td><td>
	 *			Version 5.0 �ȍ~�F �����[�g�V�X�e����ɂ��鋤�L���\�[�X��\���ł���悤�ɂ��܂��BBIF_USENEWUI�t���O�ƂƂ��Ɏw�肵�Ȃ���΂Ȃ�܂���B</td></tr>
 	 *		</table>
	 *	@param hWnd �e�E�B���h�E�B
	 *	@retval IDOK �I��
	 *	@retval IDCANCEL �L�����Z��
	 */
	int DoModal(LPCTSTR lpszDirectory, LPCTSTR lpszTitle, UINT uFlag = 0, HWND hWnd = NULL)
	{
		m_path[0] = 0;
		LPMALLOC lpMalloc = NULL;
		HRESULT hr = ::SHGetMalloc(&lpMalloc);
		if ( FAILED(hr) )
		{
			return IDCANCEL;
		}
		// �u���E�Y���󂯎��o�b�t�@�̈�̊m��
		LPTSTR lpBuffer = static_cast<LPTSTR>(lpMalloc->Alloc(MAX_PATH));
		if ( lpBuffer == NULL )
		{
			return IDCANCEL;
		}
		int	r = IDCANCEL;
		HRESULT hCom = ::CoInitialize(NULL);
		// BROWSEINFO�\���̂̏����l�ݒ�
		BROWSEINFO bi = { 0 };
		bi.hwndOwner = hWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = lpBuffer;
		bi.lpszTitle = lpszTitle;
		bi.ulFlags = uFlag;
		bi.lpfn = ms_BrowseCallbackProc; // �R�[���o�b�N�֐��̃A�h���X��ݒ�
		TParam param;
		param.lpszDefaultDir = lpszDirectory;
		param.piListener = m_piListener;
		bi.lParam = reinterpret_cast<LPARAM>(&param);
		// �t�H���_�I���_�C�A���O�̕\�� 
		LPITEMIDLIST pidlBrowse = ::SHBrowseForFolder(&bi);
		if ( pidlBrowse != NULL )
		{
			// PIDL�`���̖߂�l�̃t�@�C���V�X�e���̃p�X�ɕϊ�
			if ( ::SHGetPathFromIDList(pidlBrowse, lpBuffer) )
			{
				// �擾����
				_tcscpy(m_path, lpBuffer);
				r = IDOK;
			}
			// SHBrowseForFolder�̖߂�lPIDL�����
			lpMalloc->Free(pidlBrowse);
		}
		// �N���[���A�b�v����
		lpMalloc->Free(lpBuffer);
		if ( hCom == S_OK )
		{
			::CoUninitialize();
		}
		//
		return r;
	}

private:
	IListener*	m_piListener;			///< ���X�i�[
	TCHAR		m_path[MAX_PATH];		///< �I���p�X�ۑ��p
	/// �R�[���o�b�N�ʒm�p�p�����[�^
	struct TParam
	{
		LPCTSTR		lpszDefaultDir;	///< �����p�X
		IListener*	piListener;		///< ���X�i�[
	};
	/**
	 * [�ʒm] �R�[���o�b�N
	 *	@param hWnd �I���_�C�A���O�̃n���h��
	 *	@param uMsg ���b�Z�[�W
	 *	@param lParam ���b�Z�[�W�p�̃p�����[�^
	 *	@param lpData TParam �̃|�C���^
	 */
	static int CALLBACK ms_BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		const TParam* P = reinterpret_cast<const TParam*>(lpData);
		if ( uMsg == BFFM_INITIALIZED )
		{
			::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<LPARAM>(P->lpszDefaultDir));
			if ( P->piListener != NULL )
			{
				P->piListener->OnDirectorySelectDlgInitialized(hWnd);
				bool r = P->piListener->OnDirectorySelectDlgSelectChanged(hWnd, P->lpszDefaultDir);
				::SendMessage(hWnd, BFFM_ENABLEOK, 0, r);
			}
		}
		else if ( uMsg == BFFM_SELCHANGED ) 
		{
			if ( P->piListener != NULL )
			{
				TCHAR path[MAX_PATH];
				bool r = false;
				if ( ::SHGetPathFromIDList(reinterpret_cast<LPCITEMIDLIST>(lParam), path) )
				{
					::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, reinterpret_cast<LPARAM>(path));
					r = P->piListener->OnDirectorySelectDlgSelectChanged(hWnd, path);
				}
				::SendMessage(hWnd, BFFM_ENABLEOK, 0, r);
			}
		}
		else if( uMsg == BFFM_VALIDATEFAILED )
		{
		}
		return 0;
	}
};



};//TNB



