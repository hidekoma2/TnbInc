#pragma once
/**
 *@file
 * �`����ListCtrl�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcAbstractCustomListCtrl.h"
#include "TnbMfcAbstractDrawingCtrl.h"
#include "TnbMfcDrawingHeaderCtrl.h"
#include "TnbMfcBitmapDC.h"
#include "TnbPartsDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL DRAWABLE
 * �`����ListCtrl�R���g���[��
 *
 *		�s�P�ʂŕ\������`������w��o���܂��B
 *
 *	@note �A�C�e���̍�����ύX����ɂ́A�t�H���g��ݒ肵�Ă��������B
 *	@note �h���b�O���h���b�v���T�|�[�g����Ȃ�A CDragDrawingListCtrl ���g�p���Ă��������B
 *	@note �w�b�_�̍�����ύX����ɂ́A�w�b�_�̃t�H���g��ݒ肵�Ă��������B
 *
 *	@note �`�掞�A {@link IDrawable::DrawEx() DrawEx()���\�b�h} ���g�p���Ă��܂��B lParam �ɂ́A�A�C�e��No������܂��B
 *		lParam �̒l��ύX����ɂ́A CalcDrawParam() ���p�����܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ń��X�g�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCListCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		���X�g�R���g���[���̃v���p�e�B�́A ���|�[�g�ɂ��Ă����Ă��������B
 *		�i�{�N���X�̓��|�[�g�`���̂݃T�|�[�g���Ă��܂��j
 *
 *	@note �v���p�e�B�Łu�w�b�_���\�[�g���Ȃ��v�ɂ���ƃ}�E�X�ɂ��J�����̕��ύX���֎~���Ă��܂��B
 *		���̏�Ԃŕ��ύX�����������ꍇ�A GetHeaderCtrl()->EnableChangeWidth(true); �Ƃ��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrawingListCtrl.h
 * 
 *	@date 07/12/07	�V�K�쐬
 *	@date 08/01/18	���INDEX -> itemNO�e�[�u���ǉ��B
 *	@date 08/06/17	�J�����������̎����������ɑΉ��B
 *	@date 08/09/16	CheckBox������T�|�[�g�B
 *	@date 10/01/06	�e�L�X�g�̏ꍇ�A�\���ʒu���J�����̐ݒ�Ɠ����ɂȂ�悤�ɂ����B
 *	@date 10/01/07	Destroy���̃}�[�N�֌W�̏������R��C���B
 *	@date 10/02/26	�`�掞�A IDrawable::DrawEx() ���g���悤�ɕύX�B CalcDrawParam() �ǉ��B
 *	@date 11/09/28	�w�i�`���e�N���X�ɔC�����B OnDrawBackground() ��ǉ��B
 *	@date 14/12/14	SetItemText() ���ɕ`������N���A����悤�ɏC���B
 *	@date 15/04/01	SetTextEndEllipsisMode() ��ǉ��B
 */
class CDrawingListCtrl : public CAbstractCustomListCtrl, public CAbstractDrawingCtrl
{
	DEFSUPER(CAbstractCustomListCtrl);
	typedef CAbstractDrawingCtrl _mark;					///< SelectMark�Ǘ����Đ錾
	typedef CAutoVectorT<IDrawable::Ptr> CDrawersVector;	///< �`����̔z��^
	typedef CAutoVectorT<CDrawersVector> CDrawersVectors;	///< �`����̓񎟌��z��^
public:

	/// �R���X�g���N�^
	CDrawingListCtrl(void) : _super(), m_pPartsDrawer(NULL), m_withDtEndEllipsis(false)
	{
	}

	/// �f�X�g���N�^
	~CDrawingListCtrl(void)
	{
		m_ResetContent();
	}

	/**
	 * [�ݒ�] �������������[�h�ݒ�.
	 *	@note �f�t�H���g�ł� true �ł��B
	 *	@param b true �ɂ���ƁA�����N���C�A���g�Ɠ����ɂȂ�悤�ɒ������܂�(���X�N���[���o�[���o�Ȃ�)�B
	 *			 false �̓I���W�i���� CListCtrl �Ɠ��������ɂȂ�܂��B
	 */
	void SetAdjustWidthMode(bool b = true)
	{
		m_headerCtrl.SetAdjustWidthMode(b);
	}

