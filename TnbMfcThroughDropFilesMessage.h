#pragma once
/**
 *@file
 * �t�@�C���h���b�v���b�Z�[�W���ߏ����֌W�̃w�b�_
 *
 *		�R���g���[������e�Ƀt�@�C���h���b�v���b�Z�[�W�𓧉߂�����@�\��񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �t�@�C���h���b�v���b�Z�[�W���߃A�h�C���N���X
 *
 *		�{�N���X���g�p����ƁA�h���b�v���ꂽ���b�Z�[�W��e�ɓ��߂��邱�Ƃ��\�ɂȂ�܂��B
 *
 *	@par�g�p��
 *	\code
 *		// .h
 *		typedef CThroughDropFilesMessageAddinT<CStatic> CMyStatic;
 *
 *		class CFooDlg : public CDialog
 *		{
 *			CMyStatic	m_staticDrop;			// ���X CStatic �����������o������������
 *					:
 *	\endcode
 *	\code
 *		// .cpp
 *		BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
 *				;
 *			ON_WM_DROPFILES()
 *				;
 *
 *		void CFooDlg::OnDropFiles(HDROP hDropInfo)
 *		{
 *			if ( ! m_staticDrop.IsDroped() )	// m_staticDrop �Ƀh���b�v���ꂽ��H
 *			{
 *				return;
 *			}
 *			UINT count = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
 *			CStr fileName;
 *			::DragQueryFile(hDropInfo, 0, fileName.GetBuffer(MAX_PATH - 1), MAX_PATH);
 *			fileName.ReleaseBuffer();
 *				;
 *	\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcThroughDropFilesMessage.h
 * 
 *	@date 12/03/02 �V�K�쐬
 */
template<typename TYP = CStatic>
class CThroughDropFilesMessageAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// �R���X�g���N�^
	CThroughDropFilesMessageAddinT(void) : m_isDroped(false)
	{
	}

	/**
	 * [�m�F] �h���b�v���ꂽ�H
	 *	@note �{���\�b�h�͐e�E�B���h�E�� WM_DROPFILES ���ʒm���ꂽ���Ɏg�p���܂��B
	 *	@retval true �h���b�v���ꂽ
	 *	@retval false ��
	 */
	bool IsDroped(void) const
	{
		return m_isDroped;
	}

	/**
	 * [�擾] �h���b�v���擾.
	 *	@param hDrop �h���b�v�n���h��
	 *	@return �h���b�v��
	 */
	static size_t QueryDragFileCount(HDROP hDrop)
	{
		return ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	}

	/**
	 * [�擾] �h���b�v�t�@�C�����擾.
	 *	@param hDrop �h���b�v�n���h��
	 *	@param index �C���f�b�N�X�B 0�ȏ�A QueryDragFileCount() �����B
	 *	@retval empty������ ���s�B index �̒l�����������B
	 *	@retval ��L�ȊO �����B���e�̓t�@�C�����B
	 */
	static CStr QueryDragFileName(HDROP hDrop, INDEX index)
	{
		CStr s;
		size_t count =QueryDragFileCount(hDrop);
		if ( count > index )
		{
			::DragQueryFile(hDrop, index, s.GetBuffer(MAX_PATH), MAX_PATH);
			s.ReleaseBuffer();
		}
		return s;
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		m_isDroped = false;
		::DragAcceptFiles(_super::GetSafeHwnd(), TRUE);
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
		if ( message == WM_DROPFILES )
		{
			CWnd* P = _super::GetParent();
			if ( P != NULL )
			{
				m_isDroped = true;
				LRESULT r = P->SendMessage(message, wParam, lParam);
				m_isDroped = false;
				return r;
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
private:
	bool	m_isDroped;	///< �h���b�v���ꂽ
};



}; //MFC
}; //TNB



