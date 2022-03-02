#pragma once
/**
 *@file
 * �`���񃁃j���[�֌W�̃w�b�_
 *
 *	@todo �P�Ƃ� CDrawingMenu ���g�����Ƃ͌��ݏo���܂���B CCustomWindowFrame �p�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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
 * �I�[�i�[�h���[���j���[�T�|�[�g�v���V�[�W��
 *
 *		�I�[�i�[�h���[���T�|�[�g���郁�j���[���g���_�C�A���O�ɖ{�C���X�^���X���A�^�b�`���Ă����K�v������܂��B
 *		
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	�V�K�쐬
 */
class COwnerdrawMenuSupportProcedureHooker: public CWindowProcedureHooker
{
	DEFSUPER(CWindowProcedureHooker);
public:

	/// �R���X�g���N�^
	COwnerdrawMenuSupportProcedureHooker(void) : m_cookie(0x8000)
	{
	}

	/**
	 * [�o�^] ���j���[�o�^.
	 *		�{�C���X�^���X���A�^�b�`���Ă���E�B���h�E�Ɋ֘A�Â��Ă��郁�j���[��
	 *		�o�^���܂��B�߂�l�̃x�[�X�N�b�L�[���g���A���j���[�̃I�[�i�[�h���[�̃A�C�e���̃f�[�^��ݒ肷�邱�ƂŁA
	 *		�I�[�i�[�h���[���b�Z�[�W���������邱�Ƃ��o���܂��B
	 *	@note �ʏ� CDrawingMenu CDrawingMenuBar �N���X���ŁA�ÖٓI�Ɏg�p����܂��B
	 *	@param pMenu ���j���[�B�쐬���Ă���K�v������܂��B
	 *	@return �x�[�X�N�b�L�[�B
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
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B������I�[�o�[���C�h���ď�����ǉ����܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
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
	/// �p�����[�^
	struct TParam
	{
		HMENU		hMenu;		///< ���j���[�n���h��
		CMenu*		pMenu;		///< ���j���[�N���X�|�C���^
		WORD		cookie;		///< �x�[�X�N�b�L�[�i���WORD���j
	};
	CSimpleVectorT<TParam>	m_params;	///< �p�����[�^�Q
	WORD					m_cookie;	///< �N�b�L�[
	/**
	 * ����
	 *	@param w �x�[�X�N�b�L�[(���WORD)
	 *	@retval NULL �����炸
	 *	@retval NULL�ȊO �����p�����[�^
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
 * �`����Menu�R���g���[��
 *
 *	@note SetSelectMarkDrawer() �̃A�N�e�B�u�I���}�[�N�� Hover���A ��A�N�e�B�u�I���}�[�N�͑I�����Ɏg�p����܂��B
 *
 *	@todo ���݁A�����Ɣw�i�̐F�̂ݕύX�\�ł��B
 *	@todo WM_MEASUREITEM �Ƀ��j���[�n���h����񂪂Ȃ����߁AUserData �Ȃǂňꌳ�Ǘ�����K�v����B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	�V�K�쐬
 *	@date 10/04/19	�T�u���j���[�Ή�
 *	@date 10/05/13	�t�H���g���C
 *	@date 12/11/06	�X�e�[�^�X�擾���@�����P�B
 */
class CAbstractDrawingMenu : public CMenu, public CAbstractDrawingCtrl
{
	DEFSUPER(CMenu);
	typedef CAbstractDrawingCtrl	_mark;			///< SelectMark�Ǘ����Đ錾
public:

	/// �R���X�g���N�^
	CAbstractDrawingMenu(void) 
		: _super(), m_leftMargin(20), m_baseCookie(0), m_hWnd(NULL)
		, m_textColor(::GetSysColor(COLOR_MENUTEXT)), m_textDisableColor(CLR_INVALID)
		, m_isAllOwnerDraw(false), m_withSubMenu(false)
	{
		m_font.SetSystemMenuFont();
		m_fontBold.SetAsBold(m_font);
	}

	/// �f�X�g���N�^
	~CAbstractDrawingMenu(void)
	{
	}

	/**
	 * [�擾] �e�E�B���h�E�n���h���擾.
	 *	@return �E�B���h�E�n���h��
	 */ 
	HWND GetParentHwnd(void) const
	{
		return m_hWnd;
	}

