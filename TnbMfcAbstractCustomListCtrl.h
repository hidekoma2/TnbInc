#pragma once
/**
 *@file
 * �J�X�^���`�撊��ListCtrl�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �J�X�^���`�撊��ListCtrl�R���g���[��
 *
 *	@note Report�^�C�v�̈�s�S�I���X�^�C�����T�|�[�g���܂��B
 *
 *	@note OnBeginItemPaint() , OnEndItemPaint() , OnItemPainting() , OnItemPainted()
 *			�̂S����������K�v������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcAbstractCustomListCtrl.h
 * 
 *	@date 07/12/07	�V�K�쐬
 *	@date 11/03/04	ReportView�̎��A�\���͈͂ɖ����A�T�u�A�C�e���̕\���v�������Ȃ��悤�ɂ����B
 *	@date 11/09/28	�w�i�`�揈�����œK���A OnDrawBackground() ��ǉ��B
 *	@date 15/01/27	SetEraseBkGndOffset() ��ǉ��B
 */
class CAbstractCustomListCtrl : public CListCtrl
{
	DEFSUPER(CListCtrl);
public:

	/// �R���X�g���N�^
	CAbstractCustomListCtrl(void) : _super(), m_isReportView(false)
		, m_countOfSelected(0), m_itemOfSelected(-1), m_eraseBkGndOffset(2)
	{
	}
	
	/**
	 * [�擾] �I���A�C�e���擾.
	 *	@note �P��I���X�^�C����p�ł��B
	 *	@retval �}�C�i�X �I���Ȃ�
	 *	@retval �O�ȏ� �C���f�b�N�X
	 */
	int GetSelectedItem(void) const
	{
		return _super::GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	}

