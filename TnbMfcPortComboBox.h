#pragma once
/**
 *@file
 * �V���A���|�[�g�I���R���{�{�b�N�X�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbRs232c.h"
#include "TnbSimpleMap.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �V���A���|�[�gComboBox�R���g���[��
 *
 *
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃR���{�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCComboBox�j��{�N���X�ɒu�������Ďg�p���܂��B\n
 *		�R���{�{�b�N�X�̃v���p�e�B�́A�h���b�v�_�E�����X�g�ɂ��Ă����Ă��������B
 *	
 *	@par�g�p��
 *	\code
 *		#include <dbt.h>
 *
 *		class CFooDlg : public CDialog
 *		{
 *				:
 *			CPortComboBox m_comboPort;
 *				:
 *			// �������ꂽ���b�Z�[�W �}�b�v�֐�
 *			//{{AFX_MSG(CGpsReporterDlg)
 *				:
 *			afx_msg BOOL OnDeviceChange(UINT, DWORD);
 *			//}}AFX_MSG
 *			DECLARE_MESSAGE_MAP()
 *		}
 *
 *
 *		BEGIN_MESSAGE_MAP(CFooDlg, CDialog)
 *			//{{AFX_MSG_MAP(CFooDlg)
 *				:
 *			ON_WM_DEVICECHANGE()
 *				:
 *			//}}AFX_MSG_MAP
 *		END_MESSAGE_MAP()
 *
 *		BOOL CFooDlg::OnInitDialog()
 *		{
 *				:
 *			m_comboPort.ResetDevice();
 *				:
 *		}
 *
 *		BOOL CFooDlg::OnDeviceChange(UINT type, DWORD data)
 *		{
 *			if ( type == DBT_DEVICEARRIVAL || type == DBT_DEVICEREMOVECOMPLETE )
 *			{
 *				CWaitCursor aa;
 *				m_comboPort.ResetDevice();
 *			}
 *			return TRUE;
 *		}
 *
 *	\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcPortComboBox.h
 * 
 *	@date 10/02/18	�V�K�쐬
 *	@date 14/07/14	AddExtendPort() ��V�K�ǉ�
 *	@date 18/11/05	IsEffectiveness() ��V�K�ǉ�
 */
