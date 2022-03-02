#pragma once
/**
 *@file
 * 描画情報メニュー関係のヘッダ
 *
 *	@todo 単独で CDrawingMenu を使うことは現在出来ません。 CCustomWindowFrame 用です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcAbstractDrawingCtrl.h"
#include "TnbMfcBitmapDC.h"
#include "TnbSimpleVector.h"
#include "TnbPointerVector.h"
#include "TnbWindowProcedureHooker.h"
#include "TnbFontHandle.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * オーナードローメニューサポートプロシージャ
 *
 *		オーナードローをサポートするメニューを使うダイアログに本インスタンスをアタッチしておく必要があります。
 *		
 *
 *	@par必要ファイル
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	新規作成
 */
class COwnerdrawMenuSupportProcedureHooker: public CWindowProcedureHooker
{
	DEFSUPER(CWindowProcedureHooker);
public:

	/// コンストラクタ
	COwnerdrawMenuSupportProcedureHooker(void) : m_cookie(0x8000)
	{
	}

	/**
	 * [登録] メニュー登録.
	 *		本インスタンスがアタッチしているウィンドウに関連づいているメニューを
	 *		登録します。戻り値のベースクッキーを使い、メニューのオーナードローのアイテムのデータを設定することで、
	 *		オーナードローメッセージを処理することが出来ます。
	 *	@note 通常 CDrawingMenu CDrawingMenuBar クラス内で、暗黙的に使用されます。
	 *	@param pMenu メニュー。作成してある必要があります。
	 *	@return ベースクッキー。
	 */
	DWORD RegisterMenu(CMenu* pMenu)
	{
		TParam t;
		t.pMenu = pMenu;
		t.hMenu = pMenu->m_hMenu;
		t.cookie = m_cookie;
		m_params.Add(t);
		m_cookie++;
		m_cookie |= 0x8000;
		return t.cookie * 0x10000;
	}

protected:

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。これをオーバーライドして処理を追加します。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */	
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_INITMENUPOPUP:
#if 0
			{
				HMENU hMenu = reinterpret_cast<HMENU>(wParam);
				loop ( i, m_params.GetSize() )
				{
					if ( m_params[i].hMenu == hMenu )
					{
						MEASUREITEMSTRUCT m = { 0 };
						m.CtlType = ODT_MENU;
						m.CtlID = 'TMDM';
						m.itemData = reinterpret_cast<DWORD>(this);
						m_params[i].pMenu->MeasureItem(&m);
					}
				}
			}
#endif
			break;
		case WM_MEASUREITEM:
			{
				MEASUREITEMSTRUCT* P = reinterpret_cast<MEASUREITEMSTRUCT*>(lParam);
				if ( P->CtlType == ODT_MENU )
				{
					const TParam* T = m_Search(static_cast<WORD>(P->itemData / 0x10000));
					if ( T != NULL )
					{
						T->pMenu->MeasureItem(P);
						return 0;
					}
				}
			}
			break;
		case WM_DRAWITEM:
			{
				DRAWITEMSTRUCT* P = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
				if ( P->CtlType == ODT_MENU )
				{
					const TParam* T = m_Search(static_cast<WORD>(P->itemData / 0x10000));
					if ( T != NULL )
					{
						ASSERT( T->hMenu == reinterpret_cast<HMENU>(P->hwndItem) );
						T->pMenu->DrawItem(P);
						return 0;
					}
				}
			}
			break;
		case WM_DESTROY:
			m_params.RemoveAll();
			m_cookie = 0x8000;
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// パラメータ
	struct TParam
	{
		HMENU		hMenu;		///< メニューハンドル
		CMenu*		pMenu;		///< メニュークラスポインタ
		WORD		cookie;		///< ベースクッキー（上位WORD分）
	};
	CSimpleVectorT<TParam>	m_params;	///< パラメータ群
	WORD					m_cookie;	///< クッキー
	/**
	 * 検索
	 *	@param w ベースクッキー(上位WORD)
	 *	@retval NULL 見つからず
	 *	@retval NULL以外 発見パラメータ
	 */
	const TParam* m_Search(WORD w)
	{
		for ( size_t lp = m_params.GetSize(); lp > 0; lp-- )
		{
			size_t i = lp - 1;
			const TParam& t = m_params.At(i);
			if ( ! ::IsMenu(t.hMenu) )
			{
				m_params.Remove(i);
			}
			else if ( t.cookie == w )
			{
				return &t;
			}
		}
		return NULL;
	}
};



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報Menuコントロール
 *
 *	@note SetSelectMarkDrawer() のアクティブ選択マークは Hover時、 非アクティブ選択マークは選択時に使用されます。
 *
 *	@todo 現在、文字と背景の色のみ変更可能です。
 *	@todo WM_MEASUREITEM にメニューハンドル情報がないため、UserData などで一元管理する必要あり。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	新規作成
 *	@date 10/04/19	サブメニュー対応
 *	@date 10/05/13	フォント改修
 *	@date 12/11/06	ステータス取得方法を改善。
 */
