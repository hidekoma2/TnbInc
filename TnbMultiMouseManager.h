#pragma once
/**
 *@file
 * マルチマウス管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDummyWindow.h"
#include "TnbPointerVector.h"
#include "TnbBitmapImage.h"
#include <math.h>



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * マルチマウス監視インターフェース.
 *
 *		複数のマウスのデータを取得するインターフェースです。
 *
 *	@attention XP 以降のみ有効な API を使用しているため、最初に TnbHotWinver.h をインクルードしてください。
 *
 *	@par必要ファイル
 *			TnbMultiMouseManager.h
 *
 *	@date 10/06/17	CRawMouseWatcher を抽象化して作成。
 */
struct IMultiMouseWatcher
{
public:

	/**@ingroup WINAPI
	 * マルチマウス監視のリスナー
	 *
	 *	@par必要ファイル
	 *			TnbMultiMouseManager.h
	 *
	 *	@date 10/06/17	CRawMouseWatcher を抽象化して作成。
	 */
	struct IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/// デバイス変化
		enum EDevice
		{
			Device_Connect,		///< 接続
			Device_Reconnect,	///< 再接続
			Device_Remove,		///< 削除
		};

		/**
		 * [通知] デバイス変化通知.
		 *	@param index マウスインデックス.
		 *	@param device デバイス変化
		 */
		virtual void OnMultiMouseDevice(INDEX index, EDevice device) = 0;

		/**
		 * [通知] イベント通知.
		 *	@param index マウスインデックス.
		 *	@param mouse 情報
		 */
		virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse) = 0;
	};


	//--------------------------------


	/**
	 * [登録] リスナー登録.
	 *	@param P リスナー
	 */
	virtual void SetListener(IListener* P) = NULL;

	/**
	 * [取得] マウス数取得.
	 *	@return マウス数.
	 */
	virtual size_t GetMouseCount(void) const = NULL;

	/**
	 * [設定] 監視開始.
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool Start(void) = NULL;

	/**
	 * [設定] 監視停止.
	 */
	virtual void Stop(void) = NULL;
};



/**@ingroup WINAPI
 * マルチマウス管理クラス
 *
 *		複数のマウスを管理します。カーソルの表示、移動、イベントの通知をそれぞれで管理できます。
 *
 *	@par使用例
 *		\code
 *		// ヘッダ
 *		class CFooDlg : public CDialog, CMultiMouseManager::IListener
 *		{
 *					;
 *			virtual void OnMultiMouseDevice(INDEX index, EDevice device);
 *			virtual void OnMultiMouseButton(INDEX index, EButton button, EEvent event, int x, int y);
 *			virtual void OnMultiMouseMove(INDEX index, BYTE buttonFlags, int x, int y);
 *			virtual void OnMultiMouseWheel(INDEX index, int d);
 *					;
 *			CMultiMouseManager		m_mmm;
 *			CRawMouseWatcher		m_rmw;
 *					;
 *
 *		// ソース
 *		void CFooDlg::XX(void)
 *		{
 *			m_mmm.SetListener(this);
 *			m_mmm.SetDefaultCursorImage(6, 6, IDB_POINTER); //カーソル形指定
 *			m_mmm.EnableMouseCursor(false); //システムカーソル動作制限
 *			ShowCursor(FALSE);  //必要に応じてシステムカーソルを非表示に
 *			m_mmm.Start(&m_rmw, GetSafeHwnd()); //開始
 *		}
 *		void CFooDlg::OnMultiMouseDevice(INDEX index, EDevice device)
 *		{
 *		}
 *		void CFooDlg::OnMultiMouseButton(INDEX index, EButton button, EEvent event, int x, int y)
 *		{
 *		}
 *		void CFooDlg::OnMultiMouseMove(INDEX index, BYTE buttonFlags, int x, int y)
 *		{
 *		}
 *		void CFooDlg::OnMultiMouseWheel(INDEX index, int d)
 *		{
 *		}
 *		\endcode
 *
 *	@attention XP 以降のみ有効な API を使用しているため、最初に TnbHotWinver.h をインクルードしてください。
 *
 *	@par必要ファイル
 *			TnbMultiMouseManager.h
 *
 *	@date 10/04/20	新規作成
 *	@date 10/06/17	IMultiMouseWatcher を使用するように変更。
 *	@date 10/07/14	スレッドセーフ化。
 *	@date 10/07/14	EnableMouseCursor() を廃止(引越し)。
 */