	/**
	 * [�m�F] �L���m�F.
	 *	@note WIN32API IsMenu ���g�p���Ă��܂��B
	 *	@retval true �L��.
	 *	@retval false ����
	 */
	bool IsValid(void) const
	{
		return !! ::IsMenu(_super::m_hMenu);
	}

	/**
	 * [�ݒ�] �����F�w��
	 *	@note �w�肵�Ȃ���΃V�X�e���̐F���g�p����܂��B
	 *	@param color1 �L�����ڕ����F
	 *	@param color2 �������ڕ����F
	 */
	void SetTextColor(COLORREF color1, COLORREF color2 = CLR_INVALID)
	{
		m_textColor = color1;
		m_textDisableColor = color2;
	}

	/**
	 * [�ݒ�] �����t�H���g�w��
	 *	@note �w�肵�Ȃ���΃V�X�e���̃t�H���g���g�p����܂��B
	 *	@param font �����t�H���g�B
	 */
	void SetTextFont(HFONT font)
	{
		m_font.SetClone(font);
		m_fontBold.SetAsBold(m_font);
	}

	/**
	 * [�ݒ�] �����\���ʒu�w��
	 *	@param m ������̃}�[�W���B -1 ���w�肷��ƁA�����ɔz�u����܂��B
	 */
	void SetLeftMargin(int m)
	{
		m_leftMargin = m;
	}

	/**
	 * [�ݒ�] �f�t�H���g�̃}�[�N�`��w��.
	 *	@note �t�H�[�J�X�}�[�N�A�I���}�[�N��Window�W���Ɠ����l�ɂ��܂��B
	 *	@param base ���̊G�̔w�i�ɓ�����F�B CLR_INVALID ���w�肷��ƁA�����̐F������Ƃ���B
	 */
	void SetDefaultMarkDrawer(COLORREF base = ::GetSysColor(COLOR_MENU))
	{
		_mark::SetDefaultMarkDrawer(base);
	}

	/**
	 * [�ݒ�] �ݒ�̕���.
	 *	@note �����̐F�A�w�i��}�[�N�̕`����̐ݒ肪��������܂��B���j���[���e�̓R�s�[����܂���B
	 *	@param o �R�s�[��
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
	 * [�ǉ�] ���j���[�ǉ�.
	 *	@param nFlags ���j���[��ԁB�ȉ��̒l����ȏ�w��ł��܂��B	\n
	 *		MF_CHECKED		MF_UNCHECKED �ƃg�O���œ��삵�܂��B����̃`�F�b�N �}�[�N�����ڂׂ̗ɕt���܂��B\n
	 *		MF_UNCHECKED	MF_CHECKED �ƃg�O���œ��삵�܂��B���ڂׂ̗���`�F�b�N �}�[�N���폜���܂��B\n
	 *		MF_DISABLED		���j���[���ڂ�I���ł��Ȃ��悤�ɂ��܂����A�W�F�\���ɂ͂��܂���B\n
	 *		MF_ENABLED		���j���[���ڂ�I���ł���悤�ɂ��āA�W�F�\�����猳�̕\���ɖ߂��܂��B\n
	 *		MF_GRAYED		���j���[���ڂ�I���ł��Ȃ��悤�ɂ��āA�W�F�\���ɂ��܂��B\n
	 *		MF_MENUBARBREAK	�X�^�e�B�b�N ���j���[�̐V�����s�A�܂��̓|�b�v�A�b�v ���j���[�̐V�������ʒu�ɍ��ڂ�u���܂��B
	 *						�|�b�v�A�b�v ���j���[�̐V�������ʒu�ƌÂ����ʒu���c���ŕ����܂��B\n
	 *		MF_MENUBREAK	�X�^�e�B�b�N ���j���[�̐V�����s�A�܂��̓|�b�v�A�b�v ���j���[�̐V�������ʒu�ɍ��ڂ�u���܂��B
	 *						���ʒu�̊Ԃɂ͕������͕`����܂���B\n
	 *		�� MF_OWNERDRAW , MF_SEPARATOR , MF_STRING �͎w��ł��܂���B
	 *	@param draw �`����B
	 *	@param lpszText �����B
	 *	@param nIDNewItem �V�������j���[���ڂ̃R�}���h ID ���w�肵�܂��B
	 *			nFlags �̐ݒ肪 MF_POPUP �ɂȂ��Ă���Ƃ��́A�|�b�v�A�b�v ���j���[�̃��j���[ �n���h�� (HMENU) ���w�肵�܂��B
	 *	@retval TRUE �����B
	 *	@retval FALSE ���s�B
	 */
	BOOL AppendDrawingMenu(UINT nFlags, const IDrawable& draw, LPCSTR lpszText = NULL, UINT_PTR nIDNewItem = 0)
	{
		return InsertDrawingMenu(static_cast<UINT>(-1), nFlags | MF_BYPOSITION, draw, lpszText, nIDNewItem);
	}