class CAbstractDrawingMenu : public CMenu, public CAbstractDrawingCtrl
{
	DEFSUPER(CMenu);
	typedef CAbstractDrawingCtrl	_mark;			///< SelectMark管理名再宣言
public:

	/// コンストラクタ
	CAbstractDrawingMenu(void) 
		: _super(), m_leftMargin(20), m_baseCookie(0), m_hWnd(NULL)
		, m_textColor(::GetSysColor(COLOR_MENUTEXT)), m_textDisableColor(CLR_INVALID)
		, m_isAllOwnerDraw(false), m_withSubMenu(false)
	{
		m_font.SetSystemMenuFont();
		m_fontBold.SetAsBold(m_font);
	}

	/// デストラクタ
	~CAbstractDrawingMenu(void)
	{
	}

	/**
	 * [取得] 親ウィンドウハンドル取得.
	 *	@return ウィンドウハンドル
	 */ 
	HWND GetParentHwnd(void) const
	{
		return m_hWnd;
	}

	/**
	 * [確認] 有効確認.
	 *	@note WIN32API IsMenu を使用しています。
	 *	@retval true 有効.
	 *	@retval false 無効
	 */
	bool IsValid(void) const
	{
		return !! ::IsMenu(_super::m_hMenu);
	}

	/**
	 * [設定] 文字色指定
	 *	@note 指定しなければシステムの色が使用されます。
	 *	@param color1 有効項目文字色
	 *	@param color2 無効項目文字色
	 */
	void SetTextColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_textColor = color1;
		m_textDisableColor = color2;
	}

	/**
	 * [設定] 文字フォント指定
	 *	@note 指定しなければシステムのフォントが使用されます。
	 *	@param font 文字フォント。
	 */
	void SetTextFont(HFONT font)
	{
		m_font.SetClone(font);
		m_fontBold.SetAsBold(m_font);
	}

	/**
	 * [設定] 文字表示位置指定
	 *	@param m 左からのマージン。 -1 を指定すると、中央に配置されます。
	 */
	void SetLeftMargin(int m)
	{
		m_leftMargin = m;
	}

	/**
	 * [設定] デフォルトのマーク描画指定.
	 *	@note フォーカスマーク、選択マークをWindow標準と同じ様にします。
	 *	@param base 元の絵の背景に当たる色。 CLR_INVALID を指定すると、左下の色をそれとする。
	 */
	void SetDefaultMarkDrawer(COLORREF base = ::GetSysColor(COLOR_MENU))
	{
		_mark::SetDefaultMarkDrawer(base);
	}

	/**
	 * [設定] 設定の複製.
	 *	@note 文字の色、背景やマークの描画情報の設定が複製されます。メニュー内容はコピーされません。
	 *	@param o コピー元
	 */
	void CopyParameter(const CAbstractDrawingMenu& o)
	{
		CAbstractDrawingCtrl* pDC1 = this;
		const CAbstractDrawingCtrl* pDC2 = &o;
		*pDC1 = *pDC2;
		m_hWnd = o.m_hWnd; 		
		m_baseCookie = o.m_baseCookie;		
		m_items = o.m_items;			
		m_font = o.m_font;				
		m_fontBold = o.m_fontBold;				
		m_textColor = o.m_textColor;		
		m_textDisableColor = o.m_textDisableColor;	
		m_leftMargin = o.m_leftMargin;
	}

	/**
	 * [追加] メニュー追加.
	 *	@param nFlags メニュー状態。以下の値を一つ以上指定できます。	\n
	 *		MF_CHECKED		MF_UNCHECKED とトグルで動作します。既定のチェック マークを項目の隣に付けます。\n
	 *		MF_UNCHECKED	MF_CHECKED とトグルで動作します。項目の隣からチェック マークを削除します。\n
	 *		MF_DISABLED		メニュー項目を選択できないようにしますが、淡色表示にはしません。\n
	 *		MF_ENABLED		メニュー項目を選択できるようにして、淡色表示から元の表示に戻します。\n
	 *		MF_GRAYED		メニュー項目を選択できないようにして、淡色表示にします。\n
	 *		MF_MENUBARBREAK	スタティック メニューの新しい行、またはポップアップ メニューの新しい桁位置に項目を置きます。
	 *						ポップアップ メニューの新しい桁位置と古い桁位置を縦線で分けます。\n
	 *		MF_MENUBREAK	スタティック メニューの新しい行、またはポップアップ メニューの新しい桁位置に項目を置きます。
	 *						桁位置の間には分割線は描かれません。\n
	 *		※ MF_OWNERDRAW , MF_SEPARATOR , MF_STRING は指定できません。
	 *	@param draw 描画情報。
	 *	@param lpszText 文字。
	 *	@param nIDNewItem 新しいメニュー項目のコマンド ID を指定します。
	 *			nFlags の設定が MF_POPUP になっているときは、ポップアップ メニューのメニュー ハンドル (HMENU) を指定します。
	 *	@retval TRUE 成功。
	 *	@retval FALSE 失敗。
	 */
	BOOL AppendDrawingMenu(UINT nFlags, const IDrawable& draw, LPCSTR lpszText = NULL, UINT_PTR nIDNewItem = 0)
	{
		return InsertDrawingMenu(static_cast<UINT>(-1), nFlags | MF_BYPOSITION, draw, lpszText, nIDNewItem);
	}

	/**
	 * [追加] メニュー追加.
	 *	@param nPosition 直前に新しいメニュー項目が挿入される、メニュー項目を指定します。nPosition の解釈は、次に挙げる nFlags の設定により決まります。
	 *	@param nFlags メニュー状態。 AppendDrawingMenu() の nFlags 以外に値を一つ指定します。	\n
	 *				MF_BYCOMMAND	パラメータを既存のメニュー項目のコマンド ID で指定することを示します。MF_BYCOMMAND と MF_BYPOSITION がどちらも設定されていないときの既定の設定になります。
 	 *				MF_BYPOSITION	パラメータを既存のメニュー項目の位置で指定することを示します。最初の項目位置は 0 です。nPosition が -1 のときは、新しいメニュー項目はメニューの最後に追加されます。
 	 *	@param draw 描画情報。
	 *	@param lpszText 文字。
	 *	@param nIDNewItem 新しいメニュー項目のコマンド ID を指定します。
	 *			nFlags の設定が MF_POPUP になっているときは、ポップアップ メニューのメニュー ハンドル (HMENU) を指定します。
	 *	@retval TRUE 成功。
	 *	@retval FALSE 失敗。
	 */
	BOOL InsertDrawingMenu(UINT nPosition, UINT nFlags, const IDrawable& draw, LPCSTR lpszText = NULL, UINT_PTR nIDNewItem = 0)
	{
		size_t l = m_items.GetSize();
		TParam p;
		p.pDraw = draw.Clone();
		p.text = lpszText;
		UINT nf = MF_OWNERDRAW | MF_SEPARATOR | MF_STRING;
		nFlags = (nFlags & ~nf);
		BOOL r = _super::InsertMenu(nPosition, nFlags | MF_OWNERDRAW, nIDNewItem, reinterpret_cast<LPCTSTR>(m_baseCookie | l));
		if ( r )
		{
			m_items.Add(p);
			m_ResetWidths();
		}
		return r;
	}

	/**
	 * [通知] オーナードロー時.
	 *		ドロー時Itemの高さ確認のためにコールされる
	 *	@param lpMeasureItemStruct 情報
	 */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
	{
		if ( lpMeasureItemStruct->CtlType != ODT_MENU )
		{
			return;
		}
		if ( (lpMeasureItemStruct->itemData & 0xFFFF0000) != m_baseCookie )
		{
			return;
		}
#if 0
		if ( lpMeasureItemStruct->CtlID == 'TMDM' )
		{
			COwnerdrawMenuSupportProcedureHooker* pProc = reinterpret_cast<COwnerdrawMenuSupportProcedureHooker*>(lpMeasureItemStruct->itemData);
			if ( m_isAllOwnerDraw )
			{
				m_AllSetOwnerDrawState(pProc, m_withSubMenu);
			}
			return;
		}
#endif
		INDEX idx = m_ItemDataToPos(lpMeasureItemStruct->itemData);
		UINT flag = _super::GetMenuState(down_cast<UINT>(idx), MF_BYPOSITION);
		if ( idx != INVALID_INDEX )
		{
			lpMeasureItemStruct->itemWidth = down_cast<UINT>(m_items[lpMeasureItemStruct->itemData & 0xFFFF].width);
		}
		if ( flag & MF_SEPARATOR ) 
		{
			lpMeasureItemStruct->itemHeight /= 2;
		}
	}
		
	/**
	 * [通知] オーナードロー処理.
	 * 		オーナードロー時コールされます。
	 *	@note 描画情報を用意することで、任意に描画させることも出来ます。
	 *	@param lpDrawItemStruct 描画情報.
	 */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
		if ( lpDrawItemStruct->CtlType != ODT_MENU )
		{
			return;
		}
		if ( (lpDrawItemStruct->itemData & 0xFFFF0000) != m_baseCookie )
		{
			return;
		}
		INDEX idx = m_ItemDataToPos(lpDrawItemStruct->itemData);
		MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
		mii.fMask = MIIM_STRING;
		VERIFY( _super::GetMenuItemInfo(down_cast<UINT>(idx), &mii, TRUE) );
		CStr str;
		mii.cch++;
		mii.dwTypeData = str.GetBuffer(mii.cch);
		mii.fMask = MIIM_FTYPE | MIIM_BITMAP | MIIM_STATE | MIIM_STRING | MIIM_CHECKMARKS;
		VERIFY( _super::GetMenuItemInfo(down_cast<UINT>(idx), &mii, TRUE) );
		str.ReleaseBuffer();
		HDC dc = lpDrawItemStruct->hDC;
		const CRect& rc = lpDrawItemStruct->rcItem;
		CSize sz = rc.Size();
		if ( str.IsEmpty() )
		{
			INDEX i = lpDrawItemStruct->itemData & 0xFFFF;
			if ( m_items.IsInRange(i) )
			{
				str = m_items[i].text;
			}
		}
		CBitmapImage bi;
		if ( bi.Set(sz.cx, sz.cy) )
		{
			CBitmapDC bmpDC(&bi);
			_mark::DrawBackground(bmpDC, CRect(CPoint(0, 0), sz));
			if ( GetSelectMarkType() == BACKGROUND )
			{
				m_DrawSelectMaker(bmpDC, sz, lpDrawItemStruct->itemState);
			}
			if ( (mii.fType & MFT_SEPARATOR) == 0 )
			{
				//TTRACE3("flag=0x%X, id=0x%X, type=0x%X\n", flag, id, mii.fType);
				const IDrawable* pDraw = m_items[lpDrawItemStruct->itemData & 0xFFFF].pDraw;
				if ( pDraw != NULL )
				{
					SIZE s;
					pDraw->GetSize(s);
					pDraw->Draw(bmpDC, 0, (sz.cy - s.cy) / 2);
				}
				bool isEnable = (lpDrawItemStruct->itemState & (CDIS_DISABLED | CDIS_GRAYED)) == 0;
				if ( ! str.IsEmpty() )
				{
					COLORREF c = (isEnable ? m_textColor : m_textDisableColor);
					CFontHandle ff = m_font;
					if ( isEnable && (mii.fState & MFS_DEFAULT) != 0 )
					{
						ff = m_fontBold;
					}
					HGDIOBJ hOldFont = ::SelectObject(bmpDC, ff);
					::SetBkMode(bmpDC, TRANSPARENT);
					DWORD st = (m_leftMargin >= 0 ? 0 : DT_CENTER);
					int mar = max(m_leftMargin, 0);
					m_DrawText(bmpDC, CRect(2 + mar, 0, sz.cx - 1, sz.cy), c, str, st);
					::SelectObject(bmpDC, hOldFont);
				}
				UINT ts = (isEnable ? 0 : DFCS_INACTIVE);
				if ( (mii.fState & MFS_CHECKED) != 0 )
				{
					// チェック
					if ( mii.hbmpChecked == NULL )
					{
						if ( (mii.fType & MFT_RADIOCHECK) == 0 )
						{
							m_DrawIcon(bmpDC, sz, DFC_MENU, DFCS_MENUCHECK | ts);
						}
						else
						{
							m_DrawIcon(bmpDC, sz, DFC_MENU, DFCS_MENUBULLET | ts);
						}
					}
					else
					{
						CBitmapHandle b = mii.hbmpChecked; //動作未確認
						b.Draw(bmpDC);
						b.Detach();
					}
				}
				else if ( mii.hbmpItem != NULL )
				{
					int t = -1;
					switch ( reinterpret_cast<INT_PTR>(mii.hbmpItem) )
					{
					case HBMMENU_POPUP_CLOSE:
						t = DFCS_CAPTIONCLOSE;
						break;
					case HBMMENU_POPUP_RESTORE:
						t = DFCS_CAPTIONRESTORE;
						break;
					case HBMMENU_POPUP_MAXIMIZE:
						t = DFCS_CAPTIONMAX;
						break;
					case HBMMENU_POPUP_MINIMIZE:
						t = DFCS_CAPTIONMIN;
						break;
					default:
						break;
					}
					if ( t >= 0 )
					{
						m_DrawIcon(bmpDC, sz, DFC_CAPTION, t | DFCS_FLAT | ts);
					}
				}
			}
			else
			{
				// セパレータ
				CRect r(0, 0, sz.cx, sz.cy);
				CPoint po = r.CenterPoint();
				r.left += 3;
				r.right -= 3;
				r.top = po.y;
				r.bottom = po.y + 2;
				::DrawEdge(bmpDC, r, EDGE_ETCHED, BF_TOP | BF_BOTTOM);
			}
			if ( GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(bmpDC, sz, lpDrawItemStruct->itemState);
			}
			bmpDC.Draw(dc, rc.left, rc.top);
		}
	}