class CMultiMouseManager
{
public:
	
	/**@ingroup WINAPI
	 * マルチマウス管理のリスナー
	 *
	 *	@par必要ファイル
	 *			TnbMultiMouseManager.h
	 *
	 *	@date 10/04/20	新規作成
	 *	@date 10/06/17	IMultiMouseWatcher のサブクラス化
	 */
	struct IListener : IMultiMouseWatcher::IListener
	{
		/// デストラクタ
		virtual ~IListener(void) {}

		/// ボタン種類
		enum EButton
		{
			Button_Left,		///< 左
			Button_Right,		///< 右
			Button_Middle,		///< 中央
		};

		/// イベント
		enum EEvent
		{
			Event_Up,			///< アップ
			Event_Down,			///< ダウン
		};

		/**
		 * [通知] ボタン通知.
		 *	@param index マウスインデックス.
		 *	@param button ボタン種類
		 *	@param event イベント
		 *	@param x X 座標
		 *	@param y Y 座標
		 */
		virtual void OnMultiMouseButton(INDEX index, EButton button, EEvent event, int x, int y) = 0;

		/**
		 * [通知] 移動通知.
		 *	@param index マウスインデックス.
		 *	@param buttonFlags キー状態。 _BIT(EButton) で ＆ して ０以外ならプレスされています。
		 *	@param x X 座標
		 *	@param y Y 座標
		 */
		virtual void OnMultiMouseMove(INDEX index, BYTE buttonFlags, int x, int y) = 0;

		/**
		 * [通知] ホイール通知.
		 *	@param index マウスインデックス.
		 *	@param d ホイール移動量
		 */
		virtual void OnMultiMouseWheel(INDEX index, int d) = 0;

		/**
		 * [通知] イベント通知.
		 *	@param index マウスインデックス.
		 *	@param mouse 情報
		 */
		virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse) { }

		#ifdef _TnbDOXYGEN	//Document作成用シンボル

		/// デバイス変化
		enum EDevice
		{
			Device_Connect,		///< 接続
			Device_Reconnect,	///< 再接続
			Device_Remove,		///< 削除
		};

		/**
		 * [通知] デバイス変化通知.
		 *	@param index マウスインデックス.
		 *	@param device デバイス変化
		 */
		virtual void OnMultiMouseDevice(INDEX index, EDevice device) = 0;

		#endif