class CPortComboBox : public CComboBox
{
	DEFSUPER(CComboBox);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param isFullText true �Ȃ�h������ description (COM�|�[�g�ԍ�)�h�ɂȂ�܂��B�ȗ������ �hCOM�|�[�g�ԍ��h �ɂȂ�܂��B
	 */
	CPortComboBox(bool isFullText = false) : m_isFullText(isFullText)
	{
	}

	/**
	 * [�ݒ�] �g���|�[�g���ݒ�.
	 *	@note ���̖��O��ݒ肷��ƑI�����Ɍ���APort 0 �Ƃ��ċ@�\����悤�ɂȂ�܂��B
	 *	@attention ���̃��\�b�h���g�����ꍇ�A�K���I������Ă���|�[�g���O���ۂ��m�F���Ă���V���A���|�[�g���g�p���Ă��������B
	 */
	void AddExtendPort(LPCTSTR lpszName)
	{
		m_extPort = lpszName;
	}

	/**
	 * [�擾] �|�[�g���擾.
	 *		�I�����Ă���|�[�g��Ԃ��܂��B
	 *	@return �|�[�g
	 */
	operator CRs232c::CPort(void) const
	{
		return GetCurPort();
	}

	/**
	 * [�擾] �|�[�g���擾.
	 *		�I�����Ă���|�[�g��Ԃ��܂��B
	 *	@return �|�[�g
	 */
	int GetCurPort(void) const
	{
		int f = _super::GetCurSel();
		if ( f >= 0 )
		{
			f = _super::GetItemData(f);
		}
		return f;
	}

	/**
	 * [�I��] �|�[�g�I��.
	 *		�w��̃|�[�g��I�����܂��B
	 *	@param port �|�[�g
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetCurPort(int port)
	{
		loop ( i, _super::GetCount() )
		{
			if ( ToInt(_super::GetItemData(i)) == port )
			{
				bool r = (_super::SetCurSel(i) != CB_ERR);
				if ( r )
				{
					m_toolTip.UpdateTipText(m_descList[_super::GetItemData(_super::GetCurSel())], this);
				}
				return r;
			}
		}
		return false;
	}

	/**
	 * [�I��] �I��.
	 *	@param nSelect �I�����镶����� 0 ����n�܂�C���f�b�N�X�Ŏw�肵�܂��B
	 *	@retval CB_ERR�ȊO �����B
	 *	@retval CB_ERR  ���s�B
	 */
	int SetCurSel(int nSelect)
	{
		int r = _super::SetCurSel(nSelect);
		int f = _super::GetCurSel();
		if ( f >= 0 )
		{
			f = _super::GetItemData(f);
			m_toolTip.UpdateTipText(m_descList[f], this);
		}
		return r;
	}

	/**
	 * [�ݒ�] �f�o�C�X�ݒ�.
	 *		�R���g���[���쐬���A�y�сA�g�b�v�E�B���h�E�� WM_CHANGEDEVICE ���󂯎�������ȂǁA�f�o�C�X��Ԃ��ω��������Ɏg�p���܂��B
	 *	@param isFullText true �Ȃ�h������ description (COM�|�[�g�ԍ�)�h�ɂȂ�܂��B�ȗ������ �hCOM�|�[�g�ԍ��h �ɂȂ�܂��B
	 */
	void ResetDevice(bool isFullText = false)
	{
		CRs232c::TDeviceVector vd = CRs232c::EnumExistPortDeviceNames();
		ResetDevice(vd, isFullText);
	}

	/**
	 * [�ݒ�] �f�o�C�X�ݒ�.
	 *		�R���g���[���쐬���A�y�сA�g�b�v�E�B���h�E�� WM_CHANGEDEVICE ���󂯎�������ȂǁA�f�o�C�X��Ԃ��ω��������Ɏg�p���܂��B
	 *	@param vd �f�o�C�X���
	 *	@param isFullText true �Ȃ�h������ description (COM�|�[�g�ԍ�)�h�ɂȂ�܂��B�ȗ������ �hCOM�|�[�g�ԍ��h �ɂȂ�܂��B
	 */
	void ResetDevice(const CRs232c::TDeviceVector& vd, bool isFullText = false)
	{
		m_isFullText = isFullText;
		_super::SetRedraw(FALSE);
		int f = GetCurPort();
		m_Reset(vd);
		SetCurPort(f);
		_super::SetRedraw(TRUE);
		_super::RedrawWindow();
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
		if ( ::IsWindow(m_toolTip) )
		{
			m_toolTip.RelayEvent(pMsg);
		}
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
		if ( message == WM_DESTROY )
		{
			m_descList.RemoveAll();
			m_toolTip.DestroyWindow();
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
		DWORD style = _super::GetStyle();
		ASSERT0( (style & 3) == CBS_DROPDOWNLIST, "CPortComboBox", "DropDownList�ɂȂ��Ă��܂���" );
		_super::PreSubclassWindow();
//		m_Reset();
	}
	
	/**
	 * [�ʒm] for notifications from parent
	 *	@note	CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *			���b�Z�[�W��M������R�[������܂��B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE �����ρB
	 *	@retval FALSE �������B
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_COMMAND )
		{
			if ( lParam == reinterpret_cast<LPARAM>(_super::GetSafeHwnd()) && HIWORD(wParam) == CBN_SELCHANGE )
			{
				ASSERT ( LOWORD(wParam) == _super::GetDlgCtrlID() );
				int f = _super::GetCurSel();
				if ( f >= 0 )
				{
					f = _super::GetItemData(f);
					m_toolTip.UpdateTipText(m_descList[f], this);
				}
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	/**
	 * [�I��] �L���|�[�g�`�F�b�N
	 *	@param port �|�[�g�ԍ�
	 *	@retval true �L��
	 *	@retval false �����i���X�g�Ɋ܂܂�܂���j
	 */
	virtual bool IsEffectiveness(int port)
	{
		return true;
	}

private:

	void m_Reset(const CRs232c::TDeviceVector& vd)
	{
		_super::ResetContent();
		m_descList.RemoveAll();
		m_toolTip.DestroyWindow();
		m_toolTip.Create(this, TTS_ALWAYSTIP);
		bool hasExtPort = ! m_extPort.IsEmpty();
		if ( ! vd.IsEmpty() )
		{
			CStr s;
			loop ( i, vd )
			{
				int p = vd[i].port;
				if ( IsEffectiveness(p) )
				{
					s.Format(_T("COM%d"), p);
					if ( m_isFullText )
					{
						s += _T(": ") + vd[i].ToString();
					}
					int f = _super::AddString(s);
					_super::SetItemData(f, p);
					m_descList[p] = vd[i].ToString();
				}
			}
			_super::SetCurSel(0);
			m_toolTip.AddTool(this, m_descList[_super::GetItemData(0)]);
		}
		else if ( ! hasExtPort )
		{
			m_toolTip.AddTool(this, _T("���������܂���"));
		}
		//
		if ( hasExtPort )
		{
			int f = _super::AddString(m_extPort);
			_super::SetItemData(f, 0);
			m_descList[0] = m_extPort;
		}
		m_toolTip.Activate(TRUE);
	}

	CSimpleMapT<int, CStr>	m_descList;		///< DESC�ꗗ int �� port
	CToolTipCtrl			m_toolTip;		///< �c�[���`�b�v
	bool					m_isFullText;	///< �t�H�[�}�b�g
	CStr					m_extPort;		///< �g���|�[�g��
};



}; // MFC
}; // TNB