protected:

	/**
	 * [通知] サブメニュ発見.
	 *		Regist() で登録中にサブメニューを見つけた場合、通知されます。
	 *	@param hMenu 発見したサブメニュー
	 *	@param pProc 親となるプロシージャ
	 */
	virtual void OnFoundSubMenu(HMENU hMenu, COwnerdrawMenuSupportProcedureHooker* pProc) = 0;

	/**
	 * [設定] 登録.
	 *	@param pProc 親となるプロシージャ
	 *	@param isAllOwnerDraw true を指定するとアタッチしたアイテム、すべてオーナードロー属性を付加します。
	 *	@param withSubMenu true を指定すると、サブメニューも、すべてオーナードロー属性を付加します。
	 */
	void Regist(COwnerdrawMenuSupportProcedureHooker* pProc, bool isAllOwnerDraw = true, bool withSubMenu = true)
	{
		m_Regist(pProc);
		m_isAllOwnerDraw = isAllOwnerDraw;
		m_withSubMenu = withSubMenu;
		if ( isAllOwnerDraw )
		{
			m_AllSetOwnerDrawState(pProc, false);
		}
	}

private:
	/// アイコン描画
	void m_DrawIcon(HDC dc, const SIZE& sz, UINT t1, UINT t2)
	{
		CBitmapImage b;
		b.Set(sz.cy - 2, sz.cy - 2);
		::DrawFrameControl(b.GetDC(), CRect(-1, -1, sz.cy - 1 , sz.cy - 1), t1, t2 | DFCS_TRANSPARENT);
		b.ReleaseDC();
		b.ChangePixelColor(RGB(0, 0, 0), m_textColor);
		b.TransparentBit(dc, 1, 1);
	}
	/// 文字描画
	void m_DrawText(HDC dc, const CRect& rc, COLORREF color, const CStr& text, DWORD style = 0)
	{
		CStr s2;
		CStr s1 = text.FindCut('\t', &s2);
		CTextDrawer::DrawTextRect(dc, rc, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | style, color, s1);
		if ( ! s2.IsEmpty() )
		{
			CTextDrawer::DrawTextRect(dc, rc, DT_VCENTER | DT_RIGHT | DT_SINGLELINE | DT_END_ELLIPSIS, color, s2 + _T("   "));
		}
	}
	/// 自分を登録
	void m_Regist(COwnerdrawMenuSupportProcedureHooker* pProc)
	{
		m_baseCookie = pProc->RegisterMenu(this);
		m_hWnd = pProc->GetSafeHwnd();
	}
	/// 全部にオーナードロー状態を付加
	void m_AllSetOwnerDrawState(COwnerdrawMenuSupportProcedureHooker* pProc, bool withSubMenu = true)
	{
		m_items.RemoveAll();
		int len = ::GetMenuItemCount(m_hMenu);
		if ( len >= 0 )
		{
			loop ( i, len )
			{
				MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
				mii.fMask = MIIM_FTYPE | MIIM_STATE;
				VERIFY( ::GetMenuItemInfo(m_hMenu, down_cast<UINT>(i), TRUE, &mii) );
				mii.fMask = MIIM_FTYPE | MIIM_DATA;
				mii.dwItemData = (m_baseCookie | i);
				mii.fType |= MFT_OWNERDRAW;
				VERIFY( ::SetMenuItemInfo(m_hMenu, down_cast<UINT>(i), TRUE, &mii) );
				#ifdef _DEBUG
					CString s;
					_super::GetMenuString(down_cast<UINT>(i), s, MF_BYPOSITION);
					TTRACE3("[%s] t=0x%X s=0x%X \n", s, mii.fType, mii.fState);
				#endif
				if ( withSubMenu )
				{
					HMENU hSub = ::GetSubMenu(m_hMenu, ToInt(i));
					if ( hSub != NULL )
					{
						OnFoundSubMenu(hSub, pProc);
					}
				}
			}
		}
		m_ResetWidths();
	}
	/**
	 * アイテムデータ to MenuPos
	 *	@param data メニューアイテムの32bitの値
	 *	@return メニューのポジション。
	 */
	INDEX m_ItemDataToPos(DWORD_PTR data)
	{
		MENUITEMINFO mif = { sizeof(MENUITEMINFO), MIIM_TYPE | MIIM_DATA };
		TCHAR tempBuf[16];
		mif.cch = 10;
		mif.dwTypeData = tempBuf;
		loop ( i, _super::GetMenuItemCount() )
		{
			if ( _super::GetMenuItemInfo(down_cast<UINT>(i), &mif, TRUE) )
			{
				if ( (mif.fType & MFT_OWNERDRAW) != 0 && mif.dwItemData == data )
				{
					return i;
				}
			}
		}
		return INVALID_INDEX;
	}
	/// 幅、再計算
	void m_ResetWidths(void)
	{
		HDC dc = ::GetWindowDC(NULL);
		HGDIOBJ old = ::GetCurrentObject(dc, OBJ_FONT);
		CSize size(0, 0);
		MENUITEMINFO mif = { sizeof(MENUITEMINFO), MIIM_TYPE | MIIM_DATA | MIIM_STATE };
		TCHAR tempBuf[16];
		mif.cch = 10;
		mif.dwTypeData = tempBuf;
		int len = ::GetMenuItemCount(m_hMenu);
		if ( len >= 0 )
		{
			bool hasTab = false;
			loop ( i, len )
			{
				if ( _super::GetMenuItemInfo(down_cast<UINT>(i), &mif, TRUE) )
				{
					INDEX idx = mif.dwItemData;
					if ( (mif.fType & MFT_OWNERDRAW) != 0 && idx != 0 )
					{
						idx &= 0xFFFF;
						if ( m_items.GetSize() <= idx )
						{
							m_items.SetSize(idx + 1);
						}
						CString s = m_items[idx].text;
						if ( s.IsEmpty() )
						{
							MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
							mii.fMask = MIIM_STRING;
							VERIFY( _super::GetMenuItemInfo(down_cast<UINT>(i), &mii, TRUE) );
							CStr str;
							mii.cch++;
							mii.dwTypeData = s.GetBuffer(mii.cch);
							VERIFY( _super::GetMenuItemInfo(down_cast<UINT>(i), &mii, TRUE) );
							s.ReleaseBuffer();
						}
						if ( s.Find('\t') != INVALID_INDEX )
						{
							hasTab = true;
						}
						CFontHandle ff = m_font;
						if ( (mif.fState & MF_DEFAULT) != 0 )
						{
							ff = m_fontBold;
						}
						::SelectObject(dc, ff);
						CTextDrawer::CalcTextSize(size, dc, DT_LEFT, s);
						int mar = max(m_leftMargin, 0);
						long w = size.cx + 4 + mar;
						const IDrawable* pDraw = m_items[idx].pDraw;
						if ( pDraw != NULL && pDraw->GetSize(size) )
						{
							w = max(size.cx, w);
						}
						m_items[idx].width = w;
					}
				}
			}
			if ( hasTab )
			{
				CTextDrawer::CalcTextSize(size, dc, DT_LEFT, _T("Atl+F4   "));
				loop ( i, m_items )
				{
					m_items[i].width += size.cx;
				}
			}
		}
		::SelectObject(dc, old);
		::ReleaseDC(NULL, dc);
	}
	/// 選択マーク描画
	void m_DrawSelectMaker(HDC dc, const SIZE& sz, UINT itemState)
	{
		if ( (itemState & CDIS_HOT) != 0 )
		{
			_mark::DrawSelectMark(dc, CRect(CPoint(0, 0), sz), false);
		}
		else if ( (itemState & CDIS_SELECTED) != 0 )
		{
			_mark::DrawSelectMark(dc, CRect(CPoint(0, 0), sz), true);
		}
	}

	/// パラメータ
	struct TParam
	{
		size_t			width;	///< 幅
		IDrawable::Ptr	pDraw;	///< 描画情報
		CString			text;	///< 描画文字
	};

	HWND					m_hWnd;				///< 本メニューのウィンドウハンドル
	DWORD					m_baseCookie;		///< 本メニューのベースクッキー
	CSimpleVectorT<TParam>	m_items;			///< 各アイテム
	CFontHandle				m_font;				///< メニューで使うフォント
	CFontHandle				m_fontBold;				///< メニューで使うフォント
	COLORREF				m_textColor;		///< メニューで使う文字色
	COLORREF				m_textDisableColor;	///< メニューで使う文字色(Disable)
	int						m_leftMargin;		///< 左マージン
	bool					m_isAllOwnerDraw;
	bool					m_withSubMenu;
};



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報サブメニュー
 *
 *	@note 直接使うケースはありません。
 *	@par必要ファイル
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 10/04/16	新規作成
 */
