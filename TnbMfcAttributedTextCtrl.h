#pragma once
/**
 *@file
 * �����t���̃e�L�X�g
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbAttributedStr.h"
#include "TnbThread.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �e�L�X�g�R���g���[��
 *
 *		�ꕶ�����Ƃɑ��������邱�Ƃ̂ł���A�e�L�X�g�ł��B
 *		�i�����́A���݁A�m�[�}���A���]�A�_�ł݂̂ł��B�j
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcAttributedTextCtrl.h
 * 
 *	@date 06/10/10 �V�K�쐬
 *	@date 07/10/31 WM_PRINTCLIENT �Ή��B
 */
class CAttributedTextCtrl : public CStatic, TNB::CThread::IRunner
{
	DEFSUPER(CStatic);
public:

	/// ��������
	enum EAttr
	{
		NORMAL,		///< �W��
		REVERSE,	///< ���]
		BLINK		///< �_��
	};

	typedef CAttributedStrT<EAttr>	CAttrStr;		///< �����t��������

private:
	
	CThread			m_thread;				///< Blink�p�X���b�h
	bool			m_boIsNormalForBlink;	///< Blink�p�t���O
	CFont*			m_pfont;				///< �g�p����t�H���g
	CRect			m_rect;					///< �R���g���[���̑傫��
	CPoint			m_pointMargin;			///< �\���J�n�ʒu
	COLORREF		m_colorBack;			///< �w�i�F
	COLORREF		m_colorNormal;			///< �W�������F
	CSyncCounter	m_syncDraw;				///< �`��r��
	enum {
		BLINK_TIMING = 500					///< �u�����N�^�C�~���O
	};
	CAttrStr				m_content;		///< �\�����
	CVectorT<INDEX>			m_vdwTopPoss;	///< �s�g�b�v�|�W�V����

	/**
	 * �e�s�̃|�W�V�����v�Z
	 *	@note m_content ��ύX������K����������s
	 */
	void m_ResearchContent(void)
	{
		m_vdwTopPoss.RemoveAll();
		m_vdwTopPoss.Add(0);
		loop ( i, m_content.GetLength() )
		{
			if ( m_content.GetAt(i) == '\r' )
			{
				if ( m_content.GetAt(i + 1) == '\n' )
				{
					m_vdwTopPoss.Add(i + 2);
				}
				else
				{
					m_vdwTopPoss.Add(i + 1);
				}
			}
		}
	}

	/**
	 * �`��
	 *	@param dc �Ώۂ�DC
	 *	@param boIsForBlink true�Ȃ�Blink�ĕ`��p�ł��i��x�S�̂��������Ƃ����Ȃ��j�B
	 */
	void m_Draw(CDC* dc, bool boIsForBlink = false)
	{
		dc->SaveDC();
		CFont* pOldFont = dc->SelectObject(m_pfont);

		dc->IntersectClipRect(m_rect);

		if ( ! boIsForBlink )
		{
			dc->FillSolidRect(m_rect, m_colorBack);
		}
		LOGFONT tFont;
		m_pfont->GetLogFont(&tFont);
		dc->SetBkMode(OPAQUE/*TRANSPARENT*/);
		loop ( i, m_vdwTopPoss.GetSize() )
		{
			CRect r = m_rect;
			r.OffsetRect(m_pointMargin);
			r.OffsetRect(0, abs(static_cast<LONG>(tFont.lfHeight * i)));
			TCHAR c[3] = {0};
			UINT uChar;
			INDEX j = m_vdwTopPoss[i];
			while ( j < m_content.GetLength() )
			{
				EAttr a = m_content.GetAttribute(j);
				c[0] = m_content.GetAt(j++);
				if ( c[0] == '\n' || c[0] == 0 )
				{
					break;
				}
				if ( a == BLINK )
				{
					a = m_boIsNormalForBlink ? NORMAL : REVERSE;
				}
				if ( a == NORMAL )
				{
					dc->SetTextColor(m_colorNormal);
					dc->SetBkColor(m_colorBack);
				}
				else
				{
					dc->SetTextColor(m_colorBack);
					dc->SetBkColor(m_colorNormal);
				}
				uChar = c[0] & 0xff;
				if ( STRLIB::GetCharSize(c[0]) == 2 )
				{
					//- SJIS��
					c[1] = m_content.GetAt(j);
					uChar = (uChar << 8) | (c[1] & 0xFF);
					j++;
					dc->DrawText(c, 2, r, 0);
				}
				else
				{
					//- ASCII��
					dc->DrawText(c, 1, r, 0);
				}
				//
				ABC abc;
				VERIFY(dc->GetCharABCWidths(uChar, uChar, &abc));
				r.OffsetRect(abc.abcA + abc.abcB + abc.abcC, 0);
			}
		}
		dc->SelectObject(pOldFont);
		dc->RestoreDC(-1);
	}

