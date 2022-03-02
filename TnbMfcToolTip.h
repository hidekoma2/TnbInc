#pragma once
/**
 *@file
 * �c�[���`�b�v�������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleMap.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �c�[���`�b�v����N���X
 *
 *		CDialog �x�[�X�̃N���X�ɃA�h�C�����邱�ƂŁA�c�[���`�b�v�����e�Ղɂ��܂��B
 *
 *		������g���ƁA�R���g���[��ID�Ɠ��������񃊃\�[�X���c�[���`�b�v�Ƃ��ĕ\������܂�
 *		�i�܂��A���\�[�X�Ƃ͕ʂ̕�������\���\�j�B
 *
 *	@param TYP �v�f�̌^�B CDialog �̃T�u�N���X�ł���K�v������܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre> 
 *
 *		</pre></td></tr></table>
 *		
 *	@par�K�v�t�@�C��
 *			TnbMfcToolTip.h
 * 
 *	@date 09/04/13	�V�K�쐬
 *	@date 09/04/22	CSimpleVector ���� CSimpleMap �ɕύX�B
 */
template<typename TYP = CDialog>
class CToolTipAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// �R���X�g���N�^
	CToolTipAddinT(UINT id, CWnd* pParent) : _super(id, pParent), m_pActiveWnd(NULL), m_activeId(-1)
	{
	}

	/**
	 * [�Q��] �c�[���`�b�v�R���g���[���Q��.
	 *	@return �c�[���`�b�v�R���g���[��.
	 */
	CToolTipCtrl& ReferToolTipCtrl(void)
	{
		m_TipCreate();
		return m_toolTip;
	}

	/**
	 * [�ǉ�] �c�[���`�b�v�ǉ�.
	 *	@param P �E�B���h�E
	 *	@param lpsz �\�����镶����B�ȗ�����ƁA P �̃R���g���[��ID �Ɠ���ID�̕����񃊃\�[�X���g���܂��B
	 */
	void AddToolTip(CWnd* P, LPCTSTR lpsz = NULL)
	{
		if ( P != NULL )
		{
			m_TipCreate();
			m_toolTip.AddTool(P);
		}
		m_strMap[P->GetDlgCtrlID()] = lpsz;
	}

	/**
	 * [�ǉ�] �c�[���`�b�v�ǉ�.
	 *	@param ctrlId �R���g���[�� ID�B
	 *	@param lpsz �\�����镶����B�ȗ�����ƁA ctrlId �Ɠ���ID�̕����񃊃\�[�X���g���܂��B
	 */
	void AddToolTip(UINT ctrlId, LPCTSTR lpsz = NULL)
	{
		CWnd* P = GetDlgItem(ctrlId);
		if ( P != NULL )
		{
			m_TipCreate();
			m_toolTip.AddTool(P);
		}
		m_strMap[ctrlId] = lpsz;
	}

protected:
	/**
	 * [�ʒm] for translating Windows messages in main message pump
	 *	@param pMsg ���b�Z�[�W
	 *	@retval TRUE �f�B�X�p�b�`�ł��Ȃ��ꍇ
	 *	@retval FALSE �f�B�X�p�b�`����K�v������ꍇ
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		m_toolTip.RelayEvent(pMsg);
		return _super::PreTranslateMessage(pMsg);
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
		switch ( message )
		{
		case WM_INITDIALOG:
			m_TipCreate();
			m_toolTip.Activate(TRUE);
//			m_toolTip.AddTool(this);
			break;
		case WM_DESTROY:
			m_toolTip.DestroyWindow();
			break;
		case 0x36A:
			if ( m_pActiveWnd != NULL )
			{
				INT_PTR i = m_TestNowPosItem(m_pActiveWnd);
				if ( i < 0 )
				{
					i = m_pActiveWnd->GetDlgCtrlID();
				}
				if ( i != m_activeId )
				{
					m_toolTip.Pop();
				}
			}
			break;
		case WM_NOTIFY:
			{
				NMHDR* pNMHDR = reinterpret_cast<NMHDR*>(lParam);
				if ( pNMHDR->code == TTN_POP )
				{
					m_pActiveWnd = NULL;
					m_activeId = -1;
				}
				else if ( pNMHDR->code == TTN_NEEDTEXT )
				{
					UINT_PTR id = pNMHDR->idFrom;
					TOOLTIPTEXT* P = reinterpret_cast<TOOLTIPTEXT *>(pNMHDR);
					if ( (P->uFlags & TTF_IDISHWND) != 0 )
					{
						CWnd* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(id));
						INT_PTR i = m_TestNowPosItem(pWnd);
						if ( i < 0 )
						{
							id = pWnd->GetDlgCtrlID();
						}
						else
						{
							id = i;
						}
						if ( id != 0 )
						{
							CSimpleStr s;
							if ( m_Find(s, id) )
							{
								m_workStr = s;
								P->lpszText = m_workStr;
								P->hinst = 0;
							}
							else
							{
								P->lpszText = MAKEINTRESOURCE(id);
								P->hinst = AfxGetResourceHandle();
							}
							m_pActiveWnd = pWnd;
							m_activeId = id;
							return TRUE ;
						}
					}
				}
			}
			break;
		default:
//			TRACE1("msg = 0x%X\n", message);
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	CToolTipCtrl						m_toolTip;		///< �c�[���`�b�v�X
	CSimpleStr							m_workStr;		///< ���[�N�p
	CSimpleMapT<UINT_PTR, CSimpleStr>	m_strMap;			///< ������
	CWnd*								m_pActiveWnd;	///< Tip�\�����Ώ�Window
	INT_PTR								m_activeId;		///< Tip�\����ID
	/// TIP�R���g���[���쐬
	void m_TipCreate(void)
	{
		if ( ! ::IsWindow(m_toolTip) )
		{
			m_toolTip.Create(this, TTS_ALWAYSTIP);
		}
	}
	/// ���݂̍��W��ID
	INT_PTR m_TestNowPosItem(CWnd* pWnd)
	{
		TOOLINFO ti = { sizeof(TOOLINFO) };
		POINT po = { 0, 0 };
		::GetCursorPos(&po);
		pWnd->ScreenToClient(&po);
		return pWnd->OnToolHitTest(po, &ti);
	}
	/// ����
	bool m_Find(CSimpleStr&_s, UINT_PTR id)
	{
		if ( m_strMap.HasKey(id) )
		{
			_s = m_strMap[id];
			return ! _s.IsEmpty();
		}
		return false;
	}
};



}; //MFC
}; //TNB