	/**
	 * [�ǉ�] ���j���[�ǉ�.
	 *	@param nPosition ���O�ɐV�������j���[���ڂ��}�������A���j���[���ڂ��w�肵�܂��BnPosition �̉��߂́A���ɋ����� nFlags �̐ݒ�ɂ�茈�܂�܂��B
	 *	@param nFlags ���j���[��ԁB AppendDrawingMenu() �� nFlags �ȊO�ɒl����w�肵�܂��B	\n
	 *				MF_BYCOMMAND	�p�����[�^�������̃��j���[���ڂ̃R�}���h ID �Ŏw�肷�邱�Ƃ������܂��BMF_BYCOMMAND �� MF_BYPOSITION ���ǂ�����ݒ肳��Ă��Ȃ��Ƃ��̊���̐ݒ�ɂȂ�܂��B
 	 *				MF_BYPOSITION	�p�����[�^�������̃��j���[���ڂ̈ʒu�Ŏw�肷�邱�Ƃ������܂��B�ŏ��̍��ڈʒu�� 0 �ł��BnPosition �� -1 �̂Ƃ��́A�V�������j���[���ڂ̓��j���[�̍Ō�ɒǉ�����܂��B
 	 *	@param draw �`����B
	 *	@param lpszText �����B
	 *	@param nIDNewItem �V�������j���[���ڂ̃R�}���h ID ���w�肵�܂��B
	 *			nFlags �̐ݒ肪 MF_POPUP �ɂȂ��Ă���Ƃ��́A�|�b�v�A�b�v ���j���[�̃��j���[ �n���h�� (HMENU) ���w�肵�܂��B
	 *	@retval TRUE �����B
	 *	@retval FALSE ���s�B
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
	 * [�ʒm] �I�[�i�[�h���[��.
	 *		�h���[��Item�̍����m�F�̂��߂ɃR�[�������
	 *	@param lpMeasureItemStruct ���
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
	 * [�ʒm] �I�[�i�[�h���[����.
	 * 		�I�[�i�[�h���[���R�[������܂��B
	 *	@note �`�����p�ӂ��邱�ƂŁA�C�ӂɕ`�悳���邱�Ƃ��o���܂��B
	 *	@param lpDrawItemStruct �`����.
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
					// �`�F�b�N
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
						CBitmapHandle b = mii.hbmpChecked; //���얢�m�F
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
				// �Z�p���[�^
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
	 * [�ʒm] �T�u���j������.
	 *		Regist() �œo�^���ɃT�u���j���[���������ꍇ�A�ʒm����܂��B
	 *	@param hMenu ���������T�u���j���[
	 *	@param pProc �e�ƂȂ�v���V�[�W��
	 */
	virtual void OnFoundSubMenu(HMENU hMenu, COwnerdrawMenuSupportProcedureHooker* pProc) = 0;