	/**
	 * [�ݒ�] �e�L�X�g�ȗ��\�����[�h�ݒ�.
	 *	@note �f�t�H���g�ł� false �ł��B
	 *	@param f true ����ƁA�J�����ɓ��肫��Ȃ������̍Ō�� ... �ɂ��܂�(CListCtrl �Ɠ���)�B
	 */
	void SetTextEndEllipsisMode(bool f = false)
	{
		m_withDtEndEllipsis = f;
	}

	/**
	 * [�ݒ�] �`�F�b�N�{�b�N�X�ǉ�
	 *	@note ���X�g�̍��[�Ƀ`�F�b�N�{�b�N�X�����܂��B
	 *	@param P �`�F�b�N�{�b�N�X�`��p�B DFCS_BUTTONCHECK , DFCS_FLAT �� DFCS_CHECKED ����Ȃ����g�p����܂��B
	 */
	void AddCheckBox(IPartsDrawable* P)
	{
		m_pPartsDrawer = P;
		DWORD ss = _super::GetExtendedStyle();
		_super::SetExtendedStyle(ss | LVS_EX_CHECKBOXES);
	}

	/**
	 * [�擾] �A�C�e���̕`����擾.
	 *	@param itemNo �A�C�e��NO.
	 *	@param subItemNo �T�u�A�C�e��NO.
	 *	@retval NULL �G���[.
	 *	@retval NULL�ȊO �`����Bdelete ���Ă͂����܂���B
	 */
	IDrawable* GetItemDrawer(int itemNo, int subItemNo)
	{
		if ( itemNo >= 0 && itemNo < GetItemCount() )
		{
			if ( subItemNo >= 0 && subItemNo < GetHeaderCtrl()->GetItemCount() )
			{
				try
				{
					return m_drawersGlid[itemNo][subItemNo];
				}
				catch( CTnbException& e )
				{
					e.OnCatch();
				}
			}
		}
		return NULL;
	}

	/**
	 * [�ݒ�] �A�C�e���̕�����ݒ�.
	 *	@param itemNo �A�C�e��NO.
	 *	@param subItemNo �T�u�A�C�e��NO.
	 *	@param lpszText �e�L�X�g�B
	 *	@retval FALSE �G���[�B
	 *	@retval TRUE �����B
	 */
	BOOL SetItemText(int itemNo, int subItemNo, LPCTSTR lpszText)
	{
		BOOL r = _super::SetItemText(itemNo, subItemNo, lpszText);
		if ( r )
		{
			try
			{
				m_drawersGlid[itemNo][subItemNo].Null();
			}
			catch( CTnbException& e )
			{
				e.OnCatch();
			}
		}
		return r;
	}

	/**
	 * [�ݒ�] �A�C�e���̕`����ݒ�.
	 *	@note ItemData�ɂ͐��INDEX���i�[����܂��B
	 *	@param itemNo �A�C�e��NO.
	 *	@param subItemNo �T�u�A�C�e��NO.
	 *	@param draw �`����B �������L�����܂��̂ŁA draw �͔j�����Ă��܂��܂���B
	 *	@param isInsert �ȗ��� false �Ȃ�A�㏑���B true �Ȃ�}�����܂��B���̍� subItemNo �͖������܂��B
	 *	@param lpszWidth ���ݒ�p������B �ȗ�����ƁA draw ���畝���擾���A�g�p���܂��B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B���l�́A�ǉ������A�C�e��No�B
	 */
	int SetDrawItem(int itemNo, int subItemNo, const IDrawable& draw, bool isInsert = false, LPCTSTR lpszWidth = NULL)
	{
		CSize size;
		if ( ! draw.GetSize(size) )
		{
			return -1;
		}
		CSize sz(0, 0);
		CString str;
		if ( lpszWidth == NULL )
		{
			CDC* pDC = _super::GetDC();
			CFont* pOldFont = pDC->SelectObject(_super::GetFont());
			while ( sz.cx < size.cx )
			{
				str += _T("W");
				CTextDrawer::CalcTextSize(sz, *pDC, 0, str);
			}
			str = str.Left(str.GetLength() - 1);
			sz.cx = 0;
			while ( sz.cx < size.cx )
			{
				str += _T("l");
				CTextDrawer::CalcTextSize(sz, *pDC, 0, str);
			}
			str = str.Left(str.GetLength() - 1);
			pDC->SelectObject(pOldFont);
			_super::ReleaseDC(pDC);
		}
		else
		{
			str = lpszWidth;
		}
		_super::SetRedraw(false);
		if ( isInsert )
		{
			itemNo = _super::InsertItem(itemNo, str);
			if ( itemNo >= 0 )
			{
				subItemNo = 0;
				INDEX n = m_FindNoUseAbsoluteIndex();
				_super::SetItemData(itemNo, n);
				m_drawersGlid.Insert(itemNo, CDrawersVector());
				m_drawersLine.Insert(itemNo, IDrawable::Ptr());
				m_absoluteIndexs.Insert(itemNo, n);
				m_ClearIndexCache();
			}
		}
		else if ( subItemNo == 0 && itemNo >= _super::GetItemCount() )
		{
			itemNo = _super::InsertItem(itemNo, str);
			if ( itemNo >= 0 )
			{
				INDEX n = m_FindNoUseAbsoluteIndex();
				_super::SetItemData(itemNo, n);
				m_absoluteIndexs[itemNo] = n;
				m_ClearIndexCache();
			}
		}
		else
		{
			if ( ! _super::SetItemText(itemNo, subItemNo, str) )
			{
				itemNo = -1;
			}
		}
		_super::SetRedraw(true);
		if ( itemNo >= 0 )
		{
			m_drawersGlid[itemNo][subItemNo] = draw.Clone();
			_super::Invalidate();
		}
		m_ViewIndexCache();
		return itemNo;
	}

