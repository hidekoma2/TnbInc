#pragma once
/**
 *@file
 * �_�C�A���O�o�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �_�C�A���O�o�[�g���x�[�X�N���X.
 *
 *		�_�C�A���O�o�[���쐬����ꍇ�A CDialogBar �̑���ɂ�����g�p����ƁA
 *		Dialog���l�� Class Wizerd ��p�����ҏW��A�T�C�Y�̒������T�|�[�g�ł��܂��B
 *
 *	@par�g�p��
 *		�܂��A���\�[�X�Ń_�C�A���O���쐬���܂��B
 *		Class Wizerd �� CDialog ��e�N���X�ɂ����T�u�N���X���쐬���܂��B
 *		���̃T�u�N���X .h / .cpp �� CDialog �����ׂ� CDialogBarEx �ɒu�������܂��B\n
 *		OnInitDialog() �͌Ă΂�܂���B����� BOOL OnInitDialogBar(void) ���������Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDialogBarEx.h
 * 
 *	@date 08/09/20 �V�K�쐬
 *	@date 10/05/19 �R�����g����	
 */
class CDialogBarEx : public CDialogBar
{
	DEFSUPER(CDialogBar);
public:

	/// �R���X�g���N�^
	CDialogBarEx(...) : m_bChangeDockedSize(false), m_sizeDocked(0, 0), m_sizeFloating(0, 0)
	{
	}

	/// �f�X�g���N�^
	virtual ~CDialogBarEx()
	{
	}

	/**
	 * [�쐬] �_�C�A���O�o�[�쐬.
	 *	@param pParentWnd �e�E�B���h�E
	 *	@param lpszTemplateName ���\�[�X�e���v���[�g��.
	 *	@param nStyle �X�^�C��
	 *	@param nID �R���g���[��ID
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
	 */
	BOOL Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID)
	{
		if ( ! _super::Create(pParentWnd, lpszTemplateName, nStyle, nID) )
		{
			return FALSE;
		}
		m_bChangeDockedSize = false;
		m_sizeFloating = m_sizeDocked = m_sizeDefault;
		return OnInitDialogBar();
	}

	/**
	 * [�쐬] �_�C�A���O�o�[�쐬.
	 *	@param pParentWnd �e�E�B���h�E
	 *	@param nIDTemplate ���\�[�X�e���v���[�gID.
	 *	@param nStyle �X�^�C��
	 *	@param nID �R���g���[��ID
	 *	@retval TRUE ����
	 *	@retval FALSE ���s
	 */
	BOOL Create(CWnd * pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
	{
		return Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, nID);
	}

	/**
	 * [�ݒ�] �h�b�L���O���T�C�Y���[�h�ݒ�.
	 *	@param isResized false �Ȃ�f�t�H���g�̑傫���A true �Ȃ烊�T�C�Y�����傫���ɂȂ�܂��B
	 */
	void SetDockedSizeMode(bool isResized)
	{
		m_bChangeDockedSize = isResized;
	}