	/**
	 * [�ݒ�] �o�^.
	 *	@param pProc �e�ƂȂ�v���V�[�W��
	 *	@param isAllOwnerDraw true ���w�肷��ƃA�^�b�`�����A�C�e���A���ׂăI�[�i�[�h���[������t�����܂��B
	 *	@param withSubMenu true ���w�肷��ƁA�T�u���j���[���A���ׂăI�[�i�[�h���[������t�����܂��B
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
	/// �A�C�R���`��
	void m_DrawIcon(HDC dc, const SIZE& sz, UINT t1, UINT t2)
	{
		CBitmapImage b;
		b.Set(sz.cy - 2, sz.cy - 2);
		::DrawFrameControl(b.GetDC(), CRect(-1, -1, sz.cy - 1 , sz.cy - 1), t1, t2 | DFCS_TRANSPARENT);
		b.ReleaseDC();
		b.ChangePixelColor(RGB(0, 0, 0), m_textColor);
		b.TransparentBit(dc, 1, 1);
	}
	/// �����`��
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
	/// ������o�^
	void m_Regist(COwnerdrawMenuSupportProcedureHooker* pProc)
	{
		m_baseCookie = pProc->RegisterMenu(this);
		m_hWnd = pProc->GetSafeHwnd();
	}
	/// �S���ɃI�[�i�[�h���[��Ԃ�t��
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
	 * �A�C�e���f�[�^ to MenuPos
	 *	@param data ���j���[�A�C�e����32bit�̒l
	 *	@return ���j���[�̃|�W�V�����B
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
	/// ���A�Čv�Z
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
	/// �I���}�[�N�`��
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

	/// �p�����[�^
	struct TParam
	{
		size_t			width;	///< ��
		IDrawable::Ptr	pDraw;	///< �`����
		CString			text;	///< �`�敶��
	};

	HWND					m_hWnd;				///< �{���j���[�̃E�B���h�E�n���h��
	DWORD					m_baseCookie;		///< �{���j���[�̃x�[�X�N�b�L�[
	CSimpleVectorT<TParam>	m_items;			///< �e�A�C�e��
	CFontHandle				m_font;				///< ���j���[�Ŏg���t�H���g
	CFontHandle				m_fontBold;				///< ���j���[�Ŏg���t�H���g
	COLORREF				m_textColor;		///< ���j���[�Ŏg�������F
	COLORREF				m_textDisableColor;	///< ���j���[�Ŏg�������F(Disable)
	int						m_leftMargin;		///< ���}�[�W��
	bool					m_isAllOwnerDraw;
	bool					m_withSubMenu;
};



/**@ingroup MFCCONTROL DRAWABLE
 * �`����T�u���j���[
 *
 *	@note ���ڎg���P�[�X�͂���܂���B
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 10/04/16	�V�K�쐬
 */
class CDrawingSubMenu : public CAbstractDrawingMenu
{
	DEFSUPER(CAbstractDrawingMenu);
public:

	/// �R���X�g���N�^
	CDrawingSubMenu(void) : m_pSubParameter(NULL)
	{
	}

	/// �f�X�g���N�^
	~CDrawingSubMenu(void)
	{
		if ( m_pSubParameter != NULL )
		{
			delete m_pSubParameter;
			m_pSubParameter = NULL;
		}
	}

	/**
	 * [�Q��] �T�u���j���`����.
	 *	@note �߂�l�̎Q�ƂɃT�u���j���p�̕`��̐ݒ�����Ă����܂��B
	 *	@return �Q��
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
	 * [�ʒm] �T�u���j������.
	 *		Regist() �œo�^���ɃT�u���j���[���������ꍇ�A�ʒm����܂��B
	 *	@param hMenu ���������T�u���j���[
	 *	@param pProc �e�ƂȂ�v���V�[�W��
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
	CPointerVectorT<CDrawingSubMenu>	m_subs;				///< �T�u���j���[�Ǘ�
	CDrawingSubMenu*					m_pSubParameter;	///< �p�����[�^�p
};



/**@ingroup MFCCONTROL DRAWABLE
 * �`���񃁃j���[
 *
 *		�Ώۂ̃��j���[�̕\�������R�ɕύX���邱�Əo���܂��B
 *
 *	@note SetSelectMarkDrawer() �̃A�N�e�B�u�I���}�[�N�� Hover���A ��A�N�e�B�u�I���}�[�N�͑I�����Ɏg�p����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	�V�K�쐬
 */
class CDrawingMenu : public CDrawingSubMenu
{
	DEFSUPER(CDrawingSubMenu);
public:

	/**
	 * [�ݒ�] �A�^�b�`.
	 *	@param pProc �e�ƂȂ�v���V�[�W��
	 *	@param hMenu �A�^�b�`���郁�j���[�n���h��
	 *	@param isAllOwnerDraw true ���w�肷��ƃA�^�b�`�����A�C�e���A���ׂăI�[�i�[�h���[������t�����܂��B
	 *	@param withSubMenu true ���w�肷��ƁA�T�u���j���[���A���ׂăI�[�i�[�h���[������t�����܂��B
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
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
	 * [�\��] ���j���[�\��.
	 *	@param nFlags �t���O
	 *	@param x �X�N���[��X���W
	 *	@param y �X�N���[��Y���W
	 *	@param pWnd �|�b�v�A�b�v���j���[��L����E�B���h�E
	 *	@param lpRect �|�b�v�A�b�v ���j���[���������ꂸ�ɃN���b�N�ł���l�p�`�͈͂�������ʍ��W�w��
 	 *	@retval TRUE �����B
	 *	@retval FALSE ���s�B
	 */
	BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd = NULL, LPCRECT lpRect = 0)
	{
		pWnd->SetForegroundWindow();
		return ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, pWnd->GetSafeHwnd(), lpRect);
	}