		/**
		 * [作成] マウスイベントフラグ作成.
		 *	@param button ボタン種類
		 *	@param event イベント
		 *	@return マウスイベントフラグ.　
		 */
		static DWORD MakeMouseEventFlags(EButton button, EEvent event)
		{
			if ( event == Event_Down )
			{
				switch ( button )
				{
				case Button_Left:	return MOUSEEVENTF_LEFTDOWN;
				case Button_Right:	return MOUSEEVENTF_RIGHTDOWN;
				case Button_Middle:	return MOUSEEVENTF_MIDDLEDOWN;
				}
			}
			else if ( event == Event_Up )
			{
				switch ( button )
				{
				case Button_Left:	return MOUSEEVENTF_LEFTUP;
				case Button_Right:	return MOUSEEVENTF_RIGHTUP;
				case Button_Middle:	return MOUSEEVENTF_MIDDLEUP;
				}
			}
			return 0;
		}
	};


	//-----------------------------------


	/// コンストラクタ
	CMultiMouseManager(void) : m_pWatcher(NULL)
	{
		::SetRectEmpty(&m_in.m_corsorRect);
	}

	/**
	 * [登録] リスナー登録.
	 *	@param P リスナー
	 */
	void SetListener(IListener* P)
	{
		EXCLUSIVE( &m_in.m_sync );
		m_in.m_pListener = P;
	}

	/**
	 * [設定] 開始.
	 *	@param pWatcher マウスウォッチャーのインスタンス。開始していないものを指定します。
	 *	@param hParent 親ウィンドウハンドル。省略すると、グローバルで動作します。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool Start(IMultiMouseWatcher* pWatcher, HWND hParent = NULL)
	{
		EXCLUSIVE( &m_in.m_sync );
		Stop();
		m_pWatcher = pWatcher;
		m_pWatcher->SetListener(&m_in);
		if ( ! m_in.m_cursorImage.IsNull() )
		{
			m_in.m_hParent = hParent;
			if ( m_pWatcher->Start() )
			{
				size_t len = m_pWatcher->GetMouseCount();
				m_in.m_cursores.SetSize(len);
				loop ( i, len )
				{
					if ( ! m_in.CheckIndex(i) )
					{
						return false;
					}
				}
				return true;
			}
			Stop();
		}
		return false;
	}

	/**
	 * [設定] 停止.
	 *		開始していない場合、何もしません。
	 */
	void Stop(void)
	{
		EXCLUSIVE( &m_in.m_sync );
		if ( m_pWatcher != NULL )
		{
			m_pWatcher->Stop();
			m_in.m_cursores.RemoveAll();
			m_pWatcher = NULL;
		}
	}

	/**
	 * [設定] デフォルトカーソルイメージ設定.
	 *	@param hotspotX ホットスポット X
	 *	@param hotspotY ホットスポット Y
	 *	@param bmp 画像.
	 *	@param color 透過色.省略すると、画像の左下の色を適用します。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetDefaultCursorImage(int hotspotX, int hotspotY, CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		EXCLUSIVE( &m_in.m_sync );
		m_in.m_cursorImage = bmp;
		m_in.m_cursorMaskColor = color;
		m_in.m_cursorHotspot.x = hotspotX;
		m_in.m_cursorHotspot.y = hotspotY;
		return ! m_in.m_cursorImage.IsNull();
	}

	/**
	 * [設定] デフォルトカーソルイメージ設定.
	 *	@note ホットスポットは左上になります。
	 *	@param bmp 画像.
	 *	@param color 透過色.省略すると、画像の左下の色を適用します。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetDefaultCursorImage(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		return SetDefaultCursorImage(0, 0, bmp, color);
	}

	/**
	 * [設定] デフォルトカーソルイメージ設定.
	 *	@note アニメーションカーソルには対応していません。
	 *	@param hCursor カーソル.
	 *	@param color 処理用の背景色。カーソルに含まれない色を指定してください。省略するとマゼンタが指定される。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetDefaultCursorImage(HCURSOR hCursor, COLORREF color = RGB(255, 0, 255))
	{
		EXCLUSIVE( &m_in.m_sync );
		CBitmapHandle bmp;
		POINT hotspot;
		if ( m_IconToBitmap(bmp, hotspot, hCursor, color) )
		{
			m_in.m_cursorImage = bmp;
			m_in.m_cursorMaskColor = color;
			m_in.m_cursorHotspot = hotspot;
			return ! m_in.m_cursorImage.IsNull();
		}
		return false;
	}

	/**
	 * [取得] マウス数取得.
	 *		現在認識しているマウス数を取得できます。
	 *	@return マウス数.
	 */
	size_t GetMouseCount(void) const
	{
		EXCLUSIVE( &m_in.m_sync );
		return m_in.m_cursores.GetSize();
	}

	/**
	 * [確認] マウス有効確認.
	 *	@param index マウスインデックス
	 *	@retval true 有効.
	 *	@retval false 無効.
	 */
	bool IsValid(INDEX index) const
	{
		EXCLUSIVE( &m_in.m_sync );
		return (m_RefCursorManager(index) != NULL);
	}

	/**
	 * [設定] カーソルイメージ設定.
	 *	@param index マウスインデックス
	 *	@param hotspotX ホットスポット X
	 *	@param hotspotY ホットスポット Y
	 *	@param bmp 画像.
	 *	@param color 透過色。省略すると、画像の左下の色を適用します。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetCursorImage(INDEX index, int hotspotX, int hotspotY, CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			POINT po = { hotspotX, hotspotY };
			return P->SetImage(bmp, color, po);
		}
		return false;
	}

	/**
	 * [設定] カーソルイメージ設定.
	 *	@param index マウスインデックス
	 *	@param bmp 画像.
	 *	@param color 透過色。省略すると、画像の左下の色を適用します。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetCursorImage(INDEX index, CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		return SetCursorImage(index, 0, 0, bmp, color);
	}

	/**
	 * [設定] カーソルイメージ設定.
	 *	@note アニメーションカーソルには対応していません。
	 *	@param index マウスインデックス
	 *	@param hCursor カーソル.
	 *	@param color 処理用の背景色。カーソルに含まれない色を指定してください。
	 *	@retval true 成功.
	 *	@retval false 失敗.
	 */
	bool SetCursorImage(INDEX index, HCURSOR hCursor, COLORREF color = RGB(255, 0, 255))
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			CBitmapHandle bmp;
			POINT hotspot;
			if ( m_IconToBitmap(bmp, hotspot, hCursor, color) )
			{
				return P->SetImage(bmp, color, hotspot);
			}
		}
		return false;
	}

	/**
	 * [表示] カーソル表示.
	 *	@param index マウスインデックス
	 *	@param isShow true なら表示。 false なら非表示
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
 	bool ShowCursor(INDEX index, bool isShow)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			P->Show(isShow);
			return true;
		}
		return false;
	}

	/**
	 * [設定] カーソル稼動範囲設定.
	 *	@param index マウスインデックス
	 *	@param rect 稼動範囲
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool ClipCursor(INDEX index, const RECT& rect)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			P->Clip(rect);
			return true;
		}
		return false;
	}
	
	/**
	 * [設定] カーソル位置設定.
	 *	@param index マウスインデックス。
	 *	@param x X 座標
	 *	@param y Y 座標
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
 	bool SetCursorPos(INDEX index, int x, int y)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			return P->SetPos(x, y);
		}
		return false;
	}

	/**
	 * [取得] カーソル位置取得.
	 *	@note マウスインデックスが不正な場合も 0, 0 が返ります。
	 *	@param index マウスインデックス
	 *	@return 位置
	 */
	POINT GetCursorPos(INDEX index) const
	{
		EXCLUSIVE( &m_in.m_sync );
		const CCursorManager* P = m_RefCursorManager(index);
		if ( P != NULL )
		{
			return P->GetPos();
		}
		POINT po = { 0, 0 };
		return po;
	}

	/**
	 * [取得] ボタン状態取得.
	 *	@note マウスインデックスが不正な場合も 0 が返ります。
	 *	@param index マウスインデックス
	 *	@return ボタン状態
	 */
	BYTE GetButtonState(INDEX index) const
	{
		EXCLUSIVE( &m_in.m_sync );
		const CCursorManager* P = m_RefCursorManager(index);
		if ( P != NULL )
		{
			return P->GetButtonState();
		}
		return 0;
	}