	/**
	 * �X���b�h�����֐�
	 *	@note �u�����N�^�C�~���O�ŁA�t���O�𔽓]���Ă��܂��B
	 *			�`�撆�łȂ���`�悵�܂��B
	 *	@retval �O�i�Œ�)�B
	 */
	DWORD Run(void)
	{
		while ( IsRunnable() )
		{
			::Sleep(BLINK_TIMING);
			m_boIsNormalForBlink = ! m_boIsNormalForBlink;
			if ( m_syncDraw.Lock(0) )
			{
				if ( ::IsWindow(GetSafeHwnd()) )
				{
					HDC hdc = ::GetDC(m_hWnd);
					if ( hdc != NULL )
					{
						CDC dc;
						dc.Attach(hdc);
						m_Draw(&dc, true);
						hdc = dc.Detach();
						::ReleaseDC(m_hWnd, dc); 
					}
				}
				m_syncDraw.Unlock();
			}
		}
		return 0;
	}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if ( message == WM_PAINT )
		{
			CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
			if ( m_syncDraw.Lock(0) )
			{
				m_Draw(&dc);
				m_syncDraw.Unlock();
			}
			return 0;
		}
		else if ( message == WM_PRINTCLIENT )
		{
//			if ( (lParam & PRF_CLIENT) != 0 )
			{
				CDC* pDc = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				if ( m_syncDraw.Lock(0) )
				{
					m_Draw(pDc);
					m_syncDraw.Unlock();
				}
			}
			return 0;
		}
		else if ( message == WM_SETTEXT )
		{
			CAttrStr s(reinterpret_cast<LPCTSTR>(lParam));
			SetContent(s);
		}
		else if ( message == WM_DESTROY )
		{
			EXCLUSIVE(&m_syncDraw);
			return _super::WindowProc(message, wParam, lParam);
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
		_super::GetClientRect(m_rect);
		_super::PreSubclassWindow();
		CString s;
		_super::GetWindowText(s);
		m_content = s.operator LPCTSTR();
		m_ResearchContent();
		//
		CWnd* P = GetParent();
		if ( P != NULL )
		{
			m_pfont = P->GetFont();
		}
		m_thread.Start(_T("CAttributeTextCtrl Blink�p"));
	}

public:

	/// �R���X�g���N�^
	CAttributedTextCtrl(void)
		: _super()
		, m_colorBack(::GetSysColor(COLOR_MENU))
		, m_colorNormal(::GetSysColor(COLOR_MENUTEXT))
		, m_boIsNormalForBlink(false)
		, m_pointMargin(0, 0)
	{
		m_thread.SetRunner(this);
	}

	/// �f�X�g���N�^
	~CAttributedTextCtrl(void)
	{
		m_thread.Stop();
	}

	/**
	 * [�ݒ�] �}�[�W���ݒ�
	 *	@param x ���[�̃}�[�W��
	 *	@param y ��[�̃}�[�W��
	 */
	void SetMargin(int x, int y)
	{
		m_pointMargin.x = x;
		m_pointMargin.y = y;
	}

	/**
	 * [�ݒ�] �t�H���g�ݒ�
	 *	@note MFC �� CWnd::SetFont() �Ɠ����ł��B
	 *	@attention �ݒ肵���t�H���g�͔j�����Ȃ��ŉ������B
	 *	@param pFont �t�H���g�̎Q�ƁB
	 *	@param bRedraw TRUE �̏ꍇ�� CWnd �I�u�W�F�N�g���ĕ`�悵�܂��B
	 */
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE)
	{
		m_pfont = pFont;
		_super::SetFont(pFont, bRedraw);
	}

	/**
	 * [�擾] ���e�擾
	 *	@return �����t��������
	 */
	const CAttrStr& GetContent(void) const
	{
		return m_content;
	}

	/**
	 * [�ݒ�] ���e�ꊇ�ݒ�
	 *	@param as �����t��������
	 */
	void SetContent(const CAttrStr& as)
	{
		m_content = as;
		m_ResearchContent();
		RedrawWindow();
	}

	/**
	 * [�ݒ�] ���e�A��
	 *	@param as �A�����鑮���t��������
	 */
	void AddContent(const CAttrStr& as)
	{
		m_content += as;
		m_ResearchContent();
		RedrawWindow();
	}

	/**
	 * [�ݒ�] ���e�}��
	 *	@param iIndex �}������C���f�b�N�X
	 *	@param as �A�����鑮���t��������
	 */
	void InsertContent(int iIndex, const CAttrStr& as)
	{
		m_content.Insert(iIndex, as);
		m_ResearchContent();
		RedrawWindow();
	}

	/**
	 * [�ݒ�] ���e�ύX
	 *	@param iIndex �ύX����C���f�b�N�X
	 *	@param c �����R�[�h
	 *	@param attr ����
	 */
	void SetAt(int iIndex, TCHAR c, EAttr attr = NORMAL)
	{
		m_content.SetAt(iIndex, c, attr);
	}
};



}; //MFC
}; //TNB