	/**
	 * [�ݒ�] �A�C�e���`����ǉ�.
	 *	@note ItemData�ɂ͐��INDEX���i�[����܂��B
	 *	@param draw �`����B �������L�����܂��̂ŁA draw �͔j�����Ă��܂��܂���B
	 *	@retval -1 �G���[�B
	 *	@retval 0�ȏ� �����B���l�� itemNo�B
	 */
	int AddDrawItem(const IDrawable& draw)
	{
		int l = _super::GetItemCount();
		return SetDrawItem(l, 0, draw);
	}

	/**
	 * [�ݒ�] �A�C�e���`����}��.
	 *	@note ItemData�ɂ͐��INDEX���i�[����܂��B
	 *	@param itemNo �}���ꏊ�B
	 *	@param draw �`����B �������L�����܂��̂ŁA draw �͔j�����Ă��܂��܂���B
	 *	@retval -1 �G���[�B
	 *	@retval 0�ȏ� �����B���l�� itemNo�B
	 */
	int InsertDrawItem(int itemNo, const IDrawable& draw)
	{
		return SetDrawItem(itemNo, 0, draw, true);
	}

	/**
	 * [�ݒ�] �A�C�e������ւ�
	 *	@param itemNo1 ����ւ��Ώ� ItemNO ��ځB 
	 *	@param itemNo2 ����ւ��Ώ� ItemNO ��ځB
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SwapItem(int itemNo1, int itemNo2)
	{
		int l = _super::GetItemCount();
		if ( itemNo1 < 0 || itemNo1 >= l ) { return false; }
		if ( itemNo2 < 0 || itemNo2 >= l ) { return false; }
		//
		_super::SetRedraw(false);
		m_SwapItem(itemNo1, itemNo2);	// ������ւ�
		_super::SetRedraw(true);
		_super::Invalidate();
		m_ViewIndexCache();
		return true;
	}
		
	/**
	 * [�ݒ�] �A�C�e������ւ�.
	 *	@param fromItemNo �ړ��Ώ� ItemNO�B 
	 *	@param toItemNo �}����ItemNO�B
	 *	@retval �}�C�i�X ���s
	 *	@retval 0�ȏ� �����B���l�́A�}�����ItemNo
	 */
	int MoveItem(int fromItemNo, int toItemNo)
	{
		int l = _super::GetItemCount();
		if ( fromItemNo < 0 || fromItemNo >= l ) { return -1; }
		if ( toItemNo < 0 || toItemNo >= l + 1 ) { return -1; }
		if ( fromItemNo == toItemNo )//|| fromItemNo == toItemNo + 1 )
		{
			return fromItemNo; //��������OK����
		}
		_super::SetRedraw(false);
		int it = -1;
		if ( fromItemNo > toItemNo )
		{
			for ( int i = fromItemNo; i >= toItemNo + 1; i-- )
			{
				m_SwapItem(i, i - 1);
			}
			it = toItemNo;
		}
		else
		{
			for ( int i = fromItemNo; i <= toItemNo - 2; i++ )
			{
				m_SwapItem(i, i + 1);
			}
			it = toItemNo - 1;
		}
		_super::SetRedraw(true);
		m_ViewIndexCache();
		return it;
	}

