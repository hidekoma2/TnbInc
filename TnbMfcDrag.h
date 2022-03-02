#pragma once
/**
 *@file
 * ドラッグ&ドロップ処理関係のヘッダ
 *
 *		コントロールをドラッグ＆ドロップするための機能を提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapImage.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ドラッグコントロールアドインクラス
 *
 *		本クラスを使用すると、ドラッグすることが可能になります。
 *
 *		ドラック開始時、ドラッグ中止時、ドロップ時にメッセージを親に通知します。
 *		通知メッセージは以下のようになります。 \n
 *			message = MSG (テンプレートの引数リスト、参照)	\n
 *			WPARAMの上位WORD = グループID（ SetDragGroupID() 参照）。\n
 *			WPARAMの下位WORD = 通知種別（ CDragAddinT::EDragNotifyCode ） 。	\n
 *			LPARAMの上位WORD = 相手コントロールID （コマンド種別がドラック開始やキャンセルの時は０固定）。	\n
 *			LPARAMの下位WORD = ドラックコントロールID 。	\n
 *		※通知種別 ISDROP は、普通のコントロールだけど、ドロップ対象にしたい場合に使用します。
 *
 *	@note 上位WORDは HIWORD() マクロを使うと便利です。
 *	@note 下位WORDは LOWORD() マクロを使うと便利です。
 *
 *	\par ポイント:
 *		ドロップ対象コントロールについて。\n
 *		ドロップ可能のコントロールは、メッセージで応答したものが対象になります。\n
 *		まず、本クラスをアドインしたコントロールをドラッグすると、マウス下にあるコントロールに
 *		GetDragNotifyMessage() で得られるメッセージを SendMessage します。その際、 WPARAM の下位WORDには
 *		CANDROP 、上位WORDには グループID、LPARAM は TDropParam のポインタが入っています。
 *		コントロールが 1 で応答すると、ドロップ可能なコントロールと判断します（ TDropParam には、
 *		コントロールの領域やIDが入っており、変更も可能）。\n
 *		また、コントロールが 0（拒否・未サポート）を返した場合、親ダイアログに、 通知種別 = ISDROP で
 *		通知メッセージを送ります。これにより標準のコントロールもドロップ対象にすることが可能です。\n
 *		なお、 本クラスをアドインしたコントロールは、上記メッセージに対応しており、 SetDropMode()
 *		で、ドロップ許可、拒否の設定、 SetDragMode() でドラッグ許可、拒否の設定が可能です。\n
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		//= 宣言
 *		typedef CDragAddinT<CStatic> CDragStatic;
 *				;
 *		class CXXXX : public CDialog
 *		{
 *				;
 *		 	CDragStatic	m_dragText;		// <- 元々 CStaticだったメンバを書き換えた
 *				;
 *			afx_msg LRESULT OnDroped(WPARAM wParam, LPARAM lParam);
 *				;
 *
 *		//= 設定(グローバル)
 *		static UINT s_uDropMessage = CDragStatic::GetNotifyMessage();
 *		BEGIN_MESSAGE_MAP(CXXXX, CDialog)
 *			//{{AFX_MSG_MAP(CMfcTestAppDlg)
 *				;
 *			//}}AFX_MSG_MAP
 *			ON_REGISTERED_MESSAGE(s_uDropMessage, OnDroped)		//←追加
 *		END_MESSAGE_MAP()
 *
 *		//= 処理（OnDroped() 内)
 *		LRESULT CXXXX::OnDroped(WPARAM wParam, LPARAM lParam)
 *		{
 *			switch( LOWORD(wParam) )
 *			{
 *			case CDragStatic::START:		// ドラック開始
 *				TRACE1("ID = %d をドラッグ開始", LOWORD(lParam));
 *				break;
 *			case CDragStatic::CANCEL:		// キャンセル（ドラック終了）
 *				TRACE1("ID = %d のドラックを中止", LOWORD(lParam));
 *				break;
 *			case CDragStatic::INVALID:		// 無効（ドラック終了）
 *				TRACE1("ID = %d は許可できないところへドロップ", LOWORD(lParam));
 *				break;
 *			case CDragStatic::DROP:			// ドロップ（ドラック終了）
 *				TRACE2("ID = %d から %d へドラッグ", LOWORD(lParam), HIWORD(lParam));
 *				break;
 *			case CDragStatic::HOVER:		// ホバー（ドラック中）
 *				TRACE2("ID = %dが%dの上", LOWORD(lParam), HIWORD(lParam));
 *				break;
 *			case CDragStatic::LEAVE:		// ホバー終了（ドラック中）
 *				TRACE2("ID = %dが%dの上からはなれた", LOWORD(lParam), HIWORD(lParam));
 *				break;
 *			case CDragStatic::ISDROP:		// ドロップ確認
 *				TRACE2("ID = %dが%dの上へ。IDOK ならドロップ可能に", LOWORD(lParam), HIWORD(lParam));
 *				return HIWORD(lParam) == IDOK; 
 *			}
 *			return 0;
 *		}
 *		</pre></td></tr></table>
 *
 *	@param TYP 要素の型。CWnd のサブクラスである必須があります。
 *
 *	@par必要ファイル
 *			TnbMfcDrag.h
 * 
 *	@date 07/10/11 新規作成
 *	@date 07/10/16 専用メッセージに変更。
 *	@date 07/10/26 INVALID 通知を追加。
 *	@date 07/10/31 画像取得方法を変更。
 *	@date 07/11/14 リスナー、装備。 ドロップ可能のアイテムにマークをつけられるようにした。
 *	@date 07/12/06 ドロップ対象の登録方法を変更。
 *	@date 07/12/25 ドロップマーク指定方法、追加。 Listener 廃止。
 *	@date 08/03/04 Drag中の 破棄時に、Drag状態を解除するように修正。
 *	@date 09/04/15 Leave時、コントロール再描画処理を最適化。
 */
