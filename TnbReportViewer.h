#pragma once
/**
 *@file
 * レポートビュワー関係のヘッダ
 *
 *		T-Report Viewer によるログをサポートするクラスがあります。
 *
 *	@note	マクロ "_TnbREPORT_Viewer_ON" が定義されているか、デバグビルドなら、 Viewer によるログをとることが出来ます。
 *
 *	@note CE の場合、処理は無視されます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifdef _UNICODE
#include "TnbStr.h"
#endif
#include "TnbSync.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#ifdef _DEBUG
 #define _TnbREPORT_Viewer_ON
#endif

#if defined(_WIN32_WCE) && defined(_TnbREPORT_Viewer_ON)
 #undef _TnbREPORT_Viewer_ON
#endif

/*
 * 通信種別
 *	[31-28]		空
 *	[27-24]		カテゴリ(RS232c,Socket など)
 *	[23]		0;送信 1;受信
 *	[22]		0;バイナリ 1;テキスト
 *	[21-16]		空
 *	[15-0]		0～0xFFFF;ポート番号、ATOM識別子
*/
#define TNBVIW_KIND_RS232C		(0x01<<24)		//Rs232c
#define TNBVIW_KIND_SOCK_S		(0x02<<24)		//Socket(Server)
#define TNBVIW_KIND_SOCK_C		(0x03<<24)		//Socket(Client)
#define TNBVIW_KIND_SOCK_U		(0x04<<24)		//Socket(UDP)
#define TNBVIW_KIND_MAILS		(0x05<<24)		//Mailslot
#define TNBVIW_KIND_USB			(0x06<<24)		//USB
#define TNBVIW_KIND_FILE		(0x07<<24)		//FILE
#define TNBVIW_KIND_TEMP		(0x08<<24)		//ByteTemp
#define TNBVIW_KIND_PIPE		(0x09<<24)		//パイプ
#define TNBVIW_KIND_COPYDATA	(0x0B<<24)		//CopyData(Message)
#define TNBVIW_KIND_ATOM		(0x0F<<24)		//汎用
#define TNBVIW_DIRE_SEND		0				//送信
#define TNBVIW_DIRE_RECV		(int(1<<23))	//受信
#define TNBVIW_TYPE_BIN			0				//バイナリ
#define TNBVIW_TYPE_TEXT		(int(1<<22))	//テキスト
#define TNBVIW_TYPE_RAWTEXT		(int(1<<21))	//生テキスト
#define TNBVIW_PORT(P)			(P)				//Port番号 0～0xFFFF

/// 情報ビュワーウィンドウタイトル名
#define TNB_TNBVIEWER_WND_TITLE "TnbCommViewer"
#define TNB_TNBVIEWER_WND_CLASS "TnbReport"

/// 情報ビュワーCOM情報通知メッセージ
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
 * TNBレポートビュワー
 *
 *		T-Report Viewer にログを出力します。
 *		コンストラクタの時に T-Report Viewer を探し、無ければ以降何もしません。
 *
 *	@par必要ファイル
 *			TnbReportViewer.h
 * 
 *	@date 10/02/05 抽象化して作成
 *	@date 11/03/10 IsValid() 追加。
 *	@date 12/03/29 Invalid() 追加。
 */
class CReportViewer
{
public:

	/**
	 * コンストラクタ.
	 *	@note このコンストラクタの時 T-Report Viewer を探します。 
	 */
	CReportViewer(void)
	{
		#ifdef _TnbREPORT_Viewer_ON
			m_hPostWnd = ::FindWindow(NULL, _T(TNB_TNBVIEWER_WND_TITLE));
		#endif
	}

	/**
	 * [確認] 有効確認.
	 *	@retval true T-Report Viewer に出力できます。
	 *	@retval false 否
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
	 * [設定] 無効化.
	 *		T-Report Viewer への出力を無効にします。本メソッド実行後、 IsValid() は false を返すようにします。
	 *	@note 再び有効にする方法はありません。
	 */
	void Invalid(void)
	{
		#ifdef _TnbREPORT_Viewer_ON
			m_hPostWnd = NULL;
		#endif
	}