	/**
	 * [�폜] �A�C�e���`����폜.
	 *	@param item itemNo�B
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
	 */
	BOOL DeleteItem(int item)
	{
		BOOL r = _super::DeleteItem(item);
		if ( r )
		{
			m_drawersGlid.Remove(item);
			m_drawersLine.Remove(item);
			m_absoluteIndexs.Remove(item);
			m_ClearIndexCache();
			m_ViewIndexCache();
		}
		return r;
	}

	/**
	 * [�폜] �S�A�C�e���폜.
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
	 */
	BOOL DeleteAllItems(void)
	{
		BOOL r = _super::DeleteAllItems();
		m_ResetContent();
		m_ViewIndexCache();
		return r;
	}

	/**
	 * [�擾] ���Index ���� ItemNo �擾
	 *	@param index ���INDEX
	 *	@retval -1 ������Ȃ��B
	 *	@retval �O�ȏ� ItemNo�B
	 */
	int AbsoluteIndexToItemNo(INDEX index) const
	{
		return m_AbsoluteIndexToItemNo(index);
	}

	/**
	 * [�擾] ItemNo ���� ��� Index �擾
	 *	@param itemNo �A�C�e��NO�B
	 *	@retval INVALID_INDEX ������Ȃ��B
	 *	@retval INVALID_INDEX�ȊO ���INDEX�B
	 */
	INDEX ItemNoToAbsoluteIndex(int itemNo) const
	{
		if ( m_absoluteIndexs.IsInRange(itemNo) )
		{
			return m_absoluteIndexs[itemNo];
		}
		return INVALID_INDEX;
	}

	/**
	 * [�擾] �w�b�_�R���g���[���Q��.
	 *	@retval NULL �w�b�_�͂���܂���B
	 *	@retval NULL�ȊO �w�b�_�R���g���[���̃|�C���^�B
	 */
	CDrawingHeaderCtrl* GetHeaderCtrl(void)
	{
		m_CheckHeaderCtrl();
		return m_headerCtrl.IsWindow() ? &m_headerCtrl : NULL;
	}

	/**
	 * [�ǉ�] �J�����ǉ�.
	 *	@param nCol �V������̃C���f�b�N�X
	 *	@param pColumn �V������̑������
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�l�́A�V������̃C���f�b�N�X�B
	 */
	int InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{
		m_CheckHeaderCtrl();
		int r = _super::InsertColumn(nCol, pColumn);
		return r;
	}

