#pragma once

/** 
 * NoFoucsFreamButton
 *	@note �t�H�[�J�X�𓾂Ă��{�^�����肪�����Ȃ�܂���B
 */
class CButtonNfm : public CButton
{
	typedef CButton __super;
	//
	CBitmap*	m_pBmp;
	COLORREF	m_colorBack;
	//
	virtual void PreSubclassWindow(void)
	{
		UINT uiDefBtnSte = __super::GetButtonStyle()&0xFF;
		//
		if ( uiDefBtnSte==BS_PUSHBUTTON || uiDefBtnSte==BS_DEFPUSHBUTTON )
		{
			__super::ModifyStyle( 0, BS_OWNERDRAW);
		}
		__super::PreSubclassWindow();
	}

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
		dc->SaveDC();
		UINT uItemState = lpDrawItemStruct->itemState;
		//== �{�^���t���[���`��
		UINT uState = DFCS_BUTTONPUSH | DFCS_TRANSPARENT;
		if ( (uItemState & ODS_SELECTED)!=0 )
		{
			uState |= DFCS_PUSHED;
		}
//		if ( (uItemState & ODS_DISABLED)!=0 )
//		{
//			uState |= DFCS_MONO;
//		}
		dc->DrawFrameControl(&(lpDrawItemStruct->rcItem),DFC_BUTTON,uState);
		//===== ���gFILL
		if ( ((LONG)m_colorBack)>=0 )
		{
			CRect cRect = lpDrawItemStruct->rcItem;
			cRect.DeflateRect(2,2,2,2);
			dc->FillSolidRect(cRect,m_colorBack);
		}
		//
		if ( m_pBmp==NULL )
		{
			//== ����
			CString strText;
			GetWindowText(strText);
			if ( ! strText.IsEmpty() )
			{
				CRect cRect = lpDrawItemStruct->rcItem;
				cRect.DeflateRect(4,4,4,4);
				if ( (uItemState & ODS_SELECTED)!=0 )
				{
					cRect.OffsetRect(1,1);
				}
				dc->SetBkMode(TRANSPARENT);		// �o�b�N�J���[�͕ύX�Ȃ�
				if ( (uItemState & ODS_DISABLED)!=0 )
				{
					dc->SetTextColor(GetSysColor(COLOR_GRAYTEXT));	//�O���C
				}
				dc->DrawText(strText,cRect,DT_CENTER|DT_VCENTER);
			}	
		}
		else
		{
			//== BMP
			CRect cRect = lpDrawItemStruct->rcItem;
			cRect.DeflateRect(3,3,3,3);
			if ( (uItemState & ODS_SELECTED)!=0 )
			{
				cRect.OffsetRect(1,1);
			}
			CDC memDC;
			memDC.CreateCompatibleDC(dc);
			memDC.SelectObject(m_pBmp);
			dc->BitBlt( cRect.left,cRect.top,cRect.Width(),cRect.Height(),&memDC,0,0,SRCCOPY);
		}
		//== �t�H�[�J�X�_��
		if ( (uItemState & ODS_FOCUS)!=0 )
		{
			CRect cRect = lpDrawItemStruct->rcItem;
			cRect.DeflateRect(3,3,3,3);
			if ( (uItemState & ODS_SELECTED)!=0 )
			{
				cRect.OffsetRect(1,1);
			}
			dc->DrawFocusRect(&cRect);
		}
		dc->RestoreDC(-1);
	}
public:
	CButtonNfm(void)
	{
		m_pBmp = NULL;
		m_colorBack = -1;
	}
	void SetBitmap(CBitmap& bmp)
	{
		m_pBmp = &bmp;
	}
	void SetBkColor(COLORREF color=-1)
	{
		m_colorBack = color;
	}


};