class CDrawingSubMenu : public CAbstractDrawingMenu
{
	DEFSUPER(CAbstractDrawingMenu);
public:

	/// コンストラクタ
	CDrawingSubMenu(void) : m_pSubParameter(NULL)
	{
	}

	/// デストラクタ
	~CDrawingSubMenu(void)
	{
		if ( m_pSubParameter != NULL )
		{
			delete m_pSubParameter;
			m_pSubParameter = NULL;
		}
	}

	/**
	 * [参照] サブメニュ描画情報.
	 *	@note 戻り値の参照にサブメニュ用の描画の設定をしておきます。
	 *	@return 参照
	 */
	CAbstractDrawingMenu& ReferSubParameter(void)
	{
		if ( m_pSubParameter == NULL )
		{
			m_pSubParameter = new CDrawingSubMenu;
		}
		return *m_pSubParameter;
	}

protected:

	/**
	 * [通知] サブメニュ発見.
	 *		Regist() で登録中にサブメニューを見つけた場合、通知されます。
	 *	@param hMenu 発見したサブメニュー
	 *	@param pProc 親となるプロシージャ
	 */
	void OnFoundSubMenu(HMENU hMenu, COwnerdrawMenuSupportProcedureHooker* pProc)
	{
		loop ( i, m_subs )
		{
			if ( m_subs[i]->m_hMenu == hMenu )
			{
				return;
			}
		}
		CDrawingSubMenu* P = new CDrawingSubMenu;
		if ( m_pSubParameter == NULL )
		{
			P->CopyParameter(*this);
		}
		else
		{
			P->CopyParameter(*m_pSubParameter);
		}
		if ( P->Attach(hMenu) )
		{
			P->Regist(pProc, true, true);
			m_subs.Add(P);
			return;
		}
		delete P;
	}

private:
	CPointerVectorT<CDrawingSubMenu>	m_subs;				///< サブメニュー管理
	CDrawingSubMenu*					m_pSubParameter;	///< パラメータ用
};



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報メニュー
 *
 *		対象のメニューの表示を自由に変更すること出来ます。
 *
 *	@note SetSelectMarkDrawer() のアクティブ選択マークは Hover時、 非アクティブ選択マークは選択時に使用されます。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	新規作成
 */
