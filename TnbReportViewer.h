#pragma once
/**
 *@file
 * ���|�[�g�r�����[�֌W�̃w�b�_
 *
 *		T-Report Viewer �ɂ�郍�O���T�|�[�g����N���X������܂��B
 *
 *	@note	�}�N�� "_TnbREPORT_Viewer_ON" ����`����Ă��邩�A�f�o�O�r���h�Ȃ�A Viewer �ɂ�郍�O���Ƃ邱�Ƃ��o���܂��B
 *
 *	@note CE �̏ꍇ�A�����͖�������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifdef _UNICODE
#include "TnbStr.h"
#endif
#include "TnbSync.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#ifdef _DEBUG
 #define _TnbREPORT_Viewer_ON
#endif

#if defined(_WIN32_WCE) && defined(_TnbREPORT_Viewer_ON)
 #undef _TnbREPORT_Viewer_ON
#endif

/*
 * �ʐM���
 *	[31-28]		��
 *	[27-24]		�J�e�S��(RS232c,Socket �Ȃ�)
 *	[23]		0;���M 1;��M
 *	[22]		0;�o�C�i�� 1;�e�L�X�g
 *	[21-16]		��
 *	[15-0]		0�`0xFFFF;�|�[�g�ԍ��AATOM���ʎq
*/
#define TNBVIW_KIND_RS232C		(0x01<<24)		//Rs232c
#define TNBVIW_KIND_SOCK_S		(0x02<<24)		//Socket(Server)
#define TNBVIW_KIND_SOCK_C		(0x03<<24)		//Socket(Client)
#define TNBVIW_KIND_SOCK_U		(0x04<<24)		//Socket(UDP)
#define TNBVIW_KIND_MAILS		(0x05<<24)		//Mailslot
#define TNBVIW_KIND_USB			(0x06<<24)		//USB
#define TNBVIW_KIND_FILE		(0x07<<24)		//FILE
#define TNBVIW_KIND_TEMP		(0x08<<24)		//ByteTemp
#define TNBVIW_KIND_PIPE		(0x09<<24)		//�p�C�v
#define TNBVIW_KIND_COPYDATA	(0x0B<<24)		//CopyData(Message)
#define TNBVIW_KIND_ATOM		(0x0F<<24)		//�ėp
#define TNBVIW_DIRE_SEND		0				//���M
#define TNBVIW_DIRE_RECV		(int(1<<23))	//��M
#define TNBVIW_TYPE_BIN			0				//�o�C�i��
#define TNBVIW_TYPE_TEXT		(int(1<<22))	//�e�L�X�g
#define TNBVIW_TYPE_RAWTEXT		(int(1<<21))	//���e�L�X�g
#define TNBVIW_PORT(P)			(P)				//Port�ԍ� 0�`0xFFFF

/// ���r�����[�E�B���h�E�^�C�g����
#define TNB_TNBVIEWER_WND_TITLE "TnbCommViewer"
#define TNB_TNBVIEWER_WND_CLASS "TnbReport"

/// ���r�����[COM���ʒm���b�Z�[�W
#define WM_TNBVIEWER_CMD	 (WM_APP)
#define WM_TNBVIEWER_DATA1	 (WM_APP+1)
#define WM_TNBVIEWER_DATA2	 (WM_APP+2)
#define WM_TNBVIEWER_DATA3	 (WM_APP+3)
#define WM_TNBVIEWER_DATA4	 (WM_APP+4)

