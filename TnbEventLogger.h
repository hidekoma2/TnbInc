#pragma once
/**
 *@file
 * �C�x���g���K�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSingleton.h"
#include "TnbStrEx.h"
#include "TnbRegistryAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup LOG
 * �C�x���g���K�[
 * 
 *		�V�X�e���̃C�x���g�ɋL�^���܂��B
 *
 *	@note �V���O���g���ō쐬����Ă��܂��B
 *
 *	@par ���K�{�� �v���W�F�N�g�� rc2 �t�@�C���Ɉȉ���ǉ�����
 *		\code
 *		#include "TnbEventLogger.rc"
 *		\endcode
 *
 *	@par�g�p��
 *		\code
 *		CEventLogger* P = CEventLogger::GetInstance();
 *		P->SetSourceName(_T("TnbTestProg"));
 *		P->RegistEventSource();
 *		int i;		
 *
 *				;
 *
 *		P->Information(4, "ABCD", _T("ABC %d D"), i);	// �f�[�^�t�����A�o��
 *		P->Warning(0, _T("ABCD"));						// �x���A�o��
 *
 *				;
 *
 *		P->UnregistEventSource();
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbEventLogger.h
 *
 *	@date 09/11/02	�V�K�쐬
 */
class CEventLogger
{
	/// �R���X�g���N�^
	SINGLETON_CONSTRUCTOR(CEventLogger)
	{
		m_sourceName = GetProcessName();
	}
public:

	/**
	 * [�ݒ�] �\�[�X���ݒ�.
	 *	@note �ݒ肵�Ȃ��ꍇ�A���s�t�@�C�������\�[�X���ɂȂ�܂��B
	 *	@note RegistEventSource() �����s����O�ɂ��̃��\�b�h���g�p���Ă��������B
	 *	@param lpszSourceName �\�[�X���B
	 */
	void SetSourceName(LPCTSTR lpszSourceName)
	{
		m_sourceName = lpszSourceName;
	}

	/**
	 * [�擾] �\�[�X���擾.
	 *	@note SetSourceName() �Őݒ肵�Ă��Ȃ��ꍇ�A���s�t�@�C�������\�[�X���ɂȂ�܂��B
	 *	@return �\�[�X���B
	 */
	LPCTSTR GetSourceName(void) const
	{
		return m_sourceName;
	}

	/**
	 * [�o�^] �C�x���g�\�[�X�o�^.
	 *		�\�[�X�����C�x���g�\�[�X�ɓo�^���܂��B�o�^���Ȃ��ƁA�C�x���g�r�����[�Łu�����v���������\������܂���B
	 *	@note �C�x���g�r�����[���J���Ă���ƁA�o�^�������W���[���͍폜�ł��Ȃ��Ȃ�܂��B
	 *	@note ���W�X�g���ɓo�^���邽�߁A�N���X�I������A�o�^���ꂽ�܂܂ɂȂ�܂��B��������ꍇ�A UnregistEventSource() ���g�p���܂��B
	 *	@param lpszModuleName �C�x���g�\�[�X�����A���W���[����( exe / dll ��)���w�肵�܂��B�ȗ�����ƁA������ exe �ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool RegistEventSource(LPCTSTR lpszModuleName = NULL)
	{
		HMODULE h = NULL;
		if ( lpszModuleName != NULL )
		{
			h = ::LoadLibrary(lpszModuleName);
		}
		CStr path;
		DWORD dwRc = ::GetModuleFileName(h, path.GetBuffer(MAX_PATH), MAX_PATH);
		path.ReleaseBuffer();
		if ( dwRc > 0 )
		{
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_KeyName());
			CRegistryAccessor::CSection sec = reg[m_sourceName];
			sec.WriteString(_T("EventMessageFile"), path);
		}
		return (dwRc > 0);
	}

	/**
	 * [�o�^] �C�x���g�\�[�X�o�^����.
	 *	@note RegistEventSource() ���s��A�N���X�I������A�o�^���ꂽ�܂܂ɂȂ�܂��B�������߂ɁA���̃��\�b�h���g�p���܂��B
	 *	@note ��������O�ɁA SetSourceName() �Ń\�[�X����ς��Ȃ��悤�ɂ��Ă��������B
	 */
	void UnregistEventSource(void)
	{
		if ( ! m_sourceName.IsEmpty() )
		{
			CRegistryAccessor reg(HKEY_LOCAL_MACHINE, m_KeyName());
			reg.DeleteSection(m_sourceName);
		}
	}