template<typename TYP = CStatic>
class CDragAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/**
	 * ドラッグ通知種別コード.
	 *		通知メッセージのWPARAM の下位WORDに格納されています。
	 *	@note 親ダイアログに通知されます。
	 */
	enum EDragNotifyCode
	{
		START = 0,		///< ドラック開始.
		HOVER,			///< ホバー（ドラック中）.
		LEAVE,			///< ホバー終了（ドラック中）.
		CANCEL,			///< キャンセル（ドラック終了）.
		INVALID,		///< 無効ドロップ（ドラック終了）.
		DROP,			///< 有効ドロップ（ドラック終了）.
		ISDROP,			///< ドロップ確認.
	};

	/**
	 * ドロップコントロールコード.
	 *		通知メッセージの WPARAM の下位WORDに格納されています。
	 *	@note 自分自身に通知されます。
	 */
	enum EDropCtrlCode
	{
		CANDROP = 100,	/**<
						 *	ドロップ可能かチェック時に通知します。
						 *	通知を受けたコントロールは LRESULT で true(ドロップ可能),false(不可能) を返します。
						 *	LPARAM は TDropParam のポインタが格納されます（必要に応じて変更します）。
						 */
	};
	
	/// ドロップコントロールパラメータ
	struct TDropParam
	{
		int		dragID;	///< ドラッグ中のコントロールID。変更不可。
		POINT	point;	///< マウスカーソル位置（クライアント座標）。変更不可。
		HRGN	hRgn;	///< クライアントの領域（クライアント座標）。変更可能。
		WORD	ctrlID;	///< コントロールID。変更可能。
	};
	
	/**
	 * [取得] 通知用メッセージコード取得.
	 *	@return 通知用メッセージコード.
	 */
	static UINT GetDragNotifyMessage(void)
	{
		return ::RegisterWindowMessage(_T("TnbMfcDragAddin"));
	}

	/// コンストラクタ
	CDragAddinT(void) 
		: m_targetMarkColor(::GetSysColor(COLOR_HIGHLIGHT)), m_targetMarkThick(2)
		, m_arrowIcon(NULL), m_noIcon(NULL)
		, m_wGroupID(0), m_boCanDragging(true), m_boCanDropping(true)
		, m_boIsDragging(false), m_nowDropItem(-1), m_nowDropItemRgn(NULL)
	{
		m_nMessageCode = GetDragNotifyMessage();
		m_nowDropItemRgn = ::CreateRectRgn(0, 0, 0, 0);
	}

	/// デストラクタ
	~CDragAddinT(void)
	{
		_DeleteObject(m_nowDropItemRgn);
	}

	/**
	 * [設定] ドロップグループID設定
	 *	@note この値は通知メッセージの WPARAM の上位WORDに格納されます。
	 *			グループ値を設定することで、同時に複数のドラックアンドドロップグループを配置できます。
	 *	@param w グループ値。
	 */
	void SetDragGroupID(WORD w)
	{
		m_wGroupID = w;
	}

	/**
	 * [取得] ドロップグループID取得
	 *	@return グループ値。
	 */
	WORD GetDragGroupID(void) const
	{
		return m_wGroupID;
	}

	/**
	 * [設定] ドラック中ビットマップ設定.
	 *		ドラック中の絵を指定できます。指定しない場合、コントロールの大きさでキャプチャした物を使います。
	 *	@param bmp ビットマップ。
	 */
	void SetDraggingBitmap(CBitmapHandle bmp)
	{
		m_bmpDragging = bmp;
	}

	/**
	 * [設定] ドロップ先のマーク設定.
	 *		ドロップ可能のアイテムにマークを表示することが出来ます。
	 *	@note 設定しないと、選択色の太さ２になっています。
	 *	@param other コピー元。
	 */
	template<typename T>
	void SetDropTargetMark(const CDragAddinT<T>& other)
	{
		const IDrawable*	draw = NULL;
		COLORREF			color;
		int					thick;
		other.GetDropTargetMark(draw, color, thick);
		if ( draw != NULL )
		{
			m_targetMarkDrawer = draw->Clone();
		}
		m_targetMarkColor = color;
		m_targetMarkThick = thick;
	}

	/**
	 * [設定] ドロップ先のマーク設定.
	 *		ドロップ可能のアイテムにマークを表示することが出来ます。
	 *	@note 設定しないと、選択色の太さ２になっています。
	 *	@param draw 描画情報（Resize() を使用します）。複製を保持しますので、 draw は破棄してかまいません。
	 */
	void SetDropTargetMark(const IDrawable& draw)
	{
		m_targetMarkDrawer = draw.Clone();
	}

	/**
	 * [設定] ドロップ先のマーク設定.
	 *		ドロップ可能のアイテムにマークを表示することが出来ます。
	 *	@note 設定しないと、選択色の太さ２になっています。
	 *	@param color カラー。
	 *	@param thick 太さ。０なら設定変更なし。
	 */
	void SetDropTargetMark(COLORREF color, int thick = 0)
	{
		m_targetMarkDrawer.Null();
		m_targetMarkColor = color;
		if ( thick > 0 )
		{
			m_targetMarkThick = thick;
		}
	}

	/**
	 * [取得] ドロップ先のマーク設定取得.
	 *	@param[out] _draw 描画情報。
	 *	@param[out] _color カラー。
	 *	@param[out] _thick 太さ。
	 */
	void GetDropTargetMark(const IDrawable*& _draw, COLORREF& _color, int& _thick) const
	{
		_draw = m_targetMarkDrawer;
		_color = m_targetMarkColor;
		_thick = m_targetMarkThick;
	}

	/**
	 * [設定] ドラッグモード設定.
	 *	@param can true ならドラッグ可能になります。 false ならドラッグ出来なくなります。
	 */
	void SetDragMode(bool can)
	{
		m_boCanDragging = can;
	}

	/**
	 * [取得] ドラッグモード取得.
	 *	@retval true ドラッグ可能.
	 *	@retval false ドラッグ不可能.
	 */
	bool CanDragging(void) const
	{
		return m_boCanDragging;
	}

	/**
	 * [設定] ドロップモード設定.
	 *	@param can true ならドロップを許可します。 false なら許可しません。
	 */
	void SetDropMode(bool can)
	{
		m_boCanDropping = can;
	}

	/**
	 * [取得] ドロップモード取得.
	 *	@retval true ドロップ可能.
	 *	@retval false ドロップ不可能.
	 */
	bool CanDropping(void) const
	{
		return m_boCanDropping;
	}

