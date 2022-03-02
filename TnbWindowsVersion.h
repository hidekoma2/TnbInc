#pragma once
/**
 *@file
 * �E�B���h�E�Y�o�[�W�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #define _OSVERPACK(BASE, MAJOR, MINOR, IS64) \
				MAKELONG(MAKEWORD(MINOR, MAJOR), MAKEWORD(BASE, IS64))
#endif



/**@ingroup WINAPI
 * �E�B���h�E�Y�o�[�W�������
 *
 *	@par�K�v�t�@�C��
 *			TnbWindowsVersion.h
 *
 *	@date 08/03/19 �V�K�쐬�B
 */
class CWindowsVersion
{
public:
	/// Platform�^�C�v
	enum EPlatformType
	{
		WIN3_BASE = VER_PLATFORM_WIN32s,			///< Windows 3.1
		WIN9x_BASE = VER_PLATFORM_WIN32_WINDOWS,	///< Windows 9x�x�[�X
		WINNT_BASE = VER_PLATFORM_WIN32_NT,			///< Windows NT �x�[�X
	};
	/// OS �o�[�W����
	enum EOsVersion
	{
		UNKNOWN 	= _OSVERPACK(WIN3_BASE,  0,  0, 0),		///< �s��
		WINNT351	= _OSVERPACK(WINNT_BASE, 3, 51, 0),		///< Windows NT 3.51
		WIN95		= _OSVERPACK(WIN9x_BASE, 4,  0, 0),		///< Windows 95
		WIN98		= _OSVERPACK(WIN9x_BASE, 4, 10, 0),		///< Windows 98
		WINME		= _OSVERPACK(WIN9x_BASE, 4, 90, 0),		///< Windows ME
		WINNT4		= _OSVERPACK(WINNT_BASE, 4,  0, 0),		///< Windows NT 4.0
		WIN2000		= _OSVERPACK(WINNT_BASE, 5,  0, 0),		///< Windows 2000
		WINXP		= _OSVERPACK(WINNT_BASE, 5,  1, 0),		///< Windows XP
		WINXP64		= _OSVERPACK(WINNT_BASE, 5,  2, 1),		///< Windows XP (64bit)
		WINVISTA	= _OSVERPACK(WINNT_BASE, 6,  0, 0),		///< Windows Vista
		WIN7		= _OSVERPACK(WINNT_BASE, 6,  1, 0),		///< Windows 7
		WINVISTA_64	= _OSVERPACK(WINNT_BASE, 6,  0, 1),		///< Windows Vista (64bit)
		WIN7_64		= _OSVERPACK(WINNT_BASE, 6,  1, 1),		///< Windows 7 (64bit)
//		WINS2003	= _OSVERPACK(WINNT_BASE, 5,  2, 0),		///< Windows Server 2003
//		WINS2008	= _OSVERPACK(WINNT_BASE, 6,  0, 0),		///< Windows Server 2008
//		WINS2008R2	= _OSVERPACK(WINNT_BASE, 6,  1, 0),		///< Windows Server 2008 R2
	};

	/**
	 * �R���X�g���N�^.
	 *	@note OS�o�[�W���������擾���Ă��܂��B
	 */
	CWindowsVersion(void)
	{
		OSVERSIONINFO* P = reinterpret_cast<OSVERSIONINFO*>(&m_info);
		::ZeroMemory(&m_info, sizeof(OSVERSIONINFOEX));
		m_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		if ( ! ::GetVersionEx(P) )
		{
			::ZeroMemory(&m_info, sizeof(OSVERSIONINFOEX));
			m_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			::GetVersionEx(P);
		}
		m_boIsWow64 = m_IsWow64();
	}

	/**
	 * [�m�F] 64bit�`�F�b�N
	 *	@retval false 32bitOS
	 *	@retval true 64bitOS
	 */
	bool IsWow64(void) const
	{
		return m_boIsWow64;
	}

	/**
	 * [�擾] OS�o�[�W�����擾
	 *	@return �o�[�W����
	 */
	EOsVersion GetOsVersion(void) const
	{
		return m_GetOsVersion();
	}

	/**
	 * [�擾] �}�C�i�[�o�[�W�����擾.
	 *	@retval 0	Windows 95
	 *	@retval 10	Windows 98
	 *	@retval 90	Windows Me
	 *	@retval 51	Windows NT 3.51
	 *	@retval 0	Windows NT 4.0
	 *	@retval 0	Windows 2000
	 *	@retval 1	Windows XP
	 *	@retval 2	Windows Server 2003
	 *	@retval 0	Windows 7
	 */ 
	DWORD GetMinorVersion(void) const
	{
		return m_info.dwMinorVersion;
	}

