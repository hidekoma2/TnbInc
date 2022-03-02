#pragma once
/**
 *@file
 * 生マウス監視関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMultiMouseManager.h"
#include "TnbDummyWindow.h"
#include "TnbSimpleVector.h"
#include "TnbStr.h"
#include "TnbVector.h"
#include "TnbWindowsVersion.h"
#include <dbt.h>



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
_SELECTANY HHOOK s_hTnbRmwLowMouse = NULL;
#endif



/**@ingroup WINAPI
 * 生マウス監視クラス
 *
 *		生のマウスのデータを取得することが出来ます。複数のマウスを接続している場合、それぞれのデータを取得できます。
 *
 *	@par使用例
 *		\code
 *		// ヘッダ
 *		class CFooDlg : public CDialog, CRawMouseWatcher::IListener
 *		{
 *					;
 *			virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse);
 *			virtual void OnMultiMouseDevice(INDEX index, EDevice device);
 *					;
 *			CRawMouseWatcher	m_mouseWatcher;
 *					;
 *
 *		// ソース
 *		void CFooDlg::XX(void)
 *		{
 *			m_mouseWatcher.SetListener(this);
 *			m_mouseWatcher.Start();
 *		}
 *		void CFooDlg::OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse)
 *		{
 *			int x = mouse.lLastX; // X 移動量
 *			int y = mouse.lLastY; // Y 移動量
 *			if ( (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) != 0 )
 *			{
 *				//左ボタン押下
 *			}
 *			if ( (mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0 )
 *			{
 *				//ホイール
 *				short d = mouse.usButtonData;
 *			}
 *		}
 *		void CFooDlg::OnMultiMouseDevice(INDEX index, EDevice device)
 *		{
 *		}
 *		\endcode
 *
 *	@attention XP 以降のみ有効な API を使用しているため、最初に TnbHotWinver.h をインクルードしてください。
 *
 *	@par必要ファイル
 *			TnbRawMouseWatcher.h
 *
 *	@date 10/04/20	新規作成
 *	@date 10/06/17	IMultiMouseWatcher のサブクラス化
 *	@date 10/07/14	EnableMouseCursor() を追加。
 *	@date 10/07/23	HorizontalWheelEvent() を追加。
 */
class CRawMouseWatcher : public IMultiMouseWatcher
{
	DEFSUPER(IMultiMouseWatcher);
public:

	/// コンストラクタ
	CRawMouseWatcher(void)
	{
		Refresh();
	}

	/**
	 * [登録] リスナー登録.
	 *	@param P リスナー
	 */
	virtual void SetListener(IMultiMouseWatcher::IListener* P)
	{
		m_in.m_pListener = P;
	}

	/**
	 * [取得] マウス数取得.
	 *	@return マウス数.
	 */
	virtual size_t GetMouseCount(void) const
	{
		return m_in.m_devsList.GetSize();
	}