	/**
	 * [�\��] ���j���[�\��.
	 *	@note ���݂̃}�E�X�J�[�\���̈ʒu�Ƀ��j���[���\������܂��B
	 *	@param nFlags �t���O
 	 *	@retval TRUE �����B
	 *	@retval FALSE ���s�B
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
 * �`���񃁃j���[�o�[
 *
 *		�E�B���h�E�̎����j���[�o�[�̕\�������R�ɕύX���邱�Əo���܂��B
 *		�������A�����̓V�X�e���Ō��܂��Ă��܂����߁A���ӂ��K�v�ł��B
 *	
 *	@note SetSelectMarkDrawer() �̃A�N�e�B�u�I���}�[�N�� Hover���A ��A�N�e�B�u�I���}�[�N�͑I�����Ɏg�p����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	�V�K�쐬
 */
class CDrawingMenuBar : public CDrawingSubMenu
{
	DEFSUPER(CDrawingSubMenu);
public:

	/**
	 * [�ݒ�] �A�^�b�`.
	 *	@note �A�^�b�`�Ɠ����ɃA�C�e���̃I�[�i�[�h���[������ݒ肵�܂��B
	 *	@param pProc �e�ƂȂ�v���V�[�W��
	 *	@param withSubMenu true ���w�肷��ƁA�T�u���j���[���A���ׂăI�[�i�[�h���[������t�����܂��B
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
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
	 * [�`��] �`��
	 *	@note ���j���[��`�悵�܂��B
	 *	@attention �A�^�b�`���Ă��郁�j���[���E�B���h�E�Ɋ��蓖�Ă��Ă���V�X�e�����j���[�̏ꍇ�̂ݎg�p�\�ł��B
	 *	@param dc �`���B
	 */
	void Draw(HDC dc)
	{
		if ( IsValid() )
		{
			RECT rc;
			::GetWindowRect(_super::GetParentHwnd(), &rc);
			MENUBARINFO mbi = { sizeof(MENUBARINFO) };			// WINVER �� 0x0501 ����Ȃ��ƃG���[�ɂȂ�
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
	 * [�`��] �`��
	 *	@note ���j���[��Ώۂ̃E�B���h�E�ɕ`�悵�܂��B
	 *	@attention �A�^�b�`���Ă��郁�j���[���E�B���h�E�Ɋ��蓖�Ă��Ă��郁�j���[�̏ꍇ�̂ݎg�p�\�ł��B
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
 * �`���񃍁[�h���j���[
 *
 *		���[�h�������j���[�̕\�������R�ɕύX���邱�Əo���܂��B
 *
 *	@note �T�u���j���[�͊Ǘ��O�ɂȂ�܂��B
 *	
 *	@note SetSelectMarkDrawer() �̃A�N�e�B�u�I���}�[�N�� Hover���A ��A�N�e�B�u�I���}�[�N�͑I�����Ɏg�p����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingMenu.h
 * 
 *	@date 08/10/09	�V�K�쐬
 */
class CDrawingLoadMenu : public CDrawingMenu
{
	DEFSUPER(CDrawingMenu);
public:

	/// �R���X�g���N�^
	CDrawingLoadMenu(void)
	{
	}

	/**
	 * [�ݒ�] ���[�h���j���[
	 *	@param pProc �e�ƂȂ�v���V�[�W��
	 *	@param nIDResource ���[�h���郁�j���[ID
	 *	@param pos ���[�h�ΏۂɂȂ�T�u���j���[�|�W�V�����B�ȗ�����ƁA���j���[ID�̃g�b�v��Ώۂɂ��܂��B
	 *	@param isAllOwnerDraw true ���w�肷��ƃA�^�b�`�����A�C�e���A���ׂăI�[�i�[�h���[������t�����܂��B
	 *	@param withSubMenu true ���w�肷��ƁA�T�u���j���[���A���ׂăI�[�i�[�h���[������t�����܂��B
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
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
