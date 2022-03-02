#pragma once
/**
 *@file
 * CopyDataメッセージ関係のヘッダ
 *
 *		WM_COPYDATAなどを使ったウィンドウ間通信処理をサポートします。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCommunication.h"



//TNB Library
namespace TNB
{



/**@ingroup COMMUNICATION
 * コピーデータメッセージクラス
 *
 *		ICommunication を実装しています。
 *		WM_COPYDATA メッセージを使い、ウィンドウ間でデータを送受信することが出来ます。
 *
 *	@note 受信処理を機能させるために、親のWindowProc をフックし、
 *			本クラスの RelayEvent() をコールする必要が有ります。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *	 LRESULT CDialupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)		
 *	 {
 *		LRESULT res;
 *		if ( m_copyDataMessage.RelayEvent(message, wParam, lParam, res) ) {
 *			return res;
 *		}
 *	 	return CDialog::WindowProc(message, wParam, lParam);
 *	 }
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbCopyDataMessage.h
 * 
 *	@date 08/03/24 新規作成
 *	@date 10/05/28 SetReceiveTimeout() を実装。
 */
class CCopyDataMessage : public ICommunication, CCopyImpossible
{
	DEFSUPER(ICommunication);
	_super::TProperty	m_property;			///< プロパティ
protected:
	CVectorT<BYTE>		m_vbReceivedData;	///< 受信したデータ。
	HWND				m_hPartnerWnd;		///< 送信先のウィンドウ
	ULONG_PTR			m_dwCookie;			///< 識別子

public:

	/** 
	 * コンストラクタ 
	 */
	CCopyDataMessage(void) : m_property(_T("CopyDataMsg"), TNBVIW_KIND_TEMP), m_dwCookie(0), m_hPartnerWnd(NULL)
	{
	}

	/**
	 * [取得] 性能取得
	 *	@return 性能情報
	 */
	virtual const TProperty& GetProperty(void) const
	{
		return m_property; 
	}

	/**
	 * [設定] 受信待ち時間設定.
	 *		Receive() 実行時、受信できるデータが無い時に待つ時間を設定できます。
	 *	@note 成功すると、プロパティが変更される可能性があります。
	 *	@param to タイムアウト時間 (ms)。 
	 *	@return 常に false 。
	 */
	virtual bool SetReceiveTimeout(DWORD to)
	{
		return false;
	}

	/**
	 * [設定] オープン設定
	 *	@param hPartnerWnd 送信先ウィンドウハンドル
	 *	@param dwCookie 識別子
	 */
	void SetParamater(HWND hPartnerWnd, ULONG_PTR dwCookie = 0)
	{
		m_hPartnerWnd = hPartnerWnd;
		m_dwCookie = dwCookie;
	}

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。 SetParamater() で設定した送信先が存在しない場合、失敗します。
	 */
	virtual bool Open(void) 
	{
		return IsOpened();
	}
	
	/**
	 * [処理] クローズ.
	 */
	virtual void Close(void) 
	{
		m_vbReceivedData.RemoveAll();
		return; 
	}
	
	/**
	 * [確認] オープン確認.
	 *	@retval true オープンしている。
	 *	@retval false 失敗。 SetParamater() で設定した送信先が存在しない場合、失敗します。
	 */
	virtual bool IsOpened(void) const 
	{
		return !! ::IsWindow(m_hPartnerWnd);
	}

	/**
	 * [処理] 送信.
	 *	@param size サイズ。
	 *	@param P データ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 上記以外 送信データサイズ。
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( IsOpened() )
		{
			COPYDATASTRUCT data;
			data.dwData = m_dwCookie;
			data.cbData = ToDword(size);
			data.lpData = const_cast<PVOID>(P);
			LPARAM l = reinterpret_cast<LPARAM>(&data);
			DWORD_PTR result = 0;
			if ( ::SendMessageTimeout(m_hPartnerWnd, WM_COPYDATA, 0, l, SMTO_NORMAL, 1000, &result) )
			{
				return result;
			}
		}
		return INVALID_SIZE;
	}

	/**
	 * [処理] 受信.
	 *	@param[in] size サイズ。
	 *	@param[out] _P データ。size 分のメモリは確保しておく必要があります。
	 *	@return 受信データサイズ。
	 */
	virtual size_t Receive(size_t size, LPVOID _P)
	{
		size_t l = m_vbReceivedData.GetElements(size, static_cast<BYTE*>(_P));
		m_vbReceivedData.RemoveElements(0, l);
		return l;
	}

	/**
	 * [確認] 受信データ確認.
	 *	@return 受信可能データサイズ。
	 */
	virtual size_t GetReceivingDataSize(void) const
	{
		return m_vbReceivedData.GetSize();
	}

	/**
	 * [確認] チェック.
	 *		状態を確認します。
	 *	@retval CONNECTING	オープン＆接続状態
	 *	@retval CLOSED		クローズ状態		 
	 */
	virtual EState Check(void) const
	{
		if ( ::IsWindow(m_hPartnerWnd) )
		{
			return CONNECTING; 
		}
		return CLOSED;
	}

	/**
	 * [処理] パージ.
	 */
	virtual void Purge(void)
	{
		m_vbReceivedData.RemoveAll();
	}