	/**
	 * [設定] 監視開始.
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool Start(void)
	{
		Stop();
		if ( m_win.Create(&m_in) )
		{
			RAWINPUTDEVICE rid;
			rid.usUsagePage = 1/*HID_USAGE_PAGE_GENERIC*/; 
			rid.usUsage = 2/*HID_USAGE_GENERIC_MOUSE*/; 
			rid.dwFlags = RIDEV_INPUTSINK;
			rid.hwndTarget = m_win;
			if ( ::RegisterRawInputDevices(&rid, 1, sizeof(rid)) )
			{
				return true;
			}
		}
		Stop();
		return false;
	}

	/**
	 * [設定] 監視停止.
	 */
	virtual void Stop(void)
	{
		m_win.Destroy();
	}

	/**
	 * [取得] マウススペック取得.
	 *	@param[out] _spec スペックが代入されます。
	 *	@param[in] index マウスインデックス。 ０以上、 GetMouseCount() 未満の値です。
	 *	@retval true 取得成功
	 *	@retval false 失敗（index の値がおかしい）
	 */
	bool GetMouseSpecified(RID_DEVICE_INFO_MOUSE& _spec, INDEX index) const
	{
		if ( GetMouseCount() > index )
		{
			_spec = m_in.m_devsList[index].specified;
			return true;
		}
		return false;
	}

	/**
	 * [取得] マウスデバイス名取得.
	 *	@param[in] index マウスインデックス。 ０以上、 GetMouseCount() 未満の値です。
	 *	@return マウスデバイス名
	 */
	CStr GetMouseName(INDEX index) const
	{
		if ( GetMouseCount() > index )
		{
			return m_in.m_devsList[index].name;
		}
		return _T("");
	}

	/**
	 * [設定] 再検索.
	 *	@note デバイスの状態が変わったとき、本インスタンスは自動的に追従します。
	 *		本メソッドをコールすると、マウスインデックスも初期化されます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Refresh(void)
	{
		return m_in.Refresh();
	}

	/**
	 * [設定] システムマウス有効無効設定.
	 *		マウスを動かしても、システムマウスを動かなくすることが出来ます。
	 *		この状態は MouseEvent() による操作指示のみ有効になります。
	 *	@note 本メソッドの設定は、プロセスが終了したら元に戻ります。
	 *	@param isEnable false ならマウスデバイスでマウスカーソルは動かないようします。 MouseEvent() では動かすことが出来ます。\n
	 *				true なら通常通りです。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool EnableMouseCursor(bool isEnable)
	{
		if ( ! isEnable )
		{
			//無効へ
			if ( s_hTnbRmwLowMouse == NULL )
			{
				s_hTnbRmwLowMouse = ::SetWindowsHookEx(WH_MOUSE_LL, ms_OnLowLevelMouseProc, GetInstanceHandleByTnb(), 0);
			}
			return s_hTnbRmwLowMouse != NULL;
		}
		// 有効へ
		if ( s_hTnbRmwLowMouse != NULL ) 
		{
			::UnhookWindowsHookEx(s_hTnbRmwLowMouse);
			s_hTnbRmwLowMouse = NULL;
		}
		return s_hTnbRmwLowMouse == NULL;
	}

	/**
	 * [設定] システムマウスイベント.
	 *		システムマウスイベントを発行します。 
	 *	@note EnableMouseCorsor で false を指定していても、機能します。
	 *	@note 引数は ::mouse_event() と同じです。
	 *	@note 水平ホイールは、 HorizontalWheelEvent() を使います。
	 *	@param flags フラグ
	 *	@param dx X
	 *	@param dy Y
	 *	@param data データ
	 */
	static void MouseEvent(DWORD flags, DWORD dx = 0,DWORD dy = 0, DWORD data = 0)
	{
		::mouse_event(flags, dx, dy, data, COOKIE);
	}

	/**
	 * [設定] システム水平ホイールイベント.
	 *		水平ホイールを実現します。 
	 *	@note EnableMouseCorsor で false を指定していても、機能します。
	 *	@note VISTA以降ではマウスイベント、以前ではスクロールメッセージで対応します。
	 *	@param delta 移動量。 WHEEL_DELTA の倍数で指定します。
	 *	@todo 現在 Opera で動作しないことを確認。
	 *	@todo 現在 Excel の分割状態ではフォーカス通り動作しないことを確認。
	 */
	static void HorizontalWheelEvent(int delta)
	{
		//CWindowsVersion wv;	@todo VISTA でも HWHEEL メッセージに対応していないウィンドウが多い・・・。
		//if ( wv.GetMajorVersion() >= 6 )
		//{
		//	MouseEvent(0x1000/*MOUSEEVENTF_HWHEEL*/, 0, 0, delta);
		//	return;
		//}
		// 現在のフォアグラウンドのウィンドウ取得
		HWND hWnd = ::GetForegroundWindow();
		TTRACE1("GetForegroundWindow = %08X\n", hWnd);
		// アクティブなウインドウのスレッドIDを取得する
		DWORD dwCurrentThreadId = ::GetCurrentThreadId();
		DWORD dwActiveProcessId;
		DWORD dwActiveThreadId = ::GetWindowThreadProcessId(hWnd, &dwActiveProcessId);
		if ( ::AttachThreadInput(dwCurrentThreadId, dwActiveThreadId, TRUE) )
		{
			hWnd = ::GetFocus();
			if ( hWnd != NULL )
			{	
				LONG style = ::GetWindowLong(hWnd, GWL_STYLE);
				bool hasScrollBar = (style & WS_HSCROLL) != 0;
				#ifdef _DEBUG
					CStr s;
					::GetClassName(hWnd, s.GetBuffer(100), 100);
					s.ReleaseBuffer();
					TTRACE3(" GetFocus = %08X %d, [%s]\n", hWnd, hasScrollBar, s);
				#endif
				bool isDisp = false;
				if ( ! hasScrollBar )
				{
					// memo; WORD は兄弟に ScrollBar がある。
					isDisp = ms_SubWheel(hWnd, delta);
					if ( ! isDisp )
					{
						// memo; EXCEL は一個下に ScrollBar がある。
						HWND h = ::GetWindow(hWnd, GW_CHILD);
						isDisp = ms_SubWheel(h, delta);
					}
				}
				if ( ! isDisp )
				{
					ms_PostWheel(hWnd, NULL, delta);
				}
			}
			// 開放する
			::AttachThreadInput(dwCurrentThreadId, dwActiveThreadId, FALSE);
		}
	}

