#pragma once
/**
 *@file
 * �t�@�C���Ď��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbThread.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �f�B���N�g���Ď��N���X
 *
 *		�w��̃f�B���N�g���̕ω����Ď����邽�߂̃N���X�ł��B
 *
 *		�w��̕ω�������ƃ��X�i�[�ɒʒm����܂��B
 *		���̍ہA�ǂ�ȕω������������͂킩��܂���̂ŁA�ω����e��m�肽���ꍇ�A
 *		�Ď��t���O���ɖ{�N���X�̃C���X�^���X�𕡐����K�v������܂�
 *		�i���X�i�[�ɂ̓t���O��񂪗���̂ŁA��̃��X�i�[�őΉ��\�ł��j�B
 *
 *	@see http://nienie.com/~masapico/api_FindFirstChangeNotification.html
 *
 *	@note ���X�i�[���� SendMessage ���g���ꍇ�A ReferThreadStatus() ��{@link CThreadStatus::SetOwnerHwnd() �֘A�E�B���h�E}���w�肷��
 *				�K�v������܂��B CThread::SetOwnerHwnd() ���Q�Ƃ��Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbDirectoryWatcher.h
 *
 *	@date 06/12/01 �V�K
 *	@date 10/03/19	ReferThreadStatus() �ǉ��B
 */
class CDirectoryWatcher : TNB::CThread::IRunner
{
public:

	/**@ingroup FILE
	 * �f�B���N�g���Ď��̃��X�i�[�C���^�[�t�F�[�X
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbDirectoryWatcher.h
	 *
	 *	@date 06/12/01 �V�K�쐬�B
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �ω��ʒm
		 *	@note �����̏������w�肵���ꍇ�A�ǂ̏������́A�킩��Ȃ��̂Œ��ӁB
		 *	@param dwFlags CDirectoryWatcher::Start() �� dwFlags �Ŏw�肵���̒l�B
		 *					�ω��������e�ł͂Ȃ����Ƃɒ��ӁB
		 *	@retval true �Ď��p��
		 *	@retval false �Ď���~
		 */
		virtual bool OnDirectoryChange(DWORD dwFlags) = 0;
	};

private:

	HANDLE			m_hNotify;		///< �Ď��n���h��
	DWORD			m_dwFlags;		///< �Ď��t���O
	IListener*		m_pListener;	///< ���X�i�[
	TNB::CThread	m_thread;		///< �Ď��X���b�h

	/** 
	 * [����] �X���b�h�����{��
	 *	@return �O�B
	 */
	virtual DWORD Run(void)
	{
		while ( m_hNotify != NULL && IsRunnable() )
		{
			DWORD r = ::WaitForSingleObject(m_hNotify, INFINITE);
			if ( r != WAIT_OBJECT_0 || m_hNotify == NULL )
			{
				break;
			}
			if ( ! m_pListener->OnDirectoryChange(m_dwFlags) )
			{
				break;
			}
			if ( ! ::FindNextChangeNotification(m_hNotify) )
			{
				ASSERT(false);
				break;
			}
		}
		if ( m_hNotify != NULL )
		{
			::FindCloseChangeNotification(m_hNotify);
			m_hNotify = NULL;
		}
		return 0;
	}

public:

	/// �R���X�g���N�^
	CDirectoryWatcher(void) : m_hNotify(NULL), m_pListener(NULL)
	{
	}

	/// �f�X�g���N�^
	virtual ~CDirectoryWatcher(void)
	{
		Stop();
	}

	/**
	 * [�J�n] �Ď��J�n
	 *	@param lpszPath	�Ď����s���f�B���N�g���̃f�B���N�g����
	 *	@param boWithSubDir ture���w�肷��Ǝw��f�B���N�g���ȉ��̃T�u�f�B���N�g�������Ď��ΏۂɂȂ�܂��B
	 *	@param dwFlags �ǂ̂悤�ȃC�x���g���Ď����邩���w�肷��t���O�B									\n
	 *						FilterFlags�Ɏw��ł���l�́A�ȉ��̃t���O�̘_���a�ł���B					\n
	 *						FILE_NOTIFY_CHANGE_FILE_NAME	�t�@�C���̒ǉ��E�폜�A�t�@�C�����̕ύX		\n
	 *						FILE_NOTIFY_CHANGE_DIR_NAME	�f�B���N�g���̒ǉ��E�폜�A�f�B���N�g�����̕ύX	\n
	 *						FILE_NOTIFY_CHANGE_ATTRIBUTES	�����̕ύX									\n
	 *						FILE_NOTIFY_CHANGE_SIZE	�T�C�Y�̕ύX										\n
	 *						FILE_NOTIFY_CHANGE_LAST_WRITE	�ŏI�������ݎ����̕ύX						\n
	 *						FILE_NOTIFY_CHANGE_SECURITY	�Z�L�����e�B�����̕ύX
	 *	@param pListener ���X�i�[�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Start(LPCTSTR lpszPath, bool boWithSubDir, DWORD dwFlags, IListener* pListener)
	{
		Stop();
		m_hNotify = ::FindFirstChangeNotification(lpszPath, boWithSubDir, dwFlags);
		if ( m_hNotify == NULL )
		{
			return false;
		}
		m_dwFlags = dwFlags;
		m_pListener = pListener;
		m_thread.SetRunner(this);
		if ( ! m_thread.Start(_T("Directory�Ď�")) )
		{
			Stop();
			return false;
		}
		return true;
	}

	/**
	 * [��~] �Ď���~
	 *	@note �Ď��J�n���Ă��Ȃ��ꍇ�A�������܂���B
	 */
	void Stop(void)
	{
		if ( m_hNotify != NULL )
		{
			HANDLE h = m_hNotify;
			m_hNotify = NULL;
			::FindCloseChangeNotification(h);
		}
		m_thread.Stop();
	}

	/**
	 * [�Q��] �Ď��X���b�h��ԎQ��.
	 *	@retval �X���b�h��Ԃ̎Q��.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_thread;
	}
};



}; //TNB
