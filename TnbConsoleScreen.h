#pragma once
/**
 *@file
 * �R���\�[����ʊ֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"
#include <io.h>
#include <fcntl.h>



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �R���\�[����ʊǗ��N���X.
 *
 *		�R���\�[����ʂ��J���A������\�����邱�Ƃ��o���܂��B
 *
 *	@note ��ɊJ���p��z�肵�Ă��܂��B
 *
 *	@todo ���݁A�\���̂݃T�|�[�g���܂��B�e�L�X�g�R�s�[���o���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbConsoleScreen.h
 *
 *	@date 09/08/25 �V�K�쐬
 */
class CConsoleScreen : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CConsoleScreen(void) : m_console(-1)
	{
	}

	/// �f�X�g���N�^
	~CConsoleScreen(void)
	{
		Close();
	}

	/**
	 * [�\�z] �R���\�[���I�[�v��.
	 *	@note ���ɊJ���Ă������x���܂��B
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool Open(void)
	{
		Close();
		::AllocConsole();
		HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
		m_console = ::_open_osfhandle(reinterpret_cast<INT_PTR>(h), _O_TEXT);
		if ( m_console < 0 )
		{
			::FreeConsole();
			return false;
		}
		*stdout = *::_fdopen(m_console, "w");
		::setvbuf(stdout, NULL, _IONBF, 0);
		return true;
	}

	/**
	 * [�j��] �R���\�[���N���[�Y.
	 *	@note �J���Ă��Ȃ���΁A�������܂���B
	 */
	void Close(void)
	{
		if ( m_console >= 0 )
		{
			_close(m_console);
			::FreeConsole();
			m_console = -1;
		}
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v��
	 *	@retval false �N���[�Y
	 */
	bool IsOpened(void) const
	{
		return m_console >= 0;
	}

	/**
	 * [�\��] ������\��.
	 *	@param lpFmt ������.
	 *	@param ... �p�����[�^
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval �v���X �����B���l�͏������܂ꂽ�������B
	 */
	int Print(LPCTSTR lpFmt, ...)
	{
		int r = -1;
		if ( IsOpened() )
		{
			va_list args;
			va_start(args, lpFmt);
			r = _vftprintf(stdout, lpFmt, args);
			va_end(args);
		}
		return r;
	}

private:
	int		m_console;
};



}; // TNB
