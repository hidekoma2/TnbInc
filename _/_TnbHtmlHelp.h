#pragma once


//MFC�s�K�v�ł��B


#include "TnbDef.h"



#include <Htmlhelp.h>
#pragma comment(lib,"Htmlhelp.lib")



//TNB Library
namespace TNB
{




class CHtmlHelp : CCopyImpossible
{
	DWORD	m_dwCockie;			///< �N�b�L�[				
	HWND	m_hWnd;				///< �e�E�B���h�E			
	CStr	m_strFileName;		///< HTML�w���v�t�@�C����	
	CStr	m_strWinType;
public:

	/**
	 * �R���X�g���N�^
	 */
	CHtmlHelp(void)
	{
		::HtmlHelp( NULL,NULL.HH_INITIALIZE,(DWORD)&m_dwCockie );
		m_strWinType = CStr::Fmt(_T("_Win%08X_%08X", ::GetCurrentProcessId(), ::GetTickCount());
		m_hWnd = NULL;
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CHtmlHelp(void)
	{
		::HtmlHelp( NULL,NULL.HH_CLOSE_ALL,0 );
		::HtmlHelp( NULL,NULL.HH_UNINITIALIZE,m_dwCockie );
	}

	/**
	 * �g�����X���[�g.
	 *	@note �e�E�B���h�E�Ń��b�Z�[�W�𗬂��Ă�������
	 *	@retval true�Ȃ�MSG�̏��������B
	 *	@retval false�Ȃ珈�����Ă��Ȃ��B
	 */
	bool TranslateMessage( MSG& msg )
	{
		return !! ::HtmlHelp( NULL,NULL,HH_PRETRANSLATEMESSAGE,(DWORD)&msg );
	}

	/**
	 * HTML�w���v�̐ݒ�
	 *	@param hWnd �n���h��
	 *	@param lpszFileName HTML�w���v�t�@�C����
	 */
	bool Create( HWND hWnd,LPCTSTR lpszFileName )
	{
		m_hWnd = hWnd;
		m_strFileName = lpszFileName;
		::HtmlHelp( NULL,NULL.HH_CLOSE_ALL,0 );
	}

	/**
	 * �w��̃L�[���[�h��ʂ��J��
	 *	@param lpszIndex �C���f�b�N�X��
	 */
	void ViewIndex(LPCTSTR lpszIndex)
	{
		if ( m_hWnd==NULL )
		{
			return;
		}
		::HtmlHelp( m_hWnd,str,HH_DISPLAY_INDEX,(DWORD)lpszKeyword );
	}

	/**
	 * �w��̃g�s�b�N��ʂ��J��
	 *	@param lpszTopicName �g�s�b�N��
	 *	@param lpszWinType 
	 */
	void ViewTopic(LPCTSTR lpszTopicName,LPCTSTR lpszWinType)
	{
		if ( m_hWnd==NULL )
		{
			return;
		}
		CStr str;
		str.Format(_T("%s::%s>%s",m_strFile,lpszTopicName,lpszWinType );
		::HtmlHelp( m_hWnd,str,HH_DISPLAY_TOPIC,NULL );
	}
	void ViewSearch(LPCTSTR lpszTopicName)
	{
		if ( m_hWnd==NULL )
		{
			return;
		}
		HH_FTS_QUERY T = {sizeof(HH_FTS_QUERY)};
		



	}
};
	


}; // TNB