class CDrawingMenu : public CDrawingSubMenu
{
	DEFSUPER(CDrawingSubMenu);
public:

	/**
	 * [設定] アタッチ.
	 *	@param pProc 親となるプロシージャ
	 *	@param hMenu アタッチするメニューハンドル
	 *	@param isAllOwnerDraw true を指定するとアタッチしたアイテム、すべてオーナードロー属性を付加します。
	 *	@param withSubMenu true を指定すると、サブメニューも、すべてオーナードロー属性を付加します。
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL Attach(COwnerdrawMenuSupportProcedureHooker* pProc, HMENU hMenu, bool isAllOwnerDraw = false, bool withSubMenu = false)
	{
		if ( _super::Attach(hMenu) )
		{
			Regist(pProc, isAllOwnerDraw, withSubMenu);
			return TRUE;
		}
		return FALSE;
	}

	/**
	 * [表示] メニュー表示.
	 *	@param nFlags フラグ
	 *	@param x スクリーンX座標
	 *	@param y スクリーンY座標
	 *	@param pWnd ポップアップメニューを有するウィンドウ
	 *	@param lpRect ポップアップ メニューが消去されずにクリックできる四角形範囲を示す画面座標指定
 	 *	@retval TRUE 成功。
	 *	@retval FALSE 失敗。
	 */
	BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd = NULL, LPCRECT lpRect = 0)
	{
		pWnd->SetForegroundWindow();
		return ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, pWnd->GetSafeHwnd(), lpRect);
	}

	/**
	 * [表示] メニュー表示.
	 *	@note 現在のマウスカーソルの位置にメニューが表示されます。
	 *	@param nFlags フラグ
 	 *	@retval TRUE 成功。
	 *	@retval FALSE 失敗。
	 */
	BOOL TrackPopupMenu(UINT nFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON)
	{
		POINT po;
		if ( ::GetCursorPos(&po) )
		{
			HWND hWnd = _super::GetParentHwnd();
			::SetForegroundWindow(hWnd);
			return ::TrackPopupMenu(m_hMenu, nFlags, po.x, po.y, 0, hWnd, NULL);
		}
		return FALSE;
	}