private:

	/**
	 * カーソル型ウィンドウ管理.
	 *		カーソルの形をしたウィンドウを管理します。
	 */
	class CCursorWindow : public CDummyWindow::IListener
	{
	public:
		/// コンストラクタ
		CCursorWindow(void) : m_rgn(NULL), m_isChild(false)
		{
		}
		/// デストラクタ
		~CCursorWindow(void)
		{
			m_DeleteRgn();
		}
		/// イメージを持っているか？
		bool HasCursorImage(void) const
		{
			return ! m_image.IsEmpty();
		}
		/**
		 * 作成.
		 *	@param hParent 親ウィンドウハンドル。省略すると、グローバルで動作します。
		 */
		bool Create(HWND hParent)
		{
			if ( HasCursorImage() )
			{
				m_isChild = false;
				DWORD style = WS_POPUP;
				HWND hWnd = HWND_TOPMOST;
				if ( hParent != NULL )
				{
					style = WS_CHILD;
					hWnd = HWND_TOP;
					m_isChild = true;
				}
				HWND hFocus = ::GetFocus();
				CStr className = "TNBLIB:MultiMouseMan_";
				if ( m_win.Create(this, NULL, className, style, NULL, hParent) )
				{
					m_win.ModifyStyleEx(0, WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
					if ( ! m_isChild )
					{
						m_win.ModifyStyleEx(0, WS_EX_LAYERED);
						::SetLayeredWindowAttributes(m_win, RGB(0, 0, 0), 0, 0);
					}
					::SetFocus(hFocus);
					const SIZE& sz = m_image.GetSize();
					::SetWindowPos(m_win, hWnd, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
					HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
					::CombineRgn(rgn, m_rgn, NULL, RGN_COPY);
					::SetWindowRgn(m_win, rgn, TRUE);
					Move(0, 0);
					return true;
				}
				::SetFocus(hFocus);
			}
			return false;
		}
		/// 移動
		void Move(int x, int y)
		{
			if ( ::IsWindow(m_win) )
			{
				HWND hWnd = HWND_TOPMOST;
				UINT flags = SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS;
				if ( m_isChild )
				{
					hWnd = NULL;
					flags |= SWP_NOZORDER;
				}
				::SetWindowPos(m_win, hWnd, x - m_hotspot.x, y - m_hotspot.y, 0, 0, flags);
			}
		}
		/// イメージ設定
		bool SetImage(CBitmapHandle bmp, COLORREF color, const POINT& hotspot)
		{
			if ( m_image.Set(bmp) )
			{
				m_DeleteRgn();
				m_rgn = m_image.CreateRgn(color);
				m_hotspot = hotspot;
				if ( ::IsWindow(m_win) )
				{
					const SIZE& sz = m_image.GetSize();
					::SetWindowPos(m_win, NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
					HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
					::CombineRgn(rgn, m_rgn, NULL, RGN_COPY);
					::SetWindowRgn(m_win, rgn, TRUE);
					::InvalidateRgn(m_win, m_rgn, TRUE);
				}
				return true;
			}
			return false;
		}

		void Show(bool isShow)
		{
			::ShowWindow(m_win, isShow ? SW_SHOW : SW_HIDE);
		}

	protected:
		virtual bool OnWindowMessage(LRESULT& _result, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if ( message == WM_SETFOCUS )
			{
				::SetFocus(reinterpret_cast<HWND>(wParam));
				return true;
			}
			else if ( message == WM_PAINT )
			{
				PAINTSTRUCT ps;
				HDC dc = ::BeginPaint(hWnd, &ps);
				::SelectClipRgn(dc, m_rgn);
				m_image.Draw(dc, 0, 0);
				::EndPaint(hWnd, &ps);
				return true;
			}
			else if ( message == WM_DESTROY )
			{
				m_DeleteRgn();
			}
			return false;
		}
	private:
		void m_DeleteRgn(void)
		{
			if ( m_rgn != NULL )
			{
				_DeleteObject(m_rgn);
				m_rgn = NULL;
			}
		}
		CBitmapImage	m_image;	///< イメージ
		HRGN			m_rgn;		///< リージョン
		POINT			m_hotspot;	///< ホットスポット
		CDummyWindow	m_win;		///< ウィンドウ管理
		bool			m_isChild;	///< true なら子ウィンドウ
	};

	/**
	 * カーソル管理.
	 *		一つのカーソルを管理します。
	 */
	class CCursorManager
	{
	public:
		/// コンストラクタ
		CCursorManager(void) : m_button(0), m_hRgn(NULL)
		{
			Zero(m_pos);
		}
		/// 作成
		bool Create(HWND hParent, CBitmapHandle bmp, COLORREF color, const POINT& hotspot)
		{
			m_button = 0;
			Zero(m_pos);
			if ( m_win.SetImage(bmp, color, hotspot) )
			{
				RECT rc = { 0 };
				if ( hParent != NULL )
				{
					// 親ウィンドウ指定有り
					::GetClientRect(hParent, &rc);
					Clip(rc);
				}
				else
				{
					// デスクトップ
					Clip(rc);
					::EnumDisplayMonitors(NULL, NULL, ms_MonitorEnumProc, reinterpret_cast<LPARAM>(this));
				}
				return m_win.Create(hParent);
			}
			return false;
		}
		/// 作成
		bool SetImage(CBitmapHandle bmp, COLORREF color, const POINT& hotspot)
		{
			return m_win.SetImage(bmp, color, hotspot);
		}
		/// 表示
		void Show(bool isShow)
		{
			m_win.Show(isShow);
		}
		/// 範囲
		void Clip(const RECT& rect)
		{
			if ( m_hRgn != NULL )
			{
				_DeleteObject(m_hRgn);
			}
			m_hRgn = ::CreateRectRgnIndirect(&rect);
		}
		/// ポジション設定
		bool SetPos(int x, int y)
		{
			if ( m_hRgn != NULL && ! ::PtInRegion(m_hRgn, x, y) )
			{
				// 入っていない
				bool r = false;
				if ( ::PtInRegion(m_hRgn, m_pos.x, y) )
				{
					// X移動しなければ入る
					int f = (m_pos.x > x) ? 1 : -1;
					loop ( i, abs(m_pos.x - x) )
					{
						x += f;
						if ( ::PtInRegion(m_hRgn, x, y) )
						{
							r = true;
							break;
						}
					}
				}
				else if ( ::PtInRegion(m_hRgn, x, m_pos.y) )
				{
					// Y移動しなければ入る
					int f = (m_pos.y > y) ? 1 : -1;
					loop ( i, abs(m_pos.y - y) )
					{
						y += f;
						if ( ::PtInRegion(m_hRgn, x, y) )
						{
							r = true;
							break;
						}
					}
				}
				else
				{
					// Y移動に移動
					int f = (m_pos.y > y) ? 1 : -1;
					loop ( i, abs(m_pos.y - y) )
					{
						y += f;
						if ( ::PtInRegion(m_hRgn, m_pos.x, y) )
						{
							break;
						}
					}
					// X移動に移動
					f = (m_pos.x > x) ? 1 : -1;
					loop ( i, abs(m_pos.x - x) )
					{
						x += f;
						if ( ::PtInRegion(m_hRgn, x, y) )
						{
							r = true;
							break;
						}
					}
				}
				if ( ! r )
				{
					return false;
				}
			}
			if ( m_pos.x != x || m_pos.y != y )
			{
				m_pos.x = x;
				m_pos.y = y;
				m_win.Move(m_pos.x, m_pos.y);
				return true;
			}
			return false;
		}
		/// ポジション移動
		bool MovePos(int dx, int dy)
		{
			return SetPos(m_pos.x + dx, m_pos.y + dy);
		}
		/// ポジション取得
		const POINT& GetPos(void) const
		{
			return m_pos;
		}
		/// ボタン状態取得
		BYTE GetButtonState(void) const
		{
			return m_button;
		}
		/// ボタン状態設定
		void SetButtonFlags(WORD buttonFlags)
		{
			loop ( i, 3/*ボタン数*/ * 2 )
			{
				if ( (buttonFlags & _BIT(i)) != 0 )
				{
					if ( (i & 1) == 0 )
					{
						m_button |= _BIT(i / 2);
					}
					else
					{
						m_button &= ~_BIT(i / 2);
					}
				}
			}
		}

	private:
		/// EnumDisplayMonitors() 用コールバック
		static BOOL CALLBACK ms_MonitorEnumProc(
			HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
		{
			CCursorManager* P = reinterpret_cast<CCursorManager*>(dwData);
			HRGN& r1 = P->m_hRgn;
			HRGN r2 = ::CreateRectRgnIndirect(lprcMonitor);
			VERIFY( ::CombineRgn(r1, r1, r2, RGN_OR) != ERROR );
			_DeleteObject(r2);
			return TRUE;
		}
		CCursorWindow	m_win;			///< カーソル
		POINT			m_pos;			///< 座標
		HRGN			m_hRgn;			///< 動作範囲
		BYTE			m_button;		///< _BIT(IListener::EKind) 。
	};

	/// 内部用クラス
	class CInner : public IMultiMouseWatcher::IListener
	{
		DEFPARENTLISTENER(CMultiMouseManager, IParentListener);
	public:
		/// コンストラクタ
		CInner(void) : m_pListener(NULL)
		{
			CBitmapImage bi;
			bi.Set(1, 1);
			m_cursorImage = bi;
		}
		/// チェック
		bool CheckIndex(INDEX index)
		{
			if ( ! m_cursores.IsInRange(index) )
			{
				m_cursores.SetSize(index + 1);
			}
			if ( m_cursores[index] == NULL )
			{
				CCursorManager* P = new CCursorManager;
				if ( ! P->Create(m_hParent, m_cursorImage, m_cursorMaskColor, m_cursorHotspot) )
				{
					_GetLastError("CreateWindow");
					delete P;
					return false;
				}
				if ( ! ::IsRectEmpty(&m_corsorRect) )
				{
					P->Clip(m_corsorRect);
				}
				m_cursores.Set(index, P);
			}
			return true;
		}
		/// [通知] 受信メッセージ通知.
		virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse)
		{
			EXCLUSIVE( &m_sync );
			if ( m_pListener != NULL )
			{
				m_pListener->OnMultiMouseEvent(index, mouse);
			}
			CCursorManager* P = m_cursores[index];
			if ( P == NULL )
			{
				ASSERT( P != NULL );
				return;
			}
			const POINT& po = P->GetPos();
			P->SetButtonFlags(mouse.usButtonFlags);
			if ( P->MovePos(mouse.lLastX, mouse.lLastY) )
			{
				//変化有り
				if ( m_pListener != NULL )
				{
					m_pListener->OnMultiMouseMove(index, P->GetButtonState(), po.x, po.y);
				}
			}
			if ( m_pListener != NULL )
			{
				if ( (mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0 )
				{
					short d = mouse.usButtonData;
					m_pListener->OnMultiMouseWheel(index, d);
				}
				loop ( i, 3/*ボタン数*/ * 2 )
				{
					if ( (mouse.usButtonFlags & _BIT(i)) != 0 )
					{
						CMultiMouseManager::IListener::EEvent e = CMultiMouseManager::IListener::Event_Down;
						if ( (i & 1) != 0 )
						{
							e = CMultiMouseManager::IListener::Event_Up;
						}
						m_pListener->OnMultiMouseButton(index, CMultiMouseManager::IListener::EButton(i / 2), e, po.x, po.y);
					}
				}
			}
		}
		/// [通知]
		virtual void OnMultiMouseDevice(INDEX index, EDevice device)
		{
			EXCLUSIVE( &m_sync );
			if ( device == Device_Remove )
			{
				if ( m_pListener != NULL )
				{
					m_pListener->OnMultiMouseDevice(index, device);
				}
				return;
			}
			if ( ! CheckIndex(index) )
			{
				return;
			}
			if ( m_pListener != NULL )
			{
				m_pListener->OnMultiMouseDevice(index, device);
			}
		}
		IParentListener*				m_pListener;		///< リスナー
		CPointerVectorT<CCursorManager>	m_cursores;			///< マウスカーソル管理
		HWND							m_hParent;			///< 親ウィンドウハンドル
		CBitmapHandle					m_cursorImage;		///< カーソルイメージ
		COLORREF						m_cursorMaskColor;	///< カーソルイメージマスク色
		POINT							m_cursorHotspot;	///< カーソルホットスポット
		RECT							m_corsorRect;		///< カーソル移動範囲
		CSyncSection					m_sync;
	};	

	/// HCURSOR を BMP に変換
	bool m_IconToBitmap(CBitmapHandle& _bmp, POINT& _hotspot, HCURSOR hCursor, COLORREF color)
	{
		ICONINFO ii;
		if ( ::GetIconInfo(hCursor, &ii) )
		{
			HBITMAP h = ii.hbmColor;
			if ( h == NULL )
			{
				h = ii.hbmMask;
			}
			BITMAP bm;
			if ( ::GetObject(h, sizeof(BITMAP), &bm) > 0 )
			{
				CBitmapImage bi;
				bi.Set(bm.bmWidth, bm.bmHeight, color);
				HDC dc = bi.GetDC();
				::DrawIconEx(dc, 0, 0, hCursor, 0, 0, 0, NULL, DI_NORMAL);
				bi.ReleaseDC();
				_bmp = bi;
				_hotspot.x = ii.xHotspot;
				_hotspot.y = ii.yHotspot;
				return true;
			}
		}
		return false;
	}

	/// 指定のマウスインデックスのマネージャ取得
	const CCursorManager* m_RefCursorManager(INDEX index) const
	{
		if ( m_in.m_cursores.IsInRange(index) )
		{
			return m_in.m_cursores[index];
		}
		return NULL;
	}

	/// 指定のマウスインデックスのマネージャ取得
	CCursorManager* m_GetCursorManager(INDEX index)
	{
		m_in.CheckIndex(index);
		return m_in.m_cursores[index];
	}

	CInner				m_in;		///< 内部管理クラス
	IMultiMouseWatcher*	m_pWatcher;	///< マウス監視
};



}; //TNB