#endif //_TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup REPORT
 * TNB���|�[�g�r�����[
 *
 *		T-Report Viewer �Ƀ��O���o�͂��܂��B
 *		�R���X�g���N�^�̎��� T-Report Viewer ��T���A������Έȍ~�������܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbReportViewer.h
 * 
 *	@date 10/02/05 ���ۉ����č쐬
 *	@date 11/03/10 IsValid() �ǉ��B
 *	@date 12/03/29 Invalid() �ǉ��B
 */
class CReportViewer
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@note ���̃R���X�g���N�^�̎� T-Report Viewer ��T���܂��B 
	 */
	CReportViewer(void)
	{
		#ifdef _TnbREPORT_Viewer_ON
			m_hPostWnd = ::FindWindow(NULL, _T(TNB_TNBVIEWER_WND_TITLE));
		#endif
	}

	/**
	 * [�m�F] �L���m�F.
	 *	@retval true T-Report Viewer �ɏo�͂ł��܂��B
	 *	@retval false ��
	 */
	bool IsValid(void) const
	{
		#ifdef _TnbREPORT_Viewer_ON
			return m_hPostWnd != 0;
		#else
			return false;
		#endif
	}

	/**
	 * [�ݒ�] ������.
	 *		T-Report Viewer �ւ̏o�͂𖳌��ɂ��܂��B�{���\�b�h���s��A IsValid() �� false ��Ԃ��悤�ɂ��܂��B
	 *	@note �ĂїL���ɂ�����@�͂���܂���B
	 */
	void Invalid(void)
	{
		#ifdef _TnbREPORT_Viewer_ON
			m_hPostWnd = NULL;
		#endif
	}

	/**
	 * [�o��] �_���v�o��.
	 *	@param sizeData �f�[�^�T�C�Y.
	 *	@param lpcvData �f�[�^
	 *	@param cookie �N�b�L�[�B TNBVIW_KIND_??? + PORT ���w�肵�܂��B
	 */
	void ToViewer(size_t sizeData, LPCVOID lpcvData, DWORD cookie)
	{
		#ifdef _TnbREPORT_Viewer_ON
		if ( m_hPostWnd != NULL )
		{
			m_ToViewer(static_cast<DWORD>(cookie & ~TNBVIW_TYPE_TEXT), sizeData, lpcvData);
		}
		#endif
	}

	/**
	 * [�o��] ������o��.
	 *	@param lpszText ������.
	 *	@param cookie �N�b�L�[�B TNBVIW_KIND_??? + PORT ���w�肵�܂��B
	 */
	void ToViewer(LPCTSTR lpszText, DWORD cookie)
	{
		#ifdef _TnbREPORT_Viewer_ON
		if ( m_hPostWnd != NULL )
		{
			#ifdef _UNICODE
				CAscii asc = lpszText;
				if ( ! asc.IsEmpty() )
				{
					m_ToViewer(static_cast<DWORD>(cookie | TNBVIW_TYPE_TEXT), asc.GetLength() + 1, asc.operator LPCSTR());
				}
			#else
				m_ToViewer(static_cast<DWORD>(cookie | TNBVIW_TYPE_TEXT), strlen(lpszText) + 1, lpszText);
			#endif
		}
		#endif
	}

private:

	#ifdef _TnbREPORT_Viewer_ON
	/**
	 * PostMessage
	 */
	void m_PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		BOOL r = ::PostMessage(m_hPostWnd, message, wParam, lParam);
		if ( ! r )
		{
			_GetLastError("PostMessage");
		}
	}
	/**
	 * [����] Viewer�֒ʒm
	 *	@param cookie �N�b�L�[�B
	 *	@param dataSize ���M����f�[�^���B
	 *	@param lpcvData ���M����f�[�^�B
	 */
	void m_ToViewer(DWORD cookie, size_t dataSize, LPCVOID lpcvData)
	{
		if ( ! ::IsWindow(m_hPostWnd) )
		{
			m_hPostWnd = NULL;
		}
		else
		{
			const BYTE* B = static_cast<const BYTE*>(lpcvData);
			size_t l = dataSize / 4;
			for ( size_t i = 0; i < l; i++ )
			{
				const DWORD* W = reinterpret_cast<const DWORD*>(B);
				m_PostMessage(WM_TNBVIEWER_DATA4, cookie, *W);
				B += 4;
			}
			switch ( dataSize & 3 )
			{
			case 0:
			default:
				break;
			case 1:
				m_PostMessage(WM_TNBVIEWER_DATA1, cookie, B[0]);
				break;
			case 2:
				m_PostMessage(WM_TNBVIEWER_DATA2, cookie, B[0] | (B[1] << 8));
				break;
			case 3:
				m_PostMessage(WM_TNBVIEWER_DATA3, cookie, B[0] | (B[1] << 8) | (B[2] << 16));
				break;
			}
		}
	}
	HWND		m_hPostWnd;				///< ��M Viewer �E�B���h�E�n���h��
	#endif
};