	/**
	 * [出力] ダンプ出力.
	 *	@param sizeData データサイズ.
	 *	@param lpcvData データ
	 *	@param cookie クッキー。 TNBVIW_KIND_??? + PORT を指定します。
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
	 * [出力] 文字列出力.
	 *	@param lpszText 文字列.
	 *	@param cookie クッキー。 TNBVIW_KIND_??? + PORT を指定します。
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
	 * [内部] Viewerへ通知
	 *	@param cookie クッキー。
	 *	@param dataSize 送信するデータ長。
	 *	@param lpcvData 送信するデータ。
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
	HWND		m_hPostWnd;				///< 受信 Viewer ウィンドウハンドル
	#endif
};



/**@ingroup REPORT
 * TNBレポートビュワー
 *
 *		T-Report Viewer にログを出力します。
 *		コンストラクタの時に T-Report Viewer を探し、無ければ以降何もしません。
 *
 *	@par必要ファイル
 *			TnbReportViewer.h
 * 
 *	@date 11/03/10 新規作成
 *	@date 11/10/14 CSyncSection を実装。
 *	@date 12/03/29 Invalid() 追加。
 */
class CReportViewerEx : public CSyncSection
{
public:

	/**
	 * コンストラクタ.
	 *	@note このコンストラクタの時 T-Report Viewer を探します。 
	 */
	CReportViewerEx(void) : m_cookieBase(0), m_atom(NULL)
	{
	}

	/// デストラクタ
	~CReportViewerEx(void)
	{
		m_DeleteAtom();
	}

	/**
	 * [設定] 種類設定.
	 *	@param kind TNBVIW_KIND_??? + PORT を指定します。	 
	 */
	void SetKind(DWORD kind)
	{
		m_DeleteAtom();
		m_cookieBase = kind;
	}

	/**
	 * [設定] 種類設定.
	 *	@param lpszTitle 種類名(T-Report Viewer 上の種別になります)
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [確認] 種類設定済み確認.
	 *	@retval true 設定済み
	 *	@retval false 否
	 */
	bool HasKind(void) const
	{
		return m_cookieBase != 0;
	}

	/**
	 * [確認] 有効確認.
	 *	@retval true T-Report Viewer に出力できます。
	 *	@retval false 否
	 */
	bool IsValid(void) const
	{
		return m_viewer.IsValid();
	}

	/**
	 * [設定] 無効化.
	 *		T-Report Viewer への出力を無効にします。本メソッド実行後、 IsValid() は false を返すようにします。
	 *	@note 再び有効にする方法はありません。
	 */
	void Invalid(void)
	{
		m_viewer.Invalid();
	}

	/**
	 * [出力] ダンプ出力.
	 *	@param sizeData データサイズ.
	 *	@param lpcvData データ
	 *	@param dire 方向。 TNBVIW_DIRE_SEND か TNBVIW_DIRE_RECV を指定します。
	 */
	void ToViewer(size_t sizeData, LPCVOID lpcvData, DWORD dire = 0)
	{
		EXCLUSIVE( this );
		m_viewer.ToViewer(sizeData, lpcvData, dire | m_cookieBase);
	}

	/**
	 * [出力] 文字列出力.
	 *	@param lpszText 文字列.
	 */
	void ToViewer(LPCTSTR lpszText)
	{
		EXCLUSIVE( this );
		m_viewer.ToViewer(lpszText, TNBVIW_TYPE_RAWTEXT | m_cookieBase);
	}

private:
	/// ATOM破棄
	void m_DeleteAtom(void)
	{
		if ( m_atom != NULL )
		{
			::GlobalDeleteAtom(m_atom);
			m_atom = NULL;
		}
	}
	ATOM			m_atom;			///< アトム
	DWORD			m_cookieBase;	///< クッキーベース
	CReportViewer	m_viewer;		///< ビュワー
};



}; // TNB