private:
	BOOL LoadMenu(LPCTSTR lpszResourceName);
	BOOL LoadMenu(UINT nIDResource);
	BOOL LoadMenuIndirect(const void* lpMenuTemplate);

};



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報メニューバー
 *
 *		ウィンドウの持つメニューバーの表示を自由に変更すること出来ます。
 *		ただし、高さはシステムで決まってしまうため、注意が必要です。
 *	
 *	@note SetSelectMarkDrawer() のアクティブ選択マークは Hover時、 非アクティブ選択マークは選択時に使用されます。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	新規作成
 */
class CDrawingMenuBar : public CDrawingSubMenu
{
	DEFSUPER(CDrawingSubMenu);
public:

	/**
	 * [設定] アタッチ.
	 *	@note アタッチと同時にアイテムのオーナードロー属性を設定します。
	 *	@param pProc 親となるプロシージャ
	 *	@param withSubMenu true を指定すると、サブメニューも、すべてオーナードロー属性を付加します。
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL Attach(COwnerdrawMenuSupportProcedureHooker* pProc, bool withSubMenu = false)
	{
		HWND hWnd = pProc->GetSafeHwnd();
		HMENU hMenu = ::GetMenu(hWnd);
		if ( _super::Attach(hMenu) )
		{
			Regist(pProc, true, withSubMenu);
			return TRUE;
		}
		return FALSE;
	}

	/**
	 * [描画] 描画
	 *	@note メニューを描画します。
	 *	@attention アタッチしているメニューがウィンドウに割り当てられているシステムメニューの場合のみ使用可能です。
	 *	@param dc 描画先。
	 */
	void Draw(HDC dc)
	{
		if ( IsValid() )
		{
			RECT rc;
			::GetWindowRect(_super::GetParentHwnd(), &rc);
			MENUBARINFO mbi = { sizeof(MENUBARINFO) };			// WINVER が 0x0501 じゃないとエラーになる
			MENUITEMINFO mif = { sizeof(MENUITEMINFO), MIIM_TYPE | MIIM_DATA };
			TCHAR tempBuf[16];
			mif.cch = 10;
			mif.dwTypeData = tempBuf;
			loop ( i, _super::GetMenuItemCount() )
			{
				if ( ::GetMenuBarInfo(_super::GetParentHwnd(), OBJID_MENU, down_cast<LONG>(i + 1), &mbi) )
				{
					UINT st = ::GetMenuState(mbi.hMenu, down_cast<UINT>(i), MF_BYPOSITION);
					DRAWITEMSTRUCT dis = { 0 };
					dis.CtlType = ODT_MENU;
					dis.rcItem = mbi.rcBar;
					::OffsetRect(&dis.rcItem, -rc.left, -rc.top);
					if ( _super::GetMenuItemInfo(down_cast<UINT>(i), &mif, TRUE) )
					{
						if ( (mif.fType & MFT_OWNERDRAW) != 0 )
						{
							dis.itemData = mif.dwItemData;
						}
					}
					dis.hDC = dc;
					if ( (st & MF_DISABLED) != 0 )
					{
						dis.itemState |= CDIS_DISABLED;
					}
					if ( (st & MF_GRAYED) != 0 )
					{
						dis.itemState |= CDIS_GRAYED;
					}
					if ( (st & MF_HILITE) != 0 )
					{
						dis.itemState |= CDIS_SELECTED;
					}
					DrawItem(&dis);
				}
			}
		}
	}

