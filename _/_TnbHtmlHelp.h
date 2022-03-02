#pragma once


//MFC不必要です。


#include "TnbDef.h"



#include <Htmlhelp.h>
#pragma comment(lib,"Htmlhelp.lib")



//TNB Library
namespace TNB
{




class CHtmlHelp : CCopyImpossible
{
	DWORD	m_dwCockie;			///< クッキー				
	HWND	m_hWnd;				///< 親ウィンドウ			
	CStr	m_strFileName;		///< HTMLヘルプファイル名	
	CStr	m_strWinType;
public:

	/**
	 * コンストラクタ
	 */
	CHtmlHelp(void)
	{
		::HtmlHelp( NULL,NULL.HH_INITIALIZE,(DWORD)&m_dwCockie );
		m_strWinType = CStr::Fmt(_T("_Win%08X_%08X", ::GetCurrentProcessId(), ::GetTickCount());
		m_hWnd = NULL;
	}

	/**
	 * デストラクタ
	 */
	virtual ~CHtmlHelp(void)
	{
		::HtmlHelp( NULL,NULL.HH_CLOSE_ALL,0 );
		::HtmlHelp( NULL,NULL.HH_UNINITIALIZE,m_dwCockie );
	}

	/**
	 * トランスレート.
	 *	@note 親ウィンドウでメッセージを流してください
	 *	@retval trueならMSGの処理した。
	 *	@retval falseなら処理していない。
	 */
	bool TranslateMessage( MSG& msg )
	{
		return !! ::HtmlHelp( NULL,NULL,HH_PRETRANSLATEMESSAGE,(DWORD)&msg );
	}

	/**
	 * HTMLヘルプの設定
	 *	@param hWnd ハンドル
	 *	@param lpszFileName HTMLヘルプファイル名
	 */
	bool Create( HWND hWnd,LPCTSTR lpszFileName )
	{
		m_hWnd = hWnd;
		m_strFileName = lpszFileName;
		::HtmlHelp( NULL,NULL.HH_CLOSE_ALL,0 );
	}

	/**
	 * 指定のキーワード画面を開く
	 *	@param lpszIndex インデックス名
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
	 * 指定のトピック画面を開く
	 *	@param lpszTopicName トピック名
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