	/**
	 * [�o��] ���o��.
	 *	@param eventId �C�x���gID�B 0�`31 �Ŏw�肵�܂��B
	 *	@param lpszFmt �����w�蕶����B printf() �����ł��B
	 *	@param ... �ό����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Information(DWORD eventId, LPCTSTR lpszFmt, ...) const
	{
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_INFORMATION_TYPE, eventId, 0, NULL, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [�o��] ���o��(�f�[�^�t��).
	 *	@param eventId �C�x���gID�B 0�`31 �Ŏw�肵�܂��B
	 *	@param dataSize �f�[�^�T�C�Y�B
	 *	@param lpData �f�[�^�B
	 *	@param lpszFmt �����w�蕶����B printf() �����ł��B
	 *	@param ... �ό����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Information(DWORD eventId, size_t dataSize, LPCVOID lpData, LPCTSTR lpszFmt, ...) const
	{
		ASSERT( ! ::IsBadReadPtr(lpData, dataSize) );
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_INFORMATION_TYPE, eventId, dataSize, lpData, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [�o��] �x���o��.
	 *	@param eventId �C�x���gID�B 0�`31 �Ŏw�肵�܂��B
	 *	@param lpszFmt �����w�蕶����B printf() �����ł��B
	 *	@param ... �ό����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Warning(DWORD eventId, LPCTSTR lpszFmt, ...) const
	{
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_WARNING_TYPE, eventId, 0, NULL, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [�o��] �x���o��(�f�[�^�t��).
	 *	@param eventId �C�x���gID�B 0�`31 �Ŏw�肵�܂��B
	 *	@param dataSize �f�[�^�T�C�Y�B
	 *	@param lpData �f�[�^�B
	 *	@param lpszFmt �����w�蕶����B printf() �����ł��B
	 *	@param ... �ό����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Warning(DWORD eventId, size_t dataSize, LPCVOID lpData, LPCTSTR lpszFmt, ...) const
	{
		ASSERT( ! ::IsBadReadPtr(lpData, dataSize) );
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_WARNING_TYPE, eventId, dataSize, lpData, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [�o��] �G���[�o��.
	 *	@param eventId �C�x���gID�B 0�`31 �Ŏw�肵�܂��B
	 *	@param lpszFmt �����w�蕶����B printf() �����ł��B
	 *	@param ... �ό����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Error(DWORD eventId, LPCTSTR lpszFmt, ...) const
	{
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_ERROR_TYPE, eventId, 0, NULL, lpszFmt, args);
		va_end(args);
		return r;
	}

	/**
	 * [�o��] �G���[�o��(�f�[�^�t��).
	 *	@param eventId �C�x���gID�B 0�`31 �Ŏw�肵�܂��B
	 *	@param dataSize �f�[�^�T�C�Y�B
	 *	@param lpData �f�[�^�B
	 *	@param lpszFmt �����w�蕶����B printf() �����ł��B
	 *	@param ... �ό����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Error(DWORD eventId, size_t dataSize, LPCVOID lpData, LPCTSTR lpszFmt, ...) const
	{
		ASSERT( ! ::IsBadReadPtr(lpData, dataSize) );
		va_list args;
		va_start(args, lpszFmt);
		bool r = m_Write(EVENTLOG_ERROR_TYPE, eventId, dataSize, lpData, lpszFmt, args);
		va_end(args);
		return r;
	}

private:
	// �L�[��
	LPCTSTR m_KeyName(void) const
	{
		return _T("SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application");
	}
	// ��������
	bool m_Write(WORD type, DWORD eventId, size_t binarySize, LPCVOID lpBinary, LPCTSTR lpszFmt, va_list V) const
	{
		bool r = false;
		CStr str;
		str.FormatV(lpszFmt, V);
		LPCTSTR lpszMsg = str;
		HANDLE h = ::RegisterEventSource(NULL, m_sourceName);
		if ( h == NULL )
		{
			_GetLastError("RegisterEventSource");
			return false;
		}
		r = !! ::ReportEvent(h, type, 0, eventId, NULL, 1, static_cast<DWORD>(binarySize), &lpszMsg, const_cast<LPVOID>(lpBinary));
		if ( ! r )
		{
			_GetLastError("ReportEvent");
		}
		::DeregisterEventSource(h);
		return r;
	}

	CStr		m_sourceName;	///< �\�[�X��
};



}; // TNB