protected:

	/**
	 * [�ʒm] �_�C�A���O�o�[�������ʒm
	 *	@note �_�C�A���O�o�[���쐬���ꂽ���ʒm����܂��B������I�[�o�[���[�h���邱�ƂŁA
	 *			�R���g���[�����̏��������o���܂��B
	 *			�I�[�o�[���[�h�����ہA�ŏ��ɂ��̃��\�b�h���R�[�����Ă��������B
	 *	@retval true �����B
	 *	@retval false ���s�B�_�C�A���O�o�[�͍쐬�ł��܂���B
	 */
	virtual BOOL OnInitDialogBar(void)
	{
		UpdateData(FALSE);
		return TRUE;
	}

	/**
	 * [�v�Z] �T�C�Y�v�Z 
	 *	@note ���ڎg�p���邱�Ƃ͂���܂���B
	 *	@param nLength ���@�inMode �ɂ��A�����A�����̂����ꂩ�������j�B
	 *	@param nMode ���[�h�B LM_STRETCH , LM_HORZ �Ȃǂ� OR �ŕ����w��ł��܂��B
	 *	@return �T�C�Y�B
	 */
	CSize CalcDynamicLayout(int nLength, DWORD nMode)
	{
		if ( (nMode & LM_VERTDOCK) != 0 || (nMode & LM_HORZDOCK) != 0 )
		{
			if ( (nMode & LM_STRETCH) != 0 )
			{
				return CSize((nMode & LM_HORZ) ? 32767 : m_sizeDocked.cx,
							(nMode & LM_HORZ) ? m_sizeDocked.cy : 32767);
			}
			else
			{
				return m_sizeDocked;
			}
		}
		if ( (nMode & LM_MRUWIDTH) != 0 )
		{
			return m_sizeFloating;
		}
		if ( (nMode & LM_LENGTHY) != 0 )
		{
			return CSize(m_sizeFloating.cx, (m_bChangeDockedSize) ?
						m_sizeFloating.cy = m_sizeDocked.cy = nLength :
						m_sizeFloating.cy = nLength);
		}
		return CSize((m_bChangeDockedSize) ?
						m_sizeFloating.cx = m_sizeDocked.cx = nLength :
						m_sizeFloating.cx = nLength, m_sizeFloating.cy);
	}

	/**
	 * [�v�Z] ���������T�C�Y�v�Z 
	 *	@note ���ڎg�p���邱�Ƃ͂���܂���B
	 *	@param bStretch �o�[���t���[���̃T�C�Y�Ɋg�傷�邩�ǂ����������܂��B
	 *					�o�[���h�b�L���O �o�[�ł͂Ȃ� (�h�b�L���O�����p�ł��Ȃ�) �ꍇ�́A�p�����[�^ bStretch �� 0 �ȊO�ɐݒ肵�A
	 *					�h�b�L���O�܂��̓t���[ (�h�b�L���O�����p�ł���) �̏ꍇ�́A0 ��ݒ肵�܂��B	
	 *	@param bHorz �o�[�����������������������������܂��B�o�[�����������̂Ƃ��̓p�����[�^ bHorz �� 0 �ȊO�ɐݒ肵�A
	 *					���������̂Ƃ��� 0 ��ݒ肵�܂��B
	 *	@return �T�C�Y�B
	 */
	CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
	{
		return _super::CalcFixedLayout(bStretch, bHorz);
	}

	/**
	 * [����] ��ԍX�V�ʒm
	 *	@note �{�N���X�ł̓R���g���[�����L���ɂȂ�悤�ɐݒ肵�Ă��܂��B
	 *	@note ���ڎg�p���邱�Ƃ͂���܂���B
	 *	@param pTarget ���C���E�B���h�E�̃|�C���^�B 
	 *	@param bDisableIfNoHndler �_�~�[�i�l�͎Q�Ƃ��܂���j
	 */
	void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CDialogBar::OnUpdateCmdUI(pTarget, false);
	}

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	//{{AFX_VIRTUAL(CTrickDialogBar)
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTrickDialogBar)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	#endif // _TnbDOXYGEN

	CSize m_sizeDocked;			///< �h�b�L���O���̃T�C�Y
	CSize m_sizeFloating;		///< �t���[�e�B���O���̃T�C�Y
	BOOL m_bChangeDockedSize;	/**<�h�b�L���O���̃T�C�Y���[�h�B
									false �Ȃ�f�t�H���g�̑傫���A true �Ȃ烊�T�C�Y�����傫���ɂȂ�܂��B*/
};



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

inline void CDialogBarEx::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);
	_super::DoDataExchange(pDX);
}

#ifdef __MSVCPP6__

inline const AFX_MSGMAP* PASCAL CDialogBarEx::_GetBaseMessageMap()
{
	return &CDialogBar::messageMap; 
}

inline const AFX_MSGMAP* CDialogBarEx::GetMessageMap() const
{
	return &CDialogBarEx::messageMap; 
}

_declspec(selectany) AFX_COMDAT AFX_DATADEF const AFX_MSGMAP CDialogBarEx::messageMap =
{
	&CDialogBarEx::_GetBaseMessageMap, &CDialogBarEx::_messageEntries[0] 
};

_declspec(selectany) AFX_COMDAT const AFX_MSGMAP_ENTRY CDialogBarEx::_messageEntries[] =
{
	{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
};

#else // __MSVCPP6__

	inline const AFX_MSGMAP* CDialogBarEx::GetMessageMap() const
		{ return GetThisMessageMap(); }
	inline const AFX_MSGMAP* PASCAL CDialogBarEx::GetThisMessageMap()
	{
		static const AFX_MSGMAP_ENTRY _messageEntries[] =
		{
			{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } 
		}; 
		static const AFX_MSGMAP messageMap = 
		{ &CDialogBar::GetThisMessageMap, &_messageEntries[0] }; 
		return &messageMap; 
	}								  

#endif // __MSVCPP6__

#endif // _TnbDOXYGEN



}; //MFC
}; //TNB