	/**
	 * [�擾] ���W���[�o�[�W�����擾.
	 *	@retval 4	Windows 95 / 98 / Me
	 *	@retval 3	Windows NT 3.51
	 *	@retval 4	Windows NT 4.0
	 *	@retval 5	Windows 2000 / XP / Server 2003
	 *	@retval 6	Windows Vista / 7
	 */
	DWORD GetMajorVersion(void) const
	{
		return m_info.dwMajorVersion;
	}

	/**
	 * [�擾] �v���b�g�t�H�[��
	 *	@return �v���b�g�t�H�[��
	 */
	EPlatformType GetPlatform(void) const
	{
		return static_cast<EPlatformType>(m_info.dwPlatformId);
	}

	/**
	 * [�擾] �ǉ�������.
	 *	@note �T�[�r�X�p�b�N���C���X�g�[������Ă���ꍇ�́A���̏������������񂪊i�[����܂��i��F�gService Pack 3�h�j�B
	 *			����ȊO�̏ꍇ�͋󕶎���ɂȂ�܂��B
	 *	@return ������B
	 */
	LPCTSTR GetAdditionalString(void) const
	{
		return m_info.szCSDVersion;
	}

	/**
	 * [�擾] �T�[�r�X�p�b�N�o�[�W����
	 *	@return ���WORD���W���[�A����WORD�Ƀ}�C�i�[�o�[�W����
	 */
	DWORD GetServicePackVersion(void) const
	{
		return MAKELONG(m_info.wServicePackMinor, m_info.wServicePackMajor);
	}

	/**
	 * [�擾] �r���h�i���o�[
	 *	@return �r���h�i���o�[
	 */
	DWORD GetBuildNumber(void) const
	{
		return m_info.dwBuildNumber;
	}

	/**
	 * [�m�F] Windows98 SE �`�F�b�N
	 *	@retval true Window98SE��
	 *	@retval false ����ȊO
	 */
	bool IsWindows98SE(void) const
	{
		if ( m_GetOsVersion() == WIN98 )
		{
			return (m_info.szCSDVersion[0] == 'A' && m_info.szCSDVersion[1] == 0);
		}
		return false;
	}

	/**
	 * [�擾] OS�o�[�W����������擾.
	 *	@retval NULL �s���o�[�W����
	 *	@retval NULL�ȊO ������
	 */
	LPCTSTR GetOsVersionString(void) const
	{
		if ( IsWindows98SE() )
		{
			return _T("Windows 98SE");
		}
		switch ( m_GetOsVersion() )
		{
		case WINNT351:		return _T("Windows NT 3.51");
		case WIN95:			return _T("Windows 95");
		case WIN98:			return _T("Windows 98");
		case WINME:			return _T("Windows ME");
		case WINNT4:		return _T("Windows NT 4.0");
		case WIN2000:		return _T("Windows 2000");
		case WINXP:			return _T("Windows XP");
		case WINVISTA:		return _T("Windows Vista");
		case WIN7:			return _T("Windows 7");
		case WINXP64:		return _T("Windows XP64");
		case WINVISTA_64:	return _T("Windows Vista 64");
		case WIN7_64:		return _T("Windows 7 64");
		default:
			break;
		}
		return NULL;
	}
private:
	OSVERSIONINFOEX	m_info;		///< �o�[�W�������
	bool			m_boIsWow64;///< 64bit�t���O
	/// �o�[�W�����擾
	EOsVersion m_GetOsVersion(void) const
	{
		EOsVersion v = static_cast<EOsVersion>(
			_OSVERPACK(GetPlatform(), GetMajorVersion(), GetMinorVersion(), m_boIsWow64));
		switch ( v )
		{
		case WINNT351:
		case WIN95:
		case WIN98:
		case WINME:
		case WINNT4:
		case WIN2000:
		case WINXP:
		case WINVISTA:
		case WINXP64:
		case WINVISTA_64:
		case WIN7_64:
			break;
		default:
			v = UNKNOWN;
			break;
		}
		return v;
	}
	/**
	 * [�m�F] 64bit�`�F�b�N
	 *	@retval false 32bitOS
	 *	@retval true 64bitOS
	 */
	bool m_IsWow64(void) const
	{
		typedef BOOL (WINAPI *P_ISWOW64PROCESS)(HANDLE, PBOOL);
		P_ISWOW64PROCESS pIsWow64Process 
			= reinterpret_cast<P_ISWOW64PROCESS>(::GetProcAddress(::GetModuleHandleA("kernel32"), "IsWow64Process"));
		BOOL bIsWow64 = FALSE;
		if ( NULL != pIsWow64Process )
		{
			VERIFY( pIsWow64Process(::GetCurrentProcess(), &bIsWow64) );
		}
		return !! bIsWow64;
	}

};



};//TNB

