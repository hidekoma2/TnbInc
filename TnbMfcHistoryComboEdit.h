#pragma once
/**
 *@file
 * �t�@�C���I���_�C�A���O�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ����t�� EDIT�R���g���[��
 *
 *		����t����EDIT�R���g���[����񋟂��܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃR���{�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCComboBox�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@note �h���b�v�_�E���ɂ��A�\�[�g�� false �ɂ��܂��B
 *		
 *	@param EDT EDIT�R���g���[���̃N���X�B�ȗ������ CEdit
 *	@param CMB �R���{�{�b�N�X�R���g���[���̃N���X�B�ȗ������ CComboBox
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcHistoryComboEdit.h
 * 
 *	@date 09/06/30	�V�K�쐬
 */
template<typename EDT = CEdit, typename CMB = CComboBox>
class CHistoryComboEditT : public CMB
{
	DEFSUPER(CMB);
public:

	/// �R���X�g���N�^
	CHistoryComboEditT(void) : m_depth(10)
	{
	}

	/**
	 * [�ݒ�] ���𐔐ݒ�.
	 *		���������܂Ŏc�����A�ݒ肵�܂��B�ݒ肵�Ă��Ȃ��� 10 �ł��B
	 *	@param depth ����
	 */
	void SetHistoryDepth(int depth)
	{
		m_depth = depth;
	}

	/**
	 * [�擾] �S���𕶎���擾.
	 *	@return �S���𕶎���B
	 */
	CStrVector GetHistoryStrings(void) const
	{
		CString s;
		CStrVector vs;
		loop ( i, _super::GetCount() )
		{
			_super::GetLBText(i, s);
			vs.Add(CStr(s));
		}
		return vs;
	}

	/**
	 * [�擾] �S������擾.
	 *		�擪�� Edit�R���g���[���̕�����A�ȉ��A���𕶎��񂪕��т܂��B
	 *	@return �S������B
	 */
	CStrVector GetAllStrings(void) const
	{
		CStrVector vs;
		CString s;
		_super::GetWindowText(s);
		vs.Add(CStr(s));
		vs += GetHistoryStrings();
		return vs;
	}

	/**
	 * [�ݒ�] �S���𕶎���ݒ�.
	 *	@note Edit�R���g���[���͕ω����܂���B
	 *	@param vs �S���𕶎���B
	 */
	void SetHistoryStrings(const CStrVector& vs)
	{
		CString s;
		_super::GetWindowText(s);
		_super::ResetContent();
		loop ( i, vs.GetSize() )
		{
			_super::AddString(vs[i]);
		}
		m_CheckDepth();
		_super::SetWindowText(s);
	}

	/**
	 * [�ݒ�] �S������ݒ�.
	 *		GetAllStrings() �œ���������Q��ݒ肵�܂��B
	 *	@param vs �S������B
	 */
	void SetAllStrings(const CStrVector& vs)
	{
		_super::ResetContent();
		if ( vs.GetSize() > 0 )
		{
			loop ( i, vs.GetSize() - 1 )
			{
				_super::AddString(vs[i + 1]);
			}
			m_CheckDepth();
			_super::SetWindowText(vs[0]);
		}
	}

	/**
	 * [�ݒ�] �m��.
	 *		���ݕ\�����̕�����𗚗��Ɏc���܂��B
	 */
	void Apply(void)
	{
		CString s;
		_super::GetWindowText(s);
		if ( ! s.IsEmpty() )
		{
			int i = _super::FindStringExact(0, s);
			if ( i >= 0 )
			{
				_super::DeleteString(i);
			}
			_super::InsertString(0, s);
			m_CheckDepth();
			_super::SetWindowText(s);
		}
	}

	/**
	 * [�Q��] Edit�R���g���[���Q��.
	 *	@return Edit�R���g���[���Q��
	 */
	EDT& ReferEdit(void) { return m_edit; }

	/**
	 * [�Q��] Edit�R���g���[���Q��.
	 *	@return Edit�R���g���[���Q��
	 */
	const EDT& ReferEdit(void) const { return m_edit; }

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		DWORD s = GetStyle();
		ASSERT0( (s & CBS_DROPDOWN) != 0, "CHistoryComboEditT", "�h���b�v�_�E���ɂȂ��Ă��܂���B" );
		ASSERT0( (s & CBS_SORT) == 0, "CHistoryComboEditT", "�\�[�g���������Ă��܂��Ă��܂��B" );
		_super::PreSubclassWindow();
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
		case WM_DESTROY:
			if ( m_edit.GetSafeHwnd() != NULL )
			{
				m_edit.UnsubclassWindow();
			}
			break;
		case WM_CTLCOLOREDIT:
			if ( m_edit.GetSafeHwnd() == NULL )
			{
				m_edit.SubclassWindow(reinterpret_cast<HWND>(lParam));
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	//�@�[���`�F�b�N
	void m_CheckDepth(void)
	{
		if ( m_depth > 1 )
		{
			while ( _super::GetCount() > m_depth )
			{
				_super::DeleteString(_super::GetCount() - 1);
			}
		}
	}
	int		m_depth;	/// <�[��
	EDT		m_edit;		///< EDIT
};



/**@ingroup MFCCONTROL
 * ����t�� EDIT�R���g���[��
 *
 *		����t����EDIT�R���g���[����񋟂��܂��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃR���{�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCComboBox�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@note �h���b�v�_�E���ɂ��A�\�[�g�� false �ɂ��܂��B
 *		
 *	@par�K�v�t�@�C��
 *			TnbMfcHistoryComboEdit.h
 * 
 *	@date 09/06/30	�V�K�쐬
 */
typedef CHistoryComboEditT<> CHistoryComboEdit;



};
};