	/**
	 * [描画] 描画
	 *	@note メニューを対象のウィンドウに描画します。
	 *	@attention アタッチしているメニューがウィンドウに割り当てられているメニューの場合のみ使用可能です。
	 */
	void Draw(void)
	{
		HWND hWnd = _super::GetParentHwnd();
		HDC dc = ::GetWindowDC(hWnd);
		Draw(dc);
		::ReleaseDC(hWnd, dc);
	}
};



/**@ingroup MFCCONTROL DRAWABLE
 * 描画情報ロードメニュー
 *
 *		ロードしたメニューの表示を自由に変更すること出来ます。
 *
 *	@note サブメニューは管理外になります。
 *	
 *	@note SetSelectMarkDrawer() のアクティブ選択マークは Hover時、 非アクティブ選択マークは選択時に使用されます。
 *
 *	@par必要ファイル
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	新規作成
 */
class CDrawingLoadMenu : public CDrawingMenu
{
	DEFSUPER(CDrawingMenu);
public:

	/// コンストラクタ
	CDrawingLoadMenu(void)
	{
	}

	/**
	 * [設定] ロードメニュー
	 *	@param pProc 親となるプロシージャ
	 *	@param nIDResource ロードするメニューID
	 *	@param pos ロード対象になるサブメニューポジション。省略すると、メニューIDのトップを対象にします。
	 *	@param isAllOwnerDraw true を指定するとアタッチしたアイテム、すべてオーナードロー属性を付加します。
	 *	@param withSubMenu true を指定すると、サブメニューも、すべてオーナードロー属性を付加します。
	 *	@retval TRUE 成功
	 *	@retval FALSE 失敗
	 */
	BOOL LoadMenu(COwnerdrawMenuSupportProcedureHooker* pProc, UINT nIDResource, int pos = -1, bool isAllOwnerDraw = false, bool withSubMenu = false)
	{
		m_topMenu.DestroyMenu();
		if ( m_topMenu.LoadMenu(nIDResource) )
		{
			CMenu* pMenu = &m_topMenu;
			if ( pos >= 0 )
			{
				pMenu = m_topMenu.GetSubMenu(pos);
			}
			if ( pMenu != NULL )
			{
				return _super::Attach(pProc, pMenu->GetSafeHmenu(), isAllOwnerDraw, withSubMenu);
			}
		}
		return FALSE;
	}

private:
	CMenu m_topMenu;
};



}; // MFC
}; // TNB