/**@ingroup REPORT
 * TNB���|�[�g�r�����[
 *
 *		T-Report Viewer �Ƀ��O���o�͂��܂��B
 *		�R���X�g���N�^�̎��� T-Report Viewer ��T���A������Έȍ~�������܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbReportViewer.h
 * 
 *	@date 11/03/10 �V�K�쐬
 *	@date 11/10/14 CSyncSection �������B
 *	@date 12/03/29 Invalid() �ǉ��B
 */
class CReportViewerEx : public CSyncSection
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@note ���̃R���X�g���N�^�̎� T-Report Viewer ��T���܂��B 
	 */
	CReportViewerEx(void) : m_cookieBase(0), m_atom(NULL)
	{
	}

	/// �f�X�g���N�^
	~CReportViewerEx(void)
	{
		m_DeleteAtom();
	}

	/**
	 * [�ݒ�] ��ސݒ�.
	 *	@param kind TNBVIW_KIND_??? + PORT ���w�肵�܂��B	 
	 */
	void SetKind(DWORD kind)
	{
		m_DeleteAtom();
		m_cookieBase = kind;
	}

	/**
	 * [�ݒ�] ��ސݒ�.
	 *	@param lpszTitle ��ޖ�(T-Report Viewer ��̎�ʂɂȂ�܂�)
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetKind(LPCTSTR lpszTitle)
	{
		ATOM a = ::GlobalAddAtom(lpszTitle);
		if ( a != NULL )
		{
			if ( m_atom != a )
			{
				m_DeleteAtom();
				m_atom = a;
			}
			m_cookieBase = TNBVIW_KIND_ATOM | m_atom;
			return true;
		}
		return false;
	}

	/**
	 * [�m�F] ��ސݒ�ς݊m�F.
	 *	@retval true �ݒ�ς�
	 *	@retval false ��
	 */
	bool HasKind(void) const
	{
		return m_cookieBase != 0;
	}

	/**
	 * [�m�F] �L���m�F.
	 *	@retval true T-Report Viewer �ɏo�͂ł��܂��B
	 *	@retval false ��
	 */
	bool IsValid(void) const
	{
		return m_viewer.IsValid();
	}

	/**
	 * [�ݒ�] ������.
	 *		T-Report Viewer �ւ̏o�͂𖳌��ɂ��܂��B�{���\�b�h���s��A IsValid() �� false ��Ԃ��悤�ɂ��܂��B
	 *	@note �ĂїL���ɂ�����@�͂���܂���B
	 */
	void Invalid(void)
	{
		m_viewer.Invalid();
	}

	/**
	 * [�o��] �_���v�o��.
	 *	@param sizeData �f�[�^�T�C�Y.
	 *	@param lpcvData �f�[�^
	 *	@param dire �����B TNBVIW_DIRE_SEND �� TNBVIW_DIRE_RECV ���w�肵�܂��B
	 */
	void ToViewer(size_t sizeData, LPCVOID lpcvData, DWORD dire = 0)
	{
		EXCLUSIVE( this );
		m_viewer.ToViewer(sizeData, lpcvData, dire | m_cookieBase);
	}

	/**
	 * [�o��] ������o��.
	 *	@param lpszText ������.
	 */
	void ToViewer(LPCTSTR lpszText)
	{
		EXCLUSIVE( this );
		m_viewer.ToViewer(lpszText, TNBVIW_TYPE_RAWTEXT | m_cookieBase);
	}

private:
	/// ATOM�j��
	void m_DeleteAtom(void)
	{
		if ( m_atom != NULL )
		{
			::GlobalDeleteAtom(m_atom);
			m_atom = NULL;
		}
	}
	ATOM			m_atom;			///< �A�g��
	DWORD			m_cookieBase;	///< �N�b�L�[�x�[�X
	CReportViewer	m_viewer;		///< �r�����[
};



}; // TNB