	/**
	 * [�ݒ�] �I���A�C�e���ݒ�.
	 *	@note �P��I���X�^�C����p�ł��B
	 *	@param item �C���f�b�N�X
	 */
	void SetSelectedItem(int item)
	{
		if ( item >= 0 )
		{
			_super::SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			_super::EnsureVisible(item, false);
		}
		else
		{
			int i = GetSelectedItem();
			if ( i >= 0 )
			{
				_super::SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}

	/**
	 * [�ݒ�] BG�N���A�����l�ݒ�.
	 *	@param f �����l�B�f�t�H���g�͂Q�ł��B
	 */
	void SetEraseBkGndOffset(int f = 2)
	{
		m_eraseBkGndOffset = f;
	}

protected:

	/// �\���p�p�����[�^
	struct TDrawParam
	{
		HDC			dc;			///< �f�o�C�X�R���e�L�X�g
		CRect		rect;		///< �`��͈�
		DWORD		itemNo;		///< �A�C�e���i���o�[
		DWORD		subItemNo;	///< �T�u�A�C�e���i���o�[
		UINT		state;	/**< ���.
							 *	�ȉ��̒l�� OR ���ꂽ��ԂŐݒ肳��Ă��܂��B\n
							 *			<table><tr><td>
							 *			�l											</td><td>
							 *					����								</td></tr><tr><td>
							 *			CDIS_CHECKED								</td><td>
							 *					�A�C�e���̓`�F�b�N����Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_DEFAULT								</td><td>
							 *					�A�C�e���̓f�t�H���g��Ԃł��B		</td></tr><tr><td>
							 *			CDIS_DISABLED								</td><td>
							 *					�A�C�e���͎g�p�s�ɂȂ��Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_FOCUS									</td><td>
							 *					�A�C�e���̓t�H�[�J�X����Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_GRAYED									</td><td>
							 *					�A�C�e���͊D�F�\���ɂȂ��Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_HOT									</td><td>
							 *					�A�C�e����hot��ԂɂȂ��Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_INDETERMINATE							</td><td>
							 *					�A�C�e����interminate��ԂɂȂ��Ă��܂��B</td></tr><tr><td>
							 *			CDIS_MARKED									</td><td>
							 *					�A�C�e���̓}�[�N����Ă��܂��B		</td></tr><tr><td>
							 *			CDIS_SELECTED								</td><td>
							 *					�A�C�e���͑I������Ă��܂��B		</td></tr>*/
		COLORREF	textColor;	///< �e�L�X�g�J���[
		COLORREF	backColor;	///< �o�b�N�J���[
	};

	/**
	 * [�ʒm] �`��J�n.
	 *		��A�̕`��̊J�n��ʒm���܂��B
	 *	@note dc �͈ȉ��A OnItemPainting() OnItemPainted() OnEndItemPaint() �Ƒ�����A�̕`��ŋ��ʂ�
	 *			�g���f�o�C�X�R���e�L�X�g�ł��B OnItemPainting() �ȂǂŃf�o�C�X�R���e�L�X�g��
	 *			��ԕύX���Ė߂��^�C�~���O���Ȃ��ꍇ�A�{���\�b�h�ŋL�����A OnEndItemPaint()
	 *			�Ō��ɖ߂��悤�ɂ��܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnBeginItemPaint(HDC dc) = 0;

	/**
	 * [�ʒm] �`��I��.
	 *		��A�̕`��̏I����ʒm���܂��B
	 *	@see OnBeginItemPaint()
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnEndItemPaint(HDC dc) = 0;

	/**
	 * [�ʒm] �A�C�e���`��.
	 *	@note �T�u�A�C�e�����ɒʒm����܂��B
	 *	@param[in,out] pParam �`��p���B�͈͂�A�C�e��NO�������Ă��܂��B
	 *	@retval CDRF_NOTIFYITEMDRAW �ʏ�̕`����s��(�{���\�b�h�ł͉������Ă��Ȃ�)�B
	 *	@retval CDRF_NEWFONT �t�H���g�╶���F��ύX�������Ԃ��܂��B
	 *	@retval CDRF_SKIPDEFAULT �{���\�b�h�ŕ`�悸�݁B
	 */
	virtual LRESULT OnItemPainting(TDrawParam* pParam) = 0;

	/**
	 * [�ʒm] �A�C�e���`��I���.
	 *	@note �T�u�A�C�e���`�悪�I��������ƁA�A�C�e�����ɒʒm����܂��B
	 *	@param[in] pParam �`��p���B rect �́A�T�u�A�C�e�����܂ޔ͈́B
	 */
	virtual void OnItemPainted(const TDrawParam* pParam) = 0;

	/**
	 * [�ʒm] �I��ύX�ʒm.
	 *	@note �I����Ԃ��ω��������ɒʒm����܂��B
	 *	@param item �I������Ă���A�C�e��NO�i��������ꍇ�A��ԎႢNO�j�B -1 �Ȃ疢�I���B
	 */
	virtual void OnSelectItemChanged(int item) { }

	/**
	 * [�ʒm] �w�i�`��ʒm.
	 *	@note �w�i��`�悷��^�C�~���O�Œʒm����܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �`��͈�
	 */
	virtual void OnDrawBackground(HDC dc, const RECT& rect)
	{
		::FillRect(dc, &rect, ::GetSysColorBrush(COLOR_WINDOW));
	}

	/**
	 * [�ʒm] for notifications from parent
	 *	@note
	 *		CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE	�����ρB
	 *	@retval FALSE	�������B
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_NOTIFY )
		{
			LPNMHDR lpNmHdr = reinterpret_cast<LPNMHDR>(lParam);
			if ( lpNmHdr->hwndFrom == m_hWnd)
			{
				LPNMLISTVIEW lpLv = reinterpret_cast<LPNMLISTVIEW>(lParam);
				if ( lpLv->hdr.code == NM_CUSTOMDRAW ) 
				{
					LPNMLVCUSTOMDRAW lpLvCd = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
					*_pResult = m_OnCustomDraw(lpLvCd);
					return TRUE;
				}
				else if ( lpLv->hdr.code == LVN_ITEMCHANGED )
				{
					SetTimer(TIMERID_CHANGED, 50, NULL);
				}
			}
		} 
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *		ListBox �֌W�̃��b�Z�[�W���t�b�N���Ă��܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_ERASEBKGND:
			{
				CHeaderCtrl* H = GetHeaderCtrl();
				if ( H !=NULL )
				{
					HDC hdc = reinterpret_cast<HDC>(wParam);
					CRect clientRect;
					_super::GetClientRect(clientRect);
					CRect rect = clientRect;
					int headerHeight = 0;
					CRect rc;
					H->GetClientRect(&rc);
					headerHeight = rc.Height();
					rect.top += headerHeight;
					if ( _super::GetItemCount() != 0 )
					{
						CRect rc;
						_super::GetItemRect(_super::GetTopIndex(), &rc, LVIR_BOUNDS);
						OnDrawBackground(hdc, CRect(0, headerHeight, rect.Width(), rc.top));
						int y = _super::GetItemCount() - _super::GetTopIndex();
//						rect.top += y * rc.Height() + 1 + 1;
						rect.top += y * rc.Height() + m_eraseBkGndOffset;
						//
						if ( _super::GetItemRect (0, rc, LVIR_BOUNDS) )
						{
							if ( rc.right < clientRect.right)
							{
								rc.left = rc.right;
								rc.right = clientRect.right;
								rc.top = clientRect.top;
								rc.bottom = rect.top;
								OnDrawBackground(hdc, rc);
							}
						}
					}
					OnDrawBackground(hdc, rect);
				}
				return 0;
			}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			Invalidate();
			break;
		case WM_VSCROLL:
		case WM_MOUSEWHEEL:
			{
				SetRedraw(FALSE);
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				SetRedraw(TRUE);
				return r;
			}
			break;
		case WM_TIMER:
			if ( wParam == TIMERID_CHANGED )
			{
				KillTimer(TIMERID_CHANGED);
				int count = _super::GetSelectedCount();
				int item = _super::GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
				if ( m_countOfSelected != count || m_itemOfSelected != item )
				{
					m_countOfSelected = count;
					m_itemOfSelected = item;
					OnSelectItemChanged(item);
				}
			}
			break;
		case WM_DESTROY:
			KillTimer(TIMERID_CHANGED);
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		ASSERT0( (_super::GetStyle() & LVS_OWNERDRAWFIXED) == 0, "AbstractCustomListCtrl", "Owner�f�[�^�� OFF�ɂ��Ă��������B" );
		_super::PreSubclassWindow();
		_super::SetExtendedStyle(LVS_EX_FULLROWSELECT);
	}

private:
	enum { TIMERID_CHANGED = 100 };
	/// Item�`��T�u
	LRESULT m_OnItemPaintSub(LPNMLVCUSTOMDRAW P, int subItemNo)
	{
		TDrawParam pa;
		pa.dc			= P->nmcd.hdc;
		pa.itemNo		= ToDword(P->nmcd.dwItemSpec);
		pa.subItemNo	= subItemNo;
		pa.state		= P->nmcd.uItemState;
		pa.textColor	= P->clrText;
		pa.backColor	= P->clrTextBk;
		_super::GetSubItemRect(pa.itemNo, subItemNo, LVIR_BOUNDS, pa.rect);
		if ( m_isReportView && subItemNo == 0 )
		{
			pa.rect.right = pa.rect.left + _super::GetColumnWidth(subItemNo);
		}
		if ( m_isReportView && (pa.rect.right < 0 || pa.rect.left > m_clientWidth) )
		{
			// �N���C�A���g���ɂ������Ă��Ȃ�
			return CDRF_SKIPDEFAULT;
		}
		if ( (_super::GetItemState(pa.itemNo, LVIS_SELECTED) & LVIS_SELECTED) != 0 )
		{
			pa.state |= CDIS_SELECTED;
		}
		else
		{
			pa.state &= ~CDIS_SELECTED;
		}
		LRESULT r = OnItemPainting(&pa);
		P->clrText = pa.textColor;
		P->clrTextBk = pa.backColor;
		return r;
	}
	/// �J�X�^���h���[�ʒm�Ή�
	LRESULT m_OnCustomDraw(LPNMLVCUSTOMDRAW P)
	{
		switch ( P->nmcd.dwDrawStage )
		{
		case CDDS_PREPAINT:
			m_isReportView = (GetStyle() & LVS_TYPEMASK) == LVS_REPORT;
			m_clientWidth = 0x8000;
			if ( m_isReportView )
			{
				CRect rc;
				GetClientRect(rc);
				m_clientWidth = rc.right;
			}
			OnBeginItemPaint(P->nmcd.hdc);
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_POSTPAINT:
			OnEndItemPaint(P->nmcd.hdc) ;
			return CDRF_DODEFAULT;
		case CDDS_ITEMPOSTPAINT:
			{
				TDrawParam pa;
				pa.dc			= P->nmcd.hdc;
				pa.itemNo		= ToDword(P->nmcd.dwItemSpec);
				pa.subItemNo	= 0;
				pa.state		= P->nmcd.uItemState;
				pa.textColor	= P->clrText;
				pa.backColor	= P->clrTextBk;
				_super::GetItemRect(pa.itemNo, pa.rect, LVIR_BOUNDS);
				OnItemPainted(&pa);
			}
			break;
		case CDDS_ITEMPREPAINT:
			if ( ! m_isReportView )
			{
				return m_OnItemPaintSub(P, 0) | CDRF_NOTIFYPOSTPAINT;
			}
			return CDRF_NOTIFYSUBITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
			return m_OnItemPaintSub(P, P->iSubItem);
		}
		return CDRF_DODEFAULT;
	}

	bool	m_isReportView;			///< �r���[���[�h�H
	int		m_countOfSelected;		///< �I��
	int		m_itemOfSelected;		///< �I��itemNo
	LONG	m_clientWidth;			///< �N���C�A���g�̈敝
	int		m_eraseBkGndOffset;		///< BG�N���A���̒����l

#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��
	public:
	/**
	 * [�擾] �A�C�e�����擾
	 *	@return �A�C�e����
	 */
	int GetItemCount() const;
	/**
	 * [�擾] �A�C�e������
	 *	@param nItem �������J�n����C���f�b�N�X�B -1 �Ȃ�ŏ�����ɂȂ�܂��B
	 *	@param nFlags �ȉ��̂����ꂩ���w�肵�܂��B
 	 *		<table>
	 *		<tr><td>�l</td>	<td>����</td></tr>
	 *					<tr><td>
	 *		LVNI_ABOVE 		</td><td>
	 *				�w�肵���A�C�e���̏�ɂ���A�C�e�����������܂��B</td>
	 *					</tr><tr><td>
	 *		LVNI_ALL		</td><td>
	 *				�C���f�b�N�X���Ŏ��̃A�C�e�����������܂� (����l)�B</td>
	 *					</tr><tr><td>
	 *		LVNI_BELOW		</td><td>
	 *				�w�肵���A�C�e���̉��ɂ���A�C�e�����������܂��B</td>
	 *					</tr><tr><td>
	 *		LVNI_TOLEFT		</td><td>
	 *				�w�肵���A�C�e���̍��ɂ���A�C�e�����������܂��B</td>
	 *					</tr><tr><td>
	 *		LVNI_TORIGHT		</td><td>
	 *				�w�肵���A�C�e���̉E�ɂ���A�C�e�����������܂��B</td>
	 *					</tr>
	 *		</table>
	 *		�ȉ��̃t���O�����킹�Ďw��ł��܂��B
 	 *		<table>
	 *		<tr><td>�l</td>	<td>����</td></tr>
	 *					<tr><td>
	 *		LVNI_DROPHILITED 		</td><td>
	 *				�A�C�e���� LVIS_DROPHILITED ��ԃt���O���ݒ肳��Ă���B</td>
	 *					</tr><tr><td>
	 *		LVNI_FOCUSED		</td><td>
	 *				�A�C�e���� LVIS_FOCUSED ��ԃt���O���ݒ肳��Ă���B</td>
	 *					</tr><tr><td>
	 *		LVNI_SELECTED		</td><td>
	 *				�A�C�e���� LVIS_SELECTED ��ԃt���O���ݒ肳��Ă���B</td>
	 *					</tr>
	 *		</table>
	 *	@retval �}�C�i�X �������B
	 *	@retval 0�ȏ� �����B���l�̓C���f�b�N�XNo�B
	 */
	int GetNextItem(int nItem, int nFlags) const;
	/**
	 * [�擾] �I���A�C�e������.
	 *	@retval NULL �������B
	 *	@retval NULL�ȊO �����B GetNextSelectedItem() �Ŏg�p�ł��܂��B
	 */
	POSITION GetFirstSelectedItemPosition() const;
	/**
	 * [�擾] �I���A�C�e������.
	 *	@param[in,out] _pos �O�� GetNextSelectedItem() �܂��� 
	 *					GetFirstSelectedItemPosition() �̌Ăяo���ŕԂ��ꂽ 
	 *					POSITION �l�ւ̎Q�ƁB���̒l�́A����̌Ăяo���ɂ����
	 *					���̈ʒu�ɍX�V����܂��B
	 *	@note �{�֐����g���O�� _pos �l�� NULL �ȊO�ł��邱�Ƃ��m�F����K�v������܂��B
	 *	@retval ���������C���f�b�N�XNo�B
	 */
	int GetNextSelectedItem(POSITION& _pos) const;
	/**
	 * [�擾] ��ԏ�\���̃A�C�e��
	 *	@return ��ԏ�ɕ\������Ă���A�C�e���̃C���f�b�N�X
	 */
	int GetTopIndex() const;
	/**
	 * [�擾] ���������ɕ\�������A�C�e����
	 *	@return �\���̈�̐��������ɓ���A�C�e������Ԃ��܂��B
	 */
	int GetCountPerPage() const;
	/**
	 * [�ݒ�] �A�C�e����Ԑݒ�
	 *	@param nItem �C���f�b�N�X
	 *	@param nState {@link CAbstractCustomListCtrl::EState ��ԃr�b�g} �̐V�����l�B
	 *	@param nMask �ǂ� {@link CAbstractCustomListCtrl::EState ��ԃr�b�g} ��ύX���邩���w�肷��l�B
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOL SetItemState(int nItem, UINT nState, UINT nMask);
	/**
	 * [�擾] �A�C�e����Ԏ擾
	 *	@param nItem �C���f�b�N�X
	 *	@param nMask �ǂ� {@link CAbstractCustomListCtrl::EState ��ԃr�b�g} ��Ԃ����w�肷��l�B
	 *	@return �w��A�C�e���� {@link CAbstractCustomListCtrl::EState ��ԃr�b�g} �B
	 */
	UINT GetItemState(int nItem, UINT nMask) const;
	/**
	 * [�擾] �I���A�C�e�����擾.
	 *	@return �I������Ă���A�C�e������Ԃ��܂��B
	 */
	UINT GetSelectedCount() const;
//	CString GetItemText(int nItem, int nSubItem) const;
//	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
//	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
//	BOOL SetItemData(int nItem, DWORD dwData);
//	DWORD GetItemData(int nItem) const;
	/**
	 * �A�C�e����Ԓl.
	 *	@note ���ۂ� define �Ő錾����Ă��܂��B
	 */
	enum EState
	{
		LVIS_ACTIVATING,	///< Not currently supported.
		LVIS_CUT,			///< The item is marked for a cut-and-paste operation.
		LVIS_DROPHILITED,	///< The item is highlighted as a drag-and-drop target.
		LVIS_FOCUSED,		///< The item has the focus, so it is surrounded by a standard focus rectangle. Although more than one item may be selected, only one item can have the focus.
		LVIS_OVERLAYMASK,	///< The item's overlay image index is retrieved by a mask.
		LVIS_SELECTED,		///< The item is selected. The appearance of a selected item depends on whether it has the focus and also on the system colors used for selection.
		LVIS_STATEIMAGEMASK	///< The item's state image index is retrieved by a mask.
	};
#endif
};



}; // MFC
}; // TNB