private:

	enum
	{
		COOKIE = 'TbMm'	///< 専用クッキー
	};

	/// 情報
	struct TInfo : TNB::IComparableT<TInfo>
	{
		HANDLE					handle;		///< ハンドル。 INVALID_HANDLE_VALUE なら今は未接続状態
		CStr					name;		///< 名前。
		RID_DEVICE_INFO_MOUSE	specified;	///< スペック。
		TInfo(void) : handle(INVALID_HANDLE_VALUE)
		{
		}
		virtual INT_PTR Compare(const TInfo& t) const
		{
			return name.Compare(t.name);
		}
	};
	/// 内部用クラス
	class CInner : public CDummyWindow::IListener
	{
		DEFPARENTLISTENER(IMultiMouseWatcher, IParentListener);
	public:
		/// コンストラクタ
		CInner(void) : m_pListener(NULL), m_lastHandle(INVALID_HANDLE_VALUE)
		{
		}
		/**
		 * [通知] 受信メッセージ通知.
		 *	@retval true 処理済(DefWindowProc()を呼び出さなくなります)
		 *	@retval false 未処理
		 */
		virtual bool OnWindowMessage(LRESULT& _result, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if ( m_pListener != NULL )
			{
				if ( message == WM_DEVICECHANGE && wParam == DBT_DEVNODES_CHANGED )
				{
					CVectorT<TInfo> newInf;
					if ( Collect(newInf) )
					{
						loop ( i, m_devsList )
						{
							INDEX idx = newInf.Find(m_devsList[i]);
							if ( idx != INVALID_INDEX )
							{
								// 再発見された
								if ( m_devsList[i].handle == INVALID_HANDLE_VALUE )
								{
									m_devsList[i] = newInf[idx];
									m_pListener->OnMultiMouseDevice(i, _super::IListener::Device_Reconnect);
								}
							}
							else
							{
								// 無くなった
								if ( m_devsList[i].handle != INVALID_HANDLE_VALUE )
								{
									m_devsList[i].handle = INVALID_HANDLE_VALUE;
									m_pListener->OnMultiMouseDevice(i, _super::IListener::Device_Remove);
								}
							}
						}
						loop ( i, newInf )
						{
							INDEX idx = m_devsList.Find(newInf[i]);
							if ( idx == INVALID_INDEX )
							{
								// 新規発見
								int x = m_devsList.Add(newInf[i]);
								m_pListener->OnMultiMouseDevice(x, _super::IListener::Device_Connect);
							}
						}
					}
				}
				else if ( message == WM_INPUT )
				{
					UINT size = 40;
					CWorkMem w(size);
					::GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, w, &size, sizeof(RAWINPUTHEADER));
					RAWINPUT* P = reinterpret_cast<RAWINPUT*>(w.Ref());
					if ( P->header.dwType == RIM_TYPEMOUSE )
					{
						INDEX idx = INVALID_INDEX;
						if ( m_lastHandle == P->header.hDevice )
						{
							idx = m_lastIndex;
						}
						else
						{
							loop ( i, m_devsList.GetSize() )
							{
								if ( m_devsList[i].handle == P->header.hDevice )
								{
									idx = i;
									m_lastHandle = P->header.hDevice;
									m_lastIndex = idx;
									break;
								}
							}
						}
						if ( idx != INVALID_INDEX )
						{
							m_pListener->OnMultiMouseEvent(idx, P->data.mouse);
						}
					} 
				}
			}
			return false;
		}
		/// 再検索
		bool Refresh(void)
		{
			m_lastHandle = INVALID_HANDLE_VALUE;
			return Collect(m_devsList);
		}
		/// 情報収集
		bool Collect(CVectorT<TInfo>& _info)
		{
			_info.RemoveAll();
			UINT count = 0;
			UINT rr = ::GetRawInputDeviceList(NULL, &count, sizeof(RAWINPUTDEVICELIST));
			if ( rr != 0 )
			{
				return false;
			}
			UINT d1 = count;
			CWorkMemT<RAWINPUTDEVICELIST> devs(count);
			UINT d2 = ::GetRawInputDeviceList(devs, &count, sizeof(RAWINPUTDEVICELIST));
			if ( d1 != d2 )
			{
				return false;
			}
			loop ( i, devs.GetSize() )
			{
				if ( devs[i].dwType != RIM_TYPEMOUSE )
				{
					continue;
				}
				TInfo info;
				info.handle = devs[i].hDevice;
				// スペック取得
				RID_DEVICE_INFO dev;
				UINT size = sizeof(RID_DEVICE_INFO);
				if ( ::GetRawInputDeviceInfo(info.handle, RIDI_DEVICEINFO, &dev, &size) > 0 )
				{
					info.specified = dev.mouse;
					// デバイス名の長さ取得
					if ( ::GetRawInputDeviceInfo(info.handle, RIDI_DEVICENAME, NULL, &size) == 0 )
					{
						// デバイス名取得
						CWorkMemT<TCHAR> name(size + 1);
						int r = ::GetRawInputDeviceInfo(info.handle, RIDI_DEVICENAME, name, &size);
						if ( r > 0 )
						{
							info.name = name.Ref();
						}
					}
					_info.Add(info);
				}
			}
			return true;
		}
		IParentListener*		m_pListener;	///< リスナー
		CVectorT<TInfo>			m_devsList;		///< デバイスリスト
		mutable HANDLE			m_lastHandle;	///< ラストハンドル(cache)
		mutable INDEX			m_lastIndex;	///< ラストインデックス(cache)
	};
	/**
	 * [通知] ローレベルマウス PROC.
	 *		マウス動作をフック.
	 */
	static LRESULT CALLBACK ms_OnLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if ( nCode == HC_ACTION )
		{
			MSLLHOOKSTRUCT* P = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
			TTRACE3("OnLowLevelMouseProc msg=0x%X, x=%d, y=%d\n", wParam, P->pt.x, P->pt.y);
			TTRACE3("         data=%d, flags=0x%X, ex-info=%d\n", P->mouseData, P->flags, P->dwExtraInfo);
			if ( P->dwExtraInfo != COOKIE )
			{
				return 1;
			}
		} 
		return ::CallNextHookEx(s_hTnbRmwLowMouse, nCode, wParam, lParam);
	}
	// CLASS 名チェック
	static bool ms_IsScrollBar(LPCTSTR P)
	{
		const TCHAR src[] = _T("ScrollBar");
		loop ( i, sizeof(src) )
		{
			if ( src[i] != P[i] )
			{
				return false;
			}
		}
		return true;
	}
	// 送信
	static void ms_PostWheel(HWND hTargetWnd, HWND hCtrlWnd, int delta)
	{
		UINT msg = WM_HSCROLL;
		WPARAM wp = (delta < 0) ? SB_LINELEFT : SB_LINERIGHT;
		LPARAM lp = reinterpret_cast<LPARAM>(hCtrlWnd);
		int l = ((delta < 0) ? -delta : delta) / WHEEL_DELTA;
		loop ( i, l )
		{
			::PostMessage(hTargetWnd, msg, wp, lp);
		}
		::PostMessage(hTargetWnd, msg, SB_ENDSCROLL, lp);
	}
	// サブ
	static bool ms_SubWheel(HWND hWnd, int delta)
	{
		CWorkMemT<TCHAR> buf(100);
		HWND h = ::GetWindow(hWnd, GW_HWNDNEXT);
		while ( h != NULL )
		{
			::GetClassName(h, buf, 100);
			LONG style = ::GetWindowLong(h, GWL_STYLE);
			bool isVisible = (style & WS_VISIBLE) != 0;
			bool isHorz = (style & (SBS_VERT | SBS_SIZEBOX)) == 0;
			if ( ms_IsScrollBar(buf) && isVisible && isHorz )
			{
				TTRACE2(" Found ScrollBar = %08X, [%s]\n", h, buf.Ref());
				HWND h2 = ::GetParent(h);
				if ( h2 != NULL )
				{
					// 親に通知
					ms_PostWheel(h2, h, delta);
					return true;
				}
			}
			h = ::GetWindow(h, GW_HWNDNEXT);
		}
		return false;
	}
	CInner			m_in;						///< 内部クラス
	CDummyWindow	m_win;						///< イベント受信用ウィンドウ
};



}; //TNB