protected:

	/**
	 * [通知] メッセージ通知.
	 *	@param pWnd 通知先。
	 *	@param wParam WPARAM。
	 *	@param lParam LPARAM。
	 *	@return 応答。
	 */
	bool SendNofityMessage(CWnd* pWnd, WPARAM wParam, LPARAM lParam) const
	{
		return !! ::SendMessage(*pWnd, m_nMessageCode, wParam, lParam);
	}

	/**
	 * [確認] 通知メッセージ確認.
	 *	@param message 比較メッセージ。
	 *	@retval true 通知メッセージ。
	 *	@retval false 否。
	 */
	bool IsNofityMessage(UINT message) const
	{
		return m_nMessageCode == message;
	}

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == m_nMessageCode )
		{
			if ( wParam == static_cast<WPARAM>(MAKELONG(CANDROP, m_wGroupID)) )
			{
				return m_boCanDropping;
			}
		}
		else if ( message == WM_LBUTTONDOWN )
		{
			if ( m_BeginDrag() )
			{
				m_SendCommand(START, 0);
				return 0;
			}
		}
		else if ( message == WM_LBUTTONUP )
		{
			if ( m_EndDrag() )
			{
				if ( m_nowDropItem >= 0 )
				{
					m_SendCommand(DROP, m_nowDropItem);
				}
				else
				{
					m_SendCommand(INVALID, 0);
				}
				return 0;
			}
		}
		else if ( message == WM_CAPTURECHANGED || message == WM_RBUTTONDOWN )
		{
			if ( m_EndDrag() )
			{
				m_SendCommand(CANCEL, 0);
			}
		}
		else if ( message == WM_MOUSEMOVE )
		{
			if ( m_boIsDragging )
			{
				m_OnMouseMove();
			}
		}
		else if ( message == WM_DESTROY )
		{
			m_EndDrag();
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [通知] ドラッグ開始.
	 *	@param[in,out] _rc アイテム範囲。inで ::GetWindowRect() した範囲が入っています（Window座標）。
	 *	@param[in] po マウスのポジション（Window座標）。
	 *	@retval 0 ドラック出来ない.
	 *	@retval 1以上 コントロールID
	 */
	virtual int OnBeginDrag(RECT& _rc, const POINT& po)
	{
		if ( ! ::DragDetect(m_hWnd, po) )	//これ以外で対応したい
		{
			return 0;
		}
		return _super::GetDlgCtrlID();
	}	

	/**
	 * [通知] ドラッグ通知.
	 *	@note 親に通知するものと同じものが通知されます。
	 *	@param cmd コマンド.
	 *	@param draggingItem ドラッグ元アイテムID
	 *	@param dropedItem ドロップ先アイテムID
	 */
	virtual void OnDragNofity(UINT cmd, int draggingItem, int dropedItem)
	{
	}

private:
	//-設定
	IDrawable::Ptr		m_targetMarkDrawer;	///< ドロップ対象アイテムマーク描画
	COLORREF			m_targetMarkColor;	///< ドロップ対象アイテムマーク色
	int					m_targetMarkThick;	///< ドロップ対象アイテムマーク太さ
	CBitmapHandle		m_bmpDragging;		///< ドラック中の絵
	WORD				m_wGroupID;			///< グループID
	bool				m_boCanDragging;	///< ドラッグすることが可能？
	bool				m_boCanDropping;	///< ドロップすることが可能？
	//-動作 Work
	UINT				m_nMessageCode;		///< 専用メッセージ
	int					m_draggingItemID;	///< ドラッグ中のアイテムID
	HICON				m_arrowIcon;		///< 矢印カーソル
	HICON				m_noIcon;			///< 禁止カーソル
	CImageList			m_imglist;			///< イメージリスト（ドラック中の表示用）
	bool				m_boIsDragging;		///< true ならドラック中
	CPoint				m_dragOffset;		///< ドラック開始時のマウスカーソルとアイテムのずれ
	//-MOVE中
	int					m_nowDropItem;		///< 下にあるアイテム
	HRGN				m_nowDropItemRgn;	///< 下にあるアイテムの形
	CWnd*				m_nowDropWnd;		///< 下にあるアイテム
	/// ドロップ可能マーク描画
	void m_DrawTargetMark(CDC* pDC, HRGN hRgn)
	{
		if ( m_targetMarkDrawer.IsNull() )
		{
			CBrush br;
			br.CreateSolidBrush(m_targetMarkColor);
			::FrameRgn(*pDC, m_nowDropItemRgn, br, m_targetMarkThick, m_targetMarkThick);
		}
		else
		{
			CRect rc;
			::GetRgnBox(hRgn, &rc);
			::SelectClipRgn(*pDC, hRgn);
			m_targetMarkDrawer->Resize(rc.Size());
			m_targetMarkDrawer->Draw(*pDC, rc.left, rc.top);
			::SelectClipRgn(*pDC, NULL);
		}
	}
	/// マウスムーブ
	void m_OnMouseMove(void)
	{
		if ( m_arrowIcon == NULL ) { m_arrowIcon = ::GetCursor(); }
		CPoint pos;
		::GetCursorPos(&pos);
		m_imglist.DragMove(pos - m_dragOffset);
		//
		CWnd* pParent = _super::GetParent();
		if ( pParent == NULL )
		{
			return;
		}
		int backupDropItem = m_nowDropItem;
		CWnd* pBackupDropWnd = m_nowDropWnd;
		RECT backupDropRect;
		::GetRgnBox(m_nowDropItemRgn, &backupDropRect);
		m_nowDropItem = -1;
		HCURSOR hCursor = NULL;
		//
		CWnd* P = pParent->GetParentOwner();
		if ( P == NULL )
		{
			return;
		}
		CRect rc;
		P->GetClientRect(&rc);
		P->ClientToScreen(&rc);
		if ( rc.PtInRect(pos) )
		{
			pParent->ScreenToClient(&pos);
			CWnd* P = pParent->ChildWindowFromPoint(pos, CWP_SKIPINVISIBLE);
			if ( P != NULL )
			{
				TDropParam param;
				param.dragID = m_draggingItemID;
				param.ctrlID = static_cast<WORD>(P->GetDlgCtrlID());
				if ( param.ctrlID != 0 )
				{
					pParent->ClientToScreen(&pos);
					P->ScreenToClient(&pos);
					param.point = pos;
					RECT rect;
					P->GetClientRect(&rect);
					param.hRgn = ::CreateRectRgn(0, 0, rect.right, rect.bottom);
					P->GetWindowRgn(param.hRgn);
					if ( m_CanDropping(P, param) )
					{
						if ( param.ctrlID != m_draggingItemID )
						{
							_DeleteObject(m_nowDropItemRgn);
							m_nowDropItemRgn = param.hRgn;
							m_nowDropItem = param.ctrlID;
							m_nowDropWnd = P;
							hCursor = m_arrowIcon;
						}
						else
						{
							_DeleteObject(param.hRgn);
						}
					}
					else
					{
						_DeleteObject(param.hRgn);
					}
				}
			}
		}
		if ( hCursor == NULL )
		{
			if ( m_noIcon == NULL ) { m_noIcon = ::LoadCursor(NULL, IDC_NO); }
			hCursor = m_noIcon;
		}
		::SetCursor(hCursor);
		if ( backupDropItem != m_nowDropItem )
		{
			if ( backupDropItem >= 0 )
			{
				m_SendCommand(LEAVE, backupDropItem);
				m_imglist.DragShowNolock(false);
				CWnd* P = pBackupDropWnd->GetParent();
				if ( P != NULL )
				{
					pBackupDropWnd->ClientToScreen(&backupDropRect);
					P->ScreenToClient(&backupDropRect);
					P->RedrawWindow(&backupDropRect);
				}
				else
				{
					pBackupDropWnd->RedrawWindow(&backupDropRect);
				}
				m_imglist.DragShowNolock(true);
			}
			if ( m_nowDropItem >= 0 )
			{
				m_SendCommand(HOVER, m_nowDropItem);
				if ( IS_RGBVALUE(m_targetMarkColor) )
				{
					CDC* pDC = m_nowDropWnd->GetDC();
					m_imglist.DragShowNolock(false);
					m_DrawTargetMark(pDC, m_nowDropItemRgn);
					m_imglist.DragShowNolock(true);
					m_nowDropWnd->ReleaseDC(pDC);
				}
			}
		}
	}
	/// 可能なら true
	bool m_CanDropping(CWnd* P, TDropParam& _param)
	{
		ASSERTLIB( P != NULL );
		ASSERTLIB( _param.ctrlID != 0 );
		WPARAM wParam = MAKELONG(CANDROP, m_wGroupID);
		LPARAM lParam = reinterpret_cast<LPARAM>(&_param);
		if ( P->SendMessage(m_nMessageCode, wParam, lParam) )
		{
			return true;
		}
		if ( m_SendCommand(ISDROP, _param.ctrlID) )
		{
			return true;
		}
		return false;
	}
	//　アイテムのドラッグ開始
	bool m_BeginDrag(void)
	{
		if ( m_boIsDragging )
		{
			m_EndDrag();
		}
		if ( ! m_boCanDragging ) { return false; }
		CPoint pos;
		::GetCursorPos(&pos);
		CRect rect;
		_super::GetWindowRect(rect);
		CRect winRect = rect;
		m_draggingItemID = OnBeginDrag(rect, pos);
		if ( m_draggingItemID <= 0 )
		{
			return false;
		}
		CPoint controlPoint = rect.TopLeft();
		m_dragOffset = pos - controlPoint;
		CBitmapHandle hh = m_bmpDragging;
		CSize size;
		if ( hh.IsNull() )
		{
			CBitmapImage bi;
			bi.SetFromWindow(m_hWnd, RGB(0, 0, 1));
			rect.OffsetRect(-winRect.TopLeft());
			hh = bi.Cut(rect).GetBitmapHandle();
			hh.GetSize(size);
		}
		else
		{
			VERIFY( hh.GetSize(size) );
		}
		m_imglist.Create(size.cx, size.cy, ILC_MASK, 0, 1);
		m_imglist.Add(CBitmap::FromHandle(hh), RGB(0, 0, 1));
		m_imglist.BeginDrag(0, CPoint(0, 0));
		m_imglist.DragEnter(NULL, controlPoint);
		m_imglist.DragMove(controlPoint);
		SetCapture();
		m_boIsDragging = true;
		return true;
	}
	// ドラッグの終了
	bool m_EndDrag(void)
	{
		if ( ! m_boIsDragging ) { return false; }
		m_boIsDragging = false;
		m_imglist.EndDrag();
		m_imglist.DeleteImageList();
		if ( m_nowDropItem >= 0 && ::IsWindow(*m_nowDropWnd) )
		{
			m_nowDropWnd->Invalidate();
		}
		ReleaseCapture();
		return true;
	}
	/// 親にコマンドを通知する。
	bool m_SendCommand(UINT uCmd, int item)
	{
		WPARAM wParam = MAKELONG(uCmd, m_wGroupID);
		LPARAM lParam = MAKELONG(m_draggingItemID, item);
		LRESULT r = 0;
		CWnd* pWnd = _super::GetParent();
		if ( pWnd != NULL )
		{
			r = pWnd->SendMessage(m_nMessageCode, wParam, lParam);
		}
		OnDragNofity(uCmd, m_draggingItemID, item);
		return !! r;
	}
};



}; //MFC
}; //TNB