	/**
	 * [処理] イベント中継
	 *	@note 親ウィンドウの WindowProc() からメッセージを流してください。
	 *	@param[in] message 親ダイアログが受信したメッセージ
	 *	@param[in] wParam 親ダイアログが受信したメッセージのWPARAM
	 *	@param[in] lParam 親ダイアログが受信したメッセージのLPARAM
	 *	@param[out] _result 本関数が true の際、リザルトとして使う値を格納する。
	 *	@retval true 処理実施。 _result にリザルト値が格納されている。
	 *	@retval false 処理なしだった。
	 */
	bool RelayEvent(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& _result)
	{
		if ( message == WM_COPYDATA )
		{
			COPYDATASTRUCT* pData = reinterpret_cast<COPYDATASTRUCT*>(lParam);
			if ( pData->dwData == m_dwCookie )
			{
				size_t size = pData->cbData;
				PVOID P = pData->lpData;
				_result = m_vbReceivedData.AddElements(size, static_cast<const BYTE*>(P));
				return true;
			}
		}
		return false;
	}
};



/**@ingroup COMMUNICATION
 * コピーデータポストメッセージクラス
 *
 *		ICommunication を実装しています。
 *		PostMessage で任意のメッセージを用い、ウィンドウ間でデータを送受信することが出来ます。
 *
 *	@attention 識別子の上位2bitは、管理に使用してしまいます。
 *
 *	@note 受信処理を機能させるために、親のWindowProc をフックし、
 *			本クラスの RelayEvent() をコールする必要が有ります。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *	 LRESULT CDialupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)		
 *	 {
 *		LRESULT res;
 *		if ( m_copyDataPostMessage.RelayEvent(message, wParam, lParam, res) ) {
 *			return res;
 *		}
 *	 	return CDialog::WindowProc(message, wParam, lParam);
 *	 }
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbCopyDataMessage.h
 * 
 *	@date 08/03/24 新規作成
 */
class CCopyDataPostMessage : public CCopyDataMessage
{
	DEFSUPER(CCopyDataMessage);
	UINT		m_msg;			///< メッセージ
	enum
	{
		COOKIE_MASK		= 0x3FFFFFFF,	///< COOKIEマスク
		HASBYTE_MASK	= 0xC0000000,	///< BYTE情報マスク
		HASBYTE_SHIFT	= 30,			///< BYTE情報シフト値
		HAS1BYTE		= 0x00000000,	///< BYTE情報(1byte)
		HAS2BYTE		= 0x40000000,	///< BYTE情報(2bytes)
		HAS3BYTE		= 0x80000000,	///< BYTE情報(3bytes)
		HAS4BYTE		= 0xC0000000,	///< BYTE情報(4bytes)
	};
public:

	/// コンストラクタ
	CCopyDataPostMessage(void) : _super(), m_msg(0)
	{
	}

	/**
	 * [処理] オープン.
	 *	@retval true 成功。
	 *	@retval false 失敗。 SetMessage() が未設定の場合や、 SetParamater() で設定した送信先が存在しない場合、失敗します。
	 */
	virtual bool Open(void) 
	{
		return m_msg != 0 && IsOpened();
	}
		
	/**
	 * [設定] メッセージ設定
	 *	@param msg 送受信に使うメッセージ。
	 */
	void SetMessage(UINT msg)
	{
		m_msg = msg;
	}

	/**
	 * [設定] メッセージ設定
	 *	@param msg 送受信に使う識別子。
	 */
	void SetMessage(LPCTSTR msg)
	{
		m_msg = ::RegisterWindowMessage(msg);
	}

	/**
	 * [処理] 送信.
	 *	@param size サイズ。
	 *	@param P データ。
	 *	@retval INVALID_SIZE エラー。
	 *	@retval 上記以外 送信データサイズ。
	 */
	virtual size_t Send(size_t size, LPCVOID P)
	{
		if ( ! IsOpened() )
		{
			return INVALID_SIZE;
		}
		ULONG_PTR cookie = m_dwCookie & COOKIE_MASK;
		const BYTE* B = static_cast<const BYTE*>(P);
		INDEX pos = 0;
		loop ( i, size / 4 )
		{
			const DWORD* W = reinterpret_cast<const DWORD*>(&B[pos]);
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS4BYTE, *W);
			pos += 4;
		}
		switch ( size & 3 )
		{
		case 0:
		default:
			break;
		case 1:
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS1BYTE, B[pos]);
			break;
		case 2:
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS2BYTE, B[pos] | (B[pos + 1] << 8));
			break;
		case 3:
			::PostMessage(m_hPartnerWnd, m_msg, cookie | HAS3BYTE, B[pos] | (B[pos + 1] << 8) | (B[pos + 2] << 16));
			break;
		}
		return size;
	}

	/**
	 * [処理] メッセージ中継.
	 *	@note 親ウィンドウの WindowProc() からメッセージを流してください。
	 *	@param[in] message 親ダイアログが受信したメッセージ
	 *	@param[in] wParam 親ダイアログが受信したメッセージのWPARAM
	 *	@param[in] lParam 親ダイアログが受信したメッセージのLPARAM
	 *	@param[out] _result 本関数が true の際、リザルトとして使う値を格納する。
	 *	@retval true 処理実施。 _result にリザルト値が格納されている。
	 *	@retval false 処理なしだった。
	 */
	bool RelayEvent(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& _result)
	{
		if ( message == m_msg )
		{
			if ( (wParam & COOKIE_MASK) == (m_dwCookie & COOKIE_MASK) )
			{
				int len = (ToInt(wParam & HASBYTE_MASK) >> HASBYTE_SHIFT) + 1;
				if ( len >= 1 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>(lParam & 0xFF));
				}
				if ( len >= 2 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>((lParam >> 8) & 0xFF));
				}
				if ( len >= 3 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>((lParam >> 16) & 0xFF));
				}
				if ( len >= 4 )
				{
					m_vbReceivedData.Add(static_cast<BYTE>((lParam >> 24) & 0xFF));
				}
				return true;
			}
		}
		return false;
	}
};



}; // TNB