	/**
	 * [�ǉ�] �J�����ǉ�.
	 *	@param nCol �V������̃C���f�b�N�X
	 *	@param lpszColumnHeading ��w�b�_�[����������B
	 *	@param nFormat ��̔z�u���w�肷�鐮���B���̒l�̂ǂꂩ 1 ���w�肵�܂��B
	 *					LVCFMT_LEFT�ALVCFMT_RIGHT�A�܂��� LVCFMT_CENTER�B
	 *	@param nWidth �s�N�Z���P�ʂ̗�̕��B���̃p�����[�^�� -1 ���w�肷��ƁA��̕��͐ݒ肳��܂���B
	 *	@param nSubItem ��Ɋ֘A�t����ꂽ�T�u�A�C�e���̃C���f�b�N�X�B
	 *					���̃p�����[�^�� -1 ���w�肷��ƁA��Ɋ֘A�t������T�u�A�C�e���͂���܂���B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�l�́A�V������̃C���f�b�N�X�B
	 */
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1)
	{
		m_CheckHeaderCtrl();
		int r = _super::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
		if ( r == 0 )
		{
			HDITEM hi;
			hi.mask = HDI_FORMAT;
			if ( m_headerCtrl.GetItem(nCol, &hi) )
			{
				hi.fmt = (hi.fmt & ~LVCFMT_JUSTIFYMASK) | nFormat;
				hi.mask = HDI_FORMAT;
				m_headerCtrl.SetItem(nCol, &hi);
			}
		}
		//m_headerCtrl.Reset();
		return r;
	}

	/**
	 * [�ǉ�] �J�����ǉ�.
	 *	@param nCol �V������̃C���f�b�N�X
	 *	@param nWidth �s�N�Z���P�ʂ̗�̕��B���̃p�����[�^�� -1 ���w�肷��ƁA��̕��͐ݒ肳��܂���B
	 *	@param draw �\������`����B������ێ����܂��̂ŁA draw �͔j�����Ă����܂��܂���B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval 0�ȏ� �����B�l�́A�V������̃C���f�b�N�X�B
	 */
	int InsertColumn(int nCol, int nWidth, const IDrawable& draw)
	{
		m_CheckHeaderCtrl();
		int r = _super::InsertColumn(nCol, _T(""), LVCFMT_LEFT, nWidth, -1);
		if ( r >= 0 )
		{
			m_headerCtrl.SetColumnDrawer(nCol, draw);
			//m_headerCtrl.Reset();
		}
		return r;
	}

	/**
	 * [�ݒ�] �㏑���`����ݒ�.
	 *	@note �S�T�u�A�C�e���`���ɁA�A�C�e�����ɑ΂��ĕ`�悷��ݒ���s���܂��B
	 *	@param itemNo �A�C�e��NO.
	 *	@param draw �`����B �������L�����܂��̂ŁA draw �͔j�����Ă��܂��܂���B
	 *	@retval false �G���[�B
	 *	@retval true �����B
	 */
	bool SetItemOverDrawer(int itemNo, const IDrawable& draw)
	{
		SIZE size;
		if ( draw.GetSize(size) )
		{
			m_drawersLine[itemNo] = draw.Clone();
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *	@param color �F
	 *	@return ��� TRUE
	 */
	BOOL SetBkColor(COLORREF color)
	{
		SetBackColor(color);
		return TRUE;
	}
	
	/**
	 * [�ݒ�] �A�C�e����������}��.
	 *	@param itemNo �}���ꏊ�B
	 *	@param lpszItem �`�敶������B
	 *	@retval -1 �G���[�B
	 *	@retval 0�ȏ� �����B���l�� itemNo�B
	 */
	int InsertItem(int itemNo, LPCTSTR lpszItem)
	{
		return _super::InsertItem(itemNo, lpszItem);
	}

	/**
	 * [�ݒ�] �A�C�e����Ԑݒ�
	 *	@param nItem �C���f�b�N�X
	 *	@param nState {@link CAbstractCustomListCtrl::EState ��ԃr�b�g} �̐V�����l�B
	 *	@param nMask  �ǂ� {@link CAbstractCustomListCtrl::EState ��ԃr�b�g} ��ύX���邩���w�肷��l�B
	 *	@retval TRUE ����.
	 *	@retval FALSE ���s.
	 */
	BOOL SetItemState(int nItem, UINT nState, UINT nMask)
	{
		return _super::SetItemState(nItem, nState, nMask);
	}

protected:

	/**
	 * [�ʒm] �`��J�n.
	 *		��A�̕`��̊J�n��ʒm���܂��B
	 *	@note dc �͈ȉ��AOnItemPaint() OnEndItemPaint() �Ƒ�����A�̕`��ŋ��ʂ�
	 *			�g���f�o�C�X�R���e�L�X�g�ł��BOnItemPaint() �Ńf�o�C�X�R���e�L�X�g��
	 *			��ԕύX���Ė߂��^�C�~���O���Ȃ��ꍇ�A�{���\�b�h�ŋL�����A OnEndItemPaint()
	 *			�Ō��ɖ߂��悤�ɂ��܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnBeginItemPaint(HDC dc)
	{
		_mark::BeginItemPaint();
		m_hasCheckBox = (GetExtendedStyle() & LVS_EX_CHECKBOXES) != 0;
		CDrawingHeaderCtrl* H = GetHeaderCtrl();
		if ( H != NULL )
		{
			CRect rc;
			H->GetClientRect(&rc);
			HRGN rgn = ::CreateRectRgnIndirect(&rc);
			::ExtSelectClipRgn(dc, rgn, RGN_DIFF);
			_DeleteObject(rgn);
		}
		if ( (_super::GetStyle() & LVS_NOCOLUMNHEADER) != 0 )
		{
			if ( m_headerCtrl.IsAdjustWidthMode() )
			{
				CRect rc;
				_super::GetClientRect(rc);
				int right = rc.right;
				int cnt = m_headerCtrl.GetItemCount();
				for ( int i = 0; i < cnt; i++ )
				{
					CRect subRc;
					m_headerCtrl.GetItemRect(i, subRc);
					rc.right = rc.left + subRc.Width();
					bool r = rc.right > right;
					if ( i + 1 == cnt )
					{
						r = rc.right != right;
					}
					if ( r )
					{
						HDITEM h;
						h.mask = HDI_WIDTH;
						h.cxy = right - rc.left;
						if ( h.cxy < 0 ) { h.cxy = 0; }
						m_headerCtrl.SetItem(i, &h);
						rc.right = right;
					}
					rc.left = rc.right;
				}
			}
		}
	}

	/**
	 * [�ʒm] �`��I��.
	 *		��A�̕`��̏I����ʒm���܂��B
	 *	@see OnBeginItemPaint()
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnEndItemPaint(HDC dc)
	{
		if ( _mark::EndItemPaint() )
		{
			Invalidate();
		}
	}

	/**
	 * [�擾] �`��p�p�����[�^�v�Z.
	 *		�`����� IDrawable::DrawEx() �� lParam �ɓn���l���v�Z���܂��B
	 *	@note �{���\�b�h�ł́A itemNo �� lParam �Ƃ��Ă܂��B
	 *	@param itemNo �A�C�e�� No.
	 *	@param subItemNo �T�u�A�C�e�� No.
	 *	@return �p�����[�^.
	 */
	virtual LPARAM CalcDrawParam(DWORD_PTR itemNo, DWORD subItemNo) const
	{
		return itemNo;
	}
	
	/**
	 * [�ʒm] �A�C�e���`��.
	 *	@note �T�u�A�C�e�����ɒʒm����܂��B
	 *	@param[in,out] P �`��p���B�͈͂�A�C�e��NO�������Ă��܂��B
	 *	@retval CDRF_NOTIFYITEMDRAW �ʏ�̕`����s��(�{���\�b�h�ł͉������Ă��Ȃ�)�B
	 *	@retval CDRF_NEWFONT �t�H���g�╶���F��ύX�������Ԃ��܂��B
	 *	@retval CDRF_SKIPDEFAULT �{���\�b�h�ŕ`�悸�݁B
	 */
	virtual LRESULT OnItemPainting(TDrawParam* P)
	{
//		TRACE3("OnSubItemPaint[%d, %d, %08X]\n", pParam->itemNo, pParam->subItemNo, pParam->state);
		CRect rc = P->rect;
		rc.OffsetRect(-rc.TopLeft());
		CBitmapImage bi;
		bi.Set(rc.Width(), rc.Height(), _mark::GetBackColor());
		LPARAM lParam = CalcDrawParam(P->itemNo, P->subItemNo);
		if ( ! bi.IsEmpty() )
		{
			CBitmapDC dc(&bi);
			_mark::DrawBackground(dc, rc, lParam);
			int x = 0;
			if ( P->subItemNo == 0 && m_hasCheckBox && m_pPartsDrawer != NULL )
			{
				CRect r = rc;
				UINT s = DFCS_BUTTONCHECK | DFCS_FLAT;
				if ( _super::GetCheck(P->itemNo) )
				{
					s |= DFCS_CHECKED;
				}
				m_pPartsDrawer->DrawButtonControl(NULL, &r, s);
				r.OffsetRect(1, (rc.Height() - r.Height()) / 2);
				m_pPartsDrawer->DrawButtonControl(dc, &r, s);
				m_checkboxWidth = r.Width() + 2;
				x += m_checkboxWidth;
				rc.left += m_checkboxWidth;
				if ( rc.right < rc.left )
				{
					rc.right = rc.left;
				}
			}
			if ( _mark::GetSelectMarkType() == BACKGROUND )
			{
				m_DrawSelectMaker(dc, rc, P->state, lParam);
			}
			IDrawable* pDraw = GetItemDrawer(P->itemNo, P->subItemNo);
			if ( pDraw != NULL )
			{
				pDraw->Resize(rc.Size());
				pDraw->DrawEx(dc, x, 0, lParam);
			}
			else
			{
				CString str = GetItemText(P->itemNo, P->subItemNo);
				HDITEM hi;
				hi.mask = HDI_FORMAT;
				if ( m_headerCtrl.GetItem(P->subItemNo, &hi) )
				{
					DWORD st = _mark::GetTextDrawStyle() & (~0x03);
					switch ( hi.fmt & HDF_JUSTIFYMASK )
					{
					case HDF_RIGHT:
						st |= DT_RIGHT;
						break;
					case HDF_CENTER:
						st |= DT_CENTER;
						break;
					default:
						if ( m_withDtEndEllipsis )
						{
							st |= DT_END_ELLIPSIS;
						}
						break;
					}
					CSize sz = P->rect.Size();
					_mark::SetDefaultTextDrawer(this);
					_mark::DrawText(dc, CRect(x, 0, sz.cx, sz.cy), str, st, lParam);
				}
			}
			if ( _mark::GetSelectMarkType() == OVERLAP )
			{
				m_DrawSelectMaker(dc, rc, P->state, lParam);
			}
			dc.Draw(P->dc, P->rect.left, P->rect.top);
			if ( pDraw != NULL )
			{
				pDraw->DrawEx(NULL, P->rect.left, P->rect.top, lParam);
			}
		}
		return CDRF_SKIPDEFAULT;
	}

	/**
	 * [�ʒm] �A�C�e���`��I���.
	 *	@note �T�u�A�C�e���`�悪�I��������ƁA�A�C�e�����ɒʒm����܂��B
	 *	@note �t�H�[�J�X����`���Ă��܂��B
	 *	@param[in] P �`��p���B rect �́A�T�u�A�C�e�����܂ޔ͈́B
	 */
	virtual void OnItemPainted(const TDrawParam* P)
	{
		CRect rc = P->rect;
		if ( m_hasCheckBox && m_pPartsDrawer != NULL )
		{
			rc.left += m_checkboxWidth;
		}
		if ( ! m_drawersLine.IsEmpty() )
		{
			IDrawable* D = m_drawersLine[P->itemNo];
			if ( D != NULL )
			{
				D->Resize(rc.Size());
				D->Draw(P->dc, rc.left, rc.top);
			}
		}
		if ( (P->state & CDIS_FOCUS) != 0 )
		{
			_mark::DrawFocusMark(P->dc, rc, P->itemNo);
		}
	}

	/**
	 * [�ʒm] �w�i�`��ʒm.
	 *	@note �w�i��`�悷��^�C�~���O�Œʒm����܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 *	@param rect �`��͈�
	 */
	virtual void OnDrawBackground(HDC dc, const RECT& rect)
	{
		_mark::DrawBackColor(dc, rect);
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
		case LVM_DELETECOLUMN:
			{
				INDEX col = wParam;
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				if ( r != 0 )
				{
					loop ( i, m_drawersGlid )
					{
						m_drawersGlid[i].Remove(col);
					}
				}
				return r;
			}
			break;
		case WM_DESTROY:
			m_ResetContent();
			_mark::AllReset();
			if ( ::IsWindow(m_headerCtrl) )
			{
				m_headerCtrl.UnsubclassWindow();
			}
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
		ASSERT0( (_super::GetStyle() & LVS_TYPEMASK) == LVS_REPORT, "DrawingListCtrl", "�T�|�[�g�ł���̂� Report�^�C�v�݂̂ł��B" );
		m_CheckHeaderCtrl();
		_super::PreSubclassWindow();
	}

private:
	void m_CheckHeaderCtrl(void)
	{
		if ( ! ::IsWindow(m_headerCtrl) )
		{
			CHeaderCtrl* pHeaderCtrl = _super::GetHeaderCtrl();
			if ( pHeaderCtrl != NULL )
			{
				m_headerCtrl.SubclassWindow(*pHeaderCtrl);
			}
		}
	}
	CDrawingHeaderCtrl			m_headerCtrl;				///< �w�b�_�����̃R���g���[��
	CDrawersVectors				m_drawersGlid;				///< ���ꂼ��̍s�A�J�����̕`����
	CDrawersVector				m_drawersLine;				///< ���ꂼ��̍s�̕`����
	CAutoVectorT<INDEX>			m_absoluteIndexs;			///< �e�A�C�e���̐�΃C���f�b�N�X
	mutable CAutoVectorT<INDEX>	m_indexCache;				///< �e��΃C���f�b�N�X�̃A�C�e��
	IPartsDrawable*				m_pPartsDrawer;				///< �p�[�c�`��(�`�F�b�N�{�b�N�X�p)
	int							m_checkboxWidth;			///< �`�F�b�N�{�b�N�X�̕�(���[�N)
	bool						m_hasCheckBox;				///< �`�F�b�N�{�b�N�X�̗L��(���[�N)
	bool						m_withDtEndEllipsis;		///< DT_END_Ellipsis ����H

	/// �L���b�V���\��(�J���p)
	void m_ViewIndexCache(void) const
	{
		#ifdef _TnbDEBUG_ON
		m_MakeIndexCache();
		TRACE1( "LL;cache ���e (l = %d)\n", m_indexCache.GetSize() - 1 );
		loop ( i, m_indexCache.GetSize() )
		{
			if ( i != 0 )
			{
				TRACE2("LL;   ���INDEX %d  ==  itemNo %d\n", i, m_indexCache[i] - 1);
			}
		}
		#endif
	}
	/// INDEX�L���b�V���N���A
	void m_ClearIndexCache(void)
	{
		m_indexCache.RemoveAll();
	}
	/// INDEX�L���b�V���쐬
	void m_MakeIndexCache(void) const
	{
		if ( m_indexCache.IsEmpty() )
		{
			m_indexCache.Add(0);
			loop ( i, m_absoluteIndexs.GetSize() )
			{
				INDEX idx = m_absoluteIndexs[i];
				m_indexCache[idx] = i + 1;
			}
		}
	}
	/// ���g�p�̐�΃C���f�b�N�X�擾
	INDEX m_FindNoUseAbsoluteIndex(void) const
	{
		m_MakeIndexCache();
		INDEX r = m_indexCache.Find(0, 1);
		if ( r == INVALID_INDEX )
		{
			r = m_indexCache.GetSize();
		}
		return r;
	}
	int m_AbsoluteIndexToItemNo(INDEX index) const
	{
		m_MakeIndexCache();
		int r = static_cast<int>(m_indexCache[index]) - 1;
		ASSERT( r >= 0 );
		return r;
	}
	// �A�C�e���S����
	void m_ResetContent(void)
	{
		m_drawersGlid.RemoveAll();
		m_drawersLine.RemoveAll();
		m_absoluteIndexs.RemoveAll();
		m_ClearIndexCache();
	}
	/// �I���}�[�N�`��
	void m_DrawSelectMaker(HDC dc, const RECT& rect, UINT itemState, LPARAM lParam)
	{
		if ( (itemState & CDIS_SELECTED) != 0 )
		{
			_mark::DrawSelectMark(dc, rect, ::GetFocus() == m_hWnd, lParam);
		}
	}
	/// �A�C�e������ւ�
	void m_SwapItem(int itemNo1, int itemNo2)
	{
		// ������ւ�
		Swap(m_drawersGlid[itemNo1], m_drawersGlid[itemNo2]);
		Swap(m_drawersLine[itemNo1], m_drawersLine[itemNo2]);
		Swap(m_absoluteIndexs[itemNo1], m_absoluteIndexs[itemNo2]);
		DWORD data = ToDword(_super::GetItemData(itemNo1));
		_super::SetItemData(itemNo1, _super::GetItemData(itemNo2));
		_super::SetItemData(itemNo2, data);
		UINT state = _super::GetItemState(itemNo1, UINT_MAX);
		_super::SetItemState(itemNo1, _super::GetItemState(itemNo2, UINT_MAX), UINT_MAX);
		_super::SetItemState(itemNo2, state, UINT_MAX);
		// �e�J������������ւ�
		loop ( subItemNo, GetHeaderCtrl()->GetItemCount() )
		{
			int sin = ToInt(subItemNo);
			CString s = GetItemText(itemNo1, sin);
			SetItemText(itemNo1, sin, GetItemText(itemNo2, sin));
			SetItemText(itemNo2, sin, s);
		}
		m_ClearIndexCache();
	}

	CImageList* GetImageList(int nImageList) const;
	CImageList* SetImageList(CImageList* pImageList, int nImageListType);
	BOOL GetItem(LVITEM* pItem) const;
	BOOL SetItem(const LVITEM* pItem);
	BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
		int nImage, UINT nState, UINT nStateMask, LPARAM lParam);
	BOOL GetColumn(int nCol, LVCOLUMN* pColumn) const;
	BOOL SetColumn(int nCol, const LVCOLUMN* pColumn);
	BOOL GetOrigin(LPPOINT lpPoint) const;
	BOOL SetItemState(int nItem, LVITEM* pItem);
	BOOL SetBkImage(HBITMAP hbm, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
	BOOL SetBkImage(LPTSTR pszUrl, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
	BOOL SetBkImage(LVBKIMAGE* plvbkImage);
	int InsertItem(const LVITEM* pItem);
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	BOOL Update(int nItem);
	BOOL SortItems(PFNLVCOMPARE pfnCompare, DWORD dwData);
	int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState,
		UINT nStateMask, int nImage, LPARAM lParam);
};



}; // MFC
}; // TNB
